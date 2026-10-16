void func_8011767C(void) {
    EvShorts11 sp3C;
    EvShorts11 sp54;
    u8 sp6B;
    s16 sp42;
    u8 sp3B;
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    u8 var_a2;

    sp54 = D_80125330;
    sp3C = D_80125348;
    if ((D_800B0895 == 0x61) && ((u8) D_800B0896 >= 6U)) {
        sp42 += 1;
    } else if (D_800B0895 == 0x62) {
        sp42 += 1;
    }
    D_80124364 = (s16) D_800EECBC;
    temp_v1 = *(&D_800B0A14 + (D_800B1746 * 0x34));
    temp_a0 = temp_v1 & 0xF;
    D_80124368 = (s16) ((u32) D_800B1AE4 % (u32) ((sp3C.v[D_800B1746] * (D_800EECBC == 1)) + ((D_800EECBC == 0) * sp54.v[D_800B1746])));
    D_801252EC = 0;
    if (D_800B0896 == temp_a0) {
        if ((u8) D_800B0897 < (u32) ((u32) (temp_v1 << 0x17) >> 0x1B)) {
            D_801252EC = 1;
        }
    } else if (((u32) ((temp_a0 - D_800B0896) + 0xC) % 12U) == 1) {
        D_801252EC = 1;
    }
    D_801252F0 = 0;
    if (((D_800B0895 == 0x61) && ((u8) D_800B0896 >= 3U)) || (D_800B0895 == 0x62)) {
        D_801252F0 = 1;
    }
    sp3B = 0;
    sp6B = func_8002328C(D_800B1746, D_800EECBC, 0, (s16 (*)[]) &sp54[0]) & 0x7F;
    temp_v0 = func_8002336C(D_800B1746);
    var_a2 = sp3B;
    if ((sp6B == 0) && (temp_v0 == 0)) {
        var_a2 = 1;
    }
    if ((var_a2 != 0) && (D_800EECBC == 0)) {
        D_80124364 = 4;
        goto block_30;
    }
    if ((D_801252EC != 0) && (D_800EECBC == 0)) {
        D_80124364 = 2;
        D_80124368 = D_800B0895 - 0x5F;
        if (D_800B1746 == 6) {
            if ((sp6B < 2U) && ((u32) temp_v0 < 2U)) {
                D_80124368 = 1;
            } else {
                goto block_30;
            }
        }
    } else if ((D_801252F0 != 0) && (D_800EECBC == 0)) {
        D_80124364 = 3;
block_30:
        D_80124368 = 0;
    }
    if ((D_800B1746 == 5) && (D_80124364 == 0) && (D_80124368 == 3)) {
        *(u8 *)((unsigned char *)(&D_800B0B14) + (1)) = (u8) (*(u8 *)((unsigned char *)(&D_800B0B14) + (1)) | 0x40);
    }
    func_80011DFC();
}
