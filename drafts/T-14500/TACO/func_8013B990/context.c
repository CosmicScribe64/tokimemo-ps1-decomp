#include "common.h"
#include "ovl/TACO.h"

/* bulk_m2c declarations (T-11190) */
extern u8 D_800E7389;
/* end bulk_m2c declarations */
void func_8013B990(void) {
    s32 temp_v1;

    func_8013C1F4();
    func_8013C440();
    if (((D_800E7389 != 0xC) || (D_8015EDCC == 0)) && ((D_800E7389 != 0x16) || ((u8) D_8015EDCC < 2U))) {
        if (D_800E7389 == 0x20) {
            if ((u8) D_8015EDCC < 3U) {
                goto block_8;
            }
            return;
        }
block_8:
        func_8013BAC4((s32) D_8015E270, 1, 0);
        func_8013BAC4((s32) D_8015E4F0, 4, 0);
        func_8013BAC4((s32) D_8015E3B0, 1, 1);
        func_8013BAC4((s32) D_8015E630, 4, 1);
        temp_v1 = (((s32) D_8015EDF0 / 10) * 0xA) + 1;
        D_8015EDF0 = temp_v1;
        if (temp_v1 >= 0x989680) {
            D_8015EDF0 = 0x98967F;
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013BAC4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013C1F4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013C440);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013C6D4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013CC40);

void func_8013CE70(void) {
}

/* FAKE: s32 fall-through and the two pointer-addition orders retain the
 * original result/address registers. T-10010. */
s32 func_8013CE78(void) {
    u32 i;
    u8 value;
    for (i = 16; i < 64; i++) {
        if (((i + D_8015EDB4)->pad0[1] & 0x80) && ((i + D_8015EDB4)->unk84[3] & 1)) {
            value = (i + D_8015EDB4)->unk84[1];
            if (value != 0 && (i + D_8015EDB4)->unk84[0] != 0) {
                if (value >= 3U) {
                    (i + D_8015EDB4)->unk84[1] = value - 2;
                } else {
                    (i + D_8015EDB4)->unk84[1] = 0;
                }
                func_8013D2A0((s32)(i + D_8015EDB4)->unk74, (s32)(i + D_8015EDB4)->unk76, (s32)(s16)((i + D_8015EDB4)->unk78 - 0x200));
                if ((i + D_8015EDB4)->unk84[1] == 0) {
                    func_801335A0(1, 0x508);
                    D_8015EDF0 += D_8015F504[(D_8015EDB4 + i)->unk84[2] % 17U];
                }
            }
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013CFD8);
