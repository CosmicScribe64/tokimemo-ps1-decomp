void func_80139510(void) {
    s16 sp46;
    s16 sp44;
    s16 sp42;
    s16 sp40;
    s16 sp3E;
    s16 sp3C;
    s16 sp3A;
    s16 sp38;
    s16 sp36;
    s16 sp34;
    s16 sp32;
    s16 sp30;
    s16 sp2E;
    s16 sp2C;
    s16 sp2A;
    s16 sp28;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 temp_v1_5;
    s32 temp_v1_6;

    switch (D_8015EE30.unk_00) {
    case 0:
        D_8015ED98 = 0x1000;
        func_8014F500();
    default:
        break;
    case 1:
        func_801374D0();
        break;
    case 2:
        temp_v1 = D_8015EE30.unk_08;
        if (temp_v1 == 0) {
            func_8013F15C(0x507, 1, 0);
        }
        D_8015EE30.unk_08 = temp_v1 + 8;
        k_disp_switch(D_8015EDE8, 0);
        D_80121622 -= 8;
        D_80121666 -= 8;
        D_801216AA -= 8;
        if (D_8015EE30.unk_08 >= 0x91) {
            func_8014F500();
        }
        break;
    case 3:
        func_8014F5DC(0x1E);
        break;
    case 4:
        func_8013E7C0(0x30, 9, 1, 1);
        switch (D_8015EB9C) {
        case 0:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        case 1:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        case 2:
            func_8013E7C0(0x1D, 2, 1, 1);
            break;
        case 3:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        case 4:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        case 5:
            func_8013E7C0(0x1D, 2, 1, 1);
            break;
        case 6:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        case 7:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        case 8:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        case 9:
            func_8013E7C0(0x1D, 7, 1, 1);
            break;
        case 10:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        case 13:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        }
        func_8004500C(1, 0x202);
        func_800AE0F0(D_8015E9E0, "戦闘に勝った！！");
        func_800AE0F0(D_8015EA08, "経験値　：２３６０ＥＸＰ");
        func_800AE0F0(D_8015EA30, "お金　　：３６００ゴールド");
        func_80139E88();
        func_8014B738((s32) D_8015EB70, D_8015ED14, 0x5A);
        func_8014F500();
        break;
    case 5:
        func_8014F6D4(0);
        break;
    case 6:
        D_8015EBFC.unk_18 = 0;
        if (func_801447B8() != 0) {
            func_8014F500();
        } else {
            func_80042808();
        }
        break;
    case 7:
        func_8014F5DC(0x3C);
        break;
    case 8:
        D_80120E93 = 0x40;
        func_8013E7C0(0x30, 0, 1, 1);
        func_8013E7C0(0x1D, 0, 1, 1);
        func_8014F500();
        break;
    case 9:
        func_801375F8();
        break;
    case 10:
        func_800AE0F0(D_8015E9E0, &D_800E6354);
        func_800AE100(D_8015E9E0, "「誰だっ！");
        func_8014B738((s32) D_8015EB70, 1, 0x3C);
        func_8014F500();
        break;
    case 11:
        func_8014F6D4(0);
        break;
    case 12:
        func_8013E97C(0x3B, D_8015EE30.unk_08 - 0x80, 0);
        func_8013E7C0(0x3B, 2, 1, 1);
        temp_v1_2 = D_8015EE30.unk_08 + 8;
        D_8015EE30.unk_08 = temp_v1_2;
        if (temp_v1_2 >= 0x81) {
            func_8004500C(1, 0x201);
            k_disp_switch(D_8015EE40, 1);
            func_8014F500();
        }
        break;
    case 13:
        func_801376D4();
        break;
    case 14:
        func_8013E7C0(0x3B, 3, 5, 1);
        func_8013F15C(0x508, 1, 0);
        func_8014F500();
        break;
    case 15:
        if (func_8013E90C(0x3B) != 0) {
            func_8013EA60(0, 0x21, D_8015ED94, 1);
            func_8013E7C0(0x3B, 2, 1, 1);
            func_8013E7C0(0x30, 7, 1, 1);
            D_8015ED94 = 0;
            func_8014F500();
        }
        break;
    case 16:
        func_8004500C(1, 0x203);
        func_8013EA30(0xFF);
        temp_v1_3 = D_8015EE30.unk_08 + 1;
        D_8015EE30.unk_08 = temp_v1_3;
        if (temp_v1_3 >= 5) {
            func_8014F500();
        }
        break;
    case 17:
        func_8013EA30(0x80);
        D_80120E93 = 0x40 - D_8015EE30.unk_08;
        temp_v1_4 = D_8015EE30.unk_08 + 1;
        D_8015EE30.unk_08 = temp_v1_4;
        if (temp_v1_4 >= 0x41) {
            func_8014F500();
        }
        break;
    case 18:
        func_8013794C();
        break;
    case 19:
        D_8015EBFC.unk_28 = 0xFFF;
        func_8014F5DC(0x3C);
        break;
    case 20:
        func_8013A034();
        break;
    case 21:
        func_800AE0F0(D_8015E9E0, "（どこからともなく声が聞こえてきた）");
        func_8014B738((s32) D_8015EB70, 1, 0x5A);
        func_8014F500();
        break;
    case 22:
        func_8014F6D4(0);
        break;
    case 23:
        func_80137AC8();
        break;
    case 24:
        func_8013A2F4();
        break;
    case 25:
        func_8013E7C0(0x30, 8, 5, 1);
        D_8015ED94 = D_8015EDA0;
        temp_v1_5 = D_8015EDA0 + 0x7D0;
        D_8015EDA0 = temp_v1_5;
        if (temp_v1_5 >= 0x2710) {
            D_8015EDA0 = 0x270F;
        }
        func_8014F500();
        break;
    case 26:
        func_80137C44();
        break;
    case 27:
        func_80137E74();
        break;
    case 28:
        func_800AE0F0(D_8015E9E0, &D_800E6354);
        func_800AE100(D_8015E9E0, "「もう超眼力は見切ったぜ！");
        func_800AE0F0(D_8015EA08, "番長！　負ける訳にはいかない！！");
        func_8014B738((s32) D_8015EB70, 2, 0x5A);
        func_8014F500();
        break;
    case 29:
        func_8014F6D4(2);
        break;
    case 30:
        func_80137F50();
        break;
    case 31:
        sp42 = 0x100;
        sp40 = 0x240;
        sp46 = 0x28;
        sp44 = 0x40;
        func_8009C93C((RECT *) &sp40, 0x180, 0x180);
        sp3A = 0x128;
        sp38 = 0x240;
        sp3E = 0x28;
        sp3C = 0x40;
        func_8009C93C((RECT *) &sp38, 0x1C0, 0x180);
        sp32 = 0x100;
        sp30 = 0x220;
        sp36 = 0x28;
        sp34 = 0x40;
        func_8009C93C((RECT *) &sp30, 0x200, 0x180);
        sp2A = 0x100;
        sp28 = 0x21F;
        sp2E = 0x80;
        sp2C = 1;
        func_8009C93C((RECT *) &sp28, 0x220, 0x100);
        D_8015EC38 = 0;
        D_8015EC3C = 0x20;
        D_8015EC40 = 0x20;
        D_8015EC44 = 0x40;
        D_8015EC48 = 1;
        D_8015EC4C = 1;
        func_8014F500();
        break;
    case 32:
        D_80120E93 = (s8) D_8015EE30.unk_08;
        func_8013E7C0(0x1F, 0, 1, 0);
        temp_v1_6 = D_8015EE30.unk_08 + 1;
        D_8015EE30.unk_08 = temp_v1_6;
        if (temp_v1_6 >= 0x81) {
            func_8014F500();
        }
        break;
    case 33:
        D_8015EE1C = 0;
        D_8015EE30.unk_00 = 0;
        func_8004500C(1, 0x204);
        if (D_8015EDF0 != 0x40000000) {
            D_8015EDF0 = 0;
        }
        D_8015EBDC = 0;
        D_8015EBD8 = 0x3E8;
        D_8015EBFC.unk_00 = 0;
        D_8015EBFC.unk_04 = 0;
        D_8015EBFC.unk_08 = 0;
        D_8015EBFC.unk_0C = 0;
        D_8015EE00.unk_04 = 0;
        D_8015EDF4 = 0;
        D_8015ED98 = 0;
        D_8015EDAC = 0;
        D_8015ED9C = 0xFFFF;
        D_8015EDD8 = 0;
        D_8015EDC8 = 0;
        D_8015EDC4 = 0x200000;
        break;
    }
}
