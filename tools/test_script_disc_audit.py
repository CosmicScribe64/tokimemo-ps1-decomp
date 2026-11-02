"""T-39510: final-disc pointer and language audit with synthetic data only."""
import struct
import unittest

from tools.patch.audit_script_disc import audit_image, profile_image
from tools.patch.text import profile
from tools.patch.text.image import Image, Pointer, Slot


class ScriptAuditTest(unittest.TestCase):
    def fixture(self, payload=b'Hello!'):
        data = bytearray(64)
        struct.pack_into('<II', data, 0, 0x80100010, 0x80100012)
        data[16:16 + len(payload)] = payload
        image = Image('synthetic', data, 0x80100000,
                      [Slot('line', 16, payload, 'relocatable')],
                      [Pointer(0, 'line'), Pointer(4, 'line', 2)])
        return image, data

    def test_reads_relocated_pointers_and_suffix_alias(self):
        image, data = self.fixture(b'A\xa0\xb0!')
        data[32:37] = data[16:21]
        struct.pack_into('<II', data, 0, 0x80100020, 0x80100022)
        result = audit_image(image, data, {})
        self.assertEqual((result['strings'], result['pointers']), (1, 2))

    def test_original_font_bytes_are_rejected_even_if_expected(self):
        image, data = self.fixture(b'\x82\xa0')
        with self.assertRaisesRegex(ValueError, 'Original-font'):
            audit_image(image, data, {})

    def test_corrupt_pointer_and_payload_are_rejected(self):
        image, data = self.fixture()
        struct.pack_into('<I', data, 0, 0x80100200)
        with self.assertRaisesRegex(ValueError, 'outside file'):
            audit_image(image, data, {})
        image, data = self.fixture()
        data[16] = ord('X')
        with self.assertRaisesRegex(ValueError, 'differs'):
            audit_image(image, data, {})

    def test_unreached_slot_fails_coverage(self):
        image, data = self.fixture()
        image.pointers = []
        with self.assertRaisesRegex(ValueError, 'Unverified script slots'):
            audit_image(image, data, {})

    def test_fixed_table_slot_is_read_at_its_actual_offset(self):
        image, data = self.fixture()
        image.slots['line'].mode = 'in_place'
        image.pointers = []
        self.assertEqual(audit_image(image, data, {})['strings'], 1)
        data[16] = ord('X')
        with self.assertRaisesRegex(ValueError, 'differs'):
            audit_image(image, data, {})

    def test_profile_labels_are_audited_at_their_moved_positions(self):
        original = bytearray(0x13000)
        actual = bytearray(original)
        slots = []
        for table, base, stride, count in (
                ('club_labels', 0x12F00, 18, 13), ('zodiac_labels', 0x12200, 12, 12)):
            for i in range(count):
                rid = f'dictionary:{profile.PATH}:{table}:{i:02d}'
                offset = 16 * len(slots)
                original[offset:offset + 6] = b'Label\0'
                slots.append(Slot(rid, offset, b'Label', capacity=16))
                actual[base + i * stride:base + i * stride + 6] = b'Label\0'
        image = Image(profile.PATH, original, 0, slots, [])
        moved = profile_image(image)
        self.assertEqual(audit_image(moved, actual, {})['strings'], 25)
        self.assertEqual(next(iter(image.slots.values())).offset, 0)
        actual[0x12F00] = ord('X')
        with self.assertRaisesRegex(ValueError, 'differs'):
            audit_image(moved, actual, {})


if __name__ == '__main__':
    unittest.main()
