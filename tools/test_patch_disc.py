"""Tests for the disc rebuild and xdelta patch tools (tools/patch/, T-39501).

Run: tools/docker.sh python3 tools/test_patch_disc.py   (xdelta tests: IMG=tokimemo-patch, see
tools/Dockerfile.patch). The sector tests are synthetic. The disc tests need the original
track 1 and 2 in $TOKIMEMO_ORIG_DISC (default disc/; track1.bin, track2.bin) and skip without
them; the matching build test also needs $TOKIMEMO_MATCH_EXE (default build/SLPM_86.053.bin).
Full patched tracks (about 700 MB each) go to $TOKIMEMO_PATCH_TMP (default: a temporary dir).
"""
import hashlib
import os
import shutil
import struct
import tempfile
import unittest

from patch import disc as pd
from patch import make_patch as mp
from patch import sector as cs

ORIG = os.environ.get("TOKIMEMO_ORIG_DISC", "disc")
TRACK1 = os.path.join(ORIG, "track1.bin")
TRACK2 = os.path.join(ORIG, "track2.bin")
MATCH_EXE = os.environ.get("TOKIMEMO_MATCH_EXE", "build/SLPM_86.053.bin")
HAVE_DISC = os.path.isfile(TRACK1) and os.path.isfile(TRACK2)
HAVE_XDELTA = shutil.which(mp.XDELTA) is not None
DAMAGED_DIR_LBA = 14515  # /CDROM, no subheader
HEADER_ECC_LBA = 298499  # last sector: ECC computed over the header too (Mode 1 rule)
SAMPLE = "CDROM/BATTLE/PARK.BIN"  # an ordinary Form 1 data file


class Sector(unittest.TestCase):
    def test_header_is_bcd_msf_with_pregap(self):
        self.assertEqual(cs.lba_header(0), b"\x00\x02\x00\x02")
        self.assertEqual(cs.lba_header(14515), bytes.fromhex("03154002"))
        self.assertEqual(cs.lba_header(298499), bytes.fromhex("66217402"))

    def test_edc_known_values(self):
        self.assertEqual(cs.edc(b""), 0)
        # CRC-32/CD-ROM-EDC check value of "123456789"
        self.assertEqual(cs.edc(b"123456789"), 0x6EC2EDC4)

    def test_made_form1_sector_is_valid(self):
        s = cs.make_form1(1234, b"\x01\x02\x08\x00", b"hello" * 100)
        self.assertEqual(len(s), cs.SIZE)
        self.assertEqual(s[:12], cs.SYNC)
        self.assertEqual(s[16:24], b"\x01\x02\x08\x00" * 2)
        self.assertTrue(cs.form1_valid(s))
        s[100] ^= 1
        self.assertFalse(cs.form1_valid(s))

    def test_ecc_ignores_header(self):
        a = cs.make_form1(10, b"\0\0\x08\0", b"x")
        b = cs.make_form1(11, b"\0\0\x08\0", b"x")
        self.assertEqual(a[cs.P_OFF:], b[cs.P_OFF:])

    def test_form2_edc_zero_is_kept(self):
        s = bytearray(cs.SIZE)
        s[18] = s[22] = cs.SUB_FORM2
        cs.finish_form2(s)
        self.assertEqual(s[cs.FORM2_EDC:], b"\0\0\0\0")
        s[cs.FORM2_EDC] = 1
        cs.finish_form2(s)
        self.assertEqual(s[cs.FORM2_EDC:], cs.edc(s[16:cs.FORM2_EDC]).to_bytes(4, "little"))


def sha1_file(path):
    return mp.sha1_file(path)


@unittest.skipUnless(HAVE_DISC, "original track 1 and 2 are unavailable")
class Disc(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.mkdtemp(dir=os.environ.get("TOKIMEMO_PATCH_TMP"))
        cls.orig = pd.Track(TRACK1)

    @classmethod
    def tearDownClass(cls):
        cls.orig.close()
        shutil.rmtree(cls.tmp)

    def disc(self):
        d = pd.Disc(TRACK1)
        self.addCleanup(d.close)
        return d

    def out(self, name):
        return os.path.join(self.tmp, name)

    def test_every_form1_sector_recomputes_identically(self):
        bad = []
        form2_edc = set()
        with open(TRACK1, "rb") as f:
            for lba in range(self.orig.count):
                raw = f.read(cs.SIZE)
                if cs.has_subheader(raw) and cs.is_form2(raw):
                    form2_edc.add(raw[cs.FORM2_EDC:])
                    continue
                if not cs.has_subheader(raw) and lba != DAMAGED_DIR_LBA:
                    continue  # 67338: an XA sector with a broken subheader, never written
                s = bytearray(raw)
                cs.finish_form1(s)
                if bytes(s) != raw:
                    bad.append(lba)
        self.assertEqual(bad, [HEADER_ECC_LBA])
        self.assertEqual(form2_edc, {b"\0\0\0\0"})  # no Form 2 sector carries an EDC
        raw = self.orig.raw(HEADER_ECC_LBA)
        self.assertEqual(raw[cs.FORM1_EDC:cs.P_OFF], cs.edc(raw[16:cs.FORM1_EDC]).to_bytes(4, "little"))
        self.assertEqual(raw[cs.P_OFF:], cs.ecc(raw, zero_address=False))

    def test_tree(self):
        iso = pd.Iso(self.orig)
        self.assertEqual(iso.find("SLPM_86.053").lba, 24)
        self.assertEqual(iso.find("CDROM").lba, DAMAGED_DIR_LBA)
        self.assertEqual(iso.find("CDROM/EXEDIR/DATE.EXN").lba, 37418)
        self.assertEqual(len(iso.files()), 1372)
        self.assertEqual(iso.data_end(), 298350)
        self.assertEqual(iso.volume_space, 298500 + os.path.getsize(TRACK2) // cs.SIZE)
        self.assertTrue(iso.is_stream(iso.find("OPENXA.STR")))
        self.assertFalse(iso.is_stream(iso.find(SAMPLE)))

    def test_rebuild_without_changes_is_identical(self):
        d = self.disc()
        self.assertEqual(d.write(os.devnull), mp.SOURCE_SHA1)

    def test_damaged_directory_record_update_keeps_the_sector(self):
        d = self.disc()
        e = d.iso.find("CDROM/BATTLE")
        self.assertEqual(e.rec_lba, DAMAGED_DIR_LBA)
        d._update_record(e, e.lba, e.size)
        self.assertEqual(bytes(d.sectors[DAMAGED_DIR_LBA]), self.orig.raw(DAMAGED_DIR_LBA))

    @unittest.skipUnless(os.path.isfile(MATCH_EXE), "matching build of SLPM_86.053 is unavailable")
    def test_matching_exe_gives_identical_track(self):
        d = self.disc()
        with open(MATCH_EXE, "rb") as f:
            self.assertEqual(d.replace("SLPM_86.053", f.read()), 24)
        self.assertEqual(d.write(os.devnull), mp.SOURCE_SHA1)

    def test_streams_are_refused(self):
        d = self.disc()
        for path in ("OPENXA.STR", "CDROM/XA_G/YUMI.XA", "CDROM/DADIR/TR02.DA"):
            with self.assertRaises(pd.DiscError):
                d.replace(path, b"x")

    def changed_sectors(self, path, count):
        """LBAs where the track at `path` differs from the original (first `count` sectors)."""
        out = []
        with open(TRACK1, "rb") as a, open(path, "rb") as b:
            for lba in range(count):
                if a.read(cs.SIZE) != b.read(cs.SIZE):
                    out.append(lba)
        return out

    def test_in_place_replacement(self):
        d = self.disc()
        e = d.iso.find(SAMPLE)
        lba, sectors = e.lba, e.sectors
        data = bytearray(d.iso.read(SAMPLE))
        data[5000] ^= 0xFF
        data = bytes(data[:len(data) - 3 * cs.FORM1_DATA - 100])  # 3 sectors fewer, last one partial
        self.assertEqual(d.replace(SAMPLE, data), lba)
        out = self.out("inplace.bin")
        d.write(out)
        t = pd.Track(out)
        self.assertEqual(t.count, self.orig.count)
        iso = pd.Iso(t)
        self.assertEqual(iso.read(SAMPLE), data)
        self.assertEqual((iso.find(SAMPLE).lba, iso.find(SAMPLE).size), (lba, len(data)))
        changed = self.changed_sectors(out, t.count)
        self.assertEqual(changed, sorted([e.rec_lba, lba + 2] + list(range(lba + sectors - 4, lba + sectors))))
        for x in changed:
            self.assertTrue(cs.form1_valid(t.raw(x)), x)
        new_end, old_end = t.raw(lba + sectors - 4), self.orig.raw(lba + sectors - 1)
        self.assertEqual(new_end[16:24], old_end[16:24])  # EOR/EOF on the new last sector
        self.assertEqual(t.raw(lba + sectors - 1)[16:24], bytes(8))  # freed: empty filler
        t.close()

    def test_relocation_round_trip(self):
        d = self.disc()
        old = d.iso.find(SAMPLE)
        old_lba, old_sectors, rec_lba = old.lba, old.sectors, old.rec_lba
        data = d.iso.read(SAMPLE) + b"\x5A" * 3000
        n = (len(data) + cs.FORM1_DATA - 1) // cs.FORM1_DATA
        self.assertEqual(d.replace(SAMPLE, data), 298350)
        exe = d.iso.read("SLPM_86.053")
        self.assertEqual(d.replace("SLPM_86.053", exe, relocate=True), 298350 + n)
        out = self.out("moved.bin")
        cue = self.out("moved.cue")
        d.write(out, cue, TRACK2)
        added = n + len(exe) // cs.FORM1_DATA
        t = pd.Track(out)
        self.assertEqual(t.count, self.orig.count + added)
        iso = pd.Iso(t)
        self.assertEqual(iso.read(SAMPLE), data)
        self.assertEqual(iso.read("SLPM_86.053"), exe)
        self.assertEqual(iso.find("SLPM_86.053").lba, 298350 + n)
        self.assertEqual(iso.volume_space, d.iso.volume_space)
        self.assertEqual(iso.volume_space, pd.Iso(self.orig).volume_space + added)
        self.assertEqual(iso.find("CDROM/DADIR/TR02.DA").lba, 298650 + added)
        self.assertEqual(iso.data_end(), 298350 + added)
        for lba in range(298350, t.count):
            raw = t.raw(lba)
            self.assertEqual(raw[12:16], cs.lba_header(lba))
            self.assertTrue(cs.form1_valid(raw), lba)
        self.assertEqual(t.raw(298350 + n - 1)[16:24], self.orig.raw(old_lba + old_sectors - 1)[16:24])
        changed = self.changed_sectors(out, 298350)
        tr02 = pd.Iso(self.orig).find("CDROM/DADIR/TR02.DA").rec_lba
        self.assertEqual(changed, sorted({pd.PVD_LBA, iso.find("SLPM_86.053").rec_lba, rec_lba, tr02}))
        with open(cue) as f:
            text = f.read()
        self.assertIn('FILE "moved.bin" BINARY', text)
        self.assertIn('FILE "%s" BINARY' % os.path.relpath(os.path.abspath(TRACK2), self.tmp), text)
        self.assertIn("INDEX 01 00:02:00", text)
        t.close()

    def test_each_file_once(self):
        d = self.disc()
        d.replace(SAMPLE, b"a")
        with self.assertRaises(pd.DiscError):
            d.replace(SAMPLE, b"b")


@unittest.skipUnless(HAVE_XDELTA, "xdelta3 not installed (image of tools/Dockerfile.patch)")
class Xdelta(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.mkdtemp(dir=os.environ.get("TOKIMEMO_PATCH_TMP"))

    def tearDown(self):
        shutil.rmtree(self.tmp)

    def disc(self):
        d = pd.Disc(TRACK1)
        self.addCleanup(d.close)
        return d

    def path(self, name, data=None):
        p = os.path.join(self.tmp, name)
        if data is not None:
            with open(p, "wb") as f:
                f.write(data)
        return p

    def test_synthetic_round_trip(self):
        src_data = bytes(range(256)) * 4096
        tgt_data = bytearray(src_data)
        tgt_data[1000:1010] = b"0123456789"
        tgt_data += b"appended" * 1000
        src, tgt = self.path("a", src_data), self.path("b", bytes(tgt_data))
        want = hashlib.sha1(src_data).hexdigest()
        patch = self.path("p.xdelta")
        self.assertEqual(mp.make(src, tgt, patch, want), hashlib.sha1(tgt_data).hexdigest())
        with open(patch, "rb") as f:
            self.assertEqual(f.read(4), b"\xd6\xc3\xc4\x00")  # VCDIFF magic, version 0
        out = self.path("c")
        mp.apply(src, patch, out, want, hashlib.sha1(tgt_data).hexdigest())
        with open(out, "rb") as f:
            self.assertEqual(f.read(), bytes(tgt_data))

    def test_wrong_source_is_refused(self):
        src, tgt = self.path("a", b"one" * 100), self.path("b", b"two" * 100)
        with self.assertRaises(mp.PatchError):
            mp.make(src, tgt, self.path("p"))  # not the original track
        patch = self.path("p")
        mp.make(src, tgt, patch, "any")
        with self.assertRaises(mp.PatchError):
            mp.apply(self.path("x", b"three"), patch, self.path("out"))
        self.assertFalse(os.path.exists(self.path("out")))

    @unittest.skipUnless(HAVE_DISC, "original track 1 and 2 are unavailable")
    def test_track_round_trip(self):
        d = self.disc()
        data = bytearray(d.iso.read(SAMPLE))
        data[123] ^= 1
        d.replace(SAMPLE, bytes(data))
        d.replace("CDROM/EXEDIR/O.BIN", d.iso.read("CDROM/EXEDIR/O.BIN"), relocate=True)
        tgt = self.path("t1.bin")
        want = d.write(tgt)
        patch = self.path("t1.xdelta")
        self.assertEqual(mp.make(TRACK1, tgt, patch), want)
        self.assertLess(os.path.getsize(patch), 1 << 20)
        out = self.path("t1.out")
        self.assertEqual(mp.apply(TRACK1, patch, out, out_sha1=want), want)


if __name__ == "__main__":
    unittest.main()
