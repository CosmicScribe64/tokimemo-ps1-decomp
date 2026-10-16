void func_80135ACC(s32 arg0) {
    s32 sp68;
    s32 sp64;
    s32 *sp54;
    EnResetSlot8 *temp_v0_2;
    EnResetSlot8 *temp_v0_3;
    EnResetSlot8 *temp_v0_4;
    EnResetSlot8 *temp_v0_5;
    s16 *temp_s0;
    s16 var_fp;
    s16 var_s5;
    s32 *var_t0;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_a0_4;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_a1_3;
    s32 temp_t1;
    s32 temp_v0;
    s32 var_s1;
    s32 var_s2;

    var_t0 = &D_80139278[arg0];
    temp_s0 = D_80139228[arg0];
    var_s2 = 0;
    var_fp = -0x19;
    do {
        var_s1 = 0;
        var_s5 = -0x19;
loop_2:
        *(s16 *)((unsigned char *)(&sp68) + (2)) = var_s5;
        *(s16 *)((unsigned char *)(&sp64) + (2)) = var_fp;
        sp54 = var_t0;
        temp_v0 = func_80132A74((s32) temp_s0[1], (s32) temp_s0[3], (s32) *(s16 *)((unsigned char *)(&sp68) + (2)), (s32) *(s16 *)((unsigned char *)(&sp64) + (2)));
        temp_t1 = *var_t0 * 5;
        temp_a0 = temp_t1 + 5;
        if (temp_v0 >= (temp_t1 * temp_t1)) {
            if ((temp_a0 * temp_a0) >= temp_v0) {
                temp_v0_2 = &D_801392AC[var_s1][var_s2];
                temp_v0_2->unk0 = (s32) (temp_v0_2->unk0 + ((s32) ((sp68 - (*(s32 *)temp_s0)) * 2) / temp_t1));
                *(s32 *)((unsigned char *)temp_v0_2 + (4)) = (s32) (*(s32 *)((unsigned char *)temp_v0_2 + (4)) + ((s32) ((sp64 - *(s32 *)((unsigned char *)temp_s0 + (4))) * 2) / temp_t1));
            } else {
                temp_a1 = temp_t1 + 0xA;
                temp_a0_2 = temp_t1 + 0xF;
                if ((temp_a1 * temp_a1) < temp_v0) {
                    if ((temp_a0_2 * temp_a0_2) >= temp_v0) {
                        temp_v0_3 = &D_801392AC[var_s1][var_s2];
                        temp_v0_3->unk0 = (s32) (temp_v0_3->unk0 - ((s32) ((sp68 - (*(s32 *)temp_s0)) * 3) / temp_a1));
                        *(s32 *)((unsigned char *)temp_v0_3 + (4)) = (s32) (*(s32 *)((unsigned char *)temp_v0_3 + (4)) - ((s32) ((sp64 - *(s32 *)((unsigned char *)temp_s0 + (4))) * 3) / temp_a1));
                    } else {
                        temp_a1_2 = temp_t1 + 0x14;
                        temp_a0_3 = temp_t1 + 0x19;
                        if ((temp_a1_2 * temp_a1_2) < temp_v0) {
                            if ((temp_a0_3 * temp_a0_3) >= temp_v0) {
                                temp_v0_4 = &D_801392AC[var_s1][var_s2];
                                temp_v0_4->unk0 = (s32) (temp_v0_4->unk0 + ((s32) ((sp68 - (*(s32 *)temp_s0)) * 3) / temp_a1_2));
                                *(s32 *)((unsigned char *)temp_v0_4 + (4)) = (s32) (*(s32 *)((unsigned char *)temp_v0_4 + (4)) + ((s32) ((sp64 - *(s32 *)((unsigned char *)temp_s0 + (4))) * 3) / temp_a1_2));
                            } else {
                                temp_a1_3 = temp_t1 + 0x1E;
                                temp_a0_4 = temp_t1 + 0x23;
                                if (((temp_a1_3 * temp_a1_3) < temp_v0) && ((temp_a0_4 * temp_a0_4) >= temp_v0)) {
                                    temp_v0_5 = &D_801392AC[var_s1][var_s2];
                                    temp_v0_5->unk0 = (s32) (temp_v0_5->unk0 - ((s32) ((sp68 - (*(s32 *)temp_s0)) * 2) / temp_a1_3));
                                    *(s32 *)((unsigned char *)temp_v0_5 + (4)) = (s32) (*(s32 *)((unsigned char *)temp_v0_5 + (4)) - ((s32) ((sp64 - *(s32 *)((unsigned char *)temp_s0 + (4))) * 2) / temp_a1_3));
                                }
                            }
                        }
                    }
                }
            }
        }
        var_s1 += 1;
        var_s5 += 0x19;
        if (var_s1 != 0xF) {
            goto loop_2;
        }
        var_s2 += 1;
        var_fp += 0x19;
    } while (var_s2 != 0xC);
}
