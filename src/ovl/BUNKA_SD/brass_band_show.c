/* brass_band_show.c: src/ovl/BUNKA_SD/801388D0.c:4; brass-band show callbacks and image/script presentation; club/year dispatch is proved by src/ovl/BUNKA_SD/80137890.c:14-51,63-89. Original object 0x801388D0-0x80139910 (config/objects/BUNKA_SD.txt). */
#include "common.h"
#include "game.h"
#include "ovl/BUNKA_SD.h"

extern s16 D_801207BC;

s32 stage_show_brass_band_audio_transition(void) {
    if (game_state.substep == 0) {
        if (cd_audio_state() == 2) {
            game_state.substep += 1;
        }
    } else if (game_state.substep == 1) {
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        game_state.substep += 1;
    } else if (game_state.substep == 2) {
        if (audio_bank_load_poll() == 1) {
            game_state.substep += 1;
        }
    } else {
        tpage_buf_clear();
        actor_clear_upper();
        hizuke_disp_switch(1);
        mode_stage_set(0x10);
        audio_command(0x200);
    }
}


s32 stage_show_brass_band_sector_and_track_setup(void) {
    if (game_state.substep == 0) {
        cd_read_sectors(0x56, 0x80180000, 0x4F1D);
        game_state.substep += 1;
    } else if (game_state.substep == 1 && (cd_read_sync_status() & 1)) {
        switch (game_state.year) {
        case 0x5F:
            cd_audio_request_set(0x3F, 0x0500039C, 0xF);
            break;
        case 0x60:
            cd_audio_request_set(0x0500005B, 0x05000450, 0xF);
            break;
        default:
        case 0x61:
            cd_audio_request_set(0x02000062, 0x080003F6, 0xF);
            break;
        }
        audio_command(0x300);
        mode_step_next();
    }
}

void stage_show_brass_band_input_update(void) {
    if ((u32)game_state.mode_step >= 3 && (u32)game_state.mode_step < 6 && (game_state.pad_held & PAD_CROSS)) {
        mode_step_set(5);
        audio_command(0xC4);
    }
    stage_show_brass_band_mode_step_dispatch();
}

void stage_show_brass_band_mode_step_dispatch(void) {
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    switch (game_state.mode_step) {
    case 0:
        stage_show_scene_state_reset();
        audio_command(0x7F);
        return;
    case 1:
        stage_show_brass_band_sector_and_track_setup();
        return;
    case 2:
        stage_show_brass_band_cd_audio_end_wait();
        return;
    case 3:
        stage_show_actor_brightness_ramp_up();
        return;
    case 4:
        stage_show_brass_band_show_playback_tick();
        return;
    case 5:
        k_reset(1);
        mode_step_next();
        return;
    case 6:
        stage_show_brass_band_actor_brightness_fade();
        return;
    case 7:
        stage_show_brass_band_audio_transition();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void stage_show_brass_band_cd_audio_end_wait(void) {
    if (cd_audio_state() & 4) {
        if (game_state.mode_stage_timer++ >= 1U) {
            stage_show_brass_band_show_actor_picture_setup();
            mode_step_next();
            cd_audio_command(9, 0, 0);
        }
    } else {
        game_state.mode_stage_timer = 0;
    }
}

void stage_show_brass_band_show_playback_tick(void) {
    GameState *g;

    stage_show_brass_band_audio_frame_and_end_check();
    stage_show_brass_band_animation_frame_select();
    g = &game_state;
    g->mode_step_timer.u += 1;
    if ((g->mode_step_timer.u + 0x90000000U) == 0xFA && g->year != 0x60) {
        mode_step_next();
    }
    if ((g->mode_step_timer.u + 0x90000000U) == 0xD0 && g->year == 0x60) {
        mode_step_next();
    }
}

void stage_show_brass_band_show_actor_picture_setup(void) {
    s32 frame;
    s32 i;
    Actor *record;

    i = 0;
    do {
        load_palette(D_8013D9CC, (i + 0x10) & 0xFF, 1, 1, i);
        i += 1;
    } while (i < 0x10);
    actor_clear(0x60);
    ((Actor *)actor_pool_96)[0].unk_01 = 5;
    ((Actor *)actor_pool_96)[0].quad_flags = 0x01000000;
    ((Actor *)actor_pool_96)[0].draw_flags = 0x80;
    ((Actor *)actor_pool_96)[0].flags = 0x40;
    ((Actor *)actor_pool_96)[0].frame_table = D_8013D9C4;
    ((Actor *)actor_pool_96)[0].sequence_table = D_8013D9C8;
    ((Actor *)actor_pool_96)[0].tpage_table = D_8013D9C0;
    ((Actor *)actor_pool_96)[0].unk_18 = 0x2B;
    ((Actor *)actor_pool_96)[0].brightness = 0;
    ((Actor *)actor_pool_96)[0].x = -0xA0;
    ((Actor *)actor_pool_96)[0].y = -0x78;
    ((Actor *)actor_pool_96)[0].unk_05 = 0x80;
    ((Actor *)actor_pool_96)[0].unk_43 = 0;
    ((Actor *)actor_pool_96)[0].unk_04 = 0;
    actor_clear(0x61);
    ((Actor *)actor_pool_96)[1].unk_01 = 4;
    ((Actor *)actor_pool_96)[1].quad_flags = 0x01000000;
    ((Actor *)actor_pool_96)[1].draw_flags = 0x80;
    ((Actor *)actor_pool_96)[1].flags = 0x40;
    ((Actor *)actor_pool_96)[1].frame_table = D_8013D9C4;
    ((Actor *)actor_pool_96)[1].sequence_table = D_8013D9C8;
    ((Actor *)actor_pool_96)[1].tpage_table = D_8013D9C0;
    ((Actor *)actor_pool_96)[1].unk_18 = 0;
    ((Actor *)actor_pool_96)[1].brightness = 0;
    ((Actor *)actor_pool_96)[1].x = -0xA0;
    ((Actor *)actor_pool_96)[1].y = -0x78;
    ((Actor *)actor_pool_96)[1].unk_05 = 0x80;
    ((Actor *)actor_pool_96)[1].unk_43 = 0;
    ((Actor *)actor_pool_96)[1].unk_04 = 0;
    record = (Actor *)D_8011ED58;
    i = 2;
    frame = 0xF;
    for (; i != 6; i++) {
        actor_clear(i + 0x60);
        record[96].unk_01 = 4;
        record[96].quad_flags = 0x01000000;
        record[96].draw_flags = 0x80;
        record[96].flags = 0x40;
        record[96].frame_table = D_8013D9C4;
        record[96].sequence_table = D_8013D9C8;
        record[96].tpage_table = D_8013D9C0;
        record[96].unk_18 = frame;
        frame += 4;
        record++;
        record[95].blink_period = 1;
        record[95].brightness = 0;
        record[95].x = -0xA0;
        record[95].y = -0x78;
        record[95].unk_05 = 0x80;
        record[95].unk_43 = 0;
        record[95].unk_04 = 0;
    }
    if (D_80122EAC & 1) {
        ((Actor *)actor_pool_96)[3].unk_18 = 0x1F;
    }
    if (D_80122EAC & 2) {
        ((Actor *)actor_pool_96)[4].unk_18 = 0x23;
    }
    if (D_80122EAC & 4) {
        ((Actor *)actor_pool_96)[5].unk_18 = 0x27;
    }
    load_tpage_buf_lock(5, ((s32 *)D_8013D9C0)[0], 1);
    load_tpage_buf_lock(6, ((s32 *)D_8013D9C0)[1], 1);
    load_tpage_buf_lock(7, ((s32 *)D_8013D9C0)[2], 1);
    load_tpage_buf_lock(8, ((s32 *)D_8013D9C0)[3], 1);
    load_tpage_buf_lock(9, ((s32 *)D_8013D9C0)[4], 1);
    load_tpage_buf_lock(0xA, ((s32 *)D_8013D9C0)[5], 1);
    load_tpage_buf_lock(0x25, ((s32 *)D_8013D9C0)[6], 1);
    load_tpage_buf_lock(0x26, ((s32 *)D_8013D9C0)[7], 1);
    load_tpage_buf_lock(0x27, ((s32 *)D_8013D9C0)[8], 1);
    load_tpage_buf_lock(0x28, ((s32 *)D_8013D9C0)[9], 1);
}


s32 stage_show_brass_band_audio_frame_and_end_check(void) {
    if (game_state.mode_step_timer.u == 0xBE) {
        game_state.mode_stage_timer = cd_audio_ticks() - 0x1000;
    }
    if (game_state.mode_step_timer.u == 0xBC) {
        audio_command(0x300);
    }
    debug_print_hex(cd_audio_ticks(), 0, 0);
    debug_print_hex(game_state.mode_stage_timer, 0, 0x10);
    debug_print_hex(cd_audio_ticks() - game_state.mode_stage_timer, 0, 0x20);
    if (game_state.mode_step_timer.u >= 0xC0U) {
        if (game_state.mode_step_timer.u < 0x70000000U) {
            switch (game_state.year) {
            case 0x5F:
                if ((u32)(cd_audio_ticks() - game_state.mode_stage_timer) >= 0x2609U) {
                    game_state.mode_step_timer.u = 0x70000000;
                }
                break;
            case 0x60:
                if ((u32)(cd_audio_ticks() - game_state.mode_stage_timer) >= 0x2AF1U) {
                    game_state.mode_step_timer.u = 0x70000000;
                }
                break;
            case 0x61:
            default:
                if ((u32)(cd_audio_ticks() - game_state.mode_stage_timer) >= 0x27E9U) {
                    game_state.mode_step_timer.u = 0x70000000;
                }
                break;
            }
        }
    }
}

void stage_show_brass_band_animation_frame_select(void) {
    GameState *g = &game_state;
    switch (g->mode_step_timer.u) {
    case 0x1D:
        D_801206AC = 1;
        break;
    case 0x58:
        D_801206AC = 0;
        break;
    case 0x66:
        D_801206AC = 2;
        break;
    case 0x83:
        D_801206AC = 3;
        break;
    }
    if (g->mode_step_timer.u >= 0xBF && g->mode_step_timer.u < 0x70000000) {
        switch (((g->mode_step_timer.u - 0xBE) % 0xF2) / 0xB) {
        case 0: D_801206AC = 4; break;
        case 1: D_801206AC = 3; break;
        case 2: D_801206AC = 5; break;
        case 3: D_801206AC = 6; break;
        case 4: D_801206AC = 3; break;
        case 5: D_801206AC = 0xC; break;
        case 6: D_801206AC = 0xB; break;
        case 7: D_801206AC = 0xE; break;
        case 8: D_801206AC = 3; break;
        case 9: D_801206AC = 8; break;
        case 10: D_801206AC = 7; break;
        case 11: D_801206AC = 9; break;
        case 12: D_801206AC = 3; break;
        case 13: D_801206AC = 5; break;
        case 14: D_801206AC = 6; break;
        case 15: D_801206AC = 3; break;
        case 16: D_801206AC = 0xD; break;
        case 17: D_801206AC = 0xB; break;
        case 18: D_801206AC = 4; break;
        case 19: D_801206AC = 3; break;
        case 20: D_801206AC = 0xA; break;
        case 21: D_801206AC = 7; break;
        }
    }
    if (g->mode_step_timer.u == 0x70000000) {
        D_801206AC = 3;
    }
    switch (g->mode_step_timer.u - 0x70000000) {
    case 0x3B:
        /* FAKE: empty field test emits the speculative load before the
         * store while keeping its original global range. T-25020 */
        if (g->year) {}
        D_801206AC = 2;
        break;
    case 0x49:
        /* FAKE: stage the constant before the year preload; the unsigned
         * false test folds after uopt has placed that preload. T-25037 */
        if (g->mode_step_timer.u == 0x60) {}
        if ((u32)g->year < 0) {}
        D_801206AC = 0;
        break;
    case 0x66:
        /* FAKE: split constant/store emission from the year preload without
         * recomputing the selector; both empty tests disappear. T-25037 */
        if (g->mode_step_timer.u == 1) {}
        D_801206AC = 1;
        if ((u32)g->year < 0) {}
        break;
    case 0xA1:
        if (g->year == 0x60) {
            D_801206AC = 0;
        }
        break;
    }
    if (g->mode_step_timer.u == 0x95) {
        D_801206F0 = 0x10;
        D_80120734 = 0x14;
        D_80120778 = 0x18;
        D_801207BC = 0x1D;
        if (D_80122EAC & 1) {
            D_80120734 = 0x20;
        }
        if (D_80122EAC & 2) {
            D_80120778 = 0x24;
        }
        if (D_80122EAC & 4) {
            D_801207BC = 0x28;
        }
    }
    if (g->mode_step_timer.u >= 0xD1 && g->mode_step_timer.u < 0x70000000) {
        switch (((g->mode_step_timer.u - 0xC9) % 0x4E) / 0x27) {
        case 0:
            D_801206F0 = 0x11;
            D_80120734 = 0x15;
            D_80120778 = 0x19;
            D_801207BC = 0x1D;
            if (D_80122EAC & 1) {
                D_80120734 = 0x21;
            }
            if (D_80122EAC & 2) {
                D_80120778 = 0x25;
            }
            if (D_80122EAC & 4) {
                D_801207BC = 0x29;
            }
            break;
        case 1:
            D_801206F0 = 0x10;
            D_80120734 = 0x14;
            D_80120778 = 0x18;
            D_801207BC = 0x1C;
            if (D_80122EAC & 1) {
                D_80120734 = 0x20;
            }
            if (D_80122EAC & 2) {
                D_80120778 = 0x24;
            }
            if (D_80122EAC & 4) {
                D_801207BC = 0x28;
            }
            break;
        }
    }
    if (g->mode_step_timer.u == 0x70000000) {
        D_801206F0 = 0x10;
        D_80120734 = 0x14;
        D_80120778 = 0x18;
        D_801207BC = 0x1C;
        if (D_80122EAC & 1) {
            D_80120734 = 0x20;
        }
        if (D_80122EAC & 2) {
            D_80120778 = 0x24;
        }
        if (D_80122EAC & 4) {
            D_801207BC = 0x28;
        }
    }
    if (g->mode_step_timer.u - 0x70000000 == 0x3B && g->year != 0x60) {
        D_801206F0 = 0xF;
        D_80120734 = 0x13;
        D_80120778 = 0x17;
        D_801207BC = 0x1B;
        if (D_80122EAC & 1) {
            D_80120734 = 0x1F;
        }
        if (D_80122EAC & 2) {
            D_80120778 = 0x23;
        }
        if (D_80122EAC & 4) {
            D_801207BC = 0x27;
        }
    }
    if (g->mode_step_timer.u - 0x70000000 == 0x68 && g->year == 0x60) {
        D_801206F0 = 0x12;
        D_80120734 = 0x16;
        D_80120778 = 0x1A;
        D_801207BC = 0x1E;
        if (D_80122EAC & 1) {
            D_80120734 = 0x22;
        }
        if (D_80122EAC & 2) {
            D_80120778 = 0x26;
        }
        if (D_80122EAC & 4) {
            D_801207BC = 0x2A;
        }
    }
}

/* FAKE: T-22021. The byte game_state.game_mode_scratch is read and written only as a u8 view, so uopt promotes it to one register for the whole function (a local copy splits the range and adds a `move`). `*= 0` is the clear that survives constant folding: it keeps the `move v1,zero` and the loop stores of that register (a plain `= 0` folds to `sb zero`); the original source is unknown. */
void stage_show_brass_band_actor_brightness_fade(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        ((Actor *)actor_pool_96)[i].brightness = *(u8 *)&game_state.game_mode_scratch;
    }
    if (*(u8 *)&game_state.game_mode_scratch == 0) {
        *(u8 *)&game_state.game_mode_scratch = 2;
    }
    *(u8 *)&game_state.game_mode_scratch -= 1;
    if (*(u8 *)&game_state.game_mode_scratch == 0) {
        *(u8 *)&game_state.game_mode_scratch *= 0;
        for (i = 0; i < 0x40; i++) {
            ((Actor *)actor_pool_96)[i].brightness = *(u8 *)&game_state.game_mode_scratch;
        }
        mode_step_next();
    }
}
