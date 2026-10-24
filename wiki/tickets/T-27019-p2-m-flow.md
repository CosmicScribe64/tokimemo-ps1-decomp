---
id: T-27019
title: Phase 2 naming and spec for m-flow (boot, main loop, mode machine, overlay loader, title, pad, memory card)
status: In Review
assignee: Sonnet 5.5 analyst (workflow p2a)
created: 2026-10-24
updated: 2026-10-24
links: [[[tickets/T-27000-phase-2]], [[spec/m-flow]]]
---

## Goal

Name the game-logic functions and globals of the main-exe flow unit and write the port spec for it. This pass writes tables and wiki pages only: no change to `src/`, `include/` or the symbol files.

Files: `src/main/80041000.c`, `800420D0.c`, `80042540.c`, `80042A00.c`, `800789E0.c`, `80061710.c` and `80053650.c` (115 functions): graphics bring-up and `game_state_init` with the new-game data, the executable entry and frame loop, the mode dispatcher and setters, pad and mouse polling with the soft reset and the secret code, the overlay loader with its sector and entry tables and the TT loader, the title screen with its 3D card intro and the start mode, the memory-card layer with the system-file and game-save images.

## Acceptance criteria

- [x] `config/renames/proposals/m-flow.tsv`: one row per rename (kind scope old new note), a comment row for each O.BIN-named function touched, hypotheses marked, one name per (unit, address), no collision with names in the core, h-graphics, h-cdio, m-hud, m-scheduler, m-dates-a tables or the sibling worktree tables; `tools/rename.py plan` on the func, data, field and type rows reports no problem.
- [x] `wiki/spec/m-flow.md`: what the unit does, state and data with the new names, rules as coded, connections, open questions; linked from `wiki/spec/index.md`.
- [x] Facts only, no game text, no asm, no screenshots, nothing from `disc/`, `versions/` or `build/`.
- [ ] Review (an Opus pass checks the table and the spec against the C; no `src/` change, so no `CODING_STANDARDS.md` findings).
- [ ] Merge the table into the rename tool input (not part of this branch).

## Notes

- Evidence: the matched C and every caller in `src/`; the core table and spec (branch p2a-core, merged into the base), the h-graphics, h-cdio, m-hud, m-scheduler and m-dates-a tables, the sibling m-sysmenu table (for the shared `CardResult` names), and the emulator (`tools/emu`: states made with `boot` to the opening and to the title; probes: mode byte and stage writes through a whole title run, the loader read and state changes, the pad code, the soft reset, the card check frames, initial values of the calendar, player and girl records).
- Main findings: the title is a 3D intro (13 photographs over a still) with a 601-frame wait, a flash, a logo and a two-item menu, and an 18-step pad code at the menu jumps to the opening movie; the overlay sector and entry tables are decoded in full (48 overlay modes, 24 disc files, EVENT and GYOZI are not in them); the loader skips the read when the same file is resident, so overlay statics survive mode changes; the memory-card layer is a poll-per-frame state machine over two banks with a byte-sum check, and both save images are listed field by field; the entries of modes 0x74-0x76 in core's `MainMode` rows are in a different order than the loader code has them (question 1 of the spec).
- Naming: core names are used as given; new prefixes `boot_`, `timer_`, `sys_`, `pad_`, `mouse_`, `ovl_`, `title_`, `card_`, `mdec_` (`card_` instead of the ADR row's `memory_card_`; `timer_` moves to the 800420D0 row, see spec section 1). Three rows carry HYPOTHESIS marks. The hardware calls (libgs, libgpu, libcard, libpress) are left as `func_` placeholders; the two MDEC helpers and the card events are named at module level.
- Not done: no SDK placeholder was renamed; the alias labels inside actors and layers that the title writes (`D_801206xx`, `D_801220E0`) stay for element access; the frame at which the card jobs end in a real save was not run (the emulator's card check keeps retrying).

## Comments

- 2026-10-24 (Sonnet 5.5): table (324 rows: 87 func, 48 data, 44 field, 1 type, 10 comment, 14 file, 120 const; 3 HYPOTHESIS rows) and spec written, In Review.
