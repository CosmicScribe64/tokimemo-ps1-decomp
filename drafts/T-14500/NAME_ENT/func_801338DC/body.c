void func_801338DC(s32 arg0) {
    u8 *sp100;
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
    void *sp44;
    s32 *var_a0;
    s32 *var_a1;
    s32 *var_a2;
    s32 *var_v0;
    s32 *var_v1;
    s32 temp_t6;
    s32 var_v1_2;
    u16 temp_v1;
    void *temp_a0;
    void *temp_a1;
    void *temp_a1_2;
    void *temp_a1_3;
    void *temp_a1_4;

    switch (arg0) {
    case 0:
        if (D_800E738A == 1) {
            D_8014D010 = D_8014CCE4;
        }
        if (D_800E738A == 3) {
            D_8014D010 = D_8014CD04;
        }
        if (D_800E738A == 5) {
            D_8014D010 = D_8014CD10;
        }
        break;
    case 1:
        D_8014D010 = D_8014CCE4;
        break;
    case 2:
        D_8014D010 = D_8014CD04;
        break;
    case 3:
        D_8014D010 = D_8014CD10;
        break;
    }
    spDC = 0;
    spB8 = 0;
    sp94 = 0;
    sp70 = 0;
    sp4C = 0;
    spDD = 0;
    spB9 = 0;
    sp95 = 0;
    sp71 = 0;
    sp4D = 0;
    spDE = 0;
    spBA = 0;
    sp96 = 0;
    sp72 = 0;
    sp4E = 0;
    var_v1 = &spDF;
    var_a0 = &spBB;
    var_a1 = &sp97;
    var_a2 = &sp73;
    var_v0 = &sp4F;
    do {
        var_v0 += 4;
        *(s8 *)((unsigned char *)var_v1 + (1)) = 0;
        *(s8 *)((unsigned char *)var_a0 + (1)) = 0;
        *(s8 *)((unsigned char *)var_a1 + (1)) = 0;
        *(s8 *)((unsigned char *)var_a2 + (1)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-3)) = 0;
        *(s8 *)((unsigned char *)var_v1 + (2)) = 0;
        *(s8 *)((unsigned char *)var_a0 + (2)) = 0;
        *(s8 *)((unsigned char *)var_a1 + (2)) = 0;
        *(s8 *)((unsigned char *)var_a2 + (2)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-2)) = 0;
        *(s8 *)((unsigned char *)var_v1 + (3)) = 0;
        *(s8 *)((unsigned char *)var_a0 + (3)) = 0;
        *(s8 *)((unsigned char *)var_a1 + (3)) = 0;
        *(s8 *)((unsigned char *)var_a2 + (3)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-1)) = 0;
        var_v1 += 4;
        var_a0 += 4;
        var_a1 += 4;
        var_a2 += 4;
        *(s8 *)((unsigned char *)var_v1 + (-4)) = 0;
        *(s8 *)((unsigned char *)var_a0 + (-4)) = 0;
        *(s8 *)((unsigned char *)var_a1 + (-4)) = 0;
        *(s8 *)((unsigned char *)var_a2 + (-4)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-4)) = 0;
    } while (var_v0 != &sp6F);
    var_v1_2 = D_8014CCB4;
    if ((var_v1_2 < 0) || (var_v1_2 >= 0x2D)) {
        var_v1_2 = 0;
    }
    D_8014CCB4 = var_v1_2;
    if ((D_8014CCC8 == 2) || (D_8014CCB4 = var_v1_2, (var_v1_2 == 0x26))) {
        func_80133510((s32) D_8014D010);
    } else {
        func_8004E58C();
        set_kanji_string(-0x98, -0x68, 0xA, (s32) D_8014D010, 0);
        set_kanji_string(-0x78, -0x30, 0xA, (s32) D_8014CDB0[D_8014CCB4], 0);
        D_8014CCB0 = func_800AE0E0(D_8014CE68[D_8014CCB4]);
        temp_v1 = D_8014CE64[D_8014CCB4];
        if (temp_v1 < (u32) ((D_8014CCAC * 0x10) + 0x40)) {
            temp_t6 = (temp_v1 >> 4) - 4;
            D_8014CCAC = temp_t6;
            if (temp_t6 < 0) {
                D_8014CCAC = 0;
            }
        }
        temp_a0 = D_8014CE68[D_8014CCB4] + (D_8014CCAC << 5);
        sp100 = temp_a0;
        if (func_800AE0E0(temp_a0) < 0x20) {
            func_800AE0F0(&spDC, sp100);
        } else {
            func_800AE090(&spDC, sp100, 0x20);
            temp_a1 = sp100 + 0x20;
            sp44 = temp_a1;
            if (func_800AE0E0(temp_a1) < 0x20) {
                func_800AE0F0(&spB8, temp_a1);
            } else {
                func_800AE090(&spB8, temp_a1, 0x20);
                temp_a1_2 = sp100 + 0x40;
                sp44 = temp_a1_2;
                if (func_800AE0E0(temp_a1_2) < 0x20) {
                    func_800AE0F0(&sp94, temp_a1_2);
                } else {
                    func_800AE090(&sp94, temp_a1_2, 0x20);
                    temp_a1_3 = sp100 + 0x60;
                    sp44 = temp_a1_3;
                    if (func_800AE0E0(temp_a1_3) < 0x20) {
                        func_800AE0F0(&sp70, temp_a1_3);
                    } else {
                        func_800AE090(&sp70, temp_a1_3, 0x20);
                        temp_a1_4 = sp100 + 0x80;
                        sp44 = temp_a1_4;
                        if (func_800AE0E0(temp_a1_4) < 0x20) {
                            func_800AE0F0(&sp4C, temp_a1_4);
                        } else {
                            func_800AE090(&sp4C, temp_a1_4);
                        }
                    }
                }
            }
        }
        if (spDC != 0) {
            set_kanji_string(-0x78, -0x20, 0xA, (s32) &spDC, 0);
        }
        if (spB8 != 0) {
            set_kanji_string(-0x78, -0x10, 0xA, (s32) &spB8, 0);
        }
        if (sp94 != 0) {
            set_kanji_string(-0x78, 0, 0xA, (s32) &sp94, 0);
        }
        if (sp70 != 0) {
            set_kanji_string(-0x78, 0x10, 0xA, (s32) &sp70, 0);
        }
        if (sp4C != 0) {
            set_kanji_string(-0x78, 0x20, 0xA, (s32) &sp4C, 0);
        }
    }
    func_80132FD4();
}
