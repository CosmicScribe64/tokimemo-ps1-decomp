void func_8013DDB8(void) {
    s16 *temp_a0;
    s16 *temp_a0_2;
    s16 *temp_a1;
    s16 *temp_a2;
    s16 *temp_a2_2;
    s16 *temp_t3_2;
    s16 *temp_t5_2;
    s16 *temp_v1_2;
    s16 temp_ra;
    s16 temp_t1;
    s16 temp_t2;
    s16 temp_t3;
    s16 temp_t4;
    s16 temp_t5;
    s16 temp_t6;
    s16 temp_t9;
    s32 temp_a3;
    s32 temp_a3_2;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_s0_3;
    s32 temp_s1;
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_s4;
    s32 temp_s6;
    s32 temp_s6_2;
    s32 temp_t0;
    s32 temp_t0_2;
    s32 temp_t1_2;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v1;
    s32 var_s3;
    s32 var_s5;
    s32 var_s5_2;
    s32 var_v1;

    switch (D_80122EC4) {
    case 0:
    case 3:
    case 6:
        temp_s0 = D_801499C0.word;
        D_801499C0.word = temp_s0 >> 1;
        func_8013E1FC();
        D_80122DD4 = D_801499C0.halves.high - 0x100;
        if (D_80122EC4 == 6) {
            temp_v1 = (s16) D_801499C0.halves.high >> 1;
            temp_t6 = (D_801499C0.halves.high * 0x10 * 3) + 0x1000;
            D_80120804 = temp_t6;
            D_8012080E = (-((s16) D_80149984 >> 2) - temp_v1) - 0x28;
            D_80120812 = (-((s16) D_801499C0.halves.high >> 4) - temp_v1) - 0x68;
            D_80120802 = temp_t6;
            if (D_801499C0.halves.high >= 0x100) {
                D_8014996C = 0;
                D_80149960 = 0;
                D_80122EC4 = 0;
                func_80042808();
            }
        }
        D_801499C0.word = temp_s0;
        if (D_80122DD4 >= 0x100) {
            var_s5 = 0;
            do {
                temp_s0_2 = var_s5 * 4;
                temp_a3 = temp_s0_2 + 0x50;
                temp_s6 = var_s5 + 1;
                temp_t0 = (temp_s6 * 4) + 0x50;
                temp_v1_2 = &D_80149B24[temp_s0_2];
                temp_a1 = &D_80149C64[temp_s0_2];
                temp_a2 = &D_80149D04[temp_s0_2];
                (*(s32 *)temp_v1_2) = -0xA0;
                temp_a0 = &D_80149BC4[temp_s0_2];
                (*(s32 *)temp_a1) = -0xA0;
                temp_t3 = (temp_a3 * 2) - 0xA0;
                temp_t4 = (temp_t0 * 2) - 0xA0;
                temp_ra = (3 * temp_t0) - 0xA0;
                temp_t9 = (temp_t0 * 4) - 0xA0;
                temp_t5 = (3 * temp_a3) - 0xA0;
                temp_t1 = temp_a3 - 0xA0;
                temp_t2 = temp_t0 - 0xA0;
                var_s5 = temp_s6;
                *(s16 *)((unsigned char *)temp_a1 + (2)) = temp_t2;
                *(s16 *)((unsigned char *)temp_v1_2 + (2)) = temp_t1;
                *(s16 *)((unsigned char *)temp_a0 + (4)) = temp_t5;
                *(s16 *)((unsigned char *)temp_v1_2 + (6)) = temp_t5;
                *(s16 *)((unsigned char *)temp_a2 + (6)) = temp_t9;
                *(s16 *)((unsigned char *)temp_a2 + (4)) = temp_ra;
                *(s16 *)((unsigned char *)temp_a1 + (6)) = temp_ra;
                *(s16 *)((unsigned char *)temp_a0 + (6)) = (s16) ((temp_a3 * 4) - 0xA0);
                *(s16 *)((unsigned char *)temp_a2 + (2)) = temp_t4;
                *(s16 *)((unsigned char *)temp_a1 + (4)) = temp_t4;
                *(s16 *)((unsigned char *)temp_a0 + (2)) = temp_t3;
                *(s16 *)((unsigned char *)temp_v1_2 + (4)) = temp_t3;
                (*(s32 *)temp_a0) = temp_t1;
                (*(s32 *)temp_a2) = temp_t2;
            } while (temp_s6 != 0x14);
            var_s3 = D_80122EC4 + 1;
            D_801499C0.halves.high -= 0x200;
block_19:
            D_80122EC4 = var_s3;
        }
        break;
    case 1:
    case 4:
        func_8013E894();
        temp_v0 = (D_801499C0.halves.high * -4) + 0x800;
        D_80122DD8 = temp_v0;
        if (temp_v0 < -0x3FF) {
            if (D_80122EC4 == 4) {
                D_8012080E = 0xB0;
                D_801207EB |= 0x80;
            }
            var_s3 = D_80122EC4 + 1;
            goto block_19;
        }
        break;
    case 2:
    case 5:
        func_8013EC60();
        temp_v0_2 = (D_801499C0.halves.high * -4) + 0x800;
        D_80122DD8 = temp_v0_2;
        var_s5_2 = 0;
        if (temp_v0_2 < -0x7FF) {
            do {
                temp_s0_3 = var_s5_2 * 4;
                temp_v0_3 = var_s5_2 * 8;
                temp_s6_2 = var_s5_2 + 1;
                temp_s4 = -0x54 - temp_s0_3;
                temp_s3 = -0xA8 - temp_v0_3;
                temp_s2 = -0x50 - temp_s0_3;
                temp_s1 = -0xA0 - temp_v0_3;
                temp_t0_2 = (temp_s6_2 * 4) + 0x50;
                temp_a3_2 = temp_s0_3 + 0x50;
                var_v1 = 0;
loop_16:
                temp_lo = temp_a3_2 * var_v1;
                temp_t1_2 = var_v1 + 1;
                temp_v0_4 = temp_s0_3 + var_v1;
                temp_a0_2 = &D_80149B24[temp_v0_4];
                temp_a2_2 = &D_80149BC4[temp_v0_4];
                temp_t3_2 = &D_80149C64[temp_v0_4];
                (*(s32 *)temp_a0_2) = temp_s1 + temp_lo;
                temp_lo_2 = temp_t0_2 * var_v1;
                (*(s32 *)temp_a2_2) = temp_s2 + temp_lo;
                temp_t5_2 = &D_80149D04[temp_v0_4];
                var_v1 += 2;
                temp_lo_3 = temp_a3_2 * temp_t1_2;
                (*(s32 *)temp_t3_2) = temp_s3 + temp_lo_2;
                (*(s32 *)temp_t5_2) = temp_s4 + temp_lo_2;
                temp_lo_4 = temp_t0_2 * temp_t1_2;
                *(s16 *)((unsigned char *)temp_a2_2 + (2)) = (s16) (temp_s2 + temp_lo_3);
                *(s16 *)((unsigned char *)temp_a0_2 + (2)) = (s16) (temp_s1 + temp_lo_3);
                *(s16 *)((unsigned char *)temp_t5_2 + (2)) = (s16) (temp_s4 + temp_lo_4);
                *(s16 *)((unsigned char *)temp_t3_2 + (2)) = (s16) (temp_s3 + temp_lo_4);
                if (var_v1 != 4) {
                    goto loop_16;
                }
                var_s5_2 = temp_s6_2;
            } while (temp_s6_2 != 0x14);
            *D_80122DD0 = 0x118;
            D_80122DD4 = -0x100;
            D_80122DD8 = 0;
            D_801499C0.word = 0;
            var_s3 = D_80122EC4 + 1;
            goto block_19;
        }
        break;
    }
}
