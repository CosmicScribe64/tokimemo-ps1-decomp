# T-25151: GYOZI func_801412A4 (172 words, 684 bytes)

Result: **matched**. The default build compiles the C in `src/ovl/GYOZI/80140C90.c` (commit
`T-25151: match GYOZI:func_801412A4`). `match.c` is the body without its comments.

## Start state

`INCLUDE_ASM`. Best known body: T-25012 autotweak, 96 rows, kept here as
`seed-T-25012-best-autotweak.c`. It had a `u16 count` (gives `sltiu` instead of `slti` and an
`andi 0xFFFF`) and an `if (flag) {}` FAKE that spills the flag to a 0x30 frame.

## What was measured and changed

Started again from the matched sibling `func_80140F90` (same count loop, same retry loop without the
debug call, same tail) instead of from the seed. Rows are `funcdiff --resolve --version c` lines.

| Step | Change | Rows |
| --- | --- | --- |
| 1 | Donor shape, `D_800F6468 * 1 + i` (the donor's FAKE) | 276: the word load is hoisted out of the retry loop |
| 2 | The word read as the member `D_800F53A0.unk_10C8 + i`, no `* 1` | 19 |
| 3 | `D_801486E4` read once into an s16 (a separate `s16 sel`, or the reused counter `i`) | 10 |
| 4 | `(s32)D_800F53A0.unk_10C8 + i` | 8 |
| 5 | Count-loop body on the `for` line | 0, MATCH |

Causes, one each:

- **Hoist (step 2).** A direct `D_800F6468` (separate symbol) and any `* 1` form let uopt hoist the
  `lw` before the loop. The member read through `D_800F53A0` may alias the `unk_F2F` byte store, so
  it is reloaded every iteration, as in the original.
- **Requested girl (step 3).** The original loads `D_801486E4` once into `$v0`. Repeated reads give
  `lbu v1` plus `move v0,v1`. s32/u8/u32 locals give the same instructions with `beq v0,a2` (operand
  swap), 12 rows. Only an s16 local gives `beq a2,v0`. The source reuses the s16 counter `i`.
- **Sum operand order (step 4).** `addu t8,t7,v0` (load first) needs a signed word. `u32 + s16`
  puts the counter first whatever the source order (`i + x` too). `(s32)` cast or `*(s32 *)&`
  view: 8. `| 0`: 8. `^ 0`, `<< 0`, `(u16)i`: hoist again (159 to 209). `(s32)i`, `(u32)i`,
  `i * 1`, `(u32)(x + i)`: 10. `D_800F6468` itself is s32 (main_api.h). The header field stays
  `u32` because `801418B0.c` needs unsigned `% 4` and `/ 10` on it, so the cast is marked FAKE.
- **Loop-1 order (step 5).** The ugen listing was identical. as1 orders a block by source line:
  with the body on its own line, `slti` moves above `sra t4` and the count copy fills the delay
  slot. With the body on the `for` line, the original `sra t4; slti; move a0,t4; bnez; move v0,t6`
  comes back. Other forms: body on its own line but `}` on the body line, 8; `count = 0;` on the
  `for` line, 12; `do {} while (++i < 11)`, `do { ...; i++; } while (i < 11)`, `girl[i++]`, 4 each.

## Negatives added

- `count += (b1 == 0) ^ 1` and `count += !!b1` with the hoisted load (276/258/278 rows). These
  are not clean single-cause tests because they ran on the hoisted body. In the final body the
  donor's `b1 >= 1` gives the right `sltiu; xori` and ring slots.
- Direct `D_800F6468 + i` / `i + D_800F6468`: hoisted, 209.

## Tools

`try.py` installs a body under a `NON_MATCHING` guard and counts rows. `sweep.py` runs template
variants (placeholders `@KEY@`). `sbs.py` (Docker) prints the side-by-side resolved diff; `ccs.py`
(Docker) prints the ugen `cc -S` listing. All run from the worktree root.
