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
} AlbumPolyF4View; /* size 0x18 */

void album_portrait_tables_init(void) {
    album_portrait_tpage_table = (u8 *)0x8019DCFC;
    album_portrait_frame_table = (u8 *)0x8019DD00;
    album_portrait_sequence_table = (u8 *)0x8019DDC0;
    album_portrait_frame_count = *(s16 *)0x8019DDF8;
}

void album_stage_playback(void) {
    switch (game_state.mode_step) {
    case 0:
        album_playback_page_start();
        return;
    case 1:
        album_playback_photo(2, 0);
        return;
    case 2:
        album_playback_photo(3, 1);
        return;
    case 3:
        album_playback_photo(4, 2);
        return;
    case 4:
        album_playback_photo_last();
        return;
    case 5:
        album_playback_voice();
        return;
    case 6:
        album_playback_end_check();
        return;
    case 7:
        album_playback_ui_restore();
        /* fallthrough */
    default:
        return;
    }
}

/* T-24069: retain the earlier caller's K&R implicit-int view. */
s32 album_playback_page_start(void) {
    GameState *g = &game_state;
    /* FAKE: join step/status loads through one word temporary to preserve
     * the original v1 allocation; original local declarations unknown. T-24069. */
    s32 value;
    switch (g->substep) {
    case 0:
        album_clear_buffer_area();
        bg_layers = 0x80000000;
        if (album_ui_reload_request > 0) {
            album_load_state = 0;
        }
        if ((value = album_load_state) == -1) {
            album_model_angle = 0x800;
            album_model_matrix_update();
            mode_stage_next();
        } else {
            g->substep = (value = g->substep) + 1;
        }
        break;
    case 1:
        if (album_ui_reload_request > 0) {
            album_event_advance(1);
            if ((value = album_load_state) >= 3) {
                if (album_ui_reload_request == 1) {
                    album_load_state = 0;
                } else {
                    album_ui_reload_request = 0;
                    album_load_state = 0;
                }
                g->substep = (value = g->substep) + 1;
            } else if ((value = album_load_state) == -1) {
                album_model_angle = 0x800;
                album_model_matrix_update();
                mode_step_set(5);
            }
        } else {
            g->substep = (value = g->substep) + 1;
        }
        break;
    case 2:
        if (album_event_is_special_portrait() == 1) {
            cd_read_sectors(0xEU, 0x80197000, 0xA9D0);
            g->substep = (value = g->substep) + 1;
        } else {
            g->substep = (value = g->substep) + 1;
        }
        break;
    case 3:
        if (album_event_is_special_portrait() == 0) {
            if (album_event_has_background == 1) {
                dec_bg_show_set(1, dec_bg_decode_to_vram(1));
                album_picture_sprites_draw(1);
            }
            album_event_resources_show();
            album_portrait_face_update();
            album_date_counters_draw();
            g->substep = (value = g->substep) + 1;
        } else if ((album_event_is_special_portrait() == 1) && (cd_read_sync_status() & 1)) {
            if (album_event_has_background == 1) {
                dec_bg_show_set(1, dec_bg_decode_to_vram(1));
                album_picture_sprites_draw(1);
            }
            album_event_resources_show();
            if (album_event_is_special_portrait() == 1) {
                album_special_portrait_setup();
            }
            album_portrait_face_update();
            album_date_counters_draw();
            g->substep = (value = g->substep) + 1;
        }
        break;
    case 4:
        album_vram_copy_to_backbuffer();
        album_vram_copy_other_buffer();
        album_event_hide_current();
        album_vram_blur(0x28, 0x3C);
        if (album_event_index < 0xFF) {
            album_prev_event_index = album_event_index;
            album_event_index += 1;
        }
        if ((album_secret_mode == 0) || (album_secret_mode == 2)) {
            album_event_skip_other_girls();
        }
        mode_step_next();
        break;
    }
    album_background_quads_draw();
}


/* T-14501: retain the earlier callers' K&R implicit-int view. */
s32 album_playback_photo(s32 arg0, s32 arg1) {
    switch (game_state.substep) {
    case 0:
        album_fade_quads_draw();
        break;
    case 1:
        album_photo_zoom_draw(0, 0);
        album_event_advance(arg0);
        if (album_load_state >= 3) {
            game_state.substep += 1;
            album_load_state = 0;
        } else if (album_load_state == -1) {
            game_state.substep += 1;
        }
        break;
    case 2:
        album_photo_zoom_draw(0, 0);
        if ((game_state.current_girl == GIRL_REI) || (game_state.current_girl == GIRL_MIHARU)) {
            album_special_girl_voice();
        }
        if (((album_voice_state == 0) || (album_voice_state >= 3)) && ((game_state.pad_pressed & PAD_CIRCLE) || (album_secret_mode == 0) || (album_secret_mode == 1))) {
            game_state.substep += 1;
            album_voice_state = 0;
        }
        break;
    case 3:
        album_photo_zoom_draw(game_state.mode_step_timer.u * 2, arg1);
        if ((u32) game_state.mode_step_timer.u < 0x28U) {
            game_state.mode_step_timer.u += 1;
        } else {
            game_state.substep += 1;
        }
        break;
    case 4:
        album_clear_buffer_area();
        if (album_load_state == -1) {
            album_model_angle = 0x800;
            album_model_matrix_update();
            mode_step_set(5);
        } else {
            game_state.substep += 1;
        }
        break;
    case 5:
        if (album_event_is_special_portrait() == 1) {
            cd_read_sectors(0xEU, 0x80197000, 0xA9D0);
            game_state.substep += 1;
        } else {
            game_state.substep += 1;
        }
        break;
    case 6:
        if (album_event_is_special_portrait() == 0) {
            if (album_event_has_background == 1) {
                dec_bg_show_set(1, dec_bg_decode_to_vram(1));
                album_picture_sprites_draw(1);
            }
            album_event_resources_show();
            album_portrait_face_update();
            album_date_counters_draw();
            game_state.substep += 1;
        }
        if ((album_event_is_special_portrait() == 1) && (cd_read_sync_status() & 1)) {
            if (album_event_has_background == 1) {
                dec_bg_show_set(1, dec_bg_decode_to_vram(1));
                album_picture_sprites_draw(1);
            }
            album_event_resources_show();
            if (album_event_is_special_portrait() == 1) {
                album_special_portrait_setup();
            }
            album_portrait_face_update();
            album_date_counters_draw();
            game_state.substep += 1;
        }
        break;
    case 7:
        album_vram_copy_to_backbuffer();
        album_vram_copy_other_buffer();
        album_event_hide_current();
        album_vram_blur(0x28, 0x3C);
        if (album_event_index < 0xFF) {
            album_prev_event_index = album_event_index;
            album_event_index += 1;
        }
        if ((album_secret_mode == 0) || (album_secret_mode == 2)) {
            album_event_skip_other_girls();
        }
        mode_step_next();
        break;
    }
    album_background_quads_draw();
}

s32 album_playback_photo_last(void) {
    switch (game_state.substep) {
    case 0:
        album_background_quads_draw();
        album_fade_quads_draw();
    default:
        break;
    case 1:
        album_background_quads_draw();
        album_photo_zoom_draw(0, 0);
        album_event_advance(5);
        if (album_load_state >= 3) {
            album_load_state = 0;
            game_state.substep += 1;
        } else if (album_load_state == -1) {
            game_state.substep += 1;
        }
        break;
    case 2:
        album_background_quads_draw();
        album_photo_zoom_draw(0, 0);
        if ((game_state.current_girl == GIRL_REI) || (game_state.current_girl == GIRL_MIHARU)) {
            album_special_girl_voice();
        }
        if (((album_voice_state == 0) || (album_voice_state >= 3)) && ((game_state.pad_pressed & PAD_CIRCLE) || (album_secret_mode == 0) || (album_secret_mode == 1))) {
            album_voice_state = 0;
            game_state.substep += 1;
        }
        break;
    case 3:
        album_background_quads_draw();
        album_photo_zoom_draw(game_state.mode_step_timer.u * 2, 3);
        if ((u32) game_state.mode_step_timer.u < 0x28U) {
            game_state.mode_step_timer.u += 1;
        } else {
            game_state.substep += 1;
        }
        break;
    case 4:
        album_clear_buffer_area();
        album_model_angle = 0x800;
        album_model_matrix_update();
        mode_step_next();
        break;
    }
}

/* FAKE: retain the s32 fallthrough matching view; original return type is unproven,
 * and the sole caller ignores the result. T-24075. */
s32 album_playback_voice(void) {

    if ((game_state.current_girl == GIRL_REI) || (game_state.current_girl == GIRL_MIHARU)) {
        mode_step_next();
    } else {
        switch (game_state.substep) {
        case 0:
            if (game_state.current_girl == GIRL_SHIORI) {
                do {
                    album_voice_clip = rand() % album_voice_clip_count[game_state.current_girl];
                } while ((album_voice_clip == album_last_voice_clip) || (album_voice_clip == 20) || (album_voice_clip == 0));
            } else if (game_state.current_girl == GIRL_YUINA) {
                do {
                    album_voice_clip = rand() % album_voice_clip_count[game_state.current_girl];
                } while ((album_voice_clip == album_last_voice_clip) || (album_voice_clip == 8));
            } else if (game_state.current_girl == GIRL_AYAKO) {
                do {
                    album_voice_clip = rand() % album_voice_clip_count[game_state.current_girl];
                } while ((album_voice_clip == album_last_voice_clip) || (album_voice_clip == 9));
            } else if (game_state.current_girl == GIRL_SAKI) {
                do {
                    album_voice_clip = rand() % album_voice_clip_count[game_state.current_girl];
                } while ((album_voice_clip == album_last_voice_clip) || (album_voice_clip == 13));
            } else if (game_state.current_girl == GIRL_MEGUMI) {
                do {
                    album_voice_clip = rand() % album_voice_clip_count[game_state.current_girl];
                } while ((album_voice_clip == album_last_voice_clip) || (album_voice_clip == 0));
            } else {
                do {
                    album_voice_clip = rand() % album_voice_clip_count[game_state.current_girl];
                } while ((album_voice_clip == album_last_voice_clip));
            }
            album_last_voice_clip = album_voice_clip;
            cd_audio_request_set(album_voice_clip_table[album_voice_clip * 2], album_voice_clip_table[album_voice_clip * 2 + 1], game_state.current_girl);
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

void album_playback_end_check(void) {
    if ((album_event_year == 0 && album_event_month == 0 && album_event_day == 0) || album_event_index == 0xFF) {
        mode_stage_next();
        return;
    }
    mode_step_next();
}

s32 album_playback_ui_restore(void) {
    s32 pad; /* FAKE: unused slot, the original frame has it (real source unknown). T-22019 */
    s32 buf[6];
    u32 i;

    switch (game_state.substep) {
    case 0:
        if (album_ui_reload_request > 0) {
            cd_read_sectors(0x69U, 0x801A0000, 0xB389);
            game_state.substep += 1;
        } else {
            game_state.substep += 1;
        }
        break;
    case 1:
        if (album_ui_reload_request == 0) {
            game_state.substep += 1;
        } else if (album_ui_reload_request > 0) {
            if (cd_read_sync_status() & 1) {
                memcpy((void *)0x801E2000, 0x801A0600, 0x18000);
                if (album_ui_reload_request == 1) {
                    album_ui_reload_request = 2;
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
        if (album_model_angle >= 0xC00) {
            mode_step_set(0);
        } else {
            album_model_angle += 8;
            album_model_matrix_update();
        }
        break;
    }
}

/* FAKE: retain the s32 fallthrough matching view so v0 stays live at the
 * epilogue and as1 hoists li t9,0xC00 instead of lui v0,0x8000 in case 5;
 * dummy entry read of unk_F5F seeds cvt_pass.py entry_locations so uopt
 * promotes unk_F5F to v1 in case 0. T-24129. */
s32 album_stage_finale(void) {
    u8 unused;

    unused = game_state.current_girl;
    if ((album_event_index == 0) || (album_skip_requested == 1)) {
        *(u8 *)&game_state.mode_stage_scratch = 5;
    }
    switch (*(u8 *)&game_state.mode_stage_scratch) {
    case 0:
        if ((u32) game_state.mode_stage_timer >= 0x79U) {
            if (game_state.current_girl == GIRL_SHIORI) {
                album_voice_play(0x14);
                if (album_voice_state >= 3) {
                    *(u8 *)&game_state.mode_stage_scratch += 1;
                    album_voice_state = 0;
                }
            } else if (game_state.current_girl == GIRL_YUINA) {
                album_voice_play(8);
                if (album_voice_state == 2) {
                    game_state.mode_stage_timer = 0;
                }
                if (album_voice_state >= 3) {
                    if ((u32) game_state.mode_stage_timer >= 0x3DU) {
                        *(u8 *)&game_state.mode_stage_scratch += 1;
                        album_voice_state = 0;
                    } else {
                        game_state.mode_stage_timer += 1;
                    }
                }
            } else if (game_state.current_girl == GIRL_SAKI) {
                album_voice_play(0xD);
                if (album_voice_state >= 3) {
                    *(u8 *)&game_state.mode_stage_scratch += 1;
                    album_voice_state = 0;
                }
            } else if (game_state.current_girl == GIRL_REI) {
                album_voice_play(7);
                if (album_voice_state >= 3) {
                    *(u8 *)&game_state.mode_stage_scratch += 1;
                    album_voice_state = 0;
                }
            } else {
                *(u8 *)&game_state.mode_stage_scratch += 1;
            }
        }
        break;
    case 1:
        cd_audio_request_set(album_finale_clip[game_state.current_girl * 2], album_finale_clip[game_state.current_girl * 2 + 1], game_state.current_girl);
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
        album_fade_white_draw(game_state.mode_stage_timer);
        if ((u32) game_state.mode_stage_timer >= 0x12DU) {
            album_skip_requested = 0;
            album_load_state = 0;
            album_event_index = 0;
            album_secret_mode = 0;
            album_phase = 0;
            *(s32 *)&bg_layers = 0x80000000;
            D_801220F4 = 0x80000000;
            album_model_angle = 0xC00;
            album_model_matrix_update();
            mode_stage_set(0xFF);
        }
        break;
    }
}

void album_vram_copy_to_backbuffer(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = draw_buffer * 0xF0;
    rect.w = 0x140;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0x140, 0x100);
    func_8009C674(0);
}

void album_vram_copy_other_buffer(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = (1 - draw_buffer) * 0xF0;
    rect.w = 0x140;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0, draw_buffer * 0xF0);
    func_8009C674(0);
}

s32 album_background_quads_draw(void) {
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
s32 album_photo_zoom_draw(s32 arg0, s32 arg1) {
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

void album_picture_sprites_draw(s32 arg0) {
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

void album_portrait_part_positions_set(u8 *arg0) {
    s32 pad; /* FAKE: unused slot above base, the original frame has it (T-3330 idiom, real source unknown). T-10710 */
    s16 base;
    u8 *p;
    s16 e1;
    s16 e2;

    if (album_event_girl == 0xA && album_event_season == 1) {
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

void album_fade_white_draw(arg0)
s32 arg0;
{
    AlbumPolyF4View *poly;
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

void album_date_counters_draw(void) {
    if (album_event_day < 0xA && album_event_month < 0xA) {
        album_number_sprite_draw(album_event_year, 0x50, 0x2D, 0);
        album_number_sprite_draw(-album_event_month, 0x64, 0x2D, 0);
        album_counter_backing_draw(0xE5, 0x3E);
    } else if (album_event_day >= 0xA && album_event_month < 0xA) {
        album_number_sprite_draw(album_event_year, 0x4B, 0x2D, 0);
        album_number_sprite_draw(-album_event_month, 0x5F, 0x2D, 0);
        album_counter_backing_draw(0xE0, 0x42);
    } else if (album_event_day < 0xA && album_event_month >= 0xA) {
        album_number_sprite_draw(album_event_year, 0x4B, 0x2D, 0);
        album_number_sprite_draw(-album_event_month, 0x64, 0x2D, 0);
        album_counter_backing_draw(0xE0, 0x42);
    } else if (album_event_day >= 0xA && album_event_month >= 0xA) {
        album_number_sprite_draw(album_event_year, 0x46, 0x2D, 0);
        album_number_sprite_draw(-album_event_month, 0x5F, 0x2D, 0);
        album_counter_backing_draw(0xDB, 0x47);
    }
    album_number_sprite_draw(-album_event_day, 0x78, 0x2D, 0);
}

/* Port of the TACO sibling func_80153E78 (T-17300). */
void album_counter_backing_draw(s32 a, s32 b) {
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
 * game_state.event_log[...] accesses CSE it with the earlier load. The second table is the AlbumLogEntry record array
 * at system_data + 0x58 (main_api.h): a byte pointer plus `[11]` after the scaled index keeps the +0x58 as the
 * displacement, while a typed base folds it into the address (T-22019). */
void album_event_record_load(void) {
    GameState *g = &game_state;
    u8 *w = (u8 *)&system_data;

    if (g->options_dirty == 0) {
        album_event_girl = g->event_log[album_event_index].girl;
        album_event_scene = g->event_log[album_event_index].flag;
        album_event_category = g->event_log[album_event_index].scene;
        album_event_mood = g->event_log[album_event_index].rank % 6U;
        album_event_year = g->event_log[album_event_index].year;
        album_event_month = g->event_log[album_event_index].month;
        album_event_day = g->event_log[album_event_index].day;
        album_event_weekday = g->event_log[album_event_index].weekday;
        switch (g->event_log[album_event_index].month) {
        case 3:
        case 4:
        case 5:
            album_event_season = 0;
            return;
        case 6:
        case 7:
        case 8:
            album_event_season = 1;
            return;
        case 9:
        case 10:
        case 11:
            album_event_season = 2;
            return;
        case 1:
        case 2:
        case 12:
            album_event_season = 3;
            return;
        }
    } else {
        album_event_girl = ((AlbumLogEntry *)(w + album_event_index * 8))[11].girl;
        album_event_scene = ((AlbumLogEntry *)(w + album_event_index * 8))[11].scene;
        album_event_category = ((AlbumLogEntry *)(w + album_event_index * 8))[11].category;
        album_event_mood = ((AlbumLogEntry *)(w + album_event_index * 8))[11].mood % 6U;
        album_event_year = ((AlbumLogEntry *)(w + album_event_index * 8))[11].year;
        album_event_month = ((AlbumLogEntry *)(w + album_event_index * 8))[11].month;
        album_event_day = ((AlbumLogEntry *)(w + album_event_index * 8))[11].day;
        album_event_weekday = ((AlbumLogEntry *)(w + album_event_index * 8))[11].weekday;
        switch (((AlbumLogEntry *)(w + album_event_index * 8))[11].month) {
        case 3:
        case 4:
        case 5:
            album_event_season = 0;
            return;
        case 6:
        case 7:
        case 8:
            album_event_season = 1;
            return;
        case 9:
        case 10:
        case 11:
            album_event_season = 2;
            return;
        case 1:
        case 2:
        case 12:
            album_event_season = 3;
        }
    }
}

s32 album_event_advance(s32 arg0) {
    switch (album_load_state) {
    case -1:
        break;
    case 0:
        album_event_record_load();
        if (album_event_find_valid() != 0) {
            album_event_resources_request(arg0);
            album_load_state += 1;
        } else {
            album_load_state = -1;
        }
        break;
    case 1:
        if ((album_event_is_portrait == 1) || (album_anim_active == 1)) {
            if (cd_read_sync_status() & 1) {
                dec_bg_reset();
                album_event_background_load(album_event_category, album_event_scene, album_event_girl, album_event_season);
                if ((album_event_has_background == 0) && (album_event_is_portrait == 1)) {
                    if (album_event_index < 0xFF) {
                        album_event_index += 1;
                    }
                    if ((album_secret_mode == 0) || (album_secret_mode == 2)) {
                        album_event_skip_other_girls();
                    }
                    album_load_state = 0;
                } else if (album_event_has_background == 1) {
                    album_load_state += 1;
                } else {
                    album_load_state = 3;
                }
            }
        } else if ((album_event_is_portrait == 0) && (album_anim_active == 0)) {
            if (album_event_index < 0xFF) {
                album_event_index += 1;
            }
            if ((album_secret_mode == 0) || (album_secret_mode == 2)) {
                album_event_skip_other_girls();
            }
            album_load_state = 0;
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            album_load_state += 1;
        }
        break;
    }
}

s32 album_event_find_valid(void) {
    if (album_event_year == 0 && album_event_month == 0 && album_event_day == 0) {
        while (1) {
            if (album_event_index < 0xFF) {
                album_event_index += 1;
            }
            album_event_record_load();
            if (album_event_year != 0 && album_event_month != 0 && album_event_day != 0) {
                return 1;
            }
            if (album_event_index == 0xFF) {
                return 0;
            }
        }
    }
    return 1;
}

s32 album_event_resources_request(s32 arg0) {
    s32 temp_v0;

    album_event_picture_select(album_event_category, album_event_scene, album_event_girl, album_event_season);
    if ((album_event_is_portrait == 0) && ((album_event_category == 2) || (album_event_category == 8))) {
        if ((album_event_category == 8) && (album_event_scene == 0xE)) {
            cd_read_sectors(0x71U, 0x801AE000, 0x8768);
            album_anim_active = 1;
            album_ui_reload_request = arg0;
        } else if ((album_event_category == 8) && (album_event_scene == 0x46)) {
            cd_read_sectors(0x71U, 0x801B0000, 0x85B6);
            album_anim_active = 1;
            album_ui_reload_request = arg0;
        } else if (album_event_category == 2) {
            temp_v0 = album_event_scene;
            cd_read_sectors(album_anim_sectors[temp_v0], 0x801B0000, album_anim_lba[temp_v0]);
            album_anim_active = 1;
            if (album_event_scene == 0x20) {
                album_ui_reload_request = arg0;
            }
        }
    }
}

void album_event_resources_show(void) {
    album_tpage_slots_reset();
    if (album_event_is_portrait == 1) {
        album_portrait_show();
    } else if (album_anim_active == 1) {
        album_anim_show_dispatch();
    }
}

void album_bustup_tables_init(void) {
    album_bustup_tpage_table = (u8 *)0x80180084;
    album_bustup_frame_table = (u8 *)0x80180098;
    album_bustup_sequence_table = (u8 *)0x8018009C;
    album_bustup_frame_count = *(s16 *)0x801960AC;
    album_bustup_palette = (u8 *)0x801800AC;
}

void album_portrait_show(void) {
    album_bustup_tables_init();
    sprite_quads_clear(0, 0x40);
    portrait_build(album_bustup_palette, album_bustup_tpage_table, album_bustup_frame_table, album_bustup_sequence_table, album_event_mood);
    album_portrait_part_positions_set(album_bustup_frame_table);
}

void album_portrait_face_update(void) {
    if (album_event_is_portrait == 1) {
        portrait_face_update();
    }
}

void album_event_hide_current(void) {
    if (album_event_is_portrait == 1) {
        album_portrait_hide();
    } else if (album_anim_active == 1) {
        album_anim_hide();
    }
}

void album_anim_hide(void) {
    album_anim_active = 0;
    bg_layers = 0x80000000;
    D_80120653 = 0;
    D_80120697 = 0;
    D_801206DB = 0;
}

void album_portrait_hide(void) {
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

void album_event_skip_other_girls(void) {
loop_1:
    album_event_record_load();
    if ((album_event_year == 0) && (album_event_month == 0) && (album_event_day == 0)) {
loop_4:
        if (album_event_index < 0xFF) {
            album_event_index += 1;
        }
        album_event_record_load();
        if ((album_event_year == 0) || (album_event_month == 0) || (album_event_day == 0)) {
            if (album_event_index == 0xFF) {
                album_load_state = -1;
            } else {
                goto loop_4;
            }
        }
    }
    if (album_event_girl != game_state.current_girl) {
        if (album_load_state == -1) {
            return;
        }
        if (album_event_index < 0xFF) {
            album_event_index += 1;
            goto loop_1;
        }
        if (album_event_index == 0xFF) {
            album_load_state = -1;
        } else {
            goto loop_1;
        }
    }
}

/* Called undeclared (implicit int) by album_playback_page_start and album_playback_photo; no value is returned. The search walks the
 * records down from album_event_index - 1 with the decrement before the tests (the original's pointer step comes before
 * the compares); the record and the selector are read through `g`, which keeps the two final unk_F5F reads
 * separate as in the original. T-25113. */
s32 album_special_girl_voice(void) {
    GameState *g = &game_state;
    u8 *w = (u8 *)&system_data;
    s32 i;
    s32 type;
    s32 a;
    u8 b;

    i = album_event_index;
    b = 0;
    a = 0;
    type = 0;
    if (g->options_dirty == 0) {
        if (album_secret_mode == 1 || album_secret_mode == 3) {
            type = g->event_log[i - 1].girl;
            a = g->event_log[i - 1].scene;
            b = g->event_log[i - 1].flag;
        } else if ((album_secret_mode == 0 || album_secret_mode == 2) && i >= 0) {
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
        type = ((AlbumLogEntry *)(w + album_prev_event_index * 8))[11].girl;
        a = ((AlbumLogEntry *)(w + album_prev_event_index * 8))[11].category;
        b = ((AlbumLogEntry *)(w + album_prev_event_index * 8))[11].scene;
    }
    if (g->current_girl == GIRL_REI && type == 0xC) {
        if (a == 4) {
            album_voice_play(1);
        } else if (a == 6) {
            album_voice_play(2);
        } else if (a == 7) {
            album_voice_play(3);
        } else if (a == 2 && b == 0x4F) {
            album_voice_play(5);
        } else if (a == 2 && b == 0x50) {
            album_voice_play(4);
        } else if (a == 2 && b == 0x51) {
            album_voice_play(6);
        }
    } else if (g->current_girl == GIRL_MIHARU && type == 0xB) {
        if (a == 2 && b == 0x4A) {
            album_voice_play(0);
        } else if ((a == 2 && b == 0x4C) || (a == 0xB && b != 0x1E)) {
            album_voice_play(1);
        } else if ((a == 2 && b == 0x4E) || (a == 0xB && b == 0x1E)) {
            album_voice_play(2);
        } else if (a == 0x12) {
            album_voice_play(3);
        }
    }
}

/* T-24129: retain the earlier caller's K&R implicit-int view (album_stage_finale). */
s32 album_voice_play(s32 arg0) {
    switch (album_voice_state) {
    case 0:
        cd_audio_request_set(album_voice_clip_table[arg0 * 2], album_voice_clip_table[arg0 * 2 + 1], game_state.current_girl);
        audio_command(0x300);
        album_voice_state += 1;
        break;
    case 1:
        if (cd_audio_state() & 4) {
            album_voice_state += 1;
        }
        break;
    case 2:
        if (cd_audio_state() & 2) {
            album_voice_state += 1;
        }
        break;
    }
}

s32 album_event_is_special_portrait(void) {
    if (album_event_category == 0x15 &&
        (album_event_scene == 6 || album_event_scene == 7 || album_event_scene == 8 || album_event_scene == 9 || album_event_scene == 10)) {
        if ((album_event_girl == 0 || album_event_girl == 1 || album_event_girl == 4 || album_event_girl == 6 || album_event_girl == 9 ||
             album_event_girl == 10) &&
            album_event_mood == 5) {
            return 1;
        }
    }
    return 0;
}

void album_special_portrait_setup(void) {
    /* FAKE: unsigned halfword stores prevent IDO sharing constant 7 with the byte selector. T-9750. */
    album_tpage_slots_reset();
    album_portrait_tables_init();
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
    D_8012065C = album_portrait_frame_table;
    D_80120660 = album_portrait_sequence_table;
    D_80120684 = album_portrait_tpage_table;
    D_80120664 = album_portrait_frame_count;
    D_80120652 = 1;
    D_80120653 = 0x84;
    D_80120668 = 2;
    if (album_event_girl == 0) {
        if ((album_event_month == 6) || (album_event_month == 7) || (album_event_month == 8) || (album_event_month == 9)) {
            D_80120666 = 0;
        } else {
            D_80120666 = 1;
        }
    }
    if (album_event_girl == 1) {
        if ((album_event_month == 6) || (album_event_month == 7) || (album_event_month == 8) || (album_event_month == 9)) {
            D_80120666 = 2;
        } else {
            D_80120666 = 3;
        }
    }
    if (album_event_girl == 4) {
        if ((album_event_month == 6) || (album_event_month == 7) || (album_event_month == 8) || (album_event_month == 9)) {
            D_80120666 = 4;
        } else {
            D_80120666 = 5;
        }
        if (album_event_weekday == 0xFF) {
            *(u16 *)&sprite_quads[0].unk_0C = 7;
            sprite_quads[0].unk_0E = 0x70;
        }
    }
    if (album_event_girl == 6) {
        if ((album_event_month == 6) || (album_event_month == 7) || (album_event_month == 8) || (album_event_month == 9)) {
            D_80120666 = 6;
        } else {
            D_80120666 = 7;
            D_8012067A = -0x12;
        }
    }
    if (album_event_girl == 9) {
        if ((album_event_month == 6) || (album_event_month == 7) || (album_event_month == 8) || (album_event_month == 9)) {
            D_80120666 = 8;
        } else {
            D_80120666 = 9;
        }
        if (album_event_weekday == 0xFF) {
            *(u16 *)&sprite_quads[0].unk_0C = 7;
            sprite_quads[0].unk_0E = 0x70;
        }
    }
    if (album_event_girl == 0xA) {
        if ((album_event_month == 6) || (album_event_month == 7) || (album_event_month == 8) || (album_event_month == 9)) {
            D_80120666 = 0xA;
        } else {
            D_80120666 = 0xB;
        }
    }
}

void album_debug_fill_miharu(void) {
    /* FAKE: record view spans legacy scalar declarations; canonical main BSS aggregate is pending. T-9750. */
    /* FAKE: loop store grouped on header line retains as1 unrolled-store order. T-9750. */
    s32 i;

    for (i = 0; i < 13; i++) { game_state.girl_cleared[i] = 1; }
    game_state.girl_cleared[11] = 5;
    game_state.current_girl = GIRL_MIHARU;
    ((AlbumLogEntry *)&system_data)[11].girl = 0xB;
    ((AlbumLogEntry *)&system_data)[11].category = 0xB;
    ((AlbumLogEntry *)&system_data)[11].scene = 0x1D;
    ((AlbumLogEntry *)&system_data)[11].mood = 0;
    ((AlbumLogEntry *)&system_data)[11].year = 0x60;
    ((AlbumLogEntry *)&system_data)[11].month = 4;
    ((AlbumLogEntry *)&system_data)[11].day = 0x19;
    ((AlbumLogEntry *)&system_data)[11].weekday = 0;
    ((AlbumLogEntry *)&system_data)[12].girl = 0xB;
    ((AlbumLogEntry *)&system_data)[12].category = 0xB;
    ((AlbumLogEntry *)&system_data)[12].scene = 0x1E;
    ((AlbumLogEntry *)&system_data)[12].mood = 0;
    ((AlbumLogEntry *)&system_data)[12].year = 0x60;
    ((AlbumLogEntry *)&system_data)[12].month = 8;
    ((AlbumLogEntry *)&system_data)[12].day = 0x19;
    ((AlbumLogEntry *)&system_data)[12].weekday = 0;
    ((AlbumLogEntry *)&system_data)[13].girl = 0xB;
    ((AlbumLogEntry *)&system_data)[13].category = 0xB;
    ((AlbumLogEntry *)&system_data)[13].scene = 0xB;
    ((AlbumLogEntry *)&system_data)[13].mood = 0;
    ((AlbumLogEntry *)&system_data)[13].year = 0x60;
    ((AlbumLogEntry *)&system_data)[13].month = 7;
    ((AlbumLogEntry *)&system_data)[13].day = 0x19;
    ((AlbumLogEntry *)&system_data)[13].weekday = 0;
    ((AlbumLogEntry *)&system_data)[14].girl = 0xB;
    ((AlbumLogEntry *)&system_data)[14].category = 0xB;
    ((AlbumLogEntry *)&system_data)[14].scene = 0xA;
    ((AlbumLogEntry *)&system_data)[14].mood = 0;
    ((AlbumLogEntry *)&system_data)[14].year = 0x60;
    ((AlbumLogEntry *)&system_data)[14].month = 2;
    ((AlbumLogEntry *)&system_data)[14].day = 0x19;
    ((AlbumLogEntry *)&system_data)[14].weekday = 0;
    ((AlbumLogEntry *)&system_data)[15].girl = 0xB;
    ((AlbumLogEntry *)&system_data)[15].category = 0x12;
    ((AlbumLogEntry *)&system_data)[15].scene = 0;
    ((AlbumLogEntry *)&system_data)[15].mood = 0;
    ((AlbumLogEntry *)&system_data)[15].year = 0x60;
    ((AlbumLogEntry *)&system_data)[15].month = 5;
    ((AlbumLogEntry *)&system_data)[15].day = 0x19;
    ((AlbumLogEntry *)&system_data)[15].weekday = 0;
}
