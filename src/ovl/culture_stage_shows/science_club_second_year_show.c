/* science_club_second_year_show.c: src/ovl/BUNKA_SD/80135160.c:4; science-club show resources and callbacks; club/year dispatch is proved by src/ovl/culture_stage_shows/club_year_show_dispatch.c:14-51,63-89. Original object 0x80135160-0x801359B0 (config/objects/BUNKA_SD.txt). */
#define MAIN_API_OVERRIDE_D_80120656 /* 0x82 must not sign-extend (main_api.h: s8) */
#include "common.h"
#include "ovl/culture_stage_shows.h"

extern u8 D_80120656;

s32 stage_show_science_second_show_sector_load(void) {
    if (game_state.substep == 0) {
        cd_read_sectors(0x61, 0x80180000, 0x4E4E);
        game_state.substep += 1;
    } else if ((game_state.substep == 1) && (cd_read_sync_status() & 1)) {
        mode_step_next();
    }
}

void stage_show_science_second_show_input_update(void) {
    s32 i;

    if ((game_state.mode_step >= 3) && (game_state.mode_step < 9)) {
        if (game_state.pad_held & PAD_CIRCLE) {
            stage_show_science_second_show_mode_step_dispatch();
            D_80120656 = 0x82;
            D_8012069A = 0x82; for (i = 0; i < 2; i++) {
                D_8011ED58[(i + 1) * 0x110 + 0x18BA] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x18FE] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x1942] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x1876] = 0x82;
            }
        } else {
            D_80120656 = 1;
            D_8012069A = 1; for (i = 0; i < 2; i++) {
                D_8011ED58[(i + 1) * 0x110 + 0x18BA] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x18FE] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x1942] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x1876] = 1;
            }
        }
    }
    if ((game_state.mode_step >= 3) && (game_state.mode_step < 8) && (game_state.pad_held & PAD_CROSS)) {
        mode_step_set(7);
    }
    stage_show_science_second_show_mode_step_dispatch();
}

void stage_show_science_second_show_mode_step_dispatch(void) {
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    switch (game_state.mode_step) {
    case 0:
        stage_show_scene_state_reset();
        return;
    case 1:
        stage_show_science_second_show_sector_load();
        return;
    case 2:
        stage_show_science_second_show_audio_start();
        return;
    case 3:
        stage_show_actor_brightness_ramp_up();
        return;
    case 4:
        stage_show_science_second_show_animation_tick();
        return;
    case 5:
        stage_show_science_second_show_actor_brightness_fade();
        return;
    case 6:
        stage_show_science_second_show_display_state_update();
        return;
    case 7:
        audio_command(0xC1);
        k_reset(1);
        mode_step_next();
        return;
    case 8:
        stage_show_actor_brightness_ramp_down();
        return;
    case 9:
        stage_show_scene_finish_wait();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void stage_show_science_second_show_audio_start(void) {
    audio_bank_start(1, 0x201);
    stage_show_science_second_show_actor_texture_setup();
    mode_step_next();
}

void stage_show_science_second_show_animation_tick(void) {
    if (game_state.mode_step_timer.u == 0) {
        D_80120652 = 5;
    }
    game_state.mode_step_timer.u += 1;
    if (festival_dispatch_variant == 3) {
        if (game_state.mode_step_timer.u >= 0x1C2) {
            mode_step_next();
        }
    } else {
        if (!(D_80120652 & 1)) {
            mode_step_next();
        }
    }
}

s32 stage_show_science_second_show_display_state_update(void) {
    if (game_state.mode_step_timer.u == 0) {
        *(u8 *)&game_state.game_mode_scratch = 0x80;
        D_80120653 = 0;
        D_801206DB = 0;
        D_8012069B = 0x80;
        D_80120697 = 0x80;
        D_80120696 = 5;
    }
    message_window_title_update();
    game_state.mode_step_timer.u += 1;
    if (game_state.mode_step_timer.u % 9U == 0) {
        if ((D_801206D7 += 1) >= 0x15U) {
            D_801206D7 = 0x11;
        }
    }
    if (!(D_80120696 & 1)) {
        mode_step_next();
    }
}

/* FAKE: byte-view promotion and *= 0 retain the original v1 zero stores;
 * plain assignment folds them to zero-register stores. T-22021/T-24053. */
s32 stage_show_science_second_show_actor_brightness_fade(void) {
    s32 i;

    message_window_title_update();
    ((Actor *)actor_pool_96)[0].brightness = *(u8 *)&game_state.game_mode_scratch;
    ((Actor *)actor_pool_96)[2].brightness = *(u8 *)&game_state.game_mode_scratch;
    *(u8 *)&game_state.game_mode_scratch -= 2;
    if (*(u8 *)&game_state.game_mode_scratch == 0) {
        *(u8 *)&game_state.game_mode_scratch *= 0;
        for (i = 0; i < 0x40; i++) {
            ((Actor *)actor_pool_96)[i].brightness = *(u8 *)&game_state.game_mode_scratch;
        }
        mode_step_next();
    }
}

void stage_show_science_second_show_actor_texture_setup(void) {
    s32 var_s0;

    var_s0 = 0;
    do {
        load_palette(D_8013C72C, (var_s0 + 0x10) & 0xFF, 1, 1, var_s0);
        var_s0 += 1;
    } while (var_s0 != 0x10);
    actor_clear(0x60);
    D_80120651 = 4;
    D_80120688 = 0x01000000;
    D_80120653 = 0x80;
    D_8012065C = D_8013C724;
    D_80120660 = D_8013C728;
    D_80120684 = (u8 *)D_8013C720;
    D_80120666 = (s16) festival_dispatch_variant;
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
    D_80120697 = 0;
    D_801206A0 = (s32) D_8013C724;
    D_801206A4 = (s32) D_8013C728;
    D_801206C8 = (s32) D_8013C720;
    if (festival_dispatch_variant == 3) {
        D_801206AA = 5;
    } else {
        D_801206AA = 4;
    }
    D_8012069A = 1;
    D_8012069B = 0x80;
    D_801206BA = -0xA0;
    D_801206BE = -0x78;
    D_80120699 = 0;
    D_801206D7 = 0x11;
    D_80120698 = 0;
    actor_clear(0x62);
    D_801206D9 = 4;
    D_80120710 = 0x01000000;
    D_801206DB = 0x80;
    D_801206DA = 0x40;
    D_801206E4 = (s32) D_8013C724;
    D_801206E8 = (s32) D_8013C728;
    D_8012070C = (s32) D_8013C720;
    D_801206F0 = 0x80;
    D_801206DE = 1;
    D_801206DF = 0;
    D_801206FE = -0xA0;
    D_80120702 = -0x78;
    D_801206DD = 0x80;
    D_8012071B = 0;
    D_801206DC = 0;
    load_tpage_buf_lock(5, D_8013C720[0], 1);
    load_tpage_buf_lock(6, D_8013C720[1], 1);
    load_tpage_buf_lock(7, D_8013C720[2], 1);
    load_tpage_buf_lock(8, D_8013C720[3], 1);
    load_tpage_buf_lock(9, D_8013C720[4], 1);
    load_tpage_buf_lock(0xA, D_8013C720[5], 1);
    load_tpage_buf_lock(0x25, D_8013C720[6], 1);
    load_tpage_buf_lock(0x26, D_8013C720[7], 1);
    load_tpage_buf_lock(0x27, D_8013C720[8], 1);
    load_tpage_buf_lock(0x28, D_8013C720[9], 1);
}
