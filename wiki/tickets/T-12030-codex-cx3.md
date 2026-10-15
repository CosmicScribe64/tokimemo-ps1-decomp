---
id: T-12030
title: Match cx3 main menu functions
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]"]
---

## Goal

Deep matching pass on the 15 assigned functions in src/main/8005A0B0.c.

## Acceptance criteria

- [ ] Each retained match has exact linked-byte and unit SHA1 verification.
- [ ] Retained near misses are guarded, counted and documented.
- [ ] Final configure/build, headers, globals and prototype checks pass.
- [ ] Code review against CODING_STANDARDS.md before Done.

## Notes

Baseline Docker configure and ninja passed all 27 SHA1 targets (13 before restarting with -j2 and 14 after). Logs: /tmp/cx3-baseline.log, /tmp/cx3-baseline-resume.log. Only the assigned C file, include/config and wiki may change. No main-checkout edits.

## Comments

- 2026-10-15: claimed package cx3; baseline verified.

- Matched `main:join_club_message`: 544 literal linked bytes, production main SHA1 OK. `s32` fall-through preserves the v0 switch copy; scalar-byte selector view preserves v1. The scalar view is marked FAKE. A plain local selector changed the frame and allocation; void lost the copy. Logs: build/scratch/join-final.log and build/scratch/join-build.log.

- `func_8005BE8C`: corrected guarded return type to match its earlier implicit declaration. Three differing instruction rows remain at the final byte-status load (t9 instead of v0). Scalar view and reused local did not help. Autotweak: 262 candidates, unchanged. Permuter: 120 seconds, stripped 160 to 140, no exact match. Original readable draft retained. regdiff works with the existing trace binary copied from the orchestrator's /private/tmp scratch; diagnostic is build/scratch/be-traced.log.

- Matched `main:func_8005D448`: 292 literal linked bytes and main SHA1 OK. Saved draft indexed a fresh selector load instead of the pre-call selection; corrected to the saved selection. A two-word unused array reserves the observed stack home at sp+0x2C (marked FAKE); one word fails four instruction rows. Added the s32 prototype. Logs: build/scratch/d448-final.log, build/scratch/d448-sha.log.
