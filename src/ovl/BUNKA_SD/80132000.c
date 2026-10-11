#include "common.h"
#include "ovl/BUNKA_SD.h"

s32 func_80132000(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80044890(1, 0xBEFB, 0xBEE2, 0xD258, 0xD208, 0xD200);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    } else if (D_800E6280.unk_110D == 1) {
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        if (func_80044F94(1) == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else if (D_800E6280.unk_110D == 3) {
        func_80046318(0x52, 0x80180000, 0x4B82);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    } else if (D_800E6280.unk_110D == 4) {
        if (func_800460CC() & 1) {
            func_8004284C();
        }
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80132000", func_80132148);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80132000", func_801322EC);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80132000", func_80132428);

void func_801325E8(void) {
    func_80132940();
    k_speed_set(5);
    D_8013B7EC = 0;
    func_8004284C();
}

void func_8013261C(void) {
    if (D_800E6280.unk_1104.w == 1) {
        D_8013B808 = 0;
        D_801206DB = 0x84;
        D_801206DA = 5;
    }
    /* FAKE: the ^ 0 keeps the constant as the left operand of the bne (decomp-permuter); real source unknown. T-8060 */
    if ((D_80122EAC != 4) && (0x14 == (D_801206F0 ^ 0))) {
        D_8013B808 += 1;
        if (D_8013B808 == 0x14) {
            func_80044750(0x501);
        }
    }
    if ((D_80122EAC == 4) && (D_801206F0 == 5)) {
        D_8013B808 += 1;
        if (D_8013B808 == 0x14) {
            func_80044750(0x501);
        }
    }
    if (!((u8) D_801206DA & 1)) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80132000", func_80132708);

void func_801327E4(void) {
    if (D_800E6280.unk_1104.w == 1) {
        D_8013B80C = 0;
        D_801206DB = 4;
        D_8012071F = 0x84;
        D_8012071E = 5;
    }
    if ((D_80122EAC != 3) && (D_80122EAC != 4)) {
        if (D_80120734 == 0) {
            D_8013B80C += 1;
            if (D_8013B80C == 0x14) {
                func_80044750(0x501);
            }
        }
    }
    if ((D_80122EAC == 3) && (D_80120734 == 0xB)) {
        D_8013B80C += 1;
        if (D_8013B80C == 1) {
            func_80044750(0x501);
        }
    }
    if ((D_80122EAC == 4) && (D_80120734 == 5)) {
        D_8013B80C += 1;
        if (D_8013B80C == 1) {
            func_80044750(0x501);
        }
    }
    if (!((u8) D_8012071E & 1)) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80132000", func_80132940);

void func_80132DB8(u32 arg0) {
    func_80132708();
    if (arg0 < (u32)D_800E6280.unk_1104.u) {
        func_8004284C();
    }
}

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139910);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139920);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139938);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139958);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139970);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139988);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139998);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_801399B4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_801399C8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_801399E8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139A00);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139A20);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139A40);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139A54);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139A74);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139A80);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139A9C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139ABC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139AD0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139AE4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139B00);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139B0C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139B30);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139B44);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139B64);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139B78);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139B88);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139BA4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139BBC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139BDC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139BF8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139C10);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139C24);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139C38);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139C4C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139C74);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139C8C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139CA4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139CBC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139CD8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139CF0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139D18);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139D30);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139D48);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139D74);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139D8C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139DA8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139DC0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139DD0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139DF4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139E0C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139E24);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139E34);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139E4C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139E68);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139E80);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139E94);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139EA8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139EC4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139EDC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139EF4);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80132000", func_80132DFC);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80132000", func_80132FF0);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80132000", func_80133204);
