#include "common.h"
#include "ovl/GYOZI.h"

void func_80142D00(void) {
    D_801488F0 = 0x801B21E0;
    D_801488F4 = 0x801B21E4;
    D_801488F8 = 0x801B2228;
    D_80148900 = 0x801B0000;
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80142D00", func_80142D40);

void func_80142DF0(void) {
    func_80051DD8(0xD, 0x801B0000, 0xAFE0);
    func_80142D00();
    func_8004DE1C();
}

void func_80142E28(void) {
    func_80086AB0(0x203);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80142D00", func_80142E50);

void func_80142F6C(void) {
    func_80142D00();
    func_8004EE18(D_80148900, 0x11, 1, 2, 0);
    func_800549E8(0x60);
    D_8012B8C1 = 1;
    D_8012B8C2 = 0;
    D_8012B8F8 = 0x01000000;
    D_8012B8C3 = 0x84;
    D_8012B8CC = D_801488F4;
    D_8012B8D0 = D_801488F8;
    D_8012B8F4 = D_801488F0;
    D_8012B8D4 = D_801488FC;
    D_8012B8D6 = 0;
    D_8012B8D8 = 0;
    D_8012B8C6 = 1;
    D_8012B903 = 0x11;
    D_8012B8E6 = -0xA0;
    D_8012B8EA = -0x78;
    D_8012B8C7 = 0;
    D_8012B8C5 = 0;
    D_8012B8C4 = 0;
    D_80148620 = 0x13;
    D_80148614 = D_801481FC;
    D_80148618 = D_80148230;
    D_8014861C = D_80148264;
    func_8004DE1C();
}

void func_801430B4(void) {
    func_8008A0D4(0x417B);
    func_8004DE1C();
}

void func_801430DC(void) {
    D_8012B8C2 = 9;
    func_8004DE1C();
}
