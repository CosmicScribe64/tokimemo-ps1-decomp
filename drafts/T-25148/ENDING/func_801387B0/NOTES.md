# T-25148: ENDING func_801387B0 (188 words, 748 B): MATCH

Worktree w9-ENDING-80137340, wave 9 neighbour conveyor (T-25200). The C is in `src/ovl/ENDING/80137340.c` with no guard; `funcdiff --resolve --version default` prints MATCH (linked bytes equal).

## Start state

INCLUDE_ASM. Best earlier body: `drafts/T-25002/ENDING/func_801387B0.c` (T-25002; 156 diff lines here, 183 against 187 instructions). T-25013 lever 3 concluded that no uopt option stops uopt from CSE'ing the boolean.

## What the original does

- In the else arm it loads the byte `D_800E6280.unk_53C[6]` into `a0`, keeps the constant 1 in `a1`, computes `andi v1,a0,1` once, and recomputes `xor tN,a1,v1; sltiu` for each of the four `N - ((byte & 1) == 1)` tests. So uopt shares the AND and does not share the EQU.
- In the 0x1C arm it reloads `D_8013C360` after the `!= 0` test.
- `D_8013CAA0 == 1` compares load first (`bne t0,a1`).

## Measured, in order (diff_score from a full-unit compile; scratch harness side.py)

1. Draft as is: score 156. uopt output (ucode dump): test 1 uses the local `flag` (v1 = cvtl(a0)); tests 2-4 use the byte register `a0`, and the EQU is computed at test 2 and saved in v1. So uopt shares the largest common tree.
2. Negatives: bit-field view `Flags7D4F.b0` (189, whole EQU shared), direct field reads (189), `bit` locals as s32/u8/u32/u8+s32 (158-184), and `!(x != 1)` (not folded: xor; sltu; sltiu).
3. Different spellings per test stop the EQU sharing and leave the AND shared. Distinct from plain `==`: `(x ^ 0) == 1`, `x * 1 == 1`, `(x << 0) == 1`. These are folded into the plain form: `(x | 0) == 1`, `== 1U`, `1 == x`, `(s32)x == 1`, `== (1 | 0)`. `(s32)` gives a second constant register.
4. With `flag` assigned in the else block, test 1 still uses the local and computes its own AND. Moving `flag = D_800E6280.unk_53C[6];` before the if chain (uopt sinks the load) puts `andi v1,a0,1` before test 1. With plain, `^ 0`, `* 1`, `<< 0`, the score was 77 (length 186).
5. 0x1C arm: `if (D_8013C360 == 0) { } else if (D_8013C360 == 1)` (as in the matched sibling func_80137F64) instead of `&& D_8013C360 != 0` brings back the reload. Score 2, one row: `bne a1,t0` against `bne t0,a1` (`D_8013CAA0 == 1`).
6. Last row. uopt's raw output is load first for a local copy (`c = D_8013CAA0; ... c == 1`). Then `_constant_first` (called from `post` in `tools/cvt_pass.py`) swaps it to constant first, because `a1` holds only the constant and `D_8013CAA0`'s location is in `rewritten`. That set covers the whole unit: func_80137BB4's FAKE entry loads of D_8013CAA0 strip its CVTs there. Shown with full-unit ucode dumps (uopt input, raw output, post output). Negatives: `* 1`, `(s32)`, `1 ==`, `*(u8 *)&`, `1U`, `| 0`, `^ 0`, `(u8)` on CAA0; dummy entry loads (plain and view); local copies u8/s32/u32/u16, in the condition or before it, with `(u16)`/`(s32)` casts.
7. Fix (FAKE): the local copy reads the byte through `((u8 *)&D_8013CAA4)[-4]`. That is another location, so it is not in `rewritten` and `post` leaves the order alone. Score 0, then the default build MATCH.

## FAKEs in the matched body (marked in the source)

- The four test spellings (plain, `^ 0`, `* 1`, `<< 0`).
- `D_8011F4DE * 1 == 1`, `D_8011F4D0 * 1 == 1` (from T-25002; without them 2 or 4 rows).
- `c = ((u8 *)&D_8013CAA4)[-4];` for `D_8013CAA0`.

## Tooling note (no change made here)

`_constant_first` uses a unit-wide `rewritten` set. A per-procedure set (the locations whose CVT was stripped in this procedure) would let `c = D_8013CAA0; ... c == 1` match without the D_8013CAA4 view. It needs the lever_lab gate (no matched function may change) before adoption.

## Scratch tools used (not committed)

side.py (full-unit compile of a variant plus an aligned diff), ucode.py / ucodetu.py with dumpshim.py (uopt input, raw output and post output of one procedure, through a scratch shim around cvt_pass.run_uopt). All of them ran in Docker with TOKIMEMO_SCRATCH.
