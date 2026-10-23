# T-25108: main func_80065964 (424 B), matched

Start: `INCLUDE_ASM`; best earlier body `drafts/T-25010/main/func_80065964.c` (rolled `goto` loop, four
scalar corners, about 68 rows, no frame).

## Reading of the target

- Frame 0x68 with s0/s1 saved, but no call: the loop runs out of caller-saved registers.
- Eight coordinate registers, one pair per corner test: (v0,a3), (t0,t1), (t2,t3), (t4,t5). Four are
  copies of the other four (`or v0,t2`, `or a3,t1`, `or t0,t4`, `or t3,t5`). That is eight separate
  variables, not four values.
- The 15-trip loop stays rolled. IDO unrolls a constant-trip innermost loop, so the corner tests are an
  inner loop (4 trips, fully unrolled), which leaves the outer loop rolled.
- The dead `sw` to sp+0x20/0x24/0x28 sit below the declared locals: they are compiler temporaries (the
  shared subexpressions `&D[idx]`, x0, y0, x0+19, y0+15), not declared homes. Above them sit 15 words, the
  slots of the declared locals.

## Measured (scratch harness, whole-unit compile, rows = differing words)

| body | words | rows |
|---|---|---|
| inner `for (j < 4)` over `cx[4]`/`cy[4]`, filled as `cx[1] = cx[0] + 19; cy[1] = cy[0]; ...` | 101/106 | 131 |
| same, filled from repeated expressions `cx[1] = p[0] + dx + 19;` | 106/106 | 23 (frame, spills right) |
| + fourth bound test on `cy[1] > 120` (the target tests y0 > 120 in t1, the cy[1] register) | 106 | 16 |
| + one source line per corner (`cx[k] = ...; cy[k] = ...;`) | 106 | 14 (copy order `a3` before `v0`) |
| + other record read inline, `tx = ((s16 *)&D_80125C10[i * 4])[0];` (no `q` pointer in use) | 106 | 0 |

The `q` pointer in use puts q in a2 and tx in s0 (the target: q's address in s0, tx in a2).

## Negatives

- Without the unused `s16 *q` declaration (14 local words) the spill slots move up by 4 (4 rows). The
  original's 15th local word is unknown; the unused declaration is marked FAKE.
- Declared `x`, `y` locals for p[0] + dx and p[1] + dy (with or without `p`): 108 words, 82 rows.
- Fill order x first then y (c2): 38 rows; y before x per corner (c1): 48 rows.
- `cx[2] < -160` for the first bound test: 36 rows.

Result: plain C, MATCH in the default build (424 bytes), commit 71317c351.
