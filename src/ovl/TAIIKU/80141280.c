#include "common.h"
#include "ovl/TAIIKU.h"

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80141280", func_80141280);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80141280", func_80141964);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80141280", func_801419F8);

void func_80141AE8(void) {
    D_80122EC8 = 9;
    D_800CA148 = 2;
    D_800CA14C = 0;
    k_reset(1);
}

void func_80141B28(void) {
    D_80122EC8 = 0xB;
    D_800CA148 = 1;
    D_800CA14C = 0;
    k_reset(1);
}
