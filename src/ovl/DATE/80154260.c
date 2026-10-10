#include "common.h"
#include "ovl/DATE.h"

void func_80154260(void) {
    D_8015F520 = 0x801D2240;
    D_8015F524 = 0x801D2248;
    D_8015F528 = 0x801D2288;
    D_8015F52C = *(s16 *)0x801D22A0;
    D_8015F530 = 0x801B0000;
    D_8015F534 = 0x801B2000;
    D_8015F538 = 0x801B6000;
    D_8015F53C = 0x801BA000;
    D_8015F540 = 0x801BE000;
    D_8015F544 = 0x801C2000;
    D_8015F548 = 0x801C6000;
}

void func_80154310(void) {
    D_8015F54C = 0x801D21E0;
    D_8015F550 = 0x801D21E8;
    D_8015F554 = 0x801D2228;
    D_8015F558 = *(s16 *)0x801D2240;
    D_8015F55C = 0x801B0000;
    D_8015F560 = 0x801B2000;
    D_8015F564 = 0x801B6000;
    D_8015F568 = 0x801BA000;
    D_8015F56C = 0x801BE000;
    D_8015F570 = 0x801C2000;
    D_8015F574 = 0x801C6000;
}

void func_801543C0(void) {
    D_8015F578 = 0x801CE078;
    D_8015F57C = 0x801CE07C;
    D_8015F580 = 0x801CE08C;
    D_8015F584 = *(s16 *)0x801CE098;
    D_8015F588 = 0x801B0000;
    D_8015F58C = 0x801B2000;
    D_8015F590 = 0x801B6000;
    D_8015F594 = 0x801BA000;
    D_8015F598 = 0x801BE000;
    D_8015F59C = 0x801C2000;
    D_8015F5A0 = 0x801C6000;
}

void func_80154470(void) {
    D_8015F5A4 = 0x801CE148;
    D_8015F5A8 = 0x801CE14C;
    D_8015F5AC = 0x801CE170;
    D_8015F5B0 = *(s16 *)0x801CE184;
    D_8015F5B4 = 0x801B0000;
    D_8015F5B8 = 0x801B2000;
    D_8015F5BC = 0x801B6000;
    D_8015F5C0 = 0x801BA000;
    D_8015F5C4 = 0x801BE000;
    D_8015F5C8 = 0x801C2000;
    D_8015F5CC = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80154260", func_80154520);

void func_801545B4(void) {
    D_8015B68C = 3;
    D_8015E208 = D_8015DEE8;
    D_8015E20C = D_8015E024;
    D_8015E210 = D_8015E160;
    func_8004284C();
}

void func_8015460C(void) {
    func_80154260();
    load_palette(D_8015F530, 0x11, 1, 2, 0);
    func_80084E90(D_8015F534, D_8015F538, D_8015F53C, D_8015F540, D_8015F544, D_8015F548);
    func_800850D4(D_8015F524, D_8015F528, D_8015F520, D_8015F52C);
    D_800CA360 = 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80154260", func_801546BC);

void func_8015485C(void) {
    D_8015E208 = D_8015DEF0;
    D_8015E20C = D_8015E02C;
    D_8015E210 = D_8015E168;
    func_801543C0();
    func_80043914(D_8015F588, 0x11, 1, 2, 0);
    func_80084E90(D_8015F58C, D_8015F590, D_8015F594, D_8015F598, D_8015F59C, D_8015F5A0);
    func_800850D4(D_8015F57C, D_8015F580, D_8015F578, D_8015F584);
    D_80120688 |= 0x80000000;
    D_80120666 = 0;
    D_800E6280.unk_1BC[1].unk_02 += 2;
    D_800E6280.unk_1BC[1].unk_06 += 2;
    D_800E6280.unk_1BC[1].unk_0A -= 0x14;
    func_80084D3C();
    func_8004284C();
}

void func_80154990(void) {
    D_8015E208 = D_8015DEF4;
    D_8015E20C = D_8015E030;
    D_8015E210 = D_8015E16C;
    func_80043914(D_8015F5B4, 0x11, 1, 2, 0);
    func_80043914(D_8015F5B4, 0x12, 1, 2, 1);
    func_80084E90(D_8015F5B8, D_8015F5BC, D_8015F5C0, D_8015F5C4, D_8015F5C8, D_8015F5CC);
    func_800850D4(D_8015F5A8, D_8015F5AC, D_8015F5A4, (s32) D_8015F5B0);
    D_800E6280.unk_1BC[1].unk_02 += 3;
    D_800E6280.unk_1BC[1].unk_06 += 2;
    D_800E6280.unk_1BC[1].unk_0A -= 0x14;
    func_80084D3C();
    D_8015F5D0 = 0;
    func_8004284C();
}

void func_80154AC4(void) {
    bg_read_sub2(0x4359);
    func_8004284C();
}

void func_80154AEC(void) {
    bg_read_sub2(0x4809);
    func_8004284C();
}

void func_80154B14(void) {
    bg_read_sub2(0x432B);
    func_8004284C();
}

void func_80154B3C(void) {
    bg_read_sub2(0x48BD);
    func_8004284C();
}

void func_80154B64(void) {
    bg_read_sub2(0x45A3);
    func_8004284C();
}

void func_80154B8C(void) {
    bg_read_sub2(0x454E);
    func_8004284C();
}

void func_80154BB4(void) {
    bg_read_sub2(0x4876);
    func_8004284C();
}

void func_80154BDC(void) {
    bg_read_sub2(0x4544);
    func_8004284C();
}

void func_80154C04(void) {
    bg_read_sub2(0x4833);
    func_8004284C();
}

void func_80154C2C(void) {
    bg_read_sub2(0x43F9);
    func_8004284C();
}

void func_80154C54(void) {
    func_80044750(0x500);
    bg_read_sub2(0x43F0);
    func_8004284C();
}

typedef struct {
    void (*f[67])();
} FnTbl67; /* size 0x10C */
extern FnTbl67 D_8015F5D4;

void func_80154C84(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl67 tbl;

    tbl = D_8015F5D4;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80154D00(void) {
    func_80046318(0x45, 0x801B0000, 0x8664);
    func_80154260();
    func_8004284C();
}

void func_80154D38(void) {
    if (D_80122CDC == 1) {
        func_80083440(2);
        func_80042940(0x2E);
        return;
    }
    D_800E6280.unk_56C[9] = 1;
    D_800CA224 = 3;
    D_800CA226 = 3;
    D_800CA228 = 3;
    D_800CA234 = 2;
    D_800CA236 = 2;
    D_800CA238 = 2;
    D_800E6280.unk_1BC[1].unk_02 -= 2;
    D_800E6280.unk_1BC[1].unk_06 -= 1;
    D_800E6280.unk_1BC[1].unk_0A += 0x14;
    func_80084D3C();
    func_8004284C();
}

void func_80154E14(void) {
    func_800AE0F0(D_800CA1DC, "絶叫マシーンビビール");
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80154260", func_80154E48);

void func_80154F0C(void) {
    func_80046318(0x45, 0x801B0000, 0x86A9);
    func_80154310();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80154260", func_80154F44);

void func_80154FCC(void) {
    func_80046318(0x3D, 0x801B0000, 0x86EE);
    func_801543C0();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80154260", func_80155004);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80154260", func_80155174);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80154260", func_801551D0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80154260", func_8015522C);

void func_80155284(void) {
    func_80146224();
}
