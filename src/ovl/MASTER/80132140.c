#define MAIN_API_OVERRIDE_func_800847B8 /* T-25130: called without a prototype, as an implicit int; func_801326B4 passes the full word D_8013C2E0 (main_api.h narrows it to u8, lbu), and a void view moves its v0/v1 ranges (39 rows). */
#include "ovl/MASTER.h"

s32 func_800847B8();

void func_80132140(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl4 tbl;

    tbl = D_8013C2E4;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

void func_801321B0(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80044750(0xBF);
        func_80044890(1, 0xC043, 0xC01D, 0xC982, 0xC935, 0xC91B);
        func_80085E30(1, 0);
        func_800573AC();
        func_80057390(0);
        func_8008585C();
        func_80064E84();
        func_800649D4();
        func_80063930(0);
        func_80064FA4(0);
        func_8006764C(0);
        func_8004284C();
        break;
    case 1:
        if (func_80044E8C() == 1) {
            func_8004E9F4(0);
            switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
            case 0:
                func_8006612C((s32) "植木賞授賞式");
                func_8005742C(0x4644, 1);
                break;
            case 1:
                func_8006612C((s32) "演劇グランプリ");
                func_8005742C(0x464D, 1);
                break;
            case 2:
                func_8006612C((s32) "ノーベル賞受賞");
                func_8005742C(0x4656, 1);
                break;
            case 3:
                func_8006612C((s32) "ゲーム大賞");
                func_8005742C(0x465F, 1);
                break;
            case 4:
                func_8006612C((s32) "目展大賞");
                func_8005742C(0x4669, 1);
                break;
            case 5:
                func_8006612C((s32) "コンクール優勝");
                func_8005742C(0x4673, 1);
                break;
            default:
                func_80046500();
                break;
            }
            func_80065F34(0);
            func_80044750(0x202);
            func_8004284C();
        }
        break;
    case 2:
        func_8004284C();
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_800847B8(0U);
            func_80084E4C();
            func_80065F34(1);
            func_8004284C();
        }
        break;
    case 4:
        func_8007E81C();
        break;
    case 5:
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}

void func_80132444(void) {
    u32 temp_t8;
    u8 temp_t6;

    temp_t6 = D_800E6280.unk_110A;
    switch (temp_t6) {
    case 0:
        temp_t8 = (u16) D_800E6280.unk_0F4.h >> 0xC;
        switch (temp_t8) {
        case 0:
            cd_read_sectors(3U, 0x801E8000, 0xA9DE);
            func_80132104();
            break;
        case 1:
            cd_read_sectors(3U, 0x801E8000, 0xA9B4);
            func_80132000();
            break;
        case 2:
            cd_read_sectors(3U, 0x801E8000, 0xA9BF);
            func_8013209C();
            break;
        case 3:
            cd_read_sectors(3U, 0x801E8000, 0xA9B7);
            func_80132034();
            break;
        case 4:
            cd_read_sectors(3U, 0x801E8000, 0xA9BC);
            func_80132068();
            break;
        case 5:
            cd_read_sectors(3U, 0x801E8000, 0xA9C2);
            func_801320D0();
            break;
        default:
            func_80046500();
            break;
        }
        D_800CA160 = (s32) D_8013C2D0;
        D_800CA164 = (s32) D_8013C2D4;
        D_800CA168 = (s32) D_8013C2D8;
        func_8004284C();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0;
            D_800CA14C = 0;
            func_8004284C();
        }
        break;
    case 2:
        normal_date_speak();
        break;
    case 3:
        normal_date_bg_fadeout();
        break;
    case 4:
        if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 3) {
            func_80044750(0xBF);
            /* byte view: the original does lbu/ori/sb on D_800E7D50 (main_api.h: s32), T-22015.
             * Two or more ORs make uopt promote the byte to $v1 (folded into one ori 3).
             * FAKE: T-24208, the third OR: its ugen temporaries move the copies to $t1..$t3 */
            *(u8 *)&D_800E7D50 |= 1;
            *(u8 *)&D_800E7D50 |= 2;
            *(u8 *)&D_800E7D50 |= 1;
            D_800E6280.unk_720 = D_800E6280.unk_1108;
            D_800E6280.unk_721 = D_800E6280.unk_1109 + 1;
            D_800E6280.unk_722 = 0;
            main_state_set(0x90);
        } else {
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}

/* FAKE: T-25130. An always-false test of the mode halfword (D_800E6374) right after each call in the
 * case 4 arms. It adds one use of the global in each arm, which makes the promoted global profitable,
 * so uopt keeps it in $a0 and reloads it there after the call, as the original does; ugen folds the
 * test away and no code is emitted. Removing any one of the six gives 229 register rows. */
#define KEEP_MODE_LIVE() if ((u32) D_800E6280.unk_0F4.h < 0) {}

void func_801326B4(void) {
    u32 mode;
    s32 i;
    s32 best;
    MasterWords13 sp1A8;
    MasterWords13 sp174;
    MasterWords13 sp140;
    MasterWords13 sp10C;
    MasterWords13 spD8;
    MasterWords13 spA4;
    MasterWords13 sp70;
    MasterWords13 sp3C;

    sp1A8 = D_8013C2F4;
    sp174 = D_8013C328;
    sp140 = D_8013C35C;
    sp10C = D_8013C390;
    spD8 = D_8013C3C4;
    spA4 = D_8013C3F8;
    sp70 = D_8013C42C;
    sp3C = D_8013C460;
    func_80083808();
    switch (D_800E6280.unk_110A) {
    case 0:
        if (D_800E6280.unk_110D == 0) {
            set_dec_bri(0);
            func_8006BC28(0);
            func_80084E4C();
            func_8006BD6C(0);
            hizuke_init();
            message_window_init();
            best = func_8008647C();
            if ((u8) D_800E6280.unk_1BC[0xC].unk_0C.b[3] >= 0x41U) {
                D_8013C2E0 = 0xC;
            } else {
                if (func_8008667C(best) < 2) {
                    D_8013C2E0 = best;
                    for (i = 0; i < 0xB; i++) {
                        if ((((u16) D_800E6280.unk_0F4.h >> 0xC) == ((u8) D_800E6280.unk_1BC[i].unk_0C.b[2] >> 4)) && (((u32) (D_800E6280.unk_1BC[i].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                            if (func_8008667C(i) < 2) {
                                D_8013C2E0 = i;
                            }
                        }
                    }
                } else if ((u8) D_800E6280.unk_1BC[0xB].unk_0C.b[3] >= 2U) {
                    D_8013C2E0 = 0xB;
                } else {
                    D_8013C2E0 = 0xFF;
                    func_80042940(0xE);
                }
            }
            if (D_8013C2E0 != 0xFF) {
                func_80044750(0xBF);
                func_80044890(1, 0xC043, 0xC01D, D_800B3688[D_8013C2E0], D_800B36C8[D_8013C2E0], D_800B3708[D_8013C2E0]);
            }
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else if (func_80044E8C() == 1) {
            k_reset(0);
            if ((((u16) D_800E6280.unk_0F4.h >> 0xC) == 1) && (D_8013C2E0 == 0) && (((u8) D_800E6280.unk_1BC[0].unk_0C.h[1] >> 4) == 1)) {
                func_8006612C((s32) "近所の公園");
                D_80125CA0 = 0xC0;
                dec_bg_cd_read(0x42D8, 1);
            } else if (((((u16) D_800E6280.unk_0F4.h >> 0xC) == 1) && (D_8013C2E0 == 1) && (((u8) D_800E6280.unk_1BC[1].unk_0C.h[1] >> 4) == 1)) || (((u16) D_800E6280.unk_0F4.h >> 0xC) == 5) || (((u16) D_800E6280.unk_0F4.h >> 0xC) == 0)) {
                func_8006612C((s32) "舞台裏");
                D_80125CA0 = 0xC0;
                dec_bg_cd_read(0x40F4, 1);
            } else {
                func_8006612C((s32) "自宅前");
                D_80125CA0 = 0xC0;
                dec_bg_cd_read(0x414C, 1);
            }
            func_8004284C();
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_800847B8(D_8013C2E0);
            if ((((u16) D_800E6280.unk_0F4.h >> 0xC) == 1) && (D_8013C2E0 == 0) && (((u8) D_800E6280.unk_1BC[0].unk_0C.h[1] >> 4) == 1)) {
                func_80062CD0(0x544A);
            } else if ((((u16) D_800E6280.unk_0F4.h >> 0xC) == 1) && (D_8013C2E0 == 1) && (((u8) D_800E6280.unk_1BC[1].unk_0C.h[1] >> 4) == 1)) {
                func_80062CD0(0x5D6E);
            } else if ((((u16) D_800E6280.unk_0F4.h >> 0xC) == 5) || (((u16) D_800E6280.unk_0F4.h >> 0xC) == 0)) {
                func_80062CD0(sp174.v[D_8013C2E0]);
            } else {
                func_80062CD0(sp1A8.v[D_8013C2E0]);
            }
            func_8004284C();
        }
        break;
    case 2:
        normal_date_bg_fadein();
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = (u8) D_8013C2E0;
            func_80083474();
            if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 3) {
                cd_read_sectors(3U, 0x801E8000, 0xA9B7);
            }
            if (D_8013C2E0 == 0xC) {
                func_80083440(0U);
            }
        }
        break;
    case 4:
        switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
        case 0:
            D_800CA148 = (s16) sp140.v[D_8013C2E0];
            break;
        case 1:
            if ((D_8013C2E0 == 0) && (((u8) D_800E6280.unk_1BC[0].unk_0C.h[1] >> 4) == 1)) {
                D_800CA148 = 7;
                func_8004284C();
                KEEP_MODE_LIVE();
            } else if ((D_8013C2E0 == 1) && (((u8) D_800E6280.unk_1BC[1].unk_0C.h[1] >> 4) == 1)) {
                D_800CA148 = 8;
                func_8004284C();
                KEEP_MODE_LIVE();
            } else {
                D_800CA148 = (s16) sp10C.v[D_8013C2E0];
            }
            break;
        case 2:
            D_800CA148 = (s16) spD8.v[D_8013C2E0];
            break;
        case 3:
            if (cd_read_sync_status() & 1) {
                func_80132034();
                D_800CA160 = (s32) D_8013C2D0;
                D_800CA164 = (s32) D_8013C2D4;
                D_800CA168 = (s32) D_8013C2D8;
                D_800CA148 = (s16) spA4.v[D_8013C2E0];
                if (D_8013C2E0 == 0xB) {
                    func_80047550();
                    func_80044750(0x500);
                }
                func_80044750(0x200);
                func_8004284C();
            }
            KEEP_MODE_LIVE();
            break;
        case 4:
            D_800CA148 = (s16) sp70.v[D_8013C2E0];
            break;
        case 5:
            if ((D_8013C2E0 == 0) && (((u8) D_800E6280.unk_1BC[0].unk_0C.h[1] >> 4) == 5)) {
                D_800CA148 = 5;
                func_8004284C();
                KEEP_MODE_LIVE();
            } else if ((D_8013C2E0 == 3) && (((u8) D_800E6280.unk_1BC[3].unk_0C.h[1] >> 4) == 5)) {
                D_800CA148 = 0xC;
                func_8004284C();
                KEEP_MODE_LIVE();
            } else {
                D_800CA148 = (s16) sp3C.v[D_8013C2E0];
            }
            break;
        default:
            func_80046500();
            KEEP_MODE_LIVE();
            break;
        }
        D_800CA14C = 0;
        /* FAKE: T-25130. The switch reads the mode as (u16) D_800E6374 >> 12 and this test through a u32 copy:
         * two different expressions to uopt, so the shift is not shared between them (both are ugen
         * temporaries); the original source of the join test is unknown. */
        mode = D_800E6280.unk_0F4.h;
        if ((mode >> 0xC) != 3) {
            if (D_8013C2E0 == 0xB) {
                func_80047550();
                func_80044750(0x500);
            }
            func_80044750(0x200);
            func_8004284C();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_in();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
        case 0:
        case 2:
            if (D_8013C2E0 == 0xA) {
                D_800CA148 = 2;
                func_8004284C();
            } else if (D_8013C2E0 == 0xB) {
                if (((u32) (D_800E6280.unk_1BC[0xB].unk_0C.w << 0x14) >> 0x1D) == 0) {
                    D_800CA148 = 3;
                } else {
                    D_800CA148 = 4;
                }
                func_80042940(0xC);
            } else if (D_8013C2E0 == 0xC) {
                D_800CA148 = 5;
                func_80042940(0xC);
            } else {
                D_800CA148 = 1;
                func_8004284C();
            }
            break;
        case 1:
        case 3:
        case 5:
            if (D_8013C2E0 == 0xB) {
                if (((u32) (D_800E6280.unk_1BC[0xB].unk_0C.w << 0x14) >> 0x1D) == 0) {
                    D_800CA148 = 2;
                } else {
                    D_800CA148 = 3;
                }
                func_80042940(0xC);
            } else if (D_8013C2E0 == 0xC) {
                D_800CA148 = 4;
                func_80042940(0xC);
            } else {
                D_800CA148 = 1;
                func_8004284C();
            }
            break;
        case 4:
            if (D_8013C2E0 == 0) {
                func_80042940(0xA);
            } else if (D_8013C2E0 == 0xB) {
                if (((u32) (D_800E6280.unk_1BC[0xB].unk_0C.w << 0x14) >> 0x1D) == 0) {
                    D_800CA148 = 2;
                } else {
                    D_800CA148 = 3;
                }
                func_80042940(0xC);
            } else if (D_8013C2E0 == 0xC) {
                D_800CA148 = 4;
                func_80042940(0xC);
            } else {
                D_800CA148 = 1;
                func_8004284C();
            }
            break;
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadeout();
        break;
    case 11:
        func_80042808();
        break;
    case 12:
        normal_date_girl_out();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if ((D_8013C2E0 == 0xFF) && (((u16) D_800E6280.unk_0F4.h >> 0xC) == 3)) {
            func_8004284C();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 15:
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}

void func_80133374(void) {
    s32 i;

    D_800E6280.unk_71E |= 0x10;
    D_800E6280.unk_1BC[0].unk_02 += 0x14;
    for (i = 1; i < 0xD; i++) {
        D_800E6280.unk_1BC[i].unk_02 += 0x14;
    }
    if (D_8013C2E0 != 0xFF) {
        D_800E6280.unk_1BC[D_8013C2E0].unk_06 += 2;
    }
    ((ScheduleClubFlags *)&D_800E6280.unk_66C[(u32)D_800E6280.unk_0F4.h >> 0xC])->b30 = 1;
    func_8008585C();
    func_800573AC();
    func_80057390(0);
    func_8013348C();
    func_80072B5C(1);
    func_80086424();
}

void func_8013348C(void) {
    u32 sel; /* declared before kind: its slot sits above kind's stack home (sp+0x2B), T-18600 */
    u8 kind;

    sel = (u16) D_800E6280.unk_0F4.h >> 0xC;
    switch (sel) {
    case 0:
        kind = 0;
        break;
    case 1:
        if ((((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) == 1) && (D_8013C2E0 == 0)) {
            kind = 0xB;
        } else if (((u8) D_800E6280.unk_1BC[1].unk_0C.b[2] >> 4) == 1) {
            kind = 1;
            if (D_8013C2E0 == 1) {
                kind = 0xC;
            }
        } else {
            kind = 1;
        }
        break;
    case 2:
        kind = 2;
        break;
    case 3:
        kind = 3;
        break;
    case 4:
        kind = 4;
        break;
    case 5:
        kind = 5;
        break;
    }
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_00 = D_8013C2E0;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_02 = 0x15;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = kind;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = get_g_zyotai_h(D_8013C2E0);
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_04 = D_800E6280.unk_03E;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_05 = D_800E6280.unk_03F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_06 = D_800E6280.unk_040;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = D_800E6280.unk_041;
    D_800E6280.unk_F5E++;
    if (D_800E6280.unk_F5E >= 0xFF) {
        D_800E6280.unk_F5E = 0xFE;
    }
}
