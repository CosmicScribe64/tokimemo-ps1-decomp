#include "common.h"
#include "ovl/GYOZI.h"

void func_8013AE80(void) {
    switch (D_801474B8) {
    case 0:
        func_8013AF1C();
        return;
    case 1:
        func_8013B03C();
        return;
    case 2:
        func_8013B0C4();
        return;
    case 3:
        func_8013B2B8();
        return;
    case 4:
        func_8013B340();
        return;
    default:
        func_8013B3B4();
        return;
    }
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013AE80", func_8013AF1C);

void func_8013AFE8(void) {
    func_8013A820();
    if (D_801474AC == 2) {
        if (D_800F6474++ == 0) {
            func_80086AB0(0x501);
        }
    }
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013AE80", func_8013B03C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013AE80", func_8013B0C4);

void func_8013B1D0(void) {
    func_8013A820();
    if (D_800F6474++ == 0) {
        if (D_801474AC == 3) {
            func_80086AB0(0x500);
        }
    }
}

void func_8013B228(void) {
    func_80086AB0(0x500);
    func_8004DE1C();
}

void func_8013B250(void) {
    func_80086AB0(0x501);
    func_8004DE1C();
}

void func_8013B278(void) {
    if (D_80147650 != 0) {
        D_801474A8 += 1;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013AE80", func_8013B2B8);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013AE80", func_8013B340);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013AE80", func_8013B3B4);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013AE80", func_8013B43C);

void func_8013B55C(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\213\263\216\272");
    func_8004DE1C();
}

void func_8013B5A0(void) {
    D_801474A8 = 3;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\213\263\216\272");
    func_8004DE1C();
}

void func_8013B5E8(void) {
    D_801474A8 = 6;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_800BCE10(&D_800D92E0, "\216\300\214\261\216\272");
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013AE80", func_8013B644);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013AE80", func_8013B6CC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013AE80", func_8013B740);

void func_8013B7B4(void) {
    func_8008A0D4(0x3FFF);
    func_8004DE1C();
}

void func_8013B7DC(void) {
    func_8008A0D4(0x3FE3);
    func_8004DE1C();
}

void func_8013B804(void) {
    if (D_80147650 != 0) {
        func_8008A0D4(0x406F);
    } else {
        func_8008A0D4(0x4065);
    }
    func_8004DE1C();
}

void func_8013B848(void) {
    func_8008A0D4(0x4065);
    func_8004DE1C();
}

void func_8013B870(void) {
    func_8008A0D4(0x406F);
    func_8004DE1C();
}

void func_8013B898(void) {
    if (D_80147650 != 0) {
        D_801474A8 += 1;
    }
    func_8004DE1C();
}

void func_8013B8D8(void) {
    if (D_80147650 == 0) {
        D_801474A8 += 1;
    }
    func_8004DE1C();
}
