# T-25130: MASTER func_801326B4

Result: matched, default C in `src/ovl/MASTER/80132140.c` (3264 bytes compared; `funcdiff --resolve --version default`
prints MATCH). About 55 minutes of work.

## Start state

- `src/ovl/MASTER/80132140.c` had plain `INCLUDE_ASM` for the function.
- The best body was `drafts/T-24517/later/func_801326B4.c` (raw `sel` local in case 4, re-read after each call).
  Measured as a guard here: 113 rows. 11 of them (rows 259-370, plus an `lbu`/`lw` at row 330) came from
  `func_800847B8(D_8013C2E0)` seeing the `u8` prototype of `main_api.h`. The T-24517 replay had defined
  `MAIN_API_OVERRIDE_func_800847B8`; with that view: 102 rows, first divergence at row 430.

## First divergence (102 rows)

Row 430, the inner case 4 switch: the original computes `srl t8,a0,12` (a ugen temporary), the draft
`srl v1,a0,12` and then `sll t8,v1,2`. The traced uopt (`regdiff -v`) shows the cause: copy propagation turns
`sel` into the global D_800E6374 (`D_800E6280.unk_0F4.h`), and `shr(D.h, 12)` becomes one PRE range (bit 131,
2 uses, 7 defs, priority 1.29, `$v1`): computed at the switch, inserted after every reload in the call arms
(`srl v1,a0,12` in branch slots 464, 480, 537, 562, 578) and before the join store. The original shares nothing
between the switch shift and the join shift. Rows 627-815 were the same ugen ring shifted by the missing temporaries.

## What closed it

1. **Join test through a u32 copy.** `mode = D_800E6280.unk_0F4.h; if ((mode >> 0xC) != 3)` at the join,
   with the switch written `switch ((u16) D_800E6280.unk_0F4.h >> 0xC)`. The switch expression is
   `shr(cvt(D.h), 12)`; the copy-propagated join expression is `shr(D.h, 12)` (the u32 assignment absorbs the cvt).
   They are different expressions to uopt, so nothing is PRE'd and both shifts are ugen temporaries (t8, t6).
2. **Six always-false tests after the arm calls** (`KEEP_MODE_LIVE()`, `if ((u32) D.h < 0) {}`, FAKE). Without
   uses in the arms the global is not profitable (7 uses, 9 reload charges, priority -0.29) and is loaded into
   temporaries at each use. One test per call arm (case 1 x2, case 3 after the CD branch, case 5 x2, default)
   gives each arm a use with its reload, so the join use is free; the global gets `$a0` and uopt reloads it after
   each call, exactly where the original does. ugen folds the unsigned `< 0` test: no code.
3. **`func_800847B8` as an implicit int** (`s32 func_800847B8();` under the override). A `void` declaration
   gives 39 rows (v0/v1 ranges in case 0 and 1 move); the implicit int and `s32` views match.

## Negatives measured (regdiff --src, 815 vs 815 unless noted)

- Join `(sel / 0x1000)` with the T-24517 arms: arms' reloads die after copy propagation, global not allocated
  (282 funcdiff rows).
- Join `((u16) D.h >> 12)` or `D.h / 0x1000` with empty `if (D.h) {}` arm tests: `cvt(D.h)` becomes a PRE
  range in `$v0` (9 rows, all `v0` for `a0`).
- Head `sel = D.h; switch (sel >> 12)` with `if (D.h) {}` arm tests: the tests survive as `beqz a0,join`
  (830 instructions). The same with `(u32)` tests and a `(u16) D.h / 0x1000` join: a v1 copy range
  (`move v1,a0` in the no-call arms).
- Arm forms in the final configuration: `if (D.h) {}` and `if ((s32) D.h < 0) {}` keep a branch (830);
  `sel = D.h;` and `D.h;` are removed (806, 229 register rows); `if (D.h < 0) {}` matches like the `(u32)` form.
- Removing any one of the six tests: 806 instructions, 229 register rows.
- Natural forms with real reloads (`sel = D.h` in the arms, join on `sel`, switch on the cvt form): 806 or 829
  instructions.

## Reusable lesson

When the original recomputes a shift at a switch join from a promoted global that it reloads after calls, and
the draft shares the shift in `$v1` (PRE insertions after the reloads), make the two occurrences different
expressions to uopt (cvt form at the switch, u32 copy at the join) and give the global enough uses in the call
arms to stay promoted.
