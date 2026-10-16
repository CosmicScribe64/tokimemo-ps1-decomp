#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80075320);

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

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_800754C0);

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

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80075AF8);

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

typedef struct {
    u32 idx : 5;
    u32 pad : 20;
    u32 nib : 4;
    u32 b29 : 1;
    u32 rest : 2;
} EntryFlags29;

/* NON_MATCHING: T-10910, structure matches (97 of 97 instructions) with the bit-field entry view, the 5 explicit clears and an if (1) around cnt[idx]++ (found by decomp-permuter, FAKE); only register choice differs: the original colours n, m, arg0, idx in that order (..), 24 lines */
#ifdef NON_MATCHING
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
            if (!((EntryFlags29 *)&D_800E6280.unk_54C[i])->b29) {
                break;
            }
            if (1) {
                cnt[((EntryFlags29 *)&D_800E6280.unk_54C[i])->idx]++;
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
            if (!((EntryFlags29 *)&D_800E6280.unk_54C[i])->b29) {
                break;
            }
            if (cnt[((EntryFlags29 *)&D_800E6280.unk_54C[i])->idx] != 0) {
                idx = ((EntryFlags29 *)&D_800E6280.unk_54C[i])->idx;
                arg0[m] = idx;
                m++;
                cnt[((EntryFlags29 *)&D_800E6280.unk_54C[i])->idx] = 0;
                if (m >= 3) {
                    break;
                }
            }
        }
        return n;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80075320", func_800764AC);
#endif

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

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076C7C);

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

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80077694);

/* bulk_m2c declarations (T-11190) */
extern s8 D_800E69DD;
extern u16 D_800E62C2;
extern u16 D_800E6374;
extern u32 D_800E7384;
extern u8 D_800E62C1;
extern u8 D_800E699C;
/* end bulk_m2c declarations */
void func_80077900(void) {
    s32 temp_t6;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s8 temp_v0;
    u32 temp_t1;
    u32 temp_t5;

    if (D_800E7384 == 0x20) {
        temp_t6 = D_80120666 & 1;
        switch (temp_t6) {
        case 0:
            func_80044750(0x501);
            break;
        case 1:
            func_80044750(0x502);
            break;
        }
        parameter_up_down((s32) D_800E6280.unk_69C[D_800E699C].unk_01, 1, (s32) D_800E6280.unk_69C[D_800E699C].unk_02);
        parameter_show_init();
    } else if ((u32) D_800E7384 >= 0x41U) {
        func_80048F64(0x60);
        func_80048F64(0x61);
        if ((D_800E62C1 == 6) && (D_800E6280.unk_69C[D_800E699C].unk_01 == 4) && (temp_t5 = (u16) D_800E6374 >> 0xC, (temp_t5 != 0xB)) && (D_800E6280.unk_66C[temp_t5].unk_00 == 6) && (temp_v0 = func_80053564((s32) temp_t5), D_800E69DD = temp_v0, (temp_v0 != -1)) && ((u16) D_800E62C2 < 0x2DAU)) {
            temp_t1 = (u16) D_800E6374 >> 0xC;
            if (temp_t1 == 0xA) {
                if ((temp_v0 == 0) && (temp_v1 = D_800E6280.unk_1BC[temp_v0].unk_0C.w, (temp_v1 & 4)) && (temp_v1 & 2) && !(*(s32 *)&D_800E6280.unk_66C[temp_t1] & 0x20000000) && !(temp_v1 & 0x40000)) {
                    D_800E6280.unk_1BC[temp_v0].unk_0C.b[2] |= 4;
                    main_state_set(0x60, (s32) temp_t1);
                } else {
                    temp_v1_2 = D_800E6280.unk_1BC[temp_v0].unk_0C.w;
                    if ((temp_v1_2 & 4) && (temp_v1_2 & 2) && !(*(s32 *)&D_800E6280.unk_66C[temp_t1] & 0x20000000) && !(temp_v1_2 & 0x40000)) {
                        D_800E6280.unk_1BC[temp_v0].unk_0C.b[2] |= 4;
                        main_state_set(0x60, (s32) temp_t1);
                    } else {
                        goto block_31;
                    }
                }
            } else {
                temp_v1_3 = D_800E6280.unk_1BC[temp_v0].unk_0C.w;
                if ((temp_v1_3 & 2) && (temp_v1_3 & 4) && !(*(s32 *)&D_800E6280.unk_66C[temp_t1] & 0x20000000) && !(temp_v1_3 & 0x40000)) {
                    D_800E6280.unk_1BC[temp_v0].unk_0C.b[2] |= 4;
                    main_state_set(0x60, (s32) temp_t1);
                } else {
                    goto block_31;
                }
            }
        } else {
block_31:
            func_80072B5C(0);
        }
    }
}

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

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80077CA8);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80077E30);

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

INCLUDE_ASM("asm/nonmatchings/main/80075320", telephone_class_init);

void holiday_club_init(void) {
    func_80065F34(0);
    func_8004E58C();
    x_taku_string_set("クラブ活動をする", "クラブをやめる", 0, 2);
    x_taku_menu_set(1, 2);
    D_800E6280.unk_1094 = 0;
    k_disp_start(2);
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_8007866C);

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
