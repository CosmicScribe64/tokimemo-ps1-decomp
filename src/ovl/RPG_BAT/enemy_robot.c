/* enemy_robot.c: src/ovl/RPG_BAT/8013F4F0.c:203: robot encounter (girl 2's robot); body 204: switch (rpg_enemy_step) {; 206: rpg_enemy_damage_roll();; robot name-label creation src/ovl/RPG_BAT/labels_intro_and_wave_transition.c:44; encounter 0x8000000 dispatch src/ovl/RPG_BAT/8013C890.c:93. Original object 0x8013F4F0-0x80141060 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_actor_set_animation /* implicit int view: the call's $v0 moves the 0x10-scaled value of rpg_victory_robot to $v1 (T-17114) */
#define MAIN_API_OVERRIDE_D_8012077C /* matched as s16 (main_api.h: s32, EVENT stores a pointer) */
#include "common.h"
#include "ovl/RPG_BAT.h"

extern s16 D_8012077C;
s32 rpg_actor_set_animation();

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_yuina_calls_robot(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_enemy_0, "紐緒「真・世界征服ロボ！！");
        rpg_message_show(rpg_text_enemy_pointers, 1, 0xF0);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1A, 0x05000026, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(0);
        return;
    case 3:
        rpg_voice_reset();
        rpg_enemy_step_next();
        return;
    }
}

/* returns int without a value, as rpg_dialog_yuina_calls_robot (T-8080) */
s32 rpg_dialog_robot_attack_1(void) {
    if (rpg_dialog_step[0] != 3) {
        switch (rpg_dialog_step[0]) {
        case 0:
            strcpy(rpg_text_enemy_0, "　　　　　服従ドリルパンチ　　　　　");
            rpg_message_show(rpg_text_enemy_pointers, 1, 0xC8);
            rpg_dialog_step[0] += 1;
            break;
        case 1:
            rpg_voice_play_once(0x1C000020, 0x05000029, 1);
            rpg_dialog_step[0] += 1;
            break;
        case 2:
            rpg_voice_wait(0);
            break;
        case 3:
            break;
        }
    }
}

s32 rpg_dialog_robot_attack_2(void) {
    if (rpg_dialog_step[0] != 3) {
        switch (rpg_dialog_step[0]) {
        case 0:
            strcpy(rpg_text_enemy_0, "　　　　独裁ミサイルシャワー　　　　");
            rpg_message_show(rpg_text_enemy_pointers, 1, 0x118);
            rpg_dialog_step[0] += 1;
            break;
        case 1:
            rpg_voice_play_once(0x1B000020, 0x0300002F, 2);
            rpg_dialog_step[0] += 1;
            break;
        case 2:
            rpg_voice_wait(0);
            break;
        case 3:
            break;
        }
    }
}

s32 rpg_dialog_robot_attack_3(void) {
    if (rpg_dialog_step[0] != 3) {
        switch (rpg_dialog_step[0]) {
        case 0:
            strcpy(rpg_text_enemy_0, "　　　　世界征服メガビーム　　　　　");
            rpg_message_show(rpg_text_enemy_pointers, 1, 0xAA);
            rpg_dialog_step[0] += 1;
            break;
        case 1:
            rpg_voice_play_once(0x1C00002A, 0x06000036, 0);
            rpg_dialog_step[0] += 1;
            break;
        case 2:
            rpg_voice_wait(0);
            break;
        case 3:
            break;
        }
    }
}

void rpg_victory_robot(void) {
    s32 i;
    s32 x;
    s16 h;

    switch (rpg_enemy_step) {
    case 0:
        strcpy(rpg_text_enemy_0, "ロボ「ガギグ・・・ッ");
        rpg_message_show(rpg_text_enemy_pointers, 1, 0x5A);
        rpg_effects_select_asset_set(4);
        rpg_player_action = 0x1000;
        rpg_se_play_latched_5xx(0x510, 1, 0);
        rpg_enemy_step_next();
        break;
    case 1:
        rpg_enemy_wait_msg_step(1);
        break;
    case 2:
        rpg_enemy_timer_a += 1;
        rpg_result_line_count += 1;
        switch (rpg_enemy_timer_a) {
        case 1:
            load_palette(rpg_asset_palette_6, 0x13, 1, 4, 0xA);
            D_8012163F = 0x13;
            break;
        case 10:
            load_palette(rpg_asset_palette_6, 0x13, 1, 4, 9);
            break;
        case 20:
            load_palette(rpg_asset_palette_6, 0x13, 1, 4, 8);
            break;
        case 30:
            load_palette(rpg_asset_palette_6, 0x13, 1, 4, 7);
            break;
        case 40:
            load_palette(rpg_asset_palette_6, 0x13, 1, 4, 6);
            break;
        case 50:
            load_palette(rpg_asset_palette_6, 0x13, 1, 4, 5);
            break;
        }
        /* FAKE: the slot table rpg_enemy_slot (rpg_scratch is its element 50) is reached through its first symbol; the aggregate is not recovered yet. T-17114 */
        for (i = 0; i < 8; i++) {
            if (rpg_result_line_count >= 2 && (&rpg_scratch)[i] == 0 && rpg_rand(5) == 0) {
                rpg_result_line_count = 0;
                x = rpg_rand(0x40);
                rpg_actor_set_position(i + 5, x + 0x28, rpg_rand(0x40) + 0x40);
                rpg_actor_set_animation_alt(i + 5, 0x17, 5, 1);
                if (rpg_enemy_timer_b < 0xB0) {
                    rpg_se_play_latched_5xx(0x50D, 2, 1);
                }
            }
            if (!(((Actor *)actor_pool)[i + 101].flags & 1)) {
                rpg_actor_set_animation_alt(i + 5, 0xFF, 1, 0);
                (&rpg_enemy_slot)[i + 50] = 0;
            }
        }
        if (rpg_enemy_timer_a >= 0x3D) {
            rpg_enemy_timer_b += 2;
            rpg_actor_set_position(4, 0, 0);
            rpg_actor_set_animation(4, 0x18, 1, 1);
            D_80120778 = 5;
            h = rpg_enemy_timer_b * 0x10;
            D_8012077A = h;
            D_8012077C = h;
            /* grey level c * 0x101 * 0x101 as two steps c + (c << 8): uopt keeps D / 3 and the inner sum in $t0 and $t1 (a plain * 0x10201 is one shift/add chain in ugen temporaries). T-17114 */
            prim_gradient_rect(-0xA0, -0x78, 0x140, 0xA8, 2, (((rpg_enemy_timer_b / 3) + ((rpg_enemy_timer_b / 3) << 8)) + (((rpg_enemy_timer_b / 3) + ((rpg_enemy_timer_b / 3) << 8)) << 8)), 0x81);
            dtd_on_tpage(0, 0, 2, 1, 0);
        }
        if (rpg_enemy_timer_b == 0xB0) {
            rpg_se_play_latched_5xx(0x50C, 1, 0);
        }
        if (rpg_enemy_timer_b >= 0x151) {
            for (i = 0; i != 8; i++) {
                rpg_actor_set_animation_alt(i + 5, 0xFF, 1, 0);
                rpg_actor_set_animation_alt(0x3B, 0xFF, 1, 0);
                D_80120778 = 5;
            }
            k_disp_switch(rpg_label_enemy, 0);
            k_disp_switch(rpg_label_guest, 0);
            rpg_enemy_step_next();
        }
        break;
    case 3:
        rpg_enemy_timer_b += 4;
        rpg_actor_set_animation(4, 0x18, 1, 1);
        D_80120778 = 5;
        x = 0x150 - rpg_enemy_timer_b;
        h = x * 0x10;
        D_8012077A = h;
        D_8012077C = h;
        /* same two-step grey level as in case 2 */
        prim_gradient_rect(-0xA0, -0x78, 0x140, 0xA8, 2, (((x / 3) + ((x / 3) << 8)) + (((x / 3) + ((x / 3) << 8)) << 8)), 0x81);
        dtd_on_tpage(0, 0, 2, 1, 0);
        if (rpg_enemy_timer_b >= 0x149) {
            rpg_actor_set_animation_alt(4, 0xFF, 1, 0);
            rpg_enemy_step_next();
        }
        break;
    case 4:
        rpg_enemy_wait_step(0x3C);
        break;
    case 5:
        mode_stage_next();
        break;
    }
}

void rpg_enemy_robot_attack(void) {
    switch (rpg_enemy_step) {
    case 0:
        rpg_enemy_damage_roll();
        D_80121311 = 6;
        rpg_effects_select_asset_set(4);
        rpg_enemy_step_next();
        break;
    case 1:
        rpg_dialog_robot_attack_1();
        rpg_robot_attack_1_motion();
        break;
    case 2:
        rpg_enemy_wait_step(0x3C);
        break;
    case 3:
        rpg_voice_reset();
        rpg_enemy_step_next();
        break;
    case 4:
        rpg_dialog_robot_attack_2();
        rpg_robot_attack_2_motion();
        break;
    case 5:
        rpg_voice_reset();
        rpg_enemy_step_next();
        break;
    case 6:
        rpg_dialog_robot_attack_3();
        rpg_robot_attack_3_motion();
        break;
    case 7:
        rpg_voice_reset();
        rpg_enemy_step_next();
        break;
    case 8:
        rpg_apply_damage(0, 0x21, rpg_damage, 1);
        rpg_enemy_step_next();
        break;
    case 9:
        rpg_enemy_wait_step(0x1E);
        break;
    case 10:
        rpg_player_pose_idle();
    case 11:
        rpg_enemy_wait_step(0x14);
        break;
    case 12:
        if (rpg_player_action & 0x1000) {
            rpg_player_action = rpg_player_action & ~0x1000;
            rpg_player_ready_timer = 0xFFFF;
        }
        rpg_effects_select_asset_set(0xFF);
        rpg_enemy_action_end();
        break;
    }
}

void rpg_robot_attack_1_motion(void) {
    /* FAKE: case 6 on one line retains the global-store scheduling. T-9750. */
    s32 temp_v0;

    switch (rpg_enemy_substep) {
    case 0:
        rpg_actor_set_animation_alt(0x3B, 4, 1, 1);
        rpg_enemy_substep_next();

    default:
        break;

    case 1:
        rpg_enemy_wait_substep(7);
        break;

    case 2:
        rpg_actor_set_animation_alt(0x3B, 3, 1, 1);
        rpg_actor_set_position(0x3A, 0x70, 0x48);
        rpg_actor_set_animation_alt(0x3A, 5, 5, 1);
        rpg_enemy_substep_next();
        break;

    case 3:
        rpg_enemy_wait_animation_substep(0x3A);
        break;

    case 4:
        rpg_actor_set_animation_alt(0x3A, 6, 1, 1);
        rpg_se_play_latched_5xx(0x505, 1, 0);
        rpg_enemy_substep_next();
        break;

    case 5:
        rpg_actor_set_position(0x3A, rpg_enemy_timer_b + 0x70, 0x48);
        rpg_enemy_timer_b += 8;
        if ((rpg_enemy_timer_b >= 0x91) && (!(rpg_player_action & 0x100))) {
            rpg_actor_set_animation(0x30, 0xA, 1, 1);
        }
        if (rpg_enemy_timer_b >= 0x129) {
            rpg_actor_set_animation_alt(0x3A, 7, 1, 1);
            rpg_enemy_substep_next();
        }
        break;

    case 6: rpg_actor_set_position(0x3A, 0x198 - rpg_enemy_timer_b, 0x48); temp_v0 = rpg_enemy_timer_b + 8; rpg_enemy_timer_b = temp_v0; if ((0x198 - temp_v0) < 0x70) { rpg_player_pose_idle(); rpg_enemy_substep_next(); } break;

    case 7:
        rpg_actor_set_position(0x39, 0x48, 0x60);
        rpg_actor_set_animation_alt(0x39, 8, 1, 1);
        rpg_enemy_substep_next();
        break;

    case 8:
        rpg_enemy_wait_substep(0xB);
        break;

    case 9:
        rpg_actor_set_animation_alt(0x3B, 4, 1, 1);
        rpg_actor_set_animation_alt(0x3A, 0xFF, 1, 0);
        rpg_actor_set_animation(0x39, 0xFF, 1, 0);
        rpg_enemy_substep_next();
        break;

    case 10:
        rpg_enemy_wait_substep(7);
        break;

    case 11:
        rpg_actor_set_animation_alt(0x3B, 2, 1, 1);
        rpg_enemy_step_next();
        break;
    }
}

void rpg_robot_missile_step(s32 i) {
    u8 *p = actor_pool + i * 0x44;
    *(s16 *)(p + 0x1AB6) += 1;
    *(s16 *)(p + 0x1ABA) -= 1;
}

/* returns int without a value, as rpg_dialog_yuina_calls_robot: its call above is a statement call of an implicit int function (T-17114) */
s32 rpg_robot_attack_2_motion(void) {
    s32 i;
    s32 *p;

    switch (rpg_enemy_substep) {
    case 0:
        rpg_actor_set_animation_alt(0x3B, 0xA, 5, 1);
        rpg_se_play_latched_5xx(0x506, 1, 0);
        rpg_enemy_substep_next();
        break;
    case 1:
        rpg_enemy_wait_animation_substep(0x3B);
        break;
    case 2:
        D_801215FE = 0x40;
        D_80121614 = 0x19;
        rpg_actor_set_position(0x39, 0x60, 0x78);
        rpg_actor_set_animation_alt(0x39, 0xC, 1, 1);
        rpg_actor_set_position(0x38, 0, 0);
        rpg_actor_set_animation(0x38, 9, 5, 1);
        rpg_enemy_substep_next();
        break;
    case 3:
        rpg_actor_set_position(0x39, rpg_enemy_timer_b + 0x62, 0x76 - rpg_enemy_timer_b);
        rpg_enemy_timer_b += 2;
        if (!(D_80121532 & 1)) {
            rpg_actor_set_animation(0x38, 0xFF, 1, 0);
        }
        if (rpg_enemy_timer_b + 0x5A >= 0x77) {
            rpg_actor_set_animation(0x38, 0xFF, 1, 0);
            rpg_actor_set_animation_alt(0x39, 0xD, 5, 1);
            D_80121575 = 2;
            rpg_enemy_substep_next();
        }
        break;
    case 4:
        for (i = 0; i != 5; i++) {
            rpg_actor_set_position(i + 4, 0x80, 0x58);
            rpg_actor_set_animation_alt(i + 4, 0x14, 1, 1);
        }
        rpg_enemy_substep_next();
        break;
    case 5:
        if (rpg_enemy_timer_b % 2 == 0) {
            ((Actor *)actor_pool)[101].x -= 1;
            ((Actor *)actor_pool)[103].x -= 2;
            ((Actor *)actor_pool)[102].y += 1;
            ((Actor *)actor_pool)[104].y += 2;
        }
        if (rpg_enemy_timer_b % 2 == 0) {
            for (i = 0; i < 5; i++) {
                ((Actor *)actor_pool)[i + 100].x += 1;
                ((Actor *)actor_pool)[i + 100].y -= 1;
            }
        }
        rpg_enemy_timer_b += 1;
        if (rpg_enemy_timer_b >= 0x21) {
            rpg_enemy_substep_next();
        }
        break;
    case 6:
        if (rpg_enemy_timer_b >= 0x1B) {
            rpg_se_play_latched_5xx(0x507, 0, 1);
            ((Actor *)actor_pool)[100].x += 3;
            ((Actor *)actor_pool)[100].y -= 3;
        } else if (!(rpg_enemy_timer_b & 1)) {
            rpg_robot_missile_step(0);
        }
        if (rpg_enemy_timer_b >= 0xE) {
            rpg_se_play_latched_5xx(0x507, 0, 2);
            ((Actor *)actor_pool)[101].x += 3;
            ((Actor *)actor_pool)[101].y -= 3;
        } else if (!(rpg_enemy_timer_b & 1)) {
            rpg_robot_missile_step(1);
        }
        if (rpg_enemy_timer_b >= 0xE) {
            ((Actor *)actor_pool)[102].x += 3;
            ((Actor *)actor_pool)[102].y -= 3;
        } else if (!(rpg_enemy_timer_b & 1)) {
            rpg_robot_missile_step(2);
        }
        if (rpg_enemy_timer_b > 0) {
            rpg_se_play_latched_5xx(0x507, 0, 3);
            ((Actor *)actor_pool)[103].x += 3;
            ((Actor *)actor_pool)[103].y -= 3;
        }
        if (rpg_enemy_timer_b > 0) {
            ((Actor *)actor_pool)[104].x += 3;
            ((Actor *)actor_pool)[104].y -= 3;
        }
        rpg_enemy_timer_b += 1;
        if (rpg_enemy_timer_b >= 0x51) {
            rpg_enemy_substep_next();
        }
        break;
    case 7:
        rpg_enemy_wait_substep(0x1E);
        break;
    case 8:
        rpg_enemy_timer_b += 1;
        rpg_result_line_count += 1;
        /* FAKE: p[50] walks the slot table rpg_enemy_slot through its first symbol (element 50 on); the aggregate is not recovered yet. T-17114 */
        p = &rpg_enemy_slot;
        for (i = 0; i != 0xC; i++, p++) {
            if (p[50] == 3 && !(rpg_player_action & 0x100)) {
                rpg_actor_set_animation(0x30, 5, 1, 1);
            }
            if (rpg_enemy_timer_b < 0x78 && rpg_result_line_count >= 3 && p[50] == 0 && rpg_rand(5) == 0) {
                rpg_se_play_latched_5xx(0x508, 1, 0);
                rpg_result_line_count = 0;
                if (rpg_rand(0x64) < 0x1E) {
                    p[50] = 1;
                    rpg_actor_set_position(i + 4, rpg_rand(0x18) + 0x104, 0);
                    rpg_actor_set_animation_alt(i + 4, 0x16, 1, 1);
                } else {
                    p[50] = 2;
                    rpg_actor_set_position(i + 4, rpg_rand(0x40) + 0xF0, 0);
                    rpg_actor_set_animation_alt(i + 4, 0x16, 1, 1);
                }
            }
            if (p[50] == 1) {
                ((Actor *)actor_pool)[i + 100].y += 5;
                if (rpg_rand(4) - 0x22 < ((Actor *)actor_pool)[i + 100].y) {
                    rpg_actor_set_animation_alt(i + 4, 0xE, 5, 1);
                    p[50] = 3;
                    rpg_se_play_latched_5xx(0x509, 1, 0);
                }
            }
            if (p[50] == 2) {
                ((Actor *)actor_pool)[i + 100].y += 5;
                if (rpg_rand(4) + 8 < ((Actor *)actor_pool)[i + 100].y) {
                    rpg_actor_set_animation_alt(i + 4, 0xE, 5, 1);
                    p[50] = 3;
                    rpg_se_play_latched_5xx(0x509, 1, 0);
                }
            }
            if (p[50] == 3 && !(((Actor *)actor_pool)[i + 100].flags & 1)) {
                rpg_actor_set_animation_alt(i + 4, 0xFF, 1, 0);
                p[50] = 0;
            }
        }
        if (rpg_enemy_timer_b >= 0xDD) {
            rpg_player_pose_idle();
            rpg_enemy_substep_next();
        }
        break;
    case 9:
        rpg_actor_set_animation_alt(0x3B, 0xB, 5, 1);
        for (i = 0; i != 0xC; i++) {
            rpg_actor_set_animation_alt(i + 4, 0xFF, 1, 0);
        }
        rpg_enemy_substep_next();
        break;
    case 10:
        rpg_enemy_wait_animation_substep(0x3B);
        break;
    case 11:
        rpg_enemy_step_next();
        break;
    }
}

/* T-14501: retain the earlier unprototyped callers' implicit s32 view. */
s32 rpg_robot_attack_3_motion(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (rpg_enemy_substep) {
    case 0:
        rpg_actor_set_position(0x3A, -8, 0);
        rpg_actor_set_animation(0x3A, 0xF, 5, 1);
        rpg_se_play_latched_5xx(0x50A, 1, 0);
        rpg_enemy_substep_next();
        return;
    case 1:
        if (!(D_801215BA & 1)) {
            rpg_actor_set_animation(0x3A, 0x10, 5, 1);
            rpg_se_play_latched_5xx(0x50A, 1, 0);
            rpg_enemy_substep_next();
            return;
        }
    default:
        return;
    case 2:
        if (!(D_801215BA & 1)) {
            rpg_se_play_latched_5xx(0x50A, 1, 0);
            rpg_actor_set_animation(0x3A, 0x10, 5, 1);
            temp_v0 = rpg_enemy_timer_b + 1;
            rpg_enemy_timer_b = temp_v0;
            if (temp_v0 >= 4) {
                rpg_actor_set_animation(0x3A, 0x10, 1, 1);
                rpg_actor_set_animation_alt(0x3B, 0x12, 5, 1);
                rpg_enemy_substep_next();
                return;
            }
        }
        break;
    case 3:
        rpg_enemy_wait_animation_substep(0x3B);
        return;
    case 4:
        rpg_actor_set_animation(0x3A, 0x11, 5, 1);
        if (!(rpg_player_action & 0x100)) {
            rpg_actor_set_animation(0x30, 0xA, 1, 1);
        }
        rpg_se_play_latched_5xx(0x50B, 1, 0);
        rpg_enemy_substep_next();
        return;
    case 5:
        if (!(D_801215BA & 1)) {
            rpg_se_play_latched_5xx(0x50B, 1, 0);
            rpg_actor_set_animation(0x3A, 0x11, 5, 1);
            temp_v0_2 = rpg_enemy_timer_b + 1;
            rpg_enemy_timer_b = temp_v0_2;
            if (temp_v0_2 >= 9) {
                rpg_actor_set_animation(0x3A, 0xFF, 1, 0);
                rpg_actor_set_animation_alt(0x3B, 0x13, 5, 1);
                rpg_enemy_substep_next();
                return;
            }
        }
        break;
    case 6:
        rpg_enemy_wait_animation_substep(0x3B);
        return;
    case 7:
        rpg_actor_set_animation_alt(0x3B, 2, 1, 1);
        rpg_enemy_step_next();
        break;
    }
}

void rpg_enemy_robot_intro(void) {
    s32 temp_v0;

    switch (rpg_enemy_step) {
    case 0:
        strcpy(rpg_text_enemy_0, game_state.player_family_name);
        strcat(rpg_text_enemy_0, "「よせっ！　紐緒さん！");
        rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x5A);
        rpg_enemy_step_next();
        return;
    case 1:
        rpg_enemy_wait_msg_step(0);
        return;
    case 2:
        rpg_actor_set_animation_alt(0x3B, 1, 1, 1);
        rpg_enemy_step_next();
        return;
    case 3:
        rpg_dialog_yuina_calls_robot();
        return;
    case 4:
        rpg_actor_set_animation_alt(0x3B, 0, 1, 1);
        rpg_enemy_step_next();
        return;
    case 5:
        rpg_enemy_wait_step(0x1E);
        return;
    case 6:
        rpg_actor_set_position(0x3B, 0x48 - rpg_enemy_timer_a, 0x70);
        temp_v0 = rpg_enemy_timer_a + 0x10;
        rpg_enemy_timer_a = temp_v0;
        if (temp_v0 >= 0x61) {
            k_disp_switch(rpg_label_girl, 0);
            rpg_enemy_step_next();
            return;
        }
    default:
        return;
    case 7:
        rpg_enemy_wait_step(0x1E);
        return;
    case 8:
        rpg_robot_palette_cycle();
        return;
    case 9:
        rpg_enemy_action_end();
        k_disp_switch(rpg_label_enemy, 1);
        rpg_enemy_action = 0x08000001;
        break;
    }
}

void rpg_robot_palette_cycle(void) {
    switch (rpg_enemy_substep) {
    case 0:
        rpg_actor_set_position(0x3B, 0x48, 0x60);
        rpg_actor_set_animation_alt(0x3B, 2, 1, 1);
        load_palette(rpg_asset_palette_6, 0x12, 1, 4, 0);
        rpg_enemy_substep_next();
        break;
    case 1:
        rpg_enemy_timer_b++;
        if (rpg_enemy_timer_b == 0x1E) {
            rpg_se_play_latched_5xx(0x504, 1, 0);
        }
        if (!(rpg_enemy_timer_b & 3)) {
            load_palette(rpg_asset_palette_6, 0x12, 1, 4, (rpg_enemy_timer_b / 4) % 5);
        }
        if (rpg_enemy_timer_b >= 0x4F) {
            rpg_enemy_substep_next();
        }
        break;
    case 2:
        rpg_enemy_timer_b++;
        if (!(rpg_enemy_timer_b & 3)) {
            load_palette(rpg_asset_palette_6, 0x12, 1, 4, (rpg_enemy_timer_b / 4) % 6 + 5);
        }
        if (rpg_enemy_timer_b >= 0x17) {
            rpg_enemy_substep_next();
        }
        break;
    case 3:
        load_palette(rpg_asset_palette_1, 0x12, 1, 4, 4);
        rpg_enemy_step_next();
        break;
    }
}
