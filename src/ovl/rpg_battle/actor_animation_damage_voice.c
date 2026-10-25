/* actor_animation_damage_voice.c: src/ovl/RPG_BAT/8013E4D0.c:4: actor animation and position accessors, damage application, latched sounds and voice waits; body 8: p = (Actor *)actor_pool; 9: for (i = 40; i < 64; i++) {. Original object 0x8013E4D0-0x8013F4F0 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/rpg_battle.h"

void rpg_effect_records_reset_unused(void) {
    s32 i;
    Actor *p;

    p = (Actor *)actor_pool;
    for (i = 40; i < 64; i++) {
        p[96 + i].unk_01 = 5;
        p[96 + i].quad_flags = 0x41000000;
        p[96 + i].flags = 0x1;
        p[96 + i].blink_period = 0x1;
        p[96 + i].scale_x = 0x1000;
        p[96 + i].scale_y = 0x1000;
    }
    for (i = 29; i < 31; i++) {
        p[96 + i].unk_01 = 5;
        p[96 + i].quad_flags = 0x41000000;
        p[96 + i].flags = 0x1;
        p[96 + i].blink_period = 0x1;
        p[96 + i].scale_x = 0x1000;
        p[96 + i].scale_y = 0x1000;
    }
}

/* Implicit int with no return value (the callers have no prototype): with $v0 live at the exit, uopt
 * emits the hoisted loop limit before the strength-reduced start in each case, as the original (T-20600). */
s32 rpg_effects_select_asset_set(s32 arg0) {
    s32 i;

    switch (arg0) {
    case 3:
        for (i = 4; i < 16; i++) {
            *(s32 *)&actor_pool[0x198C + i * 0x44] = rpg_asset_frames_4;
            *(s32 *)&actor_pool[0x1990 + i * 0x44] = rpg_asset_animation_4;
            *(s32 *)&actor_pool[0x19B4 + i * 0x44] = rpg_asset_sheet_4;
            ((Actor *)actor_pool)[i + 96].unk_01 = 5;
            ((Actor *)actor_pool)[i + 96].unk_43 = 0x1D;
        }
        break;
    case 4:
        for (i = 4; i < 16; i++) {
            *(s32 *)&actor_pool[0x198C + i * 0x44] = rpg_asset_frames_1;
            *(s32 *)&actor_pool[0x1990 + i * 0x44] = rpg_asset_animation_1;
            *(s32 *)&actor_pool[0x19B4 + i * 0x44] = rpg_asset_sheet_1;
            ((Actor *)actor_pool)[i + 96].unk_01 = 5;
            ((Actor *)actor_pool)[i + 96].unk_43 = 0x12;
        }
        break;
    case 5:
        for (i = 4; i < 16; i++) {
            *(s32 *)&actor_pool[0x198C + i * 0x44] = rpg_asset_frames_5;
            *(s32 *)&actor_pool[0x1990 + i * 0x44] = rpg_asset_animation_5;
            *(s32 *)&actor_pool[0x19B4 + i * 0x44] = rpg_asset_sheet_5;
            ((Actor *)actor_pool)[i + 96].unk_01 = 5;
            ((Actor *)actor_pool)[i + 96].unk_43 = 0x1C;
        }
        break;
    case 0xFF:
        for (i = 4; i < 16; i++) {
            *(s32 *)&actor_pool[0x198C + i * 0x44] = rpg_asset_frames_0;
            *(s32 *)&actor_pool[0x1990 + i * 0x44] = rpg_asset_animation_0;
            *(s32 *)&actor_pool[0x19B4 + i * 0x44] = rpg_asset_sheet_0;
            ((Actor *)actor_pool)[i + 96].unk_43 = 0;
            ((Actor *)actor_pool)[i + 96].unk_01 = 3;
            ((Actor *)actor_pool)[i + 96].draw_flags = 0;
        }
        break;
    }
}

void rpg_actor_set_animation(s32 actor_index, s32 arg1, s32 arg2, s32 arg3) {
    u8 *temp_v0;

    temp_v0 = actor_pool + actor_index * 0x44;
    *(s16 *)(temp_v0 + 0x1988) = 0;
    *(u8 *)(temp_v0 + 0x1982) = arg2;
    if (arg3 == 1) {
        *(u8 *)(temp_v0 + 0x1983) = 0x80;
    } else {
        *(u8 *)(temp_v0 + 0x1983) = 0;
    }
    *(s16 *)(temp_v0 + 0x1998) = 0;
    if (arg1 != 0xFF) {
        *(s16 *)(temp_v0 + 0x1996) = arg1;
    }
}

void rpg_actor_set_animation_alt(s32 actor_index, s32 arg1, s32 arg2, s32 arg3) {
    u8 *temp_v0;

    temp_v0 = actor_pool + actor_index * 0x44;
    *(s16 *)(temp_v0 + 0x1988) = 0;
    *(u8 *)(temp_v0 + 0x1982) = arg2;
    if (arg3 == 1) {
        *(u8 *)(temp_v0 + 0x1983) = 0x84;
    } else {
        *(u8 *)(temp_v0 + 0x1983) = 4;
    }
    *(s16 *)(temp_v0 + 0x1998) = 0;
    if (arg1 != 0xFF) {
        *(s16 *)(temp_v0 + 0x1996) = arg1;
    }
}

void rpg_record_set_animation(s32 actor_index, s32 arg1, s32 arg2) {
    u8 *temp_v0;

    temp_v0 = actor_pool + actor_index * 0x44;
    *(s16 *)(temp_v0 + 0x8) = 0;
    *(u8 *)(temp_v0 + 0x2) = 1;
    if (arg2 == 1) {
        *(u8 *)(temp_v0 + 0x3) = 0x80;
    } else {
        *(u8 *)(temp_v0 + 0x3) = 0;
    }
    *(s16 *)(temp_v0 + 0x18) = 0;
    if (arg1 != 0xFF) {
        *(s16 *)(temp_v0 + 0x16) = arg1;
    }
}

void rpg_popup_digit_set_animation(s32 popup_actor_index, s32 arg1, s32 arg2) {
    u8 *temp_v0;

    temp_v0 = actor_pool + popup_actor_index * 0x44;
    *(s16 *)(temp_v0 + 0x1108) = 0;
    *(u8 *)(temp_v0 + 0x1102) = 1;
    if (arg2 == 1) {
        *(u8 *)(temp_v0 + 0x1103) = 0x80;
    } else {
        *(u8 *)(temp_v0 + 0x1103) = 0;
    }
    *(s16 *)(temp_v0 + 0x1118) = 0;
    if (arg1 != 0xFF) {
        *(s16 *)(temp_v0 + 0x1116) = arg1;
    }
}

s32 rpg_actor_is_idle(s32 i) {
    return (*(&D_80120652 + i * 0x44) & 1) == 0;
}

void rpg_label_create(s32 arg0, s32 arg1, s32 arg2) {
    set_kanji_string(arg0 - 0xA0, arg1 - 0x78, 0, arg2, 0);
}

void rpg_actor_set_position(s32 i, s32 x, s32 y) {
    u8 *p = actor_pool + i * 0x44;
    *(s16 *)(p + 0x19A6) = x - 0xA0;
    *(s16 *)(p + 0x19AA) = y - 0x78;
}

s32 rpg_rand(s32 upper_bound) {
    if (upper_bound == 0) {
        upper_bound = 1;
    }
    return rand() % upper_bound;
}

s32 rpg_rand2(s32 upper_bound_a, s32 upper_bound_b) {
    s32 a = rpg_rand(upper_bound_a);
    s32 b = rpg_rand(upper_bound_b);
    return a + b;
}

void rpg_fade_set_level(s32 arg0) {
    s32 i;

    for (i = 0; i < 96; i++) {
        ((Actor *)actor_pool)[i + 64].brightness = arg0;
    }
}

void rpg_apply_damage(s32 popup_slot, s32 target, s32 amount, s32 damage_effect_mode) {
    switch (target) {
    case 0:
        ((Actor *)actor_pool)[155].draw_flags &= 0xFFFE;
        RPG_GAUGE->charmed[0] = 0;
        RPG_GAUGE->hit_points[0] -= amount;
        if (RPG_GAUGE->hit_points[0] < 0) {
            RPG_GAUGE->hit_points[0] = 0;
        }
        if (amount == 0) {
            rpg_popup_miss_show(0, 0x6C, 0x77);
            return;
        }
        rpg_popup_number_show(popup_slot, 0x74, 0x77, amount, damage_effect_mode);
        return;
    case 1:
        ((Actor *)actor_pool)[156].draw_flags &= 0xFFFE;
        RPG_GAUGE->charmed[1] = 0;
        RPG_GAUGE->hit_points[1] -= amount;
        if (RPG_GAUGE->hit_points[1] < 0) {
            RPG_GAUGE->hit_points[1] = 0;
        }
        if (amount == 0) {
            rpg_popup_miss_show(0, 0x33, 0x67);
            return;
        }
        rpg_popup_number_show(popup_slot, 0x3B, 0x67, amount, damage_effect_mode);
        return;
    case 2:
        ((Actor *)actor_pool)[157].draw_flags &= 0xFFFE;
        RPG_GAUGE->charmed[2] = 0;
        RPG_GAUGE->hit_points[2] -= amount;
        if (RPG_GAUGE->hit_points[2] < 0) {
            RPG_GAUGE->hit_points[2] = 0;
        }
        if (amount == 0) {
            rpg_popup_miss_show(0, 0x30, 0x97);
            return;
        }
        rpg_popup_number_show(popup_slot, 0x34, 0x97, amount, damage_effect_mode);
        return;
    case 3:
        rpg_enemy_hit_points -= amount;
        if (rpg_enemy_hit_points < 0) {
            rpg_enemy_hit_points = 0;
        }
        if (amount == 0) {
            rpg_popup_miss_show(0, 0x38, 0x98);
            return;
        }
        rpg_popup_number_show(popup_slot, 0x40, 0x98, amount, damage_effect_mode);
        return;
    case 4:
        rpg_enemy_hit_points -= amount;
        if (rpg_enemy_hit_points < 0) {
            rpg_enemy_hit_points = 0;
        }
        if (amount == 0) {
            rpg_popup_miss_show(0, 0x38, 0x98);
            return;
        }
        rpg_popup_number_show(popup_slot, 0x40, 0x98, amount, damage_effect_mode);
        return;
    case 5:
        rpg_enemy_hit_points -= amount;
        if (rpg_enemy_hit_points < 0) {
            rpg_enemy_hit_points = 0;
        }
        if (amount == 0) {
            rpg_popup_miss_show(0, 0x38, 0x95);
            return;
        }
        rpg_popup_number_show(popup_slot, 0x40, 0x95, amount, damage_effect_mode);
        return;
    case 16:
        if (amount == 0) {
            rpg_popup_miss_show(0, 0x40, 0x97);
            return;
        }
        rpg_popup_number_show(popup_slot, 0x48, 0x97, amount, damage_effect_mode);
        return;
    case 17:
        if (amount == 0) {
            rpg_popup_miss_show(0, 0x40, 0x85);
            return;
        }
        rpg_popup_number_show(popup_slot, 0x48, 0x85, amount, damage_effect_mode);
        return;
    case 18:
        if (amount == 0) {
            rpg_popup_miss_show(0, 0x48, 0x8D);
            return;
        }
        rpg_popup_number_show(popup_slot, 0x50, 0x8D, amount, damage_effect_mode);
        return;
    case 32:
        rpg_player_hit_points -= amount;
        if (rpg_player_hit_points < 0) {
            rpg_player_hit_points = 0;
        }
        rpg_popup_number_show(popup_slot, 0xF4, 0x96, amount, damage_effect_mode);
        return;
    case 33:
        if (damage_effect_mode == 1) {
            rpg_player_hit_points -= amount;
            if (rpg_player_hit_points < 0) {
                rpg_player_hit_points = 0;
            }
        } else {
            rpg_player_hit_points += amount;
            if (rpg_player_hit_points_maximum < rpg_player_hit_points) {
                rpg_player_hit_points = rpg_player_hit_points_maximum;
            }
        }
        rpg_popup_number_show(popup_slot, 0x10C, 0x66, amount, damage_effect_mode);
        return;
    case 35:
        rpg_popup_number_show(popup_slot, 0x114, 0x96, amount, damage_effect_mode);
        /* fallthrough */
    default:
        return;
    }
}

void rpg_set_target_position(u32 target) {
    switch (target) {
    case 0:
        rpg_target_x = 0x78;
        rpg_target_y = 0x68;
        return;
    case 1:
        rpg_target_x = 0x40;
        rpg_target_y = 0x58;
        return;
    case 2:
        rpg_target_x = 0x38;
        rpg_target_y = 0x88;
        return;
    case 3:
        rpg_target_x = 0x48;
        rpg_target_y = 0x60;
        return;
    case 4:
        rpg_target_x = 0x48;
        rpg_target_y = 0x60;
        return;
    case 5:
        rpg_target_x = 0x48;
        rpg_target_y = 0x60;
        return;
    case 16:
        rpg_target_x = 0x48;
        rpg_target_y = 0x60;
        return;
    case 17:
        rpg_target_x = 0x48;
        rpg_target_y = 0x60;
        return;
    case 18:
        rpg_target_x = 0x48;
        rpg_target_y = 0x60;
        /* fallthrough */
    default:
        return;
    }
}

void rpg_set_target_position_alt(u32 target) {
    switch (target) {
    case 0:
        rpg_target_x = 0x78;
        rpg_target_y = 0x68;
        return;
    case 1:
        rpg_target_x = 0x40;
        rpg_target_y = 0x58;
        return;
    case 2:
        rpg_target_x = 0x38;
        rpg_target_y = 0x88;
        return;
    case 3:
        rpg_target_x = 0x48;
        rpg_target_y = 0x40;
        return;
    case 4:
        rpg_target_x = 0x48;
        rpg_target_y = 0x50;
        return;
    case 5:
        rpg_target_x = 0x48;
        rpg_target_y = 0x60;
        return;
    case 16:
        rpg_target_x = 0x48;
        rpg_target_y = 0x60;
        return;
    case 17:
        rpg_target_x = 0x48;
        rpg_target_y = 0x60;
        return;
    case 18:
        rpg_target_x = 0x48;
        rpg_target_y = 0x60;
        /* fallthrough */
    default:
        return;
    }
}

void rpg_se_play_latched_6xx(s32 a, s32 b, s32 c) {
    if (b == 0) {
        b = -1;
    }
    if (rpg_se_latch[c] == 0) {
        audio_command((u16)((a & 0xFF) | 0x600));
        rpg_se_latch[c] = b;
    }
    rpg_se_latch[c] -= 1;
}

void rpg_se_play_latched_5xx(s32 a, s32 b, s32 c) {
    if (b == 0) {
        b = -1;
    }
    if (rpg_se_latch[c] == 0) {
        audio_command((u16)((a & 0xFF) | 0x500));
        rpg_se_latch[c] = b;
    }
    rpg_se_latch[c] -= 1;
}

void rpg_voice_play_once(s32 a, s32 b, s32 c) {
    s32 *p;

    p = &rpg_voice_started[0][c];
    if (*p == 0) {
        game_state.current_girl = GIRL_SOTOI;
        cd_audio_request_set(a, b, 0xE);
        audio_command(0x300);
        *p = 1;
    }
}

void rpg_voice_reset(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        rpg_voice_started[i][0] = 0;
        rpg_voice_started[i][1] = 0;
        rpg_voice_started[i][2] = 0;
        rpg_voice_started[i][3] = 0;
    }
}

void rpg_voice_wait(s32 line_count) {
    if (rpg_dialog_step[1] == 0) {
        rpg_voice_wait_frames += 1;
        if (cd_audio_state() == 4 || rpg_voice_wait_frames >= 0x3D) {
            if (rpg_voice_wait_frames >= 0x3D) {
                audio_command(0x74);
            }
            rpg_message.unk_04 = 4;
            rpg_dialog_step[1] = 1;
            rpg_voice_wait_frames = 0;
        }
    }
    if (rpg_message.unk_04 == 0) {
        rpg_dialog_step[1] = 0;
        rpg_dialog_step[0] += 1;
    }
    if (line_count != 0 && rpg_message.unk_00 - 3 >= line_count) {
        rpg_dialog_step[1] = 0;
        rpg_dialog_step[0] += 1;
    }
}

/* FAKE: XOR zero retains the original repeated store; the final dead test keeps
 * value in v0 on the implicit-return paths. T-9720. */
s32 rpg_voice_wait_alt_unused(s32 arg0, s32 arg1) {
    s32 value;

    value = rpg_dialog_step[1];
    if (value == 0) {
        value = rpg_message_line_frames;
        rpg_message_line_frames = arg1;
    }
    if (rpg_message.unk_04 == 0) {
        rpg_message_line_frames = value;
        value = 0;
        rpg_dialog_step[0] += 1;
    }
    rpg_dialog_step[1] = value ^ 0;
    if (arg0 != 0) {
        rpg_dialog_step[1] = value;
        if ((rpg_message.unk_00 - 3) >= arg0) {
            rpg_message_line_frames = value;
            rpg_dialog_step[0] += 1;
            value = 0;
            rpg_dialog_step[1] = 0;
        }
    }
    if (value) {
    }
}

void rpg_effects_shift(s32 arg0, s32 arg1) {
    s32 i;
    Actor *p;

    p = (Actor *)actor_pool;
    for (i = 40; i < 64; i++) {
        p[96 + i].x += arg0;
        p[96 + i].y += arg1;
    }
    ((Actor *)actor_pool)[125].x += arg0;
    ((Actor *)actor_pool)[126].x += arg0;
    ((Actor *)actor_pool)[127].x += arg0;
    ((Actor *)actor_pool)[125].y += arg1;
    ((Actor *)actor_pool)[126].y += arg1;
    ((Actor *)actor_pool)[127].y += arg1;
}
