#include "common.h"
#include "ovl/GYOZI.h"

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80140C90", func_80140C90);

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
