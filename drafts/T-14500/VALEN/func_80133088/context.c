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

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN/80132760", func_80132834);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN/80132760", func_80132D0C);

/* bulk_m2c declarations (T-11190) */
extern s16 D_800E643E;
extern s32 D_800E7378;
extern s32 D_80122D48;
extern s32 D_80134526;
extern s8 D_80134548;
extern u8 D_800E71DE;
extern u8 D_800E71DF;
/* end bulk_m2c declarations */
void func_80133088(void) {
    s32 sp28;
    s16 temp_v0_2;
    s32 var_a2;
    s32 var_a2_2;
    s8 var_v0;
    u32 temp_v0;

    temp_v0 = D_800E7378 & 7;
    if (temp_v0 == 0) {
        var_v0 = 3;
    } else {
        var_v0 = 1;
        if (temp_v0 < 4U) {
            var_v0 = 0;
        }
    }
    if (D_800E71DF == 0) {
        var_v0 = D_800E7378 & 1;
    }
    if (var_v0 != 0) {
        if (var_v0 != 1) {
            D_80134548 = var_v0;
            if (var_v0 != 2) {
                func_800AE0F0(D_800CA1DC, "屋上");
                var_a2 = 0x4084;
            } else {
                func_800AE0F0(D_800CA1DC, "校舎裏");
                var_a2 = 0x407A;
            }
        } else {
            D_80134548 = var_v0;
            func_800AE0F0(D_800CA1DC, "教室");
            var_a2 = 0x4055;
        }
    } else {
        D_80134548 = var_v0;
        func_800AE0F0(D_800CA1DC, "廊下");
        var_a2 = 0x4045;
    }
    if (D_800E71DF != 0) {
        temp_v0_2 = D_80134520;
        if ((temp_v0_2 + 1) == D_8013453C) {
            func_800AE0F0(D_800CA1DC, "校門前");
            var_a2 = 0x4032;
        }
        if (((temp_v0_2 + 2) == D_8013453C) && (*(&D_80134526 + (temp_v0_2 * 2)) == 0)) {
            sp28 = 0x4032;
            func_800AE0F0(D_800CA1DC, "校門前");
            var_a2 = 0x4032;
        }
    }
    if (((D_80134520 + 1) == D_8013453C) && (D_800E71DF == 0) && ((D_800E643E >= 0x46) || (D_80134544 != 0))) {
        func_800AE0F0(D_800CA1DC, "自宅前");
        var_a2 = 0x414C;
    }
    if ((D_800E71DF == 9) && (D_80134544 == 1)) {
        sp28 = 0x4055;
        func_800AE0F0(D_800CA1DC, "教室");
        var_a2 = 0x4055;
    }
    if (D_800E71DF == 0) {
        if (D_800E643E >= 0x46) {
            D_80122D48 = 1;
        } else {
            D_80122D48 = 2;
        }
    } else {
        sp28 = var_a2;
        if (get_h_tokimeki(D_800E71DF) >= 0x46U) {
            D_80122D48 = 1;
        } else {
            D_80122D48 = 2;
        }
    }
    switch (var_a2) {
    case 0x4045:
        sp28 = var_a2;
        func_80085B3C(6, 0);
        var_a2_2 = var_a2;
        break;
    case 0x4055:
        sp28 = var_a2;
        func_80085B3C(6, 1);
        var_a2_2 = var_a2;
        break;
    case 0x414C:
        sp28 = var_a2;
        func_80085B3C(6, 5);
        var_a2_2 = var_a2;
        break;
    case 0x4032:
        sp28 = var_a2;
        func_80085B3C(6, 4);
        var_a2_2 = var_a2;
        break;
    case 0x407A:
        sp28 = var_a2;
        func_80085B3C(6, 3);
        var_a2_2 = var_a2;
        break;
    default:
        sp28 = var_a2;
        func_80085B3C(6, 2);
        var_a2_2 = var_a2;
        break;
    }
    if ((D_800E71DF == 9) && (D_80134544 == 1)) {
        D_800E71DE -= 1;
        D_80122D48 = 1;
        sp28 = var_a2_2;
        func_80085B3C(6, 3);
    }
    bg_read_sub2(var_a2_2);
    if (get_h_tokimeki(D_800E71DF) >= 0x46U) {
        func_80083440(0U);
    } else {
        func_80083440(1U);
    }
    if (D_800E71DF == 0) {
        if (D_800E643E >= 0x46) {
            func_80083440(0U);
        } else {
            func_80083440(1U);
        }
    }
    func_8004284C();
}

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
