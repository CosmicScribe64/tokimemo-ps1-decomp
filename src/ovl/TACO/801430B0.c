#include "common.h"
#include "ovl/TACO.h"

void func_801430B0(void) {
    func_8004ACC8(0x800000);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801430B0", func_801430D0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801430B0", func_801432F0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801430B0", func_80143574);

void func_80143644(void) {
    D_800E7312 = 1;
    if (D_800E7208 & 0x1000) {
        if (D_800E7313 != 0) {
            D_800E7313 -= 1;
        } else {
            D_800E7313 = D_800E7310 - 1;
        }
    }
    if (D_800E7208 & 0x4000) {
        if ((u32)D_800E7313 < (u32)(D_800E7310 - 1)) {
            D_800E7313 += 1;
            return;
        }
        D_800E7313 = 0;
    }
}

void func_801436D4(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = D_8011ECA0 * 0xF0;
    rect.w = 0x200;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0x200, 0);
    func_8009C674(0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801430B0", func_80143730);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801430B0", func_80143814);

void func_801438F0(s32 arg0, s32 arg1, u8 *arg2) {
    s32 pad[3]; /* FAKE: 12 bytes of unused locals above sp28 reproduce the original frame (0x38, sp28 at 0x28); real source unknown. T-4040 */
    s32 sp28;

    sp28 = arg0 + 4;
    func_8009B3C0(sp28);
    func_8009B430(sp28 + 8, arg2 + 4, arg1, sp28);
    *(u8 **)(arg2 + 8) = arg2 + 0x14;
    *(s32 *)(arg2 + 4) = 0;
}

void func_8014394C(void) {
    D_80122760 = 0;
    D_80122764 = 0;
    D_80122768 = -0x64;
    D_8012276C = 0xFF;
    D_8012276D = 0xFF;
    D_8012276E = 0xFF;
    func_8009AD70(0, &D_80122760);
    D_80122770 = 0;
    D_80122774 = 0x64;
    D_80122778 = -0x64;
    D_8012277C = 0xFF;
    D_8012277D = 0xFF;
    D_8012277E = 0xFF;
    func_8009AD70(1, &D_80122770);
    D_80122780 = -0x64;
    D_80122784 = 0;
    D_80122788 = -0x64;
    D_8012278C = 0xFF;
    D_8012278D = 0xFF;
    D_8012278E = 0xFF;
    func_8009AD70(2, &D_80122780);
    func_8009B310(0x800, 0x800, 0x800);
    func_8009B340(0);
}

void func_80143A74(void) {
    func_8009AD30(0x800);
    D_80122740 = 0;
    D_80122744 = 0;
    D_80122748 = 0x800;
    D_8012274C = 0;
    D_80122750 = 0;
    D_80122754 = 0;
    D_80122758 = 0;
    D_8012275C = 0;
    func_80099540(&D_80122740);
    func_8009AD50(-0x64);
    func_8009AD60(0x7FFF);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801430B0", func_80143AF4);

void func_80143B34(s32 arg0) {
    func_80059688((arg0 * 2) + 2);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801430B0", func_80143B58);
