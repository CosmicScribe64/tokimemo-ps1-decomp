s32 func_80144B70(void) {
    Rec24 *temp_s0_2;
    s16 *temp_s4;
    s16 *var_v1;
    s16 temp_a0;
    s16 temp_s2;
    s16 temp_v0;
    s16 temp_v0_3;
    s16 var_a0;
    s16 var_s2;
    s16 var_s2_2;
    s16 var_v1_2;
    s32 *temp_s0;
    s32 *temp_s1;
    s32 *temp_s3;
    s32 *temp_s5;
    s32 temp_at;
    s32 temp_hi;
    s32 temp_lo;
    s32 temp_s6;
    s32 temp_t7;
    s32 temp_v1;
    s8 temp_t8;
    u8 temp_v0_2;

    var_a0 = D_80148EAC;
    if (var_a0 == 0) {
        return;
    }
    if (D_80148EB0 == 0x1E) {
        D_8012CAD4 = 0xB4;
        var_a0 += 1;
        *D_80148E48 = 1;
    } else if (D_80148EB0 == 0x5A) {
        var_a0 += 1;
        D_8012CAD4 = 0x28;
    } else if (((u32) D_80148EB0 >= 0xC9U) && (((u32) D_80148EB0 % 20U) == 0) && (var_a0 < 0x32)) {
        D_80148EAC = var_a0 + 1;
        temp_a0 = D_80148EAC;
        temp_t8 = (func_800BCE40(var_a0, D_80148EB0) % 2) * 8;
        *(s8 *)((unsigned char *)(&D_8012CA40[temp_a0]) + (122)) = temp_t8;
        if ((temp_t8 & 0xFF) == 8) {
            *(&D_800F6418 + temp_a0) = 0xC;
            D_80148EAC = temp_a0;
            var_a0 = D_80148EAC;
            var_v1 = &D_80148E48[var_a0];
            *(s16 *)((unsigned char *)var_v1 + (-2)) = (s16) ((func_800BCE40(temp_a0) % 2) + 3);
        } else {
            *(&D_800F6418 + temp_a0) = 9;
            D_80148EAC = temp_a0;
            var_a0 = D_80148EAC;
            var_v1 = &D_80148E48[var_a0];
            *(s16 *)((unsigned char *)var_v1 + (-2)) = (s16) ((func_800BCE40(temp_a0) % 2) + 1);
        }
        temp_v0 = *(s16 *)((unsigned char *)var_v1 + (-2));
        *(&D_80148BEC + (var_a0 * 4)) = 0x10000 / (s32) (((temp_v0 >= 3) * 2) + temp_v0 + 1);
    }
    if (D_8012E694 == 0) {
        D_80148EAC = var_a0;
        var_v1_2 = var_a0;
    } else {
        D_80148EAC = var_a0;
        var_v1_2 = 0xA;
        var_s2 = 0xE;
        do {
            temp_lo = var_s2 * 0x24;
            temp_s2 = var_s2 + 1;
            temp_at = temp_s2 < 0x36;
            var_s2 = temp_s2;
            temp_s0 = D_8012CA40 + temp_lo;
            *temp_s0 |= 0x80000000;
        } while (temp_at != 0);
    }
    temp_s6 = var_v1_2 + 4;
    var_s2_2 = 4;
    if (temp_s6 >= 5) {
        do {
            temp_s4 = &D_80148E48[var_s2_2];
            temp_v0_2 = (u8) D_801317EB & 0xFF;
            temp_s0_2 = &D_8012CA40[var_s2_2];
            temp_s0_2->unk_15 = temp_v0_2;
            temp_s0_2->unk_14 = temp_v0_2;
            temp_s0_2->unk_16 = (u8) D_801317EB;
            temp_s0_2->unk_00 &= 0x7FFFFFFF;
            temp_s5 = &D_80148BF0[var_s2_2];
            temp_s1 = &D_80148CB8[var_s2_2];
            temp_s3 = &D_80148D80[var_s2_2];
            if (((u32) D_80148EB0 % (u32) *(s16 *)((unsigned char *)temp_s4 + (-8))) == 0) {
                temp_s0_2->unk_06 += 1;
            }
            temp_v1 = *(s32 *)((unsigned char *)temp_s5 + (-16));
            temp_t7 = *(s32 *)((unsigned char *)temp_s1 + (-16)) + temp_v1;
            *(s32 *)((unsigned char *)temp_s1 + (-16)) = temp_t7;
            *(s32 *)((unsigned char *)temp_s5 + (-16)) = (s32) (temp_v1 + ((s32) (*(s32 *)((unsigned char *)temp_s3 + (-16)) - temp_t7) >> 8));
            temp_s0_2->unk_04 = *(s16 *)((unsigned char *)temp_s1 + (-14));
            if (temp_s0_2->unk_06 >= 0x29) {
                temp_hi = func_800BCE40() % 320;
                *(s32 *)((unsigned char *)temp_s1 + (-16)) = 0;
                temp_s0_2->unk_06 = -0x78;
                temp_s0_2->unk_04 = temp_hi - 0x9F;
                *(s16 *)((unsigned char *)temp_s1 + (-14)) = (s16) temp_s0_2->unk_04;
                *(s32 *)((unsigned char *)temp_s3 + (-16)) = (s32) *(s32 *)((unsigned char *)temp_s1 + (-16));
                temp_s0_2->unk_0E = (func_800BCE40() % 2) * 8;
                if (D_8012E694 == 1) {
                    temp_s0_2->unk_0E = 0;
                }
                if (temp_s0_2->unk_0E == 8) {
                    *(&D_800F6415 + var_s2_2) = 0xC;
                    *(s16 *)((unsigned char *)temp_s4 + (-8)) = (s16) ((func_800BCE40() % 2) + 3);
                } else {
                    *(&D_800F6415 + var_s2_2) = 9;
                    *(s16 *)((unsigned char *)temp_s4 + (-8)) = (s16) ((func_800BCE40() % 2) + 1);
                }
                temp_v0_3 = *(s16 *)((unsigned char *)temp_s4 + (-8));
                *(s32 *)((unsigned char *)temp_s5 + (-16)) = (s32) (0x10000 / (s32) (((temp_v0_3 >= 3) * 2) + temp_v0_3 + 1));
            }
            var_s2_2 += 1;
        } while (var_s2_2 < temp_s6);
    }
    D_80148EB0 = (u32) D_80148EB0 + 1;
}
