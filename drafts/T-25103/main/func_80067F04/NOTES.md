# T-25103: main func_80067F04 (2452 B, 613 words)

Result: matched. `tools/docker.sh python3 tools/funcdiff.py func_80067F04 --unit main --resolve --version default`
prints MATCH with the C body in `src/main/800674B0.c` (no guard, no INCLUDE_ASM line). `body.c` here is the same
body with its two macros.

## Start state

Plain `INCLUDE_ASM`, never attempted by hand (T-11690 skipped it). The only drafts were raw m2c output
(`drafts/T-14500`, `drafts/T-17000`, census score 1019). I wrote a new body from the asm.

## What the function does

It fills 32 display records `D_8011ECD0[0x61..0x80]`, then adds calendar marks (frame `unk_18`, x `unk_26` from
`func_80068898(side, day)`, y `unk_2A` from `func_800688F0(side, day)`) for: the date word at `+0xF8`, the eleven
Rec38 dates with flag bit 6 set, the eight active `+0x54C` entries, two 0x6F marks when `unk_0F4 >> 12` is 6..10,
the day-schedule frames `D_800B5C10[unk_044[side].unk_20[i + 1]]` of both months (0x66/0x6C/0x67/0x6F start a
6- or 7-day run that repeats `kind` on the following empty days), and the `+0x75E` records of this year and month.
It stops at mark 0x3F (`n + 0x61 >= 0xA0`). Side 0 is this month, side 1 next month.

## Measurements (funcdiff --resolve rows, `-`/`+` lines)

| body | rows | change |
|---|---|---|
| v1 natural draft | 489 | 574 words; frame 0x78; F/G with one shared mark block |
| v2 | 519 | F/G: separate mark blocks for the 0x6F arm and the else arm (as m2c shows); two more locals (frame 0x80, `kind` at sp+0x68); `year` local in the +0x75E loop. 610 words, every block shape right |
| v3a/v3b `(s32)` casts on the month reads | 519/536 | no effect: the `cvt` stays shared |
| v3c volatile read in test 1 (diagnostic) | 529 | the second test reloads as in the original and the init loop matches: proves the cause |
| v3d `month == gs->unk_03F` in the first test of each pair | 16 | the cause, fixed without a FAKE |
| v4a mark statements on one line | 8 | as1 puts the frame store after `move a1` |
| v4b `D_800E6280.unk_03F + 1 == month + 12` | 6 | `bne t8,v0` operand order |
| v5a `left = 0; n = 0;` before the date extraction | 0 | MATCH |

## The causal chain

1. **First divergence (word 25):** the init loop kept the constants 8 and 0x62 in `$s7`/`$fp`; the original
   builds them with `li` each iteration. regdiff showed that these were split pieces of whole-function constant
   webs.
2. **Why they were split:** in sections B, C and D each pair of tests reads the month twice:
   `month == unk_03F`, then after the first mark `month == unk_03F + 1` (C also tests `+ 12`, D uses `% 12 + 1`).
   With absolute `D_800E6280.unk_03F` in both tests, uopt shares `cvt(unk_03F)` (PRE): it computes `+1` in the
   first test's delay slot and reloads it at the end of the mark block. The original reloads at the second test.
   With the absolute read, the extra ranges change the colouring of the whole function, and that is how the init
   loop constants got the saved registers.
3. **Fix:** the first test of each pair reads the month through the `GameState *gs = &D_800E6280` pointer
   (`gs->unk_03F`), the second test through the absolute name. uopt folds the pointer to the same absolute
   `lui/lbu`, but it is a different expression (ilod of a constant address against lod of the global), so there is
   no PRE. The same `gs` pointer is the spilled `sp+0x7C` local the loop bounds `gs->unk_044[k].unk_00[0]` read.
   The original mixes the two spellings; this is not a FAKE. 519 rows to 16.
4. **Line grouping:** each mark (three record stores plus `n++`) expands on one source line through the
   `CAL_MARK` macro. Where the record address is a constant (n = 0, and the second 0x6F mark at `s0 + 0x44`), as1
   then puts the frame store after the argument copy, as the original does.
5. `left = 0; n = 0;` before reading the date word puts `move s5,t4` (day) in the branch delay slot.

## FAKE left

- `s32 pad;` between `left` and `kind`: the original has `kind` in the sixth local slot (sp+0x68) and an 0x80
  frame, with the call-spilled walk temporary at sp+0x50. Without it: 10 rows (kind at 0x6C); with `year` moved
  into that slot: 4 rows (the spill goes to 0x54). The real local is unknown.

## Negatives

- `(s32)` on either month read does not split the shared `cvt` (v3a 519, v3b 536).
- A volatile view of the first read works for the init loop but builds the full address (`addiu` + `lbu 63`).

## Lesson for this file

Where the original rebuilds a global read that our draft shares across calls or tests, a `GameState *gs`
pointer read at one of the two places gives a separate expression. `func_8006B0C8`'s notes describe a related
`&D_800E6280` register range the original never has; this spelling is worth a try there.
