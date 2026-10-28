"""
benchmark_images.py - tests if photo search (CLIP) and OCR are realistic on your PC.

Setup (once):
    pip install pillow rapidocr-onnxruntime
Optional second OCR (a separate program): install Tesseract for Windows, then it is found automatically.

Run:
    python benchmark_images.py                      # uses generated test images
    python benchmark_images.py "D:\\Pictures\\Trip"   # also tests your real photos (best!)

First run downloads the CLIP model (about 600 MB). Takes about 5 to 15 minutes on an older PC.
Results are saved to benchmark_images_results.txt - send me that file.
"""
import os
import platform
import random
import re
import shutil
import subprocess
import sys
import tempfile
import textwrap
import time
from pathlib import Path

import numpy as np

QUICK = "--quick" in sys.argv            # tiny run, only for checking the script itself
args = [a for a in sys.argv[1:] if not a.startswith("--")]
PHOTO_DIR = Path(args[0]) if args else None

N_PHOTOS = 8 if QUICK else 48            # photos timed in the CLIP test
PHOTO_SIZE = (1200, 900) if QUICK else (4032, 3024)    # a 12-megapixel phone photo
N_TEXT_IMAGES = 3 if QUICK else 10       # screenshots used for the OCR test
BATCH = 16

SENTENCES = [
    "Thesis schedule: chapter 1 draft is due on November 5 and the defense panel is on December 12. The adviser wants weekly progress reports every Friday.",
    "Paalala sa lahat ng estudyante: ang pagsusulit sa Math ay sa Lunes, saklaw ang mga lesson tungkol sa fractions at decimals. Magdala ng calculator at ballpen.",
    "Photosynthesis is how plants turn sunlight, water and carbon dioxide into glucose and oxygen inside the chloroplasts of leaf cells.",
    "Budget ko ngayong buwan: pagkain 1500 pesos, pamasahe 800 pesos, school projects 500 pesos, at ipon 700 pesos.",
    "Mga paalala kapag may bagyo: ihanda ang go bag, i-charge ang cellphone, at alamin ang pinakamalapit na evacuation center sa barangay ninyo sa Bulacan.",
    "Programming notes: a loop repeats a block of code, while an array stores many values in one variable. Practice with for loops and nested loops.",
    "Listahan ng gagawin sa org: mag-print ng certificate, maghanda ng attendance sheet, at i-send ang invitation sa lahat ng members bago mag-Friday.",
    "Recipe for chicken adobo: simmer chicken in soy sauce, vinegar, garlic, bay leaves and peppercorns until tender, then fry it lightly for a crispy edge.",
    "Report sa Science: ang tubig sa ilog ay nagiging marumi dahil sa basura at kemikal. Kailangan natin ng clean-up drive at tamang pagtatapon ng basura.",
    "Ang bulkan ay nabubuo kapag umaakyat ang magma mula sa ilalim ng lupa at sumasabog ang lava at abo. Ang Bulkang Mayon sa Albay ay napakaganda.",
]
LOOKUPS = ["a person smiling", "food on a table", "a building", "a beach or sea", "a document with text"]

lines = []


def say(text=""):
    print(text, flush=True)
    lines.append(text)


def machine_info():
    say("=== Your machine ===")
    say(f"Python {sys.version.split()[0]} on {platform.system()} {platform.release()}")
    say(f"CPU: {platform.processor() or 'unknown'}")
    try:
        import torch
        say(f"PyTorch {torch.__version__}, threads: {torch.get_num_threads()}")
    except Exception as e:
        say(f"PyTorch not available: {e}")
    say()


# ----------------------------------------------------------------------------
# Making test images
# ----------------------------------------------------------------------------
def make_photos(folder, n_distinct):
    """Smooth coloured pictures with some noise, saved as big JPEGs like a phone photo."""
    from PIL import Image
    rng = np.random.default_rng(0)
    w, h = PHOTO_SIZE
    paths = []
    for i in range(n_distinct):
        small = rng.integers(0, 255, size=(h // 64 + 1, w // 64 + 1, 3), dtype=np.uint8)
        img = Image.fromarray(small).resize((w, h), Image.BICUBIC)
        arr = np.asarray(img, dtype=np.int16) + rng.integers(-8, 9, size=(h, w, 3), dtype=np.int16)
        img = Image.fromarray(np.clip(arr, 0, 255).astype(np.uint8))
        p = Path(folder) / f"photo_{i:02d}.jpg"
        img.save(p, quality=90)
        paths.append(p)
    return paths


def get_font(size):
    from PIL import ImageFont
    for name in ("arial.ttf", "segoeui.ttf", "DejaVuSans.ttf", "LiberationSans-Regular.ttf"):
        try:
            return ImageFont.truetype(name, size)
        except Exception:
            pass
    try:
        return ImageFont.load_default(size=size)
    except Exception:
        return ImageFont.load_default()


def make_text_images(folder, n):
    """Screenshot-like images with known text, so OCR accuracy can be checked."""
    from PIL import Image, ImageDraw
    items = []
    font = get_font(30)
    for i in range(n):
        text = SENTENCES[i % len(SENTENCES)]
        img = Image.new("RGB", (1280, 420), (255, 255, 255))
        d = ImageDraw.Draw(img)
        y = 30
        for line in textwrap.wrap(text, 58):
            d.text((40, y), line, fill=(20, 20, 20), font=font)
            y += 48
        p = Path(folder) / f"screenshot_{i:02d}.png"
        img.save(p)
        items.append((p, text))
    return items


def word_recall(expected, found):
    exp = set(re.findall(r"[a-z0-9']+", expected.lower()))
    got = set(re.findall(r"[a-z0-9']+", found.lower()))
    return len(exp & got) / max(1, len(exp))


# ----------------------------------------------------------------------------
# Test 1: photo embeddings (CLIP)
# ----------------------------------------------------------------------------
def clip_speed(model, paths, use_draft):
    from PIL import Image
    decode_s = encode_s = 0.0
    done = 0
    for i in range(0, len(paths), BATCH):
        t = time.perf_counter()
        batch = []
        for p in paths[i:i + BATCH]:
            im = Image.open(p)
            if use_draft:
                im.draft("RGB", (448, 448))          # lets the JPEG decoder shrink while reading (much faster)
            batch.append(im.convert("RGB"))
        decode_s += time.perf_counter() - t
        t = time.perf_counter()
        model.encode(batch, batch_size=BATCH, normalize_embeddings=True)
        encode_s += time.perf_counter() - t
        done += len(batch)
    return decode_s / done * 1000, done / encode_s, done / (decode_s + encode_s)


def test_clip(paths, summary):
    say("=== Test 1: photo search model (CLIP ViT-B/32) ===")
    try:
        from sentence_transformers import SentenceTransformer
        t = time.perf_counter()
        model = SentenceTransformer("clip-ViT-B-32")
        say(f"Model loaded in {time.perf_counter() - t:.1f} s")
        from PIL import Image
        model.encode([Image.open(paths[0]).convert("RGB")], normalize_embeddings=True)   # warm-up
        timed = (paths * (N_PHOTOS // len(paths) + 1))[:N_PHOTOS]
        for label, draft in (("normal decode", False), ("fast JPEG decode", True)):
            dec_ms, enc_rate, total = clip_speed(model, timed, draft)
            say(f"  {label}: decode {dec_ms:.0f} ms/photo, model {enc_rate:.1f} photos/sec, "
                f"overall {total:.1f} photos/sec -> 1,000 photos in {1000 / total / 60:.1f} min, "
                f"10,000 in {10000 / total / 60:.0f} min")
            summary.append(f"CLIP, {label}: {total:.1f} photos/sec (1,000 photos ~ {1000 / total / 60:.1f} min)")

        if PHOTO_DIR:
            say("  Real photos: type a description to see the top 3 matches (just press Enter to skip).")
            real = sorted(p for p in PHOTO_DIR.rglob("*") if p.suffix.lower() in {".jpg", ".jpeg", ".png"})[:60]
            if real:
                vecs = []
                for i in range(0, len(real), BATCH):
                    ims = []
                    for p in real[i:i + BATCH]:
                        im = Image.open(p)
                        im.draft("RGB", (448, 448))
                        ims.append(im.convert("RGB"))
                    vecs.append(model.encode(ims, normalize_embeddings=True))
                vecs = np.vstack(vecs)
                for q in LOOKUPS[:2] + ([] if QUICK else [None]):
                    q = q or input("  description> ").strip()
                    if not q:
                        break
                    qv = model.encode([q], normalize_embeddings=True)[0]
                    top = np.argsort(-(vecs @ qv))[:3]
                    say(f'  "{q}" -> ' + ", ".join(real[i].name for i in top))
            else:
                say("  (no .jpg/.png photos found in that folder)")
        say()
    except Exception as e:
        say(f"  FAILED: {type(e).__name__}: {e}")
        summary.append(f"CLIP: FAILED ({type(e).__name__}: {e})")
        say()


# ----------------------------------------------------------------------------
# Test 2: OCR
# ----------------------------------------------------------------------------
def run_ocr_test(name, read_fn, items, summary):
    try:
        read_fn(items[0][0])                          # warm-up (also loads the models)
        total_s, recalls = 0.0, []
        for p, text in items:
            t = time.perf_counter()
            found = read_fn(p)
            total_s += time.perf_counter() - t
            recalls.append(word_recall(text, found))
        per = total_s / len(items)
        acc = sum(recalls) / len(recalls) * 100
        say(f"  {name}: {per:.2f} s per screenshot, {acc:.0f}% of the words read correctly "
            f"-> 1,000 screenshots in about {per * 1000 / 60:.0f} min")
        summary.append(f"{name}: {per:.2f} s/image, {acc:.0f}% words correct")
    except Exception as e:
        say(f"  {name}: FAILED: {type(e).__name__}: {e}")
        summary.append(f"{name}: FAILED ({type(e).__name__}: {e})")


def rapidocr_reader():
    try:
        from rapidocr_onnxruntime import RapidOCR
    except ImportError:
        from rapidocr import RapidOCR
    engine = RapidOCR()

    def read(path):
        res = engine(str(path))
        if hasattr(res, "txts"):
            return " ".join(res.txts or [])
        result = res[0] if isinstance(res, tuple) else res
        return " ".join(r[1] for r in (result or []))
    return read


def find_tesseract():
    exe = shutil.which("tesseract")
    for guess in (r"C:\Program Files\Tesseract-OCR\tesseract.exe",
                  r"C:\Program Files (x86)\Tesseract-OCR\tesseract.exe"):
        if not exe and os.path.exists(guess):
            exe = guess
    return exe


def tesseract_reader(exe):
    langs = subprocess.run([exe, "--list-langs"], capture_output=True, text=True).stdout.split()
    lang = "eng+fil" if "fil" in langs else "eng"
    say(f"  (Tesseract found; language data: {lang})")

    def read(path):
        out = subprocess.run([exe, str(path), "stdout", "-l", lang], capture_output=True, text=True)
        if out.returncode != 0:
            raise RuntimeError(out.stderr.strip()[:200])
        return out.stdout
    return read


def test_ocr(items, summary):
    say("=== Test 2: reading text inside images (OCR) ===")
    say(f"  {len(items)} generated screenshots with English and Taglish text")
    try:
        reader = rapidocr_reader()
        run_ocr_test("RapidOCR", reader, items, summary)
    except ImportError:
        say("  RapidOCR: not installed (pip install rapidocr-onnxruntime)")
        summary.append("RapidOCR: not installed")
    except Exception as e:
        say(f"  RapidOCR: FAILED to start: {type(e).__name__}: {e}")
        summary.append(f"RapidOCR: FAILED to start ({type(e).__name__}: {e})")

    exe = find_tesseract()
    if exe:
        try:
            run_ocr_test("Tesseract", tesseract_reader(exe), items, summary)
        except Exception as e:
            say(f"  Tesseract: FAILED: {type(e).__name__}: {e}")
            summary.append(f"Tesseract: FAILED ({type(e).__name__}: {e})")
    else:
        say("  Tesseract: not installed (optional)")
        summary.append("Tesseract: not installed (optional)")
    say()


def main():
    machine_info()
    summary = []
    tmp = tempfile.mkdtemp(prefix="findit_bench_")
    try:
        say("Making test images (this takes a minute)...")
        photos = make_photos(tmp, 4 if QUICK else 12)
        texts = make_text_images(tmp, N_TEXT_IMAGES)
        say()
        test_clip(photos, summary)
        test_ocr(texts, summary)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)

    say("=== Summary ===")
    for s in summary:
        say("  " + s)
    with open("benchmark_images_results.txt", "w", encoding="utf-8") as f:
        f.write("\n".join(lines))
    print("\nSaved to benchmark_images_results.txt")


if __name__ == "__main__":
    main()