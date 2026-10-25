/* enemy_delinquents.c: src/ovl/RPG_BAT/80137280.c:15: delinquent trio: dialogs, attacks, victory, loot; body 16: switch (rpg_dialog_step[0]) {; 20: rpg_message_show(rpg_text_enemy_pointers, 2, 0xC8);; delinquent name-label creation src/ovl/RPG_BAT/labels_intro_and_wave_transition.c:36; encounter 0x100000 action dispatch src/ovl/RPG_BAT/enemy_dispatch.c:47. Original object 0x80137280-0x8013A480 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_actor_set_animation_alt /* implicit int view: the call's $v0 keeps rpg_enemy_slot out of $v1 in rpg_enemy_delinquent_punch_ally (T-24303) */
#define RPG_BAT_OVERRIDE_rpg_actor_set_animation /* implicit int view: the call's $v0 moves the rpg_scratch counter to $v1 (rpg_enemy_delinquent_punch, T-17103) */
#define RPG_BAT_OVERRIDE_rpg_asset_sheet_0 /* pointer view: rpg_bancho_asset_load reads the asset table through it (header: s32, defined in 8014E780.c) */
#define MAIN_API_OVERRIDE_D_80121317 /* stores 0x80 as a positive constant (main_api.h: s8) */
#include "common.h"
#include "ovl/RPG_BAT.h"

extern u8 D_80121317;
s32 rpg_actor_set_animation();
s32 rpg_actor_set_animation_alt();

extern s32 *rpg_asset_sheet_0;

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_delinquent_taunt(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_enemy_0, "不良「そんな攻撃では、");
        strcpy(rpg_text_enemy_1, "俺達の拳とは語れねぇぜ！");
        rpg_message_show(rpg_text_enemy_pointers, 2, 0xC8);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1D000007, 0x0300000C, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x0E000045, 0x0B00004D, 1);
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
s32 rpg_dialog_delinquent_defeat_taunt(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_enemy_0, "不良「口ほどにもないやつめ。");
        strcpy(rpg_text_enemy_1, "出直してきやがれ！");
        rpg_message_show(rpg_text_enemy_pointers, 2, 0xA0);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1E000073, 0x18000079, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x1D000001, 0x02000006, 1);
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
s32 rpg_dialog_delinquent_report(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_result_0, "不良「くっ！　なかなかやるなっ！");
        strcpy(rpg_text_result_1, "しかたねぇ、番長様に御報告だ！");
        rpg_message_show(rpg_text_result_pointers, 2, 0x104);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x0E000014, 0x0700001C, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x0E000037, 0x0A000044, 1);
        rpg_dialog_step[0] += 1;
        return;
    case 4:
        rpg_voice_wait(0);
        return;
    case 5:
        rpg_voice_reset();
        rpg_finale_step_next();
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_mystery_voice(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_result_0, "？？「ちょっと待てぇーい！");
        rpg_message_show(rpg_text_result_pointers, 1, 0xA0);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1A00008E, 0x14000095, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(0);
        return;
    case 3:
        rpg_voice_reset();
        rpg_finale_step_next();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_bancho_intro(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_result_0, "番長「俺様は、この世界の番長だ！");
        strcpy(rpg_text_result_1, "子分達が随分と世話になったようだな。");
        strcpy(rpg_text_result_2, "だが、茶番はここまでだ！");
        strcpy(rpg_text_result_3, "礼をさせてもらうぞ。");
        strcpy(rpg_text_result_4, "これでもくらえぇーい！");
        strcpy(rpg_text_result_5, "超眼力！");
        rpg_message_show(rpg_text_result_pointers, 6, 0x10E);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x04000002, 0x03000011, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x1A000096, 0x150000A3, 1);
        rpg_dialog_step[0] += 1;
        return;
    case 4:
        rpg_voice_wait(2);
        return;
    case 5:
        rpg_voice_play_once(0x1A0000A4, 0x160000B0, 2);
        rpg_dialog_step[0] += 1;
        return;
    case 6:
        rpg_voice_wait(3);
        return;
    case 7:
        rpg_voice_play_once(0x1B000082, 0x0800008A, 3);
        rpg_dialog_step[0] += 1;
        return;
    case 8:
        rpg_voice_wait(4);
        return;
    case 9:
        rpg_voice_play_once(0x1B000095, 0x0A0000A0, 4);
        rpg_dialog_step[0] += 1;
        return;
    case 10:
        rpg_voice_wait(5);
        return;
    case 11:
        rpg_voice_play_once(0x190000AA, 0x140000B7, 5);
        rpg_dialog_step[0] += 1;
        return;
    case 12:
        rpg_voice_wait(0);
        return;
    case 13:
        rpg_voice_reset();
        rpg_finale_step_next();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_bancho_gloat(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_result_0, "番長「ふっ、思い知ったか。");
        strcpy(rpg_text_result_1, "俺様に逆らう者の末路はこうなるのだ。");
        strcpy(rpg_text_result_2, "はっはっはっはっはっ・・・");
        rpg_message_show(rpg_text_result_pointers, 3, 0x118);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1A00005B, 0x0F000065, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x0300004C, 0x0900005B, 1);
        rpg_dialog_step[0] += 1;
        return;
    case 4:
        rpg_voice_wait(2);
        return;
    case 5:
        rpg_voice_play_once(0x1A000066, 0x10000073, 2);
        rpg_dialog_step[0] += 1;
        return;
    case 6:
        rpg_voice_wait(0);
        return;
    case 7:
        rpg_voice_reset();
        rpg_finale_step_next();
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_angel_voice_1(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_result_0, "天使の声「お願い、立ちあがって。");
        strcpy(rpg_text_result_1, "あなたには、");
        strcpy(rpg_text_result_2, "まだやるべきことがあるのよ！");
        rpg_message_show(rpg_text_result_pointers, 3, 0xE6);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x05000013, 0x0300001C, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x1F000081, 0x17000086, 1);
        rpg_dialog_step[0] += 1;
        return;
    case 4:
        rpg_voice_wait(2);
        return;
    case 5:
        rpg_voice_play_once(0x0500001D, 0x04000027, 2);
        rpg_dialog_step[0] += 1;
        return;
    case 6:
        rpg_voice_wait(0);
        return;
    case 7:
        rpg_voice_reset();
        rpg_finale_step_next();
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_angel_voice_2(void) {
    if (rpg_actor_is_idle(0x30) != 0) {
        rpg_actor_set_animation(0x30, 0, 1, 1);
    }
    if (rpg_dialog_step[0] >= 4) {
        if (rpg_player_hit_points != rpg_player_hit_points_maximum) {
            rpg_se_play_latched_5xx(0x504, 2, 5);
        }
        rpg_player_hit_points += 9;
        if (rpg_player_hit_points_maximum < rpg_player_hit_points) {
            rpg_player_hit_points = rpg_player_hit_points_maximum;
        }
    }
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_result_0, "そうよ、あなたにならできるわ！");
        strcpy(rpg_text_result_1, "新しい力をあげる。");
        strcpy(rpg_text_result_2, "彼女を・・・守ってあげて！");
        rpg_message_show(rpg_text_result_pointers, 3, 0xF0);
        rpg_dialog_step[0] += 1;
        break;
    case 1:
        rpg_voice_play_once(0x05000028, 0x05000035, 0);
        rpg_dialog_step[0] += 1;
        break;
    case 2:
        rpg_voice_wait(1);
        break;
    case 3:
        rpg_voice_play_once(0x06000003, 0x0400000C, 1);
        rpg_dialog_step[0] += 1;
        break;
    case 4:
        rpg_voice_wait(2);
        break;
    case 5:
        rpg_voice_play_once(0x0600000D, 0x05000017, 2);
        rpg_dialog_step[0] += 1;
        break;
    case 6:
        rpg_voice_wait(0);
        break;
    case 7:
        if (rpg_player_hit_points == rpg_player_hit_points_maximum) {
            rpg_voice_reset();
            rpg_actor_set_animation(0x30, 0, 1, 1);
            rpg_finale_step_next();
        }
        break;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_bancho_shock(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_result_0, "番長「なにぃぃぃー！？");
        rpg_message_show(rpg_text_result_pointers, 1, 0xA0);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1A000074, 0x1100007A, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(0);
        return;
    case 3:
        rpg_voice_reset();
        rpg_finale_step_next();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_bancho_true_power(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_result_0, "番長「こわっぱが、");
        strcpy(rpg_text_result_1, "超眼力を見切った程度で図に乗りおって");
        strcpy(rpg_text_result_2, "俺様の真の力を見せてくれる。");
        strcpy(rpg_text_result_3, "ゆくぞーっ！");
        rpg_message_show(rpg_text_result_pointers, 4, 0x118);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1A00007B, 0x1200007F, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x1A000080, 0x1300008D, 1);
        rpg_dialog_step[0] += 1;
        return;
    case 4:
        rpg_voice_wait(2);
        return;
    case 5:
        rpg_voice_play_once(0x0300005C, 0x0A00006A, 2);
        rpg_dialog_step[0] += 1;
        return;
    case 6:
        rpg_voice_wait(3);
        return;
    case 7:
        rpg_voice_play_once(0x1E00009F, 0x1D0000A4, 3);
        rpg_dialog_step[0] += 1;
        return;
    case 8:
        rpg_voice_wait(0);
        return;
    case 9:
        rpg_voice_reset();
        rpg_finale_step_next();
        return;
    }
}

void rpg_enemy_delinquent_taunt(void) {
    switch (rpg_enemy_step) {
    case 0:
        rpg_dialog_delinquent_taunt();
        return;
    case 1:
        rpg_enemy_action_end();
        return;
    }
}

/* FAKE: unsigned lvalue view reproduces the original actor-index promotion;
 * the shared overlay header retains older matched signed users. T-9760 */
void rpg_enemy_delinquent_punch_miss(void) {
    switch (rpg_enemy_step) {
    case 0:
        switch ((*(u32 *)&rpg_enemy_slot)) {
        case 0:
            strcpy(rpg_text_enemy_0, "不良Ａの攻撃");
            break;
        case 1:
            strcpy(rpg_text_enemy_0, "不良Ｂの攻撃");
            break;
        case 2:
            strcpy(rpg_text_enemy_0, "不良Ｃの攻撃");
            break;
        }
        strcpy(rpg_text_enemy_1, "不良パンチ");
        rpg_message_show(rpg_text_enemy_pointers, 2, 0x3C);
        rpg_actor_set_animation_alt((*(u32 *)&rpg_enemy_slot) + 0x3B, 1, 5, 1);
        rpg_se_play_latched_5xx(0x505, 1, 0);
        rpg_enemy_step_next();
        return;
    case 1:
        rpg_enemy_wait_animation_step((*(u32 *)&rpg_enemy_slot) + 0x3B);
        return;
    case 2:
        rpg_actor_set_animation_alt((*(u32 *)&rpg_enemy_slot) + 0x3B, 0, 1, 1);
        rpg_enemy_step_next();
        return;
    case 3:
        rpg_enemy_wait_msg_step(2);
        return;
    case 4:
        rpg_popup_miss_show(0, 0x104, 0x66);
        rpg_enemy_step_next();
        return;
    case 5:
        rpg_enemy_wait_msg_step(0);
        return;
    case 6:
        rpg_enemy_wait_step(0x14);
        return;
    case 7:
        rpg_enemy_action_end();
        /* fallthrough */
    default:
        return;
    }
}

/* FAKE: pad1 (frame slot) and the u32 view of rpg_enemy_slot, as in the siblings rpg_enemy_delinquent_punch_miss and
 * rpg_enemy_delinquent_punch. T-24303 */
void rpg_enemy_delinquent_punch_ally(void) {
    s32 pad1;
    s32 r;

    switch (rpg_enemy_step) {
    case 0:
        switch ((*(u32 *)&rpg_enemy_slot)) {
        case 0:
            strcpy(rpg_text_enemy_0, "不良Ａの攻撃");
            rpg_move_timer = (*(u32 *)&rpg_enemy_slot) + rpg_rand(2) + 1;
            break;
        case 1:
            strcpy(rpg_text_enemy_0, "不良Ｂの攻撃");
            rpg_move_timer = (*(u32 *)&rpg_enemy_slot) + rpg_rand(2) * 2 - 1;
            break;
        case 2:
            strcpy(rpg_text_enemy_0, "不良Ｃの攻撃");
            rpg_move_timer = (*(u32 *)&rpg_enemy_slot) - rpg_rand(2) - 1;
            break;
        }
        strcpy(rpg_text_enemy_1, "不良パンチ");
        switch (rpg_move_timer) {
        case 0:
            rpg_message_format_damage(rpg_text_enemy_2, "不良Ａ", "は", rpg_damage, "のダメージを受けた");
            break;
        case 1:
            rpg_message_format_damage(rpg_text_enemy_2, "不良Ｂ", "は", rpg_damage, "のダメージを受けた");
            break;
        case 2:
            rpg_message_format_damage(rpg_text_enemy_2, "不良Ｃ", "は", rpg_damage, "のダメージを受けた");
            break;
        }
        rpg_message_show(rpg_text_enemy_pointers, 3, 0x3C);
        rpg_actor_set_animation_alt((*(u32 *)&rpg_enemy_slot) + 0x3B, 1, 5, 1);
        rpg_se_play_latched_5xx(0x505, 1, 0);
        ((Actor *)actor_pool)[(*(u32 *)&rpg_enemy_slot) + 0x9B].draw_flags |= 1;
        rpg_enemy_step_next();
        return;
    case 1:
        rpg_enemy_wait_animation_step((*(u32 *)&rpg_enemy_slot) + 0x3B);
        return;
    case 2:
        rpg_actor_set_animation_alt((*(u32 *)&rpg_enemy_slot) + 0x3B, 0, 1, 1);
        ((Actor *)actor_pool)[(*(u32 *)&rpg_enemy_slot) + 0x9B].draw_flags |= 1;
        rpg_enemy_step_next();
        return;
    case 3:
        rpg_enemy_wait_msg_step(1);
        return;
    case 4:
        rpg_set_target_position(rpg_move_timer);
        rpg_actor_set_position(0x38, rpg_target_x, rpg_target_y);
        D_80121531 = 3;
        rpg_actor_set_animation_alt(0x38, 0xC, 5, 1);
        rpg_scratch = rpg_rand(4) + 1;
        r = rpg_rand(0x14);
        rpg_actor_set_position(0x38, r + rpg_target_x - 0xA, rpg_rand(0x14) + rpg_target_y - 0xA);
        rpg_se_play_latched_5xx(0x506, 1, 0);
        rpg_enemy_step_next();
        return;
    case 5:
        if (rpg_actor_is_idle(0x38) != 0) {
            rpg_se_play_latched_5xx(0x506, 1, 0);
            r = rpg_rand(0x14);
            rpg_actor_set_position(0x38, r + rpg_target_x - 0xA, rpg_rand(0x14) + rpg_target_y - 0xA);
            rpg_actor_set_animation_alt(0x38, 0xC, 5, 1);
            rpg_scratch -= 1;
            if (rpg_scratch == 0) {
                rpg_enemy_step_next();
                return;
            }
        }
    default:
        return;
    case 6:
        if (rpg_actor_is_idle(0x38) != 0) {
            rpg_actor_set_animation_alt(0x38, 0xFF, 5, 0);
            rpg_enemy_step_next();
            return;
        }
        break;
    case 7:
        rpg_enemy_wait_msg_step(2);
        return;
    case 8:
        rpg_apply_damage(0, rpg_move_timer, rpg_damage, 1);
        rpg_enemy_step_next();
        return;
    case 9:
        rpg_enemy_wait_msg_step(0);
        return;
    case 10:
        rpg_enemy_wait_step(0x14);
        return;
    case 11:
        rpg_enemy_action_end();
        break;
    }
}

/* FAKE: pad1/pad2 are unused locals that reproduce the 16 extra frame bytes above sp34; the
 * unsigned lvalue view of rpg_enemy_slot is the actor-index promotion of rpg_enemy_delinquent_punch_miss (T-9760);
 * rpg_actor_set_animation is called with the implicit int view (file-level override). T-17103 */
void rpg_enemy_delinquent_punch(void) {
    s32 pad1;
    s32 pad2;
    s32 sp34;

    switch (rpg_enemy_step) {
    case 0:
        switch ((*(u32 *) &rpg_enemy_slot)) {
        case 0:
            strcpy(rpg_text_enemy_0, "不良Ａの攻撃");
            break;
        case 1:
            strcpy(rpg_text_enemy_0, "不良Ｂの攻撃");
            break;
        case 2:
            strcpy(rpg_text_enemy_0, "不良Ｃの攻撃");
            break;
        }
        strcpy(rpg_text_enemy_1, "不良パンチ");
        rpg_message_format_damage(rpg_text_enemy_2, game_state.player_family_name, "は", rpg_damage, "のダメージを受けた");
        rpg_message_show(rpg_text_enemy_pointers, 3, 0x3C);
        rpg_actor_set_animation_alt((*(u32 *) &rpg_enemy_slot) + 0x3B, 1, 5, 1);
        rpg_se_play_latched_5xx(0x505, 1, 0);
        rpg_enemy_step_next();
        return;
    case 1:
        rpg_enemy_wait_animation_step((*(u32 *) &rpg_enemy_slot) + 0x3B);
        return;
    case 2:
        rpg_actor_set_animation_alt((*(u32 *) &rpg_enemy_slot) + 0x3B, 0, 1, 1);
        rpg_enemy_step_next();
        return;
    case 3:
        rpg_enemy_wait_msg_step(1);
        return;
    case 4:
        if (rpg_player_action & RPG_PLAYER_GUARD) {
            rpg_actor_set_animation(0x30, 3, 1, 1);
        } else {
            rpg_actor_set_animation(0x30, 5, 1, 1);
        }
        D_80121531 = 3;
        rpg_actor_set_animation(0x38, 0xC, 5, 1);
        rpg_scratch = rpg_rand(4) + 1;
        sp34 = rpg_rand(0x14);
        rpg_actor_set_position(0x38, sp34 - 5, rpg_rand(0x14) - 5);
        rpg_se_play_latched_5xx(0x506, 1, 0);
        rpg_enemy_step_next();
        return;
    case 5:
        if (rpg_actor_is_idle(0x38) != 0) {
            rpg_se_play_latched_5xx(0x506, 1, 0);
            sp34 = rpg_rand(0x14);
            rpg_actor_set_position(0x38, sp34 - 5, rpg_rand(0x14) - 5);
            rpg_actor_set_animation(0x38, 0xC, 5, 1);
            rpg_scratch -= 1;
            if (rpg_scratch == 0) {
                rpg_enemy_step_next();
                return;
            }
        }
    default:
        return;
    case 6:
        if (rpg_actor_is_idle(0x38) != 0) {
            rpg_actor_set_animation(0x38, 0xFF, 5, 0);
            rpg_enemy_step_next();
            return;
        }
        break;
    case 7:
        rpg_enemy_wait_msg_step(2);
        return;
    case 8:
        rpg_apply_damage(0, 0x21, rpg_damage, 1);
        if (rpg_player_hit_points == 0) {
            rpg_enemy_action_end();
            rpg_enemy_action = 0x100007;
            return;
        }
        rpg_enemy_step_next();
        return;
    case 9:
        rpg_enemy_wait_msg_step(0);
        return;
    case 10:
        rpg_enemy_wait_step(0x14);
        return;
    case 11:
        rpg_player_action &= ~0x1000;
        rpg_enemy_action_end();
        break;
    }
}

/* FAKE: pad1/pad2, the u32 view of rpg_enemy_slot and the implicit int rpg_actor_set_animation are the same
 * three tricks as in rpg_enemy_delinquent_punch (this function is its sibling). T-17103 */
void rpg_enemy_delinquent_punch_knockdown(void) {
    s32 pad1;
    s32 pad2;
    s32 sp34;

    switch (rpg_enemy_step) {
    case 0:
        switch ((*(u32 *) &rpg_enemy_slot)) {
        case 0:
            strcpy(rpg_text_enemy_0, "不良Ａの攻撃");
            break;
        case 1:
            strcpy(rpg_text_enemy_0, "不良Ｂの攻撃");
            break;
        case 2:
            strcpy(rpg_text_enemy_0, "不良Ｃの攻撃");
            break;
        }
        strcpy(rpg_text_enemy_1, "不良パンチ");
        rpg_message_format_damage(rpg_text_enemy_2, game_state.player_family_name, "は", rpg_damage, "のダメージを受けた");
        rpg_message_show(rpg_text_enemy_pointers, 3, 0x3C);
        rpg_actor_set_animation_alt((*(u32 *) &rpg_enemy_slot) + 0x3B, 1, 5, 1);
        rpg_se_play_latched_5xx(0x505, 1, 0);
        rpg_enemy_step_next();
        return;
    case 1:
        rpg_enemy_wait_animation_step((*(u32 *) &rpg_enemy_slot) + 0x3B);
        return;
    case 2:
        rpg_actor_set_animation_alt((*(u32 *) &rpg_enemy_slot) + 0x3B, 0, 1, 1);
        rpg_enemy_step_next();
        return;
    case 3:
        rpg_enemy_wait_msg_step(1);
        return;
    case 4:
        rpg_actor_set_animation(0x30, 4, 1, 1);
        rpg_player_action |= RPG_PLAYER_DOWN;
        if (rpg_player_action & RPG_PLAYER_READY) {
            rpg_player_ready_timer = 0;
            rpg_player_action &= ~0x10000;
            rpg_menu.unk_00 = 4;
        }
        D_80121531 = 3;
        rpg_actor_set_animation(0x38, 0xC, 5, 1);
        rpg_scratch = rpg_rand(4) + 1;
        sp34 = rpg_rand(0x14);
        rpg_actor_set_position(0x38, sp34 - 5, rpg_rand(0x14) - 5);
        rpg_se_play_latched_5xx(0x506, 1, 0);
        rpg_enemy_step_next();
        return;
    case 5:
        if (rpg_actor_is_idle(0x38) != 0) {
            rpg_se_play_latched_5xx(0x506, 1, 0);
            sp34 = rpg_rand(0x14);
            rpg_actor_set_position(0x38, sp34 - 5, rpg_rand(0x14) - 5);
            rpg_actor_set_animation(0x38, 0xC, 5, 1);
            rpg_scratch -= 1;
            if (rpg_scratch == 0) {
                rpg_enemy_step_next();
                return;
            }
        }
    default:
        return;
    case 6:
        if (rpg_actor_is_idle(0x38) != 0) {
            rpg_actor_set_animation(0x38, 0xFF, 5, 0);
            rpg_enemy_step_next();
            return;
        }
        break;
    case 7:
        rpg_enemy_wait_msg_step(2);
        return;
    case 8:
        rpg_apply_damage(0, 0x21, rpg_damage, 1);
        if (rpg_player_hit_points == 0) {
            rpg_enemy_action_end();
            rpg_enemy_action = 0x100007;
            return;
        }
        rpg_enemy_step_next();
        return;
    case 9:
        rpg_enemy_wait_msg_step(0);
        return;
    case 10:
        rpg_enemy_wait_step(0x14);
        return;
    case 11:
        rpg_player_action = rpg_player_action & ~0x1000 & 0xFFFFFF;
        rpg_enemy_action_end();
        break;
    }
}

void rpg_enemy_delinquent_punch_cover(void) {
    /* FAKE: two unused words reproduce the original 0x40-byte frame and spill home. T-20003. */
    s32 frame_pad[2];
    s32 random_value;
    s32 remaining;

    switch (rpg_enemy_step) {
    case 0:
        /* FAKE: this explicit word view keeps the selector in v1 instead of v0.
         * The original source spelling is unknown. T-20003. */
        switch (*(s32 *)&rpg_enemy_slot) {
        case 0:
            strcpy(rpg_text_enemy_0, "不良Ａの攻撃");
            break;
        case 1:
            strcpy(rpg_text_enemy_0, "不良Ｂの攻撃");
            break;
        case 2:
            strcpy(rpg_text_enemy_0, "不良Ｃの攻撃");
            break;
        }
        strcpy(rpg_text_enemy_1, "不良パンチ");
        strcpy(rpg_text_enemy_2, game_state.player_family_name);
        strcat(rpg_text_enemy_2, "は");
        strcat(rpg_text_enemy_2, girl_short_name_table[rpg_girl_id]);
        strcat(rpg_text_enemy_2, "をかばった");
        rpg_message_format_damage((s32) rpg_text_enemy_3, (s32) game_state.player_family_name, (s32) "は", rpg_damage, (s32) "のダメージを受けた");
        rpg_message_show((s32) rpg_text_enemy_pointers, 4, 0x3C);
        rpg_actor_set_animation_alt(rpg_enemy_slot + 0x3B, 1, 5, 1);
        rpg_se_play_latched_5xx(0x505, 1, 0);
        rpg_enemy_step_next();
    default:
        break;
    case 1:
        rpg_enemy_wait_animation_step(rpg_enemy_slot + 0x3B);
        break;
    case 2:
        rpg_enemy_wait_msg_step(1);
        break;
    case 3:
        rpg_actor_set_animation_alt(rpg_enemy_slot + 0x3B, 0, 1, 1);
        rpg_enemy_step_next();
        break;
    case 4:
        rpg_enemy_timer_a += 1;
        rpg_actor_set_position(0x30, (s32) (rpg_enemy_timer_a * -0x18) / 4, (s32) (rpg_enemy_timer_a * 0x30) / 4);
        if (rpg_enemy_timer_a == 4) {
            rpg_enemy_step_next();
        }
        break;
    case 5:
        rpg_actor_set_animation(0x30, 5, 1, 1);
        ((Actor *)actor_pool)[152].unk_01 = 3;
        rpg_actor_set_animation(0x38, 0xC, 5, 1);
        rpg_scratch = rpg_rand(4) + 1;
        random_value = rpg_rand(0x14);
        rpg_actor_set_position(0x38, random_value - 0x1D, rpg_rand(0x14) + 0x2B);
        rpg_se_play_latched_5xx(0x506, 1, 0);
        rpg_enemy_step_next();
        break;
    case 6:
        if (rpg_actor_is_idle(0x38) != 0) {
            rpg_se_play_latched_5xx(0x506, 1, 0);
            random_value = rpg_rand(0x14);
            rpg_actor_set_position(0x38, random_value - 0x1D, rpg_rand(0x14) + 0x2B);
            rpg_actor_set_animation(0x38, 0xC, 5, 1);
            if (--rpg_scratch == 0) {
                rpg_enemy_step_next();
            }
        }
        break;
    case 7:
        if (rpg_actor_is_idle(0x38) != 0) {
            rpg_actor_set_animation(0x38, 0xFF, 5, 0);
            rpg_enemy_step_next();
        }
        break;
    case 8:
        rpg_enemy_wait_msg_step(3);
        break;
    case 9:
        rpg_apply_damage(0, 0x20, rpg_damage, 1);
        if (rpg_player_hit_points == 0) {
            rpg_enemy_action_end();
            rpg_enemy_action = 0x100007;
        } else {
            rpg_enemy_step_next();
        }
        break;
    case 10:
        rpg_enemy_wait_msg_step(0);
        break;
    case 11:
        rpg_enemy_timer_a += 1;
        remaining = 4 - rpg_enemy_timer_a;
        rpg_actor_set_position(0x30, (s32) (remaining * -0x18) / 4, (s32) (remaining * 0x30) / 4);
        if (rpg_enemy_timer_a == 4) {
            rpg_enemy_step_next();
        }
        break;
    case 12:
        rpg_enemy_wait_step(0x14);
        break;
    case 13:
        rpg_player_action &= ~0x1000;
        rpg_enemy_action_end();
        break;
    }
}

void rpg_enemy_player_defeated(void) {
    switch (rpg_enemy_step) {
    case 0:
        rpg_player_action = RPG_PLAYER_DOWN;
        rpg_actor_set_animation(0x30, 7, 1, 1);
        audio_bank_start(1, 0x203);
        rpg_enemy_step_next();
        return;
    case 1:
        rpg_enemy_wait_step(0x78);
        return;
    case 2:
        rpg_dialog_delinquent_defeat_taunt();
        return;
    case 3:
        rpg_result_code = RPG_RESULT_BAD;
        mode_stage_next();
        return;
    }
}

void rpg_victory_delinquents(void) {
    /* FAKE: two unused words reproduce the original eight extra frame bytes
     * above the RECT locals; no real source for these homes is known. T-24013 */
    s32 pad0;
    s32 pad1;
    RECT r0;
    RECT r1;
    RECT r2;
    RECT r3;

    switch (rpg_finale_channel.unk_00) {
    case 0:
        rpg_player_action = RPG_PLAYER_DOWN;
        rpg_finale_step_next();
    default:
        break;
    case 1:
        rpg_dialog_delinquent_report();
        break;
    case 2:
        if (rpg_finale_channel.unk_08 == 0) {
            rpg_se_play_latched_5xx(0x507, 1, 0);
        }
        rpg_finale_channel.unk_08 += 8;
        k_disp_switch(rpg_label_enemy, 0);
        ((Actor *)actor_pool)[155].x = (s16) (((Actor *)actor_pool)[155].x - 8);
        ((Actor *)actor_pool)[156].x = (s16) (((Actor *)actor_pool)[156].x - 8);
        ((Actor *)actor_pool)[157].x = (s16) (((Actor *)actor_pool)[157].x - 8);
        if (rpg_finale_channel.unk_08 >= 0x91) {
            rpg_finale_step_next();
        }
        break;
    case 3:
        rpg_finale_wait_step(0x1E);
        break;
    case 4:
        rpg_actor_set_animation(0x30, 9, 1, 1);
        switch (rpg_girl_id) {
        case 0:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        case 1:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        case 2:
            rpg_actor_set_animation(0x1D, 2, 1, 1);
            break;
        case 3:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        case 4:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        case 5:
            rpg_actor_set_animation(0x1D, 2, 1, 1);
            break;
        case 6:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        case 7:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        case 8:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        case 9:
            rpg_actor_set_animation(0x1D, 7, 1, 1);
            break;
        case 10:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        case 13:
            rpg_actor_set_animation(0x1D, 4, 1, 1);
            break;
        }
        audio_bank_start(1, 0x202);
        strcpy(rpg_text_result_0, "戦闘に勝った！！");
        strcpy(rpg_text_result_1, "経験値　：２３６０ＥＸＰ");
        strcpy(rpg_text_result_2, "お金　　：３６００ゴールド");
        rpg_loot_text_delinquents();
        rpg_message_show((s32) rpg_text_result_pointers, rpg_result_line_count, 0x5A);
        rpg_finale_step_next();
        break;
    case 5:
        rpg_finale_wait_msg_step(0);
        break;
    case 6:
        rpg_state.result_code = 0;
        if (rpg_has_boss_event() != 0) {
            rpg_finale_step_next();
        } else {
            mode_stage_next();
        }
        break;
    case 7:
        rpg_finale_wait_step(0x3C);
        break;
    case 8:
        ((Actor *)actor_pool)[127].brightness = 0x40;
        rpg_actor_set_animation(0x30, 0, 1, 1);
        rpg_actor_set_animation(0x1D, 0, 1, 1);
        rpg_finale_step_next();
        break;
    case 9:
        rpg_dialog_mystery_voice();
        break;
    case 10:
        strcpy(rpg_text_result_0, game_state.player_family_name);
        strcat(rpg_text_result_0, "「誰だっ！");
        rpg_message_show((s32) rpg_text_result_pointers, 1, 0x3C);
        rpg_finale_step_next();
        break;
    case 11:
        rpg_finale_wait_msg_step(0);
        break;
    case 12:
        rpg_actor_set_position(0x3B, rpg_finale_channel.unk_08 - 0x80, 0);
        rpg_actor_set_animation(0x3B, 2, 1, 1);
        rpg_finale_channel.unk_08 += 8;
        if (rpg_finale_channel.unk_08 >= 0x81) {
            audio_bank_start(1, 0x201);
            k_disp_switch(rpg_label_guest, 1);
            rpg_finale_step_next();
        }
        break;
    case 13:
        rpg_dialog_bancho_intro();
        break;
    case 14:
        rpg_actor_set_animation(0x3B, 3, 5, 1);
        rpg_se_play_latched_5xx(0x508, 1, 0);
        rpg_finale_step_next();
        break;
    case 15:
        if (rpg_actor_is_idle(0x3B) != 0) {
            rpg_apply_damage(0, 0x21, rpg_player_hit_points, 1);
            rpg_actor_set_animation(0x3B, 2, 1, 1);
            rpg_actor_set_animation(0x30, 7, 1, 1);
            rpg_player_hit_points = 0;
            rpg_finale_step_next();
        }
        break;
    case 16:
        audio_bank_start(1, 0x203);
        rpg_fade_set_level(0xFF);
        rpg_finale_channel.unk_08 += 1;
        if (rpg_finale_channel.unk_08 >= 5) {
            rpg_finale_step_next();
        }
        break;
    case 17:
        rpg_fade_set_level(0x80);
        ((Actor *)actor_pool)[127].brightness = (s8) (0x40 - rpg_finale_channel.unk_08);
        rpg_finale_channel.unk_08 += 1;
        if (rpg_finale_channel.unk_08 >= 0x41) {
            rpg_finale_step_next();
        }
        break;
    case 18:
        rpg_dialog_bancho_gloat();
        break;
    case 19:
        rpg_state.pause_debounce = 0xFFF;
        rpg_finale_wait_step(0x3C);
        break;
    case 20:
        rpg_bancho_asset_load();
        break;
    case 21:
        strcpy(rpg_text_result_0, "（どこからともなく声が聞こえてきた）");
        rpg_message_show((s32) rpg_text_result_pointers, 1, 0x5A);
        rpg_finale_step_next();
        break;
    case 22:
        rpg_finale_wait_msg_step(0);
        break;
    case 23:
        rpg_dialog_angel_voice_1();
        break;
    case 24:
        rpg_bancho_flash();
        break;
    case 25:
        rpg_actor_set_animation(0x30, 8, 5, 1);
        rpg_player_hit_points = rpg_player_hit_points_maximum;
        rpg_player_hit_points_maximum += 0x7D0;
        if (rpg_player_hit_points_maximum >= 0x2710) {
            rpg_player_hit_points_maximum = 0x270F;
        }
        rpg_finale_step_next();
        break;
    case 26:
        rpg_dialog_angel_voice_2();
        break;
    case 27:
        rpg_dialog_bancho_shock();
        break;
    case 28:
        strcpy(rpg_text_result_0, game_state.player_family_name);
        strcat(rpg_text_result_0, "「もう超眼力は見切ったぜ！");
        strcpy(rpg_text_result_1, "番長！　負ける訳にはいかない！！");
        rpg_message_show((s32) rpg_text_result_pointers, 2, 0x5A);
        rpg_finale_step_next();
        break;
    case 29:
        rpg_finale_wait_msg_step(2);
        break;
    case 30:
        rpg_dialog_bancho_true_power();
        break;
    case 31:
        r0.x = 0x240, r0.y = 0x100;
        r0.w = 0x40, r0.h = 0x28;
        func_8009C93C(&r0, 0x180, 0x180);
        r1.x = 0x240, r1.y = 0x128;
        r1.w = 0x40, r1.h = 0x28;
        func_8009C93C(&r1, 0x1C0, 0x180);
        r2.x = 0x220, r2.y = 0x100;
        r2.w = 0x40, r2.h = 0x28;
        func_8009C93C(&r2, 0x200, 0x180);
        r3.x = 0x21F, r3.y = 0x100;
        r3.w = 1, r3.h = 0x80;
        func_8009C93C(&r3, 0x220, 0x100);
        rpg_bg_word_0 = 0;
        rpg_bg_word_1 = 0x20;
        rpg_bg_word_2 = 0x20;
        rpg_bg_word_3 = 0x40;
        rpg_bg_word_4 = 1;
        rpg_bg_word_5 = 1;
        rpg_finale_step_next();
        break;
    case 32:
        ((Actor *)actor_pool)[127].brightness = (s8) rpg_finale_channel.unk_08;
        rpg_actor_set_animation(0x1F, 0, 1, 0);
        rpg_finale_channel.unk_08 += 1;
        if (rpg_finale_channel.unk_08 >= 0x81) {
            rpg_finale_step_next();
        }
        break;
    case 33:
        rpg_over = 0;
        rpg_finale_channel.unk_00 = 0;
        audio_bank_start(1, 0x204);
        if (rpg_girl_action != 0x40000000) {
            rpg_girl_action = 0;
        }
        rpg_enemy_delay_extra = 0;
        rpg_enemy_damage_scale = RPG_ENEMY_DAMAGE_SCALE_BASE;
        rpg_state.enemy_weight_0 = 0;
        rpg_state.enemy_weight_1 = 0;
        rpg_state.enemy_weight_2 = 0;
        rpg_state.enemy_weight_3 = 0;
        rpg_girl_channel.unk_04 = 0;
        rpg_girl_timer = 0;
        rpg_player_action = 0;
        rpg_player_step = 0;
        rpg_player_ready_timer = 0xFFFF;
        rpg_enemy_step = 0;
        rpg_enemy_timer = 0;
        rpg_enemy_action = 0x200000;
        break;
    }
}

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/enemy_delinquents.rodata", D_8015B6F8);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/enemy_delinquents.rodata", D_8015B708);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/enemy_delinquents.rodata", D_8015B718);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/enemy_delinquents.rodata", D_8015B728);

void rpg_loot_text_delinquents(void) {
    /* the three scalars sit between the tables: they set flags at sp+0x64 (T-17103) */
    RpgRewardTextTable names;
    s32 n;
    s32 count;
    s32 i;
    RpgRewardSelectionFlags flags;

    names = rpg_loot_names_delinquents;
    flags = rpg_loot_flags_init;
    n = 0;
    if (rpg_rand(5) == 0) {
        count = 3;
    } else {
        count = 2;
    }
    for (i = 0; i < count; i++) {
        switch (rpg_rand(0xA)) {
        case 0:
        case 1:
        case 2:
        case 3:
            flags.selected[0] = 1;
            break;
        case 4:
        case 5:
        case 6:
            flags.selected[1] = 1;
            break;
        case 7:
        case 8:
            flags.selected[2] = 1;
            break;
        case 9:
            flags.selected[3] = 1;
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        if (flags.selected[i] == 1) {
            strcpy(*(void **)(rpg_text_result_pointers + n * 4 + 12), "おたから：");
            strcat(*(void **)(rpg_text_result_pointers + n * 4 + 12), names.strings[i]);
            n++;
        }
    }
    rpg_result_line_count = n + 3;
}

void rpg_bancho_asset_load(void) {
    s32 i;

    switch (rpg_finale_channel.unk_04) {
    case 0:
        cd_read_sectors(0x2B, 0x801B1000, 0x3E8C);
        rpg_finale_substep_next();
    default:
        break;
    case 1:
        if (rpg_state.pause_debounce == 0) {
            rpg_finale_step_next();
        }
        if (cd_read_sync_status() & 1) {
            rpg_state.pause_debounce = 0;
            rpg_finale_substep_next();
        }
        break;
    case 2:
        /* FAKE: the three constants are stored through their addresses (T-17103). A plain assignment
         * makes uopt promote the globals to registers and sink the stores below the -1 stores
         * (original: stores in source order, all three still read from memory later); the real
         * source is unknown. */
        *(s32 **) &rpg_asset_sheet_0 = (s32 *) 0x801C5DC8;
        *(s32 *) &rpg_asset_frames_0 = 0x801C5DDC;
        *(s32 *) &rpg_asset_animation_0 = 0x801C5EE4;
        game_state.tpage_buf[0x15] = -1;
        game_state.tpage_buf[0x16] = -1;
        game_state.tpage_buf[0x17] = -1;
        game_state.tpage_buf[0x18] = -1;
        game_state.tpage_buf[0x19] = -1;
        load_tpage_buf_lock(0x15, rpg_asset_sheet_0[0], 1);
        load_tpage_buf_lock(0x16, rpg_asset_sheet_0[1], 1);
        load_tpage_buf_lock(0x17, rpg_asset_sheet_0[2], 1);
        load_tpage_buf_lock(0x18, rpg_asset_sheet_0[3], 1);
        load_tpage_buf_lock(0x19, rpg_asset_sheet_0[4], 1);
        for (i = 0; i < 72; i++) {
            ((Actor *) actor_pool)[64 + i].frame_table = (u8 *) rpg_asset_frames_0;
            ((Actor *) actor_pool)[64 + i].sequence_table = (u8 *) rpg_asset_animation_0;
            ((Actor *) actor_pool)[64 + i].tpage_table = (u8 *) rpg_asset_sheet_0;
        }
        if (rpg_girl_id != 0xE) {
            for (i = 29; i < 31; i++) {
                ((Actor *) actor_pool)[96 + i].frame_table = (u8 *) rpg_asset_frames_5;
                ((Actor *) actor_pool)[96 + i].sequence_table = (u8 *) rpg_asset_animation_5;
                ((Actor *) actor_pool)[96 + i].tpage_table = (u8 *) rpg_asset_sheet_5;
                ((Actor *) actor_pool)[96 + i].unk_43 = 0x1C;
            }
        }
        rpg_finale_substep_next();
        break;
    case 3:
        game_state.current_girl = GIRL_SOTOI;
        cd_audio_request_set(0x1C0000A8, 0x0F0000B7, 0xE);
        audio_command(0x300);
        rpg_finale_substep_next();
        break;
    case 4:
        audio_command(0xE4);
        rpg_finale_substep_next();
        break;
    case 5:
        rpg_finale_channel.unk_0C += 1;
        if (rpg_finale_channel.unk_0C >= 0xB5) {
            rpg_finale_step_next();
        }
        if (cd_audio_state() & 4) {
            rpg_finale_step_next();
        }
        break;
    }
}

void rpg_bancho_flash_set();

void rpg_bancho_flash(void) {
    rpg_finale_timer += 1;
    switch (rpg_finale_timer % 4) {
    case 0:
        if (rpg_finale_timer < 0x78) {
            rpg_bancho_flash_set(0x1E, 0xFF);
        } else {
            rpg_bancho_flash_set(0x1F, 0xFF);
        }
        break;
    case 1:
        if (rpg_finale_timer < 0x78) {
            rpg_bancho_flash_set(0x1E, 0x80);
        } else {
            rpg_bancho_flash_set(0x1F, 0x80);
        }
        break;
    case 2:
        if (rpg_finale_timer < 0x3C) {
            rpg_bancho_flash_set(0x1E, 0xFF);
        } else {
            rpg_bancho_flash_set(0x1F, 0xFF);
        }
        break;
    case 3:
        if (rpg_finale_timer < 0x3C) {
            rpg_bancho_flash_set(0x1E, 0x80);
        } else {
            rpg_bancho_flash_set(0x1F, 0x80);
        }
        break;
    }
    if (rpg_finale_timer >= 0xB5) {
        D_80121353 = 0x1F;
        D_80121397 = 0x1F;
        D_80121317 = 0x80;
        rpg_finale_step_next();
    }
}

void rpg_bancho_flash_set(a0, a1)
    s8 a0;
    s8 a1;
{
    D_80121353 = a0;
    D_80121317 = a1;
}
