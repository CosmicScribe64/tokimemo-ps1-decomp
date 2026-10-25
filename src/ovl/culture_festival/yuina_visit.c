/* yuina_visit.c: src/ovl/BUNKASAI/80132480.c:4: Yuina visit block, stages 46-58. Original object 0x80132480-0x801354B0 (config/objects/BUNKASAI.txt). */
#include "common.h"
#include "ovl/culture_festival.h"

void culture_yuina_visit_entry(void) {
    CultureFestivalDispatch *dispatch = (CultureFestivalDispatch *)&culture_exhibit_club;
    dispatch->girl = 2;
    switch (dispatch->club_id) {
    case 0:
        if (game_state.year == 0x61) {
            mode_stage_set(CULTURE_STAGE_YUINA_VISIT_SPEECH_CONTEST);
            return;
        }
        mode_stage_set(CULTURE_STAGE_YUINA_VISIT_LITERATURE_EXHIBIT);
        return;
    case 1:
        mode_stage_set(CULTURE_STAGE_YUINA_VISIT_DRAMA_PLAY);
        return;
    case 2:
        mode_stage_set(CULTURE_STAGE_YUINA_VISIT_EXPERIMENT_SHOW);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(CULTURE_STAGE_YUINA_VISIT_COMPUTER_FORTUNE);
            return;
        case 0x60:
            mode_stage_set(CULTURE_STAGE_YUINA_VISIT_COMPUTER_GAME);
            return;
        case 0x61:
            mode_stage_set(CULTURE_STAGE_YUINA_VISIT_COMPUTER_EXPERIMENT_SHOW);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(CULTURE_STAGE_YUINA_VISIT_PORTRAIT_DRAWING);
            return;
        case 0x60:
            mode_stage_set(CULTURE_STAGE_YUINA_VISIT_OPEN_SKETCHING);
            return;
        case 0x61:
            mode_stage_set(CULTURE_STAGE_YUINA_VISIT_OIL_PAINTING_EXHIBIT);
            return;
        }
        break;
    case 5:
        mode_stage_set(CULTURE_STAGE_YUINA_VISIT_CONCERT);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

/* T-12220: bit-field stores preserve the original byte-load register allocation. */
void culture_yuina_visit_literature_exhibit(void) {
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
            portrait_read_start(0x522E);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUINA;
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
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(2);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 1;
                break;
            case 2:
                talk_group = 3;
                break;
            case 3:
                talk_group = 5;
                break;
            case 4:
                talk_group = 7;
                break;
            }
            if (game_state.year == 0x60) {
                talk_group += 8;
            }
            culture_yuina_visit_script_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_girl_out();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_bg_fadeout();
        break;
    case 13:
        talk_group = 0;
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (girl_state_rank(2) < 4) {
            if (game_state.year == 0x5F) {
                game_state.girls[2].flags.f.b12 = 1;
            } else {
                game_state.girls[2].flags.f.b13 = 1;
            }
        }
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yuina_visit_speech_contest(void) {
    u32 girl_rank;

    GameState *g = &game_state;

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
        saved_return_stage = (s32) g->mode_stage;
        saved_return_step = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x522E);
        break;
    case 9:
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_YUINA;
        portrait_show_checked();
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        break;
    case 10:
        if (girl_state_rank(2) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            culture_yuina_visit_script_start();
            girl_rank = girl_state_rank(2);
            switch (girl_rank) {
            case 0:
            case 1:
            case 2:
                break;
            case 3:
                mode_step_set(0x11);
                break;
            case 4:
                talk_group = 0x1E;
                mode_step_set(0x1E);
                break;
            }
        }
        break;
    case 12:
        if (girl_state_rank(2) < 2) {
            talk_group = 0x11;
        } else {
            talk_group = 0x13;
        }
        mode_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_girl_out();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        mode_step_set(0x23);
        break;
    case 17:
        talk_group = 0x15;
        mode_step_next();
        /* fallthrough */
    case 18:
        normal_date_speak();
        break;
    case 19:
        if (!(g->girls[2].syoushin & 3)) {
            talk_group = 0x21;
            mode_step_set(0x1A);
        } else {
            if (((u8) g->girls[2].flags.b[2] >> 4) == 2) {
                talk_group = 0x16;
            } else {
                talk_group = 0x17;
            }
            mode_step_next();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (g->girls[2].flags.f.b2) {
            talk_group = 0x1A;
            mode_step_set(0x18);
        } else {
            if (((u8) g->girls[2].flags.b[2] >> 4) == 2) {
                talk_group = 0x18;
            } else {
                talk_group = 0x19;
            }
            g->girls[2].flags.f.b2 = 1;
            mode_step_next();
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        talk_group = 0x1B;
        mode_step_next();
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        talk_group = 0x1C;
        mode_step_next();
        /* fallthrough */
    case 26:
        normal_date_speak();
        break;
    case 27:
        normal_date_girl_out();
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        mode_step_set(0x23);
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        normal_date_girl_in();
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        normal_date_girl_out();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        normal_date_bg_fadeout();
        break;
    case 36:
        talk_group = 0;
        mode_step_next();
        break;
    case 37:
        normal_date_speak();
        break;
    case 38:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yuina_visit_drama_play(void) {
    s32 girl_rank;

    GameState *g = &game_state;

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
        if (g->year == 0x5F) {
            talk_group = 0x27;
        } else {
            talk_group = 0x28;
        }
        break;
    case 4:
        normal_date_speak();
        break;
    case 5:
        normal_date_bg_fadeout();
        break;
    case 6:
        saved_return_stage = (s32) g->mode_stage;
        saved_return_step = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 7:
        dec_bg_reset();
        set_dec_bri(0);
        mode_step_next();
        break;
    case 8:
        portrait_read_step(0x522E);
        break;
    case 9:
        addr_init_bustup();
        g->current_girl = GIRL_YUINA;
        portrait_show_checked();
        dec_bg_cd_read(0x40F4, 1);
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            culture_backstage_title_show();
            D_80125CA0 = 0xC0;
            cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
            mode_step_next();
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            culture_yuina_visit_script_start();
            if (girl_state_rank(2) == 4) {
                talk_group = 0x29;
                mode_step_set(0x15);
            }
        }
        break;
    case 12:
        normal_date_bggirl_fadein();
        break;
    case 13:
        mode_step_next();
        girl_rank = girl_state_rank(2);
        switch (girl_rank) {                          /* switch 2; irregular */
        case 0:
        case 1:
            switch (g->year) {                   /* switch 3; irregular */
            case 0x5F:
                talk_group = 0x23;
                break;
            case 0x60:
                talk_group = 0x2C;
                break;
            case 0x61:
                talk_group = 0x32;
                break;
            }
            break;
        case 2:
            switch (g->year) {                   /* switch 4; irregular */
            case 0x5F:
                talk_group = 0x25;
                break;
            case 0x60:
                talk_group = 0x2E;
                break;
            case 0x61:
                talk_group = 0x34;
                break;
            }
            break;
        case 3:
            switch (g->year) {                   /* switch 5; irregular */
            case 0x5F:
                talk_group = 0x27;
                break;
            case 0x60:
                talk_group = 0x30;
                break;
            case 0x61:
                talk_group = 0x36;
                mode_step_set(0x1C);
                break;
            }
            break;
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        normal_date_girl_out();
        break;
    case 16:
        normal_date_speak();
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
    case 21:
        normal_date_bg_fadein();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        normal_date_girl_in();
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        normal_date_girl_out();
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        mode_step_set(0x11);
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        if (!(g->girls[2].syoushin & 3)) {
            talk_group = 0x3F;
            mode_step_set(0x24);
        } else {
            if (((u8) g->girls[2].flags.b[2] >> 4) == 2) {
                talk_group = 0x37;
            } else {
                talk_group = 0x38;
            }
            mode_step_next();
        }
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        if (g->girls[2].flags.f.b2) {
            talk_group = 0x3B;
            mode_step_set(0x22);
        } else {
            if (((u8) g->girls[2].flags.b[2] >> 4) == 2) {
                talk_group = 0x39;
            } else {
                talk_group = 0x3A;
            }
            g->girls[2].flags.f.b2 = 1;
            mode_step_next();
        }
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        talk_group = 0x3C;
        mode_step_next();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        talk_group = 0x3D;
        mode_step_next();
        break;
    case 36:
        normal_date_speak();
        break;
    case 37:
        normal_date_girl_out();
        break;
    case 38:
        normal_date_speak();
        break;
    case 39:
        mode_step_set(0x11);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yuina_visit_experiment_show(void) {
    u32 girl_rank;
    u32 girl_rank_2;
    u32 girl_rank_3;
    u32 girl_rank_4;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x522E);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUINA;
            portrait_show_checked();
            talk_group = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(2);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x41;
                break;
            case 2:
                talk_group = 0x42;
                break;
            case 3:
                talk_group = 0x43;
                break;
            case 4:
                talk_group = 0x44;
                break;
            }
            if (game_state.year == 0x61) {
                talk_group += 4;
            }
            culture_yuina_visit_script_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if ((game_state.girls[2].flags.f.b2) && (game_state.year != 0x61)) {
            girl_rank_2 = girl_state_rank(2);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x49;
                break;
            case 2:
                talk_group = 0x4A;
                break;
            case 3:
                talk_group = 0x4B;
                break;
            case 4:
                talk_group = 0x4C;
                break;
            }
            mode_step_next();
        } else {
            mode_step_set(0xC);
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (game_state.year == 0x61) {
            girl_rank_3 = girl_state_rank(2);
            switch (girl_rank_3) {
            case 0:
            case 1:
                talk_group = 0x4F;
                break;
            case 2:
                talk_group = 0x50;
                break;
            case 3:
                talk_group = 0x51;
                break;
            case 4:
                talk_group = 0x52;
                break;
            }
        } else if (girl_state_rank(2) == 4) {
            talk_group = 0x4E;
        } else {
            talk_group = 0x4D;
        }
        mode_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        festival_dispatch_variant = 0;
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 16:
        portrait_read_step(0x522E);
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            game_state.current_girl = GIRL_YUINA;
            portrait_show_checked();
            cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        }
        break;
    case 18:
        if (girl_state_rank(2) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            girl_rank_4 = girl_state_rank(2);
            switch (girl_rank_4) {
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
            talk_group = (talk_group + (game_state.year * 7)) - 0x299;
            culture_yuina_visit_script_start();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (girl_state_rank(2) == 4) {
            mode_step_set(0x17);
        } else {
            normal_date_girl_out();
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        normal_date_bg_fadeout();
        break;
    case 24:
        talk_group = 0;
        mode_step_next();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yuina_visit_computer_fortune(void) {
    u32 girl_rank;
    u32 girl_rank_2;

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
            portrait_read_start(0x522E);
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
            game_state.current_girl = GIRL_YUINA;
            portrait_show_checked();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(2);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x68;
                break;
            case 2:
                talk_group = 0x69;
                break;
            case 3:
                talk_group = 0x6A;
                break;
            case 4:
                talk_group = 0x6C;
                break;
            }
            culture_yuina_visit_script_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_girl_out();
        break;
    case 11:
        if (girl_state_rank(2) < 3) {
            mode_step_next();
        } else {
            normal_date_speak();
        }
        break;
    case 12:
        audio_command(0x202);
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        mode_stage_next();
        break;
    case 13:
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        mode_step_next();
        break;
    case 14:
        if (girl_state_rank(2) >= 3) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(2);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = (fortune_result_index * 2) + 0x6E;
                break;
            case 2:
                talk_group = (fortune_result_index * 2) + 0x74;
                break;
            case 3:
            case 4:
                talk_group = fortune_result_index + 0x7A;
                break;
            }
            culture_yuina_visit_script_start();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if ((girl_state_rank(2) < 2) && (fortune_result_index != 1)) {
            audio_command(0x508);
        }
        mode_step_next();
        break;
    case 18:
        if (girl_state_rank(2) < 3) {
            normal_date_speak();
        } else {
            mode_step_set(0x17);
        }
        break;
    case 19:
        if (girl_state_rank(2) < 2) {
            talk_group = 0x7D;
        } else {
            talk_group = 0x7F;
        }
        mode_step_next();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        normal_date_girl_out();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        normal_date_bg_fadeout();
        break;
    case 24:
        talk_group = 0;
        mode_step_next();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yuina_visit_computer_game(void) {
    u32 girl_rank;
    u32 girl_rank_2;

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
        portrait_read_step(0x522E);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUINA;
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
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(2);
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
            culture_yuina_visit_script_start();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (girl_state_rank(2) == 4) {
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
        if (girl_state_rank(2) == 4) {
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
        talk_names_set(2U);
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
        portrait_read_step(0x522E);
        break;
    case 21:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUINA;
        portrait_show_checked();
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        break;
    case 22:
        if (girl_state_rank(2) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 23:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(2);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x86;
                break;
            case 2:
                talk_group = 0x88;
                break;
            case 3:
                talk_group = 0x8A;
                break;
            case 4:
                talk_group = 0x8C;
                break;
            }
            culture_yuina_visit_script_start();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        if (girl_state_rank(2) == 4) {
            mode_step_set(0x1B);
        } else {
            normal_date_girl_out();
        }
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        normal_date_bg_fadeout();
        break;
    case 28:
        talk_group = 0;
        mode_step_next();
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yuina_visit_computer_experiment_show(void) {
    u32 girl_rank;
    u32 girl_rank_2;

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
            portrait_read_start(0x522E);
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
            game_state.current_girl = GIRL_YUINA;
            portrait_show_checked();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(2);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x8E;
                break;
            case 2:
                talk_group = 0x8F;
                break;
            case 3:
                talk_group = 0x90;
                break;
            case 4:
                talk_group = 0x91;
                break;
            }
            culture_yuina_visit_script_start();
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
        portrait_read_step(0x522E);
        break;
    case 13:
        culture_exhibit_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUINA;
        portrait_show_checked();
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        break;
    case 14:
        if (girl_state_rank(2) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(2);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x92;
                break;
            case 2:
                talk_group = 0x94;
                break;
            case 3:
                talk_group = 0x96;
                break;
            case 4:
                talk_group = 0x97;
                break;
            }
            culture_yuina_visit_script_start();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (girl_state_rank(2) == 4) {
            mode_step_set(0x13);
        } else {
            if (girl_state_rank(2) == 3) {
                talk_group = 0x95;
            }
            normal_date_girl_out();
        }
        break;
    case 18:
        normal_date_speak();
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

void culture_yuina_visit_portrait_drawing(void) {
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
            portrait_read_start(0x522E);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUINA;
            portrait_show_checked();
            talk_group = 0x2B;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(2);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x98;
                break;
            case 2:
                talk_group = 0x9D;
                break;
            case 3:
                talk_group = 0xA2;
                break;
            case 4:
                talk_group = 0xA7;
                break;
            }
            culture_yuina_visit_script_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(2) == 4) {
            normal_date_girl_out();
        } else {
            if ((u16) game_state.clubs[4].practice_points < 0x64U) {
                portrait_layout_set(4U);
                talk_group += 2;
            }
            mode_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (girl_state_rank(2) == 4) {
            mode_step_set(0xE);
        } else {
            normal_date_girl_out();
        }
        break;
    case 13:
        normal_date_speak();
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

void culture_yuina_visit_open_sketching(void) {
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
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x522E);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUINA;
            portrait_show_checked();
            talk_group = 0x2C;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(2);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0xA9;
                break;
            case 2:
                talk_group = 0xAA;
                break;
            case 3:
                talk_group = 0xAB;
                break;
            case 4:
                talk_group = 0xAC;
                break;
            }
            culture_yuina_visit_script_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        talk_group = 0xAD;
        mode_step_next();
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

void culture_yuina_visit_oil_painting_exhibit(void) {
    u32 girl_rank;

    GameState *g = &game_state;

    switch (g->mode_step) {
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
            portrait_read_start(0x522E);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_YUINA;
            portrait_show_checked();
            talk_group = 0x2D;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(2);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0xAE;
                break;
            case 2:
                talk_group = 0xB0;
                break;
            case 3:
                talk_group = 0xB2;
                break;
            case 4:
                talk_group = 0xBB;
                break;
            }
            culture_yuina_visit_script_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(2) == 3) {
            mode_step_set(0x11);
        } else {
            mode_step_next();
        }
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
    case 17:
        if (!(g->girls[2].syoushin & 3)) {
            talk_group = 0xB9;
            mode_step_set(0x14);
        } else {
            if (!(g->girls[2].flags.f.b2)) {
                g->girls[2].flags.f.b2 = 1;
                if (((u8) g->girls[2].flags.b[2] >> 4) == 2) {
                    talk_group = 0xB5;
                } else {
                    talk_group = 0xB6;
                }
            } else if (((u8) g->girls[2].flags.b[2] >> 4) == 2) {
                talk_group = 0xB3;
            } else {
                talk_group = 0xB4;
            }
            mode_step_next();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        talk_group = 0xB7;
        mode_step_next();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        normal_date_girl_out();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        mode_step_set(0xD);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yuina_visit_concert(void) {
    s32 girl_rank;

    GameState *g = &game_state;

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
        portrait_read_step(0x522E);
        break;
    case 9:
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_YUINA;
        portrait_show_checked();
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        break;
    case 10:
        if (girl_state_rank(2) == 4) {
            talk_group = 0xC3;
            normal_date_bg_fadein();
        } else {
            mode_step_next();
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            culture_yuina_visit_script_start();
        }
        break;
    case 12:
        if (girl_state_rank(2) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 13:
        if (girl_state_rank(2) == 4) {
            normal_date_girl_in();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 14:
        girl_rank = girl_state_rank(2);
        switch (girl_rank) {                          /* switch 2; irregular */
        case 0:
        case 1:
            talk_group = 0xBD;
            break;
        case 2:
            talk_group = 0xBF;
            break;
        case 3:
            talk_group = 0xC1;
            break;
        }
        if (girl_state_rank(2) < 4) {
            if (g->year == 0x60) {
                talk_group += 9;
            }
            if (g->year == 0x61) {
                talk_group += 0xF;
            }
        }
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_girl_out();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        normal_date_bg_fadeout();
        break;
    case 19:
        talk_group = 0;
        mode_step_next();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yuina_visit_script_start(void) {
    talk_names_set(2);
    culture_script_set_yuina_visit();
    talk_line = 0;
    talk_text_table = culture_script_text_table;
    talk_command_table = culture_script_command_table;
    talk_voice_table = culture_script_voice_table;
    mode_step_next();
}
