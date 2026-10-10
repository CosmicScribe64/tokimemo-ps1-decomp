#include "common.h"
#include "ovl/BUNKAKEN.h"

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80144F70", func_80144F70);

void func_80145500(void) {
    if (D_800E738A != 0) {
        D_800E738A = 0;
        D_80122EC0 += 1;
    }
}

void func_80145530(void) {
    D_800E7389 = 0;
    D_800E738A = 0;
    D_80122EC0 = 8;
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80144F70", func_80145550);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80144F70", func_80145844);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80144F70.rodata", D_80154B84);

void func_801459FC(void) {
    k_reset(0);
    func_8006612C(&D_80154B84);
    k_disp_start(1);
}

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80144F70.rodata", D_80154B8C);

void func_80145A30(void) {
    k_reset(0);
    func_8006612C(&D_80154B8C);
    func_80065F34(0);
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80144F70", func_80145A64);
