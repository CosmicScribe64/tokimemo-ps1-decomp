#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_801428D0(void) {
    ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 3;
    switch (((CultureFestivalDispatch *)&D_80122EB8)->unk_00) {
    case 4:
        switch (game_state.year) {
        case 0x5F:
            if (girl_state_rank(3) == 4 && !game_state.girls[3].flags.f.b2) {
                ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
                main_stage_set(5);
                return;
            }
            main_stage_set(0x36);
            return;
        case 0x60:
            if (girl_state_rank(3) == 4 && !game_state.girls[3].flags.f.b2) {
                ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
                main_stage_set(5);
                return;
            }
            main_stage_set(0x37);
            return;
        case 0x61:
            if (girl_state_rank(3) == 4 && !game_state.girls[3].flags.f.b2) {
                ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
                main_stage_set(5);
                return;
            }
            main_stage_set(0x38);
            return;
        }
        break;
    case 5:
        main_stage_set(0x39);
        return;
    default:
        game_reset_to_title();
        return;
    }
}

void func_80142A40(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;

    switch (g->step) {
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
            main_step_next();
        }
        break;
    case 3:
        bustup_read_step(0x5828);
        break;
    case 4:
        memcpy((void *)0x80197000, 0x80180000, 0x16800);
        main_step_next();
        break;
    case 5:
        bustup_read_step(0x58DC);
        break;
    case 6:
        memcpy((void *)0x801AE000, 0x80180000, 0x16800);
        addr_init_bustup();
        g->current_girl = GIRL_AYAKO;
        bustup_show_checked();
        func_80145844();
        cd_read_sectors(9U, 0x801E8000, 0x4B0A);
        break;
    case 7:
        normal_date_bg_fadein();
        break;
    case 8:
        if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
            main_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            func_80143D68();
            temp_v0 = girl_state_rank(3);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 4;
                break;
            case 2:
                dialogue_group = 0xD;
                break;
            case 3:
                dialogue_group = 0x16;
                break;
            }
            if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
                dialogue_group = 0x1F;
            }
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (g->girls[3].flags.f.b2) {
            main_step_set(0xD);
        } else {
            dialogue_group = 1;
            g->girls[3].flags.f.b2 = 1;
            main_step_next();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = girl_state_rank(3);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 5;
            break;
        case 2:
            dialogue_group = 0xE;
            break;
        case 3:
            dialogue_group = 0x17;
            break;
        }
        main_step_next();
        if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
            main_step_set(0x24);
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        dialogue_names_set(0xCU);
        D_80122D20 = 1;
        g->current_girl = GIRL_REI;
        main_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        memcpy((void *)0x80180000, 0x80197000, 0x16800);
        main_step_next();
        break;
    case 19:
        D_80122D20 = 0;
        addr_init_bustup();
        bustup_show_checked();
        bustup_layout_set(0U);
        break;
    case 20:
        normal_date_girl_in();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        normal_date_girl_out();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        normal_date_girl_in();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        normal_date_girl_out();
        break;
    case 27:
        memcpy((void *)0x80180000, 0x801AE000, 0x16800);
        main_step_next();
        break;
    case 28:
        addr_init_bustup();
        g->current_girl = GIRL_AYAKO;
        bustup_show_checked();
        dialogue_names_set(3U);
        break;
    case 29:
        normal_date_girl_in();
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        dialogue_names_set(0xCU);
        g->current_girl = GIRL_REI;
        D_80122D20 = 1;
        main_step_next();
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        dialogue_names_set(3U);
        g->current_girl = GIRL_AYAKO;
        D_80122D20 = 0;
        main_step_next();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        normal_date_girl_out();
        break;
    case 36:
        normal_date_bg_fadeout();
        break;
    case 37:
        dialogue_group = 0;
        main_step_next();
        break;
    case 38:
        normal_date_speak();
        break;
    case 39:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80142FD4(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;

    switch (g->step) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(9U, 0x801E8000, 0x4B0A);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(3);
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
            if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
                dialogue_group = 0x23;
                g->current_girl = GIRL_YOSHIO;
            } else {
                g->current_girl = GIRL_AYAKO;
            }
            func_80143D68();
            if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
                dialogue_names_set(0xDU);
                bustup_read_start(0x6C5F);
            } else {
                bustup_read_start(0x58DC);
            }
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            bustup_show_checked();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
            dialogue_group = 0x2C;
        } else if (g->girls[3].flags.f.b2) {
            dialogue_group = 0x24;
        } else {
            g->girls[3].flags.f.b2 = 1;
            dialogue_group = 0x25;
        }
        main_step_next();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
            main_step_set(0x13);
        } else {
            main_step_next();
        }
        break;
    case 11:
        temp_v0_2 = girl_state_rank(3);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 0x26;
            break;
        case 2:
            dialogue_group = 0x28;
            break;
        case 3:
            dialogue_group = 0x2A;
            break;
        }
        main_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_girl_out();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        normal_date_bg_fadeout();
        break;
    case 16:
        dialogue_group = 0;
        g->girls[3].flags.f.b2 = 1;
        main_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80145530();
        break;
    case 19:
        normal_date_two_select();
        break;
    case 20:
        if (D_80122CDC != 0) {
            dialogue_group = 0x2F;
        } else {
            dialogue_group = 0x2D;
        }
        main_step_next();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        normal_date_girl_out();
        break;
    case 23:
        if (D_80122CDC != 0) {
            main_step_set(0xF);
        } else {
            dialogue_names_set(3U);
            main_step_next();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        main_step_set(0xF);
        break;
    default:
        game_reset_to_title();
        break;
    }
}

/* T-12230: existing GirlFlagBits bitfields reproduce the target register choices. */
void func_80143494(void) {
    s32 temp_v0;
    s32 temp_v0_2;

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
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x58DC);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_AYAKO;
            bustup_show_checked();
            cd_read_sectors(9U, 0x801E8000, 0x4B0A);
        }
        break;
    case 5:
        if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
            main_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            func_80143D68();
            temp_v0 = girl_state_rank(3);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x30;
                break;
            case 2:
                dialogue_group = 0x34;
                break;
            case 3:
                dialogue_group = 0x38;
                break;
            }
            if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
                dialogue_group = 0x3C;
            }
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
            main_step_set(0xC);
        } else if (game_state.girls[3].flags.f.b2) {
            main_step_set(0xA);
        } else {
            dialogue_group = 1;
            main_step_next();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = girl_state_rank(3);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 0x31;
            break;
        case 2:
            dialogue_group = 0x35;
            break;
        case 3:
            dialogue_group = 0x39;
            break;
        }
        main_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_girl_out();
        }
        break;
    case 17:
        if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
            main_step_next();
        } else {
            normal_date_bg_fadeout();
        }
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

void func_801438F8(void) {
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
            cd_read_sectors(9U, 0x801E8000, 0x4B0A);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            dialogue_group = 0x3F;
            func_80143D68();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EAC = 4;
        D_80122EB0 = (s32) g->stage;
        D_80122EB4 = g->step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        cd_read_sectors(9U, 0x801E8000, 0x4B0A);
        main_step_next();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            bustup_read_step(0x58AF);
        }
        break;
    case 10:
        func_80145A30();
        func_80143D68();
        if (g->girls[3].flags.f.b2) {
            main_step_set(0xC);
        } else {
            g->girls[3].flags.f.b2 = 1;
            dialogue_group = 2;
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        dialogue_group = 3;
        main_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        addr_init_bustup();
        g->current_girl = GIRL_AYAKO;
        bustup_show_checked();
        dec_bg_cd_read(0x40F4, 1);
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            func_801459FC();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 16:
        if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 17:
        temp_v0 = girl_state_rank(3);
        switch (temp_v0) {
        case 0:
        case 1:
            dialogue_group = (g->year * 3) - 0xDD;
            break;
        case 2:
            dialogue_group = (g->year * 3) - 0xDC;
            break;
        case 3:
            dialogue_group = (g->year * 3) - 0xDB;
            break;
        }
        if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
            dialogue_group = 0x49;
        }
        main_step_next();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_girl_out();
        }
        break;
    case 20:
        if ((girl_state_rank(3) == 4) || (D_80122EA4 == 3)) {
            main_step_next();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 21:
        dialogue_group = 0;
        main_step_next();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80143D68(void) {
    dialogue_names_set(3);
    func_801321D4();
    dialogue_line = 0;
    dialogue_text_table = D_80155E90;
    dialogue_command_table = D_80155E94;
    dialogue_voice_table = D_80155E98;
    main_step_next();
}
