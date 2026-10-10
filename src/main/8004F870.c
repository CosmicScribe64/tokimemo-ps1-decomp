#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/8004F870", menu_set);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", menu_check);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", menu_check_1);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", menu_check_2);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_8004FC10);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_80050324);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", get_near_menu);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", menu_bar_color);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", menu_bar_show);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", x_taku_menu_set);

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

INCLUDE_ASM("asm/nonmatchings/main/8004F870", get_g_name);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", get_p_name);

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

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_80052060);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_8005215C);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", get_h_tokimeki);

u32 get_h_yuukou(s32 arg0) {
    s32 t;
    s32 v;

    t = arg0 % 13;
    v = D_800E6280.unk_1BC[t].unk_06 * D_800B41A0[t] / 100;
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

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_80053418);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_8005352C);

s32 func_80053564(s32 arg0) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (arg0 == (u32)D_800E6280.unk_1BC[i].unk_0C.b[2] >> 4) {
            return i;
        }
    }
    return -1;
}
