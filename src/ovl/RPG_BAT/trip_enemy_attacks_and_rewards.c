/* trip_enemy_attacks_and_rewards.c: src/ovl/RPG_BAT/80151FC0.c:7: trip encounter basic attack and loot; body 8: if (rpg_encounter == 0x40000) {; 9: rpg_actor_set_animation(0x38, 3, 5, 1);. Original object 0x80151FC0-0x801525B0 (config/objects/RPG_BAT.txt). */
#define MAIN_API_OVERRIDE_strcpy /* matched with strcpy returning its destination: with the void prototype the switch on rpg_encounter in rpg_trip_loot_text takes v0, not v1 (main_api.h: void). T-17101 */
#include "common.h"
#include "ovl/RPG_BAT.h"

void *strcpy(void *dst, void *src);

void rpg_trip_attack_hit_effect(void) {
    if (rpg_encounter == 0x40000) {
        rpg_actor_set_animation(0x38, 3, 5, 1);
    } else {
        rpg_actor_set_animation(0x38, 4, 5, 1);
    }
    switch (rpg_encounter) {
    case 0x10000:
        rpg_se_play_latched_5xx(0x505, 1, 0);
        return;
    case 0x20000:
        rpg_se_play_latched_5xx(0x506, 1, 0);
        return;
    case 0x40000:
        rpg_se_play_latched_5xx(0x505, 1, 0);
        return;
    }
}

void rpg_enemy_trip_attack(void) {
    /* FAKE: two unused locals reserve the 8 bytes of frame above the sp34 spill; real source unknown. T-17101 */
    s32 home0;
    s32 home1;
    s32 sp34;

    switch (rpg_enemy_step) {
    case 0:
        rpg_enemy_damage_roll();
        switch (rpg_encounter) {
        case 0x10000:
            strcpy(rpg_text_enemy_0, "　　　　　テール・パンチ　　　　　");
            break;
        case 0x20000:
            strcpy(rpg_text_enemy_0, "　　　　　暴れん坊頭突き　　　　　");
            break;
        case 0x40000:
            strcpy(rpg_text_enemy_0, "　　　　クラッシュ・クロー　　　　");
            break;
        }
        rpg_message_format_damage(rpg_text_enemy_1, game_state.player_family_name, "は", rpg_damage, "のダメージを受けた");
        rpg_message_show(rpg_text_enemy_pointers, 2, 0x3C);
        rpg_enemy_step_next();
    default:
        break;
    case 1:
        rpg_message_mode = 3;
        rpg_enemy_timer_a += 1;
        if ((rpg_enemy_timer_a % 3) == 0) {
            ((Actor *)actor_pool)[155].unk_40 = 0xFF;
        } else {
            ((Actor *)actor_pool)[155].unk_40 = 0;
        }
        if (rpg_enemy_timer_a >= 0x10) {
            rpg_enemy_step_next();
        }
        break;
    case 2:
        if (!(rpg_player_action & 0x100)) {
            rpg_actor_set_animation(0x30, 5, 1, 1);
        }
        D_80121531 = 3;
        rpg_trip_attack_hit_effect();
        rpg_scratch = rpg_rand(4) + 1;
        sp34 = rpg_rand(0x14);
        rpg_actor_set_position(0x38, sp34 - 5, rpg_rand(0x14) - 5);
        rpg_enemy_step_next();
        break;
    case 3:
        if (rpg_actor_is_idle(0x38) != 0) {
            rpg_trip_attack_hit_effect();
            sp34 = rpg_rand(0x14);
            rpg_actor_set_position(0x38, sp34 - 5, rpg_rand(0x14) - 5);
            if (--rpg_scratch == 0) {
                rpg_enemy_step_next();
            }
        }
        break;
    case 4:
        if (rpg_actor_is_idle(0x38) != 0) {
            rpg_actor_set_animation(0x38, 0xFF, 5, 0);
            rpg_enemy_step_next();
            rpg_message_mode = 1;
        }
        break;
    case 5:
        rpg_enemy_wait_msg_step(1);
        break;
    case 6:
        rpg_apply_damage(0, 0x21, rpg_damage, 1);
        rpg_enemy_step_next();
        break;
    case 7:
        rpg_enemy_wait_msg_step(0);
        break;
    case 8:
        rpg_enemy_wait_step(0x14);
        break;
    case 9:
        rpg_player_pose_idle();
        rpg_enemy_action_end();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/trip_enemy_attacks_and_rewards.rodata", D_8015D7D4);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/trip_enemy_attacks_and_rewards.rodata", D_8015D7E0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/trip_enemy_attacks_and_rewards.rodata", D_8015D7EC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/trip_enemy_attacks_and_rewards.rodata", D_8015D7F4);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/trip_enemy_attacks_and_rewards.rodata", D_8015D7FC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/trip_enemy_attacks_and_rewards.rodata", D_8015D808);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/trip_enemy_attacks_and_rewards.rodata", D_8015D810);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/trip_enemy_attacks_and_rewards.rodata", D_8015D820);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/trip_enemy_attacks_and_rewards.rodata", D_8015D828);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/trip_enemy_attacks_and_rewards.rodata", D_8015D834);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/trip_enemy_attacks_and_rewards.rodata", D_8015D840);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/trip_enemy_attacks_and_rewards.rodata", D_8015D84C);

void rpg_trip_loot_text(void) {
    RpgRewardTextTable spA0;
    RpgRewardTextTable sp90;
    RpgRewardTextTable sp80;
    s32 i;
    s32 n;
    s32 count;
    RpgRewardSelectionFlags sp64;

    spA0 = rpg_loot_names_snake;
    sp90 = rpg_loot_names_deer;
    sp80 = rpg_loot_names_bear;
    sp64 = rpg_loot_flags_init_trip;
    count = 0;
    if (rpg_rand(5) == 0) {
        n = 3;
    } else {
        n = 2;
    }
    i = 0;
    if (n > 0) {
        do {
            sp64.selected[rpg_rand(4)] = 1;
            i += 1;
        } while (i != n);
    }
    for (i = 0; i < 4; i++) {
        if (sp64.selected[i] == 1) {
            strcpy(((u8 **)rpg_text_result_pointers)[count + 4], "おたから：");
            switch (rpg_encounter) {
            case 0x10000:
                strcat(((u8 **)rpg_text_result_pointers)[count + 4], spA0.strings[i]);
                break;
            case 0x20000:
                strcat(((u8 **)rpg_text_result_pointers)[count + 4], sp90.strings[i]);
                break;
            case 0x40000:
                strcat(((u8 **)rpg_text_result_pointers)[count + 4], sp80.strings[i]);
                break;
            }
            count += 1;
        }
    }
    rpg_result_line_count = count + 4;
}
