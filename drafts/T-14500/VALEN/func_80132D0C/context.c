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

/* bulk_m2c declarations (T-11190) */
extern s16 D_800E643E;
extern u8 D_800E71DF;
extern s32 jtbl_8013437C;
/* end bulk_m2c declarations */
s32 func_80132D0C(void) {
    s32 temp_t3;
    s32 var_t6;
    u8 temp_a0;
    u8 temp_a1;

    if (D_80134520 == D_8013453C) {
        func_80042808();
        return 0;
    }
    temp_a0 = D_80134524[D_80134520];
    D_800E71DF = temp_a0;
    func_800847B8(temp_a0);
    temp_a1 = D_800E71DF;
    D_80134498 = get_h_tokimeki(D_800E71DF, &D_800E71DF) >= 0x46U;
    if (temp_a1 == 0) {
        D_80134498 = D_800E643E >= 0x46;
    }
    temp_t3 = temp_a1 * 4;
    if ((temp_a1 == 9) && (D_80134544 == 1)) {
        D_80134498 = 3;
    }
    if ((temp_a1 == 0) && (D_80134544 == 1) && (D_800E643E >= 0x46)) {
        D_80134498 = 3;
    }
    if ((temp_a1 == 0) && (D_80134544 == 1) && (D_800E643E < 0x46)) {
        D_80134498 = 5;
    }
    D_800E71DF = temp_a1;
    if (temp_a1 < 0xBU) {
        switch (*(&jtbl_8013437C + temp_t3)) {      /* unable to parse jump table */
        case 0:
            D_8013448C = D_801343F8;
            var_t6 = D_80134460;
            D_80134490 = D_8013442C;
block_28:
            D_80134494 = var_t6;
            break;
        case 1:
            D_8013448C = D_8013440C;
            D_80134490 = D_80134440;
            D_80134494 = D_80134474;
            break;
        case 2:
            D_8013448C = D_801343FC;
            D_80134490 = D_80134430;
            D_80134494 = D_80134464;
            break;
        case 3:
            D_8013448C = D_80134404;
            D_80134490 = D_80134438;
            D_80134494 = D_8013446C;
            break;
        case 4:
            D_8013448C = D_8013441C;
            D_80134490 = D_80134450;
            D_80134494 = D_80134484;
            break;
        case 5:
            D_8013448C = D_80134410;
            D_80134490 = D_80134444;
            D_80134494 = D_80134478;
            break;
        case 6:
            D_8013448C = D_80134414;
            D_80134490 = D_80134448;
            D_80134494 = D_8013447C;
            break;
        case 7:
            D_8013448C = D_80134408;
            D_80134490 = D_8013443C;
            D_80134494 = D_80134470;
            break;
        case 8:
            D_8013448C = D_801343F0;
            D_80134490 = D_80134424;
            D_80134494 = D_80134458;
            break;
        case 9:
            D_8013448C = D_80134418;
            D_80134490 = D_8013444C;
            D_80134494 = D_80134480;
            break;
        case 10:
            D_8013448C = D_80134420;
            var_t6 = D_80134488;
            D_80134490 = D_80134454;
            goto block_28;
        }
    }
    return func_8004284C();
}

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
