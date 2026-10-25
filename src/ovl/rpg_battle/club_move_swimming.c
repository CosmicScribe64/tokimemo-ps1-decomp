/* club_move_swimming.c: src/ovl/RPG_BAT/8014A660.c:4: club 9 special move; body 5: switch (rpg_player_step) {; 8: rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x5A); club-index dispatch src/ovl/RPG_BAT/8013CC90.c:96. Original object 0x8014A660-0x8014ADD0 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/rpg_battle.h"

void rpg_club_move_swimming(void) {
    switch (rpg_player_step) {
    case 0:
        strcpy(rpg_text_player_0, "　　　　　　　水竜破　　　　　　　");
        rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x5A);
        rpg_pick_target();
        rpg_set_target_position(rpg_enemy_slot);
        rpg_player_damage_roll();
        rpg_effects_select_asset_set(3);
        rpg_scratch = 0;
        rpg_move_timer = 0;
        rpg_move_value_0 = 0;
        rpg_move_value_1 = 0;
        rpg_effect_phase_0 = 0;
        rpg_effect_phase_1 = 0;
        rpg_effect_phase_2 = 0;
        rpg_effect_phase_3 = 0;
        rpg_player_step_next();
        rpg_player_step_next();
        return;
    case 2:
        rpg_move_approach();
        return;
    case 3:
        rpg_actor_set_animation(0x34, 3, 1, 1);
        rpg_actor_set_position(0x33, 0xD0, 0x70);
        rpg_actor_set_animation_alt(0x33, 5, 1, 1);
        rpg_player_step_next();
        return;
    case 4:
        rpg_player_wait_step(0x6C);
        return;
    case 5:
        rpg_swimming_splash_spawn(0, 0xD0, 0x70);
        rpg_actor_set_animation_alt(0x33, 5, 1, 0);
        rpg_actor_set_animation(0x34, 4, 1, 1);
        switch (rpg_enemy_slot) {
        case 0:
            D_80121685 = 4;
            break;
        case 1:
            D_801215FD = 4;
            D_80121685 = 4;
            break;
        }
        rpg_player_step_next();
        return;
    case 6:
        /* FAKE: scalar pointer views keep the promoted counter in v1 rather
         * than v0. Direct member access changes 31 instructions. T-12300 */
        (*(s32 *)&rpg_player_channel.unk_04)++;
        switch (rpg_enemy_slot) {
        case 0:
            switch (*(s32 *)&rpg_player_channel.unk_04) {
            case 9:
                rpg_swimming_splash_spawn(1, 0xB3, 0x78);
                break;
            case 18:
                rpg_player_step_next();
                break;
            }
            break;
        case 1:
            switch (*(s32 *)&rpg_player_channel.unk_04) {
            case 9:
                rpg_swimming_splash_spawn(1, 0xB4, 0x71);
                break;
            case 18:
                rpg_swimming_splash_spawn(2, 0x98, 0x72);
                break;
            case 27:
                rpg_swimming_splash_spawn(3, 0x7C, 0x73);
                break;
            case 36:
                rpg_player_step_next();
                break;
            }
            break;
        case 2:
            switch (*(s32 *)&rpg_player_channel.unk_04) {
            case 9:
                rpg_swimming_splash_spawn(1, 0xB3, 0x7A);
                break;
            case 18:
                rpg_swimming_splash_spawn(2, 0x96, 0x84);
                break;
            case 27:
                rpg_swimming_splash_spawn(3, 0x79, 0x8D);
                break;
            case 36:
                rpg_player_step_next();
                break;
            }
            break;
        default:
            switch (*(s32 *)&rpg_player_channel.unk_04) {
            case 9:
                rpg_swimming_splash_spawn(1, 0xB4, 0x7A);
                break;
            case 18:
                rpg_swimming_splash_spawn(2, 0x98, 0x84);
                break;
            case 27:
                rpg_swimming_splash_spawn(3, 0x7C, 0x8D);
                break;
            case 36:
                rpg_player_step_next();
                break;
            }
            break;
        }
        rpg_swimming_splash_update();
        return;
    case 7:
        rpg_swimming_splash_update();
        switch (rpg_enemy_slot) {
        case 0:
            rpg_swimming_main_effect(0x78, 0x82);
            return;
        case 1:
            rpg_swimming_main_effect(0x40, 0x72);
            return;
        case 2:
            rpg_swimming_main_effect(0x3C, 0xA2);
            return;
        default:
            rpg_swimming_main_effect(0x40, 0xA2);
            return;
        }
    case 8:
        rpg_player_step_next();
        return;
    case 9:
        rpg_effects_select_asset_set(0xFF);
        rpg_apply_damage_single();
        rpg_player_step_next();
        return;
    case 10:
        rpg_player_wait_step(0x14);
        return;
    case 11:
        rpg_move_return();
        return;
    case 12:
        rpg_player_action_end();
        /* fallthrough */
    default:
        return;
    }
}

/* FAKE: the slot table at rpg_enemy_slot (flag words at +0xC8, x/y pairs at +0xDC,
 * see rpg_swimming_splash_update) is not recovered and other files use its words as
 * scalars, so the stores go through the first symbol. Element indexes keep the
 * reload of arg0 in the ugen temporary $t9 (byte-offset casts gave $a3).
 * T-17108. */
void rpg_swimming_splash_spawn(s32 effect_group, s32 x, s32 y) {
    rpg_se_play_latched_6xx(0x500, 1, 0);
    (&rpg_enemy_slot)[effect_group + 0x32] = 1;
    (&rpg_enemy_slot)[effect_group * 2 + 55] = x;
    (&rpg_enemy_slot)[effect_group * 2 + 56] = y;
    rpg_actor_set_position(effect_group * 3 + 4, x, y);
    rpg_actor_set_position(effect_group * 3 + 5, x, y);
    rpg_actor_set_position(effect_group * 3 + 6, x, y);
    rpg_actor_set_animation_alt(effect_group * 3 + 4, 6, 5, 1);
    rpg_actor_set_animation_alt(effect_group * 3 + 5, 8, 1, 1);
    rpg_actor_set_animation_alt(effect_group * 3 + 6, 0xD, 5, 1);
}

/* FAKE: the slot table at rpg_enemy_slot (flag words at +0xC8, x/y pairs at +0xDC)
 * is not recovered and other files use its words as scalars, so it is read
 * through the first symbol, written inline at every use. The display records
 * use the typed Actor view; an `ent` pointer local and byte-offset
 * record casts made the base address a ugen temporary (every later t-register
 * one too low). T-17108. */
void rpg_swimming_splash_update(void) {
    s32 i;
    s32 t;
    s32 t5;
    s32 v;

    for (i = 0; i < 4; i++) {
        if ((&rpg_enemy_slot)[i + 0x32] == 1) {
            t = i * 3;
            t5 = t + 5;
            if (!(((Actor *)actor_pool)[t + 100].flags & 1)) {
                rpg_actor_set_animation_alt(t + 4, 7, 1, 1);
            }
            v = ((Actor *)actor_pool)[t + 102].unk_18;
            if (v >= 0x10) {
                v = 0x1E - v;
            }
            rpg_actor_set_position(t5, (&rpg_enemy_slot)[i * 2 + 55], (&rpg_enemy_slot)[i * 2 + 56] - v * 8);
            if (!(((Actor *)actor_pool)[t + 102].flags & 1)) {
                rpg_actor_set_animation_alt(t + 4, 0xFF, 1, 0);
                rpg_actor_set_animation_alt(t5, 0xFF, 1, 0);
                rpg_actor_set_animation_alt(t + 6, 0xFF, 1, 0);
            }
        }
    }
}
