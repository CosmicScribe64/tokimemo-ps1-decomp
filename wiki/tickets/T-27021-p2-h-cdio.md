---
id: T-27021
title: Phase 2 naming and spec for h-cdio (CD, movies, MDEC backgrounds, 3D, mouse)
status: In Review
assignee: Sonnet 5.5 analyst (workflow p2a)
created: 2026-10-24
updated: 2026-10-24
links: [[[spec/h-cdio]]]
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
- Overlaps with other units are listed in [[spec/h-cdio]] section 11: `unk_014/018` take the graphics unit's `draw_origin_x/y`; `MainMode` rows were dropped (the core unit owns the enum); 18 rows (CD functions, `dec_bg` data, `unk_000/002/004`) have a different core name and are for the merge to settle; type `TaiikuBig` (shared type in `main_api.h`).
- Found while reading: `D_800B5900` (movie flags) starts at 1 and nothing in `src/` writes it, so the 15-bit movie branch with its caption timing is not reached in the shipped data; `initLight`, `initCoordinate`, the mouse read functions and two CD poll helpers have no caller in `src/`.
- The parent ticket T-27000 is not in this worktree, so it is named in plain text.

## Comments
- 2026-10-24 (Sonnet 5.5): table (200 rows: 53 func, 49 data, 37 const, 27 comment, 17 field, 9 file, 8 type; 8 hypotheses) and spec written, In Review.
- 2026-10-24 (Sonnet 5.5, fix pass after review): table now 186 rows (53 func, 49 data, 24 const, 16 field, 9 file, 8 type, 27 comment; 10 rows carry a HYPOTHESIS mark). Fixed: data `D_800B5944` is `dec_bg_wipe_count` (the name `dec_bg_wipe_step` stays on the function); enum of public command ids is `CdCommandId` (the ring struct keeps `CdCommand`, as in core); the fade draws were misdescribed, so `func_80057D28` is `dec_bg_fade_draw` (4 sprites, shown buffer) and `func_80058398` is `dec_bg_crossfade_draw` (6 sprites, shown and hidden buffer), data `dec_bg_fade_amount`, setter `dec_bg_fade_set`, purposes marked HYPOTHESIS; `unk_014/018` are `draw_origin_x/y` with the real values; retry note (3rd, 7th, 11th failure); `models_update_draw` and `model_link_tmd_object` notes; spec nits (volume ends at 7, four debug counters, full reset call list, libgs flag note, `initView` comment row, `movie_` prefix reason); dropped the no-op `active` field row, the `CdQueueCmd` and `MainMode` const rows. No src, include or config edit.
