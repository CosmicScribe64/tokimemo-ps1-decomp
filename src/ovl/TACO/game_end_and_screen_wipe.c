/* game_end_and_screen_wipe.c: src/ovl/TACO/8013B170.c:4: end-stage dispatch, screen snapshot and progressive wipe drawing; body 5: switch (game_state.mode_step) {; 7: shooter_game_end_step_init();. Original object 0x8013B170-0x8013B990 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

void shooter_stage_game_end(void) {
    switch (game_state.mode_step) {
    case 0:
        shooter_game_end_step_init();
        break;
    case 1:
        shooter_game_end_step_wait();
        break;
    case 2:
        shooter_game_end_step_leave();
        break;
    }
    if (game_state.mode_step != 0) {
        if (game_state.frame_count & 0x10) {
            if ((u8) shooter_stages_cleared >= 3U) {
                shooter_draw_text((s32) "MISSION COMPLETE", -0x40, 0, 2);
            } else if (shooter_time_left == 0) {
                shooter_draw_text((s32) "TIME OVER", -0x40, 0, 1);
            } else {
                shooter_draw_text((s32) "GAME OVER", -0x40, 0, 1);
            }
        }
        shooter_draw_screen_wipe(0x64);
    } else {
        shooter_draw_hud();
    }
    shooter_process_load_request();
}

void shooter_game_end_step_init(void) {
    s32 pad; /* FAKE: unused 4-byte local gives frame 0x40 and puts flag at sp+0x38; real source unknown. T-19030 */
    s32 flag;
    ShooterPosition save;

    if (shooter_load_request == 0) {
        shooter_snapshot_screen();
        shooter_hide_all_models();
        view_model_slots_clear();
        save = *(ShooterPosition *)&shooter_actors->argument_x;
        flag = shooter_actors->stats[1];
        shooter_release_all_actor_models();
        *(ShooterPosition *)&shooter_actors->argument_x = save;
        shooter_actors->stats[1] = flag;
        /* FAKE: element chain through the first symbol shares one li between the last two stores and
         * keeps a separate lui per store (struct-field chains and separate statements load three
         * constants); GameState fields unk_036..unk_038. T-19030 */
        (&game_state.clear_color_r)[0] = (&game_state.clear_color_r)[1] = (&game_state.clear_color_r)[2] = 0x10;
        shooter_background_flags = 0;
        shooter_load_request = 0x44;
        audio_bgm_play_short(0, 0x202);
        mode_step_next();
    }
}

void shooter_game_end_step_wait(void) {
    if (game_state.pad_pressed != 0) {
        mode_step_next();
    }
}

void shooter_game_end_step_leave(void) {
    if (shooter_load_request == 0) {
        if (audio_bank_transfer_poll() == 1) {
            mode_stage_set(8);
        }
    }
}

void shooter_snapshot_screen(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = draw_buffer * 0xF0;
    rect.w = 0x200;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0x200, 0);
    func_8009C674(0);
}

/* FAKE: T-25137. as1 breaks equal-priority scheduling ties by source line (the lineno of its DAG node,
 * smaller first). The original stores p->y3 after the eight UV byte stores and after the lui of the
 * AddPrim argument, which needs the y3 statement on a later line than the AddPrim call. Each `#line`
 * pair gives the y3 statement that line and restores the numbering; the real source layout is unknown
 * (drafts/T-25137/TACO/shooter_draw_screen_wipe/NOTES.md). */
s32 shooter_draw_screen_wipe(s32 arg0) {
    GameState *g = &game_state;
    s32 pad; /* FAKE: unused word gives the frame home slot of the original; real source unknown. T-19030 */
    POLY_FT4 *p;

    p = (POLY_FT4 *)GetWorkBase(0x28, draw_buffer);
    func_8009F040(p);
    SetSemiTrans(p, 0);
    SetShadeTex(p, 0);
    p->tpage = func_8009ECB0(2, 0, 0x200, 0);
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->x0 = g->draw_origin_x[draw_buffer] + (((0x64 - arg0) << 9) / 100 - ((0x64 - arg0) * 0x10) / 100) - 0x100;
    p->y0 = g->draw_origin_y[draw_buffer] + ((0x64 - arg0) << 6) / 100 - 0x78;
    p->x1 = g->draw_origin_x[draw_buffer] + (((0x64 - arg0) << 9) / 100 - ((0x64 - arg0) * 0x10) / 100) + (arg0 << 8) / 100 - 0x100;
    p->y1 = g->draw_origin_y[draw_buffer] + ((0x64 - arg0) << 6) / 100 - 0x78;
    p->x2 = g->draw_origin_x[draw_buffer] + (((0x64 - arg0) << 9) / 100 - ((0x64 - arg0) * 0x10) / 100) - 0x100;
    p->y2 = g->draw_origin_y[draw_buffer] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78;
    p->x3 = g->draw_origin_x[draw_buffer] + (((0x64 - arg0) << 9) / 100 - ((0x64 - arg0) * 0x10) / 100) + (arg0 << 8) / 100 - 0x100;
#line 115
    p->y3 = g->draw_origin_y[draw_buffer] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78;
#line 112
    /* one comma statement, the shape of the libgpu setUV4 macro: gives the original's store order and constant registers. T-24200 */
    p->u0 = 0, p->v0 = 0, p->u1 = 0xFF, p->v1 = 0, p->u2 = 0, p->v2 = 0xF0, p->u3 = 0xFF, p->v3 = 0xF0;
    AddPrim((draw_buffer << 0xA) + 0x3FC + draw_ot_tags, p);
    p = (POLY_FT4 *)GetWorkBase(0x28, draw_buffer);
    func_8009F040(p);
    SetSemiTrans(p, 0);
    SetShadeTex(p, 0);
    p->tpage = func_8009ECB0(2, 0, 0x300, 0);
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->x0 = g->draw_origin_x[draw_buffer] + (((0x64 - arg0) << 8) / 100 - ((0x64 - arg0) * 0x10) / 100);
    p->y0 = g->draw_origin_y[draw_buffer] + ((0x64 - arg0) << 6) / 100 - 0x78;
    p->x1 = g->draw_origin_x[draw_buffer] + (((0x64 - arg0) << 8) / 100 - ((0x64 - arg0) * 0x10) / 100) + (arg0 << 8) / 100;
    p->y1 = g->draw_origin_y[draw_buffer] + ((0x64 - arg0) << 6) / 100 - 0x78;
    p->x2 = g->draw_origin_x[draw_buffer] + (((0x64 - arg0) << 8) / 100 - ((0x64 - arg0) * 0x10) / 100);
    p->y2 = g->draw_origin_y[draw_buffer] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78;
    p->x3 = g->draw_origin_x[draw_buffer] + (((0x64 - arg0) << 8) / 100 - ((0x64 - arg0) * 0x10) / 100) + (arg0 << 8) / 100;
#line 135
    p->y3 = g->draw_origin_y[draw_buffer] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78;
#line 133
    p->u0 = 0, p->v0 = 0, p->u1 = 0xFF, p->v1 = 0, p->u2 = 0, p->v2 = 0xF0, p->u3 = 0xFF, p->v3 = 0xF0;
    AddPrim((draw_buffer << 0xA) + 0x3FC + draw_ot_tags, p);
}
