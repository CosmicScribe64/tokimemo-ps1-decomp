/* result_screen.c: src/ovl/TACO/80138890.c:58 computes capped total from base score, timer, bomb stock and shield; :111 animates bonus and total display; :212 routes to name entry or rankings. Original object 0x80138890-0x80139170 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

s32 shooter_stage_result(void) {
    game_state.mode_stage_timer += 1;
    switch (game_state.mode_step) {
    case 0:
        shooter_result_step_compute();
        break;
    case 1:
        shooter_result_step_show();
        break;
    case 2:
        shooter_result_step_next();
        break;
    }
    if (game_state.mode_step < 2) {
        /* u32 view of unk_1100: the compare and the subtraction then share one load, as in the original */
        if (*(u32 *)&game_state.mode_stage_timer < 0x32U) {
            return shooter_draw_screen_wipe(0x64 - *(u32 *)&game_state.mode_stage_timer);
        }
        return shooter_draw_screen_wipe(0x32);
    }
}

/* No value is returned from the reset arm on purpose: v0 then holds the target (promoted shooter_count_target) as in the original; the other arm returns shooter_count_current. T-16564 */
s32 shooter_count_up_number(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg3 == 0) {
        shooter_count_target = arg0;
        shooter_count_current = 0;
    } else {
        if ((shooter_count_current < shooter_count_target) && !(game_state.frame_count & 1)) {
            if (shooter_count_current < (shooter_count_target % 10)) {
                shooter_count_current += 1;
            } else if (shooter_count_current < (shooter_count_target % 100)) {
                shooter_count_current += 10;
            } else if (shooter_count_current < (shooter_count_target % 1000)) {
                shooter_count_current += 100;
            } else if (shooter_count_current < (shooter_count_target % 10000)) {
                shooter_count_current += 1000;
            } else if (shooter_count_current < (shooter_count_target % 100000)) {
                shooter_count_current += 10000;
            } else if (shooter_count_current < (shooter_count_target % 1000000)) {
                shooter_count_current += 100000;
            } else if (shooter_count_current < (shooter_count_target % 10000000)) {
                shooter_count_current += 1000000;
            }
        }
        if (shooter_count_target < shooter_count_current) {
            shooter_count_current = shooter_count_target;
        }
        shooter_draw_number(shooter_count_current, arg1, arg2, 0);
        return shooter_count_current;
    }
}

/* FAKE: T-25152, the unused K&R parameter holds the stage and the total: a parameter live in the entry block cannot take $v0/$v1 (uopt cupcosts), so it gets $a0 as in the original. The original has no such parameter; real source unknown. */
s32 shooter_result_step_compute(arg0)
s32 arg0;
{
    s32 var_a1;
    s32 var_v0;
    s32 var_v1;
    u8 temp_v0;

    game_state.clear_color_g = 0;
    game_state.clear_color_r = 0;
    /* FAKE: T-25152, byte view of the stage: the indirect load stops copy propagation, so arg0 stays a variable instead of the promoted global */
    arg0 = *(u8 *)&shooter_stages_cleared;
    shooter_result_values[0] = shooter_score += arg0 * 0xF4240;
    game_state.clear_color_b = 0;
    var_a1 = 0;
    if (arg0 >= 3U) {
        temp_v0 = shooter_actors->stats[1];
        if (temp_v0 == 0xFF) {
            var_v1 = 0x7A120;
        } else {
            var_v1 = temp_v0 * 0x3E8;
        }
        var_a1 = shooter_time_left * 0x64;
    } else {
        var_v1 = shooter_actors->stats[1] * 0x64;
    }
    if (shooter_actors->argument_z != 0) {
        var_v0 = 0x7A120;
    } else if (shooter_actors->argument_y != 0) {
        var_v0 = 0x3D090;
    } else if (shooter_actors->argument_x != 0) {
        var_v0 = 0x186A0;
    } else {
        var_v0 = 0;
    }
    /* FAKE: T-25152, word view of the score for the same reason: the total stays in arg0 ($a0) and is stored in place, before the time bonus */
    arg0 = *(s32 *)&shooter_score + var_v1 + var_v0 + var_a1;
    shooter_result_values[2] = var_v0;
    shooter_result_values[1] = var_v1;
    shooter_result_values[4] = arg0;
    shooter_result_values[3] = var_a1;
    if (arg0 >= 0x989680) {
        /* FAKE: T-25152, the zero term holds an indirect load, so uopt keeps this as a def of arg0 ($a0) and the store reads it; a plain constant is copied into the store and built in $t2. ugen folds the term away */
        arg0 = 0x98967F + *(u8 *)&shooter_stages_cleared * 0;
        shooter_result_values[4] = arg0;
    }
    audio_command(0x24);
    func_80059048();
    shooter_release_all_actor_models();
    mode_step_next();
}

/* No return value on purpose: implicit-int function (v0 of the last call falls through). T-16564 */
s32 shooter_result_step_show(void) {
    switch (game_state.substep) {
    case 8:
    case 9:
    default:
        shooter_draw_text("TOTAL", -0xE4, 0x48, 1);
    case 6:
    case 7:
        shooter_draw_text("SHIELD BONUS", -0xE4, 0x18, 5);
    case 4:
    case 5:
        shooter_draw_text("BOMB   BONUS", -0xE4, -8, 2);
    case 2:
    case 3:
        shooter_draw_text("TIME   BONUS", -0xE4, -0x28, 3);
        break;
    case 0:
    case 1:
        break;
    }
    shooter_draw_text("SCORE", -0xE4, -0x48, 6);
    switch (game_state.substep) {
    case 10:
    default:
        shooter_draw_number(shooter_result_values[4], -0x40, 0x58, 0);
    case 8:
    case 9:
        shooter_draw_number(shooter_result_values[1], -0x40, 0x28, 0);
    case 6:
    case 7:
        shooter_draw_number(shooter_result_values[2], -0x40, 8, 0);
    case 4:
    case 5:
        shooter_draw_number(shooter_result_values[3], -0x40, -0x18, 0);
    case 2:
    case 3:
        shooter_draw_number(shooter_result_values[0], -0x40, -0x38, 0);
        break;
    case 0:
    case 1:
        break;
    }
    /* FAKE: u8 view of the selector keeps it a plain ugen temporary for the third switch; with the field it is promoted to a register (the 0xA test and the stores below). T-16564 */
    switch (*(u8 *)&game_state.substep) {
    case 0:
        shooter_count_up_number(shooter_result_values[0], -0x40, -0x38, 0);
        game_state.substep += 1;
        break;
    case 1:
        if (shooter_count_up_number(shooter_result_values[0], -0x40, -0x38, 1) == shooter_result_values[0]) {
            game_state.substep += 1;
        }
        break;
    case 2:
        shooter_count_up_number(shooter_result_values[3], -0x40, -0x18, 0);
        game_state.substep += 1;
        break;
    case 3:
        if (shooter_count_up_number(shooter_result_values[3], -0x40, -0x18, 1) == shooter_result_values[3]) {
            game_state.substep += 1;
        }
        break;
    case 4:
        shooter_count_up_number(shooter_result_values[2], -0x40, 8, 0);
        game_state.substep += 1;
        break;
    case 5:
        if (shooter_count_up_number(shooter_result_values[2], -0x40, 8, 1) == shooter_result_values[2]) {
            game_state.substep += 1;
        }
        break;
    case 6:
        shooter_count_up_number(shooter_result_values[1], -0x40, 0x28, 0);
        game_state.substep += 1;
        break;
    case 7:
        if (shooter_count_up_number(shooter_result_values[1], -0x40, 0x28, 1) == shooter_result_values[1]) {
            game_state.substep += 1;
        }
        break;
    case 8:
        shooter_count_up_number(shooter_result_values[4], -0x40, 0x58, 0);
        game_state.substep += 1;
        break;
    case 9:
        if (shooter_count_up_number(shooter_result_values[4], -0x40, 0x58, 1) == shooter_result_values[4]) {
            game_state.substep += 1;
        }
        break;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if ((u8) game_state.substep < 0xAU) {
            game_state.substep = 0xA;
        } else {
            shooter_score = shooter_result_values[4];
            func_80059308();
            mode_step_next();
        }
    }
}

void shooter_result_step_next(void) {
    func_800591D8(0);
    if (shooter_high_score_3 < shooter_score) {
        mode_stage_set(9);
        return;
    }
    mode_stage_set(5);
}
