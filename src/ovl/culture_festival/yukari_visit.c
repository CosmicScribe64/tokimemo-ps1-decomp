/* yukari_visit.c: src/ovl/BUNKASAI/8014ABC0.c:4: Yukari visit block, stages 97-108. Original object 0x8014ABC0-0x8014DD90 (config/objects/BUNKASAI.txt). */
#include "common.h"
#include "ovl/culture_festival.h"

void culture_yukari_visit_entry(void) {
    CultureFestivalDispatch *dispatch = (CultureFestivalDispatch *)&culture_exhibit_club;
    dispatch->girl = 5;
    switch (dispatch->club_id) {
    case 0:
        if (game_state.year == 0x61) {
            mode_stage_set(CULTURE_STAGE_YUKARI_VISIT_SPEECH_CONTEST);
            return;
        }
        mode_stage_set(CULTURE_STAGE_YUKARI_VISIT_LITERATURE_EXHIBIT);
        return;
    case 1:
        mode_stage_set(CULTURE_STAGE_YUKARI_VISIT_DRAMA_PLAY);
        return;
    case 2:
        mode_stage_set(CULTURE_STAGE_YUKARI_VISIT_EXPERIMENT_SHOW);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(CULTURE_STAGE_YUKARI_VISIT_COMPUTER_FORTUNE);
            return;
        case 0x60:
            mode_stage_set(CULTURE_STAGE_YUKARI_VISIT_COMPUTER_GAME);
            return;
        case 0x61:
            mode_stage_set(CULTURE_STAGE_YUKARI_VISIT_EXPERIMENT_SHOW);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(CULTURE_STAGE_YUKARI_VISIT_PORTRAIT_DRAWING);
            return;
        case 0x60:
            mode_stage_set(CULTURE_STAGE_YUKARI_VISIT_OPEN_SKETCHING);
            return;
        case 0x61:
            mode_stage_set(CULTURE_STAGE_YUKARI_VISIT_OIL_PAINTING_EXHIBIT);
            return;
        }
        break;
    case 5:
        mode_stage_set(CULTURE_STAGE_YUKARI_VISIT_CONCERT);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void culture_yukari_visit_literature_exhibit(void) {
    u32 girl_rank;
    u32 girl_rank_2;
    u32 girl_rank_3;

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
            portrait_read_start(0x5FB7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUKARI;
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
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
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
            culture_yukari_visit_script_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        girl_rank_2 = girl_state_rank(5);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x10;
            break;
        case 2:
            talk_group = 0x11;
            break;
        case 3:
            talk_group = 0x12;
            break;
        case 4:
            talk_group = 0x13;
            break;
        }
        if (game_state.year == 0x60) {
            talk_group += 4;
        }
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        girl_rank_3 = girl_state_rank(5);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 8;
            break;
        case 2:
            talk_group = 9;
            break;
        case 3:
            talk_group = 0xA;
            break;
        case 4:
            talk_group = 0x36;
            mode_step_next();
            break;
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
        if (girl_state_rank(5) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 16:
        mode_step_next();
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
        if (girl_state_rank(5) != 4) {
            if (game_state.year == 0x5F) {
                game_state.girls[5].flags.f.b12 = 1;
            } else {
                game_state.girls[5].flags.f.b13 = 1;
            }
        }
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yukari_visit_speech_contest(void) {
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
        portrait_read_step(0x5FB7);
        break;
    case 9:
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
            case 0:
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
                talk_group = 6;
                break;
            }
            culture_yukari_visit_script_start();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        girl_rank_2 = girl_state_rank(5);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x18;
            break;
        case 2:
            talk_group = 0x19;
            break;
        case 3:
            talk_group = 0x1A;
            break;
        case 4:
            talk_group = 0xE;
            break;
        }
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if ((girl_state_rank(5) == 4) || (girl_state_rank(5) == 2)) {
            mode_step_set(0x12);
        } else {
            mode_step_next();
        }
        break;
    case 16:
        if (girl_state_rank(5) == 3) {
            talk_group = 0xA;
        } else {
            talk_group = 8;
        }
        mode_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        normal_date_girl_out();
        break;
    case 19:
        if (girl_state_rank(5) == 4) {
            mode_step_set(0x15);
        } else {
            talk_group = 0x6F;
            mode_step_next();
        }
        break;
    case 20:
        normal_date_speak();
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

void culture_yukari_visit_drama_play(void) {
    s32 girl_rank_3;
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
        if (game_state.year == 0x5F) {
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
            portrait_read_step(0x5FB7);
        }
        break;
    case 9:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
            case 0:
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
                talk_group = 6;
                break;
            }
            culture_yukari_visit_script_start();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        girl_rank_2 = girl_state_rank(5);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = (game_state.year * 3) - 0x102;
            break;
        case 2:
            talk_group = (game_state.year * 3) - 0x101;
            break;
        case 3:
            talk_group = (game_state.year * 3) - 0x100;
            break;
        case 4:
            talk_group = 0xE;
            break;
        }
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (girl_state_rank(5) == 4) {
            mode_step_set(0x12);
        } else {
            mode_step_next();
        }
        break;
    case 16:
        girl_rank_3 = girl_state_rank(5);
        switch (girl_rank_3) {                        /* switch 4; irregular */
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
        }
        mode_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        normal_date_girl_out();
        break;
    case 19:
        if (girl_state_rank(5) == 4) {
            mode_step_set(0x15);
        } else {
            talk_group = 0x6F;
            mode_step_next();
        }
        break;
    case 20:
        normal_date_speak();
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

void culture_yukari_visit_experiment_show(void) {
    s32 girl_rank_4;
    u32 girl_rank;
    u32 girl_rank_2;
    u32 girl_rank_3;

    GameState *g = &game_state;

    switch (g->mode_step) {
    case 0:
        if (((u16) g->player_flags.h >> 0xC) == 2) {
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
            portrait_read_start(0x5FB7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_YUKARI;
            portrait_show_checked();
            talk_group = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
            case 0:
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
            culture_yukari_visit_script_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        talk_group = 0x24;
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        girl_rank_2 = girl_state_rank(5);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x25;
            break;
        case 2:
            talk_group = 0x26;
            break;
        case 3:
            talk_group = 0x27;
            break;
        case 4:
            talk_group = 0x28;
            break;
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
        saved_return_stage = (s32) g->mode_stage;
        saved_return_step = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 16:
        portrait_read_step(0x5FB7);
        break;
    case 17:
        culture_exhibit_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 18:
        if (girl_state_rank(5) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            girl_rank_3 = girl_state_rank(5);
            switch (girl_rank_3) {
            case 0:
            case 1:
                if (((u16) g->player_flags.h >> 0xC) == 3) {
                    talk_group = 0x32;
                } else {
                    talk_group = (g->year * 3) - 0xF4;
                }
                break;
            case 2:
                if (((u16) g->player_flags.h >> 0xC) == 3) {
                    talk_group = 0x33;
                } else {
                    talk_group = (g->year * 3) - 0xF3;
                }
                break;
            case 3:
                if (((u16) g->player_flags.h >> 0xC) == 3) {
                    talk_group = 0x34;
                } else {
                    talk_group = (g->year * 3) - 0xF2;
                }
                break;
            case 4:
                talk_group = 0xF;
            }
            culture_yukari_visit_script_start();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (girl_state_rank(5) == 4) {
            mode_step_set(0x1A);
        } else {
            girl_rank_4 = girl_state_rank(5);
            switch (girl_rank_4) {                    /* switch 5; irregular */
            case 0:
            case 1:
                talk_group = 8;
                break;
            case 2:
                talk_group = 9;
                break;
            case 3:
                talk_group = 0xA;
                break;
            }
            mode_step_next();
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        normal_date_girl_out();
        break;
    case 24:
        if (((u16) g->player_flags.h >> 0xC) == 3) {
            talk_group = 0x35;
        } else {
            talk_group = 0x6F;
        }
        mode_step_next();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        normal_date_bg_fadeout();
        break;
    case 27:
        talk_group = 0;
        mode_step_next();
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yukari_visit_computer_fortune(void) {
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
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x5FB7);
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
            game_state.current_girl = GIRL_YUKARI;
            portrait_show_checked();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
            case 0:
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
            culture_yukari_visit_script_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        girl_rank_2 = girl_state_rank(5);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x37;
            break;
        case 2:
            talk_group = 0x38;
            break;
        case 3:
            talk_group = 0x39;
            break;
        case 4:
            talk_group = 0x3A;
            break;
        }
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_girl_out();
        break;
    case 13:
        if (girl_state_rank(5) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 14:
        audio_command(0x202);
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        mode_stage_next();
        break;
    case 15:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        mode_step_next();
        break;
    case 16:
        if (girl_state_rank(5) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            culture_yukari_visit_script_start();
        }
        break;
    case 18:
        mode_step_next();
        break;
    case 19:
        talk_group = fortune_result_index + 0x3D;
        if (girl_state_rank(5) == 4) {
            talk_group += 3;
        }
        mode_step_next();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        mode_step_next();
        girl_rank_3 = girl_state_rank(5);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 0x43;
            break;
        case 2:
            talk_group = 0x44;
            break;
        case 3:
            talk_group = 0x45;
            break;
        case 4:
            mode_step_set(0x1C);
            break;
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        girl_rank_4 = girl_state_rank(5);
        switch (girl_rank_4) {                        /* switch 5; irregular */
        case 0:
        case 1:
            talk_group = 8;
            break;
        case 2:
            talk_group = 9;
            break;
        case 3:
            talk_group = 0xA;
            break;
        }
        mode_step_next();
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        normal_date_girl_out();
        break;
    case 26:
        talk_group = 0x6F;
        mode_step_next();
        break;
    case 27:
        normal_date_speak();
        break;
    case 28:
        normal_date_bg_fadeout();
        break;
    case 29:
        talk_group = 0;
        mode_step_next();
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yukari_visit_computer_game(void) {
    s32 girl_rank_3;
    u32 girl_rank;
    u32 girl_rank_2;
    u32 girl_rank_4;
    u32 girl_rank_5;

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
        portrait_read_step(0x5FB7);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
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
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
            case 0:
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
            culture_yukari_visit_script_start();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        talk_group = 0x46;
        mode_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        girl_rank_2 = girl_state_rank(5);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x47;
            break;
        case 2:
            talk_group = 0x48;
            break;
        case 3:
            talk_group = 0x49;
            break;
        case 4:
            talk_group = 0x4A;
            break;
        }
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (girl_state_rank(5) == 4) {
            normal_date_girl_out();
        } else {
            mode_step_set(0x12);
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        mode_step_set(0x1D);
        break;
    case 18:
        normal_date_bggirl_fadeout();
        break;
    case 19:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->stage_select = 1;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_TT_GAME);
        break;
    case 20:
        culture_festival_return_gate = 1;
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        mode_stage_set(CULTURE_STAGE_GAME_RESUME);
        break;
    case 21:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        culture_exhibit_title_show();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(5U);
        culture_festival_return_gate = 0;
        mode_step_next();
        break;
    case 22:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 23:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 24:
        portrait_read_step(0x5FB7);
        break;
    case 25:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 26:
        normal_date_bggirl_fadein();
        break;
    case 27:
        if (cd_read_sync_status() & 1) {
            girl_rank_3 = girl_state_rank(5);
            switch (girl_rank_3) {                    /* switch 4; irregular */
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
            }
            culture_yukari_visit_script_start();
        }
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        if (girl_state_rank(5) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 30:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->stage_select = 0;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        mode_stage_set(CULTURE_STAGE_GAME_LAUNCH);
        break;
    case 31:
        culture_festival_return_gate = 1;
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        mode_stage_set(CULTURE_STAGE_GAME_RESUME);
        break;
    case 32:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        culture_exhibit_title_show();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(5U);
        culture_festival_return_gate = 0;
        mode_step_next();
        break;
    case 33:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 34:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 35:
        portrait_read_step(0x5FB7);
        break;
    case 36:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 37:
        if (girl_state_rank(5) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 38:
        if (cd_read_sync_status() & 1) {
            girl_rank_4 = girl_state_rank(5);
            switch (girl_rank_4) {
            case 0:
            case 1:
                talk_group = 0x52;
                break;
            case 2:
                talk_group = 0x53;
                break;
            case 3:
                talk_group = 0x54;
                break;
            case 4:
                talk_group = 0x55;
                break;
            }
            culture_yukari_visit_script_start();
        }
        break;
    case 39:
        normal_date_speak();
        break;
    case 40:
        mode_step_next();
        girl_rank_5 = girl_state_rank(5);
        switch (girl_rank_5) {
        case 0:
        case 1:
            talk_group = 8;
            break;
        case 2:
            talk_group = 9;
            break;
        case 3:
            talk_group = 0xA;
            break;
        case 4:
            mode_step_set(0x2B);
            break;
        }
        break;
    case 41:
        normal_date_speak();
        break;
    case 42:
        talk_group = 0x6F;
        normal_date_girl_out();
        break;
    case 43:
        if (girl_state_rank(5) == 4) {
            normal_date_bg_fadeout();
        } else {
            normal_date_speak();
        }
        break;
    case 44:
        talk_group = 0;
        if (girl_state_rank(5) == 4) {
            mode_step_next();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 45:
        normal_date_speak();
        break;
    case 46:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yukari_visit_portrait_drawing(void) {
    s32 girl_rank_3;
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
            portrait_read_start(0x5FB7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUKARI;
            portrait_show_checked();
            talk_group = 0x2B;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
            case 0:
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
            culture_yukari_visit_script_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        talk_group = 0x57;
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        girl_rank_2 = girl_state_rank(5);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x58;
            break;
        case 2:
            talk_group = 0x59;
            break;
        case 3:
            talk_group = 0x5A;
            break;
        case 4:
            talk_group = 0x5B;
            break;
        }
        mode_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if (girl_state_rank(5) == 4) {
            mode_step_set(0x10);
        } else {
            girl_rank_3 = girl_state_rank(5);
            switch (girl_rank_3) {                    /* switch 4; irregular */
            case 0:
            case 1:
                talk_group = 8;
                break;
            case 2:
                talk_group = 9;
                break;
            case 3:
                talk_group = 0xA;
                break;
            }
            mode_step_next();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_girl_out();
        break;
    case 17:
        mode_step_next();
        break;
    case 18:
        mode_step_next();
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

void culture_yukari_visit_open_sketching(void) {
    u32 girl_rank;
    u32 girl_rank_2;

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
            portrait_read_start(0x5FB7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUKARI;
            portrait_show_checked();
            talk_group = 0x2C;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
            case 0:
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
            culture_yukari_visit_script_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        talk_group = 0x5C;
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        girl_rank_2 = girl_state_rank(5);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x5D;
            break;
        case 2:
            talk_group = 0x5E;
            break;
        case 3:
            talk_group = 0x5F;
            break;
        case 4:
            talk_group = 0x60;
            break;
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
        talk_group = 0x61;
        mode_step_next();
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
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yukari_visit_oil_painting_exhibit(void) {
    u32 girl_rank;
    u32 girl_rank_2;
    u32 girl_rank_3;

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
            portrait_read_start(0x5FB7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUKARI;
            portrait_show_checked();
            talk_group = 0x2D;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
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
            culture_yukari_visit_script_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        girl_rank_2 = girl_state_rank(5);
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
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        girl_rank_3 = girl_state_rank(5);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 8;
            break;
        case 2:
            talk_group = 9;
            break;
        case 3:
            talk_group = 0xA;
            break;
        case 4:
            talk_group = 0x36;
            mode_step_next();
            break;
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
        if (girl_state_rank(5) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 16:
        mode_step_next();
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

void culture_yukari_visit_concert(void) {
    u32 girl_rank;
    u32 girl_rank_2;
    u32 girl_rank_3;

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
        portrait_read_step(0x5FB7);
        break;
    case 9:
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
            case 0:
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
                talk_group = 6;
                break;
            }
            culture_yukari_visit_script_start();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        girl_rank_2 = girl_state_rank(5);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = (game_state.year * 3) - 0xB7;
            break;
        case 2:
            talk_group = (game_state.year * 3) - 0xB6;
            break;
        case 3:
            talk_group = (game_state.year * 3) - 0xB5;
            break;
        case 4:
            talk_group = 0xE;
            break;
        }
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        mode_step_next();
        girl_rank_3 = girl_state_rank(5);
        switch (girl_rank_3) {
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
            mode_step_set(0x11);
            break;
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        talk_group = 0x6F;
        mode_step_next();
        break;
    case 19:
        if (girl_state_rank(5) == 4) {
            mode_step_next();
        } else {
            normal_date_speak();
        }
        break;
    case 20:
        normal_date_bg_fadeout();
        break;
    case 21:
        talk_group = 0;
        mode_step_next();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_yukari_visit_script_start(void) {
    talk_names_set(5);
    culture_script_set_yukari_visit();
    talk_line = 0;
    talk_text_table = culture_script_text_table;
    talk_command_table = culture_script_command_table;
    talk_voice_table = culture_script_voice_table;
    mode_step_next();
}
