#!/usr/bin/env python3
"""Tests for tools/sync_protos.py and the main_api.h rules of tools/check_headers.py (synthetic
headers and sources, no game data).

Run: tools/docker.sh python3 tools/test_sync_protos.py
"""
import json
import tempfile
import unittest
from pathlib import Path

import check_headers
import sync_protos as sp

API = """#ifndef MAIN_API_H
#define MAIN_API_H
#include "common.h"
extern u8 D_800E7388;
extern s32 D_800E7384;
void func_80042808(void);
#ifndef MAIN_API_OVERRIDE_func_80083440
void func_80083440(u8 arg0);
#endif
#endif
"""


class Repo(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        self.inc = self.root / "include"
        (self.inc / "ovl").mkdir(parents=True)
        (self.root / "src" / "main").mkdir(parents=True)
        (self.root / "src" / "ovl" / "AAA").mkdir(parents=True)
        (self.root / "config").mkdir()
        (self.root / "config" / "overlays.txt").write_text("AAA 0x80132000 0x100\nBBB 0x80132000 0x100\nEVT 0x800F6000 0x100\n")
        (self.root / "config" / "symbol_addrs.txt").write_text("SenseMouse = 0x800460E0; // type:func\n")
        self.write("common.h", "#ifndef COMMON_H\n#define COMMON_H\ntypedef signed char s8;\ntypedef unsigned char u8;\n"
                   "typedef signed short s16;\ntypedef unsigned short u16;\ntypedef signed int s32;\n"
                   "typedef unsigned int u32;\n#endif\n")
        self.write("libgpu.h", "#ifndef LIBGPU_H\n#define LIBGPU_H\n#endif\n")
        self.write("main_api.h", API)
        self.write("game.h", '#ifndef GAME_H\n#define GAME_H\n#include "common.h"\n#include "main_api.h"\n#endif\n')

    def tearDown(self):
        self.tmp.cleanup()

    def write(self, name, text):
        p = self.inc / name
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text)

    def src(self, name, text):
        p = self.root / "src" / name
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text)

    def ovl(self, name, body, head='#include "common.h"\n#include "game.h"\n'):
        self.write("ovl/%s.h" % name, "#ifndef OVL_%s_H\n#define OVL_%s_H\n%s%s#endif\n" % (name, name, head, body))

    def problems(self):
        return check_headers.check(str(self.inc), str(self.root / "src"))

    def has(self, text):
        out = self.problems()
        self.assertTrue(any(text in p for p in out), out)


class CheckApiTest(Repo):
    def test_clean(self):
        self.ovl("AAA", "extern s32 D_80140000;\nvoid func_80132000(void);\n")
        self.assertEqual(self.problems(), [])

    def test_main_symbol_in_overlay_header_must_move(self):
        self.ovl("AAA", "extern u8 D_800E7399;\n")
        self.has("declare it in include/main_api.h")

    def test_main_symbol_in_game_h_must_move(self):
        self.write("game.h", '#ifndef GAME_H\n#define GAME_H\n#include "main_api.h"\nextern u8 D_800E7399;\n#endif\n')
        self.has("main symbol D_800E7399 is declared in include/game.h")

    def test_duplicate_of_the_api(self):
        self.ovl("AAA", "extern u8 D_800E7388;\n")
        self.has("duplicate D_800E7388")

    def test_conflict_names_the_override(self):
        self.ovl("AAA", "extern s8 D_800E7388;\n")
        self.has("MAIN_API_OVERRIDE_D_800E7388")

    def test_override_hides_the_api_declaration(self):
        self.ovl("AAA", "extern s8 D_800E7388;\n", head='#define MAIN_API_OVERRIDE_D_800E7388 /* lb */\n'
                 '#include "common.h"\n#include "game.h"\n')
        self.write("main_api.h", API.replace("extern u8 D_800E7388;", "#ifndef MAIN_API_OVERRIDE_D_800E7388\n"
                                             "extern u8 D_800E7388;\n#endif"))
        self.assertEqual(self.problems(), [])

    def test_override_must_be_guarded_in_the_api(self):
        self.ovl("AAA", "extern s8 D_800E7388;\n", head='#define MAIN_API_OVERRIDE_D_800E7388 /* lb */\n'
                 '#include "common.h"\n#include "game.h"\n')
        self.has("is not guarded")

    def test_override_needs_a_reason(self):
        self.ovl("AAA", "void func_80083440(s32 a);\n", head='#define MAIN_API_OVERRIDE_func_80083440\n'
                 '#include "common.h"\n#include "game.h"\n')
        self.has("has no reason")

    def test_redundant_override(self):
        self.ovl("AAA", "void func_80083440(u8 arg0);\n", head='#define MAIN_API_OVERRIDE_func_80083440 /* why */\n'
                 '#include "common.h"\n#include "game.h"\n')
        self.has("is redundant")

    def test_override_of_unknown_symbol(self):
        self.ovl("AAA", "", head='#define MAIN_API_OVERRIDE_D_800E7777 /* why */\n#include "common.h"\n#include "game.h"\n')
        self.has("does not declare")

    def test_implicit_override_needs_no_declaration(self):
        self.ovl("AAA", "", head='#define MAIN_API_OVERRIDE_func_80083440 /* implicit declaration, as matched */\n'
                 '#include "common.h"\n#include "game.h"\n')
        self.assertEqual(self.problems(), [])
        self.ovl("AAA", "", head='#define MAIN_API_OVERRIDE_func_80083440 /* matched */\n#include "common.h"\n#include "game.h"\n')
        self.has("has no declaration there")

    def test_overlay_header_must_reach_the_api(self):
        self.ovl("AAA", "extern s32 D_80140000;\n", head='#include "common.h"\n')
        self.has("does not include main_api.h")

    def test_definition_is_the_truth(self):
        self.src("main/80042808.c", '#include "game.h"\nvoid func_80042808(s32 a) {\n}\n')
        self.has("defines it as")

    def test_k_and_r_api_entry_is_accepted_for_a_prototype_definition(self):
        self.write("main_api.h", API.replace("void func_80042808(void);", "void func_80042808();"))
        self.src("main/80042808.c", '#include "game.h"\nvoid func_80042808(s32 a) {\n}\n')
        self.assertEqual(self.problems(), [])

    def test_c_file_duplicate(self):
        self.src("ovl/AAA/80132000.c", '#include "common.h"\n#include "ovl/AAA.h"\nextern u8 D_800E7388;\n')
        self.ovl("AAA", "")
        self.has("duplicate D_800E7388: src/ovl/AAA/80132000.c")

    def test_renamed_symbol_counts_as_main(self):
        self.ovl("AAA", "void SenseMouse(u16 a, u16 b);\n")
        self.has("main symbol SenseMouse")

    def test_event_range_is_the_overlays_own_unless_declared_elsewhere(self):
        self.ovl("EVT", "extern s32 D_80120650;\n")
        self.assertEqual(self.problems(), [])
        self.ovl("AAA", "extern u8 D_80120650[];\n")
        self.has("main symbol D_80120650 is declared in include/ovl/AAA.h")

    def test_no_api_no_rules(self):
        (self.inc / "main_api.h").unlink()
        self.write("game.h", "extern u8 D_800E7399;\n")
        self.ovl("AAA", "extern u8 D_800E7399;\n")
        self.assertTrue(any(p.startswith("duplicate D_800E7399") for p in self.problems()))


class PlanTest(Repo):
    def plan(self):
        return sp.plan(sp.Model(str(self.inc), str(self.root / "src")))

    def test_definition_wins(self):
        self.write("main_api.h", API.replace("void func_80042808(void);", ""))
        self.ovl("AAA", "void func_80042808(s32 a);\n")
        self.src("main/80042808.c", '#include "game.h"\nvoid func_80042808(u8 a) {\n}\n')
        t, raw, why = self.plan()["func_80042808"]
        self.assertEqual((t, why), ("voidF(u8)", "definition"))
        self.assertEqual(raw, "void func_80042808(u8 a)")

    def test_existing_api_entry_is_kept_over_overlay_views(self):
        self.ovl("AAA", "extern s8 D_800E7388;\n")
        self.assertEqual(self.plan()["D_800E7388"][0], "u8")

    def test_unprototyped_entry_takes_the_call_site_prototype(self):
        self.write("main_api.h", API.replace("void func_80042808(void);", "void func_80042808();"))
        self.write("game.h", '#ifndef GAME_H\n#define GAME_H\n#include "main_api.h"\n#endif\n')
        self.ovl("AAA", "void func_80042808(s32 a);\n")
        # no definition: the call sites of the overlays are the evidence
        self.write("main_api.h", "#ifndef MAIN_API_H\n#define MAIN_API_H\n#endif\n")
        self.write("game.h", '#ifndef GAME_H\n#define GAME_H\nvoid func_80042808();\n#endif\n')
        self.assertEqual(self.plan()["func_80042808"][0], "voidF(s32)")

    def test_narrow_call_site_prototype_does_not_replace_unprototyped(self):
        self.write("main_api.h", "#ifndef MAIN_API_H\n#define MAIN_API_H\n#endif\n")
        self.write("game.h", '#ifndef GAME_H\n#define GAME_H\nvoid func_80042808();\n#endif\n')
        self.ovl("AAA", "void func_80042808(u8 a);\n")
        self.assertEqual(self.plan()["func_80042808"][0], "voidF()")

    def test_most_used_overlay_view_wins_without_a_home(self):
        self.write("main_api.h", "#ifndef MAIN_API_H\n#define MAIN_API_H\n#endif\n")
        self.ovl("AAA", "extern s8 D_800E7399;\n")
        self.ovl("BBB", "extern u8 D_800E7399;\n")
        self.src("ovl/BBB/80132000.c", '#include "ovl/BBB.h"\nvoid f(void) {\n    D_800E7399 = 1;\n}\n')
        self.assertEqual(self.plan()["D_800E7399"][0], "u8")

    def test_type_only_the_header_knows_stays_out(self):
        self.ovl("AAA", "typedef struct Rec {\n    s32 a;\n} Rec;\nextern Rec D_800E7399;\n")
        self.assertNotIn("D_800E7399", self.plan())


class WriteFixTest(Repo):
    def model(self):
        return sp.Model(str(self.inc), str(self.root / "src"))

    def test_write_is_idempotent_and_keeps_comments(self):
        self.write("main_api.h", API.replace("extern u8 D_800E7388;", "extern u8 D_800E7388; /* keep me */"))
        self.ovl("AAA", "extern u8 D_800E7399; /* new one */\n")
        m = self.model()
        sp.write_api(m, sp.plan(m))
        text = (self.inc / "main_api.h").read_text()
        self.assertIn("extern u8 D_800E7388; /* keep me */", text)
        self.assertIn("extern u8 D_800E7399; /* new one */", text)
        m = self.model()
        changed, _n, _g = sp.write_api(m, sp.plan(m))
        self.assertFalse(changed)

    def test_write_keeps_comments_and_other_lines_it_does_not_own(self):
        """T-7030: --write rebuilt the globals and functions sections from the declarations and deleted
        every comment, #if block and type definition among them (the comment above D_800EAFA0)."""
        old = (sp.HEADER_TEXT + "\n/* ---- globals ---- */\n"
               "/* Sector buffer. Two views: this one and the u16 one in ovl/AAA.h. */\n"
               "extern u8 D_800E7388;\n"
               "\n"
               "/* several lines\n"
               " * of comment */\n"
               "#if 0\n"
               "extern u8 not_a_main_symbol;\n"
               "#endif\n"
               "typedef struct Tail {\n    s32 a;\n} Tail;\n"
               "extern s32 D_800E7384; /* trailing */\n"
               "/* last comment of the section */\n"
               "\n/* ---- functions ---- */\n"
               "/* about func_80042808 */\n"
               "void func_80042808(void);\n"
               "/* why this one is u8 */\n"
               "void func_80083440(u8 arg0);\n"
               "/* final note */\n"
               "\n#endif /* MAIN_API_H */\n")
        self.write("main_api.h", old)
        self.ovl("AAA", "extern u8 D_800E7000;\n")      # sorts before D_800E7384: the order changes
        self.write("ovl/BBB.h", "")
        m = self.model()
        sp.write_api(m, sp.plan(m))
        text = (self.inc / "main_api.h").read_text()
        for keep in ("/* Sector buffer. Two views: this one and the u16 one in ovl/AAA.h. */\nextern u8 D_800E7388;",
                     "/* several lines\n * of comment */\n#if 0\n",
                     "typedef struct Tail {\n    s32 a;\n} Tail;\nextern s32 D_800E7384; /* trailing */",
                     "/* last comment of the section */",
                     "/* about func_80042808 */\nvoid func_80042808(void);",
                     "/* why this one is u8 */",
                     "/* final note */"):
            self.assertIn(keep, text)
        self.assertIn("extern u8 not_a_main_symbol;", text)
        self.assertEqual(text.count("#if 0"), 1)
        lines = text.split("\n")
        self.assertEqual(sum(l.startswith("#endif") for l in lines), sum(l.startswith("#if") for l in lines))
        m = self.model()
        changed, _n, _g = sp.write_api(m, sp.plan(m))
        self.assertFalse(changed, "second run must not change the file")
        self.assertEqual((self.inc / "main_api.h").read_text(), text)

    def test_write_keeps_a_comment_with_its_declaration_when_the_order_changes(self):
        self.write("main_api.h", sp.HEADER_TEXT + "\n/* ---- globals ---- */\n/* second */\nextern u8 D_800E7388;\n"
                   "\n/* ---- functions ---- */\n\n#endif /* MAIN_API_H */\n")
        self.ovl("AAA", "extern u8 D_800E7000;\n")
        m = self.model()
        sp.write_api(m, sp.plan(m))
        text = (self.inc / "main_api.h").read_text()
        self.assertLess(text.index("D_800E7000"), text.index("/* second */"))
        self.assertIn("/* second */\nextern u8 D_800E7388;", text)

    def test_write_keeps_the_type_definitions(self):
        types = "/* ---- aggregate types ---- */\ntypedef struct Rec {\n    /* 0x00 */ s32 a;\n} Rec; /* size 0x04 */"
        self.write("main_api.h", sp.HEADER_TEXT + "\n" + types + "\n\n/* ---- globals ---- */\nextern u8 D_800E7388;\n"
                   "\n/* ---- functions ---- */\n\n#endif /* MAIN_API_H */\n")
        m = self.model()
        sp.write_api(m, sp.plan(m))
        text = (self.inc / "main_api.h").read_text()
        self.assertIn(types + "\n\n/* ---- globals ---- */", text)
        self.assertIn("extern u8 D_800E7388;", text)

    def test_write_guards_overrides_and_sorts_by_address(self):
        self.ovl("AAA", "extern s8 D_800E7388;\n", head='#define MAIN_API_OVERRIDE_D_800E7388 /* lb */\n'
                 '#include "common.h"\n#include "game.h"\n')
        self.ovl("BBB", "extern u8 D_800E7000;\n")
        m = self.model()
        sp.write_api(m, sp.plan(m))
        sp.fix_header(self.model(), sp.plan(self.model()), "ovl/BBB.h", log=lambda *a: None)
        text = (self.inc / "main_api.h").read_text()
        self.assertIn("#ifndef MAIN_API_OVERRIDE_D_800E7388\nextern u8 D_800E7388;\n#endif", text)
        self.assertLess(text.index("D_800E7000"), text.index("D_800E7384"))
        self.assertEqual(self.problems(), [])

    def test_fix_drops_same_and_unused_and_marks_used_views(self):
        self.ovl("AAA", "extern u8 D_800E7388;\nextern s8 D_800E7384;\nvoid func_80083440(s32 a);\nextern s32 D_80140000;\n")
        self.src("ovl/AAA/80132000.c", '#include "common.h"\n#include "ovl/AAA.h"\n'
                 "void f(void) {\n    func_80083440(D_800E7388);\n}\n")
        m = self.model()
        plan = sp.plan(m)
        sp.fix_header(m, plan, "ovl/AAA.h", log=lambda *a: None)
        text = (self.inc / "ovl/AAA.h").read_text()
        self.assertNotIn("extern u8 D_800E7388;", text)        # same type
        self.assertNotIn("D_800E7384", text)                    # other type, unused
        self.assertIn("extern s32 D_80140000;", text)           # overlay symbol stays
        self.assertIn("void func_80083440(s32 a);", text)       # used and different: kept as an override
        self.assertIn("#define MAIN_API_OVERRIDE_func_80083440 /* matched with void(s32) (main_api.h: void(u8)) */", text)
        self.assertLess(text.index("MAIN_API_OVERRIDE"), text.index("#include"))
        sp.write_api(self.model(), sp.plan(self.model()))
        self.assertEqual(self.problems(), [])

    def test_fix_adds_the_include_and_splits_multi_declarator_lines(self):
        self.ovl("AAA", "extern u8 D_800E7388, D_80140000, D_800E7384;\n", head='#include "common.h"\n')
        m = self.model()
        sp.fix_header(m, sp.plan(m), "ovl/AAA.h", log=lambda *a: None)
        text = (self.inc / "ovl/AAA.h").read_text()
        self.assertIn('#include "main_api.h"', text)
        self.assertIn("extern u8 D_80140000;", text)
        self.assertNotIn("D_800E7388", text)

    def test_prune_keeps_only_what_the_build_needs(self):
        head = '#define MAIN_API_OVERRIDE_D_800E7388 /* lb */\n#define MAIN_API_OVERRIDE_D_800E7384 /* lw */\n' \
               '#include "common.h"\n#include "game.h"\n'
        self.ovl("AAA", "extern s8 D_800E7388;\nextern u32 D_800E7384;\n", head=head)
        self.write("main_api.h", API.replace("extern u8 D_800E7388;", "#ifndef MAIN_API_OVERRIDE_D_800E7388\nextern u8 D_800E7388;\n#endif")
                   .replace("extern s32 D_800E7384;", "#ifndef MAIN_API_OVERRIDE_D_800E7384\nextern s32 D_800E7384;\n#endif"))
        asked = []

        def run(target):
            asked.append(target)
            return "extern s8 D_800E7388;" in (self.inc / "ovl/AAA.h").read_text()

        # the build "fails" whenever the s8 view is gone, passes when only the u32 view is removed
        removed, kept = sp.prune(self.model(), run=run, log=lambda *a: None)
        self.assertEqual(asked[0], "build/ovl/AAA.ok")
        self.assertEqual(sorted(n for _f, n in removed), ["D_800E7384"])
        self.assertEqual(sorted(n for _f, n in kept), ["D_800E7388"])
        text = (self.inc / "ovl/AAA.h").read_text()
        self.assertIn("extern s8 D_800E7388;", text)
        self.assertNotIn("D_800E7384", text)

    def test_remove_override_drops_the_empty_block_comment(self):
        self.ovl("AAA", "extern s8 D_800E7388;\n", head="/* main_api.h overrides (T-3340): views */\n"
                 "#define MAIN_API_OVERRIDE_D_800E7388 /* lb */\n#include \"common.h\"\n#include \"game.h\"\n")
        sp.remove_override(str(self.inc / "ovl/AAA.h"), "D_800E7388")
        text = (self.inc / "ovl/AAA.h").read_text()
        self.assertNotIn("main_api.h overrides", text)
        self.assertNotIn("D_800E7388", text)


    def test_remove_override_in_a_c_file_keeps_code_untouched(self):
        path = self.root / "src" / "main" / "80042808.c"
        path.write_text("/* main_api.h overrides (T-3340): views */\n#define MAIN_API_OVERRIDE_func_80083440 /* implicit */\n\n"
                        '#include "game.h"\n\nvoid f(void) {\n    if (D_800E7388) {\n        func_80083440(1);\n    }\n}\n')
        sp.remove_override(str(path), "func_80083440")
        self.assertEqual(path.read_text(), '#include "game.h"\n\nvoid f(void) {\n    if (D_800E7388) {\n'
                                           '        func_80083440(1);\n    }\n}\n')


class MinimalDiffTest(Repo):
    """T-9030: --write/--fix never repeat a hand-written declaration and touch only what they must."""
    def model(self):
        return sp.Model(str(self.inc), str(self.root / "src"))

    HAND = (sp.HEADER_TEXT + '\ntypedef struct Rec {\n    s32 a;\n} Rec;\nvoid func_80042808();\n'
            "#ifndef MAIN_API_OVERRIDE_func_80083440\nvoid func_80083440(u8 arg0);\n#endif\n"
            "\n/* ---- globals ---- */\nextern u8 D_800E7388;\n\n/* ---- functions ---- */\n\n#endif /* MAIN_API_H */\n")

    def test_hand_written_prototype_among_the_typedefs_is_moved_not_repeated(self):
        self.write("main_api.h", self.HAND)
        self.ovl("AAA", "void func_80042808(void);\n")
        m = self.model()
        sp.write_api(m, sp.plan(m))
        text = (self.inc / "main_api.h").read_text()
        self.assertEqual(text.count("func_80042808"), 1, text)
        self.assertEqual(text.count("func_80083440("), 1, text)
        self.assertIn("} Rec;", text)
        self.assertLess(text.index("/* ---- functions ---- */"), text.index("func_80042808"))
        self.assertEqual([p for p in self.problems() if p.startswith("duplicate")], [])
        m = self.model()
        self.assertFalse(sp.write_api(m, sp.plan(m))[0])

    def test_check_names_the_hand_written_duplicate(self):
        self.write("main_api.h", self.HAND.replace("\n/* ---- functions ---- */\n",
                                                   "\n/* ---- functions ---- */\nvoid func_80042808(void);\n"))
        out = [p for p in self.problems() if "func_80042808" in p]
        self.assertEqual(len(out), 1, out)
        self.assertIn("duplicate func_80042808", out[0])
        self.assertIn("sync_protos.py --write", out[0])

    def test_check_reports_an_alias_conflict_unless_known(self):
        (self.root / "config" / "obin_renames.txt").write_text("func_80043914 load_palette\n")
        self.write("main_api.h", API.replace("void func_80042808(void);",
                                             "void func_80043914(u16 a);\nvoid load_palette(u8 a);\nvoid func_80042808(void);"))
        self.has("alias conflict at 0x80043914")
        (self.root / "config").mkdir(exist_ok=True)
        (self.root / sp.KNOWN).write_text("alias 0x80043914\n")
        self.assertEqual([p for p in self.problems() if "alias" in p], [])

    def test_write_keeps_the_order_of_existing_declarations(self):
        self.write("main_api.h", sp.HEADER_TEXT + "\n/* ---- globals ---- */\nextern s32 D_800E7384;\nextern u8 D_800E7388;\n"
                   "extern u8 D_800E7380;\n\n/* ---- functions ---- */\n\n#endif /* MAIN_API_H */\n")
        self.ovl("AAA", "extern u8 D_800E7000;\nextern u8 D_800E7386;\n")
        m = self.model()
        sp.write_api(m, sp.plan(m))
        text = (self.inc / "main_api.h").read_text()
        order = [text.index(n) for n in ("D_800E7000", "D_800E7384", "D_800E7386", "D_800E7388", "D_800E7380")]
        self.assertEqual(order, sorted(order), text)

    def test_fix_leaves_a_header_with_nothing_to_change_alone(self):
        body = "extern s32 D_80140000;\n\n\n\nvoid func_80132000(void);\n"
        self.ovl("AAA", body)
        before = (self.inc / "ovl/AAA.h").read_text()
        sp.run_write(str(self.inc), str(self.root / "src"), fix=True, log=lambda *a: None)
        self.assertEqual((self.inc / "ovl/AAA.h").read_text(), before)

    def test_fix_only_rewrites_the_named_headers(self):
        self.ovl("AAA", "extern u8 D_800E7388;\n")
        self.ovl("BBB", "extern u8 D_800E7388;\n")
        sp.run_write(str(self.inc), str(self.root / "src"), fix=True, log=lambda *a: None, only={"ovl/AAA.h"})
        self.assertNotIn("D_800E7388", (self.inc / "ovl/AAA.h").read_text())
        self.assertIn("D_800E7388", (self.inc / "ovl/BBB.h").read_text())


class ViewsTest(Repo):
    def views(self):
        return sp.snapshot(sp.Model(str(self.inc), str(self.root / "src")))

    def test_snapshot_and_compare(self):
        self.ovl("AAA", "void func_80042808();\nextern s32 D_80140000;\n")
        self.src("ovl/AAA/80132000.c", '#include "common.h"\n#include "ovl/AAA.h"\n'
                 "void f(void) {\n    func_80042808();\n    D_80140000 = D_800E7388;\n}\n")
        before = self.views()
        self.assertIn("D_80140000", before["src/ovl/AAA/80132000.c"])
        self.assertIn("D_800E7388", before["src/ovl/AAA/80132000.c"])
        self.assertEqual(sp.view_changes(before, self.views()), [])
        changes = sp.view_changes({"u": {"f": "voidF()", "g": "u8", "h": "voidF(s32)"}},
                                  {"u": {"f": "voidF(s32,s32)", "g": "u8[]", "h": "voidF(u8)"}})
        self.assertEqual({c[1]: c[5] for c in changes}, {"f": "benign", "g": "risky", "h": "risky"})

    def test_implicit_to_narrow_prototype_is_risky(self):
        self.src("ovl/AAA/80132000.c", '#include "common.h"\n#include "ovl/AAA.h"\nvoid f(void) {\n    func_80083440(1);\n}\n')
        self.ovl("AAA", "")
        old = {"src/ovl/AAA/80132000.c": {}}
        changes = sp.view_changes(old, self.views())
        self.assertEqual([(c[1], c[5]) for c in changes], [("func_80083440", "risky")])

    def test_implicit_to_void_prototype_needs_the_build(self):
        self.src("ovl/AAA/80132000.c", '#include "common.h"\n#include "ovl/AAA.h"\nvoid f(void) {\n    func_80042808();\n}\n')
        self.ovl("AAA", "")
        changes = sp.view_changes({"src/ovl/AAA/80132000.c": {}}, self.views())
        self.assertEqual([(c[1], c[5]) for c in changes], [("func_80042808", "check")])

    def test_snapshot_roundtrips_json(self):
        self.ovl("AAA", "extern s32 D_80140000;\n")
        self.src("ovl/AAA/80132000.c", '#include "common.h"\n#include "ovl/AAA.h"\nvoid f(void) {\n    D_80140000 = 1;\n}\n')
        v = self.views()
        self.assertEqual(json.loads(json.dumps(v)), v)


class HelpersTest(unittest.TestCase):
    def test_benign(self):
        self.assertTrue(sp.benign("voidF()", "voidF(s32,s32)"))
        self.assertTrue(sp.benign("voidF(void)", "voidF()"))
        self.assertFalse(sp.benign("voidF()", "voidF(u8)"))
        self.assertFalse(sp.benign("voidF(s32)", "voidF(s32,s32)"))
        self.assertFalse(sp.benign("s32F()", "voidF()"))
        self.assertFalse(sp.benign("u8", "u8[]"))

    def test_effective_type(self):
        self.assertEqual(sp.effective(["voidF()", "voidF(s32)", "voidF(void)"]), "voidF(s32)")
        self.assertEqual(sp.effective(["voidF()", "voidF(void)"]), "voidF(void)")

    def test_tokens_skip_comments_strings_and_include_asm(self):
        t = sp.tokens('INCLUDE_ASM("asm/x", func_80042808);\n/* D_800E7388 */\nvoid f(void) {\n'
                      '    g("D_800E7399");\n    func_80042900();\n}\n')
        self.assertIn("func_80042900", t)
        self.assertNotIn("func_80042808", t)
        self.assertNotIn("D_800E7388", t)
        self.assertNotIn("D_800E7399", t)

    def test_is_main_symbol(self):
        with tempfile.TemporaryDirectory() as d:
            (Path(d) / "config").mkdir()
            (Path(d) / "config" / "overlays.txt").write_text("EVT 0x800F6000 0x10\nAAA 0x80132000 0x10\n")
            self.assertTrue(sp.is_main_symbol(d, "func_80042808", "AAA"))
            self.assertFalse(sp.is_main_symbol(d, "func_80132000", "AAA"))
            self.assertFalse(sp.is_main_symbol(d, "D_80120650", "EVT"))
            self.assertTrue(sp.is_main_symbol(d, "D_80120650", "AAA"))
            self.assertFalse(sp.is_main_symbol(d, "printf", "AAA"))

    def test_repo_is_clean(self):
        repo = Path(__file__).resolve().parent.parent
        if (repo / "include" / "main_api.h").exists():
            self.assertEqual(sp.check_api(str(repo / "include"), str(repo / "src")), [])


class WalkTest(Repo):
    def test_walk_evaluates_defines_and_guards(self):
        self.write("a.h", '#ifndef NOPE\nextern s32 D_1;\n#else\nextern s16 D_1;\n#endif\n#ifdef YES\nextern u8 D_2;\n#endif\n')
        self.write("b.h", '#define YES\n#include "a.h"\n')
        names = [(n, t) for n, t, _f, _r in check_headers.walk(str(self.inc / "b.h"), str(self.inc))]
        self.assertEqual(names, [("D_1", "s32"), ("D_2", "u8")])

    def test_override_define_removes_the_conflict(self):
        self.write("a.h", '#ifndef MAIN_API_OVERRIDE_D_1\nextern s32 D_1;\n#endif\n')
        self.write("b.h", '#define MAIN_API_OVERRIDE_D_1 /* x */\n#include "a.h"\nextern u8 D_1;\n')
        self.assertEqual(check_headers.check(str(self.inc), api=False), [])
        self.write("b.h", '#include "a.h"\nextern u8 D_1;\n')
        self.assertTrue(any("conflict D_1" in p for p in check_headers.check(str(self.inc), api=False)))


class SpeedTest(Repo):
    """--fix took minutes on the real tree (T-5030): per-symbol rescans of headers and sources."""

    def big_api(self, n):
        lines = ["#ifndef MAIN_API_H", "#define MAIN_API_H", '#include "common.h"']
        lines += ["extern u8 D_%08X; /* c%d */" % (0x800E0000 + 4 * i, i) for i in range(n)]
        lines += ["void func_%08X(s32 a);" % (0x80050000 + 8 * i) for i in range(n)]
        self.write("main_api.h", "\n".join(lines + ["#endif", ""]))

    def test_line_index_finds_what_find_line_found(self):
        lines = ["extern u8 D_1; /* one */", "int x", "extern u8 D_2;", "extern u8 D_1;", "void f(s32 a); /* f */"]
        idx = sp.LineIndex(lines)
        self.assertEqual(idx.find("u8 D_1"), (0, "/* one */"))
        self.assertEqual(idx.find("void f(s32 a)"), (4, "/* f */"))
        self.assertEqual(idx.find("u8   D_2"), (2, ""))
        self.assertEqual(idx.find("u8 D_3"), (None, ""))
        self.assertEqual(sp.find_line(lines, "u8 D_2"), (2, ""))

    def test_write_api_reads_candidates_once(self):
        self.big_api(60)
        self.ovl("AAA", "extern u8 D_800E0004;\nvoid func_80050008(s32 a);\n")
        model = sp.Model(str(self.inc), str(self.root / "src"))
        calls = []
        real = sp.candidates
        sp.candidates = lambda m: calls.append(1) or real(m)
        try:
            sp.write_api(model, sp.plan(model))
        finally:
            sp.candidates = real
        self.assertLessEqual(len(calls), 3)   # once per plan/build, not once per symbol

    def test_header_closure_is_memoised_and_follows_a_rewrite(self):
        self.ovl("AAA", "extern s32 D_80140000;\n", head='#include "common.h"\n')
        model = sp.Model(str(self.inc), str(self.root / "src"))
        self.assertNotIn("main_api.h", sp.closure(model, "ovl/AAA.h"))
        self.assertIs(sp.closure(model, "ovl/AAA.h"), sp.closure(model, "ovl/AAA.h"))
        sp.fix_header(model, sp.plan(model), "ovl/AAA.h")      # adds the main_api.h include
        self.assertIn("main_api.h", sp.closure(model, "ovl/AAA.h"))

    def test_fix_result_is_unchanged_by_the_caches(self):
        self.ovl("AAA", "extern u8 D_800E7388;\nvoid func_80042808(void);\nextern s32 D_80140000;\n")
        self.assertEqual(sp.main(["--inc", str(self.inc), "--src", str(self.root / "src"), "--fix"]), 0)
        self.assertEqual(self.problems(), [])
        text = (self.inc / "ovl" / "AAA.h").read_text()
        self.assertNotIn("D_800E7388", text)
        self.assertIn("D_80140000", text)


ASM = """glabel {name}
    /* 0 80132000 00000000 */  addiu      $sp, $sp, -0x18
{body}
    /* 20 80132020 00000000 */  jr         $ra
    /* 24 80132024 00000000 */   nop
endlabel {name}
"""


def call(addr, sym, after):
    """asm text of `jal sym` followed by `after` instructions (each `op args`)."""
    out = ["    /* 4 %08X 00000000 */  jal        %s" % (addr, sym), "    /* 8 %08X 00000000 */   nop" % (addr + 4)]
    for i, ins in enumerate(after):
        out.append("    /* %X %08X 00000000 */  %s" % (12 + 4 * i, addr + 8 + 4 * i, ins))
    return "\n".join(out)


class BranchTest(Repo):
    def asm(self, unit, name, body):
        d = self.root / "asm" / ("ovl/%s/nonmatchings/%s" % (unit, unit) if unit != "main" else "nonmatchings/main/80041000")
        d.mkdir(parents=True, exist_ok=True)
        (d / (name + ".s")).write_text(ASM.format(name=name, body=body))

    def test_result_read_after_call(self):
        def used(after):
            text = call(0x80132004, "func_X", after).split("\n")
            return sp.result_read_after_call(text[1:])
        self.assertTrue(used(["or         $a0, $v0, $zero"]))
        self.assertTrue(used(["beqz       $v0, .L80132040", "nop"]))
        self.assertTrue(used(["sw         $v0, 0x10($sp)"]))
        self.assertTrue(used(["addiu      $t0, $zero, 0x1", "lw         $t1, 0x0($v0)"]))
        self.assertFalse(used(["addiu      $v0, $zero, 0x1", "or         $a0, $v0, $zero"]))   # overwritten first
        self.assertFalse(used(["jal        func_Y", "nop", "or         $a0, $v0, $zero"]))   # another call first
        self.assertFalse(used(["addiu      $a0, $zero, 0x1"]))
        self.assertFalse(used([]))

    def test_alias_conflict(self):
        (self.root / "config" / "obin_renames.txt").write_text("# old new\nfunc_80043914 load_palette\n")
        self.write("main_api.h", API.replace("void func_80042808(void);",
                                             "void func_80043914();\nvoid load_palette(u8 a);\nvoid func_80042808(void);"))
        found = sp.alias_conflicts(sp.Model(str(self.inc), str(self.root / "src")))
        self.assertEqual([k for k, _m in found], ["alias 0x80043914"])
        self.assertIn("load_palette", found[0][1])
        self.assertIn("func_80043914", found[0][1])

    def test_alias_with_same_type_or_benign_k_and_r_is_fine(self):
        (self.root / "config" / "obin_renames.txt").write_text("func_80043914 load_palette\n")
        self.write("main_api.h", API.replace("void func_80042808(void);",
                                             "void func_80043914();\nvoid load_palette(s32 a);\nvoid func_80042808(void);"))
        self.assertEqual(sp.alias_conflicts(sp.Model(str(self.inc), str(self.root / "src"))), [])   # () against s32: benign

    def test_void_declared_function_with_a_caller_using_the_result(self):
        self.ovl("AAA", "extern s32 D_80140000;\n")
        self.asm("AAA", "func_80132100", call(0x80132104, "func_80042808", ["or         $a0, $v0, $zero"]))
        found = sp.void_result_conflicts(sp.Model(str(self.inc), str(self.root / "src")))
        self.assertEqual([k for k, _m in found], ["void-result AAA:func_80042808"])
        self.assertIn("func_80132100.s", found[0][1])

    def test_check_branch_exit_codes_and_known_list(self):
        import io
        self.asm("AAA", "func_80132100", call(0x80132104, "func_80042808", ["or         $a0, $v0, $zero"]))
        self.ovl("AAA", "extern s32 D_80140000;\n")
        out = io.StringIO()
        self.assertEqual(sp.check_branch(str(self.inc), str(self.root / "src"), out), 1)
        self.assertIn("1 new disagreement(s), 0 known", out.getvalue())
        sp.update_known(str(self.inc), str(self.root / "src"), io.StringIO())
        known = (self.root / sp.KNOWN).read_text()
        self.assertIn("void-result AAA:func_80042808", known)
        out = io.StringIO()
        self.assertEqual(sp.check_branch(str(self.inc), str(self.root / "src"), out), 0)
        self.assertIn("0 new disagreement(s), 1 known", out.getvalue())
        # the caller no longer reads the result: the known line is reported as stale
        self.asm("AAA", "func_80132100", call(0x80132104, "func_80042808", []))
        out = io.StringIO()
        sp.check_branch(str(self.inc), str(self.root / "src"), out)
        self.assertIn("no longer found", out.getvalue())

    def test_check_branch_reports_header_rule_violations(self):
        import io
        self.ovl("AAA", "extern u8 D_800E7399;\n")    # main symbol in an overlay header
        out = io.StringIO()
        self.assertEqual(sp.check_branch(str(self.inc), str(self.root / "src"), out), 1)
        self.assertIn("declare it in include/main_api.h", out.getvalue())


if __name__ == "__main__":
    unittest.main()
