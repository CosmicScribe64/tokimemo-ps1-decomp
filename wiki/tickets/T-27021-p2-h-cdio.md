---
id: T-27021
title: Phase 2 naming and spec for h-cdio (CD, movies, MDEC backgrounds, 3D, mouse)
status: In Review
assignee: Sonnet 5.5 analyst (workflow p2a)
created: 2026-10-24
updated: 2026-10-24
links: [[[tickets/T-27000-phase-2]], [[spec/h-cdio]]]
---

## Goal

Name the nine main-exe files of the hardware layer unit h-cdio at module level and write the port spec for it. This pass writes tables and wiki pages only: no change to `src/`, `include/` or the symbol files.

Files: `src/main/800451D0.c`, `80046500.c`, `80056300.c`, `800563F0.c`, `80057390.c`, `80058D20.c`, `80059710.c`, `80059A20.c`, `80059B40.c` (80 functions).

## Acceptance criteria

- [x] `config/renames/proposals/h-cdio.tsv`: one row per rename (kind scope old new note), a comment row for each O.BIN-named function, hypotheses marked, one name per (unit, address), no collision with names already in the repo.
- [x] `wiki/spec/h-cdio.md`: what the modules do, state and data, rules, connections, open questions; linked from `wiki/spec/index.md`.
- [x] Facts only, no game text, no asm, no screenshots, nothing from `disc/`.
- [ ] Review by the orchestrator, then merge the table into the rename tool input.

## Notes

- Evidence: the matched C, callers (index over `src/`), `wiki/ref-english-patch.md`, `wiki/disc-layout.md`, and three short emulator runs from a cold boot (`tools/emu/run.sh break` and `trace`): `cd_command` ids at boot, which unit functions run in 3600 frames, the movie and XA clip calls up to frame 7500.
- Prefixes used: `cd_`, `snd_` (as the audit proposes for main), plus `xa_`, `movie_`, `model_`, `display_`, `boot_`, `iris_`, `mouse_`, `debug_`; O.BIN names (`str*`, `dec_bg_*`, `init*`, `*Mouse`) kept with English comments.
- Overlaps to merge with other units: GameState fields `unk_000/002/004/014/018` (display size and centre; the graphics unit may also name them), mode constants `MODE_BOOT`, `MODE_LOGO_MOVIE`, `MODE_TITLE`, `MODE_EXIT_TO_SHELL` (the core unit owns the full mode enum), type `TaiikuBig` (shared type in `main_api.h`).
- Found while reading: `D_800B5900` (movie flags) starts at 1 and nothing in `src/` writes it, so the 15-bit movie branch with its caption timing is not reached in the shipped data; `initLight`, `initCoordinate`, the mouse read functions and two CD poll helpers have no caller in `src/`.
- `[[tickets/T-27000-phase-2]]` is the parent; it is not in this worktree.

## Comments
- 2026-10-24 (Sonnet 5.5): table (200 rows: 53 func, 49 data, 37 const, 27 comment, 17 field, 9 file, 8 type; 8 hypotheses) and spec written, In Review.
