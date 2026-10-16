void func_8013B5B0(void) {
    s32 temp_v0;
    s32 var_at;
    u8 temp_v1;

    temp_v1 = D_800E6280.unk_1109;
    if (temp_v1 >= 6U) {
        if (temp_v1 < 0xAU) {
            func_8013BEA8();
        }
    }
    if (temp_v1 != 9) {
        var_at = temp_v1 < 5U;
        if (!(D_800E6280.unk_1092 & 1)) {
            func_8006BA40();
            goto block_7;
        }
    } else {
block_7:
        var_at = temp_v1 < 5U;
    }
    if ((var_at == 0) && (temp_v1 < 0xCU)) {
        func_80049A40(-0xA0, 0x30, 8, 0x48, 2, 0, 1);
        func_80049A40(-0xA0, 0x70, 0x140, 8, 2, 0, 1);
        func_80049A40(0x98, 0x30, 8, 0x48, 2, 0, 1);
    }
    switch (temp_v1) {
    case 0:
        func_8014E780();
        func_80144010();
        break;
    case 1:
        func_8013D834();
        break;
    case 2:
        func_8013DC60();
        break;
    case 3:
        func_8014D380();
        func_80142F70();
        break;
    case 4:
        func_80141060();
        func_80141A64();
        func_801525B0();
        func_8014B294();
        func_80042808();
        break;
    case 5:
        func_80153784();
        break;
    case 6:
        back_clear_switch(0);
        func_8014B294();
        if (D_8015EB98 == 0x80000000) {
            func_80152F30();
        } else {
            func_801529B4();
        }
        break;
    case 7:
        func_8014B79C();
        func_8013D780();
        break;
    case 8:
        if (D_8015EB98 == 0x80000000) {
            func_8004500C(1, 0x200);
        }
        func_80042808();
        break;
    case 9:
        func_8013B910();
        break;
    case 10:
        func_80044750(0xC1);
        func_80042808();
        break;
    case 11:
        func_8013C32C();
        func_801528DC();
        break;
    case 12:
        func_8004482C();
        func_800452C4();
        func_80042808();
        break;
    case 13:
        func_800674B0();
        func_80042808();
        break;
    case 14:
        temp_v0 = D_800E6280.unk_1100 + 1;
        D_800E6280.unk_1100 = temp_v0;
        if ((u32) temp_v0 >= 0x401U) {
            func_8004482C();
            func_800452C4();
            D_800E6280.unk_1109 = 0xD;
        } else if (func_80044E8C() == 1) {
            func_80042808();
        }
        break;
    default:
        func_8014E8F4();
        break;
    }
    if (!(D_800E6280.unk_1092 & 1)) {
        func_8006BD6C(1);
        return;
    }
    func_8006BD6C(0);
}
