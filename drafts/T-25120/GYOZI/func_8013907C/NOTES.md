# T-25120: GYOZI func_8013907C (matched)

Result: default C matches, 844 bytes; GYOZI.bin SHA1 OK. The body is in `src/ovl/GYOZI/80138B50.c`.

## Start state

T-24707 guard, 23 resolved rows (`funcdiff --version c`). Two problems:

1. The table value was loaded into `$v1`, stored from `$v1` and copied to `$a0` for func_8008F618. The original loads it into `$a0` and stores from `$a0` in the jal delay slot. Later, the func_8005EBE0 argument used `lui a0; lbu a0` where the original has `lui v1; lbu a0,lo(v1)`.
2. The jump-table dispatch `sll t5,v1,2` belongs in the `== 9` branch slot. as1 filled that slot with the `lui` of D_80147240 instead.

## What closed it

1. **Work pointer, as in the matched VALEN sibling func_80132D0C** (`GameState *state`): `GyoziWork *w = &D_800F53A0;` and `w->unk_F2F` for every access. The assignment value becomes an expression temporary (bit 13, `$a0`), so problem 1's first half is fixed. The later reads become one promoted range of `S33+3887`; its piece over bb5/6/9/13 matches the original's shape (the 3rd load plus the switch value). That piece took `$a0` at cost 1, where the original has `$v1`. The R9COST trace shows `$a0` was cheaper because the bb5 use is the func_8005EBE0 argument in `$a0`. With all `w->` reads the row count was 54, with the first 21 rows correct.
2. **An s32 parameter for func_8005EBE0**: `u32 func_8005EBE0(s32 arg0);` in main_api.h (get_h_tokimeki's shape; the function was an implicit int before). The converted argument is no longer the range itself, so its `$a0` preference goes, and the range takes `$v1` (v0 is not available because of the call result). After this change the only remaining difference was the dispatch `sll`, in 46 shifted rows. `func_8005EBE0((s32)w->unk_F2F)` and `& 0xFF` have the same effect locally. The prototype was kept instead of a FAKE cast, and the file's other callers still match.
3. **`extern volatile u8 D_80147240;` in GYOZI.h (FAKE)**: the as1 lever used for VALEN func_80134030 and GYOZI func_80139E4C. The target reads the byte twice in separate conditional blocks, so volatile does not merge loads here. A `*(volatile u8 *)&` cast adds an `addiu` (190 rows).
4. The volatile changed the local order of one `lui` in func_801393C8, which still had to match. A non-volatile view at its second read, `*(u8 *)&D_80147240 == 1 && D_80147244 < 2U`, restores it (FAKE). The other D_80147240 readers (func_80139764, 80139920.c func_801399C0) still match unchanged. The func_80138C9C draft stays at 75 rows before and after.

## Negatives measured (this session)

- Mixes of direct `D_800F62CF` and `w->` per access: 33 to 251 rows. A 2-statement assignment (`D = x; f(D);`) gave 62.
- On the all-`w->` body: `(u8)`, `+ 0`, `| 0`, `(u16)` and `*(u8 *)&w->` on the func_8005EBE0 argument made no change (54 rows). `(s32)` and `& 0xFF` gave 46 rows, with the as1 issue only.
