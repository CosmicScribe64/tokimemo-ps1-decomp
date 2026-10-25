/* battle_lifecycle_and_channels.c: src/ovl/RPG_BAT/8014E780.c:57: stage 0, exit, state clear, channel helpers and the state data; body 59: actor_clear_range(0); 60: hud_cursor_actor_init(0);. Original object 0x8014E780-0x8014F790 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
s32 D_8015ED90 = 1;
s32 rpg_player_hit_points = 0;
s32 rpg_player_action = 1;
s32 rpg_player_ready_timer = 2;
s32 rpg_player_hit_points_maximum = 3;
s32 rpg_player_guard_heal = 4;
s32 rpg_player_action_words[2] = { 5, 6 };
RpgBatWords3 rpg_player_channel = { 7, 8, 9 };
s32 rpg_label_player = 0;
s32 rpg_enemy_hit_points = 0;
s32 rpg_enemy_action = 1;
s32 rpg_enemy_timer = 2;
s32 rpg_enemy_hit_points_maximum = 3;
s32 D_8015EDD0 = 4;
s32 rpg_enemy_attack_count = 5;
s32 rpg_enemy_step = 6;
s32 rpg_enemy_substep = 7;
s32 rpg_enemy_timer_a = 8;
s32 rpg_enemy_timer_b = 9;
s32 rpg_label_enemy = 0;
s32 rpg_girl_hit_points = 0;
s32 rpg_girl_action = 1;
s32 rpg_girl_timer = 2;
s32 rpg_girl_hit_points_maximum = 3;
s32 D_8015EDFC = 4;
RpgBatWords4 rpg_girl_channel = { 5, 6, 7, 8 };
s32 rpg_girl_timer_b = 9;
s32 rpg_label_girl = 0;
s32 rpg_guest_value = 0;
s32 rpg_over = 1;
s32 D_8015EE20 = 2;
s32 D_8015EE24 = 3;
s32 D_8015EE28 = 4;
s32 D_8015EE2C = 5;
RpgBatWords4 rpg_finale_channel = { 6, 7, 8, 9 };
s32 rpg_label_guest = 0;
RpgBatWords3 rpg_message = { 0, 1, 2 };
RpgBatWords3 rpg_menu = { 0, 1, 2 };
s32 rpg_popup_0 = 0;
s32 D_8015EE60 = 1;
s32 D_8015EE64 = 2;
s32 rpg_popup_1 = 0;
s32 D_8015EE6C = 1;
s32 D_8015EE70 = 2;
s32 rpg_popup_2 = 0;
s32 D_8015EE78 = 1;
s32 D_8015EE7C = 2;
s32 rpg_popup_3 = 0;
s32 D_8015EE84 = 1;
s32 D_8015EE88 = 2;
s32 rpg_asset_sheet_0 = 0;

void rpg_stage_init(void) {
    srand(game_state.timer_ticks);
    actor_clear_range(0);
    hud_cursor_actor_init(0);
    text_system_reset();
    tpage_buf_clear();
    game_state.clear_color_r = 0;
    (*(s8 *)&game_state.clear_color_b) = 0;
    (*(s8 *)&game_state.clear_color_g) = 0;
    back_clear_switch(1);
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    hizuke_disp_switch(0);
    message_window_title_show(0);
    message_disp_switch(0);
    audio_command(0x7F);
    rpg_state_clear();
    rpg_saved_girl = (s32) game_state.current_girl;
    rpg_girl_id = (s32) game_state.current_girl;
    switch (D_80122CE0) {
    case 1:
        rpg_encounter = 0x10000;
        break;
    case 2:
        rpg_encounter = 0x20000;
        break;
    case 3:
        rpg_encounter = 0x40000;
        break;
    case 4:
        rpg_encounter = 0x100000;
        break;
    case 5:
        rpg_boss_event = 1;
        rpg_encounter = 0x100000;
        break;
    case 6:
        rpg_encounter = 0x08000000;
        break;
    case 7:
    default:
        rpg_encounter = 0x80000000;
        break;
    }
    mode_stage_next();
}

void rpg_exit(void) {
    tpage_buf_clear();
    dec_bg_reset();
    text_system_reset();
    actor_clear_range(0);
    hud_cursor_actor_init(0);
    hud_cursor_show(0);
    back_clear_switch(1);
    D_80122CE0 = rpg_result_code;
    game_state.current_girl = (s8) rpg_saved_girl;
    game_mode_set((s32) game_state.saved_game_mode);
    mode_stage_set((s32) game_state.saved_mode_stage);
    mode_step_set((s32) game_state.saved_mode_step);
}

/* FAKE: the three clear loops share one counter (it keeps the first two rolled
 * and lets the last unroll by four, as in rpg_finale_clear_unused) and walk the legacy
 * scalar rpg_enemy_slot and the 2-D rpg_voice_started as flat word arrays; the third loop
 * body shares its for line to keep the store scheduling. The `(&X)[k]` stores
 * reach the later words of the shared-$at groups through their first symbol
 * (the aggregates are not recovered). T-17108. */
s32 rpg_state_clear(void) {
    s32 i;

    for (i = 0; i < 100; i++) {
        (&rpg_enemy_slot)[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        rpg_se_latch[i] = 0;
    }
    for (i = 0; i < 12; i++) (&rpg_voice_started[0][0])[i] = 0;
    rpg_message.unk_00 = 0;
    rpg_message.unk_04 = 0;
    rpg_message.unk_08 = 0;
    rpg_menu.unk_00 = 0;
    rpg_menu.unk_04 = 0;
    rpg_menu.unk_08 = 0;
    rpg_popup_0 = 0;
    D_8015EE60 = 0;
    (&D_8015EE60)[1] = 0;
    rpg_popup_1 = 0;
    (&rpg_popup_1)[1] = 0;
    D_8015EE70 = 0;
    rpg_popup_2 = 0;
    (&rpg_popup_2)[1] = 0;
    (&rpg_popup_2)[2] = 0;
    rpg_popup_3 = 0;
    (&rpg_popup_3)[1] = 0;
    (&rpg_popup_3)[2] = 0;
    rpg_player_hit_points = 0;
    (&rpg_player_hit_points)[1] = 0;
    (&rpg_player_hit_points)[2] = 0;
    rpg_player_hit_points_maximum = 0;
    (&rpg_player_hit_points_maximum)[1] = 0;
    rpg_girl_hit_points = 0;
    rpg_girl_action = 0;
    (&rpg_girl_action)[1] = 0;
    (&rpg_girl_action)[2] = 0;
    (&rpg_girl_action)[3] = 0;
    rpg_enemy_hit_points = 0;
    (&rpg_enemy_hit_points)[1] = 0;
    (&rpg_enemy_hit_points)[2] = 0;
    (&rpg_enemy_hit_points)[3] = 0;
    D_8015EDD0 = 0;
    rpg_guest_value = 0;
    (&rpg_guest_value)[1] = 0;
    D_8015EE20 = 0;
    (&D_8015EE20)[1] = 0;
    (&D_8015EE20)[2] = 0;
    rpg_player_action_words[0] = 0;
    rpg_player_action_words[1] = 0;
    rpg_player_channel.unk_04 = 0;
    rpg_player_channel.unk_08 = 0;
    rpg_girl_channel.unk_00 = 0;
    rpg_girl_channel.unk_04 = 0;
    rpg_girl_channel.unk_0C = 0;
    rpg_girl_timer_b = 0;
    rpg_enemy_attack_count = 0;
    (&rpg_enemy_attack_count)[1] = 0;
    rpg_enemy_timer_a = 0;
    (&rpg_enemy_timer_a)[1] = 0;
    D_8015EE2C = 0;
    rpg_finale_channel.unk_00 = 0;
    rpg_finale_channel.unk_08 = 0;
    rpg_finale_channel.unk_0C = 0;
    rpg_player_channel.unk_00 = 0;
    rpg_girl_channel.unk_08 = 0;
    rpg_enemy_substep = 0;
    rpg_finale_channel.unk_04 = 0;
}

void rpg_message_next_state(s32 channel) {
    switch (channel) {
    case 0:
        rpg_message.unk_00++;
        rpg_message.unk_08 = 0;
        break;
    case 1:
        rpg_menu.unk_00++;
        rpg_menu.unk_08 = 0;
        break;
    }
}

void rpg_player_step_next(void) {
    rpg_player_action_words[1]++;
    rpg_player_channel.unk_04 = 0;
    rpg_player_channel.unk_00 = 0;
    rpg_player_channel.unk_08 = 0;
}

void rpg_player_substep_next(void) {
    rpg_player_channel.unk_00++;
    rpg_player_channel.unk_08 = 0;
}

/* FAKE: the three clear loops and the record loop reuse one counter, the clear
 * loops walk the legacy scalar rpg_scratch and the 2-D rpg_voice_started as flat word
 * arrays (rpg_state_clear has the same loops), the record loop shares its for line
 * to keep the store order, and rpg_player_ready_timer is reached through rpg_player_action to share
 * its $at (aggregate not recovered). T-17108. */
void rpg_player_action_end(void) {
    s32 i;

    rpg_player_guard_heal = 0;
    rpg_player_action = 0;
    (&rpg_player_action)[1] = 0;
    rpg_player_action_words[1] = 0;
    rpg_player_channel.unk_04 = 0;
    rpg_player_channel.unk_00 = 0;
    rpg_player_channel.unk_08 = 0;
    for (i = 0; i < 50; i++) {
        (&rpg_scratch)[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        rpg_se_latch[i] = 0;
    }
    for (i = 0; i < 12; i++) {
        (&rpg_voice_started[0][0])[i] = 0;
    }
    if (rpg_encounter != 0x80000000) {
        for (i = 0x28; i < 0x30; i++) {
            rpg_actor_set_animation(i, 0xFF, 1, 0);
        }
    }
    i = 0x32;
    do {
        rpg_actor_set_animation(i, 0xFF, 1, 0);
        i++;
    } while (i < 0x38);
    for (i = 40; i < 64; i++) ((Actor *)actor_pool)[i + 96].unk_01 = 5;
    if (rpg_encounter != 0x80000000) {
        ((Actor *)actor_pool)[125].unk_01 = 5;
    }
}

void rpg_player_wait_step(s32 frames) {
    s32 temp_v0;

    temp_v0 = rpg_player_channel.unk_08 + 1;
    rpg_player_channel.unk_08 = temp_v0;
    if (temp_v0 >= frames) {
        rpg_player_step_next();
    }
}

void rpg_player_wait_substep(s32 frames) {
    s32 temp_v0;

    temp_v0 = rpg_player_channel.unk_08 + 1;
    rpg_player_channel.unk_08 = temp_v0;
    if (temp_v0 >= frames) {
        rpg_player_substep_next();
    }
}

void rpg_player_wait_animation_step(s32 actor) {
    if (!(*(&D_80120652 + (actor * 0x44)) & 1)) {
        rpg_player_step_next();
    }
}

void rpg_player_wait_animation_substep(s32 actor) {
    if (!(*(&D_80120652 + (actor * 0x44)) & 1)) {
        rpg_player_substep_next();
    }
}

void rpg_player_wait_msg_step(s32 line_count) {
    if (rpg_message.unk_04 == 0) {
        rpg_player_step_next();
    }
    if (line_count != 0) {
        if ((rpg_message.unk_00 - 3) >= line_count) {
            rpg_player_step_next();
        }
    }
}

void rpg_player_wait_msg_substep(s32 line_count) {
    if (rpg_message.unk_04 == 0) {
        rpg_player_substep_next();
    }
    if (line_count != 0) {
        if ((rpg_message.unk_00 - 3) >= line_count) {
            rpg_player_substep_next();
        }
    }
}

void rpg_sparks_park(void) {
    s32 i;

    for (i = 0x28; i < 0x30; i++) {
        rpg_actor_set_position(i, 0, 0);
    }
}

void rpg_enemy_step_next(void) {
    rpg_enemy_step++;
    rpg_enemy_timer_a = 0;
    rpg_enemy_substep = 0;
    rpg_enemy_timer_b = 0;
}

/* no prototype: rpg_enemy_wait_msg_substep_unused calls it with an argument */
void rpg_enemy_substep_next() {
    rpg_enemy_substep++;
    rpg_enemy_timer_b = 0;
}

/* FAKE: same loops as rpg_player_action_end (shared counter, flat word views of the
 * legacy scalar rpg_scratch and the 2-D rpg_voice_started, record loop on one line), and
 * D_8015EDD0, rpg_enemy_timer are reached through rpg_enemy_action (an unrecovered
 * four-word table; the view also keeps the first store ahead of the load in the
 * schedule). T-17108. */
void rpg_enemy_action_end(void) {
    s32 i;

    (&rpg_enemy_action)[3] = 0;
    rpg_enemy_action &= 0xFFFF0000;
    (&rpg_enemy_action)[1] = 0;
    rpg_enemy_step = 0;
    rpg_enemy_timer_a = 0;
    rpg_enemy_substep = 0;
    rpg_enemy_timer_b = 0;
    for (i = 0; i < 50; i++) {
        (&rpg_scratch)[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        rpg_se_latch[i] = 0;
    }
    for (i = 0; i < 12; i++) {
        (&rpg_voice_started[0][0])[i] = 0;
    }
    for (i = 40; i < 64; i++) ((Actor *)actor_pool)[i + 96].unk_01 = 5;
    if (rpg_encounter != 0x80000000) {
        ((Actor *)actor_pool)[125].unk_01 = 5;
    }
}

void rpg_enemy_wait_step(s32 frames) {
    s32 temp_v0;

    temp_v0 = rpg_enemy_timer_b + 1;
    rpg_enemy_timer_b = temp_v0;
    if (temp_v0 >= frames) {
        rpg_enemy_step_next();
    }
}

void rpg_enemy_wait_substep(s32 frames) {
    s32 temp_v0;

    temp_v0 = rpg_enemy_timer_b + 1;
    rpg_enemy_timer_b = temp_v0;
    if (temp_v0 >= frames) {
        rpg_enemy_substep_next();
    }
}

void rpg_enemy_wait_animation_step(s32 actor) {
    if (!(*(&D_80120652 + (actor * 0x44)) & 1)) {
        rpg_enemy_step_next();
    }
}

void rpg_enemy_wait_animation_substep(s32 actor) {
    if (!(*(&D_80120652 + (actor * 0x44)) & 1)) {
        rpg_enemy_substep_next();
    }
}

void rpg_enemy_wait_msg_step(s32 line_count) {
    if (rpg_message.unk_04 == 0) {
        rpg_enemy_step_next();
    }
    if (line_count != 0) {
        if ((rpg_message.unk_00 - 3) >= line_count) {
            rpg_enemy_step_next();
        }
    }
}

void rpg_enemy_wait_msg_substep_unused(s32 line_count) {
    if (rpg_message.unk_04 == 0) {
        rpg_enemy_substep_next();
    }
    if (line_count != 0) {
        if ((rpg_message.unk_00 - 3) >= line_count) {
            rpg_enemy_substep_next();
        }
    }
}

void rpg_girl_step_next(void) {
    rpg_girl_channel.unk_04++;
    rpg_girl_channel.unk_0C = 0;
    rpg_girl_channel.unk_08 = 0;
    rpg_girl_timer_b = 0;
}

void rpg_girl_substep_next(void) {
    rpg_girl_channel.unk_08++;
    rpg_girl_timer_b = 0;
}

/* FAKE: same loops as rpg_player_action_end (shared counter, flat word views of the
 * legacy scalar rpg_scratch and the 2-D rpg_voice_started, record loop on one line), and
 * D_8015EDFC, rpg_girl_timer are reached through rpg_girl_action to share its $at (the
 * four-word table is not recovered). T-17108. */
void rpg_girl_action_end(void) {
    s32 i;

    (&rpg_girl_action)[3] = 0;
    rpg_girl_action = 0;
    (&rpg_girl_action)[1] = 0;
    rpg_girl_channel.unk_04 = 0;
    rpg_girl_channel.unk_0C = 0;
    rpg_girl_channel.unk_08 = 0;
    rpg_girl_timer_b = 0;
    for (i = 0; i < 50; i++) {
        (&rpg_scratch)[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        rpg_se_latch[i] = 0;
    }
    for (i = 0; i < 12; i++) {
        (&rpg_voice_started[0][0])[i] = 0;
    }
    for (i = 40; i < 64; i++) ((Actor *)actor_pool)[i + 96].unk_01 = 5;
    if (rpg_encounter != 0x80000000) {
        ((Actor *)actor_pool)[125].unk_01 = 5;
    }
}

void rpg_girl_wait_step(s32 frames) {
    s32 temp_v0;

    temp_v0 = rpg_girl_timer_b + 1;
    rpg_girl_timer_b = temp_v0;
    if (temp_v0 >= frames) {
        rpg_girl_step_next();
    }
}

void rpg_girl_wait_substep(s32 frames) {
    s32 temp_v0;

    temp_v0 = rpg_girl_timer_b + 1;
    rpg_girl_timer_b = temp_v0;
    if (temp_v0 >= frames) {
        rpg_girl_substep_next();
    }
}

void rpg_girl_wait_animation_step(s32 actor) {
    if (!(*(&D_80120652 + (actor * 0x44)) & 1)) {
        rpg_girl_step_next();
    }
}

void rpg_girl_wait_animation_substep_unused(s32 actor) {
    if (!(*(&D_80120652 + (actor * 0x44)) & 1)) {
        rpg_girl_substep_next();
    }
}

void rpg_girl_wait_msg_step(s32 line_count) {
    if (rpg_message.unk_04 == 0) {
        rpg_girl_step_next();
    }
    if (line_count != 0) {
        if ((rpg_message.unk_00 - 3) >= line_count) {
            rpg_girl_step_next();
        }
    }
}

void rpg_girl_wait_msg_substep(s32 line_count) {
    if (rpg_message.unk_04 == 0) {
        rpg_girl_substep_next();
    }
    if (line_count != 0) {
        if ((rpg_message.unk_00 - 3) >= line_count) {
            rpg_girl_substep_next();
        }
    }
}

void rpg_finale_step_next(void) {
    rpg_finale_channel.unk_00++;
    rpg_finale_channel.unk_08 = 0;
    rpg_finale_channel.unk_04 = 0;
    rpg_finale_channel.unk_0C = 0;
}

void rpg_finale_substep_next(void) {
    rpg_finale_channel.unk_04++;
    rpg_finale_channel.unk_0C = 0;
}

/* FAKE: EC74 retains its legacy scalar declaration; the observed 50-word
 * range uses an aggregate view. The last loop body shares its for line to
 * preserve store scheduling. T-9560; original source layout unknown. */
void rpg_finale_clear_unused(void) {
    s32 index;

    D_8015EE28 = 0;
    rpg_over = 0;
    D_8015EE20 = 0;
    rpg_finale_channel.unk_00 = 0;
    rpg_finale_channel.unk_08 = 0;
    rpg_finale_channel.unk_04 = 0;
    rpg_finale_channel.unk_0C = 0;
    for (index = 0; index < 50; index++) {
        ((RpgBattleScratch *)&rpg_scratch)->values[index] = 0;
    }
    for (index = 0; index < 10; index++) {
        rpg_se_latch[index] = 0;
    }
    for (index = 0; index < 12; index++) ((s32 *)rpg_voice_started)[index] = 0;
}

void rpg_finale_wait_step(s32 frames) {
    s32 temp_v0;

    temp_v0 = rpg_finale_channel.unk_0C + 1;
    rpg_finale_channel.unk_0C = temp_v0;
    if (temp_v0 >= frames) {
        rpg_finale_step_next();
    }
}

void rpg_finale_wait_substep_unused(s32 frames) {
    s32 temp_v0;

    temp_v0 = rpg_finale_channel.unk_0C + 1;
    rpg_finale_channel.unk_0C = temp_v0;
    if (temp_v0 >= frames) {
        rpg_finale_substep_next();
    }
}

void rpg_finale_wait_animation_step_unused(s32 actor) {
    if (!(*(&D_80120652 + (actor * 0x44)) & 1)) {
        rpg_finale_step_next();
    }
}

void rpg_finale_wait_animation_substep_unused(s32 actor) {
    if (!(*(&D_80120652 + (actor * 0x44)) & 1)) {
        rpg_finale_substep_next();
    }
}

void rpg_finale_wait_msg_step(s32 line_count) {
    if (rpg_message.unk_04 == 0) {
        rpg_finale_step_next();
    }
    if (line_count != 0) {
        if ((rpg_message.unk_00 - 3) >= line_count) {
            rpg_finale_step_next();
        }
    }
}

void rpg_finale_wait_msg_substep_unused(s32 line_count) {
    if (rpg_message_mode == 0) {
        rpg_finale_substep_next();
    }
    if (line_count != 0) {
        if ((rpg_message.unk_00 - 3) >= line_count) {
            rpg_finale_substep_next();
        }
    }
}
