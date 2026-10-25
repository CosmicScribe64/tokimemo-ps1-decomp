/* labels_intro_and_wave_transition.c: src/ovl/RPG_BAT/801525B0.c:8 creates name labels; :94 slides in enemies; :219 runs alien intro; :419 and :546 generate wave strips. Original object 0x801525B0-0x80154430 (config/objects/RPG_BAT.txt). */
#define MAIN_API_OVERRIDE_prim_gradient_rect /* unprototyped view: keeps 16-bit widening off the negative x/y arguments in rpg_stage_intro_aliens (T-24124) */
#include "common.h"
#include "ovl/RPG_BAT.h"
#include "psx_types.h"

void prim_gradient_rect();

void rpg_stage_labels_init(void) {
    s32 sp28; /* FAKE (T-17100): dead copy of the column; without it the 0x28 spill of var_a3 and the frame change */
    s32 var_a3;

    game_state.screen_brightness = 0;
    rpg_label_player = rpg_label_create(0xB1, 0xB3, (s32) game_state.player_family_name) - 1;
    if (rpg_girl_id != 0xE) {
        sp28 = 0xD3;
        var_a3 = 0xD3;
        rpg_label_girl = rpg_label_create(0xB1, 0xC3, (s32) girl_short_name_table[rpg_girl_id]) - 1;
    } else {
        var_a3 = 0xC3;
    }
    switch (rpg_encounter) {
    case 0x10000:
        rpg_label_guest = rpg_label_create(0xB1, var_a3, (s32) "マングース") - 1;
        rpg_label_enemy = rpg_label_create(0x13, 0xBB, (s32) "ハブ・ロード") - 1;
        break;
    case 0x20000:
        rpg_label_guest = rpg_label_create(0xB1, var_a3, (s32) "虚無僧") - 1;
        rpg_label_enemy = rpg_label_create(0x13, 0xBB, (s32) "鹿・グレート") - 1;
        break;
    case 0x40000:
        rpg_label_guest = rpg_label_create(0xB1, var_a3, (s32) "マタギ") - 1;
        rpg_label_enemy = rpg_label_create(0x13, 0xBB, (s32) "キラーヒグマ") - 1;
        break;
    case 0x100000:
        rpg_label_guest = rpg_label_create(0x13, 0xBB, (s32) "番長") - 1;
        rpg_label_enemy = rpg_label_create(0x13, 0xBB, (s32) "不良達") - 1;
        break;
    case 0x80000000:
        rpg_label_enemy = rpg_label_create(0x13, 0xBB, (s32) "宇宙人") - 1;
        break;
    case 0x8000000:
        rpg_label_girl = rpg_label_create(0x17, 0xBB, (s32) "紐緒") - 1;
        rpg_label_guest = rpg_label_create(0x17, 0xBB, (s32) "真・世界征服") - 1;
        rpg_label_enemy = rpg_label_create(0x17, 0xBB, (s32) "真・世界征服ロボ") - 1;
        break;
    }
    k_disp_start(rpg_label_enemy + 1);
    k_disp_switch(rpg_label_player, 1);
    k_disp_switch(rpg_label_enemy, 1);
    k_disp_switch(rpg_label_guest, 0);
    if (rpg_encounter == 0x08000000) {
        k_disp_switch(rpg_label_girl, 1);
        k_disp_switch(rpg_label_guest, 0);
        k_disp_switch(rpg_label_enemy, 0);
    }
    if (rpg_encounter == 0x80000000) {
        k_disp_switch(rpg_label_enemy, 0);
    }
    if (rpg_girl_id != 0xE) {
        k_disp_switch(rpg_label_girl, 1);
    }
    k_sub_reset_point_set();
}

void rpg_stage_fade_out(void) {
    s32 i;
    u8 *p;

    p = (u8 *)&game_state.mode_stage_scratch;
    if (game_state.mode_stage_timer == 0) {
        game_state.mode_stage_timer += 1;
        *p = 0x80;
    }
    i = 0;
    do {
        ((Actor *)actor_pool)[i + 64].brightness = *p;
        game_state.screen_brightness = *p;
        if (rpg_encounter == 0x20000 && *p >= 0x61) {
            D_80120E93 = 0x60;
        }
        i++;
    } while (actor_pool_96 != &actor_pool[i * 0x44]);
    *p -= 2;
    if (*p == 0) {
        text_system_reset();
        game_state.screen_brightness = 0x80;
        actor_clear_range(0);
        hud_cursor_actor_init(0);
        back_clear_switch(1);
        mode_stage_next();
    }
}

void rpg_stage_intro_slide_in(void) {
    rpg_message_update();
    switch (game_state.mode_step) {
    case 0:
        audio_bank_start(1, 0x200);
        switch (rpg_encounter) {
        case 0x10000:
            strcpy(rpg_text_result_0, "ハブ・ロードが襲ってきた！");
            break;
        case 0x20000:
            strcpy(rpg_text_result_0, "鹿・グレートが襲ってきた！");
            break;
        case 0x40000:
            strcpy(rpg_text_result_0, "キラーヒグマが襲ってきた！");
            break;
        case 0x100000:
            strcpy(rpg_text_result_0, "不良達が襲ってきた！");
            break;
        }
        if (rpg_encounter != 0x08000000) {
            rpg_message_show((s32) rpg_text_result_pointers, 1, 0x5A);
        }
        rpg_actor_set_animation_alt(0x30, 1, 1, 1);
        switch (rpg_encounter) {
        case 0x10000:
        case 0x20000:
        case 0x40000:
        case 0x08000000:
            rpg_actor_set_animation_alt(0x3B, 0, 1, 1);
            break;
        case 0x100000:
            rpg_actor_set_animation_alt(0x3B, 0, 1, 1);
            rpg_actor_set_animation_alt(0x3C, 0, 1, 1);
            rpg_actor_set_animation_alt(0x3D, 0, 1, 1);
            break;
        }
        if (rpg_girl_id != 0xE) {
            switch (rpg_girl_id) {
            case 0:
                rpg_actor_set_animation_alt(0x1D, 2, 1, 1);
                break;
            case 1:
                rpg_actor_set_animation_alt(0x1D, 2, 1, 1);
                break;
            case 2:
                rpg_actor_set_animation_alt(0x1D, 9, 1, 1);
                break;
            case 3:
                rpg_actor_set_animation_alt(0x1D, 2, 1, 1);
                break;
            case 4:
                rpg_actor_set_animation_alt(0x1D, 1, 1, 1);
                break;
            case 5:
                rpg_actor_set_animation_alt(0x1D, 0, 1, 1);
                break;
            case 6:
                rpg_actor_set_animation_alt(0x1D, 1, 1, 1);
                break;
            case 7:
                rpg_actor_set_animation_alt(0x1D, 1, 1, 1);
                break;
            case 8:
                rpg_actor_set_animation_alt(0x1D, 1, 1, 1);
                break;
            case 9:
                rpg_actor_set_animation_alt(0x1D, 1, 1, 1);
                break;
            case 10:
                rpg_actor_set_animation_alt(0x1D, 1, 1, 1);
                break;
            case 13:
                rpg_actor_set_animation_alt(0x1D, 1, 1, 1);
                break;
            }
        }
        rpg_actor_set_position(0x30, 0x160, 0x58);
        rpg_actor_set_position(0x1D, 0x168, 0x88);
        rpg_actor_set_position(0x3B, -0x58, 0x60);
        if (rpg_encounter == 0x100000) {
            rpg_actor_set_position(0x3B, -0x28, 0x68);
            rpg_actor_set_position(0x3C, -0x60, 0x58);
            rpg_actor_set_position(0x3D, -0x68, 0x88);
        }
        if (rpg_encounter == 0x08000000) {
            rpg_actor_set_position(0x3B, -0x58, 0x70);
        }
        mode_step_next();
        break;
    case 1:
        rpg_actor_set_position(0x30, 0x160 - game_state.mode_step_timer.u, 0x58);
        rpg_actor_set_position(0x1D, 0x168 - game_state.mode_step_timer.u, 0x88);
        rpg_actor_set_position(0x3B, (game_state.mode_step_timer.u * 2) - 0x58, 0x60);
        if (rpg_encounter == 0x100000) {
            rpg_actor_set_position(0x3B, (game_state.mode_step_timer.u * 2) - 0x28, 0x68);
            rpg_actor_set_position(0x3C, (game_state.mode_step_timer.u * 2) - 0x60, 0x58);
            rpg_actor_set_position(0x3D, (game_state.mode_step_timer.u * 2) - 0x68, 0x88);
        }
        if (rpg_encounter == 0x08000000) {
            rpg_actor_set_position(0x3B, (game_state.mode_step_timer.u * 2) - 0x58, 0x70);
        }
        game_state.mode_step_timer.u += 4;
        if (game_state.mode_step_timer.u >= 0x51) {
            rpg_actor_set_position(0x30, 0, 0);
            rpg_actor_set_animation(0x30, 0, 1, 1);
            rpg_actor_set_position(0x1D, 0, 0);
            if (rpg_girl_id != 0xE) {
                rpg_actor_set_animation(0x1D, 0, 1, 1);
            }
            mode_step_next();
        }
        break;
    case 2:
        game_state.mode_step_timer.u += 1;
        if (game_state.mode_step_timer.u >= 0x3D) {
            mode_stage_next();
        }
        break;
    }
}

/* FAKE: x and y are unused locals that place color at sp+0x3C in the 0x48 frame; *(u8 *)&game_state.mode_step bypasses cvt_pass.py
 * widening so the four loads of unk_110A stay in ugen temporaries ($t7..$t9); in case 3, the *(s32 *)& reads of rpg_bg_word_0/3C/40/44/4C
 * keep rpg_bg_word_0 and rpg_bg_word_1 promoted to $a1/$a2 across the function and allocate $v0, $v1, $a3 to rpg_bg_word_3, rpg_bg_word_2,
 * rpg_bg_word_5 so uopt flushes the six stores in the original order. T-24124 */
void rpg_stage_intro_aliens(void) {
    s32 x;
    s32 y;
    RpgPackedColor color;
    u32 selector;

    selector = rpg_rand(8);
    switch (selector) {
    case 0:
        color.word = 0x9F9F9F;
        break;
    case 1:
        color.word = 0x6F6F6F;
        break;
    case 2:
        color.word = 0xBF0000;
        break;
    case 3:
        color.word = 0xBF00;
        break;
    case 4:
        color.word = 0xBF;
        break;
    case 5:
        color.word = 0xAFAF00;
        break;
    case 6:
        color.word = 0xAF00AF;
        break;
    case 7:
        color.word = 0xAFAF;
        break;
    }
    rpg_message_update();
    switch (*(u8 *)&game_state.mode_step) {
    case 0:
        game_state.current_girl = GIRL_SOTOI;
        cd_audio_request_set(0x1C0000A8, 0x0F0000B7, 0xE);
        audio_command(0x300);
        mode_step_next();
        break;
    case 1:
        audio_command(0xE4);
        mode_step_next();
        break;
    case 2:
        if (cd_audio_state() & 4) {
            mode_step_next();
        }
        break;
    case 3:
        rpg_actor_set_animation_alt(0x30, 1, 1, 1);
        rpg_actor_set_animation_alt(0x1D, 0x17, 1, 1);
        rpg_actor_set_position(0x30, 0x160, 0x58);
        rpg_actor_set_position(0x1D, 0x168, 0x88);
        rpg_bg_word_4 = 0;
        rpg_bg_word_3 = 0;
        rpg_bg_word_2 = 0;
        rpg_bg_word_5 = 0;
        rpg_bg_word_3 = *(s32 *)&rpg_bg_word_3;
        rpg_bg_word_2 = *(s32 *)&rpg_bg_word_2;
        rpg_bg_word_5 = *(s32 *)&rpg_bg_word_5;
        rpg_bg_word_0 = 0;
        rpg_bg_word_1 = -0x1E;
        rpg_actor_set_position(0x2F, *(s32 *)&rpg_bg_word_0, *(s32 *)&rpg_bg_word_1);
        rpg_actor_set_animation_alt(0x2F, 2, 1, 1);
        rpg_actor_set_position(0x2E, rpg_bg_word_0, rpg_bg_word_1);
        rpg_actor_set_animation_alt(0x2E, 6, 1, 1);
        mode_step_next();
        break;
    case 4:
        rpg_actor_set_position(0x30, 0x160 - game_state.mode_step_timer.u, 0x58);
        rpg_actor_set_position(0x1D, 0x168 - game_state.mode_step_timer.u, 0x88);
        game_state.mode_step_timer.u += 4;
        if (game_state.mode_step_timer.u >= 0x51U) {
            rpg_actor_set_position(0x30, 0, 0);
            rpg_actor_set_animation(0x30, 0, 1, 1);
            rpg_actor_set_position(0x1D, 0, 0);
            rpg_actor_set_animation(0x1D, 0x18, 1, 1);
            rpg_actor_set_position(0x38, 0, 0);
            rpg_actor_set_animation(0x38, 9, 1, 1);
            ((Actor *)actor_pool)[152].unk_01 = 2;
            mode_step_next();
        }
        break;
    case 5:
        prim_gradient_rect(-0x5C - (game_state.mode_step_timer.u >> 1), -0x78, (game_state.mode_step_timer.u >> 1) * 2, 0xA8, 3, color.word, 0x81);
        dtd_on_tpage(0, 0, 3, 1, 0);
        game_state.mode_step_timer.u += 1;
        if (game_state.mode_step_timer.u >= 0x41U) {
            mode_step_next();
        }
        break;
    case 6:
        prim_gradient_rect(-0x7C, -0x78, 0x40, 0xA8, 3, color.word, 0x81);
        dtd_on_tpage(0, 0, 3, 1, 0);
        rpg_actor_set_position(0x3B, 0x48, game_state.mode_step_timer.u - 0x80);
        rpg_actor_set_animation_alt(0x3B, 8, 1, 1);
        game_state.mode_step_timer.u += 1;
        if (game_state.mode_step_timer.u >= 0xE9U) {
            k_disp_switch(rpg_label_enemy, 1);
            mode_step_next();
        }
        break;
    case 7:
        prim_gradient_rect(-0x7C, -0x78, 0x40, 0xA8, 3, color.word, 0x81);
        game_state.mode_step_timer.u += 1;
        dtd_on_tpage(0, 0, 3, 1, 0);
        if (game_state.mode_step_timer.u >= 0x79U) {
            mode_step_next();
        }
        break;
    case 8:
        prim_gradient_rect(game_state.mode_step_timer.u - 0x7C, -0x78, 0x40 - (game_state.mode_step_timer.u * 2), 0xA8, 3, color.word, 0x81);
        dtd_on_tpage(0, 0, 3, 1, 0);
        game_state.mode_step_timer.u += 1;
        if (game_state.mode_step_timer.u >= 0x21U) {
            mode_step_next();
        }
        break;
    case 9:
        rpg_bg_word_0 = 0x40;
        rpg_bg_word_1 = game_state.mode_step_timer.u - 0x40;
        game_state.mode_step_timer.u += 1;
        if (game_state.mode_step_timer.u >= 0x59U) {
            mode_step_next();
        }
        break;
    case 10:
        strcpy(rpg_text_result_0, "宇宙人が襲ってきた！");
        rpg_message_show((s32)rpg_text_result_pointers, 1, 0x5A);
        rpg_actor_set_animation(0x38, 0xFF, 1, 0);
        rpg_actor_set_position(0x3B, 0, 0);
        rpg_actor_set_animation(0x3B, 0, 1, 1);
        mode_step_next();
        break;
    case 11:
        if (rpg_message_mode == 0) {
            mode_step_next();
        }
        break;
    case 12:
        rpg_actor_set_position(0x1E, 0, 0);
        rpg_actor_set_animation(0x1E, 0, 5, 1);
        mode_step_next();
        break;
    case 13:
        rpg_dialog_yuina_hold_out();
        break;
    case 14:
        rpg_girl_action = 0x10001000;
        mode_stage_next();
        break;
    }
    if (*(u8 *)&game_state.mode_step >= 9) {
        rpg_alien_scene_flicker();
    }
    if ((*(u8 *)&game_state.mode_step >= 5) && (*(u8 *)&game_state.mode_step < 8)) {
        rpg_se_play_latched_5xx(0x505, 0x6E, 7);
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_yuina_hold_out(void) {
    if (game_state.pad_pressed & PAD_CROSS) {
        audio_command(0x74);
        if (rpg_message.unk_00 >= 3 && rpg_message.unk_08 >= 3) {
            rpg_message.unk_08 = rpg_message_line_frames + 1;
        }
    }
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_result_0, "紐緒「私が準備するまで、");
        strcpy(rpg_text_result_1, "持ちこたえなさい。");
        rpg_actor_set_animation(0x1D, 0x19, 1, 1);
        rpg_message_show(rpg_text_result_pointers, 2, 0x96);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x2E, 0x07000033, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x34, 0x08000037, 1);
        rpg_dialog_step[0] += 1;
        return;
    case 4:
        rpg_voice_wait(0);
        return;
    case 5:
        rpg_actor_set_animation(0x1D, 0x18, 1, 1);
        rpg_voice_reset();
        mode_step_next();
        return;
    }
}

s32 rpg_stage_wave_transition(void) {
    s32 pad[3]; /* FAKE: unused words above the RECTs, the original frame has them (real source unknown). T-17112 */
    RECT r0;
    RECT r1;
    RECT r2;
    RECT r3;
    s32 i;

    /* FAKE: x/y and w/h of each RECT share a source line; as1 then stores y, x, h, w as the original does. T-17112 */
    switch (game_state.mode_step) {
    case 0:
        r0.x = 0x240; r0.y = 0x100;
        r0.w = 0x40; r0.h = 0x28;
        func_8009C93C(&r0, 0x180, 0x180);
        r1.x = 0x240; r1.y = 0x128;
        r1.w = 0x40; r1.h = 0x28;
        func_8009C93C(&r1, 0x1C0, 0x180);
        r2.x = 0x220; r2.y = 0x100;
        r2.w = 0x40; r2.h = 0x28;
        func_8009C93C(&r2, 0x200, 0x180);
        r3.x = 0x21F; r3.y = 0x100;
        r3.w = 1; r3.h = 0x80;
        func_8009C93C(&r3, 0x220, 0x100);
        rpg_bg_word_2 = 0x20;
        rpg_bg_word_3 = 0x40;
        rpg_bg_word_4 = 1;
        rpg_ripple_phase_2_step = -0x80;
        rpg_ripple_period_2 = 0x20;
        rpg_ripple_width_2 = 0x40;
        rpg_voice_playing_seen = 1;
        rpg_bg_word_0 = 0;
        rpg_bg_word_1 = 0x80;
        /* FAKE: rpg_ripple_phase_2 through the rpg_bg_word_0 symbol as in case 1; under its own name uopt moves this store above the rpg_bg_word_0 store. T-17112 */
        (&rpg_bg_word_0)[60] = 0;
        mode_step_next();
        break;
    case 1:
        /* FAKE: rpg_bg_word_2 and rpg_ripple_phase_2..rpg_voice_playing_seen (+0x08, +0xF0..+0x100) are reached through the rpg_bg_word_0 symbol; the original
         * relocations probably name one aggregate over these words that is not recovered yet. as1 keeps a load and a store of one
         * symbol in source order: the rpg_ripple_phase_2/rpg_voice_playing_seen loads stay above the rpg_bg_word_0 store, the rpg_bg_word_2 and rpg_ripple_period_2
         * loads below the stores, and the rpg_ripple_period_2 lui fills the load delay of rpg_bg_word_2. T-17112 */
        rpg_bg_word_0 += rpg_bg_word_4;
        (&rpg_bg_word_0)[60] += (&rpg_bg_word_0)[64];
        if (rpg_bg_word_0 >= (&rpg_bg_word_0)[2]) {
            rpg_bg_word_0 = 0;
        }
        if ((&rpg_bg_word_0)[60] >= (&rpg_bg_word_0)[62]) {
            (&rpg_bg_word_0)[60] = 0;
        }
        rpg_wave_strips_a();
        rpg_wave_strips_b();
        rpg_wave_fade_step();
        rpg_ripple_phase_2_step += 1;
        rpg_bg_word_1 -= 1;
        if (rpg_bg_word_1 == 0) {
            mode_step_next();
        }
        break;
    case 2:
        r2.x = 0x200; r2.y = 0x180;
        r2.w = 1; r2.h = 0x28;
        func_8009C93C(&r2, 0x220, 0x100);
        rpg_actor_set_animation(0x1F, 0, 1, 1);
        mode_step_next();
        break;
    case 3:
        rpg_scratch = 0;
        /* FAKE: the 48 words from rpg_move_value_0 through the first symbol, with rpg_move_timer on the loop's line to keep its store behind the loop setup. T-17112 */
        rpg_move_timer = 0; for (i = 0; i < 48; i++) (&rpg_move_value_0)[i] = 0;
        mode_stage_next();
        break;
    }
}

void rpg_wave_fade_step(void) {
    int dead; /* FAKE: dead copy of *p; only reserves $v0 so the table pointer lands in $v1. Found with decomp-permuter. T-10910 */
    s32 i;
    s32 *p;

    i = 0;
    p = &rpg_scratch;
    do {
        ((Actor *)actor_pool)[i + 64].brightness = *p;
        *(s8 *)&game_state.screen_brightness = *p;
        if (rpg_encounter == 0x20000 && (dead = *p) >= 0x61) {
            D_80120E93 = 0x60;
        }
        if (0) { /* FAKE: empty dead branch steers the allocation. T-10910 */
        }
        i++;
    } while (actor_pool_96 != &actor_pool[i * 0x44]);
    rpg_actor_set_animation(0x1F, 0, 1, 0);
    rpg_scratch += 2;
    if (*p > 0x80) {
        rpg_scratch = 0x80;
    }
}

void rpg_wave_strips_a(void) {
    s32 temp_s1;
    s32 temp_s3;
    s32 var_s0;

    var_s0 = 0;
    do {
        temp_s1 = var_s0 + 4;
        temp_s3 = (s32) (rsin(((s32) ((var_s0 % (s32) rpg_bg_word_3) << 0xC) / (s32) rpg_bg_word_3) + ((s32) (rpg_bg_word_0 << 0xC) / (s32) rpg_bg_word_2)) * rpg_bg_word_1) >> 0xD;
        rpg_wave_strip_draw(temp_s3, (s32) (rsin(((s32) ((temp_s1 % (s32) rpg_bg_word_3) << 0xC) / (s32) rpg_bg_word_3) + ((s32) (rpg_bg_word_0 << 0xC) / (s32) rpg_bg_word_2)) * rpg_bg_word_1) >> 0xD, var_s0, temp_s1, 0, 5);
        var_s0 += 5;
    } while (var_s0 != 0xAA);
}

void rpg_wave_strips_b(void) {
    s32 temp_s1;
    s32 temp_s3;
    s32 var_s0;

    var_s0 = 0;
    do {
        temp_s1 = var_s0 + 4;
        temp_s3 = (s32) (rsin(((s32) ((var_s0 % (s32) rpg_ripple_width_2) << 0xC) / (s32) rpg_ripple_width_2) + ((s32) (rpg_ripple_phase_2 << 0xC) / (s32) rpg_ripple_period_2)) * rpg_ripple_phase_2_step) >> 0xD;
        rpg_wave_strip_draw(temp_s3, (s32) (rsin(((s32) ((temp_s1 % (s32) rpg_ripple_width_2) << 0xC) / (s32) rpg_ripple_width_2) + ((s32) (rpg_ripple_phase_2 << 0xC) / (s32) rpg_ripple_period_2)) * rpg_ripple_phase_2_step) >> 0xD, var_s0, temp_s1, 2, 5);
        var_s0 += 5;
    } while (var_s0 != 0xAA);
}

/* FAKE: the two x stores of each point pair share a source line; as1 schedules the stores by source line and only then reproduces the original order (T-17112). */
void rpg_wave_strip_draw(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 xy[8];
    s32 uv[8];

    xy[0] = arg0; xy[2] = arg0 + 0x7F;
    xy[1] = arg2;
    xy[3] = arg2;
    xy[4] = arg1; xy[6] = arg1 + 0x7F;
    xy[5] = arg3;
    xy[7] = arg3;
    uv[0] = 0; uv[2] = 0x7F;
    uv[1] = arg2;
    uv[3] = arg2;
    uv[4] = 0; uv[6] = 0x7F;
    uv[5] = arg3;
    uv[7] = arg3;
    rpg_wave_quad_draw(xy, uv, (D_80120E93 + arg4) >> 1, arg5, 1);
    xy[0] = arg0 + 0x80; xy[2] = arg0 + 0xFF;
    xy[1] = arg2;
    xy[3] = arg2;
    xy[4] = arg1 + 0x80; xy[6] = arg1 + 0xFF;
    xy[5] = arg3;
    xy[7] = arg3;
    uv[0] = 0; uv[2] = 0x7F;
    uv[1] = arg2;
    uv[3] = arg2;
    uv[4] = 0; uv[6] = 0x7F;
    uv[5] = arg3;
    uv[7] = arg3;
    rpg_wave_quad_draw(xy, uv, (D_80120E93 + arg4) >> 1, arg5, 2);
    xy[0] = arg0 + 0x100; xy[2] = arg0 + 0x13F;
    xy[1] = arg2;
    xy[3] = arg2;
    xy[4] = arg1 + 0x100; xy[6] = arg1 + 0x13F;
    xy[5] = arg3;
    xy[7] = arg3;
    uv[0] = 0; uv[2] = 0x3F;
    uv[1] = arg2;
    uv[3] = arg2;
    uv[4] = 0; uv[6] = 0x3F;
    uv[5] = arg3;
    uv[7] = arg3;
    rpg_wave_quad_draw(xy, uv, (D_80120E93 + arg4) >> 1, arg5, 3);
    xy[0] = arg0; xy[2] = arg0 - 0x7F;
    xy[1] = arg2;
    xy[3] = arg2;
    xy[4] = arg1; xy[6] = arg1 - 0x7F;
    xy[5] = arg3;
    xy[7] = arg3;
    uv[0] = 0; uv[2] = 0x7F;
    uv[1] = arg2;
    uv[3] = arg2;
    uv[4] = 0; uv[6] = 0x7F;
    uv[5] = arg3;
    uv[7] = arg3;
    rpg_wave_quad_draw(xy, uv, (D_80120E93 + arg4) >> 1, arg5, 1);
    xy[0] = arg0 + 0x17F; xy[2] = arg0 + 0x13F;
    xy[1] = arg2;
    xy[3] = arg2;
    xy[4] = arg1 + 0x17F; xy[6] = arg1 + 0x13F;
    xy[5] = arg3;
    xy[7] = arg3;
    uv[0] = 0; uv[2] = 0x3F;
    uv[1] = arg2;
    uv[3] = arg2;
    uv[4] = 0; uv[6] = 0x3F;
    uv[5] = arg3;
    uv[7] = arg3;
    rpg_wave_quad_draw(xy, uv, (D_80120E93 + arg4) >> 1, arg5, 3);
}

void rpg_wave_quad_draw(s32 *arg0, s32 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    POLY_FT4 *p;
    s32 y;

    p = (POLY_FT4 *)GetWorkBase(0x28, draw_buffer);
    func_8009F040(p);
    SetShadeTex(p, 0);
    if (arg3 != 0) {
        SetSemiTrans(p, 1);
    } else {
        SetSemiTrans(p, 0);
    }
    p->r0 = arg2;
    p->g0 = arg2;
    p->b0 = arg2;
    if (arg3 != 0) {
        p->clut = func_8009ED74(0, 0x1F7);
    } else {
        p->clut = func_8009ED74(0, 0x1F5);
    }
    p->tpage = func_8009ECB0(1, arg3 + 4, (arg4 << 6) + 0x140, 0x100);
    y = -draw_buffer * 0xF0 + 0xF0;
    p->x0 = arg0[0];
    p->y0 = arg0[1] + y;
    p->x1 = arg0[2] + 1;
    p->y1 = arg0[3] + y;
    p->x2 = arg0[4];
    p->y2 = arg0[5] + y + 1;
    p->x3 = arg0[6] + 1;
    p->y3 = arg0[7] + y + 1;
    p->u0 = arg1[0];
    p->v0 = arg1[1];
    p->u1 = arg1[2] + 1;
    p->v1 = arg1[3];
    p->u2 = arg1[4];
    p->v2 = arg1[5] + 1;
    p->u3 = arg1[6] + 1;
    p->v3 = arg1[7] + 1;
    AddPrim((draw_buffer << 0xA) + 0x20 + D_800E8CA0, (s32)p);
}
