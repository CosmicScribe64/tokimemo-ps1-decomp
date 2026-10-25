/* corridor_choices_and_return.c: src/ovl/SHUGAKU/80134880.c:9-79,133; corridor picture resources, saved-stage branch selection, girl restoration, choices/counters and scene return; no callback-only ownership claim. Original object 0x80134880-0x80135430 (config/objects/SHUGAKU.txt). */
/* FAKE: s32 return view keeps count in $v1 across the call in trip_corridor_page_and_text_timing_update because void return lets uopt colour count into $v0. T-24091 */
#define SHUGAKU_OVERRIDE_trip_ayako_response_talk_run
#include "common.h"
#include "ovl/SHUGAKU.h"

s32 trip_ayako_response_talk_run(void);

void trip_corridor_scene_resource_pointer_setup(void) {
    D_8013C190 = (u8 *)0x801EA87C;
    D_8013C194 = (u8 *)0x801EA89C;
    D_8013C198 = (u8 *)0x801EA940;
    D_8013C19C = *(s16 *)0x801EA968;
    D_8013C1A0 = (u8 *)0x801B0000;
    D_8013C1A4 = (u8 *)0x801B2000;
    D_8013C1A8 = (u8 *)0x801B6000;
    D_8013C1AC = (u8 *)0x801BA000;
    D_8013C1B0 = (u8 *)0x801BE000;
    D_8013C1B4 = (u8 *)0x801C2000;
    D_8013C1B8 = (u8 *)0x801C6000;
}

void trip_saved_mode_stage_corridor_dispatch(void) {
    if (game_state.saved_mode_stage == 3) {
        trip_corridor_callback_dispatch_saved_mode_stage_three();
        return;
    }
    trip_corridor_callback_dispatch_and_actor_fade();
}

void trip_corridor_callback_dispatch_saved_mode_stage_three(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    TripSceneCallbacks tbl;

    tbl = D_8013C1C4;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void trip_corridor_return_date_girl_restore_current(void) {
    game_state.current_girl = game_state.date_girl;
    talk_names_set(game_state.date_girl);
    mode_step_next();
}

void trip_corridor_background_or_date_resume(void) {
    if (D_8013C1BC != 0) {
        normal_date_move_place();
        return;
    }
    if (game_state.substep == 0) {
        dec_bg_show_switch(1);
        game_state.substep += 1;
    }
    scene_bg_wipe_in();
}

void trip_corridor_window_title_and_scene_state(void) {
    strcpy(scene_place_title, "ホテル・廊下");
    trip_scene_script_resource_pointer_setup();
    D_8013BFFC = D_8013C5C4;
    D_8013C000 = D_8013C700;
    D_8013C004 = D_8013C83C;
    D_800CA2E4 = 0;
    D_800CA2E8 = 0;
    if (bg_brightness != 0) {
        D_8013C1BC = 1;
    } else {
        D_8013C1BC = 0;
    }
    mode_step_next();
}

void trip_saved_mode_stage_step_restore(void) {
    mode_stage_set(game_state.saved_mode_stage);
    mode_step_set(game_state.saved_mode_step);
}

void trip_saved_mode_stage_increment(void) {
    mode_stage_set((game_state.saved_mode_stage + 1) & 0xFF);
}

void trip_corridor_callback_dispatch_and_actor_fade(void) {
    u8 index; /* declared above table and reread after the call: same callback idiom as EVENT func_800FA6A0 (T-18000) */
    TripAlternateSceneCallbacks table;

    table = D_8013C228;
    index = game_state.mode_step;
    table.f[index](0x80);
    index = game_state.mode_step;
    if (scene_still_fade_in == table.f[index]) {
        ((Actor *)actor_pool)[98].draw_flags |= 0x80;
        ((Actor *)actor_pool)[98].brightness = ((Actor *)actor_pool)[96].brightness;
    }
    if (scene_still_fade_out == table.f[index]) {
        ((Actor *)actor_pool)[98].brightness = ((Actor *)actor_pool)[96].brightness;
        if (((Actor *)actor_pool)[96].brightness < 9) {
            ((Actor *)actor_pool)[98].draw_flags &= 0xFF7F;
        }
    }
}

void trip_hotel_birthday_route(void) {
    if (((game_state.girls[0].birthday.w & 0xF) == game_state.month) && (((u32) (game_state.girls[0].birthday.w << 0x17) >> 0x1B) == game_state.day)) {
        game_state.saved_mode_stage = 5;
        game_state.saved_mode_step = 0;
        game_state.date_girl = game_state.current_girl;
        game_state.current_girl = GIRL_SHIORI;
        mode_stage_set(6);
        return;
    }
    if ((game_state.month == (game_state.player_birthday & 0xF)) && (game_state.day == ((u32) (game_state.player_birthday << 0x17) >> 0x1B))) {
        game_state.saved_mode_stage = 5;
        game_state.saved_mode_step = 0;
        game_state.date_girl = game_state.current_girl;
        game_state.day_event_done_bits |= 8;
        mode_stage_set(7);
        return;
    }
    mode_step_next();
}

s32 trip_corridor_talk_and_effect_tick(void) {
    trip_ayako_response_talk_run();
    if ((game_state.mode_step_timer.w++ == 1) && (D_800CA2E8 == 2)) {
        D_80122D0C = 0;
        portrait_event_face_eyes_animation_start();
    }
}

s32 trip_corridor_default_talk_tick(void) {
    if (game_state.substep == 0) {
        k_reset(0);
        message_window_set_title("ホテル・廊下");
        game_state.substep += 1;
    }
    trip_ayako_response_talk_run();
    if (D_800CA2E8 == 1) {
        if (game_state.mode_step_timer.u++ == 1) {
            audio_command(0x503);
        }
    }
}

void trip_corridor_audio_transition(void) {
    audio_command(0x205);
    mode_step_next();
}

void trip_corridor_scene_sector_load(void) {
    cd_read_sectors(0x76, 0x801B0000, 0x8D47);
    trip_corridor_scene_resource_pointer_setup();
    mode_step_next();
}

void trip_yoshio_speaker_setup(void) {
    game_state.date_girl = game_state.current_girl;
    game_state.current_girl = GIRL_YOSHIO;
    talk_names_set(0xD);
    mode_step_next();
}

void trip_hotel_corridor_scene_display_setup(void) {
    trip_scene_script_resource_pointer_setup();
    D_8013BFFC = D_8013C5C4;
    D_8013C000 = D_8013C700;
    D_8013C004 = D_8013C83C;
    trip_corridor_scene_resource_pointer_setup();
    load_palette(D_8013C1A0, 0x11, 1, 2, 0);
    scene_still_show(D_8013C1A4, D_8013C1A8, D_8013C1AC, D_8013C1B0, D_8013C1B4, D_8013C1B8);
    scene_anim_show(D_8013C194, D_8013C198, D_8013C190, D_8013C19C);
    D_80120698 = 8;
    D_80120654 = 8;
    D_801206AC = 3;
    /* actor_pool_96 and D_801206D8 are records 96 and 98 of one table: the loads stay behind the stores. T-9310, T-20900 */
    ((Actor *)actor_pool_96)[2] = ((Actor *)actor_pool_96)[0];
    ((Actor *)actor_pool_96)[2].flags = 0x40;
    ((Actor *)actor_pool_96)[2].unk_18 = 0x28;
    ((Actor *)actor_pool_96)[2].unk_01 -= 1;
    D_800CA2E4 = 3;
    D_800CA2E8 = 0;
    game_state.tpage_buf[5] = -1;
    game_state.tpage_buf[6] = -1;
    game_state.tpage_buf[7] = -1;
    game_state.tpage_buf[8] = -1;
    game_state.tpage_buf[9] = -1;
    D_80122D0C = 1;
    D_8013C1C0 = *(u8 *)&game_state.voice_mode;
    mode_step_next();
}

void trip_response_affection_update(void) {
    if (D_80122CDC != 0) {
        game_state.mode_step += 0x1D;
        D_800CA2DC += 1;
        return;
    }
    event_log_add(2, 0x20);
    mode_step_next();
    D_800CA2F4 = 1;
    game_state.scene_flags[0x20] = 1;
    game_state.girls[game_state.current_girl].tokimeki += 5;
    game_state.girls[game_state.current_girl].affection += 2;
    check_para_limit();
}

void trip_shiori_birthday_route(void) {
    trip_destination_audio_transition();
    if ((game_state.girls[0].birthday.w & 0xF) == game_state.month && ((u32) (game_state.girls[0].birthday.w << 0x17) >> 0x1B) == game_state.day) {
        game_state.saved_mode_stage += 1;
        game_state.saved_mode_step = 0;
        game_state.date_girl = game_state.current_girl;
        game_state.current_girl = GIRL_SHIORI;
        mode_stage_set(6);
        return;
    }
    mode_stage_set((game_state.saved_mode_stage + 1) & 0xFF);
}

void trip_corridor_voice_mode_restore(void) {
    game_state.voice_mode = D_8013C1C0;
    D_800CA2DC = 0x29;
    mode_step_next();
}

void trip_corridor_page_and_text_timing_update(void) {
    s32 pad; /* FAKE: unused local above page and count gives the 0x38 frame and puts page at sp+0x30 and count at sp+0x2C; real source unknown. T-24091 */
    s32 page;
    s32 count;

    if (*(u8 *)&game_state.voice_mode == 2) {
        *(u8 *)&game_state.voice_mode = 1;
    }
    count = game_state.mode_step_timer.w;
    page = game_state.mode_step;
    if (!(game_state.pad_pressed & 0x600060) || (D_801206FE < -0xBF)) {
        trip_ayako_response_talk_run();
    }
    game_state.mode_step_timer.w = count + 1;
    if (D_801206FE >= -0xBF) {
        if (!(game_state.mode_step_timer.w & 1) && (check_end_k() != 0)
            && ((*(u8 *)&game_state.voice_mode == 0) || (cd_last_command() == 9))) {
            D_801206FE--;
        }
        D_8011ED5B &= 0x7F;
    }
    if (page != game_state.mode_step_timer.w) {
        if (D_801206FE >= -0xBF) {
            /* FAKE: u32 view prevents uopt from CSE-caching the s32 load of page across the branch, reproducing the reload from sp+0x30. T-24091 */
            game_state.mode_step = *(u32 *)&page;
            D_800CA2E4 = 9;
            D_800CA2E8 = 0;
        } else {
            D_800CA2E4 = 9;
            D_800CA2E8 = 1;
        }
    }
}
