/* yuina_scene_handlers.c: ; src/ovl/SHOUGATU/80137FF0.c:5-23 dispatches Yuina scene step tables; main loader routes the meeting mode here. Original object 0x80137FF0-0x80138B40 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "common.h"
#include "game/word_views.h"
#include "game_enums.h"
#include "libapi.h"
#include "libgpu.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/draw.h"
#include "main/game_mode.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/message.h"
#include "main/portrait.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "main/tpage.h"
#include "ovl/new_year_christmas.h"

void new_year_yuina_scene_group_dispatch(void) {
    switch (new_year_meeting_choice) {
    case 0:
        new_year_yuina_scene_primary_mode_step_dispatch();
        return;
    case 1:
        new_year_yuina_scene_secondary_mode_step_dispatch();
        return;
    case 2:
        new_year_yuina_scene_third_mode_step_dispatch();
        return;
    case 3:
        new_year_yuina_scene_fourth_mode_step_dispatch();
        return;
    case 4:
        new_year_yuina_scene_fifth_mode_step_dispatch();
        return;
    default:
        new_year_yuina_scene_default_mode_step_dispatch();
        return;
    }
}

void new_year_yuina_scene_primary_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80144F14;
    idx = game_state.mode_step;
    tbl.f[idx]();
    if (new_year_meeting_dialogue_group == 1) {
        if (new_year_meeting_dialogue_line == 8) {
            if (game_state.mode_step_timer.w++ == 0) {
                audio_command(0x500);
            }
        }
    }
}

s32 new_year_yuina_scene_dialogue_audio_tick(void) {
    new_year_meeting_talk_run();
    if (new_year_meeting_dialogue_line == 2) {
        if (game_state.mode_step_timer.w++ == 1) {
            audio_command(0x24);
            audio_command(0x501);
        }
    }
}

void new_year_yuina_scene_secondary_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80144F64;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_yuina_scene_third_mode_step_dispatch(void) {
    s32 idx;
    FnTbl37 table;

    table = D_80144FB4;
    idx = game_state.mode_step;
    if (new_year_yuina_club_three_flag == 0 && game_state.mode_step >= 0x13 && game_state.mode_step < 0x1C) {
        prim_disc_tiles(0x60, -0x10, 0x30, 8, 8, 0xD);
    }
    if (new_year_yuina_club_three_flag == 1 && game_state.mode_step >= 0x13 && game_state.mode_step < 0x1C) {
        prim_disc_tiles(-0x3C, -0x20, 0x20, 8, 8, 0xD);
    }
    idx = game_state.mode_step;
    table.f[idx]();
}

void new_year_yuina_scene_dialogue_audio_tick_alt(void) {
    new_year_meeting_talk_run();
    if (game_state.mode_step_timer.w++ == 0) {
        if (new_year_meeting_dialogue_line == 3) {
            audio_command(0x500);
        }
    }
}

void new_year_yuina_scene_audio_command_0500(void) {
    audio_command(0x500);
    mode_step_next();
}

void new_year_yuina_scene_audio_command_0501(void) {
    audio_command(0x501);
    mode_step_next();
}

void new_year_yuina_scene_group_state_advance(void) {
    if (new_year_yuina_club_three_flag != 0) {
        new_year_meeting_dialogue_group += 1;
    }
    mode_step_next();
}

void new_year_yuina_scene_fourth_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl30 tbl;

    tbl = D_80145048;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_yuina_scene_fifth_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl24 tbl;

    tbl = D_801450C0;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_yuina_scene_default_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80145120;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void new_year_yuina_scene_state_init(void) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    talk_clear();
    game_state.screen_brightness = 0x80;
    bg_brightness = 0;
    bg_fade_amount = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    talk_names_set(*(u8 *)&game_state.current_girl = 2);
    new_year_meeting_dialogue_addresses_init();
    new_year_meeting_dialogue_text_table = D_80144D94;
    new_year_meeting_dialogue_command_table = D_80144DB8;
    new_year_meeting_dialogue_voice_table = D_80144DDC;
    if (((u32)game_state.girls[2].flags.b[2] >> 4) == 3) {
        new_year_yuina_club_three_flag = 1;
    } else {
        new_year_yuina_club_three_flag = 0;
    }
    mode_step_next();
}

void new_year_yuina_scene_classroom_state_init(void) {
    new_year_meeting_dialogue_group = 0;
    new_year_meeting_dialogue_line = 0;
    strcpy(scene_place_title, "教室");
    mode_step_next();
}

void new_year_yuina_scene_classroom_state_init_alt(void) {
    new_year_meeting_dialogue_group = 3;
    new_year_meeting_dialogue_line = 0;
    strcpy(scene_place_title, "教室");
    mode_step_next();
}

void new_year_yuina_scene_corridor_lab_state_init(void) {
    s32 unused; /* FAKE: preserves RECT at stack +0x2C. T-10010. */
    RECT rect;

    rect.x = 0x480;
    rect.y = 0x160;
    rect.w = 0x10;
    rect.h = 0x10;
    func_8009C7F8(&rect, 0, 0, 0);
    ((NibbleView *)&game_state.player_flags.b[1])->high = ((NibbleView *)&game_state.girls[2].flags.b[2])->high;
    game_state.girls[game_state.current_girl].flags.f.b2 = 1;
    ((ShouByteFlags *)&game_state.girls[game_state.current_girl].flags.b[2])->b3 = 1;
    new_year_meeting_dialogue_group = 6;
    new_year_meeting_dialogue_line = 0;
    strcpy(scene_place_title, "廊下");
    strcpy(scene_place_title_2, "実験室");
    mode_step_next();
}

void new_year_yuina_scene_classroom_lab_state_init(void) {
    game_state.girls[game_state.current_girl].flags.f.b2 = 1;
    new_year_meeting_dialogue_group = 0xE;
    new_year_meeting_dialogue_line = 0;
    strcpy(scene_place_title, "教室");
    strcpy(scene_place_title_2, "実験室");
    mode_step_next();
}

void new_year_yuina_scene_laboratory_state_init(void) {
    game_state.girls[game_state.current_girl].flags.f.b2 = 1;
    new_year_meeting_dialogue_group = 0x12;
    new_year_meeting_dialogue_line = 0;
    strcpy(scene_place_title, "実験室");
    mode_step_next();
}

void new_year_yuina_scene_laboratory_state_init_alt(void) {
    game_state.girls[game_state.current_girl].flags.f.b2 = 1;
    new_year_meeting_dialogue_group = 0x17;
    new_year_meeting_dialogue_line = 0;
    strcpy(scene_place_title, "実験室");
    mode_step_next();
}

void new_year_yuina_scene_background_load_405f(void) {
    bg_read_sub2(0x405F);
    mode_step_next();
}

void new_year_yuina_scene_background_load_403b(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}

void new_year_yuina_scene_background_variant_load(void) {
    if (new_year_yuina_club_three_flag != 0) {
        bg_read_sub2(0x40D0);
    } else {
        bg_read_sub2(0x40C6);
    }
    mode_step_next();
}

void new_year_yuina_scene_background_load_40c6(void) {
    bg_read_sub2(0x40C6);
    mode_step_next();
}

void new_year_yuina_scene_background_load_40d0(void) {
    bg_read_sub2(0x40D0);
    mode_step_next();
}

void new_year_yuina_scene_state_conditional_advance(void) {
    if (new_year_yuina_club_three_flag != 0) {
        new_year_meeting_dialogue_group += 1;
    }
    mode_step_next();
}

void new_year_yuina_scene_state_conditional_advance_alt(void) {
    if (new_year_yuina_club_three_flag == 0) {
        new_year_meeting_dialogue_group += 1;
    }
    mode_step_next();
}
