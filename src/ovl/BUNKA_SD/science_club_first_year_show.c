/* science_club_first_year_show.c: src/ovl/BUNKA_SD/80136270.c:4; show callbacks and image resources; club/year dispatch is proved by src/ovl/BUNKA_SD/club_year_show_dispatch.c:14-51,63-89. Original object 0x80136270-0x801369D0 (config/objects/BUNKA_SD.txt). */
#include "common.h"
#include "ovl/BUNKA_SD.h"

s32 stage_show_science_first_show_sector_load(void) {
    if (game_state.substep == 0) {
        cd_read_sectors(0x6F, 0x80180000, 0x4C59);
        game_state.substep += 1;
    } else if ((game_state.substep == 1) && (cd_read_sync_status() & 1)) {
        mode_step_next();
    }
}

void stage_show_science_first_show_input_update(void) {
    s32 i;

    if (game_state.mode_step >= 3 && game_state.mode_step < 7) {
        if (game_state.pad_held & PAD_CIRCLE) {
            stage_show_science_first_show_mode_step_dispatch();
            for (i = 0; i < 3; i++) {
                ((Actor *)actor_pool)[i + 96].blink_period = 0x82;
            }
        } else {
            for (i = 0; i < 3; i++) {
                ((Actor *)actor_pool)[i + 96].blink_period = 1;
            }
        }
    }
    if (game_state.mode_step >= 3 && game_state.mode_step < 6 && (game_state.pad_held & PAD_CROSS)) {
        mode_step_set(5);
    }
    stage_show_science_first_show_mode_step_dispatch();
}

void stage_show_science_first_show_mode_step_dispatch(void) {
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    switch (game_state.mode_step) {
    case 0:
        stage_show_scene_state_reset();
        return;
    case 1:
        stage_show_science_first_show_sector_load();
        return;
    case 2:
        stage_show_science_first_show_audio_start();
        return;
    case 3:
        stage_show_actor_brightness_ramp_up();
        return;
    case 4:
        stage_show_science_first_show_animation_tick();
        return;
    case 5:
        audio_command(0xC1);
        k_reset(1);
        mode_step_next();
        return;
    case 6:
        stage_show_actor_brightness_ramp_down();
        return;
    case 7:
        stage_show_scene_finish_wait();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void stage_show_science_first_show_audio_start(void) {
    audio_bank_start(1, 0x201);
    stage_show_science_first_show_actor_table_initialize();
    D_80120696 = 1;
    mode_step_next();
}

void stage_show_science_first_show_animation_tick(void) {
    stage_show_science_first_show_timed_audio_commands();
    stage_show_science_first_show_initial_frame_state();
    game_state.mode_step_timer.u += 1;
    if (D_80122EAC == 3) {
        if (game_state.mode_step_timer.u >= 0x25B) {
            mode_step_next();
        }
    } else if (!(D_80120652 & 1)) {
        mode_step_next();
    }
}

void stage_show_science_first_show_actor_table_initialize(void) {
    s32 var_s0;

    var_s0 = 0;
    do {
        load_palette(D_8013C74C, (var_s0 + 0x10) & 0xFF, 1, 1, var_s0);
        var_s0 += 1;
    } while (var_s0 != 0x10);
    actor_clear(0x60);
    D_80120651 = 4;
    D_80120688 = 0x01000000;
    D_80120653 = 0x80;
    D_8012065C = D_8013C744;
    D_80120660 = D_8013C748;
    D_80120684 = (u8 *)D_8013C740;
    D_80120666 = (s16) D_80122EAC;
    D_80120656 = 1;
    D_80120657 = 0;
    D_80120676 = -0xA0;
    D_8012067A = -0x78;
    D_80120655 = 0x80;
    D_80120693 = 0;
    D_80120654 = 0;
    actor_clear(0x61);
    D_80120695 = 4;
    D_801206CC = 0x01000000;
    D_80120697 = 0x80;
    D_801206A0 = (s32) D_8013C744;
    D_801206A4 = (s32) D_8013C748;
    D_801206C8 = (s32) D_8013C740;
    D_801206AA = 4;
    D_8012069A = 1;
    D_8012069B = 0;
    D_801206BA = -0xA0;
    D_801206BE = -0x78;
    D_80120699 = 0x80;
    D_801206D7 = 0;
    D_80120698 = 0;
    actor_clear(0x62);
    D_801206D9 = 4;
    D_80120710 = 0x01000000;
    D_801206DB = 0x80;
    D_801206DA = 0x40;
    D_801206E4 = (s32) D_8013C744;
    D_801206E8 = (s32) D_8013C748;
    D_8012070C = (s32) D_8013C740;
    D_801206F0 = 0x58;
    D_801206DE = 1;
    D_801206DF = 0;
    D_801206FE = -0xA0;
    D_80120702 = -0x78;
    D_801206DD = 0x80;
    D_8012071B = 0;
    D_801206DC = 0;
    load_tpage_buf_lock(5, D_8013C740[0], 1);
    load_tpage_buf_lock(6, D_8013C740[1], 1);
    load_tpage_buf_lock(7, D_8013C740[2], 1);
    load_tpage_buf_lock(8, D_8013C740[3], 1);
    load_tpage_buf_lock(9, D_8013C740[4], 1);
    load_tpage_buf_lock(0xA, D_8013C740[5], 1);
    load_tpage_buf_lock(0x25, D_8013C740[6], 1);
    load_tpage_buf_lock(0x26, D_8013C740[7], 1);
    load_tpage_buf_lock(0x27, D_8013C740[8], 1);
    load_tpage_buf_lock(0x28, D_8013C740[9], 1);
    load_tpage_buf_lock(0x29, D_8013C740[10], 1);
    load_tpage_buf_lock(0x2A, D_8013C740[11], 1);
}

void stage_show_science_first_show_timed_audio_commands(void) {
    if (D_80122EAC == 3) {
        switch (game_state.mode_step_timer.u) {
        case 0x14C:
            audio_command(0x501);
            return;
        case 0x1BF:
            audio_command(0x502);
            return;
        }
    } else {
        switch (game_state.mode_step_timer.u) {
        case 0x157:
            audio_command(0x501);
            return;
        case 0x1CA:
            audio_command(0x502);
            return;
        }
    }
}

void stage_show_science_first_show_initial_frame_state(void) {
    if (game_state.mode_step_timer.w == 0) {
        D_80120652 = 5;
    }
}
