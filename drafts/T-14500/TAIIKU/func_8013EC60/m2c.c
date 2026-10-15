? func_80086204(s32, s32 *, s32 *, s32 *, s32 *);   /* extern */
extern s32 D_80122DD4;
extern s32 D_80122DD8;

void func_8013EC60(void) {
    ? spBC;
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
    s32 *var_s0;
    s32 *var_s0_2;
    s32 *var_s0_3;
    s32 *var_s0_4;
    s32 *var_s0_5;
    s32 *var_s0_6;
    s32 *var_s0_7;
    s32 *var_s1;
    s32 *var_s1_2;
    s32 *var_s1_3;
    s32 *var_s1_4;
    s32 *var_s1_5;
    s32 *var_s1_6;
    s32 *var_s1_7;
    s32 *var_t1;
    s32 *var_t1_2;
    s32 temp_a0;
    s32 temp_t0;
    s32 temp_t0_2;
    s32 temp_t2;
    s32 temp_t2_2;
    s32 temp_t2_3;
    s32 temp_t2_4;
    s32 temp_t3;
    s32 temp_t3_2;
    s32 temp_t4;
    s32 temp_t4_2;
    s32 temp_t4_3;
    s32 temp_t5;
    s32 temp_t5_2;
    s32 temp_t5_3;
    s32 temp_t5_4;
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_t7_2;
    s32 temp_t8;
    s32 temp_t8_2;
    s32 temp_t8_3;
    s32 temp_t9;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_7;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a0_4;
    s32 var_a1;
    s32 var_at;
    s32 var_at_2;
    s32 var_at_3;
    s32 var_at_4;
    s32 var_s2;
    s32 var_s2_2;
    s32 var_s2_3;
    s32 var_s2_4;
    s32 var_t0;
    s32 var_t0_2;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v0_3;
    s32 var_v0_4;
    s32 var_v0_5;
    void *temp_t1;
    void *temp_t1_2;
    void *temp_v1_2;
    void *temp_v1_3;
    void *temp_v1_4;
    void *temp_v1_5;

    var_v0 = 0;
    var_a1 = 1;
    do {
        temp_v1 = var_v0 * 8;
        temp_a0 = var_a1 * 8;
        spB0 = 0x140;
        spB8 = 0xF0;
        spA4 = temp_v1;
        spA8 = temp_a0;
        spA0 = temp_a0;
        sp9C = temp_v1;
        spB4 = 0xF0;
        spAC = 0x140;
        sp78 = var_a1;
        var_s1 = &spAC;
        var_s0 = &sp9C;
loop_2:
        sp94 = *var_s1;
        sp90 = *var_s0;
        func_80086204(D_80122DD8, &sp94, &sp90, var_s1, var_s0);
        var_s0 += 4;
        var_s1 += 4;
        if ((u32) var_s0 < (u32) &spAC) {
            goto loop_2;
        }
        var_s1_2 = &spAC;
        var_s0_2 = &sp9C;
loop_4:
        temp_t3 = *var_s0_2;
        var_s0_2 += 4;
        temp_t2 = *var_s1_2 + *D_80122DD0;
        var_s1_2 += 4;
        *(s32 *)((unsigned char *)var_s1_2 + (-4)) = temp_t2;
        *(s32 *)((unsigned char *)var_s0_2 + (-4)) = (s32) (temp_t3 + D_80122DD4);
        if ((u32) var_s0_2 < (u32) &spAC) {
            goto loop_4;
        }
        var_s0_3 = &sp9C;
loop_6:
        temp_t5 = *var_s0_3;
        var_s0_3 += 4;
        *(s32 *)((unsigned char *)var_s0_3 + (-4)) = (s32) ((s32) (temp_t5 * func_800A0140(0x2AA)) >> 0xC);
        if ((u32) var_s0_3 < (u32) &spAC) {
            goto loop_6;
        }
        temp_t8 = (s32) (spB4 + spAC) >> 1;
        temp_t3_2 = (s32) (spB8 + spB0) >> 1;
        var_s2 = 0;
        var_a0 = 0;
loop_8:
        temp_t4 = var_a0 * 8;
        temp_t1 = &sp9C + temp_t4;
        temp_v0 = temp_t1->unk0;
        var_s2 += 1;
        if ((temp_v0 > 0) || (*(s32 *)((unsigned char *)temp_t1 + (4)) > 0)) {
            temp_v1_2 = &spAC + temp_t4;
            var_t0 = temp_v1_2->unk0;
            if (temp_t8 < var_t0) {
                var_t0 = var_t0 + temp_v0 + temp_t8;
                temp_v1_2->unk0 = var_t0;
            }
            if (var_t0 < temp_t8) {
                temp_v1_2->unk0 = (s32) ((var_t0 - temp_v0) + temp_t8);
            }
            var_v0_2 = *(s32 *)((unsigned char *)temp_v1_2 + (4));
            var_at = var_v0_2 < temp_t3_2;
            if (temp_t3_2 < var_v0_2) {
                var_v0_2 = var_v0_2 + *(s32 *)((unsigned char *)temp_t1 + (4)) + temp_t3_2;
                *(s32 *)((unsigned char *)temp_v1_2 + (4)) = var_v0_2;
                var_at = var_v0_2 < temp_t3_2;
            }
            var_at_2 = var_s2 < 2;
            if (var_at != 0) {
                *(s32 *)((unsigned char *)temp_v1_2 + (4)) = (s32) ((var_v0_2 - *(s32 *)((unsigned char *)temp_t1 + (4))) + temp_t3_2);
                goto block_18;
            }
        } else {
block_18:
            var_at_2 = var_s2 < 2;
        }
        var_a0 = var_s2;
        if (var_at_2 != 0) {
            goto loop_8;
        }
        var_s1_3 = &spAC;
loop_21:
        temp_t8_2 = *var_s1_3;
        var_s1_3 += 4;
        *(s32 *)((unsigned char *)var_s1_3 + (-4)) = (s32) -temp_t8_2;
        if ((u32) var_s1_3 < (u32) &spBC) {
            goto loop_21;
        }
        var_s0_4 = &sp9C;
loop_23:
        temp_t2_2 = *var_s0_4;
        var_s0_4 += 4;
        *(s32 *)((unsigned char *)var_s0_4 + (-4)) = (s32) (temp_t2_2 - 0x18);
        if ((u32) var_s0_4 < (u32) &spAC) {
            goto loop_23;
        }
        var_s1_4 = &spAC;
loop_25:
        temp_t5_2 = *var_s1_4;
        var_s1_4 += 4;
        *(s32 *)((unsigned char *)var_s1_4 + (-4)) = (s32) (temp_t5_2 - ((s16) *D_80149984 >> 2));
        if ((u32) var_s1_4 < (u32) &spBC) {
            goto loop_25;
        }
        var_s2_2 = 0;
        var_a0_2 = 0;
loop_27:
        temp_t8_3 = var_a0_2 * 8;
        temp_v1_3 = &spAC + temp_t8_3;
        temp_t0 = temp_v1_3->unk0;
        if ((temp_t0 < -0xA0) || (var_t1 = &sp9C + temp_t8_3, ((temp_t0 < 0xA1) == 0)) || (var_v0_3 = *var_t1, ((var_v0_3 < -0x17) != 0)) || (var_v0_3 >= 0x79)) {
            temp_v0_2 = *(s32 *)((unsigned char *)temp_v1_3 + (4));
            if (temp_v0_2 >= -0xA0) {
                var_t1 = &sp9C + temp_t8_3;
                if (temp_v0_2 < 0xA1) {
                    temp_v0_3 = *(s32 *)((unsigned char *)var_t1 + (4));
                    if ((temp_v0_3 >= -0x17) && (temp_v0_3 < 0x79)) {
                        var_v0_3 = var_t1->unk0;
                        goto block_36;
                    }
                }
            }
        } else {
block_36:
            func_80049FF0((s32) (s16) temp_t0, (s32) (s16) var_v0_3, (s32) (s16) *(s32 *)((unsigned char *)temp_v1_3 + (4)), (s32) *(s16 *)((unsigned char *)var_t1 + (4)), 0xFFFFFF, 0xFFFFFF, 0xFA);
        }
        var_s2_2 += 1;
        var_a0_2 = var_s2_2;
        if (var_s2_2 != 2) {
            goto loop_27;
        }
        var_v0 = sp78;
        var_a1 = var_v0 + 1;
    } while (var_v0 < 0x40);
    D_8012080E = spB8 + 8;
    sp78 = 0;
    do {
        sp94 = 0;
        sp90 = -0x140;
        func_80086204(D_80122DD8 + sp78, &sp94, &sp90, &spAC, &sp9C);
        func_80086204(D_80122DD8 + sp78 + 0x10, &sp94, &sp90, &spB0, &spA0);
        sp94 = 0;
        sp90 = -0xF0;
        func_80086204(D_80122DD8 + sp78, &sp94, &sp90, &spB4, &spA4);
        func_80086204(D_80122DD8 + sp78 + 0x10, &sp94, &sp90, &spB8, &spA8);
        var_s1_5 = &spAC;
        var_s0_5 = &sp9C;
loop_41:
        temp_t5_3 = *var_s0_5;
        var_s0_5 += 4;
        temp_t4_2 = *var_s1_5 + *D_80122DD0;
        var_s1_5 += 4;
        *(s32 *)((unsigned char *)var_s1_5 + (-4)) = temp_t4_2;
        *(s32 *)((unsigned char *)var_s0_5 + (-4)) = (s32) (temp_t5_3 + D_80122DD4);
        if ((u32) var_s0_5 < (u32) &spAC) {
            goto loop_41;
        }
        var_s0_6 = &sp9C;
loop_43:
        temp_t7 = *var_s0_6;
        var_s0_6 += 4;
        *(s32 *)((unsigned char *)var_s0_6 + (-4)) = (s32) ((s32) (temp_t7 * func_800A0140(0x2AA)) >> 0xC);
        if ((u32) var_s0_6 < (u32) &spAC) {
            goto loop_43;
        }
        temp_t2_3 = (s32) (spB4 + spAC) >> 1;
        temp_t5_4 = (s32) (spB8 + spB0) >> 1;
        var_s2_3 = 0;
        var_a0_3 = 0;
loop_45:
        temp_t6 = var_a0_3 * 8;
        temp_t1_2 = &sp9C + temp_t6;
        temp_v0_5 = temp_t1_2->unk0;
        var_s2_3 += 1;
        if ((temp_v0_5 > 0) || (*(s32 *)((unsigned char *)temp_t1_2 + (4)) > 0)) {
            temp_v1_4 = &spAC + temp_t6;
            var_t0_2 = temp_v1_4->unk0;
            if (temp_t2_3 < var_t0_2) {
                var_t0_2 = var_t0_2 + temp_v0_5 + temp_t2_3;
                temp_v1_4->unk0 = var_t0_2;
            }
            if (var_t0_2 < temp_t2_3) {
                temp_v1_4->unk0 = (s32) ((var_t0_2 - temp_v0_5) + temp_t2_3);
            }
            var_v0_4 = *(s32 *)((unsigned char *)temp_v1_4 + (4));
            var_at_3 = var_v0_4 < temp_t5_4;
            if (temp_t5_4 < var_v0_4) {
                var_v0_4 = var_v0_4 + *(s32 *)((unsigned char *)temp_t1_2 + (4)) + temp_t5_4;
                *(s32 *)((unsigned char *)temp_v1_4 + (4)) = var_v0_4;
                var_at_3 = var_v0_4 < temp_t5_4;
            }
            var_at_4 = var_s2_3 < 2;
            if (var_at_3 != 0) {
                *(s32 *)((unsigned char *)temp_v1_4 + (4)) = (s32) ((var_v0_4 - *(s32 *)((unsigned char *)temp_t1_2 + (4))) + temp_t5_4);
                goto block_55;
            }
        } else {
block_55:
            var_at_4 = var_s2_3 < 2;
        }
        var_a0_3 = var_s2_3;
        if (var_at_4 != 0) {
            goto loop_45;
        }
        var_s1_6 = &spAC;
loop_58:
        temp_t2_4 = *var_s1_6;
        var_s1_6 += 4;
        *(s32 *)((unsigned char *)var_s1_6 + (-4)) = (s32) -temp_t2_4;
        if ((u32) var_s1_6 < (u32) &spBC) {
            goto loop_58;
        }
        var_s0_7 = &sp9C;
loop_60:
        temp_t4_3 = *var_s0_7;
        var_s0_7 += 4;
        *(s32 *)((unsigned char *)var_s0_7 + (-4)) = (s32) (temp_t4_3 - 0x18);
        if ((u32) var_s0_7 < (u32) &spAC) {
            goto loop_60;
        }
        var_s1_7 = &spAC;
loop_62:
        temp_t7_2 = *var_s1_7;
        var_s1_7 += 4;
        *(s32 *)((unsigned char *)var_s1_7 + (-4)) = (s32) (temp_t7_2 - ((s16) *D_80149984 >> 2));
        if ((u32) var_s1_7 < (u32) &spBC) {
            goto loop_62;
        }
        var_s2_4 = 0;
        var_a0_4 = 0;
loop_64:
        temp_t9 = var_a0_4 * 8;
        temp_v1_5 = &spAC + temp_t9;
        temp_t0_2 = temp_v1_5->unk0;
        if ((temp_t0_2 < -0xA0) || (var_t1_2 = &sp9C + temp_t9, ((temp_t0_2 < 0xA1) == 0)) || (var_v0_5 = *var_t1_2, ((var_v0_5 < -0x17) != 0)) || (var_v0_5 >= 0x79)) {
            temp_v0_6 = *(s32 *)((unsigned char *)temp_v1_5 + (4));
            if (temp_v0_6 >= -0xA0) {
                var_t1_2 = &sp9C + temp_t9;
                if (temp_v0_6 < 0xA1) {
                    temp_v0_7 = *(s32 *)((unsigned char *)var_t1_2 + (4));
                    if ((temp_v0_7 >= -0x17) && (temp_v0_7 < 0x79)) {
                        var_v0_5 = var_t1_2->unk0;
                        goto block_73;
                    }
                }
            }
        } else {
block_73:
            func_80049FF0((s32) (s16) temp_t0_2, (s32) (s16) var_v0_5, (s32) (s16) *(s32 *)((unsigned char *)temp_v1_5 + (4)), (s32) *(s16 *)((unsigned char *)var_t1_2 + (4)), 0xFFFFFF, 0xFFFFFF, 0xFA);
        }
        var_s2_4 += 1;
        var_a0_4 = var_s2_4;
        if (var_s2_4 != 2) {
            goto loop_64;
        }
        temp_v0_4 = sp78 + 0x10;
        sp78 = temp_v0_4;
    } while (temp_v0_4 != 0x400);
}
