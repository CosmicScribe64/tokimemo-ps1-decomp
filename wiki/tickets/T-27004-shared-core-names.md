---
id: T-27004
title: "Shared core: GameState fields, core types, first enums, top 100 functions"
status: Done
assignee: sonnet (p2-core-apply)
created: 2026-10-24
updated: 2026-10-24
blocked-by: [T-27002, T-27003]
links: `[phase2`, [[game-state]], [[data-types]], [[emulator]], [[tickets/T-20500-emulator-assisted-naming]], [[tickets/T-27000-phase-2]]]
---

## Goal

Name the shared core that about half of every reader's view goes through: GameState (`D_800E6280`, 161
top-level fields, 12,306 uses in 295 files), the shared types of `include/game.h` and `include/main_api.h`
(the 0x44 display pools first), the first enums and the 100 most used main functions (48% of function-name
uses). Conventions: `docs/adr/0001-phase-2-naming.md`; stems: `GLOSSARY.md`.

## Acceptance criteria

- [ ] GameState and its known fields named (calendar bytes, birthday, player names, the nine parameters,
      girl records, club flags, appointments, day-event queue, event log, current girl, game mode, stage and
      step, frame counter, menu mode); unknown fields stay `unk_XX`.
- [ ] Shape-named core types renamed (`Rec38`, `Rec34`, `Rec24`, `DisplayActor44`, `Work800E7D10`, ...),
      size suffixes dropped; hypotheses marked.
- [ ] First enums in their topic headers: girl, place, club, parameter, weekday, game mode, day-event type;
      fields keep their widths with an `/* enum X */` comment.
- [ ] The 100 most used main functions named, with evidence per row; O.BIN names kept. Hardware-layer
      helpers count only when they are entry points that game logic calls; otherwise the next in the ranking.
- [ ] Evidence for every name in the rename tables; emulator runs for meaning claims where the code is not
      enough; Opus review of every table.
- [ ] Clean build 27/27 sha1 after each batch.

## Notes

- Port-first scope (ADR section Scope): game logic in full, hardware layer at entry points.
- Takes over the GameState naming pilot of T-20500 (moved here 2026-10-24, because no rename lands before
  the rename tool), and builds on the field map in [[game-state]].
- The enum list differs from the audit's on purpose (month and the mode selector values left out); the
  reasons are in [[phase2]], step 2.
- Tools that hardcode `D_800E6280` or field names (audit 2.2) have to be updated or moved to `tools/legacy/`
  by T-27002 first.

## Comments

- 2026-10-24 p2-core-apply (branch `p2-core-apply`, worktree `../tokimemo-wt/p2-core-apply`, base `w2-good` 2e51793aa), In Review.
  Applied `config/renames/proposals/core.tsv` (751 rows) in eight commits: three tool commits (`tools/` only, no byte
  of the build changes) and five batches, each with a clean build (27 of 27 sha1 OK, 6,952 of 6,952 functions,
  `nice -n 19`, 2 jobs): 733 rows applied, 18 left out. Ledger
  `config/renames/core.txt` (fields, types, 18 const groups, symbols; 591 rows in all ledgers). Batch tables and
  value files: `drafts/T-27004/`.
  1. `tools/rename.py` (3 commits, 4 new tests): the field oracle maps gcc lines back through `#line` (TACO
     `8013B170` uses `GameState.unk_018`); the error sets ignore gcc's `did you mean` suggestion (RPG_BAT
     `D_800E6280.unk_3A` is K&R code that finds a member of another type by name and errors in gcc before and after);
     the round trip proof ignores the HYPOTHESIS mark of a type row (five rows failed it); `const` accepts the site
     `*.member`. Documented in [[rename-tool]].
  2. 239 field rows (GameState 140 members and the shared records): 24,587 uses located by gcc in 437 C files (the
     most used, `substep`, 1,842; `current_girl`, 1,759).
  3. 75 type rows (`Rec38` to `GirlRecord`, `DisplayActor44` to `Actor`, `Work800E7D10` to `SystemData`, ...), 5 with
     HYPOTHESIS marks: 5,858 tokens in 189 files.
  4. 18 const groups in the new `include/game_enums.h` (created empty, included from `main_api.h`): 15 enums (GirlId,
     Parameter, Weekday, Club, Place, HealthState, Vacation, DayKind, CalendarMarker, DayEventType, MainMode,
     InputDevice, VoiceMode, MenuMode, IconMode) and 3 `#define` groups (PadBits, MouseButtons, PlayerFlag); 226 rows,
     2,437 literals named. Sites (only where the member or function is unambiguous and holds the enum):
     GirlId `GameState.current_girl`, `date_girl`, `D_800E71DF`, and the first argument of `get_g_zyotai_s`,
     `get_g_zyotai_h`, `get_h_tokimeki`, `get_h_yuukou`, `birth_day_check_days` (1,412); MainMode `saved_mode`,
     `overlay_target_mode`, `main_state_set(arg0)` (408); PadBits the four pad words (361; the 204 literals that are
     not in the table, such as 0, stay); DayEventType `*.type` (102; the only other `type` member is the PsyQ `StHEADER`, and every
     hit is in the queue code); Weekday `*.weekday` (33); Club `*.club` (26); InputDevice 38, MenuMode 15,
     IconMode 14, Vacation 14, VoiceMode 8, MouseButtons 6. No literal found for Parameter, Place, HealthState,
     DayKind, CalendarMarker and PlayerFlag: no unambiguous site (`parameter_index` and `place_index` are
     placeholder variable sites that match nothing). Four literals the tool turned into `GIRL_SHIORI` or
     `WEEKDAY_MONDAY` are `0` again by hand: the always-false unsigned compares `(u32)x < 0` (FAKE in ETC
     `80146400`, TEL `80132000`, main `8006CB30`) and `x >= 0` (SHUGAKU `80138A60`). The enums are in hex or decimal as
     the table has them; `/* GirlId */` style comments went onto the typed members by the tool.
  5. 92 func and 39 data rows (the mode machine `main_state_*`/`main_stage_*`/`main_step_*`, `game_state` and
     `system_data`, actor, sprite, dialogue, portrait `bustup_*`, CD and `snd_*` layers): 38,901 tokens renamed;
     `main_step_next` alone has 4,130 uses in 219 files. 8 HYPOTHESIS rows are in the new `config/symbol_addrs_hypotheses.txt`; the names in
     EVENT's range (`actor_pool`, `draw_buffer`, `sprite_quads`, `overlay_entry`, ...) are in
     `config/symbol_addrs_main.txt`. The dead `MAIN_API_OVERRIDE_D_80120650` line of `include/ovl/EVENT.h` was
     removed by hand: `main_api.h` now declares `actor_pool_96`, and EVENT keeps its own `D_80120650`.
  6. 62 comment rows: 61 one-line English comments above the C definitions of O.BIN and patch names in `src/main`
     (K&R definitions included; wrapped at 120 columns, American spelling) and the `main_api.h` header comment.
  Left out (18 rows):
  - `field GameState u54C`, `unk_54C`, `unk_54C_0` (3 rows): `unk_54C` and `unk_54C_0` are `#define` aliases of `u54C.a`
    and `u54C.e0`, not members; the tool has no row kind for a macro alias. Needs tool work, then the three rows run
    as they are.
  - `field DisplayActor44 unk_18 frame`: `CAL_MARK(frame, side, d)` in `src/main/800674B0.c` has a macro parameter
    `frame`, so `.frame` in its body takes the argument. Rename the macro parameter first (no row kind), or give
    the field another name (`frame_index`?).
  - `const Month` (12 rows): [[phase2]] step 2 leaves the month out on purpose; drop the rows from the table or
    decide again.
  - `file core include/game.h -> include/game_types.h` and `file core - include/game_enums.h` (2 rows): file and
    header renames go in a commit of their own at the end of the unit, and the tool refuses `game.h` until the
    tools that find it by name change. `include/game_enums.h` exists already (empty header, then the const runs),
    so only the `game.h` rename is open.
  For the review:
  - Prefixes the applied names use that the ADR main map and the glossary do not list: `main_`, `mode_`, `actor_`,
    `actors_`, `audio_`, `bustup_` (O.BIN stem; the map has `portrait_`), `card_` (the map has `memory_card_`),
    `command_`, `daily_`, `debug_`, `dialogue_`, `frame_`, `game_` (the map has `game_mode_`), `icon_`, `last_`,
    `light_`, `mascot_`, `name_buf_`, `overlay_`, `parameter_`, `place_`, `quarter_`, `screen_`, `system_`, `voice_`;
    and `snd_`, which the ADR calls an SDK stem (`spec/core` section 17 proposes it as the single sound prefix).
    CODING_STANDARDS section 4 wants the map rows in the commit that first uses a prefix; I did not edit the ADR
    maps, because several rows need a decision (`snd_`, `card_`, `ending_start` against the ENDING overlay prefix).
  - Not run: emulator checks (the notes are the table's); nothing was re-derived.
  - Checks at the tip (`57629dff2`): clean build 27/27, `ninja progress` 6,952/6,952, `rename.py check` OK (591 ledger
    rows), `check_headers.py` OK, `migrate_globals.py --check` OK, `sync_protos.py --check-branch` OK, the tool suite
    (`discover -s tools -p 'test_*.py'`) 1,646 tests OK (3 skipped), no `INCLUDE_ASM` in `src/` before or after,
    `funcdiff --resolve` MATCH (compared default build) for `main_step_next`, `main_stage_set`, `main_step_set`,
    `snd_command`, `girl_state_rank`, `bustup_read_start`, `gpu_init`, `frame_render`, `actors_draw`, `dialogue_run`.
- 2026-10-24 p2-core-apply, review round 1 fix (two blocking findings: names against the ADR and glossary, map rows). 133 more
  rows through `tools/rename.py` (tables `drafts/T-27004/fix_symbols.tsv`, `fix_types.tsv`, `fix_fields.tsv`, made by
  `drafts/T-27004/mkfix.py`; ledger `config/renames/core.txt`, now 724 rows), one clean build: 27 of 27 sha1 OK, `ninja progress`
  6,952 of 6,952, no `INCLUDE_ASM` in `src/`, `rename.py check`, `check_headers.py`, `migrate_globals.py --check` and
  `sync_protos.py --check-branch` OK, tool suite 1,646 tests OK (3 skipped), `funcdiff --resolve` MATCH (default build) for
  `mode_step_next`, `game_mode_set`, `audio_command`, `talk_run`, `portrait_read_start`, `memory_card_start`, `actor_draw_all`,
  `draw_frame`, `boot_loop`, `title_mode_handler`, `talk_render`. Rename chains: the first names stay in the ledger and map to the
  new ones (`rename.py lookup main_step_next`).
  - Glossary and ADR conflicts renamed (finding a to f): `main_state_*` to `game_mode_*` (also the patch name `main_state_set`
    to `game_mode_set`), `main_stage_*`/`main_step_*` to `mode_stage_*`/`mode_step_*` and the GameState fields `mode`, `stage`,
    `step` and their timers and scratch bytes to `game_mode`, `mode_stage`, `mode_step`; `bustup_*` to `portrait_*` (O.BIN names
    keep the stem); `card_*` to `memory_card_*` (functions, `card_block`, six fields, `MemoryCardFileRequest`); `snd_*` to `audio_*`;
    `dialogue_*` to `talk_*` (with the four `Dialogue...` types and the two patch names `dialogue_render` and
    `dialogue_speak_part` of the M9 list); `event_still_show` and `event_anim_show` to `scene_*`, `event_flags` to `scene_flags`,
    `events_today` to `day_event_count_today`, `daily_event_bits` to `day_event_done_bits`; `ending_start` to
    `schedule_ending_start` (ENDING's prefix is not main's); `phone_numbers` to `girl_phone_digits` (read by TEL and SHOUGATU, so
    the TEL prefix `phone_` stays with TEL).
  - Unlisted prefixes: renamed to a listed one where one fits. `main_loop` is `boot_loop`; `mode_*` handlers are
    `<module>_mode_handler` (`boot_`, `title_`, `holiday_`, `ovl_loader_`, `schedule_command_screen_`, `schedule_day_screen_`);
    `frame_render`, `screen_shake_update`, `debug_number_show` take `draw_`; `icon_bar_update`, `mascot_update`,
    `screen_frame_draw`, `mascot_choice` take `hud_`; `light_effect_draw` takes `sprite_`; `quarter_flags_rotate` takes
    `schedule_`; `daily_flags_clear` is `day_event_today_clear`; `game_reset_to_title` is `game_mode_reset_to_title`;
    `voice_play` is `talk_voice_play`; `overlay_*` is `ovl_*` (functions, `overlay_entry`, six GameState fields);
    `name_buf_*` is `name_code_*`; `parameter_name_table` is `param_name_table`; `command_deltas` is
    `school_day_command_deltas`; `last_screen_mode` is `dec_bg_last_screen_mode`; `cd_cmd_*` is `cd_command_*`;
    `day_events_build_week` and `day_events_clear` are `day_event_queue_*`.
  - Map rows added in the same commit (ADR main map and glossary, as the ADR asks of a unit): `actor_` (glossary Module word
    `actor`), `audio_` (Module word `audio`; the ADR text now says the driver takes it), `mode_stage_`/`mode_step_` in the
    game-mode row, `name_code_` and `scene_` in the talk row, a row for `game_state`, `system_data` and `place_` (glossary rows
    game state, system data, place), a rule that mode handlers take their module's prefix and end in `_mode_handler`, and `csr` in
    the unresolved developer abbreviations. Decisions the user may still want to confirm: `audio_` as the sound-driver prefix
    (the ADR said only that `snd_` is an SDK stem and `sound_` is OPTION's), and `actor_` as a main prefix.
  - Abbreviations outside the table renamed too: `appt_place_N`, `tokimeki_frac`/`x_frac` and the other `_frac` fields (to
    `_fraction`), `attr`, `exp` (to `practice_points`, HYPOTHESIS), `player_flags_hi`, `frame_hcount_limit`, `tpage_req`,
    `GirlFlagBitsSched`, `PlayerFlagsHiBits`; `TacoSettingsBits` and `SaveTacoSettings` take TACO's stem `shooter`.
  - Not done: the `MainMode` enum tag and its `MODE_` constants. `rename.py` has no row kind for an enum tag or an enumerator (plan:
    "0 definitions visible"; limit now in [[rename-tool]]). The stem is `game_mode`, so a follow-up needs tool work, then one
    batch. Sibling proposals (`config/renames/proposals/*.tsv`, `wiki/spec/m-*.md`, `h-*.md`, `o-*.md`) still use the first core
    names; map them with `rename.py lookup` when those units are reviewed.
  - Non-blocking items: the build logs in `drafts/T-27004/` are reduced to their proof lines (steps, sha1 OK lines, check
    lines), and the tables, value files and small logs are committed; the ADR and CODING_STANDARDS example now reads
    `/* GirlId */`, the comment the tool writes. `wiki/kanban.md` still lists T-27004 under Backlog; the orchestrator syncs the
    board (this worktree does not edit it).
- 2026-10-24 review round 2 (standards and spec, at the `p2-core-apply` tip `44aaa8114`): approved, no blocking findings. The
  round 1 fixes hold: the applied names follow the ADR main map and the glossary stems, the map rows went in with the commit
  that first uses a prefix, and the ledger, build and checks are as reported above. Moves to Done. Two non-blocking
  follow-ups, not done here:
  - The `MainMode` enum tag and its `MODE_*` enumerators still conflict with the glossary (stem `game_mode`; bare `mode` is a
    trap word; `main_` is not a listed prefix). `rename.py` has no row kind for an enum tag or an enumerator, so only a hand
    rename was possible (ADR decision 5). It is disclosed above and in [[rename-tool]], but no follow-up ticket exists and
    [[phase2]] "Names to re-check" does not mention it. Wanted: a tool-support ticket (row kinds for an enum tag and an
    enumerator), one rename batch, and a line in [[phase2]].
  - Six non-O.BIN names use the O.BIN stem `dec_bg_`: `dec_bg_decode_to_vram`, `dec_bg_draw`, `dec_bg_show`,
    `dec_bg_brightness`, `dec_bg_lba_cache`, `dec_bg_last_screen_mode`. The ADR main map row is `bg_`, with `dec_bg_` for
    O.BIN names, and the glossary stem for dec_bg is `bg`; the fix applied the rule "only O.BIN names keep the stem" to
    `bustup` but not to this. Rename them to `bg_*` in the same follow-up batch, or record the choice in the ADR.
- 2026-10-24 merge into the integration tree, attempt 1 failed in `rename.py replay` (11 PROBLEM lines such as `field GirlRecord
  unk_00 -> tokimeki_frac: GirlRecord has 0 members named unk_00`). Cause, a tool defect: `chains()` keys member rows by the
  type name of the row, so `field Rec38 unk_00 tokimeki_frac` (before the type rename) and `field GirlRecord tokimeki_frac
  tokimeki_fraction` (after it) were two chains, and replay took the first for a chain that ends at `tokimeki_frac`, a name the
  tree no longer holds. Fix after the review, in its own commit: `link_member_rows()` in `tools/rename.py` puts the field rows
  under the type's current name before replay builds its chains, plus the test `test_replay_field_chain_across_a_type_rename`
  (fails without the fix with the exact message above). Only `replay` changes; `check`, `lookup` and the plan code are as
  reviewed. After merging `w2-good` (conflicts in `wiki/log.md` and `wiki/spec/core.md` only; in `core.md` the w2-good
  wording with the final core names): replay 127 rows run, 0 files changed; clean build 27 of 27 sha1 OK, `ninja progress`
  6,952 of 6,952; `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch`, `rename.py check`
  (724 ledger rows) OK; tool suite 1,647 tests OK (3 skipped). The tool commit came after the round 2 review and wants a
  look in the next review of `tools/rename.py`.
