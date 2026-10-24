#define MAIN_API_OVERRIDE_talk_work /* switched as u32 (main_api.h: s32), T-6030 */
#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[50])();
} FnTbl50; /* size 0xC8 */
extern FnTbl50 D_80146920;

typedef struct {
    void (*f[39])();
} FnTbl39; /* size 0x9C */
extern FnTbl39 D_80146884;

extern u32 talk_work;

void func_8013B930(void) {
    bg_read_sub2(0x46A3);
    mode_step_next();
}

void func_8013B958(void) {
    bg_read_sub2(0x42D8);
    mode_step_next();
}

void func_8013B980(void) {
    D_80146780 = 0x801B0000;
    D_80146784 = 0x801B2000;
    D_80146788 = 0x801B6000;
    D_8014678C = 0x801BA000;
    D_80146790 = 0x801BE000;
    D_80146794 = 0x801C2000;
    D_80146798 = 0x801C6000;
}

void func_8013B9F0(void) {
    switch (talk_work) {
    case 1:
        func_8013BA6C();
        return;
    case 2:
        func_8013BAA8();
        return;
    case 3:
        func_8013BAE4();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_8013BA6C(void) {
    if (game_state.mode_stage == 0) {
        func_8013BBE0();
        return;
    }
    game_mode_reset_to_title();
}

void func_8013BAA8(void) {
    if (game_state.mode_stage == 0) {
        func_8013BD04();
        return;
    }
    game_mode_reset_to_title();
}

void func_8013BAE4(void) {
    if (game_state.mode_stage == 0) {
        func_8013BE58();
        return;
    }
    game_mode_reset_to_title();
}

void func_8013BB20(void) {
    audio_bank_start(0, 0x200);
    mode_step_next();
}

s32 func_8013BB4C(void) {
    audio_command(0xCF);
    if ((u32)game_state.mode_step_timer.w++ < 0x40U) {
        return 0;
    }
    D_800B3D60 = 0;
    audio_bank_load_start(0, 0, 0, 0xD989, 0xD949, 0xD941);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

typedef struct {
    void (*f[58])();
} FnTbl58; /* size 0xE8 */
extern FnTbl58 D_8014679C;

void func_8013BBE0(void) {
    s32 idx; /* unused: its stack slot sits above tbl (T-3330) */
    FnTbl58 tbl;

    tbl = D_8014679C;
    if (game_state.mode_step == 0x1B) {
        audio_command(0xCF);
    }
    tbl.f[game_state.mode_step](0x80);
}

void func_8013BC74(void) {
    s32 cur; /* declared before prev: its stack slot sits above prev (T-3330) */
    s32 prev;

    prev = game_state.mode_step;
    func_8007C8A4();
    cur = game_state.mode_step;
    if (prev != cur) {
        game_state.mode_step = cur - 1;
        place_init();
    }
}

void func_8013BCBC(void) {
    s32 cur; /* declared before prev: its stack slot sits above prev (T-3330) */
    s32 prev;

    prev = game_state.mode_step;
    normal_date_bg_fadein();
    cur = game_state.mode_step;
    if (prev != cur) {
        game_state.mode_step = cur - 1;
        place_init2();
    }
}

void func_8013BD04(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl39 tbl;

    tbl = D_80146884;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_8013BD78(void) {
    cd_audio_request_set(0, 0, 0xC);
    audio_command(0x300);
    mode_step_next();
}

s32 func_8013BDB0(void) {
    if ((u32)game_state.mode_step_timer.w++ >= 0x11U) {
        k_reset(1);
        mode_step_next();
    }
}

void func_8013BDF8(void) {
    audio_command(0x500);
    mode_step_next();
}

void func_8013BE20(void) {
    cd_read_sectors(0x35, 0x801B0000, 0x8265);
    func_8013B980();
    mode_step_next();
}

void func_8013BE58(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl50 tbl;

    tbl = D_80146920;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

s32 func_8013BEE0(void) {
    func_80138AF8();
    if ((u16) D_800CA154 == 9 && check_end_k() != 0) {
        if (*(u8 *) &game_state.voice_mode == 0 || (*(u8 *) &game_state.voice_mode != 0 && func_80046094() == 9)) {
            D_80122CE4 = 0;
            k_reset(1);
            D_80122CF0 = 0;
            D_8011ED5B &= 0xFF7F;
            D_800CA2AC = 0;
            D_800CA150 = (u16) D_800CA150 + 1;
            D_800CA154 = 0;
            mode_step_next();
        }
    }
}

void func_8013BFA4(void) {
    audio_command(0x24);
    audio_command(0x503);
    mode_step_next();
}

void func_8013BFD4(void) {
    if (D_80122CDC != 0) {
        D_800CA150 = (u16) D_800CA150 + 5;
    }
    mode_step_next();
}

void func_8013C014(void) {
    mode_step_next();
    if (D_80122CDC != 0) {
        game_state.mode_step += 0x14;
        game_state.scene_flags[0x4F] = 1;
        return;
    }
    game_state.scene_flags[0x4F] = 2;
}

void func_8013C070(void) {
    mode_step_next();
}

void func_8013C090(void) {
    /* Chain over two GameState bytes, written as element stores: IDO then keeps the one
     * li v0,12 for both, as the original (T-9210). */
    (&game_state.current_girl)[0] = (&game_state.date_girl)[0] = 0xC;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    message_window_set_title("");
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    func_80137990();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    dec_bg_brightness = 0;
    talk_names_set(game_state.current_girl);
    talk_clear();
    func_80137990();
    D_80146274 = D_80145F14;
    D_80146278 = D_80146050;
    D_8014627C = D_8014618C;
    strcpy(D_800CA19C, "廊下");
    strcpy(D_800CA1DC, "廃工場前");
    if (talk_work == 1) {
        game_state.param_stress.value += 5;
    }
    check_para_limit();
    D_800CA368 = 0x4F;
    mode_step_next();
}

void func_8013C200(void) {
    /* Same chain as func_8013C090 (T-9210). */
    (&game_state.current_girl)[0] = (&game_state.date_girl)[0] = 0xC;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    message_window_set_title("");
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    func_80137990();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    dec_bg_brightness = 0;
    talk_names_set(game_state.current_girl);
    talk_clear();
    func_80137990();
    D_80146274 = D_80145F18;
    D_80146278 = D_80146054;
    D_8014627C = D_80146190;
    strcpy(D_800CA19C, "廊下");
    D_800CA368 = 0x50;
    mode_step_next();
}

void func_8013C330(void) {
    func_8013B980();
    load_palette(D_80146780, 0x11, 1, 2, 0);
    scene_still_show(D_80146784, D_80146788, D_8014678C, D_80146790, D_80146794, D_80146798);
    scene_anim_show(0, 0, 0, 0);
    mode_step_next();
}

void func_8013C3C4(void) {
    D_80146274 = D_80145F1C;
    D_80146278 = D_80146058;
    D_8014627C = D_80146194;
    D_800CA368 = 0x51;
    mode_step_next();
}

void func_8013C41C(void) {
    bg_read_sub2(0x4045);
    mode_step_next();
}

void func_8013C444(void) {
    bg_read_sub2(0x4943);
    mode_step_next();
}

void func_8013C46C(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}

void func_8013C494(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}
