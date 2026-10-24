#define MAIN_API_OVERRIDE_D_80122EB8 /* matched as u32 (main_api.h: s32) */
#include "common.h"
#include "ovl/BUNKAKEN.h"

extern u32 D_80122EB8;

void func_8013BFE0(void) {
    D_80122EBC = 1;
    switch (D_80122EB8) {
    case 0:
        if (game_state.year == 0x61) {
            main_stage_set(0x2E);
            return;
        }
        if ((girl_state_rank(1) == 4) && !game_state.girls[1].flags.f.b2) {
            D_80122EBC = 0xFF;
            main_stage_set(5);
            return;
        }
        main_stage_set(0x2D);
        return;
    case 1:
        main_stage_set(0x2F);
        return;
    default:
        game_reset_to_title();
        return;
    }
}

void func_8013C0AC(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (g->step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        D_80155F50 = 0;
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x5D41);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_MIO;
            bustup_show_checked();
            cd_read_sectors(7U, 0x801E8000, 0x4AB6);
        }
        break;
    case 5:
        if ((girl_state_rank(1) == 4) || (D_80122EA4 == 1)) {
            main_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(1);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 1;
                break;
            case 2:
                dialogue_group = 2;
                break;
            case 3:
                dialogue_group = 3;
                break;
            }
            if ((girl_state_rank(1) == 4) || (D_80122EA4 == 1)) {
                if (g->year == 0x5F) {
                    dialogue_group = 0xD;
                } else {
                    dialogue_group = 0x16;
                }
            }
            func_8013CEC0();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if ((girl_state_rank(1) == 4) || (D_80122EA4 == 1)) {
            main_step_set(0xC);
        } else if (!(g->girls[1].flags.f.b2)) {
            g->girls[1].flags.f.b2 = 1;
            dialogue_group = 4;
            main_step_next();
        } else {
            main_step_set(0xA);
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = girl_state_rank(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
            if (g->year == 0x5F) {
                dialogue_group = 7;
            } else {
                dialogue_group = 0x10;
            }
            break;
        case 2:
            if (g->year == 0x5F) {
                dialogue_group = 9;
            } else {
                dialogue_group = 0x12;
            }
            break;
        case 3:
            if (g->year == 0x5F) {
                dialogue_group = 0xB;
            } else {
                dialogue_group = 0x14;
            }
            break;
        }
        main_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((girl_state_rank(1) == 4) || (D_80122EA4 == 1)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
            dialogue_group = 0xE;
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if ((girl_state_rank(1) == 4) || (D_80122EA4 == 1)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
            temp_v0_3 = girl_state_rank(1);
            switch (temp_v0_3) {
            case 0:
            case 1:
                if (g->year == 0x5F) {
                    dialogue_group = 8;
                } else {
                    dialogue_group = 0x11;
                }
                break;
            case 2:
                if (g->year == 0x5F) {
                    dialogue_group = 0xA;
                } else {
                    dialogue_group = 0x13;
                }
                break;
            case 3:
                if (g->year == 0x5F) {
                    dialogue_group = 0xC;
                } else {
                    dialogue_group = 0x15;
                }
                break;
            }
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if ((girl_state_rank(1) == 4) || (D_80122EA4 == 1)) {
            main_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        dialogue_group = 0;
        main_step_next();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_8013C634(void) {
    GameState *g = &game_state;
    u32 temp_v0;

    switch (g->step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(7U, 0x801E8000, 0x4AB6);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            if (g->girls[1].flags.f.b2) {
                dialogue_group = 0x19;
            } else {
                dialogue_group = 0x1A;
            }
            func_8013CEC0();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EAC = 3;
        D_80122EB0 = (s32) g->stage;
        D_80122EB4 = g->step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        cd_read_sectors(7U, 0x801E8000, 0x4AB6);
        main_step_next();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            func_8013CEC0();
            if (g->girls[1].flags.f.b2) {
                main_step_set(0xB);
            } else {
                g->girls[1].flags.f.b2 = 1;
                dialogue_group = 4;
            }
            dec_bg_cd_read(0x40F4, 1);
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        dialogue_group = 6;
        main_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x5D41);
            main_step_next();
        }
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            func_801459FC();
            addr_init_bustup();
            g->current_girl = GIRL_MIO;
            bustup_show_checked();
        }
        break;
    case 15:
        if ((girl_state_rank(1) == 4) || (D_80122EA4 == 1)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 16:
        temp_v0 = girl_state_rank(1);
        switch (temp_v0) {
        case 0:
        case 1:
            dialogue_group = 0x1B;
            break;
        case 2:
            dialogue_group = 0x1C;
            break;
        case 3:
            dialogue_group = 0x1D;
            break;
        case 4:
            dialogue_group = 0x1E;
            break;
        }
        if (D_80122EA4 == 1) {
            dialogue_group = 0x1E;
        }
        main_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if ((girl_state_rank(1) == 4) || (D_80122EA4 == 1)) {
            main_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 19:
        normal_date_bg_fadeout();
        break;
    case 20:
        dialogue_group = 0;
        main_step_next();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_8013CA6C(void) {
    GameState *g = &game_state;
    s32 temp_v0;

    switch (g->step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(7U, 0x801E8000, 0x4AB6);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            dialogue_group = 0x1F;
            func_8013CEC0();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EB0 = (s32) g->stage;
        D_80122EB4 = g->step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        cd_read_sectors(7U, 0x801E8000, 0x4AB6);
        main_step_next();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            func_8013CEC0();
            if (g->girls[1].flags.f.b2) {
                main_step_set(0xB);
            } else {
                dialogue_group = 5;
                g->girls[1].flags.f.b2 = 1;
            }
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x40F4, 1);
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        dialogue_group = 6;
        main_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x5D6E);
            main_step_next();
        }
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            func_801459FC();
            addr_init_bustup();
            g->current_girl = GIRL_MIO;
            bustup_show_checked();
        }
        break;
    case 15:
        if ((girl_state_rank(1) == 4) || (D_80122EA4 == 1)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 16:
        temp_v0 = girl_state_rank(1);
        switch (temp_v0) {
        case 0:
        case 1:
            dialogue_group = 0x20;
            break;
        case 2:
            dialogue_group = 0x21;
            break;
        case 3:
            dialogue_group = 0x22;
            break;
        }
        if ((girl_state_rank(1) == 4) || (D_80122EA4 == 1)) {
            dialogue_group = 0x23;
        }
        dialogue_group = (dialogue_group + (g->year * 4)) - 0x17C;
        main_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if ((girl_state_rank(1) == 4) || (D_80122EA4 == 1)) {
            main_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 19:
        normal_date_bg_fadeout();
        break;
    case 20:
        dialogue_group = 0;
        g->girls[1].flags.f.b2 = 1;
        main_step_next();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_8013CEC0(void) {
    dialogue_names_set(1);
    func_80132104();
    dialogue_line = 0;
    dialogue_text_table = D_80155E90;
    dialogue_command_table = D_80155E94;
    dialogue_voice_table = D_80155E98;
    main_step_next();
}
