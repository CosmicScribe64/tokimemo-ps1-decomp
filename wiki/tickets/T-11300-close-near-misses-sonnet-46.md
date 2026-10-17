---
id: T-11300
title: "Close near misses (Sonnet) 46"
status: Done
assignee: s46
created: 2026-10-14
updated: 2026-10-17
links: ["[[decompile-workflow]]"]
---

## Goal

Close near-miss functions from the s46 list (33 functions, closest first).

## Acceptance criteria

- [ ] Matches committed one by one; remaining non-matches guarded or restored.
- [ ] Clean ninja 27/27 OK.

## Notes

## Comments

- 2026-10-14: matched EN_NICHI func_80137C58 (u8 `one`/`two` locals for bne operand order, FAKE) and TACO func_80146D60 (index loop). TACO func_801471D0 rewritten as index loops, 4 lines off (folded arg0 copy before the second loop), still guarded. The autotweak loop committed two in-progress edits (markers, stray RPG_BAT edit); both reverted in follow-up commits, and the loop was stopped. funcdiff `--resolve` pairs two HI16 relocs of one symbol with the wrong LO16 and reports a false `lui` diff (TACO func_80146D60 links sha1 OK).

2026-10-17: Cleanup pass. Merged: EN_NICHI func_80137C58 (FAKE u8 locals, ticket named) and TACO func_80146D60 matched; TACO func_801471D0 was left guarded at 4 lines and matched later under T-19015. Three commits titled for other functions (10f71297f, 761ec0082, 3362d488b) carry edits swept in by the autotweak runner; the ORDER markers and the RPG_BAT edit were reverted and no marker is left in src/. Review against CODING_STANDARDS.md: no blocking finding, one redundant cast in func_80146D60 goes to the Backlog cleanup ticket. Closing as Done (integration tree rebuilds clean 27/27 under T-20900).
