# T-24921 ETC:func_8013CCE4

Result: **not matched**. Guard left at **3 rows** (was 9). Owned file only:
`src/ovl/ETC/8013BEF0.c`.

## Final diff (3 rows)

```
-@row 7  1061001a  beq v1,at  vs  +1041001a  beq v0,at
-@row 10 10610037  beq v1,at  vs  +10410037  beq v0,at
-@row 13 10610073  beq v1,at  vs  +10410073  beq v0,at
```

Everything else in the function (case 0/1/3, case 2 schedule, constants, call
argument temporaries) is identical. The three rows are the switch selector's
compare register: original compares the promoted global (`$v1`), the build
compares the switch temporary (`$v0`).

## What moved the rows (kept)

1. `buf[0] = 0x801E2000; buf[1] = 0x801E6000;` (ascending order, one line)
   plus the `for` on the **same source line**. This removed all six case-2
   rows (ori `0xA000`/`0x6000` order, `sw`/`addu` order, `li -160/-125`
   order, `sw` order on the outgoing argument slots). Compare-before/after:
   9 rows -> 3 rows.
2. The unroll shape itself is sensitive to the counter type:
   `u32 i` with the plain selector reproduces the target's unrolled loop
   with `li a0,2; sll v1,a0,0xe` (the induction kept in `$a0`). `s32 i`
   folds the loop to six constants (frame 8 bytes smaller). The guard keeps
   `u32 i`.

## Stage 1/2 finding (the blocker)

- `switch (*(u8 *)&D_800E6280.unk_110D)` **does** fix the three compare rows
  exactly (`lbu v1; or v0,v1,zero; beq v1,at`), as the ticket said.
- Side effect measured with `cc -S`/objdump of the guarded TU: uopt
  constant-propagates the loop counter. All six `buf[]` constants are
  materialised separately (`lui t3..t8`), `li a0,2` and the two `sll` are
  gone, `i` gets no stack slot (IDO frame 0x58 instead of 0x60, final 0x68
  instead of 0x70). Rows 9 -> 89.
- The fold is **coupled to the compare register**, not to the loop spelling:
  every selector form that compares on `$v1` (byte view, `& 0xFF`, pointer
  cast `((GameState *)&D_800E6280)->unk_110D`, `u8` local copy, a file-local
  `extern u8 D_800E738D`) folds the loop the same way.
- Loop spellings tried under the byte view, all still folded:
  `i != 6`, `do { } while (++i < 6)`, `s32 i`, `u32 i`, body/head on one
  line, `if (&i == 0) {}` (address-taken empty test).
- The scalar `extern u8 D_800E738D` view is not usable: it breaks aliasing
  with the case bodies' `D_800E6280.unk_110D` accesses (each increment
  reloads into a new temporary; far more rows).

## Next hypothesis

The target combines both properties: `$v1` compares **and** the
induction-kept unroll. The pass that decides the switch temporary is
`tools/cvt_pass.py` (`copy_propagation_options`, `OPTN 405 0` when the
switch temporary is stored from a direct `LOD L` of an unsigned global
before the first call/branch/label). So the next step is to dump the ucode
for this procedure (`cvt_pass.py` shim with a dump, as in `T-5010` work) and
see which ucode difference between the plain field and the byte view flips
`_assigns_switch_temp_from_unsigned_global`; a plain `LOD L S` selector
should get zcopy off and would give `$v1` compares with the target unroll.
That is a toolchain-level (uniform) question, not a per-file option.

Scratch helpers used: `drafts/T-24921/ccdump.py` (compiles the guarded TU and
objdumps `func_8013CCE4`).
