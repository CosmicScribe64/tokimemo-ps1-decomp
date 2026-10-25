/* day_dispatch_and_script_tables.c: src/ovl/KANGEI/80132E40.c:4-45; binds common/per-girl script pointer rows, then camp_mode_stage_dispatch dispatches the day-coded mode_stage and performs scene presentation. Original object 0x80132E40-0x80133C10 (config/objects/KANGEI.txt). */
#include "common.h"
#include "ovl/summer_camp.h"

void camp_background_pointer_setup(void) {
    D_80139A50[0] = 0x801C1400;
    D_80139A50[1] = 0x801C1D6C;
    D_80139A50[2] = 0x801CA47C;
}

void camp_day_phase_dialogue_pointer_table_setup(void) {
    D_80139A5C[0][0] = 0x80199CD4;
    D_80139A5C[0][1] = 0x8019EE30;
    D_80139A5C[0][2] = 0x801A422C;
    D_80139A5C[0][3] = 0x801A82AC;
    D_80139A5C[0][4] = 0x801AB394;
    D_80139A5C[0][5] = 0x801AF5FC;
    D_80139A5C[0][6] = 0x801B1C9C;
    D_80139A5C[0][7] = 0x801B3370;
    D_80139A5C[1][0] = 0x80199EF4;
    D_80139A5C[1][1] = 0x8019F078;
    D_80139A5C[1][2] = 0x801A447C;
    D_80139A5C[1][3] = 0x801A8408;
    D_80139A5C[1][4] = 0x801AB4E0;
    D_80139A5C[1][5] = 0x801AF824;
    D_80139A5C[1][6] = 0x801B1D10;
    D_80139A5C[1][7] = 0x801B3478;
    D_80139A5C[2][0] = 0x8019BC24;
    D_80139A5C[2][1] = 0x801A0FD8;
    D_80139A5C[2][2] = 0x801A63F8;
    D_80139A5C[2][3] = 0x801A96B8;
    D_80139A5C[2][4] = 0x801AC800;
    D_80139A5C[2][5] = 0x801B17AC;
    D_80139A5C[2][6] = 0x801B2090;
    D_80139A5C[2][7] = 0x801B41F4;
}

/* Day dispatcher of the overlay.
 * FAKE: T-24301, D_80139AC0 (s8) is read through a u8 view; the view keeps it promoted in $v1 as in the
 * original (the plain field or a (u8) cast gives temporaries, 88 rows). */
void camp_mode_stage_dispatch(void) {
    u8 day;

    portrait_wink_update();
    day = game_state.mode_stage;
    if (!((day == 2 || day == 5 || day == 9 || day == 0xA || day == 0xC || day == 0xF || day == 0x11 || day == 0x14 ||
           day == 0x16 || day == 0x19 || day == 0x1D || day == 0x1F) &&
          game_state.mode_step != 0 && *(u8 *)&D_80139AC0 == 0) &&
        !(day == 0x1E && *(u8 *)&D_80139AC0 == 0)) {
        hud_icon_bar_update(1);
        day = game_state.mode_stage;
    }
    switch (day) {
    case 0:
        camp_entry_state_initialize();
        break;
    case 1:
        camp_audio_load_poll_retry();
        break;
    case 2:
        camp_mode_step_scene_phase_callback_dispatch();
        break;
    case 3:
        vacation_day_init();
        break;
    case 4:
        camp_mode_step_timer_update();
        break;
    case 5:
        camp_birthday_scene_gate();
        break;
    case 6:
        camp_calendar_day_advance();
        break;
    case 7:
        vacation_day_init();
        break;
    case 8:
        camp_mode_step_timer_update();
        break;
    case 9:
        camp_current_girl_scene_gate();
        break;
    case 10:
        camp_birthday_scene_gate();
        break;
    case 11:
        camp_calendar_day_advance();
        break;
    case 12:
        camp_paired_actor_step_dispatch();
        break;
    case 13:
        vacation_day_init();
        break;
    case 14:
        camp_mode_step_timer_update();
        break;
    case 15:
        camp_birthday_scene_gate();
        break;
    case 16:
        camp_calendar_day_advance();
        break;
    case 17:
        camp_paired_actor_step_dispatch();
        break;
    case 18:
        vacation_day_init();
        break;
    case 19:
        camp_mode_step_timer_update();
        break;
    case 20:
        camp_birthday_scene_gate();
        break;
    case 21:
        camp_calendar_day_advance();
        break;
    case 22:
        camp_paired_actor_step_dispatch();
        break;
    case 23:
        vacation_day_init();
        break;
    case 24:
        camp_mode_step_timer_update();
        break;
    case 25:
        camp_birthday_scene_gate();
        break;
    case 26:
        camp_calendar_day_advance();
        break;
    case 27:
        vacation_day_init();
        break;
    case 28:
        camp_mode_step_timer_update();
        break;
    case 29:
        camp_scene_result_callback_dispatch();
        break;
    case 30:
        camp_birthday_scene_gate();
        break;
    case 31:
        camp_scene_picture_callback_dispatch();
        break;
    case 32:
        camp_day_event_complete();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    day = game_state.mode_stage;
    if ((day == 3) || (day == 7) || (day == 0xD) || (day == 0x12) || (day == 0x17) || (day == 0x1B)) {
        hud_icon_bar_update(1);
    }
    parameter_show();
    check_k_scroll();
    k_disp_inc2();
    message_window_title_update();
    message_window_show();
    hizuke_show();
    draw_shake_update();
    day = game_state.mode_stage;
    if (((day == 2) || (day == 5) || (day == 9) || (day == 0xA) || (day == 0xC) || (day == 0xF) || (day == 0x11) || (day == 0x14) || (day == 0x16) || (day == 0x19) || (day == 0x1D) || (day == 0x1F)) && (game_state.mode_step != 0) && (*(u8 *)&D_80139AC0 == 0)) {
        hud_screen_frame_draw(2);
        scene_frame_update();
    } else if ((day == 0x1E) && (*(u8 *)&D_80139AC0 == 0)) {
        hud_screen_frame_draw(2);
        scene_frame_update();
    } else if (*(u8 *)&D_80139AC0 == 1) {
        dec_bg_draw(0);
        if ((D_80122D2C == 0) && (D_80122D04 == 0) && ((game_state.pad_held & 0xF000F000) || (talk_menu_active == 1))) {
            ((Actor *)actor_pool)->draw_flags |= 0x80;
        }
        if (((Actor *)actor_pool)->draw_flags & 0x80) {
            hud_cursor_update();
        }
        hud_screen_frame_draw(1);
    } else {
        dec_bg_draw(0);
        hud_screen_frame_draw(1);
        if ((D_80122D2C == 0) && (D_80122D04 == 0) && ((game_state.pad_held & 0xF000F000) || (talk_menu_active == 1))) {
            ((Actor *)actor_pool)->draw_flags |= 0x80;
        }
        if (D_80122D04 != 0) {
            ((Actor *)actor_pool)->draw_flags &= 0x7F;
        }
        if (((Actor *)actor_pool)->draw_flags & 0x80) {
            hud_cursor_update();
        }
    }
    draw_shake_update();
}

void camp_calendar_day_advance(void) {
    game_state.day_event_index += 1;
    camp_shiori_birthday_seen = 0;
    D_80139AC8 = 0;
    calendar_advance_day();
    hizuke_init();
    mode_stage_next();
}

void camp_entry_state_initialize(void) {
    s16 var_s0;

    cd_read_checked = 0;
    audio_bank_load_start(1, 0xC06F, 0xC065, 0xCC33, 0xCBE3, 0xCBD9);
    if (audio_bank_load_poll() == 1) {
        mode_stage_next();
    }
    D_80139ABC = 0;
    D_80139AC0 = 0;
    camp_shiori_birthday_seen = 0;
    camp_scene_candidate_initialize();
    game_state.day -= 1;
    hizuke_init();
    game_state.day += 1;
    hud_icon_bar_init();
    for (var_s0 = 0; var_s0 < 15; var_s0++) {
        icon_can_use_set(var_s0, 0);
    }
    hud_icon_bar_update(1);
    parameter_show_init();
    parameter_show_init();
    /* FAKE: dead byte test keeps the packed-record address in v0 (T-9580). */
    if (game_state.screen_brightness == 0) {}
    ((KangeiStateView *)&game_state)->modes[(u16)game_state.player_flags.h >> 12].mode++;
    bg_brightness = 0x80;
    mode_stage_next();
}

void camp_audio_bank_retry(void) {
    cd_read_checked = 0;
    audio_bank_load_start(1, 0xC06F, 0xC065, 0xCC33, 0xCBE3, 0xCBD9);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    audio_command(0x22);
    audio_command(0x21);
    audio_command(0x2F);
    mode_step_next();
}

void camp_audio_load_poll_retry(void) {
    if (audio_bank_load_poll() == 1) {
        mode_stage_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void camp_audio_load_poll_retry_alt(void) {
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void camp_audio_bank_start(void) {
    if (game_state.mode_step_timer.w == 0) {
        audio_bank_start(0, 0);
    }
    mode_step_next();
}

s32 camp_day_event_complete(void) {
    s32 pad; /* FAKE: unused local puts rect at the original offset; real source unknown. T-9320 */
    RECT rect;

    if (*(u8 *)&game_state.substep == 0) {
        hud_day_bgm_load();
        *(u8 *)&game_state.substep += 1;
    }
    if (*(u8 *)&game_state.substep == 1) {
        if (game_state.mode_step_timer.u++ >= 0x400U) {
            cd_command_reset();
            audio_bank_state_reset();
            *(u8 *)&game_state.substep = 0;
        }
        if (audio_bank_load_poll() == 1) {
            *(u8 *)&game_state.substep += 1;
        } else {
            return 0;
        }
    }
    dec_bg_reset();
    rect.x = 0x140;
    rect.y = 0x100;
    rect.w = 0x140;
    rect.h = 0xF8;
    func_8009C7F8(&rect, 8, 0, 0);
    rect.x = 0x2C0;
    rect.y = 0;
    func_8009C7F8(&rect, 8, 0, 0);
    calendar_advance_day();
    return day_event_next(1);
}

void camp_mode_step_timer_update(void) {
    game_state.mode_step_timer.w += 1;
    if (game_state.mode_step == 0) {
        camp_day_event_parameter_update();
    }
}

/* Non-void return type and pointer local `state = &D_80120666` match week_day_main0
 * (src/main/school_day.c) and ETC:exam_day_wait_anim (T-6050, T-24122). */
s32 camp_day_event_parameter_update(void) {
    s16 *state;
    u8 a;
    u8 b;
    u32 club;

    if (!(D_80120652 & 1)) {
        if (D_80120658 == 0) {
            state = &D_80120666;
            switch ((u32)(*state & 1)) {
            case 0:
                audio_command(0x601);
                break;
            case 1:
                audio_command(0x602);
                break;
            }
            a = game_state.day_events[game_state.day_event_index].girl;
            b = game_state.day_events[game_state.day_event_index].scene;
            game_state.clubs[(u16)game_state.player_flags.h >> 12].practice_points += 2;
            club = (u16)game_state.player_flags.h >> 12;
            if (club < 2U) {
                ((KangeiStateView *)&game_state)->param_liberal_arts_word += 0x18000;
            } else if (club < 4U) {
                ((KangeiStateView *)&game_state)->param_science_word += 0x18000;
            } else if (club < 6U) {
                ((KangeiStateView *)&game_state)->param_art_word += 0x18000;
            } else {
                ((KangeiStateView *)&game_state)->param_athletics_word += 0x18000;
                ((KangeiStateView *)&game_state)->param_guts_word += 0x18000;
            }
            parameter_up_down(a, 0, b);
            parameter_show_init();
        }
        if (D_80120658++ >= 5) {
            mode_stage_next();
        }
    }
}
