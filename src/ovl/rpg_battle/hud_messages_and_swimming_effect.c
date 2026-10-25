/* hud_messages_and_swimming_effect.c: src/ovl/RPG_BAT/8014ADD0.c:84: swimming effect plus HUD numbers, message windows, command menu and damage popups; body 88: div = 1; 89: for (i = 0; i != 4; i++, div *= 10) {. Original object 0x8014ADD0-0x8014D380 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_actor_set_animation /* implicit int view: the call's $v0 moves the rpg_menu state of rpg_command_menu_update to $v1 (T-24301) */
#include "common.h"
#include "ovl/rpg_battle.h"

s32 rpg_actor_set_animation();

/* s32 without a return: keeps the dead `move $v0, $zero` of the `&&` truth value in the original (T-24124). */
s32 rpg_swimming_main_effect(s32 arg0, s32 arg1) {
    if (rpg_player_timer == 0) {
        rpg_se_play_latched_6xx(0x501, 1, 0);
        rpg_actor_set_position(0x2A, arg0, arg1);
        rpg_actor_set_position(0x2B, arg0, arg1);
        rpg_actor_set_position(0x2F, arg0, arg1);
        rpg_actor_set_animation_alt(0x2B, 0xE, 5, 1);
    }
    if (rpg_player_timer == 1) {
        rpg_actor_set_animation_alt(0x2A, 9, 1, 1);
    }
    if ((rpg_effect_phase_0 > 0) && (rpg_effect_phase_0 < 0xF)) {
        if (((rpg_effect_phase_3 == 0) || (rpg_effect_phase_3 >= 9)) && ((rpg_effect_phase_2 == 0) || (rpg_effect_phase_2 >= 9)) && (rpg_effect_phase_1 == 0) && (rpg_rand(5) == 0)) {
            rpg_effect_phase_1 = 1;
            rpg_actor_set_position(0x2C, (rpg_rand(0x20) + arg0) - 0x10, arg1 - 8);
            rpg_actor_set_animation_alt(0x2C, 0xA, 1, 1);
        }
        if (((rpg_effect_phase_1 == 0) || (rpg_effect_phase_1 >= 9)) && ((rpg_effect_phase_3 == 0) || (rpg_effect_phase_3 >= 9)) && (rpg_effect_phase_2 == 0) && (rpg_rand(5) == 0)) {
            rpg_effect_phase_2 = 1;
            rpg_actor_set_position(0x2D, (rpg_rand(0x20) + arg0) - 0x10, arg1 - 8);
            rpg_actor_set_animation_alt(0x2D, 0xB, 1, 1);
        }
        if (((rpg_effect_phase_2 == 0) || (rpg_effect_phase_2 >= 9)) && ((rpg_effect_phase_1 == 0) || (rpg_effect_phase_1 >= 9)) && (rpg_effect_phase_3 == 0) && (rpg_rand(5) == 0)) {
            rpg_effect_phase_3 = 1;
            rpg_actor_set_position(0x2E, (rpg_rand(0x20) + arg0) - 0x10, arg1 - 8);
            rpg_actor_set_animation_alt(0x2E, 0xC, 1, 1);
        }
    }
    if (rpg_effect_phase_1 != 0) {
        ((Actor *)actor_pool)[140].y = (arg1 - (rpg_effect_phase_1 * 4)) - 0x80;
        rpg_effect_phase_1 += 1;
        if (rpg_effect_phase_1 >= 0x21) {
            rpg_effect_phase_1 = 0;
        }
    }
    if (rpg_effect_phase_2 != 0) {
        ((Actor *)actor_pool)[141].y = (arg1 - (rpg_effect_phase_2 * 4)) - 0x80;
        rpg_effect_phase_2 += 1;
        if (rpg_effect_phase_2 >= 0x21) {
            rpg_effect_phase_2 = 0;
        }
    }
    if (rpg_effect_phase_3 != 0) {
        ((Actor *)actor_pool)[142].y = (arg1 - (rpg_effect_phase_3 * 4)) - 0x80;
        rpg_effect_phase_3 += 1;
        if (rpg_effect_phase_3 >= 0x21) {
            rpg_effect_phase_3 = 0;
        }
    }
    if (!(((Actor *)actor_pool)[139].flags & 1)) {
        rpg_actor_set_animation_alt(0x2B, 0xF, 5, 1);
        rpg_effect_phase_0 += 1;
        if (rpg_effect_phase_0 == 5) {
            rpg_se_play_latched_6xx(0x502, 1, 0);
            rpg_actor_set_animation_alt(0x2F, 0x10, 5, 1);
        }
        if (rpg_effect_phase_0 == 0x10) {
            rpg_actor_set_animation_alt(0x2B, 0x11, 5, 1);
            rpg_actor_set_animation_alt(0x2C, 0xFF, 1, 0);
            rpg_actor_set_animation_alt(0x2D, 0xFF, 1, 0);
            rpg_actor_set_animation_alt(0x2E, 0xFF, 1, 0);
        }
        if (rpg_effect_phase_0 == 0x11) {
            rpg_actor_set_animation_alt(0x2B, 0xFF, 1, 0);
            rpg_player_step_next();
        }
    }
    if ((rpg_effect_phase_0 == 0x10) && (((Actor *)actor_pool)[139].unk_18 == 0x10)) {
        rpg_actor_set_animation_alt(0x2A, 0xFF, 1, 0);
    }
    if (!(((Actor *)actor_pool)[143].flags & 1)) {
        rpg_actor_set_animation_alt(0x2F, 0xFF, 1, 0);
    }
    rpg_player_timer += 1;
}

void rpg_hud_numbers_update(void) {
    s32 i;
    s32 div;

    div = 1;
    for (i = 0; i != 4; i++, div *= 10) {
        k_disp_switch(rpg_label_player, 1);
        if (rpg_player_hit_points / div != 0) {
            rpg_actor_set_animation(0x13 - i, ((rpg_player_hit_points / div) % 10) + 9, 1, 1);
        } else {
            rpg_actor_set_animation(0x13 - i, 0xFF, 1, 0);
        }
        if (rpg_girl_hit_points >= 0x10000001) {
            if (rpg_girl_id != 0xE) {
                k_disp_switch(rpg_label_girl, 0);
            }
            ((Actor *)actor_pool)[129].draw_flags = 0;
            ((Actor *)actor_pool)[119 - i].draw_flags = 0;
        } else {
            k_disp_switch(rpg_label_girl, 1);
            ((Actor *)actor_pool)[129].draw_flags = 0x80;
            if (rpg_girl_hit_points / div != 0) {
                rpg_actor_set_animation(0x17 - i, ((rpg_girl_hit_points / div) % 10) + 9, 1, 1);
            } else {
                rpg_actor_set_animation(0x17 - i, 0xFF, 1, 0);
            }
        }
        if (rpg_guest_value >= 0x10000001) {
            if (rpg_encounter & 0xF0000) {
                k_disp_switch(rpg_label_guest, 0);
            }
            if (rpg_girl_id != 0xE) {
                ((Actor *)actor_pool)[128].draw_flags = 0;
                ((Actor *)actor_pool)[123 - i].draw_flags = 0;
            } else {
                ((Actor *)actor_pool)[129].draw_flags = 0;
                ((Actor *)actor_pool)[119 - i].draw_flags = 0;
            }
        } else {
            k_disp_switch(rpg_label_guest, 1);
            if (rpg_girl_id != 0xE) {
                ((Actor *)actor_pool)[128].draw_flags = 0x80;
                if (rpg_guest_value / div != 0) {
                    rpg_actor_set_animation(0x1B - i, ((rpg_guest_value / div) % 10) + 9, 1, 1);
                } else {
                    rpg_actor_set_animation(0x1B - i, 0xFF, 1, 0);
                }
            } else {
                ((Actor *)actor_pool)[129].draw_flags = 0x80;
                if (rpg_guest_value / div != 0) {
                    rpg_actor_set_animation(0x17 - i, ((rpg_guest_value / div) % 10) + 9, 1, 1);
                } else {
                    rpg_actor_set_animation(0x1B - i, 0xFF, 1, 0);
                }
            }
        }
    }
    if (rpg_player_hit_points == 0) {
        k_disp_switch(rpg_label_player, 1);
        rpg_actor_set_animation(0x13, 9, 1, 1);
    }
}

void rpg_message_show(s32 line_texts, s32 line_count, s32 frames_per_line) {
    k_sub_reset();
    rpg_message_lines = line_texts;
    rpg_message_line_frames = frames_per_line;
    rpg_message_line_count = line_count;
    rpg_message.unk_00 = 0;
    rpg_message.unk_08 = 0;
    rpg_message.unk_04 = 1;
}

/* rpg_message_mode and rpg_message_frame are the scalar views of rpg_message.unk_04 and .unk_08: uopt only
 * promotes them into registers as scalars (as in the other RPG_BAT files); T-17110. */
void rpg_message_update(void) {
    s32 pad; /* FAKE: unused local, reserves the top spill word so idx sits at 0x30(sp); T-17110 */
    s32 idx;

    if (rpg_message_mode != 0) {
        switch (rpg_message.unk_00) {
        case 0:
            rpg_actor_set_animation(0x24, 0x32, 5, 1);
            rpg_message_next_state(0);
            break;
        case 1:
            if (rpg_actor_is_idle(0x24) != 0) {
                rpg_message_next_state(0);
            }
            break;
        case 2:
            rpg_actor_set_animation(0x24, 3, 1, 1);
            rpg_message_next_state(0);
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            idx = rpg_message.unk_00 - 3;
            if (idx >= rpg_message_line_count) {
                rpg_message.unk_00 = 0xFFFE;
            } else {
                rpg_message_frame += 1;
                if (rpg_message_frame == 1) {
                    k_disp_start(set_kanji_string(-0x7E, -0x66, 0, rpg_message_lines[idx], 0));
                }
                if (rpg_message_mode == 4 && cd_audio_state() == 2) {
                    rpg_message_frame = rpg_message_line_frames + 1;
                    rpg_message_mode = 1;
                }
                if (rpg_message_line_frames < rpg_message_frame) {
                    switch (rpg_message_mode) {
                    case 1:
                        k_sub_reset();
                        rpg_message_next_state(0);
                        break;
                    case 3:
                        /* empty on purpose: gives the compare tree of the original; T-17110 */
                        break;
                    case 4:
                        rpg_message_mode = 1;
                        break;
                    case 0x80:
                    case 0x81:
                    case 0x82:
                    case 0x83:
                    case 0x84:
                    case 0x85:
                    case 0x86:
                    case 0x87:
                    case 0x88:
                    case 0x89:
                    case 0x8A:
                        if (idx != (rpg_message_mode & 0x7F)) {
                            k_sub_reset();
                            rpg_message_next_state(0);
                        }
                        break;
                    }
                }
            }
            break;
        case 0xFFFE:
            rpg_actor_set_animation(0x24, 0x32, 0x21, 1);
            ((Actor *) actor_pool)[132].unk_18 = 6;
            rpg_message_next_state(0);
            break;
        case 0xFFFF:
        default:
            if (rpg_actor_is_idle(0x24) != 0) {
                rpg_actor_set_animation(0x24, 0xFF, 0, 0);
                rpg_message_frame = 0;
                rpg_message.unk_00 = 0;
                rpg_message_mode = 0;
            }
            break;
        }
    }
}

void rpg_message_set_wait_choice_unused(s32 message_wait_type) {
    switch (message_wait_type) {
    case 1:
        rpg_message.unk_04 = 0x80;
        return;
    case 2:
        rpg_message.unk_04 = 0x81;
        return;
    case 3:
        rpg_message.unk_04 = 0x82;
        return;
    case 4:
        rpg_message.unk_04 = 0x83;
        return;
    case 5:
        rpg_message.unk_04 = 0x84;
        return;
    case 6:
        rpg_message.unk_04 = 0x85;
        return;
    case 7:
        rpg_message.unk_04 = 0x86;
        return;
    case 8:
        rpg_message.unk_04 = 0x87;
        return;
    case 9:
        rpg_message.unk_04 = 0x88;
        return;
    case 10:
        rpg_message.unk_04 = 0x89;
        return;
    default:
        rpg_message.unk_04 = 1;
        return;
    }
}

void rpg_command_menu_update(void) {
    /* T-24522: pointer reads keep the menu choices separate from the condition range. */
    GameState *gs;
    RpgMenuPositions sp34;

    gs = &game_state;
    sp34 = rpg_menu_item_positions;
    if (rpg_menu.unk_00 != 3 && !(game_state.menu_mode & 1)) {
        hud_cursor_update();
    }
    if (!(rpg_player_action & RPG_PLAYER_READY) && rpg_menu.unk_00 != 4 && rpg_menu.unk_00 != 5) {
        rpg_menu.unk_00 = 6;
    }
    switch (rpg_menu.unk_00) {
    case 0:
        rpg_actor_set_animation(0x25, 0x31, 5, 1);
        if (rpg_special_ready == 1) {
            menu_set(0, 3, sp34.coordinates, &sp34.coordinates[3], &sp34.coordinates[6], &sp34.coordinates[9]);
        } else {
            menu_set(0, 2, sp34.coordinates, &sp34.coordinates[3], &sp34.coordinates[6], &sp34.coordinates[9]);
        }
        rpg_message_next_state(1);
    default:
        break;
    case 1:
        if (rpg_actor_is_idle(0x25) != 0) {
            rpg_message_next_state(1);
        }
        break;
    case 2:
        rpg_actor_set_animation(0x25, 2, 1, 1);
        if (rpg_special_ready == 1) {
            rpg_actor_set_animation(0x23, 0xFF, 1, 1);
        } else {
            rpg_actor_set_animation(0x23, 0xFF, 1, 0);
        }
        rpg_message_next_state(1);
        break;
    case 3:
        menu_check(0, cursor_x, cursor_y);
        menu_cursor_update(0);
        rpg_actor_set_animation(0x22, 0xFF, 1, 1);
        switch (gs->menu_cursor_0) {
        case 0:
            rpg_actor_set_animation(0x22, 5, 1, 1);
            break;
        case 1:
            rpg_actor_set_animation(0x22, 6, 1, 1);
            break;
        case 2:
            rpg_actor_set_animation(0x22, 7, 1, 1);
            break;
        case -1:
            rpg_actor_set_animation(0x22, 0xFF, 1, 0);
            break;
        }
        if ((game_state.menu_cursor_0 != -1) && (game_state.pad_pressed & PAD_CIRCLE)) {
            rpg_actor_set_animation(0x22, 0xFF, 1, 0);
            rpg_actor_set_animation(0x23, 0xFF, 1, 0);
            switch (gs->menu_cursor_0) {
            case 0:
                rpg_player_action ^= RPG_COMMAND_ATTACK;
                break;
            case 1:
                rpg_player_action ^= RPG_COMMAND_GUARD;
                break;
            case 2:
                rpg_player_action ^= RPG_COMMAND_SPECIAL;
                break;
            }
            rpg_message_next_state(1);
        }
        break;
    case 4:
        rpg_actor_set_animation(0x25, 0x31, 0x21, 1);
        ((Actor *)actor_pool)[133].unk_18 = 6;
        rpg_actor_set_animation(0x22, 0xFF, 1, 0);
        rpg_actor_set_animation(0x23, 0xFF, 1, 0);
        rpg_message_next_state(1);
        break;
    case 5:
        if (rpg_actor_is_idle(0x25) != 0) {
            rpg_actor_set_animation(0x25, 0xFF, 1, 0);
            rpg_message_next_state(1);
        }
        break;
    case 6:
        rpg_player_action &= 0xFFFEFFFF;
        rpg_actor_set_animation(0x22, 0xFF, 1, 0);
        rpg_actor_set_animation(0x23, 0xFF, 1, 0);
        rpg_actor_set_animation(0x25, 0xFF, 1, 0);
        rpg_menu.unk_00 = 0;
        break;
    }
}

/* Number display of slot arg0 (records 64 + 12 * slot, three rows of four digits) at x, y; T-25144. */
void rpg_popup_number_show(s32 popup_slot, s32 x, s32 y, s32 amount, s32 popup_kind) {
    s32 i;
    s32 j;
    s32 idx;
    s32 div;
    s32 k;
    s32 t[1]; /* FAKE: one-word array keeps slot * 12 in memory at sp+0x50, reloaded after the loop and kept out of CSE; T-25144 */

    x -= 0xA0;
    y -= 0x78;
    if (amount < 10) {
        /* FAKE: empty arm; its label makes ugen reload x and amount in each test, as the original does; T-25144 */
    } else if (amount < 100) {
        x += 4;
    } else if (amount < 1000) {
        x += 9;
    } else {
        x += 13;
    }
    div = 1;
    switch (popup_slot) {
    case 0:
        rpg_popup_0 = 0;
        D_8015EE60 = 1;
        D_8015EE64 = 0;
        break;
    case 1:
        rpg_popup_1 = 0;
        D_8015EE6C = 1;
        D_8015EE70 = 0;
        break;
    case 2:
        rpg_popup_2 = 0;
        D_8015EE78 = 1;
        D_8015EE7C = 0;
        break;
    case 3:
        rpg_popup_3 = 0;
        D_8015EE84 = 1;
        D_8015EE88 = 0;
        break;
    }
    /* FAKE: one line, so as1 schedules the record pointer before the store of t[0]; T-25144 */
    t[0] = popup_slot * 12; j = t[0]; for (i = 0; i < 4; j++, i++) {
        k = j + 0x40;
        ((Actor *) actor_pool)[64 + j].x = x - i * 9;
        ((Actor *) actor_pool)[64 + j].y = y;
        ((Actor *) actor_pool)[68 + j].x = x - i * 9;
        ((Actor *) actor_pool)[68 + j].y = y;
        ((Actor *) actor_pool)[72 + j].x = x - i * 9;
        ((Actor *) actor_pool)[72 + j].y = y;
        if (amount / div != 0) {
            rpg_record_set_animation(k + 4, amount / div % 10 + 0x27, 0);
            rpg_record_set_animation(k + 8, amount / div % 10 + 0x27, 0);
            if (popup_kind == 0) {
                rpg_record_set_animation(k, amount / div % 10 + 0x1D, 0);
            } else {
                rpg_record_set_animation(k, amount / div % 10 + 0x13, 0);
            }
        } else {
            ((Actor *) actor_pool)[k].x = 0xA0;
            ((Actor *) actor_pool)[k].y = 0x78;
            ((Actor *) actor_pool)[k + 4].x = 0xA0;
            ((Actor *) actor_pool)[k + 4].y = 0x78;
            ((Actor *) actor_pool)[k + 8].x = 0xA0;
            ((Actor *) actor_pool)[k + 8].y = 0x78;
        }
        div *= 10;
    }
    if (amount == 0) {
        ((Actor *) actor_pool)[64 + t[0]].y = y;
        ((Actor *) actor_pool)[68 + t[0]].y = y;
        ((Actor *) actor_pool)[72 + t[0]].y = y;
        ((Actor *) actor_pool)[64 + t[0]].x = x;
        ((Actor *) actor_pool)[68 + t[0]].x = x;
        ((Actor *) actor_pool)[72 + t[0]].x = x;
        rpg_record_set_animation(t[0] + 0x44, 0x27, 0);
        idx = t[0] + 0x40;
        rpg_record_set_animation(idx + 8, 0x27, 0);
        if (popup_kind == 0) {
            rpg_record_set_animation(idx, 0x1D, 0);
        } else {
            rpg_record_set_animation(idx, 0x13, 0);
        }
    }
}

void rpg_popup_miss_show(s32 popup_slot, s32 x, s32 y) {
    s32 t;
    s32 u;
    s32 i;
    Actor *rec;

    x -= 0x9E;
    y -= 0x78;
    switch (popup_slot) {
    case 0:
        rpg_popup_0 = 0;
        D_8015EE60 = 1;
        D_8015EE64 = 0;
        break;
    case 1:
        rpg_popup_1 = 0;
        D_8015EE6C = 1;
        D_8015EE70 = 0;
        break;
    case 2:
        rpg_popup_2 = 0;
        D_8015EE78 = 1;
        D_8015EE7C = 0;
        break;
    case 3:
        rpg_popup_3 = 0;
        D_8015EE84 = 1;
        D_8015EE88 = 0;
        break;
    }
    rec = (Actor *) actor_pool;
    t = popup_slot * 12;
    u = t + 0x40; /* named local: t and u get the two spill slots of the original; T-17110 */
    rec[64 + t].x = x;
    rec[64 + t].y = y;
    rec[68 + t].x = x;
    rec[68 + t].y = y;
    rec[72 + t].x = x;
    rec[72 + t].y = y;
    rpg_record_set_animation(u, 0x34, 0);
    rpg_record_set_animation(u + 4, 0x35, 0);
    rpg_record_set_animation(u + 8, 0x35, 0);
    for (i = 1; i < 4; i++) {
        rec[64 + t + i].x = 0xA0;
        rec[64 + t + i].y = 0x78;
        rec[68 + t + i].x = 0xA0;
        rec[68 + t + i].y = 0x78;
        rec[72 + t + i].x = 0xA0;
        rec[72 + t + i].y = 0x78;
    }
}

void rpg_popup_bounce_0(void) {
    s32 half;
    s32 i;
    s16 *p;

    if (D_8015EE60 == 1) {
        half = D_8015EE64 / 2;
        switch (half) {
        case 0:
            rpg_popup_digit_set_animation(0, 0xFF, 1);
            rpg_popup_digit_set_animation(4, 0xFF, 1);
            rpg_popup_digit_set_animation(8, 0xFF, 1);
            break;
        case 1:
            rpg_popup_digit_set_animation(1, 0xFF, 1);
            rpg_popup_digit_set_animation(5, 0xFF, 1);
            rpg_popup_digit_set_animation(9, 0xFF, 1);
            break;
        case 2:
            rpg_popup_digit_set_animation(2, 0xFF, 1);
            rpg_popup_digit_set_animation(6, 0xFF, 1);
            rpg_popup_digit_set_animation(0xA, 0xFF, 1);
            break;
        case 3:
            rpg_popup_digit_set_animation(3, 0xFF, 1);
            rpg_popup_digit_set_animation(7, 0xFF, 1);
            rpg_popup_digit_set_animation(0xB, 0xFF, 1);
            break;
        }
        if (half >= 0x10) {
            half = 0xF;
        }
        if (!(D_8015EE64 & 1)) {
            p = rpg_popup_bounce_table[half];
            D_8011FDFA += p[0];
            D_8011FF0A += p[1];
            D_8012001A += p[2];
            D_8011FE3E += p[3];
            D_8011FF4E += p[4];
            D_8012005E += p[5];
            D_8011FE82 += p[6];
            D_8011FF92 += p[7];
            D_801200A2 += p[8];
            D_8011FEC6 += p[9];
            D_8011FFD6 += p[10];
            D_801200E6 += p[11];
        }
        D_8015EE64 += 1;
        if (D_8015EE64 / 2 >= 0x1A) {
            for (i = 0; i != 0xC; i++) {
                rpg_popup_digit_set_animation(i, 0xFF, 0);
            }
            D_8015EE60 = 0;
        }
    }
}

void rpg_popup_bounce_1(void) {
    s32 half;
    s32 i;
    s16 *p;

    if (D_8015EE6C == 1) {
        half = D_8015EE70 / 2;
        switch (half) {
        case 0:
            rpg_popup_digit_set_animation(0xC, 0xFF, 1);
            rpg_popup_digit_set_animation(0x10, 0xFF, 1);
            rpg_popup_digit_set_animation(0x14, 0xFF, 1);
            break;
        case 1:
            rpg_popup_digit_set_animation(0xD, 0xFF, 1);
            rpg_popup_digit_set_animation(0x11, 0xFF, 1);
            rpg_popup_digit_set_animation(0x15, 0xFF, 1);
            break;
        case 2:
            rpg_popup_digit_set_animation(0xE, 0xFF, 1);
            rpg_popup_digit_set_animation(0x12, 0xFF, 1);
            rpg_popup_digit_set_animation(0x16, 0xFF, 1);
            break;
        case 3:
            rpg_popup_digit_set_animation(0xF, 0xFF, 1);
            rpg_popup_digit_set_animation(0x13, 0xFF, 1);
            rpg_popup_digit_set_animation(0x17, 0xFF, 1);
            break;
        }
        if (half >= 0x10) {
            half = 0xF;
        }
        if (!(D_8015EE70 & 1)) {
            p = rpg_popup_bounce_table[half];
            D_8012012A += p[0];
            D_8012023A += p[1];
            D_8012034A += p[2];
            D_8012016E += p[3];
            D_8012027E += p[4];
            D_8012038E += p[5];
            D_801201B2 += p[6];
            D_801202C2 += p[7];
            D_801203D2 += p[8];
            D_801201F6 += p[9];
            D_80120306 += p[10];
            D_80120416 += p[11];
        }
        D_8015EE70 += 1;
        if (D_8015EE70 / 2 >= 0x1A) {
            for (i = 0; i != 0xC; i++) {
                rpg_popup_digit_set_animation(i + 0xC, 0xFF, 0);
            }
            D_8015EE6C = 0;
        }
    }
}

void rpg_popup_bounce_2(void) {
    s32 half;
    s32 i;
    s16 *p;

    if (D_8015EE78 == 1) {
        half = D_8015EE7C / 2;
        switch (half) {
        case 0:
            rpg_popup_digit_set_animation(0x18, 0xFF, 1);
            rpg_popup_digit_set_animation(0x1C, 0xFF, 1);
            rpg_popup_digit_set_animation(0x20, 0xFF, 1);
            break;
        case 1:
            rpg_popup_digit_set_animation(0x19, 0xFF, 1);
            rpg_popup_digit_set_animation(0x1D, 0xFF, 1);
            rpg_popup_digit_set_animation(0x21, 0xFF, 1);
            break;
        case 2:
            rpg_popup_digit_set_animation(0x1A, 0xFF, 1);
            rpg_popup_digit_set_animation(0x1E, 0xFF, 1);
            rpg_popup_digit_set_animation(0x22, 0xFF, 1);
            break;
        case 3:
            rpg_popup_digit_set_animation(0x1B, 0xFF, 1);
            rpg_popup_digit_set_animation(0x1F, 0xFF, 1);
            rpg_popup_digit_set_animation(0x23, 0xFF, 1);
            break;
        }
        if (half >= 0x10) {
            half = 0xF;
        }
        if (!(D_8015EE7C & 1)) {
            p = rpg_popup_bounce_table[half];
            D_8012045A += p[0];
            D_8012056A += p[1];
            D_8012067A += p[2];
            D_8012049E += p[3];
            D_801205AE += p[4];
            D_801206BE += p[5];
            D_801204E2 += p[6];
            D_801205F2 += p[7];
            D_80120702 += p[8];
            D_80120526 += p[9];
            D_80120636 += p[10];
            D_80120746 += p[11];
        }
        D_8015EE7C += 1;
        if (D_8015EE7C / 2 >= 0x1A) {
            for (i = 0; i != 0xC; i++) {
                rpg_popup_digit_set_animation(i + 0x18, 0xFF, 0);
            }
            D_8015EE78 = 0;
        }
    }
}

void rpg_popup_bounce_3(void) {
    s32 half;
    s32 i;
    s16 *p;

    if (D_8015EE84 == 1) {
        half = D_8015EE88 / 2;
        switch (half) {
        case 0:
            rpg_popup_digit_set_animation(0x24, 0xFF, 1);
            rpg_popup_digit_set_animation(0x28, 0xFF, 1);
            rpg_popup_digit_set_animation(0x2C, 0xFF, 1);
            break;
        case 1:
            rpg_popup_digit_set_animation(0x25, 0xFF, 1);
            rpg_popup_digit_set_animation(0x29, 0xFF, 1);
            rpg_popup_digit_set_animation(0x2D, 0xFF, 1);
            break;
        case 2:
            rpg_popup_digit_set_animation(0x26, 0xFF, 1);
            rpg_popup_digit_set_animation(0x2A, 0xFF, 1);
            rpg_popup_digit_set_animation(0x2E, 0xFF, 1);
            break;
        case 3:
            rpg_popup_digit_set_animation(0x27, 0xFF, 1);
            rpg_popup_digit_set_animation(0x2B, 0xFF, 1);
            rpg_popup_digit_set_animation(0x2F, 0xFF, 1);
            break;
        }
        if (half >= 0x10) {
            half = 0xF;
        }
        if (!(D_8015EE88 & 1)) {
            p = rpg_popup_bounce_table[half];
            D_8012078A += p[0];
            D_8012089A += p[1];
            D_801209AA += p[2];
            D_801207CE += p[3];
            D_801208DE += p[4];
            D_801209EE += p[5];
            D_80120812 += p[6];
            D_80120922 += p[7];
            D_80120A32 += p[8];
            D_80120856 += p[9];
            D_80120966 += p[10];
            D_80120A76 += p[11];
        }
        D_8015EE88 += 1;
        if (D_8015EE88 / 2 >= 0x1A) {
            for (i = 0; i != 0xC; i++) {
                rpg_popup_digit_set_animation(i + 0x24, 0xFF, 0);
            }
            D_8015EE84 = 0;
        }
    }
}

void rpg_message_format_damage(u8 *dst, u8 *name, u8 *label, s32 num, u8 *suffix) {
    u32 quot;
    s32 place;
    s32 base;

    strcpy(dst, name);
    strcat(dst, label);
    if (num == 0) {
        strcat(dst, "０");
    }
    /* radix in one variable: with literal 10 the unsigned % and the signed / keep two copies in
     * two $s registers; T-17110 */
    base = 10;
    for (place = 0x3E8; place > 0; place = place / base) {
        quot = num / place;
        if (quot != 0) {
            switch (quot % base) {
            case 0:
                strcat(dst, "０");
                break;
            case 1:
                strcat(dst, "１");
                break;
            case 2:
                strcat(dst, "２");
                break;
            case 3:
                strcat(dst, "３");
                break;
            case 4:
                strcat(dst, "４");
                break;
            case 5:
                strcat(dst, "５");
                break;
            case 6:
                strcat(dst, "６");
                break;
            case 7:
                strcat(dst, "７");
                break;
            case 8:
                strcat(dst, "８");
                break;
            case 9:
                strcat(dst, "９");
                break;
            }
        }
    }
    strcat(dst, suffix);
}
