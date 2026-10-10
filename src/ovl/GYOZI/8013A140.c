#include "common.h"
#include "ovl/GYOZI.h"

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013A140", func_8013A140);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013A140", func_8013A2F4);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013A140", func_8013A4E8);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013A140", func_8013A5F4);

void func_8013A76C(void) {
    if (func_80050AB8() == 1) {
        func_8004DDD8();
    }
}

void func_8013A7A0(void) {
    func_80086AB0(0x200);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013A140", func_8013A7C8);

void func_8013A820(void) {
    D_800D9234 = (u8 *)&D_801474A8;
    D_800D9238 = (u8 *)&D_801474AC;
    D_800D923C = D_8014749C;
    D_800D9240 = D_801474A0;
    D_800D9244 = D_801474A4;
    func_8008D610(D_8012E66C, 1, 0);
}

void func_8013A89C(void) {
    D_800D9234 = (u8 *) &D_801474A8;
    D_800D9238 = (u8 *) &D_801474AC;
    D_800D923C = D_8014749C;
    D_800D9240 = D_801474A0;
    D_800D9244 = D_801474A4;
    func_8008D610(D_8012E66C, 1, 1);
}
