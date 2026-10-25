/* date_entry.c: src/ovl/DATE2/80132000.c:4; amusement-park date resource/entry helpers and amusement_mode_stage_dispatch mode handler; main overlay loader routes game mode0x58 here, whereas0x50 belongs to DATE. Original object 0x80132000-0x801326F0 (config/objects/DATE2.txt). */
#include "common.h"
#include "ovl/amusement_park_date.h"

void amusement_scene_script_pointer_setup(void) {
    D_8013A380 = 0x80197410;
    D_8013A384 = 0x80198070;
    D_8013A388 = 0x80198CBC;
    D_8013A38C = 0x801999E4;
    D_8013A390 = 0x8019A6C0;
    D_8013A394 = 0x8019B404;
    D_8013A398 = 0x8019BFF8;
    D_8013A39C = 0x8019CADC;
    D_8013A3A0 = 0x8019D688;
    D_8013A3A4 = 0x8019E2AC;
    D_8013A3A8 = 0x8019EE88;
    D_8013A3AC = 0x80197558;
    D_8013A3B0 = 0x801981B8;
    D_8013A3B4 = 0x80198E04;
    D_8013A3B8 = 0x80199B2C;
    D_8013A3BC = 0x8019A808;
    D_8013A3C0 = 0x8019B54C;
    D_8013A3C4 = 0x8019C140;
    D_8013A3C8 = 0x8019CC1C;
    D_8013A3CC = 0x8019D7D0;
    D_8013A3D0 = 0x8019E3F4;
    D_8013A3D4 = 0x8019EF4C;
    D_8013A3D8 = 0x80197A78;
    D_8013A3DC = 0x801986D8;
    D_8013A3E0 = 0x80199324;
    D_8013A3E4 = 0x8019A04C;
    D_8013A3E8 = 0x8019AD60;
    D_8013A3EC = 0x8019BA6C;
    D_8013A3F0 = 0x8019C660;
    D_8013A3F4 = 0x8019D11C;
    D_8013A3F8 = 0x8019DD0C;
    D_8013A3FC = 0x8019E914;
    D_8013A400 = 0x8019F39C;
}

void amusement_mode_stage_dispatch(void) {
    portrait_wink_update();
    switch (game_state.mode_stage) {
    case 0:
        amusement_scene_callback_dispatch();
        break;
    case 1:
        amusement_mode_step_callback_dispatch();
        break;
    case 2:
        amusement_birthday_callback_dispatch();
        break;
    case 3:
        amusement_karaoke_saved_return_callback_dispatch();
        break;
    case 4:
        amusement_birthday_mode_stage4_step_callback_dispatch();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    check_k_scroll();
    k_disp_inc2();
    if ((game_state.mode_stage == 0) && ((u8) game_state.mode_step < 0x13U)) {
        parameter_show();
        dec_bg_draw(0);
    } else {
        hud_screen_frame_draw(2);
        scene_frame_update();
    }
    message_window_show();
    message_window_title_update();
    hizuke_show();
}

void amusement_scene_talk_run(void) {
    talk_tables = &amusement_dialogue_group;
    talk_active_line_ptr = &amusement_dialogue_line;
    talk_active_text_table = amusement_special_text_table;
    talk_active_command_table = amusement_special_command_table;
    talk_active_voice_table = amusement_special_voice_table;
    talk_run(talk_choice, 1, 0);
}

void amusement_scene_alt_talk_run(void) {
    talk_tables = &amusement_scene_dialogue_group;
    talk_active_line_ptr = &amusement_scene_dialogue_line;
    talk_active_text_table = D_8013A384;
    talk_active_command_table = D_8013A3B0;
    talk_active_voice_table = D_8013A3DC;
    talk_run(talk_choice, 1, 0);
}

void amusement_scene_talk_run_alt(void) {
    talk_tables = &amusement_alternate_dialogue_group;
    talk_active_line_ptr = &amusement_alternate_dialogue_line;
    talk_active_text_table = amusement_selected_text_table;
    talk_active_command_table = amusement_selected_command_table;
    talk_active_voice_table = amusement_selected_voice_table;
    talk_run(talk_choice, 1, 0);
}

void amusement_response_talk_run(void) {
    if (talk_choice != 0) {
        talk_tables = &amusement_alternate_dialogue_group;
        talk_active_line_ptr = &amusement_alternate_dialogue_line;
        talk_active_text_table = amusement_selected_text_table;
        talk_active_command_table = amusement_selected_command_table;
        talk_active_voice_table = amusement_selected_voice_table;
    } else {
        talk_tables = &amusement_scene_dialogue_group;
        talk_active_line_ptr = &amusement_scene_dialogue_line;
        talk_active_text_table = D_8013A384;
        talk_active_command_table = D_8013A3B0;
        talk_active_voice_table = D_8013A3DC;
    }
    talk_run(talk_choice, 1, 0);
}

void amusement_script_opening_talk_run(void) {
    talk_tables = &amusement_dialogue_group;
    talk_active_line_ptr = &amusement_dialogue_line;
    talk_active_text_table = amusement_special_text_table;
    talk_active_command_table = amusement_special_command_table;
    talk_active_voice_table = amusement_special_voice_table;
    talk_run(0xFF, 1, 0);
}

void amusement_script_response_talk_run(void) {
    talk_tables = &amusement_dialogue_group;
    talk_active_line_ptr = &amusement_dialogue_line;
    talk_active_text_table = amusement_special_text_table;
    talk_active_command_table = amusement_special_command_table;
    talk_active_voice_table = amusement_special_voice_table;
    talk_run(talk_choice, 1, 2);
}

void amusement_opening_talk_run_line_1(void) {
    talk_tables = (u8 *) &amusement_dialogue_group;
    talk_active_line_ptr = (u8 *) &amusement_dialogue_line;
    talk_active_text_table = amusement_special_text_table;
    talk_active_command_table = amusement_special_command_table;
    talk_active_voice_table = amusement_special_voice_table;
    talk_run(talk_choice, 1, 1);
}
