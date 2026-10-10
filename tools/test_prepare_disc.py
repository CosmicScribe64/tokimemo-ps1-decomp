#!/usr/bin/env python3
"""Unit tests for tools/prepare_disc.py (T-3300): synthetic discs only, no game data.

Run: tools/docker.sh python3 tools/test_prepare_disc.py
"""
import hashlib
import os
import shutil
import sys
import tempfile
import unittest
import zipfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import identify_version as iv  # noqa: E402
import prepare_disc as pd  # noqa: E402
from test_identify_version import db_text, make_iso  # noqa: E402

EXE = b"PS-X EXE" + b"\x01" * 100
OTHER = b"PS-X EXE" + b"\x02" * 100
SHA = hashlib.sha1(EXE).hexdigest()


def files_of(exe):
    return {"SYSTEM.CNF;1": b"BOOT = cdrom:SLPM_86.053;1\r\n", "SLPM_86.053;1": exe}


class T(unittest.TestCase):
    def setUp(self):
        self.root = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.root, True)
        with open(os.path.join(self.root, "v.txt"), "w") as f:
            f.write(db_text())
        self.db = os.path.join(self.root, "v.txt")
        os.makedirs(os.path.join(self.root, "config"))
        os.makedirs(os.path.join(self.root, "game", "a", "b"))

    def path(self, *parts):
        return os.path.join(self.root, *parts)

    def iso(self, rel, exe=EXE):
        make_iso(self.path(rel), files_of(exe))
        return self.path(rel)

    def prepare(self):
        logs = []
        err = pd.prepare(self.root, want_sha1=SHA, db_path=self.db, log=logs.append)
        return err, logs

    def ready(self):
        with open(self.path("disc", "files", "SLPM_86.053"), "rb") as f:
            return f.read() == EXE

    def test_iso_at_depth(self):
        self.iso("game/a/b/x.iso")
        err, _ = self.prepare()
        self.assertIsNone(err)
        self.assertTrue(self.ready())

    def test_zip_and_cue_with_several_bins(self):
        iso = self.iso("tmp.iso")
        with zipfile.ZipFile(self.path("game", "a", "d.zip"), "w") as z:
            z.write(iso, "sub/disc.iso")
        err, _ = self.prepare()
        self.assertIsNone(err)
        self.assertTrue(self.ready())

    def test_bin_named_by_a_cue_is_not_a_candidate(self):
        self.iso("game/t1.iso")
        with open(self.path("game", "x.cue"), "w") as f:
            f.write('FILE "t1.iso" BINARY\n  TRACK 01 MODE1/2048\n    INDEX 01 00:00:00\n')
        open(self.path("game", "t2.bin"), "wb").close()
        open(self.path("game", "t3.bin"), "wb").close()
        cands, dirs = pd.find_candidates(self.root)
        names = [os.path.basename(c) for c in cands]
        self.assertEqual(names, ["x.cue", "t2.bin", "t3.bin"])
        self.assertIn(self.path("game", "a", "b"), dirs)

    def test_files_outside_game_are_ignored(self):
        self.iso("top.iso")
        cands, _ = pd.find_candidates(self.root)
        self.assertEqual(cands, [])
        err, _ = self.prepare()
        self.assertIn("game/", err)

    def test_unsupported_release_is_named(self):
        self.iso("game/a/old.iso", OTHER)
        err, _ = self.prepare()
        self.assertIn("a/old.iso", err)
        self.assertIn("exe SHA-1 " + hashlib.sha1(OTHER).hexdigest(), err)
        self.assertIn("PlayStation the Best", err)
        self.assertFalse(os.path.exists(self.path("disc")))

    def test_unsupported_release_keeps_the_old_disc(self):
        os.makedirs(self.path("disc", "files"))
        with open(self.path("disc", "files", "SLPM_86.053"), "wb") as f:
            f.write(EXE)
        self.iso("game/old.iso", OTHER)
        self.assertIsNotNone(self.prepare()[0])
        self.assertTrue(self.ready())

    def test_no_image_says_where_to_put_it(self):
        err, _ = self.prepare()
        self.assertIn("game/", err)

    def test_existing_disc_is_enough(self):
        os.makedirs(self.path("disc", "files"))
        with open(self.path("disc", "files", "SLPM_86.053"), "wb") as f:
            f.write(EXE)
        self.assertEqual(self.prepare(), (None, []))

    def test_unreadable_file_is_reported(self):
        with open(self.path("game", "junk.iso"), "wb") as f:
            f.write(b"x" * 100000)
        err, _ = self.prepare()
        self.assertIn("junk.iso: not readable", err)

    def test_main_writes_the_stamp(self):
        self.iso("game/x.iso")
        sha = os.path.join(self.root, "config", "SLPM_86.053.sha1")
        with open(sha, "w") as f:
            f.write(SHA + "  SLPM_86.053\n")
        stamp = self.path("build", "disc.stamp")
        old = iv.DEFAULT_DB
        iv.DEFAULT_DB = self.db
        try:
            self.assertEqual(pd.main(["--root", self.root, "--stamp", stamp]), 0)
        finally:
            iv.DEFAULT_DB = old
        self.assertTrue(os.path.exists(stamp))


if __name__ == "__main__":
    unittest.main()
