#!/usr/bin/env python3
"""Unit tests for tools/identify_version.py on synthetic data (no game data).

Run: tools/docker.sh python3 tools/test_identify_version.py
"""
import hashlib
import os
import struct
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import identify_version as iv  # noqa: E402

EXE = b"PS-X EXE" + b"\x01" * 100
OVL = b"\x02" * 64


def db_text():
    return ("version t Test_Disc X 19970101 PSX.EXE 1 aa\n"
            "exe t %d %s 0 0 0 0 0 0\n" % (len(EXE), hashlib.sha1(EXE).hexdigest()) +
            "ovl t A.EXN %s\novl u A.EXN %s\nversion u Other X 19970101 PSX.EXE 1 bb\n" %
            (hashlib.sha1(OVL).hexdigest(), "0" * 40))


def dir_rec(name, lba, size, flags):
    nm = name.encode()
    ln = 33 + len(nm)
    ln += ln & 1
    rec = bytearray(ln)
    rec[0] = ln
    rec[2:6] = struct.pack("<I", lba)
    rec[10:14] = struct.pack("<I", size)
    rec[25] = flags
    rec[32] = len(nm)
    rec[33:33 + len(nm)] = nm
    return bytes(rec)


def make_iso(path, files):
    """Cooked ISO with a flat root: files = {name: bytes}."""
    secs = {}
    root = b"".join(dir_rec(n, 20 + i, len(d), 0) for i, (n, d) in enumerate(files.items()))
    secs[19] = root
    for i, d in enumerate(files.values()):
        secs[20 + i] = d
    pvd = bytearray(2048)
    pvd[1:6] = b"CD001"
    pvd[156:190] = dir_rec("\x00", 19, 2048, 2)[:34]
    secs[16] = bytes(pvd)
    with open(path, "wb") as f:
        for lba in range(20 + len(files)):
            f.write(secs.get(lba, b"").ljust(2048, b"\0"))


class T(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.mkdtemp()
        self.db_path = os.path.join(self.tmp, "v.txt")
        with open(self.db_path, "w") as f:
            f.write(db_text())
        self.db = iv.load_db(self.db_path)

    def test_db(self):
        self.assertEqual(self.db[1][hashlib.sha1(EXE).hexdigest()], ["t"])

    def test_folder_known(self):
        d = os.path.join(self.tmp, "f")
        os.makedirs(os.path.join(d, "CDROM", "EXEDIR"))
        for rel, data in (("PSX.EXE", EXE), ("CDROM/EXEDIR/A.EXN", OVL)):
            with open(os.path.join(d, rel), "wb") as f:
                f.write(data)
        res = iv.identify(iv.read_folder(d), self.db)
        self.assertEqual(res["version"], "t")
        self.assertEqual(res["overlay_matches"]["t"], 1)

    def test_unknown_and_warning(self):
        files = {"PSX.EXE": b"other", "CDROM/EXEDIR/A.EXN": OVL}
        res = iv.identify(files, self.db)
        self.assertIsNone(res["version"])
        self.assertIn("UNKNOWN", iv.report(res, self.db))

    def test_iso(self):
        p = os.path.join(self.tmp, "d.iso")
        make_iso(p, {"SYSTEM.CNF;1": b"BOOT = cdrom:PSX.EXE;1\r\n", "PSX.EXE;1": EXE})
        files = iv.read_tree(iv.open_image(p), {"SYSTEM.CNF", "PSX.EXE"})
        res = iv.identify(files, self.db)
        self.assertEqual(res["version"], "t")
        self.assertEqual(res["boot"], "PSX.EXE")

    def test_no_exe(self):
        with self.assertRaises(ValueError):
            iv.identify({"SYSTEM.CNF": b"x"}, self.db)


if __name__ == "__main__":
    unittest.main()
