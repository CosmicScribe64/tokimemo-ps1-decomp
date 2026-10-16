#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013B990);

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

s32 func_8013CFD8(void) {
    TcActor *var_v1;
    s16 temp_s3;
    s16 temp_s4;
    s32 temp_s0;
    s32 temp_t6;
    s32 var_a0;
    s32 var_a1;
    s32 var_s1;
    u8 *temp_v1;
    u8 *temp_v1_2;
    u8 var_v0;

    temp_s3 = D_8015E770->unk0;
    temp_s4 = D_8015E770[0].unk2;
    var_s1 = 0x880;
    var_v0 = 0x10;
    var_v1 = &D_8015EDB4[16];
    do {
        if ((var_v1->unk0[1] & 0x80) && (var_v1->unk84[3] & 1) && (var_v1->unk84[1] != 0) && (var_v1->unk84[0] != 0)) {
            temp_s0 = func_800AE0C0(temp_s3 - var_v1->unk74);
            var_a0 = 0x1000 - temp_s0;
            var_a1 = 0x1000 - func_800AE0C0(temp_s4 - *(s16 *)((unsigned char *)(&D_8015EDB4->unk0[var_s1]) + (118)));
            if (var_a0 < 0) {
                var_a0 = 0;
            }
            if (var_a1 < 0) {
                var_a1 = 0;
            }
            temp_t6 = (s32) (var_a0 + var_a1) / 256;
            if (temp_t6 != 0) {
                temp_v1 = &D_8015EDB4->unk0[var_s1];
                func_8013D2A0((s32) *(s16 *)((unsigned char *)temp_v1 + (116)), (s32) *(s16 *)((unsigned char *)temp_v1 + (118)), (s32) (s16) (*(s16 *)((unsigned char *)temp_v1 + (120)) - 0x200));
            }
            temp_v1_2 = &D_8015EDB4->unk0[var_s1];
            var_v0 = *(u8 *)((unsigned char *)temp_v1_2 + (133));
            if ((u32) temp_t6 < var_v0) {
                *(u8 *)((unsigned char *)temp_v1_2 + (133)) = (u8) (var_v0 - temp_t6);
            } else {
                *(u8 *)((unsigned char *)temp_v1_2 + (133)) = 0U;
            }
            var_v1 = (TcActor *) &D_8015EDB4->unk0[var_s1];
            if (var_v1->unk84[1] == 0) {
                var_v0 = func_801335A0(1, 0x508);
                var_v1 = (TcActor *) &D_8015EDB4->unk0[var_s1];
                D_8015EDF0 += D_8015F504[(u8) var_v1->unk84[2] % 17U];
            }
        }
        var_s1 += 0x88;
        var_v1 += 1;
    } while (var_s1 != 0x2200);
    return (s32) var_v0;
}
