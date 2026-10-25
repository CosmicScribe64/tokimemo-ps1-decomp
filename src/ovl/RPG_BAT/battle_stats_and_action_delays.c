/* battle_stats_and_action_delays.c: src/ovl/RPG_BAT/80144010.c:6: start values, target pick, timers; body 11: sum = 0; 12: rpg_player_club = 0xFF;. Original object 0x80144010-0x801447E0 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

/* The club index is kept in the global rpg_player_club throughout: a local for it
 * is coloured before the loop constants and shifts every register (T-17111). */
void rpg_init_player_state(void) {
    s32 i;
    s32 sum;
    s32 v;

    sum = 0;
    rpg_player_club = 0xFF;
    for (i = 0; i < 12; i++) {
        if (((ClubFlagBits *)&game_state.clubs[i])->unk_25 == 3) {
            rpg_player_club = i;
        }
        sum += game_state.clubs[i].practice_points;
    }
    if (rpg_player_club == 0xB) {
        if (sum == 0) {
            rpg_player_club = 0xB;
        } else {
            rpg_player_club = 0xFF;
        }
    }
    if (rpg_player_club == 0xFF) {
        rpg_club_move_available = 0;
        rpg_special_ready = 0;
        rpg_state.special_gauge = 0;
    } else {
        rpg_club_move_available = 1;
        rpg_special_ready = 1;
        rpg_state.special_gauge = 0x64;
    }
    rpg_escalation_counter = 0;
    if (rpg_encounter & 0x800F0000) {
        rpg_club_move_available = 0;
        rpg_special_ready = 0;
    }
    if ((rpg_encounter & 0xF0000) == 0 && rpg_girl_id == 0xD) {
        rpg_girl_id = 9;
    }
    if ((rpg_encounter & 0xF0000) == 0 && rpg_girl_id == 0xE) {
        rpg_girl_id = 9;
    }
    if ((rpg_encounter & 0xF0000) != 0 && rpg_girl_id != 0xD) {
        rpg_girl_id = 0xE;
    }
    if (rpg_encounter & 0x80000000) {
        rpg_girl_id = 2;
    }
    if (rpg_encounter & 0x08000000) {
        rpg_girl_id = 0xE;
    }
    if (rpg_encounter != 0x08000000) {
        rpg_player_ready_timer = 0xFFFF;
    } else {
        rpg_player_ready_timer = 0;
        rpg_player_action = 0x1000;
    }
    if (game_state.param_condition.value == 0) {
        v = 0xA;
    } else {
        v = game_state.param_condition.value * 0xA;
    }
    rpg_player_hit_points_maximum = v;
    rpg_player_hit_points = v;
    switch (rpg_girl_id) {
    case 0:
        rpg_girl_hit_points = 0x100;
        break;
    case 1:
        rpg_girl_hit_points = 0x63;
        break;
    case 2:
        rpg_girl_hit_points = 0x138;
        break;
    case 3:
        rpg_girl_hit_points = 0x130;
        break;
    case 4:
        rpg_girl_hit_points = 0xBD;
        break;
    case 5:
        rpg_girl_hit_points = 0x12A;
        break;
    case 6:
        rpg_girl_hit_points = 0x756;
        break;
    case 7:
        rpg_girl_hit_points = 0x115;
        break;
    case 8:
        rpg_girl_hit_points = 0x16D;
        break;
    case 9:
        rpg_girl_hit_points = 0xAD;
        break;
    case 10:
        rpg_girl_hit_points = 0x109;
        break;
    case 13:
        rpg_girl_hit_points = 0x23D;
        break;
    case 14:
        rpg_girl_hit_points = 0x7FFFFFFF;
        break;
    }
    rpg_girl_hit_points_maximum = rpg_girl_hit_points;
    rpg_enemy_action = rpg_encounter;
    if (rpg_encounter == 0x08000000) {
        rpg_enemy_action = 0x08000002;
    }
    switch (rpg_encounter) {
    case 0x10000:
    case 0x20000:
    case 0x40000:
        rpg_enemy_hit_points = 1;
        rpg_enemy_hit_points_maximum = 1;
        break;
    case 0x8000000:
        rpg_enemy_hit_points = 0x1388;
        rpg_enemy_hit_points_maximum = 0x1388;
        break;
    case 0x80000000:
        rpg_enemy_hit_points = 0x7FFFFFFF;
        rpg_enemy_hit_points_maximum = 0x7FFFFFFF;
        rpg_enemy_damage_scale = 0x3E8;
        break;
    case 0x100000:
        rpg_enemy_hit_points = 0x10000;
        rpg_enemy_hit_points_maximum = 0x10000;
        if (rpg_has_boss_event() != 0) {
            rpg_enemy_damage_scale = 0x3E8;
            rpg_delinquent_gauges = 0x1388;
            rpg_delinquent_hit_points_1 = 0x1388;
            rpg_delinquent_hit_points_2 = 0x1388;
        } else {
            rpg_enemy_damage_scale = 0x5DC;
            rpg_delinquent_gauges = 0x270F;
            rpg_delinquent_hit_points_1 = 0x270F;
            rpg_delinquent_hit_points_2 = 0x270F;
        }
        break;
    }
    switch (rpg_encounter) {
    case 0x10000:
        rpg_guest_value = 0x10000075;
        break;
    case 0x20000:
        rpg_guest_value = 0x10000232;
        break;
    case 0x40000:
        rpg_guest_value = 0x1000012A;
        break;
    default:
        rpg_guest_value = 0x7FFFFFFF;
        break;
    }
}

/* FAKE: integer fall-through preserves the selector/result registers. T-10010. */
s32 rpg_pick_target(void) {
    switch (rpg_enemy_action & 0xFFFF0000) {
    case 0x10000:
        rpg_enemy_slot = 0x10;
        break;
    case 0x20000:
        rpg_enemy_slot = 0x11;
        break;
    case 0x40000:
        rpg_enemy_slot = 0x12;
        break;
    case 0x100000:
        if (rpg_player_action & 1) {
            switch (rpg_rand(15)) {
            case 0:
                rpg_enemy_slot = 1;
                break;
            case 1:
                rpg_enemy_slot = 2;
                break;
            default:
                rpg_enemy_slot = 0;
                break;
            }
        } else {
            switch (rpg_rand(4)) {
            case 0:
                rpg_enemy_slot = 1;
                break;
            case 1:
                rpg_enemy_slot = 2;
                break;
            default:
                rpg_enemy_slot = 0;
                break;
            }
        }
        break;
    case 0x200000:
    case 0x400000:
        rpg_enemy_slot = 3;
        break;
    case 0x8000000:
        rpg_enemy_slot = 4;
        break;
    case 0x80000000:
        rpg_enemy_slot = 5;
        break;
    }
}

s32 rpg_player_ready_delay(s32 use_encounter_delay) {
    if (use_encounter_delay == 0) {
        return 0x78;
    }
    if (rpg_enemy_action & 0x80000000) {
        return rpg_rand2(0x1E, 0x1E) + 0x1E;
    }
    return rpg_rand2(0x28, 0x1E) + 0x3C;
}

s32 rpg_enemy_action_threshold(void) {
    switch (rpg_enemy_action & ~0xFFFF) {
    case 0x10000:
    case 0x20000:
    case 0x40000:
        return 0x14A;
    case 0x8000000:
        return 0x168;
    case 0x80000000:
        return rpg_enemy_delay_extra + 0x96;
    case 0x100000:
        return rpg_enemy_delay_extra + 0x96;
    case 0x200000:
        return rpg_enemy_delay_extra + 0x96;
    case 0x400000:
        return rpg_enemy_delay_extra + 0x96;
    }
}

s32 rpg_girl_action_delay(void) {
    switch (rpg_girl_id) {
    case 1:
        return 10;
    case 0:
        return 200;
    case 7:
        return 240;
    case 9:
        return 260;
    case 6:
        return 260;
    case 8:
        return 260;
    case 10:
        return 260;
    case 4:
        return 300;
    case 2:
        return 300;
    case 3:
        return 300;
    case 5:
        return 300;
    case 13:
        if (rpg_girl_hit_points < 10) {
            return 110;
        }
        return 170;
    case 14:
        return 0x7FFFFFFF;
    }
}

s32 rpg_has_boss_event(void) {
    if (rpg_boss_event == 1) {
        return 1;
    }
    return 0;
}
