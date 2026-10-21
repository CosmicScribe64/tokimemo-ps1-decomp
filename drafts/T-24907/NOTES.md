# T-24907 GYOZI:func_80140E0C — run notes (2026-10-20)

Target: `GYOZI:func_80140E0C` (388 B), file `src/ovl/GYOZI/80140C90.c`.
Result: **not matched**. Source left exactly as at HEAD (guard unchanged). `funcdiff --resolve`
still shows the same single difference class, 3 instruction words:

```
-00002825  ; move a1,zero     (or a1,zero,zero at entry)
-a02562cf  ; sb a1,0(at)      (sb a1, D_800F62CF after the if)
-00a02025  ; move a0,a1       (or a0,a1,zero in the jal delay slot)
+a02062cf  ; sb zero,0(at)
+00002025  ; move a0,zero
```

Raw `funcdiff` (no `--resolve`): 39 diff lines, all from those three words plus the address-base
symbol text.

## Harness

`try.py <variant.c>` installs a candidate body of `func_80140E0C` into the guard, runs
`funcdiff --resolve`, prints only the diff rows. ~20 s per variant. Variants live next to it
(`v1.c` .. `v36.c`); `80140C90.base.c` is the HEAD body.

## Retried / new evidence (all negative)

| # | Variant | Result |
| --- | --- | --- |
| 1 | Promoted-scalar-global hypothesis (ticket): `D_80148754 = 0;` at entry, `D_800F62CF = D_80148754; f(D_80148754)` | no promotion: a real `lui at; sb zero,D_80148754` appears at entry, the two uses still fold. uopt does not promote a written global here (calls in the function). |
| 2 | Same with `D_800F62CF` itself as the carrier written at entry | store stays at entry (`sb zero`), i.e. no promotion and no deferral of the store to the call. |
| 3 | Zero locals: `s32`/`u8`/`s16`, declaration initializer, `z = z * 1`, `u8 *z = 0` / `s32 *z = 0`, local struct field `struct { u8 f; } z; z.f = 0;`, loop counter `i = 0` reused | all fold to `sb zero` / `move a0,zero`; some also lose the later `lui a1` address base (the T-24712 byte-view effect). |
| 4 | T-22021 lever `<byte> *= 0;` / `= (<byte> < 0)` on `D_800F62CF`, plain and through `*(u8 *)&`, and on `D_800F53A0.unk_F2F` | folds here. That lever needs the byte to be a promoted range (loop read/write with no calls); `D_800F62CF` is reloaded after calls (`lbu a0` at CEB4, `lbu t2` at CEC8, `lbu t1` at CF40), so no promoted range exists. |
| 5 | T-9020 idiom that matches the `D = N; f(N)` family for nonzero N: `f(*(u8 *)&D_800F62CF = 0)` | emits `sb zero` and a **dead** `move a1,zero` after the store; the call argument still gets its own `move a0,zero`. Row count worse (entry block shifts by one). |
| 6 | `z = (*(u8 *)&D_800F62CF = 0); f(z)` | store/argument order swaps (`move a0,zero` before the `sb`) and the later reload base moves to `v1`. Worse. |
| 7 | Two-argument call `f(z, z)` | register/scheduling chaos through the entry block (row 6 onwards), no gain. |

## Reading of the original

`or a1,zero,zero` at CE24, `sb a1,D_800F62CF` at CE64 (merge, after the `if`), `or a0,a1,zero`
in the `jal func_8005E0E0` delay slot. So the source creates a zero *variable* in the entry block
and consumes it at the merge: `z = 0;` before the `if`, `D_800F62CF = z; f(z);` after it. Stock
uopt folds `z` into the two uses in every form tried, because no other definition of `z` reaches
the merge.

The original's `li a1,0` is therefore one of two things:

1. a def of `z` that reaches the merge **without emitting code** (so the use is not a constant)
   — the next thing to try; or
2. the T-0018 "constant in a register" family (the original uopt materialises and shares a
   constant whose value stock 5.3 propagates into both uses).

## Next concrete hypotheses

1. **A non-constant path to the merge that emits no code.** Forms worth one grid: an arm that
   assigns the same zero through a node uopt keeps (e.g. `if (c) z = D_800F6468 & 0;`, a
   comparison result, a `?:`), or a scalar-replaced aggregate whose other field is defined in the
   arm. Constraint: no extra branch/instruction may appear (the entry block and the three tests
   currently match exactly).
2. **uopt trace to settle (2).** `tools/uopt_patch.py trace` + `regdiff` on this function to see
   whether stock uopt ever has a constant node with two uses (store + call argument); if it
   propagates in both by design, close as a T-0018-family blocker with the evidence.
   Note: `-nokpicopt` (project flag) stops global *addresses* in registers, not integer constants,
   so the flag is not the cause.

If (1) is exhausted and the trace shows uopt always propagates, this is a compiler-behaviour row,
not a source row; do not re-run the zero-local/chain/promotion grids.
