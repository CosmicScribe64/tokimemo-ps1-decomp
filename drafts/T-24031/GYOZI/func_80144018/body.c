/* NON_MATCHING: T-24031, source-only paired particle-handler draft; see README for blockers and type view. */
s32 func_80144018(void) {
    GyoziWork *state = &D_800F53A0;
    Rec24 *temp_s0;
    s16 *temp_s5;
    s16 *var_v1;
    s16 temp_v0;
    s16 temp_v0_3;
    s16 var_s2;
    Fix32 *temp_s1;
    s32 *temp_s3;
    s32 *temp_s4;
    s32 temp_hi;
    s32 temp_t4;
    s32 temp_v1;
    s32 var_a0;
    s8 temp_t0;
    u8 temp_t9;
    u8 temp_v0_2;

    if (D_80148EAC == 0) {
        return 0;
    }
    if (D_80148EB0 == 0x1E) {
        D_8012CA40[4].unk_04 = 0xB4;
        D_80148EAC += 1;
        *D_80148E48 = 1;
    } else if (D_80148EB0 == 0x5A) {
        D_80148EAC += 1;
        D_8012CA40[4].unk_04 = 0x28;
    } else if (((u16) D_800CA150 >= 6U) && (((u32) (u32)D_80148EB0 % 20U) == 0) && (D_80148EAC < 0x32)) {
        D_80148EAC = D_80148EAC + 1;
        temp_t0 = (func_800BCE40() % 2) * 8;
        D_8012CA40[D_80148EAC + 3].unk_0E = temp_t0;
        if ((temp_t0 & 0xFF) == 8) {
            state->unk_F30[0x145 + D_80148EAC + 3] = 0xC;
            var_v1 = &D_80148E48[D_80148EAC];
            var_v1[-1] = (s16) ((func_800BCE40() % 2) + 3);
        } else {
            state->unk_F30[0x145 + D_80148EAC + 3] = 9;
            var_v1 = &D_80148E48[D_80148EAC];
            var_v1[-1] = (s16) ((func_800BCE40() % 2) + 1);
        }
        temp_v0 = var_v1[-1];
        D_80148BF0[D_80148EAC - 1] = 0x10000 / (s32) (((temp_v0 >= 3) * 2) + temp_v0 + 1);
    }
    var_a0 = D_80148EAC + 4;
    var_s2 = 4;
    if (var_a0 >= 5) {
        do {
            temp_s5 = &D_80148E48[var_s2 - 4];
            temp_s0 = &D_8012CA40[var_s2];
            temp_v0_2 = D_801317EB & 0xFF;
            temp_s0->unk_15 = temp_v0_2;
            temp_s0->unk_14 = temp_v0_2;
            temp_s0->unk_00 &= 0x7FFFFFFF;
            temp_s4 = &D_80148BF0[var_s2 - 4];
            temp_s1 = (Fix32 *)&D_80148CB8[var_s2 - 4];
            temp_s3 = &D_80148D80[var_s2 - 4];
            temp_s0->unk_16 = D_801317EB;
            if (((u32)D_80148EB0 % (u32) *temp_s5) == 0) {
                temp_s0->unk_06 += 1;
            }
            temp_v1 = *temp_s4;
            temp_t4 = temp_s1->w + temp_v1;
            temp_s1->w = temp_t4;
            *temp_s4 = (s32) (temp_v1 + ((s32) (*temp_s3 - temp_t4) >> 8));
            temp_s0->unk_04 = temp_s1->h[1];
            if (temp_s0->unk_06 >= 0x29) {
                temp_hi = func_800BCE40() % 320;
                temp_s1->h[0] = 0;
                temp_s0->unk_06 = -0x78;
                temp_s0->unk_04 = temp_hi - 0x9F;
                temp_s1->h[1] = (s16) temp_s0->unk_04;
                *temp_s3 = (s32) temp_s1->w;
                temp_t9 = (func_800BCE40() % 2) * 8;
                temp_s0->unk_0E = temp_t9;
                if ((temp_t9 & 0xFF) == 8) {
                    state->unk_F30[0x145 + var_s2] = 0xC;
                    *temp_s5 = (s16) ((func_800BCE40() % 2) + 3);
                } else {
                    state->unk_F30[0x145 + var_s2] = 9;
                    *temp_s5 = (s16) ((func_800BCE40() % 2) + 1);
                }
                temp_v0_3 = *temp_s5;
                var_a0 = D_80148EAC + 4;
                *temp_s4 = (s32) (0x10000 / (s32) (((temp_v0_3 >= 3) * 2) + temp_v0_3 + 1));
            }
            var_s2 += 1;
        } while (var_s2 < var_a0);
    }
    D_80148EB0 += 1;
}
