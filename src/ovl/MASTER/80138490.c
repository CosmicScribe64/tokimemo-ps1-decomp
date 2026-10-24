#define MAIN_API_OVERRIDE_Vblnk_Timer_Init /* implicit declaration, as matched (main_api.h: void(void)) */
#include "ovl/MASTER.h"

void func_80138490(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    bustup_wink_update();
    switch (game_state.step) {
    case 0:
        game_state.day_event_index += 1;
        game_state.daily_event_bits |= 1;
        snd_command(0xCF);
        Vblnk_Timer_Init();
        if ((game_state.month == (game_state.player_birthday & 0xF)) && (game_state.day == ((u32) (game_state.player_birthday << 0x17) >> 0x1B))) {
            if (!(game_state.daily_event_bits & 4)) {
                if (girl_state_rank(0) < 2) {
                    game_state.daily_event_bits |= 4;
                    cd_read_sectors(3U, 0x801B0000, 0xAF43);
                    main_step_set(0x1B);
                } else {
                    goto block_7;
                }
            } else {
                goto block_8;
            }
        } else {
block_7:
block_8:
            cd_read_sectors(3U, 0x801E8000, 0x49B7);
            main_step_next();
        }
        break;
    case 1:
        normal_date_move_place();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            snd_bank_load_start(1, 0xC043, 0xC01D, 0xCA95, 0xCA4F, 0xCA3E);
            game_state.current_girl = GIRL_SHIORI;
            dialogue_names_set(0U);
            func_801336E8();
            dialogue_group = 0;
            dialogue_line = 0;
            dialogue_text_table = (s32) D_8013C4A0;
            dialogue_command_table = (s32) D_8013C4A4;
            dialogue_voice_table = (s32) D_8013C4A8;
            main_step_next();
        }
        break;
    case 3:
        normal_date_speak();
        break;
    case 4:
        normal_date_two_select();
        break;
    case 5:
        if ((*(u32 *)&D_80122CDC) != 0) {
            normal_date_speak();
        } else {
            main_step_set(7);
        }
        break;
    case 6:
        game_state.girls[0].affection -= 1;
        game_state.girls[0].syoushin += 5;
        main_stage_next();
        break;
    case 7:
        if (snd_bank_load_poll() == 1) {
            snd_command(0x200);
            bustup_read_start(0x541D);
            dialogue_group = 2;
            main_step_next();
        }
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        dialogue_group = 5;
        main_step_next();
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        normal_date_three_select();
        break;
    case 12:
        func_80138E58();
        dialogue_group = 6;
        main_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            bustup_show_checked();
        }
        break;
    case 15:
        dialogue_group = 8;
        main_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_in();
        break;
    case 18:
        temp_v0 = girl_state_rank(0);
        switch (temp_v0) {
        case 0:
        case 1:
            dialogue_group = 0xF;
            break;
        case 2:
            dialogue_group = 0xF;
            break;
        case 3:
            dialogue_group = 0x10;
            break;
        case 4:
            dialogue_group = 0x11;
            break;
        }
        main_step_next();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        temp_v0_2 = girl_state_rank(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 0x17;
            break;
        case 2:
            dialogue_group = 0x18;
            break;
        case 3:
            dialogue_group = 0x19;
            break;
        case 4:
            dialogue_group = 0x1A;
            break;
        }
        main_step_next();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        switch ((*(u32 *)&D_80122CDC)) {
        case 0:
            dialogue_group = 0x1B;
            bustup_layout_set(1U);
            game_state.girls[0].tokimeki += 3;
            game_state.girls[0].affection += 2;
            game_state.girls[0].syoushin -= 0xF;
            break;
        case 1:
            dialogue_group = 0x1D;
            bustup_layout_set(2U);
            game_state.girls[0].tokimeki += 2;
            game_state.girls[0].affection += 1;
            game_state.girls[0].syoushin -= 0xA;
            break;
        case 2:
            dialogue_group = 0x1F;
            bustup_layout_set(3U);
            game_state.girls[0].affection += 1;
            game_state.girls[0].syoushin += 0xA;
            break;
        }
        func_80063668();
        main_step_next();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        normal_date_girl_out();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        event_log_add(0x15, 6);
        main_stage_next();
        break;
    case 27:
        game_state.day_event_index += 1;
        normal_date_move_place();
        break;
    case 28:
        if (cd_read_sync_status() & 1) {
            game_state.current_girl = GIRL_SHIORI;
            dialogue_names_set(0U);
            func_801337B8();
            dialogue_group = 0;
            dialogue_line = 0;
            dialogue_text_table = D_8013C4D8;
            dialogue_command_table = D_8013C50C;
            dialogue_voice_table = D_8013C540;
            bustup_read_start(0x541D);
            main_step_next();
        }
        break;
    case 29:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            bustup_show_checked();
            snd_command(0xBF);
            snd_bank_load_start(1, 0xC043, 0xC01D, 0xCA95, 0xCA4F, 0xCA3E);
        }
        break;
    case 30:
        if (snd_bank_load_poll() == 1) {
            snd_command(0x200);
            main_step_next();
        }
        break;
    case 31:
        normal_date_speak();
        break;
    case 32:
        normal_date_girl_in();
        break;
    case 33:
        normal_date_speak();
        break;
    case 34:
        cd_read_sectors(3U, 0x801E8000, 0x49B7);
        main_step_next();
        break;
    case 35:
        if (cd_read_sync_status() & 1) {
            func_801336E8();
            dialogue_group = 0xC;
            dialogue_line = 0;
            dialogue_text_table = (s32) D_8013C4A0;
            dialogue_command_table = (s32) D_8013C4A4;
            dialogue_voice_table = (s32) D_8013C4A8;
            main_step_next();
        }
        break;
    case 36:
        normal_date_speak();
        break;
    case 37:
        dialogue_group = 0x21;
        main_step_next();
        break;
    case 38:
        normal_date_speak();
        break;
    case 39:
        normal_date_three_select();
        break;
    case 40:
        dialogue_group = 0x17;
        main_step_next();
        break;
    case 41:
        normal_date_speak();
        break;
    case 42:
        switch ((*(u32 *)&D_80122CDC)) {
        case 0:
            dialogue_group = 0x1B;
            bustup_layout_set(1U);
            game_state.girls[0].tokimeki += 3;
            game_state.girls[0].affection += 2;
            game_state.girls[0].syoushin -= 0xF;
            break;
        case 1:
            dialogue_group = 0x1D;
            bustup_layout_set(2U);
            game_state.girls[0].tokimeki += 2;
            game_state.girls[0].affection += 1;
            game_state.girls[0].syoushin -= 0xA;
            break;
        case 2:
            dialogue_group = 0x1F;
            bustup_layout_set(3U);
            game_state.girls[0].affection += 1;
            game_state.girls[0].syoushin += 0xA;
            break;
        }
        func_80063668();
        main_step_next();
        break;
    case 43:
        normal_date_speak();
        break;
    case 44:
        normal_date_girl_out();
        break;
    case 45:
        normal_date_speak();
        break;
    case 46:
        event_log_add(0x15, 6);
        main_stage_next();
        break;
    default:
        game_reset_to_title();
        break;
    }
    Default_Disp();
}

void func_80138E58(void) {
    s32 idx; /* unused: its stack slot sits above tbl (T-3330) */
    PtrTbl3 tbl;

    tbl = D_8013C700;
    strcpy(D_800CA25C, tbl.p[D_80122CDC]);
}

INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80138490.rodata", D_8013BFB0);

INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80138490.rodata", D_8013BFBC);

INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80138490.rodata", D_8013BFC8);
