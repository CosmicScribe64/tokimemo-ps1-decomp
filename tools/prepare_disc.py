#!/usr/bin/env python3
"""Find the user's copy of the game, check the release, and unpack it into disc/ (T-3300).

Usage (in Docker, normally run by ninja as the `build/disc.stamp` step):
    python3 tools/prepare_disc.py [--stamp build/disc.stamp]

Looks for
  1. game/ (any depth): .cue (BIN+CUE, one or several .bin), .chd, .iso, or a .zip / .7z of those,
  2. otherwise an existing disc/files/SLPM_86.053 (CI, or a disc extracted by hand).
Only game/ is searched; a zip in the repository root is not picked up.
Each image is identified with tools/identify_version.py. The first one whose boot exe has the SHA-1
of config/SLPM_86.053.sha1 (PlayStation the Best) is unpacked to disc/files (tools/extract_disc.py;
raw or cooked sectors, the build needs only the files). Any other release is refused with its name.
Nothing is written to disc/ unless the unpack succeeded. Exit status: 0 ready, 1 not ready
(message says what was found and what is needed).
"""
import argparse
import contextlib
import os
import re
import shutil
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import extract_disc  # noqa: E402
import identify_version as iv  # noqa: E402

CUE_FILE_RE = re.compile(r'FILE\s+"([^"]+)"')
GAME_DIR = "game"
EXE = "SLPM_86.053"
# order of trial; a .bin that a .cue names is part of that cue, a lone .bin is tried last
KINDS = (".cue", ".chd", ".iso", ".zip", ".7z", ".bin", ".img")
WHERE = ("Put your copy of Tokimeki Memorial - Forever with You (Japan) (PlayStation the Best) in the "
         "game/ folder (BIN+CUE, CHD, ISO, or a .zip or .7z of those) and run the build again; "
         "see game/README.md.")


def _rank(path):
    return (KINDS.index(os.path.splitext(path)[1].lower()), path.lower())


def find_candidates(root=".", game=GAME_DIR):
    """([image paths in trial order], [directories whose contents matter]).
    Only game/ is searched, at any depth."""
    found, dirs = [], []
    gdir = os.path.join(root, game)
    if os.path.isdir(gdir):
        for d, subdirs, files in os.walk(gdir):
            dirs.append(d)
            subdirs.sort()
            found += [os.path.join(d, f) for f in files if os.path.splitext(f)[1].lower() in KINDS]
    # a .bin that some .cue names is not an image of its own
    named = set()
    for c in found:
        if c.lower().endswith(".cue"):
            try:
                with open(c, errors="replace") as f:
                    for name in cue_files(f.read()):
                        named.add(os.path.normcase(os.path.join(os.path.dirname(c), name)))
            except OSError:
                pass
    found = [c for c in found if os.path.normcase(c) not in named]
    return sorted(found, key=_rank), dirs


def cue_files(text):
    """All file names a cue sheet names."""
    return CUE_FILE_RE.findall(text)


def expected_sha1(root="."):
    with open(os.path.join(root, "config", EXE + ".sha1")) as f:
        return f.read().split()[0]


@contextlib.contextmanager
def open_any(path):
    """The Image of a .cue/.bin/.iso/.chd/.zip/.7z, closed afterwards."""
    if path.lower().endswith(iv.ARCHIVE_EXTS):
        with iv.open_archive(path) as img:
            yield img
        return
    img = iv.open_image(path)
    try:
        yield img
    finally:
        img.f.close()


def prepare(root=".", game=GAME_DIR, disc="disc", want_sha1=None, db_path=None, log=print):
    """Make disc/files ready. Returns None on success, else the error message."""
    want_sha1 = want_sha1 or expected_sha1(root)
    cands, _dirs = find_candidates(root, game)
    have_disc = os.path.exists(os.path.join(root, disc, "files", EXE))
    if not cands:
        return None if have_disc else "no game image found. " + WHERE
    db = iv.load_db(db_path or iv.DEFAULT_DB)
    wanted = {"SYSTEM.CNF", *iv.BOOT_NAMES} | {"CDROM/EXEDIR/%s" % n for n in iv.db_overlay_names(db)}
    notes = []
    for path in cands:
        rel = os.path.relpath(path, root)
        try:
            with open_any(path) as img:
                res = iv.identify(iv.read_tree(img, wanted), db)
                if res["exe_sha1"] != want_sha1:
                    names = " / ".join(db[0].get(v, v) for v in res["versions"]) or "an unknown release"
                    notes.append("%s: %s (exe SHA-1 %s)" % (rel, names, res["exe_sha1"]))
                    continue
                out = os.path.join(root, disc)
                tmp = os.path.join(out, "files.new")
                shutil.rmtree(tmp, ignore_errors=True)
                os.makedirs(tmp)
                log("prepare_disc.py: unpacking %s into %s/files" % (rel, disc))
                counts = extract_disc.extract_image(img, tmp, EXE)
        except (OSError, ValueError, EOFError, struct.error) as e:
            notes.append("%s: not readable (%s)" % (rel, e))
            continue
        shutil.rmtree(os.path.join(out, "files"), ignore_errors=True)
        os.rename(os.path.join(tmp, "files"), os.path.join(out, "files"))
        shutil.rmtree(tmp, ignore_errors=True)
        log("prepare_disc.py: %d files, %d skipped (CD-DA/XA streams)" % (counts["files"], counts["skipped"]))
        return None
    return ("no usable image of the supported release. This build needs PlayStation the Best "
            "(%s, exe SHA-1 %s). Found:\n  %s\n%s" % (EXE, want_sha1, "\n  ".join(notes), WHERE))


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--stamp", help="file to touch when disc/files is ready")
    ap.add_argument("--root", default=".")
    args = ap.parse_args(argv)
    err = prepare(args.root)
    if err:
        print("prepare_disc.py: " + err, file=sys.stderr)
        return 1
    if args.stamp:
        os.makedirs(os.path.dirname(args.stamp) or ".", exist_ok=True)
        with open(args.stamp, "w") as f:
            f.write("ready\n")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
