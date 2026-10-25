#!/usr/bin/env python3
"""Tests for tools/sync_protos.py and the main_api.h rules of tools/check_headers.py (synthetic
headers and sources, no game data).

Run: tools/docker.sh python3 tools/test_sync_protos.py
"""
import json
import hashlib
import tempfile
import unittest
from pathlib import Path

import check_headers
import sync_protos as sp
import symtab

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

    def event_private_fixture(self):
        (self.root / "config" / "overlays.txt").write_text("EVENT 0x800F6000 0x100\nOPTION 0x80130000 0x100\n")
        names = (("D_801206E8", "s32"), ("D_8012076C", "s32"),
                 ("D_80120798", "s32"), ("D_80121884", "s32"),
                 ("D_801220EC", "s16"), ("D_80122740", "s32"),
                 ("D_8012277C", "u8"))
        owners = [{"path": p, "role": "main"} for p in (
            "main_api.h", "main/schedule.h", "main/day_event.h", "main/school_day.h",
            "main/game_mode.h", "main/pad.h", "main/holiday.h", "main/ovl.h")]
        (self.root / "config" / "header_owners.json").write_text(json.dumps({
            "mode": "topic_only", "owners": owners, "required_types": ["common.h"]}))
        (self.root / "config" / "SLPM_86.053.yaml").write_text(
            "symbol_addrs_path:\n  - config/symbol_addrs.txt\n")
        (self.root / "config" / "overlays").mkdir(exist_ok=True)
        (self.root / "config" / "overlays" / "EVENT.yaml").write_text(
            "symbol_addrs_path:\n  - config/symbols_EVENT.txt\nsegments: []\n")
        (self.root / "config" / "overlays" / "OPTION.yaml").write_text(
            "symbol_addrs_path:\n  - config/symbols_OPTION.txt\nsegments: []\n")
        self.write("common.h", "#ifndef COMMON_H\n#define COMMON_H\ntypedef signed int s32;\ntypedef signed short s16;\ntypedef unsigned char u8;\n#endif\n")
        for p in owners[1:]:
            self.write(p["path"], '#include "common.h"\n')
        api = ['#ifndef MAIN_API_H', '#define MAIN_API_H', '#include "common.h"']
        for name, typ in names:
            api += ['#ifndef SPOT_PRIVATE_DATA_VIEWS', 'extern %s %s;' % (typ, name), '#endif']
        api += ['#endif', '']
        self.write("main_api.h", "\n".join(api))
        self.write("game.h", '#include "main_api.h"\n')
        decls = "\n".join("extern %s %s;" % (typ, name) for name, typ in names)
        self.write("ovl/EVENT.h", '#ifndef OVL_EVENT_H\n#define OVL_EVENT_H\n#include "common.h"\n'
                   '#define SPOT_PRIVATE_DATA_VIEWS\n#include "game.h"\n'
                   '#ifdef SPOT_PRIVATE_DATA_VIEWS\n'
                   '/* BEGIN T-27061 EVENT private scalar views */\n' + decls + '\n'
                   '/* END T-27061 EVENT private scalar views */\n#endif\n'
                   '#undef SPOT_PRIVATE_DATA_VIEWS\n#endif\n')
        self.src("ovl/EVENT/probe.c", '#include "ovl/EVENT.h"\ns32 probe(void) { return D_801206E8; }\n')
        self.src("ovl/OPTION/probe.c", '#include "game.h"\ns32 probe_option(void) { return D_801206E8; }\n')


class EventPrivateViewTest(Repo):
    def tree_bytes(self):
        out = {}
        for label, root in (("include", self.inc), ("src", self.root / "src")):
            for path in root.rglob("*"):
                if path.is_file():
                    out["%s/%s" % (label, path.relative_to(root).as_posix())] = path.read_bytes()
        return out

    def test_exact_contract_is_unit_private_and_main_option_stays_main(self):
        self.event_private_fixture()
        self.assertTrue(sp.Model(str(self.inc)).event_private_active)
        model = sp.Model(str(self.inc), str(self.root / "src"))
        self.assertEqual(sp.event_private_contract_problems(model, str(self.inc)), [])
        self.assertEqual(sp.check_api(str(self.inc), str(self.root / "src")), [])
        self.assertEqual(sp.candidates(model), {})
        self.assertFalse(sp.is_main_symbol(str(self.root), "D_801206E8", "EVENT"))
        self.assertTrue(sp.is_main_symbol(str(self.root), "D_801206E8", "OPTION"))

    def test_future_event_alias_resolves_by_address(self):
        self.event_private_fixture()
        p = self.inc / "ovl/EVENT.h"
        p.write_text(p.read_text().replace("D_801206E8", "spot_yuko_still_pointers_3"))
        event_src = self.root / "src/ovl/EVENT/probe.c"
        event_src.write_text(event_src.read_text().replace("D_801206E8", "spot_yuko_still_pointers_3"))
        (self.root / "config" / "symbols_EVENT.txt").write_text(
            "spot_yuko_still_pointers_3 = 0x801206E8; // type:data\n")
        model = sp.Model(str(self.inc), str(self.root / "src"))
        self.assertEqual(sp.event_private_contract_problems(model, str(self.inc)), [])
        self.assertEqual(symtab.SymbolTable(str(self.root)).resolve("EVENT", "spot_yuko_still_pointers_3"),
                         ("EVENT", 0x801206E8))
        self.assertFalse(sp.is_main_symbol(str(self.root), "spot_yuko_still_pointers_3", "EVENT"))
        self.assertTrue(sp.is_main_symbol(str(self.root), "D_801206E8", "OPTION"))

    def test_seventh_event_spelling_survives_main_public_rename_and_fix(self):
        self.event_private_fixture()
        api = self.inc / "main_api.h"
        api.write_text(api.read_text().replace("extern s32 D_80122740;", "extern s32 view_reference;"))
        (self.root / "config" / "symbol_addrs.txt").write_text(
            "view_reference = 0x80122740; // type:data\n")
        (self.root / "config" / "symbols_EVENT.txt").write_text(
            "D_80122740 = 0x80122740; // type:data\n")
        self.src("ovl/EVENT/reference.c", '#include "ovl/EVENT.h"\n'
                 's32 event_reference(void) { return D_80122740; }\n')
        self.src("ovl/OPTION/reference.c", '#include "game.h"\n'
                 's32 option_reference(void) { return view_reference; }\n')
        model = sp.Model(str(self.inc), str(self.root / "src"))
        self.assertEqual(model.event_private_addr("D_80122740", "ovl/EVENT.h", "s32"), 0x80122740)
        self.assertIn("view_reference", model.api)
        self.assertNotIn("D_80122740", model.api)
        self.assertEqual(sp.check_api(str(self.inc), str(self.root / "src")), [])
        self.assertFalse(sp.is_main_symbol(str(self.root), "D_80122740", "EVENT"))
        self.assertTrue(sp.is_main_symbol(str(self.root), "view_reference", "OPTION"))
        before_bytes = self.tree_bytes()
        before_views = sp.snapshot(model)
        self.assertEqual(sp.run_write(str(self.inc), str(self.root / "src"), fix=True,
                                      log=lambda _: None), 0)
        self.assertEqual(sp.view_changes(before_views, sp.snapshot(
            sp.Model(str(self.inc), str(self.root / "src")))), [])
        self.assertEqual(before_bytes, self.tree_bytes())

    def test_seventh_binding_wrong_type_guard_and_eighth_address_fail_closed(self):
        self.event_private_fixture()
        event = self.inc / "ovl/EVENT.h"
        original = event.read_text()
        event.write_text(original.replace("extern s32 D_80122740;\n", ""))
        out = sp.event_private_contract_problems(sp.Model(str(self.inc)), str(self.inc))
        self.assertTrue(any("missing private view at 0x80122740" in x for x in out), out)
        event.write_text(original.replace("extern s32 D_80122740;", "extern u16 D_80122740;"))
        out = sp.event_private_contract_problems(sp.Model(str(self.inc)), str(self.inc))
        self.assertTrue(any("0x80122740 requires s32" in x for x in out), out)
        event.write_text(original.replace("extern s32 D_80122740;", "extern s32 D_80122740;\nextern s32 D_80123000;"))
        out = sp.event_private_contract_problems(sp.Model(str(self.inc)), str(self.inc))
        self.assertTrue(any("unexpected private declaration D_80123000" in x for x in out), out)
        api = self.inc / "main_api.h"
        api.write_text(api.read_text().replace(
            "#ifndef SPOT_PRIVATE_DATA_VIEWS\nextern s32 D_80122740;\n#endif",
            "extern s32 D_80122740;"))
        event.write_text(original)
        out = sp.event_private_contract_problems(sp.Model(str(self.inc)), str(self.inc))
        self.assertTrue(any("canonical view at 0x80122740 is not hidden" in x for x in out), out)

    def test_wrong_private_type_and_extra_address_fail_closed(self):
        self.event_private_fixture()
        p = self.inc / "ovl/EVENT.h"
        p.write_text(p.read_text().replace("extern s32 D_801206E8;", "extern u16 D_801206E8;")
                     .replace("extern u8 D_8012277C;", "extern u8 D_8012277C;\nextern s32 D_80123000;"))
        out = sp.event_private_contract_problems(sp.Model(str(self.inc)), str(self.inc))
        self.assertTrue(any("requires s32" in x for x in out), out)
        self.assertTrue(any("unexpected private declaration" in x for x in out), out)

    def test_missing_shared_guard_and_bad_selector_order_fail_closed(self):
        self.event_private_fixture()
        api = self.inc / "main_api.h"
        api.write_text(api.read_text().replace("#ifndef SPOT_PRIVATE_DATA_VIEWS\nextern s32 D_801206E8;\n#endif",
                                              "extern s32 D_801206E8;"))
        event = self.inc / "ovl/EVENT.h"
        event.write_text(event.read_text().replace('#define SPOT_PRIVATE_DATA_VIEWS\n#include "game.h"',
                                                  '#include "game.h"\n#define SPOT_PRIVATE_DATA_VIEWS'))
        out = sp.event_private_contract_problems(sp.Model(str(self.inc)), str(self.inc))
        self.assertTrue(any("not hidden" in x for x in out), out)
        self.assertTrue(any("selector must bracket" in x for x in out), out)

    def test_selector_leakage_and_missing_private_guard_fail_closed(self):
        self.event_private_fixture()
        event = self.inc / "ovl/EVENT.h"
        text = event.read_text().replace("#ifdef SPOT_PRIVATE_DATA_VIEWS\n/* BEGIN", "/* BEGIN")
        text = text.replace("#undef SPOT_PRIVATE_DATA_VIEWS\n", "")
        event.write_text(text)
        out = sp.event_private_contract_problems(sp.Model(str(self.inc)), str(self.inc))
        self.assertTrue(any("leaks beyond" in x for x in out), out)
        self.assertTrue(any("selector must bracket" in x for x in out), out)

    def test_event_translation_unit_that_includes_game_first_fails(self):
        self.event_private_fixture()
        source = self.root / "src/ovl/EVENT/probe.c"
        source.write_text('#include "game.h"\n#include "ovl/EVENT.h"\ns32 probe(void) { return D_801206E8; }\n')
        out = sp.event_private_contract_problems(
            sp.Model(str(self.inc), str(self.root / "src")), str(self.inc), str(self.root / "src"))
        self.assertTrue(any("must see one private" in x for x in out), out)

    def test_non_event_source_cannot_include_event_private_header(self):
        self.event_private_fixture()
        source = self.root / "src/ovl/OPTION/probe.c"
        source.write_text('#include "ovl/EVENT.h"\ns32 probe_option(void) { return 0; }\n')
        out = sp.event_private_contract_problems(
            sp.Model(str(self.inc), str(self.root / "src")), str(self.inc), str(self.root / "src"))
        self.assertTrue(any("non-EVENT source" in x for x in out), out)

    def test_non_event_source_cannot_define_selector_around_canonical_include(self):
        self.event_private_fixture()
        source = self.root / "src/ovl/OPTION/probe.c"
        for define in ("#define SPOT_PRIVATE_DATA_VIEWS", "#define SPOT_PRIVATE_DATA_VIEWS(x)"):
            with self.subTest(define=define):
                source.write_text(define + '\n#include "game.h"\n#undef SPOT_PRIVATE_DATA_VIEWS\n')
                out = sp.event_private_contract_problems(
                    sp.Model(str(self.inc), str(self.root / "src")), str(self.inc), str(self.root / "src"))
                self.assertTrue(any("directive is forbidden in non-EVENT source" in x for x in out), out)

    def test_event_private_alias_in_another_header_is_rejected(self):
        self.event_private_fixture()
        self.write("ovl/EVENT_data.h", "extern s32 D_801206E8;\n")
        source = self.root / "src/ovl/EVENT/probe.c"
        source.write_text('#include "ovl/EVENT_data.h"\n#include "ovl/EVENT.h"\n')
        out = sp.event_private_contract_problems(
            sp.Model(str(self.inc), str(self.root / "src")), str(self.inc), str(self.root / "src"))
        self.assertTrue(any("outside the validated EVENT private block" in x for x in out), out)

    def test_future_event_alias_without_header_is_not_missed(self):
        self.event_private_fixture()
        (self.root / "config" / "symbols_EVENT.txt").write_text(
            "spot_yuko_still_pointers_3 = 0x801206E8; // type:data\n")
        source = self.root / "src/ovl/EVENT/probe.c"
        source.write_text("s32 probe(void) { return spot_yuko_still_pointers_3; }\n")
        out = sp.event_private_contract_problems(
            sp.Model(str(self.inc), str(self.root / "src")), str(self.inc), str(self.root / "src"))
        self.assertTrue(any("uses a private address but does not include" in x for x in out), out)

    def test_event_source_own_declaration_of_future_alias_is_rejected(self):
        self.event_private_fixture()
        (self.root / "config" / "symbols_EVENT.txt").write_text(
            "spot_yuko_still_pointers_3 = 0x801206E8; // type:data\n")
        source = self.root / "src/ovl/EVENT/probe.c"
        source.write_text('#include "ovl/EVENT.h"\nextern s32 spot_yuko_still_pointers_3;\n')
        out = sp.event_private_contract_problems(
            sp.Model(str(self.inc), str(self.root / "src")), str(self.inc), str(self.root / "src"))
        self.assertTrue(any("source" in x and "declares private alias" in x for x in out), out)

    def test_partially_renamed_private_spelling_must_match_event_header(self):
        self.event_private_fixture()
        (self.root / "config" / "symbols_EVENT.txt").write_text(
            "spot_yuko_still_pointers_3 = 0x801206E8; // type:data\n")
        source = self.root / "src/ovl/EVENT/probe.c"
        source.write_text('#include "ovl/EVENT.h"\n'
                          's32 probe(void) { return spot_yuko_still_pointers_3; }\n')
        out = sp.event_private_contract_problems(
            sp.Model(str(self.inc), str(self.root / "src")), str(self.inc), str(self.root / "src"))
        self.assertTrue(any("does not see that exact s32 spelling once" in x for x in out), out)

    def test_contract_survives_topic_owner_roots_without_either_umbrella(self):
        self.event_private_fixture()
        names = (("D_801206E8", "s32"), ("D_8012076C", "s32"),
                 ("D_80120798", "s32"), ("D_80121884", "s32"),
                 ("D_801220EC", "s16"), ("D_80122740", "s32"),
                 ("D_8012277C", "u8"))
        owner = self.inc / "main/schedule.h"
        owner.write_text('#include "common.h"\n' + "\n".join(
            '#ifndef SPOT_PRIVATE_DATA_VIEWS\nextern %s %s;\n#endif' % (typ, name)
            for name, typ in names) + "\n")
        (self.inc / "main_api.h").unlink()
        (self.inc / "game.h").unlink()
        event = self.inc / "ovl/EVENT.h"
        event.write_text(event.read_text().replace('#include "game.h"', '#include "main/schedule.h"'))
        option = self.root / "src/ovl/OPTION/probe.c"
        option.write_text('#include "main/schedule.h"\ns32 probe_option(void) { return D_801206E8; }\n')
        profile = json.loads((self.root / "config/header_owners.json").read_text())
        profile["owners"] = [e for e in profile["owners"] if e["path"] != "main_api.h"]
        (self.root / "config/header_owners.json").write_text(json.dumps(profile))
        out = sp.event_private_contract_problems(
            sp.Model(str(self.inc), str(self.root / "src")), str(self.inc), str(self.root / "src"))
        self.assertEqual(out, [])
        self.assertEqual(sp.check_api(str(self.inc), str(self.root / "src")), [])

    def test_write_fix_preserve_views_and_bytes_for_both_namespaces(self):
        self.event_private_fixture()
        for renamed in (False, True):
            with self.subTest(renamed=renamed):
                if renamed:
                    header = self.inc / "ovl/EVENT.h"
                    header.write_text(header.read_text().replace("D_801206E8", "spot_yuko_still_pointers_3"))
                    source = self.root / "src/ovl/EVENT/probe.c"
                    source.write_text(source.read_text().replace("D_801206E8", "spot_yuko_still_pointers_3"))
                    (self.root / "config" / "symbols_EVENT.txt").write_text(
                        "spot_yuko_still_pointers_3 = 0x801206E8; // type:data\n")
                before_bytes = self.tree_bytes()
                model = sp.Model(str(self.inc), str(self.root / "src"))
                before_views = sp.snapshot(model)
                self.assertEqual(sp.run_write(str(self.inc), str(self.root / "src"), fix=True,
                                              log=lambda _: None), 0)
                self.assertEqual(sp.view_changes(before_views, sp.snapshot(
                    sp.Model(str(self.inc), str(self.root / "src")))), [])
                self.assertEqual(before_bytes, self.tree_bytes())

    def test_invalid_contract_blocks_write_and_fix_without_mutation(self):
        self.event_private_fixture()
        event = self.inc / "ovl/EVENT.h"
        event.write_text(event.read_text().replace("extern s32 D_80122740;", "extern s16 D_80122740;"))
        before = self.tree_bytes()
        for fix in (False, True):
            with self.subTest(fix=fix):
                self.assertNotEqual(sp.run_write(str(self.inc), str(self.root / "src"), fix=fix,
                                                 log=lambda _: None), 0)
                self.assertEqual(before, self.tree_bytes())

class LegacyImportedMainViewsTest(Repo):
    def setUp(self):
        super().setUp()
        self._original_roster_hash = sp.LEGACY_IMPORT_ROSTER_SHA256

    def tearDown(self):
        sp.LEGACY_IMPORT_ROSTER_SHA256 = self._original_roster_hash
        super().tearDown()

    def fixture(self):
        self.event_private_fixture()
        (self.root / "config/overlays.txt").write_text(
            "EVENT 0x800F6000 0x100\nGYOZI 0x800F8000 0x100\nOPTION 0x80130000 0x100\n")
        (self.root / "src/ovl/GYOZI").mkdir(parents=True, exist_ok=True)
        api = self.inc / "main_api.h"
        api_text = api.read_text()
        api_end = api_text.rfind("#endif")
        api.write_text(api_text[:api_end] + "void real_main_alias(void);\n" + api_text[api_end:])
        (self.root / "config/symbol_addrs.txt").write_text("real_main_alias = 0x80050000; // type:func\n")
        self.write("ovl/GYOZI.h", '#ifndef OVL_GYOZI_H\n#define OVL_GYOZI_H\n#include "common.h"\n#include "game.h"\n'
                   '#include "ovl/GYOZI_main_imports.h"\n#endif\n')
        event_root = self.inc / "ovl/EVENT.h"
        event_root.write_text(event_root.read_text().replace(
            "#undef SPOT_PRIVATE_DATA_VIEWS\n#endif\n",
            '#undef SPOT_PRIVATE_DATA_VIEWS\n#include "ovl/EVENT_main_imports.h"\n#endif\n'))
        units = {"EVENT": [], "GYOZI": []}
        for unit, count in (("EVENT", 51), ("GYOZI", 35)):
            for i in range(count):
                address = 0x80050000 + i * 4 if unit == "EVENT" else (0x80050000 if i == 0 else 0x80070000 + i * 4)
                name = "func_%08X" % address
                if unit == "GYOZI" and i == 0:
                    typ, declaration = "s32F()", "s32 %s();" % name
                elif i == 1:
                    typ, declaration = "voidF(void)", "void %s(void);" % name
                else:
                    typ, declaration = "voidF()", "void %s();" % name
                units[unit].append({
                    "unit": unit, "address": "0x%08X" % address,
                    "original_name": name, "original_type": typ,
                    "type_identity_sha256": hashlib.sha256(typ.encode()).hexdigest(),
                    "current_name": name, "current_type": typ,
                    "declaration_block_sha256": hashlib.sha256((declaration + "\n").encode()).hexdigest(),
                    "declaration": declaration,
                })
        stable = sorted((row["unit"], row["address"], row["type_identity_sha256"])
                        for rows in units.values() for row in rows)
        fingerprint = hashlib.sha256(json.dumps(stable, separators=(",", ":"), ensure_ascii=True).encode()).hexdigest()
        sp.LEGACY_IMPORT_ROSTER_SHA256 = fingerprint
        for unit, rows in units.items():
            provider = "ovl/%s_main_imports.h" % unit
            guard = "OVL_%s_MAIN_IMPORTS_H" % unit
            text = "#ifndef %s\n#define %s\n#include \"common.h\"\n\n%s\n#endif\n" % (
                guard, guard, "\n".join(row["declaration"] for row in rows))
            self.write(provider, text)
        profile_path = self.root / "config/header_owners.json"
        profile = json.loads(profile_path.read_text())
        profile["legacy_main_imports"] = {
            "version": "legacy-main-imports-v1", "roster_sha256": fingerprint,
            "expected_counts": {"EVENT": 51, "GYOZI": 35},
            "providers": [
                {"unit": unit, "provider_path": "ovl/%s_main_imports.h" % unit,
                 "root_header_path": "ovl/%s.h" % unit, "bindings": rows}
                for unit, rows in units.items()
            ],
        }
        profile_path.write_text(json.dumps(profile))
        self.src("ovl/EVENT/import_probe.c", '#include "ovl/EVENT.h"\nvoid event_probe(void) { func_80050000(); }\n')
        self.src("ovl/GYOZI/import_probe.c", '#include "ovl/GYOZI.h"\nint gyozi_probe(void) { func_80050000(); return 0; }\n')
        self.src("ovl/OPTION/import_probe.c", '#include "game.h"\nvoid option_probe(void) { real_main_alias(); }\n')
        return units

    def tree_bytes(self):
        out = {}
        for label, root in (("include", self.inc), ("src", self.root / "src"), ("config", self.root / "config")):
            for path in root.rglob("*"):
                if path.is_file():
                    out["%s/%s" % (label, path.relative_to(root).as_posix())] = path.read_bytes()
        return out

    def test_valid_finite_namespaces_keep_global_lookup_and_main_alias(self):
        self.fixture()
        model = sp.Model(str(self.inc), str(self.root / "src"))
        self.assertEqual(model.profile_problems, [])
        self.assertEqual(sp.legacy_import_contract_problems(model, str(self.inc), str(self.root / "src")), [])
        self.assertEqual(sp.check_api(str(self.inc), str(self.root / "src")), [])
        self.assertEqual(len(model.legacy_imports_by_unit_name), 86)
        self.assertEqual(model.owner("ovl/EVENT_main_imports.h"), "EVENT")
        self.assertFalse(model.is_main("func_80050000", "ovl/EVENT_main_imports.h", "voidF()"))
        self.assertFalse(sp.is_main_symbol(str(self.root), "func_80050000", "EVENT"))
        self.assertFalse(sp.is_main_symbol(str(self.root), "func_80050000", "GYOZI"))
        self.assertTrue(sp.is_main_symbol(str(self.root), "func_80050000", "OPTION"))
        self.assertTrue(sp.is_main_symbol(str(self.root), "real_main_alias", "OPTION"))
        self.assertEqual(model.addr("func_80050000"), 0x80050000)
        self.assertEqual(symtab.SymbolTable(str(self.root)).resolve("EVENT", "func_80050000"),
                         ("main", 0x80050000))
        self.assertNotIn("func_80050000", sp.candidates(model))
        self.assertNotIn("func_80050000", sp.unit_types(model, "OPTION"))
        self.assertIn("func_80050000", sp.unit_types(model, "EVENT"))

    def test_valid_fix_is_byte_inert_and_local_override_is_checked(self):
        self.fixture()
        path = self.inc / "ovl/EVENT.h"
        text = path.read_text().replace(
            '#include "ovl/EVENT_main_imports.h"',
            '#define MAIN_API_OVERRIDE_func_80050000 /* EVENT call-view uses signed result */\n'
            's32 func_80050000(void);\n#include "ovl/EVENT_main_imports.h"')
        path.write_text(text)
        provider = self.inc / "ovl/EVENT_main_imports.h"
        provider.write_text(provider.read_text().replace(
            "void func_80050000();",
            "#ifndef MAIN_API_OVERRIDE_func_80050000\nvoid func_80050000();\n#endif", 1))
        self.assertEqual(sp.check_api(str(self.inc), str(self.root / "src")), [])
        before_bytes = self.tree_bytes()
        before_views = sp.snapshot(sp.Model(str(self.inc), str(self.root / "src")))
        self.assertEqual(sp.run_write(str(self.inc), str(self.root / "src"), fix=True, log=lambda _: None), 0)
        self.assertEqual(self.tree_bytes(), before_bytes)
        self.assertEqual(sp.view_changes(before_views, sp.snapshot(sp.Model(str(self.inc), str(self.root / "src")))), [])

    def test_missing_roster_wrong_type_and_wrong_unit_include_fail_before_write(self):
        self.fixture()
        profile_path = self.root / "config/header_owners.json"
        profile = json.loads(profile_path.read_text())
        profile["legacy_main_imports"]["providers"][0]["bindings"].pop()
        profile_path.write_text(json.dumps(profile))
        before = self.tree_bytes()
        self.assertTrue(any("fingerprint" in p or "counts" in p for p in sp.check_api(str(self.inc), str(self.root / "src"))))
        self.assertNotEqual(sp.run_write(str(self.inc), str(self.root / "src"), fix=True, log=lambda _: None), 0)
        self.assertEqual(self.tree_bytes(), before)

        self.fixture()
        gyozi = self.inc / "ovl/GYOZI.h"
        gyozi.write_text(gyozi.read_text().replace('#include "ovl/GYOZI_main_imports.h"',
                                                   '#include "ovl/GYOZI_main_imports.h"\n#include "ovl/EVENT_main_imports.h"'))
        self.assertTrue(any("leaks through include/ovl/GYOZI.h" in p for p in sp.check_api(str(self.inc), str(self.root / "src"))))

        self.fixture()
        provider = self.inc / "ovl/EVENT_main_imports.h"
        provider.write_text(provider.read_text().replace("void func_80050000();", "s32 func_80050000();", 1))
        self.assertTrue(any("must declare exactly one" in p for p in sp.check_api(str(self.inc), str(self.root / "src"))))

    def test_recomputed_unreviewed_roster_and_malformed_binding_fail_closed(self):
        self.fixture()
        profile_path = self.root / "config/header_owners.json"
        profile = json.loads(profile_path.read_text())
        contract = profile["legacy_main_imports"]
        contract["providers"][0]["bindings"][0]["address"] = "0x8004F000"
        stable = sorted((row["unit"], row["address"], row["type_identity_sha256"])
                        for provider in contract["providers"] for row in provider["bindings"])
        contract["roster_sha256"] = hashlib.sha256(
            json.dumps(stable, separators=(",", ":"), ensure_ascii=True).encode()).hexdigest()
        profile_path.write_text(json.dumps(profile))
        model = sp.Model(str(self.inc), str(self.root / "src"))
        self.assertTrue(any("pinned reviewed" in p for p in model.profile_problems), model.profile_problems)

        self.fixture()
        profile = json.loads(profile_path.read_text())
        del profile["legacy_main_imports"]["providers"][0]["bindings"][0]["current_name"]
        profile_path.write_text(json.dumps(profile))
        model = sp.Model(str(self.inc), str(self.root / "src"))
        self.assertTrue(any("incomplete binding" in p for p in model.profile_problems), model.profile_problems)

    def test_unit_symbol_aliases_resolve_without_global_alias_fallback(self):
        self.fixture()
        profile_path = self.root / "config/header_owners.json"
        profile = json.loads(profile_path.read_text())
        for unit, address in (("EVENT", "0x80050004"), ("GYOZI", "0x80070004")):
            binding = next(row for row in profile["legacy_main_imports"]["providers"]
                           if row["unit"] == unit)["bindings"][1]
            old = binding["current_name"]
            binding["current_name"] = "shared_private"
            symbol_file = self.root / ("config/symbols_%s.txt" % unit)
            symbol_file.write_text("shared_private = %s; // type:func\n" % address)
            yaml = self.root / ("config/overlays/%s.yaml" % unit)
            yaml.write_text("symbol_addrs_path:\n  - config/symbols_%s.txt\nsegments: []\n" % unit)
            provider = self.inc / ("ovl/%s_main_imports.h" % unit)
            provider.write_text(provider.read_text().replace(old + "(", "shared_private(", 1))
            source = self.root / ("src/ovl/%s/import_probe.c" % unit)
            source.write_text(source.read_text().replace(old + "(", "shared_private(", 1))
        profile_path.write_text(json.dumps(profile))
        model = sp.Model(str(self.inc), str(self.root / "src"))
        self.assertEqual(model.addr("shared_private"), None)
        self.assertEqual(model.symbols.resolve("EVENT", "shared_private")[1], 0x80050004)
        self.assertEqual(model.symbols.resolve("GYOZI", "shared_private")[1], 0x80070004)
        self.assertFalse(sp.is_main_symbol(str(self.root), "shared_private", "EVENT"))
        self.assertFalse(sp.is_main_symbol(str(self.root), "shared_private", "GYOZI"))
        problems = sp.check_api(str(self.inc), str(self.root / "src"))
        self.assertEqual(problems, [])

    def test_source_local_and_implicit_overrides_are_unit_scoped(self):
        self.fixture()
        event = self.root / "src/ovl/EVENT/import_probe.c"
        event.write_text(
            '#define MAIN_API_OVERRIDE_func_80050000 /* matched local EVENT view */\n'
            '#include "ovl/EVENT.h"\ns32 func_80050000(void);\n'
            's32 event_probe(void) { return func_80050000(); }\n')
        provider = self.inc / "ovl/EVENT_main_imports.h"
        provider.write_text(provider.read_text().replace(
            "void func_80050000();",
            "#ifndef MAIN_API_OVERRIDE_func_80050000\nvoid func_80050000();\n#endif", 1))
        self.assertEqual(sp.check_api(str(self.inc), str(self.root / "src")), [])

        event.write_text(
            '#define MAIN_API_OVERRIDE_func_80050000 /* implicit call view retained by EVENT */\n'
            '#include "ovl/EVENT.h"\nvoid event_probe(void) { func_80050000(); }\n')
        self.assertEqual(sp.check_api(str(self.inc), str(self.root / "src")), [])

    def test_undocumented_or_duplicate_source_views_are_rejected(self):
        self.fixture()
        event = self.root / "src/ovl/EVENT/import_probe.c"
        event.write_text('#include "ovl/EVENT.h"\ns32 func_80050000(void);\n'
                         'void event_probe(void) { func_80050000(); }\n')
        self.assertTrue(sp.check_api(str(self.inc), str(self.root / "src")))

        self.fixture()
        event = self.root / "src/ovl/EVENT/import_probe.c"
        event.write_text('#define MAIN_API_OVERRIDE_func_80050000 /* local view */\n'
                         '#include "ovl/EVENT.h"\ns32 func_80050000(void);\n'
                         's32 func_80050000(s32);\nvoid event_probe(void) { func_80050000(); }\n')
        provider = self.inc / "ovl/EVENT_main_imports.h"
        provider.write_text(provider.read_text().replace(
            "void func_80050000();",
            "#ifndef MAIN_API_OVERRIDE_func_80050000\nvoid func_80050000();\n#endif", 1))
        self.assertTrue(sp.check_api(str(self.inc), str(self.root / "src")))

    def test_foreign_provider_leak_is_rejected_even_with_valid_own_call(self):
        self.fixture()
        gyozi = self.root / "src/ovl/GYOZI/import_probe.c"
        gyozi.write_text('#include "ovl/GYOZI.h"\n#include "ovl/EVENT_main_imports.h"\n'
                         's32 gyozi_probe(void) { return func_80050000(); }\n')
        problems = sp.check_api(str(self.inc), str(self.root / "src"))
        self.assertTrue(any("provider leaks into source ovl/GYOZI/import_probe.c" in p
                            for p in problems), problems)

    def test_invalid_write_and_fix_leave_all_files_unchanged(self):
        self.fixture()
        root = self.inc / "ovl/EVENT.h"
        root.write_text(root.read_text().replace('#include "ovl/EVENT_main_imports.h"', ""))
        before = self.tree_bytes()
        self.assertNotEqual(sp.run_write(str(self.inc), str(self.root / "src"), fix=False, log=lambda _: None), 0)
        self.assertEqual(self.tree_bytes(), before)
        self.assertNotEqual(sp.run_write(str(self.inc), str(self.root / "src"), fix=True, log=lambda _: None), 0)
        self.assertEqual(self.tree_bytes(), before)

    def test_absent_profile_retains_legacy_main_range_behavior(self):
        self.fixture()
        (self.root / "config/header_owners.json").unlink()
        self.assertTrue(sp.is_main_symbol(str(self.root), "func_80050000", "EVENT"))

    def test_import_type_chain_reads_explicit_type_rename_ledgers(self):
        self.fixture()
        ledger = self.root / "config/renames/topic-types.txt"
        ledger.parent.mkdir(parents=True, exist_ok=True)
        ledger.write_text("type main OldType NewType 0x80000000 reviewed\n")
        self.assertEqual(sp.current_import_type(str(self.root), "OldType *"), "NewType *")


class CheckApiTest(Repo):
    def test_write_adds_new_main_definition_called_earlier(self):
        self.src("main/8007BB00.c", '#include "game.h"\n'
                 's32 func_8007BB00(void) { return func_8007BBE8(1); }\n'
                 's32 func_8007BBE8(s32 x) { return x; }\n')
        sp.run_write(str(self.inc), str(self.root / "src"), log=lambda _: None)
        text = (self.inc / "main_api.h").read_text()
        self.assertIn("func_8007BBE8(s32", text)
        self.assertEqual(text.count("func_8007BBE8("), 1)
        self.assertEqual(self.problems(), [])

    def test_clean(self):
        self.ovl("AAA", "extern s32 D_80140000;\nvoid func_80132000(void);\n")
        self.assertEqual(self.problems(), [])

    def test_write_does_not_export_address_named_static_helpers(self):
        self.src("main/8007BB00.c", 'static s32 func_8007BBE8(s32 x) { return x; }\n')
        sp.run_write(str(self.inc), str(self.root / "src"), log=lambda _: None)
        self.assertNotIn("func_8007BBE8", (self.inc / "main_api.h").read_text())

    def test_write_explains_new_definition_with_private_type(self):
        self.src("main/8007BB00.c", 'typedef struct Local { s32 x; } Local;\n'
                 'Local *func_8007BBE8(void) { return 0; }\n')
        messages = []
        sp.run_write(str(self.inc), str(self.root / "src"), log=messages.append)
        self.assertNotIn("func_8007BBE8", (self.inc / "main_api.h").read_text())
        self.assertTrue(any("SKIPPED func_8007BBE8" in s and "Local" in s for s in messages), messages)

    def test_write_adds_const_pointer_return_with_shared_types(self):
        self.src("main/8007BB00.c", 'const u8 *func_8007BBE8(void) { return 0; }\n')
        sp.run_write(str(self.inc), str(self.root / "src"), log=lambda _: None)
        self.assertIn("func_8007BBE8(void)", (self.inc / "main_api.h").read_text())

    def test_write_explains_definition_using_type_from_source_header(self):
        game = (self.inc / 'game.h').read_text()
        self.write('game.h', game.replace('#endif', 'typedef struct Entry8 { s32 x; } Entry8;\n#endif'))
        self.src('main/8007BB00.c', '#include "game.h"\nEntry8 *func_8007BBE8(void) { return 0; }\n')
        messages = []
        sp.run_write(str(self.inc), str(self.root / 'src'), log=messages.append)
        self.assertNotIn('func_8007BBE8', (self.inc / 'main_api.h').read_text())
        self.assertTrue(any('SKIPPED func_8007BBE8' in s and 'Entry8' in s for s in messages), messages)

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

    def test_write_turns_knr_definition_into_unprototyped_declaration(self):
        for definition in ('void func_80042808(a, b)\nu8 a;\ns16 b;\n{\n}\n',
                           'void func_80042808(a) int a; { }\n'):
            self.src("main/80042808.c", definition)
            model = sp.Model(str(self.inc), str(self.root / "src"))
            self.assertEqual(sp.definitions(model)["func_80042808"],
                             ("voidF()", "void func_80042808()"))
            sp.write_api(model, sp.plan(model))
            self.assertIn("void func_80042808();", (self.inc / "main_api.h").read_text())
            self.assertNotIn("func_80042808(void)", (self.inc / "main_api.h").read_text())

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


class SkippedAndAddGlobalTest(Repo):
    def test_symbol_with_a_header_local_type_is_reported_not_silent(self):
        self.ovl("AAA", "typedef struct Rec {\n    s32 a;\n} Rec;\nextern Rec D_800E7399;\n")
        m = sp.Model(str(self.inc), str(self.root / "src"))
        sp.plan(m)
        self.assertIn("D_800E7399", m.skipped)
        self.assertIn("move the typedef of Rec into include/main_api.h", m.skipped["D_800E7399"])
        lines = []
        sp.report_skipped(m, lines.append)
        self.assertTrue(lines[0].startswith("SKIPPED D_800E7399: "))

    def test_symbol_inside_an_aggregate_is_a_warning(self):
        self.write("main_api.h", API.replace("extern u8 D_800E7388;", "typedef struct GS {\n    s32 a[8];\n} GS; /* size 0x20 */\n"
                                              "extern GS D_800E7380;\nextern u8 D_800E7388;"))
        (self.root / "config" / "migrate_globals.txt").write_text("aggregate D_800E7380 GS include/main_api.h\n")
        m = sp.Model(str(self.inc), str(self.root / "src"))
        text = [t for n, t in sp.aggregate_warnings(m)]
        self.assertEqual(len(text), 2)      # D_800E7384 and D_800E7388, not the base itself
        self.assertTrue(any("D_800E7388 lies inside GS D_800E7380" in t for t in text))

    def test_kept_symbol_inside_an_aggregate_is_not_a_warning(self):
        self.write("main_api.h", API.replace("extern u8 D_800E7388;", "typedef struct GS {\n    s32 a[8];\n} GS; /* size 0x20 */\n"
                                              "extern GS D_800E7380;\nextern u8 D_800E7388;"))
        (self.root / "config" / "migrate_globals.txt").write_text(
            "aggregate D_800E7380 GS include/main_api.h\n"
            "keep src/main/AAAA.c D_800E7388 proof=f:3:0123abcd second uopt variable (T-20820)\n")
        m = sp.Model(str(self.inc), str(self.root / "src"))
        text = [t for n, t in sp.aggregate_warnings(m)]
        self.assertEqual(len(text), 1)      # D_800E7384 only; the kept D_800E7388 is declared on purpose
        self.assertIn("D_800E7384", text[0])

    def test_add_global_declares_in_address_order_and_survives_write(self):
        self.src("main/AAAA.c", "#define MAIN_API_OVERRIDE_func_80083440 /* word */\n")   # the API fixture's guard
        inc, src = str(self.inc), str(self.root / "src")
        self.assertIsNone(sp.add_global(inc, src, "D_800E7386", "s16[4]", log=lambda _m: None))
        text = (self.inc / "main_api.h").read_text()
        self.assertIn("extern s16 D_800E7386[4];", text)
        self.assertLess(text.index("D_800E7384"), text.index("D_800E7386"))
        self.assertLess(text.index("D_800E7386"), text.index("D_800E7388"))
        m = sp.Model(inc, src)
        changed, _n, _g = sp.write_api(m, sp.plan(m))
        self.assertFalse(changed)

    def test_add_global_keeps_guards_and_changes_one_line(self):
        """T-10400: --add-global dropped every `#ifndef MAIN_API_OVERRIDE_<sym>` guard whose override
        lives in a .c file (it read only the headers), and re-decided unrelated declarations."""
        old = (sp.HEADER_TEXT + "\n/* ---- globals ---- */\n"
               "#ifndef MAIN_API_OVERRIDE_D_800E7388\nextern u8 D_800E7388;\n#endif\n"
               "/* keep: out of order on purpose */\nextern s32 D_800E7384;\n"
               "\n/* ---- functions ---- */\nvoid func_80042808(void);\n"
               "#ifndef MAIN_API_OVERRIDE_func_80083440\nvoid func_80083440(u8 arg0);\n#endif\n"
               "\n#endif /* MAIN_API_H */\n")
        self.write("main_api.h", old)
        self.src("main/AAAA.c", "#define MAIN_API_OVERRIDE_D_800E7388 /* lb */\n"
                 "#define MAIN_API_OVERRIDE_func_80083440 /* word */\n#include \"game.h\"\n")
        self.ovl("AAA", "extern u8 D_800E7000;\n")     # declared by a header, not yet in main_api.h
        inc, src = str(self.inc), str(self.root / "src")
        self.assertIsNone(sp.add_global(inc, src, "D_800E7386", "s16", log=lambda _m: None))
        added = (self.inc / "main_api.h").read_text()
        line = "extern s16 D_800E7386;\n"
        self.assertIn(line, added)
        self.assertEqual(added.replace(line, "", 1), old)      # one line more, nothing else touched
        self.assertLess(added.index("D_800E7386"), added.index("MAIN_API_OVERRIDE_D_800E7388"))
        # the --write after it changes only what --write alone would (the header's own D_800E7000)
        m = sp.Model(inc, src)
        sp.write_api(m, sp.plan(m))
        after = (self.inc / "main_api.h").read_text()
        self.assertEqual(after.replace("extern u8 D_800E7000;\n", "", 1), added)
        self.assertIn("#ifndef MAIN_API_OVERRIDE_D_800E7388\nextern u8 D_800E7388;\n#endif", after)

    def test_add_global_refuses_known_or_non_main_names(self):
        inc, src = str(self.inc), str(self.root / "src")
        self.assertIn("already declared", sp.add_global(inc, src, "D_800E7388", "u8"))
        self.assertIn("not a main-exe global", sp.add_global(inc, src, "D_80200000", "u8"))
        self.assertIn("not a main-exe global", sp.add_global(inc, src, "func_80042808", "u8"))

    def test_add_global_cli_names_types_and_boundaries(self):
        (self.root / "config" / "symbol_addrs.txt").write_text("renamed_data = 0x80122EA0;\n")
        cases = [("D_80122E98", "s32", "s32 D_80122E98"),
                 ("D_80122E99", "u8[]", "u8 D_80122E99[]"),
                 ("D_80122E9A", "s16[4]", "s16 D_80122E9A[4]"),
                 ("renamed_data", "const u8 *", "const u8 *renamed_data"),
                 ("D_80041000", "u32", "u32 D_80041000"),
                 ("D_8012B537", "u8[0x10]", "u8 D_8012B537[0x10]")]
        for name, typ, decl in cases:
            with self.subTest(name=name, typ=typ):
                self.assertEqual(sp.main(["--inc", str(self.inc), "--src", str(self.root / "src"),
                                          "--add-global", name, typ]), 0)
                self.assertIn("extern " + decl + ";", (self.inc / "main_api.h").read_text())
        for name in ("D_80040FFF", "D_8012B538", "D_80132000", "unknown", "D_80122E9B;"):
            self.assertIn("not a main-exe global", sp.add_global(str(self.inc), None, name, "u8[]"))

    def test_add_global_bad_type_does_not_write(self):
        before = (self.inc / "main_api.h").read_text()
        for typ in ("", "u8; void evil()", "u8[", "u8()"):
            self.assertIn("cannot read", sp.add_global(str(self.inc), None, "D_80122E98", typ))
        self.assertEqual((self.inc / "main_api.h").read_text(), before)


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


class TopicOwnerTests(Repo):
    def topics(self, umbrella=True):
        self.write("main/schedule.h", '#ifndef MAIN_SCHEDULE_H\n#define MAIN_SCHEDULE_H\n'
                   '#include "common.h"\n/* Preserve this table comment and conditional. */\n#if 1\n'
                   'extern s8 D_800B6740;\n#endif\nvoid func_8006CB6C(void);\n#endif\n')
        self.write("main/day_event.h", '#ifndef MAIN_DAY_EVENT_H\n#define MAIN_DAY_EVENT_H\n'
                   '#include "common.h"\ns32 func_8006D6E0();\nvoid func_8007259C(u8 arg0);\n#endif\n')
        self.write("main/school_day.h", '#ifndef MAIN_SCHOOL_DAY_H\n#define MAIN_SCHOOL_DAY_H\n'
                   '#include "common.h"\nextern s8 D_800B68E0[8][9];\n'
                   'u8 func_80073AD8(void);\nvoid func_800744A0();\n#endif\n')
        if umbrella:
            self.write("main_api.h", API.replace('#include "common.h"', '#include "common.h"\n'
                       '#include "main/schedule.h"\n#include "main/day_event.h"\n#include "main/school_day.h"'))
        else:
            (self.inc / "main_api.h").unlink(missing_ok=True)
            (self.inc / "game.h").unlink(missing_ok=True)

    def model(self):
        return sp.Model(str(self.inc), str(self.root / "src"))

    def profile(self, owners, required_types=("common.h",)):
        (self.root / "config" / "header_owners.json").write_text(json.dumps({
            "mode": "topic_only", "owners": owners, "required_types": list(required_types)}))

    def owner_bytes(self):
        return {h: (self.inc / h).read_bytes() for h in sp.canonical_headers(str(self.inc))}

    def test_explicit_registration_and_provenance_without_umbrellas(self):
        self.topics(False)
        self.write("main/unregistered.h", "void func_8006CC00(void);\n")
        self.src("main/use.c", '#include "main/day_event.h"\ns32 use(void) { return func_8006D6E0(3); }\n')
        model = self.model()
        self.assertTrue(model.api_present)
        self.assertEqual(model.api_owner["func_8006D6E0"], "main/day_event.h")
        self.assertNotIn("func_8006CC00", model.api)
        self.assertEqual(model.api_declarations["func_8006D6E0"][0][0], "main/day_event.h")
        self.assertTrue(any("func_8006CC00" in p and "registered topic" in p
                            for p in sp.check_api(str(self.inc), str(self.root / "src"))))

    def test_registered_owner_wins_over_consumer_inference(self):
        from unittest.mock import patch
        self.topics(False)
        model = self.model()
        with patch.object(sp.srcscan, "header_units", return_value={"main/day_event.h": "AAA"}):
            self.assertIsNone(model.owner("main/day_event.h"))
            self.assertTrue(model.is_main("func_8006D6E0", "main/day_event.h"))

    def test_disjoint_canonical_duplicates_fail_before_writing(self):
        for second in ("s32 func_8006D6E0();", "void func_8006D6E0(u8 x);"):
            with self.subTest(second=second):
                self.topics(False)
                p = self.inc / "main/schedule.h"
                p.write_text(p.read_text().replace("#include \"common.h\"", '#include "common.h"\n' + second))
                before = self.owner_bytes()
                problems = sp.check_api(str(self.inc), str(self.root / "src"))
                self.assertTrue(any("duplicate canonical owner for func_8006D6E0" in p for p in problems), problems)
                self.assertGreater(sp.run_write(str(self.inc), str(self.root / "src"), log=lambda _: None), 0)
                self.assertEqual(before, self.owner_bytes())

    def test_api_and_topic_are_not_two_owners(self):
        self.topics()
        self.write("main_api.h", API + "\ns32 func_8006D6E0();\n")
        self.assertTrue(any("duplicate canonical owner" in p for p in sp.check_api(str(self.inc))))

    def test_write_and_fix_preserve_bytes_views_and_implicit_definitions(self):
        for umbrella in (True, False):
            with self.subTest(umbrella=umbrella):
                # Restore the legacy fixture before the second registration.
                self.write("main_api.h", API)
                self.write("game.h", '#include "main_api.h"\n')
                self.topics(umbrella)
                self.src("main/pilot.c", '#include "main/day_event.h"\n'
                         's32 caller(void) { return func_8006F524(1); }\n'
                         's32 func_8006F524(s32 x) { return x; }\n'
                         's32 func_8006D6E0(void) { return 1; }\n')
                self.src("ovl/AAA/use.c", '#include "main/day_event.h"\n#include "main/school_day.h"\n'
                         's32 use(void) { func_800744A0(1, 2, 3); return func_8006D6E0(4); }\n')
                before, views = self.owner_bytes(), sp.snapshot(self.model())
                for fix in (False, True):
                    self.assertEqual(sp.run_write(str(self.inc), str(self.root / "src"), fix=fix,
                                                  log=lambda _: None), 0)
                    self.assertEqual(before, self.owner_bytes())
                    self.assertEqual(sp.view_changes(views, sp.snapshot(self.model())), [])
                    self.assertNotIn("func_8006F524", self.model().api)
                self.assertEqual((self.inc / "main_api.h").exists(), umbrella)

    def test_fix_keeps_topic_destination_and_guards_a_used_override(self):
        self.topics(False)
        self.ovl("AAA", "void func_8007259C(s32 arg0);\n", head='#include "common.h"\n')
        self.src("ovl/AAA/a.c", '#include "ovl/AAA.h"\nvoid use(void) { func_8007259C(300); }\n')
        views = sp.snapshot(self.model())
        self.assertEqual(sp.run_write(str(self.inc), str(self.root / "src"), fix=True, log=lambda _: None), 0)
        owner = (self.inc / "main/day_event.h").read_text()
        overlay = (self.inc / "ovl/AAA.h").read_text()
        self.assertIn("#ifndef MAIN_API_OVERRIDE_func_8007259C", owner)
        self.assertIn('#include "main/day_event.h"', overlay)
        self.assertNotIn('main/schedule.h', overlay)
        self.assertIn("#define MAIN_API_OVERRIDE_func_8007259C", overlay)
        self.assertEqual(sp.view_changes(views, sp.snapshot(self.model())), [])
        self.assertFalse((self.inc / "main_api.h").exists())
        self.assertEqual(sp.check_api(str(self.inc), str(self.root / "src")), [])

    def test_profile_fix_can_repair_a_missing_canonical_override_guard(self):
        self.topics(False)
        self.profile([{"path": "main/schedule.h", "role": "main"},
                      {"path": "main/day_event.h", "role": "main"},
                      {"path": "main/school_day.h", "role": "main"}])
        self.ovl("AAA", "void func_8007259C(s32 arg0);\n", head='#include "common.h"\n')
        self.src("ovl/AAA/a.c", '#include "ovl/AAA.h"\nvoid use(void) { func_8007259C(300); }\n')
        self.assertTrue(any("conflict func_8007259C" in p for p in sp.check_api(
            str(self.inc), str(self.root / "src"))))
        self.assertEqual(sp.run_write(str(self.inc), str(self.root / "src"), fix=True,
                                      log=lambda _: None), 0)
        self.assertIn("#ifndef MAIN_API_OVERRIDE_func_8007259C",
                      (self.inc / "main/day_event.h").read_text())

    def test_fix_keeps_existing_benign_override_visible(self):
        self.topics(False)
        owner = self.inc / "main/day_event.h"
        owner.write_text(owner.read_text().replace("s32 func_8006D6E0();",
                         "#ifndef MAIN_API_OVERRIDE_func_8006D6E0\ns32 func_8006D6E0();\n#endif"))
        self.ovl("AAA", "s32 func_8006D6E0(s32 x);\n",
                 head='#define MAIN_API_OVERRIDE_func_8006D6E0 /* matched wide prototype */\n'
                      '#include "common.h"\n#include "main/day_event.h"\n')
        self.src("ovl/AAA/a.c", '#include "ovl/AAA.h"\ns32 use(void) { return func_8006D6E0(3); }\n')
        before = (self.inc / "ovl/AAA.h").read_bytes()
        views = sp.snapshot(self.model())
        self.assertEqual(sp.run_write(str(self.inc), str(self.root / "src"), fix=True, log=lambda _: None), 0)
        self.assertEqual((self.inc / "ovl/AAA.h").read_bytes(), before)
        self.assertEqual(sp.view_changes(views, sp.snapshot(self.model())), [])

    def test_fix_can_guard_multiline_prototype_without_reflow(self):
        self.topics(False)
        owner = self.inc / "main/day_event.h"
        declaration = "void func_8007259C(\n    u8 arg0); /* preserve trailing comment */"
        owner.write_text(owner.read_text().replace("void func_8007259C(u8 arg0);", declaration))
        self.ovl("AAA", "void func_8007259C(s32 arg0);\n", head='#include "common.h"\n')
        self.src("ovl/AAA/a.c", '#include "ovl/AAA.h"\nvoid use(void) { func_8007259C(300); }\n')
        views = sp.snapshot(self.model())
        self.assertEqual(sp.run_write(str(self.inc), str(self.root / "src"), fix=True, log=lambda _: None), 0)
        self.assertIn("#ifndef MAIN_API_OVERRIDE_func_8007259C\n" + declaration + "\n#endif", owner.read_text())
        self.assertEqual(sp.view_changes(views, sp.snapshot(self.model())), [])
        self.assertEqual(sp.check_api(str(self.inc), str(self.root / "src")), [])

    def test_fix_rolls_back_every_file_if_a_late_guard_cannot_be_added(self):
        self.topics(False)
        owner = self.inc / "main/day_event.h"
        owner.write_text(owner.read_text().replace("void func_8007259C(u8 arg0);",
                         "void func_8007259C(u8 arg0); void func_800725A0(void);"))
        self.ovl("AAA", "void func_8007259C(s32 arg0);\n", head='#include "common.h"\n')
        self.src("ovl/AAA/a.c", '#include "ovl/AAA.h"\nvoid use(void) { func_8007259C(300); }\n')
        before = {p: p.read_bytes() for p in self.root.rglob("*.h")}
        before.update({p: p.read_bytes() for p in self.root.rglob("*.c")})
        messages = []
        self.assertGreater(sp.run_write(str(self.inc), str(self.root / "src"), fix=True, log=messages.append), 0)
        self.assertTrue(any("cannot add override guard" in p for p in messages), messages)
        self.assertTrue(any("restored original" in p for p in messages), messages)
        self.assertEqual(before, {p: p.read_bytes() for p in before})
        self.assertFalse((self.inc / "main_api.h").exists())

    def test_missing_owner_errors_without_recreating_umbrella(self):
        self.topics(False)
        self.write("main/unregistered.h", "void func_8006CC00(void);\n")
        before, messages = self.owner_bytes(), []
        self.assertGreater(sp.run_write(str(self.inc), str(self.root / "src"), log=messages.append), 0)
        self.assertTrue(any("no canonical declaration owner" in p for p in messages), messages)
        self.assertEqual(before, self.owner_bytes())
        self.assertFalse((self.inc / "main_api.h").exists())
        self.assertIn("explicit declaration", sp.add_global(str(self.inc), None, "D_800B67A0", "u8"))

    def test_overlay_private_record_exception_does_not_require_a_new_owner(self):
        self.topics(False)
        self.ovl("AAA", "typedef struct Local { s32 x; } Local;\nextern Local D_80120000[];\n",
                 head='#include "common.h"\n')
        before = (self.inc / "ovl/AAA.h").read_bytes()
        self.assertEqual(sp.run_write(str(self.inc), str(self.root / "src"), fix=True, log=lambda _: None), 0)
        self.assertEqual((self.inc / "ovl/AAA.h").read_bytes(), before)
        self.assertFalse((self.inc / "main_api.h").exists())

    def test_registered_declaration_owns_a_name_even_before_symbol_map_refresh(self):
        self.topics(False)
        self.write("main/schedule.h", '#include "common.h"\nvoid newly_named_rule(void);\n')
        self.write("other.h", '#include "common.h"\nvoid newly_named_rule(void);\n')
        self.assertTrue(any("duplicate newly_named_rule" in p for p in sp.check_api(str(self.inc))))

    def test_new_candidate_falls_back_to_existing_api_not_topic(self):
        self.topics()
        self.write("main/unregistered.h", "void func_8006CC00(void);\n")
        topics = {h: b for h, b in self.owner_bytes().items() if h != "main_api.h"}
        self.assertEqual(sp.run_write(str(self.inc), str(self.root / "src"), log=lambda _: None), 0)
        self.assertEqual(self.model().api_owner["func_8006CC00"], "main_api.h")
        for h, before in topics.items():
            self.assertEqual((self.inc / h).read_bytes(), before)

    def test_sdk_declarations_are_not_accidental_topic_owners(self):
        self.topics(False)
        self.write("libgpu.h", '#include "common.h"\nvoid func_8009F004();\n')
        self.assertNotIn("func_8009F004", self.model().api)
        self.write("main/schedule.h", '#include "common.h"\nvoid func_8009F004();\n')
        self.assertTrue(any("SDK symbol func_8009F004" in p for p in sp.check_api(str(self.inc))))
        self.assertGreater(sp.run_write(str(self.inc), str(self.root / "src"), log=lambda _: None), 0)

    def test_definition_mismatch_and_disjoint_source_duplicate_still_fail(self):
        self.topics(False)
        self.src("main/a.c", '#include "main/day_event.h"\nvoid func_8007259C(s32 x) {}\n')
        self.src("main/b.c", '#include "common.h"\ns32 func_8006D6E0();\n')
        problems = sp.check_api(str(self.inc), str(self.root / "src"))
        self.assertTrue(any("main exe defines" in p and "func_8007259C" in p for p in problems), problems)
        self.assertTrue(any("duplicate func_8006D6E0" in p for p in problems), problems)

    def test_per_symbol_missing_include_and_main_branch_view_without_umbrella(self):
        self.topics(False)
        self.src("main/a.c", '#include "main/schedule.h"\ns32 use(void) { return func_8006D6E0(); }\n')
        problems = sp.check_api(str(self.inc), str(self.root / "src"))
        self.assertTrue(any("without include/main/day_event.h" in p for p in problems), problems)
        self.assertEqual(sp.unit_types(self.model(), "main")["func_8006D6E0"], "s32F()")

    def test_override_guard_must_be_at_the_actual_owner(self):
        self.topics(False)
        self.write("main/schedule.h", '#include "common.h"\n#ifndef MAIN_API_OVERRIDE_func_8007259C\n#endif\n')
        self.src("main/a.c", '#define MAIN_API_OVERRIDE_func_8007259C /* matched wide */\n'
                 '#include "main/day_event.h"\nvoid func_8007259C(s32 x);\nvoid use(void) { func_8007259C(300); }\n')
        self.assertTrue(any("not guarded in include/main/day_event.h" in p
                            for p in sp.check_api(str(self.inc), str(self.root / "src"))))

    def test_explicit_profile_missing_all_owners_fails_check_write_fix_without_changes(self):
        self.topics(False)
        self.profile([{"path": "main_api.h", "role": "main"}])
        before = {p: p.read_bytes() for p in self.root.rglob("*") if p.is_file()}
        self.assertTrue(any("required main owner include/main_api.h is missing" in p
                            for p in sp.check_api(str(self.inc), str(self.root / "src"))))
        for fix in (False, True):
            self.assertGreater(sp.run_write(str(self.inc), str(self.root / "src"), fix=fix,
                                            log=lambda _: None), 0)
            self.assertEqual(before, {p: p.read_bytes() for p in before})
            self.assertFalse((self.inc / "main_api.h").exists())

    def test_explicit_sdk_self_owner_passes_but_disjoint_equal_type_fails(self):
        (self.inc / "main_api.h").unlink()
        (self.inc / "game.h").unlink()
        self.write("libgte.h", '#include "common.h"\nvoid func_8009F004(void);\n')
        self.write("main/schedule.h", '#include "common.h"\n')
        self.profile([{"path": "main/schedule.h", "role": "main"},
                      {"path": "libgte.h", "role": "sdk"}])
        self.assertEqual(sp.check_api(str(self.inc)), [])
        self.write("main/schedule.h", '#include "common.h"\nvoid func_8009F004(void);\n')
        self.assertTrue(any("duplicate canonical owner" in p for p in sp.check_api(str(self.inc))))

    def test_explicit_sdk_owner_keeps_main_alias_type_conflicts_visible(self):
        (self.inc / "main_api.h").unlink()
        (self.inc / "game.h").unlink()
        self.write("libgte.h", '#include "common.h"\ns32 func_8009F004(void);\nextern u8 SdkAlias;\n')
        (self.root / "config" / "symbol_addrs.txt").write_text(
            "SenseMouse = 0x800460E0; // type:func\nSdkAlias = 0x8009F004; // type:data\n")
        self.profile([{"path": "libgte.h", "role": "sdk"}])
        self.assertTrue(any("alias conflict at 0x8009F004" in p for p in sp.check_api(str(self.inc))),
                        sp.check_api(str(self.inc)))

    def test_legacy_sdk_paths_keep_alias_conflicts_after_owner_relocation(self):
        for sdk in ("libgpu.h", "libapi.h", "libgte.h"):
            with self.subTest(sdk=sdk):
                self.topics(False)
                for other in ("libgpu.h", "libapi.h", "libgte.h"):
                    (self.inc / other).unlink(missing_ok=True)
                (self.root / "config" / "symbol_addrs.txt").write_text(
                    "SdkAlias = 0x8009F004; // type:data\n")
                self.write("main/schedule.h", '#include "common.h"\nextern u8 SdkAlias;\n')
                self.write(sdk, '#include "common.h"\ns32 func_8009F004(void);\n')
                self.profile([{"path": "main/schedule.h", "role": "main"},
                              {"path": sdk, "role": "sdk"}])
                found = sp.alias_conflicts(self.model())
                self.assertTrue(any(key == "alias 0x8009F004" for key, _ in found),
                                (sdk, found))

    def test_profile_rejects_symlink_escape_and_normalized_alias(self):
        self.topics(False)
        outside = self.root / "outside.h"
        outside.write_text('#include "common.h"\n')
        (self.inc / "escape.h").symlink_to(outside)
        for path in ("escape.h", "main/./schedule.h"):
            self.profile([{"path": path, "role": "main"}])
            self.assertTrue(any("invalid owner path" in p for p in sp.check_api(str(self.inc))), path)

    def test_profile_type_leaf_rejects_extern(self):
        self.write("types.h", '#include "common.h"\ntypedef struct T { s32 x; } T;\nextern T D_800E0000;\n')
        self.profile([], ("common.h", "types.h"))
        self.assertTrue(any("type leaf include/types.h must not declare" in p
                            for p in sp.check_api(str(self.inc))))

    def test_profile_type_leaf_rejects_function_and_object_definitions(self):
        self.profile([], ("common.h", "types.h"))
        for content in ("s32 f(void) { return 0; }\n", "s32 values[2] = {0, 0};\n"):
            with self.subTest(content=content):
                self.write("types.h", content)
                self.assertTrue(any("type leaf include/types.h must not declare" in p
                                    for p in sp.check_api(str(self.inc))))

    def test_profile_accepts_a_pure_typedef_type_leaf(self):
        (self.inc / "main_api.h").unlink()
        (self.inc / "game.h").unlink()
        self.write("main/schedule.h", '#include "common.h"\n')
        self.write("types.h", '#include "common.h"\ntypedef struct T { s32 x; } T;\n')
        self.profile([{"path": "main/schedule.h", "role": "main"}], ("common.h", "types.h"))
        self.assertEqual(sp.check_api(str(self.inc)), [])

    def test_profile_accepts_pure_named_enum_and_rejects_instance(self):
        (self.inc / "main_api.h").unlink()
        (self.inc / "game.h").unlink()
        self.write("main/schedule.h", '#include "common.h"\n')
        self.profile([{"path": "main/schedule.h", "role": "main"}], ("common.h", "game_enums.h"))
        self.write("game_enums.h", "enum GirlId { GIRL_SHIORI = 0, GIRL_MIO = 1 };\n")
        self.assertEqual(sp.check_api(str(self.inc)), [])
        self.write("game_enums.h", "enum GirlId { GIRL_SHIORI = 0, GIRL_MIO = 1 } current_girl;\n")
        self.assertTrue(any("type leaf include/game_enums.h must not declare" in p
                            for p in sp.check_api(str(self.inc))))

    def test_no_profile_keeps_legacy_destination_and_behavior(self):
        self.assertIsNone(sp.owner_profile(str(self.inc))[0])
        self.src("main/new.c", '#include "game.h"\ns32 func_8007BB00(void) { return 0; }\n')
        self.assertEqual(sp.run_write(str(self.inc), str(self.root / "src"), log=lambda _: None), 0)
        self.assertIn("func_8007BB00", (self.inc / "main_api.h").read_text())


class StrictCompareTests(Repo):
    def test_cli_strict_gate_requires_zero_in_every_class(self):
        import io
        from contextlib import redirect_stdout
        self.src("main/use.c", '#include "game.h"\nvoid use(void) { func_80042808(); }\n')
        actual = sp.snapshot(sp.Model(str(self.inc), str(self.root / "src")))
        fixture = self.root / "views.json"
        args = ["--inc", str(self.inc), "--src", str(self.root / "src"), "--compare", str(fixture)]
        cases = [("identical", actual, 0, 0)]
        for label, signature in (("benign", "voidF()"), ("risky", "s32F(void)")):
            old = json.loads(json.dumps(actual))
            old["src/main/use.c"]["func_80042808"] = signature
            cases.append((label, old, 0 if label == "benign" else 1, 1))
        old = json.loads(json.dumps(actual))
        del old["src/main/use.c"]["func_80042808"]  # implicit -> void is a build-confirmation view change
        cases.append(("check", old, 0, 1))
        for label, old, normal, strict in cases:
            with self.subTest(label=label), redirect_stdout(io.StringIO()):
                fixture.write_text(json.dumps(old))
                self.assertEqual(sp.main(args), normal)
                self.assertEqual(sp.main(args + ["--strict-compare"]), strict)

    def test_strict_compare_requires_a_snapshot_argument(self):
        import io
        from contextlib import redirect_stderr
        with redirect_stderr(io.StringIO()), self.assertRaises(SystemExit) as error:
            sp.main(["--strict-compare"])
        self.assertEqual(error.exception.code, 2)


class MergeStructureTests(unittest.TestCase):
    def test_fix_rejects_broken_headers_without_writing(self):
        import io
        from contextlib import redirect_stdout
        from pathlib import Path
        with tempfile.TemporaryDirectory() as tmp:
            inc = Path(tmp) / "include"
            inc.mkdir()
            bad = "#if 1\ntypedef struct A { int x;\ntypedef struct B { int y; } B;\n} A;\n"
            (inc / "broken.h").write_text(bad)
            out = io.StringIO()
            with redirect_stdout(out):
                rc = sp.main(["--inc", str(inc), "--src", tmp, "--fix"])
            self.assertEqual(rc, 1)
            self.assertIn("unterminated #if", out.getvalue())
            self.assertIn("typedef inside struct/union body", out.getvalue())
            self.assertEqual((inc / "broken.h").read_text(), bad)
            self.assertFalse((inc / "main_api.h").exists())
            messages = []
            self.assertGreater(sp.run_write(str(inc), tmp, fix=True, log=messages.append), 0)
            self.assertFalse((inc / "main_api.h").exists())


if __name__ == "__main__":
    unittest.main()
