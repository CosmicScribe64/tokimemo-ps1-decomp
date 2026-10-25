"""Invented pointer streams for protected prefixes and exact body/source views."""
import copy
import hashlib
import json
from pathlib import Path
import tempfile
import unittest

from tools.assets.text import TextFormatError
from tools.assets.text_bunka_sd import BUNKA_SD_PROFILE, parse_bunka_sd


class BunkaSdTests(unittest.TestCase):
    def fixture(self, root, *, prefix=0x67, first_body=b"\x82\x60", first_nul=True,
                direct=b"\x82\x62"):
        base = 0x80132000
        data = bytearray(192)
        rows = []

        def word(offset, value, target=None):
            raw = value.to_bytes(4, "little")
            data[offset:offset + 4] = raw
            return f"/* {offset:X} {base + offset:08X} {raw.hex()} */ .word {target or hex(value)}"

        rows += ["dlabel PTRS", word(0x20, base + 0x50, "P0"), word(0x24, base + 0x60, "P1"), "enddlabel PTRS"]
        rows += ["dlabel VOICE", *[word(0x30 + i * 4, i + 1) for i in range(4)], "enddlabel VOICE"]
        rows += ["dlabel DIRECT", word(0x40, base + 0x70, "D0"), "enddlabel DIRECT"]
        for label, offset, raw in (("P0", 0x50, bytes([prefix]) + first_body + (b"\0" if first_nul else b"")),
                                   ("P1", 0x60, b"E\x82\x61\0"), ("D0", 0x70, direct + b"\0")):
            data[offset:offset + len(raw)] = raw
            rows += [f"dlabel {label}", f"/* {offset:X} {base + offset:08X} */ .byte " + ",".join(str(v) for v in raw),
                     f"enddlabel {label}"]
        code_path, data_path, source_file = "asm/reader.s", "asm/data.s", "src/reader.c"
        texts = {code_path: "glabel reader\n/* 0 80132000 00000000 */ nop\n/* 4 80132004 00000000 */ nop\nendlabel reader\n",
                 data_path: "\n".join(rows) + "\n", source_file: "void reader(void);\n"}
        for path, text in texts.items():
            target = root / path
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_text(text)
        profile = {"id": "invented", "path": "CDROM/EXEDIR/INVENTED.EXN",
                   "sha256": hashlib.sha256(data).hexdigest(), "size": len(data), "load_address": base,
                   "data_assemblies": [data_path], "code_assemblies": [code_path],
                   "source_pins": {path: hashlib.sha256((root / path).read_bytes()).hexdigest() for path in texts},
                   "limits": ["invented fixture, no execution"],
                   "tables": [{"id": "prefixed", "label": "PTRS", "address": base + 0x20, "count": 2,
                               "assembly_path": data_path, "mode": "prefixed", "body_offset": 1,
                               "handler_family": "intro", "conditions": ["selector"], "evidence": [source_file],
                               "voice": {"label": "VOICE", "address": base + 0x30, "pair_count": 2,
                                         "assembly_path": data_path}},
                              {"id": "direct", "label": "DIRECT", "address": base + 0x40, "count": 1,
                               "assembly_path": data_path, "mode": "direct", "body_offset": 0,
                               "handler_family": "direct", "conditions": ["selector"], "evidence": [source_file]}]}
        return bytes(data), profile

    def parse(self, root, **kwargs):
        data, profile = self.fixture(root, **kwargs)
        return data, profile, parse_bunka_sd(data, root, profile)

    def test_body_offsets_protected_prefix_end_control_and_direct_mode(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            data, profile, parsed = self.parse(root)
            self.assertEqual(parsed["issues"], [])
            self.assertEqual(parsed["coverage"]["display_records"], 3)
            rows = parsed["records"]
            self.assertEqual(rows[0]["offset"], 0x51)
            self.assertEqual(rows[0]["full_offset"], 0x50)
            self.assertEqual(rows[0]["full_raw_hex"], "67826000")
            self.assertEqual(rows[0]["text"]["raw_hex"], "8260")
            self.assertEqual(rows[0]["full_tokens"][0]["kind"], "protected_control_prefix")
            self.assertFalse(rows[0]["full_tokens"][0]["translatable"])
            self.assertEqual(rows[1]["context"]["uses"][0]["prefix_control"]["effect"],
                             "stream_end_after_audio_and_text_ready")
            self.assertEqual(rows[1]["text"]["raw_hex"], "8261")
            self.assertEqual(rows[2]["offset"], rows[2]["full_offset"])
            self.assertEqual(rows[2]["source_relative_offset"], 0)
            self.assertIsNone(rows[2]["context"]["uses"][0]["prefix_control"])
            for row in rows:
                self.assertEqual(b"".join(bytes.fromhex(token["raw_hex"]) for token in row["full_tokens"]).hex(),
                                 row["full_raw_hex"])
                raw = bytes.fromhex(row["text"]["raw_hex"])
                self.assertEqual(data[row["offset"]:row["offset"] + row["length"]], raw)
                self.assertEqual(row["text"]["source_decoded_text"].encode("shift_jis"), raw)

    def test_voice_words_and_unclaimed_source_ranges_are_lossless(self):
        with tempfile.TemporaryDirectory() as temp:
            data, profile, parsed = self.parse(Path(temp))
            pairs = parsed["tables"][0]["voice"]["pairs"]
            self.assertEqual([(pair["clip_start"], pair["clip_end"]) for pair in pairs], [(1, 2), (3, 4)])
            self.assertTrue(parsed["tables"][0]["voice"]["counter_initial_value_not_inferred"])
            self.assertFalse(parsed["records"][0]["context"]["uses"][0]["runtime_reachability_proven"])
            restored, seen = bytearray(len(data)), bytearray(len(data))
            chunks = [(word["offset"], word["raw_hex"]) for proof in parsed["code_proofs"] for word in proof["instructions"]]
            chunks += [(row["offset"], row["raw_hex"]) for table in parsed["tables"] for row in table["rows"]]
            chunks += [(pair["offset"], pair["raw_hex"]) for table in parsed["tables"] if table["voice"] for pair in table["voice"]["pairs"]]
            chunks += [(row["source"]["offset"], row["raw_hex"]) for row in parsed["physical_records"]]
            chunks += [(span["offset"], span["raw_hex"]) for span in parsed["unclaimed_ranges"]]
            for offset, raw_hex in chunks:
                raw = bytes.fromhex(raw_hex)
                restored[offset:offset + len(raw)] = raw
                seen[offset:offset + len(raw)] = b"\1" * len(raw)
            self.assertTrue(all(seen))
            self.assertEqual(bytes(restored), data)
            self.assertFalse(parsed["coverage"]["whole_file_text_absence_proven"])

    def test_direct_strings_never_strip_a_control_looking_first_byte(self):
        with tempfile.TemporaryDirectory() as temp:
            _, _, parsed = self.parse(Path(temp), direct=b"g\x82\x62")
            row = parsed["records"][-1]
            self.assertEqual(row["text"]["raw_hex"], "678262")
            self.assertEqual(row["source_relative_offset"], 0)
            self.assertNotEqual(row["full_tokens"][0]["kind"], "protected_control_prefix")
            self.assertTrue(parsed["coverage"]["renderer_anomaly_records"])

    def test_unknown_prefix_preserves_body_and_explicit_default_behavior(self):
        with tempfile.TemporaryDirectory() as temp:
            _, _, parsed = self.parse(Path(temp), prefix=0x58)
            row = parsed["records"][0]
            prefix = row["context"]["uses"][0]["prefix_control"]
            self.assertFalse(prefix["recognized"])
            self.assertEqual(prefix["effect"], "unknown_prefix_fallthrough")
            self.assertEqual(row["text"]["raw_hex"], "8260")
            self.assertEqual(parsed["coverage"]["unrecognized_prefixes"], 1)
            self.assertEqual(row["full_tokens"][0]["raw_hex"], "58")

    def test_no_nul_read_across_target_label_boundary(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            data, profile = self.fixture(root, first_nul=False)
            self.assertEqual(data[0x53], 0)  # A binary zero exists, but outside the source label.
            with self.assertRaises(TextFormatError):
                parse_bunka_sd(data, root, profile)

    def test_source_byte_failures_remain_raw_without_replacement_unicode(self):
        with tempfile.TemporaryDirectory() as temp:
            _, _, parsed = self.parse(Path(temp), first_body=b"\x81")
            row = parsed["records"][0]
            self.assertIsNone(row["text"]["source_decoded_text"])
            self.assertEqual(row["full_raw_hex"], "678100")
            self.assertEqual(parsed["coverage"]["body_decode_failures"], 1)
            self.assertNotIn("\ufffd", json.dumps(parsed, ensure_ascii=False))

    def test_pins_code_word_pointer_and_count_mismatches_are_rejected(self):
        for problem in ("input", "source", "code", "pointer", "count", "target_label"):
            with self.subTest(problem=problem), tempfile.TemporaryDirectory() as temp:
                root = Path(temp)
                data, profile = self.fixture(root)
                if problem == "input":
                    data = data[:-1] + b"\1"
                elif problem == "source":
                    (root / "src/reader.c").write_text("changed\n")
                elif problem == "code":
                    path = root / "asm/reader.s"
                    path.write_text(path.read_text().replace("00000000", "01000000", 1))
                    profile["source_pins"]["asm/reader.s"] = hashlib.sha256(path.read_bytes()).hexdigest()
                elif problem == "pointer":
                    raw = bytearray(data)
                    raw[0x20] ^= 1
                    data = bytes(raw)
                    profile["sha256"] = hashlib.sha256(data).hexdigest()
                elif problem == "count":
                    profile["tables"][0]["count"] = 3
                else:
                    path = root / "asm/data.s"
                    path.write_text(path.read_text().replace(".word P0", ".word P1"))
                    profile["source_pins"]["asm/data.s"] = hashlib.sha256(path.read_bytes()).hexdigest()
                with self.assertRaises(TextFormatError):
                    parse_bunka_sd(data, root, profile)

    def test_physical_and_body_ids_do_not_depend_on_c_filename(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            data, profile, first = self.parse(root)
            renamed = copy.deepcopy(profile)
            digest = renamed["source_pins"].pop("src/reader.c")
            (root / "src/reader.c").rename(root / "src/renamed.c")
            renamed["source_pins"]["src/renamed.c"] = digest
            second = parse_bunka_sd(data, root, renamed)
            self.assertEqual([row["id"] for row in first["records"]], [row["id"] for row in second["records"]])
            self.assertEqual([row["id"] for row in first["physical_records"]],
                             [row["id"] for row in second["physical_records"]])

    def test_real_profile_is_bounded_source_metadata_without_string_payloads(self):
        self.assertEqual(len(BUNKA_SD_PROFILE["tables"]), 11)
        self.assertEqual(sum(table["count"] for table in BUNKA_SD_PROFILE["tables"]), 286)
        self.assertEqual(sum(table["count"] for table in BUNKA_SD_PROFILE["tables"] if table["mode"] == "prefixed"), 214)
        self.assertEqual(len(BUNKA_SD_PROFILE["source_pins"]), 25)
        self.assertTrue(all(table["renderer"] == "set_kanji_string" for table in BUNKA_SD_PROFILE["tables"]))


if __name__ == "__main__":
    unittest.main()
