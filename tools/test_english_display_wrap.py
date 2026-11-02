"""T-39517: execute the production expanded-text renderer in a C89 harness."""
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


def function(source, signature):
    start = source.index(signature)
    return source[start:source.index('\n}', start) + 2] + '\n'


class DisplayWrapTest(unittest.TestCase):
    def test_expanded_values_scroll_without_clipping_or_losing_text(self):
        source = (ROOT / 'src/english/script.c').read_text()
        chars = function(source, 'static s32 english_char_bytes(')
        boundary = function(source, 'static s32 english_display_line_end(')
        renderer = function(source, 's32 english_talk_render(')
        fixture = r'''
#include <assert.h>
#include <string.h>
typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;
typedef short s16;
#define ENGLISH_SCRIPT_BYTES 1024
#define ENGLISH_LINE_BYTES 160
#define ENGLISH_LINE_END 10
#define ENGLISH_OPTION_END 11
static int D_80122D1C, talk_next_row_y, count;
static u8 english_open_paren[] = "(";
struct Entry { int x, y; char text[160]; } entries[20];
'''
        stubs = r'''
static int english_text_width(u8 *text) {
    int width = 0;
    while (*text) {
        int bytes = english_char_bytes(text);
        width += bytes == 2 ? 14 : *text >= 0xB0 && *text <= 0xDF ? 12 : 6;
        text += bytes;
    }
    return width;
}
static void english_script_from_original(u8 *out, u8 *in, int size) {
    assert(strlen((char *)in) < (unsigned)size); strcpy((char *)out, (char *)in);
}
static int english_punctuation_only(u8 *text) { (void)text; return 0; }
static void english_names_expand(u8 *out, u8 *in) {
    const char *career = "The second-tier company";
    while (*in) {
        if (*in == 0x1A) { strcpy((char *)out, career); out += strlen(career); }
        else *out++ = *in;
        in++;
    }
    *out = 0;
}
static void english_append(u8 *out, u8 *tail, int size) {
    assert(strlen((char *)out) + strlen((char *)tail) < (unsigned)size);
    strcat((char *)out, (char *)tail);
}
static int english_right_aligned(u8 *text, int right) {
    return right - english_text_width(text);
}
static int set_kanji_string(int x, int y, int color, u8 *text, int slot) {
    (void)color; (void)slot; assert(count < 20);
    entries[count].x=x; entries[count].y=y;
    strcpy(entries[count].text,(char *)text);
    return ++count;
}
'''
        checks = r'''
int main(void) {
    int i, result;
    char joined[512], word[70], long_script[800];
    u8 pair[] = {0x81, 0x40, 0x81, 0x41, 0};
    u8 packed[] = {0xB0, 0xB1, 0};
    u8 sentence[] = "Unfortunately, \032 was not an option.\nBut I will try again.";
    count=0; joined[0]=0;
    result=english_talk_render(-72,72,0,sentence,0,(u8 *)"\"",(u8 *)"",0);
    assert(count >= 3 && result == count);
    for (i=0;i<count;i++) {
        assert(entries[i].x + english_text_width((u8 *)entries[i].text) <= 124);
        assert(entries[i].y == 72+i*16);
        if (i) strcat(joined," ");
        strcat(joined,entries[i].text);
    }
    assert(!strcmp(joined,"Unfortunately, The second-tier company was not an option. But I will try again.\""));
    assert(talk_next_row_y == 72+count*16);
    assert(strchr((char *)sentence,0x1A));
    assert(english_display_line_end(pair,20)==2);
    assert(english_display_line_end(packed,18)==1);
    memset(word,'W',60);word[60]=0;count=0;joined[0]=0;
    english_talk_render(-72,0,0,(u8 *)word,0,(u8 *)"",(u8 *)"",0);
    for(i=0;i<count;i++) strcat(joined,entries[i].text);
    assert(!strcmp(word,joined) && count==2);
    count=0;
    english_talk_render(-72,0,0,(u8 *)word,0,(u8 *)"",(u8 *)"Label",1);
    assert(count==2 && !strcmp(entries[1].text,word));
    count=0;
    english_talk_render(-72,0,0,(u8 *)"One\n\nThree",0,(u8 *)"",(u8 *)"",0);
    assert(count==3 && entries[1].text[0]==0 && entries[2].y==32);
    long_script[0]=0;
    for(i=0;i<19;i++) strcat(long_script,"This is a complete summary row.\n");
    strcat(long_script,"END");
    assert(strlen(long_script)>512);
    count=0;
    english_talk_render(-72,0,0,(u8 *)long_script,0,(u8 *)"",(u8 *)"",0);
    assert(count==20 && !strcmp(entries[19].text,"END"));
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as tmp:
            code, binary = Path(tmp)/'wrap.c', Path(tmp)/'wrap'
            code.write_text(fixture + chars + stubs + boundary + renderer + checks)
            subprocess.run(['cc', '-std=c89', '-Wall', '-Werror', str(code), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)
