/* save_and_exit.c: src/ovl/TACO/80142820.c:7: stage 46; body 8: switch (game_state.mode_step) {; 10: func_801428DC();. Original object 0x80142820-0x801430B0 (config/objects/TACO.txt). */
#define MAIN_API_OVERRIDE_strncpy /* matched as int: the call's $v0 result range (main_api.h: void), T-19009 */
#include "common.h"
#include "ovl/TACO.h"

s32 strncpy();

void func_80142820(void) {
    switch (game_state.mode_step) {
    case 0:
        func_801428DC();
        break;
    case 1:
        func_80142FE8();
        break;
    case 2:
        func_80142AF0();
        break;
    case 3:
        func_80142EC4();
        break;
    case 4:
        func_80142EE4();
        break;
    case 5:
        func_80142F0C();
        break;
    case 6:
        func_80142F34();
        break;
    case 7:
        func_80142DE0();
        break;
    }
}

/* Settings block at system_data +0x40 (SystemData in main_api.h since T-20900; the loader func_801345D4 in
 * 80133870.c has its own view of it, T-16562): byte 0 holds the "saved" flag, the mode and the selector, byte 1 two
 * more selectors. Every access relocates against the one symbol system_data: as1 then hoists the flag byte load over
 * the word stores, as the original does (separate symbols D_800E7D50/D_800E7D5C keep it after them). T-19009 */

void func_801428DC(void) {
    system_data.bits.flag = 1;
    system_data.w0 = D_8015EDF8;
    system_data.w1 = D_8015EDFC;
    system_data.w2 = D_8015EE00;
    strncpy(system_data.name0, D_8015EE04, 3);
    strncpy(system_data.name1, D_8015EE08, 3);
    strncpy(system_data.name2, D_8015EE0C, 3);
    /* the % 2 folds away, but keeps the extra copy of the value that the original has */
    system_data.bits.mode = D_8015EDD8 % 2;
    system_data.bits.sel = D_8015EDDC % 3;
    system_data.bits.hi = D_8015EDE0 % 3;
    system_data.bits.lo = D_8015EDE4 % 3;
    sprite_tables_clear();
    draw2d3d(0, 0);
    game_state.screen_center_x[0] = 0xA0;
    game_state.screen_center_x[1] = 0xA0;
    func_8009C674(0);
    func_8009C210(3);
    func_80097D90(0x140, 0xF0, 0, 0, 0);
    if (func_80045288() == 0) {
        cd_command_reset();
    }
    func_8005907C();
    func_80059048();
    func_801347A8();
    actor_clear_range(0);
    actor_draw_enable(0);
    mode_step_next();
}

void func_80142AF0(void) {
    GameState *gs = &game_state;
    s32 temp;

    switch (gs->substep) {
    case 0:
        if (func_80053F04() != 0) {
            /* FAKE: the first read of unk_1115 goes through a local so the three tests share a
             * register range that takes $v1 (found by tools/autotweak.py); T-19009 */
            temp = gs->memory_card_result;
            if (temp == 1) {
                gs->substep += 1;
            } else if (gs->memory_card_result == 0xF1) {
                gs->substep += 1;
            } else if (gs->memory_card_result == 0xFF) {
                gs->substep += 1;
            }
        } else if ((u32)gs->memory_card_timer >= 0x201U) {
            gs->substep += 1;
        }
        break;
    case 1:
        memory_card_stop();
        if (gs->memory_card_result == 1) {
            if (func_80054AF4(0) == 1) {
                gs->substep += 1;
                memory_card_start();
                func_8005C4CC(0);
            } else {
                gs->substep = 0xFF;
            }
        } else {
            gs->substep = 0xFF;
        }
        break;
    case 2:
        if (func_80054284() == 3) {
            memory_card_stop();
            gs->substep += 1;
        } else if (((u32)gs->memory_card_timer >= 0x201U) || (gs->memory_card_result == 0xF0)) {
            memory_card_stop();
            gs->substep = 0xFF;
        }
        break;
    case 3:
        if (func_800552DC(0) == -1) {
            func_8005C4CC(1);
            gs->substep = 0xFF;
        } else {
            func_8005C4CC(1);
            gs->substep += 1;
        }
        break;
    case 4:
        memory_card_start();
        D_800E7D36 = 0;
        gs->substep += 1;
        break;
    case 5:
        if (func_80054388() == 0x13) {
            audio_command(0x50F);
            memory_card_stop();
            gs->substep += 1;
        } else if (((u32)gs->memory_card_timer >= 0x201U) || (gs->memory_card_result == 0xF0)) {
            audio_command(0x502);
            memory_card_stop();
            gs->substep += 1;
        }
        break;
    case 0xFF:
    default:
        mode_step_next();
        break;
    }
}

typedef struct {
    u32 pad0 : 1;
    u32 flag : 1;
    u32 pad1 : 30;
} TcBits;

void func_80142DE0(void) {
    text_system_reset();
    dec_bg_reset();
    draw2d3d(1, 0);
    actor_draw_enable(1);
    *(s32 *) &game_state.menu_hit[3] = 0x9F;
    if (((TcBits *) &D_800E7D50)->flag == 1) {
        actor_clear_range(0);
        tpage_buf_clear();
        game_mode_set(game_state.saved_game_mode);
        mode_stage_set(game_state.saved_mode_stage);
        mode_step_set(game_state.saved_mode_step);
        cursor_actor_init(0);
        text_marker_set(0);
        message_window_init();
        hizuke_init();
        message_disp_switch(1);
        hizuke_disp_switch(1);
        return;
    }
    game_mode_set(GAME_MODE_TITLE);
}

void func_80142EC4(void) {
    func_80059E00();
}

void func_80142EE4(void) {
    func_80059BE8();
    mode_step_next();
}

void func_80142F0C(void) {
    func_8004ADE4();
    mode_step_next();
}

u8 func_80142F34(void) {
    switch (game_state.substep) {
    case 0:
        cd_read_sectors(0x95, 0x801A0000, 0x9B94);
        game_state.substep += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_80068938(game_state.year, game_state.month, 0);
            game_state.substep += 1;
        }
        break;
    case 2:
        mode_step_next();
        break;
    }
}

u8 func_80142FE8(void) {
    switch (game_state.substep) {
    case 0:
        audio_bank_state_reset();
        func_800674B0();
        game_state.substep += 1;
        break;
    case 1:
        if (audio_bank_load_poll() == 1) {
            game_state.substep += 1;
        }
        break;
    case 2:
        if (D_800B3220 == 0) {
            memory_card_start();
        } else {
            mode_step_next();
        }
        mode_step_next();
        break;
    }
}
