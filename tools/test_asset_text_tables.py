"""Invented counted glyph-list fixtures for T-28001."""
import struct
import unittest
import copy
import hashlib
import tempfile
from pathlib import Path

from tools.assets.text_tables import verify_table_source_proof


class CurrentTableProofTests(unittest.TestCase):
    def test_current_source_formula_code_and_data_are_independently_checked(self):
        data = b'A\0\0\0\0\0\0\0'
        digest = hashlib.sha256(data).hexdigest()
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            repo, inputs = root/'source', root/'inputs'
            repo.mkdir(); inputs.mkdir()
            (inputs/'synthetic.bin').write_bytes(data)
            sources = {'table.s':'dlabel table\n/* 0 00001000 41000000 */ .byte 0x41, 0, 0, 0\nenddlabel table\n',
                       'code.s':'glabel reader\n/* 4 00001004 00000000 */ nop\n',
                       'proof.c':'s32 stride = 4;\n'}
            for name, text in sources.items():
                (repo/name).write_text(text)
            profile = {'path':'synthetic.bin','sha256':digest,'load_address':0x1000,'header_size':0,
                       'source_pins':{n:hashlib.sha256(t.encode()).hexdigest() for n,t in sources.items()},
                       'source_checks':[{'path':'proof.c','contains':'s32 stride = 4;'}],
                       'code_proofs':[{'assembly_path':'code.s','source_path':'synthetic.bin',
                           'source_sha256':digest,'load_address':0x1000,'header_size':0}],
                       'data_proofs':[{'assembly_path':'table.s','label':'table','address':0x1000,'length':4}]}
            result = verify_table_source_proof(data,profile,repo,inputs)
            self.assertEqual(result['data_labels'][0]['initialized_bytes_verified'],4)
            self.assertEqual(result['code'][0]['instruction_words_verified'],1)
            changed = copy.deepcopy(profile)
            changed['data_proofs'][0]['length']=5
            with self.assertRaises(ValueError):
                verify_table_source_proof(data,changed,repo,inputs)
            (repo/'proof.c').write_text('s32 stride = 8;\n')
            with self.assertRaises(ValueError):
                verify_table_source_proof(data,profile,repo,inputs)
            changed = copy.deepcopy(profile)
            changed['source_pins']['proof.c']=hashlib.sha256((repo/'proof.c').read_bytes()).hexdigest()
            with self.assertRaisesRegex(ValueError,'formula/type'):
                verify_table_source_proof(data,changed,repo,inputs)
from tools.assets.text import TextFormatError
from tools.assets.text_tables import parse_character_lists


class CharacterListTests(unittest.TestCase):
    def fixture(self):
        data = bytearray(64)
        struct.pack_into("<HH", data, 0, 2, 1)
        data[4:10] = "テト".encode("shift_jis") + b"\0\0"
        data[12:16] = "カ".encode("shift_jis") + b"\0\0"
        struct.pack_into("<II", data, 32, 4, 12)
        return bytes(data), {"id": "invented", "path": "TEST.NBN", "counts_offset": 0,
                             "counts_count": 2, "offsets_offset": 32, "offsets_count": 2,
                             "list_count": 2, "load_address": 0x80000000}

    def test_exact_counts_and_provenance(self):
        data, profile = self.fixture()
        parsed = parse_character_lists(data, profile)
        self.assertEqual(parsed["coverage"]["glyph_entries"], 3)
        self.assertEqual(parsed["records"][1]["offset"], 12)
        self.assertEqual(parsed["records"][0]["text"]["source_decoded_text"], "テト")
        self.assertEqual(bytes.fromhex(parsed["records"][0]["text"]["raw_hex"]), data[4:8])

    def test_bad_offsets_terminators_and_overlap_fail(self):
        data, profile = self.fixture()
        for offset, value in ((32, 31), (36, 6)):
            bad = bytearray(data)
            struct.pack_into("<I", bad, offset, value)
            with self.assertRaises(TextFormatError):
                parse_character_lists(bytes(bad), profile)
        bad = bytearray(data)
        bad[8] = 1
        with self.assertRaises(TextFormatError):
            parse_character_lists(bytes(bad), profile)

    def test_single_byte_kana_are_rejected(self):
        data, profile = self.fixture()
        bad = bytearray(data)
        bad[4:8] = bytes.fromhex("a1a2a3a4")
        with self.assertRaises(TextFormatError):
            parse_character_lists(bytes(bad), profile)


class DictionaryTests(unittest.TestCase):
    def test_pointer_and_fixed_width_terms_preserve_every_slot_byte(self):
        from tools.assets.text_tables import parse_dictionary
        data = bytearray(64)
        struct.pack_into("<I", data, 0, 0x1010)
        data[16:19] = "テ".encode("shift_jis") + b"\0"
        data[24:28] = "ト".encode("shift_jis") + b"\0\0"
        profile = {"id": "invented", "path": "TERMS.BIN", "load_address": 0x1000,
                   "header_size": 0, "tables": [
                       {"id": "pointers", "address": 0x1000, "count": 1, "kind": "pointer_strings"},
                       {"id": "slots", "address": 0x1018, "count": 1, "kind": "fixed_strings", "stride": 4}]}
        parsed = parse_dictionary(bytes(data), profile)
        self.assertEqual(parsed["coverage"]["records"], 2)
        self.assertEqual(parsed["records"][0]["offset"], 16)
        self.assertEqual(parsed["records"][1]["padding_raw_hex"], "00")
        bad = bytearray(data)
        struct.pack_into("<I", bad, 0, 0xFFFF)
        with self.assertRaises(TextFormatError):
            parse_dictionary(bytes(bad), profile)


if __name__ == "__main__":
    unittest.main()
