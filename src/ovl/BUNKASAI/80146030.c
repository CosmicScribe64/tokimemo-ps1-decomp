#define MAIN_API_OVERRIDE_bustup_layout_set /* T-12230: original K&R call; callee narrows the argument. */
#include "common.h"
#include "ovl/BUNKASAI.h"

void bustup_layout_set();

void func_80146030(void) {
    s32 selector;
    CultureFestivalDispatch *work;

    work = (CultureFestivalDispatch *)&D_80122EB8;
    selector = work->unk_00;
    work->unk_04 = 3;
    game_state.girls[3].flags.f.b2 = 1;
    switch (selector) {
    case 4:
        switch (game_state.year) {
        case 0x5F:
            main_stage_set(0x51);
            return;
        case 0x60:
            main_stage_set(0x52);
            return;
        case 0x61:
            main_stage_set(0x53);
            return;
        }
        break;
    case 5:
        main_stage_set(0x54);
        return;
    default:
        game_reset_to_title();
        return;
    }
}
void func_80146100(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x58DC);
            main_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            addr_init_bustup();
            game_state.current_girl = GIRL_AYAKO;
            bustup_show_checked();
            cd_read_sectors(0xCU, 0x801E8000, 0x4AFC);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(3);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 2;
                break;
            case 2:
            case 3:
                dialogue_group = 5;
                break;
            case 4:
                dialogue_group = 9;
                break;
            }
            func_80146F38();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if ((girl_state_rank(3) == 2) || (girl_state_rank(3) == 3)) {
            normal_date_girl_out();
        } else {
            main_step_next();
        }
        break;
    case 8:
        if ((girl_state_rank(3) == 2) || (girl_state_rank(3) == 3)) {
            normal_date_speak();
        } else {
            main_step_next();
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
        dialogue_group = 0;
        main_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_bggirl_fadein();
        break;
    case 13:
        temp_v0_2 = girl_state_rank(3);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 3;
            break;
        case 2:
        case 3:
            dialogue_group = 7;
            break;
        case 4:
            dialogue_group = 0xA;
            break;
        }
        if ((u16) game_state.clubs[4].exp < 0x64U) {
            dialogue_group += 1;
        }
        main_step_next();
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
        dialogue_group = 1;
        main_step_next();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_801464A8(void) {
    u32 temp_v0;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x58DC);
            main_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            addr_init_bustup();
            game_state.current_girl = GIRL_AYAKO;
            bustup_show_checked();
            cd_read_sectors(0xCU, 0x801E8000, 0x4AFC);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(3);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0xC;
                break;
            case 2:
                dialogue_group = 0xD;
                break;
            case 3:
                dialogue_group = 0xE;
                break;
            case 4:
                dialogue_group = 0xF;
                break;
            }
            func_80146F38();
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
            dialogue_group = 0x45;
            main_step_next();
        } else {
            main_step_set(0xA);
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bg_fadeout();
        break;
    case 11:
        dialogue_group = 1;
        main_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_801466FC(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x58DC);
            main_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            addr_init_bustup();
            game_state.current_girl = GIRL_AYAKO;
            bustup_show_checked();
            cd_read_sectors(0xCU, 0x801E8000, 0x4AFC);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(3);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x10;
                break;
            case 2:
                dialogue_group = 0x13;
                break;
            case 3:
                dialogue_group = 0x14;
                break;
            case 4:
                dialogue_group = 0x15;
                break;
            }
            func_80146F38();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if (girl_state_rank(3) < 2) {
            bustup_layout_set(0U);
            main_step_next();
        } else {
            main_step_set(0xB);
        }
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        bustup_layout_set(get_g_zyotai_h(GIRL_AYAKO));
        main_step_next();
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        normal_date_bggirl_fadeout();
        break;
    case 12:
        dialogue_group = 0;
        main_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bggirl_fadein();
        break;
    case 15:
        temp_v0_2 = girl_state_rank(3);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 0x16;
            break;
        case 2:
            dialogue_group = 0x18;
            break;
        case 3:
            dialogue_group = 0x1A;
            break;
        case 4:
            dialogue_group = 0x1C;
            break;
        }
        if ((u16) game_state.clubs[4].exp < 0x12CU) {
            dialogue_group += 1;
        }
        main_step_next();
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
        dialogue_group = 1;
        main_step_next();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

/* T-12702: reload the selector after calls that may update GameState. */
void func_80146A78(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u8 temp_v0_3;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            bustup_read_start(0x58AF);
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            addr_init_bustup();
            game_state.current_girl = GIRL_AYAKO;
            bustup_show_checked();
            cd_read_sectors(0xCU, 0x801E8000, 0x4AFC);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(3);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x1E;
                break;
            case 2:
                dialogue_group = 0x1F;
                break;
            case 3:
                dialogue_group = 0x20;
                break;
            case 4:
                dialogue_group = 0x21;
                break;
            }
            dialogue_group = (dialogue_group + (game_state.year * 4)) - 0x17C;
            func_80146F38();
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
        D_80122EAC = 6;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 10:
        bustup_read_step(0x58AF);
        break;
    case 11:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_AYAKO;
        bustup_show_checked();
        cd_read_sectors(0xCU, 0x801E8000, 0x4AFC);
        break;
    case 12:
        normal_date_bggirl_fadein();
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(3);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x2A;
                break;
            case 2:
                dialogue_group = 0x2C;
                break;
            case 3:
                dialogue_group = 0x2E;
                break;
            case 4:
                dialogue_group = 0x30;
                break;
            }
            temp_v0_3 = game_state.year;
            dialogue_group = (dialogue_group + (temp_v0_3 * 0xA)) - 0x3B6;
            if ((u16) game_state.clubs[5].exp < (u32) ((temp_v0_3 * 0x64) - 0x24B8)) {
                if (girl_state_rank(3) == 4) {
                    dialogue_group += 2;
                } else {
                    dialogue_group += 1;
                }
            }
            if ((game_state.year == 0x61) && (girl_state_rank(3) >= 2)) {
                dialogue_group -= 2;
            }
            func_80146F38();
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (girl_state_rank(3) == 4) {
            normal_date_girl_out();
        } else {
            main_step_set(0x11);
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (girl_state_rank(3) == 4) {
            main_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 18:
        normal_date_bg_fadeout();
        break;
    case 19:
        dialogue_group = 1;
        main_step_next();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80146F38(void) {
    dialogue_names_set(3);
    func_80132208();
    dialogue_line = 0;
    dialogue_text_table = D_801604F0;
    dialogue_command_table = D_801604F4;
    dialogue_voice_table = D_801604F8;
    main_step_next();
}
