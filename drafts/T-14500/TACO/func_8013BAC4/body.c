void func_8013BAC4(void *arg0, s32 arg1, s32 arg2) {
    s32 sp68[16];
    s32 sp494;
    u32 sp478;
    s32 sp468[3];
    s32 sp60;
    u8 *sp5C;
    s16 temp_s4;
    s16 temp_s4_2;
    s16 temp_s5;
    s16 temp_s5_2;
    s16 temp_s6;
    s16 var_s6;
    s32 *var_v1;
    s32 temp_at;
    s32 temp_at_2;
    s32 temp_s0_2;
    s32 temp_s0_5;
    s32 temp_s1;
    s32 temp_t1;
    s32 temp_t2;
    s32 temp_t3;
    s32 temp_t4;
    s32 temp_t4_2;
    s32 temp_t6;
    s32 temp_v0_4;
    s32 temp_v1;
    s32 temp_v1_3;
    s32 var_s2;
    s32 var_s2_2;
    s32 var_s3;
    u32 *var_v0;
    u32 *var_v0_2;
    u32 *var_v0_3;
    u32 *var_v0_4;
    u8 *temp_s0;
    u8 *temp_s0_3;
    u8 *temp_s0_4;
    u8 *temp_s0_6;
    u8 *temp_s0_7;
    u8 *temp_s0_8;
    u8 *temp_s0_9;
    u8 *var_s0;
    u8 temp_v0_2;
    u8 temp_v0_3;
    u8 temp_v0_5;
    u8 temp_v0_6;
    u8 temp_v1_2;
    u8 *temp_v0;

    (*(s32 *)&sp468) = (s32) D_8015F548;
    *(u8 *)((unsigned char *)(&sp468) + (8)) = (u8) *(u8 *)((unsigned char *)(&D_8015F548) + (8));
    *(s32 *)((unsigned char *)(&sp468) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015F548) + (4));
    if (D_800E7389 != 0xC) {
        var_v0 = &sp68[0];
        if (D_800E7389 != 0x16) {
            var_v0_2 = &sp68[0];
            if (D_800E7389 != 0x20) {
                var_v0_3 = &sp68[0];
                var_v1 = D_8015F504;
                do {
                    temp_t1 = (*(s32 *)var_v1);
                    temp_t2 = *(s32 *)((unsigned char *)var_v1 + (4));
                    temp_t3 = *(s32 *)((unsigned char *)var_v1 + (8));
                    temp_t4 = *(s32 *)((unsigned char *)var_v1 + (12));
                    var_v1 += 0x10;
                    var_v0_3 += 0x10;
                    *(s32 *)((unsigned char *)var_v0_3 + (-16)) = temp_t1;
                    *(s32 *)((unsigned char *)var_v0_3 + (-12)) = temp_t2;
                    *(s32 *)((unsigned char *)var_v0_3 + (-8)) = temp_t3;
                    *(s32 *)((unsigned char *)var_v0_3 + (-4)) = temp_t4;
                } while (var_v1 != &D_8015F904);
            } else {
                do {
                    var_v0_2 += 0x10;
                    *(s32 *)((unsigned char *)var_v0_2 + (-16)) = 0x320;
                    *(s32 *)((unsigned char *)var_v0_2 + (-12)) = 0x320;
                    *(s32 *)((unsigned char *)var_v0_2 + (-8)) = 0x320;
                    *(s32 *)((unsigned char *)var_v0_2 + (-4)) = 0x320;
                } while (var_v0_2 != &sp468);
            }
        } else {
            do {
                var_v0 += 0x10;
                *(s32 *)((unsigned char *)var_v0 + (-16)) = 0xC8;
                *(s32 *)((unsigned char *)var_v0 + (-12)) = 0xC8;
                *(s32 *)((unsigned char *)var_v0 + (-8)) = 0xC8;
                *(s32 *)((unsigned char *)var_v0 + (-4)) = 0xC8;
            } while (var_v0 != &sp468);
        }
    } else {
        var_v0_4 = &sp68[0];
        do {
            var_v0_4 += 0x10;
            *(s32 *)((unsigned char *)var_v0_4 + (-16)) = 0x190;
            *(s32 *)((unsigned char *)var_v0_4 + (-12)) = 0x190;
            *(s32 *)((unsigned char *)var_v0_4 + (-8)) = 0x190;
            *(s32 *)((unsigned char *)var_v0_4 + (-4)) = 0x190;
        } while (var_v0_4 != &sp468);
    }
    D_8015F500 = 0;
    if (*(u8 *)((unsigned char *)arg0 + (19)) & 0x20) {
        sp494 = 0;
        do {
            if (D_800E7378 & 1) {
                temp_v0 = (sp494 * 0x14) + ((u8 *)arg0);
                temp_s4 = (*(s32 *)temp_v0);
                temp_s5 = *(s16 *)((unsigned char *)temp_v0 + (2));
                var_s2 = 0x880;
                if (arg2 == 0) {
                    var_s6 = D_8015EDB4->unk78;
                } else {
                    var_s6 = *(s16 *)((unsigned char *)D_8015EDB4 + (1208));
                }
                do {
                    temp_s0 = &D_8015EDB4->pad0[var_s2];
                    if ((*(u8 *)((unsigned char *)temp_s0 + (1)) & 0x80) && (*(u8 *)((unsigned char *)temp_s0 + (135)) & 1) && (*(u8 *)((unsigned char *)temp_s0 + (133)) != 0)) {
                        temp_v0_2 = *(u8 *)((unsigned char *)temp_s0 + (132));
                        if (temp_v0_2 != 0) {
                            temp_s0_2 = func_800AE0C0(temp_s4 - *(s16 *)((unsigned char *)temp_s0 + (116)));
                            temp_v1 = (temp_v0_2 * 0x10) + 0x18;
                            temp_at = func_800AE0C0(temp_s5 - *(s16 *)((unsigned char *)(&D_8015EDB4->pad0[var_s2]) + (118))) < temp_v1;
                            if ((temp_s0_2 < temp_v1) && (temp_at != 0)) {
                                var_s0 = &D_8015EDB4->pad0[var_s2];
                                if (*(s16 *)((unsigned char *)var_s0 + (120)) < var_s6) {
                                    if ((D_8015F500 == 0) && (arg2 == 0)) {
                                        D_8015F500 = 1;
                                        func_801335A0(1, 0x507);
                                        var_s0 = &D_8015EDB4->pad0[var_s2];
                                    }
                                    temp_v0_3 = *(u8 *)((unsigned char *)var_s0 + (133));
                                    if (temp_v0_3 != 0) {
                                        *(u8 *)((unsigned char *)var_s0 + (133)) = (u8) (temp_v0_3 - 1);
                                        var_s0 = &D_8015EDB4->pad0[var_s2];
                                    }
                                    func_8013D2A0((s32) temp_s4, (s32) temp_s5, (s32) (s16) (*(s16 *)((unsigned char *)var_s0 + (120)) - 0x200));
                                    temp_s0_3 = &D_8015EDB4->pad0[var_s2];
                                    D_8015EDF0 += (u32) (&sp68[0])[(u8) *(u8 *)((unsigned char *)temp_s0_3 + (134)) % 17U] / 20U;
                                    if (*(u8 *)((unsigned char *)temp_s0_3 + (133)) == 0) {
                                        D_8015EDF0 += (&sp68[0])[(u8) *(u8 *)((unsigned char *)temp_s0_3 + (134)) % 17U];
                                    }
                                }
                            }
                        }
                    }
                    var_s2 += 0x88;
                } while (var_s2 != 0x2178);
            }
            temp_t6 = sp494 + 1;
            sp494 = temp_t6;
        } while (temp_t6 != 2);
    }
    sp494 = 0;
    sp60 = 0;
    sp5C = arg0;
    do {
        temp_v1_2 = *(u8 *)((unsigned char *)sp5C + (19));
        if (temp_v1_2 & 0x80) {
            temp_v0_4 = temp_v1_2 & 0xF;
            if (temp_v0_4 != 0) {
                temp_s4_2 = (*(s32 *)sp5C);
                temp_s5_2 = *(s16 *)((unsigned char *)sp5C + (2));
                temp_s6 = *(s16 *)((unsigned char *)sp5C + (4));
                sp478 = (u32) *(u8 *)((unsigned char *)(sp + temp_v0_4) + (1128));
                if (temp_v0_4 == 8) {
                    var_s3 = 0x60;
                } else {
                    var_s3 = 0x30;
                }
                var_s2_2 = 0x880;
loop_44:
                temp_s0_4 = &D_8015EDB4->pad0[var_s2_2];
                if ((*(u8 *)((unsigned char *)temp_s0_4 + (1)) & 0x80) && (*(u8 *)((unsigned char *)temp_s0_4 + (135)) & 1) && (*(u8 *)((unsigned char *)temp_s0_4 + (133)) != 0) && (temp_v0_5 = *(u8 *)((unsigned char *)temp_s0_4 + (132)), temp_s1 = temp_v0_5 * 0x10, (temp_v0_5 != 0)) && (func_800AE0C0(temp_s6 - *(s16 *)((unsigned char *)temp_s0_4 + (120))) < (temp_s1 + 0x100)) && (temp_s0_5 = func_800AE0C0(temp_s4_2 - *(s16 *)((unsigned char *)(&D_8015EDB4->pad0[var_s2_2]) + (116))), temp_v1_3 = temp_s1 + var_s3, temp_at_2 = func_800AE0C0(temp_s5_2 - *(s16 *)((unsigned char *)(&D_8015EDB4->pad0[var_s2_2]) + (118))) < temp_v1_3, ((temp_s0_5 < temp_v1_3) != 0)) && (temp_at_2 != 0)) {
                    if ((D_8015F500 == 0) && (arg2 == 0)) {
                        D_8015F500 = 1;
                        func_801335A0(1, 0x507);
                    }
                    temp_s0_6 = &D_8015EDB4->pad0[var_s2_2];
                    temp_v0_6 = *(u8 *)((unsigned char *)temp_s0_6 + (133));
                    if (sp478 < temp_v0_6) {
                        *(u8 *)((unsigned char *)temp_s0_6 + (133)) = (u8) (temp_v0_6 - sp478);
                    } else {
                        *(u8 *)((unsigned char *)temp_s0_6 + (133)) = 0U;
                    }
                    temp_s0_7 = &D_8015EDB4->pad0[var_s2_2];
                    if (*(u8 *)((unsigned char *)temp_s0_7 + (133)) == 0) {
                        *(u8 *)((unsigned char *)temp_s0_7 + (135)) = (u8) (*(u8 *)((unsigned char *)temp_s0_7 + (135)) & 0xF0);
                    }
                    *(s8 *)((unsigned char *)sp5C + (18)) = 0;
                    *(u8 *)((unsigned char *)sp5C + (19)) = 0U;
                    temp_s0_8 = &D_8015EDB4->pad0[var_s2_2];
                    func_8013D2A0((s32) (s16) ((s32) (*(s16 *)((unsigned char *)temp_s0_8 + (116)) + temp_s4_2) / 2), (s32) (s16) ((s32) (*(s16 *)((unsigned char *)temp_s0_8 + (118)) + temp_s5_2) / 2), (s32) (s16) (temp_s6 - 0x200));
                    temp_s0_9 = &D_8015EDB4->pad0[var_s2_2];
                    D_8015EDF0 += ((u32) (&sp68[0])[(u8) *(u8 *)((unsigned char *)temp_s0_9 + (134)) % 17U] / 10U) * sp478;
                    if (*(u8 *)((unsigned char *)temp_s0_9 + (133)) == 0) {
                        func_801335A0(1, 0x508);
                        D_8015EDF0 += (&sp68[0])[(u8) *(u8 *)((unsigned char *)(&D_8015EDB4->pad0[var_s2_2]) + (134)) % 17U];
                    }
                } else {
                    var_s2_2 += 0x88;
                    if (var_s2_2 == 0x2178) {

                    } else {
                        goto loop_44;
                    }
                }
            }
        }
        temp_t4_2 = sp60 + 0x14;
        sp5C += 0x14;
        sp60 = temp_t4_2;
    } while (temp_t4_2 != 0x140);
}
