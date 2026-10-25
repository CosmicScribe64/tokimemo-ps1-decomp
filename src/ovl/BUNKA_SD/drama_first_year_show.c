/* drama_first_year_show.c: src/ovl/BUNKA_SD/80133540.c:4,29,45; show resource loading, skip/input and timed scripts; drama club year1995; club/year dispatch is proved by src/ovl/BUNKA_SD/club_year_show_dispatch.c:14-51,63-89. Original object 0x80133540-0x80134260 (config/objects/BUNKA_SD.txt). */
#include "common.h"
#include "ovl/BUNKA_SD.h"

s32 stage_show_scene_a_data_load(void) {
    if (game_state.substep == 0) {
        audio_bank_load_start(1, 0xBEFB, 0xBEE2, 0xCDC0, 0xCD82, 0xCD70);
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
        cd_read_sectors(0xB4, 0x80180000, 0x4D9A);
        game_state.substep = game_state.substep + 1;
    } else if (game_state.substep == 4) {
        if (cd_read_sync_status() & 1) {
            stage_show_scene_a_text_data = D_8013B810;
            stage_show_scene_a_voice_data = D_8013B938;
            game_state.current_girl = GIRL_SOTOI;
            mode_step_next();
        }
    }
}

void stage_show_scene_a_input_update(void) {
    if (game_state.mode_step >= 3 && game_state.mode_step < 0x28 && (game_state.pad_held & PAD_CIRCLE)) {
        if (cd_audio_state() != 2) {
            audio_command(0x74);
        }
        stage_show_scene_a_dispatch();
    }
    if (game_state.mode_step >= 3 && game_state.mode_step < 0x26 && (game_state.pad_held & PAD_CROSS)) {
        audio_command(0xC1);
        audio_command(0xC4);
        mode_step_set(0x25);
    }
    stage_show_scene_a_dispatch();
}

void stage_show_scene_a_dispatch(void) {
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    switch (game_state.mode_step) {
    case 0:
        stage_show_scene_state_reset();
        audio_command(0x7F);
        break;
    case 1:
        stage_show_scene_a_data_load();
        break;
    case 2:
        audio_command(0x200);
        stage_show_scene_a_state_init();
        break;
    case 3:
        stage_show_actor_brightness_ramp_up();
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        stage_show_event_list_run();
        break;
    case 12:
        stage_show_actor_brightness_ramp_down();
        break;
    case 13:
        audio_command(0x201);
        stage_show_scene_a_actor_set();
        break;
    case 14:
        stage_show_actor_brightness_ramp_up();
        break;
    case 15:
    case 16:
    case 17:
    case 18:
        stage_show_event_list_run();
        break;
    case 19:
        audio_command(0x202);
        mode_step_next();
        break;
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
        stage_show_event_list_run();
        break;
    case 37:
        audio_command(0xC1);
        audio_command(0x500);
        k_reset(1);
        mode_step_next();
        break;
    case 38:
        if ((u32) game_state.mode_step_timer.u >= 0x1FU) {
            mode_step_next();
        }
        break;
    case 39:
        stage_show_actor_brightness_ramp_down();
        break;
    case 40:
        stage_show_scene_finish_wait();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    game_state.mode_step_timer.u += 1;
}

void stage_show_scene_a_state_init(void) {
    stage_show_scene_a_actor_init();
    k_speed_set(TEXT_SPEED_EVERY_SIX_FRAMES);
    stage_show_scene_a_text_cursor = 0;
    stage_show_scene_a_event_cursor = 1;
    stage_show_scene_a_script_index = 0;
    stage_show_scene_a_frame = 0;
    stage_show_scene_a_interpreter_state = 0;
    stage_show_scene_a_text_active = 0;
    mode_step_next();
}

void stage_show_scene_a_actor_init(void) {
    s32 i;

    for (i = 0; i < 0x10; i++) {
        load_palette(D_8013C6F4, (i + 0x10) & 0xFF, 1, 1, i);
    }
    for (i = 0; i < 0x20; i++) {
        actor_clear(i + 0x60);
        ((Actor *)actor_pool)[96 + i].unk_01 = 4;
        ((Actor *)actor_pool)[96 + i].quad_flags = 0x01000000;
        ((Actor *)actor_pool)[96 + i].draw_flags = 0;
        ((Actor *)actor_pool)[96 + i].flags = 0x40;
        ((Actor *)actor_pool)[96 + i].frame_table = D_8013C6EC;
        ((Actor *)actor_pool)[96 + i].sequence_table = D_8013C6F0;
        ((Actor *)actor_pool)[96 + i].tpage_table = (u8 *)D_8013C6E8;
        ((Actor *)actor_pool)[96 + i].unk_18 = 0;
        ((Actor *)actor_pool)[96 + i].blink_period = 1;
        ((Actor *)actor_pool)[96 + i].brightness = 0;
        ((Actor *)actor_pool)[96 + i].x = -0xA0;
        ((Actor *)actor_pool)[96 + i].y = -0x78;
        ((Actor *)actor_pool)[96 + i].unk_05 = 0x80;
        ((Actor *)actor_pool)[96 + i].unk_43 = 0;
        ((Actor *)actor_pool)[96 + i].unk_04 = 0;
    }
    stage_show_drama_first_show_actor_slot_setup(0x1F, 0xBF, 1);
    stage_show_drama_first_show_actor_slot_setup(1, 0, 1);
    load_tpage_buf_lock(5, D_8013C6E8[0], 1);
    load_tpage_buf_lock(6, D_8013C6E8[1], 1);
    load_tpage_buf_lock(7, D_8013C6E8[2], 1);
    load_tpage_buf_lock(8, D_8013C6E8[3], 1);
    load_tpage_buf_lock(9, D_8013C6E8[4], 1);
    load_tpage_buf_lock(0xA, D_8013C6E8[5], 1);
    load_tpage_buf_lock(0xB, D_8013C6E8[6], 1);
    load_tpage_buf_lock(0xC, D_8013C6E8[7], 1);
    load_tpage_buf_lock(0xD, D_8013C6E8[8], 1);
    load_tpage_buf_lock(0xE, D_8013C6E8[9], 1);
    load_tpage_buf_lock(0xF, D_8013C6E8[10], 1);
    load_tpage_buf_lock(0x25, D_8013C6E8[11], 1);
    load_tpage_buf_lock(0x26, D_8013C6E8[12], 1);
    load_tpage_buf_lock(0x27, D_8013C6E8[13], 1);
    load_tpage_buf_lock(0x28, D_8013C6E8[14], 1);
    load_tpage_buf_lock(0x29, D_8013C6E8[15], 1);
    load_tpage_buf_lock(0x2A, D_8013C6E8[16], 1);
    load_tpage_buf_lock(0x2B, D_8013C6E8[17], 1);
    load_tpage_buf_lock(0x2C, D_8013C6E8[18], 1);
    load_tpage_buf_lock(0x2D, D_8013C6E8[19], 1);
    load_tpage_buf_lock(0x2E, D_8013C6E8[20], 1);
}

void stage_show_scene_a_actor_set(void) {
    stage_show_drama_first_show_actor_slot_setup(0x1F, 0xBF, 0);
    stage_show_drama_first_show_actor_slot_setup(0x1E, 0xC0, 1);
    stage_show_drama_first_show_actor_slot_setup(0x1D, 0xC1, 1);
    stage_show_drama_first_show_actor_slot_setup(8, 7, 1);
    stage_show_drama_first_show_actor_slot_setup(2, 0x4D, 1);
    stage_show_drama_first_show_actor_slot_setup(1, 0x2E, 1);
    stage_show_drama_first_show_actor_slot_setup(0, 0x50, 1);
    mode_step_next();
}

s32 stage_show_event_list_run(void) {
    BsdEvent *list;
    BsdEvent *ev;
    s32 i;
    s32 slot;
    s32 img;

    if (game_state.mode_stage_timer == 0) {
        stage_show_scene_a_text_active = 0;
        if (stage_show_scene_a_interpreter_state != 0) {
            switch (stage_show_scene_a_interpreter_state) {
            case 1:
                game_state.mode_step_timer.w = 0;
                break;
            case 2:
                stage_show_scene_a_script_index++;
                stage_show_scene_a_frame = 0;
                stage_show_scene_a_interpreter_state = 0;
                stage_show_scene_a_event_cursor = 1;
                k_reset(1);
                mode_step_next();
                return 1;
            }
        }
    }
    list = stage_show_scene_a_event_lists[stage_show_scene_a_script_index];
    if (stage_show_scene_a_interpreter_state == 0) {
        stage_show_scene_a_interpreter_state = 1;
    }
    if (stage_show_scene_a_interpreter_state == 1) {
        for (i = stage_show_scene_a_event_cursor; i < 0xFFFF; i++) {
            /* FAKE: T-22003, the word view with i * 2 takes the ugen temporary that the
             * original's index expression takes; &list[i] gives $t1 for $t2 in the loop */
            ev = (BsdEvent *)((s32 *)list + i * 2);
            if (stage_show_scene_a_frame == ev->frame) {
                if (ev->command_word == 0x0FFFFFFF) {
                    stage_show_scene_a_interpreter_state = 2;
                    i = 0xFFFF;
                } else {
                    slot = (ev->command_word & 0xFF0000) >> 16;
                    img = ev->command_word & 0xFF;
                    if (img == 0xFE) {
                        stage_show_drama_first_show_actor_slot_setup(slot, 0x50, 0);
                    } else {
                        stage_show_drama_first_show_actor_slot_setup(slot, img, 1);
                    }
                    if (ev->command_word & 0xF0000000) {
                        stage_show_drama_first_show_actor_slot_clear(stage_show_scene_a_text_script_addresses[stage_show_scene_a_script_index]);
                        game_state.mode_stage_timer = 1;
                        stage_show_scene_a_text_active = 1;
                    }
                }
            } else {
                stage_show_scene_a_event_cursor = i;
                i = 0xFFFF;
            }
        }
    }
    stage_show_scene_a_frame++;
    if (stage_show_scene_a_text_active == 1) {
        stage_show_drama_first_show_event_list_init();
    }
}

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A090);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A0AC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A0C4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A0E4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A0F8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A118);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A13C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A164);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A188);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A1B0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A1CC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A1E4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A200);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A210);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A22C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A244);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A254);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A268);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A284);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A294);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A2AC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A2C4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A2D0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A2F0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A304);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A31C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A338);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A350);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A364);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A37C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A394);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A3B0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A3C8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A3E8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A400);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A414);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A430);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A450);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A46C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A480);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A498);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A4B8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A4CC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A4E4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A504);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A51C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A534);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A554);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A56C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A590);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A5B0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A5CC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A5FC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A610);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A62C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A644);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A670);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A684);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A6A0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A6BC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A6D4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A6EC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A704);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A720);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A734);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A754);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A770);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A788);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A798);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A7B0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A7D0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A7E8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A808);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A83C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A858);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A860);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A868);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A870);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A878);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A880);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A888);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A898);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A8A0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A8A8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A8B0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A8B8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A8C0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A8CC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A8D4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A8E0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A8F0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A8FC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A908);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A914);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A91C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A924);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A92C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A934);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A940);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A948);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A950);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A95C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_first_year_show.rodata", D_8013A968);

void stage_show_drama_first_show_event_list_init(void) {
    s32 i;
    s32 off;
    u8 *p;

    if (game_state.mode_stage_timer == 1) {
        game_state.mode_stage_timer += 1;
        set_kanji_string(-0x56, 0x40, 0, "「", 0);
        for (i = stage_show_scene_a_text_cursor; i < 0x64; i++) {
            off = i * 4;
            set_kanji_string(-0x48, (s16)(((i - stage_show_scene_a_text_cursor) * 0x10) + 0x40), 0,
                             *(s32 *)(stage_show_scene_a_text_data + off) + 1, 0);
            if (**(u8 **)(stage_show_scene_a_text_data + off) == 0x67) {
                if (game_state.mode_step == 0x23) {
                    set_kanji_string(-0x80, 0x70, 0, "奉行", 0);
                    set_kanji_string(-0x56, 0x80, 0, "「おのれー・・・。", 0);
                }
                if (!(game_state.pad_held & PAD_CIRCLE)) {
                    /* FAKE: two statements load the table before the index; unk_110C is read
                     * as u8 here (lbu), as in stage_show_picture_index_set. T-11430 */
                    p = stage_show_scene_a_voice_data;
                    p += *(u8 *)&game_state.mode_stage_scratch * 4;
                    cd_audio_request_set(*(s32 *)p, *(s32 *)(p + 4), game_state.current_girl);
                    audio_command(0x300);
                }
                *(u8 *)&game_state.mode_stage_scratch += 2;
                stage_show_scene_a_text_cursor = i + 1;
                i = 0x64;
            }
        }
    }
    if (cd_audio_state() == 2 && check_end_k() != 0) {
        game_state.mode_stage_timer = 0;
    }
    check_k_scroll();
    k_disp_inc();
}

void stage_show_drama_first_show_actor_slot_setup(s32 arg0, s32 arg1, s32 arg2) {
    u8 *p;

    if (arg2 == 1) {
        p = actor_pool + arg0 * 0x44;
        p[0x1983] = 0x80;
    } else {
        p = actor_pool + arg0 * 0x44;
        p[0x1983] = 0;
    }
    *(s16 *)(p + 0x1998) = arg1;
}

void stage_show_drama_first_show_actor_slot_clear(s32 arg0) {
    k_disp_start(set_kanji_string(-0x80, 0x30, 0, arg0, 0));
}

void stage_show_drama_first_show_frame_display(u32 arg0) {
    if (arg0 < (u32)game_state.mode_step_timer.u) {
        mode_step_next();
    }
}
