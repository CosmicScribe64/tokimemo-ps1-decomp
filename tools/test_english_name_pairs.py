"""T-39506: persistent name-pair encoding, using synthetic input only."""
import itertools
import ctypes
from pathlib import Path
import subprocess
import tempfile
import unittest

from tools.patch.text import names
from tools import english_font

ROOT = Path(__file__).resolve().parents[1]


class RuntimeNamePairsTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.folder = tempfile.TemporaryDirectory()
        root = Path(cls.folder.name)
        (root / 'common.h').write_text('typedef unsigned char u8; typedef unsigned int u32; typedef int s32;\n')
        library = root / 'names.so'
        subprocess.run(['cc', '-std=c89', '-Wall', '-Wextra', '-Werror', '-shared', '-fPIC',
                        '-DTOKIMEMO_ENGLISH', '-I' + str(root), '-I' + str(ROOT / 'include'),
                        str(ROOT / 'src/english/names.c'), '-o', str(library)], check=True)
        cls.runtime = ctypes.CDLL(str(library))
        for function in (cls.runtime.english_name_pack, cls.runtime.english_name_unpack):
            function.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]
            function.restype = ctypes.c_int

    @classmethod
    def tearDownClass(cls):
        cls.folder.cleanup()

    def invoke(self, function, data, size=64):
        buffer = ctypes.create_string_buffer(b'!' * 64, 65)
        result = function(buffer, data, size)
        self.assertEqual(buffer.raw[size:64], b'!' * (64 - size))
        if size > 0:
            self.assertIn(b'\0', buffer.raw[:size])
        return result, buffer.value

    def test_runtime_matches_writer_for_ascii_and_sjis(self):
        inputs = [bytes(pair) for pair in itertools.product(range(0x20, 0x7F), repeat=2)]
        inputs += [b'ka' + bytes((lead, trail)) + b'shi'
                   for lead in (0x81, 0x82, 0xE0, 0xEF) for trail in range(0xB0, 0xE0)]
        for data in inputs:
            packed = names.pack(data)
            self.assertEqual(self.invoke(self.runtime.english_name_pack, data), (len(packed), packed))
            self.assertEqual(self.invoke(self.runtime.english_name_unpack, packed), (len(data), data))

    def test_small_buffers_fail_without_overwrite_or_half_character(self):
        for size in range(8):
            for data in (b'kashiro', b'\x81\xb0\x82\xdf', b'ABCDEFG'):
                result, output = self.invoke(self.runtime.english_name_pack, data, size)
                expected = names.pack(data)
                self.assertEqual(result >= 0, len(expected) < size)
                if result >= 0:
                    self.assertEqual(output, expected)
            result, _ = self.invoke(self.runtime.english_name_unpack, b'\xb0\x81\xb0', size)
            self.assertEqual(result >= 0, size >= 5)

    def test_renderer_width_glyph_count_and_pen_positions(self):
        source = (ROOT / 'src/english/text.c').read_text()
        functions = source[source.index('s32 english_load_string('):source.index('/* set_kanji_string without')]
        fixture = r'''
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "english/names.h"
#define ENGLISH_GLYPH_SLOTS 384
struct { s32 total; s32 glyph_advance; } text_state;
struct { u32 first; u32 glyph; } text_font_range_table[46];
u8 english_font_index[256], english_font_advance[256], english_font_glyphs[256 * 18];
short english_glyph_pen[384];
s32 english_text_cell;
u8 observed[384];
static void english_glyph_load(u8 *bits) {
    observed[text_state.total++] = (bits - english_font_glyphs) / 18;
}
static void vload_font(u32 glyph, u32 offset) {
    (void)glyph; (void)offset;
    observed[text_state.total++] = '?';
}
'''
        checks = r'''
int main(void) {
    u8 packed[32];
    u8 plain[] = "Mikashiro";
    u8 kana[] = {0x82, 0xB0, 0xB0, 0};
    int i, count;
    memset(english_font_index, 255, sizeof(english_font_index));
    for (i = 32; i < 127; i++) {
        english_font_index[i] = i;
        english_font_advance[i] = 6;
    }
    for (i = 0; i < 46; i++) {
        text_font_range_table[i].first = 0x8100 + i * 0x100;
    }
    text_state.glyph_advance = 14;
    assert(english_name_pack(packed, plain, sizeof(packed)) > 0);
    assert(english_text_width(packed) == english_text_width(plain));
    count = english_load_string(packed);
    assert(count == (int)strlen((char *)plain));
    assert(!memcmp(observed, plain, count));
    for (i = 0; i < count; i++) { assert(english_glyph_pen[i] == i * 6); }
    text_state.total = 0;
    assert(english_text_width(kana) == 26);
    assert(english_load_string(kana) == 3);
    assert(!memcmp(observed, "?ka", 3));
    assert(english_glyph_pen[1] == 14 && english_glyph_pen[2] == 20);
    text_state.total = 0;
    english_text_cell = 16;
    assert(english_load_string(packed) == count);
    for (i = 0; i < count; i++) { assert(english_glyph_pen[i] == i * 16 + 5); }
    return 0;
}
'''
        root = Path(self.folder.name)
        code, binary = root / 'renderer.c', root / 'renderer'
        code.write_text(fixture + functions + checks)
        subprocess.run(['cc', '-std=c89', '-Wall', '-Wextra', '-Werror', '-DTOKIMEMO_ENGLISH',
                        '-I' + str(root), '-I' + str(ROOT / 'include'), str(code),
                        str(ROOT / 'src/english/names.c'), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)


class NamePairsTests(unittest.TestCase):
    def test_existing_save_pair_assignments(self):
        # Persisted bytes must keep their meaning even if the shared table changes.
        self.assertEqual(names.unpack(bytes(range(0xB0, 0xE0))),
                         b'kakikukekosashsusesotachtstetonaninuneno'
                         b'hahifuhehomamimumemoyayuyorarirurerowagagigugegobabibube')

    def test_font_cannot_reuse_persistent_pair_code(self):
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / 'font.txt'
            path.write_text('glyph 0xB0 collision\n' + ('............\n' * 12))
            with self.assertRaisesRegex(SystemExit, 'name-pair range reserved'):
                english_font.parse(path)

    def test_ascii_pairs_round_trip(self):
        for pair in itertools.product(range(0x20, 0x7F), repeat=2):
            text = bytes(pair)
            self.assertEqual(names.unpack(names.pack(text)), text)

    def test_shift_jis_trail_is_not_a_token(self):
        for lead in (0x81, 0x82, 0xE0, 0xEF):
            for trail in range(0xB0, 0xE0):
                text = b'ka' + bytes((lead, trail)) + b'shi'
                self.assertEqual(names.unpack(names.pack(text)), text)

    def test_editing_one_letter_of_a_pair(self):
        text = b'Mikashiro'
        while text:
            packed = names.pack(text)
            text = names.unpack(packed)[:-1]
            self.assertEqual(names.unpack(names.pack(text)), text)

    def test_existing_codes_are_not_repacked(self):
        encoded = bytes(names.PAIRS)
        self.assertEqual(names.pack(encoded), encoded)

    def test_case_and_punctuation_are_preserved(self):
        text = b"KAka O'Neil-san"
        self.assertEqual(names.unpack(names.pack(text)), text)


if __name__ == '__main__':
    unittest.main()
