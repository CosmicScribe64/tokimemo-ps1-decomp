---
id: T-39003
title: Tighten repository prose and comments
status: In Progress
assignee: Codex deslop agent
created: 2026-10-29
updated: 2026-10-29
links: ["[[status]]"]
---

## Goal
Apply the requested dotfiles deslop skill across repository prose, documentation, comments and docstrings, preserving claims, evidence, stable anchors and contracts. The skill forbids executable-code changes.

## Acceptance criteria
- [ ] Inventory tracked prose and comment-bearing source categories; record protected/generated/history exclusions.
- [ ] Inspect skill scanner findings and make justified edits in reviewed batches.
- [ ] Check diffs, links and anchors; validate comments/docstrings without changing behavior.
- [ ] Obtain code review against CODING_STANDARDS.md before Done.

## Notes
Source: https://github.com/CosmicScribe64/dotfiles/blob/main/skills/deslop/SKILL.md (read 2026-10-29). Whole-repository scope is broader than the skill's default diff-only fallback. Root owns active port-restoration files; this worker will not edit them or shared wiki index/status/log/board. Append-only history, raw originals, generated files, licenses and mandated text remain protected.

## Inventory and first batch (2026-10-29)

The skill's scanner reviewed 2,302 supported files across active root docs, wiki, tools, C sources, headers and port prose (1,907,316 words; 18,307 pattern leads). These are search leads, not confirmed defects. Many flagged terms are literal technical language (`unmatched`, `fixed`, `key`, compiler arrows). A separate pass extracted full-line comments from all 295 tracked GDScript files because the scanner does not recognize `.gd`; it reported 855 leads. Inline GDScript comments are not covered by that extraction.

Tracked inventory includes 32,564 files under drafts/, 1,214 wiki files, 527 tools, 438 C sources, 314 port files, 291 translation files, 261 configuration files and 126 headers. Raw inputs, licenses, generated notices, identifiers, commands, translations, append-only logs and historical evidence are protected. Preserved drafts were inventoried but not rewritten or exhaustively scanned. Configuration and translation documentation received a separate read-only scan. Active source comments need a source-pin dependency check before any edit, because whole-file hashes protect export evidence.

First batch edits:
- tools/translation_gallery/README.md: clarified local-only scope, directory permissions and listener addresses; repaired missing spaces around sizes; tightened prose while preserving review/API/security contracts. Reader: maintainer running the image-review server. This guide supplies executable setup and state-handling instructions absent from the code interface alone.
- wiki/cloud-matching.md: consolidated repeated Docker Hub rate-limit advice, preserving mirror/backoff/--pull=false options, all ticket references, the measured build time and the prohibition on committing the injected-CA Dockerfile. Reader: contributor compiling a matching packet in the described cloud container. The environment-specific TLS workaround is not duplicated by normal local setup docs.

Checks: git diff --check passes. Docker checks confirm unchanged headings, link targets and ticket references in both edited documents. Post-edit skill scanner rerun completed. No executable code, matching C, source-pinned comments, game data or existing log entries changed. No tests needed for this prose-only batch. Root review remains required; entire-repository cleanup is not complete.

## Findings requiring factual reconciliation

Do not fix these as style-only edits:
- CONTRIBUTING.md Code style still describes renames through splat symbol files, while current standards require tools/rename.py. Translations still says translation work is planned.
- ROADMAP.md Phase 3 describes older blockers as current; Phase 4 heading says in progress despite expansion being held; Phase 5 says image ideas are not yet created despite versioned artwork.
- wiki/overview.md retains initial partial-match/all-INCLUDE_ASM status.
- wiki/phase4.md baseline calls Phase 3 complete; current fresh-install evidence does not establish that claim.

Keep the ticket In Progress while reviewing remaining active prose and source-pin-safe comment candidates. Never report the scanner totals as completed editorial review.

## Second prose batch

- wiki/assets-input.md: split dense API and format-limit paragraphs into caller actions, entry metadata, unsupported cases and hash/bounds rules. All numeric limits, format exclusions, historical validation results and lazy-reader obligations remain. Reader: exporter author choosing and validating original inputs. The page explains provenance and format limits beyond the public method names.
- wiki/emulator.md: rewrote the introduction and setup list into direct instructions, retaining engine/BIOS revisions, license, architecture, measured sizes/timings, map fallback behavior, container resources and game-data restrictions. Reader: contributor inspecting original runtime behavior. Historical spike results and every command block remain unchanged.

Docker checks confirm unchanged headings, links and command blocks in both pages. Skill scanner rerun and git diff --check pass. Root review pending. Reviewed without edits: CONTRIBUTING.md and ROADMAP.md (factual issues sent to root), tools/port/native_audio/README.md (already concise), wiki/SCHEMA.md (required rules and notation), tools/ido_eval_cases/ugen_copy_state/README.md (technical notation and measured cases), wiki/ci.md (historical/factual claims need separate reconciliation), wiki/port-readiness-review.md (historical evidence and current-claim ambiguity). Reading a document without edits does not mark its factual claims verified.

## Third prose batch and review coverage

wiki/port-testing.md now states trace binding and preflight responsibilities directly, repairs missing spaces in source thresholds/IDs, and separates persistence requirements from their rationale. All acceptance obligations, literal commands, source thresholds, historical results, headings and links remain. Reader: reviewer qualifying a port scenario or release. The page defines evidence requirements that executable tests alone do not convey. Docker checks compare headings, links, fenced commands and inline literals; all pass. Skill scanner rerun and git diff --check pass.

The separate .github/config/game/versions/translations scan covered 343 files, 1,369,791 words and 9,015 pattern leads. Most hits are literal symbols or translated content. No translated game text was edited. Reviewed without changes: game/README.md, versions/README.md, translations/en/graphics/artwork/README.md (direct instructions and necessary provenance contracts); wiki/phase4.md (factual baseline contradictions require separate reconciliation). The ISO statement in game/README.md concerns the executable build, not full port media recovery; avoid extending it to port setup.

Pending coverage is substantial: scanner leads are not individually reviewed across the entire repository, preserved drafts were inventoried only, GDScript inline comments were not extracted, and source-pinned comments/docstrings remain deferred until dependency analysis. Changed standalone prose is limited to five files. No claim of completed repository-wide cleanup or factual audit is made.

## Fourth prose batch

wiki/toolchain.md now presents pinned tool versions and compiler locations as reference tables. Every version, path, environment variable, architecture restriction and ticket reference remains. wiki/build-system.md separates the first setup step into input selection, automatic extraction, error handling and rebuild triggers. No build command or behavior changed. Readers are contributors installing or diagnosing the Docker toolchain. These sections provide setup and dependency reference information beyond command help. Docker comparisons preserve headings, links, fenced commands and the complete inline-literal set; post-edit scanner reruns and git diff --check pass.

## Active standalone document dispositions

This is a disposition inventory, not a claim that every scanner hit was manually reviewed. `Edited` means a bounded prose-only revision with checks recorded above. `No edit` means the inspected prose or technical reference had no confirmed stylistic defect worth changing. `Factual review` identifies a concrete issue that cannot be repaired as style alone. `Preserve record` protects dated evidence, measured results, accepted decisions or literal technical records; those pages have not received an exhaustive sentence-by-sentence rewrite. `Root-owned` avoids concurrent edits.

| Document | Disposition and reason |
| --- | --- |
| README.md | Root-owned; placeholder-cleanup/translation-future and no-assets claims conflict with later source/artwork state. |
| CONTRIBUTING.md | Factual review; rename instructions predate mandatory rename.py and translation phase is stale. |
| ROADMAP.md | Factual review; old Phase 3 blockers/current Phase 4 status and uncreated-artwork claim need reconciliation. |
| GLOSSARY.md | No edit to naming reference; names, stems and game terms are contractual, not prose substitutions. |
| AGENTS.md | Preserve mandated project instructions. |
| CODING_STANDARDS.md | Preserve mandated matching, naming and review contracts. |
| src/main/README.md | No edit; object/prefix mapping tables and ranges are technical reference. |
| game/README.md | No edit; direct executable-build instructions. ISO support must not be generalized to port media recovery. |
| versions/README.md | No edit; format distinctions and supported-release instructions are necessary. |
| tools/translation_gallery/README.md | Edited; local review setup and state handling. |
| tools/ido_eval_cases/ugen_copy_state/README.md | No edit; compact measured-case notation and reproduction commands are necessary. |
| tools/port/native_audio/README.md | No edit; concise build constraints and bounded qualification. |
| tools/port/native_audio/PROVENANCE.md | Preserve reviewed compiler/binding/build evidence and qualification limits. |
| tools/port/native_audio/GODOT_CPP_LICENSE.md | Preserve license text. |
| tools/port/SETUP.md | Root-owned active fresh-clone work. |
| translations/en/README.md, CONTRIBUTING.md, INDEX.md | Generated documentation from tools/translation_tree.py; direct prose edits would diverge from regeneration. Generator changes are outside the skill's executable-code boundary. |
| translations/en/graphics/artwork/README.md | No edit; dated backup scope and approval/recovery contracts are necessary. |
| wiki/SCHEMA.md | No edit; required layout, operations and append-only rules. |
| wiki/acceleration-audit.md | Preserve record; measured audit at base 7a65b3d8 and old worklists. |
| wiki/as1-scheduler-rule.md | No edit to inspected technical recipe; measured compiler rules, instruction notation and evidence links are necessary. |
| wiki/as1-scheduling.md | Preserve record; eight historical order-only candidates and compiler-option experiments. |
| wiki/asset-export.md | Factual review; the dated scope defers translation and replacement artwork while later work exists. Preserve source-evidence continuation. |
| wiki/assets-input.md | Edited; caller API and format/hash/bounds obligations. |
| wiki/assets-media.md | Preserve record; dated export/provenance checkpoints and unknown coverage, including unavailable local witnesses. |
| wiki/assets-text.md | Preserve record; dated translator handoffs, corpus counts and unknown extents require source-aware reconciliation. |
| wiki/broad-coverage-results.md | Preserve record; exact baseline, target ledger and matching result counts. |
| wiki/build-step-gaps.md | Factual review; remaining-function framing predates completed matching. Compiler gap measurements remain evidence. |
| wiki/build-system.md | Edited setup span; historical counts and implementation records retained. |
| wiki/ci.md | Factual review; says workflow has never run and secrets remain unset, with old function census. No style-only status correction. |
| wiki/cloud-matching.md | Edited repeated Docker recovery advice. |
| wiki/compiler-mismatch-research.md | Preserve record; source-attributed compiler/licensing research. |
| wiki/data-types.md | Preserve record; conversions and retired-name mappings are evidence, with explicit current-header note. |
| wiki/decompile-workflow.md | Preserve measured recipe history and code examples; scanner leads outside inspected spans remain unreviewed. Renamed source references need coordinated technical validation. |
| wiki/disc-layout.md | Factual review; O.BIN table says not code while later ECOFF analysis identifies overlay code. |
| wiki/emulator.md | Edited introduction/setup; exact runtime probes and historical results retained. |
| wiki/ending-allocator-trace.md | Preserve record; external diagnostic binary/build controls and measured allocator results. |
| wiki/executable.md | Factual review; segment prose retains old 28-file source layout. Preserve binary offsets and hypotheses. |
| wiki/exhaustive-drafts.md | Preserve record; frozen 48362009 census and dated compilation/matching checkpoints. |
| wiki/game-state.md | No edit to inspected reference; current owner paths and historical field labels are deliberately distinguished. |
| wiki/gcc-objects.md | Preserve record; SDK attribution, assembler measurements and per-object matching proof. |
| wiki/ido-52-evaluation.md | Preserve record; private compiler provenance and experiment results. |
| wiki/index.md | Root-owned navigation hub. |
| wiki/kanban.md | Root-owned ticket board. |
| wiki/keep-lines.md | Factual review; proposed-rule/lead-decision state coexists with later applied proofs. Preserve proof data. |
| wiki/log.md | Protected append-only history. |
| wiki/matching-breakthroughs.md | Preserve record; measured matching investigations and baseline-specific conclusions. |
| wiki/matching-notes.md | Protected append-only history. |
| wiki/obin.md | Preserve record; binary-format facts, measured mapping and inference limits. |
| wiki/original-compiler.md | Preserve record; source attribution, ranked hypotheses and historical experiments. |
| wiki/overlays.md | Factual review; all-INCLUDE_ASM/partially-migrated source state and old loader unknowns need source reconciliation. |
| wiki/overview.md | Factual review; partial-match and all-INCLUDE_ASM descriptions are obsolete. |
| wiki/phase2-naming.md | Preserve accepted user decisions, naming contracts and reviewed path maps. |
| wiki/phase2.md | Preserve completed phase's baseline, work order and acceptance/publication history. |
| wiki/phase3.md | Factual review; readiness correction and later completion claims require current-route evidence reconciliation. |
| wiki/phase4.md | Factual review; baseline asserts Phase 3 completion despite current fresh-install failures. Retain proposed UI contracts. |
| wiki/plan-remaining.md | Factual review; old matching dispatch/active ownership predates completed matching. |
| wiki/playbook.md | No edit to inspected matching recipes; source spellings, compiler instructions and numbered tactics are technical notation. Uninspected historical spans remain outside manual review. |
| wiki/port-checkpoint.md | Preserve dated pause/checkpoint and superseded claims; missing local witness paths cannot be silently replaced. |
| wiki/port-finish-plan.md | Factual review; older baseline and later G1-G4 acceptance statements require current evidence. Gate obligations remain unchanged. |
| wiki/port-manager-handoff.md | Factual review; directs resuming absent local candidates and stale baseline, requiring root's current recovery plan. |
| wiki/port-readiness-review.md | Preserve historical baseline/review and resolution records; current claims need factual reconciliation. |
| wiki/port-testing.md | Edited clarity and numeric spacing; exact acceptance obligations retained. |
| wiki/psyq-sdk.md | Preserve record; signature evidence, confidence levels and unresolved SDK-version conflict. |
| wiki/ref-english-patch.md | Preserve source-attributed facts-only reference and copyright boundary. |
| wiki/register-allocation-wave7.md | Preserve dated experiment inventory, limits and validation. |
| wiki/register-cheatsheet.md | No edit to inspected diagnostic recipes; instruction/operator notation is literal. Uninspected historical spans remain outside manual review. |
| wiki/rename-tool.md | No edit; procedural plan/apply/check/replay contracts and examples are necessary. Trial records remain historical. |
| wiki/source-files.md | No edit to object-map reference; current paths and historical boundary evidence are distinguished. |
| wiki/speedup-research-2026-10-16.md | Preserve dated research and attribution; comparisons cannot be refreshed through style edits. |
| wiki/status.md | Root-owned live status. |
| wiki/toolchain.md | Edited pinned-version reference; compiler measurements and pass explanations retained. |
| wiki/tooling.md | No edit; compact tool/ticket/navigation catalog, with necessary command notation. |
| wiki/uopt-register-allocation.md | Preserve measured allocator diagnostics, score tables and compiler hypotheses. |
| wiki/versions.md | Preserve release hashes, exact catalog and measured differences; archive counts are historical. |
| wiki/widescreen-settings.md | Preserve accepted research, source geometry and platform citations; proposed policies retain their qualifiers. |

Source comments/docstrings have scanner-only coverage outside the inspected examples. Whole-file source hashes can include comments, so these need dependency-aware review before changes. No claim is made that excluded source, generated docs, preserved records or unreviewed scanner leads have been deslopped.

Root reviewed the final toolchain/build-system edits as well as the initial five guides. No lost commands, requirements or changed technical claims identified in these diffs. The overall ticket remains In Progress because protected/source-pinned and unreviewed categories are explicitly outside this completed active-document pass. Root separately corrected README source-phase and artwork-inclusion claims using current status and the committed artwork README; those are factual maintenance, not changes delegated to the prose skill.

## Fifth batch: Godot coverage and presenter comments

Reviewed the headers and relevant dispatch/resource/render implementations in seven previously clean GDScript files. Edited only full-line comments in `app/overlay_dispatcher.gd`, `presentation/vn_scene_presenter.gd`, `presentation/telephone_overlay_presenter.gd`, `presentation/encounters_and_ending_presenter.gd`, `presentation/event_cg_scenes_presenter.gd`, `services/ordinary_command_media_adapter.gd`, and `tests/app/run_overlay_dispatcher_tests.gd` (all under `port/godot/`).

- Replaced the dispatcher's blanket all-26-overlays/authentic-frame claim with its actual catalog dispatch scope. `handles_mode` checks membership; `open_resources` loads telephone/date preview presenters. Catalog membership is not route acceptance.
- Removed blanket authenticity/compliance claims from presenter headers, retaining screen lists, dimensions, source references and rendering constraints. Shared primitives need scene-specific fidelity verification.
- Documented the event-CG presenter's existing park/Shiori fallback images and static Ferris-wheel rendering (`_draw_karaoke_lounge_cg`, `_draw_ferris_wheel_viewport`, `_draw_park_entrance_cg`). Bounding-box violations are collected, not prevented by a header declaration.
- Kept the dispatch test's 27-mode, input, heroine, framebuffer and bounding-box scope; removed its unsupported assertion that those checks span all original overlays or prove route fidelity.
- Simplified the media adapter header without changing its command/store mapping.

Dependency check: searches for these seven basenames in `tools/` and `config/` found no explicit per-file source pins. `tools/port/readiness.py` hashes the runtime tree: this comment-only change changes that identity, so prior runtime-bound receipts cannot certify the revised tree. No receipts or pins were rewritten. Parent was informed before edits. Active app/integration, settings, translation and first-arrival/prologue-media files were excluded as assigned.

Validation: Docker Python comparison against saved Git HEAD versions passed for all seven: every non-full-line-comment line is identical. `git diff --check` passed. The reused recovery image lacks Git, so HEAD snapshots were exported with host Git and compared inside Docker; no dependencies were installed. These are prose-only changes; no gameplay pass is claimed. Root review pending.

Coverage remains bounded: this batch manually reviews the seven headers and the implementation needed to substantiate their wording, not every comment in those files or all 295 scanned GDScript files. Remaining source comments/docstrings, historical and generated categories retain the earlier dispositions. Ticket remains In Progress.

## Sixth batch: tracked GDScript comment inventory

Inventory baseline: 295 tracked GDScript files, with a recorded disposition for each below. Newly created/untracked translation, settings and integration scripts remain owned by their implementers and are not counted. This is an English comment/style review, not a re-verification of every source-parity claim. Full-line technical contracts, formulas, source sites, game labels and literal examples were preserved unless a concrete implementation discrepancy is documented below.

Inline inventory found 1,276 comment candidates. English prose and short technical annotations were inspected; Japanese/game-string glosses were preserved. The simple quote-aware inventory is a review aid, not a GDScript parser. No inline comments or executable lines were changed. Active opening/settings/translation files retain their explicit hold even when a scan displayed their comments.

New changes beyond the first seven GDScript files: 25 files (32 total GDScript files edited). Most changes remove unsupported blanket coverage/compliance claims or correct spacing/punctuation. The existing source-specific references and acceptance obligations remain.

- `talk_player.gd`: `_allow_voiced` and `_prepare_content` reject voiced rows by default; replaced the ambiguous “100% fail-closed when unvoiced” sentence with that condition.

- `system_overlays.gd`: settings live in object fields and `get_settings()`; persistence is a caller responsibility, not implemented here.

- `event_cg_scenes_overlay.gd`: `trigger_ride_movie` sets stage/dialogue/LBA and returns metadata; it does not stream MDEC video.

- Calendar presenter: `run_calendar_phone_tests.gd` permits per-tile MAE up to 20 and skips absent reference files; its header now describes comparison rather than bit-exact equality. Date, school-event and subgame presenters identify existing image/shape fallbacks rather than promising authentic scenes.

- Seasonal test: `_test_seasonal_content_store` samples six backgrounds and four SD sets. Kept pack totals 33/44, removed the claim that this test extracts/renders every asset.

- Three-year tests: direct capability assignment, fixture completion records and/or `FastRouteState.fast_stepping` mean model simulations, not installed-player acceptance. Kept date spans, route totals, mode counts and tested save/ending operations.

- VN presenter: corrected stale Frame 0xF0 header to Frame 1, which `render_vn_dialogue` actually requests; coordinate and shadow contracts unchanged.

Checks: all 32 edited GDScript files passed Docker comparison against Git HEAD after removing full-line comments: every remaining line is identical. `git diff --check` passed. Re-ran the invoked skill scanner on all 295 full-line-comment mirrors; remaining pattern hits are leads, not failures or proof of completion. Tool/config reference audit found only `test_extract_decomp_tables.py` references to the school-event presenter, used to lint inline CP932 pairs, which are unchanged. No explicit per-file pin was found for edited files. Whole-runtime hashes still change, so old runtime receipts remain stale and untouched. No runtime gameplay test result is claimed.

Counts: 176 Reviewed: technical comments/source references retained; 23 Hold: active integration/other-owner diff; 45 Hold: active opening/settings/translation scope; 32 Edited: comments only; review pending; 19 No full-line prose; inline annotations retained.

### File dispositions

Paths below are relative to `port/godot/`. Grouping keeps the inventory readable; every filename is listed once.


**Reviewed: technical comments/source references retained**

- `app/`: `boot_router.gd`, `boundary_save_controls.gd`, `first_launch_import_flow.gd`, `first_launch_setup.gd`, `installed_launch.gd`.

- `./`: `bgm_backend.gd`, `bgm_live_context.gd`, `bgm_sink.gd`, `bgm_source_pack.gd`, `shell.gd`.

- `core/`: `game_session.gd`, `game_state.gd`, `input_frame.gd`, `mode_router.gd`, `native_clock.gd`, `native_math.gd`, `native_rng.gd`, `native_route_registry.gd`.

- `dialogue/`: `text_tokens.gd`.

- `flow/`: `command_screen.gd`, `entry_runtime_context.gd`, `holiday_screen.gd`, `new_game_command_reload.gd`, `new_game_entry.gd`, `new_game_state_adapter.gd`.

- `overlays/`: `date_overlay.gd`, `school_events_overlay.gd`, `shooter_collision_rules.gd`, `shooter_combat_state.gd`, `shooter_effects_rules.gd`, `shooter_enemy_particles.gd`, `shooter_enemy_pool_rules.gd`, `shooter_enemy_spawn_rules.gd`, `shooter_lifecycle_rules.gd`, `shooter_player_rules.gd`, `shooter_primary_rules.gd`, `shooter_sound_rules.gd`, `shooter_stage3_enemy_rules.gd`, `shooter_stage3_rules.gd`, `telephone_overlay.gd`.

- `presentation/`: `date_appointment_presenter.gd`, `holiday_hud_adapter.gd`, `hud_compositor.gd`, `native_canvas.gd`, `native_glyphs.gd`, `native_parts.gd`, `native_sequence.gd`, `school_day_actor_service.gd`.

- `rules/`: `calendar.gd`, `calendar_phone_screens.gd`, `day_end.gd`, `day_event_modes.gd`, `holiday.gd`, `holiday_appointment.gd`, `new_game.gd`, `new_game_route_state.gd`, `scheduler.gd`, `school_day.gd`, `school_day_state.gd`, `week_end.gd`.

- `scenes/`: `holiday_controller.gd`, `school_day_controller.gd`, `shooter_combat_controller.gd`, `shooter_stage3_controller.gd`.

- `services/`: `club_command_content_store.gd`, `content_store.gd`, `external_content_path.gd`, `holiday_scene_adapter.gd`, `installed_pack_store.gd`, `month_start_audio_bridge.gd`, `month_start_bank_request.gd`, `native_bank_loader.gd`, `native_hud_season.gd`, `native_new_game_import.gd`, `native_vab_memory.gd`, `new_game_load_diagnostics.gd`, `original_game_import_backend.gd`, `playable_save_adapter.gd`, `save_service.gd`, `school_day_media_store.gd`, `seasonal_command_content_store.gd`, `shared_month_audio_service.gd`, `shooter_combat_tables.gd`, `shooter_enemy_tables.gd`, `weekday_56_content_store.gd`.

- `tests/app/`: `run_command7_app_tests.gd`, `run_command7_preflight_tests.gd`, `run_focus_tests.gd`, `run_live_desktop_full_game_tests.gd`, `run_multi_release_import_tests.gd`, `run_playable_schedule.gd`.

- `tests/core/`: `run_core_tests.gd`.

- `tests/fixtures/`: `fake_import_backend.gd`, `synthetic_install_store.gd`.

- `tests/holiday/`: `run_calendar_phone_tests.gd`, `run_continuous_scene_tests.gd`, `run_date_appointment_tests.gd`, `run_exact_activity_media_tests.gd`, `run_holiday_club_icon.gd`, `run_holiday_date_bomb_stress_tests.gd`, `run_holiday_live_loop_tests.gd`, `run_holiday_result1_hud.gd`, `run_holiday_tests.gd`, `run_native_holiday.gd`, `run_native_week2_core_trace.gd`, `run_real_week2_services.gd`, `run_repeated_sunday_services.gd`, `run_scene_adapter_tests.gd`, `run_sunday_app_integration_tests.gd`.

- `tests/hud/`: `capture_app.gd`, `checkpoint_smoke.gd`, `run_app_hud_tests.gd`, `run_external_path_tests.gd`, `run_hud_seasons_cursor_popup_tests.gd`, `run_hud_tests.gd`, `run_slash_alias_tests.gd`.

- `tests/month_start/`: `run_calendar_source.gd`, `run_june_full_quarter_route.gd`, `run_may28_to_june_weekday.gd`, `run_month_start.gd`, `run_month_start_app.gd`, `run_native_integration.gd`, `run_quarter.gd`.

- `tests/opening/`: `run_authentic_audio_playback_gate.gd`.

- `tests/overlays/`: `run_date_overlay_tests.gd`, `run_emulator_vs_godot_capture_producer.gd`, `run_encounters_and_ending_tests.gd`, `run_live_overlay_graduation_gate.gd`, `run_reachable_families_g3_gate.gd`, `run_school_events_overlay_tests.gd`, `run_subgame_overlays_tests.gd`, `run_system_overlays_tests.gd`, `run_telephone_overlay_tests.gd`.

- `tests/presentation/`: `run_cursor_actor_and_style_gate.gd`.

- `tests/render/`: `media_witness_preview.gd`, `run_native_presentation_tests.gd`.

- `tests/rules/`: `run_rule_tests.gd`.

- `tests/`: `run_audio_vab_lifecycle.gd`, `run_checker_render_diff.gd`, `run_club_content_store.gd`, `run_cold_no_card_checker.gd`, `run_entry_consumer_lifetime.gd`, `run_entry_context.gd`, `run_external_content_path.gd`, `run_import_bounds.gd`, `run_installed_entry_keyboard.gd`, `run_installed_first_visible.gd`, `run_installed_launch_tests.gd`, `run_media_location_tests.gd`, `run_native_bank_loader.gd`, `run_native_scheduler_modes.gd`, `run_new_game_entry.gd`, `run_runtime_snapshot_differential.gd`, `run_shooter_collision_tests.gd`, `run_weekday_56_content_tests.gd`, `shooter_fixtures.gd`, `smoke_new_game_entry.gd`.

- `tests/services/`: `run_background_boundary_save.gd`, `run_command_media_save.gd`, `run_holiday_initial_capture_regression.gd`, `run_keyboard_controls.gd`, `run_playable_save.gd`, `run_save_controls.gd`, `run_save_notice_ui.gd`, `run_save_service.gd`, `run_sunday_save_boundary.gd`.

- `tests/subgames/`: `run_native_shooter_combat.gd`, `run_shooter_controller_tests.gd`, `run_shooter_enemy_tests.gd`, `run_shooter_stage3_tests.gd`.

- `tests/week/`: `run_joined_week.gd`, `run_rest_week_save.gd`, `run_rest_week_tests.gd`, `run_week_tests.gd`.

- `ui/`: `command_hud.gd`, `native_hud.gd`.


**Hold: active integration/other-owner diff**

- `app/`: `first_arrival_boot.gd`, `new_game_entry_boot.gd`, `playable_boot.gd`.

- `flow/`: `prologue_arrival.gd`.

- `presentation/`: `command_hud_presenter.gd`, `dialogue_textbox_presenter.gd`, `first_arrival_scene_presenter.gd`, `live_hud_adapter.gd`, `prologue_hud_source_provider.gd`, `prologue_text_presenter.gd`.

- `services/`: `audio_service.gd`, `first_arrival_board_store.gd`, `first_arrival_media_bridge.gd`, `installed_launch_profile.gd`, `original_voice_playback.gd`, `spring_school_day_content_store.gd`.

- `tests/`: `run_connected_opening_route.gd`, `run_first_arrival_board_load.gd`, `run_installed_first_scene.gd`, `run_installed_next_arrival_boundary.gd`, `run_spring_content_store.gd`.

- `ui/`: `entry_pad.gd`, `native_pad.gd`.


**Hold: active opening/settings/translation scope**

- `app/`: `first_arrival_entry_consumer.gd`.

- `flow/`: `interactive_prologue.gd`.

- `presentation/`: `first_arrival_actor_renderer.gd`, `prologue_actor_upload_provider.gd`, `prologue_presenter.gd`.

- `rules/`: `prologue_text_work.gd`.

- `services/`: `first_arrival_audio_context.gd`, `first_arrival_bgm_bridge.gd`, `first_arrival_launch_resources.gd`, `first_arrival_media_store.gd`, `first_arrival_upload_bridge.gd`, `opening_audio_bridge.gd`, `opening_audio_rules.gd`, `prologue_line_services.gd`, `prologue_media_queue.gd`, `prologue_native_gpu_pump.gd`, `prologue_text_store.gd`.

- `tests/fixtures/`: `prologue_hud_source_provider_reference.gd`, `prologue_text_presenter_uncached.gd`.

- `tests/hud/`: `run_prologue_hud_provider_tests.gd`.

- `tests/opening/`: `run_prologue_voiced_dialogue_lip_sync.gd`.

- `tests/`: `run_first_arrival_actor.gd`, `run_first_arrival_audio_context.gd`, `run_first_arrival_bgm_load.gd`, `run_first_arrival_compositor.gd`, `run_first_arrival_installed_profile.gd`, `run_first_arrival_launch_preflight.gd`, `run_first_arrival_media.gd`, `run_first_arrival_silent_line.gd`, `run_first_arrival_upload_bridge.gd`, `run_installed_first_arrival.gd`, `run_interactive_prologue.gd`, `run_prologue_hud_installed.gd`, `run_prologue_hud_transition.gd`, `run_prologue_line_services.gd`, `run_prologue_line_transactions.gd`, `run_prologue_media_queue.gd`, `run_prologue_native_gpu_pump.gd`, `run_prologue_source_fixes.gd`, `run_prologue_stages.gd`, `run_prologue_text_presenter.gd`, `run_prologue_text_work.gd`, `run_prologue_visual_tests.gd`.

- `tests/services/`: `run_first_arrival_board_store.gd`, `run_prologue_text_store.gd`.


**Edited: comments only; review pending**

- `app/`: `overlay_dispatcher.gd`.

- `dialogue/`: `talk_player.gd`.

- `flow/`: `cold_no_card_checker.gd`, `cold_no_card_scratch.gd`.

- `overlays/`: `encounters_and_events_overlay.gd`, `ending_overlay.gd`, `event_cg_scenes_overlay.gd`, `subgame_overlays.gd`, `system_overlays.gd`.

- `presentation/`: `calendar_phone_presenter.gd`, `date_overlay_presenter.gd`, `encounters_and_ending_presenter.gd`, `event_cg_scenes_presenter.gd`, `school_events_presenter.gd`, `subgame_overlays_presenter.gd`, `telephone_overlay_presenter.gd`, `vn_scene_presenter.gd`.

- `rules/`: `ordinary_week.gd`.

- `scenes/`: `month_start_controller.gd`.

- `services/`: `native_activity_media.gd`, `native_calendar_source.gd`, `ordinary_command_media_adapter.gd`.

- `tests/app/`: `run_club_command_tests.gd`, `run_full_three_year_route_tests.gd`, `run_multi_route_playtest_suite.gd`, `run_ordinary_commands_1_6_tests.gd`, `run_overlay_dispatcher_tests.gd`, `run_seasonal_command_tests.gd`.

- `tests/dialogue/`: `run_dialogue_presentation_tests.gd`.

- `tests/overlays/`: `run_event_cg_scenes_tests.gd`, `run_subgames_and_event_cg_playtest.gd`.

- `tests/`: `run_import_backend_tests.gd`.


**No full-line prose; inline annotations retained**

- `tests/content/`: `run_content_tests.gd`.

- `tests/dialogue/`: `run_dialogue_tests.gd`, `run_original_dialogue_check.gd`.

- `tests/hud/`: `run_live_capability_tests.gd`, `run_scroll_schema_regression.gd`, `run_upload_regression.gd`.

- `tests/overlays/`: `run_phone_prompt_role.gd`.

- `tests/`: `route_media_smoke.gd`, `run_install_flow_tests.gd`, `run_native_route_registry.gd`, `run_native_vab_memory.gd`, `run_new_game.gd`, `run_new_game_command_reload.gd`, `run_new_game_entry_ui_text.gd`, `run_new_game_state_adapter.gd`, `run_shooter_player.gd`, `run_tests.gd`, `run_text_cache_differential.gd`.

- `tests/services/`: `run_native_new_game_import.gd`.


Remaining repository scope: protected raw/game data, generated/translated content, immutable historical records, source-pinned C/Python comments and active integration holds remain as recorded earlier. This inventory closes the assigned non-active tracked GDScript prose pass, not the repo-wide ticket. Source parity and player readiness need their own evidence; T-39003 remains In Progress pending review and further scope.
