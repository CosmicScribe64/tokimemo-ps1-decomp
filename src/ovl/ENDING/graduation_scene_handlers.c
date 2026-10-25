/* graduation_scene_handlers.c: ; src/ovl/ENDING/80133540.c:5-18 dispatches mode-step callbacks and updates the ending presentation. Original object 0x80133540-0x80133C10 (config/objects/ENDING.txt). */
#include "common.h"
#include "ovl/ENDING.h"
#include "common.h"
#include "game_enums.h"
#include "libapi.h"
#include "libgpu.h"
#include "main/bg.h"
#include "main/date.h"
#include "main/game_mode.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/scenes.h"
#include "main/message.h"
#include "main/name_code.h"
#include "main/portrait.h"
#include "main/scene.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "main/vram.h"

void ending_companion_scene_step_dispatch(void) {
    s32 idx; /* FAKE: never read; declared first so tbl lands at the original frame offset (T-3330) */
    EdFnTbl43 tbl;

    tbl = ending_companion_scene_step_callbacks;
    portrait_wink_update();
    tbl.f[game_state.mode_step](0x80);
    check_k_scroll();
    k_disp_inc2();
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    hizuke_show();
    scene_frame_update();
}

void ending_set_selected_girl_portrait_layout(void) {
    if (ending_saved_girl == 0xC) {
        portrait_layout_set(4);
    } else {
        portrait_layout_set(0);
    }
    mode_step_next();
}

void ending_copy_image_pages_to_vram_buffer(void) {
    memcpy((void *)(0x801A0000 + game_state.mode_step_timer.w * 0x2800), 0x80180000 + game_state.mode_step_timer.w * 0x2800, 0x2800);
    game_state.mode_step_timer.w += 1;
    if (game_state.mode_step_timer.w == 9) {
        mode_step_next();
    }
}

void ending_decode_image_rectangle(void) {
    s32 pad; /* FAKE: unused local reproduces the extra 4-byte frame slot above rect; real source unknown. T-4030 */
    RECT rect;
    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C884(&rect, (void *)0x80180000);
    game_state.tpage_buf[37] = 1;
    game_state.tpage_buf[38] = 1;
    game_state.tpage_buf[39] = 1;
    game_state.tpage_buf[40] = 1;
    game_state.tpage_buf[41] = 1;
    game_state.tpage_buf[42] = 1;
    mode_step_next();
}

void ending_copy_image_pages_from_vram_buffer(void) {
    memcpy((void *)(0x80180000 + game_state.mode_step_timer.w * 0x2800), 0x801A0000 + game_state.mode_step_timer.w * 0x2800, 0x2800);
    game_state.mode_step_timer.w += 1;
    if (game_state.mode_step_timer.w == 9) {
        mode_step_next();
    }
}

void ending_show_image_rectangle(void) {
    RECT rect;
    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C8E0(&rect, (void *)0x80180000);
    mode_step_next();
}

void ending_dialogue_line_update(void) {
    if (talk_line == 4) {
        portrait_layout_set(4);
        load_palette(face_palette, 0x10, 1, 2, 1);
    }
    normal_date_speak();
    if (talk_line == 7) {
        D_80122D20 = 1;
        return;
    }
    D_80122D20 = 0;
}

void ending_date_girl_portrait_load(void) {
    if (game_state.date_girl == GIRL_SOTOI) {
        portrait_read_start(0x6B24);
    } else {
        portrait_read_start(0x5828);
    }
    mode_step_next();
}

void ending_load_scene_palette(void) {
    load_palette(face_palette, 0x10, 1, 2, 1);
    mode_step_next();
}

s32 ending_prepare_rei_dialogue(void) {
    if ((u32)game_state.girls[14].flags.w << 0x14 >> 0x1D < 3U) {
        mode_stage_next();
        return 0;
    }
    ending_saved_girl = game_state.current_girl;
    game_state.current_girl = GIRL_REI;
    game_state.date_girl = GIRL_REI;
    talk_group = 0;
    talk_line = 0;
    strcpy(name_code_girl_short, "外井");
    ending_graduation_dialogue_resource_table_init();
    talk_text_table = ending_dialogue_resource_tables[0][15];
    talk_command_table = ending_dialogue_resource_tables[1][15];
    talk_voice_table = ending_dialogue_resource_tables[2][15];
    mode_step_next();
}

INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/graduation_scene_handlers.rodata", D_8013BFB8);

void ending_set_rei_name_for_dialogue(void) {
    game_state.date_girl = GIRL_REI;
    strcpy(name_code_girl_short, D_8013BFB8);
    mode_step_next();
}

INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/graduation_scene_handlers.rodata", D_8013BFC0);

void ending_set_sotoi_name_for_dialogue(void) {
    game_state.date_girl = GIRL_SOTOI;
    strcpy(name_code_girl_short, D_8013BFC0);
    mode_step_next();
}

void ending_restore_saved_girl(void) {
    game_state.current_girl = ending_saved_girl;
    mode_step_next();
}

void ending_gate_candidate_scene_flags(void) {
    if (game_state.current_girl >= GIRL_YOSHIO) {
        mode_stage_set(7);
        D_800E7D34 |= 8;
        return;
    }
    mode_step_next();
    D_800E7D34 |= 4;
}

void ending_secondary_scene_step_dispatch(void) {
    s32 idx; /* FAKE: never read; declared first so tbl lands at the original frame offset (T-3330) */
    EdFnTbl9 tbl;

    tbl = ending_secondary_scene_step_callbacks;
    portrait_wink_update();
    tbl.f[game_state.mode_step](0x80);
    check_k_scroll();
    k_disp_inc2();
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    hizuke_show();
    scene_frame_update();
}

void ending_start_scene_dialogue_13(void) {
    talk_group = 0;
    talk_line = 0;
    ending_graduation_dialogue_resource_table_init();
    talk_text_table = ending_dialogue_resource_tables[0][13];
    talk_command_table = ending_dialogue_resource_tables[1][13];
    talk_voice_table = ending_dialogue_resource_tables[2][13];
    mode_step_next();
}

void ending_read_background_sector_4055(void) {
    bg_read_sub2(0x4055);
    mode_step_next();
}
