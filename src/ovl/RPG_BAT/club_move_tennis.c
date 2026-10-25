/* club_move_tennis.c: src/ovl/RPG_BAT/80155CC0.c:7: club 8 special move; body 8: switch (rpg_player_step) {; 11: rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x5A); club-index dispatch src/ovl/RPG_BAT/8013CC90.c:96. Original object 0x80155CC0-0x801563A0 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_se_play_latched_6xx /* implicit int view: the call's $v0 moves the rpg_player_channel selector of rpg_tennis_step_8 to $v1 (T-17116) */
#include "common.h"
#include "ovl/RPG_BAT.h"

s32 rpg_se_play_latched_6xx();

void rpg_club_move_tennis(void) {
    switch (rpg_player_step) {
    case 0:
        strcpy(rpg_text_player_0, "　　　　　　火の鳥サーブ　　　　　　");
        rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x5A);
        rpg_pick_target();
        rpg_set_target_position(rpg_enemy_slot);
        rpg_player_damage_roll();
        rpg_effects_select_asset_set(3);
        rpg_player_step_next();
        rpg_player_step_next();
        return;
    case 2:
        rpg_move_approach();
        return;
    case 3:
        rpg_actor_set_animation(0x34, 3, 5, 1);
        rpg_player_step_next();
        return;
    case 4:
        rpg_player_wait_step(0x29);
        return;
    case 5:
        rpg_tennis_step_5();
        return;
    case 6:
        rpg_actor_set_animation_alt(0x33, 6, 5, 1);
        rpg_se_play_latched_6xx(0x510, 0, 2);
        rpg_player_step_next();
        return;
    case 7:
        rpg_player_wait_animation_step(0x33);
        return;
    case 8:
        rpg_tennis_step_8();
        return;
    case 9:
        rpg_effects_select_asset_set(0xFF);
        rpg_apply_damage_single();
        rpg_player_step_next();
        return;
    case 10:
        rpg_actor_set_animation(0x34, 2, 1, 1);
        rpg_player_step_next();
        return;
    case 11:
        rpg_player_wait_step(0x14);
        return;
    case 12:
        rpg_move_return();
        return;
    case 13:
        rpg_player_action_end();
        /* fallthrough */
    default:
        return;
    }
}

void rpg_tennis_step_5(void) {
    rpg_se_play_latched_6xx(0x50E, 0, 3);
    switch (rpg_player_channel.unk_00) {
    case 0:
        switch (*(u32 *)&rpg_enemy_slot) {
        case 0:
            D_80121685 = 4;
            break;
        case 1:
            D_801215FD = 4;
            D_80121685 = 4;
            break;
        }
        if (rpg_enemy_action & 0x100000) {
            /* FAKE: rpg_motion/54 reached through rpg_target_x (one object for as1) keep the second load after the first store; the real aggregate is unknown. T-9320 */
            (&rpg_target_x)[10] = (&rpg_target_x)[0] - 0xEC;
            (&rpg_target_x)[11] = (&rpg_target_x)[1] - 0x17;
        } else {
            rpg_motion = -0xA4;
            rpg_motion_dy = 0x74;
        }
        rpg_actor_set_position(0x33, 0xEC, 0x2C);
        rpg_actor_set_animation_alt(0x33, 5, 1, 1);
        rpg_player_substep_next();
        break;
    case 1:
        rpg_actor_set_position(0x33, rpg_motion * rpg_player_channel.unk_04 / 1000 + 0xEC, rpg_motion_dy * rpg_player_channel.unk_04 / 1000 + 0x2C);
        rpg_player_channel.unk_04 = rpg_player_channel.unk_04 + 0x32;
        if (rpg_player_channel.unk_04 >= 0x3E9) {
            rpg_player_step_next();
        }
        break;
    }
}

/* s32 without a return: rpg_club_move_tennis calls this function earlier as an implicit int, a void definition does not build. */
/* FAKE: the twelve words at rpg_enemy_slot + 4 * (i + 50) are reached through the first symbol (the loop pointer starts at rpg_damage_to_player_extra); the
 * real work-block aggregate is unknown. The records are actor_pool elements i + 96, the rolled loops start at i = 4. T-17116 */
s32 rpg_tennis_step_8(void) {
    s32 i;
    s32 x;
    s32 y;
    s16 v;

    rpg_se_play_latched_6xx(0x50F, 0, 1);
    switch (rpg_player_channel.unk_00) {
    case 0:
        for (i = 4; i < 16; i++) {
            (&rpg_enemy_slot)[i + 50] = 0;
            ((Actor *)actor_pool)[i + 96].unk_01 = 5;
        }
        rpg_player_substep_next();
        break;
    case 1:
        ((Actor *)actor_pool)[147].y -= 2;
        v = (rpg_player_channel.unk_04 << 6) + 0x1000;
        ((Actor *)actor_pool)[147].scale_x = v;
        ((Actor *)actor_pool)[147].scale_y = v;
        rpg_player_channel.unk_04 += 1;
        for (i = 4; i < 16; i++) {
            if ((&rpg_enemy_slot)[i + 50] == 0 && rpg_rand(0x2D) == 0) {
                (&rpg_enemy_slot)[i + 50] = 1;
                x = ((Actor *)actor_pool)[147].x + rpg_rand(0x80) - 0x40;
                y = ((Actor *)actor_pool)[147].y - rpg_rand(0x28);
                rpg_actor_set_position(i, x + 0xA0, y + 0x78);
                rpg_actor_set_animation_alt(i, rpg_rand(2) + 8, 1, 1);
            }
            if ((&rpg_enemy_slot)[i + 50] != 0) {
                (&rpg_enemy_slot)[i + 50] += 1;
                if ((&rpg_enemy_slot)[i + 50] >= 0x1F) {
                    switch ((&rpg_enemy_slot)[i + 50]) {
                    case 31:
                        rpg_actor_set_animation_alt(i, 0xFF, 1, 0);
                        break;
                    case 32:
                        rpg_actor_set_animation_alt(i, 0xFF, 1, 1);
                        break;
                    case 33:
                        rpg_actor_set_animation_alt(i, 0xFF, 1, 0);
                        break;
                    case 34:
                        rpg_actor_set_animation_alt(i, 0xFF, 1, 1);
                        break;
                    case 35:
                        rpg_actor_set_animation_alt(i, 0xFF, 1, 0);
                        break;
                    case 36:
                        rpg_actor_set_animation_alt(i, 0xFF, 1, 1);
                        break;
                    case 37:
                        rpg_actor_set_animation_alt(i, 0xFF, 1, 0);
                        break;
                    }
                }
            }
            ((Actor *)actor_pool)[i + 96].y += 1;
            v = (rpg_player_channel.unk_04 << 6) + 0x1000;
            ((Actor *)actor_pool)[i + 96].scale_x = v;
            ((Actor *)actor_pool)[i + 96].scale_y = v;
        }
        if (((Actor *)actor_pool)[147].y < -0xC0) {
            rpg_actor_set_animation_alt(0x33, 0xFF, 1, 0);
            for (i = 4; i < 16; i++) {
                ((Actor *)actor_pool)[i + 96].scale_x = 0x1000;
                ((Actor *)actor_pool)[i + 96].scale_y = 0x1000;
            }
            ((Actor *)actor_pool)[147].scale_x = 0x1000;
            ((Actor *)actor_pool)[147].scale_y = 0x1000;
            rpg_player_step_next();
        }
        break;
    }
}
