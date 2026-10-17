#!/usr/bin/env python3
"""Tests for tools/migrate_globals.py on a synthetic tree (no game data).

Run: tools/docker.sh python3 tools/test_migrate_globals.py
"""
import os
import sys
import tempfile
import types
import unittest
from unittest import mock

import migrate_globals as mg

HEADER = """#ifndef MAIN_API_H
#define MAIN_API_H
typedef union W {
    /* 0x00 */ s32 w;
    /* 0x00 */ u8 b[4];
} W; /* size 0x04 */
typedef struct Rec {
    /* 0x00 */ s16 a;
    /* 0x02 */ u8 b;
    /* 0x03 */ u8 c;
} Rec; /* size 0x04 */
typedef struct Flags {
    u32 x : 3;
    u32 y : 5;
} Flags;
typedef struct State {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ s8 unk_01;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ W unk_04;
    /* 0x08 */ Rec unk_08[4];
    /* 0x18 */ u8 unk_18[8];
} State; /* size 0x20 */
extern State D_80100000;
extern s8 D_80100001;
extern s16 D_80100002;
#ifndef MAIN_API_OVERRIDE_D_80100004
extern s32 D_80100004;
#endif
extern u8 D_80100007;
extern s16 D_8010000C;
extern u8 D_80100018[];
extern u8 D_8010001A[];
extern s32 D_80200000;
#endif
"""

CONFIG = "aggregate D_80100000 State include/main_api.h\n"


class MigrateTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = self.tmp.name
        for d in ("include/ovl", "src/main", "src/ovl/A", "config"):
            os.makedirs(os.path.join(self.root, d))
        self.put("include/main_api.h", HEADER)
        self.put("config/migrate_globals.txt", CONFIG)

    def tearDown(self):
        self.tmp.cleanup()

    def put(self, rel, text):
        with open(os.path.join(self.root, rel), "w") as f:
            f.write(text)

    def get(self, rel):
        with open(os.path.join(self.root, rel)) as f:
            return f.read()

    def apply(self):
        return mg.main(["--root", self.root, "--apply"])

    def test_layout(self):
        aggs, _ = mg.load_config(self.root)
        t = aggs[0].type
        self.assertEqual(t.size, 0x20)
        paths = [p for p, _t in mg.subobjects(t, 0x0F)]
        self.assertEqual(paths[-1], ".unk_08[1].c")
        self.assertEqual(mg.parse_types("typedef struct F { u32 x : 3; u32 y : 30; } F;")["F"].size, 8)

    def test_bad_offset_comment(self):
        types = mg.parse_types("typedef struct B { /* 0x00 */ u8 a; /* 0x02 */ s16 b; /* 0x03 */ u8 c; } B;")
        self.assertIsInstance(types["B"], mg.LayoutError)

    def test_apply_scalars_unions_records_arrays(self):
        self.put("src/main/a.c", '#include "main_api.h"\n'
                 "void f(s32 i) {\n"
                 "    D_80100001 = D_80100002 + D_80100004;\n"
                 "    D_80100007 |= 1; /* D_80100007 in a comment stays */\n"
                 "    D_8010000C += 1;\n"
                 "    D_80100018[i] = D_8010001A[i];\n"
                 "    D_8010001A[1] = 0;\n"
                 "    g(D_8010001A, &D_8010001A);\n"
                 "    D_80200000 = 0;\n"
                 "}\n")
        self.assertEqual(self.apply(), 0)
        out = self.get("src/main/a.c")
        self.assertIn("D_80100000.unk_01 = D_80100000.unk_02 + D_80100000.unk_04.w;", out)
        self.assertIn("D_80100000.unk_04.b[3] |= 1; /* D_80100007 in a comment stays */", out)
        self.assertIn("D_80100000.unk_08[1].a += 1;", out)
        self.assertIn("D_80100000.unk_18[i] = D_80100000.unk_18[i + 2];", out)
        self.assertIn("D_80100000.unk_18[3] = 0;", out)
        self.assertIn("g(&D_80100000.unk_18[2], &D_80100000.unk_18[2]);", out)
        self.assertIn("D_80200000 = 0;", out)
        api = self.get("include/main_api.h")
        for gone in ("D_80100001;", "D_80100004;", "MAIN_API_OVERRIDE_D_80100004", "D_8010001A[]"):
            self.assertNotIn(gone, api)
        self.assertIn("extern State D_80100000;", api)
        self.assertIn("extern s32 D_80200000;", api)
        # idempotent, and the check is clean
        self.assertEqual(self.apply(), 0)
        self.assertEqual(self.get("src/main/a.c"), out)
        self.assertEqual(mg.check(self.root, *mg.load_config(self.root)), [])

    def test_override_view_of_a_union_member(self):
        self.put("include/ovl/A.h", "#define MAIN_API_OVERRIDE_D_80100004 /* matched as u8 */\n"
                 '#include "main_api.h"\nextern u8 D_80100004;\n')
        self.put("src/ovl/A/a.c", '#include "ovl/A.h"\nvoid f(void) { D_80100004 = 1; }\n')
        self.assertEqual(self.apply(), 0)
        self.assertIn("D_80100000.unk_04.b[0] = 1;", self.get("src/ovl/A/a.c"))
        self.assertNotIn("D_80100004", self.get("include/ovl/A.h"))

    def test_type_mismatch_is_left_and_declaration_kept(self):
        self.put("src/main/a.c", '#include "main_api.h"\nvoid f(void) { D_8010000C = 1; }\n')
        self.put("include/ovl/A.h", '#include "main_api.h"\n')
        api = self.get("include/main_api.h").replace("extern s16 D_8010000C;", "extern s32 D_8010000C;")
        self.put("include/main_api.h", api)
        self.assertEqual(self.apply(), 1)
        self.assertIn("D_8010000C = 1;", self.get("src/main/a.c"))
        self.assertIn("extern s32 D_8010000C;", self.get("include/main_api.h"))

    def test_new_code_without_declaration(self):
        api = self.get("include/main_api.h")
        for line in ("extern s16 D_80100002;\n", "extern s16 D_8010000C;\n"):
            api = api.replace(line, "")
        self.put("include/main_api.h", api)
        self.put("src/main/a.c", '#include "main_api.h"\nvoid f(void) { D_80100002 = 1; D_80100008 = 2; }\n')
        self.assertEqual(self.apply(), 1)            # D_80100008: record, array and s16 start there
        out = self.get("src/main/a.c")
        self.assertIn("D_80100000.unk_02 = 1;", out)
        self.assertIn("D_80100008 = 2;", out)
        msgs = mg.check(self.root, *mg.load_config(self.root))
        self.assertTrue(any("D_80100008" in m and "no field fits" in m for m in msgs))

    def test_own_declarations_of_new_code(self):
        # m2c output pasted with its extern lines: the declarations give the view and are deleted
        self.put("src/main/a.c", '#include "main_api.h"\n'
                 "extern s8 D_80100004;\nextern u8 D_8010001C;\n"
                 "void f(void) {\n    D_80100004 = 1;\n    D_8010001C += 1;\n}\n")
        self.assertEqual(self.apply(), 0)
        out = self.get("src/main/a.c")
        self.assertNotIn("extern", out)
        self.assertIn("D_80100000.unk_04.b[0] = 1;", out)       # only stored: u8 byte fits the s8 view
        self.assertIn("D_80100000.unk_18[4] += 1;", out)

    def test_sign_mismatch_with_a_load_is_left(self):
        self.put("src/main/a.c", '#include "main_api.h"\nextern s8 D_80100004;\n'
                 "s32 f(void) {\n    return D_80100004;\n}\n")
        self.assertEqual(self.apply(), 1)
        self.assertIn("return D_80100004;", self.get("src/main/a.c"))

    def put_asm(self, hoisted):
        """Original asm of object 80010000: D_80100002 += 1; D_80100004 += 1; with the second load
        above the first store (separate symbols, as1) or after it (one symbol)."""
        x, y = "D_80100002", "D_80100004"
        lx = ["lui $t6, %%hi(%s)" % x, "lh $t6, %%lo(%s)($t6)" % x]
        sx = ["lui $at, %%hi(%s)" % x, "addiu $t7, $t6, 1", "sh $t7, %%lo(%s)($at)" % x]
        ly = ["lui $t8, %%hi(%s)" % y, "lw $t8, %%lo(%s)($t8)" % y]
        sy = ["lui $at, %%hi(%s)" % y, "addiu $t9, $t8, 1", "sw $t9, %%lo(%s)($at)" % y]
        body = lx + ly + sx + sy if hoisted else lx + sx + ly + sy
        d = os.path.join(self.root, "asm", "nonmatchings", "main", "80010000")
        os.makedirs(d, exist_ok=True)
        lines = ["glabel f"] + ["    /* 0 %08X 00000000 */  %s" % (0x80010000 + 4 * i, b) for i, b in enumerate(body)]
        self.put("asm/nonmatchings/main/80010000/f.s", "\n".join(lines) + "\nendlabel f\n")

    def test_keep_needs_a_hoisted_pair(self):
        self.put("config/migrate_globals.txt", CONFIG + "keep src/main/80010000.c D_80100002 reason\n")
        self.put("src/main/80010000.c", '#include "main_api.h"\nvoid f(void) { D_80100002 = 1; }\n')
        self.assertEqual(self.apply(), 0)
        msgs = mg.check(self.root, *mg.load_config(self.root))
        self.assertEqual(len(msgs), 1)
        self.assertIn("no asm of the original object", msgs[0])
        self.put_asm(hoisted=False)
        msgs = mg.check(self.root, *mg.load_config(self.root))
        self.assertEqual(len(msgs), 1)
        self.assertIn("no evidence for a separate symbol", msgs[0])
        self.put_asm(hoisted=True)
        self.assertEqual(mg.check(self.root, *mg.load_config(self.root)), [])

    def test_keep_and_base_views(self):
        self.put_asm(hoisted=True)
        self.put("config/migrate_globals.txt", CONFIG + "keep src/main/80010000.c D_80100002 matched only as a scalar\n")
        self.put("src/main/80010000.c", '#include "main_api.h"\nvoid f(void) { D_80100002 = 1; }\n')
        self.put("src/main/b.c", '#include "main_api.h"\nvoid g(s32 i) { D_80100002 = 2; D_80100000[i] = 0; '
                 "h(&D_80100000); D_80100000.unk_00 = 1; }\n")
        self.assertEqual(self.apply(), 1)            # the u8[] view of the base needs a hand rewrite
        self.assertIn("D_80100002 = 1;", self.get("src/main/80010000.c"))
        b = self.get("src/main/b.c")
        self.assertIn("D_80100000.unk_02 = 2;", b)
        self.assertIn("D_80100000[i] = 0;", b)
        self.assertIn("extern s16 D_80100002;", self.get("include/main_api.h"))
        msgs = mg.check(self.root, *mg.load_config(self.root))
        self.assertEqual(len(msgs), 1)
        self.assertIn("src/main/b.c:2: D_80100000 is the State struct", msgs[0])


    # ---- keep lines backed by a byte-match necessity proof (T-20820) ----

    FUNC_ROWS = ["3C0E8010", "8DCE0004", "25CF0001", "3C018010", "AC2F0004", "03E00008", "00000000"]
    PLAIN = '#include "main_api.h"\nvoid f(void) { D_80100002 = 1; }\n'

    def put_func_asm(self, words=None, folder="matchings", name="f"):
        """Original asm of `name` (plain C, matched) in object 80010000, one row per word."""
        words = words or self.FUNC_ROWS
        d = os.path.join(self.root, "asm", folder, "main", "80010000")
        os.makedirs(d, exist_ok=True)
        rows = ["    /* %X %08X %s */  nop" % (4 * i, 0x80010000 + 4 * i, w) for i, w in enumerate(words)]
        self.put("asm/%s/main/80010000/%s.s" % (folder, name),
                 "\n".join(["glabel " + name] + rows + ["endlabel " + name]) + "\n")

    def proof_tree(self, src=None, sha=None, rows=5, asm=True):
        """A tree whose keep line cites a proof for f; returns the sha of f's asm (or None)."""
        if asm:
            self.put_func_asm()
        real = mg.asm_sha1(self.get("asm/matchings/main/80010000/f.s")) if asm else "0123abcd"
        self.put("config/migrate_globals.txt", CONFIG + "keep src/main/80010000.c D_80100002 proof=f:%d:%s "
                 "second uopt variable\n" % (rows, sha or real))
        self.put("src/main/80010000.c", src or self.PLAIN)
        self.assertEqual(self.apply(), 0)           # folds the other absorbed names, keeps D_80100002
        return real

    def check_lines(self):
        return mg.check(self.root, *mg.load_config(self.root))

    def test_parse_old_and_new_keep_syntax(self):
        self.put("config/migrate_globals.txt", CONFIG +
                 "keep src/main/a.c D_80100002 plain reason words\n"
                 "keep src/main/b.c D_80100004 proof=f:242:0123abcd second uopt variable\n")
        _aggs, keep = mg.load_config(self.root)
        self.assertEqual(keep["src/main/a.c"], {"D_80100002"})
        self.assertEqual(keep["src/main/a.c"].proofs, {})
        self.assertEqual(keep["src/main/b.c"], {"D_80100004"})
        proof = keep["src/main/b.c"].proofs["D_80100004"][0]
        self.assertEqual((proof.func, proof.rows, proof.sha), ("f", 242, "0123abcd"))
        self.assertEqual(keep["src/main/a.c"].plain, {"D_80100002"})
        self.assertEqual(keep["src/main/b.c"].plain, set())
        self.assertEqual(str(proof), "proof=f:242:0123abcd")

    def test_malformed_proof_is_a_config_error(self):
        for bad in ("proof=f:0:0123abcd reason",          # necessity needs at least one differing row
                    "proof=f:3:0123ABCD reason",          # upper case digits
                    "proof=f:3:0123abc reason",           # seven digits
                    "proof=f:3:0123abcd",                 # no reason
                    "proof=f:x:0123abcd reason",
                    "proof=:3:0123abcd reason"):
            self.put("config/migrate_globals.txt", CONFIG + "keep src/main/a.c D_80100002 " + bad + "\n")
            with self.assertRaises(SystemExit, msg=bad):
                mg.load_config(self.root)

    def test_asm_sha1_hashes_the_words_only(self):
        a = ("glabel f\n    /* 0 80010000 3C0E8010 */  lui $t6, %hi(D_80100002)\n"
             "    /* 4 80010004 03E00008 */  jr $ra\nendlabel f\n")
        renamed = a.replace("D_80100002", "counter").replace("80010000", "80020000")
        self.assertEqual(mg.asm_sha1(a), mg.asm_sha1(renamed))
        self.assertEqual(len(mg.asm_sha1(a)), 8)
        self.assertNotEqual(mg.asm_sha1(a), mg.asm_sha1(a.replace("3C0E8010", "3C0E8011")))
        rodata = a + "glabel jtbl\n    /* 8 80010008 00000000 */  .word 0\n"
        self.assertEqual(mg.asm_sha1(a), mg.asm_sha1(rodata))

    def test_proof_accepts_plain_c_with_matching_sha(self):
        sha = self.proof_tree()
        self.assertEqual(self.check_lines(), [])
        aggs, _keep = mg.load_config(self.root)
        self.assertEqual(mg.keep_evidence(self.root, aggs, "src/main/80010000.c", "D_80100002",
                                          mg.Proof("f", 5, sha)), (True, "f"))

    def test_stale_sha_is_rejected(self):
        self.proof_tree(sha="0123abcd")
        msgs = self.check_lines()
        self.assertEqual(len(msgs), 1)
        self.assertIn("proof=f:5:0123abcd is stale", msgs[0])
        self.assertIn(mg.asm_sha1(self.get("asm/matchings/main/80010000/f.s")), msgs[0])

    def test_changed_original_makes_the_line_stale(self):
        self.proof_tree()
        self.assertEqual(self.check_lines(), [])
        self.put_func_asm(words=self.FUNC_ROWS[:-1] + ["00000001"])
        self.assertIn("is stale", self.check_lines()[0])

    def test_include_asm_or_guarded_function_is_rejected(self):
        for src in ('#include "main_api.h"\nINCLUDE_ASM("asm/nonmatchings/main/80010000", f);\n',
                    '#include "main_api.h"\n#ifdef NON_MATCHING\nvoid f(void) { D_80100002 = 1; }\n#else\n'
                    'INCLUDE_ASM("asm/nonmatchings/main/80010000", f);\n#endif\n',
                    '#include "main_api.h"\nvoid g(void) { D_80100002 = 1; }\n'):
            self.proof_tree(src=src)
            msgs = self.check_lines()
            self.assertIn("is not plain C", msgs[0], src)
            self.assertTrue(all("not plain C" in m or "used outside f" in m for m in msgs), msgs)

    def test_proof_without_original_asm_is_rejected(self):
        self.proof_tree(asm=False)
        msgs = self.check_lines()
        self.assertEqual(len(msgs), 1)
        self.assertIn("no original asm of f", msgs[0])

    def test_hoisted_pair_evidence_is_unchanged_by_the_proof_path(self):
        self.put_asm(hoisted=True)
        self.put("config/migrate_globals.txt", CONFIG + "keep src/main/80010000.c D_80100002 plain reason\n")
        self.put("src/main/80010000.c", self.PLAIN)
        self.assertEqual(self.apply(), 0)
        self.assertEqual(self.check_lines(), [])
        # a proof line is backed by the hoisted pair of its object as well: either evidence suffices
        self.put("config/migrate_globals.txt", CONFIG +
                 "keep src/main/80010000.c D_80100002 proof=f:5:0123abcd reason\n")
        self.assertEqual(self.check_lines(), [])
        # and a plain keep line without any evidence keeps failing as before
        self.put_asm(hoisted=False)
        self.put("config/migrate_globals.txt", CONFIG + "keep src/main/80010000.c D_80100002 plain reason\n")
        msgs = self.check_lines()
        self.assertEqual(len(msgs), 1)
        self.assertIn("no evidence for a separate symbol", msgs[0])
        self.assertIn("keepproof.py", msgs[0])

    def test_verify_proofs_runs_the_recompiling_check(self):
        self.proof_tree()
        fake = types.ModuleType("keepproof")
        fake.verify_config = mock.Mock(return_value=[])
        with mock.patch.dict(sys.modules, {"keepproof": fake}):
            self.assertEqual(mg.main(["--root", self.root, "--verify-proofs"]), 0)
            fake.verify_config.assert_called_once()
            fake.verify_config.return_value = ["config/migrate_globals.txt: proof not reproduced"]
            self.assertEqual(mg.main(["--root", self.root, "--verify-proofs"]), 1)
            fake.verify_config.reset_mock()
            self.assertEqual(mg.main(["--root", self.root, "--check"]), 0)       # --check never compiles
            fake.verify_config.assert_not_called()

    TWO = ('#include "main_api.h"\nvoid f(void) { D_80100002 = 1; }\nvoid g(void) { D_80100002 = 2; }\n'
           'void h(void) { D_80100000.unk_02 = 3; }\n')

    def put_two_func_asm(self):
        self.put_func_asm()
        self.put_func_asm(words=self.FUNC_ROWS[:-1] + ["00000002"], name="g")
        d = "asm/matchings/main/80010000/"
        return (mg.asm_sha1(self.get(d + "f.s")), mg.asm_sha1(self.get(d + "g.s")))

    def test_one_proof_line_per_function_of_the_same_symbol(self):
        sf, sg = self.put_two_func_asm()
        self.put("config/migrate_globals.txt", CONFIG +
                 "keep src/main/80010000.c D_80100002 proof=f:5:%s first\n"
                 "keep src/main/80010000.c D_80100002 proof=g:9:%s second\n" % (sf, sg))
        self.put("src/main/80010000.c", self.TWO)
        self.assertEqual(self.apply(), 0)
        _aggs, keep = mg.load_config(self.root)
        self.assertEqual([p.func for p in keep["src/main/80010000.c"].proofs["D_80100002"]], ["f", "g"])
        self.assertEqual(self.check_lines(), [])
        # g's original changes: its line goes stale, f's stays good
        self.put_func_asm(words=self.FUNC_ROWS[:-1] + ["00000003"], name="g")
        msgs = self.check_lines()
        self.assertEqual(len(msgs), 1)
        self.assertIn("proof=g:9:", msgs[0])

    def test_kept_symbol_used_outside_its_proven_function_is_flagged(self):
        sf, _sg = self.put_two_func_asm()
        self.put("config/migrate_globals.txt", CONFIG +
                 "keep src/main/80010000.c D_80100002 proof=f:5:%s first\n" % sf)
        self.put("src/main/80010000.c", self.TWO)
        self.assertEqual(self.apply(), 0)                     # --apply leaves the kept symbol everywhere
        msgs = self.check_lines()
        self.assertEqual(len(msgs), 1)
        self.assertIn("src/main/80010000.c:3: keep D_80100002 used outside f", msgs[0])
        # a plain (hoisted pair) line for the same symbol covers the whole file
        self.put_asm(hoisted=True)
        self.put("config/migrate_globals.txt", CONFIG +
                 "keep src/main/80010000.c D_80100002 proof=f:5:%s first\n"
                 "keep src/main/80010000.c D_80100002 plain reason\n" % sf)
        self.assertEqual(self.check_lines(), [])

    def test_function_span(self):
        text = 'void f(void) {\n    s("}"); /* } */\n    if (1) { g(); }\n}\nvoid h(void) {\n}\n'
        s, e = mg.function_span(text, "f")
        self.assertEqual(text[s:e].count("{"), 2)
        self.assertEqual(text[e:].lstrip(), "void h(void) {\n}\n")
        self.assertIsNone(mg.function_span(text, "zz"))

    def test_plain_c(self):
        self.assertTrue(mg.plain_c("void f(void) {\n}\n", "f"))
        self.assertTrue(mg.plain_c("s32 f(a)\ns32 a;\n{\n return a;\n}\n", "f"))
        self.assertFalse(mg.plain_c("INCLUDE_ASM(\"asm/x\", f);\n", "f"))
        self.assertFalse(mg.plain_c("#ifdef NON_MATCHING\nvoid f(void) {\n}\n#else\n"
                                    "INCLUDE_ASM(\"a\", f);\n#endif\n", "f"))
        self.assertFalse(mg.plain_c("void g(void) {\n f();\n}\n", "f"))


if __name__ == "__main__":
    unittest.main()
