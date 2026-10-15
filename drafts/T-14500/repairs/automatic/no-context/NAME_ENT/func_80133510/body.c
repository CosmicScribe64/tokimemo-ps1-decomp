void func_80133510(u8 *arg0) {
    s32 sp124;
    s32 sp103[1];
    s8 sp102;
    s8 sp101;
    u8 sp100;
    s32 spDF[1];
    s8 spDE;
    s8 spDD;
    u8 spDC;
    s32 spBB[1];
    s8 spBA;
    s8 spB9;
    u8 spB8;
    s32 sp97[1];
    s8 sp96;
    s8 sp95;
    u8 sp94;
    s32 sp73[1];
    s8 sp72;
    s8 sp71;
    u8 sp70;
    s32 sp6F[1];
    s32 sp4F[1];
    s8 sp4E;
    s8 sp4D;
    u8 sp4C;
    s32 sp48;
    s32 *var_a0;
    s32 *var_a1;
    s32 *var_a2;
    s32 *var_a3;
    s32 *var_v0;
    s32 *var_v1;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_a1_3;
    s32 temp_a1_4;
    s32 temp_a1_5;
    s32 temp_t2;
    u16 temp_v1;

    var_a0 = &spDF;
    sp100 = 0;
    spDC = 0;
    spB8 = 0;
    sp94 = 0;
    sp70 = 0;
    sp4C = 0;
    sp101 = 0;
    spDD = 0;
    spB9 = 0;
    sp95 = 0;
    sp71 = 0;
    sp4D = 0;
    sp102 = 0;
    spDE = 0;
    spBA = 0;
    sp96 = 0;
    sp72 = 0;
    sp4E = 0;
    var_v1 = &sp103;
    var_a1 = &spBB;
    var_a2 = &sp97;
    var_a3 = &sp73;
    var_v0 = &sp4F;
    do {
        var_v0 += 4;
        *(s8 *)((unsigned char *)var_v1 + (1)) = 0;
        *(s8 *)((unsigned char *)var_a0 + (1)) = 0;
        *(s8 *)((unsigned char *)var_a1 + (1)) = 0;
        *(s8 *)((unsigned char *)var_a2 + (1)) = 0;
        *(s8 *)((unsigned char *)var_a3 + (1)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-3)) = 0;
        *(s8 *)((unsigned char *)var_v1 + (2)) = 0;
        *(s8 *)((unsigned char *)var_a0 + (2)) = 0;
        *(s8 *)((unsigned char *)var_a1 + (2)) = 0;
        *(s8 *)((unsigned char *)var_a2 + (2)) = 0;
        *(s8 *)((unsigned char *)var_a3 + (2)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-2)) = 0;
        *(s8 *)((unsigned char *)var_v1 + (3)) = 0;
        *(s8 *)((unsigned char *)var_a0 + (3)) = 0;
        *(s8 *)((unsigned char *)var_a1 + (3)) = 0;
        *(s8 *)((unsigned char *)var_a2 + (3)) = 0;
        *(s8 *)((unsigned char *)var_a3 + (3)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-1)) = 0;
        var_v1 += 4;
        var_a0 += 4;
        var_a1 += 4;
        var_a2 += 4;
        var_a3 += 4;
        *(s8 *)((unsigned char *)var_v1 + (-4)) = 0;
        *(s8 *)((unsigned char *)var_a0 + (-4)) = 0;
        *(s8 *)((unsigned char *)var_a1 + (-4)) = 0;
        *(s8 *)((unsigned char *)var_a2 + (-4)) = 0;
        *(s8 *)((unsigned char *)var_a3 + (-4)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-4)) = 0;
    } while (var_v0 != &sp6F);
    func_8004E58C(var_a0, var_a1, var_a2, var_a3);
    set_kanji_string(-0x98, -0x68, 0xA, arg0, 0);
    temp_v1 = *(u16 *)((unsigned char *)D_8014CE64 + (76));
    if (temp_v1 < (u32) ((D_8014CCAC * 0x10) + 0x50)) {
        temp_t2 = (temp_v1 >> 4) - 5;
        D_8014CCAC = temp_t2;
        if (temp_t2 < 0) {
            D_8014CCAC = 0;
        }
    }
    D_8014CCB0 = func_800AE0E0(*(s32 *)((unsigned char *)D_8014CE68 + (152)));
    temp_a0 = *(s32 *)((unsigned char *)D_8014CE68 + (152)) + (D_8014CCAC << 5);
    sp124 = temp_a0;
    if (func_800AE0E0(temp_a0) < 0x20) {
        func_800AE0F0(&sp100, sp124);
    } else {
        func_800AE090(&sp100, sp124, 0x20);
        temp_a1 = sp124 + 0x20;
        sp48 = temp_a1;
        if (func_800AE0E0(temp_a1, temp_a1) < 0x20) {
            func_800AE0F0(&spDC, temp_a1);
        } else {
            func_800AE090(&spDC, temp_a1, 0x20);
            temp_a1_2 = sp124 + 0x40;
            sp48 = temp_a1_2;
            if (func_800AE0E0(temp_a1_2, temp_a1_2) < 0x20) {
                func_800AE0F0(&spB8, temp_a1_2);
            } else {
                func_800AE090(&spB8, temp_a1_2, 0x20);
                temp_a1_3 = sp124 + 0x60;
                sp48 = temp_a1_3;
                if (func_800AE0E0(temp_a1_3, temp_a1_3) < 0x20) {
                    func_800AE0F0(&sp94, temp_a1_3);
                } else {
                    func_800AE090(&sp94, temp_a1_3, 0x20);
                    temp_a1_4 = sp124 + 0x80;
                    sp48 = temp_a1_4;
                    if (func_800AE0E0(temp_a1_4, temp_a1_4) < 0x20) {
                        func_800AE0F0(&sp70, temp_a1_4);
                    } else {
                        func_800AE090(&sp70, temp_a1_4);
                        temp_a1_5 = sp124 + 0xA0;
                        sp48 = temp_a1_5;
                        if (func_800AE0E0(temp_a1_5, temp_a1_5) < 0x20) {
                            func_800AE0F0(&sp4C, temp_a1_5);
                        } else {
                            func_800AE090(&sp4C, temp_a1_5);
                        }
                    }
                }
            }
        }
    }
    if (sp100 != 0) {
        set_kanji_string(-0x78, -0x30, 0xA, &sp100, 0);
    }
    if (spDC != 0) {
        set_kanji_string(-0x78, -0x20, 0xA, &spDC, 0);
    }
    if (spB8 != 0) {
        set_kanji_string(-0x78, -0x10, 0xA, &spB8, 0);
    }
    if (sp94 != 0) {
        set_kanji_string(-0x78, 0, 0xA, &sp94, 0);
    }
    if (sp70 != 0) {
        set_kanji_string(-0x78, 0x10, 0xA, &sp70, 0);
    }
    if (sp4C != 0) {
        set_kanji_string(-0x78, 0x20, 0xA, &sp4C, 0);
    }
}
