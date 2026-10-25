/* bear_finale_and_rescue.c: src/ovl/RPG_BAT/80132FE0.c:38: bear finale and hunter rescue; body 45: switch (rpg_enemy_step) {; 48: rpg_actor_set_position(0x38, 0, 0);; enemy identity from bear name-label creation src/ovl/RPG_BAT/labels_intro_and_wave_transition.c:32; encounter 0x40000 finale dispatch src/ovl/RPG_BAT/enemy_dispatch.c:37. Original object 0x80132FE0-0x801340F0 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_se_play_latched_5xx /* implicit int view: the call's $v0 moves the rpg_enemy_timer_a loads of rpg_enemy_bear_finale to $v1 (T-17118) */
#include "common.h"
#include "ovl/RPG_BAT.h"

s32 rpg_se_play_latched_5xx();

void rpg_dialog_matagi_warning(void) {
    switch (*rpg_dialog_step) {
    case 0:
        strcpy(rpg_text_enemy_0, "マタギ「こったらとこ来たらだめだべ！");
        strcpy(rpg_text_enemy_1, "早くけえれ！");
        rpg_message_show((s32) rpg_text_enemy_pointers, 2, 0xAA);
        *rpg_dialog_step += 1;
        return;
    case 1:
        rpg_voice_play_once(0x0B000001, 0x02000008, 0);
        *rpg_dialog_step += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x1E000047, 0x0F00004B, 1);
        *rpg_dialog_step += 1;
        return;
    case 4:
        rpg_voice_wait(0);
        return;
    case 5:
        rpg_voice_reset();
        rpg_enemy_step_next();
        /* fallthrough */
    default:
        return;
    }
}

void rpg_enemy_bear_finale(void) {
    s32 i;
    s32 ang;
    s32 tmp; /* the sine of cases 11/13/15 and case 19's call result: one local, one register (s2) */
    s32 cs;
    s32 r;

    switch (rpg_enemy_step) {
    case 0:
        strcpy(rpg_text_enemy_0, "　　　　サーモンブーメラン　　　　");
        rpg_actor_set_position(0x38, 0, 0);
        rpg_actor_set_animation(0x38, 1, 5, 1);
        rpg_se_play_latched_5xx(0x50A, 1, 0);
        rpg_enemy_step_next();
        break;
    case 1:
        if (rpg_actor_is_idle(0x38) != 0) {
            rpg_actor_set_animation(0x38, 1, 1, 0);
            rpg_message_show((s32) rpg_text_enemy_pointers, 1, 0x3C);
            rpg_enemy_step_next();
        }
        break;
    case 2:
        rpg_enemy_wait_msg_step(1);
        break;
    case 3:
        D_80121531 = 4;
        rpg_actor_set_position(0x38, 0, 0);
        rpg_actor_set_animation(0x38, 2, 1, 1);
        rpg_actor_set_animation_alt(0x3B, 4, 1, 1);
        rpg_enemy_step_next();
        break;
    case 4:
        rpg_enemy_timer_a += 2;
        rpg_actor_set_position(0x38, (s32) (rpg_enemy_timer_a * 0xE0) / 100, (s32) (rpg_enemy_timer_a * -0x40) / 100);
        if (D_80121548 == 0) {
            rpg_se_play_latched_5xx(0x506, 1, 0);
        }
        if (rpg_enemy_timer_a >= 0x65) {
            rpg_actor_set_position(0x38, 0xE0, 0x40);
            rpg_enemy_step_next();
        }
        if (rpg_enemy_timer_a == 0x46) {
            rpg_se_play_latched_5xx(0x50B, 1, 6);
            rpg_player_action = RPG_PLAYER_DOWN;
            rpg_actor_set_animation(0x30, 0xA, 1, 1);
        }
        break;
    case 5:
        rpg_enemy_timer_a += 3;
        rpg_actor_set_position(0x38, 0xE0 - ((s32) (rpg_enemy_timer_a * 0xE0) / 100), ((s32) (rpg_enemy_timer_a << 6) / 100) - 0x40);
        if (rpg_enemy_timer_a < 0x50) {
            if (D_80121548 == 0) {
                rpg_se_play_latched_5xx(0x506, 1, 0);
            }
        }
        if (rpg_enemy_timer_a >= 0x65) {
            rpg_actor_set_position(0x38, 0, 0);
            rpg_actor_set_animation(0x38, 2, 1, 0);
            rpg_actor_set_animation_alt(0x3B, 0, 1, 1);
            rpg_enemy_step_next();
        }
        if (rpg_enemy_timer_a == 0x33) {
            rpg_actor_set_animation(0x30, 4, 1, 1);
            rpg_apply_damage(0, 0x21, rpg_player_hit_points - 3, 1);
        }
        break;
    case 6:
        strcpy(rpg_text_enemy_0, game_state.player_family_name);
        strcat(rpg_text_enemy_0, "「だめだ！　喰われてしまう！");
        strcpy(rpg_text_enemy_1, "くそったれめー！");
        rpg_message_show((s32) rpg_text_enemy_pointers, 2, 0x78);
        rpg_enemy_step_next();
        break;
    case 7:
        rpg_enemy_wait_msg_step(0);
        break;
    case 8:
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
            rpg_guest_value ^= 0x10000000;
            strcpy(rpg_text_enemy_0, "正義のマタギ見参！");
            strcpy(rpg_text_enemy_1, "名銃「次郎丸」ショット");
            strcpy(rpg_text_enemy_2, "「次郎丸」エネルギー充填");
            rpg_message_show((s32) rpg_text_enemy_pointers, 3, 0x78);
            rpg_enemy_step_next();
        }
        break;
    case 9:
        for (i = 0; i < 32; i++) {
            ((Actor *)actor_pool)[i + 64].frame_table = (u8 *) rpg_asset_frames_2;
            ((Actor *)actor_pool)[i + 64].sequence_table = (u8 *) rpg_asset_animation_2;
            ((Actor *)actor_pool)[i + 64].tpage_table = (u8 *) rpg_asset_sheet_2;
            ((Actor *)actor_pool)[i + 64].quad_flags = 0x41000000;
            ((Actor *)actor_pool)[i + 64].unk_43 = 0x18;
        }
        rpg_enemy_step_next();
        break;
    case 10:
        rpg_enemy_wait_msg_step(2);
        break;
    case 11:
        rpg_se_play_latched_5xx(0x507, 0, 1);
        ang = (rpg_enemy_timer_a << 0xA) / 60;
        for (i = 0; i < 8; i++) {
            rpg_actor_set_animation(i - 0x20, ((rpg_enemy_timer_a * 6) / 60) + 1, 1, 1);
            tmp = rsin(ang + i * 0x200);
            cs = func_800A0140(ang + i * 0x200);
            r = 0x3C - rpg_enemy_timer_a;
            rpg_actor_set_position(i - 0x20, ((tmp * r) >> 0xD) - 0x10, (cs * r) >> 0xD);
        }
        rpg_enemy_timer_a += 1;
        if (rpg_enemy_timer_a == 0x3C) {
            rpg_enemy_step_next();
        }
        break;
    case 12:
        for (i = 0; i < 8; i++) {
            rpg_actor_set_animation(i - 0x20, 0xFF, 1, 0);
        }
        rpg_enemy_wait_step(0xF);
        break;
    case 13:
        rpg_se_play_latched_5xx(0x50C, 0, 2);
        ang = (rpg_enemy_timer_a << 0xA) / 45;
        for (i = 0; i < 8; i++) {
            rpg_actor_set_animation(i - 0x20, ((rpg_enemy_timer_a * 6) / 45) + 1, 1, 1);
            tmp = rsin(ang + i * 0x200);
            cs = func_800A0140(ang + i * 0x200);
            r = 0x2D - rpg_enemy_timer_a;
            rpg_actor_set_position(i - 0x20, ((tmp * r * 0x3C / 0x2D) >> 0xD) - 0x10, (cs * r * 0x3C / 0x2D) >> 0xD);
        }
        rpg_enemy_timer_a += 1;
        if (rpg_enemy_timer_a == 0x2D) {
            rpg_enemy_step_next();
        }
        break;
    case 14:
        for (i = 0; i < 8; i++) {
            rpg_actor_set_animation(i - 0x20, 0xFF, 1, 0);
        }
        rpg_enemy_wait_step(0xA);
        break;
    case 15:
        rpg_se_play_latched_5xx(0x50D, 0, 3);
        ang = (rpg_enemy_timer_a << 0xA) / 30;
        for (i = 0; i < 8; i++) {
            rpg_actor_set_animation(i - 0x20, ((rpg_enemy_timer_a * 6) / 30) + 1, 1, 1);
            tmp = rsin(ang + i * 0x200);
            cs = func_800A0140(ang + i * 0x200);
            r = 0x1E - rpg_enemy_timer_a;
            rpg_actor_set_position(i - 0x20, ((tmp * r * 2) >> 0xD) - 0x10, (cs * r * 2) >> 0xD);
        }
        rpg_enemy_timer_a += 1;
        if (rpg_enemy_timer_a == 0x1E) {
            rpg_enemy_step_next();
        }
        break;
    case 16:
        for (i = 0; i < 8; i++) {
            rpg_actor_set_animation(i - 0x20, 0xFF, 1, 0);
        }
        rpg_enemy_wait_step(5);
        break;
    case 17:
        for (i = 0; i < 8; i++) {
            rpg_actor_set_animation(i - 0x20, 0xFF, 1, 0);
        }
        rpg_actor_set_animation(0x28, 7, 5, 1);
        rpg_se_play_latched_5xx(0x508, 1, 4);
        rpg_enemy_step_next();
        break;
    case 18:
        rpg_enemy_wait_step(0x14);
        break;
    case 19:
        if (!(rpg_enemy_timer_a & 3)) {
            tmp = rpg_rand(0x40);
            rpg_actor_set_position((rpg_enemy_timer_a / 4) - 0x20, tmp - 0xE0, rpg_rand(0x60) - 0x40);
            rpg_actor_set_animation((rpg_enemy_timer_a / 4) - 0x20, 8, 5, 1);
        }
        if (rpg_enemy_timer_a >= 0x11 && rpg_enemy_timer_a < 0x60) {
            rpg_se_play_latched_5xx(0x509, 0xF, 5);
        }
        rpg_enemy_timer_a += 1;
        if (rpg_enemy_timer_a >= 0x81) {
            rpg_enemy_step_next();
        }
        break;
    case 20:
        rpg_enemy_timer_a += 1;
        if (!(rpg_enemy_timer_a & 1)) {
            rpg_actor_set_position(0x3B, 0, 0);
            rpg_actor_set_animation(0x3B, 0xFF, 1, 1);
            for (i = 0; i < 0x20; i++) {
                rpg_actor_set_animation(i - 0x20, 0xFF, 0x21, 1);
            }
        } else {
            rpg_actor_set_position(0x3B, 0, 0);
            rpg_actor_set_animation(0x3B, 0xFF, 1, 0);
            for (i = 0; i < 0x20; i++) {
                rpg_actor_set_animation(i - 0x20, 0xFF, 0x21, 0);
            }
        }
        if (rpg_enemy_timer_a >= 0x3D) {
            k_disp_switch(rpg_label_enemy, 0);
            rpg_enemy_step_next();
        }
        break;
    case 21:
        rpg_dialog_matagi_warning();
        break;
    case 22:
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
    case 23:
        rpg_enemy_wait_step(0x1E);
        break;
    case 24:
        strcpy(rpg_text_result_0, "戦闘に勝った！！");
        strcpy(rpg_text_result_1, "ありがとう、マタギのおじさん");
        strcpy(rpg_text_result_2, "経験値　：１０２４ＥＸＰ");
        strcpy(rpg_text_result_3, "お金　　：５３ゴールド");
        rpg_trip_loot_text();
        rpg_enemy_step_next();
        break;
    case 25:
        rpg_enemy_wait_step(0x3C);
        break;
    case 26:
        audio_bank_start(1, 0x202);
        rpg_enemy_step_next();
        break;
    case 27:
        rpg_message_show((s32) rpg_text_result_pointers, rpg_result_line_count, 0x5A);
        rpg_actor_set_animation(0x30, 9, 1, 1);
        if (rpg_girl_id != 0xE) {
            rpg_actor_set_animation(0x1D, 4, 1, 1);
        }
        rpg_enemy_step_next();
        break;
    case 28:
        rpg_enemy_wait_msg_step(0);
        break;
    case 29:
        mode_stage_next();
        break;
    default:
        rpg_enemy_action_end();
        break;
    }
}
