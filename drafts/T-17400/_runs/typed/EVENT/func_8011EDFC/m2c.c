? func_8001BB10(s16, s16, ?, ?, s32, s32, s32, s32); /* extern */
? func_80046480();                                  /* extern */
? func_80046598();                                  /* extern */

void func_8011EDFC(void) {
    s16 *temp_a0;
    s16 *temp_s1_2;
    s16 *temp_s2;
    s16 *temp_v0;
    s16 temp_a1;
    s16 temp_v0_2;
    s16 var_s3;
    s32 temp_s1;
    s32 temp_s1_3;
    s32 var_at;
    u8 temp_s0;
    u8 temp_s0_2;

    switch (D_800B1AF0) {
    case 0:
        D_800EED08 = 1;
        func_8011F530(D_800B1AE4);
        func_800433D0(0x202);
        D_801258DC = 0;
        func_800352B4(0);
        func_800469F4(0x432A);
        D_801258E4->unk0 = -0x50;
        D_801258E4[1] = 0;
        D_801258E4[2] = 0x64;
        D_801258EC->unk0 = -0x28;
        D_801258EC[1] = 0;
        D_801258EC[2] = -0x3C;
        D_800B1AF0 += 1;
        D_801258F4->unk0 = (func_800789A0() % 3) - 1;
        D_801258F4[1] = (func_800789A0() % 3) - 1;
        D_801258F4[2] = (func_800789A0() % 3) - 1;
        D_801258FC->unk0 = (func_800789A0() % 3) - 1;
        D_801258FC[1] = (func_800789A0() % 3) - 1;
        D_801258FC[2] = (func_800789A0() % 3) - 1;
        return;
    case 1:
        if (func_80015A44() & 1) {
            D_800B1AF0 += 1;
            return;
        }
        return;
    case 2:
        func_8002755C(1);
        func_80027438(1);
        D_800B1AF0 += 1;
        return;
    case 3:
        temp_s0 = D_800B1AF6;
        temp_s1 = D_800B1AF0;
        func_80046480();
        D_800B1AF0 = temp_s1;
        if (temp_s0 != D_800B1AF6) {
            D_800B1AF0 = temp_s1 + 1;
            func_800433D0(0x201);
        }
        D_800B1AF6 = temp_s0;
        return;
    case 4:
        var_s3 = 0;
        do {
            if (((u32) D_800B1AE4 % 30U) == (func_800789A0() % 30)) {
                D_801258F4[var_s3] = (func_800789A0() % 3) - 1;
            }
            temp_s1_2 = &D_801258F4[var_s3];
            if (((u32) D_800B1AE4 % 30U) == (func_800789A0() % 30)) {
                D_801258FC[var_s3] = (func_800789A0() % 3) - 1;
            }
            temp_s2 = &D_801258FC[var_s3];
            if ((*temp_s1_2 == 0) && (*temp_s2 == 0)) {
                if ((func_800789A0() % 2) != 0) {
                    *temp_s1_2 = ((func_800789A0() % 2) * 2) - 1;
                } else {
                    *temp_s2 = ((func_800789A0() % 2) * 2) - 1;
                }
            }
            temp_v0 = &D_801258E4[var_s3];
            *temp_v0 += *temp_s1_2;
            temp_a0 = &D_801258EC[var_s3];
            temp_a1 = *temp_v0;
            *temp_a0 += *temp_s2;
            if (temp_a1 < -0x95) {
                *temp_s1_2 = 1;
            }
            if (temp_a1 >= 0x96) {
                *temp_s1_2 = -1;
            }
            temp_v0_2 = *temp_a0;
            var_s3 += 1;
            if (temp_v0_2 < -0x6D) {
                *temp_s2 = 1;
            }
            var_at = var_s3 < 3;
            if (temp_v0_2 >= 0x1E) {
                *temp_s2 = -1;
                var_at = var_s3 < 3;
            }
        } while (var_at != 0);
        func_8001BB10(D_801258E4->unk0, D_801258EC->unk0, 0xA, 2, 2, 0x808080, 1, 1);
        func_8001BB10(D_801258E4[1], D_801258EC[1], 0xA, 2, 2, 0x804040, 1, 1);
        func_8001BB10(D_801258E4[2], D_801258EC[2], 0xA, 2, 2, 0x800000, 1, 1);
        if ((D_801258DC++ ^ 0x12C) == 0) {
            D_800B1AF0 += 1;
            func_800433D0(0x202);
            return;
        }
        break;
    case 5:
        temp_s0_2 = D_800B1AF6;
        temp_s1_3 = D_800B1AF0;
        func_80046598();
        D_800B1AF0 = temp_s1_3;
        if (temp_s0_2 != D_800B1AF6) {
            D_800B1AF0 = temp_s1_3 + 1;
        }
        D_800B1AF6 = temp_s0_2;
        return;
    case 6:
        func_80027438(1);
        D_800B1AF0 += 1;
        return;
    default:
        func_800433D0(0x206);
        D_800EED08 = 0;
        func_800352B4(1);
        func_80011DFC();
        break;
    }
}
