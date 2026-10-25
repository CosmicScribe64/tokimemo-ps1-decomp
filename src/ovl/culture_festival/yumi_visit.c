/* yumi_visit.c: src/ovl/BUNKASAI/8013FD90.c:4: Yumi visit block, stages 154-162. Original object 0x8013FD90-0x80142700 (config/objects/BUNKASAI.txt). */
#include "common.h"
#include "ovl/culture_festival.h"

void culture_yumi_visit_entry(void) {
    /* FAKE: game_mode_scene_girl reached through culture_exhibit_club keeps the selector load behind this store (as1 does not
     * move a load over a store through the same symbol); the real source has one object. T-9140 */
    (&culture_exhibit_club)[1] = 0xA;
    switch (culture_exhibit_club) {
    case 0:
        if (game_state.year == 0x60) {
            mode_stage_set(CULTURE_STAGE_YUMI_VISIT_LITERATURE_EXHIBIT);
            return;
        }
        mode_stage_set(CULTURE_STAGE_YUMI_VISIT_SPEECH_CONTEST);
        return;
    case 1:
        mode_stage_set(CULTURE_STAGE_YUMI_VISIT_DRAMA_PLAY);
        return;
    case 2:
        mode_stage_set(CULTURE_STAGE_YUMI_VISIT_EXPERIMENT_SHOW);
        return;
    case 3:
        if (game_state.year == 0x60) {
            mode_stage_set(CULTURE_STAGE_YUMI_VISIT_COMPUTER_GAME);
            return;
        }
        mode_stage_set(CULTURE_STAGE_YUMI_VISIT_EXPERIMENT_SHOW);
        return;
    case 4:
        if (game_state.year == 0x60) {
            mode_stage_set(CULTURE_STAGE_YUMI_VISIT_OPEN_SKETCHING);
            return;
        }
        mode_stage_set(CULTURE_STAGE_YUMI_VISIT_OIL_PAINTING_EXHIBIT);
        return;
    case 5:
        mode_stage_set(CULTURE_STAGE_YUMI_VISIT_CONCERT);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void culture_yumi_visit_literature_exhibit(void) {
    s32 girl_rank_2;
    u32 girl_rank;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x6935);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUMI;
            portrait_show_checked();
            talk_group = 0x25;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0xA);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 7;
                break;
            case 2:
                talk_group = 8;
                break;
            case 3:
                talk_group = 9;
                break;
            case 4:
                talk_group = 0xA;
                break;
            }
            culture_yumi_visit_script_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(0xA) == 4) {
            mode_step_set(0xC);
        } else {
            girl_rank_2 = girl_state_rank(0xA);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 1;
                break;
            case 2:
                talk_group = 2;
                break;
            case 3:
                talk_group = 3;
                break;
            }
            mode_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_girl_out();
        break;
    case 13:
        if (girl_state_rank(0xA) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 14:
        normal_date_bg_fadeout();
        break;
    case 15:
        talk_group = 0;
        mode_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (girl_state_rank(0xA) != 4) {
            game_state.girls[10].flags.f.b13 = 1;
        }
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yumi_visit_speech_contest(void) {
    u32 girl_rank;
    u32 girl_rank_2;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_backstage_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        talk_group = 0x26;
        mode_step_next();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        festival_dispatch_variant = 0;
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x6935);
        break;
    case 9:
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUMI;
        portrait_show_checked();
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0xA);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0xC;
                break;
            case 2:
                talk_group = 0xD;
                break;
            case 3:
                talk_group = 0xE;
                break;
            case 4:
                talk_group = 0xF;
                break;
            }
            culture_yumi_visit_script_start();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        girl_rank_2 = girl_state_rank(0xA);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 4;
            break;
        case 2:
            talk_group = 2;
            break;
        case 3:
            talk_group = 6;
            break;
        case 4:
            mode_step_next();
            break;
        }
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (girl_state_rank(0xA) < 2) {
            talk_group = 0x75;
        }
        normal_date_girl_out();
        break;
    case 16:
        if ((girl_state_rank(0xA) == 4) || (girl_state_rank(0xA) < 2)) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        talk_group = 0;
        mode_step_next();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yumi_visit_drama_play(void) {
    s32 girl_rank_2;
    u32 girl_rank;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_backstage_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        talk_group = 0x28;
        break;
    case 4:
        normal_date_speak();
        break;
    case 5:
        normal_date_bg_fadeout();
        break;
    case 6:
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 7:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x40F4, 1);
        mode_step_next();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            portrait_read_step(0x6935);
        }
        break;
    case 9:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUMI;
        portrait_show_checked();
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0xA);
            switch (girl_rank) {
            case 0:
            case 1:
                if (game_state.year == 0x60) {
                    talk_group = 0x11;
                } else {
                    talk_group = 0x16;
                }
                break;
            case 2:
                if (game_state.year == 0x60) {
                    talk_group = 0x12;
                } else {
                    talk_group = 0x18;
                }
                break;
            case 3:
                if (game_state.year == 0x60) {
                    talk_group = 0x13;
                } else {
                    talk_group = 0x1A;
                }
                break;
            case 4:
                talk_group = 0x14;
                break;
            }
            culture_yumi_visit_script_start();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        mode_step_next();
        if (game_state.year == 0x60) {
            girl_rank_2 = girl_state_rank(0xA);
            switch (girl_rank_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                talk_group = 1;
                break;
            case 2:
                talk_group = 2;
                break;
            case 3:
                talk_group = 3;
                break;
            }
        }
        if (girl_state_rank(0xA) == 4) {
            mode_step_set(0xF);
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        normal_date_girl_out();
        break;
    case 16:
        if (girl_state_rank(0xA) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        talk_group = 0;
        mode_step_next();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yumi_visit_experiment_show(void) {
    u32 girl_rank;
    u32 girl_rank_2;
    u32 girl_rank_3;

    switch (game_state.mode_step) {
    case 0:
        if (((u16) game_state.player_flags.h >> 0xC) == 2) {
            dec_bg_cd_read(0x40FC, 1);
        } else {
            dec_bg_cd_read(0x4105, 1);
        }
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x6935);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUMI;
            portrait_show_checked();
            talk_group = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0xA);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x1C;
                break;
            case 2:
                talk_group = 0x1D;
                break;
            case 3:
                talk_group = 0x1E;
                break;
            case 4:
                talk_group = 0x22;
                break;
            }
            if ((girl_state_rank(0xA) < 4) && (game_state.year == 0x61)) {
                talk_group += 3;
            }
            culture_yumi_visit_script_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadeout();
        break;
    case 11:
        festival_dispatch_variant = 0;
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 12:
        portrait_read_step(0x6935);
        break;
    case 13:
        culture_exhibit_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUMI;
        portrait_show_checked();
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        break;
    case 14:
        if (girl_state_rank(0xA) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(0xA);
            switch (girl_rank_2) {
            case 0:
            case 1:
                if (((u16) game_state.player_flags.h >> 0xC) == 2) {
                    talk_group = 0x23;
                } else {
                    talk_group = 0x40;
                }
                break;
            case 2:
                if (((u16) game_state.player_flags.h >> 0xC) == 2) {
                    talk_group = 0x24;
                } else {
                    talk_group = 0x41;
                }
                break;
            case 3:
                if (((u16) game_state.player_flags.h >> 0xC) == 2) {
                    talk_group = 0x25;
                } else {
                    talk_group = 0x42;
                }
                break;
            case 4:
                talk_group = 0x29;
            }
            if ((girl_state_rank(0xA) < 4) && (game_state.year == 0x61) && (((u16) game_state.player_flags.h >> 0xC) == 2)) {
                talk_group += 3;
            }
            culture_yumi_visit_script_start();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        mode_step_next();
        girl_rank_3 = girl_state_rank(0xA);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 1;
            break;
        case 2:
            talk_group = 2;
            break;
        case 3:
            talk_group = 3;
            break;
        case 4:
            mode_step_set(0x15);
            break;
        }
        if ((girl_state_rank(0xA) < 4) && (game_state.year == 0x61)) {
            talk_group += 3;
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if ((game_state.year == 0x61) && (girl_state_rank(0xA) < 2)) {
            talk_group = 0x75;
        }
        normal_date_girl_out();
        break;
    case 20:
        if ((girl_state_rank(0xA) == 4) || ((game_state.year == 0x61) && (girl_state_rank(0xA) < 2))) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 21:
        normal_date_bg_fadeout();
        break;
    case 22:
        talk_group = 0;
        mode_step_next();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yumi_visit_computer_game(void) {
    s32 girl_rank_2;
    u32 girl_rank;
    u32 girl_rank_3;
    u32 girl_rank_4;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        portrait_read_step(0x6935);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUMI;
        portrait_show_checked();
        culture_exhibit_title_show();
        talk_group = 0x2A;
        break;
    case 5:
        normal_date_bg_fadein();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0xA);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x2A;
                break;
            case 2:
                talk_group = 0x2B;
                break;
            case 3:
                talk_group = 0x2C;
                break;
            case 4:
                talk_group = 0x3C;
                break;
            }
            culture_yumi_visit_script_start();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (girl_state_rank(0xA) == 4) {
            normal_date_girl_out();
        } else {
            mode_step_set(0xE);
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        mode_step_set(0x19);
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->stage_select = 3;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_TT_GAME);
        break;
    case 16:
        culture_festival_return_gate = 1;
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        mode_stage_set(CULTURE_STAGE_GAME_RESUME);
        break;
    case 17:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        culture_exhibit_title_show();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(0xAU);
        culture_festival_return_gate = 0;
        mode_step_next();
        break;
    case 18:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 20:
        portrait_read_step(0x6935);
        break;
    case 21:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUMI;
        portrait_show_checked();
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        break;
    case 22:
        normal_date_bggirl_fadein();
        break;
    case 23:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(0xA);
            switch (girl_rank_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                talk_group = 0x2D;
                break;
            case 2:
                talk_group = 0x2E;
                break;
            case 3:
                talk_group = 0x2F;
                break;
            }
            culture_yumi_visit_script_start();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        if (girl_state_rank(0xA) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 26:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->stage_select = 0;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        mode_stage_set(CULTURE_STAGE_GAME_LAUNCH);
        break;
    case 27:
        culture_festival_return_gate = 1;
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        mode_stage_set(CULTURE_STAGE_GAME_RESUME);
        break;
    case 28:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        culture_exhibit_title_show();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(0xAU);
        culture_festival_return_gate = 0;
        mode_step_next();
        break;
    case 29:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 30:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 31:
        portrait_read_step(0x6935);
        break;
    case 32:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUMI;
        portrait_show_checked();
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        break;
    case 33:
        if (girl_state_rank(0xA) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 34:
        if (cd_read_sync_status() & 1) {
            girl_rank_3 = girl_state_rank(0xA);
            switch (girl_rank_3) {
            case 0:
            case 1:
                talk_group = 0x30;
                break;
            case 2:
                talk_group = 0x32;
                break;
            case 3:
                talk_group = 0x34;
                break;
            case 4:
                talk_group = 0x3E;
                break;
            }
            if ((u8) tt_stages_cleared < 3U) {
                talk_group += 1;
            }
            culture_yumi_visit_script_start();
        }
        break;
    case 35:
        normal_date_speak();
        break;
    case 36:
        mode_step_next();
        girl_rank_4 = girl_state_rank(0xA);
        switch (girl_rank_4) {
        case 0:
        case 1:
            talk_group = 1;
            break;
        case 2:
            talk_group = 2;
            break;
        case 3:
            talk_group = 3;
            break;
        case 4:
            mode_step_set(0x27);
            break;
        }
        break;
    case 37:
        normal_date_speak();
        break;
    case 38:
        normal_date_girl_out();
        break;
    case 39:
        normal_date_bg_fadeout();
        break;
    case 40:
        talk_group = 0;
        mode_step_next();
        break;
    case 41:
        normal_date_speak();
        break;
    case 42:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yumi_visit_open_sketching(void) {
    s32 girl_rank;

    GameState *g = &game_state;

    switch (g->mode_step) {
    case 0x0:
        dec_bg_cd_read(0x4118, 1);
        mode_step_next();
        break;
    case 0x1:
        normal_date_bg_fadeout();
        break;
    case 0x2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 0x3:
        portrait_read_step(0x6C5F);
        break;
    case 0x4:
        if (girl_state_rank(0xA) == 4) {
            culture_exhibit_title_show();
            cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
            mode_step_set(0x32);
        } else {
            culture_exhibit_title_show();
            memcpy((void *)0x80197000, 0x80180000, 0x16800);
            portrait_read_start(0x6935);
            mode_step_next();
        }
        break;
    case 0x5:
        normal_date_bg_fadein();
        break;
    case 0x6:
        if (cd_read_sync_status() & 1) {
            memcpy((void *)0x801AE000, 0x80180000, 0x16800);
            addr_init_bustup();
            g->current_girl = GIRL_YUMI;
            portrait_show_checked();
            talk_group = 0x2C;
        }
        break;
    case 0x7:
        normal_date_speak();
        break;
    case 0x8:
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        mode_step_next();
        break;
    case 0x9:
        normal_date_girl_in();
        break;
    case 0xA:
        if (cd_read_sync_status() & 1) {
            culture_yumi_visit_script_start();
            girl_rank = girl_state_rank(0xA);
            switch (girl_rank) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0x43;
                break;
            case 2:
                talk_group = 0x51;
                break;
            case 3:
                talk_group = 0x5D;
                break;
            }
        }
        break;
    case 0xB:
        normal_date_speak();
        break;
    case 0xC:
        normal_date_girl_out();
        break;
    case 0xD:
        memcpy((void *)0x80180000, 0x80197000, 0x16800);
        addr_init_bustup();
        g->current_girl = GIRL_YOSHIO;
        talk_names_set(g->current_girl);
        portrait_show_checked();
        break;
    case 0xE:
        normal_date_girl_in();
        break;
    case 0xF:
        normal_date_speak();
        break;
    case 0x10:
        normal_date_girl_out();
        break;
    case 0x11:
        memcpy((void *)0x80180000, 0x801AE000, 0x16800);
        addr_init_bustup();
        g->current_girl = GIRL_YUMI;
        talk_names_set(g->current_girl);
        portrait_show_checked();
        break;
    case 0x12:
        normal_date_girl_in();
        break;
    case 0x13:
        normal_date_speak();
        break;
    case 0x14:
        normal_date_girl_out();
        break;
    case 0x15:
        memcpy((void *)0x80180000, 0x80197000, 0x16800);
        addr_init_bustup();
        g->current_girl = GIRL_YOSHIO;
        talk_names_set(g->current_girl);
        portrait_show_checked();
        break;
    case 0x16:
        normal_date_girl_in();
        break;
    case 0x17:
        normal_date_speak();
        break;
    case 0x18:
        D_80122D20 = 1;
        g->current_girl = GIRL_YUMI;
        talk_names_set(g->current_girl);
        mode_step_next();
        break;
    case 0x19:
        normal_date_speak();
        break;
    case 0x1A:
        D_80122D20 = 0;
        g->current_girl = GIRL_YOSHIO;
        talk_names_set(g->current_girl);
        mode_step_next();
        break;
    case 0x1B:
        normal_date_speak();
        break;
    case 0x1C:
        D_80122D20 = 1;
        g->current_girl = GIRL_YUMI;
        talk_names_set(g->current_girl);
        mode_step_next();
        break;
    case 0x1D:
        normal_date_speak();
        break;
    case 0x1E:
        D_80122D20 = 0;
        g->current_girl = GIRL_YOSHIO;
        talk_names_set(g->current_girl);
        mode_step_next();
        break;
    case 0x1F:
        normal_date_speak();
        break;
    case 0x20:
        if (girl_state_rank(0xA) == 3) {
            mode_step_set(0x22);
        } else {
            D_80122D20 = 1;
            g->current_girl = GIRL_YUMI;
        talk_names_set(g->current_girl);
            mode_step_next();
        }
        break;
    case 0x21:
        normal_date_speak();
        break;
    case 0x22:
        if (girl_state_rank(0xA) == 3) {
            mode_step_set(0x24);
        } else {
            D_80122D20 = 0;
            g->current_girl = GIRL_YOSHIO;
        talk_names_set(g->current_girl);
            mode_step_next();
        }
        break;
    case 0x23:
        normal_date_speak();
        break;
    case 0x24:
        normal_date_girl_out();
        break;
    case 0x25:
        memcpy((void *)0x80180000, 0x801AE000, 0x16800);
        addr_init_bustup();
        g->current_girl = GIRL_YUMI;
        talk_names_set(g->current_girl);
        portrait_show_checked();
        break;
    case 0x26:
        normal_date_girl_in();
        break;
    case 0x27:
        normal_date_speak();
        break;
    case 0x28:
        if (girl_state_rank(0xA) >= 2) {
            mode_step_set(0x2A);
        } else {
            D_80122D20 = 1;
            g->current_girl = GIRL_YOSHIO;
        talk_names_set(g->current_girl);
            mode_step_next();
        }
        break;
    case 0x29:
        normal_date_speak();
        break;
    case 0x2A:
        D_80122D20 = 0;
        g->current_girl = GIRL_YUMI;
        talk_names_set(g->current_girl);
        mode_step_next();
        break;
    case 0x2B:
        if ((girl_state_rank(0xA) == 3) || (girl_state_rank(0xA) == 2)) {
            mode_step_next();
        } else {
            normal_date_speak();
        }
        break;
    case 0x2C:
        normal_date_girl_out();
        break;
    case 0x2D:
        normal_date_speak();
        break;
    case 0x2E:
        normal_date_bg_fadeout();
        break;
    case 0x2F:
        talk_group = 0;
        mode_step_next();
        break;
    case 0x30:
        normal_date_speak();
        break;
    case 0x31:
        culture_scene_end();
        break;
    case 0x32:
        normal_date_bg_fadein();
        break;
    case 0x33:
        if (cd_read_sync_status() & 1) {
            culture_yumi_visit_script_start();
            talk_group = 0x67;
            addr_init_bustup();
            g->current_girl = GIRL_YOSHIO;
        talk_names_set(g->current_girl);
            portrait_show_checked();
        }
        break;
    case 0x34:
    case 0x35:
        normal_date_girl_in();
        break;
    case 0x36:
        normal_date_speak();
        break;
    case 0x37:
        normal_date_girl_out();
        break;
    case 0x38:
        talk_names_set(0xAU);
        mode_step_set(0x2D);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yumi_visit_oil_painting_exhibit(void) {
    u32 girl_rank;
    u32 girl_rank_2;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x6935);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUMI;
            portrait_show_checked();
            talk_group = 0x2D;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0xA);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x69;
                break;
            case 2:
                talk_group = 0x6A;
                break;
            case 3:
                talk_group = 0x6B;
                break;
            case 4:
                talk_group = 0xF;
                break;
            }
            culture_yumi_visit_script_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        mode_step_next();
        girl_rank_2 = girl_state_rank(0xA);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 4;
            break;
        case 2:
            talk_group = 5;
            break;
        case 3:
            talk_group = 6;
            break;
        case 4:
            mode_step_set(0xC);
            break;
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (girl_state_rank(0xA) < 2) {
            talk_group = 0x75;
        }
        normal_date_girl_out();
        break;
    case 13:
        if ((girl_state_rank(0xA) == 4) || (girl_state_rank(0xA) < 2)) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 14:
        normal_date_bg_fadeout();
        break;
    case 15:
        talk_group = 0;
        mode_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yumi_visit_concert(void) {
    u32 girl_rank;
    u32 girl_rank_2;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_backstage_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        talk_group = 0x2F;
        mode_step_next();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        festival_dispatch_variant = 2;
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x6935);
        break;
    case 9:
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUMI;
        portrait_show_checked();
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0xA);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x6C;
                break;
            case 2:
                talk_group = 0x6D;
                break;
            case 3:
                talk_group = 0x6E;
                break;
            case 4:
                talk_group = 0x72;
                break;
            }
            if ((girl_state_rank(0xA) < 4) && (game_state.year == 0x61)) {
                talk_group += 3;
            }
            culture_yumi_visit_script_start();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        mode_step_next();
        girl_rank_2 = girl_state_rank(0xA);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 1;
            break;
        case 2:
            talk_group = 2;
            break;
        case 3:
            talk_group = 3;
            break;
        case 4:
            mode_step_set(0xF);
            break;
        }
        if ((girl_state_rank(0xA) < 4) && (game_state.year == 0x61)) {
            talk_group += 3;
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if ((game_state.year == 0x61) && (girl_state_rank(0xA) < 2)) {
            talk_group = 0x75;
        }
        normal_date_girl_out();
        break;
    case 16:
        if ((girl_state_rank(0xA) == 4) || ((game_state.year == 0x61) && (girl_state_rank(0xA) < 2))) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        talk_group = 0;
        mode_step_next();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yumi_visit_script_start(void) {
    talk_names_set(10);
    culture_script_set_yumi_visit();
    talk_line = 0;
    talk_text_table = culture_script_text_table;
    talk_command_table = culture_script_command_table;
    talk_voice_table = culture_script_voice_table;
    mode_step_next();
}
