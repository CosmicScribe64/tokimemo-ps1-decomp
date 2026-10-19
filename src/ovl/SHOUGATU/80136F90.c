#include "common.h"
#include "ovl/SHOUGATU.h"

void func_80136F90(void) {
    D_80144D90 = 0x801976F8;
    D_80144D94 = 0x80198BF0;
    D_80144D98 = 0x8019A37C;
    D_80144D9C = 0x8019ADCC;
    D_80144DA0 = 0x8019BC80;
    D_80144DA4 = 0x8019C918;
    D_80144DA8 = 0x8019D460;
    D_80144DAC = 0x8019E168;
    D_80144DB0 = 0x8019EB4C;
    D_80144DB4 = 0x80197774;
    D_80144DB8 = 0x80198CF8;
    D_80144DBC = 0x8019A414;
    D_80144DC0 = 0x8019ADF8;
    D_80144DC4 = 0x8019BD34;
    D_80144DC8 = 0x8019C960;
    D_80144DCC = 0x8019D4C4;
    D_80144DD0 = 0x8019E210;
    D_80144DD4 = 0x8019EBA8;
    D_80144DD8 = 0x80197C94;
    D_80144DDC = 0x80199738;
    D_80144DE0 = 0x8019AB30;
    D_80144DE4 = 0x8019B000;
    D_80144DE8 = 0x8019C4B4;
    D_80144DEC = 0x8019CC70;
    D_80144DF0 = 0x8019D910;
    D_80144DF4 = 0x8019E7B4;
    D_80144DF8 = 0x8019EE04;
}

void func_80137144(void) {
    func_80083808();
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80137664();
        break;
    case 1:
        func_80137818();
        break;
    case 2:
        func_80137A04();
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_80042808();
        } else {
            if (D_800E6280.unk_1100++ >= 0x401U) {
                func_80042908(2);
            }
        }
        break;
    case 4:
        switch (D_80144E10) {
        case 0:
            func_80046500();
            break;
        case 1:
            func_801398F0();
            break;
        case 2:
            func_80137FF0();
            break;
        case 3:
            func_80138B40();
            break;
        case 4:
            func_8013A950();
            break;
        case 5:
            func_8013A190();
            break;
        case 6:
            func_8013A500();
            break;
        case 7:
            func_801391D4();
            break;
        case 8:
            func_80137BB0();
            break;
        case 9:
            func_80046500();
            break;
        case 10:
            func_8013AF10();
            break;
        default:
            func_80046500();
            break;
        }
        break;
    case 5:
        func_801377CC();
        break;
    case 6:
        func_80137818();
        break;
    case 7:
        func_8013737C();
        break;
    default:
        func_80046500();
        break;
    }
    check_k_scroll();
    k_disp_inc2();
    func_80066C08(2);
    message_window_show();
    func_80066334();
    hizuke_show();
    func_80083A10();
    func_80047560();
}

s32 func_8013737C(void) {
    GameState *g = &D_800E6280;
    s32 pad;

    /* FAKE: touch narrow unsigned globals at entry for cvt_pass.py (T-20610); dead-store eliminated by uopt. T-24130 */
    pad = D_80144E10;
    pad = D_80144E14;
    pad = D_800E6280.unk_03F;
    if (D_800E6280.unk_110D == 0) {
        func_80044750(0xCF);
        func_800674B0();
        D_800E6280.unk_110D += 1;
    }
    if (D_800E6280.unk_110D == 1) {
        if (D_800E6280.unk_1104.u++ >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E6280.unk_110D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            return 0;
        }
    }
    D_800E6280.unk_F5F = D_80144E10;
    if (D_80144E10 == 2 && (D_80144E14 == 2 || D_80144E14 == 3) &&
        (D_800E6280.unk_1BC[2].unk_0C.b[2] >> 4) == 3) {
        func_80085B3C(8, D_80144E10 * 10 + 6);
    } else if (D_80144E10 == 4 && D_80144E14 == 0 &&
               D_800E6280.unk_03F >= 6 && D_800E6280.unk_03F < 10) {
        func_80085B3C(8, D_80144E10 * 10 + 3);
    } else {
        func_80085B3C(8, D_80144E10 * 10 + D_80144E14);
    }
    if (D_80144E10 == 10) {
        D_800E6280.unk_1BC[D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01].unk_06 = 0x28;
    } else {
        D_800E6280.unk_1BC[D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01].unk_06 = 0x32;
    }
    if (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 != 8) {
        D_800E6280.unk_1BC[D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01].unk_0C.f.b1 = 1;
    } else {
        g->unk_1BC[8].unk_0C.f.f9 += 1;
        if (g->unk_1BC[8].unk_0C.f.f9 == 4) {
            D_800E6280.unk_1BC[D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01].unk_0C.f.b1 = 1;
        }
    }
    func_80044750(0x22);
    func_80044750(0x21);
    func_80044750(0x2F);
    func_80072B5C(1);
}

void func_80137664(void) {
    GameState *g = &D_800E6280;
    s32 a1;
    s32 a2;

    D_800B3D60 = 0;
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    hizuke_init();
    message_window_init();
    func_8008585C();
    /* FAKE: g-view store keeps ILOD CSE of unk_69C[unk_71C].unk_01. T-24137 */
    g->unk_F5F = g->unk_69C[g->unk_71C].unk_01;
    D_80144E14 = D_80122CD0;
    D_80144E10 = g->unk_F5F;
    if (g->unk_69C[g->unk_71C].unk_01 == 2 ||
        g->unk_69C[g->unk_71C].unk_01 == 6) {
        a1 = 0xC063;
        a2 = 0xC045;
    } else {
        a1 = 0xC043;
        a2 = 0xC01D;
    }
    func_80044890(1, a1, a2,
                  D_800B3688[g->unk_69C[g->unk_71C].unk_01],
                  D_800B36C8[g->unk_69C[g->unk_71C].unk_01],
                  D_800B3708[g->unk_69C[g->unk_71C].unk_01]);
    if (func_80044E8C() == 1) {
        func_80042808();
    }
    g->unk_1BC[g->unk_69C[g->unk_71C].unk_01].unk_06 = 0x46;
    func_80042808();
}

void func_801377CC(void) {
    func_80044750(0xCF);
    func_800674B0();
    if (func_80044E8C() == 1) {
        func_80042808();
    }
    func_80042808();
}

void func_80137818(void) {
    if (func_80044E8C() == 1) {
        func_80042808();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042908((D_800E6280.unk_1109 - 1) & 0xFF);
    }
}

void func_80137898(void) {
    func_80044750(0xCF);
    func_8004284C();
}

void func_801378C0(void) {
    D_800B3D60 = 0;
    func_80044890(0, 0xBF98, 0xBF79, 0xD989, 0xD949, 0xD941);
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x2E);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80137940(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0x23);
        func_80044750(0x24);
        func_80044750(0x2E);
        func_8004284C();
    }
    if (D_800E6280.unk_1104.u++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_801379D8(void) {
    func_8004500C(1, 0x200);
    func_8004284C();
}

void func_80137A04(void) {
    cd_read_sectors(0x10, 0x80197000, 0xAEFD);
    func_80136F90();
    D_80144E10 = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01;
    func_80042808();
}

void func_80137A5C(void) {
    cd_read_sectors(0x10, 0x80197000, 0xAEFD);
    func_80136F90();
    D_80144E10 = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01;
    func_8004284C();
}

void func_80137AB4(void) {
    D_800CA134 = &D_80144E08;
    D_800CA138 = &D_80144E0C;
    D_800CA13C = D_80144DFC;
    D_800CA140 = D_80144E00;
    D_800CA144 = D_80144E04;
    func_80082764(D_80122CDC, 1, 0);
}

void func_80137B30(void) {
    D_800CA134 = (u8 *) &D_80144E08;
    D_800CA138 = (u8 *) &D_80144E0C;
    D_800CA13C = D_80144DFC;
    D_800CA140 = D_80144E00;
    D_800CA144 = D_80144E04;
    func_80082764(D_80122CDC, 1, 1);
}
