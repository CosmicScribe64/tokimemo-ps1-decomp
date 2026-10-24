/* album_playback.c: ; src/ovl/ETC/8013BEF0.c:24-31 begins the album playback stage handlers. Original object 0x8013BEF0-0x8013F980 (config/objects/ETC.txt). */
#define MAIN_API_OVERRIDE_memcpy /* original called it undeclared (implicit int); T-24706 */
#include "common.h"
#include "psx_types.h"
#include "ovl/ETC.h"

extern s32 memcpy();

typedef struct {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ s16 x1;
    /* 0x0E */ s16 y1;
    /* 0x10 */ s16 x2;
    /* 0x12 */ s16 y2;
    /* 0x14 */ s16 x3;
    /* 0x16 */ s16 y3;
} EtcPolyF4; /* size 0x18 */

void func_8013BEF0(void) {
    D_80150030 = (u8 *)0x8019DCFC;
    D_80150034 = (u8 *)0x8019DD00;
    D_80150038 = (u8 *)0x8019DDC0;
    D_8015003C = *(s16 *)0x8019DDF8;
}

void func_8013BF34(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8013BFFC();
        return;
    case 1:
        func_8013C330(2, 0);
        return;
    case 2:
        func_8013C330(3, 1);
        return;
    case 3:
        func_8013C330(4, 2);
        return;
    case 4:
        func_8013C700();
        return;
    case 5:
        func_8013C8EC();
        return;
    case 6:
        func_8013CC74();
        return;
    case 7:
        func_8013CCE4();
        /* fallthrough */
    default:
        return;
    }
}

/* T-24069: retain the earlier caller's K&R implicit-int view. */
s32 func_8013BFFC(void) {
    GameState *g = &game_state;
    /* FAKE: join step/status loads through one word temporary to preserve
     * the original v1 allocation; original local declarations unknown. T-24069. */
    s32 value;
    switch (g->substep) {
    case 0:
        func_8013FC08();
        bg_layers = 0x80000000;
        if (D_80150068 > 0) {
            D_80150070 = 0;
        }
        if ((value = D_80150070) == -1) {
            D_801500D4 = 0x800;
            func_8013FA7C();
            mode_stage_next();
        } else {
            g->substep = (value = g->substep) + 1;
        }
        break;
    case 1:
        if (D_80150068 > 0) {
            func_8013E89C(1);
            if ((value = D_80150070) >= 3) {
                if (D_80150068 == 1) {
                    D_80150070 = 0;
                } else {
                    D_80150068 = 0;
                    D_80150070 = 0;
                }
                g->substep = (value = g->substep) + 1;
            } else if ((value = D_80150070) == -1) {
                D_801500D4 = 0x800;
                func_8013FA7C();
                mode_step_set(5);
            }
        } else {
            g->substep = (value = g->substep) + 1;
        }
        break;
    case 2:
        if (func_8013F3F8() == 1) {
            cd_read_sectors(0xEU, 0x80197000, 0xA9D0);
            g->substep = (value = g->substep) + 1;
        } else {
            g->substep = (value = g->substep) + 1;
        }
        break;
    case 3:
        if (func_8013F3F8() == 0) {
            if (D_8014E0C0 == 1) {
                dec_bg_show_set(1, dec_bg_decode_to_vram(1));
                func_8013DEA0(1);
            }
            func_8013ECDC();
            func_8013EDF8();
            func_8013E364();
            g->substep = (value = g->substep) + 1;
        } else if ((func_8013F3F8() == 1) && (cd_read_sync_status() & 1)) {
            if (D_8014E0C0 == 1) {
                dec_bg_show_set(1, dec_bg_decode_to_vram(1));
                func_8013DEA0(1);
            }
            func_8013ECDC();
            if (func_8013F3F8() == 1) {
                func_8013F498();
            }
            func_8013EDF8();
            func_8013E364();
            g->substep = (value = g->substep) + 1;
        }
        break;
    case 4:
        func_8013D264();
        func_8013D2C0();
        func_8013EE28();
        func_801328A4(0x28, 0x3C);
        if (D_80150064 < 0xFF) {
            D_80150080 = D_80150064;
            D_80150064 += 1;
        }
        if ((D_801500D8 == 0) || (D_801500D8 == 2)) {
            func_8013EEFC();
        }
        mode_step_next();
        break;
    }
    func_8013D32C();
}


/* T-14501: retain the earlier callers' K&R implicit-int view. */
s32 func_8013C330(s32 arg0, s32 arg1) {
    switch (game_state.substep) {
    case 0:
        func_80132C3C();
        break;
    case 1:
        func_8013D570(0, 0);
        func_8013E89C(arg0);
        if (D_80150070 >= 3) {
            game_state.substep += 1;
            D_80150070 = 0;
        } else if (D_80150070 == -1) {
            game_state.substep += 1;
        }
        break;
    case 2:
        func_8013D570(0, 0);
        if ((game_state.current_girl == GIRL_REI) || (game_state.current_girl == GIRL_MIHARU)) {
            func_8013F054();
        }
        if (((D_8015006C == 0) || (D_8015006C >= 3)) && ((game_state.pad_pressed & PAD_CIRCLE) || (D_801500D8 == 0) || (D_801500D8 == 1))) {
            game_state.substep += 1;
            D_8015006C = 0;
        }
        break;
    case 3:
        func_8013D570(game_state.mode_step_timer.u * 2, arg1);
        if ((u32) game_state.mode_step_timer.u < 0x28U) {
            game_state.mode_step_timer.u += 1;
        } else {
            game_state.substep += 1;
        }
        break;
    case 4:
        func_8013FC08();
        if (D_80150070 == -1) {
            D_801500D4 = 0x800;
            func_8013FA7C();
            mode_step_set(5);
        } else {
            game_state.substep += 1;
        }
        break;
    case 5:
        if (func_8013F3F8() == 1) {
            cd_read_sectors(0xEU, 0x80197000, 0xA9D0);
            game_state.substep += 1;
        } else {
            game_state.substep += 1;
        }
        break;
    case 6:
        if (func_8013F3F8() == 0) {
            if (D_8014E0C0 == 1) {
                dec_bg_show_set(1, dec_bg_decode_to_vram(1));
                func_8013DEA0(1);
            }
            func_8013ECDC();
            func_8013EDF8();
            func_8013E364();
            game_state.substep += 1;
        }
        if ((func_8013F3F8() == 1) && (cd_read_sync_status() & 1)) {
            if (D_8014E0C0 == 1) {
                dec_bg_show_set(1, dec_bg_decode_to_vram(1));
                func_8013DEA0(1);
            }
            func_8013ECDC();
            if (func_8013F3F8() == 1) {
                func_8013F498();
            }
            func_8013EDF8();
            func_8013E364();
            game_state.substep += 1;
        }
        break;
    case 7:
        func_8013D264();
        func_8013D2C0();
        func_8013EE28();
        func_801328A4(0x28, 0x3C);
        if (D_80150064 < 0xFF) {
            D_80150080 = D_80150064;
            D_80150064 += 1;
        }
        if ((D_801500D8 == 0) || (D_801500D8 == 2)) {
            func_8013EEFC();
        }
        mode_step_next();
        break;
    }
    func_8013D32C();
}

s32 func_8013C700(void) {
    switch (game_state.substep) {
    case 0:
        func_8013D32C();
        func_80132C3C();
    default:
        break;
    case 1:
        func_8013D32C();
        func_8013D570(0, 0);
        func_8013E89C(5);
        if (D_80150070 >= 3) {
            D_80150070 = 0;
            game_state.substep += 1;
        } else if (D_80150070 == -1) {
            game_state.substep += 1;
        }
        break;
    case 2:
        func_8013D32C();
        func_8013D570(0, 0);
        if ((game_state.current_girl == GIRL_REI) || (game_state.current_girl == GIRL_MIHARU)) {
            func_8013F054();
        }
        if (((D_8015006C == 0) || (D_8015006C >= 3)) && ((game_state.pad_pressed & PAD_CIRCLE) || (D_801500D8 == 0) || (D_801500D8 == 1))) {
            D_8015006C = 0;
            game_state.substep += 1;
        }
        break;
    case 3:
        func_8013D32C();
        func_8013D570(game_state.mode_step_timer.u * 2, 3);
        if ((u32) game_state.mode_step_timer.u < 0x28U) {
            game_state.mode_step_timer.u += 1;
        } else {
            game_state.substep += 1;
        }
        break;
    case 4:
        func_8013FC08();
        D_801500D4 = 0x800;
        func_8013FA7C();
        mode_step_next();
        break;
    }
}

/* FAKE: retain the s32 fallthrough matching view; original return type is unproven,
 * and the sole caller ignores the result. T-24075. */
s32 func_8013C8EC(void) {

    if ((game_state.current_girl == GIRL_REI) || (game_state.current_girl == GIRL_MIHARU)) {
        mode_step_next();
    } else {
        switch (game_state.substep) {
        case 0:
            if (game_state.current_girl == GIRL_SHIORI) {
                do {
                    D_801500C0 = rand() % D_8014E0A4[game_state.current_girl];
                } while ((D_801500C0 == D_80150078) || (D_801500C0 == 20) || (D_801500C0 == 0));
            } else if (game_state.current_girl == GIRL_YUINA) {
                do {
                    D_801500C0 = rand() % D_8014E0A4[game_state.current_girl];
                } while ((D_801500C0 == D_80150078) || (D_801500C0 == 8));
            } else if (game_state.current_girl == GIRL_AYAKO) {
                do {
                    D_801500C0 = rand() % D_8014E0A4[game_state.current_girl];
                } while ((D_801500C0 == D_80150078) || (D_801500C0 == 9));
            } else if (game_state.current_girl == GIRL_SAKI) {
                do {
                    D_801500C0 = rand() % D_8014E0A4[game_state.current_girl];
                } while ((D_801500C0 == D_80150078) || (D_801500C0 == 13));
            } else if (game_state.current_girl == GIRL_MEGUMI) {
                do {
                    D_801500C0 = rand() % D_8014E0A4[game_state.current_girl];
                } while ((D_801500C0 == D_80150078) || (D_801500C0 == 0));
            } else {
                do {
                    D_801500C0 = rand() % D_8014E0A4[game_state.current_girl];
                } while ((D_801500C0 == D_80150078));
            }
            D_80150078 = D_801500C0;
            cd_audio_request_set(D_801500C4[D_801500C0 * 2], D_801500C4[D_801500C0 * 2 + 1], game_state.current_girl);
            audio_command(0x300);
            game_state.substep += 1;
            break;
        case 1:
            if (cd_audio_state() & 4) {
                game_state.substep += 1;
            }
            break;
        case 2:
            if (cd_audio_state() & 2) {
                mode_step_next();
            }
            break;
        }
    }
}

void func_8013CC74(void) {
    if ((D_80150054 == 0 && D_80150058 == 0 && D_8015005C == 0) || D_80150064 == 0xFF) {
        mode_stage_next();
        return;
    }
    mode_step_next();
}

s32 func_8013CCE4(void) {
    s32 pad; /* FAKE: unused slot, the original frame has it (real source unknown). T-22019 */
    s32 buf[6];
    u32 i;

    switch (game_state.substep) {
    case 0:
        if (D_80150068 > 0) {
            cd_read_sectors(0x69U, 0x801A0000, 0xB389);
            game_state.substep += 1;
        } else {
            game_state.substep += 1;
        }
        break;
    case 1:
        if (D_80150068 == 0) {
            game_state.substep += 1;
        } else if (D_80150068 > 0) {
            if (cd_read_sync_status() & 1) {
                memcpy((void *)0x801E2000, 0x801A0600, 0x18000);
                if (D_80150068 == 1) {
                    D_80150068 = 2;
                }
                game_state.substep += 1;
            }
        }
        break;
    case 2:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            /* FAKE: the two-literal line and the one-line loop keep as1's
             * scheduling; the byte chain shares one li, as the original. T-24706 */
            buf[0] = 0x801E2000; buf[1] = 0x801E6000; for (i = 2; i < 6; i++) buf[i] = 0x801E2000 + i * 0x4000;
            func_8004A1A0(buf, 0, 6, 3, 2, 0x1FF, -0xA0, -0x7D, 0xD, 5);
            bg_layers = 0x01000000;
            (&D_801220E0)[0] = (&D_801220E0)[1] = (&D_801220E0)[2] = 0x80;
            game_state.screen_brightness = 0x80;
            game_state.substep += 1;
        }
        break;
    case 3:
        if (D_801500D4 >= 0xC00) {
            mode_step_set(0);
        } else {
            D_801500D4 += 8;
            func_8013FA7C();
        }
        break;
    }
}

/* FAKE: retain the s32 fallthrough matching view so v0 stays live at the
 * epilogue and as1 hoists li t9,0xC00 instead of lui v0,0x8000 in case 5;
 * dummy entry read of unk_F5F seeds cvt_pass.py entry_locations so uopt
 * promotes unk_F5F to v1 in case 0. T-24129. */
s32 func_8013CF2C(void) {
    u8 unused;

    unused = game_state.current_girl;
    if ((D_80150064 == 0) || (D_801500E0 == 1)) {
        *(u8 *)&game_state.mode_stage_scratch = 5;
    }
    switch (*(u8 *)&game_state.mode_stage_scratch) {
    case 0:
        if ((u32) game_state.mode_stage_timer >= 0x79U) {
            if (game_state.current_girl == GIRL_SHIORI) {
                func_8013F320(0x14);
                if (D_8015006C >= 3) {
                    *(u8 *)&game_state.mode_stage_scratch += 1;
                    D_8015006C = 0;
                }
            } else if (game_state.current_girl == GIRL_YUINA) {
                func_8013F320(8);
                if (D_8015006C == 2) {
                    game_state.mode_stage_timer = 0;
                }
                if (D_8015006C >= 3) {
                    if ((u32) game_state.mode_stage_timer >= 0x3DU) {
                        *(u8 *)&game_state.mode_stage_scratch += 1;
                        D_8015006C = 0;
                    } else {
                        game_state.mode_stage_timer += 1;
                    }
                }
            } else if (game_state.current_girl == GIRL_SAKI) {
                func_8013F320(0xD);
                if (D_8015006C >= 3) {
                    *(u8 *)&game_state.mode_stage_scratch += 1;
                    D_8015006C = 0;
                }
            } else if (game_state.current_girl == GIRL_REI) {
                func_8013F320(7);
                if (D_8015006C >= 3) {
                    *(u8 *)&game_state.mode_stage_scratch += 1;
                    D_8015006C = 0;
                }
            } else {
                *(u8 *)&game_state.mode_stage_scratch += 1;
            }
        }
        break;
    case 1:
        cd_audio_request_set(D_8014DBF8[game_state.current_girl * 2], D_8014DBF8[game_state.current_girl * 2 + 1], game_state.current_girl);
        audio_command(0x300);
        *(u8 *)&game_state.mode_stage_scratch += 1;
        break;
    case 2:
        if (cd_audio_state() & 4) {
            *(u8 *)&game_state.mode_stage_scratch += 1;
        }
        break;
    case 3:
        if (cd_audio_state() & 2) {
            game_state.mode_stage_timer = 0;
            *(u8 *)&game_state.mode_stage_scratch += 1;
        }
        break;
    case 4:
        if ((u32) game_state.mode_stage_timer >= 0x79U) {
            game_state.mode_stage_timer = 0;
            *(u8 *)&game_state.mode_stage_scratch += 1;
        }
        break;
    case 5:
        func_8013E1DC(game_state.mode_stage_timer);
        if ((u32) game_state.mode_stage_timer >= 0x12DU) {
            D_801500E0 = 0;
            D_80150070 = 0;
            D_80150064 = 0;
            D_801500D8 = 0;
            D_801500DC = 0;
            *(s32 *)&bg_layers = 0x80000000;
            D_801220F4 = 0x80000000;
            D_801500D4 = 0xC00;
            func_8013FA7C();
            mode_stage_set(0xFF);
        }
        break;
    }
}

void func_8013D264(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = draw_buffer * 0xF0;
    rect.w = 0x140;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0x140, 0x100);
    func_8009C674(0);
}

void func_8013D2C0(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = (1 - draw_buffer) * 0xF0;
    rect.w = 0x140;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0, draw_buffer * 0xF0);
    func_8009C674(0);
}

s32 func_8013D32C(void) {
    s32 i;
    POLY_FT4 *poly[2];

    for (i = 0; i < 2; i++) {
        poly[i] = GetWorkBase(0x28, draw_buffer);
        func_8009F040(poly[i]);
        SetSemiTrans(poly[i], 0);
        SetShadeTex(poly[i], 0);
        poly[i]->tpage = func_8009ECB0(2, 0, 0x280 + i * 0x80, 0);
        poly[i]->x0 = i * 0xA0;
        poly[i]->y0 = game_state.screen_center_y[draw_buffer] - 0x78;
        poly[i]->x1 = (i + 1) * 0xA0;
        poly[i]->y1 = game_state.screen_center_y[draw_buffer] - 0x78;
        poly[i]->x2 = i * 0xA0;
        poly[i]->y2 = game_state.screen_center_y[draw_buffer] + 0x78;
        poly[i]->x3 = (i + 1) * 0xA0;
        poly[i]->y3 = game_state.screen_center_y[draw_buffer] + 0x78;
        poly[i]->u0 = i * 0x20;
        poly[i]->v0 = 0;
        poly[i]->u1 = (i + 5) * 0x20;
        poly[i]->v1 = 0;
        poly[i]->u2 = i * 0x20;
        poly[i]->v2 = 0xF0;
        poly[i]->u3 = (i + 5) * 0x20;
        poly[i]->v3 = 0xF0;
        poly[i]->r0 = 0x80;
        poly[i]->g0 = 0x80;
        poly[i]->b0 = 0x80;
        AddPrim(draw_buffer * 0x400 + 0x38 + D_800E8CA0, poly[i]);
    }
}

/* FAKE: one source line per vertex (x and y, u and v, the colour). as1 schedules by source line, and
 * case 0 needs x2, h and y2 on one line; the original layout is plausible but unproven. T-25113 */
s32 func_8013D570(s32 arg0, s32 arg1) {
    s32 i;
    POLY_FT4 *poly[2];
    s32 h;

    for (i = 0; i < 2; i++) {
        poly[i] = GetWorkBase(0x28, draw_buffer);
        func_8009F040(poly[i]);
        SetSemiTrans(poly[i], 0);
        SetShadeTex(poly[i], 1);
        poly[i]->tpage = func_8009ECB0(2, 0, 0x140 + i * 0x80, 0x100);
        switch (arg1) {
        case 0:
            poly[i]->x0 = 0x18 - arg0 * 11 / 80 + i * 0x88 - arg0 * i * 0x44 / 80; poly[i]->y0 = game_state.screen_center_y[draw_buffer] - 0x65;
            poly[i]->x1 = 0x18 - arg0 * 11 / 80 + i * 0x88 - arg0 * i * 0x44 / 80 - arg0 * 0x44 / 80 + 0x88; poly[i]->y1 = game_state.screen_center_y[draw_buffer] - 0x65;
            poly[i]->x2 = 0x18 - arg0 * 11 / 80 + i * 0x88 - arg0 * i * 0x44 / 80; h = 0xA0 - arg0; poly[i]->y2 = game_state.screen_center_y[draw_buffer] + h - 0x65;
            poly[i]->x3 = 0xA0 - arg0 * 11 / 80 + i * 0x88 - arg0 * i * 0x44 / 80 - arg0 * 0x44 / 80; poly[i]->y3 = game_state.screen_center_y[draw_buffer] + h - 0x65;
            break;
        case 1:
            poly[i]->x0 = arg0 * 141 / 80 + i * 0x88 - arg0 * i * 0x44 / 80 + 0x18; poly[i]->y0 = game_state.screen_center_y[draw_buffer] - 0x65;
            poly[i]->x1 = arg0 * 141 / 80 + i * 0x88 - arg0 * i * 0x44 / 80 - arg0 * 0x44 / 80 + 0xA0; poly[i]->y1 = game_state.screen_center_y[draw_buffer] - 0x65;
            poly[i]->x2 = arg0 * 141 / 80 + i * 0x88 - arg0 * i * 0x44 / 80 + 0x18; h = 0xA0 - arg0; poly[i]->y2 = game_state.screen_center_y[draw_buffer] + h - 0x65;
            poly[i]->x3 = arg0 * 141 / 80 + i * 0x88 - arg0 * i * 0x44 / 80 - arg0 * 0x44 / 80 + 0xA0; poly[i]->y3 = game_state.screen_center_y[draw_buffer] + h - 0x65;
            break;
        case 2:
            poly[i]->x0 = 0x18 - arg0 * 11 / 80 + i * 0x88 - arg0 * i * 0x44 / 80; poly[i]->y0 = game_state.screen_center_y[draw_buffer] + arg0 * 112 / 80 - 0x65;
            poly[i]->x1 = 0x18 - arg0 * 11 / 80 + i * 0x88 - arg0 * i * 0x44 / 80 - arg0 * 0x44 / 80 + 0x88; poly[i]->y1 = game_state.screen_center_y[draw_buffer] + arg0 * 112 / 80 - 0x65;
            poly[i]->x2 = 0x18 - arg0 * 11 / 80 + i * 0x88 - arg0 * i * 0x44 / 80; h = 0xA0 - arg0; poly[i]->y2 = game_state.screen_center_y[draw_buffer] + arg0 * 112 / 80 + h - 0x65;
            poly[i]->x3 = 0xA0 - arg0 * 11 / 80 + i * 0x88 - arg0 * i * 0x44 / 80 - arg0 * 0x44 / 80; poly[i]->y3 = game_state.screen_center_y[draw_buffer] + arg0 * 112 / 80 + h - 0x65;
            break;
        case 3:
            poly[i]->x0 = arg0 * 141 / 80 + i * 0x88 - arg0 * i * 0x44 / 80 + 0x18; poly[i]->y0 = game_state.screen_center_y[draw_buffer] + arg0 * 112 / 80 - 0x65;
            poly[i]->x1 = arg0 * 141 / 80 + i * 0x88 - arg0 * i * 0x44 / 80 - arg0 * 0x44 / 80 + 0xA0; poly[i]->y1 = game_state.screen_center_y[draw_buffer] + arg0 * 112 / 80 - 0x65;
            poly[i]->x2 = arg0 * 141 / 80 + i * 0x88 - arg0 * i * 0x44 / 80 + 0x18; h = 0xA0 - arg0; poly[i]->y2 = game_state.screen_center_y[draw_buffer] + arg0 * 112 / 80 + h - 0x65;
            poly[i]->x3 = arg0 * 141 / 80 + i * 0x88 - arg0 * i * 0x44 / 80 - arg0 * 0x44 / 80 + 0xA0; poly[i]->y3 = game_state.screen_center_y[draw_buffer] + arg0 * 112 / 80 + h - 0x65;
            break;
        }
        poly[i]->u0 = i * 8 + 0x18; poly[i]->v0 = 0x13;
        poly[i]->u1 = i * 8 + 0xA0; poly[i]->v1 = 0x13;
        poly[i]->u2 = i * 8 + 0x18; poly[i]->v2 = 0xB3;
        poly[i]->u3 = i * 8 + 0xA0; poly[i]->v3 = 0xB3;
        poly[i]->r0 = 0x80; poly[i]->g0 = 0x80; poly[i]->b0 = 0x80;
        AddPrim(draw_buffer * 0x400 + 0x28 + D_800E8CA0, poly[i]);
    }
}

void func_8013DEA0(s32 arg0) {
    SPRT *temp_v0;
    SPRT *temp_v0_2;

    temp_v0 = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(temp_v0);
    SetSemiTrans(temp_v0, 0);
    SetShadeTex(temp_v0, 0);
    temp_v0->r0 = 0x80;
    temp_v0->g0 = 0x80;
    temp_v0->b0 = 0x80;
    if (arg0 == 1) {
        temp_v0->x0 = (s16) (game_state.screen_center_x[draw_buffer] - 0xA0);
        temp_v0->y0 = (s16) (game_state.screen_center_y[draw_buffer] - 0x65);
        temp_v0->v0 = 0;
        temp_v0->u0 = 0;
        /* FAKE: paired stores share a source line for the original as1 order. T-9930. */
        temp_v0->w = 0xC0; temp_v0->h = 0xA0;
    }
    AddPrim((draw_buffer << 0xA) + 0x34 + D_800E8CA0, temp_v0);
    goto_tpage(0x140, 0x100, 0xD, 0, 2);
    temp_v0_2 = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(temp_v0_2);
    SetSemiTrans(temp_v0_2, 0);
    SetShadeTex(temp_v0_2, 0);
    temp_v0_2->r0 = 0x80;
    temp_v0_2->g0 = 0x80;
    temp_v0_2->b0 = 0x80;
    if (arg0 == 1) {
        temp_v0_2->x0 = (s16) (game_state.screen_center_x[draw_buffer] + 0x20);
        temp_v0_2->y0 = (s16) (game_state.screen_center_y[draw_buffer] - 0x65);
        temp_v0_2->v0 = 0;
        temp_v0_2->u0 = 0;
        /* FAKE: same dimension-store scheduling dependency as above. T-9930. */
        temp_v0_2->w = 0x80; temp_v0_2->h = 0xA0;
    }
    AddPrim((draw_buffer << 0xA) + 0x34 + D_800E8CA0, temp_v0_2);
    goto_tpage(0x200, 0x100, 0xD, 0, 2);
}

void func_8013E094(u8 *arg0) {
    s32 pad; /* FAKE: unused slot above base, the original frame has it (T-3330 idiom, real source unknown). T-10710 */
    s16 base;
    u8 *p;
    s16 e1;
    s16 e2;

    if (D_8015004C == 0xA && D_80150050 == 1) {
        D_8011F536 = -0x15;
        D_8011F4F2 = -0x15;
    } else {
        D_8011F536 = -0xD;
        D_8011F4F2 = -0xD;
    }
    p = *(u8 **)arg0;
    if (*(s16 *)p >= 6) {
        base = *(s16 *)(p + 4);
        e1 = *(s16 *)(p + 0x50);
        e2 = *(s16 *)(p + 0x42);
        D_801217D6 = D_8011F536 + e1 - base;
        D_801217FA = D_8011F536 + e2 - base;
    }
    if (*(s16 *)p >= 4) {
        e1 = *(s16 *)(p + 0x26);
        e2 = *(s16 *)(p + 0x34);
        D_8012181E = D_8011F536 + e1 - base;
        D_80121842 = D_8011F536 + e2 - base;
    }
    if (*(s16 *)p >= 7) {
        e1 = *(s16 *)(p + 0x5E);
        D_80121866 = D_8011F536 + e1 - base;
    }
    if (*(s16 *)p == 8) {
        e2 = *(s16 *)(p + 0x6C);
        D_8012188A = D_8011F536 + e2 - base;
    }
}

void func_8013E1DC(arg0)
s32 arg0;
{
    EtcPolyF4 *poly;
    s32 sprite;

    poly = GetWorkBase(0x18, draw_buffer);
    func_8009F02C(poly);
    SetSemiTrans(poly, 1);
    SetShadeTex(poly, 0);
    if (arg0 >= 0x100) {
        arg0 = 0xFF;
    }
    poly->x0 = 0;
    poly->y0 = game_state.screen_center_y[draw_buffer] - 0x78;
    poly->x1 = 0x140;
    poly->y1 = game_state.screen_center_y[draw_buffer] - 0x78;
    poly->x2 = 0;
    poly->y2 = game_state.screen_center_y[draw_buffer] + 0x78;
    poly->x3 = 0x140;
    poly->y3 = game_state.screen_center_y[draw_buffer] + 0x78;
    poly->b0 = arg0;
    poly->g0 = arg0;
    poly->r0 = arg0;
    AddPrim(draw_buffer * 0x400 + D_800E8CA0, poly);
    sprite = GetWorkBase(0xC, draw_buffer);
    func_8009D294(sprite, 0, 0, func_8009ECB0(1, 2, 0, 0), 0);
    AddPrim(draw_buffer * 0x400 + D_800E8CA0, sprite);
    safe_env(0);
}

void func_8013E364(void) {
    if (D_8015005C < 0xA && D_80150058 < 0xA) {
        func_8013FC64(D_80150054, 0x50, 0x2D, 0);
        func_8013FC64(-D_80150058, 0x64, 0x2D, 0);
        func_8013E524(0xE5, 0x3E);
    } else if (D_8015005C >= 0xA && D_80150058 < 0xA) {
        func_8013FC64(D_80150054, 0x4B, 0x2D, 0);
        func_8013FC64(-D_80150058, 0x5F, 0x2D, 0);
        func_8013E524(0xE0, 0x42);
    } else if (D_8015005C < 0xA && D_80150058 >= 0xA) {
        func_8013FC64(D_80150054, 0x4B, 0x2D, 0);
        func_8013FC64(-D_80150058, 0x64, 0x2D, 0);
        func_8013E524(0xE0, 0x42);
    } else if (D_8015005C >= 0xA && D_80150058 >= 0xA) {
        func_8013FC64(D_80150054, 0x46, 0x2D, 0);
        func_8013FC64(-D_80150058, 0x5F, 0x2D, 0);
        func_8013E524(0xDB, 0x47);
    }
    func_8013FC64(-D_8015005C, 0x78, 0x2D, 0);
}

/* Port of the TACO sibling func_80153E78 (T-17300). */
void func_8013E524(s32 a, s32 b) {
    POLY_F4 *poly;
    s32 sprite;

    poly = GetWorkBase(0x18, draw_buffer);
    func_8009F02C(poly);
    SetSemiTrans(poly, 1);
    SetShadeTex(poly, 0);
    poly->x0 = a;
    poly->y0 = game_state.screen_center_y[draw_buffer] + 0x2B;
    poly->x1 = a + b;
    poly->y1 = game_state.screen_center_y[draw_buffer] + 0x2B;
    poly->x2 = a;
    poly->y2 = game_state.screen_center_y[draw_buffer] + 0x37;
    poly->x3 = a + b;
    poly->y3 = game_state.screen_center_y[draw_buffer] + 0x37;
    poly->b0 = 0;
    poly->g0 = 0;
    poly->r0 = 0;
    AddPrim(draw_buffer * 0x400 + 0xC + D_800E8CA0, poly);
    sprite = GetWorkBase(0xC, draw_buffer);
    func_8009D294(sprite, 0, 0, func_8009ECB0(1, 0, 0, 0), 0);
    AddPrim(draw_buffer * 0x400 + 0xC + D_800E8CA0, sprite);
    safe_env(3);
}

/* Both record tables are read through pointer locals: the original reloads the +5 byte for the switch, direct
 * game_state.event_log[...] accesses CSE it with the earlier load. The second table is the EtcConfig8 record array
 * at system_data + 0x58 (main_api.h): a byte pointer plus `[11]` after the scaled index keeps the +0x58 as the
 * displacement, while a typed base folds it into the address (T-22019). */
void func_8013E6AC(void) {
    GameState *g = &game_state;
    u8 *w = (u8 *)&system_data;

    if (g->options_dirty == 0) {
        D_8015004C = g->event_log[D_80150064].girl;
        D_80150044 = g->event_log[D_80150064].flag;
        D_80150040 = g->event_log[D_80150064].scene;
        D_80150048 = g->event_log[D_80150064].rank % 6U;
        D_80150054 = g->event_log[D_80150064].year;
        D_80150058 = g->event_log[D_80150064].month;
        D_8015005C = g->event_log[D_80150064].day;
        D_80150060 = g->event_log[D_80150064].weekday;
        switch (g->event_log[D_80150064].month) {
        case 3:
        case 4:
        case 5:
            D_80150050 = 0;
            return;
        case 6:
        case 7:
        case 8:
            D_80150050 = 1;
            return;
        case 9:
        case 10:
        case 11:
            D_80150050 = 2;
            return;
        case 1:
        case 2:
        case 12:
            D_80150050 = 3;
            return;
        }
    } else {
        D_8015004C = ((EtcConfig8 *)(w + D_80150064 * 8))[11].unk_00;
        D_80150044 = ((EtcConfig8 *)(w + D_80150064 * 8))[11].unk_01;
        D_80150040 = ((EtcConfig8 *)(w + D_80150064 * 8))[11].unk_02;
        D_80150048 = ((EtcConfig8 *)(w + D_80150064 * 8))[11].unk_03 % 6U;
        D_80150054 = ((EtcConfig8 *)(w + D_80150064 * 8))[11].unk_04;
        D_80150058 = ((EtcConfig8 *)(w + D_80150064 * 8))[11].unk_05;
        D_8015005C = ((EtcConfig8 *)(w + D_80150064 * 8))[11].unk_06;
        D_80150060 = ((EtcConfig8 *)(w + D_80150064 * 8))[11].unk_07;
        switch (((EtcConfig8 *)(w + D_80150064 * 8))[11].unk_05) {
        case 3:
        case 4:
        case 5:
            D_80150050 = 0;
            return;
        case 6:
        case 7:
        case 8:
            D_80150050 = 1;
            return;
        case 9:
        case 10:
        case 11:
            D_80150050 = 2;
            return;
        case 1:
        case 2:
        case 12:
            D_80150050 = 3;
        }
    }
}

s32 func_8013E89C(s32 arg0) {
    switch (D_80150070) {
    case -1:
        break;
    case 0:
        func_8013E6AC();
        if (func_8013EAC0() != 0) {
            func_8013EB90(arg0);
            D_80150070 += 1;
        } else {
            D_80150070 = -1;
        }
        break;
    case 1:
        if ((D_8014F990 == 1) || (D_8015007C == 1)) {
            if (cd_read_sync_status() & 1) {
                dec_bg_reset();
                func_80132FB0(D_80150040, D_80150044, D_8015004C, D_80150050);
                if ((D_8014E0C0 == 0) && (D_8014F990 == 1)) {
                    if (D_80150064 < 0xFF) {
                        D_80150064 += 1;
                    }
                    if ((D_801500D8 == 0) || (D_801500D8 == 2)) {
                        func_8013EEFC();
                    }
                    D_80150070 = 0;
                } else if (D_8014E0C0 == 1) {
                    D_80150070 += 1;
                } else {
                    D_80150070 = 3;
                }
            }
        } else if ((D_8014F990 == 0) && (D_8015007C == 0)) {
            if (D_80150064 < 0xFF) {
                D_80150064 += 1;
            }
            if ((D_801500D8 == 0) || (D_801500D8 == 2)) {
                func_8013EEFC();
            }
            D_80150070 = 0;
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            D_80150070 += 1;
        }
        break;
    }
}

s32 func_8013EAC0(void) {
    if (D_80150054 == 0 && D_80150058 == 0 && D_8015005C == 0) {
        while (1) {
            if (D_80150064 < 0xFF) {
                D_80150064 += 1;
            }
            func_8013E6AC();
            if (D_80150054 != 0 && D_80150058 != 0 && D_8015005C != 0) {
                return 1;
            }
            if (D_80150064 == 0xFF) {
                return 0;
            }
        }
    }
    return 1;
}

s32 func_8013EB90(s32 arg0) {
    s32 temp_v0;

    func_8013B1F0(D_80150040, D_80150044, D_8015004C, D_80150050);
    if ((D_8014F990 == 0) && ((D_80150040 == 2) || (D_80150040 == 8))) {
        if ((D_80150040 == 8) && (D_80150044 == 0xE)) {
            cd_read_sectors(0x71U, 0x801AE000, 0x8768);
            D_8015007C = 1;
            D_80150068 = arg0;
        } else if ((D_80150040 == 8) && (D_80150044 == 0x46)) {
            cd_read_sectors(0x71U, 0x801B0000, 0x85B6);
            D_8015007C = 1;
            D_80150068 = arg0;
        } else if (D_80150040 == 2) {
            temp_v0 = D_80150044;
            cd_read_sectors(D_8014F828[temp_v0], 0x801B0000, D_8014F6C0[temp_v0]);
            D_8015007C = 1;
            if (D_80150044 == 0x20) {
                D_80150068 = arg0;
            }
        }
    }
}

void func_8013ECDC(void) {
    func_801408F0();
    if (D_8014F990 == 1) {
        func_8013ED8C();
    } else if (D_8015007C == 1) {
        func_80136448();
    }
}

void func_8013ED38(void) {
    D_80150084 = (u8 *)0x80180084;
    D_80150088 = (u8 *)0x80180098;
    D_8015008C = (u8 *)0x8018009C;
    D_80150090 = *(s16 *)0x801960AC;
    D_80150094 = (u8 *)0x801800AC;
}

void func_8013ED8C(void) {
    func_8013ED38();
    sprite_quads_clear(0, 0x40);
    portrait_build(D_80150094, D_80150084, D_80150088, D_8015008C, D_80150048);
    func_8013E094(D_80150088);
}

void func_8013EDF8(void) {
    if (D_8014F990 == 1) {
        portrait_face_update();
    }
}

void func_8013EE28(void) {
    if (D_8014F990 == 1) {
        func_8013EEAC();
    } else if (D_8015007C == 1) {
        func_8013EE7C();
    }
}

void func_8013EE7C(void) {
    D_8015007C = 0;
    bg_layers = 0x80000000;
    D_80120653 = 0;
    D_80120697 = 0;
    D_801206DB = 0;
}

void func_8013EEAC(void) {
    D_8011F50F = 0;
    D_8011F4CB = 0;
    D_80120653 = 0;
    sprite_quads[0].flags = 0x80000000;
    sprite_quads[1].flags = 0x80000000;
    sprite_quads[2].flags = 0x80000000;
    sprite_quads[3].flags = 0x80000000;
    sprite_quads[4].flags = 0x80000000;
    sprite_quads[5].flags = 0x80000000;
}

void func_8013EEFC(void) {
loop_1:
    func_8013E6AC();
    if ((D_80150054 == 0) && (D_80150058 == 0) && (D_8015005C == 0)) {
loop_4:
        if (D_80150064 < 0xFF) {
            D_80150064 += 1;
        }
        func_8013E6AC();
        if ((D_80150054 == 0) || (D_80150058 == 0) || (D_8015005C == 0)) {
            if (D_80150064 == 0xFF) {
                D_80150070 = -1;
            } else {
                goto loop_4;
            }
        }
    }
    if (D_8015004C != game_state.current_girl) {
        if (D_80150070 == -1) {
            return;
        }
        if (D_80150064 < 0xFF) {
            D_80150064 += 1;
            goto loop_1;
        }
        if (D_80150064 == 0xFF) {
            D_80150070 = -1;
        } else {
            goto loop_1;
        }
    }
}

/* Called undeclared (implicit int) by func_8013BFFC and func_8013C330; no value is returned. The search walks the
 * records down from D_80150064 - 1 with the decrement before the tests (the original's pointer step comes before
 * the compares); the record and the selector are read through `g`, which keeps the two final unk_F5F reads
 * separate as in the original. T-25113. */
s32 func_8013F054(void) {
    GameState *g = &game_state;
    u8 *w = (u8 *)&system_data;
    s32 i;
    s32 type;
    s32 a;
    u8 b;

    i = D_80150064;
    b = 0;
    a = 0;
    type = 0;
    if (g->options_dirty == 0) {
        if (D_801500D8 == 1 || D_801500D8 == 3) {
            type = g->event_log[i - 1].girl;
            a = g->event_log[i - 1].scene;
            b = g->event_log[i - 1].flag;
        } else if ((D_801500D8 == 0 || D_801500D8 == 2) && i >= 0) {
            do {
                i--;
                if ((g->event_log[i].girl == 0xC && g->current_girl == GIRL_REI) ||
                    (g->event_log[i].girl == 0xB && g->current_girl == GIRL_MIHARU)) {
                    type = g->event_log[i].girl;
                    a = g->event_log[i].scene;
                    b = g->event_log[i].flag;
                    break;
                }
            } while (i >= 0);
        }
    } else {
        type = ((EtcConfig8 *)(w + D_80150080 * 8))[11].unk_00;
        a = ((EtcConfig8 *)(w + D_80150080 * 8))[11].unk_02;
        b = ((EtcConfig8 *)(w + D_80150080 * 8))[11].unk_01;
    }
    if (g->current_girl == GIRL_REI && type == 0xC) {
        if (a == 4) {
            func_8013F320(1);
        } else if (a == 6) {
            func_8013F320(2);
        } else if (a == 7) {
            func_8013F320(3);
        } else if (a == 2 && b == 0x4F) {
            func_8013F320(5);
        } else if (a == 2 && b == 0x50) {
            func_8013F320(4);
        } else if (a == 2 && b == 0x51) {
            func_8013F320(6);
        }
    } else if (g->current_girl == GIRL_MIHARU && type == 0xB) {
        if (a == 2 && b == 0x4A) {
            func_8013F320(0);
        } else if ((a == 2 && b == 0x4C) || (a == 0xB && b != 0x1E)) {
            func_8013F320(1);
        } else if ((a == 2 && b == 0x4E) || (a == 0xB && b == 0x1E)) {
            func_8013F320(2);
        } else if (a == 0x12) {
            func_8013F320(3);
        }
    }
}

/* T-24129: retain the earlier caller's K&R implicit-int view (func_8013CF2C). */
s32 func_8013F320(s32 arg0) {
    switch (D_8015006C) {
    case 0:
        cd_audio_request_set(D_801500C4[arg0 * 2], D_801500C4[arg0 * 2 + 1], game_state.current_girl);
        audio_command(0x300);
        D_8015006C += 1;
        break;
    case 1:
        if (cd_audio_state() & 4) {
            D_8015006C += 1;
        }
        break;
    case 2:
        if (cd_audio_state() & 2) {
            D_8015006C += 1;
        }
        break;
    }
}

s32 func_8013F3F8(void) {
    if (D_80150040 == 0x15 &&
        (D_80150044 == 6 || D_80150044 == 7 || D_80150044 == 8 || D_80150044 == 9 || D_80150044 == 10)) {
        if ((D_8015004C == 0 || D_8015004C == 1 || D_8015004C == 4 || D_8015004C == 6 || D_8015004C == 9 ||
             D_8015004C == 10) &&
            D_80150048 == 5) {
            return 1;
        }
    }
    return 0;
}

void func_8013F498(void) {
    /* FAKE: unsigned halfword stores prevent IDO sharing constant 7 with the byte selector. T-9750. */
    func_801408F0();
    func_8013BEF0();
    actor_clear(0x60);
    D_80120651 = 0;
    D_80120688 = 0x01000000;
    D_80120657 = 0x80;
    D_80120655 = 0x80;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
    D_80120693 = 0x10;
    D_80120676 = 0;
    D_8012067A = -0xD;
    D_8012065C = D_80150034;
    D_80120660 = D_80150038;
    D_80120684 = D_80150030;
    D_80120664 = D_8015003C;
    D_80120652 = 1;
    D_80120653 = 0x84;
    D_80120668 = 2;
    if (D_8015004C == 0) {
        if ((D_80150058 == 6) || (D_80150058 == 7) || (D_80150058 == 8) || (D_80150058 == 9)) {
            D_80120666 = 0;
        } else {
            D_80120666 = 1;
        }
    }
    if (D_8015004C == 1) {
        if ((D_80150058 == 6) || (D_80150058 == 7) || (D_80150058 == 8) || (D_80150058 == 9)) {
            D_80120666 = 2;
        } else {
            D_80120666 = 3;
        }
    }
    if (D_8015004C == 4) {
        if ((D_80150058 == 6) || (D_80150058 == 7) || (D_80150058 == 8) || (D_80150058 == 9)) {
            D_80120666 = 4;
        } else {
            D_80120666 = 5;
        }
        if (D_80150060 == 0xFF) {
            *(u16 *)&sprite_quads[0].unk_0C = 7;
            sprite_quads[0].unk_0E = 0x70;
        }
    }
    if (D_8015004C == 6) {
        if ((D_80150058 == 6) || (D_80150058 == 7) || (D_80150058 == 8) || (D_80150058 == 9)) {
            D_80120666 = 6;
        } else {
            D_80120666 = 7;
            D_8012067A = -0x12;
        }
    }
    if (D_8015004C == 9) {
        if ((D_80150058 == 6) || (D_80150058 == 7) || (D_80150058 == 8) || (D_80150058 == 9)) {
            D_80120666 = 8;
        } else {
            D_80120666 = 9;
        }
        if (D_80150060 == 0xFF) {
            *(u16 *)&sprite_quads[0].unk_0C = 7;
            sprite_quads[0].unk_0E = 0x70;
        }
    }
    if (D_8015004C == 0xA) {
        if ((D_80150058 == 6) || (D_80150058 == 7) || (D_80150058 == 8) || (D_80150058 == 9)) {
            D_80120666 = 0xA;
        } else {
            D_80120666 = 0xB;
        }
    }
}

void func_8013F77C(void) {
    /* FAKE: record view spans legacy scalar declarations; canonical main BSS aggregate is pending. T-9750. */
    /* FAKE: loop store grouped on header line retains as1 unrolled-store order. T-9750. */
    s32 i;

    for (i = 0; i < 13; i++) { game_state.girl_cleared[i] = 1; }
    game_state.girl_cleared[11] = 5;
    game_state.current_girl = GIRL_MIHARU;
    ((EtcConfig8 *)&system_data)[11].unk_00 = 0xB;
    ((EtcConfig8 *)&system_data)[11].unk_02 = 0xB;
    ((EtcConfig8 *)&system_data)[11].unk_01 = 0x1D;
    ((EtcConfig8 *)&system_data)[11].unk_03 = 0;
    ((EtcConfig8 *)&system_data)[11].unk_04 = 0x60;
    ((EtcConfig8 *)&system_data)[11].unk_05 = 4;
    ((EtcConfig8 *)&system_data)[11].unk_06 = 0x19;
    ((EtcConfig8 *)&system_data)[11].unk_07 = 0;
    ((EtcConfig8 *)&system_data)[12].unk_00 = 0xB;
    ((EtcConfig8 *)&system_data)[12].unk_02 = 0xB;
    ((EtcConfig8 *)&system_data)[12].unk_01 = 0x1E;
    ((EtcConfig8 *)&system_data)[12].unk_03 = 0;
    ((EtcConfig8 *)&system_data)[12].unk_04 = 0x60;
    ((EtcConfig8 *)&system_data)[12].unk_05 = 8;
    ((EtcConfig8 *)&system_data)[12].unk_06 = 0x19;
    ((EtcConfig8 *)&system_data)[12].unk_07 = 0;
    ((EtcConfig8 *)&system_data)[13].unk_00 = 0xB;
    ((EtcConfig8 *)&system_data)[13].unk_02 = 0xB;
    ((EtcConfig8 *)&system_data)[13].unk_01 = 0xB;
    ((EtcConfig8 *)&system_data)[13].unk_03 = 0;
    ((EtcConfig8 *)&system_data)[13].unk_04 = 0x60;
    ((EtcConfig8 *)&system_data)[13].unk_05 = 7;
    ((EtcConfig8 *)&system_data)[13].unk_06 = 0x19;
    ((EtcConfig8 *)&system_data)[13].unk_07 = 0;
    ((EtcConfig8 *)&system_data)[14].unk_00 = 0xB;
    ((EtcConfig8 *)&system_data)[14].unk_02 = 0xB;
    ((EtcConfig8 *)&system_data)[14].unk_01 = 0xA;
    ((EtcConfig8 *)&system_data)[14].unk_03 = 0;
    ((EtcConfig8 *)&system_data)[14].unk_04 = 0x60;
    ((EtcConfig8 *)&system_data)[14].unk_05 = 2;
    ((EtcConfig8 *)&system_data)[14].unk_06 = 0x19;
    ((EtcConfig8 *)&system_data)[14].unk_07 = 0;
    ((EtcConfig8 *)&system_data)[15].unk_00 = 0xB;
    ((EtcConfig8 *)&system_data)[15].unk_02 = 0x12;
    ((EtcConfig8 *)&system_data)[15].unk_01 = 0;
    ((EtcConfig8 *)&system_data)[15].unk_03 = 0;
    ((EtcConfig8 *)&system_data)[15].unk_04 = 0x60;
    ((EtcConfig8 *)&system_data)[15].unk_05 = 5;
    ((EtcConfig8 *)&system_data)[15].unk_06 = 0x19;
    ((EtcConfig8 *)&system_data)[15].unk_07 = 0;
}
