/* club_move_basketball.c: src/ovl/RPG_BAT/801321D0.c:4: club 10 special move; body 5: if ((s32) rpg_player_step >= 3 && (s32) rpg_player_step < 6) {; 6: if (D_80121438 == 0 && D_80121428 == 0) {; club-index dispatch src/ovl/rpg_battle/player_commands_and_club_moves.c:96. Original object 0x801321D0-0x80132FE0 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/rpg_battle.h"

void rpg_club_move_basketball(void) {
    if ((s32) rpg_player_step >= 3 && (s32) rpg_player_step < 6) {
        if (D_80121438 == 0 && D_80121428 == 0) {
            rpg_se_play_latched_6xx(0x503, 1, 0);
        }
    }
    switch (rpg_player_step) {
    case 0:
        strcpy(rpg_text_player_0, "　　　　　ナイアガラダンク　　　　　");
        rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x5A);
        rpg_pick_target();
        rpg_set_target_position(rpg_enemy_slot);
        rpg_player_damage_roll();
        rpg_player_step_next();
        rpg_player_step_next();
        return;
    case 2:
        rpg_move_approach();
        return;
    case 3:
        rpg_basketball_step_3();
        return;
    case 4:
        rpg_basketball_step_4();
        return;
    case 5:
        rpg_basketball_step_5();
        return;
    case 6:
        rpg_basketball_step_6();
        return;
    case 7:
        rpg_actor_set_animation_alt(0x34, 2, 1, 1);
        rpg_player_wait_step(0x14);
        return;
    case 8:
        rpg_move_return();
        return;
    case 9:
        rpg_player_action_end();
        break;
    }
}


void rpg_basketball_step_6(void) {
    RpgMoveMotion *p = (RpgMoveMotion *)&rpg_motion;

    switch (rpg_player_channel.unk_00) {
    case 0:
        p->dx = -0x30;
        p->dy = -0x140;
        rpg_actor_set_animation_alt(0x34, 5, 5, 1);
        rpg_se_play_latched_6xx(0x505, 1, 0);
        rpg_player_substep_next();
    default:
        break;
    case 1:
        if (D_80121438 == 2) {
            rpg_player_substep_next();
        }
        break;
    case 2:
        rpg_actor_set_position(0x34, ((s32) (p->dx * rpg_player_channel.unk_08) / 1000) + 0x110, ((s32) (p->dy * rpg_player_channel.unk_08) / 1000) + 0x58);
        rpg_player_channel.unk_08 += 0x14;
        if (rpg_player_channel.unk_08 >= 0x3E9) {
            rpg_actor_set_animation_alt(0x34, 6, 1, 1);
            rpg_basketball_set_target_rect();
            p->dx = p->rect.param_2 - p->rect.param_0;
            p->dy = p->rect.param_3 - p->rect.param_1;
            rpg_player_substep_next();
        }
        break;
    case 3:
        rpg_actor_set_position(0x34, ((s32) (p->dx * rpg_player_channel.unk_08) / 1000) + p->rect.param_0, ((s32) (p->dy * rpg_player_channel.unk_08) / 1000) + p->rect.param_1);
        rpg_player_channel.unk_08 += 0x1E;
        if ((p->rect.param_5 - 0x10) < (p->rect.param_1 + ((s32) (p->dy * rpg_player_channel.unk_08) / 1000))) {
            rpg_se_play_latched_6xx(0x504, 0, 1);
            rpg_basketball_spark_start(p->rect.param_4, p->rect.param_5);
            rpg_basketball_spark_burst(p->rect.param_4, p->rect.param_5);
            rpg_actor_set_animation_alt(0x34, 0xD, 1, 1);
        }
        if (rpg_player_channel.unk_08 >= 0x3E9) {
            if (rpg_enemy_slot == 1) {
                D_801215FD = 4;
                D_80121685 = 4;
            }
            rpg_actor_set_animation_alt(0x34, 7, 5, 1);
            D_80121438 = 1;
            rpg_player_substep_next();
        }
        break;
    case 4:
        rpg_basketball_spark_burst(p->rect.param_4, p->rect.param_5);
        rpg_player_wait_substep(0x1E);
        break;
    case 5:
        rpg_apply_damage_single();
        rpg_player_substep_next();
        break;
    case 6:
        rpg_basketball_spark_burst(p->rect.param_4, p->rect.param_5);
        rpg_actor_set_position(0x34, ((s32) ((0xF0 - p->rect.param_2) * rpg_player_channel.unk_08) / 1000) + p->rect.param_2, (p->rect.param_3 + ((s32) ((0x58 - p->rect.param_3) * rpg_player_channel.unk_08) / 1000)) - (rsin((s32) (rpg_player_channel.unk_08 << 0xB) / 1000) >> 7));
        rpg_player_channel.unk_08 += 0x14;
        rpg_actor_set_animation_alt(0x34, 8, 1, 1);
        if (rpg_player_channel.unk_08 >= 0x3E9) {
            rpg_se_play_latched_6xx(0x502, 1, 0);
            rpg_player_step_next();
        }
        break;
    }
}


void rpg_basketball_spark_burst(s32 origin_x, s32 origin_y) {
    s32 i;
    RpgSparkDirections dirs;

    dirs = rpg_spark_directions;
    for (i = 0; i < 8; i++) {
        rpg_actor_set_position(i + 0x28, (dirs.offsets[i * 2] * rpg_player_channel.unk_04) / 1000 + origin_x, origin_y + (dirs.offsets[i * 2 + 1] * rpg_player_channel.unk_04) / 1000 - (rsin((rpg_player_channel.unk_04 << 11) / 1000) >> 7));
    }
    rpg_player_channel.unk_04 += 0x19;
    if (rpg_player_channel.unk_04 > 1000) {
        for (i = 0; i < 8; i++) {
            rpg_actor_set_animation_alt(i + 0x28, 9, 1, 0);
        }
    }
}

void rpg_basketball_spark_start(s32 x, s32 y) {
    s32 i;

    for (i = 0; i < 8; i++) {
        rpg_actor_set_position(i + 0x28, x, y);
        rpg_actor_set_animation_alt(i + 0x28, 9, 1, 1);
        ((Actor *)actor_pool)[i + 136].unk_01 = 4;
    }
}

s32 rpg_basketball_set_target_rect(void) {
    switch (rpg_enemy_slot) {
    case 0:
        (&rpg_move_params.param_2)[0] = 0x84;
        (&rpg_move_params.param_3)[0] = 0x6C;
        rpg_move_params.param_4 = 0x73;
        rpg_move_params.param_5 = 0x57;
        rpg_move_params.param_0 = rpg_move_params.param_2 + 0x30;
        rpg_move_params.param_1 = rpg_move_params.param_3 - 0xB0;
        break;
    case 1:
        (&rpg_move_params.param_2)[0] = 0x4C;
        (&rpg_move_params.param_3)[0] = 0x5C;
        rpg_move_params.param_4 = 0x3B;
        rpg_move_params.param_5 = 0x47;
        rpg_move_params.param_0 = rpg_move_params.param_2 + 0x30;
        rpg_move_params.param_1 = rpg_move_params.param_3 - 0xB0;
        break;
    case 2:
        (&rpg_move_params.param_2)[0] = 0x44;
        (&rpg_move_params.param_3)[0] = 0x8C;
        rpg_move_params.param_4 = 0x33;
        rpg_move_params.param_5 = 0x77;
        rpg_move_params.param_0 = rpg_move_params.param_2 + 0x30;
        rpg_move_params.param_1 = rpg_move_params.param_3 - 0xB0;
        break;
    case 3:
        (&rpg_move_params.param_2)[0] = 0x40;
        (&rpg_move_params.param_3)[0] = 0x8C;
        rpg_move_params.param_4 = 0x48;
        rpg_move_params.param_5 = 0x27;
        rpg_move_params.param_0 = rpg_move_params.param_2 + 0x30;
        rpg_move_params.param_1 = rpg_move_params.param_3 - 0xB0;
        break;
    case 4:
        (&rpg_move_params.param_2)[0] = 0x4A;
        (&rpg_move_params.param_3)[0] = 0x8C;
        rpg_move_params.param_4 = 0x49;
        rpg_move_params.param_5 = 0x3A;
        rpg_move_params.param_0 = rpg_move_params.param_2 + 0x30;
        rpg_move_params.param_1 = rpg_move_params.param_3 - 0xB0;
        break;
    }
}

void rpg_basketball_step_3(void) {
    if (rpg_player_channel.unk_04 == 0) {
        D_801215FD = 4;
        rpg_player_channel.unk_04 += 4;
        rpg_actor_set_position(0x34, 0xF0 - rpg_player_channel.unk_04, 0x58);
        rpg_actor_set_position(0x33, 0xF0 - rpg_player_channel.unk_04, 0x58);
        rpg_actor_set_animation_alt(0x34, 3, 1, 1);
        rpg_actor_set_animation_alt(0x33, 4, 1, 1);
    }
    rpg_actor_set_position(0x34, 0xF0 - rpg_player_channel.unk_04, 0x58);
    rpg_actor_set_position(0x33, 0xF0 - rpg_player_channel.unk_04, 0x58);
    D_80121426 = 0x82;
    rpg_player_channel.unk_04 += 6;
    if (rpg_player_channel.unk_04 >= 0x150) {
        D_801215FD = 5;
        rpg_player_step_next();
    }
}

void rpg_basketball_step_4(void) {
    if (rpg_player_channel.unk_04 == 0) {
        D_801213DD = 4;
        D_80121421 = 4;
        rpg_actor_set_position(0x34, rpg_player_channel.unk_04 - 0x10, 0x8A);
        rpg_actor_set_position(0x33, rpg_player_channel.unk_04 - 0x10, 0x8A);
        rpg_actor_set_animation_alt(0x34, 0xB, 1, 1);
        rpg_actor_set_animation_alt(0x33, 0xC, 1, 1);
    }
    rpg_actor_set_position(0x34, rpg_player_channel.unk_04 - 0x10, 0x8A);
    rpg_actor_set_position(0x33, rpg_player_channel.unk_04 - 0x10, 0x8A);
    rpg_player_channel.unk_04 += 6;
    if (rpg_player_channel.unk_04 >= 0x186) {
        D_801213DD = 5;
        D_80121421 = 5;
        rpg_player_step_next();
    }
}

void rpg_basketball_step_5(void) {
    s32 temp_v0_2;


    if (rpg_player_channel.unk_04 == 0) {
        rpg_actor_set_position(0x34, 0x170 - rpg_player_channel.unk_04, 0x58);
        rpg_actor_set_position(0x33, 0x170 - rpg_player_channel.unk_04, 0x58);
        rpg_actor_set_animation_alt(0x34, 3, 1, 1);
        rpg_actor_set_animation_alt(0x33, 4, 1, 1);
    }
    rpg_actor_set_position(0x34, 0x170 - rpg_player_channel.unk_04, 0x58);
    rpg_actor_set_position(0x33, 0x170 - rpg_player_channel.unk_04, 0x58);
    temp_v0_2 = rpg_player_channel.unk_04 + 6;
    rpg_player_channel.unk_04 = temp_v0_2;
    if (temp_v0_2 >= 0x61) {
        D_80121426 = 1;
        rpg_actor_set_animation_alt(0x33, 4, 1, 0);
        rpg_player_step_next();
    }
}
