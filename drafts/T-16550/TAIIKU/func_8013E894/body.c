void func_8013E894(void) {
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
    s32 *var_s0;
    s32 *var_s0_2;
    s32 *var_s0_3;
    s32 *var_v1;
    s32 *var_v1_2;
    s32 *var_v1_3;
    s32 *var_v1_4;
    s32 temp_t1;
    s32 temp_t3;
    s32 temp_t4;
    s32 temp_t4_2;
    s32 temp_t4_3;
    s32 temp_t5;
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_t7_2;
    s32 temp_t8;
    s32 temp_t9;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_at;
    s32 var_at_2;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_t1;
    s32 var_v0;
    s32 var_v0_2;
    u8 *temp_t0;
    u8 *temp_t0_2;
    u8 *temp_v1;

    sp78 = 0;
    do {
        sp94 = -0x140;
        sp90 = 0;
        Rot_2D(D_80122DD8 + sp78, &sp94, &sp90, &spAC, &sp9C);
        Rot_2D(D_80122DD8 + sp78 + 0x10, &sp94, &sp90, &spB0, &spA0);
        sp94 = -0xF0;
        sp90 = 0;
        Rot_2D(D_80122DD8 + sp78, &sp94, &sp90, &spB4, &spA4);
        Rot_2D(D_80122DD8 + sp78 + 0x10, &sp94, &sp90, &spB8, &spA8);
        var_v1 = &spAC;
        var_s0 = &sp9C;
loop_2:
        temp_t5 = *var_s0;
        var_s0 += 4;
        temp_t4 = *var_v1 + *D_80122DD0;
        var_v1 += 4;
        *(s32 *)((unsigned char *)var_v1 + (-4)) = temp_t4;
        *(s32 *)((unsigned char *)var_s0 + (-4)) = (s32) (temp_t5 + D_80122DD4);
        if ((u32) var_s0 < (u32) &spAC) {
            goto loop_2;
        }
        var_s0_2 = &sp9C;
loop_4:
        temp_t7 = *var_s0_2;
        var_s0_2 += 4;
        *(s32 *)((unsigned char *)var_s0_2 + (-4)) = (s32) ((s32) (temp_t7 * func_800A0140(0x2AA)) >> 0xC);
        if ((u32) var_s0_2 < (u32) &spAC) {
            goto loop_4;
        }
        temp_t4_2 = (s32) (spB4 + spAC) >> 1;
        temp_t7_2 = (s32) (spB8 + spB0) >> 1;
        var_s1 = 0;
        var_a0 = 0;
loop_6:
        temp_t8 = var_a0 * 8;
        temp_v1 = &sp9C + temp_t8;
        temp_v0_2 = (*(s32 *)temp_v1);
        var_s1 += 1;
        if ((temp_v0_2 > 0) || (*(s32 *)((unsigned char *)temp_v1 + (4)) > 0)) {
            temp_t0 = &spAC + temp_t8;
            var_t1 = (*(s32 *)temp_t0);
            if (temp_t4_2 < var_t1) {
                var_t1 = var_t1 + temp_v0_2 + temp_t4_2;
                (*(s32 *)temp_t0) = var_t1;
            }
            if (var_t1 < temp_t4_2) {
                (*(s32 *)temp_t0) = (s32) ((var_t1 - temp_v0_2) + temp_t4_2);
            }
            var_v0 = *(s32 *)((unsigned char *)temp_t0 + (4));
            var_at = var_v0 < temp_t7_2;
            if (temp_t7_2 < var_v0) {
                var_v0 = var_v0 + *(s32 *)((unsigned char *)temp_v1 + (4)) + temp_t7_2;
                *(s32 *)((unsigned char *)temp_t0 + (4)) = var_v0;
                var_at = var_v0 < temp_t7_2;
            }
            var_at_2 = var_s1 < 2;
            if (var_at != 0) {
                *(s32 *)((unsigned char *)temp_t0 + (4)) = (s32) ((var_v0 - *(s32 *)((unsigned char *)temp_v1 + (4))) + temp_t7_2);
                goto block_16;
            }
        } else {
block_16:
            var_at_2 = var_s1 < 2;
        }
        var_a0 = var_s1;
        if (var_at_2 != 0) {
            goto loop_6;
        }
        var_v1_2 = &spAC;
loop_19:
        temp_t4_3 = *var_v1_2;
        var_v1_2 += 4;
        *(s32 *)((unsigned char *)var_v1_2 + (-4)) = (s32) -temp_t4_3;
        if ((u32) var_v1_2 < (u32) &spBC) {
            goto loop_19;
        }
        var_s0_3 = &sp9C;
loop_21:
        temp_t6 = *var_s0_3;
        var_s0_3 += 4;
        *(s32 *)((unsigned char *)var_s0_3 + (-4)) = (s32) (temp_t6 - 0x18);
        if ((u32) var_s0_3 < (u32) &spAC) {
            goto loop_21;
        }
        var_v1_3 = &spAC;
loop_23:
        temp_t9 = *var_v1_3;
        var_v1_3 += 4;
        *(s32 *)((unsigned char *)var_v1_3 + (-4)) = (s32) (temp_t9 - ((s16) D_80149984 >> 2));
        if ((u32) var_v1_3 < (u32) &spBC) {
            goto loop_23;
        }
        var_s1_2 = 0;
        var_a0_2 = 0;
loop_25:
        temp_t3 = var_a0_2 * 8;
        temp_t0_2 = &spAC + temp_t3;
        temp_t1 = (*(s32 *)temp_t0_2);
        if ((temp_t1 < -0xA0) || (var_v1_4 = &sp9C + temp_t3, ((temp_t1 < 0xA1) == 0)) || (var_v0_2 = *var_v1_4, ((var_v0_2 < -0x17) != 0)) || (var_v0_2 >= 0x79)) {
            temp_v0_3 = *(s32 *)((unsigned char *)temp_t0_2 + (4));
            if (temp_v0_3 >= -0xA0) {
                var_v1_4 = &sp9C + temp_t3;
                if (temp_v0_3 < 0xA1) {
                    temp_v0_4 = *(s32 *)((unsigned char *)var_v1_4 + (4));
                    if ((temp_v0_4 >= -0x17) && (temp_v0_4 < 0x79)) {
                        var_v0_2 = (*(s32 *)var_v1_4);
                        goto block_34;
                    }
                }
            }
        } else {
block_34:
            func_80049FF0((s32) (s16) temp_t1, (s32) (s16) var_v0_2, (s32) (s16) *(s32 *)((unsigned char *)temp_t0_2 + (4)), (s32) *(s16 *)((unsigned char *)var_v1_4 + (4)), 0xFFFFFF, 0xFFFFFF, 0xFA);
        }
        var_s1_2 += 1;
        var_a0_2 = var_s1_2;
        if (var_s1_2 != 2) {
            goto loop_25;
        }
        temp_v0 = sp78 + 0x10;
        sp78 = temp_v0;
    } while (temp_v0 != 0x800);
}
