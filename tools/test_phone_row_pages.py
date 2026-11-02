"""T-39520: execute production direct-row batching and block closing in C89."""
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class PhoneRowPageTest(unittest.TestCase):
    def test_page_closing_reset_interleaving_and_width(self):
        source = (ROOT / 'src/english/script.c').read_text()
        begin = source.index('/* T-39520: direct sndi rows')
        batch = source[begin:source.index('static s32 english_speaker_draw(', begin)]
        begin = source.index('static void english_phone_wrap(')
        wrap = source[begin:source.index('void english_sndisp(', begin)]
        begin = source.index('void english_sndisp(')
        block = source[begin:source.index('\n}', begin) + 2]
        fixture = r'''
#include <assert.h>
#include <string.h>
typedef unsigned char u8;
typedef int s32;
typedef short s16;
#define ENGLISH_LINE_BYTES 160
#define ENGLISH_SCRIPT_BYTES 1024
#define ENGLISH_TEXT_X (-72)
#define ENGLISH_LINE_END 10
#define ENGLISH_CODE_PLAYER_FORM 16
#define ENGLISH_CODE_GIRL 21
static u8 english_quote[] = "\"";
static u8 english_close_paren[] = ")";
static s32 english_phone_mark_x;
static u8 text_list[52 * 8];
static struct { int count, total, pos, mark; } text_state;
static struct { int text_first_line, text_lines_total, text_scroll_state; } game_state;
static char drawn[52][160];
static int rows[52], started;
static int check_set_k(void) { return text_state.count; }
static void english_copy(u8 *dst, u8 *src, int cap) {
    strncpy((char *)dst, (char *)src, cap - 1); dst[cap - 1] = 0;
}
static void english_append(u8 *dst, u8 *src, int cap) {
    strncat((char *)dst, (char *)src, cap - strlen((char *)dst) - 1);
}
static int english_char_bytes(u8 *text) { (void)text; return 1; }
static int english_text_width(u8 *text) { return strlen((char *)text) * 6; }
static void english_script_from_original(u8 *dst, u8 *src, int cap) {
    english_copy(dst, src, cap);
}
static void k_disp_start(int first) { started = first; }
void english_sndisp(u8 *text, s32 first_row, s32 speaker_style);
void english_sndi(u8 *text, s32 row, s32 speaker_style);
'''
        render = r'''
void english_sndi(u8 *text, s32 row, s32 style) {
    int entry = text_state.count;
    english_phone_row_remember(text, row, style);
    assert(entry < 52);
    strcpy(drawn[entry], (char *)text);
    rows[entry] = row;
    text_list[entry * 8 + 5] = strlen((char *)text);
    text_state.count++;
    english_phone_rows_count = text_state.count;
}
static void fresh(void) {
    english_k_reset(0);
    text_state.mark = 1;
    text_state.count = 1;
    text_list[5] = 6;
    strcpy(drawn[0], "Family");
    english_phone_mark_x = -96;
}
int main(void) {
    int i;
    fresh();
    english_sndi((u8 *)"Hello,", 0, 15);
    english_sndi((u8 *)"friend.", 1, 15);
    english_k_sub_disp_start(0);
    assert(text_state.count == 3 && started == 1);
    assert(game_state.text_scroll_state == 0);
    assert(!strcmp(drawn[0], "Family"));
    assert(!strcmp(drawn[1], "Hello,"));
    assert(!strcmp(drawn[2], "friend.\""));
    english_k_sub_disp_start(0);
    assert(text_state.count == 3 && !strcmp(drawn[2], "friend.\""));
    fresh();
    english_sndi((u8 *)"Already closed.\"", 0, 15);
    english_k_sub_disp_start(0);
    assert(!strcmp(drawn[1], "Already closed.\""));
    fresh();
    english_sndi((u8 *)"A thought", 0, 31);
    english_k_sub_disp_start(0);
    assert(!strcmp(drawn[1], "A thought)"));
    fresh();
    english_sndi((u8 *)"Ring, ring...", 0, 47);
    english_k_sub_disp_start(0);
    assert(!strcmp(drawn[1], "Ring, ring..."));
    fresh();
    english_sndi((u8 *)"Stale", 0, 15);
    english_k_reset(1);
    text_state.count = 2;
    strcpy(drawn[1], "Other screen");
    english_k_sub_disp_start(0);
    assert(!strcmp(drawn[1], "Other screen"));
    fresh();
    english_sndi((u8 *)"First", 0, 15);
    text_state.count++;
    english_sndi((u8 *)"Interleaved", 1, 15);
    english_k_sub_disp_start(0);
    assert(!strcmp(drawn[3], "Interleaved"));
    fresh();
    english_sndi((u8 *)"We can meet at the neighborhood park", 0, 15);
    english_sndi((u8 *)"and walk around together.", 1, 15);
    english_k_sub_disp_start(0);
    assert(text_state.count <= 4);
    for (i = 1; i < text_state.count; i++) {
        assert(rows[i] < 3);
        assert(english_text_width((u8 *)drawn[i]) <= 214);
    }
    assert(drawn[text_state.count - 1][strlen(drawn[text_state.count - 1]) - 1] == '"');
    fresh();
    english_sndi((u8 *)"One two three four five six seven eight nine ten eleven twelve", 0, 15);
    english_sndi((u8 *)"More words in this reply need another line before the closing mark.", 1, 15);
    english_k_sub_disp_start(0);
    assert(text_state.count > 4);
    assert(game_state.text_scroll_state == 0x80);
    assert(drawn[text_state.count - 1][strlen(drawn[text_state.count - 1]) - 1] == '"');
    fresh();
    english_sndisp((u8 *)"Block path", 0, 15);
    english_k_sub_disp_start(0);
    assert(!strcmp(drawn[1], "Block path\""));
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as directory:
            code, binary = Path(directory) / 'phone.c', Path(directory) / 'phone'
            code.write_text(fixture + batch + wrap + block + render)
            subprocess.run(['cc', '-std=c89', '-Wall', '-Werror', '-Wno-pointer-sign',
                            str(code), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)


if __name__ == '__main__':
    unittest.main()
