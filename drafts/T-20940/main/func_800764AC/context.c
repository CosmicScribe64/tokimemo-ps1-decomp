#define MAIN_API_OVERRIDE_func_80053418 /* this object calls it as void (no v0 result in the call block, func_800754C0); main_api.h has s32 */
#include "common.h"
#include "game.h"

void func_80053418();

void func_80075320(void) {
    /* FAKE: byte view of the u8 field avoids the redundant selector copy (move v0,v1), as in TEL
     * (T-10630). T-17000 */
    switch (*(u8 *)&D_800E6280.unk_1109) {
    case 0x0:
        func_80075468();
        break;
    case 0x1:
        func_80075BE8();
        break;
    case 0x5:
        func_80076F48();
        break;
    case 0x6:
        holiday_club();
        break;
    case 0x7:
        func_8007894C();
        break;
    case 0x10:
        holiday_tel();
        break;
    case 0x20:
        func_80077BE0();
        break;
    case 0x30:
        func_8005B798();
        break;
    case 0x40:
        func_8005D9B4();
        break;
    case 0xFE:
        func_80076EC0();
        break;
    }
    if (((DisplayActor44 *)D_8011ECD0)[27].unk_03 & 0x80) {
        func_80067870();
    }
    message_window_show();
    func_800578F4(0);
    hizuke_show();
    parameter_show();
    func_8006BA40();
    func_80066C08(1);
}

void func_80075468(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
    default:
        func_800755B4();
        break;
    case 1:
        func_80075A64();
        break;
    }
    func_80066334();
    func_80065B0C(0);
}

/* NON_MATCHING: T-17006, 9 differing lines; the local m keeps the month in v0 as the original does, but the
 * beq delay slot gets move a1,v0 instead of lui a0 and the jal slot stays empty. Same 9 lines with m as u8/u32,
 * (u8)m in the call, the tests nested, m assigned in the condition, or the test and call on one source line;
 * the call reading D_800E6280.unk_03F again puts the month in a1 (lui a0 then fills the slot, 10 lines); a switch
 * on m adds 8 bytes of frame. */
#ifdef NON_MATCHING
s32 func_800754C0(void) {
    s32 m;
    u8 rank[16];
    s16 points[16];

    func_80053418(rank, points);
    if (((D_800E6280.unk_0F8 & 0xF) != (D_800E6280.unk_1BC[0].unk_10.w & 0xF)) ||
        (((u32)(D_800E6280.unk_0F8 << 0x17) >> 0x1B) != ((u32)(D_800E6280.unk_1BC[0].unk_10.w << 0x17) >> 0x1B))) {
        return 0;
    }
    m = D_800E6280.unk_03F;
    if ((m != 3) && ((m != 4) || ((u8)D_800E6280.unk_040 >= 4U))) {
        return 0;
    }
    if (appointment_find_today(D_800E6280.unk_03E, m, D_800E6280.unk_040) > 0) {
        return 0;
    }
    if (birth_day_check(0xF) != 1) {
        return 0;
    }
    if (get_g_zyotai_s(0) & 0x80) {
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80075320", func_800754C0);
#endif

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_800755B4);

void func_80075A64(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(0, func_8005751C(0));
        if (D_800B594C == -1) {
            dec_bg_reset();
            dec_bg_cd_read(D_800B5960[func_80066A2C()], 0);
        } else {
            set_dec_bri(0x80);
            func_80042808();
        }
    }
}

void func_80075AF8(void) {
    s32 i;
    s32 n;

    if (appointment_find_today(D_800E6280.unk_03E, D_800E6280.unk_03F, D_800E6280.unk_040) <= 0) {
        icon_can_use_set(9, 0);
    }
    icon_can_use_set(0xD, 0);
    /* the original reads the s8 field unsigned here (lbu), as 8005A0B0.c does */
    if (*(u8 *)&D_800E6280.unk_F6C == 2) {
        icon_can_use_set(0xE, 0);
    }
    if (D_800E6280.unk_041 == 6) {
        n = 0;
        for (i = 0; i < 11; i++) {
            if (D_800E6280.unk_66C[i].unk_03 & 1) {
                n++;
            }
        }
        if (n >= 11) {
            icon_can_use_set(4, 0);
        }
    } else if ((D_800E6280.unk_0F4.h >> 12) == 11) {
        icon_can_use_set(4, 0);
    }
}

void func_80075BE8(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80075C24();
    }
    func_80066334();
    func_80065B0C(0);
}

void func_80075C24(void) {
    k_disp_inc();
    func_8006BA40();
    if (func_80076000() == 0 && func_80075C84() == 0 && func_8005B0F4() == 0) {
        func_8005B1A8();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80075C84);

s32 func_80075FA0(void) {
    u8 x;
    u16 h;

    x = D_800E6280.unk_71C;
    if (D_800E6280.unk_69C[x].unk_01 != 4) {
        return D_800B699C[D_800E6280.unk_69C[x].unk_01];
    }
    h = D_800E6280.unk_0F4.h;
    return D_800B6780[h >> 0xC];
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076000);


s32 func_800764AC(u32 *arg0) {
    s32 padA[1];
    s32 cnt[32];
    s32 padB[1];
    s32 i;
    s32 n;
    s32 m;
    s32 idx;

    switch (D_800E6280.unk_54C[0].w & 0x1F) {
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
        return 1;
    default:
        for (i = 0; i < 32; i++) {
            cnt[i] = 0;
        }
        for (i = 0; i < 8; i++) {
            if (!((ScheduleEntryFlags *)&D_800E6280.unk_54C[i])->active) {
                break;
            }
            if (1) {
                cnt[((ScheduleEntryFlags *)&D_800E6280.unk_54C[i])->unk_00]++;
            }
        }
        cnt[16] = 0;
        cnt[17] = 0;
        cnt[19] = 0;
        cnt[20] = 0;
        cnt[18] = 0;
        n = 0;
        m = 0;
        for (i = 0; i < 32; i++) {
            if (cnt[i] != 0) {
                n++;
            }
        }
        if (n < 2) {
            *arg0 = (u32)(D_800E6280.unk_54C[0].w << 2) >> 31;
            return 1;
        }
        for (i = 0; i < 8; i++) {
            if (!((ScheduleEntryFlags *)&D_800E6280.unk_54C[i])->active) {
                break;
            }
            if (cnt[((ScheduleEntryFlags *)&D_800E6280.unk_54C[i])->unk_00] != 0) {
                idx = ((ScheduleEntryFlags *)&D_800E6280.unk_54C[i])->unk_00;
                arg0[m] = idx;
                m++;
                cnt[((ScheduleEntryFlags *)&D_800E6280.unk_54C[i])->unk_00] = 0;
                if (m >= 3) {
                    break;
                }
            }
        }
        return n;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076630);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_800767A4);

s32 func_80076B48(void) {
    GameState *g;
    g = &D_800E6280;
    switch (g->unk_110D) {                           /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        xa_wait();
        break;
    case 1:                                         /* switch 1 */
        k_sub_reset();
        func_800767A4();
        g->unk_110D += 1;
        k_sub_disp_start(3);
        break;
    case 2:                                         /* switch 1 */
        menu_check(1, (s32) D_8011ECF6, (s32) D_8011ECFA);
        func_8004FC10(1);
        menu_bar_show(1);
        if (g->unk_F88 & 0x20) {
            switch (g->unk_1094) {                   /* switch 2; irregular */
            case 0:                                 /* switch 2 */
            case 1:                                 /* switch 2 */
                /* FAKE: index the two adjacent scalar choice bytes as a table. T-9910. */
                g->unk_724 = *(&g->unk_725 + g->unk_1094);
                func_8004284C();
                break;
            case 2:                                 /* switch 2 */
                if (g->unk_724 == 2) {
                    g->unk_724 = g->unk_727;
                    func_8004284C();
                }
                break;
            }
        }
        break;
    }
}

/* Implicit int in the original (no return value): the switch selector then stays live in $v0 (or v0,v1,zero in the
 * delay slot of the first beqz). T-17006 */
s32 func_80076C7C(void) {
    s32 pad; /* FAKE: T-17006, unused local that reproduces the original stack layout (a 4-byte slot above sp4C; the
              * buffers are otherwise 4 bytes too high); its real use is unknown. */
    u8 sp4C[0x100];
    u8 sp2C[0x20];

    switch (D_800E6280.unk_110D) {
    case 0:
        k_reset(1);
        k_sub_reset_point_set();
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        strcpy(sp4C, "それじゃ、 ");
        strcat(sp4C, D_800B342C[D_800E6280.unk_724]);
        strcat(sp4C, "で、 ");
        get_g_name(sp2C, (u32)(D_800E6280.unk_54C[0].w * 8) >> 0x1C);
        strcat(sp4C, sp2C);
        strcat(sp4C, "を待つか。）");
        sndisp(sp4C, 0, 0x1F);
        k_sub_disp_start(4);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        func_8004284C();
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", appointment_launch);

void func_80076EC0(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80076630();
        break;
    case 1:
        func_80076B48();
        break;
    case 2:
        func_80076C7C();
        break;
    case 3:
        appointment_launch();
        break;
    }
    func_80066334();
}

void func_80076F48(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8007739C();
        break;
    case 1:
        func_80077694();
        break;
    case 2:
        func_80077900();
        break;
    }
    parameter_show();
    func_80066334();
    func_80065B0C(1);
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076FDC);

s32 func_80077330(void) {
    s32 r;

    if ((D_800E6280.unk_0F4.b[1] & 0xF) == 3) {
        r = dec_bg_cd_read(D_800B5960[func_80066A2C()], 0);
    } else {
        r = dec_bg_cd_read(func_80075FA0(), 0);
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_8007739C);

void func_80077694(void) {
    /* FAKE: T-20940, unused local declared first, as the sibling func_8007437C declares its record pointer; IDO gives
     * every local a slot, so temp spills at sp+0x30 as in the original (sp+0x34 without it). */
    GsRec69C *rec;
    s32 temp;

    func_80048F64(0x60);
    temp = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02;
    if (((D_800E6280.unk_0F4.b[1] & 0xF) == 3) && !(D_800E6280.unk_0F6.h & 1)) {
        addr_init_else_sd(2);
        temp = 0;
    } else if (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 == 4) {
        addr_init_club_sd(D_801230D0);
        if (func_8006F524(4) != 0xF) {
            ((DisplayActor44 *)D_8011ECD0)[97].unk_01 = 1;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_02 = 5;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_38 = 0x01000000;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_03 = 0x84;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (u8 *)D_80125CA8;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (u8 *)D_80125CAC;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (u8 *)D_80125CB0;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_05 = 2;
            if (*(u8 *)&D_800E6280.unk_F6E != 0) {
                ((DisplayActor44 *)D_8011ECD0)[97].unk_06 = 0x82;
            }
            ((DisplayActor44 *)D_8011ECD0)[97].unk_43 = 0x10;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_16 = (s16)(temp + 2);
            ((DisplayActor44 *)D_8011ECD0)[97].unk_26 = -0x18;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_2A = -0x40;
        }
    } else {
        if ((D_801230D0 == 6) || (D_801230D0 == 7)) {
            D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02 = 0;
            temp = 0;
        }
        addr_init_holiday_sd(D_801230D0);
    }
    load_palette(D_80125CA4, 0x10, 1, 1, 0);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 5;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0x84;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_0C = (u8 *)D_80125CA8;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_10 = (u8 *)D_80125CAC;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_34 = (u8 *)D_80125CB0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 2;
    if (*(u8 *)&D_800E6280.unk_F6E != 0) {
        ((DisplayActor44 *)D_8011ECD0)[96].unk_06 = 0x82;
    }
    ((DisplayActor44 *)D_8011ECD0)[96].unk_43 = 0x10;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = temp;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0x50;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x40;
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80077900);

void func_80077BE0(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80077C50();
        break;
    case 1:
        func_80077E30();
        break;
    case 2:
        magazine_exit();
        break;
    }
    func_80066334();
}

void func_80077C50(void) {
    func_8006A044(0, 0);
    func_8004E58C();
    func_8006612C("情報誌");
    func_8006A2CC();
    icon_disp_switch(0);
    hizuke_disp_switch(0);
    func_8004284C();
}

/* NON_MATCHING: T-17006, 28 differing lines; the original copies the selector into v0 (or v0,v1,zero) in the delay
 * slot of the first beq, before the dispatch, so the case blocks keep their own lui v0 and the dispatch slots hold
 * the next li at. Every form tried (selector local, return local, s32/u32 return, comma and assignment in the
 * switch) leaves the copy at the default return (move v0,v1 or andi v0,v1,0xff) and hoists lui v0 into the beq
 * slots. */
#ifdef NON_MATCHING
u8 func_80077CA8(void) {
    s32 x;

    x = D_800E6280.unk_03E;
    switch (x) {
    default:
        return x;
    case 0x5F:
        if (D_800E6280.unk_03F < 5U) {
            return 0;
        }
        if (D_800E6280.unk_03F < 8U) {
            return 1;
        }
        if (D_800E6280.unk_03F < 0xBU) {
            return 2;
        }
        return 3;
    case 0x60:
        if (D_800E6280.unk_03F < 2U) {
            return 3;
        }
        if (D_800E6280.unk_03F < 5U) {
            return 4;
        }
        if (D_800E6280.unk_03F < 8U) {
            return 5;
        }
        if (D_800E6280.unk_03F < 0xBU) {
            return 6;
        }
        return 7;
    case 0x61:
        if (D_800E6280.unk_03F == 8) {
            return 0xD;
        }
        if (D_800E6280.unk_03F < 2U) {
            return 7;
        }
        if (D_800E6280.unk_03F < 5U) {
            return 8;
        }
        if (D_800E6280.unk_03F < 8U) {
            return 9;
        }
        if (D_800E6280.unk_03F < 0xBU) {
            return 0xA;
        }
        return 0xB;
    case 0x62:
        if (D_800E6280.unk_03F < 2U) {
            return 0xB;
        }
        return 0xC;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80077CA8);
#endif

void func_80077E30(void) {
    magazine_base_show();
    /* FAKE: byte view of the u8 field avoids the redundant selector copy (move v0,v1), as in TEL
     * (T-10630) on the same field. T-17000 */
    switch (*(u8 *)&D_800E6280.unk_110D) {
    case 0:
        xa_wait();
        return;
    case 1:
        if (D_800B3CFC != 0) {
            D_800E6280.unk_110D += 1;
            return;
        }
        func_8004284C();
        return;
    case 2:
        k_sub_reset();
        k_sub_disp_start(0);
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_800B6CF8[func_80077CA8()], 0, 0x1F);
        D_800E6280.unk_110D += 1;
        return;
    case 3:
        xa_wait();
        return;
    case 4:
    default:
        func_8004284C();
        return;
    }
}

void magazine_exit(void) {
    func_8006D138();
    hizuke_disp_switch(1);
    func_80042908(0);
}

void holiday_tel(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        holiday_tel_init();
        break;
    case 1:
        holiday_tel_call();
        break;
    case 2:
        holiday_tel_exit();
        break;
    }
    cal_base_show();
}

void holiday_tel_init(void) {
    func_8004E58C();
    func_80048E78();
    func_80065F34(0);
    icon_disp_switch(0);
    telephone_class_init();
    k_sub_reset_point_set();
    gnsx(D_800E6280.unk_0D4);
    sndi((u8 *)"誰に電話かけようかな？）", 0, 0x1F);
    k_sub_disp_start(2);
    func_8004284C();
}

void holiday_tel_call(void) {
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042908(0);
        return;
    }
    if ((D_800E6280.unk_F88 & 0x20) && D_800E6280.unk_1093 != -1) {
        if (D_800E6280.unk_1093 == 0xD) {
            func_80042908(0);
            return;
        }
        func_80044750(0x50A);
        if (D_800E6280.unk_1093 == 0) {
            D_800E6280.unk_F5F = 0xD;
        } else if (D_800E6280.unk_1093 == 0xC) {
            D_800E6280.unk_F5F = 0xB;
        } else {
            D_800E6280.unk_F5F = D_800E6280.unk_1093 - 1;
        }
        func_8004284C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", holiday_tel_exit);

void telephone_class_init(void) {
    s32 i;
    /* FAKE: T-20940, a second local above the four row arrays puts them at sp+0xBC/0xA0/0x84/0x68 as in the
     * original (IDO gives every local a slot); its real use is unknown, here it holds the first row's text. */
    u8 *str;
    s16 x[14];
    s16 y[14];
    s16 w[14];
    s16 h[14];

    set_kanji_string(-0x80, -0x54, 3, "電話番号", 0);
    for (i = 0; i < 14; i++) {
        x[i] = (i / 5) * 0x58 - 0x88;
        y[i] = (i % 5) * 0x10 - 0x38;
        w[i] = 0x48;
        h[i] = 0xE;
        if (i == 0) {
            str = D_800B3538[13];
            set_kanji_string(x[i], y[i], 3, str, 0);
        } else if (i == 13) {
            y[i] += 0x10;
            set_kanji_string(x[i], y[i], 3, "取り消し", 0);
        } else if (i == 12) {
            set_kanji_string(x[i], y[i], 3, D_800B3538[12], 0);
        } else if ((u32)(D_800E6280.unk_1BC[i - 1].unk_0C.w << 28) >> 31) {
            set_kanji_string(x[i], y[i], 3, D_800B3538[i - 1], 0);
        } else {
            w[i] = 0;
        }
    }
    menu_set(0, 14, x, y, w, h);
    D_800E6280.unk_1093 = 0;
    func_80048F64(0x60);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 0x40;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0x40000000;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0x84;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_0C = sprite_frame_table;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_10 = D_800C9A14;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_34 = D_800C9730;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_14 = D_800C9A54;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 8;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_06 = 2;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0x3F;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0x10;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x13;
}

void holiday_club_init(void) {
    func_80065F34(0);
    func_8004E58C();
    x_taku_string_set("クラブ活動をする", "クラブをやめる", 0, 2);
    x_taku_menu_set(1, 2);
    D_800E6280.unk_1094 = 0;
    k_disp_start(2);
    func_8004284C();
}

void func_8007866C(void) {
    u8 buf[72];
    GameState *gs;

    menu_check(1, (s32) ((DisplayActor44 *)D_8011ECD0)->unk_26, (s32) ((DisplayActor44 *)D_8011ECD0)->unk_2A);
    menu_bar_show(1);
    func_8004FC10(1);
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042908(0);
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {
        case 0:
            func_8004284C();
            return;
        case 1:
            func_80042940((D_800E6280.unk_110A + 2) & 0xFF);
            func_8004E58C();
            strcpy(buf, club_name_table[(u16) D_800E6280.unk_0F4.h >> 0xC]);
            strcat(buf, "部を退部した。");
            sndi(buf, 0, 0xFF);
            gs = &D_800E6280;
            /* FAKE: T-17006, a bit-field store through the borrowed CharFlags view (one ugen temporary more than
             * `|= 1`, so the original's t6..t0 numbering); the record's real bit-field layout is unknown. */
            ((CharFlags *)&gs->unk_66C[D_800E6280.unk_0F4.h >> 12].unk_03)->b0 = 1;
            gs->unk_0F4.b[1] = (gs->unk_0F4.b[1] & 0xFF0F) | 0xB0;
            k_disp_start(1);
            break;
        }
    }
}

void holiday_club_exit(void) {
    func_80048E78();
    if (func_8005352C() == 1 && (u32)(D_800E6280.unk_040 + 6) / 7 == 3 && D_800E6280.unk_03F % 3U == 0 && D_800E6280.unk_041 == 6) {
        main_state_set(0x73);
        return;
    }
    func_80042908(5);
}

void holiday_taibu_club_exit(void) {
    func_80048E78();
    if (D_800E6280.unk_F88 & 0x60) {
        func_80042908(0);
    }
}

s32 holiday_club(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        holiday_club_init();
        break;
    case 1:
        func_8007866C();
        break;
    case 2:
        holiday_club_exit();
        break;
    case 3:
        holiday_taibu_club_exit();
        break;
    }
}

void holiday_club_join_exit(void) {
    func_80048E78();
    icon_disp_switch(1);
    func_80042908(5);
}

void func_8007894C(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        join_club_init();
        break;
    case 1:
        join_club_select();
        break;
    case 2:
        join_club_message();
        break;
    case 3:
        holiday_club_join_exit();
        break;
    }
    cal_base_show();
}
