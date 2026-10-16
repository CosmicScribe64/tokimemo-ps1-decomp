#include "common.h"
#include "ovl/TACO.h"

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7378;
extern u8 D_8015E782;
s16 func_801420DC();
s32 func_801423B0(void);
/* end bulk_m2c declarations */
s32 func_80141DE0(s32 arg0) {
    Tc14 *temp_v0;
    Tc14 *temp_v0_2;
    Tc14 *var_v0_2;
    s16 temp_v0_3;
    s32 var_v0;
    u32 var_v1;

    if (func_80142220(arg0) == 0) {
        return 0;
    }
    if ((func_80142220(arg0) != 4) && (func_80142220(arg0) != 3)) {
        if (func_80142220(arg0) == 1) {
            if (!(D_800E7378 & 3) && (D_80160464 >= 0x41)) {
                func_8013CE78();
                func_801335A0(1, 0x505);
            }
        } else if (!(D_8015E782 & 3)) {
            var_v1 = (u8) D_8015E782 >> 2;
            if ((u8) D_8015E782 >= 0x81U) {
                func_8013CFD8();
                func_801335A0(1, 0x504);
                var_v1 = (u8) D_8015E782 >> 2;
            }
            if ((s32) var_v1 < 0x10) {
                temp_v0 = &D_8015E770[var_v1];
                temp_v0->unk13 = 0x90;
                temp_v0->unk0 = D_8015EDB4->unk74;
                temp_v0->unk2 = D_8015EDB4->unk76 + 0x10;
                temp_v0->unk8 = 0;
                temp_v0->unk4 = D_8015EDB4->unk78 - 0x64;
                temp_v0->unkA = 0;
                temp_v0->unkC = -0x20;
                temp_v0->unk12 = 0;
            }
            temp_v0_2 = &D_8015E770[var_v1];
            if (((u8) temp_v0_2->unk12 >= 0x11U) && ((u8) D_8015E782 < 0x80U)) {
                temp_v0_2->unkC = -0x100;
            }
            temp_v0_3 = *(s16 *)((unsigned char *)D_8015EDB4 + (1204));
            *(s16 *)((unsigned char *)D_8015EDB4 + (1204)) = (s16) (temp_v0_3 + ((s32) (D_8015EDB4->unk74 - temp_v0_3) / 2));
            *(s16 *)((unsigned char *)D_8015EDB4 + (1206)) = (s16) (*(s16 *)((unsigned char *)D_8015EDB4 + (1206)) + ((s32) (D_8015EDB4->unk76 - *(s16 *)((unsigned char *)D_8015EDB4 + (1206))) / 2));
            *(s16 *)((unsigned char *)D_8015EDB4 + (1208)) = (s16) (*(s16 *)((unsigned char *)D_8015EDB4 + (1208)) + ((s32) ((D_8015EDB4->unk78 - *(s16 *)((unsigned char *)D_8015EDB4 + (1208))) - 0x200) / 2));
        }
    }
    if (D_80160460 != 0) {
        var_v0 = func_80154014();
        if (var_v0 == 0) {
            D_8015EDB4->unk2 &= 0xFF3F;
            func_8013CE78();
            return func_801420DC();
        }
        /* Duplicate return node #25. Try simplifying control flow for better match */
        return var_v0;
    }
    var_v0 = func_801423B0();
    if (var_v0 == 0) {
        var_v0_2 = D_8015E770;
        D_8015EDB4->unk2 &= 0xFF3F;
        do {
            var_v0_2 += 0x50;
            *(s8 *)((unsigned char *)var_v0_2 + (-41)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (-21)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (-1)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (-61)) = 0;
        } while (var_v0_2 != D_8015E8B0);
        func_8013CFD8();
        var_v0 = func_801420DC();
    }
    return var_v0;
}

s16 func_801420DC() {
    if (D_8015EDB4->unk80 == 3 || D_8015EDB4->unk80 == 4) {
        D_8015EDB4->unk80 = 0;
        return D_8015EDB4->unk80;
    }
    if (D_8015EDB4->unk7E == 3 || D_8015EDB4->unk7E == 4) {
        D_8015EDB4->unk7E = 0;
        return D_8015EDB4->unk7E;
    }
    if (D_8015EDB4->unk7C == 3 || D_8015EDB4->unk7C == 4) {
        D_8015EDB4->unk7C = 0;
        return D_8015EDB4->unk7C;
    }
    return 0;
}

s16 func_801421AC() {
    if (D_8015EDB4->unk80 == 3 || D_8015EDB4->unk80 == 4) {
        return D_8015EDB4->unk80;
    }
    if (D_8015EDB4->unk7E == 3 || D_8015EDB4->unk7E == 4) {
        return D_8015EDB4->unk7E;
    }
    if (D_8015EDB4->unk7C == 3 || D_8015EDB4->unk7C == 4) {
        return D_8015EDB4->unk7C;
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80141DE0", func_80142220);

s32 func_801423B0(void) {
    Tc14 *var_s0;
    s32 var_s1;
    u8 temp_v0;

    var_s0 = D_8015E770;
    var_s1 = 0;
loop_1:
    temp_v0 = var_s0->unk12;
    if (temp_v0 >= 0xFFU) {
        var_s0->unk13 = 0;
        return 0;
    }
    if (temp_v0 >= 0x80U) {
        var_s0->unkC = -0x400;
    }
    if (var_s0->unk4 < -0x7D00) {
        var_s0->unk13 = 0;
    }
    if (var_s0->unk13 & 0x80) {
        var_s0->unk12++;
        var_s0->unk0 += var_s0->unk8;
        var_s0->unk2 += var_s0->unkA;
        var_s0->unk4 += var_s0->unkC;
        /* A saved byte or extra mask changes IDO's reload here (T-9560). */
        func_801424A8(var_s1, var_s0->unk12);
    }
    var_s1 += 1;
    var_s0 += 1;
    if (var_s1 == 0x10) {
        return 1;
    }
    goto loop_1;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80141DE0", func_801424A8);
