/* pad_input.c: src/main/80042A00.c; Original object 0x80042A00-0x80043510: detects and initializes pad/mouse, polls controller state, maps buttons, handles turbo/pad code, pause and soft-reset sequences. Original object 0x80042A00-0x80043510 (config/objects/main.txt). */
#include "common.h"
#include "common.h"
#include "game/game_state.h"
#include "game_enums.h"
#include "libapi.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/boot.h"
#include "main/cd.h"
#include "main/cursor.h"
#include "main/draw.h"
#include "main/game_mode.h"
#include "main/hud.h"
#include "main/import_views/scenes.h"
#include "main/memory_card.h"
#include "main/mouse.h"
#include "main/ovl.h"
#include "main/pad.h"
#include "main/settings.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/timer.h"
#include "main/tpage.h"
#include "main/view.h"
#include "main/game_mode.h"
#include "main/pad.h"
#include "main/ovl.h"

void pad_update(void) {
    if (pad_device_detect() == 0) {
        if (game_state.input_device == INPUT_MOUSE) {
            mouse_to_pad();
        } else if (game_state.input_device != INPUT_NONE) {
            pad_poll();
        }
        if (pad_turbo != 0 && (game_state.pad_pressed & 0xF9DF)) {
            pad_turbo = 0;
        }
        if ((game_state.pad_held & 0xF00000) == 0xF00000) {
            pad_turbo = 1;
        }
        if (game_state.pad_pressed & 0x08000000) {
            game_state.sd_anim_fast = 1 - (u8)game_state.sd_anim_fast;
        }
    }
}

s32 pad_device_detect(void) {
    GameState *g = &game_state;
    u8 old;

    old = g->input_device;
    if (pad_buffer_1[0] == 0) {
        if ((pad_buffer_1[1] & 0xF) * 2 <= 0x40) {
            if ((pad_buffer_1[1] & 0xF0) == 0x10) {
                g->input_device = INPUT_MOUSE;
                g->menu_mode = MENU_MODE_MOUSE_LIKE;
            } else {
                g->input_device = INPUT_PAD;
            }
        }
    } else if (pad_buffer_1[0] != 0xFF || g->memory_card_busy == 0) {
        g->input_device = INPUT_NONE;
    }
    if (old != g->input_device && (g->input_device != INPUT_NONE || old != 0xFF)) {
        if (g->input_device == INPUT_MOUSE) {
            pad_driver_init();
            pad_state_clear();
            mouse_driver_init();
            return 1;
        }
        if (g->input_device == INPUT_PAD) {
            pad_state_clear();
            pad_driver_init();
        }
        return 1;
    }
    return 0;
}

void mouse_driver_init(void) {
    InitMouse(pad_buffer_1, pad_buffer_2);
    SenseMouse(3, 4);
    SetMouse(0, 0xA0, 0x78);
    RangeMouse(0, 0x140, 0, 0xF0);
}

void pad_driver_init(void) {
    InitPAD(pad_buffer_1, 8, pad_buffer_2, 8);
    StartPAD();
    ChangeClearPAD(0);
}

void pad_state_clear(void) {
    cursor_x = 0;
    cursor_y = 0;
    game_state.mouse_x = 0;
    game_state.mouse_y = 0;
    game_state.pad_held = 0;
    game_state.pad_prev = 0;
    game_state.pad_pressed = 0;
}

/* Mouse/pad polling and the double-click state machine. Fields go through `g`; the F6F reads use the
 * global (it keeps the pad bytes loaded first, T-24301). */
void mouse_to_pad(void) {
    GameState *g = &game_state;
    s32 dx;
    s32 dy;
    s16 qx;
    s16 qy;
    s32 b2;
    s32 b3;

    dx = *(s8 *)&pad_buffer_1[4];
    dy = *(s8 *)&pad_buffer_1[5];
    qx = (s32)(dx * game_state.cursor_speed) / 3;
    qy = (s32)(dy * game_state.cursor_speed) / 3;
    /* FAKE: T-24301, dead stores; without them uopt colours the clamps differently (134 rows) */
    dx = 0;
    dy = 0;
    g->mouse_x += qx;
    g->mouse_y += qy;
    if (g->mouse_x >= 0x8D) {
        g->mouse_x = 0x8C;
    }
    if (g->mouse_x < -0x96) {
        g->mouse_x = -0x96;
    }
    if (g->mouse_y < -0x64) {
        g->mouse_y = -0x64;
    }
    if (g->mouse_y >= 0x65) {
        g->mouse_y = 0x64;
    }
    cursor_x = g->mouse_x;
    cursor_y = g->mouse_y;
    g->mouse_held = ~pad_buffer_1[3];
    g->pad_prev = g->pad_held;
    g->mouse_buttons = g->mouse_held & ~g->mouse_held_previous;
    if (*(u8 *)&g->click_mode != 0 && (*(u8 *)&g->click_timer == 0 || g->click_button != g->mouse_buttons) && g->mouse_buttons != 0) {
        if (g->mouse_buttons & MOUSE_RIGHT) {
            *(u8 *)&g->click_timer = *(u8 *)&g->double_click_interval + 8;
            g->click_button = g->mouse_buttons;
        } else if (g->mouse_buttons & MOUSE_LEFT) {
            *(u8 *)&g->click_timer = *(u8 *)&g->double_click_interval + 8;
            g->click_button = g->mouse_buttons;
        }
    }
    g->pad_held = 0;
    if (*(u8 *)&g->click_mode == 0 || (*(u8 *)&g->click_mode != 0 && *(u8 *)&g->click_timer != 0 && *(u8 *)&g->double_click_interval + 8 != *(u8 *)&g->click_timer && g->click_button == g->mouse_buttons)) {
        g->click_button = 0;
        *(u8 *)&g->click_timer = 0;
        if (g->confirm_button != SETTINGS_CONFIRM_BUTTON_MOUSE_LEFT) {
            if (g->mouse_held & 4) {
                g->pad_held |= PAD_CIRCLE;
            } else if (g->mouse_held & 8) {
                g->pad_held |= PAD_CROSS;
            }
        } else if (g->mouse_held & 8) {
            g->pad_held |= PAD_CIRCLE;
        } else if (g->mouse_held & 4) {
            g->pad_held |= PAD_CROSS;
        }
    }
    if (*(u8 *)&g->click_timer != 0) {
        *(u8 *)&g->click_timer -= 1;
    } else {
        g->click_button = 0;
    }
    if (pad_buffer_2[0] == 0 && (pad_buffer_2[1] & 0xF0) == 0x40) {
        b2 = ~pad_buffer_2[3];
        b3 = pad_buffer_2[2];
        g->pad_held |= ~b3 << 24;
        g->pad_held |= (b2 & 0xFF) << 16;
    }
    g->pad_pressed = ~g->pad_prev & g->pad_held;
    g->pad_released = g->pad_prev & (u32)~g->pad_held;
    g->mouse_held_previous = g->mouse_held;
}

/* Implicit int (no return value on the unk_F74 == 0xFF path in the original). */
s32 pad_raw_read(void) {
    s32 b0;
    s32 b1;
    s32 b2;
    s32 b3;
    s32 a0;

    if (game_state.input_device != INPUT_NONE) {
        if (game_state.input_device != INPUT_MOUSE) {
            b0 = ~pad_buffer_1[3];
            b1 = ~pad_buffer_1[2];
            b2 = ~pad_buffer_2[3];
            b3 = pad_buffer_2[2];
            return (~b3 << 24) + ((b2 & 0xFF) << 16) + ((b1 & 0xFF) << 8) + (b0 & 0xFF);
        }
        a0 = 0;
        b0 = ~pad_buffer_1[3] & 0xFF;
        if (b0 & 8) {
            a0 = 0x20;
        }
        if (b0 & 4) {
            a0 |= 0x40;
        }
        return a0;
    }
}

s32 pad_poll(void) {
    s32 raw;

    game_state.pad_prev = game_state.pad_held;
    raw = pad_raw_read();
    if ((raw & 0xFFFF0000) == 0xFFFF0000) {
        game_state.pad_held = raw & 0xFFFF;
        if (game_state.pad_held == 0xFFFF) {
            game_state.pad_held = 0;
        }
    } else {
        game_state.pad_held = raw;
        if ((raw & 0xFFFF) == 0xFFFF) {
            if ((raw & 0xFFFF0000) == 0xFFFF0000) {
                game_state.pad_held = 0;
            } else {
                game_state.pad_held = raw & 0xFFFF0000;
            }
        }
    }
    game_state.pad_pressed = ~game_state.pad_prev & game_state.pad_held;
    /* FAKE: the (u32) cast keeps ugen's operand order (prev, ~pad) of the original; real source unknown. T-17009 */
    game_state.pad_released = game_state.pad_prev & (u32)~game_state.pad_held;
    if (pad_turbo != 0 && (game_state.frame_count & 1)) {
        game_state.pad_pressed |= PAD_CIRCLE;
    }
    if (game_state.pause_allowed == 1) {
        if (game_state.pad_pressed & 0x08000800) {
            if (game_state.paused == 0) {
                game_state.paused = 1;
                game_state.actor_draw_enabled = 0;
            } else {
                game_state.actor_draw_enabled = 1;
                game_state.paused = 0;
            }
        }
    }
    if (game_state.game_mode != 0xD0 && game_state.game_mode != 0x20 && game_state.game_mode != 0x90 && game_state.input_device == INPUT_PAD) {
        if ((game_state.pad_held & 0x90C) == 0x90C) {
            if (pad_reset_frames++ >= 0x81) {
                pad_reset_frames = 0;
                ovl_load_reset();
                view_display_set_wide_or_standard(0);
                game_state_init(0);
                game_mode_set(GAME_MODE_BOOT);
                sprite_tables_clear();
                actor_deactivate(0);
                back_clear_switch(1);
                dec_bg_reset();
                audio_bank_state_reset();
                tpage_buf_clear();
                audio_command(0x7F);
                timer_event_stop();
                audio_shutdown();
                audio_init();
                timer_event_start();
                cd_command_reset();
                boot_game_data_init();
                boot_settings_defaults_set();
                boot_event_state_init();
                if (game_state.memory_card_busy != 0) {
                    memory_card_stop();
                }
                hud_cursor_actor_init(0);
                return -1;
            }
        } else if ((game_state.pad_held & 0x090C0000) == 0x090C0000) {
            if (pad_reset_frames++ >= 0x81) {
                pad_reset_frames = 0;
                ovl_load_reset();
                view_display_set_wide_or_standard(0);
                game_state_init(0);
                game_mode_set(GAME_MODE_OPENING);
                sprite_tables_clear();
                actor_deactivate(0);
                back_clear_switch(1);
                dec_bg_reset();
                audio_bank_state_reset();
                tpage_buf_clear();
                audio_command(0x7F);
                timer_event_stop();
                audio_shutdown();
                audio_init();
                timer_event_start();
                cd_command_reset();
                boot_game_data_init();
                boot_settings_defaults_set();
                boot_event_state_init();
                if (game_state.memory_card_busy != 0) {
                    memory_card_stop();
                }
                hud_cursor_actor_init(0);
                return -1;
            }
        } else {
            pad_reset_frames = 0;
        }
    }
    /* no return value on this path, as in the original (v0 is whatever pad_code_update leaves) */
    pad_code_update();
}

void pad_code_update(void) {
    if (pad_code_entered != 0) {
        if (++pad_code_timer == 0) {
            pad_code_entered = 0;
            pad_code_index = 0;
        }
    } else {
        if (game_state.pad_held == pad_code_table[pad_code_index]) {
            if (++pad_code_index >= 0x12U) {
                pad_code_entered = 1;
            }
            pad_code_timer = 0x10;
        }
        if (--pad_code_timer == 0) {
            pad_code_index = 0;
        }
    }
}

void boot_paused_step(void) {}
