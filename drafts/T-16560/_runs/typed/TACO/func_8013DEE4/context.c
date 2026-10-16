#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013DB80);

void func_8013DDE4(void) {
    Tc14 *p;
    s32 i;

    /* FAKE: init and `do` share a source line (as1 schedules by line); the second base is spelled from the
     * end label so that IDO does not share the first loop's address. Same bytes as the original. T-10320 */
    p = D_8015EB30; do {
        p->unk12 = p->unk12 + 1;
        if (p->unk12 == 0xFF) {
            p->unk13 = 0;
        }
        if (p->unk4 >= 0x2711) {
            p->unk13 = 0;
        }
        if (p->unk13 & 0x80) {
            p->unk0 = p->unk0 + p->unk8;
            p->unk2 = p->unk2 + p->unkA;
            p->unk4 = p->unk4 + p->unkC;
        }
        p++;
    } while (p < D_8015EC70);
    p = (Tc14 *)((u8 *)D_8015EC70 - 0x140);
    i = 0;
    do {
        if (p->unk13 & 0x80) {
            func_8013DEE4(i);
        }
        i++;
        p++;
    } while (i != 0x10);
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E6298;
s32 func_800A1070();
/* end bulk_m2c declarations */
void func_8013DEE4(s32 arg0) {
    s32 spA0;
    s32 spA2;
    s32 spA6;
    s32 sp9E;
    s32 sp9C;
    s32 spA4;
    s32 sp9A;
    MATRIX spB0;
    s16 spAC;
    s16 spAA;
    s16 spA8;
    s16 sp98;
    s32 sp90;
    s32 sp8C[1];
    s32 sp88[1];
    u32 sp80;
    s16 sp70;
    s16 sp6C;
    u32 sp64;
    s32 sp60;
    s32 sp54;
    Tc14 *temp_v0;
    s16 temp_v0_2;
    s32 temp_fp;
    s32 temp_t3;
    s32 temp_v0_3;
    s32 var_s0;
    s32 var_s1;
    s32 var_s2_2;
    s32 var_s7;
    s32 var_v0_2;
    s32 var_v0_3;
    s8 var_v0;
    s8 var_v0_4;
    u8 *var_s2;

    temp_v0 = &D_8015EB30[arg0];
    sp6C = (s32) temp_v0->unk0;
    *(s32 *)((unsigned char *)(&sp6C) + (4)) = (s32) *(s32 *)((unsigned char *)temp_v0 + (4));
    sp80 = temp_v0->unk11 & 0xF;
    spB0 = D_80129C40;
    sp70 -= 0x200;
    func_800A1100((s32) &spB0);
    func_800A1070(&spB0);
    switch (sp80) {
    case 3:
    case 5:
        var_s7 = 0x100;
        break;
    case 0:
        var_s7 = 0x40;
        break;
    default:
    case 1:
    case 2:
    case 4:
        var_s7 = 0x80;
        break;
    }
    sp60 = 0;
    sp90 = 0;
    sp54 = 0;
    sp64 = sp80;
    spAC = sp70;
    temp_fp = var_s7 / 2;
    do {
        spAA = (sp6E + sp60) - temp_fp;
        var_s0 = 0;
        var_s1 = 0;
        var_s2 = sp54 + &sp98;
loop_7:
        spA8 = (sp6C + var_s1) - temp_fp;
        func_800AB170((s32) &spA8, (s32) var_s2, (s32) &sp8C, (s32) &sp88);
        var_s0 += 1;
        var_s1 += var_s7;
        var_s2 += 8;
        if (var_s0 != 2) {
            goto loop_7;
        }
        temp_t3 = sp90 + 1;
        sp54 += 4;
        sp60 += var_s7;
        sp90 = temp_t3;
    } while (temp_t3 != 2);
    if ((spA0 >= -0x2C0) && (sp98 < 0x2C1)) {
        temp_v0_2 = *(&D_800E6298 + (D_8011ECA0 * 2));
        if (sp9E >= (temp_v0_2 - 0x200)) {
            if ((temp_v0_2 + 0x200) >= spA6) {
                var_s2_2 = (s32) (D_80122754 - sp70) / 127;
                if (var_s2_2 >= 0x100) {
                    var_s2_2 = 0xFF;
                    goto block_17;
                }
                if (var_s2_2 <= 0) {

                } else {
block_17:
                    temp_v0_3 = GetWorkBase(0x28, D_8011ECA0);
                    func_8009F040(temp_v0_3);
                    if (sp80 != 5) {
                        var_v0 = 0xFF;
                        if (sp80 != 6) {
                            var_v0 = 0x80;
                        }
                    } else {
                        var_v0 = 0xFF;
                    }
                    *(s8 *)((unsigned char *)temp_v0_3 + (4)) = var_v0;
                    *(s8 *)((unsigned char *)temp_v0_3 + (5)) = var_v0;
                    *(s8 *)((unsigned char *)temp_v0_3 + (6)) = var_v0;
                    switch (sp64) {
                    case 3:
                    case 5:
                    case 6:
                        SetSemiTrans(temp_v0_3, 1);
                        break;
                    default:
                    case 0:
                    case 1:
                    case 2:
                    case 4:
                        SetSemiTrans(temp_v0_3, 0);
                        break;
                    }
                    if (sp80 != 4) {
                        var_v0_2 = func_8009ECB0(1, 1, 0x3C0, 0x100);
                    } else {
                        var_v0_2 = func_8009ECB0(0, 1, 0x380, 0x100);
                    }
                    *(s16 *)((unsigned char *)temp_v0_3 + (22)) = (s16) var_v0_2;
                    if (sp80 != 4) {
                        if (sp80 == 5) {
                            var_v0_3 = func_8009ED74(0, 0x1EA);
                        } else {
                            var_v0_3 = func_8009ED74(0x100, 0x1F8);
                        }
                    } else {
                        var_v0_3 = func_8009ED74(0, 0x1E5);
                    }
                    *(s16 *)((unsigned char *)temp_v0_3 + (14)) = (s16) var_v0_3;
                    switch (sp64) {
                    default:
                    case 0:
                    case 1:
                        var_v0_4 = 1;
                        *(s8 *)((unsigned char *)temp_v0_3 + (29)) = 0x5F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (37)) = 0x5F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (13)) = 0x41;
                        *(s8 *)((unsigned char *)temp_v0_3 + (21)) = 0x41;
                        *(s8 *)((unsigned char *)temp_v0_3 + (20)) = 0x1F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (36)) = 0x1F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (12)) = 1;
block_39:
                        *(s8 *)((unsigned char *)temp_v0_3 + (28)) = var_v0_4;
                        break;
                    case 2:
                        *(s8 *)((unsigned char *)temp_v0_3 + (20)) = 0x7F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (36)) = 0x7F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (12)) = 0x41;
                        *(s8 *)((unsigned char *)temp_v0_3 + (28)) = 0x41;
                        *(s8 *)((unsigned char *)temp_v0_3 + (29)) = 0x3F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (37)) = 0x3F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (13)) = 1;
                        *(s8 *)((unsigned char *)temp_v0_3 + (21)) = 1;
                        break;
                    case 3:
                        var_v0_4 = 1;
                        *(s8 *)((unsigned char *)temp_v0_3 + (20)) = 0x3F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (29)) = 0x3F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (36)) = 0x3F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (37)) = 0x3F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (12)) = 1;
                        *(s8 *)((unsigned char *)temp_v0_3 + (13)) = 1;
                        *(s8 *)((unsigned char *)temp_v0_3 + (21)) = 1;
                        goto block_39;
                    case 4:
                        var_v0_4 = 0x40;
                        *(s8 *)((unsigned char *)temp_v0_3 + (20)) = 0x7F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (36)) = 0x7F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (29)) = 0xBF;
                        *(s8 *)((unsigned char *)temp_v0_3 + (37)) = 0xBF;
                        *(s8 *)((unsigned char *)temp_v0_3 + (13)) = 0x80;
                        *(s8 *)((unsigned char *)temp_v0_3 + (21)) = 0x80;
                        *(s8 *)((unsigned char *)temp_v0_3 + (12)) = 0x40;
                        goto block_39;
                    }
                    *(s16 *)((unsigned char *)temp_v0_3 + (8)) = spA0;
                    *(s16 *)((unsigned char *)temp_v0_3 + (10)) = spA2;
                    *(s16 *)((unsigned char *)temp_v0_3 + (16)) = sp98;
                    *(s16 *)((unsigned char *)temp_v0_3 + (18)) = sp9A;
                    *(s16 *)((unsigned char *)temp_v0_3 + (24)) = spA4;
                    *(s16 *)((unsigned char *)temp_v0_3 + (26)) = spA6;
                    *(s16 *)((unsigned char *)temp_v0_3 + (32)) = sp9C;
                    *(s16 *)((unsigned char *)temp_v0_3 + (34)) = sp9E;
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (var_s2_2 * 4)], temp_v0_3);
                }
            }
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013E424);

void func_8013E4FC(void) {
    Tc14 *p;
    s32 i;

    /* FAKE: the loop entry is written on one source line; as1 schedules by line and this orders the
     * end-pointer and base setup as the original does. T-10320 */
    p = D_8015EC70; do { p->unk12 += 1; if (p->unk12 == 0xFF) { p->unk12 = 0;
            p->unk13 += 1;
        }
        if (p->unk13 == 0x85) {
            p->unk13 = 0;
        }
        if (p->unk4 >= 0x2711) {
            p->unk13 = 0;
        }
        if (p->unk13 & 0x80) {
            p->unk0 += p->unk8;
            p->unk2 += p->unkA;
            p->unk4 += p->unkC;
            if (p->unk8 > 0) {
                p->unk8 -= 1;
            }
            if (p->unkA > 0) {
                p->unkA -= 1;
            }
            if (p->unk8 < 0) {
                p->unk8 += 1;
            }
            if (p->unkA < 0) {
                p->unkA += 1;
            }
            if (!(p->unk13 & 4)) {
                if (p->unk0 > 0x800) {
                    p->unk0 = 0x800;
                }
                if (p->unk0 < -0x800) {
                    p->unk0 = -0x800;
                }
                if (p->unk2 > 0x80) {
                    p->unk2 = 0x80;
                }
                if (p->unk2 < -0x700) {
                    p->unk2 = -0x700;
                }
                if (p->unk4 > D_8015EDB4->unk78 + 0x10) {
                    if (p->unkC > 0) {
                        p->unkC = p->unkC * -1;
                    }
                }
                if (p->unk4 < -0x1000) {
                    if (p->unkC < 0) {
                        p->unkC = p->unkC * -1;
                    }
                }
            }
        }
        p++;
    } while (p < (Tc14 *)&D_8015EDB0);
    /* FAKE: the base is spelled from the end label so that IDO does not share the first loop's address; same bytes. T-10320 */
    p = (Tc14 *)((u8 *)&D_8015EDB0 - 0x140);
    i = 0;
    do {
        if (p->unk13 & 0x80) {
            if (p->unk11 < 2) {
                func_8013E778(i);
            } else {
                func_8013EAA4(i);
            }
        }
        i++;
        p++;
    } while (i != 0x10);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013E778);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013EAA4);
