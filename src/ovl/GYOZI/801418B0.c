#include "common.h"
#include "ovl/GYOZI.h"

void func_801418B0(void) {
    D_801487F0 = (u8 *)0x801D6360;
    D_801487F4 = (u8 *)0x801D6384;
    D_801487F8 = (u8 *)0x801D6414;
    D_801487FC = *(s16 *)0x801D641C;
    D_80148800 = (u8 *)0x801B0000;
}

void func_80141900(void) {
    D_80148804 = 0x801DDBB4;
    D_80148808 = 0x801DDBB8;
    D_8014880C = 0x801DDC70;
    D_80148814 = 0x801DB000;
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801418B0", func_80141944);

void func_80141A94(void) {
    func_8004C6A0(0, 0x40);
    func_80050D60(0, 0);
    func_80081D30(1);
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801418B0", func_80141ACC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801418B0", func_8014206C);

void func_8014210C(void) {
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];

    a[0] = -0x96;
    a[1] = -0x48;
    a[2] = 6;
    a[3] = 0x54;
    b[0] = b[1] = b[2] = b[3] = -0x40;
    c[0] = c[1] = c[2] = c[3] = 0x40;
    d[0] = d[1] = d[2] = d[3] = 0x40;
    func_8005BD20(0, 4, a, b, c, d);
    D_8012E6C0 = 1;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801418B0", func_801421B4);

void func_801425F4(void) {
    func_8008A0D4(0x4183);
    func_8004DE1C();
}

void func_8014261C(void) {
    s16 i;

    for (i = 0; i < 6; i++) {
        D_80129F40[i * 0x44 + 0x1983] = 0;
    }
    D_80148820 = 0;
    D_80148824 = 0;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801418B0", func_8014267C);

void func_80142790(void) {
    if (D_80148828[D_80148818] == 0xB) {
        func_80086AB0(0x200);
    } else {
        func_80086AB0(0x201);
    }
    func_8004DE1C();
}

void func_801427E4(void) {
    s16 v;

    v = D_8012B920 + 0x200;
    D_8012B920 = v;
    if (v >= 0x1000) {
        func_8004DE1C();
    }
}

void func_80142828(void) {
    func_80051DD8(0x4D, 0x801B0000, 0xB0C5);
    func_8004DE1C();
}

void func_80142858(void) {
    func_80051DD8(0xE, 0x801D7000, 0xB112);
    func_8004DE1C();
}

void func_8014288C(void) {
    D_80148624 = 0;
    if (D_80148828[D_80148818] == 0xB) {
        D_80148620 = 0x27;
    } else {
        D_80148620 = 0x26;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801418B0", func_801428EC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801418B0", func_80142A8C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801418B0", func_80142C14);
