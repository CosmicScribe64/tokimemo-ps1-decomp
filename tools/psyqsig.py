"""Shared helpers for the PsyQ signature tools (psyq_sigmatch.py, psyq_sigfuzzy.py).

Signature data: lab313ru/psx_psyq_signatures JSON (see wiki/psyq-sdk.md), kept in the gitignored
tools/psyq_sigs/, one directory per PsyQ version. Entries have name, sig ("??" = relocated byte)
and labels. The executable text starts at file offset 0x800 and vram 0x80041000.
"""
import json
import os
import sys

TEXT_FILE_OFF = 0x800
TEXT_VRAM = 0x80041000
TEXT_SIZE = 0xA2800


def load_text(exe):
    """Return the .text bytes of the PS-X EXE at path `exe`."""
    with open(exe, "rb") as f:
        data = f.read()
    return data[TEXT_FILE_OFF:TEXT_FILE_OFF + TEXT_SIZE]


def versions(sigdir, wanted):
    """Version directory names to scan; exits non-zero if SIGDIR or a wanted version is missing."""
    if not os.path.isdir(sigdir):
        sys.exit("psyqsig: signature dir %s not found (see wiki/psyq-sdk.md)" % sigdir)
    if wanted:
        vers = wanted.split(",")
        for v in vers:
            if not os.path.isdir(os.path.join(sigdir, v)):
                sys.exit("psyqsig: version dir %s/%s not found" % (sigdir, v))
        return vers
    vers = sorted(v for v in os.listdir(sigdir) if os.path.isdir(os.path.join(sigdir, v)))
    if not vers:
        sys.exit("psyqsig: no version dirs in %s" % sigdir)
    return vers


def objects(vdir):
    """Yield (library file stem, entry) for every entry with a signature in a version dir."""
    for fn in sorted(os.listdir(vdir)):
        if fn.endswith(".json"):
            with open(os.path.join(vdir, fn)) as f:
                for o in json.load(f):
                    if "sig" in o:
                        yield fn[:-5], o
