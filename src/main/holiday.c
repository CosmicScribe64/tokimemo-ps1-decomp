/* holiday.c: src/main/80075320.c; T-27059: original object [0x80075320,0x800789E0); mode 0x46 holiday menu, activities, phone, magazine, appointment and club flows; reuse reviewed m-holiday name. Original object 0x80075320-0x800789E0 (config/objects/main.txt). */
#define MAIN_API_OVERRIDE_girl_scores /* this object calls it as void (no v0 result in the call block, holiday_shared_birthday_scene_due); main_api.h has s32 */
#include "common.h"
#include "game.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
u8 holiday_dial_keys[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0 };
u8 holiday_dial_key_pos = 0;
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_activity_bg_sectors);
u8 appointment_choice_suffix[0x14] = { "で、待ち合わせ』" };
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_sd_set_summer);
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_sd_set_other);
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_sd_set_sectors);
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_magazine_hint_biorhythm_legend);
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_magazine_hint_biorhythm_crossing);
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_magazine_hint_biorhythm_distance);
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_magazine_hint_phone);
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_magazine_hint_stereo_radio);
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_magazine_hint_mascot_unlock);
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_magazine_hint_mascot_controls);
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_magazine_hint_manual);
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_magazine_hint_second_pad_shortcut);
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_magazine_hint_cursor_shape);
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_magazine_hint_menu_mode);
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_magazine_hint_sd_speed);
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_magazine_hint_after_clear);
INCLUDE_RODATA("asm/data/main/holiday.data", D_800B6CE0);
INCLUDE_RODATA("asm/data/main/holiday.data", holiday_magazine_hints);
/* pointer-only label holiday_magazine_hint_biorhythm_legend may be inside holiday_sd_set_sectors; replace its include too. */
/* pointer-only label holiday_magazine_hint_biorhythm_crossing may be inside holiday_sd_set_sectors; replace its include too. */
/* pointer-only label holiday_magazine_hint_biorhythm_distance may be inside holiday_sd_set_sectors; replace its include too. */
/* pointer-only label holiday_magazine_hint_phone may be inside holiday_sd_set_sectors; replace its include too. */
/* pointer-only label holiday_magazine_hint_stereo_radio may be inside holiday_sd_set_sectors; replace its include too. */
/* pointer-only label holiday_magazine_hint_mascot_unlock may be inside holiday_sd_set_sectors; replace its include too. */
/* pointer-only label holiday_magazine_hint_mascot_controls may be inside holiday_sd_set_sectors; replace its include too. */
/* pointer-only label holiday_magazine_hint_manual may be inside holiday_sd_set_sectors; replace its include too. */
/* pointer-only label holiday_magazine_hint_second_pad_shortcut may be inside holiday_sd_set_sectors; replace its include too. */
/* pointer-only label holiday_magazine_hint_cursor_shape may be inside holiday_sd_set_sectors; replace its include too. */
/* pointer-only label holiday_magazine_hint_menu_mode may be inside holiday_sd_set_sectors; replace its include too. */
/* pointer-only label holiday_magazine_hint_sd_speed may be inside holiday_sd_set_sectors; replace its include too. */
/* pointer-only label holiday_magazine_hint_after_clear may be inside holiday_sd_set_sectors; replace its include too. */
/* pointer-only label D_800B6CE0 may be inside holiday_sd_set_sectors; replace its include too. */

void girl_scores();

void holiday_mode_handler(void) {
    /* FAKE: byte view of the u8 field avoids the redundant selector copy (move v0,v1), as in TEL
     * (T-10630). T-17000 */
    switch (*(u8 *)&game_state.mode_stage) {
    case 0x0:
        holiday_mode_stage_open();
        break;
    case 0x1:
        holiday_mode_stage_menu();
        break;
    case 0x5:
        holiday_mode_stage_activity();
        break;
    case 0x6:
        holiday_club();
        break;
    case 0x7:
        holiday_mode_stage_club_join();
        break;
    case 0x10:
        holiday_tel();
        break;
    case 0x20:
        holiday_mode_stage_magazine();
        break;
    case 0x30:
        settings_command_screen_mode_stage_calendar();
        break;
    case 0x40:
        settings_mode_stage_dispatch();
        break;
    case 0xFE:
        holiday_mode_stage_appointment();
        break;
    }
    if (((Actor *)actor_pool)[27].draw_flags & 0x80) {
        hud_mascot_update();
    }
    message_window_show();
    dec_bg_draw(0);
    hizuke_show();
    parameter_show();
    hud_cursor_update();
    hud_screen_frame_draw(1);
}

void holiday_mode_stage_open(void) {
    switch (game_state.mode_step) {
    case 0:
    default:
        holiday_open_init();
        break;
    case 1:
        holiday_open_wait_bg();
        break;
    }
    message_window_title_update();
    hud_icon_bar_update(0);
}

/* FAKE: T-24108, dead entry read of unk_03F satisfies cvt_pass's entry condition and keeps m's stack slot (placing rank/points at sp+0x4C/0x2C) so unk_03F is promoted to $v0 across the month tests and copied to $a1 in the hud_appointment_find_today delay slot. */
s32 holiday_shared_birthday_scene_due(void) {
    s32 m;
    u8 rank[16];
    s16 points[16];

    m = game_state.month;
    girl_scores(rank, points);
    if (((game_state.player_birthday & 0xF) != (game_state.girls[0].birthday.w & 0xF)) ||
        (((u32)(game_state.player_birthday << 0x17) >> 0x1B) != ((u32)(game_state.girls[0].birthday.w << 0x17) >> 0x1B))) {
        return 0;
    }
    if ((game_state.month != 3) && ((game_state.month != 4) || ((u8)game_state.day >= 4U))) {
        return 0;
    }
    if (hud_appointment_find_today(game_state.year, game_state.month, game_state.day) > 0) {
        return 0;
    }
    if (birth_day_check(0xF) != 1) {
        return 0;
    }
    if (get_g_zyotai_s(GIRL_SHIORI) & 0x80) {
        return 1;
    }
    return 0;
}

void holiday_open_init(void) {
    s32 i;
    s32 r;
    s32 notice_line_count;
    u8 holiday_message[0x104];
    u8 notice_name_buffer[0x20];

    /* FAKE: T-20940, the clear on one source line (as1 schedules a line as a unit) gives the original's order
     * of the unrolled stores. */
    holiday_dial_key_pos = 0;
    for (i = 0; i < 7; i++) { holiday_dial_keys[i] = 0xFF; }
    hizuke_init();
    if (holiday_shared_birthday_scene_due() != 0) {
        D_80122CD4 = 0;
        talk_work = 8;
        parameter_show_init();
        parameter_show_init();
        game_mode_set(GAME_MODE_SPECIAL_EVENT);
        return;
    }
    sprite_tables_clear();
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    message_window_set_title_hidden("休日コマンド画面");
    k_disp_start(1);
    game_state.icon_count = 0xE;
    hud_icon_bar_init();
    hud_icon_layout_apply((s32) (u8) game_state.icon_count);
    holiday_icons_set_available();
    gnsx(game_state.player_family_name);
    if (game_state.player_flags_upper.h & 1) {
        game_state.icon_count = 0xE;
        hud_icon_bar_init();
        hud_icon_layout_apply((s32) (u8) game_state.icon_count);
        settings_command_screen_icons_reset(1);
        icon_can_use_set(0xD, 0);
        icon_can_use_set(0, 1);
        icon_can_use_set(1, 1);
        icon_can_use_set(2, 1);
        sndisp("一週間強制補習を しなければならない）", 0, 0x1F);
    } else if ((game_state.player_flags.b[1] & 0xF) == 3) {
        game_state.icon_count = 0xE;
        hud_icon_bar_init();
        hud_icon_layout_apply((s32) (u8) game_state.icon_count);
        settings_command_screen_icons_reset(1);
        icon_can_use_set(0xD, 0);
        icon_can_use_set(7, 1);
        game_state.menu_cursor_0 = 7;
        sndisp("病気で身体が動かない）", 0, 0x1F);
    } else {
        notice_line_count = 0;
        if (hud_appointment_find_today((s32) game_state.year, (s32) game_state.month, (s32) game_state.day) > 0) {
            strcpy(holiday_message, "今日は、");
            get_g_name(notice_name_buffer, (s32) ((u32) (game_state.unk_54C[0].w * 8) >> 0x1C));
            strcat(holiday_message, notice_name_buffer);
            strcat(holiday_message, "とデートだ）");
            sndisp(holiday_message, 0, 0x1F);
            game_state.menu_cursor_0 = 9;
            notice_line_count = 1;
        }
        if ((girl_player_sports_club_check() == 1) && (((u32) (game_state.day + 6) / 7U) == 3) && (game_state.weekday == WEEKDAY_SUNDAY)) {
            if (((u8) game_state.month % 3U) == 0) {
                if (notice_line_count != 0) {
                    strcpy(holiday_message, "（");
                    strcat(holiday_message, "今日は、部活の練習試合だ）");
                    set_kanji_string(-0x56, 0x42, 0U, holiday_message, 0);
                } else {
                    strcpy(holiday_message, "今日は、部活の練習試合だ）");
                    sndisp(holiday_message, notice_line_count, 0x1F);
                }
                notice_line_count += 1;
            } else {
                if (notice_line_count != 0) {
                    strcpy(holiday_message, "（");
                    strcat(holiday_message, "今日は、部活の練習日だ）");
                    set_kanji_string(-0x56, 0x42, 0U, holiday_message, 0);
                } else {
                    strcpy(holiday_message, "今日は、部活の練習日だ）");
                    sndisp(holiday_message, notice_line_count, 0x1F);
                }
                notice_line_count += 1;
            }
            game_state.menu_cursor_0 = 4;
        }
        if (notice_line_count == 0) {
            sndisp("今日は何をしようかな？）", 0, 0x1F);
        }
    }
    k_disp_start(5);
    actor_clear_upper();
    hud_cursor_actor_init(0);
    parameter_show_init();
    parameter_show_init();
    message_window_init();
    r = dec_bg_cd_read(D_800B5960[hud_season_index()], 0);
    if (r != -1) {
        if ((r == 0) || (r == 1)) {
            set_dec_bri(0x80U);
            dec_bg_show_set(0, r);
            mode_stage_next();
        }
    } else {
        mode_step_next();
    }
}

void holiday_open_wait_bg(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(0, dec_bg_decode_to_vram(0));
        if (bg_decode_result == -1) {
            dec_bg_reset();
            dec_bg_cd_read(D_800B5960[hud_season_index()], 0);
        } else {
            set_dec_bri(0x80);
            mode_stage_next();
        }
    }
}

void holiday_icons_set_available(void) {
    s32 i;
    s32 quit_club_count;

    if (hud_appointment_find_today(game_state.year, game_state.month, game_state.day) <= 0) {
        icon_can_use_set(9, 0);
    }
    icon_can_use_set(0xD, 0);
    /* the original reads the s8 field unsigned here (lbu), as 8005A0B0.c does */
    if (*(u8 *)&game_state.icon_mode == 2) {
        icon_can_use_set(0xE, 0);
    }
    if (game_state.weekday == WEEKDAY_SUNDAY) {
        quit_club_count = 0;
        for (i = 0; i < 11; i++) {
            if (game_state.clubs[i].flags & 1) {
                quit_club_count++;
            }
        }
        if (quit_club_count >= 11) {
            icon_can_use_set(4, 0);
        }
    } else if ((game_state.player_flags.h >> 12) == 11) {
        icon_can_use_set(4, 0);
    }
}

void holiday_mode_stage_menu(void) {
    if (game_state.mode_step == 0) {
        holiday_menu_frame();
    }
    message_window_title_update();
    hud_icon_bar_update(0);
}

void holiday_menu_frame(void) {
    k_disp_inc();
    hud_cursor_update();
    if (holiday_menu_click() == 0 && holiday_phone_dial_shortcut() == 0 && settings_room_click_radio_holiday() == 0) {
        settings_room_click_mascot_holiday();
    }
}

s32 holiday_phone_dial_shortcut(void) {
    s32 i;
    s32 j;
    u32 dial_ring_read_index;
    s32 dial_random_value;

    if ((((Actor *)actor_pool)->x < -0x40) && (((Actor *)actor_pool)->x >= -0x4F)
        && (((Actor *)actor_pool)->y < 0x18) && (((Actor *)actor_pool)->y >= -0xF)) {
        /* FAKE T-25050: the raw input use keeps the word and mask in
         * separate promoted registers. */
        if (game_state.pad_pressed) {}
        switch (game_state.pad_pressed & 0x70) {
        case 0x20:
            holiday_dial_keys[holiday_dial_key_pos] = 0;
            holiday_dial_key_pos++;
            audio_command(0x505);
            break;
        case 0x40:
            holiday_dial_keys[holiday_dial_key_pos] = 1;
            holiday_dial_key_pos++;
            audio_command(0x506);
            break;
        case 0x10:
            holiday_dial_keys[holiday_dial_key_pos] = 2;
            holiday_dial_key_pos++;
            audio_command(0x507);
            break;
        }
        if (holiday_dial_key_pos >= 7) {
            holiday_dial_key_pos = 0;
        }
        if (game_state.pad_pressed & 0x70) {
            for (i = 0; i < 13; i++) {
                dial_ring_read_index = holiday_dial_key_pos + 7;
                for (j = 0; j < 7; dial_ring_read_index++, j++) {
                    if (holiday_dial_keys[dial_ring_read_index % 7] != girl_phone_digits[i][j]) {
                        break;
                    }
                    if ((j == 6) && ((u32)(game_state.girls[i].flags.w << 28) >> 31)) {
                        if (i != 11) {
                            if (i == 12) {
                                D_801230D0 = 11;
                                game_state.current_girl = GIRL_MIHARU;
                                game_state.menu_cursor_0 = 12;
                            } else {
                                D_801230D0 = i;
                                game_state.current_girl = i;
                                game_state.menu_cursor_0 = i + 1;
                            }
                        }
                        dial_random_value = rand();
                        /* The original adds to the full first record word. */
                        *(s32 *)&game_state.girls[i].tokimeki_fraction += dial_random_value & 0x7FFF;
                        if (game_state.player_flags_upper.h & 1) {
                            game_state.param_stress.value += 0x20;
                        }
                        mode_stage_set(0x10);
                        mode_step_set(2);
                        game_state.substep = 11;
                        text_system_reset();
                        icon_disp_switch(0);
                        message_window_title_show(0);
                        telephone_class_init();
                        k_sub_reset_point_set();
                        k_sub_disp_start(0);
                        return 1;
                    }
                }
                /* FAKE T-25050: this impossible unsigned branch keeps the
                 * position load inside the outer loop during optimization.
                 * The branch and store disappear from the emitted code. */
                if ((u32)i < 0) {
                    holiday_dial_key_pos = 0;
                }
            }
        }
    }
    return 0;
}

s32 holiday_activity_bg_sector(void) {
    u8 queue_index;
    u16 player_flags_word;

    queue_index = game_state.day_event_index;
    if (game_state.day_events[queue_index].girl != 4) {
        return holiday_activity_bg_sectors[game_state.day_events[queue_index].girl];
    }
    player_flags_word = game_state.player_flags.h;
    return D_800B6780[player_flags_word >> 0xC];
}

s32 holiday_menu_click(void) {
    s32 pad[4]; /* FAKE: preserves the 0x40 frame and confirm_hit at sp+0x2C. T-24501 */
    s32 confirm_hit;
    s32 sunday_club_member;
    s32 weekday_club_member;

    /* FAKE: unsigned read of the signed state byte reproduces the original lbu. */
    if (((u8)game_state.icon_mode == ICON_MODE_FREE) && (game_state.pad_held & PAD_L2)) {
        return 1;
    }
    /* FAKE: explicit signed-byte views preserve the selector range across packed stores. */
    confirm_hit = 0;
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if (*(s8 *)&game_state.menu_cursor_0 >= 0) {
            confirm_hit = 1;
            cd_audio_command(9, 0, 0);
            audio_command(3);
            if (audio_bgm_is_playing() == 0) {
                audio_bank_start(0, 0);
            }
        }
        switch (*(s8 *)&game_state.menu_cursor_0) {
        case 4:
            if (get_icon_can_use((s32) *(s8 *)&game_state.menu_cursor_0) != 0) {
                audio_command(0x500);
                if (game_state.weekday == WEEKDAY_SUNDAY) {
                    if (((u16) game_state.player_flags.h >> 0xC) == 0xB) {
                        ((LastCommandBits *)&game_state.last_command)->icon = *(s8 *)&game_state.menu_cursor_0;
                        game_state.day_events[game_state.day_event_index].girl = *(s8 *)&game_state.menu_cursor_0;
                        game_state.day_events[game_state.day_event_index].scene = hud_command_roll_poor_result(1);
                        mode_stage_set(7);
                    } else {
                        game_state.day_events[game_state.day_event_index].girl = *(s8 *)&game_state.menu_cursor_0;
                        game_state.day_events[game_state.day_event_index].scene = hud_command_roll_poor_result(1);
                        ((LastCommandBits *)&game_state.last_command)->icon = *(s8 *)&game_state.menu_cursor_0;
                        sunday_club_member = girl_find_by_club((s32) ((u16) game_state.player_flags.h >> 0xC));
                        /* Shift back extracts flag bit 2 and preserves ugen temporary numbering. T-24501 */
                        if ((sunday_club_member != -1) && (((s32)(game_state.girls[sunday_club_member].flags.u << 29) >> 31) != 0)) {
                            game_state.girls[sunday_club_member].affection += 1;
                        }
                        mode_stage_set(6);
                    }
                } else {
                    weekday_club_member = girl_find_by_club((s32) ((u16) game_state.player_flags.h >> 0xC));
                    /* Keep the shift-back extraction here for the same temporary sequence. T-24501 */
                    if ((weekday_club_member != -1) && (((s32)(game_state.girls[weekday_club_member].flags.u << 29) >> 31) != 0)) {
                        game_state.girls[weekday_club_member].affection += 1;
                    }
                    game_state.day_events[game_state.day_event_index].girl = *(s8 *)&game_state.menu_cursor_0;
                    game_state.day_events[game_state.day_event_index].scene = hud_command_roll_poor_result(1);
                    ((LastCommandBits *)&game_state.last_command)->icon = *(s8 *)&game_state.menu_cursor_0;
                    mode_stage_set(5);
                }
            }
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 7:
            if (get_icon_can_use((s32) *(s8 *)&game_state.menu_cursor_0) != 0) {
                ((LastCommandBits *)&game_state.last_command)->icon = *(s8 *)&game_state.menu_cursor_0;
                audio_command(0x500);
                game_state.day_events[game_state.day_event_index].girl = *(s8 *)&game_state.menu_cursor_0;
                game_state.day_events[game_state.day_event_index].scene = hud_command_roll_poor_result(1);
                mode_stage_set(5);
            }
            break;
        case 9:
            if ((get_icon_can_use((s32) *(s8 *)&game_state.menu_cursor_0) != 0) && (hud_appointment_find_today((s32) game_state.year, (s32) game_state.month, (s32) game_state.day) > 0)) {
                ((LastCommandBits *)&game_state.last_command)->icon = *(s8 *)&game_state.menu_cursor_0;
                audio_command(0x500);
                mode_stage_set(0xFE);
            }
            break;
        case 8:
            if (get_icon_can_use((s32) *(s8 *)&game_state.menu_cursor_0) != 0) {
                ((LastCommandBits *)&game_state.last_command)->icon = *(s8 *)&game_state.menu_cursor_0;
                audio_command(0x50D);
                mode_stage_set(0x10);
            }
            break;
        case 10:
            if (get_icon_can_use((s32) *(s8 *)&game_state.menu_cursor_0) != 0) {
                audio_command(0x50D);
                mode_stage_set(0x20);
            }
            break;
        case 12:
            audio_command(0x50D);
            mode_stage_set(0x30);
            break;
        case 11:
            audio_command(0x50D);
            mode_stage_set(0x40);
            break;
        }
    }
    return confirm_hit;
}


s32 appointment_place_candidates(u32 *place_choices_out) {
    /* FAKE: T-10910, T-24135, padA places place_counts at sp+0x2C within the 0xB0 frame, and if (1) around place_counts[...]++ reproduces the original branch/delay-slot layout. */
    s32 padA[1];
    s32 place_counts[32];
    s32 i;
    s32 distinct_place_count;
    s32 copied_choice_count;
    s32 place_id;

    switch (game_state.unk_54C[0].w & 0x1F) {
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
        return 1;
    default:
        for (i = 0; i < 32; i++) {
            place_counts[i] = 0;
        }
        for (i = 0; i < 8; i++) {
            if (!((AppointmentBits *)&game_state.unk_54C[i])->active) {
                break;
            }
            if (1) {
                place_counts[((AppointmentBits *)&game_state.unk_54C[i])->place]++;
            }
        }
        place_counts[16] = 0;
        place_counts[17] = 0;
        place_counts[19] = 0;
        place_counts[20] = 0;
        place_counts[18] = 0;
        distinct_place_count = 0;
        copied_choice_count = 0;
        for (i = 0; i < 32; i++) {
            if (place_counts[i] != 0) {
                distinct_place_count++;
            }
        }
        if (distinct_place_count < 2) {
            *place_choices_out = (u32)(game_state.unk_54C[0].w << 2) >> 31;
            return 1;
        }
        for (i = 0; i < 8; i++) {
            if (!((AppointmentBits *)&game_state.unk_54C[i])->active) {
                break;
            }
            if (place_counts[place_id = ((AppointmentBits *)&game_state.unk_54C[i])->place] != 0) {
                place_choices_out[copied_choice_count] = place_id;
                copied_choice_count++;
                place_counts[((AppointmentBits *)&game_state.unk_54C[i])->place] = 0;
                if (copied_choice_count >= 3) {
                    break;
                }
            }
        }
        return distinct_place_count;
    }
}

void appointment_start(void) {
    /* FAKE: T-24135, 4-byte stack slot above place_choices_scratch[32] places place_choices_scratch at sp+0x2C as in the original. */
    s32 pad[1];
    u32 place_choices_scratch[32];

    game_state.appointment_place_0 = 0xFF;
    game_state.appointment_place_1 = 0xFF;
    game_state.appointment_place_2 = 0xFF;
    game_state.appointment_place_3 = 0xFF;
    hud_appointment_prompt_noop();
    if (appointment_place_candidates(place_choices_scratch) >= 2) {
        k_reset(1);
        k_sub_reset_point_set();
        gnsx(game_state.player_family_name);
        sndisp((u8 *)"どこで、 待ち合わせだったかな？）", 0, 0x1F);
        k_sub_disp_start(1);
        mode_step_next();
        return;
    } else {
        game_state.appointment_place_0 = game_state.unk_54C_0.unk_00;
        game_state.appointment_place_1 = game_state.unk_54C_0.unk_00;
        game_state.appointment_place_2 = 0xFF;
        game_state.appointment_place_3 = 0xFF;
        game_state.day_events[game_state.day_event_index].type = DAY_EVENT_APPOINTMENT;
        game_state.day_events[game_state.day_event_index].girl = ((AppointmentBits *)&game_state.unk_54C[0])->girl;
        game_state.day_events[game_state.day_event_index].scene = ((AppointmentBits *)&game_state.unk_54C[0])->place;
        game_state.current_girl = game_state.unk_54C_0.unk_25;
        game_state.date_girl = game_state.unk_54C_0.unk_25;
        game_state.appointment_flag = ((AppointmentBits *)&game_state.unk_54C[0])->flag;
    }
    mode_step_set(2);
}

/* T-24501: 932-byte literal match. The split stack padding preserves the
 * original 0x198 frame: selected IDs at 0x188/0x18C/0x190, candidate IDs at
 * 0x108, and string buffers at 0x3C/0x7C/0xBC. */
void appointment_place_menu_build(void) {
    s32 top_gap; /* FAKE: reserve the original top stack word. T-24501. */
    s32 third_place;
    s32 second_place;
    s32 first_place;
    s32 place_candidates[32];
    s32 list_gap; /* FAKE: reserve the original word between IDs and buffers. T-24501. */
    s32 temp_hi; /* FAKE: unused original draft scalar retained for the diagnosed stack layout. T-24501. */
    s32 first_choice_index;
    u8 first_place_text[64];
    u8 second_place_text[64];
    u8 third_place_text[64];
    s32 i;
    for (i = 0; i < 64; i++) {
        first_place_text[i] = second_place_text[i] = third_place_text[i] = 0;
    }
    if (appointment_place_candidates(place_candidates) >= 3) {
        first_choice_index = rand() % 3;
        if (first_choice_index < 0) {
            first_choice_index = 0;
        }
        first_place = place_candidates[first_choice_index % 3];
        second_place = place_candidates[(s32) (first_choice_index + 1) % 3];
        third_place = place_candidates[(s32) (first_choice_index + 2) % 3];
        game_state.appointment_place_0 = 2;
        x_taku_menu_set(1, 3);
        strcpy(first_place_text, "『");
        strcat(first_place_text, place_name_table[first_place]);
        strcat(first_place_text, &appointment_choice_suffix);
        strcpy(second_place_text, "『");
        strcat(second_place_text, place_name_table[second_place]);
        strcat(second_place_text, &appointment_choice_suffix);
        strcpy(third_place_text, "『");
        strcat(third_place_text, place_name_table[third_place]);
        strcat(third_place_text, &appointment_choice_suffix);
        x_taku_string_set((s32) first_place_text, (s32) second_place_text, (s32) third_place_text, 3);
    } else {
        first_choice_index = rand() & 1;
        if (first_choice_index < 0) {
            first_choice_index = 0;
        }
        first_place = place_candidates[first_choice_index % 2];
        second_place = place_candidates[(s32) (first_choice_index + 1) % 2];
        third_place = 0xFF;
        game_state.appointment_place_0 = 1;
        x_taku_menu_set(1, 2);
        strcpy(first_place_text, "『");
        strcat(first_place_text, place_name_table[first_place]);
        strcat(first_place_text, &appointment_choice_suffix);
        strcpy(second_place_text, "『");
        strcat(second_place_text, place_name_table[second_place]);
        strcat(second_place_text, &appointment_choice_suffix);
        x_taku_string_set((s32) first_place_text, (s32) second_place_text, 0, 2);
    }
    game_state.appointment_place_1 = (u8) first_place;
    game_state.appointment_place_2 = (u8) second_place;
    game_state.appointment_place_3 = (u8) third_place;
}

s32 appointment_place_choose(void) {
    GameState *g;
    g = &game_state;
    switch (g->substep) {                           /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        xa_wait();
        break;
    case 1:                                         /* switch 1 */
        k_sub_reset();
        appointment_place_menu_build();
        g->substep += 1;
        k_sub_disp_start(3);
        break;
    case 2:                                         /* switch 1 */
        menu_check(1, (s32) cursor_x, (s32) cursor_y);
        menu_cursor_update(1);
        menu_bar_show(1);
        if (g->pad_pressed & PAD_CIRCLE) {
            switch (g->menu_cursor_1) {                   /* switch 2; irregular */
            case 0:                                 /* switch 2 */
            case 1:                                 /* switch 2 */
                /* FAKE: index the two adjacent scalar choice bytes as a table. T-9910. */
                g->appointment_place_0 = *(&g->appointment_place_1 + g->menu_cursor_1);
                mode_step_next();
                break;
            case 2:                                 /* switch 2 */
                if (g->appointment_place_0 == 2) {
                    g->appointment_place_0 = g->appointment_place_3;
                    mode_step_next();
                }
                break;
            }
        }
        break;
    }
}

/* Implicit int in the original (no return value): the switch selector then stays live in $v0 (or v0,v1,zero in the
 * delay slot of the first beqz). T-17006 */
s32 appointment_place_confirm(void) {
    s32 pad; /* FAKE: T-17006, unused local that reproduces the original stack layout (a 4-byte slot above appointment_message; the
              * buffers are otherwise 4 bytes too high); its real use is unknown. */
    u8 appointment_message[0x100];
    u8 appointment_girl_name[0x20];

    switch (game_state.substep) {
    case 0:
        k_reset(1);
        k_sub_reset_point_set();
        k_sub_reset();
        gnsx(game_state.player_family_name);
        strcpy(appointment_message, "それじゃ、 ");
        strcat(appointment_message, place_name_table[game_state.appointment_place_0]);
        strcat(appointment_message, "で、 ");
        get_g_name(appointment_girl_name, (u32)(game_state.unk_54C[0].w * 8) >> 0x1C);
        strcat(appointment_message, appointment_girl_name);
        strcat(appointment_message, "を待つか。）");
        sndisp(appointment_message, 0, 0x1F);
        k_sub_disp_start(4);
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        mode_step_next();
        return;
    }
}

void appointment_launch(void) {
    GameState *g;
    s32 i;
    s32 j;
    s32 offered_places[3];

    g = &game_state;
    offered_places[0] = game_state.appointment_place_1;
    offered_places[1] = game_state.appointment_place_2;
    offered_places[2] = game_state.appointment_place_3;
    game_state.appointment_place_1 = game_state.unk_54C_0.unk_00;
    for (i = 0, j = 2; i < 3; i++) {
        if (offered_places[i] != (g->unk_54C[0].w & 0x1F)) {
            (&game_state.appointment_place_0)[j] = offered_places[i];
            j++;
        }
    }
    game_state.day_events[game_state.day_event_index].type = DAY_EVENT_APPOINTMENT;
    game_state.day_events[game_state.day_event_index].girl = game_state.unk_54C_0.unk_25;
    game_state.day_events[game_state.day_event_index].scene = game_state.unk_54C_0.unk_00;
    game_state.current_girl = game_state.unk_54C_0.unk_25;
    game_state.date_girl = game_state.unk_54C_0.unk_25;
    game_state.appointment_flag = ((AppointmentBits *)&game_state.unk_54C[0])->flag;
    game_mode_set(GAME_MODE_DATE);
}

void holiday_mode_stage_appointment(void) {
    switch (game_state.mode_step) {
    case 0:
        appointment_start();
        break;
    case 1:
        appointment_place_choose();
        break;
    case 2:
        appointment_place_confirm();
        break;
    case 3:
        appointment_launch();
        break;
    }
    message_window_title_update();
}

void holiday_mode_stage_activity(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        holiday_activity_load();
        break;
    case 1:
        holiday_activity_sd_setup();
        break;
    case 2:
        holiday_activity_play();
        break;
    }
    parameter_show();
    message_window_title_update();
    hud_icon_bar_update(1);
}

void holiday_activity_pick_sd_set(void) {
    s32 holiday_command;
    s32 club_partner_column;

    D_801230D0 = 0xFF;
    if (hud_in_summer_months() == 1) {
        holiday_command = game_state.day_events[game_state.day_event_index].girl;
        if (holiday_command == 4) {
            D_801230D0 = 0xFF;
        } else {
            D_801230D0 = holiday_sd_set_summer[holiday_command];
        }
    } else {
        holiday_command = game_state.day_events[game_state.day_event_index].girl;
        if (holiday_command == 4) {
            D_801230D0 = 0xFF;
        } else {
            D_801230D0 = holiday_sd_set_other[holiday_command];
        }
    }
    if (D_801230D0 == 0xFF) {
        club_partner_column = schedule_command_girl_select(4);
        if (club_partner_column != 0) {
            club_partner_column = 1;
        }
        if (hud_in_summer_months() == 1) {
            switch (game_state.player_flags.h >> 12) {
            case 0:
                D_801230D0 = D_800B6890[0][club_partner_column];
                break;
            case 1:
                D_801230D0 = D_800B6890[1][club_partner_column];
                break;
            case 2:
                D_801230D0 = D_800B6890[2][club_partner_column];
                break;
            case 3:
                D_801230D0 = D_800B6890[3][club_partner_column];
                break;
            case 4:
                D_801230D0 = D_800B6890[4][club_partner_column];
                break;
            case 5:
                D_801230D0 = D_800B6890[5][club_partner_column];
                break;
            case 6:
                D_801230D0 = D_800B6890[6][club_partner_column];
                break;
            case 7:
                D_801230D0 = D_800B6890[7][club_partner_column];
                break;
            case 8:
                D_801230D0 = D_800B6890[8][club_partner_column];
                break;
            case 9:
                D_801230D0 = D_800B6890[9][club_partner_column];
                break;
            case 10:
                D_801230D0 = D_800B6890[10][club_partner_column];
                break;
            }
        } else {
            switch (game_state.player_flags.h >> 12) {
            case 0:
                D_801230D0 = D_800B68A8[0][club_partner_column];
                break;
            case 1:
                D_801230D0 = D_800B68A8[1][club_partner_column];
                break;
            case 2:
                D_801230D0 = D_800B68A8[2][club_partner_column];
                break;
            case 3:
                D_801230D0 = D_800B68A8[3][club_partner_column];
                break;
            case 4:
                D_801230D0 = D_800B68A8[4][club_partner_column];
                break;
            case 5:
                D_801230D0 = D_800B68A8[5][club_partner_column];
                break;
            case 6:
                D_801230D0 = D_800B68A8[6][club_partner_column];
                break;
            case 7:
                D_801230D0 = D_800B68A8[7][club_partner_column];
                break;
            case 8:
                D_801230D0 = D_800B68A8[8][club_partner_column];
                break;
            case 9:
                D_801230D0 = D_800B68A8[9][club_partner_column];
                break;
            case 10:
                D_801230D0 = D_800B68A8[10][club_partner_column];
                break;
            }
        }
    }
}

s32 holiday_activity_bg_read(void) {
    s32 r;

    if ((game_state.player_flags.b[1] & 0xF) == 3) {
        r = dec_bg_cd_read(D_800B5960[hud_season_index()], 0);
    } else {
        r = dec_bg_cd_read(holiday_activity_bg_sector(), 0);
    }
    return r;
}

void holiday_activity_load(void) {
    GameState *gs = &game_state;
    s32 i;
    s32 r;

    text_system_reset();
    message_window_title_show(0);
    if (gs->substep == 0) {
        for (i = 0; i < 14; i++) {
            if (i != gs->day_events[gs->day_event_index].girl) {
                icon_can_use_set(i, 0);
            } else {
                icon_can_use_set(i, 1);
            }
        }
        ((NibbleView *)&gs->last_command.b[0])->high = gs->day_events[gs->day_event_index].girl;
        gs->progress_state[0x15 + (gs->last_command.b[0] >> 4) % 9]++;
        if ((gs->last_command.b[0] >> 4) == 4) {
            if (((PlayerFlagBits *)&gs->player_flags)->club < CLUB_NONE) {
                gs->clubs[((PlayerFlagBits *)&gs->player_flags)->club].practice_points += 6;
            }
        }
        r = holiday_activity_bg_read();
        if (r != -1) {
            if ((r == 0) || (r == 1)) {
                dec_bg_show_set(0, r);
                gs->substep = 2;
            }
        } else {
            gs->substep = 1;
        }
    } else if (gs->substep == 1) {
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(0, dec_bg_decode_to_vram(0));
            if (bg_decode_result == -1) {
                dec_bg_reset();
                holiday_activity_bg_read();
            } else {
                gs->substep = 2;
            }
        }
    } else if (gs->substep == 2) {
        holiday_activity_pick_sd_set();
        if (((gs->player_flags.b[1] & 0xF) == 3) && !(gs->player_flags_upper.h & 1)) {
            cd_read_sectors(9, 0x801E0000, 0x7D8A);
            gs->substep = 3;
        } else if (gs->day_events[gs->day_event_index].girl != 4) {
            cd_read_sectors(0x29, 0x801E0000, holiday_sd_set_sectors[D_801230D0]);
            gs->substep = 3;
        } else {
            cd_read_sectors(0x29, 0x801E0000, D_800B6810[D_801230D0]);
            gs->substep = 3;
        }
    } else if (gs->substep == 3) {
        if (cd_read_sync_status() & 1) {
            mode_step_next();
        }
    }
}

void holiday_activity_sd_setup(void) {
    /* FAKE: T-20940, unused local declared first, as the sibling school_day_sd_actors_init declares its record pointer; IDO gives
     * every local a slot, so holiday_activity_result_flag spills at sp+0x30 as in the original (sp+0x34 without it). */
    DayEvent *rec;
    s32 holiday_activity_result_flag;

    actor_clear(0x60);
    holiday_activity_result_flag = game_state.day_events[game_state.day_event_index].scene;
    if (((game_state.player_flags.b[1] & 0xF) == 3) && !(game_state.player_flags_upper.h & 1)) {
        addr_init_else_sd(2);
        holiday_activity_result_flag = 0;
    } else if (game_state.day_events[game_state.day_event_index].girl == 4) {
        addr_init_club_sd(D_801230D0);
        if (schedule_command_girl_select(4) != 0xF) {
            ((Actor *)actor_pool)[97].unk_01 = 1;
            ((Actor *)actor_pool)[97].flags = 5;
            ((Actor *)actor_pool)[97].quad_flags = 0x01000000;
            ((Actor *)actor_pool)[97].draw_flags = 0x84;
            ((Actor *)actor_pool)[97].frame_table = (u8 *)D_80125CA8;
            ((Actor *)actor_pool)[97].sequence_table = (u8 *)D_80125CAC;
            ((Actor *)actor_pool)[97].tpage_table = (u8 *)D_80125CB0;
            ((Actor *)actor_pool)[97].unk_05 = 2;
            /* FAKE: u8 view of the s8 flag unk_F6E (both reads in this function; the original loads it with lbu).
             * Same view as in school_day_sd_actors_init (800737A0.c, T-17016). Matches; real source unknown. T-20940 */
            if (*(u8 *)&game_state.sd_anim_fast != 0) {
                ((Actor *)actor_pool)[97].blink_period = 0x82;
            }
            ((Actor *)actor_pool)[97].unk_43 = 0x10;
            ((Actor *)actor_pool)[97].sequence_slot = (s16)(holiday_activity_result_flag + 2);
            ((Actor *)actor_pool)[97].x = -0x18;
            ((Actor *)actor_pool)[97].y = -0x40;
        }
    } else {
        if ((D_801230D0 == 6) || (D_801230D0 == 7)) {
            game_state.day_events[game_state.day_event_index].scene = 0;
            holiday_activity_result_flag = 0;
        }
        addr_init_holiday_sd(D_801230D0);
    }
    load_palette(D_80125CA4, 0x10, 1, 1, 0);
    ((Actor *)actor_pool)[96].unk_01 = 1;
    ((Actor *)actor_pool)[96].flags = 5;
    ((Actor *)actor_pool)[96].quad_flags = 0x01000000;
    ((Actor *)actor_pool)[96].draw_flags = 0x84;
    ((Actor *)actor_pool)[96].frame_table = (u8 *)D_80125CA8;
    ((Actor *)actor_pool)[96].sequence_table = (u8 *)D_80125CAC;
    ((Actor *)actor_pool)[96].tpage_table = (u8 *)D_80125CB0;
    ((Actor *)actor_pool)[96].unk_05 = 2;
    if (*(u8 *)&game_state.sd_anim_fast != 0) {
        ((Actor *)actor_pool)[96].blink_period = 0x82;
    }
    ((Actor *)actor_pool)[96].unk_43 = 0x10;
    ((Actor *)actor_pool)[96].sequence_slot = holiday_activity_result_flag;
    ((Actor *)actor_pool)[96].x = -0x50;
    ((Actor *)actor_pool)[96].y = -0x40;
    mode_step_next();
}

/* The original passes club to game_mode_set as a second argument (the definition takes one; main_api.h keeps
 * the `()` declaration): club lives in a1 from its second read on. */
void holiday_activity_play(void) {
    GameState *gs = &game_state;
    u8 club_activity_result_flag;
    s32 club_member_id;
    u32 club_id;

    if (gs->mode_step_timer.w == 0x20) {
        switch (((Actor *)actor_pool)[96].sequence_slot & 1) {
        case 0:
            audio_command(0x501);
            break;
        case 1:
            audio_command(0x502);
            break;
        }
        club_activity_result_flag = gs->day_events[gs->day_event_index].scene;
        parameter_up_down(gs->day_events[gs->day_event_index].girl, 1, club_activity_result_flag);
        parameter_show_init();
        return;
    }
    if (gs->mode_step_timer.u <= 0x40) {
        return;
    }
    actor_clear(0x60);
    actor_clear(0x61);
    if ((gs->weekday == WEEKDAY_SUNDAY) && (gs->day_events[gs->day_event_index].girl == 4)
        && ((club_id = gs->player_flags.h >> 12) != 11) && (gs->clubs[club_id].practice_points == 6)) {
        club_member_id = girl_find_by_club(club_id);
        gs->date_girl = club_member_id;
        if ((club_member_id != -1) && (gs->day_count < 0x2DA)) {
            club_id = gs->player_flags.h >> 12;
            /* Club 10 tests the condition twice: under club_member_id == 0, then again for every club_member_id (the first copy falls
             * through into the second). Both copies are in the original. */
            if (club_id == 10) {
                if (club_member_id == 0) {
                    if (gs->girls[club_member_id].flags.f.b2 && gs->girls[club_member_id].flags.f.met
                        && !((ClubFlagBits *)&gs->clubs[club_id])->b29
                        && !gs->girls[club_member_id].flags.f.b18) {
                        gs->girls[club_member_id].flags.f.b18 = 1;
                        game_mode_set(GAME_MODE_CLUB_CAMP_2, club_id);
                        return;
                    }
                }
                if (gs->girls[club_member_id].flags.f.b2 && gs->girls[club_member_id].flags.f.met
                    && !((ClubFlagBits *)&gs->clubs[club_id])->b29
                    && !gs->girls[club_member_id].flags.f.b18) {
                    gs->girls[club_member_id].flags.f.b18 = 1;
                    game_mode_set(GAME_MODE_CLUB_CAMP_2, club_id);
                    return;
                }
            } else if (gs->girls[club_member_id].flags.f.met && gs->girls[club_member_id].flags.f.b2
                       && !((ClubFlagBits *)&gs->clubs[club_id])->b29
                       && !gs->girls[club_member_id].flags.f.b18) {
                gs->girls[club_member_id].flags.f.b18 = 1;
                game_mode_set(GAME_MODE_CLUB_CAMP_2, club_id);
                return;
            }
        }
    }
    day_event_next(0);
}

void holiday_mode_stage_magazine(void) {
    switch (game_state.mode_step) {
    case 0:
        holiday_magazine_init();
        break;
    case 1:
        holiday_magazine_show();
        break;
    case 2:
        magazine_exit();
        break;
    }
    message_window_title_update();
}

void holiday_magazine_init(void) {
    hud_magazine_mark_read(0, 0);
    text_system_reset();
    message_window_set_title("情報誌");
    hud_magazine_page_build();
    icon_disp_switch(0);
    hizuke_disp_switch(0);
    mode_step_next();
}

/* FAKE: T-24108, omitting default return keeps the selector copy in the first beq delay slot; the always-true >= 0 check in case 0x62 promotes unk_03F to $v0 across the branch and is folded away by uopt. */
u8 holiday_magazine_hint_index(void) {
    switch (game_state.year) {
    case 0x5F:
        if (game_state.month < 5U) {
            return 0;
        }
        if (game_state.month < 8U) {
            return 1;
        }
        if (game_state.month < 0xBU) {
            return 2;
        }
        return 3;
    case 0x60:
        if (game_state.month < 2U) {
            return 3;
        }
        if (game_state.month < 5U) {
            return 4;
        }
        if (game_state.month < 8U) {
            return 5;
        }
        if (game_state.month < 0xBU) {
            return 6;
        }
        return 7;
    case 0x61:
        if (game_state.month == 8) {
            return 0xD;
        }
        if (game_state.month < 2U) {
            return 7;
        }
        if (game_state.month < 5U) {
            return 8;
        }
        if (game_state.month < 8U) {
            return 9;
        }
        if (game_state.month < 0xBU) {
            return 0xA;
        }
        return 0xB;
    case 0x62:
        if (game_state.month < 2U) {
            return 0xB;
        }
        if (game_state.month >= 0) {
            return 0xC;
        }
        break;
    }
}

void holiday_magazine_show(void) {
    magazine_base_show();
    /* FAKE: byte view of the u8 field avoids the redundant selector copy (move v0,v1), as in TEL
     * (T-10630) on the same field. T-17000 */
    switch (*(u8 *)&game_state.substep) {
    case 0:
        xa_wait();
        return;
    case 1:
        if (pad_code_entered != 0) {
            game_state.substep += 1;
            return;
        }
        mode_step_next();
        return;
    case 2:
        k_sub_reset();
        k_sub_disp_start(0);
        gnsx(game_state.player_family_name);
        sndisp(holiday_magazine_hints[holiday_magazine_hint_index()], 0, 0x1F);
        game_state.substep += 1;
        return;
    case 3:
        xa_wait();
        return;
    case 4:
    default:
        mode_step_next();
        return;
    }
}

void magazine_exit(void) {
    schedule_progress_state_update();
    hizuke_disp_switch(1);
    mode_stage_set(0);
}

void holiday_tel(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        holiday_tel_init();
        break;
    case 1:
        holiday_tel_call();
        break;
    case 2:
        holiday_tel_exit();
        break;
    }
    cal_base_show();
}

void holiday_tel_init(void) {
    text_system_reset();
    actor_clear_upper();
    message_window_title_show(0);
    icon_disp_switch(0);
    telephone_class_init();
    k_sub_reset_point_set();
    gnsx(game_state.player_family_name);
    sndi((u8 *)"誰に電話かけようかな？）", 0, 0x1F);
    k_sub_disp_start(2);
    mode_step_next();
}

void holiday_tel_call(void) {
    menu_check(0, cursor_x, cursor_y);
    menu_bar_show(0);
    menu_cursor_update(0);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_stage_set(0);
        return;
    }
    if ((game_state.pad_pressed & PAD_CIRCLE) && game_state.menu_cursor_0 != -1) {
        if (game_state.menu_cursor_0 == 0xD) {
            mode_stage_set(0);
            return;
        }
        audio_command(0x50A);
        if (game_state.menu_cursor_0 == 0) {
            game_state.current_girl = GIRL_YOSHIO;
        } else if (game_state.menu_cursor_0 == 0xC) {
            game_state.current_girl = GIRL_MIHARU;
        } else {
            game_state.current_girl = game_state.menu_cursor_0 - 1;
        }
        mode_step_next();
    }
}

/* Implicit int: holiday_tel calls it before this definition. */
s32 holiday_tel_exit(void) {
    GameState *g = &game_state;
    s32 phone_digit_index;

    if (g->substep != 11) {
        if ((g->mode_step_timer.w & 7) == 7) {
            if (g->substep == 0) {
                audio_command(0x504);
            } else if (g->substep >= 4) {
                phone_digit_index = g->substep - 4;
                if (g->current_girl == GIRL_YOSHIO) {
                    audio_command((u16)(girl_phone_digits[10][phone_digit_index] + 0x505));
                } else {
                    audio_command((u16)(girl_phone_digits[g->current_girl][phone_digit_index] + 0x505));
                }
            }
            g->substep++;
        }
    } else if (g->current_girl == GIRL_YOSHIO) {
        g->menu_cursor_0 = 0;
        k_sub_reset();
        restore_bgm();
        game_mode_set(GAME_MODE_TELEPHONE_PROFILE);
        mode_stage_set(0);
    } else if (g->current_girl == GIRL_MIHARU) {
        k_sub_reset();
        g->menu_cursor_0 = 12;
        D_801230D0 = 11;
        restore_bgm();
        game_mode_set(GAME_MODE_TELEPHONE_PROFILE);
        mode_stage_set(1);
    } else {
        k_sub_reset();
        g->menu_cursor_0 = g->current_girl + 1;
        D_801230D0 = g->current_girl;
        restore_bgm();
        game_mode_set(GAME_MODE_TELEPHONE_PROFILE);
        mode_stage_set(1);
    }
}

void telephone_class_init(void) {
    s32 i;
    /* FAKE: T-20940, a second local above the four row arrays puts them at sp+0xBC/0xA0/0x84/0x68 as in the
     * original (IDO gives every local a slot); its real use is unknown, here it holds the first row's text. */
    u8 *str;
    s16 x[14];
    s16 y[14];
    s16 w[14];
    s16 h[14];

    set_kanji_string(-0x80, -0x54, 3, "電話番号", 0);
    for (i = 0; i < 14; i++) {
        x[i] = (i / 5) * 0x58 - 0x88;
        y[i] = (i % 5) * 0x10 - 0x38;
        w[i] = 0x48;
        h[i] = 0xE;
        if (i == 0) {
            str = girl_full_name_table[13];
            set_kanji_string(x[i], y[i], 3, str, 0);
        } else if (i == 13) {
            y[i] += 0x10;
            set_kanji_string(x[i], y[i], 3, "取り消し", 0);
        } else if (i == 12) {
            set_kanji_string(x[i], y[i], 3, girl_full_name_table[12], 0);
        } else if (game_state.girls[i - 1].flags.f.b3) {
            set_kanji_string(x[i], y[i], 3, girl_full_name_table[i - 1], 0);
        } else {
            w[i] = 0;
        }
    }
    menu_set(0, 14, x, y, w, h);
    game_state.menu_cursor_0 = 0;
    actor_clear(0x60);
    ((Actor *)actor_pool)[96].unk_01 = 0;
    ((Actor *)actor_pool)[96].flags = 0x40;
    ((Actor *)actor_pool)[96].quad_flags = 0x40000000;
    ((Actor *)actor_pool)[96].draw_flags = 0x84;
    ((Actor *)actor_pool)[96].frame_table = sprite_frame_table;
    ((Actor *)actor_pool)[96].sequence_table = actor_default_sequence_table;
    ((Actor *)actor_pool)[96].tpage_table = actor_default_texture_blocks;
    ((Actor *)actor_pool)[96].unk_14 = D_800C9A54;
    ((Actor *)actor_pool)[96].unk_04 = 8;
    ((Actor *)actor_pool)[96].unk_05 = 1;
    ((Actor *)actor_pool)[96].sequence_slot = 0;
    ((Actor *)actor_pool)[96].blink_period = 2;
    ((Actor *)actor_pool)[96].unk_18 = 0x3F;
    ((Actor *)actor_pool)[96].x = -0x10;
    ((Actor *)actor_pool)[96].y = -0x13;
}

void holiday_club_init(void) {
    message_window_title_show(0);
    text_system_reset();
    x_taku_string_set("クラブ活動をする", "クラブをやめる", 0, 2);
    x_taku_menu_set(1, 2);
    game_state.menu_cursor_1 = 0;
    k_disp_start(2);
    mode_step_next();
}

void holiday_club_select(void) {
    u8 club_departure_message[72];
    GameState *gs;

    menu_check(1, (s32) ((Actor *)actor_pool)->x, (s32) ((Actor *)actor_pool)->y);
    menu_bar_show(1);
    menu_cursor_update(1);
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_stage_set(0);
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_1) {
        case 0:
            mode_step_next();
            return;
        case 1:
            mode_step_set((game_state.mode_step + 2) & 0xFF);
            text_system_reset();
            strcpy(club_departure_message, club_name_table[(u16) game_state.player_flags.h >> 0xC]);
            strcat(club_departure_message, "部を退部した。");
            sndi(club_departure_message, 0, 0xFF);
            gs = &game_state;
            /* FAKE: T-17006, a bit-field store through the borrowed GirlFlagBits view (one ugen temporary more than
             * `|= 1`, so the original's t6..t0 numbering); the record's real bit-field layout is unknown. */
            ((GirlFlagBits *)&gs->clubs[game_state.player_flags.h >> 12].flags)->b0 = 1;
            gs->player_flags.b[1] = (gs->player_flags.b[1] & 0xFF0F) | 0xB0;
            k_disp_start(1);
            break;
        }
    }
}

void holiday_club_exit(void) {
    actor_clear_upper();
    if (girl_player_sports_club_check() == 1 && (u32)(game_state.day + 6) / 7 == 3 && game_state.month % 3U == 0 && game_state.weekday == WEEKDAY_SUNDAY) {
        game_mode_set(GAME_MODE_PRACTICE_MATCH);
        return;
    }
    mode_stage_set(5);
}

void holiday_taibu_club_exit(void) {
    actor_clear_upper();
    if (game_state.pad_pressed & 0x60) {
        mode_stage_set(0);
    }
}

s32 holiday_club(void) {
    switch (game_state.mode_step) {
    case 0:
        holiday_club_init();
        break;
    case 1:
        holiday_club_select();
        break;
    case 2:
        holiday_club_exit();
        break;
    case 3:
        holiday_taibu_club_exit();
        break;
    }
}

void holiday_club_join_exit(void) {
    actor_clear_upper();
    icon_disp_switch(1);
    mode_stage_set(5);
}

void holiday_mode_stage_club_join(void) {
    switch (game_state.mode_step) {
    case 0:
        join_club_init();
        break;
    case 1:
        join_club_select();
        break;
    case 2:
        join_club_message();
        break;
    case 3:
        holiday_club_join_exit();
        break;
    }
    cal_base_show();
}
