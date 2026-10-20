void func_80140AE4(void);
void func_8013FFAC(void) {
    s32 sp54[6];
    u32 i;
    s32 *var_v1;
    s32 var_v0;
    u8 temp_t3;

    switch (D_800E6280.unk_110D) {
    case 0:
        func_800438DC(1, 0);
        D_800E6280.unk_037 = 0;
        D_800E6280.unk_036 = 0;
        D_800E6280.unk_038 = 0;
        func_80048DAC(1);
        func_80048EB8(0);
        func_8006BD6C(0);
        func_80041584();
        func_8004E58C();
        func_800438F0(1);
        func_80048390();
        D_801500C0 = 0;
        func_801408F0();
        D_801500C4 = D_8014E070[D_800E6280.unk_F5F % 13U];
        func_80044890(0, 0xBF98, 0xBF98, 0xCFA7, 0xCF8B, 0xCF83);
        D_800E6280.unk_110D += 1;
    default:
        break;
    case 1:
        if (func_80044E8C() == 1) {
            func_80044750(0x201);
            func_80044890(1, 0xC5B7, 0xC594, 0xD6B2, 0xD664, 0xD643);
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        if (func_80044E8C() == 1) {
            func_800462C8(1U, 0x801FA800, 0xBE09);
            cd_read_sectors(0x69U, 0x80180000, 0xB389);
            D_800E6280.unk_110D += 1;
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 5:
        func_800438DC(1, 1);
        func_8013F980();
        func_80061710();
        func_80061790();
        func_800618B0();
        func_80043914(0x80180000, 0x1F, 1, 3, 0);
        func_80043914(0x80180200, 0x1E, 1, 3, 0);
        func_80043914(0x80180400, 0x1D, 1, 3, 0);
        func_80048514(0xF, 0x801B0600, 1);
        var_v0 = 0x80198600;
        var_v1 = &sp54[0];
        do {
            *var_v1 = var_v0;
            var_v0 += 0x4000;
            var_v1++;
        } while (var_v0 < (s32)0x801B0600);
        func_8004A1A0(sp54, 1, 6, 3, 2, 0x1FE, -0xA0, -0x7D, 0xC, 0x15);
        D_801220F4 = 0x01000000;
        D_80122106 = 0;
        D_80122105 = 0;
        D_80122104 = 0;
        D_800E6280.unk_03A = 0;
        for (i = 0; i < 6; i++) {
            sp54[i] = i * 0x4000 + 0x80180600;
        }
        func_8004A1A0(sp54, 0, 6, 3, 2, 0x1FF, -0xA0, -0x7D, 0xD, 0x18);
        D_801220D0 = 0x80000000;
        D_801220E1 = 0x80;
        D_801220E0 = 0x80;
        D_800E6280.unk_03A = 0;
        D_801220E2 = 0x80;
        func_800AE0A0((void *)0x801E2000, 0x80180600, 0x18000);
        func_80046290(D_8014DB90[D_800E6280.unk_F5F][0], D_8014DB90[D_800E6280.unk_F5F][1], (s32) D_800E6280.unk_F5F);
        func_80044750(0x300);
        D_800E6280.unk_110D += 1;
        break;
    case 6:
        if (func_800460EC() & 4) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 7:
        func_80140AE4();
        if ((u8) D_800E6280.unk_03A < 0x81U) {
            D_80122106 = D_800E6280.unk_03A;
            D_80122105 = D_800E6280.unk_03A;
            D_80122104 = D_800E6280.unk_03A;
            D_800E6280.unk_03A += 1;
        }
        if (func_800460EC() & 2) {
            D_801500DC += 1;
            D_800B3CF8 = 0;
            D_800B3CFC = 0;
            D_800B3D00 = 0;
            D_800E6280.unk_1104.w = 0;
            D_800E6280.unk_110D += 1;
        }
        break;
    case 8:
        if ((u8) D_800E6280.unk_03A < 0x81U) {
            D_80122106 = D_800E6280.unk_03A;
            D_80122105 = D_800E6280.unk_03A;
            D_80122104 = D_800E6280.unk_03A;
            D_800E6280.unk_03A += 1;
        }
        if (D_800E6280.unk_F5F == 0) {
            if ((u32) D_800E6280.unk_1104.w >= 0x1EU) {
                func_8013F320(0);
                if (D_8015006C >= 3) {
                    D_8015006C = 0;
                    D_800E6280.unk_110D += 1;
                }
            }
        } else if (D_800E6280.unk_F5F == 9) {
            if ((u32) D_800E6280.unk_1104.w >= 0x1EU) {
                func_8013F320(0);
                if (D_8015006C >= 3) {
                    D_8015006C = 0;
                    D_800E6280.unk_110D += 1;
                }
            }
        } else if (D_800E6280.unk_F5F == 0xC) {
            if ((u32) D_800E6280.unk_1104.w >= 0x1EU) {
                func_8013F320(0);
                if (D_8015006C >= 3) {
                    D_8015006C = 0;
                    D_800E6280.unk_110D += 1;
                }
            }
        } else {
            D_800E6280.unk_110D += 1;
        }
        D_800E6280.unk_1104.w += 1;
        break;
    case 9:
        D_80122106 = D_800E6280.unk_03A;
        D_80122105 = D_800E6280.unk_03A;
        D_80122104 = D_800E6280.unk_03A;
        temp_t3 = (D_800E6280.unk_03A + 1) & 0xFF;
        D_800E6280.unk_03A = temp_t3;
        if (temp_t3 >= 0x80U) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 10:
        func_8013FC08();
        D_800E6280.unk_110D += 1;
        break;
    case 11:
        D_801220F4 = 0x01000000;
        D_801220D0 = 0x01000000;
        D_800E6280.unk_110D += 1;
        break;
    case 12:
        D_801500D4 = 0x800;
        func_8013FA7C();
        D_801220F4 = 0x80000000;
        D_800E6280.unk_110D += 1;
        break;
    case 13:
        func_80140AE4();
        if (D_800E6280.unk_F88 & 0x20) {
            if ((D_801500D8 == 0) || (D_801500D8 == 2)) {
                func_8013EEFC();
            }
            D_800E6280.unk_110D += 1;
        }
        break;
    case 14:
        if (D_80150070 >= 3) {
            if (D_801500D4 >= 0xC00) {
                D_80150070 = 0;
                func_80042808();
            } else {
                goto block_50;
            }
        } else {
block_50:
            if ((D_80150070 == -1) && (D_801500D4 >= 0xC00)) {
                func_80042808();
            } else {
                func_8013E89C(1);
                D_801500D4 += 8;
                func_8013FA7C();
            }
        }
        break;
    }
}
