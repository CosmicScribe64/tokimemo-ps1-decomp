#include "ovl/OMIMAI.h"

typedef struct {
    void (*f[26])();
} FnTbl26; /* size 0x68 */
extern FnTbl26 D_80134CD0;

void func_801331B0(void) {
    *D_80134C30 = 0x8019709C;
    D_80134C34 = 0x801977CC;
    D_80134C38 = 0x8019816C;
    D_80134C3C = 0x801984E0;
    D_80134C40 = 0x801987B8;
    D_80134C44 = 0x801989B4;
    D_80134C48 = 0x80198B84;
    D_80134C4C = 0x80198E70;
    D_80134C50 = 0x80199168;
    D_80134C54 = 0x80199420;
    D_80134C58 = 0x801996C4;
    D_80134C5C = 0x80199AE0;
    D_80134C60 = 0x80199DD8;
    *D_80134C64 = 0x801970AC;
    D_80134C68 = 0x801978AC;
    D_80134C6C = 0x8019819C;
    D_80134C70 = 0x80198520;
    D_80134C74 = 0x801987E8;
    D_80134C78 = 0x801989BC;
    D_80134C7C = 0x80198BB4;
    D_80134C80 = 0x80198E90;
    D_80134C84 = 0x801991A8;
    D_80134C88 = 0x80199438;
    D_80134C8C = 0x801996FC;
    D_80134C90 = 0x80199B38;
    D_80134C94 = 0x80199DF8;
    *D_80134C98 = 0x80197140;
    D_80134C9C = 0x80197F3C;
    D_80134CA0 = 0x80198304;
    D_80134CA4 = 0x80198620;
    D_80134CA8 = 0x80198950;
    D_80134CAC = 0x801989F8;
    D_80134CB0 = 0x80198D1C;
    D_80134CB4 = 0x80198F80;
    D_80134CB8 = 0x80199388;
    D_80134CBC = 0x801994B4;
    D_80134CC0 = 0x801998F0;
    D_80134CC4 = 0x80199CD0;
    D_80134CC8 = 0x80199EE8;
}

void func_80133424(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl26 tbl;

    tbl = D_80134CD0;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_801334AC(void) {
    if ((D_800E6280.unk_F5F == 4) && (D_800CA148 == 5)) {
        normal_date_speak_1line();
        return;
    }
    func_8004284C();
}

s32 func_80133500(void) {
    if (D_800E6280.unk_110D == 0) {
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
    return func_80072B5C(1);
}

void func_801335C4(void) {
    func_80044890(1, 0xBF98, 0xBF79, D_800B3688[D_800E6280.unk_F5F], D_800B36C8[D_800E6280.unk_F5F], D_800B3708[D_800E6280.unk_F5F]);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_8013364C(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_801336CC(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_801336F4(void) {
    cd_read_sectors(6, 0x80197000, 0xAF33);
    func_801331B0();
    func_8004284C();
}

void func_80133730(void) {
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    k_reset(1);
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    func_8008585C();
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    hizuke_init();
    message_window_init();
    addr_init_bustup();
    func_80084E4C();
    func_801337EC();
    func_8004284C();
}

void func_801337EC(void) {
    func_800847B8(D_800E6280.unk_F5F = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01);
    func_801331B0();
    if (D_800E6280.unk_F5F == 0xC) {
        D_800CA148 = 8;
    } else {
        D_800CA148 = 0;
    }
    D_800CA14C = 0;
    D_800CA160 = D_80134C5C;
    D_800CA164 = D_80134C90;
    D_800CA168 = D_80134CC4;
}

/* FAKE: uopt priority and cvt_pass NO_WIDEN helpers so unk_F68.w and unk_0F4.h
 * promote to $v1 per case, unk_03E promotes to $a0 in case 4, and D_800CA148
 * colours into $a0; T-24140 */
#define F68_SEL_388C ((((D_800E6280.unk_F68.w ^ 1) * 0) + (D_800E6280.unk_F68.w & 0xF)))
#define F0F4_SHR_388C (D_800E6280.unk_0F4.h >> (((pad = 0) * 0) + 0xC))
#define F0F4_DIV_388C (D_800E6280.unk_0F4.h / (((pad = 0) * 0) + 0x1000U))
#define F0F4_BOOST_388C ((D_800E6280.unk_0F4.h - (((pad = 0) * 0) + 1)) * 0)

void func_8013388C(void) {
    s32 pad; /* FAKE: 4-byte frame pad and dead-store target for SWP NO_WIDEN, T-24140 */
    u8 sp2B;

    pad = (D_800E6280.unk_03E == 0);
    sp2B = get_g_zyotai_s(D_800E6280.unk_F5F) & 0x7F;
    func_800AE120(D_800E6280.unk_10F8);
    switch (D_800E6280.unk_F5F) {
    case 0:
        if (F68_SEL_388C < 2U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
        } else if (F68_SEL_388C == 2) {
            D_80134CCC = 0;
            D_800CA148 = 2;
        } else if (F68_SEL_388C == 3) {
            D_80134CCC = 0;
            D_800CA148 = 4;
        } else if (F68_SEL_388C == 4) {
            D_80134CCC = 0;
            D_800CA148 = (F0F4_SHR_388C * 2) + 6 + F0F4_BOOST_388C;
            if ((F0F4_DIV_388C + (F68_SEL_388C * 0)) == 0xA) {
                D_800CA148 = D_800CA148 - 4;
            }
        } else {
            D_80134CCC = 3;
            D_800CA148 = 0x1A;
        }
        if (func_800AE0D0() % 2) {
            D_800CA148 = D_800CA148 + 1;
            if (D_800CA148 == 5) {
                D_80134CCC = 2;
            }
        }
        break;
    case 1:
        if (F68_SEL_388C < 2U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
        } else {
            D_80134CCC = 0;
            D_800CA148 = (F0F4_SHR_388C * 2) + 2 + F0F4_BOOST_388C;
        }
        if (func_800AE0D0() % 2) {
            D_800CA148 += 1;
        }
        break;
    case 2:
        if (F68_SEL_388C < 2U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
        } else {
            D_80134CCC = 4;
            D_800CA148 = (F0F4_SHR_388C * 2) - 2 + F0F4_BOOST_388C;
        }
        if (func_800AE0D0() % 2) {
            D_800CA148 += 1;
        }
        break;
    case 3:
        if (F68_SEL_388C < 3U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
        } else {
            D_80134CCC = 0;
            D_800CA148 = (F0F4_SHR_388C * 2) - 6 + F0F4_BOOST_388C;
        }
        if (func_800AE0D0() % 2) {
            D_800CA148 += 1;
        }
        break;
    case 4:
        if (F68_SEL_388C < 4U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
            if (func_800AE0D0() % 2) {
                D_800CA148 += 1;
            }
        } else if ((func_800AE0D0() % 2) || ((F0F4_SHR_388C == 6) && ((D_800E6280.unk_03E == 0x62) || ((D_800E6280.unk_03E == 0x61) && (D_800E6280.unk_03F >= 9U)))) || (((((D_800E6280.unk_03E == 0) * 0) + F0F4_SHR_388C) == 7) && (D_800E6280.unk_03E == 0x62))) {
            if (sp2B < 2U) {
                D_80134CCC = 6;
                D_800CA148 = 4;
            } else {
                D_80134CCC = 5;
                D_800CA148 = 5;
            }
        } else {
            D_80134CCC = 0;
            D_800CA148 = F0F4_DIV_388C - 4 + F0F4_BOOST_388C;
        }
        break;
    case 5:
        if (F68_SEL_388C < 4U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
        } else {
            D_80134CCC = 0;
            D_800CA148 = 2;
        }
        if (func_800AE0D0() % 2) {
            D_800CA148 += 1;
        }
        break;
    case 6:
        if (F68_SEL_388C < 4U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
            if (func_800AE0D0() % 2) {
                D_800CA148 += 1;
                D_80134CCC = 2;
            }
        } else {
            D_80134CCC = 0;
            D_800CA148 = 4;
            if (func_800AE0D0() % 2) {
                D_800CA148 += 1;
            }
        }
        if (sp2B < 2U) {
            D_800CA148 += 2;
        }
        break;
    case 7:
        D_80134CCC = 3;
        D_800CA148 = 0;
        break;
    case 8:
        D_80134CCC = 7;
        D_800CA148 = 0;
        if (func_800AE0D0() % 2) {
            D_800CA148 += 1;
        }
        break;
    case 9:
        D_80134CCC = (u8) (func_800AE0D0() % 2);
        D_800CA148 = 0;
        if (func_800AE0D0() % 2) {
            D_800CA148 += 1;
        }
        break;
    case 10:
        if (F68_SEL_388C < 4U) {
            D_800CA148 = 0;
            if (func_800AE0D0() % 2) {
                D_800CA148 += 1;
            }
            if (D_800CA148 == 1) {
                D_80134CCC = 2;
            } else {
                D_80134CCC = 0;
            }
        } else {
            D_80134CCC = 0;
            D_800CA148 = 2;
            if (func_800AE0D0() % 2) {
                D_800CA148 += 1;
            }
        }
        break;
    default:
        D_800CA148 = (s16) (func_800AE0D0() % 8);
        D_80134CCC = (func_800AE0D0() % 2) + 8;
        if (F68_SEL_388C == 3) {
            D_80134CCC = 9;
        }
        break;
    }
    D_800CA14C = 0;
    func_801340E4();
    func_8004284C();
}

#undef F68_SEL_388C
#undef F0F4_SHR_388C
#undef F0F4_DIV_388C
#undef F0F4_BOOST_388C

s32 func_80134030(void) {
    if (D_800E6280.unk_F5F == 9) {
        if (D_800CA148 == 2) {
            func_8004284C();
            return 0;
        }
    }
    if (D_800E6280.unk_F5F == 8) {
        func_8004284C();
        func_8004284C();
        return 0;
    }
    D_800CA160 = D_80134C5C;
    D_800CA164 = D_80134C90;
    D_800CA168 = D_80134CC4;
    D_800CA148 = D_80134CCC + 1;
    func_8004284C();
}

void func_801340E4(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        D_800CA160 = D_80134C30[1];
        D_800CA164 = D_80134C64[1];
        D_800CA168 = D_80134C98[1];
        return;
    case 1:
        D_800CA160 = D_80134C30[6];
        D_800CA164 = D_80134C64[6];
        D_800CA168 = D_80134C98[6];
        return;
    case 2:
        D_800CA160 = D_80134C30[2];
        D_800CA164 = D_80134C64[2];
        D_800CA168 = D_80134C98[2];
        return;
    case 3:
        D_800CA160 = D_80134C30[4];
        D_800CA164 = D_80134C64[4];
        D_800CA168 = D_80134C98[4];
        return;
    case 4:
        D_800CA160 = D_80134C30[10];
        D_800CA164 = D_80134C64[10];
        D_800CA168 = D_80134C98[10];
        return;
    case 5:
        D_800CA160 = D_80134C30[7];
        D_800CA164 = D_80134C64[7];
        D_800CA168 = D_80134C98[7];
        return;
    case 6:
        D_800CA160 = D_80134C30[8];
        D_800CA164 = D_80134C64[8];
        D_800CA168 = D_80134C98[8];
        return;
    case 7:
        D_800CA160 = D_80134C30[5];
        D_800CA164 = D_80134C64[5];
        D_800CA168 = D_80134C98[5];
        return;
    case 8:
        D_800CA160 = D_80134C30[0];
        D_800CA164 = D_80134C64[0];
        D_800CA168 = D_80134C98[0];
        return;
    case 9:
        D_800CA160 = D_80134C30[9];
        D_800CA164 = D_80134C64[9];
        D_800CA168 = D_80134C98[9];
        return;
    case 10:
        D_800CA160 = D_80134C30[12];
        D_800CA164 = D_80134C64[12];
        D_800CA168 = D_80134C98[12];
        return;
    default:
        D_800CA160 = D_80134C30[3];
        D_800CA164 = D_80134C64[3];
        D_800CA168 = D_80134C98[3];
        return;
    }
}

void func_80134384(void) {
    switch (D_800E6280.unk_F68.w & 0xF) {
    case 0:
        func_800AE0F0(D_800CA19C, "図書室");
        bg_read_sub2(0x40BC);
        break;
    case 1:
        func_800AE0F0(D_800CA19C, "教室");
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            bg_read_sub2(0x404D);
        } else {
            bg_read_sub2(0x4055);
        }
        break;
    case 2:
        func_800AE0F0(D_800CA19C, "中庭");
        bg_read_sub2(0x4071);
        break;
    case 3:
        func_800AE0F0(D_800CA19C, "自宅前");
        bg_read_sub2(0x414C);
        break;
    case 4:
        switch (D_800E6280.unk_0F4.h >> 12) {
        case 0:
            func_800AE0F0(D_800CA19C, "図書室");
            bg_read_sub2(0x40BC);
            break;
        case 1:
            func_800AE0F0(D_800CA19C, "演劇部");
            bg_read_sub2(0x40B2);
            break;
        case 2:
            func_800AE0F0(D_800CA19C, "実験室");
            bg_read_sub2(0x40C6);
            break;
        case 3:
            func_800AE0F0(D_800CA19C, "電脳部室");
            bg_read_sub2(0x40D0);
            break;
        case 4:
            func_800AE0F0(D_800CA19C, "美術室");
            bg_read_sub2(0x40D9);
            break;
        case 5:
            func_800AE0F0(D_800CA19C, "音楽室");
            bg_read_sub2(0x40E2);
            break;
        case 6:
        case 7:
        case 8:
            func_800AE0F0(D_800CA19C, "グランド");
            bg_read_sub2(0x4097);
            break;
        case 9:
            func_800AE0F0(D_800CA19C, "プール");
            bg_read_sub2(0x40A0);
            break;
        default:
            func_800AE0F0(D_800CA19C, "体育館");
            bg_read_sub2(0x40A9);
            break;
        }
        break;
    default:
        func_800AE0F0(D_800CA19C, "廊下");
        bg_read_sub2(0x4045);
        break;
    }
    func_8004284C();
}

void func_80134628(void) {
    u8 girl;
    OmimaiWords11 all;
    OmimaiWords2 first;
    OmimaiWords2 second;
    OmimaiWords2 third;

    all = D_80134D38;
    first = D_80134D64;
    second = D_80134D6C;
    third = D_80134D74;
    if ((D_800E6280.unk_03F < 6U) || (D_800E6280.unk_03F >= 0xAU)) {
        all.unk_00[0] = 0x544A;
        all.unk_00[3] = 0x544A;
        all.unk_00[5] = 0x544A;
        first.unk_00[0] = 0x5D41;
        second.unk_00[1] = 0x522E;
        third.unk_00[1] = 0x58AF;
    }
    if ((D_800E6280.unk_F68.w & 0xF) == 4) {
        girl = D_800E6280.unk_F5F;
        switch (girl) {
        case 0:
            func_80062CD0(all.unk_00[D_800E6280.unk_0F4.h >> 12]);
            break;
        case 1:
            func_80062CD0(first.unk_00[D_800E6280.unk_0F4.h >> 12]);
            break;
        case 2:
            func_80062CD0(second.unk_00[(D_800E6280.unk_0F4.h >> 12) - 2]);
            break;
        case 3:
            func_80062CD0(third.unk_00[(D_800E6280.unk_0F4.h >> 12) - 4]);
            break;
        case 4:
            func_80062CD0(0x66EC);
            break;
        case 5:
            func_80062CD0(0x5FE4);
            break;
        case 6:
            func_80062CD0(0x625A);
            break;
        case 10:
            func_80062CD0(0x6962);
            break;
        case 9:
            func_80062CD0(0x64D0);
            break;
        default:
            if ((D_800E6280.unk_03F >= 6U) && (D_800E6280.unk_03F < 0xAU)) {
                func_80062CD0(0x57FB);
            } else {
                func_80062CD0(0x5828);
            }
            break;
        }
        func_8004284C();
        return;
    }
    read_bustup_uniform();
}
