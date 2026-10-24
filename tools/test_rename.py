#!/usr/bin/env python3
"""Tests of tools/rename.py and tools/symtab.py on synthetic trees (T-27002). No game data.

Run (Docker): python3 -m unittest discover -s tools -p test_rename.py
The gcc oracle tests need the native gcc of the image and skip without it.
"""
import io
import os
import shutil
import sys
import tempfile
import textwrap
import unittest
import unittest.mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rename as R  # noqa: E402
import srcscan  # noqa: E402
import symtab  # noqa: E402

HAVE_GCC = shutil.which("gcc") is not None
try:
    import pycparser  # noqa: F401
    HAVE_PYCPARSER = True
except ImportError:
    HAVE_PYCPARSER = False

FILES = {
    "config/overlays.txt": "# name load_address text_size\nAAA 0x80132000 0x200\nEVT 0x800F6000 0x100\n",
    "config/SLPM_86.053.yaml": """\
        options:
          symbol_addrs_path:
            - config/symbol_addrs.txt
            - config/symbol_addrs_main.txt
        segments:
          - name: main
            type: code
            start: 0x800
            vram: 0x80041000
            subsegments:
              - { start: 0x800, type: c, name: main/80041000 }
              - { start: 0x900, type: c, name: main/80041100 }
        """,
    "config/overlays/AAA.yaml": """\
        options:
          symbol_addrs_path:
            - config/symbol_addrs.txt
        segments:
          - name: AAA
            type: code
            start: 0x0
            vram: 0x80132000
            subsegments:
              - [0x0, c, AAA/80132000]
              - [0x100, c, AAA/80132100]
              - [0x200, .rodata, AAA/80132100]
              - [0x300, data, AAA_data]
          - [0x30000]
        """,
    "config/overlays/EVT.yaml": """\
        options:
          symbol_addrs_path:
            - config/symbol_addrs.txt
        segments:
          - name: EVT
            type: code
            start: 0x0
            vram: 0x800F6000
            subsegments:
              - [0x0, c, EVT/800F6000]
          - [0x30000]
        """,
    "config/symbol_addrs.txt": "// shared main names\nknown_func = 0x80041100; // type:func\nResetPad = 0x80041300; // type:func\n",
    "config/symbol_addrs_main.txt": "// main only\nfunc_80041180 = 0x80041180; // type:func size:0x20\n",
    "config/symbol_addrs_types.txt": "D_800E6280 = 0x800E6280; // GameState base\n",
    "config/proto_known.txt": "# findings\nvoid-result AAA:func_80132000\nvoid-result main:func_80041000\n",
    "config/migrate_globals.txt": "# aggregates\naggregate D_800E6280 GameState include/game.h\n",
    "config/toolchains.txt": "# objects\nAAA/80132100 ido 5.3\n",
    "config/objects/AAA.txt": "# objects\nobject 80132000 80132100 - - no x -\nobject 80132100 80132200 80132200 80132300 yes x x\n",
    "include/common.h": """\
        #ifndef COMMON_H
        #define COMMON_H
        typedef int s32;
        typedef short s16;
        typedef unsigned char u8;
        #define INCLUDE_ASM(FOLDER, NAME)
        #define INCLUDE_RODATA(FOLDER, NAME)
        #endif
        """,
    "include/game.h": """\
        #ifndef GAME_H
        #define GAME_H
        #include "common.h"
        #include "main_api.h"
        typedef struct GameState {
            /* 0x00 */ s16 unk_00;
            /* 0x02 */ s16 unk_02;
        } GameState; /* size 0x04 */
        typedef struct Rec38 {
            /* 0x00 */ s16 unk_00;
        } Rec38;
        #define GS_MODE(g) ((g).unk_00)
        extern GameState D_800E6280;
        #endif
        """,
    "include/main_api.h": """\
        #ifndef MAIN_API_H
        #define MAIN_API_H
        #ifndef MAIN_API_OVERRIDE_func_80041000
        void func_80041000(void);
        #endif
        void known_func(s32 arg0);
        void func_80041180(s32 count, s32 value);
        extern s32 D_800F6100;
        #endif
        """,
    "include/ovl/AAA.h": """\
        #ifndef OVL_AAA_H
        #define OVL_AAA_H
        #include "game.h"
        void func_80132000(void);
        extern s32 D_80132100;
        #endif /* OVL_AAA_H */
        """,
    "include/ovl/EVT.h": """\
        #ifndef OVL_EVT_H
        #define OVL_EVT_H
        #include "game.h"
        #endif
        """,
    "src/main/80041000.c": """\
        #include "game.h"

        /* func_80041000 calls known_func; see AAA:func_80132000 and EVT:D_800F6100.
         * D_800E6280.unk_00 is read first. */
        void func_80041000(void) {
            s32 index;
            index = D_800E6280.unk_00;
            GS_MODE(D_800E6280) = 2;
            known_func(index);
            D_800F6100 = 1;
        }
        """,
    "src/main/80041100.c": """\
        #include "game.h"

        void known_func(s32 arg0) {
            D_800E6280.unk_02 = arg0;
        }

        void func_80041180(count, value)
            s32 count;
            s32 value;
        {
            Rec38 r;
            r.unk_00 = count + value;
            func_80041000();
        }
        """,
    "src/ovl/AAA/80132000.c": """\
        #define MAIN_API_OVERRIDE_func_80041000 /* s32 argument view */
        #include "ovl/AAA.h"

        void func_80041000(s32 x);

        void func_80132000(void) {
            func_80041000(1);
            D_80132100 = D_800E6280.unk_02;
            if (D_800E6280.unk_02 == 2) {
                known_func((s32) "func_80132000");
            }
            switch (D_800E6280.unk_02) {
            case 0:
                break;
            case 5:
                break;
            }
        }
        """,
    "src/ovl/AAA/80132100.c": """\
        #include "ovl/AAA.h"

        INCLUDE_RODATA("asm/ovl/AAA/data/AAA/80132100.rodata", D_80132150);

        s32 D_80132100 = 0;

        void func_80132100(void) {
            func_80132000();
        }
        INCLUDE_ASM("asm/ovl/AAA/nonmatchings/AAA/80132100", func_80132180);
        """,
    "src/ovl/EVT/800F6000.c": """\
        #include "ovl/EVT.h"

        void func_800F6000(void) {
            D_800F6100 = 2; /* EVT's own D_800F6100 */
        }
        """,
}


class Fixture(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.mkdtemp(prefix="test-rename-")
        self.root = self.tmp
        for path, text in FILES.items():
            self.write(path, textwrap.dedent(text))
        srcscan._DIR_UNITS.clear()
        srcscan._OVERLAYS.clear()
        srcscan._HEADER_UNITS.clear()

    def tearDown(self):
        shutil.rmtree(self.tmp, ignore_errors=True)

    def write(self, path, text):
        full = os.path.join(self.root, path)
        os.makedirs(os.path.dirname(full), exist_ok=True)
        with open(full, "w") as f:
            f.write(text)

    def read(self, path):
        full = os.path.join(self.root, path)
        if not os.path.exists(full):
            return None
        with open(full) as f:
            return f.read()

    def table(self, text, name="t.txt"):
        self.write("tables/" + name, textwrap.dedent(text))
        return os.path.join(self.root, "tables", name)

    def run_plan(self, rows, apply=False, verify=False, **kw):
        out = io.StringIO()
        rc = R.plan_cmd(self.root, self.table(rows), apply=apply, verify=verify, out=out, **kw)
        return rc, out.getvalue()

    def ok(self, rows, **kw):
        rc, out = self.run_plan(rows, apply=True, **kw)
        self.assertEqual(rc, 0, out)
        return out

    def refused(self, rows, why, **kw):
        rc, out = self.run_plan(rows, **kw)
        self.assertEqual(rc, 1, out)
        self.assertIn(why, out)
        return out


class TableTest(Fixture):
    def test_rows_and_notes(self):
        rows = R.parse_rows("# c\nfunc main func_80041000 boot T-1 entry # tail\n\n", "t")
        self.assertEqual((rows[0].kind, rows[0].scope, rows[0].old, rows[0].new, rows[0].note),
                         ("func", "main", "func_80041000", "boot", "T-1 entry"))
        self.assertFalse(rows[0].hypothesis)
        self.assertTrue(R.parse_rows("data main D_80041000 x HYPOTHESIS: y\n", "t")[0].hypothesis)
        with self.assertRaises(R.RenameError):
            R.parse_rows("func main only_three\n", "t")
        with self.assertRaises(R.RenameError):
            R.parse_rows("rename main a b\n", "t")

    def test_chain_lint(self):
        rows = R.parse_rows("func main a b 0x80041000\nfunc main b c 0x80041000\n", "l", ledger=True)
        self.assertEqual(R.chains(rows)[("sym", "main", "0x80041000", 0)][0], ["a", "b", "c"])
        with self.assertRaises(R.RenameError):
            R.chains(R.parse_rows("func main a b 0x80041000\nfunc main a c 0x80041000\n", "l", ledger=True))
        with self.assertRaises(R.RenameError):
            R.chains(R.parse_rows("func main a b 0x80041000\nfunc main b a 0x80041000\n", "l", ledger=True))

    def test_lexer_and_rewrite(self):
        text = 'f(func_X); /* func_X and AAA:func_X and EVT:func_X */ "func_X" s.func_X MAIN_API_OVERRIDE_func_X x_func_X'
        new, n_id, n_c = R.rewrite_text(text, "func_X", "go", accept=lambda q: q in (None, "AAA"))
        self.assertEqual(new, 'f(go); /* go and AAA:go and EVT:func_X */ "func_X" s.func_X MAIN_API_OVERRIDE_go x_func_X')
        self.assertEqual((n_id, n_c), (2, 2))
        self.assertEqual(R.guard_suffix("W9_TT_func_X", "func_X"), "W9_TT_")
        self.assertIsNone(R.guard_suffix("my_func_X", "func_X"))
        self.assertEqual(R.tokens("0x8004284C"), [("n", 0, 10)])


class SymbolRenameTest(Fixture):
    def test_overlay_function(self):
        out = self.ok("func AAA func_80132000 aaa_start T-1 entry of AAA\n")
        self.assertIn("aaa_start = 0x80132000; // type:func", self.read("config/symbols_AAA.txt"))
        self.assertIn("config/symbols_AAA.txt", self.read("config/overlays/AAA.yaml"))
        self.assertNotIn("config/symbols_AAA.txt", self.read("config/overlays/EVT.yaml"))
        self.assertIn("void aaa_start(void);", self.read("include/ovl/AAA.h"))
        self.assertIn("void aaa_start(void) {", self.read("src/ovl/AAA/80132000.c"))
        self.assertIn('known_func((s32) "func_80132000")', self.read("src/ovl/AAA/80132000.c"))
        self.assertIn("    aaa_start();", self.read("src/ovl/AAA/80132100.c"))
        self.assertIn("see AAA:aaa_start and", self.read("src/main/80041000.c"))
        self.assertIn("void-result AAA:aaa_start", self.read("config/proto_known.txt"))
        self.assertIn("string mentions func_80132000", out)
        self.assertIn("func AAA func_80132000 aaa_start 0x80132000 T-1 entry of AAA", self.read("config/renames/AAA.txt"))

    def test_main_symbol_inside_event_range(self):
        self.ok("data main D_800F6100 event_flag T-1\n")
        self.assertIn("event_flag = 0x800F6100;", self.read("config/symbol_addrs_main.txt"))
        self.assertNotIn("event_flag", self.read("config/symbol_addrs.txt"))
        self.assertIn("extern s32 event_flag;", self.read("include/main_api.h"))
        self.assertIn("    event_flag = 1;", self.read("src/main/80041000.c"))
        self.assertIn("EVT:D_800F6100", self.read("src/main/80041000.c"))
        self.assertEqual(self.read("src/ovl/EVT/800F6000.c"), textwrap.dedent(FILES["src/ovl/EVT/800F6000.c"]))

    def test_guard_macro_suffix_and_config_keys(self):
        self.ok("func main func_80041000 game_boot T-1\n")
        api = self.read("include/main_api.h")
        self.assertIn("#ifndef MAIN_API_OVERRIDE_game_boot\nvoid game_boot(void);", api)
        aaa = self.read("src/ovl/AAA/80132000.c")
        self.assertIn("#define MAIN_API_OVERRIDE_game_boot", aaa)
        self.assertIn("void game_boot(s32 x);", aaa)
        self.assertIn("/* game_boot calls known_func", self.read("src/main/80041000.c"))
        self.assertIn("void-result main:game_boot", self.read("config/proto_known.txt"))
        self.assertIn("game_boot = 0x80041000; // type:func", self.read("config/symbol_addrs.txt"))

    def test_pin_renamed_in_place(self):
        self.ok("func main func_80041180 text_merge T-1\n")
        self.assertIn("text_merge = 0x80041180; // type:func size:0x20", self.read("config/symbol_addrs_main.txt"))
        self.assertNotIn("text_merge", self.read("config/symbol_addrs.txt"))

    def test_types_file_line_renamed_and_made_visible(self):
        self.ok("data main D_800E6280 game_state T-1\n")
        self.assertIn("game_state = 0x800E6280; // GameState base", self.read("config/symbol_addrs_types.txt"))
        self.assertIn("game_state = 0x800E6280;", self.read("config/symbol_addrs.txt"))
        self.assertIn("aggregate game_state GameState include/game.h", self.read("config/migrate_globals.txt"))
        self.assertIn("game_state.unk_02 = arg0;", self.read("src/main/80041100.c"))

    def test_hypotheses_files(self):
        self.ok("data AAA D_80132100 aaa_counter HYPOTHESIS: counts something\n")
        text = self.read("config/symbols_AAA_hypotheses.txt")
        self.assertIn("HYPOTHESES", text.splitlines()[0])
        self.assertIn("aaa_counter = 0x80132100;", text)
        self.ok("func main known_func pad_poll HYPOTHESIS: reads the pad\n", ledger="main")
        self.assertNotIn("pad_poll", self.read("config/symbol_addrs.txt"))
        self.assertNotIn("known_func", self.read("config/symbol_addrs.txt"))
        self.assertIn("pad_poll = 0x80041100; // type:func", self.read("config/symbol_addrs_hypotheses.txt"))
        for y in ("config/SLPM_86.053.yaml", "config/overlays/AAA.yaml", "config/overlays/EVT.yaml"):
            self.assertIn("config/symbol_addrs_hypotheses.txt", self.read(y))
        self.assertEqual(symtab.SymbolTable(self.root).problems(), [])

    def test_collisions(self):
        self.refused("func AAA func_80132000 int\n", "C keyword")
        self.refused("func AAA func_80132000 func_80132004\n", "placeholder")
        self.refused("func AAA func_80132000 start_80132000\n", "holds an address")
        self.refused("func AAA func_80132000 AaaStart\n", "snake_case")
        self.refused("func AAA func_80132000 _start\n", "reserved")
        self.refused("func AAA func_80132000 memcpy\n", "C library")
        self.refused("func main func_80041000 count\n", "already an identifier")
        self.refused("func main func_80041000 index\n", "C library")
        self.refused("func main func_80041000 known_func\n", "already names 0x80041100")
        self.refused("func main func_80041000 resetpad\n", "only in case")
        self.refused("func AAA func_80041000 x_boot\n", "main-exe symbol")
        self.refused("func AAA D_80132100 x_boot\n", "data; use kind data")
        self.refused("data AAA func_80132000 x_boot\n", "function; use kind func")
        self.refused("func AAA func_80133000 x_boot\nfunc AAA func_80133000 y_boot\n", "is named x_boot already")
        self.refused("func AAA aaa_nothing x_boot\n", "neither a placeholder")
        self.ok("func AAA func_80132000 aaa_start\n")
        self.refused("func AAA func_80132100 aaa_start\n", "already names 0x80132000")
        self.refused("func AAA func_80132000 aaa_other\n", "is named aaa_start already")

    def test_alias_needs_flag(self):
        self.write("config/symbol_addrs_main.txt", "boot_view = 0x80041000; // type:func\n")
        self.refused("func main func_80041000 boot_view\n", "already names 0x80041000")


@unittest.skipUnless(HAVE_GCC, "needs the native gcc of the Docker image")
class OracleTest(Fixture):
    def test_field_uses_found_by_gcc(self):
        out = self.ok("field GameState unk_00 mode HYPOTHESIS: the game mode\n", verify=True)
        self.assertIn("2 uses located by gcc", out)
        game = self.read("include/game.h")
        self.assertIn("/* 0x00 */ s16 mode; /* HYPOTHESIS: the game mode */", game)
        self.assertIn("#define GS_MODE(g) ((g).mode)", game)
        self.assertIn("    /* 0x00 */ s16 unk_00;\n} Rec38;", game)
        main = self.read("src/main/80041000.c")
        self.assertIn("index = D_800E6280.mode;", main)
        self.assertIn("D_800E6280.mode is read first", main)
        self.assertIn("r.unk_00 = count + value;", self.read("src/main/80041100.c"))

    def test_two_uses_on_one_line(self):
        self.write("src/main/80041100.c", self.read("src/main/80041100.c").replace(
            "D_800E6280.unk_02 = arg0;", "D_800E6280.unk_02 = arg0 + D_800E6280.unk_02 + D_800E6280.unk_02;"))
        out = self.ok("field GameState unk_02 step\n", verify=True)
        self.assertIn("D_800E6280.step = arg0 + D_800E6280.step + D_800E6280.step;", self.read("src/main/80041100.c"))
        self.assertNotIn("PROBLEM", out)

    def test_field_member_clash(self):
        self.refused("field GameState unk_00 unk_02x\n", "placeholder")
        self.refused("field GameState unk_00 unk_02\n", "placeholder")
        self.write("include/game.h", self.read("include/game.h").replace("s16 unk_02;", "s16 mode;"))
        self.refused("field GameState unk_00 mode\n", "already has a member mode")

    def test_full_proofs_on_symbols(self):
        out = self.ok("func main func_80041000 game_boot T-1\ndata AAA D_80132100 aaa_counter T-1\n", verify=True)
        self.assertNotIn("PROBLEM", out)


class TypeLocalTest(Fixture):
    def test_type(self):
        self.ok("type main Rec38 GirlRecord T-1\n")
        game = self.read("include/game.h")
        self.assertIn("typedef struct GirlRecord {", game)
        self.assertIn("} GirlRecord;", game)
        self.assertIn("    GirlRecord r;", self.read("src/main/80041100.c"))
        self.ok("type main GameState GameStatus T-1\n", ledger="main")
        self.assertIn("aggregate D_800E6280 GameStatus include/game.h", self.read("config/migrate_globals.txt"))
        self.refused("type main GameStatus s32\n", "PascalCase")
        self.refused("type main GameStatus GirlRecord\n", "used before in the ledger")

    def test_local_and_param(self):
        self.ok("local main:func_80041000 index mode_value T-1\n")
        main = self.read("src/main/80041000.c")
        self.assertIn("s32 mode_value;", main)
        self.assertIn("known_func(mode_value);", main)
        self.ok("param main:func_80041180 count total T-1\n", ledger="main")
        f = self.read("src/main/80041100.c")
        self.assertIn("void func_80041180(total, value)\n    s32 total;", f)
        self.assertIn("r.unk_00 = total + value;", f)
        self.assertIn("void func_80041180(s32 total, s32 value);", self.read("include/main_api.h"))
        self.refused("local main:func_80041180 value r\n", "use kind param")
        self.refused("param main:func_80041180 value r\n", "already occurs")
        self.refused("local main:func_80041000 mode_value u8\n", "file-scope name")
        self.refused("local main:func_8004FFFF a b\n", "no definition")

    def test_local_scope_follows_function_rename(self):
        self.ok("func main func_80041000 game_boot\nlocal main:func_80041000 index mode_value\n")
        self.assertIn("s32 mode_value;", self.read("src/main/80041000.c"))


class FileRenameTest(Fixture):
    def test_file(self):
        out = self.ok("file AAA 80132100 aaa_tables T-1 the tables of AAA\n")
        y = self.read("config/overlays/AAA.yaml")
        self.assertIn("[0x100, c, AAA/aaa_tables]", y)
        self.assertIn("[0x200, .rodata, AAA/aaa_tables]", y)
        self.assertIn("[0x300, data, AAA_data]", y)
        self.assertIsNone(self.read("src/ovl/AAA/80132100.c"))
        c = self.read("src/ovl/AAA/aaa_tables.c")
        self.assertTrue(c.startswith("/* aaa_tables.c: the tables of AAA. Original object 0x80132100-0x80132200 "
                                     "(config/objects/AAA.txt). */\n"), c)
        self.assertIn('INCLUDE_RODATA("asm/ovl/AAA/data/AAA/aaa_tables.rodata", D_80132150);', c)
        self.assertIn('INCLUDE_ASM("asm/ovl/AAA/nonmatchings/AAA/aaa_tables", func_80132180);', c)
        self.assertIn("AAA/aaa_tables ido 5.3", self.read("config/toolchains.txt"))
        self.assertIn("move src/ovl/AAA/80132100.c -> src/ovl/AAA/aaa_tables.c", out)
        self.assertIn("file AAA 80132100 aaa_tables 0x80132100", self.read("config/renames/AAA.txt"))
        self.assertEqual(srcscan.object_name("AAA", 0x80132100, self.root), "AAA/aaa_tables")

    def test_file_rules(self):
        self.refused("file AAA 80132100 aaa_tables\nfunc AAA func_80132000 aaa_start\n", "table of their own")
        self.refused("file AAA 80132100 Tables\n", "snake_case")
        self.refused("file AAA 80132100 tables_80132100\n", "holds an address")
        self.refused("file AAA 80139999 x\n", "0 c subsegments")
        self.write("src/ovl/AAA/taken.c", "")
        self.refused("file AAA 80132100 taken\n", "exists already")

    def test_dir(self):
        self.ok("dir AAA AAA alpha_event T-1\n")
        y = self.read("config/overlays/AAA.yaml")
        self.assertIn("[0x0, c, alpha_event/80132000]", y)
        self.assertIn("[0x200, .rodata, alpha_event/80132100]", y)
        c = self.read("src/ovl/alpha_event/80132100.c")
        self.assertIn('"asm/ovl/AAA/data/alpha_event/80132100.rodata"', c)
        self.assertIn('"asm/ovl/AAA/nonmatchings/alpha_event/80132100"', c)
        self.assertIn("alpha_event/80132100 ido 5.3", self.read("config/toolchains.txt"))
        self.assertEqual(srcscan.unit_dir("AAA", self.root), "alpha_event")
        srcscan._DIR_UNITS.clear()
        self.assertEqual(srcscan.unit_of_source(os.path.join(self.root, "src/ovl/alpha_event/80132100.c")), "AAA")
        self.assertEqual(srcscan.asm_dirs_of_source(os.path.join(self.root, "src/ovl/alpha_event/x.c"))[1],
                         "asm/ovl/AAA/nonmatchings/alpha_event/x")

    def test_header(self):
        self.ok("file include ovl/AAA.h ovl/alpha.h T-1\n")
        self.assertIsNone(self.read("include/ovl/AAA.h"))
        h = self.read("include/ovl/alpha.h")
        self.assertIn("#ifndef OVL_ALPHA_H\n#define OVL_ALPHA_H", h)
        self.assertIn("#endif /* OVL_ALPHA_H */", h)
        self.assertIn('#include "ovl/alpha.h"', self.read("src/ovl/AAA/80132000.c"))
        self.assertIn('#include "ovl/alpha.h"', self.read("src/ovl/AAA/80132100.c"))
        self.assertEqual(srcscan.unit_header("AAA", self.root), "ovl/alpha.h")
        self.refused("file include game.h world.h\n", "by name")
        self.refused("file include ovl/EVT.h evt.h\n", "stays in include/ovl/")


class LedgerTest(Fixture):
    def test_check_replay_lookup_export(self):
        self.ok("func AAA func_80132000 aaa_start T-1\n")
        out = io.StringIO()
        self.assertEqual(R.check(self.root, out=out), 0, out.getvalue())
        # a branch brings the old name back
        self.write("src/ovl/AAA/80132100.c", self.read("src/ovl/AAA/80132100.c") + "void again(void) { func_80132000(); }\n")
        out = io.StringIO()
        self.assertGreater(R.check(self.root, out=out), 0)
        self.assertIn("func_80132000 was renamed to aaa_start", out.getvalue())
        self.assertEqual(R.replay(self.root, out=io.StringIO(), use_gcc=False), 0)
        self.assertIn("void again(void) { aaa_start(); }", self.read("src/ovl/AAA/80132100.c"))
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)
        before = {p: self.read(p) for p in ("src/ovl/AAA/80132100.c", "include/ovl/AAA.h", "config/symbols_AAA.txt")}
        out = io.StringIO()
        self.assertEqual(R.replay(self.root, out=out, use_gcc=False), 0)
        self.assertIn("0 files changed", out.getvalue())
        self.assertEqual(before, {p: self.read(p) for p in before})
        out = io.StringIO()
        R.lookup(self.root, "func_80132000", out=out)
        self.assertIn("func_80132000 -> aaa_start (current: aaa_start)", out.getvalue())
        out = io.StringIO()
        R.export(self.root, out=out)
        self.assertEqual(out.getvalue(), "func\tAAA\t0x80132000\tfunc_80132000\taaa_start\n")

    def test_replay_chain_across_ledgers(self):
        self.write("config/renames/a.txt", "func AAA func_80132000 aaa_first 0x80132000\n")
        self.write("config/renames/b.txt", "func AAA aaa_first aaa_last 0x80132000\n")
        self.assertEqual(R.replay(self.root, out=io.StringIO(), use_gcc=False), 0)
        self.assertIn("void aaa_last(void) {", self.read("src/ovl/AAA/80132000.c"))
        self.assertIn("aaa_last = 0x80132000;", self.read("config/symbols_AAA.txt"))
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)
        # a branch still uses the middle name of the chain
        self.write("src/ovl/AAA/80132100.c", self.read("src/ovl/AAA/80132100.c") + "void b(void) { aaa_first(); }\n")
        self.assertEqual(R.replay(self.root, out=io.StringIO(), use_gcc=False), 0)
        self.assertIn("void b(void) { aaa_last(); }", self.read("src/ovl/AAA/80132100.c"))

    def test_replay_symbol_line_dedupe(self):
        self.ok("func main func_80041180 text_merge\n")
        self.write("config/symbol_addrs_main.txt", self.read("config/symbol_addrs_main.txt")
                   + "func_80041180 = 0x80041180; // type:func size:0x20\n")
        self.assertGreater(R.check(self.root, out=io.StringIO()), 0)
        R.replay(self.root, out=io.StringIO(), use_gcc=False)
        self.assertEqual(self.read("config/symbol_addrs_main.txt").count("text_merge"), 1)
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)

    def test_check_other_kinds(self):
        self.ok("local main:func_80041000 index mode_value\n")
        self.ok("file include ovl/AAA.h ovl/alpha.h\n", ledger="main")
        self.ok("type main Rec38 GirlRecord\n", ledger="main")
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)
        self.write("src/main/80041000.c", self.read("src/main/80041000.c").replace("s32 mode_value;", "s32 mode_value, index;"))
        self.write("src/ovl/AAA/80132000.c", self.read("src/ovl/AAA/80132000.c").replace('"ovl/alpha.h"', '"ovl/AAA.h"'))
        self.write("include/ovl/EVT.h", self.read("include/ovl/EVT.h").replace("#endif", "typedef Rec38 Other;\n#endif"))
        out = io.StringIO()
        self.assertEqual(R.check(self.root, out=out), 3, out.getvalue())
        self.refused("type main GirlRecord Rec38\n", "already an identifier")
        self.write("include/ovl/EVT.h", self.read("include/ovl/EVT.h").replace("typedef Rec38 Other;\n", ""))
        self.refused("type main GameState GirlRecord2\ntype main GirlRecord2 Rec38\n", "used before in the ledger")

    def test_config_keys_scoped_in_check(self):
        self.write("config/proto_known.txt", "void-result EVT:func_80132000\n")
        self.ok("func AAA func_80132000 aaa_start\n")
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)
        self.assertIn("EVT:func_80132000", self.read("config/proto_known.txt"))

    def test_round_trip_catches_impure_edit(self):
        ctx = R.Context(self.root)
        row = R.Row("func", "AAA", "func_80132000", "aaa_start", where="t:1")
        before = {"src/ovl/AAA/80132000.c": "void func_80132000(void) { x = 1; }\n"}
        ctx.tree.texts["src/ovl/AAA/80132000.c"] = "void aaa_start(void) { x = 2; }\n"
        R.round_trip(ctx, row, before)
        self.assertTrue(ctx.problems)


@unittest.skipUnless(HAVE_GCC and HAVE_PYCPARSER, "needs gcc and pycparser (Docker image)")
class ConstTest(Fixture):
    def test_enum_at_sites(self):
        values = R.parse_values("AAA_MODE_IDLE 0 idle\nAAA_MODE_RUN 2\n", "v")
        tree, report, problems = R.const_plan(self.root, "include/ovl/AAA.h", "AaaMode", False, values,
                                              ["GameState.unk_02"])
        self.assertEqual(problems, [])
        tree.flush()
        c = self.read("src/ovl/AAA/80132000.c")
        self.assertIn("if (D_800E6280.unk_02 == AAA_MODE_RUN) {", c)
        self.assertIn("case AAA_MODE_IDLE:", c)
        self.assertIn("case 5:", c)
        self.assertIn("    func_80041000(1);", c)
        h = self.read("include/ovl/AAA.h")
        self.assertIn("enum AaaMode {\n    AAA_MODE_IDLE = 0, /* idle */\n    AAA_MODE_RUN = 2\n};", h)
        self.assertNotIn("AaaMode", self.read("include/game.h"))      # GameState is shared, the enum is AAA's
        self.assertTrue(any("5 has no name" in r for r in report), report)
        self.assertTrue(any("the type is shared" in r for r in report), report)
        values = R.parse_values("GAME_MODE_ZERO 0\n", "v")
        tree, report, problems = R.const_plan(self.root, "include/game.h", "GameMode", False, values,
                                              ["GameState.unk_02"])
        self.assertEqual(problems, [])
        tree.flush()
        self.assertIn("s16 unk_02; /* GameMode */", self.read("include/game.h"))

    def test_ambiguous_site(self):
        values = R.parse_values("X_A 0\n", "v")
        _t, _r, problems = R.const_plan(self.root, "include/ovl/AAA.h", "Xa", False, values, ["GameState.unk_00"])
        self.assertTrue(any("ambiguous" in p for p in problems), problems)


class SymtabTest(Fixture):
    def test_scope_and_placement(self):
        t = symtab.SymbolTable(self.root)
        self.assertEqual(t.resolve("AAA", "func_80132000"), ("AAA", 0x80132000))
        self.assertEqual(t.resolve("EVT", "D_800F6100"), ("EVT", 0x800F6100))
        self.assertEqual(t.resolve("AAA", "D_800F6100"), ("main", 0x800F6100))
        self.assertEqual(t.resolve("AAA", "known_func"), ("main", 0x80041100))
        self.assertEqual(t.placement("main", 0x800F6100), "config/symbol_addrs_main.txt")
        self.assertEqual(t.placement("main", 0x80041000, True), "config/symbol_addrs_hypotheses.txt")
        self.assertEqual(t.placement("AAA", 0x80132000), "config/symbols_AAA.txt")
        self.assertEqual(t.main_link_files(), ["config/symbol_addrs_main.txt", "config/symbol_addrs_types.txt"])
        self.assertEqual(t.problems(), [])

    def test_problems(self):
        self.write("config/symbol_addrs.txt", "a_one = 0x80041000;\nb_two = 0x80041000;\nc_view = 0x80041000; // alias-view\n"
                   "evt_name = 0x800F6200;\nCase = 0x80041010;\ncase = 0x80041014;\n")
        p = "\n".join(symtab.SymbolTable(self.root).problems())
        self.assertIn("0x80041000 has 2 names (a_one", p)
        self.assertNotIn("c_view in", p)
        self.assertIn("evt_name = 0x800F6200 lies in the range of EVT", p)
        self.assertIn("differ only in case: Case, case", p)

    def test_check_symbols_cli(self):
        import check_symbols
        with unittest.mock.patch("sys.stdout", io.StringIO()):
            self.assertEqual(check_symbols.main(["--root", self.root]), 0)
            self.write("config/symbol_addrs.txt", "evt_name = 0x800F6200;\n")
            self.assertEqual(check_symbols.main(["--root", self.root]), 1)


class PrerequisiteTest(Fixture):
    def test_gen_overlay_configs_keeps_symbol_files(self):
        import gen_overlay_configs as g
        old = "options:\n  symbol_addrs_path:\n    - config/symbol_addrs.txt\n    - config/symbols_AAA.txt\n  x: 1\n"
        new = "options:\n  symbol_addrs_path:\n    - config/symbol_addrs.txt\n    - config/labels/AAA.txt\n  x: 1\n"
        self.assertEqual(symtab.yaml_symbol_files(g.keep_symbol_files(new, old)),
                         ["config/symbol_addrs.txt", "config/symbols_AAA.txt", "config/labels/AAA.txt"])

    def test_split_objects_keeps_renamed_object_names(self):
        import split_objects as so
        self.write("config/overlays/AAA.yaml", self.read("config/overlays/AAA.yaml").replace(
            "AAA/80132100]", "AAA/aaa_tables]"))

        class O:
            def __init__(self, a):
                self.text_start = a
        names = so.object_names(self.root, "AAA", [O(0x80132000), O(0x80132100)])
        self.assertEqual(names(0x80132100), "AAA/aaa_tables")
        self.assertEqual(names(0x80132000), "AAA/80132000")
        split = so.object_names(self.root, "AAA", [O(0x80132000), O(0x80132100), O(0x80132180)])
        self.assertEqual(split(0x80132100), "AAA/80132100")      # the file is cut further: address names
        self.assertEqual(so.label_addresses(self.root, "AAA"), {})
        self.write("config/labels/AAA.txt", "aaa_label = 0x80132150;\n")
        self.assertIn("aaa_label = 0x80132150;", so.labels_text("AAA", {"aaa_label", "D_80132158"},
                                                                so.label_addresses(self.root, "AAA")))

    def test_migrate_globals_named_base(self):
        import migrate_globals as mg
        self.write("config/symbol_addrs.txt", self.read("config/symbol_addrs.txt") + "game_state = 0x800E6280;\n")
        mg._NAMES.clear()
        mg.load_names(self.root)
        agg = mg.Aggregate("game_state", "GameState", "include/game.h", self.root)
        self.assertEqual((agg.addr, agg.end), (0x800E6280, 0x800E6284))
        self.assertIs(mg.agg_for([agg], "D_800E6282"), agg)


if __name__ == "__main__":
    unittest.main()
