# T-25107: MASTER func_801363F4 (2,608 bytes), matched

Start: plain `INCLUDE_ASM`. Best earlier body: `drafts/T-24517/func_801363F4.c`.
Under the guard it needed `void func_80137BF4(void);`, which is now in
`include/ovl/MASTER.h`. Measured whole-unit: 167 rows, all register-only. The
first divergence was row 213, the first 0x44-byte record copy in case 4. The
end pointer was `addiu t3,t0,60`; the target has `t2`, which is source + 2 in
ugen's ring.

## Cause

ugen burns one or two scratch registers before a block copy's end pointer.
The count depends on a stale ugen stack slot (T-24067, T-25029). In our
pipeline, `tools/ugen_pass.py` inserts a sentinel store before every `Umov`,
and that sentinel decides the count. A B sentinel (`Ustr`) burns two, and an
F sentinel (`Uistr`) burns one. Shown with `cc -S` on a test file, with and
without the shim: after `if (n) f();`, raw ugen burns one and the shim burns
two. The pass picks F when the last state op before the copy is an `istr`,
or when the procedure has had an `istr` followed by a signed
blt-positive-immediate `fjp`/`tjp` (sticky for the rest of the procedure).
Here every store before the copy is a direct global store, so the state was
B. Both copies of this function are F in the original: the record copy and
the copy loop `[i] = [1]`.

## Fix

1. FAKE (T-25107): just before the first copy,
   `if ((u32)D_8013C578 < 0) { do { *(u8 *)0 = 0; } while ((s32)D_8013C578 < 1); }`.
   uopt keeps the arm, so the pass sees an `istr` and then a signed `les 1`
   `tjp`. That makes the state sticky F for both copies. ugen folds the
   unsigned `< 0` test and drops the arm. The only code left is a dead `lbu`
   of the tested byte, which as1 deletes. The frame stays the same. This left
   5 rows.
2. The point loop stores x before y, as record 1 does:
   `D_8013C58C[i].x = value - 0xA0; D_8013C58C[i].y = 0x3C;`. as1 then hoists
   the `li 60`/`sh` above the `mfhi`, and the ugen temporaries come out as
   `t9`, `t1`, `t2`. Result: 0 rows, default build MATCH, MASTER sha1 OK.

## Measured negatives

- `value` must stay declared. Removing it and folding both uses gives 7
  rows, because the spill slots move. Folding only one of the two uses still
  matches. The committed form keeps both uses.
- Two separate dead arms, one before each copy (the second inside the copy
  loop), give 19 rows: the uopt ranges inside the loop change.
- One dead arm with only `*(u8 *)0 = 0` gives F at the first copy only. The
  copy loop stays B: 145 rows, first at row 260 (end `t4`, target `t3`).
- An `istr` that reaches the target bytes cannot be had from a store
  spelling. Integer, volatile and `(u32)` views put the address in a
  register (`la`, then `sb 0(reg)`). Constant-address pointers, array
  elements and local pointers all become `str` after uopt.
- The dead-arm condition variable matters little. `D_8013C588` and
  `D_8013C578` both match. `D_8013C4CC` and `D_800B5BD4` disturb uopt (214
  and 258 rows).

## Next

The same block-copy state miss is recorded for MASTER `func_801344E0` (same
file) and ENDING `func_80134270`: "F after global byte stores" in the
original (wiki/notes/matching-05.md, T-19001). Try the same sticky dead arm
before their first copy, after checking which of their copies need F.
