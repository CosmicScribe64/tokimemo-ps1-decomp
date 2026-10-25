/* annual_candidate_results.c: ; src/ovl/GYOZI/80140C90.c:17-39 evaluates candidate status and follows result state. Original object 0x80140C90-0x801418B0 (config/objects/GYOZI.txt). */
#define MAIN_API_OVERRIDE_func_800BCDF0 /* T-24523: integer debug-call result keeps the following flag out of v0. */
#define MAIN_API_OVERRIDE_func_8008F618 /* T-24712: integer result (viewsweep) keeps the loop counter in $v1. */
#include "common.h"
#include "ovl/GYOZI.h"

s32 func_800BCDF0();
s32 func_8008F618();

typedef struct {
    s32 (*f[26])();
} FnTbl26R; /* size 0x68 */
extern FnTbl26R D_8014877C;

/* The record fields at 0x8012B8C7..0x8012B8D8 are reached through one base: the original keeps the
 * stores and loads in program order (separate scalars let as1 hoist the loads). The result of the
 * last operation is left in $v0 without a return statement (T-4060). */
s16 annual_candidate_result_mode_step_dispatch(void) {
    s32 mode_step_index; /* declared before candidate_result_callbacks: its stack slot sits above candidate_result_callbacks (T-3330) */
    FnTbl26R candidate_result_callbacks;

    candidate_result_callbacks = D_8014877C;
    mode_step_index = D_800F647A;
    candidate_result_callbacks.f[mode_step_index](0x80);
    *(s8 *)(&D_8012B8C2 + 5) = *(u8 *)&D_801317EB;
    if ((*(s16 *)(&D_8012B8C2 + 0x16) == 0) && (*(s16 *)(&D_8012B8C2 + 6) == 0)) {
        *(s16 *)(&D_8012B8C2 + 0x14) = *(s16 *)(&D_8012B8C2 + 0x14) + 1;
        if (*(s16 *)(&D_8012B8C2 + 0x14) >= 3) {
            *(s16 *)(&D_8012B8C2 + 0x14) = 0;
        }
    }
}

void annual_candidate_rank_gate(void) {
    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    if (((u8)func_8005E0E0(*(u8 *)&D_800F62CF) & 0x7F) != 4) {
        func_8004DE1C();
        return;
    }
    annual_result_talk_run_current();
}

void annual_candidate_override_counter_update(void) {
    if (D_80148750 != 0) {
        D_80148620 += 1;
    }
    func_8004DE1C();
}

void annual_candidate_girl_result_apply(void) {
    s16 i;
    s32 girl;

    /* FAKE: late-folded zero and early assignment preserve the entry $a1
     * carrier; the compiler sinks the store past the flag tests. T-25035 */
    girl = D_800F62CF * 0;
    D_800F62CF = girl;
    D_80148750 = 0;
    if (((GyoziBits32 *)D_800F53A0.girl[9].unk_10)->b1 == 0 &&
        D_800F53A0.girl[9].unk_06 >= 20 && (D_800F6468 & 1)) {
        D_80148750 = 1;
    }
    /* FAKE: narrowing spends the original temporary slots; the byte argument
     * view keeps the later reload's address base in $a1. T-24712, T-25035 */
    D_80148620 = (((u8)func_8005E0E0(*(u8 *)&D_800F62CF) & 0x7F) == 4) * 4;
    D_80148614 = D_801481D4;
    D_80148618 = D_80148208;
    D_8014861C = D_8014823C;
    func_8008F618(D_800F62CF);
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0A += 4;
    for (i = 0; i < 7; i++) {
        if (D_800F54AE < D_8014875C[i]) {
            break;
        }
    }
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_06 += D_8014876C[i];
    func_80090960(4, 0);
    func_8004DE1C();
}

void annual_random_target_girl_select_and_reward(void) {
    s16 girl_index;
    s16 met_girl_count;
    s32 candidate_status;

    met_girl_count = 0;
    for (girl_index = 0; girl_index < 11; girl_index++) {
        met_girl_count += ((GyoziBits32 *)D_800F53A0.girl[girl_index].unk_10)->b1 >= 1;
    }
    girl_index = 0;
    do {
        /* FAKE: multiplication by one preserves the original sum operand order. T-24523 */
        D_800F53A0.unk_F2F = (D_800F6468 * 1 + girl_index) % 10U + 1;
        func_800BCDF0("num%d girl%d\n", met_girl_count, D_800F53A0.unk_F2F);
        girl_index++;
    } while ((!((GyoziBits32 *)D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10)->b1 && D_800F53A0.unk_F2F >= 9) ||
        (!((GyoziBits32 *)D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10)->b1 && met_girl_count >= 2));
    if (D_80148750 != 0) {
        D_800F53A0.unk_F2F = 9;
    }
    D_80148754 = D_800F53A0.unk_F2F;
    D_80148620 = 0;
    /* FAKE: the redundant byte conversion reserves the original temporary slots. T-24523 */
    candidate_status = (u8)func_8005E0E0(D_800F53A0.unk_F2F);
    if (D_800F53A0.unk_F2F == 6 && (candidate_status & 0x7F) >= 2U) {
        D_80148620++;
    }
    if ((candidate_status & 0x7F) == 4) {
        D_80148620++;
    }
    annual_result_dialogue_tables_select();
    func_8008F618(D_800F62CF);
    if (!((GyoziBits32 *)D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10)->b1 || (D_80148750 != 0 && D_800F53A0.unk_F2F == 9)) {
        ((GyoziBits32 *)D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10)->b1 = 1;
        D_800F53A0.unk_710 = D_800F53A0.unk_10D8;
        D_800F53A0.unk_711 = D_800F53A0.unk_10D9;
        D_800F53A0.unk_712 = 0x11;
        func_80090960(4, 0);
        D_800F53A0.unk_713[D_800F53A0.unk_F2E * 8 + 0x1E] = 2;
        func_8004DEAC(5);
        return;
    }
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0A += 4;
    for (girl_index = 0; girl_index < 7; girl_index++) {
        if (D_800F54AE < D_8014875C[girl_index]) {
            break;
        }
    }
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_06 += D_8014876C[girl_index];
    func_80090960(4, 0);
    func_8004DE1C();
}

void annual_target_girl_select_and_reward(void) {
    s16 i;
    s16 count;

    count = 0;
    /* FAKE: the body on the for line keeps as1's per-line order (the count copy before the
     * branch, the counter copy in its delay slot). T-25151 */
    for (i = 0; i < 11; i++) { count += ((GyoziBits32 *)D_800F53A0.girl[i].unk_10)->b1 >= 1; }
    i = 0;
    do {
        /* FAKE: the s32 view of the random word (the type of D_800F6468) puts the load first in
         * the sum; the member read keeps it reloaded after the unk_F2F store. T-25151 */
        D_800F53A0.unk_F2F = ((s32)D_800F53A0.unk_10C8 + i) % 10U + 1;
        i++;
    } while ((!((GyoziBits32 *)D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10)->b1 && D_800F53A0.unk_F2F >= 9) ||
        D_800F53A0.unk_F2F == D_80148754 ||
        (!((GyoziBits32 *)D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10)->b1 && count >= 3));
    i = D_801486E4; /* the counter is reused for the requested girl (s16 compares, T-25151) */
    if ((i == 7 || i == 9) && i != D_80148754) {
        D_800F53A0.unk_F2F = i;
    }
    D_80148758 = D_800F53A0.unk_F2F;
    D_80148620 = 0;
    annual_result_dialogue_tables_select();
    func_8008F618(D_800F62CF);
    if (!((GyoziBits32 *)D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10)->b1) {
        ((GyoziBits32 *)D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10)->b1 = 1;
        D_800F53A0.unk_710 = D_800F53A0.unk_10D8;
        D_800F53A0.unk_711 = D_800F53A0.unk_10D9;
        D_800F53A0.unk_712 = 0x18;
        func_80090960(4, 0);
        D_800F53A0.unk_713[D_800F53A0.unk_F2E * 8 + 0x1E] = 2;
        func_8004DEAC(5);
        return;
    }
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0A += 4;
    for (i = 0; i < 7; i++) {
        if (D_800F54AE < D_8014875C[i]) {
            break;
        }
    }
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_06 += D_8014876C[i];
    func_80090960(4, 0);
    func_8004DE1C();
}

void annual_result_dialogue_tables_select(void) {
    switch (D_800F62CF) {
    case 1:
        D_80148614 = D_801481E4;
        D_80148618 = D_80148218;
        D_8014861C = D_8014824C;
        return;
    case 2:
        D_80148614 = D_801481D8;
        D_80148618 = D_8014820C;
        D_8014861C = D_80148240;
        return;
    case 3:
        D_80148614 = D_801481DC;
        D_80148618 = D_80148210;
        D_8014861C = D_80148244;
        return;
    case 4:
        D_80148614 = D_801481F4;
        D_80148618 = D_80148228;
        D_8014861C = D_8014825C;
        return;
    case 5:
        D_80148614 = D_801481E8;
        D_80148618 = D_8014821C;
        D_8014861C = D_80148250;
        return;
    case 6:
        D_80148614 = D_801481EC;
        D_80148618 = D_80148220;
        D_8014861C = D_80148254;
        return;
    case 7:
        D_80148614 = D_801481E0;
        D_80148618 = D_80148214;
        D_8014861C = D_80148248;
        return;
    case 8:
        D_80148614 = D_801481D0;
        D_80148618 = D_80148204;
        D_8014861C = D_80148238;
        return;
    case 9:
        D_80148614 = D_801481F0;
        D_80148618 = D_80148224;
        D_8014861C = D_80148258;
        return;
    case 10:
        D_80148614 = D_80148200;
        D_80148618 = D_80148234;
        D_8014861C = D_80148268;
    default:
        return;
    }
}

void annual_candidate_voice_resource_select(void) {
    switch (D_800F62CF) {
    case 0:
        func_80072734(0x5617);
        break;
    case 1:
        func_80072734(0x5DA6);
        break;
    case 2:
        func_80072734(0x52C0);
        break;
    case 3:
        func_80072734(0x5941);
        break;
    case 4:
        func_80072734(0x66CA);
        break;
    case 5:
        func_80072734(0x6049);
        break;
    case 6:
        func_80072734(0x6292);
        break;
    case 7:
        func_80072734(0x5BB7);
        break;
    case 8:
        func_80072734(0x50FE);
        break;
    case 9:
        if (D_80148750 != 0) {
            func_80072734(0x6508);
        } else {
            func_80072734(0x63A0);
        }
        break;
    case 10:
        func_80072734(0x677E);
        break;
    }
    func_8004DE1C();
}
