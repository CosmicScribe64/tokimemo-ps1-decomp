/* scene_render_tables.c: ; src/ovl/SHOUGATU/8013F190.c:7-33 declares render and actor data islands. Original object 0x8013F190-0x80141780 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "common.h"
#include "game/display_records.h"
#include "game/game_state.h"
#include "game_enums.h"
#include "libapi.h"
#include "libgpu.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/cd.h"
#include "main/date.h"
#include "main/day_event.h"
#include "main/draw.h"
#include "main/event_log.h"
#include "main/game_mode.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/entry_ui_scenes_school_events.h"
#include "main/import_views/scenes.h"
#include "main/import_views/scenes_school_events.h"
#include "main/menu.h"
#include "main/message.h"
#include "main/param.h"
#include "main/portrait.h"
#include "main/prim.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "main/vram.h"
#include "psx_types.h"
#include "ovl/new_year_christmas.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_80146130);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_80146134);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_80146138);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_8014613C);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_80146140);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_80146144);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_80146148);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_8014614C);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_80146150);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_80146154);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", christmas_result_choice_index);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_8014615C);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_80146160);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_80146164);
u8 christmas_result_candidate_girls[4] = { 0 };
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_8014616C);
ShoColors6x2 D_801461D8 = { {
    { 0x00F870A0, 0x00F8A0C0 },
    { 0x0030C050, 0x0070E090 },
    { 0x005078F8, 0x0098C0F8 },
    { 0x00F8A828, 0x00F8D868 },
    { 0x00F88030, 0x00F8D078 },
    { 0x005850B0, 0x009098E8 },
} };
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_80146208);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_80146230);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_80146234);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_80146238);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_8014623C);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_80146240);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/new_year_christmas/scene_render_tables.data", D_80146244);

void christmas_result_actor_sequence_randomize(void);


void new_year_scene_actor_resource_addresses_init(void) {
    D_80146130 = (u8 *)0x801D6360;
    D_80146134 = (u8 *)0x801D6384;
    D_80146138 = (u8 *)0x801D6414;
    D_8014613C = *(s16 *)0x801D641C;
    D_80146140 = (u8 *)0x801B0000;
}

void new_year_scene_actor_resource_addresses_init_alt(void) {
    D_80146144 = 0x801DDBB4;
    D_80146148 = 0x801DDBB8;
    D_8014614C = 0x801DDC70;
    D_80146154 = 0x801DB000;
}

/* Fall-through return preserves the sibling callback schedule (T-24026). */
s32 new_year_scene_result_callback_update(void) {
    s32 idx; /* Index precedes the copied table (T-3330, T-20006). */
    FnTbl27 table;

    table = D_8014616C;
    idx = game_state.mode_step;
    table.f[idx](0x80);
    if (game_state.mode_step < 8) {
        ((Actor *)actor_pool)[96].brightness = bg_brightness;
        if (((Actor *)actor_pool)[96].unk_18 == 0 && ((Actor *)actor_pool)[96].unk_08 == 0) {
            if (++((Actor *)actor_pool)[96].sequence_slot >= 3) {
                ((Actor *)actor_pool)[96].sequence_slot = 0;
            }
        }
    }
    if (game_state.mode_step >= 0xA && D_80146160 != 0) {
        christmas_result_choice_triangles_draw();
    }
    if (game_state.mode_step >= 0xA && D_80146164 != 0) {
        christmas_result_actor_sequence_randomize();
    }
}

void new_year_year_selector_special_case_route(void) {
    if ((christmas_result_candidate_girls[christmas_result_choice_index] == 0xB) && (game_state.year == 0x60)) {
        new_year_current_talk_step();
        return;
    }
    mode_step_next();
}

s32 new_year_scene_audio_and_day_transition(void) {
    if (game_state.substep == 0) {
        text_system_reset();
        hud_day_bgm_load();
        game_state.substep += 1;
    }
    if (game_state.substep == 1) {
        if ((u32)game_state.mode_step_timer.w++ >= 0x400) {
            cd_command_reset();
            audio_bank_state_reset();
            game_state.substep = 0;
        }
        if (audio_bank_load_poll() == 1) {
            game_state.substep += 1;
        } else {
            return 0;
        }
    }
    sprite_tables_clear();
    return day_event_next(1);
}

void new_year_scene_result_actor_group_init(void) {
    s32 unused; /* FAKE: unused local restores observed implicit-copy stack offsets. T-13202. */
    s16 i;

    D_8014615C = 0;
    D_80146160 = 0;
    D_80146164 = 0;
    new_year_scene_actor_resource_addresses_init();
    i = 0;
    do {
        load_palette(D_80146140, (i + 0x10) & 0xFF, 1, 2, i);
        i++;
    } while (i < 9);
    actor_clear(0x60);
    ((Actor *)actor_pool)[96].unk_01 = 8;
    ((Actor *)actor_pool)[96].flags = 0x40;
    ((Actor *)actor_pool)[96].quad_flags = 0x01000000;
    ((Actor *)actor_pool)[96].draw_flags = 4;
    ((Actor *)actor_pool)[96].frame_table = D_80146134;
    ((Actor *)actor_pool)[96].sequence_table = D_80146138;
    ((Actor *)actor_pool)[96].tpage_table = D_80146130;
    ((Actor *)actor_pool)[96].unk_14 = D_8014613C;
    ((Actor *)actor_pool)[96].sequence_slot = 0;
    ((Actor *)actor_pool)[96].unk_18 = 0;
    ((Actor *)actor_pool)[96].blink_period = 1;
    ((Actor *)actor_pool)[96].unk_43 = 0;
    ((Actor *)actor_pool)[96].x = 0;
    ((Actor *)actor_pool)[96].y = 8;
    ((Actor *)actor_pool)[96].brightness = 0x80;
    ((Actor *)actor_pool)[96].unk_05 = 0x80;
    ((Actor *)actor_pool)[96].unk_04 = 0;
    ((Actor *)actor_pool)[97] = ((Actor *)actor_pool)[96];
    new_year_scene_actor_resource_addresses_init_alt();
    load_palette((u8 *) D_80146154, 0x1F, 1, 2, 0);
    ((Actor *)actor_pool)[98] = ((Actor *)actor_pool)[96];
    ((Actor *)actor_pool)[98].unk_01 = 0xB;
    ((Actor *)actor_pool)[98].unk_04 = 8;
    ((Actor *)actor_pool)[98].flags = 1;
    ((Actor *)actor_pool)[98].frame_table = D_80146148;
    ((Actor *)actor_pool)[98].sequence_table = D_8014614C;
    ((Actor *)actor_pool)[98].tpage_table = D_80146144;
    ((Actor *)actor_pool)[98].unk_14 = D_80146150;
    ((Actor *)actor_pool)[98].sequence_slot = (s16) (rand() % 3);
    ((Actor *)actor_pool)[98].unk_18 = 0;
    ((Actor *)actor_pool)[98].x = -0xA0;
    ((Actor *)actor_pool)[98].y = -0x78;
    ((Actor *)actor_pool)[98].unk_43 = 0x1F;
    ((Actor *)actor_pool)[101] = ((Actor *)actor_pool)[99] = ((Actor *)actor_pool)[100] = ((Actor *)actor_pool)[98];
    ((Actor *)actor_pool)[99].x += 0x48;
    ((Actor *)actor_pool)[99].sequence_slot = (s16) (rand() % 3);
    ((Actor *)actor_pool)[99].unk_18 = 4;
    ((Actor *)actor_pool)[100].x += 0x90;
    ((Actor *)actor_pool)[100].sequence_slot = (s16) (rand() % 3);
    ((Actor *)actor_pool)[100].unk_18 = 0xA;
    ((Actor *)actor_pool)[101].unk_01 = 0xA;
    ((Actor *)actor_pool)[101].flags = 0x40;
    ((Actor *)actor_pool)[101].sequence_slot = 0;
    christmas_selection_dialogue_group = 0;
    christmas_selection_dialogue_text_table = D_80145AF8;
    christmas_selection_dialogue_command_table = D_80145B2C;
    christmas_selection_dialogue_voice_table = D_80145B60;
    mode_step_next();
}

void new_year_scene_result_flags_update(void) {
    s32 temp = christmas_saved_scan_counter;

    if (temp != 0 && temp != 7 && temp != 9) {
        D_801206DB |= 0x80;
    }
    if (temp != 0 && temp != 7 && temp != 9) {
        D_8012071F |= 0x80;
    }
    if (temp != 0 && temp != 7 && temp != 9) {
        ((u8 *)&D_80120760)[3] |= 0x80;
    }
    D_801207A7 |= 0x80;
    D_80146164 = 1;
    mode_step_next();
}

void new_year_scene_choice_menu_init(void) {
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];

    a[0] = -0x96;
    a[1] = -0x48;
    a[2] = 6;
    a[3] = 0x54;
    b[0] = b[1] = b[2] = b[3] = -0x40;
    c[0] = c[1] = c[2] = c[3] = 0x40;
    d[0] = d[1] = d[2] = d[3] = 0x40;
    menu_set(0, 4, a, b, c, d);
    talk_menu_active = 1;
    mode_step_next();
}

/* Fall-through s32 return preserves the early-return register ($v0 = 0). T-24137. */
s32 new_year_scene_choice_update(void) {
    s16 i;
    GameState *g;

    g = &game_state;
    christmas_result_candidate_girls[0] = g->frame_count % 4;
    i = 1;
    while (1) {
        christmas_result_candidate_girls[1] = (g->frame_count * 2 + i) % 4;
        if (christmas_result_candidate_girls[0] != christmas_result_candidate_girls[1]) {
            break;
        }
        i++;
    }
    while (1) {
        christmas_result_candidate_girls[2] = (g->frame_count / 10 + i) % 4;
        if (christmas_result_candidate_girls[0] != christmas_result_candidate_girls[2] && christmas_result_candidate_girls[1] != christmas_result_candidate_girls[2]) {
            break;
        }
        i++;
    }
    while (1) {
        christmas_result_candidate_girls[3] = i % 4;
        if (christmas_result_candidate_girls[0] != christmas_result_candidate_girls[3] && christmas_result_candidate_girls[1] != christmas_result_candidate_girls[3] && christmas_result_candidate_girls[2] != christmas_result_candidate_girls[3]) {
            break;
        }
        i++;
    }
    for (i = 0; i < 4; i++) {
        if (christmas_result_candidate_girls[i] == 1) {
            christmas_result_candidate_girls[i] = 0xB;
        } else if (christmas_result_candidate_girls[i] == 2) {
            christmas_result_candidate_girls[i] = christmas_saved_girl;
        } else if (christmas_result_candidate_girls[i] == 3) {
            christmas_result_candidate_girls[i] = christmas_saved_scene_girl;
        }
    }
    menu_check(0, ((Actor *)actor_pool)[0].x, ((Actor *)actor_pool)[0].y, christmas_result_candidate_girls);
    menu_cursor_update(0);
    if ((g->pad_pressed & 0xF000) && ((g->menu_mode & 3) == 1 || (g->menu_mode & 3) == 3)) {
        audio_command(0x502);
    }
    if (g->pad_pressed & 0x200020) {
        if (check_end_k() != 0) {
            if (g->menu_items[0].hover != 0) {
                christmas_result_choice_index = 0;
            } else if (g->menu_items[2].hover != 0) {
                christmas_result_choice_index = 1;
            } else if (g->menu_items[4].hover != 0) {
                christmas_result_choice_index = 2;
            } else if (g->menu_items[6].hover != 0) {
                christmas_result_choice_index = 3;
            } else {
                return 0;
            }
            ((Actor *)actor_pool)[0].draw_flags &= 0xFF7F;
            talk_menu_active = 0;
            k_reset(1);
            ((Actor *)actor_pool)[97].draw_flags |= 0x80;
            ((Actor *)actor_pool)[97].unk_18 =
                christmas_result_candidate_girls[christmas_result_choice_index]
                + (g->year == 0x60) * 0xB
                + (g->year == 0x61) * 0x17
                - (christmas_result_candidate_girls[christmas_result_choice_index] == 0xB && g->year == 0x5F)
                + 1;
            /* FAKE: *(u8 *)& keeps christmas_result_choice_index unwidened ($a2); !(!=) and * 0 keep 0x5F in $a1 without CSE. T-24137. */
            ((Actor *)actor_pool)[97].x = *(u8 *)&christmas_result_choice_index * 0x4E - *(u8 *)&christmas_result_choice_index - 0x75;
            ((Actor *)actor_pool)[97].y = -0x3C;
            ((Actor *)actor_pool)[97].scale_y = 0;
            christmas_selection_dialogue_group += christmas_result_candidate_girls[christmas_result_choice_index] * 3 + g->year - 0x5F;
            D_80146160 = 1;
            if ((!(g->year != 0x5F) && christmas_result_candidate_girls[christmas_result_choice_index] == 0 && g->param_art.value >= 0x64)
                || ((christmas_result_candidate_girls[christmas_result_choice_index] == 0 & g->year == 0x5F) && g->clubs[5].practice_points >= 0x64)
                || ((christmas_result_candidate_girls[christmas_result_choice_index] == 1 & (g->year + (g->year * 0)) == 0x5F) && g->param_liberal_arts.value >= 0x46)
                || ((christmas_result_candidate_girls[christmas_result_choice_index] == 1 & g->year == 0x61) && g->param_liberal_arts.value >= 0x64)
                || ((christmas_result_candidate_girls[christmas_result_choice_index] == 2 & g->year == 0x60) && g->param_liberal_arts.value >= 0x64)) {
                christmas_selection_dialogue_line = 1;
            }
            mode_step_next();
        }
    }
}

void new_year_scene_background_read(void) {
    bg_read_sub2(0x41E4);
    mode_step_next();
}

void new_year_scene_result_actor_group_clear(void) {
    s16 i;

    for (i = 0; i < 6; i++) {
        ((Actor *)actor_pool)[i + 96].draw_flags = 0;
    }
    D_80146160 = 0;
    D_80146164 = 0;
    mode_step_next();
}

s32 christmas_result_girl_mode_route(void) {
    D_80122D2C = 0;
    if (christmas_saved_scan_counter == 0) {
        game_state.current_girl = GIRL_SHIORI;
        scene_work_selector = 0;
        talk_work = 7;
        mode_stage_set(6);
        game_state.scene_flags[6] += 1;
        return 0;
    }
    if (christmas_saved_scan_counter == 7) {
        game_state.current_girl = GIRL_MIRA;
        scene_work_selector = 7;
        talk_work = 7;
        mode_stage_set(7);
        game_state.scene_flags[52] += 1;
        return 0;
    }
    if (christmas_saved_scan_counter == 9) {
        game_state.current_girl = GIRL_MEGUMI;
        scene_work_selector = 9;
        talk_work = 7;
        mode_stage_set(8);
        game_state.scene_flags[66] += 1;
        return 0;
    }
    mode_step_next();
}

void christmas_result_affection_update(void) {
    if (christmas_result_candidate_girls[christmas_result_choice_index] == 0xB) {
        audio_command(0x200);
    } else {
        audio_command(0x201);
        game_state.girls[christmas_result_candidate_girls[christmas_result_choice_index]].affection += 1;
    }
    check_para_limit();
    mode_step_next();
}

void christmas_result_progress_threshold(void) {
    s16 v;

    v = D_801206B0 + 0x200;
    D_801206B0 = v;
    if (v >= 0x1000) {
        mode_step_next();
    }
}

void christmas_result_sector_read_4d(void) {
    cd_read_sectors(0x4D, 0x801B0000, 0xBDAC);
    mode_step_next();
}

void christmas_result_sector_read_e(void) {
    cd_read_sectors(0xE, 0x801D7000, 0xBDF9);
    mode_step_next();
}

void christmas_result_stress_update(void) {
    christmas_selection_dialogue_line = 0;
    if (christmas_result_candidate_girls[christmas_result_choice_index] == 0xB) {
        christmas_selection_dialogue_group = 0x27;
        game_state.param_stress.value += 0xA;
    } else {
        christmas_selection_dialogue_group = 0x26;
        game_state.param_stress.value -= 0xA;
    }
    check_para_limit();
    mode_step_next();
}

/* Fall-through return preserves the sibling callback schedule (T-24101). */
s32 christmas_result_choice_triangles_draw(void) {
    s32 unused; /* FAKE: unused local pads stack frame to 0xA0 bytes. T-24101. */
    s16 dy;
    s16 dx;
    ShoColors6x2 colors;
    ShoBytes3x12 sel;

    colors = D_801461D8;
    sel = D_80146208;
    dy = 0x1C - (((Actor *)actor_pool)[97].scale_y / 512) * 4;
    dx = 1;
    if (((Actor *)actor_pool)[97].scale_y == 0x1000) {
        dy = 0;
        dx = 0;
    }
    christmas_result_gouraud_triangle_submit(
        ((Actor *)actor_pool)[97].x - dx - 0x1C,
        ((Actor *)actor_pool)[97].y + dy - 0x1C,
        ((Actor *)actor_pool)[97].x - 1,
        ((Actor *)actor_pool)[97].y + dy - 0x1C,
        ((Actor *)actor_pool)[97].x - dx - 0x1C,
        ((Actor *)actor_pool)[97].y - 1,
        colors.v[sel.v[(game_state.year - 0x5F) * 12 + christmas_result_candidate_girls[christmas_result_choice_index]]][0],
        colors.v[sel.v[(game_state.year - 0x5F) * 12 + christmas_result_candidate_girls[christmas_result_choice_index]]][1],
        9, 0);
    christmas_result_gouraud_triangle_submit(
        ((Actor *)actor_pool)[97].x + dx + 0x1C,
        ((Actor *)actor_pool)[97].y - dy + 0x1C,
        ((Actor *)actor_pool)[97].x + dx + 0x1C,
        ((Actor *)actor_pool)[97].y,
        ((Actor *)actor_pool)[97].x,
        ((Actor *)actor_pool)[97].y - dy + 0x1C,
        colors.v[sel.v[(game_state.year - 0x5F) * 12 + christmas_result_candidate_girls[christmas_result_choice_index]]][0],
        colors.v[sel.v[(game_state.year - 0x5F) * 12 + christmas_result_candidate_girls[christmas_result_choice_index]]][1],
        9, 0);
    dtd_on(0xB);
}

/* Fall-through int return follows the matched DATE builder (T-24026). */
s32 christmas_result_gouraud_triangle_submit(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, u32 c0, u32 c1, s32 z, s32 trans) {
    GameState *g;
    POLY_G3 *poly;

    g = &game_state;
    poly = GetWorkBase(0x1C, draw_buffer);
    func_8009F004(poly);
    SetSemiTrans(poly, trans);
    /* FAKE: explicit colour masks and comma lines retain the matched sibling register schedule (T-20600, T-24026). */
    poly->r0 = (c0 >> 16) & 0xFF, poly->g0 = (c0 >> 8) & 0xFF, poly->b0 = c0 & 0xFF;
    poly->r1 = (c1 >> 16) & 0xFF, poly->g1 = (c1 >> 8) & 0xFF, poly->b1 = c1 & 0xFF;
    poly->r2 = (c1 >> 16) & 0xFF, poly->g2 = (c1 >> 8) & 0xFF, poly->b2 = c1 & 0xFF;
    poly->x0 = g->draw_origin_x[draw_buffer] + x0;
    poly->y0 = g->draw_origin_y[draw_buffer] + y0;
    poly->x1 = g->draw_origin_x[draw_buffer] + x1;
    poly->y1 = g->draw_origin_y[draw_buffer] + y1;
    poly->x2 = g->draw_origin_x[draw_buffer] + x2;
    poly->y2 = g->draw_origin_y[draw_buffer] + y2;
    AddPrim(&draw_ot_tags[(draw_buffer << 10) + z * 4], poly);
}

void christmas_result_actor_sequence_randomize(void) {
    s16 i;
    u8 *p;

    for (i = 0; i < 3; i++) {
        p = actor_pool + i * 0x44;
        if (*(s16 *)(p + 0x1A20) == 0xE && *(s16 *)(p + 0x1A10) == 9) {
            *(s16 *)(p + 0x1A1E) = rand() % 3;
        }
    }
}

void christmas_dialogue_resource_addresses_init(void) {
    D_80146230 = 0x801B21E0;
    D_80146234 = 0x801B21E4;
    D_80146238 = 0x801B2228;
    D_80146240 = 0x801B0000;
}

void christmas_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl31 tbl;
    u32 j;

    tbl = D_80146244;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
    j = game_state.mode_step;
    if (normal_date_bg_fadein == tbl.f[j] || normal_date_bg_fadeout == tbl.f[j]) {
        D_80120653 |= 0x80;
        D_80120657 = bg_brightness;
    }
}

void christmas_audio_bank_load_start(void) {
    audio_command(0xBF);
    audio_bank_load_start(1, 0xBF98, 0xBF79, D_800B36AC, D_800B36EC, D_800B372C);
    if (audio_bank_load_poll() == 1) {
        audio_command(0x200);
        mode_step_next();
    }
    mode_step_next();
}

void christmas_audio_bank_load_retry(void) {
    if (audio_bank_load_poll() == 1) {
        audio_command(0x200);
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void christmas_audio_bank_load_start_alt(void) {
    audio_command(0xBF);
    audio_bank_load_start(1, 0xC621, 0xC613, 0xD93B, 0xD8F0, 0xD8D0);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

void christmas_audio_bank_load_retry_alt(void) {
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void christmas_scene_sector_read(void) {
    cd_read_sectors(0xD, 0x801B0000, 0xBCBB);
    christmas_dialogue_resource_addresses_init();
    mode_step_next();
}

void christmas_scene_audio_command_0203(void) {
    audio_command(0x203);
    mode_step_next();
}

s32 christmas_megumi_meeting_condition(void) {
    if (game_state.girls[9].flags.f.met == 0 && (game_state.girls[9].tokimeki >= 0x14) && (game_state.frame_count & 1)) {
        christmas_unmet_megumi_branch = 1;
        game_state.girls[9].flags.f.met = 1;
        game_state.girls[9].affection = 0x32;
        christmas_selection_dialogue_group = 5;
        christmas_selection_dialogue_text_table = D_80145AF0;
        christmas_selection_dialogue_command_table = D_80145B24;
        christmas_selection_dialogue_voice_table = D_80145B58;
        game_state.current_girl = GIRL_MEGUMI;
        event_log_add(4, 1);
        talk_names_set(game_state.current_girl);
        mode_step_next();
    } else {
        game_state.mode_step += 0xB;
    }
}

void christmas_scene_actor_and_talk_init(void) {
    message_window_title_show(0);
    k_reset(1);
    christmas_dialogue_resource_addresses_init();
    load_palette(D_80146240, 0x11, 1, 2, 0);
    actor_clear(0x60);
    D_80120651 = 1;
    D_80120652 = 0;
    D_80120688 = 0x01000000;
    D_80120653 = 4;
    D_8012065C = (u8 *)D_80146234;
    D_80120660 = (u8 *)D_80146238;
    D_80120684 = (u8 *)D_80146230;
    D_80120664 = D_8014623C;
    D_80120666 = 0;
    D_80120668 = 0;
    D_80120656 = 1;
    D_80120693 = 0x11;
    D_80120676 = -0xA0;
    D_8012067A = -0x78;
    D_80120657 = 0;
    D_80120655 = 0;
    D_80120654 = 0;
    christmas_selection_dialogue_group = 0x13;
    christmas_selection_dialogue_text_table = D_80145AFC;
    christmas_selection_dialogue_command_table = D_80145B30;
    christmas_selection_dialogue_voice_table = D_80145B64;
    mode_step_next();
}

void christmas_scene_background_load(void) {
    bg_read_sub2(0x41DC);
    mode_step_next();
}

void christmas_scene_actor_state_set(void) {
    D_80120652 = 9;
    mode_step_next();
}

void christmas_current_girl_affection_set(void) {
    game_state.girls[game_state.current_girl].affection = 0x28;
    mode_step_next();
}

s32 christmas_girl_scene_handler_dispatch(void) {
    if (game_state.mode_step == 0) {
        game_state.girls[game_state.current_girl].affection = 0x46;
        talk_names_set(game_state.current_girl);
    }
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        game_mode_reset_to_title();
        break;
    case GIRL_MIO:
        christmas_mio_mode_step_dispatch();
        break;
    case GIRL_YUINA:
        christmas_yuina_mode_step_dispatch();
        break;
    case GIRL_AYAKO:
        christmas_ayako_mode_step_dispatch();
        break;
    case GIRL_SAKI:
        christmas_saki_mode_step_dispatch();
        break;
    case GIRL_YUKARI:
        christmas_yukari_mode_step_dispatch();
        break;
    case GIRL_NOZOMI:
        christmas_nozomi_mode_step_dispatch();
        break;
    case GIRL_MIRA:
        christmas_mira_mode_step_dispatch();
        break;
    case GIRL_YUKO:
        christmas_yuko_mode_step_dispatch();
        break;
    case GIRL_MEGUMI:
        christmas_megumi_mode_step_dispatch();
        break;
    case GIRL_YUMI:
        game_mode_reset_to_title();
        break;
    }
    ((Actor *)actor_pool)[96].brightness = bg_brightness;
    if (((Actor *)actor_pool)[96].unk_18 == 0 && ((Actor *)actor_pool)[96].unk_08 == 0) {
        if (++((Actor *)actor_pool)[96].sequence_slot >= 3) {
            ((Actor *)actor_pool)[96].sequence_slot = 0;
        }
    }
}

void christmas_mio_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl17 tbl;

    tbl = D_801462C0;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void christmas_scene_selector_set_three(void) {
    christmas_selection_dialogue_group = 3;
    mode_step_next();
}

void christmas_scene_audio_command_0506(void) {
    audio_command(0x506);
    mode_step_next();
}

void christmas_yuina_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl14 tbl;

    tbl = D_80146304;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void christmas_yuina_flag_and_selector_set(void) {
    game_state.girls[2].flags.f.b2 = 1;
    christmas_selection_dialogue_group = 3;
    mode_step_next();
}

void christmas_yuina_selector_conditional_advance(void) {
    if (((u32) game_state.girls[2].flags.b[2] >> 4) == 3) {
        christmas_selection_dialogue_group += 1;
    }
    mode_step_next();
}

void christmas_saki_selector_conditional_advance_alt(void) {
    if (((u32) game_state.girls[2].flags.b[2] >> 4) != 3) {
        christmas_selection_dialogue_group += 1;
    }
    mode_step_next();
}

void christmas_ayako_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl10 tbl;

    tbl = D_8014633C;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void christmas_ayako_selector_set_three(void) {
    christmas_selection_dialogue_group = 3;
    mode_step_next();
}

void christmas_saki_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl14 tbl;

    tbl = D_80146364;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void christmas_saki_audio_command_0501(void) {
    audio_command(0x501);
    mode_step_next();
}

void christmas_saki_selector_conditional_advance(void) {
    if (((u32) game_state.girls[4].flags.b[2] >> 4) == 7) {
        christmas_selection_dialogue_group += 3;
    }
    mode_step_next();
}

void christmas_saki_flag_and_selector_set(void) {
    game_state.girls[4].flags.f.b2 = 1;
    christmas_selection_dialogue_group = 3;
    mode_step_next();
}

void christmas_yukari_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl12 tbl;

    tbl = D_8014639C;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void christmas_nozomi_selector_set_three(void) {
    christmas_selection_dialogue_group = 3;
    mode_step_next();
}

void christmas_nozomi_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl17 tbl;

    tbl = D_801463CC;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void christmas_mira_flag_and_selector_set(void) {
    game_state.girls[6].flags.f.b2 = 1;
    christmas_selection_dialogue_group = 4;
    mode_step_next();
}

void christmas_mira_audio_command_0505(void) {
    audio_command(0x505);
    mode_step_next();
}

void christmas_mira_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl17 tbl;

    tbl = D_80146410;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void christmas_yuko_input_wait(void) {
    if (check_end_k() != 0) {
        k_reset(1);
        mode_step_next();
    }
}

void christmas_yuko_selector_set_three(void) {
    christmas_selection_dialogue_group = 3;
    mode_step_next();
}

void christmas_yuko_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl11 tbl;

    tbl = D_80146454;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void christmas_megumi_selector_set_three(void) {
    christmas_selection_dialogue_group = 3;
    mode_step_next();
}

void christmas_megumi_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl12 tbl;

    tbl = D_80146480;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void christmas_yumi_selector_set_three(void) {
    christmas_selection_dialogue_group = 3;
    mode_step_next();
}

void christmas_portrait_resource_read(void) {
    portrait_read_start(0x56C0);
    mode_step_next();
}

void christmas_saved_mode_state_restore(void) {
    mode_stage_set(game_state.saved_mode_stage);
    mode_step_set(game_state.saved_mode_step);
}
