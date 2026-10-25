#!/usr/bin/env python3
"""Tests for tools/migrate_globals.py on a synthetic tree (no game data).

Run: tools/docker.sh python3 tools/test_migrate_globals.py
"""
import contextlib
import io
import json
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
        self.extra_temps = []
        self.root = self.tmp.name
        for d in ("include/ovl", "src/main", "src/ovl/A", "config"):
            os.makedirs(os.path.join(self.root, d))
        self.put("include/main_api.h", HEADER)
        self.put("config/migrate_globals.txt", CONFIG)

    def tearDown(self):
        self.tmp.cleanup()
        for tmp in self.extra_temps:
            tmp.cleanup()

    def put(self, rel, text):
        with open(os.path.join(self.root, rel), "w") as f:
            f.write(text)

    def get(self, rel):
        with open(os.path.join(self.root, rel)) as f:
            return f.read()

    def tree_bytes(self):
        out = {}
        for directory, _dirs, files in os.walk(self.root):
            for name in files:
                path = os.path.join(directory, name)
                with open(path, "rb") as fh:
                    out[os.path.relpath(path, self.root)] = fh.read()
        return out

    def profile_tree(self):
        api = os.path.join(self.root, "include", "main_api.h")
        if os.path.exists(api):
            os.unlink(api)
        os.makedirs(os.path.join(self.root, "include", "main"), exist_ok=True)
        self.put("include/common.h", "typedef signed char s8;\ntypedef unsigned char u8;\n"
                 "typedef signed short s16;\ntypedef unsigned short u16;\n"
                 "typedef signed int s32;\ntypedef unsigned int u32;\n")
        self.put("include/types.h", "typedef struct State {\n"
                 "    /* 0x00 */ u8 unk_00;\n    /* 0x01 */ s8 unk_01;\n"
                 "    /* 0x02 */ s16 unk_02;\n    /* 0x04 */ s32 unk_04;\n"
                 "    /* 0x08 */ u8 unk_08[24];\n} State; /* size 0x20 */\n")
        self.put("include/main/state.h", '#ifndef MAIN_STATE_H\n#define MAIN_STATE_H\n'
                 '#include "common.h"\n#include "types.h"\n'
                 '#ifndef MAIN_API_OVERRIDE_D_80100000\nextern State D_80100000;\n#endif\n#endif\n')
        self.put("include/main/fields.h", '#ifndef MAIN_FIELDS_H\n#define MAIN_FIELDS_H\n'
                 '#include "common.h"\n#include "types.h"\n'
                 'extern s8 D_80100001;\nextern s16 D_80100002;\n#endif\n')
        self.put("config/header_owners.json", json.dumps({
            "mode": "topic_only",
            "owners": [{"path": "main/state.h", "role": "main"},
                       {"path": "main/fields.h", "role": "main"}],
            "required_types": ["common.h", "types.h"]}))
        self.put("config/migrate_globals.txt", "aggregate D_80100000 State include/types.h\n")

    def test_profile_union_keeps_alias_owner_and_rejects_unregistered_keep(self):
        self.profile_tree()
        views = mg.Views(self.root)
        self.assertEqual(views.main["D_80100002"], "s16")
        self.assertEqual(views.main_owner["D_80100002"], "main/fields.h")
        self.put("config/migrate_globals.txt", "aggregate D_80100000 State include/types.h\n"
                 "keep src/main/80010000.c D_80100002 reason\n"
                 "keep src/main/80010000.c D_80100003 reason\n")
        aggs, keep = mg.load_config(self.root)
        lines = mg.check(self.root, aggs, keep)
        self.assertFalse(any("kept symbol D_80100002 is not declared" in line for line in lines), lines)
        self.assertTrue(any("kept symbol D_80100003 is not declared in canonical owner(s) "
                            "include/main/state.h, include/main/fields.h" in line for line in lines), lines)

    def test_profile_canonical_include_first_does_not_hide_matched_view(self):
        self.profile_tree()
        self.put("include/ovl/A.h", '#define MAIN_API_OVERRIDE_D_80100002 /* matched byte view */\n'
                 '#include "main/fields.h"\nextern u8 D_80100002;\n')
        views = mg.Views(self.root)
        text = '#include "main/fields.h"\n#include "ovl/A.h"\n'
        self.assertEqual(views.view(text, "D_80100002"), "u8")

    def test_profile_duplicate_owners_and_missing_path_fail_before_apply(self):
        self.profile_tree()
        for decl in ("extern s16 D_80100002;\n", "extern s32 D_80100002;\n"):
            self.put("include/main/extra.h", '#include "common.h"\n' + decl)
            self.put("config/header_owners.json", json.dumps({
                "mode": "topic_only",
                "owners": [{"path": "main/state.h", "role": "main"},
                           {"path": "main/fields.h", "role": "main"},
                           {"path": "main/extra.h", "role": "main"}],
                "required_types": ["common.h", "types.h"]}))
            changes, problems = mg.plan(self.root, *mg.load_config(self.root))
            self.assertEqual(changes, {})
            self.assertTrue(any("duplicate canonical owner for D_80100002" in p[3] for p in problems), problems)
            before = self.tree_bytes()
            with contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(mg.main(["--root", self.root, "--apply"]), 1)
            self.assertEqual(before, self.tree_bytes())
        self.put("config/header_owners.json", json.dumps({
            "mode": "topic_only", "owners": [{"path": "main/missing.h", "role": "main"}],
            "required_types": ["common.h", "types.h"]}))
        changes, problems = mg.plan(self.root, *mg.load_config(self.root))
        self.assertEqual(changes, {})
        self.assertTrue(any("required main owner include/main/missing.h is missing" in p[3]
                            for p in problems), problems)
        before = self.tree_bytes()
        with contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(mg.main(["--root", self.root, "--apply"]), 1)
        self.assertEqual(before, self.tree_bytes())

    def test_profile_kept_and_pending_aliases_remain_declared(self):
        self.profile_tree()
        self.put("config/migrate_globals.txt", "aggregate D_80100000 State include/types.h\n"
                 "keep src/main/80010000.c D_80100001 selected view\n")
        self.put("src/main/80010000.c", '#include "main/fields.h"\nextern u32 D_80100002;\n'
                 's32 use(void) { D_80100001 = 1; return D_80100002; }\n')
        changes, problems = mg.plan(self.root, *mg.load_config(self.root))
        self.assertTrue(any(item[2] == "D_80100002" for item in problems), problems)
        fields_path = os.path.join(self.root, "include/main/fields.h")
        fields = changes.get(fields_path, self.get("include/main/fields.h"))
        self.assertIn("extern s8 D_80100001;", fields)
        self.assertIn("extern s16 D_80100002;", fields)
        source_path = os.path.join(self.root, "src/main/80010000.c")
        source = changes.get(source_path, self.get("src/main/80010000.c"))
        self.assertIn("D_80100001 = 1;", source)
        self.assertIn("extern u32 D_80100002;", source)

    def test_profile_base_owner_cleanup_matches_legacy_c_and_overlay_views(self):
        original_root = self.root
        results = []
        for profile_mode in (False, True):
            if profile_mode:
                tmp = tempfile.TemporaryDirectory()
                self.extra_temps.append(tmp)
                self.root = tmp.name
                for directory in ("include/ovl", "src/main", "src/ovl/A", "config"):
                    os.makedirs(os.path.join(self.root, directory))
                self.profile_tree()
                owner_file = "include/main/state.h"
                source_include = "main/state.h"
            else:
                self.root = original_root
                owner_file = "include/main_api.h"
                source_include = "main_api.h"
                api = HEADER.replace("extern State D_80100000;",
                                     "#ifndef MAIN_API_OVERRIDE_D_80100000\n"
                                     "extern State D_80100000;\n#endif")
                self.put(owner_file, api)
            self.put("src/main/a.c", '#define MAIN_API_OVERRIDE_D_80100000 /* C pointer view */\n'
                     '#include "%s"\nextern s32 D_80100000;\nextern s8 D_80100001;\n'
                     'void use(void) { D_80100001 = 1; consume(&D_80100000); }\n' % source_include)
            if profile_mode:
                self.put("include/ovl/A.h", '#define MAIN_API_OVERRIDE_D_80100000 /* overlay pointer view */\n'
                         '#include "main/state.h"\n#include "main/fields.h"\n'
                         'extern s32 D_80100000;\nextern s8 D_80100001;\n')
            else:
                self.put("include/ovl/A.h", '#define MAIN_API_OVERRIDE_D_80100000 /* overlay pointer view */\n'
                         '#include "main_api.h"\nextern s32 D_80100000;\nextern s8 D_80100001;\n')
            self.put("src/ovl/A/a.c", '#include "ovl/A.h"\n'
                     'void use_overlay(void) { D_80100001 = 2; consume(&D_80100000); }\n')
            self.assertEqual(self.apply(), 0)
            owner_text = self.get(owner_file)
            self.assertNotIn("#ifndef MAIN_API_OVERRIDE_D_80100000", owner_text)
            self.assertIn("extern State D_80100000;", owner_text)
            self.assertNotIn("MAIN_API_OVERRIDE_D_80100000", self.get("src/main/a.c"))
            self.assertNotIn("MAIN_API_OVERRIDE_D_80100000", self.get("include/ovl/A.h"))
            main_text = self.get("src/main/a.c").replace('#include "%s"' % source_include,
                                                         '#include "CANONICAL"')
            results.append((main_text, self.get("src/ovl/A/a.c")))
        self.root = original_root
        self.assertEqual(results[0][:2], results[1][:2])

    def test_named_aggregate_base_uses_profile_owner_and_matches_legacy_cleanup(self):
        original_root = self.root
        results = []
        original_addr = mg.sym_addr

        def address(name):
            return 0x80100000 if name == "game_state" else original_addr(name)

        for profile_mode in (False, True):
            if profile_mode:
                tmp = tempfile.TemporaryDirectory()
                self.extra_temps.append(tmp)
                self.root = tmp.name
                for directory in ("include/ovl", "src/main", "src/ovl/A", "config"):
                    os.makedirs(os.path.join(self.root, directory))
                self.profile_tree()
                owner_file, source_include = "include/main/state.h", "main/state.h"
                owner_text = self.get(owner_file).replace("D_80100000", "game_state")
                self.put(owner_file, owner_text)
            else:
                self.root = original_root
                owner_file, source_include = "include/main_api.h", "main_api.h"
                owner_text = HEADER.replace("extern State D_80100000;",
                    "#ifndef MAIN_API_OVERRIDE_game_state\nextern State game_state;\n#endif")
                self.put(owner_file, owner_text)
            self.put("config/migrate_globals.txt",
                     "aggregate game_state State %s\n" % owner_file)
            self.put("src/main/a.c", '#define MAIN_API_OVERRIDE_game_state /* C registration */\n'
                     '#include "%s"\nvoid use(void) { }\n' % source_include)
            if profile_mode:
                self.put("include/ovl/A.h", '#define MAIN_API_OVERRIDE_game_state /* overlay registration */\n'
                         '#include "main/state.h"\n')
            else:
                self.put("include/ovl/A.h", '#define MAIN_API_OVERRIDE_game_state /* overlay registration */\n'
                         '#include "main_api.h"\n')
            self.put("src/ovl/A/a.c", '#include "ovl/A.h"\nvoid use_overlay(void) { }\n')
            views = mg.Views(self.root)
            self.assertEqual(views.base_owner["game_state"],
                             "main/state.h" if profile_mode else "main_api.h")
            with mock.patch.object(mg, "sym_addr", side_effect=address):
                self.assertEqual(self.apply(), 0)
            owner_text = self.get(owner_file)
            self.assertNotIn("#ifndef MAIN_API_OVERRIDE_game_state", owner_text)
            self.assertIn("extern State game_state;", owner_text)
            self.assertNotIn("MAIN_API_OVERRIDE_game_state", self.get("src/main/a.c"))
            self.assertNotIn("MAIN_API_OVERRIDE_game_state", self.get("include/ovl/A.h"))
            results.append((self.get("src/main/a.c").replace('#include "%s"' % source_include,
                                                               '#include "CANONICAL"'),
                            self.get("src/ovl/A/a.c")))
        self.root = original_root
        self.assertEqual(results[0], results[1])

    def apply(self):
        return mg.main(["--root", self.root, "--apply"])

    def test_layout(self):
        aggs, _ = mg.load_config(self.root)
        t = aggs[0].type
        self.assertEqual(t.size, 0x20)
        paths = [p for p, _t in mg.subobjects(t, 0x0F)]
        self.assertEqual(paths[-1], ".unk_08[1].c")
        self.assertEqual(mg.parse_types("typedef struct F { u32 x : 3; u32 y : 30; } F;")["F"].size, 8)

    def test_quoted_type_dependency_in_source_order(self):
        self.put("include/record.h", "typedef struct Rec { /* 0x00 */ u16 x; } Rec;\n")
        self.put("include/main_api.h", '#include "record.h"\ntypedef struct State { Rec value[3]; } State;\n')
        aggregate = mg.load_config(self.root)[0][0]
        self.assertEqual((aggregate.type.size, aggregate.type.align), (6, 2))
        self.assertEqual(aggregate.type.fields[0][2].count, 3)

    def test_local_and_project_root_includes(self):
        os.makedirs(os.path.join(self.root, "include/game"))
        self.put("include/game/record.h", "typedef union W { u32 w; u8 b[4]; } W;\n")
        self.put("include/game/state.h", '#include "record.h"\ntypedef struct State { W value; } State;\n')
        self.put("include/main_api.h", '#include "game/state.h"\n')
        self.assertEqual(mg.load_config(self.root)[0][0].type.size, 4)

    def test_include_expands_at_its_parent_source_position(self):
        self.put("include/second.h", "typedef struct Second { First x; } Second;\n")
        self.put("include/main_api.h", 'typedef struct First { u32 x; } First;\n#include "second.h"\ntypedef struct State { Second x; } State;\n')
        text = mg.read_type_context(self.root, "include/main_api.h")
        self.assertLess(text.index("struct First"), text.index("struct Second"))
        self.assertLess(text.index("struct Second"), text.index("struct State"))
        self.assertEqual(mg.load_config(self.root)[0][0].type.size, 4)

    def test_guarded_repeat_and_cycle(self):
        self.put("include/a.h", '#ifndef A_H\n#define A_H\n#include "b.h"\ntypedef struct W { u32 x; } W;\n#endif\n')
        self.put("include/b.h", '#ifndef B_H\n#define B_H\n#include "a.h"\n#endif\n')
        self.put("include/main_api.h", '#include "a.h"\n#include "a.h"\ntypedef struct State { W x; } State;\n')
        self.assertEqual(mg.load_config(self.root)[0][0].type.size, 4)

    def test_unguarded_repeat_cycle_terminates_once(self):
        self.put("include/a.h", '#include "b.h"\ntypedef struct W { u32 x; } W;\n')
        self.put("include/b.h", '#include "a.h"\n')
        self.put("include/main_api.h", '#include "a.h"\n#include "b.h"\ntypedef struct State { W x; } State;\n')
        self.assertEqual(mg.load_config(self.root)[0][0].type.size, 4)

    def test_missing_quoted_dependency(self):
        self.put("include/main_api.h", '#include "missing.h"\n' + HEADER)
        with self.assertRaisesRegex(SystemExit, r"main_api.h:1: missing quoted include missing.h"):
            mg.load_config(self.root)

    def test_commented_include_ignored_and_offset_retained(self):
        self.put("include/main_api.h", '/*\n#include "missing.h"\n*/\n// #include "also_missing.h"\n' + HEADER)
        aggregate = mg.load_config(self.root)[0][0]
        self.assertEqual(aggregate.type.size, 0x20)
        self.assertIn("/* 0x18 */", mg.read_type_context(self.root, "include/main_api.h"))

    def test_duplicate_alias_has_both_origins(self):
        self.put("include/a.h", "typedef struct W { u32 x; } W;\n")
        self.put("include/b.h", "typedef union W { u32 y; } W;\n")
        self.put("include/main_api.h", '#include "a.h"\n#include "b.h"\n' + HEADER)
        with self.assertRaisesRegex(SystemExit, r"include/b.h:1: duplicate record typedef W .*include/a.h:1"):
            mg.load_config(self.root)

    def test_common_guard_context_skips_second_definition(self):
        self.put("include/a.h", '#ifndef RECORD_H\n#define RECORD_H\ntypedef struct W { u32 x; } W;\n#endif\n')
        self.put("include/b.h", '#ifndef RECORD_H\n#define RECORD_H\ntypedef struct W { u32 x[2]; } W;\n#endif\n')
        self.put("include/main_api.h", '#include "a.h"\n#include "b.h"\ntypedef struct State { W x; } State;\n')
        self.assertEqual(mg.load_config(self.root)[0][0].type.size, 4)

    def test_unsupported_condition_with_include_fails(self):
        self.put("include/main_api.h", '#if MODE == 2\n#include "missing.h"\n#endif\n' + HEADER)
        with self.assertRaisesRegex(SystemExit, "unsupported condition around include"):
            mg.load_config(self.root)

    def test_unsupported_condition_with_type_fails(self):
        self.put("include/main_api.h", '#if 0\ntypedef\nstruct Extra { u32 x; } Extra;\n#endif\n' + HEADER)
        with self.assertRaisesRegex(SystemExit, "unsupported condition around record definition"):
            mg.load_config(self.root)

    def test_predefined_nested_guard_does_not_add_inactive_record(self):
        self.put("include/main_api.h", '#define MODE\n#ifndef MODE\n#define MODE\ntypedef struct W { u32 x[99]; } W;\n#endif\n' + HEADER)
        self.assertEqual(mg.load_config(self.root)[0][0].type.size, 0x20)

    def test_nested_guard_under_ambiguous_condition_rejected(self):
        prefix = '#if FEATURE\n#ifndef MODE\n#define MODE\ntypedef struct W { u32 x; } W;\n#endif\n#endif\n'
        self.put("include/main_api.h", prefix + HEADER)
        with self.assertRaisesRegex(SystemExit, "unsupported condition around record definition"):
            mg.load_config(self.root)

    def test_prior_conditional_define_makes_later_guard_ambiguous(self):
        prefix = '#if FEATURE\n#define MODE\n#endif\n#ifndef MODE\n#define MODE\ntypedef struct W { u32 x; } W;\n#endif\n'
        self.put("include/main_api.h", prefix + HEADER)
        with self.assertRaisesRegex(SystemExit, "unsupported condition around record definition"):
            mg.load_config(self.root)

    def test_unconditional_undef_restores_known_guard(self):
        prefix = '#define MODE\n#undef MODE\n#ifndef MODE\n#define MODE\ntypedef struct Extra { u32 x; } Extra;\n#endif\n'
        self.put("include/main_api.h", prefix + HEADER)
        aggregate = mg.load_config(self.root)[0][0]
        self.assertEqual(aggregate.types["Extra"].size, 4)

    def test_conditional_fields_do_not_become_unconditional_layout(self):
        self.put("include/main_api.h", 'typedef struct State {\n#if MODE == 2\nu32 optional;\n#endif\nu32 x;\n} State;\n')
        with self.assertRaisesRegex(SystemExit, r"main_api.h:2: unsupported directive inside record State"):
            mg.load_config(self.root)

    def test_nonconditional_directive_inside_record_rejected(self):
        self.put("include/main_api.h", 'typedef struct State {\n#define FIELD_SIZE 4\nu32 x;\n} State;\n')
        with self.assertRaisesRegex(SystemExit, "unsupported directive inside record State"):
            mg.load_config(self.root)

    def test_harmless_conditions_and_macro_body_ignored(self):
        prefix = '#if PLATFORM == 2\n#define HIDDEN ' + chr(92) + '\n'
        self.put("include/main_api.h", prefix + 'typedef struct W { u32 x[9]; } W;\n#endif\n' + HEADER)
        self.assertEqual(mg.load_config(self.root)[0][0].type.size, 0x20)

    def test_dependency_offset_error_and_unneeded_error_not_hidden(self):
        self.put("include/bad.h", "typedef struct Bad { /* 0x02 */ u8 x; } Bad;\n")
        self.put("include/main_api.h", '#include "bad.h"\n' + HEADER)
        self.assertEqual(mg.load_config(self.root)[0][0].type.size, 0x20)
        self.put("include/main_api.h", '#include "bad.h"\ntypedef struct State { Bad x; } State;\n')
        with self.assertRaises(mg.LayoutError):
            mg.load_config(self.root)

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
        with mock.patch.dict(sys.modules, {"keepproof": fake}), contextlib.redirect_stdout(io.StringIO()):
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
