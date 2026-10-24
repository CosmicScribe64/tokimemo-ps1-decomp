#include "common.h"
#include "ovl/SHOUGATU.h"

void func_80133D10(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_80143CC0;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

/* FAKE: the (u16) cast on the first store and `var_v0 = var_v0;` keep the redundant second store and the original's temporary numbering (t1); found with the permuter, T-9380 */
void func_80133D98(void) {
    s16 var_v0;

    strcpy(D_800CA19C, "初詣");
    var_v0 = (D_80143B20 != 0) ? 0 : 0xF;
    if (D_80143B18 == 0) {
        var_v0 += 2;
    }
    D_80143B00 = (u16)var_v0;
    if (D_80143B18 == 0) {
        var_v0 = var_v0;
        D_80143B00 = var_v0;
        if (game_state.current_girl == GIRL_YUKARI) {
            var_v0 = 2 + var_v0;
            D_80143B00 = var_v0;
        }
    }
    mode_step_next();
}

void func_80133E30(void) {
    if (D_80143B20 != 0) {
        func_801323D4();
        return;
    }
    mode_step_next();
}

void func_80133E6C(void) {
    if (D_80122CDC != 0) {
        func_800833F0();
        return;
    }
    func_800833A0();
}

void func_80133EA8(void) {
    if ((D_80143B20 == 0) && (((u32) game_state.frame_count % 3U) == 0)) {
        if (D_80143B24 != 0) {
            event_log_add(5, 3);
        } else {
            event_log_add(5, 0);
        }
    }
    bg_read_sub2(0x41ED);
    mode_step_next();
}

void func_80133F24(void) {
    if ((game_state.current_girl != GIRL_YUKARI) || (D_80143B20 != 0) || (D_80143B18 == 0)) {
        mode_stage_next();
        return;
    }
    mode_step_next();
}

void func_80133F88(void) {
    if (D_80143B20 != 0) {
        game_state.mode_step += 4;
    }
    mode_step_next();
}

void func_80133FC8(void) {
    if (D_80143B18 == 0) {
        mode_step_next();
        return;
    }
    func_801323D4();
}

void func_80134004(void) {
    if ((game_state.current_girl == GIRL_YUKARI) && (D_80143B18 == 0)) {
        func_801323D4();
        return;
    }
    mode_step_next();
}

void func_80134058(void) {
    if (D_80143B18 != 0) {
        func_801323D4();
        return;
    }
    mode_step_next();
}

void func_80134094(void) {
    if (D_80122CDC != 0) {
        game_state.girls[5].affection -= 1;
        game_state.girls[5].tokimeki -= 1;
        game_state.girls[5].syoushin += 0xA;
    } else {
        game_state.girls[5].tokimeki += 1;
    }
    check_para_limit();
    mode_step_next();
}
