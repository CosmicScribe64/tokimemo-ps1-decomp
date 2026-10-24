/* pad_input.c: src/main/80042A00.c; Original object 0x80042A00-0x80043510: detects and initializes pad/mouse, polls controller state, maps buttons, handles turbo/pad code, pause and soft-reset sequences. Original object 0x80042A00-0x80043510 (config/objects/main.txt). */
#include "common.h"
#include "game.h"

void func_80042A00(void) {
    if (func_80042AC8() == 0) {
        if (game_state.input_device == INPUT_MOUSE) {
            func_80042CB0();
        } else if (game_state.input_device != INPUT_NONE) {
            func_800430C0();
        }
        if (D_800B3CA0 != 0 && (game_state.pad_pressed & 0xF9DF)) {
            D_800B3CA0 = 0;
        }
        if ((game_state.pad_held & 0xF00000) == 0xF00000) {
            D_800B3CA0 = 1;
        }
        if (game_state.pad_pressed & 0x08000000) {
            game_state.sd_anim_fast = 1 - (u8)game_state.sd_anim_fast;
        }
    }
}

s32 func_80042AC8(void) {
    GameState *g = &game_state;
    u8 old;

    old = g->input_device;
    if (D_800E8BF0[0] == 0) {
        if ((D_800E8BF0[1] & 0xF) * 2 <= 0x40) {
            if ((D_800E8BF0[1] & 0xF0) == 0x10) {
                g->input_device = INPUT_MOUSE;
                g->menu_mode = MENU_MODE_MOUSE_LIKE;
            } else {
                g->input_device = INPUT_PAD;
            }
        }
    } else if (D_800E8BF0[0] != 0xFF || g->memory_card_busy == 0) {
        g->input_device = INPUT_NONE;
    }
    if (old != g->input_device && (g->input_device != INPUT_NONE || old != 0xFF)) {
        if (g->input_device == INPUT_MOUSE) {
            func_80042C30();
            func_80042C74();
            func_80042BD4();
            return 1;
        }
        if (g->input_device == INPUT_PAD) {
            func_80042C74();
            func_80042C30();
        }
        return 1;
    }
    return 0;
}

void func_80042BD4(void) {
    InitMouse(D_800E8BF0, D_800E8C30);
    SenseMouse(3, 4);
    SetMouse(0, 0xA0, 0x78);
    RangeMouse(0, 0x140, 0, 0xF0);
}

void func_80042C30(void) {
    InitPAD(D_800E8BF0, 8, D_800E8C30, 8);
    StartPAD();
    ChangeClearPAD(0);
}

void func_80042C74(void) {
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
void func_80042CB0(void) {
    GameState *g = &game_state;
    s32 dx;
    s32 dy;
    s16 qx;
    s16 qy;
    s32 b2;
    s32 b3;

    dx = *(s8 *)&D_800E8BF0[4];
    dy = *(s8 *)&D_800E8BF0[5];
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
    g->unk_F76 = ~D_800E8BF0[3];
    g->pad_prev = g->pad_held;
    g->mouse_buttons = g->unk_F76 & ~g->unk_F78;
    if (*(u8 *)&g->click_mode != 0 && (*(u8 *)&g->unk_F71 == 0 || g->unk_F72 != g->mouse_buttons) && g->mouse_buttons != 0) {
        if (g->mouse_buttons & MOUSE_RIGHT) {
            *(u8 *)&g->unk_F71 = *(u8 *)&g->double_click_interval + 8;
            g->unk_F72 = g->mouse_buttons;
        } else if (g->mouse_buttons & MOUSE_LEFT) {
            *(u8 *)&g->unk_F71 = *(u8 *)&g->double_click_interval + 8;
            g->unk_F72 = g->mouse_buttons;
        }
    }
    g->pad_held = 0;
    if (*(u8 *)&g->click_mode == 0 || (*(u8 *)&g->click_mode != 0 && *(u8 *)&g->unk_F71 != 0 && *(u8 *)&g->double_click_interval + 8 != *(u8 *)&g->unk_F71 && g->unk_F72 == g->mouse_buttons)) {
        g->unk_F72 = 0;
        *(u8 *)&g->unk_F71 = 0;
        if (g->confirm_button != 0) {
            if (g->unk_F76 & 4) {
                g->pad_held |= PAD_CIRCLE;
            } else if (g->unk_F76 & 8) {
                g->pad_held |= PAD_CROSS;
            }
        } else if (g->unk_F76 & 8) {
            g->pad_held |= PAD_CIRCLE;
        } else if (g->unk_F76 & 4) {
            g->pad_held |= PAD_CROSS;
        }
    }
    if (*(u8 *)&g->unk_F71 != 0) {
        *(u8 *)&g->unk_F71 -= 1;
    } else {
        g->unk_F72 = 0;
    }
    if (D_800E8C30[0] == 0 && (D_800E8C30[1] & 0xF0) == 0x40) {
        b2 = ~D_800E8C30[3];
        b3 = D_800E8C30[2];
        g->pad_held |= ~b3 << 24;
        g->pad_held |= (b2 & 0xFF) << 16;
    }
    g->pad_pressed = ~g->pad_prev & g->pad_held;
    g->pad_released = g->pad_prev & (u32)~g->pad_held;
    g->unk_F78 = g->unk_F76;
}

/* Implicit int (no return value on the unk_F74 == 0xFF path in the original). */
s32 func_80043010(void) {
    s32 b0;
    s32 b1;
    s32 b2;
    s32 b3;
    s32 a0;

    if (game_state.input_device != INPUT_NONE) {
        if (game_state.input_device != INPUT_MOUSE) {
            b0 = ~D_800E8BF0[3];
            b1 = ~D_800E8BF0[2];
            b2 = ~D_800E8C30[3];
            b3 = D_800E8C30[2];
            return (~b3 << 24) + ((b2 & 0xFF) << 16) + ((b1 & 0xFF) << 8) + (b0 & 0xFF);
        }
        a0 = 0;
        b0 = ~D_800E8BF0[3] & 0xFF;
        if (b0 & 8) {
            a0 = 0x20;
        }
        if (b0 & 4) {
            a0 |= 0x40;
        }
        return a0;
    }
}

s32 func_800430C0(void) {
    s32 raw;

    game_state.pad_prev = game_state.pad_held;
    raw = func_80043010();
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
    if (D_800B3CA0 != 0 && (game_state.frame_count & 1)) {
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
            if (D_800B3CA4++ >= 0x81) {
                D_800B3CA4 = 0;
                func_800789E0();
                func_800591D8(0);
                game_state_init(0);
                game_mode_set(GAME_MODE_BOOT);
                sprite_tables_clear();
                func_80048DD0(0);
                back_clear_switch(1);
                dec_bg_reset();
                audio_bank_state_reset();
                tpage_buf_clear();
                audio_command(0x7F);
                func_800424FC();
                func_8007BDE8();
                func_8007B844();
                func_80042488();
                cd_command_reset();
                func_80041878();
                func_80042058();
                func_80041F48();
                if (game_state.memory_card_busy != 0) {
                    memory_card_stop();
                }
                cursor_actor_init(0);
                return -1;
            }
        } else if ((game_state.pad_held & 0x090C0000) == 0x090C0000) {
            if (D_800B3CA4++ >= 0x81) {
                D_800B3CA4 = 0;
                func_800789E0();
                func_800591D8(0);
                game_state_init(0);
                game_mode_set(GAME_MODE_OPENING);
                sprite_tables_clear();
                func_80048DD0(0);
                back_clear_switch(1);
                dec_bg_reset();
                audio_bank_state_reset();
                tpage_buf_clear();
                audio_command(0x7F);
                func_800424FC();
                func_8007BDE8();
                func_8007B844();
                func_80042488();
                cd_command_reset();
                func_80041878();
                func_80042058();
                func_80041F48();
                if (game_state.memory_card_busy != 0) {
                    memory_card_stop();
                }
                cursor_actor_init(0);
                return -1;
            }
        } else {
            D_800B3CA4 = 0;
        }
    }
    /* no return value on this path, as in the original (v0 is whatever func_80043448 leaves) */
    func_80043448();
}

void func_80043448(void) {
    if (D_800B3CFC != 0) {
        if (++D_800B3D00 == 0) {
            D_800B3CFC = 0;
            D_800B3CF8 = 0;
        }
    } else {
        if (game_state.pad_held == D_800B3CA8[D_800B3CF8]) {
            if (++D_800B3CF8 >= 0x12U) {
                D_800B3CFC = 1;
            }
            D_800B3D00 = 0x10;
        }
        if (--D_800B3D00 == 0) {
            D_800B3CF8 = 0;
        }
    }
}

void func_80043504(void) {}
