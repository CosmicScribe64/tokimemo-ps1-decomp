/* candidate_scene_state_handlers.c: ; src/ovl/TAIIKU/80141C00.c:4-16 dispatches state handlers for the Nozomi scene. Original object 0x80141C00-0x80142140 (config/objects/TAIIKU.txt). */
#include "common.h"
#include "ovl/sports_festival.h"
#include "main/scene.h"

void sports_nozomi_scene_entry_check(void) {
    if ((girl_state_rank(6) < 2) && (((u32) (game_state.girls[6].flags.w << 0x1E) >> 0x1F) == 1) && (game_state.scene_flags[45] == 0)) {
        game_state.scene_flags[45] = 1;
        mode_stage_next();
    } else {
        game_state.mode_stage = 0;
        game_state.mode_step = 0;
        sports_scene_variant = 1;
    }
    Default_Disp();
}

void sports_nozomi_scene_return_update(void) {
    s32 scene_result;

    scene_result = sports_nozomi_courtyard_scene_step();
    if (scene_result != 0) {
        if (scene_result == 1) {
            game_state.mode_stage = 0;
            game_state.mode_step = 0;
            sports_scene_variant = 1;
            return;
        }
        game_mode_reset_to_title();
    }
}

s32 sports_nozomi_courtyard_scene_step(void) {
    s32 i;

    portrait_wink_update();
    switch (game_state.mode_step) {
    case 0:
        audio_bank_load_start(0, 0xBF98, 0xBF79, 0xD712, 0xD6C3, 0xD6B7);
        sports_nozomi_scene_result = 0;
        talk_clear();
        text_system_reset();
        tpage_buf_clear();
        dec_bg_reset();
        actor_clear_upper();
        k_reset(0);
        message_window_set_title((s32) "中庭");
        message_window_title_show(0);
        message_window_init();
        bg_brightness = 0;
        mode_step_next();
        break;
    case 1:
        if (audio_bank_load_poll() == 1) {
            dec_bg_cd_read(0x4071, 1);
            mode_step_next();
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x6476);
            mode_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_NOZOMI;
            portrait_show_checked();
            cd_read_sectors(1U, 0x801E8000, 0xB7BF);
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            text_marker_hide();
            talk_names_set(6U);
            sports_nozomi_talk_resource_addresses_init();
            talk_group = 0;
            talk_line = 0;
            talk_text_table = D_8014A0D0;
            talk_command_table = D_8014A0D4;
            talk_voice_table = D_8014A0D8;
            mode_step_next();
        }
        break;
    case 5:
        if (check_end_k() != 0) {
            message_window_title_show(1);
        }
        /* fallthrough */
    case 7:
    case 9:
    case 10:
    case 12:
    case 18:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadein();
        if (game_state.mode_step == 7) {
            audio_bank_start(0, 0x202);
        }
        break;
    case 8:
        audio_command(0x60A);
        draw_shake_start();
        mode_step_next();
        break;
    case 11:
        normal_date_girl_in();
        break;
    case 13:
        cd_read_sectors(0x3DU, 0x80197000, 0xBB5A);
        mode_step_next();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            sports_nozomi_scene_picture_resource_addresses_init();
            load_palette(D_8014A3A0, 0x11, 1, 2, 0);
            scene_still_show(D_8014A3A4, D_8014A3A8, D_8014A3AC, D_8014A3B0, D_8014A3B4, D_8014A3B8);
            scene_anim_show(D_8014A394, D_8014A398, D_8014A390, (s32) D_8014A39C);
            for (i = 0; i < 2; i++) {
                ((Actor *)actor_pool)[96 + i].unk_04 |= 8;
            }
            dec_bg_cd_read(0x4728, 1);
            mode_step_next();
        }
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 17:
        scene_still_fade_in();
        break;
    case 19:
        audio_command(0xD1);
        mode_step_next();
        break;
    case 20:
        scene_still_fade_out();
        break;
    case 21:
        sports_nozomi_scene_result = 1;
        event_log_add(2, 0x2D);
        game_state.girls[6].affection += 2;
        game_state.girls[6].tokimeki += 1;
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
    return sports_nozomi_scene_result;
}
