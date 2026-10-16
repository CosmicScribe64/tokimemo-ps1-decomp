#include "common.h"
#include "ovl/OLH.h"

void func_80132000(void) {
    switch (D_800E6280.unk_1109) {
    case 0x0:
        func_80132114();
        break;
    case 0x1:
        func_80132294();
        break;
    case 0x2:
        func_801323AC();
        break;
    case 0x10:
        func_80132420();
        break;
    case 0x20:
        func_80132FA0();
        break;
    case 0x30:
        func_801364E0();
        break;
    case 0x40:
        func_80133B80();
        break;
    case 0x50:
        func_80134500();
        break;
    case 0x60:
        func_80134F40();
        break;
    case 0x70:
        func_80135AC0();
        break;
    }
    func_8006BA40();
    if (D_800E6280.unk_F88 & 0x800) {
        main_state_set(0x34);
    }
    func_801323CC();
    func_80066334();
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_80139EE0;
extern s32 D_80139F00;
extern s8 D_800E62B6;
extern s8 D_800E62B7;
extern s8 D_800E62B8;
/* end bulk_m2c declarations */
s32 func_80132114(void) {
    s16 spA4;
    s16 sp94;
    s16 sp84;
    s16 sp74;
    s16 *var_s0;
    s16 *var_s1;
    s16 *var_s4;
    s16 *var_s5;
    s16 var_s3;
    s32 *var_s2;

    func_8004E58C();
    func_80048EB8(0);
    back_clear_switch(1);
    D_800E62B7 = 0xA0;
    D_800E62B6 = 0xA0;
    D_800E62B8 = 0xA0;
    func_8006612C((s32) "Ｑ＆Ａコーナー");
    set_kanji_string(-0x68, 0x5C, 2, (s32) "スタートボタンでゲームに戻れます", 0);
    k_sub_reset_point_set();
    var_s2 = &D_80139EE0;
    var_s0 = &spA4;
    var_s1 = &sp94;
    var_s3 = -0x3C;
    var_s4 = &sp84;
    var_s5 = &sp74;
    do {
        *var_s0 = -0x60;
        *var_s1 = var_s3;
        *var_s4 = 0x80;
        *var_s5 = 0x10;
        set_kanji_string(*var_s0, *var_s1, 0xF, *var_s2, 0);
        var_s2 += 4;
        var_s0 += 2;
        var_s1 += 2;
        var_s3 += 0x10;
        var_s4 += 2;
        var_s5 += 2;
    } while (var_s2 != &D_80139F00);
    menu_set(0, 8, &spA4, &sp94, &sp84, &sp74);
    k_disp_start(set_kanji_string(-0x10, -0x50, 0xF, (s32) "項目を選んでください", 0));
    func_80042808();
}

void func_80132294(void) {
    func_8004F984(0, D_8011ECF6, D_8011ECFA);
    func_80050B54(0);
    func_8004FC10(0);
    if ((D_800E6280.unk_F88 & 0x20) && D_800E6280.unk_1093 != -1) {
        func_80044750(0x501);
        switch (*(u8 *)&D_800E6280.unk_1093) { /* FAKE: u8 view keeps the selector load as lbu, as in the original (T-5010) */
        case 0:
            func_80042908(0x10);
            break;
        case 1:
            func_80042908(0x20);
            break;
        case 2:
            func_80042908(0x30);
            break;
        case 3:
            func_80042908(0x40);
            break;
        case 4:
            func_80042908(0x50);
            break;
        case 5:
            func_80042908(0x60);
            break;
        case 6:
            func_80042908(0x70);
            break;
        case 7:
            func_80042908(2);
            break;
        }
    }
}

void func_801323AC(void) {
    main_state_set(0x34);
}

void func_801323CC(void) {
    func_80049A40(-0x90, -0x58, 0x120, 0xC4, 4, 0xA0A0A0, 2);
    dtd_on(4);
}
