/* NON_MATCHING: T-24031, source-only paired particle-handler draft; see README for blockers and type view. */
s32 func_80144B70(void) {
    GyoziWork *state = &D_800F53A0;
    Rec24 *temp_s0_2;
    s16 *var_v1;
    s16 temp_v0;
    s16 var_s2;
    s16 var_v1_2;
    s32 temp_s6;
    s32 temp_t7;
    s32 temp_v1;
    s8 temp_t8;
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
    } else if (((u32) D_80148EB0 >= 0xC9U) && (((u32) (u32)D_80148EB0 % 20U) == 0) && (D_80148EAC < 0x32)) {
        D_80148EAC = D_80148EAC + 1;
        temp_t8 = (func_800BCE40() % 2) * 8;
        D_8012CA40[D_80148EAC + 3].unk_0E = temp_t8;
        if ((temp_t8 & 0xFF) == 8) {
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
    if (D_8012E694 == 0) {
        var_v1_2 = D_80148EAC;
    } else {
        var_v1_2 = 0xA;
        for (var_s2 = 14; var_s2 < 54; var_s2++) {
            D_8012CA40[var_s2].unk_00 |= 0x80000000;
        }
    }
    temp_s6 = var_v1_2 + 4;
    var_s2 = 4;
    if (temp_s6 >= 5) {
        do {
            temp_v0_2 = D_801317EB & 0xFF;
            temp_s0_2 = &D_8012CA40[var_s2];
            temp_s0_2->unk_15 = temp_v0_2;
            temp_s0_2->unk_14 = temp_v0_2;
            temp_s0_2->unk_16 = D_801317EB;
            temp_s0_2->unk_00 &= 0x7FFFFFFF;
            if (((u32)D_80148EB0 % (u32) D_80148E48[var_s2 - 4]) == 0) {
                temp_s0_2->unk_06 += 1;
            }
            temp_v1 = D_80148BF0[var_s2 - 4];
            temp_t7 = D_80148CB8[var_s2 - 4] + temp_v1;
            D_80148CB8[var_s2 - 4] = temp_t7;
            D_80148BF0[var_s2 - 4] = (s32) (temp_v1 + ((s32) (D_80148D80[var_s2 - 4] - temp_t7) >> 8));
            temp_s0_2->unk_04 = ((Fix32 *)D_80148CB8)[var_s2 - 4].h[1];
            if (temp_s0_2->unk_06 >= 0x29) {
                temp_v1 = func_800BCE40() % 320;
                ((Fix32 *)D_80148CB8)[var_s2 - 4].h[0] = 0;
                temp_s0_2->unk_06 = -0x78;
                temp_s0_2->unk_04 = temp_v1 - 0x9F;
                ((Fix32 *)D_80148CB8)[var_s2 - 4].h[1] = (s16) temp_s0_2->unk_04;
                D_80148D80[var_s2 - 4] = (s32) D_80148CB8[var_s2 - 4];
                temp_s0_2->unk_0E = (func_800BCE40() % 2) * 8;
                if (D_8012E694 == 1) {
                    temp_s0_2->unk_0E = 0;
                }
                if (temp_s0_2->unk_0E == 8) {
                    state->unk_F30[0x145 + var_s2] = 0xC;
                    D_80148E48[var_s2 - 4] = (s16) ((func_800BCE40() % 2) + 3);
                } else {
                    state->unk_F30[0x145 + var_s2] = 9;
                    D_80148E48[var_s2 - 4] = (s16) ((func_800BCE40() % 2) + 1);
                }
                temp_v0 = D_80148E48[var_s2 - 4];
                D_80148BF0[var_s2 - 4] = (s32) (0x10000 / (s32) (((temp_v0 >= 3) * 2) + temp_v0 + 1));
            }
            var_s2 += 1;
        } while (var_s2 < temp_s6);
    }
    D_80148EB0 += 1;
}
