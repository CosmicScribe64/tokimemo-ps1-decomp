---
id: T-20003
title: Match RPG battle timer callbacks with the required callee views
status: In Progress
assignee: Codex
created: 2026-10-16
updated: 2026-10-16
links: ["[[matching-notes]]", "[[rpg-bat-family-results]]", "[[tickets/T-17101-w3-rpg-bat-p01]]"]
---

## Goal

Test the three RPG_BAT callbacks suggested by the latest T-17101 matching notes: func_80138F6C (src/ovl/RPG_BAT/80137280.c), func_80146C30 (801468B0.c) and func_80157158 (80156CC0.c). Reuse the archived typed drafts, inspect them against assembly, and try the existing integer-return callee overrides for func_8013E7C0, func_8013E97C and func_8013F15C.

## Acceptance criteria

- [ ] Only verified default linked matches replace production INCLUDE_ASM.
- [ ] Full RPG_BAT checksum and all 27 binaries pass, with header/global/prototype checks.
- [ ] Local standards review and a focused upstream PR containing verified work.

## Notes

Branch codex/rpg-timer-callees starts from upstream main 68650e14. T-17101 is Done and explicitly suggests these three follow-ups; no T-17102..T-17119 tickets exist in the fetched upstream checkout. Source drafts live in drafts/T-17100/_runs/typed/RPG_BAT/. All tools run in Docker. TT remains excluded.

First result: func_80157158 matches 1,256 linked bytes in the actual default build; full RPG_BAT SHA1 passes. Its entry selector must be reread from D_8015EDD8 after the setup/effect calls instead of preserving m2c's local snapshot. The required integer-return views are func_8013D030 and func_8013F15C; the former gets a guarded overlay prototype, and the latter uses its existing guard. Direct global timer updates/decrement remove the draft's stale locals. Splitting D_8015EDC4's flag update into &= 0xFFFF0000 then |= 3 promotes the word into the original v0 instead of a ugen temporary. Two unused frame words reproduce the 0x40 frame and spill home (explicit FAKE, T-20003). All source is ordinary C89, with no new compiler rule or recovered aggregate.

Evidence: build/T-20003/func57158-proof.log and rpg-check.log (gitignored); actual default funcdiff MATCH, 1,256 bytes, RPG_BAT.bin SHA1 OK. Other callbacks remain scratch-only while evaluated.

Second result: func_80138F6C matches 1,268 actual default linked bytes. Canonical in-place timer increments and reading the timer again remove m2c's stale locals; the only remaining 28 differing listing lines were D_8015EBAC in v0 rather than v1. A same-width signed-word pointer read in the inner switch makes the complete selector web use v1 (explicit FAKE, original spelling unknown). Two unused frame words preserve the 0x40 frame and spill home (FAKE); an unused m2c tail variable was removed without changing the match. The text destination D_8015E8A0 is declared once as an unsized byte array in RPG_BAT.h, like the neighboring buffers. No callee override is needed for this callback.

While this work ran, upstream main advanced to fa1dffb4 and matched the third target, func_80146C30, in T-17104. Its local probes were not retained or submitted; integrate the upstream match instead of duplicating it.

## Comments

Implementation and verification pending.
