/* confession_and_epilogue_handlers.c: ; src/ovl/ETC/80132000.c:48-60,143-153,167-173,230-253 covers confession scene flow and epilogue initialization/display. Original object 0x80132000-0x80132880 (config/objects/ETC.txt). */
#include "common.h"
#include "ovl/ETC.h"

void misc_stub_wait_stage_next(void) {
    if ((game_state.pad_pressed & PAD_CROSS) || ((game_state.frame_count & 0x2FF) == 0x2FF)) {
        mode_stage_next();
    }
}

void misc_stub_wait_mode_set(u8 arg0) {
    if ((game_state.pad_pressed & PAD_CROSS) || (game_state.frame_count & 0x2FF) == 0x2FF) {
        game_mode_set(arg0);
    }
}

void confession_judge_stub_entry(void) {
    switch (game_state.mode_stage) {
    case 0:
        confession_judge_stub_stage_judge();
        break;
    case 1:
        confession_judge_stub_stage_butler();
        break;
    case 2:
        confession_judge_stub_stage_message();
        break;
    case 3:
        confession_judge_stub_stage_letter();
        break;
    }
    message_window_show();
    parameter_show();
    hizuke_show();
    hud_screen_frame_draw(0);
    hud_cursor_update();
    message_window_title_update();
}
void confession_judge_stub_stage_judge(void) {
    switch (game_state.mode_step) {
    case 0:
        confession_judge_stub_judge_init();
        break;
    case 1:
        misc_stub_wait_stage_next();
        break;
    }
}
void confession_judge_stub_judge_init(void) {
    s32 i;

    for (i = 0x60; i < 0xA0; i++) {
        actor_clear(i);
    }
    text_system_reset();
    message_window_set_title("告白判定");
    hizuke_init();
    k_disp_start(1);
    mode_step_next();
}

void confession_judge_stub_stage_butler(void) {
    switch (game_state.mode_step) {
    case 0:
        confession_judge_stub_butler_init();
        break;
    case 1:
        misc_stub_wait_stage_next();
        break;
    }
}
void confession_judge_stub_butler_init(void) {
    text_system_reset();
    message_window_set_title("外井告白シーン");
    k_disp_start(1);
    mode_step_next();
}

void confession_judge_stub_stage_message(void) {
    switch (game_state.mode_step) {
    case 0:
        confession_judge_stub_message_init();
        break;
    case 1:
        misc_stub_wait_stage_next();
        break;
    }
}
void confession_judge_stub_message_init(void) {
    audio_bgm_stop(0);
    cd_audio_request_set(0x3000656, 0xB000E0C, 0xF);
    audio_command(0x300);
    text_system_reset();
    message_window_set_title("女々しい野郎どもへ");
    k_disp_start(1);
    mode_step_next();
}

void confession_judge_stub_stage_letter(void) {
    switch (game_state.mode_step) {
    case 0:
        confession_judge_stub_letter_init();
        break;
    case 1:
        misc_stub_wait_mode_set(0x63);
        break;
    }
}
void confession_judge_stub_letter_init(void) {
    text_system_reset();
    message_window_set_title("机の中に手紙が");
    k_disp_start(1);
    mode_step_next();
}

void confession_scene_stub_entry(void) {
    switch (game_state.mode_stage) {
    case 0:
        confession_scene_stub_stage_data_load();
        break;
    case 1:
        confession_scene_stub_stage_silhouette();
        break;
    case 2:
        confession_scene_stub_stage_bustup_scroll();
        break;
    case 3:
        confession_scene_stub_stage_scene();
        break;
    }
    hud_screen_frame_draw(0);
    message_window_title_update();
}
void confession_scene_stub_stage_data_load(void) {
    switch (game_state.mode_step) {
    case 0:
        confession_scene_stub_data_load_init();
        break;
    case 1:
        misc_stub_wait_stage_next();
        break;
    }
}
void confession_scene_stub_data_load_init(void) {
    if (game_state.substep == 0) {
        audio_bank_load_start(0, 0, 0, 0xCE6D, 0xCE33, 0xCE1E);
        game_state.substep += 1;
    } else if (audio_bank_load_poll() == 1) {
        audio_command(0x201);
        actor_clear_range(0);
        text_system_reset();
        message_window_set_title("告白シーンデータリード");
        k_disp_start(1);
        mode_step_next();
    }
}

void confession_scene_stub_stage_silhouette(void) {
    switch (game_state.mode_step) {
    case 0:
        confession_scene_stub_silhouette_init();
        break;
    case 1:
        misc_stub_wait_stage_next();
        break;
    }
}
void confession_scene_stub_silhouette_init(void) {
    text_system_reset();
    message_window_set_title("シルエット画面");
    k_disp_start(1);
    mode_step_next();
}

void confession_scene_stub_stage_bustup_scroll(void) {
    switch (game_state.mode_step) {
    case 0:
        confession_scene_stub_bustup_scroll_init();
        break;
    case 1:
        misc_stub_wait_stage_next();
        break;
    }
}
void confession_scene_stub_bustup_scroll_init(void) {
    text_system_reset();
    message_window_set_title("バストアップスクロール");
    k_disp_start(1);
    mode_step_next();
}

void confession_scene_stub_stage_scene(void) {
    switch (game_state.mode_step) {
    case 0:
        confession_scene_stub_scene_init();
        break;
    case 1:
        misc_stub_wait_mode_set(0xC1);
        break;
    }
}
void confession_scene_stub_scene_init(void) {
    text_system_reset();
    message_window_set_title("告白シーン");
    k_disp_start(1);
    mode_step_next();
}

void epilogue_stub_entry(void) {
    switch (game_state.mode_stage) {
    case 0:
        epilogue_stub_stage_prepare();
        break;
    case 1:
        epilogue_stub_stage_show();
        break;
    }
    message_window_title_update();
    hud_screen_frame_draw(0);
}
void epilogue_stub_stage_prepare(void) {
    switch (game_state.mode_step) {
    case 0:
        epilogue_stub_prepare_init();
        break;
    case 1:
        misc_stub_wait_stage_next();
        break;
    }
}
void epilogue_stub_prepare_init(void) {
    audio_command(0x200);
    text_system_reset();
    message_window_set_title("エピローグ用初期化");
    k_disp_start(1);
    mode_step_next();
}

void epilogue_stub_stage_show(void) {
    switch (game_state.mode_step) {
    case 0:
        epilogue_stub_show_init();
        break;
    case 1:
        misc_stub_wait_mode_set(0xC2);
        break;
    }
}
void epilogue_stub_show_init(void) {
    text_system_reset();
    message_window_set_title("エピローグ");
    k_disp_start(1);
    mode_step_next();
}
