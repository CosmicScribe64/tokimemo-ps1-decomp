#!/usr/bin/env python3
"""Tests for tools/migrate_globals.py on a synthetic tree (no game data).

Run: tools/docker.sh python3 tools/test_migrate_globals.py
"""
import os
import tempfile
import unittest

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

    def test_keep_and_base_views(self):
        self.put("config/migrate_globals.txt", CONFIG + "keep src/main/a.c D_80100002 matched only as a scalar\n")
        self.put("src/main/a.c", '#include "main_api.h"\nvoid f(void) { D_80100002 = 1; }\n')
        self.put("src/main/b.c", '#include "main_api.h"\nvoid g(s32 i) { D_80100002 = 2; D_80100000[i] = 0; '
                 "h(&D_80100000); D_80100000.unk_00 = 1; }\n")
        self.assertEqual(self.apply(), 1)            # the u8[] view of the base needs a hand rewrite
        self.assertIn("D_80100002 = 1;", self.get("src/main/a.c"))
        b = self.get("src/main/b.c")
        self.assertIn("D_80100000.unk_02 = 2;", b)
        self.assertIn("D_80100000[i] = 0;", b)
        self.assertIn("extern s16 D_80100002;", self.get("include/main_api.h"))
        msgs = mg.check(self.root, *mg.load_config(self.root))
        self.assertEqual(len(msgs), 1)
        self.assertIn("src/main/b.c:2: D_80100000 is the State struct", msgs[0])


if __name__ == "__main__":
    unittest.main()
