void func_801365DC(s32 arg0);

s32 func_80135E50(void) {
    s16 *temp_a0;
    s16 *temp_v0;
    s16 temp_a1;
    s16 temp_v0_2;
    s16 var_s3;
    s32 remainder;
    u32 temp_s1;
    u32 temp_s1_3;
    u8 temp_s0;
    u8 temp_s0_2;

    switch (D_800E6280.unk_1104.u) {
    case 0:
        D_80122D2C = 1;
        func_800AE120((*(s32 *)&D_800E6280.unk_10F8));
        D_8013A5CC = 0;
        hizuke_disp_switch(0);
        func_801365DC(0x43BA);
        D_8013A5D4[0] = -0x50;
        D_8013A5D4[1] = 0;
        D_8013A5D4[2] = 0x64;
        D_8013A5DC[0] = -0x28;
        D_8013A5DC[1] = 0;
        D_8013A5DC[2] = -0x3C;
        D_800E6280.unk_1104.u += 1;
        D_8013A5E4[0] = (func_800AE0D0() % 3) - 1;
        D_8013A5E4[1] = (func_800AE0D0() % 3) - 1;
        D_8013A5E4[2] = (func_800AE0D0() % 3) - 1;
        D_8013A5EC[0] = (func_800AE0D0() % 3) - 1;
        D_8013A5EC[1] = (func_800AE0D0() % 3) - 1;
        D_8013A5EC[2] = (func_800AE0D0() % 3) - 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_1104.u += 1;
            break;
        }
        break;
    case 2:
        if (((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000) || ((((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n");
            ((RECT *)D_800B5920)[0].w = 0;
            ((RECT *)D_800B5920)[0].h = 0;
            D_800E6280.unk_1104.u -= 2;
            return 0;
        }
        func_8005751C(1);
        dec_bg_show_switch(1);
        func_80044750(0x203);
        D_800E6280.unk_1104.u += 1;
        break;
    case 3:
        temp_s0 = D_800E6280.unk_110A;
        temp_s1 = D_800E6280.unk_1104.u;
        normal_date_bg_fadein();
        D_800E6280.unk_1104.u = temp_s1;
        if (temp_s0 != D_800E6280.unk_110A) {
            D_800E6280.unk_1104.u = temp_s1 + 1;
        }
        D_800E6280.unk_110A = temp_s0;
        break;
    case 4:
        var_s3 = 0;
        do {
            if (((u32) (*(s32 *)&D_800E6280.unk_10F8) % 30U) == (func_800AE0D0() % 30)) {
                (D_8013A5E4)[var_s3] = (func_800AE0D0() % 3) - 1;
            }

            if (((u32) (*(s32 *)&D_800E6280.unk_10F8) % 30U) == (func_800AE0D0() % 30)) {
                (D_8013A5EC)[var_s3] = (func_800AE0D0() % 3) - 1;
            }

            if ((D_8013A5E4[var_s3] == 0) && (D_8013A5EC[var_s3] == 0)) {
                if ((remainder = func_800AE0D0() % 2)) {
                    D_8013A5E4[var_s3] = ((func_800AE0D0() % 2) * 2) - 1;
                } else {
                    D_8013A5EC[var_s3] = ((func_800AE0D0() % 2) * 2) - 1;
                }
            }
            temp_v0 = &(D_8013A5D4)[var_s3];
            *temp_v0 += D_8013A5E4[var_s3];
            temp_a0 = &(D_8013A5DC)[var_s3];
            *temp_a0 += D_8013A5EC[var_s3];
            temp_a1 = *temp_v0;
            if (temp_a1 < -0x95) {
                D_8013A5E4[var_s3] = 1;
            }
            if (temp_a1 >= 0x96) {
                D_8013A5E4[var_s3] = -1;
            }
            temp_v0_2 = *temp_a0;
            if (temp_v0_2 < -0x6D) {
                D_8013A5EC[var_s3] = 1;
            }
            if (temp_v0_2 >= 0x1E) {
                D_8013A5EC[var_s3] = -1;
            }
            var_s3++;
        } while (var_s3 < 3);
        func_8004A814((s32) D_8013A5D4[0], (s32) D_8013A5DC[0], 0xA, 2, 2, 0x808080, 3, 1);
        func_8004A814((s32) D_8013A5D4[1], (s32) D_8013A5DC[1], 0xA, 2, 2, 0x804040, 3, 1);
        func_8004A814((s32) D_8013A5D4[2], (s32) D_8013A5DC[2], 0xA, 2, 2, 0x800000, 3, 1);
        if (D_8013A5CC++ == 0x96) {
            D_800E6280.unk_1104.u += 1;
            break;
        }
        break;
    case 5:
        temp_s0_2 = D_800E6280.unk_110A;
        temp_s1_3 = D_800E6280.unk_1104.u;
        normal_date_bg_fadeout();
        D_800E6280.unk_1104.u = temp_s1_3;
        if (temp_s0_2 != D_800E6280.unk_110A) {
            D_800E6280.unk_1104.u = temp_s1_3 + 1;
        }
        D_800E6280.unk_110A = temp_s0_2;
        break;
    case 6:
        dec_bg_show_switch(1);
        D_800E6280.unk_1104.u += 1;
        break;
    default:
        func_80044750(0x200);
        D_80122D2C = 0;
        hizuke_disp_switch(1);
        func_8004284C();
        break;
    }
}
