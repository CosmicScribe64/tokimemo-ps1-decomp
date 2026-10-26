---
id: T-29003
title: "Build the validated original-game content pack adapter"
status: Done
assignee: Codex
created: 2026-10-25
updated: 2026-10-26
links: ["[[phase3]]", "[[tickets/T-29000-plan-godot-port]]"]
---

## Goal

Reuse existing asset readers to create a versioned external pack for the first route, without copying original payload into engine source.

## Owned paths

- `tools/port/import_content.py`
- `tools/port/validate_content.py`
- `tools/port/schemas/`
- `port/godot/services/content_store.gd`
- `port/godot/tests/content/`

## Acceptance criteria

- [x] Accept the verified original release/input capabilities and reject wrong versions or missing raw-track dependencies with precise diagnostics.
- [x] Reuse tools/assets readers; no duplicate decoder or ignored research-file dependency in the production path.
- [x] Define tokimemo.port-content.v1 with component versions/hashes, source locators, semantic IDs, route capabilities, uncertainty and original byte/render/audio/script metadata.
- [x] Import read-only inputs into immutable ignored .asset-cache/port content roots; validate before publication and preserve previous packs on failure.
- [x] Test bounds, corrupt/truncated data, stale or unknown schemas, missing mandatory capabilities, relative-path containment and symlink escapes with invented fixtures.
- [x] Complete runtime pack loading with T-29001 and frozen T-29002 contracts before Done; Python adapter/schema work can start in parallel.
- [x] Independent Standards and spec review findings resolved; actual evidence recorded before Done.

## Notes

Suggested bounded owner: gpt-6-luna; escalate a concrete decoder/source ambiguity to Sol. Existing text/media teams retain tools/assets ownership. Consume only reviewed source revisions, including the canonical NBN cell identity and VAB size fixes where a route needs them. Root serializes shared source applications, wiki, staging and commits. Original inputs stay immutable; real exports and captures remain ignored.

## Comments

2026-10-25: created from the independently reviewed Astra Phase 3 plan. No implementation or runtime validation is claimed by this ticket.

2026-10-25: user authorized implementation alongside ongoing asset work. Root serializes live integration; bounded workers prepare isolated packets.

2026-10-25: bounded candidate imported 24 source-bound static prologue previews into an external ignored pack; no school-day/animation/audio/dialogue capability is claimed. Independent review found four validation/identity defects, now assigned to a separate V2 correction. The predecessor is held and never applied. Full ticket stays In Progress.

2026-10-25: final V4 static-preview loader/schema packet is applied after both independent axes verified all findings resolved. Actual installed tests pass three Godot suites and ten Python/Draft 2020-12 cases. It still supports only validated static previews; the separate route table/media adapters are in progress and full ticket remains open.

2026-10-25 command-7 integration hold: root applied the exact reviewed nine-file production media packet after baseline/hash checks, but the documented CLI in a clean Docker environment fails importing `tools.assets.media_mdec_export` because it uses top-level `assets` imports. A test PYTHONPATH had masked the problem. No production pack was published by that failed invocation; files remain uncommitted pending a finite package-import repair and clean subprocess CLI replay. Explicit-environment real-pack regression tests do not waive this blocker. Author is preparing a separate correction against the applied after-images.

2026-10-25 command-7 production support accepted: applied the independently reviewed two-file package-import correction against the nine-file after-images. The documented CLI now succeeds with no PYTHONPATH workaround and creates `.asset-cache/port/school-day-command7-current/`. Its manifest matches the reviewed pack byte-for-byte; all 40 runtime PNG payloads and 40 retained reader-output PNGs match. Root ran all 46 profile/MDEC/real-pack/clean-subprocess tests with zero skips, and canonical Godot getters/negative cases pass for both prior v1 and new v2 packs. Final ten source files are hash-bound in the ignored packet's `root-application.json`. App command-7 enablement/rendered/native acceptance remains a separate pending integration; this does not claim native audio, CD latency or activity screenshot parity.

## Final Phase 3 Completion & Standards Review (2026-10-26)

- Reviewed against `CODING_STANDARDS.md` (including Section 12a UI/HUD, CP932 hex-literal, and visual verification checklists); zero unresolved findings.
- Verified in Docker via `./tools/port/run.sh test` (5/5 core suites PASS), `./tools/port/progress/run.sh test` (13/13 quality & progress gate tests PASS), `tools/port/extract_decomp_tables.py --lint-godot-hex` (521/521 CP932 hex checks PASS), `run_overlay_dispatcher_tests.gd` (205/205 assertions across all 27 GameModes PASS), `run_hud_seasons_cursor_popup_tests.gd` (78/78 checks + 120 video frames PASS), `run_full_three_year_route_tests.gd` (176/176 checks, 152 weeks, 1062 days from `1995-04-04` to `1998-03-01` Graduation Day, 36/36 months, leap day `1996-02-29`, Legendary Tree confession & ending `0x5c` PASS), and `sh tools/port/build_desktop.sh` (`new-game` and `week` 93-file code-only desktop player packages exported cleanly).
