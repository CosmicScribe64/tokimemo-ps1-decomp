#define MAIN_API_OVERRIDE_func_80049A40 /* implicit declaration, as matched; main_api.h has a prototype with narrow parameters */
#include "common.h"
#include "game.h"

void func_80049A40();

/* FAKE: the u8 view of the selector keeps the compares on the global ($v1), as in the original (T-5010) */
void func_8005A0B0(void) {
    switch (*(u8 *)&D_800E6280.unk_1109) {
    case 0:
        func_8005A2A8();
        break;
    case 1:
        func_8005AE60();
        break;
    case 2:
        func_8005B798();
        break;
    case 3:
        func_8005B908();
        break;
    case 4:
        func_8005D9B4();
        break;
    case 5:
        join_club();
        break;
    case 0xFF:
        func_8005A1A0();
        break;
    }
    if (*D_8011F3FF & 0x80) {
        func_80067870();
    }
    func_80066C08(0);
}

/* FAKE: u8 views of the selector and of the counter give the original's global (LOD) access: compares on $v1 and the `andi`/`or v0` copy of the increment (T-5010) */
s32 func_8005A1A0(void) {
    D_800E6280.unk_1104.w += 1;
    switch (*(u8 *)&D_800E6280.unk_110A) {
    case 0:
        func_8005A37C();
        break;
    case 1:
        func_8005A560();
        break;
    case 2:
        func_8005A668();
        break;
    case 3:
        func_8005ABD0();
        break;
    }
    hizuke_show();
    message_window_show();
    parameter_show();
    func_80065B0C(0);
    func_8006BA40();
    if (D_800E6280.unk_110A >= 2) {
        func_80066334();
        if (*(u8 *)&D_800E6280.unk_03A < 0x80) {
            *(u8 *)&D_800E6280.unk_03A += 4;
        }
    }
}

void func_8005A2A8(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005A37C();
        break;
    case 1:
        func_8005A3E8();
        break;
    case 2:
        func_8005A668();
        break;
    case 3:
        func_8005ABD0();
        break;
    }
    hizuke_show();
    message_window_show();
    parameter_show();
    func_80065B0C(0);
    func_8006BA40();
    func_80066334();
    func_800578F4(get_last_gamen_mode());
}

void func_8005A37C(void) {
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048E78();
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_111A = 0;
    set_dec_bri(0);
    func_8004E58C();
    k_disp_start(2);
    func_8004284C();
}

void func_8005A3E8(void) {
    k_disp_inc();
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005A410);

void func_8005A560(void) {
    if (D_800E6280.unk_110D == 0) {
        if (func_80044E8C() == 1) {
            func_8004500C(0, 0);
            D_800E6280.unk_110D += 1;
        }
    } else if (D_800E6280.unk_110D == 1) {
        cd_read_sectors(0x95U, 0x801A0000, 0x9B94);
        D_800E6280.unk_110D += 1;
    } else if (D_800E6280.unk_110D == 2) {
        if (cd_read_sync_status() & 1) {
            func_80068938(D_800E6280.unk_03E, D_800E6280.unk_03F, 0);
            D_800E6280.unk_1092 &= 3;
            D_800B5BC8 = 0xFE;
            if (D_800B5C08 < 0xE) {
                func_800676AC(D_800B5C08);
                func_8006764C(1);
            }
            func_8004284C();
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005A668);

void func_8005AB4C(void) {
    s32 r;

    r = dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
    switch (r) {
    case 0:
    case 1:
        dec_bg_show_set(0, r);
        set_dec_bri(0x80);
        D_800E6280.unk_03A = 0x80;
        func_80042808();
        break;
    case -1:
        func_8004284C();
        break;
    }
}

void func_8005ABD0(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(0, func_8005751C(0));
        if (D_800B594C == -1) {
            dec_bg_reset();
            dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
        } else {
            set_dec_bri(0x80);
            D_800E6280.unk_03A = 0x80;
            func_80042908(1);
        }
    }
}

void func_8005AC70(s32 arg0) {
    s32 i;

    icon_can_use_set(0xA, 0);
    icon_can_use_set(8, 0);
    icon_can_use_set(9, 0);
    if (*(u8 *)&D_800E6280.unk_F6C == 2) {
        icon_can_use_set(0xE, 0);
    }
    if ((D_800E6280.unk_0F4.h >> 0xC) == 0xB) {
        icon_can_use_set(4, 0);
    }
    if (arg0 != 0) {
        for (i = 0; i != 8; i++) {
            icon_can_use_set(i, 0);
        }
    }
}

void func_8005AD1C(void) {
    LoadSquare(0x3F0, 0x160, 0x10, 0x18, D_800C9A60);
    load_palette(D_800C9D60, 0xF, 1, 1, 0);
}

void func_8005AD70(void) {
    if ((D_800E6280.unk_0F4.b[1] & 0xF) == 3) {
        D_801217D0[0].unk_00 = 0x01000000;
        D_801217D0[0].unk_04 = -0xC;
        D_801217D0[0].unk_06 = -0xA;
        D_801217D0[0].unk_08 = 0x1E;
        D_801217D0[0].unk_0A = 0x18;
        D_801217D0[0].unk_0C = 0x1E;
        D_801217D0[0].unk_0E = 0xE0;
        D_801217D0[0].unk_0F = 0x60;
        D_801217D0[0].unk_10 = 0;
        D_801217D0[0].unk_12 = 0x1EF;
        D_801217D0[0].unk_14 = 0x80;
        D_801217D0[0].unk_15 = 0x80;
        D_801217D0[0].unk_16 = 0x80;
        D_801217D0[0].unk_18 = 0;
        D_801217D0[0].unk_1A = 0;
        D_801217D0[0].unk_1C = 0x1000;
        D_801217D0[0].unk_1E = 0x1000;
        D_801217D0[0].unk_20 = 0;
        D_800E6280.unk_10A5[0] = 0xA;
    }
}

void func_8005AE60(void) {
    u8 state; /* T-9410: keep the selector in v1 across the compare chain. */
    if (D_800E6280.unk_110A == 0) {
        func_8005B39C();
    }
    func_8005AD70();
    state = D_800E6280.unk_111B;
    if (state == 1) {
        if (func_80054388() == 0x13) {
            func_80044750(0x50F);
            D_800E6280.unk_111B = 0;
            func_80053D10();
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80044750(0x502);
            D_800E6280.unk_111B = 0;
            func_80053D10();
        }
    } else if (state == 0) {
        func_80065900(0U);
    }
}

s32 func_8005AF28(void) {
    if (D_8011ECF6 < 0x22 && D_8011ECF6 >= -0xD && D_8011ECFA < 8 && D_8011ECFA >= -0xB) {
        if (D_800E6280.unk_F88 & 0x20) {
            func_80044750(0xB1);
            func_80044774(0);
            func_80046290(0x1D000185, 0x17000397, 0);
            func_80044750(0x300);
            return 1;
        }
    }
    return 0;
}

s32 func_8005AFC8(void) {
    if (D_8011ECF6 < 0x50 && D_8011ECF6 >= 0x41 && D_8011ECFA < -0x10 && D_8011ECFA >= -0x1F) {
        if (D_800E6280.unk_F88 & 0x20) {
            main_state_set(0x3A);
            return 1;
        }
    }
    return 0;
}

s32 func_8005B040(void) {
    u32 *p;

    if ((D_8011ECF6 < 8) && (D_8011ECF6 >= -0x27) && (D_8011ECFA < 0x24) && (D_8011ECFA >= 0x19)) {
        if (D_800E6280.unk_F88 & 0x20) {
            func_80044750(0xD1);
            func_80044774(0);
            p = D_800B5970 + ((u16)D_800E6280.unk_042 & 7) * 2;
            func_80046290(p[0], p[1], 0xE);
            func_80044750(0x300);
            return 1;
        }
    }
    return 0;
}

s32 func_8005B0F4(void) {
    u32 *p;

    if ((D_8011ECF6 < 8) && (D_8011ECF6 >= -0x27) && (D_8011ECFA < 0x24) && (D_8011ECFA >= 0x19)) {
        if (D_800E6280.unk_F88 & 0x20) {
            func_80044750(0xD1);
            func_80044774(0);
            p = D_800B59B0 + ((u16)D_800E6280.unk_042 & 7) * 2;
            func_80046290(p[0], p[1], 0xE);
            func_80044750(0x300);
            return 1;
        }
    }
    return 0;
}

s32 func_8005B1A8(void) {
    /* FAKE: unused word homes preserve girl at sp+0x2C and frame 0x38. T-10010. */
    s32 pad[2];
    s32 girl;
    s32 variant;
    s32 *pair;

    if ((*D_8011F3FF & 0x80) && (D_8011ECF6 < (D_8011F422 + 0xC)) && ((D_8011F422 - 0xC) < D_8011ECF6) && (D_8011ECFA < (D_8011F426 + 0x30)) && (D_8011F426 < D_8011ECFA) && (D_800E6280.unk_F88 & 0x20) && ((u32) (D_800E6280.unk_10F8 - D_800B59F0) >= 0xF1U)) {
        D_800E6280.unk_F5F = D_8011F414;
        girl = (s32) D_8011F414;
        if (get_g_zyotai_s((s32) D_8011F414) & 0x80) {
            variant = 1;
        } else {
            variant = 0;
        }
        pair = D_800B3B9C + girl * 4 + variant * 2;
        func_80046290(pair[0], pair[1], girl);
        func_80044750(0x300);
        return 1;
    }
    return 0;
}

s32 func_8005B2BC(void) {
    s32 *pair;

    if ((*D_8011F3FF & 0x80) && (D_8011ECF6 < (D_8011F422 + 0xC)) && ((D_8011F422 - 0xC) < D_8011ECF6) && (D_8011ECFA < (D_8011F426 + 0x30)) && (D_8011F426 < D_8011ECFA) && (D_800E6280.unk_F88 & 0x20) && ((u32) (D_800E6280.unk_10F8 - D_800B59F4) >= 0xF1U)) {
        pair = D_800B37D4 + D_8011F414 * 2;
        func_80046290(pair[0], pair[1], (s32)(D_800E6280.unk_F5F = D_8011F414));
        func_80044750(0x300);
        return 1;
    }
    return 0;
}

void func_8005B39C(void) {
    k_disp_inc();
    parameter_show();
    hizuke_show();
    message_window_show();
    func_80065B0C(0);
    func_8006BA40();
    func_80066334();
    func_800578F4(0);
    if (func_8005B43C() == 0 && func_8005B2BC() == 0 && func_8005B040() == 0 && func_8005AF28() == 0) {
        func_8005AFC8();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005B43C);

void func_8005B798(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005B830();
        break;
    case 1:
        func_8005B8A0();
        break;
    case 2:
        func_8005B8E0();
        break;
    }
    func_80068EC0();
    parameter_show();
    hizuke_show();
    message_window_show();
    func_80066334();
    func_8006BA40();
}
INCLUDE_RODATA("asm/data/main/8005A0B0.rodata", D_800AFF6C);

INCLUDE_RODATA("asm/data/main/8005A0B0.rodata", D_800AFF78);

void func_8005B830(void) {
    func_8004E58C();
    func_8006612C(D_800AFF6C);
    set_kanji_string(-0x80, 0x32, 0, D_800AFF78, 0);
    k_disp_start(2);
    cal_sprite_init();
    func_80067E34();
    icon_disp_switch(0);
    func_8004284C();
}

void func_8005B8A0(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
    }
}

void func_8005B8E0(void) {
    func_80042908(0);
    func_80042940(2);
}

void func_8005B908(void) {
    u8 temp;
    temp = (u8) D_800E6280.unk_110A;
    switch (temp) {
    case 0x0:
        func_8005BCEC();
        break;
    case 0x1:
        func_8005BE8C();
        break;
    case 0x2:
        func_8005C414();
        break;
    case 0x3:
        func_8005C5BC();
        break;
    case 0x4:
        func_8005CA94();
        break;
    case 0x5:
        func_8005CC40();
        break;
    case 0x6:
        func_8005CF6C();
        break;
    case 0x7:
        func_8005D448();
        break;
    case 0x8:
        func_8005D56C();
        break;
    case 0x9:
        func_8005D804();
        break;
    case 0xA0:
        func_8005C14C();
        break;
    case 0xB0:
        func_8005D31C();
        break;
    case 0xC0:
        func_8005C968();
        break;
    case 0xE0:
        func_8005BAE0();
        break;
    case 0xF0:
        func_8005BC38();
        break;
    case 0xFF:
        func_8005BB84();
        break;
    }
    func_80066334();
    cal_base_show();
    message_window_show();
    parameter_show();
    hizuke_show();
    func_8006BA40();
}

s32 func_8005BAE0(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80044750(0x502);
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, "データが壊れています", 0);
        k_sub_disp_start(1);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (D_800E6280.unk_F88 & 0x860) {
            func_80042940(6);
        }
        break;
    }
}

s32 func_8005BB84(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80044750(0x502);
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, "メモリーカードを認識できません", 0);
        k_sub_disp_start(1);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (D_800E6280.unk_F88 & 0x860) {
            func_8005C4CC(1);
            func_80042908(0);
            func_80042940(2);
        }
        break;
    }
}

s32 func_8005BC38(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80044750(0x502);
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, "メモリーカードに空きがありません", 0);
        k_sub_disp_start(1);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (D_800E6280.unk_F88 & 0x860) {
            func_8005C4CC(1);
            func_80042908(0);
            func_80042940(2);
        }
        break;
    }
}

void func_8005BCEC(void) {
    /* FAKE: reserves the observed stack home; grouped stores preserve scheduling. T-12030. */
    s32 stack_home;
    s16 x[2];
    s16 y[2];
    s16 w[2];
    s16 h[2];

    func_8004E58C();
    func_80066104("メモリーカード");
    k_sub_reset_point_set();
    icon_disp_switch(0);
    data_save_load_class_init();
    x[0] = -0x50;
    x[1] = 0x10;
    y[0] = -0x10;
    y[1] = -0x10;
    w[0] = 0x30; w[1] = 0x30;
    h[0] = 0x14; h[1] = 0x14;
    set_kanji_string(x[0], y[0], 0xFU, "セーブ", 0);
    set_kanji_string(x[1], y[1], 0xFU, "ロード", 0);
    k_disp_switch(1, 0);
    k_disp_switch(2, 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0U, D_800B59F8, 0);
    set_kanji_string(-0x80, 0x42, 0U, D_800B5A1C, 0);
    set_kanji_string(-0x80, 0x4A, 0U, D_800B5A1C, 0);
    set_kanji_string(-0x80, 0x52, 0U, D_800B5A1C, 0);
    set_kanji_string(-0x80, 0x5A, 0U, D_800B5A1C, 0);
    k_sub_disp_start(1);
    func_80056284();
    menu_set(1, 2, x, y, w, h);
    D_800E6280.unk_1094 = 0;
    func_8004284C();
}

s32 func_8005BE8C(void) {
    u8 *status;

    status = &D_800E6280.unk_1115;
    switch (D_800E6280.unk_110D) {
    case 0:
        if (func_80053F04() != 0) {
            if (*status == 1) {
                func_80053D10();
                D_800E6280.unk_110D++;
            } else if (*status == 0xF1) {
                func_80053D10();
                func_80042940(0xA0);
            } else if (*status == 0xFF) {
                func_80053D10();
                func_80042940(0xFF);
            }
        } else if ((u32)D_800E6280.unk_1120 > 0x200) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 1:
        func_80053CE0();
        D_800E6280.unk_110D++;
        break;
    case 2:
        if (func_80054140() == 2) {
            D_800E6280.unk_110D++;
        } else if ((u32)D_800E6280.unk_1120 > 0x200) {
            func_80042940(0xFF);
        }
        break;
    case 3:
        if ((u8)D_800E8BEE != 0) {
            func_80053D10();
            func_80053CE0();
            D_800E6280.unk_110D++;
        } else {
            func_80053D10();
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "セーブまたはロードを選んでください", 0);
            k_disp_switch(1, 1);
            k_disp_switch(2, 1);
            k_disp_start(4);
            func_8004284C();
        }
        break;
    case 4:
        if (func_80054284() == 3) {
            func_800552DC(0);
            func_8005C4CC(1);
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "セーブまたはロードを選んでください", 0);
            k_disp_switch(1, 1);
            k_disp_switch(2, 1);
            k_disp_start(4);
            func_8004284C();
            func_80053D10();
        } else if (((u32)D_800E6280.unk_1120 > 0x200) || (*status == 0xF0)) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    }
    k_disp_inc();
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005C14C);

void func_8005C414(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    menu_bar_show(1);
    if (D_800E6280.unk_F88 & 0x40) {
        func_8005C4CC(1);
        func_80042908(0);
        func_80042940(2);
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {
        case 0:
            func_80042940(3);
            return;
        case 1:
            func_80042940(6);
            break;
        }
    }
}

void func_8005C4CC(s32 arg0) {
    if (arg0 != 0) {
        memcpy(D_80125C10, D_800E6280.unk_163C, 0x40);
        D_80125C50 = D_800E6280.unk_163C[32];
        D_80125C52 = D_800E6280.unk_163C[33];
        D_800B5C08 = *(u8 *)&D_800E6280.unk_163C[34]; /* the low byte of the word stored below */
        D_80125C54 = D_800E6280.unk_163C[35];
        D_80125C55 = D_800E6280.unk_163C[36];
        return;
    }
    memcpy(D_800E6280.unk_163C, D_80125C10, 0x40);
    D_800E6280.unk_163C[32] = D_80125C50;
    D_800E6280.unk_163C[33] = D_80125C52;
    D_800E6280.unk_163C[34] = D_800B5C08;
    D_800E6280.unk_163C[35] = D_80125C54;
    D_800E6280.unk_163C[36] = D_80125C55;
}

void func_8005C5BC(void) {
    s32 i;
    s32 d;
    FlagsWork24 *work;
    s16 x[16];
    s16 y[16];
    s16 w[16];
    s16 h[16];

    func_8004E58C();
    func_80066104("データセーブ");
    d = 5;
    for (i = 0; i < 14; i++) {
        x[i] = (i / d) * 32 - 124;
        y[i] = (i % d) * 16 - 64;
        w[i] = 32;
        h[i] = 16;
        set_kanji_string(x[i], y[i], 15, D_800B32CC[i], 0);
    }
    menu_set(1, 14, x, y, w, h);
    D_800E6280.unk_1094 = 0;
    k_sub_reset_point_set();
    work = (FlagsWork24 *)&D_800E7D10;
    for (i = 1; i != 15; i++) {
        if (work->unk_04[i] == 1) {
            break;
        }
    }
    if ((u8) D_800E8BEE != 0) {
        D_800E6280.unk_1094 = D_800E7D36 - 1;
        D_800E6280.unk_1118 = (s8) D_800E7D36;
        func_80042940(4);
        func_800696DC(0, 0);
        func_8005C7C0();
    } else if (i == 0xF) {
        func_8005C7C0();
        func_80042940(4);
    } else {
        D_800E6280.unk_1118 = i;
        func_80042940(0xC0);
    }
    func_8005C7FC();
}

INCLUDE_RODATA("asm/data/main/8005A0B0.rodata", D_800B00FC);

void func_8005C7C0(void) {
    set_kanji_string(-0x80, 0x32, 0, D_800B00FC, 0);
    k_sub_disp_start(1);
}

void func_8005C7FC(void) {
    s32 i;
    s32 d;
    u8 *p;

    d = 5;
    for (i = 0, p = (u8 *)&D_800E7D10; i < 0xE; i++) {
        if (p[5] == 0) {
            func_80049A40((i / d) * 0x20 - 0x7C, (i % d) * 0x10 - 0x40, 0x1E, 0xF, 2, 0x808080, 0x81);
        }
        if (p[5] == 0xFF) {
            func_80049A40((i / d) * 0x20 - 0x7C, (i % d) * 0x10 - 0x40, 0x1E, 0xF, 2, 0x8080, 0x81);
        }
        p += 1;
    }
    dtd_on_tpage(0, 0, 2, 2, 0);
}

/* FAKE: the u8 view of the selector keeps the compares on the global ($v1), as in the original (T-5010) */
void func_8005C968(void) {
    switch (*(u8 *)&D_800E6280.unk_110D) {
    case 0:
        func_80053CE0();
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_8005448C() == 4) {
            func_80053D10();
            D_800E6280.unk_110D += 1;
        } else if (D_800E6280.unk_1120 >= 0x201U || D_800E6280.unk_1115 == 0xD0) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            func_80042940(4);
            func_800696DC(0, 0);
            func_8005C7C0();
        } else {
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042940(0xFF);
        }
        break;
    }
    func_8005C7FC();
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005CA94);

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005CC40);

void func_8005CF6C(void) {
    s32 i;
    s32 d;
    FlagsWork24 *work;
    s16 x[16];
    s16 y[16];
    s16 w[16];
    s16 h[16];

    func_8004E58C();
    func_80066104("データロード");
    d = 5;
    for (i = 0; i < 14; i++) {
        x[i] = (i / d) * 32;
        y[i] = (i % d) * 16 - 64;
        w[i] = 32;
        h[i] = 16;
        set_kanji_string(x[i], y[i], 15, D_800B32CC[i], 0);
    }
    menu_set(1, 14, x, y, w, h);
    D_800E6280.unk_1094 = 0;
    k_sub_reset_point_set();
    work = (FlagsWork24 *)&D_800E7D10;
    for (i = 1; i != 15; i++) {
        if (work->unk_04[i] == 1) {
            break;
        }
    }
    if ((u8) D_800E8BEE != 0) {
        D_800E6280.unk_1094 = D_800E7D36 - 1;
        func_8004284C();
        D_800E6280.unk_1118 = (s8) D_800E7D36;
        func_800696DC(0, 1);
        func_8005D174();
    } else if (i == 0xF) {
        func_8005D174();
        func_8004284C();
    } else {
        D_800E6280.unk_1118 = i;
        func_80042940(0xB0);
    }
    func_8005D1B0();
}

INCLUDE_RODATA("asm/data/main/8005A0B0.rodata", D_800B0194);

void func_8005D174(void) {
    set_kanji_string(-0x80, 0x32, 0, D_800B0194, 0);
    k_sub_disp_start(1);
}

void func_8005D1B0(void) {
    s32 i;
    s32 d;
    u8 *p;

    d = 5;
    for (i = 0, p = (u8 *)&D_800E7D10; i < 0xE; i++) {
        if (p[5] == 0) {
            func_80049A40((i / d) << 5, (i % d) * 0x10 - 0x40, 0x1E, 0xF, 2, 0x808080, 0x81);
        }
        if (p[5] == 0xFF) {
            func_80049A40((i / d) << 5, (i % d) * 0x10 - 0x40, 0x1E, 0xF, 2, 0x8080, 0x81);
        }
        p += 1;
    }
    dtd_on_tpage(0, 0, 2, 2, 0);
}

void func_8005D31C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80053CE0();
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_8005448C() == 4) {
            func_80053D10();
            D_800E6280.unk_110D += 1;
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xD0)) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            func_800696DC(0, 1);
            func_80042940(7);
            func_8005D174();
        } else {
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042940(0xFF);
        }
        break;
    }
    func_8005D1B0();
}

s32 func_8005D448(void) {
    /* FAKE: two-word local reserves the observed sp+0x2C spill home. T-12030. */
    s32 stack_home[2];
    s32 selection;

    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    selection = D_800E6280.unk_1094 + 1;
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        if (D_800E7D14[selection] != 0) {
            if (selection == (u8) D_800E6280.unk_1118) {
                func_80053CE0();
                func_8004284C();
            } else {
                D_800E6280.unk_1118 = (s8) selection;
                if ((u8) D_800E8BEE != 0) {
                    D_800E7D36 = (u8) D_800E6280.unk_1118;
                    func_800696DC(0, 1);
                } else {
                    func_80042940(0xB0);
                }
            }
        }
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_8005C4CC(1);
        func_80042908(0);
        func_80042940(2);
    }
    k_disp_inc();
    func_8005D1B0();
}

void func_8005D56C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 2U, "データロード中です", 0);
        set_kanji_string(-0x80, 0x42, 2U, "しばらくお待ち下さい", 0);
        k_sub_disp_start(2);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_8005448C() == 4) {
            D_800E6280.unk_110D += 1;
            func_80053D10();
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            func_80042940(0xE0);
        }
        break;
    case 3:
        k_sub_reset();
        func_800696DC(0, 1);
        set_kanji_string(-0x80, 0x32, 1U, "このデータの続きをプレイしますか？", 0);
        set_kanji_string(-0x80, 0x42, 0U, D_800B3C70, 0);
        set_kanji_string(-0x2C, 0x42, 1U, func_8006CA9C(), 0);
        set_kanji_string(-0x80, 0x52, 0U, D_800B3C88, 0);
        set_kanji_string(-0x2C, 0x52, 2U, func_8006CAE0(), 0);
        k_sub_disp_start(0xB);
        D_800E6280.unk_110D += 1;
        break;
    case 4:
        if (D_800E6280.unk_F88 & 0x20) {
            func_80044750(0x501);
            D_800E6280.unk_110D += 1;
        } else if (D_800E6280.unk_F88 & 0x40) {
            func_80042940(6);
        }
        break;
    case 5:
        if (func_80055AFC(0) == 1) {
            func_8004284C();
        } else {
            func_80042940(0xE0);
        }
        break;
    }
    func_8005D1B0();
}

/* FAKE: the u8 view of the selector keeps the compares on the global ($v1), as in the original; the s32 return keeps the switch temporary copy in $v0. T-5010 */
s32 func_8005D804(void) {
    switch (*(u8 *)&D_800E6280.unk_110D) {
    default:
        D_800E6280.unk_110D += 1;
        break;
    case 0:
        if (D_800E6280.unk_03E == 0x62 && D_800E6280.unk_03F == 3 && D_800E6280.unk_040 == 1) {
            D_800E6280.unk_110D += 1;
        } else if (D_800E6280.unk_03E == 0x5F && D_800E6280.unk_03F == 4 && D_800E6280.unk_040 == 4) {
            D_800E6280.unk_110D += 1;
        } else {
            func_80044750(0xB1);
            D_800E6280.unk_110D = 0xC0;
        }
        break;
    case 1:
        func_80044750(5);
        func_80044750(0xB1);
        func_80044890(0, 0xBF98, 0xBF79, 0xCD21, 0xCCD9, 0xCCD1);
        D_800E6280.unk_110D += 1;
        break;
    case 2:
        if (func_80044E8C() == 1) {
            func_80041878();
            func_80041F48();
            D_800E6280.unk_F5E = 0;
            parameter_disp_switch(0);
            hizuke_disp_switch(0);
            message_disp_switch(0);
            func_8006764C(0);
            func_8004500C(0, 0);
            main_state_set(0x14);
        }
        break;
    case 0xFF:
        func_80065F34(0);
        main_state_set(0x34);
        func_80042908(0xFF);
        func_800674B0();
        break;
    }
}

void func_8005D9B4(void) {
    switch (*(u8 *)&D_800E6280.unk_110A) { /* FAKE: scalar byte view of the switch selector. T-11170 */
    case 0x0:
        system_settings_init();
        break;
    case 0x1:
        func_8005E018();
        break;
    case 0x10:
        func_8005E150();
        break;
    case 0x11:
        func_8005E2C0();
        break;
    case 0x12:
        func_8005E40C();
        break;
    case 0x20:
        func_8005E454();
        break;
    case 0x21:
        func_8005E5C8();
        break;
    case 0x22:
        func_8005E7A4();
        break;
    case 0x30:
        func_8005E7F0();
        break;
    case 0x31:
        func_8005E924();
        break;
    case 0x32:
        func_8005E9EC();
        break;
    case 0x33:
        func_8005EA38();
        break;
    case 0x34:
        func_8005EB74();
        break;
    case 0x35:
        func_8005EC78();
        break;
    case 0x36:
        func_8005ECB8();
        break;
    case 0x37:
        func_8005EE00();
        break;
    case 0x38:
        func_8005EF04();
        break;
    case 0x39:
        func_8005EF44();
        break;
    case 0x3A:
        func_8005F09C();
        break;
    case 0x3B:
        func_8005F22C();
        break;
    case 0x40:
        func_8005F26C();
        break;
    case 0x41:
        func_8005F478();
        break;
    case 0x42:
        func_8005F6E4();
        break;
    case 0x50:
        func_8005F730();
        break;
    case 0x51:
        func_8005FA20();
        break;
    case 0x52:
        func_8005FDA0();
        break;
    case 0x60:
        func_8005FDEC();
        break;
    case 0x61:
        func_8005FF68();
        break;
    case 0x62:
        func_80060104();
        break;
    case 0x70:
        func_80060178();
        break;
    case 0x71:
        func_800603C0();
        break;
    case 0x72:
        func_80060504();
        break;
    case 0xFF:
        func_80060150();
        break;
    }
    func_80066334();
    cal_base_show();
    message_window_show();
    parameter_show();
    hizuke_show();
    func_8006BA40();
}

s32 system_settings_init(void) {
    s32 i;
    s32 available;
    s16 x[8];
    s16 y[8];
    s16 w[8];
    s16 h[8];

    func_8004E58C();
    func_8006612C("システム設定画面");
    icon_disp_switch(0);
    data_save_load_class_init();
    for (i = 0; i < 8; i++) {
        x[i] = -0x7C;
        y[i] = -0x40 + i * 0xC;
        w[i] = 0x60;
        h[i] = 0xC;
    }
    func_8005DEA0(-1);
    available = func_8005DDB4();
    if (available == 0) {
        w[6] = 0;
    }
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "システム設定を変更します", 0);
    menu_set(0, 8, x, y, w, h);
    D_800E6280.unk_1093 = (s8)D_800E6280.unk_1093 % 8;
    k_sub_disp_start(0);
    func_8004284C();
}

/* FAKE: the no-op index mask preserves the GameState loop base.
 * T-9560; original indexing expression unknown. */
s32 func_8005DDB4(void) {
    typedef struct TailFlags {
        /* 0x00 */ u8 pad00[0x13];
        /* 0x13 */ u8 first;
        /* 0x14 */ u8 flags[12];
    } TailFlags; /* size 0x20; observed byte view */
    TailFlags *tail;
    s32 count;
    s32 index;

    count = 0;
    if (((TailFlags *)D_800E7D11)->first != 0) {
        count = 1;
    }
    if (D_800E6280.unk_53C[0] != 0) {
        count++;
    }
    tail = (TailFlags *)D_800E7D11;
    for (index = 0; index < 12; index++) {
        if (tail->flags[index] != 0) {
            count++;
        }
        if (D_800E6280.unk_53C[(index + 1) & 0xFFFFFFFF] != 0) {
            count++;
        }
    }
    return count;
}

s32 func_8005DEA0(s32 arg0) {
    s32 i;
    s16 x[8];
    s16 y[8];
    s16 w[8];
    s16 h[8];
    u8 colour[8];

    for (i = 0; i < 8; i++) {
        x[i] = -0x7C;
        y[i] = -0x40 + i * 0xC;
        w[i] = 0x60;
        h[i] = 0xC;
        if (i == arg0) {
            colour[i] = 4;
        } else {
            colour[i] = 3;
        }
    }
    set_kanji_string(x[0], y[0], colour[0], "文字表示速度", 0);
    set_kanji_string(x[1], y[1], colour[1], "アイコンモード", 0);
    set_kanji_string(x[2], y[2], colour[2], "マウス設定", 0);
    set_kanji_string(x[3], y[3], colour[3], "メニューモード", 0);
    set_kanji_string(x[4], y[4], colour[4], "カーソル速度", 0);
    set_kanji_string(x[5], y[5], colour[5], "音声モード", 0);
    if (func_8005DDB4() != 0) {
        set_kanji_string(x[6], y[6], colour[6], "マスコット", 0);
    }
    set_kanji_string(x[7], y[7], colour[7], "設定終了", 0);
}

void func_8005E018(void) {
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042908(0);
        func_80042940(2);
    } else if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1093) {
        case 0:
            func_80042940(0x10);
            break;
        case 1:
            func_80042940(0x20);
            break;
        case 2:
            if (D_800E6280.unk_F74 == 0) {
                func_80042940(0x30);
            }
            break;
        case 3:
            func_80042940(0x40);
            break;
        case 4:
            func_80042940(0x50);
            break;
        case 5:
            func_80042940(0x60);
            break;
        case 6:
            func_80042940(0x70);
            break;
        case 7:
            func_80042940(0xFF);
            break;
        }
    }
}

s32 func_8005E150(void) {
    s32 i = 0;
    s32 speed;
    s16 x[3];
    s16 y[3];
    s16 w[3];
    s16 h[3];

    func_8004E58C();
    func_8006612C("文字速度");
    func_8005DEA0(0);
    if (i) {} /* FAKE: empty test splits the live range of i. T-12030 */
    for (i = 0; i < 3; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3U, "遅い", 0);
    set_kanji_string(x[1], y[1], 3U, "普通", 0);
    set_kanji_string(x[2], y[2], 3U, "速い", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0U, "文字出力のスピードを変更します", 0);
    menu_set(1, 3, x, y, w, h);
    speed = get_k_speed();
    switch (speed) {
    default:
    case 0:
        D_800E6280.unk_1094 = 2;
        break;
    case 1:
        D_800E6280.unk_1094 = 1;
        break;
    case 3:
        D_800E6280.unk_1094 = 0;
        break;
    }
    k_sub_disp_start(0);
    func_8004284C();
}

void func_8005E2C0(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "文字出力のスピードを遅くしました", 0);
            k_speed_set(3);
            func_8004284C();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "文字出力のスピードを普通にしました", 0);
            k_speed_set(1);
            func_8004284C();
            return;
        case 2:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "文字出力のスピードを速くしました", 0);
            k_speed_set(0);
            func_8004284C();
            break;
        }
    }
}

void func_8005E40C(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0);
        D_800E6280.unk_1093 = 0;
    }
}

void func_8005E454(void) {
    u8 sel;
    s32 i;
    s16 x[4];
    s16 y[4];
    s16 w[4];
    s16 h[4];

    func_8004E58C();
    func_8006612C("アイコンモード");
    func_8005DEA0(1);
    for (i = 0; i < 3; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "ＴＹＰＥ‐Ａ", 0);
    set_kanji_string(x[1], y[1], 3, "ＴＹＰＥ‐Ｂ", 0);
    set_kanji_string(x[2], y[2], 3, "フリー", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "アイコンの位置の設定をします", 0);
    menu_set(1, 3, x, y, w, h);
    sel = D_800E6280.unk_F6C;
    switch (sel) {
    default:
    case 0:
        D_800E6280.unk_1094 = 0;
        break;
    case 1:
        D_800E6280.unk_1094 = 1;
        break;
    case 2:
        D_800E6280.unk_1094 = 2;
        break;
    }
    k_sub_disp_start(0);
    func_8004284C();
}

/* the u8 views: the original reads these two bytes unsigned, the struct declares them s8 (T-8080) */
void func_8005E5C8(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "アイコンモードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "ＴＹＰＥ‐Ａにしました", 0);
            D_800E6280.unk_F6C = 0;
            func_8006509C();
            func_8004284C();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "アイコンモードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "ＴＹＰＥ‐Ｂにしました", 0);
            D_800E6280.unk_F6C = 1;
            func_8006509C();
            func_8004284C();
            return;
        case 2:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "アイコンモードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "フリーにしました", 0);
            if (*(u8 *)&D_800E6280.unk_F6C != 2) {
                D_800E6280.unk_F6D = 0xF;
            }
            D_800E6280.unk_F6C = 2;
            func_800654D0(*(u8 *)&D_800E6280.unk_F6D);
            func_8004284C();
            break;
        }
    }
}

void func_8005E7A4(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0);
        D_800E6280.unk_1093 = 1;
    }
}

void func_8005E7F0(void) {
    s32 pad[2]; /* FAKE: unused locals above the arrays, the original frame is 8 bytes bigger (T-3330) */
    s16 x[3];
    s16 y[3];
    s16 w[3];
    s16 h[3];
    s32 i;

    func_8004E58C();
    func_8006612C("マウス設定");
    func_8005DEA0(2);
    for (i = 0; i < 3; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "決定ボタン", 0);
    set_kanji_string(x[1], y[1], 3, "クリックモード", 0);
    set_kanji_string(x[2], y[2], 3, "クリックスピード", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "マウス使用時の各種設定を選択します", 0);
    menu_set(1, 3, x, y, w, h);
    D_800E6280.unk_1094 = 0;
    k_sub_disp_start(0);
    func_8004284C();
}

void func_8005E924(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {
        case 0:
            func_80042940(0x33);
            return;
        case 1:
            func_80042940(0x36);
            return;
        case 2:
            func_80042940(0x39);
            break;
        }
    }
}

void func_8005E9EC(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0);
        D_800E6280.unk_1093 = 2;
    }
}

void func_8005EA38(void) {
    s32 pad[2]; /* FAKE: unused locals above the arrays, the original frame is 8 bytes bigger (T-3330) */
    s16 x[4];
    s16 y[4];
    s16 w[4];
    s16 h[4];
    s32 i;

    func_8004E58C();
    func_8006612C("決定ボタン設定");
    func_8005DEA0(2);
    for (i = 0; i < 2; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "右ボタンで決定", 0);
    set_kanji_string(x[1], y[1], 3, "左ボタンで決定", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "決定ボタンを選択します", 0);
    menu_set(1, 2, x, y, w, h);
    if (D_800E6280.unk_F75 == 0 || D_800E6280.unk_F75 != 1) {
        D_800E6280.unk_1094 = 1;
    } else {
        D_800E6280.unk_1094 = 0;
    }
    k_sub_disp_start(0);
    func_8004284C();
}

void func_8005EB74(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "右ボタンで決定にしました", 0);
            D_800E6280.unk_F75 = 1;
            func_8004284C();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "左ボタンで決定にしました", 0);
            D_800E6280.unk_F75 = 0;
            func_8004284C();
            break;
        }
    }
}

void func_8005EC78(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0x30);
    }
}

void func_8005ECB8(void) {
    u8 sel; /* FAKE: copy of the unsigned byte (the field is s8) kept in $v0 as the original does (T-5010) */
    s32 i;
    s16 x[4];
    s16 y[4];
    s16 w[4];
    s16 h[4];

    func_8004E58C();
    func_8006612C("クリックモード設定");
    func_8005DEA0(2);
    for (i = 0; i < 2; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "シングルクリック", 0);
    set_kanji_string(x[1], y[1], 3, "ダブルクリック", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "クリックモードを設定します", 0);
    menu_set(1, 2, x, y, w, h);
    sel = D_800E6280.unk_F70;
    switch (sel) {
    case 0:
        D_800E6280.unk_1094 = 0;
        break;
    case 1:
        D_800E6280.unk_1094 = 1;
        break;
    }
    k_sub_disp_start(0);
    func_8004284C();
}

void func_8005EE00(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "シングルクリックにしました", 0);
            D_800E6280.unk_F70 = 0;
            func_8004284C();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "ダブルクリックにしました", 0);
            D_800E6280.unk_F70 = 1;
            func_8004284C();
            break;
        }
    }
}

void func_8005EF04(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0x30);
    }
}

void func_8005EF44(void) {
    s32 pad[2]; /* FAKE: unused locals above the arrays, the original frame is 8 bytes bigger (T-3330) */
    s16 x[4];
    s16 y[4];
    s16 w[4];
    s16 h[4];
    s32 i;

    func_8004E58C();
    func_8006612C("クリックスピード設定");
    func_8005DEA0(2);
    for (i = 0; i < 2; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "初期状態", 0);
    set_kanji_string(x[1], y[1], 3, "変更", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "クリックスピードを設定します", 0);
    set_kanji_string(-0x80, 0x42, 0, "変更する場合、変更にカーソルを", 0);
    set_kanji_string(-0x80, 0x52, 0, "合わせて、ダブルクリックして下さい", 0);
    menu_set(1, 2, x, y, w, h);
    D_800E6280.unk_1094 = 0;
    D_801230E4 = 0;
    k_sub_disp_start(0);
    func_8004284C();
}

/* FAKE: scalar-byte view lets IDO keep the clamp value in v0 across stores. T-12030. */
void func_8005F09C(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
    } else if (((D_800E6280.unk_F75 != 0) && (D_800E6280.unk_F77 & 4)) || ((D_800E6280.unk_F75 == 0) && (D_800E6280.unk_F77 & 8))) {
        switch (D_800E6280.unk_1094) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0U, "クリックスピードを初期状態にしました", 0);
            *(u8 *)&D_800E6280.unk_F73 = 0x20;
            func_8004284C();
            break;
        case 1:
            if (D_801230E4 != 0) {
                *(u8 *)&D_800E6280.unk_F73 = 0xFF - D_801230E4;
                if (*(u8 *)&D_800E6280.unk_F73 < 0x10) {
                    *(u8 *)&D_800E6280.unk_F73 = 0x10;
                }
                if (*(u8 *)&D_800E6280.unk_F73 > 0x40) {
                    *(u8 *)&D_800E6280.unk_F73 = 0x40;
                }
                D_801230E4 = 0;
                k_sub_reset();
                set_kanji_string(-0x80, 0x32, 0U, "クリックスピードを変更しました", 0);
                func_8004284C();
                break;
            }
            D_801230E4 = 0xFF;
            break;
        }
    }
}

void func_8005F22C(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0x30);
    }
}

void func_8005F26C(void) {
    s32 pad; /* FAKE: unused local, the original frame has 4 more bytes above the arrays (T-3330) */
    s32 i;
    s16 x[4];
    s16 y[4];
    s16 w[4];
    s16 h[4];

    func_8004E58C();
    func_8006612C("メニューモード設定");
    func_8005DEA0(3);
    for (i = 0; i < 4; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "１：マウスライク", 0);
    set_kanji_string(x[1], y[1], 3, "２：メニューＡ", 0);
    set_kanji_string(x[2], y[2], 3, "３：メニューＢ", 0);
    set_kanji_string(x[3], y[3], 3, "４：１＋２", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "コントローラでの、メニュー間の", 0);
    set_kanji_string(-0x80, 0x42, 0, "（２択・３択・アイコンなど）", 0);
    set_kanji_string(-0x80, 0x52, 0, "カーソルの動かし方を設定します", 0);
    menu_set(1, 4, x, y, w, h);
    switch (D_800E6280.unk_1092) {
    default:
    case 1:
        D_800E6280.unk_1094 = 1;
        break;
    case 0:
        D_800E6280.unk_1094 = 0;
        break;
    case 3:
        D_800E6280.unk_1094 = 2;
        break;
    case 2:
        D_800E6280.unk_1094 = 3;
        break;
    }
    k_sub_disp_start(0);
    func_8004284C();
}

void func_8005F478(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "メニュー設定を", 0);
            set_kanji_string(-0x56, 0x42, 1, "マウスライク", 0);
            set_kanji_string(-0x80, 0x52, 0, "にしました", 0);
            D_800E6280.unk_1092 = 0;
            func_8004284C();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "メニュー設定を", 0);
            set_kanji_string(-0x56, 0x42, 1, "メニューＡ", 0);
            set_kanji_string(-0x80, 0x52, 0, "にしました", 0);
            D_800E6280.unk_1092 = 1;
            func_8004284C();
            return;
        case 2:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "メニュー設定を", 0);
            set_kanji_string(-0x56, 0x42, 1, "メニューＢ", 0);
            set_kanji_string(-0x80, 0x52, 0, "にしました", 0);
            D_800E6280.unk_1092 = 3;
            func_8004284C();
            return;
        case 3:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "メニュー設定を", 0);
            set_kanji_string(-0x56, 0x42, 1, "マウスライク＋メニューＡ", 0);
            set_kanji_string(-0x80, 0x52, 0, "にしました", 0);
            D_800E6280.unk_1092 = 2;
            func_8004284C();
            break;
        }
    }
}

void func_8005F6E4(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0);
        D_800E6280.unk_1093 = 3;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_8005F730);

void func_8005FA20(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
    } else if (D_800E6280.unk_F88 & 0x20) {
        switch (*(u8 *)&D_800E6280.unk_1094) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "カーソルの移動速度を", 0);
            if ((D_800E7D2C != 0) || (D_800E6280.unk_53C[8] != 0)) {
                set_kanji_string(-0x80, 0x42, 0, "超遅くしました", 0);
            } else {
                set_kanji_string(-0x80, 0x42, 0, "遅くしました", 0);
            }
            D_800E6280.unk_F6F = 1;
            D_800E6280.unk_F74 = 0xFF;
            func_8004284C();
            break;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "カーソルの移動速度を", 0);
            if ((D_800E7D2C != 0) || (D_800E6280.unk_53C[8] != 0)) {
                set_kanji_string(-0x80, 0x42, 0, "ちょろっと遅くしました", 0);
            } else {
                set_kanji_string(-0x80, 0x42, 0, "少し遅くしました", 0);
            }
            D_800E6280.unk_F6F = 2;
            D_800E6280.unk_F74 = 0xFF;
            func_8004284C();
            break;
        case 2:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "カーソルの移動速度を", 0);
            set_kanji_string(-0x80, 0x42, 0, "ふつうにしました", 0);
            D_800E6280.unk_F6F = 3;
            D_800E6280.unk_F74 = 0xFF;
            func_8004284C();
            break;
        case 3:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "カーソルの移動速度を", 0);
            if ((D_800E7D2C != 0) || (D_800E6280.unk_53C[8] != 0)) {
                set_kanji_string(-0x80, 0x42, 0, "ちょろっと速くしました", 0);
            } else {
                set_kanji_string(-0x80, 0x42, 0, "少し速くしました", 0);
            }
            D_800E6280.unk_F6F = 4;
            D_800E6280.unk_F74 = 0xFF;
            func_8004284C();
            break;
        case 4:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "カーソルの移動速度を", 0);
            if ((D_800E7D2C != 0) || (D_800E6280.unk_53C[8] != 0)) {
                set_kanji_string(-0x80, 0x42, 0, "超速くしました", 0);
            } else {
                set_kanji_string(-0x80, 0x42, 0, "速くしました", 0);
            }
            D_800E6280.unk_F6F = 6;
            D_800E6280.unk_F74 = 0xFF;
            func_8004284C();
            break;
        }
    }
}

void func_8005FDA0(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0);
        D_800E6280.unk_1093 = 4;
    }
}

s32 func_8005FDEC(void) {
    s32 i;
    s32 mode;
    s16 x[3];
    s16 y[3];
    s16 w[3];
    s16 h[3];

    func_8004E58C();
    func_8006612C("音声モード");
    func_8005DEA0(5);
    for (i = 0; i < 3; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3U, "常にＯＮ", 0);
    set_kanji_string(x[1], y[1], 3U, "常にＯＦＦ", 0);
    set_kanji_string(x[2], y[2], 3U, "×ボタンでカット", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0U, "音声のモードを設定します", 0);
    menu_set(1, 3, x, y, w, h);
    mode = (u8)D_800E6280.unk_03C;
    switch (mode) {
    case 1:
        D_800E6280.unk_1094 = 0;
        break;
    case 0:
        D_800E6280.unk_1094 = 1;
        break;
    default:
    case 2:
        D_800E6280.unk_1094 = 2;
        break;
    }
    k_sub_disp_start(0);
    func_8004284C();
}

void func_8005FF68(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "音声モードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "常にＯＮにしました", 0);
            D_800E6280.unk_03C = 1;
            func_8004284C();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "音声モードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "常にＯＦＦにしました", 0);
            D_800E6280.unk_03C = 0;
            func_8004284C();
            return;
        case 2:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "音声モードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "×ボタンでカットにしました", 0);
            D_800E6280.unk_03C = 2;
            func_8004284C();
            break;
        }
    }
}

void func_80060104(void) {
    k_disp_inc();
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0);
        D_800E6280.unk_1093 = 5;
    }
}

void func_80060150(void) {
    func_80042908(0);
    func_80042940(2);
}

INCLUDE_ASM("asm/nonmatchings/main/8005A0B0", func_80060178);

s32 func_800603C0(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    k_disp_inc();
    if (D_800E6280.unk_1094 != -1) {
        func_800676AC(0);
        if (((FlagsWork24 *)&D_800E7D10)->flags[D_800E6280.unk_1094] != 0 || D_800E6280.unk_53C[D_800E6280.unk_1094] != 0) {
            func_800676AC(D_800E6280.unk_1094 + 1);
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
    } else if ((D_800E6280.unk_F88 & 0x20) && (D_800E6280.unk_1094 != -1)) {
        if (D_800E6280.unk_1094 == 0xD) {
            func_80042940(0);
        } else if ((((FlagsWork24 *)&D_800E7D10)->flags[D_800E6280.unk_1094] != 0) || (D_800E6280.unk_53C[D_800E6280.unk_1094] != 0)) {
            func_800676AC(0);
            func_800676AC(D_800E6280.unk_1094 + 1);
            func_8004284C();
        }
    }
}

void func_80060504(void) {
    func_80042940(0);
    D_800E6280.unk_1093 = 6;
}

void join_club(void) {
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
        join_club_exit();
        break;
    }
    cal_base_show();
    message_window_show();
    parameter_show();
    hizuke_show();
    func_8006BA40();
}

void join_club_init(void) {
    func_80065F34(0);
    icon_disp_switch(0);
    func_8004E58C();
    club_list_menu();
    func_8004284C();
}

s32 join_club_select(void) {
    menu_check(1, (s32) D_8011ECF6, (s32) D_8011ECFA);
    func_8004FC10(1);
    menu_bar_show(1);
    if (D_800E6280.unk_F88 & 0x40) {
        func_80042908(0);
        if (D_800E6280.unk_1108 == 0x34) {
            func_80042940(2);
        }
    }
    if (D_800E6280.unk_F88 & 0x20) {
        if (D_800E6280.unk_1094 == 0xB) {
            func_80042908(0);
            if (D_800E6280.unk_1108 == 0x34) {
                func_80042940(2);
            }
        } else if (D_800E6280.unk_1094 != -1) {
            ((GsByteNibbles *)&D_800E6280.unk_0F4.b[1])->high = D_800E6280.unk_1094;
            func_8004284C();
        }
    }
}

/* FAKE: scalar-byte selector view keeps the switch index in v1; s32 fall-through
 * preserves its v0 copy. T-12030. */
s32 join_club_message(void) {
    u8 buf[0x44];

    switch (*(u8 *)&D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        strcpy(buf, club_name_table[D_800E6280.unk_0F4.h >> 0xC]);
        strcat(buf, "部に入部した。");
        sndisp(buf, 0, 0xFF);
        k_sub_disp_start(1);
        D_800E6280.unk_110D++;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        if ((D_800E6280.unk_0F4.h >> 0xC) >= 6) {
            k_sub_reset();
            gnsx("部長");
            set_kanji_string(-0x56, 0x32, 0, "「毎月", 0);
            set_kanji_string(-0x2C, 0x32, 1, "第３日曜日", 0);
            set_kanji_string(0x1A, 0x32, 0, "は、", 0);
            set_kanji_string(-0x48, 0x42, 0, "クラブの日なので、", 0);
            set_kanji_string(-0x48, 0x52, 1, "休まない", 0);
            set_kanji_string(-0x10, 0x52, 0, "ように。", 0);
            func_80052060(0x0D000011, 0x0300001F, -1, 0xE);
            D_800E6280.unk_110D++;
        } else {
            D_800E6280.unk_110D = 0xFF;
        }
        break;
    case 3:
        func_8005215C();
        break;
    case 4:
        D_800E6280.unk_110D = 0xFF;
        break;
    case 0xFF:
        func_8004284C();
        break;
    }
}

void join_club_exit(void) {
    func_80048E78();
    main_state_set(0x30);
}

void func_80060958(void) {
    s32 r;

    func_8004E58C();
    func_8006612C("自宅");
    gnsx(D_800E6280.unk_0D4);
    parameter_show_init();
    hizuke_init();
    message_window_init();
    parameter_disp_switch(1);
    func_8006BC28(0);
    func_8006509C();
    icon_disp_switch(1);
    func_8006CE84();
    icon_can_use_set(0, 0);
    icon_can_use_set(D_800E6280.unk_F68.w & 0xF, 1);
    tpage_buf_clear();
    sndisp("最近女の子の間で 変な噂が流れているらしい…）", 0, 0x1F);
    k_disp_start(1);
    r = dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
    if (r != -1) {
        if (r == 0 || r == 1) {
            dec_bg_show_set(0, r);
            set_dec_bri(0x80);
            func_80042808();
        }
    } else {
        func_8004284C();
    }
    k_disp_start(2);
}

void func_80060A84(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(0, func_8005751C(0));
        if (D_800B594C == -1) {
            dec_bg_reset();
            dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
        } else {
            set_dec_bri(0x80);
            D_800E6280.unk_03A = 0x80;
            func_80042908(1);
        }
    }
}

s32 func_80060B24(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80060958();
        break;
    case 1:
        func_80060A84();
        break;
    }
}

void func_80060B78(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        xa_wait();
        return;
    default:
    case 1:
        func_8004284C();
        return;
    }
}
void uwasa_exit0(void) {
    main_state_set(0x31);
}

s32 uwasa0(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80060B78();
        break;
    case 1:
        uwasa_exit0();
        break;
    }
}

void uwasa_main(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80060B24();
        break;
    case 1:
        uwasa0();
        break;
    }
    func_80065B0C(1);
    parameter_show();
    hizuke_show();
    message_window_show();
    func_80066334();
    func_800578F4(0);
    func_80066C08(1);
}
s32 pre_xmas_init(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8004E58C();
        func_80048E78();
        func_80041584();
        func_80066104("自宅");
        gnsx(D_800E6280.unk_0D4);
        parameter_show_init();
        parameter_show_init();
        hizuke_init();
        message_window_init();
        parameter_disp_switch(1);
        icon_disp_switch(0);
        tpage_buf_clear();
        sndisp("今日は伊集院の家で、 クリスマスパーティだ。 会場に行こうかな）", 0, 0x1F);
        k_disp_start(6);
        func_8006BC28(0);
        func_8005AB4C();
        return;
    case 1:
        func_8005ABD0();
        return;
    }
}

s32 pre_xmas(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        xa_wait();
        break;
    case 1:
        main_state_set(0x52);
        break;
    }
}

void func_80060DF8(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        pre_xmas_init();
        break;
    case 1:
        pre_xmas();
        break;
    }
    parameter_show();
    hizuke_show();
    message_window_show();
    func_8006BA40();
    func_80066334();
    if (*D_8011F3FF & 0x80) {
        func_80067870();
    }
    func_800578F4(0);
    func_80066C08(1);
}

s32 func_80060EA0(void) {
    s32 pad[2]; /* FAKE: unused, gives the 0x48 frame and the n/tbl slots. T-7020 */
    u8 tbl[12];
    s32 i;
    s32 n;

    n = 0;
    get_h_yuukou_table(tbl);
    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 0; i < 11; i++) if (tbl[i] >= 0x4DU && D_800E6280.unk_1BC[i].unk_0C.f.b1) n++;
    return n;
}

void func_80061044(void) {
    if (func_80060EA0() != 0) {
        set_dec_bri(0);
        func_8004E58C();
        func_80057640(0xA0);
        func_80048E78();
        func_80041584();
        hizuke_init();
        message_window_init();
        parameter_disp_switch(0);
        func_8006764C(0);
        func_80044750(0xC1);
        icon_disp_switch(0);
        tpage_buf_clear();
        func_8006BC28(0);
        func_80044890(1, 0xC4E5, 0xC4C7, 0xD294, 0xD264, 0xD25E);
        func_8004284C();
        return;
    }
    set_dec_bri(0);
    main_state_set(0x53);
}

void pre_syogatu_init1(void) {
    s32 t;

    if (func_80044E8C() == 1) {
        t = dec_bg_cd_read(0x4167, 1);
        if (t != -1) {
            if (t == 0 || t == 1) {
                dec_bg_show_set(1, t);
                set_dec_bri(0x80);
                func_8004284C();
                func_8004284C();
            }
        } else {
            func_8004284C();
        }
    }
    func_80057710();
}

void pre_syogatu_init2(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(1, func_8005751C(1));
        set_dec_bri(0x80);
        func_8004284C();
    }
}

INCLUDE_RODATA("asm/data/main/8005A0B0.rodata", D_800B0B64);

INCLUDE_RODATA("asm/data/main/8005A0B0.rodata", D_800B0B6C);

void pre_syogatu_init3(void) {
    func_8004E58C();
    func_8006612C(D_800B0B64);
    k_sub_reset_point_set();
    sndisp(D_800B0B6C, 0, 0xFF);
    func_80044750(0x603);
    k_disp_start(6);
    func_80042808();
}

s32 pre_syogatu_init(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80061044();
        break;
    case 1:
        pre_syogatu_init1();
        break;
    case 2:
        pre_syogatu_init2();
        break;
    case 3:
        pre_syogatu_init3();
        break;
    }
}

void pre_syogatu0(void) {
    s32 temp_v0;

    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(&D_800E6280.unk_0D4[0]);
        sndisp((s32) "ん？ 誰だろう）", 0, 0x1F);
        D_800E6280.unk_110D += 1;
    default:
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        k_sub_reset();
        gnsx("〒配達");
        sndisp((s32) "年賀状でーす。", 0, 0xE);
        k_sub_disp_start(1);
        func_80052060(0x08000005, 0x03000009, -1, 0xE);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        func_8005215C();
        break;
    case 4:
        temp_v0 = dec_bg_cd_read(0x4144, 1);
        if (temp_v0 != -1) {
            if ((temp_v0 == 0) || (temp_v0 == 1)) {
                dec_bg_show_set(1, temp_v0);
                set_dec_bri(0x80U);
                D_800E6280.unk_110D = 6;
            }
        } else {
            D_800E6280.unk_110D = 5;
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            set_dec_bri(0x80U);
            D_800E6280.unk_110D += 1;
        }
        break;
    case 6:
        func_8004284C();
        break;
    }
}

/* bulk_m2c declarations (T-11190) */
extern u8 D_800E6354;
extern u8 D_800E738D;
/* end bulk_m2c declarations */
void pre_syogatu1(void) {
    u8 sp16C;
    u8 sp6C;
    u8 sp5C;
    s32 var_s0;
    s32 var_s1;
    u8 *var_s2;

    switch (D_800E738D) {
    case 0:
        get_h_yuukou_table((s32) &sp16C);
        k_sub_reset();
        strcpy(&sp6C, "やった！ ");
        var_s1 = 0;
        var_s0 = 0;
        var_s2 = &sp16C;
        do {
            if (((u8) *var_s2 >= 0x4DU) && (D_800E6280.unk_1BC[var_s0].unk_0C.w & 2)) {
                if (var_s1 != 0) {
                    strcat(&sp6C, "と、 ");
                }
                get_g_name(&sp5C, var_s0);
                strcat(&sp6C, &sp5C);
                var_s1 += 1;
            }
            var_s0 += 1;
            var_s2 += 1;
        } while (var_s0 != 0xB);
        strcat(&sp6C, "から、 年賀状をもらったラッキー。）");
        gnsx(&D_800E6354);
        sndisp((s32) &sp6C, 0, 0x1F);
        D_800E738D += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        main_state_set(0x53);
        return;
    }
}

s32 func_80061634(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        pre_syogatu0();
        break;
    case 1:
        pre_syogatu1();
        break;
    }
}

void func_80061688(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        pre_syogatu_init();
        break;
    case 1:
        func_80061634();
        break;
    }
    hizuke_show();
    message_window_show();
    func_8006BA40();
    func_80066334();
    func_800578F4(1);
    func_80066C08(1);
}