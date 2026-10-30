---
id: T-39006
title: Replace fabricated production event state with original rules
status: In Progress
assignee: Codex
created: 2026-10-29
updated: 2026-10-29
links: ["[[port-adversarial-review]]", "[[port-testing]]"]
---

## Goal

Resolve review R1, R2 and R13: production event handlers must consume actual saved player/heroine state and original rules, including companion activities and cancellation. Synthetic completion cannot certify gameplay.

## Acceptance criteria

- [ ] Inventory every fabricated roster, fixed reward and handler substitution in production dispatch.
- [ ] Replace them using source-backed inputs/outcomes; cancellation preserves state unless original source specifies an effect.
- [ ] Reach affected events naturally through unchanged production handlers, validate original media and state, and test save/relaunch.
- [ ] Separate bounded simulation tests from actual-route acceptance.
- [ ] Independent code review resolves all findings against CODING_STANDARDS.md.

## Notes

Reviewer reproduced identical culture results for all-zero/all-200 parameters and full rewards on immediate Cancel for culture, exam and GEKO. Review report holds exact locations and counterexamples. Do not remove explicit unsupported guards as a shortcut.

## 2026-10-29 route-parity fan-out slice (worktree, uncommitted)

Status stays Ready on the board (board/log edits are reserved for the integrator). This slice needs independent review before any status change.

Inventory of production dispatch (`port/godot/app/overlay_dispatcher.gd` `step_overlay` with saved `rule_state`):

| Dispatcher mode | Fabrication found | Disposition in this slice |
|---|---|---|
| 0x48 BUNKASAI | fixed humanities/science/art 80/80/85, preparation 120, fixed +3/+3 and parameter gains, Cancel rewarded | Replaced by `rules/culture_festival.gd`: phase-0 block choice (spec 5.1), `culture_visitor_pick` with `srand(timer_ticks)` (5.2), member-entry bit 2 and literature festival-call bits (8.4), `culture_result_apply` (8.1, gain tables byte-checked against `asm/ovl/BUNKASAI/data/BUNKASAI_data.data.s`), `culture_event_log_add` (8.2). Cancel has no source exit and is refused without effect. |
| 0x3D TEL | closeness default 80, looks/closeness arguments swapped, `randi()` fallback, fixed +2/+2; source `phone_date_invitation_resolve` also writes stress+9, guts+rand&7, affection+frame_count&3, syoushin-12, and an appointment flag from `D_80140560` | Rejected with saved state (reason string names each gap) |
| 0x3E TEL_ACTION, 0x34 GEKO, 0x35 EXAM, 0x4E SHOUGATU, 0x4F VALEN, 0x52 MASTER, 0x47 TAIIKU | `_default_girls_roster()` (renamed `_preview_girls_roster`), fixed topic inputs, player total 485, practice 385, fixed affection lists | Rejected with saved state |
| 0x50 DATE, 0x58 DATE2, 0x4B OMIMAI, 0x49 BUNKA_SD, 0x4A BUNKAKEN, 0x4C SHUGAKU, 0x4D KANGEI, 0x51 RENSYU, 0x53 EN_NICHI, 0x5E EVENT_CG | default date/spot state, fixed evaluation inputs, fixed rewards | Rejected with saved state |
| 0x57 RPG | fixed `rand_seed` 512, invented affection ±, talk-code defaults | Rejected with saved state |
| 0x59 TACO, 0x5A TT | invented affection +3/+1, payload defaults (`menu_hit_3` 0x9F, `system_data_flag` 1, 4250 frames) | Only fields present in the subgame return payload pass through; no affection, no defaults. Subgame internals not audited here |
| menus (0x01, 0x3F, 0x36, 0x37) | default `affection_delta` +1 on exit | No deltas |
| ENDING | unchanged (R11 territory) | not in this slice |

Preview `step_overlay` calls without saved state no longer return any reward deltas. The fixed-reward table `_extract_overlay_deltas` is deleted. `school_day_controller._overlay_mode` now surfaces a dispatcher rejection as `result.error` and leaves saved state and the overlay untouched.

R2 companion activities:
- `rules/scheduler.gd::command_girl_select` ports `schedule_command_girl_select` (all nine command cases, club cases 0-10, tie rules) from saved met/b2/club bits and girl_scores closeness. It replaces the command-0-only shortcut in both week builders. Rank bars come from a new `command_rank_bars` s32[5] row of `tools/port/school_day_tables.py` (0x800B6748..0x800B6758). Fresh Docker export `.asset-cache/port/school-day-tables-rank-bars-v1/manifest.json` (SHA256 `a7934845ef86648c9ee40311235fb5c272e8f68932ed06650095d82d88dfe6f8`) gives `[40, 35, 25, 45, 55]`, matching `asm/data/data_800B5950.data.s`. With an older pack lacking the row, commands 1, 2, 3, 5 and 6 fail closed; command 0 uses `command0_rank_threshold` (same word), 4 and 7 need no bar. `new_game_tables.py` SOURCE_PINS updated for the changed exporter hash. `playable_boot` still pins the old `TABLES_SHA`; switching it to the new pack is an integration step.
- `school_day_controller._school_day` no longer rejects companion records; it rejects only `extra` values outside 0..10/0xF. Club command SD sets now use `club_sd_set_*[club][variant]` with variant 0 for companion 0 (new summer pair table from m-weekday). Vacation club days re-run `command_girl_select(4)` for the pack and actor B, as `school_day_vacation_sd_load/actors_init` do. The rest-id poor-flag clear no longer hits club ids 15/16.
- New `girl_state_rank` / `girl_log_rank` helpers port `get_g_zyotai_s & 0x7F` and `get_g_zyotai_h`.

Tests: `tests/rules/run_route_state_parity_tests.gd` (added to `run_tests.gd`). Docker `tokimemo-port` core run: PASS, 6 suites. Covers saved-state mutation (affection/met/b2/club change companion; parameters 0 vs 200 change festival tokimeki 10 vs 20; practice points; visitor pick only among met heroines across srand seeds), the spec 8.1/8.2 emulator case (gains `[4,5,4,4,5,5,4,4,4,4,4,4,4]`, visitor affection +2, event log girl 0 flag 1), Cancel refused with no deltas, eight event modes rejected with saved state, preview exits without rewards, and the culture festival through the unchanged production `_overlay_mode` (queue advance, writes applied) versus an exam left untouched. `run_native_scheduler_modes` 55 checks and `run_native_route_registry` 230 checks pass.

Not done / limits: these are bounded rule tests on invented fixtures, not actual-route acceptance. The saved-state model has no `clubs`, `event_log`/`event_log_count` or `timer_ticks` from the opening adapter, so a real route reaching BUNKASAI is rejected until those fields are carried. BUNKASAI scenes, dialogue, BUNKA_SD shows, fortune and TT are not played; girls 1-3 scenes, the TT exhibit, the girl-0 birthday scene and girls 9/10 in 1995 are rejected. TEL and every other event family above still need source ports. Original media validation, natural reach from a fresh installed save and save/relaunch were not exercised. `run_full_three_year_route_tests.gd` and `run_multi_route_playtest_suite.gd` (R1) were not run (they need `installed-clean-v1` and orchestrator paths); their TEL/DATE steps now get rejections instead of fixed deltas. `run_overlay_dispatcher_tests.gd` and the telephone suite's dispatcher section already fail to open (missing `hud-production-v1`), unrelated to this change.

## 2026-10-29 route-parity v2 (review fixes, worktree, uncommitted)

Status unchanged (Ready); review is still required. Six review findings re-verified against source and fixed:

- `rules/culture_festival.gd` visitor pool reads the met bit of every girl 0..10, girl 0 included (`src/ovl/culture_festival/mode_stage_dispatch.c` `culture_visitor_pick`). The old "girl 0 always qualifies" came from the spec, not the code. An empty pool is rejected (source `rand() % 0`).
- `clubs[club]` must be `{"practice_points": int}`; a missing field, a bare int or a non-int value is rejected with `saved state lacks clubs[N].practice_points` instead of defaulting to 0xFFFF.
- RNG: the club, year, birthday and computer-club 1996 TT rejections now run before the reseed. The TT one holds for every girl because each `*_visit_entry` and member block sends club 3 in 0x60 to the computer-game exhibit. Rejections after the pick (girls 1-3, girls 9/10 in 1995, a refused draw) restore `rng.checkpoint()`, so a refused step leaves the shared RNG unchanged.
- Girls 9/10 in 1995: the rejection reason now cites `megumi_visit.c` / `yumi_visit.c` `*_visit_entry`. Club 0 sends every year except 0x60 to the speech contest, and other clubs have their own non-0x60 scene. None of these scenes is ported or verified, and the 1995 gain rows are zero. The previous "no 1995 scene" wording was unsourced.
- Parity suite (`tests/rules/run_route_state_parity_tests.gd`) gained these targeted cases:
  - commands 5/6 with girls 8/7
  - the club-10 girl-10 branch
  - Shiori in the player's club without b2 (self score 0)
  - an equal-score tie (the second girl wins)
  - hand-computed `girl_state_rank`/`girl_log_rank` tables
  - rank-4 literature visits setting neither bit 12 nor 13, in both 95 and 96
  - club 4 with a girl-10 visitor at rank 4 writing no event_log entry
  - event-log rank checked against hand-computed `get_g_zyotai_h` (2/1/0/3)
  - girl 0 without met excluded from the pool, and the empty-pool rejection
  - the girl-0 birthday rejection
  - the girls 9/10 1995 rejection, with the 1996 visit running
  - practice_points shape rejections
  - the RNG unchanged after rejected picks
- Mutation check in Docker: 16 single mutations, the reviewer's 12 plus 4 more (girl-0 pool shortcut, RNG restore, 95/96 bit swap, practice-points shape). Each makes the parity suite fail.
- `tests/overlays/run_reachable_families_g3_gate.gd` is re-scoped. It no longer asserts removed fixed rewards: TEL_ACTION +1, TEL +2/+2 and appointment, DATE positive deltas and date_count, GEKO +2/+2, TAIIKU +4, EXAM +2, the RENSYU/SHUGAKU calendar advance, and RPG talk_result_code. For each family that is now refused:
  - every saved-state input fails the frame, keeps the native mode and leaves saved state unchanged, with no deltas (BUNKASAI: CIRCLE is refused because this fixture lacks clubs/event_log/timer_ticks; CROSS is cancel_rejected without writes)
  - screens are stepped in preview only, and the preview exit carries no reward deltas
  - family evidence is marked `verified_live: false`
- `tests/overlays/run_telephone_overlay_tests.gd` Suite 6 now asserts the saved-state TEL refusal and steps day_select/call_response in preview.

Evidence limits (v2):
- Core run (`run_tests.gd`): PASS, 6 suites. `run_native_scheduler_modes`: 55 checks pass. `run_native_route_registry`: 230 checks pass.
- G3 gate: 418 pass, 41 fail; at HEAD it was 278 pass, 43 fail. Every remaining failure also occurs at HEAD or is a render check that needs the missing `.asset-cache/port/hud-production-v1`. The HEAD-only failures that the re-scope removed are the TEL appointment and +2/+2 reward checks. The OLH/OPTION/ETC failures and the reconciliation JSON write (read-only mount) also fail at HEAD.
- Telephone suite: 2 of 35 fail, the same two HUD-capability failures as at HEAD. Suite 6, which holds the new TEL refusal checks, does not run without the HUD capability, so those checks have not executed yet.
- Not run: `run_full_three_year_route_tests.gd` and `run_multi_route_playtest_suite.gd`.
- These remain bounded rule tests, not actual-route acceptance.

## 2026-10-29 integration follow-up: live overlay graduation gate

- After integration, `tests/overlays/run_live_overlay_graduation_gate.gd` failed 23 of 82 checks; it failed 4 of 75 at `c1c2a7aa5`. The 19 new failures were the TEL place/day/call stepping and appointment/affection writeback, GEKO dispatch and +2 writeback, DATE stage and counter, RPG_BAT victory and `talk_result_code` writeback, and `PlayableBoot.step_active_overlay` on TEL_ACTION. All came from this track's `PRODUCTION_REJECTIONS` guard in `app/overlay_dispatcher.gd`, which refuses TEL, TEL_ACTION, GEKO, DATE and RPG_BAT when saved state is present. The v2 slice never ran this gate.
- The gate is now re-scoped the same way as the G3 gate. Each saved-state input must fail the frame with the dispatcher's reason, keep the mode and leave saved state unchanged. Refused TEL books no appointment and writes no affection, refused DATE leaves the date counter alone, refused RPG_BAT writes no `talk_result_code`, and refused TEL_ACTION keeps `girl_id` 0. The screens (TEL cursor and subscreens, GEKO stages, DATE stage 2, RPG_BAT victory) are stepped in preview only, and each preview exit must carry no deltas. A refused family has no production exit, so the gate drops its overlay and points the queue at the next event.
- Result (Docker, tokimemo-port 4.7.2): 4 of 98 fail. These are the same 4 that fail at `c1c2a7aa5` (the HUD capability and notebook photo, `PlayableBoot` resources and the `_present_committed` render), all of which need the missing `.asset-cache/port/hud-production-v1`. The 91 `_draw_actor` SCRIPT ERROR lines from `hud_compositor.gd` also occur at `c1c2a7aa5`.
- Observation, not fixed: on a refusal, `PlayableBoot.step_active_overlay` still stores the dispatcher's returned overlay state, which carries `action: "rejected"`, back into `rule_state.overlay_state`. The session path leaves saved state unchanged.

## 2026-10-29 fresh-handoff slice (worktree, uncommitted)

The saved-state fields this track found missing are now carried from the fresh init. Review is still required.

- `flow/new_game_state_adapter.gd` projects `clubs` (12 x `{practice_points, level, flags}` from +0x66C, ClubRecord), `event_log` (256 x 8-byte EventLogEntry dictionaries from +0x75E) and `event_log_count` (+0xF5E). All are boot_event_state_init / boot_calendar_init zeros (game_init.c:294-468), and the ownership check now covers 0x75E..0xF5E.
- `timer_ticks` is not copied into rule_state: GameState.timer_ticks is the live core word. `school_day_controller._overlay_mode` exposes `state.timer_ticks` to the dispatcher for the call only and removes it afterwards. The parity suite checks that the router visitor pick follows `srand(core timer_ticks)` for 24 tick values; a mutation that feeds a constant fails it.
- `flow/fresh_gameplay_handoff.gd` refuses a stage 21 state whose boot-owned records differ from the source boot values.
- Still not covered: a real route reaching BUNKASAI.

### 2026-10-29 fresh handoff v2 (worktree, uncommitted)

- The state adapter records unsourced fields in `opening.unsourced`, and the handoff copies them to `rule_state.resources.unsourced` at attach. They are `scene_work` (golden -1024, writer not identified), `background_cache`/`background_show` (golden a6 [0,16361,18803,18794] / 1: the prologue layer-1 loads and the command-room read do not run on this route) and every NAME_ENT `rand()` result. The school-day setup and the holiday background lookups now refuse to read the unsourced cache (`school_day_state.unsourced_error`).
- girls[1] club: for Shiori 1/1 type A the original has DRAMA (flags 0x00100000, a1-a8, both sets). The port's launch seed gives LITERATURE. The BIOS rand state at NAME_ENT is in OpenBIOS kernel RAM and is not sourced, so the club stays a reported unsourced value. girls[2] COMPUTER and girls[4] match by chance.
- `frame_count`, `timer_ticks` and `system_data.seed` in the handoff test and the comparison are placeholders (1000 / 0 / 2000). The interactive opening passes the live `EntryRuntimeContext` words. The golden values depend on timing (a7: frame_count 21333/21363, timer_ticks 0x5773/0x578F), so comparisons must allow for timing. `srand(timer_ticks)` for the visitor pick still needs the live word.
