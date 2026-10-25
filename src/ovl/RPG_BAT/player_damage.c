/* player_damage.c: src/ovl/RPG_BAT/80148F70.c:4: player damage rolls; body 10: stat = game_state.clubs[rpg_player_club].practice_points; 11: if (stat < 50) {. Original object 0x80148F70-0x801496F0 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

void rpg_player_damage_roll(void) {
    s32 flag;
    u16 stat;
    s32 base;
    s32 half;

    stat = game_state.clubs[rpg_player_club].practice_points;
    if (stat < 50) {
        base = 50;
    } else if (stat > 550) {
        base = 550;
    } else {
        base = stat;
    }
    base = base * 10 - 500;
    if (rpg_encounter & 0xF0000) {
        rpg_player_basic_damage();
    }
    if (rpg_encounter & 0x80000000) {
        rpg_player_basic_damage();
    }
    flag = rpg_enemy_action & 0x100000;
    if (flag || (rpg_enemy_action & 0x600000) || (rpg_enemy_action & 0x08000000)) {
        if (rpg_player_action & RPG_PLAYER_SPECIAL) {
            switch (rpg_player_club) {
            case 2:
                if (flag) {
                    rpg_damage = rpg_rand2(2500, 2500) + 2500;
                    rpg_damage_slot1 = rpg_rand2(2500, 2500) + 2500;
                    rpg_damage_slot2 = rpg_rand2(2500, 2500) + 2500;
                    if (rpg_enemy_slot == 0) {
                        rpg_damage = RPG_DAMAGE_MAXIMUM;
                    }
                    if (rpg_enemy_slot == 1) {
                        rpg_damage_slot1 = RPG_DAMAGE_MAXIMUM;
                    }
                    if (rpg_enemy_slot == 2) {
                        rpg_damage_slot2 = RPG_DAMAGE_MAXIMUM;
                    }
                } else {
                    rpg_damage = RPG_DAMAGE_MAXIMUM;
                }
                break;
            case 5:
                if (flag) {
                    rpg_damage = rpg_rand2(2500, 1000) + 2000;
                    rpg_damage_slot1 = rpg_rand2(2500, 1000) + 2000;
                    rpg_damage_slot2 = rpg_rand2(2500, 1000) + 2000;
                    if (rpg_enemy_slot == 0) {
                        rpg_damage += 2000;
                    }
                    if (rpg_enemy_slot == 1) {
                        rpg_damage_slot1 += 2000;
                    }
                    if (rpg_enemy_slot == 2) {
                        rpg_damage_slot2 += 2000;
                    }
                } else {
                    rpg_damage = rpg_rand2(2500, 1000) + 4000;
                }
                break;
            case 6:
                if (flag) {
                    rpg_damage = rpg_rand(50) * 100 + 2000;
                    rpg_damage_slot1 = rpg_rand(50) * 100 + 1000;
                    rpg_damage_slot2 = rpg_rand(50) * 100 + 1000;
                } else {
                    rpg_damage = rpg_rand(50) * 100 + 3000;
                }
                break;
            case 1:
                if (flag) {
                    rpg_damage = rpg_rand2(6000, base);
                    rpg_damage_slot1 = rpg_rand2(6000, base);
                    rpg_damage_slot2 = rpg_rand2(6000, base);
                } else {
                    rpg_damage = rpg_rand2(6000, base);
                }
                break;
            case 4:
                if (flag) {
                    if (rpg_rand(100) < 20) {
                        rpg_damage = 1000;
                    } else {
                        rpg_damage = rpg_rand2(base / 2, 3500) + 2500;
                    }
                } else {
                    rpg_damage = rpg_rand2(base / 2, 3500) + 2500;
                }
                break;
            case 0:
                rpg_damage = rpg_rand(base / 1000 + 1) * 1000 + 5000;
                break;
            case 9:
                half = base / 2;
                rpg_damage = rpg_rand2(half, half) + 6000;
                break;
            case 3:
                rpg_damage = rpg_rand2(1500, base) + 3000;
                break;
            case 7:
                rpg_damage = rpg_rand2(base, base + 2000) + 1000;
                break;
            case 10:
                rpg_damage = rpg_rand2(base, base + 1000) + 2000;
                break;
            case 8:
                half = (base + 2000) / 2;
                rpg_damage = rpg_rand2(half, half) + 4000;
                break;
            case 11:
                switch (rpg_escalation_counter) {
                case 0:
                case 1:
                    rpg_escalation_counter = 0;
                    rpg_damage = rpg_rand2(500, 500);
                    break;
                case 2:
                case 3:
                    rpg_escalation_counter = 1;
                    rpg_damage = rpg_rand2(1000, 1000);
                    break;
                case 4:
                case 5:
                    rpg_escalation_counter = 2;
                    rpg_damage = rpg_rand2(1500, 1500);
                    break;
                case 6:
                case 7:
                    rpg_escalation_counter = 3;
                    rpg_damage = rpg_rand2(2000, 2000);
                    break;
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                    rpg_escalation_counter = 4;
                    rpg_damage = rpg_rand2(2500, 2500);
                    break;
                default:
                    rpg_escalation_counter = 5;
                    if (flag) {
                        rpg_damage = RPG_DAMAGE_MAXIMUM;
                    } else {
                        rpg_enemy_hit_points -= 30000;
                        rpg_damage = RPG_DAMAGE_MAXIMUM;
                    }
                    break;
                }
                break;
            }
        } else {
            rpg_player_basic_damage();
        }
    }
    if (rpg_damage >= RPG_DAMAGE_CLAMP_LIMIT) {
        rpg_damage = RPG_DAMAGE_MAXIMUM;
    }
    if (rpg_damage_slot1 >= RPG_DAMAGE_CLAMP_LIMIT) {
        rpg_damage_slot1 = RPG_DAMAGE_MAXIMUM;
    }
    if (rpg_damage_slot2 >= RPG_DAMAGE_CLAMP_LIMIT) {
        rpg_damage_slot2 = RPG_DAMAGE_MAXIMUM;
    }
}

void rpg_player_basic_damage(void) {
    s32 rate;
    s32 lv;
    s32 base;
    s32 r;

    rate = game_state.param_athletics.value;
    lv = (rate > 300) ? 300 : rate;
    base = rpg_rand2(100, 100) + lv * 9 / 2;
    r = rpg_rand(0x1000);
    if (r == 0) {
        rpg_damage = RPG_DAMAGE_MAXIMUM;
    } else if (r < 0x40) {
        rpg_damage = 0;
    } else if (r < 0x10E) {
        rpg_damage = rpg_rand2(500, 1000) + base;
    } else {
        rpg_damage = base;
    }
    if (rpg_damage >= RPG_DAMAGE_CLAMP_LIMIT) {
        rpg_damage = RPG_DAMAGE_MAXIMUM;
    }
    if ((rpg_enemy_action & 0x600000) && rpg_club_move_available == 0 && rpg_damage != 0) {
        rpg_enemy_hit_points -= 1500;
    }
}
