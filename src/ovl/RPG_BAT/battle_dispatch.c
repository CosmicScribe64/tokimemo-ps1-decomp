/* battle_dispatch.c: src/ovl/RPG_BAT/8013B5B0.c:7: battle stage switch, fight loop, pause and pad-sequence handling; body 8: if (game_state.mode_stage >= 6 && game_state.mode_stage < 0xA) {; 9: rpg_pause_update();. Original object 0x8013B5B0-0x8013C5D0 (config/objects/RPG_BAT.txt). */
#define MAIN_API_OVERRIDE_cd_command_reset /* implicit int view: the call's $v0 result moves the promoted state byte unk_1109 to $v1 in rpg_entry (T-20600) */
#include "common.h"
#include "ovl/RPG_BAT.h"

s32 cd_command_reset();

void rpg_entry(void) {
    if (game_state.mode_stage >= 6 && game_state.mode_stage < 0xA) {
        rpg_pause_update();
    }
    if (game_state.mode_stage != 9 && !(game_state.menu_mode & 1)) {
        hud_cursor_update();
    }
    if (game_state.mode_stage >= 5 && game_state.mode_stage < 0xC) {
        prim_gradient_rect(-0xA0, 0x30, 8, 0x48, 2, 0, 1);
        prim_gradient_rect(-0xA0, 0x70, 0x140, 8, 2, 0, 1);
        prim_gradient_rect(0x98, 0x30, 8, 0x48, 2, 0, 1);
    }
    switch (game_state.mode_stage) {
    case 0:
        rpg_stage_init();
        rpg_init_player_state();
        break;
    case 1:
        rpg_stage_load_assets_1();
        break;
    case 2:
        rpg_stage_load_assets_2();
        break;
    case 3:
        rpg_stage_asset_addresses();
        rpg_stage_palettes_load();
        break;
    case 4:
        rpg_stage_sprites_setup();
        rpg_stage_hud_setup();
        rpg_stage_labels_init();
        rpg_hud_numbers_update();
        mode_stage_next();
        break;
    case 5:
        rpg_stage_wave_transition();
        break;
    case 6:
        back_clear_switch(0);
        rpg_hud_numbers_update();
        if (rpg_encounter == 0x80000000) {
            rpg_stage_intro_aliens();
        } else {
            rpg_stage_intro_slide_in();
        }
        break;
    case 7:
        rpg_message_update();
        rpg_stage_start_shout();
        break;
    case 8:
        if (rpg_encounter == 0x80000000) {
            audio_bank_start(1, 0x200);
        }
        mode_stage_next();
        break;
    case 9:
        rpg_stage_fight();
        break;
    case 10:
        audio_command(0xC1);
        mode_stage_next();
        break;
    case 11:
        rpg_background_wave();
        rpg_stage_fade_out();
        break;
    case 12:
        audio_bank_state_reset();
        cd_command_reset();
        mode_stage_next();
        break;
    case 13:
        hud_day_bgm_load();
        mode_stage_next();
        break;
    case 14:
        if ((u32)++game_state.mode_stage_timer > 0x400U) {
            audio_bank_state_reset();
            cd_command_reset();
            game_state.mode_stage = 0xD;
        } else if (audio_bank_load_poll() == 1) {
            mode_stage_next();
        }
        break;
    case 15:
    default:
        rpg_exit();
        break;
    }
    if (!(game_state.menu_mode & 1)) {
        hud_cursor_show(1);
        return;
    }
    hud_cursor_show(0);
}

s32 rpg_stage_fight(void) {
    rpg_background_wave();
    if (rpg_pause_state == 3) {
        rpg_girl_timer += 1;
        rpg_girl_update();
        rpg_message_update();
        return 0;
    } else {
        if ((game_state.pad_pressed & PAD_CROSS) && rpg_pause_debounce == 0) {
            audio_command(0x74);
            if (rpg_message.unk_00 >= 3 && rpg_message.unk_08 >= 3) {
                rpg_message.unk_08 = rpg_message_line_frames + 1;
            }
        }
        if (rpg_enemy_action != 0x08000002 && (rpg_enemy_action & 0x08000000) && rpg_over != 1) {
            if (rpg_menu.unk_00 == 2 || rpg_menu.unk_00 == 3) {
                k_disp_switch(rpg_label_guest, 1);
                k_disp_switch(rpg_label_enemy, 0);
            } else {
                k_disp_switch(rpg_label_guest, 0);
                k_disp_switch(rpg_label_enemy, 1);
            }
        }
        if (!(rpg_player_action & 0xFF0111FF)) {
            rpg_player_ready_timer += 1;
        }
        if ((rpg_enemy_action & 0xFF) == 0 && !(rpg_player_action & 0xFF) && !(rpg_girl_action & 0xFF) && !(rpg_enemy_action & 0x1000)) {
            rpg_enemy_timer += 1;
        }
        if ((rpg_enemy_action & 0xFF) == 0 && !(rpg_player_action & 0xFF) && !(rpg_girl_action & 0xFF) && !(rpg_girl_action & 0x1000) && !(rpg_girl_action & 0x80000000)) {
            rpg_girl_timer += 1;
        }
        rpg_girl_update(rpg_player_action, rpg_girl_action);
        rpg_check_enemy_defeated();
        rpg_player_update();
        rpg_check_enemy_defeated();
        rpg_enemy_update();
        rpg_check_enemy_defeated();
        rpg_start_victory_sequence();
        rpg_hud_numbers_update();
        rpg_message_update();
        if (D_8015EE60 == 1) {
            rpg_popup_bounce_0();
        }
        if (D_8015EE6C == 1) {
            rpg_popup_bounce_1();
        }
        if (D_8015EE78 == 1) {
            rpg_popup_bounce_2();
        }
        if (D_8015EE84 == 1) {
            rpg_popup_bounce_3();
        }
        if (rpg_encounter == 0x80000000) {
            rpg_alien_scene_flicker();
        }
    }
}

void rpg_code_entry_update(void) {
    RpgPadButtonSequence sp58;
    RpgPadButtonSequence sp30;
    s32 ten; /* FAKE: named 10 mentioned first puts the hoisted constant before the counters in the bne operands; real source unknown. T-17107 */

    ten = 10;
    sp58 = rpg_code_sequence_a;
    sp30 = rpg_code_sequence_b;
    rpg_code_entry.pad_now = pad_raw_read();
    if (rpg_code_entry.pad_now != rpg_code_entry.pad_last) {
        rpg_code_entry.pad_last = rpg_code_entry.pad_now;
        if ((rpg_code_entry.pad_now & 0xFFFF) != 0) {
            if (rpg_code_entry.pad_now == sp58.buttons[rpg_code_entry.sequence_a_progress]) {
                rpg_code_entry.sequence_a_progress += 1;
            } else {
                rpg_code_entry.sequence_a_progress = 0;
            }
            if (rpg_code_entry.pad_now == sp30.buttons[rpg_code_entry.sequence_b_progress]) {
                rpg_code_entry.sequence_b_progress += 1;
            } else {
                rpg_code_entry.sequence_b_progress = 0;
            }
            if (ten == rpg_code_entry.sequence_a_progress) {
                rpg_code_entry.sequence_a_progress = 0;
                switch (rpg_encounter) {
                case 0x8000000:
                    rpg_enemy_hit_points = 1;
                    /* fallthrough */
                case 0x10000:
                case 0x20000:
                case 0x40000:
                    if (rpg_player_hit_points >= 4) {
                        rpg_player_hit_points_maximum = 0x1662;
                        rpg_player_hit_points = 0x1662;
                    }
                    break;
                case 0x100000:
                    if (rpg_player_hit_points > 0 && rpg_player_hit_points < 10) {
                        rpg_player_hit_points_maximum = 0x1661;
                        rpg_player_hit_points = 0x1661;
                    }
                    /* fallthrough */
                case 0x80000000:
                    if (rpg_player_hit_points >= 2 && rpg_player_hit_points < 10) {
                        rpg_player_hit_points_maximum = 0x1661;
                        rpg_player_hit_points = 0x1661;
                    }
                    if (rpg_player_hit_points >= 10) {
                        rpg_player_hit_points_maximum += 1;
                        rpg_player_hit_points += 1;
                        if (rpg_player_hit_points_maximum >= 0x2710) {
                            rpg_player_hit_points = 0x270F;
                            rpg_player_hit_points_maximum = 0x270F;
                        }
                    }
                    break;
                }
            }
            if (ten == rpg_code_entry.sequence_b_progress) {
                rpg_code_entry.sequence_b_progress = 0;
                switch (rpg_encounter) {
                case 0x100000:
                    rpg_player_hit_points = 1;
                    break;
                case 0x80000000:
                    rpg_player_hit_points = 2;
                    break;
                case 0x10000:
                case 0x20000:
                case 0x40000:
                case 0x8000000:
                    break;
                }
            }
        }
    }
}

void rpg_pause_update(void) {
    if (rpg_pause_state == 3) {
        rpg_code_entry_update();
    }
    rpg_pause_debounce = rpg_pause_debounce - 1;
    if (rpg_pause_debounce < 0) {
        rpg_pause_debounce = 0;
    }
    while (rpg_pause_state == 1 && rpg_pause_debounce == 0) {
        if (!(pad_raw_read() & 0x800)) {
            while (1) {
                if (pad_raw_read() & 0x800) {
                    rpg_se_play_latched_5xx(0x500, 1, 0);
                    rpg_pause_state = 0;
                    rpg_actor_set_animation(0x1C, 0x33, 1, 0);
                    rpg_pause_debounce = 2;
                    break;
                }
                rpg_code_entry_update();
            }
        }
    }
    if ((game_state.pad_pressed & PAD_START) && rpg_pause_debounce == 0) {
        if (!(rpg_enemy_action & 0xFF) && !(rpg_player_action & 0xFF) && !(rpg_girl_action & 0xFF) && !(rpg_over & 0xFF) &&
            rpg_girl_id == 9 && rpg_pause_state == 0 && game_state.mode_stage == 9) {
            audio_command(0x74);
            if (rpg_message.unk_00 >= 3 && rpg_message.unk_08 >= 3) {
                rpg_message.unk_08 = rpg_message_line_frames - 5;
            }
            rpg_se_play_latched_5xx(0x500, 1, 0);
            rpg_pause_state = 3;
            rpg_code_entry.sequence_a_progress = 0;
            rpg_code_entry.sequence_b_progress = 0;
            rpg_code_entry.pad_last = 0;
            rpg_actor_set_animation(0x1C, 0x33, 1, 1);
            rpg_pause_debounce = 2;
        } else if (rpg_pause_state != 0) {
            rpg_se_play_latched_5xx(0x500, 1, 0);
            rpg_pause_state = 0;
            rpg_actor_set_animation(0x1C, 0x33, 1, 0);
            rpg_pause_debounce = 2;
        } else {
            audio_command(0x74);
            if (rpg_message.unk_00 >= 3 && rpg_message.unk_08 >= 3) {
                rpg_message.unk_08 = rpg_message_line_frames - 5;
            }
            rpg_se_play_latched_5xx(0x500, 1, 0);
            rpg_pause_state = 1;
            rpg_code_entry.sequence_a_progress = 0;
            rpg_code_entry.sequence_b_progress = 0;
            rpg_code_entry.pad_last = 0;
            rpg_actor_set_animation(0x1C, 0x33, 1, 1);
            rpg_pause_debounce = 2;
        }
    }
}

/* s32 without a return: $v0 stays live at the exit, so as1 leaves the first branch slot as a nop (T-9000) */
s32 rpg_check_enemy_defeated(void) {
    if (!(rpg_player_action & 0xFF) && !(rpg_enemy_action & 0xFF) && !(rpg_girl_action & 0xFF)) {
        if (rpg_enemy_action & 0x100000) {
            if (rpg_delinquent_gauges == 0 || rpg_delinquent_hit_points_1 == 0 || rpg_delinquent_hit_points_2 == 0) {
                rpg_over = 1;
            }
        } else if (rpg_enemy_action & 0x600000) {
            if (rpg_enemy_hit_points == 0) {
                rpg_over = 1;
            }
        } else if ((rpg_enemy_action & 0x08000000) && rpg_enemy_hit_points == 0) {
            rpg_over = 1;
        }
    }
}

void rpg_start_victory_sequence(void) {
    if (rpg_over == 1) {
        switch (rpg_encounter) {
        case 0x100000:
            if (rpg_enemy_action & 0x100000) {
                rpg_victory_delinquents();
                return;
            }
            rpg_victory_bancho();
            return;
        case 0x8000000:
            rpg_victory_robot();
            break;
        }
    }
}

void rpg_background_wave(void) {
    s32 endpoint;
    s32 height;
    s32 index;

    if ((rpg_bg_word_5 == 1) && (rpg_encounter & 0x100000)) {
        index = 0;
        if (rpg_pause_state != 3) {
            rpg_bg_word_0 += rpg_bg_word_4;
            if (rpg_bg_word_0 >= rpg_bg_word_2 * 2) {
                rpg_bg_word_0 = 0;
            }
        }
        do {
            endpoint = index + 7;
            height = (rsin(((index % rpg_bg_word_3) << 12) / rpg_bg_word_3 + ((rpg_bg_word_0 << 12) / 2) / rpg_bg_word_2) * rpg_bg_word_1) >> 13;
            rpg_wave_strip_draw(height, (rsin(((endpoint % rpg_bg_word_3) << 12) / rpg_bg_word_3 + ((rpg_bg_word_0 << 12) / 2) / rpg_bg_word_2) * rpg_bg_word_1) >> 13, index, endpoint, 0, 0);
            index += 8;
        } while (index != 168);
    }
}
