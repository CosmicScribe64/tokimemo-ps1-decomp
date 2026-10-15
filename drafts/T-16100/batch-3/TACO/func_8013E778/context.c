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

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E6298;
extern s32 D_80129C40;
s32 func_800A1070();
/* end bulk_m2c declarations */
void func_8013E778(s32 arg0) {
    SVECTOR stack88;
    s32 sp98;
    s32 sp92;
    s32 sp9E;
    s32 sp96;
    s32 sp9C;
    s32 sp9A;
    s32 sp94;
    s32 spA8[8];
    s16 spA4;
    s16 spA2;
    s16 spA0;
    s16 sp90;
    s32 sp7C[1];
    s32 sp78[1];
    s32 sp70;
    s32 sp6C;
    Tc14 *temp_v0;
    s16 *var_s1;
    s16 temp_v0_2;
    s32 temp_t1;
    s32 temp_v0_3;
    s32 var_fp;
    s32 var_s0;
    s32 var_s1_2;
    s32 var_s7;
    s8 temp_a0;
    s8 temp_a1;
    u8 *var_s2;

    (*(s32 *)&spA8) = (s32) D_80129C40;
    *(s32 *)((unsigned char *)(&spA8) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (4));
    *(s32 *)((unsigned char *)(&spA8) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (8));
    *(s32 *)((unsigned char *)(&spA8) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (12));
    *(s32 *)((unsigned char *)(&spA8) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (16));
    *(s32 *)((unsigned char *)(&spA8) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (20));
    temp_v0 = &(&D_8015EC70)[arg0];
    *(s32 *)((unsigned char *)(&spA8) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (24));
    *(s32 *)((unsigned char *)(&spA8) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (28));
    stack88.vx = (s32) temp_v0->unk0;
    *(s32 *)((unsigned char *)(&stack88.vx) + (4)) = (s32) *(s32 *)((unsigned char *)temp_v0 + (4));
    stack88.vz -= 0x200;
    sp6C = ((u8) temp_v0->unk12 >> 2) & 7;
    sp70 = (s32) temp_v0->padE[3];
    func_800A1100((s32) &spA8);
    func_800A1070(&spA8);
    var_fp = 0;
    var_s7 = 0;
    spA4 = stack88.vz;
    do {
        var_s1 = &sp90;
        spA2 = (stack88.vy + var_fp) - 0x40;
        var_s2 = var_s7 + var_s1;
        var_s0 = 0;
loop_2:
        spA0 = (stack88.vx + var_s0) - 0x40;
        func_800AB170((s32) &spA0, (s32) var_s2, (s32) &sp7C, (s32) &sp78);
        var_s1 += 8;
        var_s0 += 0x80;
        var_s2 += 8;
        if (var_s1 != &spA0) {
            goto loop_2;
        }
        var_s7 += 4;
        var_fp += 0x80;
    } while (var_s7 != 8);
    if ((sp98 >= -0x2C0) && (sp90 < 0x2C1)) {
        temp_v0_2 = *(&D_800E6298 + (D_8011ECA0 * 2));
        if (sp96 >= (temp_v0_2 - 0x200)) {
            if ((temp_v0_2 + 0x200) >= sp9E) {
                var_s1_2 = (s32) (D_80122754 - stack88.vz) / 127;
                if (var_s1_2 >= 0x100) {
                    var_s1_2 = 0xFF;
                } else if (var_s1_2 <= 0) {
                    var_s1_2 = 1;
                }
                temp_v0_3 = GetWorkBase(0x28, D_8011ECA0);
                func_8009F040(temp_v0_3);
                SetSemiTrans(temp_v0_3, 1);
                *(s8 *)((unsigned char *)temp_v0_3 + (4)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_3 + (5)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_3 + (6)) = 0x80;
                *(s16 *)((unsigned char *)temp_v0_3 + (22)) = func_8009ECB0(1, 1, 0x2C0, 0x100);
                *(s16 *)((unsigned char *)temp_v0_3 + (14)) = func_8009ED74(0, sp70 + 0x1E2);
                *(s8 *)((unsigned char *)temp_v0_3 + (12)) = 0x61;
                temp_t1 = sp6C << 5;
                temp_a0 = temp_t1 + 1;
                temp_a1 = temp_t1 + 0x1F;
                *(s8 *)((unsigned char *)temp_v0_3 + (29)) = temp_a1;
                *(s8 *)((unsigned char *)temp_v0_3 + (37)) = temp_a1;
                *(s8 *)((unsigned char *)temp_v0_3 + (13)) = temp_a0;
                *(s8 *)((unsigned char *)temp_v0_3 + (21)) = temp_a0;
                *(s8 *)((unsigned char *)temp_v0_3 + (20)) = 0x7F;
                *(s8 *)((unsigned char *)temp_v0_3 + (36)) = 0x7F;
                *(s8 *)((unsigned char *)temp_v0_3 + (28)) = 0x61;
                *(s16 *)((unsigned char *)temp_v0_3 + (8)) = sp90;
                *(s16 *)((unsigned char *)temp_v0_3 + (10)) = sp92;
                *(s16 *)((unsigned char *)temp_v0_3 + (16)) = sp94;
                *(s16 *)((unsigned char *)temp_v0_3 + (18)) = sp96;
                *(s16 *)((unsigned char *)temp_v0_3 + (24)) = sp98;
                *(s16 *)((unsigned char *)temp_v0_3 + (26)) = sp9A;
                *(s16 *)((unsigned char *)temp_v0_3 + (32)) = sp9C;
                *(s16 *)((unsigned char *)temp_v0_3 + (34)) = sp9E;
                AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (var_s1_2 * 4)], temp_v0_3, 0x61, 0x7F);
            }
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013EAA4);
