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
            if (D_8015EE44 >= 3 && D_8015EE4C >= 3) {
                D_8015EE4C = D_8015E744 + 1;
            }
        }
        if (D_8015EDC4 != 0x08000002 && (D_8015EDC4 & 0x08000000) && D_8015EE1C != 1) {
            if (D_8015EE50 == 2 || D_8015EE50 == 3) {
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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013B5B0", func_8013BBF8);

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
            if (D_8015EE44 >= 3 && D_8015EE4C >= 3) {
                D_8015EE4C = D_8015E744 - 5;
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
            if (D_8015EE44 >= 3 && D_8015EE4C >= 3) {
                D_8015EE4C = D_8015E744 - 5;
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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013B5B0", func_8013C1CC);

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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013B5B0", func_8013C32C);
