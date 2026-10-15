void func_80144018(void) {
    s16 *temp_s5;
    s16 *var_v1;
    s16 temp_v0;
    s16 temp_v0_4;
    s16 var_a0;
    s16 var_a1;
    s16 var_s2;
    s32 temp_hi;
    s32 temp_t4;
    s32 temp_v0_3;
    s32 temp_v1;
    s8 temp_t0;
    s8 temp_t9;
    s8 temp_v0_2;
    u8 *temp_s0;
    u8 *temp_s1;
    u8 *temp_s3;
    u8 *temp_s4;

    var_a1 = D_80148EAC;
    if (var_a1 == 0) {
        return;
    }
    if (D_80148EB0 == 0x1E) {
        D_8012CAD4 = 0xB4;
        var_a1 += 1;
        D_80148E48 = 1;
    } else if (D_80148EB0 == 0x5A) {
        var_a1 += 1;
        D_8012CAD4 = 0x28;
    } else if (((u16) D_800D9250 >= 6U) && (((u32) D_80148EB0 % 20U) == 0) && (var_a1 < 0x32)) {
        D_80148EAC = var_a1 + 1;
        temp_t0 = (func_800BCE40(var_a1, D_80148EB0) % 2) * 8;
        *(s8 *)((unsigned char *)((D_80148EAC * 0x24) + &D_8012CA40) + (122)) = temp_t0;
        if ((temp_t0 & 0xFF) == 8) {
            *(s8 *)((unsigned char *)(&D_800F53A0.unk_000[D_80148EAC]) + (4216)) = 0xC;
            D_80148EAC = D_80148EAC;
            var_a1 = D_80148EAC;
            var_v1 = &(&D_80148E48)[var_a1];
            *(s16 *)((unsigned char *)var_v1 + (-2)) = (s16) ((func_800BCE40() % 2) + 3);
        } else {
            *(s8 *)((unsigned char *)(&D_800F53A0.unk_000[D_80148EAC]) + (4216)) = 9;
            D_80148EAC = D_80148EAC;
            var_a1 = D_80148EAC;
            var_v1 = &(&D_80148E48)[var_a1];
            *(s16 *)((unsigned char *)var_v1 + (-2)) = (s16) ((func_800BCE40() % 2) + 1);
        }
        temp_v0 = *(s16 *)((unsigned char *)var_v1 + (-2));
        *(&D_80148BEC + (var_a1 * 4)) = 0x10000 / (s32) (((temp_v0 >= 3) * 2) + temp_v0 + 1);
    }
    D_80148EAC = var_a1;
    var_a0 = var_a1 + 4;
    var_s2 = 4;
    if (var_a0 >= 5) {
        do {
            temp_s5 = &(&D_80148E48)[var_s2];
            temp_s0 = (var_s2 * 0x24) + &D_8012CA40;
            temp_v0_2 = (u8) D_801317EB & 0xFF;
            *(s8 *)((unsigned char *)temp_s0 + (21)) = temp_v0_2;
            *(s8 *)((unsigned char *)temp_s0 + (20)) = temp_v0_2;
            (*(s32 *)temp_s0) = (s32) ((*(s32 *)temp_s0) & 0x7FFFFFFF);
            temp_v0_3 = var_s2 * 4;
            temp_s4 = temp_v0_3 + &D_80148BF0;
            temp_s1 = &D_80148CB8 + temp_v0_3;
            temp_s3 = temp_v0_3 + &D_80148D80;
            *(u8 *)((unsigned char *)temp_s0 + (22)) = (u8) D_801317EB;
            if ((D_80148EB0 % (u32) *(s16 *)((unsigned char *)temp_s5 + (-8))) == 0) {
                *(s16 *)((unsigned char *)temp_s0 + (6)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (6)) + 1);
            }
            temp_v1 = *(s32 *)((unsigned char *)temp_s4 + (-16));
            temp_t4 = *(s32 *)((unsigned char *)temp_s1 + (-16)) + temp_v1;
            *(s32 *)((unsigned char *)temp_s1 + (-16)) = temp_t4;
            *(s32 *)((unsigned char *)temp_s4 + (-16)) = (s32) (temp_v1 + ((s32) (*(s32 *)((unsigned char *)temp_s3 + (-16)) - temp_t4) >> 8));
            *(s16 *)((unsigned char *)temp_s0 + (4)) = (s16) *(s16 *)((unsigned char *)temp_s1 + (-14));
            if (*(s16 *)((unsigned char *)temp_s0 + (6)) >= 0x29) {
                temp_hi = func_800BCE40(var_a0, (u32) var_a1, D_80148EB0) % 320;
                *(s32 *)((unsigned char *)temp_s1 + (-16)) = 0;
                *(s16 *)((unsigned char *)temp_s0 + (6)) = -0x78;
                *(s16 *)((unsigned char *)temp_s0 + (4)) = (s16) (temp_hi - 0x9F);
                *(s16 *)((unsigned char *)temp_s1 + (-14)) = (s16) *(s16 *)((unsigned char *)temp_s0 + (4));
                *(s32 *)((unsigned char *)temp_s3 + (-16)) = (s32) *(s32 *)((unsigned char *)temp_s1 + (-16));
                temp_t9 = (func_800BCE40() % 2) * 8;
                *(s8 *)((unsigned char *)temp_s0 + (14)) = temp_t9;
                if ((temp_t9 & 0xFF) == 8) {
                    *(s8 *)((unsigned char *)(&D_800F53A0.unk_000[var_s2]) + (4213)) = 0xC;
                    *(s16 *)((unsigned char *)temp_s5 + (-8)) = (s16) ((func_800BCE40() % 2) + 3);
                } else {
                    *(s8 *)((unsigned char *)(&D_800F53A0.unk_000[var_s2]) + (4213)) = 9;
                    *(s16 *)((unsigned char *)temp_s5 + (-8)) = (s16) ((func_800BCE40() % 2) + 1);
                }
                temp_v0_4 = *(s16 *)((unsigned char *)temp_s5 + (-8));
                var_a1 = D_80148EAC;
                var_a0 = var_a1 + 4;
                *(s32 *)((unsigned char *)temp_s4 + (-16)) = (s32) (0x10000 / (s32) (((temp_v0_4 >= 3) * 2) + temp_v0_4 + 1));
            }
            var_s2 += 1;
        } while (var_s2 < var_a0);
    }
    D_80148EB0 += 1;
}
