#include "common.h"
#include "ovl/TAIIKU.h"

void func_80141C00(void) {
    if ((girl_state_rank(6) < 2) && (((u32) (game_state.girls[6].flags.w << 0x1E) >> 0x1F) == 1) && (game_state.scene_flags[45] == 0)) {
        game_state.scene_flags[45] = 1;
        mode_stage_next();
    } else {
        game_state.mode_stage = 0;
        game_state.mode_step = 0;
        D_80122ECC = 1;
    }
    Default_Disp();
}

void func_80141C88(void) {
    s32 temp_v0;

    temp_v0 = func_80141CDC();
    if (temp_v0 != 0) {
        if (temp_v0 == 1) {
            game_state.mode_stage = 0;
            game_state.mode_step = 0;
            D_80122ECC = 1;
            return;
        }
        game_mode_reset_to_title();
    }
}

s32 func_80141CDC(void) {
    s32 i;

    portrait_wink_update();
    switch (game_state.mode_step) {
    case 0:
        audio_bank_load_start(0, 0xBF98, 0xBF79, 0xD712, 0xD6C3, 0xD6B7);
        D_8014A0C0 = 0;
        talk_clear();
        text_system_reset();
        tpage_buf_clear();
        dec_bg_reset();
        actor_clear_upper();
        k_reset(0);
        message_window_set_title((s32) "中庭");
        message_window_title_show(0);
        message_window_init();
        dec_bg_brightness = 0;
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
            func_80142140();
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
        func_80047550();
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
            func_80145210();
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
        func_80085368();
        break;
    case 19:
        audio_command(0xD1);
        mode_step_next();
        break;
    case 20:
        func_800853FC();
        break;
    case 21:
        D_8014A0C0 = 1;
        event_log_add(2, 0x2D);
        game_state.girls[6].affection += 2;
        game_state.girls[6].tokimeki += 1;
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
    return D_8014A0C0;
}
