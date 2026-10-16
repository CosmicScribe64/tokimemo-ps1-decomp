void func_80152F30(void) {
    s32 sp3C;
    s32 temp_a2;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v1;
    u32 temp_v1_2;
    u32 temp_v1_3;
    u32 temp_v1_4;
    u32 temp_v1_5;

    temp_v0 = func_8013E9A8(8);
    switch (temp_v0) {
    case 0:
        sp3C = 0x9F9F9F;
        break;
    case 1:
        sp3C = 0x6F6F6F;
        break;
    case 2:
        sp3C = 0xBF0000;
        break;
    case 3:
        sp3C = 0xBF00;
        break;
    case 4:
        sp3C = 0xBF;
        break;
    case 5:
        sp3C = 0xAFAF00;
        break;
    case 6:
        sp3C = 0xAF00AF;
        break;
    case 7:
        sp3C = 0xAFAF;
        break;
    }
    func_8014B79C();
    switch (D_800E6280.unk_110A) {
    case 0:
        D_800E6280.unk_F5F = 0xE;
        func_80046290(0x1C0000A8, 0x0F0000B7, 0xE);
        func_80044750(0x300);
        func_8004284C();
        break;
    case 1:
        func_80044750(0xE4);
        func_8004284C();
        break;
    case 2:
        if (func_800460EC() & 4) {
            func_8004284C();
        }
        break;
    case 3:
        func_8013E810(0x30, 1, 1, 1);
        func_8013E810(0x1D, 0x17, 1, 1);
        func_8013E97C(0x30, 0x160, 0x58);
        func_8013E97C(0x1D, 0x168, 0x88);
        D_8015EC48 = 0;
        D_8015EC44 = 0;
        D_8015EC40 = 0;
        D_8015EC38 = 0;
        D_8015EC3C = -0x1E;
        D_8015EC4C = 0;
        func_8013E97C(0x2F, 0, -0x1E);
        func_8013E810(0x2F, 2, 1, 1);
        func_8013E97C(0x2E, D_8015EC38, D_8015EC3C);
        func_8013E810(0x2E, 6, 1, 1);
        func_8004284C();
        break;
    case 4:
        func_8013E97C(0x30, 0x160 - D_800E7384, 0x58);
        func_8013E97C(0x1D, 0x168 - D_800E7384, 0x88);
        temp_v1 = D_800E7384 + 4;
        D_800E7384 = temp_v1;
        if (temp_v1 >= 0x51U) {
            func_8013E97C(0x30, 0, 0);
            func_8013E7C0(0x30, 0, 1, 1);
            func_8013E97C(0x1D, 0, 0);
            func_8013E7C0(0x1D, 0x18, 1, 1);
            func_8013E97C(0x38, 0, 0);
            func_8013E7C0(0x38, 9, 1, 1);
            D_80121531 = 2;
            func_8004284C();
        }
        break;
    case 5:
        temp_v0_2 = (u32) D_800E7384 >> 1;
        func_80049A40(-0x5C - temp_v0_2, -0x78, temp_v0_2 * 2, 0xA8, 3, sp3C, 0x81);
        dtd_on_tpage(0, 0, 3, 1, 0);
        temp_v1_2 = D_800E7384 + 1;
        D_800E7384 = temp_v1_2;
        if (temp_v1_2 >= 0x41U) {
            func_8004284C();
        }
        break;
    case 6:
        func_80049A40(-0x7C, -0x78, 0x40, 0xA8, 3, sp3C, 0x81);
        dtd_on_tpage(0, 0, 3, 1, 0);
        func_8013E97C(0x3B, 0x48, D_800E7384 - 0x80);
        func_8013E810(0x3B, 8, 1, 1);
        temp_v1_3 = D_800E7384 + 1;
        D_800E7384 = temp_v1_3;
        if (temp_v1_3 >= 0xE9U) {
            k_disp_switch(D_8015EDE8, 1);
            func_8004284C();
        }
        break;
    case 7:
        func_80049A40(-0x7C, -0x78, 0x40, 0xA8, 3, sp3C, 0x81);
        D_800E7384 += 1;
        dtd_on_tpage(0, 0, 3, 1, 0);
        if ((u32) D_800E7384 >= 0x79U) {
            func_8004284C();
        }
        break;
    case 8:
        func_80049A40(D_800E7384 - 0x7C, -0x78, 0x40 - (D_800E7384 * 2), 0xA8, 3, sp3C, 0x81);
        dtd_on_tpage(0, 0, 3, 1, 0);
        temp_v1_4 = D_800E7384 + 1;
        D_800E7384 = temp_v1_4;
        if (temp_v1_4 >= 0x21U) {
            func_8004284C();
        }
        break;
    case 9:
        temp_a2 = D_800E7384 - 0x40;
        temp_v1_5 = D_800E7384 + 1;
        D_800E7384 = temp_v1_5;
        D_8015EC38 = 0x40;
        D_8015EC3C = temp_a2;
        if (temp_v1_5 >= 0x59U) {
            func_8004284C();
        }
        break;
    case 10:
        func_800AE0F0(D_8015E9E0, "宇宙人が襲ってきた！");
        func_8014B738((s32) D_8015EB70, 1, 0x5A);
        func_8013E7C0(0x38, 0xFF, 1, 0);
        func_8013E97C(0x3B, 0, 0);
        func_8013E7C0(0x3B, 0, 1, 1);
        func_8004284C();
        break;
    case 11:
        if (D_8015EE44.unk_04 == 0) {
            func_8004284C();
        }
        break;
    case 12:
        func_8013E97C(0x1E, 0, 0);
        func_8013E7C0(0x1E, 0, 5, 1);
        func_8004284C();
        break;
    case 13:
        func_801535E4();
        break;
    case 14:
        D_8015EDF0 = 0x10001000;
        func_80042808();
        break;
    }
    if ((u8) D_800E6280.unk_110A >= 9U) {
        func_8015785C();
    }
    if (((u8) D_800E6280.unk_110A >= 5U) && ((u8) D_800E6280.unk_110A < 8U)) {
        func_8013F15C(0x505, 0x6E, 7);
    }
}
