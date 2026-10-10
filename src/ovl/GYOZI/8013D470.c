#include "common.h"
#include "ovl/GYOZI.h"

void func_8013D470(void) {
    func_8008A0D4(0x4040);
    func_8004DE1C();
}

void func_8013D498(void) {
    func_80072734(0x6346);
    func_8004DE1C();
}

void func_8013D4C0(void) {
    func_80072734(0x6157);
    func_8004DE1C();
}

void func_8013D4E8(void) {
    if (((u32) D_800F5488 >> 0x1C) == 9) {
        D_801474A8 = 5;
    }
    func_8004DE1C();
}

void func_8013D528(void) {
    D_801474A8 = 3;
    func_8004DE1C();
}

void func_8013D550(void) {
    switch (D_801474B8) {
    case 0:
        func_8013D5B0();
        return;
    case 1:
        func_8013D62C();
        return;
    default:
        func_8013D6B4();
        return;
    }
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013D470", func_8013D5B0);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013D470", func_8013D62C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013D470", func_8013D6B4);

void func_8013D73C(void) {
    func_80072734(0x658F);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013D470", func_8013D764);

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013D470.rodata", D_80145C20);

void func_8013D884(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145C20);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013D470.rodata", D_80145C28);

void func_8013D8C8(void) {
    D_801474A8 = 0xC;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145C28);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013D470.rodata", D_80145C34);

void func_8013D910(void) {
    D_801474A8 = 0xF;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145C34);
    func_8004DE1C();
}

void func_8013D958(void) {
    func_8008A0D4(0x3FF5);
    func_8004DE1C();
}

void func_8013D980(void) {
    func_8008A0D4(0x4037);
    func_8004DE1C();
}

void func_8013D9A8(void) {
    if (((u32) D_800F563A >> 4) == 7) {
        D_801474A8 = 6;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013D470", func_8013D9E8);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013D470", func_8013DA58);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013D470", func_8013DAA4);
