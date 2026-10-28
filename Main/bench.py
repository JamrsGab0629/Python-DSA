"""
benchmark.py - checks if your PC is fast enough for the models, and if Taglish search works.

Run (needs internet the first time, to download the models):
    python benchmark.py

It takes about 5 to 15 minutes on an older PC. It saves a summary to benchmark_results.txt
- send me that file or paste its contents.
"""
import platform
import random
import sys
import time

import numpy as np

# (name shown, model id, prefix for searches, prefix for documents)
MODELS = [
    ("English small (MiniLM-L6)", "sentence-transformers/all-MiniLM-L6-v2", "", ""),
    ("Multilingual small (e5)", "intfloat/multilingual-e5-small", "query: ", "passage: "),
]
SEQ_LENGTHS = [128, 256]      # how many tokens of each text piece the model reads
N_CHUNKS = 200                # pieces timed in the speed test
BATCH = 16
PROJECT_TO = 5000             # "how long for this many pieces?"

DOCS = {
    1: "Ang bulkan ay nabubuo kapag umaakyat ang magma mula sa ilalim ng lupa at sumasabog ang lava at abo. Ang Bulkang Mayon sa Albay ay isa sa pinakamaaktibong bulkan sa Pilipinas.",
    2: "Photosynthesis is how plants turn sunlight, water and carbon dioxide into glucose and oxygen inside the chloroplasts of leaf cells.",
    3: "Budget ko ngayong buwan: pagkain 1500 pesos, pamasahe 800 pesos, school projects 500 pesos, at ipon 700 pesos.",
    4: "Thesis schedule: chapter 1 draft is due on November 5 and the defense panel is on December 12. The adviser wants weekly progress reports every Friday.",
    5: "Paalala sa lahat ng estudyante: ang pagsusulit sa Math ay sa Lunes, saklaw ang mga lesson tungkol sa fractions at decimals. Magdala ng calculator at ballpen.",
    6: "Mga paalala kapag may bagyo: ihanda ang go bag, i-charge ang cellphone, at alamin ang pinakamalapit na evacuation center sa barangay ninyo sa Bulacan.",
    7: "Recipe for chicken adobo: simmer chicken in soy sauce, vinegar, garlic, bay leaves and peppercorns until tender, then fry it lightly for a crispy edge.",
    8: "Report sa Science: ang tubig sa ilog ay nagiging marumi dahil sa basura at kemikal. Kailangan natin ng clean-up drive at tamang pagtatapon ng basura sa Pandi at Balagtas.",
    9: "Programming notes: a loop repeats a block of code, while an array stores many values in one variable. Practice with for loops and nested loops.",
    10: "Listahan ng gagawin sa org: mag-print ng certificate, maghanda ng attendance sheet, at i-send ang invitation sa lahat ng members bago mag-Friday.",
}
QUERIES = [  # (search text, id of the document that should come first)
    ("notes tungkol sa bulkan", 1),
    ("volcano eruption lava", 1),
    ("how do plants make oxygen", 2),
    ("paano gumagana ang photosynthesis", 2),
    ("magkano ang pamasahe at baon ko", 3),
    ("kailan ang due ng chapter 1", 4),
    ("thesis defense date", 4),
    ("may exam ba sa math sa lunes", 5),
    ("ano dapat gawin pag may bagyo", 6),
    ("typhoon preparation checklist", 6),
    ("how to cook adobo", 7),
    ("maruming ilog at basura", 8),
    ("what is a loop in coding", 9),
    ("ano pa ang kulang sa certificate at attendance", 10),
]

lines = []


def say(text=""):
    print(text, flush=True)
    lines.append(text)


def machine_info():
    say("=== Your machine ===")
    say(f"Python {sys.version.split()[0]} on {platform.system()} {platform.release()} ({platform.machine()})")
    say(f"CPU: {platform.processor() or 'unknown'}")
    try:
        import torch
        say(f"PyTorch {torch.__version__}, threads used: {torch.get_num_threads()}")
        try:
            say(f"CPU instruction level PyTorch uses: {torch.backends.cpu.get_cpu_capability()}")
        except Exception:
            pass
    except Exception as e:
        say(f"PyTorch not available: {e}")
    say()


def make_chunks(n):
    """Fake text pieces of about 150 words, built from the test documents."""
    rng = random.Random(1)
    words = " ".join(DOCS.values()).split()
    return [" ".join(rng.choice(words) for _ in range(150)) for _ in range(n)]


def speed_test(model, p_prefix, chunks, seq_len):
    model.max_seq_length = seq_len
    texts = [p_prefix + c for c in chunks]
    model.encode(texts[:8], batch_size=BATCH, normalize_embeddings=True)       # warm-up
    t = time.perf_counter()
    model.encode(texts, batch_size=BATCH, normalize_embeddings=True)
    secs = time.perf_counter() - t
    rate = len(texts) / secs
    return rate, PROJECT_TO / rate / 60


def quality_test(model, q_prefix, p_prefix):
    ids = list(DOCS)
    d = model.encode([p_prefix + DOCS[i] for i in ids], normalize_embeddings=True)
    top1 = top3 = 0
    misses = []
    for text, want in QUERIES:
        q = model.encode([q_prefix + text], normalize_embeddings=True)[0]
        order = [ids[i] for i in np.argsort(-(d @ q))]
        top1 += order[0] == want
        top3 += want in order[:3]
        if order[0] != want:
            misses.append(text)
    return top1, top3, misses


def main():
    machine_info()
    chunks = make_chunks(N_CHUNKS)
    summary = []

    for name, model_id, qp, pp in MODELS:
        say(f"=== {name} ===")
        try:
            from sentence_transformers import SentenceTransformer
            t = time.perf_counter()
            model = SentenceTransformer(model_id)
            say(f"Model loaded in {time.perf_counter() - t:.1f} s (after the first download)")
            row = {"name": name}
            for L in SEQ_LENGTHS:
                rate, mins = speed_test(model, pp, chunks, L)
                say(f"  reads {L} tokens per piece: {rate:.1f} pieces/sec  ->  {PROJECT_TO} pieces in about {mins:.1f} min")
                row[L] = (rate, mins)
            top1, top3, misses = quality_test(model, qp, pp)
            say(f"  Search test ({len(QUERIES)} English/Tagalog/Taglish queries): "
                f"right file first {top1}/{len(QUERIES)}, in top 3: {top3}/{len(QUERIES)}")
            for m in misses:
                say(f'     missed: "{m}"')
            row["q"] = (top1, top3)
            summary.append(row)
        except Exception as e:
            say(f"  FAILED: {type(e).__name__}: {e}")
        say()

    say("=== Summary ===")
    for r in summary:
        parts = [f"{L} tokens: {r[L][1]:.1f} min per {PROJECT_TO} pieces" for L in SEQ_LENGTHS]
        say(f"{r['name']}: " + "; ".join(parts) + f"; search test {r['q'][0]}/{len(QUERIES)} first, {r['q'][1]}/{len(QUERIES)} top-3")

    with open("benchmark_results.txt", "w", encoding="utf-8") as f:
        f.write("\n".join(lines))
    print("\nSaved to benchmark_results.txt")


if __name__ == "__main__":
    main()