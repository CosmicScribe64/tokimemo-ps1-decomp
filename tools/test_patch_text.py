"""Tests for the English-patch text-container writers (T-39503).

Synthetic fixtures need no game data. The disc-backed tests run when ``disc/files`` and
``.asset-cache/text/units/units.jsonl`` exist (``tools.assets.text_export --include-source``
then ``tools.assets.text_units``) and skip otherwise.

  tools/docker.sh python3 -m unittest tools.test_patch_text -v
"""
import json
import struct
import unittest
from pathlib import Path

from tools.assets.text import parse_script, tokenize_text
from tools.patch.text import c_literals, coverage
from tools.patch.text.containers import _script_container, build_containers, identity_check, load_units
from tools.patch.text.image import Image, ImageError, Pointer, Slot, has_cd_trailer, write_cd_trailer
from tools.patch.text.segments import (Control, ProvisionalAsciiEncoder, SegmentError, ShiftJisEncoder, Text,
                                       encode, source_segments, target_segments)
from tools.patch.text.sizes import english_payloads
from tools.patch.text.translations import load_jsonl, load_targets, target_of

REPOSITORY = Path(__file__).resolve().parents[1]
DISC = REPOSITORY / "disc" / "files"
UNITS = REPOSITORY / ".asset-cache" / "text" / "units" / "units.jsonl"
RETURNED = REPOSITORY / "translations" / "en" / "returned-v3.jsonl"
SUPPLEMENTAL = REPOSITORY / "translations" / "en" / "supplemental.jsonl"
HAVE_DATA = DISC.is_dir() and UNITS.is_file()

BASE = 0x80100000
PATH = "CDROM/SERIFU/SYNTH.BIN"
# Invented text: kana glyph pairs and the talk controls (0x20 row, G girl name).
LINE0 = "あい".encode("shift_jis")
LINE1 = "う".encode("shift_jis") + b" G" + "え".encode("shift_jis")


def synthetic_script():
    """A one-group talk file: two strings, text/command/voice lists, root tables, CD trailer."""
    data = bytearray(2048)
    data[0:len(LINE0)] = LINE0                     # string 0 at 0x00, NUL, pad to 0x08
    data[8:8 + len(LINE1)] = LINE1                 # string 1 at 0x08 (7 bytes + NUL)
    struct.pack_into("<II", data, 0x10, BASE + 0x00, BASE + 0x08)   # group text list
    data[0x18:0x1B] = bytes((9, 9, 0xFF))          # command list: two effect lines, end
    struct.pack_into("<I", data, 0x1C, BASE + 0x10)  # root text table
    struct.pack_into("<I", data, 0x20, BASE + 0x18)  # root command table
    struct.pack_into("<I", data, 0x24, BASE + 0x18)  # root voice table (unused by command 9)
    write_cd_trailer(data)
    profile = {"id": "synthetic", "path": PATH, "load_address": BASE,
               "sections": [{"id": "s", "text_table": BASE + 0x1C, "command_table": BASE + 0x20,
                             "voice_table": BASE + 0x24, "group_count": 1}]}
    return bytes(data), profile


def units_for(data, profile):
    units = []
    for record in parse_script(data, PATH, profile)["records"]:
        text = record.get("text")
        if text is None:
            continue
        raw = bytes.fromhex(text["raw_hex"])
        units.append({"id": record["id"], "kind": "script_record", "source_path": PATH,
                      "source_offset": text["offset"], "raw_hex": text["raw_hex"],
                      "source_text": raw.decode("shift_jis"), "tokens": text["tokens"],
                      "context": {}, "parent_id": None})
    return units


class SegmentTests(unittest.TestCase):
    def test_controls_kept_and_sjis_rebuilds(self):
        unit = {"kind": "script_record", "raw_hex": LINE1.hex(), "source_text": LINE1.decode("shift_jis"),
                "tokens": tokenize_text(LINE1, 9)["tokens"]}
        segments = source_segments(unit)
        self.assertEqual([type(s) for s in segments], [Text, Control, Control, Text])
        self.assertEqual(encode(segments, ShiftJisEncoder()), LINE1)

    def test_target_markers_must_match(self):
        refs, kinds = {"[[T001:row]]": b" "}, {"[[T001:row]]": "separator"}
        segments = target_segments("Hi[[T001:row]]there", refs, kinds)
        self.assertEqual(encode(segments, ProvisionalAsciiEncoder()), b"Hi there")
        with self.assertRaises(SegmentError):
            target_segments("no marker", refs, kinds)
        with self.assertRaises(SegmentError):
            target_segments("[[T001:row]][[T001:row]]", refs, kinds)

    def test_supplemental_markers_take_source_bytes_in_order(self):
        source = [Text("a"), Control("separator", "row", b" "), Text("b"), Control("separator", "row", b"q")]
        row = {"translation": "x[[T002:choice_option]]y[[T001:row]]",
               "protected_tokens": json.dumps([{"marker": "[[T001:row]]"}, {"marker": "[[T002:choice_option]]"}])}
        segments, problem = target_of(row, source)
        self.assertIsNone(problem)
        self.assertEqual(encode(segments, ShiftJisEncoder()), b"xqy ")

    def test_wrapped_japanese_is_not_written_back(self):
        wrapped = "かな".encode("shift_jis")
        row = {"target_text": "Kana[[T001:unknown_renderer_bytes]]",
               "protected_tokens": [{"marker": "[[T001:unknown_renderer_bytes]]", "kind": "unknown_renderer_bytes",
                                     "offset": 0, "raw_hex": wrapped.hex()}]}
        segments, problem = target_of(row, [])
        self.assertEqual((encode(segments, ShiftJisEncoder()), problem), (b"Kana", "wrapped_source_text_dropped"))

    def test_provisional_encoder_folds_and_counts(self):
        encoder = ProvisionalAsciiEncoder()
        self.assertEqual(encoder.encode_text("It’s… ☃"), b"It's... ?")
        self.assertEqual(encoder.unmapped, {"☃": 1})


class ImageTests(unittest.TestCase):
    def test_trailer(self):
        data = bytearray(4096)
        data[5] = 7
        write_cd_trailer(data)
        self.assertTrue(has_cd_trailer(bytes(data)))
        self.assertEqual(data[-4:], b"\x00\x07\x12\x10")

    def test_script_identity_and_relocation(self):
        data, profile = synthetic_script()
        container = _script_container(PATH, data, profile, units_for(data, profile))
        self.assertEqual(identity_check(container)["status"], "identical")
        first, second = sorted(container.image.slots)
        long_line = b"A much longer English line that no longer fits in place"
        result = container.image.write({first: long_line})
        self.assertTrue(has_cd_trailer(result.data))
        self.assertEqual(len(result.data), len(data))
        moved = parse_script(result.data, PATH, profile)["records"]
        self.assertEqual(bytes.fromhex(moved[0]["text"]["raw_hex"]), long_line)
        self.assertEqual(bytes.fromhex(moved[1]["text"]["raw_hex"]), LINE1)
        self.assertEqual(result.data[0x18:0x28], data[0x18:0x28])  # commands and roots untouched

    def test_growth_adds_sectors(self):
        data, profile = synthetic_script()
        container = _script_container(PATH, data, profile, units_for(data, profile))
        first = sorted(container.image.slots)[0]
        result = container.image.write({first: b"x" * 5000})
        self.assertEqual(len(result.data), 6144)
        self.assertTrue(has_cd_trailer(result.data))
        self.assertEqual(bytes.fromhex(parse_script(result.data, PATH, profile)["records"][0]["text"]["raw_hex"]),
                         b"x" * 5000)

    def test_in_place_capacity(self):
        data = bytearray(16)
        data[0:3] = b"abc"
        data[8:10] = b"zz"
        slot = Slot("s", 0, b"abc", "in_place", capacity=8)
        image = Image("t", bytes(data), 0, [slot], [], growable=False)
        self.assertEqual(image.write({}).data, bytes(data))
        self.assertEqual(image.write({"s": b"abcdefg"}).data[:8], b"abcdefg\0")
        result = image.write({"s": b"abcdefgh"})
        self.assertEqual(result.overflows[0]["reason"], "in_place_capacity")
        self.assertEqual(result.data, bytes(data))
        with self.assertRaises(ImageError):
            Image("t", bytes(data), 0, [Slot("s", 0, b"abc", "in_place", capacity=10)], [])

    def test_pointer_must_reach_slot(self):
        data = bytearray(16)
        data[0:2] = b"ab"
        struct.pack_into("<I", data, 8, 0x80000001)
        with self.assertRaises(ImageError):
            Image("t", bytes(data), 0x80000000, [Slot("s", 0, b"ab", "relocatable")], [Pointer(8, "s")])


class LiteralTests(unittest.TestCase):
    def test_render_roundtrip(self):
        samples = [bytes(range(1, 256)), "テスト".encode("shift_jis") + b"%d\n\t\"\\??=",
                   b"\x81\x5c\x83\x5c", b"\x82"]
        for sample in samples:
            self.assertEqual(c_literals.string_bytes(c_literals.render_literal(sample)), sample)


class HeuristicTests(unittest.TestCase):
    def test_runs_need_kana_and_terminator(self):
        text = "あいうえお".encode("shift_jis")
        self.assertEqual(coverage.sjis_runs(b"\xff" + text + b"\0"), [(1, len(text))])
        self.assertEqual(coverage.sjis_runs(text), [])
        self.assertEqual(coverage.sjis_runs(b"\x83\x83" * 6 + b"\0"), [])


@unittest.skipUnless(HAVE_DATA, "needs disc/files and the units export")
class DiscTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.units = load_units(UNITS)
        cls.supplemental = load_jsonl(SUPPLEMENTAL)
        cls.containers = build_containers(DISC, cls.units, cls.supplemental)

    def test_every_container_rebuilds_byte_exact(self):
        rows = [identity_check(c) for c in self.containers.values() if c.image is not None]
        self.assertEqual([r["path"] for r in rows if r["status"] != "identical"], [])
        self.assertGreaterEqual(len(rows), 140)

    def test_every_c_literal_renders_back(self):
        sites = c_literals.locate_sites(REPOSITORY, self.units)
        self.assertEqual(c_literals.identity_check(REPOSITORY, sites)["status"], "identical")

    def test_every_record_maps_once(self):
        mapping = coverage.coverage_map(self.containers)
        ids = [r["id"] for r in load_jsonl(RETURNED)] + [r["id"] for r in self.supplemental]
        check = coverage.check_ids(mapping, ids)
        self.assertEqual((check["unmapped"], check["duplicate_ids"], check["map_ids_without_translation"]),
                         ([], [], []))

    def test_magazine_tables_have_records(self):
        rows = coverage.magazine_tables(self.containers)
        self.assertEqual(len(rows), 16 + 13 + 13 + 39)
        self.assertEqual([r for r in rows if not r["records"]], [])

    def test_english_talk_files_follow_their_pointers(self):
        targets = load_targets(RETURNED, SUPPLEMENTAL)
        payloads, _ = english_payloads(self.containers, targets, ProvisionalAsciiEncoder())
        checked = 0
        for path, container in self.containers.items():
            if not container.kind.startswith("talk_script"):
                continue
            new = payloads.get(path, {})
            result = container.image.write(new)
            self.assertEqual(result.overflows, [], path)
            self.assertEqual(has_cd_trailer(result.data), has_cd_trailer(container.image.data), path)
            self.assertEqual(result.data[-2:] == b"\x12\x10", container.image.data[-2:] == b"\x12\x10", path)
            for pointer in container.image.pointers:
                word = struct.unpack_from("<I", result.data, pointer.offset)[0] - container.image.base
                end = result.data.index(b"\0", word)
                expected = new.get(pointer.slot, container.image.slots[pointer.slot].payload)
                self.assertEqual(result.data[word:end], expected, pointer.slot)
                checked += 1
        self.assertGreater(checked, 30000)


if __name__ == "__main__":
    unittest.main()
