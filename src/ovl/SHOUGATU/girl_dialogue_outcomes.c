/* girl_dialogue_outcomes.c: ; src/ovl/SHOUGATU/8013BA00.c:4-10,13-50,68-83 dispatches outcome steps, selects the current girl, applies relationship changes and advances the day event. Original object 0x8013BA00-0x8013C070 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013BA00(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl19 tbl;

    tbl = D_80145A80;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

s32 func_8013BA7C(void) {
    s16 i;

    if (game_state.substep == 0) {
        func_800674B0();
        game_state.substep += 1;
    }
    if (game_state.substep == 1) {
        if (game_state.mode_step_timer.u++ >= 0x400U) {
            cd_command_reset();
            audio_bank_state_reset();
            game_state.substep = 0;
        }
        if (audio_bank_load_poll() == 1) {
            game_state.substep += 1;
        } else {
            return 0;
        }
    }
    for (i = 0; i < 11; i++) {
        if (((ShouByteFlags *)&game_state.girls[i].flags.b[1])->b0 != 0) {
            ((ShouByteFlags *)&game_state.girls[i].flags.b[1])->b0 = 0;
            if (i != (s32)game_state.current_girl) {
                game_state.girls[i].affection = game_state.girls[i].affection + 1;
                /* FAKE: uopt CSE priority boost so &game_state + i * 0x38 gets $v0 (i in $v1)
                 * and &game_state gets $a1 (0x38 in $a2), matching pre-unification GameState layout. T-24125. */
                if (game_state.girls[i].affection) {
                }
                if (game_state.girls[0].affection) {
                }
            }
        }
    }
    game_state.girls[game_state.current_girl].tokimeki += 8;
    game_state.girls[game_state.current_girl].affection += 8;
    game_state.girls[game_state.current_girl].syoushin -= 10;
    check_para_limit();
    day_event_next(1);
}

void func_8013BC2C(void) {
    s16 i;

    D_801459FC = D_8014598C;
    D_80145A00 = D_801459C0;
    D_80145A04 = D_801459F4;
    D_80145A08 = 0;
    D_80145A0C = 0;
    for (i = 0; i < 0xB; i++) {
        D_800CA2B0[i] = (game_state.girls[i].flags.b[1] & 1) >= 1;
    }
    mode_step_next();
}

void func_8013BCE0(void) {
    game_state.current_girl = (u8) D_80122CDC;
    event_log_add(7, 0);
    talk_names_set(game_state.current_girl);
    mode_step_next();
}

void func_8013BD2C(void) {
    u8 s;

    s = get_g_zyotai_s(game_state.current_girl) & 0x7F;
    if (s >= 2U) {
        D_80145A08 += 2;
    }
    if (s >= 3U) {
        D_80145A08 += 2;
    }
    if (s == 4) {
        D_80145A08 += 2;
    }
    if (game_state.current_girl == GIRL_MEGUMI && s < 2U) {
        portrait_layout_set(0);
    }
    mode_step_next();
}

void func_8013BDE0(void) {

    D_80145A08 = 0;
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        D_801459FC = D_80145964;
        D_80145A00 = D_80145998;
        D_80145A04 = D_801459CC;
        break;
    case GIRL_MIO:
        D_801459FC = D_80145978;
        D_80145A00 = D_801459AC;
        D_80145A04 = D_801459E0;
        break;
    case GIRL_YUINA:
        D_801459FC = D_80145968;
        D_80145A00 = D_8014599C;
        D_80145A04 = D_801459D0;
        break;
    case GIRL_AYAKO:
        D_801459FC = D_80145970;
        D_80145A00 = D_801459A4;
        D_80145A04 = D_801459D8;
        break;
    case GIRL_SAKI:
        D_801459FC = D_80145988;
        D_80145A00 = D_801459BC;
        D_80145A04 = D_801459F0;
        break;
    case GIRL_YUKARI:
        D_801459FC = D_8014597C;
        D_80145A00 = D_801459B0;
        D_80145A04 = D_801459E4;
        break;
    case GIRL_NOZOMI:
        D_801459FC = D_80145980;
        D_80145A00 = D_801459B4;
        D_80145A04 = D_801459E8;
        break;
    case GIRL_MIRA:
        D_801459FC = D_80145974;
        D_80145A00 = D_801459A8;
        D_80145A04 = D_801459DC;
        break;
    case GIRL_YUKO:
        D_801459FC = D_80145960;
        D_80145A00 = D_80145994;
        D_80145A04 = D_801459C8;
        break;
    case GIRL_MEGUMI:
        D_801459FC = D_80145984;
        D_80145A00 = D_801459B8;
        D_80145A04 = D_801459EC;
        break;
    case GIRL_YUMI:
        D_801459FC = D_80145990;
        D_80145A00 = D_801459C4;
        D_80145A04 = D_801459F8;
        break;
    }
    mode_step_next();
}
