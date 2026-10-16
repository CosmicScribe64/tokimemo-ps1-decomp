#include "common.h"
#include "game.h"

/* Accessed menu-selection view at GameState +0x1090. */
typedef struct MenuSelection5 {
    /* 0x00 */ u8 count[2];
    /* 0x02 */ u8 pad_02;
    /* 0x03 */ s8 current[2];
} MenuSelection5; /* size 0x05 */

INCLUDE_ASM("asm/nonmatchings/main/8004F870", menu_set);

s8 menu_check(arg0, arg1, arg2)
    s32 arg0;
    s16 arg1;
    s16 arg2;
{
    u8 x;

    if (arg0 >= 2) {
        return -1;
    }
    x = D_800E6280.unk_1092;
    if ((x & 3) == 0) {
        return menu_check_1(arg0, arg1, arg2);
    }
    if ((x & 3) == 1) {
        menu_check_2(arg0, arg1, arg2);
        return (&D_800E6280.unk_1093)[arg0];
    }
    if ((x & 3) == 3) {
        menu_check_2(arg0, arg1, arg2);
        return (&D_800E6280.unk_1093)[arg0];
    }
    if (D_800E6280.unk_F88 & 4) {
        return menu_check_1(arg0, arg1, arg2);
    }
    if (D_800E6280.unk_F80 & 4) {
        return menu_check_2(arg0, arg1, arg2);
    }
    return menu_check_1(arg0, arg1, arg2);
}

/* FAKE: duplicate GameState pointer steers record/base allocation. T-12050 (inherited T-10850) */
/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7200;
/* end bulk_m2c declarations */
s8 menu_check_1(s32 arg0, s16 arg1, s16 arg2) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 var_v1;
    u32 var_a3;
    u32 var_t0;
    u8 *temp_t1;
    u8 *var_t2;

    temp_t1 = &D_800E6280 + arg0;
    var_v1 = 0;
    var_a3 = -1U;
    var_t0 = 0;
    if (*(u8 *)((unsigned char *)temp_t1 + (4240)) != 0) {
        var_t2 = (arg0 * 8) + &D_800E6280;
        do {
            temp_a0 = arg1 - *(s16 *)((unsigned char *)var_t2 + (3984));
            *(s8 *)((unsigned char *)var_t2 + (3990)) = 0;
            if (((temp_a0 - *(u8 *)((unsigned char *)var_t2 + (3988))) * temp_a0) < 0) {
                temp_a0_2 = arg2 - *(s16 *)((unsigned char *)var_t2 + (3986));
                if (((temp_a0_2 - *(u8 *)((unsigned char *)var_t2 + (3989))) * temp_a0_2) < 0) {
                    *(s8 *)((unsigned char *)var_t2 + (3990)) = 1;
                    var_v1 += 1;
                    var_a3 = var_t0;
                }
            }
            var_t0 += 1;
            var_t2 += 0x10;
        } while (var_t0 < (u8) *(u8 *)((unsigned char *)(arg0 + &D_800E6280) + (4240)));
    }
    if (var_v1 == 0) {
        *(s8 *)((unsigned char *)temp_t1 + (4243)) = -1;
    }
    if (!(D_800E7200 & 4) && (var_a3 != -1U)) {
        D_800E6280.unk_1095[arg0] = (s8) var_a3;
    }
    return (s8) var_a3;
}

s8 menu_check_2(arg0, arg1, arg2)
    s32 arg0;
    s32 arg1;
    s32 arg2;
{
    GameState *g;
    s32 sel;
    u32 i;

    g = &D_800E6280;
    sel = -1;
    g->unk_1095[arg0] = -1;
    i = 0;
    if (((MenuSelection5 *)&g->unk_1090)->count[arg0] != 0) {
        do {
            if (i != ((MenuSelection5 *)&g->unk_1090)->current[arg0]) {
                ((GsRecF90 (*)[2])g->unk_F90)[i][arg0].unk_06 = 0;
            } else {
                ((GsRecF90 (*)[2])g->unk_F90)[i][arg0].unk_06 = 10;
                sel = i;
            }
            i += 1;
        } while (i < ((MenuSelection5 *)&g->unk_1090)->count[arg0]);
    }
    if (!(g->unk_F80 & 4) && sel != -1) {
        g->unk_1095[arg0] = sel;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_8004FC10);

s32 func_80050324(s32 menu, s32 reverse) {
    GameState *g;
    s32 i;
    /* The menu entries are interleaved: one record for each of two menus. */
    g = &D_800E6280;
    if (reverse != 0) {
        for (i = ((MenuSelection5 *)&g->unk_1090)->count[menu] - 1; i >= 0; i--) {
            if (((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_04 && ((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_05) {
                ((MenuSelection5 *)&g->unk_1090)->current[menu] = i;
                return i;
            }
        }
    } else {
        for (i = 0; (u32)i < ((MenuSelection5 *)&g->unk_1090)->count[menu]; i++) {
            if (((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_04 && ((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_05) {
                ((MenuSelection5 *)&g->unk_1090)->current[menu] = i;
                return i;
            }
        }
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/main/8004F870", get_near_menu);

/* NON_MATCHING: T-12050, 17 differing listing rows; shared lui requires unowned D_800E3680 data range */
#ifdef NON_MATCHING
void menu_bar_color(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg0 >= 0x10) {
        D_800E3680[0] = 0x832F7FAF; D_800E3680[1] = 0x822F7F2F;
        return;
    }
    D_800E3680[arg0] = ((arg2 & 0xF) << 0x18) + arg1 + (arg3 << 0x1C);
}
#else
INCLUDE_ASM("asm/nonmatchings/main/8004F870", menu_bar_color);
#endif

void menu_bar_show(menu)
    s32 menu;
{
    s32 mode;
    s32 page;
    s32 flag;
    s8 item;
    page = (D_800E3680[menu] >> 24) & 15;
    mode = (D_800E3680[menu] >> 28) & 7;
    flag = (D_800E3680[menu] >> 31) << 7;
    item = ((MenuSelection5 *)&D_800E6280.unk_1090)->current[menu];
    if (item >= 0 && (u32)item < ((MenuSelection5 *)&D_800E6280.unk_1090)->count[menu]) {
        func_80049A40(((GsRecF90 (*)[2])D_800E6280.unk_F90)[item][menu].unk_00, ((GsRecF90 (*)[2])D_800E6280.unk_F90)[item][menu].unk_02, ((GsRecF90 (*)[2])D_800E6280.unk_F90)[item][menu].unk_04, ((GsRecF90 (*)[2])D_800E6280.unk_F90)[item][menu].unk_05, page, D_800E3680[menu] & 0xFFFFFF, flag | mode);
        dtd_on_tpage(0, 0, page, 0, 1);
    }
}

/* NON_MATCHING: T-12050, 24 differing listing rows; array homes are eight bytes too high. */
#ifdef NON_MATCHING
void x_taku_menu_set(s32 menu, s32 count)
{
    s16 x[4];
    s16 width[4];
    s16 height[4];
    s32 i;

    for (i = 0; i < count; i++) {
        x[i] = -120;
        width[i] = 240;
        height[i] = 16;
    }
    if (count == 3) {
        menu_set(menu, 3, x, D_800B40F0, width, height);
        return;
    }
    menu_set(menu, 2, x, D_800B40F0, width, height);
}
#else
INCLUDE_ASM("asm/nonmatchings/main/8004F870", x_taku_menu_set);
#endif

void x_taku_string_set(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    set_kanji_string(-0x78, 0x32, 0, arg0, 0);
    set_kanji_string(-0x78, 0x42, 0, arg1, 0);
    if (arg3 == 3) {
        if (arg2 != 0) {
            set_kanji_string(-0x78, 0x52, 0, arg2, 0);
        }
    }
}

void gnsx(u8 *arg0) {
    switch (strlen(arg0)) {
    case 2:
        set_kanji_string(-0x72, 0x32, 0, arg0, 0);
        return;
    case 4:
        set_kanji_string(-0x79, 0x32, 0, arg0, 0);
        return;
    default:
    case 6:
        set_kanji_string(-0x80, 0x32, 0, arg0, 0);
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8004F870", sndisp);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", sndi);

void set_c_girl(u8 arg0) {
    D_800E6280.unk_F5F = arg0;
}

s32 get_g_name(u8 *arg0, s32 arg1) {
    if (arg1 == 0) {
        strcpy(arg0, "詩織");
    } else if (arg1 == 0xA) {
        strcpy(arg0, D_800B3630);
        strcat(arg0, "ちゃん");
    } else if (arg1 == 0xC) {
        strcpy(arg0, D_800B3624);
    } else {
        strcpy(arg0, girl_short_name_table[arg1 & 0xF]);
        strcat(arg0, "さん");
    }
    if (arg1 >= 0x10 || arg1 < 0) {
        return -1;
    }
    return 1;
}

void get_p_name(u8 *arg0, u32 arg1) {
    switch (arg1) {
    case 0:
        if (get_g_zyotai_s((s32) arg1) & 0x80) {
            strcpy(arg0, &D_800E6280.unk_0E4[0]);
            return;
        }
        strcpy(arg0, &D_800E6280.unk_0DC[0]);
        strcat(arg0, &D_800B3634);
        return;
    case 1:
        if (get_g_zyotai_s((s32) arg1) & 0x80) {
            strcpy(arg0, &D_800E6280.unk_0D4[0]);
            strcat(arg0, &D_800B3634);
            return;
        }
        strcpy(arg0, &D_800E6280.unk_0D4[0]);
        strcat(arg0, &D_800B3638);
        return;
    case 2:
        if (get_g_zyotai_s((s32) arg1) & 0x80) {
            strcpy(arg0, &D_800E6280.unk_0DC[0]);
            strcat(arg0, &D_800B3634);
            return;
        }
        strcpy(arg0, &D_800E6280.unk_0D4[0]);
        strcat(arg0, &D_800B3634);
        return;
    case 3:
        if (get_g_zyotai_s((s32) arg1) & 0x80) {
            strcpy(arg0, &D_800E6280.unk_0E4[0]);
            return;
        }
        strcpy(arg0, &D_800E6280.unk_0D4[0]);
        strcat(arg0, &D_800B3634);
        return;
    case 4:
        if (get_g_zyotai_s((s32) arg1) & 0x80) {
            strcpy(arg0, &D_800E6280.unk_0DC[0]);
            strcat(arg0, &D_800B3634);
            return;
        }
        if ((get_g_zyotai_s((s32) arg1) & 0x7F) == 4) {
            strcpy(arg0, &D_800E6280.unk_0D4[0]);
            strcat(arg0, &D_800B3634);
            return;
        }
        strcpy(arg0, &D_800E6280.unk_0D4[0]);
        strcat(arg0, &D_800B3634);
        return;
    case 5:
        if (get_g_zyotai_s((s32) arg1) & 0x80) {
            strcpy(arg0, &D_800E6280.unk_0DC[0]);
            strcat(arg0, &D_800B3638);
            return;
        }
        if ((get_g_zyotai_s((s32) arg1) & 0x7F) == 4) {
            strcpy(arg0, &D_800E6280.unk_0D4[0]);
            strcat(arg0, &D_800B3638);
            return;
        }
        strcpy(arg0, &D_800E6280.unk_0D4[0]);
        strcat(arg0, &D_800B3638);
        return;
    case 6:
        if (get_g_zyotai_s((s32) arg1) & 0x80) {
            strcpy(arg0, &D_800E6280.unk_0E4[0]);
            return;
        }
        strcpy(arg0, &D_800E6280.unk_0D4[0]);
        strcat(arg0, &D_800B3634);
        return;
    case 7:
        if (get_g_zyotai_s((s32) arg1) & 0x80) {
            strcpy(arg0, &D_800E6280.unk_0DC[0]);
            strcat(arg0, &D_800B3634);
            return;
        }
        strcpy(arg0, &D_800E6280.unk_0D4[0]);
        strcat(arg0, &D_800B3634);
        return;
    case 8:
        if (get_g_zyotai_s((s32) arg1) & 0x80) {
            strcpy(arg0, &D_800E6280.unk_0E4[0]);
            return;
        }
        strcpy(arg0, &D_800E6280.unk_0DC[0]);
        strcat(arg0, &D_800B3634);
        return;
    case 9:
        if (get_g_zyotai_s((s32) arg1) & 0x80) {
            strcpy(arg0, &D_800E6280.unk_0D4[0]);
            strcat(arg0, &D_800B3634);
            return;
        }
        strcpy(arg0, &D_800E6280.unk_0D4[0]);
        strcat(arg0, &D_800B3638);
        return;
    case 10:
        if (get_g_zyotai_s((s32) arg1) & 0x80) {
            strcpy(arg0, &D_800E6280.unk_0DC[0]);
            strcat(arg0, &D_800B3638);
            return;
        }
        strcpy(arg0, &D_800E6280.unk_0D4[0]);
        strcat(arg0, &D_800B3638);
        return;
    case 13:
        strcpy(arg0, &D_800E6280.unk_0DC[0]);
        return;
    case 11:
        strcpy(arg0, &D_800E6280.unk_0DC[0]);
        strcat(arg0, &D_800B3634);
        return;
    case 12:
        strcpy(arg0, &D_800E6280.unk_0D4[0]);
        strcat(arg0, &D_800B3634);
        return;
    case 15:
        strcpy(arg0, &D_800E6280.unk_0D4[0]);
        strncat(arg0, &D_800E6280.unk_0DC[0], 6);
        return;
    default:
        strcpy(arg0, &D_800E6280.unk_0D4[0]);
        strcat(arg0, &D_800B3634);
        return;
    }
}

s32 get_g_zyotai_s(s32 arg0) {
    if (get_h_yuukou(arg0) < 0xA) {
        return 4;
    }
    if (get_h_yuukou(arg0) < 0x1E) {
        return 3;
    }
    if (get_h_yuukou(arg0) < 0x32) {
        if (get_h_tokimeki(arg0) < 0x32) {
            return 3;
        }
        return 1;
    }
    if (get_h_yuukou(arg0) < 0x50) {
        if (get_h_tokimeki(arg0) < 0x32) {
            return 2;
        }
        return 1;
    }
    if (get_h_tokimeki(arg0) < 0x32) {
        return 0x82;
    }
    return 0x80;
}

s32 get_g_zyotai_h(s32 arg0) {
    if (get_h_yuukou(arg0) < 0xA) {
        return 4;
    }
    if (get_h_yuukou(arg0) < 0x32) {
        return 3;
    }
    if (get_h_tokimeki(arg0) < 0x32) {
        return 2;
    }
    if (get_h_tokimeki(arg0) < 0x50) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/main/8004F870", menu_girl_taku_set);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", xa_wait);

void func_80052000(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x860) {
        k_disp_goto_line_end();
        if (check_end_k() != 0) {
            D_800E6280.unk_110D += 1;
        }
    }
}

void func_80052060(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 selector;

    if (arg2 == -1) {
        selector = arg3 & 15;
        if (selector != 15) {
            if ((u8)D_800E6280.unk_03C != 0) {
                func_80046290(arg0, arg1, arg3);
                func_80044750(0x300);
            }
        }
    } else {
        selector = arg3 & 15;
        if (selector != 15) {
            if ((u8)D_800E6280.unk_03C != 0) {
                func_80046290(((s32 *)arg0)[arg2 * 2], ((s32 *)arg0)[arg2 * 2 + 1], arg3);
                func_80044750(0x300);
            }
        }
    }
    D_80125BF4 = arg0;
    D_80125BF8 = arg1;
    D_80125BFC = arg2;
    if (selector == 15) {
        D_80125C00 = 2;
        return;
    }
    D_80125C00 = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_8005215C);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", get_h_tokimeki);

u32 get_h_yuukou(s32 arg0) {
    s32 t;
    s32 v;

    t = arg0 % 13;
    v = D_800E6280.unk_1BC[t].unk_06 * D_800B41A0[t];
    v /= 100;
    if (v >= 0x65) {
        return 0x64;
    }
    if (v < 0) {
        return 0;
    }
    return v & 0xFF;
}

void get_h_tokimeki_table(u8 *arg0) {
    s32 i;

    for (i = 0; i < 11; i++) {
        arg0[i] = get_h_tokimeki(i);
    }
}

void get_h_yuukou_table(u8 *arg0) {
    s32 i;

    for (i = 0; i < 11; i++) {
        arg0[i] = get_h_yuukou(i);
    }
}

void birth_day_check(s32 arg0) {
    birth_day_check_days(arg0, D_800E6280.unk_03F, D_800E6280.unk_040);
}

s32 birth_day_check_days(s32 arg0, s32 arg1, s32 arg2) {
    s32 t;
    s32 v;

    if (arg1 == 0xD) {
        arg1 = 1;
    }
    t = arg0 & 0xF;
    if (t == 0xF) {
        v = D_800E6280.unk_0F8;
        if (arg1 == (v & 0xF)) {
            if (arg2 == (u32)(v << 23) >> 27) {
                return 1;
            }
        }
    } else {
        v = D_800E6280.unk_1BC[t].unk_10.w;
        if (arg1 == (v & 0xF)) {
            if (arg2 == (u32)(v << 23) >> 27) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80052E60(u8 *arg0, s16 *arg1) {
    s32 i;
    s32 sum;
    u8 *p;

    if (*arg0 < 0x55U) {
        return 0;
    }
    if (D_800E6280.unk_0F4.b[1] & 0xF) {
        return 0;
    }
    i = 1;
    p = D_800B4341;
    do {
        if ((u32) (&D_800E6280.unk_0FC)[i].unk_02 < p[-1]) {
            return 0;
        }
        i++;
        p++;
    } while (i < 8);
    sum = 0;
    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 1; i < 36; i++) sum += D_800E6280.unk_1BC[0].unk_14[i];
    if (sum < 8) {
        return 0;
    }
    if (D_800E6280.unk_1BC[0].unk_0A >= 0x33) {
        return 0;
    }
    *arg1 = 0x311;
}

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_80052F68);

s32 func_80053418(u8 *arg0, s16 *arg1) {
    s32 i;
    u8 love[16];
    u8 friendship[16];
    s32 j;
    get_h_yuukou_table(friendship);
    get_h_tokimeki_table(love);
    for (i = 0, j = 0; j != 32; j += 2, i++) {
        arg1[i] = 0;
        friendship[i] /= 5U;
        if (friendship[i] >= 20U) {
            friendship[i] = 19;
        }
        love[i] /= 5U;
        if (love[i] >= 20U) {
            love[i] = 19;
        }
        arg0[i] = D_800B41B0[love[i]][friendship[i]];
    }
    func_80052E60(arg0, arg1);
    return func_80052F68(arg0, arg1);
}

s32 func_8005352C(void) {
    u16 x;

    x = D_800E6280.unk_0F4.h;
    if (((x >> 0xC) >= 6) && ((x >> 0xC) < 0xB)) {
        return 1;
    }
    return 0;
}

s32 func_80053564(s32 arg0) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (arg0 == (u32)D_800E6280.unk_1BC[i].unk_0C.b[2] >> 4) {
            return i;
        }
    }
    return -1;
}
