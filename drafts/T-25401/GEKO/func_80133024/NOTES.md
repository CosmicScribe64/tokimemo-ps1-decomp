# GEKO func_80133024 (T-25401)

## Pass 1 (wave 11, 2026-10-23): matched

Start: the T-24503 guard in `src/ovl/GEKO/801323E0.c` (three FAKE empty tests),
2 `funcdiff --resolve` rows: the day load is `lui a1; lbu a2,64(a1)` where the
original has `lui a2; lbu a2,0(a2)`. Result: MATCH, 280 bytes, default C, no
FAKE in the body. About 50 minutes.

Measuring: whole-unit `funcdiff --version c` (about 20 s) plus `regdiff -v`
with the traced uopt, and uopt input/output ucode dumps: a wrapper used as
`uopt_eval.py compile --uopt` that copies the input file, runs the traced uopt
and copies the output file, and a renderer built on `cvt_pass.parse`
(`m3 b0 oN` in the output is register N). The output ucode shows which value
sits in which register before ugen and as1 fold the copies.

### What the dumps showed

- The guard had two ranges for the day: the field (`rlod` into `$a1`) and the
  u8 local (`cvtl 8` copy into `$a2`). ugen emits `lbu a1; andi a2,a1,0xff`,
  as1 folds it into `lbu a2,64(a1)`, so the `lui` base is the field's register
  (the T-11910 finding).
- The test and the arms must not share the `day + i` sum. In the guard they
  differ because the test reads the field (copy-propagated into the header
  block) and the arms read the local (blocks with a call are not propagated
  into). A single day range makes uopt merge the sums, unless one side has the
  `CVT J<-L` that `cvt_pass.widen` puts after a direct read of a narrow
  unsigned global in arithmetic.
- With a single day range in `$a2` (the field, live into both call blocks,
  so `$a0`/`$a1` are barred), only the month was left: as a one-block range it
  took the first free register (`$a0`, or `$a1` behind the T-24503 table test).
  Nothing in the header block keeps `$a0` and `$a1` busy in the original.

### The cause

The original passes the day and month as third and fourth arguments to
func_80132D04, which reads only two. Passed values are precoloured to their
argument registers and live in the call blocks past the `$a0`/`$a1` writes,
so uopt colours the day field `$a2` and the month field `$a3`, and the loads
in the loop head go straight into them. Measured with a temporary
four-parameter prototype: the caller's 70 instructions were identical to the
original (regdiff MATCH), but the callee then homed `$a2`/`$a3`
(`sw a2,64(sp); sw a3,68(sp)`), so the callee has exactly two parameters and
the original call had no prototype in scope.

Final form: func_80132D04 gets an old-style definition (`arg0, arg1` declared
`s16`; its bytes are unchanged). The call then has no prototype, and the
extra arguments must be loads whose consumer is a store, otherwise
`cvt_pass.widen` adds a `CVT` for the int argument and uopt makes `cvt(field)`
its own range (60 rows). So each arm reads `day` and `month` into the locals
and passes them; the arm sums use `day` (the plain field after local copy
propagation), the test reads `D_800E6280.unk_040 + i` directly (widened), and
the table index reads `D_800E6280.unk_03F` directly. `n` stays (without it the
locals take 4 bytes less of the frame and the table copy sits at sp+0x3C,
4 rows).

### Measured on the way (rows from `funcdiff --resolve --version c`)

| body | rows | note |
|---|---|---|
| T-24503 guard | 2 | day `lui` base `$a1` |
| guard without `if (day) {}` | 10 | month field `$a1`, day field `$a3` |
| `day = unk_040 & 0xFF` | 44 | the and-expression is propagated as well, frame 0x60 |
| switch on `unk_03E` (zcopy-off rule) | 36 | a one-case switch makes no switch temporary; the rule does not fire |
| direct field reads, `(s32)` on the test's day | 43 | `cvt(field)` becomes a `$v0` range |
| test on local `day`, arms on the field (e1) | 30 | day correct (`lui a2; lbu a2`); month uncoloured (`$t2`) |
| e1 + `if (month) {}` | 6 | month `$a0` |
| e1 + month test + T-24503 table test | 6 | month `$a1` |
| natural locals passed as args, u8 prototype | 8 | locals `$a2`/`$a3`, fields `$a0`/`$a1` as `lui` bases |
| fields passed, u8 prototype | 0 in the caller | callee homes `$a2`/`$a3` |
| same with `s32` extra parameters | 62 | |
| `register u8` extra parameters | callee still homes them | |
| function-pointer cast with a four-parameter type | 48 | address hoisted to `$s1`, `jalr` |
| old-style callee, fields passed | 60 | widened `cvt(field)` ranges |
| old-style callee, `(u8)` casts on the fields | 60 | casts dropped |
| old-style callee, header locals passed | 51 | |
| old-style callee, `day = field` as the argument | MATCH | |
| arms with `day = ...; month = ...;` before the call, header locals | MATCH | |
| same, arm sums on `day` (header locals) | 31 | sums merged |
| same, no `n` | 4 | frame |
| final: header on the globals, arm locals | MATCH | |

Not needed any more: the three T-24503 empty tests, the `(u32)` casts (K&R
makes the compare unsigned), the do-while.
