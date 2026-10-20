---
id: T-24204
title: "Cloud packet: main func_8006D7EC"
status: Done
assignee: cloud
created: 2026-10-20
updated: 2026-10-20
links: ["[[cloud-matching]]", "[[matching-notes]]", "[[tickets/T-23003-w5-big-p03]]", "[[tickets/T-24001-main-schedule-final-row]]"]
---

## Goal

Match `func_8006D7EC` in `src/main/8006CB30.c` (4,692 B) from a match packet in a cloud session ([[cloud-matching]]). Start: the T-23003 guard at 1 row, the first `idx++` after `idx = 0` folded to `li v1,1` where the original keeps `addiu v1,v1,1`.

## Acceptance criteria

- [x] Single-function setup: asm fallback agrees with the packet reference; guarded C measured.
- [x] Plain C if it matches, else the guard comment updated (T-24204, row count, reason).
- [ ] Local linked build proves the result (funcdiff MATCH of the default build, SHA1).
- [ ] Review against CODING_STANDARDS.md.

## Notes

Cloud recipe of [[cloud-matching]]; packet saved outside the repo, `asm/` gitignored, nothing of it committed.

## Comments

### Result (2026-10-20, cloud)

No match. The guarded C stays as it is; only the `NON_MATCHING` comment now names T-24204 (1 row, same reason).

- Setup: scratch copy with every other `INCLUDE_ASM` removed, reference object from the packet `.s`. The asm fallback is equal to the reference except the three trailing alignment nops of the scratch reference object; `-DNON_MATCHING` (prepended `#define`) gives 1,173 instructions with one differing row, `addiu v1,v1,1` expected, `li v1,1` built (row 95 of the scratch diff, `0x8006D964` in the linked build per T-24001).
- Full-function variants, all still the one row: the 0xFE store through a literal constant address (`*(u8 *)0x800E691C`, rejected anyway: changes the `lui` scheduling, 1,515 diff lines), `gs->unk_69C[idx++]`, `D_800E6280.unk_69C[idx++]`, `gs->unk_69C[0]`, increment before the store; the month-boundary block wrapped in `do { ... } while (c)` with `c` = `monthOff == 0`, `monthOff != 1`, `monthOff < 1`, `day != 0`, `0` (uopt deletes the exit test and still folds).
- Probes on a 15-line test file with the same shape (`idx` address-taken, zero set after a call, increment in an `if`): uopt folds in every form tried: duplicated or self-assigned zero, `register`, `long`, ternary/`-= -1`/`|= 1`/`^= 1`, zero reaching through an `if/else` or `switch` phi of equal constants, 2/3-member struct or array member, union with overlapping `short`/`char[4]` members, zero from `d - d`, `d ^ d`, `d & 0`, `d * 0`, `d != d`, `(u32)d < 0U`; unreachable backward `goto` (after `return`, `if (0)`, `while (0)`, or under `c != c`, `d - d`, `(d & 0) != 0`, `c ^ c`) is removed before the fold. Only a live backedge keeps the `addiu` (T-24001), and the original has no branch target before the `bne` of the test, so no loop form can match.
- Reading: uopt here proves the zero through everything a source form can express; the original either had a value uopt could not see as constant that still materialised as `or v1,zero,zero`, or came from a uopt build that did not propagate this case. Next step, if any: compare against another IDO 5.3/7.1 uopt build ([[toolchain]]) rather than more source forms.

## Review notes

Self-check against CODING_STANDARDS.md: one comment line changed in `src/main/8006CB30.c` (guard and FAKE comments of T-23003 unchanged, default build still `INCLUDE_ASM`); no asm, packet or game data committed. Needs the local linked build and an integrating review.

- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): cloud branch changed only its C file and wiki, no asm or game data; merged with a clean 27/27 build and header/global/prototype checks. No match; the guard stays with an updated note. Done.
