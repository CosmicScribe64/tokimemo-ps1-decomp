#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013B990);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013BAC4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013C1F4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013C440);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013C6D4);

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7378;
extern s32 subroutine_arg1;
extern s32 subroutine_arg2;
extern s32 subroutine_arg3;
extern s32 subroutine_arg0;
/* end bulk_m2c declarations */
s32 func_8013CC40(void) {
    s32 var_s1;
    s32 var_v0;
    u8 *temp_s0;

    var_s1 = 0x880;
    var_v0 = 0x10;
    do {
        temp_s0 = &D_8015EDB4->pad0[var_s1];
        if ((*(u8 *)((unsigned char *)temp_s0 + (133)) == 0) && (*(u8 *)((unsigned char *)temp_s0 + (1)) & 0x80)) {
            if (!(D_800E7378 & 0x1F)) {
                func_801335A0(1, 0x508);
            }
            func_8013D1E0(subroutine_arg0, subroutine_arg1, subroutine_arg2, subroutine_arg3, /* extra? */ 0);
            var_v0 = func_8013D1E0(subroutine_arg0, subroutine_arg1, subroutine_arg2, subroutine_arg3, /* extra? */ 0);
            if (var_s1 == 0x440) {
                *(s8 *)((unsigned char *)(&D_8015EDB4->pad0[var_s1]) + (133)) = (s8) (func_800AE0D0() & 0x3F);
                *(s16 *)((unsigned char *)(&D_8015EDB4->pad0[var_s1]) + (116)) = (s16) (func_800AE0D0() % 100);
                var_v0 = func_800AE0D0();
                *(s16 *)((unsigned char *)(&D_8015EDB4->pad0[var_s1]) + (118)) = (s16) (var_v0 % 100);
            }
        }
        var_s1 += 0x88;
    } while (var_s1 != 0x2200);
    return var_v0;
}

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
