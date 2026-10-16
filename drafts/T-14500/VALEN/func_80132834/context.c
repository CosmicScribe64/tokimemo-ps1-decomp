#include "ovl/VALEN.h"

typedef struct {
    void (*f[56])();
} FnTbl56; /* size 0xE0 */
extern FnTbl56 D_8013454C;

void func_80132760(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl56 tbl;

    tbl = D_8013454C;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_801327E8(void) {
    func_800847B8(9);
    func_80132348();
}

void func_80132810(void) {
    D_800E6280.unk_F5F = 0;
    func_8004284C();
}

/* bulk_m2c declarations (T-11190) */
extern s16 D_800E643E;
extern s16 D_800E6636;
extern s16 D_800E663A;
extern s32 D_800E6640;
extern s8 D_80134548;
extern u32 D_800E7378;
void func_8013387C(void);
/* end bulk_m2c declarations */
void func_80132834(void) {
    s32 sp5C[16];
    s16 sp54;
    s16 sp52;
    s16 sp50;
    s16 *temp_a0;
    s16 *temp_t6;
    s16 *temp_v0_3;
    s16 *temp_v0_4;
    s16 temp_s1;
    s16 temp_t2;
    s16 temp_t4_2;
    s16 temp_t8;
    s16 temp_v0_2;
    s16 temp_v1;
    s16 var_s1_2;
    s16 var_s1_3;
    s16 var_s1_4;
    s16 var_s2;
    s32 temp_hi;
    s32 temp_t4;
    s32 var_t0;
    u8 temp_a2;
    u8 temp_t7;
    u8 temp_v0;
    u8 var_s1;

    D_80134520 = 0;
    D_80134548 = 0;
    D_8013453C = 0;
    D_80134544 = 0;
    var_s2 = 0;
    if (!(D_800E6640 & 2)) {
        if (D_800E6636 >= 0x14) {
            D_80134544 = 1;
            D_800E663A = 0x32;
        }
    }
    temp_v0 = 0xFF & 0xFF;
    D_80134541 = temp_v0;
    *(u8 *)((unsigned char *)(&D_80134541) + (-1)) = temp_v0;
    sp50 = 0;
    *(u8 *)((unsigned char *)(&D_80134541) + (1)) = 0xFFU;
    var_s1 = 0;
    sp52 = 0;
    sp54 = 0;
    do {
        if ((D_800E6280.unk_1BC[var_s1].unk_0C.w & 2) && (get_h_tokimeki(var_s1) >= 0x46U)) {
            temp_v0_2 = D_800E6280.unk_1BC[var_s1].unk_02 + D_800E6280.unk_1BC[var_s1].unk_06;
            temp_t2 = sp50;
            if (sp50 < temp_v0_2) {
                temp_t7 = D_80134540;
                sp50 = D_800E6280.unk_1BC[var_s1].unk_06 + D_800E6280.unk_1BC[var_s1].unk_02;
                sp54 = sp52;
                D_80134540 = var_s1;
                sp52 = temp_t2;
                *(u8 *)((unsigned char *)(&D_80134540) + (2)) = (u8) D_80134541;
                *(u8 *)((unsigned char *)(&D_80134540) + (1)) = temp_t7;
            } else if (sp52 < temp_v0_2) {
                temp_a2 = D_80134541;
                temp_t8 = sp52;
                sp52 = D_800E6280.unk_1BC[var_s1].unk_06 + D_800E6280.unk_1BC[var_s1].unk_02;
                D_80134541 = var_s1;
                sp54 = temp_t8;
                *(u8 *)((unsigned char *)(&D_80134541) + (1)) = temp_a2;
            } else if (sp54 < temp_v0_2) {
                D_80134542 = var_s1;
                sp54 = temp_v0_2;
            }
        }
        var_s1 = (u8) (s16) (var_s1 + 1);
    } while ((s32) var_s1 < 0xB);
    var_s1_2 = 0;
    do {
        if (((((u32) (D_800E6280.unk_1BC[var_s1_2].unk_0C.w << 0x1E) >> 0x1F) != 0) || ((var_s1_2 == 9) && (D_80134544 == 1))) && ((get_h_yuukou(var_s1_2, (s16 (*)[]) &sp5C[0], 9) >= 0x32U) || ((D_80134544 == 1) && (var_s1_2 == 9)))) {
            (&sp5C[0])[var_s2] = var_s1_2;
            D_8013453C += 1;
            var_s2 += 1;
            D_800E6280.unk_1BC[var_s1_2].unk_0C.b[1] = (D_800E6280.unk_1BC[var_s1_2].unk_0C.b[1] & 0xFFFE) | 1;
        }
        temp_s1 = var_s1_2 + 1;
        var_s1_2 = temp_s1;
    } while (temp_s1 < 0xB);
    temp_t4 = D_8013453C > 0;
    if (temp_t4 != 0) {
        var_s1_3 = 0;
        if (temp_t4 != 0) {
            do {
                temp_hi = (s32) (var_s1_3 + (s16) ((u32) D_800E7378 % (u32) D_8013453C)) % (s16) D_8013453C;
                temp_t6 = &D_80134524[var_s1_3];
                temp_t4_2 = var_s1_3 + 1;
                var_s1_3 = temp_t4_2;
                *temp_t6 = (&sp5C[0])[temp_hi];
            } while (temp_t4_2 < D_8013453C);
        }
        func_8013387C();
    }
    var_s1_4 = 0;
    if (D_8013453C > 0) {
        var_t0 = 0 * 2;
        do {
            temp_a0 = D_80134524 + var_t0;
            temp_v1 = *temp_a0;
            if ((temp_v1 == 0) && (D_800E643E >= 0x46)) {
                temp_v0_3 = &D_80134524[D_8013453C];
                *temp_a0 = *(s16 *)((unsigned char *)temp_v0_3 + (-2));
                *(s16 *)((unsigned char *)temp_v0_3 + (-2)) = 0;
            } else if ((temp_v1 == 0) && (D_80134544 != 0)) {
                temp_v0_4 = &D_80134524[D_8013453C];
                *temp_a0 = *(s16 *)((unsigned char *)temp_v0_4 + (-2));
                *(s16 *)((unsigned char *)temp_v0_4 + (-2)) = 0;
            } else if (temp_v1 == 0) {
                *temp_a0 = *D_80134524;
                *D_80134524 = 0;
            }
            if ((*temp_a0 == 9) && (D_80134544 != 0)) {
                *temp_a0 = *D_80134524;
                *D_80134524 = 9;
            }
            var_s1_4 += 1;
            var_t0 = var_s1_4 * 2;
        } while (var_s1_4 < D_8013453C);
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN/80132760", func_80132D0C);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN/80132760", func_80133088);

void func_801334DC(void) {
    D_80134520 += 1;
    func_80042940(1);
}

void func_80133510(void) {
    if (strcmp(D_800CA19C, D_800CA1DC) == 0) {
        func_8004284C();
    }
    strcpy(D_800CA19C, D_800CA1DC);
    func_8004284C();
}

void func_80133568(void) {
    if ((D_80134544 == 1) && (D_800E6280.unk_F5F == 9)) {
        D_800E6280.unk_110A += 6;
        return;
    }
    func_8004284C();
}

void func_801335C0(void) {
    func_800AE0F0(D_800CA1DC, "校舎裏");
    func_800AE0F0(D_800CA19C, "　");
    func_8007ED84(0x407A);
    func_8004284C();
}

void func_80133610(void) {
    func_800847B8(0);
    func_80062CD0(0x544A);
    func_8004284C();
}

void func_80133640(void) {
    func_800847B8(9);
    func_80062CD0(0x64D0);
    func_8004284C();
}

#ifdef NON_MATCHING
/* NON_MATCHING: T-10520, only the base register of the argument load differs ($a0 here, $v0 in the original; ugen temporary), 2 lines */
s32 func_80133670(void) {
    if ((D_800E6280.unk_040 == ((u32)(D_800E6280.unk_0F8 << 0x17) >> 0x1B)) && (D_800E6280.unk_03F == (D_800E6280.unk_0F8 & 0xF)) && (get_h_tokimeki(D_800E6280.unk_F5F) >= 0x46U) && ((D_80134540 == D_800E6280.unk_F5F) || (D_80134541 == D_800E6280.unk_F5F) || (D_80134542 == D_800E6280.unk_F5F)) && (D_800E6280.unk_F5F == 9)) {
        func_8004284C();
        return 0;
    }
    return normal_date_girl_out();
}
#else
INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN/80132760", func_80133670);
#endif

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN/80132760", func_80133728);

void func_8013387C(void) {
    s16 n;
    s16 i;
    s16 j;
    s16 k;
    s16 t;

    func_800AE120(D_800E6280.unk_10F8);
    n = func_800AE0D0() % 256 + 10;
    for (i = 0; i < n; i++) {
        j = func_800AE0D0() % D_8013453C;
        k = func_800AE0D0() % D_8013453C;
        t = D_80134524[j];
        D_80134524[j] = D_80134524[k];
        D_80134524[k] = t;
    }
}
