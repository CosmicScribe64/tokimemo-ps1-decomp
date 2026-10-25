/* ayako_scene_and_birthday_detours.c: src/ovl/SHUGAKU/80133D60.c:4-58; Ayako scene resources/callbacks with lighting, actor-copy/fade behavior and birthday redirects. The Ayako route selects mode_stage9 in 80138A60.c:144-151. Original object 0x80133D60-0x801344B0 (config/objects/SHUGAKU.txt). */
#include "common.h"
#include "ovl/SHUGAKU.h"
#include "main/scene.h"

void trip_ayako_scene_resource_pointer_setup(void) {
    D_8013BFD0 = (u8 *)0x801D2384;
    D_8013BFD4 = (u8 *)0x801D238C;
    D_8013BFD8 = (u8 *)0x801D23F0;
    D_8013BFDC = *(s16 *)0x801D240C;
    D_8013BFE0 = (u8 *)0x801B0000;
    D_8013BFE4 = (u8 *)0x801B2000;
    D_8013BFE8 = (u8 *)0x801B6000;
    D_8013BFEC = (u8 *)0x801BA000;
    D_8013BFF0 = (u8 *)0x801BE000;
    D_8013BFF4 = (u8 *)0x801C2000;
    D_8013BFF8 = (u8 *)0x801C6000;
}

void trip_ayako_mode_step_callback_dispatch(void) {
    u8 index;
    TripSceneStepCallbacks table;

    table = D_8013C008;
    if (game_state.mode_step < 8) {
        hud_sprite_light_effect_draw();
    } else if (game_state.mode_step >= 0x1D && game_state.mode_step < 0x23) {
        hud_sprite_light_effect_draw();
    }
    index = game_state.mode_step;
    table.f[index](0x80);
    index = game_state.mode_step;
    if (scene_still_fade_in == table.f[index]) {
        ((Actor *)actor_pool)[98].draw_flags |= 0x80;
        ((Actor *)actor_pool)[98].brightness = ((Actor *)actor_pool)[96].brightness;
    }
    if (index >= 0xD && index < 0x16) {
        ((Actor *)actor_pool)[98].brightness = ((Actor *)actor_pool)[96].brightness;
    }
    if (scene_still_fade_out == table.f[index]) {
        ((Actor *)actor_pool)[98].brightness = ((Actor *)actor_pool)[96].brightness;
        if (((Actor *)actor_pool)[98].brightness < 8) {
            ((Actor *)actor_pool)[98].draw_flags &= 0xFF7F;
        }
    }
}

void trip_special_birthday_route(void) {
    if (((game_state.girls[0].birthday.w & 0xF) == game_state.month) && (((u32) (game_state.girls[0].birthday.w << 0x17) >> 0x1B) == game_state.day)) {
        game_state.date_girl = game_state.current_girl;
        game_state.current_girl = GIRL_SHIORI;
        mode_stage_set(6);
        return;
    }
    if ((game_state.month == (game_state.player_birthday & 0xF)) && (game_state.day == ((u32) (game_state.player_birthday << 0x17) >> 0x1B))) {
        game_state.date_girl = game_state.current_girl;
        mode_stage_set(7);
        return;
    }
    mode_step_next();
}

s32 trip_ayako_talk_tick(void) {
    trip_ayako_response_talk_run();
    if (game_state.mode_step_timer.w++ == 0x3C) {
        D_801206AA = 1;
    }
}

void trip_scene_affection_update(void) {
    game_state.girls[game_state.current_girl].affection = game_state.girls[game_state.current_girl].affection + 2;
    check_para_limit();
    audio_command(0x201);
    trip_saved_mode_stage_restore();
}

void trip_ayako_scene_sector_load(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x83FB);
    trip_ayako_scene_resource_pointer_setup();
    mode_step_next();
}

void trip_liberal_arts_threshold_check(void) {
    if (game_state.param_liberal_arts.value >= TRIP_LIBERAL_ARTS_THRESHOLD) {
        mode_step_next();
    }
    mode_step_next();
}

void trip_ayako_scene_select(void) {
    game_state.current_girl = GIRL_AYAKO;
    D_80120666 = 2;
    D_801206EE = 4;
    talk_names_set(3);
    mode_step_next();
}

INCLUDE_RODATA("asm/ovl/SHUGAKU/data/SHUGAKU/ayako_scene_and_birthday_detours.rodata", D_8013AD70);

void trip_sotoi_speaker_setup(void) {
    game_state.current_girl = GIRL_SOTOI;
    D_80120666 = 4;
    D_801206EE = 2;
    strcpy(&name_code_girl_short, &D_8013AD70);
    mode_step_next();
}

void trip_liberal_arts_counter_update(void) {
    if (game_state.param_liberal_arts.value >= TRIP_LIBERAL_ARTS_THRESHOLD) {
        D_800CA2E4 += 3;
    }
    mode_step_next();
}

void trip_ayako_background_event_log(void) {
    audio_command(0x204);
    bg_read_sub2(0x476A);
    event_log_add(2, 0x1A);
    mode_step_next();
}

void trip_ayako_scene_display_setup(void) {
    trip_scene_script_resource_pointer_setup();
    D_8013BFFC = D_8013C530;
    D_8013C000 = D_8013C66C;
    D_8013C004 = D_8013C7A8;
    D_800CA2E4 = 0;
    D_800CA2E8 = 0;
    trip_ayako_scene_resource_pointer_setup();
    load_palette(D_8013BFE0, 0x11, 1, 2, 0);
    scene_still_show(D_8013BFE4, D_8013BFE8, D_8013BFEC, D_8013BFF0, D_8013BFF4, D_8013BFF8);
    scene_anim_show(D_8013BFD4, D_8013BFD8, D_8013BFD0, D_8013BFDC);
    ((Actor *)actor_pool_96)[1].unk_04 = 8;
    ((Actor *)actor_pool_96)->unk_04 = 8;
    *(Actor *) &actor_pool_96[0x88] = *(Actor *) actor_pool_96;
    ((Actor *)actor_pool_96)[2].sequence_slot = 4;
    mode_step_next();
}

void trip_ayako_response_talk_run(void) {
    talk_tables = &D_800CA2E4;
    D_800CA138 = &D_800CA2E8;
    D_800CA13C = D_8013BFFC;
    D_800CA140 = D_8013C000;
    D_800CA144 = D_8013C004;
    talk_run(D_80122CDC, 1, 0);
}

void trip_ayako_default_talk_run(void) {
    talk_tables = (u8 *) &D_800CA2E4;
    D_800CA138 = (u8 *) &D_800CA2E8;
    D_800CA13C = D_8013BFFC;
    D_800CA140 = D_8013C000;
    D_800CA144 = D_8013C004;
    talk_run(0xFF, 1, 0);
}
