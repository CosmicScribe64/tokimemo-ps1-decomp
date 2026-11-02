"""Tests for the English disc build (T-39507): encoder, word wrap, windows, CD layout, in-place
writes and the build report.

Synthetic fixtures need no game data. The end-to-end test runs the whole build
(tools/patch/build_english.py) when TOKIMEMO_ENGLISH_DISC=1 is set and the original track is in
disc/orig/; it takes several minutes and needs the image of tools/Dockerfile.patch.

  tools/docker.sh python3 -m unittest tools.test_patch_english -v
  IMG=tokimemo-patch tools/docker.sh env TOKIMEMO_ENGLISH_DISC=1 python3 -m unittest tools.test_patch_english -v
"""
import json
import os
import struct
import tempfile
import unittest
from pathlib import Path

from tools.patch import build_english, cd_layout
from tools.patch.text.containers import Container, Record
from tools.patch.text.english import EMPTY_OPTION, LINE_END, NAME_CODES, OPTION_END, EnglishEncoder
from tools.patch.text.image import Image, Slot, has_cd_trailer, write_cd_trailer
from tools.patch.text.inplace import patch_built
from tools.patch.text.layout import english_payloads
from tools.patch.text.segments import Control, Text, encode
from tools.patch.text.windows import NAME_PX, WINDOWS, Window, window_for
from tools.patch.text.wrap import FontMetrics, label_end, wrap

REPOSITORY = Path(__file__).resolve().parents[1]
ORIG_TRACK = REPOSITORY / "disc" / "orig" / "track1.bin"

# Synthetic font: every glyph 6 px, space 4 px, Shift-JIS 14 px.
METRICS = FontMetrics({**{c: 6 for c in range(0x21, 0x7F)}, 0x20: 4, **{c: 8 for c in range(0xA0, 0xAA)}}, 14, 16)


def window(width, *, lines=3, scroll=True, wrap_rows=True, max_rows=None):
    return Window("test", 0, width, lines, scroll, wrap_rows, None, "test", max_rows)


class EncoderTest(unittest.TestCase):
    def setUp(self):
        self.encoder = EnglishEncoder()

    def test_ascii_is_itself(self):
        self.assertEqual(self.encoder.encode_text("Hi, q n P!"), b"Hi, q n P!")

    def test_extra_glyphs(self):
        self.assertEqual(self.encoder.encode_text("“A” ‘b’ — … ♥ ♪ é ★"),
                         bytes([0xA0, 0x41, 0xA1, 0x20, 0xA2, 0x62, 0xA3, 0x20, 0xA4, 0x20, 0xA5, 0x20,
                                0xA6, 0x20, 0xA7, 0x20, 0xA8, 0x20, 0xA9]))

    def test_typographic_variants_are_normalised(self):
        self.assertEqual(self.encoder.encode_text("a–b ♡ ♫ ☆ ！ è 　x"),
                         b"a-b \xa6 \xa7 \xa9 ! e  x")
        self.assertIn("–", self.encoder.folded)

    def test_shift_jis_kept_and_unmapped_reported(self):
        self.assertEqual(self.encoder.encode_text("「\U0001f600"), "「".encode("shift_jis") + b"?")
        notes = self.encoder.take_record_notes()
        self.assertEqual(notes, {"unmapped": {"\U0001f600": 1}, "kept_shift_jis": {"「": 1}})
        self.assertEqual(self.encoder.take_record_notes(), {})

    def test_controls(self):
        enc = self.encoder.encode_control
        self.assertEqual(enc(Control("separator", "row", b" ")), bytes([LINE_END]))
        self.assertEqual(enc(Control("separator", "choice_option", b"q")), bytes([OPTION_END]))
        self.assertEqual(enc(Control("separator", "choice_option", b" ")), bytes([LINE_END]))
        self.assertEqual(enc(Control("separator", "reply_variant", b"q")), bytes([OPTION_END]))
        self.assertEqual(enc(Control("row_separator", "row_separator", b" ")), bytes([LINE_END]))
        for index, letter in enumerate("PpmFYGSOWAM"):
            self.assertEqual(enc(Control("substitution", "x", letter.encode())), bytes([0x10 + index]))
        self.assertEqual(enc(Control("conditional_substitution", "x", b"G")), bytes([NAME_CODES["G"]]))
        self.assertEqual(enc(Control("conditional_choice_zero", "x", b"nn")), bytes([EMPTY_OPTION] * 2))
        self.assertEqual(enc(Control("reply_skip_marker", "x", b"n\x41")), bytes([EMPTY_OPTION, 0x41]))
        self.assertEqual(enc(Control("terminal_byte_before_nul", "x", b" ")), b"")
        for kept in (Control("format_directive", "f", b"%d"), Control("inline_label", "l", b">"),
                     Control("source_storage_control", "line_feed", b"\n"), Control("opaque", "o", b"\x82\xa0")):
            self.assertEqual(enc(kept), kept.raw)

    def test_segments_end_to_end(self):
        segments = [Text("Hello "), Control("substitution", "girl_name", b"G"), Text("."),
                    Control("separator", "row", b" "), Text("Bye")]
        self.assertEqual(encode(segments, self.encoder), b"Hello \x15.\nBye")


class WrapTest(unittest.TestCase):
    def test_fitting_rows_and_explicit_breaks_stay(self):
        result = wrap(b"ab cd\nef", window(100), METRICS)
        self.assertEqual(result.payload, b"ab cd\nef")
        self.assertEqual((result.rows, result.breaks_added, result.overflow_rows), (2, 0, []))

    def test_long_row_breaks_at_the_last_fitting_space(self):
        # "aaaa bbbb cccc": 24 + 4 + 24 + 4 + 24 px; 60 px holds two words
        result = wrap(b"aaaa bbbb cccc", window(60), METRICS)
        self.assertEqual(result.payload, b"aaaa bbbb\ncccc")
        self.assertEqual((result.rows, result.breaks_added), (2, 1))

    def test_word_wider_than_the_window_is_reported(self):
        result = wrap(b"aaaaaaaaaaaa bb", window(30), METRICS)
        self.assertEqual(result.payload, b"aaaaaaaaaaaa\nbb")
        self.assertEqual(result.overflow_rows, [72])

    def test_choice_windows_do_not_wrap(self):
        result = wrap(b"aaaa bbbb\x0bcc\x0bdd\x0bee", window(30, scroll=False, wrap_rows=False), METRICS)
        self.assertEqual(result.payload, b"aaaa bbbb\x0bcc\x0bdd\x0bee")
        self.assertEqual(result.overflow_rows, [52])
        self.assertTrue(result.too_many_lines)

    def test_lines_count_per_option_block_in_message_windows(self):
        result = wrap(b"a\nb\nc\nd\x0be", window(50, scroll=False), METRICS)
        self.assertEqual(result.rows, 4)
        self.assertTrue(result.too_many_lines)
        self.assertFalse(wrap(b"a\nb\nc\nd", window(50), METRICS).too_many_lines)
        self.assertTrue(wrap(b"a\nb\nc\nd", window(50, max_rows=3), METRICS).too_many_lines)

    def test_widths_of_names_shift_jis_and_labels(self):
        self.assertEqual(METRICS.width(bytes([NAME_CODES["G"]])), NAME_PX)
        self.assertEqual(METRICS.width(bytes([NAME_CODES["Y"]])), 6 * 6)
        self.assertEqual(METRICS.width("あ".encode("shift_jis") + b"a"), 14 + 6)
        self.assertEqual(METRICS.width(b"\x0a\x0b\x0c"), 0)
        self.assertEqual(label_end(b">Teacher: Hi\nthere"), len(b">Teacher: "))
        self.assertEqual(label_end(b">Hi"), 1)
        self.assertEqual(label_end(b"Hi: there"), 0)
        # the label does not count against the row
        self.assertEqual(wrap(b">Teacher: aaaa", window(30), METRICS).payload, b">Teacher: aaaa")


class WindowTest(unittest.TestCase):
    def record(self, kind, segments, command=None):
        unit = {"context": {"command": {"name": "c", "value": command}}} if command is not None else {}
        return Record("id", kind, "P", 0, b"", segments, unit=unit)

    def test_talk_commands(self):
        self.assertEqual(window_for("talk_script", self.record("script_record", [], 1))[0].name, "message_speech")
        self.assertEqual(window_for("talk_script", self.record("script_record", [], 9))[0].name, "message_narration")
        self.assertEqual(window_for("talk_script", self.record("script_record", [], 3))[0].name, "choice")
        labelled = [Control("inline_label", "inline_label", b">"), Text("A: b")]
        self.assertEqual(window_for("talk_script", self.record("script_record", labelled, 7))[0].name,
                         "message_speech")
        self.assertEqual(window_for("talk_script", self.record("script_record", [Text("x")], 7))[0].name,
                         "message_narration")

    def test_renderer_and_unknown_contexts(self):
        rows = [Text("a"), Control("row_separator", "row_separator", b" "), Text("b")]
        self.assertEqual(window_for("overlay", self.record("source_literal", rows))[0].name, "telephone_narration")
        self.assertIsNone(window_for("overlay", self.record("source_literal", [Text("a")]))[0])
        self.assertEqual(window_for("talk_script_supplemental", self.record("supplemental", []))[0].name,
                         "message_speech")

    def test_geometry(self):
        self.assertEqual(WINDOWS["message_speech"].width, 0x7C + 0x48)
        self.assertEqual(WINDOWS["message_narration"].width, 0x7C + 0x80)
        self.assertFalse(WINDOWS["choice"].wrap)


def in_place_container(data):
    slots = [Slot("a", 0x10, data[0x10:0x13], "in_place", capacity=8),
             Slot("b", 0x20, data[0x20:0x22], "in_place", capacity=4),
             Slot("c", 0x30, data[0x30:0x32], "in_place", capacity=4)]
    container = Container("CDROM/EXEDIR/T.EXN", "overlay", Image("T", data, 0x80132000, slots, [], growable=False))
    for slot, kind in zip(slots, ("dictionary_slot", "source_extent", "source_literal")):
        container.records[slot.id] = Record(slot.id, kind, "T", slot.offset, slot.payload, [])
    return container


class InPlaceTest(unittest.TestCase):
    def test_writes_moves_and_overflows(self):
        data = bytearray(2048)
        data[0x10:0x13], data[0x20:0x22], data[0x30:0x32] = b"abc", b"de", b"fg"
        write_cd_trailer(data)
        container = in_place_container(bytes(data))
        built = bytearray(data)
        built[0x20] = 0x55                                    # slot b moved in the English link
        write_cd_trailer(built)
        result = patch_built(container, {"a": b"ABCDEF", "b": b"XY", "c": b"ZZ"}, bytes(built))
        self.assertEqual(result.written, ["a"])
        self.assertEqual(result.moved_in_build, ["b"])
        self.assertEqual(result.data[0x10:0x18], b"ABCDEF\0\0")
        self.assertEqual(result.data[0x30:0x32], b"fg")       # C literals are compiled, not patched
        self.assertTrue(has_cd_trailer(result.data))
        over = patch_built(container, {"a": b"ABCDEFGHIJ"}, bytes(data))
        self.assertEqual([o["id"] for o in over.overflows], ["a"])


class CdLayoutTest(unittest.TestCase):
    def test_scan_read_sites(self):
        with tempfile.TemporaryDirectory() as tmp:
            src = Path(tmp) / "src" / "ovl"
            src.mkdir(parents=True)
            (src / "a.c").write_text("void f(void) {\n    cd_read_sectors(7U, (void *)0x801E8000, 0xB037);\n"
                                     "    cd_read_sectors_nowait(0x29, 0x801E0000, 1234);\n"
                                     "    cd_read_sectors(n, 0x801E0000, lba);\n"
                                     "    if (cd_file_checksum_ok(0x2D, 0x80180000)) {}\n}\n")
            sites = cd_layout.scan_read_sites(Path(tmp) / "src")
            self.assertEqual([(s["sectors"], s["address"], s["lba"], s["checked"]) for s in sites],
                             [(7, 0x801E8000, 0xB037, True), (0x29, 0x801E0000, 1234, False)])
            self.assertEqual(sites[0]["source"], "src/ovl/a.c:2")
            checks = cd_layout.scan_checksum_sites(Path(tmp) / "src")
            self.assertEqual([(c["blocks"], c["address"]) for c in checks], [(0x2D, 0x80180000)])

    def plan(self, new_size, sites, checks=(), size=10 * 2048):
        limits = cd_layout.buffer_limits(sites)
        files = {100: ("F", 10), 110: ("NEXT", 4)}
        return cd_layout.plan_file("F", 100, size, new_size, sites, limits, list(checks), files)

    def test_plans(self):
        site = {"source": "s", "sectors": 10, "address": 0x801E8000, "lba": 100}
        bigger = {"source": "t", "sectors": 14, "address": 0x801E8000, "lba": 200}
        same = self.plan(10 * 2048, [site])
        self.assertTrue(same.accept)
        self.assertEqual(same.new_sectors, 10)
        grown = self.plan(12 * 2048, [site, bigger])
        self.assertTrue(grown.accept)
        self.assertEqual((grown.new_sectors, grown.problems), (12, []))
        too_big = self.plan(15 * 2048, [site, bigger])
        self.assertFalse(too_big.accept)
        self.assertIn("buffer_limit", too_big.reason)
        date = self.plan(25 * 2048, [dict(site, address=0x80198000)], size=20 * 2048)
        self.assertFalse(date.accept)
        self.assertIn("limit 24", date.reason)
        inside = self.plan(12 * 2048, [site, dict(site, source="u", lba=104, sectors=2)])
        self.assertIn("from inside the file", inside.problems[0])
        spanning = self.plan(12 * 2048, [dict(site, sectors=14), bigger])
        self.assertIn("also reads NEXT", spanning.problems[0])
        checked = self.plan(12 * 2048, [site, bigger], [{"source": "k", "blocks": 10, "address": 0x801E8000}])
        self.assertIn("checks 10 blocks", checked.problems[0])
        unknown = self.plan(12 * 2048, [])
        self.assertTrue(unknown.accept)
        self.assertIn("no literal read site", unknown.warnings[0])

    def test_remap_table(self):
        placed = [{"path": "B", "lba": 200, "sectors": 3, "new_lba": 300000, "new_sectors": 4},
                  {"path": "A", "lba": 100, "sectors": 2, "new_lba": 100, "new_sectors": 2}]
        rows = cd_layout.remap_entries(placed)
        self.assertEqual([r["path"] for r in rows], ["B"])
        asm = cd_layout.remap_asm(rows)
        self.assertIn("english_cd_remap_count:\n    .word 1", asm)
        self.assertIn(".word 200, 300000  # B", asm)
        self.assertIn(".byte 3, 4, 0, 0", asm)
        self.assertIn(".size english_cd_remap, 12", asm)
        self.assertIn(".word 0", cd_layout.remap_asm([]))
        with self.assertRaises(ValueError):
            cd_layout.remap_asm([dict(rows[0], new_sectors=256)])

    def test_cli_without_layout_writes_an_empty_table(self):
        with tempfile.TemporaryDirectory() as tmp:
            out = Path(tmp) / "gen" / "remap.s"
            self.assertEqual(cd_layout.main(["x", "remap-asm", str(Path(tmp) / "none.json"), str(out)]), 0)
            self.assertIn("    .word 0", out.read_text())


class ReportTest(unittest.TestCase):
    def test_payloads_statuses_and_report(self):
        talk = Container("CDROM/SERIFU/T.BIN", "talk_script", None)
        unit = {"context": {"command": {"name": "girl", "value": 1}}}
        talk.records["t1"] = Record("t1", "script_record", talk.path, 0, b"", [Text("x")], unit=unit)
        talk.records["t2"] = Record("t2", "script_record", talk.path, 8, b"", [Text("x")], unit=unit)
        talk.records["t3"] = Record("t3", "script_record", talk.path, 16, b"", [Text("x")], unit=unit)
        talk.records["t4"] = Record("t4", "script_record", talk.path, 24, b"", [Text("x")], role="alias")
        targets = {"t1": {"target_text": "Hello there", "protected_tokens": []},
                   "t2": {"target_text": "W" * 40, "protected_tokens": []},
                   "t4": {"target_text": "alias", "protected_tokens": []}}
        text = english_payloads({talk.path: talk}, targets, METRICS)
        self.assertEqual(text.payloads[talk.path], {"t1": b"Hello there", "t2": b"W" * 40})
        self.assertEqual(text.notes["t1"]["status"], "placed")
        self.assertEqual(text.notes["t2"]["status"], "unwrappable")
        self.assertEqual(text.notes["t3"], {"path": talk.path, "kind": "script_record", "container": "talk_script",
                                            "status": "kept_original", "reason": "no_translation_row"})
        self.assertNotIn("t4", text.notes)
        literals = {"literals_reverted": {}, "files": [], "literals_written": 0, "files_reverted": {}}
        data = {"overflows": {}, "kept_original": [{"path": talk.path, "reason": "buffer_limit: x", "records": 2}],
                "placed": [], "unchanged": 0}
        records = build_english.record_report(text, {talk.path: talk}, literals, data, {"in_place": {}})
        self.assertEqual(records["totals"], {"file_kept_original": 2, "kept_original": 1})
        self.assertEqual(records["by_container"][talk.path]["file_kept_original"], 2)
        self.assertEqual([i["id"] for i in records["records"]["file_kept_original"]], ["t1", "t2"])
        report = {"records": records, "data_files": dict(data, placed=[
            {"path": "X", "lba": 1, "new_lba": 9, "bytes": 10, "new_bytes": 30}]),
            "c_literals": literals, "characters": {"unmapped": {}, "kept_shift_jis": {}},
            "disc": {"track1_sha1": "ab" * 20, "sectors_added": 3, "xdelta_bytes": 99},
            "timings": {"total": 1.5}}
        lines = build_english.headline(report)
        self.assertIn("data files: 1 written (+20 bytes), 1 moved, 1 kept original, 0 unchanged", lines)
        self.assertTrue(lines[0].startswith("records: file_kept_original 2, kept_original 1"))


class LiteralSiteTest(unittest.TestCase):
    def test_moved_literal_is_found_by_its_bytes(self):
        from tools.patch.text import c_literals
        with tempfile.TemporaryDirectory() as tmp:
            src = Path(tmp) / "src" / "main"
            src.mkdir(parents=True)
            text = 'void f(void) {\n#ifdef X\n    g();\n#endif\n    h("ab"); h("cd"); h("ab");\n}\n'
            (src / "a.c").write_text(text)
            first, second = text.index('"ab"'), text.index('"cd"')

            def unit(uid, raw, offset):
                return {"id": uid, "kind": "source_literal", "raw_hex": raw.hex(),
                        "context": {"source_locator_id": "c:src/main/a.c:1:1", "source_character_offset": offset}}
            # the export saw the file without the three #ifdef lines (28 characters)
            units = [unit("u1", b"ab", first - 28), unit("u2", b"cd", second)]
            sites = {s.id: s.start for s in c_literals.locate_sites(tmp, units)}
            self.assertEqual(sites, {"u1": first, "u2": second})
            with self.assertRaises(c_literals.LiteralError):
                c_literals.locate_sites(tmp, [unit("u3", b"zz", 5)])


class GrowArrayTest(unittest.TestCase):
    def test_sized_array_grows_for_a_longer_literal(self):
        from tools.patch.text.c_literals import _grow_array
        self.assertEqual(_grow_array("    u8 name[27] = ", 30), "    u8 name[30] = ")
        self.assertEqual(_grow_array("    u8 name[0x20] = ", 30), "    u8 name[0x20] = ")
        self.assertEqual(_grow_array("    f(", 30), "    f(")


class EnglishLinkFreeTest(unittest.TestCase):
    def test_free_link_has_anchors_and_no_pins(self):
        """T-39505: the English script names every input section and pins nothing, so the section
        labels after a shrinking or growing object simply follow it."""
        import tools.english_link as english_link
        with tempfile.TemporaryDirectory() as tmp:
            ld, out, mapfile = Path(tmp) / "a.ld", Path(tmp) / "b.ld", Path(tmp) / "a.map"
            ld.write_text("SECTIONS\n{\n    X_START = .;\n    .X 0x80132000 : AT(0)\n    {\n"
                          "        build/a.o(.rodata);\n        X_RODATA_END = .;\n        X_DATA_START = .;\n"
                          "        build/b.o(.data);\n    }\n    X_VRAM_END = .;\n}\n")
            mapfile.write_text("")
            english_link.cmd_ld(["X", "build/en", str(ld), str(mapfile), str(out), "--redirect", "build/a.o"])
            lines = [line.strip() for line in out.read_text().splitlines()]
            self.assertIn("build/en/a.o(.rodata);", lines)
            self.assertIn("__en_X_0 = ALIGN(., 2);", lines)
            self.assertIn("__en_X_1 = ALIGN(., 2);", lines)
            self.assertFalse(any(line.startswith(". = 0x") for line in lines))


@unittest.skipUnless(os.environ.get("TOKIMEMO_ENGLISH_DISC") == "1" and ORIG_TRACK.is_file(),
                     "set TOKIMEMO_ENGLISH_DISC=1 with disc/orig/track1.bin to run the disc build")
class EndToEndTest(unittest.TestCase):
    def test_disc_build(self):
        from tools.patch import disc as disc_mod
        out = "build/en/disc-test"
        self.assertEqual(build_english.main(["--out", out, "--no-patch"]), 0)
        report = json.loads((REPOSITORY / out / "report.json").read_text())
        self.assertGreater(report["records"]["totals"].get("placed", 0), 30000)
        with open(REPOSITORY / out / "track1.bin", "rb"):
            pass
        track = disc_mod.Track(str(REPOSITORY / out / "track1.bin"))
        try:
            iso = disc_mod.Iso(track)
            for row in report["cd_remap"]:
                entry = iso.find(row["path"])
                self.assertEqual((entry.lba, entry.sectors), (row["new_lba"], row["new_sectors"]))
                self.assertTrue(has_cd_trailer(iso.read(row["path"]) + bytes(-entry.size % 2048))
                                or not (REPOSITORY / "disc" / "files" / row["path"]).read_bytes().endswith(b"\x12\x10"))
            exe = iso.read("SLPM_86.053")
            english = (REPOSITORY / "build" / "en" / "SLPM_86.053.bin").read_bytes()
            self.assertEqual(exe[:0x800], english[:0x800])
            self.assertEqual(struct.unpack_from("<I", exe, 0x10)[0], struct.unpack_from("<I", english, 0x10)[0])
        finally:
            track.close()


if __name__ == "__main__":
    unittest.main()
