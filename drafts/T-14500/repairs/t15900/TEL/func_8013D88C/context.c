#include "common.h"
#include "ovl/TEL.h"

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/8013CCD0", func_8013CCD0);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/8013CCD0", func_8013D03C);

/* bulk_m2c declarations (T-11190) */
extern s32 D_800B3454;
extern s32 D_800B346C;
extern s32 D_800B3470;
extern s32 D_800B3474;
extern s32 D_800B3478;
extern s32 D_800B347C;
extern s32 D_800E71E4;
extern s8 D_800E7313;
extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E71E5;
/* end bulk_m2c declarations */
void func_8013D88C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s16 spC0[12];
    s16 spA8[12];
    s16 sp90[12];
    s16 sp78[12];
    s16 *sp64;
    s16 *sp5C;
    GameState *sp58;
    GameState *var_v1;
    GameState *var_v1_2;
    s16 *temp_s2;
    s16 *temp_s2_2;
    s16 *temp_s2_3;
    s16 *var_a0;
    s16 *var_a1;
    s16 *var_fp;
    s16 *var_fp_2;
    s16 *var_fp_3;
    s16 *var_s2;
    s16 *var_s2_2;
    s16 *var_s2_3;
    s16 temp_s0;
    s16 temp_s0_2;
    s16 temp_s0_3;
    s16 temp_s1;
    s16 temp_s1_2;
    s16 temp_s1_3;
    s32 var_s3_3;
    s32 var_s4;
    u32 var_s3;
    u32 var_s3_2;

    var_s2 = sp90;
    do {
        var_s2 += 2;
        *(s16 *)((unsigned char *)var_s2 + (-2)) = 0;
    } while ((u32) var_s2 < (u32) &sp90[10]);
    sp78[10] = 0x10;
    sp90[10] = 0x64;
    spA8[10] = -0x54;
    spC0[10] = 0;
    if (arg0 == 0) {
        set_kanji_string(-0x80, -0x52, 3, (s32) "デートスポット", 0);
        set_kanji_string(0, -0x52, 3, (s32) "次のページへ", 0);
        var_v1 = &D_800E6280;
        var_s3 = 0;
        var_s2_2 = sp90;
        var_fp = spC0;
        var_a0 = spA8;
        var_a1 = sp78;
        do {
            if (var_v1->unk_120[0] & 0x7F) {
                goto block_27;
            }
            *var_s2_2 = 0;
            switch (var_s3) {
            case 1:
                if ((((u32) (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) << 0x1E) >> 0x1F) == 1) && (arg1 == 0x5F) && (arg2 == 6)) {
block_27:
                    *var_s2_2 = 0x74;
                }
                break;
            case 3:
                if ((((u32) (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) << 0x1C) >> 0x1F) == 1) && (arg1 == 0x5F) && (arg2 == 0xC)) {
                    goto block_27;
                }
                break;
            case 5:
                if ((((u32) (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) << 0x1A) >> 0x1F) == 1) && (arg1 == 0x60) && (arg2 == 6)) {
                    goto block_27;
                }
                break;
            case 6:
                if ((((u8) D_800E6280.unk_F64[0] >> 7) == 1) && (arg1 == 0x60) && (arg2 == 0xC)) {
                    goto block_27;
                }
                break;
            case 7:
                if ((((u32) (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) << 0x1D) >> 0x1F) == 1) && (arg1 == 0x5F) && (arg2 == 9)) {
                    goto block_27;
                }
                break;
            }
            *var_a1 = 0x10;
            temp_s0 = (- -((s32) var_s3 / 5) * 0x88) - 0x88;
            temp_s1 = (((s32) var_s3 % 5) * 0x10) - 0x38;
            *var_fp = temp_s0;
            *var_a0 = temp_s1;
            if (*var_s2_2 != 0) {
                sp5C = var_a1;
                sp64 = var_a0;
                sp58 = var_v1;
                set_kanji_string(temp_s0, temp_s1, 3, (s32) D_800B342C[var_s3], 0);
            }
            var_s3 += 1;
            var_s2_2 += 2;
            var_fp += 2;
            var_a0 += 2;
            var_a1 += 2;
            var_v1 += 1;
        } while (var_s3 != 0xA);
    } else {
        set_kanji_string(-0x80, -0x52, 3, (s32) "デートスポット", 0);
        set_kanji_string(0, -0x52, 3, (s32) "前のページへ", 0);
        var_v1_2 = &D_800E6280;
        sp5C = sp78;
        sp64 = spA8;
        var_s3_2 = 0;
        var_s2_3 = sp90;
        var_fp_2 = spC0;
        do {
            if (var_v1_2->unk_120[0xA] & 0x7F) {
                if ((s32) var_s3_2 < 3) {
                    goto block_86;
                }
                if (var_s3_2 == 3) {
                    switch (arg2) {
                    case 12:
                    case 2:
                    case 0:
                        break;
                    default:
                        if (arg2 == D_800E62BF) {
                            if (((u32) (D_800E71E4 << 0x11) >> 0x1F) == 1) {
                                goto block_86;
                            }
                        } else if (((u8) D_800E62BF % 3U) == 2) {
                            if (((u32) (D_800E71E4 << 0x12) >> 0x1F) == 1) {
                                goto block_86;
                            }
                        } else if (((u32) (D_800E71E4 << 0x11) >> 0x1F) == 1) {
                            goto block_86;
                        }
                        break;
                    }
                } else if (var_s3_2 == 4) {
                    if (arg2 == D_800E62BF) {
                        if (((u32) (D_800E71E4 << 0x11) >> 0x1F) == 1) {
                            goto block_86;
                        }
                    } else if (((u8) D_800E62BF % 3U) == 2) {
                        if (((u32) (D_800E71E4 << 0x12) >> 0x1F) == 1) {
                            goto block_86;
                        }
                    } else if (((u32) (D_800E71E4 << 0x11) >> 0x1F) == 1) {
                        goto block_86;
                    }
                } else if (var_s3_2 == 5) {
                    if (arg2 == D_800E62BF) {
                        if (((u32) (D_800E71E4 << 0x11) >> 0x1F) == 1) {
                            goto block_86;
                        }
                    } else if (((u8) D_800E62BF % 3U) == 2) {
                        if (((u32) (D_800E71E4 << 0x12) >> 0x1F) == 1) {
                            goto block_86;
                        }
                    } else if (((u32) (D_800E71E4 << 0x11) >> 0x1F) == 1) {
                        goto block_86;
                    }
                }
            } else {
                switch (var_s3_2) {
                case 0:
                    if ((((u32) (D_800E71E4 << 0x19) >> 0x1F) == 1) && (arg1 == 0x60) && (arg2 >= 9)) {
block_86:
                        *var_s2_3 = 0x74;
                    default:
                    case 14:;
                    }
                    break;
                case 1:
                    if (((D_800E71E5 & 1) == 1) && (arg1 == 0x61) && (arg2 >= 3)) {
                        goto block_86;
                    }
                    break;
                case 3:
                    if ((((u32) (D_800E71E4 << 0x12) >> 0x1F) == 1) && (D_800E62BF == 2) && (arg2 == 3) && (D_800E62BE == 0x60)) {
                        goto block_86;
                    }
                    break;
                case 4:
                    if ((((u32) (D_800E71E4 << 0x12) >> 0x1F) == 1) && (arg1 == 0x5F) && (arg2 == 6) && (D_800E62BF == 5)) {
                        goto block_86;
                    }
                    break;
                }
            }
            temp_s0_2 = (- -((s32) var_s3_2 / 5) * 0x88) - 0x88;
            *var_fp_2 = temp_s0_2;
            temp_s1_2 = (((s32) var_s3_2 % 5) * 0x10) - 0x38;
            *sp64 = temp_s1_2;
            *sp5C = 0x10;
            if (*var_s2_3 != 0) {
                sp58 = var_v1_2;
                set_kanji_string(temp_s0_2, temp_s1_2, 3 & 0xFF, *(&D_800B3454 + (var_s3_2 * 4)), 0);
            }
            var_s3_2 += 1;
            sp5C += 2;
            sp64 += 2;
            var_s2_3 += 2;
            var_fp_2 += 2;
            var_v1_2 += 1;
        } while ((s32) var_s3_2 < 6);
        sp5C = &sp78[6];
        sp64 = &spA8[6];
        var_s3_3 = 6;
        var_s4 = 0xC;
        var_fp_3 = &spC0[6];
        do {
            if (var_s3_3 == 6) {
                temp_s2 = (s16 *)((u8 *)sp90 + var_s4);
                if (func_80066B40(arg2, arg3) == 1) {
                    set_kanji_string((s16) ((1 - (var_s3_3 / 5)) * -0x88), (s16) (((var_s3_3 % 5) * 0x10) - 0x38), 3, D_800B346C, 0);
                    *temp_s2 = 0x74;
                } else if (func_80066B40(arg2, arg3) == 2) {
                    set_kanji_string((s16) ((- -(var_s3_3 / 5) * 0x88) - 0x88), (s16) (((var_s3_3 % 5) * 0x10) - 0x38), 3, D_800B3478, 0);
                    *temp_s2 = 0x74;
                } else {
                    *temp_s2 = 0;
                }
            }
            temp_s0_3 = (- -(var_s3_3 / 5) * 0x88) - 0x88;
            temp_s1_3 = ((var_s3_3 % 5) * 0x10) - 0x38;
            if (var_s3_3 == 7) {
                temp_s2_2 = (s16 *)((u8 *)sp90 + var_s4);
                if (func_80066B40(arg2, arg3) == 2) {
                    set_kanji_string(temp_s0_3, temp_s1_3, 3, D_800B347C, 0);
                    *temp_s2_2 = 0x74;
                } else if (func_80066B40(arg2, arg3) == 1) {
                    set_kanji_string(temp_s0_3, temp_s1_3, 3, D_800B3470, 0);
                    *temp_s2_2 = 0x74;
                } else {
                    *temp_s2_2 = 0;
                }
            }
            if (var_s3_3 == 8) {
                sp90[var_s4 / 2] = 0;
            }
            if (var_s3_3 == 9) {
                temp_s2_3 = (s16 *)((u8 *)sp90 + var_s4);
                if ((((u16) D_800E71E4 >> 0xF) == 1) && (arg2 == 8) && (((s32) (arg3 + 6) / 7) == 1)) {
                    set_kanji_string(temp_s0_3, temp_s1_3, 3, D_800B3474, 0);
                    *temp_s2_3 = 0x74;
                } else {
                    *temp_s2_3 = 0;
                }
            }
            *var_fp_3 = temp_s0_3;
            *sp64 = temp_s1_3;
            var_s3_3 += 1;
            *sp5C = 0x10;
            sp5C += 2;
            sp64 += 2;
            var_s4 += 2;
            var_fp_3 += 2;
        } while (var_s3_3 != 0xA);
    }
    menu_set(0, 0xB, spC0, spA8, sp90, sp78);
    D_800E7313 = 0xA;
    func_80048F64(0x60);
    D_80120651 = 4;
    D_80120652 = 0x40;
    D_80120688 = 0x40000000;
    D_80120653 = 0x84;
    D_8012065C = sprite_frame_table;
    D_80120660 = D_800C9A14;
    D_80120684 = D_800C9730;
    D_80120654 = 8;
    D_80120655 = 1;
    D_80120668 = 0x40;
    D_80120676 = -0x10;
    D_8012067A = -0x13;
}
