/* save_and_exit.c: src/ovl/TACO/80142820.c:7: stage 46; body 8: switch (game_state.mode_step) {; 10: shooter_exit_step_save_settings();. Original object 0x80142820-0x801430B0 (config/objects/TACO.txt). */
#define MAIN_API_OVERRIDE_strncpy /* matched as int: the call's $v0 result range (main_api.h: void), T-19009 */
#include "common.h"
#include "ovl/shooter_3d.h"

s32 strncpy();

void shooter_stage_exit(void) {
    switch (game_state.mode_step) {
    case 0:
        shooter_exit_step_save_settings();
        break;
    case 1:
        shooter_exit_step_stop_sound();
        break;
    case 2:
        shooter_exit_step_write_memory_card();
        break;
    case 3:
        shooter_exit_step_reload_main_data_1();
        break;
    case 4:
        shooter_exit_step_reload_main_data_2();
        break;
    case 5:
        shooter_exit_step_reload_main_data_3();
        break;
    case 6:
        shooter_exit_step_load_day_data();
        break;
    case 7:
        shooter_exit_step_return();
        break;
    }
}

/* Settings block at system_data +0x40 (SystemData in main_api.h since T-20900; the loader shooter_load_settings_and_scores in
 * 80133870.c has its own view of it, T-16562): byte 0 holds the "saved" flag, the mode and the selector, byte 1 two
 * more selectors. Every access relocates against the one symbol system_data: as1 then hoists the flag byte load over
 * the word stores, as the original does (separate symbols D_800E7D50/D_800E7D5C keep it after them). T-19009 */

void shooter_exit_step_save_settings(void) {
    system_data.bits.flag = 1;
    system_data.w0 = shooter_high_score_1;
    system_data.w1 = shooter_high_score_2;
    system_data.w2 = shooter_high_score_3;
    strncpy(system_data.name0, shooter_high_score_name_1, 3);
    strncpy(system_data.name1, shooter_high_score_name_2, 3);
    strncpy(system_data.name2, shooter_high_score_name_3, 3);
    /* the % 2 folds away, but keeps the extra copy of the value that the original has */
    system_data.bits.mode = shooter_setting_auto_shot % 2;
    system_data.bits.sel = shooter_setting_roll_button % 3;
    system_data.bits.hi = shooter_setting_shot_button % 3;
    system_data.bits.lo = shooter_setting_bomb_button % 3;
    sprite_tables_clear();
    draw2d3d(0, 0);
    game_state.draw_origin_x[0] = 0xA0;
    game_state.draw_origin_x[1] = 0xA0;
    func_8009C674(0);
    func_8009C210(3);
    func_80097D90(0x140, 0xF0, 0, 0, 0);
    if (cd_queue_idle() == 0) {
        cd_command_reset();
    }
    view_display_reinit();
    view_model_slots_clear();
    shooter_hide_all_models();
    actor_clear_range(0);
    actor_draw_enable(0);
    mode_step_next();
}

void shooter_exit_step_write_memory_card(void) {
    GameState *gs = &game_state;
    s32 temp;

    switch (gs->substep) {
    case 0:
        if (memory_card_check_poll() != 0) {
            /* FAKE: the first read of unk_1115 goes through a local so the three tests share a
             * register range that takes $v1 (found by tools/autotweak.py); T-19009 */
            temp = gs->memory_card_result;
            if (temp == 1) {
                gs->substep += 1;
            } else if (gs->memory_card_result == 0xF1) {
                gs->substep += 1;
            } else if (gs->memory_card_result == 0xFF) {
                gs->substep += 1;
            }
        } else if ((u32)gs->memory_card_timer >= 0x201U) {
            gs->substep += 1;
        }
        break;
    case 1:
        memory_card_stop();
        if (gs->memory_card_result == 1) {
            if (memory_card_file_exists(0) == 1) {
                gs->substep += 1;
                memory_card_start();
                settings_save_load_layout_stash(0);
            } else {
                gs->substep = 0xFF;
            }
        } else {
            gs->substep = 0xFF;
        }
        break;
    case 2:
        if (memory_card_system_read_poll() == 3) {
            memory_card_stop();
            gs->substep += 1;
        } else if (((u32)gs->memory_card_timer >= 0x201U) || (gs->memory_card_result == 0xF0)) {
            memory_card_stop();
            gs->substep = 0xFF;
        }
        break;
    case 3:
        if (memory_card_system_bank_load(0) == -1) {
            settings_save_load_layout_stash(1);
            gs->substep = 0xFF;
        } else {
            settings_save_load_layout_stash(1);
            gs->substep += 1;
        }
        break;
    case 4:
        memory_card_start();
        D_800E7D36 = 0;
        gs->substep += 1;
        break;
    case 5:
        if (memory_card_system_write_poll() == 0x13) {
            audio_command(0x50F);
            memory_card_stop();
            gs->substep += 1;
        } else if (((u32)gs->memory_card_timer >= 0x201U) || (gs->memory_card_result == 0xF0)) {
            audio_command(0x502);
            memory_card_stop();
            gs->substep += 1;
        }
        break;
    case 0xFF:
    default:
        mode_step_next();
        break;
    }
}

typedef struct {
    u32 pad0 : 1;
    u32 flag : 1;
    u32 pad1 : 30;
} ShooterUnlockBits;

void shooter_exit_step_return(void) {
    text_system_reset();
    dec_bg_reset();
    draw2d3d(1, 0);
    actor_draw_enable(1);
    *(s32 *) &game_state.menu_hit[3] = 0x9F;
    if (((ShooterUnlockBits *) &D_800E7D50)->flag == 1) {
        actor_clear_range(0);
        tpage_buf_clear();
        game_mode_set(game_state.saved_game_mode);
        mode_stage_set(game_state.saved_mode_stage);
        mode_step_set(game_state.saved_mode_step);
        hud_cursor_actor_init(0);
        hud_text_marker_set(0);
        message_window_init();
        hizuke_init();
        message_disp_switch(1);
        hizuke_disp_switch(1);
        return;
    }
    game_mode_set(GAME_MODE_TITLE);
}

void shooter_exit_step_reload_main_data_1(void) {
    boot_data_load_step();
}

void shooter_exit_step_reload_main_data_2(void) {
    boot_hud_textures_load();
    mode_step_next();
}

void shooter_exit_step_reload_main_data_3(void) {
    debug_font_load();
    mode_step_next();
}

u8 shooter_exit_step_load_day_data(void) {
    switch (game_state.substep) {
    case 0:
        cd_read_sectors(0x95, 0x801A0000, 0x9B94);
        game_state.substep += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            calendar_month_load(game_state.year, game_state.month, 0);
            game_state.substep += 1;
        }
        break;
    case 2:
        mode_step_next();
        break;
    }
}

u8 shooter_exit_step_stop_sound(void) {
    switch (game_state.substep) {
    case 0:
        audio_bank_state_reset();
        hud_day_bgm_load();
        game_state.substep += 1;
        break;
    case 1:
        if (audio_bank_load_poll() == 1) {
            game_state.substep += 1;
        }
        break;
    case 2:
        if (debug_hud_enabled == 0) {
            memory_card_start();
        } else {
            mode_step_next();
        }
        mode_step_next();
        break;
    }
}
