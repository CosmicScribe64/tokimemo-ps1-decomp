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

INCLUDE_ASM("asm/nonmatchings/main/8004F870", x_taku_string_set);

INCLUDE_ASM("asm/nonmatchings/main/8004F870", gnsx);

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
