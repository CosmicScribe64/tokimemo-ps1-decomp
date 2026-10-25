/* scene_step_handlers.c: ; src/ovl/SHOUGATU/801397D0.c:25-42 dispatches five scene-step callbacks. Original object 0x801397D0-0x8013A190 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "ovl/new_year_christmas.h"

void new_year_scene_asset_addresses_init(void) {
    D_801453B0 = (u8 *)0x801E60CC;
    D_801453B4 = (u8 *)0x801E60D0;
    D_801453B8 = (u8 *)0x801E60E4;
    D_801453BC = *(s16 *)0x801E60F0;
    D_801453C0 = (u8 *)0x801AE000;
    D_801453C4 = (u8 *)0x801B0000;
    D_801453C8 = (u8 *)0x801B2000;
    D_801453CC = (u8 *)0x801B6000;
    D_801453D0 = (u8 *)0x801BA000;
    D_801453D4 = (u8 *)0x801BE000;
    D_801453D8 = (u8 *)0x801C2000;
    D_801453DC = (u8 *)0x801C6000;
    D_801453E0 = (u8 *)0x801CE000;
    D_801453E4 = (u8 *)0x801D2000;
    D_801453E8 = (u8 *)0x801D6000;
    D_801453EC = (u8 *)0x801DA000;
    D_801453F0 = (u8 *)0x801DE000;
    D_801453F4 = (u8 *)0x801E2000;
}

void new_year_scene_callback_group_dispatch(void) {
    u8 sel = new_year_meeting_choice; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        new_year_scene_callback_group_0_dispatch();
        return;
    case 1:
        new_year_scene_callback_group_1_dispatch();
        return;
    case 2:
        new_year_scene_callback_group_2_dispatch();
        return;
    case 3:
        new_year_scene_callback_group_3_dispatch();
        return;
    default:
        new_year_scene_callback_group_4_dispatch();
        return;
    }
}

void new_year_scene_callback_group_0_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_801453F8;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_scene_callback_group_1_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80145448;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_scene_callback_group_2_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl25 tbl;

    tbl = D_80145498;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_scene_callback_group_3_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_801454FC;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_scene_callback_group_4_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl37 tbl;

    tbl = D_8014554C;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

s32 new_year_scene_mio_audio_wait(void) {
    new_year_meeting_talk_run();
    if (new_year_meeting_dialogue_line == 2) {
        if (game_state.mode_step_timer.w++ == 1) {
            audio_command(0x501);
        }
    }
}

void new_year_scene_mio_current_girl_update(void) {
    game_state.current_girl = GIRL_MIO;
    new_year_meeting_talk_run();
}

void new_year_scene_resource_read(void) {
    cd_read_sectors(0x71, 0x801AE000, 0x8768);
    new_year_scene_asset_addresses_init();
    mode_step_next();
}

void new_year_library_scene_state_init(void) {
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    talk_clear();
    game_state.screen_brightness = 0x80;
    bg_brightness = 0;
    D_80122CF8 = 0;
    D_80122CF4 = 0;
    bg_fade_amount = 0;
    addr_init_bustup();
    text_marker_hide();
    /* FAKE: the store inside the call shares $a0 with the argument (T-9020, T-9170) */
    talk_names_set(*(u8 *)&game_state.current_girl = 1);
    new_year_meeting_dialogue_addresses_init();
    new_year_meeting_dialogue_text_table = D_80144DA0;
    /* three words of a 0x24-byte record array reached through one base: as1 keeps the loads in order */
    new_year_meeting_dialogue_command_table = (&D_80144DA0)[9];
    new_year_meeting_dialogue_voice_table = (&D_80144DA0)[18];
    mode_step_next();
}

void new_year_library_scene_selector_0(void) {
    new_year_meeting_dialogue_group = 0;
    new_year_meeting_dialogue_line = 0;
    strcpy(scene_place_title, "図書室");
    mode_step_next();
}

void new_year_library_scene_selector_3(void) {
    new_year_meeting_dialogue_group = 3;
    new_year_meeting_dialogue_line = 0;
    strcpy(scene_place_title, "図書室");
    mode_step_next();
}

void new_year_library_scene_selector_6(void) {
    new_year_meeting_dialogue_group = 6;
    new_year_meeting_dialogue_line = 0;
    strcpy(scene_place_title, "図書室");
    mode_step_next();
}

void new_year_library_target_scene_init(void) {
    ((Bits64B8 *)&game_state.girls[game_state.current_girl].flags.b[0])->f = 1;
    new_year_meeting_dialogue_group = 0xA;
    new_year_meeting_dialogue_line = 0;
    strcpy(scene_place_title, "図書室");
    mode_step_next();
}

void new_year_theater_scene_resource_setup(void) {
    game_state.girls[game_state.current_girl].flags.f.b2 = 1;
    new_year_meeting_dialogue_group = 0xD;
    new_year_meeting_dialogue_line = 0;
    strcpy(scene_place_title, "演劇部");
    new_year_scene_asset_addresses_init();
    if (((u8)game_state.month >= 6U) && ((u8)game_state.month < 0xAU)) {
        load_palette((s32)D_801453C0, 0x11, 1, 2, 0);
        scene_still_show((s32)D_801453C8, (s32)D_801453CC, (s32)D_801453D0, (s32)D_801453D4, (s32)D_801453D8, (s32)D_801453DC);
        scene_anim_show((s32)D_801453B4, (s32)D_801453B8, (s32)D_801453B0, (s32)D_801453BC);
    } else {
        load_palette((s32)D_801453C4, 0x11, 1, 2, 0);
        scene_still_show((s32)D_801453E0, (s32)D_801453E4, (s32)D_801453E8, (s32)D_801453EC, (s32)D_801453F0, (s32)D_801453F4);
        scene_anim_show((s32)D_801453B4, (s32)D_801453B8, (s32)D_801453B0, (s32)D_801453BC);
    }
    D_80120688 |= 0x80000000;
    D_80120666 = 0;
    D_80122D0C = 1;
    game_state.current_girl = GIRL_SOTOI;
    mode_step_next();
}

void new_year_scene_background_read_40bc_a(void) {
    bg_read_sub2(0x40BC);
    mode_step_next();
}

void new_year_scene_background_read_403b(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}

void new_year_scene_background_read_40bc_b(void) {
    bg_read_sub2(0x40BC);
    mode_step_next();
}

void new_year_scene_background_read_40b2(void) {
    bg_read_sub2(0x40B2);
    mode_step_next();
}

void new_year_scene_background_read_46c4(void) {
    bg_read_sub2(0x46C4);
    mode_step_next();
}

void new_year_scene_portrait_read_5d6e(void) {
    portrait_read_start(0x5D6E);
    mode_step_next();
}
