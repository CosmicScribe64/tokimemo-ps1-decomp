/* home_screens.c: src/main/8005A0B0.c; T-27059: original object [0x8005A0B0,0x80061710); command/calendar/card/settings screens plus club, rumor and Christmas/New Year prep; reuse prior reviewed m-sysmenu recommendation. Original object 0x8005A0B0-0x80061710 (config/objects/main.txt). */
#define MAIN_API_OVERRIDE_prim_gradient_rect /* implicit declaration, as matched; main_api.h has a prototype with narrow parameters */
#include "common.h"
#include "common.h"
#include "game/game_state.h"
#include "game/system_data.h"
#include "game/word_views.h"
#include "game_enums.h"
#include "libapi.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/boot.h"
#include "main/calendar.h"
#include "main/cd.h"
#include "main/club.h"
#include "main/cursor.h"
#include "main/draw.h"
#include "main/game_mode.h"
#include "main/girl.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/entry_ui_scenes.h"
#include "main/import_views/minigame_scenes_school_events.h"
#include "main/import_views/resident_views.h"
#include "main/import_views/scenes.h"
#include "main/memory_card.h"
#include "main/menu.h"
#include "main/message.h"
#include "main/prim.h"
#include "main/rumor.h"
#include "main/settings.h"
#include "main/speaker.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "main/tpage.h"
#include "main/vram.h"
#include "main/game_mode.h"
#include "main/settings.h"

void prim_gradient_rect();

/* FAKE: the u8 view of the selector keeps the compares on the global ($v1), as in the original (T-5010) */
void settings_command_screen_mode_handler(void) {
    switch (*(u8 *)&game_state.mode_stage) {
    case 0:
        settings_command_screen_mode_stage_setup();
        break;
    case 1:
        settings_command_screen_mode_stage_select();
        break;
    case 2:
        settings_command_screen_mode_stage_calendar();
        break;
    case 3:
        settings_save_load_mode_stage_dispatch();
        break;
    case 4:
        settings_mode_stage_dispatch();
        break;
    case 5:
        join_club();
        break;
    case 0xFF:
        settings_command_screen_mode_stage_reload();
        break;
    }
    if (*D_8011F3FF & 0x80) {
        hud_mascot_update();
    }
    hud_screen_frame_draw(0);
}

/* FAKE: u8 views of the selector and of the counter give the original's global (LOD) access: compares on $v1 and the `andi`/`or v0` copy of the increment (T-5010) */
s32 settings_command_screen_mode_stage_reload(void) {
    game_state.mode_step_timer.w += 1;
    switch (*(u8 *)&game_state.mode_step) {
    case 0:
        settings_command_screen_setup_reset();
        break;
    case 1:
        settings_command_screen_reload_calendar();
        break;
    case 2:
        settings_command_screen_build();
        break;
    case 3:
        settings_room_bg_load_finish();
        break;
    }
    hizuke_show();
    message_window_show();
    parameter_show();
    hud_icon_bar_update(0);
    hud_cursor_update();
    if (game_state.mode_step >= 2) {
        message_window_title_update();
        if (*(u8 *)&game_state.screen_brightness < 0x80) {
            *(u8 *)&game_state.screen_brightness += 4;
        }
    }
}

void settings_command_screen_mode_stage_setup(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        settings_command_screen_setup_reset();
        break;
    case 1:
        settings_command_screen_setup_wait();
        break;
    case 2:
        settings_command_screen_build();
        break;
    case 3:
        settings_room_bg_load_finish();
        break;
    }
    hizuke_show();
    message_window_show();
    parameter_show();
    hud_icon_bar_update(0);
    hud_cursor_update();
    message_window_title_update();
    dec_bg_draw(get_last_gamen_mode());
}

void settings_command_screen_setup_reset(void) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_upper();
    game_state.tile_layer_enabled = 0;
    game_state.boot_hold = 0;
    set_dec_bri(0);
    text_system_reset();
    k_disp_start(2);
    mode_step_next();
}

void settings_command_screen_setup_wait(void) {
    k_disp_inc();
    mode_step_next();
}

void settings_icon_layout_repair(void) {
    /* FAKE: unused words preserve the 0x40 stack frame and the saved_icon_mode spill at sp+0x28. T-24142 */
    s32 pad[2];
    s32 i;
    s32 invalid_icon_coordinate_count;
    s32 zero_position_pair_count;
    s32 saved_icon_mode;

    invalid_icon_coordinate_count = 0;
    zero_position_pair_count = 0;
    /* FAKE: pointer view on unk_F6C emits lbu (declared s8 in header). T-24142 */
    if (*(u8 *)&game_state.icon_mode == 2) {
        for (i = 0; i < 8; i++) {
            if (((s16 *)D_80125C10)[i * 4 + 0] >= 0x7D) {
                invalid_icon_coordinate_count++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 0] < -0x86) {
                invalid_icon_coordinate_count++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 1] < -0x54) {
                invalid_icon_coordinate_count++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 1] >= 0x55) {
                invalid_icon_coordinate_count++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 0] == 0 && ((s16 *)D_80125C10)[i * 4 + 1] == 0) {
                zero_position_pair_count++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 2] >= 0x7D) {
                invalid_icon_coordinate_count++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 2] < -0x86) {
                invalid_icon_coordinate_count++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 3] < -0x54) {
                invalid_icon_coordinate_count++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 3] >= 0x55) {
                invalid_icon_coordinate_count++;
            }
            if (((s16 *)D_80125C10)[i * 4 + 2] == 0 && ((s16 *)D_80125C10)[i * 4 + 3] == 0) {
                zero_position_pair_count++;
            }
        }
    }
    if (zero_position_pair_count >= 2) {
        if (invalid_icon_coordinate_count != 0) {
            game_state.icon_count = 0xF;
            saved_icon_mode = *(u8 *)&game_state.icon_mode;
            *(u8 *)&game_state.icon_mode = 0;
            hud_icon_bar_init();
            icon_disp_switch(0);
            *(u8 *)&game_state.icon_mode = saved_icon_mode;
        }
    }
}

void settings_command_screen_reload_calendar(void) {
    if (game_state.substep == 0) {
        if (audio_bank_load_poll() == 1) {
            audio_bank_start(0, 0);
            game_state.substep += 1;
        }
    } else if (game_state.substep == 1) {
        cd_read_sectors(0x95U, 0x801A0000, 0x9B94);
        game_state.substep += 1;
    } else if (game_state.substep == 2) {
        if (cd_read_sync_status() & 1) {
            calendar_month_load(game_state.year, game_state.month, 0);
            game_state.menu_mode &= 3;
            D_800B5BC8 = 0xFE;
            if (hud_mascot_choice < 0xE) {
                hud_mascot_set(hud_mascot_choice);
                hud_mascot_display_switch(1);
            }
            mode_step_next();
        }
    }
}

/* Weekday command screen setup. The calendar reads are written from `unk_040 - 1` (the original keeps
 * that sum and adds the offsets 2 and 7 to it, T-24301). */
void settings_command_screen_build(void) {
    GameState *gs = &game_state;
    u8 calendar_marker;

    back_clear_switch(0);
    actor_clear_upper();
    sprite_tables_clear();
    gs->clear_color_r = 0;
    gs->clear_color_g = 0;
    gs->clear_color_b = 0;
    text_system_reset();
    message_window_set_title_hidden("平日コマンド画面");
    gnsx(&gs->player_family_name);
    parameter_show_init();
    parameter_show_init();
    hizuke_init();
    message_window_init();
    parameter_disp_switch(1);
    tpage_buf_clear();
    calendar_marker = gs->month_calendar[0].marker[gs->day - 1 + 2];
    if (calendar_marker == 5) {
        gs->icon_count = 0xF;
        hud_icon_bar_init();
        hud_icon_layout_apply((u8)gs->icon_count);
        settings_command_screen_icons_reset(1);
        gs->menu_cursor_0 = 0xE;
        icon_can_use_set(0xE, 1);
        sndisp("明日からテストだ）", 0, 0x1F);
    } else if (calendar_marker == 8 && (gs->player_flags.h >> 12) != 0xB) {
        gs->icon_count = 0xE;
        hud_icon_bar_init();
        hud_icon_layout_apply((u8)gs->icon_count);
        settings_command_screen_icons_reset(1);
        icon_can_use_set(4, 1);
        gs->menu_cursor_0 = 4;
        if ((gs->player_flags.h >> 12) == 6 && gs->year == 0x61 && (*(u16 *)&gs->clubs[6].level & 0xF) >= 8) {
            sndisp("明日から、念願の 全国高校野球大会だ）", 0, 0x1F);
        } else {
            sndisp("明日から夏合宿だ）", 0, 0x1F);
        }
    } else if (gs->month_calendar[0].marker[gs->day - 1 + 7] == 0xC && (gs->player_flags.h >> 12) < 6) {
        if ((gs->player_flags.b[1] & 0xF) != 3) {
            gs->icon_count = 0xE;
            hud_icon_bar_init();
            hud_icon_layout_apply((u8)gs->icon_count);
            settings_command_screen_icons_reset(1);
            icon_can_use_set(4, 1);
            gs->menu_cursor_0 = 4;
            sndisp("文化祭に向けて部活をせねば）", 0, 0x1F);
        } else {
            gs->icon_count = 0xE;
            hud_icon_bar_init();
            hud_icon_layout_apply((u8)gs->icon_count);
            settings_command_screen_icons_reset(1);
            icon_can_use_set(7, 1);
            gs->menu_cursor_0 = 7;
            settings_room_sick_icon_load();
            sndisp("文化祭に向けて部活をせねば でも病気で身体が動かない）", 0, 0x1F);
        }
    } else if (calendar_marker == 0xB) {
        gs->icon_count = 0xF;
        hud_icon_bar_init();
        hud_icon_layout_apply((u8)gs->icon_count);
        settings_command_screen_icons_reset(1);
        gs->menu_cursor_0 = 0xE;
        icon_can_use_set(0xE, 1);
        sndisp("明日から修学旅行だ）", 0, 0x1F);
    } else if (gs->player_flags_upper.h & 1) {
        gs->icon_count = 0xE;
        hud_icon_bar_init();
        hud_icon_layout_apply((u8)gs->icon_count);
        settings_command_screen_icons_reset(1);
        icon_can_use_set(0, 1);
        icon_can_use_set(1, 1);
        icon_can_use_set(2, 1);
        gs->menu_cursor_0 = 0;
        sndisp("一週間強制補習を しなければならない）", 0, 0x1F);
    } else if ((gs->player_flags.b[1] & 0xF) == 3) {
        gs->icon_count = 0xE;
        hud_icon_bar_init();
        hud_icon_layout_apply((u8)gs->icon_count);
        settings_command_screen_icons_reset(1);
        icon_can_use_set(7, 1);
        gs->menu_cursor_0 = 7;
        settings_room_sick_icon_load();
        sndisp("病気で身体が動かない）", 0, 0x1F);
    } else {
        gs->icon_count = 0xE;
        hud_icon_bar_init();
        hud_icon_layout_apply((u8)gs->icon_count);
        settings_command_screen_icons_reset(0);
        gs->menu_cursor_0 = gs->last_command.w & 0xF;
        sndisp("明日から何をしようかな？）", 0, 0x1F);
    }
    k_disp_start(4);
    hud_cursor_actor_init(0);
    if (gs->year == 0x5F && gs->month == 4 && gs->day == 4) {
        icon_can_use_set(0xD, 0);
    }
    menu_bar_color(0x10, 0, 0, 8);
    settings_room_bg_load_start();
}

void settings_room_bg_load_start(void) {
    s32 r;

    r = dec_bg_cd_read(D_800B5950[hud_season_index()], 0);
    switch (r) {
    case 0:
    case 1:
        dec_bg_show_set(0, r);
        set_dec_bri(0x80);
        game_state.screen_brightness = 0x80;
        mode_stage_next();
        break;
    case -1:
        mode_step_next();
        break;
    }
}

void settings_room_bg_load_finish(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(0, dec_bg_decode_to_vram(0));
        if (bg_decode_result == -1) {
            dec_bg_reset();
            dec_bg_cd_read(D_800B5950[hud_season_index()], 0);
        } else {
            set_dec_bri(0x80);
            game_state.screen_brightness = 0x80;
            mode_stage_set(1);
        }
    }
}

void settings_command_screen_icons_reset(s32 disable_weekday_commands) {
    s32 i;

    icon_can_use_set(0xA, 0);
    icon_can_use_set(8, 0);
    icon_can_use_set(9, 0);
    if (*(u8 *)&game_state.icon_mode == 2) {
        icon_can_use_set(0xE, 0);
    }
    if ((game_state.player_flags.h >> 0xC) == 0xB) {
        icon_can_use_set(4, 0);
    }
    if (disable_weekday_commands != 0) {
        for (i = 0; i != 8; i++) {
            icon_can_use_set(i, 0);
        }
    }
}

void settings_room_sick_icon_load(void) {
    LoadSquare(0x3F0, 0x160, 0x10, 0x18, D_800C9A60);
    load_palette(D_800C9D60, 0xF, 1, 1, 0);
}

void settings_room_sick_icon_show(void) {
    if ((game_state.player_flags.b[1] & 0xF) == 3) {
        sprite_quads[0].flags = 0x01000000;
        sprite_quads[0].x = -0xC;
        sprite_quads[0].y = -0xA;
        sprite_quads[0].w = 0x1E;
        sprite_quads[0].h = 0x18;
        sprite_quads[0].unk_0C = 0x1E;
        sprite_quads[0].unk_0E = 0xE0;
        sprite_quads[0].unk_0F = 0x60;
        sprite_quads[0].unk_10 = 0;
        sprite_quads[0].unk_12 = 0x1EF;
        sprite_quads[0].tint_r = 0x80;
        sprite_quads[0].tint_g = 0x80;
        sprite_quads[0].tint_b = 0x80;
        sprite_quads[0].anchor_x = 0;
        sprite_quads[0].anchor_y = 0;
        sprite_quads[0].unk_1C = 0x1000;
        sprite_quads[0].unk_1E = 0x1000;
        sprite_quads[0].unk_20 = 0;
        game_state.quad_layer[0] = 0xA;
    }
}

void settings_command_screen_mode_stage_select(void) {
    u8 card_job_state; /* T-9410: keep the selector in v1 across the compare chain. */
    if (game_state.mode_step == 0) {
        settings_command_screen_select_frame();
    }
    settings_room_sick_icon_show();
    card_job_state = game_state.memory_card_job;
    if (card_job_state == 1) {
        if (memory_card_system_write_poll() == 0x13) {
            audio_command(0x50F);
            game_state.memory_card_job = 0;
            memory_card_stop();
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            audio_command(0x502);
            game_state.memory_card_job = 0;
            memory_card_stop();
        }
    } else if (card_job_state == 0) {
        hud_icon_memory_card_animation_set(0U);
    }
}

s32 settings_room_click_window(void) {
    if (cursor_x < 0x22 && cursor_x >= -0xD && cursor_y < 8 && cursor_y >= -0xB) {
        if (game_state.pad_pressed & PAD_CIRCLE) {
            audio_command(0xB1);
            audio_bgm_stop(0);
            cd_audio_request_set(0x1D000185, 0x17000397, 0);
            audio_command(0x300);
            return 1;
        }
    }
    return 0;
}

s32 settings_room_click_help(void) {
    if (cursor_x < 0x50 && cursor_x >= 0x41 && cursor_y < -0x10 && cursor_y >= -0x1F) {
        if (game_state.pad_pressed & PAD_CIRCLE) {
            game_mode_set(GAME_MODE_HELP);
            return 1;
        }
    }
    return 0;
}

s32 settings_room_click_radio(void) {
    u32 *p;

    if ((cursor_x < 8) && (cursor_x >= -0x27) && (cursor_y < 0x24) && (cursor_y >= 0x19)) {
        if (game_state.pad_pressed & PAD_CIRCLE) {
            audio_command(0xD1);
            audio_bgm_stop(0);
            p = settings_room_radio_clips + ((u16)game_state.day_count & 7) * 2;
            cd_audio_request_set(p[0], p[1], 0xE);
            audio_command(0x300);
            return 1;
        }
    }
    return 0;
}

s32 settings_room_click_radio_holiday(void) {
    u32 *p;

    if ((cursor_x < 8) && (cursor_x >= -0x27) && (cursor_y < 0x24) && (cursor_y >= 0x19)) {
        if (game_state.pad_pressed & PAD_CIRCLE) {
            audio_command(0xD1);
            audio_bgm_stop(0);
            p = settings_room_radio_clips_holiday + ((u16)game_state.day_count & 7) * 2;
            cd_audio_request_set(p[0], p[1], 0xE);
            audio_command(0x300);
            return 1;
        }
    }
    return 0;
}

s32 settings_room_click_mascot_holiday(void) {
    /* FAKE: unused word homes preserve current_girl at sp+0x2C and frame 0x38. T-10010. */
    s32 pad[2];
    s32 current_girl;
    s32 girl_voice_variant;
    s32 *voice_clip_pair;

    if ((*D_8011F3FF & 0x80) && (cursor_x < (D_8011F422 + 0xC)) && ((D_8011F422 - 0xC) < cursor_x) && (cursor_y < (D_8011F426 + 0x30)) && (D_8011F426 < cursor_y) && (game_state.pad_pressed & PAD_CIRCLE) && ((u32) (game_state.frame_count - settings_room_mascot_holiday_stamp) >= 0xF1U)) {
        game_state.current_girl = D_8011F414;
        current_girl = (s32) D_8011F414;
        if (get_g_zyotai_s((s32) D_8011F414) & 0x80) {
            girl_voice_variant = 1;
        } else {
            girl_voice_variant = 0;
        }
        voice_clip_pair = D_800B3B9C + current_girl * 4 + girl_voice_variant * 2;
        cd_audio_request_set(voice_clip_pair[0], voice_clip_pair[1], current_girl);
        audio_command(0x300);
        return 1;
    }
    return 0;
}

s32 settings_room_click_mascot(void) {
    s32 *voice_clip_pair;

    if ((*D_8011F3FF & 0x80) && (cursor_x < (D_8011F422 + 0xC)) && ((D_8011F422 - 0xC) < cursor_x) && (cursor_y < (D_8011F426 + 0x30)) && (D_8011F426 < cursor_y) && (game_state.pad_pressed & PAD_CIRCLE) && ((u32) (game_state.frame_count - settings_room_mascot_stamp) >= 0xF1U)) {
        voice_clip_pair = D_800B37D4 + D_8011F414 * 2;
        cd_audio_request_set(voice_clip_pair[0], voice_clip_pair[1], (s32)(game_state.current_girl = D_8011F414));
        audio_command(0x300);
        return 1;
    }
    return 0;
}

void settings_command_screen_select_frame(void) {
    k_disp_inc();
    parameter_show();
    hizuke_show();
    message_window_show();
    hud_icon_bar_update(0);
    hud_cursor_update();
    message_window_title_update();
    dec_bg_draw(0);
    if (settings_command_screen_icon_confirm() == 0 && settings_room_click_mascot() == 0 && settings_room_click_radio() == 0 && settings_room_click_window() == 0) {
        settings_room_click_help();
    }
}

s32 settings_command_screen_icon_confirm(void) {
    s32 pad[3]; /* FAKE: T-25004, frame 0x38 with the result at sp+0x28 (T-24301). */
    s32 menu_result;
    s32 selected_girl;
    u32 player_club_id;
    u8 tomorrow_marker;

    if (((u8) game_state.icon_mode == ICON_MODE_FREE) && (game_state.pad_held & PAD_L2)) {
        return 1;
    }
    menu_result = 0;
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if (game_state.menu_cursor_0 >= 0) {
            menu_result = 1;
            cd_audio_command(9, 0, 0);
            audio_command(3);
            if (audio_bgm_is_playing() == 0) {
                audio_bank_start(0, 0);
            }
        }
        switch (game_state.menu_cursor_0) {
        case 4:
            if (get_icon_can_use(game_state.menu_cursor_0) != 0) {
                audio_command(0x500);
                if ((game_state.player_flags.h >> 0xC) == 0xB) {
                    game_state.last_command.date.month = game_state.menu_cursor_0;
                    mode_stage_set(5);
                } else {
                    /* T-25039: define the call argument only on this path so
                     * the shared shift stays in $v1 until the call. */
                    player_club_id = game_state.player_flags.h >> 0xC;
                    /* FAKE: T-25004, a second read of the halfword makes uopt keep the load in a register
                     * ($a1), so the shift costs no ugen temporary (permuter find). */
                    if (game_state.player_flags.h) {
                    }
                    selected_girl = girl_find_by_club(player_club_id);
                    if ((selected_girl != -1) && game_state.girls[selected_girl].flags.f.b2) {
                        game_state.girls[selected_girl].affection += 1;
                    }
                    game_mode_set(GAME_MODE_SCHEDULE);
                    game_state.last_command.date.month = game_state.menu_cursor_0;
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
        case 8:
        case 9:
        case 10:
            if (get_icon_can_use(game_state.menu_cursor_0) != 0) {
                if ((game_state.menu_cursor_0 != 8) && (game_state.menu_cursor_0 != 0xA)) {
                    audio_command(0x500);
                } else {
                    audio_command(0x50D);
                }
                game_mode_set(GAME_MODE_SCHEDULE);
                game_state.last_command.date.month = game_state.menu_cursor_0;
            }
            break;
        case 12:
            audio_command(0x50D);
            mode_stage_set(2);
            break;
        case 13:
            if (get_icon_can_use(game_state.menu_cursor_0) != 0) {
                if (game_state.memory_card_job == 0) {
                    settings_save_load_layout_stash(0);
                    audio_command(0x50D);
                    mode_stage_set(3);
                }
            }
            break;
        case 11:
            audio_command(0x50D);
            mode_stage_set(4);
            break;
        case 14:
            if (get_icon_can_use(game_state.menu_cursor_0) != 0) {
                game_state.last_command.date.month = 0xE;
                tomorrow_marker = (&game_state.month_calendar[0].marker[1])[game_state.day];
                if (tomorrow_marker == 5) {
                    audio_command(0x500);
                    game_mode_set(GAME_MODE_SCHEDULE);
                    sndisp("明日からテストだ）", 0, 0x1F);
                } else if (tomorrow_marker == 0xB) {
                    audio_command(0x500);
                    game_mode_set(GAME_MODE_SCHEDULE);
                    sndisp("明日から修学旅行だ）", 0, 0x1F);
                } else {
                    game_mode_set(GAME_MODE_SCHEDULE);
                    game_state.last_command.date.month = 7;
                }
            }
            break;
        }
    }
    return menu_result;
}

void settings_command_screen_mode_stage_calendar(void) {
    switch (game_state.mode_step) {
    case 0:
        settings_command_screen_calendar_init();
        break;
    case 1:
        settings_command_screen_calendar_wait();
        break;
    case 2:
        settings_command_screen_calendar_exit();
        break;
    }
    calendar_view_update();
    parameter_show();
    hizuke_show();
    message_window_show();
    message_window_title_update();
    hud_cursor_update();
}
INCLUDE_RODATA("asm/data/main/home_screens.rodata", D_800AFF6C);

INCLUDE_RODATA("asm/data/main/home_screens.rodata", D_800AFF78);

void settings_command_screen_calendar_init(void) {
    text_system_reset();
    message_window_set_title(D_800AFF6C);
    set_kanji_string(-0x80, 0x32, 0, D_800AFF78, 0);
    k_disp_start(2);
    cal_sprite_init();
    calendar_screen_init();
    icon_disp_switch(0);
    mode_step_next();
}

void settings_command_screen_calendar_wait(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_next();
    }
}

void settings_command_screen_calendar_exit(void) {
    mode_stage_set(0);
    mode_step_set(2);
}

void settings_save_load_mode_stage_dispatch(void) {
    u8 mode_step;
    mode_step = (u8) game_state.mode_step;
    switch (mode_step) {
    case 0x0:
        settings_save_load_menu_init();
        break;
    case 0x1:
        settings_save_load_card_check();
        break;
    case 0x2:
        settings_save_load_menu();
        break;
    case 0x3:
        settings_save_load_save_init();
        break;
    case 0x4:
        settings_save_load_save_select();
        break;
    case 0x5:
        settings_save_load_save_write();
        break;
    case 0x6:
        settings_save_load_load_init();
        break;
    case 0x7:
        settings_save_load_load_select();
        break;
    case 0x8:
        settings_save_load_load_read();
        break;
    case 0x9:
        settings_save_load_load_finish();
        break;
    case 0xA0:
        settings_save_load_format_prompt();
        break;
    case 0xB0:
        settings_save_load_load_info_read();
        break;
    case 0xC0:
        settings_save_load_save_info_read();
        break;
    case 0xE0:
        settings_save_load_data_broken();
        break;
    case 0xF0:
        settings_save_load_card_full();
        break;
    case 0xFF:
        settings_save_load_card_unreadable();
        break;
    }
    message_window_title_update();
    cal_base_show();
    message_window_show();
    parameter_show();
    hizuke_show();
    hud_cursor_update();
}

s32 settings_save_load_data_broken(void) {
    switch (game_state.substep) {
    case 0:
        audio_command(0x502);
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, "データが壊れています", 0);
        k_sub_disp_start(1);
        game_state.substep += 1;
        break;
    case 1:
        if (game_state.pad_pressed & 0x860) {
            mode_step_set(6);
        }
        break;
    }
}

s32 settings_save_load_card_unreadable(void) {
    switch (game_state.substep) {
    case 0:
        audio_command(0x502);
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, "メモリーカードを認識できません", 0);
        k_sub_disp_start(1);
        game_state.substep += 1;
        break;
    case 1:
        if (game_state.pad_pressed & 0x860) {
            settings_save_load_layout_stash(1);
            mode_stage_set(0);
            mode_step_set(2);
        }
        break;
    }
}

s32 settings_save_load_card_full(void) {
    switch (game_state.substep) {
    case 0:
        audio_command(0x502);
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, "メモリーカードに空きがありません", 0);
        k_sub_disp_start(1);
        game_state.substep += 1;
        break;
    case 1:
        if (game_state.pad_pressed & 0x860) {
            settings_save_load_layout_stash(1);
            mode_stage_set(0);
            mode_step_set(2);
        }
        break;
    }
}

void settings_save_load_menu_init(void) {
    /* FAKE: reserves the observed stack home; grouped stores preserve scheduling. T-12030. */
    s32 stack_home;
    s16 x[2];
    s16 y[2];
    s16 w[2];
    s16 h[2];

    text_system_reset();
    message_window_set_title_hidden("メモリーカード");
    k_sub_reset_point_set();
    icon_disp_switch(0);
    data_save_load_class_init();
    x[0] = -0x50;
    x[1] = 0x10;
    y[0] = -0x10;
    y[1] = -0x10;
    w[0] = 0x30; w[1] = 0x30;
    h[0] = 0x14; h[1] = 0x14;
    set_kanji_string(x[0], y[0], 0xFU, "セーブ", 0);
    set_kanji_string(x[1], y[1], 0xFU, "ロード", 0);
    k_disp_switch(1, 0);
    k_disp_switch(2, 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0U, D_800B59F8, 0);
    set_kanji_string(-0x80, 0x42, 0U, D_800B5A1C, 0);
    set_kanji_string(-0x80, 0x4A, 0U, D_800B5A1C, 0);
    set_kanji_string(-0x80, 0x52, 0U, D_800B5A1C, 0);
    set_kanji_string(-0x80, 0x5A, 0U, D_800B5A1C, 0);
    k_sub_disp_start(1);
    memory_card_state_reset();
    menu_set(1, 2, x, y, w, h);
    game_state.menu_cursor_1 = 0;
    mode_step_next();
}

s32 settings_save_load_card_check(void) {
    u8 *card_result;

    card_result = &game_state.memory_card_result;
    switch (game_state.substep) {
    case 0:
        if (memory_card_check_poll() != 0) {
            if (*card_result == 1) {
                memory_card_stop();
                game_state.substep++;
            } else if (*card_result == 0xF1) {
                memory_card_stop();
                mode_step_set(0xA0);
            } else if (*card_result == 0xFF) {
                memory_card_stop();
                mode_step_set(0xFF);
            }
        } else if ((u32)game_state.memory_card_timer > 0x200) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    case 1:
        memory_card_start();
        game_state.substep++;
        break;
    case 2:
        if (memory_card_scan_poll() == 2) {
            game_state.substep++;
        } else if ((u32)game_state.memory_card_timer > 0x200) {
            mode_step_set(0xFF);
        }
        break;
    case 3:
        if ((u8)D_800E8BEE != 0) {
            memory_card_stop();
            memory_card_start();
            game_state.substep++;
        } else {
            memory_card_stop();
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "セーブまたはロードを選んでください", 0);
            k_disp_switch(1, 1);
            k_disp_switch(2, 1);
            k_disp_start(4);
            mode_step_next();
        }
        break;
    case 4:
        if (memory_card_system_read_poll() == 3) {
            memory_card_system_bank_load(0);
            settings_save_load_layout_stash(1);
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "セーブまたはロードを選んでください", 0);
            k_disp_switch(1, 1);
            k_disp_switch(2, 1);
            k_disp_start(4);
            mode_step_next();
            memory_card_stop();
        } else if (((u32)game_state.memory_card_timer > 0x200) || (*card_result == 0xF0)) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    }
    k_disp_inc();
}

s32 settings_save_load_format_prompt(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 0xF, "未初期化のメモリーカードです。", 0);
        k_disp_start(1);
        game_state.substep++;
    default:
        break;
    case 1:
        if (game_state.pad_pressed & 0x860) {
            game_state.substep++;
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 0xF, "初期化していいですか？", 0);
        set_kanji_string(-0x80, 0x42, 0, D_800B3C70, 0);
        set_kanji_string(-0x2C, 0x42, 1, hud_confirm_button_label(), 0);
        set_kanji_string(-0x80, 0x52, 0, D_800B3C88, 0);
        set_kanji_string(-0x2C, 0x52, 2, hud_cancel_button_label(), 0);
        k_sub_disp_start(5);
        game_state.substep++;
        break;
    case 3:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            if (memory_card_format() == 1) {
                mode_step_set(0);
            } else {
                settings_save_load_layout_stash(1);
                mode_stage_set(0);
                mode_step_set(2);
            }
        } else if (game_state.pad_pressed & PAD_CROSS) {
            game_state.substep++;
        }
        break;
    case 4:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 0xF, "別のメモリーカードを", 0);
        set_kanji_string(-0x80, 0x42, 1, "差し込み口１", 0);
        set_kanji_string(-0x80, 0x42, 0xF, "　　　　　　にセットして、", 0);
        set_kanji_string(-0x80, 0x52, 0xF, "○ボタンを押してください。", 0);
        k_sub_disp_start(4);
        game_state.substep++;
        break;
    case 5:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            settings_save_load_layout_stash(1);
            mode_stage_set(0);
            mode_step_set(2);
        }
        break;
    }
}

void settings_save_load_menu(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_cursor_update(1);
    menu_bar_show(1);
    if (game_state.pad_pressed & PAD_CROSS) {
        settings_save_load_layout_stash(1);
        mode_stage_set(0);
        mode_step_set(2);
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_1) {
        case 0:
            mode_step_set(3);
            return;
        case 1:
            mode_step_set(6);
            break;
        }
    }
}

void settings_save_load_layout_stash(s32 restore_layout) {
    if (restore_layout != 0) {
        memcpy(D_80125C10, game_state.scratch_words, 0x40);
        D_80125C50 = game_state.scratch_words[32];
        D_80125C52 = game_state.scratch_words[33];
        hud_mascot_choice = *(u8 *)&game_state.scratch_words[34]; /* the low byte of the word stored below */
        D_80125C54 = game_state.scratch_words[35];
        D_80125C55 = game_state.scratch_words[36];
        return;
    }
    memcpy(game_state.scratch_words, D_80125C10, 0x40);
    game_state.scratch_words[32] = D_80125C50;
    game_state.scratch_words[33] = D_80125C52;
    game_state.scratch_words[34] = hud_mascot_choice;
    game_state.scratch_words[35] = D_80125C54;
    game_state.scratch_words[36] = D_80125C55;
}

void settings_save_load_save_init(void) {
    s32 i;
    s32 d;
    SystemData *work;
    s16 x[16];
    s16 y[16];
    s16 w[16];
    s16 h[16];

    text_system_reset();
    message_window_set_title_hidden("データセーブ");
    d = 5;
    for (i = 0; i < 14; i++) {
        x[i] = (i / d) * 32 - 124;
        y[i] = (i % d) * 16 - 64;
        w[i] = 32;
        h[i] = 16;
        set_kanji_string(x[i], y[i], 15, D_800B32CC[i], 0);
    }
    menu_set(1, 14, x, y, w, h);
    game_state.menu_cursor_1 = 0;
    k_sub_reset_point_set();
    work = &system_data;
    for (i = 1; i != 15; i++) {
        if (work->slot_status[i] == 1) {
            break;
        }
    }
    if ((u8) D_800E8BEE != 0) {
        game_state.menu_cursor_1 = D_800E7D36 - 1;
        game_state.save_slot = (s8) D_800E7D36;
        mode_step_set(4);
        hud_save_slot_info_show(0, 0);
        settings_save_load_save_prompt();
    } else if (i == 0xF) {
        settings_save_load_save_prompt();
        mode_step_set(4);
    } else {
        game_state.save_slot = i;
        mode_step_set(0xC0);
    }
    settings_save_load_save_marks();
}

INCLUDE_RODATA("asm/data/main/home_screens.rodata", D_800B00FC);

void settings_save_load_save_prompt(void) {
    set_kanji_string(-0x80, 0x32, 0, D_800B00FC, 0);
    k_sub_disp_start(1);
}

void settings_save_load_save_marks(void) {
    s32 i;
    s32 d;
    u8 *p;

    d = 5;
    for (i = 0, p = (u8 *)&system_data; i < 0xE; i++) {
        if (p[5] == 0) {
            prim_gradient_rect((i / d) * 0x20 - 0x7C, (i % d) * 0x10 - 0x40, 0x1E, 0xF, 2, 0x808080, 0x81);
        }
        if (p[5] == 0xFF) {
            prim_gradient_rect((i / d) * 0x20 - 0x7C, (i % d) * 0x10 - 0x40, 0x1E, 0xF, 2, 0x8080, 0x81);
        }
        p += 1;
    }
    dtd_on_tpage(0, 0, 2, 2, 0);
}

/* FAKE: the u8 view of the selector keeps the compares on the global ($v1), as in the original (T-5010) */
void settings_save_load_save_info_read(void) {
    switch (*(u8 *)&game_state.substep) {
    case 0:
        memory_card_start();
        game_state.substep += 1;
        break;
    case 1:
        if (memory_card_save_read_poll() == 4) {
            memory_card_stop();
            game_state.substep += 1;
        } else if (game_state.memory_card_timer >= 0x201U || game_state.memory_card_result == 0xD0) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    case 2:
        if (memory_card_bank_validate(0) == 1) {
            mode_step_set(4);
            hud_save_slot_info_show(0, 0);
            settings_save_load_save_prompt();
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            mode_step_set(0xFF);
        }
        break;
    }
    settings_save_load_save_marks();
}

/* The menu handler of ETC func_80144DF0 with the cancel arm first (T-24301). */
void settings_save_load_save_select(void) {
    s32 pad[2]; /* FAKE: T-24301, with `pad2` reproduces the 0x40 frame and the slots of slot_status/save_slot_index */
    u8 *slot_status;
    s32 save_slot_index;
    s32 pad2; /* FAKE: T-24301, see pad */

    menu_check(1, cursor_x, cursor_y);
    menu_cursor_update(1);
    menu_bar_show(1);
    save_slot_index = game_state.menu_cursor_1 + 1;
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        settings_save_load_layout_stash(1);
        mode_stage_set(0);
        mode_step_set(2);
        game_state.memory_card_job = 0;
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        if (game_state.menu_cursor_1 != -1) {
            slot_status = &system_data.slot_status[save_slot_index];
            if (*slot_status == 0) {
                /* FAKE: T-24301 (as T-24209), byte view store + plain re-read promote unk_1118 to v0 */
                ((u8 *)&game_state)[0x1118] = save_slot_index;
                if (memory_card_file_create(game_state.save_slot) == 1) {
                    memory_card_file_delete(game_state.save_slot);
                    *slot_status = 0xFF;
                    memory_card_start();
                    mode_step_next();
                } else {
                    mode_step_set(0xF0);
                }
            } else if (save_slot_index == game_state.save_slot) {
                memory_card_start();
                mode_step_next();
            } else {
                game_state.save_slot = save_slot_index;
                if ((u8) D_800E8BEE != 0) {
                    /* FAKE: T-24301 (as T-24209), the repeated view store keeps unk_1118 promoted in both arms */
                    ((u8 *)&game_state)[0x1118] = save_slot_index;
                    D_800E7D36 = game_state.save_slot;
                    hud_save_slot_info_show(0, 0);
                    game_state.menu_cursor_1 = D_800E7D36 - 1;
                } else {
                    mode_step_set(0xC0);
                }
            }
        }
    }
    k_disp_inc();
    settings_save_load_save_marks();
}

/* FAKE: u8 view of the switch selector keeps the compares on the global ($v1), as in the original (T-5010, T-24111) */
s32 settings_save_load_save_write(void) {
    s32 pressed_buttons;
    u32 voice_girl_id;
    u8 save_slot_index;
    s32 *voice_clip_pair;

    switch (*(u8 *)&game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1U, "上書きしてもいいですか？", 0);
        set_kanji_string(-0x80, 0x42, 0U, D_800B3C70, 0);
        set_kanji_string(-0x2C, 0x42, 1U, hud_confirm_button_label(), 0);
        set_kanji_string(-0x80, 0x52, 0U, D_800B3C88, 0);
        set_kanji_string(-0x2C, 0x52, 2U, hud_cancel_button_label(), 0);
        k_sub_disp_start(5);
        game_state.substep = game_state.substep + 1;
        break;
    case 1:
        pressed_buttons = game_state.pad_pressed;
        if (pressed_buttons & 0x20) {
            game_state.substep = game_state.substep + 1;
        } else if (pressed_buttons & 0x40) {
            mode_step_set(3);
            save_slot_index = game_state.save_slot;
            if (system_data.slot_status[save_slot_index] == 0xFF) {
                system_data.slot_status[save_slot_index] = 0;
            }
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1U, "データセーブ中です", 0);
        if ((u8) D_800E8BEE != 0) {
            set_kanji_string(-0x80, 0x42, 1U, "アイコンの点滅が終わるまで", 0);
            set_kanji_string(-0x80, 0x52, 1U, "電源を切らないで下さい", 0);
        }
        k_sub_disp_start(3);
        game_state.substep = game_state.substep + 1;
        break;
    case 3:
        if (memory_card_save_write_poll() == 0x14) {
            D_800E7D14[game_state.save_slot] = 1;
            game_state.substep = game_state.substep + 1;
            memory_card_stop();
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            voice_girl_id = (u8) game_state.current_girl % 13U;
            voice_clip_pair = (voice_girl_id * 2) + D_800B39C4;
            cd_audio_request_set(voice_clip_pair[0], voice_clip_pair[1], (s32) voice_girl_id);
            audio_command(0x300);
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    case 4:
        if ((u8) D_800E8BEE != 0) {
            memory_card_start();
            game_state.memory_card_job = 1;
            D_800E7D36 = game_state.save_slot;
            game_state.save_slot = 0;
            memory_card_slot_summary_store();
            settings_save_load_layout_stash(1);
            mode_stage_set(0);
            mode_step_set(2);
        } else {
            settings_save_load_layout_stash(1);
            mode_stage_set(0);
            mode_step_set(2);
        }
        break;
    }
    settings_save_load_save_marks();
}

void settings_save_load_load_init(void) {
    s32 i;
    s32 d;
    SystemData *work;
    s16 x[16];
    s16 y[16];
    s16 w[16];
    s16 h[16];

    text_system_reset();
    message_window_set_title_hidden("データロード");
    d = 5;
    for (i = 0; i < 14; i++) {
        x[i] = (i / d) * 32;
        y[i] = (i % d) * 16 - 64;
        w[i] = 32;
        h[i] = 16;
        set_kanji_string(x[i], y[i], 15, D_800B32CC[i], 0);
    }
    menu_set(1, 14, x, y, w, h);
    game_state.menu_cursor_1 = 0;
    k_sub_reset_point_set();
    work = &system_data;
    for (i = 1; i != 15; i++) {
        if (work->slot_status[i] == 1) {
            break;
        }
    }
    if ((u8) D_800E8BEE != 0) {
        game_state.menu_cursor_1 = D_800E7D36 - 1;
        mode_step_next();
        game_state.save_slot = (s8) D_800E7D36;
        hud_save_slot_info_show(0, 1);
        settings_save_load_load_prompt();
    } else if (i == 0xF) {
        settings_save_load_load_prompt();
        mode_step_next();
    } else {
        game_state.save_slot = i;
        mode_step_set(0xB0);
    }
    settings_save_load_load_marks();
}

INCLUDE_RODATA("asm/data/main/home_screens.rodata", D_800B0194);

void settings_save_load_load_prompt(void) {
    set_kanji_string(-0x80, 0x32, 0, D_800B0194, 0);
    k_sub_disp_start(1);
}

void settings_save_load_load_marks(void) {
    s32 i;
    s32 d;
    u8 *p;

    d = 5;
    for (i = 0, p = (u8 *)&system_data; i < 0xE; i++) {
        if (p[5] == 0) {
            prim_gradient_rect((i / d) << 5, (i % d) * 0x10 - 0x40, 0x1E, 0xF, 2, 0x808080, 0x81);
        }
        if (p[5] == 0xFF) {
            prim_gradient_rect((i / d) << 5, (i % d) * 0x10 - 0x40, 0x1E, 0xF, 2, 0x8080, 0x81);
        }
        p += 1;
    }
    dtd_on_tpage(0, 0, 2, 2, 0);
}

void settings_save_load_load_info_read(void) {
    switch (game_state.substep) {
    case 0:
        memory_card_start();
        game_state.substep += 1;
        break;
    case 1:
        if (memory_card_save_read_poll() == 4) {
            memory_card_stop();
            game_state.substep += 1;
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xD0)) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    case 2:
        if (memory_card_bank_validate(0) == 1) {
            hud_save_slot_info_show(0, 1);
            mode_step_set(7);
            settings_save_load_load_prompt();
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            mode_step_set(0xFF);
        }
        break;
    }
    settings_save_load_load_marks();
}

s32 settings_save_load_load_select(void) {
    /* FAKE: two-word local reserves the observed sp+0x2C spill home. T-12030. */
    s32 stack_home[2];
    s32 save_slot_index;

    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    save_slot_index = game_state.menu_cursor_1 + 1;
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if (D_800E7D14[save_slot_index] != 0) {
            if (save_slot_index == (u8) game_state.save_slot) {
                memory_card_start();
                mode_step_next();
            } else {
                game_state.save_slot = (s8) save_slot_index;
                if ((u8) D_800E8BEE != 0) {
                    D_800E7D36 = (u8) game_state.save_slot;
                    hud_save_slot_info_show(0, 1);
                } else {
                    mode_step_set(0xB0);
                }
            }
        }
    } else if (game_state.pad_pressed & PAD_CROSS) {
        settings_save_load_layout_stash(1);
        mode_stage_set(0);
        mode_step_set(2);
    }
    k_disp_inc();
    settings_save_load_load_marks();
}

void settings_save_load_load_read(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 2U, "データロード中です", 0);
        set_kanji_string(-0x80, 0x42, 2U, "しばらくお待ち下さい", 0);
        k_sub_disp_start(2);
        game_state.substep += 1;
        break;
    case 1:
        if (memory_card_save_read_poll() == 4) {
            game_state.substep += 1;
            memory_card_stop();
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    case 2:
        if (memory_card_bank_validate(0) == 1) {
            game_state.substep += 1;
        } else {
            mode_step_set(0xE0);
        }
        break;
    case 3:
        k_sub_reset();
        hud_save_slot_info_show(0, 1);
        set_kanji_string(-0x80, 0x32, 1U, "このデータの続きをプレイしますか？", 0);
        set_kanji_string(-0x80, 0x42, 0U, D_800B3C70, 0);
        set_kanji_string(-0x2C, 0x42, 1U, hud_confirm_button_label(), 0);
        set_kanji_string(-0x80, 0x52, 0U, D_800B3C88, 0);
        set_kanji_string(-0x2C, 0x52, 2U, hud_cancel_button_label(), 0);
        k_sub_disp_start(0xB);
        game_state.substep += 1;
        break;
    case 4:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            audio_command(0x501);
            game_state.substep += 1;
        } else if (game_state.pad_pressed & PAD_CROSS) {
            mode_step_set(6);
        }
        break;
    case 5:
        if (memory_card_game_bank_load(0) == 1) {
            mode_step_next();
        } else {
            mode_step_set(0xE0);
        }
        break;
    }
    settings_save_load_load_marks();
}

/* FAKE: the u8 view of the selector keeps the compares on the global ($v1), as in the original; the s32 return keeps the switch temporary copy in $v0. T-5010 */
s32 settings_save_load_load_finish(void) {
    switch (*(u8 *)&game_state.substep) {
    default:
        game_state.substep += 1;
        break;
    case 0:
        if (game_state.year == 0x62 && game_state.month == 3 && game_state.day == 1) {
            game_state.substep += 1;
        } else if (game_state.year == 0x5F && game_state.month == 4 && game_state.day == 4) {
            game_state.substep += 1;
        } else {
            audio_command(0xB1);
            game_state.substep = 0xC0;
        }
        break;
    case 1:
        audio_command(5);
        audio_command(0xB1);
        audio_bank_load_start(0, 0xBF98, 0xBF79, 0xCD21, 0xCCD9, 0xCCD1);
        game_state.substep += 1;
        break;
    case 2:
        if (audio_bank_load_poll() == 1) {
            boot_game_data_init();
            boot_event_state_init();
            game_state.event_log_count = 0;
            parameter_disp_switch(0);
            hizuke_disp_switch(0);
            message_disp_switch(0);
            hud_mascot_display_switch(0);
            audio_bank_start(0, 0);
            game_mode_set(GAME_MODE_NAME_ENTRY);
        }
        break;
    case 0xFF:
        message_window_title_show(0);
        game_mode_set(GAME_MODE_COMMAND_SCREEN);
        mode_stage_set(0xFF);
        hud_day_bgm_load();
        break;
    }
}

void settings_mode_stage_dispatch(void) {
    switch (*(u8 *)&game_state.mode_step) { /* FAKE: scalar byte view of the switch selector. T-11170 */
    case 0x0:
        settings_root_init();
        break;
    case 0x1:
        settings_root_menu();
        break;
    case 0x10:
        settings_text_speed_init();
        break;
    case 0x11:
        settings_text_speed_menu();
        break;
    case 0x12:
        settings_text_speed_done();
        break;
    case 0x20:
        settings_icon_mode_init();
        break;
    case 0x21:
        settings_icon_mode_menu();
        break;
    case 0x22:
        settings_icon_mode_done();
        break;
    case 0x30:
        settings_mouse_init();
        break;
    case 0x31:
        settings_mouse_menu();
        break;
    case 0x32:
        settings_mouse_done();
        break;
    case 0x33:
        settings_mouse_confirm_init();
        break;
    case 0x34:
        settings_mouse_confirm_menu();
        break;
    case 0x35:
        settings_mouse_confirm_done();
        break;
    case 0x36:
        settings_mouse_click_init();
        break;
    case 0x37:
        settings_mouse_click_menu();
        break;
    case 0x38:
        settings_mouse_click_done();
        break;
    case 0x39:
        settings_mouse_click_speed_init();
        break;
    case 0x3A:
        settings_mouse_click_speed_menu();
        break;
    case 0x3B:
        settings_mouse_click_speed_done();
        break;
    case 0x40:
        settings_menu_mode_init();
        break;
    case 0x41:
        settings_menu_mode_menu();
        break;
    case 0x42:
        settings_menu_mode_done();
        break;
    case 0x50:
        settings_cursor_speed_init();
        break;
    case 0x51:
        settings_cursor_speed_menu();
        break;
    case 0x52:
        settings_cursor_speed_done();
        break;
    case 0x60:
        settings_voice_mode_init();
        break;
    case 0x61:
        settings_voice_mode_menu();
        break;
    case 0x62:
        settings_voice_mode_done();
        break;
    case 0x70:
        settings_mascot_init();
        break;
    case 0x71:
        settings_mascot_menu();
        break;
    case 0x72:
        settings_mascot_done();
        break;
    case 0xFF:
        settings_exit();
        break;
    }
    message_window_title_update();
    cal_base_show();
    message_window_show();
    parameter_show();
    hizuke_show();
    hud_cursor_update();
}

s32 settings_root_init(void) {
    s32 i;
    s32 unlock_check_count;
    s16 x[8];
    s16 y[8];
    s16 w[8];
    s16 h[8];

    text_system_reset();
    message_window_set_title("システム設定画面");
    icon_disp_switch(0);
    data_save_load_class_init();
    for (i = 0; i < 8; i++) {
        x[i] = -0x7C;
        y[i] = -0x40 + i * 0xC;
        w[i] = 0x60;
        h[i] = 0xC;
    }
    settings_labels_draw(-1);
    unlock_check_count = settings_unlocked_mascot_count();
    if (unlock_check_count == 0) {
        w[6] = 0;
    }
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "システム設定を変更します", 0);
    menu_set(0, 8, x, y, w, h);
    game_state.menu_cursor_0 = (s8)game_state.menu_cursor_0 % 8;
    k_sub_disp_start(0);
    mode_step_next();
}

/* FAKE: the no-op index mask preserves the GameState loop base.
 * T-9560; original indexing expression unknown. */
s32 settings_unlocked_mascot_count(void) {
    typedef struct TailFlags {
        /* 0x00 */ u8 pad00[0x13];
        /* 0x13 */ u8 first;
        /* 0x14 */ u8 flags[12];
    } TailFlags; /* size 0x20; observed byte view */
    TailFlags *tail;
    s32 nonzero_unlock_check_count;
    s32 girl_flag_index;

    nonzero_unlock_check_count = 0;
    if (((TailFlags *)D_800E7D11)->first != 0) {
        nonzero_unlock_check_count = 1;
    }
    if (game_state.girl_cleared[0] != 0) {
        nonzero_unlock_check_count++;
    }
    tail = (TailFlags *)D_800E7D11;
    for (girl_flag_index = 0; girl_flag_index < 12; girl_flag_index++) {
        if (tail->flags[girl_flag_index] != 0) {
            nonzero_unlock_check_count++;
        }
        if (game_state.girl_cleared[(girl_flag_index + 1) & 0xFFFFFFFF] != 0) {
            nonzero_unlock_check_count++;
        }
    }
    return nonzero_unlock_check_count;
}

s32 settings_labels_draw(s32 highlighted_setting_item) {
    s32 i;
    s16 x[8];
    s16 y[8];
    s16 w[8];
    s16 h[8];
    u8 colour[8];

    for (i = 0; i < 8; i++) {
        x[i] = -0x7C;
        y[i] = -0x40 + i * 0xC;
        w[i] = 0x60;
        h[i] = 0xC;
        if (i == highlighted_setting_item) {
            colour[i] = 4;
        } else {
            colour[i] = 3;
        }
    }
    set_kanji_string(x[0], y[0], colour[0], "文字表示速度", 0);
    set_kanji_string(x[1], y[1], colour[1], "アイコンモード", 0);
    set_kanji_string(x[2], y[2], colour[2], "マウス設定", 0);
    set_kanji_string(x[3], y[3], colour[3], "メニューモード", 0);
    set_kanji_string(x[4], y[4], colour[4], "カーソル速度", 0);
    set_kanji_string(x[5], y[5], colour[5], "音声モード", 0);
    if (settings_unlocked_mascot_count() != 0) {
        set_kanji_string(x[6], y[6], colour[6], "マスコット", 0);
    }
    set_kanji_string(x[7], y[7], colour[7], "設定終了", 0);
}

void settings_root_menu(void) {
    menu_check(0, cursor_x, cursor_y);
    menu_bar_show(0);
    menu_cursor_update(0);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_stage_set(0);
        mode_step_set(2);
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_0) {
        case 0:
            mode_step_set(0x10);
            break;
        case 1:
            mode_step_set(0x20);
            break;
        case 2:
            if (game_state.input_device == INPUT_MOUSE) {
                mode_step_set(0x30);
            }
            break;
        case 3:
            mode_step_set(0x40);
            break;
        case 4:
            mode_step_set(0x50);
            break;
        case 5:
            mode_step_set(0x60);
            break;
        case 6:
            mode_step_set(0x70);
            break;
        case 7:
            mode_step_set(0xFF);
            break;
        }
    }
}

s32 settings_text_speed_init(void) {
    s32 i = 0;
    s32 text_speed;
    s16 x[3];
    s16 y[3];
    s16 w[3];
    s16 h[3];

    text_system_reset();
    message_window_set_title("文字速度");
    settings_labels_draw(0);
    if (i) {} /* FAKE: empty test splits the live range of i. T-12030 */
    for (i = 0; i < 3; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3U, "遅い", 0);
    set_kanji_string(x[1], y[1], 3U, "普通", 0);
    set_kanji_string(x[2], y[2], 3U, "速い", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0U, "文字出力のスピードを変更します", 0);
    menu_set(1, 3, x, y, w, h);
    text_speed = get_k_speed();
    switch (text_speed) {
    default:
    case 0:
        game_state.menu_cursor_1 = 2;
        break;
    case 1:
        game_state.menu_cursor_1 = 1;
        break;
    case 3:
        game_state.menu_cursor_1 = 0;
        break;
    }
    k_sub_disp_start(0);
    mode_step_next();
}

void settings_text_speed_menu(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_1) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "文字出力のスピードを遅くしました", 0);
            k_speed_set(TEXT_SPEED_SLOW);
            mode_step_next();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "文字出力のスピードを普通にしました", 0);
            k_speed_set(TEXT_SPEED_NORMAL);
            mode_step_next();
            return;
        case 2:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "文字出力のスピードを速くしました", 0);
            k_speed_set(TEXT_SPEED_FAST);
            mode_step_next();
            break;
        }
    }
}

void settings_text_speed_done(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0);
        game_state.menu_cursor_0 = 0;
    }
}

void settings_icon_mode_init(void) {
    u8 icon_mode;
    s32 i;
    s16 x[4];
    s16 y[4];
    s16 w[4];
    s16 h[4];

    text_system_reset();
    message_window_set_title("アイコンモード");
    settings_labels_draw(1);
    for (i = 0; i < 3; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "ＴＹＰＥ‐Ａ", 0);
    set_kanji_string(x[1], y[1], 3, "ＴＹＰＥ‐Ｂ", 0);
    set_kanji_string(x[2], y[2], 3, "フリー", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "アイコンの位置の設定をします", 0);
    menu_set(1, 3, x, y, w, h);
    icon_mode = game_state.icon_mode;
    switch (icon_mode) {
    default:
    case 0:
        game_state.menu_cursor_1 = 0;
        break;
    case 1:
        game_state.menu_cursor_1 = 1;
        break;
    case 2:
        game_state.menu_cursor_1 = 2;
        break;
    }
    k_sub_disp_start(0);
    mode_step_next();
}

/* the u8 views: the original reads these two bytes unsigned, the struct declares them s8 (T-8080) */
void settings_icon_mode_menu(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_1) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "アイコンモードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "ＴＹＰＥ‐Ａにしました", 0);
            game_state.icon_mode = ICON_MODE_TYPE_A;
            hud_icon_bar_init();
            mode_step_next();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "アイコンモードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "ＴＹＰＥ‐Ｂにしました", 0);
            game_state.icon_mode = ICON_MODE_TYPE_B;
            hud_icon_bar_init();
            mode_step_next();
            return;
        case 2:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "アイコンモードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "フリーにしました", 0);
            if (*(u8 *)&game_state.icon_mode != 2) {
                game_state.icon_count = 0xF;
            }
            game_state.icon_mode = ICON_MODE_FREE;
            hud_icon_layout_apply(*(u8 *)&game_state.icon_count);
            mode_step_next();
            break;
        }
    }
}

void settings_icon_mode_done(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0);
        game_state.menu_cursor_0 = 1;
    }
}

void settings_mouse_init(void) {
    s32 pad[2]; /* FAKE: unused locals above the arrays, the original frame is 8 bytes bigger (T-3330) */
    s16 x[3];
    s16 y[3];
    s16 w[3];
    s16 h[3];
    s32 i;

    text_system_reset();
    message_window_set_title("マウス設定");
    settings_labels_draw(2);
    for (i = 0; i < 3; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "決定ボタン", 0);
    set_kanji_string(x[1], y[1], 3, "クリックモード", 0);
    set_kanji_string(x[2], y[2], 3, "クリックスピード", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "マウス使用時の各種設定を選択します", 0);
    menu_set(1, 3, x, y, w, h);
    game_state.menu_cursor_1 = 0;
    k_sub_disp_start(0);
    mode_step_next();
}

void settings_mouse_menu(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_1) {
        case 0:
            mode_step_set(0x33);
            return;
        case 1:
            mode_step_set(0x36);
            return;
        case 2:
            mode_step_set(0x39);
            break;
        }
    }
}

void settings_mouse_done(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0);
        game_state.menu_cursor_0 = 2;
    }
}

void settings_mouse_confirm_init(void) {
    s32 pad[2]; /* FAKE: unused locals above the arrays, the original frame is 8 bytes bigger (T-3330) */
    s16 x[4];
    s16 y[4];
    s16 w[4];
    s16 h[4];
    s32 i;

    text_system_reset();
    message_window_set_title("決定ボタン設定");
    settings_labels_draw(2);
    for (i = 0; i < 2; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "右ボタンで決定", 0);
    set_kanji_string(x[1], y[1], 3, "左ボタンで決定", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "決定ボタンを選択します", 0);
    menu_set(1, 2, x, y, w, h);
    if (game_state.confirm_button == SETTINGS_CONFIRM_BUTTON_MOUSE_LEFT || game_state.confirm_button != SETTINGS_CONFIRM_BUTTON_MOUSE_RIGHT) {
        game_state.menu_cursor_1 = 1;
    } else {
        game_state.menu_cursor_1 = 0;
    }
    k_sub_disp_start(0);
    mode_step_next();
}

void settings_mouse_confirm_menu(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_1) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "右ボタンで決定にしました", 0);
            game_state.confirm_button = SETTINGS_CONFIRM_BUTTON_MOUSE_RIGHT;
            mode_step_next();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "左ボタンで決定にしました", 0);
            game_state.confirm_button = SETTINGS_CONFIRM_BUTTON_MOUSE_LEFT;
            mode_step_next();
            break;
        }
    }
}

void settings_mouse_confirm_done(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0x30);
    }
}

void settings_mouse_click_init(void) {
    u8 click_mode; /* FAKE: copy of the unsigned byte (the field is s8) kept in $v0 as the original does (T-5010) */
    s32 i;
    s16 x[4];
    s16 y[4];
    s16 w[4];
    s16 h[4];

    text_system_reset();
    message_window_set_title("クリックモード設定");
    settings_labels_draw(2);
    for (i = 0; i < 2; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "シングルクリック", 0);
    set_kanji_string(x[1], y[1], 3, "ダブルクリック", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "クリックモードを設定します", 0);
    menu_set(1, 2, x, y, w, h);
    click_mode = game_state.click_mode;
    switch (click_mode) {
    case 0:
        game_state.menu_cursor_1 = 0;
        break;
    case 1:
        game_state.menu_cursor_1 = 1;
        break;
    }
    k_sub_disp_start(0);
    mode_step_next();
}

void settings_mouse_click_menu(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_1) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "シングルクリックにしました", 0);
            game_state.click_mode = SETTINGS_CLICK_MODE_SINGLE;
            mode_step_next();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "ダブルクリックにしました", 0);
            game_state.click_mode = SETTINGS_CLICK_MODE_DOUBLE;
            mode_step_next();
            break;
        }
    }
}

void settings_mouse_click_done(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0x30);
    }
}

void settings_mouse_click_speed_init(void) {
    s32 pad[2]; /* FAKE: unused locals above the arrays, the original frame is 8 bytes bigger (T-3330) */
    s16 x[4];
    s16 y[4];
    s16 w[4];
    s16 h[4];
    s32 i;

    text_system_reset();
    message_window_set_title("クリックスピード設定");
    settings_labels_draw(2);
    for (i = 0; i < 2; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "初期状態", 0);
    set_kanji_string(x[1], y[1], 3, "変更", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "クリックスピードを設定します", 0);
    set_kanji_string(-0x80, 0x42, 0, "変更する場合、変更にカーソルを", 0);
    set_kanji_string(-0x80, 0x52, 0, "合わせて、ダブルクリックして下さい", 0);
    menu_set(1, 2, x, y, w, h);
    game_state.menu_cursor_1 = 0;
    settings_click_speed_ticks = 0;
    k_sub_disp_start(0);
    mode_step_next();
}

/* FAKE: scalar-byte view lets IDO keep the clamp value in v0 across stores. T-12030. */
void settings_mouse_click_speed_menu(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
    } else if (((game_state.confirm_button != SETTINGS_CONFIRM_BUTTON_MOUSE_LEFT) && (game_state.mouse_buttons & MOUSE_RIGHT)) || ((game_state.confirm_button == 0) && (game_state.mouse_buttons & MOUSE_LEFT))) {
        switch (game_state.menu_cursor_1) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0U, "クリックスピードを初期状態にしました", 0);
            *(u8 *)&game_state.double_click_interval = 0x20;
            mode_step_next();
            break;
        case 1:
            if (settings_click_speed_ticks != 0) {
                *(u8 *)&game_state.double_click_interval = 0xFF - settings_click_speed_ticks;
                if (*(u8 *)&game_state.double_click_interval < 0x10) {
                    *(u8 *)&game_state.double_click_interval = 0x10;
                }
                if (*(u8 *)&game_state.double_click_interval > 0x40) {
                    *(u8 *)&game_state.double_click_interval = 0x40;
                }
                settings_click_speed_ticks = 0;
                k_sub_reset();
                set_kanji_string(-0x80, 0x32, 0U, "クリックスピードを変更しました", 0);
                mode_step_next();
                break;
            }
            settings_click_speed_ticks = 0xFF;
            break;
        }
    }
}

void settings_mouse_click_speed_done(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0x30);
    }
}

void settings_menu_mode_init(void) {
    s32 pad; /* FAKE: unused local, the original frame has 4 more bytes above the arrays (T-3330) */
    s32 i;
    s16 x[4];
    s16 y[4];
    s16 w[4];
    s16 h[4];

    text_system_reset();
    message_window_set_title("メニューモード設定");
    settings_labels_draw(3);
    for (i = 0; i < 4; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3, "１：マウスライク", 0);
    set_kanji_string(x[1], y[1], 3, "２：メニューＡ", 0);
    set_kanji_string(x[2], y[2], 3, "３：メニューＢ", 0);
    set_kanji_string(x[3], y[3], 3, "４：１＋２", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "コントローラでの、メニュー間の", 0);
    set_kanji_string(-0x80, 0x42, 0, "（２択・３択・アイコンなど）", 0);
    set_kanji_string(-0x80, 0x52, 0, "カーソルの動かし方を設定します", 0);
    menu_set(1, 4, x, y, w, h);
    switch (game_state.menu_mode) {
    default:
    case MENU_MODE_A:
        game_state.menu_cursor_1 = 1;
        break;
    case MENU_MODE_MOUSE_LIKE:
        game_state.menu_cursor_1 = 0;
        break;
    case MENU_MODE_B:
        game_state.menu_cursor_1 = 2;
        break;
    case MENU_MODE_MOUSE_PLUS_A:
        game_state.menu_cursor_1 = 3;
        break;
    }
    k_sub_disp_start(0);
    mode_step_next();
}

void settings_menu_mode_menu(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_1) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "メニュー設定を", 0);
            set_kanji_string(-0x56, 0x42, 1, "マウスライク", 0);
            set_kanji_string(-0x80, 0x52, 0, "にしました", 0);
            game_state.menu_mode = MENU_MODE_MOUSE_LIKE;
            mode_step_next();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "メニュー設定を", 0);
            set_kanji_string(-0x56, 0x42, 1, "メニューＡ", 0);
            set_kanji_string(-0x80, 0x52, 0, "にしました", 0);
            game_state.menu_mode = MENU_MODE_A;
            mode_step_next();
            return;
        case 2:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "メニュー設定を", 0);
            set_kanji_string(-0x56, 0x42, 1, "メニューＢ", 0);
            set_kanji_string(-0x80, 0x52, 0, "にしました", 0);
            game_state.menu_mode = MENU_MODE_B;
            mode_step_next();
            return;
        case 3:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "メニュー設定を", 0);
            set_kanji_string(-0x56, 0x42, 1, "マウスライク＋メニューＡ", 0);
            set_kanji_string(-0x80, 0x52, 0, "にしました", 0);
            game_state.menu_mode = MENU_MODE_MOUSE_PLUS_A;
            mode_step_next();
            break;
        }
    }
}

void settings_menu_mode_done(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0);
        game_state.menu_cursor_0 = 3;
    }
}

/* s32 with no return, like the sibling menus (settings_voice_mode_init). */
s32 settings_cursor_speed_init(void) {
    s32 pad[2]; /* FAKE: T-24301, the frame has 8 more bytes above the arrays (as settings_menu_mode_init) */
    s32 i;
    s16 x[5];
    s16 y[5];
    s16 w[5];
    s16 h[5];

    text_system_reset();
    message_window_set_title("カーソル速度");
    settings_labels_draw(4);
    for (i = 0; i < 5; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    if (D_800E7D2C >= 9 || game_state.girl_cleared[8] >= 2) {
        set_kanji_string(x[0], y[0], 3U, "超遅い", 0);
        set_kanji_string(x[1], y[1], 3U, "遅い", 0);
        set_kanji_string(x[2], y[2], 3U, "普通", 0);
        set_kanji_string(x[3], y[3], 3U, "速い", 0);
        set_kanji_string(x[4], y[4], 3U, "超速い", 0);
    } else {
        set_kanji_string(x[0], y[0], 3U, "遅い", 0);
        set_kanji_string(x[1], y[1], 3U, "少し遅い", 0);
        set_kanji_string(x[2], y[2], 3U, "普通", 0);
        set_kanji_string(x[3], y[3], 3U, "少し速い", 0);
        set_kanji_string(x[4], y[4], 3U, "速い", 0);
    }
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0U, "カーソルの移動速度を設定します", 0);
    menu_set(1, 5, x, y, w, h);
    switch (game_state.cursor_speed) {
    default:
    case SETTINGS_CURSOR_SPEED_SLOWEST:
        game_state.menu_cursor_1 = 0;
        break;
    case SETTINGS_CURSOR_SPEED_SLOWER:
        game_state.menu_cursor_1 = 1;
        break;
    case SETTINGS_CURSOR_SPEED_NORMAL:
        game_state.menu_cursor_1 = 2;
        break;
    case SETTINGS_CURSOR_SPEED_FASTER:
        game_state.menu_cursor_1 = 3;
        break;
    case SETTINGS_CURSOR_SPEED_FASTEST:
        game_state.menu_cursor_1 = 4;
        break;
    }
    k_sub_disp_start(0);
    mode_step_next();
}

void settings_cursor_speed_menu(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (*(u8 *)&game_state.menu_cursor_1) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "カーソルの移動速度を", 0);
            if ((D_800E7D2C != 0) || (game_state.girl_cleared[8] != 0)) {
                set_kanji_string(-0x80, 0x42, 0, "超遅くしました", 0);
            } else {
                set_kanji_string(-0x80, 0x42, 0, "遅くしました", 0);
            }
            game_state.cursor_speed = SETTINGS_CURSOR_SPEED_SLOWEST;
            game_state.input_device = INPUT_NONE;
            mode_step_next();
            break;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "カーソルの移動速度を", 0);
            if ((D_800E7D2C != 0) || (game_state.girl_cleared[8] != 0)) {
                set_kanji_string(-0x80, 0x42, 0, "ちょろっと遅くしました", 0);
            } else {
                set_kanji_string(-0x80, 0x42, 0, "少し遅くしました", 0);
            }
            game_state.cursor_speed = SETTINGS_CURSOR_SPEED_SLOWER;
            game_state.input_device = INPUT_NONE;
            mode_step_next();
            break;
        case 2:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "カーソルの移動速度を", 0);
            set_kanji_string(-0x80, 0x42, 0, "ふつうにしました", 0);
            game_state.cursor_speed = SETTINGS_CURSOR_SPEED_NORMAL;
            game_state.input_device = INPUT_NONE;
            mode_step_next();
            break;
        case 3:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "カーソルの移動速度を", 0);
            if ((D_800E7D2C != 0) || (game_state.girl_cleared[8] != 0)) {
                set_kanji_string(-0x80, 0x42, 0, "ちょろっと速くしました", 0);
            } else {
                set_kanji_string(-0x80, 0x42, 0, "少し速くしました", 0);
            }
            game_state.cursor_speed = SETTINGS_CURSOR_SPEED_FASTER;
            game_state.input_device = INPUT_NONE;
            mode_step_next();
            break;
        case 4:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "カーソルの移動速度を", 0);
            if ((D_800E7D2C != 0) || (game_state.girl_cleared[8] != 0)) {
                set_kanji_string(-0x80, 0x42, 0, "超速くしました", 0);
            } else {
                set_kanji_string(-0x80, 0x42, 0, "速くしました", 0);
            }
            game_state.cursor_speed = SETTINGS_CURSOR_SPEED_FASTEST;
            game_state.input_device = INPUT_NONE;
            mode_step_next();
            break;
        }
    }
}

void settings_cursor_speed_done(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0);
        game_state.menu_cursor_0 = 4;
    }
}

s32 settings_voice_mode_init(void) {
    s32 i;
    s32 voice_mode;
    s16 x[3];
    s16 y[3];
    s16 w[3];
    s16 h[3];

    text_system_reset();
    message_window_set_title("音声モード");
    settings_labels_draw(5);
    for (i = 0; i < 3; i++) {
        x[i] = -4;
        y[i] = -0x40 + i * 0x10;
        w[i] = 0x6C;
        h[i] = 0x10;
    }
    set_kanji_string(x[0], y[0], 3U, "常にＯＮ", 0);
    set_kanji_string(x[1], y[1], 3U, "常にＯＦＦ", 0);
    set_kanji_string(x[2], y[2], 3U, "×ボタンでカット", 0);
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0U, "音声のモードを設定します", 0);
    menu_set(1, 3, x, y, w, h);
    voice_mode = (u8)game_state.voice_mode;
    switch (voice_mode) {
    case 1:
        game_state.menu_cursor_1 = 0;
        break;
    case 0:
        game_state.menu_cursor_1 = 1;
        break;
    default:
    case 2:
        game_state.menu_cursor_1 = 2;
        break;
    }
    k_sub_disp_start(0);
    mode_step_next();
}

void settings_voice_mode_menu(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_1) {
        case 0:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "音声モードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "常にＯＮにしました", 0);
            game_state.voice_mode = VOICE_ON;
            mode_step_next();
            return;
        case 1:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "音声モードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "常にＯＦＦにしました", 0);
            game_state.voice_mode = VOICE_OFF;
            mode_step_next();
            return;
        case 2:
            k_sub_reset();
            set_kanji_string(-0x80, 0x32, 0, "音声モードを", 0);
            set_kanji_string(-0x80, 0x42, 0, "×ボタンでカットにしました", 0);
            game_state.voice_mode = VOICE_CANCELABLE;
            mode_step_next();
            break;
        }
    }
}

void settings_voice_mode_done(void) {
    k_disp_inc();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0);
        game_state.menu_cursor_0 = 5;
    }
}

void settings_exit(void) {
    mode_stage_set(0);
    mode_step_set(2);
}

void settings_mascot_init(void) {
    s32 i;
    s32 d;
    s32 pad; /* FAKE: unused local reserves the third home word above the arrays (frame 0xE8); real source unknown. T-17005 */
    s16 x[14];
    s16 y[14];
    s16 w[14];
    s16 h[14];

    text_system_reset();
    message_window_set_title("マスコット");
    settings_labels_draw(6);
    d = 3;
    for (i = 0; i < 14; i++) {
        x[i] = (i % d) * 0x2B - 8;
        y[i] = (i / d) * 0x10 - 0x40;
        w[i] = 0x28;
        h[i] = 0x10;
        if (i == 13) {
            w[i] = 0x46;
            set_kanji_string(x[i], y[i], 5, "表示しない", 0);
        } else if (system_data.flags[i] != 0 || game_state.girl_cleared[i] != 0) {
            set_kanji_string(x[i], y[i], 3, girl_short_name_table[i], 0);
        } else {
            w[i] = 0;
        }
    }
    k_sub_reset_point_set();
    set_kanji_string(-0x80, 0x32, 0, "マスコットを選びます", 0);
    menu_set(1, 14, x, y, w, h);
    if (hud_mascot_choice == 0 || hud_mascot_choice >= 14) {
        game_state.menu_cursor_1 = 0;
    } else {
        game_state.menu_cursor_1 = hud_mascot_choice;
    }
    k_sub_disp_start(0);
    mode_step_next();
}

s32 settings_mascot_menu(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    k_disp_inc();
    if (game_state.menu_cursor_1 != -1) {
        hud_mascot_set(0);
        if (system_data.flags[game_state.menu_cursor_1] != 0 || game_state.girl_cleared[game_state.menu_cursor_1] != 0) {
            hud_mascot_set(game_state.menu_cursor_1 + 1);
        }
    }
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
    } else if ((game_state.pad_pressed & PAD_CIRCLE) && (game_state.menu_cursor_1 != -1)) {
        if (game_state.menu_cursor_1 == 0xD) {
            mode_step_set(0);
        } else if ((system_data.flags[game_state.menu_cursor_1] != 0) || (game_state.girl_cleared[game_state.menu_cursor_1] != 0)) {
            hud_mascot_set(0);
            hud_mascot_set(game_state.menu_cursor_1 + 1);
            mode_step_next();
        }
    }
}

void settings_mascot_done(void) {
    mode_step_set(0);
    game_state.menu_cursor_0 = 6;
}

/* Runs the club-joining screen steps and refreshes its HUD. */
void join_club(void) {
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
        join_club_exit();
        break;
    }
    cal_base_show();
    message_window_show();
    parameter_show();
    hizuke_show();
    hud_cursor_update();
}

/* Initializes the club list and club-joining screen. */
void join_club_init(void) {
    message_window_title_show(0);
    icon_disp_switch(0);
    text_system_reset();
    hud_club_list_menu();
    mode_step_next();
}

/* Handles club-list navigation and confirms the selected club. */
s32 join_club_select(void) {
    menu_check(1, (s32) cursor_x, (s32) cursor_y);
    menu_cursor_update(1);
    menu_bar_show(1);
    if (game_state.pad_pressed & PAD_CROSS) {
        mode_stage_set(0);
        if (game_state.game_mode == 0x34) {
            mode_step_set(2);
        }
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if (game_state.menu_cursor_1 == 0xB) {
            mode_stage_set(0);
            if (game_state.game_mode == 0x34) {
                mode_step_set(2);
            }
        } else if (game_state.menu_cursor_1 != -1) {
            ((NibbleView *)&game_state.player_flags.b[1])->high = game_state.menu_cursor_1;
            mode_step_next();
        }
    }
}

/* FAKE: scalar-byte selector view keeps the switch index in v1; s32 fall-through
 * preserves its v0 copy. T-12030. */
/* Displays the club enrollment message and the recurring club-day notice for club IDs 6 and above. */
s32 join_club_message(void) {
    u8 buf[0x44];

    switch (*(u8 *)&game_state.substep) {
    case 0:
        k_sub_reset();
        strcpy(buf, club_name_table[game_state.player_flags.h >> 0xC]);
        strcat(buf, "部に入部した。");
        sndisp(buf, 0, 0xFF);
        k_sub_disp_start(1);
        game_state.substep++;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        if ((game_state.player_flags.h >> 0xC) >= 6) {
            k_sub_reset();
            gnsx("部長");
            set_kanji_string(-0x56, 0x32, 0, "「毎月", 0);
            set_kanji_string(-0x2C, 0x32, 1, "第３日曜日", 0);
            set_kanji_string(0x1A, 0x32, 0, "は、", 0);
            set_kanji_string(-0x48, 0x42, 0, "クラブの日なので、", 0);
            set_kanji_string(-0x48, 0x52, 1, "休まない", 0);
            set_kanji_string(-0x10, 0x52, 0, "ように。", 0);
            talk_voice_play(0x0D000011, 0x0300001F, -1, 0xE);
            game_state.substep++;
        } else {
            game_state.substep = 0xFF;
        }
        break;
    case 3:
        talk_text_step();
        break;
    case 4:
        game_state.substep = 0xFF;
        break;
    case 0xFF:
        mode_step_next();
        break;
    }
}

/* Closes the club-joining screen and returns to the schedule. */
void join_club_exit(void) {
    actor_clear_upper();
    game_mode_set(GAME_MODE_SCHEDULE);
}

void rumor_scene_init(void) {
    s32 r;

    text_system_reset();
    message_window_set_title("自宅");
    gnsx(game_state.player_family_name);
    parameter_show_init();
    hizuke_init();
    message_window_init();
    parameter_disp_switch(1);
    hud_cursor_actor_init(0);
    hud_icon_bar_init();
    icon_disp_switch(1);
    schedule_command_icons_lock_to_record();
    icon_can_use_set(0, 0);
    icon_can_use_set(game_state.last_command.w & 0xF, 1);
    tpage_buf_clear();
    sndisp("最近女の子の間で 変な噂が流れているらしい…）", 0, 0x1F);
    k_disp_start(1);
    r = dec_bg_cd_read(D_800B5950[hud_season_index()], 0);
    if (r != -1) {
        if (r == 0 || r == 1) {
            dec_bg_show_set(0, r);
            set_dec_bri(0x80);
            mode_stage_next();
        }
    } else {
        mode_step_next();
    }
    k_disp_start(2);
}

void rumor_bg_wait(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(0, dec_bg_decode_to_vram(0));
        if (bg_decode_result == -1) {
            dec_bg_reset();
            dec_bg_cd_read(D_800B5950[hud_season_index()], 0);
        } else {
            set_dec_bri(0x80);
            game_state.screen_brightness = 0x80;
            mode_stage_set(1);
        }
    }
}

s32 rumor_setup_mode_stage_dispatch(void) {
    switch (game_state.mode_step) {
    case 0:
        rumor_scene_init();
        break;
    case 1:
        rumor_bg_wait();
        break;
    }
}

void rumor_wait_voice(void) {
    switch (game_state.substep) {
    case 0:
        xa_wait();
        return;
    default:
    case 1:
        mode_step_next();
        return;
    }
}
/* Returns from the rumor report to the day screen. */
void uwasa_exit0(void) {
    game_mode_set(GAME_MODE_DAY_SCREEN);
}

/* Advances the rumor report through voice playback and exit. */
s32 uwasa0(void) {
    switch (game_state.mode_step) {
    case 0:
        rumor_wait_voice();
        break;
    case 1:
        uwasa_exit0();
        break;
    }
}

/* Runs the rumor report setup and display stages. */
void uwasa_main(void) {
    switch (game_state.mode_stage) {
    case 0:
        rumor_setup_mode_stage_dispatch();
        break;
    case 1:
        uwasa0();
        break;
    }
    hud_icon_bar_update(1);
    parameter_show();
    hizuke_show();
    message_window_show();
    message_window_title_update();
    dec_bg_draw(0);
    hud_screen_frame_draw(1);
}
/* Initializes the Christmas preparation scene and its room background. */
s32 pre_xmas_init(void) {
    switch (game_state.mode_step) {
    case 0:
        text_system_reset();
        actor_clear_upper();
        sprite_tables_clear();
        message_window_set_title_hidden("自宅");
        gnsx(game_state.player_family_name);
        parameter_show_init();
        parameter_show_init();
        hizuke_init();
        message_window_init();
        parameter_disp_switch(1);
        icon_disp_switch(0);
        tpage_buf_clear();
        sndisp("今日は伊集院の家で、 クリスマスパーティだ。 会場に行こうかな）", 0, 0x1F);
        k_disp_start(6);
        hud_cursor_actor_init(0);
        settings_room_bg_load_start();
        return;
    case 1:
        settings_room_bg_load_finish();
        return;
    }
}

/* Waits for XA completion, then selects GAME_MODE_NEW_YEAR_A. */
s32 pre_xmas(void) {
    switch (game_state.substep) {
    case 0:
        xa_wait();
        break;
    case 1:
        game_mode_set(GAME_MODE_NEW_YEAR_A);
        break;
    }
}

void settings_christmas_prep_mode_handler(void) {
    switch (game_state.mode_stage) {
    case 0:
        pre_xmas_init();
        break;
    case 1:
        pre_xmas();
        break;
    }
    parameter_show();
    hizuke_show();
    message_window_show();
    hud_cursor_update();
    message_window_title_update();
    if (*D_8011F3FF & 0x80) {
        hud_mascot_update();
    }
    dec_bg_draw(0);
    hud_screen_frame_draw(1);
}

s32 settings_new_year_card_sender_count(void) {
    s32 pad[2]; /* FAKE: unused, gives the 0x48 frame and the sender_count/weighted_affection slots. T-7020 */
    u8 weighted_affection[12];
    s32 i;
    s32 sender_count;

    sender_count = 0;
    get_h_yuukou_table(weighted_affection);
    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 0; i < 11; i++) if (weighted_affection[i] >= 0x4DU && game_state.girls[i].flags.f.met) sender_count++;
    return sender_count;
}

void settings_new_year_card_scene_start(void) {
    if (settings_new_year_card_sender_count() != 0) {
        set_dec_bri(0);
        text_system_reset();
        bg_capture_screen(0xA0);
        actor_clear_upper();
        sprite_tables_clear();
        hizuke_init();
        message_window_init();
        parameter_disp_switch(0);
        hud_mascot_display_switch(0);
        audio_command(0xC1);
        icon_disp_switch(0);
        tpage_buf_clear();
        hud_cursor_actor_init(0);
        audio_bank_load_start(1, 0xC4E5, 0xC4C7, 0xD294, 0xD264, 0xD25E);
        mode_step_next();
        return;
    }
    set_dec_bri(0);
    game_mode_set(GAME_MODE_NEW_YEAR_B);
}

/* Loads or waits for the New Year preparation background. */
void pre_syogatu_init1(void) {
    s32 t;

    if (audio_bank_load_poll() == 1) {
        t = dec_bg_cd_read(0x4167, 1);
        if (t != -1) {
            if (t == 0 || t == 1) {
                dec_bg_show_set(1, t);
                set_dec_bri(0x80);
                mode_step_next();
                mode_step_next();
            }
        } else {
            mode_step_next();
        }
    }
    bg_wipe_step();
}

/* Finishes loading and installs the New Year preparation background. */
void pre_syogatu_init2(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        set_dec_bri(0x80);
        mode_step_next();
    }
}

INCLUDE_RODATA("asm/data/main/home_screens.rodata", D_800B0B64);

INCLUDE_RODATA("asm/data/main/home_screens.rodata", D_800B0B6C);

/* Starts the New Year preparation message and advances its stage. */
void pre_syogatu_init3(void) {
    text_system_reset();
    message_window_set_title(D_800B0B64);
    k_sub_reset_point_set();
    sndisp(D_800B0B6C, 0, 0xFF);
    audio_command(0x603);
    k_disp_start(6);
    mode_stage_next();
}

/* Runs the New Year preparation scene setup steps. */
s32 pre_syogatu_init(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        settings_new_year_card_scene_start();
        break;
    case 1:
        pre_syogatu_init1();
        break;
    case 2:
        pre_syogatu_init2();
        break;
    case 3:
        pre_syogatu_init3();
        break;
    }
}

/* Runs the New Year card-delivery message and background transition. */
void pre_syogatu0(void) {
    s32 temp_v0;

    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp((s32) "ん？ 誰だろう）", 0, 0x1F);
        game_state.substep += 1;
    default:
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        k_sub_reset();
        gnsx("〒配達");
        sndisp((s32) "年賀状でーす。", 0, 0xE);
        k_sub_disp_start(1);
        talk_voice_play(0x08000005, 0x03000009, -1, 0xE);
        game_state.substep += 1;
        break;
    case 3:
        talk_text_step();
        break;
    case 4:
        temp_v0 = dec_bg_cd_read(0x4144, 1);
        if (temp_v0 != -1) {
            if ((temp_v0 == 0) || (temp_v0 == 1)) {
                dec_bg_show_set(1, temp_v0);
                set_dec_bri(0x80U);
                game_state.substep = 6;
            }
        } else {
            game_state.substep = 5;
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            set_dec_bri(0x80U);
            game_state.substep += 1;
        }
        break;
    case 6:
        mode_step_next();
        break;
    }
}

/* Displays the New Year card sender list and enters the next mode. */
s32 pre_syogatu1(void) {
    s32 girl_index;
    s32 sender_count;
    /* FAKE: two words recover the observed frame without moving the buffers. T-17005 */
    s32 stack_home[2];
    u8 weighted_affection[0xC];
    u8 sender_text[0x100];
    u8 girl_name_buffer[0x10];

    switch (game_state.substep) {
    case 0:
        get_h_yuukou_table(weighted_affection);
        k_sub_reset();
        strcpy(sender_text, "やった！ ");
        sender_count = 0;
        for (girl_index = 0; girl_index < 0xB; girl_index++) {
            if ((weighted_affection[girl_index] >= 0x4DU) && (game_state.girls[girl_index].flags.f.met)) {
                if (sender_count != 0) {
                    strcat(sender_text, "と、 ");
                }
                get_g_name(girl_name_buffer, girl_index);
                strcat(sender_text, girl_name_buffer);
                sender_count += 1;
            }
        }
        strcat(sender_text, "から、 年賀状をもらったラッキー。）");
        gnsx(game_state.player_family_name);
        sndisp(sender_text, 0, 0x1F);
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        game_mode_set(GAME_MODE_NEW_YEAR_B);
        return;
    }
}

s32 settings_new_year_scene_mode_stage_dispatch(void) {
    switch (game_state.mode_step) {
    case 0:
        pre_syogatu0();
        break;
    case 1:
        pre_syogatu1();
        break;
    }
}

void settings_new_year_prep_mode_handler(void) {
    switch (game_state.mode_stage) {
    case 0:
        pre_syogatu_init();
        break;
    case 1:
        settings_new_year_scene_mode_stage_dispatch();
        break;
    }
    hizuke_show();
    message_window_show();
    hud_cursor_update();
    message_window_title_update();
    dec_bg_draw(1);
    hud_screen_frame_draw(1);
}
