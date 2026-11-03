"""T-39545: run the generated game's secret-name comparisons."""
import os
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


@unittest.skipUnless(os.environ.get('TOKIMEMO_ENGLISH_DISC') == '1',
                     'Requires freshly generated English sources')
class EnglishNameCodesTest(unittest.TestCase):
    def test_all_four_effects_and_nonmatching_names(self):
        source = (ROOT / 'build/en/src/ovl/name_entry/name_entry.c').read_text()
        start = source.index('void name_entry_cheat_names_apply(void) {')
        end = source.index('\n}\n', start) + 3
        function = source[start:end]
        fixture = r'''
#include <assert.h>
#include <string.h>
typedef int s32;
typedef struct { int value; } ParamValue;
typedef struct {
    int tokimeki, affection;
    union { unsigned char b[4]; struct { unsigned met:1; unsigned b3:1; } f; } flags;
} Girl;
static struct {
    ParamValue param_condition, other_parameters[8];
    Girl girls[13];
} game_state;
static char name_entry_family_name[8], name_entry_given_name[8], name_entry_nickname[8];
int english_name_pack(unsigned char *dst, unsigned char *src, int size);
static void names(char *family, char *given, char *nick) {
    assert(strlen(family) <= 7 && strlen(given) <= 7 && strlen(nick) <= 7);
    memset(&game_state, 0, sizeof(game_state));
    assert(english_name_pack((unsigned char *)name_entry_family_name, (unsigned char *)family, 8) >= 0);
    assert(english_name_pack((unsigned char *)name_entry_given_name, (unsigned char *)given, 8) >= 0);
    assert(english_name_pack((unsigned char *)name_entry_nickname, (unsigned char *)nick, 8) >= 0);
}
static void check_stats(int value) {
    int i;
    for (i = 0; i < 9; i++) assert((&game_state.param_condition)[i].value == value);
}
'''
        checks = r'''
int main(void) {
    int i;
    names("Smith", "John", "KONAMI");
    name_entry_cheat_names_apply();
    check_stats(573);
    names("UPUP", "DNDN", "LRLRX");
    for (i = 0; i < 9; i++) (&game_state.param_condition)[i].value = 123;
    name_entry_cheat_names_apply();
    check_stats(0);
    names("UU", "DD", "LRLRX");
    name_entry_cheat_names_apply();
    check_stats(999);
    names("ALL", "FRIENDS", "John");
    for (i = 1; i < 9; i++) game_state.girls[i].flags.b[2] = 0x10;
    game_state.girls[4].flags.b[2] = 0;
    name_entry_cheat_names_apply();
    assert(game_state.girls[0].tokimeki == 32 && game_state.girls[0].affection == 64);
    for (i = 1; i < 9; i++) {
        assert(game_state.girls[i].affection == (i == 4 ? 0 : 127));
        assert(game_state.girls[i].tokimeki == (i == 4 ? 0 : 127));
        assert(game_state.girls[i].flags.f.met == (i != 4));
        assert(game_state.girls[i].flags.f.b3 == (i != 4));
    }
    names("UU", "DNDN", "LRLRX");
    for (i = 0; i < 9; i++) (&game_state.param_condition)[i].value = 123;
    name_entry_cheat_names_apply();
    check_stats(123);
    assert(game_state.girls[0].affection == 0);
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as directory:
            code = Path(directory) / 'codes.c'
            binary = Path(directory) / 'codes'
            code.write_text(fixture + function + checks)
            (Path(directory) / 'common.h').write_text(
                'typedef unsigned char u8; typedef unsigned int u32; typedef int s32;\n')
            subprocess.run(['cc', '-std=c89', '-Wall', '-Werror',
                            '-DTOKIMEMO_ENGLISH', '-I' + directory,
                            '-I' + str(ROOT / 'include'), str(ROOT / 'src/english/names.c'), str(code),
                            '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)


if __name__ == '__main__':
    unittest.main()
