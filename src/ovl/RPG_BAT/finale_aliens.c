/* finale_aliens.c: src/ovl/RPG_BAT/8014F790.c:31: alien encounter timer and rescue; body 35: rpg_girl_channel.unk_00 += 1; 38: if ((rpg_girl_channel.unk_04 >= 2) && (rpg_girl_channel.unk_04 < 0x15) && (((Actor *)actor_pool)[125].unk_08 == 1)) {; alien name-label creation src/ovl/RPG_BAT/labels_intro_and_wave_transition.c:39; survival/rescue dispatch src/ovl/RPG_BAT/801439E0.c:14. Original object 0x8014F790-0x801504E0 (config/objects/RPG_BAT.txt). */
#define MAIN_API_OVERRIDE_prim_gradient_rect /* T-9740: original caller passes full-width coordinates without a prototype. */
#include "common.h"
#include "ovl/RPG_BAT.h"

void prim_gradient_rect();

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_yuina_after_aliens(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_result_0, "紐緒「想定強度に問題があったみたいね");
        rpg_actor_set_animation(0x1D, 0x19, 1, 1);
        rpg_message_show(rpg_text_result_pointers, 1, 0xC8);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x38, 0x09000041, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(0);
        return;
    case 3:
        rpg_actor_set_animation(0x1D, 0x18, 1, 1);
        rpg_voice_reset();
        rpg_girl_step_next();
        return;
    }
}

void rpg_girl_survival_timer(void) {
    s32 sp28; /* FAKE (T-17100): dead copy of the loop index; without it the 0x28 spill and the frame change */
    s32 var_a0;

    rpg_girl_channel.unk_00 += 1;
    ((Actor *)actor_pool)[125].unk_01 = 3;
    ((Actor *)actor_pool)[126].unk_01 = 3;
    if ((rpg_girl_channel.unk_04 >= 2) && (rpg_girl_channel.unk_04 < 0x15) && (((Actor *)actor_pool)[125].unk_08 == 1)) {
        /* u16 view of the s16 unk_18: the original loads it with lhu and range-checks with sltiu */
        switch (*(u16 *)&((Actor *)actor_pool)[125].unk_18) {
        case 0:
        case 2:
            if (rpg_rand(3) == 0) {
                rpg_se_play_latched_5xx(0x50C, 1, 9);
            }
            if (rpg_rand(2) == 0) {
                rpg_se_play_latched_5xx(0x50B, 1, 9);
            }
            break;
        case 4:
        case 8:
        case 12:
            if (rpg_rand(2) == 0) {
                rpg_se_play_latched_5xx(0x50A, 1, 9);
            }
            break;
        }
    }
    switch (rpg_girl_channel.unk_04) {
    case 0:
        rpg_effects_select_asset_set(5);
        var_a0 = 4;
        do {
            sp28 = var_a0;
            rpg_actor_set_position(var_a0, -0x20, 0);
            var_a0 += 1;
        } while (var_a0 != 0x10);
        rpg_actor_set_animation(0x1D, 6, 1, 1);
        rpg_actor_set_animation(0x1E, 2, 1, 1);
        rpg_actor_set_position(0x1D, -0x20, 0);
        rpg_se_play_latched_5xx(0x509, 1, 0);
        rpg_girl_step_next();
        break;
    case 1:
        if (rpg_girl_channel.unk_00 >= 0x3D) {
            rpg_girl_step_next();
        }
        break;
    case 2:
        rpg_actor_set_animation(8, 7, 1, 1);
        rpg_girl_step_next();
        break;
    case 3:
        if (rpg_girl_channel.unk_00 >= 0x4B1) {
            rpg_girl_step_next();
        }
        break;
    case 4:
        rpg_actor_set_animation(8, 8, 1, 1);
        rpg_girl_step_next();
        break;
    case 5:
        if (rpg_girl_channel.unk_00 >= 0x961) {
            rpg_girl_step_next();
        }
        break;
    case 6:
        rpg_actor_set_animation(8, 9, 1, 1);
        rpg_girl_step_next();
        break;
    case 7:
        if (rpg_girl_channel.unk_00 >= 0xE11) {
            rpg_girl_step_next();
        }
        break;
    case 8:
        rpg_actor_set_animation(0x1E, 3, 1, 1);
        rpg_actor_set_animation(8, 0xA, 1, 1);
        rpg_girl_step_next();
        break;
    case 9:
        if (rpg_girl_channel.unk_00 >= 0x12C1) {
            rpg_girl_step_next();
        }
        break;
    case 10:
        rpg_actor_set_animation(8, 0xB, 1, 1);
        rpg_girl_step_next();
        break;
    case 11:
        if (rpg_girl_channel.unk_00 >= 0x1771) {
            rpg_girl_step_next();
        }
        break;
    case 12:
        rpg_actor_set_animation(0x1E, 4, 1, 1);
        rpg_actor_set_animation(8, 0xC, 1, 1);
        rpg_girl_step_next();
        break;
    case 13:
        if (rpg_girl_channel.unk_00 >= 0x1C21) {
            rpg_girl_step_next();
        }
        break;
    case 14:
        rpg_actor_set_animation(8, 0xD, 1, 1);
        rpg_girl_step_next();
        break;
    case 15:
        if (rpg_girl_channel.unk_00 >= 0x20D1) {
            rpg_girl_step_next();
        }
        break;
    case 16:
        rpg_actor_set_animation(0x1E, 5, 1, 1);
        rpg_actor_set_animation(8, 0xE, 1, 1);
        rpg_girl_step_next();
        break;
    case 17:
        if (rpg_girl_channel.unk_00 >= 0x2581) {
            rpg_girl_step_next();
        }
        break;
    case 18:
        rpg_actor_set_animation(8, 0xF, 1, 1);
        rpg_girl_step_next();
        break;
    case 19:
        if (rpg_girl_channel.unk_00 >= 0x2A31) {
            rpg_girl_step_next();
        }
        break;
    case 20:
        if (!(rpg_player_action & 0xFF) && !(rpg_enemy_action & 0xFF) && !(rpg_over & 0xFF)) {
            rpg_girl_action_end();
            rpg_se_play_latched_5xx(0x50D, 1, 0);
            rpg_girl_action = 0x10001001;
        }
        break;
    }
}

void rpg_girl_rescue_sequence(void) {
    D_80120E05 = 3;
    D_80120E49 = 3;
    switch (rpg_girl_step) {
    case 0:
        rpg_actor_set_position(0x1D, 0, 0);
        rpg_actor_set_animation(0x1D, 0x18, 1, 1);
        rpg_girl_step_next();
        return;
    case 1:
        rpg_girl_wait_step(0x3C);
        return;
    case 2:
        rpg_actor_set_animation(7, 0x10, 5, 1);
        rpg_girl_step_next();
        return;
    case 3:
        rpg_girl_wait_animation_step(7);
        return;
    case 4:
        rpg_actor_set_animation(7, 0x11, 1, 1);
        rpg_girl_step_next();
        return;
    case 5:
        rpg_actor_set_animation(6, 0x12, 1, 1);
        rpg_se_play_latched_5xx(0x50F, 1, 0);
        rpg_girl_step_next();
        return;
    case 6:
        rpg_girl_wait_step(0x3C);
        return;
    case 7:
        rpg_actor_set_animation(6, 0x13, 1, 1);
        rpg_se_play_latched_5xx(0x50F, 1, 0);
        rpg_girl_step_next();
        return;
    case 8:
        rpg_girl_wait_step(0x3C);
        return;
    case 9:
        rpg_actor_set_animation(6, 0x14, 1, 1);
        rpg_girl_step_next();
        return;
    case 10:
        rpg_se_play_latched_5xx(0x50E, 0xF, 1);
        rpg_girl_wait_step(0xB4);
        return;
    case 11:
        rpg_actor_set_animation(6, 0xFF, 1, 0);
        rpg_actor_set_animation(7, 0xFF, 1, 0);
        rpg_actor_set_animation(8, 0xF, 1, 1);
        rpg_apply_damage(0, 5, 0x270F, 1);
        rpg_enemy_hit_points = 0;
        rpg_girl_step_next();
        return;
    case 12:
        rpg_girl_wait_step(0x3C);
        return;
    case 13:
        rpg_alien_departure();
        return;
    case 14:
        rpg_girl_wait_step(0x3C);
        return;
    case 15:
        strcpy(rpg_text_result_0, "宇宙人は去っていった。");
        rpg_message_show(rpg_text_result_pointers, 1, 0x3C);
        rpg_actor_set_animation(0x30, 9, 1, 1);
        audio_bank_start(1, 0x202);
        rpg_girl_step_next();
        return;
    case 16:
        rpg_girl_wait_msg_step(0);
        return;
    case 17:
        rpg_girl_wait_step(0x78);
        return;
    case 18:
        rpg_actor_set_animation(0x30, 0, 1, 1);
        rpg_actor_set_animation(8, 0x15, 5, 1);
        rpg_se_play_latched_5xx(0x510, 1, 0);
        rpg_girl_step_next();
        return;
    case 19:
        rpg_girl_wait_animation_step(8);
        return;
    case 20:
        rpg_girl_wait_step(0x1E);
        return;
    case 21:
        rpg_dialog_yuina_after_aliens();
        return;
    case 22:
        rpg_girl_wait_step(0x3C);
        return;
    case 23:
        rpg_result_code = RPG_RESULT_GOOD;
        mode_stage_next();
        /* fallthrough */
    default:
        return;
    }
}

void rpg_alien_departure(void) {
    u32 shade;
    s32 count;
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
    rpg_player_action = RPG_PLAYER_DOWN;
    shade = color.word;
    switch (((RpgTransitionState *)&rpg_girl_substep)->step) {
    case 0:
        strcpy(rpg_text_enemy_0, "宇宙人「仝Щ△※〒ヰヱΞ＆");
        strcpy(rpg_text_enemy_1, "ΠΣΨБДЗЦЧ＠Ъ◇Ш");
        rpg_message_show(rpg_text_enemy_pointers, 2, 0x5A);
        rpg_se_play_latched_5xx(0x513, 1, 0);
        rpg_girl_substep_next();
    default:
        break;
    case 1:
        rpg_girl_wait_msg_substep(1);
        break;
    case 2:
        rpg_se_play_latched_5xx(0x512, 1, 0);
        rpg_girl_substep_next();
        break;
    case 3:
        rpg_girl_wait_msg_substep(0);
        break;
    case 4:
        rpg_bg_word_1 = 0x18 - ((RpgTransitionState *)&rpg_girl_substep)->frame_offset;
        count = ((RpgTransitionState *)&rpg_girl_substep)->frame_offset += 2;
        if (count >= 0x79) {
            rpg_actor_set_position(0x38, 0, 0);
            rpg_actor_set_animation(0x38, 9, 1, 1);
            D_80121531 = 2;
            rpg_actor_set_animation(0x3B, 8, 1, 1);
            rpg_girl_substep_next();
        }
        break;
    case 5:
        prim_gradient_rect(-0x5C - ((RpgTransitionState *)&rpg_girl_substep)->frame_offset, -0x78, ((RpgTransitionState *)&rpg_girl_substep)->frame_offset * 2, 0xA8, 3, shade, 0x81);
        dtd_on_tpage(0, 0, 3, 1, 0);
        count = ((RpgTransitionState *)&rpg_girl_substep)->frame_offset += 2;
        if (count >= 0x21) {
            rpg_girl_substep_next();
        }
        break;
    case 6:
        prim_gradient_rect(-0x7C, -0x78, 0x40, 0xA8, 3, shade, 0x81);
        dtd_on_tpage(0, 0, 3, 1, 0);
        rpg_se_play_latched_5xx(0x511, 0, 6);
        rpg_actor_set_position(0x3B, 0x48, 0x68 - ((RpgTransitionState *)&rpg_girl_substep)->frame_offset);
        rpg_actor_set_animation_alt(0x3B, 8, 1, 1);
        count = ((RpgTransitionState *)&rpg_girl_substep)->frame_offset += 2;
        if (count >= 0xE9) {
            k_disp_switch(rpg_label_enemy, 0);
            rpg_girl_substep_next();
        }
        break;
    case 7:
        prim_gradient_rect(((RpgTransitionState *)&rpg_girl_substep)->frame_offset - 0x7C, -0x78, 0x40 - (((RpgTransitionState *)&rpg_girl_substep)->frame_offset * 2), 0xA8, 3, shade, 0x81);
        dtd_on_tpage(0, 0, 3, 1, 0);
        count = ((RpgTransitionState *)&rpg_girl_substep)->frame_offset += 2;
        if (count >= 0x21) {
            rpg_girl_step_next();
        }
        break;
    }
}
