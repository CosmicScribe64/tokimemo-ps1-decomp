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
    D_800E71DF = arg0;
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
    if (D_800E7208 & 0x860) {
        k_disp_goto_line_end();
        if (check_end_k() != 0) {
            D_800E738D += 1;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_80052060);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_8005215C);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", get_h_tokimeki);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", get_h_yuukou);

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
    birth_day_check_days(arg0, D_800E62BF, D_800E62C0);
}

INCLUDE_ASM("asm/nonmatchings/main/8004F870", birth_day_check_days);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_80052E60);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_80052F68);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_80053418);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_8005352C);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_80053564);
