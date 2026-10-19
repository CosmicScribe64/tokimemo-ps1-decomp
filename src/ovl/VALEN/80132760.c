#include "ovl/VALEN.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/VALEN/data/VALEN/80132760.data", D_80134520);
INCLUDE_RODATA("asm/ovl/VALEN/data/VALEN/80132760.data", D_80134524);
INCLUDE_RODATA("asm/ovl/VALEN/data/VALEN/80132760.data", D_8013453C);
u8 D_80134540[4] = { 0 };
INCLUDE_RODATA("asm/ovl/VALEN/data/VALEN/80132760.data", D_80134544);
INCLUDE_RODATA("asm/ovl/VALEN/data/VALEN/80132760.data", D_80134548);
INCLUDE_RODATA("asm/ovl/VALEN/data/VALEN/80132760.data", D_8013454C);

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

#ifdef NON_MATCHING
/* NON_MATCHING: score=749 (304/310 insns). .data island D_80134540[4] resolves the 0xFF/sp50/D_80134540 shift chain; IDO 5.3 uopt treats (unk_06 + unk_02) and (unk_02 + unk_06) as commutative in isearchloop and CSEs all 6 into $v0, eliminating 4 loads from &D_800E6280 + i * 56 and swapping $s0/$s1. T-24101. */
void func_80132834(void) {
    s16 i;
    s16 j;
    u8 sp50[4];

    for (i = 0; i < 11; i++) {
        D_80134524[i] = 0xFF;
    }
    D_8013453C = 0;
    D_80134520 = 0;
    for (i = 1; i < 11; i++) {
        if ((i != 9 &&
             !(D_800E6280.unk_1BC[i].unk_0C.w & 0x20000000) &&
             (D_800E6280.unk_1BC[i].unk_0C.w & 0x04000000) &&
             get_h_tokimeki(i) >= 0x1EU &&
             ((u32)(D_800E6280.unk_1BC[i].unk_0C.w << 4) >> 28) <= 2U) ||
            (i == 9 &&
             (D_800E6280.unk_1BC[9].unk_0C.w & 0x02000000) &&
             (D_800E6280.unk_1BC[9].unk_0C.w & 0x04000000) &&
             get_h_tokimeki(9) >= 0x46U &&
             ((u32)(D_800E6280.unk_1BC[9].unk_0C.w << 4) >> 28) <= 2U)) {
            D_80134524[D_8013453C] = i;
            D_8013453C += 1;
        }
    }
    func_8013387C();
    if (D_800E6280.unk_1BC[0].unk_02 >= 0x1E &&
        !(D_800E6280.unk_1BC[0].unk_0C.w & 0x20000000) &&
        (D_800E6280.unk_1BC[0].unk_0C.w & 0x04000000) &&
        ((u32)(D_800E6280.unk_1BC[0].unk_0C.w << 4) >> 28) <= 2U) {
        D_80134524[D_8013453C] = 0;
        D_8013453C += 1;
    }
    D_80134540[0] = D_80134540[1] = D_80134540[2] = 0xFF;
    sp50[0] = sp50[1] = sp50[2] = 0;
    for (i = 0; i < 11; i++) {
        if ((i != 9 &&
             !(D_800E6280.unk_1BC[i].unk_0C.w & 0x20000000) &&
             (D_800E6280.unk_1BC[i].unk_0C.w & 0x04000000)) ||
            (i == 9 &&
             (D_800E6280.unk_1BC[i].unk_0C.w & 0x02000000) &&
             (D_800E6280.unk_1BC[i].unk_0C.w & 0x04000000))) {
            if (D_800E6280.unk_1BC[i].unk_06 + D_800E6280.unk_1BC[i].unk_02 >= sp50[0]) {
                sp50[2] = sp50[1];
                sp50[1] = sp50[0];
                sp50[0] = D_800E6280.unk_1BC[i].unk_02 + D_800E6280.unk_1BC[i].unk_06;
                D_80134540[2] = D_80134540[1];
                D_80134540[1] = D_80134540[0];
                D_80134540[0] = i;
            } else if (D_800E6280.unk_1BC[i].unk_06 + D_800E6280.unk_1BC[i].unk_02 >= sp50[1]) {
                sp50[2] = sp50[1];
                sp50[1] = D_800E6280.unk_1BC[i].unk_02 + D_800E6280.unk_1BC[i].unk_06;
                D_80134540[2] = D_80134540[1];
                D_80134540[1] = i;
            } else if (D_800E6280.unk_1BC[i].unk_06 + D_800E6280.unk_1BC[i].unk_02 >= sp50[2]) {
                sp50[2] = D_800E6280.unk_1BC[i].unk_06 + D_800E6280.unk_1BC[i].unk_02;
                D_80134540[2] = i;
            }
        }
    }
    D_80134544 = 0;
    j = 0;
    for (i = 0; i < 11; i++) {
        if ((i != 0 && get_h_tokimeki(i) >= 0x46U && D_80134524[j] == i) ||
            (i == 0 && D_800E6280.unk_1BC[0].unk_02 >= 0x46 && D_80134524[j] == 0)) {
            D_80134544 += 1;
        }
        if (D_80134524[j] == i) {
            j++;
        }
    }
    func_8004284C();
}
#else
INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN/80132760", func_80132834);
#endif

#ifdef NON_MATCHING
/* NON_MATCHING: T-24033, 7 resolved rows. Dispatch shift is late in a branch delay slot; original shifts earlier and keeps one extra nop, changing two branch offsets. */
s32 func_80132D0C(void) {
    s32 mode;
    GameState *state;

    state = &D_800E6280;

    if (D_80134520 == D_8013453C) {
        func_80042808();
        return 0;
    }
    state->unk_F5F = D_80134524[D_80134520];
    func_800847B8(state->unk_F5F);
    D_80134498 = get_h_tokimeki(state->unk_F5F) >= 0x46U;
    if (state->unk_F5F == 0) {
        D_80134498 = state->unk_1BC[0].unk_02 >= 0x46;
    }
    mode = D_80134544;
    if ((state->unk_F5F == 9) && (mode == 1)) {
        D_80134498 = 3;
    }
    if ((state->unk_F5F == 0) && (mode == 1) && (state->unk_1BC[0].unk_02 >= 0x46)) {
        D_80134498 = 3;
    }
    if ((state->unk_F5F == 0) && (mode == 1) && (state->unk_1BC[0].unk_02 < 0x46)) {
        D_80134498 = 5;
    }
    switch (state->unk_F5F) {
    case 0:
        D_8013448C = D_801343F8;
        D_80134490 = D_8013442C;
        D_80134494 = D_80134460;
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
        D_80134490 = D_80134454;
        D_80134494 = D_80134488;
        break;
    }
    func_8004284C();
}

#else
INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN/80132760", func_80132D0C);
#endif

#ifdef NON_MATCHING
/* NON_MATCHING: score=171 (278/277 insns). D_80134548 promotes to $v0 across the % 8, % 2, and switch blocks; remaining diffs are CVTL 8 on the initial % 8 assignment and uopt hoisting D_800E6280.unk_F5F into $a0 at .L80133208. T-24101. */
void func_80133088(void) {
    D_80134548 = (D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 +
                  D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 +
                  (D_800E6280.unk_10F8 & 0xFFFF)) % 8;
    if ((get_h_tokimeki(D_800E6280.unk_F5F) < 0x46U && D_800E6280.unk_F5F != 0) ||
        (D_800E6280.unk_1BC[0].unk_02 < 0x46 && D_800E6280.unk_F5F == 0)) {
        if ((D_80134548 % 2) != 0) {
            D_80134548 = (D_80134548 - 1) & 0xFF;
        }
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 3;
    } else {
        if ((D_80134548 % 2) == 0) {
            D_80134548 = (D_80134548 + 1) & 0xFF;
        }
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 5;
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 += 5;
    }
    switch (D_80134548) {
    case 0:
        D_800E6280.unk_11C.unk_02 += 2;
        break;
    case 1:
        D_800E6280.unk_11C.unk_02 -= 3;
        D_800E6280.unk_11C.unk_04 += 0xA;
        D_800E6280.unk_11C.unk_06 += 0xA;
        D_800E6280.unk_11C.unk_08 += 0xA;
        D_800E6280.unk_11C.unk_0A += 0xA;
        D_800E6280.unk_11C.unk_0E += 0xA;
        break;
    case 2:
        D_800E6280.unk_11C.unk_04 += 5;
        break;
    case 3:
        D_800E6280.unk_11C.unk_02 -= 5;
        D_800E6280.unk_11C.unk_06 += 0xA;
        break;
    case 4:
        D_800E6280.unk_11C.unk_02 += 2;
        D_800E6280.unk_11C.unk_0A += 3;
        break;
    case 5:
        D_800E6280.unk_11C.unk_08 += 0xA;
        break;
    case 6:
        D_800E6280.unk_11C.unk_0E += 5;
        break;
    case 7:
        D_800E6280.unk_11C.unk_04 += 5;
        D_800E6280.unk_11C.unk_0E += 5;
        break;
    }
    if (D_800E6280.unk_F5F == 9 && D_80134544 == 1) {
        D_80134548 = 9;
    }
    if (D_800E6280.unk_F5F == 0 && D_80134544 == 1 && D_800E6280.unk_1BC[0].unk_02 >= 0x46) {
        D_80134548 = 8;
    }
    if (D_800E6280.unk_F5F == 0 && D_80134544 == 1 && D_800E6280.unk_1BC[0].unk_02 < 0x46) {
        D_80134548 = 0;
    }
    check_para_limit();
    if (D_80134548 < 8) {
        func_8007ED84(D_80134548 + 0x4072);
    } else if (D_80134548 == 8) {
        func_8007ED84(0x41DB);
    } else {
        func_8007ED84(0x41DA);
    }
    func_8004284C();
}
#else
INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN/80132760", func_80133088);
#endif

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

s32 func_80133670(void) {
    GameState *g; /* the selected girl is read through a state pointer (lui v0 base for the call argument), T-19016 */

    g = &D_800E6280;
    if ((D_800E6280.unk_040 == ((u32)(D_800E6280.unk_0F8 << 0x17) >> 0x1B)) && (D_800E6280.unk_03F == (D_800E6280.unk_0F8 & 0xF)) && (get_h_tokimeki(g->unk_F5F) >= 0x46U) && ((D_80134540[0] == g->unk_F5F) || (D_80134540[1] == g->unk_F5F) || (D_80134540[2] == g->unk_F5F)) && (g->unk_F5F == 9)) {
        func_8004284C();
        return 0;
    }
    return normal_date_girl_out();
}

/* s32: the first exit returns 0 (or v0,zero,zero in its branch delay slot), the second is a bare return and the last call falls off the end. T-19016 */
s32 func_80133728(void) {
    GameState *g; /* the selected girl is read through a state pointer (lui v0 base for the call argument), T-19016 */

    g = &D_800E6280;
    if ((D_800E6280.unk_040 == ((u32)(D_800E6280.unk_0F8 << 0x17) >> 0x1B)) && (D_800E6280.unk_03F == (D_800E6280.unk_0F8 & 0xF)) && (((get_h_tokimeki(g->unk_F5F) >= 0x46U) && (g->unk_F5F != 0)) || ((D_800E6280.unk_1BC[0].unk_02 >= 0x46) && (g->unk_F5F == 0))) && ((D_80134540[0] == g->unk_F5F) || (D_80134540[1] == g->unk_F5F) || (D_80134540[2] == g->unk_F5F))) {
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        func_80042908(3);
        return 0;
    }
    if (((get_h_tokimeki(g->unk_F5F) >= 0x46U) && (g->unk_F5F != 0)) || ((D_800E6280.unk_1BC[0].unk_02 >= 0x46) && (g->unk_F5F == 0))) {
        func_80132348();
        return;
    }
    func_8004284C();
}

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
