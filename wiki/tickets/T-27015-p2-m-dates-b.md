---
id: T-27015
title: Phase 2 naming and port spec for the main scene library (8007C030.c)
status: Done
assignee: Sonnet analyst (p2a-m-dates-b)
created: 2026-10-24
updated: 2026-10-24
links: ["[[tickets/T-27000-phase-2]]", "`spec/m-dates-b`", "config/renames/proposals/m-dates-b.tsv"]
---

## Goal

Roadmap phase 2, unit `m-dates-b`: name the functions, globals, fields and constants of the main-exe object `src/main/8007C030.c` (0x8007C030-0x80085E30, 111 symbols) and write the port spec. The task text called the unit "girls, day advance and limits"; the object is the shared scene step library of the event scenes (background load and wipes, portrait load and fades, dialogue engine, choice menus, heroine picker, event still, SD sprite address tables, CD level meter) with `select_girl*`, `change_girl`, `change_yoshio`, `day_plus` and `check_para_limit` inside it. Scope is the port-first one of 2026-10-24: game logic, state and flow in full, the hardware layer at module level. This pass writes tables and wiki pages only; `src/`, `include/` and the symbol files are not touched.

## Acceptance criteria

- [x] `config/renames/proposals/m-dates-b.tsv`: 255 rows (2 file, 20 func, 75 comment, 67 data, 27 field, 64 const); every O.BIN function name of the object (78) has a comment row (74 here, 4 in `core.tsv`); 14 rows are HYPOTHESIS. Rows that repeat a row of a merged table (m-dates-a, m-hud, m-weekday) carry the identical name.
- [x] `wiki/spec/m-dates-b.md`: the step-table scene model, scene state, background and wipe rules with frame counts, portrait file and outfit rules (with the disc file name of every sector), dialogue engine (script tables, 20 speak codes, name codes, choice shuffle, voice wait and cancel), heroine picker, event still, calendar and limit helpers, SD sprite tables, CD level meter, port notes, 17 open questions.
- [x] Names follow `core`, the hardware tables and the merged phase 2 tables (m-dates-a sound driver names for the CD level meter, m-hud names for the mood flag, the actor slots and the portrait quads, m-weekday names for the actor-96 aliases); disagreements are listed in the spec open questions, not as second names.
- [x] No game text, asm, screenshots or game data committed; no edits under `src/`, `include/` or `config/symbol*` files.
- [x] Code review against `CODING_STANDARDS.md` (tables and wiki only) and merge of the table by the phase 2 merge step.

## Notes

- Evidence: the matched C and the overlay step tables, the SERIFU script data parsed from the disc (numbers only), and about twenty short emulator runs (a patched scratch copy of `tools/emu` with a memory-write option; nothing in the repo changed). Run list in `spec/m-dates-b` section 15.
- Findings worth reviewing first: every event scene is a table of function pointers indexed by `step` and this object holds the shared steps; dialogue scripts are SERIFU disc files linked for a fixed RAM address with three parallel tables (text, command, voice) and 20 speak codes; wipe kind is `month % 7`; portrait files map to BUSTUP file names by sector (`0x4FE5 + 0x2D * k`); the three-way choice order is a function of `frame_count`; `check_para_limit` clamps are exact (emulator-checked).
- Core corrections requested in the spec open questions: `bustup_quad_offsets` takes an expression index, not a girl id; `step_timer` is not advanced by the main loop; the core comments for `normal_date_girl_in/out` say slide, the code fades.

## Comments

2026-10-24: analysis and tables written by the p2a-m-dates-b worktree; moved to In Review. The board card is not moved by this pass (the kanban is edited by the merge step).

2026-10-24 (review round 1 fixes): the reviewer's two blocking findings and the non-blocking ones are resolved; branch rebased on `w2-good` (dcca60039). Name clashes with merged tables removed: the CD level meter now uses the m-dates-a names (`snd_cd_level`, `snd_cd_level_right`, fields `precision_bits` and `options`, buffer `snd_cd_level_samples`); `D_800B5A60` is dropped in favour of the m-hud name `mouth_frames_reversed` (its swap partner `D_800CA360` is `mouth_frames_reversed_saved`); `ActorSlot` 30 and 31 take the m-hud names `ACTOR_BUSTUP_EYES` and `ACTOR_BUSTUP_MOUTH`; the three SD sector tables are no longer proposed (m-weekday and m-holiday name them); the actor-96 and actor-97 alias labels follow the m-weekday names (`actor96_anim_flags`, `actor96_sequence_index`) and the same pattern for the rest. Spec fix: the event picture mouth flag `D_80122D10` has three modes (0 voice-driven lip sync, 1 periodic animation every 240 frames, 2 neither), not 1 = animated and 0 = static. Other fixes: usage lists and counts recomputed from the data asm and the C sources, the `dec_init` header test and the `nn` skip rule worded as the C has them, speak-code names without game-meaning words (`SPEAK_PLAYER_PAREN`, `SPEAK_PLAIN_AUTO`, `SPEAK_CHOICE_2_PAREN`), a `file` row for `include/main/scene.h`, four new open questions (unowned `D_80125C04`, the m-menutext name code overlap, the `dialogue_` versus `talk_` prefix, the mood flag pair). Code reading only, no new emulator run. Back in review.

2026-10-24 (review round 2, approved): both blocking findings are resolved and the reviewer found no new clear errors. The branch is rebased onto the `w2-good` tip 9404a703a (merge-base equals tip). Findings: none.
- Blocking 1, name clashes: a script that compared every merged proposals table against `m-dates-b.tsv` found no row with the same kind, scope and old name but a different new name, and no new name shared by two different old names. The only differences are `comment` rows that `core.tsv` words differently, and the `file main -` key that m-hud also uses (m-hud maps it to `hud.h`, m-dates-b to `scene.h`); neither is a name clash.
- (a) The CD level meter rows use the m-dates-a names (`snd_cd_level`, `snd_cd_level_right`, `snd_cd_level_samples`, fields `precision_bits` and `options`). The `SD_GetCDLevel` guard text matches the C (top byte of `D_80125D14` is 0, `audio_state` bit 0x4000 clear, SPU transfer calls idle), and spec section 11 and the comment rows agree.
- (b) The `D_800B5A60` `event_anim_flag` row is gone. The swap partner `D_800CA360` is `mouth_frames_reversed_saved`, and the `func_80085234` swap and copy-back description matches the C (actors 96 and 97, sprites 58-63).
- (c) `ActorSlot` 30 and 31 are `ACTOR_BUSTUP_EYES` and `ACTOR_BUSTUP_MOUTH`, the same as m-hud. The `SPRITE_BUSTUP_MOUTH` and `SPRITE_BUSTUP_EYES` rows match `func_80063668` (sprite 0 is 16 * frame of actor 31, sprite 1 is 32 * frame of actor 30).
- (d) The `sd_file_sector_*` rows are gone, and the actor-96 alias rows follow the m-weekday names.
- Scope check at merge time: `git diff --name-only w2-good...HEAD` lists only `config/renames/proposals/m-dates-b.tsv`, `wiki/log.md`, `wiki/spec/index.md`, `wiki/spec/m-dates-a.md`, `wiki/spec/m-dates-b.md`, `wiki/spec/m-hud.md` and this ticket. Nothing under `src/`, `include/` or `config/symbol*`.

Moved to Done. The board card is moved by the kanban sync in the merge step (this branch does not edit `wiki/kanban.md`).
