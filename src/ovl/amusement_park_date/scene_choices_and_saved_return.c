/* scene_choices_and_saved_return.c: src/ovl/DATE2/80137360.c:4; local script/choice flow and saved-step return, with a player-birthday test in amusement_birthday_scene_gate; no unproved girl-birthday identity is assigned. Original object 0x80137360-0x80137D50 (config/objects/DATE2.txt). */
#define MAIN_API_OVERRIDE_portrait_layout_set /* matched with void(s32) (main_api.h: void(u8)) */
#include "common.h"
#include "ovl/amusement_park_date.h"

void portrait_layout_set(s32 arg0);

void amusement_karaoke_dialogue_pointer_setup(void) {
    D_8013A770 = 0x801B0478;
    D_8013A774 = 0x801B13A4;
    D_8013A778 = 0x801B2998;
    D_8013A77C = 0x801B38C8;
    D_8013A780 = 0x801B4258;
    D_8013A784 = 0x801B4B98;
    D_8013A788 = 0x801B5574;
    D_8013A78C = 0x801B6560;
    D_8013A790 = 0x801B7B74;
    D_8013A794 = 0x801B8B18;
    D_8013A798 = 0x801B93D0;
    D_8013A79C = 0x801B9DA0;
    D_8013A7A0 = 0x801BA780;
    D_8013A7A4 = 0x801B0510;
    D_8013A7A8 = 0x801B14E0;
    D_8013A7AC = 0x801B2AD0;
    D_8013A7B0 = 0x801B3960;
    D_8013A7B4 = 0x801B42F0;
    D_8013A7B8 = 0x801B4C38;
    D_8013A7BC = 0x801B560C;
    D_8013A7C0 = 0x801B66A0;
    D_8013A7C4 = 0x801B7CB4;
    D_8013A7C8 = 0x801B8BB0;
    D_8013A7CC = 0x801B9468;
    D_8013A7D0 = 0x801B9EA8;
    D_8013A7D4 = 0x801BA818;
    D_8013A7D8 = 0x801B0888;
    D_8013A7DC = 0x801B1E14;
    D_8013A7E0 = 0x801B33B0;
    D_8013A7E4 = 0x801B3CF4;
    D_8013A7E8 = 0x801B4684;
    D_8013A7EC = 0x801B4FEC;
    D_8013A7F0 = 0x801B59F4;
    D_8013A7F4 = 0x801B6FB8;
    D_8013A7F8 = 0x801B85CC;
    D_8013A7FC = 0x801B8F28;
    D_8013A800 = 0x801B97E0;
    D_8013A804 = 0x801BA2C8;
    D_8013A808 = 0x801BAB90;
}

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 D_8013A81C;

void amusement_karaoke_saved_return_callback_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_8013A81C;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void amusement_karaoke_mode_stage_restore(void) {
    mode_stage_set(2);
    mode_step_set(game_state.saved_mode_step);
}

void amusement_karaoke_sector_load(void) {
    cd_read_sectors(0x16, 0x801B0000, 0xAF0D);
    amusement_karaoke_dialogue_pointer_setup();
    mode_step_next();
}

void amusement_karaoke_response_save(void) {
    talk_choice = amusement_saved_selection_index;
    mode_step_next();
}

void amusement_karaoke_dialogue_table_bind(void) {
    amusement_saved_selection_index = talk_choice;
    talk_group = 0;
    talk_line = 0;
    talk_text_table = D_8013A774;
    talk_command_table = D_8013A7A8;
    talk_voice_table = D_8013A7DC;
    mode_step_next();
}

void amusement_choice_parameter_penalty(void) {
    if (talk_choice != 0) {
        game_state.girls[game_state.current_girl].affection -= 1;
        game_state.girls[game_state.current_girl].syoushin += 5;
        check_para_limit();
        game_state.mode_step += 0xD;
        return;
    }
    talk_group += 1;
    mode_step_next();
}

void amusement_status_talk_group_select(void) {
    u32 t;

    t = (u8)get_g_zyotai_s(game_state.current_girl) & 0x7F;
    if (t < 2) {
        talk_group = 6;
    } else if (t == 2) {
        talk_group = 8;
    } else if (t == 3) {
        talk_group = 0xA;
    } else {
        talk_group = 0xC;
    }
    talk_line = 1;
    mode_step_next();
}

void amusement_response_group_text_insert(void) {
    s32 pad; /* FAKE: unused local above buf, puts buf at sp+0x28 as in the original; real source unknown. T-4010 */
    u8 buf[3] = "こ";

    talk_group = talk_choice * 2 + 0xD;
    if (talk_choice == 0) {
        (**(u8 ***)(talk_text_table + talk_group * 4 + 4))[0] = buf[0];
        (**(u8 ***)(talk_text_table + talk_group * 4 + 4))[1] = buf[1];
    }
    mode_step_next();
}

void amusement_birthday_scene_gate(void) {
    u32 t;

    t = (u8)get_g_zyotai_s(GIRL_SHIORI) & 0x7F;
    if (game_state.month == (game_state.player_birthday & 0xF) && game_state.day == ((u32)(game_state.player_birthday << 0x17) >> 0x1B) && t < 2) {
        game_state.day_event_done_bits |= 4;
        mode_stage_set(4);
        return;
    }
    mode_step_next();
}

void amusement_karaoke_special_dialogue_setup(void) {
    talk_group = 0x1D;
    talk_text_table = amusement_saved_text_table;
    talk_command_table = amusement_saved_command_table;
    talk_voice_table = amusement_saved_voice_table;
    amusement_karaoke_indexed_text_select();
    mode_step_next();
}

typedef struct {
    u8 s[0xCC3];
} DateTxt; /* size 0xCC3 */
extern DateTxt D_8013A878;

void amusement_karaoke_indexed_text_select(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    DateTxt tbl;

    tbl = D_8013A878;
    strcpy(name_code_a, &tbl.s[game_state.current_girl * 0x129 + game_state.year * 0x63 + talk_choice * 0x21 - 0x24BD]);
}

void amusement_karaoke_dialogue_pointer_swap(void) {
    amusement_saved_text_table = talk_text_table;
    amusement_saved_command_table = talk_command_table;
    amusement_saved_voice_table = talk_voice_table;
    talk_group = 0;
    talk_line = (talk_line + game_state.year) - 0x5F;
    talk_text_table = D_8013A79C;
    talk_command_table = D_8013A7D0;
    talk_voice_table = D_8013A804;
    mode_step_next();
}

void amusement_response_parameter_update_alt(void) {
    portrait_layout_set(talk_choice + 1);
    if (talk_choice == 0) {
        game_state.girls[game_state.current_girl].tokimeki += 3;
        game_state.girls[game_state.current_girl].affection += 2;
        game_state.girls[game_state.current_girl].syoushin -= 0xF;
    } else if (talk_choice == 1) {
        game_state.girls[game_state.current_girl].tokimeki += 2;
        game_state.girls[game_state.current_girl].affection += 1;
        game_state.girls[game_state.current_girl].syoushin -= 0xA;
    } else {
        game_state.girls[game_state.current_girl].affection += 1;
        game_state.girls[game_state.current_girl].syoushin += 0xA;
    }
    check_para_limit();
    mode_step_next();
}
