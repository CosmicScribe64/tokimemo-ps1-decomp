/* date_scene_state.c: src/ovl/DATE2/80132DE0.c:35: date state machine, movie and scene callbacks. Original object 0x80132DE0-0x80136D50 (config/objects/DATE2.txt). */
#define MAIN_API_OVERRIDE_read_bustup /* implicit int view: the call's $v0 result moves the reloaded unk_110A to $v1 in amusement_decoded_scene_sequence_c (definition: void, T-11880) */
#include "common.h"
#include "ovl/amusement_park_date.h"
#include "main/sprite.h"

s32 read_bustup();
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/DATE2/data/amusement_park_date/date_scene_state.data", amusement_scene_choice_index);
INCLUDE_RODATA("asm/ovl/DATE2/data/amusement_park_date/date_scene_state.data", amusement_scene_choice_bits);
INCLUDE_RODATA("asm/ovl/DATE2/data/amusement_park_date/date_scene_state.data", D_8013A4C8);
INCLUDE_RODATA("asm/ovl/DATE2/data/amusement_park_date/date_scene_state.data", D_8013A5B0);
INCLUDE_RODATA("asm/ovl/DATE2/data/amusement_park_date/date_scene_state.data", D_8013A5B4);
INCLUDE_RODATA("asm/ovl/DATE2/data/amusement_park_date/date_scene_state.data", D_8013A5B8);
INCLUDE_RODATA("asm/ovl/DATE2/data/amusement_park_date/date_scene_state.data", D_8013A5BC);
INCLUDE_RODATA("asm/ovl/DATE2/data/amusement_park_date/date_scene_state.data", D_8013A5C0);
INCLUDE_RODATA("asm/ovl/DATE2/data/amusement_park_date/date_scene_state.data", D_8013A5C4);
INCLUDE_RODATA("asm/ovl/DATE2/data/amusement_park_date/date_scene_state.data", D_8013A5C8);
INCLUDE_RODATA("asm/ovl/DATE2/data/amusement_park_date/date_scene_state.data", amusement_scene_effect_frames);
INCLUDE_RODATA("asm/ovl/DATE2/data/amusement_park_date/date_scene_state.data", D_8013A5D0);
s16 D_8013A5D4[3] = { 0, 0, 0 };
s16 D_8013A5DC[3] = { 0, 0, 0 };
s16 D_8013A5E4[3] = { 0, 0, 0 };
s16 D_8013A5EC[3] = { 0, 0, 0 };

typedef struct {
    void (*f[44])();
} FnTbl44; /* size 0xB0 */
extern FnTbl44 D_8013A600;

typedef struct {
    void (*f[58])();
} FnTbl58; /* size 0xE8 */
extern FnTbl58 D_8013A4C8;

void amusement_mode_step_callback_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl58 tbl;

    tbl = D_8013A4C8;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void amusement_texture_page_copy_shiori(void) {
    memcpy((void *)(0x80180000 + game_state.mode_step_timer.w * 0x1400), 0x801C0000 + game_state.mode_step_timer.w * 0x1400, 0x1400);
    game_state.mode_step_timer.w += 1;
    if (game_state.mode_step_timer.w == 0x12) {
        mode_step_next();
    }
}

void amusement_texture_page_copy_name_entry(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    temp_v0 = game_state.mode_step_timer.w * 0x1400;
    memcpy(temp_v0 + 0x80180000, temp_v0 + 0x801A0000, 0x1400);
    temp_v0_2 = game_state.mode_step_timer.w + 1;
    game_state.mode_step_timer.w = temp_v0_2;
    if (temp_v0_2 == 0x12) {
        game_state.current_girl = GIRL_SHIORI;
        get_p_name(name_code_player_form, 0);
        mode_step_next();
    }
}

void amusement_bustup_background_setup(void) {
    s32 pad; /* FAKE: unused local that moves rect to sp+0x2C like the original (T-3330 frame rule). T-4050 */
    RECT rect;

    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C8E0(&rect, (void *)0x80180000);
    game_state.current_girl = game_state.date_girl;
    talk_names_set(game_state.date_girl);
    amusement_date_girl_dialogue_pointer_bind();
    mode_step_next();
}

void amusement_texture_page_copy_restore_c(void) {
    memcpy((void *)(0x801C0000 + game_state.mode_step_timer.w * 0x1400), 0x80180000 + game_state.mode_step_timer.w * 0x1400, 0x1400);
    game_state.mode_step_timer.w += 1;
    if (game_state.mode_step_timer.w == 0x12) {
        portrait_show_checked();
    }
}

void amusement_texture_page_copy_restore_a(void) {
    memcpy((void *)(0x801A0000 + game_state.mode_step_timer.w * 0x1400), 0x80180000 + game_state.mode_step_timer.w * 0x1400, 0x1400);
    game_state.mode_step_timer.w += 1;
    if (game_state.mode_step_timer.w == 0x12) {
        portrait_show_checked();
    }
}

void amusement_portrait_background_restore(void) {
    RECT rect;

    portrait_show_checked();
    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C884(&rect, (void *)0x80180000);
}

void amusement_scene_counter_girl_select(void) {
    GameState *g;
    s16 sums[10];
    s16 i;
    s16 j;
    s32 best;

    best = 100; /* set before the loops: the original holds 100 in a2 from the entry */
    amusement_scene_choice_index = 0;
    game_state.current_girl = GIRL_MIO;
    g = &game_state;
    for (i = 1; i < 10; i++) {
        sums[i] = 0;
        for (j = 1; j < 0x25; j++) {
            sums[i] += g->girls[i].scene_counters[j]; /* j = 0x24 reads the byte after unk_14 (the original does) */
        }
        sums[i] -= g->girls[i].scene_counters[0x1A];
    }

    for (i = 1; i < 10; i++) {
        if (sums[i] < best && g->girls[i].flags.f.met) {
            g->current_girl = i; /* through g: the store may alias sums, so sums[i] is reloaded below */
            best = sums[i];
        }
    }
    talk_names_set(game_state.date_girl = game_state.current_girl);
    game_state.current_girl = GIRL_SHIORI;
    event_log_add(0xC, 0);
    game_state.current_girl = game_state.date_girl;
    event_log_add(0xC, 0);
    amusement_date_girl_dialogue_pointer_bind();
    amusement_status_scene_value_adjust();
    mode_step_next();
}

void amusement_date_girl_dialogue_pointer_bind(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        amusement_selected_text_table = D_8013A384;
        amusement_selected_command_table = D_8013A3B0;
        amusement_selected_voice_table = D_8013A3DC;
        return;
    case GIRL_MIO:
        amusement_selected_text_table = D_8013A394;
        amusement_selected_command_table = D_8013A3C0;
        amusement_selected_voice_table = D_8013A3EC;
        return;
    case GIRL_YUINA:
        amusement_selected_text_table = D_8013A388;
        amusement_selected_command_table = D_8013A3B4;
        amusement_selected_voice_table = D_8013A3E0;
        return;
    case GIRL_AYAKO:
        amusement_selected_text_table = D_8013A38C;
        amusement_selected_command_table = D_8013A3B8;
        amusement_selected_voice_table = D_8013A3E4;
        return;
    case GIRL_SAKI:
        amusement_selected_text_table = D_8013A3A4;
        amusement_selected_command_table = D_8013A3D0;
        amusement_selected_voice_table = D_8013A3FC;
        return;
    case GIRL_YUKARI:
        amusement_selected_text_table = D_8013A398;
        amusement_selected_command_table = D_8013A3C4;
        amusement_selected_voice_table = D_8013A3F0;
        return;
    case GIRL_NOZOMI:
        amusement_selected_text_table = D_8013A39C;
        amusement_selected_command_table = D_8013A3C8;
        amusement_selected_voice_table = D_8013A3F4;
        return;
    case GIRL_MIRA:
        amusement_selected_text_table = D_8013A390;
        amusement_selected_command_table = D_8013A3BC;
        amusement_selected_voice_table = D_8013A3E8;
        return;
    case GIRL_YUKO:
        amusement_selected_text_table = D_8013A380;
        amusement_selected_command_table = D_8013A3AC;
        amusement_selected_voice_table = D_8013A3D8;
        return;
    case GIRL_MEGUMI:
        amusement_selected_text_table = D_8013A3A0;
        amusement_selected_command_table = D_8013A3CC;
        amusement_selected_voice_table = D_8013A3F8;
        /* fallthrough */
    default:
        return;
    }
}

void amusement_scene_background_select(void) {
    s32 temp;
    temp = amusement_scene_choice_index;
    switch (temp) {
    case 0:
        bg_read_sub2(0x4350);
        strcpy(scene_place_title_2, "観覧車");
        break;
    case 1:
        if ((u8) game_state.year >= 0x61U) {
            bg_read_sub2(0x4359);
            strcpy(scene_place_title_2, "絶叫マシーンビビール");
        } else {
            bg_read_sub2(0x433E);
            strcpy(scene_place_title_2, "ジェットコースター");
        }
        break;
    case 2:
        bg_read_sub2(0x4347);
        strcpy(scene_place_title_2, "ゴーストハウス");
        break;
    }
    mode_step_next();
}

void amusement_portrait_read_start(void) {
    portrait_read_start(0x6C8C);
    mode_step_next();
}

void amusement_shiori_name_portrait_setup(void) {
    game_state.current_girl = GIRL_SHIORI;
    get_p_name(&name_code_player_form, 0);
    portrait_read_start(0x55DF);
    mode_step_next();
}

void amusement_date_girl_bustup_setup(void) {
    game_state.current_girl = game_state.date_girl;
    talk_names_set(game_state.date_girl);
    amusement_date_girl_dialogue_pointer_bind();
    read_bustup();
}

void amusement_date_girl_portrait_read(void) {
    game_state.current_girl = game_state.date_girl;
    amusement_date_girl_dialogue_pointer_bind();
    talk_names_set(game_state.current_girl);
    switch (game_state.current_girl) {
    case GIRL_MIO:
        portrait_read_start(0x5DC8);
        break;
    case GIRL_YUINA:
        portrait_read_start(0x52B5);
        break;
    case GIRL_AYAKO:
        portrait_read_start(0x5936);
        break;
    case GIRL_SAKI:
        portrait_read_start(0x6746);
        break;
    case GIRL_YUKARI:
        portrait_read_start(0x603E);
        break;
    case GIRL_NOZOMI:
        portrait_read_start(0x62B4);
        break;
    case GIRL_MIRA:
        portrait_read_start(0x5B7F);
        break;
    case GIRL_YUKO:
        portrait_read_start(0x506C);
        break;
    case GIRL_MEGUMI:
        portrait_read_start(0x652A);
        break;
    }
    mode_step_next();
}

void amusement_response_portrait_branch(void) {
    if (talk_choice != 0) {
        amusement_date_girl_portrait_read();
        amusement_scene_choice_bits |= 1 << (s8)amusement_scene_choice_index;
    } else {
        amusement_shiori_name_portrait_setup();
    }
    amusement_date_girl_dialogue_pointer_bind();
}

void amusement_scene_value_setup(void) {
    s32 temp;
    temp = amusement_scene_choice_index;
    switch (temp) {
    case 0:
        amusement_scene_dialogue_group = 1;
        amusement_alternate_dialogue_group = 1;
        break;
    case 1:
        if ((u8) game_state.year >= 0x61U) {
            amusement_scene_dialogue_group = 5;
            amusement_alternate_dialogue_group = 5;
            amusement_dialogue_group += 2;
        } else {
            amusement_scene_dialogue_group = 3;
            amusement_alternate_dialogue_group = 3;
        }
        break;
    case 2:
        amusement_scene_dialogue_group = 7;
        amusement_alternate_dialogue_group = 7;
        if ((u8) game_state.year < 0x61U) {
            amusement_dialogue_group += 2;
        }
        break;
    }
    amusement_status_scene_value_adjust();
    mode_step_next();
}

void amusement_scene_selector_mode_stage_exit(void) {
    if (amusement_scene_choice_index == 2) {
        mode_stage_next();
        return;
    }
    mode_step_next();
}

void amusement_scene_selector_advance(void) {
    amusement_scene_choice_index += 1;
    mode_step_set(0x1B);
}

/* FAKE: s32 fall-through keeps the original counter/result register. T-10010. */
s32 amusement_status_scene_value_adjust(void) {
    u8 previous_state;
    u8 first_state;
    first_state = get_g_zyotai_s(GIRL_SHIORI) & 0x7F;
    if (first_state >= 2U) {
        amusement_scene_dialogue_group += 0xA;
    }
    if (first_state >= 3U) {
        amusement_scene_dialogue_group += 0xA;
    }
    if (first_state == 4) {
        amusement_scene_dialogue_group += 0xA;
    }
    previous_state = get_g_zyotai_s(game_state.date_girl) & 0x7F;
    if (previous_state >= 2U) {
        amusement_alternate_dialogue_group += 0xA;
    }
    if (previous_state >= 3U) {
        amusement_alternate_dialogue_group += 0xA;
    }
    if (previous_state == 4) {
        amusement_alternate_dialogue_group += 0xA;
    }
}

void amusement_scene_texture_pointer_setup(void) {
    D_8013A5B0 = 0x801C6000;
    D_8013A5B4 = 0x801AE000;
    D_8013A5B8 = 0x801B2000;
    D_8013A5BC = 0x801B6000;
    D_8013A5C0 = 0x801BA000;
    D_8013A5C4 = 0x801BE000;
    D_8013A5C8 = 0x801C2000;
}

void amusement_scene_selector_callback_dispatch(void) {
    u8 sel = amusement_scene_choice_index; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        amusement_decoded_scene_sequence_b();
        return;
    case 1:
        if ((u8) game_state.year >= 0x61U) {
            amusement_decoded_scene_sequence_a();
            return;
        }
        amusement_decoded_scene_sequence_c();
        return;
    default:
        amusement_random_scene_sprite_sequence();
        return;
    }
}

/* s32: the header-error path and the scene steps outside 9..11 return 0, the dot animation falls off the end with the timer in v0 (as amusement_decoded_scene_sequence_b). T-11880 */
s32 amusement_decoded_scene_sequence_a(void) {
    GameState *g;
    s16 i;
    s16 v;
    u8 x;
    RECT rect;
    s32 previous_step;
    u32 saved_step_frames;
    SpriteQuad *p;
    DateMovieWork *movie;
    u8 value;
    u32 color;
    u32 timer;

    g = &game_state;
    movie = (DateMovieWork *)bg_lba_cache;
    cd_read_checked = 0;
    switch (g->mode_step_timer.u) {
    case 0:
        D_80122D2C = 1;
        srand(game_state.frame_count);
        audio_command(0x206);
        movie->unk_06 = 0;
        movie->unk_02 = 0;
        movie->unk_04 = 0;
        movie->unk_00 = 0;
        movie->unk_19 = 0;
        movie->unk_18 = 0;
        movie->unk_28 = -1;
        movie->unk_0A = 0;
        movie->unk_1A = 0;
        hizuke_disp_switch(0);
        D_8013A5D0 = 2;
        game_state.substep = 0;
        amusement_scene_effect_frames = 0; for (i = 0; i < 2; i++) { /* FAKE: one source line keeps the as1 store order (as in amusement_decoded_scene_sequence_b) T-11880 */
            sprite_quads[i + 4].w = (i << 6) + 0x80;
            sprite_quads[i + 4].flags = 0x82000000;
            sprite_quads[i + 4].h = 0;
            sprite_quads[i + 4].unk_0E = 0;
            sprite_quads[i + 4].unk_0F = 0xA0;
            sprite_quads[i + 4].unk_10 = 0;
            sprite_quads[i + 4].unk_0C = (i * 2) + 0x15;
            sprite_quads[i + 4].unk_12 = 0x1E0;
            sprite_quads[i + 4].tint_r = 0;
            sprite_quads[i + 4].tint_g = 0;
            sprite_quads[i + 4].tint_b = 0;
            sprite_quads[i + 4].anchor_x = 0;
            sprite_quads[i + 4].anchor_y = 0;
            sprite_quads[i + 4].unk_1C = 0x1000;
            sprite_quads[i + 4].unk_1E = 0x1000;
            sprite_quads[i + 4].unk_20 = 0;
            sprite_quads[i + 4].x = (i << 7) - 0xA0;
            sprite_quads[i + 4].y = -0x78;
            game_state.quad_layer[i + 4] = 0xB;
        }
        for (i = 0; i < 2; i++) {
            sprite_quads[i + 6].w = (i << 6) + 0x80;
            sprite_quads[i + 6].flags = 0x82000000;
            sprite_quads[i + 6].h = 0xA0;
            sprite_quads[i + 6].unk_0E = 0;
            sprite_quads[i + 6].unk_0F = 0;
            sprite_quads[i + 6].unk_10 = 0;
            sprite_quads[i + 6].unk_0C = (i * 2) + 0xA;
            sprite_quads[i + 6].unk_12 = 0x1E0;
            sprite_quads[i + 6].tint_r = 0;
            sprite_quads[i + 6].tint_g = 0;
            sprite_quads[i + 6].tint_b = 0;
            sprite_quads[i + 6].anchor_x = 0;
            sprite_quads[i + 6].anchor_y = 0;
            sprite_quads[i + 6].unk_1C = 0x1000;
            sprite_quads[i + 6].unk_1E = 0x1000;
            sprite_quads[i + 6].unk_20 = 0;
            sprite_quads[i + 6].x = (i << 7) - 0xA0;
            sprite_quads[i + 6].y = -0x78;
            game_state.quad_layer[i + 6] = 0xB;
        }
        dec_bg_cd_read(0x43CB, 1);
        g->mode_step_timer.u += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            bg_still_decode_to_vram(0x140, 0x100, 0x180, 0xA0);
            rect.x = 0x140;
            rect.y = 0x100;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_8009C8E0(&rect, (void *)0x80183000);
            g->mode_step_timer.u += 1;
        }
        break;
    case 2:
        dec_bg_cd_read(0x43D4, 1);
        g->mode_step_timer.u += 1;
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            bg_still_decode_to_vram(0x280, 0, 0x180, 0xA0);
            rect.x = 0x280;
            rect.y = 0;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_8009C8E0(&rect, (void *)0x801A1000);
            g->mode_step_timer.u += 1;
        }
        break;
    case 4:
        dec_bg_cd_read(0x43DE, 1);
        g->mode_step_timer.u += 1;
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            bg_still_decode_to_vram(0x140, 0x100, 0x180, 0xA0);
            rect.x = 0x140;
            rect.y = 0x100;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_8009C8E0(&rect, (void *)0x801BF000);
            g->mode_step_timer.u += 1;
        }
        break;
    case 6:
        dec_bg_cd_read(0x43E7, 1);
        g->mode_step_timer.u += 1;
        break;
    case 7:
        if (cd_read_sync_status() & 1) {
            bg_still_decode_to_vram(0x280, 0, 0x180, 0xA0);
            rect.x = 0x280;
            rect.y = 0;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_8009C8E0(&rect, (void *)0x801DD000);
            g->mode_step_timer.u += 1;
        }
        break;
    case 8:
        for (i = 0; i < 4; i++) {
            sprite_quads[i + 4].flags = 0x02000000;
        }
        g->mode_step_timer.u += 1;
        audio_command(0x500);
        break;
    case 9:
        value = sprite_quads[4].tint_r;
        value += 2;
        color = (value & 0xFF);
        sprite_quads[7].tint_b = color;
        sprite_quads[7].tint_g = color;
        sprite_quads[7].tint_r = color;
        sprite_quads[6].tint_b = color;
        sprite_quads[6].tint_g = color;
        sprite_quads[6].tint_r = color;
        sprite_quads[5].tint_b = color;
        sprite_quads[5].tint_g = color;
        sprite_quads[5].tint_r = color;
        sprite_quads[4].tint_b = color;
        sprite_quads[4].tint_g = color;
        sprite_quads[4].tint_r = color;
        if (value == 0x80) {
            g->mode_step_timer.u += 1;
        }
        break;
    case 10:
        break;
    case 11:
        value = sprite_quads[4].tint_r;
        value -= 2;
        color = (value & 0xFF);
        sprite_quads[7].tint_b = color;
        sprite_quads[7].tint_g = color;
        sprite_quads[7].tint_r = color;
        sprite_quads[6].tint_b = color;
        sprite_quads[6].tint_g = color;
        sprite_quads[6].tint_r = color;
        sprite_quads[5].tint_b = color;
        sprite_quads[5].tint_g = color;
        sprite_quads[5].tint_r = color;
        sprite_quads[4].tint_b = color;
        sprite_quads[4].tint_g = color;
        sprite_quads[4].tint_r = color;
        if (value == 0) {
            for (i = 0; i < 4; i++) {
                sprite_quads[i + 4].flags = 0x82000000;
            }
            g->mode_step_timer.u += 1;
        }
        break;
    case 12:
        previous_step = game_state.mode_step;
        saved_step_frames = g->mode_step_timer.u;
        read_bustup();
        g->mode_step_timer.u = saved_step_frames;
        if (previous_step != game_state.mode_step) {
            game_state.mode_step--;
            g->mode_step_timer.u += 1;
        }
        break;
    case 13:
    case 16:
    case 19:
    case 21:
    case 23:
        if (cd_read_sync_status() & 1) {
            g->mode_step_timer.u += 1;
        }
        break;
    case 14:
        saved_step_frames = g->mode_step_timer.u;
        portrait_show_checked();
        game_state.mode_step -= 1;
        g->mode_step_timer.u = saved_step_frames + 1;
        break;
    case 15:
        cd_read_sectors_nowait(0xA, 0x80162000, 0x4359);
        g->mode_step_timer.u += 1;
        break;
    case 17:
        if (((((DateMovieHeader8 *)bg_still_bitstream)->unk_00 & 0xFFFF0000) != 0x38000000) || ((((DateMovieHeader8 *)bg_still_bitstream)->unk_04 & 0xFF010000) != 0x10000)) {
            printf("DEC_BS header Error \n");
            movie->unk_04 = 0;
            movie->unk_06 = 0;
            g->mode_step_timer.u -= 2;
            return 0;
        }
        dec_bg_decode_to_vram(1);
        dec_bg_show_switch(1);
        g->mode_step_timer.u += 1;
        break;
    case 18:
        audio_command(0x200);
        saved_step_frames = g->mode_step_timer.u;
        amusement_opening_sector_load();
        game_state.mode_step -= 1;
        g->mode_step_timer.u = saved_step_frames + 1;
        break;
    default:
        hizuke_disp_switch(1);
        D_80122D2C = 0;
        mode_step_next();
        break;
    }
    if (g->mode_step_timer.u < 9 || g->mode_step_timer.u >= 12) {
        return 0;
    }
    p = sprite_quads;
    v = p[4].unk_0E;
    v += D_8013A5D0;
    if (v < 0) {
        v = 0;
        D_8013A5D0 = (rand() % 4) + 2;
    }
    if (v >= 0x40) {
        v = 0x3F;
        D_8013A5D0 = -2 - (rand() % 4);
    }
    x = p[4].unk_0F;
    p[7].unk_0E = v;
    p[6].unk_0E = v;
    p[5].unk_0E = v;
    p[4].unk_0E = v;
    if (x != 0) {
        p[4].h += 0x14;
        p[5].h += 0x14;
        x -= 0x14;
        p[5].unk_0F -= 0x14;
    } else {
        p[4].h -= 0x14;
        p[4].y += 0x14;
        p[5].h -= 0x14;
        p[5].y += 0x14;
    }
    if (p[6].unk_0F != 0) {
        p[6].h += 0x14;
        p[7].h += 0x14;
        p[6].unk_0F -= 0x14;
        p[7].unk_0F -= 0x14;
    } else {
        p[6].h -= 0x14;
        p[6].y += 0x14;
        p[7].h -= 0x14;
        p[7].y += 0x14;
    }
    p[4].unk_0F = x;
    if ((x == 0 && (g->substep == 1 || g->substep == 3)) || (p[6].unk_0F == 0 && (g->substep == 0 || g->substep == 2))) {
        switch (game_state.substep) { /* FAKE: g-> in the test, the symbol here; one access path keeps the byte in $v0 and drops the reload T-11880 */
        case 0:
            for (i = 0; i < 2; i++) {
                sprite_quads[i + 4].h = 0;
                sprite_quads[i + 4].unk_0F = 0xA0;
                sprite_quads[i + 4].y = -0x78;
                sprite_quads[i + 4].unk_0C = (i * 2) + 0x15;
            }
            LoadSquare(0x140, 0x100, 0x180, 0xA0, 0x80183000);
            break;
        case 1:
            for (i = 0; i < 2; i++) {
                sprite_quads[i + 6].h = 0;
                sprite_quads[i + 6].unk_0F = 0xA0;
                sprite_quads[i + 6].y = -0x78;
                sprite_quads[i + 6].unk_0C = (i * 2) + 0xA;
            }
            LoadSquare(0x280, 0, 0x180, 0xA0, 0x801A1000);
            break;
        case 2:
            for (i = 0; i < 2; i++) {
                sprite_quads[i + 4].h = 0;
                sprite_quads[i + 4].unk_0F = 0xA0;
                sprite_quads[i + 4].y = -0x78;
                sprite_quads[i + 4].unk_0C = (i * 2) + 0x15;
            }
            LoadSquare(0x140, 0x100, 0x180, 0xA0, 0x801BF000);
            break;
        case 3:
            for (i = 0; i < 2; i++) {
                sprite_quads[i + 6].h = 0;
                sprite_quads[i + 6].unk_0F = 0xA0;
                sprite_quads[i + 6].y = -0x78;
                sprite_quads[i + 6].unk_0C = (i * 2) + 0xA;
            }
            LoadSquare(0x280, 0, 0x180, 0xA0, 0x801DD000);
            break;
        }
        if (++game_state.substep >= 4) {
            game_state.substep = 0;
        }
    }
    timer = (amusement_scene_effect_frames + 1) & 0xFFFF;
    amusement_scene_effect_frames = timer;
    if (timer == 0x96) {
        g->mode_step_timer.u += 1;
    }
}

/* s32: the step exits return 0 (or v0,zero,zero), the fade steps fall off the end (as in the DATE twin func_8014718C). T-11880 */
s32 amusement_decoded_scene_sequence_b(void) {
    GameState *g;
    RECT rect; /* FAKE: unused, reserves the 8 bytes above saved_step_frames in the original 0x40 frame (as in the DATE twin func_8014718C) T-11880 */
    u32 saved_step_frames;
    u32 timer;
    s16 i;
    u8 x;
    u32 y; /* FAKE: second copy of the fade step; it holds $v1 so the stored value lands in $a1 T-11880 */
    DateMovieWork *movie;

    g = &game_state;
    movie = (DateMovieWork *)bg_lba_cache;
    cd_read_checked = 0;
    switch (g->mode_step_timer.u) {
    case 0:
        D_80122D2C = 1;
        audio_command(0x201);
        movie->unk_06 = 0;
        movie->unk_02 = 0;
        movie->unk_04 = 0;
        movie->unk_00 = 0;
        movie->unk_19 = 0;
        movie->unk_18 = 0;
        movie->unk_28 = -1;
        movie->unk_0A = 0;
        movie->unk_1A = 0;
        hizuke_disp_switch(0);
        amusement_scene_effect_frames = 0; for (i = 0; i < 2; i++) { /* FAKE: one source line keeps the as1 store order T-11880 */
            sprite_quads[i + 10].w = (i << 6) + 0x80;
            sprite_quads[i + 10].flags = 0x82000000;
            sprite_quads[i + 10].h = 0xA0;
            sprite_quads[i + 10].unk_0E = 0;
            sprite_quads[i + 10].unk_0F = 0;
            sprite_quads[i + 10].unk_10 = 0;
            sprite_quads[i + 10].unk_0C = (i * 2) + 0x15;
            sprite_quads[i + 10].unk_12 = 0x1E0;
            sprite_quads[i + 10].tint_r = 0;
            sprite_quads[i + 10].tint_g = 0;
            sprite_quads[i + 10].tint_b = 0;
            sprite_quads[i + 10].anchor_x = 0;
            sprite_quads[i + 10].anchor_y = 0;
            sprite_quads[i + 10].unk_1C = 0x1000;
            sprite_quads[i + 10].unk_1E = 0x1000;
            sprite_quads[i + 10].unk_20 = 0;
            sprite_quads[i + 10].x = (i << 7) - 0xA0;
            sprite_quads[i + 10].y = -0x96;
            game_state.quad_layer[i + 10] = 0xC;
        }
        amusement_decoded_background_read_retry(0x43C2);
        g->mode_step_timer.u += 1;
        return 0;
    case 1:
        if (cd_read_sync_status() & 1) {
            if (((((DateMovieHeader8 *)bg_still_bitstream)->unk_00 & 0xFFFF0000) != 0x38000000) || ((((DateMovieHeader8 *)bg_still_bitstream)->unk_04 & 0xFF010000) != 0x10000)) {
                printf("DEC_BS header Error \n");
                movie->unk_04 = 0;
                movie->unk_06 = 0;
                g->mode_step_timer.u -= 1;
                return 0;
            }
            bg_still_decode_to_vram(0x140, 0x100, 0x140, 0xA0);
            g->mode_step_timer.u += 1;
        }
        return 0;
    case 2:
        cd_read_sectors(0x31, 0x801AE000, 0xBCD1);
        amusement_scene_texture_pointer_setup();
        g->mode_step_timer.u += 1;
        return 0;
    case 3:
    case 11:
    case 14:
    case 16:
    case 18:
        if (cd_read_sync_status() & 1) {
            g->mode_step_timer.u += 1;
        }
        return 0;
    case 4:
        load_palette(D_8013A5B0, 0x11, 1, 0, 0);
        scene_still_show(D_8013A5B4, D_8013A5B8, D_8013A5BC, D_8013A5C0, D_8013A5C4, D_8013A5C8);
        g->mode_step_timer.u += 1;
        return 0;
    case 5:
        for (i = 10; i < 12; i++) {
            sprite_quads[i].flags &= 0x7FFFFFFF;
        }
        for (i = 58; i < 64; i++) {
            sprite_quads[i].flags &= 0x7FFFFFFF;
        }
        g->mode_step_timer.u += 1;
        return 0;
    case 6:
        x = (sprite_quads[58].tint_r += 4); y = sprite_quads[58].tint_g = x; sprite_quads[58].tint_b = y; /* FAKE: see y; one line keeps the store order T-11880 */
        for (i = 10; i < 12; i++) {
            sprite_quads[i].tint_r = sprite_quads[i].tint_g = sprite_quads[i].tint_b = sprite_quads[58].tint_r;
        }
        for (i = 59; i < 64; i++) {
            sprite_quads[i].tint_r = sprite_quads[i].tint_g = sprite_quads[i].tint_b = sprite_quads[58].tint_r;
        }
        if (sprite_quads[58].tint_r == 0x40) {
            g->mode_step_timer.u += 1;
        }
        return 0;
    case 7:
        x = (sprite_quads[58].tint_r += 4); y = sprite_quads[58].tint_g = x; sprite_quads[58].tint_b = y; /* FAKE: see y; one line keeps the store order T-11880 */
        for (i = 10; i < 12; i++) {
            sprite_quads[i].tint_r = sprite_quads[i].tint_g = sprite_quads[i].tint_b = sprite_quads[58].tint_r;
        }
        for (i = 59; i < 64; i++) {
            sprite_quads[i].tint_r = sprite_quads[i].tint_g = sprite_quads[i].tint_b = sprite_quads[58].tint_r;
        }
        if (sprite_quads[58].tint_r == 0x80) {
            g->mode_step_timer.u += 1;
        }
        break;
    case 8:
        break;
    case 9:
        x = (sprite_quads[58].tint_r -= 4); y = sprite_quads[58].tint_g = x; sprite_quads[58].tint_b = y; /* FAKE: see y; one line keeps the store order T-11880 */
        for (i = 10; i < 12; i++) {
            sprite_quads[i].tint_r = sprite_quads[i].tint_g = sprite_quads[i].tint_b = sprite_quads[58].tint_r;
        }
        for (i = 59; i < 64; i++) {
            sprite_quads[i].tint_r = sprite_quads[i].tint_g = sprite_quads[i].tint_b = sprite_quads[58].tint_r;
        }
        if (sprite_quads[58].tint_r == 0) {
            for (i = 10; i < 12; i++) {
                sprite_quads[i].flags |= 0x80000000;
            }
            for (i = 58; i < 64; i++) {
                sprite_quads[i].flags |= 0x80000000;
            }
            g->mode_step_timer.u += 1;
        }
        break;
    case 10:
        cd_read_sectors_nowait(0xA, 0x80162000, 0x4350);
        g->mode_step_timer.u += 1;
        return 0;
    case 12:
        if (((((DateMovieHeader8 *)bg_still_bitstream)->unk_00 & 0xFFFF0000) != 0x38000000) || ((((DateMovieHeader8 *)bg_still_bitstream)->unk_04 & 0xFF010000) != 0x10000)) {
            printf("DEC_BS header Error \n");
            movie->unk_04 = 0;
            movie->unk_06 = 0;
            g->mode_step_timer.u -= 2;
            return 0;
        }
        dec_bg_decode_to_vram(1);
        dec_bg_show_switch(1);
        g->mode_step_timer.u += 1;
        return 0;
    case 13:
        saved_step_frames = g->mode_step_timer.u;
        amusement_opening_sector_load();
        game_state.mode_step -= 1;
        g->mode_step_timer.u = saved_step_frames;
        g->mode_step_timer.u++;
        return 0;
    default:
        mode_step_next();
        D_80122D2C = 0;
        audio_command(0x200);
        hizuke_disp_switch(1);
        return 0;
    }
    timer = (amusement_scene_effect_frames + 1) & 0xFFFF;
    amusement_scene_effect_frames = timer;
    if (timer == 0x96) {
        g->mode_step_timer.u += 1;
    }
}

/* s32: only the header-error path returns 0 (or v0,zero,zero), as in the DATE twin func_80147958. T-11880 */
s32 amusement_decoded_scene_sequence_c(void) {
    GameState *g;
    s16 i;
    s32 previous_step;
    u32 saved_step_frames;
    RECT rect;
    DateMovieWork *movie;
    DateMovieHeader8 *header; /* FAKE: unused, reserves the observed frame slot (as in the DATE twin func_80147958) T-11880 */
    u8 value;
    u32 color;

    g = &game_state;
    movie = (DateMovieWork *)bg_lba_cache;
    cd_read_checked = 0;
    switch (g->mode_step_timer.u) {
    case 0:
        audio_command(0xCF);
        D_80122D2C = 1;
        movie->unk_06 = 0;
        movie->unk_02 = 0;
        movie->unk_04 = 0;
        movie->unk_00 = 0;
        movie->unk_19 = 0;
        movie->unk_18 = 0;
        movie->unk_28 = -1;
        movie->unk_0A = 0;
        movie->unk_1A = 0;
        hizuke_disp_switch(0);
        amusement_scene_effect_frames = 0;
        for (i = 0; i < 2; i++) {
            sprite_quads[i + 4].w = (i << 6) + 0x80;
            sprite_quads[i + 4].flags = 0x82000000;
            sprite_quads[i + 4].h = 0xA0;
            sprite_quads[i + 4].unk_0E = 0;
            sprite_quads[i + 4].unk_0F = 0;
            sprite_quads[i + 4].unk_10 = 0;
            sprite_quads[i + 4].unk_0C = (i * 2) + 0x15;
            sprite_quads[i + 4].unk_12 = 0x1E0;
            sprite_quads[i + 4].tint_r = 0;
            sprite_quads[i + 4].tint_g = 0;
            sprite_quads[i + 4].tint_b = 0;
            sprite_quads[i + 4].anchor_x = 0;
            sprite_quads[i + 4].anchor_y = 0;
            sprite_quads[i + 4].unk_1C = 0x1000;
            sprite_quads[i + 4].unk_1E = 0x1000;
            sprite_quads[i + 4].unk_20 = 0;
            sprite_quads[i + 4].x = (i << 7) - 0xA0;
            sprite_quads[i + 4].y = -0x78;
            game_state.quad_layer[i + 4] = 0xB;
        }
        for (i = 0; i < 2; i++) {
            sprite_quads[i + 6].w = (i << 6) + 0x80;
            sprite_quads[i + 6].flags = 0x82000000;
            sprite_quads[i + 6].h = 0;
            sprite_quads[i + 6].unk_0E = 0;
            sprite_quads[i + 6].unk_0F = 0xA0;
            sprite_quads[i + 6].unk_10 = 0;
            sprite_quads[i + 6].unk_0C = (i * 2) + 0xA;
            sprite_quads[i + 6].unk_12 = 0x1E0;
            sprite_quads[i + 6].tint_r = 0;
            sprite_quads[i + 6].tint_g = 0;
            sprite_quads[i + 6].tint_b = 0;
            sprite_quads[i + 6].anchor_x = 0;
            sprite_quads[i + 6].anchor_y = 0;
            sprite_quads[i + 6].unk_1C = 0x1000;
            sprite_quads[i + 6].unk_1E = 0x1000;
            sprite_quads[i + 6].unk_20 = 0;
            sprite_quads[i + 6].x = (i << 7) - 0xA0;
            sprite_quads[i + 6].y = -0x78;
            game_state.quad_layer[i + 6] = 0xB;
        }
        dec_bg_cd_read(0x43B1, 1);
        g->mode_step_timer.u += 1;
        return;
    case 1:
    case 4:
    case 7:
    case 10:
    case 13:
    case 16:
    case 31:
    case 34:
    case 37:
        if (cd_read_sync_status() & 1) {
            g->mode_step_timer.u += 1;
            return;
        }
        return;
    case 2:
        bg_still_decode_to_vram(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x80183000);
        g->mode_step_timer.u += 1;
        return;
    case 3:
        dec_bg_cd_read(0x43AA, 1);
        g->mode_step_timer.u += 1;
        return;
    case 5:
        bg_still_decode_to_vram(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x8019C000);
        g->mode_step_timer.u += 1;
        return;
    case 6:
        dec_bg_cd_read(0x43A3, 1);
        g->mode_step_timer.u += 1;
        return;
    case 8:
        bg_still_decode_to_vram(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x801B5000);
        g->mode_step_timer.u += 1;
        return;
    case 9:
        dec_bg_cd_read(0x439C, 1);
        g->mode_step_timer.u += 1;
        return;
    case 11:
        bg_still_decode_to_vram(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x801CE000);
        g->mode_step_timer.u += 1;
        return;
    case 12:
        dec_bg_cd_read(0x4395, 1);
        g->mode_step_timer.u += 1;
        return;
    case 14:
        bg_still_decode_to_vram(0x280, 0, 0x140, 0xA0);
        g->mode_step_timer.u += 1;
        return;
    case 15:
        dec_bg_cd_read(0x438D, 1);
        g->mode_step_timer.u += 1;
        return;
    case 17:
        bg_still_decode_to_vram(0x140, 0x100, 0x140, 0xA0);
        g->mode_step_timer.u += 1;
        return;
    case 18:
        for (i = 0; i < 4; i++) {
            sprite_quads[i + 4].flags = (s32) (sprite_quads[i + 4].flags & 0x7FFFFFFF);
        }
        g->mode_step_timer.u++;
        return;
    case 19:
        for (i = 4; i < 8; i++) {
            func_80084C98(i, 0x80);
        }
        for (i = 0; i < 2; i++) {
            sprite_quads[i + 4].flags = 0x02000000;
            sprite_quads[i + 4].h = 0xA0;
            sprite_quads[i + 4].unk_0F = 0;
            sprite_quads[i + 4].y = -0x78;
        }
        for (i = 0; i < 2; i++) {
            sprite_quads[i + 6].flags = 0x02000000;
            sprite_quads[i + 6].h = 0;
            sprite_quads[i + 6].unk_0F = 0xA0;
            sprite_quads[i + 6].y = -0x78;
        }
        amusement_scene_effect_frames = 0;
        audio_command(0x500);
        g->mode_step_timer.u += 1;
        return;
    case 20:
    case 22:
    case 24:
        amusement_scene_quad_transition_step(8);
        if (++amusement_scene_effect_frames == 0x14) {
            g->mode_step_timer.u += 1;
            return;
        }
        break;
    case 21:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        LoadSquare(0x280U, 0U, 0x140U, 0xA0U, 0x801CE000);
        for (i = 0; i < 2; i++) {
            sprite_quads[i + 4].h = 0xA0;
            sprite_quads[i + 4].unk_0F = 0;
            sprite_quads[i + 4].y = -0x78;
        }
        for (i = 0; i < 2; i++) {
            sprite_quads[i + 6].h = 0;
            sprite_quads[i + 6].unk_0F = 0xA0;
            sprite_quads[i + 6].y = -0x78;
        }
        amusement_scene_effect_frames = 0;
        g->mode_step_timer.u += 1;
        return;
    case 23:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        LoadSquare(0x280U, 0U, 0x140U, 0xA0U, 0x801B5000);
        for (i = 0; i < 2; i++) {
            sprite_quads[i + 4].h = 0xA0;
            sprite_quads[i + 4].unk_0F = 0;
            sprite_quads[i + 4].y = -0x78;
        }
        for (i = 0; i < 2; i++) {
            sprite_quads[i + 6].h = 0;
            sprite_quads[i + 6].unk_0F = 0xA0;
            sprite_quads[i + 6].y = -0x78;
        }
        amusement_scene_effect_frames = 0;
        g->mode_step_timer.u += 1;
        return;
    case 25:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        LoadSquare(0x280U, 0U, 0x140U, 0xA0U, 0x8019C000);
        for (i = 0; i < 2; i++) {
            sprite_quads[i + 4].h = 0xA0;
            sprite_quads[i + 4].unk_0F = 0;
            sprite_quads[i + 4].y = -0x78;
        }
        for (i = 0; i < 2; i++) {
            sprite_quads[i + 6].h = 0;
            sprite_quads[i + 6].unk_0F = 0xA0;
            sprite_quads[i + 6].y = -0x78;
        }
        amusement_scene_effect_frames = 0;
        g->mode_step_timer.u += 1;
        return;
    case 26:
        amusement_scene_quad_transition_step(8);
        if ((u16) amusement_scene_effect_frames >= 8U) {
            value = sprite_quads[4].tint_r;
            value -= 4;
            color = (value & 0xFF);
            sprite_quads[7].tint_b = color;
            sprite_quads[7].tint_g = color;
            sprite_quads[7].tint_r = color;
            sprite_quads[6].tint_b = color;
            sprite_quads[6].tint_g = color;
            sprite_quads[6].tint_r = color;
            sprite_quads[5].tint_b = color;
            sprite_quads[5].tint_g = color;
            sprite_quads[5].tint_r = color;
            sprite_quads[4].tint_b = color;
            sprite_quads[4].tint_g = color;
            sprite_quads[4].tint_r = color;
        }
        if (++amusement_scene_effect_frames == 0x14) {
            g->mode_step_timer.u += 1;
            return;
        }
        break;
    case 27:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        LoadSquare(0x280U, 0U, 0x140U, 0xA0U, 0x80183000);
        for (i = 0; i < 2; i++) {
            sprite_quads[i + 4].h = 0xA0;
            sprite_quads[i + 4].unk_0F = 0;
            sprite_quads[i + 4].y = -0x78;
        }
        for (i = 0; i < 2; i++) {
            sprite_quads[i + 6].h = 0;
            sprite_quads[i + 6].unk_0F = 0xA0;
            sprite_quads[i + 6].y = -0x78;
        }
        amusement_scene_effect_frames = 0;
        g->mode_step_timer.u += 1;
        return;
    case 28:
        amusement_scene_quad_transition_step(8);
        value = sprite_quads[4].tint_r;
        value -= 4;
        color = (value & 0xFF);
        sprite_quads[7].tint_b = color;
        sprite_quads[7].tint_g = color;
        sprite_quads[7].tint_r = color;
        sprite_quads[6].tint_b = color;
        sprite_quads[6].tint_g = color;
        sprite_quads[6].tint_r = color;
        sprite_quads[5].tint_b = color;
        sprite_quads[5].tint_g = color;
        sprite_quads[5].tint_r = color;
        sprite_quads[4].tint_b = color;
        sprite_quads[4].tint_g = color;
        sprite_quads[4].tint_r = color;
        if (value < 4U) {
            g->mode_step_timer.u += 1;
            return;
        }
        break;
    case 29:
        for (i = 0; i < 4; i++) {
            sprite_quads[i + 4].flags = (s32) (sprite_quads[i + 4].flags | 0x80000000);
        }
        g->mode_step_timer.u++;
        return;
    case 30:
        saved_step_frames = g->mode_step_timer.u;

        previous_step = game_state.mode_step;
        read_bustup();
        g->mode_step_timer.u = saved_step_frames; if (previous_step != game_state.mode_step) { /* FAKE: one source line keeps the as1 load order T-11880 */
            game_state.mode_step -= 1;
            g->mode_step_timer.u++;
        }
        break;
    case 32:
        saved_step_frames = g->mode_step_timer.u;

        portrait_show_checked();
        game_state.mode_step -= 1;
        g->mode_step_timer.u = saved_step_frames;
        g->mode_step_timer.u++;
        return;
    case 33:
        cd_read_sectors_nowait(0xAU, 0x80162000, 0x433E);
        g->mode_step_timer.u += 1;
        return;
    case 35:
        if (((((DateMovieHeader8 *)bg_still_bitstream)->unk_00 & 0xFFFF0000) != 0x38000000) || ((((DateMovieHeader8 *)bg_still_bitstream)->unk_04 & 0xFF010000) != 0x10000)) {
            printf("DEC_BS header Error \n");
            movie->unk_04 = 0;
            movie->unk_06 = 0;
            g->mode_step_timer.u -= 2;
            return 0;
        }
        dec_bg_decode_to_vram(1);
        dec_bg_show_switch(1);
        g->mode_step_timer.u += 1;
        return;
    case 36:
        saved_step_frames = g->mode_step_timer.u;

        amusement_opening_sector_load();
        game_state.mode_step -= 1;
        g->mode_step_timer.u = saved_step_frames;
        g->mode_step_timer.u++;
        return;
    default:
        mode_step_next();
        D_80122D2C = 0;
        audio_command(0x200);
        hizuke_disp_switch(1);
        break;
    }
    return;
}

s32 amusement_scene_quad_transition_step(s16 arg0) {
    sprite_quads[4].y += arg0;
    sprite_quads[5].y += arg0;
    sprite_quads[4].h -= arg0;
    sprite_quads[5].h -= arg0;
    sprite_quads[6].h += arg0;
    sprite_quads[7].h += arg0;
    sprite_quads[6].unk_0F -= arg0;
    sprite_quads[7].unk_0F -= arg0;
}

/* Returns int like the DATE twin func_801489D0: only the header-error path of case 2 sets a
 * result (move v0,zero), the other paths leave v0 alone. T-18800 */
s32 amusement_random_scene_sprite_sequence(void) {
    s16 *temp_a0;
    s16 *temp_v0;
    s16 temp_a1;
    s16 temp_v0_2;
    s16 var_s3;
    s32 remainder;
    u32 saved_step_frames;
    u32 saved_step_frames_alt;
    u8 previous_step;
    u8 previous_step_alt;

    switch (game_state.mode_step_timer.u) {
    case 0:
        D_80122D2C = 1;
        srand((*(s32 *)&game_state.frame_count));
        amusement_scene_effect_frames = 0;
        hizuke_disp_switch(0);
        amusement_decoded_background_read_retry(0x43BA);
        D_8013A5D4[0] = -0x50;
        D_8013A5D4[1] = 0;
        D_8013A5D4[2] = 0x64;
        D_8013A5DC[0] = -0x28;
        D_8013A5DC[1] = 0;
        D_8013A5DC[2] = -0x3C;
        game_state.mode_step_timer.u += 1;
        D_8013A5E4[0] = (rand() % 3) - 1;
        D_8013A5E4[1] = (rand() % 3) - 1;
        D_8013A5E4[2] = (rand() % 3) - 1;
        D_8013A5EC[0] = (rand() % 3) - 1;
        D_8013A5EC[1] = (rand() % 3) - 1;
        D_8013A5EC[2] = (rand() % 3) - 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            game_state.mode_step_timer.u += 1;
            break;
        }
        break;
    case 2:
        if (((((u32 *)bg_still_bitstream)[0] & 0xFFFF0000) != 0x38000000) || ((((u32 *)bg_still_bitstream)[1] & 0xFF010000) != 0x10000)) {
            printf("DEC_BS header Error \n");
            ((RECT *)bg_lba_cache)[0].w = 0;
            ((RECT *)bg_lba_cache)[0].h = 0;
            game_state.mode_step_timer.u -= 2;
            return 0;
        }
        dec_bg_decode_to_vram(1);
        dec_bg_show_switch(1);
        audio_command(0x203);
        game_state.mode_step_timer.u += 1;
        break;
    case 3:
        previous_step = game_state.mode_step;
        saved_step_frames = game_state.mode_step_timer.u;
        normal_date_bg_fadein();
        game_state.mode_step_timer.u = saved_step_frames;
        if (previous_step != game_state.mode_step) {
            game_state.mode_step_timer.u = saved_step_frames + 1;
        }
        game_state.mode_step = previous_step;
        break;
    case 4:
        var_s3 = 0;
        do {
            if (((u32) (*(s32 *)&game_state.frame_count) % 30U) == (rand() % 30)) {
                (D_8013A5E4)[var_s3] = (rand() % 3) - 1;
            }

            if (((u32) (*(s32 *)&game_state.frame_count) % 30U) == (rand() % 30)) {
                (D_8013A5EC)[var_s3] = (rand() % 3) - 1;
            }

            if ((D_8013A5E4[var_s3] == 0) && (D_8013A5EC[var_s3] == 0)) {
                if ((remainder = rand() % 2)) {
                    D_8013A5E4[var_s3] = ((rand() % 2) * 2) - 1;
                } else {
                    D_8013A5EC[var_s3] = ((rand() % 2) * 2) - 1;
                }
            }
            temp_v0 = &(D_8013A5D4)[var_s3];
            *temp_v0 += D_8013A5E4[var_s3];
            temp_a0 = &(D_8013A5DC)[var_s3];
            *temp_a0 += D_8013A5EC[var_s3];
            temp_a1 = *temp_v0;
            if (temp_a1 < -0x95) {
                D_8013A5E4[var_s3] = 1;
            }
            if (temp_a1 >= 0x96) {
                D_8013A5E4[var_s3] = -1;
            }
            temp_v0_2 = *temp_a0;
            if (temp_v0_2 < -0x6D) {
                D_8013A5EC[var_s3] = 1;
            }
            if (temp_v0_2 >= 0x1E) {
                D_8013A5EC[var_s3] = -1;
            }
            var_s3++;
        } while (var_s3 < 3);
        prim_glow_disc((s32) D_8013A5D4[0], (s32) D_8013A5DC[0], 0xA, 2, 2, 0x808080, 3, 1);
        prim_glow_disc((s32) D_8013A5D4[1], (s32) D_8013A5DC[1], 0xA, 2, 2, 0x804040, 3, 1);
        prim_glow_disc((s32) D_8013A5D4[2], (s32) D_8013A5DC[2], 0xA, 2, 2, 0x800000, 3, 1);
        /* FAKE (T-18800): an empty test (no code) ends the block before the counter test; without it
         * IDO keeps the old count in a1 and branches with bnez instead of the original's v1 and
         * xori/sltiu/beqz (the DATE twin func_801489D0 has real code there). */
        if (var_s3) {
        }
        if (amusement_scene_effect_frames++ == 0x96) {
            game_state.mode_step_timer.u += 1;
            break;
        }
        break;
    case 5:
        previous_step_alt = game_state.mode_step;
        saved_step_frames_alt = game_state.mode_step_timer.u;
        normal_date_bg_fadeout();
        game_state.mode_step_timer.u = saved_step_frames_alt;
        if (previous_step_alt != game_state.mode_step) {
            game_state.mode_step_timer.u = saved_step_frames_alt + 1;
        }
        game_state.mode_step = previous_step_alt;
        break;
    case 6:
        dec_bg_show_switch(1);
        game_state.mode_step_timer.u += 1;
        break;
    default:
        audio_command(0x200);
        D_80122D2C = 0;
        hizuke_disp_switch(1);
        mode_step_next();
        break;
    }
}

void amusement_decoded_background_read_retry(s32 background_sector) {
    s32 v;

    cd_read_checked = 0;
    v = dec_bg_cd_read(background_sector, 1);
    if (v == D_800B5939) {
        printf("same\n");
        game_state.mode_step_timer.w += 3;
        return;
    }
    if (v == 1 - D_800B5939) {
        printf("another\n");
        game_state.mode_step_timer.w += 2;
    }
}

void amusement_birthday_callback_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl44 tbl;

    tbl = D_8013A600;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

s32 amusement_shiori_birthday_gate(void) {
    s32 v;

    v = get_g_zyotai_s(GIRL_SHIORI);
    if ((game_state.player_birthday & 0xF) == game_state.month && ((u32)(game_state.player_birthday << 0x17) >> 0x1B) == game_state.day) {
        if (((u8)v & 0x7F) < 2U) {
            game_state.day_event_done_bits |= 4;
            game_state.saved_mode_step = game_state.mode_step + 1;
            mode_stage_set(4);
            return 0;
        }
        game_state.day_event_done_bits |= 8;
    }
    mode_step_next();
}

/* The selected girl and the game_state flag bytes are read through a state
 * pointer: uopt then keeps unk_F5F and unk_71E in registers (a1, v0), which
 * gives the `lui a1; lbu a0,lo(a1)` argument loads. T-11880 */
s32 amusement_date_girl_birthday_gate(void) {
    GameState *g;
    s32 v;

    g = &game_state;
    v = get_g_zyotai_s(game_state.current_girl); /* FAKE: direct symbol here but g-> below; the mix decides which loads uopt keeps in registers T-11880 */
    if ((g->player_birthday & 0xF) == g->month && ((u32)(g->player_birthday << 0x17) >> 0x1B) == g->day) {
        if (((u8)v & 0x7F) < 2U) {
            g->day_event_done_bits &= ~8;
            g->day_event_done_bits |= 4;
            g->saved_mode_step = g->mode_step + 1;
            g->current_girl = g->date_girl;
            talk_names_set(g->current_girl);
            mode_stage_set(4);
            return 0;
        }
        g->day_event_done_bits |= 8;
    }
    mode_step_next();
}

s32 amusement_response_parameter_update(void) {
    u8 x;
    u8 count;

    if (game_state.substep == 0) {
        hud_day_bgm_load();
        game_state.substep++;
    }
    if (game_state.substep == 1) {
        if (game_state.mode_step_timer.w++ >= 0x400U) {
            cd_command_reset();
            audio_bank_state_reset();
            game_state.substep = 0;
        }
        if (audio_bank_load_poll() == 1) {
            game_state.substep++;
        } else {
            return 0;
        }
    }
    x = amusement_scene_choice_bits;
    count = ((x & 1) > 0) + ((x & 2) > 0) + ((x & 4) > 0);
    if (count == 0) {
        game_state.girls[0].affection += 0xC;
        game_state.girls[game_state.date_girl].syoushin += 7;
    } else if (count == 1) {
        game_state.girls[0].affection += 3;
        game_state.girls[game_state.date_girl].syoushin += 2;
    } else if (count == 2) {
        game_state.girls[game_state.date_girl].affection += 3;
        game_state.girls[0].syoushin += 2;
    } else {
        game_state.girls[game_state.date_girl].affection += 0xC;
        game_state.girls[0].syoushin += 7;
    }
    check_para_limit();
    day_event_next(1);
}

void amusement_status_random_scene_value_adjust(void) {
    amusement_scene_dialogue_group = 9;
    amusement_alternate_dialogue_group = 9;
    amusement_status_scene_value_adjust();
    if (((u8) get_g_zyotai_s(GIRL_SHIORI) & 0x7F) == 4 && (rand() % 2)) {
        amusement_scene_dialogue_group += 1;
    }
    if (((u8) get_g_zyotai_s(game_state.date_girl) & 0x7F) == 4 && (rand() % 2) && (game_state.date_girl != GIRL_NOZOMI)) {
        amusement_alternate_dialogue_group += 1;
    }
    mode_step_next();
}

void amusement_background_load(void) {
    bg_read_sub2(0x432B);
    mode_step_next();
}

void amusement_scene_candidate_select(void) {
    if (game_state.girls[0].affection < 0xA) {
        amusement_dialogue_group += 2;
        game_state.current_girl = GIRL_SHIORI;
    } else if (get_h_yuukou(game_state.current_girl) < 0xA) {
        amusement_dialogue_group += 2;
    } else if (game_state.girls[0].syoushin >= 0x33) {
        game_state.current_girl = GIRL_SHIORI;
    } else if (game_state.girls[game_state.current_girl].syoushin < 0x33) {
        if (game_state.girls[0].tokimeki >= 0x32) {
            amusement_dialogue_group += 1;
            game_state.current_girl = GIRL_SHIORI;
        } else if (get_h_tokimeki(game_state.current_girl) >= 0x32) {
            amusement_dialogue_group += 1;
        } else {
            amusement_dialogue_group += 4;
            mode_step_next();
            mode_step_next();
        }
    }
    mode_step_next();
}

void amusement_scene_value_default_set(void) {
    amusement_dialogue_group = 0x17;
    mode_step_next();
}

void amusement_date_girl_scene_value_set(void) {
    talk_names_set((u8) *(s8 *)&game_state.current_girl);
    amusement_dialogue_group = 0x15;
    mode_step_next();
}
