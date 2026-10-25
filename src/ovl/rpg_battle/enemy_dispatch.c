/* enemy_dispatch.c: src/ovl/RPG_BAT/8013C890.c:5: enemy action dispatch; body 6: if (rpg_enemy_action_threshold() < rpg_enemy_timer && !(rpg_player_action & 0xFF) && !(rpg_enemy_action & 0xFF) && !(rpg_over & 0xFF) &&; 8: rpg_enemy_delay_extra = rpg_rand2(0x14, 0x28);. Original object 0x8013C890-0x8013CC90 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_enemy_update /* defined s32 without a return: v0 live at the exit keeps the lw ra delay slot (T-17113); callers see void */
#include "common.h"
#include "ovl/rpg_battle.h"

s32 rpg_enemy_update(void) {
    if (rpg_enemy_action_threshold() < rpg_enemy_timer && !(rpg_player_action & 0xFF) && !(rpg_enemy_action & 0xFF) && !(rpg_over & 0xFF) &&
        !(rpg_girl_action & 0xFF)) {
        rpg_enemy_delay_extra = rpg_rand2(0x14, 0x28);
        rpg_enemy_timer = 0;
        rpg_enemy_action |= rpg_enemy_pick_action();
        if (!(rpg_enemy_action & 0xFF)) {
            rpg_enemy_timer = rpg_rand2(0x3C, 0x5A);
        }
    }
    if (rpg_enemy_action & 0xFF) {
        switch (rpg_enemy_action & 0xFFFF0000) {
        case 0x10000:
            switch (rpg_enemy_action & 0xFFFF) {
            case 1:
                rpg_enemy_trip_attack();
                break;
            case 2:
                rpg_enemy_snake_finale();
                break;
            }
            break;
        case 0x20000:
            switch (rpg_enemy_action & 0xFFFF) {
            case 1:
                rpg_enemy_trip_attack();
                break;
            case 2:
                rpg_enemy_deer_finale();
                break;
            }
            break;
        case 0x40000:
            switch (rpg_enemy_action & 0xFFFF) {
            case 1:
                rpg_enemy_trip_attack();
                break;
            case 2:
                rpg_enemy_bear_finale();
                break;
            }
            break;
        case 0x100000:
            switch (rpg_enemy_action & 0xFFFF) {
            case 1:
                rpg_enemy_delinquent_taunt();
                break;
            case 2:
                rpg_enemy_delinquent_punch();
                break;
            case 3:
                rpg_enemy_delinquent_punch_miss();
                break;
            case 4:
                rpg_enemy_delinquent_punch_ally();
                break;
            case 5:
                rpg_enemy_delinquent_punch_knockdown();
                break;
            case 6:
                rpg_enemy_delinquent_punch_cover();
                break;
            case 7:
                rpg_enemy_player_defeated();
                break;
            }
            break;
        case 0x200000:
        case 0x400000:
            switch (rpg_enemy_action & 0xFFFF) {
            case 1:
                rpg_enemy_bancho_taunt();
                break;
            case 2:
                rpg_enemy_bancho_attack();
                break;
            case 3:
                rpg_enemy_bancho_phase_two();
                break;
            case 4:
                rpg_enemy_bancho_attack_powered();
                break;
            case 5:
                rpg_enemy_player_defeated_bancho();
                break;
            }
            break;
        case 0x8000000:
            switch (rpg_enemy_action & 0xFFFF) {
            case 1:
                rpg_enemy_robot_attack();
                break;
            case 2:
                rpg_enemy_robot_intro();
                break;
            }
            break;
        case 0x80000000:
            switch (rpg_enemy_action & 0xFFFF) {
            case 1:
                rpg_enemy_alien_attack_1();
                break;
            case 2:
                rpg_enemy_alien_attack_2();
                break;
            case 3:
                rpg_enemy_alien_retreat();
                break;
            case 4:
                rpg_enemy_alien_chatter();
                break;
            }
            break;
        }
    }
}
