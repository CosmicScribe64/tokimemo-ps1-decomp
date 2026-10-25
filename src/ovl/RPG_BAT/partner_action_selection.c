/* partner_action_selection.c: src/ovl/RPG_BAT/801477E0.c:7: partner damage rolls and weighted action selection; body 12: switch (sel) {; 14: rpg_damage = rpg_rand2(0x4E2, 0x2EE) + 1;. Original object 0x801477E0-0x80147F10 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_delinquent_charmed_0 /* scalar view: the original sums three flag words with EBD4 evaluated first (T-17109) */
#include "common.h"
#include "ovl/RPG_BAT.h"

extern s32 rpg_delinquent_charmed_0;

void rpg_girl_damage_roll(void) {
    s32 temp_t4;
    s32 player_extra_damage_bound;
    u32 sel = rpg_girl_id; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 2:
        rpg_damage = rpg_rand2(0x4E2, 0x2EE) + 1;
        return;
    case 5:
        rpg_damage = rpg_rand2(0x7D0, 0x3E8) + 1;
        return;
    case 3:
        if (rpg_enemy_action & 0x600000) {
            rpg_damage = rpg_rand2(0x3E8, 0x1F4) + 1;
        } else {
            rpg_damage = rpg_rand2(0x258, 0x12C) + 1;
            rpg_damage_slot1 = rpg_rand2(0x258, 0x12C) + 1;
            rpg_damage_slot2 = rpg_rand2(0x258, 0x12C) + 1;
        }
        temp_t4 = (s32) rpg_player_hit_points / 4;
        player_extra_damage_bound = temp_t4;
        if (temp_t4 >= 0x65) {
            player_extra_damage_bound = 0x64;
        }
        rpg_damage_to_player_extra = rpg_rand2(player_extra_damage_bound, player_extra_damage_bound);
        return;
    case 4:
        rpg_damage = rpg_rand2(0x4B, 0x7D) + 1;
        return;
    }
}

s32 rpg_girl_pick_action(void) {
    s32 temp_v0;

    if (rpg_state.girl_weight_act + rpg_state.girl_weight_wait < 0x14) {
        switch (rpg_girl_id) {
        case 0:
            rpg_state.girl_weight_wait = 0x32;
            rpg_state.girl_weight_act = 0x32;
            break;
        case 2:
            rpg_state.girl_weight_wait = 0x32;
            rpg_state.girl_weight_act = 0x32;
            break;
        case 3:
            rpg_state.girl_weight_wait = 0x32;
            rpg_state.girl_weight_act = 0x32;
            break;
        case 4:
            rpg_state.girl_weight_wait = 0x32;
            rpg_state.girl_weight_act = 0x32;
            break;
        case 5:
            rpg_state.girl_weight_wait = 0x32;
            rpg_state.girl_weight_act = 0x32;
            break;
        case 6:
            rpg_state.girl_weight_wait = 0x32;
            rpg_state.girl_weight_act = 0x32;
            break;
        case 10:
            rpg_state.girl_weight_wait = 0x32;
            rpg_state.girl_weight_act = 0x32;
            break;
        case 8:
            rpg_state.girl_weight_wait = 0x32;
            rpg_state.girl_weight_act = 0x32;
            break;
        case 1:
            rpg_state.girl_weight_wait = 0x64;
            rpg_state.girl_weight_act = 0x64;
            break;
        case 7:
            rpg_state.girl_weight_wait = 0x32;
            rpg_state.girl_weight_act = 0x32;
            break;
        case 9:
            rpg_state.girl_weight_wait = 0x32;
            rpg_state.girl_weight_act = 0x32;
            break;
        case 13:
            rpg_state.girl_weight_wait = 0x64;
            rpg_state.girl_weight_act = 0x64;
            break;
        case 14:
            rpg_state.girl_weight_wait = 0x64;
            rpg_state.girl_weight_act = 0x64;
            break;
        }
    }
    switch (rpg_girl_id) {
    case 0:
        if (rpg_rand(rpg_state.girl_weight_act + rpg_state.girl_weight_wait) < rpg_state.girl_weight_wait) {
            rpg_state.girl_weight_wait -= 5;
            return 0;
        }
        rpg_enemy_timer = 0;
        rpg_state.girl_weight_act -= 5;
        return 1;
    case 2:
        if (rpg_rand(rpg_state.girl_weight_act + rpg_state.girl_weight_wait) < rpg_state.girl_weight_wait) {
            rpg_state.girl_weight_wait -= 5;
            return 0;
        }
        rpg_state.girl_weight_act -= 5;
        return 1;
    case 3:
        if (rpg_rand(rpg_state.girl_weight_act + rpg_state.girl_weight_wait) < rpg_state.girl_weight_wait) {
            rpg_state.girl_weight_wait -= 5;
            return 0;
        }
        rpg_state.girl_weight_act -= 5;
        return 1;
    case 4:
        if (rpg_rand(rpg_state.girl_weight_act + rpg_state.girl_weight_wait) < rpg_state.girl_weight_wait) {
            rpg_state.girl_weight_wait -= 5;
            return 0;
        }
        rpg_state.girl_weight_act -= 5;
        return 1;
    case 5:
        if (rpg_rand(rpg_state.girl_weight_act + rpg_state.girl_weight_wait) < rpg_state.girl_weight_wait) {
            rpg_state.girl_weight_wait -= 5;
            return 0;
        }
        rpg_state.girl_weight_act -= 5;
        return 1;
    case 6:
        if (rpg_rand(rpg_state.girl_weight_act + rpg_state.girl_weight_wait) < rpg_state.girl_weight_wait) {
            rpg_state.girl_weight_wait -= 5;
            return 0;
        }
        rpg_enemy_damage_scale += 0xA;
        rpg_state.girl_weight_act -= 5;
        return 1;
    case 10:
        if (rpg_rand(rpg_state.girl_weight_act + rpg_state.girl_weight_wait) < rpg_state.girl_weight_wait) {
            rpg_state.girl_weight_wait -= 5;
            return 0;
        }
        rpg_enemy_miss_weight = 0x32;
        rpg_state.girl_weight_act -= 5;
        return 1;
    case 8:
        temp_v0 = rpg_rand(rpg_state.girl_weight_act + rpg_state.girl_weight_wait);
        if (rpg_enemy_delay_extra >= 0x65) {
            return 0;
        }
        if (temp_v0 < rpg_state.girl_weight_wait) {
            rpg_state.girl_weight_wait -= 5;
            return 0;
        }
        rpg_enemy_delay_extra = 0x168;
        rpg_state.girl_weight_act -= 5;
        return 1;
    case 1:
        return 0x80000000;
    case 7:
        if (rpg_rand(rpg_state.girl_weight_act + rpg_state.girl_weight_wait) < rpg_state.girl_weight_wait) {
            rpg_state.girl_weight_wait -= 5;
            return 0;
        }
        rpg_state.girl_weight_act -= 5;
        if (rpg_enemy_action & 0x100000) {
            if (rpg_delinquent_charmed_2 + rpg_delinquent_charmed_0 + rpg_delinquent_charmed_1 == 3) {
                return 0;
            }
            if (rpg_rand(0x64) < 0x32) {
                return 1;
            }
            return 2;
        }
        return 2;
    case 9:
        if (rpg_rand(rpg_state.girl_weight_act + rpg_state.girl_weight_wait) < rpg_state.girl_weight_wait) {
            rpg_state.girl_weight_wait -= 5; return rpg_girl_action; /* one line: as1 then loads rpg_girl_action before the decrement */
        }
        rpg_state.girl_weight_act -= 5;
        if (rpg_girl_action != 0x40000000) {
            return 1;
        }
        if (rpg_rand(5) == 0) {
            return 2;
        }
        return 3;
    case 13:
        if (rpg_girl_hit_points < 8) {
            return 3;
        }
        if (rpg_rand(2) == 0) {
            return 2;
        }
        return 1;
    case 14:
        break;
    }
}
