#include "common.h"
#include "game.h"

/* Menu palette colors, data island 0x800E3680..0x800E36BF. */
u32 D_800E3680[16] = {0};

/* .data range 0x800B40F0..0x800B438F of this object (T-25005: its own section, tools/data_split.py).
 * The template of sndisp's pos[] is D_800B40F8, so the rest of the range follows sndisp. */
INCLUDE_RODATA("asm/data/main/8004F870.data", D_800B40F0);

/* get_now_k (8004E500.c) returns u8; xa_wait needs that view, the other callers keep the implicit int one. T-17018 */
u8 get_now_k(void);

/* Accessed menu-selection view at GameState +0x1090. */
typedef struct MenuSelection5 {
    /* 0x00 */ u8 count[2];
    /* 0x02 */ u8 pad_02;
    /* 0x03 */ s8 current[2];
} MenuSelection5; /* size 0x05 */

s32 menu_set(menu, count, x, y, w, h)
    s32 menu;
    u8 count;
    s16 *x;
    s16 *y;
    s16 *w;
    s16 *h;
{
    GameState *g;
    s32 i;
    s16 *px;
    s16 *py;
    s16 *pw;
    s16 *ph;

    g = &D_800E6280;
    for (i = 0; i < 16; i++) {
        ((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_07 = 0;
        ((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_05 = 0;
        ((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_04 = 0;
    }
    px = x;
    py = y;
    pw = w;
    ph = h;
    if (count >= 17) {
        return -1;
    }
    ((MenuSelection5 *)&g->unk_1090)->count[menu] = count;
    g->unk_1095[menu] = -1;
    if (menu >= 2) {
        return -1;
    }
    for (i = 0; i < count; i++) {
        ((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_00 = *px++;
        ((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_02 = *py++;
        if ((*pw * *ph) == 0) {
            ((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_07 = 1;
        } else {
            ((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_07 = 0;
        }
        ((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_04 = *pw++;
        ((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_05 = *ph++;
        ((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_06 = 0;
    }
}

s32 menu_check(arg0, arg1, arg2)
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

s32 menu_check_1(arg0, arg1, arg2)
    s32 arg0;
    s16 arg1;
    s16 arg2;
{
    GameState *g;
    s32 cnt;
    s32 sel;
    u32 i;

    g = &D_800E6280;
    cnt = 0;
    sel = -1;
    for (i = 0; i < ((MenuSelection5 *)&g->unk_1090)->count[arg0]; i++) {
        ((GsRecF90 (*)[2])g->unk_F90)[i][arg0].unk_06 = 0;
        if ((s32)((arg1 - ((GsRecF90 (*)[2])g->unk_F90)[i][arg0].unk_00) * ((arg1 - ((GsRecF90 (*)[2])g->unk_F90)[i][arg0].unk_00) - ((GsRecF90 (*)[2])g->unk_F90)[i][arg0].unk_04)) < 0) {
            if ((s32)((arg2 - ((GsRecF90 (*)[2])g->unk_F90)[i][arg0].unk_02) * ((arg2 - ((GsRecF90 (*)[2])g->unk_F90)[i][arg0].unk_02) - ((GsRecF90 (*)[2])g->unk_F90)[i][arg0].unk_05)) < 0) {
                ((GsRecF90 (*)[2])g->unk_F90)[i][arg0].unk_06 = 1;
                cnt += 1;
                sel = i;
            }
        }
    }
    if (cnt == 0) {
        ((MenuSelection5 *)&g->unk_1090)->current[arg0] = -1;
    }
    if (!(g->unk_F80 & 4) && sel != -1) {
        g->unk_1095[arg0] = sel;
    }
    return sel;
}

s32 menu_check_2(arg0, arg1, arg2)
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

s32 get_near_menu(s32 mode, s32 dir, s32 menu) {
    GameState *g;
    s32 n;
    s16 best_d;
    s32 best;
    s16 diff[16];
    s32 i;

    g = &D_800E6280;
    n = 16;
    best_d = 320;
    if (((MenuSelection5 *)&g->unk_1090)->count[menu] < 16) {
        n = ((MenuSelection5 *)&g->unk_1090)->count[menu];
    }
    switch (mode) {
    case 0:
        for (i = 0; i < n; i++) {
            diff[i] = ((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_00 - ((GsRecF90 (*)[2])g->unk_F90)[((MenuSelection5 *)&g->unk_1090)->current[menu]][menu].unk_00;
        }
        break;
    case 1:
        for (i = 0; i < n; i++) {
            diff[i] = ((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_02 - ((GsRecF90 (*)[2])g->unk_F90)[((MenuSelection5 *)&g->unk_1090)->current[menu]][menu].unk_02;
        }
        break;
    }
    switch (dir) {
    case -1:
        for (i = 0; i < n; i++) {
            if (diff[i] > 0) {
                diff[i] = 0;
            } else {
                diff[i] = -diff[i];
            }
        }
        break;
    case 1:
        for (i = 0; i < n; i++) {
            if (diff[i] < 0) {
                diff[i] = 0;
            }
        }
        break;
    }
    best = -1;
    for (i = 0; i < n; i++) {
        if (((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_07 == 0 && diff[i] != 0) {
            if (mode == 0 && ((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_02 == ((GsRecF90 (*)[2])g->unk_F90)[((MenuSelection5 *)&g->unk_1090)->current[menu]][menu].unk_02) {
                if (best_d > 0) {
                    best_d = -320;
                }
                if (best_d < -abs(((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_00 - ((GsRecF90 (*)[2])g->unk_F90)[((MenuSelection5 *)&g->unk_1090)->current[menu]][menu].unk_00)) {
                    best_d = -abs(((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_00 - ((GsRecF90 (*)[2])g->unk_F90)[((MenuSelection5 *)&g->unk_1090)->current[menu]][menu].unk_00);
                    best = i;
                }
            } else if (mode == 1 && ((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_00 == ((GsRecF90 (*)[2])g->unk_F90)[((MenuSelection5 *)&g->unk_1090)->current[menu]][menu].unk_00) {
                if (best_d > 0) {
                    best_d = -320;
                }
                if (best_d < -abs(((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_02 - ((GsRecF90 (*)[2])g->unk_F90)[((MenuSelection5 *)&g->unk_1090)->current[menu]][menu].unk_02)) {
                    best_d = -abs(((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_02 - ((GsRecF90 (*)[2])g->unk_F90)[((MenuSelection5 *)&g->unk_1090)->current[menu]][menu].unk_02);
                    best = i;
                }
            } else if (best_d > 0) {
                if (diff[i] < best_d) {
                    best_d = diff[i];
                    best = i;
                } else if (best_d == diff[i]) {
                    switch (mode) {
                    case 0:
                        if (abs(((GsRecF90 (*)[2])g->unk_F90)[best][menu].unk_02 - ((GsRecF90 (*)[2])g->unk_F90)[((MenuSelection5 *)&g->unk_1090)->current[menu]][menu].unk_02) > abs(((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_02 - ((GsRecF90 (*)[2])g->unk_F90)[((MenuSelection5 *)&g->unk_1090)->current[menu]][menu].unk_02)) {
                            best = i;
                        }
                        break;
                    case 1:
                        if (abs(((GsRecF90 (*)[2])g->unk_F90)[best][menu].unk_00 - ((GsRecF90 (*)[2])g->unk_F90)[((MenuSelection5 *)&g->unk_1090)->current[menu]][menu].unk_00) > abs(((GsRecF90 (*)[2])g->unk_F90)[i][menu].unk_00 - ((GsRecF90 (*)[2])g->unk_F90)[((MenuSelection5 *)&g->unk_1090)->current[menu]][menu].unk_00)) {
                            best = i;
                        }
                        break;
                    }
                }
            }
        }
    }
    if (best >= 0) {
        return best;
    }
    return -1;
}

void menu_bar_color(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg0 >= 0x10) {
        D_800E3680[0] = 0x832F7FAF;
        D_800E3680[1] = 0x822F7F2F;
        return;
    }
    /* FAKE: redundant nibble mask preserves the original temporary numbering. T-24501 */
    D_800E3680[arg0] = ((arg2 & 0xF) << 0x18) + arg1 + ((arg3 & 0xF) << 0x1C);
}

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

void x_taku_menu_set(s32 menu, s32 count)
{
    s32 pad; /* FAKE: unused local shifts array homes down by 8 bytes within the 0x70 frame. T-24111 */
    s32 i;
    s16 x[4];
    s16 width[4];
    s16 height[4];

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

void sndisp(u8 *str, s32 y, s32 col) {
    s32 pad[2]; /* FAKE: T-25001, two unused home words at the top of the 0x168 frame */
    s32 i;
    s32 q;
    s32 n;
    u8 pos[5] = { 0, 0, 0, 0, 0 };
    u8 buf[256];

    strncpy(buf, str, 256);
    q = 0;
    n = 0;
    for (i = 0; buf[i] != 0; i++, q++) {
        if (buf[i] == ' ' && !(q & 1)) {
            pos[n] = i;
            buf[i] = 0;
            q = -1;
            n++;
        }
        if (buf[i] == 'P' && !(q & 1)) {
            q = -1;
        }
        if (buf[i] == 'G' && !(q & 1)) {
            q = -1;
        }
    }
    D_800E6280.unk_F61 = check_set_k();
    sndi(buf, y, col);
    for (i = 0; i < n; i++) {
        sndi(&buf[pos[i]] + 1, y + 1 + i, col);
    }
    D_800E6280.unk_F60 = check_set_k();
    D_800E6280.unk_F62 = 0x80;
}

/* rest of the 0x800B40F0 range, after the template of sndisp */
INCLUDE_RODATA("asm/data/main/8004F870.data", D_800B4100);
INCLUDE_RODATA("asm/data/main/8004F870.data", D_800B4110);
INCLUDE_RODATA("asm/data/main/8004F870.data", D_800B4120);
INCLUDE_RODATA("asm/data/main/8004F870.data", D_800B4130);
INCLUDE_RODATA("asm/data/main/8004F870.data", D_800B4198);
INCLUDE_RODATA("asm/data/main/8004F870.data", D_800B419C);
INCLUDE_RODATA("asm/data/main/8004F870.data", D_800B41A0);
INCLUDE_RODATA("asm/data/main/8004F870.data", D_800B41B0);
INCLUDE_RODATA("asm/data/main/8004F870.data", D_800B436C);
INCLUDE_RODATA("asm/data/main/8004F870.data", D_800B437C);

void sndi(u8 *str, s32 y, s32 col) {
    u8 *s;
    u8 original[64];
    /* FAKE: the sizes below 64 are inferred from the target's frame offsets (each buffer ends
     * where the next one starts); the real declarations are unknown. T-25101 */
    u8 quoted[40];
    u8 stage[40];
    u8 result[40];
    u8 name[20];
    u8 *p;
    s32 i;
    s32 found;

    strncpy(original, str, 64);
    D_800E6280.unk_F62 = 0;
    found = 0;
    if ((col & 15) == 15) {
        for (i = 0, s = original; s[i] != 0; i++) {
            p = s + i;
            if (*p == 'P' && (i & 1) == 0) {
                *p = 0;
                found = 1;
                strcpy(stage, original);
                get_p_name(name, col & 15);
                strcat(stage, name);
                strcat(stage, p + 1);
                break;
            }
        }
        if (found == 0) {
            strcpy(stage, original);
        }
        found = 0;
        for (i = 0, s = stage; s[i] != 0; i++) {
            p = s + i;
            if (*p == 'G' && (i & 1) == 0) {
                *p = 0;
                found = 1;
                strcpy(result, stage);
                get_g_name(name, D_800E6280.unk_F5F);
                strcat(result, name);
                strcat(result, p + 1);
                break;
            }
        }
        if (found == 0) {
            strcpy(result, stage);
        }
    } else {
        for (i = 0, s = original; s[i] != 0; i++) {
            p = s + i;
            if (*p == 'P' && (i & 1) == 0) {
                *p = 0;
                found = 1;
                strcpy(result, original);
                get_p_name(name, D_800E6280.unk_F5F);
                strcat(result, name);
                strcat(result, p + 1);
                break;
            }
        }
        if (found == 0) {
            strcpy(result, original);
        }
    }
    switch (y) {
    case 0:
        if (col == 0xF) {
            strcpy(quoted, "『");
            strcat(quoted, result);
        } else if (col == 0x1F) {
            strcpy(quoted, "（");
            strcat(quoted, result);
        } else if (col == 0x2F) {
            strcpy(quoted, result);
        } else {
            strcpy(quoted, "「");
            strcat(quoted, result);
        }
        if (col == 0xFF) {
            set_kanji_string(-0x80, 0x32, 0, quoted, 0);
            return;
        }
        set_kanji_string(-0x56, 0x32, 0, quoted, 0);
        return;
    case 1:
        if (col == 0xFF) {
            set_kanji_string(-0x80, 0x42, 0, result, 0);
            return;
        }
        set_kanji_string(-0x48, 0x42, 0, result, 0);
        return;
    default:
    case 2:
        if (col == 0xFF) {
            set_kanji_string(-0x80, 0x52, 0, result, 0);
            return;
        }
        set_kanji_string(-0x48, 0x52, 0, result, 0);
        return;
    }
}

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

void menu_girl_taku_set(s32 menu, s32 *arg1)
{
    s32 i;
    s32 g;
    Rec38 *p;
    s16 x[12];
    s16 y[12];
    s16 w[12];
    s16 h[12];

    i = 0;
    do {
        if (i == 11) {
            g = 12;
        } else {
            g = i;
        }
        p = &D_800E6280.unk_1BC[g];
        if (p->unk_0C.f.b1) {
            *arg1 = set_kanji_string((i / 3) * 64 - 0x80, (i % 3) * 16 + 0x32, 0, girl_short_name_table[g], 0);
        }
        x[i] = (i / 3) * 64 - 0x80;
        y[i] = (i % 3) * 16 + 0x32;
        if (p->unk_0C.f.b1) {
            w[i] = 0x30;
        } else {
            w[i] = 0;
        }
        h[i] = 0x10;
        menu_set(menu, 12, x, y, w, h);
        i++;
    } while (i != 12);
}

void xa_wait(void) {
    /* FAKE: two unused home words above the spills; the original frame is 8 bytes larger. T-17018 */
    s32 pad[2];
    s32 r;
    u8 *a;
    u8 *b;
    s32 now;
    s32 ph;
    u32 t;

    r = 0;
    now = get_now_k();
    if (D_800E6280.unk_F62 & 0x80) {
        ph = D_800E6280.unk_F62 & 0x7F;
        if (D_800E6280.unk_F61 + 2 >= (u32)now) {
            r = k_disp_inc();
            now = get_now_k();
            t = D_800E6280.unk_F61;
            if (now == t + 3) {
                if (D_800E6280.unk_F60 >= t + 3) {
                    k_disp_switch(now, 0);
                }
            }
        } else if (ph == 0) {
            k_disp_switch(D_800E6280.unk_F61, 0);
            a = get_k_work(D_800E6280.unk_F61 + 1);
            b = get_k_work(D_800E6280.unk_F61 + 2);
            *(s16 *)(a + 2) -= 2;
            *(s16 *)(b + 2) -= 2;
            D_800E6280.unk_F62 += 1;
        } else if (ph != 0) {
            /* FAKE: redundant test; the second use makes ph a uopt variable in $v0 as in the original (a plain else gives a $t9 temporary). T-17018 */
            a = get_k_work(D_800E6280.unk_F61 + 1);
            b = get_k_work(D_800E6280.unk_F61 + 2);
            *(s16 *)(a + 2) -= 2;
            *(s16 *)(b + 2) -= 2;
            if (*(s16 *)(a + 2) < 0x33) {
                *(s16 *)(a + 2) = 0x32;
                *(s16 *)(b + 2) = 0x42;
                D_800E6280.unk_F62 -= 1;
                D_800E6280.unk_F61 += 1;
                k_disp_switch(D_800E6280.unk_F61 + 2, 1);
            }
        }
    } else {
        r = k_disp_inc();
        get_now_k();
    }
    if (r == 1) {
        if (D_8011ED5B & 0x80) {
            func_8006C934();
        } else {
            func_8006C848(1);
        }
    }
    if (D_800E6280.unk_F88 & 0x60) {
        if (r == 1) {
            func_8006C848(0);
            D_800E6280.unk_110D += 1;
            D_800E6280.unk_F62 = 0;
            return;
        }
        k_disp_goto_line_end();
    }
}

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

void func_8005215C(void) {
    /* FAKE: two unused home words above the spills; the original frame is 8 bytes larger, as in xa_wait. T-25101 */
    s32 pad[2];
    s32 r;
    u8 *a;
    u8 *b;
    s32 *k;
    s32 now;
    s32 ph;
    u32 t;

    r = 0;
    now = get_now_k();
    if (D_800E6280.unk_F62 & 0x80) {
        ph = D_800E6280.unk_F62 & 0x7F;
        if (D_800E6280.unk_F61 + 2 >= (u32)now) {
            r = k_disp_inc();
            now = get_now_k();
            t = D_800E6280.unk_F61;
            if (now == t + 3) {
                if (D_800E6280.unk_F60 >= t + 3) {
                    k_disp_switch(now, 0);
                }
            }
        } else if (ph == 0) {
            /* FAKE: `+ 0` keeps this argument off the F61 range, which then has no a0 preference and takes v1 (F62 a0) as in the original; the plain argument gives a2. T-25101 */
            k_disp_switch(D_800E6280.unk_F61 + 0, 0);
            a = get_k_work(D_800E6280.unk_F61 + 1);
            b = get_k_work(D_800E6280.unk_F61 + 2);
            *(s16 *)(a + 2) -= 2;
            *(s16 *)(b + 2) -= 2;
            D_800E6280.unk_F62 += 1;
        } else if (ph != 0) {
            /* FAKE: redundant test, as in xa_wait; the second use keeps ph in $v0. T-25101 */
            a = get_k_work(D_800E6280.unk_F61 + 1);
            b = get_k_work(D_800E6280.unk_F61 + 2);
            *(s16 *)(a + 2) -= 2;
            *(s16 *)(b + 2) -= 2;
            if (*(s16 *)(a + 2) < 0x33) {
                *(s16 *)(a + 2) = 0x32;
                *(s16 *)(b + 2) = 0x42;
                D_800E6280.unk_F62 -= 1;
                D_800E6280.unk_F61 += 1;
                k_disp_switch(D_800E6280.unk_F61 + 2, 1);
            }
        }
    } else {
        /* FAKE: the s8 mode field is read through a u8 address view: lbu, and uopt keeps the byte as one promoted global
         * reloaded after each call; `(u8)D_800E6280.unk_03C` adds conversion ranges that cache it. T-25101 */
        if (((*(u8 *)&D_800E6280.unk_03C) == 1 && D_80125C00 != 0) || ((*(u8 *)&D_800E6280.unk_03C) == 2 && D_80125C00 != 0)) {
            r = k_disp_inc();
        }
        if ((*(u8 *)&D_800E6280.unk_03C) == 0) {
            r = k_disp_inc();
        }
        get_now_k();
    }
    if (D_80125C00 == 0 && (func_800460EC() & 4)) {
        D_80125C00 = 1;
    }
    k = (s32 *)func_80046284();
    if (D_80125C00 == 1) {
        if (func_800460EC() & 2) {
            if (D_80125BFC != -1) {
                if (((s32 *)D_80125BF4)[D_80125BFC * 2] != ((s32 *)D_80125BF8)[D_80125BFC * 2] && ((s32 *)D_80125BF4)[D_80125BFC * 2] == k[2]) {
                    func_80046290(((s32 *)D_80125BF8)[D_80125BFC * 2], ((s32 *)D_80125BF8)[D_80125BFC * 2 + 1], D_800E6280.unk_F5F);
                    func_80044750(0x300);
                    D_80125C00 = 0;
                } else {
                    D_80125C00 = 2;
                }
            } else {
                D_80125C00 = 2;
            }
        }
    }
    if (r == 1 && D_80125C00 == 2) {
        if (D_8011ED5B & 0x80) {
            func_8006C934();
        } else {
            func_8006C848(1);
        }
    }
    if (r == 1 && (*(u8 *)&D_800E6280.unk_03C) == 0) {
        if (D_8011ED5B & 0x80) {
            func_8006C934();
        } else {
            func_8006C848(1);
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        if (r == 1) {
            if ((*(u8 *)&D_800E6280.unk_03C) == 2) {
                D_80125C00 = 2;
                func_80044750(0x74);
                func_80045414(9, 0, 0);
            }
            D_800E6280.unk_F62 = 0;
        } else {
            k_disp_goto_line_end();
        }
    }
    if (D_800E6280.unk_F88 & 0x60) {
        if (r == 1) {
            if ((*(u8 *)&D_800E6280.unk_03C) == 1) {
                if (D_80125C00 == 2) {
                    func_8006C848(0);
                    D_800E6280.unk_110D += 1;
                }
                D_800E6280.unk_F62 = 0;
                return;
            }
            if ((*(u8 *)&D_800E6280.unk_03C) == 2) {
                if (D_80125C00 == 2) {
                    func_8006C848(0);
                    D_800E6280.unk_110D += 1;
                }
                D_800E6280.unk_F62 = 0;
                return;
            }
            func_8006C848(0);
            D_800E6280.unk_110D += 1;
            D_800E6280.unk_F62 = 0;
            return;
        }
        k_disp_goto_line_end();
    }
}

/* Love level of girl arg0: base value from her record, plus one bonus from D_800B4130 for each
 * parameter that her mask in D_800B4110 selects, by the threshold ladder of her kind (D_800B4120).
 * The u32 level locals give the frame and the colour order of the thresholds; reading the
 * parameters through gs gives the bonus row s1 and the parameter value s2 (T-25101). */
u32 get_h_tokimeki(s32 arg0) {
    s32 girl;
    s32 sum;
    s32 i;
    u8 mask;
    u8 kind;
    u32 lv0;
    u32 lv1;
    u32 lv2;
    u32 lv3;
    u32 lv4;
    u32 lv5;
    u32 lv6;
    s32 use[8];
    s8 *bonus;
    GameState *gs;

    gs = &D_800E6280;
    girl = arg0 % 13;
    sum = gs->unk_1BC[girl].unk_02 * D_800B4100[girl];
    sum /= 100;
    mask = D_800B4110[girl];
    for (i = 1; i < 8; i++) {
        if (mask & (1 << i)) {
            use[i] = 1;
        } else {
            use[i] = 0;
        }
    }
    kind = D_800B4120[girl];
    lv0 = D_800B4198[0];
    lv1 = D_800B4198[1];
    lv2 = D_800B4198[2];
    lv3 = D_800B4198[3];
    lv4 = D_800B4198[4];
    lv5 = D_800B4198[5];
    lv6 = D_800B4198[6];
    for (i = 1; i < 7; i++) {
        if (use[i]) {
            switch (kind) {
            case 1:
                bonus = D_800B4130[girl];
                if ((&gs->unk_0FC)[i].unk_02 > lv0 + 50) {
                    sum += bonus[0];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv1 + 35) {
                    sum += bonus[1];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv2 + 20) {
                    sum += bonus[2];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv3 + 20) {
                    sum += bonus[3];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv4 + 5) {
                    sum += bonus[4];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv5 + 5) {
                    sum += bonus[5];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv6) {
                    sum += bonus[6];
                } else {
                    sum += bonus[7];
                }
                break;
            case 2:
                bonus = D_800B4130[girl];
                if ((&gs->unk_0FC)[i].unk_02 > lv0) {
                    sum += bonus[0];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv1) {
                    sum += bonus[1];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv2) {
                    sum += bonus[2];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv3) {
                    sum += bonus[3];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv4) {
                    sum += bonus[4];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv5) {
                    sum += bonus[5];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv6) {
                    sum += bonus[6];
                } else {
                    sum += bonus[7];
                }
                break;
            case 3:
                bonus = D_800B4130[girl];
                if ((&gs->unk_0FC)[i].unk_02 > lv0 + 50) {
                    sum += bonus[0];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv1 + 50) {
                    sum += bonus[1];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv2 + 50) {
                    sum += bonus[2];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv3 + 20) {
                    sum += bonus[3];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv4 + 20) {
                    sum += bonus[4];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv5 + 20) {
                    sum += bonus[5];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv6 + 20) {
                    sum += bonus[6];
                } else {
                    sum += bonus[7];
                }
                break;
            case 4:
                bonus = D_800B4130[girl];
                if ((&gs->unk_0FC)[i].unk_02 > lv0 + 10) {
                    sum += bonus[0];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv1 + 10) {
                    sum += bonus[1];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv2 + 10) {
                    sum += bonus[2];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv3 + 10) {
                    sum += bonus[3];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv4) {
                    sum += bonus[4];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv5) {
                    sum += bonus[5];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv6) {
                    sum += bonus[6];
                } else {
                    sum += bonus[7];
                }
                break;
            case 5:
                bonus = D_800B4130[girl];
                if ((&gs->unk_0FC)[i].unk_02 > lv0 + 20) {
                    sum += bonus[0];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv1 + 20) {
                    sum += bonus[1];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv2 + 20) {
                    sum += bonus[2];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv3 + 20) {
                    sum += bonus[3];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv4) {
                    sum += bonus[4];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv5 - 5) {
                    sum += bonus[5];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv6) {
                    sum += bonus[6];
                } else {
                    sum += bonus[7];
                }
                break;
            case 6:
                bonus = D_800B4130[girl];
                if ((&gs->unk_0FC)[i].unk_02 > lv0) {
                    sum += bonus[0];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv1) {
                    sum += bonus[1];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv2) {
                    sum += bonus[2];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv3) {
                    sum += bonus[3];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv4 - 5) {
                    sum += bonus[4];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv5 - 5) {
                    sum += bonus[5];
                } else if ((&gs->unk_0FC)[i].unk_02 > lv6 - 5) {
                    sum += bonus[6];
                } else {
                    sum += bonus[7];
                }
                break;
            }
        }
    }
    if (sum >= 0x65) {
        return 0x64;
    }
    if (sum < 0) {
        return 0;
    }
    return sum & 0xFF;
}

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
