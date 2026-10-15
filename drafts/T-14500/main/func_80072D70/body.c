s32 func_80072D70(void) {
    s32 sp3C;
    s32 sp38;
    s32 s0;
    s32 s1;
    s32 s2;

    sp3C = 0;
    s1 = ((s16) D_800E6280.unk_118.unk_02 / 8) + 0xCC;
    if (s1 >= 0xFF) {
        s1 = 0xFE;
    }
    switch (D_800E6280.unk_0F4.b[1] & 0xF) {
    case 0:
        s0 = 0;
        if ((D_800E6280.unk_0F6.b[0] >> 5) == 0) {
            if ((D_800E6280.unk_0FC.unk_02 < 0x14) && (s1 < (rand() & 0xFF))) {
                s0 = 1;
                sp3C = 1;
            } else if (D_800E6280.unk_0FC.unk_02 == 0) {
                s0 = 1;
                sp3C = 1;
            }
            if ((D_800E6280.unk_11C.unk_02 >= 0x51) && ((u32) s1 < (u32) (D_800E7D10 & 0xFF))) {
                s0 += 2;
                sp3C = 1;
            }
        }
        if (s0 == 1) {
            func_80042940(1);
        }
        if (s0 == 2) {
            func_80042940(2);
        }
        if (s0 == 3) {
            func_80042940(3);
        }
        break;
    case 1:
        s0 = 1;
        if ((D_800E6280.unk_11C.unk_02 >= 0x51) && ((u32) s1 < (u32) (D_800E7D10 & 0xFF))) {
            s0 = 3;
            sp3C = 1;
        }
        if (D_800E6280.unk_0FC.unk_02 >= 0x1F) {
            s0 = 0;
            sp3C = 1;
        }
        if (s0 == 3) {
            func_80042940(3);
        }
        if (s0 == 0) {
            func_80042940(5);
        }
        break;
    case 2:
        s0 = 2;
        if ((D_800E6280.unk_0FC.unk_02 < 0x14) && (s1 < (rand() & 0xFF))) {
            s0 = 3;
            sp3C = 1;
        } else if (D_800E6280.unk_0FC.unk_02 == 0) {
            s0 = 3;
            sp3C = 1;
        }
        if (D_800E6280.unk_11C.unk_02 < 0x46) {
            s0 = 0;
            sp3C = 1;
        }
        if (s0 == 3) {
            func_80042940(3);
        }
        if (s0 == 0) {
            func_80042940(6);
        }
        break;
    case 3:
        s0 = 3;
        if ((D_800E6280.unk_11C.unk_02 < 0x46) && (D_800E6280.unk_0FC.unk_02 >= 0x1F)) {
            s0 = 0;
            sp3C = 1;
        }
        if (s0 == 0) {
            func_80042940(7);
            break;
        }
        if (((u8 *) D_800E6280.unk_044)[D_800E6280.unk_040] == 0) {
            sp38 = rand() % 11;
            s0 = 0;
            s1 = sp38;
            do {
                s2 = s1 % 11;
                if (D_800E6280.unk_1BC[s2].unk_0C.f.b1 && (get_h_tokimeki(s2) >= 0x33U) && (get_g_zyotai_s(sp38) != 4)) {
                    if ((rand() & 0xFF) < 0x3F) {
                        D_800E6280.unk_75D = s2;
                        main_state_set(0x62);
                        return 1;
                    }
                }
                s0 += 1;
                s1 += 1;
            } while (s0 != 0xB);
        }
        break;
    default:
        D_800E6280.unk_0F4.b[1] &= 0xFFF0;
        break;
    }
    if (sp3C == 0) {
        func_80042908(0xFF);
    }
}
