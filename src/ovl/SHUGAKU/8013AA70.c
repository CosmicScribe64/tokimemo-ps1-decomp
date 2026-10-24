#include "common.h"
#include "ovl/SHUGAKU.h"

void func_8013AA70(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    ShFnTbl25 tbl;

    tbl = D_8013CC20;
    idx = game_state.step;
    tbl.f[idx](0x80);
}

void func_8013AAEC(void) {
    s32 temp;
    temp = D_800CA2CC;
    switch (temp) {
    case 0:
        strcpy(D_800CA23C, "京都");
        break;
    case 1:
        strcpy(D_800CA23C, "沖縄");
        break;
    case 2:
        strcpy(D_800CA23C, "北海道");
        break;
    }
    D_800CA2DC += D_800CA2F4 * (1 - D_800CA2EC);
    main_step_next();
}

s32 func_8013ABAC(void) {
    if (game_state.substep == 0) {
        dec_bg_reset();
        func_800674B0();
        game_state.substep += 1;
    }
    if (game_state.substep == 1) {
        if ((u32)game_state.step_timer.w++ >= 0x400U) {
            cd_cmd_reset();
            snd_bank_state_reset();
            game_state.substep = 0;
        }
        if (snd_bank_load_poll() == 1) {
            game_state.substep += 1;
        } else {
            return 0;
        }
    }
    calendar_advance_day();
    if (D_800CA2EC != 0) {
        game_state.girls[10].affection += 1;
    } else {
        game_state.girls[game_state.current_girl].tokimeki += 1;
        game_state.girls[game_state.current_girl].affection += 1;
    }
    check_para_limit();
    return day_event_next(1);
}
