"""T-39014 holiday scene profile: decoder, witnesses and original-track negatives."""
import os
from pathlib import Path
import shutil
import struct
import tempfile
import unittest
from unittest import mock

from tools.port import holiday_scene_content as hs

ROOT = Path(__file__).resolve().parents[2]


def _image(words=None, ring=b"\xff" * 7 + b"\x00", pos=0):
    """Minimal PS-X EXE holding only the pinned words, for decoder tests."""
    exe = bytearray(hs.HEADER_SIZE + hs.TEXT_SIZE)
    exe[:8] = b"PS-X EXE"
    struct.pack_into("<II", exe, 0x18, hs.LOAD_ADDRESS, hs.TEXT_SIZE)
    for vaddr, (word, _mask, _meaning) in hs.CODE.items():
        struct.pack_into("<I", exe, hs._offset(vaddr, 4), (words or {}).get(vaddr, word))
    exe[hs._offset(hs.DIAL_KEYS_VADDR, 8):hs._offset(hs.DIAL_KEYS_VADDR, 8) + 8] = ring
    exe[hs._offset(hs.DIAL_POS_VADDR, 1)] = pos
    return bytes(exe)


class DecoderTests(unittest.TestCase):
    def test_values_come_from_decoded_instructions_and_data(self):
        values = hs.derive(_image())["values"]
        self.assertEqual(values, {"dial_keys": [255] * 7 + [0], "dial_pos": 0,
                                  "icon_count": 14, "background_brightness": 128})

    def test_changed_instruction_data_or_header_is_rejected(self):
        cases = {"icon": _image({0x80075678: 0x2409000F}),
                 "brightness": _image({0x80075ADC: 0x24040040}),
                 "register": _image({0x800755C4: 0x27386990}),
                 "ring": _image(ring=b"\xff" * 6 + b"\x00\x00"),
                 "pos": _image(pos=3),
                 "header": b"NOT EXE!" + _image()[8:]}
        for name, exe in cases.items():
            with self.subTest(name=name), self.assertRaises(hs.HolidaySceneError):
                hs.derive(exe)

    def test_changed_address_immediate_is_decoded_and_rejected(self):
        for vaddr, word in ((0x800755C4, 0x27396991), (0x800755B8, 0xA0206999),
                            (0x80075684, 0xA02971EE), (0x8007567C, 0x3C01800F)):
            with self.subTest(vaddr=hex(vaddr)), \
                    self.assertRaisesRegex(hs.HolidaySceneError, "decoded holiday addresses differ"):
                hs.derive(_image({vaddr: word}))

    def test_instructions_record_the_words_read(self):
        instructions = hs.derive(_image())["instructions"]
        self.assertEqual(instructions["0x800755C4"]["word"], "0x27396990")
        self.assertEqual(len(instructions), len(hs.CODE))

    def test_changed_source_witness_is_rejected(self):
        missing = [p for p in hs.SOURCE_FILES if not (ROOT / p).is_file()]
        if missing:
            self.skipTest("generated asm witnesses are absent: " + ", ".join(missing))
        with tempfile.TemporaryDirectory() as temp:
            copy = Path(temp)
            for relative in hs.SOURCE_FILES:
                (copy / relative).parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(ROOT / relative, copy / relative)
            self.assertEqual(hs.check_source(copy), hs.SOURCE_FILES)
            target = copy / "src/main/holiday.c"
            target.write_bytes(target.read_bytes().replace(b"icon_count = 0xE;", b"icon_count = 0xF;", 1))
            with self.assertRaises(hs.HolidaySceneError):
                hs.check_source(copy)


@unittest.skipUnless(os.environ.get("TOKIMEMO_ORIGINAL_TRACK"), "requires the pinned original track")
class OriginalTrackTests(unittest.TestCase):
    track = os.environ.get("TOKIMEMO_ORIGINAL_TRACK", "")

    def test_original_profile(self):
        profile = hs.build_profile(self.track, ROOT)
        self.assertEqual(profile["scene_owned"]["dial_keys"], [255] * 7 + [0])
        self.assertEqual(profile["provenance"]["iso_entry"]["lba"], 24)
        self.assertEqual(sorted(profile["session_owned_fields"]), sorted(hs.SESSION_FIELDS))
        for field in hs.SESSION_FIELDS:
            self.assertNotIn(field, profile["scene_owned"])

    def test_wrong_entry_path_or_record_is_rejected(self):
        with mock.patch.object(hs, "EXE_PATH", "SYSTEM.CNF"), self.assertRaises(hs.HolidaySceneError):
            hs.build_profile(self.track, ROOT)
        with mock.patch.object(hs, "EXE_ENTRY", dict(hs.EXE_ENTRY, lba=25)), self.assertRaises(hs.HolidaySceneError):
            hs.build_profile(self.track, ROOT)

    def test_altered_track_is_rejected(self):
        scratch = os.environ.get("TOKIMEMO_TEST_TMP") or None
        with tempfile.TemporaryDirectory(dir=scratch) as temp:
            altered = Path(temp) / "altered.bin"
            shutil.copyfile(self.track, altered)
            with altered.open("r+b") as stream:
                stream.seek(-1, os.SEEK_END)
                last = stream.read(1)
                stream.seek(-1, os.SEEK_END)
                stream.write(bytes([last[0] ^ 1]))
            with self.assertRaisesRegex(hs.HolidaySceneError, "pinned MODE2"):
                hs.build_profile(str(altered), ROOT)
            with self.assertRaises(hs.HolidaySceneError):
                hs.export(str(altered), ROOT, Path(temp) / "out")
            self.assertFalse((Path(temp) / "out").exists())


if __name__ == "__main__":
    unittest.main()
