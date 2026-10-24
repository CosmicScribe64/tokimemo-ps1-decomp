/* main_loop.c: src/main/800420D0.c; Original object 0x800420D0-0x80042540: executable entry func_800420D0 and boot_loop perform callback/system init, repeated game startup, frame polling and per-frame dispatch. Original object 0x800420D0-0x80042540 (config/objects/main.txt). */
#include "common.h"
#include "game.h"
#include "libapi.h"

void func_800420D0(void) {
    D_80125124 = SetSp(0x801FEFF0);
    InitHeap(0x801FF800, 0x7F0);
    bzero((void *)0x800E60A0, 0x45498);
    D_80125120 = GetGp();
    boot_loop();
}

void boot_loop(void) {
    GameState *g = &game_state;
    ResetCallback();
    func_80087730();
    printf("TOKIMEKI MEMORIAL : PSX\n");
    func_80042418();
    func_8009C210(0);
    func_8004111C();
loop_1:
    func_8007B844();
    cd_command_reset();
    gpu_init();
    game_state_init(1);
    if (game_state.restart_request != 0) {
        goto loop_1;
    }
    do {
        func_800422C8();
        if (game_state.paused == 0) {
            game_mode_dispatch();
        } else {
            func_80043504();
        }
        game_state.frame_count += 1;
        if (D_800B3220 != 0) {
            func_80059A20();
            if ((game_state.pad_held & 0xC0000) == 0xC0000) {
                menu_vram_view_mode_handler();
            }
            if (game_state.pad_pressed & 0xA0000) {
                func_8004AD60(1);
            }
        }
        if (game_state.restart_request == 0) {
            draw_frame(1);
            cd_command_pump();
            if (game_state.sound_active != 0) {
                audio_command_pump();
            }
        }
    } while (g->restart_request == 0);
    goto loop_1;
}

void func_800422C8(void) {
    draw_buffer = func_80098370();
    if (game_state.game_mode == 0x90) {
        func_800985E0((s32) D_800FD4A0 + draw_buffer * 0x10C00);
        SetWorkBase((s32) D_800E94A0 + draw_buffer * 0xA000, draw_buffer);
    } else {
        func_800985E0((s32) D_800E94A0 + draw_buffer * 0xA000);
        SetWorkBase((s32) D_800FD4A0 + draw_buffer * 0x10C00, draw_buffer);
    }
    func_800985F0(0, 0, (s32) &D_800E8C70 + draw_buffer * 0x14);
    D_8011ECA4 = 0;
}

void func_800423D4(void) {
    game_state.timer_ticks += 1;
    system_data.seed += 0x377;
}

s32 func_80042400(void) {
    system_data.seed += 0x377;
    return system_data.seed;
}

void func_80042418(void) {
    bzero(&system_data, 0xEE0);
    func_80042488();
    card_ev_set();
    func_80042C30();
}

void func_80042458(void) {
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
}

void func_80042488(void) {
    s32 ev;

    EnterCriticalSection();
    ev = OpenEvent(0xF2000003, 2, 0x1000, func_800423D4);
    D_8011ECA8 = ev;
    EnableEvent(ev);
    SetRCnt(0xF2000003, 1, 0x1000);
    StartRCnt(0xF2000003);
    ExitCriticalSection();
}

void func_800424FC(void) {
    EnterCriticalSection();
    CloseEvent(D_8011ECA8);
    ExitCriticalSection();
}
