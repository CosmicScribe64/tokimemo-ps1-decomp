#define MAIN_API_OVERRIDE_talk_names_set /* Word argument at this call; T-9520. */
#include "common.h"
#include "ovl/SHOUGATU.h"
void talk_names_set();

void func_80134120(void) {
    void (**p)();
    FnTbl25 tbl;

    tbl = D_80143D24;
    if (D_80143B20 != 0) {
        p = &tbl.f[game_state.mode_step];
        if (*p == normal_date_girl_in || tbl.f[game_state.mode_step] == normal_date_girl_out) {
            tbl.f[game_state.mode_step] = mode_step_next;
            /* FAKE: empty double test (decomp-permuter, score 0); it only shifts the register choice of base, index and pointer to the original's; real source unknown. T-4090 */
            if (!game_state.mode_step && !game_state.mode_step) {
            }
        }
    }
    tbl.f[game_state.mode_step](0x80);
}

void func_801341E4(void) {
    u16 six;
    u32 result;
    u32 mode;

    /* FAKE: the narrow call cast reserves a temporary; the dead raw-byte
     * test retains separate raw/shifted ranges, and u16 six keeps the branch
     * operand order. T-10010. */
    six = 6;
    if (D_80143B20 == 0) {
        result = (u8) get_g_zyotai_s(game_state.current_girl) & 0x7F;
        if ((game_state.current_girl == GIRL_SAKI) && (result >= 2U)) {
            mode = (u8) game_state.girls[4].flags.b[2] >> 4;
            if (game_state.girls[4].flags.b[2]) {
            }
            if ((mode == six) && (D_80143B04 == 1)) {
                D_80143B04 = 2;
            } else if ((mode != six) && (D_80143B04 == 0)) {
                D_80143B04 = 1;
            }
        }
    }
    func_801323D4();
}

void func_8013428C(void) {
    func_801323D4();
    if ((D_80143B04 == 2) && (game_state.substep == 0) && (D_8011ED5B & 0x80)) {
        audio_command(0x202);
        game_state.substep += 1;
    }
    if ((game_state.substep == 0) && (D_80143B04 == 3)) {
        audio_command(0x202);
        game_state.substep += 1;
    }
}

void func_80134340(void) {
    game_state.date_girl = game_state.current_girl;
    mode_step_next();
}

void func_8013436C(void) {
    if ((D_80143B20 == 0) && !(game_state.frame_count & 1) && (game_state.event_log[game_state.event_log_count - 1].scene != 5)) {
        if (D_80143B24 != 0) {
            event_log_add(5, 4);
        } else {
            event_log_add(5, 1);
        }
    }
    bg_read_sub2(0x41F6);
    mode_step_next();
}

void func_80134400(void) {
    s32 pad; /* FAKE: unused local declared first puts tbl at sp+0x28 as in the original; real source unknown. T-9340 */
    SjisStr3 tbl;

    tbl = D_80143D88;
    D_80143D20 = D_80122CDC;
    D_80143B00 += D_80122CDC;
    strcpy(D_800CA250, tbl.s[D_80122CDC]);
    if (D_80143B20 == 0 && game_state.date_girl == GIRL_AYAKO && D_80122CDC == 2) {
        strcpy(D_800CA250, tbl.s[1]);
    }
    if (D_80122CDC == 0) {
        game_state.param_liberal_arts.value += 0xA;
        game_state.param_science.value += 0xA;
        game_state.current_girl = GIRL_SOTOI;
    } else if (D_80122CDC == 1) {
        game_state.param_condition.value += 0x19;
        game_state.current_girl = GIRL_SOTOI;
    }
    check_para_limit();
    mode_step_next();
}

void func_80134554(void) {
    u8 sel = D_80143D20; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 2:
        break;
    case 0:
        D_80143B00 += 3;
        break;
    case 1:
        D_80143B00 += 2;
        break;
    }
    mode_step_next();
}

void func_801345C8(void) {
    s32 v;

    if (D_80143B20 == 0) {
        /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
        v = (u8)get_g_zyotai_s(game_state.current_girl) & 0x7F;
        if ((v != 0) && (v != 1)) {
            if (v != 2) {
                D_80143B00 += 2;
            } else {
                D_80143B00 += 1;
            }
        }
    }
    mode_step_next();
}

void func_80134648(void) {
    s32 v;

    if (D_80143B20 == 0) {
        /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
        v = (u8)get_g_zyotai_s(game_state.current_girl) & 0x7F;
        switch (v) {
        case 0:
        case 1:
            D_80143B00 += 2;
            break;
        case 2:
            D_80143B00 += 1;
            break;
        }
    } else {
        D_80143B00 += 2;
    }
    mode_step_next();
}

void func_801346E8(void) {
    if (D_80122CDC != 2) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
    }
    mode_step_next();
}

/* FAKE: empty do loops and size ^ 0 preserve the two original multiplications
 * and temporary allocation; keep statement-per-line layout. Source unknown.
 * The word-argument prototype override above is required here. T-9520. */
void func_80134730(void)
{
    u32 size;
    s32 index;
    talk_names_set(D_80122CDC);
    size = 0x38;
    index = D_80122CDC;
    do { } while (0);
    ((GameState *) (((u32) (&game_state)) + (index * size)))->girls[0].tokimeki += 5;
    ((GameState *) (((u32) (&game_state)) + (index * (size ^ 0))))->girls[0].affection += 1;
    do { check_para_limit(size, index); } while (0);
    game_state.current_girl = GIRL_SOTOI;
    mode_step_next();
}

void func_801347B4(void) {
    D_80122CEC = D_80122CDC;
    mode_step_next();
}

/* FAKE: passing girl to talk_names_set colors the entry range to $a0, and the outer * 0 subtree reserves $v1 so game_state.current_girl is promoted to $a0 after get_g_zyotai_s without emitting instructions in ugen. T-24137 */
void func_801347E0(void) {
    u32 state;
    s32 girl;

    girl = game_state.date_girl;
    game_state.current_girl = girl;
    talk_names_set(girl);
    D_80122CDC = D_80122CEC;
    state = (u8)get_g_zyotai_s(GIRL_NOZOMI) & 0x7F;
    if ((((((D_80143B00 * 0) + (D_80143B00 * 0) + (D_80143B00 * 0)) * 0) + game_state.current_girl) == ((game_state.current_girl * 0) + 6)) && (state < 2) && (D_80143B20 == 0)) {
        D_80143AF4 = D_80143A60;
        D_80143AF8 = D_80143A9C;
        D_80143AFC = D_80143AD8;
    }
    mode_step_next();
}
