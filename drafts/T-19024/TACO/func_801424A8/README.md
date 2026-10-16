# TACO func_801424A8 (T-19024)

Typed draft ported from the matched sibling `func_801413B8` (`src/ovl/TACO/80140DF0.c`): variable step
`step` from `arg1`, record `D_8015E770[arg0]`, the same quad drawer with `setUVWH(poly, tbl[side], 0, 0xF, 0xF)`.

State: 220 of 220 instructions, one structural difference left (`move a1,t4`: the original loads
`D_8011ECA0` straight into `$a1` for `func_800490F0`), 81 rows differ in registers (162 funcdiff lines).
Frame (208 bytes, spill slots, local order `step` after `j`) and the unit sha1 aside, the open problem is
the temporary pool: the original has `$t3` reserved by a uopt range (second reload of the spilled
`o * 4` accumulator), so ugen's temporaries start one register later (`lw t4` in the matrix copy
against `lw t3`). Needs the extra FAKE `pad[2]` and `k` locals of the sibling for the frame.
Not used in the build; `src/ovl/TACO/80141DE0.c` keeps `INCLUDE_ASM`.
