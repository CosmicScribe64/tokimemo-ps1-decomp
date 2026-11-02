"""Execute the production English telephone wrapper with synthetic font metrics (T-39511).

Run in tokimemo-patch; the image provides cc. No game data is needed.
"""
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class PhoneWrapTest(unittest.TestCase):
    def test_dynamic_sentence_and_explicit_paragraphs(self):
        source = (ROOT / "src/english/script.c").read_text()
        function = source[source.index("static void english_phone_wrap("):source.index("void english_sndisp(")]
        fixture = r'''
#include <assert.h>
#include <string.h>
typedef unsigned char u8;
typedef int s32;
#define ENGLISH_TEXT_X (-72)
#define ENGLISH_LINE_END 10
#define ENGLISH_CODE_PLAYER_FORM 16
#define ENGLISH_CODE_GIRL 21
static u8 english_quote[] = "\"";
static s32 english_phone_mark_x = -96;
static s32 english_char_bytes(u8 *text) { return *text >= 0xE0 ? 2 : 1; }
static s32 english_text_width(u8 *text) {
    s32 width = 0;
    while (*text) { width += *text++ == ' ' ? 4 : 6; }
    return width;
}
'''
        checks = r'''
int main(void) {
    u8 sentence[] = "Okay, 4/16 at Neighborhood Park. Let's meet at the entrance. See you!";
    u8 original[sizeof(sentence)];
    u8 paragraphs[] = "Uh, yeah... Nothing...\n(A bit too friendly...)";
    u8 thought[] = "Uh, yeah... Nothing...\n(A bit too friendly...)";
    char *row;
    int i;
    strcpy((char *)original, (char *)sentence);
    english_phone_wrap(sentence, 15);
    assert(strchr((char *)sentence, '\n'));
    for (i = 0; sentence[i]; i++) {
        assert(sentence[i] == original[i] || (sentence[i] == '\n' && original[i] == ' '));
    }
    row = strtok((char *)sentence, "\n");
    while (row) {
        assert(english_text_width((u8 *)row) <= 208);
        row = strtok(0, "\n");
    }
    english_phone_wrap(paragraphs, 15);
    assert(!strcmp((char *)paragraphs, (char *)thought));
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as directory:
            code = Path(directory) / "phone.c"
            binary = Path(directory) / "phone"
            code.write_text(fixture + function + checks)
            subprocess.run(["cc", "-std=c89", "-Wall", "-Werror", str(code), "-o", str(binary)], check=True)
            subprocess.run([str(binary)], check=True)


if __name__ == "__main__":
    unittest.main()
