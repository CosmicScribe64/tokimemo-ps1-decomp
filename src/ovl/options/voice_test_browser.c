/* voice_test_browser.c: src/ovl/OPTION/801389A0.c:432: voice test menu, character/clip selection, list display and audio completion replay; body 443: if (game_state.pad_pressed & 0x08000000) {; 444: options_voice_test_repeat_enabled = 1 - options_voice_test_repeat_enabled;. Original object 0x801389A0-0x8013A550 (config/objects/OPTION.txt). */
#include "common.h"
#include "ovl/options.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", D_8013D360);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", D_8013D364);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", D_8013D368);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", D_8013D36C);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", D_8013D370);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", D_8013D374);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", D_8013D378);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", D_8013D37C);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", D_8013D380);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", D_8013D39C);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_voice_test_music_clip_index);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_voice_test_character_clip_index);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_voice_test_bank_start_argument);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_voice_bank_load_step);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_music_bank_load_step);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_debug_audio_track);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", D_8013D3BC);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", D_8013D3CC);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_voice_test_character_index);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", D_8013D3E0);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_voice_test_repeat_enabled);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", D_8013D3F4);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_voice_test_character_labels);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_voice_test_character_bank_load_commands);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_voice_test_character_bank_load_arguments);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_voice_test_music_clip_labels);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_voice_test_character_clip_labels);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_voice_test_music_bank_load_arg_1);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_voice_test_music_bank_load_arg_2);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_voice_test_music_bank_load_arg_3);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_voice_test_character_bank_load_arg_1);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_voice_test_character_bank_load_arg_2);
INCLUDE_RODATA("asm/ovl/OPTION/data/options/voice_test_browser.data", options_voice_test_character_bank_load_arg_3);
s32 options_debug_audio_bank = 0;
s32 D_8013E734 = 0;
s32 D_8013E738 = 0;
s32 D_8013E73C = 0;

void options_debug_audio_menu_handler(void) {
    game_state.mode_stage_timer += 1;
    switch (game_state.mode_stage) {
    case 0:
        options_debug_audio_background_load();
        break;
    case 1:
        options_debug_audio_step_dispatch();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    hud_screen_frame_draw(0);
    message_window_title_update();
}

void options_debug_audio_background_load(void) {
    if (game_state.substep == 0) {
        cd_read_sectors(0x3A, 0x80180000, 0xA1AB);
        game_state.substep += 1;
    } else if (cd_read_sync_status() & 1) {
        mode_stage_next();
    }
}

void options_debug_audio_step_dispatch(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        options_debug_audio_menu_init();
        return;
    case 1:
        options_voice_test_fade_in();
        return;
    case 2:
        options_debug_audio_menu_update();
        return;
    case 3:
        options_debug_audio_track_list_page();
        return;
    case 4:
        options_debug_audio_second_track_list_page();
        return;
    case 5:
        options_debug_audio_voice_list_page();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void options_voice_test_character_rows_initialize(s32 arg0) {
    s32 i;
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];

    for (i = 0; i < 16; i++) {
        a[i] = 0;
        b[i] = i * 10 - 0x64;
        c[i] = 0x40;
        d[i] = 0xA;
        menu_set(1, 0x10, a, b, c, d);
    }
}

void options_debug_audio_track_list_page(void) {
    s32 i;
    s32 y;
    s32 next;
    u8 (*rows)[16];

    if (game_state.substep == 0) {
        options_voice_test_character_rows_initialize(0);
        i = 0;
        do {
            next = i + 1;
            k_disp_switch(next, 0);
            i++;
        } while (i != 7);
        game_state.substep += 1;
    } else {
        if (game_state.pad_pressed & PAD_CIRCLE) {
            if (game_state.menu_cursor_1 != -1) {
                options_voice_test_character_clip_index = game_state.menu_cursor_1 + options_voice_test_character_clip_index;
            }
            options_debug_audio_voice_load_begin();
            i = 0;
            do {
                next = i + 1;
                k_disp_switch(next, 1);
                i++;
            } while (i != 7);
            mode_step_set(2);
        }
    }
    i = 0;
    y = -0x64;
    do {
        debug_print_int_color((s32) ((u32) (options_voice_test_character_clip_index + i) % 63U), -0xA, y, 6);
        i += 1;
        y += 0xA;
    } while (i < 0x10);
    rows = options_voice_test_character_clip_labels;
    for (i = 0, y = -0x64; i != 0x10; i++, y += 0xA) {
        debug_print_string((s32) rows[((u32) (options_voice_test_character_clip_index + i) % 63U)], 0xA, y);
    }
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    menu_bar_show(0);
    options_voice_test_selection_gradient_draw();
    options_voice_test_background_gradient_draw();
}

void options_debug_audio_voice_list_page(void) {
    s32 i;
    s32 y;
    u8 (*rows)[16];

    if (game_state.substep == 0) {
        options_voice_test_character_rows_initialize(0);
        for (i = 0; i < 7; i++) {
            k_disp_switch(i + 1, 0);
        }
        game_state.substep += 1;
    } else {
        if (game_state.pad_pressed & PAD_CIRCLE) {
            if (game_state.menu_cursor_1 != -1) {
                options_voice_test_character_index = game_state.menu_cursor_1 + options_voice_test_character_index;
            }
            for (i = 0; i < 7; i++) {
                k_disp_switch(i + 1, 1);
            }
            mode_step_set(2);
        }
    }
    i = 0;
    y = -0x64;
    do {
        debug_print_int_color((s32) ((u32) (options_voice_test_character_index + i) % 58U), -0xA, y, 6);
        i += 1;
        y += 0xA;
    } while (i < 0x10);
    rows = options_voice_test_character_labels;
    for (i = 0, y = -0x64; i != 0x10; i++, y += 0xA) {
        debug_print_string((s32) rows[((u32) (options_voice_test_character_index + i) % 58U)], 0xA, y);
    }
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    menu_bar_show(0);
    options_voice_test_selection_gradient_draw();
    options_voice_test_background_gradient_draw();
}

void options_debug_audio_second_track_list_page(void) {
    s32 i;
    s32 y;
    s32 next;
    u8 (*rows)[16];

    if (game_state.substep == 0) {
        options_voice_test_character_rows_initialize(1);
        i = 0;
        do {
            next = i + 1;
            k_disp_switch(next, 0);
            i++;
        } while (i != 7);
        game_state.substep += 1;
    } else {
        if (game_state.pad_pressed & PAD_CIRCLE) {
            if (game_state.menu_cursor_1 != -1) {
                options_voice_test_music_clip_index = game_state.menu_cursor_1 + options_voice_test_music_clip_index;
            }
            options_debug_audio_music_load_begin();
            i = 0;
            do {
                next = i + 1;
                k_disp_switch(next, 1);
                i++;
            } while (i != 7);
            mode_step_set(2);
        }
    }
    i = 0;
    y = -0x64;
    do {
        debug_print_int_color((s32) ((u32) (options_voice_test_music_clip_index + i) % 63U), -0xA, y, 6);
        i += 1;
        y += 0xA;
    } while (i < 0x10);
    rows = options_voice_test_music_clip_labels;
    for (i = 0, y = -0x64; i != 0x10; i++, y += 0xA) {
        debug_print_string((s32) rows[((u32) (options_voice_test_music_clip_index + i) % 63U)], 0xA, y);
    }
    menu_check(1, cursor_x, cursor_y);
    menu_bar_show(1);
    menu_cursor_update(1);
    menu_bar_show(0);
    options_voice_test_selection_gradient_draw();
    options_voice_test_background_gradient_draw();
}

s32 options_debug_audio_menu_init(void) {
    s32 i;
    Actor *p;

    debug_hud_level_set(4);
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(0);
    tpage_buf_clear();
    game_state.tile_layer_enabled = 0;
    *(s32 *)&game_state.menu_hit[3] = 0xA0;
    text_system_reset();
    message_window_set_title((s32) "オプション画面");
    k_disp_start(1);
    set_kanji_string(D_8013D364[0], D_8013D372[0], 0, "ＸＡコール", 0);
    set_kanji_string(D_8013D364[1], D_8013D372[1], 0, "メインＶＡＢトランス", 0);
    set_kanji_string(D_8013D364[2], D_8013D372[2], 0, "サブＶＡＢトランス", 0);
    set_kanji_string(D_8013D364[3], D_8013D372[3], 0, "メインＳＥＱコール", 0);
    set_kanji_string(D_8013D364[4], D_8013D372[4], 0, "サブＳＥＱコール", 0);
    set_kanji_string(D_8013D364[5], D_8013D372[5], 0, "メインＳＥコール", 0);
    set_kanji_string(D_8013D364[6], D_8013D372[6], 3, "ＳＥＱーＮＵＭ", 0);
    game_state.menu_cursor_0 = 0;
    game_state.menu_cursor_1 = 0;
    game_state.menu_mode = MENU_MODE_MOUSE_LIKE;
    hud_cursor_actor_init(0);
    ((Actor *)actor_pool)[0].draw_flags |= 1;
    options_voice_bank_load_step = 0;
    options_music_bank_load_step = 0;
    D_801220E2 = 0;
    D_801220E1 = 0;
    D_801220E0 = 0;
    /* FAKE: argument evaluation delays the flag clear until after row-address
     * setup, preserving the original store schedule. T-24002 */
    menu_set(0, 7, (game_state.screen_brightness = 0, D_8013D364), D_8013D372, D_8013D380, D_8013D38E);
    menu_bar_color(0, 0x5F7F3F, 9, 8);
    menu_bar_color(1, 0x7F7F7F, 2, 1);
    for (i = 0, p = (Actor *)actor_pool; i < 13; i++, p++) {
        actor_clear(i + 0x60);
        p[96].unk_01 = 2;
        p[96].flags = 0x40;
        p[96].quad_flags = 0x40000000;
        p[96].draw_flags = 0x94;
        p[96].frame_table = D_800C5FD4;
        p[96].sequence_table = D_800C6010;
        p[96].tpage_table = &D_800C5FCC;
        p[96].unk_14 = D_800C6014;
        p[96].unk_04 = 8;
        p[96].unk_05 = 1;
        p[96].sequence_slot = 0;
        p[96].unk_18 = i;
        p[96].dx = 3 - (rand() & 7);
        p[96].dy = 4 - (rand() & 7);
        p[96].dx_fraction = rand();
        p[96].dy_fraction = rand();
        p[96].x = 0;
        p[96].y = 0;
    }
    mode_step_next();
}

/* FAKE: s32 without an explicit return keeps $v0 live at epilogue so as1 retains move $v0, $zero in the third loop preheader. T-24126 */
s32 options_voice_test_actor_selection_update(void) {
    s32 i;
    Actor *p;
    s16 temp_v0;
    s32 dx;
    s32 dy;
    s32 count;
    u8 flags;

    count = 0;
    for (i = 0, p = (Actor *)actor_pool; i < 13; i++, p++) {
        dx = abs((((Actor *)actor_pool)->x - p[96].x) & 0x3FF);
        debug_print_int_color(abs((((Actor *)actor_pool)->y - p[96].y) & 0x1FF) + dx, -0x8C, i * 8 - 0x64, 6);
        dx = abs((((Actor *)actor_pool)->x - p[96].x) & 0x3FF);
        if (((abs((((Actor *)actor_pool)->y - p[96].y) & 0x1FF) + dx) < 0x20) && (game_state.pad_pressed & PAD_CIRCLE)) {
            flags = p[96].draw_flags;
            if (flags & 0x10) {
                p[96].draw_flags = flags & 0xFFEF;
                if (p[96].x < -0x200) {
                    p[96].x = 0x200;
                }
                if (p[96].x >= 0x201) {
                    p[96].x = -0x200;
                }
                if (p[96].y < -0x200) {
                    p[96].y = 0x200;
                }
                if (p[96].y >= 0x201) {
                    p[96].y = -0x200;
                }
            } else {
                p[96].draw_flags = flags | 0x10;
            }
        }
        if (!(p[96].draw_flags & 0x10)) {
            temp_v0 = p[96].unk_08;
            p[96].unk_08 = temp_v0 + 1;
            ((ActorFixed *)p)[96].y.w += rsin(temp_v0 << 6) * 4;
            count += 1;
        }
    }
    debug_print_int_color(count, 0, -0x64, 6);
    if (count == 13) {
        for (i = 0, p = (Actor *)actor_pool; i < 13; i++, p++) {
            p[96].x = i * 0x20 - 0x96;
            p[96].y = -0x64;
        }
    } else if (count >= 5) {
        /* FAKE: i = 0; p = ...; do { on one line with &actor_pool[i + 13] folds D_8011F044 into $s2 before 40 ($s3). T-24126 */
        i = 0; p = (Actor *)actor_pool; do {
            if (!(p[96].draw_flags & 0x10)) {
                dx = p[96].x - (rsin(p[96].unk_08) / 32);
                dy = p[96].y - (func_800A0140(p[96].unk_08) / 40);
                if ((dx < -0x10) || (dx >= 0x11)) {
                    if (dx > 0) {
                        p[96].x--;
                    } else {
                        p[96].x++;
                    }
                }
                if ((dy < -0x10) || (dy >= 0x11)) {
                    if (dy > 0) {
                        p[96].y--;
                    } else {
                        p[96].y++;
                    }
                }
            }
            p++;
        } while (p != &((Actor *)actor_pool)[i + 13]);
    }
}

void options_voice_test_fade_in(void) {
    u8 t;

    *(u8 *)&game_state.screen_brightness += 4;
    t = *(u8 *)&game_state.screen_brightness;
    D_801220E2 = t;
    D_801220E1 = t;
    D_801220E0 = t;
    D_801220E8 = 0xA0;
    D_801220EA = 0x78;
    /* FAKE: empty test separates raw and truncated byte live ranges. T-10850 */
    if (t) {
    }
    D_801220D4 = 0;
    D_801220D6 = 0;
    if (t >= 0x80) {
        *(u8 *)&game_state.screen_brightness = 0x80;
        k_disp_start(7);
        mode_step_next();
    }
}

void options_debug_audio_menu_update(void) {
    D_801220EC = rsin(game_state.mode_step_timer.w << 5);
    D_801220EE = rsin(game_state.mode_step_timer.w << 5);
    D_801220F0 = game_state.mode_step_timer.w << 0xC;
    menu_check(0, cursor_x, cursor_y);
    menu_bar_show(0);
    menu_cursor_update(0);
    options_voice_test_background_gradient_draw();
    options_debug_audio_input_handle();
    k_disp_inc();
    options_voice_test_actor_selection_update();
    options_debug_audio_name_display_toggle();
}

void options_debug_audio_name_display_toggle(void) {
    if (game_state.pad_pressed & PAD_START) {
        D_8013D3F4 += 1;
        audio_reverb_off_toggle();
    }
    debug_print_string("reverve", 0x78, 0);
    debug_print_hex(D_8013D3F4 & 1, 0x78, 0xA);
}

void options_debug_audio_input_handle(void) {
    u32 repeat_selector;
    u32 press_selector;
    u32 decrease_selector;
    s32 decreased;
    /* FAKE: word-sized staging preserves the byte/halfword live ranges;
     * direct narrow updates use different registers. T-24607 */
    u32 value;
    u32 character;
    GameState *game = &game_state;

    if (game_state.pad_pressed & 0x08000000) {
        options_voice_test_repeat_enabled = 1 - options_voice_test_repeat_enabled;
    }
    if ((options_voice_test_repeat_enabled != 0) && (cd_audio_state() & 2)) {
        game_state.pad_pressed |= PAD_CIRCLE;
        game_state.pad_held |= PAD_L1;
    }
    if (game_state.pad_held & 5) {
        if (game_state.pad_pressed & PAD_LEFT) {
            character = game->current_girl;
            game->current_girl = character + 1;
        }
        character = game->current_girl;
        if (game_state.pad_pressed & PAD_RIGHT) {
            character = (character - 1) & 0xFF;
        }
        if (character == 0xFF) {
            character = 0xF;
        }
        game->current_girl = character;
        if (character >= 0x11U) {
            game->current_girl = GIRL_SHIORI;
        }
    }
    if ((options_voice_bank_load_step | options_music_bank_load_step) != 0) {
        options_debug_audio_bank_load_dispatch();
    } else if (game_state.pad_pressed & 0x860) {
        switch (*(u8 *)&game_state.menu_cursor_0) {
        case 0:
            options_debug_audio_play_request(0);
            break;
        case 1:
            mode_step_set(3);
            break;
        case 2:
            mode_step_set(4);
            break;
        case 3:
            options_debug_audio_bank_start_primary();
            break;
        case 4:
            options_debug_audio_bank_start_secondary();
            break;
        case 5:
            if (game_state.pad_pressed & PAD_START) {
                mode_step_set(5);
            } else {
                options_debug_audio_bank_command();
            }
            break;
        case 6:
            options_debug_audio_bank_select();
            break;
        }
    }
    /* FAKE: options_debug_audio_track is volatile for options_debug_audio_play_request (T-11340). Its
     * nonvolatile halfword view here permits the original load coalescing.
     * Explicit staged stores preserve the observed updates. T-24607 */
    press_selector = game_state.pad_pressed & 0xF00000;
    repeat_selector = game_state.pad_held & 0xF00000;
    if (game_state.pad_held & 0x50000) {
        switch (repeat_selector) {
        case 0x200000:
            value = (*(u16 *)&options_debug_audio_track);
            (*(u16 *)&options_debug_audio_track) = value + 1;
            break;
        case 0x400000:
            value = (*(u16 *)&options_debug_audio_track);
            (*(u16 *)&options_debug_audio_track) = value + 0xA;
            break;
        case 0x800000:
            value = (*(u16 *)&options_debug_audio_track);
            (*(u16 *)&options_debug_audio_track) = value + 0x64;
            break;
        case 0x100000:
            value = (*(u16 *)&options_debug_audio_track);
            (*(u16 *)&options_debug_audio_track) = value + 0x3E8;
            break;
        }
        value = (*(u16 *)&options_debug_audio_track);
    } else {
        switch (press_selector) {
        default:
            value = (*(u16 *)&options_debug_audio_track);
            break;
        case 0x200000:
            value = ((*(u16 *)&options_debug_audio_track) + 1) & 0xFFFF;
            break;
        case 0x400000:
            value = ((*(u16 *)&options_debug_audio_track) + 0xA) & 0xFFFF;
            break;
        case 0x800000:
            value = ((*(u16 *)&options_debug_audio_track) + 0x64) & 0xFFFF;
            break;
        case 0x100000:
            value = ((*(u16 *)&options_debug_audio_track) + 0x3E8) & 0xFFFF;
            break;
        }
    }
    decrease_selector = game_state.pad_held & 0xF00000;
    if (game_state.pad_held & 0xA0000) {
        switch (decrease_selector) {
        case 0x200000:
            decreased = value - 1;
            /* FAKE: retain the decrement result in v0 before narrowing;
             * omitting the test adds a copy through v1. T-24607 */
            if (decreased) {}
            value = (u16)decreased;
            break;
        case 0x400000:
            decreased = value - 0xA;
            if (decreased != 0) {
                value = (u16)decreased;
            }
            break;
        case 0x800000:
            decreased = value - 0x64;
            if (decreased != 0) {
                value = (u16)decreased;
            }
            break;
        case 0x100000:
            decreased = value - 0x3E8;
            if (decreased != 0) {
                value = (u16)decreased;
            }
            break;
        }
    }
    (*(u16 *)&options_debug_audio_track) = value;
    if (value >= 0xB54U) {
        (*(u16 *)&options_debug_audio_track) = 0;
    }
    options_voice_test_debug_status_draw();
}

#define F80_BOOST_9E3C \
    (((game_state.pad_held ^ 1) * 0) + ((game_state.pad_held ^ 2) * 0) + ((game_state.pad_held ^ 3) * 0) + \
     ((game_state.pad_held ^ 4) * 0) + ((game_state.pad_held ^ 5) * 0) + ((game_state.pad_held ^ 6) * 0) + \
     ((game_state.pad_held ^ 7) * 0) + ((game_state.pad_held ^ 8) * 0) + ((game_state.pad_held ^ 9) * 0))
#define T_BOOST_9E3C \
    ((((s32)t ^ 1) * 0) + (((s32)t ^ 2) * 0) + (((s32)t ^ 3) * 0) + (((s32)t ^ 4) * 0) + (((s32)t ^ 5) * 0))
#define T2_BOOST_9E3C ((t2[2] * 0) + (t2[3] * 0) + (t2[4] * 0))

void options_debug_audio_play_request(s32 track_request) {
    u16 v;
    s32 *t;
    s32 *t2;

    if (track_request > 0) {
        options_debug_audio_track = track_request;
    }
    v = options_debug_audio_track;
    if (track_request < 0) {
        v++;
    }
    /* FAKE: zero-valued xor*0 and ptr[k]*0 terms reproduce uopt's priority ordering ($v0/$v1/$a3) and (void *) cast suppresses cvt_pass widening so ugen holds unk_F5F in $t2. T-24140 */
    if ((game_state.pad_held + F80_BOOST_9E3C) & PAD_L1) {
        v++;
    }
    if ((game_state.pad_held & PAD_L2) && (v != 0)) {
        v--;
    }
    t = D_8013D360[game_state.current_girl & 0xF];
    options_debug_audio_track = v + T_BOOST_9E3C;
    if (t != 0) {
        t2 = (t + ((s32)t * 0)) + v * 2;
        cd_audio_request_set(t2[0] + T2_BOOST_9E3C, t2[1], ((options_debug_audio_track = v) * 0) + (u32)(void *)game_state.current_girl);
    }
    audio_command(0x300);
}
#undef F80_BOOST_9E3C
#undef T_BOOST_9E3C
#undef T2_BOOST_9E3C


void options_debug_audio_bank_command(void) {
    if (game_state.pad_held & PAD_L2) {
        options_debug_audio_bank -= 1;
    }
    if (game_state.pad_held & PAD_L1) {
        options_debug_audio_bank += 1;
    }
    if (options_debug_audio_bank < 0) {
        options_debug_audio_bank = 0x20;
    }
    if (options_debug_audio_bank >= 0x21) {
        options_debug_audio_bank = 0;
    }
    audio_command((options_debug_audio_bank | 0x500) & 0xFFFF);
}

/* FAKE: width-staged locals preserve raw-byte and call-argument ranges. T-10850 */
void options_debug_audio_bank_select(void)
{
    GameState *g = &game_state;
    s32 t;
    u32 value;
    u16 code;
    t = g->pad_pressed & 0x60;
    if (t != 0x20)
    {
        if (t == 0x40)
        {
            options_voice_test_bank_start_argument--;
        }
    }
    else
    {
        options_voice_test_bank_start_argument++;
    }
    if (options_voice_test_bank_start_argument > 0x10)
    {
        value = options_voice_test_bank_start_argument;
        code = (value & 0xFFFF) | 0x1200; /* FAKE: the redundant mask makes IDO copy the byte to a0 first. T-24142 */
        audio_command(code);
    }
}

void options_debug_audio_bank_start_primary(void) {
    audio_bank_start(0, options_voice_test_bank_start_argument);
}

void options_debug_audio_bank_start_secondary(void) {
    audio_bank_start(1, options_voice_test_bank_start_argument);
}

void options_debug_audio_bank_load_dispatch(void) {
    if (options_voice_bank_load_step != 0) {
        options_debug_audio_bank_load_voice();
        return;
    }
    options_debug_audio_bank_load_music();
}

void options_debug_audio_bank_load_music(void) {
    u8 t = options_music_bank_load_step;

    switch (t) {
    case 1:
        audio_bank_load_start(1, 0xBF98, 0xBF79, options_voice_test_music_bank_load_arg_3[options_voice_test_music_clip_index % 63], options_voice_test_music_bank_load_arg_2[options_voice_test_music_clip_index % 63],
                      options_voice_test_music_bank_load_arg_1[options_voice_test_music_clip_index % 63]);
        options_music_bank_load_step += 1;
        break;
    case 2:
        if (audio_bank_load_poll() == 1) {
            options_music_bank_load_step = 0;
            strcpy(D_8013D3CC, options_voice_test_music_clip_labels[options_voice_test_music_clip_index % 63]);
            strcpy(D_8013D3E0, options_voice_test_character_labels[options_voice_test_character_index % 58]);
        }
        break;
    }
}

void options_debug_audio_bank_load_voice(void) {
    u8 t = options_voice_bank_load_step;

    switch (t) {
    case 1:
        audio_bank_load_start(0, options_voice_test_character_bank_load_arguments[options_voice_test_character_index % 58], options_voice_test_character_bank_load_commands[options_voice_test_character_index % 58], options_voice_test_character_bank_load_arg_3[options_voice_test_character_clip_index % 63],
                      options_voice_test_character_bank_load_arg_2[options_voice_test_character_clip_index % 63], options_voice_test_character_bank_load_arg_1[options_voice_test_character_clip_index % 63]);
        options_voice_bank_load_step += 1;
        break;
    case 2:
        if (audio_bank_load_poll() == 1) {
            options_voice_bank_load_step = 0;
            strcpy(D_8013D3BC, options_voice_test_character_clip_labels[options_voice_test_character_clip_index % 63]);
            strcpy(D_8013D3E0, options_voice_test_character_labels[options_voice_test_character_index % 58]);
        }
        break;
    }
}

void options_debug_audio_music_load_begin(void) {
    options_music_bank_load_step += 1;
}

void options_debug_audio_voice_load_begin(void) {
    options_voice_bank_load_step += 1;
}

void options_voice_test_debug_status_draw(void) {
    debug_print_int_color(options_debug_audio_track, 0x50, -0x3C, 6);
    debug_print_int_color(game_state.current_girl & 0xF, -0x50, -0x3C, 6);
    debug_print_int_color(options_voice_bank_load_step, 0x50, -0x28, 6);
    debug_print_int_color(options_music_bank_load_step, 0x50, -0x14, 6);
    debug_print_int_color(options_voice_test_character_clip_index, 0x50, 0, 6);
    debug_print_string_color(D_8013D3BC, -0x96, 8, 6);
    debug_print_int_color(options_voice_test_music_clip_index, 0x50, 0x14, 6);
    debug_print_string_color(D_8013D3CC, -0x96, 0x14, 6);
    debug_print_int_color(D_8013D39C, -0x50, 0xA - D_8013D39C * 0xA, 6);
    debug_print_int_color(options_debug_audio_bank, 0x50, 0x28, 6);
    debug_print_string_color(D_8013D3E0, -0x96, 0x20, 6);
    debug_print_int_color(options_voice_test_bank_start_argument, 0x50, 0x3C, 6);
}

void options_voice_test_background_gradient_draw(void) {
    prim_gradient_rect(-0x50, -0x40, 0xB0, 0x90, 0xA, 0x1E021D, 0);
    prim_gradient_rect(-0x96, 4, 0x50, 0x2C, 0xA, 0x101802, 0);
    dtd_on(0xA);
}

void options_voice_test_selection_gradient_draw(void) {
    prim_gradient_rect(-0xA, -0x6E, 0x50, 0xB4, 6, 0xA08080, 2);
    dtd_on(6);
}
