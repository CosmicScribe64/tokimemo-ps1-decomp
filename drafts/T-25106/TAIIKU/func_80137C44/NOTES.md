# T-25106: TAIIKU func_80137C44 (matched)

Result: matched, 153 words / 612 bytes, default build C (no guard), `TAIIKU.bin: OK`. Source:
`src/ovl/TAIIKU/80133C80.c`, commits `3042ed030` (match) and `807da5dac` (spelling tidy-up).

## Start state

Production was `INCLUDE_ASM`. Best archived body: `drafts/T-24008/TAIIKU/func_80137C44/body.c`
(snapshot locals `mode`/`selected`/`frame`/`slot`, an `actor = &DISP(i)` pointer). Measured in the
full unit (`funcdiff --resolve --version c`): 189 diff rows. T-24521's typed drafts: 223-241 rows.

## Steps (one lever each, rows after each)

| step | change | rows |
|---|---|---|
| 0 | T-24008 body | 189 |
| 1 | read the four globals directly in the loop, plain `DISP(107 + i)` stores (no `actor` pointer) | 148 |
| 2 | the table term common to both mode arms (`D_801492B8[player.unk_04]`), no `off` local | 150 |
| 3 | table term first in every sum: `D_801492B8[k] + ((... >> 3) * 6)` | 59 |
| 4 | selected index as `(&DISP(46))[D_801491D8]` | 27 |
| 5 | `pos = D_80149298.halves.high;` before the second loop | 22 |
| 6 | byte-offset record address in one loop (`(DisplayActor44 *)(D_8011ECD0 + i * 0x44)`) | 4 |
| 7 | `*(s32 *)&D_80122E94 == 1` | 0, MATCH |

Why each step works (regdiff, traced uopt; all ranges of this leaf are unconstrained, so colours
follow bit order = first mention in the optimised IR):

1. The original colours `i` v0, `runner` v1, the record address a0 before the mode/selected/frame
   values (a1..a3). Snapshot locals at the top get the lowest bits and take v0..a1. Read directly,
   the globals are first mentioned inside the loop and uopt promotes them (priority 2.5-5) with the
   load moved to the preheader, which is where the original loads them. An `actor` pointer local
   makes every store an indirect store that kills the globals (priority -10, never promoted); the
   plain `DISP(107 + i)` stores do not.
2/3. `product + table` is turned around by uopt (`addu table,product` and the table loaded first);
   `table + product` gives the original's `addu product,table`, the load order of the else arm and
   the bit order selected < frame < table value (a2, a3, t0). The table value shared by the two
   mode arms is the CSE range t0 (an `off` local was not retried with the new order).
4. `46 + D_801491D8` keeps the CVT of the cvt pass (arithmetic), and the widened index multiplies
   with the counter's 0x44 constant (t3). Indexed straight from record 46 the global is an array
   index, has no CVT and shares the runner field's 0x44 constant (t5); the constants then number
   D_8011ECD0, 0x44, 1, 0x44, 6, D_80149320, D_801492B8 = t2..t5, s0..s2 as in the original.
5. The second loop's `lh v0` of the position: as a snapshot it is mentioned before the loop and
   takes v0, then runner pointer v1, record pointer a0, `x` a1.
6. With `DISP(107 + i)` in loop 1 and `DISP(108 + i)` in loop 2 both loops use one expression
   `ixa(D_8011ECD0, i * 0x44)` (one range, a0). Loop 2's induction-variable replacement then ends
   on the runner pointer (`bne v1,t1` against 0x80149274) instead of the record pointer
   (`bne a0,t1` against 0x8011ED9C). A byte-offset spelling in either loop separates the two
   expressions; the committed source puts it in loop 2 (three uses). FAKE.
7. `bne t4,a1`: a word global is a plain variable and uopt puts it first. The address view makes it
   an indirect load, so the hoisted constant stays first. FAKE.

## Negatives measured here

- `(u32)D_80122E94 == 1`: 17 rows; `1 == D_80122E94`: unchanged (4 rows).
- Separate counter `j` for loop 2: 45 rows (before the `pos` snapshot).
- `(&DISP(i))[107]` in loop 1: same IR as `DISP(107 + i)` (27 rows, LFTR still wrong).
- `DISP(i + 108)` in loop 2: 18 rows (same expression as loop 1).
- A record pointer local in loop 2 (`rec = &DISP(i)` or `&DISP(108 + i)`): 221 / 229 rows.

## Reproduce

`tools/docker.sh python3 tools/funcdiff.py func_80137C44 --unit TAIIKU --resolve --version default`
prints MATCH; regdiff needs the traced uopt under `$TOKIMEMO_SCRATCH/r9-uopt/variants/trace`.
