/* enemy_alien.c: src/ovl/RPG_BAT/80156CC0.c:9: alien actions; body 10: switch (rpg_enemy_step) {; 13: rpg_message_show(rpg_text_enemy_pointers, 1, 0x5A);; alien name-label creation src/ovl/RPG_BAT/801525B0.c:39; encounter 0x80000000 dispatch src/ovl/RPG_BAT/8013C890.c:103. Original object 0x80156CC0-0x80157AE0 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_se_play_latched_5xx /* implicit int view: the call's $v0 moves rpg_enemy_timer_a of rpg_enemy_alien_attack_2 to $v1 (T-17102) */
#define MAIN_API_OVERRIDE_prim_gradient_rect /* the original passes the coordinates as promoted words (T-17102) */
#include "common.h"
#include "ovl/rpg_battle.h"

void prim_gradient_rect();
s32 rpg_se_play_latched_5xx();

void rpg_enemy_alien_chatter(void) {
    switch (rpg_enemy_step) {
    case 0:
        strcpy(rpg_text_enemy_0, "宇宙人「Ж○ξ♂×♀？〆£∞Й！");
        rpg_message_show(rpg_text_enemy_pointers, 1, 0x5A);
        rpg_se_play_latched_5xx(0x512, 1, 0);
        rpg_enemy_step_next();
        return;
    case 1:
        rpg_enemy_wait_msg_step(0);
        return;
    case 2:
        rpg_enemy_wait_step(0x14);
        return;
    case 3:
        rpg_enemy_action_end();
        return;
    }
}

void rpg_enemy_alien_attack_1(void) {
    s32 unused; /* FAKE: unused slot above sp38; without it sp38 sits at 0x3C instead of 0x38. T-17102 */
    s32 sp38;

    if (rpg_enemy_step < 0x50) {
        if (rpg_player_action == RPG_COMMAND_GUARD) {
            rpg_player_action = RPG_PLAYER_GUARD;
        }
        rpg_player_pose_idle();
    }
    switch (rpg_enemy_step) {
    case 0x0:
        strcpy(rpg_text_enemy_0, "宇宙人「ЮΩ▽☆§◎");
        rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x3C);
        rpg_player_action |= RPG_PLAYER_DOWN;
        rpg_se_play_latched_5xx(0x514, 1, 0);
        rpg_enemy_step_next();
        return;
    case 0x1:
        rpg_enemy_wait_msg_step(0);
        return;
    case 0x2:
        rpg_actor_set_animation(0x3B, 1, 5, 1);
        rpg_player_action &= ~0x1000;
        rpg_enemy_step_next();
        return;
    case 0x3:
        rpg_enemy_wait_animation_step(0x3B);
        return;
    case 0x4:
        sp38 = rpg_enemy_timer_a;
        rpg_actor_set_animation(0x3B, 0, 1, 1);
        D_80121311 = 6;
        prim_gradient_rect(-0x26, -0x1A, sp38, 2, 5, 0xBFBF00, 0x81);
        dtd_on_tpage(0, 0, 5, 1, 0);
        rpg_enemy_timer_a += 2;
        if (rpg_enemy_timer_a < 0x72) {
            rpg_se_play_latched_5xx(0x504, 0x24, 1);
        }
        if (rpg_enemy_timer_a >= 0x97) {
            rpg_enemy_step = 0x50;
            return;
        }
        return;
    case 0x50:
        rpg_actor_set_position(0x28, 0x110, 0x58);
        rpg_actor_set_animation_alt(0x28, 5, 5, 1);
        if (rpg_player_action != RPG_PLAYER_GUARD) {
            rpg_actor_set_animation(0x30, 5, 1, 1);
        }
        rpg_se_play_latched_5xx(0x508, 1, 0);
        rpg_enemy_step_next();
        return;
    case 0x51:
        rpg_enemy_wait_animation_step(0x28);
        return;
    case 0x52:
        rpg_enemy_damage_roll();
        if (rpg_damage >= rpg_player_hit_points) {
            rpg_damage = rpg_player_hit_points - 1;
        }
        rpg_message_format_damage((s32) rpg_text_enemy_0, (s32) game_state.player_family_name, (s32) "は", rpg_damage, (s32) "のダメージを受けた");
        rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x3C);
        rpg_actor_set_animation_alt(0x28, 0xFF, 1, 0);
        rpg_enemy_step_next();
        return;
    case 0x53:
        rpg_apply_damage(0, 0x21, rpg_damage, 1);
        if (rpg_player_hit_points == 1) {
            rpg_player_action = RPG_PLAYER_DOWN;
            rpg_enemy_action &= 0xFFFF0000;
            rpg_enemy_action |= 3;
            rpg_enemy_step = 0;
            rpg_actor_set_animation(0x30, 4, 1, 1);
            rpg_enemy_step_next();
            return;
        }
        rpg_enemy_step_next();
        return;
    case 0x54:
        rpg_enemy_wait_msg_step(0);
        return;
    case 0x55:
        rpg_enemy_wait_step(0x14);
        return;
    case 0x56:
        rpg_enemy_action_end();
        break;
    }
}

void rpg_enemy_alien_attack_2(void) {
    s32 unused[2]; /* FAKE: two unused words above sp34 give the original 0x40 frame. T-17102 */
    s32 sp34;

    if (rpg_enemy_step < 0x50) {
        if (rpg_player_action == RPG_COMMAND_GUARD) {
            rpg_player_action = RPG_PLAYER_GUARD;
        }
        rpg_player_pose_idle();
    }
    if ((rpg_enemy_step >= 4) && (rpg_enemy_step < 0x51)) {
        rpg_se_play_latched_5xx(0x507, 8, 3);
    }
    switch (rpg_enemy_step) {
    case 0x0:
        strcpy(rpg_text_enemy_0, "宇宙人「ЛΦΘζ□Э℃¢Λ");
        rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x3C);
        rpg_player_action |= RPG_PLAYER_DOWN;
        rpg_se_play_latched_5xx(0x513, 1, 0);
        rpg_enemy_step_next();
        break;
    case 0x1:
        rpg_enemy_wait_msg_step(0);
        break;
    case 0x2:
        rpg_player_action &= ~0x1000;
        rpg_bg_word_0 = rpg_enemy_timer_a + 0x40;
        rpg_enemy_timer_a += 4;
        rpg_se_play_latched_5xx(0x506, 0, 2);
        if (rpg_enemy_timer_a >= 0xD1) {
            rpg_enemy_step_next();
        }
        break;
    case 0x3:
        rpg_enemy_wait_step(0x14);
        break;
    case 0x4:
        rpg_actor_set_position(0x28, 0, 0);
        ((Actor *)actor_pool)[136].quad_flags = 0x51000000;
        rpg_actor_set_animation(0x28, 3, 5, 1);
        rpg_enemy_step_next();
        break;
    case 0x5:
        rpg_enemy_wait_animation_step(0x28);
        break;
    case 0x6:
        rpg_actor_set_animation(0x28, 4, 1, 1);
        rpg_scratch = rpg_rand(4) + 3;
        if (rpg_player_action != RPG_PLAYER_GUARD) {
            rpg_actor_set_animation(0x30, 5, 1, 1);
        }
        sp34 = rpg_rand(0x10);
        rpg_actor_set_position(0x29, sp34 + 0x108, rpg_rand(0x10) + 0x50);
        rpg_actor_set_animation_alt(0x29, 5, 5, 1);
        rpg_se_play_latched_5xx(0x508, 1, 0);
        rpg_enemy_step = 0x50;
        break;
    case 0x50:
        if (!(((Actor *)actor_pool)[137].flags & 1)) {
            rpg_se_play_latched_5xx(0x508, 1, 0);
            rpg_scratch -= 1;
            if (rpg_scratch == 0) {
                ((Actor *)actor_pool)[136].quad_flags = 0x41000000;
                rpg_actor_set_animation_alt(0x28, 0xFF, 1, 0);
                rpg_actor_set_animation_alt(0x29, 0xFF, 1, 0);
                rpg_enemy_step_next();
            } else {
                sp34 = rpg_rand(0x10);
                rpg_actor_set_position(0x29, sp34 + 0x108, rpg_rand(0x10) + 0x50);
                rpg_actor_set_animation_alt(0x29, 5, 5, 1);
            }
        }
        break;
    case 0x51:
        rpg_enemy_damage_roll();
        if (rpg_damage >= rpg_player_hit_points) {
            rpg_damage = rpg_player_hit_points - 1;
        }
        rpg_message_format_damage((s32) rpg_text_enemy_0, (s32) game_state.player_family_name, (s32) "は", rpg_damage, (s32) "のダメージを受けた");
        rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x3C);
        rpg_enemy_step_next();
        break;
    case 0x52:
        rpg_apply_damage(0, 0x21, rpg_damage, 1);
        if (rpg_player_hit_points == 1) {
            rpg_player_action = RPG_PLAYER_DOWN;
            rpg_enemy_action &= 0xFFFF0000;
            rpg_enemy_action |= 3;
            rpg_enemy_step = 0;
            rpg_actor_set_animation(0x30, 4, 1, 1);
        } else {
            rpg_enemy_step_next();
        }
        break;
    case 0x53:
        rpg_enemy_wait_msg_step(0);
        break;
    case 0x54:
        rpg_bg_word_0 = 0x110 - rpg_enemy_timer_a;
        rpg_enemy_timer_a += 4;
        if (rpg_enemy_timer_a >= 0xD1) {
            rpg_enemy_step_next();
        }
        break;
    case 0x55:
        rpg_enemy_wait_step(0x14);
        break;
    case 0x56:
        rpg_enemy_action_end();
        break;
    }
}

void rpg_enemy_alien_retreat(void) {
    switch (rpg_enemy_step) {
    case 0:
        rpg_bg_word_0 = 0x110 - rpg_enemy_timer_a;
        rpg_girl_action = 0;
        rpg_enemy_timer_a += 4;
        rpg_actor_set_position(0x1D, 0, 0);
        rpg_actor_set_animation(0x1D, 0x18, 1, 1);
        if (rpg_enemy_timer_a >= 0xD1) {
            rpg_enemy_step_next();
        }
        break;
    case 1:
        rpg_girl_action = 0;
        rpg_actor_set_position(0x1D, 0, 0);
        rpg_actor_set_animation(0x1D, 0x18, 1, 1);
        rpg_enemy_step_next();
        break;
    case 2:
        rpg_enemy_wait_step(0x1E);
        break;
    case 3:
        strcpy(rpg_text_enemy_0, game_state.player_family_name);
        strcat(rpg_text_enemy_0, "達は逃げ出した。");
        rpg_message_show(rpg_text_enemy_pointers, 1, 0x5A);
        rpg_enemy_step_next();
        break;
    case 4:
        rpg_enemy_wait_msg_step(0);
        break;
    case 5:
        ((Actor *)actor_pool)[125].draw_flags = 0x81;
        rpg_actor_set_animation(0x30, 2, 1, 1);
        rpg_se_play_latched_5xx(0x503, 1, 0);
        rpg_enemy_step_next();
        break;
    case 6:
        ((Actor *)actor_pool)[125].x += 4;
        ((Actor *)actor_pool)[144].x += 4;
        rpg_enemy_timer_a += 1;
        if (rpg_enemy_timer_a >= 0x21) {
            rpg_enemy_step_next();
        }
        break;
    case 7:
        rpg_enemy_wait_step(0x3C);
        break;
    case 8:
        rpg_state.result_code = 1;
        mode_stage_next();
        break;
    }
}

void rpg_alien_scene_flicker(void) {
    rpg_actor_set_position(0x2F, rpg_bg_word_0, rpg_bg_word_1);
    rpg_actor_set_position(0x2E, rpg_bg_word_0, rpg_bg_word_1);
    rpg_actor_set_position(0x2A, rpg_bg_word_0 - 0x67, rpg_bg_word_1 - 6);
    rpg_actor_set_position(0x2B, rpg_bg_word_0 - 0x43, rpg_bg_word_1 + 6);
    rpg_actor_set_position(0x2C, rpg_bg_word_0 + 0x43, rpg_bg_word_1 + 6);
    rpg_actor_set_position(0x2D, rpg_bg_word_0 + 0x67, rpg_bg_word_1 - 6);
    if (!(D_8012117A & 1)) {
        rpg_actor_set_animation_alt(0x2A, 0xFF, 1, 0);
        rpg_bg_word_2 = 0;
    }
    if (!(D_801211BE & 1)) {
        rpg_actor_set_animation_alt(0x2B, 0xFF, 1, 0);
        rpg_bg_word_3 = 0;
    }
    if (!(D_80121202 & 1)) {
        rpg_actor_set_animation_alt(0x2C, 0xFF, 1, 0);
        rpg_bg_word_4 = 0;
    }
    if (!(D_80121246 & 1)) {
        rpg_actor_set_animation_alt(0x2D, 0xFF, 1, 0);
        rpg_bg_word_5 = 0;
    }
    if ((rpg_bg_word_2 == 0) && (rpg_rand(0x64) == 0)) {
        rpg_actor_set_animation_alt(0x2A, 7, 5, 1);
        rpg_bg_word_2 = 1;
    }
    if ((rpg_bg_word_3 == 0) && (rpg_rand(0x64) == 0)) {
        rpg_actor_set_animation_alt(0x2B, 7, 5, 1);
        rpg_bg_word_3 = 1;
    }
    if ((rpg_bg_word_4 == 0) && (rpg_rand(0x64) == 0)) {
        rpg_actor_set_animation_alt(0x2C, 7, 5, 1);
        rpg_bg_word_4 = 1;
    }
    if (rpg_bg_word_5 == 0) {
        if (rpg_rand(0x64) == 0) {
            rpg_actor_set_animation_alt(0x2D, 7, 5, 1);
            rpg_bg_word_5 = 1;
        }
    }
}
