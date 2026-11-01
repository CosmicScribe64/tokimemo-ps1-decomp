#!/usr/bin/env python3
"""Make and apply the xdelta3 (VCDIFF) patch from the original to the patched track 1 (T-39501).

Usage (in Docker, image from tools/Dockerfile.patch, which adds xdelta3):
    python3 tools/patch/make_patch.py make ORIG_TRACK1 PATCHED_TRACK1 OUT.xdelta
    python3 tools/patch/make_patch.py apply ORIG_TRACK1 PATCH.xdelta OUT_TRACK1 [--out-sha1 HEX]

Both commands check the SHA-1 of ORIG_TRACK1 first (default: the PlayStation the Best track 1,
SOURCE_SHA1; --source-sha1 overrides, `any` skips). The patch has no secondary compression
(-S none) so that browser patchers (Rom Patcher JS) can apply it; xdelta3 checks each target
window with its own Adler-32. `make` also applies the new patch and compares the result with
PATCHED_TRACK1 before it reports success.
"""
import argparse
import hashlib
import os
import subprocess
import sys

SOURCE_SHA1 = "00b595095e26108d0e07d76492a11aaf3f71ccf1"  # track 1, MODE2/2352, 298500 sectors
XDELTA = "xdelta3"
# -B: source window as large as the whole track, so moved data is found anywhere in it
SOURCE_WINDOW = 1 << 30


class PatchError(Exception):
    pass


def sha1_file(path):
    h = hashlib.sha1()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def check_source(path, want):
    if want == "any":
        return
    have = sha1_file(path)
    if have != want:
        raise PatchError("%s: SHA-1 %s, expected %s (this patch is for the PlayStation the Best "
                         "track 1, MODE2/2352)" % (path, have, want))


def _xdelta(args):
    try:
        r = subprocess.run([XDELTA] + args, capture_output=True, text=True)
    except FileNotFoundError:
        raise PatchError("xdelta3 not found (use the image of tools/Dockerfile.patch)")
    if r.returncode:
        raise PatchError("xdelta3 %s failed: %s" % (args[0], r.stderr.strip()))


def apply(source, patch, out, source_sha1=SOURCE_SHA1, out_sha1=None):
    """Write OUT from SOURCE and PATCH. Returns the SHA-1 of OUT."""
    check_source(source, source_sha1)
    _xdelta(["-d", "-f", "-B", str(SOURCE_WINDOW), "-s", source, patch, out])
    have = sha1_file(out)
    if out_sha1 and have != out_sha1:
        os.remove(out)
        raise PatchError("patched track SHA-1 %s, expected %s" % (have, out_sha1))
    return have


def make(source, target, patch, source_sha1=SOURCE_SHA1):
    """Write PATCH (VCDIFF) from SOURCE to TARGET and verify it by applying it once.
    Returns the SHA-1 of TARGET."""
    check_source(source, source_sha1)
    _xdelta(["-e", "-f", "-9", "-S", "none", "-B", str(SOURCE_WINDOW), "-s", source, target, patch])
    want = sha1_file(target)
    check = patch + ".check"
    try:
        apply(source, patch, check, "any", want)
    finally:
        if os.path.exists(check):
            os.remove(check)
    return want


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    mk = sub.add_parser("make", help="make a patch from ORIG to PATCHED")
    mk.add_argument("source")
    mk.add_argument("target")
    mk.add_argument("patch")
    ap_ = sub.add_parser("apply", help="apply a patch to ORIG")
    ap_.add_argument("source")
    ap_.add_argument("patch")
    ap_.add_argument("out")
    ap_.add_argument("--out-sha1", help="expected SHA-1 of the result")
    for p in (mk, ap_):
        p.add_argument("--source-sha1", default=SOURCE_SHA1, help="expected SHA-1 of ORIG, or `any`")
    args = ap.parse_args(argv)
    try:
        if args.cmd == "make":
            sha = make(args.source, args.target, args.patch, args.source_sha1)
            print("%s: %d bytes, target SHA-1 %s" % (args.patch, os.path.getsize(args.patch), sha))
        else:
            print("%s: SHA-1 %s" % (args.out, apply(args.source, args.patch, args.out, args.source_sha1,
                                                    args.out_sha1)))
    except (PatchError, OSError) as e:
        print("make_patch.py: %s" % e, file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
