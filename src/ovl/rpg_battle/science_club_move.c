/* science_club_move.c: src/ovl/RPG_BAT/801421E0.c:4: club 2 special move; body 7: switch (rpg_player_step) {; 10: rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x5A); club-index dispatch src/ovl/RPG_BAT/8013CC90.c:96. Original object 0x801421E0-0x80142F70 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/rpg_battle.h"

void rpg_club_move_science(void) {
    s32 temp_v0;

    switch (rpg_player_step) {
    case 0:
        strcpy(rpg_text_player_0, "　　　　　　世界征服ロボ　　　　　　");
        rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x5A);
        rpg_sparks_park();
        rpg_pick_target();
        rpg_set_target_position_alt(rpg_enemy_slot);
        rpg_player_damage_roll();
        rpg_player_step_next();
        rpg_player_step_next();
    default:
        break;
    case 2:
        rpg_move_approach();
        break;
    case 3:
        rpg_actor_set_animation(0x34, 3, 5, 1);
        rpg_player_step_next();
        break;
    case 4:
        if (!(D_80121422 & 1)) {
            rpg_actor_set_animation(0x34, 3, 5, 1);
            temp_v0 = rpg_player_channel.unk_04 + 1;
            rpg_player_channel.unk_04 = temp_v0;
            if (temp_v0 >= 5) {
                rpg_player_step_next();
            }
        }
        break;
    case 5:
        rpg_actor_set_animation(0x34, 2, 1, 1);
        rpg_actor_set_animation(0x2B, 0x11, 5, 1);
        rpg_se_play_latched_6xx(0x500, 1, 0);
        rpg_player_step_next();
        break;
    case 6:
        rpg_player_wait_animation_step(0x2B);
        break;
    case 7:
        rpg_science_step_7();
        break;
    case 8:
        rpg_blink_move_hero_and_girl_60();
        break;
    case 9:
        rpg_player_wait_step(0x1E);
        break;
    case 10:
        rpg_science_step_10();
        break;
    case 11:
        strcpy(rpg_text_player_0, "ロボ「服従パーンチ！");
        rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x3C);
        rpg_player_step_next();
        break;
    case 12:
        rpg_player_wait_msg_step(1);
        break;
    case 13:
        rpg_science_step_13();
        break;
    case 14:
        strcpy(rpg_text_player_0, "ロボ「独裁ミサイールッ！");
        rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x3C);
        rpg_player_step_next();
        break;
    case 15:
        rpg_player_wait_msg_step(1);
        break;
    case 16:
        rpg_science_step_16();
        break;
    case 17:
        rpg_player_wait_step(0x1E);
        break;
    case 18:
        strcpy(rpg_text_player_0, "ロボ「世界征服ビィーム！");
        rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x3C);
        rpg_player_step_next();
        break;
    case 19:
        rpg_player_wait_msg_step(0);
        break;
    case 20:
        rpg_science_step_20();
        break;
    case 21:
        rpg_apply_damage_to_targets();
        rpg_player_step_next();
        break;
    case 22:
        rpg_player_wait_step(0x28);
        break;
    case 23:
        rpg_science_step_23();
        break;
    case 24:
        rpg_player_wait_step(0x1E);
        break;
    case 25:
        rpg_blink_move_hero_and_girl_61();
        break;
    case 26:
        rpg_player_wait_step(0x14);
        break;
    case 27:
        rpg_move_return();
        break;
    case 28:
        rpg_player_action_end();
        break;
    }
}

/* FAKE: s32 fall-through keeps the selector in v1 and division result in v0. T-9780. */
s32 rpg_science_step_7(void) {
    s32 value;

    rpg_se_play_latched_6xx(0x509, 0, 1);
    rpg_actor_set_animation(0x2B, 0x12, 1, 1);
    rpg_actor_set_animation(0x2C, 0x13, 1, 1);
    if (rpg_player_channel.unk_04 < 200) {
        value = (rpg_player_channel.unk_04 << 12) / 30;
        ((Actor *)actor_pool)[139].scale_x = value;
        ((Actor *)actor_pool)[139].scale_y = value;
        ((Actor *)actor_pool)[140].scale_x = value;
        ((Actor *)actor_pool)[140].scale_y = value;
    }
    if (rpg_player_channel.unk_04 >= 10) {
        rpg_actor_set_position(0x2B, 0, ((10 - rpg_player_channel.unk_04) * rpg_player_channel.unk_04) / 100);
        rpg_actor_set_position(0x2C, 0, rpg_player_channel.unk_04 - 10);
    }
    rpg_player_channel.unk_04++;
    if (rpg_player_channel.unk_04 >= 129) {
        rpg_actor_set_animation(0x2B, 0xFF, 1, 0);
        rpg_actor_set_animation(0x2C, 0xFF, 1, 0);
        ((Actor *)actor_pool)[139].scale_x = 0x1000;
        ((Actor *)actor_pool)[139].scale_y = 0x1000;
        ((Actor *)actor_pool)[140].scale_x = 0x1000;
        ((Actor *)actor_pool)[140].scale_y = 0x1000;
        rpg_player_step_next();
    }
}

void rpg_science_step_10(void) {
    s32 t;

    if (rpg_player_channel.unk_04 == 0) {
        rpg_actor_set_animation_alt(0x2B, 4, 1, 1);
        rpg_actor_set_animation_alt(0x2C, 0x13, 1, 1);
        rpg_actor_set_position(0x2C, 0x110, 0x90);
    }
    rpg_actor_set_position(0x2B, 0x110, rpg_player_channel.unk_04 * 4 - 0xC0);
    t = (rpg_player_channel.unk_04 << 14) / 292;
    D_8012121A = t;
    D_8012121C = t;
    if (rpg_player_channel.unk_04 * 4 - 0xC0 >= 0x55) {
        rpg_actor_set_animation_alt(0x2B, 0x15, 1, 1);
    }
    rpg_player_channel.unk_04 += 1;
    if (rpg_player_channel.unk_04 * 4 - 0xC0 >= 0x65) {
        rpg_actor_set_animation(0x2B, 6, 1, 1);
        rpg_actor_set_animation(0x2C, 0xFF, 1, 0);
        rpg_actor_set_position(0x2B, 0, 0);
        rpg_player_step_next();
    }
}

s32 rpg_science_step_13(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (rpg_player_channel.unk_00) {
    case 0:
        rpg_actor_set_animation(0x2B, 8, 5, 1);
        rpg_player_substep_next();
    default:
        break;
    case 1:
        if (!(D_801211BE & 1)) {
            rpg_player_substep_next();
        }
        break;
    case 2:
        rpg_actor_set_animation(0x2B, 7, 1, 1);
        rpg_actor_set_position(0x2A, 0, 0);
        rpg_actor_set_animation(0x2A, 9, 5, 1);
        rpg_player_substep_next();
        break;
    case 3:
        if (((Actor *)actor_pool)[138].unk_18 == 7) {
            rpg_player_substep_next();
        }
        break;
    case 4:
        rpg_actor_set_position(0x2A, 0xD2, 0x48);
        rpg_actor_set_animation_alt(0x2A, 0xA, 1, 1);
        rpg_se_play_latched_6xx(0x504, 1, 4);
        rpg_player_substep_next();
        break;
    case 5:
        temp_v0 = ++rpg_player_channel.unk_04;
        rpg_actor_set_position(0x2A, 0xD2 - (temp_v0 * 8), 0x48);
        if (rpg_player_channel.unk_04 >= 0x31) {
            rpg_actor_set_animation_alt(0x2A, 0x16, 1, 1);
            rpg_player_substep_next();
        }
        break;
    case 6:
        temp_v0_2 = --rpg_player_channel.unk_04;
        rpg_actor_set_position(0x2A, 0xD2 - (temp_v0_2 * 8), 0x48);
        if (rpg_player_channel.unk_04 < -4) {
            rpg_actor_set_animation(0x2B, 8, 1, 1);
            rpg_actor_set_animation_alt(0x2A, 0xFF, 1, 0);
            rpg_player_substep_next();
        }
        break;
    case 7:
        temp_v0_3 = rpg_player_channel.unk_08 + 1;
        rpg_player_channel.unk_08 = temp_v0_3;
        if (temp_v0_3 >= 9) {
            rpg_actor_set_animation(0x2B, 6, 1, 1);
            rpg_player_step_next();
        }
        break;
    }
}

s32 rpg_science_step_16(void) {
    switch (rpg_player_channel.unk_00) {
    case 0:
        rpg_actor_set_animation(0x2B, 0xB, 5, 1);
        rpg_se_play_latched_6xx(0x502, 1, 2);
        /* FAKE: rpg_enemy_slot, rpg_target_x/2C/50/54 read as one object (loads stay behind the stores, as in the original); the real type is unknown. T-9310 */
        (&rpg_enemy_slot)[41] = (&rpg_enemy_slot)[31] - 0xF0;
        (&rpg_enemy_slot)[42] = (&rpg_enemy_slot)[32] - 0x58;
        if ((&rpg_enemy_slot)[0] == 1) {
            D_801215FD = 4;
        }
        rpg_player_substep_next();
        return;
    case 1:
        if (!(D_801211BE & 1)) {
            rpg_actor_set_position(0x2A, 0xF0, 0x58);
            rpg_actor_set_animation_alt(0x2A, 0xC, 1, 1);
            rpg_actor_set_animation(0x2B, 0x14, 5, 1);
            rpg_se_play_latched_6xx(0x506, 1, 6);
            rpg_player_substep_next();
            return;
        }
        return;
    case 2:
        if (!(D_801211BE & 1)) {
            rpg_actor_set_animation(0x2B, 6, 1, 1);
        }
        rpg_actor_set_position(0x2A, rpg_motion * rpg_player_channel.unk_04 / 100 + 0xF0, rpg_motion_dy * rpg_player_channel.unk_04 / 100 + 0x58);
        rpg_player_channel.unk_04 += 3;
        if (rpg_player_channel.unk_04 >= 0x65) {
            rpg_actor_set_animation_alt(0x2A, 0xE, 5, 1);
            rpg_se_play_latched_6xx(0x503, 1, 3);
            rpg_player_substep_next();
            return;
        }
        break;
    case 3:
        if (!(D_8012117A & 1)) {
            rpg_actor_set_animation(0x2B, 6, 1, 1);
            rpg_actor_set_animation_alt(0x2A, 0xFF, 1, 0);
            rpg_player_step_next();
        }
        break;
    }
}

void rpg_science_step_20(void) {
    switch (rpg_player_channel.unk_00) {
    case 0:
        rpg_actor_set_position(0x2A, 0, 0);
        rpg_actor_set_animation(0x2A, 0xF, 5, 1);
        rpg_player_substep_next();
        return;
    case 1:
        if (!(D_8012117A & 1)) {
            rpg_se_play_latched_6xx(0x505, 0xF, 5);
            rpg_actor_set_animation(0x2A, 0x10, 5, 1);
            rpg_player_channel.unk_04 += 1;
            rpg_player_substep_next();
            return;
        }
        return;
    case 2:
        if (rpg_player_channel.unk_04 < 0x32) {
            rpg_se_play_latched_6xx(0x505, 0xF, 5);
            if (!(D_8012117A & 1)) {
                rpg_actor_set_animation(0x2A, 0x10, 5, 1);
                rpg_player_channel.unk_04 += 1;
                return;
            }
        } else {
            rpg_actor_set_animation(0x2A, 0xFF, 1, 0);
            rpg_player_step_next();
        }
        break;
    }
}

void rpg_science_step_23(void) {
    s32 t;
    s32 u;
    s32 d;
    s32 y;

    if (rpg_player_channel.unk_04 == 0) {
        rpg_actor_set_animation_alt(0x2B, 0x15, 1, 1);
        rpg_actor_set_animation_alt(0x2C, 0x13, 1, 1);
        rpg_actor_set_position(0x2C, 0x110, 0x90);
        rpg_se_play_latched_6xx(0x507, 1, 7);
    }
    rpg_actor_set_position(0x2B, 0x110, 0x64 - rpg_player_channel.unk_04 * 4);
    d = 0x124;
    u = rpg_player_channel.unk_04 * 4;
    t = ((d - u) << 12) / d;
    D_8012121A = t;
    D_8012121C = t;
    y = 0x64 - u;
    if (y < 0x54 && y >= 0x4D) {
        rpg_actor_set_animation_alt(0x2B, 4, 1, 1);
    }
    rpg_player_channel.unk_04 += 1;
    if (0x64 - rpg_player_channel.unk_04 * 4 < -0xC0) {
        rpg_actor_set_animation(0x2B, 6, 1, 0);
        rpg_actor_set_animation(0x2C, 0xFF, 1, 0);
        rpg_player_step_next();
    }
}
