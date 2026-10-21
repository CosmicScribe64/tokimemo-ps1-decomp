# ETC func_8013F054 (src/ovl/ETC/8013BEF0.c), 716 B, unguarded

Control flow written from the asm (see body.c); `s32 func_8013F054(void)` (implicit-int callers, no return value). Not in `src/` (178 vs 170 instructions).

Findings (build/scratch sweeps with drafts/T-25012/tools/score.py, `struct` = diff with registers masked):
- Use `D_800E6280.` / `D_800E7D10` directly, no `GameState *g` / `u8 *w` locals (they hoist the base into a0 and change the whole prologue).
- The 0x756-based record scan is a pointer do/while: `p = (u8 *)&D_800E6280 + idx * 8; do { v = p[0x756]; p -= 8; if (...) {a = p[0x760]; b = p[0x75F]; type = v; break;} } while ((u32)p >= (u32)&D_800E6280);`. The `for (i = idx; i >= 0; i--)` and index do/while forms hoist the `D_800E71DF` load out of the loop; the original reloads it in the loop, only the pointer do/while does that.
- Types: `type` and `a` s32, `b` and `v` u8 gives the original's t1/a3/t0 colouring and a shared `li t2,2` (struct 28 -> 26, diff 94 -> 65). `u8` for all four leaves `andi t1,v1,0xff`.
- `idx = D_80150064; if (idx) {}` (FAKE empty test) loads idx in the entry block like the original.
- Left: the original keeps idx in $a0 and a copy `move v1,a0` (a second variable that uopt does not copy-propagate) and sets `li a0,12 / li a1,11` after the found and not-found loop exits; mine colours idx $v1 (regdiff: bit 144 constant 0xC outranks the lui constant of D_80150064).
