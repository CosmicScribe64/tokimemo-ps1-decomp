#!/usr/bin/env python3
"""Scratch ugen shim for T-24925: run the real ugen, dump its input and output.

Called as: python3 ugen_shim.py --ugen /opt/ido/5.3/ugen <ugen args...>
Copies every plain file argument (the .B input, the .G output) into
UCODE_DUMP_DIR as <tag>_ugen_in_<n> / <tag>_ugen_out_<n>.
"""
import os
import shutil
import subprocess
import sys


def main(argv):
    real, args = argv[1], argv[2:]
    out_dir = os.environ.get("UCODE_DUMP_DIR")
    tag = os.environ.get("UCODE_DUMP_TAG", "u")
    rc = subprocess.call([real] + args)
    if rc == 0 and out_dir:
        os.makedirs(out_dir, exist_ok=True)
        n = 0
        for a in args:
            if not a or a.startswith("-") or not os.path.isfile(a):
                continue
            n += 1
            shutil.copy(a, os.path.join(out_dir, "%s_ugen_arg%d_%s" %
                                        (tag, n, os.path.basename(a))))
    return rc


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
