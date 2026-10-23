# T-25406: main func_80067870 (952 B, 238 words)

## Pass 1 (wave 11)

Result: not matched. Guarded in `src/main/800674B0.c` at 14 rows
(`tools/docker.sh python3 tools/funcdiff.py func_80067870 --unit main --resolve --version c`), body also in
`body.c` here. A second draft, `views.c` (15 rows), closes the D_80125C54 gap but opens two others.

### Start state

`INCLUDE_ASM`. The T-25103 body (`drafts/T-25103/main/func_80067870/body.c`) measured 75 rows. Its first divergence
was the one T-25103 described: the `15 - SquareRoot0()` range (the "sub", coloured but forwarded, so it never shows in
the code) takes `$a1`, `unk_F80` is pushed to `$a3`, `D_800B5C08` is split, and the ugen ring runs one register late
from word 14 to word 97. Its next hypothesis was to make the sub's range ineligible or lower it below dy.

### Measurements that decided it (traced uopt via regdiff; colour lists per variant)

- uopt interference is per basic block. After SquareRoot0 the block holds the x1 sum (X), the y1 sum (Y), the sub,
  cvt(D_80125C52), cvt(D_80125C54), D_800B5C08, unk_F80, x1, y1 and dy: ten ranges for the nine registers the original
  shows (v0, v1, a0-a3, t0-t2). With X `$v0` and Y `$v1` in one block, the sub can only take `$a1` (T-25103) or `$t0`
  (three empty tests, x1/y1/dy move up). dy always ranks below the sub (2/D against 3/D with D_dy >= D_sub), so
  lowering the sub cannot work. T-25103's hypothesis is closed.
- `$v0` is never available to a range live in the block right after the SquareRoot0 call.
- The sub must still span two blocks with one use in each: block-local, it is emitted (an extra `move`, 240 words).
- With X and Y in different uopt blocks, both take `$v0` and the sub takes `$v1`. Then, without the T-25103 FAKE
  `(u32)` on the `D_8011F46A` store (cvt(D_80125C52) back to priority 1.333, coloured after the sub), cvt(D_80125C52)
  takes `$a0`, unk_F80 `$a1` (its block holds the sub's `$v1`) and D_800B5C08 one `$a3` range. Rows 0..69 match except
  the D_80125C54 rows.
- All stores here use `lui at; sh`, so as1 keeps them in ugen order: the store order of the original
  (F422, F426, F414, F466, F46A) is the source order. With the F466 store in the x1 block the stores come out early
  (84 to 113 rows for every placement and line grouping; `#line` changes nothing).
- Separate-symbol stores (`D_8011F414` ...) and the `D_8011ECD0[0x1B/0x1C].unk_xx` fields give the same bytes. The
  guard uses the fields as the sibling `func_800676AC` does; `D_8011F41C` stays the declared s32 word.
- `D_800E6280.unk_F88 >> 16` is `srl` in the original: `(u32)` cast (the field is s32 in the header).
- `x1 = D_80125C50 + dx` beats `dx + D_80125C50` in the new structure (14 against 17 rows).

### Changes (75 to 14 rows)

| change | rows | why |
|---|---|---|
| `(u32)D_800E6280.unk_F88 >> 16` | 75 (register-only rows before) | `srl`, as in the original |
| second empty test between `D_8011ECD0[0x1B].unk_26 = x1;` and `y1 = ...` (FAKE) | 75 to 97 (k4) | X and Y in different blocks; the sub takes `$v1` |
| drop the `(u32)` FAKE on the F46A store | (in k4) | cvt(D_80125C52) coloured after the sub, takes `$a0` |
| F466 store after the F414 store, `x1 = D_80125C50 + dx` | 97 to 14 | store order; ugen ring order |

### What is left (14 rows)

1. The `D_8011ECD0[0x1C].unk_26 = D_80125C50;` store is after the block split, so uopt reloads D_80125C50
   (`lui t4; lh t4`, 2 words, 2 branch-offset rows). The original reuses the load of the x1 sum (`sh t4`). The store
   order puts F466 after F426, and F426 is in the y1 block, so in uopt terms the x1 load and the F466 store cannot
   share a block while X and Y are apart. A local copy (`cx = D_80125C50`) is copy-propagated back (no change).
2. T-0018 promo gap (10 rows): the D_80125C54 global takes `$a3` (bb8 `lbu`, the `lui` bases after both calls, the
   case `addiu/andi`), cvt(D_80125C54) `$a2`.

### Second draft `views.c` (15 rows)

Every read of D_80125C54 through `*(u8 *)&D_80125C54` and the call-side empty test removed: `tools/cvt_pass.py`
adds no CVT to an indirect load, cvt(D_80125C54) is gone and the global takes `$a2` in every block, as in the
original. The tests that ordered priorities keep working. New differences: uopt puts the promoted variable first in
the y1 sums (`addu t0,a2,a0` against `addu t1,a0,a2`: B and the four case sums) and y1 (0.857) outranks x1 (0.75),
so x1/y1 get `t1/t0`. Casts on the view (`(u32)`, `(s32)`) bring a cvt range back (15 and 81 rows).

### Negatives added (this pass)

- One split anywhere in the 120 orders of {x1 pair, y1 pair, F414, F466, F46A}: best is the T-25103 order (75).
- Second split after the F46A store or before F414 (g1..g5): unk_F80 takes `$v1` (108..117 rows).
- y1 defined in its own block before x1 (s1..s3): the y1 sum is not shared with the store (241..242 words).
- Global copy propagation off (`register u32 cp = D_800E6280.unk_10F8;`, the T-25301 rule): all colours right, but the
  F426 store reads dy (no sub), dy narrowed in place, 237 words, 91 rows.
- `(x1 = ...)` / `(y1 = ...)` assignment expressions in the stores: X and Y stay (75..148).
- `*(u8 *)&D_80125C54` views on subsets of the reads, on the T-25103 structure: 75..163.

### Next hypothesis

Combine the two drafts' wins. Start from `views.c` (D_80125C54 right everywhere) and fix (a) the add operand order:
the original keeps cvt(D_80125C52) first, so in the original the D_80125C54 operand of the y1 sums is not a plain
variable; try the views only on the compare, multiply, bb8 and empty-test reads while the y1 sums read the global
directly, with a store or call between blocks so each sum's cvt stays block-local (no cross-block cvt range), and
(b) x1 over y1 (one more x1 reference in an existing block, or one fewer y1 block). Separately, the D_80125C50 reuse
needs the F466 store and the x1 sum in one uopt block while F426 (y1 block) stays between them in store order: look
for a construct that splits uopt blocks for colouring but not for local CSE, or a reason the original's F466 value is
not a plain reload (an aggregate copy of the D_80125C50/52 pair, as `func_80053650` does with `memcpy`).

Scratch tools (not committed, `build/scratch/vt/`): `vt.py` (scores variant bodies in one Docker run, about 0.5 s
each on an idle machine; `-r` adds the regdiff colour list), `gen.py` (statement-order generator), `search1.py`.

## Closer (wave 11): matched

Result: default-C match (`funcdiff --resolve --version default` and `--version c`: MATCH, 952 bytes), guard
and INCLUDE_ASM removed. No empty test and no `#line` left. Intermediate bodies: `closer_s4.c` (3 rows),
`closer_match_linemark.c` (0 rows through a `#line` shift, scratch-only marker), `closer_W4.c` (first plain match).

### Steps (rows from `funcdiff --resolve --version c`)

| change | rows | why (measured) |
|---|---|---|
| start: `views.c` | 15 | |
| y1 sums as `*(u8 *)&D_80125C54 + *(s16 *)&D_80125C52` | 15 (order rows gone) | cfe orders a commutative `+` by operand weight; with both operands views the weights are equal and the D_80125C52 operand comes out first (`addu t1,a0,a2`). Source order, `* 1`, `^ 0`, `0 - x`, a local copy, `(y1 = D52) + v` left it unchanged |
| `if ((u32)x1 < 0) {}` before the call (later dropped) | 3 | x1 one block more, 7/9 against y1 6/8: x1 takes t0 |
| drop the second empty test, `x1 = *(s16 *)&D_80125C50 + dx` | 2 | x1 sum, y1 sum and the F466 store in one block: D_80125C50 load shared (`sh t4`), 238 words; with the plain read cvt(D_80125C54) comes back as a range ($v0, 146 rows) |
| as1 trace | | last 2 rows: `lui a3` (D_800B5C08 reload after SquareRoot0) against `sll t2,t8,16` (dy narrowing). Both tie on time 11, aftercycles 9, latency 1; the smaller line wins, and the reload carries the line of the statement that holds the call, the same as dy's narrowing. A `#line` shift of dy by 3 or 4 lines matched. |
| `r = SquareRoot0(...); dy = 0xF - r;` (new s32 local), no empty test at all | 0 | the call and its reloads keep their line, the sub and the narrowing move to the next line, as the original orders them |
| clean-up | 0 | dropped: x1 priority test, every other D_80125C54 view (bb8, compares, case sums), the F466 view, the y1 recomputation in the F426 store (`y1 - dy`), one of the two multiply views |

FAKEs left (marked in the source): one byte view in the SquareRoot0 multiply (33 rows plain), the D_80125C50 view
in the x1 sum (146 rows plain), both views in the y1 sum (161 rows plain, 1 row with one view), the T-25103 joined
lines of the 0x8 cases (4 rows split).

Scratch tools (not committed): `build/scratch/vt/vt.py` (scores bodies; `LINEMARK(k);`/`LINEREST;` lines become
`#line` after preprocessing, so the shift reaches IDO), `build/scratch/vt/tr.py` (as1 `-R -xbbdbg 8` trace of a body,
T-24900 shim in `build/as1tr/`).
