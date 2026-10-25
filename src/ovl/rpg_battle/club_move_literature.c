/* club_move_literature.c: src/ovl/RPG_BAT/801368B0.c:4: club 0 special move; body 5: switch (rpg_player_step) {; 8: rpg_message_show(&rpg_text_player_pointers, 1, 0x5A); club-index dispatch src/ovl/rpg_battle/player_commands_and_club_moves.c:96. Original object 0x801368B0-0x80136D50 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/rpg_battle.h"

void rpg_club_move_literature(void) {
    switch (rpg_player_step) {
    case 0:
        strcpy(rpg_text_player_0, "　　　　　　メガトン辞書　　　　　　");
        rpg_message_show(&rpg_text_player_pointers, 1, 0x5A);
        rpg_pick_target();
        rpg_set_target_position(rpg_enemy_slot);
        rpg_player_damage_roll();
        rpg_player_step_next();
        rpg_player_step_next();
        break;
    case 2:
        rpg_move_approach();
        break;
    case 3:
        rpg_actor_set_animation(0x34, 3, 5, 1);
        rpg_player_step_next();
        break;
    case 4:
        rpg_player_wait_animation_step(0x34);
        if (D_80121438 == 1) {
            rpg_se_play_latched_6xx(0x500, 0, 5);
        }
        if (D_80121438 == 9) {
            rpg_se_play_latched_6xx(0x501, 0, 1);
        }
        if (D_80121438 == 0xB) {
            rpg_se_play_latched_6xx(0x502, 0, 2);
        }
        break;
    case 5:
        rpg_actor_set_animation(0x34, 2, 1, 1);
        rpg_player_step_next();
        break;
    case 6:
        rpg_player_wait_step(0x3C);
        break;
    case 7:
        rpg_literature_step_7();
        break;
    case 8:
        rpg_literature_step_8();
        break;
    case 9:
        rpg_literature_step_9();
        break;
    case 10:
        rpg_apply_damage_single();
        rpg_player_step_next();
        break;
    case 11:
        rpg_player_wait_step(0x14);
        break;
    case 12:
        rpg_move_return();
        break;
    case 13:
        rpg_player_action_end();
        break;
    }
}

void rpg_literature_step_7(void) {
    switch (rpg_player_channel.unk_00) {
    case 0:
        if (rpg_enemy_action & 0x100000) {
            D_8015EC6C = rpg_target_y - 0xA0;
        } else {
            D_8015EC6C = rpg_target_y - 0x78;
        }
        /* FAKE: the address-taken view keeps the selector load in place ($v1, after the D_8015EC6C store); the plain switch (rpg_enemy_slot) puts it in $v0. Real source unknown. T-17114 */
        switch (*(s32 *)&rpg_enemy_slot) {
        case 0:
            D_80121685 = 4;
            break;
        case 1:
            D_801215FD = 4;
            D_80121685 = 4;
            break;
        }
        rpg_actor_set_position(0x2B, rpg_target_x, -0xC4);
        rpg_actor_set_animation_alt(0x2B, 4, 1, 1);
        rpg_se_play_latched_6xx(0x503, 0, 3);
        rpg_player_substep_next();
        break;
    case 1:
        D_801211E6 += 4;
        if (D_8015EC6C < D_801211E6) {
            D_801211E6 = D_8015EC6C;
            rpg_player_step_next();
        }
        break;
    }
}

void rpg_literature_step_8(void) {
    rpg_se_play_latched_6xx(0x504, 0, 4);
    if (rpg_player_channel.unk_04 < 0x14) {
        if (!(rpg_player_channel.unk_04 & 1)) {
            rpg_effects_shift(0, 4);
        } else {
            rpg_effects_shift(0, -4);
        }
    }
    if (rpg_player_channel.unk_04 == 0xF) {
        rpg_actor_set_position(0x2A, rpg_target_x, D_8015EC6C + 0x78);
        rpg_actor_set_animation_alt(0x2A, 5, 5, 1);
    }
    rpg_player_channel.unk_04 += 1;
    if (!(D_8012117A & 1)) {
        rpg_actor_set_animation_alt(0x2A, 0xFF, 1, 0);
        rpg_player_step_next();
    }
}
void rpg_literature_step_9(void) {
    if (!(rpg_player_timer & 1)) {
        rpg_actor_set_animation_alt(0x2B, 0xFF, 1, 0);
    } else {
        rpg_actor_set_animation_alt(0x2B, 0xFF, 1, 1);
    }
    if (++rpg_player_timer >= 0x3D) {
        rpg_player_step_next();
    }
}
