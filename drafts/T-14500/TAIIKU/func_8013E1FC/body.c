void func_8013E1FC(void) {
    s32 spBC[1];
    s32 spB8;
    s32 spB4;
    s32 spB0;
    s32 spAC;
    s32 spA8;
    s32 spA4;
    s32 spA0;
    s32 sp9C;
    s32 sp94;
    s32 sp90;
    s32 sp78;
    s32 sp60;
    s32 *var_s0;
    s32 *var_s0_2;
    s32 *var_s0_3;
    s32 *var_s0_4;
    s32 *var_s0_5;
    s32 *var_s0_6;
    s32 *var_t1;
    s32 *var_t1_2;
    s32 *var_v1;
    s32 *var_v1_2;
    s32 *var_v1_3;
    s32 *var_v1_4;
    s32 *var_v1_5;
    s32 *var_v1_6;
    s32 temp_a1;
    s32 temp_t0;
    s32 temp_t0_2;
    s32 temp_t2;
    s32 temp_t2_2;
    s32 temp_t3;
    s32 temp_t3_2;
    s32 temp_t3_3;
    s32 temp_t3_4;
    s32 temp_t4;
    s32 temp_t4_2;
    s32 temp_t4_3;
    s32 temp_t5;
    s32 temp_t5_2;
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_t7_2;
    s32 temp_t7_3;
    s32 temp_t7_4;
    s32 temp_t8;
    s32 temp_t8_2;
    s32 temp_t9;
    s32 temp_t9_2;
    s32 temp_t9_3;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a0_4;
    s32 var_at;
    s32 var_at_2;
    s32 var_at_3;
    s32 var_at_4;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_3;
    s32 var_s1_4;
    s32 var_t0;
    s32 var_t0_2;
    s32 var_t8;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v0_3;
    s32 var_v0_4;
    s32 var_v0_5;
    u8 *temp_t1;
    u8 *temp_t1_2;
    u8 *temp_v1;
    u8 *temp_v1_2;
    u8 *temp_v1_3;
    u8 *temp_v1_4;

    var_v0 = 0;
    var_t8 = 0 * 8;
    do {
        temp_a1 = var_v0 + 1;
        spB0 = -0x140;
        spB8 = -0xF0;
        sp9C = var_t8;
        spA0 = temp_a1 * 8;
        spB4 = -0xF0;
        spAC = -0x140;
        var_v1 = &spAC;
loop_2:
        temp_t2 = *var_v1;
        var_v1 += 4;
        *(s32 *)((unsigned char *)var_v1 + (-4)) = (s32) (temp_t2 + *D_80122DD0);
        if ((u32) var_v1 < (u32) &spBC) {
            goto loop_2;
        }
        var_s0 = &sp9C;
loop_4:
        temp_t4 = *var_s0;
        var_s0 += 4;
        *(s32 *)((unsigned char *)var_s0 + (-4)) = (s32) (temp_t4 + D_80122DD4);
        if ((u32) var_s0 < (u32) &spA4) {
            goto loop_4;
        }
        var_s0_2 = &sp9C;
        sp78 = temp_a1;
loop_6:
        temp_t7 = *var_s0_2;
        var_s0_2 += 4;
        *(s32 *)((unsigned char *)var_s0_2 + (-4)) = (s32) ((s32) (temp_t7 * func_800A0140(0x2AA)) >> 0xC);
        if ((u32) var_s0_2 < (u32) &spA4) {
            goto loop_6;
        }
        temp_t3 = (s32) (spB4 + spAC) >> 1;
        temp_t7_2 = (s32) (spB8 + spB0) >> 1;
        var_s1 = 0;
        spA4 = sp9C;
        spA8 = spA0;
        var_a0 = 0;
loop_8:
        temp_t6 = var_a0 * 8;
        temp_t1 = &sp9C + temp_t6;
        temp_v0 = (*(s32 *)temp_t1);
        var_s1 += 1;
        if ((temp_v0 > 0) || (*(s32 *)((unsigned char *)temp_t1 + (4)) > 0)) {
            temp_v1 = &spAC + temp_t6;
            var_t0 = (*(s32 *)temp_v1);
            if (temp_t3 < var_t0) {
                var_t0 = var_t0 + temp_v0 + temp_t3;
                (*(s32 *)temp_v1) = var_t0;
            }
            if (var_t0 < temp_t3) {
                (*(s32 *)temp_v1) = (s32) ((var_t0 - temp_v0) + temp_t3);
            }
            var_v0_2 = *(s32 *)((unsigned char *)temp_v1 + (4));
            var_at = var_v0_2 < temp_t7_2;
            if (temp_t7_2 < var_v0_2) {
                var_v0_2 = var_v0_2 + *(s32 *)((unsigned char *)temp_t1 + (4)) + temp_t7_2;
                *(s32 *)((unsigned char *)temp_v1 + (4)) = var_v0_2;
                var_at = var_v0_2 < temp_t7_2;
            }
            var_at_2 = var_s1 < 2;
            if (var_at != 0) {
                *(s32 *)((unsigned char *)temp_v1 + (4)) = (s32) ((var_v0_2 - *(s32 *)((unsigned char *)temp_t1 + (4))) + temp_t7_2);
                goto block_18;
            }
        } else {
block_18:
            var_at_2 = var_s1 < 2;
        }
        var_a0 = var_s1;
        if (var_at_2 != 0) {
            goto loop_8;
        }
        var_v1_2 = &spAC;
loop_21:
        temp_t3_2 = *var_v1_2;
        var_v1_2 += 4;
        *(s32 *)((unsigned char *)var_v1_2 + (-4)) = (s32) -temp_t3_2;
        if ((u32) var_v1_2 < (u32) &spBC) {
            goto loop_21;
        }
        var_s0_3 = &sp9C;
loop_23:
        temp_t5 = *var_s0_3;
        var_s0_3 += 4;
        *(s32 *)((unsigned char *)var_s0_3 + (-4)) = (s32) (temp_t5 - 0x18);
        if ((u32) var_s0_3 < (u32) &spAC) {
            goto loop_23;
        }
        var_v1_3 = &spAC;
loop_25:
        temp_t9 = *var_v1_3;
        var_v1_3 += 4;
        *(s32 *)((unsigned char *)var_v1_3 + (-4)) = (s32) (temp_t9 - ((s16) *D_80149984 >> 2));
        if ((u32) var_v1_3 < (u32) &spBC) {
            goto loop_25;
        }
        var_s1_2 = 0;
        var_a0_2 = 0;
loop_27:
        temp_t4_2 = var_a0_2 * 8;
        temp_v1_2 = &spAC + temp_t4_2;
        temp_t0 = (*(s32 *)temp_v1_2);
        if ((temp_t0 < -0xA0) || (var_t1 = &sp9C + temp_t4_2, ((temp_t0 < 0xA1) == 0)) || (var_v0_3 = *var_t1, ((var_v0_3 < -0x17) != 0)) || (var_v0_3 >= 0x79)) {
            temp_v0_2 = *(s32 *)((unsigned char *)temp_v1_2 + (4));
            if (temp_v0_2 >= -0xA0) {
                var_t1 = &sp9C + temp_t4_2;
                if (temp_v0_2 < 0xA1) {
                    temp_v0_3 = *(s32 *)((unsigned char *)var_t1 + (4));
                    if ((temp_v0_3 >= -0x17) && (temp_v0_3 < 0x79)) {
                        var_v0_3 = (*(s32 *)var_t1);
                        goto block_36;
                    }
                }
            }
        } else {
block_36:
            func_80049FF0((s32) (s16) temp_t0, (s32) (s16) var_v0_3, (s32) (s16) *(s32 *)((unsigned char *)temp_v1_2 + (4)), (s32) *(s16 *)((unsigned char *)var_t1 + (4)), 0xFFFFFF, 0xFFFFFF, 0xFA);
        }
        var_s1_2 += 1;
        var_a0_2 = var_s1_2;
        if (var_s1_2 != 2) {
            goto loop_27;
        }
        var_v0 = sp78;
        var_t8 = var_v0 * 8;
    } while (var_v0 < 0x40);
    sp60 = 0x10;
    sp78 = 0;
    do {
        sp94 = -0x140;
        sp90 = 0;
        Rot_2D(sp78, &sp94, &sp90, &spAC, &sp9C);
        Rot_2D(sp60, &sp94, &sp90, &spB0, &spA0);
        sp94 = -0xF0;
        sp90 = 0;
        Rot_2D(sp78, &sp94, &sp90, &spB4, &spA4);
        Rot_2D(sp60, &sp94, &sp90, &spB8, &spA8);
        var_v1_4 = &spAC;
        var_s0_4 = &sp9C;
loop_41:
        temp_t9_2 = *var_s0_4;
        var_s0_4 += 4;
        temp_t8 = *var_v1_4 + *D_80122DD0;
        var_v1_4 += 4;
        *(s32 *)((unsigned char *)var_v1_4 + (-4)) = temp_t8;
        *(s32 *)((unsigned char *)var_s0_4 + (-4)) = (s32) (temp_t9_2 + D_80122DD4);
        if ((u32) var_s0_4 < (u32) &spAC) {
            goto loop_41;
        }
        var_s0_5 = &sp9C;
loop_43:
        temp_t3_3 = *var_s0_5;
        var_s0_5 += 4;
        *(s32 *)((unsigned char *)var_s0_5 + (-4)) = (s32) ((s32) (temp_t3_3 * func_800A0140(0x2AA)) >> 0xC);
        if ((u32) var_s0_5 < (u32) &spAC) {
            goto loop_43;
        }
        temp_t7_3 = (s32) (spB4 + spAC) >> 1;
        temp_t9_3 = (s32) (spB8 + spB0) >> 1;
        var_s1_3 = 0;
        var_a0_3 = 0;
loop_45:
        temp_t2_2 = var_a0_3 * 8;
        temp_t1_2 = &sp9C + temp_t2_2;
        temp_v0_4 = (*(s32 *)temp_t1_2);
        var_s1_3 += 1;
        if ((temp_v0_4 > 0) || (*(s32 *)((unsigned char *)temp_t1_2 + (4)) > 0)) {
            temp_v1_3 = &spAC + temp_t2_2;
            var_t0_2 = (*(s32 *)temp_v1_3);
            if (temp_t7_3 < var_t0_2) {
                var_t0_2 = var_t0_2 + temp_v0_4 + temp_t7_3;
                (*(s32 *)temp_v1_3) = var_t0_2;
            }
            if (var_t0_2 < temp_t7_3) {
                (*(s32 *)temp_v1_3) = (s32) ((var_t0_2 - temp_v0_4) + temp_t7_3);
            }
            var_v0_4 = *(s32 *)((unsigned char *)temp_v1_3 + (4));
            var_at_3 = var_v0_4 < temp_t9_3;
            if (temp_t9_3 < var_v0_4) {
                var_v0_4 = var_v0_4 + *(s32 *)((unsigned char *)temp_t1_2 + (4)) + temp_t9_3;
                *(s32 *)((unsigned char *)temp_v1_3 + (4)) = var_v0_4;
                var_at_3 = var_v0_4 < temp_t9_3;
            }
            var_at_4 = var_s1_3 < 2;
            if (var_at_3 != 0) {
                *(s32 *)((unsigned char *)temp_v1_3 + (4)) = (s32) ((var_v0_4 - *(s32 *)((unsigned char *)temp_t1_2 + (4))) + temp_t9_3);
                goto block_55;
            }
        } else {
block_55:
            var_at_4 = var_s1_3 < 2;
        }
        var_a0_3 = var_s1_3;
        if (var_at_4 != 0) {
            goto loop_45;
        }
        var_v1_5 = &spAC;
loop_58:
        temp_t7_4 = *var_v1_5;
        var_v1_5 += 4;
        *(s32 *)((unsigned char *)var_v1_5 + (-4)) = (s32) -temp_t7_4;
        if ((u32) var_v1_5 < (u32) &spBC) {
            goto loop_58;
        }
        var_s0_6 = &sp9C;
loop_60:
        temp_t8_2 = *var_s0_6;
        var_s0_6 += 4;
        *(s32 *)((unsigned char *)var_s0_6 + (-4)) = (s32) (temp_t8_2 - 0x18);
        if ((u32) var_s0_6 < (u32) &spAC) {
            goto loop_60;
        }
        var_v1_6 = &spAC;
loop_62:
        temp_t3_4 = *var_v1_6;
        var_v1_6 += 4;
        *(s32 *)((unsigned char *)var_v1_6 + (-4)) = (s32) (temp_t3_4 - ((s16) *D_80149984 >> 2));
        if ((u32) var_v1_6 < (u32) &spBC) {
            goto loop_62;
        }
        var_s1_4 = 0;
        var_a0_4 = 0;
loop_64:
        temp_t5_2 = var_a0_4 * 8;
        temp_v1_4 = &spAC + temp_t5_2;
        temp_t0_2 = (*(s32 *)temp_v1_4);
        if ((temp_t0_2 < -0xA0) || (var_t1_2 = &sp9C + temp_t5_2, ((temp_t0_2 < 0xA1) == 0)) || (var_v0_5 = *var_t1_2, ((var_v0_5 < -0x17) != 0)) || (var_v0_5 >= 0x79)) {
            temp_v0_5 = *(s32 *)((unsigned char *)temp_v1_4 + (4));
            if (temp_v0_5 >= -0xA0) {
                var_t1_2 = &sp9C + temp_t5_2;
                if (temp_v0_5 < 0xA1) {
                    temp_v0_6 = *(s32 *)((unsigned char *)var_t1_2 + (4));
                    if ((temp_v0_6 >= -0x17) && (temp_v0_6 < 0x79)) {
                        var_v0_5 = (*(s32 *)var_t1_2);
                        goto block_73;
                    }
                }
            }
        } else {
block_73:
            func_80049FF0((s32) (s16) temp_t0_2, (s32) (s16) var_v0_5, (s32) (s16) *(s32 *)((unsigned char *)temp_v1_4 + (4)), (s32) *(s16 *)((unsigned char *)var_t1_2 + (4)), 0xFFFFFF, 0xFFFFFF, 0xFA);
        }
        var_s1_4 += 1;
        var_a0_4 = var_s1_4;
        if (var_s1_4 != 2) {
            goto loop_64;
        }
        temp_t4_3 = sp60 + 0x10;
        sp60 = temp_t4_3;
        sp78 += 0x10;
    } while (temp_t4_3 != 0x410);
}
