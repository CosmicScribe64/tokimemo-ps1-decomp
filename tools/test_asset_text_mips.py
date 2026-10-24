"""Invented original-byte MIPS fixtures for static text arguments (T-28001)."""
import hashlib
import tempfile
import unittest
from pathlib import Path

from tools.assets.text_mips import enrich_mips_context

BASE = 0x80041000
TEXT = 0x80042000
CALLEE = BASE + 0x100


def lui(rt, value):
    return (15 << 26) | (rt << 16) | value


def ori(rt, rs, value):
    return (13 << 26) | (rs << 21) | (rt << 16) | value


def jal(address):
    return (3 << 26) | ((address >> 2) & 0x03FFFFFF)


class MipsContextTests(unittest.TestCase):
    def fixture(self, root, words, callee="printf", labels=None):
        for directory in ("asm/matchings/main/unit", "config", "disc/files"):
            (root / directory).mkdir(parents=True)
        (root / "config/overlays.txt").write_text("")
        (root / "config/symbol_addrs_sdk.txt").write_text(f"{callee} = 0x{CALLEE:08X};\n")
        binary = bytearray(0x2000)
        lines = ["glabel caller"]
        for i, word in enumerate(words):
            pc = BASE + i * 4
            if labels and i in labels:
                lines.append("jlabel join")
            raw = word.to_bytes(4, "little")
            binary[0x800 + i * 4:0x804 + i * 4] = raw
            lines.append(f"/* {i * 4:X} {pc:08X} {raw.hex()} */ instruction")
        (root / "asm/matchings/main/unit/caller.s").write_text("\n".join(lines))
        code = (0x03E00008).to_bytes(4, "little")
        binary[0x900:0x904] = code
        (root / "asm/matchings/main/unit/callee.s").write_text(
            f"glabel {callee}\n/* 100 {CALLEE:08X} {code.hex()} */ jr $ra\n"
            f"/* 104 {CALLEE + 4:08X} 00000000 */ nop\n")
        raw_text = b"INVENTED\0"
        offset = TEXT - BASE + 0x800
        binary[offset:offset + len(raw_text)] = raw_text
        (root / "disc/files/SLPM_86.053").write_bytes(binary)
        record = {"id": "invented", "category": "unknown", "raw_hex": raw_text.hex(),
                  "raw_length": len(raw_text), "extent_status": "verified",
                  "source": {"path": "SLPM_86.053", "address": TEXT, "offset": offset,
                             "length": len(raw_text), "file_size": len(binary),
                             "sha256": hashlib.sha256(binary).hexdigest()}}
        return {"records": [record]}

    def run_case(self, words, callee="printf", labels=None):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            inventory = self.fixture(root, words, callee, labels)
            return enrich_mips_context(inventory, root, root / "disc/files")

    def test_jal_delay_slot_finishes_argument_and_has_original_evidence(self):
        result = self.run_case([lui(4, TEXT >> 16), jal(CALLEE), ori(4, 4, TEXT & 0xFFFF)])
        context = result["records"][0]["mips_contexts"][0]
        self.assertEqual(context["argument_value"], TEXT)
        self.assertEqual(context["argument_register"], "a0")
        self.assertEqual(context["delay_slot_pc"], BASE + 8)
        self.assertEqual(len(context["instruction_evidence"]), 3)
        self.assertEqual(context["status"], "potential_static_callsite")
        self.assertEqual(context["callee_evidence"]["pc"], CALLEE)

    def test_coordinate_or_numeric_printf_argument_is_not_a_string(self):
        for callee, register in (("set_kanji_string", 4), ("printf", 5)):
            result = self.run_case([lui(register, TEXT >> 16), ori(register, register, TEXT & 0xFFFF), jal(CALLEE), 0], callee)
            self.assertEqual(result["mips_context"]["contexts"], 0)
        actual = self.run_case([lui(7, TEXT >> 16), ori(7, 7, TEXT & 0xFFFF), jal(CALLEE), 0], "set_kanji_string")
        self.assertEqual(actual["records"][0]["mips_contexts"][0]["argument_register"], "a3")

    def test_unknown_call_clobbers_argument_registers(self):
        result = self.run_case([lui(4, TEXT >> 16), ori(4, 4, TEXT & 0xFFFF), jal(CALLEE + 0x100), 0, jal(CALLEE), 0])
        self.assertEqual(result["mips_context"]["contexts"], 0)

    def test_jalr_target_construction_and_negative_addiu_have_evidence(self):
        jalr_t9 = (25 << 21) | (31 << 11) | 9
        result = self.run_case([lui(4, TEXT >> 16), lui(25, CALLEE >> 16), ori(25, 25, CALLEE & 0xFFFF),
                                jalr_t9, ori(4, 4, TEXT & 0xFFFF)])
        context = result["records"][0]["mips_contexts"][0]
        self.assertEqual({i["pc"] for i in context["instruction_evidence"]}, {BASE + i * 4 for i in range(5)})
        negative_addiu = (9 << 26) | (4 << 21) | (4 << 16) | 0xFFF0
        result = self.run_case([lui(4, TEXT >> 16), ori(4, 4, (TEXT & 0xFFFF) + 16), negative_addiu,
                                jal(CALLEE), 0])
        self.assertEqual(result["mips_context"]["contexts"], 1)

    def test_callee_saved_constant_and_move_survive_call(self):
        move_a0_s0 = (16 << 21) | (4 << 11) | 0x25
        result = self.run_case([lui(16, TEXT >> 16), ori(16, 16, TEXT & 0xFFFF), jal(CALLEE + 0x100), 0,
                                move_a0_s0, jal(CALLEE), 0])
        self.assertEqual(result["mips_context"]["contexts"], 1)

    def test_branches_and_labels_invalidate_incoming_constants(self):
        words = [lui(4, TEXT >> 16), ori(4, 4, TEXT & 0xFFFF), 4 << 26, 0, jal(CALLEE), 0]
        self.assertEqual(self.run_case(words)["mips_context"]["contexts"], 0)
        words = [lui(4, TEXT >> 16), ori(4, 4, TEXT & 0xFFFF), jal(CALLEE), 0]
        self.assertEqual(self.run_case(words, labels={1})["mips_context"]["contexts"], 0)
        # Taken flow bypasses both assignments; numeric join detection must not
        # depend on the disassembler emitting an explicit target label.
        branch_to_call = (4 << 26) | (5 << 16) | 3
        words = [branch_to_call, 0, lui(4, TEXT >> 16), ori(4, 4, TEXT & 0xFFFF), jal(CALLEE), 0]
        self.assertEqual(self.run_case(words)["mips_context"]["contexts"], 0)

    def test_interior_and_unexported_constant_targets_remain_explicit(self):
        result = self.run_case([lui(4, TEXT >> 16), ori(4, 4, (TEXT + 2) & 0xFFFF), jal(CALLEE), 0])
        self.assertEqual(result["mips_context"]["contexts"], 0)
        interior = result["mips_context"]["interior_references"][0]
        self.assertEqual(interior["relative_byte_offset"], 2)
        self.assertTrue(interior["substring_bytes_not_decoded"])
        self.assertEqual(result["records"][0]["category"], "unknown")
        result = self.run_case([lui(4, TEXT >> 16), ori(4, 4, (TEXT + 12) & 0xFFFF), jal(CALLEE), 0])
        self.assertEqual(len(result["mips_context"]["unresolved_constant_targets"]), 1)

    def test_empty_source_extent_keeps_nontranslatable_classification(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            inventory = self.fixture(root, [lui(4, TEXT >> 16), ori(4, 4, TEXT & 0xFFFF), jal(CALLEE), 0])
            path = root / "disc/files/SLPM_86.053"
            binary = bytearray(path.read_bytes())
            binary[TEXT - BASE + 0x800] = 0
            path.write_bytes(binary)
            record = inventory["records"][0]
            record.update(raw_hex="00", raw_length=1, category="empty_extent_no_translatable_bytes")
            record["source"].update(length=1, sha256=hashlib.sha256(binary).hexdigest())
            enrich_mips_context(inventory, root, root / "disc/files")
            self.assertEqual(record["category"], "empty_extent_no_translatable_bytes")
            self.assertEqual(len(record["mips_contexts"]), 1)

    def test_memory_load_and_unmodeled_write_are_unknown(self):
        lw_a0 = (0x23 << 26) | (28 << 21) | (4 << 16)
        signed_add_a0 = (4 << 21) | (4 << 11) | 0x20
        for instruction in (lw_a0, signed_add_a0):
            result = self.run_case([lui(4, TEXT >> 16), ori(4, 4, TEXT & 0xFFFF), instruction, jal(CALLEE), 0])
            self.assertEqual(result["mips_context"]["contexts"], 0)

    def test_shift_constants_are_32_bit_and_addiu_is_sign_extended(self):
        srl_t0 = (8 << 16) | (8 << 11) | (16 << 6) | 2
        sll_t0 = (8 << 16) | (8 << 11) | (16 << 6)
        addiu_a0 = (9 << 26) | (8 << 21) | (4 << 16) | 0x2000
        result = self.run_case([lui(8, 0x8004), srl_t0, sll_t0, addiu_a0, jal(CALLEE), 0])
        self.assertEqual(result["mips_context"]["contexts"], 1)

    def test_stale_text_hash_and_wrong_instruction_bytes_block_proof(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            inventory = self.fixture(root, [lui(4, TEXT >> 16), ori(4, 4, TEXT & 0xFFFF), jal(CALLEE), 0])
            inventory["records"][0]["source"]["sha256"] = "0" * 64
            result = enrich_mips_context(inventory, root, root / "disc/files")
            self.assertEqual(result["mips_context"]["contexts"], 0)
            self.assertIn("text_extent_unverified", [b["status"] for b in result["mips_context"]["blocked"]])
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            inventory = self.fixture(root, [lui(4, TEXT >> 16), ori(4, 4, TEXT & 0xFFFF), jal(CALLEE), 0])
            path = root / "disc/files/SLPM_86.053"
            binary = bytearray(path.read_bytes())
            binary[0x800] ^= 1
            path.write_bytes(binary)
            inventory["records"][0]["source"]["sha256"] = hashlib.sha256(binary).hexdigest()
            result = enrich_mips_context(inventory, root, root / "disc/files")
            self.assertEqual(result["mips_context"]["contexts"], 0)
            self.assertIn("instruction_bytes_disagree", [b["status"] for b in result["mips_context"]["blocked"]])

    def test_revalidation_does_not_retain_stale_context_or_category(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            inventory = self.fixture(root, [lui(4, TEXT >> 16), ori(4, 4, TEXT & 0xFFFF), jal(CALLEE), 0])
            enrich_mips_context(inventory, root, root / "disc/files")
            self.assertEqual(inventory["records"][0]["category"], "debug_or_format")
            inventory["records"][0]["source"]["sha256"] = "0" * 64
            enrich_mips_context(inventory, root, root / "disc/files")
            self.assertEqual(inventory["records"][0]["mips_contexts"], [])
            self.assertEqual(inventory["records"][0]["category"], "unknown")

    def test_foreign_overlay_same_vma_is_not_visible(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            inventory = self.fixture(root, [])
            main_record = inventory["records"][0]
            (root / "config/overlays.txt").write_text("A 0x80132000 0x80\nB 0x80132000 0x80\n")
            directory = root / "disc/files/CDROM/EXEDIR"
            directory.mkdir(parents=True)
            caller = bytearray(0x80)
            words = [lui(4, 0x8013), ori(4, 4, 0x2020), jal(CALLEE), 0]
            lines = ["glabel overlay_caller"]
            for i, word in enumerate(words):
                raw = word.to_bytes(4, "little")
                caller[i * 4:i * 4 + 4] = raw
                lines.append(f"/* {i * 4:X} {0x80132000 + i * 4:08X} {raw.hex()} */ instruction")
            (directory / "A.EXN").write_bytes(caller)
            foreign = bytearray(0x80)
            foreign[0x20:0x23] = b"AB\0"
            (directory / "B.EXN").write_bytes(foreign)
            asm = root / "asm/ovl/A/matchings/A/unit"
            asm.mkdir(parents=True)
            (asm / "caller.s").write_text("\n".join(lines))
            record = {"id": "foreign", "category": "unknown", "raw_hex": "414200", "raw_length": 3,
                      "extent_status": "verified", "source": {"path": "CDROM/EXEDIR/B.EXN",
                      "offset": 0x20, "address": 0x80132020, "length": 3, "file_size": 0x80,
                      "sha256": hashlib.sha256(foreign).hexdigest()}}
            inventory["records"] = [record]
            result = enrich_mips_context(inventory, root, root / "disc/files")
            self.assertEqual(result["mips_context"]["contexts"], 0)
            words = [lui(4, TEXT >> 16), ori(4, 4, TEXT & 0xFFFF), jal(CALLEE), 0]
            lines = ["glabel overlay_caller"]
            for i, word in enumerate(words):
                raw = word.to_bytes(4, "little")
                caller[i * 4:i * 4 + 4] = raw
                lines.append(f"/* {i * 4:X} {0x80132000 + i * 4:08X} {raw.hex()} */ instruction")
            (directory / "A.EXN").write_bytes(caller)
            (asm / "caller.s").write_text("\n".join(lines))
            result = enrich_mips_context({"records": [main_record]}, root, root / "disc/files")
            self.assertEqual(result["mips_context"]["contexts"], 1)
            self.assertEqual(main_record["mips_contexts"][0]["caller_unit"], "A")


if __name__ == "__main__":
    unittest.main()
