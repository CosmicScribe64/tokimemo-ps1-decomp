/* yuina_scene_and_actor_effects.c: src/ovl/SHUGAKU/80135430.c:4-34,73-100; Yuina scene resources, callback table and actor fade/effect updates. 801378B0.c:391-394 selects mode_stage8 for Yuina; scene init sets current_girl2. Original object 0x80135430-0x80136380 (config/objects/SHUGAKU.txt). */
#include "common.h"
#include "ovl/SHUGAKU.h"

void trip_yuina_scene_resource_pointer_setup(void) {
    D_8013C2D0 = 0x801C25B8;
    D_8013C2D4 = 0x801C25C8;
    D_8013C2D8 = 0x801C2674;
    D_8013C2DC = *(s16 *)0x801C26A0;
    D_8013C2E0 = 0x801B0000;
}

/* s32 with no return (v0 live at the exit): the original keeps the dead `li v0,-129` of the last `&= ~0x80`. T-22014 */
s32 trip_yuina_callback_dispatch_and_actor_fade(void) {
    u8 index;
    ShFnTbl74 table;

    table = D_8013C2F0;
    index = game_state.mode_step;
    table.f[index](0x80);
    index = game_state.mode_step;
    if (scene_still_fade_in == table.f[index]) {
        ((Actor *)actor_pool)[98].draw_flags |= 0x80;
    }
    if ((scene_still_fade_out == table.f[index] || trip_yuina_actor_brightness_sync == table.f[index]) && bg_brightness < 8) {
        ((Actor *)actor_pool)[98].draw_flags &= ~0x80;
        ((Actor *)actor_pool)[99].draw_flags &= ~0x80;
    }
    if ((index >= 0x16 && index < 0x1C) || (index >= 0x20 && index < 0x25)) {
        ((Actor *)actor_pool)[98].brightness = ((Actor *)actor_pool)[96].brightness;
    }
    if (D_8013C2E4 != 0) {
        trip_yuina_actor_effect_and_projectile_update();
    }
}

void trip_yuina_auxiliary_scene_branch(void) {
    if (D_8013C2EC != 0) {
        trip_sightseeing_hotel_lobby_background_load();
        return;
    }
    trip_yuina_background_load();
}

void trip_yuina_mode_step_skip(void) {
    mode_step_set(0x1F);
}

void trip_yuina_scene_ui_and_state_initialize(void) {
    s32 pad; /* FAKE: unused scalar puts the RECT at +0x2C, as in GEKO func_8013B3C0 (T-9770); original local unknown. T-18000 */
    RECT r;

    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    hud_cursor_show(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    talk_clear();
    k_reset(1);
    r.x = 0x140;
    r.y = 0x1A0;
    r.w = 0x140;
    r.h = 0x58;
    func_8009C7F8(&r, 8, 0, 0);
    r.x = 0x2C0;
    r.y = 0xA0;
    func_8009C7F8(&r, 8, 0, 0);
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    /* FAKE: byte store then member read shares the constant in a0, as in GEKO func_8013B3C0 (T-9770). T-18000 */
    *(u8 *)&game_state.current_girl = 2;
    talk_names_set(game_state.current_girl);
    addr_init_bustup();
    game_state.screen_brightness = 0x80;
    bg_brightness = 0;
    hizuke_init();
    message_window_init();
    text_marker_hide();
    D_8013C2E4 = 0;
    D_8013C2E8 = 0;
    D_8013C2EC = D_80122CE0;
    D_80122CE0 = 0xFF;
    trip_scene_script_resource_pointer_setup();
    D_8013BFFC = D_8013C508;
    D_8013C000 = D_8013C644;
    D_8013C004 = D_8013C780;
    mode_step_next();
}

void trip_yuina_scene_picture_and_actor_setup(void) {
    s16 i;

    trip_yuina_scene_resource_pointer_setup();
    load_palette(D_8013C2E0, 0x11, 1, 2, 0);
    for (i = 0; i < 6; i++) {
        sprite_quads[58 + i].w = 0;
    }
    scene_anim_show(D_8013C2D4, D_8013C2D8, D_8013C2D0, (s32) D_8013C2DC);
    ((Actor *)actor_pool)[97].unk_04 = 8;
    ((Actor *)actor_pool)[96].unk_04 = 8;
    ((Actor *)actor_pool)[97].unk_01 = 0xB;
    ((Actor *)actor_pool)[96].unk_01 = 0xB;
    ((Actor *)actor_pool)[98] = ((Actor *)actor_pool)[96];
    ((Actor *)actor_pool)[98].flags = 0x40;
    ((Actor *)actor_pool)[98].unk_18 = 0x14;
    make_color_bar16(game_state.scratch_words, 0xFFFFFF, 0xFFFFFF, 0x10, 0);
    load_palette((s32) game_state.scratch_words, 0x1F, 1, 3, 0);
    game_state.tpage_buf[0x25] = 1;
    cd_iris_texture_build(0x801C4000, 0x25, 0x80);
    mode_step_next();
}

void trip_yuina_audio_transition(void) {
    audio_command(0x203);
    mode_step_next();
}

void trip_yuina_mode_stage_transition(void) {
    if (D_8013C2EC == 0) {
        audio_command(0x202);
    }
    mode_stage_set(5);
}

void trip_yuina_rpg_battle_launch(void) {
    D_80122CE0 = 7;
    actor_clear_range(0);
    hud_cursor_show(0);
    game_state.saved_game_mode = game_state.game_mode;
    game_state.saved_mode_stage = game_state.mode_stage;
    game_state.saved_mode_step = 0x38;
    k_reset(0);
    game_mode_set(GAME_MODE_RPG_BATTLE);
}

void trip_yuina_scene_sector_load(void) {
    cd_read_sectors(0x25, 0x801B0000, 0x8240);
    trip_yuina_scene_resource_pointer_setup();
    mode_step_next();
}

void trip_yuina_background_brightness_sync(void) {
    scene_still_fade_out();
    D_80120657 = bg_brightness;
    D_8012069B = bg_brightness;
    D_801206DF = bg_brightness;
    D_80120723 = bg_brightness;
}

s32 trip_yuina_actor_brightness_sync(void) {
    if (D_8013C2E8 < 0xA0 || (u8)D_800B5BD4 != 9) {
        return 0;
    }
    scene_still_fade_out();
    D_80120657 = bg_brightness;
    D_8012069B = bg_brightness;
    D_801206DF = bg_brightness;
    D_80120723 = bg_brightness;
}

/* One symbol for the whole 0x44-byte-entry table: as1 then keeps the later lbu below the earlier stores (T-2040, T-4020). */
void trip_yuina_actor_pose_set_a(void) {
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->sequence_slot = 8;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].sequence_slot = 7;
    ((Actor *)actor_pool_96)[2].flags = 0x40;
    ((Actor *)actor_pool_96)[2].unk_18 = 0x14;
    mode_step_next();
}

u32 trip_yuina_actor_pose_clear(void) {
    ((Actor *)actor_pool_96)->draw_flags &= ~0x80;
    ((Actor *)actor_pool_96)[1].draw_flags &= ~0x80;
    ((Actor *)actor_pool_96)[2].flags = 5;
    ((Actor *)actor_pool_96)[2].sequence_slot = 4;
    ((Actor *)actor_pool_96)[2].unk_18 = 0;
    return ~0x80;
}

void trip_yuina_actor_pose_set_b(void) {
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)->sequence_slot = 3;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].sequence_slot = 1;
    ((Actor *)actor_pool_96)[2].flags = 0x40;
    ((Actor *)actor_pool_96)[2].unk_18 = 0x16;
}

void trip_yuina_scene_display_selector_setup(void) {
    D_80120666 = 2;
    D_801206AA = 0;
    D_801206DA = 0x40;
    D_801206F0 = 0x14;
    mode_step_next();
}

void trip_yuina_scene_counter_reset(void) {
    D_8013C2E4 = 0;
    D_800B5BD4 = 0xF;
    mode_step_next();
}

void trip_yuina_background_event_log(void) {
    audio_command(0x203);
    bg_read_sub2(0x4761);
    event_log_add(2, 0x13);
    mode_step_next();
}

void trip_yuina_background_load(void) {
    audio_command(0x203);
    bg_read_sub2(0x4761);
    mode_step_next();
}

void trip_yuina_actor_auxiliary_setup(void) {
    D_8013C2E4 = 1;
    *(Actor *) &actor_pool_96[0xCC] = *(Actor *) actor_pool_96;
    ((Actor *)actor_pool_96)[3].unk_01 = 0xB;
    ((Actor *)actor_pool_96)[3].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[3].sequence_slot = 5;
    ((Actor *)actor_pool_96)[3].flags = 9;
    ((Actor *)actor_pool_96)[3].x = -0x60;
    ((Actor *)actor_pool_96)[3].y = -0x48;
    mode_step_next();
}

void trip_scene_parameter_update(void) {
    if (D_8013C2EC == 0) {
        game_state.girls[(s32) game_state.current_girl].tokimeki += 1;
        game_state.girls[(s32) game_state.current_girl].affection += 2;
        check_para_limit();
    } else {
        game_state.girls[(s32) game_state.current_girl].affection += 1;
        check_para_limit();
        D_800CA2E4 += 2;
        game_state.mode_step += 0xB;
    }
    mode_step_next();
}

void trip_yuina_scene_entry_and_picture_setup(void) {
    s16 i;

    game_state.current_girl = GIRL_YUINA;
    D_8013C2E4 = 0;
    D_8013C2E8 = 0;
    D_800CA2F4 = 1;
    trip_scene_script_resource_pointer_setup();
    D_800CA2E4 = 0;
    D_800CA2E8 = 0;
    D_8013BFFC = D_8013C508;
    D_8013C000 = D_8013C644;
    D_8013C004 = D_8013C780;
    trip_yuina_scene_resource_pointer_setup();
    load_palette(D_8013C2E0, 0x11, 1, 2, 0);
    D_80122D04 = 0;
    D_80122D0C = 0;
    D_80122D10 = 0;
    for (i = 0; i < 6; i++) {
        sprite_quads[58 + i].w = 0;
    }
    scene_anim_show(D_8013C2D4, D_8013C2D8, D_8013C2D0, (s32) D_8013C2DC);
    ((Actor *)actor_pool)[97].unk_04 = 8;
    ((Actor *)actor_pool)[96].unk_04 = 8;
    ((Actor *)actor_pool)[97].unk_01 = 0xB;
    ((Actor *)actor_pool)[96].unk_01 = 0xB;
    ((Actor *)actor_pool)[98] = ((Actor *)actor_pool)[96];
    ((Actor *)actor_pool)[98].flags = 0x40;
    ((Actor *)actor_pool)[98].unk_18 = 0x14;
    make_color_bar16(game_state.scratch_words, 0xFFFFFF, 0xFFFFFF, 0x10, 0);
    load_palette((s32) game_state.scratch_words, 0x1F, 1, 3, 0);
    game_state.tpage_buf[0x25] = 1;
    cd_iris_texture_build(0x801C4000, 0x25, 0x80);
    mode_step_next();
}

/* Record 99 of the actor_pool display table: unk_24..unk_30 are two 16.16 position pairs (the s32 views below).
 * FAKE: D_8013C2E8 is read and written through an address-taken view, so uopt does not promote the counter into a
 * register; the original loads and stores it at every use. cnt keeps one load for the colour and the square. T-22014 */
s32 trip_yuina_actor_effect_and_projectile_update(void) {
    s16 cnt;
    u32 var_v1;

    if ((D_80120732 == 5) && !(((Actor *)actor_pool)[99].draw_flags & 0x80)) {
        *(s16 *)&D_8013C2E8 += 1;
        if (*(s16 *)&D_8013C2E8 >= 0x29) {
            audio_command(0x500);
            *(s16 *)&D_8013C2E8 = 0;
            D_80120732 = 6;
            D_80120734 = 0;
            ((Actor *)actor_pool)[99].unk_08 = 0;
            ((Actor *)actor_pool)[99].draw_flags |= 0x80;
            D_8012071E = 1;
            *(s32 *)&((Actor *)actor_pool)[99].dx_fraction = 0x64000;
            *(s32 *)&((Actor *)actor_pool)[99].dy_fraction = 0x12000;
        }
    }
    if ((((Actor *)actor_pool)[99].draw_flags & 0x80) && (D_80120732 == 6)) {
        *(s32 *)&((Actor *)actor_pool)[99].dx_fraction -= 0x1800;
        *(s32 *)&((Actor *)actor_pool)[99].x_fraction +=
            *(s32 *)&((Actor *)actor_pool)[99].dx_fraction;
        *(s32 *)&((Actor *)actor_pool)[99].dy_fraction -= 0x600;
        *(s32 *)&((Actor *)actor_pool)[99].y_fraction +=
            *(s32 *)&((Actor *)actor_pool)[99].dy_fraction;
        if (((Actor *)actor_pool)[99].x < -0xA0) {
            ((Actor *)actor_pool)[99].draw_flags &= 0xFF7F;
        }
    }
    if (((Actor *)actor_pool)[99].x < -0xA0) {
        D_800B5BD4 = 9;
        if (*(s16 *)&D_8013C2E8 == 0x64) {
            trip_yuina_actor_pose_clear();
        }
        if (*(s16 *)&D_8013C2E8 == 0xA) {
            audio_command(0x502);
        }
        if ((D_801206F0 == 1) && (D_801206E0 == 0x21)) {
            trip_yuina_actor_pose_set_b();
        }
        *(s16 *)&D_8013C2E8 += 1;
        if (*(s16 *)&D_8013C2E8 >= 0xA6) {
            *(s16 *)&D_8013C2E8 = 0xA5;
        }
        cnt = *(s16 *)&D_8013C2E8;
        var_v1 = cnt + 0x80;
        if (var_v1 >= 0x100U) {
            var_v1 = 0xFF;
        }
        if (var_v1 == 0xFF) {
            if (bg_brightness == 0) {
                var_v1 = 0;
            } else {
                var_v1 = (bg_brightness * 2) - 1;
            }
        }
        cd_iris_wipe_draw(0x25, 0, 0x1FF, (s32) (s16) ((u32) (cnt * cnt) >> 1), -0xA0, -0x78,
                      (var_v1 << 0x10) | (var_v1 << 8) | var_v1 | 0x10000000, 0xA);
        dtd_on_tpage(0, 0, 0xA, 0, 0);
    }
}
