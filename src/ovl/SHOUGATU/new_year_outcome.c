/* new_year_outcome.c: ; src/ovl/SHOUGATU/80132D00.c:19-53 handles the New Year scene outcome branch. Original object 0x80132D00-0x801330C0 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "ovl/SHOUGATU.h"

void func_80132D00(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_80143B60;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_80132D88(void) {
    audio_command(0x603);
    mode_step_next();
}

void func_80132DB0(void) {
    strcpy(D_800CA1DC, "正月");
    mode_step_next();
}

void func_80132DE4(void) {
    bg_read_sub2(0x4144);
    mode_step_next();
}

void func_80132E0C(void) {
    if (D_80122CDC != 0) {
        if ((game_state.current_girl == GIRL_YUINA) || (game_state.current_girl == GIRL_MIRA) || (game_state.current_girl == GIRL_YUKO) || (game_state.current_girl == GIRL_MEGUMI) || (game_state.current_girl == GIRL_YUMI)) {
            func_80083418();
            return;
        }
        func_800833F0();
        return;
    }
    mode_step_next();
}

void func_80132E8C(void) {
    if (D_80122CDC != 0) {
        D_80143B00 += 1;
        game_state.girls[game_state.current_girl].affection -= 1;
        game_state.girls[game_state.current_girl].tokimeki -= 1;
        game_state.girls[game_state.current_girl].syoushin += 0x14;
    } else if (game_state.current_girl == GIRL_SHIORI) {
        game_state.girls[game_state.current_girl].tokimeki += 1;
    }
    check_para_limit();
    mode_step_next();
}

void func_80132F90(void) {
    if (D_80122CDC == 0) {
        mode_stage_set(3);
        return;
    }
    mode_step_next();
}

void func_80132FCC(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        portrait_read_start(0x571A);
        break;
    case GIRL_MIO:
        portrait_read_start(0x5E22);
        break;
    case GIRL_YUINA:
        portrait_read_start(0x530F);
        break;
    case GIRL_AYAKO:
        portrait_read_start(0x5990);
        break;
    case GIRL_SAKI:
        portrait_read_start(0x67A0);
        break;
    case GIRL_YUKARI:
        portrait_read_start(0x614C);
        break;
    case GIRL_NOZOMI:
        portrait_read_start(0x630E);
        break;
    case GIRL_MIRA:
        portrait_read_start(0x5BD9);
        break;
    case GIRL_YUKO:
        portrait_read_start(0x50C6);
        break;
    case GIRL_MEGUMI:
        portrait_read_start(0x6584);
        break;
    case GIRL_YUMI:
        portrait_read_start(0x6A16);
        break;
    }
    mode_step_next();
}
