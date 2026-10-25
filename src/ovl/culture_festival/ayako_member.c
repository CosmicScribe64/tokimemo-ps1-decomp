/* ayako_member.c: src/ovl/BUNKASAI/80146030.c:7: Ayako member block, stages 80-84. Original object 0x80146030-0x80146FA0 (config/objects/BUNKASAI.txt). */
#define MAIN_API_OVERRIDE_portrait_layout_set /* T-12230: original K&R call; callee narrows the argument. */
#include "common.h"
#include "ovl/culture_festival.h"

void portrait_layout_set();

void culture_ayako_member_entry(void) {
    s32 selector;
    CultureFestivalDispatch *work;

    work = (CultureFestivalDispatch *)&culture_exhibit_club;
    selector = work->club_id;
    work->girl = 3;
    game_state.girls[3].flags.f.b2 = 1;
    switch (selector) {
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(CULTURE_STAGE_AYAKO_MEMBER_PORTRAIT_DRAWING);
            return;
        case 0x60:
            mode_stage_set(CULTURE_STAGE_AYAKO_MEMBER_OPEN_SKETCHING);
            return;
        case 0x61:
            mode_stage_set(CULTURE_STAGE_AYAKO_MEMBER_OIL_PAINTING_EXHIBIT);
            return;
        }
        break;
    case 5:
        mode_stage_set(CULTURE_STAGE_AYAKO_MEMBER_CONCERT);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}
void culture_ayako_member_portrait_drawing(void) {
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
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x58DC);
            mode_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            addr_init_bustup();
            game_state.current_girl = GIRL_AYAKO;
            portrait_show_checked();
            cd_read_sectors(0xCU, 0x801E8000, 0x4AFC);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(3);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 2;
                break;
            case 2:
            case 3:
                talk_group = 5;
                break;
            case 4:
                talk_group = 9;
                break;
            }
            culture_ayako_member_script_start();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if ((girl_state_rank(3) == 2) || (girl_state_rank(3) == 3)) {
            normal_date_girl_out();
        } else {
            mode_step_next();
        }
        break;
    case 8:
        if ((girl_state_rank(3) == 2) || (girl_state_rank(3) == 3)) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 9:
        if ((girl_state_rank(3) == 2) || (girl_state_rank(3) == 3)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 10:
        talk_group = 0;
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_bggirl_fadein();
        break;
    case 13:
        girl_rank_2 = girl_state_rank(3);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 3;
            break;
        case 2:
        case 3:
            talk_group = 7;
            break;
        case 4:
            talk_group = 0xA;
            break;
        }
        if ((u16) game_state.clubs[4].practice_points < 0x64U) {
            talk_group += 1;
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
        talk_group = 1;
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

void culture_ayako_member_open_sketching(void) {
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
            portrait_read_start(0x58DC);
            mode_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            addr_init_bustup();
            game_state.current_girl = GIRL_AYAKO;
            portrait_show_checked();
            cd_read_sectors(0xCU, 0x801E8000, 0x4AFC);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(3);
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
            culture_ayako_member_script_start();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_girl_out();
        break;
    case 8:
        if (girl_state_rank(3) == 4) {
            talk_group = 0x45;
            mode_step_next();
        } else {
            mode_step_set(0xA);
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bg_fadeout();
        break;
    case 11:
        talk_group = 1;
        mode_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_ayako_member_oil_painting_exhibit(void) {
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
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x58DC);
            mode_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            addr_init_bustup();
            game_state.current_girl = GIRL_AYAKO;
            portrait_show_checked();
            cd_read_sectors(0xCU, 0x801E8000, 0x4AFC);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(3);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x10;
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
            culture_ayako_member_script_start();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if (girl_state_rank(3) < 2) {
            portrait_layout_set(0U);
            mode_step_next();
        } else {
            mode_step_set(0xB);
        }
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        portrait_layout_set(get_g_zyotai_h(GIRL_AYAKO));
        mode_step_next();
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        normal_date_bggirl_fadeout();
        break;
    case 12:
        talk_group = 0;
        mode_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bggirl_fadein();
        break;
    case 15:
        girl_rank_2 = girl_state_rank(3);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x16;
            break;
        case 2:
            talk_group = 0x18;
            break;
        case 3:
            talk_group = 0x1A;
            break;
        case 4:
            talk_group = 0x1C;
            break;
        }
        if ((u16) game_state.clubs[4].practice_points < 0x12CU) {
            talk_group += 1;
        }
        mode_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        normal_date_bg_fadeout();
        break;
    case 19:
        talk_group = 1;
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

/* T-12702: reload the selector after calls that may update GameState. */
void culture_ayako_member_concert(void) {
    u32 girl_rank;
    u32 girl_rank_2;
    u8 year;

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
            portrait_read_start(0x58AF);
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            culture_backstage_title_show();
            addr_init_bustup();
            game_state.current_girl = GIRL_AYAKO;
            portrait_show_checked();
            cd_read_sectors(0xCU, 0x801E8000, 0x4AFC);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(3);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x1E;
                break;
            case 2:
                talk_group = 0x1F;
                break;
            case 3:
                talk_group = 0x20;
                break;
            case 4:
                talk_group = 0x21;
                break;
            }
            talk_group = (talk_group + (game_state.year * 4)) - 0x17C;
            culture_ayako_member_script_start();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_girl_out();
        break;
    case 8:
        normal_date_bggirl_fadeout();
        break;
    case 9:
        festival_dispatch_variant = 6;
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 10:
        portrait_read_step(0x58AF);
        break;
    case 11:
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_AYAKO;
        portrait_show_checked();
        cd_read_sectors(0xCU, 0x801E8000, 0x4AFC);
        break;
    case 12:
        normal_date_bggirl_fadein();
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(3);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x2A;
                break;
            case 2:
                talk_group = 0x2C;
                break;
            case 3:
                talk_group = 0x2E;
                break;
            case 4:
                talk_group = 0x30;
                break;
            }
            year = game_state.year;
            talk_group = (talk_group + (year * 0xA)) - 0x3B6;
            if ((u16) game_state.clubs[5].practice_points < (u32) ((year * 0x64) - 0x24B8)) {
                if (girl_state_rank(3) == 4) {
                    talk_group += 2;
                } else {
                    talk_group += 1;
                }
            }
            if ((game_state.year == 0x61) && (girl_state_rank(3) >= 2)) {
                talk_group -= 2;
            }
            culture_ayako_member_script_start();
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (girl_state_rank(3) == 4) {
            normal_date_girl_out();
        } else {
            mode_step_set(0x11);
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (girl_state_rank(3) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 18:
        normal_date_bg_fadeout();
        break;
    case 19:
        talk_group = 1;
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

void culture_ayako_member_script_start(void) {
    talk_names_set(3);
    culture_script_set_ayako_member();
    talk_line = 0;
    talk_text_table = culture_script_text_table;
    talk_command_table = culture_script_command_table;
    talk_voice_table = culture_script_voice_table;
    mode_step_next();
}
