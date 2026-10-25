/* festival_stall_year_dispatch.c: ; src/ovl/EN_NICHI/80132000.c:63-73 branches the festival-stall flow by game year. Original object 0x80132000-0x80136B10 (config/objects/EN_NICHI.txt). */
#include "common.h"
#include "ovl/festival_stalls.h"


/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_80139200);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_80139204);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_80139208);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_8013920C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_80139210);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_80139214);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_80139218);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_8013921C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_80139220);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_80139224);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_80139228);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", stalls_wave_ages);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", stalls_wave_ring_index);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_801392A4);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_801392A8);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_801392AC);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_8013930C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_801393CC);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", stalls_objects);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_80139868);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_8013987C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_801398AC);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_80139A2C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_80139A34);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", stalls_selection_meter);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", stalls_object_substate);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", stalls_substate_frames);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", stalls_regular_object_count);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", stalls_kind4_object_count);
/* Ten status bytes followed by two bytes of end padding. T-10020. */
u8 stalls_object_proximity_classes[12] = { 0 };
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", stalls_saved_cursor_speed);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", stalls_pad_code_stage);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", stalls_audio_command_cooldown);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_80139B20);
s32 D_80139B24 = 0;
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/festival_stalls/festival_stall_year_dispatch.data", D_80139B28);

void stalls_year_variant_dispatch(void) {
    if (game_state.year == 0x5F) {
        stalls_first_year_mode_stage_dispatch();
    } else {
        stalls_later_year_callback_dispatch();
    }
}

void stalls_asset_pointer_setup(void) {
    D_801391E0 = 0x801E0400;
    D_801391E4 = 0x801E20F8;
    D_801391E8 = 0x801E2120;
    D_801391EC = 0x801E21C4;
    D_801391F0 = *(s16 *)0x801E5A10;
    D_801391F4 = 0x801B0000;
    D_801391F8 = 0x801E21CC;
    D_801391FC = 0x801E0408;
}

void stalls_first_year_mode_stage_dispatch(void) {
    switch (game_state.mode_stage) {
    case 0:
        stalls_intro_substep_dispatch();
        return;
    case 1:
        stalls_title_fade_input_dispatch();
        return;
    case 2:
        stalls_actor_graphics_setup();
        stalls_game_state_reset();
        mode_stage_next();
        return;
    case 3:
        stalls_game_frame_update();
        return;
    case 4:
        stalls_result_summary_dispatch();
        return;
    case 5:
        stalls_saved_caller_state_restore();
        /* fallthrough */
    default:
        return;
    }
}

void stalls_intro_substep_dispatch(void) {
    switch (game_state.mode_step) {
    case 0:
        stalls_intro_scene_reset();
        return;
    case 1:
        stalls_intro_sector_load();
        return;
    case 2:
        stalls_palette_load();
        return;
    default:
    case 3:
        mode_stage_next();
        return;
    }
}
void stalls_intro_scene_reset(void) {
    scene_screen_reset(1, 0);
    audio_command(0x7F);
    hud_cursor_actor_init(0);
    hud_cursor_show(0);
    srand(game_state.timer_ticks);
    hizuke_disp_switch(0);
    message_window_title_show(0);
    message_disp_switch(0);
    hud_mascot_display_switch(0);
    mode_step_next();
    stalls_saved_cursor_speed = game_state.cursor_speed;
    game_state.cursor_speed = SETTINGS_CURSOR_SPEED_SLOWEST;
}

void stalls_saved_caller_state_restore(void) {
    tpage_buf_clear();
    actor_clear_upper();
    hud_cursor_actor_init(0);
    game_state.clear_color_r = 0;
    game_state.clear_color_g = 0;
    game_state.clear_color_b = 0;
    back_clear_switch(1);
    game_state.cursor_speed = (s8) stalls_saved_cursor_speed;
    game_mode_set((s32) game_state.saved_game_mode);
    mode_stage_set((s32) game_state.saved_mode_stage);
    mode_step_set((s32) game_state.saved_mode_step);
}

void stalls_intro_sector_load(void) {
    if (game_state.substep == 0) {
        audio_command(0x201);
        cd_read_sectors(0x3F, 0x801B0000, 0x7C8C);
        game_state.substep += 1;
    } else if (cd_read_sync_status() & 1) {
        mode_step_next();
    }
}

void stalls_palette_load(void) {
    load_palette(D_80139210, 0x10, 1, 1, 0);
    load_palette(D_80139210, 0x11, 1, 1, 1);
    load_palette(D_80139210, 0x12, 1, 1, 2);
    load_palette(D_80139210, 0x13, 1, 1, 3);
    load_palette(D_80139210, 0x1F, 1, 3, 0);
    load_palette(D_80139210, 0x1E, 1, 3, 1);
    mode_step_next();
}

/* Loads the textures and sets up display records 96..159, 144..147 and 152..155. Implicit int (the
 * caller has no prototype). The second loop indexes records 96..115 absolutely; the same loop on
 * actor_pool_96 (record 96) puts its walk in other registers. T-20920 */
s32 stalls_actor_graphics_setup(void) {
    s32 i;

    load_tpage_buf_lock(5, D_80139204[0], 1);
    load_tpage_buf_lock(0x25, D_80139204[1], 1);
    load_tpage_buf_lock(6, D_80139204[2], 1);
    load_tpage_buf_lock(7, 0x801BA000, 1);
    load_tpage_buf_lock(8, 0x801BA000, 1);
    load_tpage_buf_lock(9, 0x801BA000, 1);
    load_tpage_buf_lock(0x27, 0x801BA000, 1);
    load_tpage_buf_lock(0x28, 0x801BA000, 1);
    load_tpage_buf_lock(0x29, 0x801BA000, 1);
    for (i = 0; i < 64; i++) {
        actor_clear(i + 96);
        ((Actor *)actor_pool)[i + 96].unk_01 = 7;
        ((Actor *)actor_pool)[i + 96].quad_flags = 0x01000000;
        ((Actor *)actor_pool)[i + 96].brightness = 0;
        ((Actor *)actor_pool)[i + 96].flags = 0x40;
        ((Actor *)actor_pool)[i + 96].draw_flags = 4;
        ((Actor *)actor_pool)[i + 96].blink_period = 1;
        ((Actor *)actor_pool)[i + 96].x = -0xA0;
        ((Actor *)actor_pool)[i + 96].y = -0x78;
        ((Actor *)actor_pool)[i + 96].unk_05 = 0;
        ((Actor *)actor_pool)[i + 96].unk_43 = 0x11;
        ((Actor *)actor_pool)[i + 96].unk_04 = 0;
        ((Actor *)actor_pool)[i + 96].frame_table = (u8 *)D_80139208;
        ((Actor *)actor_pool)[i + 96].sequence_table = (u8 *)D_8013920C;
        ((Actor *)actor_pool)[i + 96].tpage_table = (u8 *)D_80139204;
    }
    ((Actor *)actor_pool)[154].draw_flags = 0x80;
    ((Actor *)actor_pool)[154].unk_18 = 0x22;
    ((Actor *)actor_pool)[153].draw_flags = 0x80;
    ((Actor *)actor_pool)[153].unk_18 = 0x20;
    ((Actor *)actor_pool)[152].unk_01 = 3;
    ((Actor *)actor_pool)[152].draw_flags = 0x84;
    ((Actor *)actor_pool)[152].unk_18 = 0x1F;
    ((Actor *)actor_pool)[152].x = ((Actor *)actor_pool)[0].x;
    ((Actor *)actor_pool)[152].y = ((Actor *)actor_pool)[0].y;
    for (i = 96; i < 116; i++) {
        ((Actor *)actor_pool)[i].unk_01 = 3;
        ((Actor *)actor_pool)[i].unk_18 = 0x2E;
    }
    ((Actor *)actor_pool)[155].unk_01 = 2;
    ((Actor *)actor_pool)[155].unk_18 = 0x2D;
    ((Actor *)actor_pool)[155].quad_flags = 0x51000000;
    ((Actor *)actor_pool)[155].unk_43 = 0x1E;
    ((Actor *)actor_pool)[144].unk_01 = 2;
    ((Actor *)actor_pool)[144].x = 0x10;
    ((Actor *)actor_pool)[144].y = -8;
    ((Actor *)actor_pool)[145].unk_01 = 2;
    ((Actor *)actor_pool)[145].x = 0;
    ((Actor *)actor_pool)[145].y = -8;
    ((Actor *)actor_pool)[146].unk_01 = 2;
    ((Actor *)actor_pool)[146].x = 0x10;
    ((Actor *)actor_pool)[146].y = 0x18;
    ((Actor *)actor_pool)[147].unk_01 = 2;
    ((Actor *)actor_pool)[147].x = 0;
    ((Actor *)actor_pool)[147].y = 0x18;
}

void stalls_game_state_reset(void) {
    s32 index;
    s32 *word;

    for (index = 0; index < 10; index++) {
        stalls_random_object_spawn(index, 0);
    }
    stalls_particle_grid_reset();
    /* FAKE: reusing index for this reset keeps the first loop bound
     * as an immediate comparison instead of a saved register. T-10020. */
    index = 0;
    stalls_wave_ring_index = index;
    /* FAKE: the shared line preserves the pointer setup schedule. T-10020. */
    word = stalls_wave_ages; do {
        *word++ = 0;
    } while (word < &stalls_wave_ring_index);
    D_80139B20 = 0;
    stalls_selection_meter = 0;
    stalls_object_substate = 0;
    stalls_substate_frames = 0;
    stalls_regular_object_count = 0;
    stalls_kind4_object_count = 0;
    for (index = 0; index < 10; index++) {
        stalls_object_proximity_classes[index] = 0;
    }
    stalls_pad_code_stage = 0;
}

s32 stalls_title_fade_input_dispatch(void) {
    s32 i;

    switch (game_state.mode_step) {
    case 0:
        game_state.screen_brightness = 0;
        layer_tileset_init(0, (u32 *) D_80139218, D_80139200);
        srn_init(0, (u32 *) D_80139214, 0);
        srn_vram_set(0, 5, 0x580, 0xF0);
        Fade_In_Init();
        mode_step_next();
        break;
    case 1:
        if (cd_audio_state() == 2) {
            mode_step_next();
        }
        break;
    case 2:
        Fade_In_Main();
        if (game_state.pad_pressed & 0x60) {
            audio_command(0x74);
            Fade_Out_Init();
            mode_step_next();
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        break;
    case 3:
        if (Fade_Out_Main() != 0) {
            /* FAKE: the first two of six iterations written out: the original stores them through absolute
             * addresses with one constant each, then runs the loop from 2; one 6-trip loop unrolls the other
             * way in this IDO. T-18200 */
            game_state.tpage_buf[5] = -1;
            game_state.tpage_buf[37] = -1;
            game_state.tpage_buf[6] = -1;
            game_state.tpage_buf[38] = -1;
            for (i = 2; i < 6; i++) {
                game_state.tpage_buf[i + 5] = -1;
                game_state.tpage_buf[i + 37] = -1;
            }
            audio_command(0x201);
            mode_stage_next();
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void stalls_game_frame_update(void) {
    s32 v;

    stalls_wave_response_update();
    v = stalls_audio_command_cooldown - 1;
    stalls_audio_command_cooldown = v;
    if (v < 0) {
        stalls_audio_command_cooldown = 0;
    }
    stalls_brightness_ramp_up();
    stalls_particle_field_update();
    stalls_light_animation_update();
    stalls_object_mode_substate_dispatch();
    stalls_object_scene_update();
}

/* FAKE: permuter expression/layout spelling preserves target scheduling; original source unknown. T-9710. */
s32 stalls_cursor_distance_squared(x1, y1, x2, y2)
s16 x1;
s16 y1;
s16 x2;
s16 y2;
{
    s32 dx;
    s32 dy;
    u32 x;
    u32 y;
    x = (dx = x1 - x2); dy = (y = y1 - y2); if (dx < 0) { x = -dx; } if (dy < 0) {
        y = -dy;
    }
    x *= x;
    y *= y;
    return x + y;
}

void stalls_light_animation_update(void) {
    D_80139B24 += 1;
    if (D_80139B24 >= 0x36) {
        D_80139B24 = 0;
    }
    switch (D_80139B24) {
    case 0:
        D_8012158C = 0x20;
        break;
    case 0x1B:
        D_8012158C = 0x21;
        break;
    }
}

s32 stalls_brightness_ramp_up(void) {
    s32 i;

    if (D_8013921C == 0x7C) {
        D_8013921C = 0x80;
        for (i = 0; i < 64; i++) {
            ((Actor *)actor_pool)[i + 96].brightness = 0x80;
        }
        game_state.clear_color_r = 0x40;
        game_state.clear_color_g = 0x40;
        game_state.clear_color_b = 0x80;
        return;
    }
    if (D_8013921C < 0x80) {
        D_8013921C += 4;
        for (i = 0; i < 64; i++) {
            ((Actor *)actor_pool)[i + 96].brightness += 4;
        }
        game_state.clear_color_r += 2;
        game_state.clear_color_g += 2;
        game_state.clear_color_b += 4;
    }
}

void stalls_brightness_ramp_down(void) {
    s32 i;

    if (D_8013921C == 4) {
        D_8013921C = 0;
        for (i = 0; i < 64; i++) {
            ((Actor *)actor_pool)[i + 96].brightness = 0;
        }
        game_state.clear_color_r = 0;
        game_state.clear_color_g = 0;
        game_state.clear_color_b = 0;
        return;
    }
    if (D_8013921C > 0) {
        D_8013921C -= 4;
        for (i = 0; i < 64; i++) {
            ((Actor *)actor_pool)[i + 96].brightness -= 4;
        }
        game_state.clear_color_r -= 2;
        game_state.clear_color_g -= 2;
        game_state.clear_color_b -= 4;
    }
}

void stalls_object_mode_substate_dispatch(void) {
    switch (stalls_object_substate) {                           /* irregular */
    case 0:
        stalls_target_selection_score_update();
        return;
    case 1:
        stalls_target_release_score_update();
        return;
    case 2:
        stalls_score_completion_update();
        return;
    case 3:
        stalls_idle_timeout_update();
        return;
    }
}

void stalls_idle_timeout_update(void) {
    if (stalls_substate_frames == 0) {
        audio_command(0x504);
    }
    stalls_substate_frames += 1;
    D_80121548 = 0x1E;
    stalls_cursor_and_selection_input();
    if (stalls_substate_frames >= 0xB4) {
        D_80121533 = 0;
        D_80121531 = 3;
        mode_stage_next();
    }
}

void stalls_cursor_and_selection_input(void) {
    stalls_cursor_position_update();
    if (game_state.pad_held & PAD_CIRCLE) {
        if (D_80121531 != 7) {
            stalls_wave_start(cursor_x, cursor_y);
            stalls_audio_command_debounce(0x501);
        }
        D_80121531 = 7;
        return;
    }
    if (D_80121531 != 3) {
        stalls_wave_start(cursor_x, cursor_y);
        stalls_audio_command_debounce(0x502);
    }
    D_80121531 = 3;
}

/* FAKE: end before i = 0 on one line and non-invariant * 0 on LHS of != align preheader la order and bne v0,a3 operand order. T-24131 */
void stalls_score_completion_update(void) {
    s32 i;
    EnObj *end;

    stalls_substate_frames++;
    if (stalls_selection_meter >= 80 && stalls_substate_frames >= 30) {
        end = &stalls_objects[10], i = 0; do {
            if (stalls_objects[i].kind_flags & 0x80) {
                stalls_objects[i].kind_flags &= 0x7F;
                if (stalls_objects[i].kind_flags == 4) {
                    stalls_kind4_object_count--;
                } else {
                    stalls_regular_object_count--;
                }
            }
            i++;
        } while ((EnObj *)((u8 *)end + (stalls_objects->kind_flags * 0)) != &stalls_objects[i]);
        game_state.mouse_x = ((Actor *)actor_pool)[152].x;
        game_state.mouse_y = ((Actor *)actor_pool)[152].y;
        stalls_wave_start(((Actor *)actor_pool)->x, ((Actor *)actor_pool)->y);
        stalls_audio_command_debounce(0x502);
        stalls_object_substate = 3;
    }
    if (stalls_substate_frames >= 60) {
        game_state.mouse_x = ((Actor *)actor_pool)[152].x;
        game_state.mouse_y = ((Actor *)actor_pool)[152].y;
        stalls_substate_frames = 0;
        stalls_object_substate = 0;
        stalls_audio_command_debounce(0x50D);
        for (i = 0; i < 10; i++) {
            if (stalls_objects[i].kind_flags & 0x80) {
                stalls_objects[i].kind_flags = 0;
            }
        }
    }
}

/* Moves display record 0 by the pad bits of game_state.pad_held in 16.16 steps (diagonals
 * by v * 0xB505, v / sqrt 2), clamps its integer position and copies it to record 152. The words go
 * through the record view, so their loads stay after the stores as in the original; the clamp goes
 * through a pointer to the pool, whose reloads after each store and $v1 base match. Replaces the
 * T-10610 NON_MATCHING draft. T-20920 */
void stalls_cursor_position_update(void) {
    s32 sel;
    s32 v;
    s32 d;
    Actor *p;

    sel = game_state.pad_held;
    if (sel & 0x40) {
        v = 3;
    } else {
        v = 2;
    }
    if (game_state.input_device == INPUT_PAD) {
        switch (sel & ~0x60) {
        case 0x1000:
            ((ActorFixed *)actor_pool)->y.w -= v << 16;
            break;
        case 0x2000:
            ((ActorFixed *)actor_pool)->x.w += v << 16;
            break;
        case 0x4000:
            ((ActorFixed *)actor_pool)->y.w += v << 16;
            break;
        case 0x8000:
            ((ActorFixed *)actor_pool)->x.w -= v << 16;
            break;
        case 0x3000:
            d = v * 0xB505;
            ((ActorFixed *)actor_pool)->x.w += d;
            ((ActorFixed *)actor_pool)->y.w -= d;
            break;
        case 0x6000:
            d = v * 0xB505;
            ((ActorFixed *)actor_pool)->x.w += d;
            ((ActorFixed *)actor_pool)->y.w += d;
            break;
        case 0xC000:
            d = v * 0xB505;
            ((ActorFixed *)actor_pool)->x.w -= d;
            ((ActorFixed *)actor_pool)->y.w += d;
            break;
        case 0x9000:
            d = v * 0xB505;
            ((ActorFixed *)actor_pool)->x.w -= d;
            ((ActorFixed *)actor_pool)->y.w -= d;
            break;
        }
    }
    p = (Actor *)actor_pool;
    if (p->x < -0x8F) {
        p->x = -0x90;
    }
    if (p->x >= 0x90) {
        p->x = 0x90;
    }
    if (p->y < -0x67) {
        p->y = -0x68;
    }
    if (p->y >= 0x68) {
        p->y = 0x68;
    }
    p[152].x = p->x;
    p[152].y = p->y;
}

/* FAKE: s32 without return keeps the clamp accumulator in v0 and the original
 * split epilogue. Original return type is unconfirmed. T-14000. */
s32 stalls_target_selection_score_update(void) {
    s32 i;

    stalls_cursor_position_update();
    stalls_substate_frames += 1;
    if ((game_state.pad_held & PAD_CIRCLE) && stalls_substate_frames >= 4) {
        stalls_wave_start(cursor_x, cursor_y);
        stalls_audio_command_debounce(0x501);
        stalls_object_substate = 1;
        stalls_substate_frames = 0;
        D_80121531 = 7;
        stalls_selection_meter += 2;
        stalls_cursor_object_proximity_classify();
        for (i = 0; i < 10; i++) {
            if (stalls_object_proximity_classes[i] == 2) {
                stalls_selection_meter += 0x20;
            } else if (stalls_object_proximity_classes[i] == 1) {
                stalls_selection_meter += 0x10;
            }
        }
        if (stalls_selection_meter >= 0x50) {
            stalls_selection_meter = 0x64;
        }
    }
}

/* FAKE: * 0 and | 0 idioms align uopt constant LIFO order ($t0..$t2), CSE priority
 * ($v0/$v1), and bne operand order without emitting instructions in ugen. T-24131 */
s32 stalls_target_release_score_update(void) {
    s32 i;
    s32 v;

    stalls_cursor_position_update();
    stalls_substate_frames += 1;
    if (stalls_substate_frames >= 0x19) {
        stalls_substate_frames = 0;
        stalls_selection_meter += 1;
        if (stalls_selection_meter >= 0x50) {
            stalls_object_substate = 3;
            return 0;
        }
    }
    if (!(game_state.pad_held & PAD_CIRCLE) && stalls_substate_frames >= 4) {
        stalls_wave_start(cursor_x, cursor_y);
        stalls_audio_command_debounce(0x502);
        stalls_object_substate = 0;
        D_80121531 = 3;
        stalls_substate_frames = 0;
        stalls_cursor_object_proximity_classify();
        for (i = 0; i < 10; i++) {
            if (stalls_object_proximity_classes[i] == 2) {
                v = ((stalls_kind4_object_count == 4) * 0) + stalls_objects[i].kind_flags;
                stalls_object_substate = ((s32)&stalls_objects[i] * 0) + 2;
                if ((v | 0) == 4) {
                    stalls_selection_meter += 0x50;
                    stalls_kind4_object_count += 1;
                } else {
                    stalls_selection_meter += 0x10;
                    stalls_regular_object_count += 1;
                }
                stalls_objects[i].kind_flags = v | 0x80;
            } else if (stalls_object_proximity_classes[i] == 1) {
                v = stalls_objects[i].kind_flags;
                stalls_object_substate = ((s32)&stalls_objects[i] * 0) + 2;
                if ((v | 0) == 4) {
                    stalls_kind4_object_count += 1;
                } else {
                    stalls_regular_object_count += 1;
                }
                stalls_objects[i].kind_flags = v | 0x80;
            }
        }
    }
}


void stalls_cursor_object_proximity_classify(void) {
    s32 distance;
    s32 radius;
    u8 *status;
    EnObj *object;

    radius = ((s32) stalls_regular_object_count / 2) + 5;
    if (radius >= 0xB) {
        radius = 0xA;
    }
    radius *= radius;
    /* FAKE: this shared line preserves the loop-setup load schedule. T-10020. */
    object = stalls_objects; status = stalls_object_proximity_classes; do {
        distance = stalls_cursor_distance_squared((s32) object->x.h[1], (s32) object->y.h[1], (s32) cursor_x, (s32) cursor_y);
        if (object->kind_flags != 0) {
            if (distance < radius) {
                *status = 2;
            } else if (distance < 0x100) {
                *status = 1;
            } else {
                *status = 0;
            }
        } else {
            *status = 0;
        }
        status += 1;
        object++;
    } while (status != D_80139B12);
}

void stalls_object_scene_update(void) {
    stalls_object_out_of_bounds_clear();
    stalls_random_object_spawn(1);
    stalls_object_ai_update_dispatch();
    stalls_object_actor_render_update();
}

void stalls_object_ai_update_dispatch(void) {
    s32 i;
    s32 *p;
    u8 n;

    p = (s32 *) stalls_objects;
    for (i = 0, n = 10; i != n; i++) {
        if (!(p[0] & 0x80)) {
            if ((D_80121531 == 7) && (p[1] != 0)) {
                stalls_object_special_ai_update(i);
                stalls_object_motion_update(i);
            } else {
                stalls_object_random_steering_update(i);
                stalls_object_motion_update(i);
            }
        }
        p += 12;
    }
}

/* Moves object arg0: kind 4 drifts at a fixed step, the others steer by unk_2C and move at the
 * speed of class unk_28; unk_14/unk_18 are 16.16 positions, unk_1C the angle (4096 = 360 degrees).
 * Index form: a pointer local changes the spill slot and the frame. T-20920 */
void stalls_object_motion_update(s32 object_index) {
    stalls_objects[object_index].steering_frames--;
    if (stalls_objects[object_index].kind_flags == 4) {
        stalls_objects[object_index].x.w += (func_800A0140(stalls_objects[object_index].heading) * 0x8000) >> 12;
        stalls_objects[object_index].y.w += (rsin(stalls_objects[object_index].heading) * 0x8000) >> 12;
    } else {
        switch (stalls_objects[object_index].speed_class) {
        case 0:
            stalls_objects[object_index].movement_step = 0xC000;
            break;
        case 1:
            stalls_objects[object_index].movement_step = 0xF000;
            break;
        case 2:
            stalls_objects[object_index].movement_step = 0x12000;
            break;
        case 3:
            stalls_objects[object_index].movement_step = 0x15000;
            break;
        case 4:
            stalls_objects[object_index].movement_step = 0x18000;
            break;
        case 5:
            stalls_objects[object_index].movement_step = 0x1B000;
            break;
        case 6:
            stalls_objects[object_index].movement_step = 0x1E000;
            break;
        case 7:
            stalls_objects[object_index].movement_step = 0x21000;
            break;
        case 8:
            stalls_objects[object_index].movement_step = 0x24000;
            break;
        }
        if (stalls_objects[object_index].steering_flags == 1) {
            if (stalls_objects[object_index].steering_frames % 40 < 10) {
                stalls_objects[object_index].heading += 30;
            } else if (stalls_objects[object_index].steering_frames % 40 >= 20 && stalls_objects[object_index].steering_frames % 40 < 30) {
                stalls_objects[object_index].heading -= 30;
            }
        }
        if (stalls_objects[object_index].steering_flags & 0x10) {
            stalls_objects[object_index].heading += 20;
        }
        if (stalls_objects[object_index].steering_flags & 0x20) {
            stalls_objects[object_index].heading -= 20;
        }
        switch (stalls_objects[object_index].steering_flags) {
        case 0x40:
            stalls_objects[object_index].movement_step = 0xC000;
            stalls_objects[object_index].heading += 0x100;
            break;
        case 0x41:
            stalls_objects[object_index].movement_step = 0xC000;
            stalls_objects[object_index].heading += 0x200;
            break;
        case 0x80:
            stalls_objects[object_index].movement_step = 0xC000;
            stalls_objects[object_index].heading -= 0x100;
            break;
        case 0x81:
            stalls_objects[object_index].movement_step = 0xC000;
            stalls_objects[object_index].heading -= 0x200;
            break;
        }
        if (stalls_objects[object_index].heading >= 0x1000) {
            stalls_objects[object_index].heading -= 0x1000;
        }
        if (stalls_objects[object_index].heading < 0) {
            stalls_objects[object_index].heading += 0x1000;
        }
        stalls_objects[object_index].x.w += (stalls_objects[object_index].movement_step * func_800A0140(stalls_objects[object_index].heading)) >> 12;
        stalls_objects[object_index].y.w += (stalls_objects[object_index].movement_step * rsin(stalls_objects[object_index].heading)) >> 12;
    }
}

void stalls_object_actor_render_update(void) {
    s32 i;
    s16 v;

    for (i = 0; i != 10; i++) {
        if (stalls_objects[i].kind_flags & 0x80) {
            ((Actor *)actor_pool)[128 + i].unk_01 = 3;
            stalls_objects[i].unk_10 += 4;
            stalls_object_collision_actor_setup(i, 1);
        } else {
            stalls_objects[i].unk_10 += 1;
            ((Actor *)actor_pool)[128 + i].unk_01 = 7;
            stalls_object_collision_actor_setup(i, 0);
        }
        if (stalls_objects[i].unk_10 >= 15) {
            stalls_objects[i].unk_10 = 0;
            stalls_objects[i].unk_0C++;
        }
        switch (stalls_objects[i].kind_flags & 0x7F) {
        case 0:
        default:
            ((Actor *)actor_pool)[128 + i].draw_flags = 0;
            break;
        case 1:
        case 2:
        case 3:
            if (stalls_objects[i].unk_0C >= 6) {
                stalls_objects[i].unk_0C = 0;
            }
            v = D_80139A34[stalls_object_direction_sector_select(i)][stalls_objects[i].unk_0C];
            if (v & 0x80) {
                ((Actor *)actor_pool)[128 + i].draw_flags = 0x85;
            } else {
                ((Actor *)actor_pool)[128 + i].draw_flags = 0x84;
            }
            ((Actor *)actor_pool)[128 + i].unk_18 = v & 0x7F;
            ((Actor *)actor_pool)[128 + i].unk_43 = D_80139A2C[stalls_objects[i].kind_flags & 0x7F];
            ((Actor *)actor_pool)[128 + i].x = stalls_objects[i].x.h[1];
            ((Actor *)actor_pool)[128 + i].y = stalls_objects[i].y.h[1];
            break;
        case 4:
            if (stalls_objects[i].unk_0C >= 4) {
                stalls_objects[i].unk_0C = 0;
            }
            v = D_80139A34[stalls_object_direction_sector_select(i) + 8][stalls_objects[i].unk_0C];
            if (v & 0x80) {
                ((Actor *)actor_pool)[128 + i].draw_flags = 0x85;
            } else {
                ((Actor *)actor_pool)[128 + i].draw_flags = 0x84;
            }
            ((Actor *)actor_pool)[128 + i].unk_18 = v & 0x7F;
            ((Actor *)actor_pool)[128 + i].unk_43 = D_80139A2C[stalls_objects[i].kind_flags & 0x7F];
            ((Actor *)actor_pool)[128 + i].x = stalls_objects[i].x.h[1];
            ((Actor *)actor_pool)[128 + i].y = stalls_objects[i].y.h[1];
            break;
        }
    }
}

void stalls_object_collision_actor_setup(object_index, collected)
s32 object_index;
s32 collected;
{
    u8 *rec;
    s32 j;
    EnObj *e;
    s16 x;
    s16 y;

    j = object_index * 2; /* FAKE: the doubled index as its own variable keeps the original's sll-first multiply (T-9330) */
    rec = actor_pool + j * 0x44;
    rec[0x1983] = 4;
    rec[0x19C7] = 5;
    if (collected == 1) {
        e = &stalls_objects[object_index];
        if (e->kind_flags == 4) {
            if (!(e->unk_0C & 1)) {
                x = e->x.h[1];
                y = e->y.h[1];
                rec[0x1983] |= 0x80;
                rec[0x19C7] |= 0x80;
                *(s16 *) (rec + 0x19A6) = x - 0x10;
                *(s16 *) (rec + 0x19AA) = y - 7;
                *(s16 *) (rec + 0x19EA) = x + 0x10;
                *(s16 *) (rec + 0x19EE) = y + 7;
            }
        } else if (!(e->unk_0C & 1)) {
            x = e->x.h[1];
            y = e->y.h[1];
            rec[0x1983] |= 0x80;
            rec[0x19C7] |= 0x80;
            *(s16 *) (rec + 0x19A6) = x - 0x10;
            *(s16 *) (rec + 0x19AA) = y - 7;
            *(s16 *) (rec + 0x19EA) = x + 0x10;
            *(s16 *) (rec + 0x19EE) = y + 7;
        }
    }
}

s32 stalls_object_direction_sector_select(s32 object_index) {
    s32 v;

    v = stalls_objects[object_index].heading;
    if (v < 0x100) {
        return 0;
    }
    if (v < 0x300) {
        return 1;
    }
    if (v < 0x500) {
        return 2;
    }
    if (v < 0x700) {
        return 3;
    }
    if (v < 0x900) {
        return 4;
    }
    if (v < 0xB00) {
        return 5;
    }
    if (v < 0xD00) {
        return 6;
    }
    if (v < 0xF00) {
        return 7;
    }
    return 0;
}

void stalls_object_out_of_bounds_clear(void) {
    s32 i;
    s32 x;
    s32 y;

    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 0; i < 10; i++) if (stalls_objects[i].x.h[1] < -0xC0 || stalls_objects[i].x.h[1] >= 0xC1 || stalls_objects[i].y.h[1] < -0x98 || stalls_objects[i].y.h[1] >= 0x99) stalls_objects[i].kind_flags = 0;
}

void stalls_random_object_spawn(s32 edge_spawn) {
    s32 n;
    s32 m;
    s32 r;
    s32 i;

    stalls_object_type_count(&n, &m);
    for (i = 0; i != 10; i++) {
        if (stalls_objects[i].kind_flags == 0 && n * 10 < rand() % 100) {
            r = rand();
            if (m == 0 && r % 100 >= 61) {
                stalls_objects[i].kind_flags = 4;
                m++;
            } else {
                stalls_objects[i].kind_flags = rand() % 3 + 1;
                n++;
            }
            r = rand() % 100;
            if (r < 20) {
                stalls_objects[i].steering_profile = 0;
            } else if (r < 60) {
                stalls_objects[i].steering_profile = 1;
            } else {
                stalls_objects[i].steering_profile = 2;
            }
            r = rand() % 100;
            if (r < 20) {
                stalls_objects[i].speed_profile = 0;
            } else if (r < 60) {
                stalls_objects[i].speed_profile = 1;
            } else {
                stalls_objects[i].speed_profile = 2;
            }
            if (stalls_objects[i].kind_flags == 4) {
                stalls_objects[i].steering_profile = 0;
            }
            stalls_objects[i].unk_0C = 0;
            stalls_objects[i].unk_10 = 0;
            stalls_object_spawn_position_init(i, edge_spawn);
        }
    }
}

void stalls_object_spawn_position_init(s32 object_index, s32 edge_spawn) {
    s32 temp_hi;
    s32 temp_hi_2;
    s32 spawn_edge;
    EnObj *object;

    if (edge_spawn == 1) {
        spawn_edge = rand() % 4;
        switch (spawn_edge) {
        default:
            object = &stalls_objects[object_index];
            break;
        case 0:
            temp_hi = rand() % 320 - 0xA0;
            object = &stalls_objects[object_index];
            object->x.h[1] = temp_hi;
            object->y.h[1] = -0x96;
            object->heading = (s32) ((rand() % 1024) + 0x200);
            break;
        case 1:
            object = &stalls_objects[object_index];
            object->x.h[1] = 0xBE;
            object->y.h[1] = (s16) ((rand() % 240) - 0x78);
            object->heading = (s32) ((rand() % 1024) + 0x600);
            break;
        case 2:
            temp_hi_2 = rand() % 320 - 0xA0;
            object = &stalls_objects[object_index];
            object->x.h[1] = temp_hi_2;
            object->y.h[1] = 0x96;
            object->heading = (s32) ((rand() % 1024) + 0xA00);
            break;
        case 3:
            object = &stalls_objects[object_index];
            object->x.h[1] = -0xBE;
            object->y.h[1] = (s16) ((rand() % 240) - 0x78);
            object->heading = (s32) ((rand() % 1024) + 0xE00);
            break;
        }
    } else {
        object = &stalls_objects[object_index];
        object->x.h[1] = (s16) ((rand() % 320) - 0xA0);
        object->y.h[1] = (s16) ((rand() % 240) - 0x78);
        object->heading = (s32) (rand() % 4096);
    }
    object->steering_frames = 1;
    if (object->heading >= 0x1000) {
        object->heading -= 0x1000;
    }
    if (object->heading < 0) {
        object->heading += 0x1000;
    }
}

void stalls_object_type_count(s32 *regular_count, s32 *kind4_count) {
    s32 i;

    *regular_count = 0;
    *kind4_count = 0;
    for (i = 0; i < 10; i++) {
        switch (stalls_objects[i].kind_flags) {
        case 0:
            break;
        case 1:
        case 2:
        case 3:
            (*regular_count)++;
            break;
        case 4:
            (*kind4_count)++;
            break;
        }
    }
}

/* When the countdown unk_24 of object arg0 has run out, picks its next unk_28 (the speed class of
 * stalls_object_motion_update) and unk_2C (the steering mode) at random. Index form as in T-18200. T-20920 */
void stalls_object_random_steering_update(s32 object_index) {
    s32 r;

    if (stalls_objects[object_index].steering_frames <= 0) {
        r = rand();
        switch (stalls_objects[object_index].speed_profile) {
        case 0:
            if (r % 100 < 80) {
                stalls_objects[object_index].speed_class = 0;
            } else {
                stalls_objects[object_index].speed_class = 1;
            }
            break;
        case 1:
            if (r % 100 < 10) {
                stalls_objects[object_index].speed_class = 0;
            } else if (r % 100 < 90) {
                stalls_objects[object_index].speed_class = 1;
            } else {
                stalls_objects[object_index].speed_class = 2;
            }
            break;
        case 2:
            if (r % 100 < 20) {
                stalls_objects[object_index].speed_class = 1;
            } else {
                stalls_objects[object_index].speed_class = 2;
            }
            break;
        }
        r = rand();
        if (stalls_objects[object_index].steering_flags == 0 || stalls_objects[object_index].steering_flags == 1) {
            if (r % 100 < 25) {
                stalls_objects[object_index].steering_flags = 0;
            } else if (r % 100 < 40) {
                stalls_objects[object_index].steering_flags = 1;
            } else if (r % 100 < 70) {
                stalls_objects[object_index].steering_flags = 0x10;
            } else {
                stalls_objects[object_index].steering_flags = 0x20;
            }
        } else if (stalls_objects[object_index].steering_flags & 0x50) {
            if (r % 100 < 40) {
                stalls_objects[object_index].steering_flags = 0;
            } else if (r % 100 < 80) {
                stalls_objects[object_index].steering_flags = 1;
            } else {
                stalls_objects[object_index].steering_flags = 0x20;
            }
        } else if (stalls_objects[object_index].steering_flags & 0xA0) {
            if (r % 100 < 40) {
                stalls_objects[object_index].steering_flags = 0;
            } else if (r % 100 < 80) {
                stalls_objects[object_index].steering_flags = 1;
            } else {
                stalls_objects[object_index].steering_flags = 0x10;
            }
        }
        if ((stalls_objects[object_index].steering_flags & 0x10) || (stalls_objects[object_index].steering_flags & 0x20)) {
            r = rand() % 100;
            if (r < 33) {
                stalls_objects[object_index].steering_flags |= 1;
            } else if (r < 66) {
                stalls_objects[object_index].steering_flags |= 2;
            } else {
                stalls_objects[object_index].steering_flags |= 4;
            }
        }
        if (stalls_objects[object_index].steering_flags == 0 || stalls_objects[object_index].steering_flags == 1) {
            stalls_objects[object_index].steering_frames = rand() % 25 + 40;
            if (stalls_objects[object_index].speed_class >= 4) {
                stalls_objects[object_index].steering_frames /= 4;
            }
        } else if (stalls_objects[object_index].steering_flags & 1) {
            stalls_objects[object_index].steering_frames = 25;
        } else if (stalls_objects[object_index].steering_flags & 2) {
            stalls_objects[object_index].steering_frames = 50;
        } else if (stalls_objects[object_index].steering_flags & 4) {
            stalls_objects[object_index].steering_frames = 100;
        }
    }
}

/* FAKE: the record pointer stays an expression; as a local it is spilled to another stack slot than the original's (T-3330, T-9330) */
#define EN_OBJ(i) (&stalls_objects[i])
void stalls_object_special_ai_update(object_index)
s32 object_index;
{
    s32 d;

    if (((EN_OBJ(object_index)->steering_flags & 0x40) || (EN_OBJ(object_index)->steering_flags & 0x80)) && EN_OBJ(object_index)->steering_frames <= 0) {
        EN_OBJ(object_index)->speed_class = EN_OBJ(object_index)->speed_profile * 2 + 4;
        EN_OBJ(object_index)->steering_frames = rand() % 15 + 0x1E;
        if (!(rand() & 1)) {
            EN_OBJ(object_index)->steering_flags = 0;
        } else {
            EN_OBJ(object_index)->steering_flags = 1;
        }
    }
    d = stalls_cursor_distance_squared(EN_OBJ(object_index)->x.h[1], EN_OBJ(object_index)->y.h[1], cursor_x, cursor_y);
    switch (stalls_cursor_relative_direction_sector(object_index)) {
    case 0:
        stalls_object_steer_handler_00(object_index, d);
        break;
    case 1:
        stalls_object_steer_handler_01(object_index, d);
        break;
    case 2:
        stalls_object_steer_handler_02(object_index, d);
        break;
    case 3:
        stalls_object_steer_handler_03(object_index, d);
        break;
    }
    stalls_object_random_steering_update(object_index);
}

void stalls_object_steer_handler_00(s32 object_index, s32 distance_squared) {
    s32 steering_profile;
    EnObj *object;

    object = EN_OBJ(object_index);
    if (object->speed_class < 3) {
        steering_profile = object->steering_profile;
        switch (steering_profile) {
        case 1:
            if (distance_squared < 0x844) {
                object->speed_class = (s32) ((object->speed_profile * 2) + 4);
                object->steering_frames = (s32) ((rand() % 15) + 0x1E);
                if (!(rand() & 1)) {
                    object->steering_flags = 0;
                } else {
                    object->steering_flags = 1;
                }
                if (distance_squared >= 0x191) {
                    object->steering_frames = 8;
                    if (!(rand() & 1)) {
                        object->steering_flags = 0x80;
                    } else {
                        object->steering_flags = 0x40;
                    }
                }
            }
            break;
        case 2:
            if (distance_squared < 0xF04) {
                object->speed_class = (s32) ((object->speed_profile * 2) + 4);
                object->steering_frames = (s32) ((rand() % 15) + 0x1E);
                if (!(rand() & 1)) {
                    object->steering_flags = 0;
                } else {
                    object->steering_flags = 1;
                }
                if (distance_squared >= 0x191) {
                    object->steering_frames = 4;
                    if (!(rand() & 1)) {
                        object->steering_flags = 0x81;
                    } else {
                        object->steering_flags = 0x41;
                    }
                }
            }
            break;
        }
        if (object->heading >= 0x1000) {
            object->heading -= 0x1000;
        }
        if (object->heading < 0) {
            object->heading += 0x1000;
        }
    }
}

void stalls_object_steer_handler_01(s32 object_index, s32 distance_squared) {

    if (EN_OBJ(object_index)->speed_class < 3) {
        switch (EN_OBJ(object_index)->steering_profile) {
        case 1:
            if (distance_squared < 0x384) {
                EN_OBJ(object_index)->speed_class = EN_OBJ(object_index)->speed_profile * 2 + 4;
                EN_OBJ(object_index)->steering_flags = 0x80;
                if (!(rand() & 1)) {
                    EN_OBJ(object_index)->steering_frames = 1;
                } else {
                    EN_OBJ(object_index)->steering_frames = 2;
                }
            }
            break;
        case 2:
            if (distance_squared < 0x844) {
                EN_OBJ(object_index)->speed_class = EN_OBJ(object_index)->speed_profile * 2 + 4;
                EN_OBJ(object_index)->steering_flags = 0x81;
                if (!(rand() & 1)) {
                    EN_OBJ(object_index)->steering_frames = 1;
                } else {
                    EN_OBJ(object_index)->steering_frames = 2;
                }
            }
            break;
        }
        if (EN_OBJ(object_index)->heading >= 0x1000) {
            EN_OBJ(object_index)->heading -= 0x1000;
        }
        if (EN_OBJ(object_index)->heading < 0) {
            EN_OBJ(object_index)->heading += 0x1000;
        }
    }
}

void stalls_object_steer_handler_02(s32 object_index, s32 distance_squared) {

    if (EN_OBJ(object_index)->speed_class < 3) {
        switch (EN_OBJ(object_index)->steering_profile) {
        case 1:
            if (distance_squared < 0x384) {
                EN_OBJ(object_index)->speed_class = EN_OBJ(object_index)->speed_profile * 2 + 4;
                EN_OBJ(object_index)->steering_flags = 0x40;
                if (!(rand() & 1)) {
                    EN_OBJ(object_index)->steering_frames = 1;
                } else {
                    EN_OBJ(object_index)->steering_frames = 2;
                }
            }
            break;
        case 2:
            if (distance_squared < 0x844) {
                EN_OBJ(object_index)->speed_class = EN_OBJ(object_index)->speed_profile * 2 + 4;
                EN_OBJ(object_index)->steering_flags = 0x41;
                if (!(rand() & 1)) {
                    EN_OBJ(object_index)->steering_frames = 1;
                } else {
                    EN_OBJ(object_index)->steering_frames = 2;
                }
            }
            break;
        }
        if (EN_OBJ(object_index)->heading >= 0x1000) {
            EN_OBJ(object_index)->heading -= 0x1000;
        }
        if (EN_OBJ(object_index)->heading < 0) {
            EN_OBJ(object_index)->heading += 0x1000;
        }
    }
}

void stalls_object_steer_handler_03(s32 object_index, s32 distance_squared) {
    EnObj *p;

    p = &stalls_objects[object_index];
    if (p->speed_class < 3) {
        switch (p->steering_profile) {
        case 1:
            if (distance_squared < 0xC4) {
                if (!(rand() & 1)) {
                    p->steering_flags = 0;
                } else {
                    p->steering_flags = 1;
                }
                p->speed_class = p->speed_profile * 2 + 4;
                p->steering_frames = rand() % 15 + 0x1E;
            }
            break;
        case 2:
            if (distance_squared < 0x384) {
                if (!(rand() & 1)) {
                    p->steering_flags = 0;
                } else {
                    p->steering_flags = 1;
                }
                p->speed_class = p->speed_profile * 2 + 4;
                p->steering_frames = rand() % 15 + 0x1E;
            }
            break;
        }
    }
}

/* The quadrant (0..3) of display record 0 as seen from object arg0, turned by the object's
 * direction stalls_object_direction_sector_select(arg0) (0..7). dir is unset for other values, so IDO keeps it in memory
 * (sp+0x2C); dx, dy, dir declared in this order give the original's slots. T-20920 */
s32 stalls_cursor_relative_direction_sector(s32 object_index) {
    s32 dx;
    s32 dy;
    s32 dir;

    dx = cursor_x - stalls_objects[object_index].x.h[1];
    dy = cursor_y - stalls_objects[object_index].y.h[1];
    switch (stalls_object_direction_sector_select(object_index)) {
    case 0:
        if (dy < dx) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (dy <= -dx) {
            dir += 2;
        }
        break;
    case 1:
        if (stalls_objects[object_index].x.h[1] < cursor_x) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (cursor_y <= stalls_objects[object_index].y.h[1]) {
            dir += 2;
        }
        break;
    case 2:
        if (-dx < dy) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (dy <= dx) {
            dir += 2;
        }
        break;
    case 3:
        if (stalls_objects[object_index].y.h[1] < cursor_y) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (stalls_objects[object_index].x.h[1] <= cursor_x) {
            dir += 2;
        }
        break;
    case 4:
        if (dx < dy) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (-dx <= dy) {
            dir += 2;
        }
        break;
    case 5:
        if (cursor_x < stalls_objects[object_index].x.h[1]) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (stalls_objects[object_index].y.h[1] <= cursor_y) {
            dir += 2;
        }
        break;
    case 6:
        if (dy < -dx) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (dx <= dy) {
            dir += 2;
        }
        break;
    case 7:
        if (cursor_y < stalls_objects[object_index].y.h[1]) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (cursor_x <= stalls_objects[object_index].x.h[1]) {
            dir += 2;
        }
        break;
    }
    return dir;
}

void stalls_particle_grid_reset(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 12; i++) {
        for (j = 0; j < 15; j++) {
            D_801392AC[j][i].x.h[1] = 0;
            D_801392AC[j][i].y.h[1] = 0;
        }
    }
    D_801392A4 = 0;
    D_801392A8 = 0;
}

void stalls_particle_field_update(void) {
    D_80139220 += 1;
    stalls_particle_grid_position_update();
    D_80139224 += 3;
    if (D_80139224 >= 0x1000) {
        D_80139224 = 0;
    }
    D_801392A4 = (rsin(D_80139224) * 8) >> 12;
    D_801392A8 = (func_800A0140(D_80139224) * 0x10) >> 12;
    stalls_active_wave_update();
    stalls_wave_grid_draw();
}

void stalls_particle_grid_position_update(void) {
    s32 x;
    s32 y;
    s32 b;
    EnWords64 table;

    table = D_80139B28;
    b = (D_80139220 / 7) % 8;
    for (x = 0; x < 12; x++) {
        for (y = 0; y < 15; y++) {
            D_801392AC[y][x].x.h[1] = table.value[(x + y * 3) % 8][(D_80139220 / 5) % 8];
            D_801392AC[y][x].y.h[1] = table.value[(x * 3 + y) % 8][b];
        }
    }
}


void stalls_active_wave_update(void) {
    s32 i;

    for (i = 0; i != 10; i++) {
        if (stalls_wave_ages[i] != 0) {
            stalls_wave_ages[i] += 1;
            stalls_wave_grid_displacement(i);
            if (stalls_wave_ages[i] >= 0x51) {
                stalls_wave_ages[i] = 0;
            }
        }
    }
}

/* Implicit int in the original (no return value): as1 then leaves the nop after the second test. T-18200 */
s32 stalls_wave_start(s32 center_x, s32 center_y) {
    if (stalls_wave_ages[stalls_wave_ring_index] <= 0 || stalls_wave_ages[stalls_wave_ring_index] >= 10) {
        stalls_wave_ring_index++;
        if (stalls_wave_ring_index >= 10) {
            stalls_wave_ring_index = 0;
        }
        D_80139228[stalls_wave_ring_index].x.h[1] = center_x + 0xA0;
        D_80139228[stalls_wave_ring_index].y.h[1] = center_y + 0x78;
        stalls_wave_ages[stalls_wave_ring_index] = 1;
    }
}

/* Pushes the grid points of ring arg0 (radius 5 * age) outwards or inwards in four bands. x and y are
 * 16.16 locals of which only the integer half is set; r0..r7 are the band bounds, and with x and y in
 * memory each declared local takes a stack slot (T-3330), which gives the original frame. T-20920 */
void stalls_wave_grid_displacement(s32 wave_index) {
    s32 i;
    s32 j;
    s32 d;
    s32 r0;
    s32 r1;
    s32 r2;
    s32 r3;
    s32 r4;
    s32 r5;
    s32 r6;
    s32 r7;
    Fix32 x;
    Fix32 y;
    s32 px;
    s32 py;

    for (j = 0, py = -25; j < 12; j++, py += 25) {
        for (i = 0, px = -25; i < 15; i++, px += 25) {
            x.h[1] = px;
            y.h[1] = py;
            d = stalls_cursor_distance_squared(D_80139228[wave_index].x.h[1], D_80139228[wave_index].y.h[1], x.h[1], y.h[1]);
            r0 = stalls_wave_ages[wave_index] * 5;
            r1 = r0 + 5;
            if (r0 * r0 <= d) {
                if (d <= r1 * r1) {
                    D_801392AC[i][j].x.w += (x.w - D_80139228[wave_index].x.w) * 2 / r0;
                    D_801392AC[i][j].y.w += (y.w - D_80139228[wave_index].y.w) * 2 / r0;
                } else {
                    r2 = r0 + 10;
                    r3 = r0 + 15;
                    if (r2 * r2 < d) {
                        if (d <= r3 * r3) {
                            D_801392AC[i][j].x.w -= (x.w - D_80139228[wave_index].x.w) * 3 / r2;
                            D_801392AC[i][j].y.w -= (y.w - D_80139228[wave_index].y.w) * 3 / r2;
                        } else {
                            r4 = r0 + 20;
                            r5 = r0 + 25;
                            if (r4 * r4 < d) {
                                if (d <= r5 * r5) {
                                    D_801392AC[i][j].x.w += (x.w - D_80139228[wave_index].x.w) * 3 / r4;
                                    D_801392AC[i][j].y.w += (y.w - D_80139228[wave_index].y.w) * 3 / r4;
                                } else {
                                    r6 = r0 + 30;
                                    r7 = r0 + 35;
                                    if (r6 * r6 < d && d <= r7 * r7) {
                                        D_801392AC[i][j].x.w -= (x.w - D_80139228[wave_index].x.w) * 2 / r6;
                                        D_801392AC[i][j].y.w -= (y.w - D_80139228[wave_index].y.w) * 2 / r6;
                                    }
                                }
                            }; /* FAKE: empty statement (found with the permuter) gives the original's register order, j in s2 before the grid base and the constant 3; T-20920 */
                        }
                    }
                }
            }
        }
    }
}

void stalls_wave_grid_draw(void) {
    s32 i;
    s32 j;

    for (j = 0; j < 11; j++) {
        for (i = 0; i < 14; i++) {
            stalls_wave_grid_cell_draw(i, j);
        }
    }
}

void stalls_wave_grid_cell_draw(s32 arg0, s32 arg1) {
    POLY_FT4 *poly;
    s32 p[4][2];
    s32 u0;
    s32 u1;
    s32 u2;
    s32 u3;
    s32 v0;
    s32 v1;
    s32 v2;
    s32 v3;
    s32 ofs;

    p[0][0] = D_801392AC[arg0][arg1].x.h[1] + arg0 * 25 - 25;
    u1 = arg0; /* FAKE: an extra use of u1 (found with the permuter) gives the original s-register order; T-20920 */
    p[1][0] = D_801392AC[arg0 + 1][arg1].x.h[1] + arg0 * 25 - 1;
    p[2][0] = D_801392AC[u1][arg1 + 1].x.h[1] + arg0 * 25 - 25;
    p[3][0] = D_801392AC[arg0 + 1][arg1 + 1].x.h[1] + arg0 * 25 - 1;
    p[0][1] = D_801392AC[arg0][arg1].y.h[1] + arg1 * 25 - 25;
    p[1][1] = D_801392AC[arg0 + 1][arg1].y.h[1] + arg1 * 25 - 25;
    p[2][1] = D_801392AC[arg0][arg1 + 1].y.h[1] + arg1 * 25 - 1;
    p[3][1] = D_801392AC[arg0 + 1][arg1 + 1].y.h[1] + arg1 * 25 - 1;
    u2 = u0 = arg0 * 25 + D_801392A4 + 0x27;
    u3 = u1 = arg0 * 25 + D_801392A4 + 0x3F;
    v1 = v0 = arg1 * 25 + D_801392A8 + 0x27;
    v3 = v2 = arg1 * 25 + D_801392A8 + 0x3F;
    while (u0 > 0xC0) {
        u0 -= 0x80;
        u1 -= 0x80;
        u2 -= 0x80;
        u3 -= 0x80;
    }
    while (v0 > 0xC0) {
        v0 -= 0x80;
        v1 -= 0x80;
        v2 -= 0x80;
        v3 -= 0x80;
    }
    poly = GetWorkBase(0x28, draw_buffer);
    func_8009F040(poly);
    SetShadeTex(poly, 0);
    SetSemiTrans(poly, 1);
    poly->r0 = D_8013921C;
    poly->g0 = D_8013921C;
    poly->b0 = D_8013921C;
    poly->clut = func_8009ED74(0, 0x1FF);
    poly->tpage = func_8009ECB0(1, 4, 0x1C0, 0);
    ofs = -draw_buffer * 0xF0 + 0xF0;
    poly->x0 = p[0][0];
    poly->y0 = p[0][1] + ofs;
    poly->x1 = p[1][0] + 1;
    poly->y1 = p[1][1] + ofs;
    poly->x2 = p[2][0];
    poly->y2 = p[2][1] + ofs + 1;
    poly->x3 = p[3][0] + 1;
    poly->y3 = p[3][1] + ofs + 1;
    /* FAKE: single-line grouping makes as1 schedule the 4 UV +1 byte stores in target order (T-24107) */
    poly->u1 = u1 + 1; poly->v2 = v2 + 1; poly->u3 = u3 + 1; poly->v3 = v3 + 1;
    poly->u2 = u2;
    poly->v1 = v1;
    poly->v0 = v0;
    poly->u0 = u0;
    AddPrim(draw_buffer * 0x400 + 0x14 + draw_ot_tags, poly);
}

s32 stalls_result_summary_dispatch(void) {
    s32 pad; /* FAKE: unused word above x/y preserves frame size 0x38 and spill offsets 0x2C/0x30 (T-24107) */
    s32 x;
    s32 y;

    stalls_wave_response_update();
    stalls_particle_field_update();
    stalls_light_animation_update();
    stalls_object_scene_update();
    switch (game_state.mode_step) {
    case 0:
        stalls_score_panel_fade_draw();
        break;
    case 1:
        if (stalls_regular_object_count == 0 && stalls_kind4_object_count == 0) {
            audio_command(0x204);
        } else {
            audio_command(0x202);
        }
        if (stalls_regular_object_count >= 100) {
            stalls_regular_object_count = 99;
        }
        if (stalls_kind4_object_count >= 100) {
            stalls_kind4_object_count = 99;
        }
        mode_step_next();
        break;
    case 2:
        game_state.mode_step_timer.u++;
        if (game_state.mode_step_timer.u % 15 == 0) {
            x = game_state.substep;
            y = x;
            if (stalls_regular_object_count < y) {
                y = x; /* FAKE: kills uopt copy-propagation of $v0 across the branch so as1 folds $v0 into $a2 (T-24107) */
                x = stalls_regular_object_count;
            }
            if (stalls_kind4_object_count < y) {
                y = stalls_kind4_object_count;
            }
            if (x == stalls_regular_object_count && y == stalls_kind4_object_count) {
                audio_command(0x505);
                stalls_counter_digit_set(stalls_regular_object_count, stalls_kind4_object_count);
                mode_step_next();
            }
            stalls_counter_digit_set(x, y);
            if (game_state.substep != 0) {
                audio_command(0x505);
            }
            game_state.substep++;
        }
        if (game_state.pad_pressed & PAD_CROSS) {
            stalls_counter_digit_set(stalls_regular_object_count, stalls_kind4_object_count);
            if (stalls_regular_object_count == 0 && stalls_kind4_object_count == 0) {
                mode_step_next();
            } else {
                mode_step_next();
                mode_step_next();
                mode_step_next();
            }
        }
        break;
    case 3:
        if (stalls_regular_object_count == 0 && stalls_kind4_object_count == 0) {
            switch (game_state.substep) {
            case 0:
                if (cd_audio_state() == 2) {
                    game_state.substep++;
                }
                break;
            case 1:
                cd_audio_request_set(0x01000007, 0x03000015, 0xE);
                audio_command(0x300);
                game_state.substep++;
                break;
            case 2:
                if (cd_audio_state() == 4) {
                    game_state.substep++;
                }
                break;
            case 3:
                if (cd_audio_state() == 2) {
                    audio_command(0x505);
                    stalls_counter_digit_set(1, 0);
                    mode_step_next();
                }
                break;
            }
            game_state.mode_step_timer.u++;
            if (game_state.mode_step_timer.u >= 361) {
                audio_command(0x7F);
                mode_step_next();
                mode_step_next();
            }
            if (game_state.pad_pressed & PAD_START) {
                audio_command(0x7F);
                mode_step_next();
                mode_step_next();
            }
        } else {
            mode_step_next();
        }
        break;
    case 4:
        game_state.mode_step_timer.u++;
        if (game_state.pad_pressed & 0x60) {
            audio_command(0x7F);
            mode_step_next();
        }
        break;
    case 5:
        stalls_brightness_ramp_down();
        if (D_8013921C == 0) {
            mode_stage_next();
        }
        break;
    }
}

void stalls_score_panel_fade_draw(void) {
    s32 pad; /* FAKE: unused word above poly, puts poly at sp+0x30 as in the original (T-3330 slot rule) */
    POLY_F4 *poly;

    poly = GetWorkBase(0x18, draw_buffer);
    func_8009F02C(poly);
    SetSemiTrans(poly, 1);
    poly->r0 = D_80139B20;
    poly->g0 = D_80139B20;
    poly->b0 = D_80139B20;
    poly->x0 = game_state.draw_origin_x[draw_buffer] - 0x58;
    poly->y0 = game_state.draw_origin_y[draw_buffer] - 0x40;
    poly->x1 = game_state.draw_origin_x[draw_buffer] + 0x58;
    poly->y1 = game_state.draw_origin_y[draw_buffer] - 0x40;
    poly->x2 = game_state.draw_origin_x[draw_buffer] - 0x58;
    poly->y2 = game_state.draw_origin_y[draw_buffer] + 0x30;
    poly->x3 = game_state.draw_origin_x[draw_buffer] + 0x58;
    poly->y3 = game_state.draw_origin_y[draw_buffer] + 0x30;
    AddPrim(draw_buffer * 0x400 + 8 + draw_ot_tags, poly);
    dtd_on_tpage(0, 0, 2, 2, 0);
    if (D_80139B20 == 0) {
        D_801215FF = 0x80;
    }
    D_80139B20 += 2;
    D_80121603 = D_80139B20;
    if (D_80139B20 >= 0x80) {
        *(u8 *) &D_80121603 = 0x80; /* FAKE: unsigned view keeps the constant 0x80 (an s8 store would load -128), T-9330 */
    }
    if (D_80139B20 == 0xA0) {
        D_80121634 = 0x01000000;
        D_8012163F = 0x11;
        mode_step_next();
    }
}

void stalls_counter_digit_set(s32 arg0, s32 arg1) {
    D_80121313 = 0x84;
    D_80121357 = 0x84;
    D_8012139B = 0x84;
    D_801213DF = 0x84;
    D_80121328 = (arg0 % 10) + 0x23;
    D_8012136C = (arg0 / 10) + 0x23;
    if (arg0 < 0xA) {
        D_80121357 = 0;
    }
    D_801213B0 = (arg1 % 10) + 0x23;
    D_801213F4 = (arg1 / 10) + 0x23;
    if (arg1 < 0xA) {
        D_801213DF = 0;
    }
}

void stalls_audio_command_debounce(s32 audio_command_id) {
    if (stalls_audio_command_cooldown <= 0) {
        stalls_audio_command_cooldown = 1;
        audio_command(audio_command_id & 0xFFFF);
    }
}

/* A plain index loop: IDO unrolls it by two into the original's pointer walk (T-10850 had written the
 * unrolled form by hand, with a FAKE). T-20920 */
void stalls_wave_response_update(void) {
    s32 i;

    if (pad_code_entered != 0) {
        pad_code_index = 0;
        pad_code_entered = 0;
        stalls_pad_code_stage++;
    }
    if (stalls_pad_code_stage > 0) {
        for (i = 0; i < 10; i++) {
            if (stalls_objects[i].kind_flags > 0 && stalls_objects[i].kind_flags < 4) {
                if (stalls_pad_code_stage < 3) {
                    stalls_objects[i].kind_flags = stalls_objects[i].steering_profile + 1;
                } else {
                    stalls_objects[i].kind_flags = 4;
                    stalls_objects[i].steering_profile = 0;
                }
            }
        }
    }
}
