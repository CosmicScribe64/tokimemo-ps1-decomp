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
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_801327E8(void) {
    talk_names_set(9);
    func_80132348();
}

void func_80132810(void) {
    game_state.current_girl = GIRL_SHIORI;
    mode_step_next();
}

/* Locals take frame slots top-down in declaration order, so this order puts candidates at sp+0x5C
 * and the scores at sp+0x50 (T-25117). Reads through `state` are pointer loads that uopt lets every
 * later store change: the ranking loop reloads both fields for the first two score assignments, and
 * `r` stays ahead of the rotation loop's guard instead of being copied into it. The direct
 * game_state reads stay available, so the player's affinity is hoisted out of the last loop. Each
 * sum is written in the order the original loads its two fields. */
void func_80132834(void) {
    s16 r;
    s16 candidates[13];
    GameState *state;
    s16 i;
    s16 rank_scores[3];
    s16 count;

    state = &game_state;
    D_80134520 = 0;
    D_80134548 = 0;
    D_8013453C = 0;
    D_80134544 = 0;
    count = 0;
    if (game_state.girls[9].flags.f.met == 0 &&
        game_state.girls[9].tokimeki >= 20) {
        D_80134544 = 1;
        game_state.girls[9].affection = 50;
    }
    D_80134540[0] = D_80134540[1] = D_80134540[2] = 0xFF;
    rank_scores[0] = rank_scores[1] = rank_scores[2] = 0;
    for (i = 0; i < 11; i++) {
        if (state->girls[i].flags.f.met != 0 &&
            get_h_tokimeki(i) >= 0x46U) {
            if (rank_scores[0] < state->girls[i].affection + state->girls[i].tokimeki) {
                rank_scores[2] = rank_scores[1];
                rank_scores[1] = rank_scores[0];
                rank_scores[0] = state->girls[i].tokimeki + state->girls[i].affection;
                D_80134540[2] = D_80134540[1];
                D_80134540[1] = D_80134540[0];
                D_80134540[0] = i;
            } else if (rank_scores[1] < state->girls[i].affection + state->girls[i].tokimeki) {
                rank_scores[2] = rank_scores[1];
                rank_scores[1] = state->girls[i].tokimeki + state->girls[i].affection;
                D_80134540[2] = D_80134540[1];
                D_80134540[1] = i;
            } else if (rank_scores[2] < state->girls[i].affection + state->girls[i].tokimeki) {
                rank_scores[2] = state->girls[i].affection + state->girls[i].tokimeki;
                D_80134540[2] = i;
            }
        }
    }
    for (i = 0; i < 11; i++) {
        if ((game_state.girls[i].flags.f.met != 0 ||
             (i == 9 && D_80134544 == 1)) &&
            (get_h_yuukou(i) >= 0x32U || (D_80134544 == 1 && i == 9))) {
            candidates[count] = i;
            count += 1;
            D_8013453C += 1;
            game_state.girls[i].flags.f.b8 = 1;
        }
    }
    if (D_8013453C > 0) {
        r = (u32)state->frame_count % (u32)D_8013453C;
        for (i = 0; i < D_8013453C; i++) {
            D_80134524[i] = candidates[(i + r) % D_8013453C];
        }
        func_8013387C();
    }
    for (i = 0; i < D_8013453C; i++) {
        if (D_80134524[i] == 0 && game_state.girls[0].tokimeki >= 70) {
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
    mode_step_next();
}

/* The mode flag is read twice on purpose (T-24903): as1's global scheduler moves the
 * head `lui` of a dominated block into a branch delay slot, so the first read has to be
 * a volatile access (VALEN.h) to keep `lui v0` out of the `sel == 9` slot, and the second
 * read keeps uopt from loading the byte once instead (13 rows). In the original the
 * `sel == 9` slot holds the switch's dispatch `sll` (wiki/as1-scheduler-rule.md). */
s32 func_80132D0C(void) {
    s32 mode;
    GameState *state;

    state = &game_state;

    if (D_80134520 == D_8013453C) {
        mode_stage_next();
        return 0;
    }
    state->current_girl = D_80134524[D_80134520];
    talk_names_set(state->current_girl);
    D_80134498 = get_h_tokimeki(state->current_girl) >= 0x46U;
    if (state->current_girl == GIRL_SHIORI) {
        D_80134498 = state->girls[0].tokimeki >= 0x46;
    }
    if (state->current_girl == GIRL_MEGUMI) {
        mode = D_80134544;
        if (mode == 1) {
            D_80134498 = 3;
        }
    }
    mode = D_80134544;
    if ((state->current_girl == GIRL_SHIORI) && (mode == 1) && (state->girls[0].tokimeki >= 0x46)) {
        D_80134498 = 3;
    }
    if ((state->current_girl == GIRL_SHIORI) && (mode == 1) && (state->girls[0].tokimeki < 0x46)) {
        D_80134498 = 5;
    }
    switch (state->current_girl) {
    case GIRL_SHIORI:
        D_8013448C = D_801343F8;
        D_80134490 = D_8013442C;
        D_80134494 = D_80134460;
        break;
    case GIRL_MIO:
        D_8013448C = D_8013440C;
        D_80134490 = D_80134440;
        D_80134494 = D_80134474;
        break;
    case GIRL_YUINA:
        D_8013448C = D_801343FC;
        D_80134490 = D_80134430;
        D_80134494 = D_80134464;
        break;
    case GIRL_AYAKO:
        D_8013448C = D_80134404;
        D_80134490 = D_80134438;
        D_80134494 = D_8013446C;
        break;
    case GIRL_SAKI:
        D_8013448C = D_8013441C;
        D_80134490 = D_80134450;
        D_80134494 = D_80134484;
        break;
    case GIRL_YUKARI:
        D_8013448C = D_80134410;
        D_80134490 = D_80134444;
        D_80134494 = D_80134478;
        break;
    case GIRL_NOZOMI:
        D_8013448C = D_80134414;
        D_80134490 = D_80134448;
        D_80134494 = D_8013447C;
        break;
    case GIRL_MIRA:
        D_8013448C = D_80134408;
        D_80134490 = D_8013443C;
        D_80134494 = D_80134470;
        break;
    case GIRL_YUKO:
        D_8013448C = D_801343F0;
        D_80134490 = D_80134424;
        D_80134494 = D_80134458;
        break;
    case GIRL_MEGUMI:
        D_8013448C = D_80134418;
        D_80134490 = D_8013444C;
        D_80134494 = D_80134480;
        break;
    case GIRL_YUMI:
        D_8013448C = D_80134420;
        D_80134490 = D_80134454;
        D_80134494 = D_80134488;
        break;
    }
    mode_step_next();
}

void func_80133088(void) {
    GameState *state;
    s32 id;

    state = &game_state;
    /* FAKE: dead entry store (every arm below rewrites the global); it gives the promoted
     * global $v0 and the random word $v1 (without it they take $v1 and $a0), as the entry
     * stores of the GYOZI twin func_801393C8 do (T-24130). T-25117 */
    D_80134548 = state->frame_count & 7;
    if ((state->frame_count & 7) == 0) {
        D_80134548 = 3;
    } else if ((state->frame_count & 7) < 4U) {
        D_80134548 = 0;
    } else {
        D_80134548 = 1;
    }
    if (state->current_girl == GIRL_SHIORI) {
        D_80134548 = state->frame_count & 1;
    }
    switch (D_80134548) {
    case 0:
        strcpy(D_800CA1DC, "廊下");
        id = 0x4045;
        break;
    case 1:
        strcpy(D_800CA1DC, "教室");
        id = 0x4055;
        break;
    case 2:
        strcpy(D_800CA1DC, "校舎裏");
        id = 0x407A;
        break;
    default:
        strcpy(D_800CA1DC, "屋上");
        id = 0x4084;
        break;
    }
    if (state->current_girl != GIRL_SHIORI) {
        if (D_80134520 + 1 == D_8013453C) {
            strcpy(D_800CA1DC, "校門前");
            id = 0x4032;
        }
        if ((D_80134520 + 2 == D_8013453C) && (D_80134524[D_80134520 + 1] == 0)) {
            strcpy(D_800CA1DC, "校門前");
            id = 0x4032;
        }
    }
    /* FAKE: non-volatile view of D_80134544 (volatile in VALEN.h for func_80132D0C, T-24903)
     * so as1 may hoist its lui above the affinity test, as the original does. T-25117 */
    if ((D_80134520 + 1 == D_8013453C) && (state->current_girl == GIRL_SHIORI) &&
        ((state->girls[0].tokimeki >= 0x46) || (*(u8 *)&D_80134544 != 0))) {
        strcpy(D_800CA1DC, "自宅前");
        id = 0x414C;
    }
    if ((state->current_girl == GIRL_MEGUMI) && (D_80134544 == 1)) {
        strcpy(D_800CA1DC, "教室");
        id = 0x4055;
    }
    if (state->current_girl == GIRL_SHIORI) {
        if (state->girls[0].tokimeki >= 0x46) {
            D_80122D48 = 1;
        } else {
            D_80122D48 = 2;
        }
    } else {
        if (get_h_tokimeki(state->current_girl) >= 0x46U) {
            D_80122D48 = 1;
        } else {
            D_80122D48 = 2;
        }
    }
    switch (id) {
    case 0x4045:
        event_log_add(6, 0);
        break;
    case 0x4055:
        event_log_add(6, 1);
        break;
    case 0x414C:
        event_log_add(6, 5);
        break;
    case 0x4032:
        event_log_add(6, 4);
        break;
    case 0x407A:
        event_log_add(6, 3);
        break;
    default:
        event_log_add(6, 2);
        break;
    }
    if ((state->current_girl == GIRL_MEGUMI) && (D_80134544 == 1)) {
        state->event_log_count -= 1;
        D_80122D48 = 1;
        event_log_add(6, 3);
    }
    bg_read_sub2(id);
    if (get_h_tokimeki(state->current_girl) >= 0x46U) {
        portrait_layout_set(0);
    } else {
        portrait_layout_set(1);
    }
    if (state->current_girl == GIRL_SHIORI) {
        if (state->girls[0].tokimeki >= 0x46) {
            portrait_layout_set(0);
        } else {
            portrait_layout_set(1);
        }
    }
    mode_step_next();
}

void func_801334DC(void) {
    D_80134520 += 1;
    mode_step_set(1);
}

void func_80133510(void) {
    if (strcmp(D_800CA19C, D_800CA1DC) == 0) {
        mode_step_next();
    }
    strcpy(D_800CA19C, D_800CA1DC);
    mode_step_next();
}

void func_80133568(void) {
    if ((D_80134544 == 1) && (game_state.current_girl == GIRL_MEGUMI)) {
        game_state.mode_step += 6;
        return;
    }
    mode_step_next();
}

void func_801335C0(void) {
    strcpy(D_800CA1DC, "校舎裏");
    strcpy(D_800CA19C, "　");
    bg_read_sub2(0x407A);
    mode_step_next();
}

void func_80133610(void) {
    talk_names_set(0);
    portrait_read_start(0x544A);
    mode_step_next();
}

void func_80133640(void) {
    talk_names_set(9);
    portrait_read_start(0x64D0);
    mode_step_next();
}

s32 func_80133670(void) {
    GameState *g; /* the selected girl is read through a state pointer (lui v0 base for the call argument), T-19016 */

    g = &game_state;
    if ((game_state.day == ((u32)(game_state.player_birthday << 0x17) >> 0x1B)) && (game_state.month == (game_state.player_birthday & 0xF)) && (get_h_tokimeki(g->current_girl) >= 0x46U) && ((D_80134540[0] == g->current_girl) || (D_80134540[1] == g->current_girl) || (D_80134540[2] == g->current_girl)) && (g->current_girl == GIRL_MEGUMI)) {
        mode_step_next();
        return 0;
    }
    return normal_date_girl_out();
}

/* s32: the first exit returns 0 (or v0,zero,zero in its branch delay slot), the second is a bare return and the last call falls off the end. T-19016 */
s32 func_80133728(void) {
    GameState *g; /* the selected girl is read through a state pointer (lui v0 base for the call argument), T-19016 */

    g = &game_state;
    if ((game_state.day == ((u32)(game_state.player_birthday << 0x17) >> 0x1B)) && (game_state.month == (game_state.player_birthday & 0xF)) && (((get_h_tokimeki(g->current_girl) >= 0x46U) && (g->current_girl != GIRL_SHIORI)) || ((game_state.girls[0].tokimeki >= 0x46) && (g->current_girl == GIRL_SHIORI))) && ((D_80134540[0] == g->current_girl) || (D_80134540[1] == g->current_girl) || (D_80134540[2] == g->current_girl))) {
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        mode_stage_set(3);
        return 0;
    }
    if (((get_h_tokimeki(g->current_girl) >= 0x46U) && (g->current_girl != GIRL_SHIORI)) || ((game_state.girls[0].tokimeki >= 0x46) && (g->current_girl == GIRL_SHIORI))) {
        func_80132348();
        return;
    }
    mode_step_next();
}

void func_8013387C(void) {
    s16 n;
    s16 i;
    s16 j;
    s16 k;
    s16 t;

    srand(game_state.frame_count);
    n = rand() % 256 + 10;
    for (i = 0; i < n; i++) {
        j = rand() % D_8013453C;
        k = rand() % D_8013453C;
        t = D_80134524[j];
        D_80134524[j] = D_80134524[k];
        D_80134524[k] = t;
    }
}
