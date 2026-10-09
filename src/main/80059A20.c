#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80059A20", func_80059A20);

void func_80059B04(s32 arg0) {
    printf(D_800AFDF0, arg0);
    D_800B3D54[D_800B3D24] = 1;
}

INCLUDE_ASM("asm/nonmatchings/main/80059A20", func_80059B40);

void func_80059BC0(void) {
    func_80059BE8();
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/80059A20", func_80059BE8);

INCLUDE_ASM("asm/nonmatchings/main/80059A20", func_80059E00);

INCLUDE_ASM("asm/nonmatchings/main/80059A20", func_80059EF0);

void func_8005A06C(void) {
    if (D_800E7389 == 0) {
        func_80059B40();
        return;
    }
    func_80042878(0x12);
}
