/* shiori_visit.c: src/ovl/BUNKASAI/80148040.c:4: Shiori visit block, stages 15-27. Original object 0x80148040-0x8014ABC0 (config/objects/BUNKASAI.txt). */
#include "common.h"
#include "ovl/culture_festival.h"

void culture_shiori_visit_entry(void) {
    CultureFestivalDispatch *dispatch = (CultureFestivalDispatch *)&culture_exhibit_club;
    dispatch->girl = 0;
    switch (dispatch->club_id) {
    case 0:
        if (game_state.year == 0x61) {
            mode_stage_set(CULTURE_STAGE_SHIORI_VISIT_SPEECH_CONTEST);
            return;
        }
        mode_stage_set(CULTURE_STAGE_SHIORI_VISIT_LITERATURE_EXHIBIT);
        return;
    case 1:
        mode_stage_set(CULTURE_STAGE_SHIORI_VISIT_DRAMA_PLAY);
        return;
    case 2:
        mode_stage_set(CULTURE_STAGE_SHIORI_VISIT_EXPERIMENT_SHOW);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(CULTURE_STAGE_SHIORI_VISIT_COMPUTER_FORTUNE);
            return;
        case 0x60:
            mode_stage_set(CULTURE_STAGE_SHIORI_VISIT_COMPUTER_GAME);
            return;
        case 0x61:
            mode_stage_set(CULTURE_STAGE_SHIORI_VISIT_COMPUTER_EXPERIMENT_SHOW);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(CULTURE_STAGE_SHIORI_VISIT_PORTRAIT_DRAWING);
            return;
        case 0x60:
            mode_stage_set(CULTURE_STAGE_SHIORI_VISIT_OPEN_SKETCHING);
            return;
        case 0x61:
            mode_stage_set(CULTURE_STAGE_SHIORI_VISIT_OIL_PAINTING_EXHIBIT);
            return;
        }
        break;
    case 5:
        mode_stage_set(CULTURE_STAGE_SHIORI_VISIT_CONCERT);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void culture_shiori_visit_literature_exhibit(void) {
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
            portrait_read_start(0x544A);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            portrait_show_checked();
            if (game_state.year == 0x5F) {
                talk_group = 0x24;
            } else {
                talk_group = 0x25;
            }
        }
        break;
    case 5:
    case 9:
    case 11:
    case 16:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0xA;
                break;
            case 2:
                talk_group = 0xB;
                break;
            case 3:
                talk_group = 0xC;
                break;
            case 4:
                talk_group = 0xD;
                break;
            }
            if (game_state.year == 0x60) {
                talk_group += 4;
            }
            culture_shiori_visit_script_start();
        }
        break;
    case 10:
        if (girl_state_rank(0) == 4) {
            mode_step_set(0xC);
        } else {
            girl_rank_2 = girl_state_rank(0);
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
    case 12:
        talk_group = 0x90;
        normal_date_girl_out();
        break;
    case 13:
        if (girl_state_rank(0) == 4) {
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
    case 17:
        if (girl_state_rank(0) < 4) {
            if (game_state.year == 0x5F) {
                game_state.girls[0].flags.f.b12 = 1;
            } else {
                game_state.girls[0].flags.f.b13 = 1;
            }
        }
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void culture_shiori_visit_speech_contest(void) {
    u32 girl_rank;
    u32 girl_rank_2;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        mode_step_next();
        break;
    case 1:
    case 6:
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
    case 12:
    case 14:
    case 18:
        normal_date_speak();
        break;
    case 7:
        festival_dispatch_variant = 0;
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x544A);
        break;
    case 9:
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        portrait_show_checked();
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x12;
                break;
            case 2:
                talk_group = 0x13;
                break;
            case 3:
                talk_group = 0x14;
                break;
            case 4:
                talk_group = 0x15;
                break;
            }
            culture_shiori_visit_script_start();
        }
        break;
    case 13:
        girl_rank_2 = girl_state_rank(0);
        switch (girl_rank_2) {
        case 0:
            talk_group = 4;
            break;
        case 1:
            talk_group = 5;
            break;
        case 2:
            talk_group = 2;
            break;
        case 3:
            talk_group = 3;
            break;
        case 4:
            talk_group = 7;
            break;
        }
        mode_step_next();
        break;
    case 15:
        normal_date_girl_out();
        break;
    case 16:
        normal_date_bg_fadeout();
        break;
    case 17:
        talk_group = 0;
        mode_step_next();
        break;
    case 19:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void culture_shiori_visit_drama_play(void) {
    u32 girl_rank;
    u32 girl_rank_2;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        mode_step_next();
        break;
    case 1:
    case 5:
    case 17:
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
        if (game_state.year == 0x5F) {
            talk_group = 0x27;
        } else {
            talk_group = 0x28;
        }
        break;
    case 4:
    case 12:
    case 14:
    case 19:
        normal_date_speak();
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
            portrait_read_step(0x544A);
        }
        break;
    case 9:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        portrait_show_checked();
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x16;
                break;
            case 2:
                talk_group = 0x17;
                break;
            case 3:
                talk_group = 0x18;
                break;
            case 4:
                talk_group = 0x19;
                break;
            }
            talk_group = (talk_group + (game_state.year * 4)) - 0x17C;
            culture_shiori_visit_script_start();
        }
        break;
    case 13:
        mode_step_next();
        girl_rank_2 = girl_state_rank(0);
        switch (girl_rank_2) {
        case 0:
            talk_group = 4;
            break;
        case 1:
            talk_group = 5;
            break;
        case 2:
            talk_group = 2;
            break;
        case 3:
            talk_group = 3;
            break;
        case 4:
            talk_group = 7;
            mode_step_set(0xF);
            break;
        }
        break;
    case 15:
        normal_date_girl_out();
        break;
    case 16:
        if (girl_state_rank(0) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 18:
        talk_group = 0;
        mode_step_next();
        break;
    case 20:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void culture_shiori_visit_experiment_show(void) {
    u32 girl_rank;
    u32 girl_rank_2;
    u32 girl_rank_3;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        mode_step_next();
        break;
    case 1:
    case 20:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x544A);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            portrait_show_checked();
            talk_group = 0x29;
        }
        break;
    case 5:
    case 9:
    case 16:
    case 18:
    case 22:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x22;
                break;
            case 2:
                talk_group = 0x23;
                break;
            case 3:
                talk_group = 0x24;
                break;
            case 4:
                talk_group = 0x25;
                break;
            }
            talk_group = (talk_group + (game_state.year * 8)) - 0x2F8;
            culture_shiori_visit_script_start();
        }
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
        portrait_read_step(0x544A);
        break;
    case 13:
        culture_exhibit_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        portrait_show_checked();
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        break;
    case 14:
        if (girl_state_rank(0) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(0);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x26;
                break;
            case 2:
                talk_group = 0x27;
                break;
            case 3:
                talk_group = 0x28;
                break;
            case 4:
                talk_group = 0x29;
                break;
            }
            talk_group = (talk_group + (game_state.year * 8)) - 0x2F8;
            culture_shiori_visit_script_start();
        }
        break;
    case 17:
        mode_step_next();
        girl_rank_3 = girl_state_rank(0);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 1;
            break;
        case 2:
            talk_group = 0x76;
            break;
        case 3:
            talk_group = 3;
            break;
        case 4:
            mode_step_set(0x14);
            break;
        }
        break;
    case 19:
        normal_date_girl_out();
        break;
    case 21:
        talk_group = 0;
        mode_step_next();
        break;
    case 23:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void culture_shiori_visit_computer_fortune(void) {
    s32 girl_rank_4;
    u32 girl_rank;
    u32 girl_rank_2;
    u32 girl_rank_3;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        mode_step_next();
        break;
    case 1:
    case 22:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x544A);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x2A;
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            portrait_show_checked();
        }
        break;
    case 5:
    case 9:
    case 16:
    case 18:
    case 20:
    case 24:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x3A;
                break;
            case 2:
                talk_group = 0x3F;
                break;
            case 3:
                talk_group = 0x44;
                break;
            case 4:
                talk_group = 0x49;
                break;
            }
            culture_shiori_visit_script_start();
        }
        break;
    case 10:
    case 21:
        normal_date_girl_out();
        break;
    case 11:
        if (girl_state_rank(0) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 12:
        audio_command(0x202);
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        mode_stage_next();
        break;
    case 13:
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        mode_step_next();
        break;
    case 14:
        if (girl_state_rank(0) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(0);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x3B;
                break;
            case 2:
                talk_group = 0x40;
                break;
            case 3:
                talk_group = 0x45;
                break;
            case 4:
                talk_group = 0x4B;
                break;
            }
            talk_group += fortune_result_index;
            culture_shiori_visit_script_start();
        }
        break;
    case 17:
        mode_step_next();
        girl_rank_3 = girl_state_rank(0);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 0x3E;
            break;
        case 2:
            talk_group = 0x43;
            break;
        case 3:
            if (fortune_result_index == 2) {
                talk_group = 0x48;
            } else {
                mode_step_set(0x13);
            }
            break;
        case 4:
            mode_step_set(0x16);
            break;
        }
        break;
    case 19:
        girl_rank_4 = girl_state_rank(0);
        switch (girl_rank_4) {
        case 0:
        case 1:
            talk_group = 1;
            break;
        case 2:
            talk_group = 0x76;
            break;
        case 3:
            talk_group = 3;
            break;
        }
        mode_step_next();
        break;
    case 23:
        talk_group = 0;
        mode_step_next();
        break;
    case 25:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void culture_shiori_visit_computer_game(void) {
    s32 girl_rank_4;
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
        portrait_read_step(0x544A);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
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
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x4E;
                break;
            case 2:
                talk_group = 0x4F;
                break;
            case 3:
                talk_group = 0x50;
                break;
            case 4:
                talk_group = 0x51;
                break;
            }
            culture_shiori_visit_script_start();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (girl_state_rank(0) == 4) {
            normal_date_girl_out();
        } else {
            mode_step_set(0xE);
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        mode_step_next();
        break;
    case 14:
        if (girl_state_rank(0) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 15:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->stage_select = 0;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        mode_stage_set(CULTURE_STAGE_GAME_LAUNCH);
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
        talk_names_set(0U);
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
        portrait_read_step(0x544A);
        break;
    case 21:
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        portrait_show_checked();
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        break;
    case 22:
        if (girl_state_rank(0) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 23:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(0);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x53;
                break;
            case 2:
                talk_group = 0x55;
                break;
            case 3:
                talk_group = 0x57;
                break;
            case 4:
                talk_group = 0x59;
                break;
            }
            if ((u8) tt_stages_cleared < 2U) {
                talk_group += 1;
            }
            culture_shiori_visit_script_start();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        mode_step_next();
        girl_rank_3 = girl_state_rank(0);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 0x5B;
            break;
        case 2:
            talk_group = 0x5C;
            break;
        case 3:
            talk_group = 0x5D;
            break;
        case 4:
            mode_step_set(0x1E);
            break;
        }
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        girl_rank_4 = girl_state_rank(0);
        switch (girl_rank_4) {
        case 0:
        case 1:
            talk_group = 1;
            break;
        case 2:
            talk_group = 0x76;
            break;
        case 3:
            talk_group = 3;
            break;
        }
        mode_step_next();
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        normal_date_girl_out();
        break;
    case 30:
        normal_date_bg_fadeout();
        break;
    case 31:
        talk_group = 0;
        mode_step_next();
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void culture_shiori_visit_computer_experiment_show(void) {
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
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x544A);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x29;
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            portrait_show_checked();
        }
        break;
    case 5:
    case 9:
    case 16:
    case 18:
    case 22:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x5E;
                break;
            case 2:
                talk_group = 0x5F;
                break;
            case 3:
                talk_group = 0x60;
                break;
            case 4:
                talk_group = 0x61;
                break;
            }
            culture_shiori_visit_script_start();
        }
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
        portrait_read_step(0x544A);
        break;
    case 13:
        culture_exhibit_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        portrait_show_checked();
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        break;
    case 14:
        if (girl_state_rank(0) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(0);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x62;
                break;
            case 2:
                talk_group = 0x63;
                break;
            case 3:
                talk_group = 0x64;
                break;
            case 4:
                talk_group = 0x65;
                break;
            }
            culture_shiori_visit_script_start();
        }
        break;
    case 17:
        mode_step_next();
        girl_rank_3 = girl_state_rank(0);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 1;
            break;
        case 2:
            talk_group = 0x76;
            break;
        case 3:
            talk_group = 3;
            break;
        case 4:
            mode_step_set(0x14);
            break;
        }
        break;
    case 19:
        normal_date_girl_out();
        break;
    case 20:
        normal_date_bg_fadeout();
        break;
    case 21:
        talk_group = 0;
        mode_step_next();
        break;
    case 23:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void culture_shiori_visit_portrait_drawing(void) {
    s32 girl_rank_2;
    s32 girl_rank_3;
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
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x544A);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            portrait_show_checked();
            talk_group = 0x2B;
        }
        break;
    case 5:
    case 9:
    case 11:
    case 13:
    case 15:
    case 19:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x66;
                break;
            case 2:
                talk_group = 0x6A;
                break;
            case 3:
                talk_group = 0x6E;
                break;
            case 4:
                talk_group = 0x72;
                break;
            }
            culture_shiori_visit_script_start();
        }
        break;
    case 10:
        if (girl_state_rank(0) == 4) {
            mode_step_set(0x10);
        } else {
            if ((u16) game_state.clubs[4].practice_points < 0x64U) {
                talk_group += 1;
            }
            mode_step_next();
        }
        break;
    case 12:
        girl_rank_2 = girl_state_rank(0);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x69;
            break;
        case 2:
            talk_group = 0x6D;
            break;
        case 3:
            talk_group = 0x71;
            break;
        }
        mode_step_next();
        break;
    case 14:
        girl_rank_3 = girl_state_rank(0);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 1;
            break;
        case 2:
            talk_group = 0x76;
            break;
        case 3:
            talk_group = 3;
            break;
        }
        mode_step_next();
        break;
    case 16:
        normal_date_girl_out();
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        talk_group = 0;
        mode_step_next();
        break;
    case 20:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void culture_shiori_visit_open_sketching(void) {
    u32 girl_rank;
    u32 girl_rank_2;
    u32 girl_rank_3;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        mode_step_next();
        break;
    case 1:
    case 15:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x544A);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            portrait_show_checked();
            talk_group = 0x2C;
        }
        break;
    case 5:
    case 9:
    case 11:
    case 14:
    case 17:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x73;
                break;
            case 2:
                talk_group = 0x74;
                break;
            case 3:
                talk_group = 0x79;
                break;
            case 4:
                talk_group = 0x7A;
                break;
            }
            culture_shiori_visit_script_start();
        }
        break;
    case 10:
        mode_step_next();
        girl_rank_2 = girl_state_rank(0);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 1;
            break;
        case 2:
            talk_group = 0x75;
            break;
        case 3:
            talk_group = 3;
            break;
        case 4:
            talk_group = 0x7B;
            mode_step_set(0xC);
            break;
        }
        break;
    case 12:
        normal_date_girl_out();
        break;
    case 13:
        girl_rank_3 = girl_state_rank(0);
        switch (girl_rank_3) {
        case 0:
        case 1:
            mode_step_set(0xF);
            break;
        case 2:
        case 3:
            talk_group = 0x78;
            mode_step_next();
            break;
        case 4:
            mode_step_next();
            break;
        }
        break;
    case 16:
        talk_group = 0;
        mode_step_next();
        break;
    case 18:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void culture_shiori_visit_oil_painting_exhibit(void) {
    u32 girl_rank;
    u32 girl_rank_2;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        mode_step_next();
        break;
    case 1:
    case 13:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x544A);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            portrait_show_checked();
            talk_group = 0x2D;
        }
        break;
    case 5:
    case 9:
    case 15:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x7C;
                break;
            case 2:
                talk_group = 0x7D;
                break;
            case 3:
                talk_group = 0x7E;
                break;
            case 4:
                talk_group = 0x7F;
                break;
            }
            culture_shiori_visit_script_start();
        }
        break;
    case 10:
        girl_rank_2 = girl_state_rank(0);
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
            talk_group = 0x80;
            break;
        }
        mode_step_next();
        break;
    case 11:
        if (girl_state_rank(0) == 4) {
            normal_date_girl_out();
        } else {
            normal_date_speak();
        }
        break;
    case 12:
        if (girl_state_rank(0) == 4) {
            normal_date_speak();
        } else {
            normal_date_girl_out();
        }
        break;
    case 14:
        talk_group = 0;
        mode_step_next();
        break;
    case 16:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void culture_shiori_visit_concert(void) {
    GameState *g;
    u32 girl_rank;
    u32 girl_rank_2;

    g = &game_state;
    switch (g->mode_step) {
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
        if (g->year == 0x5F) {
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
        saved_return_stage = (s32) g->mode_stage;
        saved_return_step = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x544A);
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            culture_backstage_title_show();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            g->current_girl = GIRL_SHIORI;
            portrait_show_checked();
            cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        }
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x81;
                break;
            case 2:
                talk_group = 0x82;
                break;
            case 3:
                talk_group = 0x83;
                break;
            case 4:
                talk_group = 0x84;
                break;
            }
            if (g->year == 0x60) {
                talk_group += 7;
            }
            if (g->year == 0x61) {
                talk_group += 0xB;
            }
            culture_shiori_visit_script_start();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        girl_rank_2 = girl_state_rank(0);
        switch (girl_rank_2) {
        case 0:
            if (g->year == 0x5F) {
                talk_group = 0x85;
            } else {
                talk_group = 4;
            }
            break;
        case 1:
            if (g->year == 0x5F) {
                talk_group = 0x86;
            } else {
                talk_group = 5;
            }
            break;
        case 2:
            talk_group = 2;
            break;
        case 3:
            talk_group = 3;
            break;
        case 4:
            talk_group = 7;
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
        normal_date_bg_fadeout();
        break;
    case 17:
        talk_group = 0;
        mode_step_next();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void culture_shiori_visit_script_start(void) {
    talk_names_set(0);
    culture_script_set_shiori_visit();
    talk_line = 0;
    talk_text_table = culture_script_text_table;
    talk_command_table = culture_script_command_table;
    talk_voice_table = culture_script_voice_table;
    mode_step_next();
}
