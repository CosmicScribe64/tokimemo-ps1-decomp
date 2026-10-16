s32 func_80138D08(void) {
    u8 var_t3;

    switch (D_800E6280.unk_110D) {
    default:
        func_801432F0((s32) "TOTAL", -0xE4, 0x48, 1);
        /* fallthrough */
    case 6:
    case 7:
        func_801432F0((s32) "SHIELD BONUS", -0xE4, 0x18, 5);
        /* fallthrough */
    case 4:
    case 5:
        func_801432F0((s32) "BOMB   BONUS", -0xE4, -8, 2);
        /* fallthrough */
    case 2:
    case 3:
        func_801432F0((s32) "TIME   BONUS", -0xE4, -0x28, 3);
        break;
    }
    func_801432F0((s32) "SCORE", -0xE4, -0x48, 6);
    switch (D_800E6280.unk_110D) {
    default:
        func_801430D0(D_8015F4A0, -0x40, 0x58, 0);
        /* fallthrough */
    case 8:
    case 9:
        func_801430D0(D_8015F494, -0x40, 0x28, 0);
        /* fallthrough */
    case 6:
    case 7:
        func_801430D0(D_8015F498, -0x40, 8, 0);
        /* fallthrough */
    case 4:
    case 5:
        func_801430D0(D_8015F49C, -0x40, -0x18, 0);
        /* fallthrough */
    case 2:
    case 3:
        func_801430D0(D_8015F490, -0x40, -0x38, 0);
        break;
    }
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80138964(D_8015F490, -0x40, -0x38, 0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_80138964(D_8015F490, -0x40, -0x38, 1) == D_8015F490) {
            var_t3 = D_800E6280.unk_110D + 1;
block_30:
            D_800E6280.unk_110D = var_t3;
        }
        break;
    case 2:
        func_80138964(D_8015F49C, -0x40, -0x18, 0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        if (func_80138964(D_8015F49C, -0x40, -0x18, 1) == D_8015F49C) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 4:
        func_80138964(D_8015F498, -0x40, 8, 0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        if (func_80138964(D_8015F498, -0x40, 8, 1) == D_8015F498) {
            var_t3 = D_800E6280.unk_110D + 1;
            goto block_30;
        }
        break;
    case 6:
        func_80138964(D_8015F494, -0x40, 0x28, 0);
        D_800E6280.unk_110D += 1;
        break;
    case 7:
        if (func_80138964(D_8015F494, -0x40, 0x28, 1) == D_8015F494) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 8:
        func_80138964(D_8015F4A0, -0x40, 0x58, 0);
        D_800E6280.unk_110D += 1;
        break;
    case 9:
        if (func_80138964(D_8015F4A0, -0x40, 0x58, 1) == D_8015F4A0) {
            var_t3 = D_800E6280.unk_110D + 1;
            goto block_30;
        }
        break;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        if ((u8) D_800E6280.unk_110D < 0xAU) {
            D_800E6280.unk_110D = 0xA;
        } else {
            D_8015EDF0 = D_8015F4A0;
            func_80059308();
            func_8004284C();
        }
    }
}
