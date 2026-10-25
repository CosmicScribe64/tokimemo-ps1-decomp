"""Synthetic tests for the read-only original input inventory."""
import os
import tempfile
import unittest
import zipfile
import hashlib
from pathlib import Path

from assets.input import InputError, open_input
from assets.input_stream import _validate_cached_stream, stage_form2_inventory, stage_raw_stream
from assets.manifest import CACHE, REPOSITORY, asset_id


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


def form2_image():
    data = bytearray(image())
    # The ISO record stores address units; this raw stream is one full unit.
    record_start = 18*2352 + 24 + 68
    data[record_start+10:record_start+14] = (2048).to_bytes(4, "little")
    data[record_start+14:record_start+18] = (2048).to_bytes(4, "big")
    # Mark the payload at LBA 19 as Form2 and fill its full 2324-byte region.
    data[19*2352+18] = data[19*2352+22] = 0x20
    data[19*2352+24:19*2352+2348] = bytes((i % 251 for i in range(2324)))
    return bytes(data)


class InputTests(unittest.TestCase):
    def test_raw_sector_iterator_reports_form_and_hashes_without_changing_read(self):
        with tempfile.TemporaryDirectory() as td:
            p = os.path.join(td, "disc.bin")
            data = bytearray(form2_image())
            with open(p, "wb") as f:
                f.write(data)
            with open_input(p) as src:
                entry = next(e for e in src.entries() if e["path"] == "FILE.BIN")
                self.assertEqual(entry["kind"], "unsupported")
                self.assertEqual(entry["location"]["sectors"][0]["lba"], 19)
                with self.assertRaisesRegex(InputError, "Form2"):
                    src.read(entry)
                sector, = list(src.iter_raw_sectors(entry, src.describe()["source_sha256"]))
                self.assertEqual(sector["form_name"], "Form2")
                self.assertEqual((sector["raw_size"], sector["payload_size"]), (2352, 2324))
                self.assertEqual(sector["payload"], data[19*2352+24:19*2352+2348])
                self.assertEqual(sector["raw_sha256"], hashlib.sha256(sector["raw"]).hexdigest())
                self.assertEqual(sector["edc_validation"], "not_performed")

    def test_raw_sector_iterator_validates_input_extent_and_subheader(self):
        with tempfile.TemporaryDirectory() as td:
            p = os.path.join(td, "disc.bin")
            data = bytearray(form2_image())
            with open(p, "wb") as f:
                f.write(data)
            with open_input(p) as src:
                entry = {"path": "X/STREAM.STR", "size": 2048, "kind": "unsupported",
                         "location": {"lba": 19, "extent_bytes": 2048}}
                with self.assertRaisesRegex(InputError, "SHA256"):
                    list(src.iter_raw_sectors(entry, "0" * 64))
                with self.assertRaisesRegex(InputError, "positive 2048"):
                    list(src.iter_raw_sectors(dict(entry, size=2049), src.describe()["source_sha256"]))
                with self.assertRaisesRegex(InputError, "crosses the end"):
                    list(src.iter_raw_sectors(dict(entry, location={"lba": 20}), src.describe()["source_sha256"]))
            data[19*2352+20] ^= 1
            with open(p, "wb") as f:
                f.write(data)
            with open_input(p) as src:
                with self.assertRaisesRegex(InputError, "duplicated XA"):
                    list(src.iter_raw_sectors(entry, src.describe()["source_sha256"]))

    def test_raw_stream_stage_binds_baseline_and_preserves_raw_bytes(self):
        root = Path(CACHE) / "input"
        root.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(prefix="test-stream-", dir=str(root)) as output_dir:
            with tempfile.TemporaryDirectory() as td:
                p = os.path.join(td, "disc.bin")
                data = form2_image()
                with open(p, "wb") as f:
                    f.write(data)
                with open_input(p) as src:
                    entry = next(e for e in src.entries() if e["path"] == "FILE.BIN")
                    baseline = {"input": src.describe(), "assets": [{"source": entry}]}
                    result = stage_raw_stream(src, baseline, entry, output_dir)
                    self.assertEqual(result["source"]["input_sha256"], src.describe()["source_sha256"])
                    self.assertEqual(result["source"]["lba"], 19)
                    self.assertEqual(result["sectors"][0]["form"], 2)
                    self.assertEqual(result["edc_validation"], "not_performed")
                    raw_path = Path(output_dir) / Path(result["raw_stream"]["path"]).name
                    self.assertEqual(raw_path.read_bytes(), data[19*2352:20*2352])
                    with self.assertRaisesRegex(InputError, "SHA256"):
                        stage_raw_stream(src, {"input": dict(src.describe(), source_sha256="0"*64)}, entry, output_dir)
                    altered = dict(entry, unsupported="ISO interleaved extent")
                    with self.assertRaisesRegex(InputError, "same identified Form2"):
                        stage_raw_stream(src, baseline, altered, output_dir)
            with tempfile.TemporaryDirectory(prefix="test-stream-link-", dir=str(root)) as output_dir:
                with tempfile.TemporaryDirectory() as td:
                    p = os.path.join(td, "disc.bin")
                    with open(p, "wb") as f:
                        f.write(form2_image())
                    with open_input(p) as src:
                        entry = next(e for e in src.entries() if e["path"] == "FILE.BIN")
                        baseline = {"input": src.describe(), "assets": [{"source": entry}]}
                        link = Path(output_dir) / (asset_id(entry["path"], src.describe()["source_sha256"]) + ".raw2352")
                        outside = Path(td) / "outside.raw"
                        outside.write_bytes(b"keep")
                        link.symlink_to(outside)
                        with self.assertRaisesRegex(InputError, "may not be symlinks"):
                            stage_raw_stream(src, baseline, entry, output_dir)

    def test_form2_inventory_aggregate_separates_raw_and_payload_counts(self):
        root = Path(CACHE) / "input"
        root.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(prefix="test-form2-batch-", dir=str(root)) as output_dir:
            with tempfile.TemporaryDirectory() as td:
                p = os.path.join(td, "disc.bin")
                with open(p, "wb") as f:
                    f.write(form2_image())
                with open_input(p) as src:
                    entry = next(e for e in src.entries() if e["path"] == "FILE.BIN")
                    baseline = {"input": src.describe(), "assets": [{"id": "synthetic-asset", "source": entry}]}
                    result = stage_form2_inventory(src, baseline, output_dir,
                                                   expected_count=1, reuse_dirs=[])
                    self.assertEqual(result["stream_count"], 1)
                    self.assertEqual(result["sector_count"], 1)
                    self.assertEqual(result["raw_bytes"], 2352)
                    self.assertEqual(result["payload_bytes"], 2324)
                    self.assertEqual(result["distributions"]["form"], {"2": 1})
                    self.assertEqual(result["distributions"]["file_channel"], {"8:0": 1})
                    self.assertIn("stream_manifest_path", result["streams"][0])
                    self.assertEqual(result["hash_semantics"]["iso_logical_file_sha256"], "not_computed; ISO byte size is used only to determine 2048-byte sector count")
                    self.assertTrue((Path(output_dir) / "aggregate.json").is_file())

    def test_opaque_mismatched_sector_preserves_bytes_and_never_claims_payload(self):
        root = Path(CACHE) / "input"
        root.mkdir(parents=True, exist_ok=True)
        data = bytearray(form2_image())
        # Keep Mode2 and sync valid, but change one byte in the duplicate copy.
        data[19*2352+22] ^= 1
        with tempfile.TemporaryDirectory(prefix="test-form2-opaque-", dir=str(root)) as output_dir:
            with tempfile.TemporaryDirectory() as td:
                p = os.path.join(td, "disc.bin")
                with open(p, "wb") as f:
                    f.write(data)
                with open_input(p) as src:
                    entry = {"path": "XA/OPAQUE.XA", "kind": "unsupported",
                             "unsupported": "Form2 XA/STR stream; ISO byte size does not describe payload",
                             "size": 2048, "location": {"lba": 19, "extent_bytes": 2048}}
                    sector, = list(src.iter_raw_sectors(
                        {"path": entry["path"], "kind": "unsupported", "size": 2048,
                         "location": entry["location"]}, src.describe()["source_sha256"],
                        allow_invalid_subheaders=True))
                    self.assertFalse(sector["subheader_valid"])
                    self.assertIsNone(sector["form"])
                    self.assertIsNone(sector["payload"])
                    self.assertEqual(sector["subheader_primary"], data[19*2352+16:19*2352+20].hex())
                    self.assertEqual(sector["subheader_secondary"], data[19*2352+20:19*2352+24].hex())
                    baseline = {"input": src.describe(), "assets": [{"id": "opaque-asset", "source": entry}]}
                    with self.assertRaisesRegex(InputError, "duplicated XA"):
                        list(src.iter_raw_sectors(entry, src.describe()["source_sha256"]))
                    result = stage_form2_inventory(src, baseline, output_dir,
                                                   expected_count=1, reuse_dirs=[])
                    self.assertEqual(result["status"], "partial")
                    self.assertEqual(result["invalid_subheader_sector_count"], 1)
                    self.assertEqual(result["interpreted_metadata_sector_count"], 0)
                    stream = result["streams"][0]
                    self.assertEqual(stream["payload_stream"]["sha256"], None)
                    self.assertFalse(stream["payload_stream"]["complete"])
                    self.assertEqual(stream["sectors"][0]["form"], None)
                    self.assertEqual(result["invalid_sector_records"][0]["subheader_primary"], "08002000")
                    raw_path = Path(REPOSITORY) / stream["raw_stream"]["path"]
                    self.assertEqual(raw_path.read_bytes(), data[19*2352:20*2352])
                    cache_manifest = Path(output_dir) / (asset_id(entry["path"], src.describe()["source_sha256"]) + ".json")
                    self.assertIsNone(_validate_cached_stream(cache_manifest, entry, baseline["assets"][0],
                                                              src.describe()["source_sha256"]))

    def test_opaque_mode_still_rejects_non_mode2_sector(self):
        data = bytearray(form2_image())
        data[19*2352+15] = 1
        with tempfile.TemporaryDirectory() as td:
            p = os.path.join(td, "disc.bin")
            with open(p, "wb") as f:
                f.write(data)
            with open_input(p) as src:
                entry = {"path": "XA/OPAQUE.XA", "kind": "unsupported", "size": 2048,
                         "location": {"lba": 19, "extent_bytes": 2048}}
                with self.assertRaisesRegex(InputError, "not MODE2"):
                    list(src.iter_raw_sectors(entry, src.describe()["source_sha256"],
                                              allow_invalid_subheaders=True))
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
