/* player_birthday.c: src/ovl/DATE/80145AA0.c:4: stage 3: the girl greets the player on his birthday. Original object 0x80145AA0-0x801461B0 (config/objects/DATE.txt). */
#include "common.h"
#include "ovl/date.h"

void date_player_birthday_sections_init(void) {
    D_8015CE80 = 0x801B00DC;
    D_8015CE84 = 0x801B02AC;
    D_8015CE88 = 0x801B03C8;
    D_8015CE8C = 0x801B04C4;
    D_8015CE90 = 0x801B0668;
    D_8015CE94 = 0x801B0808;
    D_8015CE98 = 0x801B09DC;
    D_8015CE9C = 0x801B0BE8;
    D_8015CEA0 = 0x801B0D8C;
    D_8015CEA4 = 0x801B0F5C;
    D_8015CEA8 = 0x801B1164;
    D_8015CEAC = 0x801B12D8;
    D_8015CEB0 = 0x801B1454;
    D_8015CEB4 = 0x801B00F4;
    D_8015CEB8 = 0x801B02C8;
    D_8015CEBC = 0x801B03D0;
    D_8015CEC0 = 0x801B04DC;
    D_8015CEC4 = 0x801B0680;
    D_8015CEC8 = 0x801B0820;
    D_8015CECC = 0x801B09F8;
    D_8015CED0 = 0x801B0C00;
    D_8015CED4 = 0x801B0DA4;
    D_8015CED8 = 0x801B0F78;
    D_8015CEDC = 0x801B1180;
    D_8015CEE0 = 0x801B12E0;
    D_8015CEE4 = 0x801B1470;
    D_8015CEE8 = 0x801B01A8;
    D_8015CEEC = 0x801B0398;
    D_8015CEF0 = 0x801B03F0;
    D_8015CEF4 = 0x801B0590;
    D_8015CEF8 = 0x801B0734;
    D_8015CEFC = 0x801B08D4;
    D_8015CF00 = 0x801B0AE4;
    D_8015CF04 = 0x801B0CB4;
    D_8015CF08 = 0x801B0E58;
    D_8015CF0C = 0x801B1064;
    D_8015CF10 = 0x801B126C;
    D_8015CF14 = 0x801B1338;
    D_8015CF18 = 0x801B155C;
}

void date_mode_stage_player_birthday(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    FnTbl15 step_handlers;

    step_handlers = date_player_birthday_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
    if ((D_80122D44 & 1) && (bg_brightness == 0x80)) {
        hud_sprite_light_effect_draw();
    }
}

void date_player_birthday_load_data(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    date_player_birthday_sections_init();
    mode_step_next();
}

void date_player_birthday_start(void) {
    u8 girl_state_flags;

    game_state.day_event_done_bits |= 8;
    girl_state_flags = get_g_zyotai_s(game_state.current_girl);
    if ((girl_state_flags & 0x7F) >= 2U) {
        mode_step_set(9);
        return;
    }
    game_state.day_event_done_bits |= 4;
    event_log_add(0xD, talk_date_spot);
    mode_step_next();
}

void date_player_birthday_prepare(void) {
    date_player_birthday_group = 1;
    date_player_birthday_line = 0;
    talk_names_set(game_state.current_girl);
    date_player_birthday_sections_init();
    date_player_birthday_pick_sections();
    mode_step_next();
}

void date_player_birthday_pick_sections(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        date_player_birthday_text = D_8015CE84;
        date_player_birthday_command = D_8015CEB8;
        date_player_birthday_voice = D_8015CEEC;
        return;
    case GIRL_MIO:
        date_player_birthday_text = D_8015CE98;
        date_player_birthday_command = D_8015CECC;
        date_player_birthday_voice = D_8015CF00;
        return;
    case GIRL_YUINA:
        date_player_birthday_text = D_8015CE8C;
        date_player_birthday_command = D_8015CEC0;
        date_player_birthday_voice = D_8015CEF4;
        return;
    case GIRL_AYAKO:
        date_player_birthday_text = D_8015CE90;
        date_player_birthday_command = D_8015CEC4;
        date_player_birthday_voice = D_8015CEF8;
        return;
    case GIRL_SAKI:
        date_player_birthday_text = D_8015CEA8;
        date_player_birthday_command = D_8015CEDC;
        date_player_birthday_voice = D_8015CF10;
        return;
    case GIRL_YUKARI:
        date_player_birthday_text = D_8015CE9C;
        date_player_birthday_command = D_8015CED0;
        date_player_birthday_voice = D_8015CF04;
        return;
    case GIRL_NOZOMI:
        date_player_birthday_text = D_8015CEA0;
        date_player_birthday_command = D_8015CED4;
        date_player_birthday_voice = D_8015CF08;
        return;
    case GIRL_MIRA:
        date_player_birthday_text = D_8015CE94;
        date_player_birthday_command = D_8015CEC8;
        date_player_birthday_voice = D_8015CEFC;
        return;
    case GIRL_YUKO:
        date_player_birthday_text = D_8015CE80;
        date_player_birthday_command = D_8015CEB4;
        date_player_birthday_voice = D_8015CEE8;
        return;
    case GIRL_MEGUMI:
        date_player_birthday_text = D_8015CEA4;
        date_player_birthday_command = D_8015CED8;
        date_player_birthday_voice = D_8015CF0C;
        return;
    case GIRL_YUMI:
        date_player_birthday_text = D_8015CEB0;
        date_player_birthday_command = D_8015CEE4;
        date_player_birthday_voice = D_8015CF18;
        /* fallthrough */
    default:
        return;
    }
}

void date_player_birthday_speak(void) {
    talk_tables = &date_player_birthday_group;
    talk_active_line_ptr = &date_player_birthday_line;
    talk_active_text_table = date_player_birthday_text;
    talk_active_command_table = date_player_birthday_command;
    talk_active_voice_table = date_player_birthday_voice;
    talk_run(talk_choice, 1, 0);
}
