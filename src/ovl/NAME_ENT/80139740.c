#define MAIN_API_OVERRIDE_func_80049A40 /* Original calls pass promoted integer coordinates. T-10880. */
#include "common.h"
#include "ovl/NAME_ENT.h"

void func_80049A40();

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80139740);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_801398EC);

void func_80139AD8(s32 arg0, s32 arg1, s32 arg2) {
    u8 *p;

    p = (u8 *)D_801217D0 + arg0 * 36;
    p[0x14] = 0x80;
    p[0x15] = 0x80;
    p[0x16] = 0x80;
    *(s16 *)(p + 4) = arg1;
    *(s16 *)(p + 6) = arg2;
    *(s32 *)p = 0;
    *(s16 *)(p + 8) = 0x10;
    *(s16 *)(p + 0xA) = 0x10;
    p[0xE] = arg0 / 8 * 0x30 + D_800E6280.unk_10F8 / 32 % 3 * 0x10 + 0x80;
    p[0xF] = (arg0 & 7) * 0x10;
    *(s16 *)(p + 0xC) = 0x1F;
    *(s16 *)(p + 0x10) = arg0 * 0x10;
    *(s16 *)(p + 0x18) = 8;
    *(s16 *)(p + 0x1A) = 8;
    *(s16 *)(p + 0x12) = 0x1F8;
    D_800E6280.unk_10A5[arg0] = 2;
}

void func_80139BA0(void) {
    if (D_800E6280.unk_F88 & 0x10) {
        D_8014D0D4 += 1;
        return;
    }
    if ((D_800E6280.unk_F88 & 0x20) && (D_8011ECF6 < 0x20) && (D_8011ECF6 >= -0x3F) && (D_8011ECFA < 0x1C) && (D_8011ECFA >= 0xD)) {
        D_8014D0D4 += 1;
    }
}

void func_80139C24(void) {
    if (D_8014D0D4 < 0) {
        D_8014D0D4 += 3;
    }
    switch (D_8014D0D4 % 3) {
    case 0:
        func_80139F30();
        break;
    case 1:
        func_8013ACA4();
        break;
    case 2:
        func_8013A9C8();
        break;
    }
    func_80139BA0();
}

void func_80139CB4(void) {
    u8 sp30[0x108]; /* FAKE: observed frame and buffer slot, T-9580. */

    if (D_800E8BEE != 0) {
        func_800AE090(sp30, D_800B3288[D_800E7D36], 5);
        func_800AE100(sp30, "番");
        func_8004E788(-0x20, -0x5C, 0xF, (s32) sp30, 0);
        func_8004E788(0x20, -0x4B, 0xF, (s32) ((NameMetricWork *)&D_800E7D10)->family_name[D_800E7D36], 0);
        func_8004E788(0x4E, -0x4B, 0xF, (s32) ((NameMetricWork *)&D_800E7D10)->given_name[D_800E7D36], 0);
        func_8004E788(0x20, -0x3B, 0xF, (s32) ((NameMetricWork *)&D_800E7D10)->nickname[D_800E7D36], 0);
        func_8004E788(0x20, -0x2B, 0xF, (s32) D_800B3908[((NameMetricWork *)&D_800E7D10)->club[D_800E7D36]], 0);
        return;
    }
    func_800AE090(sp30, D_800B3288[D_800E6280.unk_1118], 5);
    func_800AE100(sp30, "番");
    func_8004E788(-0x20, -0x5C, 0xF, (s32) sp30, 0);
    func_8004E788(0x20, -0x4B, 0xF, (s32) ((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].family_name, 0);
    func_8004E788(0x4E, -0x4B, 0xF, (s32) ((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].given_name, 0);
    func_8004E788(0x20, -0x3B, 0xF, (s32) ((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].nickname, 0);
    func_8004E788(0x20, -0x2B, 0xF, (s32) D_800B3908[((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].club >> 0xC], 0);
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80139F30);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013A9C8);

void func_8013ACA4(void) {
    if (D_800E8BEE != 0) {
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[0][D_800E7D36].unk_02, 16, 30);
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[1][D_800E7D36].unk_02, 16, 46);
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[2][D_800E7D36].unk_02, 16, 62);
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[3][D_800E7D36].unk_02, 16, 78);
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[4][D_800E7D36].unk_02, 128, 14);
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[5][D_800E7D36].unk_02, 128, 30);
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[6][D_800E7D36].unk_02, 128, 46);
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[7][D_800E7D36].unk_02, 128, 62);
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[8][D_800E7D36].unk_02, 128, 78);
        return;
    }
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[0].unk_02, 16, 30);
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[1].unk_02, 16, 46);
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[2].unk_02, 16, 62);
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[3].unk_02, 16, 78);
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[4].unk_02, 128, 14);
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[5].unk_02, 128, 30);
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[6].unk_02, 128, 46);
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[7].unk_02, 128, 62);
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[8].unk_02, 128, 78);
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013AF78);

s32 func_8013B0A0(void) {
    s32 a;
    s32 b;
    s32 c;
    s32 d;

    a = 0;
    b = 0;
    if (D_800E6280.unk_1109 == 0) {
        return 0;
    }
    c = 0;
    d = 0;
    if (D_800E6280.unk_110A < 0xA0U) {
        a = 1;
    } else if (D_800E6280.unk_110A < 0xC0U) {
        b = 1;
    } else if (D_800E6280.unk_110A < 0xE0U) {
        c = 1;
    } else {
        d = 1;
    }
    if (a != 0) {
        D_80120650[0xCF] |= 0x80;
    } else {
        D_80120650[0xCF] &= ~0x80;
    }
    if (b != 0) {
        D_80120650[0x47] |= 0x80;
    } else {
        D_80120650[0x47] &= ~0x80;
    }
    if (c != 0) {
        D_80120650[3] |= 0x80;
    } else {
        D_80120650[3] &= ~0x80;
    }
    if (d != 0) {
        D_80120650[0x8B] |= 0x80;
    } else {
        D_80120650[0x8B] &= ~0x80;
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013B1D8);

void func_8013B51C(s32 arg0) {
    s16 x;
    s16 y;

    x = (arg0 / 7) * 0x1F - 0x82;
    *(s16 *)&D_8011ECD0[0x26] = x;
    y = (arg0 % 7) * 0x10 - 0xB;
    *(s16 *)&D_8011ECD0[0x2A] = y;
    D_8011ECD0[0x1F9F] |= 0x80;
    *(s16 *)&D_8011ECD0[0x1FC2] = x;
    *(s16 *)&D_8011ECD0[0x1FC6] = y;
}

void func_8013B59C(void) {
    func_8004E58C();
    func_80139CB4();
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    D_8014D0D4 = 1;
    func_8013B51C(D_800E6280.unk_1118 - 1);
}

void func_8013B5F0(void) {
    if ((D_800E6280.unk_1109 != 0 || D_800E6280.unk_110A != 0) && D_8014D0E0 < 0x4001) {
        D_8014D0E0 = (D_8014D0E0 * 16 + 0x70) / 15;
        D_8014D0E4 += D_8014D0EC;
        D_8014D0E8 += D_8014D0F0;
        if (D_8014D0E4 >= 0xA1) {
            D_8014D0EC = -(func_800AE0D0() & 0xF);
        }
        if (D_8014D0E8 >= 0x79) {
            D_8014D0F0 = -(func_800AE0D0() & 0xF);
        }
        if (D_8014D0E4 < -0xA0) {
            D_8014D0EC = func_800AE0D0() & 0xF;
        }
        if (D_8014D0E8 < -0x78) {
            D_8014D0F0 = func_800AE0D0() & 0xF;
        }
    }
    func_80046AC8(0x1A, 0, 0x1F7, D_8014D0E0, D_8014D0E4, D_8014D0E8, 0x01DF7FCF, 2);
}

void func_8013B77C(void) {
    if (D_8014D0AC != 0) {
        func_800AE0B0("option  kidou back ");
        if (D_8014D0AC & 0x80) {
            k_speed_set(D_8014D0B0);
        }
        func_800AE0B0("kanji_speed%d \n", D_8014D0B0);
        if (D_8014D0B4 >= 3) {
            D_800E6280.unk_F6C = D_8014D0B4 % 3;
        }
        func_800AE0B0("icon_mode %d \n", D_8014D0B4);
        if (D_8014D0B8 >= 4) {
            D_800E6280.unk_1092 = D_8014D0B8 & 3;
        }
        func_800AE0B0("menu_mode %d \n", D_8014D0B8);
        if (D_8014D0AC & 8) {
            D_800E6280.unk_F6F = D_8014D0BC;
        }
        func_800AE0B0("cursor_speed %d \n", D_8014D0BC);
        if (D_8014D0C0 >= 3) {
            D_800E6280.unk_03C = D_8014D0C0 % 3;
        }
        func_800AE0B0("sound_mode %d \n", D_8014D0C0);
        if (D_8014D0AC & 0x40) {
            D_800E6280.unk_F70 = D_8014D0C4;
        }
        func_800AE0B0("click_mode %d \n", D_8014D0C4);
        if (D_8014D0AC & 0x20) {
            D_800E6280.unk_F73 = D_8014D0C8;
        }
        func_800AE0B0("click_speed %d \n", D_8014D0C8);
        if (D_8014D0AC & 0x10) {
            D_800E6280.unk_F75 = D_8014D0CC;
        }
        func_800AE0B0("mouse_ok %d \n", D_8014D0CC);
        if (*(u8 *)&D_800E6280.unk_F6C == 2) {
            D_800E6280.unk_F6D = 0xF;
        } else {
            D_800E6280.unk_F6D = 0xE;
        }
        func_8006509C();
        func_80064FA4(0);
    }
    D_8014D0A8 = 0;
    func_8013BB1C();
    func_8013B994();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013B994);

void func_8013BB10(void) {
    D_8014D0A8 = 0;
}

void func_8013BB1C(void) {
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013BB24);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013BE18);

void func_8013BF74(void) {
    s32 i;
    s32 column, row;
    s16 x[16], y[16], width[16], height[16];
    s16 x2[4], y2[4], width2[4], height2[4];
    s32 addresses[6], addresses2[6];
    DisplayRecord44 *record;

    for (i = 0; i < 6; i++) {
        addresses[i] = 0x80182000 + i * 0x4000;
        addresses2[i] = 0x801A4000 + i * 0x4000;
    }
    for (i = 0; i < 14; i++) {
        column = i / 7;
        row = i % 7;
        x[i] = column * 32 - 144;
        y[i] = row * 16 - 18;
        width[i] = 32;
        height[i] = 16;
    }
    func_8004F870(0, 14, x, y, width, height);
    for (i = 0; i < 4; i++) {
        column = i & 1;
        row = i & 2;
        x2[i] = column * 44 - 144;
        y2[i] = row * 8 - 96;
        width2[i] = 40;
        height2[i] = 20;
    }
    func_8004F870(1, 4, x2, y2, width2, height2);
    D_800E6280.unk_1093 = 0;
    D_800E6280.unk_1094 = -1;
    D_8014D0D0 = 0;
    D_8014D0D4 = 1;
    D_8014D0D8 = 0;
    D_8014D0DC = 0;
    record = (DisplayRecord44 *)D_80120650;
    for (i = 0; i < 45; i++) {
        record[i].unk_18 = i;
        record[i].unk_01 = 5;
        record[i].unk_02 = 64;
        record[i].unk_38 = 0x01000000;
        record[i].unk_03 = 36;
        record[i].unk_0C = (u8 *)0x801C048C;
        record[i].unk_10 = (u8 *)0x801C0540;
        record[i].unk_34 = (u8 *)0x801C047C;
        record[i].unk_04 = 0;
        record[i].unk_3C[7] = 0;
        record[i].unk_05 = 128;
        record[i].unk_26 = -160;
        record[i].unk_2A = -120;
        record[i].unk_07 = 128;
    }
    for (i = 0; i < 11; i++) {
        record[i + 45].unk_01 = 4;
        record[i + 45].unk_02 = 64;
        record[i + 45].unk_38 = 0x40000000;
        record[i + 45].unk_03 = 36;
        record[i + 45].unk_0C = D_800C975C;
        record[i + 45].unk_10 = D_800C9A14;
        record[i + 45].unk_34 = D_800C9730;
        record[i + 45].unk_04 = 8;
        record[i + 45].unk_18 = 89;
        record[i + 45].unk_3C[7] = 0;
        record[i + 45].unk_05 = 1;
        record[i + 45].unk_26 = i / 4 * 65 - 15;
        record[i + 45].unk_2A = i % 4 * 16 + 36;
    }
    load_tpage_buf_lock(5, ((s32 *)D_80120684)[0], 1);
    load_tpage_buf_lock(6, ((s32 *)D_80120684)[1], 1);
    load_tpage_buf_lock(7, ((s32 *)D_80120684)[2], 1);
    load_tpage_buf_lock(8, ((s32 *)D_80120684)[3], 1);
    ((DisplayRecord44 *)D_8011ECD0)[114].unk_03 = 0;
    ((DisplayRecord44 *)D_8011ECD0)[100].unk_03 |= 0x80;
    ((DisplayRecord44 *)D_8011ECD0)[101].unk_03 |= 0x80;
    ((DisplayRecord44 *)D_8011ECD0)[102].unk_03 |= 0x80;
    record = &((DisplayRecord44 *)D_8011ECD0)[103];
    for (i = 0; i < 12; i++) {
        record[i].unk_03 |= 128;
    }
    if (D_800E8BEE != 0) {
        ((DisplayRecord44 *)D_8011ECD0)[126].unk_3C[7] = 0x11;
    } else {
        ((DisplayRecord44 *)D_8011ECD0)[127].unk_3C[7] = 0x11;
    }
    ((DisplayRecord44 *)D_8011ECD0)[126].unk_03 |= 0x80;
    ((DisplayRecord44 *)D_8011ECD0)[127].unk_03 |= 0x80;
    func_80043914(0x801A2000, 0x10, 1, 1, 0);
    func_80043914(0x801A2000, 0x11, 1, 1, 1);
    func_80043914(0x80180000, 0x1F, 1, 1, 0);
    func_8004A1A0((s32) addresses, 0, 6, 3, 2, 0x1FF, -0xA0, -0x78, 0xA, 0xD);
    func_8004A1A0((s32) addresses2, 1, 6, 3, 2, 0x1F0, -0xA0, -0x78, 8, 0xA);
    func_80048F64(0x72);
    func_80042808();
}

void func_8013C4A8(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013BB24();
        return;
    case 1:
        func_8013BE18();
        return;
    case 2:
        func_8013BF74();
        return;
    }
}
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B7B4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B7C8);

void func_8013C510(void) {
    func_80056284();
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014B7B4, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014B7C8, 0);
    k_disp_start(2);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013C580);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B824);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B838);

void func_8013C664(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014B824, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014B838, 0);
    k_disp_start(2);
    func_8004284C();
}

void func_8013C6CC(void) {
    if (D_800E6280.unk_F88 & 0x860) {
        func_8004284C();
    }
}

void func_8013C6FC(void) {
    func_8004E58C();
    func_8004E788(-0x40, 0x20, 0xF, "新たにゲームを始めますか？", 0);
    func_8004E788(-0x40, 0x30, 0xF, D_800B3C70, 0);
    func_8004E788(0x14, 0x30, 1, func_8006CA9C(), 0);
    func_8004E788(-0x40, 0x40, 0xF, D_800B3C88, 0);
    func_8004E788(0x14, 0x40, 2, func_8006CAE0(), 0);
    func_8004E884(5);
    func_8004284C();
}

void func_8013C7C0(void) {
    if (D_800E6280.unk_F88 & 0x800) {
        func_8013B77C();
        func_80042878(0x14);
    } else if (D_800E6280.unk_F88 & 0x20) {
        func_8013B77C();
        func_80042878(0x14);
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
    }
}

void func_8013C834(void) {
    func_8004E58C();
    func_8004E788(-0x40, 0x20, 0xF, "メモリーカードを", 0);
    func_8004E788(-0x40, 0x30, 1, "差し込み口１", 0);
    func_8004E788(-0x40, 0x30, 0xF, "　　　　　　にセットして、", 0);
    func_8004E788(-0x40, 0x40, 0xF, "　ボタンを押してください。", 0);
    func_8004E788(-0x40, 0x40, 1, func_8006CA9C(), 0);
    func_80053D10();
    func_8004E884(5);
    func_8004284C();
}

void func_8013C8FC(void) {
    if (D_800E6280.unk_F88 & 0x20) {
        func_80056284();
        func_80042940(0);
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013C934);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013C9F0);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013CB44);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013CEC8);

void func_8013D054(void) {
    u8 state;

    switch (D_800E6280.unk_110D) {
    case 0:
        func_80053CE0();
        D_800E6280.unk_110D++;
        break;
    case 1:
        if (func_8005448C() == 4) {
            func_80053D10();
            D_800E6280.unk_110D++;
        } else if ((state = D_800E6280.unk_1115) == 0xF0) {
            func_80056284();
            func_80042908(1);
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (state == 0xD0)) {
            func_80053D10();
            func_80042908(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            D_800E6280.unk_110D++;
        } else {
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042908(0xF0);
        }
        break;
    case 3:
        if (func_80055AFC(0) == 1) {
            func_8013BB10();
            func_800AE0B0("SYS0 DATA OK!");
            func_80053D10();
            func_80044750(0x501);
            func_80053CE0();
            D_800E7D36 = D_800E6280.unk_1118;
            func_80054E6C();
            D_800E6280.unk_110D++;
        } else {
            state = D_800E6280.unk_1115;
            func_800AE0B0("s->mem_stat %d \n", state);
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042908(0xF0);
        }
        break;
    case 4:
        if (func_80054388() == 0x13) {
            func_80053D10();
            func_80042908(3);
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || ((state = D_800E6280.unk_1115) == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    }
    func_8013B1D8();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013D2AC);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013D4CC);

void func_8013D62C(void) {
    s32 pad; /* FAKE: unused, declared before buf for its stack slot (T-3330) */
    u8 buf[0x100];

    func_8004E58C();
    func_800AE0F0(buf, D_800B3288[D_800E6280.unk_1118]);
    strcat(buf, "番アルバムを");
    func_8004E788(-0x40, 0x20, 0xF, buf, 0);
    func_8004E788(-0x40, 0x30, 0xF, "ロードしています。", 0);
    func_8004E884(2);
    func_8004284C();
}

/* Original implicit-int dispatch falls through without a return statement. */
s32 func_8013D6C0(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80053CE0();
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_8005448C() == 4) {
            D_800E6280.unk_110D += 1;
        } else if (D_800E6280.unk_1115 == 0xF0) {
            func_80056284();
            func_80042908(1);
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xD0)) {
            func_80053D10();
            func_80042908(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            func_8013B59C();
            func_80042940(0x21);
        } else {
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042908(0xFF);
        }
        break;
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B998);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B9A4);

void func_8013D7F4(void) {
    if (func_80055AFC(0) == 1) {
        func_8013BB10();
        func_800AE0B0(D_8014B998);
        func_80053D10();
        func_80044750(0x501);
        func_80042908(3);
    } else {
        func_800AE0B0(D_8014B9A4);
        func_80056284();
        func_80042908(1);
    }
    func_8013B1D8();
}

void func_8013D874(void) {
    func_8004E58C();
    func_8004E788(-0x40, 0x20, 0xF, "このゲームのデータがありません", 0);
    func_8004E788(-0x40, 0x30, 0xF, "新たにアルバムを作成し、", 0);
    func_8004E788(-0x40, 0x40, 0xF, "ゲームをプレイしますか？", 0);
    func_8004E788(-0x40, 0x50, 0xF, "　　はい…　　いいえ…　", 0);
    func_8004E788(6, 0x50, 1, func_8006CA9C(), 0);
    func_8004E788(0x5A, 0x50, 2, func_8006CAE0(), 0);
    func_8004E884(6);
    func_8004284C();
}

void func_8013D954(void) {
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0x44);
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BA28);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BA44);

void func_8013D9A0(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BA28, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BA44, 0);
    k_disp_start(2);
    func_8004284C();
}

void func_8013DA08(void) {
    if (D_800E6280.unk_F88 & 0x820) {
        func_8013B77C();
        func_80042878(0x14);
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BA60);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BA78);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BA88);

void func_8013DA40(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BA60, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BA78, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BA88, 0);
    k_disp_start(3);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013DAC4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BAA4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BAC4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BAE0);

void func_8013DBD0(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BAA4, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BAC4, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BAE0, 0);
    k_disp_start(3);
    func_8004284C();
}

void func_8013DC54(void) {
    if (D_800E6280.unk_F88 & 0x860) {
        func_8004284C();
    }
}

void func_8013DC84(void) {
    func_8004E58C();
    func_8004E788(-0x40, 0x20, 0xF, "システムファイルを作成しますか", 0);
    func_8004E788(-0x40, 0x30, 0xF, D_800B3C70, 0);
    func_8004E788(0x14, 0x30, 1, func_8006CA9C(), 0);
    func_8004E788(-0x40, 0x40, 0xF, D_800B3C88, 0);
    func_8004E788(0x14, 0x40, 2, func_8006CAE0(), 0);
    func_8004E884(5);
    func_8004284C();
}

void func_8013DD48(void) {
    if (D_800E6280.unk_F88 & 0x820) {
        func_80042940(0x4C);
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
    }
}

s32 func_8013DD94(void) {
    u8 sp38[0x108]; /* FAKE: observed frame and buffer slot, T-9580. */
    u8 temp_v0;

    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004E58C();
        func_800AE0F0(sp38, D_800B32CC[D_800E6280.unk_1118 - 1]);
        func_800AE100(sp38, "番アルバムを作成しています。");
        func_8004E788(-0x40, 0x20, 0xF, (s32) sp38, 0);
        func_8004E884(2);
        temp_v0 = D_800E7D14[D_800E6280.unk_1118];
        if (temp_v0 == 0xFF) {
            D_800E6280.unk_110D += 1;
        } else if (temp_v0 == 1) {
            D_800E6280.unk_110D += 1;
        } else if (func_80054694((s32) D_800E6280.unk_1118) == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            func_80042908(4);
        }
        break;
    case 1:
        func_80053CE0();
        func_80041878();
        func_80042058();
        D_800E6280.unk_110D += 1;
        break;
    case 2:
        if (func_80054590() == 0x14) {
            D_800E6280.unk_110D += 1;
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 3:
        func_80053D10();
        func_8013B77C();
        func_80042878(0x14);
        break;
    }
}


/* NON_MATCHING: T-10880, 11 file-index register substitutions. */
#ifdef NON_MATCHING
void func_8013DF88(void) {
    s32 result;
    u8 state;
    s32 i;

    u8 text[256];
    state = D_800E6280.unk_110D;
    switch (state) {
    case 0:
        func_8004E58C();
        func_800AE0F0(text, D_800B32CC[D_800E6280.unk_1118 - 1]);
        strcat(text, "番アルバム　と、");
        func_8004E788(-0x40, 0x20, 0xF, (s32) text, 0);
        func_8004E788(-0x40, 0x30, 0xF, (s32) "システムファイルを", 0);
        func_8004E788(-0x40, 0x40, 0xF, (s32) "作成しています。", 0);
        func_8004E884(3);
        D_800E6280.unk_110D += 1;
    default:
        break;
    case 1:
        if ((result = func_80054694((s32) D_800E6280.unk_1118)) == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            func_80042908(4);
        }
        break;
    case 2:
        if ((result = func_80054694(0)) == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            func_80054704((s32) D_800E6280.unk_1118);
            func_80042908(4);
        }
        break;
    case 3:
        func_80053D10();
        ((NameMetricWork *)&D_800E7D10)->selected_slot = D_800E6280.unk_1118;
        i = 0;
        while (i < 15) {
            ((NameMetricWork *)&D_800E7D10)->slot_status[i] = 0;
            ++i;
        }
        D_800E7D14[D_800E6280.unk_1118] = 1;
        *D_800E7D14 = 1;
        D_800E6280.unk_1118 = 0;
        func_80053CE0();
        func_80054E6C();
        D_800E6280.unk_110D += 1;
        break;
    case 4:
        if ((result = func_80054388()) == 0x13) {
            func_80053D10();
            D_800E6280.unk_110D += 1;
        } else if (((u32) (*(u32 *)&D_800E6280.unk_1120) >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 5:
        func_80053CE0();
        func_80041878();
        func_80042058();
        func_80042058();
        D_800E6280.unk_1118 = ((NameMetricWork *)&D_800E7D10)->selected_slot;
        func_800AE0B0("s->file_no %x \n", ((NameMetricWork *)&D_800E7D10)->selected_slot);
        D_800E6280.unk_110D += 1;
        break;
    case 6:
        if ((result = func_80054590()) == 0x14) {
            D_800E7D14[D_800E6280.unk_1118] = 1;
            D_800E6280.unk_110D += 1;
        } else if (((u32) (*(u32 *)&D_800E6280.unk_1120) >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 7:
        func_80053D10();
        func_8013B77C();
        func_80042878(0x14);
        break;
    }
}
#else
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013DF88);
#endif

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BB8C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BBA8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BBC0);

void func_8013E30C(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BB8C, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BBA8, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BBC0, 0);
    k_disp_start(3);
    func_8004284C();
}

void func_8013E390(void) {
    if (D_800E6280.unk_F88 & 0x860) {
        func_8004284C();
    }
    func_80142880();
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BBD4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BBEC);

void func_8013E3C8(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BBD4, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BBEC, 0);
    k_disp_start(2);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013E430);

void func_8013E59C(void) {
    s32 pad; /* FAKE: unused, declared before buf for its stack slot (T-3330) */
    u8 buf[0x100];

    func_8004E58C();
    func_800AE0F0(buf, D_800B3288[D_800E6280.unk_1118]);
    strcat(buf, "番アルバムを");
    func_8004E788(-0x40, 0x20, 0xF, buf, 0);
    func_8004E788(-0x40, 0x30, 0xF, "やりなおします。", 0);
    func_8004E788(-0x40, 0x40, 0xF, D_800B3C70, 0);
    func_8004E788(0x14, 0x40, 1, func_8006CA9C(), 0);
    func_8004E788(-0x40, 0x50, 0xF, D_800B3C88, 0);
    func_8004E788(0x14, 0x50, 2, func_8006CAE0(), 0);
    func_8004E884(6);
    func_8004284C();
}

void func_8013E6A8(void) {
    if (D_800E6280.unk_F88 & 0x40) {
        func_80044750(0x501);
        func_80042940(0);
    } else if (D_800E6280.unk_F88 & 0x20) {
        if (D_800E8BEE) {
            func_80042940(0xA6);
        } else {
            func_80042940(0xA7);
        }
    }
}

/* Packed date word: day in bits 0-3, month in bits 4-8, week in bits 9-10. */
typedef struct NameDate {
    u32 day : 4;
    u32 month : 5;
    u32 week : 2;
    u32 rest : 21;
} NameDate;

void func_8013E71C(void) {
    func_800AE0A0(D_8014D070, &D_800E6280.unk_0D4, 8);
    func_800AE0A0(D_8014D078, &D_800E6280.unk_0DC, 8);
    func_800AE0A0(D_8014D080, &D_800E6280.unk_0E4, 0xC);
    D_8014D08C = ((NameDate *)&D_800E6280.unk_0F8)->day - 1;
    D_8014D090 = ((NameDate *)&D_800E6280.unk_0F8)->month - 1;
    D_8014D094 = (((NameDate *)&D_800E6280.unk_0F8)->week + 3) & 3;
    D_8014D098 = ((NameDate *)&D_800E6280.unk_1BC[0].unk_10.w)->day - 1;
    D_8014D09C = ((NameDate *)&D_800E6280.unk_1BC[0].unk_10.w)->month - 1;
    D_8014D0A0 = (((NameDate *)&D_800E6280.unk_1BC[0].unk_10.w)->week + 3) & 3;
    D_8014D0A4 = 1;
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013E80C);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013E9FC);

void func_8013EBEC(void) {
    u8 state;
    s32 result;
    u8 text[256];

    state = D_800E6280.unk_110D;
    switch (state) {
    case 0:
        func_8004E58C();
        func_800AE0F0(text, D_800B32CC[D_800E6280.unk_1118 - 1]);
        strcat(text, "番アルバム　を、");
        func_8004E788(-0x40, 0x20, 0xF, (s32) text, 0);
        func_8004E788(-0x40, 0x30, 0xF, (s32) "作成しています。", 0);
        func_8004E884(2);
        D_800E6280.unk_110D += 1;
    default:
        break;
    case 1:
        if ((result = func_80054694((s32) D_800E6280.unk_1118)) == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            func_80042908(4);
        }
        break;
    case 2:
        func_80053CE0();
        func_80041878();
        func_80042058();
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        if ((result = func_80054590()) == 0x14) {
            D_800E7D14[D_800E6280.unk_1118] = 1;
            D_800E6280.unk_110D += 1;
        } else if (((u32) (*(u32 *)&D_800E6280.unk_1120) >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 4:
        func_80053D10();
        func_80053CE0();
        D_800E7D36 = D_800E6280.unk_1118;
        func_80054E6C();
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        if ((result = func_80054388()) == 0x13) {
            func_80053D10();
            D_800E6280.unk_110D += 1;
        } else if (((u32) (*(u32 *)&D_800E6280.unk_1120) >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 6:
        func_8013B77C();
        func_80042878(0x14);
        break;
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013EE7C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BCB4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BCCC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BCE0);

void func_8013F058(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BCB4, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BCCC, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BCE0, 0);
    k_disp_start(3);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013F0DC);

/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* NON_MATCHING: T-11010, frame is 8 bytes larger than the original (buffer at sp+0x38 instead of 0x30) once the jump-table switch holds the calls; rest matches, 6 lines */
#ifdef NON_MATCHING
/* Falls through without a return statement. */
s32 func_8013F208(void) {
    u8 sp30[0x108]; /* FAKE: observed frame and buffer slot, T-9580. */

    switch (D_800E6280.unk_110D) {
    case 0:
        func_80053CE0();
        D_800E6280.unk_110D++;
    default:
        break;
    case 1:
        if (func_8005448C() == 4) {
            D_800E6280.unk_110D++;
        } else if (D_800E6280.unk_1115 == 0xF0) {
            func_80056284();
            func_80042908(1);
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xD0)) {
            func_80053D10();
            func_80042908(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            func_8013B59C();
            D_800E6280.unk_110D++;
        } else {
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042908(0xFF);
        }
        break;
    case 3:
        if (func_80055AFC(0) == 1) {
            func_8013BB10();
            D_8014D0A8 = 1;
            func_800AE0B0("COPY LOAD SUM !\n");
            D_800E6280.unk_110D++;
        } else {
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042908(0xFF);
        }
        break;
    case 4:
        func_8004E58C();
        func_800AE0F0(sp30, D_800B3288[D_800E6280.unk_1118]);
        func_800AE100(sp30, "番アルバムを");
        func_8004E788(-0x40, 0x20, 0xF, sp30, 0);
        func_8004E788(-0x40, 0x30, 0xF, "どのアルバムにコピーしますか？", 0);
        func_8004E788(-0x40, 0x40, 0xF, "カーソルで選んで下さい。", 0);
        func_8004E884(3);
        func_8004284C();
        break;
    }
}
#else
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013F208);
#endif


INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013F454);

void func_8013F5B0(void) {
    u8 buf[0x108]; /* FAKE: 8 bytes more than the neighbours give the original frame (0x140). T-9170 */

    func_8004E58C();
    func_800AE0F0(buf, D_800B3288[D_800E6280.unk_1118]);
    strcat(buf, "番アルバムに");
    func_8004E788(-0x40, 0x20, 0xF, buf, 0);
    func_8004E788(-0x40, 0x30, 0xF, "上書きしてもいいですか？", 0);
    func_8004E788(-0x40, 0x40, 0xF, D_800B3C70, 0);
    func_8004E788(0x14, 0x40, 1, func_8006CA9C(), 0);
    func_8004E788(-0x40, 0x50, 0xF, D_800B3C88, 0);
    func_8004E788(0x14, 0x50, 2, func_8006CAE0(), 0);
    func_8004E884(6);
    func_8004284C();
}

void func_8013F6BC(void) {
    if (D_800E6280.unk_F88 & 0x20) {
        func_80044750(0x501);
        func_8004284C();
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
        D_800E6280.unk_110D = 2;
    }
}

void func_8013F71C(void) {
    if (D_800E8BEE) {
        func_80042940(0xC7);
    } else {
        func_80042940(0xC8);
    }
}

s32 func_8013F758(void) {
    u8 sp38[0x108]; /* FAKE: observed frame and buffer slot, T-9580. */

    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004E58C();
        func_800AE0F0(sp38, D_800B3288[D_800E6280.unk_1118]);
        func_800AE100(sp38, "番アルバムに");
        func_8004E788(-0x40, 0x20, 0xF, (s32) sp38, 0);
        func_8004E788(-0x40, 0x30, 0xF, (s32) "コピーします。", 0);
        func_8004E884(2);
        func_80053CE0();
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_80054590() == 0x14) {
            D_800E7D14[D_800E6280.unk_1118] = 1;
            D_800E6280.unk_110D += 1;
            func_80053D10();
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_800AE0B0("NO MEM CARD \n");
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 2:
        func_80053D10();
        D_800E7D36 = D_800E6280.unk_1118;
        D_800E6280.unk_1118 = 0;
        func_80053CE0();
        func_80054E6C();
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        if (func_80054388() == 0x13) {
            func_80053D10();
            func_80042940(0);
            D_800E6280.unk_110D = 2;
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    }
}


s32 func_8013F984(void) {
    u8 sp38[0x108]; /* FAKE: observed frame and buffer slot, T-9580. */

    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004E58C();
        func_800AE0F0(sp38, D_800B3288[D_800E6280.unk_1118]);
        func_800AE100(sp38, "番アルバムに");
        func_8004E788(-0x40, 0x20, 0xF, (s32) sp38, 0);
        func_8004E788(-0x40, 0x30, 0xF, (s32) "コピーします。", 0);
        func_8004E884(2);
        func_80053CE0();
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_80054590() == 0x14) {
            D_800E7D14[D_800E6280.unk_1118] = 1;
            D_800E6280.unk_110D += 1;
            func_80053D10();
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_800AE0B0("NO MEM CARD \n");
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 2:
        func_80042940(0);
        D_800E6280.unk_110D = 2;
        break;
    }
}


INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BDE8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BE00);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BE10);

void func_8013FB08(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BDE8, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BE00, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BE10, 0);
    k_disp_start(3);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013FB8C);

void func_8013FCBC(void) {
    s32 pad; /* FAKE: unused, declared before buf for its stack slot (T-3330) */
    u8 buf[0x100];

    func_8004E58C();
    func_800AE0F0(buf, D_800B3288[D_800E6280.unk_1118]);
    strcat(buf, "番アルバムを");
    func_8004E788(-0x40, 0x20, 0xF, buf, 0);
    func_8004E788(-0x40, 0x30, 0xF, "本当に消去していいですか？", 0);
    func_8004E788(-0x40, 0x40, 0xF, D_800B3C70, 0);
    func_8004E788(0x14, 0x40, 1, func_8006CA9C(), 0);
    func_8004E788(-0x40, 0x50, 0xF, D_800B3C88, 0);
    func_8004E788(0x14, 0x50, 2, func_8006CAE0(), 0);
    func_8004E884(6);
    func_8004284C();
}

void func_8013FDC8(void) {
    if (D_800E6280.unk_F88 & 0x20) {
        func_80044750(0x501);
        if (D_800E8BEE) {
            func_80042940(0xE4);
        } else {
            func_80042940(0xE5);
        }
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
    }
}

void func_8013FE40(void) {
    switch (D_800E6280.unk_110D) {                           /* irregular */
    case 0:
        if (func_80054704((s32) D_800E6280.unk_1118) == 1) {
            func_800AE0B0("DELETE OK");
            D_800E7D14[D_800E6280.unk_1118] = 0;
            D_800E6280.unk_110D += 1;
        } else {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 1:
        func_80053CE0();
        D_800E7D36 = D_800E6280.unk_1118;
        D_800E6280.unk_1118 = 0;
        D_800E6280.unk_110D += 1;
        break;
    case 2:
        if (func_80054388() == 0x13) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    }
}

void func_8013FF98(void) {
    if (D_800E6280.unk_110D == 0) {
        if (func_80054704(D_800E6280.unk_1118) == 1) {
            func_800AE0B0("DELETE OK");
            D_800E7D14[D_800E6280.unk_1118] = 0;
            func_80042940(0);
            D_800E6280.unk_110D = 2;
            return;
        }
        func_80053D10();
        func_80056284();
        func_80042908(1);
    }
}

void func_80140024(void) {
    func_80042940(0);
}

void func_80140044(void) {
    u8 state;
    D_800E6280.unk_1104.w += 1;
    state = D_800E6280.unk_110A;
    switch (state) {
    case 0x0:
        func_8013C9F0();
        return;
    case 0x1:
        func_8013CB44();
        return;
    case 0x2:
        func_8013CEC8();
        return;
    case 0x3:
        func_8013D054();
        return;
    case 0x20:
        func_8013D2AC();
        return;
    case 0x21:
        func_8013D4CC();
        return;
    case 0x22:
        func_8013D62C();
        return;
    case 0x23:
        func_8013D6C0();
        return;
    case 0x24:
        func_8013D7F4();
        return;
    case 0x40:
        func_8013D874();
        return;
    case 0x41:
        func_8013D954();
        return;
    case 0x42:
        func_8013D9A0();
        return;
    case 0x43:
        func_8013DA08();
        return;
    case 0x44:
        func_8013DA40();
        return;
    case 0x45:
        func_8013DAC4();
        return;
    case 0x46:
        func_8013DBD0();
        return;
    case 0x47:
        func_8013DC54();
        return;
    case 0x48:
        func_8013DC84();
        return;
    case 0x49:
        func_8013DD48();
        return;
    case 0x4A:
        func_8013DD94();
        return;
    case 0x4C:
        func_8013DF88();
        return;
    case 0xA0:
        func_8013E30C();
        return;
    case 0xA1:
        func_8013E390();
        return;
    case 0xA2:
        func_8013E3C8();
        return;
    case 0xA3:
        func_8013E430();
        return;
    case 0xA4:
        func_8013E59C();
        return;
    case 0xA5:
        func_8013E6A8();
        return;
    case 0xA6:
        func_8013E80C();
        return;
    case 0xA7:
        func_8013E9FC();
        return;
    case 0xA8:
        func_8013EBEC();
        return;
    case 0xA9:
        func_8013EE7C();
        return;
    case 0xC0:
        func_8013F058();
        return;
    case 0xC1:
        func_8013F0DC();
        return;
    case 0xC2:
        func_8013F208();
        return;
    case 0xC3:
        func_8013F454();
        return;
    case 0xC4:
        func_8013F5B0();
        return;
    case 0xC5:
        func_8013F6BC();
        return;
    case 0xC6:
        func_8013F71C();
        return;
    case 0xC7:
        func_8013F758();
        return;
    case 0xC8:
        func_8013F984();
        return;
    case 0xE0:
        func_8013FB08();
        return;
    case 0xE1:
        func_8013FB8C();
        return;
    case 0xE2:
        func_8013FCBC();
        return;
    case 0xE3:
        func_8013FDC8();
        return;
    case 0xE4:
        func_8013FE40();
        return;
    case 0xE5:
        func_8013FF98();
        return;
    case 0xE6:
        func_80140024();
        return;
    default:
        func_8013B77C();
        func_80042878(0x10);
        return;
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140430);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140574);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8014068C);

/* NON_MATCHING: T-10710, 14 lines: the six stores of the value use t6 (one andi) where the original has an extra andi v0,t6,0xFF (double u8 conversion); the original reads D_800E62BA as its own u8 scalar */
#ifdef NON_MATCHING
void func_80140B20(void) {
    u8 x;
    u8 y;

    x = *(u8 *)&D_800E6280.unk_3A;
    if (x >= 3) {
        x -= 2;
        y = x;
        D_801220E0 = D_801220E1 = D_801220E2 = D_80122104 = D_80122105 = D_80122106 = y;
        *(u8 *)&D_800E6280.unk_3A = x;
        if (x < 0x80) {
            *(s8 *)&D_800E6280.unk_3A = x;
            func_80057390(x);
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140B20);
#endif

void func_80140BA4(void) {
    if (D_800E6280.unk_110D++ >= 0x41U) {
        func_8013B77C();
        func_80057390(0);
        if (D_800E6280.unk_03E == 0x62 && D_800E6280.unk_03F == 3 && D_800E6280.unk_040 == 1) {
            func_8013E71C();
            func_80041878();
            func_80041F48();
            D_800E6280.unk_F5E = 0;
            func_80042878(0x14);
        } else if (D_800E6280.unk_03E == 0x5F && D_800E6280.unk_03F == 4 && D_800E6280.unk_040 == 4) {
            func_8013E71C();
            func_80041878();
            func_80041F48();
            D_800E6280.unk_F5E = 0;
            func_80042878(0x14);
        } else {
            func_800674B0();
            func_80042878(0x34);
            func_80042908(0xFF);
        }
    }
    D_8011ECA4 = 1;
}

s32 func_80140CCC(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8014068C();
        break;
    case 1:
        func_80140BA4();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BEB0);

void func_80140D20(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BEB0, 0);
    k_disp_start(2);
    func_8004284C();
}

/* FAKE: biased byte cursor, alternate end expression and statement layout preserve addressing and scheduling, T-9580. */
void func_80140D6C(void)
{
  u8 *p;
  u8 *new_var;
  if (D_800E6280.unk_F88 & 0x860)
  {
    p = (u8 *) (&D_800E7D10); do
    {
      if (p[4] == 1)
      {
        break;
      }
      p++;
    }
    while (p != (&D_800E7D1F));
    new_var = ((u8 *) (&D_800E7D10)) - -15;
    if (p == new_var)
    {
      func_8004284C();
      return;
    }
    func_80042908(2);
    if (D_800E8BEE != 0)
    {
      func_80042940(1);
      return;
    }
    func_80042940(0x20);
  }
}

void func_80140E18(void) {
    func_8004E58C();
    func_8004E788(-0x40, 0x20, 0xF, "別のメモリーカードを差し込み口", 0);
    func_8004E788(-0x40, 0x30, 0xF, "１にセットするか、他のゲームの", 0);
    func_8004E788(-0x40, 0x40, 0xF, "データを消去してください。", 0);
    func_8004E884(2);
    func_8004284C();
}

void func_80140E9C(void) {
    if (D_800E6280.unk_F88 & 0x860) {
        func_80042908(1);
        func_80042940(6);
    }
}

void func_80140ED4(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80140D20();
        return;
    case 1:
        func_80140D6C();
        return;
    case 2:
        func_80140E18();
        return;
    case 3:
        func_80140E9C();
        return;
    }
}

void func_80140F54(void) {
    if (D_800E8BEE) {
        func_80042940(1);
    } else {
        func_80042940(2);
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80140F90);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80141820);

s32 func_80141FA4(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80140F54();
        break;
    case 1:
        func_80140F90();
        break;
    case 2:
        func_80141820();
        break;
    }
    D_800E6280.unk_1092 = 0;
}

s32 func_80142028(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004E58C();
        func_8004E788(-0x40, 0x20, 0xF, "未初期化のメモリーカードです。", 0);
        func_8004E884(1);
        D_800E6280.unk_110D++;
    default:
        break;
    case 1:
        if (D_800E6280.unk_F88 & 0x860) {
            D_800E6280.unk_110D++;
        }
        break;
    case 2:
        func_8004E58C();
        func_8004E788(-0x40, 0x20, 0xF, "初期化していいですか？", 0);
        func_8004E788(-0x40, 0x30, 0xF, D_800B3C70, 0);
        func_8004E788(0x14, 0x30, 1, func_8006CA9C(), 0);
        func_8004E788(-0x40, 0x40, 0xF, D_800B3C88, 0);
        func_8004E788(0x14, 0x40, 2, func_8006CAE0(), 0);
        func_8004E884(5);
        D_800E6280.unk_110D++;
        break;
    case 3:
        if (D_800E6280.unk_F88 & 0x20) {
            if (func_80054108() == 1) {
                func_80042908(2);
            } else {
                func_80042908(1);
            }
        } else if (D_800E6280.unk_F88 & 0x40) {
            D_800E6280.unk_110D++;
        }
        break;
    case 4:
        func_8004E58C();
        func_8004E788(-0x40, 0x20, 0xF, "別のメモリーカードを", 0);
        func_8004E788(-0x40, 0x30, 1, "差し込み口１", 0);
        func_8004E788(-0x40, 0x30, 0xF, "　　　　　　にセットして、", 0);
        func_8004E788(-0x40, 0x40, 0xF, "　ボタンを押してください。", 0);
        func_8004E788(-0x40, 0x40, 1, func_8006CA9C(), 0);
        func_8004E884(5);
        D_800E6280.unk_110D++;
        break;
    case 5:
        if (D_800E6280.unk_F88 & 0x20) {
            func_80042908(1);
        }
        break;
    }
}

void func_801422F0(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80142028();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8014231C);

void func_80142560(void) {
    if ((D_800E6280.unk_1109 | D_800E6280.unk_110A) != 0) {
        D_801230D0 -= 1;
    }
    if (D_801230D0 > 0) {
        func_8004AC18(D_801230D0);
    }
}

s32 func_801425C0(void) {
    s32 temp_t3;
    s32 temp_t7;
    s32 var_s0;

    for (var_s0 = 0; var_s0 < 14; var_s0++) {
        if (((NameMetricWork *)&D_800E7D10)->slot_status[var_s0 + 1] == 0) {
            func_80049A40(((var_s0 / 7) * 0x1F) - 0x90, ((var_s0 % 7) * 0x10) - 0x12, 0x1E, 0xF, 2, 0x808080, 0x81);
        }
        if (((NameMetricWork *)&D_800E7D10)->slot_status[var_s0 + 1] == 0xFF) {
            func_80049A40(((var_s0 / 7) * 0x1F) - 0x90, ((var_s0 % 7) * 0x10) - 0x12, 0x1E, 0xF, 2, 0x8080, 0x81);
        }
    }
    func_800494BC(0, 0, 2, 2, 0);
    if (D_800E6280.unk_1109 != 0) {
        if (D_800E8BEE != 0) {
            temp_t7 = D_800E6280.unk_10F8 & 0x1F;
            if (temp_t7 != 0) {
                if (temp_t7 == 0x10) {
                    ((DisplayRecord44 *)D_8011ECD0)[126].unk_03 &= 0x7F;
                }
            } else {
                ((DisplayRecord44 *)D_8011ECD0)[126].unk_03 |= 0x80;
            }
            ((DisplayRecord44 *)D_8011ECD0)[126].unk_3C[7] = 0x11;
            ((DisplayRecord44 *)D_8011ECD0)[127].unk_3C[7] = 0;
        } else {
            temp_t3 = D_800E6280.unk_10F8 & 0x1F;
            if (temp_t3 != 0) {
                if (temp_t3 == 0x10) {
                    ((DisplayRecord44 *)D_8011ECD0)[127].unk_03 &= 0x7F;
                }
            } else {
                ((DisplayRecord44 *)D_8011ECD0)[127].unk_03 |= 0x80;
            }
            ((DisplayRecord44 *)D_8011ECD0)[126].unk_3C[7] = 0;
            ((DisplayRecord44 *)D_8011ECD0)[127].unk_3C[7] = 0x11;
        }
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_80142880);

s32 func_801429BC(void) {
    if (D_800E6280.unk_F88 & 0x20) {
        if (D_8011ECF6 < -0x50 && D_8011ECF6 >= -0x8F) {
            if (D_8011ECFA < -0x18 && D_8011ECFA >= -0x27) {
                func_80044750(0x50F);
                func_80042908(0xD0);
                return 1;
            }
        }
    }
    return 0;
}

void func_80142A3C(void) {
    s32 i;

    /* FAKE: loop body on the for line; as1 schedules by source line and then fills the delay slot like the original. T-9170 */
    for (i = 0; i < 13; i++) D_801217D0[i].unk_00 = 0x80000000;
}
