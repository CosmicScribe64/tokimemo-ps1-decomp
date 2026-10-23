# T-24906 TACO `func_801511C0` (5 rows, unresolved)

Target: `src/ovl/TACO/80149F90.c`, 164 B. Guard unchanged, `src/` untouched this run
(`funcdiff --resolve TACO:func_801511C0` = 5 rows, rows 9-13, the `sw a1,0x3C(sp)` home
store one place late). Time stopped after ~45 min with no row-count progress.

## Scratch harness (all under `build/t24906/`, gitignored)

Single-function pair, ~4.3 s per variant:

- `build/t24906/ref.c`: `#include "common.h"` + `#include "ovl/TACO.h"` +
  `INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_801511C0);`
- `build/t24906/runall.sh BODY...`: writes `draft.c` (`#define NON_MATCHING` + the two
  includes + the body), compiles both with `tools/cc.py ... ido 5.3`, and runs
  `tools/funcdiff.py --built draft.o --expected ref.o --resolve TACO:func_801511C0`.
- `build/t24906/mklisting.sh BODY OUT.s`: ugen's `-S` listing (`/opt/ido/5.3/cc -S` with
  `IDO_CFLAGS` minus `-c`); the listing lands in `/work/draft.s`, not at `-o`.

The scratch pair reproduces the tree's exact 5 rows (`body.c` = current guard body).

## What the two known shapes are (ugen `-S`)

Current body (empty test, A):

```
move $6,$5            <- uopt promotes arg1 into a register web
sll  $14,$6,2
la   $15,D_8016024C
addu $18,$14,$15
.loc do
sw   $6,44($sp)       <- spill at the END of the preheader block
li   $19,6
$32: lw $6,44($sp)    <- dead reload (as1 deletes it, no ring slot spent)
...
lw   $24,44($sp)      <- reload for the post-loop test
```

With the test dropped (B):

```
sw   $5,44($sp)       <- entry home store (correct place)
...
lw   $14,44($sp)      <- first-use reload; as1 forwards $a1 and deletes it,
sll  $15,$14,2           but $14 = $t6 is already spent in ugen
addu $18,$15,$24
...
lw   $25,44($sp)
```

So A has the right temp numbering but the store at block end (as1 then sinks it below
`li`); B has the store in place but every ugen temporary one higher (6 rows: `lui t8`,
`addiu t8`, `sll t7`, `addu t7/t8`, `lw t9`, `beqz t9`). The ugen ring slot is spent
before as1 deletes the load, so a C form is needed where the preheader's first use of
`arg1` is a register copy (like A's `move $6,$5`) while the home store is the entry
store (like B), or where the store-back is emitted before `addu`.

## Variants run this session (all <= 5 rows unless stated)

| # | form | result |
| --- | --- | --- |
| A | current guard body | 5 rows |
| B | test dropped | 6 rows (store in place, all temps +1) |
| v1 | `p = ... + arg1; if (arg1) {}` then `i = 0; n = arg0;` | 11 rows (p first: p->s0, i->s1, n->s2, store still last) |
| v2 | inits line, test on its own line, then `do` | 5 rows |
| v3 | test first on its own line, then inits | 5 rows (spill still after `addu`) |
| v4 | test first, p on its own line, then `i/n` | 11 rows (same s-reg swap as v1) |
| v6 | `s32 k; k = arg1; p = D_8016024C + k;` + test | 14 rows (k = load, s-regs swap) |
| v7 | `k = arg1; p = ... + k;` one line | 14 rows |
| v8 | `if (&arg1) {}` after inits instead of test | 6 rows (= B) |
| v9 | `if (&arg1) {}` before inits | 6 rows (= B) |
| v10 | K&R definition + test | 5 rows |
| v11 | K&R definition, no test | 6 rows |
| w1 | `p = ... + (arg1 & -1);` no test | 6 rows |
| w2 | `p = ... + (arg1 ^ 0);` no test | 6 rows |
| w3 | test as `if (arg1 & -1) {}` | 5 rows |
| w4 | test as `if (arg1 ^ 0) {}` | 5 rows |
| x1 | `if (arg1) {} arg1 = arg1;` | 5 rows |
| x2 | `arg1 = arg1; if (arg1) {}` | 5 rows |
| x3 | `arg1 = arg1;` no test | 6 rows (= B) |
| g1 | `if (arg1) goto L; L:` before `do` | 5 rows (branch folded, block not split) |
| g2 | `goto L; L:` before `i/n` inits | 5 rows |

Not usable here: `s32`/implicit-int return views (the tree prototype is
`void func_801511C0(s32, s32);` in `include/ovl/TACO.h`, and this ticket may not edit a
header); both compiled only after removing the prototype and are not pursued.

## Next concrete hypothesis

The store-back is placed at the end of the *basic block*, and as1 then sinks it below
`li` (stores have no dependents). The entry store (B) is scheduled by as1 into exactly the
original position, so the remaining problem is only the deleted first-use reload's ring
slot. Next run: aim for a ugen order where the first use of `arg1` is A's
`move $6,$5` (no ring slot) and the *entry store* still exists.

1. Force an early block boundary between the empty test and the `p` init without emitting
   code: a label that is the target of a real branch, e.g. `if (arg1) goto L;` where the
   taken arm has the init and the fallthrough re-joins, or `while (arg1) { break; }` (adds
   a `beqz`, so only if the branch is provably folded before as1). Check the `-S`
   listing: the spill `sw $6,44` must appear before `addu`, not under `.loc do`.
2. Try two promotions of the same value: `if (arg1) {}` (register web for the shift)
   plus a second, non-promoted use of `arg1` in the preheader (e.g. a dead `&arg1` or a
   read of `*(volatile s32 *)&arg1` in a discarded expression) so uopt emits the entry
   home store `sw $5,44` **and** the `move $6,$5` copy. The -S listing decides in one
   compile; do not trust row counts alone.
3. If both fail, the class is the known T-24205 open idea ("arg1 in memory at entry but
   used from $a1 in the first block; no C form found that gives both"); leave the guard.

Do not repeat: the 24 init orders, line splits, test spellings (`==0`, `!`, `?:`, `&&`,
`switch`, `goto`, `(void)&arg1`), index/while loops, pointer temporaries, K&R, and the
`& -1` / `^ 0` wrappers above.
