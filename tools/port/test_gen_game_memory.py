"""T-39300: the generated game layout is reproducible and agrees with the header comments.

Run in Docker: python3 -m unittest discover -s tools/port -p 'test_gen_game_memory.py'
"""
import re
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import gen_game_memory as gen  # noqa: E402


class GameLayoutTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.text = gen.generate()
        cls.parser = gen.load_model()

    def test_committed_file_is_current(self):
        self.assertEqual(gen.DEFAULT_OUT.read_text(), self.text,
                         "regenerate with tools/port/gen_game_memory.py")

    def test_output_is_deterministic(self):
        self.assertEqual(gen.generate(), self.text)

    def test_header_comments_match_computed_layout(self):
        self.assertEqual(gen.layout_errors(self.parser), [])

    def test_game_state_size_and_base(self):
        state = self.parser.structs["GameState"]
        self.assertEqual(state.size, 0x1A90)
        self.assertEqual(state.size_comment, 0x1A90)
        self.assertIn("const SIZEOF_GAME_STATE: int = 0x1A90", self.text)
        self.assertEqual(self.parser.globals["game_state"].address, 0x800E6280)
        self.assertEqual(self.parser.globals["system_data"].address, 0x800E7D10)

    def test_game_state_offsets_against_header_text(self):
        """Independent of the parser: each `/* 0xNNN */ type name` line of game_state.h."""
        header = (gen.REPO / "include/game/game_state.h").read_text()
        body = header[header.index("typedef struct GameState"):header.index("} GameState;")]
        rows = re.findall(r"/\*\s*(0x[0-9A-Fa-f]+)\s*\*/\s*(?:volatile\s+)?\w+\s+(\w+)(\[[^;]*\])?;", body)
        self.assertEqual(len(rows), 161)
        members = {m.name: m for m in self.parser.structs["GameState"].members}
        for offset, name, dims in rows:
            self.assertIn(name, members)
            self.assertEqual(members[name].offset, int(offset, 16), name)
            self.assertIn("const GAME_STATE_%s: int = 0x%X" % (gen.snake_upper(name), int(offset, 16)), self.text)
        self.assertEqual(len(members), 161)

    def test_widths_and_signedness_of_scalars(self):
        header = (gen.REPO / "include/game/game_state.h").read_text()
        body = header[header.index("typedef struct GameState"):header.index("} GameState;")]
        sizes = {"u8": (1, False), "s8": (1, True), "u16": (2, False), "s16": (2, True),
                 "u32": (4, False), "s32": (4, True)}
        members = {m.name: m for m in self.parser.structs["GameState"].members}
        checked = 0
        for kind, name in re.findall(r"/\*\s*0x[0-9A-Fa-f]+\s*\*/\s*(u8|s8|u16|s16|u32|s32)\s+(\w+)\s*(?:\[|;)", body):
            width, signed = sizes[kind]
            m = members[name]
            self.assertEqual((m.type.width, m.type.signed), (width, signed), name)
            checked += 1
        self.assertGreater(checked, 100)

    def test_nested_and_bit_field_records(self):
        girl = self.parser.structs["GirlRecord"]
        self.assertEqual(girl.size, 0x38)
        game_state = {m.name: m for m in self.parser.structs["GameState"].members}
        self.assertEqual((game_state["girls"].offset, game_state["girls"].count), (0x1BC, 16))
        flags = self.parser.structs["GirlFlagBits"]
        self.assertEqual(flags.size, 4)
        fields = {m.name: m for m in flags.members}
        self.assertEqual((fields["story_stage"].bit_shift, fields["story_stage"].bits), (9, 3))

    def test_bit_positions_against_independent_sources(self):
        """Bit positions inside a unit are not covered by the offset comments; these come from other text."""
        def bits(struct, name):
            member = {m.name: m for m in self.parser.structs[struct].members}[name]
            return member.offset, member.bit_shift, member.bits
        # include/game/word_views.h BirthdayBits: month bits 0-3, day bits 4-8; blood_type follows (bits 9-10),
        # as port/godot/rules/new_game.gd _birthday stores it: month & 15 | day << 4 | blood << 9.
        self.assertEqual(bits("BirthdayBits", "month"), (0, 0, 4))
        self.assertEqual(bits("BirthdayBits", "day"), (0, 4, 5))
        self.assertEqual(bits("BirthdayBits", "blood_type"), (0, 9, 2))
        # include/game/girl.h GirlFlagBits: "reads bits 9-11 as one value" (story_stage); met is bit 1.
        self.assertEqual(bits("GirlFlagBits", "story_stage"), (0, 9, 3))
        self.assertEqual(bits("GirlFlagBits", "met"), (0, 1, 1))
        # include/game/display_records.h ColorPair555: the second colour starts at bit 16 (comment 0x02).
        self.assertEqual(bits("ColorPair555", "r1"), (0, 16, 5))

    def test_constant_evaluator_refuses_everything_but_integers(self):
        self.assertEqual(gen.eval_int("0x10 * (2 + 1) << 1"), 96)
        self.assertEqual(gen.eval_int("7 / 2"), 3)
        for text in ("__import__('os')", "a + 1", "1.5", "2 ** 3", "(1", "1 / 0", "[1]"):
            with self.assertRaises(gen.ParseError):
                gen.eval_int(text)

    def test_globals_resolve_to_addresses(self):
        self.assertIn('"schedule_last_picked_girl": [0x800B6740, 1, true, 1, "", 1]', self.text)
        self.assertIn('"game_state": [0x800E6280, 6800, false, 1, "GameState", 6800]', self.text)


if __name__ == "__main__":
    unittest.main()
