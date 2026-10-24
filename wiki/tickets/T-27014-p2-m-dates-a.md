---
id: T-27014
title: Phase 2 naming and spec for m-dates-a (src/main/80079B10.c, which is the sound driver)
status: In Review
assignee: Sonnet 5.5 analyst (workflow p2a)
created: 2026-10-24
updated: 2026-10-24
links: [[[tickets/T-27000-phase-2]], [[spec/m-dates-a]]]
---

## Goal

Name every function and global of `src/main/80079B10.c`, the fields and types it relies on, and write the port spec. The work list called this unit "date sequences, first half" (`normal_date_*`, girl selection, portrait loads, event faces, 165 functions); the file is in fact the game's sound driver (54 functions, 0x80079B10 to 0x8007C030). The date and girl functions are in `src/main/8007C030.c` (unit m-dates-b). This pass writes tables and wiki pages only: no change to `src/`, `include/` or the symbol files.

## Acceptance criteria

- [x] `config/renames/proposals/m-dates-a.tsv`: one row per rename (kind scope old new note), a comment row for the O.BIN-named function that is applied in the repo (`SD_InitCDLevelInfo`), hypotheses marked with `HYPOTHESIS:` first in the note, one name per (unit, address).
- [x] `wiki/spec/m-dates-a.md`: what the module does, state and data, the command protocol, rules (fades, voice clips, banks), connections, open questions; linked from `wiki/spec/index.md`.
- [x] Facts only, no game text, no asm, no screenshots, nothing from `disc/`.
- [ ] Review by the orchestrator, then merge the table into the rename tool input.

## Notes

- Evidence: the matched C, its callers (every literal `func_80044750(...)` argument in `src/`), the libsnd asm in `asm/libsnd.s` (internal labels name the library calls), the O.BIN developer names (`SD_*`, med/low alignment; all 49 unapplied ones agree with the code), and five short cold-boot emulator runs (`tools/emu/run.sh break func_80079B10`, `watch 0x80125D10:8`, `break func_8007A924`, `break func_8007A6AC`, `break func_80090D60`): command stream of the first 5100 frames, flag-word writes, the state dump at a voice clip start, the clip request words.
- Prefix `snd_` as `core` proposes (the three tables disagree on `snd_` / `sound_` / `bgm_`; `core` and `h-cdio` use `snd_`). The 49 unapplied O.BIN names are quoted in the notes, not used as the new names; only `SD_InitCDLevelInfo` (applied) is kept.
- Found while reading: `func_80079E00` ignores its argument (the original asm never reads `a0`), so the bank-ready wait tests only the BGM slot; command ids 0xB2/0xC2 do nothing; the mono command (1) has no sender; the voice pause the game sends (class-0 0xE4) is a cancel, not a pause or a fade-and-stop: the command calls `snd_xa_promote_next` right after `snd_xa_control`, which clears the paused bit and the fade request, so a pause during playback only makes the driver forget the clip (no stop is issued) and a pause during the CD start stops the CD at once; 0xF4 never resumes (spec section 8.4; read from the C, not run in the emulator, the RPG_BAT sender timing is a HYPOTHESIS); a command word of 0 ends the queue drain; the XA clip arithmetic was verified to the sector on a real request.
- Shared names: `snd_command_pump` and `audio_state` are core's; `CdLevelWork` (`CdLevelMeter`) is shared with `8007C030.c`.
- The parent ticket T-27000 is not in this worktree.

## Comments
- 2026-10-24 (Sonnet 5.5): table and spec written, In Review.
- 2026-10-24 (fix agent, review round 1): reviewer finding fixed. The pause/resume description (table rows `SND_CMD_PAUSE_XA`, `SND_CMD_RESUME_XA`, `SND_STATE_XA_PAUSED`, `snd_xa_control`, `snd_xa_promote_next`, `xa_resume_pos`; spec sections 3.1, 3.3, 4, 8.4, 12, 15) now follows the class-0 path the game uses. Also fixed: `snd_init` caller (`func_80042134`), the `snd_xa_check_end` note, the command 5 and bank-set claims (HYPOTHESIS marks, six of ten command 5 senders sit next to a bank load), the `SsSetTickMode` row. Left for the orchestrator: ownership of `normal_date_*` and related code (unit label), O.BIN naming policy, kanban card for this ticket.
