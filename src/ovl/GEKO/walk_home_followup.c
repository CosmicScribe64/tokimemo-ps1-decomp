/* walk_home_followup.c: src/ovl/GEKO/801338F0.c:12,81,143; follow-up callback dispatch, club/topic flags and dialogue group selection. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x801338F0-0x80134040 (config/objects/GEKO.txt). */
#define MAIN_API_OVERRIDE_strcpy /* FAKE: T-25002 implicit-int strcpy view (s32 return) keeps the original return register allocation in func_80133C50. */
#include "common.h"
#include "ovl/GEKO.h"

extern s32 strcpy();

typedef struct {
    void (*f[19])();
} FnTbl19; /* size 0x4C */
extern FnTbl19 D_80144D40;

void func_801338F0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl19 tbl;

    tbl = D_80144D40;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_8013396C(void) {
    u32 cur;

    cur = D_80144C50;
    if (cur == 0x64) {
        func_801322E8();
        return;
    }
    mode_step_next();
}

void func_801339AC(void) {
    u32 cur;
    u16 raw;

    cur = D_80144C50;
    if (cur == 0x64) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
    } else if (game_state.current_girl == GIRL_SAKI) {
        if (D_80144C3C == 0x26) {
            raw = game_state.player_flags.h;
            if (((raw >> 0xC) == 6) && ((game_state.girls[4].flags.b[2] >> 4) == 6)) {
                D_80144C3C = 0x28;
            } else if (((raw >> 0xC) == 7) && ((game_state.girls[4].flags.b[2] >> 4) == 7)) {
                D_80144C3C = 0x29;
            } else {
                D_80144C3C = 0x2A;
            }
        } else {
            mode_step_next();
        }
    } else {
        mode_step_next();
    }
    mode_step_next();
}

s32 func_80133AA0() {
    /* FAKE: `c50` is a word local above `state` that puts `state` at 0x2B(sp) and avoids cvt_pass widening on D_80144C50 so uopt keeps bne reg,$v1; `| 0` commutes xor $v0,$v1,reg. T-24090 */
    u32 c50;
    u8 state;

    state = get_g_zyotai_h(game_state.current_girl);
    func_801322E8();
    if (*(s16 *)&D_80144C40 == 2 && game_state.current_girl == GIRL_YUKO && (c50 = D_80144C50) == 1U && (game_state.mode_step_timer.u++ | 0) == 1U) {
        portrait_layout_set(3U);
    }
    if (*(s16 *)&D_80144C40 == 4 && game_state.current_girl == GIRL_YUKO && (c50 = D_80144C50) == 1U && (game_state.mode_step_timer.u++ | 0) == 1U) {
        portrait_layout_set(state);
    }
    if ((*(s16 *)&D_80144C40 == 1 || *(s16 *)&D_80144C40 == 5) && game_state.current_girl == GIRL_YUMI && (c50 = D_80144C50) == 1U && (game_state.mode_step_timer.u++ | 0) == 1U) {
        portrait_layout_set(2U);
    }
    if (*(s16 *)&D_80144C40 == 3 && game_state.current_girl == GIRL_YUMI && (c50 = D_80144C50) == 1U && (game_state.mode_step_timer.u++ | 0) == 1U) {
        portrait_layout_set(3U);
    }
}

void func_80133C50(void) {
    s32 v;
    u32 g;
    GekoNames12 names;
    u32 temp;

    names = D_80144D8C;
    strcpy(&D_800CA280, names.name[game_state.player_flags.h >> 12]);
    game_state.girls[game_state.current_girl].flags.f.b0 = 1;
    if (D_80144C50 == 10) {
        game_state.girls[game_state.current_girl].flags.f.b6 = 1;
    }
    D_80144C3C = 0x16;
    v = get_g_zyotai_s(game_state.current_girl);
    /* FAKE: T-25002 reading D_80144C50 through the word local g keeps bne t8,a0 operand order */
    if ((g = D_80144C50) == 2U) {
        g = game_state.current_girl;
        D_80144C50 = 0;
        if (g == 0) {
            /* FAKE: T-11910 the volatile club view keeps this read before the record load (uopt otherwise sinks it to
             * the first compare), giving the original ugen order; also used in the 1/2/3 branch. */
            v = game_state.player_flags.vh >> 12;
            temp = game_state.girls[g].flags.b[2] >> 4;
            g = temp;
            if (v == g && (u32)v < 6) {
                func_80132A4C(2);
            } else if (v != g && (u32)v < 6 && g < 6) {
                func_80132A4C(3);
            } else if ((u32)v < 6 && g >= 6) {
                func_80132A4C(4);
            } else {
                func_80132A4C(5);
            }
        } else if (g == 1 || g * 1 == 2 || g == 3 /* FAKE: T-25002 g * 1 puts the shared constant 2 first in the beq */) {
            v = game_state.player_flags.vh >> 12;
            g = game_state.girls[game_state.current_girl].flags.b[2] >> 4;
            if (v == g) {
                func_80132A4C(2);
            } else if (v != g && (u32)v < 6) {
                func_80132A4C(3);
            } else {
                func_80132A4C(5);
            }
        } else if (g == 4 || g == 5 || g == 10) {
            if ((game_state.player_flags.h >> 12) < 6) {
                func_80132A4C(3);
            } else if ((u8)(v & 0x7F) < 2) {
                func_80132A4C(4);
            } else {
                func_80132A4C(5);
            }
        } else if (g == 6 || g == 7 || g == 8 || g == 9) {
            if ((game_state.player_flags.h >> 12) < 6) {
                func_80132A4C(2);
            } else {
                func_80132A4C(3);
            }
        }
    }
    mode_step_next();
}

void func_80133F94(void) {
    u32 cur;

    D_80144C3C = (D_80144C3C + D_80144C50) - 1;
    cur = D_80144C50;
    if (cur == 0x64) {
        D_80144C3C = 0x28;
    }
    mode_step_next();
}

void func_80133FE8(void) {
    D_80144C3C = 0x27;
    mode_step_next();
}

void func_80134010(void) {
    bg_read_sub2(0x42D8);
    mode_step_next();
}
