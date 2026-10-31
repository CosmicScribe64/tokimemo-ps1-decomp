"""Synthetic contract tests for the installed disc pack exporter (T-39303)."""
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import tempfile
import unittest

MODULE = Path(__file__).with_name("disc_pack_content.py")
SPEC = importlib.util.spec_from_file_location("disc_pack_content", MODULE)
pack = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(pack)

SYNC = b"\x00" + b"\xff" * 10 + b"\x00"


def sector(payload: bytes, form: int = 1, mode: int = 2, subheader: bytes | None = None) -> bytes:
    sub = subheader if subheader is not None else bytes([0, 0, 0x08 if form == 1 else 0x20 | 0x04, 0]) * 2
    body = payload + b"\x00" * ((2048 if form == 1 else 2324) - len(payload))
    return (SYNC + b"\x00\x02\x00" + bytes([mode]) + sub + body).ljust(2352, b"\x00")


def record(name: bytes, lba: int, size: int, flags: int = 0) -> bytes:
    rec = bytearray(33 + len(name))
    rec[0] = len(rec)
    rec[2:6] = struct.pack("<I", lba)
    rec[10:14] = struct.pack("<I", size)
    rec[25] = flags
    rec[32] = len(name)
    rec[33:] = name
    return bytes(rec)


def image(files: dict[str, tuple[int, bytes, int]], extra: dict[int, bytes] | None = None) -> bytes:
    """files: name -> (lba, data, form). Root directory at LBA 20, PVD at 16."""
    sectors: dict[int, bytes] = {}
    pvd = bytearray(2048)
    pvd[0:6] = b"\x01CD001"
    pvd[40:72] = b"SYNTHVOL".ljust(32)
    pvd[156:190] = record(b"\x00", 20, 2048, 2)[:34].ljust(34, b"\x00")
    sectors[16] = sector(bytes(pvd))
    entries = b"".join(record(name.encode() + b";1", lba, len(data) if form == 1 else len(data) // 2324 * 2048)
                       for name, (lba, data, form) in files.items())
    sectors[20] = sector(entries)
    for name, (lba, data, form) in files.items():
        step = 2048 if form == 1 else 2324
        for index in range((len(data) + step - 1) // step):
            sectors[lba + index] = sector(data[index * step:(index + 1) * step], form)
    sectors.update(extra or {})
    total = max(sectors) + 1
    return b"".join(sectors.get(i, sector(b"")) for i in range(total))


class DiscPackExportTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="disc-pack-test-")
        self.root = Path(self.temp.name)

    def tearDown(self):
        self.temp.cleanup()

    def export(self, files, **kwargs):
        track = self.root / "track.bin"
        track.write_bytes(image(files, **kwargs))
        return pack.build(track, self.root / "out")

    def test_form1_files_are_stored_as_whole_sectors_with_hashes(self):
        data = bytes(range(256)) * 12  # 3072 bytes, two sectors, the second half-filled
        manifest = self.export({"SLPM_86.053": (30, b"exe" * 100, 1), "DATA.BIN": (40, data, 1)})
        by_path = {r["path"]: r for r in manifest["files"]}
        record = by_path["DATA.BIN"]
        self.assertEqual((record["lba"], record["sectors"], record["size"], record["stored_size"]), (40, 2, 3072, 4096))
        self.assertEqual((record["mode"], record["form"]), (2, 1))
        stored = (self.root / "out/files/DATA.BIN").read_bytes()
        self.assertEqual(len(stored), 4096)
        self.assertEqual(stored[:3072], data)
        self.assertEqual(stored[3072:], b"\x00" * 1024)
        self.assertEqual(record["sha256"], hashlib.sha256(stored).hexdigest())
        self.assertEqual(record["content_sha256"], hashlib.sha256(data).hexdigest())
        on_disk = json.loads((self.root / "out/disc_pack.json").read_text())
        self.assertEqual(on_disk, manifest)
        self.assertEqual(on_disk["schema"], "tokimemo.disc-pack.v1")
        self.assertEqual(on_disk["volume_id"], "SYNTHVOL")
        self.assertEqual(on_disk["totals"]["files"], 2)

    def test_streams_are_excluded_with_a_reason_and_have_no_data(self):
        manifest = self.export({"SLPM_86.053": (30, b"x", 1), "AUDIO.XA": (50, b"a" * 2324 * 3, 2)})
        self.assertEqual([r["path"] for r in manifest["files"]], ["SLPM_86.053"])
        self.assertEqual(len(manifest["excluded"]), 1)
        self.assertEqual(manifest["excluded"][0]["path"], "AUDIO.XA")
        self.assertIn("form 2", manifest["excluded"][0]["reason"])
        self.assertFalse((self.root / "out/files/AUDIO.XA").exists())

    def test_files_past_the_image_are_excluded(self):
        track = self.root / "track.bin"
        raw = image({"SLPM_86.053": (30, b"x", 1)})
        sectors = len(raw) // 2352
        extra = {20: sector(record(b"SLPM_86.053;1", 30, 1) + record(b"TR02.DA;1", sectors + 100, 4096))}
        track.write_bytes(image({"SLPM_86.053": (30, b"x", 1)}, extra=extra))
        manifest = pack.build(track, self.root / "out")
        self.assertEqual([e["path"] for e in manifest["excluded"]], ["TR02.DA"])

    def test_bad_sectors_fail_the_export(self):
        for label, bad in (("sync", b"\x01" + sector(b"x")[1:]), ("mode", sector(b"x", mode=1)),
                           ("subheader", sector(b"x", subheader=b"\x00\x00\x08\x00\x01\x00\x08\x00"))):
            with self.subTest(label):
                track = self.root / (label + ".bin")
                files = {"SLPM_86.053": (30, b"x" * 100, 1), "BAD.BIN": (40, b"y" * 100, 1)}
                raw = bytearray(image(files))
                raw[40 * 2352:41 * 2352] = bad
                track.write_bytes(bytes(raw))
                with self.assertRaises(pack.PackError):
                    pack.build(track, self.root / ("out-" + label))
                self.assertFalse((self.root / ("out-" + label)).exists())
                self.assertFalse((self.root / ("out-" + label + ".partial")).exists())

    def test_overlapping_extents_and_missing_executable_fail(self):
        with self.assertRaises(pack.PackError):
            self.export({"SLPM_86.053": (30, b"x" * 4096, 1), "B.BIN": (31, b"y", 1)})
        track = self.root / "noexe.bin"
        track.write_bytes(image({"A.BIN": (30, b"x", 1)}))
        with self.assertRaises(pack.PackError):
            pack.build(track, self.root / "out3")

    def test_output_must_be_new_and_cross_check_detects_a_difference(self):
        track = self.root / "track.bin"
        track.write_bytes(image({"SLPM_86.053": (30, b"exe", 1)}))
        (self.root / "exists").mkdir()
        with self.assertRaises(pack.PackError):
            pack.build(track, self.root / "exists")
        files = self.root / "files"
        files.mkdir()
        (files / "SLPM_86.053").write_bytes(b"exe")
        pack.build(track, self.root / "ok", files)
        (files / "SLPM_86.053").write_bytes(b"exf")
        with self.assertRaises(pack.PackError):
            pack.build(track, self.root / "differs", files)

    def test_export_is_deterministic(self):
        track = self.root / "track.bin"
        track.write_bytes(image({"SLPM_86.053": (30, b"exe", 1), "D.BIN": (35, b"d" * 5000, 1)}))
        pack.build(track, self.root / "one")
        pack.build(track, self.root / "two")
        self.assertEqual((self.root / "one/disc_pack.json").read_bytes(), (self.root / "two/disc_pack.json").read_bytes())


if __name__ == "__main__":
    unittest.main()
