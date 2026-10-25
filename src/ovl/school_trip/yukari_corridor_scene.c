/* yukari_corridor_scene.c: src/ovl/SHUGAKU/801344B0.c:4-82; picture/animation resources, corridor scene callbacks, dialogue and scores. The Yukari-specific branch sets mode_stage11 in 801378B0.c:397-400; 80136380.c routes it to trip_yukari_mode_step_callback_dispatch. Original object 0x801344B0-0x80134880 (config/objects/SHUGAKU.txt). */
#include "common.h"
#include "ovl/school_trip.h"

void trip_yukari_scene_resource_pointer_setup(void) {
    D_8013C0C0 = (u8 *)0x801D22B8;
    D_8013C0C4 = (u8 *)0x801D22C0;
    D_8013C0C8 = (u8 *)0x801D2310;
    D_8013C0CC = *(s16 *)0x801D2328;
    D_8013C0D0 = (u8 *)0x801B0000;
    D_8013C0D4 = (u8 *)0x801B2000;
    D_8013C0D8 = (u8 *)0x801B6000;
    D_8013C0DC = (u8 *)0x801BA000;
    D_8013C0E0 = (u8 *)0x801BE000;
    D_8013C0E4 = (u8 *)0x801C2000;
    D_8013C0E8 = (u8 *)0x801C6000;
}

void trip_yukari_mode_step_callback_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    TripSceneTransitionCallbacks tbl;

    tbl = D_8013C0EC;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void trip_yukari_scene_audio_transition(void) {
    audio_command(0x24);
    audio_command(0x206);
    audio_command(0x504);
    mode_step_next();
}

void trip_yukari_scene_sector_load(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x8902);
    trip_yukari_scene_resource_pointer_setup();
    mode_step_next();
}

/* FAKE: `* 0 + 8` folds to `li v1, 8` after uopt copy propagation, promoting D_80120698 to $v1 across the block (T-24104, cf. T-22021). */
void trip_scene_relationship_update(void) {
    trip_scene_script_resource_pointer_setup();
    trip_scene_text_table = D_8013C57C;
    trip_scene_command_table = D_8013C6B8;
    trip_scene_voice_table = D_8013C7F4;
    trip_yukari_scene_resource_pointer_setup();
    load_palette(D_8013C0D0, 0x11, 1, 2, 0);
    scene_still_show(D_8013C0D4, D_8013C0D8, D_8013C0DC, D_8013C0E0, D_8013C0E4, D_8013C0E8);
    scene_anim_show(D_8013C0C4, D_8013C0C8, D_8013C0C0, D_8013C0CC);
    D_80120698 = D_80120698 * 0 + 8;
    D_80120654 = D_80120698;
    trip_secondary_talk_group = 0;
    trip_secondary_talk_line = 0;
    game_state.girls[game_state.current_girl].tokimeki += 1;
    game_state.girls[game_state.current_girl].affection += 2;
    check_para_limit();
    strcpy(scene_place_title, "ホテル・廊下");
    mode_step_next();
}

void trip_yukari_background_load(void) {
    bg_read_sub2(0x4285);
    mode_step_next();
}

void trip_yukari_background_event_log(void) {
    bg_read_sub2(0x4774);
    event_log_add(2, 0x27);
    mode_step_next();
}

void trip_yukari_scene_actor_clear(void) {
    s32 pad; /* FAKE: unused local above `unused`, puts it at sp+0x2B as in the original; real source unknown. T-4010 */
    u8 unused; /* read uninitialised: the original passes the stack byte */

    get_g_zyotai_h(game_state.current_girl);
    portrait_value_clear_if_high(unused);
    actor_clear(0x60);
    actor_clear(0x61);
    actor_clear(0x62);
    mode_step_next();
}
