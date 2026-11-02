"""T-39515: execute the actual exam-row layout with maximum-width name metrics.

Run in Docker; no game data is needed. Covers both columns and persistent row
creation, keeping the complete name separate from three-digit rank and score.
"""
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class ExamLayoutTest(unittest.TestCase):
    def test_english_primitive_arena_uses_aligned_bytes(self):
        source = (ROOT / 'src/main/gpu_primitives.c').read_text()
        start = source.index('void SetWorkBase(')
        functions = source[start:source.index('/* Emits', start)]
        fixture = r'''
#include <assert.h>
typedef int s32;
struct Arena { int size, used, base; };
static struct { struct Arena prim_arena[2]; } game_state;
'''
        checks = r'''
int main(void) {
    int buffer, i, base, next, size, got;
    for (buffer = 0; buffer < 2; buffer++) {
        base = 0x100000 + buffer * 0x10C00;
        SetWorkBase(base, buffer);
        next = base;
        /* Full glyph capacity, foreground and shadow, plus scene polygons. */
        for (i = 0; i < 1400; i++) {
            size = i < 768 ? 16 : 39;
            got = GetWorkBase(size, buffer);
            assert(got == next);
            assert((got & 3) == 0);
            assert(got + size <= base + 0x10C00);
            next += (size + 3) / 4 * 4;
        }
        assert(game_state.prim_arena[buffer].used == next - base);
    }
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as tmp:
            code, binary = Path(tmp) / 'arena.c', Path(tmp) / 'arena'
            code.write_text(fixture + functions + checks)
            subprocess.run(['cc', '-std=c89', '-Wall', '-Werror',
                            '-DTOKIMEMO_ENGLISH', str(code), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)

    def test_full_names_and_numbers_fit_both_columns(self):
        source = (ROOT / 'src/ovl/misc_scenes/final_exam.c').read_text()
        start = source.index('static void exam_english_row(')
        function = source[start:source.index('\n#endif', start)]
        fixture = r'''
#include <assert.h>
#include <stdio.h>
#include <string.h>
typedef unsigned char u8;
typedef int s32;
static int exam_ranking_order[14], exam_ranking_rank[14], exam_ranking_value[14];
static int exam_row_y[14], exam_player_in_lower_half;
struct Entry { int x, y; char text[40]; } entries[3];
static int count;
static int english_text_width(u8 *text) {
    return !strcmp((char *)text, "FULL MAXIMUM NAME") ? 116 : strlen((char *)text) * 6;
}
static void set_kanji_string(int x, int y, int color, u8 *text, int slot) {
    (void)color; (void)slot;
    assert(count < 3);
    entries[count].x = x; entries[count].y = y;
    strcpy(entries[count++].text, (char *)text);
}
'''
        checks = r'''
int main(void) {
    int row, col;
    for (row = 0; row < 14; row++) {
        count = 0;
        col = row < 7 ? -160 : 0;
        exam_ranking_order[row] = 13;
        exam_ranking_rank[13] = 499;
        exam_ranking_value[13] = 500;
        exam_row_y[row] = -82 + (row % 7) * 16;
        exam_english_row(row, (u8 *)"FULL MAXIMUM NAME");
        assert(count == 3);
        assert(!strcmp(entries[0].text, "499"));
        assert(!strcmp(entries[1].text, "FULL MAXIMUM NAME"));
        assert(!strcmp(entries[2].text, "500"));
        assert(entries[0].x >= col + 2);
        assert(entries[0].x + 18 + 2 <= entries[1].x);
        assert(entries[1].x + 116 + 2 <= entries[2].x);
        assert(entries[2].x + 18 <= col + 158);
        assert(entries[0].y == exam_row_y[row] && entries[2].y == exam_row_y[row]);
        assert(exam_player_in_lower_half == (row >= 7));
    }
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as tmp:
            code, binary = Path(tmp) / 'exam.c', Path(tmp) / 'exam'
            code.write_text(fixture + function + checks)
            subprocess.run(['cc', '-std=c89', '-Wall', '-Werror', str(code), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)
