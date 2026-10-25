/* movie_player.c: src/main/800563F0.c; T-27059: object [0x800563F0,0x80057390); STR movie setup/playback, double-buffered MDEC decode/sync, CD reads, frame loop and timed subtitle text; preserve complete object (reuse h-cdio candidate). Original object 0x800563F0-0x80057390 (config/objects/main.txt). */
#define MAIN_API_OVERRIDE_func_8008B750 /* original narrows both volume arguments (movie_play); T-25118 */
#define MAIN_API_OVERRIDE_func_8008BFB0 /* original narrows both volume arguments (movie_play); T-25118 */
/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this file was matched with. */

#include "common.h"
#include "common.h"
#include "game_enums.h"
#include "libcd.h"
#include "libetc.h"
#include "libgpu.h"
#include "libgs.h"
#include "libpress.h"
#include "libsnd.h"
#include "main/boot.h"
#include "main/draw.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/scenes.h"
#include "main/movie.h"
#include "main/pad.h"
#include "main/prim.h"
#include "main/state.h"
#include "main/text.h"
#include "main/vram.h"
#include "libcd.h"
#include "psx_types.h"
#include "main/pad.h"
#include "main/movie.h"

void func_8008B750(s16, s16);
void func_8008BFB0(s32, s16, s16);

/* Sets the x and y shift applied to the movie picture in VRAM. */
void set_movie_offset(s16 arg0, s16 arg1) {
    movie_offset_x = arg0;
    movie_offset_y = arg1;
}

/* Resets the MDEC, installs the decode callback, sets up the CD stream ring and starts the stream (PsyQ movie sample). */
void strInit(s32 arg0, s32 arg1) {
    DecDCTReset(0);
    movie_context->end_flag = 0;
    func_800869C8(arg1);
    func_80088150((u8 *)movie_context + 0x44, 0x20);
    func_80088180(movie_flags & 1, 1, -1, 0, 0);
    strKickCD(arg0);
}

/* Fills the decode environment: two VLC buffers, image buffer, two target rectangles and the slice size for 15 or 24 bit. */
void strSetDefDecEnv(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u32 width;
    u32 height;
    MovieDecodeEnvironment *env;

    env = arg0;
    env->vlcbuf[0] = (u8 *) movie_context + 0x12D44;
    env->vlcbuf[1] = (u8 *) movie_context + 0x48D44;
    env->vlcid = 0;
    env->imgbuf = (u8 *) movie_context + 0x10044;
    env->rectid = 0;
    env->isdone = 0;
    width = 0x140;
    height = 0xF0;
    if (movie_flags & 1) {
        width = 0x1E0;
        height = 0xF0;
        /* FAKE: one source line gives as1 the original halfword-store order. T-9930. */
        env->rect[0].w = width; env->rect[0].h = height;
        env->rect[0].x = arg1;
        env->rect[0].y = arg2;
        env->rect[1].x = arg3;
        env->rect[1].y = arg4;
        env->rect[1].h = height;
        env->rect[1].w = width;
        env->slice.w = 0x18;
        env->slice.h = height;
        env->slice.y = arg2;
        env->slice.x = arg1;
        return;
    }
    env->rect[0].x = arg1;
    env->rect[0].y = arg2;
    env->rect[0].w = width;
    env->rect[0].h = height;
    env->rect[1].x = arg3;
    env->rect[1].y = arg4;
    env->rect[1].h = height;
    env->rect[1].w = width;
    env->slice.w = 0x10;
    env->slice.h = height;
    env->slice.y = arg2;
    env->slice.x = arg1;
}

/* MDEC output callback: copies a decoded slice to VRAM, steps along the row, flips the target rectangle at frame end. */
void strCallback(void) {
    RECT rect;

    /* FAKE: T-26000: never-true arm that ugen drops (as1 deletes the dead byte load). Its
     * indirect store puts the RECT copy below in block-copy state F (tools/ugen_pass.py), as in
     * the original (one scratch register before the second copy word; 75 rows without it).
     * IDO 4.1's ugen gives this body F without the arm, because it also sets F at a branch on a
     * value (`movie_flags & 1`); the pass does not model that and counts the store of an arm ugen
     * drops, which 4.1 ignores (tools/ido_eval_cases/ugen_copy_state/copy_dropped_arm_store.c).
     * Still needed in T-26002. */
    if ((u32)D_800B0896 < 0) {
        *(u8 *)0 = 0;
    }
    if ((movie_flags & 1) && D_80125C5C != 0) {
        func_80088210();
        D_80125C5C = 0;
    }
    rect = movie_context->decode_environment.slice;
    if (movie_flags & 1) {
        rect.x += (movie_offset_x * 3 + 2) / 3;
        rect.y += (movie_offset_y * 3 + 2) / 3;
    } else {
        rect.x += movie_offset_x;
        rect.y += movie_offset_y;
    }
    func_8009C884(&rect, movie_context->decode_environment.imgbuf);
    func_8009C674(0);
    movie_context->decode_environment.slice.x += movie_context->decode_environment.slice.w;
    if (movie_context->decode_environment.slice.x < movie_context->decode_environment.rect[movie_context->decode_environment.rectid].x + movie_context->decode_environment.rect[movie_context->decode_environment.rectid].w) {
        func_80086944(movie_context->decode_environment.imgbuf, movie_context->decode_environment.slice.w * movie_context->decode_environment.slice.h / 2);
        return;
    }
    movie_context->decode_environment.isdone = 1;
    if (movie_context->decode_environment.rectid != 0) {
        movie_context->decode_environment.rectid = 0;
    } else {
        movie_context->decode_environment.rectid = 1;
    }
    movie_context->decode_environment.slice.x = movie_context->decode_environment.rect[movie_context->decode_environment.rectid].x;
    movie_context->decode_environment.slice.y = movie_context->decode_environment.rect[movie_context->decode_environment.rectid].y;
}

/* Waits for the next frame bitstream, swaps the VLC buffer and starts VLC decoding; -1 on timeout. */
s32 strNextVlc(MovieDecodeEnvironment *arg0) {
    s32 vlc;
    s32 timeout;

    timeout = 0x800000;
    vlc = strNext(arg0);
    while (vlc == 0) {
        if (--timeout == 0) {
            return -1;
        }
        func_80088CB0(vlc);
        vlc = strNext(arg0);
    }
    if (arg0->vlcid != 0) {
        arg0->vlcid = 0;
    } else {
        arg0->vlcid = 1;
    }
    func_80086870(vlc, arg0->vlcbuf[arg0->vlcid]);
    func_80088CB0(vlc);
    return 0;
}

/* Takes the next frame from the CD stream ring, checks its header, updates the movie size and frame number. */
s32 strNext(MovieDecodeEnvironment *env) {
    u32 *addr;
    StHEADER *sector;
    s32 cnt;
    RECT rect;
    s32 w;

    cnt = 0x800000;
    while (func_80088D70(&addr, &sector) != 0) {
        if (--cnt == 0) {
            return 0;
        }
    }
    if (sector->frameCount >= movie_end_frame) {
        movie_context->end_flag = 1;
    }
    if (sector->headm != addr[0] || sector->headv != addr[1]) {
        func_80088CB0(addr);
        return 0;
    }
    movie_last_frame = sector->frameCount;
    w = movie_context->width;
    /* FAKE: `w * 1` keeps the sector width first in the bne (a plain w would go first); the local
     * holds $v0 so the width takes $a0. T-17014 */
    if (w * 1 != sector->width || movie_context->height != sector->height) {
        rect.x = 0;
        rect.y = 0;
        rect.w = 0x280;
        rect.h = 0x1E0;
        func_8009C7F8(&rect, 0, 0, 0);
        movie_context->width = sector->width;
        movie_context->height = sector->height;
    }
    if (movie_flags & 1) {
        env->rect[0].w = env->rect[1].w = movie_context->width * 3 / 2;
    } else {
        env->rect[0].w = env->rect[1].w = movie_context->width;
    }
    env->rect[0].h = env->rect[1].h = movie_context->height;
    env->slice.h = movie_context->height;
    return (s32)addr;
}

/* Implicit-int return type with no return statement: it changes how as1 fills the loop's
 * back-branch delay slot (T-9140, found by the permuter in T-1330). */
/* Waits for the decode callback to finish the frame; on timeout, marks it finished and switches the decode rectangle. */
u32 strSync(MovieDecodeEnvironment *arg0, s32 arg1) {
    volatile s32 timeout = 0x800000;

    while (arg0->isdone == 0) {
        if (--timeout == 0) {
            arg0->isdone = 1;
            if (arg0->rectid != 0) {
                arg0->rectid = 0;
            } else {
                arg0->rectid = 1;
            }
            arg0->slice.x = arg0->rect[arg0->rectid].x;
            arg0->slice.y = arg0->rect[arg0->rectid].y;
        }
    }
    arg0->isdone = 0;
}

/* Seeks to the movie sector and starts the streaming read. */
void strKickCD(s32 arg0) {
    while (func_800879D0(0x15, arg0, 0) == 0) {
    }
    while (func_800880C0(0x1C0) == 0) {
    }
}

/* The PsyQ movie sample's player loop (CdSearchFile retry, DecDCTin/DecDCTout, double-buffered
 * DISPENV/DRAWENV) on the MovieStreamContext at 0x80180000; arg0 is the file name, arg1 the end frame and arg2
 * the start sector (CdIntToPos when it is not 0). Returns 0 when the file is not found; the normal
 * exit has no return statement, as in the original (v0 is left by the last call). T-17014, T-25118 */
s32 movie_play(s32 movie_name_or_zero, s32 requested_end_frame, s32 movie_start_sector) {
    s32 frames;
    s16 vol;
    DISPENV disp;
    DRAWENV draw;
    s32 i;
    s32 id;
    s32 pad[5]; /* FAKE: measured frame homes (draw at sp+0x90, file at sp+0x5C), not a source recovery. T-25118 */
    CdlFILE file;
    s32 pad2; /* FAKE: measured frame home (start at sp+0x54), not a source recovery. T-25118 */
    u32 start;

    movie_end_frame = requested_end_frame;
    DecDCTReset(0);
    func_800869C8(0);
    func_800869A4(0);
    func_8009C210(1);
    if (draw_buffer == 0) {
        func_8009AC20();
        boot_frame_begin();
    }
    movie_last_frame = 0;
    movie_context = (MovieStreamContext *)0x80180000;
    movie_context->width = 0x140;
    movie_context->height = 0xF0;
    frames = 0;
    i = 0;
    if (movie_start_sector == 0) {
        while (func_80088E34(&file, movie_name_or_zero) == 0) {
            if (i++ >= 0x10000) {
                return 0;
            }
        }
        movie_context->start_position[0] = file.pos[0];
        movie_context->start_position[1] = file.pos[1];
        movie_context->start_position[2] = file.pos[2];
    } else {
        func_80087EE8(movie_start_sector, movie_context);
    }
    strSetDefDecEnv(&movie_context->decode_environment, 0, 0, 0, 0xF0);
    strInit((s32)movie_context, (s32)strCallback);
    strNextVlc(&movie_context->decode_environment);
    func_8008BEE0(0, 0, 1);
    func_8008BFB0(0, 0x7FFF, 0x7FFF);
    SetDefDispEnv(&disp, movie_context->decode_environment.rect[1].x, movie_context->decode_environment.rect[1].y, movie_context->decode_environment.rect[1].w, movie_context->decode_environment.rect[1].h);
    func_8009F57C(&draw, movie_context->decode_environment.rect[1].x, movie_context->decode_environment.rect[1].y, movie_context->decode_environment.rect[1].w, movie_context->decode_environment.rect[1].h);
    if (movie_flags & 1) {
        disp.isrgb24 = 1;
        disp.disp.w = disp.disp.w * 2 / 3;
    }
    func_8009CCF4(&disp);
    func_8009CC04(&draw);
    start = game_state.timer_ticks;
    for (;;) {
        func_800868BC(movie_context->decode_environment.vlcbuf[movie_context->decode_environment.vlcid], (movie_flags & 1) + 2);
        func_80086944(movie_context->decode_environment.imgbuf, movie_context->decode_environment.slice.w * movie_context->decode_environment.slice.h / 2);
        strNextVlc(&movie_context->decode_environment);
        if (!(movie_flags & 1)) {
            boot_frame_begin();
            movie_captions_update((game_state.timer_ticks - start) / 6);
            actor_draw_all();
            disp_string();
            func_8009AC20();
            func_8009AD00((u8 *)&draw_ot_table + draw_buffer * 0x14);
        }
        strSync(&movie_context->decode_environment, 0);
        func_800AD950(0);
        id = movie_context->decode_environment.rectid ? 0 : 1;
        SetDefDispEnv(&disp, movie_context->decode_environment.rect[id].x, movie_context->decode_environment.rect[id].y, movie_context->decode_environment.rect[id].w, movie_context->decode_environment.rect[id].h);
        func_8009F57C(&draw, movie_context->decode_environment.rect[id].x, movie_context->decode_environment.rect[id].y, movie_context->decode_environment.rect[id].w, movie_context->decode_environment.rect[id].h);
        if (movie_flags & 1) {
            disp.isrgb24 = 1;
            disp.disp.w = disp.disp.w * 2 / 3;
        }
        func_8009CCF4(&disp);
        func_8009CC04(&draw);
        func_8009C5E0(1);
        /* FAKE: the empty test adds two uopt basic blocks inside the loop; with them the first use of a
         * new saved register costs more than keeping &draw or movie_name_or_zero in one, as in the original. T-25118 */
        if (movie_last_frame) {
        }
        if (!(movie_last_frame < requested_end_frame - 3 && frames < requested_end_frame - 0xF)) {
            break;
        }
        game_state.frame_count += 1;
        frames++;
        if (movie_context->end_flag == 1) {
            break;
        }
        pad_update();
        if (game_state.pad_pressed & 0x860) {
            break;
        }
    }
    disp.isrgb24 = 0;
    func_8009CCF4(&disp);
    func_8009C5E0(0);
    vram_clear_framebuffers();
    for (vol = 0x7F; vol > 0; vol -= 10) {
        func_8008B750(vol, vol);
        func_8008BFB0(0, vol, vol);
    }
    func_8009C5E0(1);
    func_8009C674(0);
}

void movie_caption_bar_draw(s32 arg0, u8 arg1) {
    s32 t;

    t = arg1;
    prim_gradient_rect(-0x90, 0x5B, 0x90, 0xE, 4, arg0, t);
    prim_gradient_rect(0, 0x5B, 0x90, 0xE, 4, arg0, t);
    prim_gradient_rect(-0x90, 0x69, 0x100, 0x10, 1, 0, 1);
}

void movie_captions_update(s32 arg0) {
    /* FAKE: unused scalar reserving the original's fourth stack slot (declared first = highest). T-17014 */
    s32 pad;
    s32 d;
    s32 i;
    u8 *p;

    p = movie_caption_marks;
    for (i = 0; i < 22; i++) {
        if (arg0 < (s32)(*p * 0x50 / 0xF)) {
            break;
        }
        p++;
    }
    if (i != 0) {
        k_disp_switch(i - 1, 0);
        k_disp_start(i + 1);
    }
    if (p >= &movie_caption_marks[4] && p < &movie_caption_marks[9]) {
        hud_corner_icon_set(0);
    } else {
        hud_corner_icon_set(1);
    }
    if (p >= &movie_caption_marks[21]) {
        hud_corner_icon_set(0);
        movie_caption_bar_draw(0, 1);
        text_system_reset();
        return;
    }
    if (movie_offset_x != 0) {
        d = *p * 0x50 / 0xF - arg0;
        if (d == 4 || d == 5) {
            movie_caption_bar_draw((movie_flags + D_800E7D35) * 0xDF7FCF + 0xFF208031, 1);
        } else {
            movie_caption_bar_draw((movie_flags + D_800E7D35) * 0xDF7FCF + 0xFF208031, 0x81);
        }
    }
}
