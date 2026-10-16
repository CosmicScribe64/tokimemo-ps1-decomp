void func_80145C28(void) {
    u8 *sp3C;
    s32 *var_v0_2;
    s32 *var_v0_3;
    s16 temp_t0;
    s16 temp_t1;
    s16 temp_t4;
    s16 temp_t5;
    s16 temp_t6;
    s16 temp_t7;
    s16 temp_t8;
    s16 temp_t9;
    s32 var_s0;
    u8 *var_v0;

    Scroll(0, 0xF, 0, 0xF0, 0x280);
    switch (D_800E738A) {
    case 0:
        func_80044750(0x50B);
        Fade_In_Init();
        func_8004284C();
        break;
    case 1:
        if (Fade_In_Main() != 0) {
            if ((*D_8014A3D8 == 1) || (D_8014A3D9 == 1)) {
                func_80044750(0x205);
            } else {
                func_80044750(0x206);
            }
            func_8004284C();
        }
        break;
    case 2:
        if (func_801485C4() != 0) {
            func_8004284C();
        }
        break;
    case 3:
        if (func_80148678() != 0) {
            func_8004284C();
        }
        break;
    case 4:
        func_801487B4();
        var_v0 = D_8014A3D8;
        var_s0 = 0;
        do {
            if (*var_v0 == 1) {
                sp3C = var_v0;
                func_8013384C(0xB, var_s0 & 0xFF);
            }
            var_s0 += 1;
            var_v0 += 1;
        } while (var_s0 != 4);
        func_8004284C();
        break;
    case 5:
        func_80148820();
        break;
    case 6:
        Vblnk_Timer_Init();
        func_8004284C();
        break;
    case 7:
        if (Vblnk_Timer() >= 0xF1) {
            Fade_Out_Init();
            func_8004284C();
        }
        break;
    case 8:
        if (Fade_Out_Main() != 0) {
            if (*D_8014A3D8 == 1) {
                D_800E643E += 5;
                var_v0_2 = &D_800E62B8;
                do {
                    temp_t6 = *(s16 *)((unsigned char *)var_v0_2 + (502));
                    temp_t8 = *(s16 *)((unsigned char *)var_v0_2 + (558));
                    temp_t0 = *(s16 *)((unsigned char *)var_v0_2 + (614));
                    temp_t4 = *(s16 *)((unsigned char *)var_v0_2 + (446));
                    var_v0_2 += 0xE0;
                    *(s16 *)((unsigned char *)var_v0_2 + (390)) = (s16) (temp_t0 + 5);
                    *(s16 *)((unsigned char *)var_v0_2 + (334)) = (s16) (temp_t8 + 5);
                    *(s16 *)((unsigned char *)var_v0_2 + (278)) = (s16) (temp_t6 + 5);
                    *(s16 *)((unsigned char *)var_v0_2 + (222)) = (s16) (temp_t4 + 5);
                } while (var_v0_2 != &D_800E6558);
            } else if (D_8014A3D9 == 1) {
                D_800E643E += 3;
                var_v0_3 = &D_800E62B8;
                do {
                    temp_t7 = *(s16 *)((unsigned char *)var_v0_3 + (502));
                    temp_t9 = *(s16 *)((unsigned char *)var_v0_3 + (558));
                    temp_t1 = *(s16 *)((unsigned char *)var_v0_3 + (614));
                    temp_t5 = *(s16 *)((unsigned char *)var_v0_3 + (446));
                    var_v0_3 += 0xE0;
                    *(s16 *)((unsigned char *)var_v0_3 + (390)) = (s16) (temp_t1 + 3);
                    *(s16 *)((unsigned char *)var_v0_3 + (334)) = (s16) (temp_t9 + 3);
                    *(s16 *)((unsigned char *)var_v0_3 + (278)) = (s16) (temp_t7 + 3);
                    *(s16 *)((unsigned char *)var_v0_3 + (222)) = (s16) (temp_t5 + 3);
                } while (var_v0_3 != &D_800E6558);
            }
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}
