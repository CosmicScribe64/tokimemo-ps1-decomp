/* main_loop.c: src/main/800420D0.c; Original object 0x800420D0-0x80042540: executable entry boot_start and boot_loop perform callback/system init, repeated game startup, frame polling and per-frame dispatch. Original object 0x800420D0-0x80042540 (config/objects/main.txt). */
#include "common.h"
#include "common.h"
#include "game/game_state.h"
#include "game_enums.h"
#include "libapi.h"
#include "libgpu.h"
#include "libspu.h"
#include "main/audio.h"
#include "main/boot.h"
#include "main/cd.h"
#include "main/draw.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/resident_views.h"
#include "main/import_views/scenes.h"
#include "main/memory_card.h"
#include "main/pad.h"
#include "main/prim.h"
#include "main/state.h"
#include "main/sys.h"
#include "main/timer.h"
#include "main/vram.h"
#include "libapi.h"
#include "main/pad.h"

void boot_start(void) {
    boot_previous_stack_pointer = SetSp(0x801FEFF0);
    InitHeap(0x801FF800, 0x7F0);
    bzero((void *)0x800E60A0, 0x45498);
    boot_global_pointer = GetGp();
    boot_loop();
}

void boot_loop(void) {
    GameState *g = &game_state;
    ResetCallback();
    func_80087730();
    printf("TOKIMEKI MEMORIAL : PSX\n");
    sys_init();
    func_8009C210(0);
    vram_clear_framebuffers();
loop_1:
    audio_init();
    cd_command_reset();
    gpu_init();
    game_state_init(1);
    if (game_state.restart_request != 0) {
        goto loop_1;
    }
    do {
        boot_frame_begin();
        if (game_state.paused == 0) {
            game_mode_dispatch();
        } else {
            boot_paused_step();
        }
        game_state.frame_count += 1;
        if (debug_hud_enabled != 0) {
            debug_state_numbers_draw();
            if ((game_state.pad_held & 0xC0000) == 0xC0000) {
                menu_vram_view_mode_handler();
            }
            if (game_state.pad_pressed & 0xA0000) {
                debug_hud_level_step(1);
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

void boot_frame_begin(void) {
    draw_buffer = func_80098370();
    if (game_state.game_mode == 0x90) {
        func_800985E0((s32) prim_arena_buffers + draw_buffer * 0x10C00);
        SetWorkBase((s32) draw_work_buffers + draw_buffer * 0xA000, draw_buffer);
    } else {
        func_800985E0((s32) draw_work_buffers + draw_buffer * 0xA000);
        SetWorkBase((s32) prim_arena_buffers + draw_buffer * 0x10C00, draw_buffer);
    }
    func_800985F0(0, 0, (s32) &draw_ot_table + draw_buffer * 0x14);
    D_8011ECA4 = 0;
}

void timer_tick(void) {
    game_state.timer_ticks += 1;
    system_data.seed += 0x377;
}

s32 timer_seed_step(void) {
    system_data.seed += 0x377;
    return system_data.seed;
}

void sys_init(void) {
    bzero(&system_data, 0xEE0);
    timer_event_start();
    card_ev_set();
    pad_driver_init();
}

void sys_cache_flush(void) {
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
}

void timer_event_start(void) {
    s32 ev;

    EnterCriticalSection();
    ev = OpenEvent(0xF2000003, 2, 0x1000, timer_tick);
    timer_event = ev;
    EnableEvent(ev);
    SetRCnt(0xF2000003, 1, 0x1000);
    StartRCnt(0xF2000003);
    ExitCriticalSection();
}

void timer_event_stop(void) {
    EnterCriticalSection();
    CloseEvent(timer_event);
    ExitCriticalSection();
}
