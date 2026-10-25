/* yumi_scenes.c: src/ovl/GEKO/8013F360.c:47,93,108,152,354; Yumi selectors1/2/7, script/score callbacks and shared color-quad effects. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x8013F360-0x80140A00 (config/objects/GEKO.txt). */
#define MAIN_API_OVERRIDE_talk_work /* switched as u32 (main_api.h: s32), T-6030 */
#include "common.h"
#include "ovl/GEKO.h"
#include "psx_types.h"

void encounter_color_quad_draw(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u32 arg4, s32 arg5, s32 arg6);

extern u32 talk_work;

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 encounter_yumi_scene_two_steps;

typedef struct {
    void (*callbacks[61])(); /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; src/ovl/GEKO/8013F360.c:17; func_8013F850 at line152 copies this exact61-callback table and dispatches by mode_step; scene number is the actual talk_work selector, not a narrative identity. */
} EncounterYumiSceneSevenSteps; /* size 0xF4 */

void encounter_yumi_assets_bind_1(void) {
    encounter_yumi_pack_1_animation_input_2 = 0x801D21BC;
    encounter_yumi_pack_1_animation_input_0 = 0x801D21C4;
    encounter_yumi_pack_1_animation_input_1 = 0x801D2204;
    encounter_yumi_pack_1_animation_input_3 = (*(s16 *)0x801D221C);
    encounter_yumi_pack_1_palette = 0x801B0000;
    encounter_yumi_pack_1_still_image_0 = 0x801B2000;
    encounter_yumi_pack_1_still_image_1 = 0x801B6000;
    encounter_yumi_pack_1_still_image_2 = 0x801BA000;
    encounter_yumi_pack_1_still_image_3 = 0x801BE000;
    encounter_yumi_pack_1_still_image_4 = 0x801C2000;
    encounter_yumi_pack_1_still_image_5 = 0x801C6000;
}

void encounter_yumi_assets_bind_2(void) {
    encounter_yumi_pack_2_animation_input_2 = 0x801D2600;
    encounter_yumi_pack_2_animation_input_0 = 0x801D2608;
    encounter_yumi_pack_2_animation_input_1 = 0x801D2674;
    encounter_yumi_pack_2_animation_input_3 = (*(s16 *)0x801D268C);
    encounter_yumi_pack_2_palette = 0x801B0000;
    encounter_yumi_pack_2_still_image_0 = 0x801B2000;
    encounter_yumi_pack_2_still_image_1 = 0x801B6000;
    encounter_yumi_pack_2_still_image_2 = 0x801BA000;
    encounter_yumi_pack_2_still_image_3 = 0x801BE000;
    encounter_yumi_pack_2_still_image_4 = 0x801C2000;
    encounter_yumi_pack_2_still_image_5 = 0x801C6000;
}

void encounter_yumi_scene_select_1(void) {
    switch (talk_work) {
    case 1:
        encounter_yumi_scene_one_dispatch();
        return;
    case 2:
        encounter_yumi_scene_two_dispatch();
        return;
    case 7:
        encounter_yumi_scene_seven_dispatch();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void encounter_yumi_scene_one_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_yumi_scene_one_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_yumi_scene_two_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_yumi_scene_two_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

void encounter_yumi_scene_seven_dispatch(void) {
    if (game_state.mode_stage == 0) {
        encounter_yumi_scene_seven_step_dispatch();
        return;
    }
    game_mode_reset_to_title();
}

typedef struct {
    void (*callbacks[49])(); /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; src/ovl/GEKO/8013F360.c:90; func_8013F5F0 at line93 copies this exact49-callback table and dispatches by mode_step; scene number is the actual talk_work selector, not a narrative identity. */
} EncounterYumiSceneOneSteps; /* size 0xC4 */
extern EncounterYumiSceneOneSteps encounter_yumi_scene_one_steps;

void encounter_yumi_scene_one_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    EncounterYumiSceneOneSteps mode_step_callbacks;

    mode_step_callbacks = encounter_yumi_scene_one_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.callbacks[mode_step_index](0x80);
}

void encounter_yumi_assets_load_1(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x8DBD);
    encounter_yumi_assets_bind_1();
    mode_step_next();
}

void encounter_yumi_scene_two_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    FnTbl23 mode_step_callbacks;

    mode_step_callbacks = encounter_yumi_scene_two_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.f[mode_step_index](0x80);
}

void encounter_yumi_scene_repeat_group_offset(void) {
    if ((u8) game_state.scene_flags[67] >= 2U) {
        D_800CA150 = (u16) D_800CA150 + 5;
    }
    mode_step_next();
}

void encounter_yumi_scene_repeat_skip(void) {
    portrait_value_clear_if_high(get_g_zyotai_h(GIRL_YUMI));
    if ((u8) game_state.scene_flags[67] >= 2U) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
        mode_step_next();
        mode_step_next();
        mode_step_next();
        mode_step_next();
        mode_step_next();
        mode_step_next();
    }
    mode_step_next();
}

void encounter_yumi_girl_state_group_offset(void) {
    u8 girl_state;

    girl_state = get_g_zyotai_s(game_state.current_girl) & 0x7F;
    if (girl_state >= 2U) {
        D_800CA150 = (u16)D_800CA150 + 2;
    }
    mode_step_next();
}

extern EncounterYumiSceneSevenSteps encounter_yumi_scene_seven_steps;

void encounter_yumi_scene_seven_step_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    EncounterYumiSceneSevenSteps mode_step_callbacks;

    mode_step_callbacks = encounter_yumi_scene_seven_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.callbacks[mode_step_index](0x80);
    if ((encounter_yumi_white_flash_enabled != 0) && (game_state.mode_step >= 0x20U)) {
        encounter_yumi_white_flash_draw();
    }
    if ((encounter_yumi_closing_actor_timer != 0) && (game_state.mode_step >= 0x20U)) {
        encounter_yumi_closing_actors_update();
    }
}

void encounter_yumi_closing_bg_request(void) {
    s32 background_read_result;

    cd_read_checked = 0;
    background_read_result = dec_bg_cd_read(0x3FCE, 0);
    if (background_read_result == *dec_bg_show) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
    } else if (background_read_result == (1 - *dec_bg_show)) {
        mode_step_next();
        mode_step_next();
    }
    mode_step_next();
}

s32 encounter_yumi_closing_bg_decode(void) {
    if ((((u32 *)bg_still_bitstream)[0] & 0xFFFF0000) != 0x38000000 || (((u32 *)bg_still_bitstream)[1] & 0xFF010000) != 0x10000) {
        dec_bg_reset();
        mode_step_next();
        game_state.mode_step -= 3;
        return 0;
    }
    dec_bg_decode_to_vram(0);
    mode_step_next();
}

void encounter_yumi_closing_bg_show(void) {
    dec_bg_show_switch(0);
    mode_step_next();
}

void encounter_yumi_closing_talk_chime(void) {
    encounter_scene_talk();
    if ((u16)D_800CA154 == 2) {
        if (game_state.mode_step_timer.w++ == 0) {
            audio_command(0x603);
        }
    }
}

void encounter_yumi_audio_command_1(void) {
    audio_command(0x200);
    mode_step_next();
}

void encounter_yumi_assets_load_2(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x8EFE);
    encounter_yumi_assets_bind_2();
    mode_step_next();
}

void encounter_yumi_audio_command_2(void) {
    audio_command(0x500);
    mode_step_next();
}

void encounter_yumi_audio_load_1(void) {
    cd_read_checked = 0;
    audio_bank_load_start(1, 0xC65F, 0xC659, 0xD9E1, 0xD993, 0xD98E);
    mode_step_next();
}

void encounter_yumi_closing_talk_portrait_update(void) {
    encounter_scene_talk();
    if (D_80122CE4 != 0) {
        D_80122CE4 = 0;
        D_80122CF0 = 0;
        D_8011ED5B &= 0xFF7F;
        talk_voice_wait_frames = 0;
        D_800CA150 = (u16) D_800CA150 + 1;
        D_800CA154 = 0;
        mode_step_next();
    }
}

void encounter_yumi_portrait_restore(void) {
    s16 i;

    portrait_value_clear_if_high(get_g_zyotai_h(GIRL_YUMI));
    D_800B5BD4 = 0xF;
    for (i = 0; i < 0xA; i++) {
        ((Actor *)actor_pool)[i + 99].draw_flags &= ~0x80;
    }
    mode_step_next();
}

s16 encounter_yumi_zoom_flash_start(void) {
    if (game_state.mode_step_timer.w == 0) {
        D_801206DB |= 0x80;
        D_801206F4 = 0;
        D_801206F2 = 0;
        audio_command(0x501);
    }
    D_801206F2 += 0x80;
    D_801206F4 += 0x80;
    if (D_801206F2 >= 0x1000) {
        D_801206DB &= 0xFF7F;
        encounter_yumi_white_flash_enabled = 1;
        mode_step_next();
    }
    game_state.mode_step_timer.w += 1;
}

void encounter_yumi_white_flash_draw(void) {
    encounter_color_quad_draw(-0xA0, -0x78, 0x140, 0xA0, 0xFFFFFF, 0xA, 0);
    dtd_on(9);
}

/* FAKE: the explicit "& 0xFF" masks and one source line per colour trio, as in the matched main sibling
 * _sprite_set_box_shade_tarao (T-17001; one colour instead of two): the masks leave the shifts in ugen
 * temporaries (t6, t7) and as1 schedules the stores by source line. The real source is unknown. T-24017 */
void encounter_color_quad_draw(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u32 arg4, s32 arg5, s32 arg6) {
    GameState *gs;
    POLY_G4 *p;

    p = (POLY_G4 *)GetWorkBase(0x24, draw_buffer);
    func_8009F054(p);
    SetSemiTrans(p, arg6);
    gs = &game_state;
    p->r0 = (arg4 >> 16) & 0xFF; p->g0 = (arg4 >> 8) & 0xFF; p->b0 = arg4 & 0xFF;
    p->r1 = (arg4 >> 16) & 0xFF; p->g1 = (arg4 >> 8) & 0xFF; p->b1 = arg4 & 0xFF;
    p->r2 = (arg4 >> 16) & 0xFF; p->g2 = (arg4 >> 8) & 0xFF; p->b2 = arg4 & 0xFF;
    p->r3 = (arg4 >> 16) & 0xFF; p->g3 = (arg4 >> 8) & 0xFF; p->b3 = arg4 & 0xFF;
    p->x0 = gs->draw_origin_x[draw_buffer] + arg0;
    p->y0 = gs->draw_origin_y[draw_buffer] + arg1;
    p->x1 = gs->draw_origin_x[draw_buffer] + arg0 + arg2;
    p->y1 = gs->draw_origin_y[draw_buffer] + arg1;
    p->x2 = gs->draw_origin_x[draw_buffer] + arg0;
    p->y2 = gs->draw_origin_y[draw_buffer] + arg1 + arg3;
    p->x3 = gs->draw_origin_x[draw_buffer] + arg0 + arg2;
    p->y3 = gs->draw_origin_y[draw_buffer] + arg1 + arg3;
    AddPrim(draw_ot_tags + (draw_buffer << 10) + arg5 * 4, (s32)p);
}


void encounter_yumi_closing_actors_init(void) {
    s16 i;

    D_800B5BD4 = 0xA;
    message_window_title_show(0);
    scene_still_layer_set(1);
    D_80122D04 = 1;
    D_80120657 = 0x80;
    D_8012069B = 0x80;
    for (i = 0x3A; i < 0x40; i++) {
        func_80084C98(i, 0x80);
    }
    encounter_yumi_white_flash_enabled = 0;
    for (i = 0; i < 0xA; i++) {
        ((Actor *)actor_pool)[i + 99].draw_flags |= 0x80;
    }
    encounter_yumi_closing_actor_timer = 1;
    mode_step_next();
}

/* FAKE: byte-offset display-record view matches the EVENT sibling (fields add one register, T-20900);
 * `if (any == 0 && rand()) {}` (where EVENT sibling func_80108DD4 has a sound trigger inside the if)
 * keeps the post-call label that prevents as1 from hoisting $ra into the branch delay slot. T-24090 */
void encounter_yumi_closing_actors_update(void) {
    s16 i;
    s32 fake_any_actor;

    for (i = 0; i < 0xA; i++) {
        if (encounter_yumi_closing_actor_timer >= (u32)(i * 0x1E)) {
            *(s16 *)&actor_pool[i * 0x44 + 0x1A7A] = -1;
            *(s16 *)&actor_pool[i * 0x44 + 0x1A7E] = 1;
        }
        *(s32 *)&actor_pool[i * 0x44 + 0x1A70] += *(s32 *)&actor_pool[i * 0x44 + 0x1A78];
        *(s32 *)&actor_pool[i * 0x44 + 0x1A74] += *(s32 *)&actor_pool[i * 0x44 + 0x1A7C];
        if (*(s16 *)&actor_pool[i * 0x44 + 0x1A76] > 0xA0) {
            actor_pool[i * 0x44 + 0x1A4F] &= ~0x80;
        }
    }
    fake_any_actor = 0;
    i = 0;
    do {
        if (actor_pool[i * 0x44 + 0x1A4F] & 0x80) {
            fake_any_actor = 1;
        }
        i++;
    } while (i < 0xA);
    if (fake_any_actor == 0 && rand()) {
    }
    encounter_yumi_closing_actor_timer += 1;
}

void encounter_yumi_scene_init_1(void) {
    game_state.current_girl = GIRL_YUMI;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    message_window_set_title("");
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    encounter_scene_scripts_bind();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    bg_brightness = 0;
    talk_names_set(game_state.current_girl);
    talk_clear();
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_67;
    encounter_scene_command_table = encounter_scene_command_section_67;
    encounter_scene_voice_table = encounter_scene_voice_section_67;
    strcpy(scene_place_title, "廊下");
    if (talk_work == 1) {
        game_state.girls[0xA].tokimeki += 1;
        game_state.girls[0xA].affection += 2;
        game_state.param_condition.value -= 5;
        game_state.param_guts.value += 5;
        check_para_limit();
    }
    D_800CA368 = 0x43;
    mode_step_next();
}

void encounter_yumi_still_show_1(void) {
    encounter_yumi_assets_bind_1();
    load_palette(encounter_yumi_pack_1_palette, 0x11, 1, 2, 0);
    scene_still_show(encounter_yumi_pack_1_still_image_0, encounter_yumi_pack_1_still_image_1, encounter_yumi_pack_1_still_image_2, encounter_yumi_pack_1_still_image_3, encounter_yumi_pack_1_still_image_4, encounter_yumi_pack_1_still_image_5);
    scene_anim_show(encounter_yumi_pack_1_animation_input_0, encounter_yumi_pack_1_animation_input_1, encounter_yumi_pack_1_animation_input_2, encounter_yumi_pack_1_animation_input_3);
    mode_step_next();
}

void encounter_yumi_scene_init_2(void) {
    u8 girl_state;

    game_state.current_girl = GIRL_YUMI;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    strcpy(scene_place_title, "廊下");
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    encounter_scene_scripts_bind();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    bg_brightness = 0;
    talk_names_set(game_state.current_girl);
    talk_clear();
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_68;
    encounter_scene_command_table = encounter_scene_command_section_68;
    encounter_scene_voice_table = encounter_scene_voice_section_68;
    girl_state = get_g_zyotai_s(GIRL_YUMI) & 0x7F;
    if ((u32)girl_state < 2U) {
        game_state.girls[10].tokimeki += 1;
    }
    game_state.girls[10].affection += 2;
    check_para_limit();
    D_800CA368 = 0x44;
    mode_step_next();
}

void encounter_yumi_scene_init_3(void) {
    s16 i;

    game_state.current_girl = GIRL_YUMI;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    message_window_set_title("");
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    encounter_scene_scripts_bind();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    D_80122D38 = 1;
    hud_icon_bar_init();
    for (i = 0; i < 0xF; i++) {
        icon_can_use_set(i, 0);
    }
    hud_icon_bar_update(1);
    parameter_show_init();
    parameter_show_init();
    bg_brightness = 0x80;
    encounter_yumi_white_flash_enabled = 0;
    encounter_yumi_closing_actor_timer = 0;
    talk_names_set(game_state.current_girl);
    encounter_scene_scripts_bind();
    encounter_scene_text_table = encounter_scene_text_section_73;
    encounter_scene_command_table = encounter_scene_command_section_73;
    encounter_scene_voice_table = encounter_scene_voice_section_73;
    strcpy(scene_place_title, "玄関");
    strcpy(scene_place_title_2, "自宅前");
    game_state.girls[game_state.current_girl].affection += 2;
    check_para_limit();
    D_800CA368 = 0x49;
    mode_step_next();
}

void encounter_yumi_closing_still_show(void) {
    /* FAKE: unused 4-byte local places t0..t2 at the original stack offsets; real source unknown. T-24092 */
    s32 fake_stack_pad;
    s16 i;
    GekoShorts10 t0;
    GekoShorts10 t1;
    GekoShorts10 t2;

    t0 = encounter_yumi_closing_actor_initial_values;
    t1 = encounter_yumi_closing_actor_x;
    t2 = encounter_yumi_closing_actor_y;
    encounter_yumi_assets_bind_2();
    load_palette(encounter_yumi_pack_2_palette, 0x11, 1, 2, 0);
    scene_still_show(encounter_yumi_pack_2_still_image_0, encounter_yumi_pack_2_still_image_1, encounter_yumi_pack_2_still_image_2, encounter_yumi_pack_2_still_image_3, encounter_yumi_pack_2_still_image_4, encounter_yumi_pack_2_still_image_5);
    for (i = 0; i < 6; i++) {
        game_state.quad_layer[i + 0x3A] = 0xC;
    }
    scene_anim_show(encounter_yumi_pack_2_animation_input_0, encounter_yumi_pack_2_animation_input_1, encounter_yumi_pack_2_animation_input_2, encounter_yumi_pack_2_animation_input_3);
    scene_mouth_frames_reversed_saved = 1;
    ((Actor *)actor_pool_96)[1].unk_04 = 8;
    ((Actor *)actor_pool_96)->unk_04 = 8;
    ((Actor *)actor_pool_96)[1].unk_01 = 0xC;
    ((Actor *)actor_pool_96)->unk_01 = 0xC;
    ((Actor *)actor_pool_96)[2] = ((Actor *)actor_pool_96)[0];
    ((Actor *)actor_pool_96)[2].unk_01 = 0xB;
    ((Actor *)actor_pool_96)[2].flags = 0x40;
    ((Actor *)actor_pool_96)[2].unk_18 = 0x14;
    ((Actor *)actor_pool_96)[2].x = -6;
    ((Actor *)actor_pool_96)[2].y = -0x30;
    ((Actor *)actor_pool_96)[2].brightness = 0x80;
    for (i = 0; i < 0xA; i++) {
        ((Actor *)actor_pool)[i + 99] = ((Actor *)actor_pool_96)[2];
        ((Actor *)actor_pool)[i + 99].unk_18 = t0.v[i];
        ((Actor *)actor_pool)[i + 99].x = t1.v[i] - 0xA0;
        ((Actor *)actor_pool)[i + 99].y = t2.v[i] - 0x78;
    }
    mode_step_next();
}

void encounter_yumi_bg_load_1(void) {
    bg_read_sub2(0x4045);
    mode_step_next();
}
