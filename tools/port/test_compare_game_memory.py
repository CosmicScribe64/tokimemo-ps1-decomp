"""T-39300: tools/port/compare_game_memory.py on synthetic RAM images (invented bytes only).

Run in Docker: python3 -m unittest discover -s tools/port -p 'test_compare_game_memory.py'
"""
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import compare_game_memory as cmp  # noqa: E402
import gen_game_memory as gen  # noqa: E402

GS = 0x800E6280


class CompareTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.parser = gen.load_model()

    def images(self):
        return bytearray(cmp.RAM_SIZE), bytearray(cmp.RAM_SIZE)

    def put(self, ram, address, size, value):
        off = address & 0x1FFFFF
        ram[off:off + size] = (value & ((1 << (size * 8)) - 1)).to_bytes(size, "little")

    def fields(self, port, emu):
        return {f["name"]: f for f in cmp.compare_struct(self.parser, "GameState", GS, port, emu, {})}

    def test_equal_images_have_no_differences(self):
        port, emu = self.images()
        fields = self.fields(port, emu)
        self.assertEqual(len(fields), 161)
        self.assertTrue(all(f["equal"] for f in fields.values()))

    def test_first_differing_field_is_named_with_its_path(self):
        port, emu = self.images()
        self.put(emu, GS + 0x1BC + 3 * 0x38 + 2, 2, 55)           # girls[3].tokimeki
        self.put(emu, GS + 0x56C + 10, 1, 7)                      # scene_flags[10]
        fields = self.fields(port, emu)
        self.assertEqual(sorted(n for n, f in fields.items() if not f["equal"]), ["girls", "scene_flags"])
        self.assertEqual(fields["girls"]["differing"][0]["path"], "girls[3].tokimeki")
        self.assertEqual(fields["girls"]["differing"][0]["address"], "0x%08X" % (GS + 0x1BC + 3 * 0x38 + 2))
        self.assertEqual(fields["scene_flags"]["differing"][0]["path"], "scene_flags[10]")
        self.assertEqual(fields["scene_flags"]["differing"][0]["emulator"], "7 (0x07)")

    def test_signed_values_and_widths(self):
        port, emu = self.images()
        self.put(port, GS + 0xF7A, 2, -5)                          # mouse_x s16
        d = self.fields(port, emu)["mouse_x"]["differing"][0]
        self.assertEqual(d["port"], "-5 (0xFFFB)")

    def test_bit_fields_are_named(self):
        port, emu = self.images()
        flags = GS + 0x1BC + 2 * 0x38 + 0xC
        self.put(port, flags, 4, 2 << 20)                          # club = 2
        self.put(emu, flags, 4, 3 << 20)                           # club = 3
        d = self.fields(port, emu)["girls"]["differing"][0]
        self.assertEqual(d["path"], "girls[2].flags")
        self.assertEqual(d["bit_fields"], ["f.club port=2 emu=3"])

    def test_clock_fields_are_not_counted(self):
        port, emu = self.images()
        self.put(port, GS + 0x10F8, 4, 100)                        # frame_count
        fields = cmp.compare_struct(self.parser, "GameState", GS, port, emu, {})
        summary = cmp.summarize("GameState", fields, GS, 0x1A90, port, emu)
        self.assertEqual(summary["fields_differing"], [])
        self.assertEqual([c["name"] for c in summary["clock_fields"]], ["timer_ticks", "frame_count"])
        self.assertEqual(summary["bytes_equal"], 0x1A90 - 1)

    def test_known_reasons_and_globals(self):
        port, emu = self.images()
        self.put(emu, GS + 0x1128, 4, 5)                           # tpage_slot_state[0]
        fields = {f["name"]: f for f in cmp.compare_struct(self.parser, "GameState", GS, port, emu,
                                                           {"GameState.tpage_slot_state": "loader"})}
        self.assertEqual(fields["tpage_slot_state"]["reason"], "loader")
        self.put(emu, 0x800B6740, 1, 0xFF)                          # schedule_last_picked_girl s8
        glob = cmp.compare_globals(self.parser, ["schedule_last_picked_girl", "icon_enabled"], port, emu)
        self.assertEqual(glob[0]["differing"][0]["emulator"], -1)
        self.assertTrue(glob[1]["equal"] and glob[1]["elements"] == 15)

    def test_main_returns_one_on_difference(self):
        import tempfile
        port, emu = self.images()
        self.put(emu, GS + 0x3E, 1, 95)                            # year
        with tempfile.TemporaryDirectory() as tmp:
            a, b = Path(tmp) / "port.bin", Path(tmp) / "emu.bin"
            a.write_bytes(bytes(port))
            b.write_bytes(bytes(emu))
            self.assertEqual(cmp.main([str(a), str(b), "--json", str(Path(tmp) / "r.json")]), 1)
            b.write_bytes(bytes(port))
            self.assertEqual(cmp.main([str(a), str(b)]), 0)


if __name__ == "__main__":
    unittest.main()
