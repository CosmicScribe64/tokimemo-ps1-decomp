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

- Matched `main:func_800603C0`: 324 literal linked bytes, main SHA1/header/global checks OK. Replaced saved pointer arithmetic with indexed byte arrays and recovered the GameState field. D_800E7D24 is a byte-indexed availability array in target assembly; added its declaration. Existing aggregate proposals overlap unrelated regions, so none was adopted. No fakematch in this function. Logs: build/scratch/603c0-final.log and build/scratch/603c0-build.log.

- Matched `main:system_settings_init`: 360 literal linked bytes; main SHA1, headers and globals pass. Four s16[8] arrays replace malformed one-element m2c storage. A loop counter and the real availability result declared before the arrays reproduce their stack positions without padding or fakematch. Logs: build/scratch/settings-final.log and build/scratch/settings-build.log.
- `func_8005A410` initial pass: typed coordinate loads, unsigned state, counted loop and explicit two-point loop tested. Best experiment has 62 diff output lines; end-address hoisting, two-vs-four-way unrolling and spill width remain. Restored assembly; experiments remain in build/scratch/a410-*.log for the deep pass.

- Matched `main:func_8005E150`: 368 literal linked bytes, main SHA1/header/global checks OK. Typed s16[3] arrays and s32 result/counter locals recover the frame. Traced autotweak found an empty counter test before the loop; initialized the counter to zero to avoid the generated candidate's uninitialized read. Initialized test remains byte-exact; removing it or removing the loop reinitialization fails. Marked FAKE T-12030. This resolves the historical T-8080 shifted-loop-register case. Logs: build/scratch/e150-final.log, e150-build.log and e150-regdiff-verbose.log.

- Matched `main:func_8005DEA0`: 376 literal linked bytes; main SHA1/header/global checks pass. Recovered four s16[8] coordinate arrays plus u8[8] colours. An ordinary indexed loop produces the target's induction variables and saved constant; declaring the counter before arrays recovers exact stack offsets. No fakematch. Logs: build/scratch/dea0-final.log and dea0-build.log.

- Matched `main:func_8005FDEC`: 380 literal linked bytes, main SHA1/header/global checks pass. Indexed s16[3] arrays with scalar counter and selector locals reproduce the stack; an explicit unsigned conversion of the signed GameState selector matches lbu. No fakematch. Logs: build/scratch/fdec-final.log and fdec-build.log.

- Matched `main:func_8005F09C`: 400 literal linked bytes, main SHA1/header/global checks pass. Scalar-byte view of the clamped global reproduces v0 and all narrowing; marked FAKE. A proper void prototype removes the extra return-address reload caused by an implicit s32 declaration. Autotweak and eight view variations alone did not fix that exit. Removed unused saved-draft locals. Logs: build/scratch/f09c-final.log and f09c-build.log.

- `func_800603C0` declaration cleanup: the established `FlagsWork24` aggregate already describes the fourteen flags at D_800E7D10+0x14. Reused that view and removed the redundant D_800E7D24 alias declaration. This corrects the earlier aggregate assessment. Exact 324-byte match remains, and the full header-triggered rebuild passes 27/27 checksums. Logs: build/scratch/603c0-aggregate.log and aggregate-full-build.log.

- Matched `main:func_8005BCEC`: 416 literal linked bytes; production main SHA1, headers and globals pass. Two-element coordinate arrays and their real argument accesses recover values/registers. Grouping each width/height pair on one source line fixes the historical T-8080 store-pair ordering. A one-word stack reservation is marked FAKE, as is the grouping purpose. Guarded full-TU proof was normalized-zero but differed literally; default production proof passes, demonstrating interference from other active drafts. Logs: build/scratch/bcec-group.log, bcec-build.log and bcec-final.log.
- `pre_syogatu1`: retained typed buffers and the recovered character bit-field. Scalar declaration placement recovers buffer offsets; 19 exact diff rows remain (frame eight bytes smaller and one cached buffer address instead of separate computations). Traced 180-second search tried 96 candidates without improvement. A proper void prototype also leaves func_8005BE8C's three differing instruction rows unchanged; that experiment was restored. Logs: build/scratch/pre-move.log, pre-autotweak.log and be-void.log.
