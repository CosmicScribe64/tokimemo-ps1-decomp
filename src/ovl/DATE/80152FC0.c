#include "common.h"
#include "ovl/DATE.h"

void func_80152FC0(void) {
    D_8015F160 = 0x801CE124;
    D_8015F164 = 0x801CE128;
    D_8015F168 = 0x801CE148;
    D_8015F16C = *(s16 *)0x801CE15C;
    D_8015F170 = 0x801B0000;
    D_8015F174 = 0x801B2000;
    D_8015F178 = 0x801B6000;
    D_8015F17C = 0x801BA000;
    D_8015F180 = 0x801BE000;
    D_8015F184 = 0x801C2000;
    D_8015F188 = 0x801C6000;
}

void func_80153070(void) {
    D_8015F18C = 0x801CE0F4;
    D_8015F190 = 0x801CE0F8;
    D_8015F194 = 0x801CE118;
    D_8015F198 = *(s16 *)0x801CE12C;
    D_8015F19C = 0x801B0000;
    D_8015F1A0 = 0x801B2000;
    D_8015F1A4 = 0x801B6000;
    D_8015F1A8 = 0x801BA000;
    D_8015F1AC = 0x801BE000;
    D_8015F1B0 = 0x801C2000;
    D_8015F1B4 = 0x801C6000;
}

void func_80153120(void) {
    D_8015F1B8 = (u8 *)0x801CE088;
    D_8015F1BC = (u8 *)0x801CE08C;
    D_8015F1C0 = (u8 *)0x801CE0A0;
    D_8015F1C4 = *(s16 *)0x801CE0B4;
    D_8015F1C8 = (u8 *)0x801B0000;
    D_8015F1CC = (u8 *)0x801B2000;
    D_8015F1D0 = (u8 *)0x801B6000;
    D_8015F1D4 = (u8 *)0x801BA000;
    D_8015F1D8 = (u8 *)0x801BE000;
    D_8015F1DC = (u8 *)0x801C2000;
    D_8015F1E0 = (u8 *)0x801C6000;
}

void func_801531D0(void) {
    D_8015F1E4 = 0x801CE0F4;
    D_8015F1E8 = 0x801CE0F8;
    D_8015F1EC = 0x801CE118;
    D_8015F1F0 = *(s16 *)0x801CE12C;
    D_8015F1F4 = 0x801B0000;
    D_8015F1F8 = 0x801B2000;
    D_8015F1FC = 0x801B6000;
    D_8015F200 = 0x801BA000;
    D_8015F204 = 0x801BE000;
    D_8015F208 = 0x801C2000;
    D_8015F20C = 0x801C6000;
}

void func_80153280(void) {
    switch (D_80122CD0) {
    case 2:
        func_80153B38();
        break;
    case 3:
        func_80153CEC();
        break;
    case 4:
        func_80153E1C();
        break;
    case 5:
        func_80154004();
        break;
    case 6:
        func_80154118();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80153320(void) {
    D_8015E208 = D_8015DECC;
    D_8015E20C = D_8015E008;
    D_8015E210 = D_8015E144;
    D_800CA21C[0] = 1;
    D_800CA21C[2] = 2;
    D_800CA21C[1] = 4;
    D_800CA224[0] = 1;
    D_800CA224[2] = 2;
    D_800CA224[1] = 3;
    D_800CA22C[0] = 0;
    D_800CA22C[2] = 2;
    D_800CA22C[1] = 4;
    D_800CA234[0] = 0;
    D_800CA234[2] = 1;
    D_800CA234[1] = 2;
    func_80152FC0();
    load_palette(D_8015F170, 0x11, 1, 2, 0);
    func_80084E90(D_8015F174, D_8015F178, D_8015F17C, D_8015F180, D_8015F184, D_8015F188);
    func_800850D4(D_8015F164, D_8015F168, D_8015F160, D_8015F16C);
    func_8004284C();
}

void func_8015347C(void) {
    D_8015E208 = D_8015DED0;
    D_8015E20C = D_8015E00C;
    D_8015E210 = D_8015E148;
    D_800E6280.unk_1BC[7].unk_02 += 1;
    D_800E6280.unk_1BC[7].unk_06 += 1;
    D_800E6280.unk_1BC[7].unk_0A -= 0x14;
    func_80084D3C();
    func_80153070();
    func_80043914(D_8015F19C, 0x11, 1, 2, 0);
    func_80084E90(D_8015F1A0, D_8015F1A4, D_8015F1A8, D_8015F1AC, D_8015F1B0, D_8015F1B4);
    func_800850D4(D_8015F190, D_8015F194, D_8015F18C, D_8015F198);
    D_800CA224[0] = 3;
    D_800CA224[1] = 3;
    D_800CA224[2] = 3;
    D_800CA234[0] = 2;
    D_800CA234[1] = 2;
    D_800CA234[2] = 2;
    func_8004284C();
}

void func_801535D8(void) {
    D_8015B68C = 1;
    D_8015E208 = D_8015DED4;
    D_8015E20C = D_8015E010;
    D_8015E210 = D_8015E14C;
    func_800AE0F0(D_800CA19C, "遊園地");
    func_800AE0F0(D_800CA19C, "ゴーストハウス");
    D_800CA21C[0] = 2;
    D_800CA21C[1] = 2;
    D_800CA21C[2] = 2;
    D_800CA224[0] = 2;
    D_800CA224[1] = 2;
    D_800CA224[2] = 2;
    D_800CA22C[0] = 2;
    D_800CA22C[1] = 2;
    D_800CA22C[2] = 2;
    D_800CA234[0] = 1;
    D_800CA234[1] = 1;
    D_800CA234[2] = 1;
    func_80153120();
    func_80043914((s32) D_8015F1C8, 0x11, 1, 2, 0);
    func_80084E90((s32) D_8015F1CC, (s32) D_8015F1D0, (s32) D_8015F1D4, (s32) D_8015F1D8, (s32) D_8015F1DC, (s32) D_8015F1E0);
    func_800850D4((s32) D_8015F1BC, (s32) D_8015F1C0, (s32) D_8015F1B8, (s32) D_8015F1C4);
    D_800CA360 = 1;
    D_800CA224[0] = 3;
    D_800CA224[1] = 3;
    D_800CA224[2] = 3;
    D_800CA234[0] = 2;
    D_800CA234[1] = 2;
    D_800CA234[2] = 2;
    func_8004284C();
}

void func_801537C0(void) {
    D_8015E208 = D_8015DED8;
    D_8015E20C = D_8015E014;
    D_8015E210 = D_8015E150;
    D_800E6280.unk_1BC[7].unk_02 += 3;
    D_800E6280.unk_1BC[7].unk_06 += 2;
    D_800E6280.unk_1BC[7].unk_0A -= 0x14;
    func_80084D3C();
    func_801531D0();
    func_80043914(D_8015F1F4, 0x11, 1, 2, 0);
    func_80084E90(D_8015F1F8, D_8015F1FC, D_8015F200, D_8015F204, D_8015F208, D_8015F20C);
    func_800850D4(D_8015F1E8, D_8015F1EC, D_8015F1E4, (s32) D_8015F1F0);
    D_800CA360 = 1;
    D_800CA224[0] = 3;
    D_800CA224[1] = 3;
    D_800CA224[2] = 3;
    D_800CA234[0] = 2;
    D_800CA234[1] = 2;
    D_800CA234[2] = 2;
    func_8004284C();
}

void func_80153928(void) {
    D_8015E208 = D_8015DEDC;
    D_8015E20C = D_8015E018;
    D_8015E210 = D_8015E154;
    D_800E6280.unk_1BC[7].unk_02 += 3;
    D_800E6280.unk_1BC[7].unk_06 += 2;
    func_80084D3C();
    func_8004284C();
}

void func_801539A4(void) {
    bg_read_sub2(0x486E);
    func_8004284C();
}

void func_801539CC(void) {
    func_80044750(0x203);
    bg_read_sub2(0x4520);
    func_8004284C();
}

void func_801539FC(void) {
    bg_read_sub2(0x48D6);
    func_8004284C();
}

void func_80153A24(void) {
    bg_read_sub2(0x45A3);
    func_8004284C();
}

void func_80153A4C(void) {
    bg_read_sub2(0x482A);
    func_8004284C();
}

void func_80153A74(void) {
    bg_read_sub2(0x432B);
    func_8004284C();
}

void func_80153A9C(void) {
    bg_read_sub2(0x4347);
    D_800E6280.unk_F5E -= 1;
    func_80085B3C(0, D_80122D08);
    func_8004284C();
}

void func_80153AE8(void) {
    bg_read_sub2(0x4891);
    func_8004284C();
}

void func_80153B10(void) {
    bg_read_sub2(0x4558);
    func_8004284C();
}

void func_80153B38(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl41 tbl;

    tbl = D_8015F210;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80153BC0(void) {
    func_80044750(0x500);
    func_8004284C();
}

void func_80153BE8(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x847D);
    func_80152FC0();
    func_8004284C();
}

void func_80153C20(void) {
    D_800CA150 = (u16) D_800CA150 + D_80122CDC;
    if (D_80122CDC == 0) {
        load_palette(D_8015F170, 0x11, 1, 2, 0);
        D_8015E204 = 0;
    } else if (D_80122CDC == 1) {
        load_palette(D_8015F170, 0x11, 1, 2, 2);
        D_8015E204 = 2;
    } else {
        load_palette(D_8015F170, 0x11, 1, 2, 1);
        D_8015E204 = 1;
    }
    func_8004284C();
}

void func_80153CEC(void) {
    u8 index;
    DateCallbacks39 table;

    table = D_8015F2B4;
    index = D_800E6280.unk_110A;
    if ((index >= 27 || index < 8) && D_800B593C == 0x80) {
        func_8006B900();
    }
    index = D_800E6280.unk_110A;
    table.callback[index](0x80);
}

void func_80153D94(void) {
    u32 v;

    v = func_80051B48(7);
    D_800CA150 = (v >= 1) + (v >= 2) + (v >= 3) + 1;
    func_8014C5C8();
}

void func_80153DE4(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x84BA);
    func_80153070();
    func_8004284C();
}

void func_80153E1C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl57 tbl;

    tbl = D_8015F350;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80153E90(void) {
    if (D_80122CDC != 0) {
        D_800CA150 = 6;
        func_80042940(0x24);
        return;
    }
    D_800E6280.unk_56C[0x31] = 1;
    D_800E6280.unk_1BC[7].unk_06 -= 1;
    D_800E6280.unk_1BC[7].unk_0A += 0xA;
    func_80084D3C();
    func_8004284C();
}

void func_80153F10(void) {
    func_8014C5C8();
    if (D_800E6280.unk_1104.w == 1) {
        if ((u16) D_800CA154 == 7) {
            func_80083440(1);
        }
    }
}

void func_80153F5C(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_80153F84(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80044750(0x205);
        D_800E6280.unk_110D += 1;
    }
    func_8014C5C8();
}

void func_80153FCC(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x84F7);
    func_80153120();
    func_8004284C();
}

void func_80154004(void) {
    s32 idx;
    DateDispatch80154004 tbl;
    tbl = D_8015F434;
    if (D_800E6280.unk_110A < 9) {
        func_8006B900();
    }
    if ((D_800E6280.unk_110A >= 0x16) && (D_800E6280.unk_110A < 0x1C)) {
        func_8006B900();
    }
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_801540B8(void) {
    func_80044750(0x502);
    func_8004284C();
}

void func_801540E0(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8534);
    func_801531D0();
    func_8004284C();
}

/* FAKE: D_800CA2F8 is read through its DateSelectionView; a direct read of the u8 keeps cvt_pass's
 * widening CVT, whose CSE temporary (two compares) costs a 4-byte frame slot the original does not
 * have (0x70 against 0x68). Same words otherwise. T-17203 */
void func_80154118(void) {
    GameState *g = &D_800E6280; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl15 tbl;

    tbl = D_8015F4DC;
    if (g->unk_110A < 0xC) {
        if (((g->unk_03E == 0x5F && g->unk_03F == 7 && g->unk_040 == 0x17) ||
             (g->unk_03E == 0x60 && g->unk_03F == 7 && g->unk_040 == 0x1C) ||
             (g->unk_03E == 0x61 && g->unk_03F == 7 && g->unk_040 == 0x1B)) &&
            D_800B593C == 0x80) {
            func_8006B900();
        } else if (((DateSelectionView *)&D_800CA2F8)->place == 0x11 ||
                   ((DateSelectionView *)&D_800CA2F8)->place == 0x10) {
            func_8006B900();
        }
    }
    tbl.f[g->unk_110A](0x80);
}
