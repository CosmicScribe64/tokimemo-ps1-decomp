/* runner_scene_handlers.c: ; src/ovl/TAIIKU/80132BD0.c:6-12 handles runner setup with four runner-loop counters. Original object 0x80132BD0-0x80133BF0 (config/objects/TAIIKU.txt). */
#include "common.h"
#include "ovl/TAIIKU.h"

#define DISP(n) (((Actor *)actor_pool)[n])

void func_80132BD0(void) {
    s32 i;
    s32 j; /* FAKE: five unused word locals reproduce the original frame (arrays at 0x68, frame 0x128); real locals unknown. T-19031 */
    s32 k;
    s32 l;
    s32 m;
    s32 n;
    TaiBytes23 spF8;
    TaiShorts23 spC8;
    TaiShorts23 sp98;
    TaiShorts23 sp68;

    spF8 = D_80148F10;
    spC8 = D_80148F28;
    sp98 = D_80148F58;
    sp68 = D_80148F88;
    for (i = 0; i < 0x17; i++) {
        actor_clear(i + 1);
        DISP(i + 1).quad_flags = 0x01000000;
        DISP(i + 1).unk_04 = 0;
        DISP(i + 1).unk_05 = 0x80;
        DISP(i + 1).unk_01 = spF8.v[i];
        DISP(i + 1).blink_period = 2;
        DISP(i + 1).unk_08 = 0;
        DISP(i + 1).frame_table = (u8 *)D_80149FCC;
        DISP(i + 1).sequence_table = (u8 *)D_8014A004;
        DISP(i + 1).tpage_table = (u8 *)D_8014A070;
        DISP(i + 1).unk_14 = D_8014A03C;
        DISP(i + 1).scale_x = 0x1000;
        DISP(i + 1).scale_y = 0x1000;
        DISP(i + 1).brightness = 0x80;
        DISP(i + 1).x = spC8.v[i] - 0xA0;
        DISP(i + 1).y = sp98.v[i] - 0x78;
        DISP(i + 1).unk_18 = 0;
        DISP(i + 1).sequence_slot = sp68.v[i];
    }
}

void func_80132E20(arg0, arg1)
s16 arg0;
s32 arg1;
{
    s32 i;
    s32 j;
    s32 t;
    s32 k; /* FAKE: k and l are unused word locals; the original frame has two more scalar slots (one above, one below the arrays), real locals unknown. T-19031 */
    s16 spA8[4];
    TaiShorts23 sp78;
    s32 l;

    sp78 = D_80148FB8;
    for (i = 0; i < 0x17; i++) {
        for (j = 0; j < arg1; j++) {
            spA8[j] = sp78.v[i] + j * 0x500 - arg0 - 0xA0;
        }
        for (j = 0; j < arg1; j++) {
            t = abs(spA8[j]);
            if (t < abs(spA8[0])) {
                spA8[0] = spA8[j];
            }
        }
        DISP(i + 1).x = spA8[0];
    }
    for (i = 0; i < 0x17; i++) {
        if ((DISP(i + 1).x < 0xB1) && (DISP(i + 1).x >= -0x127)) {
            DISP(i + 1).flags = 1;
            DISP(i + 1).draw_flags = 0xA4;
        } else {
            DISP(i + 1).flags = 0;
            DISP(i + 1).draw_flags = 0;
            DISP(i + 1).unk_18 = 0;
            DISP(i + 1).unk_08 = 0;
        }
    }
}

void func_801330D4(void) {
    func_801330FC();
    func_801331D0();
}

void func_801330FC(void) {
    /* FAKE: two unused word slots reproduce the eight-byte frame tail.
     * T-9560; original local declarations unknown. */
    s32 unused0;
    s32 unused1;
    RECT rect;
    s32 index;

    rect.x = 0x140;
    rect.y = 0;
    rect.w = 0x20;
    rect.h = 0x70;
    func_8004E44C(0, (u32 *)D_801491C4, D_801491C0);
    srn_init(0, D_801491C8, 0);
    srn_vram_set(0, 5, 0x580, 0xF0);
    func_8009C93C(&rect, 0x3C0, 0);
    for (index = 0; index < 11; index++) {
        game_state.tpage_buf[index + 5] = 8;
    }
}

void func_801331D0(void) {
    /* FAKE: unused 8-byte local reproduces the original frame (real source unknown). T-9150 */
    s32 pad[2];
    RECT r;
    s32 i;

    i = 0x140;
    do {
        r.x = i;
        r.y = 0x70;
        r.w = 0x40;
        r.h = 0x80;
        func_8009C884(&r, D_801491CC);
        i += 0x40;
    } while (i < 0x200);
    for (i = 0; i < 3; i++) {
        game_state.tpage_buf[i + 0x25] = 8;
    }
}

void func_80133258(void) {
    s32 i;
    TaiBytes10 sp88;
    TaiikuHalf10 sp74;
    TaiikuHalf10 sp60;
    TaiikuHalf10 sp4C;

    sp88 = D_80148FE8;
    sp74 = D_80148FF4;
    sp60 = D_80149008;
    sp4C = D_8014901C;
    for (i = 0; i < 0x17; i++) {
        actor_clear(i + 1);
    }
    for (i = 0; i < 10; i++) {
        /* FAKE: the (u8) narrowing takes one ugen temporary and emits no code; the original's temporaries (t6 first in the loop) need it, real source unknown. T-19031 */
        ((Actor *)actor_pool)[i + 1].quad_flags = ((u8)(i & 1) << 0x1E) | 0x01000000;
        ((Actor *)actor_pool)[i + 1].unk_05 = 0x80;
        ((Actor *)actor_pool)[i + 1].blink_period = 2;
        ((Actor *)actor_pool)[i + 1].frame_table = (u8 *)D_80149FCC;
        ((Actor *)actor_pool)[i + 1].sequence_table = (u8 *)D_8014A004;
        ((Actor *)actor_pool)[i + 1].tpage_table = (u8 *)D_8014A070;
        ((Actor *)actor_pool)[i + 1].unk_14 = D_8014A03C;
        ((Actor *)actor_pool)[i + 1].scale_x = 0x1000;
        ((Actor *)actor_pool)[i + 1].scale_y = 0x1000;
        ((Actor *)actor_pool)[i + 1].brightness = 0x80;
        ((Actor *)actor_pool)[i + 1].flags = 1;
        ((Actor *)actor_pool)[i + 1].draw_flags = 0xA4;
        ((Actor *)actor_pool)[i + 1].unk_01 = sp88.v[i];
        ((Actor *)actor_pool)[i + 1].x = sp74.h[i];
        ((Actor *)actor_pool)[i + 1].y = sp60.h[i];
        ((Actor *)actor_pool)[i + 1].sequence_slot = sp4C.h[i];
    }
}

void func_80133438(void) {
    func_80132E20(0, 1);
    Scroll(0, 0xF, 0, 0x70, 0x280);
    Scroll(0, 0xF, 0x70, 0x80, 0x80);
}

void func_80133494(void) {
    switch (game_state.mode_step) {
    case 0:
        audio_command(0xBF);
        actor_clear_range(0);
        mode_step_next();
        break;
    case 1:
        func_80059E00();
        break;
    case 2:
        func_80059BC0();
        break;
    case 3:
        func_8004ADE4();
        dec_bg_reset();
        set_dec_bri(0);
        cursor_actor_init(0);
        text_marker_hide();
        hizuke_init();
        message_window_init();
        talk_names_set(0U);
        text_system_reset();
        k_reset(0);
        message_window_set_title((s32) "　");
        message_window_title_show(0);
        game_state.screen_brightness = 0x80;
        Default_Disp();
        mode_step_next();
        break;
    case 4:
        cd_read_sectors(0x95U, 0x801A0000, 0x9B94);
        Default_Disp();
        mode_step_next();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_80068938(game_state.year, game_state.month, 0);
            mode_step_next();
        }
        Default_Disp();
        break;
    case 6:
        Default_Disp();
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    func_8006764C(0);
    cursor_show(0);
    icon_disp_switch(0);
    parameter_disp_switch(0);
}

void func_80133644(void) {
    portrait_wink_update();
    switch (game_state.mode_step) {
    case 0:
        switch (game_state.input_device) {
        case INPUT_MOUSE:
        case INPUT_NONE:
            mode_step_next();
            break;
        case INPUT_PAD:
            mode_stage_next();
            break;
        }
        break;
    case 1:
        portrait_read_step(0x541D);
        break;
    case 2:
        game_state.current_girl = GIRL_SHIORI;
        addr_init_bustup();
        portrait_show_checked();
        portrait_layout_set(1U);
        break;
    case 3:
        normal_date_girl_in();
        break;
    case 4:
        talk_names_set(0U);
        func_80132B90();
        talk_group = 3;
        talk_line = 0;
        talk_text_table = D_80148F00;
        talk_command_table = D_80148F04;
        talk_voice_table = D_80148F08;
        if (game_state.input_device == INPUT_NONE) {
            mode_step_set(6);
        } else {
            mode_step_next();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        talk_group = 4;
        mode_step_next();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (game_state.input_device == INPUT_PAD) {
            mode_step_next();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (game_state.pad_pressed != 0) {
            talk_clear();
            mode_stage_next();
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

void func_8013384C(arg0, arg1)
s32 arg0;
u8 arg1;
{
    /* Promoted parameter locals precede the table copies (T-9560). */
    s32 index;
    s32 column;
    TaiikuWords48 a;
    TaiikuWords48 b;

    a = D_80149030;
    b = D_801490F0;
    if (arg0 == 11) {
        index = 0;
    } else {
        index = arg0;
    }
    column = arg1;
    cd_audio_request_set(a.unk_00[arg0][column], b.unk_00[arg0][column], index);
    audio_command(0x300);
}

void func_80133928(s32 arg0) {
    u8 *p;

    func_801339C4();
    p = actor_pool + arg0 * 0x44;
    if (*(s16 *)(p + 0x26) >= 0x90) {
        *(s16 *)(p + 0x26) = 0x90;
    }
    if (*(s16 *)(p + 0x26) < -0x9F) {
        *(s16 *)(p + 0x26) = -0xA0;
    }
    if (*(s16 *)(p + 0x2A) >= 0x68) {
        *(s16 *)(p + 0x2A) = 0x68;
    }
    if (*(s16 *)(p + 0x2A) < -0x67) {
        *(s16 *)(p + 0x2A) = -0x68;
    }
}

void func_801339C4(arg0)
s32 arg0;
{
    u8 *p;
    s32 *ptr; /* FAKE: the pointer to the selector word moves the load into $v1 as in the original; found with decomp-permuter. T-10910 */
    int sel;

    sel = *(ptr = &game_state.pad_held) & 0xF000;

    switch (sel) {
    case 0x3000:
        p = actor_pool + arg0 * 0x44;
        *(s32 *)(p + 0x28) += -0x1E000;
        *(s32 *)(p + 0x24) += 0x22000;
        break;
    case 0x9000:
        p = actor_pool + arg0 * 0x44;
        *(s32 *)(p + 0x28) += -0x1E000;
        *(s32 *)(p + 0x24) += -0x22000;
        break;
    case 0x6000:
        p = actor_pool + arg0 * 0x44;
        *(s32 *)(p + 0x28) += 0x1E000;
        *(s32 *)(p + 0x24) += 0x22000;
        break;
    case 0xC000:
        p = actor_pool + arg0 * 0x44;
        *(s32 *)(p + 0x28) += 0x1E000;
        *(s32 *)(p + 0x24) += -0x22000;
        break;
    case 0x1000:
        p = actor_pool + arg0 * 0x44;
        *(s32 *)(p + 0x28) += -0x20000;
        break;
    case 0x4000:
        p = actor_pool + arg0 * 0x44;
        *(s32 *)(p + 0x28) += 0x20000;
        break;
    case 0x8000:
        p = actor_pool + arg0 * 0x44;
        *(s32 *)(p + 0x24) += -0x24000;
        break;
    case 0x2000:
        p = actor_pool + arg0 * 0x44;
        *(s32 *)(p + 0x24) += 0x24000;
        break;
    }
}
