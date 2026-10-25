/* birthday.c: src/ovl/BUNKAKEN/80148ED0.c:9: birthday phases 9 and 10 and their tables. Original object 0x80148ED0-0x8014A6A0 (config/objects/BUNKAKEN.txt). */
#define MAIN_API_OVERRIDE_talk_names_set /* T-18500: tour_phase_player_birthday passes the full word without a prototype. */
#define MAIN_API_OVERRIDE_portrait_layout_set /* T-18500: K&R call in tour_phase_player_birthday; the callee narrows the argument. */
#include "common.h"
#include "ovl/BUNKAKEN.h"

void talk_names_set();
void portrait_layout_set();

void tour_birthday_tables_init(void) {
    tour_birthday_script[0][0] = 0x801B00DC;
    tour_birthday_script[0][1] = 0x801B02AC;
    tour_birthday_script[0][2] = 0x801B03C8;
    tour_birthday_script[0][3] = 0x801B04C4;
    tour_birthday_script[0][4] = 0x801B0668;
    tour_birthday_script[0][5] = 0x801B0808;
    tour_birthday_script[0][6] = 0x801B09DC;
    tour_birthday_script[0][7] = 0x801B0BE8;
    tour_birthday_script[0][8] = 0x801B0D8C;
    tour_birthday_script[0][9] = 0x801B0F5C;
    tour_birthday_script[0][10] = 0x801B1164;
    tour_birthday_script[0][11] = 0x801B12D8;
    tour_birthday_script[0][12] = 0x801B1454;
    tour_birthday_script[1][0] = 0x801B00F4;
    tour_birthday_script[1][1] = 0x801B02C8;
    tour_birthday_script[1][2] = 0x801B03D0;
    tour_birthday_script[1][3] = 0x801B04DC;
    tour_birthday_script[1][4] = 0x801B0680;
    tour_birthday_script[1][5] = 0x801B0820;
    tour_birthday_script[1][6] = 0x801B09F8;
    tour_birthday_script[1][7] = 0x801B0C00;
    tour_birthday_script[1][8] = 0x801B0DA4;
    tour_birthday_script[1][9] = 0x801B0F78;
    tour_birthday_script[1][10] = 0x801B1180;
    tour_birthday_script[1][11] = 0x801B12E0;
    tour_birthday_script[1][12] = 0x801B1470;
    tour_birthday_script[2][0] = 0x801B01A8;
    tour_birthday_script[2][1] = 0x801B0398;
    tour_birthday_script[2][2] = 0x801B03F0;
    tour_birthday_script[2][3] = 0x801B0590;
    tour_birthday_script[2][4] = 0x801B0734;
    tour_birthday_script[2][5] = 0x801B08D4;
    tour_birthday_script[2][6] = 0x801B0AE4;
    tour_birthday_script[2][7] = 0x801B0CB4;
    tour_birthday_script[2][8] = 0x801B0E58;
    tour_birthday_script[2][9] = 0x801B1064;
    tour_birthday_script[2][10] = 0x801B126C;
    tour_birthday_script[2][11] = 0x801B1338;
    tour_birthday_script[2][12] = 0x801B155C;
}

void tour_phase_shiori_birthday(void) {
    u32 shiori_club;
    u32 girl_rank;
    u32 girl_rank_2;
    u32 girl_rank_3;

    switch (game_state.mode_step) {
    case 0x0:
        game_state.day_event_done_bits |= 1;
        k_reset(0);
        message_window_set_title((s32) "　");
        message_window_title_show(0);
        dec_bg_reset();
        set_dec_bri(0);
        audio_command(0xCF);
        cd_read_sectors(3U, 0x801E8000, 0x49B7);
        Vblnk_Timer_Init();
        mode_step_next();
        break;
    case 0x1:
        if (Vblnk_Timer() >= 0x15) {
            if (cd_read_sync_status() & 1) {
                audio_bank_load_start(1, 0xC478, 0xC44B, 0xCA95, 0xCA4F, 0xCA3E);
                game_state.current_girl = GIRL_SHIORI;
                talk_names_set(0U);
                tour_script_birthday();
                talk_group = 0;
                talk_line = 0;
                talk_text_table = (s32) tour_script_text;
                talk_command_table = (s32) tour_script_commands;
                talk_voice_table = (s32) tour_script_voices;
                mode_step_next();
            }
        }
        break;
    case 0x2:
        if ((u32) ((u8) game_state.girls[0].flags.b[2] >> 4) < 6U) {
            mode_step_set(0x2E);
        } else if (((game_state.player_birthday & 0xF) == game_state.month) && (((u32) (game_state.player_birthday << 0x17) >> 0x1B) == game_state.day) && (girl_state_rank(0) < 2)) {
            mode_step_set(0x16);
        } else {
            mode_step_next();
        }
        break;
    case 0x3:
        if (audio_bank_load_poll() == 1) {
            talk_group = 0x25;
            if (girl_state_rank(0) < 2) {
                dec_bg_cd_read(0x42D8, 1);
            } else {
                dec_bg_cd_read(0x403B, 1);
            }
            mode_step_next();
        }
        break;
    case 0x4:
        normal_date_speak();
        break;
    case 0x5:
        if (cd_read_sync_status() & 1) {
            audio_command(0x200);
            k_reset(0);
            if (girl_state_rank(0) < 2) {
                message_window_set_title((s32) "近所の公園");
                event_log_add(0x14, 0x19);
            } else {
                message_window_set_title((s32) "廊下");
                event_log_add(0x14, 0x1A);
            }
            D_80125CA0 = 0xC0;
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 0x6:
        normal_date_bggirl_fadein();
        break;
    case 0x7:
        talk_group = 0x28;
        mode_step_next();
        break;
    case 0x8:
        normal_date_speak();
        break;
    case 0x9:
        talk_group = game_state.year - 0x5C;
        mode_step_next();
        break;
    case 0xA:
        normal_date_speak();
        break;
    case 0xB:
        normal_date_three_select();
        break;
    case 0xC:
        girl_rank = girl_state_rank(0);
        switch (girl_rank) {
        case 0:
        case 1:
            talk_group = 0x17;
            break;
        case 2:
            talk_group = 0x18;
            break;
        case 3:
            talk_group = 0x19;
            break;
        case 4:
            talk_group = 0x1A;
            break;
        }
        mode_step_next();
        break;
    case 0xD:
        normal_date_speak();
        break;
    case 0xE:
        switch (*(u32 *)&D_80122CDC) {
        case 0:
            talk_group = 0x1B;
            portrait_layout_set(1U);
            game_state.girls[0].tokimeki += 3;
            game_state.girls[0].affection += 2;
            game_state.girls[0].syoushin -= 0xF;
            break;
        case 1:
            talk_group = 0x1D;
            portrait_layout_set(2U);
            game_state.girls[0].tokimeki += 2;
            game_state.girls[0].affection += 1;
            game_state.girls[0].syoushin -= 0xA;
            break;
        case 2:
            talk_group = 0x1F;
            portrait_layout_set(3U);
            game_state.girls[0].affection += 1;
            game_state.girls[0].syoushin += 0xA;
            break;
        }
        portrait_face_update();
        mode_step_next();
        break;
    case 0xF:
        normal_date_speak();
        break;
    case 0x10:
        if (girl_state_rank(0) < 2) {
            talk_group = 0x27;
        }
        mode_step_next();
        break;
    case 0x11:
        normal_date_speak();
        break;
    case 0x12:
        normal_date_bggirl_fadeout();
        break;
    case 0x13:
        mode_step_next();
    case 0x14:
        mode_step_next();
        break;
    case 0x15:
        tour_finish_visit();
        break;
    case 0x16:
        if (audio_bank_load_poll() == 1) {
            game_state.day_event_done_bits |= 4;
            talk_group = 0x25;
            dec_bg_cd_read(0x42D8, 1);
            mode_step_next();
        }
        break;
    case 0x17:
        normal_date_speak();
        break;
    case 0x18:
        if (cd_read_sync_status() & 1) {
            event_log_add(0x14, 0x19);
            audio_command(0x200);
            k_reset(0);
            message_window_set_title((s32) "近所の公園");
            D_80125CA0 = 0xC0;
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(3U, 0x801B0000, 0xAF43);
            mode_step_next();
        }
        break;
    case 0x19:
        normal_date_bggirl_fadein();
        break;
    case 0x1A:
        if (cd_read_sync_status() & 1) {
            tour_birthday_tables_init();
            talk_group = 1;
            talk_line = 0;
            talk_text_table = tour_birthday_script[0][1];
            talk_command_table = tour_birthday_script[1][1];
            talk_voice_table = tour_birthday_script[2][1];
            mode_step_next();
        }
        break;
    case 0x1B:
        normal_date_speak();
        break;
    case 0x1C:
        cd_read_sectors(3U, 0x801E8000, 0x49B7);
        mode_step_next();
        break;
    case 0x1D:
        if (cd_read_sync_status() & 1) {
            tour_script_birthday();
            talk_group = 0x26;
            talk_line = 0;
            talk_text_table = (s32) tour_script_text;
            talk_command_table = (s32) tour_script_commands;
            talk_voice_table = (s32) tour_script_voices;
            mode_step_next();
        }
        break;
    case 0x1E:
        normal_date_speak();
        break;
    case 0x1F:
        talk_group = 0x29;
        mode_step_next();
        break;
    case 0x20:
        normal_date_speak();
        break;
    case 0x21:
        talk_group = game_state.year - 0x5C;
        mode_step_next();
        break;
    case 0x22:
        normal_date_speak();
        break;
    case 0x23:
        normal_date_three_select();
        break;
    case 0x24:
        talk_group = 0x23;
        mode_step_next();
        break;
    case 0x25:
        normal_date_speak();
        break;
    case 0x26:
        mode_step_next();
        break;
    case 0x27:
        mode_step_next();
        break;
    case 0x28:
        switch (*(u32 *)&D_80122CDC) {
        case 0:
            talk_group = 0x1B;
            portrait_layout_set(1U);
            game_state.girls[0].tokimeki += 3;
            game_state.girls[0].affection += 2;
            game_state.girls[0].syoushin -= 0xF;
            break;
        case 1:
            talk_group = 0x1D;
            portrait_layout_set(2U);
            game_state.girls[0].tokimeki += 2;
            game_state.girls[0].affection += 1;
            game_state.girls[0].syoushin -= 0xA;
            break;
        case 2:
            talk_group = 0x1F;
            portrait_layout_set(3U);
            game_state.girls[0].affection += 1;
            game_state.girls[0].syoushin += 0xA;
            break;
        }
        portrait_face_update();
        mode_step_next();
        break;
    case 0x29:
        normal_date_speak();
        break;
    case 0x2A:
        if ((girl_state_rank(0) < 2) && (D_80122CDC == 0)) {
            talk_group = 0x27;
        }
        mode_step_next();
        break;
    case 0x2B:
        normal_date_speak();
        break;
    case 0x2C:
        normal_date_bggirl_fadeout();
        break;
    case 0x2D:
        tour_finish_visit();
        break;
    case 0x2E:
        if (audio_bank_load_poll() == 1) {
            talk_group = 0;
            switch ((u8) game_state.girls[0].flags.b[2] >> 4) {
            case 0:
                if (game_state.year == 0x61) {
                    dec_bg_cd_read(0x40F4, 1);
                } else {
                    dec_bg_cd_read(0x40EA, 1);
                }
                break;
            case 1:
            case 5:
                dec_bg_cd_read(0x40F4, 1);
                break;
            case 2:
                dec_bg_cd_read(0x40FC, 1);
                break;
            case 3:
                dec_bg_cd_read(0x4105, 1);
                break;
            case 4:
                if (game_state.year == 0x60) {
                    dec_bg_cd_read(0x4118, 1);
                } else {
                    dec_bg_cd_read(0x410F, 1);
                }
                break;
            }
            mode_step_next();
        }
        break;
    case 0x2F:
        normal_date_speak();
        break;
    case 0x30:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            shiori_club = (u8) game_state.girls[0].flags.b[2] >> 4;
            switch (shiori_club) {
            case 1:
                portrait_read_start(0x5477);
                break;
            case 2:
                portrait_read_start(0x54D1);
                break;
            case 4:
                portrait_read_start(0x54A4);
                break;
            default:
                portrait_read_start(0x544A);
                break;
            }
            mode_step_next();
        }
        break;
    case 0x31:
        normal_date_two_select();
        break;
    case 0x32:
        if (D_80122CDC == 0) {
            mode_step_set(0x34);
        } else {
            normal_date_speak();
        }
        break;
    case 0x33:
        game_state.girls[0].affection -= 1;
        game_state.girls[0].syoushin += 5;
        tour_finish_visit();
        break;
    case 0x34:
        audio_command(0x200);
        talk_group = game_state.year - 0x5C;
        mode_step_next();
        break;
    case 0x35:
        normal_date_speak();
        break;
    case 0x36:
        normal_date_three_select();
        break;
    case 0x37:
        if (cd_read_sync_status() & 1) {
            talk_group = 6;
            tour_birthday_option_name_set();
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            talk_names_set(0U);
            portrait_show_checked();
        }
        break;
    case 0x38:
        normal_date_speak();
        break;
    case 0x39:
        normal_date_bg_fadein();
        break;
    case 0x3A:
        talk_group = 0xD;
        mode_step_next();
        break;
    case 0x3B:
        normal_date_speak();
        break;
    case 0x3C:
        normal_date_girl_in();
        break;
    case 0x3D:
        girl_rank_2 = girl_state_rank(0);
        switch (girl_rank_2) {
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
        mode_step_next();
        break;
    case 0x3E:
        normal_date_speak();
        break;
    case 0x3F:
        girl_rank_3 = girl_state_rank(0);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 0x17;
            break;
        case 2:
            talk_group = 0x18;
            break;
        case 3:
            talk_group = 0x19;
            break;
        case 4:
            talk_group = 0x1A;
            break;
        }
        mode_step_next();
        break;
    case 0x40:
        normal_date_speak();
        break;
    case 0x41:
        switch (*(u32 *)&D_80122CDC) {
        case 0:
            talk_group = 0x1B;
            portrait_layout_set(1U);
            game_state.girls[0].tokimeki += 3;
            game_state.girls[0].affection += 2;
            game_state.girls[0].syoushin -= 0xF;
            break;
        case 1:
            talk_group = 0x1D;
            portrait_layout_set(2U);
            game_state.girls[0].tokimeki += 2;
            game_state.girls[0].affection += 1;
            game_state.girls[0].syoushin -= 0xA;
            break;
        case 2:
            talk_group = 0x1F;
            portrait_layout_set(3U);
            game_state.girls[0].affection += 1;
            game_state.girls[0].syoushin += 0xA;
            break;
        }
        portrait_face_update();
        mode_step_next();
        break;
    case 0x42:
        normal_date_speak();
        break;
    case 0x43:
        normal_date_girl_out();
        break;
    case 0x44:
        normal_date_speak();
        break;
    case 0x45:
        normal_date_bg_fadeout();
        break;
    case 0x46:
        talk_group = 0x24;
        mode_step_next();
        break;
    case 0x47:
        normal_date_speak();
        break;
    case 0x48:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_phase_player_birthday(void) {
    switch (game_state.mode_step) {
    case 0:
        mode_step_next();
        break;
    case 1:
        k_reset(0);
        message_window_set_title((s32) "　");
        message_window_title_show(0);
        dec_bg_reset();
        set_dec_bri(0);
        game_state.day_event_done_bits |= 4;
        audio_command(0xCF);
        Vblnk_Timer_Init();
        cd_read_sectors(3U, 0x801E8000, 0x49B7);
        mode_step_next();
        break;
    case 2:
        if (Vblnk_Timer() >= 0x15) {
            if (cd_read_sync_status() & 1) {
                audio_bank_load_start(1, 0xC478, 0xC44B, 0xCA95, 0xCA4F, 0xCA3E);
                if (D_80122EBC == 0xB) {
                    game_state.current_girl = GIRL_SHIORI;
                    talk_names_set(0U);
                } else {
                    game_state.current_girl = D_80122EBC;
                    talk_names_set(D_80122EBC);
                }
                tour_script_birthday();
                talk_group = 0x25;
                talk_line = 0;
                talk_text_table = (s32) tour_script_text;
                talk_command_table = (s32) tour_script_commands;
                talk_voice_table = (s32) tour_script_voices;
                mode_step_next();
            }
        }
        break;
    case 3:
        normal_date_speak();
        break;
    case 4:
        if (audio_bank_load_poll() == 1) {
            dec_bg_cd_read(0x42D8, 1);
            mode_step_next();
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            portrait_layout_set(get_g_zyotai_h(game_state.current_girl) & 0x7F);
            audio_command(0x200);
            k_reset(0);
            message_window_set_title((s32) "近所の公園");
            D_80125CA0 = 0xC0;
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(3U, 0x801B0000, 0xAF43);
            mode_step_next();
        }
        break;
    case 6:
        normal_date_bggirl_fadein();
        break;
    case 7:
        if (cd_read_sync_status() & 1) {
            tour_birthday_tables_init();
            if (D_80122EBC == 0xB) {
                game_state.current_girl = GIRL_SHIORI;
                talk_names_set(0U);
            } else {
                game_state.current_girl = D_80122EBC;
                talk_names_set(D_80122EBC);
            }
            talk_group = 1;
            talk_line = 0;
            switch (D_80122EBC) {
            case 11:
                talk_text_table = tour_birthday_script[0][1];
                talk_command_table = tour_birthday_script[1][1];
                talk_voice_table = tour_birthday_script[2][1];
                break;
            case 4:
                talk_text_table = tour_birthday_script[0][10];
                talk_command_table = tour_birthday_script[1][10];
                talk_voice_table = tour_birthday_script[2][10];
                break;
            case 5:
                talk_text_table = tour_birthday_script[0][7];
                talk_command_table = tour_birthday_script[1][7];
                talk_voice_table = tour_birthday_script[2][7];
                break;
            case 6:
                talk_text_table = tour_birthday_script[0][8];
                talk_command_table = tour_birthday_script[1][8];
                talk_voice_table = tour_birthday_script[2][8];
                break;
            case 7:
                talk_text_table = tour_birthday_script[0][5];
                talk_command_table = tour_birthday_script[1][5];
                talk_voice_table = tour_birthday_script[2][5];
                break;
            case 8:
                talk_text_table = tour_birthday_script[0][0];
                talk_command_table = tour_birthday_script[1][0];
                talk_voice_table = tour_birthday_script[2][0];
                break;
            case 9:
                talk_text_table = tour_birthday_script[0][9];
                talk_command_table = tour_birthday_script[1][9];
                talk_voice_table = tour_birthday_script[2][9];
                break;
            case 10:
                talk_text_table = tour_birthday_script[0][12];
                talk_command_table = tour_birthday_script[1][12];
                talk_voice_table = tour_birthday_script[2][12];
                break;
            default:
                game_mode_reset_to_title();
                break;
            }
            mode_step_next();
        }
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadeout();
        break;
    case 11:
        event_log_add(0x14, 0x19);
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

s32 tour_birthday_option_name_set(void) {
    GameState *g = &game_state;
    TourOptionNames names;

    names = tour_birthday_option_names;
    strcpy(name_code_a, names.names[g->year - 0x5F][D_80122CDC]);
}
