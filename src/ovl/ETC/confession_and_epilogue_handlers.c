/* confession_and_epilogue_handlers.c: ; src/ovl/ETC/80132000.c:48-60,143-153,167-173,230-253 covers confession scene flow and epilogue initialization/display. Original object 0x80132000-0x80132880 (config/objects/ETC.txt). */
#include "common.h"
#include "ovl/ETC.h"

void func_80132000(void) {
    if ((game_state.pad_pressed & PAD_CROSS) || ((game_state.frame_count & 0x2FF) == 0x2FF)) {
        mode_stage_next();
    }
}

void func_80132048(u8 arg0) {
    if ((game_state.pad_pressed & PAD_CROSS) || (game_state.frame_count & 0x2FF) == 0x2FF) {
        game_mode_set(arg0);
    }
}

void func_80132098(void) {
    switch (game_state.mode_stage) {
    case 0:
        func_80132148();
        break;
    case 1:
        func_80132204();
        break;
    case 2:
        func_80132290();
        break;
    case 3:
        func_80132344();
        break;
    }
    message_window_show();
    parameter_show();
    hizuke_show();
    hud_screen_frame_draw(0);
    hud_cursor_update();
    message_window_title_update();
}
void func_80132148(void) {
    switch (game_state.mode_step) {
    case 0:
        func_80132198();
        break;
    case 1:
        func_80132000();
        break;
    }
}
void func_80132198(void) {
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

void func_80132204(void) {
    switch (game_state.mode_step) {
    case 0:
        func_80132254();
        break;
    case 1:
        func_80132000();
        break;
    }
}
void func_80132254(void) {
    text_system_reset();
    message_window_set_title("外井告白シーン");
    k_disp_start(1);
    mode_step_next();
}

void func_80132290(void) {
    switch (game_state.mode_step) {
    case 0:
        func_801322E0();
        break;
    case 1:
        func_80132000();
        break;
    }
}
void func_801322E0(void) {
    func_80044774(0);
    cd_audio_request_set(0x3000656, 0xB000E0C, 0xF);
    audio_command(0x300);
    text_system_reset();
    message_window_set_title("女々しい野郎どもへ");
    k_disp_start(1);
    mode_step_next();
}

void func_80132344(void) {
    switch (game_state.mode_step) {
    case 0:
        func_80132394();
        break;
    case 1:
        func_80132048(0x63);
        break;
    }
}
void func_80132394(void) {
    text_system_reset();
    message_window_set_title("机の中に手紙が");
    k_disp_start(1);
    mode_step_next();
}

void func_801323D0(void) {
    switch (game_state.mode_stage) {
    case 0:
        func_80132460();
        break;
    case 1:
        func_80132558();
        break;
    case 2:
        func_801325E4();
        break;
    case 3:
        func_80132670();
        break;
    }
    hud_screen_frame_draw(0);
    message_window_title_update();
}
void func_80132460(void) {
    switch (game_state.mode_step) {
    case 0:
        func_801324B0();
        break;
    case 1:
        func_80132000();
        break;
    }
}
void func_801324B0(void) {
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

void func_80132558(void) {
    switch (game_state.mode_step) {
    case 0:
        func_801325A8();
        break;
    case 1:
        func_80132000();
        break;
    }
}
void func_801325A8(void) {
    text_system_reset();
    message_window_set_title("シルエット画面");
    k_disp_start(1);
    mode_step_next();
}

void func_801325E4(void) {
    switch (game_state.mode_step) {
    case 0:
        func_80132634();
        break;
    case 1:
        func_80132000();
        break;
    }
}
void func_80132634(void) {
    text_system_reset();
    message_window_set_title("バストアップスクロール");
    k_disp_start(1);
    mode_step_next();
}

void func_80132670(void) {
    switch (game_state.mode_step) {
    case 0:
        func_801326C0();
        break;
    case 1:
        func_80132048(0xC1);
        break;
    }
}
void func_801326C0(void) {
    text_system_reset();
    message_window_set_title("告白シーン");
    k_disp_start(1);
    mode_step_next();
}

void func_801326FC(void) {
    switch (game_state.mode_stage) {
    case 0:
        func_8013275C();
        break;
    case 1:
        func_801327F0();
        break;
    }
    message_window_title_update();
    hud_screen_frame_draw(0);
}
void func_8013275C(void) {
    switch (game_state.mode_step) {
    case 0:
        func_801327AC();
        break;
    case 1:
        func_80132000();
        break;
    }
}
void func_801327AC(void) {
    audio_command(0x200);
    text_system_reset();
    message_window_set_title("エピローグ用初期化");
    k_disp_start(1);
    mode_step_next();
}

void func_801327F0(void) {
    switch (game_state.mode_step) {
    case 0:
        func_80132840();
        break;
    case 1:
        func_80132048(0xC2);
        break;
    }
}
void func_80132840(void) {
    text_system_reset();
    message_window_set_title("エピローグ");
    k_disp_start(1);
    mode_step_next();
}
