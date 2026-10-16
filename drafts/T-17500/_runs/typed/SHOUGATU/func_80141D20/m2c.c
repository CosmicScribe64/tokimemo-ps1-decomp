extern u32 D_80146800;

void func_80141D20(void) {
    Rec24 *temp_s0;
    s16 *temp_s5;
    s16 *var_v1;
    s16 temp_v0;
    s16 temp_v0_3;
    s16 var_a1;
    s16 var_s2;
    s32 *temp_s1;
    s32 *temp_s3;
    s32 *temp_s4;
    s32 temp_hi;
    s32 temp_t4;
    s32 temp_v1;
    s32 var_a0;
    s8 temp_t0;
    u8 temp_t9;
    u8 temp_v0_2;

    var_a1 = D_801467FC;
    if (var_a1 == 0) {
        return;
    }
    if (D_80146800 == 0x1E) {
        D_80121864 = 0xB4;
        var_a1 += 1;
        *D_80146798 = 1;
    } else if (D_80146800 == 0x5A) {
        var_a1 += 1;
        D_80121864 = 0x28;
    } else if (((u16) D_800CA150 >= 6U) && (((u32) D_80146800 % 20U) == 0) && (var_a1 < 0x32)) {
        D_801467FC = var_a1 + 1;
        temp_t0 = (func_800AE0D0() % 2) * 8;
        *(s8 *)((unsigned char *)(&D_801217D0[D_801467FC]) + (122)) = temp_t0;
        if ((temp_t0 & 0xFF) == 8) {
            *(s8 *)((unsigned char *)(&D_800E6280 + D_801467FC) + (4264)) = 0xC;
            D_801467FC = D_801467FC;
            var_a1 = D_801467FC;
            var_v1 = &D_80146798[var_a1];
            *(s16 *)((unsigned char *)var_v1 + (-2)) = (s16) ((func_800AE0D0() % 2) + 3);
        } else {
            *(s8 *)((unsigned char *)(&D_800E6280 + D_801467FC) + (4264)) = 9;
            D_801467FC = D_801467FC;
            var_a1 = D_801467FC;
            var_v1 = &D_80146798[var_a1];
            *(s16 *)((unsigned char *)var_v1 + (-2)) = (s16) ((func_800AE0D0() % 2) + 1);
        }
        temp_v0 = *(s16 *)((unsigned char *)var_v1 + (-2));
        *(&D_8014653C + (var_a1 * 4)) = 0x10000 / (s32) (((temp_v0 >= 3) * 2) + temp_v0 + 1);
    }
    D_801467FC = var_a1;
    var_a0 = var_a1 + 4;
    var_s2 = 4;
    if (var_a0 >= 5) {
        do {
            temp_s5 = &D_80146798[var_s2];
            temp_s0 = &D_801217D0[var_s2];
            temp_v0_2 = D_800B593C & 0xFF;
            temp_s0->unk_15 = temp_v0_2;
            temp_s0->unk_14 = temp_v0_2;
            temp_s0->unk_00 &= 0x7FFFFFFF;
            temp_s4 = &D_80146540[var_s2];
            temp_s1 = &D_80146608[var_s2];
            temp_s3 = &D_801466D0[var_s2];
            temp_s0->unk_16 = D_800B593C;
            if ((D_80146800 % (u32) *(s16 *)((unsigned char *)temp_s5 + (-8))) == 0) {
                temp_s0->unk_06 += 1;
            }
            temp_v1 = *(s32 *)((unsigned char *)temp_s4 + (-16));
            temp_t4 = *(s32 *)((unsigned char *)temp_s1 + (-16)) + temp_v1;
            *(s32 *)((unsigned char *)temp_s1 + (-16)) = temp_t4;
            *(s32 *)((unsigned char *)temp_s4 + (-16)) = (s32) (temp_v1 + ((s32) (*(s32 *)((unsigned char *)temp_s3 + (-16)) - temp_t4) >> 8));
            temp_s0->unk_04 = *(s16 *)((unsigned char *)temp_s1 + (-14));
            if (temp_s0->unk_06 >= 0x29) {
                temp_hi = func_800AE0D0() % 320;
                *(s32 *)((unsigned char *)temp_s1 + (-16)) = 0;
                temp_s0->unk_06 = -0x78;
                temp_s0->unk_04 = temp_hi - 0x9F;
                *(s16 *)((unsigned char *)temp_s1 + (-14)) = (s16) temp_s0->unk_04;
                *(s32 *)((unsigned char *)temp_s3 + (-16)) = (s32) *(s32 *)((unsigned char *)temp_s1 + (-16));
                temp_t9 = (func_800AE0D0() % 2) * 8;
                temp_s0->unk_0E = temp_t9;
                if ((temp_t9 & 0xFF) == 8) {
                    D_800E6280.unk_10A5[var_s2] = 0xC;
                    *(s16 *)((unsigned char *)temp_s5 + (-8)) = (s16) ((func_800AE0D0() % 2) + 3);
                } else {
                    D_800E6280.unk_10A5[var_s2] = 9;
                    *(s16 *)((unsigned char *)temp_s5 + (-8)) = (s16) ((func_800AE0D0() % 2) + 1);
                }
                temp_v0_3 = *(s16 *)((unsigned char *)temp_s5 + (-8));
                var_a0 = D_801467FC + 4;
                *(s32 *)((unsigned char *)temp_s4 + (-16)) = (s32) (0x10000 / (s32) (((temp_v0_3 >= 3) * 2) + temp_v0_3 + 1));
            }
            var_s2 += 1;
        } while (var_s2 < var_a0);
    }
    D_80146800 += 1;
}
