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

/* FAKE: s32 with no return keeps the loop counter init (v0) live at the exit (void drops two
 * instructions); the hit in the second loop returns before it. T-19006. */
s32 func_8013C1F4(void) {
    u32 i;
    s32 a;
    s32 b;
    u8 value;
    u8 value2;
    D_8015F500 = 0;
    if (!(D_8015EDB4->unk2 & 0x80)) {
        for (i = 0; i < 16; i++) {
            if (D_8015EB30[i].unk13 & 0x80) {
                a = func_800AE0C0(D_8015EB30[i].unk0 - D_8015EDB4->unk74);
                b = func_800AE0C0(D_8015EB30[i].unk2 - D_8015EDB4->unk76);
                if (func_800AE0C0(D_8015EB30[i].unk4 - D_8015EDB4->unk78) < 0x100 && b < 0x80 && a < 0x80) {
                    D_8015EB30[i].unk13 = 0;
                    func_801335A0(1, 0x506);
                    D_8015EDB4->unk2 |= 4;
                    value = D_8015EDB4->unk84[1];
                    if (value >= 5) {
                        D_8015EDB4->unk84[1] = value - 4;
                    } else {
                        D_8015EDB4->unk84[1] = 0;
                    }
                    D_8015EDB4->unk7A = 0x10;
                    break;
                }
            }
        }
    }
    if (!(D_8015EDB4[8].unk2 & 0x80)) {
        for (i = 0; i < 16; i++) {
            if (D_8015EB30[i].unk13 & 0x80) {
                a = func_800AE0C0(D_8015EB30[i].unk0 - D_8015EDB4[8].unk74);
                b = func_800AE0C0(D_8015EB30[i].unk2 - D_8015EDB4[8].unk76);
                if (func_800AE0C0(D_8015EB30[i].unk4 - D_8015EDB4[8].unk78) < 0x100 && b < 0x80 && a < 0x80) {
                    D_8015EB30[i].unk13 = 0;
                    value2 = D_8015EDB4[8].unk84[1];
                    if (value2 >= 5) {
                        D_8015EDB4[8].unk84[1] = value2 - 4;
                    } else {
                        D_8015EDB4[8].unk84[1] = 0;
                    }
                    D_8015EDB4[8].unk7A = 0x10;
                    return;
                }
            }
        }
    }
}

/* FAKE: s32 with no return keeps the loop counter init (v0) live at the exit (void changes the output),
 * as in func_8013C1F4. The first loop is entered by a goto instead of an enclosing if: the if form
 * shares the guard's load of D_8015EDB4 with the loop's pointer, the original loads it again. T-19006. */
s32 func_8013C440(void) {
    u32 i;
    s32 a;
    s32 b;
    s32 c;
    s32 limit;
    u8 value;
    u8 value2;
    D_8015F500 = 0;
    if (D_8015EDB4->unk2 & 0x80) {
        goto skip1;
    }
    for (i = 16; i < 63; i++) {
        if (((i + D_8015EDB4)->unk0[1] & 0xC0) && ((i + D_8015EDB4)->unk84[1] != 0)) {
            a = func_800AE0C0((i + D_8015EDB4)->unk74 - D_8015EDB4->unk74);
            b = func_800AE0C0((i + D_8015EDB4)->unk76 - D_8015EDB4->unk76);
            c = func_800AE0C0((i + D_8015EDB4)->unk78 - D_8015EDB4->unk78);
            limit = (s32)(i + D_8015EDB4)->unk84[0] << 4;
            if (c < 0x100 && b < limit && a < limit) {
                func_801335A0(1, 0x506);
                D_8015EDB4->unk2 |= 4;
                value = D_8015EDB4->unk84[1];
                if (value >= 0xD) {
                    D_8015EDB4->unk84[1] = value - 0xC;
                } else {
                    D_8015EDB4->unk84[1] = 0;
                }
                D_8015EDB4->unk7A = 0x10;
                break;
            }
        }
    }
skip1:
    if (D_8015EDB4[8].unk2 & 0x80) {
        return;
    }
    for (i = 0; i < 16; i++) {
        if (((i + D_8015EDB4)->unk0[1] & 0xC0) && ((i + D_8015EDB4)->unk84[1] != 0)) {
            a = func_800AE0C0((i + D_8015EDB4)->unk74 - D_8015EDB4[8].unk74);
            b = func_800AE0C0((i + D_8015EDB4)->unk76 - D_8015EDB4[8].unk76);
            c = func_800AE0C0((i + D_8015EDB4)->unk78 - D_8015EDB4[8].unk78);
            limit = (s32)(i + D_8015EDB4)->unk84[0] << 4;
            if (c < 0x100 && b < limit && a < limit) {
                value2 = D_8015EDB4[8].unk84[1];
                if (value2 >= 0xD) {
                    D_8015EDB4[8].unk84[1] = value2 - 0xC;
                } else {
                    D_8015EDB4[8].unk84[1] = 0;
                }
                D_8015EDB4[8].unk7A = 0x10;
                return;
            }
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013C6D4);

/* FAKE: s32 with no return keeps v0 live at the exit (void changes one instruction), as in
 * func_8013CE78. The i == 8 test is dead (the loop starts at 16) but the original has it. T-19006. */
s32 func_8013CC40(void) {
    u32 i;
    for (i = 16; i < 64; i++) {
        if (((i + D_8015EDB4)->unk84[1] == 0) && ((i + D_8015EDB4)->unk0[1] & 0x80)) {
            if (!(D_800E6280.unk_10F8 & 0x1F)) {
                func_801335A0(1, 0x508);
            }
            func_8013D1E0(*(TcPos *)&(i + D_8015EDB4)->unk74, *(TcPos *)&(i + D_8015EDB4)->unk7C, 0);
            func_8013D1E0(*(TcPos *)&(i + D_8015EDB4)->unk74, *(TcPos *)&(i + D_8015EDB4)->unk7C, 0);
            if (i == 8) {
                (D_8015EDB4 + i)->unk84[1] = func_800AE0D0() & 0x3F;
                (D_8015EDB4 + i)->unk74 = func_800AE0D0() % 100;
                (D_8015EDB4 + i)->unk76 = func_800AE0D0() % 100;
            }
        }
    }
}

void func_8013CE70(void) {
}

/* FAKE: s32 fall-through and the two pointer-addition orders retain the
 * original result/address registers. T-10010. */
s32 func_8013CE78(void) {
    u32 i;
    u8 value2;
    for (i = 16; i < 64; i++) {
        if (((i + D_8015EDB4)->unk0[1] & 0x80) && ((i + D_8015EDB4)->unk84[3] & 1)) {
            value2 = (i + D_8015EDB4)->unk84[1];
            if (value2 != 0 && (i + D_8015EDB4)->unk84[0] != 0) {
                if (value2 >= 3U) {
                    (i + D_8015EDB4)->unk84[1] = value2 - 2;
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
