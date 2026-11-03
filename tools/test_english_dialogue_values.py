"""T-39542: run the production value setter against bounded state fields."""
import re
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class DialogueValueTests(unittest.TestCase):
    def test_complete_values_and_bounded_legacy_fields(self):
        source = (ROOT / 'src/english/script.c').read_text()
        definitions = '\n'.join(re.findall(r'^#define ENGLISH_.*$', source, re.M))
        arrays = '\n'.join(re.findall(r'^static u8 english_value_[ow]_long\[.*?;', source, re.M))
        functions = source[source.index('static s32 english_char_bytes('):
                           source.index('void english_girl_short_set(')]
        fixture = r'''
#include <assert.h>
#include <string.h>
typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;
#define ENGLISH_CODE_O 0x17
#define ENGLISH_CODE_W 0x18
struct { u8 before[4]; u8 o[8]; u8 w[12]; u8 after[4]; } fields;
#define name_code_o fields.o
#define name_code_w fields.w
void english_append(u8 *, u8 *, s32);
'''
        checks = r'''
int main(void) {
    u8 value[] = "A Long Fortune With A Closing Mark)";
    u8 long_value[96];
    u8 sjis[] = { 'A', 'B', 'C', 'D', 'E', 'F', 0x82, 0xA0, 0 };
    memset(&fields, 0x55, sizeof(fields));
    english_value_set(ENGLISH_CODE_O, value);
    assert(!strcmp((char *)english_long_name(name_code_o, english_value_o_long, 8), (char *)value));
    assert(!memcmp(name_code_o, "A Long ", 7) && name_code_o[7] == 0);
    assert(fields.w[0] == 0x55 && fields.before[3] == 0x55);
    english_value_set(ENGLISH_CODE_W, value);
    assert(!strcmp((char *)english_long_name(name_code_w, english_value_w_long, 12), (char *)value));
    assert(name_code_w[11] == 0 && fields.after[0] == 0x55);
    strcpy((char *)name_code_o, "Other");
    assert(english_long_name(name_code_o, english_value_o_long, 8) == name_code_o);
    english_value_set(ENGLISH_CODE_O, sjis);
    assert(name_code_o[6] == 0);
    assert(!memcmp(english_value_o_long, sjis, sizeof(sjis)));
    memset(long_value, 'X', sizeof(long_value)); long_value[95] = 0;
    english_value_set(ENGLISH_CODE_W, long_value);
    assert(strlen((char *)english_value_w_long) == sizeof(english_value_w_long) - 1);
    assert(fields.after[0] == 0x55 && fields.before[3] == 0x55);
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as directory:
            code = Path(directory) / 'values.c'
            binary = Path(directory) / 'values'
            code.write_text(fixture + definitions + '\n' + arrays + '\n' + functions + checks)
            subprocess.run(['cc', '-std=c89', '-Wall', '-Werror', '-Wno-pointer-sign',
                            str(code), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)


if __name__ == '__main__':
    unittest.main()
