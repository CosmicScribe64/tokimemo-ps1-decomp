#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013B990);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013BAC4);

/* bulk_m2c declarations (T-11190) */
extern s32 D_8015F500;
/* end bulk_m2c declarations */
void func_8013C1F4(void) {
    Tc14 *var_s0;
    Tc14 *var_s0_2;
    s32 temp_s1;
    s32 temp_s1_2;
    s32 temp_s2;
    s32 temp_s2_2;
    u8 temp_v0;
    u8 temp_v1;

    D_8015F500 = 0;
    var_s0 = D_8015EB30;
    if (!(D_8015EDB4->unk2 & 0x80)) {
loop_2:
        if ((var_s0->unk13 & 0x80) && (temp_s2 = func_800AE0C0(var_s0->unk0 - D_8015EDB4->unk74), temp_s1 = func_800AE0C0(var_s0->unk2 - D_8015EDB4->unk76), ((func_800AE0C0(var_s0->unk4 - D_8015EDB4->unk78) < 0x100) != 0)) && (temp_s1 < 0x80) && (temp_s2 < 0x80)) {
            var_s0->unk13 = 0;
            func_801335A0(1, 0x506);
            D_8015EDB4->unk2 |= 4;
            temp_v1 = D_8015EDB4->unk84[1];
            if (temp_v1 >= 5U) {
                D_8015EDB4->unk84[1] = temp_v1 - 4;
            } else {
                D_8015EDB4->unk84[1] = 0;
            }
            D_8015EDB4->unk7A = 0x10;
        } else {
            var_s0 += 1;
            if (var_s0 != D_8015EC70) {
                goto loop_2;
            }
        }
    }
    var_s0_2 = D_8015EB30;
    if (!(D_8015EDB4[8].unk2 & 0x80)) {
loop_13:
        if (var_s0_2->unk13 & 0x80) {
            temp_s2_2 = func_800AE0C0(var_s0_2->unk0 - D_8015EDB4[8].unk74);
            temp_s1_2 = func_800AE0C0(var_s0_2->unk2 - D_8015EDB4[8].unk76);
            if ((func_800AE0C0(var_s0_2->unk4 - D_8015EDB4[8].unk78) < 0x100) && (temp_s1_2 < 0x80) && (temp_s2_2 < 0x80)) {
                var_s0_2->unk13 = 0;
                temp_v0 = D_8015EDB4[8].unk84[1];
                if (temp_v0 >= 5U) {
                    D_8015EDB4[8].unk84[1] = (u8) (temp_v0 - 4);
                } else {
                    D_8015EDB4[8].unk84[1] = 0U;
                }
                D_8015EDB4[8].unk7A = 0x10;
                return;
            }
        }
        var_s0_2 += 1;
        if (var_s0_2 == D_8015EC70) {

        } else {
            goto loop_13;
        }
    }
}

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
        if (((i + D_8015EDB4)->unk0[1] & 0x80) && ((i + D_8015EDB4)->unk84[3] & 1)) {
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
