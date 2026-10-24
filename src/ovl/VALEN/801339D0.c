#include "ovl/VALEN.h"

typedef struct {
    void (*f[10])();
} FnTbl10; /* size 0x28 */
extern FnTbl10 D_80134630;

void func_801339D0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl10 tbl;

    tbl = D_80134630;
    idx = game_state.step;
    tbl.f[idx](0x80);
}

void func_80133A4C(void) {
    if (strcmp(D_800CA1DC, "自宅前") == 0) {
        main_step_next();
        return;
    }
    strcpy(D_800CA1DC, "自宅前");
    place_init2();
}

s32 func_80133AA8(void) {
    if (game_state.substep == 0) {
        func_800674B0();
        game_state.substep += 1;
    }
    if (game_state.substep == 1) {
        if (game_state.step_timer.u++ >= 0x400U) {
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
    return day_event_next(1);
}

void func_80133B6C(void) {
    if (D_80134544 != 0) {
        game_state.girls[9].flags.f.met = 1;
    }
    if (D_8013453C >= 7) {
        D_80134498 = 0;
    } else if (D_8013453C >= 5) {
        D_80134498 = 1;
    } else if (D_8013453C >= 3) {
        D_80134498 = 2;
    } else if (D_8013453C > 0) {
        D_80134498 = 3;
    } else {
        D_80134498 = 4;
    }
    D_8013448C = D_801343F4;
    D_80134490 = D_80134428;
    D_80134494 = D_8013445C;
    main_step_next();
}

void func_80133C44(void) {
    bg_read_sub2(0x414C);
    main_step_next();
}
