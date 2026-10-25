/* mio_member.c: src/ovl/BUNKASAI/801354B0.c:4: Mio member block, stages 42-45. Original object 0x801354B0-0x801362A0 (config/objects/BUNKASAI.txt). */
#include "common.h"
#include "ovl/BUNKASAI.h"

void culture_mio_member_entry(void) {
    CultureFestivalDispatch *work = (CultureFestivalDispatch *)&D_80122EB8;
    s32 selector = work->unk_00;
    work->unk_04 = 1;
    game_state.girls[1].flags.f.b2 = 1;
    switch (selector) {
    case 0:
        if (game_state.year == 0x61) {
            mode_stage_set(0x2C);
            return;
        }
        mode_stage_set(0x2B);
        return;
    case 1:
        mode_stage_set(0x2D);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void culture_mio_member_literature_exhibit(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
            portrait_read_step(0x5D41);
        }
        break;
    case 3:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        culture_exhibit_title_show();
        addr_init_bustup();
        game_state.current_girl = GIRL_MIO;
        portrait_show_checked();
        cd_read_sectors(0xBU, 0x801E8000, 0x4AA9);
        break;
    case 4:
        normal_date_bg_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(1);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 2;
                break;
            case 2:
                talk_group = 4;
                break;
            case 3:
                talk_group = 6;
                break;
            case 4:
                talk_group = 8;
                break;
            }
            if (game_state.year == 0x60) {
                talk_group += 8;
            }
            culture_mio_member_script_start();
        }
        break;
    case 6:
        normal_date_girl_in();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        normal_date_bggirl_fadeout();
        talk_group = 0;
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        temp_v0_2 = girl_state_rank(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x12;
            break;
        case 2:
            talk_group = 0x14;
            break;
        case 3:
            talk_group = 0x16;
            break;
        case 4:
            talk_group = 0x18;
            break;
        }
        if ((u16) game_state.clubs[0].practice_points < (u32) ((game_state.year * 0x64) - 0x24B8)) {
            talk_group += 1;
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
        if (girl_state_rank(1) == 4) {
            if ((u16) game_state.clubs[0].practice_points < (u32) ((game_state.year * 0x64) - 0x24B8)) {
                talk_group = 0x3D;
            } else {
                talk_group = 0x3E;
            }
            mode_step_next();
        } else {
            mode_step_set(0x11);
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        talk_group = 1;
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

void culture_mio_member_speech_contest(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
            portrait_read_step(0x5D41);
        }
        break;
    case 3:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        culture_backstage_title_show();
        addr_init_bustup();
        game_state.current_girl = GIRL_MIO;
        portrait_show_checked();
        cd_read_sectors(0xBU, 0x801E8000, 0x4AA9);
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(1);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x1A;
                break;
            case 2:
                talk_group = 0x1C;
                break;
            case 3:
                talk_group = 0x1E;
                break;
            case 4:
                talk_group = 0x20;
                break;
            }
            culture_mio_member_script_start();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_bggirl_fadeout();
        break;
    case 8:
        D_80122EAC = 3;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 9:
        portrait_read_step(0x5D41);
        break;
    case 10:
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_MIO;
        portrait_show_checked();
        cd_read_sectors(0xBU, 0x801E8000, 0x4AA9);
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(1);
            switch (temp_v0_2) {
            case 0:
            case 1:
                talk_group = 0x1B;
                break;
            case 2:
                talk_group = 0x1D;
                break;
            case 3:
                talk_group = 0x1F;
                break;
            case 4:
                talk_group = 0x21;
                break;
            }
            culture_mio_member_script_start();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_girl_out();
        break;
    case 15:
        normal_date_bg_fadeout();
        break;
    case 16:
        talk_group = 1;
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

void culture_mio_member_drama_play(void) {
    GameState *state;
    u32 choice;
    u32 choice2;
    u32 choice3;

    state = &game_state;
    switch (state->mode_step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            portrait_read_step(0x5D6E);
        }
        break;
    case 3:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        culture_backstage_title_show();
        addr_init_bustup();
        state->current_girl = GIRL_MIO;
        portrait_show_checked();
        cd_read_sectors(0xBU, 0x801E8000, 0x4AA9);
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            if (state->year == 0x60) {
                choice = girl_state_rank(1);
                switch (choice) {
                case 0:
                case 1:
                    talk_group = 0x26;
                    break;
                case 2:
                case 3:
                    talk_group = 0x27;
                    break;
                case 4:
                    talk_group = 0x28;
                    break;
                }
            } else {
                choice2 = girl_state_rank(1);
                switch (choice2) {
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
                if (state->year == 0x61) {
                    talk_group += 7;
                }
            }
            culture_mio_member_script_start();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if ((state->year == 0x61) && (girl_state_rank(1) == 4)) {
            talk_group = 0x3E;
            normal_date_girl_out();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 8:
        if ((state->year == 0x61) && (girl_state_rank(1) == 4)) {
            normal_date_speak();
        } else {
            mode_step_set(0xA);
        }
        break;
    case 9:
        normal_date_bg_fadeout();
        break;
    case 10:
        D_80122EB0 = (s32) state->mode_stage;
        D_80122EB4 = state->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 11:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x40F4, 1);
        mode_step_next();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            portrait_read_step(0x5D6E);
        }
        break;
    case 13:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        state->current_girl = GIRL_MIO;
        portrait_show_checked();
        cd_read_sectors(0xBU, 0x801E8000, 0x4AA9);
        break;
    case 14:
        normal_date_bggirl_fadein();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            choice3 = girl_state_rank(1);
            switch (choice3) {
            case 0:
            case 1:
                talk_group = 0x2D;
                break;
            case 2:
                talk_group = 0x2F;
                break;
            case 3:
                talk_group = 0x31;
                break;
            case 4:
                talk_group = 0x33;
                break;
            }
            if ((u16) state->clubs[1].practice_points < (u32) ((state->year * 0x64) - 0x24B8)) {
                talk_group += 1;
            }
            if (state->year == 0x61) {
                talk_group = talk_group + 8;
            }
            culture_mio_member_script_start();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        if (girl_state_rank(1) == 4) {
            if (((u8) state->year < 0x61U) && ((u16) state->clubs[1].practice_points >= (u32) ((state->year * 0x64) - 0x24B8))) {
                talk_group = 0x3E;
                mode_step_next();
            } else if (state->year == 0x61) {
                if ((u16) state->clubs[1].practice_points >= 0x12CU) {
                    talk_group = 0x3E;
                    mode_step_next();
                } else {
                    talk_group = 0x3F;
                    mode_step_next();
                }
            } else {
                mode_step_set(0x14);
            }
        } else {
            mode_step_set(0x14);
        }
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        normal_date_bg_fadeout();
        break;
    case 21:
        talk_group = 1;
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

void culture_mio_member_script_start(void) {
    talk_names_set(1);
    culture_script_set_mio_member();
    talk_line = 0;
    talk_text_table = culture_script_text_table;
    talk_command_table = culture_script_command_table;
    talk_voice_table = culture_script_voice_table;
    mode_step_next();
}
