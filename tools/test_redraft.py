#!/usr/bin/env python3
"""Tests for tools/redraft.py, tools/redraft_rewrites.py and tools/redraft_extras.py (T-20800).

All input is synthetic: small headers, m2c-shaped C and asm written for the test, in a temporary tree. The C
shapes are those the types-first passes (T-16550 .. T-18800) rewrote by hand; no game data.

Run: tools/docker.sh python3 tools/test_redraft.py   (from the repo root)
"""
import json
import os
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import redraft as rd  # noqa: E402
import redraft_extras as rx  # noqa: E402
import redraft_rewrites as rw  # noqa: E402

LIBGTE = "typedef struct { s16 m[3][3]; s16 pad; s32 t[3]; } MATRIX;\n"
PSX = "typedef struct { s16 x; s16 y; s16 w; s16 h; } RECT;\ntypedef struct { u8 r0; u8 g0; u8 b0; u8 code; } POLY_X;\n"
GAME = """
typedef struct DisplayActor44 {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02;
    u8 pad[0x40];
} DisplayActor44; /* size 0x44 */
typedef struct GameState {
    s32 unk_000;
    s16 unk_004;
    s16 unk_006;
    u8 unk_008[0x18];
} GameState; /* size 0x20 */
"""
MAIN_API = """
typedef struct Rec24 {
    s32 unk_00;
    u8 pad_04[0x10];
    s16 unk_14;
    u8 unk_16;
    u8 unk_17;
    u8 pad_18[0xC];
} Rec24; /* size 0x24 */
extern GameState D_800E6280;
extern u8 D_8011ECD0[];
extern Rec24 D_801217D0[]; /* at least 12 records */
extern s16 D_80100200[4];
extern s32 *D_80100300;
"""
OVL = """
typedef struct FooObj {
    s32 unk_00;
    s16 unk_04;
    u8 pad_06[2];
    u8 unk_08;
    u8 unk_09;
    u8 pad_0A[2];
} FooObj; /* size 0x0C */
extern FooObj D_80140000;
extern FooObj D_80140100[4];
typedef struct FooTbl { s16 v[4]; } FooTbl; /* size 0x8 */
extern FooTbl D_80140200;
"""


def make_tree(root):
    root = Path(root)
    (root / "include/ovl").mkdir(parents=True)
    (root / "include/libgte.h").write_text(LIBGTE)
    (root / "include/psx_types.h").write_text(PSX)
    (root / "include/main_api.h").write_text(MAIN_API)
    (root / "include/game.h").write_text(GAME)
    (root / "include/ovl/FOO.h").write_text(OVL)
    return root


def asm(*lines):
    return "\n".join("    /* %X 80100000 00000000 */  %s" % (i * 4, l) if not l.startswith(".L") else l
                     for i, l in enumerate(lines)) + "\n"


class EnvCase(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory()
        cls.root = make_tree(cls.tmp.name)
        cls.env = rw.Env("FOO", str(cls.root))
        cls.main = rw.Env("main", str(cls.root))

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()


class TestEnv(EnvCase):
    def test_types_and_globals(self):
        e = self.env
        self.assertEqual(e.types["Rec24"].size, 0x24)
        self.assertEqual(e.types["FooObj"].size, 0x0C)
        self.assertEqual(e.globals["D_80140000"][1], "obj")
        self.assertEqual(e.globals["D_80140100"][1:], ("array", 4))
        self.assertEqual(e.globals["D_801217D0"][1:], ("array", 12))      # COUNTS_MAIN, no count in the header
        self.assertEqual(e.globals["D_8011ECD0"][1:], ("array", 160))     # the record view
        self.assertIn("D_80140200", e.tables)
        self.assertEqual(e.tables["D_80140200"], ("FooTbl", "v", [4], 2))

    def test_ranges_do_not_overlap(self):
        spans = sorted(self.env.ranges)          # (the lookup order is innermost first; here nothing is nested)
        for (a, b, _), (c, d, _) in zip(spans, spans[1:]):
            self.assertLessEqual(b, c)
        bases = {r[2] for r in spans}
        self.assertTrue({"D_800E6280", "D_8011ECD0", "D_801217D0", "D_80140000", "D_80140100", "D_80100200"} <= bases)

    def test_main_env_has_no_overlay_symbols(self):
        self.assertNotIn("D_80140000", self.main.globals)
        self.assertIn("D_801217D0", self.main.globals)

    def test_context_declares_the_views(self):
        text = self.env.context("extern u8 D_8011ECD0[];\nint x;\n")
        self.assertIn("extern DisplayActor44 D_8011ECD0[];", text)
        self.assertNotIn("extern u8 D_8011ECD0[]", text)


class TestUnitViews(unittest.TestCase):
    def test_master_reads_the_pool_and_the_sprites_as_one_type(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = make_tree(tmp)
            (root / "include/ovl/MASTER.h").write_text(
                "typedef struct DisplayFixActor44 {\n    u8 unk_00;\n    u8 pad[0x43];\n} DisplayFixActor44; /* size 0x44 */\n")
            env = rw.Env("MASTER", str(root))
            self.assertEqual(env.record_views["D_8011ECD0"], ("DisplayFixActor44", 160))
            self.assertEqual(env.record_views["D_80120650"], ("DisplayFixActor44", 64))
            # D_80120650 sits inside D_8011ECD0: the innermost range comes first
            bases = [r[2] for r in env.ranges]
            self.assertLess(bases.index("D_80120650"), bases.index("D_8011ECD0"))
            out = rw.fold(env, asm("lbu $v0, %lo(D_80120654)($a0)", "lbu $v1, %lo(D_80120650)($a0)"), "bare")
            self.assertIn("%lo(D_80120650+0x4)", out)
            self.assertIn("%lo(D_80120650)", out)                     # the innermost base itself stays
            self.assertNotIn("D_8011ECD0", out)

    def test_name_ent_and_etc_views_belong_to_their_units(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = make_tree(tmp)
            (root / "game_extra.h").write_text("")
            with open(root / "include/game.h", "a") as f:
                f.write("typedef struct NameRecordF00 {\n    u8 unk_00;\n    u8 pad[0xEFF];\n} NameRecordF00; /* size 0xF00 */\n"
                        "typedef struct NameMetricWork {\n    s32 unk_00;\n} NameMetricWork;\n")
            (root / "include/ovl/NAME_ENT.h").write_text("")
            self.assertIn("D_80123120", rw.Env("NAME_ENT", str(root)).record_views)
            self.assertIn("D_800E7D10", rw.Env("NAME_ENT", str(root)).obj_views)
            self.assertNotIn("D_80123120", rw.Env("FOO", str(root)).record_views)
            self.assertEqual(rw.Env("FOO", str(root)).obj_views, {})

    def test_event_has_its_own_pool_and_the_default_is_kept(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = make_tree(tmp)
            (root / "include/ovl/EVENT.h").write_text("")
            env = rw.Env("EVENT", str(root))
            self.assertIn("D_800E9620", env.record_views)
            self.assertIn("D_800EC190", env.record_views)
            self.assertIn("D_8011ECD0", env.record_views)

    def test_gyozi_range_starts_inside_its_object(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = make_tree(tmp)
            (root / "include/ovl/GYOZI.h").write_text("")
            env = rw.Env("GYOZI", str(root))
            out = rw.fold(env, asm("lh $v0, %lo(D_800F5550)($a0)"), "late")
            self.assertIn("%lo(D_800F53A0+0x1B0)", out)               # relative to the object, not the range

    def test_view_is_only_declared_where_the_context_has_it(self):
        with tempfile.TemporaryDirectory() as tmp:
            env = rw.Env("FOO", str(make_tree(tmp)))
            self.assertEqual(env.context("extern s32 D_8011ECD0;\n"), "extern s32 D_8011ECD0;\n")


class TestFold(EnvCase):
    ASM = asm("lui $a0, %hi(D_80140008)", "lbu $v0, %lo(D_80140008)($a0)", "lui $a1, %hi(D_80140009)",
              "lbu $v1, %lo(D_80140009)($a1)")

    def test_bare_fold_gives_the_hi_the_base(self):
        out = rw.fold(self.env, self.ASM, "bare")
        self.assertIn("%hi(D_80140000)", out)
        self.assertIn("%lo(D_80140000+0x8)", out)
        self.assertIn("%lo(D_80140000+0x9)", out)

    def test_label_equal_to_the_base_or_outside_is_left(self):
        text = asm("lui $a0, %hi(D_80140000)", "lw $v0, %lo(D_80140000)($a0)", "lw $v1, %lo(D_80150000)($a0)")
        self.assertEqual(rw.fold(self.env, text, "bare"), text)

    def test_keep_mode_keeps_the_offset_of_an_exact_hi(self):
        bare = rw.fold(self.env, self.ASM, "bare")
        keep = rw.fold(self.env, self.ASM, "keep")
        self.assertIn("%hi(D_80140000)", bare)
        # both lui lines name only their own symbol in their %lo uses: the offset stays in the %hi
        self.assertIn("%hi(D_80140000+0x8)", keep)
        self.assertIn("%hi(D_80140000+0x9)", keep)

    def test_keep_mode_keeps_an_indexed_hi(self):
        text = asm("lui $a0, %hi(D_80140100+0x8)", "addu $a0, $a0, $v0", "lbu $v1, %lo(D_80140100+0x8)($a0)",
                   "lbu $v0, %lo(D_80140100+0x9)($a0)")
        out = rw.fold(self.env, text, "keep")
        self.assertIn("%hi(D_80140100+0x8)", out)

    def test_bare_mode_keeps_an_indexed_hi_too(self):
        text = asm("lui $a0, %hi(D_80140100+0x8)", "addu $a0, $a0, $v0", "lbu $v1, %lo(D_80140100+0x8)($a0)")
        self.assertIn("%hi(D_80140100+0x8)", rw.fold(self.env, text, "bare"))

    def test_late_mode_never_bares_the_hi(self):
        out = rw.fold(self.env, self.ASM, "late")
        self.assertIn("%hi(D_80140000+0x8)", out)
        self.assertIn("%lo(D_80140000+0x8)", out)

    def test_named_mode_skips_pad_offsets(self):
        text = asm("lui $a0, %hi(D_80140000)", "lbu $v0, %lo(D_80140006)($a0)", "lbu $v1, %lo(D_80140008)($a0)")
        out = rw.fold(self.env, text, "named")
        self.assertIn("%lo(D_80140006)", out)           # pad_06
        self.assertIn("%lo(D_80140000+0x8)", out)       # unk_08

    def test_gamestate_label_folds(self):
        out = rw.fold(self.env, asm("lh $v0, %lo(D_800E6286)($a0)"), "bare")
        self.assertIn("%lo(D_800E6280+0x6)", out)


class TestPre(EnvCase):
    def test_display_records(self):
        body = "    p = D_8011ECD0 + (i * 0x44);\n    q = D_8011ECD0 + 0x88;\n    r = D_8011ECD0 + 0x10;\n    s = D_8011ECD0;\n"
        out = rw.display_records(self.env, body)
        v = "((DisplayActor44 *)D_8011ECD0)"
        self.assertIn("p = &%s[i];" % v, out)
        self.assertIn("q = &%s[2];" % v, out)
        self.assertIn("r = (DisplayActor44 *)(D_8011ECD0 + 0x10);", out)
        self.assertIn("s = %s;" % v, out)
        self.assertEqual(rw.display_records(self.env, out), out)       # idempotent
        both = rw.display_records(self.env, "    temp_lo = i * 0x44;\n    p = D_8011ECD0 + temp_lo;\n")
        self.assertEqual(rw.display_records(self.env, both), both)

    def test_display_records_through_a_byte_temp(self):
        body = "    temp_lo = i * 0x44;\n    p = D_8011ECD0 + temp_lo;\n"
        out = rw.display_records(self.env, body)
        self.assertIn("p = (DisplayActor44 *)((u8 *)D_8011ECD0 + temp_lo);", out)

    def test_stack_table_copy_and_elements(self):
        body = ("void f(void) {\n    s16 sp10;\n    M2C_MEMCPY(&sp10, &D_80140200, 8);\n"
                "    x = (&sp10)[1];\n    y = &sp10[0];\n}\n")
        out, locals_ = rw.stack_tables(self.env, body)
        self.assertIn("FooTbl sp10;", out)
        self.assertIn("sp10 = D_80140200;", out)
        self.assertIn("x = sp10.v[1];", out)
        self.assertIn("y = sp10.v;", out)
        self.assertEqual(locals_, {"sp10": ("FooTbl", "v", [4], 2)})

    def test_stack_element_local_becomes_the_element(self):
        body = "void f(void) {\n    FooTbl sp10;\n    s16 sp12;\n    sp10 = D_80140200;\n    a = sp12;\n}\n"
        out = rw.stack_elements(self.env, body, {"sp10": ("FooTbl", "v", [4], 2)})
        self.assertNotIn("s16 sp12;", out)
        self.assertIn("a = sp10.v[1];", out)

    def test_object_view(self):
        env = rw.Env("FOO", str(self.root))
        env.obj_views = {"D_80140000": ("FooObj", 0xC)}
        out = rw.obj_views(env, "x = D_80140000.unk_04;\ny = &D_80140000.unk_08;\nz = &D_80140000;\np = &D_80140000;\n")
        self.assertIn("x = ((FooObj *)&D_80140000)->unk_04;", out)
        self.assertIn("y = &((FooObj *)&D_80140000)->unk_08;", out)

    def test_pre_steps_logs_the_names(self):
        log = []
        rw.pre_steps(self.env, "    p = D_8011ECD0 + 0x44;\n", log)
        self.assertEqual(log, ["display-records"])


class TestPost(EnvCase):
    def test_actor_offset_on_an_array_element(self):
        body = "a = *(s16 *)((unsigned char *)&D_80140100[i] + (4));\nb = *(u8 *)((unsigned char *)D_80140100 + (21));\n"
        out = rw.actor_offsets(self.env, body)
        self.assertIn("a = D_80140100[i].unk_04;", out)
        self.assertIn("b = D_80140100[1].unk_09;", out)

    def test_actor_offset_on_an_object_and_a_signedness_cast(self):
        body = "c = *(s8 *)((unsigned char *)&D_80140000 + (8));\nD_80140000.unk_08 = 1;\n*(s16 *)((unsigned char *)&D_80140000 + (4)) = 3;\n"
        out = rw.actor_offsets(self.env, body)
        self.assertIn("c = *(s8 *)&D_80140000.unk_08;", out)        # a load keeps its signedness
        self.assertIn("D_80140000.unk_04 = 3;", out)

    def test_plus_form_on_an_array(self):
        out = rw.actor_offsets(self.env, "p = D_80140100 + 0x18;\nq = D_80140100 + 5;\n")
        self.assertIn("p = &D_80140100[2];", out)
        self.assertIn("q = D_80140100 + 5;", out)

    def test_pointer_step(self):
        body = "    Rec24 *p;\n    s16 *q;\n    s32 **r;\n    p += 0x24;\n    q += 4;\n    r += 8;\n    p += 5;\n"
        out = rw.pointer_steps(self.env, body)
        self.assertIn("p += 1;", out)
        self.assertIn("q += 2;", out)
        self.assertIn("r += 2;", out)
        self.assertIn("p += 5;", out)                                 # not a multiple of the size

    def test_record_temp_with_a_byte_temporary(self):
        body = "    Rec24 *p;\n    temp_lo = i * 0x24;\n    p = D_801217D0 + temp_lo;\n    x = *(s16 *)((unsigned char *)p + (20));\n"
        out = rw.record_temps(self.env, body)
        self.assertIn("p = &D_801217D0[i];", out)

    def test_record_temp_pointer_offsets_become_fields(self):
        body = "    s32 *p;\n    p = &D_801217D0[2];\n    x = *(s16 *)((unsigned char *)p + (20));\n    *(u8 *)((unsigned char *)p + (58)) = 1;\n"
        out = rw.record_temps(self.env, body)
        self.assertIn("Rec24 *p;", out)
        self.assertIn("x = p->unk_14;", out)
        self.assertIn("p[1].unk_16 = 1;", out)

    def test_display_offsets_in_cleaned_text(self):
        body = "x = *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6535));\n"
        out = rw.record_temps(self.env, body)
        self.assertIn("((DisplayActor44 *)D_8011ECD0)[96].pad[3]", out)      # 6535 = 96 * 0x44 + 7

    def test_display_base_walk(self):
        body = "    s32 *p;\n    p = D_8011ECD0;\n    x = *(s16 *)((unsigned char *)p + (2));\n"
        out = rw.record_temps(self.env, body)
        self.assertIn("DisplayActor44 *p;", out)
        self.assertIn("p = ((DisplayActor44 *)D_8011ECD0);", out)
        self.assertIn("x = p->unk_02;", out)

    def test_offsets_without_the_view_cast(self):
        out = rw.record_temps(self.env, "x = *(u8 *)((unsigned char *)D_8011ECD0 + (69));\n")
        self.assertIn("((DisplayActor44 *)D_8011ECD0)[1].unk_01", out)
        out = rw.record_temps(self.env, "y = *(s16 *)((unsigned char *)D_801217D0 + (56));\n")
        self.assertIn("D_801217D0[1].unk_14", out)

    def test_scaled_index_in_a_temporary(self):
        body = ("void f(void) {\n    s16 sp10;\n    M2C_MEMCPY(&sp10, &D_80140200, 8);\n    temp_s2 = i * 2;\n"
                "    x = *(s16 *)((unsigned char *)(sp + temp_s2) + (16));\n}\n")
        out, _ = rw.stack_tables(self.env, body)
        self.assertIn("x = sp10.v[i];", out)

    def test_other_allocator_for_primitives(self):
        env = rw.Env("FOO", str(self.root))
        env.types["POLY_FT4"] = env.types["POLY_X"]
        body = "    s32 *p;\n    p = func_80054B60(0x28, D_8011ECA0);\n    *(u8 *)((unsigned char *)p + (3)) = 9;\n"
        out = rw.work_prims(env, body)
        self.assertIn("p = (POLY_FT4 *)func_80054B60(0x28, D_8011ECA0);", out)
        self.assertIn("p->code = 9;", out)

    def test_product_first_record_index(self):
        out = rw.display_records(self.env, "p = (i * 0x44) + D_8011ECD0;\n")
        self.assertEqual(out, "p = &((DisplayActor44 *)D_8011ECD0)[i];\n")

    def test_struct_walk_makes_a_byte_pointer(self):
        body = "    GameState *p;\n    p = &D_800E6280;\n    x = p->unk_004;\n    p += 4;\n"
        out = rw.struct_walks(self.env, body)
        self.assertIn("u8 *p;", out)
        self.assertIn("p = (u8 *)&D_800E6280;", out)
        self.assertIn("x = *(s16 *)(p + 0x4);", out)

    def test_header_arrays_and_views(self):
        body = ("x = *(&D_80100200 + (i * 2));\n"
                "y = (&D_80100200)[1];\n"
                "z = *(s16 *)((unsigned char *)(&D_80100200) + (4));\n"
                "w = D_80100200->unk0;\n")
        out = rw.array_views(self.env, rw.header_arrays(self.env, body))
        self.assertIn("x = D_80100200[i];", out)
        self.assertIn("y = D_80100200[1];", out)
        self.assertIn("z = D_80100200[2];", out)
        self.assertIn("w = D_80100200[0];", out)

    def test_array_symbols_label_inside_an_array(self):
        self.assertIn("D_80100200[2]", rw.array_symbols(self.env, "x = D_80100204;\n"))

    def test_pointer_index(self):
        body = "x = *(s32 *)((unsigned char *)D_80100300 + (8));\ny = (*(s32 *)D_80100300);\nz = D_80100300->unk0;\n"
        out = rw.pointer_index(self.env, body)
        self.assertIn("x = D_80100300[2];", out)
        self.assertIn("y = D_80100300[0];", out)
        self.assertIn("z = D_80100300[0];", out)

    def test_struct_bytes_and_gamestate_words(self):
        self.assertIn("(u8 *)&D_800E6280 + i", rw.struct_bytes(self.env, "p = &D_800E6280 + i;\n"))
        self.assertIn("*(s32 *)&D_800E6280.unk_100", rw.gamestate_words(self.env, "(s32) D_800E6280.unk_100 + 1\n"))

    def test_gamestate_records(self):
        out = rw.gamestate_records(self.env, "x = *(&D_800E6280.unk_1BC[0].unk_0A + (i * 0x38));\n")
        self.assertEqual(out, "x = D_800E6280.unk_1BC[i].unk_0A;\n")

    def test_raw_display(self):
        out = rw.raw_display(self.env, "*(void *)0x8011ED12 = (u8) x;\n")
        self.assertIn("((DisplayActor44 *)D_8011ECD0)[0]", out)

    def test_return_type(self):
        env = rw.Env("FOO", str(self.root))
        env.decl_text += "\ns32 func_80140010(void);\n"
        self.assertTrue(rw.return_type(env, "void func_80140010(void) {\n}\n").startswith("s32 func_80140010"))
        self.assertTrue(rw.return_type(env, "void func_80140020(void) {\n}\n").startswith("void"))

    def test_work_prim(self):
        env = rw.Env("FOO", str(self.root))
        env.types["POLY_FT4"] = env.types["POLY_X"]
        body = "    s32 *p;\n    p = GetWorkBase(0x28, D_8011ECA0);\n    *(u8 *)((unsigned char *)p + (3)) = 9;\n"
        out = rw.work_prims(env, body)
        self.assertIn("POLY_FT4 *p;", out)
        self.assertIn("p = (POLY_FT4 *)GetWorkBase(0x28, D_8011ECA0);", out)
        self.assertIn("p->code = 9;", out)

    def test_implicit_int_when_called_before_definition(self):
        src = Path(self.root, "src/ovl/FOO")
        src.mkdir(parents=True, exist_ok=True)
        (src / "a.c").write_text('void g(void) { func_80140030(); }\nINCLUDE_ASM("asm/x", func_80140030);\n')
        row = {"func": "func_80140030", "src": "src/ovl/FOO/a.c"}
        env = rw.Env("FOO", str(self.root))
        self.assertTrue(rw.implicit_int(env, "void func_80140030(void) {\n}\n", row).startswith("s32 func_80140030"))
        self.assertTrue(rw.implicit_int(env, "void func_80140031(void) {\n}\n", dict(row, func="func_80140031")).startswith("void"))

    def test_gs_local(self):
        body = "void f(void) {\n    x = D_800E6280.unk_004;\n    p = &D_800E6280;\n}\n"
        out = rw.gs_local(body)
        self.assertIn("GameState *g = &D_800E6280;", out)
        self.assertIn("x = g->unk_004;", out)
        self.assertIn("p = g;", out)
        self.assertEqual(rw.gs_local(out), out)

    def test_field_symbols_writes_the_aggregate_field(self):
        body = "void f(void) {\n    x = D_80140004;\n    D_80140008 = 1;\n}\n"
        known = {"D_80140004": "extern s16 D_80140004;", "D_80140008": "extern u8 D_80140008;"}
        out, left = rw.field_symbols(self.env, body, known)
        self.assertIn("x = D_80140000.unk_04;", out)
        self.assertIn("D_80140000.unk_08 = 1;", out)
        self.assertEqual(left, {})

    def test_apply_post_runs_every_step_and_logs(self):
        body = "void f(void) {\n    Rec24 *p;\n    p += 0x24;\n    x = D_80140004;\n}\n"
        log = []
        out, known = rw.apply_post(self.env, body, {"D_80140004": "extern s16 D_80140004;"}, {}, log)
        self.assertEqual(log, ["pointer-step", "field-symbols"])
        self.assertIn("D_80140000.unk_04", out)

    def test_rewrite_all_is_stable_on_plain_text(self):
        body = "void f(void) {\n    return;\n}\n"
        out, log = rw.rewrite_all(self.env, body)
        self.assertEqual((out, log), (body, []))


class TestExtras(EnvCase):
    def test_matrix_copy(self):
        body = "void f(void) {\n    s32 sp20;\n" + "".join(
            "    *(s32 *)((unsigned char *)(&sp20) + (%d)) = *(s32 *)((unsigned char *)D_80125C60 + (%d));\n" % (
                4 * i, 4 * i) for i in range(8)) + "}\n"
        out = rx.matrix_copies(self.env, body)
        self.assertIn("    MATRIX sp20;", out)
        self.assertIn("    sp20 = *(MATRIX *)D_80125C60;", out)
        self.assertEqual(out.count("sp20 ="), 1)

    def test_matrix_copy_needs_all_eight_words(self):
        body = "\n".join("*(s32 *)((unsigned char *)(&sp20) + (%d)) = *(s32 *)((unsigned char *)D_80125C60 + (%d));" % (
            4 * i, 4 * i) for i in range(7)) + "\n"
        self.assertEqual(rx.matrix_copies(self.env, body), body)

    def test_rect_locals(self):
        body = ("void f(void) {\n    s16 sp2C;\n    s16 sp2E;\n    s16 sp30;\n    s16 sp32;\n"
                "    sp2C = 1;\n    sp32 = 2;\n    g((RECT *) &sp2C);\n}\n")
        out = rx.rect_locals(self.env, body)
        self.assertIn("    RECT sp2C;", out)
        self.assertIn("sp2C.x = 1;", out)
        self.assertIn("sp2C.h = 2;", out)
        self.assertIn("g(&sp2C);", out)
        self.assertNotIn("s16 sp2E;", out)

    def test_rect_view(self):
        out = rx.rect_view(self.env, "D_800B5922 = 1;\nD_800B592E = 2;\n*D_800B5920 = 3;\nD_800B5923 = 4;\n")
        self.assertIn("((RECT *)D_800B5920)[0].y = 1;", out)
        self.assertIn("((RECT *)D_800B5920)[1].h = 2;", out)
        self.assertIn("((RECT *)D_800B5920)[0].x = 3;", out)
        self.assertIn("D_800B5923 = 4;", out)                         # an odd offset is not a halfword field

    def test_menu_arrays_with_s16_rows(self):
        body = ("void f(void) {\n    s16 sp40;\n    s16 sp30;\n    s16 sp20;\n    s16 sp10;\n"
                "    menu_set(1, 2, &sp40, &sp30, &sp20, &sp10);\n}\n")
        out = rx.menu_arrays(self.env, body)
        self.assertIn("    s16 sp40[8];", out)
        self.assertIn("menu_set(1, 2, sp40, sp30, sp20, sp10);", out)

    def test_tcpos_needs_the_type(self):
        self.assertEqual(rx.tcpos_args(self.env, "func_8015ACCC(a, b);\n", "x"), "func_8015ACCC(a, b);\n")

    def test_club_bits_and_record_copies_need_their_types(self):
        text = "x = (u32) ((u32) ((s32) D_800E6280.unk_66C[6] << 5) >> 0x1E);\n"
        self.assertEqual(rx.club_bits(self.env, text), text)
        self.assertEqual(rx.record_copies(self.env, "*(M2cBlock44 *)(a) = *(M2cBlock44 *)(b);\n"),
                         "*(M2cBlock44 *)(a) = *(M2cBlock44 *)(b);\n")
        env = rw.Env("FOO", str(self.root))
        env.types["ScheduleClubFlags"] = env.types["Rec24"]
        self.assertIn("((ScheduleClubFlags *)&D_800E6280.unk_66C[6])->unk_25", rx.club_bits(env, text))

    def test_reroll_leaves_other_loops_alone(self):
        body = "void f(void) {\n    do {\n        x += 1;\n    } while (x < 4);\n}\n"
        self.assertEqual(rx.reroll(self.env, body), body)


class TestTargets(unittest.TestCase):
    ROWS = [dict(unit=u, func=f, scope="%s:%s" % (u, f), src="s.c", size=8, guarded=False)
            for u, f in (("DATE", "func_80132000"), ("GEKO", "func_80132000"), ("TT", "func_80132100"),
                         ("main", "func_80050000"), ("GEKO", "func_80132200"))]

    def test_unit_and_all(self):
        rows, msgs = rd.select_targets(self.ROWS, units=("GEKO",))
        self.assertEqual([r["scope"] for r in rows], ["GEKO:func_80132000", "GEKO:func_80132200"])
        rows, msgs = rd.select_targets(self.ROWS, all_unmatched=True)
        self.assertEqual(sorted(r["unit"] for r in rows), ["DATE", "GEKO", "GEKO", "main"])      # TT skipped
        self.assertEqual(msgs, [])

    def test_bare_name_matches_every_unit_and_scoped_name_one(self):
        rows, _ = rd.select_targets(self.ROWS, ["func_80132000"])
        self.assertEqual(len(rows), 2)
        rows, _ = rd.select_targets(self.ROWS, ["GEKO:func_80132000"])
        self.assertEqual([r["scope"] for r in rows], ["GEKO:func_80132000"])

    def test_tt_is_refused_unless_included(self):
        rows, msgs = rd.select_targets(self.ROWS, ["TT:func_80132100"])
        self.assertEqual(rows, [])
        self.assertTrue(any("skipped" in m for m in msgs))
        rows, msgs = rd.select_targets(self.ROWS, ["TT:func_80132100"], skip_units=())
        self.assertEqual([r["scope"] for r in rows], ["TT:func_80132100"])

    def test_unknown_function_is_reported(self):
        rows, msgs = rd.select_targets(self.ROWS, ["GEKO:func_99999999", "nosuch"])
        self.assertEqual(rows, [])
        self.assertEqual(len(msgs), 2)


class TestArchive(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        cat = self.root / "drafts/T-14500"
        (cat / "GEKO/func_1").mkdir(parents=True)
        (cat / "GEKO/func_1/body.c").write_text("void func_1(void) {\n}\n")
        (cat / "best-quality-catalog.jsonl").write_text(
            json.dumps({"scope": "GEKO:func_1", "score": 40, "selected_path": "drafts/T-14500/GEKO/func_1"}) + "\n")

    def tearDown(self):
        self.tmp.cleanup()

    def add(self, ticket, unit, func, score, hand=None):
        d = self.root / "drafts" / ticket / unit / func
        d.mkdir(parents=True)
        (d / "body.c").write_text("body %s\n" % ticket)
        r = {"scope": "%s:%s" % (unit, func), "score": score}
        if hand is not None:
            (d / "hand.c").write_text("hand\n")
            r["hand_score"] = hand
        (d / "result.json").write_text(json.dumps(r))

    def test_best_is_the_lowest_score_then_the_newest_ticket(self):
        self.add("T-17700", "GEKO", "func_1", 30)
        self.add("T-17800", "GEKO", "func_1", 30)
        self.add("T-17900", "GEKO", "func_1", 35)
        a = rd.Archive(self.root)
        self.assertEqual(a.best_of("GEKO:func_1")["ticket"], "T-17800")
        self.assertEqual(a.census_of("GEKO:func_1")["score"], 40)

    def test_hand_draft_counts_when_it_scores_better(self):
        self.add("T-17700", "GEKO", "func_1", 30, hand=12)
        best = rd.Archive(self.root).best_of("GEKO:func_1")
        self.assertEqual((best["score"], best["file"]), (12, "hand.c"))

    def test_runs_and_foreign_folders_are_not_archives(self):
        self.add("T-17700", "_runs", "func_1", 1)
        self.add("T-99999", "GEKO", "func_1", 1)           # not a draft ticket (outside T-15xxx..T-2xxxx)
        a = rd.Archive(self.root)
        self.assertEqual(a.best_of("GEKO:func_1")["score"], 40)

    def test_unscored_or_bodyless_entries_are_skipped(self):
        self.add("T-17700", "GEKO", "func_2", None)
        self.assertIsNone(rd.Archive(self.root).best_of("GEKO:func_2"))


class TestArchivedSource(EnvCase):
    def test_declarations_filtered_and_typedefs_move_into_the_body(self):
        d = Path(self.tmp.name) / "arch"
        d.mkdir()
        (d / "declarations.h").write_text("extern FooObj D_80140000;\nextern s32 D_80999999;\n"
                                          "typedef struct { s16 a; } Local3;\nextern Local3 D_80888888;\n")
        (d / "body.c").write_text("void f(void) {\n    return;\n}\n")
        text = rd.archived_source(self.env, d, "body.c")
        self.assertNotIn("D_80140000", text)             # the header declares it now
        self.assertIn("extern s32 D_80999999;", text.split("void f")[0])
        self.assertIn("typedef struct { s16 a; } Local3;", text.split("void f")[1])
        self.assertIn("extern Local3 D_80888888;", text.split("void f")[1])


class TestResult(unittest.TestCase):
    ROW = dict(unit="GEKO", func="func_1", scope="GEKO:func_1", src="src/ovl/GEKO/a.c", size=64, guarded=False)

    def test_choose_variant_prefers_the_lower_score_then_the_earlier_variant(self):
        order = ["typed", "typed-late", "plain", "archived"]
        self.assertEqual(rd.choose_variant({"plain": 3, "typed": 5}, order), "plain")
        self.assertEqual(rd.choose_variant({"plain": 3, "typed": 3}, order), "typed")
        self.assertEqual(rd.choose_variant({"archived": 0, "typed": 0}, order), "typed")

    def test_record_fields(self):
        best = {"score": 20, "path": "drafts/T-17700/GEKO/func_1", "file": "body.c", "ticket": "T-17700"}
        r = rd.result_record(self.ROW, "T-20810", {"typed": 12, "plain": None}, "typed", ["pointer-step"],
                             {"score": 30, "path": "drafts/T-14500/GEKO/func_1"}, best)
        for key in ("scope", "score", "census_score", "variant", "provenance"):
            self.assertIn(key, r)
        self.assertEqual((r["score"], r["census_score"], r["variant"], r["status"]), (12, 30, "typed", "scored"))
        self.assertEqual((r["vs_census"], r["vs_archived"]), ("better", "better"))
        self.assertEqual(r["provenance"]["ticket"], "T-20810")
        self.assertEqual(r["provenance"]["archived"], best)
        self.assertIn("harness_hash", r["provenance"])
        self.assertEqual(r["rewrites"], ["pointer-step"])

    def test_zero_failed_and_worse(self):
        r0 = rd.result_record(self.ROW, "T", {"typed": 0}, "typed", [], {"score": 5}, None)
        self.assertEqual((r0["status"], r0["vs_archived"]), ("candidate-zero", None))
        rf = rd.result_record(self.ROW, "T", {"typed": None}, None, [], {}, None)
        self.assertEqual((rf["status"], rf["score"], rf["vs_census"]), ("failed", None, None))
        rw_ = rd.result_record(self.ROW, "T", {"plain": 9}, "plain", [], {"score": 5}, {"score": 4, "path": "p"})
        self.assertEqual((rw_["vs_census"], rw_["vs_archived"]), ("worse", "worse"))

    def test_variants_are_known(self):
        for v in rd.DEFAULT_VARIANTS:
            self.assertIn(v, rd.VARIANTS)
        row = dict(self.ROW, archived=None)
        self.assertFalse(rd.applicable(row, "archived"))
        self.assertFalse(rd.applicable(row, "guarded"))
        self.assertTrue(rd.applicable(dict(row, guarded=True, archived={"path": "p"}), "guarded"))
        self.assertTrue(rd.applicable(dict(row, archived={"path": "p"}), "archived"))
        self.assertTrue(rd.applicable(row, "typed"))


class TestInstall(EnvCase):
    """install() patches m2c and bulk_m2c; the order of the fold and fix_shared_hi is the variant."""

    def patched(self, variant):
        import bulk_m2c
        import m2c
        stack = __import__("contextlib").ExitStack()
        stack.enter_context(mock.patch.object(m2c, "fix_shared_hi", lambda t: t + "|fix"))
        stack.enter_context(mock.patch.object(m2c, "build_context", lambda *a, **k: None))
        stack.enter_context(mock.patch.object(m2c, "split_draft", lambda text, name: ("", text)))
        stack.enter_context(mock.patch.object(m2c, "run_m2c", lambda *a, **k: "m2c"))
        stack.enter_context(mock.patch.object(bulk_m2c, "cleanup", lambda d, b, g=None: (b, {}, None)))
        applied = []
        rd.install(self.env, rd.VARIANTS[variant], applied, {}, ("drafts/x", "body.c"))
        return m2c, bulk_m2c, applied, stack

    def test_bare_folds_before_fix_shared_hi(self):
        m2c, _, _, stack = self.patched("typed")
        with stack:
            out = m2c.fix_shared_hi(asm("lui $a0, %hi(D_80140008)", "lbu $v0, %lo(D_80140008)($a0)"))
        self.assertIn("%hi(D_80140000)", out)
        self.assertTrue(out.endswith("|fix"))

    def test_late_folds_after_fix_shared_hi(self):
        m2c, _, _, stack = self.patched("typed-late")
        with stack:
            out = m2c.fix_shared_hi(asm("lui $a0, %hi(D_80140008)", "lbu $v0, %lo(D_80140008)($a0)"))
        self.assertIn("%hi(D_80140000+0x8)", out)
        self.assertTrue(out.endswith("|fix"))

    def test_rewrites_run_on_the_split_and_the_cleanup(self):
        m2c, bulk, applied, stack = self.patched("typed")
        with stack:
            _, body = m2c.split_draft("    p = D_8011ECD0 + 0x44;\n", "f")
            cleaned, known, problem = bulk.cleanup("", "void f(void) {\n    Rec24 *p;\n    p += 0x24;\n}\n")
        self.assertIn("&((DisplayActor44 *)D_8011ECD0)[1]", body)
        self.assertIn("p += 1;", cleaned)
        self.assertIsNone(problem)
        self.assertEqual(sorted(set(applied)), ["display-records", "pointer-step"])

    def test_plain_changes_nothing_but_an_archived_body_replaces_m2c(self):
        m2c, bulk, applied, stack = self.patched("plain")
        with stack:
            self.assertEqual(m2c.fix_shared_hi("x"), "x|fix")
            self.assertEqual(m2c.run_m2c(".", "A:f"), "m2c")
        m2c, bulk, applied, stack = self.patched("archived")
        with stack, mock.patch.object(rd, "archived_source", lambda env, folder, name: "ARCH %s %s" % (folder, name)):
            self.assertEqual(m2c.run_m2c(".", "A:f"), "ARCH drafts/x body.c")


class TestUnitText(unittest.TestCase):
    SRC = ('#include "common.h"\n\nINCLUDE_ASM("asm/nonmatchings/FOO/a", func_80140010);\n\n'
           '/* NON_MATCHING: T-1 note */\n#ifdef NON_MATCHING\nvoid func_80140020(void) {\n    old();\n}\n#else\n'
           'INCLUDE_ASM("asm/nonmatchings/FOO/a", func_80140020);\n#endif\n')

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        (self.root / "a.c").write_text(self.SRC)
        (self.root / "decl").mkdir()
        (self.root / "decl" / "declarations.h").write_text("extern s32 D_80140100;\n")

    def tearDown(self):
        self.tmp.cleanup()

    def row(self, func):
        return {"func": func, "src": str(self.root / "a.c")}

    def test_plain_include_asm_gets_the_body_and_the_declarations(self):
        text = rd.unit_text(self.row("func_80140010"), "void func_80140010(void) {\n    new();\n}\n", self.root / "decl")
        self.assertIn("extern s32 D_80140100;", text)
        self.assertIn("void func_80140010(void) {\n    new();\n}", text)
        self.assertNotIn('INCLUDE_ASM("asm/nonmatchings/FOO/a", func_80140010)', text)
        self.assertIn("old();", text)                     # the other function's guard is untouched

    def test_guarded_function_regenerated_replaces_the_block(self):
        text = rd.unit_text(self.row("func_80140020"), "void func_80140020(void) {\n    new();\n}\n", self.root / "decl")
        self.assertIn("new();", text)
        self.assertNotIn("old();", text)
        self.assertNotIn("#ifdef NON_MATCHING", text)

    def test_guarded_body_replaces_the_block_as_it_is(self):
        text = rd.unit_text(self.row("func_80140020"), "void func_80140020(void) {\n    old();\n}\n", self.root / "decl",
                            guard_body=True)
        self.assertIn("old();", text)
        self.assertNotIn("extern s32 D_80140100;", text)  # no declaration block for the function's own guard body

    def test_unknown_function_has_no_place(self):
        self.assertIsNone(rd.unit_text(self.row("func_80149999"), "x", self.root / "decl"))


class TestPruneContext(unittest.TestCase):
    def test_guarded_bodies_keep_their_unit_text(self):
        with tempfile.TemporaryDirectory() as tmp:
            for name in ("a", "b"):
                (Path(tmp) / name).mkdir()
                (Path(tmp) / name / "context.c").write_text("TU")
            rd.prune_context(Path(tmp) / "a", rd.VARIANTS["typed"])
            rd.prune_context(Path(tmp) / "b", rd.VARIANTS["guarded"])
            self.assertFalse((Path(tmp) / "a" / "context.c").exists())
            self.assertTrue((Path(tmp) / "b" / "context.c").exists())
            rd.prune_context(Path(tmp) / "a", rd.VARIANTS["plain"])        # already gone: no error


class TestCompileCache(unittest.TestCase):
    def test_same_text_is_compiled_once_and_errors_are_cached(self):
        import order_search
        calls = []

        def fake(c, text, work, non_matching):
            calls.append(text)
            if "bad" in text:
                raise ValueError("cfe: Error: nope")
            work.mkdir(parents=True, exist_ok=True)
            (work / "guarded.o").write_bytes(b"OBJ" + text.encode())
            return str(work / "guarded.o")
        with tempfile.TemporaryDirectory() as tmp:
            old = os.getcwd()
            os.chdir(tmp)
            try:
                Path("include").mkdir()
                Path("include/a.h").write_text("x")
                c = mock.Mock(src="src/a.c")
                with mock.patch.object(order_search, "compile_variant", fake):
                    rd.install_compile_cache(Path(tmp) / "cache")
                    cached = order_search.compile_variant
                    a = cached(c, "good", Path(tmp) / "w1", False)
                    b = cached(c, "good", Path(tmp) / "w2", False)
                    self.assertEqual(Path(a).read_bytes(), Path(b).read_bytes())
                    self.assertNotEqual(a, b)
                    with self.assertRaises(ValueError) as e1:
                        cached(c, "bad", Path(tmp) / "w3", False)
                    with self.assertRaises(ValueError) as e2:
                        cached(c, "bad", Path(tmp) / "w4", False)
                    self.assertEqual(str(e1.exception), str(e2.exception))
                    cached(c, "good", Path(tmp) / "w5", True)            # another mode is another key
                self.assertEqual(calls, ["good", "bad", "good"])
                # a header change misses
                Path("include/a.h").write_text("y")
                with mock.patch.object(order_search, "compile_variant", fake):
                    rd.install_compile_cache(Path(tmp) / "cache")
                    order_search.compile_variant(c, "good", Path(tmp) / "w6", False)
                self.assertEqual(calls, ["good", "bad", "good", "good"])
            finally:
                os.chdir(old)


class TestFinish(unittest.TestCase):
    """finish() with the compile steps replaced: it picks the best variant and writes the folder."""

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        self.work, self.out = self.root / "work", self.root / "out"
        row = dict(unit="GEKO", func="func_1", scope="GEKO:func_1", src="src/ovl/GEKO/a.c", size=64, guarded=False,
                   census={"score": 30, "path": "drafts/T-14500/GEKO/func_1"},
                   archived={"score": 20, "path": "drafts/T-17700/GEKO/func_1", "file": "body.c", "ticket": "T-17700"})
        self.work.mkdir()
        (self.work / "inventory.json").write_text(json.dumps([row]))
        self.add_run("typed", 12, "typed body\n")
        self.add_run("plain", 25, "plain body\n")
        self.add_run("archived", None, "old body\n")

    def tearDown(self):
        self.tmp.cleanup()

    def add_run(self, variant, score, body):
        d = self.work / "runs" / variant / "GEKO" / "func_1"
        d.mkdir(parents=True, exist_ok=True)
        (d / "body.c").write_text(body)
        (d / "declarations.h").write_text("extern s32 D_1;\n")
        (d / "result.json").write_text(json.dumps({"score": score, "status": "scored" if score is not None else "failed",
                                                   "rewrites": ["pointer-step"] if variant == "typed" else [],
                                                   "reason": "boom"}))

    def run_finish(self, fixes, **patches):
        import draft_fixes
        scorer = mock.Mock(return_value=7)
        with mock.patch.object(rd, "unit_text", lambda row, body, folder, guard_body=False: "TU{" + body + "}"), \
                mock.patch.object(draft_fixes, "make_scorer", lambda row: scorer), \
                mock.patch.object(draft_fixes, "asm_for", lambda scope, files: None), \
                mock.patch.object(draft_fixes, "best_rewrite", patches.get("best_rewrite", mock.Mock(
                    return_value=(12, 12, [], "typed body\n", "TU{typed body\n}", {})))), \
                mock.patch("m2c.locate", side_effect=LookupError("no asm")):
            return rd.finish("GEKO:func_1", self.work, self.out, "T-20810", ("typed", "plain", "archived"), fixes)

    def test_best_variant_is_written(self):
        r = self.run_finish(False)
        d = self.out / "GEKO/func_1"
        self.assertEqual((d / "body.c").read_text(), "typed body\n")
        self.assertEqual((d / "context.c").read_text(), "TU{typed body\n}")
        self.assertTrue((d / "declarations.h").exists())
        on_disk = json.loads((d / "result.json").read_text())
        self.assertEqual(on_disk, r)
        self.assertEqual((on_disk["score"], on_disk["variant"], on_disk["census_score"]), (12, "typed", 30))
        self.assertEqual(on_disk["variant_scores"], {"typed": 12, "plain": 25, "archived": None})
        self.assertEqual((on_disk["vs_census"], on_disk["vs_archived"]), ("better", "better"))
        self.assertEqual(on_disk["rewrites"], ["pointer-step"])

    def test_draft_fixes_result_replaces_the_body_when_better(self):
        import draft_fixes
        fixed = mock.Mock(return_value=(12, 4, [("sjis-escape", 0)], "fixed body\n", "TU{fixed body\n}", {"sjis-escape#0": 4}))
        r = self.run_finish(True, best_rewrite=fixed)
        self.assertEqual((r["score"], r["variant"], r["draft_fixes"]), (4, "typed+draft-fixes", ["sjis-escape#0"]))
        self.assertEqual((self.out / "GEKO/func_1/body.c").read_text(), "fixed body\n")
        self.assertEqual(draft_fixes.best_rewrite, draft_fixes.best_rewrite)

    def test_first_write_stands_when_the_search_is_cut_short(self):
        cut = mock.Mock(side_effect=KeyboardInterrupt)
        with self.assertRaises(KeyboardInterrupt):
            self.run_finish(True, best_rewrite=cut)
        d = self.out / "GEKO/func_1"
        r = json.loads((d / "result.json").read_text())
        self.assertEqual((r["score"], r["variant"], r["status"]), (12, "typed", "scored"))
        self.assertEqual((d / "body.c").read_text(), "typed body\n")

    def test_gs_local_candidate_is_scored_and_kept_when_better(self):
        self.add_run("typed", 12, "void f(void) {\n    x = D_800E6280.unk_004;\n}\n")
        body = "void f(void) {\n    x = D_800E6280.unk_004;\n}\n"
        better = mock.Mock(return_value=(12, 12, [], body, "TU{" + body + "}", {}))
        import draft_fixes
        scorer = mock.Mock(return_value=5)
        with mock.patch.object(rd, "unit_text", lambda row, b, folder, guard_body=False: "TU{" + b + "}"), \
                mock.patch.object(draft_fixes, "make_scorer", lambda row: scorer), \
                mock.patch.object(draft_fixes, "asm_for", lambda scope, files: None), \
                mock.patch.object(draft_fixes, "best_rewrite", better), \
                mock.patch("m2c.locate", side_effect=LookupError("no asm")):
            r = rd.finish("GEKO:func_1", self.work, self.out, "T-20810", ("typed", "plain", "archived"), True)
        self.assertEqual((r["score"], r["variant"]), (5, "typed+gs-local"))
        self.assertIn("GameState *g = &D_800E6280;", (self.out / "GEKO/func_1/body.c").read_text())
        self.assertEqual(r["variant_scores"]["gs-local"], 5)

    def test_no_variant_scored_writes_a_failure(self):
        for v in ("typed", "plain"):
            p = self.work / "runs" / v / "GEKO/func_1/result.json"
            p.write_text(json.dumps({"score": None, "status": "failed", "reason": "IDO says no"}))
        r = self.run_finish(False)
        self.assertEqual((r["status"], r["score"], r["variant"]), ("failed", None, None))
        self.assertIn("IDO says no", r["provenance"]["reason"])
        self.assertTrue((self.out / "GEKO/func_1/body.c").exists())


class TestPool(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)

    def tearDown(self):
        self.tmp.cleanup()

    def task(self, name, code, after=(), timeout=20, on_fail=None):
        res = self.root / (name + ".json")
        return rd.Task(name, [sys.executable, "-c", code.replace("RES", repr(str(res)))], res, timeout, after, on_fail)

    def test_order_and_dependencies(self):
        order = self.root / "order.txt"
        w = "open(RES,'w').write('x'); open(%r,'a').write(%%r + '\\n')" % str(order)
        a = self.task("a", w % "a")
        b = self.task("b", w % "b")
        fin = self.task("fin", w % "fin", after=("a", "b"))
        rd.run_pool([fin, a, b], 2, log=lambda m: None, poll=0.01)
        seen = order.read_text().split()
        self.assertEqual(seen[-1], "fin")
        self.assertEqual(sorted(seen[:2]), ["a", "b"])

    def test_finished_results_are_not_rerun(self):
        t = self.task("a", "open(RES,'w').write('x')")
        t.result.write_text("old")
        rd.run_pool([t], 2, log=lambda m: None, poll=0.01)
        self.assertEqual(t.result.read_text(), "old")

    def test_timeout_kills_and_records_failure(self):
        reasons = []
        t = self.task("slow", "import time; time.sleep(30)", timeout=0.3, on_fail=reasons.append)
        t0 = __import__("time").monotonic()
        rd.run_pool([t], 1, log=lambda m: None, poll=0.05)
        self.assertLess(__import__("time").monotonic() - t0, 10)
        self.assertEqual(reasons, ["timeout"])

    def test_worker_that_crashes_without_a_result(self):
        reasons = []
        t = self.task("bad", "import sys; sys.exit(3)", on_fail=reasons.append)
        rd.run_pool([t], 1, log=lambda m: None, poll=0.01)
        self.assertEqual(reasons, ["exit 3"])

    def test_interrupt_kills_the_running_tasks(self):
        slow = self.task("slow", "import time; time.sleep(60)")
        started = []
        real = rd.subprocess.Popen

        def popen(*a, **k):
            started.append(real(*a, **k))
            return started[-1]
        with mock.patch.object(rd.subprocess, "Popen", popen), \
                mock.patch.object(rd.time, "sleep", side_effect=KeyboardInterrupt):
            with self.assertRaises(KeyboardInterrupt):
                rd.run_pool([slow], 1, log=lambda m: None)
        started[0].wait(timeout=10)
        self.assertIsNotNone(started[0].returncode)

    def test_jobs_bound_the_parallelism(self):
        logdir = self.root / "live"
        logdir.mkdir()
        code = ("import os,time; d=%r; open(os.path.join(d,'%%s'%%os.getpid()),'w').close(); "
                "n=len(os.listdir(d)); open(RES,'w').write(str(n)); time.sleep(0.4); os.remove(os.path.join(d,'%%s'%%os.getpid()))"
                % str(logdir))
        tasks = [self.task("t%d" % i, code) for i in range(4)]
        rd.run_pool(tasks, 2, log=lambda m: None, poll=0.02)
        self.assertLessEqual(max(int(t.result.read_text()) for t in tasks), 2)

    def test_failure_result_file_shape(self):
        row = dict(unit="GEKO", func="func_1", scope="GEKO:func_1")
        rd.write_failure(row, "typed", self.root)("timeout")
        r = json.loads((self.root / "runs/typed/GEKO/func_1/result.json").read_text())
        self.assertEqual((r["status"], r["score"], r["reason"]), ("timeout", None, "timeout"))

    def test_build_tasks_skips_finished_functions_and_unavailable_variants(self):
        rows = [dict(unit="GEKO", func="func_1", scope="GEKO:func_1", src="a.c", size=4, guarded=False, archived=None),
                dict(unit="GEKO", func="func_2", scope="GEKO:func_2", src="a.c", size=4, guarded=True,
                     archived={"path": "p", "file": "body.c", "score": 1})]
        out = self.root / "out"
        done = out / "GEKO/func_2"
        tasks = rd.build_tasks(rows, ("typed", "archived", "guarded"), self.root / "w", out, "T-1", 10, True, False)
        self.assertEqual([t.name for t in tasks], ["GEKO:func_1/typed", "GEKO:func_1/finish", "GEKO:func_2/typed",
                                                    "GEKO:func_2/archived", "GEKO:func_2/guarded", "GEKO:func_2/finish"])
        self.assertEqual(tasks[1].after, {"GEKO:func_1/typed"})
        done.mkdir(parents=True)
        (done / "result.json").write_text("{}")
        tasks = rd.build_tasks(rows, ("typed",), self.root / "w", out, "T-1", 10, True, False)
        self.assertEqual([t.name for t in tasks], ["GEKO:func_1/typed", "GEKO:func_1/finish"])
        tasks = rd.build_tasks(rows, ("typed",), self.root / "w", out, "T-1", 10, True, True)       # --force
        self.assertEqual(len(tasks), 4)
        self.assertFalse((done / "result.json").exists())            # removed, so the finish task runs again


class TestCommandLine(unittest.TestCase):
    def test_rewrite_prints_the_body_and_the_names(self):
        with tempfile.TemporaryDirectory() as tmp:
            make_tree(tmp)
            body = Path(tmp, "body.c")
            body.write_text("void f(void) {\n    Rec24 *p;\n    p += 0x24;\n}\n")
            old = os.getcwd()
            os.chdir(tmp)
            try:
                with mock.patch("sys.stdout", new=__import__("io").StringIO()) as out, \
                        mock.patch("sys.stderr", new=__import__("io").StringIO()) as err:
                    rc = rd.main(["--unit", "FOO", "--rewrite", str(body)])
            finally:
                os.chdir(old)
        self.assertEqual(rc, 0)
        self.assertIn("p += 1;", out.getvalue())
        self.assertIn("pointer-step", err.getvalue())

    def test_unknown_variant_is_an_error(self):
        with mock.patch("sys.stderr", new=__import__("io").StringIO()):
            with self.assertRaises(SystemExit):
                rd.main(["--variants", "nosuch", "--unit", "GEKO"])

    def test_no_target_is_an_error(self):
        with mock.patch("sys.stderr", new=__import__("io").StringIO()):
            with self.assertRaises(SystemExit):
                rd.main([])


class TestTypeAliases(unittest.TestCase):
    """T-20900: archived bodies that name a retired per-unit view read the shared type."""

    def test_fixed_view_and_field_names(self):
        out = rw.type_aliases("MasterSprite44 *p;\np->unk_26 = 1;\np->unk_28.w += 2;\n")
        self.assertEqual(out, "DisplayFixActor44 *p;\np->unk_24.h[1] = 1;\np->unk_28.w += 2;\n")
        out = rw.type_aliases("((NameMotionActor44 *)D_8011ECD0)[96].x.half.high = 3;")
        self.assertEqual(out, "((DisplayFixActor44 *)D_8011ECD0)[96].unk_24.h[1] = 3;")

    def test_local_typedef_and_header_types_are_kept(self):
        text = "typedef struct { s32 w[17]; } Ev44;\nEv44 a;\n"
        self.assertEqual(rw.type_aliases(text), text)
        self.assertEqual(rw.type_aliases("TelRec44 *r;", {"TelRec44": None}), "TelRec44 *r;")
        self.assertEqual(rw.type_aliases("TelRec44 *r;"), "DisplayFixActor44 *r;")

    def test_gamestate_views(self):
        out = rw.type_aliases("((NameDate *)&D_800E6280.unk_0F8)->day = ((NameDate *)&x)->month;")
        self.assertEqual(out, "((GsDateBits *)&D_800E6280.unk_0F8)->month = ((GsDateBits *)&x)->day;")


if __name__ == "__main__":
    unittest.main()
