"""Trust-boundary tests for the finite school-day background source profiles."""
import copy
import hashlib
import shutil
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.assets import bs_cohort_profile as profile
from tools.assets.media_mdec_export import (
    _cohort_context_for, _native_cd_read_extent, export_mdec_source,
)


REPOSITORY = Path(__import__("os").environ.get("TOKIMEMO_REPOSITORY_ROOT", Path.cwd()))
TRACK = REPOSITORY / "disc/track1.bin"


def inventory_rows():
    return inventory_rows_for(profile.ASSETS)


def inventory_rows_for(assets):
    rows = []
    for item in assets:
        rows.append({
            "path": item["path"], "kind": "file", "size": item["size"],
            "sha256": item["sha256"],
            "location": {"lba": item["lba"], "extent_bytes": item["size"],
                         "record_lba": item["record_lba"],
                         "record_offset": item["record_offset"],
                         "sectors": [{} for _ in range(item["file_sectors"]) ]},
        })
    return rows


class ProfileConstantsTests(unittest.TestCase):
    def test_fixed_selector_table_and_cohort_rows_agree(self):
        checked = profile._validate_asset_inventory(inventory_rows())
        self.assertEqual([row[0]["path"] for row in checked],
                         [f"CDROM/BS/C_01_{number:02d}.BS" for number in range(27, 31)])
        self.assertEqual([row[0]["girl_index"] for row in checked], [0, 1, 2, 3])
        self.assertEqual([row[0]["lba"] for row in checked], list(profile.TABLE_WORDS))
        self.assertEqual(profile.EXE_TABLE_OFFSET, 0x75F60)
        self.assertEqual(profile.EXE_TABLE_RAW.hex(), "033f00000d3f0000173f0000203f0000")

    def test_seasonal_selector_table_and_cohort_rows_agree(self):
        checked = profile._validate_asset_inventory(
            inventory_rows_for(profile.SEASONAL_ASSETS), profile.SEASONAL_ASSETS,
            profile.SEASONAL_TABLE_WORDS, "season_index")
        self.assertEqual([row[0]["path"] for row in checked], [
            "CDROM/BS/C_02_02H.BS", "CDROM/BS/C_02_02N.BS",
            "CDROM/BS/C_02_02A.BS", "CDROM/BS/C_02_02F.BS"])
        self.assertEqual([row[0]["season_index"] for row in checked], [0, 1, 2, 3])
        self.assertEqual([row[0]["lba"] for row in checked], list(profile.SEASONAL_TABLE_WORDS))
        self.assertEqual(profile.SEASONAL_TABLE_OFFSET, 0x75150)
        self.assertEqual(profile.SEASONAL_TABLE_RAW.hex(), "e93f0000f23f0000d73f0000e03f0000")

    def test_seasonal_path_or_selector_substitution_fails_closed(self):
        rows = inventory_rows_for(profile.SEASONAL_ASSETS)
        rows[-1]["path"] = "CDROM/BS/C_02_02X.BS"
        with self.assertRaisesRegex(ValueError, "C_02_02F"):
            profile._validate_asset_inventory(rows, profile.SEASONAL_ASSETS,
                                              profile.SEASONAL_TABLE_WORDS, "season_index")
        altered = list(profile.SEASONAL_ASSETS)
        altered[-1] = dict(altered[-1], season_index=2)
        with self.assertRaisesRegex(ValueError, "selector indices"):
            profile._validate_asset_inventory(inventory_rows_for(profile.SEASONAL_ASSETS),
                                              altered, profile.SEASONAL_TABLE_WORDS, "season_index")

    def test_executable_seasonal_table_is_pinned_at_its_own_offset(self):
        exe_path = REPOSITORY / "disc/files/SLPM_86.053"
        if not exe_path.is_file():
            self.skipTest("original executable is unavailable")
        data = exe_path.read_bytes()
        self.assertEqual(profile._validate_executable_bytes(
            data, profile.SEASONAL_TABLE_OFFSET, profile.SEASONAL_TABLE_RAW,
            profile.SEASONAL_TABLE_WORDS), profile.SEASONAL_TABLE_RAW)
        fake = bytearray(data)
        fake[profile.SEASONAL_TABLE_OFFSET:profile.SEASONAL_TABLE_OFFSET + 16] = bytes(16)
        with self.assertRaises(ValueError):
            profile._validate_executable_bytes(bytes(fake), profile.SEASONAL_TABLE_OFFSET,
                                               profile.SEASONAL_TABLE_RAW,
                                               profile.SEASONAL_TABLE_WORDS)

    def test_substituting_c_01_31_for_the_fourth_row_fails(self):
        rows = inventory_rows()
        rows[-1]["path"] = "CDROM/BS/C_01_31.BS"
        rows[-1]["location"]["lba"] = 0x3F28
        with self.assertRaisesRegex(ValueError, "C_01_30"):
            profile._validate_asset_inventory(rows)

    def test_reordered_or_repointed_profile_rows_fail_closed(self):
        rows = inventory_rows()
        altered = list(profile.ASSETS)
        altered[-1] = dict(altered[-1], girl_index=2)
        with mock.patch.object(profile, "ASSETS", tuple(altered)):
            with self.assertRaisesRegex(ValueError, "selector indices"):
                profile._validate_asset_inventory(rows)

        altered = list(profile.ASSETS)
        altered[0] = dict(altered[0], lba=0x3F0D)
        with mock.patch.object(profile, "ASSETS", tuple(altered)):
            with self.assertRaisesRegex(ValueError, "table order"):
                profile._validate_asset_inventory(rows)

    def test_actual_executable_table_is_pinned_at_fixed_offset(self):
        exe_path = REPOSITORY / "disc/files/SLPM_86.053"
        if not exe_path.is_file():
            self.skipTest("original executable is unavailable")
        data = exe_path.read_bytes()
        self.assertEqual(profile._validate_executable_bytes(data), profile.EXE_TABLE_RAW)
        for offset in (0x75F5C, 0x75F64):
            with self.subTest(offset=hex(offset)):
                fake = bytearray(data)
                fake[offset:offset + 16] = profile.EXE_TABLE_RAW
                with self.assertRaises(ValueError):
                    profile._validate_executable_bytes(bytes(fake))

    def test_external_self_hashed_or_replaced_report_is_not_authority(self):
        fake_source = type("Source", (), {})()
        fake_source.path = str(REPOSITORY / "disc/track1.bin")
        for payload in ({"schema": "tokimemo.bs-caller-cohort.v1",
                             "status": "source_backed_geometry_proved_for_bounded_cohort",
                             "source_witness": {"evidence_files": {}},
                             "geometry": {"active_pixels": {"width": 320, "height": 88}},
                             "assets": copy.deepcopy(list(profile.ASSETS))},):
            with self.subTest(payload_keys=sorted(payload)):
                with tempfile.TemporaryDirectory() as temporary:
                    with self.assertRaisesRegex(ValueError, "ignored BS cohort reports"):
                        export_mdec_source(fake_source, Path(temporary) / "out", payload)
        with self.assertRaisesRegex(ValueError, "expected tokimemo.background-context"):
            export_mdec_source(fake_source, Path("out"), {})


class SourceEvidenceTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.root = Path(self.temporary.name)
        for relative in profile.SOURCE_FILES:
            source = REPOSITORY / relative
            destination = self.root / relative
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(source, destination)

    def tearDown(self):
        self.temporary.cleanup()

    def test_complete_current_source_evidence_passes_hash_and_anchor_pins(self):
        checked = profile._check_source_evidence(self.root)
        self.assertEqual(checked, profile.SOURCE_FILES)

    def test_deleted_source_evidence_is_rejected(self):
        (self.root / "src/main/memory_card.c").unlink()
        with self.assertRaisesRegex(FileNotFoundError, "memory_card.c"):
            profile._check_source_evidence(self.root)

    def test_rehashed_source_evidence_without_trusted_pin_is_rejected(self):
        path = self.root / "src/main/school_day.c"
        path.write_bytes(path.read_bytes().replace(b"get_weekly_bg_sector", b"fake_weekly_bg_sector"))
        with self.assertRaisesRegex(ValueError, "source evidence hash differs"):
            profile._check_source_evidence(self.root)

    def test_season_index_or_season_table_source_substitution_is_rejected(self):
        for relative, marker in (("src/main/hud_panels.c", b"hud_season_index"),
                                 ("asm/data/data_800B5950.data.s", b"D_800B5950")):
            with self.subTest(relative=relative):
                for source_relative in profile.SOURCE_FILES:
                    source = REPOSITORY / source_relative
                    destination = self.root / source_relative
                    destination.parent.mkdir(parents=True, exist_ok=True)
                    shutil.copyfile(source, destination)
                path = self.root / relative
                content = path.read_bytes()
                path.write_bytes(content.replace(marker, marker + b"_substituted", 1))
                with self.assertRaisesRegex(ValueError, "source evidence hash differs"):
                    profile._check_source_evidence(self.root)
                path.write_bytes(content)

    def test_consistently_rehashed_report_cannot_repin_source_evidence(self):
        relative = "src/main/school_day.c"
        path = self.root / relative
        altered = path.read_bytes() + b"\n/* fabricated profile evidence */\n"
        path.write_bytes(altered)
        fake_hash = hashlib.sha256(altered).hexdigest()
        fake_report = {"schema": "tokimemo.bs-caller-cohort.v1",
                       "source_witness": {"evidence_files": {relative: {"sha256": fake_hash}}},
                       "assets": [], "geometry": {"active_pixels": {"width": 320, "height": 88}}}
        self.assertEqual(fake_report["source_witness"]["evidence_files"][relative]["sha256"], fake_hash)
        with self.assertRaisesRegex(ValueError, "source evidence hash differs"):
            profile._check_source_evidence(self.root)


class NativeReadExtentTests(unittest.TestCase):
    class RawSource:
        def __init__(self, *, missing_lba=None, invalid_lba=None):
            self.missing_lba = missing_lba
            self.invalid_lba = invalid_lba

        def iter_raw_sectors(self, entry, expected_input_sha256):
            self.expected_input_sha256 = expected_input_sha256
            first = entry["location"]["lba"]
            count = entry["size"] // 2048
            for index in range(count):
                lba = first + index
                if lba == self.missing_lba:
                    raise EOFError("sector missing")
                payload = bytes([lba & 0xFF]) * 2048
                raw = (b"\x00" + b"\xff" * 10 + b"\x00" + b"\x00\x00\x00\x02"
                       + b"\x01\x00\x00\x00" * 2 + payload + b"\x00" * 280)
                yield {"lba": lba, "raw": raw, "payload": payload,
                       "raw_sha256": hashlib.sha256(raw).hexdigest(),
                       "payload_sha256": hashlib.sha256(payload).hexdigest(),
                       "payload_size": 2048, "form": 1,
                       "form_name": "Form1", "subheader_valid": lba != self.invalid_lba}

    def setUp(self):
        self.entry = {"path": "CDROM/BS/C_01_30.BS", "kind": "file", "size": 16224,
                      "location": {"lba": 0x3F20}}

    def test_ten_sector_native_extent_records_but_excludes_overread_payloads(self):
        source = self.RawSource()
        extent = _native_cd_read_extent(source, self.entry, profile.TRACK_SHA256, 10)
        self.assertEqual(extent["overread_lbas"], [0x3F28, 0x3F29])
        self.assertEqual(extent["sectors_after_iso_extent"], 2)
        self.assertFalse(extent["overread_payload_used_for_mdec"])
        self.assertEqual(len(extent["sectors"]), 10)

    def test_incomplete_or_invalid_native_extent_fails_closed(self):
        from tools.assets.media_mdec_export import _native_cd_read_extent

        with self.assertRaisesRegex(EOFError, "missing"):
            _native_cd_read_extent(self.RawSource(missing_lba=0x3F29), self.entry,
                                   profile.TRACK_SHA256, 10)
        with self.assertRaisesRegex(ValueError, "non-Form1"):
            _native_cd_read_extent(self.RawSource(invalid_lba=0x3F20), self.entry,
                                   profile.TRACK_SHA256, 10)

@unittest.skipUnless(TRACK.is_file(), "original disc track is unavailable")
class OriginalInputTests(unittest.TestCase):
    def test_original_track_and_current_source_build_the_finite_profile(self):
        from tools.assets.input import open_input

        with open_input(TRACK) as source:
            entries = source.entries()
            context = profile.build_profile_context(source, entries, REPOSITORY)
        self.assertEqual(context["schema"], profile.PROFILE_SCHEMA)
        self.assertEqual([item["path"] for item in context["assets"]],
                         [item["path"] for item in profile.ASSETS])
        self.assertEqual(context["geometry"]["active_pixels"], {"width": 320, "height": 88})
        self.assertEqual(context["geometry"]["padded_pixels"], {"width": 320, "height": 96})
        geometry = context["geometry"]
        self.assertEqual(geometry["caller_mode"], 0)
        self.assertEqual(geometry["active_decode_rect_height"], 0x58)
        self.assertEqual(geometry["vram_rectangles_by_dec_bg_decode_to_vram_return"], [
            {"return_value": 0, "rect_xywh": [0x2C0, 0xA0, 0x140, 0x58]},
            {"return_value": 1, "rect_xywh": [0x140, 0x1A0, 0x140, 0x58]},
        ])
        self.assertEqual(geometry["display_sprite_pieces_relative_to_draw_origin"], [
            {"origin_offset_xy": [-0xA0, -0x30], "size_wh": [0xC0, 0x58]},
            {"origin_offset_xy": [0x20, -0x30], "size_wh": [0x80, 0x58]},
        ])
        self.assertFalse(geometry["runtime_draw_origin_values_known"])
        bound = _cohort_context_for(
            next(entry for entry in entries if entry["path"] == profile.ASSETS[0]["path"]),
            context["assets"][0], geometry)
        self.assertEqual(bound["caller_geometry_profile"], geometry)

    def test_original_inventory_rejects_row_three_replacement(self):
        from tools.assets.input import open_input

        with open_input(TRACK) as source:
            rows = source.entries()
        entry = next(row for row in rows if row["path"] == "CDROM/BS/C_01_30.BS")
        replacement = dict(entry, path="CDROM/BS/C_01_31.BS", sha256="0" * 64,
                           size=1, location=dict(entry["location"], lba=0x3F28,
                                                 extent_bytes=1))
        rows.remove(entry)
        rows.append(replacement)
        with self.assertRaisesRegex(ValueError, "C_01_30"):
            profile._validate_asset_inventory(rows)

    def test_original_track_and_current_source_build_seasonal_profile(self):
        from tools.assets.input import open_input

        with open_input(TRACK) as source:
            entries = source.entries()
            context = profile.build_profile_context(source, entries, REPOSITORY, "seasonal")
        self.assertEqual(context["source_profile"]["profile_kind"], "seasonal")
        self.assertEqual([item["path"] for item in context["assets"]],
                         [item["path"] for item in profile.SEASONAL_ASSETS])
        table = context["source_profile"]["executable"]
        self.assertEqual(table["table_offset"], profile.SEASONAL_TABLE_OFFSET)
        self.assertEqual(table["table_words"], list(profile.SEASONAL_TABLE_WORDS))
        self.assertEqual(table["table_raw_hex"], "e93f0000f23f0000d73f0000e03f0000")
        for record, expected in zip(context["assets"], profile.SEASONAL_ASSETS):
            self.assertEqual(record["season_index"], expected["season_index"])
            self.assertEqual(record["fixed_reader_extent"]["sectors_after_iso_file_extent"], 1)
            self.assertFalse(record["fixed_reader_extent"]["post_file_sectors_claimed_consumed_by_decoder"])
        self.assertEqual(context["geometry"]["active_pixels"], {"width": 320, "height": 88})
        self.assertEqual(context["geometry"]["padded_pixels"], {"width": 320, "height": 96})


if __name__ == "__main__":
    unittest.main()
