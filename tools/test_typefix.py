"""Unit tests for tools/typefix.py (synthetic C only, no game data, no compiler).

Run: python3 tools/test_typefix.py   (from the repo root; no Docker needed)
"""
import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import autotweak  # noqa: E402
import typefix as T  # noqa: E402

GLOBALS = {
    "D_8015E740": ("s32", 0, []),       # a word holding an address
    "D_8013D98C": ("u8", 1, []),
    "D_800E71DF": ("u8", 0, []),
    "D_8015B560": ("s32", 0, []),
    "D_800CA22C": ("s16", 1, []),
    "D_80125BF4": ("s32", 0, []),
    "D_8014A13C": ("s32", 0, []),
    "D_80146804": ("s32", 0, []),
}


def fn(*lines, head="void f(s32 arg0, s32 arg1) {"):
    return head + "\n" + "\n".join("    " + l for l in lines) + "\n}\n"


class DerefTest(unittest.TestCase):
    def fix(self, *lines, head="void f(s32 arg0, s32 arg1) {", types=None):
        return T.fix_expressions(fn(*lines, head=head), types or GLOBALS)

    def test_integer_global_plus_offset(self):
        out = self.fix("s32 temp_v0;", "temp_v0 = *(D_8015E740 + (arg0 * 4));")
        self.assertIn("temp_v0 = *(s32 *)(D_8015E740 + (arg0 * 4));", out)

    def test_nested_integer_loads(self):
        out = self.fix("s32 t;", "t = *(*(D_8015E740 + arg0) + (arg1 * 4));")
        self.assertIn("t = *(s32 *)(*(s32 *)(D_8015E740 + arg0) + (arg1 * 4));", out)

    def test_cast_gives_the_access_width(self):
        out = self.fix("D_800CA22C[0] = (s16) *((D_800E71DF * 6) + D_8015B560 + arg0);")
        self.assertIn("(s16) *(s16 *)((D_800E71DF * 6) + D_8015B560 + arg0);", out)

    def test_store_through_integer_local(self):
        out = self.fix("s32 var_s0;", "var_s0 = arg0;", "*var_s0 = (s32) (arg1 + 1);")
        self.assertIn("*(s32 *)var_s0 = (s32) (arg1 + 1);", out)

    def test_store_uses_the_cast_of_the_value(self):
        out = self.fix("s32 var_s0;", "*var_s0 = (u8) arg1;")
        self.assertIn("*(u8 *)var_s0 = (u8) arg1;", out)

    def test_pointer_table_entry_is_read_as_a_word(self):
        out = self.fix("u8 *q;", "if (**(q + arg0) == 0x67) {", "}")
        self.assertIn("if (*(s32 *)*(s32 *)(q + arg0) == 0x67) {", out)

    def test_valid_derefs_are_untouched(self):
        src = fn("u8 *q;", "s32 v;", "v = *q;", "v = *(q + arg0);", "v = *(s32 *)(arg0 + 4);",
                 "v = *D_800CA22C;", "v = D_8013D98C[arg0];")
        self.assertEqual(T.fix_expressions(src, GLOBALS), src)

    def test_unknown_operands_are_untouched(self):
        src = fn("s32 v;", "v = *func_80012345(arg0);", "v = *unknown_sym;", "v = *(x.y + 4);")
        self.assertEqual(T.fix_expressions(src, GLOBALS), src)

    def test_condition_return_and_for(self):
        out = self.fix("s32 i;", "for (i = *D_80125BF4; i < 4; i++) {", "}", "return *(D_80125BF4 + i);",
                       head="s32 f(s32 arg0, s32 arg1) {")
        self.assertIn("i = *(s32 *)D_80125BF4;", out)
        self.assertIn("return *(s32 *)(D_80125BF4 + i);", out)

    def test_goto_labels_and_cases_parse(self):
        out = self.fix("s32 v;", "switch (arg0) {", "case 1:", "    v = *D_80125BF4;", "    break;", "default:",
                       "    goto end;", "}", "end:", "v = 0;")
        self.assertIn("v = *(s32 *)D_80125BF4;", out)


class CallTest(unittest.TestCase):
    def test_call_through_stack_table(self):
        src = fn("s32 sp2C[16];", "(&sp2C[0])[D_800E71DF](0x80);", "arg0 = (&sp2C[0])[arg1](0x3C);")
        out = T.fix_expressions(src, GLOBALS)
        self.assertIn("((s32 (*)())(&sp2C[0])[D_800E71DF])(0x80);", out)
        self.assertIn("arg0 = ((s32 (*)())(&sp2C[0])[arg1])(0x3C);", out)

    def test_call_of_an_integer_variable(self):
        out = T.fix_expressions(fn("D_8014A13C(arg0);"), GLOBALS)
        self.assertIn("((s32 (*)())D_8014A13C)(arg0);", out)

    def test_ordinary_calls_are_untouched(self):
        src = fn("func_80012345(arg0, 2);", "D_800CA22C[arg0] = func_80012346(arg1);")
        self.assertEqual(T.fix_expressions(src, GLOBALS), src)

    def test_block_copy_grows_the_stack_array(self):
        src = fn("s32 sp2C[16];", "typedef struct { s32 w[40]; } M2cBlockA0;",
                 "*(M2cBlockA0 *)(&sp2C[0]) = *(M2cBlockA0 *)(&D_80146804);")
        self.assertIn("s32 sp2C[40];", T.late(src, GLOBALS))
        small = fn("s32 sp2C[64];", "*(M2cBlockA0 *)(&sp2C[0]) = *(M2cBlockA0 *)(&D_80146804);")
        self.assertIn("s32 sp2C[64];", T.late(small, GLOBALS))


class VoidTest(unittest.TestCase):
    def test_arithmetic_on_a_void_pointer(self):
        src = fn("u8 *a;", "s32 b;", "a = arg0 + 3;", "b = (arg1 * 2) + arg0;", head="void f(void *arg0, s32 arg1) {")
        out = T.fix_expressions(src, GLOBALS)
        self.assertIn("a = ((u8 *)arg0) + 3;", out)
        self.assertIn("b = (arg1 * 2) + ((u8 *)arg0);", out)

    def test_comparisons_and_calls_keep_the_void_pointer(self):
        src = fn("if (arg0 == 0) {", "}", "func_80012345(arg0);", head="void f(void *arg0, s32 arg1) {")
        self.assertEqual(T.fix_expressions(src, GLOBALS), src)


class TextRewriteTest(unittest.TestCase):
    def test_unset_register_arguments_are_dropped(self):
        e = "M2C_ERROR(/* Read from unset register $a%d */)"
        self.assertEqual(T.drop_unset_args("f(x, %s, %s);" % (e % 1, e % 2)), "f(x);")
        self.assertEqual(T.drop_unset_args("f(%s, %s);" % (e % 0, e % 1)), "f();")
        keep = "f(%s, y);" % (e % 0)
        self.assertEqual(T.drop_unset_args(keep), keep)

    def test_unaligned_marker(self):
        self.assertEqual(T.strip_unaligned("a = (s32) (unaligned s32) *(s32 *)p;"), "a = (s32) *(s32 *)p;")
        self.assertEqual(T.strip_unaligned("f((unaligned s32) sp50, 1);"), "f(sp50, 1);")

    def test_label_before_a_closing_brace(self):
        out = T.fix_labels(fn("switch (arg0) {", "case 1:", "    arg1 = 2;", "    break;", "default:", "}"))
        self.assertIn("default: ;\n    }", out)
        self.assertEqual(T.fix_labels(fn("arg1 = arg0 ? 1 : 2;")), fn("arg1 = arg0 ? 1 : 2;"))
        ok = fn("switch (arg0) {", "case 1:", "    arg1 = 2;", "}")
        self.assertEqual(T.fix_labels(ok), ok)

    def test_switch_default_labels(self):
        src = fn("if (arg0) {", "    switch (arg1) {", "    case 1:", "        break;", "    }", "} else {", "default:", "}")
        out = T.fix_switch_labels(src)
        self.assertNotIn("default", out)
        self.assertNotIn("else", out)
        dup = fn("switch (arg0) {", "default:", "default:", "    return;", "}")
        self.assertEqual(T.fix_switch_labels(dup).count("default:"), 1)

    def test_renamed_symbols_use_the_new_name(self):
        table = {"func_800573F8": "dec_bg_show_switch", "D_800B3634": "g_flag", "func_80012345": "mine"}
        src = "void func_80012345(void) {\n    func_800573F8(1);\n    D_800B3634 = 0;\n    func_80099999();\n}\n"
        out = T.rename_symbols(src, table, keep=("func_80012345",))
        self.assertIn("dec_bg_show_switch(1);", out)
        self.assertIn("g_flag = 0;", out)
        self.assertIn("void func_80012345(void)", out)
        self.assertIn("func_80099999();", out)

    def test_early_stage_combines_the_rewrites(self):
        e = "M2C_ERROR(/* Read from unset register $a1 */)"
        src = "void func_80012345(void) {\n    func_800573F8(1, %s);\n}\n" % e
        out = T.early(src, {"func_800573F8": "dec_bg_show_switch"})
        self.assertIn("dec_bg_show_switch(1);", out)

    def test_read_renames(self):
        import tempfile
        with tempfile.TemporaryDirectory() as d:
            os.makedirs(os.path.join(d, "config"))
            with open(os.path.join(d, "config", "obin_renames.txt"), "w") as fh:
                fh.write("# old new\nfunc_800573F8 dec_bg_show_switch\n\n")
            self.assertEqual(T.read_renames(d), {"func_800573F8": "dec_bg_show_switch"})


class EditTest(unittest.TestCase):
    def setUp(self):
        T._GLOBALS[os.path.abspath(".")] = {
            "D_800E71DF": ("u8", 0, ()), "D_800CA22C": ("s16", 0, ()), "D_8015B560": ("s16", 0, ("[4]",))}

    def labels(self, tr, src):
        return {v.label.split()[0]: v for v in tr(autotweak.Func(src))}

    def texts(self, tr, src):
        return [v.text for v in tr(autotweak.Func(src))]

    def test_registered_in_autotweak(self):
        for tr in T.TRANSFORMS:
            self.assertIn(tr, autotweak.TRANSFORMS)
        self.assertIn("cast", autotweak.HINT_EDITS["structure"])
        self.assertEqual(autotweak.edit_kind("castflip L4 (u8)->s8"), "castflip")

    def test_variants_reach_the_solver(self):
        src = fn("s16 v;", "v = arg0 >> 2;")
        kinds = {autotweak.edit_kind(v.label) for v in autotweak.variants(src)}
        self.assertTrue({"cast", "decl"} <= kinds)

    def test_cast_added_to_a_read(self):
        src = fn("s32 v;", "v = arg1 + 1;")
        texts = self.texts(T.t_castadd, src)
        self.assertIn(fn("s32 v;", "v = (u8)arg1 + 1;"), texts)
        self.assertIn(fn("s32 v;", "v = (s16)arg1 + 1;"), texts)

    def test_compare_and_shift_get_word_casts(self):
        src = fn("if (arg0 < 4) {", "}", "arg1 = arg0 >> 2;")
        texts = self.texts(T.t_castadd, src)
        self.assertIn(fn("if ((u32)arg0 < 4) {", "}", "arg1 = arg0 >> 2;"), texts)
        self.assertIn(fn("if (arg0 < 4) {", "}", "arg1 = (u32)arg0 >> 2;"), texts)
        self.assertNotIn(fn("if ((u8)arg0 < 4) {", "}", "arg1 = arg0 >> 2;"), texts)

    def test_stores_and_callees_get_no_casts(self):
        src = fn("arg0 = 1;", "func_80012345(arg1);")
        texts = self.texts(T.t_castadd, src)
        self.assertFalse(any("(u8)arg0 =" in t for t in texts))
        self.assertTrue(any("func_80012345((u8)arg1)" in t for t in texts))

    def test_cast_dropped_and_flipped(self):
        src = fn("arg1 = (u8) arg0;", "arg0 = (u32) arg1 >> 4;")
        texts = self.texts(T.t_castflip, src)
        self.assertIn(fn("arg1 = arg0;", "arg0 = (u32) arg1 >> 4;"), texts)
        self.assertIn(fn("arg1 = (s8)arg0;", "arg0 = (u32) arg1 >> 4;"), texts)
        self.assertIn(fn("arg1 = (u8) arg0;", "arg0 = (s32)arg1 >> 4;"), texts)
        self.assertIn(fn("arg1 = (u8) arg0;", "arg0 = arg1 >> 4;"), texts)

    def test_mask_and_cast_trade_places(self):
        src = fn("arg1 = (arg0 + 1) & 0xFF;", "arg0 = (u16) arg1;")
        texts = self.texts(T.t_mask, src)
        self.assertIn(fn("arg1 = (u8)(arg0 + 1);", "arg0 = (u16) arg1;"), texts)
        self.assertIn(fn("arg1 = (arg0 + 1);", "arg0 = (u16) arg1;"), texts)
        self.assertIn(fn("arg1 = (arg0 + 1) & 0xFF;", "arg0 = (arg1 & 0xFFFF);"), texts)

    def test_global_view(self):
        src = fn("arg0 = D_800E71DF + D_8015B560[arg1];", "D_800CA22C = 2;")
        texts = self.texts(T.t_view, src)
        self.assertIn(fn("arg0 = (*(s8 *)&D_800E71DF) + D_8015B560[arg1];", "D_800CA22C = 2;"), texts)
        self.assertIn(fn("arg0 = D_800E71DF + ((u16 *)D_8015B560)[arg1];", "D_800CA22C = 2;"), texts)
        self.assertFalse(any("&D_800CA22C" in t for t in texts))

    def test_access_type(self):
        src = fn("arg1 = *(s16 *)(arg0 + 4);")
        texts = self.texts(T.t_access, src)
        self.assertIn(fn("arg1 = *(u16 *)(arg0 + 4);"), texts)
        self.assertIn(fn("arg1 = *(s8 *)(arg0 + 4);"), texts)

    def test_unsigned_suffix(self):
        src = fn("if (arg0 < 0x10) {", "}", "if (arg1 >= 3U) {", "}")
        texts = self.texts(T.t_usuffix, src)
        self.assertIn(fn("if (arg0 < 0x10U) {", "}", "if (arg1 >= 3U) {", "}"), texts)
        self.assertIn(fn("if (arg0 < 0x10) {", "}", "if (arg1 >= 3) {", "}"), texts)

    def test_declared_types(self):
        src = fn("u8 v;", "v = arg0;")
        texts = self.texts(T.t_decl, src)
        self.assertIn(fn("u16 v;", "v = arg0;"), texts)
        self.assertNotIn(fn("s8 v;", "v = arg0;"), texts)      # t_width already tries the sign pair

    def test_edits_keep_the_function_parsable(self):
        src = fn("s16 v;", "v = D_800E71DF + (arg1 & 0xFF);", "if ((u8) v < 0x10) {", "}")
        for tr in T.TRANSFORMS:
            for v in tr(autotweak.Func(src)):
                autotweak.Func(v.text)


if __name__ == "__main__":
    unittest.main()
