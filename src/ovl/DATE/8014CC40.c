#include "common.h"
#include "ovl/DATE.h"

typedef struct {
    void (*f[43])();
} FnTbl43; /* size 0xAC */
extern FnTbl43 D_8015E530;

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

void func_8014D000(void) {
    switch (D_80122CD0) {
    case 2:
        func_8014DB08();
        return;
    case 3:
        func_8014DCCC();
        return;
    case 4:
        func_8014E2E8();
        return;
    case 5:
        func_8014EBB8();
        return;
    case 6:
        func_8014ECF0();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8014D0A0(void) {
    D_8015E208 = D_8015DE54;
    D_8015E20C = D_8015DF90;
    D_8015E210 = D_8015E0CC;
    D_800E6280.unk_1BC[8].unk_06 += 3;
    D_800E6280.unk_1BC[8].unk_0A -= 0xA;
    func_80084D3C();
    func_8014CC40();
    func_80043914(D_8015E230, 0x11, 1, 2, 0);
    func_80084E90(D_8015E234, D_8015E238, D_8015E23C, D_8015E240, D_8015E244, D_8015E248);
    func_800850D4(D_8015E224, D_8015E228, D_8015E220, (s32) D_8015E22C);
    D_800CA360 = 1;
    func_8014CFB0();
    func_80043914(D_8015E30C, 0x12, 1, 2, 0);
    func_80048F64(0x62);
    D_801206D9 = 0xC;
    D_801206DA = 0;
    D_80120710 = 0x41000000;
    D_801206DB = 4;
    D_801206E4 = D_8015E300;
    D_8012070C = D_8015E2FC;
    D_801206E8 = D_8015E304;
    D_801206EC = D_8015E308;
    D_801206EE = 0;
    D_801206F0 = 0;
    D_801206DE = 1;
    D_8012071B = 0x12;
    D_801206FE = -0xA0;
    D_80120702 = -0x78;
    D_801206DF = 0;
    D_801206DD = 0;
    D_8015E310 = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014D298);

void func_8014D57C(void) {
    D_8015E310 = 0;
    D_8015E314 = 3;
    D_8015E208 = D_8015DE5C;
    D_8015E20C = D_8015DF98;
    D_8015E210 = D_8015E0D4;
    D_800E6280.unk_1BC[8].unk_02 += 1;
    D_800E6280.unk_1BC[8].unk_06 += 1;
    D_800E6280.unk_1BC[8].unk_0A -= 0x14;
    func_80084D3C();
    func_8014CDA0();
    func_80043914(D_8015E288, 0x11, 1, 2, 0);
    func_80084E90(D_8015E28C, D_8015E290, D_8015E294, D_8015E298, D_8015E29C, D_8015E2A0);
    func_800850D4(D_8015E27C, D_8015E280, D_8015E278, (s32) D_8015E284);
    D_800CA360 = 1;
    func_8004284C();
}

void func_8014D6B0(void) {
    D_8015E208 = D_8015DE60;
    D_8015E20C = D_8015DF9C;
    D_8015E210 = D_8015E0D8;
    D_800E6280.unk_1BC[8].unk_02 += 1;
    D_800E6280.unk_1BC[8].unk_06 += 1;
    D_800E6280.unk_1BC[8].unk_0A -= 0x14;
    func_80084D3C();
    func_8014CE50();
    func_80043914(D_8015E2B4, 0x11, 1, 2, 0);
    func_80084E90(D_8015E2B8, D_8015E2BC, D_8015E2C0, D_8015E2C4, D_8015E2C8, D_8015E2CC);
    func_800850D4(D_8015E2A8, D_8015E2AC, D_8015E2A4, D_8015E2B0);
    D_800CA360 = 1;
    D_800CA224[0] = 3;
    D_800CA224[1] = 3;
    D_800CA224[2] = 3;
    D_800CA234[0] = 2;
    D_800CA234[1] = 2;
    D_800CA234[2] = 2;
    func_8004284C();
}

void func_8014D818(void) {
    D_8015E208 = D_8015DE64;
    D_8015E20C = D_8015DFA0;
    D_8015E210 = D_8015E0DC;
    func_8014CF00();
    func_80043914(D_8015E2E0, 0x11, 1, 2, 0);
    func_80084E90(D_8015E2E4, D_8015E2E8, D_8015E2EC, D_8015E2F0, D_8015E2F4, D_8015E2F8);
    func_800850D4(D_8015E2D4, D_8015E2D8, D_8015E2D0, D_8015E2DC);
    D_80120650[4] = D_80120650[0x48] = 8;
    D_800E6280.unk_1BC[8].unk_06 += 2;
    D_800E6280.unk_1BC[8].unk_0A -= 0x14;
    func_80084D3C();
    D_800CA224[0] = 2;
    D_800CA224[1] = 2;
    D_800CA224[2] = 2;
    D_800CA234[0] = 1;
    D_800CA234[1] = 1;
    D_800CA234[2] = 1;
    func_8004284C();
}

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

typedef struct {
    void (*f[42])();
} FnTbl42; /* size 0xA8 */
extern FnTbl42 D_8015E31C;

void func_8014DB08(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_8015E31C;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    if (D_80122D04 != 0) {
        func_8014DB98();
    }
}

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

void func_8014DE10(void) {
    func_80132458();
    func_80043914(D_8015B644, 0x11, 1, 3, 0);
    func_80048F64(0x60);
    D_80120651 = 0xC;
    D_80120652 = 0x40;
    D_80120688 = 0x41000000;
    D_80120653 = 4;
    D_8012065C = D_8015B638;
    D_80120660 = D_8015B63C;
    D_80120684 = D_8015B634;
    D_80120664 = D_8015B640;
    D_80120666 = 0;
    D_80120668 = 0;
    D_80120656 = 1;
    D_80120693 = 0x11;
    D_80120676 = -0xA0;
    D_8012067A = -0x78;
    D_80120657 = 0x80;
    D_8015B6A0 = 0;
    func_8004284C();
}

void func_8014DF1C(void) {
    func_80046318(0x45, 0x801B0000, 0x7E10);
    func_8014CCF0();
    func_8004284C();
}

void func_8014DF54(void) {
    func_80085368();
    D_80120650[0x8B] |= 0x80;
    D_80120650[0xCF] |= 0x80;
    D_80120650[0x8F] = D_80120650[7];
    D_80120650[0xD3] = D_80120650[7];
}

void func_8014DFB0(void) {
    func_800853FC();
    if (D_80120650[7] == 0) {
        D_80120650[0x8B] &= 0xFF7F;
        D_80120650[0xCF] &= 0xFF7F;
    }
    D_80120650[0x8F] = D_80120650[7];
    D_80120650[0xD3] = D_80120650[7];
}

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

typedef struct {
    void (*f[44])();
} FnTbl44; /* size 0xB0 */
extern FnTbl44 D_8015E460;

void func_8014E2E8(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl44 tbl;

    tbl = D_8015E460;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    if (D_800E6280.unk_110A == 0xD) {
        func_8014E3E8();
    }
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014E38C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014E3E8);

void func_8014EBB8(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl43 tbl;

    tbl = D_8015E530;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8014EC34(void) {
    func_8014C5C8();
    if (((u16) D_800CA154 == 5) && (D_800E6280.unk_1104.w == 1)) {
        func_80083440(1);
    }
    D_800E6280.unk_1104.w += 1;
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

typedef struct {
    void (*f[44])();
} FnTbl44; /* size 0xB0 */
extern FnTbl44 D_8015E5DC;

void func_8014ECF0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl44 tbl;

    tbl = D_8015E5DC;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    if (D_80122D04 != 0 && D_800E6280.unk_10F8 % 180 == 0) {
        func_80044750(0x501);
    }
    if (D_80122D04 != 0 && D_800E6280.unk_10F8 % 210 == 0xA) {
        func_80044750(0x502);
    }
    if (D_80122D04 != 0 && D_800E6280.unk_10F8 % 450 == 0x14) {
        func_80044750(0x500);
    }
}

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
        D_800E6280.unk_110A -= 1;
    }
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014CC40", func_8014EF1C);
