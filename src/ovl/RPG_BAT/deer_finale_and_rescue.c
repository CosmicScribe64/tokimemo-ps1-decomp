/* deer_finale_and_rescue.c: src/ovl/RPG_BAT/801468B0.c:99: deer finale and monk rescue; body 102: switch (rpg_enemy_step) {; 105: rpg_actor_set_position(0x38, 0, 0);; enemy identity from deer name-label creation src/ovl/RPG_BAT/labels_intro_and_wave_transition.c:28; encounter 0x20000 finale dispatch src/ovl/RPG_BAT/8013C890.c:27. Original object 0x801468B0-0x801477E0 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_actor_set_animation /* implicit int view: the call's $v0 moves the rpg_enemy_timer_a web of rpg_enemy_deer_finale to $v1 (T-17104) */
#define RPG_BAT_OVERRIDE_rpg_actor_set_position /* implicit int view, same reason (T-17104) */
#include "common.h"
#include "ovl/RPG_BAT.h"

s32 rpg_actor_set_animation();
s32 rpg_actor_set_position();

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_komuso_offer(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_enemy_0, "？？？「拙僧にまかせなされ。");
        rpg_message_show(rpg_text_enemy_pointers, 1, 0xC8);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x05000045, 0x0700004E, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(0);
        return;
    case 3:
        rpg_voice_reset();
        rpg_enemy_step_next();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_komuso_calms_deer(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_enemy_0, "虚無僧「迷える大鹿よ、");
        strcpy(rpg_text_enemy_1, "怒りを静め給え。");
        rpg_message_show(rpg_text_enemy_pointers, 2, 0xA0);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1D00009C, 0x1D0000A2, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x1D0000A3, 0x1E0000A9, 1);
        rpg_dialog_step[0] += 1;
        return;
    case 4:
        rpg_voice_wait(0);
        return;
    case 5:
        rpg_voice_reset();
        rpg_enemy_step_next();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_komuso_farewell(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_enemy_0, "虚無僧「はしたないところを");
        strcpy(rpg_text_enemy_1, "お見せしてしまった。");
        strcpy(rpg_text_enemy_2, "では、拙僧はこれにて。");
        rpg_message_show(rpg_text_enemy_pointers, 3, 0xC8);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1D0000AA, 0x1F0000AE, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x0D0000B2, 0x0D0000B7, 1);
        rpg_dialog_step[0] += 1;
        return;
    case 4:
        rpg_voice_wait(2);
        return;
    case 5:
        rpg_voice_play_once(0x1000008A, 0x05000092, 2);
        rpg_dialog_step[0] += 1;
        return;
    case 6:
        rpg_voice_wait(0);
        return;
    case 7:
        rpg_voice_reset();
        rpg_enemy_step_next();
        return;
    }
}

void rpg_enemy_deer_finale(void) {
    s32 i;

    switch (rpg_enemy_step) {
    case 0:
        strcpy(rpg_text_enemy_0, "　　　　　　スタンピート　　　　　　");
        rpg_actor_set_position(0x38, 0, 0);
        rpg_actor_set_position(0x39, 0, 0);
        ((Actor *)actor_pool)[155].unk_01 = 4;
        ((Actor *)actor_pool)[152].unk_01 = 4;
        ((Actor *)actor_pool)[153].unk_01 = 4;
        rpg_actor_set_animation(0x38, 3, 5, 1);
        rpg_actor_set_animation(0x39, 1, 5, 1);
        rpg_se_play_latched_5xx(0x508, 1, 2);
        rpg_enemy_step_next();
        break;
    case 1:
        if (!(((Actor *)actor_pool)[152].flags & 1)) {
            rpg_actor_set_animation(0x38, 0xFF, 1, 0);
        }
        rpg_enemy_wait_step(0x32);
        break;
    case 2:
        rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x3C);
        rpg_actor_set_animation(0x38, 0xFF, 1, 0);
        rpg_actor_set_animation(0x39, 0xFF, 1, 0);
        rpg_actor_set_position(0x3B, 0, 0);
        rpg_actor_set_animation(0x3B, 2, 1, 1);
        rpg_enemy_step_next();
        break;
    case 3:
        rpg_enemy_wait_msg_step(1);
        break;
    case 4:
        rpg_enemy_timer_a += 6;
        rpg_se_play_latched_5xx(0x507, 0, 1);
        rpg_actor_set_position(0x3B, (s32) (rpg_enemy_timer_a * 0x90) / 100, 0);
        if (rpg_enemy_timer_a >= 0x65) {
            rpg_actor_set_position(0x3B, 0x90, 0);
            rpg_enemy_step_next();
        }
        break;
    case 5:
        rpg_actor_set_animation(0x30, 0xA, 1, 1);
        rpg_apply_damage(0, 0x21, rpg_player_hit_points - 3, 1);
        rpg_player_action = RPG_PLAYER_DOWN;
        rpg_se_play_latched_5xx(0x505, 1, 0);
        rpg_enemy_step_next();
        break;
    case 6:
        rpg_enemy_wait_step(0x1E);
        break;
    case 7:
        rpg_enemy_timer_a += 6;
        rpg_actor_set_animation(0x3B, 0, 1, 1);
        rpg_actor_set_animation(0x30, 4, 1, 1);
        rpg_actor_set_position(0x3B, 0x90 - ((s32) (rpg_enemy_timer_a * 0x90) / 100), 0);
        if (rpg_enemy_timer_a >= 0x65) {
            rpg_actor_set_position(0x3B, 0, 0);
            rpg_enemy_step_next();
        }
        break;
    case 8:
        rpg_enemy_wait_step(0xA);
        break;
    case 9:
        strcpy(rpg_text_enemy_0, game_state.player_family_name);
        strcat(rpg_text_enemy_0, "「だめだ！　体が動かない！");
        rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x78);
        rpg_enemy_step_next();
        break;
    case 10:
        rpg_enemy_wait_msg_step(0);
        break;
    case 11:
        rpg_dialog_komuso_offer();
        break;
    case 12:
        strcpy(rpg_text_enemy_0, game_state.player_family_name);
        strcat(rpg_text_enemy_0, "「？！");
        rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x3C);
        rpg_enemy_step_next();
        break;
    case 13:
        rpg_enemy_wait_msg_step(0);
        break;
    case 14:
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
    case 15:
        rpg_guest_value ^= 0x10000000;
        strcpy(rpg_text_enemy_0, "正義の虚無僧推参！");
        rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x5A);
        rpg_enemy_step_next();
        break;
    case 16:
        rpg_enemy_wait_msg_step(1);
        break;
    case 17:
        rpg_dialog_komuso_calms_deer();
        break;
    case 18:
        strcpy(rpg_text_enemy_0, "　　　　　現世忘却の呪文　　　　　");
        rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x5A);
        rpg_enemy_step_next();
        break;
    case 19:
        rpg_enemy_wait_msg_step(1);
        break;
    case 20:
        rpg_actor_set_animation(0x28, 1, 5, 1);
        rpg_enemy_step_next();
        break;
    case 21:
        rpg_enemy_wait_animation_step(0x28);
        break;
    case 22:
        ((Actor *)actor_pool)[155].unk_01 = 6;
        rpg_actor_set_animation(0x28, 2, 1, 1);
        rpg_effect_phase_0 = rpg_rand(0x400);
        rpg_effect_phase_1 = rpg_rand(0x400) + 0x400;
        rpg_effect_phase_2 = rpg_rand(0x400) + 0x800;
        rpg_actor_set_animation(0x29, rpg_rand(2) + 3, 1, 1);
        rpg_actor_set_animation(0x2A, rpg_rand(2) + 3, 1, 1);
        rpg_actor_set_animation(0x2B, rpg_rand(2) + 3, 1, 1);
        rpg_actor_set_position(0x29, 0x10, (rsin(rpg_effect_phase_0) >> 8) + 0x20);
        rpg_actor_set_position(0x2A, 0x10, (rsin(rpg_effect_phase_1) >> 8) - 0x10);
        rpg_actor_set_position(0x2B, 0x10, (rsin(rpg_effect_phase_2) >> 8) - 0x40);
        rpg_voice_play_once(0x0C00003A, 0x06000062, 9);
        rpg_se_play_latched_5xx(0x509, 1, 3);
        rpg_enemy_step_next();
        break;
    case 23:
        rpg_enemy_timer_a += 1;
        rpg_actor_set_position(0x29, 0x10 - ((s32) rpg_enemy_timer_a / 2), (rsin(rpg_effect_phase_0) >> 8) + 0x20);
        rpg_actor_set_position(0x2A, 0x10 - ((s32) rpg_enemy_timer_a / 2), (rsin(rpg_effect_phase_1) >> 8) - 0x10);
        rpg_actor_set_position(0x2B, 0x10 - ((s32) rpg_enemy_timer_a / 2), (rsin(rpg_effect_phase_2) >> 8) - 0x40);
        /* FAKE: rpg_effect_phase_0..rpg_effect_phase_2 are three separate words in the header (8014A660.c clears them one by one);
         * the original walks them as an array, and a 3-trip index loop stays rolled. T-17104 */
        for (i = 0; i < 3; i++) {
            (&rpg_effect_phase_0)[i] += 0x28;
        }
        if (rpg_enemy_timer_a >= 0x221) {
            rpg_actor_set_animation(0x28, 0, 1, 1);
            rpg_se_play_latched_5xx(0x507, 1, 4);
            rpg_enemy_step_next();
        }
        if ((cd_audio_state() == 2) && (rpg_enemy_timer_a >= 0x221)) {
            rpg_actor_set_animation(0x28, 0, 1, 1);
            rpg_se_play_latched_5xx(0x507, 1, 4);
            rpg_enemy_step_next();
        }
        break;
    case 24:
        ((Actor *)actor_pool)[155].draw_flags = 0x81;
        rpg_enemy_timer_a += 6;
        rpg_actor_set_position(0x3B, -rpg_enemy_timer_a, 0);
        if (rpg_enemy_timer_a >= 0x89) {
            rpg_enemy_step_next();
        }
        break;
    case 25:
        strcpy(rpg_text_enemy_0, "鹿・グレートは去っていった。");
        k_disp_switch(rpg_label_enemy, 0);
        rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x78);
        rpg_enemy_step_next();
        break;
    case 26:
        rpg_enemy_wait_msg_step(1);
        break;
    case 27:
        rpg_dialog_komuso_farewell();
        break;
    case 28:
        rpg_enemy_timer_a += 1;
        if (!(rpg_enemy_timer_a & 1)) {
            rpg_actor_set_animation(0x28, 0, 1, 1);
        } else {
            rpg_actor_set_animation(0x28, 0, 1, 0);
        }
        if (rpg_enemy_timer_a >= 0x5B) {
            rpg_guest_value ^= 0x10000000;
            rpg_actor_set_animation(0x30, 0, 1, 1);
            if (rpg_girl_id != 0xE) {
                rpg_actor_set_animation(0x1D, 0, 1, 1);
            }
            rpg_enemy_step_next();
        }
        break;
    case 29:
        rpg_enemy_wait_step(0x1E);
        break;
    case 30:
        strcpy(rpg_text_result_0, "戦闘に勝った！！");
        strcpy(rpg_text_result_1, "ありがとう、正義の虚無僧");
        strcpy(rpg_text_result_2, "経験値　：８４１ＥＸＰ");
        strcpy(rpg_text_result_3, "お金　　：２０４５ゴールド");
        rpg_trip_loot_text();
        rpg_enemy_step_next();
        break;
    case 31:
        rpg_enemy_wait_step(0x3C);
        break;
    case 32:
        audio_bank_start(1, 0x202);
        rpg_enemy_step_next();
        break;
    case 33:
        rpg_message_show((s32) rpg_text_result_pointers, rpg_result_line_count, 0x5A);
        rpg_actor_set_animation(0x30, 9, 1, 1);
        if (rpg_girl_id != 0xE) {
            rpg_actor_set_animation(0x1D, 4, 1, 1);
        }
        rpg_enemy_step_next();
        break;
    case 34:
        rpg_enemy_wait_msg_step(0);
        break;
    case 35:
        mode_stage_next();
        break;
    default:
        rpg_enemy_action_end();
        break;
    }
}
