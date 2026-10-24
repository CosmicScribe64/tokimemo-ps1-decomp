#define MAIN_API_OVERRIDE_dialogue_work /* matched as u32 (main_api.h: s32), switch selector in $v1 (T-6010) */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 dialogue_work;

typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
extern FnTbl22 D_80146F34;

typedef struct {
    void (*f[46])();
} FnTbl46; /* size 0xB8 */
extern FnTbl46 D_80146E7C;

void func_8013DF70(void) {
    D_80146E50 = 0x801D22F4;
    D_80146E54 = 0x801D22F8;
    D_80146E58 = 0x801D2318;
    D_80146E5C = *(s16 *)0x801D232C;
    D_80146E60 = 0x801B0000;
    D_80146394 = 0x801D2000;
    D_80146E64 = 0x801B2000;
    D_80146E68 = 0x801B6000;
    D_80146E6C = 0x801BA000;
    D_80146E70 = 0x801BE000;
    D_80146E74 = 0x801C2000;
    D_80146E78 = 0x801C6000;
    D_801463B0 = 0x801CE000;
}

void func_8013E040(void) {
    switch (dialogue_work) {
    case 1:
        func_8013E0A4();
        return;
    case 2:
        func_8013E0E0();
        return;
    default:
        game_reset_to_title();
        return;
    }
}

void func_8013E0A4(void) {
    if (game_state.stage == 0) {
        func_8013E11C();
        return;
    }
    game_reset_to_title();
}

void func_8013E0E0(void) {
    if (game_state.stage == 0) {
        func_8013E4F0();
        return;
    }
    game_reset_to_title();
}

void func_8013E11C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl46 tbl;

    tbl = D_80146E7C;
    idx = game_state.step;
    tbl.f[idx](0x80);
    func_8013E950();
}

void func_8013E1A0(void) {
    if (get_h_yuukou(GIRL_MEGUMI) >= 0x50U) {
        game_state.girls[9].tokimeki += 1;
        game_state.girls[9].affection += 2;
    } else {
        game_state.girls[9].flags.f.b14 = 1;
        game_state.girls[9].affection += 1;
    }
    check_para_limit();
    main_step_next();
}

void func_8013E22C(void) {
    s16 i;
    SpriteQuad *p;

    game_state.tpage_buf[5] = 1;
    LoadSquare(0x140, 0, 0x40, 0x80, D_801463B0);
    func_8013DF70();
    load_palette(D_80146394, 0x12, 1, 3, 0);
    for (i = 4; i < 0xE; i++) {
        p = &sprite_quads[i];
        p->flags = 0xC1000000;
        p->x = rand() % 84 - 0x9F;
        p->y = rand() % 130 - 0x5F;
        p->w = 8;
        p->h = 0x18;
        p->unk_0C = 5;
        p->unk_0E = 0;
        p->unk_0F = 0;
        p->unk_10 = 0;
        p->unk_12 = 0x1F2;
        p->tint_r = 0;
        p->tint_g = 0;
        p->tint_b = 0;
        p->anchor_x = 0;
        p->anchor_y = 0;
        p->unk_1C = 0x1000;
        p->unk_1E = 0x1000;
        p->unk_20 = 0;
        game_state.quad_layer[i] = 0x8C;
        D_801463E0[i - 4] = 0;
        D_80146400[i - 4] = 0;
    }
    main_step_next();
}

void func_8013E410(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x8A88);
    func_8013DF70();
    main_step_next();
}

void func_8013E448(void) {
    func_8013DF70();
    load_palette(D_80146E60, 0x11, 1, 2, 0);
    event_still_show(D_80146E64, D_80146E68, D_80146E6C, D_80146E70, D_80146E74, D_80146E78);
    event_anim_show(D_80146E54, D_80146E58, D_80146E50, D_80146E5C);
    main_step_next();
}

void func_8013E4F0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl22 tbl;

    tbl = D_80146F34;
    idx = game_state.step;
    tbl.f[idx](0x80);
}

s32 func_8013E56C(void) {
    func_80138AF8();
    if (game_state.step_timer.w++ == 1) {
        if ((u16) D_800CA154 == 9) {
            bustup_layout_set(0);
        }
    }
}

void func_8013E5C8(void) {
    if (get_h_yuukou(GIRL_MEGUMI) < 0x50U) {
        D_800CA150 = (u16) D_800CA150 + 2;
    }
    main_step_next();
}

void func_8013E60C(void) {
    if (get_h_yuukou(GIRL_MEGUMI) < 0x50U) {
        game_state.step += 0xE;
    }
    main_step_next();
}

void func_8013E650(void) {
    if (get_h_yuukou(GIRL_MEGUMI) >= 0x50U) {
        main_step_next();
        main_step_next();
        main_step_next();
    }
    main_step_next();
}

void func_8013E69C(void) {
    s16 i;
    SpriteQuad *records;
    s32 before;
    s32 after;

    before = game_state.step;
    func_800853FC();
    records = (SpriteQuad *)sprite_quads;
    for (i = 4; i < 19; i++) {
        if (dec_bg_brightness < 112) {
            records[i].tint_r = records[i].tint_g = records[i].tint_b = dec_bg_brightness;
        }
    }
    after = game_state.step;
    if (before != after) {
        for (i = 4; i < 19; i++) {
            records[i].flags |= 0x80000000;
        }
    }
}

void func_8013E760(void) {
    s16 i;
    SpriteQuad *records;

    if (game_state.step_timer.w == 0) {
        records = (SpriteQuad *)sprite_quads;
        for (i = 4; i < 19; i++) {
            records[i].flags &= 0x7FFFFFFF;
        }
    }
    func_80085368();
    records = (SpriteQuad *)sprite_quads;
    for (i = 4; i < 19; i++) {
        if (dec_bg_brightness < 113) {
            records[i].tint_r = records[i].tint_g = records[i].tint_b = dec_bg_brightness;
        }
    }
}

void func_8013E828(void) {
    game_state.current_girl = GIRL_MEGUMI;
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
    dialogue_names_set(game_state.current_girl);
    dialogue_clear();
    func_80137990();
    D_80146274 = D_80145F98;
    D_80146278 = D_801460D4;
    D_8014627C = D_80146210;
    strcpy(D_800CA19C, "教室");
    D_800CA368 = 0x3C;
    main_step_next();
}

void func_8013E950(void) {
    s16 i;

    for (i = 4; i < 0xE; i++) {
        sprite_quads[i].y += 0xA;
        if (((rand() & 3) == 0 &&
             ((sprite_quads[i].y >= -0x52 && sprite_quads[i].y < -0x39 && sprite_quads[i].x < -0x57) ||
              (sprite_quads[i].y >= -0x39 && sprite_quads[i].y < -0x1A && sprite_quads[i].x < -0x77) ||
              (sprite_quads[i].y >= -0xB && sprite_quads[i].y < 3 && sprite_quads[i].x >= -0x8D && sprite_quads[i].x < -0x6F) ||
              (sprite_quads[i].y >= 5 && sprite_quads[i].x >= -0x7F && sprite_quads[i].x < -0x5C)) &&
             D_801463E0[i - 4] == 0 && D_80146400[i - 4] == 0) ||
            D_801463E0[i - 4] != 0 || D_80146400[i - 4] != 0) {
            sprite_quads[i].y -= 0xA;
            if (D_801463E0[i - 4] == 0) {
                if (D_80146400[i - 4] == 0) {
                    if (sprite_quads[i].y < -0x44) {
                        sprite_quads[i].unk_0F += 8;
                    }
                    sprite_quads[i].x -= 0xC;
                    sprite_quads[i].y += 0x10;
                    /* FAKE: signed byte/halfword views on SpriteQuad unk_0E/unk_0A share constant registers $s3 (8) and $s5 (0x18) with unk_08, matching func_80139E1C. T-24085 */
                    *(s8 *)&sprite_quads[i].unk_0E = 8;
                    sprite_quads[i].w = 0x18;
                    *(s16 *)&sprite_quads[i].h = 8;
                    D_80146400[i - 4] += 1;
                } else {
                    D_80146400[i - 4] = 0;
                    D_801463E0[i - 4] += 1;
                }
            } else if (D_801463E0[i - 4] == 4) {
                sprite_quads[i].unk_0E = 0;
                sprite_quads[i].unk_0F = 0;
                sprite_quads[i].w = 8;
                *(s16 *)&sprite_quads[i].h = 0x18;
                /* FAKE: same-line stores place mfhi after the zero stores as in the original. T-24085 */
                sprite_quads[i].x = rand() % 84 - 0x9F; sprite_quads[i].y = -0x78; D_80146400[i - 4] = 0; D_801463E0[i - 4] = 0;
            } else if (D_801463E0[i - 4] == 5) {
                if (D_80146400[i - 4] == 0) {
                    sprite_quads[i].unk_0E -= 0x18;
                    D_80146400[i - 4] += 1;
                } else {
                    D_80146400[i - 4] = 0;
                    D_801463E0[i - 4] += 1;
                }
            } else {
                if (D_80146400[i - 4] == 0) {
                    sprite_quads[i].unk_0E += 0x18;
                    D_80146400[i - 4] += 1;
                } else {
                    D_80146400[i - 4] = 0;
                    D_801463E0[i - 4] += 1;
                }
            }
        } else if (sprite_quads[i].y >= 0x19 && D_801463E0[i - 4] == 0 && D_80146400[i - 4] == 0) {
            /* FAKE: same-line store places mfhi after unk_06 = -0x78 as in the original. T-24085 */
            sprite_quads[i].x = rand() % 84 - 0x9F; sprite_quads[i].y = -0x78;
        }
    }
}

void func_8013ECA8(void) {
    game_state.current_girl = GIRL_MEGUMI;
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
    dialogue_names_set(game_state.current_girl);
    dialogue_clear();
    func_80137990();
    D_80146274 = D_80145F9C;
    D_80146278 = D_801460D8;
    D_8014627C = D_80146214;
    D_800CA368 = 0x3D;
    main_step_next();
}

void func_8013EDC4(void) {
    bg_read_sub2(0x4068);
    main_step_next();
}

void func_8013EDEC(void) {
    bg_read_sub2(0x46B2);
    main_step_next();
}

void func_8013EE14(void) {
    bg_read_sub2(0x403B);
    main_step_next();
}
