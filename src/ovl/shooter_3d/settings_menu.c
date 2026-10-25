/* settings_menu.c: src/ovl/TACO/80135720.c:11 dispatches menu steps; :95 calls changing-value animation or input; :102 renders settings; :341 processes menu cursor and button remapping. Original object 0x80135720-0x80136B40 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/shooter_3d.h"

void shooter_settings_reset_state(void) {
    shooter_settings_cursor = 0;
    shooter_settings_changing = 0;
    shooter_settings_old_value = 0;
    shooter_settings_animation_frame = 0;
}

s32 shooter_stage_settings(void) {
    switch (game_state.mode_step) {
    case 0:
        shooter_settings_step_init();
        break;
    case 1:
        shooter_settings_step_run();
        break;
    case 2:
        shooter_settings_step_exit();
        break;
    }
}

/* FAKE: k repeats the stride of i so that uopt strength-reduces arg5 + k and leaves k's initial 0 in v0
   (live at the exit of this implicit-int function); the original source is unknown. T-19023.
   "i = 0; do {" on one line orders the preheader moves as the original does. */
s32 shooter_draw_label(arg0, arg1, arg2, arg3, arg4, arg5, arg6)
u8 *arg0;
s32 arg1;
s32 arg2;
s32 arg3;
s32 arg4;
s32 arg5;
s32 arg6;
{
    s32 i;
    s32 k;
    u8 *p;

    k = 0;
    if (*arg0 != 0) {
        p = arg0;
        i = 0; do {
            shooter_draw_glyph(*p, arg3, 3, 0x80, arg5 + k, arg6, arg1, arg2, arg4);
            i += 0xD;
            p++;
            k += 0xD;
        } while (i < 0x111 && *p != 0);
    }
}

s32 shooter_settings_step_init(void) {
    switch (game_state.substep) {
    case 0:
        dec_bg_reset();
        cd_read_sectors(8, 0x80162000, 0xB378);
        shooter_settings_reset_state();
        game_state.substep += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            game_state.substep += 1;
        }
        break;
    case 2:
        bg_still_decode_to_vram(0x2C0, 0, 0x140, 0xF0);
        shooter_clear_text_strip();
        mode_step_next();
        break;
    }
}

void shooter_clear_text_strip(void) {
    RECT r;

    r.x = 0x2C0;
    r.y = 0;
    r.w = 0x140;
    r.h = 0x14;
    func_8009C7F8(&r, 0, 0, 0);
    func_8009C674(0);
}

void shooter_settings_step_exit(void) {
    mode_stage_set(2);
}

void shooter_settings_step_run(void) {
    if (shooter_settings_changing == 1) {
        shooter_settings_draw_changing();
    } else {
        shooter_settings_input();
    }
    dec_bg_draw(2);
    shooter_settings_draw();
}

/* Implicit-int function without a return value (v0 stays live, which keeps the switch temporaries,
   T-19023). i is declared before sp44 so that the array sits at 0x44; the loop body stays on the line of
   the for so that IDO stores element 0 first. */
s32 shooter_settings_draw(void) {
    s32 i;
    s32 sp44[6];

    for (i = 0; i < 6; i++) sp44[i] = 3;
    sp44[shooter_settings_cursor] = 5;
    prim_gradient_rect(-0x88, -0x1C, 0x10E, 0x78, 4, 0x3F3F3F, 0x81);
    shooter_draw_label("SETTING", 0x1000, 0x1000, sp44[5], 3, -0x30, -0x40);
    shooter_draw_label("AUTO SHOT", 0x1000, 0x1000, sp44[0], 3, -0x78, -0x10);
    shooter_draw_label("ROLL BUTTON", 0x1000, 0x1000, sp44[1], 3, -0x78, 8);
    shooter_draw_label("SHOT BUTTON", 0x1000, 0x1000, sp44[2], 3, -0x78, 0x20);
    shooter_draw_label("BOMB BUTTON", 0x1000, 0x1000, sp44[3], 3, -0x78, 0x38);
    shooter_draw_label("EXIT", 0x1000, 0x1000, sp44[4], 3, -0x78, 0x50);
    if (shooter_settings_changing != 1 || shooter_settings_cursor != 0) {
        switch (shooter_setting_auto_shot & 1) {
        case 0:
            shooter_draw_label("OFF", 0x1000, 0x1000, sp44[0], 3, 0x2A, -0x10);
            break;
        case 1:
            shooter_draw_label("ON", 0x1000, 0x1000, sp44[0], 3, 0x2A, -0x10);
            break;
        }
    }
    if (shooter_settings_changing != 1 || shooter_settings_cursor != 1) {
        switch (shooter_setting_roll_button % 3) {
        case 0:
            shooter_draw_label("R12-L12", 0x1000, 0x1000, sp44[1], 3, 0x2A, 8);
            break;
        case 1:
            shooter_draw_label("R1-L1", 0x1000, 0x1000, sp44[1], 3, 0x2A, 8);
            break;
        case 2:
            shooter_draw_label("R2-L2", 0x1000, 0x1000, sp44[1], 3, 0x2A, 8);
            break;
        }
    }
    if (shooter_settings_changing != 1 || shooter_settings_cursor != 2) {
        switch (shooter_setting_shot_button % 3) {
        case 0:
            shooter_draw_label("@", 0x1000, 0x1000, sp44[2], 3, 0x2A, 0x20);
            break;
        case 1:
            shooter_draw_label("*", 0x1000, 0x1000, sp44[2], 3, 0x2A, 0x20);
            break;
        case 2:
            shooter_draw_label("#", 0x1000, 0x1000, sp44[2], 3, 0x2A, 0x20);
            break;
        }
    }
    if (shooter_settings_changing != 1 || shooter_settings_cursor != 3) {
        switch (shooter_setting_bomb_button % 3) {
        case 0:
            shooter_draw_label("*", 0x1000, 0x1000, sp44[3], 3, 0x2A, 0x38);
            break;
        case 1:
            shooter_draw_label("#", 0x1000, 0x1000, sp44[3], 3, 0x2A, 0x38);
            break;
        case 2:
            shooter_draw_label("@", 0x1000, 0x1000, sp44[3], 3, 0x2A, 0x38);
            break;
        }
    }
}

void shooter_settings_draw_changing(void) {
    shooter_settings_animation_frame += 1;
    switch (shooter_settings_cursor) {
    case 0:
        shooter_settings_animation_auto_shot();
        break;
    case 1:
        shooter_settings_animation_roll();
        break;
    case 2:
        shooter_settings_animation_shot();
        break;
    case 3:
        shooter_settings_animation_bomb();
        break;
    }
    if (shooter_settings_animation_frame >= 0x1F) {
        shooter_settings_animation_frame = 0;
        shooter_settings_changing = 0;
    }
}

void shooter_settings_animation_auto_shot(void) {
    s32 t;

    if (shooter_settings_animation_frame < 0xF) {
        t = (0xF - shooter_settings_animation_frame << 0xC) / 15;
        switch (shooter_settings_old_value % 2) {
        case 0:
            shooter_draw_label("OFF", t, 0x1000, 5, 3, 0x2A, -0x10);
            return;
        case 1:
            shooter_draw_label("ON", t, 0x1000, 5, 3, 0x2A, -0x10);
            return;
        }
    } else {
        t = ((shooter_settings_animation_frame << 0xC) - 0xF000) / 15;
        switch (shooter_setting_auto_shot & 1) {
        case 0:
            shooter_draw_label("OFF", t, 0x1000, 5, 3, 0x2A, -0x10);
            return;
        case 1:
            shooter_draw_label("ON", t, 0x1000, 5, 3, 0x2A, -0x10);
            return;
        }
    }
}

void shooter_settings_animation_roll(void) {
    s32 old_roll_option;
    s32 temp_lo;
    s32 temp_lo_2;
    u32 new_roll_option;

    if (shooter_settings_animation_frame < 0xF) {
        temp_lo = (s32) ((0xF - shooter_settings_animation_frame) << 0xC) / 15;
        old_roll_option = (s32) shooter_settings_old_value % 3;
        switch (old_roll_option) {
        case 0:
            shooter_draw_label((s32) "R12-L12", temp_lo, 0x1000, 5, 3, 0x2A, 8);
            return;
        case 1:
            shooter_draw_label((s32) "R1-L1", temp_lo, 0x1000, 5, 3, 0x2A, 8);
            return;
        case 2:
            shooter_draw_label((s32) "R2-L2", temp_lo, 0x1000, 5, 3, 0x2A, 8);
            return;
        }
    } else {
        temp_lo_2 = (s32) ((shooter_settings_animation_frame << 0xC) + 0xFFFF1000) / 15;
        new_roll_option = (u8) shooter_setting_roll_button % 3U;
        switch (new_roll_option) {
        case 0:
            shooter_draw_label((s32) "R12-L12", temp_lo_2, 0x1000, 5, 3, 0x2A, 8);
            return;
        case 1:
            shooter_draw_label((s32) "R1-L1", temp_lo_2, 0x1000, 5, 3, 0x2A, 8);
            return;
        case 2:
            shooter_draw_label((s32) "R2-L2", temp_lo_2, 0x1000, 5, 3, 0x2A, 8);
            return;
        }
    }
}

void shooter_settings_animation_shot(void) {
    s32 old_shot_button_option;
    s32 temp_lo;
    s32 temp_lo_2;
    u32 shot_button_option;

    if (shooter_settings_animation_frame < 0xF) {
        temp_lo = (s32) ((0xF - shooter_settings_animation_frame) << 0xC) / 15;
        old_shot_button_option = (s32) shooter_settings_old_value % 3;
        switch (old_shot_button_option) {
        case 0:
            shooter_draw_label((s32) "@", temp_lo, 0x1000, 5, 3, 0x2A, 0x20);
            return;
        case 1:
            shooter_draw_label((s32) "*", temp_lo, 0x1000, 5, 3, 0x2A, 0x20);
            return;
        case 2:
            shooter_draw_label((s32) "#", temp_lo, 0x1000, 5, 3, 0x2A, 0x20);
            return;
        }
    } else {
        temp_lo_2 = (s32) ((shooter_settings_animation_frame << 0xC) + 0xFFFF1000) / 15;
        shot_button_option = (u8) shooter_setting_shot_button % 3U;
        switch (shot_button_option) {
        case 0:
            shooter_draw_label((s32) "@", temp_lo_2, 0x1000, 5, 3, 0x2A, 0x20);
            return;
        case 1:
            shooter_draw_label((s32) "*", temp_lo_2, 0x1000, 5, 3, 0x2A, 0x20);
            return;
        case 2:
            shooter_draw_label((s32) "#", temp_lo_2, 0x1000, 5, 3, 0x2A, 0x20);
            return;
        }
    }
}

void shooter_settings_animation_bomb(void) {
    s32 old_bomb_option;
    s32 temp_lo;
    s32 temp_lo_2;
    u32 new_bomb_option;

    if (shooter_settings_animation_frame < 0xF) {
        temp_lo = (s32) ((0xF - shooter_settings_animation_frame) << 0xC) / 15;
        old_bomb_option = (s32) shooter_settings_old_value % 3;
        switch (old_bomb_option) {
        case 0:
            shooter_draw_label((s32) "*", temp_lo, 0x1000, 5, 3, 0x2A, 0x38);
            return;
        case 1:
            shooter_draw_label((s32) "#", temp_lo, 0x1000, 5, 3, 0x2A, 0x38);
            return;
        case 2:
            shooter_draw_label((s32) "@", temp_lo, 0x1000, 5, 3, 0x2A, 0x38);
            return;
        }
    } else {
        temp_lo_2 = (s32) ((shooter_settings_animation_frame << 0xC) + 0xFFFF1000) / 15;
        new_bomb_option = (u8) shooter_setting_bomb_button % 3U;
        switch (new_bomb_option) {
        case 0:
            shooter_draw_label((s32) "*", temp_lo_2, 0x1000, 5, 3, 0x2A, 0x38);
            return;
        case 1:
            shooter_draw_label((s32) "#", temp_lo_2, 0x1000, 5, 3, 0x2A, 0x38);
            return;
        case 2:
            shooter_draw_label((s32) "@", temp_lo_2, 0x1000, 5, 3, 0x2A, 0x38);
            return;
        }
    }
}

void shooter_settings_cycle_value(u8 *setting_value, u8 option_count) {
    u8 v;

    if (shooter_settings_cursor < 4) {
        shooter_settings_changing = 1;
    }
    v = *setting_value;
    shooter_settings_old_value = v;
    if (game_state.pad_pressed & 0x2020) {
        *setting_value = ((u32)v + option_count + 1) % (u32)option_count;
    }
    if (game_state.pad_pressed & PAD_LEFT) {
        *setting_value = (*setting_value + option_count - 1U) % (u32)option_count;
    }
}

void shooter_settings_input(void) {
    if (game_state.pad_pressed & 0x5000) {
        shooter_play_se(0, 0x500);
    }
    if (game_state.pad_pressed & PAD_UP) {
        shooter_settings_cursor -= 1;
    }
    if (game_state.pad_pressed & PAD_DOWN) {
        shooter_settings_cursor += 1;
    }
    if (shooter_settings_cursor < 0) {
        shooter_settings_cursor = 4;
    }
    if (shooter_settings_cursor >= 5) {
        shooter_settings_cursor = 0;
    }
    if (game_state.pad_pressed & 0xA020) {
        if (shooter_settings_cursor != 4) {
            shooter_play_se(0, 0x501);
        }
        switch (shooter_settings_cursor) {
        case 0:
            shooter_settings_cycle_value(&shooter_setting_auto_shot, 2);
            break;
        case 1:
            shooter_settings_cycle_value(&shooter_setting_roll_button, 3);
            break;
        case 2:
            shooter_settings_cycle_value(&shooter_setting_shot_button, 3);
            break;
        case 3:
            shooter_settings_cycle_value(&shooter_setting_bomb_button, 3);
            break;
        case 4:
            if (game_state.pad_pressed & PAD_CIRCLE) {
                shooter_play_se(0, 0x502);
                mode_step_next();
            }
            break;
        }
    }
    if (game_state.pad_pressed & 0x840) {
        shooter_play_se(0, 0x502);
        mode_step_next();
    }
}
