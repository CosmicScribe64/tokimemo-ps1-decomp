/* enemy_bancho.c: src/ovl/RPG_BAT/80157AE0.c:462: gang boss action, defeat, dialog and reward sequences; body 463: switch (rpg_enemy_step) {; 465: if (rpg_rand(0xA) == 0) {; gang-boss name-label creation src/ovl/RPG_BAT/801525B0.c:35; boss action masks dispatched src/ovl/RPG_BAT/8013C890.c:72. Original object 0x80157AE0-0x80159D50 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/rpg_battle.h"

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_bancho_shout(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_enemy_0, "番長「袖竜！");
        rpg_message_show(rpg_text_enemy_pointers, 1, 0x78);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x180000A6, 0x0B0000B0, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_reset();
        rpg_enemy_step_next();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_bancho_resolve_1(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_enemy_0, "番長「男の意地にかけて、");
        strcpy(rpg_text_enemy_1, "負ける訳にはいかねぇ！");
        rpg_message_show(rpg_text_enemy_pointers, 2, 0xDC);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1E000083, 0x1A00008B, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x1E000094, 0x1C00009E, 1);
        rpg_dialog_step[0] += 1;
        return;
    case 4:
        rpg_voice_wait(0);
        return;
    case 5:
        rpg_voice_reset();
        rpg_enemy_step_next();
        return;
    }
}

s32 rpg_dialog_bancho_grunt_random(void) {
    switch (*rpg_dialog_step) {
    case 0:
        rpg_voice_playing_seen = rpg_rand(6) ^ 0; /* FAKE: the original copies the call result into the switch temporary ($v1), source unknown. T-9370 */
        switch (rpg_voice_playing_seen) {
        case 0:
            rpg_voice_play_once(0x180000B1, 0x0C0000B7, 0);
            break;
        case 1:
            rpg_voice_play_once(0x1200009B, 0x050000A4, 0);
            break;
        case 2:
            rpg_voice_play_once(0x1D0000AF, 0x200000B7, 0);
            break;
        case 3:
            rpg_voice_play_once(0x0300006B, 0x0B00006E, 0);
            break;
        case 4:
            rpg_voice_play_once(0x08000028, 0x06000030, 0);
            break;
        case 5:
            rpg_voice_play_once(0x150000AC, 0x0B0000B7, 0);
            break;
        }
        *rpg_dialog_step += 1;
        break;
    case 1:
        rpg_voice_reset();
        rpg_enemy_step_next();
        break;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_bancho_scorn(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_enemy_0, "番長「ふっ！　情けない。");
        strcpy(rpg_text_enemy_1, "俺様に楯突こうとは十年早いわ！");
        rpg_message_show(rpg_text_enemy_pointers, 2, 0x118);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1F000099, 0x1C0000A0, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x08000019, 0x05000027, 1);
        rpg_dialog_step[0] += 1;
        return;
    case 4:
        rpg_voice_wait(0);
        return;
    case 5:
        rpg_voice_reset();
        rpg_enemy_step_next();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_bancho_defeated(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_result_0, "番長「ふっ、恐れ入ったぜっ");
        strcpy(rpg_text_result_1, "俺様を倒せるやつが、");
        strcpy(rpg_text_result_2, "この世にいたとはな・・・。");
        rpg_message_show(rpg_text_result_pointers, 3, 0xC8);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1C00007A, 0x0C000083, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x0300006F, 0x0C000076, 1);
        rpg_dialog_step[0] += 1;
        return;
    case 4:
        rpg_voice_wait(2);
        return;
    case 5:
        rpg_voice_play_once(0x160000A5, 0x080000AC, 2);
        rpg_dialog_step[0] += 1;
        return;
    case 6:
        rpg_voice_wait(0);
        return;
    case 7:
        rpg_voice_reset();
        rpg_finale_step_next();
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_bancho_hands_over_title(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_result_0, "番長「いや、");
        strcpy(rpg_text_result_1, "その名はもう俺にはふさわしくない。");
        strcpy(rpg_text_result_2, "これからは、お前が番長だ。");
        rpg_message_show(rpg_text_result_pointers, 3, 0xE6);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x03000077, 0x0D000078, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x160000AD, 0x090000B7, 1);
        rpg_dialog_step[0] += 1;
        return;
    case 4:
        rpg_voice_wait(2);
        return;
    case 5:
        rpg_voice_play_once(0x140000AC, 0x0A0000B7, 2);
        rpg_dialog_step[0] += 1;
        return;
    case 6:
        rpg_voice_wait(0);
        return;
    case 7:
        rpg_voice_reset();
        rpg_finale_step_next();
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_ex_bancho_farewell(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_result_0, "元・番長「あばよ、彼女と仲良くなっ！");
        rpg_message_show(rpg_text_result_pointers, 1, 0x104);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x0800000A, 0x04000018, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(0);
        return;
    case 3:
        rpg_voice_reset();
        rpg_finale_step_next();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_bancho_taunt_sixth_5(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_enemy_0, "番長「貴様の力はそんなもんか！");
        rpg_message_show(rpg_text_enemy_pointers, 1, 0xC8);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1E0000A5, 0x1E0000AF, 0);
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

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_bancho_taunt_sixth_3(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_enemy_0, "番長「片腹痛いわ！");
        rpg_message_show(rpg_text_enemy_pointers, 1, 0xB4);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x04000021, 0x05000028, 0);
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

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_bancho_taunt_sixth_2(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_enemy_0, "番長「なかなかやるな！");
        rpg_message_show(rpg_text_enemy_pointers, 1, 0x96);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1700009B, 0x090000A1, 0);
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

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_bancho_taunt_sixth_1(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_enemy_0, "番長「少しはできそうだな。");
        rpg_message_show(rpg_text_enemy_pointers, 1, 0xB4);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1E00007A, 0x19000082, 0);
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

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_bancho_resolve_2(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_enemy_0, "番長「男の意地にかけて、");
        strcpy(rpg_text_enemy_1, "負ける訳にはいかねぇ！");
        rpg_message_show(rpg_text_enemy_pointers, 2, 0xE6);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1E000083, 0x1A00008B, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x1E000094, 0x1C00009E, 1);
        rpg_dialog_step[0] += 1;
        return;
    case 4:
        rpg_voice_wait(0);
        return;
    case 5:
        rpg_voice_reset();
        rpg_enemy_step_next();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_bancho_last_stand(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_enemy_0, "番長「俺様が・・・");
        strcpy(rpg_text_enemy_1, "番長のこの俺様が・・・");
        strcpy(rpg_text_enemy_2, "こんな所で負ける訳にはいかん！");
        rpg_message_show(rpg_text_enemy_pointers, 3, 0xFA);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x09000001, 0x02000006, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x10000099, 0x070000A2, 1);
        rpg_dialog_step[0] += 1;
        return;
    case 4:
        rpg_voice_wait(2);
        return;
    case 5:
        rpg_voice_play_once(0x05000036, 0x06000044, 2);
        rpg_dialog_step[0] += 1;
        return;
    case 6:
        rpg_voice_wait(0);
        return;
    case 7:
        rpg_voice_reset();
        rpg_enemy_step_next();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_bancho_groan(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_enemy_0, "番長「うぬぬぬぬ・・・");
        rpg_message_show(rpg_text_enemy_pointers, 1, 0x96);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1E0000B0, 0x1F0000B7, 0);
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

void rpg_enemy_bancho_taunt(void) {
    switch (rpg_enemy_step) {
    case 0:
        rpg_scratch = rpg_rand(2);
        rpg_enemy_step_next();
        return;
    case 1:
        switch (rpg_enemy_hit_points * 6 / rpg_enemy_hit_points_maximum) {
        case 0:
            if (rpg_scratch == 0) {
                rpg_dialog_bancho_last_stand();
                return;
            }
            rpg_dialog_bancho_groan();
            return;
        case 1:
            rpg_dialog_bancho_resolve_2();
            return;
        case 2:
            rpg_dialog_bancho_taunt_sixth_1();
            return;
        case 3:
            rpg_dialog_bancho_taunt_sixth_2();
            return;
        case 4:
            rpg_dialog_bancho_taunt_sixth_3();
            return;
        case 5:
            rpg_dialog_bancho_taunt_sixth_5();
            return;
        case 6:
            rpg_dialog_bancho_taunt_sixth_5();
            return;
        default:
            return;
        }
        break;
    case 2:
        rpg_enemy_wait_step(0x14);
        return;
    case 3:
        rpg_enemy_action_end();
        break;
    }
}

void rpg_bancho_grant_command(void) {
    switch (rpg_enemy_substep) {
    case 0:
        rpg_move_timer = rpg_player_ready_delay(0) - rpg_player_ready_timer;
        if (rpg_move_timer <= 0) {
            rpg_move_timer = 1;
        }
        rpg_enemy_substep_next();
        return;
    case 1:
        rpg_enemy_wait_substep(rpg_move_timer);
        return;
    case 2:
        rpg_player_action |= RPG_PLAYER_READY;
        rpg_player_ready_timer = 0;
        rpg_enemy_substep_next();
        return;
    case 3:
        rpg_enemy_wait_substep(0x5A);
        return;
    case 4:
        rpg_enemy_step_next();
        return;
    }
}

void rpg_enemy_bancho_attack(void) {
    switch (rpg_enemy_step) {
    case 0:
        if (rpg_rand(0xA) == 0) {
            if (rpg_player_action == 0) {
                rpg_enemy_step_next();
                return;
            }
            rpg_enemy_step = 2;
            return;
        }
        rpg_enemy_step = 2;
        return;
    case 1:
        rpg_bancho_grant_command();
        return;
    case 2:
        rpg_dialog_bancho_shout();
        return;
    case 3:
        rpg_enemy_wait_step(0x3C);
        return;
    case 4:
        if (rpg_player_action == 0) {
            rpg_player_action |= RPG_PLAYER_READY;
            rpg_player_ready_timer = 0;
        }
        ((Actor *)actor_pool)[154].quad_flags = 0x51000000;
        ((Actor *)actor_pool)[154].unk_01 = 4;
        ((Actor *)actor_pool)[125].unk_01 = 4;
        rpg_se_play_latched_5xx(0x50C, 1, 0);
        rpg_enemy_step_next();
        return;
    case 5:
        if (rpg_scratch * 4 < 0x40) {
            rpg_actor_set_position(0x3A, rpg_scratch * 4 + 0x60, 0x50);
            rpg_actor_set_animation_alt(0x3A, 4, 1, 1);
        } else {
            rpg_actor_set_position(0x3A, rpg_scratch * 4 + 0x60, 0x38);
            rpg_actor_set_animation_alt(0x3A, 5, 1, 1);
        }
        if (rpg_scratch % 4 < 2) {
            ((Actor *)actor_pool)[154].unk_43 = 0x13;
        } else {
            ((Actor *)actor_pool)[154].unk_43 = 0x12;
        }
        rpg_scratch += 1;
        if (rpg_scratch * 4 + 0x60 >= 0x109) {
            rpg_actor_set_position(0x3A, 0, -8);
            rpg_actor_set_animation(0x3A, 5, 1, 1);
            rpg_se_play_latched_5xx(0x50D, 1, 0);
            rpg_enemy_step_next();
            return;
        }
    default:
        return;
    case 6:
        if (rpg_scratch % 4 < 2) {
            ((Actor *)actor_pool)[154].unk_43 = 0x13;
        } else {
            ((Actor *)actor_pool)[154].unk_43 = 0x12;
        }
        rpg_scratch += 1;
        if ((rpg_scratch * 4 + 0x60 < 0x200) && (rpg_player_action == RPG_COMMAND_GUARD)) {
            rpg_player_action = RPG_PLAYER_GUARD;
        }
        if (rpg_scratch * 4 + 0x60 >= 0x281) {
            if (rpg_player_action != RPG_PLAYER_GUARD) {
                rpg_actor_set_animation(0x30, 5, 1, 1);
            }
            rpg_enemy_step_next();
            return;
        }
        break;
    case 7:
        rpg_actor_set_animation_alt(0x3A, 0xFF, 1, 0);
        ((Actor *)actor_pool)[154].quad_flags = 0x41000000;
        ((Actor *)actor_pool)[154].unk_43 = 0x12;
        rpg_enemy_step_next();
        return;
    case 8:
        rpg_enemy_damage_roll();
        rpg_message_format_damage((s32) rpg_text_enemy_0, (s32) game_state.player_family_name, (s32) "は", rpg_damage, (s32) "のダメージを受けた");
        rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x3C);
        rpg_enemy_step_next();
        return;
    case 9:
        rpg_enemy_wait_msg_step(1);
        return;
    case 10:
        rpg_apply_damage(0, 0x21, rpg_damage, 1);
        if (rpg_player_hit_points == 0) {
            rpg_player_action = RPG_PLAYER_DOWN;
            /* FAKE: T-17105, rpg_enemy_step written through the base symbol of rpg_enemy_action (the
               aggregate is not declared yet); as1 then keeps the rpg_enemy_action load above this store */
            (&rpg_enemy_action)[5] = 0;
            rpg_enemy_action &= 0xFFFF0000;
            rpg_enemy_action |= 5;
            rpg_actor_set_animation(0x30, 7, 1, 1);
        }
        rpg_enemy_step_next();
        return;
    case 11:
        rpg_enemy_wait_msg_step(0);
        return;
    case 12:
        rpg_enemy_wait_step(0x14);
        return;
    case 13:
        rpg_enemy_action_end();
        break;
    }
}

void rpg_enemy_bancho_phase_two(void) {
    switch (rpg_enemy_step) {
    case 0:
        rpg_dialog_bancho_resolve_1();
        break;
    case 1:
        rpg_actor_set_animation(0x3B, 6, 5, 1);
        rpg_enemy_step_next();
        break;
    case 2:
        if (rpg_enemy_timer_a == 0x22) {
            rpg_se_play_latched_5xx(0x509, 1, 0);
            rpg_actor_set_position(0x3A, 0, 0);
            rpg_actor_set_animation(0x3A, 7, 1, 1);
        }
        rpg_enemy_timer_a += 1;
        if (rpg_enemy_timer_a >= 0x2B) {
            rpg_enemy_step_next();
        }
        break;
    case 3:
        if (!(rpg_enemy_timer_a & 1)) {
            rpg_actor_set_animation(0x3A, 7, 1, 0);
        } else {
            rpg_actor_set_animation(0x3A, 7, 1, 1);
        }
        rpg_enemy_timer_a += 1;
        if (rpg_enemy_timer_a >= 0x3D) {
            rpg_actor_set_animation(0x3B, 8, 1, 1);
            rpg_enemy_action |= 0x400000;
            rpg_enemy_action &= ~0x200000;
            rpg_enemy_step_next();
        }
        break;
    case 4:
        rpg_enemy_wait_step(0x14);
        break;
    case 5:
        rpg_enemy_action_end();
        break;
    }
}

/* `sh` holds each shifted value so that addu takes (value, shifted) in the original operand order (T-17105) */
void rpg_enemy_bancho_attack_powered(void) {
    s32 temp;
    s32 color;
    s32 half;
    s32 sh;
    s32 mid;

    switch (rpg_enemy_step) {
    case 0:
        rpg_dialog_bancho_grunt_random();
        return;
    case 1:
        rpg_actor_set_animation(0x3B, 9, 5, 1);
        rpg_se_play_latched_5xx(0x50A, 1, 0);
        rpg_enemy_step_next();
        return;
    case 2:
        if (rpg_enemy_timer_a == 0x6E) {
            rpg_actor_set_position(0x3A, 0, 0);
            ((Actor *)actor_pool)[154].scale_x = 0x100;
            ((Actor *)actor_pool)[154].scale_y = 0x100;
            ((Actor *)actor_pool)[154].unk_01 = 4;
            rpg_actor_set_animation(0x3A, 0xB, 1, 1);
            rpg_se_play_latched_5xx(0x50B, 1, 0);
            rpg_enemy_step_next();
        }
        rpg_enemy_timer_a += 1;
        break;
    case 3:
        temp = ((rpg_enemy_timer_a << 0xD) / 256) + 0x100;
        ((Actor *)actor_pool)[154].scale_x = temp;
        ((Actor *)actor_pool)[154].scale_y = temp;
        rpg_enemy_timer_a += 3;
        if (rpg_enemy_timer_a < 0x100) {
            half = rpg_enemy_timer_a / 2;
            sh = half << 8;
            mid = half + sh;
            sh = mid << 8;
            color = mid + sh;
        } else {
            color = 0x7FFE7F;
        }
        prim_gradient_rect(-0xA0, -0x78, 0x140, 0xA8, 2, color, 0x81);
        dtd_on_tpage(0, 0, 2, 1, 0);
        if (rpg_enemy_timer_a >= 0x1F5) {
            ((Actor *)actor_pool)[154].scale_x = 0x1000;
            ((Actor *)actor_pool)[154].scale_y = 0x1000;
            ((Actor *)actor_pool)[154].unk_01 = 5;
            rpg_actor_set_animation(0x3A, 0xFF, 1, 0);
            rpg_actor_set_animation(0x3B, 8, 1, 1);
            if (rpg_player_action == RPG_COMMAND_GUARD) {
                rpg_player_action = RPG_PLAYER_GUARD;
            }
            if (rpg_player_action != RPG_PLAYER_GUARD) {
                rpg_actor_set_animation(0x30, 5, 1, 1);
            }
            rpg_enemy_step_next();
            return;
        }
        break;
    case 4:
        half = 0x7F - rpg_enemy_timer_a;
        sh = half << 8;
        mid = half + sh;
        sh = mid << 8;
        prim_gradient_rect(-0xA0, -0x78, 0x140, 0xA8, 2, mid + sh, 0x81);
        dtd_on_tpage(0, 0, 2, 1, 0);
        rpg_enemy_timer_a += 7;
        if (rpg_enemy_timer_a >= 0x80) {
            rpg_enemy_step_next();
        }
        return;
    case 5:
        rpg_enemy_damage_roll();
        rpg_message_format_damage((s32) rpg_text_enemy_0, (s32) game_state.player_family_name, (s32) "は", rpg_damage, (s32) "のダメージを受けた");
        rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x3C);
        rpg_enemy_step_next();
        return;
    case 6:
        rpg_enemy_wait_msg_step(1);
        return;
    case 7:
        rpg_apply_damage(0, 0x21, rpg_damage, 1);
        if (rpg_player_hit_points == 0) {
            rpg_player_action = RPG_PLAYER_DOWN;
            /* FAKE: T-17105, rpg_enemy_step written through the base symbol of rpg_enemy_action (the
               aggregate is not declared yet); as1 then keeps the rpg_enemy_action load above this store */
            (&rpg_enemy_action)[5] = 0;
            rpg_enemy_action &= 0xFFFF0000;
            rpg_enemy_action |= 5;
            rpg_actor_set_animation(0x30, 7, 1, 1);
        }
        rpg_enemy_step_next();
        return;
    case 8:
        rpg_enemy_wait_msg_step(0);
        return;
    case 9:
        rpg_enemy_wait_step(0x14);
        return;
    case 10:
        rpg_enemy_action_end();
        break;
    }
}

void rpg_enemy_player_defeated_bancho(void) {
    switch (rpg_enemy_step) {
    case 0:
        rpg_player_action = RPG_PLAYER_DOWN;
        rpg_enemy_wait_msg_step(0);
        return;
    case 1:
        rpg_player_action = RPG_PLAYER_DOWN;
        rpg_enemy_wait_msg_step(0);
        return;
    case 2:
        rpg_enemy_wait_step(0x14);
        return;
    case 3:
        audio_bank_start(1, 0x203);
        rpg_enemy_step_next();
        return;
    case 4:
        rpg_dialog_bancho_scorn();
        return;
    case 5:
        rpg_result_code = RPG_RESULT_BAD;
        mode_stage_next();
        return;
    }
}

void rpg_victory_bancho(void) {
    s32 temp_v0;

    switch (rpg_finale_channel.unk_00) {
    case 0:
        rpg_player_action = RPG_PLAYER_DOWN;
        rpg_finale_step_next();
    default:
        break;
    case 1:
        rpg_dialog_bancho_defeated();
        break;
    case 2:
        strcpy(rpg_text_result_0, &game_state.player_family_name);
        strcat(rpg_text_result_0, "「番長・・・。");
        rpg_message_show((s32) rpg_text_result_pointers, 1, 0x5A);
        rpg_finale_step_next();
        break;
    case 3:
        rpg_finale_wait_msg_step(0);
        break;
    case 4:
        rpg_dialog_bancho_hands_over_title();
        break;
    case 5:
        text_system_reset();
        rpg_label_player = rpg_label_create(0xB1, 0xB3, (s32) "番長") - 1;
        rpg_label_girl = rpg_label_create(0xB1, 0xC3, (s32) girl_short_name_table[rpg_girl_id]) - 1;
        rpg_label_guest = rpg_label_create(0x13, 0xBB, (s32) "元・番長") - 1;
        k_disp_start(rpg_label_guest + 1);
        k_disp_switch(rpg_label_player, 1);
        k_disp_switch(rpg_label_girl, 1);
        k_disp_switch(rpg_label_guest, 1);
        k_sub_reset_point_set();
        rpg_se_play_latched_5xx(0x515, 1, 0);
        rpg_finale_step_next();
        break;
    case 6:
        rpg_dialog_ex_bancho_farewell();
        break;
    case 7:
        ((Actor *)actor_pool)[155].draw_flags |= 1;
        ((Actor *)actor_pool)[155].x -= 0x10;
        temp_v0 = rpg_finale_timer + 1;
        rpg_finale_timer = temp_v0;
        if (temp_v0 >= 0x21) {
            rpg_finale_step_next();
        }
        break;
    case 8:
        rpg_finale_wait_step(0x1E);
        break;
    case 9:
        rpg_actor_set_animation(0x30, 9, 1, 1);
        switch (rpg_girl_id) {
        case 0:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        case 1:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        case 2:
            rpg_actor_set_animation(0x1D, 2, 1, 1);
            break;
        case 3:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        case 4:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        case 5:
            rpg_actor_set_animation(0x1D, 2, 1, 1);
            break;
        case 6:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        case 7:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        case 8:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        case 9:
            rpg_actor_set_animation(0x1D, 7, 1, 1);
            break;
        case 10:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        case 13:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        }
        rpg_finale_step_next();
        break;
    case 10:
        strcpy(rpg_text_result_0, "戦闘に勝った！！");
        strcpy(rpg_text_result_1, "経験値　：３２７６７ＥＸＰ");
        strcpy(rpg_text_result_2, "お金　　：６５５３５ゴールド");
        rpg_loot_text_bancho();
        rpg_message_show((s32) rpg_text_result_pointers, rpg_result_line_count, 0x5A);
        audio_bank_start(1, 0x202);
        rpg_finale_step_next();
        break;
    case 11:
        rpg_finale_wait_msg_step(0);
        break;
    case 12:
        rpg_result_code = RPG_RESULT_GOOD;
        mode_stage_next();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/rpg_battle/enemy_bancho.rodata", D_8015E13C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/rpg_battle/enemy_bancho.rodata", D_8015E14C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/rpg_battle/enemy_bancho.rodata", D_8015E15C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/rpg_battle/enemy_bancho.rodata", D_8015E16C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/rpg_battle/enemy_bancho.rodata", D_8015E178);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/rpg_battle/enemy_bancho.rodata", D_8015E184);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/rpg_battle/enemy_bancho.rodata", D_8015E190);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/rpg_battle/enemy_bancho.rodata", D_8015E19C);

/* The declaration order sets the frame layout (T-17105): the original has three scalar slots between the second string table and the flag table, and one counter serves both loops. */
s32 rpg_loot_text_bancho(void) {
    RpgRewardTextTable sp88;
    RpgRewardTextTable sp78;
    s32 count;
    s32 n;
    s32 i;
    RpgRewardSelectionFlags sp5C;

    sp88 = rpg_loot_names_bancho;
    sp78 = rpg_titles_bancho;
    sp5C = rpg_loot_flags_init_bancho;
    count = 0;
    if (rpg_rand(5) == 0) {
        n = 3;
    } else {
        n = 2;
    }
    for (i = 0; i < n; i++) {
        switch (rpg_rand(0xA)) {
        case 0:
        case 1:
        case 2:
        case 3:
            sp5C.selected[0] = 1;
            break;
        case 4:
        case 5:
        case 6:
            sp5C.selected[1] = 1;
            break;
        case 7:
        case 8:
            sp5C.selected[2] = 1;
            break;
        case 9:
            sp5C.selected[3] = 1;
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        if (sp5C.selected[i] == 1) {
            strcpy(((void **)rpg_text_result_pointers)[count + 3], "おたから：");
            strcat(((void **)rpg_text_result_pointers)[count + 3], sp88.strings[i]);
            count++;
        }
    }
    strcpy(((void **)rpg_text_result_pointers)[count + 3], "獲得称号：");
    strcat(((void **)rpg_text_result_pointers)[count + 3], sp78.strings[rpg_rand(4)]);
    rpg_result_line_count = count + 4;
}
