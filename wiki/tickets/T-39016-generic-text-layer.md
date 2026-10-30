---
id: T-39016
title: Build a generic source-keyed text layer
status: In Progress
assignee: Claude (fan-out worker)
created: 2026-10-29
updated: 2026-10-29
links: ["[[port-pause-2026-10-29]]", "[[port-adversarial-review]]"]
---

## Goal

Provide a runtime translation layer keyed by original source location (container, offset, record id) that replaces text at load/decode time for any screen, using the approved text records and original Latin glyph rendering.

## Acceptance criteria

- [ ] Implementation follows original source with explicit rejection where unsupported; no synthetic readiness, injected state or fabricated fields.
- [ ] Docker tests pass, including negative cases; evidence limits stated.
- [ ] Independent adversarial review against CODING_STANDARDS.md with all findings resolved.

## Notes

Started in the 2026-10-29 fan-out; worked in an isolated worktree, integrated by root after review.

### 2026-10-29 implementation (fan-out worker, worktree wf_75c1998c-8be-10)

- Design and adoption guide: [[port-text-layer]]. New files: `tools/port/build_text_pack.py`, `tools/port/test_build_text_pack.py`, `port/godot/services/text_layer_store.gd`, `port/godot/presentation/native_latin_layout.gd`, `port/godot/tests/translation/run_text_layer_tests.gd` and `run_text_layer_hud.gd`.
- Changed: `services/translation_pack_service.gd` (`select_text_layer`), `services/settings_runtime.gd` (`translated_text`, `select_ui_text_pack`), `services/user_preferences_service.gd` (optional `ui_text_manifest`/`ui_text_sha256`), `presentation/hud_compositor.gd` (optional per-glyph `glyph_x` in a text run), `presentation/command_hud_presenter.gd` (`set_translated_text` for the narration row) and `app/playable_boot.gd` (two lines binding the HUD next to the image pack). The prologue text pack and the opening files are unchanged.
- The pack was built in Docker from the main checkout's read-only `translations/` and `disc/files`. Output: `.asset-cache/text-layer-out/v2/text_layer.json`, SHA-256 `c6fed34545dfe06d615886742b29753f6d7dad479e51e160f7aab49ff682edae`. The earlier `v1` (`ff75ad90...`) differs only in the hash of `inventory_decisions.json`, which concurrent translation work changed between the two builds. A rebuild from the same inputs produces the same bytes.
- Coverage: 54,059 of 55,911 approved records (96.7%) are representable with original glyphs, and no character lacks an original glyph. 3,512 physical ids were re-verified against the original bytes, and 52,150 entries are bound to an original container hash. Rejections, each with its reason: 1,018 ASCII/format literals, 617 records without English, 190 supplemental records whose markers have no source bytes, and 27 records that hold only markers.
- Consumer: command HUD narration. Both executable strings resolve by location (`SLPM_86.053` + offset + source hash + executable hash), and the English row is laid out inside the original 14-cell extent. Clearing the layer restores the exact original frame. A rejected or unopened layer keeps the original and gives the reason.
- Tests (Docker): `python3 -m unittest tools.port.test_build_text_pack` passes 7 tests. `run_text_layer_tests.gd` passes with no failures. `run_text_layer_hud.gd` passes with no failures (installed-runtime-v12 HUD manifest `c19f4306...` and font, pack v2). `tests/run_tests.gd` passes 5 suites, and `run_modern_settings.gd` and `run_language_packs.gd` pass. `run_settings_entry_window.gd`, `run_prologue_text_store.gd`, `tests/hud/run_fresh_content_tests.gd`, `run_hud_seasons_cursor_popup_tests.gd` and `run_date_appointment_tests.gd` fail the same way with these changes reverted, so the failures predate this work.
- Limits: the HUD test uses caller-supplied rule and context state and is not an emulator capture or a played route. English proportional layout has no emulator reference. The pack does not prove where a record is drawn; each consumer must prove that. The settings menu has no picker for the text layer yet; `select_ui_text_pack(path)` is the API. Root still has to add [[port-text-layer]] to `wiki/index.md` and write the log entry.

### 2026-10-29 review fixes (fan-out worker, worktree wf_75c1998c-8be-25)

Three review findings, each re-verified before the fix.

- Single-byte renderer controls (high, confirmed). `sndisp` and `sndi` (`src/main/menu_speaker_girl.c`) treat ' ' at even byte phase as a line break and 'P'/'G' as name codes. Pack v2 held 800 physical entries with untokenized single-byte source units, for example `NAME_ENT.EXN:0001b3b8` ("I'm P."). Six entries also held a literal `%s`/`%d`. The builder now walks each physical source as Shift-JIS units. Every single-byte unit must lie inside a protected token's bytes, at the token's recorded `offset` when it has one, and every token must occur in the source. Failures are rejected as `source_single_byte_control_untokenized` or `protected_token_not_in_source`. A printf directive outside a marker is rejected as `format_directive_untokenized`. A `renderer_tokenization_unavailable` review status is rejected too. The one exception is a record whose renderer the builder's `RENDERER_PROVEN` table cites from the source, and the other checks still run on it. That table holds one id: `D_800AFB74`, the full-width "(" that `sndi` prepends for speaker style 0x1F, which the command HUD narration draws. Rejecting that flag for it would have removed the HUD consumer.
- Source-hash/length guard untested (medium, confirmed). A scratch mutant with `if false:` in `lookup_source` passed the old suite. `run_text_layer_tests.gd` now opens entries that share the 12-digit id prefix but differ in the full `raw_sha256` or in `raw_length`, and asserts the exact rejection. The same mutant now fails both checks.
- Docstring overstated (low, confirmed). It now describes only the NUL-terminated `text_binary.py` scheme and says that `text_source.py` extents without a NUL, and NUL-appended aliases, are rejected as `source_bytes_mismatch`. It also documents the three deliberate substitutions: ASCII '"' to 0x8168, ASCII "'" to 0x8166 and the en dash to 0x817C. JIS X 0208 has no straight quotes or en dash; CP932 places the full-width straight quotes only in the NEC extension. `coverage.json` counts each substitution.
- Pack v3: `.asset-cache/text-layer-out/v3/text_layer.json`, SHA-256 `2f2db8362a50623b7192a05975662d0510c278b1db36e67ac1d479aec305b9a1`, built from the same input hashes as v2. 51,631 of 55,911 records are representable, 1,087 of them physical. Rejections: 2,430 tokenization unavailable, 1,018 ASCII/format literals, 617 without English, 190 supplemental markers without bytes, 13 untokenized single-byte controls, 9 marker-only and 3 printf directives. Substitutions: 239 straight double quotes and 28,064 apostrophes. A probe of v3 finds no physical entry with an uncovered single-byte unit or a printf directive.
- Tests (Docker): the builder unittest passes 11 tests. `run_text_layer_tests.gd` reports no failures. `run_text_layer_hud.gd` reports no failures with pack v3, HUD manifest `c19f4306...` and the installed-runtime-v12 font. `tests/run_tests.gd` (5 suites), `tests/services/run_modern_settings.gd` and `tests/services/run_language_packs.gd` pass.
- Limit: logical ids (`talk:`, `c:` and the rest) carry no source bytes, so the single-byte walk cannot run on them. They rely on their recorded tokens and the printf check. No logical record in these inputs has the tokenization flag.

### 2026-10-29 integration evidence (main working tree)

- The integrated tree was checked against the reviewed pack v3 (`.asset-cache/text-layer-out/v3/text_layer.json`, SHA-256 `2f2db836...`). `run_text_layer_hud.gd` gave `Text layer HUD failures: []` with 51,631 entries, using the installed-runtime-v12 HUD manifest `c19f4306...` and its font. `run_text_layer_tests.gd` gave `Text layer failures: []`. The earlier integration run used the pre-review pack v2 (54,059 entries), so it is not evidence for the reviewed builder.
- Open: `text_layer_store.gd` still opens v2 without complaint, although v2 holds records the reviewed builder rejects. Nothing records a builder or format version, so the runtime cannot refuse a pre-fix pack. Retire v2 or add a version field before the layer ships.

## Comments
