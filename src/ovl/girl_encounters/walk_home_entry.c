/* walk_home_entry.c: src/ovl/GEKO/80132000.c:4,43,65; binds walk-home script sections, dispatches approach/follow-up mode stages and adapts the talk runner. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x80132000-0x801323E0 (config/objects/GEKO.txt). */
#include "common.h"
#include "ovl/girl_encounters.h"

void encounter_walk_home_scripts_bind(void) {
    encounter_walk_home_text_section_0 = 0x80197BFC;
    encounter_walk_home_text_section_1 = 0x801996B8;
    encounter_walk_home_text_section_2 = 0x8019B050;
    encounter_walk_home_text_section_3 = 0x8019C9A8;
    encounter_walk_home_text_section_4 = 0x8019E3F0;
    encounter_walk_home_text_section_5 = 0x8019FF58;
    encounter_walk_home_text_section_6 = 0x801A18FC;
    encounter_walk_home_text_section_7 = 0x801A3030;
    encounter_walk_home_text_section_8 = 0x801A48F4;
    encounter_walk_home_text_section_9 = 0x801A63EC;
    encounter_walk_home_text_section_10 = 0x801A7324;
    encounter_walk_home_text_section_11 = 0x801A7F90;
    encounter_walk_home_command_section_0 = 0x80197D80;
    encounter_walk_home_command_section_1 = 0x80199838;
    encounter_walk_home_command_section_2 = 0x8019B1C8;
    encounter_walk_home_command_section_3 = 0x8019CB2C;
    encounter_walk_home_command_section_4 = 0x8019E584;
    encounter_walk_home_command_section_5 = 0x801A00D8;
    encounter_walk_home_command_section_6 = 0x801A1A60;
    encounter_walk_home_command_section_7 = 0x801A31AC;
    encounter_walk_home_command_section_8 = 0x801A4A74;
    encounter_walk_home_command_section_9 = 0x801A657C;
    encounter_walk_home_command_section_10 = 0x801A7354;
    encounter_walk_home_command_section_11 = 0x801A8108;
    encounter_walk_home_voice_section_0 = 0x80198980;
    encounter_walk_home_voice_section_1 = 0x8019A3C8;
    encounter_walk_home_voice_section_2 = 0x8019BD20;
    encounter_walk_home_voice_section_3 = 0x8019D6BC;
    encounter_walk_home_voice_section_4 = 0x8019F1C4;
    encounter_walk_home_voice_section_5 = 0x801A0C84;
    encounter_walk_home_voice_section_6 = 0x801A24F4;
    encounter_walk_home_voice_section_7 = 0x801A3D04;
    encounter_walk_home_voice_section_8 = 0x801A5674;
    encounter_walk_home_voice_section_9 = 0x801A7134;
    encounter_walk_home_voice_section_10 = 0x801A7414;
    encounter_walk_home_voice_section_11 = 0x801A8C28;
}

void encounter_walk_home_mode_handler(void) {
    portrait_wink_update();
    switch (game_state.mode_stage) {
    case 0:
        encounter_walk_home_approach_dispatch();
        break;
    case 1:
        encounter_walk_home_followup_dispatch();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    check_k_scroll();
    k_disp_inc2();
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    hizuke_show();
    scene_frame_update();
}

s32 encounter_walk_home_talk(void) {
    talk_tables = &encounter_walk_home_group;
    talk_active_line_ptr = &encounter_walk_home_line;
    talk_active_text_table = encounter_walk_home_text_table;
    talk_active_command_table = encounter_walk_home_command_table;
    talk_active_voice_table = encounter_walk_home_voice_table;
    talk_run(talk_choice, 1, 0);
}

void encounter_walk_home_choice_talk(void) {
    talk_tables = (u8 *) &encounter_walk_home_group;
    talk_active_line_ptr = &encounter_walk_home_line;
    talk_active_text_table = encounter_walk_home_text_table;
    talk_active_command_table = encounter_walk_home_command_table;
    talk_active_voice_table = encounter_walk_home_voice_table;
    talk_run(0xFF, 1, 0);
}
