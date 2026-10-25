/* graduation_epilogue_handlers.c: ; src/ovl/ENDING/8013B6A0.c:5-21 dispatches epilogue scene steps. Original object 0x8013B6A0-0x8013BF00 (config/objects/ENDING.txt). */
#include "common.h"
#include "ovl/ending.h"
#include "common.h"
#include "game_enums.h"
#include "libapi.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/cd.h"
#include "main/date.h"
#include "main/draw.h"
#include "main/game_mode.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/scenes.h"
#include "main/import_views/scenes_school_events.h"
#include "main/message.h"
#include "main/portrait.h"
#include "main/scene.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "main/tpage.h"
#include "main/vram.h"

void ending_set_song_scene_assets(void) {
    D_8013CBB0.unk_00 = 0x801C22CC;
    D_8013CBB0.unk_04 = 0x801C22D4;
    D_8013CBB0.unk_08 = 0x801C22F0;
    D_8013CBB0.unk_0C = *(s16 *)0x801C2300;
    D_8013CBB0.unk_10[0] = 0x801A0000;
    D_8013CBB0.unk_10[1] = 0x801C2000;
    D_8013CBB0.unk_10[2] = 0x801AA000;
    D_8013CBB0.unk_10[3] = 0x801AE000;
    D_8013CBB0.unk_10[4] = 0x801B2000;
    D_8013CBB0.unk_10[5] = 0x801B6000;
    D_8013CBB0.unk_10[6] = 0x801BA000;
    D_8013CBB0.unk_10[7] = 0x801BE000;
}

void ending_song_scene_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    EdFnTbl25 tbl;

    tbl = D_8013CBE4;
    if (game_state.mode_stage == 7 && game_state.mode_step < 0x10) {
        portrait_wink_update();
    }
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
    if (game_state.mode_stage == 7) {
        if (game_state.mode_step < 0x10) {
            check_k_scroll();
            k_disp_inc2();
            hud_screen_frame_draw(2);
            message_window_show();
            message_window_title_update();
            hizuke_show();
            scene_frame_update();
        }
    }
}

void ending_set_mode_stage_6(void) {
    mode_stage_set(6);
}

void ending_set_mode_stage_8(void) {
    mode_stage_set(8);
}

void ending_wait_scene_flag_or_exit_girl(void) {
    if (game_state.scene_flags[82] != 0) {
        mode_step_next();
        return;
    }
    normal_date_girl_out();
}

void ending_read_yoshio_scene_data(void) {
    cd_read_sectors(0x45, 0x801A0000, 0x727A);
    ending_set_song_scene_assets();
    mode_step_next();
}

s32 ending_start_yoshio_dialogue(void) {
    if (game_state.scene_flags[82] != 0) {
        if ((u32) game_state.mode_step_timer.w++ < 0x78U) {
            return 0;
        }
    }
    game_state.current_girl = GIRL_YOSHIO;
    talk_group = 0;
    talk_line = 0;
    talk_names_set(game_state.scene_flags[82]);
    ending_graduation_dialogue_resource_table_init();
    talk_text_table = ending_dialogue_resource_tables[0][0];
    talk_command_table = ending_dialogue_resource_tables[1][0];
    talk_voice_table = ending_dialogue_resource_tables[2][0];
    if (game_state.scene_flags[82] != 0) {
        talk_group = 2;
        game_state.mode_step += 0xA;
    }
    mode_step_next();
}

void ending_song_scene_initialize(void) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    hud_cursor_show(0);
    back_clear_switch(1);
    tpage_buf_clear_all();
    game_state.tpage_buf[28] = 1;
    game_state.tpage_buf[29] = 1;
    game_state.tpage_buf[30] = 1;
    text_system_reset();
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    talk_clear();
    game_state.screen_brightness = 0x80;
    bg_brightness = 0;
    bg_fade_amount = 0;
    ending_set_song_scene_assets();
    load_palette(D_8013CBB0.unk_10[1], 0x11, 1, 2, 0);
    load_palette(D_8013CBB0.unk_10[0], 0x12, 1, 2, 0);
    scene_still_show(D_8013CBB0.unk_10[2], D_8013CBB0.unk_10[3], D_8013CBB0.unk_10[4], D_8013CBB0.unk_10[5], D_8013CBB0.unk_10[6], D_8013CBB0.unk_10[7]);
    scene_anim_show(D_8013CBB0.unk_04, D_8013CBB0.unk_08, D_8013CBB0.unk_00, D_8013CBB0.unk_0C);
    sprite_quads[61].h = sprite_quads[62].h = sprite_quads[63].h = 0x70;
    actor_clear(0x61);
    D_80120666 = 0;
    D_80120654 = 8;
    D_80120693 = 0x12;
    D_80120652 = 0;
    D_80120653 = 4;
    mode_step_next();
}

void ending_song_title_audio_start(void) {
    s32 r;
    EdWords4 v;

    v = D_8013CD90;
    audio_command(0xBF);
    if (game_state.scene_flags[82] != 0) {
        cd_audio_request_set(v.w[2], v.w[3], 0xF);
    } else {
        cd_audio_request_set(v.w[0], v.w[1], 0xF);
    }
    audio_command(0x300);
    r = set_kanji_string(-0x50, 0x50, 0, "『女々しい野郎どもの詩』", 0);
    k_disp_start(r);
    ending_song_elapsed_frames = 0;
    mode_step_next();
}

void ending_advance_song_lyrics(void) {
    EdShorts13 tbl;
    s16 i;
    s16 sel;

    tbl = D_8013CDA0;
    sel = 0xFF;
    for (i = 0; i < 13; i++) {
        if (ending_song_elapsed_frames >= tbl.h[i]) {
            sel = i;
        }
    }
    if (ending_song_elapsed_frames == 0x3E8) {
        k_reset(1, sel, &tbl);
    }
    if (sel != 0xFF) {
        if (ending_song_elapsed_frames == tbl.h[sel]) {
            k_reset(1, sel, &tbl);
            ending_draw_song_lyric(sel);
        }
    }
    if (ending_song_elapsed_frames == 0x1536) {
        k_reset(1);
    }
    if (cd_audio_state() & 4) {
        ending_song_elapsed_frames += 1;
    }
    if (cd_last_command() == 9) {
        mode_step_next();
    }
}

void ending_draw_song_lyric(s16 lyric_index) {
    u8 *p;

    p = D_8013CC48 + lyric_index * 0x19;
    k_disp_start(set_kanji_string((s16) (-strlen(p) * 7 / 2), 0x50, 0, p, 0));
}

void ending_wait_song_visual_timeout(void) {
    if (game_state.mode_step_timer.w == 0) {
        D_80120652 = 5;
    }
    if ((D_80120668 == 2) && (D_80120658 == 4)) {
        D_80120652 = 1;
        D_80120668 = 0;
        D_80120658 = 0;
        D_80120666 = 1;
    }
    if ((u32)++game_state.mode_step_timer.w >= 0xB5U) {
        mode_step_next();
    }
}
