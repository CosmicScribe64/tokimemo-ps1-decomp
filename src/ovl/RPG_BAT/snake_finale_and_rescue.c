/* snake_finale_and_rescue.c: src/ovl/RPG_BAT/80145CC0.c:11: snake finale; body 12: switch (rpg_enemy_step) {; 15: rpg_actor_set_position(0x38, 0, 0);; enemy identity from snake name-label creation src/ovl/RPG_BAT/labels_intro_and_wave_transition.c:24; encounter 0x10000 finale dispatch src/ovl/RPG_BAT/8013C890.c:17. Original object 0x80145CC0-0x801468B0 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_actor_set_animation /* implicit int view: the calls' $v0 moves the rpg_enemy_timer_a timer to $v1 (T-17101) */
#define RPG_BAT_OVERRIDE_rpg_actor_set_position /* implicit int view, same reason (T-17101) */
#define RPG_BAT_OVERRIDE_rpg_se_play_latched_5xx /* implicit int view: the call's $v0 moves the rpg_enemy_substep selector of rpg_snake_finale_rescuer_strike to $v1 (T-17101) */
#include "common.h"
#include "ovl/RPG_BAT.h"

s32 rpg_actor_set_animation();
s32 rpg_actor_set_position();
s32 rpg_se_play_latched_5xx();

void rpg_enemy_snake_finale(void) {
    switch (rpg_enemy_step) {
    case 0:
        strcpy(rpg_text_enemy_0, "　　　　アストラルファング　　　　");
        rpg_actor_set_position(0x38, 0, 0);
        D_80121531 = 4;
        rpg_actor_set_animation(0x38, 1, 1, 1);
        rpg_enemy_step_next();
        break;
    case 1:
        rpg_enemy_wait_step(0x3C);
        break;
    case 2:
        rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x3C);
        rpg_actor_set_animation(0x38, 0xFF, 1, 0);
        rpg_enemy_step_next();
        break;
    case 3:
        rpg_enemy_wait_step(0x14);
        break;
    case 4:
        rpg_actor_set_animation(0x38, 2, 5, 1);
        rpg_se_play_latched_5xx(0x507, 1, 0);
        rpg_enemy_step_next();
        break;
    case 5:
        rpg_enemy_timer_a += 3;
        rpg_actor_set_position(0x38, (s32) (rpg_enemy_timer_a * 0x90) / 100, 0);
        if (rpg_enemy_timer_a >= 0x65) {
            rpg_se_play_latched_5xx(0x506, 1, 0);
            rpg_actor_set_position(0x38, 0x90, 0);
            rpg_actor_set_animation(0x38, 3, 1, 1);
            rpg_enemy_step_next();
        }
        break;
    case 6:
        rpg_enemy_wait_step(0xA);
        break;
    case 7:
        rpg_apply_damage(0, 0x21, rpg_player_hit_points - 3, 1);
        rpg_actor_set_animation(0x38, 0xFF, 1, 0);
        rpg_actor_set_animation(0x30, 4, 1, 1);
        rpg_player_action = RPG_PLAYER_DOWN;
        rpg_enemy_step_next();
        break;
    case 8:
        rpg_enemy_wait_step(0x96);
        break;
    case 9:
        strcpy(rpg_text_enemy_0, game_state.player_family_name);
        strcat(rpg_text_enemy_0, "「だめだ！　やられてしまう！");
        rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x78);
        rpg_enemy_step_next();
        break;
    case 10:
        rpg_enemy_wait_msg_step(1);
        break;
    case 11:
        strcpy(rpg_text_enemy_0, "？？？「ピキィー！");
        rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x5A);
        rpg_enemy_step_next();
        break;
    case 12:
        rpg_enemy_wait_step(0xA);
        break;
    case 13:
        rpg_se_play_latched_5xx(0x50B, 1, 0);
        rpg_enemy_step_next();
        break;
    case 14:
        rpg_enemy_wait_msg_step(1);
        break;
    case 15:
        strcpy(rpg_text_enemy_0, game_state.player_family_name);
        strcat(rpg_text_enemy_0, "「なっ、なんだ！");
        rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x5A);
        rpg_enemy_step_next();
        break;
    case 16:
        rpg_enemy_wait_msg_step(1);
        break;
    case 17:
        rpg_enemy_timer_a += 1;
        if (rpg_enemy_timer_a < 0x3D) {

            if (!(rpg_enemy_timer_a & 1)) {
                rpg_actor_set_animation(0x30, 0xFF, 1, 0);
                if (rpg_girl_id != 0xE) {
                    rpg_actor_set_animation(0x1D, 0xFF, 1, 0);
                }
            } else {
                rpg_actor_set_animation(0x30, 0xFF, 1, 1);
                if (rpg_girl_id != 0xE) {
                    rpg_actor_set_animation(0x1D, 0xFF, 1, 1);
                }
            }
        }
        rpg_actor_set_position(0x28, 0, 0);
        if (rpg_enemy_timer_a >= 0x3D) {

            if (!(rpg_enemy_timer_a & 1)) {
                rpg_actor_set_animation(0x28, 0, 1, 1);
            } else {
                rpg_actor_set_animation(0x28, 0, 1, 0);
            }
        }
        if (rpg_enemy_timer_a >= 0x78) {
            rpg_enemy_step_next();
        }
        break;
    case 18:
        strcpy(rpg_text_enemy_0, "正義のマングース参上！");
        strcpy(rpg_text_enemy_1, "　　　　　マングース斬鉄剣　　　　　");
        rpg_message_show((s32) rpg_text_enemy_pointers, 2, 0x5A);
        rpg_guest_value ^= 0x10000000;
        rpg_enemy_step_next();
        break;
    case 19:
        rpg_enemy_wait_msg_step(1);
        break;
    case 20:
        rpg_actor_set_animation(0x28, 1, 5, 1);
        rpg_se_play_latched_5xx(0x509, 1, 0);
        rpg_enemy_step_next();
        break;
    case 21:
        if (rpg_actor_is_idle(0x28) != 0) {
            rpg_actor_set_animation(0x28, 2, 1, 1);
            rpg_se_play_latched_5xx(0x50A, 1, 0);
            rpg_enemy_step_next();
        }
        break;
    case 22:
        rpg_enemy_timer_a += 1;
        rpg_actor_set_position(0x28, (-rpg_enemy_timer_a) << 3, 0);
        if ((rpg_enemy_timer_a == 0x13) || (rpg_enemy_timer_a == 0x16) || (rpg_enemy_timer_a == 0x19) || (rpg_enemy_timer_a == 0x1C)) {
            rpg_fade_set_level(0xFF);
        } else {
            rpg_fade_set_level(0x80);
        }
        if (rpg_enemy_timer_a >= 0x26) {
            rpg_enemy_step_next();
        }
        break;
    case 23:
        rpg_enemy_wait_step(0x1E);
        break;
    case 24:
        rpg_actor_set_position(0x28, -0xC0, -0xA0);
        rpg_actor_set_animation(0x28, 3, 1, 1);
        rpg_se_play_latched_5xx(0x50A, 1, 0);
        rpg_enemy_step_next();
        break;
    case 25:
        rpg_enemy_timer_a += 1;
        rpg_actor_set_position(0x28, -0xC0, (rpg_enemy_timer_a * 8) - 0xA0);
        if ((rpg_enemy_timer_a == 0xD) || (rpg_enemy_timer_a == 0x10) || (rpg_enemy_timer_a == 0x13) || (rpg_enemy_timer_a == 0x16)) {
            rpg_fade_set_level(0xFF);
        } else {
            rpg_fade_set_level(0x80);
        }
        if (rpg_enemy_timer_a >= 0x1F) {
            rpg_enemy_step_next();
        }
        break;
    case 26:
        rpg_enemy_wait_step(0x3C);
        break;
    case 27:
        rpg_snake_finale_rescuer_strike();
        break;
    case 28:
        rpg_enemy_wait_step(0x1E);
        break;
    case 29:
        rpg_actor_set_animation(0x30, 0, 1, 1);
        if (rpg_girl_id != 0xE) {
            rpg_actor_set_animation(0x1D, 0, 1, 1);
        }
        k_disp_switch(rpg_label_enemy, 0);
        strcpy(rpg_text_result_0, "戦闘に勝った！！");
        strcpy(rpg_text_result_1, "ありがとう、正義のマングース");
        strcpy(rpg_text_result_2, "経験値　：８２０ＥＸＰ");
        strcpy(rpg_text_result_3, "お金　　：１７５ゴールド");
        rpg_trip_loot_text();
        rpg_enemy_step_next();
        break;
    case 30:
        rpg_enemy_wait_step(0x3C);
        break;
    case 31:
        audio_bank_start(1, 0x202);
        rpg_enemy_step_next();
        break;
    case 32:
        rpg_message_show((s32) rpg_text_result_pointers, rpg_result_line_count, 0x5A);
        rpg_actor_set_animation(0x30, 9, 1, 1);
        if (rpg_girl_id != 0xE) {
            rpg_actor_set_animation(0x1D, 4, 1, 1);
        }
        rpg_enemy_step_next();
        break;
    case 33:
        rpg_enemy_wait_msg_step(0);
        break;
    case 34:
        mode_stage_next();
        break;
    default:
        rpg_enemy_action_end();
        break;
    }
}

/* The caller (rpg_enemy_snake_finale) calls this without a prototype, so it is int here and returns nothing (K&R). */
s32 rpg_snake_finale_rescuer_strike(void) {
    s32 timer; /* also the loop counter of the three loops */
    s32 x_neg;
    s32 bounce;
    s32 x_pos;
    s32 y;
    s32 zero[1]; /* FAKE: a one-word array keeps the 0 in its stack word and reloads it for the later calls (a scalar is folded away); T-17101 */

    rpg_se_play_latched_5xx(0x508, 0, 1);
    switch (rpg_enemy_substep) {
    case 0:
        for (timer = 0; timer < 12; timer++) {
            ((Actor *)actor_pool)[timer + 148].frame_table = (u8 *)rpg_asset_frames_1;
            ((Actor *)actor_pool)[timer + 148].sequence_table = (u8 *)rpg_asset_animation_1;
            ((Actor *)actor_pool)[timer + 148].tpage_table = (u8 *)rpg_asset_sheet_1;
            ((Actor *)actor_pool)[timer + 148].quad_flags = 0x41000000;
            ((Actor *)actor_pool)[timer + 148].brightness = 0x80;
            ((Actor *)actor_pool)[timer + 148].unk_43 = 0x13;
            rpg_actor_set_position(timer + 0x34, 0, 0);
            rpg_actor_set_animation(timer + 0x34, timer + 5, 1, 1);
        }
        rpg_enemy_substep_next();
        break;
    case 1:
        rpg_enemy_timer_a += 1;
        timer = rpg_enemy_timer_a;
        bounce = rsin((timer << 11) / 100) >> 8;
        x_neg = (timer * -0x30) / 100;
        rpg_actor_set_position(0x34, x_neg, x_neg - bounce);
        y = x_neg - bounce;
        /* FAKE: the store of 0 rides in the argument so it lands in the call's delay slot; T-17101 */
        rpg_actor_set_position(0x35, zero[0] = 0, y);
        x_pos = (timer * 0x30) / 100;
        rpg_actor_set_position(0x36, x_pos, y);
        y = ((timer * -0x10) / 100) - bounce;
        rpg_actor_set_position(0x37, x_neg, y);
        rpg_actor_set_position(0x38, zero[0], y);
        rpg_actor_set_position(0x39, x_pos, y);
        y = ((timer * 0x10) / 100) - bounce;
        rpg_actor_set_position(0x3A, x_neg, y);
        rpg_actor_set_position(0x3B, zero[0], y);
        rpg_actor_set_position(0x3C, x_pos, y);
        rpg_actor_set_position(0x3D, (timer * 0x60) / 100, y);
        y = x_pos - bounce;
        rpg_actor_set_position(0x3E, zero[0], y);
        rpg_actor_set_position(0x3F, x_pos, y);
        if (rpg_enemy_timer_a >= 0x51) {
            if (!(rpg_enemy_timer_a & 1)) {
                for (timer = 0; timer < 12; timer++) {
                    rpg_actor_set_animation(timer + 0x34, timer + 5, 1, 1);
                }
            } else {
                for (timer = 0; timer < 12; timer++) {
                    rpg_actor_set_animation(timer + 0x34, timer + 5, 1, 0);
                }
            }
        }
        if (rpg_enemy_timer_a >= 0x65) {
            rpg_guest_value ^= 0x10000000;
            rpg_enemy_step_next();
        }
        break;
    }
}
