# T-25103: main func_80067870 (952 B, 238 words)

Result: not matched. Best body `body.c` here, 75 rows with
`tools/docker.sh python3 tools/funcdiff.py func_80067870 --unit main --resolve --version c`
(guarded in the file). `src/main/800674B0.c` keeps plain `INCLUDE_ASM` (over the 30-row guard limit).

## Start state

`INCLUDE_ASM`. The newest draft was `drafts/T-17002/main/func_80067870/body.c` (T-17002 said 244 "rows"; that
was diff lines). It still compiles against the current headers and measures 122 rows today. Its globals are the
current `main_api.h` names (`D_8011F414` etc. are `D_8011ECD0[0x1B/0x1C]` fields; `--resolve` compares addresses).

## What the original does (read from the asm)

- `D_800B5C08` (s16) is one uopt range in `$a3` for the whole function: three loads, all `lui a3; lh a3`, the call
  argument is `move a0,a3`.
- `D_80125C54` (u8) is `$a2` everywhere: `lui a2; lbu a2` in bb8 and after both calls, `addiu a2,a2,-2` in the
  case. There is no separate cvt range.
- `D_80125C52` value `$a0` from the block after `SquareRoot0` into both 0x2 cases; `unk_F80` (`D_800E7200`) `$a1`.
- `15 - SquareRoot0()` is a ugen temporary (`t8`), used twice: `sll t2,t8,16` (dy) and `subu t9,t1,t8` (store).
- x1 `$t0`, y1 `$t1`, dy `$t2`; `D_80125C55` in bb10/11 is a ugen temporary (`t4`, decrement `t5`).

## Changes that held (122 to 75 rows)

| change | rows | why |
|---|---|---|
| `((-dx * 2) / 3)` for `((dx * -2) / 3)` | 122 to 113 | original `negu t3,t9; sll t4,t3,1`; `* -2` expands with `negu at` and spends one ring slot less |
| `D_80125C55--` for `-= 1` | (149 alone, needed) | `cvt_pass` widens no `dec`: no cvt(D_80125C55) range in `$v0`, byte in `t4/t5`, `unk_10F8` in `$v0` as in the original |
| `D_8011F46A = (u32)D_80125C52;` (FAKE) | 149 to 100 | fourth use of cvt(D_80125C52): prio 1.333 to 1.667, coloured before the sub phantom, takes `$a0` |
| `x1 = dx + D_80125C50;`, 0x8-case bodies joined on one line (autotweak) | 100 to 76 | ugen ring order; whole tail (words 166..237) now equal |
| drop redundant K&R casts (`3U`, `0xC4U`, `(u32)` on the shift, `(s32)` on unk_10F8) | 76 (no change) | K&R already promotes u8 to unsigned |

## The blocker (measured with the R9 trace, `build/scratch/me/units.py` style dumps of `draft.log`)

Colouring order in the 75-row body: bit41 v1, bit37 a1, x1-add phantom v0, y1-add phantom v1 (both block-local
in the x1/y1 block), and(unk_F80) v0, cvt(D_80125C54) a2 (2.375), cvt(D_80125C52) a0 (1.667),
**sub(15,ret) a1 (1.5)**, unk_F80 a3 (1.333), D_800B5C08 whole range: no caller register left, cost 13.1 at s0,
split (entry piece a0, later piece uncoloured, so word 14 loads it into ugen `t6` and every ugen temporary is one
register late up to word 97).

The sub range exists because `D_8011F426 = y1 - dy` is copy-propagated to `y1 - sub` (cvtl dropped for the 16-bit
store) and the T-17002 empty test splits its two uses across blocks. Its colour is never emitted (the code uses
`t8`), but it blocks `$a1`. Measured consequences:

- Without the empty test the sub is block-local, prio 3, and is emitted in `$a0` (`subu a0,t9,v0`).
- Moving the test anywhere else in the block, or splitting before the `unk_F80` test, breaks the D_80125C50/52
  CSE (extra reloads, 237..245 words).
- Three empty tests in a row give the sub D=3, prio 1: then unk_F80 `$a1` and D_800B5C08 `$a3` are right
  (words 0..19 match), but the sub takes `$t0` and x1/y1/dy move to t1/t2/t3 (119 rows).
- An always-false redefinition `if ((u32)D_80125C54 < 0) { dy = 0; }` removes the sub range: D52 a0, unk_F80 a1,
  D_800B5C08 a3 all right, but dy is then narrowed in place (`subu t2; sll t3,t2; sra t2,t3`, no `move`), one
  word short (235), 173 rows.
- `y1 - (s32)dy`, `(s16)(y1 - dy)`, `(u16)dy`, `dy & 0xFFFF`, chained assignments, s32 dy, a separate s32 temp
  (`h`), the call result in y1: no gain or worse.

Second, independent gap (T-0018 promo): `D_80125C54`'s global range (`$a3`) and cvt(D_80125C54) (`$a2`)
interfere (the global is reloaded in bb13/bb16 to feed the cvt), so the original's single `$a2` cannot be
reproduced while `cvt_pass` keeps the widening on arithmetic uses (mpy, add). An entry dead read
(`y1 = D_80125C54;`) only removes the compare CVTs and gives 215..220 rows. About 12 words depend on it
(bb8 `lbu/addiu`, the `lui` bases after both calls, case `addiu/andi`).

## Negatives added (on the 100- and 76-row bodies)

Empty tests on D_800B5C08 at any of 5 places; `if (D_80125C55)`; nested if for the decrement; `(u8)`/`(u32)`/
`(s32)` casts on D_80125C54 arithmetic; `(D+1) >> 1`, `1 + D`; `(s32)` on the y1 sums (139, fixes D_800B5C08/unk_F80
but loses the D52 range, reloads in the cases); empty tests on dx, dy, x1, D_80125C50/52, unk_F80 in place of the
T-17002 test; multi-block tests (`&&`, `||`, `else`); dy/x1/y1 statement reorders and one-line joins.

Also negative: a T-25054-style `do { } while ((u32)X < 0);` boundary after the dy statement or around it
(X = D_80125C54 or unk_F88): the sub keeps D=2, prio 1.5, `$a1`; 84..236 rows, an extra branch word in most.

## Next hypothesis

Get rid of the sub(15,ret) range without changing the emitted dy sequence. The original's code is exactly the
phantom shape (sub in t8, `sll t2; sra t3; or t2,t3`, store from t8), so look for a construct that keeps the
copy-propagated store but makes the sub's range ineligible (priority <= 0, or every caller register costing more
than priority*D = 3). Or lower it below dy (0.25), which empty tests cannot do (D grows by one per four blocks).
If that works, D52/unk_F80/D_800B5C08 fall into place (the three-test variant showed it) and only the
D_80125C54 promo rows (about 12) remain, which need the T-0018 pass gap closed (no source lever found).

Scratch tools used (not committed): a side-by-side compile of the guarded unit (`funcdiff_verify.compile_unit` +
objdump, about 1.5 s per variant) and a parser for regdiff's `draft.log` R9SAVE/R9UNIT/R9COST lines that prints
each coloured range with its blocks and register; that table is what found the sub phantom.
