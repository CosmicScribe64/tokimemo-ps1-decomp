"""Synthetic tests for the read-only original input inventory."""
import os
import tempfile
import unittest
import zipfile

from assets.input import InputError, open_input


def record(name, lba, size, flags=0, file_unit=0, interleave_gap=0):
    n = len(name)
    length = 33 + n + (1 if n % 2 == 0 else 0)
    r = bytearray(length)
    r[0] = length
    r[2:6] = lba.to_bytes(4, "little")
    r[6:10] = lba.to_bytes(4, "big")
    r[10:14] = size.to_bytes(4, "little")
    r[14:18] = size.to_bytes(4, "big")
    r[25] = flags
    r[26] = file_unit
    r[27] = interleave_gap
    r[28:32] = b"\x01\x00\x00\x01"
    r[32] = n
    r[33:33+n] = name
    return bytes(r)


def image():
    sectors = [bytearray(2352) for _ in range(20)]
    for s in sectors:
        s[:12] = b"\x00" + b"\xff"*10 + b"\x00"
        s[15] = 2
        s[16:20] = s[20:24] = b"\x08\x00\x00\x00"
    pvd = bytearray(2048); pvd[:7] = b"\x01CD001\x01"
    pvd[80:84] = (20).to_bytes(4, "little")
    pvd[84:88] = (20).to_bytes(4, "big")
    pvd[156:190] = record(b"\x00", 18, 2048, 2)[:34]
    sectors[16][24:24+2048] = pvd
    d = record(b"\x00", 18, 2048, 2) + record(b"\x01", 18, 2048, 2)
    d += record(b"FILE.BIN;1", 19, 5)
    sectors[18][24:24+len(d)] = d
    sectors[19][24:29] = b"hello"
    return b"".join(bytes(s) for s in sectors)


class InputTests(unittest.TestCase):
    def test_raw_and_zip_inventory_read(self):
        with tempfile.TemporaryDirectory() as td:
            raw = os.path.join(td, "track.bin")
            data = image()
            with open(raw, "wb") as f: f.write(data)
            with open_input(raw) as src:
                files = src.entries()
                entry = next(e for e in files if e["path"] == "FILE.BIN")
                self.assertEqual(src.read(entry), b"hello")
                self.assertEqual(entry["location"]["lba"], 19)
            archive = os.path.join(td, "game.zip")
            with zipfile.ZipFile(archive, "w") as z: z.writestr("(Track 1).bin", data)
            with open_input(archive) as src:
                self.assertEqual(src.read(next(e for e in src.entries() if e["path"] == "FILE.BIN")), b"hello")
                self.assertEqual(src.describe()["track_member"], "(Track 1).bin")

    def test_cached_raw_entry_can_be_read_before_inventory(self):
        cached_entry = {"path": "FILE.BIN", "size": 5, "kind": "file",
                        "location": {"lba": 19}}
        with tempfile.TemporaryDirectory() as td:
            track_data = image()
            raw = os.path.join(td, "track.bin")
            with open(raw, "wb") as f:
                f.write(track_data)
            with open_input(raw) as src:
                self.assertEqual(src.read(cached_entry), b"hello")
                self.assertIsNone(src._entries)

            archive = os.path.join(td, "track.zip")
            with zipfile.ZipFile(archive, "w") as z:
                z.writestr("track.bin", track_data)
            with open_input(archive) as src:
                self.assertEqual(src.read(cached_entry), b"hello")
                self.assertIsNone(src._entries)

    def test_directory_entries_and_symlink_are_safe(self):
        with tempfile.TemporaryDirectory() as td, tempfile.TemporaryDirectory() as outside:
            with open(os.path.join(td, "a"), "wb") as f: f.write(b"x")
            with open(os.path.join(outside, "secret"), "wb") as f: f.write(b"no")
            os.symlink(os.path.join(outside, "secret"), os.path.join(td, "link"))
            with open_input(td) as src:
                entries = src.entries()
                self.assertEqual(src.read(next(e for e in entries if e["path"] == "a")), b"x")
                link = next(e for e in entries if e["path"] == "link")
                self.assertEqual(link["kind"], "unsupported")
                with self.assertRaises(InputError): src.read(link)

    def test_rejects_cooked_iso(self):
        with tempfile.TemporaryDirectory() as td:
            p = os.path.join(td, "disc.iso")
            with open(p, "wb") as f: f.write(b"\0" * 2048)
            with self.assertRaises(InputError): open_input(p)

    def test_rejects_bad_file_subheader_and_reports_interleave(self):
        with tempfile.TemporaryDirectory() as td:
            raw = bytearray(image())
            # Invalid duplicated XA subheader on the file sector must not be treated as payload.
            raw[19*2352+20] ^= 0x01
            p = os.path.join(td, "bad.bin")
            with open(p, "wb") as f: f.write(raw)
            with open_input(p) as src:
                with self.assertRaises(InputError): src.entries()

    def test_zip_cue_selects_first_track_and_multiple_cues_fail(self):
        with tempfile.TemporaryDirectory() as td:
            archive = os.path.join(td, "tracks.zip")
            with zipfile.ZipFile(archive, "w") as z:
                z.writestr("track-a.bin", image())
                z.writestr("track-b.img", image())
                z.writestr("disc.cue", 'FILE "track-b.img" BINARY\n  TRACK 01 MODE2/2352\n')
            with open_input(archive) as src:
                self.assertEqual(src.describe()["track_member"], "track-b.img")
            with zipfile.ZipFile(archive, "a") as z:
                z.writestr("second.cue", 'FILE "track-a.bin" BINARY\n')
            with self.assertRaisesRegex(InputError, "multiple CUE"):
                open_input(archive)

    def test_zip_without_cue_rejects_ambiguous_tracks(self):
        with tempfile.TemporaryDirectory() as td:
            archive = os.path.join(td, "tracks.zip")
            with zipfile.ZipFile(archive, "w") as z:
                z.writestr("one.img", image())
                z.writestr("two.img", image())
            with self.assertRaisesRegex(InputError, "ambiguous raw data tracks"):
                open_input(archive)

    def test_directory_only_subheader_fallback_and_interleaved_extent(self):
        data = bytearray(image())
        # The volume directory's malformed subheader is a known game-disc quirk.
        data[18*2352+20] ^= 0x01
        # Replace the file record with an interleaved one.
        directory = bytearray(data[18*2352+24:18*2352+24+2048])
        r = record(b"FILE.BIN;1", 19, 5, file_unit=1)
        directory[68:68+len(r)] = r
        data[18*2352+16:18*2352+16+2048] = directory
        with tempfile.TemporaryDirectory() as td:
            p = os.path.join(td, "disc.bin")
            with open(p, "wb") as f: f.write(data)
            with open_input(p) as src:
                entry = next(e for e in src.entries() if e["path"] == "FILE.BIN")
                self.assertEqual(entry["kind"], "unsupported")
                self.assertIn("interleaved", entry["unsupported"])


if __name__ == "__main__": unittest.main()
