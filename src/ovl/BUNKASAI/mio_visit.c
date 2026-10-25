/* mio_visit.c: src/ovl/BUNKASAI/80154018.c:4: Mio visit block, stages 28-41. Original object 0x80154018-0x80158EB0 (config/objects/BUNKASAI.txt). */
#include "common.h"
#include "ovl/BUNKASAI.h"

void culture_mio_visit_entry(void) {
    ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 1;
    switch (((CultureFestivalDispatch *)&D_80122EB8)->unk_00) {
    case 0:
        if (game_state.year == 0x61) {
            mode_stage_set(0x1E);
            return;
        }
        mode_stage_set(0x1D);
        return;
    case 1:
        mode_stage_set(0x1F);
        return;
    case 2:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x20);
            return;
        case 0x60:
            mode_stage_set(0x21);
            return;
        case 0x61:
            mode_stage_set(0x22);
            return;
        }
        break;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x23);
            return;
        case 0x60:
            mode_stage_set(0x25);
            return;
        case 0x61:
            mode_stage_set(0x22);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x26);
            return;
        case 0x60:
            mode_stage_set(0x27);
            return;
        case 0x61:
            mode_stage_set(0x28);
            return;
        }
        break;
    case 5:
        mode_stage_set(0x29);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void culture_mio_visit_literature_exhibit(void) {
    GameState *state;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_7;
    u32 temp_v0;

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
            portrait_read_start(0x5D41);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_MIO;
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
        cd_read_sectors(0x14U, 0x801E8000, 0x4A83);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(1);
            switch (temp_v0) {
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
                if (game_state.year == 0x5F) {
                    talk_group = 0x27;
                } else {
                    talk_group = 0x28;
                }
                break;
            }
            culture_mio_visit_script_a_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(1) == 4) {
            mode_step_set(0x14);
        } else if (((GirlFlagBits *)&game_state.girls[1].flags)->b2) {
            talk_group = 0xB;
            mode_step_next();
        } else {
            talk_group = 6;
            mode_step_set(0xD);
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = girl_state_rank(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0xF;
            break;
        case 2:
            talk_group = 0x10;
            break;
        case 3:
            talk_group = 0x11;
            break;
        }
        mode_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        temp_v0_3 = girl_state_rank(1);
        switch (temp_v0_3) {
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
        }
        if (game_state.year == 0x60) {
            talk_group += 3;
        }
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (!(game_state.girls[1].syoushin & 3)) {
            temp_v0_4 = girl_state_rank(1);
            switch (temp_v0_4) {
        case 0:
        case 1:
            talk_group = 0x24;
            break;
        case 2:
            talk_group = 0x25;
            break;
        case 3:
            talk_group = 0x26;
            break;
        }
        } else {
            temp_v0_5 = girl_state_rank(1);
            switch (temp_v0_5) {
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
        }
        }
        mode_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        state = &game_state;
        if (!(state->girls[1].syoushin & 3)) {
            temp_v0_6 = girl_state_rank(1);
            switch (temp_v0_6) {
            case 0:
            case 1:
                talk_group = 0xD;
                break;
            case 2:
                talk_group = 0xC;
                break;
            case 3:
                talk_group = 0xC;
                break;
            }
        } else {
            temp_v0_7 = girl_state_rank(1);
            switch (temp_v0_7) {
        case 0:
        case 1:
            talk_group = 0x1B;
            break;
        case 2:
            talk_group = 0x1C;
            break;
        case 3:
            talk_group = 0x1D;
            break;
        }
            if (!(state->girls[1].flags.f.b2)) {
                state->girls[1].flags.f.b2 = 1;
                if (state->year == 0x5F) {
                    talk_group += 3;
                } else {
                    talk_group += 6;
                }
            }
        }
        mode_step_next();
        break;
    case 19:
        if (!(game_state.girls[1].syoushin & 3)) {
            normal_date_girl_out();
        } else {
            normal_date_speak();
        }
        break;
    case 20:
        if (!(game_state.girls[1].syoushin & 3) && (girl_state_rank(1) < 4)) {
            normal_date_speak();
        } else {
            normal_date_girl_out();
        }
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
        if (girl_state_rank(1) != 4) {
            if (game_state.year == 0x5F) {
                ((GirlFlagBits *)&game_state.girls[1].flags)->b12 = 1;
            } else {
                ((GirlFlagBits *)&game_state.girls[1].flags)->b13 = 1;
            }
        }
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_mio_visit_speech_contest(void) {
    u8 year;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;
    u32 temp_v0_4;

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
        D_80122EAC = 0;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x5D41);
        break;
    case 9:
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_MIO;
        portrait_show_checked();
        cd_read_sectors(0x14U, 0x801E8000, 0x4A83);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(1);
            switch (temp_v0) {
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
            culture_mio_visit_script_a_start();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = girl_state_rank(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
        case 2:
        case 3:
            talk_group = 7;
            break;
        case 4:
            talk_group = 8;
            break;
        }
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        temp_v0_3 = girl_state_rank(1);
        switch (temp_v0_3) {
        case 0:
        case 1:
            talk_group = 0x29;
            break;
        case 2:
            talk_group = 0x2A;
            break;
        case 3:
            talk_group = 0x2B;
            break;
        case 4:
            talk_group = 0x2C;
            break;
        }
        if (!(game_state.girls[1].syoushin & 3)) {
            talk_group += 8;
        } else if (!(((GirlFlagBits *)&game_state.girls[1].flags)->b2)) {
            talk_group += 4;
        }
        mode_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (!(game_state.girls[1].syoushin & 3)) {
            temp_v0_4 = girl_state_rank(1);
            switch (temp_v0_4) {
            case 0:
            case 1:
                talk_group = 0xD;
                break;
            case 2:
            case 3:
                talk_group = 0xC;
                break;
            case 4:
                talk_group = 0xE;
                break;
            }
        } else {
            game_state.girls[1].flags.f.b2 = 1;
        }
        mode_step_next();
        break;
    case 18:
        normal_date_girl_out();
        break;
    case 19:
        if (!(game_state.girls[1].syoushin & 3)) {
            normal_date_speak();
        } else {
            mode_step_next();
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

void culture_mio_visit_drama_play(void) {
    GameState *state;
    s32 temp_v0_4;
    s32 temp_v0_5;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;
    u32 temp_v0_6;

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
            culture_backstage_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        if (state->year == 0x5F) {
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
        D_80122EB0 = (s32) state->mode_stage;
        D_80122EB4 = state->mode_step + 1;
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
            portrait_read_step(0x5D41);
        }
        break;
    case 9:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        state->current_girl = GIRL_MIO;
        portrait_show_checked();
        cd_read_sectors(0x14U, 0x801E8000, 0x4A83);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(1);
            switch (temp_v0) {
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
            culture_mio_visit_script_a_start();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = girl_state_rank(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
        case 2:
        case 3:
            talk_group = 7;

            break;
        case 4:
            talk_group = 8;
            break;
        }
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        mode_step_next();
        temp_v0_3 = girl_state_rank(1);
        switch (temp_v0_3) {
        case 0:
        case 1:
            talk_group = (state->year * 3) - 0xE8;
            break;
        case 2:
            talk_group = (state->year * 3) - 0xE7;
            break;
        case 3:
            talk_group = (state->year * 3) - 0xE6;
            break;
        case 4:
            mode_step_set(0x1A);
            break;
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        temp_v0_4 = girl_state_rank(1);
        switch (temp_v0_4) {
        default:

            break;
        case 0:
        case 1:
            talk_group = 0x3E;
            break;
        case 2:
            talk_group = 0x3F;
            break;
        case 3:
            talk_group = 0x40;
            break;
        }
        if (!(state->girls[1].syoushin & 3)) {
            talk_group = talk_group + 6;
        } else {

            if (state->year == 0x61) {
                talk_group = talk_group + 3;
            }
        }
        mode_step_next();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if (!(state->girls[1].syoushin & 3)) {
            mode_step_set(0x1A);
        } else {
            temp_v0_5 = girl_state_rank(1);
            switch (temp_v0_5) {
            default:

                break;
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
            }
            if (!(((GirlFlagBits *)&state->girls[1].flags)->b2)) {
                talk_group += 3;
            }

            if (state->year == 0x61) {
                talk_group = talk_group + 6;
            }
            state->girls[1].flags.f.b2 = 1;
            mode_step_next();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        normal_date_girl_out();
        break;
    case 22:
        normal_date_bg_fadeout();
        break;
    case 23:
        talk_group = 0;
        mode_step_next();
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        culture_scene_end();
        break;
    case 26:
        normal_date_girl_out();
        break;
    case 27:
        temp_v0_6 = girl_state_rank(1);
        switch (temp_v0_6) {
        case 0:
        case 1:
            talk_group = 0xD;

            break;
        case 2:
        case 3:
            talk_group = 0xC;
            break;
        case 4:
            talk_group = 0xE;
            break;
        }
        mode_step_next();
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        mode_step_set(0x16);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_mio_visit_experiment_show_95(void) {
    GameState *state;
    s32 temp_v0_2;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_7;
    u32 temp_v0;
    u32 temp_v0_3;
    u32 temp_v0_4;

    state = &game_state;
    switch (state->mode_step) {
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
            portrait_read_start(0x5D41);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            state->current_girl = GIRL_MIO;
            portrait_show_checked();
            talk_group = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x14U, 0x801E8000, 0x4A83);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(1);
            switch (temp_v0) {
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
                talk_group = 5;
                break;
            }
            culture_mio_visit_script_a_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if ((state->girls[1].flags.f.b2) && (girl_state_rank(1) != 4)) {
            if (((u8) state->girls[1].flags.b[2] >> 4) == 0) {
                talk_group = 0xA;
            } else {
                talk_group = 0xB;
            }

            mode_step_next();
        } else {
            mode_step_set(0xE);
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = girl_state_rank(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x53;
            break;
        case 2:
            talk_group = 0x54;
            break;
        case 3:
            talk_group = 0x55;
            break;
        }
        mode_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        temp_v0_3 = girl_state_rank(1);
        switch (temp_v0_3) {
        case 0:
        case 1:
            talk_group = 0x56;
            break;
        case 2:
            talk_group = 0x57;
            break;
        case 3:
            talk_group = 0x58;
            break;
        case 4:
            talk_group = 0x59;
            break;
        }
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_bggirl_fadeout();
        break;
    case 17:
        D_80122EAC = 0;
        D_80122EB0 = (s32) state->mode_stage;
        D_80122EB4 = state->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 18:
        portrait_read_step(0x5D41);
        break;
    case 19:
        culture_exhibit_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        state->current_girl = GIRL_MIO;
        portrait_show_checked();
        cd_read_sectors(0x14U, 0x801E8000, 0x4A83);
        break;
    case 20:
        normal_date_bg_fadein();
        break;
    case 21:
        if (cd_read_sync_status() & 1) {
            temp_v0_4 = girl_state_rank(1);
            switch (temp_v0_4) {
            case 0:
            case 1:
                talk_group = 0x5A;
                break;
            case 2:
                talk_group = 0x5C;
                break;
            case 3:
                talk_group = 0x5E;
                break;
            case 4:
                talk_group = 0x60;
                break;
            }
            culture_mio_visit_script_a_start();
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        if (girl_state_rank(1) == 4) {
            mode_step_set(0x1E);
        } else {
            normal_date_girl_in();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        temp_v0_5 = girl_state_rank(1);
        switch (temp_v0_5) {
        case 0:
        case 1:
            talk_group = 0x61;
            break;
        case 2:
            talk_group = 0x62;
            break;
        case 3:
            talk_group = 0x63;
            break;
        }
        mode_step_next();
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        if (!(state->girls[1].syoushin & 3)) {
            temp_v0_6 = girl_state_rank(1);
            switch (temp_v0_6) {
            case 0:
            case 1:
                talk_group = 0x70;
                break;
            case 2:
                talk_group = 0x71;
                break;
            case 3:
                talk_group = 0x72;
                break;
            }
        } else {
            temp_v0_7 = girl_state_rank(1);
            switch (temp_v0_7) {
            case 0:
            case 1:
                talk_group = 0x64;
                break;
            case 2:
                talk_group = 0x65;
                break;
            case 3:
                talk_group = 0x66;
                break;
            }
            if (!state->girls[1].flags.f.b2) {
                talk_group += 3;
            }

            /* FAKE: dead result test supplies a live range without code,
             * keeping the message in a0 and the flag load temporary. T-10040. */
            if (temp_v0_7) {}
            if ((((u8) state->girls[1].flags.b[2]) >> 4) == 1) {
                talk_group = talk_group + 6;
            }
            state->girls[1].flags.f.b2 = 1;
        }
        mode_step_next();
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        normal_date_girl_out();
        if (((u8) state->girls[1].flags.b[2] >> 4) == 1) {
            talk_group = 0xC;
        } else if (girl_state_rank(1) < 2) {
            talk_group = 0xD;
        } else {
            talk_group = 0xC;
        }
        break;
    case 30:
        if (girl_state_rank(1) < 4) {
            normal_date_speak();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 31:
        if (girl_state_rank(1) < 4) {
            normal_date_bg_fadeout();
        } else {
            mode_step_next();
        }
        talk_group = 0;
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_mio_visit_experiment_show_96(void) {
    GameState *state;
    s32 temp_v0_2;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_7;
    u32 temp_v0;
    u32 temp_v0_3;
    u32 temp_v0_4;

    state = &game_state;
    switch (state->mode_step) {
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
            portrait_read_start(0x5D41);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            state->current_girl = GIRL_MIO;
            portrait_show_checked();
            talk_group = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x14U, 0x801E8000, 0x4A83);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(1);
            switch (temp_v0) {
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
                talk_group = 5;
                break;
            }
            culture_mio_visit_script_a_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if ((state->girls[1].flags.f.b2) && (girl_state_rank(1) != 4)) {
            if (((u8) state->girls[1].flags.b[2] >> 4) == 0) {
                talk_group = 0xA;
            } else {
                talk_group = 0xB;
            }

            mode_step_next();
        } else {
            mode_step_set(0xE);
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = girl_state_rank(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x53;
            break;
        case 2:
            talk_group = 0x54;
            break;
        case 3:
            talk_group = 0x55;
            break;
        }
        mode_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        temp_v0_3 = girl_state_rank(1);
        switch (temp_v0_3) {
        case 0:
        case 1:
            talk_group = 0x56;

            break;
        case 2:
            talk_group = 0x57;
            break;
        case 3:
            talk_group = 0x58;
            break;
        case 4:
            talk_group = 0x59;
            break;
        }
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_bggirl_fadeout();
        break;
    case 17:
        D_80122EAC = 0;
        D_80122EB0 = (s32) state->mode_stage;
        D_80122EB4 = state->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 18:
        portrait_read_step(0x5D41);
        break;
    case 19:
        culture_exhibit_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        state->current_girl = GIRL_MIO;
        portrait_show_checked();
        cd_read_sectors(0x14U, 0x801E8000, 0x4A83);
        break;
    case 20:
        if (girl_state_rank(1) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 21:
        if (cd_read_sync_status() & 1) {
            temp_v0_4 = girl_state_rank(1);
            switch (temp_v0_4) {
            case 0:
            case 1:
                talk_group = 0x73;

                break;
            case 2:
                talk_group = 0x74;
                break;
            case 3:
                talk_group = 0x75;
                break;
            case 4:
                talk_group = 0x76;
                break;
            }
            culture_mio_visit_script_a_start();
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        if (girl_state_rank(1) == 4) {
            mode_step_set(0x1E);
        } else {
            mode_step_next();
        }
        break;
    case 24:
        temp_v0_5 = girl_state_rank(1);
        switch (temp_v0_5) {
        case 0:
        case 1:
            talk_group = 0x77;
            break;
        case 2:
            talk_group = 0x78;
            break;
        case 3:
            talk_group = 0x79;
            break;
        }
        if (!(state->girls[1].syoushin & 3)) {
            talk_group += 0xF;
        }
        mode_step_next();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        if (!(state->girls[1].syoushin & 3)) {
            temp_v0_6 = girl_state_rank(1);
            switch (temp_v0_6) {
            case 0:
            case 1:
                talk_group = 0xD;
                break;
            case 2:
            case 3:
                talk_group = 0xC;
                break;
            }
            mode_step_set(0x1C);
        } else {
            temp_v0_7 = girl_state_rank(1);
            switch (temp_v0_7) {
            case 0:
            case 1:
                talk_group = 0x7A;
                break;
            case 2:
                talk_group = 0x7B;
                break;
            case 3:
                talk_group = 0x7C;
                break;
            }
            if (!(state->girls[1].flags.f.b2)) {
                talk_group += 3;
            }

            if (((u8) state->girls[1].flags.b[2] >> 4) == 1) {
                talk_group = talk_group + 6;
            }
            state->girls[1].flags.f.b2 = 1;
            mode_step_next();
        }
        break;
    case 27:
        normal_date_speak();
        break;
    case 28:
        normal_date_girl_out();
        break;
    case 29:
        if (!(state->girls[1].syoushin & 3)) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
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
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_mio_visit_experiment_show_97(void) {
    GameState *state;
    s32 temp_v0_2;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_7;
    u32 temp_v0;
    u32 temp_v0_3;
    u32 temp_v0_4;

    state = &game_state;
    switch (state->mode_step) {
    case 0:
        if (((u16) state->player_flags.h >> 0xC) == 2) {
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
            portrait_read_start(0x5D41);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            state->current_girl = GIRL_MIO;
            portrait_show_checked();
            talk_group = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x14U, 0x801E8000, 0x4A83);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(1);
            switch (temp_v0) {
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
                talk_group = 5;
                break;
            }
            culture_mio_visit_script_a_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if ((state->girls[1].flags.f.b2) && (girl_state_rank(1) < 4)) {
            if (((u8) state->girls[1].flags.b[2] >> 4) == 0) {
                talk_group = 0xA;
            } else {
                talk_group = 0xB;
            }

            mode_step_next();
        } else {
            mode_step_set(0xE);
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = girl_state_rank(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x53;
            break;
        case 2:
            talk_group = 0x54;
            break;
        case 3:
            talk_group = 0x55;
            break;
        }
        mode_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        temp_v0_3 = girl_state_rank(1);
        switch (temp_v0_3) {
        case 0:
        case 1:
            talk_group = 0x56;

            break;
        case 2:
            talk_group = 0x57;
            break;
        case 3:
            talk_group = 0x58;
            break;
        case 4:
            talk_group = 0x59;
            break;
        }
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_bggirl_fadeout();
        break;
    case 17:
        if (((u16) state->player_flags.h >> 0xC) == 2) {
            D_80122EAC = 0;
        } else {
            D_80122EAC = 0;
        }
        D_80122EB0 = (s32) state->mode_stage;
        D_80122EB4 = state->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 18:
        portrait_read_step(0x5D41);
        break;
    case 19:
        culture_exhibit_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        state->current_girl = GIRL_MIO;
        portrait_show_checked();
        cd_read_sectors(0x14U, 0x801E8000, 0x4A83);
        break;
    case 20:
        normal_date_bg_fadein();
        break;
    case 21:
        if (cd_read_sync_status() & 1) {
            temp_v0_4 = girl_state_rank(1);
            switch (temp_v0_4) {
            case 0:
            case 1:
                talk_group = 0x89;

                break;
            case 2:
                talk_group = 0x8B;
                break;
            case 3:
                talk_group = 0x8D;
                break;
            case 4:
                talk_group = 0x8F;
                break;
            }
            culture_mio_visit_script_a_start();
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        if (girl_state_rank(1) == 4) {
            mode_step_set(0x1E);
        } else {
            normal_date_girl_in();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        if (!(state->girls[1].syoushin & 3)) {
            temp_v0_5 = girl_state_rank(1);
            switch (temp_v0_5) {
            case 0:
            case 1:
                talk_group = 0x9C;
                break;
            case 2:
                talk_group = 0x9D;
                break;
            case 3:
                talk_group = 0x9E;
                break;
            }
        } else {
            temp_v0_6 = girl_state_rank(1);
            switch (temp_v0_6) {
            case 0:
            case 1:
                talk_group = 0x90;
                break;
            case 2:
                talk_group = 0x91;
                break;
            case 3:
                talk_group = 0x92;
                break;
            }
            if (!(state->girls[1].flags.f.b2)) {
                talk_group += 3;
            }

            if (((u8) state->girls[1].flags.b[2] >> 4) == 1) {
                talk_group = talk_group + 6;
            }
            state->girls[1].flags.f.b2 = 1;
        }
        mode_step_next();
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        normal_date_girl_out();
        break;
    case 28:
        temp_v0_7 = girl_state_rank(1);
        switch (temp_v0_7) {
            case 0:
            case 1:
                talk_group = 0xD;
                break;
            case 2:
            case 3:
                talk_group = 0xC;
                break;
            }
        mode_step_next();
        break;
    case 29:
        normal_date_speak();
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
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_mio_visit_computer_fortune(void) {
    GameState *state;
    s32 temp_v0_6;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;
    u32 temp_v0_4;
    u32 temp_v0_5;

state = &game_state;
    switch (state->mode_step) {
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
            portrait_read_start(0x5D41);
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
            state->current_girl = GIRL_MIO;
            portrait_show_checked();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x12U, 0x801E8000, 0x4A97);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(1);
            switch (temp_v0) {
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
                talk_group = 5;
                break;
            }
            culture_mio_visit_script_b_start();
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
            talk_group = 0xE;

            break;
        case 2:
            talk_group = 0xF;
            break;
        case 3:
            talk_group = 0x10;
            break;
        case 4:
            talk_group = 0x11;
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
        if (girl_state_rank(1) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 14:
        audio_command(0x202);
        D_80122EB0 = (s32) state->mode_stage;
        D_80122EB4 = state->mode_step + 1;
        mode_stage_next();
        break;
    case 15:
        cd_read_sectors(0x12U, 0x801E8000, 0x4A97);
        mode_step_next();
        break;
    case 16:
        if (girl_state_rank(1) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = girl_state_rank(1);
            switch (temp_v0_3) {
            case 0:
            case 1:
                talk_group = 0x13;

                break;
            case 2:
                talk_group = 0x16;
                break;
            case 3:
                talk_group = 0x19;
                break;
            case 4:
                talk_group = 0x1C;
                break;
            }
            talk_group += D_80122EA8;
            culture_mio_visit_script_b_start();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        mode_step_next();
        if (!(state->girls[1].syoushin & 3)) {
            temp_v0_4 = girl_state_rank(1);
            switch (temp_v0_4) {
            case 0:
            case 1:
                talk_group = 0x2E;
                break;
            case 2:
                talk_group = 0x2F;
                break;
            case 3:
                talk_group = 0x30;
                break;
            case 4:
                mode_step_set(0x1A);
                break;
            }
        } else {
            temp_v0_5 = girl_state_rank(1);
            switch (temp_v0_5) {
            case 0:
            case 1:
                talk_group = 0x1F;
                break;
            case 2:
                talk_group = 0x20;
                break;
            case 3:
                talk_group = 0x21;
                break;
            case 4:
                mode_step_set(0x1A);
                break;
            }
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (!(state->girls[1].syoushin & 3)) {
            mode_step_set(0x17);
        } else {
            temp_v0_6 = girl_state_rank(1);
            switch (temp_v0_6) {
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
            }
            if (!(state->girls[1].flags.f.b2)) {
                talk_group += 3;
            }

            if (((u8) state->girls[1].flags.b[2] >> 4) == 1) {
                talk_group = talk_group + 6;
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
        if (!(state->girls[1].syoushin & 3)) {
            if (girl_state_rank(1) < 2) {
                talk_group = 0xD;
            } else {
                talk_group = 0xC;
            }

            mode_step_next();
        } else {
            state->girls[1].flags.f.b2 = 1;
            mode_step_set(0x1A);
        }
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

void culture_mio_visit_computer_game(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;
    u32 temp_v0_4;
    u32 temp_v0_5;

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
        portrait_read_step(0x5D41);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_MIO;
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
        cd_read_sectors(0x12U, 0x801E8000, 0x4A97);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(1);
            switch (temp_v0) {
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
                talk_group = 5;
                break;
            }
            culture_mio_visit_script_b_start();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        temp_v0_2 = girl_state_rank(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x31;
            break;
        case 2:
            talk_group = 0x39;
            break;
        case 3:
            talk_group = 0x41;
            break;
        case 4:
            talk_group = 0x49;
            break;
        }
        mode_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (girl_state_rank(1) == 4) {
            normal_date_girl_out();
        } else {
            mode_step_set(0xF);
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (girl_state_rank(1) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 16:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        mode_stage_set(CULTURE_STAGE_GAME_LAUNCH);
        break;
    case 17:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_set(CULTURE_STAGE_GAME_RESUME);
        break;
    case 18:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        culture_exhibit_title_show();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(1U);
        D_80122E98 = 0;
        mode_step_next();
        break;
    case 19:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 20:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 21:
        portrait_read_step(0x5D41);
        break;
    case 22:
        addr_init_bustup();
        game_state.current_girl = GIRL_MIO;
        portrait_show_checked();
        cd_read_sectors(0x12U, 0x801E8000, 0x4A97);
        break;
    case 23:
        if (girl_state_rank(1) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 24:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = girl_state_rank(1);
            switch (temp_v0_3) {
            case 0:
            case 1:
                talk_group = 0x32;
                break;
            case 2:
                talk_group = 0x3A;
                break;
            case 3:
                talk_group = 0x42;
                break;
            case 4:
                talk_group = 0x4B;
                break;
            }
            culture_mio_visit_script_b_start();
        }
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        mode_step_next();
        if (!(game_state.girls[1].syoushin & 3)) {
            temp_v0_4 = girl_state_rank(1);
            switch (temp_v0_4) {
            case 0:
            case 1:
                talk_group = 0x38;
                break;
            case 2:
                talk_group = 0x40;
                break;
            case 3:
                talk_group = 0x48;
                break;
            case 4:
                mode_step_set(0x1F);
                break;
            }
        } else {
            temp_v0_5 = girl_state_rank(1);
            switch (temp_v0_5) {
            case 0:
            case 1:
                talk_group = 0x33;
                break;
            case 2:
                talk_group = 0x3B;
                break;
            case 3:
                talk_group = 0x43;
                break;
            case 4:
                mode_step_set(0x1F);
                break;
            }
        }
        break;
    case 27:
        normal_date_speak();
        break;
    case 28:
        if (!(game_state.girls[1].syoushin & 3)) {
            normal_date_girl_out();
        } else {
            if (!(((GirlFlagBits *)&game_state.girls[1].flags)->b2)) {
                talk_group += 1;
            }
            if (((u8) game_state.girls[1].flags.b[2] >> 4) == 1) {
                talk_group += 2;
            }
            mode_step_next();
        }
        break;
    case 29:
        if (!(game_state.girls[1].syoushin & 3)) {
            if (girl_state_rank(1) < 2) {
                talk_group = 0xD;
            } else {
                talk_group = 0xC;
            }
            mode_step_next();
        } else {
            normal_date_speak();
        }
        break;
    case 30:
        if (!(game_state.girls[1].syoushin & 3)) {
            normal_date_speak();
        } else {
            normal_date_girl_out();
        }
        break;
    case 31:
        normal_date_bg_fadeout();
        break;
    case 32:
        talk_group = 0;
        mode_step_next();
        break;
    case 33:
        normal_date_speak();
        break;
    case 34:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_mio_visit_portrait_drawing(void) {
    GameState *game = &game_state;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game->mode_step) {
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
            portrait_read_start(0x5D41);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game->current_girl = GIRL_MIO;
            portrait_show_checked();
            talk_group = 0x2B;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x12U, 0x801E8000, 0x4A97);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(1);
            switch (temp_v0) {
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
                talk_group = 5;
                break;
            }
            culture_mio_visit_script_b_start();
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
            talk_group = 0x4D;

            break;
        case 2:
            talk_group = 0x50;
            break;
        case 3:
            talk_group = 0x53;
            break;
        case 4:
            talk_group = 0x56;
            break;
        }
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (girl_state_rank(1) == 4) {
            mode_step_set(0x12);
        } else {
            if ((u16) game->clubs[4].practice_points < 0x64U) {
                talk_group += 1;
            }
            mode_step_next();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if (!(game->girls[1].syoushin & 3)) {
            temp_v0_3 = girl_state_rank(1);
            switch (temp_v0_3) {
        case 0:
        case 1:
            talk_group = 0x67;
            break;
        case 2:
            talk_group = 0x68;
            break;
        case 3:
            talk_group = 0x69;
            break;
        }
        } else {
            temp_v0_4 = girl_state_rank(1);
            switch (temp_v0_4) {
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
        }
        }
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (!(game->girls[1].syoushin & 3)) {
            if (girl_state_rank(1) < 2) {
                talk_group = 0xD;
            } else {
                talk_group = 0xC;
            }

            mode_step_set(0x12);
        } else {
            temp_v0_5 = girl_state_rank(1);
            switch (temp_v0_5) {
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
            }
            if (!(game->girls[1].flags.f.b2)) {
                talk_group += 3;
            }

            /* FAKE: dead result test keeps the byte load temporary. T-10040. */
            if (temp_v0_5) {}
            if (((u8) game->girls[1].flags.b[2] >> 4) == 1) {
                talk_group = talk_group + 6;
            }
            game->girls[1].flags.f.b2 = 1;
            mode_step_next();
        }
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        normal_date_girl_out();
        break;
    case 19:
        if ((girl_state_rank(1) == 4) || !(game->girls[1].syoushin & 3)) {
            normal_date_speak();
        } else {
            mode_step_next();
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

void culture_mio_visit_open_sketching(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
            portrait_read_start(0x5D41);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_MIO;
            portrait_show_checked();
            talk_group = 0x2C;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x12U, 0x801E8000, 0x4A97);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(1);
            switch (temp_v0) {
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
                talk_group = 5;
                break;
            }
            culture_mio_visit_script_b_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        talk_group = 0x6A;
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = girl_state_rank(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x6B;
            break;
        case 2:
            talk_group = 0x6C;
            break;
        case 3:
            talk_group = 0x6D;
            break;
        case 4:
            talk_group = 0x6E;
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
        talk_group = 0x6F;
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

void culture_mio_visit_oil_painting_exhibit(void) {
    GameState *state;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    u32 temp_v0;

state = &game_state;
    switch (state->mode_step) {
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
            portrait_read_start(0x5D41);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            state->current_girl = GIRL_MIO;
            portrait_show_checked();
            talk_group = 0x2D;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x12U, 0x801E8000, 0x4A97);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(1);
            switch (temp_v0) {
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
                talk_group = 5;
                break;
            }
            culture_mio_visit_script_b_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(1) == 4) {
            talk_group = 0x70;
        } else if (!state->girls[1].flags.f.b2) {
            talk_group = 6;
        } else if ((state->girls[1].flags.b[2] >> 4) == 0) {
            talk_group = 0xA;
        } else {
            talk_group = 0xB;
        }
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (girl_state_rank(1) == 4) {
            mode_step_set(0x14);
        } else if (((u32) (state->girls[1].flags.w << 0x1D) >> 0x1F) == 1) {
            temp_v0_2 = girl_state_rank(1);
            switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x72;
            break;
        case 2:
            talk_group = 0x73;
            break;
        case 3:
            talk_group = 0x74;
            break;
        }
            mode_step_next();
        } else {
            mode_step_set(0xE);
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        temp_v0_3 = girl_state_rank(1);
        switch (temp_v0_3) {
        case 0:
        case 1:
            talk_group = 0x75;
            break;
        case 2:
            talk_group = 0x76;
            break;
        case 3:
            talk_group = 0x77;
            break;
        }
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (!(state->girls[1].syoushin & 3)) {
            temp_v0_4 = girl_state_rank(1);
            switch (temp_v0_4) {
        case 0:
        case 1:
            talk_group = 0x8A;
            break;
        case 2:
            talk_group = 0x8B;
            break;
        case 3:
            talk_group = 0x8C;
            break;
        }
        } else {
            temp_v0_5 = girl_state_rank(1);
            switch (temp_v0_5) {
        case 0:
        case 1:
            talk_group = 0x78;
            break;
        case 2:
            talk_group = 0x79;
            break;
        case 3:
            talk_group = 0x7A;
            break;
        }
        }
        mode_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if (!(state->girls[1].syoushin & 3)) {
            mode_step_set(0x14);
        } else {
            temp_v0_6 = girl_state_rank(1);
            switch (temp_v0_6) {
            case 0:
            case 1:
                talk_group = 0x7E;
                break;
            case 2:
                talk_group = 0x7F;
                break;
            case 3:
                talk_group = 0x80;
                break;
            }
            if (!(state->girls[1].flags.f.b2)) {
                talk_group += 3;
            }

            if (((u8) state->girls[1].flags.b[2] >> 4) == 1) {
                talk_group = talk_group + 6;
            }
            state->girls[1].flags.f.b2 = 1;
            mode_step_next();
        }
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        normal_date_girl_out();
        break;
    case 21:
        if (girl_state_rank(1) == 4) {
            mode_step_next();
        } else if (!(state->girls[1].syoushin & 3)) {
            if (girl_state_rank(1) < 2) {
                talk_group = 0xD;
            } else {
                talk_group = 0xC;
            }

            mode_step_next();
        } else {
            mode_step_set(0x17);
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

void culture_mio_visit_concert(void) {
    GameState *state;
    s32 choice3;
    s32 choice4;
    s32 choice5;
    u32 choice;
    u32 choice2;
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
            culture_backstage_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (state->year == 0x5F) {
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
        D_80122EAC = 2;
        D_80122EB0 = (s32) state->mode_stage;
        D_80122EB4 = state->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x5D41);
        break;
    case 9:
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        state->current_girl = GIRL_MIO;
        portrait_show_checked();
        cd_read_sectors(0x12U, 0x801E8000, 0x4A97);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            choice = girl_state_rank(1);
            switch (choice) {
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
            culture_mio_visit_script_b_start();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (girl_state_rank(1) == 4) {
            talk_group = 9;
        } else {
            talk_group = 0x8D;
        }
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        mode_step_next();
        choice2 = girl_state_rank(1);
        switch (choice2) {
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
            mode_step_set(0x15);
            break;
        }
        if (state->year == 0x60) {
            talk_group = talk_group + 0x18;
        } else {
            if (state->year == 0x61) {
                talk_group = talk_group + 0x1B;
            }
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (!(state->girls[1].syoushin & 3)) {
            choice3 = girl_state_rank(1);
            switch (choice3) {
            case 0:
            case 1:
                talk_group = 0xA3;
                break;
            case 2:
                talk_group = 0xA4;
                break;
            case 3:
                talk_group = 0xA5;
                break;
            }
        } else {
            choice4 = girl_state_rank(1);
            switch (choice4) {
            case 0:
            case 1:
                talk_group = 0x91;
                break;
            case 2:
                talk_group = 0x92;
                break;
            case 3:
                talk_group = 0x93;
                break;
            }
            if ((state->girls[1].flags.b[2] >> 4) == 1) {
                talk_group += 3;
            }
            if (state->year == 0x61) {
                if ((state->girls[1].flags.b[2] >> 4) == 0) {
                    talk_group = talk_group + 0x1B;
                }
            }
        }
        mode_step_next();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        choice5 = girl_state_rank(1);
        switch (choice5) {
        case 0:
        case 1:
            talk_group = 0x97;
            break;
        case 2:
            talk_group = 0x98;
            break;
        case 3:
            talk_group = 0x99;
            break;
        }
        if (!(state->girls[1].flags.f.b2)) {
            talk_group += 3;
        }
        if ((state->girls[1].flags.b[2] >> 4) == 1) {
            talk_group = talk_group + 6;
        } else {
            if (state->year == 0x61) {
                talk_group = talk_group + 0x18;
            }
        }
        mode_step_next();
        if (!(state->girls[1].syoushin & 3)) {
            mode_step_set(0x15);
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        normal_date_girl_out();
        break;
    case 22:
        if (girl_state_rank(1) == 4) {
            talk_group = 0xB5;
            mode_step_next();
        } else if (!(state->girls[1].syoushin & 3)) {
            if (girl_state_rank(1) < 2) {
                talk_group = 0xD;
            } else {
                talk_group = 0xC;
            }
            mode_step_next();
        } else {
            mode_step_set(0x18);
        }
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        normal_date_bg_fadeout();
        break;
    case 25:
        talk_group = 0;
        mode_step_next();
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_mio_visit_script_a_start(void) {
    talk_names_set(1);
    culture_script_set_mio_visit_a();
    talk_line = 0;
    talk_text_table = culture_script_text_table;
    talk_command_table = culture_script_command_table;
    talk_voice_table = culture_script_voice_table;
    mode_step_next();
}

void culture_mio_visit_script_b_start(void) {
    talk_names_set(1);
    culture_script_set_mio_visit_b();
    talk_line = 0;
    talk_text_table = culture_script_text_table;
    talk_command_table = culture_script_command_table;
    talk_voice_table = culture_script_voice_table;
    mode_step_next();
}
