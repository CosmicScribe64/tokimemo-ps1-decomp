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
    } while (p < &D_8015EC70);
    p = (Tc14 *)((u8 *)&D_8015EC70 - 0x140);
    i = 0;
    do {
        if (p->unk13 & 0x80) {
            func_8013DEE4(i);
        }
        i++;
        p++;
    } while (i != 0x10);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013DEE4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013E424);

void func_8013E4FC(void) {
    Tc14 *p;
    s32 i;

    /* FAKE: the loop entry is written on one source line; as1 schedules by line and this orders the
     * end-pointer and base setup as the original does. T-10320 */
    p = &D_8015EC70; do { p->unk12 += 1; if (p->unk12 == 0xFF) { p->unk12 = 0;
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
            if (p->padE[3] < 2) {
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

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E6298;
extern s32 D_800E8CA0;
extern s32 D_8011ECA0;
extern s32 D_80122754;
extern s32 D_80129C40;
extern s32 D_8015EC70;
extern s32 D_8015F560;
extern s32 D_8015F568;
extern s32 D_8015F570;
extern s32 D_8015F578;
s16 func_8009ECB0();
s16 func_8009ED74();
s32 func_8009F040();
s32 func_800A0070();
s32 func_800A0140();
s32 func_800A1070();
s32 func_800A1100();
s32 func_800AB170();
/* end bulk_m2c declarations */
void func_8013EAA4(s32 arg0) {
    s32 spE8[8];
    s16 spC4;
    s16 spC2;
    s16 spC0;
    s16 spB0;
    s16 spAC;
    s16 spA8;
    s32 sp94[1];
    s32 sp90[1];
    s32 sp88;
    u32 sp84;
    s32 sp74;
    s32 sp6C;
    u8 *sp64;
    s32 sp60;
    s32 sp5C;
    s32 sp58;
    s32 sp54;
    s16 *var_s3;
    s16 temp_v0_2;
    s32 temp_a0_2;
    s32 temp_s5;
    s32 temp_t3;
    s32 temp_t4;
    s32 temp_t8;
    s32 temp_v0_4;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_3;
    s32 var_s1_4;
    s32 var_s2;
    s32 var_s4;
    s32 var_s5;
    s32 var_s5_2;
    u32 var_a2;
    u8 *temp_a0;
    u8 *temp_v1_2;
    u8 temp_v1;
    u8 *temp_v0;
    u8 *temp_v0_3;
    u8 *temp_v0_5;
    u8 *temp_v0_6;
    u8 *var_s0;
    u8 *var_s0_2;
    u8 *var_s0_3;

    (*(s32 *)&spE8) = (s32) D_80129C40;
    *(s32 *)((unsigned char *)(&spE8) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (4));
    *(s32 *)((unsigned char *)(&spE8) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (8));
    *(s32 *)((unsigned char *)(&spE8) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (12));
    *(s32 *)((unsigned char *)(&spE8) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (16));
    *(s32 *)((unsigned char *)(&spE8) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (20));
    temp_v0 = (arg0 * 0x14) + &D_8015EC70;
    *(s32 *)((unsigned char *)(&spE8) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (24));
    *(s32 *)((unsigned char *)(&spE8) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (28));
    spA8 = (s32) (*(s32 *)temp_v0);
    *(s32 *)((unsigned char *)(&spA8) + (4)) = (s32) *(s32 *)((unsigned char *)temp_v0 + (4));
    spAC -= 0x200;
    sp88 = *(u8 *)((unsigned char *)temp_v0 + (17)) & 7;
    sp84 = (u8) *(u8 *)((unsigned char *)temp_v0 + (18)) >> 1;
    sp64 = temp_v0;
    func_800A1100(&spE8);
    func_800A1070(&spE8);
    var_s5 = 0;
    sp54 = 0;
    spC4 = spAC;
    do {
        var_s3 = &spB0;
        spC2 = (spAA + var_s5) - 0x40;
        var_s1 = 0;
        var_s0 = sp54 + var_s3;
loop_2:
        spC0 = (spA8 + var_s1) - 0x40;
        func_800AB170(&spC0, var_s0, &sp94, &sp90);
        var_s3 += 8;
        var_s1 += 0x80;
        var_s0 += 8;
        if (var_s3 != &spC0) {
            goto loop_2;
        }
        var_s5 += 0x80;
        temp_t3 = sp54 + 4;
        sp54 = temp_t3;
    } while (temp_t3 < 8);
    if ((spB8 >= -0x2C0) && (spB0 < 0x2C1)) {
        temp_v0_2 = *(&D_800E6298 + (D_8011ECA0 * 2));
        if (spB6 >= (temp_v0_2 - 0x200)) {
            if ((temp_v0_2 + 0x200) >= spBE) {
                var_s1_2 = (s32) (D_80122754 - spAC) / 127;
                if (var_s1_2 >= 0x100) {
                    var_s1_2 = 0xFF;
                } else if (var_s1_2 <= 0) {
                    var_s1_2 = 1;
                }
                temp_v0_3 = GetWorkBase(0x28, D_8011ECA0);
                func_8009F040(temp_v0_3);
                SetSemiTrans(temp_v0_3, 0);
                *(s8 *)((unsigned char *)temp_v0_3 + (4)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_3 + (5)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_3 + (6)) = 0x80;
                *(s16 *)((unsigned char *)temp_v0_3 + (22)) = func_8009ECB0(0, 1, 0x380, 0x100);
                temp_v1 = *(&D_8015F560 + sp88);
                if (temp_v1 != 0) {
                    var_a2 = sp84 % temp_v1;
                } else {
                    var_a2 = 0;
                }
                *(s16 *)((unsigned char *)temp_v0_3 + (14)) = func_8009ED74(var_a2 * 0x10, *(&D_8015F568 + sp88) + 0x1E0, var_a2, sp88);
                temp_v1_2 = sp88 + &D_8015F570;
                *(s8 *)((unsigned char *)temp_v0_3 + (12)) = (s8) (*temp_v1_2 + 1);
                temp_a0 = sp88 + &D_8015F578;
                temp_v0_4 = var_s1_2 * 4;
                *(s8 *)((unsigned char *)temp_v0_3 + (13)) = (s8) (*temp_a0 + 1);
                *(s8 *)((unsigned char *)temp_v0_3 + (20)) = (s8) (*temp_v1_2 + 0xF);
                *(s8 *)((unsigned char *)temp_v0_3 + (21)) = (s8) (*temp_a0 + 1);
                *(s8 *)((unsigned char *)temp_v0_3 + (28)) = (s8) (*temp_v1_2 + 1);
                *(s8 *)((unsigned char *)temp_v0_3 + (29)) = (s8) (*temp_a0 + 0xF);
                *(s8 *)((unsigned char *)temp_v0_3 + (36)) = (s8) (*temp_v1_2 + 0xF);
                *(s8 *)((unsigned char *)temp_v0_3 + (37)) = (s8) (*temp_a0 + 0xF);
                *(s16 *)((unsigned char *)temp_v0_3 + (8)) = spB8;
                *(s16 *)((unsigned char *)temp_v0_3 + (10)) = spBA;
                *(s16 *)((unsigned char *)temp_v0_3 + (16)) = spB0;
                *(s16 *)((unsigned char *)temp_v0_3 + (18)) = spB2;
                *(s16 *)((unsigned char *)temp_v0_3 + (24)) = spBC;
                *(s16 *)((unsigned char *)temp_v0_3 + (26)) = spBE;
                *(s16 *)((unsigned char *)temp_v0_3 + (32)) = spB4;
                *(s16 *)((unsigned char *)temp_v0_3 + (34)) = spB6;
                sp60 = temp_v0_4;
                AddPrim((D_8011ECA0 << 0xA) + temp_v0_4 + &D_800E8CA0, temp_v0_3);
                temp_t8 = (s32) (func_800A0070(*(u8 *)((unsigned char *)sp64 + (18)) * 0x10) << 7) / 4096;
                sp6C = temp_t8 * 2;
                sp74 = temp_t8;
                var_s4 = 0;
                sp54 = 0;
                var_s5_2 = 0;
                spC4 = spAC;
                do {
                    spC2 = (spAA + var_s5_2) - sp74;
                    var_s1_3 = 0;
                    var_s0_2 = sp54 + &spB0;
loop_17:
                    spC0 = (spA8 + var_s1_3) - 0x80;
                    func_800AB170(&spC0, var_s0_2, &sp94, &sp90);
                    var_s1_3 += 0x100;
                    var_s0_2 += 8;
                    if (var_s1_3 != 0x200) {
                        goto loop_17;
                    }
                    var_s4 += 1;
                    sp54 += 4;
                    var_s5_2 += sp6C;
                } while (var_s4 < 2);
                temp_v0_5 = GetWorkBase(0x28, D_8011ECA0);
                func_8009F040(temp_v0_5);
                SetSemiTrans(temp_v0_5, 1);
                *(s8 *)((unsigned char *)temp_v0_5 + (4)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_5 + (5)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_5 + (6)) = 0x80;
                *(s16 *)((unsigned char *)temp_v0_5 + (22)) = func_8009ECB0(0, 1, 0x380, 0x100);
                temp_a0_2 = ((s32) sp84 % 14) * 0x10;
                sp5C = temp_a0_2;
                *(s16 *)((unsigned char *)temp_v0_5 + (14)) = func_8009ED74(temp_a0_2, 0x1E7);
                *(s8 *)((unsigned char *)temp_v0_5 + (29)) = 0x3F;
                *(s8 *)((unsigned char *)temp_v0_5 + (37)) = 0x3F;
                *(s8 *)((unsigned char *)temp_v0_5 + (20)) = 0x7F;
                *(s8 *)((unsigned char *)temp_v0_5 + (36)) = 0x7F;
                *(s8 *)((unsigned char *)temp_v0_5 + (13)) = 0x21;
                *(s8 *)((unsigned char *)temp_v0_5 + (21)) = 0x21;
                *(s8 *)((unsigned char *)temp_v0_5 + (12)) = 0x61;
                *(s8 *)((unsigned char *)temp_v0_5 + (28)) = 0x61;
                *(s16 *)((unsigned char *)temp_v0_5 + (8)) = spB8;
                *(s16 *)((unsigned char *)temp_v0_5 + (10)) = spBA;
                *(s16 *)((unsigned char *)temp_v0_5 + (16)) = spB0;
                *(s16 *)((unsigned char *)temp_v0_5 + (18)) = spB2;
                *(s16 *)((unsigned char *)temp_v0_5 + (24)) = spBC;
                *(s16 *)((unsigned char *)temp_v0_5 + (26)) = spBE;
                *(s16 *)((unsigned char *)temp_v0_5 + (32)) = spB4;
                *(s16 *)((unsigned char *)temp_v0_5 + (34)) = spB6;
                AddPrim((D_8011ECA0 << 0xA) + sp60 + &D_800E8CA0, temp_v0_5, 0x21, 0x7F);
                temp_s5 = (s32) (func_800A0140(*(u8 *)((unsigned char *)sp64 + (18)) * 0x10) << 7) / 4096;
                sp54 = 0;
                sp58 = 0;
                spC4 = spAC;
                do {
                    spC2 = (spAA + sp58) - 0x80;
                    var_s2 = 0;
                    var_s1_4 = 0;
                    var_s0_3 = sp54 + &spB0;
loop_21:
                    spC0 = (spA8 + var_s1_4) - temp_s5;
                    func_800AB170(&spC0, var_s0_3, &sp94, &sp90);
                    var_s2 += 1;
                    var_s0_3 += 8;
                    var_s1_4 += temp_s5 * 2;
                    if (var_s2 != 2) {
                        goto loop_21;
                    }
                    temp_t4 = sp58 + 0x100;
                    sp58 = temp_t4;
                    sp54 += 4;
                } while (temp_t4 != 0x200);
                temp_v0_6 = GetWorkBase(0x28, D_8011ECA0);
                func_8009F040(temp_v0_6);
                SetSemiTrans(temp_v0_6, 1);
                *(s8 *)((unsigned char *)temp_v0_6 + (4)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_6 + (5)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_6 + (6)) = 0x80;
                *(s16 *)((unsigned char *)temp_v0_6 + (22)) = func_8009ECB0(0, 1, 0x380, 0x100);
                *(s16 *)((unsigned char *)temp_v0_6 + (14)) = func_8009ED74(sp5C, 0x1E7);
                *(s8 *)((unsigned char *)temp_v0_6 + (29)) = 0x3F;
                *(s8 *)((unsigned char *)temp_v0_6 + (37)) = 0x3F;
                *(s8 *)((unsigned char *)temp_v0_6 + (20)) = 0x7F;
                *(s8 *)((unsigned char *)temp_v0_6 + (36)) = 0x7F;
                *(s8 *)((unsigned char *)temp_v0_6 + (13)) = 0x21;
                *(s8 *)((unsigned char *)temp_v0_6 + (21)) = 0x21;
                *(s8 *)((unsigned char *)temp_v0_6 + (12)) = 0x61;
                *(s8 *)((unsigned char *)temp_v0_6 + (28)) = 0x61;
                *(s16 *)((unsigned char *)temp_v0_6 + (8)) = spB8;
                *(s16 *)((unsigned char *)temp_v0_6 + (10)) = spBA;
                *(s16 *)((unsigned char *)temp_v0_6 + (16)) = spB0;
                *(s16 *)((unsigned char *)temp_v0_6 + (18)) = spB2;
                *(s16 *)((unsigned char *)temp_v0_6 + (24)) = spBC;
                *(s16 *)((unsigned char *)temp_v0_6 + (26)) = spBE;
                *(s16 *)((unsigned char *)temp_v0_6 + (32)) = spB4;
                *(s16 *)((unsigned char *)temp_v0_6 + (34)) = spB6;
                AddPrim((D_8011ECA0 << 0xA) + sp60 + &D_800E8CA0, temp_v0_6, 0x21, 0x7F);
            }
        }
    }
}
