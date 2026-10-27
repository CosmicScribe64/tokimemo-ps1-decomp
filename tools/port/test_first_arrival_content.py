"""Focused tests for the first-arrival original-transfer pack."""
from __future__ import annotations

import hashlib
import json
from pathlib import Path
import shutil
import tempfile
import uuid
import unittest

from tools.port import first_arrival_content as arrival


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


class FakeSectorSource:
    def __init__(self, *, count: int, start_lba: int, bad_index: int = -1, bad_field: str = ""):
        self.count = count
        self.start_lba = start_lba
        self.bad_index = bad_index
        self.bad_field = bad_field

    def iter_raw_sectors(self, entry, expected_sha):
        assert expected_sha == arrival.TRACK_SHA256
        assert entry["size"] == self.count * 2048
        for index in range(self.count):
            payload = bytes([index & 0xFF]) * 2048
            raw = bytes([0x80 + (index & 0x7F)]) * 2352
            row = {
                "lba": self.start_lba + index,
                "form": 1,
                "form_name": "Form1",
                "subheader_valid": True,
                "payload_size": len(payload),
                "raw_size": len(raw),
                "payload": payload,
                "raw": raw,
                "payload_sha256": digest(payload),
                "raw_sha256": digest(raw),
            }
            if index == self.bad_index:
                if self.bad_field == "lba":
                    row["lba"] += 1
                elif self.bad_field == "form":
                    row["form"] = 2
                    row["form_name"] = "Form2"
                elif self.bad_field == "subheader":
                    row["subheader_valid"] = False
                elif self.bad_field == "payload_size":
                    row["payload_size"] = 2047
                elif self.bad_field == "raw_size":
                    row["raw_size"] = 2351
            yield row


class FirstArrivalContentTests(unittest.TestCase):
    def test_native_extent_preserves_exact_payload_and_sector_hashes(self):
        entry = {"path": arrival.BACKGROUND_PATH}
        data, proof = arrival._native_extent(
            FakeSectorSource(count=10, start_lba=arrival.BACKGROUND_LBA),
            entry, arrival.TRACK_SHA256, 10, arrival.BACKGROUND_LBA)
        self.assertEqual(len(data), 20480)
        self.assertEqual(proof["payload_bytes"], 20480)
        self.assertEqual(proof["raw_sector_bytes"], 23520)
        self.assertEqual(len(proof["sectors"]), 10)
        self.assertEqual([s["lba"] for s in proof["sectors"]],
                         list(range(arrival.BACKGROUND_LBA, arrival.BACKGROUND_LBA + 10)))
        self.assertEqual(proof["payload_sha256"], digest(data))

    def test_native_extent_rejects_unverified_or_wrong_sectors(self):
        entry = {"path": arrival.BACKGROUND_PATH}
        for field in ("lba", "form", "subheader", "payload_size", "raw_size"):
            with self.subTest(field=field):
                with self.assertRaises(arrival.FirstArrivalError):
                    arrival._native_extent(
                        FakeSectorSource(count=2, start_lba=arrival.BACKGROUND_LBA,
                                         bad_index=1, bad_field=field),
                        entry, arrival.TRACK_SHA256, 2, arrival.BACKGROUND_LBA)
        with self.assertRaises(arrival.FirstArrivalError):
            arrival._native_extent(FakeSectorSource(count=1, start_lba=arrival.BACKGROUND_LBA),
                                   entry, arrival.TRACK_SHA256, 2, arrival.BACKGROUND_LBA)

    def test_original_pack_keeps_neighbor_overread_separate_and_actor_exact(self):
        root = arrival._source_root()
        manifest, files = arrival.build_pack(root / "disc/track1.bin", source_root=root)
        self.assertEqual(manifest["schema"], arrival.SCHEMA)
        background = manifest["native_transfers"]["background"]
        decoder = files["background/P_01_01.BS"]
        transfer = files[background["file"]]
        self.assertEqual((len(decoder), len(transfer)), (18432, 20480))
        self.assertEqual(transfer[:len(decoder)], decoder)
        self.assertEqual(digest(transfer[-2048:]), background["adjacent_extent"]["sha256"])
        self.assertEqual(background["adjacent_extent"]["path"], "CDROM/BS/P_01_02.BS")
        self.assertEqual(background["adjacent_extent"]["lba"], 0x4954)
        self.assertEqual(background["overread_used_for_decoder"], False)
        actor = manifest["native_transfers"]["actor"]
        self.assertEqual(len(files[actor["file"]]), 26624)
        self.assertEqual(digest(files[actor["file"]]), actor["sha256"])
        self.assertEqual(actor["sectors"], 13)
        self.assertEqual(actor["destination"], 0x80180000)
        profile = manifest["runtime_profile"]
        self.assertEqual(profile["actor96_initial_draw_flags"], 0x24)
        self.assertFalse(profile["actor96_visible_rendering"])
        self.assertFalse(profile["native_visible_composition"])
        self.assertFalse(profile["hardware_idct_color_parity"])
        self.assertFalse(profile["complete_arrival_scene"])
        self.assertFalse(manifest["capabilities"]["complete_arrival_frame"])
        for name, metadata in manifest["files"].items():
            self.assertEqual(metadata["size"], len(files[name]))
            self.assertEqual(metadata["sha256"], digest(files[name]))
            if name.lower().endswith(".png"):
                self.assertTrue(files[name].startswith(arrival.PNG_SIGNATURE))

    def test_manifest_files_reject_unsafe_names_and_bad_pngs(self):
        with self.assertRaises(arrival.FirstArrivalError):
            arrival._validate_generated_file("../escape.bin", b"x")
        with self.assertRaises(arrival.FirstArrivalError):
            arrival._validate_generated_file("..\\escape.bin", b"x")
        with self.assertRaises(arrival.FirstArrivalError):
            arrival._validate_generated_file("a\x00b.bin", b"x")
        with self.assertRaises(arrival.FirstArrivalError):
            arrival._validate_generated_file("preview.png", b"not a png")

    def test_publish_is_candidate_scoped_and_refuses_overwrite(self):
        with tempfile.TemporaryDirectory(prefix="first-arrival-outside-") as outside:
            with self.assertRaises(arrival.FirstArrivalError):
                arrival._publish(Path(outside) / "pack", {"schema": arrival.SCHEMA}, {})
        root = arrival.PROJECT_ROOT
        cache = root / ".asset-cache/port"
        cache.mkdir(parents=True, exist_ok=True)
        target = cache / "test-first-arrival-content"
        if target.exists():
            shutil.rmtree(target)
        try:
            manifest = {"schema": arrival.SCHEMA, "files": {"probe.bin": {"size": 1}}}
            path = arrival._publish(target, manifest, {"probe.bin": b"x"})
            self.assertTrue(path.is_file())
            self.assertEqual(json.loads(path.read_text())["schema"], arrival.SCHEMA)
            with self.assertRaises(arrival.FirstArrivalError):
                arrival._publish(target, manifest, {"probe.bin": b"x"})
        finally:
            if target.exists():
                shutil.rmtree(target)

    def test_non_original_input_is_rejected(self):
        with tempfile.TemporaryDirectory(prefix="first-arrival-input-") as temp:
            bad = Path(temp) / "not-track.bin"
            bad.write_bytes(b"not an original MODE2 track")
            with self.assertRaises(arrival.FirstArrivalError):
                arrival.build_pack(bad, source_root=arrival._source_root())

    def test_exported_tree_matches_manifest_and_has_no_unlisted_files(self):
        root = arrival._source_root()
        cache = arrival.PROJECT_ROOT / ".asset-cache/port"
        cache.mkdir(parents=True, exist_ok=True)
        target = cache / ("test-first-arrival-" + uuid.uuid4().hex)
        try:
            manifest_path = arrival.export(root / "disc/track1.bin", target, source_root=root)
            pack = manifest_path.parent
            manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
            expected = set(manifest["files"])
            actual = {p.relative_to(pack).as_posix() for p in pack.rglob("*")
                      if p.is_file() and p.name != "manifest.json"}
            self.assertEqual(actual, expected)
            for relative, metadata in manifest["files"].items():
                path = pack / relative
                self.assertFalse(path.is_symlink())
                data = path.read_bytes()
                self.assertEqual(len(data), metadata["size"])
                self.assertEqual(digest(data), metadata["sha256"])
                if metadata["kind"] == "png":
                    self.assertTrue(data.startswith(arrival.PNG_SIGNATURE))
            self.assertEqual(manifest["producer"]["source_sha256"],
                             digest(Path(arrival.__file__).read_bytes()))
        finally:
            if target.exists():
                shutil.rmtree(target)


if __name__ == "__main__":
    unittest.main()
