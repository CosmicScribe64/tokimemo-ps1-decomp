/* girl_action_ayako.c: src/ovl/RPG_BAT/80151C10.c:7: girl 3 action; body 8: switch (rpg_girl_channel.unk_04) {; 11: rpg_message_show((s32) rpg_text_girl_pointers, 1, 0x5A); partner-id dispatch src/ovl/RPG_BAT/801439E0.c:24. Original object 0x80151C10-0x80151FC0 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_actor_set_animation /* implicit int view: the call's $v0 moves the selector of rpg_girl_action_ayako to $v1 (T-17102) */
#include "common.h"
#include "ovl/RPG_BAT.h"

s32 rpg_actor_set_animation();

void rpg_girl_action_ayako(void) {
    switch (rpg_girl_channel.unk_04) {
    case 0x0:
        strcpy(rpg_text_girl_0, "　　　　　ポコポコ乱れ撃ち　　　　　");
        rpg_message_show((s32) rpg_text_girl_pointers, 1, 0x5A);
        rpg_effects_select_asset_set(0xFF);
        rpg_scratch = rpg_rand(4) + 3;
        rpg_move_value_0 = 0;
        rpg_girl_step_next();
        return;
    case 0x1:
        rpg_girl_wait_msg_step(1);
        return;
    case 0x2:
    case 0x3:
    case 0x4:
    case 0x5:
    case 0x6:
    case 0x7:
    case 0x8:
    case 0x9:
    case 0xA:
    case 0xB:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0xF:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
        if (!(rpg_girl_channel.unk_04 & 1)) {
            if (rpg_girl_channel.unk_0C == 0) {
                rpg_ayako_shot_pick();
                rpg_actor_set_animation(0x1D, 5, 1, 1);
                rpg_move_timer = rpg_rand(0x38) - 0x30;
                rpg_girl_channel.unk_0C += 1;
                return;
            }
            rpg_girl_move_linear(0x1D, 0, rpg_move_value_0, -0xEF, rpg_move_timer, 0x1E);
            return;
        }
        /* FAKE: u32 view of the s32 counter; plain rpg_scratch hoists its lui above the sub and renumbers the temporaries. Real type unknown. T-17102 */
        if (*(u32 *)&rpg_scratch == ((rpg_girl_channel.unk_04 - 3) / 2)) {
            rpg_ayako_shot_pick();
            rpg_actor_set_animation(0x1D, 6, 1, 1);
            rpg_girl_channel.unk_04 = 0x32;
            return;
        }
        if (rpg_girl_channel.unk_0C == 0) {
            rpg_ayako_shot_pick();
            rpg_actor_set_animation(0x1D, 6, 1, 1);
            rpg_move_value_0 = rpg_rand(0x38) - 0x30;
            rpg_girl_channel.unk_0C += 1;
            return;
        }
        rpg_girl_move_linear(0x1D, -0xEF, rpg_move_timer, 0, rpg_move_value_0, 0x1E);
        return;
    case 0x32:
        rpg_girl_move_linear(0x1D, -0xEF, rpg_move_timer, 0, 0, 0x1E);
        return;
    case 0x33:
        rpg_actor_set_animation(0x1D, 0, 1, 1);
        rpg_girl_damage_roll();
        if (rpg_enemy_action & 0x100000) {
            rpg_apply_damage(0, 0, rpg_damage, 1);
            rpg_apply_damage(1, 1, rpg_damage_slot1, 1);
            rpg_apply_damage(2, 2, rpg_damage_slot2, 1);
            rpg_apply_damage(3, 0x21, rpg_damage_to_player_extra, 1);
        } else {
            rpg_apply_damage(0, 3, rpg_damage, 1);
            rpg_apply_damage(1, 0x21, rpg_damage_to_player_extra, 1);
        }
        rpg_girl_step_next();
        return;
    case 0x34:
        rpg_girl_wait_step(0x3C);
        return;
    case 0x35:
        rpg_girl_action_end();
        /* fallthrough */
    default:
        return;
    }
}

void rpg_ayako_shot_pick(void) {
    s32 temp_v0;

    temp_v0 = rpg_rand(3);
    switch (temp_v0) {
    case 0:
        rpg_se_play_latched_5xx(0x50E, 1, 0);
        return;
    case 1:
        rpg_se_play_latched_5xx(0x50F, 1, 0);
        return;
    case 2:
        rpg_se_play_latched_5xx(0x510, 1, 0);
        return;
    }
}
