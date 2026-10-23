# T-25144: RPG_BAT func_8014BFE4 (892 B), matched

Start: `INCLUDE_ASM`; best known body was the T-17110 hand draft (`drafts/T-17110/RPG_BAT/func_8014BFE4/hand.c`), 128 funcdiff rows. Result: MATCH in the default build (C in `src/ovl/RPG_BAT/8014ADD0.c`, no guard).

## Method

Besides funcdiff, two scratch helpers (not committed, under `build/`) did most of the work:

- an `objdump` side-by-side of the guarded whole-unit build against the original (difflib over the instruction text);
- `cc -S` on the function alone with the shims of `tools/cc.py` (`cc.ido_frame_env`), which prints ugen's listing before as1 (pseudo-ops such as `blt`, `mul`, and every `lw`/`sw` of a stack home). That listing shows which reads are real loads, the ugen ring order, and where uopt places an expression, in about 3 s per variant.

## Levers, in the order they cut rows (128 -> 0)

1. **Head chain: `if (arg3 < 10) {} else if (arg3 < 100) ...`.** The original reloads arg3 and arg1 in every test (`lw` that as1 turns into the dead `move t8,a3`, `move t9,a3`, `move t0,t7`). With `if (arg3 >= 10) { if ... }` ugen keeps arg3 in `$24` through the fall-through and reuses arg1's `$15`. The empty first arm puts a label before the second test, and the label clears ugen's register cache. This fixed the ring for the head and the switch (t7..t0); a `goto` chain gives the same listing.
2. **Record index `j = slot * 12` as its own variable, incremented in the loop.** Its increment is dead after strength reduction, so its init is the original's dead `move v0,t1`, and `i = 0` moves to `v1` (`move v1,zero`).
3. **`slot * 12` in memory: `s32 t[1]` (FAKE).** The original stores slot*12 to sp+0x50 and reloads it three times after the loop; `t + 0x40` after the loop is not CSE'd with the one before the loop. A plain scalar is copy-propagated and spilled as a uopt CSE (second spill, frame 0x70). An address-taken scalar (`*&t = ...`) gives the same ugen listing as the array. Reading `j = t[0]` loads straight into `$2` with no ugen temporary, which keeps the ring.
4. **Frame: five scalars (i, j, idx, div, k), div fourth.** Declared scalars take slots top-down from sp+0x64, so div's home is 0x58 (the original spills div there) and the array sits at 0x50.
5. **Post-loop index in a variable other than the loop's** (`idx = t[0] + 0x40` after the first post-loop call). The original colours it `s0`, not the loop index's `s2`; a separate variable gives a separate range. Its uses are not forward-substituted because the array load cannot move past a call.
6. **`div = 1` before the switch.** div's range then spans the switch blocks, so its priority drops below the other saved-register ranges, and `li a1,1` fills the delay slot of the first switch compare. The T-25007 note had this as worse (241) on the old body; it only helps once the other ranges are right.
7. **Per-iteration call index `k = j + 0x40;` at the top of the loop body.** Strength reduction makes it the IV in `s2`, initialised with `addiu s2,t1,64`. Because the arm's `k + 8` is now an expression on an SR temporary, uopt computes it before the first call (`addiu s5,s2,8`), as the original does. That tenth saved-register range pushes div out: div goes to `a1`, spilled at its home 0x58 around the calls. With `idx++` as a plain variable, `idx + 8` is forward-substituted to the second call and div keeps `s5`.
8. **`for (i = 0; i < 4; j++, i++)` with `div *= 10` at the end of the body.** This puts the SR increments in the original order (`s2`, `s0`, then `s6`, `s4`).
9. **`t[0] = arg0 * 12; j = t[0]; for (...) {` on one source line (FAKE).** as1 schedules per source line. On separate lines, `addu s0,t2,t3` (record pointer init) lands after `sw t1,80(sp)` and `move v0,t1`; on one line it comes right after `lw s4,108(sp)`, as in the original (3 rows -> 0).

## Negatives measured this session (do not repeat)

- A plain variable `k = idx + 8` before the first call: forward-substituted to the second call, both for one use and for two uses (`k - 8` in the third call is then not folded: `addiu a0,s1,-8`). `k - 4` in the first call, `(k - 8) + 4`, `k + -4`, `k - k`, `k & 0`, `k ^ k`, `0 * k` are all folded before CSE.
- An empty test `if (k) {}`, `if (k < 0) {}` or `if (k == idx) {}` before the first call does place `idx + 8` early, but arg2 loses its register and the frame grows (250-300 rows).
- Labels (`do {} while (0)`, `goto`) do not stop uopt's forward substitution.
- `j + 0x48` as the second call's argument keeps j alive as a variable (no IV elimination).
- A dedicated IV `k = j + 0x48; k++` stays in memory.
- `j = arg0 * 12` with `t[0] = j`, chained assignments, or the CSE of `arg0 * 12` used twice: the product becomes a uopt-coloured range (`mul $2`), the wrong colours.
- `slot * 12` as a spilled CSE (no array, four scalars): same pre-loop listing and frame, but the post-loop index is forward-substituted (29 rows).
- `volatile` arg1/arg3: as1 keeps volatile loads as `lw`, so the dead moves vanish (376 rows).

## Toolchain note

A traced uopt (for `tools/regdiff.py`) can be rebuilt from scratch in about 6 minutes in Docker: clone ido-static-recomp at 9c242adc, apply `tools/ido-ecvt.patch`, `make RELEASE=1 setup`, `make RELEASE=1 VERSION=5.3 IDO_TC=uopt build/5.3/out/uopt`, then `tools/uopt_patch.py trace` and gcc as in `wiki/uopt-register-allocation.md`. The link step needs `build/5.3/libc_impl.o`, which the build provides. It was built but not needed in the end.
