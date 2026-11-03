"""T-39543: execute the stage speaker helper against dialogue-frame bounds."""
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class StageSpeakerLayoutTest(unittest.TestCase):
    def test_long_names_leave_a_gap_inside_the_frame(self):
        source = (ROOT / 'src/ovl/culture_stage_shows/literature_speech_contest.c').read_text()
        helper = source[source.index('static s32 stage_show_script_text_x'):
                        source.index('#define STAGE_SHOW_TEXT_X')]
        fixture = r'''
#include <assert.h>
typedef long s32;
typedef unsigned char u8;
static s32 width, drawn_x, drawn_color;
s32 english_text_width(u8 *text) { return width; }
void set_kanji_string(s32 x, s32 y, s32 color, s32 text, s32 slot) {
    drawn_x = x; drawn_color = color;
}
'''
        checks = r'''
int main(void) {
    for (width = 1; width <= 70; width++) {
        stage_show_script_speaker(-120, 48, 0, 0, 0);
        assert(drawn_x >= -128);
        assert(drawn_color == 1);
        assert(stage_show_script_text_x - 14 - (drawn_x + width) >= 8);
        assert(stage_show_script_text_x + 143 <= 124);
    }
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as directory:
            code = Path(directory) / 'stage.c'
            binary = Path(directory) / 'stage'
            code.write_text(fixture + helper + checks)
            subprocess.run(['cc', '-std=c89', '-Wall', '-Werror', str(code), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)


if __name__ == '__main__':
    unittest.main()
