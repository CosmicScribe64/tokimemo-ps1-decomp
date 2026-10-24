---
id: T-27015
title: Phase 2 naming and port spec for the main scene library (8007C030.c)
status: In Review
assignee: Sonnet analyst (p2a-m-dates-b)
created: 2026-10-24
updated: 2026-10-24
links: ["[[tickets/T-27000-phase-2]]", "[[spec/m-dates-b]]", "config/renames/proposals/m-dates-b.tsv"]
---

## Goal

Roadmap phase 2, unit `m-dates-b`: name the functions, globals, fields and constants of the main-exe object `src/main/8007C030.c` (0x8007C030-0x80085E30, 111 symbols) and write the port spec. The task text called the unit "girls, day advance and limits"; the object is the shared scene step library of the event scenes (background load and wipes, portrait load and fades, dialogue engine, choice menus, heroine picker, event still, SD sprite address tables, CD level meter) with `select_girl*`, `change_girl`, `change_yoshio`, `day_plus` and `check_para_limit` inside it. Scope is the port-first one of 2026-10-24: game logic, state and flow in full, the hardware layer at module level. This pass writes tables and wiki pages only; `src/`, `include/` and the symbol files are not touched.

## Acceptance criteria

- [x] `config/renames/proposals/m-dates-b.tsv`: 258 rows (1 file, 20 func, 75 comment, 71 data, 27 field, 64 const); every O.BIN function name of the object (78) has a comment row (74 here, 4 in `core.tsv`); 25 rows are HYPOTHESIS.
- [x] `wiki/spec/m-dates-b.md`: the step-table scene model, scene state, background and wipe rules with frame counts, portrait file and outfit rules (with the disc file name of every sector), dialogue engine (script tables, 20 speak codes, name codes, choice shuffle, voice wait and cancel), heroine picker, event still, calendar and limit helpers, SD sprite tables, CD level meter, port notes, 16 open questions.
- [x] Names follow `core` and the hardware tables where they exist; disagreements are listed in the spec open questions, not as second names.
- [x] No game text, asm, screenshots or game data committed; no edits under `src/`, `include/` or `config/symbol*` files.
- [ ] Code review against `CODING_STANDARDS.md` (tables and wiki only) and merge of the table by the phase 2 merge step.

## Notes

- Evidence: the matched C and the overlay step tables, the SERIFU script data parsed from the disc (numbers only), and about twenty short emulator runs (a patched scratch copy of `tools/emu` with a memory-write option; nothing in the repo changed). Run list in [[spec/m-dates-b]] section 15.
- Findings worth reviewing first: every event scene is a table of function pointers indexed by `step` and this object holds the shared steps; dialogue scripts are SERIFU disc files linked for a fixed RAM address with three parallel tables (text, command, voice) and 20 speak codes; wipe kind is `month % 7`; portrait files map to BUSTUP file names by sector (`0x4FE5 + 0x2D * k`); the three-way choice order is a function of `frame_count`; `check_para_limit` clamps are exact (emulator-checked).
- Core corrections requested in the spec open questions: `bustup_quad_offsets` takes an expression index, not a girl id; `step_timer` is not advanced by the main loop; the core comments for `normal_date_girl_in/out` say slide, the code fades.

## Comments

2026-10-24: analysis and tables written by the p2a-m-dates-b worktree; moved to In Review. The board card is not moved by this pass (the kanban is edited by the merge step).
