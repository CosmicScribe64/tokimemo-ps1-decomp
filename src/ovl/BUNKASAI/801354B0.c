#include "common.h"
#include "ovl/BUNKASAI.h"

void func_801354B0(void) {
    CultureFestivalDispatch *work = (CultureFestivalDispatch *)&D_80122EB8;
    s32 selector = work->unk_00;
    work->unk_04 = 1;
    game_state.girls[1].flags.f.b2 = 1;
    switch (selector) {
    case 0:
        if (game_state.year == 0x61) {
            main_stage_set(0x2C);
            return;
        }
        main_stage_set(0x2B);
        return;
    case 1:
        main_stage_set(0x2D);
        return;
    default:
        game_reset_to_title();
        return;
    }
}

void func_80135554(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            bustup_read_step(0x5D41);
        }
        break;
    case 3:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        func_801431D4();
        addr_init_bustup();
        game_state.current_girl = GIRL_MIO;
        bustup_show_checked();
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
                dialogue_group = 2;
                break;
            case 2:
                dialogue_group = 4;
                break;
            case 3:
                dialogue_group = 6;
                break;
            case 4:
                dialogue_group = 8;
                break;
            }
            if (game_state.year == 0x60) {
                dialogue_group += 8;
            }
            func_80136230();
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
        dialogue_group = 0;
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
            dialogue_group = 0x12;
            break;
        case 2:
            dialogue_group = 0x14;
            break;
        case 3:
            dialogue_group = 0x16;
            break;
        case 4:
            dialogue_group = 0x18;
            break;
        }
        if ((u16) game_state.clubs[0].exp < (u32) ((game_state.year * 0x64) - 0x24B8)) {
            dialogue_group += 1;
        }
        main_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_girl_out();
        break;
    case 15:
        if (girl_state_rank(1) == 4) {
            if ((u16) game_state.clubs[0].exp < (u32) ((game_state.year * 0x64) - 0x24B8)) {
                dialogue_group = 0x3D;
            } else {
                dialogue_group = 0x3E;
            }
            main_step_next();
        } else {
            main_step_set(0x11);
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        dialogue_group = 1;
        main_step_next();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80135914(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
            bustup_read_step(0x5D41);
        }
        break;
    case 3:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        func_8014338C();
        addr_init_bustup();
        game_state.current_girl = GIRL_MIO;
        bustup_show_checked();
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
                dialogue_group = 0x1A;
                break;
            case 2:
                dialogue_group = 0x1C;
                break;
            case 3:
                dialogue_group = 0x1E;
                break;
            case 4:
                dialogue_group = 0x20;
                break;
            }
            func_80136230();
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
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 9:
        bustup_read_step(0x5D41);
        break;
    case 10:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_MIO;
        bustup_show_checked();
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
                dialogue_group = 0x1B;
                break;
            case 2:
                dialogue_group = 0x1D;
                break;
            case 3:
                dialogue_group = 0x1F;
                break;
            case 4:
                dialogue_group = 0x21;
                break;
            }
            func_80136230();
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
        dialogue_group = 1;
        main_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80135C4C(void) {
    GameState *state;
    u32 choice;
    u32 choice2;
    u32 choice3;

    state = &game_state;
    switch (state->step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            bustup_read_step(0x5D6E);
        }
        break;
    case 3:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        func_8014338C();
        addr_init_bustup();
        state->current_girl = GIRL_MIO;
        bustup_show_checked();
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
                    dialogue_group = 0x26;
                    break;
                case 2:
                case 3:
                    dialogue_group = 0x27;
                    break;
                case 4:
                    dialogue_group = 0x28;
                    break;
                }
            } else {
                choice2 = girl_state_rank(1);
                switch (choice2) {
                case 0:
                case 1:
                    dialogue_group = 0x22;
                    break;
                case 2:
                    dialogue_group = 0x23;
                    break;
                case 3:
                    dialogue_group = 0x24;
                    break;
                case 4:
                    dialogue_group = 0x25;
                    break;
                }
                if (state->year == 0x61) {
                    dialogue_group += 7;
                }
            }
            func_80136230();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if ((state->year == 0x61) && (girl_state_rank(1) == 4)) {
            dialogue_group = 0x3E;
            normal_date_girl_out();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 8:
        if ((state->year == 0x61) && (girl_state_rank(1) == 4)) {
            normal_date_speak();
        } else {
            main_step_set(0xA);
        }
        break;
    case 9:
        normal_date_bg_fadeout();
        break;
    case 10:
        D_80122EB0 = (s32) state->stage;
        D_80122EB4 = state->step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 11:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x40F4, 1);
        main_step_next();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            bustup_read_step(0x5D6E);
        }
        break;
    case 13:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        state->current_girl = GIRL_MIO;
        bustup_show_checked();
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
                dialogue_group = 0x2D;
                break;
            case 2:
                dialogue_group = 0x2F;
                break;
            case 3:
                dialogue_group = 0x31;
                break;
            case 4:
                dialogue_group = 0x33;
                break;
            }
            if ((u16) state->clubs[1].exp < (u32) ((state->year * 0x64) - 0x24B8)) {
                dialogue_group += 1;
            }
            if (state->year == 0x61) {
                dialogue_group = dialogue_group + 8;
            }
            func_80136230();
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
            if (((u8) state->year < 0x61U) && ((u16) state->clubs[1].exp >= (u32) ((state->year * 0x64) - 0x24B8))) {
                dialogue_group = 0x3E;
                main_step_next();
            } else if (state->year == 0x61) {
                if ((u16) state->clubs[1].exp >= 0x12CU) {
                    dialogue_group = 0x3E;
                    main_step_next();
                } else {
                    dialogue_group = 0x3F;
                    main_step_next();
                }
            } else {
                main_step_set(0x14);
            }
        } else {
            main_step_set(0x14);
        }
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        normal_date_bg_fadeout();
        break;
    case 21:
        dialogue_group = 1;
        main_step_next();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80136230(void) {
    dialogue_names_set(1);
    func_801320D0();
    dialogue_line = 0;
    dialogue_text_table = D_801604F0;
    dialogue_command_table = D_801604F4;
    dialogue_voice_table = D_801604F8;
    main_step_next();
}
