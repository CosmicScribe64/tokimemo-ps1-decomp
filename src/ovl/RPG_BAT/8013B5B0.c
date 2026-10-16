#include "common.h"
#include "ovl/RPG_BAT.h"

#ifdef NON_MATCHING
/* NON_MATCHING: T-17107, 25 differing lines; the state byte D_800E6280.unk_1109 is promoted to $v0, the original colours it $v1 (T-0018 promoted-global family; s32 return, u8 view, int callees, post-increment, temp form tried) */
void func_8013B5B0(void) {
    if (D_800E6280.unk_1109 >= 6 && D_800E6280.unk_1109 < 0xA) {
        func_8013BEA8();
    }
    if (D_800E6280.unk_1109 != 9 && !(D_800E6280.unk_1092 & 1)) {
        func_8006BA40();
    }
    if (D_800E6280.unk_1109 >= 5 && D_800E6280.unk_1109 < 0xC) {
        func_80049A40(-0xA0, 0x30, 8, 0x48, 2, 0, 1);
        func_80049A40(-0xA0, 0x70, 0x140, 8, 2, 0, 1);
        func_80049A40(0x98, 0x30, 8, 0x48, 2, 0, 1);
    }
    switch (D_800E6280.unk_1109) {
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
        if ((u32)D_800E6280.unk_1100++ >= 0x400U) {
            func_8004482C();
            func_800452C4();
            D_800E6280.unk_1109 = 0xD;
        } else if (func_80044E8C() == 1) {
            func_80042808();
        }
        break;
    case 15:
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
#else
INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013B5B0", func_8013B5B0);
#endif

s32 func_8013B910(void) {
    func_8013C32C();
    if (D_8015EC20 == 3) {
        D_8015EDF4 += 1;
        func_801439E0();
        func_8014B79C();
        return 0;
    } else {
        if ((D_800E6280.unk_F88 & 0x40) && D_8015EC24 == 0) {
            func_80044750(0x74);
            if (D_8015EE44.unk_00 >= 3 && D_8015EE44.unk_08 >= 3) {
                D_8015EE44.unk_08 = D_8015E744 + 1;
            }
        }
        if (D_8015EDC4 != 0x08000002 && (D_8015EDC4 & 0x08000000) && D_8015EE1C != 1) {
            if (D_8015EE50.unk_00 == 2 || D_8015EE50.unk_00 == 3) {
                func_8004E93C(D_8015EE40, 1);
                func_8004E93C(D_8015EDE8, 0);
            } else {
                func_8004E93C(D_8015EE40, 0);
                func_8004E93C(D_8015EDE8, 1);
            }
        }
        if (!(D_8015ED98 & 0xFF0111FF)) {
            D_8015ED9C += 1;
        }
        if ((D_8015EDC4 & 0xFF) == 0 && !(D_8015ED98 & 0xFF) && !(D_8015EDF0 & 0xFF) && !(D_8015EDC4 & 0x1000)) {
            D_8015EDC8 += 1;
        }
        if ((D_8015EDC4 & 0xFF) == 0 && !(D_8015ED98 & 0xFF) && !(D_8015EDF0 & 0xFF) && !(D_8015EDF0 & 0x1000) && !(D_8015EDF0 & 0x80000000)) {
            D_8015EDF4 += 1;
        }
        func_801439E0(D_8015ED98, D_8015EDF0);
        func_8013C1CC();
        func_8013CC90();
        func_8013C1CC();
        func_8013C890();
        func_8013C1CC();
        func_8013C2A0();
        func_8014B294();
        func_8014B79C();
        if (D_8015EE60 == 1) {
            func_8014C4FC();
        }
        if (D_8015EE6C == 1) {
            func_8014C804();
        }
        if (D_8015EE78 == 1) {
            func_8014CB0C();
        }
        if (D_8015EE84 == 1) {
            func_8014CE14();
        }
        if (D_8015EB98 == 0x80000000) {
            func_8015785C();
        }
    }
}

void func_8013BBF8(void) {
    RpgWordTbl10 sp58;
    RpgWordTbl10 sp30;
    s32 ten; /* FAKE: named 10 mentioned first puts the hoisted constant before the counters in the bne operands; real source unknown. T-17107 */

    ten = 10;
    sp58 = D_8015E550;
    sp30 = D_8015E578;
    D_8015EBEC.unk_00 = func_80043010();
    if (D_8015EBEC.unk_00 != D_8015EBEC.unk_04) {
        D_8015EBEC.unk_04 = D_8015EBEC.unk_00;
        if ((D_8015EBEC.unk_00 & 0xFFFF) != 0) {
            if (D_8015EBEC.unk_00 == sp58.v[D_8015EBEC.unk_0C]) {
                D_8015EBEC.unk_0C += 1;
            } else {
                D_8015EBEC.unk_0C = 0;
            }
            if (D_8015EBEC.unk_00 == sp30.v[D_8015EBEC.unk_08]) {
                D_8015EBEC.unk_08 += 1;
            } else {
                D_8015EBEC.unk_08 = 0;
            }
            if (ten == D_8015EBEC.unk_0C) {
                D_8015EBEC.unk_0C = 0;
                switch (D_8015EB98) {
                case 0x8000000:
                    D_8015EDC0 = 1;
                    /* fallthrough */
                case 0x10000:
                case 0x20000:
                case 0x40000:
                    if (D_8015ED94 >= 4) {
                        D_8015EDA0 = 0x1662;
                        D_8015ED94 = 0x1662;
                    }
                    break;
                case 0x100000:
                    if (D_8015ED94 > 0 && D_8015ED94 < 10) {
                        D_8015EDA0 = 0x1661;
                        D_8015ED94 = 0x1661;
                    }
                    /* fallthrough */
                case 0x80000000:
                    if (D_8015ED94 >= 2 && D_8015ED94 < 10) {
                        D_8015EDA0 = 0x1661;
                        D_8015ED94 = 0x1661;
                    }
                    if (D_8015ED94 >= 10) {
                        D_8015EDA0 += 1;
                        D_8015ED94 += 1;
                        if (D_8015EDA0 >= 0x2710) {
                            D_8015ED94 = 0x270F;
                            D_8015EDA0 = 0x270F;
                        }
                    }
                    break;
                }
            }
            if (ten == D_8015EBEC.unk_08) {
                D_8015EBEC.unk_08 = 0;
                switch (D_8015EB98) {
                case 0x100000:
                    D_8015ED94 = 1;
                    break;
                case 0x80000000:
                    D_8015ED94 = 2;
                    break;
                case 0x10000:
                case 0x20000:
                case 0x40000:
                case 0x8000000:
                    break;
                }
            }
        }
    }
}

void func_8013BEA8(void) {
    if (D_8015EC20 == 3) {
        func_8013BBF8();
    }
    D_8015EC24 = D_8015EC24 - 1;
    if (D_8015EC24 < 0) {
        D_8015EC24 = 0;
    }
    while (D_8015EC20 == 1 && D_8015EC24 == 0) {
        if (!(func_80043010() & 0x800)) {
            while (1) {
                if (func_80043010() & 0x800) {
                    func_8013F15C(0x500, 1, 0);
                    D_8015EC20 = 0;
                    func_8013E7C0(0x1C, 0x33, 1, 0);
                    D_8015EC24 = 2;
                    break;
                }
                func_8013BBF8();
            }
        }
    }
    if ((D_800E6280.unk_F88 & 0x800) && D_8015EC24 == 0) {
        if (!(D_8015EDC4 & 0xFF) && !(D_8015ED98 & 0xFF) && !(D_8015EDF0 & 0xFF) && !(D_8015EE1C & 0xFF) &&
            D_8015EB9C == 9 && D_8015EC20 == 0 && D_800E6280.unk_1109 == 9) {
            func_80044750(0x74);
            if (D_8015EE44.unk_00 >= 3 && D_8015EE44.unk_08 >= 3) {
                D_8015EE44.unk_08 = D_8015E744 - 5;
            }
            func_8013F15C(0x500, 1, 0);
            D_8015EC20 = 3;
            D_8015EBEC.unk_0C = 0;
            D_8015EBEC.unk_08 = 0;
            D_8015EBEC.unk_04 = 0;
            func_8013E7C0(0x1C, 0x33, 1, 1);
            D_8015EC24 = 2;
        } else if (D_8015EC20 != 0) {
            func_8013F15C(0x500, 1, 0);
            D_8015EC20 = 0;
            func_8013E7C0(0x1C, 0x33, 1, 0);
            D_8015EC24 = 2;
        } else {
            func_80044750(0x74);
            if (D_8015EE44.unk_00 >= 3 && D_8015EE44.unk_08 >= 3) {
                D_8015EE44.unk_08 = D_8015E744 - 5;
            }
            func_8013F15C(0x500, 1, 0);
            D_8015EC20 = 1;
            D_8015EBEC.unk_0C = 0;
            D_8015EBEC.unk_08 = 0;
            D_8015EBEC.unk_04 = 0;
            func_8013E7C0(0x1C, 0x33, 1, 1);
            D_8015EC24 = 2;
        }
    }
}

/* s32 without a return: $v0 stays live at the exit, so as1 leaves the first branch slot as a nop (T-9000) */
s32 func_8013C1CC(void) {
    if (!(D_8015ED98 & 0xFF) && !(D_8015EDC4 & 0xFF) && !(D_8015EDF0 & 0xFF)) {
        if (D_8015EDC4 & 0x100000) {
            if (D_8015EBC0 == 0 || D_8015EBC4 == 0 || D_8015EBC8 == 0) {
                D_8015EE1C = 1;
            }
        } else if (D_8015EDC4 & 0x600000) {
            if (D_8015EDC0 == 0) {
                D_8015EE1C = 1;
            }
        } else if ((D_8015EDC4 & 0x08000000) && D_8015EDC0 == 0) {
            D_8015EE1C = 1;
        }
    }
}

void func_8013C2A0(void) {
    if (D_8015EE1C == 1) {
        switch (D_8015EB98) {
        case 0x100000:
            if (D_8015EDC4 & 0x100000) {
                func_80139510();
                return;
            }
            func_801596E8();
            return;
        case 0x8000000:
            func_8013F82C();
            break;
        }
    }
}

void func_8013C32C(void) {
    s32 endpoint;
    s32 height;
    s32 index;

    if ((D_8015EC4C == 1) && (D_8015EB98 & 0x100000)) {
        index = 0;
        if (D_8015EC20 != 3) {
            D_8015EC38 += D_8015EC48;
            if (D_8015EC38 >= D_8015EC40 * 2) {
                D_8015EC38 = 0;
            }
        }
        do {
            endpoint = index + 7;
            height = (func_800A0070(((index % D_8015EC44) << 12) / D_8015EC44 + ((D_8015EC38 << 12) / 2) / D_8015EC40) * D_8015EC3C) >> 13;
            func_80153F3C(height, (func_800A0070(((endpoint % D_8015EC44) << 12) / D_8015EC44 + ((D_8015EC38 << 12) / 2) / D_8015EC40) * D_8015EC3C) >> 13, index, endpoint, 0, 0);
            index += 8;
        } while (index != 168);
    }
}
