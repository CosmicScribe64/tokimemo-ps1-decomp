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
    def test_cached_closure_does_not_evaluate_fallback(self):
        tree = R.Tree(self.root)
        path = "src/main/80041000.c"
        expected = {path} | tree.index()["closure"][path]
        with unittest.mock.patch.object(tree, "closure", side_effect=AssertionError("unexpected scan")):
            self.assertEqual(set(R.closure_files(tree, [path])), expected)
            tree.index()["closure"][path] = set()
            self.assertEqual(R.closure_files(tree, [path]), [path])

    def test_unindexed_source_closure_uses_fallback(self):
        path = "src/main/not_mapped.c"
        self.write(path, '#include "game.h"\n')
        tree = R.Tree(self.root)
        self.assertNotIn(path, tree.index()["closure"])
        result = R.closure_files(tree, [path])
        self.assertIn(path, result)
        self.assertIn("include/game.h", result)

    def test_rows_and_notes(self):
        rows = R.parse_rows("# c\nfunc main func_80041000 boot T-1 entry # tail\n\n", "t")
        self.assertEqual((rows[0].kind, rows[0].scope, rows[0].old, rows[0].new, rows[0].note),
                         ("func", "main", "func_80041000", "boot", "T-1 entry # tail"))
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

    def test_placeholder_shapes(self):
        for name in ("sp1C", "sp50", "var_v0", "temp_v0_2", "phi_s0", "var_at", "temp_f12", "arg0"):
            self.refused("local main:func_80041000 index %s\n" % name, "placeholder")
        for name in ("speed", "space", "spec", "spade", "var_count", "temp_value", "phi_angle"):
            rc, out = self.run_plan("local main:func_80041000 index %s\n" % name)
            self.assertEqual(rc, 0, out)

    def test_alias_needs_flag(self):
        self.write("config/symbol_addrs_main.txt", "boot_view = 0x80041000; // type:func\n")
        self.refused("func main func_80041000 boot_view\n", "already names 0x80041000")

    def test_carried_obin_and_sdk_names(self):
        """A name from the O.BIN or SDK file keeps its spelling when it replaces the placeholder (T-27003)."""
        self.write("config/symbol_addrs_obin.txt", "LoadSquare = 0x80041000; // type:func\n")
        self.write("config/SLPM_86.053.yaml", self.read("config/SLPM_86.053.yaml").replace(
            "    - config/symbol_addrs_main.txt\n",
            "    - config/symbol_addrs_main.txt\n    - config/symbol_addrs_obin.txt\n"))
        self.refused("func main func_80041000 LoadSquare\n", "already names 0x80041000")
        rc, out = self.run_plan("func main func_80041000 LoadSquare\n", allow_alias=True)
        self.assertEqual(rc, 0, out)
        self.write("config/symbol_addrs_obin.txt", "memcpy = 0x80041000; // type:func\n")   # libc name, carried
        rc, out = self.run_plan("func main func_80041000 memcpy\n", allow_alias=True)
        self.assertEqual(rc, 0, out)
        self.write("config/symbol_addrs_obin.txt", "")
        self.write("config/symbol_addrs_main.txt", "plain_view = 0x80041000; // type:func\n")
        self.refused("func main func_80041000 plain_view\n", "already names 0x80041000")   # not carried: needs the flag
        self.write("config/symbol_addrs_main.txt", "LoadSquare = 0x80041000; // type:func\n")
        self.refused("func main func_80041000 LoadSquare\n", "is not snake_case", allow_alias=True)   # not from O.BIN/SDK

    def test_alias_merge_keeps_one_declaration(self):
        """An alias row drops the declaration of the old name and keeps the new name's (T-27003)."""
        self.write("config/symbol_addrs_obin.txt", "boot_view = 0x80041000; // type:func\n")
        self.write("config/SLPM_86.053.yaml", self.read("config/SLPM_86.053.yaml").replace(
            "    - config/symbol_addrs_main.txt\n",
            "    - config/symbol_addrs_main.txt\n    - config/symbol_addrs_obin.txt\n"))
        self.write("include/main_api.h", self.read("include/main_api.h").replace(
            "void known_func(s32 arg0);\n", "void known_func(s32 arg0);\nvoid boot_view(s32 arg0);\n"))
        out = self.ok("func main func_80041000 boot_view T-1 carried\n", allow_alias=True)
        api = self.read("include/main_api.h")
        self.assertEqual(api.count("boot_view"), 2, api)          # the wrapper and the declaration, nothing else
        self.assertIn("#ifndef MAIN_API_OVERRIDE_boot_view\nvoid boot_view(s32 arg0);\n#endif\n", api)
        self.assertNotIn("func_80041000", api)
        self.assertIn("declaration of func_80041000 dropped", out)



@unittest.skipUnless(HAVE_GCC, "needs the native gcc of the Docker image")
class OracleTest(Fixture):
    def test_nested_anonymous_member_ownership(self):
        self.write("include/game.h", self.read("include/game.h") +
                   "\ntypedef struct Outer { union { struct { int score; } s; } set; } Outer;\n")
        self.write("src/main/80041000.c", self.read("src/main/80041000.c") +
                   "\nvoid anon_load(Outer *p) { p->set.s.score = 0; }\n")
        out = self.ok("field main:Outer score ranking_score T-1\n", verify=True)
        self.assertIn("p->set.s.ranking_score = 0;", self.read("src/main/80041000.c"))
        self.assertIn("1 uses located by gcc", out)

    def test_ambiguous_anonymous_member_owners_fail_closed(self):
        self.write("include/game.h", self.read("include/game.h") +
                   "\ntypedef struct OuterA { union { struct { int score; } s; } set; } OuterA;\n"
                   "typedef struct OuterB { union { struct { int score; } s; } set; } OuterB;\n")
        self.write("src/main/80041000.c", self.read("src/main/80041000.c") +
                   "\nvoid anon_load(OuterA *a, OuterB *b) { a->set.s.score = 0; b->set.s.score = 0; }\n")
        out = self.refused("field main:OuterA score ranking_score T-1\n"
                           "field main:OuterB score metric_score T-1\n", "field oracle:")
        self.assertIn("a->set.s.score", self.read("src/main/80041000.c"))
        self.assertIn("b->set.s.score", self.read("src/main/80041000.c"))

    def test_preexisting_anonymous_missing_member_blocks_fallback(self):
        self.write("include/game.h", self.read("include/game.h") +
                   "\ntypedef struct Outer { union { struct { int score; } s; } set; } Outer;\n"
                   "typedef struct Other { union { struct { int unrelated; } s; } set; } Other;\n")
        self.write("src/main/80041000.c", self.read("src/main/80041000.c") +
                   "\nvoid anon_load(Outer *a, Other *b) { a->set.s.score = 0; b->set.s.score = 0; }\n")
        out = self.refused("field main:Outer score ranking_score T-1\n", "field oracle:")
        self.assertIn("a->set.s.score", self.read("src/main/80041000.c"))
        self.assertIn("b->set.s.score", self.read("src/main/80041000.c"))

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

    def test_field_uses_after_a_line_directive(self):
        # `#line N` renumbers the lines gcc reports (TACO keeps the original line numbers this way)
        src = self.read("src/main/80041100.c")
        self.write("src/main/80041100.c", src.replace(
            "    D_800E6280.unk_02 = arg0;\n",
            "#line 90\n    D_800E6280.unk_02 = arg0;\n#line 7\n    D_800E6280.unk_02 = D_800E6280.unk_02 + 1;\n"))
        out = self.ok("field GameState unk_02 step\n", verify=True)
        self.assertNotIn("PROBLEM", out)
        text = self.read("src/main/80041100.c")
        self.assertIn("#line 90\n    D_800E6280.step = arg0;\n#line 7\n    D_800E6280.step = D_800E6280.step + 1;", text)

    def test_norm_diag_ignores_the_suggestion(self):
        d = ("f.c", 3, 5, "'GameState' has no member named 'unk_3A'; did you mean 'unk_03A'?")
        e = ("f.c", 3, 5, "'GameState' has no member named 'unk_3A'; did you mean 'brightness'?")
        self.assertEqual(R.norm_diag(d, {"unk_03A": "brightness"}), R.norm_diag(e, {}))

    def test_physical_lines(self):
        self.assertEqual(R.physical_lines("a\nb\n", 2), [2])
        text = "a\n#line 10\nb\nc\n#line 3\nd\n"
        self.assertEqual(R.physical_lines(text, 10), [3])
        self.assertEqual(R.physical_lines(text, 11), [4])
        self.assertEqual(R.physical_lines(text, 3), [6])
        self.assertEqual(R.physical_lines(text, 1), [1])

    def test_field_member_clash(self):
        self.refused("field GameState unk_00 unk_02x\n", "placeholder")
        self.refused("field GameState unk_00 unk_02\n", "placeholder")
        self.write("include/game.h", self.read("include/game.h").replace("s16 unk_02;", "s16 mode;"))
        self.refused("field GameState unk_00 mode\n", "already has a member mode")

    def test_field_hypothesis_marks_the_renamed_member(self):
        # GameState, earlier in the header, has a member with the new name already
        self.write("include/game.h", self.read("include/game.h").replace("s16 unk_02;", "s16 flags;"))
        self.write("src/main/80041100.c", self.read("src/main/80041100.c").replace(".unk_02 =", ".flags ="))
        self.write("src/ovl/AAA/80132000.c", self.read("src/ovl/AAA/80132000.c").replace(".unk_02", ".flags"))
        self.ok("field Rec38 unk_00 flags HYPOTHESIS: bit flags\n", verify=True)
        game = self.read("include/game.h")
        self.assertIn("/* 0x02 */ s16 flags;\n", game)
        self.assertIn("/* 0x00 */ s16 flags; /* HYPOTHESIS: bit flags */\n} Rec38;", game)

    def test_two_members_of_one_type_in_one_table(self):
        out = self.ok("field GameState unk_00 mode\nfield GameState unk_02 step HYPOTHESIS: the step\n", verify=True)
        self.assertNotIn("PROBLEM", out)
        game = self.read("include/game.h")
        self.assertIn("/* 0x00 */ s16 mode;\n", game)
        self.assertIn("/* 0x02 */ s16 step; /* HYPOTHESIS: the step */\n", game)
        self.assertIn("D_800E6280.step = arg0;", self.read("src/main/80041100.c"))

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

    def test_type_hypothesis_mark_passes_the_round_trip(self):
        out = self.ok("type main Rec38 GirlRecord HYPOTHESIS: a record per girl\n")
        self.assertNotIn("PROBLEM", out)
        game = self.read("include/game.h")
        self.assertIn("s16 unk_00; /* HYPOTHESIS: a record per girl */\n} GirlRecord;", game)

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

    def test_kr_parameter_names_are_not_file_scope_names(self):
        p = "src/main/80041100.c"
        self.write(p, self.read(p) + "\nvoid kr_probe(object_index)\n    s32 object_index;\n{ (void)object_index; }\n")
        self.ok("local main:func_80041000 index object_index T-1\n")
        self.assertIn("s32 object_index;", self.read("src/main/80041000.c"))

    def test_real_global_still_blocks_local_name(self):
        p = "src/main/80041000.c"
        self.write(p, "s32 object_index;\n" + self.read(p))
        self.refused("local main:func_80041000 index object_index T-1\n", "file-scope name")

    def test_type_private_to_one_overlay(self):
        # AAA and EVT each define their own FnTbl; renaming AAA's leaves EVT's (and its views) alone
        for unit, member, addr in (("AAA", "s32 a;", "80132200"), ("EVT", "s16 b;", "800F6200")):
            h = "include/ovl/%s.h" % unit
            self.write(h, self.read(h).replace("#endif", "typedef struct FnTbl {\n    %s\n} FnTbl;\n"
                                               "extern FnTbl D_%s;\n#endif" % (member, addr), 1))
        self.write("src/ovl/EVT/800F6000.c", self.read("src/ovl/EVT/800F6000.c").replace(
            "D_800F6100 = 2;", "D_800F6100 = D_800F6200.b;"))
        self.write("src/ovl/AAA/80132100.c", self.read("src/ovl/AAA/80132100.c").replace(
            "func_80132000();", "func_80132000();\n    D_80132100 = D_80132200.a;"))
        out = self.ok("type AAA FnTbl AaaTable\n", verify=True)
        self.assertNotIn("PROBLEM", out)
        self.assertIn("} AaaTable;\nextern AaaTable D_80132200;", self.read("include/ovl/AAA.h"))
        self.assertIn("} FnTbl;\nextern FnTbl D_800F6200;", self.read("include/ovl/EVT.h"))


class EnumRenameTest(Fixture):
    def setUp(self):
        super().setUp()
        self.write("include/common.h", self.read("include/common.h").replace(
            "#endif", "enum MainMode { MODE_BOOT = 0, MODE_RUN = MODE_BOOT + 1 };\n"
            "typedef enum MainMode ModeAlias;\n#endif"))
        for path in ("src/main/80041000.c", "src/ovl/AAA/80132100.c"):
            self.write(path, self.read(path) + "\ns32 mode_value(void) { return MODE_RUN; }\n")

    def test_tag_and_enumerator_with_proofs(self):
        self.ok("type main MainMode GameMode\n"
                "enumerator main MODE_RUN GAME_MODE_RUN\n", verify=True)
        h = self.read("include/common.h")
        self.assertIn("enum GameMode { MODE_BOOT = 0, GAME_MODE_RUN = MODE_BOOT + 1 };", h)
        self.assertIn("typedef enum GameMode ModeAlias;", h)
        for path in ("src/main/80041000.c", "src/ovl/AAA/80132100.c"):
            self.assertIn("return GAME_MODE_RUN;", self.read(path))
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)

    def test_initializer_references_are_not_definitions(self):
        self.ok("enumerator main MODE_BOOT GAME_MODE_BOOT\n", verify=True)
        self.assertIn("MODE_RUN = GAME_MODE_BOOT + 1", self.read("include/common.h"))

    def test_constant_in_anonymous_enum_and_string_unchanged(self):
        self.write("include/common.h", self.read("include/common.h") +
                   '\nenum{ANON_MODE = sizeof("a,b"), OTHER_MODE = ANON_MODE + 1};\n')
        self.write("src/main/80041000.c", self.read("src/main/80041000.c") +
                   '\nchar *mode_label = "ANON_MODE";\n')
        self.ok("enumerator main ANON_MODE GAME_MODE_ANONYMOUS\n", verify=True)
        self.assertIn("OTHER_MODE = GAME_MODE_ANONYMOUS + 1", self.read("include/common.h"))
        self.assertIn('"ANON_MODE"', self.read("src/main/80041000.c"))

    def test_enumerator_collision_case_and_not_a_member(self):
        self.refused("enumerator main MODE_BOOT MODE_RUN\n", "already an identifier")
        self.refused("enumerator main MODE_BOOT game_mode_boot\n", "not UPPER_SNAKE")
        self.refused("enumerator main s32 GAME_MODE_BOOT\n", "0 definitions")

    def test_private_overlay_scope(self):
        for unit in ("AAA", "EVT"):
            path = "include/ovl/%s.h" % unit
            self.write(path, self.read(path) + "\nenum PrivateMode { PRIVATE_WAIT = 0 };\n")
        self.ok("enumerator AAA PRIVATE_WAIT AAA_MODE_WAIT\n", verify=True)
        self.assertIn("AAA_MODE_WAIT", self.read("include/ovl/AAA.h"))
        self.assertIn("PRIVATE_WAIT", self.read("include/ovl/EVT.h"))

    def test_hypothesis_check_and_replay(self):
        self.ok("enumerator main MODE_RUN GAME_MODE_RUN HYPOTHESIS: inferred role\n")
        self.assertIn("/* HYPOTHESIS: inferred role */", self.read("include/common.h"))
        path = "src/main/80041000.c"
        self.write(path, self.read(path).replace("GAME_MODE_RUN", "MODE_RUN"))
        self.assertNotEqual(R.check(self.root, out=io.StringIO()), 0)
        self.assertEqual(R.replay(self.root, out=io.StringIO(), use_gcc=True), 0)
        self.assertIn("return GAME_MODE_RUN;", self.read(path))
        self.assertEqual(R.replay(self.root, out=io.StringIO(), use_gcc=True), 0)
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)

    def test_private_main_enum_does_not_claim_overlay_member(self):
        main = "src/main/80041000.c"
        self.write(main, self.read(main) + "\nenum LocalMode { MODE_WAIT = 0 };\n")
        header = "include/ovl/AAA.h"
        self.write(header, self.read(header) + "\nenum OtherMode { MODE_WAIT = 1 };\n")
        self.ok("enumerator main MODE_WAIT GAME_MODE_WAIT\n", verify=True)
        self.assertIn("MODE_WAIT = 1", self.read(header))
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)
        self.write(main, self.read(main).replace("GAME_MODE_WAIT", "MODE_WAIT"))
        self.assertEqual(R.replay(self.root, out=io.StringIO(), use_gcc=True), 0)
        self.assertIn("GAME_MODE_WAIT = 0", self.read(main))
        self.assertIn("MODE_WAIT = 1", self.read(header))

    def test_enum_alias_in_including_header(self):
        path = "include/game.h"
        self.write(path, self.read(path) + "\ntypedef enum MainMode SeparateAlias;\n")
        self.ok("type main SeparateAlias ReadableMode\n", verify=True)
        self.assertIn("typedef enum MainMode ReadableMode;", self.read(path))
        self.assertIn("enum MainMode {", self.read("include/common.h"))

    def test_case_insensitive_enumerator_collision(self):
        self.write("include/common.h", self.read("include/common.h") +
                   "\nextern s32 game_mode_wait;\n")
        self.refused("enumerator main MODE_BOOT GAME_MODE_WAIT\n", "already an identifier")


class FileRenameTest(Fixture):
    def test_path_scan_skips_irrelevant_files_and_preserves_path_rules(self):
        unrelated = "/* NO_OLD_PATH_SENTINEL */\ns32 unrelated_path_value;\n"
        self.write("src/main/irrelevant.c", unrelated)
        selected = "src/main/80041000.c"
        self.write(selected, self.read(selected) +
                   '\n/* See src/ovl/AAA/80132100.c and AAA/80132100. */\n'
                   'char *asm_path = "asm/ovl/AAA/nonmatchings/AAA/80132100";\n'
                   'char *plain_path = "src/ovl/AAA/80132100.c";\n'
                   'char *longer_path = "asm/ovl/AAA/nonmatchings/AAA/80132100_extra";\n'
                   'char *case_path = "asm/ovl/aaa/nonmatchings/aaa/80132100";\n')
        original_tokens, original_rewrite = R.tokens, R.path_rewrite
        candidates = []

        def checked_tokens(text):
            self.assertNotIn("NO_OLD_PATH_SENTINEL", text)
            return original_tokens(text)

        def checked_rewrite(tree, files, *args, **kwargs):
            candidates.append(list(files))
            return original_rewrite(tree, files, *args, **kwargs)

        ctx = R.Context(self.root)
        row = R.Row("file", "AAA", "80132100", "aaa_tables", "T-27069", "test")
        with unittest.mock.patch.object(R, "tokens", checked_tokens), \
                unittest.mock.patch.object(R, "path_rewrite", checked_rewrite):
            R.plan_file(ctx, row)
        self.assertEqual(ctx.problems, [])
        self.assertEqual(len(candidates), 2)
        self.assertEqual(candidates[0], candidates[1])
        self.assertNotIn("src/main/irrelevant.c", candidates[0])
        self.assertEqual(ctx.tree.read("src/main/irrelevant.c"), unrelated)
        text = ctx.tree.read(selected)
        self.assertIn("src/ovl/AAA/aaa_tables.c and AAA/aaa_tables.", text)
        self.assertIn('"asm/ovl/AAA/nonmatchings/AAA/aaa_tables"', text)
        self.assertIn('"src/ovl/AAA/80132100.c"', text)
        self.assertIn('"asm/ovl/AAA/nonmatchings/AAA/80132100_extra"', text)
        self.assertIn('"asm/ovl/aaa/nonmatchings/aaa/80132100"', text)
        self.assertIn("AAA/aaa_tables", ctx.tree.read("src/ovl/AAA/aaa_tables.c"))

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


class NamedPlaceholderTest(Fixture):
    def test_check_reports_a_placeholder_of_a_named_symbol(self):
        """check refuses func_80041100 in the sources while the symbol files name 0x80041100 (T-27003)."""
        self.write("src/main/80041100.c", self.read("src/main/80041100.c") + "\nvoid use_it(void) {\n    func_80041100();\n}\n")
        out = io.StringIO()
        self.assertEqual(R.check(self.root, out=out), 1)
        self.assertIn("src/main/80041100.c: func_80041100 is used for 0x80041100, which the symbol files name known_func",
                      out.getvalue())
        self.write("config/symbol_addrs.txt", self.read("config/symbol_addrs.txt")
                   + "func_80041100 = 0x80041100; // alias-view call view\n")
        out = io.StringIO()
        self.assertEqual(R.check(self.root, out=out), 0, out.getvalue())     # a deliberate second name is exempt
        self.assertIn("renames OK", out.getvalue())


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

    def test_replay_long_chain_with_local_and_field_rows(self):
        # a 3-name chain, then a local row (scoped by the middle name) and a field row; the tree has the old names
        self.write("config/renames/main.txt",
                   "func main func_80041000 boot_a 0x80041000\n"
                   "func main boot_a boot_b 0x80041000\n"
                   "local main:boot_a index mode_value -\n"
                   "field GameState unk_02 step -\n")
        out = io.StringIO()
        self.assertEqual(R.replay(self.root, out=out, use_gcc=HAVE_GCC), 0, out.getvalue())
        main = self.read("src/main/80041000.c")
        self.assertIn("void boot_b(void) {", main)
        self.assertIn("s32 mode_value;", main)
        self.assertIn("/* 0x02 */ s16 step;", self.read("include/game.h"))
        if HAVE_GCC:
            self.assertIn("D_800E6280.step = arg0;", self.read("src/main/80041100.c"))
        out = io.StringIO()
        self.assertEqual(R.check(self.root, out=out), 0, out.getvalue())
        out = io.StringIO()
        self.assertEqual(R.replay(self.root, out=out, use_gcc=HAVE_GCC), 0, out.getvalue())
        self.assertIn("0 files changed", out.getvalue())

    def field_chain_tree(self, held):
        """A tree whose GameState holds `held` (unk_02, step or stage) while its code uses all three names."""
        self.write("config/renames/main.txt", "field GameState unk_02 step -\nfield GameState step stage -\n")
        h = self.read("include/game.h")
        self.write("include/game.h", h.replace("s16 unk_02;", "s16 %s;" % held))
        self.write("src/main/80041100.c", self.read("src/main/80041100.c").replace("D_800E6280.unk_02", "D_800E6280.%s" % held))
        self.write("src/ovl/AAA/80132100.c", self.read("src/ovl/AAA/80132100.c")
                   + "void again(void) { D_800E6280.unk_02 = 1; D_800E6280.step = 2; D_800E6280.stage = 3; }\n")

    @unittest.skipUnless(HAVE_GCC and HAVE_PYCPARSER, "needs gcc and pycparser (Docker image)")
    def test_replay_field_chain_walks_from_the_name_the_tree_has(self):
        # unk_02 -> step -> stage (B1 of the T-27002 review): the definition holds any one of the three names
        for held in ("unk_02", "step", "stage"):
            with self.subTest(held=held):
                self.tearDown()
                self.setUp()
                self.field_chain_tree(held)
                out = io.StringIO()
                self.assertEqual(R.replay(self.root, out=out), 0, out.getvalue())
                self.assertIn("/* 0x02 */ s16 stage;", self.read("include/game.h"))
                self.assertNotIn("unk_02", self.read("include/game.h").split("Rec38")[0])
                self.assertIn("D_800E6280.stage = arg0;", self.read("src/main/80041100.c"))
                self.assertIn("void again(void) { D_800E6280.stage = 1; D_800E6280.stage = 2; D_800E6280.stage = 3; }",
                              self.read("src/ovl/AAA/80132100.c"))
                self.assertIn("D_80132100 = D_800E6280.stage;", self.read("src/ovl/AAA/80132000.c"))
                self.assertEqual(R.check(self.root, out=io.StringIO()), 0)
                out = io.StringIO()
                self.assertEqual(R.replay(self.root, out=out), 0, out.getvalue())
                self.assertIn("0 files changed", out.getvalue())

    def test_replay_field_chain_across_a_type_rename(self):
        # Rec38 unk_00 -> frac, the type becomes GirlRecord, then frac -> fraction (T-27004 merge): the tree has
        # the last names, and `unk_00` is still a member of another type (GameState), so the first row is a candidate
        self.write("config/renames/main.txt",
                   "type main Rec38 GirlRecord -\n"
                   "field Rec38 unk_00 frac -\n"
                   "field GirlRecord frac fraction -\n")
        h = self.read("include/game.h").replace("typedef struct Rec38 {", "typedef struct GirlRecord {")
        h = h.replace("    /* 0x00 */ s16 unk_00;\n} Rec38;", "    /* 0x00 */ s16 fraction;\n} GirlRecord;")
        self.write("include/game.h", h)
        self.write("src/main/80041100.c", self.read("src/main/80041100.c").replace("Rec38 r;", "GirlRecord r;").replace("r.unk_00", "r.fraction"))
        self.assertIn("s16 fraction;", self.read("include/game.h"))
        out = io.StringIO()
        self.assertEqual(R.replay(self.root, out=out, use_gcc=False), 0, out.getvalue())
        self.assertNotIn("PROBLEM", out.getvalue())
        self.assertIn("0 files changed", out.getvalue())
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)

    @unittest.skipUnless(HAVE_GCC and HAVE_PYCPARSER, "needs gcc and pycparser (Docker image)")
    def test_replay_field_chain_with_two_names_declared(self):
        self.field_chain_tree("step")
        self.write("include/game.h", self.read("include/game.h").replace("s16 step;", "s16 step;\n    s16 unk_02;"))
        out = io.StringIO()
        self.assertEqual(R.replay(self.root, out=out), 1, out.getvalue())
        self.assertIn("cannot tell which one", out.getvalue())
        self.assertIn("unk_02;", self.read("include/game.h"))          # nothing written

    def test_replay_local_row_before_its_function_rename(self):
        self.write("config/renames/a.txt", "local main:func_80041000 index mode_value -\n")
        self.write("config/renames/b.txt", "func main func_80041000 boot_a 0x80041000\n"
                                           "func main boot_a boot_b 0x80041000\n")
        out = io.StringIO()
        self.assertEqual(R.replay(self.root, out=out, use_gcc=False), 0, out.getvalue())
        main = self.read("src/main/80041000.c")
        self.assertIn("void boot_b(void) {", main)
        self.assertIn("s32 mode_value;", main)

    def test_ledger_note_keeps_hash(self):
        rows = R.parse_rows("# head\nfunc main a b 0x80041000 T-1 see issue #3\n", "l", ledger=True)
        self.assertEqual(rows[0].note, "T-1 see issue #3")

    def test_replay_symbol_line_dedupe(self):
        self.ok("func main func_80041180 text_merge\n")
        self.write("config/symbol_addrs_main.txt", self.read("config/symbol_addrs_main.txt")
                   + "func_80041180 = 0x80041180; // type:func size:0x20\n")
        self.assertGreater(R.check(self.root, out=io.StringIO()), 0)
        R.replay(self.root, out=io.StringIO(), use_gcc=False)
        self.assertEqual(self.read("config/symbol_addrs_main.txt").count("text_merge"), 1)
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)

    def test_replay_symbol_line_dedupe_keeps_attributes(self):
        self.ok("func main func_80041180 text_merge\n")
        # the kept line has no size; the repeats carry attributes
        main = self.read("config/symbol_addrs_main.txt")
        self.write("config/symbol_addrs_main.txt", main.replace("// type:func size:0x20", "// type:func")
                   + "func_80041180 = 0x80041180; // type:func size:0x10 align:4\n"
                   + "func_80041180 = 0x80041180;\n")
        out = io.StringIO()
        self.assertEqual(R.replay(self.root, out=out, use_gcc=False), 0, out.getvalue())
        lines = [l for l in self.read("config/symbol_addrs_main.txt").splitlines() if "text_merge" in l]
        self.assertEqual(lines, ["text_merge = 0x80041180; // type:func size:0x10 align:4"])
        # a conflicting value stays as the first line has it, and is listed
        main = self.read("config/symbol_addrs_main.txt")
        self.write("config/symbol_addrs_main.txt", main + "func_80041180 = 0x80041180; // size:0x99\n")
        out = io.StringIO()
        self.assertEqual(R.replay(self.root, out=out, use_gcc=False), 0, out.getvalue())
        self.assertIn("has size:0x10, the dropped duplicate line has size:0x99", out.getvalue())
        self.assertEqual([l for l in self.read("config/symbol_addrs_main.txt").splitlines() if "text_merge" in l],
                         ["text_merge = 0x80041180; // type:func size:0x10 align:4"])

    def test_table_note_keeps_hash(self):
        self.ok("func AAA func_80132000 aaa_start T-1 see issue #3 # and more\n")
        row = R.read_ledger(self.root)[0]
        self.assertEqual(row.note, "T-1 see issue #3 # and more")
        self.assertIn("func AAA func_80132000 aaa_start 0x80132000 T-1 see issue #3 # and more",
                      self.read("config/renames/AAA.txt"))

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
class ReplayMovesTest(Fixture):
    """Replay of file, folder and header rows on trees where a stale branch left things under the old name
    (B2 of the T-27002 review)."""

    STALE_C = ('#include "ovl/AAA.h"\n\nINCLUDE_RODATA("asm/ovl/AAA/data/AAA/80132200.rodata", D_80132280);\n\n'
               'void func_80132200(void) {\n}\nINCLUDE_ASM("asm/ovl/AAA/nonmatchings/AAA/80132200", func_80132240);\n')

    def add_stale_object(self, folder):
        """What a branch that forked before the rename adds: an object under the old folder."""
        y = self.read("config/overlays/AAA.yaml")
        self.write("config/overlays/AAA.yaml", y.replace(
            "  - [0x300, data, AAA_data]",
            "  - [0x280, c, %s/80132200]\n  - [0x2c0, .rodata, %s/80132200]\n  - [0x300, data, AAA_data]" % (folder, folder)))
        self.write("src/ovl/%s/80132200.c" % folder, self.STALE_C)
        self.write("config/toolchains.txt", self.read("config/toolchains.txt") + "%s/80132200 ido 5.3\n" % folder)

    def remove(self, path):
        os.remove(os.path.join(self.root, path))

    def replay_ok(self, expect_changed=True):
        out = io.StringIO()
        self.assertEqual(R.replay(self.root, out=out, use_gcc=False), 0, out.getvalue())
        if not expect_changed:
            self.assertIn("0 files changed", out.getvalue())
        return out.getvalue()

    def assert_stable(self):
        """check is clean and a second replay changes nothing."""
        out = io.StringIO()
        self.assertEqual(R.check(self.root, out=out), 0, out.getvalue())
        self.replay_ok(expect_changed=False)

    def test_dir_object_added_under_the_old_folder(self):
        self.ok("dir AAA AAA alpha_event T-1\n")
        self.add_stale_object("AAA")
        out = io.StringIO()
        self.assertGreater(R.check(self.root, out=out), 0)
        self.assertIn("AAA was renamed to alpha_event", out.getvalue())
        self.replay_ok()
        y = self.read("config/overlays/AAA.yaml")
        self.assertIn("[0x280, c, alpha_event/80132200]", y)
        self.assertIn("[0x2c0, .rodata, alpha_event/80132200]", y)
        self.assertNotIn("AAA/8", y)
        self.assertIsNone(self.read("src/ovl/AAA/80132200.c"))
        c = self.read("src/ovl/alpha_event/80132200.c")
        self.assertIn('"asm/ovl/AAA/data/alpha_event/80132200.rodata"', c)
        self.assertIn('"asm/ovl/AAA/nonmatchings/alpha_event/80132200"', c)
        self.assertIn("alpha_event/80132200 ido 5.3", self.read("config/toolchains.txt"))
        self.assertNotIn("AAA/80132200", self.read("config/toolchains.txt"))
        self.assert_stable()

    def test_dir_c_file_kept_at_the_old_path(self):
        # the yaml says alpha_event already, the branch's copy of the C file sits in the old folder
        self.ok("dir AAA AAA alpha_event T-1\n")
        self.add_stale_object("alpha_event")
        self.write("src/ovl/AAA/80132200.c", self.read("src/ovl/alpha_event/80132200.c"))
        self.remove("src/ovl/alpha_event/80132200.c")
        out = io.StringIO()
        self.assertGreater(R.check(self.root, out=out), 0)
        self.replay_ok()
        self.assertIsNone(self.read("src/ovl/AAA/80132200.c"))
        self.assertIn("void func_80132200(void)", self.read("src/ovl/alpha_event/80132200.c"))
        self.assert_stable()

    def test_dir_old_and_new_file_both_exist(self):
        self.ok("dir AAA AAA alpha_event T-1\n")
        self.add_stale_object("AAA")
        self.write("src/ovl/alpha_event/80132200.c", "/* another */\n")
        before = {p: self.read(p) for p in ("config/overlays/AAA.yaml", "src/ovl/AAA/80132200.c")}
        out = io.StringIO()
        self.assertEqual(R.replay(self.root, out=out, use_gcc=False), 1)
        self.assertIn("both exist", out.getvalue())
        self.assertEqual(before, {p: self.read(p) for p in before})

    def test_dir_chain_of_three_names(self):
        self.write("config/renames/AAA.txt", "dir AAA AAA alpha_event -\ndir AAA alpha_event beta_event -\n")
        self.replay_ok()                                    # the tree has the first name
        self.assertIn("[0x0, c, beta_event/80132000]", self.read("config/overlays/AAA.yaml"))
        self.assertIsNotNone(self.read("src/ovl/beta_event/80132100.c"))
        self.assertIsNone(self.read("src/ovl/AAA/80132100.c"))
        self.assert_stable()
        # the tree has the middle name, and a stale object under the first one
        self.tearDown()
        self.setUp()
        self.ok("dir AAA AAA alpha_event T-1\n", ledger="scratch")
        self.remove("config/renames/scratch.txt")
        self.write("config/renames/AAA.txt", "dir AAA AAA alpha_event -\ndir AAA alpha_event beta_event -\n")
        self.add_stale_object("AAA")
        self.replay_ok()
        self.assertIn("[0x280, c, beta_event/80132200]", self.read("config/overlays/AAA.yaml"))
        self.assertIsNotNone(self.read("src/ovl/beta_event/80132200.c"))
        self.assert_stable()

    def test_file_island_added_under_the_old_name(self):
        # data_island.py in a branch adds a .data island named after the object, and its C text
        self.ok("file AAA 80132100 aaa_tables T-1 the tables of AAA\n")
        y = self.read("config/overlays/AAA.yaml")
        self.write("config/overlays/AAA.yaml", y.replace(
            "  - [0x300, data, AAA_data]", "  - [0x2c0, .data, AAA/80132100]\n  - [0x300, data, AAA_data]"))
        c = self.read("src/ovl/AAA/aaa_tables.c")
        self.write("src/ovl/AAA/aaa_tables.c", c + 'INCLUDE_RODATA("asm/ovl/AAA/data/AAA/80132100.data", D_801321C0);\n')
        out = io.StringIO()
        self.assertGreater(R.check(self.root, out=out), 0)
        self.assertIn("80132100 was renamed to aaa_tables", out.getvalue())
        self.replay_ok()
        self.assertIn("[0x2c0, .data, AAA/aaa_tables]", self.read("config/overlays/AAA.yaml"))
        self.assertIn('"asm/ovl/AAA/data/AAA/aaa_tables.data"', self.read("src/ovl/AAA/aaa_tables.c"))
        self.assert_stable()

    def test_file_c_file_kept_at_the_old_path(self):
        self.ok("file AAA 80132100 aaa_tables T-1 the tables of AAA\n")
        self.remove("src/ovl/AAA/aaa_tables.c")
        self.write("src/ovl/AAA/80132100.c", '#include "ovl/AAA.h"\nINCLUDE_ASM("asm/ovl/AAA/nonmatchings/AAA/80132100", func_80132180);\n')
        out = io.StringIO()
        self.assertGreater(R.check(self.root, out=out), 0)
        self.replay_ok()
        self.assertIsNone(self.read("src/ovl/AAA/80132100.c"))
        c = self.read("src/ovl/AAA/aaa_tables.c")
        self.assertTrue(c.startswith("/* aaa_tables.c: the tables of AAA. Original object 0x80132100-0x80132200 "), c)
        self.assertIn('"asm/ovl/AAA/nonmatchings/AAA/aaa_tables"', c)
        self.assert_stable()

    def test_file_added_object_with_the_old_yaml_line(self):
        # a branch that forked before the rename: the old yaml line and the old file came back with the new file
        self.ok("file AAA 80132100 aaa_tables T-1 the tables of AAA\n")
        self.write("config/overlays/AAA.yaml", self.read("config/overlays/AAA.yaml").replace(
            "AAA/aaa_tables", "AAA/80132100"))
        self.remove("src/ovl/AAA/aaa_tables.c")
        self.write("src/ovl/AAA/80132100.c", '#include "ovl/AAA.h"\nvoid f(void) {}\n')
        self.replay_ok()
        self.assertIn("[0x100, c, AAA/aaa_tables]", self.read("config/overlays/AAA.yaml"))
        self.assertIn("[0x200, .rodata, AAA/aaa_tables]", self.read("config/overlays/AAA.yaml"))
        self.assertIn("void f(void) {}", self.read("src/ovl/AAA/aaa_tables.c"))
        self.assert_stable()

    def test_file_old_and_new_file_both_exist(self):
        self.ok("file AAA 80132100 aaa_tables T-1 the tables of AAA\n")
        self.write("src/ovl/AAA/80132100.c", "/* the branch's copy */\n")
        before = self.read("src/ovl/AAA/aaa_tables.c")
        out = io.StringIO()
        self.assertEqual(R.replay(self.root, out=out, use_gcc=False), 1)
        self.assertIn("both exist", out.getvalue())
        self.assertEqual(before, self.read("src/ovl/AAA/aaa_tables.c"))
        self.assertEqual(self.read("src/ovl/AAA/80132100.c"), "/* the branch's copy */\n")

    def test_file_chain_of_three_names(self):
        self.write("config/renames/AAA.txt", "file AAA 80132100 tables_a 0x80132100 T-1 the tables\n"
                                              "file AAA tables_a tables_b 0x80132100 T-2\n")
        self.replay_ok()                                    # the tree has the oldest name
        self.assertIn("[0x100, c, AAA/tables_b]", self.read("config/overlays/AAA.yaml"))
        self.assertIsNotNone(self.read("src/ovl/AAA/tables_b.c"))
        self.assert_stable()
        # the tree has the middle name
        self.tearDown()
        self.setUp()
        self.ok("file AAA 80132100 tables_a 0x80132100 T-1 the tables\n", ledger="scratch")
        self.remove("config/renames/scratch.txt")
        self.write("config/renames/AAA.txt", "file AAA 80132100 tables_a 0x80132100 T-1 the tables\n"
                                              "file AAA tables_a tables_b 0x80132100 T-2\n")
        self.replay_ok()
        self.assertIn("[0x100, c, AAA/tables_b]", self.read("config/overlays/AAA.yaml"))
        self.assertIsNone(self.read("src/ovl/AAA/tables_a.c"))
        self.assert_stable()

    def test_header_chain_of_three_names(self):
        self.write("config/renames/main.txt", "file include ovl/AAA.h ovl/alpha.h - T-1\n"
                                               "file include ovl/alpha.h ovl/beta.h - T-2\n")
        self.replay_ok()
        self.assertIsNotNone(self.read("include/ovl/beta.h"))
        self.assertIn('#include "ovl/beta.h"', self.read("src/ovl/AAA/80132000.c"))
        self.assertIn("#ifndef OVL_BETA_H", self.read("include/ovl/beta.h"))
        self.assert_stable()
        # the tree has the middle name; a stale file includes the first one
        self.tearDown()
        self.setUp()
        self.ok("file include ovl/AAA.h ovl/alpha.h T-1\n", ledger="scratch")
        self.remove("config/renames/scratch.txt")
        self.write("src/ovl/EVT/800F6000.c", '#include "ovl/AAA.h"\n' + self.read("src/ovl/EVT/800F6000.c"))
        self.write("config/renames/main.txt", "file include ovl/AAA.h ovl/alpha.h - T-1\n"
                                               "file include ovl/alpha.h ovl/beta.h - T-2\n")
        self.replay_ok()
        self.assertIsNone(self.read("include/ovl/alpha.h"))
        self.assertIn('#include "ovl/beta.h"', self.read("src/ovl/EVT/800F6000.c"))
        self.assert_stable()


class ConstTest(Fixture):
    def test_cli_positive_caller_apply_check_replay_and_repeat(self):
        import contextlib
        import io
        path = "src/ovl/AAA/80132000.c"
        original = self.read(path) + "\nvoid caller_cli(void) { if (D_800E6280.unk_02 == 0) {} }\n"
        self.write(path, original)
        values_path = os.path.join(self.root, "values.txt")
        self.write("values.txt", "AAA_MODE_ZERO 0\n")
        args = ["--root", self.root, "const", "--header", "include/ovl/AAA.h", "--enum", "AaaMode",
                "--values", values_path, "--site", "in:AAA:caller_cli:GameState.unk_02", "--apply"]
        with contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(R.main(args), 0)
        applied_source = self.read(path)
        applied_header = self.read("include/ovl/AAA.h")
        self.assertIn("D_800E6280.unk_02 == AAA_MODE_ZERO", applied_source)
        self.assertIn("AAA_MODE_ZERO = 0", applied_header)
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)
        self.write(path, original)
        self.assertNotEqual(R.check(self.root, out=io.StringIO()), 0)
        with contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(R.main(["--root", self.root, "replay"]), 0)
        self.assertEqual(self.read(path), applied_source)
        self.assertEqual(self.read("include/ovl/AAA.h"), applied_header)
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)
        with contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(R.main(["--root", self.root, "replay"]), 0)
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)

    def test_caller_site_is_unit_and_function_scoped(self):
        path = "src/ovl/AAA/80132000.c"
        original = self.read(path)
        self.write(path, original + "\nvoid caller_a(void) { if (D_800E6280.unk_02 == 0) {} }\n"
                   "void caller_b(void) { if (D_800E6280.unk_02 == 2) {} }\n")
        main = "src/main/80041000.c"
        self.write(main, self.read(main) + "\nvoid caller_a(void) { if (D_800E6280.unk_02 == 0) {} }\n")
        values = R.parse_values("AAA_ZERO 0\n", "v")
        tree, report, problems = R.const_plan(self.root, "include/game.h", "AaaMode", False,
                                              values, ["in:AAA:caller_a:GameState.unk_02"])
        self.assertEqual(problems, [], (report, problems))
        tree.flush()
        text = self.read(path)
        self.assertIn("D_800E6280.unk_02 == AAA_ZERO", text)
        self.assertIn("D_800E6280.unk_02 == 2", text)
        self.assertIn("D_800E6280.unk_02 == 0", self.read(main))

    def test_caller_site_rejects_unknown_unit_or_function(self):
        values = R.parse_values("AAA_ZERO 0\n", "v")
        for site, expected in (("in:BAD:caller_a:GameState.unk_02", "one active definition"),
                               ("in:AAA:missing:GameState.unk_02", "one active definition")):
            _tree, _report, problems = R.const_plan(self.root, "include/ovl/AAA.h", "AaaMode", False,
                                                    values, [site], _signed=True)
            self.assertTrue(any(expected in p for p in problems), problems)

    def test_caller_site_rejects_inactive_or_macro_origin(self):
        path = "src/ovl/AAA/80132000.c"
        original = self.read(path)
        self.write(path, original + "\n#define MODE_TEST(value) if (D_800E6280.unk_02 == value) {}\n"
                   "#if 0\nvoid caller_a(void) { if (D_800E6280.unk_02 == 0) {} }\n#endif\n"
                   "void caller_b(void) { MODE_TEST(0); }\n")
        values = R.parse_values("AAA_ZERO 0\n", "v")
        _tree, _report, problems = R.const_plan(self.root, "include/ovl/AAA.h", "AaaMode", False,
                                                values, ["in:AAA:caller_a:GameState.unk_02"], _signed=True)
        self.assertTrue(any("one active definition" in p for p in problems), problems)
        self.write(path, original + "\n#define MODE_TEST(value) if (D_800E6280.unk_02 == value) {}\n"
                   "void caller_a(void) { MODE_TEST(0); }\n")
        ctx = R.Context(self.root)
        R.signed_const_plan(ctx, "include/ovl/AAA.h", "AaaMode", False, values,
                            ["in:AAA:caller_a:GameState.unk_02"], verify=False)
        self.assertTrue(any("tokens changed after preprocessing" in p for p in ctx.problems), ctx.problems)

    def test_caller_filter_supports_a_local_selector(self):
        path = "src/ovl/AAA/80132000.c"
        self.write(path, self.read(path) + "\nvoid caller_local(void) { s32 phase; phase = 1; if (phase == 0) {} }\n")
        values = R.parse_values("AAA_ZERO 0\n", "v")
        tree, _report, problems = R.const_plan(self.root, "include/ovl/AAA.h", "AaaMode", False,
                                              values, ["in:AAA:caller_local:phase"], _signed=True)
        self.assertEqual(problems, [])
        tree.flush()
        self.assertIn("if (phase == AAA_ZERO)", self.read(path))

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

    def test_any_type_site(self):
        values = R.parse_values("X_A 0\n", "v")
        _t, _r, problems = R.const_plan(self.root, "include/ovl/AAA.h", "Xa", False, values, ["*.unk_00"])
        self.assertEqual(problems, [])
        self.assertEqual(R.parse_site("*.weekday"), ("member", "*", "weekday"))


class ExistingConstantSitesTest(Fixture):
    """T-27068: reuse is opt-in, exact and fails closed on unsupported sites or declarations."""
    def setUp(self):
        super().setUp()
        h = self.read("include/common.h")
        self.write("include/common.h", h.replace("#endif", "enum Club { CLUB_BASEBALL = 6, CLUB_NONE = 11 };\n#endif"))
        h = self.read("include/game.h")
        h = h.replace("typedef struct GameState {", "typedef union Flags { unsigned short h; s32 w; } Flags;\n"
                      "typedef struct ClubRecord { s32 level; } ClubRecord;\ntypedef struct GameState {")
        h = h.replace("/* 0x00 */ s16 unk_00;", "Flags player_flags; ClubRecord clubs[12];\n    /* 0x00 */ s16 unk_00;", 1)
        self.write("include/game.h", h)
        self.path = "src/main/80041000.c"
        self.source = self.read(self.path) + "\ns32 club_rule(void) {\n    GameState *gs = &D_800E6280;\n" \
            "    s32 unrelated = 6;\n    s32 girl = 7;\n" \
            "    if ((gs->player_flags.h >> 12) == 6 && (D_800E6280.clubs[6].level & 15) >= 8) return 1;\n" \
            "    return unrelated + girl;\n}\n"
        self.write(self.path, self.source)
        self.values = R.parse_values("CLUB_BASEBALL 6\n", "values")
        self.sites = ["expr:main:club_rule:(gs->player_flags.h >> 12)",
                      "index:main:club_rule:D_800E6280.clubs"]

    def plan_reuse(self, values=None, sites=None, header="include/common.h", enum="Club"):
        ctx = R.Context(self.root)
        R.const_reuse_plan(ctx, header, enum, self.values if values is None else values,
                           self.sites if sites is None else sites)
        return ctx

    def test_exact_comparison_and_index_leave_unselected_values(self):
        before_header = self.read("include/common.h")
        ctx = self.plan_reuse()
        self.assertEqual(ctx.problems, [])
        text = ctx.tree.read(self.path)
        self.assertIn("(gs->player_flags.h >> 12) == CLUB_BASEBALL", text)
        self.assertIn("clubs[CLUB_BASEBALL].level & 15) >= 8", text)
        self.assertIn("unrelated = 6", text)
        self.assertIn("girl = 7", text)
        self.assertEqual(ctx.tree.read("include/common.h"), before_header)
        self.assertEqual(ctx.tree.changed(), [self.path])

    def test_comparison_and_index_require_separate_sites(self):
        ctx = self.plan_reuse(sites=self.sites[:1])
        self.assertEqual(ctx.problems, [])
        self.assertIn("clubs[6]", ctx.tree.read(self.path))

    def test_unsigned_literal_keeps_its_type(self):
        self.write(self.path, self.source.replace(
            "== 6 &&", "== 6U &&").replace("clubs[6]", "clubs[6UL]"))
        ctx = self.plan_reuse()
        self.assertEqual(ctx.problems, [])
        self.assertIn("== ((unsigned int)CLUB_BASEBALL)", ctx.tree.read(self.path))
        self.assertIn("clubs[((unsigned long int)CLUB_BASEBALL)]", ctx.tree.read(self.path))
        ctx.tree.flush()
        self.assertEqual(self.plan_reuse().problems, [])

    def test_ambiguous_same_value_names_are_refused(self):
        values = R.parse_values("CLUB_BASEBALL 6\nCLUB_ALIAS 6\n", "values")
        ctx = self.plan_reuse(values=values)
        self.assertTrue(any("same-value" in p for p in ctx.problems), ctx.problems)
        self.assertEqual(ctx.tree.changed(), [])

    def test_wrong_existing_value_and_wrong_enum_are_refused(self):
        for values, enum in ((R.parse_values("CLUB_BASEBALL 5", "values"), "Club"), (self.values, "OtherClub")):
            ctx = self.plan_reuse(values=values, enum=enum)
            self.assertTrue(ctx.problems)
            self.assertEqual(ctx.tree.changed(), [])

    def test_wrong_header_owner_is_refused(self):
        ctx = self.plan_reuse(header="include/ovl/AAA.h")
        self.assertTrue(any("own/see" in p for p in ctx.problems), ctx.problems)
        self.assertEqual(ctx.tree.changed(), [])

    def test_enum_initializer_type_mismatch_is_refused(self):
        self.write("include/common.h", self.read("include/common.h").replace("CLUB_BASEBALL = 6", "CLUB_BASEBALL = 6U"))
        ctx = self.plan_reuse()
        self.assertTrue(any("range/type" in p for p in ctx.problems), ctx.problems)

    def test_other_enum_member_cannot_force_an_unsupported_range(self):
        header = self.read("include/common.h")
        for initializer in ("0x80000000U", "-1", "CLUB_BASEBALL + 1"):
            self.write("include/common.h", header.replace("CLUB_NONE = 11", "CLUB_NONE = " + initializer))
            ctx = self.plan_reuse()
            self.assertTrue(any("range/type" in p for p in ctx.problems), ctx.problems)
            self.assertEqual(ctx.tree.changed(), [])
        self.write("include/common.h", header)

    def test_local_or_macro_shadow_is_refused(self):
        for shadow in ("s32 CLUB_BASEBALL = 9;", "\n#define CLUB_BASEBALL 9\n"):
            self.write(self.path, self.source.replace("s32 unrelated = 6;", shadow + "\ns32 unrelated = 6;"))
            ctx = self.plan_reuse()
            self.assertTrue(any("shadow" in p for p in ctx.problems), ctx.problems)
            self.assertEqual(ctx.tree.changed(), [])

    def test_repeated_expression_requires_explicit_occurrence(self):
        self.write(self.path, self.source.replace("    return unrelated", "    if ((gs->player_flags.h >> 12) == 6) girl++;\n    return unrelated"))
        ctx = self.plan_reuse(sites=self.sites[:1])
        self.assertTrue(any("2 matches" in p for p in ctx.problems), ctx.problems)
        ctx = self.plan_reuse(sites=[self.sites[0].replace("club_rule:", "club_rule@1:")])
        self.assertEqual(ctx.problems, [])
        text = ctx.tree.read(self.path)
        self.assertEqual(text.count("== CLUB_BASEBALL"), 1)
        self.assertEqual(text.count("== 6"), 1)

    def test_expression_boundary_and_missing_function_are_refused(self):
        self.write(self.path, self.source.replace("== 6 &&", "== 6 + 1 &&"))
        ctx = self.plan_reuse(sites=self.sites[:1])
        self.assertTrue(any("disagree" in p for p in ctx.problems), ctx.problems)
        ctx = self.plan_reuse(sites=[self.sites[0].replace("club_rule:", "other_rule:")])
        self.assertTrue(ctx.problems)
        self.assertEqual(ctx.tree.changed(), [])

    def test_failed_proof_restores_pending_tree(self):
        def fail(ctx):
            ctx.problems.append("synthetic gcc proof failure")
        with unittest.mock.patch.object(R, "gcc_proof", fail):
            ctx = self.plan_reuse()
        self.assertTrue(ctx.problems)
        self.assertEqual(ctx.tree.changed(), [])
        self.assertEqual(self.read(self.path), self.source)

    def test_disabled_exact_expression_cannot_stand_in_for_active_macro(self):
        original = "    if ((gs->player_flags.h >> 12) == 6 && (D_800E6280.clubs[6].level & 15) >= 8) return 1;"
        replacement = "#define LIVE_CLUB (gs->player_flags.h >> 12)\n#if 0\n" \
            "    if ((gs->player_flags.h >> 12) == 6) return 9;\n#endif\n" \
            "    if (LIVE_CLUB == 6 && (D_800E6280.clubs[6].level & 15) >= 8) return 1;"
        source = self.source.replace(original, replacement)
        self.assertNotEqual(source, self.source)
        self.write(self.path, source)
        ctx = self.plan_reuse(sites=self.sites[:1])
        self.assertTrue(any("provenance" in p for p in ctx.problems), ctx.problems)
        self.assertEqual(ctx.tree.changed(), [])
        self.assertEqual(self.read(self.path), source)

    def test_discarded_macro_argument_cannot_supply_same_line_provenance(self):
        definitions = "#define DROP(x)\n#define LIVE_CLUB (gs->player_flags.h >> 12)\n"
        original = "    if ((gs->player_flags.h >> 12) == 6 && (D_800E6280.clubs[6].level & 15) >= 8) return 1;"
        replacement = "    DROP((gs->player_flags.h >> 12) == 6); if (LIVE_CLUB == 6) return 1;"
        source = definitions + self.source.replace(original, replacement)
        self.write(self.path, source)
        ctx = self.plan_reuse(sites=self.sites[:1])
        self.assertTrue(any("provenance" in p for p in ctx.problems), ctx.problems)
        self.assertEqual(ctx.tree.changed(), [])
        self.assertEqual(self.read(self.path), source)

    def test_inactive_function_cannot_match_identical_active_header_function(self):
        start = self.source.index("s32 club_rule(void)")
        function = self.source[start:]
        self.write("include/game.h", self.read("include/game.h") + "\n" + function)
        source = self.source[:start] + "#if 0\n" + function + "#endif\n"
        self.write(self.path, source)
        ctx = self.plan_reuse(sites=self.sites[:1])
        self.assertTrue(any("provenance" in p for p in ctx.problems), ctx.problems)
        self.assertEqual(ctx.tree.changed(), [])
        self.assertEqual(self.read(self.path), source)

    def test_apply_ledger_check_and_repeated_replay(self):
        ctx = self.plan_reuse()
        self.assertEqual(ctx.problems, [])
        row = R.reuse_ledger_row(ctx.tree, "include/common.h", "Club", self.values, self.sites, "T-27068 proven IDs")
        R.apply_reuse(ctx, row, "reuse", io.StringIO())
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)
        applied = self.read(self.path)
        self.write(self.path, self.source)
        self.assertNotEqual(R.check(self.root, out=io.StringIO()), 0)
        self.assertEqual(R.replay(self.root, out=io.StringIO()), 0)
        self.assertEqual(self.read(self.path), applied)
        self.assertEqual(R.replay(self.root, out=io.StringIO()), 0)
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)

    def test_failed_post_write_check_rolls_back_source_and_ledger(self):
        ctx = self.plan_reuse()
        row = R.reuse_ledger_row(ctx.tree, "include/common.h", "Club", self.values, self.sites, "T-27068")
        with unittest.mock.patch.object(R, "check", return_value=1):
            with self.assertRaises(R.RenameError):
                R.apply_reuse(ctx, row, "reuse", io.StringIO())
        self.assertEqual(self.read(self.path), self.source)
        self.assertIsNone(self.read("config/renames/reuse.txt"))

    def test_reuse_metadata_follows_function_enum_and_header_ledgers(self):
        enum = "enum Club { CLUB_BASEBALL = 6, CLUB_NONE = 11 };"
        self.write("include/clubs.h", "#ifndef CLUBS_H\n#define CLUBS_H\n%s\n#endif\n" % enum)
        self.write("include/common.h", self.read("include/common.h").replace(enum, '#include "clubs.h"'))
        self.write("config/symbol_addrs.txt", self.read("config/symbol_addrs.txt") +
                   "\nclub_rule = 0x80041080; // type:func\n")
        ctx = self.plan_reuse(header="include/clubs.h")
        self.assertEqual(ctx.problems, [])
        row = R.reuse_ledger_row(ctx.tree, "include/clubs.h", "Club", self.values, self.sites, "T-27068")
        R.apply_reuse(ctx, row, "reuse", io.StringIO())
        self.ok("func main club_rule readable_club_rule\n", verify=True)
        self.ok("type main Club ActivityClub\n", verify=True)
        self.ok("enumerator main CLUB_BASEBALL CLUB_SPORT_BASEBALL\n", verify=True)
        self.ok("file include clubs.h shared.h\n", verify=True)
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)
        self.assertEqual(R.replay(self.root, out=io.StringIO()), 0)

    def test_const_cli_reuse_and_legacy_const_are_separate(self):
        self.write("values.txt", "CLUB_BASEBALL 6\n")
        argv = ["--root", self.root, "const", "--header", "include/common.h", "--reuse", "Club",
                "--values", os.path.join(self.root, "values.txt"), "--site", self.sites[0], "--apply"]
        with unittest.mock.patch("sys.stdout", io.StringIO()):
            self.assertEqual(R.main(argv), 0)
        self.assertIn("== CLUB_BASEBALL", self.read(self.path))


class SignedConstantTest(Fixture):
    def setUp(self):
        super().setUp()
        self.path = "src/main/80041000.c"
        self.sites = ["direction", "main:signed_sink(arg0)"]
        self.values = R.parse_values("SIGNED_NEGATIVE_ONE -1\n", "values")
        self.body = "\nvoid signed_sink(s32 value) { (void)value; }\n" \
            "s32 signed_rule(void) {\n    s32 direction;\n    direction = -1;\n" \
            "    if (direction == -(1)) signed_sink(-1);\n" \
            "    signed_sink(-(1));\n    signed_sink(-((1L)));\n" \
            "    signed_sink(-1U);\n    signed_sink(-((1UL)));\n    return -1;\n}\n"
        self.before = self.read(self.path) + self.body
        self.write(self.path, self.before)

    def plan_signed(self, values=None, sites=None, defines=True, existing=False):
        ctx = R.Context(self.root)
        R.signed_const_plan(ctx, "include/common.h", None if defines else "SignedDirection", defines,
                            self.values if values is None else values, self.sites if sites is None else sites,
                            "T-27070", existing=existing)
        return ctx

    def test_whole_comparison_assignment_call_and_parenthesized_operands(self):
        ctx = self.plan_signed()
        self.assertEqual(ctx.problems, [])
        text = ctx.tree.read(self.path)
        self.assertIn("direction = SIGNED_NEGATIVE_ONE;", text)
        self.assertIn("direction == SIGNED_NEGATIVE_ONE", text)
        self.assertIn("signed_sink(SIGNED_NEGATIVE_ONE);", text)
        self.assertIn("signed_sink(((long int)SIGNED_NEGATIVE_ONE));", text)
        self.assertIn("signed_sink(((unsigned int)SIGNED_NEGATIVE_ONE));", text)
        self.assertIn("signed_sink(((unsigned long int)SIGNED_NEGATIVE_ONE));", text)
        self.assertNotIn("-SIGNED_NEGATIVE_ONE", text)
        self.assertIn("return -1;", text)

    def test_parentheses_of_call_and_outer_grouping_are_preserved(self):
        self.write(self.path, self.before.replace("signed_sink(-1);", "signed_sink(((-((1)))));"))
        ctx = self.plan_signed(sites=self.sites[1:])
        self.assertEqual(ctx.problems, [])
        self.assertIn("signed_sink(((SIGNED_NEGATIVE_ONE)));", ctx.tree.read(self.path))

    def test_caller_scoped_signed_sites_keep_suffix_and_replay(self):
        sites = ["in:main:signed_rule:direction", "in:main:signed_rule:main:signed_sink(arg0)"]
        ctx = self.plan_signed(sites=sites)
        self.assertEqual(ctx.problems, [])
        changed = ctx.tree.read(self.path)
        self.assertIn("signed_sink(((unsigned long int)SIGNED_NEGATIVE_ONE));", changed)
        row = R.signed_ledger_row(ctx.tree, "include/common.h", None, True, self.values, sites, "T-27072")
        R.apply_reuse(ctx, row, "caller", io.StringIO())
        applied = self.read(self.path)
        self.write(self.path, self.before)
        self.assertNotEqual(R.check(self.root, out=io.StringIO()), 0)
        self.assertEqual(R.replay(self.root, out=io.StringIO()), 0)
        self.assertEqual(self.read(self.path), applied)
        self.assertEqual(R.replay(self.root, out=io.StringIO()), 0)
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)

    def test_independent_integer_macros_normalize_but_remain_untouched(self):
        self.write("include/common.h", self.read("include/common.h") +
                   "\n#define OTHER_ZERO 0\n#define OTHER_THREE 3\n")
        source = self.before.replace("return -1;", "signed_sink(OTHER_ZERO); if (1 == OTHER_THREE) {} return -1;")
        self.write(self.path, source)
        ctx = self.plan_signed(sites=["direction"])
        self.assertEqual(ctx.problems, [], ctx.problems)
        text = ctx.tree.read(self.path)
        self.assertIn("direction = SIGNED_NEGATIVE_ONE;", text)
        self.assertIn("signed_sink(OTHER_ZERO);", text)
        self.assertIn("1 == OTHER_THREE", text)

    def test_foreign_macro_selected_as_a_site_fails_closed(self):
        self.write("include/common.h", self.read("include/common.h") + "\n#define OTHER_ZERO 0\n")
        source = self.before.replace("return -1;", "signed_sink(OTHER_ZERO); return -1;")
        self.write(self.path, source)
        values = R.parse_values("OTHER_ZERO_NAME 0\n", "values")
        ctx = R.Context(self.root)
        R.signed_const_plan(ctx, "include/common.h", None, True, values,
                            ["main:signed_sink(arg0)"], "T-27073", verify=False)
        self.assertTrue(any("foreign macro" in problem for problem in ctx.problems), ctx.problems)
        self.assertEqual(ctx.tree.changed(), [])
        self.assertEqual(self.read(self.path), source)

    def test_positive_macro_families_coexist_through_cli_replay(self):
        import contextlib
        import io
        path = "src/main/80041000.c"
        source = self.read(path) + "\ns32 macro_coexist(void) { s32 score_scale; s32 failure_count; " \
                 "score_scale = 16; if (score_scale == 16) {} signed_sink(3); " \
                 "failure_count = 0; if (failure_count == 3) {} return 0; }\n"
        self.write(path, source)
        self.write("scale.values", "HUD_PROBE_SCALE 3\n")
        self.write("failure.values", "HUD_FAILURES_NONE 0\nHUD_FAILURES_ALL_THREE 3\n")
        prefix = ["--root", self.root, "const", "--header", "include/common.h", "--defines"]
        first = prefix + ["--values", os.path.join(self.root, "scale.values"),
                         "--site", "in:main:macro_coexist:main:signed_sink(arg0)", "--apply"]
        second = prefix + ["--values", os.path.join(self.root, "failure.values"),
                          "--site", "in:main:macro_coexist:failure_count", "--apply"]
        with contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(R.main(first), 0)
            self.assertEqual(R.main(second), 0)
        applied = self.read(path)
        self.assertIn("signed_sink(HUD_PROBE_SCALE);", applied)
        self.assertIn("failure_count = HUD_FAILURES_NONE;", applied)
        self.assertIn("failure_count == HUD_FAILURES_ALL_THREE", applied)
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)
        with contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(R.main(["--root", self.root, "replay"]), 0)
        self.assertEqual(self.read(path), applied)
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)
        self.write(path, applied.replace("failure_count == HUD_FAILURES_ALL_THREE",
                                         "failure_count == 3", 1))
        stale = self.read(path)
        self.assertNotEqual(R.check(self.root, out=io.StringIO()), 0)
        with contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(R.main(["--root", self.root, "replay"]), 0)
        self.assertEqual(self.read(path), applied)
        self.assertIn("signed_sink(HUD_PROBE_SCALE);", self.read(path))
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)
        with contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(R.main(["--root", self.root, "replay"]), 0)
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)

    def test_ambiguous_or_function_like_foreign_macro_is_not_normalized(self):
        for definitions, use in (("#define OTHER_VALUE 3\n#define OTHER_VALUE 3\n", "OTHER_VALUE"),
                                 ("#define OTHER_VALUE(x) (x)\n", "OTHER_VALUE(3)")):
            self.write("include/common.h", self.read("include/common.h") + "\n" + definitions)
            source = self.before.replace("return -1;", "signed_sink(%s); return -1;" % use)
            self.write(self.path, source)
            values = R.parse_values("SIGNED_NEGATIVE_ONE -1\n", "values")
            ctx = R.Context(self.root)
            R.signed_const_plan(ctx, "include/common.h", None, True, values,
                                ["main:signed_sink(arg0)"], "T-27073", verify=False)
            self.assertTrue(any("provenance" in problem or "tokens changed" in problem
                                for problem in ctx.problems), ctx.problems)
            self.assertEqual(ctx.tree.changed(), [])
            self.assertEqual(self.read(self.path), source)
            self.write("include/common.h", self.read("include/common.h").replace(definitions, ""))

    def test_compiler_checks_value_and_suffix_type_of_replacement(self):
        import subprocess
        prefix = self.before[:self.before.index("s32 signed_rule(void)")]
        for original in ("-1", "-(1)", "-((1L))", "-1U", "-((1UL))"):
            self.write(self.path, prefix + "s32 signed_rule(void) {\n    signed_sink(%s);\n    return 0;\n}\n" % original)
            ctx = self.plan_signed(sites=self.sites[1:])
            self.assertEqual(ctx.problems, [])
            import re
            expression = re.search(r"^    signed_sink\((.*)\);$", ctx.tree.read(self.path), re.M).group(1)
            source = "#define SIGNED_NEGATIVE_ONE -1\nint main(void) {\n" \
                "if (sizeof(%s) != sizeof(%s)) return 1;\n" \
                "if (((%s) < 0) != ((%s) < 0)) return 2;\n" \
                "if ((%s) != (%s)) return 3;\nreturn 0;\n}\n" % (expression, original, expression, original, expression, original)
            self.write("probe.c", source)
            binary = os.path.join(self.root, "probe")
            result = subprocess.run(["gcc", "-std=c89", os.path.join(self.root, "probe.c"), "-o", binary],
                                    stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertEqual(subprocess.run([binary]).returncode, 0)

    def test_negative_hex_graph_scale_and_unselected_values(self):
        self.values = R.parse_values("HUD_BIORHYTHM_GRAPH_SCALE -32", "values")
        body = "\ns32 graph(void) { signed_sink(-0x20); signed_sink(-(0x20)); signed_sink(-((0x20L))); return 7; }\n"
        self.write(self.path, self.read(self.path) + body)
        ctx = self.plan_signed(sites=self.sites[1:])
        self.assertEqual(ctx.problems, [])
        text = ctx.tree.read(self.path)
        self.assertEqual(text.count("signed_sink(HUD_BIORHYTHM_GRAPH_SCALE);"), 2)
        self.assertIn("signed_sink(((long int)HUD_BIORHYTHM_GRAPH_SCALE));", text)
        self.assertIn("signed_sink(-1);", text)
        self.assertIn("return 7;", text)

    def test_mixed_positive_and_negative_values_preserve_types(self):
        self.write(self.path, self.before.replace("return -1;", "signed_sink(3U); return -1;"))
        values = R.parse_values("SIGNED_NEGATIVE_ONE -1\nSIGNED_POSITIVE_THREE 3", "values")
        ctx = self.plan_signed(values=values)
        self.assertEqual(ctx.problems, [])
        self.assertIn("signed_sink(((unsigned int)SIGNED_POSITIVE_THREE));", ctx.tree.read(self.path))

    def test_preprocessing_or_inactive_raw_expression_fails_closed(self):
        for replacement in (
                "#define LIVE_MINUS -1\n#if 0\n    signed_sink(-1);\n#endif\n    signed_sink(LIVE_MINUS);",
                "#define DROP(x)\n#define LIVE_MINUS -1\n    DROP(signed_sink(-1)); signed_sink(LIVE_MINUS);",):
            source = self.before.replace("    signed_sink(-(1));", replacement)
            self.write(self.path, source)
            ctx = self.plan_signed(sites=self.sites[1:])
            self.assertTrue(any("preprocessing" in p for p in ctx.problems), ctx.problems)
            self.assertEqual(ctx.tree.changed(), [])
            self.assertEqual(self.read(self.path), source)

    def test_ambiguous_values_and_unsupported_ranges_fail_closed(self):
        for values in ("SIGNED_NEGATIVE_ONE -1\nSIGNED_OTHER_NEGATIVE_ONE -1", "SIGNED_TOO_LOW -2147483648"):
            ctx = self.plan_signed(values=R.parse_values(values, "values"))
            self.assertTrue(ctx.problems)
            self.assertEqual(ctx.tree.changed(), [])

    def test_proof_failure_rolls_back_pending_source_and_definition(self):
        def fail(ctx):
            ctx.problems.append("synthetic signed proof failure")
        with unittest.mock.patch.object(R, "gcc_proof", fail):
            ctx = self.plan_signed()
        self.assertTrue(ctx.problems)
        self.assertEqual(ctx.tree.changed(), [])
        self.assertEqual(self.read(self.path), self.before)

    def test_apply_check_stale_restore_and_repeated_replay(self):
        for defines in (True, False):
            if not defines:
                self.tearDown()
                self.setUp()
            ctx = self.plan_signed(defines=defines)
            self.assertEqual(ctx.problems, [])
            row = R.signed_ledger_row(ctx.tree, "include/common.h", None if defines else "SignedDirection", defines,
                                      self.values, self.sites, "T-27070")
            R.apply_reuse(ctx, row, "signed", io.StringIO())
            after = self.read(self.path)
            self.assertEqual(R.check(self.root, out=io.StringIO()), 0)
            self.write(self.path, self.before)
            self.assertNotEqual(R.check(self.root, out=io.StringIO()), 0)
            self.assertEqual(R.replay(self.root, out=io.StringIO()), 0)
            self.assertEqual(self.read(self.path), after)
            self.assertEqual(R.replay(self.root, out=io.StringIO()), 0)

    def test_post_write_failure_rolls_back_source_header_and_ledger(self):
        header = self.read("include/common.h")
        ctx = self.plan_signed()
        row = R.signed_ledger_row(ctx.tree, "include/common.h", None, True, self.values, self.sites, "T-27070")
        with unittest.mock.patch.object(R, "check", return_value=1):
            with self.assertRaises(R.RenameError):
                R.apply_reuse(ctx, row, "signed", io.StringIO())
        self.assertEqual(self.read(self.path), self.before)
        self.assertEqual(self.read("include/common.h"), header)
        self.assertIsNone(self.read("config/renames/signed.txt"))

    def test_enum_request_follows_function_tag_and_member_ledgers(self):
        self.write("config/symbol_addrs.txt", self.read("config/symbol_addrs.txt") +
                   "\nsigned_sink = 0x80041080; // type:func\n")
        ctx = self.plan_signed(defines=False)
        self.assertEqual(ctx.problems, [])
        row = R.signed_ledger_row(ctx.tree, "include/common.h", "SignedDirection", False,
                                  self.values, self.sites, "T-27070")
        R.apply_reuse(ctx, row, "signed", io.StringIO())
        self.ok("func main signed_sink semantic_sink\n", verify=True)
        self.ok("type main SignedDirection SignedTestDirection\n", verify=True)
        self.ok("enumerator main SIGNED_NEGATIVE_ONE SIGNED_OTHER_NEGATIVE_ONE\n", verify=True)
        self.assertEqual(R.check(self.root, out=io.StringIO()), 0)
        self.assertEqual(R.replay(self.root, out=io.StringIO()), 0)

    def test_check_rejects_inactive_signed_definition(self):
        ctx = self.plan_signed()
        self.assertEqual(ctx.problems, [])
        row = R.signed_ledger_row(ctx.tree, "include/common.h", None, True, self.values, self.sites, "T-27070")
        R.apply_reuse(ctx, row, "signed", io.StringIO())
        header = self.read("include/common.h")
        self.write("include/common.h", header.replace("#define SIGNED_NEGATIVE_ONE -1",
                   "#if 0\n#define SIGNED_NEGATIVE_ONE -1\n#endif"))
        self.assertNotEqual(R.check(self.root, out=io.StringIO()), 0)

    def test_legacy_cli_automatically_routes_negative_values(self):
        self.write("negative.values", "SIGNED_NEGATIVE_ONE -1\n")
        argv = ["--root", self.root, "const", "--header", "include/common.h", "--defines", "--values",
                os.path.join(self.root, "negative.values"), "--site", "main:signed_sink(arg0)", "--apply"]
        with unittest.mock.patch("sys.stdout", io.StringIO()):
            self.assertEqual(R.main(argv), 0)
        self.assertIn("signed_sink(SIGNED_NEGATIVE_ONE)", self.read(self.path))


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

    def test_event_range_in_obin_and_hypotheses_files(self):
        """Every yaml loads the O.BIN, SDK and hypotheses files (T-27003): a main name in EVENT's range there is refused."""
        for yaml in ("config/SLPM_86.053.yaml", "config/overlays/AAA.yaml", "config/overlays/EVT.yaml"):
            self.write(yaml, self.read(yaml).replace(
                "    - config/symbol_addrs.txt\n",
                "    - config/symbol_addrs.txt\n    - config/symbol_addrs_obin.txt\n    - config/symbol_addrs_hypotheses.txt\n", 1))
        self.write("config/symbol_addrs_obin.txt", "obin_ok = 0x80041200; // type:func\nobin_evt = 0x800F6300;\n")
        self.write("config/symbol_addrs_hypotheses.txt", "// HYPOTHESES\nhyp_evt = 0x800F6400;\n")
        p = "\n".join(symtab.SymbolTable(self.root).problems())
        self.assertIn("config/symbol_addrs_obin.txt:2: obin_evt = 0x800F6300 lies in the range of EVT", p)
        self.assertIn("config/symbol_addrs_hypotheses.txt:2: hyp_evt = 0x800F6400 lies in the range of EVT", p)
        self.assertNotIn("obin_ok", p)
        self.write("config/symbol_addrs_obin.txt", "obin_ok = 0x80041200; // type:func\n")
        self.write("config/symbol_addrs_hypotheses.txt", "// HYPOTHESES\n")
        self.assertEqual(symtab.SymbolTable(self.root).problems(), [])

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


class EssentialSelectorFormsTest(Fixture):
    def test_indexed_member_is_opt_in_and_exact(self):
        from pycparser import c_parser
        ast = c_parser.CParser().parse("""
            void f(void) {
                if (gs->flags.b[1] & 0x10U) {}
                if (gs->flags.b[0] & 2) {}
                if (gs->other.b[1] & 4) {}
            }
        """)
        indexed = [R.parse_site("*.b[1]")]
        hits = R.const_hits(ast, indexed, R.CMP_OPS + R.BIT_OPS)
        self.assertEqual([value for _node, value in hits], [16, 4])
        old_wildcard = [R.parse_site("*.b")]
        self.assertEqual(R.const_hits(ast, old_wildcard, R.CMP_OPS + R.BIT_OPS), [])
        with self.assertRaises(R.RenameError):
            R.parse_site("Flags.b[0x1]")

    def test_compound_add_matches_only_plus_equals(self):
        from pycparser import c_parser
        ast = c_parser.CParser().parse("""
            void f(void) {
                gs->mode_step += 6;
                gs->mode_step -= 6;
                if (gs->mode_step == 6) {}
                switch (gs->mode_step) { case 6: break; }
            }
        """)
        sites = [R.parse_site("add:*.mode_step")]
        hits = R.const_hits(ast, sites, R.CMP_OPS)
        self.assertEqual([value for _node, value in hits], [6])
        legacy_hits = R.const_hits(ast, [R.parse_site("*.mode_step")], R.CMP_OPS)
        self.assertEqual([value for _node, value in legacy_hits], [6, 6])
        values = R.parse_values("STEP_DELTA 6\n", "values")
        _tree, _report, problems = R.const_plan(self.root, "include/game.h", "GameMode", False,
                                                values, ["add:*.mode_step"])
        self.assertTrue(any("exact caller wrapper" in p for p in problems), problems)

    def test_wildcard_index_requires_caller_and_signed_metadata_roundtrips(self):
        values = R.parse_values("FLAG 1\n", "values")
        _tree, _report, problems = R.const_plan(self.root, "include/game.h", "GameMode", False,
                                                values, ["*.b[1]"])
        self.assertTrue(any("exact caller wrapper" in p for p in problems), problems)
        _tree, _report, problems = R.const_plan(self.root, "include/game.h", "GameMode", False,
                                                values, ["add:*.mode_step"])
        self.assertTrue(any("exact caller wrapper" in p for p in problems), problems)
        for selector in ("in:ENDING:career_result:*.b[1]", "in:SHUGAKU:saved_step:add:*.mode_step"):
            self.assertEqual(R.site_text(R.parse_site(selector)), selector)

    def test_index_selector_respects_exact_caller_and_local_alias(self):
        game = self.read("include/game.h")
        game = game.replace("typedef struct GameState {", "typedef struct FlagBytes { s16 b[2]; } FlagBytes;\n"
                            "typedef struct GameState {\n    FlagBytes player_flags_upper;")
        self.write("include/game.h", game)
        path = "src/main/80041000.c"
        original = self.read(path)
        self.write(path, original + "\nvoid ending_career_result_select(void) {\n"
                   "    GameState *gs = &D_800E6280;\n"
                   "    if (gs->player_flags_upper.b[1] & 1) {}\n"
                   "    if (gs->player_flags_upper.b[0] & 2) {}\n}\n"
                   "void unrelated_result(void) { if (D_800E6280.player_flags_upper.b[1] & 1) {} }\n")
        values = R.parse_values("CAREER_RESULT_FLAG 1\n", "values")
        tree, _report, problems = R.const_plan(self.root, "include/game.h", None, True,
                                               values, ["in:main:ending_career_result_select:*.b[1]"],
                                               _signed=True)
        self.assertEqual(problems, [])
        changed = tree.read(path)
        self.assertIn("b[1] & CAREER_RESULT_FLAG", changed)
        self.assertIn("b[0] & 2", changed)
        self.assertIn("unrelated_result(void) { if (D_800E6280.player_flags_upper.b[1] & 1)", changed)

    def test_add_selector_rewrites_only_compound_rvalue_and_replays(self):
        game = self.read("include/game.h").replace("/* 0x02 */ s16 unk_02;", "/* 0x02 */ s16 unk_02;\n"
                                                       "    s32 mode_step; s32 saved_stage;")
        self.write("include/game.h", game)
        path = "src/main/80041000.c"
        original = self.read(path)
        body = "\nvoid trip_saved_mode_step_advance(void) {\n"
        body += "    D_800E6280.mode_step += 6;\n    D_800E6280.mode_step -= 6;\n"
        body += "    if (D_800E6280.mode_step == 6) {}\n"
        body += "    switch (D_800E6280.mode_step) { case 6: break; }\n}\n"
        self.write(path, original + body)
        values = R.parse_values("TRIP_SAVED_MODE_STEP_DELTA 6\n", "values")
        sites = ["in:main:trip_saved_mode_step_advance:add:*.mode_step"]
        ctx = R.Context(self.root)
        R.signed_const_plan(ctx, "include/game.h", "TripSavedModeStepDelta", False, values, sites,
                            "T-27075", verify=False)
        self.assertEqual(ctx.problems, [], ctx.problems)
        changed = ctx.tree.read(path)
        self.assertIn("mode_step += TRIP_SAVED_MODE_STEP_DELTA", changed)
        self.assertIn("mode_step -= 6", changed)
        self.assertIn("mode_step == 6", changed)
        self.assertIn("case 6:", changed)
        row = R.signed_ledger_row(ctx.tree, "include/game.h", "TripSavedModeStepDelta", False,
                                  values, sites, "T-27075")
        R.apply_reuse(ctx, row, "caller", io.StringIO())
        applied = self.read(path)
        self.write(path, original + body)
        self.assertNotEqual(R.check(self.root, out=io.StringIO()), 0)
        self.assertEqual(R.replay(self.root, out=io.StringIO()), 0)
        self.assertEqual(self.read(path), applied)

    def test_signed_metadata_selector_serialization_tracks_function_alias(self):
        class Owner:
            def header_owner(self, _header):
                return "main"
        values = R.parse_values("ONE 1\n", "values")
        site = "in:main:ending_career_result_select:*.b[1]"
        row = R.signed_ledger_row(Owner(), "include/game.h", "Flag", False, values, [site], "T-27075")
        alias = R.Row("func", "main", "ending_career_result_select", "career_result_choose",
                      key="0x80041010")
        change_map = {("sym", "main", "0x80041010", 0):
                      (["ending_career_result_select", "career_result_choose"], [alias])}
        metadata = R.signed_request(row, change_map)
        self.assertEqual(metadata["sites"], ["in:main:career_result_choose:*.b[1]"])

    def test_qualified_owner_is_refused_and_foreign_type_alias_is_ignored(self):
        class Owner:
            def header_owner(self, _header):
                return "main"
        values = R.parse_values("ONE 1\n", "values")
        with self.assertRaises(R.RenameError):
            R.parse_site("in:main:career_result:FlagBytes.b[1]")
        site = "in:main:career_result:*.b[1]"
        row = R.signed_ledger_row(Owner(), "include/game.h", "Flag", False, values, [site], "T-27075")
        unrelated = R.Row("type", "AAA", "FlagBytes", "OtherOverlayFlags", key="-")
        change_map = {("type", "AAA", "-", 0): (["FlagBytes", "OtherOverlayFlags"], [unrelated])}
        metadata = R.signed_request(row, change_map)
        self.assertEqual(metadata["sites"], [site])

    def test_macro_owner_selector_refusal_leaves_all_paths_unchanged(self):
        header_path = "include/game.h"
        source_path = "src/main/80041000.c"
        header = self.read(header_path)
        header = header.replace("#endif", "typedef struct FlagBytes { int b[2]; } FlagBytes;\n"
                                "#define OTHER_FIELDS int b[2];\n"
                                "typedef struct OtherBytes { OTHER_FIELDS } OtherBytes;\n#endif")
        self.write(header_path, header)
        source = self.read(source_path) + "\nvoid rule(FlagBytes *a, OtherBytes *o) {\n"
        source += "    if (a->b[1] & 1) {}\n    if (o->b[1] & 1) {}\n}\n"
        self.write(source_path, source)
        before = {header_path: self.read(header_path), source_path: self.read(source_path)}
        ctx = R.Context(self.root)
        R.signed_const_plan(ctx, header_path, None, True, R.parse_values("MASK 1\n", "values"),
                            ["in:main:rule:FlagBytes.b[1]"], "T-27075", verify=False)
        self.assertTrue(ctx.problems, "unsupported typed selector must fail closed")
        self.assertEqual(ctx.tree.changed(), [])
        self.assertEqual({path: self.read(path) for path in before}, before)
        self.assertFalse(os.path.exists(os.path.join(self.root, "config/renames/essential-selector.txt")))

    def test_same_unit_field_rename_makes_restored_literal_stale_and_refuses(self):
        header_path = "include/game.h"
        source_path = "src/main/80041000.c"
        header = self.read(header_path)
        header = header.replace("typedef struct GameState {", "typedef struct FlagBytes { s16 b[2]; } FlagBytes;\n"
                                "typedef struct GameState {\n    FlagBytes flags;")
        self.write(header_path, header)
        source = self.read(source_path) + "\nvoid caller(void) { if (D_800E6280.flags.b[1] & 1) {} }\n"
        self.write(source_path, source)
        values = R.parse_values("FLAG_MASK 1\n", "values")
        sites = ["in:main:caller:*.b[1]"]
        ctx = R.Context(self.root)
        R.signed_const_plan(ctx, header_path, None, True, values, sites, "T-27075", verify=False)
        self.assertEqual(ctx.problems, [], ctx.problems)
        row = R.signed_ledger_row(ctx.tree, header_path, None, True, values, sites, "T-27075")
        R.apply_reuse(ctx, row, "caller", io.StringIO())
        self.assertIn("b[1] & FLAG_MASK", self.read(source_path))

        self.ok("field main:FlagBytes b bytes\n", verify=False)
        renamed = self.read(source_path)
        self.assertIn("bytes[1] & FLAG_MASK", renamed)
        self.write(source_path, renamed.replace("bytes[1] & FLAG_MASK", "bytes[1] & 1"))
        restored = self.read(source_path)
        header_after = self.read(header_path)
        ledger_dir = os.path.join(self.root, "config/renames")
        def ledger_snapshot():
            snapshot = {}
            for name in os.listdir(ledger_dir):
                path = os.path.join(ledger_dir, name)
                if os.path.isfile(path):
                    with open(path) as ledger:
                        snapshot[name] = ledger.read()
            return snapshot
        ledgers_before = ledger_snapshot()
        self.assertNotEqual(R.check(self.root, out=io.StringIO()), 0)
        self.assertEqual(R.replay(self.root, out=io.StringIO()), 1)
        self.assertEqual(self.read(source_path), restored)
        self.assertEqual(self.read(header_path), header_after)
        self.assertEqual(ledger_snapshot(), ledgers_before)

    def test_foreign_field_alias_does_not_block_wildcard_metadata(self):
        class Owner:
            def header_owner(self, _header):
                return "main"
        values = R.parse_values("ONE 1\n", "values")
        site = "in:main:career_result:*.b[1]"
        row = R.signed_ledger_row(Owner(), "include/game.h", None, True, values, [site], "T-27075")
        foreign = R.Row("field", "AAA:FlagBytes", "b", "bytes", key="-")
        changes = {("field", "AAA:FlagBytes", "-", 0): (["b", "bytes"], [foreign])}
        metadata = R.signed_request(row, changes, R.Tree(self.root))
        self.assertEqual(metadata["sites"], [site])

    def test_main_field_alias_is_relevant_to_overlay_caller(self):
        source_path = "src/ovl/AAA/80132000.c"
        game_header = self.read("include/game.h").replace(
            "typedef struct GameState {", "typedef struct FlagBytes { s16 b[2]; } FlagBytes;\n"
            "typedef struct GameState {")
        self.write("include/game.h", game_header)
        source = '#include "game.h"\n' + self.read(source_path)
        source += "\nvoid caller(void) { FlagBytes *flags; if (flags->b[1] & 1) {} }\n"
        self.write(source_path, source)
        values = R.parse_values("MASK 1\n", "values")
        sites = ["in:AAA:caller:*.b[1]"]
        ctx = R.Context(self.root)
        R.signed_const_plan(ctx, "include/ovl/AAA.h", None, True, values, sites, "T-27075", verify=False)
        self.assertEqual(ctx.problems, [], ctx.problems)
        row = R.signed_ledger_row(ctx.tree, "include/ovl/AAA.h", None, True, values, sites, "T-27075")
        R.apply_reuse(ctx, row, "caller", io.StringIO())
        self.assertIn("flags->b[1] & MASK", self.read(source_path))

        self.ok("field main:FlagBytes b bytes\n", verify=False)
        renamed = self.read(source_path)
        self.assertIn("flags->bytes[1] & MASK", renamed)
        self.write(source_path, renamed.replace("flags->bytes[1] & MASK", "flags->bytes[1] & 1"))
        restored = self.read(source_path)
        game_after = self.read("include/game.h")
        ledger_dir = os.path.join(self.root, "config/renames")
        def ledger_snapshot():
            snapshot = {}
            for name in os.listdir(ledger_dir):
                path = os.path.join(ledger_dir, name)
                if os.path.isfile(path):
                    with open(path) as ledger:
                        snapshot[name] = ledger.read()
            return snapshot
        ledgers_before = ledger_snapshot()
        self.assertNotEqual(R.check(self.root, out=io.StringIO()), 0)
        self.assertEqual(R.replay(self.root, out=io.StringIO()), 1)
        self.assertEqual(self.read(source_path), restored)
        self.assertEqual(self.read("include/game.h"), game_after)
        self.assertEqual(ledger_snapshot(), ledgers_before)


if __name__ == "__main__":
    unittest.main()
