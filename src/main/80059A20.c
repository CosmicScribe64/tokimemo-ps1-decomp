#include "common.h"
#include "game.h"

void func_80059A20(void) {
    func_8004B19C(game_state.step, 0x90, 0x58);
    debug_number_show(game_state.step_timer.w, 0x80, 0x58);
    func_8004B19C(game_state.stage, 0x90, 0x50);
    debug_number_show(game_state.stage_timer, 0x80, 0x50);
    func_8004B19C(game_state.mode, 0x90, 0x48);
    debug_number_show(game_state.mode_timer, 0x80, 0x48);
    func_8004B19C(game_state.timer_ticks, 0x80, 0x60);
    func_8004B19C(game_state.substep, 0x90, 0x60);
    func_8004B19C(D_800E7D34, 0x90, 0x40);
    func_8004B19C(GetSp(), 0x90, 0x68);
}

INCLUDE_RODATA("asm/data/main/80059A20.rodata", D_800AFDF0);

void func_80059B04(s32 arg0) {
    printf(D_800AFDF0, arg0);
    D_800B3D54[D_800B3D24] = 1;
}
