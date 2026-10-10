#include "ovl/OMIMAI.h"

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_801331B0);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_80133424);

void func_801334AC(void) {
    if ((D_800E71DF == 4) && (D_800CA148 == 5)) {
        normal_date_speak_1line();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_80133500);

void func_801335C4(void) {
    func_80044890(1, 0xBF98, 0xBF79, D_800B3688[D_800E71DF], D_800B36C8[D_800E71DF], D_800B3708[D_800E71DF]);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_8013364C);

void func_801336CC(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_801336F4(void) {
    func_80046318(6, 0x80197000, 0xAF33);
    func_801331B0();
    func_8004284C();
}

void func_80133730(void) {
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    k_reset(1);
    D_800E7322 = 0;
    D_800E7368 = 1;
    func_8008585C();
    D_800E62BA = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    hizuke_init();
    message_window_init();
    addr_init_bustup();
    func_80084E4C();
    func_801337EC();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_801337EC);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_8013388C);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_80134030);

void func_801340E4(void) {
    switch (D_800E71DF) {
    case 0:
        D_800CA160 = D_80134C30[1];
        D_800CA164 = D_80134C64[1];
        D_800CA168 = D_80134C98[1];
        return;
    case 1:
        D_800CA160 = D_80134C30[6];
        D_800CA164 = D_80134C64[6];
        D_800CA168 = D_80134C98[6];
        return;
    case 2:
        D_800CA160 = D_80134C30[2];
        D_800CA164 = D_80134C64[2];
        D_800CA168 = D_80134C98[2];
        return;
    case 3:
        D_800CA160 = D_80134C30[4];
        D_800CA164 = D_80134C64[4];
        D_800CA168 = D_80134C98[4];
        return;
    case 4:
        D_800CA160 = D_80134C30[10];
        D_800CA164 = D_80134C64[10];
        D_800CA168 = D_80134C98[10];
        return;
    case 5:
        D_800CA160 = D_80134C30[7];
        D_800CA164 = D_80134C64[7];
        D_800CA168 = D_80134C98[7];
        return;
    case 6:
        D_800CA160 = D_80134C30[8];
        D_800CA164 = D_80134C64[8];
        D_800CA168 = D_80134C98[8];
        return;
    case 7:
        D_800CA160 = D_80134C30[5];
        D_800CA164 = D_80134C64[5];
        D_800CA168 = D_80134C98[5];
        return;
    case 8:
        D_800CA160 = D_80134C30[0];
        D_800CA164 = D_80134C64[0];
        D_800CA168 = D_80134C98[0];
        return;
    case 9:
        D_800CA160 = D_80134C30[9];
        D_800CA164 = D_80134C64[9];
        D_800CA168 = D_80134C98[9];
        return;
    case 10:
        D_800CA160 = D_80134C30[12];
        D_800CA164 = D_80134C64[12];
        D_800CA168 = D_80134C98[12];
        return;
    default:
        D_800CA160 = D_80134C30[3];
        D_800CA164 = D_80134C64[3];
        D_800CA168 = D_80134C98[3];
        return;
    }
}

void func_80134384(void) {
    switch (D_800E71E8 & 0xF) {
    case 0:
        func_800AE0F0(D_800CA19C, "図書室");
        bg_read_sub2(0x40BC);
        break;
    case 1:
        func_800AE0F0(D_800CA19C, "教室");
        if (D_800E62BF >= 6U && D_800E62BF < 10U) {
            bg_read_sub2(0x404D);
        } else {
            bg_read_sub2(0x4055);
        }
        break;
    case 2:
        func_800AE0F0(D_800CA19C, "中庭");
        bg_read_sub2(0x4071);
        break;
    case 3:
        func_800AE0F0(D_800CA19C, "自宅前");
        bg_read_sub2(0x414C);
        break;
    case 4:
        switch (D_800E6374 >> 12) {
        case 0:
            func_800AE0F0(D_800CA19C, "図書室");
            bg_read_sub2(0x40BC);
            break;
        case 1:
            func_800AE0F0(D_800CA19C, "演劇部");
            bg_read_sub2(0x40B2);
            break;
        case 2:
            func_800AE0F0(D_800CA19C, "実験室");
            bg_read_sub2(0x40C6);
            break;
        case 3:
            func_800AE0F0(D_800CA19C, "電脳部室");
            bg_read_sub2(0x40D0);
            break;
        case 4:
            func_800AE0F0(D_800CA19C, "美術室");
            bg_read_sub2(0x40D9);
            break;
        case 5:
            func_800AE0F0(D_800CA19C, "音楽室");
            bg_read_sub2(0x40E2);
            break;
        case 6:
        case 7:
        case 8:
            func_800AE0F0(D_800CA19C, "グランド");
            bg_read_sub2(0x4097);
            break;
        case 9:
            func_800AE0F0(D_800CA19C, "プール");
            bg_read_sub2(0x40A0);
            break;
        default:
            func_800AE0F0(D_800CA19C, "体育館");
            bg_read_sub2(0x40A9);
            break;
        }
        break;
    default:
        func_800AE0F0(D_800CA19C, "廊下");
        bg_read_sub2(0x4045);
        break;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_80134628);
