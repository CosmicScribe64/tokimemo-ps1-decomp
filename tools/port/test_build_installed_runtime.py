"""Synthetic contract tests for installed-runtime bundle assembly."""
import hashlib
import importlib.util
import json
from pathlib import Path
import tempfile
import unittest
import sys

MODULE = Path(__file__).with_name("build_installed_runtime.py")
SPEC = importlib.util.spec_from_file_location("build_installed_runtime", MODULE)
runtime = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(runtime)
SHIORI_MODULE = Path(__file__).with_name("native_audio") / "export_shiori_bank.py"
SHIORI_SPEC = importlib.util.spec_from_file_location("export_shiori_bank", SHIORI_MODULE)
shiori = importlib.util.module_from_spec(SHIORI_SPEC)
SHIORI_SPEC.loader.exec_module(shiori)


def _raw_form1_sector(fill: int) -> bytes:
    header = b"\x00" + b"\xff" * 10 + b"\x00" + b"\x00\x02\x00" + b"\x02"
    subheader = b"\x00\x00\x08\x00" * 2  # submode 0x08: data, Form 1
    return header + subheader + bytes([fill]) * 2048 + b"\x00" * 280


class InstalledRuntimeBundleTests(unittest.TestCase):
    def test_exporter_failure_preserves_bounded_actionable_diagnostic(self):
        ok, reason = runtime._run([sys.executable, "-c",
            "import sys; sys.stderr.write('x' * 3000 + ' missing asm/proof.s'); sys.exit(7)"], "rules")
        self.assertFalse(ok)
        self.assertIn("exit 7", reason)
        self.assertTrue(reason.endswith("missing asm/proof.s"))
        self.assertLess(len(reason), 2100)

    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="installed-runtime-test-")
        self.root = Path(self.temp.name)
        self.bundle = self.root / "bundle"
        (self.bundle / "packs/rules").mkdir(parents=True)
        (self.bundle / "packs/rules/manifest.json").write_text('{"fixture":true}\n')
        self.exe = self.root / "exe.bin"
        self.exe.write_bytes(b"synthetic executable identity")
        self.track = self.root / "track.bin"
        self.track.write_bytes(b"synthetic track identity")

    def tearDown(self):
        self.temp.cleanup()

    def test_manifest_hashes_files_and_source_inputs_deterministically(self):
        roles = {"rule_tables": "packs/rules/manifest.json"}
        first = runtime._manifest(self.bundle, self.exe, self.track, roles, {"limitations": []})
        second = runtime._manifest(self.bundle, self.exe, self.track, roles, {"limitations": []})
        self.assertEqual(first, second)
        self.assertEqual(first["schema"], "tokimemo.installed-runtime.v1")
        self.assertEqual(first["release_profile"], "original-ja-ps-the-best")
        self.assertEqual(first["files"]["packs/rules/manifest.json"], {
            "size": (self.bundle / "packs/rules/manifest.json").stat().st_size,
            "sha256": hashlib.sha256((self.bundle / "packs/rules/manifest.json").read_bytes()).hexdigest(),
        })
        self.assertEqual(first["source"]["track_sha256"], hashlib.sha256(self.track.read_bytes()).hexdigest())
        self.assertEqual(first["roles"], roles)

    def test_manifest_rejects_unknown_role_and_wrong_role_kind(self):
        for roles in ({"made_up": "packs/rules/manifest.json"},
                      {"font_root": "packs/rules/manifest.json"}):
            with self.subTest(roles=roles), self.assertRaises(runtime.BundleError):
                runtime._manifest(self.bundle, self.exe, None, roles, {})

    def test_manifest_rejects_missing_or_escaping_role(self):
        for target in ("packs/missing/manifest.json", "../outside.json"):
            with self.subTest(target=target), self.assertRaises(runtime.BundleError):
                runtime._manifest(self.bundle, self.exe, None,
                                  {"rule_tables": target}, {})

    def test_manifest_rejects_noncanonical_role_locators(self):
        inside_absolute = str(self.bundle / "packs/rules/manifest.json")
        for target in (inside_absolute, "packs/../packs/rules/manifest.json",
                       "packs//rules/manifest.json", "packs\\rules\\manifest.json"):
            with self.subTest(target=target), self.assertRaises(runtime.BundleError):
                runtime._manifest(self.bundle, self.exe, None,
                                  {"rule_tables": target}, {})

    def test_track_hash_is_bound_only_to_the_input_track(self):
        runtime._check_track_binding(self.track, self.track)
        runtime._check_track_binding(self.root, None)
        with self.assertRaises(runtime.BundleError):
            runtime._check_track_binding(self.root, self.track)
        with self.assertRaises(runtime.BundleError):
            runtime._check_track_binding(self.exe, self.track)

    def test_failed_geko_role_is_removed_before_manifest_inventory(self):
        stage = self.root / "stage"
        partial = stage / "packs/geko_dialogue"
        partial.mkdir(parents=True)
        (partial / "table-0.json").write_text('{"partial":true}\n')
        runtime._discard_partial_role(stage, "packs/geko_dialogue")
        self.assertFalse(partial.exists())
        document = runtime._manifest(stage, self.exe, None, {}, {})
        self.assertEqual(document["files"], {})

    def test_symlinked_generated_content_is_rejected(self):
        target = self.root / "outside.bin"
        target.write_bytes(b"outside")
        (self.bundle / "linked.bin").symlink_to(target)
        with self.assertRaises(runtime.BundleError):
            runtime._manifest(self.bundle, self.exe, None,
                              {"rule_tables": "packs/rules/manifest.json"}, {})

    def test_manifest_json_is_stable_and_does_not_claim_capabilities(self):
        document = runtime._manifest(self.bundle, self.exe, None,
                                     {"rule_tables": "packs/rules/manifest.json"},
                                     {"limitations": ["synthetic fixture"]})
        encoded = json.dumps(document, sort_keys=True, ensure_ascii=False, indent=2) + "\n"
        self.assertEqual(encoded, json.dumps(document, sort_keys=True, ensure_ascii=False, indent=2) + "\n")
        self.assertNotIn("capabilities", document)

    def test_geko_role_is_directory_and_keeps_section_index_hashable(self):
        geko = self.bundle / "packs/geko_dialogue"
        section = geko / "table-0"
        section.mkdir(parents=True)
        (section / "content.json").write_text('{"fixture":true}\n')
        (geko / "index.json").write_text(
            '{"section_count":12,"sections":[{"id":"table-0","path":"table-0/content.json"}]}\n'
        )
        document = runtime._manifest(self.bundle, self.exe, None,
                                     {"geko_dialogue": "packs/geko_dialogue"}, {})
        self.assertIn("packs/geko_dialogue/index.json", document["files"])
        self.assertIn("packs/geko_dialogue/table-0/content.json", document["files"])
        self.assertEqual(runtime.ROLE_KINDS["geko_dialogue"], "dir")

    def test_font_role_uses_native_loader_relative_root(self):
        font = self.bundle / ".asset-cache/port/fonts/native-font-v2"
        font.mkdir(parents=True)
        (font / "font.json").write_text('{"schema":"tokimemo.native-font.v1"}\n')
        (font / "glyph_bank.bin").write_bytes(b"synthetic")
        document = runtime._manifest(
            self.bundle, self.exe, None,
            {"font_root": ".asset-cache/port/fonts/native-font-v2"}, {})
        self.assertEqual(document["roles"]["font_root"], ".asset-cache/port/fonts/native-font-v2")

    def test_holiday_scene_stays_unavailable_when_source_profile_is_present(self):
        roles = {"holiday_scene_source": "packs/holiday_scene_source/manifest.json",
                 "rule_tables": "packs/rules/manifest.json"}
        coverage = runtime._coverage(roles, {"school_day_media": "export failed"})
        self.assertIn("holiday_scene", coverage["unavailable_roles"])
        self.assertIn("session-owned boundary fields", coverage["unavailable_roles"]["holiday_scene"])
        self.assertIn("hud_context", coverage["unavailable_roles"])
        self.assertEqual(coverage["unavailable_roles"]["school_day_media"], "export failed")
        self.assertNotIn("holiday_scene", coverage["available_roles"])
        self.assertIn("holiday_scene_source", coverage["available_roles"])
        self.assertTrue(any("holiday_scene_source" in item for item in coverage["limitations"]))
        self.assertEqual(runtime.ROLE_KINDS["holiday_scene_source"], "file")

    def test_unsupplied_gameplay_role_cannot_be_published(self):
        for role in runtime.UNSUPPLIED_ROLES:
            with self.subTest(role=role), self.assertRaises(runtime.BundleError):
                runtime._coverage({role: "packs/rules/manifest.json"}, {})

    def test_first_arrival_board_role_is_directory(self):
        board = self.bundle / "packs/first_arrival_board"
        board.mkdir(parents=True)
        (board / "manifest.json").write_text('{"schema":"tokimemo.first-arrival-board.v1"}\n')
        (board / "board.bs").write_bytes(b"synthetic")
        document = runtime._manifest(
            self.bundle, self.exe, None,
            {"first_arrival_board": "packs/first_arrival_board"}, {})
        self.assertEqual(runtime.ROLE_KINDS["first_arrival_board"], "dir")
        self.assertEqual(document["roles"]["first_arrival_board"], "packs/first_arrival_board")
        self.assertIn("packs/first_arrival_board/manifest.json", document["files"])
        self.assertIn("packs/first_arrival_board/board.bs", document["files"])

    def test_first_arrival_finish_bank_role_is_directory(self):
        # T-39002: prologue_finish_run case 0's five reads, finish_bank.json
        # being the manifest the Godot store parses.
        finish = self.bundle / "packs/first_arrival_finish_bank"
        (finish / "reads").mkdir(parents=True)
        (finish / "finish_bank.json").write_text('{"schema":"tokimemo.prologue-finish-bank.v1"}\n')
        (finish / "reads/se_header.bin").write_bytes(b"synthetic")
        document = runtime._manifest(
            self.bundle, self.exe, None,
            {"first_arrival_finish_bank": "packs/first_arrival_finish_bank"}, {})
        self.assertEqual(runtime.ROLE_KINDS["first_arrival_finish_bank"], "dir")
        self.assertIn("packs/first_arrival_finish_bank/finish_bank.json", document["files"])
        self.assertIn("packs/first_arrival_finish_bank/reads/se_header.bin", document["files"])

    def test_first_arrival_command_reload_role_is_directory(self):
        # T-39002: the mode 0x34 reload's CALROM and room reads, with
        # command_reload.json being the manifest the Godot store parses.
        reload = self.bundle / "packs/first_arrival_command_reload"
        (reload / "reads").mkdir(parents=True)
        (reload / "command_reload.json").write_text('{"schema":"tokimemo.command-reload-reads.v1"}\n')
        (reload / "reads/hud_calendar.bin").write_bytes(b"synthetic")
        document = runtime._manifest(
            self.bundle, self.exe, None,
            {"first_arrival_command_reload": "packs/first_arrival_command_reload"}, {})
        self.assertEqual(runtime.ROLE_KINDS["first_arrival_command_reload"], "dir")
        self.assertIn("packs/first_arrival_command_reload/command_reload.json", document["files"])
        self.assertIn("packs/first_arrival_command_reload/reads/hud_calendar.bin", document["files"])

    def test_command_reload_exporter_parses_the_source_reads(self):
        # The CALROM read and the slot-0 room read come from the C source.
        path = Path(__file__).with_name("command_reload_export.py")
        spec = importlib.util.spec_from_file_location("command_reload_export", path)
        export = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(export)
        reads = export.source_reads(export.ROOT)
        self.assertEqual([reads["hud_calendar"][k] for k in ("lba", "sectors", "destination")],
                         [0x9B94, 0x95, 0x801A0000])
        self.assertEqual([reads["room_spring"][k] for k in ("sectors", "destination")], [0xA, 0x80162000])

    def test_failed_shiori_export_keeps_opening_pack_and_records_its_own_gap(self):
        stage = self.root / "stage"
        (stage / "packs/opening_pack/reads").mkdir(parents=True)
        (stage / "packs/opening_pack/opening.json").write_text('{"fixture":true}\n')
        work = self.root / "shiori-work"
        gap = runtime._merge_opening_shiori_reads(
            stage, work, (False, "opening_shiori_reads: exporter failed (exit 1): fixture missing"))
        self.assertIn("fixture missing", gap)
        self.assertTrue((stage / "packs/opening_pack/opening.json").is_file())
        coverage = runtime._coverage({"opening_pack": "packs/opening_pack"}, {runtime.SHIORI_GAP: gap})
        self.assertIn("opening_pack", coverage["available_roles"])
        self.assertNotIn("opening_pack", coverage["unavailable_roles"])
        self.assertEqual(coverage["unavailable_roles"][runtime.SHIORI_GAP], gap)
        # A clean exit without all three reads is still a gap, not a partial merge.
        (work / "reads").mkdir(parents=True)
        (work / "reads/prologue_shiori-sequence_pack.bin").write_bytes(b"x")
        self.assertIn("missing", runtime._merge_opening_shiori_reads(stage, work, (True, "")))
        self.assertFalse(any((stage / "packs/opening_pack/reads").iterdir()))

    def test_shiori_reads_merge_into_opening_pack(self):
        stage = self.root / "stage"
        (stage / "packs/opening_pack/reads").mkdir(parents=True)
        work = self.root / "shiori-work"
        (work / "reads").mkdir(parents=True)
        for field in runtime.SHIORI_FIELDS:
            (work / "reads" / ("prologue_shiori-" + field + ".bin")).write_bytes(field.encode())
        self.assertEqual(runtime._merge_opening_shiori_reads(stage, work, (True, "")), "")
        for field in runtime.SHIORI_FIELDS:
            self.assertEqual((stage / "packs/opening_pack/reads" / ("prologue_shiori-" + field + ".bin"))
                             .read_bytes(), field.encode())

    def _synthetic_shiori_disc(self):
        """A temp project with a raw track and no .asset-cache/emu captures."""
        project = self.root / "project"
        (project / ".asset-cache").mkdir(parents=True)
        track = project / "track.bin"
        sectors = [_raw_form1_sector(i + 1) for i in range(6)]
        track.write_bytes(b"".join(sectors))
        reads = []
        for field, lba, count, fixture in (("sequence_pack", 0, 2, "expect_a.bin"),
                                           ("music_body", 2, 3, "expect_b.bin"),
                                           ("music_header", 5, 1, "expect_c.bin")):
            data = b"".join(sec[24:24 + 2048] for sec in sectors[lba:lba + count])
            reads.append((field, lba, count, 0x80000000, fixture, hashlib.sha256(data).hexdigest()))
        return project, tuple(reads)

    def _with_shiori_project(self, project, reads):
        saved = (shiori.ROOT, shiori.FIXTURES, shiori.READS)
        shiori.ROOT, shiori.FIXTURES, shiori.READS = (
            project, project / ".asset-cache/emu/shiori-landing-m2", reads)
        self.addCleanup(lambda: setattr(shiori, "ROOT", saved[0]) or setattr(shiori, "FIXTURES", saved[1])
                        or setattr(shiori, "READS", saved[2]))

    def test_shiori_export_needs_only_the_disc_when_no_emulator_captures_exist(self):
        project, reads = self._synthetic_shiori_disc()
        self._with_shiori_project(project, reads)
        self.assertFalse((project / ".asset-cache/emu").exists())
        self.assertEqual(shiori.main(["--output", ".asset-cache/out", "--track", "track.bin"]), 0)
        document = json.loads((project / ".asset-cache/out/shiori.json").read_text())
        for field, *_rest, digest in reads:
            self.assertEqual(document["reads"][field]["sha256"], digest)
            self.assertNotIn("emulator_fixture", document["reads"][field])
            self.assertTrue((project / ".asset-cache/out/reads" / ("prologue_shiori-" + field + ".bin")).is_file())
        # The developer flag still demands the captures.
        with self.assertRaises(SystemExit):
            shiori.main(["--output", ".asset-cache/out2", "--track", "track.bin", "--require-fixtures"])
        self.assertFalse((project / ".asset-cache/out2").exists())

    def test_shiori_export_checks_present_emulator_captures_and_pins(self):
        project, reads = self._synthetic_shiori_disc()
        self._with_shiori_project(project, reads)
        fixtures = project / ".asset-cache/emu/shiori-landing-m2"
        fixtures.mkdir(parents=True)
        for _field, lba, count, _dest, fixture, _digest in reads:
            (fixtures / fixture).write_bytes(b"different")
        with self.assertRaises(SystemExit):
            shiori.main(["--output", ".asset-cache/out", "--track", "track.bin"])
        bad_pin = ((reads[0][0], *reads[0][1:5], "0" * 64),) + reads[1:]
        shiori.READS = bad_pin
        import shutil
        shutil.rmtree(fixtures)
        with self.assertRaises(SystemExit):
            shiori.main(["--output", ".asset-cache/out", "--track", "track.bin"])


if __name__ == "__main__":
    unittest.main()
