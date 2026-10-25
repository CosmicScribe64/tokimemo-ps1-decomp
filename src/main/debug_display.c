/* debug_display.c: src/main/80059A20.c; Displays mode/stage/step and associated timers, timer_ticks, substep and GetSp with debug-number drawing; second helper prints diagnostic label D_800AFDF0 and marks a work-queue flag. Original object 0x80059A20-0x80059B40 (config/objects/main.txt). */
#include "common.h"
#include "game.h"

void debug_state_numbers_draw(void) {
    debug_print_hex(game_state.mode_step, 0x90, 0x58);
    draw_debug_number(game_state.mode_step_timer.w, 0x80, 0x58);
    debug_print_hex(game_state.mode_stage, 0x90, 0x50);
    draw_debug_number(game_state.mode_stage_timer, 0x80, 0x50);
    debug_print_hex(game_state.game_mode, 0x90, 0x48);
    draw_debug_number(game_state.game_mode_timer, 0x80, 0x48);
    debug_print_hex(game_state.timer_ticks, 0x80, 0x60);
    debug_print_hex(game_state.substep, 0x90, 0x60);
    debug_print_hex(D_800E7D34, 0x90, 0x40);
    debug_print_hex(GetSp(), 0x90, 0x68);
}

INCLUDE_RODATA("asm/data/main/debug_display.rodata", D_800AFDF0);

void audio_debug_load_error_report(s32 arg0) {
    printf(D_800AFDF0, arg0);
    D_800B3D54[D_800B3D24] = 1;
}
