/* text_assets_and_camera.c: src/ovl/TACO/801430B0.c:4: number/text drawing, texture uploads, model setup/release, camera and asset-load requests; body 5: prim_flash_color_wide(0x800000); 20: if (arg0 < 0) {. Original object 0x801430B0-0x80143E80 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

void shooter_flash_red(void) {
    prim_flash_color_wide(0x800000);
}

void shooter_draw_number(arg0, arg1, arg2, arg3)
s32 arg0;
s32 arg1;
s32 arg2;
s32 arg3;
{
    s32 n;
    s32 sign;
    s32 xoff;
    SPRT *sprite;
    u8 *uv;

    if (arg0 < 0) {
        n = -arg0;
        sign = -1;
    } else {
        n = arg0;
        sign = 1;
    }
    if (sign != -1 || !(game_state.frame_count & 4)) {
        xoff = 0;
        if (n >= 0) {
            do {
                sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
                func_8009F0A4(sprite);
                sprite->w = 0xC;
                sprite->h = 0x10;
                SetSemiTrans(sprite, 0);
                sprite->r0 = 0x80;
                sprite->g0 = 0x80;
                sprite->b0 = 0x80;
                sprite->x0 = game_state.draw_origin_x[draw_buffer] + arg1 - xoff;
                sprite->y0 = game_state.draw_origin_y[draw_buffer] + arg2;
                uv = &shooter_digit_uv[n % 10 * 2];
                sprite->u0 = uv[0] + 0x80;
                sprite->v0 = uv[1];
                sprite->clut = func_8009ED74(arg3 * 0x10, 0x1EF);
                AddPrim((draw_buffer << 10) + 4 + draw_ot_tags, (s32)sprite);
                if (n < 10) {
                    break;
                }
                xoff += 0xC;
                n = n / 10;
            } while (n >= 0);
        }
        dtd_on_tpage(0x340, 0x180, 1, 0, 0);
    }
}

/* The character is read as *s in every test (no local copy): uopt then keeps the load in $v1 and the `*s - 0x30` / `*s - 0x41` values in $a0, as the original does. T-16562 */
void shooter_draw_text(arg0, arg1, arg2, arg3)
u8 *arg0;
s32 arg1;
s32 arg2;
s32 arg3;
{
    SPRT *sprite;
    u8 *s;
    u8 idx;
    s32 x;

    s = arg0;
    x = arg1;
    if (*s != 0) {
        do {
            sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
            func_8009F0A4(sprite);
            SetSemiTrans(sprite, 0);
            sprite->r0 = 0x80;
            sprite->g0 = 0x80;
            sprite->b0 = 0x80;
            sprite->x0 = game_state.draw_origin_x[draw_buffer] + x;
            x += 0xC;
            sprite->y0 = game_state.draw_origin_y[draw_buffer] + arg2;
            if (*s >= 0x30 && *s < 0x3A) {
                idx = *s - 0x30;
                sprite->u0 = shooter_digit_uv[idx * 2] + 0x80;
                sprite->v0 = shooter_digit_uv[idx * 2 + 1];
            } else {
                idx = *s - 0x41;
                if (idx < 0x1A) {
                    sprite->u0 = shooter_letter_uv[idx * 2] + 0x80;
                    sprite->v0 = shooter_letter_uv[idx * 2 + 1];
                } else if (*s == 0x2E) {
                    sprite->u0 = shooter_glyph_period_u + 0x80;
                    sprite->v0 = shooter_glyph_period_v;
                } else if (*s == 0x2D) {
                    sprite->u0 = shooter_glyph_hyphen_u + 0x80;
                    sprite->v0 = shooter_glyph_hyphen_v;
                } else if (*s == 0x20) {
                    sprite->u0 = shooter_glyph_space_u + 0x80;
                    sprite->v0 = shooter_glyph_space_v;
                } else {
                    sprite->u0 = shooter_glyph_other_u + 0x80;
                    sprite->v0 = shooter_glyph_other_v;
                }
            }
            /* FAKE: w and h on one source line give the original's store order (h first) and the register for 0xC. T-16562 */
            sprite->w = 0xC; sprite->h = 0x10;
            sprite->clut = func_8009ED74(arg3 << 4, 0x1EF);
            AddPrim((draw_buffer << 10) + 4 + draw_ot_tags, (s32)sprite);
        } while (*++s != 0);
    }
    dtd_on_tpage(0x340, 0x180, 1, 0, 0);
}

void shooter_draw_dot(s32 arg0, s32 arg1, s32 arg2) {
    s32 pad; /* FAKE: unused local declared first puts p at sp+0x28 as in the original; real source unknown. T-6070 */
    u8 *p;

    p = GetWorkBase(0xC, draw_buffer);
    func_8009F0B8(p);
    SetSemiTrans(p, 0);
    /* FAKE: p[4] and p[5] on one source line give the original's store order 5, 4, 6 (line-based scheduling). T-6070 */
    p[4] = (arg2 >> 16) & 0xFF; p[5] = (arg2 >> 8) & 0xFF;
    p[6] = arg2 & 0xFF;
    *(s16 *)(p + 8) = game_state.draw_origin_x[draw_buffer] + arg0;
    *(s16 *)(p + 0xA) = game_state.draw_origin_y[draw_buffer] + arg1;
    AddPrim((draw_buffer << 10) + 0x3C + draw_ot_tags, p);
}

void shooter_menu_cursor_move(void) {
    game_state.menu_mode = MENU_MODE_A;
    if (game_state.pad_pressed & PAD_UP) {
        if (game_state.menu_cursor_0 != 0) {
            game_state.menu_cursor_0 -= 1;
        } else {
            game_state.menu_cursor_0 = game_state.menu_count_0 - 1;
        }
    }
    if (game_state.pad_pressed & PAD_DOWN) {
        if ((u32)game_state.menu_cursor_0 < (u32)(game_state.menu_count_0 - 1)) {
            game_state.menu_cursor_0 += 1;
            return;
        }
        game_state.menu_cursor_0 = 0;
    }
}

void shooter_snapshot_screen_copy(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = draw_buffer * 0xF0;
    rect.w = 0x200;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0x200, 0);
    func_8009C674(0);
}

void shooter_load_texture_page(s32 arg0, s32 arg1) {
    s32 x;
    s32 y;

    x = (arg1 & 0xF) << 6;
    y = (arg1 & 0x10) << 4;
    LoadSquare(x, y, 0x40, 0x80, arg0);
    LoadSquare(x + 0x40, y, 0x40, 0x80, arg0 + 0x4000);
    LoadSquare(x, y + 0x80, 0x40, 0x80, arg0 + 0x8000);
    LoadSquare(x + 0x40, y + 0x80, 0x40, 0x80, arg0 + 0xC000);
}

void shooter_load_texture_page_32(s32 arg0, s32 arg1) {
    s32 x;
    s32 y;

    x = (arg1 & 0xF) << 6;
    y = (arg1 & 0x10) << 4;
    LoadSquare(x, y, 0x20, 0x80, arg0);
    LoadSquare(x + 0x20, y, 0x20, 0x80, arg0 + 0x2000);
    LoadSquare(x, y + 0x80, 0x20, 0x80, arg0 + 0x4000);
    LoadSquare(x + 0x20, y + 0x80, 0x20, 0x80, arg0 + 0x6000);
}

void shooter_init_actor_model(s32 arg0, s32 arg1, u8 *arg2) {
    s32 pad[3]; /* FAKE: 12 bytes of unused locals above sp28 reproduce the original frame (0x38, sp28 at 0x28); real source unknown. T-4040 */
    s32 sp28;

    sp28 = arg0 + 4;
    func_8009B3C0(sp28);
    func_8009B430(sp28 + 8, arg2 + 4, arg1, sp28);
    *(u8 **)(arg2 + 8) = arg2 + 0x14;
    *(s32 *)(arg2 + 4) = 0;
}

void shooter_set_white_lights(void) {
    view_light_0 = 0;
    D_80122764 = 0;
    D_80122768 = -0x64;
    D_8012276C = 0xFF;
    D_8012276D = 0xFF;
    D_8012276E = 0xFF;
    func_8009AD70(0, &view_light_0);
    view_light_1 = 0;
    D_80122774 = 0x64;
    D_80122778 = -0x64;
    D_8012277C = 0xFF;
    D_8012277D = 0xFF;
    D_8012277E = 0xFF;
    func_8009AD70(1, &view_light_1);
    view_light_2 = -0x64;
    D_80122784 = 0;
    D_80122788 = -0x64;
    D_8012278C = 0xFF;
    D_8012278D = 0xFF;
    D_8012278E = 0xFF;
    func_8009AD70(2, &view_light_2);
    GsSetAmbient(0x800, 0x800, 0x800);
    func_8009B340(0);
}

void shooter_init_camera(void) {
    func_8009AD30(0x800);
    D_80122740 = 0;
    D_80122744 = 0;
    D_80122748 = 0x800;
    D_8012274C = 0;
    D_80122750 = 0;
    D_80122754 = 0;
    D_80122758 = 0;
    D_8012275C = 0;
    func_80099540(&D_80122740);
    func_8009AD50(-0x64);
    func_8009AD60(0x7FFF);
}

void shooter_release_all_actor_models(void) {
    s32 i;

    for (i = 2; i != 0xA0; i += 2) {
        view_model_record_init(i);
    }
}

void shooter_release_actor_model(s32 arg0) {
    view_model_record_init((arg0 * 2) + 2);
}

void shooter_process_load_request(void) {
    switch (shooter_load_request) {
    case 0: /* empty case of its own: the jump table starts at 0 and its entry 0 is not the default */
        break;
    case SHOOTER_LOAD_GAME_START_BANKS:
        cd_read_sectors_nowait(0x20, 0x80162000, 0xB69B);
        cd_read_sectors_nowait(0x20, 0x80172000, 0xB6BB);
        cd_read_sectors_nowait(8, 0x80182000, 0xB47E);
        shooter_load_request += 1;
        break;
    case SHOOTER_LOAD_SOUND_BANK_STAGE1:
        audio_bank_load_start(0, 0xBF43, 0xBF26, 0xD310, 0xD2C2, 0xD29D);
        shooter_load_request += 1;
        break;
    case SHOOTER_LOAD_BOSS1:
        cd_read_sectors_nowait(0x12, 0x8018D000, 0xB438);
        cd_read_sectors(0xC, 0x801E0000, 0xB44A);
        cd_read_sectors_nowait(0x20, 0x80162000, 0xB418);
        shooter_load_request += 1;
        break;
    case SHOOTER_LOAD_STAGE2:
        cd_read_sectors_nowait(0x20, 0x80162000, 0xB647);
        cd_read_sectors_nowait(0x20, 0x80172000, 0xB5A0);
        shooter_load_request += 1;
        break;
    case SHOOTER_LOAD_SOUND_BANK_STAGE2:
        audio_bank_load_start(0, 0xBF61, 0xBF45, 0xD310, 0xD2C2, 0xD29D);
        shooter_load_request += 1;
        break;
    case SHOOTER_LOAD_BOSS2:
        cd_read_sectors_nowait(7, 0x80196000, 0xB45B);
        cd_read_sectors(5, 0x801E0000, 0xB462);
        cd_read_sectors_nowait(0x20, 0x80162000, 0xB535);
        shooter_load_request += 1;
        break;
    case SHOOTER_LOAD_STAGE3:
        cd_read_sectors_nowait(0x20, 0x80162000, 0xB67B);
        cd_read_sectors_nowait(0x20, 0x80172000, 0xB5C1);
        shooter_load_request += 1;
        break;
    case SHOOTER_LOAD_SOUND_BANK_STAGE3:
        audio_bank_load_start(0, 0xBF77, 0xBF63, 0xD310, 0xD2C2, 0xD29D);
        shooter_load_request += 1;
        break;
    case SHOOTER_LOAD_BOSS3:
        cd_read_sectors_nowait(0xA, 0x801A7000, 0xB515);
        cd_read_sectors(0x15, 0x801E0000, 0xB51F);
        cd_read_sectors_nowait(0x20, 0x80162000, 0xB5FC);
        cd_read_sectors_nowait(0x20, 0x80172000, 0xB61D);
        shooter_load_request += 1;
        break;
    case SHOOTER_LOAD_SOUND_BANK_RESULT:
        audio_bank_load_start(0, 0xC56D, 0xC54D, 0xD310, 0xD2C2, 0xD29D);
        shooter_load_request += 1;
        break;
    default:
        if (cd_queue_idle() == 1) {
            shooter_load_request = 0;
        }
        break;
    }
}

