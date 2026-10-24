#define MAIN_API_OVERRIDE_talk_work /* matched as u32 (main_api.h: s32), switch selector in $v1 (T-6010) */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 talk_work;

typedef struct {
    void (*f[41])();
} FnTbl41; /* size 0xA4 */
extern FnTbl41 D_801466D8;

void func_8013AF80(void) {
    D_80146600 = 0x801D212C;
    D_80146604 = 0x801D2130;
    D_80146608 = 0x801D2150;
    D_8014660C = (*(s16 *)0x801D216C);
    D_80146610 = 0x801B0000;
    D_80146614 = 0x801B2000;
    D_80146618 = 0x801B6000;
    D_8014661C = 0x801BA000;
    D_80146620 = 0x801BE000;
    D_80146624 = 0x801C2000;
    D_80146628 = 0x801C6000;
}

void func_8013B030(void) {
    switch (talk_work) {
    case 1:
        func_8013B094();
        return;
    case 7:
        func_8013B0D0();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_8013B094(void) {
    if (game_state.mode_stage == 0) {
        func_8013B10C();
        return;
    }
    game_mode_reset_to_title();
}

void func_8013B0D0(void) {
    if (game_state.mode_stage == 0) {
        func_8013B2C8();
        return;
    }
    game_mode_reset_to_title();
}

s32 func_8013B10C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    s32 level;
    GekoFnTbl42 tbl;

    tbl = D_80146630;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
    if (D_80122D04 != 0) {
        /* FAKE: Actor view on the first two stores keeps as1 from hoisting the unk_03C load above sb D_80120723. T-24095 */
        ((Actor *)actor_pool_96)[3].brightness = dec_bg_brightness;
        ((Actor *)actor_pool_96)[3].draw_flags |= 0x80;
        if ((u8)game_state.voice_mode != VOICE_OFF) {
            if (!D_8014662C) {
                D_8012071E = 3;
            }
            if (D_80120734 == 6 && !D_8014662C) {
                D_8012071E = 0;
            }
            if (cd_audio_state() & 4) {
                level = getCDlevel();
            } else {
                level = 0;
            }
            if (level != 0) {
                if (!D_8014662C) {
                    D_8012071E = 3;
                    D_8014662C = 1;
                }
            }
        } else if (!D_8014662C) {
            D_8012071E = 3;
            D_80120734 = 1;
            D_8014662C = 1;
        }
    } else {
        D_8012071F &= 0xFF7F;
    }
}

void func_8013B290(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x80FF);
    func_8013AF80();
    mode_step_next();
}

void func_8013B2C8(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl41 tbl;

    tbl = D_801466D8;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_8013B350(void) {
    D_80122CE0 = 6;
    actor_clear_range(0);
    cursor_show(0);
    game_state.saved_game_mode = game_state.game_mode;
    game_state.saved_mode_stage = game_state.mode_stage;
    game_state.saved_mode_step = game_state.mode_step + 1;
    game_mode_set(GAME_MODE_RPG_BATTLE);
}

void func_8013B3C0(void) {
    s32 pad; /* FAKE: unused scalar reproduces RECT home at +0x2C; original local unknown. T-9770 */
    RECT r;

    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    cursor_show(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    talk_clear();
    k_reset(1);
    r.x = 0x140;
    r.y = 0x100;
    r.w = 0x140;
    r.h = 0xF8;
    func_8009C7F8(&r, 8, 0, 0);
    r.x = 0x2C0;
    r.y = 0;
    func_8009C7F8(&r, 8, 0, 0);
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    /* FAKE: scalar byte store then member read preserves argument scheduling; sibling func_8013CC20 uses the same idiom. T-9770 */
    *(u8 *)&game_state.current_girl = 2;
    talk_names_set(game_state.current_girl);
    addr_init_bustup();
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    hizuke_init();
    message_window_init();
    text_marker_hide();
    func_80137990();
    D_80146274 = D_80145F10;
    D_80146278 = D_8014604C;
    D_8014627C = D_80146188;
    mode_step_next();
}

void func_8013B510(void) {
    game_state.current_girl = GIRL_YUINA;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    func_80137990();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    talk_names_set(game_state.current_girl);
    talk_clear();
    func_80137990();
    D_80146274 = D_80145EF8;
    D_80146278 = D_80146034;
    D_8014627C = D_80146170;
    strcpy(D_800CA19C, "屋上");
    D_8014662C = 0;
    game_state.girls[2].tokimeki += 1;
    game_state.girls[2].affection += 2;
    check_para_limit();
    D_800CA368 = 0xE;
    mode_step_next();
}

void func_8013B65C(void) {
    func_8013AF80();
    load_palette(D_80146610, 0x11, 1, 2, 0);
    scene_still_show(D_80146614, D_80146618, D_8014661C, D_80146620, D_80146624, D_80146628);
    scene_anim_show(0, 0, 0, 0);
    actor_clear(0x63);
    D_8012071D = 0xA;
    D_8012071E = 0;
    D_80120754 = 0x41000000;
    D_8012071F = 4;
    D_80120728 = D_80146604;
    D_8012072C = D_80146608;
    D_80120750 = D_80146600;
    D_80120730 = D_8014660C;
    D_80120732 = 4;
    D_80120734 = 0;
    D_80120724 = 0;
    D_80120722 = 1;
    D_8012075F = 0x11;
    D_80120742 = -0xA0;
    D_80120746 = -0x78;
    D_80120723 = 0;
    D_80120721 = 0;
    D_80120720 = 0;
    game_state.tpage_buf[26] = -1;
    mode_step_next();
}

void func_8013B7D0(void) {
    game_state.year = 0x62;
    game_state.month = 2;
    game_state.day = 0x1C;
    game_state.weekday = WEEKDAY_SATURDAY;
    game_state.current_girl = GIRL_YUINA;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    func_80137990();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    talk_names_set(game_state.current_girl);
    talk_clear();
    func_80137990();
    D_80146274 = D_80145F10;
    D_80146278 = D_8014604C;
    D_8014627C = D_80146188;
    strcpy(D_800CA19C, "近所の公園");
    D_800CA368 = 0x14;
    mode_step_next();
}

void func_8013B908(void) {
    bg_read_sub2(0x412C);
    mode_step_next();
}
