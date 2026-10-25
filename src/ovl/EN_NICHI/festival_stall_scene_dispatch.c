/* festival_stall_scene_dispatch.c: ; src/ovl/EN_NICHI/80136B10.c:30-37 dispatches three festival-stall scene stages through its scene table. Original object 0x80136B10-0x801390D0 (config/objects/EN_NICHI.txt). */
#include "common.h"
#include "ovl/EN_NICHI.h"

typedef struct EnWork1C20 {
    /* 0x0000 */ u8 pad0000[0x1A2E];
    /* 0x1A2E */ s16 unk_1A2E;
    /* 0x1A30 */ u8 pad1A30[2];
    /* 0x1A32 */ s16 unk_1A32;
    /* 0x1A34 */ u8 pad1A34[0x1C8];
    /* 0x1BFC */ s16 unk_1BFC;
    /* 0x1BFE */ u8 pad1BFE[0x1E];
    /* 0x1C1C */ s32 unk_1C1C;
} EnWork1C20; /* size 0x1C20 */
typedef struct {
    void (*f[3])();
} FnTbl3; /* size 0xC */

/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/festival_stall_scene_dispatch.data", D_80139C30);
u8 D_80139C34[8] = { 0 };
u8 D_80139C3C[8] = { 0 };
u8 D_80139C44[8] = { 0 };
s16 stalls_collected_result_codes[6] = { 0 };
u16 D_80139C58 = 0;
/* Scene table, indexed by game_state.mode_stage in stalls_later_year_callback_dispatch. Defining it here as the
 * original did matters for code: as1 treats every function named in this file's data as an
 * address-taken label, so the jump-table dispatch inside stalls_game_actor_setup gets an edge back to
 * that function's entry, and the resulting loop decides its delay-slot fills (T-25201). */
FnTbl3 D_80139C5C = { { stalls_presentation_intro_dispatch, stalls_game_actor_setup, stalls_game_state_dispatch } };
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/festival_stall_scene_dispatch.data", D_80139C68);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/festival_stall_scene_dispatch.data", D_80139C94);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/festival_stall_scene_dispatch.data", D_80139CC0);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/festival_stall_scene_dispatch.data", D_80139CEC);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/festival_stall_scene_dispatch.data", D_80139D04);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/festival_stall_scene_dispatch.data", D_80139D1C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/festival_stall_scene_dispatch.data", D_80139DA0);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/festival_stall_scene_dispatch.data", D_80139E20);

void stalls_later_year_callback_dispatch(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    FnTbl3 tbl;

    tbl = D_80139C5C;
    tbl.f[game_state.mode_stage]();
}

void stalls_presentation_intro_dispatch(void) {
    s32 i;

    switch (game_state.mode_step) {
    case 0:
        scene_screen_reset(1, 0);
        hud_cursor_show(0);
        hizuke_disp_switch(0);
        message_disp_switch(0);
        srand(game_state.timer_ticks);
        audio_command(0xCF);
        mode_step_next();
        return;
    case 1:
        cd_read_sectors(0x6C, 0x801B0000, 0x7D0B);
        mode_step_next();
        return;
    case 2:
        if (cd_read_sync_status() & 1) {
            game_state.screen_brightness = 0;
            stalls_asset_pointer_setup();
            for (i = 0; i != 2; i++) {
                load_palette(D_801391F4, (i + 0x10) & 0xFF, 1, 1, i);
            }
            layer_tileset_init(0, (u32 *) D_801391FC, D_801391E0);
            srn_init(0, (u32 *) D_801391F8, 0);
            srn_vram_set(0, 5, 0x580, 0xF0);
            Fade_In_Init();
            mode_step_next();
        }
        return;
    case 3:
        audio_command(0x203);
        mode_step_next();
        return;
    case 4:
        Fade_In_Main();
        if (game_state.pad_pressed & 0x60) {
            audio_command(0x74);
            Fade_Out_Init();
            mode_step_next();
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        return;
    case 5:
        if (Fade_Out_Main() != 0) {
            for (i = 0; i < 6; i++) {
                game_state.tpage_buf[i + 5] = -1;
                game_state.tpage_buf[i + 37] = -1;
            }
            mode_stage_next();
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        return;
    default:
        game_mode_reset_to_title();
        break;
    }
}

/* The dispatch delay slots hold the next compare's li s0,2 and li at,3 only because D_80139C5C
 * (above) names this function: the inner jump-table jr then has an edge back to the entry, and
 * as1 schedules the compare chain and the case-3 head as one loop region without the case-1 and
 * case-2 heads (T-25201; history T-25109, drafts/T-25109/). */
void stalls_game_actor_setup(void) {
    GameState *gs;
    s32 i;
    EnHalf22 frames;
    EnHalf22 xs;
    EnHalf22 ys;
    EnBytes22 kinds;

    frames = D_80139C68;
    xs = D_80139C94;
    ys = D_80139CC0;
    kinds = D_80139CEC;
    gs = &game_state;
    switch (gs->mode_step) {
    case 0:
        load_tpage_buf_lock(5, ((s32 *)D_801391E4)[7], 1);
        for (i = 0; i < 7; i++) {
            search_load_tpage_buf_lock(((s32 *)D_801391E4)[i], 1);
        }
        for (i = 8; i < 10; i++) {
            search_load_tpage_buf_lock(((s32 *)D_801391E4)[i], 1);
        }
        mode_step_next();
        return;
    case 1:
        for (i = 0; i < 22; i++) {
            actor_clear(i + 96);
            ((Actor *)actor_pool)[96 + i].unk_01 = kinds.value[i];
            ((Actor *)actor_pool)[96 + i].quad_flags = 0x01000000;
            ((Actor *)actor_pool)[96 + i].brightness = 0;
            ((Actor *)actor_pool)[96 + i].flags = 0x40;
            ((Actor *)actor_pool)[96 + i].draw_flags = 4;
            ((Actor *)actor_pool)[96 + i].frame_table = (u8 *)D_801391E8;
            ((Actor *)actor_pool)[96 + i].sequence_table = (u8 *)D_801391EC;
            ((Actor *)actor_pool)[96 + i].tpage_table = (u8 *)D_801391E4;
            ((Actor *)actor_pool)[96 + i].unk_14 = D_801391F0;
            ((Actor *)actor_pool)[96 + i].unk_04 = 0;
            ((Actor *)actor_pool)[96 + i].unk_05 = 0x80;
            ((Actor *)actor_pool)[96 + i].scale_x = 0x1000;
            ((Actor *)actor_pool)[96 + i].scale_y = 0x1000;
            ((Actor *)actor_pool)[96 + i].blink_period = 0;
            ((Actor *)actor_pool)[96 + i].unk_18 = frames.value[i];
            ((Actor *)actor_pool)[96 + i].x = xs.value[i];
            ((Actor *)actor_pool)[96 + i].y = ys.value[i];
        }
        for (i = 0; i < 21; i++) {
            ((Actor *)actor_pool)[96 + i].draw_flags |= 0x80;
        }
        mode_step_next();
        return;
    case 2:
        if (((Actor *)actor_pool)[96].brightness >= 0x80U) {
            gs->screen_brightness = 0x80;
            ((Actor *)actor_pool)[96].brightness = 0x80;
            mode_step_next();
        } else {
            ((Actor *)actor_pool)[96].brightness += 4;
        }
        for (i = 1; i < 22; i++) {
            ((Actor *)actor_pool)[96 + i].brightness = ((Actor *)actor_pool)[96].brightness;
        }
        return;
    case 3:
        switch (gs->substep) {
        case 0:
            cd_audio_request_set(0x07000002, 0x03000008, 0xE);
            audio_command(0x300);
            gs->substep += 1;
            return;
        case 1:
        case 3:
            if (cd_audio_state() == 4) {
                gs->substep += 1;
                return;
            }
        default:
            return;
        case 2:
            if (cd_audio_state() == 2) {
                cd_audio_request_set(0x08000001, 0x02000004, 0xE);
                audio_command(0x300);
                gs->substep += 1;
                return;
            }
            break;
        case 4:
            if (cd_audio_state() == 2) {
                D_80139C58 = 0;
                D_80139C30 = 5;
                for (i = 0; i < 5; i++) {
                    D_80139C34[i] = 0;
                    D_80139C3C[i] = 0;
                    D_80139C44[i] = 0;
                }
                ((Actor *)actor_pool)->x = 0;
                ((Actor *)actor_pool)->y = 0;
                mode_stage_next();
                return;
            }
            break;
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void stalls_game_state_dispatch(void) {
    s32 i;
    s32 n;

    switch (game_state.mode_step) {
    case 0:
        hud_cursor_update();
        ((Actor *)actor_pool)[103].x = ((Actor *)actor_pool)[0].x;
        ((Actor *)actor_pool)[103].y = ((Actor *)actor_pool)[0].y;
        if ((game_state.pad_pressed & PAD_CIRCLE) && D_80139C30 != 0) {
            D_80139C30--;
            D_80139C34[D_80139C30] = 1;
            D_80139C3C[D_80139C30] = (D_80139C58 >> 4) + 1;
            /* Slots from record 98 indexed by the plain counter: the original scales it by 0x44 with
             * shifts here, while case 1's [n + 98] form multiplies by the hoisted 0x44 register. */
            ((Actor *)actor_pool + 98)[D_80139C30].x = ((Actor *)actor_pool)[103].x;
            ((Actor *)actor_pool + 98)[D_80139C30].y = ((Actor *)actor_pool)[103].y;
            ((Actor *)actor_pool + 98)[D_80139C30].unk_01 = 4;
            audio_command(0x50C);
        }
        stalls_target_collision_update();
        stalls_eligible_girl_frame_select();
        if (D_80139C30 == 0 && D_80139C34[0] == 0 && D_80139C34[1] == 0 && D_80139C34[2] == 0 && D_80139C34[3] == 0 && D_80139C34[4] == 0) {
            mode_step_next();
        }
        break;
    case 1:
        ((Actor *)actor_pool)[117].draw_flags |= 0x80;
        n = 0;
        for (i = 0; i < 5; i++) {
            if (stalls_collected_result_codes[i] != 0xFF) {
                ((Actor *)actor_pool)[n + 98].unk_18 = stalls_collected_result_codes[i];
                n++;
            }
            ((Actor *)actor_pool)[i + 98].unk_01 = 1;
        }
        /* FAKE: T-25109, a never-true loop that ugen folds away (no code). Its blocks lengthen the
         * live range of the hoisted 0xFF, so uopt colours actor_pool first (t1) and 0xFF second (t2). */
        do {
        } while ((u32)n < 0 && (u32)i < 0);
        for (; n < 5; n++) {
            ((Actor *)actor_pool)[n + 98].unk_18 = 0xFF;
        }
        Vblnk_Timer_Init();
        D_80139C30 = 0;
        mode_step_next();
        break;
    case 2:
        if (Vblnk_Timer() >= 0x2E) {
            if (((Actor *)actor_pool + 98)[D_80139C30].unk_18 == 0xFF || D_80139C30 >= 5) {
                if (stalls_collected_result_codes[0] == 0x1F && stalls_collected_result_codes[1] == 0x1F && stalls_collected_result_codes[2] == 0x1F && stalls_collected_result_codes[3] == 0x1F && stalls_collected_result_codes[4] == 0x1F) {
                    mode_step_set(5);
                } else if (stalls_collected_result_codes[0] == 0x20 && stalls_collected_result_codes[1] == 0x20 && stalls_collected_result_codes[2] == 0x20 && stalls_collected_result_codes[3] == 0x20 && stalls_collected_result_codes[4] == 0x20) {
                    mode_step_set(6);
                } else {
                    Vblnk_Timer_Init();
                    mode_step_next();
                }
                audio_command(0x50B);
            } else {
                audio_command(0x50A);
                ((Actor *)actor_pool + 98)[D_80139C30].draw_flags = 0x84;
                ((Actor *)actor_pool + 98)[D_80139C30].x = (D_80139C30 << 6) - 0x80;
                ((Actor *)actor_pool + 98)[D_80139C30].y = 0x38;
                D_80139C30++;
                Vblnk_Timer_Init();
            }
        }
        break;
    case 3:
        if (Vblnk_Timer() >= 0x79) {
            if (((Actor *)actor_pool)[96].brightness < 4U) {
                actor_clear_range(0);
                mode_step_next();
            } else {
                ((Actor *)actor_pool)[96].brightness -= 4;
            }
            for (i = 1; i < 22; i++) {
                ((Actor *)actor_pool)[96 + i].brightness = ((Actor *)actor_pool)[96].brightness;
            }
        }
        break;
    case 4:
        if (stalls_collected_result_codes[0] < 0x10 && stalls_collected_result_codes[1] < 0x10 && stalls_collected_result_codes[2] < 0x10 && stalls_collected_result_codes[3] < 0x10 && stalls_collected_result_codes[4] < 0x10) {
        } else if (stalls_collected_result_codes[0] == 0xFF && stalls_collected_result_codes[1] == 0xFF) {
            /* FAKE: T-25109, a loop that uopt keeps and ugen folds away; it keeps the empty arm's two
             * 0xFF tests that the original has (an empty or dead-store arm is removed whole). */
            n = 0;
            do {
            } while ((u32)n < 0);
        }
        if (stalls_collected_result_codes[0] == 0x10 && stalls_collected_result_codes[1] == 0x10 && stalls_collected_result_codes[2] == 0x10 && stalls_collected_result_codes[3] == 0x10 && stalls_collected_result_codes[4] == 0x10 && game_state.current_girl == GIRL_YUINA) {
            /* FAKE: T-25109, a second use so uopt keeps unk_F5F of this test in v1 (register-cheatsheet). */
            if ((s32)game_state.current_girl) {
            }
            normal_date_minigame_result = 3;
        } else if (game_state.current_girl + 0x14 == stalls_collected_result_codes[0] && game_state.current_girl + 0x14 == stalls_collected_result_codes[1] && game_state.current_girl + 0x14 == stalls_collected_result_codes[2] && game_state.current_girl + 0x14 == stalls_collected_result_codes[3] && game_state.current_girl + 0x14 == stalls_collected_result_codes[4] && game_state.current_girl != GIRL_YUINA) {
            normal_date_minigame_result = 3;
        } else if (stalls_collected_result_codes[0] < 0x21 && stalls_collected_result_codes[1] < 0x21 && stalls_collected_result_codes[2] < 0x21 && stalls_collected_result_codes[3] < 0x21 && stalls_collected_result_codes[4] < 0x21) {
            normal_date_minigame_result = 1;
        } else {
            normal_date_minigame_result = 4;
        }
        game_mode_set(game_state.saved_game_mode);
        mode_stage_set(game_state.saved_mode_stage);
        mode_step_set(game_state.saved_mode_step);
        break;
    case 5:
        switch (game_state.substep) {
        case 0:
            cd_audio_request_set(0x1A0000B1, 0x170000B7, 0xE);
            audio_command(0x300);
            game_state.substep += 1;
            break;
        case 1:
            if (cd_audio_state() == 4) {
                game_state.substep += 1;
            }
            break;
        case 2:
            if (cd_audio_state() == 2) {
                audio_command(0x74);
                game_state.substep += 1;
            }
            break;
        case 3:
            Vblnk_Timer_Init();
            game_state.substep += 1;
            break;
        case 4:
            if (Vblnk_Timer() >= 0x79) {
                Vblnk_Timer_Init();
                mode_step_set(3);
            }
            break;
        default:
            game_mode_reset_to_title();
            break;
        }
        break;
    case 6:
        switch (game_state.substep) {
        case 0:
            cd_audio_request_set(0x03000000, 0x01000007, 0xE);
            audio_command(0x300);
            game_state.substep += 1;
            break;
        case 1:
            if (cd_audio_state() == 4) {
                game_state.substep += 1;
            }
            break;
        case 2:
            if (cd_audio_state() == 2) {
                audio_command(0x74);
                game_state.substep += 1;
            }
            break;
        case 3:
            Vblnk_Timer_Init();
            game_state.substep += 1;
            break;
        case 4:
            if (Vblnk_Timer() >= 0x79) {
                Vblnk_Timer_Init();
                mode_step_set(3);
            }
            break;
        default:
            game_mode_reset_to_title();
            break;
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

/* FAKE: the u8 locals `one` and `two` put the hoisted constants first in both bne compares (T-9410 idiom), T-11300 */
void stalls_target_collision_update(void) {
    s32 i;
    u8 *p;
    u8 *r;
    u8 v;
    u8 one;
    u8 two;

    one = 1;
    two = 2;
    D_80139C58 = D_80139C58 + 4;
    if (D_80139C58 >= 0x71) {
        D_80139C58 = 0;
    }
    stalls_progress_meter_draw();
    for (i = 0, p = D_80139C34; i < 5; i++, p++) {
        v = *p;
        if (one == v) {
            r = actor_pool + i * 0x44;
            *(s16 *)(r + 0x1A22) -= D_80139C3C[i] * 8;
            *(s16 *)(r + 0x1A24) -= D_80139C3C[i] * 8;
            if (*(s16 *)(r + 0x1A22) < 0x401) {
                if (stalls_target_collision_resolve(i) != 0) {
                    *(s16 *)(r + 0x1A20) = 0x27;
                    if ((stalls_collected_result_codes[i] < 0x10) || (stalls_collected_result_codes[i] >= 0x14)) {
                        audio_command(0x508);
                    } else {
                        audio_command(0x509);
                    }
                } else {
                    audio_command(0x507);
                    *(s16 *)(r + 0x1A20) = 0x26;
                }
                *(s16 *)(r + 0x1A22) = 0x1000;
                *(s16 *)(r + 0x1A24) = 0x1000;
                *p = 2;
                v = 2;
            }
        }
        if (two == v) {
            D_80139C44[i] += 1;
            if (D_80139C44[i] >= 0x3D) {
                ((Actor *)actor_pool)[i + 98].draw_flags = 0;
                *p = 0;
            }
        }
    }
}

/* FAKE: the empty do-while after the load of D_80139C58 puts the load and its uses in the original's registers (a2, v0); found with the permuter, T-9380 */
void stalls_progress_meter_draw(void) {
    u8 *p;
    u16 w;
    s32 a;

    p = GetWorkBase(0x24, draw_buffer);
    func_8009F054(p);
    SetSemiTrans(p, 1);
    SetShadeTex(p, 0);
    w = D_80139C58;
    do {
    } while (0);
    a = w * 2 + 0x1F;
    p[6] = 0xFF;
    p[4] = 0x1F;
    p[5] = 0x1F;
    p[0xC] = a;
    p[0xE] = 0xFF;
    p[0xD] = 0x1F;
    p[0x16] = 0xFF;
    p[0x14] = 0x1F;
    p[0x15] = 0x1F;
    p[0x1C] = a;
    p[0x1E] = 0xFF;
    p[0x1D] = 0x1F;
    *(s16 *)(p + 8) = game_state.draw_origin_x[draw_buffer] + 0x20;
    *(s16 *)(p + 0xA) = game_state.draw_origin_y[draw_buffer] + 0x50;
    *(s16 *)(p + 0x10) = game_state.draw_origin_x[draw_buffer] + w + 0x20;
    *(s16 *)(p + 0x12) = game_state.draw_origin_y[draw_buffer] + 0x50;
    *(s16 *)(p + 0x18) = game_state.draw_origin_x[draw_buffer] + 0x20;
    *(s16 *)(p + 0x1A) = game_state.draw_origin_y[draw_buffer] + 0x60;
    *(s16 *)(p + 0x20) = game_state.draw_origin_x[draw_buffer] + w + 0x20;
    *(s16 *)(p + 0x22) = game_state.draw_origin_y[draw_buffer] + 0x60;
    AddPrim((draw_buffer << 10) + 0xC + draw_ot_tags, p, w);
}

/* FAKE: local reuse and the callback comma expression retain the table slots
 * and post-call decrement scheduling. T-9720. */
s32 stalls_target_collision_resolve(s32 arg0) {
    s32 i;
    s32 value;
    s32 outside;
    EnHalf12 limits;
    EnFn33 calls;
    EnWork1C20 *record;
    s16 limit;

    limits = D_80139D04;
    calls = D_80139D1C;
    outside = 0;
    record = (EnWork1C20 *)((u8 *)actor_pool + arg0 * 0x44);
    i = 0;
    value = -0x78;
    do {
        if (record->unk_1A32 < value && ((limit = limits.value[i], record->unk_1A2E < -limit) || limit < D_801206FE)) {
            outside = 1;
        }
        i++;
        value += 8;
    } while (i < 12);
    i = 11;
    if (outside != 0) {
        stalls_collected_result_codes[arg0] = 0xFF;
        return 0;
    }
    record = (EnWork1C20 *)((u8 *)actor_pool + 11 * 0x44);
loop:
    if ((outside = calls.f[record->unk_1BFC](arg0, i), i--, outside != 0)) {
        value = record->unk_1C1C;
        if (!(value & 0x40000000)) {
            record->unk_1C1C = value | 0x40000000;
            stalls_collected_result_codes[arg0] = record->unk_1BFC;
            return 1;
        }
    }
    record = (EnWork1C20 *)((u8 *)record - 0x44);
    if (i < 0) {
        stalls_collected_result_codes[arg0] = 0xFF;
        return 0;
    }
    goto loop;
}

s32 stalls_target_pair_hit_test_00(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xh;

    a = actor_pool + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xh = x + 0x10;
    b = actor_pool + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (xh >= px && px >= x - 0x10) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x8 >= py && py >= y - 0x8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xh >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y - 0x8 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 stalls_target_pair_hit_test_01(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = actor_pool + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = actor_pool + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (x >= px && px >= x - 0x10) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x8 >= py && py >= y - 0x8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 stalls_target_pair_hit_test_02(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = actor_pool + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = actor_pool + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x8 >= py && py >= y - 0x8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 stalls_target_pair_hit_test_03(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xs;

    a = actor_pool + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xs = x + 8;
    b = actor_pool + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= xs) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xs >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 8 >= py && py >= y - 8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 stalls_target_pair_hit_test_04(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xs;

    a = actor_pool + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xs = x + 8;
    b = actor_pool + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= xs) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xs >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 stalls_target_pair_hit_test_05(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xs;

    a = actor_pool + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xs = x + 8;
    b = actor_pool + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x - 0x10) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 8 >= py && py >= y - 8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xs >= px && px >= x - 8) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 stalls_target_pair_hit_test_06(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xs;

    a = actor_pool + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xs = x - 8;
    b = actor_pool + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= xs) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xs >= px && px >= x - 0x10) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 8 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 stalls_target_pair_hit_test_07(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = actor_pool + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = actor_pool + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 stalls_target_pair_hit_test_08(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xa;
    s32 xb;

    a = actor_pool + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xa = x + 8;
    xb = x + 0x10;
    b = actor_pool + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (xa >= px && px >= x - 8) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xb >= px && px >= xa) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 8 >= py && py >= y - 8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 stalls_target_pair_hit_test_09(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = actor_pool + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = actor_pool + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x - 0x10) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 8 >= py && py >= y - 8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 stalls_target_pair_hit_test_10(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = actor_pool + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = actor_pool + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x - 0x8) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x8 >= py && py >= y - 0x8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 stalls_target_pair_hit_test_11(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xh;

    a = actor_pool + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xh = x + 0x10;
    b = actor_pool + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (xh >= px && px >= x - 0x8) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xh >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y - 0x8 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 stalls_target_pair_hit_test_12(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = actor_pool + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = actor_pool + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x20 >= px && px >= x - 0x20) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x20 >= py && py >= y - 0x20) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 stalls_target_pair_hit_test_13(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = actor_pool + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = actor_pool + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x20 >= px && px >= x - 0x20) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 16 >= py && py >= y - 16) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

/* Moves the three rows of four display records (105.., 109.., 113..) in 16.16 steps; a record that
 * leaves the screen re-enters behind its neighbour with a frame from tbl, whose entries 32..47 are
 * the values of girls picked at random from list (girls with flag bit 1, and 11, 12 when set).
 * The neighbour is read before the new frame is stored, as in the original. T-20920 */
void stalls_eligible_girl_frame_select(void) {
    s32 i;
    s32 r1;
    s32 r2;
    s32 r3;
    EnHalf64 tbl;
    EnHalf13 girls;
    u8 n;
    u8 list[15];

    tbl = D_80139DA0;
    girls = D_80139E20;
    n = 0;
    for (i = 0; i < 11; i++) {
        if (game_state.girls[i].flags.f.met == 1) {
            list[n] = i;
            n++;
        }
    }
    if (system_data.flags[11] != 0 || game_state.girls[11].flags.f.story_stage != 0) {
        list[n] = 11;
        n++;
    }
    if (system_data.flags[12] != 0) {
        list[n] = 12;
        n++;
    }
    for (i = 32; i < 48; i++) {
        tbl.value[i] = girls.value[list[rand() % n]];
    }
    for (i = 0; i < 4; i++) {
        ((ActorFixed *)actor_pool)[105 + i].x.h[1]++;
        if (((ActorFixed *)actor_pool)[105 + i].x.h[1] >= 0xC1) {
            r1 = rand() & 0x3F;
            ((ActorFixed *)actor_pool)[105 + i].x.h[1] = ((ActorFixed *)actor_pool)[105 + (i + 1) % 4].x.h[1] - 0x60;
            ((ActorFixed *)actor_pool)[105 + i].frame = tbl.value[r1];
            if ((r1 & 1) && r1 < 0x20) {
                ((ActorFixed *)actor_pool)[105 + i].y.h[1] = -0x38;
            } else {
                ((ActorFixed *)actor_pool)[105 + i].y.h[1] = -0x48;
            }
            ((ActorFixed *)actor_pool)[105 + i].quad_flags = 0x01000000;
        }
    }
    for (i = 0; i < 4; i++) {
        ((ActorFixed *)actor_pool)[109 + i].x.w += 0xFFFF2000;
        if (((ActorFixed *)actor_pool)[109 + i].x.h[1] < -0xC0) {
            r2 = rand() & 0x3F;
            ((ActorFixed *)actor_pool)[109 + i].x.h[1] = ((ActorFixed *)actor_pool)[109 + (i + 3) % 4].x.h[1] + 0x60;
            ((ActorFixed *)actor_pool)[109 + i].frame = tbl.value[r2];
            if ((r2 & 1) && r2 < 0x20) {
                ((ActorFixed *)actor_pool)[109 + i].y.h[1] = -0x10;
            } else {
                ((ActorFixed *)actor_pool)[109 + i].y.h[1] = -0x20;
            }
            ((ActorFixed *)actor_pool)[109 + i].quad_flags = 0x01000000;
        }
    }
    for (i = 0; i < 4; i++) {
        ((ActorFixed *)actor_pool)[113 + i].x.w += 0xC000;
        if (((ActorFixed *)actor_pool)[113 + i].x.h[1] >= 0xC1) {
            r3 = rand() & 0x3F;
            ((ActorFixed *)actor_pool)[113 + i].x.h[1] = ((ActorFixed *)actor_pool)[113 + (i + 1) % 4].x.h[1] - 0x60;
            ((ActorFixed *)actor_pool)[113 + i].frame = tbl.value[r3];
            if ((r3 & 1) && r3 < 0x20) {
                ((ActorFixed *)actor_pool)[113 + i].y.h[1] = 0x18;
            } else {
                ((ActorFixed *)actor_pool)[113 + i].y.h[1] = 8;
            }
            ((ActorFixed *)actor_pool)[113 + i].quad_flags = 0x01000000;
        }
    }
}
