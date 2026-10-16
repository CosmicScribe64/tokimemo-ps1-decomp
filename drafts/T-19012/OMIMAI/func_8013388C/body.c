void func_8013388C(void) {
    u8 sp2B;
    u32 sel;
    u32 idx;

    sp2B = get_g_zyotai_s(D_800E6280.unk_F5F) & 0x7F;
    func_800AE120(D_800E6280.unk_10F8);
    switch (D_800E6280.unk_F5F) {
    case 0:
        sel = D_800E6280.unk_F68.w & 0xF;
        if (sel < 2U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
        } else if (sel == 2) {
            D_80134CCC = 0;
            D_800CA148 = 2;
        } else if (sel == 3) {
            D_80134CCC = 0;
            D_800CA148 = 4;
        } else if (sel == 4) {
            D_80134CCC = 0;
            D_800CA148 = (((u16) D_800E6280.unk_0F4.h >> 0xC) * 2) + 6;
            if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 0xA) {
                D_800CA148 = D_800CA148 - 4;
            }
        } else {
            D_80134CCC = 3;
            D_800CA148 = 0x1A;
        }
        if (func_800AE0D0() % 2) {
            D_800CA148 = D_800CA148 + 1;
            if (D_800CA148 == 5) {
                D_80134CCC = 2;
            }
        }
        break;
    case 1:
        if ((D_800E6280.unk_F68.w & 0xF) < 2U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
        } else {
            D_80134CCC = 0;
            D_800CA148 = (((u16) D_800E6280.unk_0F4.h >> 0xC) * 2) + 2;
        }
        if (func_800AE0D0() % 2) {
            D_800CA148 += 1;
        }
        break;
    case 2:
        if ((D_800E6280.unk_F68.w & 0xF) < 2U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
        } else {
            D_80134CCC = 4;
            D_800CA148 = (((u16) D_800E6280.unk_0F4.h >> 0xC) * 2) - 2;
        }
        if (func_800AE0D0() % 2) {
            D_800CA148 += 1;
        }
        break;
    case 3:
        if ((D_800E6280.unk_F68.w & 0xF) < 3U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
        } else {
            D_80134CCC = 0;
            D_800CA148 = (((u16) D_800E6280.unk_0F4.h >> 0xC) * 2) - 6;
        }
        if (func_800AE0D0() % 2) {
            D_800CA148 += 1;
        }
        break;
    case 4:
        if ((D_800E6280.unk_F68.w & 0xF) < 4U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
            if (func_800AE0D0() % 2) {
                D_800CA148 += 1;
            }
        } else if ((func_800AE0D0() % 2) || ((idx = (u16) D_800E6280.unk_0F4.h >> 0xC, (idx == 6)) && ((D_800E6280.unk_03E == 0x62) || ((D_800E6280.unk_03E == 0x61) && (D_800E6280.unk_03F >= 9U)))) || ((idx == 7) && (D_800E6280.unk_03E == 0x62))) {
            if (sp2B < 2U) {
                D_80134CCC = 6;
                D_800CA148 = 4;
            } else {
                D_80134CCC = 5;
                D_800CA148 = 5;
            }
        } else {
            D_80134CCC = 0;
            D_800CA148 = ((u16) D_800E6280.unk_0F4.h >> 0xC) - 4;
        }
        break;
    case 5:
        if ((D_800E6280.unk_F68.w & 0xF) < 4U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
        } else {
            D_80134CCC = 0;
            D_800CA148 = 2;
        }
        if (func_800AE0D0() % 2) {
            D_800CA148 += 1;
        }
        break;
    case 6:
        if ((D_800E6280.unk_F68.w & 0xF) < 4U) {
            D_80134CCC = 0;
            D_800CA148 = 0;
            if (func_800AE0D0() % 2) {
                D_80134CCC = 2;
                D_800CA148 += 1;
            }
        } else {
            D_80134CCC = 0;
            D_800CA148 = 4;
            if (func_800AE0D0() % 2) {
                D_800CA148 += 1;
            }
        }
        if (sp2B < 2U) {
            D_800CA148 += 2;
        }
        break;
    case 7:
        D_80134CCC = 3;
        D_800CA148 = 0;
        break;
    case 8:
        D_80134CCC = 7;
        D_800CA148 = 0;
        if (func_800AE0D0() % 2) {
            D_800CA148 += 1;
        }
        break;
    case 9:
        D_80134CCC = (u8) (func_800AE0D0() % 2);
        D_800CA148 = 0;
        if (func_800AE0D0() % 2) {
            D_800CA148 += 1;
        }
        break;
    case 10:
        if ((D_800E6280.unk_F68.w & 0xF) < 4U) {
            D_800CA148 = 0;
            if (func_800AE0D0() % 2) {
                D_800CA148 += 1;
            }
            if (D_800CA148 == 1) {
                D_80134CCC = 2;
            } else {
                D_80134CCC = 0;
            }
        } else {
            D_80134CCC = 0;
            D_800CA148 = 2;
            if (func_800AE0D0() % 2) {
                D_800CA148 += 1;
            }
        }
        break;
    default:
        D_800CA148 = (s16) (func_800AE0D0() % 8);
        D_80134CCC = (func_800AE0D0() % 2) + 8;
        if ((D_800E6280.unk_F68.w & 0xF) == 3) {
            D_80134CCC = 9;
        }
        break;
    }
    D_800CA14C = 0;
    func_801340E4();
    func_8004284C();
}
