#include "common.h"
#include "game.h"

s32 func_800451D0(void) {
    return D_801255D8;
}

void func_800451E0(s32 arg0) {
    D_801255D8 = arg0;
}

void func_800451EC(void) {
    D_8012512A = 0x80;
    D_800B3D68 = func_80087E4C(1, D_801255C0);
}

void func_80045224(void) {
    switch (func_80087954(1, D_801255C8)) {
    case 2:
        D_8012512A |= 2;
        break;
    case 5:
        D_8012512B = -1;
        break;
    }
}

s32 func_80045288(void) {
    if (D_80125129 == D_80125128 && D_8012512B == -1) {
        return 1;
    }
    return 0;
}

void func_800452C4(void) {
    D_80125128 = 0;
    D_80125129 = 0;
    D_8012512A = 2;
    D_8012512B = -1;
    D_801255B0 = 0;
    D_801255B4 = 0;
    D_801255B5 = 0;
    D_800B3D60 = 0;
    D_800B3D6C = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/800451D0", func_80045318);

INCLUDE_ASM("asm/nonmatchings/main/800451D0", func_80045414);

INCLUDE_ASM("asm/nonmatchings/main/800451D0", func_800455C4);

u8 func_80046094(void) {
    return D_80125130[(D_80125128 + 0x2FU) % 0x30U * 24];
}

u8 func_800460CC(void) {
    return D_8012512A;
}

u8 func_800460DC(void) {
    return D_801255B5;
}

u8 func_800460EC(void) {
    return D_801255B4;
}

INCLUDE_ASM("asm/nonmatchings/main/800451D0", func_800460FC);

s32 func_80046274(void) {
    return D_801255B0;
}

s32 *func_80046284(void) {
    return &D_800B3D70;
}

INCLUDE_ASM("asm/nonmatchings/main/800451D0", func_80046290);

void func_800462BC(u8 arg0, s32 arg1, s32 *arg2) {
    *arg2 = arg1;
    ((u8 *)arg2)[3] = arg0;
}

void func_800462C8(u8 arg0, s32 arg1, s32 arg2) {
    /* FAKE: size 0x20 is taken from the original frame (local at 0x28 in 0x48);
     * the real type of this buffer is unknown. T-0016 */
    u8 buf[0x20];

    func_80044750(6);
    func_800462BC(arg0, arg1, (s32 *)buf);
    func_80045414(6, arg2, buf);
    D_800B3D60 = 0;
}

void func_80046318(u8 arg0, s32 arg1, s32 arg2) {
    /* FAKE: size 0x20 is taken from the original frame (local at 0x28 in 0x48);
     * the real type of this buffer is unknown. T-0016 */
    u8 buf[0x20];

    func_80044750(6);
    func_800462BC(arg0, arg1, (s32 *)buf);
    func_80045414(6, arg2, buf);
    D_800B3D60 = 1;
}

INCLUDE_ASM("asm/nonmatchings/main/800451D0", func_8004636C);

INCLUDE_ASM("asm/nonmatchings/main/800451D0", func_800463E8);

INCLUDE_ASM("asm/nonmatchings/main/800451D0", func_80046478);
