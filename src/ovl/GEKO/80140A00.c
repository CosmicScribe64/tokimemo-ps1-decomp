#define MAIN_API_OVERRIDE_dialogue_work /* switched as u32 (main_api.h: s32), T-6030 */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 dialogue_work;

void func_80140A00(void) {
    bg_read_sub2(0x4699);
    main_step_next();
}

void func_80140A28(void) {
    bg_read_sub2(0x4084);
    main_step_next();
}

void func_80140A50(void) {
    bg_read_sub2(0x4045);
    main_step_next();
}

void func_80140A78(void) {
    bg_read_sub2(0x4144);
    main_step_next();
}

void func_80140AA0(void) {
    D_80122D38 = 0;
    parameter_disp_switch(0);
    icon_disp_switch(0);
    bg_read_sub2(0x4144);
    main_step_next();
}

void func_80140AE0(void) {
    bg_read_sub2(0x415E);
    main_step_next();
}

void func_80140B08(void) {
    bg_read_sub2(0x4743);
    main_step_next();
}

void func_80140B30(void) {
    D_80147300 = 0x801B0000;
    D_80147304 = 0x801B2000;
    D_80147308 = 0x801B6000;
    D_8014730C = 0x801BA000;
    D_80147310 = 0x801BE000;
    D_80147314 = 0x801C2000;
    D_80147318 = 0x801C6000;
}

void func_80140BA0(void) {
    D_8014731C = 0x801CE150;
    D_80147320 = 0x801CE154;
    D_80147324 = 0x801CE1A4;
    D_80147328 = (*(s16 *)0x801CE1BC);
    D_8014732C = 0x801B0000;
    D_80147330 = 0x801B2000;
    D_80147334 = 0x801B6000;
    D_80147338 = 0x801BA000;
    D_8014733C = 0x801BE000;
    D_80147340 = 0x801C2000;
    D_80147344 = 0x801C6000;
}

void func_80140C50(void) {
    switch (dialogue_work) {
    case 1:
        func_80140CCC();
        return;
    case 2:
        func_80140D08();
        return;
    case 4:
        func_80140D44();
        return;
    default:
        game_reset_to_title();
        return;
    }
}

void func_80140CCC(void) {
    if (game_state.stage == 0) {
        func_80140D80();
        return;
    }
    game_reset_to_title();
}

void func_80140D08(void) {
    if (game_state.stage == 0) {
        func_80140E40();
        return;
    }
    game_reset_to_title();
}

void func_80140D44(void) {
    if (game_state.stage == 0) {
        func_80141760();
        return;
    }
    game_reset_to_title();
}

typedef struct {
    void (*f[27])();
} FnTbl27; /* size 0x6C */
extern FnTbl27 D_80147348;

void func_80140D80(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl27 tbl;

    tbl = D_80147348;
    idx = game_state.step;
    tbl.f[idx]();
}

void func_80140DF4(void) {
    set_kanji_string(-0x80, 0x30, 1, "ドン", 0);
    snd_command(0x24);
    snd_command(0x500);
    main_step_next();
}

typedef struct {
    void (*f[28])();
} FnTbl28; /* size 0x70 */
extern FnTbl28 D_801473B4;

void func_80140E40(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl28 tbl;

    tbl = D_801473B4;
    idx = game_state.step;
    tbl.f[idx](0x80);
}

s32 func_80140EBC(void) {
    func_80138AF8();
    if ((u16) D_800CA154 == 3) {
        if (game_state.step_timer.w++ == 1) {
            snd_command(0x501);
        }
    }
}

void func_80140F14(void) {
    cd_read_sectors(0x35, 0x801B0000, 0x8F43);
    main_step_next();
}

void func_80140F44(void) {
    load_palette(D_80147300, 0x11, 1, 2, 0);
    event_still_show(D_80147304, D_80147308, D_8014730C, D_80147310, D_80147314, D_80147318);
    event_anim_show(0, 0, 0, 0);
    main_step_next();
}

void func_80140FD0(void) {
    u32 value;
    GameState *g;

    g = &game_state;
    value = g->girls[11].flags.f.story_stage;
    if (value >= 5U) {
        g->girls[11].flags.f.story_stage = 4;
        value = game_state.girls[11].flags.f.story_stage;
    }
    D_800CA150 = (u16)D_800CA150 + value * 2;
    g->girls[11].flags.f.story_stage++;
    main_step_next();
}

void func_8014107C(void) {
    message_window_title_show(0);
    main_step_next();
}

void func_801410A4(void) {
    game_state.current_girl = GIRL_MIHARU;
    draw2d3d(1, 0);
    actors_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    strcpy(D_800CA19C, "廊下");
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
    strcpy(name_buf_family, game_state.player_family_name);
    strcpy(name_buf_girl_short, "謎の女");
    dialogue_clear();
    func_80137990();
    D_80146274 = D_80145FE8;
    D_80146278 = D_80146124;
    D_8014627C = D_80146260;
    D_800CA368 = 0x4A;
    main_step_next();
}

void func_801411DC(void) {
    s16 i;

    game_state.current_girl = GIRL_MIHARU;
    draw2d3d(1, 0);
    actors_draw_enable(1);
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
    strcpy(name_buf_family, game_state.player_family_name);
    dialogue_clear();
    D_80146274 = D_80145FEC;
    D_80146278 = D_80146128;
    D_8014627C = D_80146264;
    if (dialogue_work == 1) {
        game_state.param_stress.value -= 5;
        for (i = 0; i < 0xB; i++) {
            if (game_state.girls[i].flags.f.met) {
                game_state.girls[i].tokimeki -= 1;
            }
        }
        check_para_limit();
    }
    D_800CA368 = 0xFF;
    main_step_next();
}

void func_80141374(void) {
    s16 var_s0;

    game_state.current_girl = GIRL_MIHARU;
    draw2d3d(1, 0);
    actors_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    k_reset(1);
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    func_8006509C();
    var_s0 = 0;
    do {
        icon_can_use_set(var_s0, 0);
        var_s0 += 1;
    } while (var_s0 < 0xF);
    icon_bar_update(1);
    parameter_show_init();
    parameter_show_init();
    dec_bg_brightness = 0x80;
    strcpy(name_buf_family, game_state.player_family_name);
    strcpy(name_buf_girl_short, "館林");
    strcpy(girl_name_buf, "館林さん");
    dialogue_clear();
    D_80122D38 = 1;
    func_80137990();
    D_80146274 = D_80145FF4;
    D_80146278 = D_80146130;
    D_8014627C = D_8014626C;
    strcpy(D_800CA19C, "きらめき中央公園");
    strcpy(D_800CA1DC, "並木道");
    main_step_next();
}


void func_80141514(void) {
    func_80140BA0();
    load_palette(D_8014732C, 0x11, 1, 2, 0);
    event_still_show(D_80147330, D_80147334, D_80147338, D_8014733C, D_80147340, D_80147344);
    event_anim_show(D_80147320, D_80147324, D_8014731C, D_80147328);
    ((Actor *)actor_pool_96)->unk_04 = ((Actor *)actor_pool_96)[1].unk_04 = 8;
    D_800CA368 = 0x4D;
    main_step_next();
}

void func_801415D8(void) {
    bg_read_sub2(0x4045);
    main_step_next();
}

void func_80141600(void) {
    bg_read_sub2(0x403B);
    main_step_next();
}

void func_80141628(void) {
    bg_read_sub2(0x4708);
    main_step_next();
}

void func_80141650(void) {
    D_80122D38 = 0;
    parameter_disp_switch(0);
    icon_disp_switch(0);
    bg_read_sub2(0x42EB);
    main_step_next();
}

void func_80141690(void) {
    bg_read_sub2(0x430F);
    main_step_next();
}

void func_801416B8(void) {
    bg_read_sub2(0x47F7);
    main_step_next();
}

void func_801416E0(void) {
    s32 temp_v0;

    D_800B3D60 = 0;
    temp_v0 = dec_bg_cd_read(0x3FB6, 0);
    if (temp_v0 == *dec_bg_show) {
        main_step_next();
        main_step_next();
        main_step_next();
    } else if (temp_v0 == (1 - *dec_bg_show)) {
        main_step_next();
        main_step_next();
    }
    main_step_next();
}

void func_80141760(void) {
    u8 index;
    GekoCallbacks64 table;

    table = D_80147424;
    index = game_state.step;
    table.callback[index](0x20);
    index = game_state.step;
    if (index >= 36 && index < 43) {
        light_effect_draw();
    } else if (index == 49) {
        func_80139128();
    }
}

void func_8014181C(void) {
    if (game_state.substep == 0) {
        k_reset(0);
        message_window_set_title(D_800CA1DC);
        game_state.substep += 1;
    }
    normal_date_girl_in();
}

s32 func_80141870(void) {
    /* FAKE: dead entry-block read puts D_800CA154 in cvt_pass _entry_locations so uopt promotes it to $a0 (T-24115) */
    if (D_800CA154) {}
    if (check_end_k() != 0 && D_800CA154 == 1) {
        if (game_state.substep++ < 0x40U) {
            return 0;
        }
        game_state.substep = 0;
        k_reset(1);
    }
    if (D_800CA154 == 0 || D_800CA154 == 8) {
        func_80138B74();
    } else {
        func_80138AF8();
    }
    /* FAKE: signed .w in the first test and unsigned .u++ in the second stops uopt from CSE-merging the == 1 boolean while still CSE-ing the load (T-24115) */
    if (game_state.step_timer.w == 1 && D_800CA154 == 1) {
        snd_command(0x609);
    }
    if (game_state.step_timer.u++ == 1 && D_800CA154 == 9) {
        snd_command(0x60B);
    }
}

void func_8014197C(void) {
    snd_command(0x24);
    snd_command(0x500);
    main_step_next();
}

void func_801419AC(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8F78);
    func_80140BA0();
    main_step_next();
}

void func_801419E4(void) {
    snd_bank_load_start(1, 0xC043, 0xC01D, 0xCEFD, 0xCEB4, 0xCEB1);
    if (snd_bank_load_poll() == 1) {
        main_step_next();
    }
    main_step_next();
}

void func_80141A44(void) {
    func_80138AF8();
    D_800CA150 = (u16) D_800CA150 + 1;
    D_800CA154 = 0;
    main_step_next();
}

void func_80141A84(void) {
    k_reset(1);
    D_80122CE4 = 0;
    D_80122CF0 = 0;
    D_800CA2AC = 0;
    main_step_next();
}

void func_80141AC0(void) {
    bustup_read_start(0x6C05);
    main_step_next();
}
