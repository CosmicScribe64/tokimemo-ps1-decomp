#include "ovl/VALEN.h"

extern s32 D_80122D48; /* main bss; SHOUGATU, DATE and main declare it the same way */
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

/* Locals take frame slots top-down in declaration order, so this order puts candidates at sp+0x5C
 * and the scores at sp+0x50 (T-25117). Reads through `state` are pointer loads that uopt lets every
 * later store change: the ranking loop reloads both fields for the first two score assignments, and
 * `r` stays ahead of the rotation loop's guard instead of being copied into it. The direct
 * D_800E6280 reads stay available, so the player's affinity is hoisted out of the last loop. Each
 * sum is written in the order the original loads its two fields. */
void func_80132834(void) {
    s16 r;
    s16 candidates[13];
    GameState *state;
    s16 i;
    s16 rank_scores[3];
    s16 count;

    state = &D_800E6280;
    D_80134520 = 0;
    D_80134548 = 0;
    D_8013453C = 0;
    D_80134544 = 0;
    count = 0;
    if (D_800E6280.unk_1BC[9].unk_0C.f.b1 == 0 &&
        D_800E6280.unk_1BC[9].unk_02 >= 20) {
        D_80134544 = 1;
        D_800E6280.unk_1BC[9].unk_06 = 50;
    }
    D_80134540[0] = D_80134540[1] = D_80134540[2] = 0xFF;
    rank_scores[0] = rank_scores[1] = rank_scores[2] = 0;
    for (i = 0; i < 11; i++) {
        if (state->unk_1BC[i].unk_0C.f.b1 != 0 &&
            get_h_tokimeki(i) >= 0x46U) {
            if (rank_scores[0] < state->unk_1BC[i].unk_06 + state->unk_1BC[i].unk_02) {
                rank_scores[2] = rank_scores[1];
                rank_scores[1] = rank_scores[0];
                rank_scores[0] = state->unk_1BC[i].unk_02 + state->unk_1BC[i].unk_06;
                D_80134540[2] = D_80134540[1];
                D_80134540[1] = D_80134540[0];
                D_80134540[0] = i;
            } else if (rank_scores[1] < state->unk_1BC[i].unk_06 + state->unk_1BC[i].unk_02) {
                rank_scores[2] = rank_scores[1];
                rank_scores[1] = state->unk_1BC[i].unk_02 + state->unk_1BC[i].unk_06;
                D_80134540[2] = D_80134540[1];
                D_80134540[1] = i;
            } else if (rank_scores[2] < state->unk_1BC[i].unk_06 + state->unk_1BC[i].unk_02) {
                rank_scores[2] = state->unk_1BC[i].unk_06 + state->unk_1BC[i].unk_02;
                D_80134540[2] = i;
            }
        }
    }
    for (i = 0; i < 11; i++) {
        if ((D_800E6280.unk_1BC[i].unk_0C.f.b1 != 0 ||
             (i == 9 && D_80134544 == 1)) &&
            (get_h_yuukou(i) >= 0x32U || (D_80134544 == 1 && i == 9))) {
            candidates[count] = i;
            count += 1;
            D_8013453C += 1;
            D_800E6280.unk_1BC[i].unk_0C.f.b8 = 1;
        }
    }
    if (D_8013453C > 0) {
        r = (u32)state->unk_10F8 % (u32)D_8013453C;
        for (i = 0; i < D_8013453C; i++) {
            D_80134524[i] = candidates[(i + r) % D_8013453C];
        }
        func_8013387C();
    }
    for (i = 0; i < D_8013453C; i++) {
        if (D_80134524[i] == 0 && D_800E6280.unk_1BC[0].unk_02 >= 70) {
            D_80134524[i] = D_80134524[D_8013453C - 1];
            D_80134524[D_8013453C - 1] = 0;
        } else if (D_80134524[i] == 0 && D_80134544 != 0) {
            D_80134524[i] = D_80134524[D_8013453C - 1];
            D_80134524[D_8013453C - 1] = 0;
        } else if (D_80134524[i] == 0) {
            D_80134524[i] = D_80134524[0];
            D_80134524[0] = 0;
        }
        if (D_80134524[i] == 9 && D_80134544 != 0) {
            D_80134524[i] = D_80134524[0];
            D_80134524[0] = 9;
        }
    }
    func_8004284C();
}

/* The mode flag is read twice on purpose (T-24903): as1's global scheduler moves the
 * head `lui` of a dominated block into a branch delay slot, so the first read has to be
 * a volatile access (VALEN.h) to keep `lui v0` out of the `sel == 9` slot, and the second
 * read keeps uopt from loading the byte once instead (13 rows). In the original the
 * `sel == 9` slot holds the switch's dispatch `sll` (wiki/as1-scheduler-rule.md). */
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
    if (state->unk_F5F == 9) {
        mode = D_80134544;
        if (mode == 1) {
            D_80134498 = 3;
        }
    }
    mode = D_80134544;
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

void func_80133088(void) {
    GameState *state;
    s32 id;

    state = &D_800E6280;
    /* FAKE: dead entry store (every arm below rewrites the global); it gives the promoted
     * global $v0 and the random word $v1 (without it they take $v1 and $a0), as the entry
     * stores of the GYOZI twin func_801393C8 do (T-24130). T-25117 */
    D_80134548 = state->unk_10F8 & 7;
    if ((state->unk_10F8 & 7) == 0) {
        D_80134548 = 3;
    } else if ((state->unk_10F8 & 7) < 4U) {
        D_80134548 = 0;
    } else {
        D_80134548 = 1;
    }
    if (state->unk_F5F == 0) {
        D_80134548 = state->unk_10F8 & 1;
    }
    switch (D_80134548) {
    case 0:
        func_800AE0F0(D_800CA1DC, "廊下");
        id = 0x4045;
        break;
    case 1:
        func_800AE0F0(D_800CA1DC, "教室");
        id = 0x4055;
        break;
    case 2:
        func_800AE0F0(D_800CA1DC, "校舎裏");
        id = 0x407A;
        break;
    default:
        func_800AE0F0(D_800CA1DC, "屋上");
        id = 0x4084;
        break;
    }
    if (state->unk_F5F != 0) {
        if (D_80134520 + 1 == D_8013453C) {
            func_800AE0F0(D_800CA1DC, "校門前");
            id = 0x4032;
        }
        if ((D_80134520 + 2 == D_8013453C) && (D_80134524[D_80134520 + 1] == 0)) {
            func_800AE0F0(D_800CA1DC, "校門前");
            id = 0x4032;
        }
    }
    /* FAKE: non-volatile view of D_80134544 (volatile in VALEN.h for func_80132D0C, T-24903)
     * so as1 may hoist its lui above the affinity test, as the original does. T-25117 */
    if ((D_80134520 + 1 == D_8013453C) && (state->unk_F5F == 0) &&
        ((state->unk_1BC[0].unk_02 >= 0x46) || (*(u8 *)&D_80134544 != 0))) {
        func_800AE0F0(D_800CA1DC, "自宅前");
        id = 0x414C;
    }
    if ((state->unk_F5F == 9) && (D_80134544 == 1)) {
        func_800AE0F0(D_800CA1DC, "教室");
        id = 0x4055;
    }
    if (state->unk_F5F == 0) {
        if (state->unk_1BC[0].unk_02 >= 0x46) {
            D_80122D48 = 1;
        } else {
            D_80122D48 = 2;
        }
    } else {
        if (get_h_tokimeki(state->unk_F5F) >= 0x46U) {
            D_80122D48 = 1;
        } else {
            D_80122D48 = 2;
        }
    }
    switch (id) {
    case 0x4045:
        func_80085B3C(6, 0);
        break;
    case 0x4055:
        func_80085B3C(6, 1);
        break;
    case 0x414C:
        func_80085B3C(6, 5);
        break;
    case 0x4032:
        func_80085B3C(6, 4);
        break;
    case 0x407A:
        func_80085B3C(6, 3);
        break;
    default:
        func_80085B3C(6, 2);
        break;
    }
    if ((state->unk_F5F == 9) && (D_80134544 == 1)) {
        state->unk_F5E -= 1;
        D_80122D48 = 1;
        func_80085B3C(6, 3);
    }
    bg_read_sub2(id);
    if (get_h_tokimeki(state->unk_F5F) >= 0x46U) {
        func_80083440(0);
    } else {
        func_80083440(1);
    }
    if (state->unk_F5F == 0) {
        if (state->unk_1BC[0].unk_02 >= 0x46) {
            func_80083440(0);
        } else {
            func_80083440(1);
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
    bg_read_sub2(0x407A);
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
