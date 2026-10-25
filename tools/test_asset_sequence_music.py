"""Invented-data tests for canonical sequence JSON and opt-in MIDI export."""
import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from tools.assets import manifest
from tools.assets.sequence_music import (
    UnsupportedEvent,
    export_smf,
    inspect_seq,
    parse_events,
    reconstruct_seq,
    validate_event_records,
)
from tools.assets.sequence_music_export import export_directory
from tools.assets.sequence_music_validate import validate_directory


def package(events1, events2=b"\x00\xc0\x03\x00\xff\x2f\x00"):
    """Build a two-entry invented 16-slot offset package with reverb bytes."""
    raw = bytearray(0xA0)
    raw[0:4] = (0x40).to_bytes(4, "little")
    raw[4:8] = (0x70).to_bytes(4, "little")
    raw[0x40:0x50] = bytes(range(0xA0, 0xB0))
    for start, events in ((0x50, events1), (0x80, events2)):
        raw[start : start + 4] = b"pQES"
        raw[start + 4 : start + 8] = (1).to_bytes(4, "big")
        raw[start + 8 : start + 10] = (480).to_bytes(2, "big")
        raw[start + 10 : start + 13] = (500000).to_bytes(3, "big")
        raw[start + 13 : start + 15] = b"\x04\x02"
        raw[start + 15 : start + 15 + len(events)] = events
    return bytes(raw)


class SequenceMusicTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name).resolve()
        self.cache = self.root / ".asset-cache"
        self.source = self.root / "disc" / "files" / "CDROM" / "VAB_SEQ"
        self.source.mkdir(parents=True)
        self.source_file = self.source / "fixture.SEQ"
        self.source_bytes = package(bytes.fromhex("00903c40023c0000c00200b0631400b0067f00b0631e00ff2f00"))
        self.source_file.write_bytes(self.source_bytes)
        self.repo_patch = patch.object(manifest, "REPOSITORY", self.root)
        self.cache_patch = patch.object(manifest, "CACHE", self.cache)
        self.repo_patch.start()
        self.cache_patch.start()

    def tearDown(self):
        self.cache_patch.stop()
        self.repo_patch.stop()
        self.temp.cleanup()

    def test_structured_reconstruction_proves_fields_and_preserved_gaps(self):
        parsed = inspect_seq(self.source_bytes, "disc/files/CDROM/VAB_SEQ/fixture.SEQ")
        self.assertEqual(reconstruct_seq(parsed, self.source_bytes), self.source_bytes)
        self.assertEqual(parsed["offset_table_le_u32"][:2], [0x40, 0x70])
        self.assertEqual(parsed["reverb_depth_raw_bytes"], list(range(0xA0, 0xB0)))
        self.assertEqual([s["offset"] for s in parsed["sequences"]], [0x50, 0x80])
        self.assertEqual(parsed["sequence_slots"][1]["resolved_offset"], 0x80)
        self.assertEqual(parsed["sequence_slots"][2]["resolved_offset"], 0x50)
        for mutation in (
            lambda r: r["offset_table_le_u32"].__setitem__(0, 0x41),
            lambda r: r["reverb_depth_raw_bytes"].__setitem__(0, 0),
            lambda r: r["sequences"][0]["header"].__setitem__("tempo_u24_be", 1),
            lambda r: r["sequences"][0]["score"].__setitem__("raw_hex", "00"),
        ):
            damaged = json.loads(json.dumps(parsed))
            mutation(damaged)
            with self.assertRaises(ValueError):
                reconstruct_seq(damaged, self.source_bytes)

    def test_event_records_validate_running_status_timing_order_and_midi(self):
        package_record = inspect_seq(self.source_bytes)
        sequence = package_record["sequences"][0]
        events = parse_events(self.source_bytes, sequence)
        validate_event_records(self.source_bytes, sequence, events)
        self.assertEqual(events["events"][1]["tick"], 2)
        self.assertTrue(events["events"][1]["running_status"])
        damaged = json.loads(json.dumps(events))
        damaged["events"][0]["data"][0] ^= 1
        with self.assertRaisesRegex(ValueError, "event records"):
            validate_event_records(self.source_bytes, sequence, damaged)
        midi = export_smf(sequence, events)
        self.assertTrue(midi.startswith(b"MThd"))
        self.assertIn(b"\xff\x51\x03\x07\xa1\x20", midi)
        self.assertIn(b"\xb0\x63\x14", midi)
        self.assertIn(b"\xb0\x63\x1e", midi)

    def test_fail_closed_retains_source_for_sysEx_meta_and_unknown_controller(self):
        for body, reason in (
            (bytes.fromhex("00f00301ff2f00ff2f00"), "unsupported channel/system status f0"),
            (bytes.fromhex("00ff5103ff2f0000ff2f00"), "unsupported meta event 51"),
            (bytes.fromhex("00b0010100ff2f00"), "unsupported control-change number 1"),
        ):
            raw = package(body)
            rec = inspect_seq(raw)
            with self.assertRaisesRegex(UnsupportedEvent, reason):
                parse_events(raw, rec["sequences"][0])
            self.assertEqual(reconstruct_seq(rec, raw), raw)

    def test_bad_vlq_data_status_and_table_targets_reject(self):
        for body, reason in (
            (bytes.fromhex("818181819000ff2f00"), "four-byte"),
            (bytes.fromhex("81"), "truncated delta-time"),
            (bytes.fromhex("00903c"), "truncated channel event data"),
            (bytes.fromhex("003c4000ff2f00"), "running status used before channel"),
        ):
            raw = package(body)
            rec = inspect_seq(raw)
            seq = rec["sequences"][0]
            if body == bytes.fromhex("81") or body == bytes.fromhex("00903c"):
                seq = dict(seq, bound_end=seq["offset"] + 0x0F + len(body))
            with self.assertRaisesRegex(ValueError, reason):
                parse_events(raw, seq)
        raw = bytearray(self.source_bytes)
        raw[4:8] = (0x48).to_bytes(4, "little")
        with self.assertRaisesRegex(ValueError, "not to SEQp"):
            inspect_seq(bytes(raw))

    def test_eot_pattern_in_delta_vlq_does_not_terminate_before_event(self):
        # FF 2F is a valid two-byte delta (0x3faf = 16303), not EOT.
        # Both running-status and explicit-status following events stay intact.
        cases = (
            ("00903c40ff2f003c00903d4000ff2f00", 3, 16303),
            ("00903c40ff2f003c01ff2f00", 2, 16304),
        )
        for encoded, count, end_tick in cases:
            body = bytes.fromhex(encoded)
            raw = package(body)
            rec = inspect_seq(raw)
            seq = dict(rec["sequences"][0], bound_end=rec["sequences"][0]["offset"] + 0x0F + len(body))
            parsed = parse_events(raw, seq)
            self.assertEqual(len(parsed["events"]), count)
            self.assertEqual(parsed["end_tick"], end_tick)
        missing_eot_delta = bytes.fromhex("00903c40ff2f00")
        malformed = package(missing_eot_delta)
        malformed_rec = inspect_seq(malformed)
        malformed_seq = dict(malformed_rec["sequences"][0], bound_end=malformed_rec["sequences"][0]["offset"] + 0x0F + len(missing_eot_delta))
        with self.assertRaises(ValueError):
            parse_events(malformed, malformed_seq)

    def test_validator_rejects_rehashed_serialized_metadata_mutations(self):
        attacks = (
            "unsupported_laundering",
            "native_header_tempo",
            "manifest_asset_id",
            "manifest_source_sha256",
            "manifest_source_path",
            "manifest_physical_offset",
            "coverage_physical_sequences",
            "coverage_decoded_source_events",
            "duplicate_manifest_file_row",
            "extra_manifest_file_row",
        )
        for attack in attacks:
            with self.subTest(attack=attack):
                output = self.cache / ("validator-" + attack)
                export_directory(self.source, output)
                package_path = next((output / "packages").glob("*.json"))
                package_record = json.loads(package_path.read_text())
                manifest_path = output / "manifest.json"
                manifest_record = json.loads(manifest_path.read_text())
                file_row = manifest_record["files"][0]
                entry = package_record["entries"][0]
                identity = entry["identity"]
                sequence_row = file_row["sequences"][0]
                if attack == "unsupported_laundering":
                    entry["event_parse"]["events"][0]["data"][0] ^= 1
                    entry["unsupported_reason"] = "stored event parse differs from source parse"
                    identity["event_status"] = sequence_row["event_status"] = "unsupported"
                    identity["event_count"] = sequence_row["event_count"] = None
                    manifest_record["coverage"]["unsupported_sequence_entries"] += 1
                    manifest_record["coverage"]["decoded_source_events"] -= len(entry["event_parse"]["events"])
                elif attack == "native_header_tempo":
                    entry["native_entry"]["header"]["tempo_u24_be"] ^= 1
                elif attack == "manifest_asset_id":
                    identity["asset_id"] = sequence_row["asset_id"] = "fabricated"
                elif attack == "manifest_source_sha256":
                    identity["source_sha256"] = sequence_row["source_sha256"] = "0" * 64
                elif attack == "manifest_source_path":
                    identity["source_relative_path"] = sequence_row["source_relative_path"] = "disc/fabricated.SEQ"
                elif attack == "manifest_physical_offset":
                    identity["physical_offset"] += 1
                    sequence_row["physical_offset"] += 1
                elif attack == "coverage_physical_sequences":
                    manifest_record["coverage"]["physical_sequences"] += 1
                elif attack == "coverage_decoded_source_events":
                    manifest_record["coverage"]["decoded_source_events"] += 1
                elif attack == "duplicate_manifest_file_row":
                    manifest_record["files"].insert(0, json.loads(json.dumps(file_row)))
                elif attack == "extra_manifest_file_row":
                    extra = json.loads(json.dumps(file_row))
                    extra["asset_id"] = "forged-package"
                    extra["source"]["path"] = "disc/fabricated.SEQ"
                    manifest_record["files"].append(extra)
                payload = (json.dumps(package_record, ensure_ascii=True, sort_keys=True, indent=2) + "\n").encode()
                package_path.unlink()
                manifest.write_bytes(package_path, payload)
                file_row["output"]["sha256"] = manifest.digest(payload)
                manifest_path.unlink()
                manifest.write_manifest(manifest_path, manifest_record)
                with self.assertRaises(ValueError):
                    validate_directory(self.source, output)

    def test_manifest_ids_are_source_path_hash_and_physical_offset_bound(self):
        record = inspect_seq(self.source_bytes)
        source_rel = "disc/files/CDROM/VAB_SEQ/fixture.SEQ"
        source_hash = manifest.digest(self.source_bytes)
        package_id = manifest.asset_id(source_rel, source_hash)
        sequence_id = manifest.asset_id(source_rel + "#seq@00000050", source_hash)
        self.assertNotEqual(package_id, sequence_id)
        self.assertNotEqual(sequence_id, manifest.asset_id(source_rel + "#seq@00000080", source_hash))
        self.assertEqual(len(record["sequence_slots"]), 16)

    def test_export_json_is_default_stable_and_midi_is_explicit_opt_in(self):
        output = self.cache / "music" / "sequence-json"
        result = export_directory(self.source, output)
        self.assertEqual(result["coverage"]["files"], 1)
        self.assertEqual(result["coverage"]["physical_sequences"], 2)
        self.assertEqual(result["coverage"]["midi_previews_exported"], 0)
        self.assertFalse((output / "midi").exists())
        manifest_before = (output / "manifest.json").read_bytes()
        manifest_data = json.loads(manifest_before)
        file_row = manifest_data["files"][0]
        source_rel = "disc/files/CDROM/VAB_SEQ/fixture.SEQ"
        source_hash = manifest.digest(self.source_bytes)
        self.assertEqual(file_row["asset_id"], manifest.asset_id(source_rel, source_hash))
        self.assertIn(file_row["asset_id"], file_row["output"]["path"])
        package_json = json.loads((self.root / file_row["output"]["path"]).read_text())
        for entry in package_json["entries"]:
            offset = entry["identity"]["physical_offset"]
            self.assertEqual(entry["identity"]["source_relative_path"], source_rel)
            self.assertEqual(entry["identity"]["asset_id"], manifest.asset_id("%s#seq@%08x" % (source_rel, offset), source_hash))
        export_directory(self.source, output)
        self.assertEqual((output / "manifest.json").read_bytes(), manifest_before)
        validate_directory(self.source, output)

        preview = self.cache / "music" / "sequence-midi"
        optin = export_directory(self.source, preview, midi_preview_assuming_u24_is_mpqn=True)
        self.assertEqual(optin["coverage"]["midi_previews_exported"], 2)
        self.assertTrue(any(preview.joinpath("midi").glob("*.mid")))
        validate_directory(self.source, preview)

    def test_output_cache_containment_symlink_and_conflict_guards(self):
        with self.assertRaises(ValueError):
            export_directory(self.source, self.root / "outside")
        self.cache.mkdir()
        escape = self.root / "outside-dir"
        escape.mkdir()
        (self.cache / "escape").symlink_to(escape, target_is_directory=True)
        with self.assertRaises(ValueError):
            export_directory(self.source, self.cache / "escape" / "music")
        path = self.cache / "music" / "bytes.bin"
        manifest.write_bytes(path, b"fixture")
        manifest.write_bytes(path, b"fixture")
        with self.assertRaises(ValueError):
            manifest.write_bytes(path, b"changed")
        self.assertEqual(path.read_bytes(), b"fixture")

    def test_event_with_unsupported_meta_only_emits_canonical_json(self):
        raw = package(bytes.fromhex("00ff510301020300ff2f00"))
        self.source_file.write_bytes(raw)
        output = self.cache / "music" / "unsupported"
        result = export_directory(self.source, output, midi_preview_assuming_u24_is_mpqn=True)
        self.assertEqual(result["coverage"]["unsupported_sequence_entries"], 1)
        self.assertEqual(result["coverage"]["midi_previews_exported"], 1)  # second physical entry is supported
        json_path = next((output / "packages").glob("*.json"))
        exported = json.loads(json_path.read_text())
        unsupported = next(e for e in exported["entries"] if e["identity"]["event_status"] == "unsupported")
        self.assertEqual(unsupported["unsupported_reason"], "unsupported meta event 51")
        self.assertEqual(reconstruct_seq(exported["package"]), raw)
        validate_directory(self.source, output)


if __name__ == "__main__":
    unittest.main()
