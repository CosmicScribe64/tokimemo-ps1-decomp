#include "common.h"
#include "ovl/DATE.h"

void func_8014CC40(void) {
    D_8015E220 = 0x801CE0F4;
    D_8015E224 = 0x801CE0F8;
    D_8015E228 = 0x801CE118;
    D_8015E22C = *(s16 *)0x801CE12C;
    D_8015E230 = 0x801B0000;
    D_8015E234 = 0x801B2000;
    D_8015E238 = 0x801B6000;
    D_8015E23C = 0x801BA000;
    D_8015E240 = 0x801BE000;
    D_8015E244 = 0x801C2000;
    D_8015E248 = 0x801C6000;
}

void func_8014CCF0(void) {
    D_8015E24C = 0x801D2184;
    D_8015E250 = 0x801D218C;
    D_8015E254 = 0x801D21B4;
    D_8015E258 = *(s16 *)0x801D21C8;
    D_8015E25C = 0x801B0000;
    D_8015E260 = 0x801B2000;
    D_8015E264 = 0x801B6000;
    D_8015E268 = 0x801BA000;
    D_8015E26C = 0x801BE000;
    D_8015E270 = 0x801C2000;
    D_8015E274 = 0x801C6000;
}

void func_8014CDA0(void) {
    D_8015E278 = 0x801CE080;
    D_8015E27C = 0x801CE084;
    D_8015E280 = 0x801CE094;
    D_8015E284 = *(s16 *)0x801CE0A8;
    D_8015E288 = 0x801B0000;
    D_8015E28C = 0x801B2000;
    D_8015E290 = 0x801B6000;
    D_8015E294 = 0x801BA000;
    D_8015E298 = 0x801BE000;
    D_8015E29C = 0x801C2000;
    D_8015E2A0 = 0x801C6000;
}

void func_8014CE50(void) {
    D_8015E2A4 = 0x801CE0C0;
    D_8015E2A8 = 0x801CE0C4;
    D_8015E2AC = 0x801CE0DC;
    D_8015E2B0 = *(s16 *)0x801CE0F0;
    D_8015E2B4 = 0x801B0000;
    D_8015E2B8 = 0x801B2000;
    D_8015E2BC = 0x801B6000;
    D_8015E2C0 = 0x801BA000;
    D_8015E2C4 = 0x801BE000;
    D_8015E2C8 = 0x801C2000;
    D_8015E2CC = 0x801C6000;
}

void func_8014CF00(void) {
    D_8015E2D0 = 0x801D2240;
    D_8015E2D4 = 0x801D2248;
    D_8015E2D8 = 0x801D2298;
    D_8015E2DC = *(s16 *)0x801D22B0;
    D_8015E2E0 = 0x801B0000;
    D_8015E2E4 = 0x801B2000;
    D_8015E2E8 = 0x801B6000;
    D_8015E2EC = 0x801BA000;
    D_8015E2F0 = 0x801BE000;
    D_8015E2F4 = 0x801C2000;
    D_8015E2F8 = 0x801C6000;
}

void func_8014CFB0(void) {
    D_8015E2FC = 0x801D2074;
    D_8015E300 = 0x801D2078;
    D_8015E304 = 0x801D2088;
    D_8015E308 = *(s16 *)0x801D6094;
    D_8015E30C = 0x801D0000;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014D000);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014D0A0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014D298);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014D57C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014D6B0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014D818);

void func_8014D970(void) {
    bg_read_sub2(0x484E);
    func_8004284C();
}

void func_8014D998(void) {
    bg_read_sub2(0x44D6);
    func_8004284C();
}

void func_8014D9C0(void) {
    func_80044750(0x204);
    bg_read_sub2(0x480F);
    func_8004284C();
}

void func_8014D9F0(void) {
    bg_read_sub2(0x436B);
    func_8004284C();
}

void func_8014DA18(void) {
    bg_read_sub2(0x483C);
    func_8004284C();
}

void func_8014DA40(void) {
    bg_read_sub2(0x4470);
    func_8004284C();
}

void func_8014DA68(void) {
    bg_read_sub2(0x4466);
    func_8004284C();
}

void func_8014DA90(void) {
    bg_read_sub2(0x48A3);
    func_8004284C();
}

void func_8014DAB8(void) {
    bg_read_sub2(0x456C);
    func_8004284C();
}

void func_8014DAE0(void) {
    bg_read_sub2(0x4880);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014DB08);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014DB98);

void func_8014DC5C(void) {
    func_80046318(0xD, 0x801D0000, 0x90FC);
    func_8014CFB0();
    func_8004284C();
}

void func_8014DC94(void) {
    func_80046318(0x3D, 0x801B0000, 0x7DD3);
    func_8014CC40();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014DCCC);

void func_8014DDD8(void) {
    func_80046318(0x1D, 0x801B0000, 0xBD41);
    func_80132458();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014DE10);

void func_8014DF1C(void) {
    func_80046318(0x45, 0x801B0000, 0x7E10);
    func_8014CCF0();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014DF54);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014DFB0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014E010);

void func_8014E108(void) {
    func_8014EF1C(-0x96, -0x78, -0x9B, -0x78, -0x50, 0x28, -0x69, 0x28, 0xFFFF00, 0xFFFF00, 9, 1);
    func_8014EF1C(-0xA0, -0x78, -0xA5, -0x78, -0x64, 0x28, -0x73, 0x28, 0xFFFFFF, 0xFFFFFF, 9, 1);
    func_8014EF1C(-0xA2, -0x78, -0xA7, -0x78, -0x6E, 0x28, -0x87, 0x28, 0xFFFF00, 0xFFFF00, 9, 1);
    dtd_on_tpage(0, 0, 9, 3, 0);
    func_8014EF1C(-0x7D, -0x78, -0x82, -0x78, -0x28, 0x28, -0x32, 0x28, 0xFFFF00, 0xFFFF00, 0xD, 1);
    func_8014EF1C(-0x84, -0x78, -0x89, -0x78, -0x3C, 0x28, -0x46, 0x28, 0xFFFFFF, 0xFFFFFF, 0xD, 1);
    dtd_on_tpage(0, 0, 0xD, 3, 0);
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014E2E8);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014E38C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014E3E8);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014EBB8);

void func_8014EC34(void) {
    func_8014C5C8();
    if (((u16) D_800CA154 == 5) && (D_800E7384 == 1)) {
        func_80083440(1);
    }
    D_800E7384 += 1;
}

void func_8014EC90(void) {
    func_80044750(0x503);
    func_8004284C();
}

void func_8014ECB8(void) {
    func_80046318(0x3D, 0x801B0000, 0x7E92);
    func_8014CE50();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014ECF0);

void func_8014EE1C(void) {
    event_face1();
    D_800B5A60 = 0;
}

void func_8014EE40(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_8014EE68(void) {
    func_80044750(0x201);
    func_8004284C();
}

void func_8014EE90(void) {
    func_80046318(0x45, 0x801B0000, 0x7ECF);
    func_8014CF00();
    func_8004284C();
}

void func_8014EEC8(void) {
    func_8014C5C8();
    if ((u16) D_800CA154 == 4) {
        D_800B5A60 = 1;
        event_face0();
        D_800E738A -= 1;
    }
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014EF1C);
