#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80141DE0", func_80141DE0);

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

s32 func_80142220(s32 arg0) {
    Tc14 *p;

    for (p = D_8015E770; p < &D_8015E770[16]; p++) {
        if (p->unk13 & 0x80) {
            return -1;
        }
    }
    if (D_80160460 != 0) {
        return 1;
    }
    if (D_8015EDB4->unk2 & 0x80) {
        return 0;
    }
    if (arg0 & 0x40) {
        if (func_801421AC() == 3) {
            D_8015EDB4->unk2 |= 0xC0;
            func_801334BC();
            func_8013356C();
            D_80160460 = 1;
            return 3;
        }
        if (func_801421AC() == 4) {
            D_8015EDB4->unk2 |= 0xC0;
            func_801334BC();
            func_8013356C();
            D_8015E770[0].unk13 = 0x90;
            D_8015E770[0].unk0 = D_8015EDB4->unk74;
            D_8015E770[0].unk2 = D_8015EDB4->unk76;
            D_8015E770[0].unk4 = D_8015EDB4->unk78 - 0xC8;
            D_8015E770[0].unk8 = 0;
            D_8015E770[0].unkA = 0;
            D_8015E770[0].unkC = -8;
            D_8015E770[0].unk12 = 0;
            func_801335A0(1, 0x505);
            return 4;
        }
    }
    return 0;
}

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
