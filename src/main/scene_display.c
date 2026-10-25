/* scene_display.c: src/main/80085E30.c; T-27059: original object [0x80085E30,0x80086810); scene display reset, VBlank timer, texture scroll, 2D rotation, fades, finger cursor and display helpers, plus small portrait-read/state helpers; keep whole mixed object. Also retains best-girl selection at 0x8008647C and girl_state_rank; these are explicit non-display helpers in this mixed object. Original object 0x80085E30-0x80086810 (config/objects/main.txt). */
#include "common.h"
#include "common.h"
#include "game_enums.h"
#include "libgpu.h"
#include "libgte.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/cd.h"
#include "main/cursor.h"
#include "main/draw.h"
#include "main/game_mode.h"
#include "main/girl.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/scenes_school_events.h"
#include "main/message.h"
#include "main/portrait.h"
#include "main/prim.h"
#include "main/scene.h"
#include "main/select_girl.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "main/tpage.h"
#include "main/vsync.h"
#include "psx_types.h"
#include "main/game_mode.h"

void scene_screen_reset(s32 arg0, s32 arg1) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_upper();
    back_clear_switch(arg0);
    game_state.clear_color_r = 0;
    game_state.clear_color_g = 0;
    game_state.clear_color_b = 0;
    game_state.screen_brightness = 0x80;
    text_system_reset();
    game_state.tile_layer_enabled = 0;
    if (arg1 != 0) {
        tpage_buf_clear_all();
        return;
    }
    tpage_buf_clear();
}

/* Starts the VBLANK timer interval. */
void Vblnk_Timer_Init(void) {
    vsync_timer_start = game_state.timer_ticks;
    vsync_timer_elapsed = 0;
}

/* Updates and returns elapsed ticks in the VBLANK timer interval. */
s32 Vblnk_Timer(void) {
    vsync_timer_elapsed = game_state.timer_ticks - vsync_timer_start;
    return vsync_timer_elapsed;
}

/* Emits textured GPU quads for the scrolling background effect. */
void Scroll(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4) {
    POLY_FT4 *p;
    s32 i;
    u16 lo;
    s32 pad1;
    s32 pad2;

    for (i = 0; i < 5; i++) {
        p = (POLY_FT4 *)GetWorkBase(0x28, draw_buffer);
        func_8009F040(p);
        SetSemiTrans(p, 0);
        SetShadeTex(p, 0);
        p->r0 = game_state.screen_brightness;
        p->g0 = game_state.screen_brightness;
        p->b0 = game_state.screen_brightness;
        lo = (arg0 * 2 + (i % 2) * 0x40) & 0x7F;
        p->u1 = lo + 0x40;
        p->u3 = lo + 0x40;
        p->u0 = lo;
        p->u2 = lo;
        /* FAKE: dead stores to pad1/pad2 order the loop-invariant expression bit positions and stack home slots (`i * 0x20` before `i * 0x40`, `(u32)arg4` at 0x60($sp)). T-24127 */
        pad1 = i * 0x20;
        pad2 = (u32)arg4;
        p->v3 = arg2 + arg3;
        p->v2 = arg2 + arg3;
        p->v1 = arg2;
        p->v0 = arg2;
        p->x0 = game_state.draw_origin_x[draw_buffer] + i * 0x40 - 0xA0;
        p->y0 = game_state.draw_origin_y[draw_buffer] + arg2 - 0x78;
        p->x1 = game_state.draw_origin_x[draw_buffer] + i * 0x40 - 0x60;
        p->y1 = game_state.draw_origin_y[draw_buffer] + arg2 - 0x78;
        p->x2 = game_state.draw_origin_x[draw_buffer] + i * 0x40 - 0xA0;
        p->y2 = game_state.draw_origin_y[draw_buffer] + arg2 + arg3 - 0x78;
        p->x3 = game_state.draw_origin_x[draw_buffer] + i * 0x40 - 0x60;
        p->y3 = game_state.draw_origin_y[draw_buffer] + arg2 + arg3 - 0x78;
        p->tpage = func_8009ECB0(1, 0, (((u32)(i * 0x20) + (u32)arg0) / 0x40 * 0x40) % (u32)arg4 + 0x140, 0);
        p->clut = func_8009ED74(0, 0x1F0);
        AddPrim(draw_ot_tags + (draw_buffer << 10) + arg1 * 4, p);
    }
    /* FAKE: raises `(u8)arg2` priority above `arg3` and `arg2 + arg3` so `$s6 = arg2, $s7 = arg3, $s8 = arg2 + arg3`. T-24127 */
    if ((u8)arg2) {}
}

/* Rotates a two-dimensional point using the engine sine and cosine values. */
void Rot_2D(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
    s32 s;
    s32 c;

    s = rsin();
    c = func_800A0140(arg0);
    *arg3 = c * *arg1 / 4096 - *arg2 * s / 4096;
    *arg4 = s * *arg1 / 4096 + *arg2 * c / 4096;
}

/* Starts a new fade-out timer interval. */
void Fade_Out_Init(void) {
    Vblnk_Timer_Init();
}

/* temp: the byte is read into a local after the timer test; with a direct compare IDO loads it into $v0 and puts the li v0,1 in the branch delay slot (T-17022, matching-notes) */
/* Lowers screen brightness to zero and returns when the fade-out is complete. */
s32 Fade_Out_Main(void) {
    u8 temp;
    if (Vblnk_Timer() != 0) {
        temp = game_state.screen_brightness;
        if (temp < 3U) {
            game_state.screen_brightness = 0;
            return 1;
        }
        game_state.screen_brightness -= 4;
        Vblnk_Timer_Init();
        return 0;
    }
    return 0;
}

/* Starts a new fade-in timer interval. */
void Fade_In_Init(void) {
    Vblnk_Timer_Init();
}

/* temp: same byte local as Fade_Out_Main (T-17022, matching-notes) */
/* Raises screen brightness to 0x80 and returns when the fade-in is complete. */
s32 Fade_In_Main(void) {
    u8 temp;
    if (Vblnk_Timer() != 0) {
        temp = game_state.screen_brightness;
        if (temp >= 0x7EU) {
            game_state.screen_brightness = 0x80;
            return 1;
        }
        game_state.screen_brightness += 4;
        Vblnk_Timer_Init();
        return 0;
    }
    return 0;
}

/* Updates the cursor actor position from the current HUD cursor. */
void Yubi(s32 idx) {
    u8 *p;
    hud_cursor_update();
    p = actor_pool + idx * 0x44;
    *(s16 *)(p + 0x26) = cursor_x;
    *(s16 *)(p + 0x2A) = cursor_y;
}

/* Runs the common per-frame scene and message display updates. */
void Default_Disp(void) {
    k_disp_inc2();
    check_k_scroll();
    scene_frame_update();
    message_window_show();
    hizuke_show();
    hud_screen_frame_draw(2);
    message_window_title_update();
    draw_shake_update();
}

s32 select_girl_by_scores(void) {
    s32 sum1;
    s32 sum2;
    s32 cur;
    s32 list[14]; /* FAKE: 14-word list keeps list at 0x6C($sp) in the 0xB0 frame alongside sum1/sum2/cur. T-24111 */
    u32 n;
    s32 i;
    s32 best;
    s32 bscore;
    s32 score;
    u32 t;

    n = 0;
    for (i = 0; i < 11; i++) {
        if (game_state.girls[i].flags.f.met == 1) {
            list[n] = i;
            n = (n + 1) & 0xFF;
        }
    }
    t = get_h_yuukou(GIRL_SHIORI) & 0xFF;
    bscore = get_h_tokimeki(GIRL_SHIORI) + t;
    best = 0;
    for (i = 1; i < n; i++) {
        t = get_h_tokimeki(list[i]) & 0xFF;
        score = get_h_yuukou(list[i]) + t;
        if (bscore < score && (t = girl_state_rank(best) & 0xFF, t >= girl_state_rank(list[i]))) {
            best = list[i];
            bscore = score;
        } else if (bscore == score) {
            t = girl_state_rank(best) & 0xFF;
            if (t >= girl_state_rank(list[i])) {
                cur = list[i];
                /* FAKE: explicit (s32) cast on unk_06 evaluates unk_06 before unk_02 while keeping the sum in $a0/$a1. T-24111 */
                sum1 = game_state.girls[best].tokimeki + (s32)game_state.girls[best].affection;
                sum2 = game_state.girls[cur].tokimeki + (s32)game_state.girls[cur].affection;
                if (sum1 < sum2) {
                    bscore = score;
                    best = cur;
                }
            }
        }
    }
    return best;
}

/* FAKE: same-line swap and pointer read retain the original halfword load order. T-9550 */
s32 actor_pair_swap_sequences(void)
{

  s16 *right;
  s16 old_right;
  s16 old_left;
  old_left = D_80120666; right = &D_801206EE; old_right = *right; D_80120666 = old_right; D_801206EE = old_left;
  D_80120668 = 0;
  D_801206F0 = 0;
  D_80120658 = 0;
}

s32 girl_state_rank(s32 a) {
    /* FAKE: the (u8) cast reserves the extra temp the original has (same as GYOZI func_80140D7C); real return type unknown. T-4060 */
    return (u8)get_g_zyotai_s(a) & 0x7F;
}

void portrait_read_step(void) {
    if (game_state.substep == 0) {
        portrait_read_start();
        game_state.substep += 1;
        return;
    }
    if (cd_read_sync_status() & 1) {
        if (*(s32 *)face_images != 0x801820AC || *(s32 *)(face_images + 4) != 0x801860AC || *(s32 *)(face_images + 8) != 0x8018A0AC || *(s32 *)(face_images + 0xC) != 0x8018E0AC || *(s32 *)(face_images + 0x10) != 0x801920AC || *(s32 *)face_sequences != 0x8018005C || *(s32 *)(face_sequences + 4) != 0x80180070 || *(s32 *)face_frame_tables != 0x80180000) {
            game_state.substep = 0;
            return;
        }
        if (cd_file_checksum_ok(0x2D, 0x80180000) == 0) {
            game_state.substep = 0;
            return;
        }
        mode_step_next();
    }
}

void audio_bank_wait_latch(void) {
    if (audio_bank_wait_done == 0) {
        if (audio_bank_load_poll() == 1) {
            audio_bank_wait_done = 1;
        }
    }
}
