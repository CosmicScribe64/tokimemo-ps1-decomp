/* drama_final_year_show.c: src/ovl/BUNKA_SD/801369D0.c:4; drama show callbacks, event-list interpreter and images; club/year dispatch is proved by src/ovl/culture_stage_shows/club_year_show_dispatch.c:14-51,63-89. Original object 0x801369D0-0x80137890 (config/objects/BUNKA_SD.txt). */
#include "common.h"
#include "ovl/culture_stage_shows.h"

s32 stage_show_scene_b_data_load(void) {
    if (game_state.substep == 0) {
        audio_bank_load_start(1, 0xBEFB, 0xBEE2, 0xD19D, 0xD170, 0xD164);
        game_state.substep = game_state.substep + 1;
    } else if (game_state.substep == 1) {
        if (audio_bank_load_poll() == 1) {
            game_state.substep = game_state.substep + 1;
        }
    } else if (game_state.substep == 2) {
        if (audio_bank_transfer_done(1) == 1) {
            game_state.substep = game_state.substep + 1;
        }
    } else if (game_state.substep == 3) {
        cd_read_sectors(0x6E, 0x80180000, 0x4EAF);
        game_state.substep = game_state.substep + 1;
    } else if (game_state.substep == 4) {
        if (cd_read_sync_status() & 1) {
            stage_show_final_text_table = D_8013C750;
            stage_show_final_voice_ranges = D_8013C7B4;
            game_state.current_girl = GIRL_SOTOI;
            mode_step_next();
        }
    }
}

void stage_show_scene_b_input_update(void) {
    if (game_state.mode_step >= 3 && game_state.mode_step < 0x26 && (game_state.pad_held & PAD_CIRCLE)) {
        if (cd_audio_state() != 2) {
            audio_command(0x74);
        }
        stage_show_scene_b_dispatch();
    }
    if (game_state.mode_step >= 3 && game_state.mode_step < 0x24 && (game_state.pad_held & PAD_CROSS)) {
        audio_command(0xC1);
        audio_command(0xC4);
        mode_step_set(0x23);
    }
    stage_show_scene_b_dispatch();
}

void stage_show_scene_b_dispatch(void) {
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    switch (game_state.mode_step) {
    case 0:
        stage_show_scene_state_reset();
        audio_command(0x7F);
        break;
    case 1:
        stage_show_scene_b_data_load();
        break;
    case 2:
        audio_command(0x200);
        stage_show_drama_final_show_state_initialize();
        break;
    case 3:
        stage_show_actor_brightness_ramp_up();
        break;
    case 4:
        stage_show_drama_second_timer_gate(0x78U);
        break;
    case 5:
        stage_show_actor_brightness_ramp_down();
        break;
    case 6:
        stage_show_drama_final_event_actor_set_a();
        break;
    case 7:
        stage_show_actor_brightness_ramp_up();
        break;
    case 8:
    case 9:
    case 10:
    case 11:
        stage_show_scene_b_event_list_run();
        break;
    case 12:
        audio_command(0xC1);
        mode_step_next();
        break;
    case 13:
        stage_show_actor_brightness_ramp_down();
        break;
    case 14:
        stage_show_drama_final_event_list_step();
        break;
    case 15:
        stage_show_drama_final_timer_gate(0x3C);
        break;
    case 16:
        stage_show_drama_final_event_actor_clear_and_set();
        break;
    case 17:
        stage_show_drama_final_timer_gate(0x3C);
        break;
    case 18:
        audio_command(0x201);
        mode_step_next();
        break;
    case 19:
        stage_show_actor_brightness_ramp_up();
        break;
    case 20:
        stage_show_drama_final_timer_gate(0x78);
        break;
    case 21:
        stage_show_actor_brightness_ramp_down();
        break;
    case 22:
        stage_show_drama_final_timer_gate(0x1E);
        break;
    case 23:
        stage_show_drama_final_event_list_step();
        break;
    case 24:
        stage_show_drama_final_timer_gate(0xF);
        break;
    case 25:
        stage_show_drama_final_event_list_step();
        break;
    case 26:
        stage_show_drama_final_event_actor_set_b();
        break;
    case 27:
        stage_show_actor_brightness_ramp_up();
        break;
    case 28:
    case 29:
    case 30:
        stage_show_scene_b_event_list_run();
        break;
    case 31:
        audio_command(0x202);
        mode_step_next();
        break;
    case 32:
        stage_show_drama_final_timer_gate(0x78);
        break;
    case 33:
    case 34:
        stage_show_scene_b_event_list_run();
        break;
    case 35:
        audio_command(0xC1);
        audio_command(0x500);
        k_reset(1);
        mode_step_next();
        break;
    case 36:
        if ((u32) game_state.mode_step_timer.u >= 0x1FU) {
            mode_step_next();
        }
        break;
    case 37:
        stage_show_actor_brightness_ramp_down();
        break;
    case 38:
        stage_show_scene_finish_wait();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    game_state.mode_step_timer.u += 1;
}

void stage_show_drama_final_show_state_initialize(void) {
    stage_show_drama_final_show_actor_table_initialize();
    k_speed_set(TEXT_SPEED_EVERY_SIX_FRAMES);
    stage_show_final_text_line_index = 0;
    stage_show_scene_b_event_cursor = 1;
    stage_show_scene_b_script_index = 0;
    stage_show_scene_b_frame = 0;
    stage_show_scene_b_interpreter_state = 0;
    stage_show_scene_b_text_active = 0;
    mode_step_next();
}

void stage_show_drama_final_show_actor_table_initialize(void) {
    s32 i;

    for (i = 0; i < 0x10; i++) {
        load_palette(D_8013CEE8, (i + 0x10) & 0xFF, 1, 1, i);
    }
    for (i = 0; i < 0x40; i++) {
        actor_clear(i + 0x60);
        ((Actor *)actor_pool)[96 + i].unk_01 = 4;
        ((Actor *)actor_pool)[96 + i].quad_flags = 0x01000000;
        ((Actor *)actor_pool)[96 + i].draw_flags = 0;
        ((Actor *)actor_pool)[96 + i].flags = 0x40;
        ((Actor *)actor_pool)[96 + i].frame_table = D_8013CEE0;
        ((Actor *)actor_pool)[96 + i].sequence_table = D_8013CEE4;
        ((Actor *)actor_pool)[96 + i].tpage_table = (u8 *)D_8013CEDC;
        ((Actor *)actor_pool)[96 + i].unk_18 = 0;
        ((Actor *)actor_pool)[96 + i].blink_period = 1;
        ((Actor *)actor_pool)[96 + i].brightness = 0;
        ((Actor *)actor_pool)[96 + i].x = -0xA0;
        ((Actor *)actor_pool)[96 + i].y = -0x78;
        ((Actor *)actor_pool)[96 + i].unk_05 = 0x80;
        ((Actor *)actor_pool)[96 + i].unk_43 = 0;
        ((Actor *)actor_pool)[96 + i].unk_04 = 0;
    }
    stage_show_drama_final_actor_slot_state_set(0, 0x40, 1);
    stage_show_drama_final_actor_slot_state_set(0x1F, 0x44, 1);
    load_tpage_buf_lock(5, D_8013CEDC[0], 1);
    load_tpage_buf_lock(6, D_8013CEDC[1], 1);
    load_tpage_buf_lock(7, D_8013CEDC[2], 1);
    load_tpage_buf_lock(8, D_8013CEDC[3], 1);
    load_tpage_buf_lock(9, D_8013CEDC[4], 1);
    load_tpage_buf_lock(0xA, D_8013CEDC[5], 1);
    load_tpage_buf_lock(0x25, D_8013CEDC[6], 1);
    load_tpage_buf_lock(0x26, D_8013CEDC[7], 1);
    load_tpage_buf_lock(0x27, D_8013CEDC[8], 1);
    load_tpage_buf_lock(0x28, D_8013CEDC[9], 1);
    load_tpage_buf_lock(0x29, D_8013CEDC[10], 1);
    load_tpage_buf_lock(0x2A, D_8013CEDC[11], 1);
    load_tpage_buf_lock(0x15, D_8013CEDC[12], 1);
}

void stage_show_drama_final_event_actor_set_a(void) {
    stage_show_drama_final_actor_slot_state_set(0, 0x44, 1);
    stage_show_drama_final_actor_slot_state_set(1, 0x18, 1);
    stage_show_drama_final_actor_slot_state_set(2, 0, 1);
    stage_show_drama_final_actor_slot_state_set(3, 1, 1);
    stage_show_drama_final_actor_slot_state_set(0x1F, 0x41, 1);
    mode_step_next();
}

void stage_show_drama_final_event_actor_clear_and_set(void) {
    stage_show_drama_final_actor_slot_state_set(1, 0xFF, 0);
    stage_show_drama_final_actor_slot_state_set(2, 0xFF, 0);
    stage_show_drama_final_actor_slot_state_set(3, 0xFF, 0);
    stage_show_drama_final_actor_slot_state_set(4, 0xFF, 0);
    stage_show_drama_final_actor_slot_state_set(5, 0xFF, 0);
    stage_show_drama_final_actor_slot_state_set(0, 0x44, 1);
    stage_show_drama_final_actor_slot_state_set(0x1F, 0x42, 1);
    mode_step_next();
}

void stage_show_drama_final_event_actor_set_b(void) {
    stage_show_drama_final_actor_slot_state_set(0, 0x44, 1);
    stage_show_drama_final_actor_slot_state_set(1, 0x26, 1);
    stage_show_drama_final_actor_slot_state_set(2, 0x27, 1);
    stage_show_drama_final_actor_slot_state_set(0x1F, 0x43, 1);
    mode_step_next();
}

void stage_show_drama_final_event_list_step(void) {
    if (game_state.mode_step_timer.u == 1) {
        stage_show_drama_final_kanji_line_start(stage_show_scene_b_text_script_addresses[stage_show_scene_b_script_index]);
        game_state.mode_stage_timer = 1;
        stage_show_drama_final_event_text_and_audio_display();
        return;
    }
    stage_show_drama_final_event_text_and_audio_display();
    if (game_state.mode_stage_timer == 0) {
        stage_show_scene_b_script_index += 1;
        stage_show_scene_b_event_cursor = 1;
        stage_show_scene_b_frame = 0;
        stage_show_scene_b_interpreter_state = 0;
        k_reset(1);
        mode_step_next();
    }
}

s32 stage_show_scene_b_event_list_run(void) {
    BsdEvent *list;
    BsdEvent *ev;
    s32 i;
    s32 slot;
    s32 img;

    if (game_state.mode_stage_timer == 0) {
        stage_show_scene_b_text_active = 0;
        if (stage_show_scene_b_interpreter_state != 0) {
            switch (stage_show_scene_b_interpreter_state) {
            case 1:
                game_state.mode_step_timer.w = 0;
                break;
            case 2:
                stage_show_scene_b_script_index++;
                stage_show_scene_b_frame = 0;
                stage_show_scene_b_interpreter_state = 0;
                stage_show_scene_b_event_cursor = 1;
                k_reset(1);
                mode_step_next();
                return 0;
            }
        }
    }
    list = stage_show_scene_b_event_lists[stage_show_scene_b_script_index];
    if (stage_show_scene_b_interpreter_state == 0) {
        stage_show_scene_b_interpreter_state = 1;
    }
    if (stage_show_scene_b_interpreter_state == 1) {
        for (i = stage_show_scene_b_event_cursor; i < 0xFFFF; i++) {
            /* FAKE: T-22003, the word view with i * 2 takes the ugen temporary that the
             * original's index expression takes; &list[i] gives $t1 for $t2 in the loop */
            ev = (BsdEvent *)((s32 *)list + i * 2);
            if (stage_show_scene_b_frame == ev->frame) {
                if (ev->command_word == 0x0FFFFFFF) {
                    stage_show_scene_b_interpreter_state = 2;
                    i = 0xFFFF;
                } else {
                    slot = (ev->command_word & 0xFF0000) >> 16;
                    img = ev->command_word & 0xFF;
                    if (img == 0xFE) {
                        stage_show_drama_final_actor_slot_state_set(slot, 0x50, 0);
                    } else {
                        stage_show_drama_final_actor_slot_state_set(slot, img, 1);
                    }
                    if (ev->command_word & 0xF0000000) {
                        stage_show_drama_final_kanji_line_start(stage_show_scene_b_text_script_addresses[stage_show_scene_b_script_index]);
                        game_state.mode_stage_timer = 1;
                        stage_show_scene_b_text_active = 1;
                    }
                }
            } else {
                stage_show_scene_b_event_cursor = i;
                i = 0xFFFF;
            }
        }
    }
    stage_show_scene_b_frame++;
    if (stage_show_scene_b_text_active == 1) {
        stage_show_drama_final_event_text_and_audio_display();
    }
}

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AAD0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AAE8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AB00);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AB18);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AB30);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AB48);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AB5C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AB70);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AB88);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013ABA0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013ABBC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013ABCC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013ABDC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013ABF4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AC10);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AC2C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AC48);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AC60);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AC78);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AC94);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013ACB0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013ACBC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013ACD0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013ACD8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013ACF4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AD10);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AD18);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AD20);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AD30);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AD40);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AD48);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AD58);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AD68);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AD70);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AD74);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AD78);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata", D_8013AD88);

void stage_show_drama_final_event_text_and_audio_display(void) {
    s32 index;
    s32 *coordinates;

    if (game_state.mode_stage_timer == 1) {
        game_state.mode_stage_timer++;
        if (stage_show_final_text_line_index != 21 && stage_show_final_text_line_index != 22) {
            set_kanji_string(-86, 64, 0, (s32)"「", 0);
        }
        index = stage_show_final_text_line_index;
        if (stage_show_final_text_line_index < 100) {
            do {
                set_kanji_string(-72, (s16)((index - stage_show_final_text_line_index) * 16 + 64), 0, (s32)(((u8 **)stage_show_final_text_table)[index] + 1), 0);
                if (((u8 **)stage_show_final_text_table)[index][0] == 103) {
                    if (!(game_state.pad_held & PAD_CIRCLE)) {
                        coordinates = (s32 *)stage_show_final_voice_ranges;
                        coordinates += *(u8 *)&game_state.mode_stage_scratch;
                        cd_audio_request_set(coordinates[0], coordinates[1], game_state.current_girl);
                        audio_command(0x300);
                    }
                    (*(u8 *)&game_state.mode_stage_scratch) += 2;
                    stage_show_final_text_line_index = index + 1;
                    index = 100;
                }
                index++;
            } while (index < 100);
        }
    }
    if (cd_audio_state() == 2 && check_end_k() != 0) {
        game_state.mode_stage_timer = 0;
    }
    check_k_scroll();
    k_disp_inc();
}

void stage_show_drama_final_actor_slot_state_set(s32 actor_index, s32 animation_frame, s32 set_draw_flag) {
    u8 *p;

    if (set_draw_flag == 1) {
        p = actor_pool + actor_index * 0x44;
        p[0x1983] = 0x80;
    } else {
        p = actor_pool + actor_index * 0x44;
        p[0x1983] = 0;
    }
    *(s16 *)(p + 0x1998) = animation_frame;
}

void stage_show_drama_final_kanji_line_start(s32 text) {
    k_disp_start(set_kanji_string(-0x80, 0x30, 0, text, 0));
}

void stage_show_drama_final_timer_gate(u32 wait_frames) {
    if (wait_frames < (u32) game_state.mode_step_timer.w) {
        mode_step_next();
    }
}
