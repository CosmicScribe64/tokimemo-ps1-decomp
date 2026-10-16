#include "common.h"
#include "ovl/TACO.h"

void func_8013B990(void) {
    func_8013C1F4();
    func_8013C440();
    if (D_800E6280.unk_1109 == 0xC && D_8015EDCC != 0) {
        return;
    }
    if (D_800E6280.unk_1109 == 0x16 && D_8015EDCC >= 2) {
        return;
    }
    if (D_800E6280.unk_1109 == 0x20 && D_8015EDCC >= 3) {
        return;
    }
    func_8013BAC4(D_8015E270, 1, 0);
    func_8013BAC4(D_8015E4F0, 4, 0);
    func_8013BAC4(D_8015E3B0, 1, 1);
    func_8013BAC4(D_8015E630, 4, 1);
    D_8015EDF0 = D_8015EDF0 / 10 * 10 + 1;
    if (D_8015EDF0 >= 0x989680) {
        D_8015EDF0 = 0x98967F;
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

/* FAKE: s32 with no return keeps v0 live at the exit, as in func_8013CE78 (void changes one
 * instruction). T-19006. */
s32 func_8013CFD8(void) {
    u32 i;
    s16 x;
    s16 y;
    s32 a;
    s32 b;
    s32 d;
    s32 t;
    s32 u;
    x = D_8015E770[0].unk0;
    y = D_8015E770[0].unk2;
    for (i = 16; i < 64; i++) {
        if (((i + D_8015EDB4)->unk0[1] & 0x80) && ((i + D_8015EDB4)->unk84[3] & 1) && ((i + D_8015EDB4)->unk84[1] != 0) && ((i + D_8015EDB4)->unk84[0] != 0)) {
            t = func_800AE0C0(x - (i + D_8015EDB4)->unk74);
            u = func_800AE0C0(y - (i + D_8015EDB4)->unk76);
            a = 0x1000 - t;
            if (a < 0) {
                a = 0;
            }
            b = 0x1000 - u;
            if (b < 0) {
                b = 0;
            }
            d = (a + b) / 256;
            if (d != 0) {
                func_8013D2A0((s32)(i + D_8015EDB4)->unk74, (s32)(i + D_8015EDB4)->unk76, (s32)(s16)((i + D_8015EDB4)->unk78 - 0x200));
            }
            if ((u32)d < (i + D_8015EDB4)->unk84[1]) {
                (i + D_8015EDB4)->unk84[1] -= d;
            } else {
                (i + D_8015EDB4)->unk84[1] = 0;
            }
            if ((i + D_8015EDB4)->unk84[1] == 0) {
                func_801335A0(1, 0x508);
                D_8015EDF0 += D_8015F504[(D_8015EDB4 + i)->unk84[2] % 17U];
            }
        }
    }
}
