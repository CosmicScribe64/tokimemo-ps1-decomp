void func_8013FFAC(void) {
    s32 sp58;
    s32 *temp_v1;
    s32 *var_v1;
    s32 temp_v0;
    s32 var_v0;
    u8 temp_t3;
    u8 temp_v0_2;
    u8 *temp_v1_2;

    switch (*(u8 *)((unsigned char *)(&D_800E6280) + (4365))) {
    case 0:
        draw2d3d(1, 0);
        *(s8 *)((unsigned char *)(&D_800E6280) + (55)) = 0;
        *(s8 *)((unsigned char *)(&D_800E6280) + (54)) = 0;
        *(s8 *)((unsigned char *)(&D_800E6280) + (56)) = 0;
        func_80048DAC(1);
        func_80048EB8(0);
        func_8006BD6C(0);
        func_80041584();
        func_8004E58C();
        back_clear_switch(1);
        tpage_buf_clear();
        D_801500C0 = 0;
        func_801408F0();
        D_801500C4 = *(&D_8014E070 + (((u8) *(u8 *)((unsigned char *)(&D_800E6280) + (3935)) % 13U) * 4));
        func_80044890(0, 0xBF98, 0xBF98, 0xCFA7, 0xCF8B, 0xCF83);
        *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
    default:
        break;
    case 1:
        if (func_80044E8C(*(u8 *)((unsigned char *)(&D_800E6280) + (4365))) == 1) {
            func_80044750(0x201);
            func_80044890(1, 0xC5B7, 0xC594, 0xD6B2, 0xD664, 0xD643);
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        }
        break;
    case 2:
        if (func_80044E8C(*(u8 *)((unsigned char *)(&D_800E6280) + (4365))) == 1) {
            func_800462C8(1, 0x801FA800, 0xBE09);
            cd_read_sectors(0x69, 0x80180000, 0xB389);
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        }
        break;
    case 3:
        if (cd_read_sync_status(*(u8 *)((unsigned char *)(&D_800E6280) + (4365))) & 1) {
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        }
        break;
    case 4:
        if (cd_read_sync_status(*(u8 *)((unsigned char *)(&D_800E6280) + (4365))) & 1) {
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        }
        break;
    case 5:
        draw2d3d(1, 1);
        func_8013F980();
        func_80061710();
        func_80061790();
        func_800618B0();
        load_palette(0x80180000, 0x1F, 1, 3, 0);
        load_palette(0x80180200, 0x1E, 1, 3, 0);
        load_palette(0x80180400, 0x1D, 1, 3, 0);
        load_tpage_buf_lock(0xF, 0x801B0600, 1);
        var_v0 = 0x80198600;
        var_v1 = &sp54[0];
        do {
            *var_v1 = var_v0;
            var_v0 += 0x4000;
            var_v1 += 1;
        } while (var_v0 < 0x801B0600);
        func_8004A1A0((s32 (*)[]) &sp54[0], 1, 6, 3, 2, 0x1FE, -0xA0, -0x7D, 0xC, 0x15);
        D_801220F4 = 0x01000000;
        D_80122106 = 0;
        D_80122105 = 0;
        D_80122104 = 0;
        *(u8 *)((unsigned char *)(&D_800E6280) + (58)) = 0U;
        temp_v0 = 2 << 0xE;
        temp_v1 = &(&sp54[0])[2];
        sp54[0] = 0x80180600;
        sp58 = 0x80184600;
        (*(s32 *)temp_v1) = temp_v0 + 0x80180600;
        *(s32 *)((unsigned char *)temp_v1 + (4)) = (s32) (temp_v0 + 0x80184600);
        *(s32 *)((unsigned char *)temp_v1 + (12)) = (s32) (temp_v0 + 0x8018C600);
        *(s32 *)((unsigned char *)temp_v1 + (8)) = (s32) (temp_v0 + 0x80188600);
        func_8004A1A0((s32 (*)[]) &sp54[0], 0, 6, 3, 2, 0x1FF, -0xA0, -0x7D, 0xD, 0x18);
        D_801220D0 = 0x80000000;
        D_801220E1 = 0x80;
        D_801220E0 = 0x80;
        *(u8 *)((unsigned char *)(&D_800E6280) + (58)) = 0U;
        D_801220E2 = 0x80;
        func_800AE0A0(0x801E2000, 0x80180600, 0x18000);
        temp_v1_2 = (*(u8 *)((unsigned char *)(&D_800E6280) + (3935)) * 8) + &D_8014DB90;
        func_80046290((*(s32 *)temp_v1_2), *(s32 *)((unsigned char *)temp_v1_2 + (4)), *(u8 *)((unsigned char *)(&D_800E6280) + (3935)));
        func_80044750(0x300);
        *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        break;
    case 6:
        if (func_800460EC(*(u8 *)((unsigned char *)(&D_800E6280) + (4365))) & 4) {
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        }
        break;
    case 7:
        func_80140AE4(*(u8 *)((unsigned char *)(&D_800E6280) + (4365)));
        if ((u8) *(u8 *)((unsigned char *)(&D_800E6280) + (58)) < 0x81U) {
            D_80122106 = *(u8 *)((unsigned char *)(&D_800E6280) + (58));
            D_80122105 = *(u8 *)((unsigned char *)(&D_800E6280) + (58));
            D_80122104 = *(u8 *)((unsigned char *)(&D_800E6280) + (58));
            *(u8 *)((unsigned char *)(&D_800E6280) + (58)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (58)) + 1);
        }
        if (func_800460EC() & 2) {
            D_801500DC += 1;
            D_800B3CF8 = 0;
            D_800B3CFC = 0;
            D_800B3D00 = 0;
            *(u32 *)((unsigned char *)(&D_800E6280) + (4356)) = 0U;
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        }
        break;
    case 8:
        if ((u8) *(u8 *)((unsigned char *)(&D_800E6280) + (58)) < 0x81U) {
            temp_v0_2 = *(u8 *)((unsigned char *)(&D_800E6280) + (58)) & 0xFF;
            D_80122106 = temp_v0_2;
            D_80122105 = temp_v0_2;
            D_80122104 = temp_v0_2;
            *(u8 *)((unsigned char *)(&D_800E6280) + (58)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (58)) + 1);
        }
        if (*(u8 *)((unsigned char *)(&D_800E6280) + (3935)) == 0) {
            if ((u32) *(u32 *)((unsigned char *)(&D_800E6280) + (4356)) >= 0x1EU) {
                func_8013F320(0, *(u8 *)((unsigned char *)(&D_800E6280) + (4365)));
                if (D_8015006C >= 3) {
                    D_8015006C = 0;
                    *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
                }
            }
        } else if (*(u8 *)((unsigned char *)(&D_800E6280) + (3935)) == 9) {
            if ((u32) *(u32 *)((unsigned char *)(&D_800E6280) + (4356)) >= 0x1EU) {
                func_8013F320(0, *(u8 *)((unsigned char *)(&D_800E6280) + (4365)));
                if (D_8015006C >= 3) {
                    D_8015006C = 0;
                    *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
                }
            }
        } else if (*(u8 *)((unsigned char *)(&D_800E6280) + (3935)) == 0xC) {
            if ((u32) *(u32 *)((unsigned char *)(&D_800E6280) + (4356)) >= 0x1EU) {
                func_8013F320(0, *(u8 *)((unsigned char *)(&D_800E6280) + (4365)));
                if (D_8015006C >= 3) {
                    D_8015006C = 0;
                    *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
                }
            }
        } else {
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        }
        *(u32 *)((unsigned char *)(&D_800E6280) + (4356)) = (u32) (*(u32 *)((unsigned char *)(&D_800E6280) + (4356)) + 1);
        break;
    case 9:
        D_80122106 = *(u8 *)((unsigned char *)(&D_800E6280) + (58));
        D_80122105 = *(u8 *)((unsigned char *)(&D_800E6280) + (58));
        D_80122104 = *(u8 *)((unsigned char *)(&D_800E6280) + (58));
        temp_t3 = (*(u8 *)((unsigned char *)(&D_800E6280) + (58)) + 1) & 0xFF;
        *(u8 *)((unsigned char *)(&D_800E6280) + (58)) = temp_t3;
        if (temp_t3 >= 0x80U) {
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        }
        break;
    case 10:
        func_8013FC08(*(u8 *)((unsigned char *)(&D_800E6280) + (4365)));
        *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        break;
    case 11:
        D_801220F4 = 0x01000000;
        D_801220D0 = 0x01000000;
        *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        break;
    case 12:
        D_801500D4 = 0x800;
        func_8013FA7C(*(u8 *)((unsigned char *)(&D_800E6280) + (4365)));
        D_801220F4 = 0x80000000;
        *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        break;
    case 13:
        func_80140AE4(*(u8 *)((unsigned char *)(&D_800E6280) + (4365)));
        if (*(s32 *)((unsigned char *)(&D_800E6280) + (3976)) & 0x20) {
            if ((D_801500D8 == 0) || (D_801500D8 == 2)) {
                func_8013EEFC();
            }
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        }
        break;
    case 14:
        if (D_80150070 >= 3) {
            if (D_801500D4 >= 0xC00) {
                D_80150070 = 0;
                func_80042808(*(u8 *)((unsigned char *)(&D_800E6280) + (4365)));
            } else {
                goto block_50;
            }
        } else {
block_50:
            if ((D_80150070 == -1) && (D_801500D4 >= 0xC00)) {
                func_80042808(*(u8 *)((unsigned char *)(&D_800E6280) + (4365)));
            } else {
                func_8013E89C(1, *(u8 *)((unsigned char *)(&D_800E6280) + (4365)));
                D_801500D4 += 8;
                func_8013FA7C();
            }
        }
        break;
    }
}
