/* name_entry.c: src/ovl/NAME_ENT/80132000.c:103,112,1962,2451; mode0x14 name/preset/grid/birthday input, profile storage and new-game initial scores/clubs. Original object 0x80132000-0x80139740 (config/objects/NAME_ENT.txt). */
#define MAIN_API_OVERRIDE_strncpy /* FAKE: implicit int view, no prototype: name_entry_symbol_page_draw calls it without the length, and the unused $v0 result keeps the address in name_entry_name_finish off $v0 (main_api.h: void, three parameters; T-20810, T-21520) */
#include "common.h"
#include "ovl/NAME_ENT.h"
#include "game.h"

s32 strncpy();

/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_unused_clip_start);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_family_prompt_clip_start);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_family_prompt_clip_end);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_given_prompt_clip_start);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_given_prompt_clip_end);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_nickname_prompt_clip_start);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_nickname_prompt_clip_end);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_birthday_prompt_clip_start);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_birthday_prompt_clip_end);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_shiori_prompt_clip_start);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_shiori_prompt_clip_end);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_confirm_clip_start);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_confirm_clip_end);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_cursor);
u8 name_entry_family_name[8] = { 0 };
u8 name_entry_given_name[8] = { 0 };
u8 name_entry_nickname[12] = { 0 };
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_player_month);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_player_day);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_player_blood);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_shiori_month);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_shiori_day);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_shiori_blood);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_page_scroll);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_page_text_length);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_reading);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_reading_column);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_reading_row);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_cell_column);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_cell_row);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_tab);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_tab_hover);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_tab_y);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_family_prompt_text);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_given_prompt_text);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_nickname_prompt_text);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_hiragana_rows);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_hiragana_rows_01);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_hiragana_rows_02);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_hiragana_rows_03);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_hiragana_rows_04);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_hiragana_rows_05);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_hiragana_rows_06);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_hiragana_rows_07);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_hiragana_rows_08);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_hiragana_rows_09);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_hiragana_rows_10);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_hiragana_rows_11);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_hiragana_rows_12);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_hiragana_rows_13);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_hiragana_rows_14);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_hiragana_rows_15);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_hiragana_rows_16);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_katakana_rows);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_katakana_rows_01);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_katakana_rows_02);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_katakana_rows_03);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_katakana_rows_04);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_katakana_rows_05);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_katakana_rows_06);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_katakana_rows_07);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_katakana_rows_08);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_katakana_rows_09);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_katakana_rows_10);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_katakana_rows_11);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_katakana_rows_12);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_katakana_rows_13);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_katakana_rows_14);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_katakana_rows_15);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_katakana_rows_16);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_reading_labels);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_candidate_counts);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_candidate_lists);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_preset_family_names);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_preset_given_names);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_preset_nicknames);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_prompt_text);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_small_digit_uv);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.data", name_entry_wide_digit_uv);
u8 name_entry_blood_compatibility_level[16] = {
    1, 4, 3, 2,
    3, 1, 2, 2,
    2, 3, 3, 4,
    3, 2, 3, 1,
};
u8 name_entry_initial_scores[20] = {
    0x2D, 0x0A, 0x0A,
    0x32, 0x14, 0x00,
    0x32, 0x19, 0x00,
    0x37, 0x19, 0x00,
    0, 0, 0, 0, 0, 0, 0, 0,
};

void name_entry_preset_apply(s32 arg0) {
    s32 rem;

    rem = arg0 % 15;
    strncpy(name_entry_family_name, name_entry_preset_family_names + rem * 8, 8);
    strncpy(name_entry_given_name, name_entry_preset_given_names + rem * 8, 8);
    strncpy(name_entry_nickname, name_entry_preset_nicknames + rem * 0xC, 0xC);
}

void name_entry_mode_handler(void) {
    switch (game_state.mode_stage) {
    case 0:
        name_entry_mode_stage_load();
        break;
    case 1:
        name_entry_mode_stage_input();
        break;
    case 2:
        name_entry_mode_stage_shiori();
        break;
    }
    dec_bg_draw(2);
    func_8004B19C(bg_layers, 0, 0x50);
    func_8004B19C(D_801220F4, 0, 0x5A);
}

void name_entry_mode_stage_load(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        name_entry_load_init();
        return;
    case 1:
        name_entry_load_fade_in();
        return;
    }
}
/* FAKE: an empty statement after initialization preserves base-load/store scheduling. T-9910. */
void name_entry_load_init(void)
{
  if (game_state.substep == 0)
  {
    text_system_reset();
    text_set_advance(0x10);
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_upper();
    hud_cursor_actor_init(0);
    hud_cursor_show(0);
    back_clear_switch(1);
    game_state.clear_color_g = 0;
    game_state.clear_color_r = 0;
    game_state.clear_color_b = 0;
    tpage_buf_clear();
    func_80041168(0);
    game_state.tile_layer_enabled = 0;
    dec_bg_reset();
    dec_bg_show_switch(2);
    cd_read_sectors_nowait(0xAU, 0x80162000, 0xAA18);
    game_state.cursor_speed = 3;
    dec_bg_brightness = 0;
    game_state.substep = 1;
 ;
  }
  else
    if (game_state.substep == 1)
  {
    if (cd_read_sync_status() & 1)
    {
      func_80053650(0x80167000, 0x80188000, 0x80162000);
      func_800536AC(0x140, 0x100, 0x140, 0xF0);
      cd_read_sectors(0x36U, 0x80190000, 0xA9E2);
      game_state.substep += 1;
    }
  }
  else
    if (game_state.substep == 2)
  {
    if (cd_read_sync_status() & 1)
    {
      cd_read_sectors(6U, 0x801E0000, 0x9DBF);
      mode_step_next();
    }
  }
}

/* FAKE: the five & -1 identities retain the masked byte in the original
 * register. Removing any one loses the match; see T-20013. */
void name_entry_load_fade_in(void) {
    s32 i;
    s32 val;
    u8 **p;

    if (game_state.substep == 0) {
        if (cd_read_sync_status() & 1) {
            p = name_entry_candidate_lists;
            for (i = 0; i < 50; i++) {
                p[i] += 0x801E0000;
            }
            name_entry_background_setup();
            D_801220E1 = 0;
            D_801220E0 = 0;
            game_state.substep += 1;
            D_801220E2 = 0;
            game_state.screen_brightness = 0;
        }
    } else {
        val = (dec_bg_brightness + 4) & 0xFF;
        D_801220E1 = (val & -1);
        D_801220E0 = (val & -1);
        dec_bg_brightness = (val & -1);
        D_801220E2 = (val & -1);
        game_state.screen_brightness = (val & -1);
        if ((u32)val >= 0x80) {
            func_80057640(0xF0);
            name_entry_input_actors_init();
            bg_layers = 0x80000000;
            D_801220D4 = -0x18;
            hud_cursor_show(1);
            back_clear_switch(0);
            mode_stage_next();
        }
    }
    func_8004B19C(dec_bg_brightness, 0, 0);
    func_8004B19C(D_801220E0, 0, 0x10);
}

void name_entry_input_actors_init(void) {
    s32 index;
    Actor *actor;

    name_entry_overlay_tables_set();
    for (index = 0; index < 0x20;) {
        actor_clear(index + 0x60);
        actor = (Actor *)actor_pool + index;
        actor[96].unk_01 = 2;
        actor[96].flags = 0x40;
        actor[96].quad_flags = 0;
        actor[96].draw_flags = 0x84;
        actor[96].frame_table = (u8 *)game_state.ovl_frame_table;
        actor[96].sequence_table = (u8 *)game_state.ovl_sequence_table;
        actor[96].tpage_table = (u8 *)game_state.ovl_tpage_table;
        /* FAKE: re-index here for the original induction-update order. T-10890. */
        index++;
        actor = (Actor *)actor_pool + index;
        actor[95].unk_04 = 8;
        actor[95].unk_05 = 0x81;
        actor[95].unk_18 = 0;
        actor[95].unk_43 = 0x10;
        actor[95].x = 0x190;
        actor[95].y = 0;
    }
    ((Actor *)actor_pool)[96].unk_18 = 3;
    ((Actor *)actor_pool)[96].x = -0x70;
    ((Actor *)actor_pool)[96].y = -0x3C;
    ((Actor *)actor_pool)[97].unk_18 = 0;
    ((Actor *)actor_pool)[97].x = 0;
    ((Actor *)actor_pool)[97].y = 0;
    ((Actor *)actor_pool)[98].unk_18 = 0xD;
    ((Actor *)actor_pool)[98].x = -0x60;
    ((Actor *)actor_pool)[98].y = -0x4A;
    ((Actor *)actor_pool)[99].unk_18 = 0xD;
    ((Actor *)actor_pool)[99].x = -0x50;
    ((Actor *)actor_pool)[99].y = -0x4A;
    ((Actor *)actor_pool)[100].unk_18 = 0xD;
    ((Actor *)actor_pool)[100].x = -0x40;
    ((Actor *)actor_pool)[100].y = -0x4A;
    ((Actor *)actor_pool)[101].unk_18 = 0xD;
    ((Actor *)actor_pool)[101].x = -0x28;
    ((Actor *)actor_pool)[101].y = -0x4A;
    ((Actor *)actor_pool)[102].unk_18 = 0xD;
    ((Actor *)actor_pool)[102].x = -0x18;
    ((Actor *)actor_pool)[102].y = -0x4A;
    ((Actor *)actor_pool)[103].unk_18 = 0xD;
    ((Actor *)actor_pool)[103].x = -8;
    ((Actor *)actor_pool)[103].y = -0x4A;
    ((Actor *)actor_pool)[104].unk_18 = 0xD;
    ((Actor *)actor_pool)[104].x = 0x50;
    ((Actor *)actor_pool)[104].y = -0x4A;
    ((Actor *)actor_pool)[105].unk_18 = 0xD;
    ((Actor *)actor_pool)[105].x = 0x60;
    ((Actor *)actor_pool)[105].y = -0x4A;
    ((Actor *)actor_pool)[106].unk_18 = 0xD;
    ((Actor *)actor_pool)[106].x = 0x70;
    ((Actor *)actor_pool)[106].y = -0x4A;
    ((Actor *)actor_pool)[107].unk_18 = 0xD;
    ((Actor *)actor_pool)[107].x = 0x80;
    ((Actor *)actor_pool)[107].y = -0x4A;
    ((Actor *)actor_pool)[108].unk_18 = 0xD;
    ((Actor *)actor_pool)[108].x = 0x90;
    ((Actor *)actor_pool)[108].y = -0x4A;
    ((Actor *)actor_pool)[109].unk_18 = 0xF;
    ((Actor *)actor_pool)[109].x = -0x8F;
    ((Actor *)actor_pool)[109].y = -0x28;
    ((Actor *)actor_pool)[110].unk_18 = 0x11;
    ((Actor *)actor_pool)[110].x = -0x8F;
    ((Actor *)actor_pool)[110].y = -8;
    ((Actor *)actor_pool)[111].unk_18 = 0x13;
    ((Actor *)actor_pool)[111].x = -0x8F;
    ((Actor *)actor_pool)[111].y = 0x18;
    ((Actor *)actor_pool)[112].unk_18 = 0x15;
    ((Actor *)actor_pool)[112].x = -0x8F;
    ((Actor *)actor_pool)[112].y = 0x38;
    ((Actor *)actor_pool)[113].unk_18 = 0x17;
    ((Actor *)actor_pool)[113].x = -0x8F;
    ((Actor *)actor_pool)[113].y = 0x58;
    ((Actor *)actor_pool)[114].unk_18 = 7;
    ((Actor *)actor_pool)[114].x = -0x8F;
    ((Actor *)actor_pool)[114].y = -0x28;
    ((Actor *)actor_pool)[115].unk_18 = 7;
    ((Actor *)actor_pool)[115].x = -0x8F;
    ((Actor *)actor_pool)[115].y = -8;
    ((Actor *)actor_pool)[116].unk_18 = 7;
    ((Actor *)actor_pool)[116].x = -0x8F;
    ((Actor *)actor_pool)[116].y = 0x18;
    ((Actor *)actor_pool)[117].unk_18 = 7;
    ((Actor *)actor_pool)[117].x = -0x8F;
    ((Actor *)actor_pool)[117].y = 0x38;
    ((Actor *)actor_pool)[118].unk_18 = 7;
    ((Actor *)actor_pool)[118].x = -0x8F;
    ((Actor *)actor_pool)[118].y = 0x58;
    /* FAKE: one line preserves as1 scheduling of the unrolled flag stores. T-10890. */
    index = 0; do { actor = (Actor *)actor_pool + index; actor[114].draw_flags &= 0x7F; index++; } while (index < 5);
    ((Actor *)actor_pool)[119].unk_18 = 9;
    ((Actor *)actor_pool)[119].x = -0x8F;
    ((Actor *)actor_pool)[119].y = -0x28;
    ((Actor *)actor_pool)[119].unk_01 = 1;
    ((Actor *)actor_pool)[120].unk_18 = 4;
    ((Actor *)actor_pool)[120].x = 0x100;
    ((Actor *)actor_pool)[120].y = 0x100;
    ((Actor *)actor_pool)[120].unk_01 = 3;
    ((Actor *)actor_pool)[121].unk_18 = 0x19;
    ((Actor *)actor_pool)[121].x = 0x88;
    ((Actor *)actor_pool)[121].y = -0x28;
    ((Actor *)actor_pool)[121].draw_flags = 0;
    ((Actor *)actor_pool)[122].unk_18 = 0x1B;
    ((Actor *)actor_pool)[122].x = 0x88;
    ((Actor *)actor_pool)[122].y = 0x18;
    ((Actor *)actor_pool)[122].draw_flags = 0;
    ((Actor *)actor_pool)[123].unk_18 = 8;
    ((Actor *)actor_pool)[123].x = 0x88;
    ((Actor *)actor_pool)[123].y = -0x28;
    ((Actor *)actor_pool)[123].draw_flags = 0;
    ((Actor *)actor_pool)[124].unk_18 = 8;
    ((Actor *)actor_pool)[124].x = 0x88;
    ((Actor *)actor_pool)[124].y = 0x18;
    ((Actor *)actor_pool)[124].draw_flags = 0;
}

void name_entry_background_setup(void) {
    s32 i;
    s32 pad[2]; /* FAKE: unused 8-byte local reproduces the original frame (buf at 0x4C); real source unknown. T-11650 */
    s32 buf[6];

    name_entry_overlay_tables_set();
    /* FAKE: loop body on the for line gives the original as1 schedule. T-11650 */
    for (i = 0; i < 6; i++) { buf[i] = (u32)game_state.ovl_image_buffer + i * 0x4000; }
    func_8004A1A0(buf, 0, 6, 3, 2, 0x1F8, -0xA0, -0x78, 8, 0xD);
    bg_layers |= 0x40000000;
    game_state.tpage_buf[search_tpage_multi(*(s32 *)game_state.ovl_tpage_table, 2)] = *(s32 *)game_state.ovl_tpage_table;
    load_palette(game_state.ovl_palette_b, 0x18, 1, 2, 0);
    load_palette(game_state.ovl_palette_a, 0x10, 1, 2, 0);
}

void name_entry_slot_markers_update(s32 arg0) {
    s32 i;
    s32 x;
    s32 y;
    s32 pad; /* FAKE: unused; its stack home gives the original 72-byte frame. T-11650 */

    if (arg0 == 0 && name_entry_cursor >= 3) {
        name_entry_cursor = 0xC;
    }
    if (arg0 == 1 && name_entry_cursor >= 6) {
        name_entry_cursor = 0xC;
    }
    if (arg0 == 2 && name_entry_cursor >= 11) {
        name_entry_cursor = 0xC;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        x = ((Actor *)actor_pool)[0].x;
        y = ((Actor *)actor_pool)[0].y;
        if (arg0 == 0) {
            for (i = 0; i < 3; i++) {
                if (x < i * 16 - 0x58 && i * 16 - 0x68 < x
                    && y < -0x46 && y >= -0x55) {
                    name_entry_cursor = i;
                }
            }
        } else if (arg0 == 1) {
            for (i = 0; i < 3; i++) {
                if (x < i * 16 - 0x20 && i * 16 - 0x30 < x
                    && y < -0x46 && y >= -0x55) {
                    name_entry_cursor = i + 3;
                }
            }
        } else if (arg0 == 2) {
            for (i = 0; i < 5; i++) {
                if (x < i * 16 + 0x58 && i * 16 < x - 0x48
                    && y < -0x46 && y >= -0x55) {
                    name_entry_cursor = i + 6;
                }
            }
        }
    }
    for (i = 0; i < 11; i++) {
        if (i == name_entry_cursor) {
            if (((Actor *)actor_pool)[98 + i].unk_18 == 0xD) {
                ((Actor *)actor_pool)[98 + i].unk_18 = 0;
            }
            name_entry_actor_animation_start(i + 0x62);
        } else {
            ((Actor *)actor_pool)[98 + i].flags = 0x40;
            ((Actor *)actor_pool)[98 + i].unk_18 = 0xD;
        }
    }
}

void name_entry_mode_stage_input(void) {
    if (game_state.mode_step != 0) {
        if (game_state.mode_step < 6) {
            name_entry_frames_draw(0);
            name_entry_cell_cursor_update();
        } else if (game_state.mode_step < 8) {
            name_entry_birthday_frames_draw(0);
            name_entry_birthday_hit_test(0);
        } else {
            name_entry_confirm_frames_draw(0);
            name_entry_empty_callback();
        }
    }
    switch (game_state.mode_step) {
    case 0:
        name_entry_step_init();
        break;
    case 1:
        name_entry_family_name_input();
        break;
    case 2:
        name_entry_given_name_init();
        break;
    case 3:
        name_entry_given_name_input();
        break;
    case 4:
        name_entry_nickname_init();
        break;
    case 5:
        name_entry_nickname_input();
        break;
    case 6:
        name_entry_birthday_init();
        break;
    case 7:
        name_entry_birthday_input();
        break;
    case 8:
        name_entry_confirm_init();
        break;
    case 9:
        name_entry_confirm_input();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B140);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B150);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B15C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B168);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B174);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B180);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B18C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B198);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B1A4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B1AC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B1B8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B1C0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B1CC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B1D8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B1E4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B1F0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B1FC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B208);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B214);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B220);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B22C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B238);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B244);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B250);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B25C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B268);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B270);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B27C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B284);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B290);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B29C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B2A8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B2B4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B2C0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B2CC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B2D8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B2E4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B2F0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B2FC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B308);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B314);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B320);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B32C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B338);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B344);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B350);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B35C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B368);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B374);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B380);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B38C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B398);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B3A4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B3B0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B3BC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B3C8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B3D0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B3D8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B3E0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B3E8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B3F0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B400);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B410);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B420);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B430);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B440);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B448);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B450);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B458);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B460);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B468);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B470);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B478);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B480);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B484);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B48C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B494);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B49C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B4A4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/name_entry.rodata", D_8014B4AC);

void name_entry_reading_rows_draw(void) {
    set_kanji_string(-0x78, 0x38, 0xA, name_entry_hiragana_rows, 0);
    set_kanji_string(-0x18, 0x38, 0xA, name_entry_hiragana_rows_01, 0);
    set_kanji_string(0x48, 0x38, 0xA, name_entry_hiragana_rows_02, 0);
    set_kanji_string(-0x78, 0x48, 0xA, name_entry_hiragana_rows_03, 0);
    set_kanji_string(-0x18, 0x48, 0xA, name_entry_hiragana_rows_04, 0);
    set_kanji_string(0x48, 0x48, 0xA, name_entry_hiragana_rows_05, 0);
    set_kanji_string(-0x78, 0x58, 0xA, name_entry_hiragana_rows_06, 0);
    set_kanji_string(-0x18, 0x58, 0xA, "やゆよ　わ", 0);
    set_kanji_string(0x48, 0x58, 0xA, name_entry_hiragana_rows_08, 0);
}

void name_entry_hiragana_page_draw(s32 arg0) {
    text_system_reset();
    set_kanji_string(-0x98, -0x68, 0, arg0, 0);
    set_kanji_string(-0x78, -0x30, 0xA, name_entry_hiragana_rows, 0);
    set_kanji_string(-0x78, -0x20, 0xA, name_entry_hiragana_rows_03, 0);
    set_kanji_string(-0x78, -0x10, 0xA, name_entry_hiragana_rows_06, 0);
    set_kanji_string(-0x78, 0, 0xA, name_entry_hiragana_rows_09, 0);
    set_kanji_string(-0x78, 0x10, 0xA, name_entry_hiragana_rows_12, 0);
    set_kanji_string(-0x78, 0x20, 0xA, name_entry_hiragana_rows_15, 0);
    set_kanji_string(-0x18, -0x30, 0xA, name_entry_hiragana_rows_01, 0);
    set_kanji_string(-0x18, -0x20, 0xA, name_entry_hiragana_rows_04, 0);
    set_kanji_string(-0x18, -0x10, 0xA, name_entry_hiragana_rows_07, 0);
    set_kanji_string(-0x18, 0, 0xA, name_entry_hiragana_rows_10, 0);
    set_kanji_string(-0x18, 0x10, 0xA, name_entry_hiragana_rows_13, 0);
    set_kanji_string(-0x18, 0x20, 0xA, name_entry_hiragana_rows_16, 0);
    set_kanji_string(0x48, -0x30, 0xA, name_entry_hiragana_rows_02, 0);
    set_kanji_string(0x48, -0x20, 0xA, name_entry_hiragana_rows_05, 0);
    set_kanji_string(0x48, -0x10, 0xA, name_entry_hiragana_rows_08, 0);
    set_kanji_string(0x48, 0, 0xA, name_entry_hiragana_rows_11, 0);
    set_kanji_string(0x48, 0x10, 0xA, name_entry_hiragana_rows_14, 0);
}

void name_entry_katakana_page_draw(s32 arg0) {
    text_system_reset();
    set_kanji_string(-0x98, -0x68, 0xA, arg0, 0);
    set_kanji_string(-0x78, -0x30, 0xA, name_entry_katakana_rows, 0);
    set_kanji_string(-0x78, -0x20, 0xA, name_entry_katakana_rows_03, 0);
    set_kanji_string(-0x78, -0x10, 0xA, name_entry_katakana_rows_06, 0);
    set_kanji_string(-0x78, 0, 0xA, name_entry_katakana_rows_09, 0);
    set_kanji_string(-0x78, 0x10, 0xA, name_entry_katakana_rows_12, 0);
    set_kanji_string(-0x78, 0x20, 0xA, name_entry_katakana_rows_15, 0);
    set_kanji_string(-0x18, -0x30, 0xA, name_entry_katakana_rows_01, 0);
    set_kanji_string(-0x18, -0x20, 0xA, name_entry_katakana_rows_04, 0);
    set_kanji_string(-0x18, -0x10, 0xA, name_entry_katakana_rows_07, 0);
    set_kanji_string(-0x18, 0, 0xA, name_entry_katakana_rows_10, 0);
    set_kanji_string(-0x18, 0x10, 0xA, name_entry_katakana_rows_13, 0);
    set_kanji_string(-0x18, 0x20, 0xA, name_entry_katakana_rows_16, 0);
    set_kanji_string(0x48, -0x30, 0xA, name_entry_katakana_rows_02, 0);
    set_kanji_string(0x48, -0x20, 0xA, name_entry_katakana_rows_05, 0);
    set_kanji_string(0x48, -0x10, 0xA, name_entry_katakana_rows_08, 0);
    set_kanji_string(0x48, 0, 0xA, name_entry_katakana_rows_11, 0);
    set_kanji_string(0x48, 0x10, 0xA, name_entry_katakana_rows_14, 0);
}

void name_entry_symbol_page_draw(u8 *arg0) {
    s32 i;
    u8 *p;
    u8 *text;
    u8 line0[0x24];
    u8 line1[0x24];
    u8 line2[0x24];
    u8 line3[0x24];
    u8 line4[0x24];
    u8 line5[0x24];

    for (i = 0; i < 0x23; i++) {
        line0[i] = 0;
        line1[i] = 0;
        line2[i] = 0;
        line3[i] = 0;
        line4[i] = 0;
        line5[i] = 0;
    }
    text_system_reset();
    set_kanji_string(-0x98, -0x68, 0xA, arg0, 0);
    if (name_entry_candidate_counts[38] < name_entry_page_scroll * 16 + 0x50) {
        name_entry_page_scroll = (name_entry_candidate_counts[38] >> 4) - 5;
        if (name_entry_page_scroll < 0) {
            name_entry_page_scroll = 0;
        }
    }
    name_entry_page_text_length = strlen(name_entry_candidate_lists[38]);
    text = name_entry_candidate_lists[38];
    text += name_entry_page_scroll * 32;
    if (strlen(text) < 0x20) {
        strcpy(line0, text);
    } else {
        strncpy(line0, text, 0x20);
        p = text + 0x20;
        if (strlen(p) < 0x20) {
            strcpy(line1, p);
        } else {
            strncpy(line1, p, 0x20);
            p = text + 0x40;
            if (strlen(p) < 0x20) {
                strcpy(line2, p);
            } else {
                strncpy(line2, p, 0x20);
                p = text + 0x60;
                if (strlen(p) < 0x20) {
                    strcpy(line3, p);
                } else {
                    strncpy(line3, p, 0x20);
                    p = text + 0x80;
                    if (strlen(p) < 0x20) {
                        strcpy(line4, p);
                    } else {
                        strncpy(line4, p);
                        p = text + 0xA0;
                        if (strlen(p) < 0x20) {
                            strcpy(line5, p);
                        } else {
                            strncpy(line5, p);
                        }
                    }
                }
            }
        }
    }
    if (line0[0] != 0) {
        set_kanji_string(-0x78, -0x30, 0xA, line0, 0);
    }
    if (line1[0] != 0) {
        set_kanji_string(-0x78, -0x20, 0xA, line1, 0);
    }
    if (line2[0] != 0) {
        set_kanji_string(-0x78, -0x10, 0xA, line2, 0);
    }
    if (line3[0] != 0) {
        set_kanji_string(-0x78, 0, 0xA, line3, 0);
    }
    if (line4[0] != 0) {
        set_kanji_string(-0x78, 0x10, 0xA, line4, 0);
    }
    if (line5[0] != 0) {
        set_kanji_string(-0x78, 0x20, 0xA, line5, 0);
    }
}

void name_entry_kanji_page_draw(s32 arg0) {
    s32 i;
    s32 pad[2]; /* FAKE: unused; its home gives the original 0x110-byte frame. T-11650 */
    u8 *text;
    u8 line0[0x24];
    u8 line1[0x24];
    u8 line2[0x24];
    u8 line3[0x24];
    u8 line4[0x24];

    switch (arg0) {
    case 0:
        if (game_state.mode_step == 1) {
            name_entry_prompt_text = name_entry_family_prompt_text;
        }
        if (game_state.mode_step == 3) {
            name_entry_prompt_text = name_entry_given_prompt_text;
        }
        if (game_state.mode_step == 5) {
            name_entry_prompt_text = name_entry_nickname_prompt_text;
        }
        break;
    case 1:
        name_entry_prompt_text = name_entry_family_prompt_text;
        break;
    case 2:
        name_entry_prompt_text = name_entry_given_prompt_text;
        break;
    case 3:
        name_entry_prompt_text = name_entry_nickname_prompt_text;
        break;
    }
    for (i = 0; i < 0x23; i++) {
        line0[i] = 0;
        line1[i] = 0;
        line2[i] = 0;
        line3[i] = 0;
        line4[i] = 0;
    }
    if (name_entry_reading < 0 || name_entry_reading >= 0x2D) {
        name_entry_reading = 0;
    }
    if (name_entry_tab == 2 || name_entry_reading == 0x26) {
        name_entry_symbol_page_draw(name_entry_prompt_text);
    } else {
        text_system_reset();
        set_kanji_string(-0x98, -0x68, 0xA, name_entry_prompt_text, 0);
        set_kanji_string(-0x78, -0x30, 0xA, name_entry_reading_labels[name_entry_reading], 0);
        name_entry_page_text_length = strlen(name_entry_candidate_lists[name_entry_reading]);
        if (name_entry_candidate_counts[name_entry_reading] < name_entry_page_scroll * 16 + 0x40) {
            name_entry_page_scroll = (name_entry_candidate_counts[name_entry_reading] >> 4) - 4;
            if (name_entry_page_scroll < 0) {
                name_entry_page_scroll = 0;
            }
        }
        text = name_entry_candidate_lists[name_entry_reading];
        text += name_entry_page_scroll * 32;
        if (strlen(text) < 0x20) {
            strcpy(line0, text);
        } else {
            strncpy(line0, text, 0x20);
            if (strlen(text + 0x20) < 0x20) {
                strcpy(line1, text + 0x20);
            } else {
                strncpy(line1, text + 0x20, 0x20);
                if (strlen(text + 0x40) < 0x20) {
                    strcpy(line2, text + 0x40);
                } else {
                    strncpy(line2, text + 0x40, 0x20);
                    if (strlen(text + 0x60) < 0x20) {
                        strcpy(line3, text + 0x60);
                    } else {
                        strncpy(line3, text + 0x60, 0x20);
                        if (strlen(text + 0x80) < 0x20) {
                            strcpy(line4, text + 0x80);
                        } else {
                            strncpy(line4, text + 0x80);
                        }
                    }
                }
            }
        }
        if (line0[0] != 0) {
            set_kanji_string(-0x78, -0x20, 0xA, line0, 0);
        }
        if (line1[0] != 0) {
            set_kanji_string(-0x78, -0x10, 0xA, line1, 0);
        }
        if (line2[0] != 0) {
            set_kanji_string(-0x78, 0, 0xA, line2, 0);
        }
        if (line3[0] != 0) {
            set_kanji_string(-0x78, 0x10, 0xA, line3, 0);
        }
        if (line4[0] != 0) {
            set_kanji_string(-0x78, 0x20, 0xA, line4, 0);
        }
    }
    name_entry_reading_rows_draw();
}

void name_entry_cell_hit_test(void) {
    s32 x;
    s32 y;

    y = cursor_y;
    x = cursor_x;
    name_entry_cell_column = -1;
    if ((x >= -0x77) && (x < 0x98) && (y >= -0x2F) && (y < 0x30)) {
        name_entry_cell_column = (s32) (x + 0x78) / 16;
        name_entry_cell_row = (s32) (y + 0x30) / 16;
    } else {
        name_entry_cell_row = -1;
    }
    if ((name_entry_tab == 2) || (name_entry_tab == 3)) {
        if (name_entry_cell_column == 0x10) {
            name_entry_cell_column = -1;
        }
    }
}

void name_entry_tab_hit_test(void) {
    s32 x;
    s32 y;
    s32 i;

    x = cursor_x;
    y = cursor_y;
    name_entry_tab_hover = -1;
    for (i = 0; i < 5; i++) {
        if (x >= -0x9D && x < -0x80 && name_entry_tab_y[i] < y && y < name_entry_tab_y[i] + 0x10) {
            name_entry_tab_hover = i;
        }
    }
}

void name_entry_tab_highlight_update(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        if (i == name_entry_tab) {
            ((Actor *)actor_pool)[i + 114].draw_flags |= 0x80;
        } else {
            ((Actor *)actor_pool)[i + 114].draw_flags &= 0x7F;
        }
    }
    if (name_entry_tab_hover >= 0) {
        D_80120C96 = (name_entry_tab_hover << 5) - 0x28;
        return;
    }
    D_80120C96 = -0xB4;
}

/* FAKE: implicit integer exit liveness is required; void differs in five
 * instructions. Callers use only the coordinate outputs. T-20011. */
s32 name_entry_reading_hit_test(void) {
    Actor *actor;
    s32 x;
    s32 y;

    actor = (Actor *)actor_pool;
    x = actor->x;
    if (x >= -0x77 && x < 0x98) {
        y = actor->y;
        if (y >= 0x39 && y < 0x68) {
            name_entry_reading_column = (x + 0x78) / 16;
            name_entry_reading_row = (y - 0x38) / 16;
            return;
        }
    }
    name_entry_reading_column = -1;
    name_entry_reading_row = -1;
}

void name_entry_reading_select(void) {
    s32 prev;

    prev = name_entry_reading;
    if ((game_state.pad_pressed & PAD_CIRCLE) && name_entry_reading_column != -1 && name_entry_reading_row != -1) {
        name_entry_reading = name_entry_reading_row * 15 + name_entry_reading_column % 6 + name_entry_reading_column / 6 * 5;
        if (name_entry_reading != prev) {
            name_entry_page_scroll = 0;
        }
    }
}

/* FAKE: indirect remainder loads and empty column tests preserve distinct
 * value ranges; | 0 preserves comparison operand order. T-20013 records the
 * individual removal controls for these matching constraints. */
void name_entry_character_put(s32 arg0) {
    s32 selected;
    u32 index;
    s32 column;
    u8 *dest;

    index = name_entry_cursor;
    selected = index;
    switch (arg0) {
    case 0:
        if (selected >= 3) {
            selected = 2;
        }
        break;
    case 1:
        index = name_entry_cursor;
        selected = index - 3;
        if (selected >= 3) {
            selected = 2;
        }
        break;
    case 2:
        index = name_entry_cursor;
        selected = index - 6;
        if (selected >= 5) {
            selected = 4;
        }
        break;
    }
    switch (arg0) {
    case 0:
        dest = name_entry_family_name;
        break;
    case 1:
        dest = name_entry_given_name;
        break;
    case 2:
        dest = name_entry_nickname;
        break;
    }
    switch (name_entry_tab) {
    case 0:
        column = *(s32 *)&name_entry_cell_column % 6;
        if ((column | 0) == 5) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (name_entry_cell_row == 2 && (name_entry_cell_column == 9 || name_entry_cell_column == 10)) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (name_entry_cell_row == 3 && (name_entry_cell_column == 3 || name_entry_cell_column == 4)) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (name_entry_cell_row == 5 && name_entry_cell_column >= 11) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else {
            dest[selected * 2] = ((NameKanaPages44 *)&name_entry_hiragana_rows)->text[(s32)name_entry_cell_column / 6 + name_entry_cell_row * 3][column * 2];
            dest[selected * 2 + 1] = ((NameKanaPages44 *)&name_entry_hiragana_rows)->text[(s32)name_entry_cell_column / 6 + name_entry_cell_row * 3][((s32)name_entry_cell_column % 6) * 2 + 1];
        }
        break;
    case 1:
        column = *(s32 *)&name_entry_cell_column % 6;
        if ((column | 0) == 5) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (name_entry_cell_row == 2 && (name_entry_cell_column == 9 || name_entry_cell_column == 10)) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (name_entry_cell_row == 3 && (name_entry_cell_column == 3 || name_entry_cell_column == 4)) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (name_entry_cell_row == 5 && name_entry_cell_column >= 11) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else {
            dest[selected * 2] = ((NameKanaPages44 *)&name_entry_katakana_rows)->text[(s32)name_entry_cell_column / 6 + name_entry_cell_row * 3][column * 2];
            dest[selected * 2 + 1] = ((NameKanaPages44 *)&name_entry_katakana_rows)->text[(s32)name_entry_cell_column / 6 + name_entry_cell_row * 3][((s32)name_entry_cell_column % 6) * 2 + 1];
        }
        break;
    case 2:
        if (name_entry_cell_column) {
        }
        if (name_entry_cell_column < 16) {
            index = name_entry_cell_row * 16 + name_entry_page_scroll * 16 + name_entry_cell_column;
            if (index < name_entry_candidate_counts[38]) {
                dest[selected * 2] = name_entry_candidate_lists[38][index * 2];
                dest[selected * 2 + 1] = name_entry_candidate_lists[38][index * 2 + 1];
            } else {
                dest[selected * 2] = D_800B3644[0];
                dest[selected * 2 + 1] = D_800B3644[1];
            }
        }
        break;
    case 3:
        if (name_entry_cell_column) {
        }
        if (name_entry_cell_column < 16) {
            if (name_entry_reading == 38) {
                index = name_entry_cell_row * 16 + name_entry_page_scroll * 16 + name_entry_cell_column;
                if (index < name_entry_candidate_counts[name_entry_reading]) {
                    dest[selected * 2] = name_entry_candidate_lists[name_entry_reading][index * 2];
                    dest[selected * 2 + 1] = name_entry_candidate_lists[name_entry_reading][index * 2 + 1];
                } else {
                    dest[selected * 2] = D_800B3644[0];
                    dest[selected * 2 + 1] = D_800B3644[1];
                }
            } else if (name_entry_cell_row == 0) {
                if (name_entry_cell_column < strlen(name_entry_reading_labels[name_entry_reading]) / 2) {
                    dest[selected * 2] = name_entry_reading_labels[name_entry_reading][name_entry_cell_column * 2];
                    dest[selected * 2 + 1] = name_entry_reading_labels[name_entry_reading][name_entry_cell_column * 2 + 1];
                } else {
                    dest[selected * 2] = D_800B3644[0];
                    dest[selected * 2 + 1] = D_800B3644[1];
                }
            } else {
                index = name_entry_cell_row * 16 + name_entry_page_scroll * 16 + name_entry_cell_column - 16;
                if (index < name_entry_candidate_counts[name_entry_reading]) {
                    dest[selected * 2] = name_entry_candidate_lists[name_entry_reading][index * 2];
                    dest[selected * 2 + 1] = name_entry_candidate_lists[name_entry_reading][index * 2 + 1];
                } else {
                    dest[selected * 2] = D_800B3644[0];
                    dest[selected * 2 + 1] = D_800B3644[1];
                }
            }
        }
        break;
    }
    name_entry_cursor++;
}

void name_entry_names_draw(void) {
    k_sub_reset();
    set_kanji_string(-0x68, -0x58, 0xE, name_entry_family_name, 0);
    set_kanji_string(-0x30, -0x58, 0xE, name_entry_given_name, 0);
    set_kanji_string(0x48, -0x58, 0xE, name_entry_nickname, 0);
    k_sub_disp_start(3);
}

/* FAKE: s32 result with no return statement: as a void function the build loses the original's `move v0,zero`; no caller
 * reads the result. T-20810, T-21520 */
s32 name_entry_name_finish(s32 arg0) {
    s32 i;
    s32 n;
    s32 r;

    r = rand() % 15;
    n = 0;
    if (arg0 == 0) {
        for (i = 0; i < 6; i++) {
            if (name_entry_family_name[i] == D_800B3644[i & 1]) {
                n++;
            }
        }
        if (name_entry_family_name[n] == 0) {
            strncpy(name_entry_family_name, name_entry_preset_family_names + r * 8, 8);
        }
        if (n == 6) {
            strncpy(name_entry_family_name, name_entry_preset_family_names + r * 8, 8);
        }
        if (name_entry_family_name[0] == 0) {
            strncpy(name_entry_family_name, name_entry_preset_family_names + r * 8, 8);
        }
    }
    if (arg0 == 1) {
        for (i = 0; i < 6; i++) {
            if (name_entry_given_name[i] == D_800B3644[i & 1]) {
                n++;
            }
        }
        if (name_entry_given_name[n] == 0) {
            strncpy(name_entry_given_name, name_entry_preset_given_names + r * 8, 8);
        }
        if (n == 6) {
            strncpy(name_entry_given_name, name_entry_preset_given_names + r * 8, 8);
        }
        if (name_entry_given_name[0] == 0) {
            strncpy(name_entry_given_name, name_entry_preset_given_names + r * 8, 8);
        }
    }
    if (arg0 == 2) {
        for (i = 0; i < 10; i++) {
            if (name_entry_nickname[i] == D_800B3644[i & 1]) {
                n++;
            }
        }
        if (name_entry_nickname[n] == 0) {
            strncpy(name_entry_nickname, name_entry_preset_nicknames + r * 12, 12);
        }
        if (n == 10) {
            strncpy(name_entry_nickname, name_entry_preset_nicknames + r * 12, 12);
        }
        if (name_entry_nickname[0] == 0) {
            strncpy(name_entry_nickname, name_entry_preset_nicknames + r * 12, 12);
        }
    }
}

void name_entry_cell_cursor_update(void) {
    if ((name_entry_cell_column >= 0) && (name_entry_cell_row >= 0)) {
        D_80120CD6 = name_entry_cell_column * 0x10 - 0x71;
        D_80120CDA = name_entry_cell_row * 0x10 - 0x29;
        return;
    }
    if ((name_entry_reading_column >= 0) && (name_entry_reading_row >= 0) && (name_entry_reading_column % 6 != 5)) {
        D_80120CD6 = name_entry_reading_column * 0x10 - 0x71;
        D_80120CDA = name_entry_reading_row * 0x10 + 0x3F;
        return;
    }
    D_80120CD6 = 0x100;
    D_80120CDA = 0x100;
}

void name_entry_preset_chord_check(void) {
    if (game_state.pad_held & PAD_SELECT) {
        switch (game_state.pad_pressed) {
        case PAD_L1:
            name_entry_preset_apply(0xA);
            name_entry_names_draw();
            return;
        case PAD_L2:
            name_entry_preset_apply(1);
            name_entry_names_draw();
            return;
        case PAD_R1:
            name_entry_preset_apply(2);
            name_entry_names_draw();
            return;
        case PAD_R2:
            name_entry_preset_apply(9);
            name_entry_names_draw();
            return;
        case 5:
            name_entry_preset_apply(4);
            name_entry_names_draw();
            return;
        case 10:
            name_entry_preset_apply(3);
            name_entry_names_draw();
            return;
        case 12:
            name_entry_preset_apply(5);
            name_entry_names_draw();
            return;
        case 3:
            name_entry_preset_apply(6);
            name_entry_names_draw();
            return;
        case 9:
            name_entry_preset_apply(7);
            name_entry_names_draw();
            return;
        case 6:
            name_entry_preset_apply(8);
            name_entry_names_draw();
            return;
        case 13:
            name_entry_preset_apply(0xB);
            name_entry_names_draw();
            return;
        case 7:
            name_entry_preset_apply(0xC);
            name_entry_names_draw();
            return;
        case 14:
            name_entry_preset_apply(0xD);
            name_entry_names_draw();
            return;
        case 11:
            name_entry_preset_apply(0xE);
            name_entry_names_draw();
            return;
        case 15:
            name_entry_preset_apply(0);
            name_entry_player_month = 0xB;
            name_entry_player_day = 9;
            name_entry_player_blood = 0;
            name_entry_shiori_month = 1;
            name_entry_shiori_day = 0xD;
            name_entry_shiori_blood = 3;
            name_entry_names_draw();
            break;
        }
    }
}

void name_entry_step_init(void) {
    u8 *p;

    /* u8 view of the s8 flag name_entry_retained_state (unsigned compare against 2). T-11650 */
    if (*(u8 *)&name_entry_retained_state < 2) {
        if (*(u8 *)&name_entry_retained_state == 1) {
            memcpy(name_entry_family_name, name_entry_retained_family_name, 8);
            memcpy(name_entry_given_name, name_entry_retained_given_name, 8);
            memcpy(name_entry_nickname, name_entry_retained_nickname, 0xC);
            name_entry_player_month = (u8) name_entry_retained_player_month % 12;
            name_entry_player_day = (u8) name_entry_retained_player_day % 31;
            name_entry_player_blood = (u8) name_entry_retained_player_blood & 3;
            name_entry_shiori_month = (u8) name_entry_retained_shiori_month % 12;
            name_entry_shiori_day = (u8) name_entry_retained_shiori_day % 31;
            name_entry_shiori_blood = (u8) name_entry_retained_shiori_blood & 3;
        } else {
            for (p = name_entry_family_name; p < &name_entry_family_name[6]; p += 2) {
                strcpy(p, D_800B3644);
            }
            for (p = name_entry_given_name; p < &name_entry_given_name[6]; p += 2) {
                strcpy(p, D_800B3644);
            }
            /* FAKE: initialisation on the do line gives the original as1 order of the two addiu. T-11650 */
            p = name_entry_nickname; do {
                strcpy(p, D_800B3644);
                p += 2;
            } while (p != &name_entry_nickname_end);
            name_entry_nickname_end = 0;
            name_entry_given_name[7] = 0;
            name_entry_given_name[6] = 0;
            name_entry_family_name[7] = 0;
            name_entry_family_name[6] = 0;
            name_entry_player_month = 0xFF;
            name_entry_player_day = 0xFF;
            name_entry_player_blood = 0xFF;
            name_entry_shiori_month = 0xFF;
            name_entry_shiori_day = 0xFF;
            name_entry_shiori_blood = 0xFF;
            name_entry_nickname_end_1 = 0;
            name_entry_preset_chord_check();
        }
    }
    name_entry_cursor = 0;
    name_entry_tab = 0;
    name_entry_hiragana_page_draw(name_entry_family_prompt_text);
    name_entry_reading = 0;
    name_entry_reading_column = -1;
    name_entry_reading_row = -1;
    name_entry_input_actors_init();
    cd_audio_request_set(name_entry_family_prompt_clip_start, name_entry_family_prompt_clip_end, 0);
    audio_command(0x300);
    k_sub_reset_point_set();
    name_entry_names_draw();
    *(u8 *)&name_entry_retained_state = 2;
    mode_step_next();
}

void name_entry_frames_draw(s32 arg0) {
    s32 col;

    if (arg0 != 0) {
        col = 0x90002;
    } else {
        col = 0x8081F;
    }
    prim_gradient_rect(-0x9D, -0x30, 0x1B, 0xF, 4, col, 0x83);
    prim_gradient_rect(-0x9D, -0x10, 0x1B, 0xF, 4, col, 0x83);
    prim_gradient_rect(-0x9D, 0x10, 0x1B, 0xF, 4, col, 0x83);
    prim_gradient_rect(-0x9D, 0x30, 0x1B, 0xF, 4, col, 0x83);
    prim_gradient_rect(-0x9D, 0x50, 0x1B, 0xF, 4, col, 0x83);
    prim_gradient_rect(-0x9A, -0x6A, 0x130, 0x20, 4, col, 0x83);
    prim_gradient_rect(-0x7F, -0x40, 0x11D, 0x70, 4, col, 0x83);
    if (name_entry_tab == 3) {
        prim_gradient_rect(-0x7A, 0x36, 0x50, 0x30, 4, col, 0x83);
        prim_gradient_rect(-0x1A, 0x36, 0x50, 0x30, 4, col, 0x83);
        prim_gradient_rect(0x46, 0x36, 0x50, 0x30, 4, col, 0x83);
    }
    dtd_on_tpage(0, 0, 4, 0, 0);
    if (!(game_state.pad_pressed & PAD_CIRCLE) && !(game_state.pad_pressed & PAD_START)) {
        if (!(game_state.pad_pressed & PAD_CROSS)) {
            name_entry_preset_chord_check();
        }
    }
}

/* Implicit int in the original (no return value): v0 stays live at the exit, which changes how as1 fills the delay slots. T-11650 */
s32 name_entry_scroll_arrows_update(void) {
    if (name_entry_tab == 2 || name_entry_tab == 3) {
        ((Actor *)actor_pool)[121].unk_18 = 0x19;
        ((Actor *)actor_pool)[122].unk_18 = 0x1B;
        ((Actor *)actor_pool)[121].draw_flags = 0x80;
        ((Actor *)actor_pool)[122].draw_flags = 0x80;
        ((Actor *)actor_pool)[123].draw_flags = 0;
        ((Actor *)actor_pool)[124].draw_flags = 0;
        if (((Actor *)actor_pool)[0].x >= 0x89 && ((Actor *)actor_pool)[0].x < 0x98) {
            if (((Actor *)actor_pool)[0].y >= -0x2F && ((Actor *)actor_pool)[0].y < -0x20) {
                ((Actor *)actor_pool)[121].unk_18 = 0x1A;
                if (game_state.pad_held & PAD_CIRCLE) {
                    ((Actor *)actor_pool)[123].draw_flags = 0x80;
                }
                if (game_state.pad_pressed & PAD_CIRCLE) {
                    if (name_entry_page_scroll > 0) {
                        name_entry_page_scroll--;
                    }
                    name_entry_kanji_page_draw(0);
                }
            } else if (((Actor *)actor_pool)[0].y >= 0x11 && ((Actor *)actor_pool)[0].y < 0x20) {
                ((Actor *)actor_pool)[122].unk_18 = 0x1C;
                if (game_state.pad_held & PAD_CIRCLE) {
                    ((Actor *)actor_pool)[124].draw_flags = 0x80;
                }
                if (game_state.pad_pressed & PAD_CIRCLE) {
                    name_entry_page_scroll++;
                    name_entry_kanji_page_draw(0);
                }
            }
        }
    } else {
        ((Actor *)actor_pool)[121].draw_flags = 0;
        ((Actor *)actor_pool)[122].draw_flags = 0;
        ((Actor *)actor_pool)[123].draw_flags = 0;
        ((Actor *)actor_pool)[124].draw_flags = 0;
    }
}

void name_entry_family_name_input(void) {
    hud_cursor_update();
    name_entry_cell_hit_test();
    name_entry_tab_hit_test();
    name_entry_tab_highlight_update();
    name_entry_slot_markers_update(0);
    if (name_entry_tab == 3) {
        name_entry_reading_hit_test();
        name_entry_reading_select();
    }
    name_entry_scroll_arrows_update();
    if (game_state.pad_pressed & PAD_CROSS) {
        /* FAKE: address-taken u8 view of the (already u8) selector keeps it in $v0 as in the original; real source unknown. T-11650 */
        if (*(u8 *)&name_entry_cursor == 0xC) {
            *(u8 *)&name_entry_cursor = 2;
        } else if (*(u8 *)&name_entry_cursor != 0) {
            *(u8 *)&name_entry_cursor -= 1;
        }
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        if (name_entry_cursor < 3 && name_entry_cell_column >= 0 && name_entry_cell_row >= 0) {
            name_entry_character_put(0);
            name_entry_names_draw();
        }
        if (name_entry_tab == 3) {
            name_entry_kanji_page_draw(1);
            k_sub_reset_point_set();
            name_entry_names_draw();
        } else if (name_entry_tab == 2) {
            name_entry_symbol_page_draw(name_entry_family_prompt_text);
            k_sub_reset_point_set();
            name_entry_names_draw();
        }
        if (name_entry_tab_hover == 0) {
            name_entry_tab = 0;
            name_entry_hiragana_page_draw(name_entry_family_prompt_text);
            k_sub_reset_point_set();
            name_entry_names_draw();
        }
        if (name_entry_tab_hover == 1) {
            name_entry_tab = 1;
            name_entry_katakana_page_draw(name_entry_family_prompt_text);
            k_sub_reset_point_set();
            name_entry_names_draw();
        }
        if (name_entry_tab_hover == 2) {
            name_entry_tab = 2;
            name_entry_symbol_page_draw(name_entry_family_prompt_text);
            k_sub_reset_point_set();
            name_entry_names_draw();
        }
        if (name_entry_tab_hover == 3) {
            name_entry_tab = 3;
            name_entry_kanji_page_draw(1);
            k_sub_reset_point_set();
            name_entry_names_draw();
        }
        if (name_entry_tab_hover == 4) {
            name_entry_tab = 4;
            name_entry_name_finish(0);
            name_entry_names_draw();
            audio_command(0x501);
            mode_step_next();
        }
    } else if (game_state.pad_pressed & PAD_START) {
        name_entry_name_finish(0);
        name_entry_names_draw();
        audio_command(0x501);
        mode_step_next();
    }
}

void name_entry_given_name_init(void) {

    text_system_reset();
    if (name_entry_tab == 4) {
        name_entry_tab = 0;
    }
    switch (name_entry_tab) {
    case 0:
        name_entry_hiragana_page_draw((s32) name_entry_given_prompt_text);
        break;
    case 1:
        name_entry_katakana_page_draw((s32) name_entry_given_prompt_text);
        break;
    case 2:
        name_entry_symbol_page_draw((s32) name_entry_given_prompt_text);
        break;
    case 3:
        name_entry_kanji_page_draw(1);
        break;
    }
    name_entry_cursor = 3;
    D_801206AC = 0;
    D_801206BA = 0;
    D_801206BE = 0;
    cd_audio_request_set(name_entry_given_prompt_clip_start, name_entry_given_prompt_clip_end, 0);
    audio_command(0x300);
    k_sub_reset_point_set();
    name_entry_names_draw();
    mode_step_next();
}

void name_entry_given_name_input(void) {
    u8 cur;
    u8 temp;

    hud_cursor_update();
    name_entry_cell_hit_test();
    name_entry_tab_hit_test();
    name_entry_tab_highlight_update();
    name_entry_slot_markers_update(1);
    if (name_entry_tab == 3) {
        name_entry_reading_hit_test();
        name_entry_reading_select();
    }
    name_entry_scroll_arrows_update();
    if (game_state.pad_pressed & PAD_CROSS) {
        cur = (*(u16 *)&name_entry_cursor);
        if (cur == 0xC) {
            cur = 6;
        }
        name_entry_cursor = cur;
        if (cur >= 4) {
            name_entry_cursor = cur - 1;
        }
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        temp = name_entry_cursor;
        if (temp < 6 && name_entry_cell_column >= 0 && name_entry_cell_row >= 0) {
            name_entry_character_put(1);
            name_entry_names_draw();
        }
        if (name_entry_tab == 3) {
            name_entry_kanji_page_draw(2);
            k_sub_reset_point_set();
            name_entry_names_draw();
        } else if (name_entry_tab == 2) {
            name_entry_symbol_page_draw(name_entry_given_prompt_text);
            k_sub_reset_point_set();
            name_entry_names_draw();
        }
        if (name_entry_tab_hover == 0) {
            name_entry_tab = 0;
            name_entry_hiragana_page_draw(name_entry_given_prompt_text);
            k_sub_reset_point_set();
            name_entry_names_draw();
        }
        if (name_entry_tab_hover == 1) {
            name_entry_tab = 1;
            name_entry_katakana_page_draw(name_entry_given_prompt_text);
            k_sub_reset_point_set();
            name_entry_names_draw();
        }
        if (name_entry_tab_hover == 2) {
            name_entry_tab = 2;
            name_entry_symbol_page_draw(name_entry_given_prompt_text);
            k_sub_reset_point_set();
            name_entry_names_draw();
        }
        if (name_entry_tab_hover == 3) {
            name_entry_tab = 3;
            name_entry_kanji_page_draw(2);
            k_sub_reset_point_set();
            name_entry_names_draw();
        }
        if (name_entry_tab_hover == 4) {
            name_entry_tab = 4;
            name_entry_name_finish(1);
            name_entry_names_draw();
            audio_command(0x501);
            mode_step_next();
        }
    } else if (game_state.pad_pressed & PAD_START) {
        name_entry_name_finish(1);
        name_entry_names_draw();
        audio_command(0x501);
        mode_step_next();
    }
}

void name_entry_nickname_init(void) {

    text_system_reset();
    if (name_entry_tab == 4) {
        name_entry_tab = 0;
    }
    switch (name_entry_tab) {
    case 0:
        name_entry_hiragana_page_draw((s32) name_entry_nickname_prompt_text);
        break;
    case 1:
        name_entry_katakana_page_draw((s32) name_entry_nickname_prompt_text);
        break;
    case 2:
        name_entry_symbol_page_draw((s32) name_entry_nickname_prompt_text);
        break;
    case 3:
        name_entry_kanji_page_draw(2);
        break;
    }
    name_entry_cursor = 6;
    D_801206AC = 0;
    D_801206BA = 0;
    D_801206BE = 0;
    cd_audio_request_set(name_entry_nickname_prompt_clip_start, name_entry_nickname_prompt_clip_end, 0);
    audio_command(0x300);
    k_sub_reset_point_set();
    name_entry_names_draw();
    mode_step_next();
}

void name_entry_nickname_input(void) {
    hud_cursor_update();
    name_entry_cell_hit_test();
    name_entry_tab_hit_test();
    name_entry_tab_highlight_update();
    name_entry_slot_markers_update(2);
    if (name_entry_tab == 3) {
        name_entry_reading_hit_test();
        name_entry_reading_select();
    }
    name_entry_scroll_arrows_update();
    if (game_state.pad_pressed & PAD_CROSS) {
        /* FAKE: same u8 view of the selector as in name_entry_family_name_input. T-11650 */
        if (*(u8 *)&name_entry_cursor == 0xC) {
            *(u8 *)&name_entry_cursor = 0xA;
        } else if (*(u8 *)&name_entry_cursor >= 7) {
            *(u8 *)&name_entry_cursor -= 1;
        }
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        if (name_entry_cursor < 0xB && name_entry_cell_column >= 0 && name_entry_cell_row >= 0) {
            name_entry_character_put(2);
            name_entry_names_draw();
        }
        if (name_entry_tab == 3) {
            name_entry_kanji_page_draw(3);
            k_sub_reset_point_set();
            name_entry_names_draw();
        } else if (name_entry_tab == 2) {
            name_entry_symbol_page_draw(name_entry_nickname_prompt_text);
            k_sub_reset_point_set();
            name_entry_names_draw();
        }
        if (name_entry_tab_hover == 0) {
            name_entry_tab = 0;
            name_entry_hiragana_page_draw(name_entry_nickname_prompt_text);
            k_sub_reset_point_set();
            name_entry_names_draw();
        }
        if (name_entry_tab_hover == 1) {
            name_entry_tab = 1;
            name_entry_katakana_page_draw(name_entry_nickname_prompt_text);
            k_sub_reset_point_set();
            name_entry_names_draw();
        }
        if (name_entry_tab_hover == 2) {
            name_entry_tab = 2;
            name_entry_symbol_page_draw(name_entry_nickname_prompt_text);
            k_sub_reset_point_set();
            name_entry_names_draw();
        }
        if (name_entry_tab_hover == 3) {
            name_entry_tab = 3;
            name_entry_kanji_page_draw(3);
            k_sub_reset_point_set();
            name_entry_names_draw();
        }
        if (name_entry_tab_hover == 4) {
            name_entry_tab = 4;
            name_entry_name_finish(2);
            name_entry_names_draw();
            audio_command(0x501);
            mode_step_next();
        }
    } else if (game_state.pad_pressed & PAD_START) {
        name_entry_name_finish(2);
        name_entry_names_draw();
        audio_command(0x501);
        mode_step_next();
    }
}

void name_entry_birthday_frames_draw(s32 arg0) {
    s32 color;

    if (arg0 != 0) {
        color = 0x90002;
    } else {
        color = 0x8081F;
    }
    prim_gradient_rect(-0x9A, -0x6A, 0x130, 0x20, 4, color, 0x83);
    prim_gradient_rect(-0x7C, -0x3D, 0x118, 0x9E, 4, color, 0x83);
    prim_gradient_rect(-0x9D, 0x50, 0x1C, 0x10, 4, color, 0x83);
}

void name_entry_birthday_init(void) {
    text_system_reset();
    name_entry_birthday_actors_init();
    name_entry_cursor = 0;
    set_kanji_string(-0x98, -0x68, 0xA, "誕生日と血液型も教えてね？", 0);
    cd_audio_request_set(name_entry_birthday_prompt_clip_start, name_entry_birthday_prompt_clip_end, 0);
    audio_command(0x300);
    k_sub_reset_point_set();
    name_entry_birthday_text_draw(0);
    k_sub_disp_start(0);
    mode_step_next();
}

void name_entry_birthday_actors_init(void) {
    s32 i;

    actor_clear_upper();
    for (i = 0; i < 12; i++) {
        ((Actor *)actor_pool)[96 + i].unk_01 = 2;
        ((Actor *)actor_pool)[96 + i].flags = 0x40;
        ((Actor *)actor_pool)[96 + i].quad_flags = 0;
        ((Actor *)actor_pool)[96 + i].draw_flags = 0x84;
        ((Actor *)actor_pool)[96 + i].frame_table = (u8 *)game_state.ovl_frame_table;
        ((Actor *)actor_pool)[96 + i].sequence_table = (u8 *)game_state.ovl_sequence_table;
        ((Actor *)actor_pool)[96 + i].tpage_table = (u8 *)game_state.ovl_tpage_table;
        ((Actor *)actor_pool)[96 + i].unk_04 = 8;
        ((Actor *)actor_pool)[96 + i].unk_05 = 0x81;
        ((Actor *)actor_pool)[96 + i].unk_43 = 0x10;
        ((Actor *)actor_pool)[96 + i].x = 0x190;
    }
    ((Actor *)actor_pool)[96].unk_18 = 1;
    ((Actor *)actor_pool)[96].x = 0;
    ((Actor *)actor_pool)[96].y = 0;
    ((Actor *)actor_pool)[97].unk_18 = 0xD;
    ((Actor *)actor_pool)[97].x = -0x48;
    ((Actor *)actor_pool)[97].y = -0x4A;
    ((Actor *)actor_pool)[98].unk_18 = 0xD;
    ((Actor *)actor_pool)[98].x = -0x38;
    ((Actor *)actor_pool)[98].y = -0x4A;
    ((Actor *)actor_pool)[99].unk_18 = 0xD;
    ((Actor *)actor_pool)[99].x = -0x18;
    ((Actor *)actor_pool)[99].y = -0x4A;
    ((Actor *)actor_pool)[100].unk_18 = 0xD;
    ((Actor *)actor_pool)[100].x = -8;
    ((Actor *)actor_pool)[100].y = -0x4A;
    ((Actor *)actor_pool)[101].unk_18 = 0xD;
    ((Actor *)actor_pool)[101].x = 0x68;
    ((Actor *)actor_pool)[101].y = -0x4A;
    ((Actor *)actor_pool)[102].unk_18 = 0xD;
    ((Actor *)actor_pool)[102].x = 0x78;
    ((Actor *)actor_pool)[102].y = -0x4A;
    ((Actor *)actor_pool)[103].unk_18 = 9;
    ((Actor *)actor_pool)[103].x = -0x8F;
    ((Actor *)actor_pool)[103].y = 0x58;
    ((Actor *)actor_pool)[103].unk_01 = 1;
    ((Actor *)actor_pool)[104].unk_18 = 5;
    ((Actor *)actor_pool)[104].x = 0;
    ((Actor *)actor_pool)[104].y = 0;
    ((Actor *)actor_pool)[104].unk_01 = 3;
    ((Actor *)actor_pool)[105].unk_18 = 0x17;
    ((Actor *)actor_pool)[105].x = -0x8F;
    ((Actor *)actor_pool)[105].y = 0x58;
    ((Actor *)actor_pool)[106].unk_18 = 7;
    ((Actor *)actor_pool)[106].x = -0x8F;
    ((Actor *)actor_pool)[106].y = 0x58;
    ((Actor *)actor_pool)[106].draw_flags &= 0x7F;
    ((Actor *)actor_pool)[107].unk_18 = 9;
    ((Actor *)actor_pool)[107].x = -0x8F;
    ((Actor *)actor_pool)[107].y = -0x28;
    ((Actor *)actor_pool)[107].unk_01 = 1;
}

void name_entry_actor_animation_start(s32 arg0) {
    u8 *p = actor_pool + arg0 * 0x44;

    if (!(p[2] & 1)) {
        *(s16 *)(p + 0x18) = 0;
        p[2] = 1;
    }
}

void name_entry_birthday_field_marks_update(void) {
    u8 *p;
    s32 i;

    if (name_entry_cursor == 0) {
        name_entry_actor_animation_start(0x61);
        name_entry_actor_animation_start(0x62);
        ((Actor *)actor_pool)[99].unk_18 = 0xD;
        ((Actor *)actor_pool)[99].flags = 0x40;
        ((Actor *)actor_pool)[100].unk_18 = 0xD;
        ((Actor *)actor_pool)[100].flags = 0x40;
        ((Actor *)actor_pool)[101].unk_18 = 0xD;
        ((Actor *)actor_pool)[101].flags = 0x40;
        ((Actor *)actor_pool)[102].unk_18 = 0xD;
        ((Actor *)actor_pool)[102].flags = 0x40;
    } else if (name_entry_cursor == 1) {
        name_entry_actor_animation_start(0x63);
        name_entry_actor_animation_start(0x64);
        for (i = 0; i < 2; i++) {
            p = actor_pool + i * 0x44;
            *(s16 *)(p + 0x19DC) = 0xD;
            p[0x19C6] = 0x40;
        }
        for (i = 4; i < 6; i++) {
            p = actor_pool + i * 0x44;
            *(s16 *)(p + 0x19DC) = 0xD;
            p[0x19C6] = 0x40;
        }
    } else if (name_entry_cursor == 2) {
        ((Actor *)actor_pool)[97].unk_18 = 0xD;
        ((Actor *)actor_pool)[97].flags = 0x40;
        ((Actor *)actor_pool)[98].unk_18 = 0xD;
        ((Actor *)actor_pool)[98].flags = 0x40;
        ((Actor *)actor_pool)[99].unk_18 = 0xD;
        ((Actor *)actor_pool)[99].flags = 0x40;
        ((Actor *)actor_pool)[100].unk_18 = 0xD;
        ((Actor *)actor_pool)[100].flags = 0x40;
        name_entry_actor_animation_start(0x65);
        name_entry_actor_animation_start(0x66);
    }
}

void name_entry_birthday_hit_test(s32 arg0) {
    s32 i;
    s32 x;
    s32 mouse_x;
    s32 y;

    if (name_entry_cursor == 0) {
        mouse_x = ((Actor *)actor_pool)[0].x;
        ((Actor *)actor_pool)[104].unk_18 = 5;
        ((Actor *)actor_pool)[104].x = -256;
        ((Actor *)actor_pool)[104].y = -256;
        for (i = 0; i < 12; i++) {
            x = (i % 6) * 40 - 104;
            y = (i / 6) * 16 - 40;
            if (x < mouse_x && mouse_x < x + 32 &&
                y < ((Actor *)actor_pool)[0].y && ((Actor *)actor_pool)[0].y < y + 16) {
                ((Actor *)actor_pool)[104].x = x + 22;
                ((Actor *)actor_pool)[104].y = y + 6;
                if (arg0 != 0) {
                    name_entry_shiori_month = i;
                } else {
                    name_entry_player_month = i;
                }
            }
        }
    } else if (name_entry_cursor == 1) {
        mouse_x = ((Actor *)actor_pool)[0].x;
        ((Actor *)actor_pool)[104].unk_18 = 4;
        ((Actor *)actor_pool)[104].x = -256;
        ((Actor *)actor_pool)[104].y = -256;
        for (i = 0; i < 31; i++) {
            x = (i % 11) * 24 - 104;
            y = (i / 11) * 16 + 8;
            if (x - 16 < mouse_x && mouse_x < x + 16 &&
                y < ((Actor *)actor_pool)[0].y && ((Actor *)actor_pool)[0].y < y + 16) {
                ((Actor *)actor_pool)[104].x = x + 1;
                ((Actor *)actor_pool)[104].y = y + 6;
                if (arg0 != 0) {
                    name_entry_shiori_day = i;
                } else {
                    name_entry_player_day = i;
                }
            }
        }
    } else if (name_entry_cursor == 2) {
        mouse_x = ((Actor *)actor_pool)[0].x;
        ((Actor *)actor_pool)[104].unk_18 = 5;
        ((Actor *)actor_pool)[104].x = -256;
        ((Actor *)actor_pool)[104].y = -256;
        for (i = 0; i < 4; i++) {
            x = i * 64 - 96;
            if (x < mouse_x && mouse_x < x + 32 &&
                72 < ((Actor *)actor_pool)[0].y && ((Actor *)actor_pool)[0].y < 88) {
                ((Actor *)actor_pool)[104].x = x + 16;
                ((Actor *)actor_pool)[104].y = 78;
                if (arg0 != 0) {
                    name_entry_shiori_blood = i;
                } else {
                    name_entry_player_blood = i;
                }
            }
        }
    }
}

void name_entry_birthday_text_draw(arg0)
s32 arg0;
{
    s32 a;
    s32 b;
    s32 c;

    if (arg0 != 0) {
        a = name_entry_shiori_month;
        b = name_entry_shiori_day;
        c = name_entry_shiori_blood;
    } else {
        a = name_entry_player_month;
        b = name_entry_player_day;
        c = name_entry_player_blood;
    }
    k_sub_reset();
    if (a == 0xFF) {
        set_kanji_string(-0x50, -0x58, 0xE, "　", 0);
        set_kanji_string(-0x40, -0x58, 0xE, "　", 0);
    } else {
        if (a >= 9) {
            set_kanji_string(-0x50, -0x58, 0xE, D_800B3288[1], 0);
        } else {
            set_kanji_string(-0x50, -0x58, 0xE, "　", 0);
        }
        set_kanji_string(-0x40, -0x58, 0xE, D_800B3288[(a + 1) % 10], 0);
    }
    if (b == 0xFF) {
        set_kanji_string(-0x20, -0x58, 0xE, "　", 0);
        set_kanji_string(-0x10, -0x58, 0xE, "　", 0);
    } else {
        if (b >= 9) {
            set_kanji_string(-0x20, -0x58, 0xE, D_800B3288[(b + 1) / 10], 0);
        } else {
            set_kanji_string(-0x20, -0x58, 0xE, D_800B3644, 0);
        }
        set_kanji_string(-0x10, -0x58, 0xE, D_800B3288[(b + 1) % 10], 0);
    }
    switch (c) {
    case 0:
        set_kanji_string(0x60, -0x58, 0xE, "　", 0);
        set_kanji_string(0x70, -0x58, 0xE, "Ａ", 0);
        break;
    case 1:
        set_kanji_string(0x60, -0x58, 0xE, "　", 0);
        set_kanji_string(0x70, -0x58, 0xE, "Ｂ", 0);
        break;
    case 2:
        set_kanji_string(0x60, -0x58, 0xE, "Ａ", 0);
        set_kanji_string(0x70, -0x58, 0xE, "Ｂ", 0);
        break;
    case 3:
        set_kanji_string(0x60, -0x58, 0xE, "　", 0);
        set_kanji_string(0x70, -0x58, 0xE, "Ｏ", 0);
        break;
    }
    k_sub_disp_start(6);
}

void name_entry_birthday_buttons_update(s32 arg0) {
    s32 pad; /* FAKE: unused locals around the two cursor values, which the original reads without ever assigning them (T-3330 layout) */
    s32 sp30;
    s32 sp2C;
    s32 pad2;

    if (sp30 >= -0x9D && sp30 < -0x80 && sp2C >= 0x51 && sp2C < 0x60) {
        D_8012093F |= 0x80;
        if (game_state.pad_pressed & PAD_CIRCLE) {
            name_entry_birthday_finalize();
            mode_step_next();
        }
    } else {
        D_8012093F &= 0x7F;
    }
    if (name_entry_cursor == 3) {
        D_801208FB |= 0x80;
        if (game_state.pad_pressed & PAD_CIRCLE) {
            name_entry_birthday_finalize(arg0);
            mode_step_next();
        }
    } else {
        D_801208FB &= 0x7F;
    }
}

void name_entry_birthday_input(void) {
    u8 temp;
    hud_cursor_update();
    name_entry_birthday_field_marks_update();
    name_entry_birthday_text_draw(0);
    name_entry_birthday_buttons_update(0);
    if (game_state.pad_pressed & PAD_CROSS) {
        temp = name_entry_cursor;
        if (temp != 0) {
            name_entry_cursor -= 1;
        }
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        name_entry_cursor += 1;
        name_entry_birthday_text_draw(0);
        if ((u8) name_entry_cursor >= 3U) {
            name_entry_birthday_finalize(0);
            mode_step_next();
        }
    } else if (game_state.pad_pressed & PAD_START) {
        name_entry_birthday_finalize(0);
        mode_step_next();
    }
    name_entry_day_grid_numbers_draw();
}

void name_entry_birthday_finalize(arg0)
s32 arg0;
{
    s32 a;

    if (arg0 != 0) {
        if (name_entry_shiori_month == 0xFF) {
            name_entry_shiori_month = rand() % 12;
        }
        if (name_entry_shiori_day == 0xFF) {
            name_entry_shiori_day = rand() % 31;
        }
        if (name_entry_shiori_blood == 0xFF) {
            name_entry_shiori_blood = rand() % 3;
        }
        a = name_entry_shiori_month + 1;
        if (a == 2 && name_entry_shiori_day + 1 >= 0x1D) {
            name_entry_shiori_day = 0x1B;
        }
        if (a == 4 && name_entry_shiori_day + 1 >= 0x1F) {
            name_entry_shiori_day = 0x1D;
        }
        if (a == 6 && name_entry_shiori_day + 1 >= 0x1F) {
            name_entry_shiori_day = 0x1D;
        }
        if (a == 9 && name_entry_shiori_day + 1 >= 0x1F) {
            name_entry_shiori_day = 0x1D;
        }
        if (a == 0xB && name_entry_shiori_day + 1 >= 0x1F) {
            name_entry_shiori_day = 0x1D;
        }
    } else {
        if (name_entry_player_month == 0xFF) {
            name_entry_player_month = rand() % 12;
        }
        if (name_entry_player_day == 0xFF) {
            name_entry_player_day = rand() % 31;
        }
        if (name_entry_player_blood == 0xFF) {
            name_entry_player_blood = rand() % 3;
        }
        a = name_entry_player_month + 1;
        if (a == 2 && name_entry_player_day + 1 >= 0x1D) {
            name_entry_player_day = 0x1B;
        }
        if (a == 4 && name_entry_player_day + 1 >= 0x1F) {
            name_entry_player_day = 0x1D;
        }
        if (a == 6 && name_entry_player_day + 1 >= 0x1F) {
            name_entry_player_day = 0x1D;
        }
        if (a == 9 && name_entry_player_day + 1 >= 0x1F) {
            name_entry_player_day = 0x1D;
        }
        if (a == 0xB && name_entry_player_day + 1 >= 0x1F) {
            name_entry_player_day = 0x1D;
        }
    }
}

void name_entry_day_grid_numbers_draw(void) {
    s32 i;
    s32 y;

    for (i = 1, y = -0x70; i < 0xA; i++, y += 0x18) {
        name_entry_digit_sprite_draw(i, y, 8);
    }
    for (i = 0, y = 0x70; i < 2; i++, y += 0x18) {
        name_entry_number_sprites_draw(i + 0xA, y, 8);
    }
    for (i = 0, y = -0x68; i < 0xB; i++, y += 0x18) {
        name_entry_number_sprites_draw(i + 0xC, y, 0x18);
    }
    for (i = 0, y = -0x68; i != 9; i++, y += 0x18) {
        name_entry_number_sprites_draw(i + 0x17, y, 0x28);
    }
    goto_tpage(0x140, 0, 2, 0, 0);
}

void name_entry_digit_sprite_draw(s32 arg0, s32 arg1, s32 arg2) {
    u8 *p;
    u8 *q;

    p = GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(p);
    SetSemiTrans(p, 0);
    p[4] = game_state.screen_brightness;
    p[5] = game_state.screen_brightness;
    p[6] = game_state.screen_brightness;
    *(s16 *)(p + 8) = game_state.screen_center_x[draw_buffer] + arg1;
    *(s16 *)(p + 0xA) = game_state.screen_center_y[draw_buffer] + arg2;
    q = name_entry_wide_digit_uv + arg0 * 2;
    p[0xC] = q[0];
    p[0xD] = q[1];
    *(s16 *)(p + 0x12) = 0x10;
    *(s16 *)(p + 0x10) = 0x10;
    *(s16 *)(p + 0xE) = func_8009ED74(0x10, 0x1F0);
    AddPrim((draw_buffer << 10) + 8 + D_800E8CA0, p);
}

void name_entry_number_sprites_draw(s32 arg0, s32 arg1, s32 arg2) {
    SPRT *sprite;
    u8 *uv;

    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 0);
    sprite->r0 = game_state.screen_brightness;
    sprite->g0 = game_state.screen_brightness;
    sprite->b0 = game_state.screen_brightness;
    sprite->x0 = game_state.screen_center_x[draw_buffer] + arg1 - 8;
    sprite->y0 = game_state.screen_center_y[draw_buffer] + arg2;
    uv = &name_entry_small_digit_uv[arg0 / 10 * 2];
    sprite->u0 = uv[0];
    sprite->v0 = uv[1];
    setWH(sprite, 8, 0x10);
    sprite->clut = func_8009ED74(0x10, 0x1F0);
    AddPrim((draw_buffer << 10) + 8 + D_800E8CA0, (s32)sprite);
    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 0);
    sprite->r0 = game_state.screen_brightness;
    sprite->g0 = game_state.screen_brightness;
    sprite->b0 = game_state.screen_brightness;
    sprite->x0 = game_state.screen_center_x[draw_buffer] + arg1;
    sprite->y0 = game_state.screen_center_y[draw_buffer] + arg2;
    uv = &name_entry_small_digit_uv[arg0 % 10 * 2];
    sprite->u0 = uv[0];
    sprite->v0 = uv[1];
    setWH(sprite, 8, 0x10);
    sprite->clut = func_8009ED74(0x10, 0x1F0);
    AddPrim((draw_buffer << 10) + 8 + D_800E8CA0, (s32)sprite);
}

void name_entry_confirm_actors_init(void) {
    u8 *p;
    s32 i;

    actor_clear_upper();
    for (i = 0; i < 2; i++) {
        p = actor_pool + i * 0x44;
        p[0x1981] = 2;
        p[0x1982] = 0x40;
        *(s32 *)(p + 0x19B8) = 0;
        p[0x1983] = 0x84;
        *(s32 *)(p + 0x198C) = game_state.ovl_frame_table;
        *(s32 *)(p + 0x1990) = game_state.ovl_sequence_table;
        *(s32 *)(p + 0x19B4) = game_state.ovl_tpage_table;
        p[0x1984] = 8;
        p[0x1985] = 0x81;
        p[0x19C3] = 0x10;
        *(s16 *)(p + 0x19A6) = 0x190;
    }
    D_80120668 = 2;
    D_80120676 = 0;
    D_8012067A = 0;
    D_801206AC = 6;
    D_801206BA = -0xA;
    D_801206BE = 0x58;
    D_80120695 = 3;
}

void name_entry_confirm_init(void) {
    text_system_reset();
    name_entry_confirm_actors_init();
    set_kanji_string(-0x18, 0x40, 0xA, "これでいい？", 0);
    set_kanji_string(-0x38, -0x37, 0xA, name_entry_family_name, 0);
    set_kanji_string(0, -0x37, 0xA, name_entry_given_name, 0);
    set_kanji_string(-0x38, -0x17, 0xA, name_entry_nickname, 0);
    if (name_entry_player_month + 1 >= 10) {
        set_kanji_string(-0x38, 8, 0xA, D_800B3288[(name_entry_player_month + 1) / 10], 0);
    }
    set_kanji_string(-0x28, 8, 0xA, D_800B3288[(name_entry_player_month + 1) % 10], 0);
    if (name_entry_player_day + 1 >= 10) {
        set_kanji_string(-8, 8, 0xA, D_800B3288[(name_entry_player_day + 1) / 10], 0);
    }
    set_kanji_string(8, 8, 0xA, D_800B3288[(name_entry_player_day + 1) % 10], 0);
    switch (name_entry_player_blood) {
    case 0:
        set_kanji_string(-0x28, 0x28, 0xA, "Ａ", 0);
        break;
    case 1:
        set_kanji_string(-0x28, 0x28, 0xA, "Ｂ", 0);
        break;
    case 2:
        set_kanji_string(-0x38, 0x28, 0xA, "ＡＢ", 0);
        break;
    case 3:
        set_kanji_string(-0x28, 0x28, 0xA, "Ｏ", 0);
        break;
    }
    cd_audio_request_set(name_entry_confirm_clip_start, name_entry_confirm_clip_end, 0);
    audio_command(0x300);
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    mode_step_next();
}

void name_entry_confirm_frames_draw(s32 arg0) {
    s32 color;

    if (arg0 != 0) {
        color = 0x90002;
    } else {
        color = 0x8081F;
    }
    if (arg0 != 0) {
        prim_gradient_rect(-0x7E, -0x3D, 0x11A, 0x58, 4, color, 0x83);
    } else {
        prim_gradient_rect(-0x7E, -0x3D, 0x11A, 0x76, 4, color, 0x83);
    }
    prim_gradient_rect(-0x28, 0x3B, 0x7C, 0x26, 4, color, 0x83);
}

void name_entry_empty_callback(void) {
}

void name_entry_confirm_input(void) {
    s32 pad; /* FAKE: unused local, the original frame has the spill slots 4 bytes higher (T-3330) */
    s32 x;
    s32 y;

    x = cursor_x;
    y = cursor_y;
    hud_cursor_update();
    if (x >= -0x2F && x < 0x10 && y >= 0x51 && y < 0x60) {
        D_801206BA = -0xA;
    }
    if (x >= 0x11 && x < 0x50 && y >= 0x51 && y < 0x60) {
        D_801206BA = 0x2E;
    }
    if (game_state.pad_pressed & PAD_START) {
        audio_command(0x501);
        mode_stage_next();
    } else if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        if (x >= -0x2F && x < 0x10 && y >= 0x51 && y < 0x60) {
            audio_command(0x501);
            mode_stage_next();
        }
        if (x >= 0x11 && x < 0x50 && y >= 0x51 && y < 0x60) {
            mode_step_set(0);
        }
    }
}

void name_entry_mode_stage_shiori(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        name_entry_shiori_birthday_init();
        break;
    case 1:
        name_entry_shiori_birthday_input();
        break;
    case 2:
        name_entry_shiori_confirm_init();
        break;
    case 3:
        name_entry_shiori_confirm_input();
        break;
    case 4:
        name_entry_commit();
        break;
    case 5:
        name_entry_exit_to_prologue();
        break;
    }
    if (game_state.mode_step < 4) {
        if (game_state.mode_step < 2) {
            name_entry_birthday_frames_draw(1);
            name_entry_birthday_hit_test(1);
        } else {
            name_entry_confirm_frames_draw(1);
            name_entry_empty_callback();
        }
    }
}

void name_entry_shiori_birthday_init(void) {
    text_system_reset();
    name_entry_cursor = 0;
    set_kanji_string(-0x98, -0x68, 0xA, "藤崎詩織の誕生日と血液型は覚えてる？", 0);
    name_entry_birthday_actors_init();
    cd_audio_request_set(name_entry_shiori_prompt_clip_start, name_entry_shiori_prompt_clip_end, 0);
    audio_command(0x300);
    k_sub_reset_point_set();
    name_entry_birthday_text_draw(1);
    k_sub_disp_start(0);
    mode_step_next();
}

void name_entry_shiori_birthday_input(void) {
    u8 temp;
    hud_cursor_update();
    name_entry_birthday_field_marks_update();
    name_entry_birthday_text_draw(1);
    name_entry_birthday_buttons_update(1);
    if (game_state.pad_pressed & PAD_CROSS) {
        temp = name_entry_cursor;
        if (temp != 0) {
            name_entry_cursor -= 1;
        }
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        name_entry_cursor += 1;
        name_entry_birthday_text_draw(1);
        if ((u8) name_entry_cursor >= 3U) {
            name_entry_birthday_finalize(1);
            mode_step_next();
        }
    } else if (game_state.pad_pressed & PAD_START) {
        name_entry_birthday_finalize(1);
        mode_step_next();
    }
    name_entry_day_grid_numbers_draw();
}

void name_entry_shiori_confirm_actors_init(void) {
    u8 *p;
    s32 i;

    actor_clear_upper();
    for (i = 0; i < 2; i++) {
        p = actor_pool + i * 0x44;
        p[0x1981] = 2;
        p[0x1982] = 0x40;
        *(s32 *)(p + 0x19B8) = 0;
        p[0x1983] = 0x84;
        *(s32 *)(p + 0x198C) = game_state.ovl_frame_table;
        *(s32 *)(p + 0x1990) = game_state.ovl_sequence_table;
        *(s32 *)(p + 0x19B4) = game_state.ovl_tpage_table;
        p[0x1984] = 8;
        p[0x1985] = 0x81;
        p[0x19C3] = 0x10;
        *(s16 *)(p + 0x19A6) = 0x190;
    }
    D_80120668 = 0xE;
    D_80120676 = 0;
    D_8012067A = 0;
    D_801206AC = 6;
    D_801206BA = -0xA;
    D_801206BE = 0x58;
    D_80120695 = 3;
}

void name_entry_shiori_confirm_init(void) {
    text_system_reset();
    set_kanji_string(-0x18, 0x40, 0xA, "これでいい？", 0);
    set_kanji_string(-0x38, -0x38, 0xA, "藤崎", 0);
    set_kanji_string(0, -0x38, 0xA, "詩織", 0);
    if (name_entry_shiori_month + 1 >= 10) {
        set_kanji_string(-0x38, -0x18, 0xA, D_800B3288[(name_entry_shiori_month + 1) / 10], 0);
    }
    set_kanji_string(-0x28, -0x18, 0xA, D_800B3288[(name_entry_shiori_month + 1) % 10], 0);
    if (name_entry_shiori_day + 1 >= 10) {
        set_kanji_string(-8, -0x18, 0xA, D_800B3288[(name_entry_shiori_day + 1) / 10], 0);
    }
    set_kanji_string(8, -0x18, 0xA, D_800B3288[(name_entry_shiori_day + 1) % 10], 0);
    switch (name_entry_shiori_blood) {
    case 0:
        set_kanji_string(-0x28, 8, 0xA, "Ａ", 0);
        break;
    case 1:
        set_kanji_string(-0x28, 8, 0xA, "Ｂ", 0);
        break;
    case 2:
        set_kanji_string(-0x38, 8, 0xA, "ＡＢ", 0);
        break;
    case 3:
        set_kanji_string(-0x28, 8, 0xA, "Ｏ", 0);
        break;
    }
    name_entry_shiori_confirm_actors_init();
    cd_audio_request_set(name_entry_confirm_clip_start, name_entry_confirm_clip_end, 0);
    audio_command(0x300);
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    mode_step_next();
}

void name_entry_shiori_confirm_input(void) {
    s32 pad; /* FAKE: unused local, the original frame has the spill slots 4 bytes higher (T-3330) */
    s32 x;
    s32 y;

    x = cursor_x;
    y = cursor_y;
    hud_cursor_update();
    if (x >= -0x2F && x < 0x10 && y >= 0x51 && y < 0x60) {
        D_801206BA = -0xA;
    }
    if (x >= 0x11 && x < 0x50 && y >= 0x51 && y < 0x60) {
        D_801206BA = 0x2E;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if (x >= -0x2F && x < 0x10 && y >= 0x51 && y < 0x60) {
            audio_command(0x501);
            mode_step_next();
        }
        if (x >= 0x11 && x < 0x50 && y >= 0x51 && y < 0x60) {
            mode_step_set(0);
        }
    } else if (game_state.pad_pressed & PAD_START) {
        audio_command(0x501);
        mode_step_next();
    } else if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
    }
}

void name_entry_commit(void) {
    audio_command(5);
    audio_command(0xC1);
    audio_command(3);
    actor_clear_upper();
    name_entry_new_game_setup();
    text_system_reset();
    mode_step_next();
}

/* FAKE: s32 with no return statement, as declared in NAME_ENT.h: with a void declaration the caller
 * name_entry_new_game_setup allocates its date registers differently; no caller reads the result. T-25149 */
s32 name_entry_profile_store(void) {
    s32 n;
    s32 i;
    GameState *gs;
    /* FAKE: 0x20 bytes give the original's 96-byte frame with buf at sp+0x34; the copies need only
     * 17 bytes and the real size is unknown. T-25149 */
    u8 buf[0x20];
    u8 c;

    for (n = 0; n < 8; n++) {
        game_state.player_family_name[n] = 0;
        game_state.player_given_name[n] = 0;
    }
    for (n = 0; n < 0x10; n++) {
        game_state.player_full_name[n] = 0;
        game_state.player_nickname[n] = 0;
    }
    /* Stores through gs may alias the name arrays, so uopt reloads the second byte in each test
     * while as1 still hoists it above the stores; the full-width space bytes stay promoted. T-25149 */
    gs = &game_state;
    for (i = 0, n = 0; i < 6; i += 2) {
        c = name_entry_family_name[i];
        if ((c != D_800B3644[0] || name_entry_family_name[i + 1] != D_800B3644[1]) && (c != 0 || name_entry_family_name[i + 1] != 0)) {
            gs->player_family_name[n * 2] = c;
            gs->player_family_name[n * 2 + 1] = name_entry_family_name[i + 1];
            n++;
        }
    }
    for (i = 0, n = 0; i < 6; i += 2) {
        c = name_entry_given_name[i];
        if ((c != D_800B3644[0] || name_entry_given_name[i + 1] != D_800B3644[1]) && (c != 0 || name_entry_given_name[i + 1] != 0)) {
            gs->player_given_name[n * 2] = c;
            gs->player_given_name[n * 2 + 1] = name_entry_given_name[i + 1];
            n++;
        }
    }
    for (i = 0, n = 0; i < 10; i += 2) {
        c = name_entry_nickname[i];
        if ((c != D_800B3644[0] || name_entry_nickname[i + 1] != D_800B3644[1]) && (c != 0 || name_entry_nickname[i + 1] != 0)) {
            gs->player_nickname[n * 2] = c;
            gs->player_nickname[n * 2 + 1] = name_entry_nickname[i + 1];
            n++;
        }
    }
    strncpy(buf, game_state.player_family_name, 8);
    strncat(buf, game_state.player_given_name, 8);
    strncpy(game_state.player_full_name, buf, 0x10);
    /* FAKE: the redundant `& 3` on the two-bit fields spends the ugen temporary the original spends
     * there (as1 deletes the AND); without it every later temporary is one register low. T-25149 */
    ((WordView *)&game_state.player_birthday)->date.month = name_entry_player_month + 1;
    ((WordView *)&game_state.player_birthday)->date.day = name_entry_player_day + 1;
    ((WordView *)&game_state.player_birthday)->date.blood_type = (name_entry_player_blood + 1) & 3;
    game_state.girls[0].birthday.date.month = name_entry_shiori_month + 1;
    game_state.girls[0].birthday.date.day = name_entry_shiori_day + 1;
    game_state.girls[0].birthday.date.blood_type = (name_entry_shiori_blood + 1) & 3;
}

void name_entry_overlay_tables_set(void) {
    game_state.ovl_tpage_table = 0x801AAF58;
    game_state.ovl_frame_table = 0x801AAF5C;
    game_state.ovl_sequence_table = 0x801AAFD0;
    game_state.ovl_palette_a = 0x80190000;
    game_state.ovl_palette_b = 0x801AA200;
    game_state.ovl_image_buffer = 0x80192200;
}

void name_entry_new_game_setup(void) {
    /* FAKE: register + pre-call initializers place month/day/idx on $v0/$a2/$a1 after name_entry_profile_store. T-24113 */
    register u32 month;
    u32 day;
    u32 idx;
    u32 var_t0;

    month = name_entry_cursor;
    day = 0;
    idx = 0;
    func_80041878();
    name_entry_profile_store();
    month = game_state.girls[0].birthday.date.month;
    day = game_state.girls[0].birthday.date.day;
    game_state.girls[0].flags.f.club = (s32)(day * 3 + month) % 11;
    idx = name_entry_blood_compatibility_level[game_state.girls[0].birthday.date.blood_type + (*(NameProfileFlags *)&game_state.player_birthday).blood_type * 4] - 1;
    game_state.girls[0].affection = name_entry_initial_scores[idx * 3];
    game_state.girls[0].tokimeki = name_entry_initial_scores[idx * 3 + 1];
    game_state.girls[0].syoushin = name_entry_initial_scores[idx * 3 + 2];
    var_t0 = game_state.girls[0].flags.f.club;
    if (var_t0 == 0) {
        game_state.girls[1].flags.f.club = CLUB_DRAMA;
    } else if (var_t0 == 1) {
        game_state.girls[1].flags.f.club = CLUB_LITERATURE;
    } else if (rand() & 1) {
        game_state.girls[1].flags.f.club = CLUB_DRAMA;
    } else {
        game_state.girls[1].flags.f.club = CLUB_LITERATURE;
    }
    game_state.girls[1].birthday.date.blood_type = 1;
    if (var_t0 == 2) {
        game_state.girls[2].flags.f.club = CLUB_COMPUTER;
    } else if (var_t0 == 3) {
        game_state.girls[2].flags.f.club = CLUB_SCIENCE;
    } else if (rand() & 1) {
        game_state.girls[2].flags.f.club = CLUB_COMPUTER;
    } else {
        game_state.girls[2].flags.f.club = CLUB_SCIENCE;
    }
    game_state.girls[2].birthday.date.blood_type = 1;
    if (var_t0 == 4) {
        game_state.girls[3].flags.f.club = CLUB_BRASS_BAND;
    } else if (var_t0 == 5) {
        game_state.girls[3].flags.f.club = CLUB_ART;
    } else if (rand() & 1) {
        game_state.girls[3].flags.f.club = CLUB_BRASS_BAND;
    } else {
        game_state.girls[3].flags.f.club = CLUB_ART;
    }
    game_state.girls[3].birthday.date.blood_type = 2;
    if (var_t0 == 6) {
        game_state.girls[4].flags.f.club = CLUB_SOCCER;
    } else if (var_t0 == 7) {
        game_state.girls[4].flags.f.club = CLUB_BASEBALL;
    } else if (rand() & 1) {
        game_state.girls[4].flags.f.club = CLUB_SOCCER;
    } else {
        game_state.girls[4].flags.f.club = CLUB_BASEBALL;
    }
    game_state.girls[4].birthday.date.blood_type = 1;
    game_state.girls[5].flags.f.club = CLUB_TENNIS;
    game_state.girls[5].birthday.date.blood_type = 2;
    game_state.girls[6].flags.f.club = CLUB_SWIMMING;
    game_state.girls[6].birthday.date.blood_type = 1;
    game_state.girls[7].flags.f.club = CLUB_NONE;
    game_state.girls[7].birthday.date.blood_type = 0;
    game_state.girls[8].flags.f.club = CLUB_NONE;
    game_state.girls[8].birthday.date.blood_type = 2;
    game_state.girls[9].flags.f.club = CLUB_NONE;
    game_state.girls[9].birthday.date.blood_type = 1;
    game_state.girls[10].flags.f.club = CLUB_BASKETBALL;
    game_state.girls[10].birthday.date.blood_type = 0;
    game_state.girls[11].flags.f.club = CLUB_NONE;
    game_state.girls[11].birthday.date.blood_type = 1;
    game_state.girls[12].birthday.date.blood_type = 3;
    name_entry_cheat_names_apply();
}

/* The nine ParamValue members of GameState (+0xFC..+0x11C) are walked as one array, as in main
 * 8007C030: IDO unrolls each nine-record loop into one peeled store and two groups of four. */
void name_entry_cheat_names_apply(void) {
    s32 i;

    if (strcmp(name_entry_nickname, "こなみまん") == 0) {
        for (i = 0; i < 9; i++) {
            (&game_state.param_condition)[i].value = 0x23D;
        }
    }
    if (strcmp(name_entry_family_name, "みんな") == 0 && strcmp(name_entry_given_name, "仲良し") == 0) {
        game_state.girls[0].tokimeki = 0x20;
        game_state.girls[0].affection = 0x40;
        for (i = 1; i < 9; i++) {
            if ((game_state.girls[i].flags.b[2] >> 4) != (game_state.girls[0].flags.b[2] >> 4)) {
                game_state.girls[i].tokimeki = 0x7F;
                game_state.girls[i].flags.f.met = 1;
                game_state.girls[i].flags.f.b3 = 1;
                game_state.girls[i].affection = 0x7F;
            }
        }
    }
    if (strcmp(name_entry_family_name, "上上") == 0 && strcmp(name_entry_given_name, "下下") == 0
        && strcmp(name_entry_nickname, "左右左右×") == 0) {
        for (i = 0; i < 9; i++) {
            (&game_state.param_condition)[i].value = 0;
        }
    }
    if (strcmp(name_entry_family_name, "↑↑") == 0 && strcmp(name_entry_given_name, "↓↓") == 0
        && strcmp(name_entry_nickname, "←→←→×") == 0) {
        for (i = 0; i < 9; i++) {
            (&game_state.param_condition)[i].value = 0x3E7;
        }
    }
}

/* Implicit int like name_entry_scroll_arrows_update. FAKE: the address-taken u8 views of the u8 fields give the original $v1/$a0 chain (a member access does not); real source unknown. T-11650 */
s32 name_entry_exit_to_prologue(void) {
    switch (*(u8 *)&game_state.substep) {
    case 0:
        if (game_state.mode_step_timer.u >= 0x41U) {
            *(u8 *)&game_state.substep += 1;
            return;
        }
        return;
    case 1:
        if (*(u8 *)&game_state.screen_brightness >= 4) {
            *(u8 *)&game_state.screen_brightness -= 4;
        } else {
            *(u8 *)&game_state.substep += 1;
            *(u8 *)&game_state.screen_brightness = 0;
        }
        set_dec_bri(*(u8 *)&game_state.screen_brightness);
        return;
    case 2:
        game_mode_set(GAME_MODE_PROLOGUE);
        text_set_advance(0xE);
        break;
    }
}

