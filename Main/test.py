"""
benchmark_speed.py - which speed-up ideas really help on YOUR PC, using YOUR documents.

Setup (once):   pip install pypdfium2        (optional, only for the PDF reader test)
Run:            python benchmark_speed.py "C:\\path\\to\\your\\documents"
Options:        --e5          test the multilingual model instead of the small English one
                --skip-load   skip the model-loading test (saves 2 to 3 minutes)

It uses up to 40 of your documents (PDF, Word, text). Nothing is changed or uploaded.
Takes about 5 to 10 minutes. Results are saved to benchmark_speed_results.txt - send me that file.
"""
import os
import platform
import re
import statistics
import subprocess
import sys
import time
from pathlib import Path

import numpy as np

E5 = "--e5" in sys.argv
SKIP_LOAD = "--skip-load" in sys.argv
args = [a for a in sys.argv[1:] if not a.startswith("--")]
FOLDER = Path(args[0]) if args else None

MODEL = "intfloat/multilingual-e5-small" if E5 else "sentence-transformers/all-MiniLM-L6-v2"
P_PREFIX, Q_PREFIX = ("passage: ", "query: ") if E5 else ("", "")
MAX_DOCS = 40
lines = []
summary = []


def say(text=""):
    try:
        print(text, flush=True)
    except UnicodeEncodeError:
        print(text.encode("ascii", "replace").decode(), flush=True)
    lines.append(text)


# ----------------------------------------------------------------------------
# Getting some documents
# ----------------------------------------------------------------------------
FALLBACK = ("Ang bulkan ay nabubuo kapag umaakyat ang magma mula sa ilalim ng lupa. Photosynthesis is how plants "
            "turn sunlight water and carbon dioxide into glucose and oxygen. Thesis schedule chapter one draft is "
            "due in November and the defense panel meets in December. Budget ko ngayong buwan pagkain pamasahe at ipon. "
            "Mga paalala kapag may bagyo ihanda ang go bag at alamin ang evacuation center sa barangay. ")


def read_doc(path):
    try:
        from findit import read_text
        return read_text(path, log=lambda *_: None)
    except ImportError:
        return path.read_text(encoding="utf-8", errors="ignore") if path.suffix.lower() in {".txt", ".md"} else ""


def load_docs():
    docs = []
    if FOLDER and FOLDER.is_dir():
        files = [p for p in sorted(FOLDER.rglob("*")) if p.suffix.lower() in {".pdf", ".docx", ".txt", ".md"}]
        say(f"Found {len(files)} documents in {FOLDER}; reading up to {MAX_DOCS}...")
        t = time.perf_counter()
        for p in files:
            text = read_doc(p)
            if len(text.split()) >= 50:
                docs.append(text)
            if len(docs) >= MAX_DOCS:
                break
        say(f"  read {len(docs)} usable documents in {time.perf_counter() - t:.1f} s")
    if not docs:
        say("No usable documents found - using made-up text (about 750 words each) instead.")
        docs = [(FALLBACK * 12) + f" doc{i}" for i in range(MAX_DOCS)]
    words = [len(d.split()) for d in docs]
    say(f"  documents: {len(docs)}, words per document: median {int(statistics.median(words))}, "
        f"average {int(statistics.mean(words))}, longest {max(words)}")
    say()
    return docs


# ----------------------------------------------------------------------------
# Test 1: PDF readers
# ----------------------------------------------------------------------------
def test_pdf():
    say("=== Test 1: reading text out of PDFs ===")
    pdfs = [p for p in sorted(FOLDER.rglob("*.pdf"))][:15] if FOLDER and FOLDER.is_dir() else []
    if not pdfs:
        say("  no PDFs in that folder - skipped\n")
        return
    PAGES = 10
    try:
        from pypdf import PdfReader
        t = time.perf_counter(); pages = chars = 0
        for p in pdfs:
            try:
                r = PdfReader(str(p))
                for pg in r.pages[:PAGES]:
                    chars += len(pg.extract_text() or ""); pages += 1
            except Exception:
                pass
        s = time.perf_counter() - t
        say(f"  pypdf:      {pages / max(s, 1e-9):.1f} pages/sec ({s:.1f} s for {pages} pages, {chars} characters)")
        summary.append(f"PDF reading, pypdf: {pages / max(s, 1e-9):.1f} pages/sec")
    except Exception as e:
        say(f"  pypdf: FAILED {type(e).__name__}: {e}")
    try:
        import pypdfium2 as pdfium
        t = time.perf_counter(); pages = chars = 0
        for p in pdfs:
            try:
                doc = pdfium.PdfDocument(str(p))
                for i in range(min(PAGES, len(doc))):
                    page = doc[i]
                    tp = page.get_textpage()
                    chars += len(tp.get_text_range()); pages += 1
                    tp.close(); page.close()
                doc.close()
            except Exception:
                pass
        s = time.perf_counter() - t
        say(f"  pypdfium2:  {pages / max(s, 1e-9):.1f} pages/sec ({s:.1f} s for {pages} pages, {chars} characters)")
        summary.append(f"PDF reading, pypdfium2: {pages / max(s, 1e-9):.1f} pages/sec")
    except ImportError:
        say("  pypdfium2: not installed (pip install pypdfium2)")
    except Exception as e:
        say(f"  pypdfium2: FAILED {type(e).__name__}: {e}")
    say()


# ----------------------------------------------------------------------------
# Test 2: embedding strategies
# ----------------------------------------------------------------------------
def pieces(text, per, max_pieces=None, first_words=None):
    w = text.split()
    if first_words:
        w = w[:first_words]
    out = [" ".join(w[i:i + per]) for i in range(0, len(w), per)]
    out = [p for p in out if len(p) > 20]
    return out[:max_pieces] if max_pieces else out


def token_count(tok, text):
    try:
        return len(tok(text, add_special_tokens=True)["input_ids"])
    except Exception:
        return len(text.split())


def test_strategies(model, docs):
    say(f"=== Test 2: how much faster is it to read less? ({MODEL.split('/')[-1]}) ===")
    default_len = model.max_seq_length
    tok = getattr(model, "tokenizer", None)
    total_tokens = sum(token_count(tok, d) for d in docs) if tok else 0
    n_docs = len(docs)

    def run(name, per, max_pieces, first_words, seq_len, batches, dedupe):
        texts = [P_PREFIX + p for d in docs for p in pieces(d, per, max_pieces, first_words)]
        if dedupe:
            texts = list(dict.fromkeys(texts))
        model.max_seq_length = seq_len
        read_tokens = sum(min(token_count(tok, t), seq_len) for t in texts) if tok else 0
        t = time.perf_counter()
        if batches == "per64":                        # like the current script: a call per 64 pieces
            for i in range(0, len(texts), 64):
                model.encode(texts[i:i + 64], batch_size=64, normalize_embeddings=True)
        else:                                         # one big call, the library sorts by length
            model.encode(texts, batch_size=32, normalize_embeddings=True)
        secs = time.perf_counter() - t
        per_doc = secs / n_docs
        cov = f"{read_tokens / total_tokens * 100:.0f}% of the text read" if total_tokens else ""
        say(f"  {name}: {len(texts)} pieces, {secs:.1f} s -> {per_doc:.2f} s/doc -> 1,000 docs in {per_doc * 1000 / 60:.1f} min  ({cov})")
        summary.append(f"{name}: 1,000 docs in {per_doc * 1000 / 60:.1f} min ({cov})")
        return secs

    model.encode(["warm up"], normalize_embeddings=True)
    base = run("A. current script (150-word pieces, all text)", 150, 30, None, default_len, "per64", False)
    run("B. 90-word pieces, 128 tokens, one big call", 90, None, None, 128, "one", False)
    run("C. B + only the first 300 words of each file", 90, None, 300, 128, "one", False)
    c = run("D. C + remove duplicate pieces", 90, None, 300, 128, "one", True)
    say(f"  -> D is {base / max(c, 1e-9):.1f}x faster than A for these documents")
    say()
    model.max_seq_length = default_len


# ----------------------------------------------------------------------------
# Test 3: quantization
# ----------------------------------------------------------------------------
def test_quant(model, docs):
    say("=== Test 3: int8 quantization (smaller and maybe faster) ===")
    try:
        import copy
        import torch
        texts = [P_PREFIX + p for d in docs[:10] for p in pieces(d, 90, 6, 300)][:60]
        model.max_seq_length = 128

        def timed(m):
            m.encode(texts[:4], normalize_embeddings=True)
            t = time.perf_counter()
            v = m.encode(texts, batch_size=16, normalize_embeddings=True)
            return time.perf_counter() - t, v

        s0, v0 = timed(model)
        q = torch.quantization.quantize_dynamic(copy.deepcopy(model), {torch.nn.Linear}, dtype=torch.qint8)
        s1, v1 = timed(q)
        cos = float(np.mean(np.sum(v0 * v1, axis=1)))
        say(f"  normal: {len(texts) / s0:.1f} pieces/sec;  int8: {len(texts) / s1:.1f} pieces/sec  "
            f"({s0 / s1:.2f}x), average similarity to the normal vectors: {cos:.3f} (1.000 = identical)")
        summary.append(f"int8 quantization: {s0 / s1:.2f}x speed, similarity {cos:.3f}")
    except Exception as e:
        say(f"  FAILED: {type(e).__name__}: {str(e)[:200]}")
        summary.append(f"int8 quantization: FAILED ({type(e).__name__})")
    say()


# ----------------------------------------------------------------------------
# Test 4: one search
# ----------------------------------------------------------------------------
def test_query(model):
    say("=== Test 4: how long does ONE search take? ===")
    qs = ["notes about volcanoes", "kailan ang due ng chapter 1", "my thesis schedule", "budget para sa pamasahe"]
    vecs = np.random.default_rng(0).standard_normal((20000, 384 if not E5 else 384)).astype(np.float32)
    vecs /= np.linalg.norm(vecs, axis=1, keepdims=True)
    model.encode([Q_PREFIX + "warm up"], normalize_embeddings=True)
    times, match = [], []
    for _ in range(5):
        for q in qs:
            t = time.perf_counter()
            v = model.encode([Q_PREFIX + q], normalize_embeddings=True)[0]
            t1 = time.perf_counter()
            s = vecs[:, :len(v)] @ v
            np.argpartition(-s, 5)[:5]
            match.append(time.perf_counter() - t1)
            times.append(t1 - t)
    say(f"  turning the description into numbers: median {statistics.median(times) * 1000:.0f} ms (worst {max(times) * 1000:.0f} ms)")
    say(f"  matching against 20,000 saved pieces: median {statistics.median(match) * 1000:.1f} ms")
    summary.append(f"One search: {statistics.median(times) * 1000:.0f} ms for the description + "
                   f"{statistics.median(match) * 1000:.1f} ms matching 20,000 pieces")
    say()


# ----------------------------------------------------------------------------
# Test 5: model loading
# ----------------------------------------------------------------------------
LOAD_CODE = """
import time, sys
t=time.perf_counter()
from sentence_transformers import SentenceTransformer
t1=time.perf_counter()
SentenceTransformer(sys.argv[1])
t2=time.perf_counter()
print(f"{t1-t:.1f} {t2-t1:.1f}")
"""


def test_load():
    say("=== Test 5: starting the app (loading the model) ===")
    say("  (runs the loading 4 times in fresh processes; about 2 to 3 minutes)")
    results = {}
    for label, offline in (("normal", False), ("offline mode (HF_HUB_OFFLINE=1)", True)):
        env = dict(os.environ)
        env.pop("HF_HUB_OFFLINE", None)
        if offline:
            env["HF_HUB_OFFLINE"] = "1"
        runs = []
        for _ in range(2):
            out = subprocess.run([sys.executable, "-c", LOAD_CODE, MODEL], capture_output=True, text=True, env=env)
            try:
                a, b = out.stdout.split()[-2:]
                runs.append((float(a), float(b)))
            except Exception:
                say(f"  {label}: FAILED {out.stderr.strip()[-150:]}")
                break
        if runs:
            imp, load = runs[-1]                      # the second run: files already cached by Windows
            results[label] = imp + load
            say(f"  {label}: importing the libraries {imp:.1f} s + loading the model {load:.1f} s = {imp + load:.1f} s")
    if len(results) == 2:
        a, b = results.values()
        summary.append(f"Startup: normal {a:.1f} s vs offline mode {b:.1f} s")
    say()


def main():
    say("=== Your machine ===")
    say(f"Python {sys.version.split()[0]} on {platform.system()} {platform.release()}, CPU: {platform.processor() or 'unknown'}")
    say()
    docs = load_docs()
    test_pdf()
    try:
        from sentence_transformers import SentenceTransformer
        say(f"Loading {MODEL}...")
        model = SentenceTransformer(MODEL)
        say()
        test_strategies(model, docs)
        test_quant(model, docs)
        test_query(model)
    except Exception as e:
        say(f"Model tests FAILED: {type(e).__name__}: {e}")
    if not SKIP_LOAD:
        test_load()

    say("=== Summary ===")
    for s in summary:
        say("  " + s)
    Path("benchmark_speed_results.txt").write_text("\n".join(lines), encoding="utf-8")
    print("\nSaved to benchmark_speed_results.txt")


if __name__ == "__main__":
    main()