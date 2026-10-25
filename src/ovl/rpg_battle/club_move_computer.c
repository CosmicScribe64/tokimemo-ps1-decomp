/* club_move_computer.c: src/ovl/RPG_BAT/8013A650.c:4: club 3 special move; body 7: switch (rpg_player_step) {; 10: rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x5A); club-index dispatch src/ovl/rpg_battle/player_commands_and_club_moves.c:96. Original object 0x8013A650-0x8013AD50 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/rpg_battle.h"

void rpg_club_move_computer(void) {
    s32 temp_v0;

    switch (rpg_player_step) {
    case 0:
        strcpy(rpg_text_player_0, "　　　　　サイバーファング　　　　　");
        rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x5A);
        rpg_sparks_park();
        rpg_pick_target();
        rpg_set_target_position_alt(rpg_enemy_slot);
        rpg_player_damage_roll();
        rpg_player_step_next();
        rpg_player_step_next();
    default:
        break;
    case 2:
        rpg_move_approach();
        break;
    case 3:
        rpg_actor_set_animation(0x34, 3, 5, 1);
        rpg_se_play_latched_6xx(0x505, 1, 0);
        rpg_player_step_next();
        break;
    case 4:
        rpg_player_wait_animation_step(0x34);
        break;
    case 5:
        rpg_actor_set_animation(0x34, 3, 5, 1);
        rpg_player_step_next();
        break;
    case 6:
        rpg_player_wait_animation_step(0x34);
        break;
    case 7:
        rpg_actor_set_animation(0x34, 3, 5, 1);
        rpg_player_step_next();
        break;
    case 8:
        rpg_player_wait_animation_step(0x34);
        break;
    case 9:
        rpg_actor_set_animation(0x34, 4, 1, 1);
        rpg_actor_set_animation(0x2F, 5, 1, 1);
        D_80121238 = 0x71000000;
        D_8012127C = 0x71000000;
        rpg_actor_set_animation(0x2C, 6, 1, 1);
        rpg_se_play_latched_6xx(0x50D, 0, 4);
        rpg_player_step_next();
        break;
    case 10:
        rpg_player_wait_step(6);
        break;
    case 11:
        rpg_actor_set_animation(0x2D, 7, 1, 1);
        rpg_player_step_next();
        break;
    case 12:
        rpg_player_wait_step(0x38);
        break;
    case 13:
        rpg_computer_step_13();
        break;
    case 14:
        rpg_player_wait_step(0x28);
        break;
    case 15:
        rpg_computer_step_15();
        break;
    case 16:
        rpg_actor_set_position(0x2E, rpg_target_x, rpg_target_y);
        rpg_actor_set_animation_alt(0x2E, 0xB, 5, 1);
        rpg_se_play_latched_6xx(0x508, 0, 3);
        rpg_player_step_next();
        break;
    case 17:
        rpg_fade_set_level(0xFF);
        temp_v0 = rpg_player_channel.unk_04 + 1;
        rpg_player_channel.unk_04 = temp_v0;
        if (temp_v0 >= 5) {
            rpg_fade_set_level(0x80);
            rpg_player_step_next();
        }
        break;
    case 18:
        rpg_player_wait_animation_step(0x2E);
        break;
    case 19:
        rpg_apply_damage_single();
        rpg_actor_set_animation_alt(0x2E, 0xFF, 1, 0);
        rpg_player_step_next();
        break;
    case 20:
        rpg_player_wait_step(0x14);
        break;
    case 21:
        rpg_move_return();
        break;
    case 22:
        rpg_player_action_end();
        break;
    }
}

void rpg_computer_step_13(void) {
    rpg_actor_set_animation(0x2E, 8, 1, 1);
    D_801212CB = 0x1B;
    D_80121685 = 4;
    if (rpg_player_channel.unk_04 < 0x3C) {
        load_palette(rpg_asset_palette_5, 0x1B, 1, 4, rpg_player_channel.unk_04 / 10);
    }
    if (rpg_player_channel.unk_04 >= 0x3C) {
        rpg_se_play_latched_6xx(0x506, 0, 1);
        load_palette(rpg_asset_palette_5, 0x1B, 1, 4, (rpg_player_channel.unk_04 - 0x3C) % 8 / 2 + 6);
    }
    if (rpg_player_channel.unk_04 >= 0x44) {
        load_palette(rpg_asset_palette_5, 0x1B, 1, 4, (rpg_player_channel.unk_04 - 0x44) % 8 / 2 + 6);
    }
    if (rpg_player_channel.unk_04 >= 0x4C) {
        load_palette(rpg_asset_palette_5, 0x1B, 1, 4, (rpg_player_channel.unk_04 - 0x4C) % 8 / 2 + 6);
    }
    if (rpg_player_channel.unk_04 >= 0x54) {
        load_palette(rpg_asset_palette_5, 0x1B, 1, 4, (rpg_player_channel.unk_04 - 0x54) % 8 / 2 + 6);
    }
    if (rpg_player_channel.unk_04 >= 0x5C) {
        load_palette(rpg_asset_palette_5, 0x1B, 1, 4, (rpg_player_channel.unk_04 - 0x5C) % 12 / 2 + 0xA);
    }
    rpg_player_channel.unk_04 = rpg_player_channel.unk_04 + 1;
    if (rpg_player_channel.unk_04 >= 0x69) {
        D_801212CB = 0x1D;
        rpg_actor_set_animation(0x2E, 9, 1, 1);
        rpg_actor_set_animation(0x2F, 0xFF, 1, 0);
        rpg_actor_set_animation(0x2C, 0xFF, 1, 0);
        rpg_actor_set_animation(0x2D, 0xFF, 1, 0);
        D_80121238 = 0x41000000;
        D_8012127C = 0x41000000;
        rpg_player_step_next();
    }
}

void rpg_computer_step_15(void) {
    s32 temp_v0;

    rpg_se_play_latched_6xx(0x507, 0, 2);
    rpg_actor_set_animation(0x2E, 0xA, 1, 1);
    rpg_actor_set_position(0x2E, -rpg_player_channel.unk_04 * 8, 0);
    temp_v0 = rpg_player_channel.unk_04 + 1;
    rpg_player_channel.unk_04 = temp_v0;
    if (temp_v0 >= 0x27) {
        D_80121685 = 5;
        rpg_actor_set_animation(0x2E, 0xA, 1, 0);
        rpg_player_step_next();
    }
}
