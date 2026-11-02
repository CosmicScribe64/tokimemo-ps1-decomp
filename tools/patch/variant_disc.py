#!/usr/bin/env python3
"""Disc image of a relinked build variant (T-39505, wiki/relink.md).

Usage (in Docker):
    python3 tools/patch/variant_disc.py VARIANT_DIR TRACK1 TRACK2 [OUT_DIR]

Writes OUT_DIR (default VARIANT_DIR/disc)/track1.bin and tokimemo.cue: the original track 1 with
SLPM_86.053 moved to the end of the track (tools/patch/disc.py, relocate=True) and every overlay of
VARIANT_DIR/ovl/ written in place. The cue names TRACK2 unchanged. The output is game data: never commit it.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from patch.disc import Disc, DiscError  # noqa: E402

EXE = "SLPM_86.053"


def main(argv):
    if len(argv) not in (3, 4):
        sys.exit(__doc__)
    variant, track1, track2 = argv[:3]
    out_dir = argv[3] if len(argv) == 4 else os.path.join(variant, "disc")
    os.makedirs(out_dir, exist_ok=True)
    ovl_dir = os.path.join(variant, "ovl")
    overlays = sorted(f[:-4] for f in os.listdir(ovl_dir) if f.endswith(".bin"))
    try:
        with Disc(track1) as disc:
            lba = disc.replace(EXE, open(os.path.join(variant, EXE + ".bin"), "rb").read(), relocate=True)
            for name in overlays:
                path = "CDROM/EXEDIR/%s.EXN" % name
                before = disc.iso.find(path).lba
                if disc.replace(path, open(os.path.join(ovl_dir, name + ".bin"), "rb").read()) != before:
                    raise DiscError("%s moved; the loader reads it by its fixed LBA" % name)
            out = os.path.join(out_dir, "track1.bin")
            sha = disc.write(out, cue_path=os.path.join(out_dir, "tokimemo.cue"), track2_path=track2)
    except (DiscError, OSError) as e:
        sys.exit("variant_disc.py: %s" % e)
    print("variant disc: %s (sha1 %s), %s at LBA %d, %d overlays in place" % (out, sha, EXE, lba, len(overlays)))


if __name__ == "__main__":
    main(sys.argv[1:])
