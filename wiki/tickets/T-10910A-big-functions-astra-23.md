---
id: T-10910A
title: Big functions (astra) 23
status: Done
assignee: Codex a23
created: 2026-10-13
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]"]
---

## Goal

Match the assigned big functions in five owned objects, largest first, with at least four hours of sustained work. Preserve close guarded drafts and reusable type/idiom evidence.

## Acceptance criteria

- [ ] Largest-first deep attempts, regdiff for every register miss.
- [ ] Verified matches and useful guarded near misses; shared type/idiom notes.
- [ ] Final 27/27 build, header/global/prototype checks, inline standards review.
- [ ] Uncommitted changes limited to the assigned files and permitted documentation/declarations.

## Notes

Started 2026-10-13 15:29 UTC. Work list: scratchpad/astra23-list.txt; experiments confined to scratchpad/a23. Prior T-10880 work already supplies several relevant layouts and guarded drafts. No subagents, commits, or shared Docker cleanup.

## Comments

Schedule-builder follow-up: clean baseline passes 27/27. A local u8 year copy restores the original reload behavior and reduces the full-file draft score from 416 to 260 (1177/1173 words). Corrected the signed random gate from equality to <= 0, following the target bgtz. Club counter, statement-order, and result-lifetime experiments are ongoing in scratch.

Added DateMovieScene50 to DATE.h; IDO verified size 0x50 and row offsets 0x24/0x2C/0x34/0x3C. DATE 47958's color stores improve when the raw arithmetic local feeds byte stores directly, removing an unnecessary separate color local. The current scratch candidate is 1014/1010 words, with timer/state restore differences still under investigation. Its decoder-error path returns zero in the target despite the legacy void prototype; no existing header declaration has been changed.

- DATE `func_801462B0`: reconstructed indexed sprite loops, RECT, and the
  animation tail after the main switch. A scratch draft reaches 950/950
  instructions, but differs substantially in branch layout/registers (not a
  match or a retainable <30-line draft). Both callback cases save the selector
  as a word; a byte local introduced an incorrect extra stack-byte spill.
- DATE `func_80147958`: bounded permuter search started from the 1014/1010
  instruction draft while refining the fade and callback live ranges.

2026-10-17 cleanup: nothing was matched; the run ended at the Codex usage limit with a documented DateMovieScene50 layout and workflow notes (merged as e1157f598). Its targets were taken over by T-12100 (DATE 47958 and 462B0 attempts), then T-17201 matched func_80147958. func_801462B0 (950 instructions, no match) and func_8007132C (4.3 KB, census draft only) stay INCLUDE_ASM and are listed in T-16520's sibling clusters and T-14500's draft inventory. Closed as superseded.
