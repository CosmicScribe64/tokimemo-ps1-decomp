# T-25147: main func_8005938C (764 bytes), matched

Start state: `INCLUDE_ASM` in `src/main/80058D20.c`. Best known body: the T-17017 closer draft
(`drafts/T-17017/main/func_8005938C-closer/body.c`), 124 differing funcdiff lines (62 rows), all
ugen temporaries; frame, stack offsets and every saved register already agreed (T-17017, T-25007).

Measured first divergence (row 46): the first MATRIX copy `m = *(MATRIX *)D_80125C60` loads its
data words into `t5` in the original and `t6` in the build. After the three `lh` temporaries
(`t1..t3`) the original skips one ring slot before each copy's data register (`t5`, then `t7`, then
`t6` for `r->coord = m`), the build skips two (`t6`, `t9`, ...). T-25007 had read this as an
unexplained copy cost.

Cause: the block-copy direction state of `tools/ugen_pass.py` (T-24092; one skipped slot in state
F, two in state B, as in T-25105). A dump of the ucode that ugen reads (a ugen shim copying `u.O`,
then replaying the pass's state machine) showed all three `Umov` in state B: the last state op before
each copy is a direct `Ustr` (the `rot` halfword stores, and the `m.t[]` word stores; uopt turns
`p->vz` with `p = &rot` into a direct store, and `p = &rot` before the loop does too).

Fix: the T-25107 recipe (MASTER `func_801363F4`), a FAKE never-true arm before the loop:

    if ((u32)D_800B0896 < 0) { do { *(u8 *)0 = 0; } while ((s32)D_800B0896 < 1); }

The `Uistr` followed by a signed `<` jump against a positive constant makes the procedure sticky F,
so all three copies switch at once. ugen drops the arm; as1 deletes the dead byte load. Result:
`funcdiff --resolve --version c` MATCH, then `--version default` MATCH (764 bytes) with the guard
removed. `D_800B0896` is an unrelated main byte global from `main_api.h`.

Negatives added: `p = &rot` hoisted before the loop keeps the stores direct (state B, no change).

Removal control: without the arm, 62 rows (the closer draft's count).
