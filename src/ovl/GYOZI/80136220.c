#include "common.h"
#include "ovl/GYOZI.h"

void func_80136220(void) {
    D_80146150 = (s32 *)0x801DABCC;
    D_80146154 = (s32 *)0x801DB66C;
    D_80146158 = (s32 *)0x801DABD4;
    D_8014615C = (s32 *)0x801DB688;
    D_80146160 = (s32 *)0x801DAC34;
    D_80146164 = (s32 *)0x801DB6B4;
    D_80146170 = 0x801C0000;
    D_80146174 = 0x801C2000;
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80136220", func_801362A0);

void func_801363BC(void) {
    func_80136220();
    if ((*D_80146150 & 0x80000000) && (*D_80146158 & 0x80000000) && (*D_80146160 & 0x80000000)) {
        func_8004DE1C();
        return;
    }
    D_800F647A -= 2;
}

void func_80136454(void) {
    func_80090960(5, 2);
    func_8008A0D4(0x419F);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80136220", func_80136488);

void func_801366E4(void) {
    func_80086AB0(0x201);
    func_8004DE1C();
}

void func_8013670C(void) {
    func_80086AB0(0x200);
    func_8004DE1C();
}

void func_80136734(void) {
    if (D_800F62CF == 2) {
        func_8004DE1C();
    }
    func_8004DE1C();
}

void func_8013676C(void) {
    if ((D_80146178 == 3) || (D_80146178 == 4)) {
        D_80145EB4 += 2;
    } else if (D_80146178 != 0) {
        D_80145EB4 += 1;
    }
    func_8004DE1C();
}

void func_801367D4(void) {
    if (D_80146178 == 0) {
        D_80145EB4 += 2;
    } else if ((D_80146178 != 3) && (D_80146178 != 4)) {
        D_80145EB4 += 1;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80136220", func_8013683C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80136220", func_801368B8);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80136220", func_80137188);

void func_80137238(void) {
    func_800549E8(0x60);
    func_800549E8(0x61);
    func_800549E8(0x62);
    func_800549E8(0x63);
    D_801461A0 = 0;
    func_8004DE1C();
}

void func_8013727C(void) {
    func_80051DD8(0x37, 0x801C0000, 0xB081);
    func_8004DE1C();
}
