/* scene_shiori.c: src/ovl/DATE/8014F0F0.c:7: mode 0x5E scenes of Shiori (girl 0). Original object 0x8014F0F0-0x80150010 (config/objects/DATE.txt). */
#define MAIN_API_OVERRIDE_talk_work /* switched as u32: selector in $v1 (main_api.h: s32) */
#include "common.h"
#include "ovl/DATE.h"

extern u32 talk_work;

void date_scene_shiori_autumn_data_init(void) {
    D_8015E690 = 0x801D21D4;
    D_8015E694 = 0x801D21DC;
    D_8015E698 = 0x801D21FC;
    D_8015E69C = *(s16 *)0x801D2210;
    D_8015E6A0 = 0x801B0000;
    D_8015E6A4 = 0x801B2000;
    D_8015E6A8 = 0x801B6000;
    D_8015E6AC = 0x801BA000;
    D_8015E6B0 = 0x801BE000;
    D_8015E6B4 = 0x801C2000;
    D_8015E6B8 = 0x801C6000;
}

void date_scene_shiori_spring_data_init(void) {
    D_8015E6BC = 0x801D21E0;
    D_8015E6C0 = 0x801D21E8;
    D_8015E6C4 = 0x801D2228;
    D_8015E6C8 = *(s16 *)0x801D2240;
    D_8015E6CC = 0x801B0000;
    D_8015E6D0 = 0x801B2000;
    D_8015E6D4 = 0x801B6000;
    D_8015E6D8 = 0x801BA000;
    D_8015E6DC = 0x801BE000;
    D_8015E6E0 = 0x801C2000;
    D_8015E6E4 = 0x801C6000;
}

void date_scene_shiori_winter_data_init(void) {
    D_8015E6E8 = 0x801D2320;
    D_8015E6EC = 0x801D2328;
    D_8015E6F0 = 0x801D2368;
    D_8015E6F4 = *(s16 *)0x801D2380;
    D_8015E6F8 = 0x801B0000;
    D_8015E6FC = 0x801B2000;
    D_8015E700 = 0x801B6000;
    D_8015E704 = 0x801BA000;
    D_8015E708 = 0x801BE000;
    D_8015E70C = 0x801C2000;
    D_8015E710 = 0x801C6000;
}

void date_scene_shiori_summer_data_init(void) {
    D_8015E714 = 0x801B0000;
    D_8015E718 = 0x801B2000;
    D_8015E71C = 0x801B6000;
    D_8015E720 = 0x801BA000;
    D_8015E724 = 0x801BE000;
    D_8015E728 = 0x801C2000;
    D_8015E72C = 0x801C6000;
}

void date_scene_shiori_dispatch(void) {
    switch (talk_work) {
    case 3:
        date_scene_shiori_autumn();
        return;
    case 4:
        date_scene_shiori_spring();
        return;
    case 5:
        date_scene_shiori_winter();
        return;
    case 6:
        date_scene_shiori_summer();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void date_scene_shiori_autumn_setup(void) {
    date_scene_text = D_8015DE70;
    date_scene_command = D_8015DFAC;
    date_scene_voice = D_8015E0E8;
    load_palette(D_8015E6A0, 0x11, 1, 2, 0);
    scene_still_show(D_8015E6A4, D_8015E6A8, D_8015E6AC, D_8015E6B0, D_8015E6B4, D_8015E6B8);
    scene_anim_show(D_8015E694, D_8015E698, D_8015E690, D_8015E69C);
    game_state.girls[0].tokimeki += 2;
    game_state.girls[0].syoushin -= 0x1C;
    check_para_limit();
    mode_step_next();
}

void date_scene_shiori_spring_setup(void) {
    date_scene_text = D_8015DE74;
    date_scene_command = D_8015DFB0;
    date_scene_voice = D_8015E0EC;
    load_palette(D_8015E6CC, 0x11, 1, 2, 0);
    scene_still_show(D_8015E6D0, D_8015E6D4, D_8015E6D8, D_8015E6DC, D_8015E6E0, D_8015E6E4);
    scene_anim_show(D_8015E6C0, D_8015E6C4, D_8015E6BC, D_8015E6C8);
    game_state.girls[0].tokimeki += 3;
    game_state.girls[0].affection += 2;
    game_state.girls[0].syoushin -= 0x14;
    check_para_limit();
    mode_step_next();
}

void date_scene_shiori_winter_setup(void) {
    date_ride_index = 2;
    date_scene_text = D_8015DE78;
    date_scene_command = D_8015DFB4;
    date_scene_voice = D_8015E0F0;
    game_state.girls[0].tokimeki += 1;
    game_state.girls[0].affection += 1;
    game_state.girls[0].syoushin -= 0x14;
    check_para_limit();
    D_80122D08 = 0x17;
    D_800CA21C[0] = 2;
    D_800CA21C[2] = 2;
    D_800CA21C[1] = 2;
    D_800CA224[0] = 2;
    D_800CA224[2] = 2;
    D_800CA224[1] = 2;
    D_800CA22C[0] = 2;
    D_800CA22C[2] = 2;
    D_800CA22C[1] = 2;
    D_800CA234[0] = 1;
    D_800CA234[2] = 1;
    D_800CA234[1] = 1;
    mode_step_next();
}

void date_scene_shiori_winter_still_setup(void) {
    date_scene_shiori_winter_data_init();
    load_palette(D_8015E6F8, 0x11, 1, 2, 0);
    scene_still_show(D_8015E6FC, D_8015E700, D_8015E704, D_8015E708, D_8015E70C, D_8015E710);
    scene_anim_show(D_8015E6EC, D_8015E6F0, D_8015E6E8, D_8015E6F4);
    D_800CA360 = 1;
    mode_step_next();
}

void date_scene_shiori_summer_setup(void) {
    date_scene_text = D_8015DE7C;
    date_scene_command = D_8015DFB8;
    date_scene_voice = D_8015E0F4;
    load_palette(D_8015E714, 0x11, 1, 2, 0);
    scene_still_show(D_8015E718, D_8015E71C, D_8015E720, D_8015E724, D_8015E728, D_8015E72C);
    scene_anim_show(0, 0, 0, 0);
    game_state.girls[0].tokimeki += 1;
    game_state.girls[0].affection += 2;
    game_state.girls[0].syoushin -= 0x14;
    check_para_limit();
    mode_step_next();
}

void date_scene_shiori_autumn_background_1(void) {
    bg_read_sub2(0x47BA);
    mode_step_next();
}

void date_scene_shiori_spring_background_1(void) {
    bg_read_sub2(0x47CD);
    mode_step_next();
}

void date_scene_shiori_spring_background_2(void) {
    bg_read_sub2(0x42EB);
    mode_step_next();
}

void date_scene_shiori_winter_background_1(void) {
    bg_read_sub2(0x4350);
    mode_step_next();
}

void date_scene_shiori_winter_background_2(void) {
    bg_read_sub2(0x4800);
    mode_step_next();
}

void date_scene_shiori_winter_background_3(void) {
    bg_read_sub2(0x432B);
    mode_step_next();
}

void date_scene_shiori_summer_background_1(void) {
    bg_read_sub2(0x48EA);
    mode_step_next();
}

void date_scene_shiori_summer_background_2(void) {
    bg_read_sub2(0x45BF);
    mode_step_next();
}

void date_scene_reset_choices(void) {
    D_80122CDC = 0;
    D_800CA21C[0] = 1;
    D_800CA21C[1] = 1;
    D_800CA21C[2] = 1;
    D_800CA234[0] = 0;
    D_800CA234[1] = 0;
    D_800CA234[2] = 0;
    D_800CA224[0] = 1;
    D_800CA224[1] = 1;
    D_800CA224[2] = 1;
    D_800CA22C[0] = 0;
    D_800CA22C[1] = 0;
    D_800CA22C[2] = 0;
    mode_step_next();
    D_800CA150 = 0;
    D_800CA154 = 0;
    date_scene_sections_init();
}

void date_scene_shiori_autumn(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl41 tbl;

    tbl = date_scene_shiori_autumn_steps;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void date_scene_shiori_autumn_load_data(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x7F61);
    date_scene_shiori_autumn_data_init();
    mode_step_next();
}

void date_scene_shiori_spring(void) {
    s32 idx; /* FAKE: never read; declared first so tbl lands at the original frame offset (T-3330) */
    FnTbl43 tbl;

    tbl = date_scene_shiori_spring_steps;
    if (game_state.mode_step < 0xAU && dec_bg_brightness == 0x80) {
        hud_sprite_light_effect_draw();
    }
    tbl.f[game_state.mode_step]();
}

void date_scene_shiori_spring_load_data(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x7FA6);
    date_scene_shiori_spring_data_init();
    mode_step_next();
}

void date_scene_shiori_winter(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl65 tbl;

    tbl = date_scene_shiori_winter_steps;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void date_scene_shiori_winter_audio_command_1(void) {
    audio_command(0x200);
    mode_step_next();
}

s32 date_scene_shiori_winter_shake_and_wait(void) {
    if (game_state.mode_step_timer.w == 0) {
        func_8007BFA8();
        audio_command(0xB1);
        audio_command(0x501);
        func_80047550();
    }
    if (game_state.mode_step_timer.w++ == 0x80) {
        mode_step_next();
    }
}

s32 date_scene_shiori_winter_speak_with_effect(void) {
    s32 unused; /* FAKE: extra local moves the spill slot of sel to sp+0x28; real source unknown. T-4090 */
    s32 sel;

    date_scene_speak();
    if ((u16) D_800CA154 == 2) {
        sel = game_state.mode_step_timer.w;
        event_face1();
        game_state.mode_step -= 1;
        game_state.mode_step_timer.w = sel + 1;
        if (sel == 1) {
            audio_command(0x201);
            audio_command(0x502);
        }
    }
}

void date_scene_shiori_winter_load_data(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x7FEB);
    date_scene_shiori_winter_data_init();
    mode_step_next();
}

void date_scene_shiori_winter_ferris_wheel_title(void) {
    strcpy(D_800CA1DC, "観覧車");
    mode_step_next();
}

void date_scene_shiori_summer(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl45 tbl;

    tbl = date_scene_shiori_summer_steps;
    if (D_80122D04 == 0 && dec_bg_brightness == 0x80) {
        hud_sprite_light_effect_draw();
    }
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void date_scene_shiori_summer_audio_command_1(void) {
    audio_command(0x502);
    mode_step_next();
}

void date_scene_shiori_summer_speak_with_face(void) {
    if ((u16) D_800CA154 == 5) {
        portrait_layout_set(1);
    } else if ((u16) D_800CA154 == 9) {
        portrait_layout_set(2);
    } else if ((u16) D_800CA154 == 0xB) {
        portrait_layout_set(get_g_zyotai_h(game_state.current_girl));
    }
    date_scene_speak();
}

void date_scene_shiori_summer_load_data(void) {
    cd_read_sectors(0x35, 0x801B0000, 0x8030);
    date_scene_shiori_summer_data_init();
    mode_step_next();
}
