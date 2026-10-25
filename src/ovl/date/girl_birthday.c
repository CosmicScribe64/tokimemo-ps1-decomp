/* girl_birthday.c: src/ovl/DATE/80149310.c:62: stage 2: gift choice on the girl's birthday. Original object 0x80149310-0x8014A1B0 (config/objects/DATE.txt). */
#include "common.h"
#include "ovl/date.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015CFD0);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015CFD4);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015CFD8);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015CFDC);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015CFE0);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015CFE4);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015CFE8);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015CFEC);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015CFF0);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015CFF4);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015CFF8);
s32 D_8015CFFC = 0x801B9DA0;
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D000);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D004);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D008);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D00C);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D010);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D014);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D018);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D01C);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D020);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D024);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D028);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D02C);
s32 D_8015D030 = 0x801B9EA8;
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D034);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D038);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D03C);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D040);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D044);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D048);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D04C);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D050);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D054);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D058);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D05C);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D060);
s32 D_8015D064 = 0x801BA2C8;
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", D_8015D068);
s32 date_gift_text = 0;
s32 date_gift_command = 0;
s32 date_gift_voice = 0;
s32 date_gift_saved_text = 0;
s32 date_gift_saved_command = 0;
s32 date_gift_saved_voice = 0;
s16 date_gift_group = 0;
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", date_gift_line);
s16 date_gift_saved_group = 0;
u8 date_school_year = 0;
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", date_gift_choice);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", date_girl_birthday_steps);
INCLUDE_RODATA("asm/ovl/DATE/data/date/girl_birthday.data", date_gift_names);

typedef struct {
    u8 text_bytes[0xCC3];
} DateGiftNameTable; /* size 0xCC3 */

void date_gift_sections_init(void) {
    D_8015CFD0 = 0x801B0478;
    D_8015CFD4 = 0x801B13A4;
    D_8015CFD8 = 0x801B2998;
    D_8015CFDC = 0x801B38C8;
    D_8015CFE0 = 0x801B4258;
    D_8015CFE4 = 0x801B4B98;
    D_8015CFE8 = 0x801B5574;
    D_8015CFEC = 0x801B6560;
    D_8015CFF0 = 0x801B7B74;
    D_8015CFF4 = 0x801B8B18;
    D_8015CFF8 = 0x801B93D0;
    D_8015CFFC = 0x801B9DA0;
    D_8015D000 = 0x801BA780;
    D_8015D004 = 0x801B0510;
    D_8015D008 = 0x801B14E0;
    D_8015D00C = 0x801B2AD0;
    D_8015D010 = 0x801B3960;
    D_8015D014 = 0x801B42F0;
    D_8015D018 = 0x801B4C38;
    D_8015D01C = 0x801B560C;
    D_8015D020 = 0x801B66A0;
    D_8015D024 = 0x801B7CB4;
    D_8015D028 = 0x801B8BB0;
    D_8015D02C = 0x801B9468;
    D_8015D030 = 0x801B9EA8;
    D_8015D034 = 0x801BA818;
    D_8015D038 = 0x801B0888;
    D_8015D03C = 0x801B1E14;
    D_8015D040 = 0x801B33B0;
    D_8015D044 = 0x801B3CF4;
    D_8015D048 = 0x801B4684;
    D_8015D04C = 0x801B4FEC;
    D_8015D050 = 0x801B59F4;
    D_8015D054 = 0x801B6FB8;
    D_8015D058 = 0x801B85CC;
    D_8015D05C = 0x801B8F28;
    D_8015D060 = 0x801B97E0;
    D_8015D064 = 0x801BA2C8;
    D_8015D068 = 0x801BAB90;
}

void date_mode_stage_girl_birthday(void) {
    s32 step_index; /* declared before step_handlers: its stack slot sits above step_handlers (T-3330) */
    DateGirlBirthdaySteps step_handlers;

    step_handlers = date_girl_birthday_steps;
    step_index = game_state.mode_step;
    step_handlers.callbacks[step_index](0x80);
    if ((D_80122D44 & 1) && (bg_brightness == 0x80)) {
        hud_sprite_light_effect_draw();
    }
}

void date_gift_group_reset(void) {
    date_gift_group = 2;
    date_gift_line = 0;
    mode_step_next();
}

void date_gift_load_data(void) {
    cd_read_sectors(0x16, 0x801B0000, 0xAF0D);
    date_gift_sections_init();
    mode_step_next();
}

void date_gift_choice_restore(void) {
    talk_choice = date_gift_choice;
    mode_step_next();
}

s32 date_gift_start(void) {
    GameState *game = &game_state;

    if (!game->girls[game->current_girl].flags.f.b6) {
        game->mode_step += 0x11;
        return 0;
    }
    if (game->current_girl == GIRL_SHIORI) {
        game->day_event_done_bits |= 1;
    } else {
        game->day_event_done_bits |= 2;
    }
    if (((D_800CA2F8 == 0) || (D_800CA2F8 == 0x12)) && (game->event_log_count < 0xFE)) {
        game->event_log_count -= 1;
    }
    event_log_add(0xA, talk_date_spot);
    date_gift_choice = talk_choice;
    date_gift_group = 0x1D;
    date_gift_line = 0;
    talk_names_set(game->current_girl);
    switch (game->current_girl) {
    case GIRL_SHIORI:
        date_gift_text = D_8015CFD4;
        date_gift_command = D_8015D008;
        date_gift_voice = D_8015D03C;
        break;
    case GIRL_MIO:
        date_gift_text = D_8015CFE4;
        date_gift_command = D_8015D018;
        date_gift_voice = D_8015D04C;
        break;
    case GIRL_YUINA:
        date_gift_text = D_8015CFD8;
        date_gift_command = D_8015D00C;
        date_gift_voice = D_8015D040;
        break;
    case GIRL_AYAKO:
        date_gift_text = D_8015CFDC;
        date_gift_command = D_8015D010;
        date_gift_voice = D_8015D044;
        break;
    case GIRL_SAKI:
        date_gift_text = D_8015CFF8;
        date_gift_command = D_8015D02C;
        date_gift_voice = D_8015D060;
        break;
    case GIRL_YUKARI:
        date_gift_text = D_8015CFE8;
        date_gift_command = D_8015D01C;
        date_gift_voice = D_8015D050;
        break;
    case GIRL_NOZOMI:
        date_gift_text = D_8015CFF0;
        date_gift_command = D_8015D024;
        date_gift_voice = D_8015D058;
        /* (u8) as in the SHUGAKU twin func_8013282C: a plain compare gives slti and moves every later temporary. T-17205 */
        if ((u8)(get_g_zyotai_s(GIRL_NOZOMI) & 0x7F) < 2) {
            date_gift_text = D_8015CFEC;
            date_gift_command = D_8015D020;
            date_gift_voice = D_8015D054;
        }
        break;
    case GIRL_MIRA:
        date_gift_text = D_8015CFE0;
        date_gift_command = D_8015D014;
        date_gift_voice = D_8015D048;
        break;
    case GIRL_YUKO:
        date_gift_text = D_8015CFD0;
        date_gift_command = D_8015D004;
        date_gift_voice = D_8015D038;
        break;
    case GIRL_MEGUMI:
        date_gift_text = D_8015CFF4;
        date_gift_command = D_8015D028;
        date_gift_voice = D_8015D05C;
        break;
    case GIRL_YUMI:
        date_gift_text = D_8015D000;
        date_gift_command = D_8015D034;
        date_gift_voice = D_8015D068;
    }
    if (game->month >= 4) {
        date_school_year = game->year - 0x5F;
    } else {
        date_school_year = game->year - 0x60;
    }
    mode_step_next();
}

void date_gift_reaction_group_by_mood(void) {
    u32 girl_state;

    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    girl_state = (u8)get_g_zyotai_s(game_state.current_girl) & 0x7F;
    if (girl_state < 2U) {
        date_gift_group += 2;
    } else if (girl_state == 2) {
        date_gift_group += 4;
    } else if (girl_state == 3) {
        date_gift_group += 6;
    } else {
        date_gift_group += 8;
    }
    date_gift_line = 1;
    mode_step_next();
}

void date_gift_pick_reply_group(void) {
    s32 fake_padding; /* FAKE: unused local above reply_prefix, puts reply_prefix at sp+0x28 as in the original; real source unknown. T-4010 */
    u8 reply_prefix[3] = "こ";

    date_gift_group = talk_choice * 2 + 0xD;
    if (talk_choice == 0) {
        (**(u8 ***)(date_gift_text + date_gift_group * 4 + 4))[0] = reply_prefix[0];
        (**(u8 ***)(date_gift_text + date_gift_group * 4 + 4))[1] = reply_prefix[1];
    }
    mode_step_next();
}

void date_gift_player_birthday_check(void) {
    if ((game_state.month == (game_state.player_birthday & 0xF)) && (game_state.day == ((u32)(game_state.player_birthday << 0x17) >> 0x1B))) {
        if ((u8)game_state.event_log_count < 0xFEU) {
            game_state.event_log_count -= 1;
        }
        game_mode_set(GAME_MODE_DATE);
        mode_stage_set(3);
        return;
    }
    mode_step_next();
}

void date_gift_prepare_choice(void) {
    date_gift_group = date_gift_saved_group;
    date_gift_text = date_gift_saved_text;
    date_gift_command = date_gift_saved_command;
    date_gift_voice = date_gift_saved_voice;
    date_gift_name_set();
    mode_step_next();
}

extern DateGiftNameTable date_gift_names;

void date_gift_name_set(void) {
    s32 fake_padding; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    DateGiftNameTable gift_names;

    gift_names = date_gift_names;
    strcpy(name_code_a, &gift_names.text_bytes[game_state.current_girl * 0x129 + date_school_year * 0x63 + talk_choice * 0x21]);
}

void date_gift_speak(void) {
    talk_tables = &date_gift_group;
    talk_active_line_ptr = &date_gift_line;
    talk_active_text_table = date_gift_text;
    talk_active_command_table = date_gift_command;
    talk_active_voice_table = date_gift_voice;
    talk_run(talk_choice, 1, 0);
}

void date_gift_speak_wait(void) {
    talk_tables = (u8 *)&date_gift_group;
    talk_active_line_ptr = (u8 *)&date_gift_line;
    talk_active_text_table = date_gift_text;
    talk_active_command_table = date_gift_command;
    talk_active_voice_table = date_gift_voice;
    if (talk_run(talk_choice, 1, 0) == 1) {
        mode_step_next();
    }
}

void date_gift_speak_unshuffled(void) {
    talk_tables = &date_gift_group;
    talk_active_line_ptr = &date_gift_line;
    talk_active_text_table = date_gift_text;
    talk_active_command_table = date_gift_command;
    talk_active_voice_table = date_gift_voice;
    talk_run(0xFF, 1, 0);
}

void date_gift_choice_sections_set(void) {
    date_gift_saved_group = date_gift_group;
    date_gift_saved_text = date_gift_text;
    date_gift_saved_command = date_gift_command;
    date_gift_saved_voice = date_gift_voice;
    date_gift_group = game_state.current_girl;
    /* (s16) leaves the ugen temporary between sll and subu unused, as in the original; the product sits before the last three copies so their constant loads take t5/t6/t7. T-17205 */
    date_gift_group = (s16)(date_gift_group * 3) + date_school_year;
    date_gift_text = D_8015CFFC;
    date_gift_command = D_8015D030;
    date_gift_voice = D_8015D064;
    mode_step_next();
}

void date_gift_apply_reward(void) {
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
