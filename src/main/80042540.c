#include "common.h"
#include "game.h"

/* Byte views of both selectors: the original reads them with lbu (unk_111A is
 * s8 in GameState), and a plain unk_1108 switch keeps a copy in v1 (the
 * switch-selector idiom of T-10860). */
void game_mode_dispatch(void) {
    switch (*(u8 *)&game_state.game_mode) {
    case 0x0:
        boot_mode_handler();
        break;
    case 0x1:
        if (*(u8 *)&game_state.boot_hold == 0) {
            game_mode_set(GAME_MODE_BOOT);
        }
        break;
    case 0xFF:
        exit_function();
        break;
    case 0xFE:
        func_8004F6B0();
        break;
    case 0x30:
        schedule_init();
        break;
    case 0x31:
        schedule_day_screen_handler();
        break;
    case 0x34:
        schedule_command_screen_handler();
        break;
    case 0x12:
        title_mode_handler();
        break;
    case 0x40:
        week_day();
        break;
    case 0x46:
        holiday_mode_handler();
        break;
    case 0x44:
        uwasa_main();
        break;
    case 0x41:
        func_80061688();
        break;
    case 0x42:
        func_80060DF8();
        break;
    case 0x3:
        ((s32 (*)()) ovl_entry)();
        break;
    case 0x10:
        func_80062C28();
        break;
    case 0x38:
        schedule_month_start_mode_handler();
        break;
    case 0x39:
        schedule_day_notice_mode_handler();
        break;
    case 0x32:
        schedule_week_end_mode_handler();
        break;
    case 0x2:
        ovl_loader_mode_handler();
        break;
    case 0xD0:
        func_8007959C();
        break;
    case 0x11:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x35:
    case 0x36:
    case 0x37:
    case 0x3A:
    case 0x43:
    case 0x45:
    case 0x50:
    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57:
    case 0x58:
    case 0x59:
    case 0x5A:
    case 0x5B:
    case 0x5C:
    case 0x5D:
    case 0x5E:
    case 0x5F:
    case 0x60:
    case 0x61:
    case 0x62:
    case 0x63:
    case 0x70:
    case 0x71:
    case 0x72:
    case 0x73:
    case 0x74:
    case 0x75:
    case 0x76:
    case 0x80:
    case 0x81:
    case 0x82:
    case 0x90:
    case 0x91:
    case 0xC2:
    case 0xC3:
    case 0xC4:
        func_8004B338((s32)"LOAD FUNC EXEC", 0x14, 0x2C);
        ((s32 (*)()) ovl_entry)();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    game_state.game_mode_timer += 1;
}

void game_mode_next(void) {
    game_state.game_mode += 1;
    game_state.mode_stage = 0;
    game_state.mode_step = 0;
    game_state.game_mode_timer = 0;
    game_state.mode_stage_timer = 0;
    game_state.mode_step_timer.w = 0;
    game_state.game_mode_scratch = 0;
    game_state.mode_stage_scratch = 0;
    game_state.substep = 0;
    game_mode_request_overlay();
}

/* FAKE: z is read through its address, so uopt sets v0 = 0 before the last
 * store and as1 puts that store in the jr delay slot, as the original does; no
 * `return 0` form does (the normal_date_girl_in_init trick, T-9540). s32 with no
 * return. T-11610 */
s32 mode_stage_next(void) {
    s32 *p;
    s32 z;

    game_state.mode_stage += 1;
    game_state.mode_step = 0;
    game_state.mode_stage_timer = 0;
    game_state.mode_step_timer.w = 0;
    game_state.mode_stage_scratch = 0;
    z = 0;
    p = &z;
    game_state.substep = *p;
}

/* FAKE: z is read through its address, so uopt sets v0 = 0 before the last
 * store and as1 puts that store in the jr delay slot, as the original does; no
 * `return 0` form does (the normal_date_girl_in_init trick, T-9540). s32 with no
 * return. T-11610 */
s32 mode_step_next(void) {
    s32 *p;
    s32 z;

    game_state.mode_step += 1;
    game_state.mode_step_timer.w = 0;
    z = 0;
    p = &z;
    game_state.substep = *p;
}

/* Switch to a mode and reset stage, step, timers, substep */
void game_mode_set(s32 arg0) {
    u32 t;

    game_state.game_mode = arg0;
    game_state.mode_stage = 0;
    game_state.mode_step = 0;
    game_state.game_mode_timer = 0;
    game_state.mode_stage_timer = 0;
    game_state.mode_step_timer.w = 0;
    game_state.game_mode_scratch = 0;
    game_state.mode_stage_scratch = 0;
    game_state.substep = 0;
    game_mode_request_overlay();
    t = game_state.ovl_target_game_mode;
    D_800CA2AC = 0;
    if (t != 0x5E) {
        if (t != 0x50) {
            message_window_title_show(0);
        }
    }
}

/* FAKE: z is read through its address, so uopt sets v0 = 0 before the last
 * store and as1 puts that store in the jr delay slot, as the original does; no
 * `return 0` form does (the normal_date_girl_in_init trick, T-9540). s32 with no
 * return. T-11610 */
s32 mode_stage_set(s32 arg0) {
    s32 *p;
    s32 z;

    game_state.mode_stage = arg0;
    game_state.mode_step = 0;
    game_state.mode_stage_timer = 0;
    game_state.mode_step_timer.w = 0;
    game_state.mode_stage_scratch = 0;
    z = 0;
    p = &z;
    game_state.substep = *p;
}

/* FAKE: z is read through its address, so uopt sets v0 = 0 before the last
 * store and as1 puts that store in the jr delay slot, as the original does; no
 * `return 0` form does (the normal_date_girl_in_init trick, T-9540). s32 with no
 * return. T-11610 */
s32 mode_step_set(s32 arg0) {
    s32 *p;
    s32 z;

    game_state.mode_step = arg0;
    game_state.mode_step_timer.w = 0;
    z = 0;
    p = &z;
    game_state.substep = *p;
}

s32 game_mode_request_overlay(void) {
    switch (*(u8 *)&game_state.game_mode) {
    case 0x11:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x35:
    case 0x36:
    case 0x37:
    case 0x3A:
    case 0x43:
    case 0x45:
    case 0x50:
    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57:
    case 0x58:
    case 0x59:
    case 0x5A:
    case 0x5B:
    case 0x5C:
    case 0x5D:
    case 0x5E:
    case 0x5F:
    case 0x60:
    case 0x61:
    case 0x62:
    case 0x63:
    case 0x70:
    case 0x71:
    case 0x72:
    case 0x73:
    case 0x74:
    case 0x75:
    case 0x76:
    case 0x80:
    case 0x81:
    case 0x82:
    case 0x90:
    case 0x91:
    case 0xC2:
    case 0xC3:
    case 0xC4:
        game_state.ovl_target_game_mode = game_state.game_mode;
        game_state.game_mode = 2;
        return;
    }
}
