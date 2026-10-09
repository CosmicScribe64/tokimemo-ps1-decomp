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

INCLUDE_ASM("asm/nonmatchings/main/8004F870", get_g_zyotai_s);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", get_g_zyotai_h);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", menu_girl_taku_set);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", xa_wait);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_80052000);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_80052060);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_8005215C);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", get_h_tokimeki);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", get_h_yuukou);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", get_h_tokimeki_table);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", get_h_yuukou_table);

void birth_day_check(s32 arg0) {
    birth_day_check_days(arg0, D_800E62BF, D_800E62C0);
}

INCLUDE_ASM("asm/nonmatchings/main/8004F870", birth_day_check_days);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_80052E60);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_80052F68);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_80053418);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_8005352C);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", func_80053564);
