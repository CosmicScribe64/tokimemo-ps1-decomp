/* yuko_visit.c: src/ovl/BUNKASAI/80138A70.c:4: Yuko visit block, stages 133-144. Original object 0x80138A70-0x8013B190 (config/objects/BUNKASAI.txt). */
#include "common.h"
#include "ovl/culture_festival.h"

void culture_yuko_visit_entry(void) {
    CultureFestivalDispatch *dispatch = (CultureFestivalDispatch *)&culture_exhibit_club;
    dispatch->girl = 8;
    switch (dispatch->club_id) {
    case 0:
        if (game_state.year == 0x61) {
            mode_stage_set(CULTURE_STAGE_YUKO_VISIT_SPEECH_CONTEST);
            return;
        }
        mode_stage_set(CULTURE_STAGE_YUKO_VISIT_LITERATURE_EXHIBIT);
        return;
    case 1:
        mode_stage_set(CULTURE_STAGE_YUKO_VISIT_DRAMA_PLAY);
        return;
    case 2:
        mode_stage_set(CULTURE_STAGE_YUKO_VISIT_EXPERIMENT_SHOW);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(CULTURE_STAGE_YUKO_VISIT_COMPUTER_FORTUNE);
            return;
        case 0x60:
            mode_stage_set(CULTURE_STAGE_YUKO_VISIT_COMPUTER_GAME);
            return;
        case 0x61:
            mode_stage_set(CULTURE_STAGE_YUKO_VISIT_EXPERIMENT_SHOW);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(CULTURE_STAGE_YUKO_VISIT_PORTRAIT_DRAWING);
            return;
        case 0x60:
            mode_stage_set(CULTURE_STAGE_YUKO_VISIT_OPEN_SKETCHING);
            return;
        case 0x61:
            mode_stage_set(CULTURE_STAGE_YUKO_VISIT_OIL_PAINTING_EXHIBIT);
            return;
        }
        break;
    case 5:
        mode_stage_set(CULTURE_STAGE_YUKO_VISIT_CONCERT);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

/* T-12220: bit-field stores preserve the original byte-load register allocation. */
void culture_yuko_visit_literature_exhibit(void) {
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
            portrait_read_start(0x5012);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUKO;
            portrait_show_checked();
            if (game_state.year == 0x5F) {
                talk_group = 0x24;
            } else {
                talk_group = 0x25;
            }
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        mode_step_next();
        break;
    case 7:
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(8);
            switch (girl_rank) {
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
                talk_group = 4;
                break;
            }
            if (game_state.year == 0x60) {
                talk_group += 5;
            }
            culture_yuko_visit_script_start();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        normal_date_girl_out();
        break;
    case 12:
        if (girl_state_rank(8) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 13:
        normal_date_bg_fadeout();
        break;
    case 14:
        talk_group = 0;
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (girl_state_rank(8) != 4) {
            if (game_state.year == 0x5F) {
                game_state.girls[8].flags.f.b12 = 1;
            } else {
                game_state.girls[8].flags.f.b13 = 1;
            }
        }
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yuko_visit_speech_contest(void) {
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
        portrait_read_step(0x5012);
        break;
    case 9:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKO;
        portrait_show_checked();
        culture_exhibit_title_show();
        D_80125CA0 = 0xC0;
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(8);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0xB;
                break;
            case 2:
                talk_group = 0xC;
                break;
            case 3:
                talk_group = 0xD;
                break;
            case 4:
                talk_group = 0xE;
                break;
            }
            culture_yuko_visit_script_start();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_girl_out();
        break;
    case 14:
        if (girl_state_rank(8) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 15:
        normal_date_bg_fadeout();
        break;
    case 16:
        talk_group = 0;
        mode_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yuko_visit_drama_play(void) {
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
        if (game_state.year == 0x5F) {
            talk_group = 0x27;
        } else {
            talk_group = 0x28;
        }
        normal_date_bg_fadein();
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
            portrait_read_step(0x5012);
        }
        break;
    case 9:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKO;
        portrait_show_checked();
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            culture_yuko_visit_script_start();
            girl_rank = girl_state_rank(8);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = (game_state.year * 3) - 0x10D;
                break;
            case 2:
                talk_group = (game_state.year * 3) - 0x10C;
                break;
            case 3:
                talk_group = (game_state.year * 3) - 0x10B;
                break;
            case 4:
                talk_group = 0x19;
                break;
            }
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_girl_out();
        break;
    case 14:
        if (girl_state_rank(8) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 15:
        normal_date_bg_fadeout();
        break;
    case 16:
        talk_group = 0;
        mode_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yuko_visit_experiment_show(void) {
    u32 girl_rank;
    u32 girl_rank_2;

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
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        portrait_read_step(0x5012);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKO;
        portrait_show_checked();
        culture_exhibit_title_show();
        talk_group = 0x29;
        break;
    case 5:
        normal_date_bg_fadein();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(8);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x1B;
                break;
            case 2:
                talk_group = 0x1C;
                break;
            case 3:
                talk_group = 0x1D;
                break;
            case 4:
                talk_group = 0x1E;
                break;
            }
            culture_yuko_visit_script_start();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        normal_date_bggirl_fadeout();
        break;
    case 12:
        festival_dispatch_variant = 0;
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 13:
        portrait_read_step(0x5012);
        break;
    case 14:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKO;
        portrait_show_checked();
        culture_exhibit_title_show();
        D_80125CA0 = 0xC0;
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        break;
    case 15:
        if (girl_state_rank(8) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(8);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = (game_state.year * 3) - 0xFE;
                break;
            case 2:
                talk_group = (game_state.year * 3) - 0xFD;
                break;
            case 3:
                talk_group = (game_state.year * 3) - 0xFC;
                break;
            case 4:
                talk_group = 0x28;
                break;
            }
            culture_yuko_visit_script_start();
        }
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if (girl_state_rank(8) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 19:
        normal_date_bg_fadeout();
        break;
    case 20:
        talk_group = 0;
        mode_step_next();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yuko_visit_computer_fortune(void) {
    u32 girl_rank;
    u32 girl_rank_2;
    u32 girl_rank_3;

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
        portrait_read_step(0x5012);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKO;
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
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(8);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x29;
                break;
            case 2:
                talk_group = 0x2A;
                break;
            case 3:
                talk_group = 0x2B;
                break;
            case 4:
                talk_group = 0x2C;
                break;
            }
            culture_yuko_visit_script_start();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        normal_date_girl_out();
        break;
    case 12:
        if (girl_state_rank(8) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 13:
        audio_command(0x202);
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        mode_stage_next();
        break;
    case 14:
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        mode_step_next();
        break;
    case 15:
        if (girl_state_rank(8) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            culture_yuko_visit_script_start();
            girl_rank_2 = girl_state_rank(8);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x2E;
                break;
            case 2:
                talk_group = 0x31;
                break;
            case 3:
                talk_group = 0x34;
                break;
            case 4:
                talk_group = 0x37;
                break;
            }
            talk_group += fortune_result_index;
        }
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        mode_step_next();
        girl_rank_3 = girl_state_rank(8);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 0x3A;
            break;
        case 2:
            talk_group = 0x3B;
            break;
        case 3:
            talk_group = 0x3C;
            break;
        case 4:
            mode_step_set(0x15);
            break;
        }
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        if (girl_state_rank(8) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
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

void culture_yuko_visit_computer_game(void) {
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
        portrait_read_step(0x5012);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKO;
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
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(8);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x3D;
                break;
            case 2:
                talk_group = 0x3E;
                break;
            case 3:
                talk_group = 0x3F;
                break;
            case 4:
                talk_group = 0x40;
                break;
            }
            culture_yuko_visit_script_start();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (girl_state_rank(8) == 4) {
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
        ((ExtraModeFlags *)&D_800E7D4F)->stage_select = 4;
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
        talk_names_set(8U);
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
        portrait_read_step(0x5012);
        break;
    case 21:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKO;
        portrait_show_checked();
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        break;
    case 22:
        normal_date_bggirl_fadein();
        break;
    case 23:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(8);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x42;
                break;
            case 2:
                talk_group = 0x43;
                break;
            case 3:
                talk_group = 0x44;
                break;
            }
            culture_yuko_visit_script_start();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        if (girl_state_rank(8) == 4) {
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
        talk_names_set(8U);
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
        portrait_read_step(0x5012);
        break;
    case 32:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKO;
        portrait_show_checked();
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        break;
    case 33:
        if (girl_state_rank(8) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 34:
        if (cd_read_sync_status() & 1) {
            girl_rank_3 = girl_state_rank(8);
            switch (girl_rank_3) {
            case 0:
            case 1:
                talk_group = 0x45;
                break;
            case 2:
                talk_group = 0x47;
                break;
            case 3:
                talk_group = 0x49;
                break;
            case 4:
                talk_group = 0x4B;
                break;
            }
            if ((u8) tt_stages_cleared < 4U) {
                talk_group += 1;
            }
            culture_yuko_visit_script_start();
        }
        break;
    case 35:
        normal_date_speak();
        break;
    case 36:
        mode_step_next();
        girl_rank_4 = girl_state_rank(8);
        switch (girl_rank_4) {
        case 0:
        case 1:
            talk_group = 0x3A;
            break;
        case 2:
            talk_group = 0x3B;
            break;
        case 3:
            talk_group = 0x3C;
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

void culture_yuko_visit_portrait_drawing(void) {
    s32 girl_rank_2;
    u32 girl_rank;

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
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        portrait_read_step(0x5012);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKO;
        portrait_show_checked();
        talk_group = 0x2B;
        culture_exhibit_title_show();
        break;
    case 5:
        normal_date_bg_fadein();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(8);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x56;
                break;
            case 2:
                talk_group = 0x59;
                break;
            case 3:
                talk_group = 0x5C;
                break;
            case 4:
                talk_group = 0x5F;
                break;
            }
            culture_yuko_visit_script_start();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (girl_state_rank(8) == 4) {
            mode_step_set(0xF);
        } else {
            if ((u16) game_state.clubs[4].practice_points < 0x64U) {
                talk_group += 1;
            }
            mode_step_next();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        girl_rank_2 = girl_state_rank(8);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x3A;
            break;
        case 2:
            talk_group = 0x3B;
            break;
        case 3:
            talk_group = 0x3C;
            break;
        }
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        normal_date_girl_out();
        break;
    case 16:
        if (girl_state_rank(8) == 4) {
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

void culture_yuko_visit_open_sketching(void) {
    u32 girl_rank;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
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
        portrait_read_step(0x5012);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKO;
        portrait_show_checked();
        culture_exhibit_title_show();
        talk_group = 0x2C;
        break;
    case 5:
        normal_date_bg_fadein();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(8);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x61;
                break;
            case 2:
                talk_group = 0x63;
                break;
            case 3:
                talk_group = 0x65;
                break;
            case 4:
                talk_group = 0x67;
                break;
            }
            culture_yuko_visit_script_start();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        normal_date_girl_out();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bg_fadeout();
        break;
    case 14:
        talk_group = 0;
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yuko_visit_oil_painting_exhibit(void) {
    u32 girl_rank;

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
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        portrait_read_step(0x5012);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKO;
        portrait_show_checked();
        culture_exhibit_title_show();
        talk_group = 0x2D;
        break;
    case 5:
        normal_date_bg_fadein();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(8);
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
                talk_group = 0x6C;
                break;
            }
            culture_yuko_visit_script_start();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        normal_date_girl_out();
        break;
    case 12:
        if (girl_state_rank(8) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 13:
        normal_date_bg_fadeout();
        break;
    case 14:
        talk_group = 0;
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yuko_visit_concert(void) {
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
        break;
    case 4:
        if (game_state.year == 0x5F) {
            talk_group = 0x2E;
        } else {
            talk_group = 0x2F;
        }
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
        portrait_read_step(0x5012);
        break;
    case 9:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKO;
        portrait_show_checked();
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(8);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = (game_state.year * 3) - 0xAF;
                break;
            case 2:
                talk_group = (game_state.year * 3) - 0xAE;
                break;
            case 3:
                talk_group = (game_state.year * 3) - 0xAD;
                break;
            case 4:
                talk_group = 0x77;
                break;
            }
            culture_yuko_visit_script_start();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_girl_out();
        break;
    case 14:
        if (girl_state_rank(8) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 15:
        normal_date_bg_fadeout();
        break;
    case 16:
        talk_group = 0;
        mode_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yuko_visit_script_start(void) {
    talk_names_set(8);
    culture_script_set_yuko_visit();
    talk_line = 0;
    talk_text_table = culture_script_text_table;
    talk_command_table = culture_script_command_table;
    talk_voice_table = culture_script_voice_table;
    mode_step_next();
}
