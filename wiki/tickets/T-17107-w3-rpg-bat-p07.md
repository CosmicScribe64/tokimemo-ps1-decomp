---
id: T-17107
title: "Wave 3 package w3-rpg-bat-p07"
status: In Review
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Match the RPG_BAT functions of package w3-rpg-bat-p07 (`src/ovl/RPG_BAT/8013E4D0.c`, `8013B5B0.c`): `func_8013B5B0` (928 B, best draft score 109), `func_8013EA60` (1,244 B, 144), `func_8013BBF8` (688 B, 221) and `func_8013E54C` (628 B, 1095).

## Acceptance criteria

- [x] each function matched, or its draft stays guarded with a current differing-line count (or restored `INCLUDE_ASM` over 30 lines): 2 of 4 matched (`func_8013BBF8`, `func_8013EA60`); `func_8013B5B0` guarded at 25 lines, `func_8013E54C` guarded at 16 lines
- [x] unit sha1 OK, clean rebuild 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK
- [x] wiki: matching notes section, t0018 rows where needed, cheat sheet rows, family page, log, kanban, index

## Notes

Worktree `w3-rpg-bat-p07` (branch `w3-rpg-bat-p07`, from `w2-good`). Started 2026-10-16 19:29 UTC, about 80 minutes of work. Result: 2 of 4 matched (1,788 bytes by the census sizes of `func_8013BBF8` 688 and `func_8013EA60` 1,100 linked).

## Comments

### 2026-10-16 matcher run (claude-opus, branch `w3-rpg-bat-p07`)

- `func_8013BBF8` matched: hand-written from the asm (sequence matcher, two `switch (D_8015EB98)` blocks), struct `RpgPadSeq` at `D_8015EBEC` (four words) fixed the load/store order, FAKE `s32 ten = 10;` for the `bne` operand order.
- `func_8013EA60` matched: direct global compound updates plus `RpgGauge` struct view over `D_8015EBC0..D_8015EBD4` (`RPG_GAUGE`). Follow-up (not done, other files): migrate 801504E0.c, 8014DF60.c, 80154430.c and `func_8013C1CC` to a real `RpgGauge D_8015EBC0` and drop the macro.
- `func_8013B5B0` guarded, 25 lines: promoted `unk_1109` in `$v0` instead of `$v1` (T-0018 `U1` family, row added).
- `func_8013E54C` guarded, 16 lines: loop bodies match in the byte-array form, preheader `lui`/`addiu` order differs (row added).
- Code review against `CODING_STANDARDS.md`: pending (ticket stays In Review until then).

### 2026-10-16 closer pass (claude-opus, branch `w3-rpg-bat-p07`, about 35 minutes)

Scope: `func_8013E54C` only. Result: still guarded at 16 lines, no new match.

- `autotweak --time 120 --score-jobs 2`: 512 candidates, no improvement.
- Cause found: ugen hands as1 `la start` (the strength-reduction init, `D_8011ECD0 + 0x110`) before `la limit` (the hoisted loop-test constant `+ 0x440`, followed by the other hoisted constants and the three loads in descending register order). Swapping only those two binasm records and running as1 again reproduces the original's case 3 exactly. `.loc` line changes, the dead `li v1,4` of the counter and the relocation symbols do not change the order. So the gap is in uopt's preheader order.
- About 60 source shapes keep the build's order: base pointer locals (in the case, before the switch, in the `for` head), counter types (`s16`/`u8`/`u16`/`s8` stop the strength reduction), separate or block-scoped counters, `arg0` as counter, labels, `goto` loops, `do { } while (0)`, if-chain, `return`/`default`, a manual 4x body, dead reassignments of `i`, address-expression tests (no unroll), pointer loops (no unroll, no hoisted loads), the `D_8011EDE0` alias base.
- Survey: all 443 C files compiled with a binasm dump. None of the 190 matched loops whose start and limit both come from `la` has the limit first. In the original's unmatched asm, 12 loops in 8 functions show the original's `addiu` order. EN_NICHI `func_80132EE8` has the same one-block preheader, with the limit `lui` in a delay slot.
- Diagnostic scripts: `drafts/T-17107/binasm-order/` (binasm dump, decoder, record editor plus as1, loop scan).
