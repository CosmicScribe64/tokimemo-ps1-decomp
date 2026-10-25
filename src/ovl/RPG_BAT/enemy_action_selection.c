/* enemy_action_selection.c: src/ovl/RPG_BAT/801447E0.c:19: enemy damage rolls and weighted action selectors; body 24: if (game_state.param_guts.value >= 0x97) {; 25: lim = 0x96;. Original object 0x801447E0-0x801454F0 (config/objects/RPG_BAT.txt). */
/* FAKE: T-25138, rpg_delinquent_charmed_0 seen as one block up to the battle counters rpg_state (header: s32[3]). In
 * rpg_enemy_pick_delinquent the indexed flag read must alias EBE0 and the counters (uopt reloads them after it) and as1
 * keeps the EBE0 load before the counter writebacks only when both share one symbol. The real aggregate's base
 * and extent are not recovered. */
#define RPG_BAT_OVERRIDE_rpg_delinquent_charmed_0 /* RpgBattleView below, a partial view of the battle block (T-25138) */
#include "common.h"
#include "ovl/RPG_BAT.h"

typedef struct RpgBattleView {
    /* 0x00 */ s32 flag[3]; /* rpg_delinquent_charmed_0..rpg_delinquent_charmed_2 (RpgDelinquentGauges.charmed) */
    /* 0x0C */ s32 enemy_damage_scale;  /* rpg_enemy_damage_scale */
    /* 0x10 */ s32 enemy_delay_extra;  /* rpg_enemy_delay_extra */
    /* 0x14 */ s32 enemy_miss_weight;  /* rpg_enemy_miss_weight */
    /* 0x18 */ u8 pad[0x18]; /* rpg_voice_wait_frames..D_8015EBFB, RpgCodeEntry rpg_code_entry among them */
    /* 0x30 */ RpgBattleState stat; /* rpg_state */
} RpgBattleView; /* size 0x6C */
extern RpgBattleView rpg_delinquent_charmed_0;

void rpg_enemy_damage_roll(void) {
    /* T-24522: the result global carries the intermediate arithmetic range. */
    s32 k;
    s32 lim;

    if (game_state.param_guts.value >= 0x97) {
        lim = 0x96;
    } else {
        lim = game_state.param_guts.value;
    }
    if (rpg_encounter & 0x80000000) {
        switch (rpg_enemy_action & 0xFFFF) {
        case 1:
        default:
            rpg_damage = rpg_rand2(rpg_player_hit_points_maximum / 10, rpg_player_hit_points_maximum * 5 / 100) + 0x64;
            break;
        case 2:
            k = rpg_player_hit_points_maximum / 10;
            rpg_damage = rpg_rand2(k, k) + 0x96;
            break;
        }
        rpg_damage *= 0xE1 - lim;
        rpg_damage /= 150;
        rpg_damage = rpg_enemy_damage_scale * rpg_damage;
        rpg_damage /= 1000;
        if (rpg_player_action & 0x100) {
            rpg_enemy_damage_scale += 0x64;
            rpg_damage = rpg_damage / 2;
        }
    }
    if (rpg_encounter & 0x100000) {
        k = 0xE1 - lim;
        switch (rpg_enemy_action & 0xFFFF) {
        case 2:
        default:
            rpg_damage = rpg_rand(rpg_player_hit_points_maximum * 0x19 / 100 - 0x1F4) + 0x1F4;
            if (rpg_player_action & 0x100) {
                rpg_damage = rpg_damage / 3;
            }
            break;
        case 4:
            rpg_damage = rpg_rand(rpg_player_hit_points_maximum * 0x32 / 100 - 0x3E8) + 0x3E8;
            if (rpg_player_action & 0x100) {
                rpg_damage = rpg_damage / 10;
            }
            break;
        }
        rpg_damage *= k;
        rpg_damage /= 150;
        rpg_damage = rpg_enemy_damage_scale * rpg_damage;
        rpg_damage /= 1000;
    }
    if (rpg_encounter & 0xF0000) {
        if (rpg_player_action & 0x100) {
            rpg_damage = rpg_player_hit_points / 100;
        } else {
            rpg_damage = rpg_player_hit_points / 10;
        }
    }
    if (rpg_encounter & 0x08000000) {
        if (rpg_player_action & 0x1000) {
            rpg_damage = rpg_player_hit_points / 2;
            return;
        }
        if (rpg_player_action & 0x100) {
            rpg_damage = rpg_player_hit_points / 50;
        } else {
            rpg_damage = rpg_player_hit_points / 5;
        }
        if (rpg_player_hit_points < 0x14) {
            rpg_damage = 0;
        }
    }
}

s32 rpg_enemy_pick_action(void) {
    switch (rpg_enemy_action & 0xFFFF0000) {
    case 0x10000:
    case 0x20000:
    case 0x40000:
        return rpg_enemy_pick_trip();
    case 0x100000:
        return rpg_enemy_pick_delinquent();
    case 0x200000:
        return rpg_enemy_pick_bancho_1();
    case 0x400000:
        return rpg_enemy_pick_bancho_2();
    case 0x8000000:
        rpg_state.escalation_counter += 2;
        return 1;
    case 0x80000000:
        return rpg_enemy_pick_alien();
    }
}

s32 rpg_enemy_pick_trip(void) {
    if (++rpg_enemy_attack_count < 3) {
        return 1;
    }
    return 2;
}

s32 rpg_enemy_pick_delinquent(void) {
    s32 sel;
    s32 total;
    s32 lim;
    s32 roll;

    if (game_state.param_guts.value >= 0x97) {
        lim = 0x96;
    } else {
        lim = game_state.param_guts.value;
    }
    sel = rpg_rand(3);
    switch (sel) {
    case 0:
        rpg_enemy_slot = 0;
        break;
    case 1:
        rpg_enemy_slot = 1;
        break;
    case 2:
        rpg_enemy_slot = 2;
        break;
    }
    if (rpg_has_boss_event()) {
        if (rpg_delinquent_charmed_0.stat.enemy_weight_3 + rpg_delinquent_charmed_0.stat.enemy_weight_0 + rpg_delinquent_charmed_0.stat.enemy_weight_1 + rpg_delinquent_charmed_0.stat.enemy_weight_2 < 0x15) {
            rpg_delinquent_charmed_0.stat.enemy_weight_0 = 0xF;
            rpg_delinquent_charmed_0.stat.enemy_weight_1 = 0xF;
            rpg_delinquent_charmed_0.stat.enemy_weight_2 = 5;
            rpg_delinquent_charmed_0.stat.enemy_weight_3 = 0x1E;
        }
    } else {
        if (rpg_delinquent_charmed_0.stat.enemy_weight_3 + rpg_delinquent_charmed_0.stat.enemy_weight_0 + rpg_delinquent_charmed_0.stat.enemy_weight_1 + rpg_delinquent_charmed_0.stat.enemy_weight_2 < 0x15) {
            rpg_delinquent_charmed_0.stat.enemy_weight_0 = 0xA;
            rpg_delinquent_charmed_0.stat.enemy_weight_1 = 0xA;
            rpg_delinquent_charmed_0.stat.enemy_weight_2 = 0;
            rpg_delinquent_charmed_0.stat.enemy_weight_3 = 0x28;
        }
    }
    total = rpg_delinquent_charmed_0.enemy_miss_weight + rpg_delinquent_charmed_0.stat.enemy_weight_0 + rpg_delinquent_charmed_0.stat.enemy_weight_1 + rpg_delinquent_charmed_0.stat.enemy_weight_2 + rpg_delinquent_charmed_0.stat.enemy_weight_3;
    roll = rpg_rand(total);
    if (roll < rpg_delinquent_charmed_0.stat.enemy_weight_0) {
        rpg_delinquent_charmed_0.stat.enemy_weight_0 -= 5;
        return 1;
    }
    if (roll < rpg_delinquent_charmed_0.stat.enemy_weight_1 + rpg_delinquent_charmed_0.stat.enemy_weight_0) {
        /* FAKE: T-24522, comma return keeps the decrement on the return line (as1 schedules per line). */
        return (rpg_delinquent_charmed_0.stat.enemy_weight_1 -= 5, 0);
    }
    if (roll < rpg_delinquent_charmed_0.enemy_miss_weight + rpg_delinquent_charmed_0.stat.enemy_weight_0 + rpg_delinquent_charmed_0.stat.enemy_weight_1 + rpg_delinquent_charmed_0.stat.enemy_weight_2) {
        if (rpg_delinquent_charmed_0.flag[rpg_enemy_slot] == 1) {
            rpg_damage = rpg_rand2(0x64, 0x64);
            return 4;
        }
        rpg_delinquent_charmed_0.stat.enemy_weight_2 -= 5;
        rpg_delinquent_charmed_0.enemy_miss_weight -= 0x14;
        if (rpg_delinquent_charmed_0.stat.enemy_weight_2 < 0) {
            rpg_delinquent_charmed_0.stat.enemy_weight_2 = 0;
        }
        if (rpg_delinquent_charmed_0.enemy_miss_weight < 0) {
            rpg_delinquent_charmed_0.enemy_miss_weight = 0;
        }
        return 3;
    }
    rpg_delinquent_charmed_0.stat.enemy_weight_3 -= 5;
    if (rpg_has_boss_event() == 0) {
        rpg_delinquent_charmed_0.enemy_damage_scale += 0x19;
    }
    if (rpg_delinquent_charmed_0.flag[rpg_enemy_slot] == 1) {
        rpg_damage = rpg_rand2(0x64, 0x64);
        return 4;
    }
    if (rpg_girl_action != 0x40000000 && rpg_rand(0x64) < 5) {
        rpg_damage = rpg_girl_hit_points / 10;
        if (rpg_player_action & 0x100) {
            rpg_damage = rpg_damage / 10;
        }
        rpg_delinquent_charmed_0.stat.escalation_counter += 2;
        return 6;
    }
    if (rpg_player_action != 0x100) {
        if (rpg_rand(0x64) < 5) {
            rpg_damage = rpg_rand2(0xA, 0xA);
            rpg_damage = rpg_delinquent_charmed_0.enemy_damage_scale * rpg_damage;
            rpg_damage /= 1000;
            rpg_delinquent_charmed_0.stat.escalation_counter += 1;
            return 5;
        }
    }
    rpg_damage = rpg_rand2(0x7D, 0x4B);
    rpg_damage *= 0xE1 - lim;
    rpg_damage /= 150;
    rpg_damage = rpg_delinquent_charmed_0.enemy_damage_scale * rpg_damage;
    rpg_damage /= 1000;
    if (rpg_player_action & 0x100) {
        rpg_damage = rpg_damage / 10;
    }
    rpg_delinquent_charmed_0.stat.escalation_counter += 1;
    return 2;
}

s32 rpg_enemy_pick_bancho_1(void) {
    s32 total;
    s32 roll;

    if (rpg_enemy_hit_points < rpg_enemy_hit_points_maximum / 3) {
        rpg_state.enemy_weight_2 = 0xF;
        rpg_state.enemy_weight_1 = 5;
        rpg_state.enemy_weight_0 = 0;
        return 3;
    }
    total = rpg_state.enemy_weight_2 + rpg_state.enemy_weight_0 + rpg_state.enemy_weight_1;
    if (total < 0xB) {
        rpg_state.enemy_weight_0 = 0x14;
        rpg_state.enemy_weight_2 = 0x14;
        rpg_state.enemy_weight_1 = 0xF;
        total = 0x37;
    }
    roll = rpg_rand(total);
    if (roll < rpg_state.enemy_weight_0) {
        rpg_state.enemy_weight_0 -= 5;
        return 0;
    }
    if (roll < rpg_state.enemy_weight_1 + rpg_state.enemy_weight_0) {
        rpg_state.enemy_weight_1 -= 5;
        return 1;
    }
    rpg_state.enemy_weight_2 -= 5;
    rpg_state.escalation_counter += 2;
    return 2;
}

s32 rpg_enemy_pick_bancho_2(void) {
    s32 total;
    s32 roll;

    total = rpg_state.enemy_weight_2 + rpg_state.enemy_weight_0 + rpg_state.enemy_weight_1;
    if (total < 0xB) {
        rpg_state.enemy_weight_2 = 0xF;
        rpg_state.enemy_weight_0 = 0x19;
        rpg_state.enemy_weight_1 = 0xF;
        total = 0x37;
    }
    roll = rpg_rand(total);
    if (roll < rpg_state.enemy_weight_0) {
        rpg_state.enemy_weight_0 -= 5;
        return 0;
    }
    if (roll < rpg_state.enemy_weight_1 + rpg_state.enemy_weight_0) {
        rpg_state.enemy_weight_1 -= 5;
        return 1;
    }
    rpg_state.enemy_weight_2 -= 5;
    rpg_state.escalation_counter += 2;
    return 4;
}

/* FAKE: the word view at +0x0C fills an unrecovered RpgBattleState field; the
 * comma return preserves the original decrement scheduling. T-10010. */
s32 rpg_enemy_pick_alien(void) {
    s32 roll;
    s32 total;

    total = rpg_state.enemy_weight_3 + rpg_state.enemy_weight_0 + rpg_state.enemy_weight_1 + rpg_state.enemy_weight_2;
    if (total < 6) {
        rpg_state.enemy_weight_3 = 0xA;
        rpg_state.enemy_weight_1 = 5;
        rpg_state.enemy_weight_2 = 0xA;
        rpg_state.enemy_weight_0 = 0xF;
        total = 0x28;
    }
    roll = rpg_rand(total);
    if (roll < rpg_state.enemy_weight_0) {
        rpg_state.enemy_weight_0 -= 5;
        return 0;
    }
    if (roll < (rpg_state.enemy_weight_1 + rpg_state.enemy_weight_0)) {
        return (rpg_state.enemy_weight_1 -= 5, 4);
    }
    if (roll < (rpg_state.enemy_weight_2 + rpg_state.enemy_weight_0 + rpg_state.enemy_weight_1)) {
        rpg_state.enemy_weight_2 -= 5;
        return 1;
    }
    rpg_state.enemy_weight_3 -= 5;
    return 2;
}
