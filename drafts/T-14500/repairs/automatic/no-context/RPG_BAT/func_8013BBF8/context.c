#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013B5B0", func_8013B5B0);

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

/* bulk_m2c declarations (T-11190) */
extern s32 D_8015E550;
extern s32 D_8015E578;
extern s32 D_8015EB98;
extern s32 D_8015EBEC;
extern s32 D_8015EBF0;
extern s32 D_8015EBF4;
extern s32 D_8015EBF8;
extern s32 D_8015ED94;
extern s32 D_8015EDA0;
extern s32 D_8015EDC0;
s32 func_80043010();
/* end bulk_m2c declarations */
void func_8013BBF8(void) {
    typedef struct { s32 w[9]; } M2cBlock24; /* m2c memcpy of 36 bytes */
    typedef struct { s32 w[10]; } M2cBlock28; /* m2c memcpy of 40 bytes */
    s32 sp58[10];
    s32 sp30[10];
    s32 temp_t8;
    s32 temp_v0;
    s32 var_a0;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_4;
    s32 var_v1_5;

    *(M2cBlock24 *)(&sp58) = *(M2cBlock24 *)(&D_8015E550);
    *(s32 *)((unsigned char *)(&sp58) + (36)) = (s32) *(s32 *)((unsigned char *)(&D_8015E550) + (36));
    *(M2cBlock28 *)(&sp30) = *(M2cBlock28 *)(&D_8015E578);
    temp_v0 = func_80043010();
    D_8015EBEC = temp_v0;
    if ((temp_v0 != D_8015EBF0) && (D_8015EBF0 = temp_v0, ((temp_v0 & 0xFFFF) != 0))) {
        if (temp_v0 == *(s32 *)((unsigned char *)(sp + (D_8015EBF8 * 4)) + (88))) {
            var_v1 = D_8015EBF8 + 1;
        } else {
            var_v1 = 0;
        }
        if (temp_v0 == *(s32 *)((unsigned char *)(sp + (D_8015EBF4 * 4)) + (48))) {
            var_a0 = D_8015EBF4 + 1;
        } else {
            var_a0 = 0;
        }
        D_8015EBF8 = var_v1;
        if (var_v1 == 0xA) {
            D_8015EBF8 = 0;
            if (D_8015EB98 != 0x80000000) {
                if ((D_8015EB98 != 0x10000) && (D_8015EB98 != 0x20000) && (D_8015EB98 != 0x40000)) {
                    if (D_8015EB98 != 0x100000) {
                        if (D_8015EB98 == 0x08000000) {
                            D_8015EDC0 = 1;
                            goto block_16;
                        }
                    } else {
                        var_v1_2 = D_8015ED94;
                        if ((var_v1_2 > 0) && (var_v1_2 < 0xA)) {
                            D_8015EDA0 = 0x1661;
                            var_v1_2 = 0x1661;
                        }
                        D_8015ED94 = var_v1_2;
                        goto block_22;
                    }
                } else {
block_16:
                    var_v1_3 = 0x1662;
                    if (D_8015ED94 >= 4) {
                        D_8015EDA0 = 0x1662;
                        goto block_28;
                    }
                }
            } else {
block_22:
                var_v1_4 = D_8015ED94;
                if ((var_v1_4 >= 2) && (var_v1_4 < 0xA)) {
                    D_8015EDA0 = 0x1661;
                    var_v1_4 = 0x1661;
                }
                D_8015ED94 = var_v1_4;
                if (var_v1_4 >= 0xA) {
                    temp_t8 = D_8015EDA0 + 1;
                    D_8015EDA0 = temp_t8;
                    D_8015ED94 = var_v1_4 + 1;
                    var_v1_3 = 0x270F;
                    if (temp_t8 >= 0x2710) {
                        D_8015EDA0 = 0x270F;
block_28:
                        D_8015ED94 = var_v1_3;
                    }
                }
            }
        }
        D_8015EBF4 = var_a0;
        if (var_a0 == 0xA) {
            D_8015EBF4 = 0;
            var_v1_5 = 2;
            switch (D_8015EB98) {
            default:
                var_v1_5 = 1;
                switch (D_8015EB98) {
                case 0x8000000:
                    break;
                case 0x100000:
block_39:
                    D_8015ED94 = var_v1_5;
                    break;
                }
                break;
            case 0x80000000:
                goto block_39;
            }
        }
    } else {
    case 0x10000:
    case 0x20000:
    case 0x40000:
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
            D_8015EBF8 = 0;
            D_8015EBF4 = 0;
            D_8015EBF0 = 0;
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
            D_8015EBF8 = 0;
            D_8015EBF4 = 0;
            D_8015EBF0 = 0;
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
