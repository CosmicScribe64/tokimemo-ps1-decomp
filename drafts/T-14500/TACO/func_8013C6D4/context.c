#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013B990);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013BAC4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013C1F4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B990", func_8013C440);

s16 func_8013C6D4(void) {
    s32 sp54;
    s32 sp4C;
    s32 sp48;
    s32 sp44;
    Tc14 *var_s0;
    Tc14 *var_s0_2;
    s32 temp_s1;
    s32 temp_s1_2;
    s32 temp_s2;
    s32 temp_s2_2;
    s32 var_v0_2;
    u8 temp_v0;
    u8 temp_v0_2;
    u8 var_s4;
    u8 var_v0;

    sp44 = 1;
    var_s4 = -7U;
    sp54 = -7;
    sp4C = -7;
    sp48 = -7;
    var_s0 = &D_8015EC70;
    do {
        if (var_s0->unk13 & 0x80) {
            temp_s2 = func_800AE0C0(var_s0->unk0 - D_8015EDB4->unk74);
            temp_s1 = func_800AE0C0(var_s0->unk2 - D_8015EDB4->unk76);
            if ((func_800AE0C0(var_s0->unk4 - D_8015EDB4->unk78) < 0x200) && (temp_s1 < 0x200) && (temp_s2 < 0x200)) {
                D_8015EDF0 += 0x3E8;
                temp_v0 = var_s0->padE[3];
                var_s0->unk13 = 0;
                switch (temp_v0) {
                case 0:
                case 1:
                    var_s4 = temp_v0;
                    break;
                case 2:
                    sp54 = 1;
                    break;
                case 3:
                case 4:
                    func_801335A0(1, 0x509);
                    if (D_8015EDB4->unk7C == 0) {
                        D_8015EDB4->unk7C = (s16) var_s0->padE[3];
                    } else if (D_8015EDB4->unk7E == 0) {
                        D_8015EDB4->unk7E = (s16) var_s0->padE[3];
                    } else {
                        D_8015EDB4->unk80 = (s16) var_s0->padE[3];
                    }
                    break;
                case 5:
                    sp48 = 1;
                    temp_v0_2 = D_8015EDB4->unk84[1];
                    if (temp_v0_2 < 0xEFU) {
                        D_8015EDB4->unk84[1] = temp_v0_2 + 0x10;
                    } else {
                        D_8015EDB4->unk84[1] = 0xFF;
                    }
                    break;
                default:
                    D_8015EDD0 = 0x20;
                    sp4C = 1;
                    D_8015EDD4 = 1;
                    switch (func_800AE0D0() & 7) {
                    case 0:
                        var_v0_2 = 0x64;
                        break;
                    case 1:
                        var_v0_2 = 0xC8;
                        break;
                    case 2:
                        var_v0_2 = 0x190;
                        break;
                    case 3:
                        var_v0_2 = 0x320;
                        break;
                    case 4:
                        var_v0_2 = 0x640;
                        break;
                    case 5:
                        var_v0_2 = 0xC80;
                        break;
                    case 6:
                        var_v0_2 = 0x1900;
                        break;
                    case 7:
                        var_v0_2 = 0xDFD4;
                        break;
                    }
                    D_8015EDF0 += var_v0_2;
                    D_8015EDD4 = var_v0_2;
                    break;
                case 7:
                    var_s4 = -1U;
                    break;
                }
            }
        }
        var_s0 += 0x14;
        var_v0 = 0;
    } while ((u32) var_s0 < (u32) &D_8015EDB0);
    var_s0_2 = &D_8015EC70;
    do {
        if (var_s0_2->unk13 & 0x80) {
            var_v0 = var_s0_2->padE[3];
            if ((var_v0 != 3) && (var_v0 != 4)) {
                temp_s2_2 = func_800AE0C0(var_s0_2->unk0 - *(s16 *)((unsigned char *)D_8015EDB4 + (1204)));
                temp_s1_2 = func_800AE0C0(var_s0_2->unk2 - *(s16 *)((unsigned char *)D_8015EDB4 + (1206)));
                var_v0 = func_800AE0C0(var_s0_2->unk4 - *(s16 *)((unsigned char *)D_8015EDB4 + (1208)));
                if (((s32) var_v0 < 0x200) && (temp_s1_2 < 0x200) && (temp_s2_2 < 0x200)) {
                    var_v0 = var_s0_2->padE[3];
                    var_s0_2->unk13 = 0;
                    switch (var_v0) {
                    case 0:
                    case 1:
                    case 7:
                        *(s16 *)((unsigned char *)D_8015EDB4 + (1202)) = (s16) var_v0;
                        break;
                    case 2:
                        var_v0 = (u8) *(s16 *)((unsigned char *)D_8015EDB4 + (1218));
                        if (var_v0 == -1U) {
                            *(s16 *)((unsigned char *)D_8015EDB4 + (1218)) = 1;
                            var_v0 = (u8) *(s16 *)((unsigned char *)D_8015EDB4 + (1218));
                        }
                        *(s16 *)((unsigned char *)D_8015EDB4 + (1218)) = (s16) (1 - var_v0);
                        break;
                    case 5:
                        var_v0 = *(u8 *)((unsigned char *)D_8015EDB4 + (1221));
                        if (var_v0 < 0xEFU) {
                            *(u8 *)((unsigned char *)D_8015EDB4 + (1221)) = (u8) (var_v0 + 0x10);
                        } else {
                            *(u8 *)((unsigned char *)D_8015EDB4 + (1221)) = 0xFFU;
                        }
                        break;
                    }
                }
            }
        }
        var_s0_2 += 0x14;
    } while (var_s0_2 != &D_8015EDB0);
    if (var_s4 != D_8015EDB4->unk72) {
        if (var_s4 != -7U) {
            D_8015EDB4->unk72 = (s16) var_s4;
            switch (var_s4) {
            case -1:
                func_80133410(0);
                var_v0 = func_801335A0(1, 0x500);
block_55:
                sp44 = 0;
                break;
            case 0:
                func_80133410(0);
                var_v0 = func_801335A0(1, 0x514);
                goto block_55;
            case 1:
                func_80133410(0);
                var_v0 = func_801335A0(1, 0x512);
                goto block_55;
            }
        }
    }
    if (sp54 == 1) {
        var_v0 = (u8) D_8015EDB4->unk82;
        if (var_v0 == -1U) {
            D_8015EDB4->unk82 = 1;
            var_v0 = (u8) D_8015EDB4->unk82;
        }
        D_8015EDB4->unk82 = 1 - var_v0;
        if (sp44 != 0) {
            var_v0 = (u8) D_8015EDB4->unk82;
            switch (var_v0) {
            case 0:
                sp44 = 0;
                var_v0 = func_801335A0(1, 0x511);
                break;
            case 1:
                sp44 = 0;
                var_v0 = func_801335A0(1, 0x516);
                break;
            }
        }
    }
    if (sp48 == 1) {
        if (sp44 != 0) {
            sp44 = 0;
            var_v0 = func_801335A0(1, 0x510);
        }
    }
    if (sp4C == 1) {
        if (sp44 != 0) {
            var_v0 = func_801335A0(1, 0x509);
        }
    }
    return (s16) var_v0;
}

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
