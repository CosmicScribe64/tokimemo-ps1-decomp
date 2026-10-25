/* prologue.c: src/ovl/NAME_ENT/80144AD0.c:71,83,105,2742; owns opening-mode dispatcher plus the prologue line player, speaker/voice tables and mode-stage handlers; object stays intact. Original object 0x80144AD0-0x8014B140 (config/objects/NAME_ENT.txt). */
#define MAIN_API_OVERRIDE_gnsx /* T-10860: profile clear loops use the legacy integer call view. */
#include "common.h"
#include "ovl/name_entry.h"

/* Canonical gnsx stays void; this historical call view reserves the return range. */
s32 gnsx();

/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_scene_counter);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_voice_started);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_blood_text_o);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_blood_text_a);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_blood_text_b);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_blood_text_ab);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_blood_texts);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_cd_level_last);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_cd_level_history);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_cd_level_history_1);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_cd_level_history_2);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_cd_level_history_3);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_00);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_01);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_02);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_03);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_04);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_05);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_06);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_07);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_08);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_09);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_10);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_11);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_12);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_13);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_14);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_15);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_16);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_17);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_18);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_19);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_20);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_21);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_22);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_23);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_24);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_25);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_26);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_27);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_28);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_29);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_30);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_31);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_32);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_33);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_34);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_35);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_36);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_37);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_38);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_39);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_40);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_41);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_text_42);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_texts);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_speakers);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_clips);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/prologue.data", prologue_line_clips_second);

void opening_mode_handler(void) {
    game_state.mode_stage_timer += 1;
    switch (game_state.mode_stage) {
    case 0:
        opening_story_mode_stage();
        return;
    case 1:
        opening_movie();
        return;
    }
}

void prologue_line_start(void) {
    /* FAKE: unsigned byte view on prologue_line (declared s8 in header) and 1D word view on prologue_line_clips keep index arithmetic in ugen without sll/sra widening. T-24106 */
    k_sub_reset();
    if ((prologue_line_speakers[*(u8 *)&prologue_line] & 0xF) == 0xF) {
        gnsx(game_state.player_family_name);
    } else if (prologue_line_speakers[*(u8 *)&prologue_line] == 0x1E) {
        gnsx("女子Ａ");
    } else if (prologue_line_speakers[*(u8 *)&prologue_line] == 0x2E) {
        gnsx("女子Ｂ");
    } else {
        gnsx(girl_short_name_table[prologue_line_speakers[*(u8 *)&prologue_line]]);
    }
    sndisp(prologue_line_texts[*(u8 *)&prologue_line], 0, prologue_line_speakers[*(u8 *)&prologue_line]);
    if ((prologue_line_speakers[*(u8 *)&prologue_line] & 0xF) != 0xF) {
        cd_audio_request_set(((s32 *)prologue_line_clips)[*(u8 *)&prologue_line * 2], ((s32 *)prologue_line_clips)[*(u8 *)&prologue_line * 2 + 1], prologue_line_speakers[*(u8 *)&prologue_line]);
        audio_command(0x300);
    }
    game_state.current_girl = prologue_line_speakers[*(u8 *)&prologue_line];
    prologue_voice_started = 0;
    k_sub_disp_start(1);
}

void prologue_line_run(void) {
    s32 current_text_line;
    s32 scroll_phase;
    s32 text_complete;
    u8 *a;
    u8 *b;
    u32 t;

    text_complete = 0;
    current_text_line = get_now_k();
    /* FAKE: pointer views on unk_F62/unk_F61 avoid cvt_pass widening so uopt keeps the scroll fields in caller-saved temps, and redundant `else if (scroll_phase != 0)` preserves the branch (same as xa_wait, T-17018). T-24106 */
    if (*(u8 *)&game_state.text_scroll_state & 0x80) {
        scroll_phase = *(u8 *)&game_state.text_scroll_state & 0x7F;
        if (*(u8 *)&game_state.text_first_line + 2 >= (u32)current_text_line) {
            text_complete = k_disp_inc();
            current_text_line = get_now_k();
            t = game_state.text_first_line;
            if (current_text_line == t + 3) {
                if (game_state.text_lines_total >= t + 3) {
                    k_disp_switch(current_text_line, 0);
                }
            }
        } else if (scroll_phase == 0) {
            k_disp_switch(*(u8 *)&game_state.text_first_line, 0);
            a = get_k_work(game_state.text_first_line + 1);
            b = get_k_work(game_state.text_first_line + 2);
            *(s16 *)(a + 2) -= 2;
            *(s16 *)(b + 2) -= 2;
            game_state.text_scroll_state += 1;
        } else if (scroll_phase != 0) {
            a = get_k_work(*(u8 *)&game_state.text_first_line + 1);
            b = get_k_work(game_state.text_first_line + 2);
            *(s16 *)(a + 2) -= 2;
            *(s16 *)(b + 2) -= 2;
            if (*(s16 *)(a + 2) < 0x33) {
                *(s16 *)(a + 2) = 0x32;
                *(s16 *)(b + 2) = 0x42;
                game_state.text_scroll_state -= 1;
                game_state.text_first_line += 1;
                k_disp_switch(game_state.text_first_line + 2, 1);
            }
        }
    } else {
        text_complete = k_disp_inc();
        get_now_k();
    }
    if (cd_audio_state() & 4) {
        prologue_voice_started = 1;
    }
    t = cd_audio_request_get();
    if ((game_state.current_girl & 0xF) != 0xF) {
        if (prologue_voice_started != 0) {
            if (cd_audio_state() & 2) {
                if ((((s32 *)prologue_line_clips)[*(u8 *)&prologue_line * 2] != ((s32 *)prologue_line_clips_second)[*(u8 *)&prologue_line * 2]) && (((s32 *)prologue_line_clips_second)[*(u8 *)&prologue_line * 2] != ((s32 *)t)[2])) {
                    cd_audio_request_set(((s32 *)prologue_line_clips_second)[*(u8 *)&prologue_line * 2], ((s32 *)prologue_line_clips_second)[*(u8 *)&prologue_line * 2 + 1], prologue_line_speakers[*(u8 *)&prologue_line]);
                    audio_command(0x300);
                } else if (text_complete == 1) {
                    game_state.mode_step_timer.w += 1;
                    game_state.text_scroll_state = 0;
                }
            } else {
                game_state.mode_step_timer.w = 0;
            }
        }
    } else if (text_complete == 1) {
        game_state.mode_step_timer.w += 1;
        game_state.text_scroll_state = 0;
    } else {
        game_state.mode_step_timer.w = 0;
    }
    if (game_state.mode_step_timer.u >= 0x3DU) {
        game_state.mode_step_timer.w = 0;
        game_state.substep += 1;
    }
}

void prologue_arrival_bank_start(void) {
    audio_bank_load_start(0, 0xC592, 0xC580, 0xD15F, 0xD110, 0xD102);
    mode_step_next();
}

void prologue_arrival_scene_init(void) {
    Actor *actors;
    s32 i;

    actors = (Actor *)actor_pool;
    game_state.mode_step_timer.u += 1;
    if (game_state.substep == 0) {
        D_8011ECD3 = 0;
        D_800B5BC8 = 0xFE;
        /* Unsigned byte store for the no-dialogue sentinel (header reads it signed). */
        *(u8 *)&prologue_line = 0xFF;
        draw2d3d(1, 0);
        actor_draw_enable(1);
        vram_clear_blocks(0xFE);
        sprite_tables_clear();
        actor_clear_upper();
        back_clear_switch(1);
        tpage_buf_clear();
        game_state.tile_layer_enabled = 0;
        srand(game_state.frame_count);
        text_system_reset();
        k_speed_set(TEXT_SPEED_EVERY_SIX_FRAMES);
        message_window_set_title((s32) "プロローグ");
        k_disp_start(1);
        k_sub_reset_point_set();
        parameter_show_init();
        parameter_show_init();
        parameter_disp_switch(0);
        hizuke_init();
        message_window_init();
        game_state.screen_brightness = 0;
        dec_bg_reset();
        game_state.substep += 1;
        hizuke_disp_switch(1);
    } else if (game_state.substep == 1) {
        if (audio_bank_load_poll() == 1) {
            audio_bank_start(0, 0);
            dec_bg_cd_read(0x494B, 1);
            game_state.substep += 1;
        }
    } else if (game_state.substep == 2) {
        if ((u32) game_state.mode_step_timer.u >= 0x11U) {
            game_state.substep += 1;
        }
    } else if (game_state.substep == 3) {
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            if (bg_decode_result == -1) {
                dec_bg_reset();
                dec_bg_cd_read(0x494B, 1);
            } else {
                cd_read_sectors(0xDU, 0x80180000, 0xABAB);
                game_state.substep += 1;
            }
        }
    } else if ((game_state.substep == 4) && (cd_read_sync_status() & 1)) {
        actor_clear(0x60);
        D_80120651 = 0xA;
        D_80120652 = 1;
        D_80120688 = 0x41000000;
        D_80120653 = 0x24;
        D_8012065C = (u8 *)0x801800AC;
        D_80120660 = (u8 *)0x801800C0;
        actors[96].tpage_table = (u8 *)0x801800A8;
        D_80120656 = 2;
        D_80120693 = 0;
        D_80120655 = 0x80;
        D_80120676 = -0xA0;
        D_8012067A = -0x78;
        D_80120657 = 0x80;
        load_tpage_buf_lock(0x1A, *(s32 *)actors[96].tpage_table, 1);
        for (i = 0; i < 16; i++) {
            load_palette((u8 *)0x80184490, (i + 16) & 255, 1, 2, i);
        }
        color_palette_to_gray(0x80184610, (s32) game_state.scratch_words);
        load_palette(game_state.scratch_words, 0xD, 1, 2, 0);
        mode_step_next();
    }
    if ((u8) game_state.screen_brightness < 0x80U) {
        game_state.screen_brightness += 1;
    }
}

void prologue_arrival_line(void) {
    u8 *scene_step = &game_state.substep;
    switch (*scene_step) {
    case 0:
        back_clear_switch(0);
        game_state.screen_brightness = 0x80;
        prologue_line = 0;
        prologue_line_start();
        *scene_step += 1;
        return;
    case 1:
        prologue_line_run();
        return;
    case 2:
        *scene_step += 1;
        game_state.mode_step_timer.u = 0;
        /* fallthrough */
    case 3:
        if (game_state.mode_step_timer.u++ >= 0x3DU) {
            D_80120653 |= 0x80;
            mode_stage_next();
        }
        return;
    }
}

void prologue_mode_stage_arrival(void) {
    switch (game_state.mode_step) {
    case 0:
        prologue_arrival_bank_start();
        break;
    case 1:
        prologue_arrival_scene_init();
        break;
    case 2:
        prologue_arrival_line();
        break;
    }
    hizuke_show();
    dec_bg_draw(1);
}

void prologue_petals_init(void) {
    s32 var_s1;
    Actor *var_s0;
    s32 last;

    for (var_s0 = (Actor *)actor_pool, var_s1 = 0; var_s1 != 8; var_s1++) {
        actor_clear(var_s1 + 0x60);
        var_s0[96].unk_01 = 0xA;
        var_s0[96].flags = 1;
        var_s0[96].quad_flags = 0x41000000;
        var_s0[96].draw_flags = 0xA4;
        var_s0[96].frame_table = (u8 *)0x80184228;
        var_s0[96].sequence_table = (u8 *)0x80184258;
        var_s0[96].tpage_table = (u8 *)0x80184224;
        var_s0[96].sequence_slot = 0;
        var_s0[96].unk_18 = (s16) (rand() & 7);
        var_s0[96].unk_05 = 0x80;
        var_s0[96].x = (s16) ((rand() % 440) - 0xA0);
        var_s0[96].y = (s16) (-0x78 - (rand() % 120));
        last = rand();
        var_s0++;
        var_s0[95].brightness = last;
    }
}

void prologue_petals_update(s32 motion_variant) {
    ActorFixed *p;

    /* FAKE: sharing the initialization and loop source line places the actor
     * base setup after the two endpoint loads. T-10030. */
    p = (ActorFixed *)actor_pool; do {
        p[96].unk_07++;
        p[96].x.w += rsin(p[96].unk_07 << 5) * 8;
        if (motion_variant == 0) {
            p[96].x.h[1]--;
        }
        p[96].y.h[1]++;
        if (p[96].y.h[1] >= 41) {
            if (motion_variant == 1 && p >= (ActorFixed *)D_8011EDE0) {
                p[96].draw_flags = 0;
            }
            p[96].x.h[1] = (rand() % 320) - 160;
            p[96].y.h[1] = -120 - (rand() % 48);
        }
        p++;
    } while (p != (ActorFixed *)D_8011EEF0);
}

void prologue_board_load(void) {
    switch (game_state.substep) {
    case 0:
        D_800B3DB0 = 1;
        hizuke_show();
        cd_read_sectors(9, 0x80162000, 0x4954);
        game_state.substep += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            actor_clear(0x60);
            hizuke_disp_switch(0);
            prologue_petals_init();
            bg_still_decode_to_vram(0x140, 0, 0x280, 0xA0);
            move_255_line(5, 0xE, 0xF);
            prologue_scene_counter = 0;
            mode_step_next();
        } else {
            hizuke_show();
        }
        break;
    }
    dec_bg_draw(1);
}

void prologue_board_scroll(void) {
    /* FAKE: unused homes place RECT at sp+0x4C in the 0x58 frame. T-10860. */
    s32 unused;
    RECT rect;
    s32 unused2;

    if (game_state.frame_count & 1) {
        prologue_scene_counter += 1;
    }
    /* FAKE: scalar selector view avoids the aggregate promotion copy. T-10860. */
    switch (*(u8 *)&game_state.substep) {
    case 0:
        prologue_line = 1;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 1:
        prologue_line_run();
        break;
    case 2:
        if (prologue_scene_counter >= 0x141) {
            prologue_scene_counter = 0x140;
            game_state.substep += 1;
        }
        break;
    case 3:
        prologue_scene_counter = 0x140;
        game_state.mode_step_timer.u = 0;
        dec_bg_cd_read(0x495D, 1);
        game_state.substep += 1;
        break;
    case 4:
        prologue_scene_counter = 0x140;
        if (game_state.mode_step_timer.u++ >= 0x79U) {
            game_state.substep += 1;
        }
        break;
    case 5:
        prologue_scene_counter = 0x140;
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            if (bg_decode_result == -1) {
                dec_bg_reset();
                dec_bg_cd_read(0x495D, 1);
            } else {
                mode_stage_next();
            }
        }
        break;
    }
    /* FAKE: one source line schedules the four stores in reverse order. T-10860. */
    rect.x = -0xA0; rect.y = -0x78; rect.w = 0x140; rect.h = 0xA0;
    srn_tpage_show(5, 2, 0, &rect, prologue_scene_counter, 0, 0, 0, 0xD);
    if (prologue_scene_counter == 0x140) {
        prologue_petals_update(1);
        return;
    }
    prologue_petals_update(0);
}

void prologue_mode_stage_board(void) {
    switch (game_state.mode_step) {
    case 0:
        prologue_board_load();
        return;
    case 1:
        prologue_board_scroll();
        return;
    }
}
void prologue_names_crossfade(void) {
    GameState *scene_state;
    /* The calls below were matched without a prototype (main_api.h: void(u8)); the other functions of this file use it. */
    extern void set_dec_bri();

    scene_state = &game_state;
    switch (scene_state->substep) {
    case 0:
        dec_bg_cd_read(0x4966, 1);
        prologue_line = 2;
        prologue_scene_counter = 0;
        prologue_line_start();
        dec_bg_draw(1);
        scene_state->substep += 1;
        return;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_decode_to_vram(1);
            prologue_scene_counter = 0x80;
            scene_state->substep += 1;
            bg_fade_set(0x80);
        }
        dec_bg_draw(1);
        prologue_line_run();
        return;
    case 2:
        if (prologue_scene_counter >= 0x11) {
            prologue_scene_counter -= 3;
            bg_fade_set(prologue_scene_counter & 0xFF);
        } else {
            scene_state->substep += 1;
        }
        bg_crossfade_draw(1);
        prologue_line_run();
        return;
    case 3:
        prologue_line_run();
        bg_crossfade_draw(1);
        return;
    case 4:
        bg_crossfade_draw(1);
        if (prologue_scene_counter < 0x80) {
            bg_fade_set(prologue_scene_counter & 0xFF);
            prologue_scene_counter += 4;
            return;
        }
        prologue_scene_counter = 0x80;
        bg_fade_set(0x80);
        set_dec_bri(prologue_scene_counter & 0xFF);
        cd_read_sectors(0x31U, 0x801A4000, 0xAB18);
        scene_state->substep += 1;
        return;
    case 5:
        if (prologue_scene_counter >= 3) {
            prologue_scene_counter -= 1;
            set_dec_bri(prologue_scene_counter & 0xFF);
        } else {
            scene_state->substep += 1;
        }
        dec_bg_draw(1);
        return;
    case 6:
        if (cd_read_sync_status() & 1) {
            prologue_palette_adjust();
            actor_clear_upper();
            cd_read_sectors(0x11U, 0x80180000, 0xAC3F);
            scene_state->substep += 1;
        }
        dec_bg_draw(1);
        return;
    case 7:
        if (cd_read_sync_status() & 1) {
            memcpy((void *)0x801F0000, 0x80180000, 0x8800);
            D_80120651 = 0xA;
            D_80120652 = 0;
            D_80120688 = 0x41000000;
            D_80120653 = 0x24;
            D_8012065C = (u8 *)0x801800D8;
            D_80120660 = (u8 *)0x801800F0;
            D_80120684 = (u8 *)0x801800D4;
            D_80120666 = 0;
            D_80120655 = 0x80;
            D_80120676 = -0xA0;
            D_8012067A = -0x78;
            scene_state->substep += 1;
            dec_bg_cd_read(0xACDD, 1);
        }
        dec_bg_draw(1);
        return;
    case 8:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            if (bg_decode_result == -1) {
                dec_bg_reset();
                dec_bg_cd_read(0xACDD, 1);
            } else {
                mode_stage_next();
                D_80120653 = 0xA4;
                D_80120676 = -0xA0;
                D_8012067A = -0x78;
            }
        }
        dec_bg_draw(1);
        /* fallthrough */
    default:
        return;
    }
}

void prologue_palette_adjust(void) {
    s32 i;
    u32 *p;
    u32 *q;

    q = (u32 *)0x801BC004;
    p = (u32 *)0x801BC000;
    if (!(q[0] & 0x7FFF0000)) {
        q[0] |= 0x17FFF;
    }
    /* FAKE: the last store of the first word shares a source line with the loop (as1 orders the loop set-up by line). T-10510 */
    if (!(q[0] & 0x7FFF)) { q[0] |= 0x7FFF0001; } for (i = 1; i < 0x40; i++) {
        if (!(p[i * 2] & 0x7FFF0000)) {
            p[i * 2] |= 0x17FFF;
        }
        if (!(p[i * 2] & 0x7FFF)) {
            p[i * 2] |= 0x7FFF0001;
        }
        if (!(p[i * 2 + 1] & 0x7FFF0000)) {
            p[i * 2 + 1] |= 0x17FFF;
        }
        if (!(p[i * 2 + 1] & 0x7FFF)) {
            p[i * 2 + 1] |= 0x7FFF0001;
        }
    }
}




void prologue_mode_stage_names(void) {
    prologue_petals_update(1);
    if (game_state.mode_step == 0) {
        prologue_names_crossfade();
    }
}

void prologue_background_slot_set(u32 image_address, s32 palette_row_offset) {
    s32 i;
    u32 addr[6];
    /* FAKE: unused local preserves the 0x70 stack frame and places image_address + 0x18000 spill at sp+0x4C. T-24106 */
    s32 pad;

    /* FAKE: single-line loop orders the unrolled stores in as1. T-24106 */
    for (i = 0; i < 6; i++) { addr[i] = image_address + i * 0x4000; }
    prim_bg_slot_setup(addr, 0, 6, 3, 2, palette_row_offset + 0x1E0, -0xA0, -0x78, 8, 8);
    load_palette((u8 *)(image_address + 0x18000), 0xF, 1, 2, 0);
    color_palette_to_gray(image_address + 0x18000, (s32) game_state.scratch_words);
    load_palette(game_state.scratch_words, 0xE, 1, 2, 0);
    game_state.bg_layer_order[0] = 0xD;
    ((BgLayer *)&bg_layers)[0].unk_00 = 0x41000000;
}

void prologue_line_03_run(void) {
    switch (game_state.substep) {
    case 0:
        prologue_line = 3;
        D_801220E1 = 0;
        D_801220E2 = 0;
        D_801220E0 = 0;
        set_dec_bri(0);
        prologue_line_start();
        prologue_background_slot_set(0x801A4000, 0xE);
        game_state.substep += 1;
        break;
    case 1:
        /* FAKE: pointer views on D_801220E0/D_801220E1 give LDA D_801220E1 colour $v0 (folded in ugen), +4 colour $v1, and D_801220E0 colour $a1. T-24106 */
        if (*(u8 *)&D_801220E0 < 0x81U) {
            *(u8 *)&D_801220E0 = *(u8 *)&D_801220E1 = D_801220E2 = *(u8 *)&D_801220E0 + 4;
            set_dec_bri(*(u8 *)&D_801220E0);
        }
        prologue_line_run();
        break;
    case 2:
        cd_read_sectors_nowait(0x31U, 0x801A4000, 0xAA23);
        game_state.substep += 1;
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            dec_bg_cd_read(0xACE6, 1);
            game_state.substep += 1;
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            if (bg_decode_result == -1) {
                dec_bg_reset();
                dec_bg_cd_read(0xACE6, 1);
            } else {
                mode_stage_next();
            }
        }
        break;
    }
    dec_bg_draw(1);
}

void prologue_mode_stage_line_03(void) {
    if (game_state.mode_step == 0) {
        prologue_line_03_run();
    }
}

void prologue_line_04_run(void) {
    switch (game_state.substep) {
    case 0:
        actor_clear(0x60);
        cd_read_sectors(9U, 0x80180000, 0xABB8);
        prologue_line = 4;
        prologue_background_slot_set(0x801A4000, 0xF);
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            D_80120651 = 0xA;
            D_80120652 = 0;
            D_80120688 = 0x41000000;
            D_80120653 = 0xAC;
            D_8012065C = (u8 *)0x8018007C;
            D_80120660 = (u8 *)0x8018008C;
            D_80120684 = (u8 *)0x80180078;
            D_80120666 = 0;
            D_80120655 = 0x80;
            D_80120676 = -0xA0;
            D_8012067A = -0x78;
            game_state.substep += 1;
        }
        break;
    case 2:
        if (((rand() & 0x7F) == 0x7F) && !(D_80120652 & 1)) {
            D_80120652 = 0x11;
        }
        prologue_line_run();
        break;
    case 3:
        cd_read_sectors(0x31U, 0x801A4000, 0xAB18);
        game_state.substep += 1;
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            prologue_palette_adjust();
            dec_bg_show_set(1, dec_bg_cd_read(0xACDD, 1));
            memcpy((void *)0x80180000, 0x801F0000, 0x8800);
            prologue_background_slot_set(0x801A4000, 0xE);
            actor_clear(0x60);
            D_80120651 = 0xA;
            D_80120652 = 0;
            D_80120688 = 0x41000000;
            D_80120653 = 0xAC;
            D_8012065C = (u8 *)0x801800D8;
            D_80120660 = (u8 *)0x801800F0;
            D_80120684 = (u8 *)0x801800D4;
            D_80120666 = 0;
            D_80120655 = 0x80;
            D_80120676 = -0xA0;
            D_8012067A = -0x78;
            actor_clear(0x61);
            D_80120695 = 0xA;
            D_80120696 = 0x40;
            D_801206CC = 0x41000000;
            D_80120697 = 0xAC;
            D_801206A0 = 0x80184184;
            D_801206A4 = 0x8018418C;
            D_801206C8 = 0x80184180;
            D_801206AA = 0;
            D_80120699 = 0x80;
            D_801206BA = -0x20;
            D_801206BE = -0x6C;
            k_sub_reset();
            prologue_scene_counter = 0;
            mode_stage_next();
        }
        break;
    }
    dec_bg_draw(1);
}

void prologue_mode_stage_line_04(void) {
    if (game_state.mode_step == 0) {
        prologue_line_04_run();
    }
}

void prologue_scene_05_transition_run(void) {

    /* FAKE: scalar selector view avoids the aggregate promotion copy. T-10860. */
    switch (*(u8 *)&game_state.substep) {
    case 0:
        if (prologue_scene_counter < 0x40) {
            prologue_scene_counter += 1;
        }
        if (prologue_scene_counter++ < 0x80) {
            D_801206BA = -0x20 - prologue_scene_counter;
        } else {
            audio_command(0x605);
            dec_bg_cd_read(0xACF6, 1);
            D_80120696 = 5;
            D_80120697 = 0xA4;
            D_801206AA = 0;
            D_801206AC = 1;
            D_80120699 = 0x80;
            game_state.substep += 1;
        }
        break;
    case 1:
        if ((D_801206AC == 3) && (((Actor *)actor_pool)[97].unk_08 == 0)) {
            audio_command(0x605);
        }
        if (!(D_80120696 & 1) && (cd_read_sync_status() & 1)) {
            prologue_scene_counter = 0x80;
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            prologue_background_slot_set(0x801A4000, 0xF);
            cd_read_sectors(0x31U, 0x801A4000, 0xAA54);
            D_801206D7 = 0x1D;
            game_state.substep += 1;
        }
        break;
    case 2:
        if (prologue_scene_counter < -0x20) {
            game_state.substep += 1;
        } else {
            prologue_scene_counter -= 3;
            D_801206BA = -0x20 - prologue_scene_counter;
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            dec_bg_cd_read(0xACD4, 1);
            game_state.substep += 1;
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x19U, 0x801F0000, 0xABCA);
            game_state.substep += 1;
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            prologue_background_slot_set(0x801A4000, 0xF);
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            if (bg_decode_result == -1) {
                dec_bg_reset();
                dec_bg_cd_read(0xACD4, 1);
            } else {
                actor_clear_upper();
                memcpy((void *)0x80180000, 0x801F0000, 0xC800);
                prologue_actor96_set_scene_06();
                prologue_actor99_set_scene_06();
                mode_stage_next();
            }
        }
        break;
    }
    dec_bg_draw(1);
}

void prologue_mode_stage_scene_05_transition(void) {
    if (game_state.mode_step == 0) {
        prologue_scene_05_transition_run();
    }
}

void prologue_actor96_set_scene_06(void) {
    D_80120651 = 0xA;
    D_80120652 = 0x40;
    D_80120688 = 0x41000000;
    D_80120653 = 0xA4;
    D_8012065C = (u8 *)0x801842C4;
    D_80120660 = (u8 *)0x801842F0;
    D_80120684 = (u8 *)0x801842BC;
    D_80120668 = 0;
    D_80120655 = 0x80;
    D_80120676 = -0xA0;
    D_8012067A = -0x78;
}

s32 prologue_lip_synchronization_yoshio(void) {
    /* FAKE: unused homes preserve the original frame and mean spill slot. T-10860. */
    s32 unused;
    s32 padding[1];
    s32 level;
    s32 frame;
    s32 average;

    /* FAKE: dead initialization changes allocator range order. T-10860. */
    average = frame = 0;
    if (cd_audio_state() & 4) {
        level = getCDlevel();
    } else {
        level = 0;
    }
    prologue_cd_level_history[game_state.frame_count & 3] = level;
    average = (prologue_cd_level_history[0] + prologue_cd_level_history[1] + prologue_cd_level_history[2] + prologue_cd_level_history[3]) / 4;
    frame = average / 10;
    prologue_cd_level_last = level;
    if (frame >= 4) {
        frame = 3;
    }
    actor_clear(0x61);
    if (cd_audio_state() & 4) {
        D_80120695 = 9;
        D_80120696 = 0x40;
        D_801206CC = 0x41000000;
        D_80120697 = 0xA4;
        D_801206A0 = 0x801842C4;
        D_801206A4 = 0x801842F0;
        D_801206C8 = 0x801842BC;
        D_801206AC = 6 - frame;
        D_80120699 = 0x80;
        D_801206BA = -0xA0;
        D_801206BE = -0x78;
    }
}

void prologue_actor102_set_scene_06(void) {
    D_801207E9 = 9;
    D_801207EA = 3;
    D_80120820 = 0x41000000;
    D_801207EB = 0xA4;
    D_801207F4 = 0x801842C4;
    D_801207F8 = 0x801842F0;
    D_8012081C = 0x801842BC;
    D_801207FE = 0;
    D_801207ED = 0x80;
    D_8012080E = -0xA0;
    D_80120812 = -0x78;
}

/* FAKE: the empty test of D_801206E0 makes its load the first instruction, as in the original; found with the permuter, T-9380 */
void prologue_actor98_step_scene_06(void) {
    if ((D_801206E0 && D_801206E0) && D_801206E0) {
    }
    D_801206D9 = 9;
    D_801206DA = 0x40;
    D_80120710 = 0x41000000;
    D_801206DB = 0xA4;
    D_801206E4 = 0x801842C4;
    D_801206E8 = 0x801842F0;
    D_8012070C = 0x801842BC;
    D_801206E0 += 1;
    if (D_801206E0 >= 5) {
        D_801206F0 = 0xA;
    } else if (D_801206E0 >= 5) {
        D_801206F0 = 9;
    } else {
        D_801206F0 = 8;
    }
    D_801206DD = 0x80;
    D_801206FE = -0xA0;
    D_80120702 = -0x78;
}

void prologue_actor99_set_scene_06(void) {
    D_8012071D = 9;
    D_8012071E = 0;
    D_80120754 = 0x41000000;
    D_8012071F = 0xA4;
    D_80120728 = 0x801800F8;
    D_8012072C = 0x80180114;
    D_80120750 = 0x801800F4;
    D_80120721 = 0x80;
    D_80120742 = -0xA0;
    D_80120746 = -0x78;
}

void prologue_actor99_set_scene_06_b(void) {
    D_8012071D = 9;
    D_8012071E = 5;
    D_80120754 = 0x41000000;
    D_8012071F = 0xA4;
    D_80120728 = 0x801800F8;
    D_8012072C = 0x80180114;
    D_80120750 = 0x801800F4;
    D_80120721 = 0x80;
    D_80120742 = -0xA0;
    D_80120746 = -0x78;
}

void prologue_actor100_mouth_girl_a(void) {
    if (cd_audio_state() & 4) {
        D_80120761 = 8;
        D_80120762 = 1;
        D_80120798 = 0x41000000;
        D_80120763 = 0xA4;
        D_8012076C = 0x801800F8;
        D_80120770 = 0x80180114;
        D_80120794 = 0x801800F4;
        D_80120776 = 1;
        D_80120765 = 0x80;
        D_80120786 = -0xA0;
        D_8012078A = -0x78;
        return;
    }
    D_80120762 = 0;
}

void prologue_actor101_mouth_girl_b(void) {
    if (cd_audio_state() & 4) {
        D_801207A5 = 8;
        D_801207A6 = 1;
        D_801207DC = 0x41000000;
        D_801207A7 = 0xA4;
        D_801207B0 = 0x801800F8;
        D_801207B4 = 0x80180114;
        D_801207D8 = 0x801800F4;
        D_801207BA = 2;
        D_801207A9 = 0x80;
        D_801207CA = -0xA0;
        D_801207CE = -0x78;
        return;
    }
    D_801207A6 = 0;
}

void prologue_actor103_set_scene_06(void) {
    D_8012082D = 8;
    D_8012082E = 0x40;
    D_80120864 = 0x41000000;
    D_8012082F = 0xA4;
    D_80120838 = 0x801842C4;
    D_8012083C = 0x801842F0;
    D_80120860 = 0x801842BC;
    D_80120844 = 7;
    D_80120831 = 0x80;
    D_80120852 = -0xA0;
    D_80120856 = -0x78;
}

void prologue_yoshio_greeting_run(void) {
    switch (game_state.substep) {
    case 0:
        prologue_line = 5;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 1:
        if (cd_audio_state() & 4) {
            game_state.substep += 1;
        }
        break;
    case 2:
        prologue_actor98_step_scene_06();
        prologue_line_run();
        prologue_lip_synchronization_yoshio();
        if (game_state.mode_step_timer.w == 4) {
            prologue_actor102_set_scene_06();
        }
        break;
    case 3:
        prologue_line = 6;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 4:
        prologue_line_run();
        prologue_lip_synchronization_yoshio();
        break;
    case 5:
        prologue_line = 7;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 6:
        prologue_line_run();
        prologue_lip_synchronization_yoshio();
        break;
    case 7:
        prologue_line = 8;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 8:
        prologue_line_run();
        break;
    case 9:
        audio_command(0x503);
        prologue_actor99_set_scene_06_b();
        prologue_line = 9;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 10:
        prologue_line_run();
        prologue_actor100_mouth_girl_a();
        break;
    case 11:
        prologue_actor103_set_scene_06();
        prologue_line = 0xA;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 12:
        prologue_line_run();
        prologue_actor101_mouth_girl_b();
        break;
    case 13:
        dec_bg_cd_read(0xACEC, 1);
        game_state.substep += 1;
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x31U, 0x801A4000, 0xAA85);
            game_state.substep += 1;
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x19U, 0x801F0000, 0xABE3);
            game_state.substep += 1;
        }
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            actor_clear_upper();
            memcpy((void *)0x80180000, 0x801F0000, 0xC800);
            prologue_background_slot_set(0x801A4000, 0xF);
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            D_80120651 = 0xA;
            D_80120652 = 0x40;
            D_80120688 = 0x41000000;
            D_80120653 = 0xA4;
            D_8012065C = (u8 *)0x8018014C;
            D_80120660 = (u8 *)0x80180170;
            D_80120684 = (u8 *)0x80180140;
            D_80120668 = 0;
            D_80120655 = 0x80;
            D_80120676 = -0xE0;
            D_8012067A = -0x78;
            mode_stage_next();
        }
        break;
    }
    dec_bg_draw(1);
}

void prologue_mode_stage_yoshio_greeting(void) {
    if (game_state.mode_step == 0) {
        prologue_yoshio_greeting_run();
    }
}

void prologue_lip_synchronization_line_11(void) {
    /* FAKE: unused homes preserve the original frame and mean spill slot. T-10860. */
    s32 unused;
    s32 padding[2];
    s32 level;
    s32 frame;
    s32 average;

    /* FAKE: dead initialization changes allocator range order. T-10860. */
    average = frame = 0;
    if (cd_audio_state() & 4) {
        level = getCDlevel();
    } else {
        level = 0;
    }
    prologue_cd_level_history[game_state.frame_count & 3] = level;
    average = (prologue_cd_level_history[0] + prologue_cd_level_history[1] + prologue_cd_level_history[2] + prologue_cd_level_history[3]) / 4;
    frame = average / 10;
    prologue_cd_level_last = level;
    if (frame >= 4) {
        frame = 3;
    }
    actor_clear(0x61);
    actor_clear(0x62);
    if (cd_audio_state() & 4) {
        D_80120695 = 9;
        D_80120696 = 0x40;
        D_801206CC = 0x41000000;
        D_80120697 = 0xA4;
        D_801206A0 = 0x8018014C;
        D_801206A4 = 0x80180170;
        D_801206C8 = 0x80180140;
        D_801206AC = frame + 1;
        D_80120699 = 0x80;
        D_801206BA = -0xA0;
        D_801206BE = -0x78;
        D_801206D9 = 9;
        D_801206DA = 0x40;
        D_80120710 = 0x41000000;
        D_801206DB = 0xA4;
        D_801206E4 = 0x8018014C;
        D_801206E8 = 0x80180170;
        D_8012070C = 0x80180140;
        D_801206F0 = frame + 5;
        D_801206DD = 0x80;
        D_801206FE = -0xA0;
        D_80120702 = -0x78;
    }
}

void prologue_line_11_run(void) {
    GameState *scene_state;

    scene_state = &game_state;
    switch (scene_state->substep) {
    case 0:
        prologue_line = 0xB;
        prologue_line_start();
        scene_state->substep += 1;
        break;
    case 1:
        if (cd_audio_state() & 4) {
            scene_state->substep += 1;
        }
        if (D_80120676 < -0xA0) {
            D_80120676 += 1;
        }
        break;
    case 2:
        prologue_line_run();
        prologue_lip_synchronization_line_11();
        if (D_80120676 < -0xA0) {
            D_801206FE = D_801206BA = (D_80120676 += 1);
        }
        break;
    case 3:
        dec_bg_cd_read(0xAD00, 1);
        scene_state->substep += 1;
        /* fallthrough */
    case 4:
        if (cd_read_sync_status() & 1) {
            audio_bank_load_start(1, 0, 0, 0xCCCB, 0xCC86, 0xCC82);
            audio_command(5);
            audio_command(0xB1);
            scene_state->substep += 1;
        }
        break;
    case 5:
        if (audio_bank_load_poll() == 1) {
            cd_read_sectors(0x31U, 0x801A4000, 0xAAB6);
            scene_state->substep += 1;
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(9U, 0x801F0000, 0xABFC);
            scene_state->substep += 1;
        }
        break;
    case 7:
        if (cd_read_sync_status() & 1) {
            actor_clear_upper();
            memcpy((void *)0x80180000, 0x801F0000, 0x4800);
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            set_dec_bri(0);
            k_sub_reset();
            prologue_background_slot_set(0x801A4000, 0xE);
            D_801220E1 = 0;
            D_801220E0 = 0;
            D_801220E2 = 0;
            actor_clear(0x60);
            D_80120651 = 0xC;
            D_80120652 = 0x40;
            D_80120688 = 0x41000000;
            D_80120653 = 0x84;
            D_8012065C = (u8 *)0x80180078;
            D_80120660 = (u8 *)0x80180088;
            D_80120684 = (u8 *)0x80180074;
            D_80120668 = 0;
            D_80120693 = 0xE;
            D_8012067A = -0x78;
            D_80120657 = 0;
            D_80120676 = -0xA0;
            prologue_white_palette_set();
            prologue_scene_counter = 0;
            audio_bank_start(1, 0);
            mode_stage_next();
        }
        break;
    }
    dec_bg_draw(1);
}

void prologue_mode_stage_line_11(void) {
    if (game_state.mode_step == 0) {
        prologue_line_11_run();
    }
}

void prologue_white_palette_set(void) {
    s32 i;

    for (i = 0; i < 128; i++) {
        game_state.scratch_words[i] = 0x7FFF7FFF;
    }
    game_state.scratch_words[0] = 0x7FFF0000;
    load_palette((s32)&game_state.scratch_words[0], 0xE, 1, 2, 0);
}

void prologue_flash_boxes_draw(void) {
    s32 unused; /* FAKE: unused 4-byte local, the original frame keeps the spill slot of var_v0 one word lower; real source unknown. T-6020 */
    s32 var_v0;

    if (prologue_scene_counter < 0x100) {
        var_v0 = prologue_scene_counter * 0x10101;
    } else {
        var_v0 = 0xFFFFFF;
    }
    prim_gradient_rect(-0xA0, -0x78, 0xA0, 0xA0, 0xB, var_v0, 0x81);
    prim_gradient_rect(0, -0x78, 0xA0, 0xA0, 0xB, var_v0, 0x81);
    dtd_on_tpage(0, 0, 0xB, 1, 1);
}

/* The original call sites use an integer-returning K&R fallthrough signature. */
s32 prologue_white_boxes_draw(void) {
    prim_gradient_rect(-0xA0, -0x78, 0xA0, 0xA0, 0xD, 0xFFFFFF, 1);
    prim_gradient_rect(0, -0x78, 0xA0, 0xA0, 0xD, 0xFFFFFF, 1);
}

s32 prologue_lip_synchronization_rei(void) {
    /* FAKE: unused homes preserve the 0x38 frame and spill at sp+0x28. T-10860. */
    s32 unused;
    s32 padding[1];
    s32 level;
    s32 frame;
    s32 average;

    /* FAKE: dead initialization changes allocator range order. T-10860. */
    average = frame = 0;
    if (cd_audio_state() & 4) {
        level = getCDlevel();
    } else {
        level = 0;
    }
    prologue_cd_level_history[game_state.frame_count & 3] = level;
    average = (prologue_cd_level_history[0] + prologue_cd_level_history[1] + prologue_cd_level_history[2] + prologue_cd_level_history[3]) / 4;
    if (average) {} /* FAKE: empty test splits the live range of average. T-11170 */
    if (average) {} /* FAKE: empty test splits the live range of average. T-11170 */
    frame = average / 10;
    prologue_cd_level_last = level;
    if (frame >= 4) {
        frame = 3;
    }
    if (cd_audio_state() & 4) {
        D_80120651 = 9;
        D_80120652 = 0x40;
        D_80120688 = 0x41000000;
        D_80120653 = 0xA4;
        D_8012065C = (u8 *)0x80180078;
        D_80120660 = (u8 *)0x80180088;
        D_80120684 = (u8 *)0x80180074;
        D_80120668 = (s16) frame;
        D_80120655 = 0x80;
        D_80120693 = 0;
        D_80120676 = -0xA0;
        D_8012067A = -0x78;
        return;
    }
    D_80120668 = 0;
}

void prologue_rei_banner_run(void) {
    s32 temp_v0;

    switch (game_state.substep) {
    case 0:
        prologue_flash_boxes_draw();
        prologue_scene_counter += 4;
        if (prologue_scene_counter >= 0x100) {
            game_state.substep += 1;
            prologue_scene_counter = 0;
        } else {
            temp_v0 = prologue_scene_counter / 4;
            D_801220E2 = (u8) temp_v0;
            D_801220E1 = (u8) temp_v0;
            D_801220E0 = (u8) temp_v0;
            D_80120657 = (u8) temp_v0;
        }
        break;
    case 1:
        prologue_white_boxes_draw();
        if (prologue_scene_counter++ >= 0xB) {
            D_80120693 = 0;
            D_80120655 = 0x80;
            D_80120657 = 0x80;
            prologue_background_slot_set(0x801A4000, 0xF);
            game_state.substep += 1;
            prologue_scene_counter = 0;
        }
        break;
    case 2:
        prologue_white_boxes_draw();
        D_801220E2 = (u8) prologue_scene_counter;
        D_801220E1 = (u8) prologue_scene_counter;
        D_801220E0 = (u8) prologue_scene_counter;
        D_80120657 = (u8) prologue_scene_counter;
        prologue_scene_counter += 8;
        if (prologue_scene_counter >= 0x81) {
            prologue_line = 0xC;
            prologue_scene_counter = 0;
            prologue_line_start();
            game_state.substep += 1;
        }
        break;
    case 3:
        prologue_white_boxes_draw();
        if (cd_audio_state() & 4) {
            game_state.substep += 1;
        }
        break;
    case 4:
        prologue_white_boxes_draw();
        prologue_line_run();
        prologue_lip_synchronization_rei();
        break;
    case 5:
        prologue_white_boxes_draw();
        cd_read_sectors(0x31U, 0x801A4000, 0xAAE7);
        game_state.substep += 1;
        break;
    case 6:
        prologue_white_boxes_draw();
        if (cd_read_sync_status() & 1) {
            bg_layers = 0x80000000;
            actor_clear(0x60);
            cd_read_sectors(0x19U, 0x80180000, 0xAC05);
            game_state.substep += 1;
        }
        break;
    case 7:
        prologue_white_boxes_draw();
        if (cd_read_sync_status() & 1) {
            set_dec_bri(0x80);
            actor_clear_upper();
            D_80120651 = 0xA;
            D_80120652 = 0x40;
            D_80120688 = 0x41000000;
            D_80120653 = 0xA4;
            D_8012065C = (u8 *)0x80184288;
            D_80120660 = (u8 *)0x801842AC;
            D_80120684 = (u8 *)0x80184280;
            D_80120668 = 0;
            D_80120655 = 0x80;
            D_80120676 = -0xA0;
            D_8012067A = -0x78;
            prologue_background_slot_set(0x801A4000, 0xF);
            bg_layers = 0x80000000;
            mode_stage_next();
        }
        break;
    }
    dec_bg_draw(1);
}

void prologue_mode_stage_rei_banner(void) {
    if (game_state.mode_step == 0) {
        prologue_rei_banner_run();
    }
}

s32 prologue_lip_synchronization_rei_lines_13_and_14(void) {
    /* FAKE: unused homes preserve the original frame and mean spill slot. T-10860. */
    s32 unused;
    s32 padding[1];
    s32 level;
    s32 frame;
    s32 average;

    /* FAKE: dead initialization changes allocator range order. T-10860. */
    average = frame = 0;
    if (cd_audio_state() & 4) {
        level = getCDlevel();
    } else {
        level = 0;
    }
    prologue_cd_level_history[game_state.frame_count & 3] = level;
    average = (prologue_cd_level_history[0] + prologue_cd_level_history[1] + prologue_cd_level_history[2] + prologue_cd_level_history[3]) / 4;
    frame = average / 10;
    prologue_cd_level_last = level;
    if (frame >= 4) {
        frame = 3;
    }
    actor_clear(0x61);
    if (cd_audio_state() & 4) {
        D_80120695 = 9;
        D_80120696 = 0x40;
        D_801206CC = 0x41000000;
        D_80120697 = 0xA4;
        D_801206A0 = 0x80184288;
        D_801206A4 = 0x801842AC;
        D_801206C8 = 0x80184280;
        D_801206AC = frame + 5;
        D_80120699 = 0x80;
        D_801206BA = -0xA0;
        D_801206BE = -0x78;
    }
}

void prologue_actor98_animation_start_rei(void) {
    D_801206D9 = 9;
    D_801206DA = 3;
    D_80120710 = 0x41000000;
    D_801206DB = 0xAC;
    D_801206E4 = 0x80184288;
    D_801206E8 = 0x801842AC;
    D_8012070C = 0x80184280;
    D_801206EE = 0;
    D_801206DD = 0x80;
    D_801206FE = -0xA0;
    D_80120702 = -0x78;
}

void prologue_actors99_100_set_rei(void) {
    D_8012071D = 0xC;
    D_8012071E = 0;
    D_80120754 = 0x41000000;
    D_8012071F = 0xA4;
    D_80120728 = 0x80180110;
    D_8012072C = 0x80180130;
    D_80120750 = 0x8018010C;
    D_80120732 = 0;
    D_80120721 = 0x80;
    D_80120742 = -0xA0;
    D_80120746 = -0x78;
    D_80120761 = 0xC;
    D_80120762 = 0;
    D_80120798 = 0x41000000;
    D_80120763 = 0xA4;
    D_8012076C = 0x80180110;
    D_80120770 = 0x80180130;
    D_80120794 = 0x8018010C;
    D_80120776 = 1;
    D_80120765 = 0x80;
    D_80120786 = -0xA0;
    D_8012078A = -0x78;
}

void prologue_rei_lines_13_and_14_run(void) {

    /* FAKE: scalar selector view avoids a cvt switch copy. T-10860. */
    switch (*(u8 *)&game_state.substep) {
    case 0:
        prologue_line = 0xD;
        prologue_line_start();
        prologue_scene_counter = 0;
        game_state.substep += 1;
        dec_bg_draw(1);
        break;
    case 1:
        if (prologue_scene_counter < 0xFF) {
            prologue_scene_counter += 1;
            prologue_flash_boxes_draw();
        }
        if (cd_audio_state() & 4) {
            game_state.substep += 1;
        }
        dec_bg_draw(1);
        break;
    case 2:
        if (prologue_scene_counter < 0xFF) {
            prologue_scene_counter += 1;
            prologue_flash_boxes_draw();
        } else {
            prologue_flash_boxes_draw();
        }
        prologue_line_run();
        dec_bg_draw(1);
        prologue_lip_synchronization_rei_lines_13_and_14();
        if (get_now_k() == ((u8)game_state.text_first_line + 3)) {
            game_state.substep += 1;
            bg_layers = 0x41000000;
            prologue_actors99_100_set_rei();
            prologue_flash_boxes_draw();
        }
        break;
    case 3:
        if (prologue_scene_counter >= 4) {
            prologue_flash_boxes_draw();
            prologue_scene_counter -= 4;
        }
        prologue_line_run();
        prologue_lip_synchronization_rei_lines_13_and_14();
        break;
    case 4:
        D_80120762 = 5;
        ((Actor *)actor_pool)[99].flags = 5;
        game_state.substep += 1;
        break;
    case 5:
        if ((((Actor *)actor_pool)[99].flags == 0) && (((Actor *)actor_pool)[99].flags == 0)) {
            game_state.substep++;
            prologue_scene_counter = 0;
        }
        break;
    case 6:
        if (prologue_scene_counter++ >= 0x1F) {
            game_state.substep++;
        }
        break;
    case 7:
        D_801206DA = 1;
        D_801206F0 = 3;
        D_801206E0 = 0;
        prologue_line = 0xE;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 8:
        if (D_801206F0 == 3) {
            D_801206DA = 0;
        }
        prologue_line_run();
        prologue_lip_synchronization_rei_lines_13_and_14();
        break;
    case 9:
        cd_read_sectors(0x31U, 0x801A4000, 0xAA85);
        game_state.substep += 1;
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x21U, 0x801E8000, 0xAC1E);
            game_state.substep += 1;
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            actor_clear_upper();
            memcpy((void *)0x80180000, 0x801E8000, 0x10800);
            D_80120651 = 0xA;
            D_80120652 = 0x40;
            D_80120688 = 0x41000000;
            D_80120653 = 0xA4;
            D_8012065C = (u8 *)0x80180150;
            D_80120660 = (u8 *)0x80180174;
            D_80120684 = (u8 *)0x80180140;
            D_80120668 = 0;
            D_80120655 = 0x80;
            D_80120676 = -0xA0;
            D_8012067A = -0x78;
            dec_bg_show_switch(1);
            prologue_background_slot_set(0x801A4000, 0xF);
            mode_stage_next();
        }
        break;
    }
    if ((u8) game_state.substep < 6U) {
        if ((rand() & 0xFF) == 0xFF) {
            if (!((u8) D_801206DA & 1)) {
                prologue_actor98_animation_start_rei();
            }
        }
    }
}

void prologue_mode_stage_rei_lines_13_and_14(void) {
    if (game_state.mode_step == 0) {
        prologue_rei_lines_13_and_14_run();
    }
}

s32 prologue_lip_synchronization_lines_15_and_16(void) {
    /* FAKE: unused homes preserve the original frame and mean spill slot. T-10860. */
    s32 unused;
    s32 padding[2];
    s32 level;
    s32 frame;
    s32 average;

    /* FAKE: dead initialization changes allocator range order. T-10860. */
    average = frame = 0;
    if (cd_audio_state() & 4) {
        level = getCDlevel();
    } else {
        level = 0;
    }
    prologue_cd_level_history[game_state.frame_count & 3] = level;
    average = (prologue_cd_level_history[0] + prologue_cd_level_history[1] + prologue_cd_level_history[2] + prologue_cd_level_history[3]) / 4;
    frame = average / 10;
    prologue_cd_level_last = level;
    if (frame >= 4) {
        frame = 3;
    }
    actor_clear(0x61);
    actor_clear(0x62);
    if (cd_audio_state() & 4) {
        D_80120695 = 9;
        D_80120696 = 0x40;
        D_801206CC = 0x41000000;
        D_80120697 = 0xA4;
        D_801206A0 = 0x80180150;
        D_801206A4 = 0x80180174;
        D_801206C8 = 0x80180140;
        D_801206AC = frame + 1;
        D_80120699 = 0x80;
        D_801206BA = -0xA0;
        D_801206BE = -0x78;
        D_801206D9 = 9;
        D_801206DA = 0x40;
        D_80120710 = 0x41000000;
        D_801206DB = 0xA4;
        D_801206E4 = 0x80180150;
        D_801206E8 = 0x80180174;
        D_8012070C = 0x80180140;
        D_801206F0 = frame + 5;
        D_801206DD = 0x80;
        D_801206FE = -0xA0;
        D_80120702 = -0x78;
    }
}

void prologue_lines_15_and_16_run(void) {
    ActorFixed *actors;
    actors = (ActorFixed *)actor_pool;
    switch (game_state.substep) {
    case 0:
        prologue_line = 0xF;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 1:
        if (cd_audio_state() & 4) {
            game_state.substep += 1;
        }
        break;
    case 2:
        prologue_lip_synchronization_lines_15_and_16();
        prologue_line_run();
        break;
    case 3:
        prologue_line = 0x10;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 4:
        prologue_line_run();
        break;
    case 5:
        cd_read_sectors(0x31U, 0x801A4000, 0xAB18);
        game_state.substep += 1;
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            prologue_palette_adjust();
            cd_read_sectors(0x11U, 0x801F0000, 0xAC3F);
            game_state.substep += 1;
        }
        break;
    case 7:
        if (cd_read_sync_status() & 1) {
            dec_bg_cd_read(0xACF6, 1);
            game_state.substep += 1;
        }
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            if (bg_decode_result == -1) {
                dec_bg_reset();
                dec_bg_cd_read(0xACF6, 1);
            } else {
                actor_clear_upper();
                set_dec_bri(0x80);
                memcpy((void *)0x80180000, 0x801F0000, 0x8800);
                prologue_background_slot_set(0x801A4000, 0xF);
                D_80120651 = 0xA;
                D_80120652 = 0;
                D_80120688 = 0x41000000;
                D_80120653 = 0xA4;
                D_8012065C = (u8 *)0x801800D8;
                D_80120660 = (u8 *)0x801800F0;
                D_80120684 = (u8 *)0x801800D4;
                D_80120666 = 0;
                D_80120655 = 0x80;
                actors[96].x.h[1] = -0xA0;
                D_8012067A = -0x78;
                audio_command(5);
                audio_command(0xC1);
                audio_bank_load_start(1, 0, 0, 0xCBD4, 0xCBAA, 0xCBA5);
                mode_stage_next();
            }
        }
        break;
    }
    if ((u8) game_state.substep >= 2U) {
        actors[96].x.w = (s32) (actors[96].x.w - 0x8000);
        actors[98].x.h[1] = actors[97].x.h[1] = actors[96].x.h[1];
    }
    dec_bg_draw(1);
}

void prologue_mode_stage_lines_15_and_16(void) {
    if (game_state.mode_step == 0) {
        prologue_lines_15_and_16_run();
    }
}

/* rec is record 96 of the actor_pool table (D_80120658 and D_80120668 are its +0x08 and +0x18): read through the
 * record, uopt compares constant-first as the original does. T-16930 */
void prologue_line_17_run(void) {
    Actor *rec = &((Actor *)actor_pool)[96];

    switch (game_state.substep) {
    case 0:
        prologue_line = 0x11;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 1:
        prologue_line_run();
        break;
    case 2:
        if (audio_bank_load_poll() & 1) {
            audio_bank_start(1, 0);
            cd_read_sectors(0x31U, 0x801A4000, 0xAB18);
            game_state.substep += 1;
            D_80120652 = 5;
        }
        break;
    case 3:
        if ((rec->unk_18 == 1) && (rec->unk_08 == 1)) {
            audio_command(0x604);
            game_state.substep += 1;
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            prologue_palette_adjust();
            dec_bg_cd_read(0xAD05, 1);
            game_state.substep += 1;
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x41U, 0x801D8000, 0xAC50);
            game_state.substep += 1;
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            if (bg_decode_result == -1) {
                dec_bg_reset();
                dec_bg_cd_read(0xAD05, 1);
            } else {
                actor_clear_upper();
                memcpy((void *)0x80180000, 0x801D8000, 0x20800);
                prologue_background_slot_set(0x801A4000, 0xF);
                bg_layers = 0x80000000;
                D_80120651 = 0xA;
                D_80120652 = 5;
                D_80120688 = 0x41000000;
                D_80120653 = 0xA4;
                D_8012065C = (u8 *)0x80180204;
                D_80120660 = (u8 *)0x80180220;
                D_80120684 = (u8 *)0x801801E4;
                D_80120666 = 0;
                D_80120655 = 0x80;
                D_80120676 = -0xA0;
                D_8012067A = -0x78;
                mode_stage_next();
            }
        }
        break;
    }
    dec_bg_draw(1);
}

void prologue_mode_stage_line_17(void) {
    if (game_state.mode_step == 0) {
        prologue_line_17_run();
    }
}

void prologue_shiori_load_run(void) {
    switch (game_state.substep) {
    case 0:
        cd_read_sectors(0x31U, 0x801A4000, 0xAB49);
        game_state.substep += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(9U, 0x801F0000, 0xAC91);
            game_state.substep += 1;
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_cd_read(0xAD0B, 1);
            game_state.substep += 1;
        }
        break;
    case 3:
        if ((cd_read_sync_status() & 1) && !(D_80120652 & 1)) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            if (bg_decode_result == -1) {
                dec_bg_reset();
                dec_bg_cd_read(0xAD0B, 1);
            } else {
                actor_clear_upper();
                prologue_background_slot_set(0x801A4000, 0xF);
                memcpy((void *)0x80180000, 0x801F0000, 0x4800);
                D_80120695 = 9;
                D_80120696 = 0x40;
                D_801206CC = 0x41000000;
                D_80120697 = 0xA4;
                D_801206A0 = 0x801800F4;
                D_801206A4 = 0x80180114;
                D_801206C8 = 0x801800F0;
                D_801206AC = 4;
                D_80120699 = 0x80;
                D_801206BA = -0xA0;
                D_801206BE = -0x78;
                mode_stage_next();
            }
        }
        break;
    }
    dec_bg_draw(1);
}

void prologue_mode_stage_shiori_load(void) {
    if (game_state.mode_step == 0) {
        prologue_shiori_load_run();
    }
}

s32 prologue_lip_synchronization_shiori(void) {
    /* FAKE: unused homes preserve the 0x38 frame and spill at sp+0x28. T-10860. */
    s32 unused;
    s32 padding[1];
    s32 level;
    s32 frame;
    s32 average;

    /* FAKE: dead initialization changes allocator range order. T-10860. */
    average = frame = 0;
    if (cd_audio_state() & 4) {
        level = getCDlevel();
    } else {
        level = 0;
    }
    prologue_cd_level_history[game_state.frame_count & 3] = level;
    average = (prologue_cd_level_history[0] + prologue_cd_level_history[1] + prologue_cd_level_history[2] + prologue_cd_level_history[3]) / 4;
    if (average) {} /* FAKE: empty test splits the live range of average. T-11170 */
    if (average) {} /* FAKE: empty test splits the live range of average. T-11170 */
    frame = average / 10;
    prologue_cd_level_last = level;
    if (frame >= 4) {
        frame = 3;
    }
    if (cd_audio_state() & 4) {
        D_80120695 = 9;
        D_80120696 = 0x40;
        D_801206CC = 0x41000000;
        D_80120697 = 0xA4;
        D_801206A0 = 0x801800F4;
        D_801206A4 = 0x80180114;
        D_801206C8 = 0x801800F0;
        D_801206AC = frame + 4;
        D_80120699 = 0x80;
        D_801206BA = -0xA0;
        D_801206BE = -0x78;
        return;
    }
    D_801206AC = 4;
}

void prologue_actor98_animation_start_shiori(void) {
    D_801206D9 = 9;
    D_801206DA = 0x11;
    D_80120710 = 0x41000000;
    D_801206DB = 0xAC;
    D_801206E4 = 0x801800F4;
    D_801206E8 = 0x80180114;
    D_8012070C = 0x801800F0;
    D_801206EE = 0;
    D_801206DD = 0x80;
    D_801206FE = -0xA0;
    D_80120702 = -0x78;
}

void prologue_shiori_line_18_run(void) {
    switch (game_state.substep) {
    case 0:
        prologue_line = 0x12;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 1:
        if (cd_audio_state() & 4) {
            game_state.substep += 1;
        }
        break;
    case 2:
        prologue_line_run();
        prologue_lip_synchronization_shiori();
        break;
    case 3:
        cd_read_sectors(0x21U, 0x801E8000, 0xAC9A);
        game_state.substep += 1;
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            actor_clear_upper();
            dec_bg_show_switch(1);
            bg_layers = 0x80000000;
            memcpy((void *)0x80180000, 0x801E8000, 0x10800);
            D_80120651 = 9;
            D_80120652 = 0x40;
            D_80120688 = 0x41000000;
            D_80120653 = 0xA4;
            D_8012065C = (u8 *)0x80180294;
            D_80120660 = (u8 *)0x801802C8;
            D_80120684 = (u8 *)0x80180284;
            D_80120668 = 0;
            D_80120655 = 0x80;
            D_80120676 = -0xA0;
            D_8012067A = -0x78;
            mode_stage_next();
        }
        break;
    }
    if (((rand() & 0xFF) == 0xFF) && !((u8) D_801206DA & 1)) {
        prologue_actor98_animation_start_shiori();
    }
    dec_bg_draw(1);
}

void prologue_mode_stage_shiori_line_18(void) {
    if (game_state.mode_step == 0) {
        prologue_shiori_line_18_run();
    }
}

s32 prologue_lip_synchronization_shiori_line_19(void) {
    /* FAKE: unused homes preserve the original frame and mean spill slot. T-10860. */
    s32 unused;
    s32 padding[1];
    s32 level;
    s32 frame;
    s32 average;

    /* FAKE: dead initialization changes allocator range order. T-10860. */
    average = frame = 0;
    if (cd_audio_state() & 4) {
        level = getCDlevel();
    } else {
        level = 0;
    }
    prologue_cd_level_history[game_state.frame_count & 3] = level;
    average = (prologue_cd_level_history[0] + prologue_cd_level_history[1] + prologue_cd_level_history[2] + prologue_cd_level_history[3]) / 4;
    frame = average / 10;
    prologue_cd_level_last = level;
    if (frame >= 4) {
        frame = 3;
    }
    actor_clear(0x61);
    if (cd_audio_state() & 4) {
        D_80120695 = 8;
        D_80120696 = 0x40;
        D_801206CC = 0x41000000;
        D_80120697 = 0xA4;
        D_801206A0 = 0x80180294;
        D_801206A4 = 0x801802C8;
        D_801206C8 = 0x80180284;
        D_801206AC = frame + 5;
        D_80120699 = 0x80;
        D_801206BA = -0xA0;
        D_801206BE = -0x78;
    }
}

void prologue_actor98_animation_start_shiori_line_19(void) {
    if (!(D_8012071E & 1)) {
        actor_clear(0x62);
        D_801206D9 = 8;
        D_801206DA = 3;
        D_80120710 = 0x41000000;
        D_801206DB = 0xAC;
        D_801206E4 = 0x80180294;
        D_801206E8 = 0x801802C8;
        D_8012070C = 0x80180284;
        D_801206EE = 0;
        D_801206DD = 0x80;
        D_801206FE = -0xA0;
        D_80120702 = -0x78;
    }
}

s32 prologue_actors_reset_shiori_line_19(void) {
    actor_clear(0x60);
    actor_clear(0x61);
    actor_clear(0x62);
    actor_clear(0x63);
    D_8012071D = 8;
    D_8012071E = 5;
    D_80120754 = 0x41000000;
    D_8012071F = 0xA4;
    D_80120728 = 0x80180294;
    D_8012072C = 0x801802C8;
    D_80120750 = 0x80180284;
    D_80120732 = 2;
    D_80120721 = 0x80;
    D_80120742 = -0xA0;
    D_80120746 = -0x78;
}

void prologue_shiori_line_19_run(void) {
    /* FAKE: scalar selector read avoids a cvt copy. T-10860. */
    switch (*(u8 *)&game_state.substep) {
    case 0:
        prologue_line = 0x13;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 1:
        prologue_line_run();
        prologue_lip_synchronization_shiori_line_19();
        break;
    case 2:
        prologue_actors_reset_shiori_line_19();
        game_state.substep += 1;
        break;
    case 3:
        if (!(D_8012071E & 1)) {
            game_state.substep += 1;
            dec_bg_cd_read(0xACEC, 1);
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x31U, 0x801A4000, 0xAB7A);
            game_state.substep += 1;
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x19U, 0x801E8000, 0xACBB);
            game_state.substep += 1;
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            if (bg_decode_result == -1) {
                dec_bg_reset();
                dec_bg_cd_read(0xACEC, 1);
            } else {
                actor_clear_upper();
                prologue_background_slot_set(0x801A4000, 0xF);
                memcpy((void *)0x80180000, 0x801E8000, 0xC800);
                D_80120651 = 0xB;
                D_80120652 = 0x40;
                D_80120688 = 0x41000000;
                D_80120653 = 0xA4;
                D_8012065C = (u8 *)0x80180258;
                D_80120660 = (u8 *)0x80180298;
                D_80120684 = (u8 *)0x8018024C;
                D_80120668 = 0;
                D_80120655 = 0x80;
                D_80120676 = -0xF0;
                D_8012067A = -0x78;
                mode_stage_next();
            }
        }
        break;
    }
    if (((rand() & 0xFF) == 0xFF) && !((u8) D_801206DA & 1)) {
        prologue_actor98_animation_start_shiori_line_19();
    }
    dec_bg_draw(1);
}

void prologue_mode_stage_shiori_line_19(void) {
    if (game_state.mode_step == 0) {
        prologue_shiori_line_19_run();
    }
}

/* FAKE: implicit-int exit liveness is needed; void changes three
 * instructions. Callers ignore the value. T-20009. */
s32 prologue_lip_synchronization_lines_20_to_31(void) {
    Actor *actors;
    /* FAKE: two unused words keep the original 0x40-byte frame; removing
     * them changes only the two stack adjustments to 0x38. T-20009. */
    s32 padding[2];
    s32 level;
    s32 frame;
    s32 average;

    actors = (Actor *)actor_pool;
    /* FAKE: dead joined initialization preserves the original CD-level
     * and average register order, as in prologue_lip_synchronization_lines_15_and_16. T-20009. */
    average = frame = 0;
    if (cd_audio_state() & 4) {
        level = getCDlevel();
    } else {
        level = 0;
    }
    prologue_cd_level_history[game_state.frame_count & 3] = level;
    average = (prologue_cd_level_history[0] + prologue_cd_level_history[1] + prologue_cd_level_history[2] + prologue_cd_level_history[3]) / 4;
    frame = average / 10;
    prologue_cd_level_last = level;
    if (frame >= 4) {
        frame = 3;
    }
    actor_clear(0x61);
    if (cd_audio_state() & 4) {
        actors[97].unk_01 = 9;
        actors[97].flags = 0x40;
        actors[97].quad_flags = 0x41000000;
        actors[97].draw_flags = 0xA4;
        actors[97].frame_table = (u8 *)0x80180258;
        actors[97].sequence_table = (u8 *)0x80180298;
        actors[97].tpage_table = (u8 *)0x8018024C;
        actors[97].unk_18 = frame + 6;
        actors[97].unk_05 = 0x80;
        actors[97].x = actors[96].x;
        actors[97].y = -0x78;
        actors[98].unk_01 = 9;
        actors[98].flags = 0x40;
        actors[98].quad_flags = 0x41000000;
        actors[98].draw_flags = 0xA4;
        actors[98].frame_table = (u8 *)0x80180258;
        actors[98].sequence_table = (u8 *)0x80180298;
        actors[98].tpage_table = (u8 *)0x8018024C;
        actors[98].unk_18 = frame + 10;
        actors[98].unk_05 = 0x80;
        actors[98].x = actors[96].x;
        actors[98].y = -0x78;
        actors[100].flags = 0;
    }
}

void prologue_actor99_set_lines_20_to_31(void) {
    D_8012071D = 9;
    D_8012071E = 5;
    D_80120754 = 0x41000000;
    D_8012071F = 0xA4;
    D_80120728 = 0x80180258;
    D_8012072C = 0x80180298;
    D_80120750 = 0x8018024C;
    D_80120721 = 0x80;
    D_80120742 = -0xA0;
    D_80120746 = -0x78;
}

void prologue_actor100_set_lines_20_to_31(void) {
    D_80120761 = 0xA;
    D_80120762 = 0;
    D_80120798 = 0x41000000;
    D_80120763 = 0xA4;
    D_8012076C = 0x80180258;
    D_80120770 = 0x80180298;
    D_80120794 = 0x8018024C;
    D_80120776 = 3;
    D_80120765 = 0x80;
    D_80120786 = -0xA0;
    D_8012078A = -0x38;
    D_800B5BD4 = 9;
}

void prologue_actor100_pulse(void) {
    if (!(D_80120762 & 1)) {
        D_80120762 = 1;
    }
    D_800B5BD4 = 0xF;
}

void prologue_lines_20_to_31_run(void) {
    s32 x;
    /* FAKE: unused word puts text at sp+0x38 in the 0xC0 frame. T-10860. */
    u32 unused;
    u8 text[128];
    s32 i;

    GameState *prologue_state;

    prologue_state = &game_state;
    switch (prologue_state->substep) {
    case 0:
        x = D_80120676;
        if (x < -0xA0) {
            D_801206BA = D_80120676 = x + 1;
        }
        prologue_line = 0x14;
        prologue_line_start();
        prologue_state->substep += 1;
        break;
    case 1:
        x = D_80120676;
        if (x < -0xA0) {
            D_801206BA = D_80120676 = x + 1;
        }
        prologue_line_run();
        prologue_lip_synchronization_lines_20_to_31();
        break;
    case 2:
        prologue_line = 0x15;
        prologue_line_start();
        prologue_state->substep += 1;
        break;
    case 3:
        prologue_line_run();
        break;
    case 4:
        prologue_actor99_set_lines_20_to_31();
        prologue_state->substep += 1;
        break;
    case 5:
        if (!(D_8012071E & 1)) {
            prologue_state->substep += 1;
        }
        break;
    case 6:
        prologue_line = 0x16;
        prologue_line_start();
        prologue_state->substep += 1;
        break;
    case 7:
        prologue_line_run();
        prologue_lip_synchronization_lines_20_to_31();
        break;
    case 8:
        prologue_line = 0x17;
        prologue_line_start();
        prologue_state->substep += 1;
        break;
    case 9:
        prologue_line_run();
        break;
    case 10:
        prologue_actor100_set_lines_20_to_31();
        prologue_line = 0x18;
        prologue_line_start();
        prologue_state->substep += 1;
        break;
    case 11:
        if (D_8012078A >= -0x77) {
            D_8012078A -= 1;
        }
        prologue_line_run();
        prologue_lip_synchronization_lines_20_to_31();
        break;
    case 12:
        k_sub_reset();
        gnsx(&prologue_state->player_family_name[0]);
        /* FAKE: same-line body fixes unrolled store scheduling. T-10860. */
        for (i = 0; i < 128; i++) { text[i] = 0; }
        strcpy(text, D_800B3288[((NameEntryBirthdayBits *)&prologue_state->girls[0].birthday)->month]);
        strncat(text, "月", 2);
        if (((NameEntryBirthdayBits *)&prologue_state->girls[0].birthday)->day >= 0xAU) {
            strncat(text, D_800B3288[((NameEntryBirthdayBits *)&prologue_state->girls[0].birthday)->day / 10U], 2);
            }
        strncat(text, D_800B3288[((NameEntryBirthdayBits *)&prologue_state->girls[0].birthday)->day % 10U], 2);
        strncat(text, "日。", 4);
        sndisp(text, 0, 0xF);
        prologue_state->current_girl = GIRL_PLAYER;
        k_sub_disp_start(1);
        prologue_state->substep += 1;
        break;
    case 13:
        prologue_line_run();
        prologue_actor100_pulse();
        break;
    case 14:
        prologue_line = 0x19;
        prologue_line_start();
        prologue_state->substep += 1;
        break;
    case 15:
        prologue_line_run();
        prologue_lip_synchronization_lines_20_to_31();
        break;
    case 16:
        k_sub_reset();
        gnsx(&prologue_state->player_family_name[0]);
        /* FAKE: same-line body fixes unrolled store scheduling. T-10860. */
        for (i = 0; i < 128; i++) { text[i] = 0; }
        strcpy(text, prologue_blood_texts[((NameEntryBirthdayBits *)&prologue_state->girls[0].birthday)->blood_type]);
        strncat(text, "型。", 4);
        sndisp(text, 0, 0xF);
        prologue_state->current_girl = GIRL_PLAYER;
        k_sub_disp_start(1);
        prologue_state->substep += 1;
        break;
    case 17:
        prologue_line_run();
        prologue_actor100_pulse();
        break;
    case 18:
        prologue_line = 0x1A;
        prologue_line_start();
        prologue_state->substep += 1;
        break;
    case 19:
        prologue_line_run();
        prologue_lip_synchronization_lines_20_to_31();
        break;
    case 20:
        prologue_line = 0x1B;
        prologue_line_start();
        prologue_state->substep += 1;
        break;
    case 21:
        prologue_line_run();
        prologue_actor100_pulse();
        break;
    case 22:
        prologue_line = 0x1C;
        prologue_line_start();
        prologue_state->substep += 1;
        break;
    case 23:
        prologue_line_run();
        prologue_lip_synchronization_lines_20_to_31();
        break;
    case 24:
        prologue_line = 0x1D;
        prologue_line_start();
        prologue_state->substep += 1;
        break;
    case 25:
        prologue_line_run();
        break;
    case 26:
        x = D_80120676;
        if (x >= -0x18F) {
            /* FAKE: signed narrowing preserves the original temporary sequence. T-10860. */
            x = (s16)(x - 2);
            D_801207CA = x;
            D_80120786 = x;
            D_80120742 = x;
            D_801206FE = x;
            D_801206BA = x;
            D_80120676 = x;
        } else {
            prologue_state->substep += 1;
        }
        break;
    case 27:
        prologue_line = 0x1E;
        prologue_line_start();
        prologue_state->substep += 1;
        break;
    case 28:
        prologue_line_run();
        break;
    case 29:
        prologue_line = 0x1F;
        prologue_line_start();
        prologue_state->substep += 1;
        break;
    case 30:
        prologue_line_run();
        break;
    case 31:
        dec_bg_cd_read(0xACD4, 1);
        prologue_state->substep += 1;
        break;
    case 32:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x31U, 0x801A4000, 0xAA54);
            prologue_state->substep += 1;
        }
        break;
    case 33:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x19U, 0x801E8000, 0xABCA);
            prologue_state->substep += 1;
        }
        break;
    case 34:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            if (bg_decode_result == -1) {
                dec_bg_reset();
                cd_read_sectors(0x31U, 0x801A4000, 0xAA54);
            } else {
                actor_clear_upper();
                prologue_background_slot_set(0x801A4000, 0xF);
                memcpy((void *)0x80180000, 0x801E8000, 0xC800);
                prologue_actor96_set_scene_06();
                prologue_actor99_set_scene_06();
                mode_stage_next();
            }
        }
        break;
    }
    dec_bg_draw(1);
}

void prologue_mode_stage_lines_20_to_31(void) {
    if (game_state.mode_step == 0) {
        prologue_lines_20_to_31_run();
    }
}

void prologue_lines_32_to_36_run(void) {
    switch (game_state.substep) {
    case 0:
        prologue_line = 0x20;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 1:
        prologue_line_run();
        prologue_lip_synchronization_yoshio();
        break;
    case 2:
        prologue_line = 0x21;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 3:
        prologue_line_run();
        break;
    case 4:
        audio_bank_start(0, 1);
        prologue_line = 0x22;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 5:
        prologue_line_run();
        prologue_lip_synchronization_yoshio();
        break;
    case 6:
        prologue_line = 0x23;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 7:
        prologue_line_run();
        break;
    case 8:
        prologue_line = 0x24;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 9:
        prologue_line_run();
        prologue_lip_synchronization_yoshio();
        break;
    case 10:
        dec_bg_cd_read(0x496A, 1);
        game_state.substep += 1;
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            if (bg_decode_result == -1) {
                dec_bg_reset();
                dec_bg_cd_read(0x496A, 1);
            } else {
                actor_clear_upper();
                bg_layers = 0x80000000;
                mode_stage_next();
            }
        }
        break;
    }
    dec_bg_draw(1);
}

void prologue_mode_stage_lines_32_to_36(void) {
    if (game_state.mode_step == 0) {
        prologue_lines_32_to_36_run();
    }
}

void prologue_line_37_run(void) {
    switch (game_state.substep) {
    case 0:
        prologue_line = 0x25;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 1:
        prologue_line_run();
        break;
    case 2:
        dec_bg_cd_read(0x4973, 1);
        game_state.substep += 1;
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            if (bg_decode_result == -1) {
                dec_bg_reset();
                dec_bg_cd_read(0x4973, 1);
            } else {
                cd_read_sectors(0x41, 0x80180000, 0xAC50);
                mode_stage_next();
            }
        }
        break;
    }
    dec_bg_draw(1);
}

void prologue_mode_stage_line_37(void) {
    if (game_state.mode_step == 0) {
        prologue_line_37_run();
    }
}

void prologue_lines_38_to_41_run(void) {
    switch (game_state.substep) {
    case 0:
        prologue_line = 0x26;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 1:
        if (cd_audio_state() & 4) {
            game_state.substep += 1;
        }
        break;
    case 2:
        prologue_line_run();
        break;
    case 3:
        prologue_line = 0x27;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 4:
        prologue_line_run();
        break;
    case 5:
        prologue_line = 0x28;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 6:
        prologue_line_run();
        break;
    case 7:
        prologue_line = 0x29;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 8:
        prologue_line_run();
        break;
    case 9:
        prologue_scene_counter = 0;
        mode_stage_next();
        break;
    }
    dec_bg_draw(1);
}

void prologue_mode_stage_lines_38_to_41(void) {
    if (game_state.mode_step == 0) {
        prologue_lines_38_to_41_run();
    }
}

void prologue_flash_run(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;

    switch (game_state.substep) {
    case 0:
        prologue_flash_boxes_draw();
        temp_v0 = prologue_scene_counter + 4;
        prologue_scene_counter = temp_v0;
        if (temp_v0 >= 0x100) {
            actor_clear(0x60);
            D_80120651 = 0xC;
            D_80120652 = 5;
            D_80120688 = 0x41000000;
            D_80120653 = 0xA4;
            D_8012065C = (u8 *)0x80180204;
            D_80120660 = (u8 *)0x80180220;
            D_80120684 = (u8 *)0x801801E4;
            D_80120666 = 0;
            D_80120656 = 2;
            D_80120655 = 0x80;
            D_80120676 = -0xA0;
            D_8012067A = -0x78;
            game_state.substep += 1;
        }
        break;
    case 1:
        prologue_white_boxes_draw();
        prologue_flash_boxes_draw();
        temp_v0_2 = prologue_scene_counter - 4;
        prologue_scene_counter = temp_v0_2;
        if (temp_v0_2 < 3) {
            game_state.substep += 1;
        }
        break;
    case 2:
        prologue_white_boxes_draw();
        prologue_flash_boxes_draw();
        temp_v0_3 = prologue_scene_counter + 4;
        prologue_scene_counter = temp_v0_3;
        if (temp_v0_3 >= 0x100) {
            game_state.substep += 1;
            D_80120653 = 0;
        }
        break;
    case 3:
        prologue_flash_boxes_draw();
        temp_v0_4 = prologue_scene_counter - 4;
        prologue_scene_counter = temp_v0_4;
        if (temp_v0_4 < 3) {
            game_state.substep += 1;
        }
        break;
    case 4:
        mode_stage_next();
        break;
    }
    dec_bg_draw(1);
}

void prologue_mode_stage_flash(void) {
    if (game_state.mode_step == 0) {
        prologue_flash_run();
    }
}

void prologue_closing_run(void) {
    switch (game_state.substep) {
    case 0:
        prologue_line = 0x2A;
        prologue_scene_counter = 0;
        prologue_line_start();
        game_state.substep += 1;
        break;
    case 1:
        prologue_line_run();
        break;
    case 2:
        if (prologue_scene_counter++ >= 0x3D) {
            audio_command(5);
            audio_command(0xD1);
            mode_stage_next();
        }
        break;
    }
    dec_bg_draw(1);
}

void prologue_mode_stage_closing(void) {
    if (game_state.mode_step == 0) {
        prologue_closing_run();
    }
}

void prologue_finish_run(void) {
    switch (game_state.substep) {
    case 0:
        cd_command_reset();
        D_800B5BD4 = 0xF;
        game_state.screen_brightness = 0x80;
        audio_bank_state_reset();
        audio_command(5);
        audio_command(0xD1);
        audio_bank_load_start(0, 0xBF98, 0xBF79, 0xD3D5, 0xD387, 0xD37D);
        game_state.substep += 1;
        return;
    default:
    case 1:
        D_800B3DB0 = 0;
        k_speed_set(TEXT_SPEED_SLOW);
        game_state.menu_mode = MENU_MODE_A;
        game_mode_set(GAME_MODE_COMMAND_SCREEN);
        mode_stage_set(0xFF);
        return;
    }
}

void prologue_mode_stage_finish(void) {
    if (game_state.mode_step == 0) {
        prologue_finish_run();
    }
}

void prologue_mode_handler(void) {
    switch (game_state.mode_stage) {
    case 0:
        prologue_mode_stage_arrival();
        break;
    case 1:
        prologue_mode_stage_board();
        break;
    case 2:
        prologue_mode_stage_names();
        break;
    case 3:
        prologue_mode_stage_line_03();
        break;
    case 4:
        prologue_mode_stage_line_04();
        break;
    case 5:
        prologue_mode_stage_scene_05_transition();
        break;
    case 6:
        prologue_mode_stage_yoshio_greeting();
        break;
    case 7:
        prologue_mode_stage_line_11();
        break;
    case 8:
        prologue_mode_stage_rei_banner();
        break;
    case 9:
        prologue_mode_stage_rei_lines_13_and_14();
        break;
    case 10:
        prologue_mode_stage_lines_15_and_16();
        break;
    case 11:
        prologue_mode_stage_line_17();
        break;
    case 12:
        prologue_mode_stage_shiori_load();
        break;
    case 13:
        prologue_mode_stage_shiori_line_18();
        break;
    case 14:
        prologue_mode_stage_shiori_line_19();
        break;
    case 15:
        prologue_mode_stage_lines_20_to_31();
        break;
    case 16:
        prologue_mode_stage_lines_32_to_36();
        break;
    case 17:
        prologue_mode_stage_line_37();
        break;
    case 18:
        prologue_mode_stage_lines_38_to_41();
        break;
    case 19:
        prologue_mode_stage_flash();
        break;
    case 20:
        prologue_mode_stage_closing();
        break;
    case 21:
        prologue_mode_stage_finish();
        break;
    }
    message_window_title_update();
    if (game_state.mode_stage == 0) {
        hud_screen_frame_draw(0);
    } else {
        hud_screen_frame_draw(2);
    }
    message_window_show();
    if (((game_state.mode_step != 0) || (game_state.mode_stage != 0)) && (game_state.pad_pressed & PAD_START)) {
        mode_stage_set(0x15);
    }
}
