#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    s32 (*f[26])();
} FnTbl26R; /* size 0x68 */
extern FnTbl26R D_8014877C;

/* The record fields at 0x8012B8C7..0x8012B8D8 are reached through one base: the original keeps the
 * stores and loads in program order (separate scalars let as1 hoist the loads). The result of the
 * last operation is left in $v0 without a return statement (T-4060). */
s16 func_80140C90(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl26R tbl;

    tbl = D_8014877C;
    idx = D_800F647A;
    tbl.f[idx](0x80);
    *(s8 *)(&D_8012B8C2 + 5) = *(u8 *)&D_801317EB;
    if ((*(s16 *)(&D_8012B8C2 + 0x16) == 0) && (*(s16 *)(&D_8012B8C2 + 6) == 0)) {
        *(s16 *)(&D_8012B8C2 + 0x14) = *(s16 *)(&D_8012B8C2 + 0x14) + 1;
        if (*(s16 *)(&D_8012B8C2 + 0x14) >= 3) {
            *(s16 *)(&D_8012B8C2 + 0x14) = 0;
        }
    }
}

void func_80140D7C(void) {
    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    if (((u8)func_8005E0E0(D_800F62CF) & 0x7F) != 4) {
        func_8004DE1C();
        return;
    }
    func_8013FD20();
}

void func_80140DCC(void) {
    if (D_80148750 != 0) {
        D_80148620 += 1;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80140C90", func_80140E0C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80140C90", func_80140F90);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80140C90", func_801412A4);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80140C90", func_80141550);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80140C90", func_80141790);
