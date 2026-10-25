/* candidate_girl_scene.c: src/ovl/TAIIKU/80138950.c:59,256-260; girl-candidate scene callbacks, selectable actors and transition logic with scene-layer resources; wiki/spec/o-taiiku.md provides the inspected selection rules. Original object 0x80138950-0x80140810 (config/objects/TAIIKU.txt). */
#define MAIN_API_OVERRIDE_cd_read_sectors /* arg1 is a RAM address: matched as u32 (main_api.h: s32), T-16552 */
#define MAIN_API_OVERRIDE_scene_screen_reset /* called as an implicit int function (definition: void), T-16552 */
#define MAIN_API_OVERRIDE_Rot_2D /* implicit declaration, as matched (main_api.h: void(s32,s32*,s32*,s32*,s32*)) */
#include "common.h"
#include "ovl/sports_festival.h"
#include "psx_types.h"
void cd_read_sectors(u8 arg0, u32 arg1, s32 arg2);
s32 scene_screen_reset();
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149940);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", sports_candidate_pointer_region);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", sports_candidate_input_latched);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", sports_candidate_sequence_step);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", sports_candidate_secondary_sequence_step);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149954);
s16 D_80149958 = 0;
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_8014995C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", sports_candidate_motion_speed);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149964);
SportsFixedPointBlock D_80149968 = { 0, 0, 0, 0, 0, 0, 0, 0, { 0, 0 } };
TaiikuSel sports_candidate_sequence_counters = { 0, 0, 0 };
Taiiku3s32 sports_candidate_input_and_boundary_state = { 0, 0, 0 };
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", sports_candidate_secondary_sequence_count);
TaiikuA0 sports_candidate_sequence_motion = { 0, 0, 0, 0, 0, 0 };
SportsPairValue sports_candidate_player_progress_record[1] = { { 0, 0 } };
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", sports_candidate_cpu_progress_records);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", sports_candidate_player_position);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", sports_candidate_activity_frames);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", sports_candidate_activity_flags);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", sports_candidate_result_order);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", sports_candidate_transition_wait_frames);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_801499DC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", sports_candidate_speed_update_frames);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_801499E4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149A84);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149B24);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149BC4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149C64);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149D04);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149DA4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149DAC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149DF4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149E08);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149E2C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149E34);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149E7C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149E90);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149EBC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149EE8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149F00);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149F20);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149F40);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", sports_candidate_sequence_frame_limits);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149F68);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", sports_candidate_progress_deltas);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/candidate_girl_scene.data", D_80149F8C);

void sports_candidate_scene_audio_request(void) {
    TaiikuTbl11 audio_table_a;
    TaiikuTbl11 audio_table_b;

    audio_table_a = D_801498E8;
    audio_table_b = D_80149914;
    cd_audio_request_set(audio_table_a.v[sports_runner_selected_girl], audio_table_b.v[sports_runner_selected_girl], sports_runner_selected_girl);
    audio_command(0x300);
}

typedef struct {
    void (*f[9])();
} FnTbl9; /* size 0x24 */
extern FnTbl9 D_80149E08;

void sports_candidate_mode_dispatch(void) {
    s32 mode_stage_index;
    s32 pad; /* FAKE: second word above candidate_scene_callbacks (T-3330 two-word case); real source unknown. T-4030 */
    FnTbl9 candidate_scene_callbacks;

    candidate_scene_callbacks = D_80149E08;
    mode_stage_index = game_state.mode_stage;
    candidate_scene_callbacks.f[mode_stage_index]();
}

/* scene_screen_reset is called as an int function here (see the override at the top): its unused $v0 result keeps
 * the loop pointer of case 0 out of $v0, and the constant 8 takes it, as in the original (with void the pointer,
 * priority 41, is coloured first and takes $v0). The call result goes through idx: A[f()] = v with the call in
 * the index reserves a second spill slot (see sports_girl_scene_choose). T-16552. */
void sports_scene_asset_load_setup(void) {
    s32 p; /* FAKE: T-16552, case 2 buffer address; its only job is the third scalar frame slot, real local unknown */
    s32 i;
    s32 idx;
    TaiikuW2 tpage;
    TaiikuW18 tbl;
    TaiikuW5 rgb;

    tpage = D_80149E2C;
    tbl = D_80149E34;
    rgb = D_80149E7C;
    switch (game_state.mode_step) {
    case 0:
        scene_screen_reset(1, 1);
        for (i = 0; i < 3; i++) {
            game_state.tpage_buf[i + 5] = 8;
        }
        for (i = 0; i < 3; i++) {
            game_state.tpage_buf[i + 0x25] = 8;
        }
        game_state.tpage_buf[28] = 8;
        actor_clear_range(0);
        hud_cursor_show(0);
        for (i = 0; i < 2; i++) {
            D_80149DA4[i] = tpage.v[i];
        }
        for (i = 0; i < 18; i++) {
            D_80149DAC[i] = tbl.v[i];
        }
        D_80149DF4[0] = rgb.v[0];
        for (i = 1; i < 5; i++) {
            D_80149DF4[i] = rgb.v[i];
        }
        mode_step_next();
        break;
    case 1:
        cd_read_sectors(0x1E, 0x80180000, 0xBAD0);
        Fade_Out_Init();
        mode_step_next();
        break;
    case 2:
        Fade_Out_Main();
        if (cd_read_sync_status() & 1) {
            p = 0x8018F000;
            cd_read_sectors(0x27, p, 0xBAEE);
            mode_step_next();
        }
        break;
    case 3:
        if (Fade_Out_Main() != 0 && (cd_read_sync_status() & 1)) {
            sports_scene_actor_resource_addresses_init();
            sports_candidate_scene_picture_resource_addresses_init();
            for (i = 0; i < 2; i++) {
                idx = search_load_tpage_buf_lock(D_80149DA4[i], 1);
                game_state.tpage_buf[idx] = i + 0x80000300;
                D_80149DA4[i] = i + 0x80000300;
            }
            for (i = 0; i < 16; i++) {
                load_palette(D_8014A0B4, (u8) (i + 0x10), 1, 1, i);
            }
            cd_read_sectors(0xB9, 0x8018F000, 0xBA17);
            sports_candidate_scene_state_init();
            sports_candidate_scene_render_resource_setup();
            sports_candidate_actor_display_init();
            mode_stage_next();
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

/* The A[f()] = v stores go through idx: with the call inside the index IDO reserves a second, unused spill slot
 * and the tables sit 4 bytes high (T-16552). cd_read_sectors takes its address as u32 here (see the override at
 * the top): with s32 IDO builds 0x801F8000 of case 0 through a hoisted 0x801F0000 in $s0. p is the buffer address
 * of case 1. */
void sports_girl_scene_choose(void) {
    s32 p;
    s32 i;
    s32 idx;
    RECT rect;
    TaiWords11 tblA;
    TaiWords11 tblB;

    tblA = D_80149E90;
    tblB = D_80149EBC;
    switch (game_state.mode_step) {
    case 0:
        /* FAKE: T-16552. i = 0 between the call and the test puts `move s0,zero` in the beqz delay slot; the goto
         * enters the loop without an entry test (with one, uopt leaves the pointer start as `sll t9,zero,2`) and
         * keeps the increments on the for line, where as1 schedules them before the stores. Real source unknown. */
        idx = cd_read_sync_status();
        i = 0;
        if (idx & 1) {
            goto first;
            for (; i < 18; i++) {
            first:
                idx = search_load_tpage_buf_lock(D_80149DAC[i], 1);
                game_state.tpage_buf[idx] = i + 0x80000100;
                D_80149DAC[i] = i + 0x80000100;
            }
            rect.x = 0x300;
            rect.y = 0x80;
            rect.w = 0x40;
            rect.h = 0x80;
            func_8009C884(&rect, (void *)0x801BB000);
            game_state.tpage_buf[44] = 8;
            for (i = 0; i < 5; i++) {
                idx = search_load_tpage_buf_lock(D_80149DF4[i], 1);
                game_state.tpage_buf[idx] = i + 0x80000200;
                D_80149DF4[i] = i + 0x80000200;
            }
            cd_read_sectors(3, 0x801F8000, 0xBE2A);
            Fade_In_Init();
            audio_command(0x200);
            mode_step_next();
        }
        break;
    case 1:
        if (Fade_In_Main() != 0) {
            if (cd_read_sync_status() & 1) {
                draw2d3d(1, 1);
                sports_candidate_model_lighting_init();
                sports_candidate_scene_lighting_init();
                sports_candidate_model_slots_init();
                p = 0x801F8000;
                sports_candidate_model_draw_packets_init(p);
                sports_candidate_model_draw_packet_bind(p, 0);
                mode_step_next();
            }
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        break;
    case 2:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        sports_girl_scene_flagged_actors_scroll();
        Yubi(0x6B);
        if (game_state.pad_pressed & 0xF0) {
            for (i = 0; i < 11; i++) {
                if (((Actor *)actor_pool)[96 + i].x <= ((Actor *)actor_pool)[107].x + 8 &&
                    ((Actor *)actor_pool)[107].x + 8 <= ((Actor *)actor_pool)[96 + i].x + 0x20 &&
                    ((Actor *)actor_pool)[96 + i].y <= ((Actor *)actor_pool)[107].y + 0x10 &&
                    ((Actor *)actor_pool)[107].y + 0x10 <= ((Actor *)actor_pool)[96 + i].y + 0x40 &&
                    ((Actor *)actor_pool)[96 + i].draw_flags != 0) {
                    D_80149940 = i;
                    Fade_Out_Init();
                    audio_command(0x74);
                    mode_step_next();
                }
            }
        }
        break;
    case 3:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if (cd_audio_state() == 2) {
            cd_read_sectors(0x87, 0x8018F000, 0xB98F);
            mode_step_next();
        }
        break;
    case 4:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if (Fade_Out_Main() != 0) {
            mode_step_next();
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            sports_candidate_scene_layer_resource_addresses_init();
            layer_tileset_init(0, (u32 *)D_801491C4, D_801491C0);
            srn_init(0, D_801491C8, 0);
            srn_vram_set(0, 5, 0x140, 0xF0);
            sports_runner_scene_background_tile_upload();
            for (i = 0; i < 6; i++) {
                search_load_tpage_buf_lock(D_8014A070[i], 1);
            }
            sports_girl_scene_actors_init();
            audio_bank_wait_done = 0;
            audio_bank_load_start(0, 0xBF98, 0xBF79, tblA.value[D_80149940], tblB.value[D_80149940], 0xD6B7);
            Fade_In_Init();
            mode_step_next();
        }
        break;
    case 6:
        audio_bank_wait_latch();
        if (Fade_In_Main() != 0) {
            Vblnk_Timer_Init();
            mode_stage_next();
        }
        sports_candidate_camera_transition_update();
        sports_candidate_motion_speed_and_scroll_update();
        sports_candidate_model_rotation_update();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void sports_candidate_scene_frame_update(void) {
    switch (game_state.mode_step) {
    case 0:
        audio_bank_wait_latch();
        if (audio_bank_wait_done != 0) {
            audio_command(0x2E);
            sports_candidate_input_and_boundary_state.boundary_side = 0;
            mode_step_next();
        }
        sports_candidate_camera_transition_update();
        sports_candidate_motion_speed_and_scroll_update();
        sports_candidate_model_rotation_update();
        return;
    case 1:
        audio_command(0x24);
        sports_candidate_camera_transition_update();
        sports_candidate_motion_speed_and_scroll_update();
        sports_candidate_model_rotation_update();
        mode_step_next();
        return;
    case 2:
        audio_command(0x23);
        sports_candidate_camera_transition_update();
        sports_candidate_motion_speed_and_scroll_update();
        sports_candidate_model_rotation_update();
        mode_step_next();
        return;
    case 3:
        audio_command(0x502);
        sports_candidate_camera_transition_update();
        sports_candidate_motion_speed_and_scroll_update();
        sports_candidate_model_rotation_update();
        Vblnk_Timer_Init();
        mode_step_next();
        return;
    case 4:
        sports_candidate_scene_frame_counters_advance();
        sports_candidate_motion_update();
        sports_candidate_scene_actor_state_update();
        sports_candidate_sequence_effect_dispatch();
        if (D_8012067A >= D_801206BE) {
            D_80120651 = 2;
            D_80120695 = 3;
        } else {
            D_80120651 = 3;
            D_80120695 = 2;
        }
        ((Actor *)actor_pool)[98].x = ((Actor *)actor_pool)[96].x;
        ((Actor *)actor_pool)[99].x = ((Actor *)actor_pool)[97].x;
        D_80120702 = D_8012067A;
        D_80120746 = D_801206BE;
        D_80120852 = (s16) ((s16) D_80149968.screen_coordinates[0] >> 3);
        D_80120856 = ((s16) D_80149968.screen_coordinates[1] >> 3) + 0x20;
        sports_candidate_camera_transition_update();
        sports_candidate_motion_speed_and_scroll_update();
        sports_candidate_rival_display_update();
        sports_candidate_scene_progress_update();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void sports_girl_scene_result_slots_init(void) {
    s32 result_slot_index;

    audio_command(0x504);
    result_slot_index = 0;
    while (sports_candidate_result_order[result_slot_index] != 0xFF) {
        result_slot_index++;
    }
    sports_candidate_result_order[result_slot_index] = 0;
    for (result_slot_index++; result_slot_index < 4; result_slot_index++) {
        sports_candidate_result_order[result_slot_index] = sports_girl_scene_result_candidate_select();
    }
    sports_candidate_camera_transition_update();
    sports_candidate_motion_speed_and_scroll_update();
    D_80149954 = 0x1200;
    D_8012082F = 0;
    mode_stage_next();
}

void sports_girl_scene_result_update(void) {
    s32 pad; /* FAKE: T-11640, unused local; its slot puts the spills of the case 3 loop where the original has them */
    s32 i;

    switch (game_state.mode_step) {
    case 0:
        if (D_80149954 < 0x80) {
            actor_clear_upper();
            D_80149954 = 0;
            Fade_Out_Init();
            game_state.screen_brightness = 0;
            mode_step_next();
        }
        sports_candidate_model_transform_update();
        sports_candidate_camera_transition_update();
        sports_candidate_motion_speed_and_scroll_update();
        break;
    case 1:
        sports_girl_scene_result_actor_setup();
        Fade_In_Init();
        sports_runner_scene_actor_init();
        mode_step_next();
        break;
    case 2:
        if (Fade_In_Main() != 0) {
            if (sports_candidate_result_order[0] == 0 || sports_candidate_result_order[1] == 0) {
                audio_command(0x205);
            } else {
                audio_command(0x206);
            }
            Vblnk_Timer_Init();
            mode_step_next();
        }
        Scroll(0, 0xF, 0, 0xF0, 0xC0);
        break;
    case 3:
        if (Vblnk_Timer() >= 0x5B) {
            for (i = 0; i < 4; i++) {
                if (sports_candidate_result_order[i] == 0) {
                    sports_scene_audio_sequence_request(D_80149940, (u8) i);
                }
            }
            sports_girl_result_actor_motion_update();
            sports_girl_result_actor_sequence_advance();
            sports_candidate_transition_wait_frames = 0;
            Vblnk_Timer_Init();
            mode_step_next();
        }
        Scroll(0, 0xF, 0, 0xF0, 0xC0);
        break;
    case 4:
        sports_candidate_transition_wait_frames++;
        sports_girl_result_animation_toggle();
        if (Vblnk_Timer() >= 0xB5) {
            Vblnk_Timer_Init();
            mode_step_next();
        }
        Scroll(0, 0xF, 0, 0xF0, 0xC0);
        break;
    case 5:
        if (Vblnk_Timer() >= 0xF1) {
            Fade_Out_Init();
            mode_step_next();
        }
        Scroll(0, 0xF, 0, 0xF0, 0xC0);
        break;
    case 6:
        if (Fade_Out_Main() != 0) {
            if (sports_candidate_result_order[0] == 0) {
                for (i = 0; i < 13; i++) {
                    game_state.girls[i].tokimeki += 4;
                }
                game_state.girls[D_80149940].tokimeki += 2;
                game_state.girls[D_80149940].affection += 10;
            } else if (sports_candidate_result_order[1] == 0) {
                for (i = 0; i < 13; i++) {
                    game_state.girls[i].tokimeki += 2;
                }
                game_state.girls[D_80149940].tokimeki += 1;
                game_state.girls[D_80149940].affection += 5;
            }
            mode_stage_next();
        }
        Scroll(0, 0xF, 0, 0xF0, 0xC0);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void sports_candidate_scene_gate(void) {
    if ((girl_state_rank(6) < 2) && (((u32) (game_state.girls[6].flags.w << 0x1E) >> 0x1F) == 1) && (D_80149940 == 6) && (game_state.scene_flags[45] == 0)) {
        game_state.scene_flags[45] = 1;
        mode_stage_next();
        return;
    }
    game_state.mode_stage = 0;
    game_state.mode_step = 0;
    sports_scene_variant = 1;
}

/* FAKE: T-25102, the halves of the 16.16 word unk_18 are written through s16 views and D_8014995A/D_8014995C
 * through (&D_80149958)[n], as elsewhere in this file; the real aggregates are unknown. */
void sports_candidate_scene_state_init(void) {
    s32 candidate_state_index;
    s32 candidate_coordinate_index;
    s32 pad; /* FAKE: T-25102, unused local; its slot puts the spill of candidate_state_index at sp+0x50 as in the original */

    D_80149964 = 0;
    sports_candidate_motion_speed = 0;
    D_80149968.screen_coordinates[0] = 0;
    D_80149968.screen_coordinates[1] = 0x100;
    D_80149968.position_x = 0;
    ((s16 *)&D_80149968.position_y)[1] = 0x80;
    ((s16 *)&D_80149968.position_y)[0] = 0;
    D_80149968.acceleration_x = 0;
    D_80149968.acceleration_y = 0;
    D_80149968.velocity_x = 0;
    D_80149968.velocity_y = 0;
    D_80149940 = 0;
    sports_candidate_input_latched = 0;
    sports_candidate_sequence_step = 0;
    sports_candidate_player_position.word = 0;
    D_80122EC4 = 0;
    sports_candidate_pointer_region = 1;
    sports_candidate_speed_update_frames = 0;
    D_801499DC = 0;
    sports_candidate_sequence_counters.sequence_count = 0;
    sports_candidate_sequence_counters.primary_frame_counter = 0;
    sports_candidate_input_and_boundary_state.input_acceleration_x = 0;
    sports_candidate_input_and_boundary_state.input_acceleration_y = 0;
    sports_candidate_input_and_boundary_state.boundary_side = 1;
    sports_candidate_sequence_counters.secondary_frame_counter = 0;
    sports_candidate_secondary_sequence_count = 0;
    sports_candidate_sequence_motion.action_frames = 0;
    sports_candidate_sequence_motion.acceleration_x = 0;
    sports_candidate_sequence_motion.acceleration_y = 0;
    sports_candidate_sequence_motion.sequence_frames = 0;
    sports_candidate_sequence_motion.target_x = -0x20;
    sports_candidate_sequence_motion.target_y = 0x30;
    sports_candidate_player_progress_record[0].unk0 = 0;
    sports_candidate_player_progress_record[0].val = 0;
    srand(game_state.timer_ticks);
    D_80149958 = 0;
    (&D_80149958)[1] = 0;
    (&D_80149958)[2] = 0;
    ((TaiikuCamera0C *)overlay_scratch)->x = 0x118;
    ((TaiikuCamera0C *)overlay_scratch)->y = -0x100;
    ((TaiikuCamera0C *)overlay_scratch)->angle = 0;
    for (candidate_state_index = 0; candidate_state_index < 3; candidate_state_index++) {
        sports_candidate_activity_flags[candidate_state_index] = 0;
        sports_candidate_activity_frames[candidate_state_index] = 0;
        sports_candidate_cpu_progress_records[candidate_state_index].word = 0;
    }
    for (candidate_state_index = 0; candidate_state_index < 4; candidate_state_index++) {
        sports_candidate_result_order[candidate_state_index] = 0xFF;
    }
    for (candidate_state_index = 0; candidate_state_index < 80; candidate_state_index++) {
        D_801499E4[candidate_state_index] = (candidate_state_index >> 2) * 8 - 24;
        D_80149A84[candidate_state_index] = (candidate_state_index >> 2) * 8 - 16;
    }
    for (candidate_state_index = 0; candidate_state_index < 20; candidate_state_index++) {
        for (candidate_coordinate_index = 0; candidate_coordinate_index < 4; candidate_coordinate_index++) {
            D_80149B24[(candidate_state_index << 2) + candidate_coordinate_index] = -160 - (candidate_state_index << 3) + ((candidate_state_index << 2) + 80) * candidate_coordinate_index;
            D_80149BC4[(candidate_state_index << 2) + candidate_coordinate_index] = -80 - (candidate_state_index << 2) + ((candidate_state_index << 2) + 80) * candidate_coordinate_index;
            D_80149C64[(candidate_state_index << 2) + candidate_coordinate_index] = -168 - (candidate_state_index << 3) + (((candidate_state_index + 1) << 2) + 80) * candidate_coordinate_index;
            D_80149D04[(candidate_state_index << 2) + candidate_coordinate_index] = -84 - (candidate_state_index << 2) + (((candidate_state_index + 1) << 2) + 80) * candidate_coordinate_index;
        }
    }
}

void sports_candidate_scene_render_resource_setup(void) {
    layer_tileset_init(0, D_8014A0BC, D_8014A0B0);
    srn_init(0, D_8014A0B8, 0);
    srn_vram_set(0, 5, 0x580, 0xF0);
}

typedef struct {
    u32 low : 1;
    u32 f : 1;
    u32 rest : 30;
} TaiikuBit1;

void sports_candidate_actor_display_init(void) {
    s32 candidate_actor_index;
    s32 j;
    Actor *p;
    TaiikuS11 tbl;

    tbl = D_80149EE8;
    j = 0;
    p = (Actor *)actor_pool;
    for (candidate_actor_index = 0; candidate_actor_index < 12; candidate_actor_index++) {
        actor_clear(candidate_actor_index + 0x60);
        p[96].quad_flags = 0x01000000;
        p[96].brightness = 0x80;
        p[96].frame_table = (u8 *)D_80149FE0;
        p[96].sequence_table = (u8 *)D_8014A018;
        p[96].tpage_table = (u8 *)D_80149DA4;
        p[96].unk_14 = D_8014A050;
        p[96].unk_05 = 0x80;
        p[96].scale_x = 0x1000;
        p[96].scale_y = 0x1000;
        p[96].blink_period = 2;
        p[96].flags = 1;
        if (candidate_actor_index == 11) {
            p[96].x = 0;
            p[96].y = 0;
            p[96].unk_01 = 2;
            p[96].draw_flags = 0xA4;
        } else {
            if (((TaiikuBit1 *)&game_state.girls[candidate_actor_index].flags)->f == 1) {
                p[96].x = tbl.v[j];
                p[96].draw_flags = 0xA4;
                j++;
            } else {
                p[96].x = 0xC8;
                p[96].draw_flags = 0;
            }
            p[96].y = 0x28;
            p[96].unk_01 = 3;
        }
        p[96].sequence_slot = candidate_actor_index;
        p++;
    }
    ((Actor *)actor_pool)[0].x = 0;
    ((Actor *)actor_pool)[0].y = 0;
}

/* FAKE: T-16552. This leaf function is coloured in bit order and the bits follow the first mention in the
 * ucode: the loop pointer (game_state + i * 0x38) is numbered in the first loop and so takes a lower register than
 * the record address and the unk_F80 copy, while the original has cnt $a0, record $a1, flags $a2, pointer $a3.
 * The two dead assignments at the top of the first loop (m = a field of record 96 + i, cnt = unk_F80) are
 * overwritten before any use and generate no code; they number the record address and unk_F80 first. Mentions
 * in code that cfe drops (if (0), unused locals, an address without a load, two adjacent assignments to one
 * local) do not renumber. m also reproduces the fourth scalar slot (sp1C at sp+0x1C), real local unknown. */
void sports_girl_scene_flagged_actors_scroll(void) {
    s32 n;
    s32 i;
    s32 cnt;
    s32 m;
    s32 sp1C[11];

    n = 0;
    for (i = 0, cnt = 0; i < 0xB; i++) {
        m = ((Actor *)actor_pool)[96 + i].x;
        cnt = game_state.pad_held;
        if (((TaiikuBit1 *)&game_state.girls[i].flags)->f == 1) {
            sp1C[n] = i;
            n++;
        }
    }
    cnt = 0;
    m = n;
    for (i = 0; i < 0xB; i++) {
        if (((TaiikuBit1 *)&game_state.girls[i].flags)->f == 1) {
            if (game_state.pad_held & PAD_L1) {
                ((Actor *)actor_pool)[96 + i].x = ((Actor *)actor_pool)[96 + i].x - 4;
            } else {
                ((Actor *)actor_pool)[96 + i].x = ((Actor *)actor_pool)[96 + i].x - 1;
            }
            if (((Actor *)actor_pool)[96 + i].x < -0xC0) {
                ((Actor *)actor_pool)[96 + i].x = ((Actor *)actor_pool)[96 + sp1C[(cnt + n - 1) % m]].x + 0x78;
                if (((Actor *)actor_pool)[96 + i].x < 0xA0) {
                    ((Actor *)actor_pool)[96 + i].x = 0xA0;
                }
            }
            cnt++;
        }
    }
}

void sports_girl_scene_actors_init(void) {
    s32 i;
    TaiikuH16 x;
    TaiikuH16 y;
    TaiikuB16 frame;

    x = D_80149F00;
    y = D_80149F20;
    frame = D_80149F40;
    actor_clear_upper();
    for (i = 0; i < 16; i++) {
        ((TaiikuActor44 *)actor_pool)[96 + i].unk_38 = 0x1000000;
        ((TaiikuActor44 *)actor_pool)[96 + i].unk_07 = 0x80;
        ((TaiikuActor44 *)actor_pool)[96 + i].unk_05 = 0x80;
        ((TaiikuActor44 *)actor_pool)[96 + i].unk_0C = (u8 *)D_80149FDC;
        ((TaiikuActor44 *)actor_pool)[96 + i].unk_10 = (u8 *)D_8014A014;
        ((TaiikuActor44 *)actor_pool)[96 + i].unk_34 = (u8 *)D_80149DAC;
        ((TaiikuActor44 *)actor_pool)[96 + i].unk_14 = D_8014A04C;
        ((TaiikuActor44 *)actor_pool)[96 + i].unk_1A = 0x1000;
        ((TaiikuActor44 *)actor_pool)[96 + i].unk_1C = 0x1000;
        ((TaiikuActor44 *)actor_pool)[96 + i].unk_02 = 0x40;
        ((TaiikuActor44 *)actor_pool)[96 + i].unk_03 = 0x84;
        ((TaiikuActor44 *)actor_pool)[96 + i].unk_01 = frame.v[i] + 0xF0;
        ((TaiikuActor44 *)actor_pool)[96 + i].unk_26 = x.v[i];
        ((TaiikuActor44 *)actor_pool)[96 + i].unk_2A = y.v[i];
    }
    ((TaiikuActor44 *)actor_pool)[96].unk_18 = 0;
    ((TaiikuActor44 *)actor_pool)[96].unk_01 = 3;
    ((TaiikuActor44 *)actor_pool)[97].unk_18 = D_80149940 * 6 + 6;
    ((TaiikuActor44 *)actor_pool)[97].unk_01 = 3;
    ((TaiikuActor44 *)actor_pool)[98].unk_18 = 0x82;
    ((TaiikuActor44 *)actor_pool)[98].unk_07 = 0x68;
    ((TaiikuActor44 *)actor_pool)[98].unk_38 |= 0x40000000;
    ((TaiikuActor44 *)actor_pool)[99].unk_18 = 0x83;
    ((TaiikuActor44 *)actor_pool)[99].unk_07 = 0x68;
    ((TaiikuActor44 *)actor_pool)[99].unk_38 |= 0x40000000;
    ((TaiikuActor44 *)actor_pool)[100].unk_18 = 0x94;
    ((TaiikuActor44 *)actor_pool)[101].unk_01 = 1;
    ((TaiikuActor44 *)actor_pool)[101].unk_03 = 4;
    ((TaiikuActor44 *)actor_pool)[101].unk_18 = 0x84;
    ((TaiikuActor44 *)actor_pool)[102].unk_18 = 0x85;
    ((TaiikuActor44 *)actor_pool)[102].unk_03 = 4;
    ((TaiikuActor44 *)actor_pool)[103].unk_18 = 0x81;
    ((TaiikuActor44 *)actor_pool)[103].unk_07 = 0x68;
    ((TaiikuActor44 *)actor_pool)[103].unk_38 |= 0x40000000;
    ((TaiikuActor44 *)actor_pool)[104].unk_18 = 0x8C;
    ((TaiikuActor44 *)actor_pool)[104].unk_03 |= 1;
    for (i = 0; i < 3; i++) {
        ((TaiikuActor44 *)actor_pool)[105 + i].unk_18 = 0x86;
        ((TaiikuActor44 *)actor_pool)[105 + i].unk_03 |= 1;
        ((TaiikuActor44 *)actor_pool)[105 + i].unk_43 = i + 0x1C;
    }
    ((TaiikuActor44 *)actor_pool)[108].unk_18 = 0x92;
    for (i = 0; i < 3; i++) {
        ((TaiikuActor44 *)actor_pool)[109 + i].unk_18 = 0x93;
        ((TaiikuActor44 *)actor_pool)[109 + i].unk_43 = i + 0x1C;
    }
}

void sports_candidate_scene_frame_counters_advance(void) {
    if (Vblnk_Timer() >= 2) {
        sports_candidate_sequence_motion.sequence_frames += 1;
        Vblnk_Timer_Init();
    }
    D_801499DC += 1;
    D_80149968.input_decay_frames += 1;
    D_80149968.velocity_update_frames += 1;
    sports_candidate_speed_update_frames += 1;
    sports_candidate_sequence_counters.primary_frame_counter += 1;
    sports_candidate_sequence_counters.secondary_frame_counter += 1;
    sports_candidate_sequence_motion.action_frames += 1;
}

void sports_candidate_scene_actor_state_update(void) {
    s16 actor_101_next_x;
    s32 limit_y1;
    s32 limit_y2;
    s32 limit_y3;
    s32 limit_y4;

    switch (sports_candidate_pointer_region) {
    case 0:
        Yubi(0x60);
        break;
    case 1:
        sports_dance_actor_move_clamp(0x60);
        sports_candidate_pointer_position_clamp();
        limit_y1 = ((s16) D_80149968.screen_coordinates[1] >> 3) + 0x10;
        if (((Actor *)actor_pool)[96].y < limit_y1) {
            ((Actor *)actor_pool)[96].y = limit_y1;
        }
        break;
    case 2:
        sports_dance_actor_move_clamp(0x60);
        sports_candidate_pointer_position_clamp();
        if ((((s16) D_80149968.screen_coordinates[0] >> 3) + 0x50) >= ((Actor *)actor_pool)[96].x) {
            ((Actor *)actor_pool)[96].x = ((s16) D_80149968.screen_coordinates[0] >> 3) + 0x51;
        }
        break;
    case 3:
        sports_dance_actor_move_clamp(0x60);
        sports_candidate_pointer_position_clamp();
        if (((Actor *)actor_pool)[96].x >= (((s16) D_80149968.screen_coordinates[0] >> 3) - 0x50)) {
            ((Actor *)actor_pool)[96].x = ((s16) D_80149968.screen_coordinates[0] >> 3) - 0x51;
        }
        break;
    case 5:
        sports_dance_actor_move_clamp(0x60);
        sports_candidate_pointer_position_clamp();
        if (((Actor *)actor_pool)[96].x >= (((s16) D_80149968.screen_coordinates[0] >> 3) - 0x50)) {
            limit_y2 = ((s16) D_80149968.screen_coordinates[1] >> 3) + 0x10;
            if (((Actor *)actor_pool)[96].y < limit_y2) {
                ((Actor *)actor_pool)[96].x = ((s16) D_80149968.screen_coordinates[0] >> 3) - 0x51;
                ((Actor *)actor_pool)[96].y = limit_y2;
            }
        }
        break;
    case 6:
        sports_dance_actor_move_clamp(0x60);
        sports_candidate_pointer_position_clamp();
        if ((((s16) D_80149968.screen_coordinates[0] >> 3) + 0x50) >= ((Actor *)actor_pool)[96].x) {
            limit_y3 = ((s16) D_80149968.screen_coordinates[1] >> 3) + 0x10;
            if (((Actor *)actor_pool)[96].y < limit_y3) {
                ((Actor *)actor_pool)[96].x = ((s16) D_80149968.screen_coordinates[0] >> 3) + 0x51;
                ((Actor *)actor_pool)[96].y = limit_y3;
            }
        }
        break;
    case 7:
        switch (sports_candidate_sequence_step) {
        case 0:
            limit_y4 = ((s16) D_80149968.screen_coordinates[1] >> 3) + 0x10;
            if (((Actor *)actor_pool)[96].y < limit_y4) {
                if ((((Actor *)actor_pool)[96].x < (((s16) D_80149968.screen_coordinates[0] >> 3) + 0x50)) && ((((s16) D_80149968.screen_coordinates[0] >> 3) - 0x50) < ((Actor *)actor_pool)[96].x)) {
                    ((Actor *)actor_pool)[96].y = limit_y4;
                }
            }
            actor_101_next_x = ((Actor *)actor_pool)[101].x - 8;
            ((Actor *)actor_pool)[101].x = actor_101_next_x;
            if ((actor_101_next_x < -0x1C0) && (sports_candidate_motion_speed == 0)) {
                sports_candidate_sequence_counters.sequence_count = 0;
                ((Actor *)actor_pool)[96].draw_flags = 0xA4;
                sports_candidate_sequence_step += 1;
            }
            break;
        case 1:
            sports_candidate_sequence_step += 1;
            break;
        case 2:
            sports_candidate_sequence_step += 1;
            break;
        case 3:
            if (sports_candidate_input_and_boundary_state.boundary_side == 1) {
                ((Actor *)actor_pool)[96].x = 0x90;
            } else {
                ((Actor *)actor_pool)[96].x = -0x90;
                ((Actor *)actor_pool)[96].draw_flags |= 1;
            }
            ((Actor *)actor_pool)[96].y = 0x10;
            sports_candidate_sequence_step += 1;
            break;
        case 4:
            if (sports_candidate_secondary_sequence_step == 4) {
                if (sports_candidate_input_and_boundary_state.boundary_side == 1) {
                    ((Actor *)actor_pool)[96].x = ((s16) D_80149968.screen_coordinates[0] >> 3) + 0x60;
                    ((Actor *)actor_pool)[96].y = ((s16) D_80149968.screen_coordinates[1] >> 3) - 0x10;
                    if (D_80149968.position_x <= 0) {
                        sports_candidate_sequence_counters.sequence_count = 0;
                        D_80149968.velocity_x = 0;
                        sports_candidate_sequence_step += 1;
                    }
                } else {
                    ((Actor *)actor_pool)[96].x = ((s16) D_80149968.screen_coordinates[0] >> 3) - 0x60;
                    ((Actor *)actor_pool)[96].y = ((s16) D_80149968.screen_coordinates[1] >> 3) - 0x10;
                    if (D_80149968.position_x >= 0) {
                        sports_candidate_sequence_counters.sequence_count = 0;
                        ((Actor *)actor_pool)[96].draw_flags = 0xA4;
                        D_80149968.velocity_x = 0;
                        ((Actor *)actor_pool)[96].x = -0x50;
                        sports_candidate_sequence_step += 1;
                    }
                }
            }
            break;
        case 5:
            if (((Actor *)actor_pool)[96].y < 0x30) {
                ((Actor *)actor_pool)[96].y += 2;
            } else {
                ((Actor *)actor_pool)[96].y = 0x30;
                if (sports_candidate_input_and_boundary_state.boundary_side == 2) {
                    ((Actor *)actor_pool)[96].draw_flags |= 1;
                }
                sports_candidate_sequence_counters.sequence_count = 0;
                sports_candidate_sequence_step += 1;
            }
            break;
        case 6:
            if (sports_candidate_input_and_boundary_state.boundary_side == 1) {
                if (((Actor *)actor_pool)[96].x >= 0x21) {
                    ((Actor *)actor_pool)[96].x -= 2;
                } else {
                    ((Actor *)actor_pool)[96].x = 0x20;
                    sports_candidate_sequence_step += 1;
                }
            } else if (((Actor *)actor_pool)[96].x < 0x20) {
                ((Actor *)actor_pool)[96].x += 2;
            } else {
                ((Actor *)actor_pool)[96].x = 0x20;
                sports_candidate_sequence_step += 1;
            }
            break;
        case 7:
            if (sports_candidate_secondary_sequence_step >= 7) {
                sports_candidate_motion_state_init();
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
    sports_candidate_pointer_region = sports_candidate_pointer_region_select();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        sports_candidate_input_latched = 1;
    }
    if ((sports_candidate_input_latched != 0) || (game_state.pad_held & 0xF000) || (sports_candidate_input_and_boundary_state.boundary_side != 0)) {
        sports_candidate_animation_selector_update();
    }
}

void sports_candidate_motion_state_init(void) {
    sports_candidate_input_and_boundary_state.boundary_side = 0;
    sports_candidate_sequence_counters.sequence_count = 0;
    sports_candidate_input_and_boundary_state.input_acceleration_x = 0;
    sports_candidate_input_and_boundary_state.input_acceleration_y = 0;
    sports_candidate_secondary_sequence_count = 0;
    sports_candidate_sequence_motion.sequence_frames = 0;
    sports_candidate_sequence_motion.acceleration_x = 0;
    sports_candidate_sequence_motion.acceleration_y = 0;
    sports_candidate_player_progress_record[0].val = 0;
    D_80149968.position_x = 0;
    ((s16 *)&D_80149968.position_y)[0] = 0;
    ((s16 *)&D_80149968.position_y)[1] = 0x80;
    D_80149968.acceleration_x = 0;
    D_80149968.acceleration_y = 0;
    D_80149968.velocity_x = 0;
    D_80149968.velocity_y = 0;
    D_8014995A = 0;
    sports_candidate_pointer_region = 1;
    D_80120674 = 0;
    D_80120676 = 0x20;
    D_80120678 = 0;
    D_8012067A = 0x30;
    D_801206B8 = 0;
    if (((D_80122EC4 % 3) == 0) || (D_80149940 == 2)) {
        D_801206BA = -0x20;
    } else {
        D_801206BA = 0;
    }
    D_801206BC = 0;
    D_801206BE = 0x30;
    D_80120668 = 0;
    D_801206AC = (D_80149940 * 6) + 6;
    D_80120653 = 0xA4;
    D_80120697 = 0xA4;
    D_801207CA = 0xA0;
}

void sports_candidate_animation_selector_update(void) {
    if (sports_candidate_sequence_counters.primary_frame_counter >= 0xB) {
        if (sports_candidate_sequence_counters.sequence_count >= 3 && sports_candidate_input_and_boundary_state.boundary_side != 0 && (sports_candidate_sequence_step == 2 || sports_candidate_sequence_step == 4 || sports_candidate_sequence_step == 6)) {
            sports_candidate_sequence_counters.sequence_count = 0;
        } else if (sports_candidate_sequence_counters.sequence_count >= 5) {
            sports_candidate_sequence_counters.sequence_count = 0;
        } else {
            sports_candidate_sequence_counters.sequence_count = sports_candidate_sequence_counters.sequence_count + 1;
        }
        sports_candidate_input_latched = 0;
        sports_candidate_sequence_counters.primary_frame_counter = 0;
    }
    D_80120668 = sports_candidate_sequence_counters.sequence_count;
    if (sports_candidate_input_and_boundary_state.boundary_side != 0) {
        if (sports_candidate_sequence_step == 2 || sports_candidate_sequence_step == 4 || sports_candidate_sequence_step == 6) {
            D_80120668 += 0x48;
        }
    }
}

void sports_candidate_pointer_position_clamp(void) {
    if (D_80120676 > 0xA0) {
        D_80120676 = 0xA0;
    }
    if (D_80120676 < -0xA0) {
        D_80120676 = -0xA0;
    }
    if (D_8012067A > 0x30) {
        D_8012067A = 0x30;
    }
    if (D_8012067A < -0x18) {
        D_8012067A = -0x18;
    }
}

s32 sports_candidate_pointer_region_select(void) {
    s32 v;

    if (sports_candidate_input_and_boundary_state.boundary_side != 0) {
        return 7;
    }
    if (D_8012067A < 0x31 && D_8012067A >= ((s16)D_80149968.screen_coordinates[1] >> 3) + 0x10 && ((s16)D_80149968.screen_coordinates[0] >> 3) + 0x50 >= D_80120676 && D_80120676 >= ((s16)D_80149968.screen_coordinates[0] >> 3) - 0x50) {
        return 1;
    }
    v = ((s16)D_80149968.screen_coordinates[1] >> 3) + 0x10;
    if (D_8012067A < v && ((s16)D_80149968.screen_coordinates[0] >> 3) + 0x50 < D_80120676) {
        return 2;
    }
    if (D_8012067A < v && D_80120676 < ((s16)D_80149968.screen_coordinates[0] >> 3) - 0x50) {
        return 3;
    }
    if (D_8012067A >= v && D_80120676 < ((s16)D_80149968.screen_coordinates[0] >> 3) - 0x50) {
        return 5;
    }
    if (D_8012067A >= v && ((s16)D_80149968.screen_coordinates[0] >> 3) + 0x50 < D_80120676) {
        return 6;
    }
    return 0;
}

void sports_candidate_sequence_effect_dispatch(void) {
    if (sports_candidate_input_and_boundary_state.boundary_side == 0) {
        sports_candidate_sequence_state_dispatch();
    } else {
        sports_candidate_effect_motion_state_update();
    }
    if ((u16)sports_candidate_player_progress_record[0].unk0 == 0) {
        sports_candidate_effect_animation_step();
    }
}

void sports_candidate_sequence_state_dispatch(void) {
    switch (D_80149940) {
    case 0:
        sports_candidate_sequence_effect_update_a();
        break;
    case 1:
        sports_candidate_sequence_effect_update_b();
        break;
    case 2:
        sports_candidate_sequence_audio_counter_update();
        break;
    case 3:
        sports_candidate_sequence_effect_update_c();
        break;
    case 4:
        sports_candidate_sequence_effect_update_d();
        break;
    case 5:
        sports_candidate_sequence_effect_update_e();
        break;
    case 6:
        sports_candidate_sequence_effect_update_f();
        break;
    case 7:
        sports_candidate_sequence_effect_phase_update();
        break;
    case 8:
        sports_candidate_sequence_direction_audio_update();
        break;
    case 9:
        sports_candidate_actor_proximity_motion_update();
        break;
    case 10:
        sports_candidate_sequence_effect_update_g();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

/* FAKE: T-19008. The counter sports_candidate_sequence_motion.unk_02 and the flag sports_candidate_player_progress_record[0].val are read through u16 pointer
 * views, as in sports_candidate_sequence_direction_audio_update (the plain field gets a CVT copy of the promoted counter; with the volatile
 * sports_candidate_player_progress_value_view the lui of the flag load takes the delay slot the original gives to lw ra). tmp is a stack
 * word whose halves are written and read through s16 views (the 16.16 position of the sibling functions);
 * the x >> 3 term is written out at each use, a local x gets a range of its own. Real source unknown. */
void sports_candidate_sequence_effect_update_a(void) {
    s32 tmp;

    sports_candidate_motion_position_approach(0x18000);
    sports_candidate_motion_bounds_clamp();
    if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 4)) {
        sports_candidate_motion_velocity_set(0x10000);
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] % 6 == 0) {
        ((s16 *)&tmp)[0] = 0;
        ((s16 *)&tmp)[1] = D_80149968.screen_coordinates[0] >> 3;
        tmp += D_80149968.velocity_x * 40;
        ((s16 *)&tmp)[1] += (D_80149968.screen_coordinates[0] >> 3) * 40 / 48;
        if (D_80122EC4 % 3 != 0 && (D_80149968.screen_coordinates[0] >= 0 || D_80149968.velocity_x > 0)) {
            ((s16 *)&tmp)[1] = (D_80149968.screen_coordinates[0] >> 3) + 0x20;
        }
        sports_candidate_sequence_motion.target_x = ((s16 *)&tmp)[1];
        sports_candidate_sequence_motion.target_y = (D_80149968.screen_coordinates[1] >> 3) + 0x10;
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] >= 0x23A) {
        ((u16 *)sports_candidate_player_progress_record)[1] = 0;
        audio_command(0x802);
        ((u16 *)&sports_candidate_sequence_motion)[1] = 0;
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] != 0 && ((u16 *)&sports_candidate_sequence_motion)[1] % 30 == 0) {
        if (((u16 *)sports_candidate_player_progress_record)[1] == 0) {
            ((u16 *)sports_candidate_player_progress_record)[1] = 1;
            audio_command(0x800);
        } else {
            ((u16 *)sports_candidate_player_progress_record)[1] = 0;
            audio_command(0x801);
        }
        ((u16 *)&sports_candidate_sequence_motion)[1]++;
    }
}

/* FAKE: T-24124. The counter and flag use volatile/u16 views (as in sports_candidate_sequence_effect_update_c and sports_candidate_sequence_effect_update_g), and
 * tmp/v reproduce the original's stack word and ugen temporary before the 80 / 48 term. */
void sports_candidate_sequence_effect_update_b(void) {
    s32 tmp;
    s16 v;

    if (sports_candidate_sequence_frame_view == 0x258) {
        audio_command(0x802);
        ((u16 *)sports_candidate_player_progress_record)[0] = 1;
        sports_candidate_sequence_motion.acceleration_x = 0;
        sports_candidate_sequence_motion.acceleration_y = 0;
        ((u16 *)sports_candidate_player_progress_record)[1] = 0;
        D_801206AC = 0x7F;
    } else if (sports_candidate_sequence_frame_view >= 0x2D1) {
        ((u16 *)sports_candidate_player_progress_record)[0] = 0;
        ((u16 *)&sports_candidate_sequence_motion)[1] = 0;
    } else if (((u16 *)&sports_candidate_sequence_motion)[1] >= 0x259) {
        if (D_801206BE < 0x30) {
            D_801206BE++;
        }
    } else {
        sports_candidate_motion_position_approach(0x10000);
        sports_candidate_motion_bounds_clamp();
        if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 8)) {
            sports_candidate_motion_velocity_set(0x8000);
        }
        if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 0xF)) {
            ((s16 *)&tmp)[0] = 0;
            v = D_80149968.screen_coordinates[0] >> 3;
            v += (D_80149968.screen_coordinates[0] >> 3) * 80 / 48;
            if (D_80122EC4 % 3 != 0 && (D_80149968.screen_coordinates[0] >= 0 || D_80149968.velocity_x > 0)) {
                v = (D_80149968.screen_coordinates[0] >> 3) + 0x20;
            }
            sports_candidate_sequence_motion.target_x = v;
            sports_candidate_sequence_motion.target_y = (D_80149968.screen_coordinates[1] >> 3) + 0x10;
            ((s16 *)&tmp)[1] = v;
        }
    }
    if (((u16 *)sports_candidate_player_progress_record)[0] == 0 && ((u16 *)&sports_candidate_sequence_motion)[1] != 0 && ((u16 *)&sports_candidate_sequence_motion)[1] % 45 == 0) {
        if (((u16 *)sports_candidate_player_progress_record)[1] == 0) {
            ((u16 *)sports_candidate_player_progress_record)[1] = 1;
            audio_command(0x800);
        } else {
            ((u16 *)sports_candidate_player_progress_record)[1] = 0;
            audio_command(0x801);
        }
        ((u16 *)&sports_candidate_sequence_motion)[1]++;
    }
}

void sports_candidate_sequence_audio_counter_update(void) {
    if (sports_candidate_sequence_motion.sequence_frames == 1) {
        audio_command(0x800);
        return;
    }
    if ((u32)(u16)sports_candidate_sequence_motion.sequence_frames >= 0x2D1) {
        sports_candidate_sequence_motion.sequence_frames = 0;
    }
}

/* FAKE: T-19008. The counter and the flag are read through u16 pointer views and the halves of tmp are
 * written through s16 views, as in sports_candidate_sequence_direction_audio_update. v takes x >> 3 first and adds the x * 80 / 48 term in a
 * second statement: the in-place add gives the ugen temporary the original has before that chain (as1
 * drops the redundant sign extension of the first assignment). */
void sports_candidate_sequence_effect_update_c(void) {
    s32 tmp;
    s16 v;

    if (((u16 *)&sports_candidate_sequence_motion)[1] >= 0x258) {
        ((u16 *)sports_candidate_player_progress_record)[1] = 0;
        audio_command(0x802);
        ((u16 *)&sports_candidate_sequence_motion)[1] = 0;
    }
    sports_candidate_motion_position_approach(0x10000);
    sports_candidate_motion_bounds_clamp();
    if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 8)) {
        sports_candidate_motion_velocity_set(0x9000);
    }
    if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 0xF)) {
        ((s16 *)&tmp)[0] = 0;
        v = D_80149968.screen_coordinates[0] >> 3;
        v += (D_80149968.screen_coordinates[0] >> 3) * 80 / 48;
        if (D_80122EC4 % 3 != 0 && (D_80149968.screen_coordinates[0] >= 0 || D_80149968.velocity_x > 0)) {
            v = (D_80149968.screen_coordinates[0] >> 3) + 0x20;
        }
        sports_candidate_sequence_motion.target_x = v;
        sports_candidate_sequence_motion.target_y = (D_80149968.screen_coordinates[1] >> 3) + 0x10;
        ((s16 *)&tmp)[1] = v;
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] != 0 && ((u16 *)&sports_candidate_sequence_motion)[1] % 45 == 0) {
        if (((u16 *)sports_candidate_player_progress_record)[1] == 0) {
            ((u16 *)sports_candidate_player_progress_record)[1] = 1;
            audio_command(0x800);
        } else {
            ((u16 *)sports_candidate_player_progress_record)[1] = 0;
            audio_command(0x801);
        }
        ((u16 *)&sports_candidate_sequence_motion)[1]++;
    }
}

/* FAKE: T-24133. Same counter/flag u16 pointer views and split-half stack word `tmp` as `sports_candidate_sequence_direction_audio_update`;
 * `tmp2 = ((u16 *)&sports_candidate_sequence_motion)[1]; tmp2 = tmp2;` keeps the promoted counter live into the call block and
 * raises its priority into `$v1` ahead of `D_801206BA` (`$a0`). */
void sports_candidate_sequence_effect_update_d(void) {
    s32 tmp;
    s32 tmp2;

    if (((u16 *)&sports_candidate_sequence_motion)[1] >= 0x230) {
        ((u16 *)sports_candidate_player_progress_record)[1] = 0;
        audio_command(0x802);
        ((u16 *)&sports_candidate_sequence_motion)[1] = 0;
    }
    tmp2 = ((u16 *)&sports_candidate_sequence_motion)[1];
    tmp2 = tmp2;
    sports_candidate_motion_position_approach(0x15000);
    sports_candidate_motion_bounds_clamp();
    if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 4)) {
        sports_candidate_motion_velocity_set(0xF000);
    }
    if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 0xC)) {
        if (((u16 *)&sports_candidate_sequence_motion)[1] >= 0x227 && ((u16 *)&sports_candidate_sequence_motion)[1] < 0x258) {
            if (D_801206BA + 0x1E < D_80120676) {
                sports_candidate_sequence_motion.target_x = D_80120676 - 0x1E;
            } else if (D_80120676 < D_801206BA - 0x1E) {
                sports_candidate_sequence_motion.target_x = D_80120676 + 0x1E;
            }
            sports_candidate_sequence_motion.target_y = D_8012067A;
        } else {
            ((s16 *)&tmp)[0] = 0;
            ((s16 *)&tmp)[1] = D_80149968.screen_coordinates[0] >> 3;
            tmp += D_80149968.velocity_x * 80;
            sports_candidate_sequence_motion.target_x = ((s16 *)&tmp)[1];
            sports_candidate_sequence_motion.target_y = (D_80149968.screen_coordinates[1] >> 3) + 0x10;
        }
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] != 0) {
        if (((u16 *)&sports_candidate_sequence_motion)[1] % 35 == 0) {
            if (((u16 *)sports_candidate_player_progress_record)[1] == 0) {
                ((u16 *)sports_candidate_player_progress_record)[1] = 1;
                audio_command(0x800);
            } else {
                ((u16 *)sports_candidate_player_progress_record)[1] = 0;
                audio_command(0x801);
            }
            ((u16 *)&sports_candidate_sequence_motion)[1]++;
        }
    }
}

/* FAKE: T-24124. Same counter/flag pointer views and split-half stack word tmp as sports_candidate_sequence_effect_update_f and
 * sports_candidate_sequence_effect_update_g. */
void sports_candidate_sequence_effect_update_e(void) {
    s32 tmp;

    if (sports_candidate_sequence_frame_view == 1) {
        if (((D_80149968.screen_coordinates[0] >> 3) - D_801206BA >= 0x21 && sports_candidate_velocity_x_view > 0) ||
            ((D_80149968.screen_coordinates[0] >> 3) - D_801206BA < -0x20 && D_80149968.velocity_x < 0)) {
            audio_command(0x802);
            sports_candidate_player_progress_value_view = 2;
        }
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] >= 0x88) {
        ((u16 *)&sports_candidate_sequence_motion)[1] = 0;
    }
    sports_candidate_motion_position_approach(0x12000);
    sports_candidate_motion_bounds_clamp();
    if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 0x1F)) {
        sports_candidate_motion_velocity_set(0xC000);
    }
    if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 0xF)) {
        ((s16 *)&tmp)[0] = 0;
        ((s16 *)&tmp)[1] = D_80149968.screen_coordinates[0] >> 3;
        ((s16 *)&tmp)[1] += (D_80149968.screen_coordinates[0] >> 3) * 79 / 48;
        sports_candidate_sequence_motion.target_x = ((s16 *)&tmp)[1];
        sports_candidate_sequence_motion.target_y = (D_80149968.screen_coordinates[1] >> 3) + 0x10;
    }
    if (sports_candidate_player_progress_value_view == 2 && *(u16 *)&sports_candidate_sequence_frame_view >= 0x79) {
        sports_candidate_player_progress_value_view = 0;
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] != 0 && ((u16 *)&sports_candidate_sequence_motion)[1] % 90 == 0) {
        if (((u16 *)sports_candidate_player_progress_record)[1] == 0) {
            sports_candidate_player_progress_value_view = 1;
            audio_command(0x800);
            ((u16 *)&sports_candidate_sequence_motion)[1]++;
        } else if (*(u16 *)&sports_candidate_player_progress_value_view == 1) {
            sports_candidate_player_progress_value_view = 0;
            audio_command(0x801);
            ((u16 *)&sports_candidate_sequence_motion)[1]++;
        }
    }
}

/* FAKE: T-11640, same views as sports_candidate_sequence_effect_update_g (counter and flag reloaded per block, tmp split into s16 halves). */
void sports_candidate_sequence_effect_update_f(void) {
    s32 tmp;

    if (D_80149968.velocity_y == 0 && sports_candidate_player_progress_value_view != 2) {
        audio_command(0x802);
        sports_candidate_player_progress_value_view = 2;
        ((u16 *)&sports_candidate_sequence_motion)[1] = 0;
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] > 600) {
        ((u16 *)&sports_candidate_sequence_motion)[1] = 0x1F;
    }
    sports_candidate_motion_position_approach(0x1C000);
    sports_candidate_motion_bounds_clamp();
    if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 4)) {
        sports_candidate_motion_velocity_set(0x12000);
    }
    if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 0xF)) {
        ((s16 *)&tmp)[0] = 0;
        ((s16 *)&tmp)[1] = D_80149968.screen_coordinates[0] >> 3;
        tmp += D_80149968.velocity_x * 80;
        sports_candidate_sequence_motion.target_x = ((s16 *)&tmp)[1];
        sports_candidate_sequence_motion.target_y = (D_80149968.screen_coordinates[1] >> 3) + 0x10;
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] != 0 && ((u16 *)&sports_candidate_sequence_motion)[1] % 30 == 0 && ((u16 *)&sports_candidate_sequence_motion)[1] >= 60) {
        if (sports_candidate_player_progress_value_view == 0) {
            sports_candidate_player_progress_value_view = 1;
            audio_command(0x800);
            ((u16 *)&sports_candidate_sequence_motion)[1]++;
        } else if (*(u16 *)&sports_candidate_player_progress_value_view == 1) {
            sports_candidate_player_progress_value_view = 0;
            audio_command(0x801);
            ((u16 *)&sports_candidate_sequence_motion)[1]++;
        } else {
            sports_candidate_player_progress_value_view = 0;
        }
    }
}

/* FAKE: T-19008, T-24133. The counter and the flag are read through u16 pointer views as in sports_candidate_sequence_direction_audio_update,
 * and `tmp = ((u16 *)&sports_candidate_sequence_motion)[1]; tmp = tmp;` keeps the promoted counter live into the call block so
 * uopt spills `$v0` back to `sports_candidate_sequence_frame_view` before `sports_candidate_motion_position_approach` without taking a ugen temporary. */
void sports_candidate_sequence_effect_phase_update(void) {
    s16 v;
    s32 tmp;

    if (((u16 *)&sports_candidate_sequence_motion)[1] == 0x12C) {
        ((u16 *)sports_candidate_player_progress_record)[0] = 1;
        if (((u16 *)sports_candidate_player_progress_record)[1] == 0) {
            ((u16 *)sports_candidate_player_progress_record)[1] = 1;
            audio_command(0x800);
        } else {
            ((u16 *)sports_candidate_player_progress_record)[1] = 0;
            audio_command(0x801);
        }
        sports_candidate_sequence_motion.acceleration_x = 0;
        sports_candidate_sequence_motion.acceleration_y = 0;
        ((u16 *)&sports_candidate_sequence_motion)[1]++;
        D_801206AC = 0x7C;
        sports_candidate_motion_bounds_clamp();
        return;
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] == 0x12F) {
        D_801206AC = 0x7D;
        sports_candidate_motion_bounds_clamp();
        return;
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] == 0x131) {
        D_801206AC = 0x7E;
        sports_candidate_motion_bounds_clamp();
        return;
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] < 0x1A5 && ((u16 *)&sports_candidate_sequence_motion)[1] >= 0x12D) {
        sports_candidate_motion_bounds_clamp();
        return;
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] >= 0x1A5) {
        sports_candidate_motion_bounds_clamp();
        ((u16 *)sports_candidate_player_progress_record)[0] = 0;
        ((u16 *)&sports_candidate_sequence_motion)[1] = 0;
        return;
    }
    tmp = ((u16 *)&sports_candidate_sequence_motion)[1];
    tmp = tmp;
    sports_candidate_motion_position_approach(0x12000);
    sports_candidate_motion_bounds_clamp();
    if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 8)) {
        sports_candidate_motion_velocity_set(0xE000);
    }
    if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 0xF)) {
        v = D_80149968.screen_coordinates[0] >> 3;
        v += (D_80149968.screen_coordinates[0] >> 3) * 79 / 48;
        if (D_80122EC4 % 3 != 0 && (D_80149968.screen_coordinates[0] >= 0 || D_80149968.velocity_x > 0)) {
            v = (D_80149968.screen_coordinates[0] >> 3) + 0x20;
        }
        sports_candidate_sequence_motion.target_x = v;
        sports_candidate_sequence_motion.target_y = (D_80149968.screen_coordinates[1] >> 3) + 0x10;
    }
}

/* FAKE: T-11640. The counter sports_candidate_sequence_motion.unk_02 and the flag sports_candidate_player_progress_record[0].val are read through u16 pointer
 * views: with the plain field IDO keeps a CVT copy of the promoted counter in a second register, and the
 * volatile sports_candidate_player_progress_value_view of sports_candidate_sequence_effect_update_g stops the lui of the flag load from moving into the branch delay slot.
 * tmp is a stack word whose halves are written and read through s16 views (as in sports_candidate_sequence_effect_update_g).
 * Real source unknown (same family as sports_candidate_sequence_effect_update_f/sports_candidate_sequence_effect_update_g). */
void sports_candidate_sequence_direction_audio_update(void) {
    s32 tmp;

    sports_candidate_motion_position_approach(0x15000);
    sports_candidate_motion_bounds_clamp();
    if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 4)) {
        sports_candidate_motion_velocity_set(0xF000);
    }
    if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 0xC)) {
        ((s16 *)&tmp)[0] = 0;
        ((s16 *)&tmp)[1] = D_80149968.screen_coordinates[0] >> 3;
        tmp += D_80149968.velocity_x * 80;
        sports_candidate_sequence_motion.target_x = ((s16 *)&tmp)[1];
        sports_candidate_sequence_motion.target_y = (D_80149968.screen_coordinates[1] >> 3) + 0x10;
    }
    if (((u16 *)sports_candidate_player_progress_record)[1] != 0 && ((u16 *)&sports_candidate_sequence_motion)[1] > 64) {
        ((u16 *)sports_candidate_player_progress_record)[1] = 0;
    }
    if (sports_candidate_input_and_boundary_state.boundary_side == 0 && !(((u16 *)&sports_candidate_sequence_motion)[1] & 0x1F) && ((u16 *)sports_candidate_player_progress_record)[1] == 0) {
        if (D_80149968.velocity_x > 0) {
            if (D_801206BA < (D_80149968.screen_coordinates[0] >> 3) - 0x18) {
                ((u16 *)sports_candidate_player_progress_record)[1] = 1;
                ((u16 *)&sports_candidate_sequence_motion)[1] = 0;
                audio_command(0x802);
            } else {
                audio_command(0x801);
            }
        } else if (D_80149968.velocity_x < 0) {
            if ((D_80149968.screen_coordinates[0] >> 3) + 0x18 < D_801206BA) {
                ((u16 *)sports_candidate_player_progress_record)[1] = 1;
                ((u16 *)&sports_candidate_sequence_motion)[1] = 0;
                audio_command(0x802);
            } else {
                audio_command(0x800);
            }
        }
        ((u16 *)&sports_candidate_sequence_motion)[1]++;
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] > 128) {
        ((u16 *)&sports_candidate_sequence_motion)[1] = 64;
    }
}

/* FAKE: T-11640, the u16 pointer views of the counter and the flag and the s16 views of tmp as in sports_candidate_sequence_direction_audio_update. */
void sports_candidate_actor_proximity_motion_update(void) {
    s32 tmp;

    sports_candidate_motion_position_approach(0x11800);
    sports_candidate_motion_bounds_clamp();
    if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 8)) {
        sports_candidate_motion_velocity_set(0x8000);
    }
    if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 0xF)) {
        ((s16 *)&tmp)[0] = 0;
        ((s16 *)&tmp)[1] = D_80149968.screen_coordinates[0] >> 3;
        tmp += D_80149968.velocity_x * 80;
        sports_candidate_sequence_motion.target_x = ((s16 *)&tmp)[1];
        sports_candidate_sequence_motion.target_y = (D_80149968.screen_coordinates[1] >> 3) + 0x10;
    }
    if (D_80120676 < ((ActorFixed *)actor_pool)[97].x.h[1] &&
        abs(D_8012067A - D_801206BE) < 12 &&
        ((ActorFixed *)actor_pool)[97].x.h[1] - 0x20 < D_80120676) {
        ((ActorFixed *)actor_pool)[97].x.w += 0x11800;
    } else if (((ActorFixed *)actor_pool)[97].x.h[1] < D_80120676 &&
               abs(D_8012067A - D_801206BE) < 12 &&
               D_80120676 < ((ActorFixed *)actor_pool)[97].x.h[1] + 0x20) {
        ((ActorFixed *)actor_pool)[97].x.w -= 0x11800;
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] > 128) {
        ((u16 *)&sports_candidate_sequence_motion)[1] = 0x20;
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] != 0 && !(((u16 *)&sports_candidate_sequence_motion)[1] & 0x1F) && ((u16 *)&sports_candidate_sequence_motion)[1] > 32) {
        if (((u16 *)sports_candidate_player_progress_record)[1] == 0) {
            ((u16 *)sports_candidate_player_progress_record)[1] = 1;
            ((u16 *)&sports_candidate_sequence_motion)[1]++;
            audio_command(0x800);
        } else if (((u16 *)sports_candidate_player_progress_record)[1] == 1) {
            ((u16 *)sports_candidate_player_progress_record)[1] = 0;
            ((u16 *)&sports_candidate_sequence_motion)[1]++;
            audio_command(0x801);
        } else {
            ((u16 *)sports_candidate_player_progress_record)[1] = 0;
        }
    }
}

/* FAKE: T-11640. The original reloads the counter sports_candidate_sequence_motion.unk_02 and the flag sports_candidate_player_progress_value_view in
 * every block where IDO keeps them in a register: the counter is read through a u16 view of
 * sports_candidate_sequence_motion, the flag is declared volatile and its second test reads it through a plain u16
 * view (all-volatile reads change the delay-slot fill). tmp is a stack word whose halves are written and
 * read through s16 views (the 16.16 position of the four sibling functions). Real source unknown. */
void sports_candidate_sequence_effect_update_g(void) {
    s32 tmp;

    if (D_80149968.velocity_y == 0 && sports_candidate_player_progress_value_view != 2) {
        sports_candidate_player_progress_value_view = 2;
        ((u16 *)&sports_candidate_sequence_motion)[1] = 0;
        audio_command(0x802);
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] > 600) {
        ((u16 *)&sports_candidate_sequence_motion)[1] = 0;
    }
    sports_candidate_motion_position_approach(0x18800);
    sports_candidate_motion_bounds_clamp();
    if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 4)) {
        sports_candidate_motion_velocity_set(0x10000);
    }
    if (!(((u16 *)&sports_candidate_sequence_motion)[1] & 0xC)) {
        ((s16 *)&tmp)[0] = 0;
        ((s16 *)&tmp)[1] = D_80149968.screen_coordinates[0] >> 3;
        tmp += D_80149968.velocity_x * 80;
        sports_candidate_sequence_motion.target_x = ((s16 *)&tmp)[1];
        sports_candidate_sequence_motion.target_y = (D_80149968.screen_coordinates[1] >> 3) + 0x10;
    }
    if (((u16 *)&sports_candidate_sequence_motion)[1] != 0 && ((u16 *)&sports_candidate_sequence_motion)[1] % 30 == 0 && ((u16 *)&sports_candidate_sequence_motion)[1] >= 60) {
        if (sports_candidate_player_progress_value_view == 0) {
            sports_candidate_player_progress_value_view = 1;
            audio_command(0x800);
            ((u16 *)&sports_candidate_sequence_motion)[1]++;
        } else if (*(u16 *)&sports_candidate_player_progress_value_view == 1) {
            sports_candidate_player_progress_value_view = 0;
            audio_command(0x801);
            ((u16 *)&sports_candidate_sequence_motion)[1]++;
        } else {
            sports_candidate_player_progress_value_view = 0;
        }
    }
}

void sports_candidate_motion_velocity_set(s32 motion_magnitude) {
    s32 var_a0;
    /* FAKE: two unused locals reproduce the original's frame (the real locals are unknown). T-10520 */
    s32 pad;
    s32 pad2;
    s32 sp40;
    s32 sp3C;
    s32 sp38;
    s32 sp34;

    if (abs((D_80149968.screen_coordinates[0] >> 3) - D_801206BA) < 0x50) {
        if (D_80149968.velocity_y < 0) {
            D_80149968.velocity_y = 0;
        }
        var_a0 = -((((D_80149968.screen_coordinates[0] >> 3) - D_801206BA) << 0xA) >> 6);
        if ((D_80122EC4 % 3) != 0) {
            var_a0 = var_a0 - 0x100;
        }
        sp40 = 0;
        sp3C = motion_magnitude;
        Rot_2D(var_a0, &sp40, &sp3C, &sp38, &sp34);
        sports_candidate_sequence_motion.acceleration_x = sp38;
        sports_candidate_sequence_motion.acceleration_y = sp34;
        return;
    }
    sports_candidate_sequence_motion.acceleration_x = 0;
    sports_candidate_sequence_motion.acceleration_y = 0;
}

void sports_candidate_motion_position_approach(s32 position_step)
{
  /* FAKE: empty conditions retain the shared record base in V1 instead
   * of folding the word accesses to separate addresses. T-9510. */
  TkCoordsView *actor_fixed_point_coordinates = (TkCoordsView *) actor_pool;
  if ((sports_candidate_sequence_motion.target_x + 2) < D_801206BA)
  {
    actor_fixed_point_coordinates->pos[0].word -= position_step;
  }
  else
  {
    if ((actor_fixed_point_coordinates->pos[0].halves.hi + 2) < sports_candidate_sequence_motion.target_x)
    {
      actor_fixed_point_coordinates->pos[0].word += position_step;
    }
    if (!actor_fixed_point_coordinates->pos[0].halves.hi)
    {
    }
    if (!actor_fixed_point_coordinates->pos[1].halves.hi)
    {
    }
  }
  if ((sports_candidate_sequence_motion.target_y + 2) < D_801206BE)
  {
    actor_fixed_point_coordinates->pos[1].word -= position_step;
    return;
  }
  if ((actor_fixed_point_coordinates->pos[1].halves.hi + 2) < sports_candidate_sequence_motion.target_y)
  {
    actor_fixed_point_coordinates->pos[1].word += position_step;
  }
}

void sports_candidate_motion_bounds_clamp(void) {
    if (D_801206BE < (D_80149968.screen_coordinates[1] >> 3) + 0x10) {
        if (D_801206BA < (D_80149968.screen_coordinates[0] >> 3) + 0x50 && (D_80149968.screen_coordinates[0] >> 3) - 0x50 < D_801206BA) {
            D_801206BE = (D_80149968.screen_coordinates[1] >> 3) + 0x10;
        }
    }
    if ((D_80149968.screen_coordinates[0] >> 3) - 0x50 < D_801206BA && D_801206BA < (D_80149968.screen_coordinates[0] >> 3) && D_801206BE < (D_80149968.screen_coordinates[1] >> 3) + 0x10) {
        D_801206BA = (D_80149968.screen_coordinates[0] >> 3) - 0x50;
    }
    if (D_801206BA < (D_80149968.screen_coordinates[0] >> 3) + 0x50) {
        if ((D_80149968.screen_coordinates[0] >> 3) < D_801206BA && D_801206BE < (D_80149968.screen_coordinates[1] >> 3) + 0x10) {
            D_801206BA = (D_80149968.screen_coordinates[0] >> 3) + 0x50;
        }
    }
}

void sports_candidate_effect_animation_step(void) {
    TaiikuU11 sequence_frame_limit_table;
    TaiikuS11 sequence_frame_index_table;

    sequence_frame_limit_table = sports_candidate_sequence_frame_limits;
    sequence_frame_index_table = D_80149F68;
    if (sequence_frame_limit_table.v[D_80149940] < sports_candidate_sequence_motion.action_frames && (D_80149940 != 2 || (D_80149940 == 2 && sports_candidate_motion_speed > 0) || sports_candidate_input_and_boundary_state.boundary_side != 0)) {
        if (sports_candidate_secondary_sequence_count >= 3 && sports_candidate_input_and_boundary_state.boundary_side != 0 && (sports_candidate_secondary_sequence_step == 2 || sports_candidate_secondary_sequence_step == 4 || sports_candidate_secondary_sequence_step == 6)) {
            sports_candidate_secondary_sequence_count = 0;
        } else if (sports_candidate_secondary_sequence_count >= 5) {
            sports_candidate_secondary_sequence_count = 0;
        } else {
            sports_candidate_secondary_sequence_count++;
        }
        sports_candidate_sequence_motion.action_frames = 0;
    }
    D_801206AC = D_80149940 * 6 + sports_candidate_secondary_sequence_count + 6;
    if (sports_candidate_input_and_boundary_state.boundary_side != 0 && (sports_candidate_secondary_sequence_step == 2 || sports_candidate_secondary_sequence_step == 4 || sports_candidate_secondary_sequence_step == 6)) {
        if (sports_candidate_secondary_sequence_count >= 3) {
            sports_candidate_secondary_sequence_count = 0;
        }
        D_801206AC = sequence_frame_index_table.v[D_80149940] + sports_candidate_secondary_sequence_count;
        if (D_80149940 == 2 && D_80120697 == 0xA5) {
            D_801206AC += 4;
        }
    }
}

void sports_candidate_effect_motion_state_update(void) {
    s32 motion_target_x;

    switch (sports_candidate_secondary_sequence_step) {
    case 0:
        if (D_801206BE < (D_80149968.screen_coordinates[1] >> 3) + 0x10 &&
            D_801206BA < (D_80149968.screen_coordinates[0] >> 3) + 0x50 && (D_80149968.screen_coordinates[0] >> 3) - 0x50 < D_801206BA) {
            D_801206BE = (D_80149968.screen_coordinates[1] >> 3) + 0x10;
        }
        if (D_801207CA < -0x1C0 && sports_candidate_motion_speed == 0) {
            sports_candidate_secondary_sequence_count = 0;
            sports_candidate_secondary_sequence_step++;
        }
        break;
    case 1:
        sports_candidate_secondary_sequence_step++;
        break;
    case 2:
        sports_candidate_secondary_sequence_step++;
        break;
    case 3:
        if (sports_candidate_input_and_boundary_state.boundary_side == 1) {
            D_801206BA = 0x90;
        } else {
            D_801206BA = -0x90;
            D_80120697 |= 1;
        }
        D_801206BE = 0x20;
        sports_candidate_secondary_sequence_step++;
        break;
    case 4:
        if (sports_candidate_input_and_boundary_state.boundary_side == 1 && sports_candidate_sequence_step >= 4) {
            D_801206BA = (D_80149968.screen_coordinates[0] >> 3) + 0x60;
            D_801206BE = D_80149968.screen_coordinates[1] >> 3;
            if (D_80149968.position_x <= 0) {
                sports_candidate_secondary_sequence_count = 0;
                D_80149968.velocity_x = 0;
                sports_candidate_secondary_sequence_step++;
            }
        }
        if (sports_candidate_input_and_boundary_state.boundary_side == 2 && sports_candidate_sequence_step >= 4) {
            D_801206BA = (D_80149968.screen_coordinates[0] >> 3) - 0x60;
            D_801206BE = D_80149968.screen_coordinates[1] >> 3;
            if (D_80149968.position_x >= 0) {
                sports_candidate_secondary_sequence_count = 0;
                D_80120697 = 0xA4;
                D_80149968.velocity_x = 0;
                D_801206BA = -0x50;
                sports_candidate_secondary_sequence_step++;
            }
        }
        break;
    case 5:
        if (D_801206BE < 0x30) {
            D_801206BE += 2;
        } else {
            D_801206BE = 0x30;
            if (sports_candidate_input_and_boundary_state.boundary_side == 2) {
                D_80120697 |= 1;
            }
            sports_candidate_secondary_sequence_count = 0;
            sports_candidate_secondary_sequence_step++;
        }
        break;
    case 6:
        if (D_80122EC4 % 3 == 0 || D_80149940 == 2) {
            motion_target_x = -0x20;
        } else {
            motion_target_x = 0;
        }
        if (sports_candidate_input_and_boundary_state.boundary_side == 1) {
            if (motion_target_x < D_801206BA) {
                D_801206BA -= 2;
            } else {
                D_801206BA = motion_target_x;
                sports_candidate_secondary_sequence_step++;
            }
        } else if (D_801206BA < motion_target_x) {
            D_801206BA += 2;
        } else {
            D_801206BA = motion_target_x;
            sports_candidate_secondary_sequence_step++;
        }
        break;
    case 7:
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void sports_candidate_motion_update(void) {
    sports_candidate_motion_input_update();
    sports_candidate_motion_integrate();
    sports_candidate_motion_project_and_bound();
    sports_candidate_model_rotation_update();
}

void sports_candidate_motion_input_update(void) {
    s32 unused; /* FAKE: T-11640, reserves the original's frame slot above a0 (no use in the code) */
    s32 a0;
    s32 sp44;
    s32 sp40;
    s32 sp3C;
    s32 sp38;

    if (sports_candidate_input_and_boundary_state.boundary_side != 0) {
        D_80149968.acceleration_x = 0;
        D_80149968.acceleration_y = 0;
    } else if (sports_candidate_pointer_proximity_test() != 0 && (game_state.pad_pressed & PAD_CIRCLE)) {
        if (D_80149968.velocity_y < 0) {
            D_80149968.velocity_y = 0;
        }
        a0 = -(((((s16) D_80149968.screen_coordinates[0] >> 3) - D_80120676) << 10) >> 6);
        if (D_80122EC4 % 3 != 0) {
            a0 = -(((((s16) D_80149968.screen_coordinates[0] >> 3) - D_80120676) << 10) >> 6) - 0x200;
        }
        sp44 = 0;
        sp40 = 0x10000;
        Rot_2D(a0, &sp44, &sp40, &sp3C, &sp38);
        sports_candidate_input_and_boundary_state.input_acceleration_x = sp3C;
        sports_candidate_input_and_boundary_state.input_acceleration_y = sp38;
        D_80149968.input_decay_frames = 0;
    } else if (D_80149968.input_decay_frames > 10) {
        D_80149968.input_decay_frames = 0;
        if (sports_candidate_sequence_motion.acceleration_y <= 0 && (u16) sports_candidate_player_progress_record[0].unk0 == 0) {
            sports_candidate_input_and_boundary_state.input_acceleration_y -= 0x4000;
            if (D_80149968.velocity_x > 0) {
                sports_candidate_input_and_boundary_state.input_acceleration_x -= 0x4000;
            } else if (D_80149968.velocity_x < 0) {
                sports_candidate_input_and_boundary_state.input_acceleration_x += 0x4000;
            }
        } else {
            sports_candidate_input_and_boundary_state.input_acceleration_x = 0;
            sports_candidate_input_and_boundary_state.input_acceleration_y = 0;
        }
    }
    if (sports_candidate_input_and_boundary_state.boundary_side == 0) {
        D_80149968.acceleration_x = sports_candidate_input_and_boundary_state.input_acceleration_x + sports_candidate_sequence_motion.acceleration_x;
        D_80149968.acceleration_y = sports_candidate_input_and_boundary_state.input_acceleration_y + sports_candidate_sequence_motion.acceleration_y;
    }
}

s32 sports_candidate_pointer_proximity_test(void) {
    s32 dx;
    s32 vertical_distance;

    dx = abs(((s16) D_80149968.screen_coordinates[0] >> 3) - D_80120676);
    vertical_distance = abs(((s16) D_80149968.screen_coordinates[1] >> 3) - D_8012067A);
    if (sports_candidate_pointer_region != 1) {
        return 0;
    }
    if ((0x10 - (dx >> 2)) >= (vertical_distance - 0x10)) {
        return 1;
    }
    return 0;
}

void sports_candidate_motion_integrate(void) {
    if (sports_candidate_input_and_boundary_state.boundary_side != 0 && sports_candidate_sequence_step == 4 && sports_candidate_secondary_sequence_step == 4) {
        D_80149968.velocity_y = 0;
        if (sports_candidate_input_and_boundary_state.boundary_side == 1) {
            D_80149968.velocity_x = -0x10000;
            return;
        }
        D_80149968.velocity_x = 0x10000;
        return;
    }
    if (D_80149968.velocity_update_frames > 10) {
        D_80149968.velocity_update_frames = 0;
        D_80149968.velocity_y += D_80149968.acceleration_y;
        if (D_80149968.velocity_y < 0) {
            D_80149968.velocity_y = 0;
        }
        if (D_80149968.velocity_y > 0x10000) {
            D_80149968.velocity_y = 0x10000;
        }
        if (D_80149968.velocity_x > 0) {
            D_80149968.velocity_x += D_80149968.acceleration_x;
            if (D_80149968.velocity_x <= 0) {
                sports_candidate_input_and_boundary_state.input_acceleration_x = 0;
                sports_candidate_sequence_motion.acceleration_x = 0;
                D_80149968.acceleration_x = 0;
                D_80149968.velocity_x = 0;
            }
        } else if (D_80149968.velocity_x < 0) {
            D_80149968.velocity_x += D_80149968.acceleration_x;
            if (D_80149968.velocity_x >= 0) {
                sports_candidate_input_and_boundary_state.input_acceleration_x = 0;
                sports_candidate_sequence_motion.acceleration_x = 0;
                D_80149968.acceleration_x = 0;
                D_80149968.velocity_x = 0;
            }
        } else {
            D_80149968.velocity_x += D_80149968.acceleration_x;
        }
        if (D_80149968.velocity_x < -0x10000) {
            D_80149968.velocity_x = -0x10000;
        }
        if (D_80149968.velocity_x > 0x10000) {
            D_80149968.velocity_x = 0x10000;
        }
        if (D_80149968.velocity_y < -0x10000) {
            D_80149968.velocity_y = -0x10000;
        }
        if (D_80149968.velocity_y > 0x10000) {
            D_80149968.velocity_y = 0x10000;
        }
    }
}

/* FAKE: T-11640, the high halves of the 16.16 words (unk_14, unk_18) and of tmp are read and written through s16
 * views, which is what reproduces the original's code here. Real source unknown. */
void sports_candidate_motion_project_and_bound(void) {
    s32 tmp;

    if (D_80149968.acceleration_y <= 0) {
        D_80149968.position_y += 0x18000;
    } else {
        D_80149968.position_y -= 0x10000;
    }
    D_80149968.position_x += D_80149968.velocity_x;
    if (((s16 *)&D_80149968.position_x)[1] > 0x60) {
        sports_candidate_input_and_boundary_state.boundary_side = 1;
        ((s16 *)&D_80149968.position_x)[1] = 0x60;
        sports_candidate_player_progress_record[0].unk0 = 0;
        sports_candidate_motion_reset();
    } else if (((s16 *)&D_80149968.position_x)[1] < -0x60) {
        sports_candidate_input_and_boundary_state.boundary_side = 2;
        ((s16 *)&D_80149968.position_x)[1] = -0x60;
        sports_candidate_player_progress_record[0].unk0 = 0;
        sports_candidate_motion_reset();
    }
    if (((s16 *)&D_80149968.position_y)[1] < 0) {
        ((s16 *)&D_80149968.position_y)[1] = 0;
    }
    if (((s16 *)&D_80149968.position_y)[1] >= 0x80) {
        ((s16 *)&D_80149968.position_y)[1] = 0x80;
    }
    tmp = D_80149968.position_x * 4;
    D_80149968.screen_coordinates[0] = ((s16 *)&tmp)[1];
    tmp = D_80149968.position_y * 2;
    D_80149968.screen_coordinates[1] = ((s16 *)&tmp)[1];
}

void sports_candidate_motion_reset(void) {
    audio_command(0x504);
    sports_candidate_sequence_counters.sequence_count = 0;
    sports_candidate_secondary_sequence_count = 0;
    sports_candidate_player_progress_record[0].unk0 = 0;
    D_80149968.velocity_x = 0;
    sports_candidate_sequence_step = 0;
    sports_candidate_secondary_sequence_step = 0;
    D_801207A7 |= 0x80;
}

/* FAKE: T-24133. `u32 raw` with `if (game_state.param_athletics.unk_02 + raw) {}` and `raw = 0` places both promoted global
 * loads (`$a0`, `$a1`) in the entry block and keeps the `& 0xF` mask in `$v0` (`sports_candidate_model_rotation_update`) or `$v1` (`sports_candidate_motion_speed_and_scroll_update`);
 * `pad` reserves the top frame slot in `sports_candidate_model_rotation_update`. */
void sports_candidate_model_rotation_update(void) {
    s32 pad;
    s32 sp90;
    TaiikuSVec rot;
    MATRIX m1;
    MATRIX m2;
    VECTOR scale;
    s16 athletics_tier_value;
    s32 low_input_bits;
    u32 raw;

    raw = game_state.player_flags.b[1];
    if (game_state.param_athletics.value + raw) {}
    low_input_bits = raw & 0xF;
    athletics_tier_value = game_state.param_athletics.value;
    raw = 0;
    if ((low_input_bits & 1) || (low_input_bits & 2)) {
        athletics_tier_value = game_state.param_athletics.value >> 1;
    }
    sp90 = abs(D_80149968.velocity_y * 8);
    if ((sports_candidate_input_and_boundary_state.input_acceleration_y > 0) && (sports_candidate_sequence_motion.acceleration_y > 0)) {
        if (athletics_tier_value >= 0xA0) {
            sp90 = abs(D_80149968.velocity_y * 0x10);
        } else if (athletics_tier_value >= 0x64) {
            sp90 += abs(D_80149968.velocity_x * 2) * 3;
        } else if (athletics_tier_value >= 0x4B) {
            sp90 += abs(D_80149968.velocity_x * 4);
        } else if (athletics_tier_value >= 0x32) {
            sp90 += abs(D_80149968.velocity_x * 2);
        }
    } else if (D_80149940 == 2) {
        if (athletics_tier_value >= 0x190) {
            sp90 = abs(D_80149968.velocity_y * 0x10);
        } else if (athletics_tier_value >= 0x12C) {
            sp90 += abs(D_80149968.velocity_x * 2) * 3;
        } else if (athletics_tier_value >= 0xC8) {
            sp90 += abs(D_80149968.velocity_x * 4);
        } else if (athletics_tier_value >= 0x64) {
            sp90 += abs(D_80149968.velocity_x * 2);
        }
    }
    if (sports_candidate_input_and_boundary_state.boundary_side == 0) {
        if (D_80149940 == 6) {
            ((s16 *)&sp90)[1] += 8;
        }
        if (D_80149940 == 0xA) {
            ((s16 *)&sp90)[1] += 4;
        }
    }
    if (sports_candidate_input_and_boundary_state.boundary_side == 0) {
        D_80149958 -= ((s16 *)&sp90)[1];
    } else if ((sports_candidate_sequence_step == 4) && (sports_candidate_secondary_sequence_step == 4)) {
        if (sports_candidate_input_and_boundary_state.boundary_side == 1) {
            D_80149958 += 8;
            (&D_80149958)[1] = 0x400;
        } else {
            D_80149958 += 8;
            (&D_80149958)[1] = -0x400;
        }
    }
    m1 = *(MATRIX *)view_base_matrix;
    rot.vx = 0;
    rot.vz = 0;
    rot.vy = (&D_80149958)[1];
    RotMatrix(&rot, &m1);
    m2 = *(MATRIX *)view_base_matrix;
    rot.vy = 0;
    rot.vz = 0;
    rot.vx = D_80149958;
    RotMatrix(&rot, &m2);
    MulMatrix(&m1, &m2);
    scale.vx = 0x1200;
    scale.vy = 0x1200;
    scale.vz = 0x1200;
    ScaleMatrix(&m2, &scale);
    MulMatrix(&m1, &m2);
    m1.t[0] = D_80149968.screen_coordinates[0];
    m1.t[1] = D_80149968.screen_coordinates[1];
    m1.t[2] = 0x1B58;
    *(MATRIX *)D_801227A4 = m1;
    view_coordinates->flag = 0;
}

/* FAKE: T-24133. `u32 raw` with `if (game_state.param_athletics.unk_02 + raw) {}` and `raw = 0` keeps the raw flags byte in $a0
 * and its `& 0xF` mask in $v1 without a J<-L widening CVT, while loading `unk_02` directly into $v0. */
void sports_candidate_motion_speed_and_scroll_update(void) {
    s16 athletics_tier_value;
    s32 speed_limit;
    s32 low_input_bits;
    u32 raw;

    raw = game_state.player_flags.b[1];
    if (game_state.param_athletics.value + raw) {}
    low_input_bits = raw & 0xF;
    athletics_tier_value = game_state.param_athletics.value;
    raw = 0;
    if ((low_input_bits & 1) || (low_input_bits & 2)) {
        athletics_tier_value = game_state.param_athletics.value >> 1;
    }
    if (athletics_tier_value >= 0xA0) {
        speed_limit = 0x18000;
    } else if (athletics_tier_value >= 0x64) {
        speed_limit = 0x16000;
    } else if (athletics_tier_value >= 0x4B) {
        speed_limit = 0x14000;
    } else if (athletics_tier_value >= 0x32) {
        speed_limit = 0x12000;
    } else {
        speed_limit = 0x10000;
    }
    if (D_80149940 == 2) {
        if (athletics_tier_value >= 0x190) {
            speed_limit = 0x18000;
        } else if (athletics_tier_value >= 0x12C) {
            speed_limit = 0x16000;
        } else if (athletics_tier_value >= 0xC8) {
            speed_limit = 0x14000;
        } else if (athletics_tier_value >= 0x64) {
            speed_limit = 0x12000;
        } else {
            speed_limit = 0x10000;
        }
    }
    if (sports_candidate_speed_update_frames >= 0xB) {
        if (D_80149968.acceleration_y > 0) {
            sports_candidate_motion_speed += 0x2000;
        } else if (sports_candidate_motion_speed >= 0x10001) {
            sports_candidate_motion_speed -= 0x4000;
        } else {
            sports_candidate_motion_speed -= 0x1800;
        }
        if ((sports_candidate_motion_speed >= 0xE001) && (sports_candidate_input_and_boundary_state.input_acceleration_y <= 0)) {
            sports_candidate_motion_speed -= 0x4000;
            if (sports_candidate_motion_speed < 0xE000) {
                sports_candidate_motion_speed = 0xE000;
            }
        }
        if (speed_limit < sports_candidate_motion_speed) {
            sports_candidate_motion_speed = speed_limit;
        }
        if (sports_candidate_motion_speed < 0) {
            sports_candidate_motion_speed = 0;
        }
        sports_candidate_speed_update_frames = 0;
    }
    D_80149964 -= sports_candidate_motion_speed;
    if (((s16 *)&D_80149964)[1] < 0) {
        ((s16 *)&D_80149964)[1] = 0x80;
    }
    sports_candidate_background_quad_draw();
    debug_print_hex(sports_candidate_motion_speed, 0, -0x64);
    sports_candidate_player_position.word += sports_candidate_motion_speed;
}

/* FAKE: T-11640, the (u8) narrowing of v before "+ 8" changes no stored value (the fields are u8) but
 * reproduces the original's ugen temporaries; perhaps a u8 texture-coordinate macro in the real source. */
void sports_candidate_background_quad_draw(void) {
    s16 dx;
    s16 dy;
    s32 i;
    s32 row;
    POLY_FT4 *p;

    dx = game_state.draw_origin_x[draw_buffer];
    dy = game_state.draw_origin_y[draw_buffer];
    for (i = 0; i < 80; i++) {
        p = GetWorkBase(0x28, draw_buffer);
        func_8009F040(p);
        SetSemiTrans(p, 0);
        SetShadeTex(p, 0);
        row = i >> 2;
        switch (row) {
        case 0:
            p->r0 = 0x68;
            p->g0 = 0x68;
            p->b0 = 0x68;
            p->u0 = 0;
            p->v0 = (D_80149966 >> 3) + row * 8;
            p->u1 = 0x40;
            p->v1 = (D_80149966 >> 3) + row * 8;
            p->u2 = 0;
            p->v2 = (u8) ((D_80149966 >> 3) + row * 8) + 8;
            p->u3 = 0x40;
            p->v3 = (u8) ((D_80149966 >> 3) + row * 8) + 8;
            break;
        case 1:
            p->r0 = 0x70;
            p->g0 = 0x70;
            p->b0 = 0x70;
            p->u0 = 0;
            p->v0 = (D_80149966 >> 2) + row * 8;
            p->u1 = 0x40;
            p->v1 = (D_80149966 >> 2) + row * 8;
            p->u2 = 0;
            p->v2 = (u8) ((D_80149966 >> 2) + row * 8) + 8;
            p->u3 = 0x40;
            p->v3 = (u8) ((D_80149966 >> 2) + row * 8) + 8;
            break;
        case 2:
            p->r0 = 0x78;
            p->g0 = 0x78;
            p->b0 = 0x78;
            p->u0 = 0;
            p->v0 = (D_80149966 >> 1) + row * 8;
            p->u1 = 0x40;
            p->v1 = (D_80149966 >> 1) + row * 8;
            p->u2 = 0;
            p->v2 = (u8) ((D_80149966 >> 1) + row * 8) + 8;
            p->u3 = 0x40;
            p->v3 = (u8) ((D_80149966 >> 1) + row * 8) + 8;
            break;
        default:
            p->r0 = 0x80;
            p->g0 = 0x80;
            p->b0 = 0x80;
            p->u0 = 0;
            p->v0 = D_80149966 + row * 8;
            p->u1 = 0x40;
            p->v1 = D_80149966 + row * 8;
            p->u2 = 0;
            p->v2 = (u8) (D_80149966 + row * 8) + 8;
            p->u3 = 0x40;
            p->v3 = (u8) (D_80149966 + row * 8) + 8;
            break;
        }
        p->x0 = D_80149B24[i] + dx;
        p->y0 = D_801499E4[i] + dy;
        p->x1 = D_80149BC4[i] + dx;
        p->y1 = D_801499E4[i] + dy;
        p->x2 = D_80149C64[i] + dx;
        p->y2 = D_80149A84[i] + dy;
        p->x3 = D_80149D04[i] + dx;
        p->y3 = D_80149A84[i] + dy;
        p->tpage = func_8009ECB0(1, 0, 0x300, 0);
        p->clut = func_8009ED74(0, 0x1F0);
        AddPrim(&draw_ot_tags[draw_buffer * 0x400 + 0x3EC], p);
    }
}

void sports_candidate_camera_transition_update(void) {
    s32 i;
    s32 j;
    s32 pos;
    s16 h;
    TaiHalves *p; /* FAKE: T-25102, the halves are read through p and the word directly: the original keeps
                   * &sports_candidate_player_position in s2 after each call but builds the word's address with %hi/%lo */

    p = &sports_candidate_player_position;
    switch (((TaiikuWork *)overlay_scratch)->step) {
    case 0:
    case 3:
    case 6:
        pos = sports_candidate_player_position.word;
        sports_candidate_player_position.word = pos >> 1;
        sports_candidate_floor_strip_draw();
        h = p->halves.high;
        /* FAKE: T-25102, an unsigned subtraction (same bits); a signed one lets uopt test h >= 0x100 as
         * bltz on this difference */
        ((TaiikuWork *)overlay_scratch)->camera.y = h - 0x100U;
        if (((TaiikuWork *)overlay_scratch)->step == 6) {
            ((TaiikuActor44 *)actor_pool)[102].unk_1A = (h << 4) * 3 + 0x1000;
            ((TaiikuActor44 *)actor_pool)[102].unk_1C = ((TaiikuActor44 *)actor_pool)[102].unk_1A;
            ((TaiikuActor44 *)actor_pool)[102].unk_26 = -(D_80149968.screen_coordinates[0] >> 2) - (h >> 1) - 40;
            ((TaiikuActor44 *)actor_pool)[102].unk_2A = -(h >> 4) - (h >> 1) - 104;
            if (h >= 0x100) {
                D_80149968.acceleration_y = 0;
                sports_candidate_motion_speed = 0;
                ((TaiikuWork *)overlay_scratch)->step = 0;
                mode_stage_next();
            }
        }
        sports_candidate_player_position.word = pos;
        if (((TaiikuWork *)overlay_scratch)->camera.y >= 0x100) {
            for (i = 0; i < 20; i++) {
                for (j = 0; j < 4; j++) {
                    D_80149B24[(i << 2) + j] = -160 + j * ((i << 2) + 80);
                    D_80149BC4[(i << 2) + j] = -160 + (j + 1) * ((i << 2) + 80);
                    D_80149C64[(i << 2) + j] = -160 + j * (((i + 1) << 2) + 80);
                    D_80149D04[(i << 2) + j] = -160 + (j + 1) * (((i + 1) << 2) + 80);
                }
            }
            p->halves.high -= 0x200;
            ((TaiikuWork *)overlay_scratch)->step++;
        }
        break;
    case 1:
    case 4:
        sports_candidate_rotated_floor_strip_draw();
        ((TaiikuWork *)overlay_scratch)->camera.angle = -p->halves.high * 4 + 0x800;
        if (((TaiikuWork *)overlay_scratch)->camera.angle < -0x3FF) {
            if (((TaiikuWork *)overlay_scratch)->step == 4) {
                ((TaiikuActor44 *)actor_pool)[102].unk_26 = 0xB0;
                ((TaiikuActor44 *)actor_pool)[102].unk_03 |= 0x80;
            }
            ((TaiikuWork *)overlay_scratch)->step++;
        }
        break;
    case 2:
    case 5:
        sports_candidate_floor_transition_draw();
        ((TaiikuWork *)overlay_scratch)->camera.angle = -p->halves.high * 4 + 0x800;
        if (((TaiikuWork *)overlay_scratch)->camera.angle < -0x7FF) {
            for (i = 0; i < 20; i++) {
                for (j = 0; j < 4; j++) {
                    D_80149B24[(i << 2) + j] = -160 - (i << 3) + ((i << 2) + 80) * j;
                    D_80149BC4[(i << 2) + j] = -80 - (i << 2) + ((i << 2) + 80) * j;
                    D_80149C64[(i << 2) + j] = -168 - (i << 3) + (((i + 1) << 2) + 80) * j;
                    D_80149D04[(i << 2) + j] = -84 - (i << 2) + (((i + 1) << 2) + 80) * j;
                }
            }
            ((TaiikuWork *)overlay_scratch)->camera.x = 0x118;
            ((TaiikuWork *)overlay_scratch)->camera.y = -0x100;
            ((TaiikuWork *)overlay_scratch)->camera.angle = 0;
            sports_candidate_player_position.word = 0;
            ((TaiikuWork *)overlay_scratch)->step++;
        }
        break;
    }
}

void sports_candidate_floor_strip_draw(void) {
    s32 i;
    s32 middle0;
    s32 middle1;
    s32 x[4];
    s32 y[4];
    s32 scale;
    s32 input_x;
    s32 input_y;
    s32 camera_x;
    s32 camera_y;
    s32 ball_x;
    s32 angle;
    s32 first_y;

    /* Straight strips. The chained stores share one register per constant (T-25102). */
    for (angle = 0; angle < 64; angle++) {
        x[1] = x[0] = -320;
        x[3] = x[2] = -240;
        y[0] = angle << 3;
        y[1] = (angle + 1) << 3;
        camera_x = ((TaiikuCamera0C *)overlay_scratch)->x;
        for (i = 0; i < 4; i++) {
            x[i] += camera_x;
        }
        camera_y = ((TaiikuCamera0C *)overlay_scratch)->y;
        for (i = 0; i < 2; i++) {
            y[i] += camera_y;
        }
        for (i = 0; i < 2; i++) {
            scale = func_800A0140(0x2AA);
            y[i] = (y[i] * scale) >> 12;
        }
        /* The copies come before the midpoints: ugen's temporaries follow this order (T-25102). */
        y[2] = y[0];
        y[3] = y[1];
        middle0 = (x[0] + x[2]) >> 1;
        middle1 = (x[1] + x[3]) >> 1;
        /* Rows are x[2 * i], x[2 * i + 1]. `i << 1` keeps the counter (uopt strength-reduces `i * 2`), and
         * plain indexing instead of row pointers keeps uopt's no-alias marks for the x and y walks of the
         * rotated strips below (T-25102). */
        for (i = 0; i < 2; i++) {
            first_y = y[i << 1];
            if (first_y > 0 || y[(i << 1) + 1] > 0) {
                if (middle0 < x[i << 1]) {
                    x[i << 1] = x[i << 1] + first_y + middle0;
                }
                if (x[i << 1] < middle0) {
                    x[i << 1] = x[i << 1] - first_y + middle0;
                }
                if (middle1 < x[(i << 1) + 1]) {
                    x[(i << 1) + 1] = y[(i << 1) + 1] + x[(i << 1) + 1] + middle1;
                }
                if (x[(i << 1) + 1] < middle1) {
                    x[(i << 1) + 1] = x[(i << 1) + 1] - y[(i << 1) + 1] + middle1;
                }
            }
        }
        for (i = 0; i < 4; i++) {
            x[i] = -x[i];
        }
        for (i = 0; i < 4; i++) {
            y[i] -= 24;
        }
        ball_x = (s16)D_80149968.screen_coordinates[0] >> 2;
        for (i = 0; i < 4; i++) {
            x[i] -= ball_x;
        }
        for (i = 0; i != 2; i++) {
            if ((x[i << 1] >= -160 && x[i << 1] <= 160 && y[i << 1] >= -23 && y[i << 1] < 121) ||
                (x[(i << 1) + 1] >= -160 && x[(i << 1) + 1] <= 160 &&
                 y[(i << 1) + 1] >= -23 && y[(i << 1) + 1] < 121)) {
                prim_gradient_line((s16)x[i << 1], (s16)y[i << 1], (s16)x[(i << 1) + 1], (s16)y[(i << 1) + 1],
                              0xFFFFFF, 0xFFFFFF, 250);
            }
        }
    }
    /* Rotated strips. Rot_2D is called without a prototype (implicit int), as in
     * sports_candidate_rotated_floor_strip_draw below: with the void Rot_2D the registers of the loops differ (T-25102). */
    for (angle = 0; angle < 64; angle++) {
        input_x = -320;
        input_y = 0;
        Rot_2D(angle * 16, &input_x, &input_y, &x[0], &y[0]);
        Rot_2D((angle + 1) * 16, &input_x, &input_y, &x[1], &y[1]);
        input_x = -240;
        input_y = 0;
        Rot_2D(angle * 16, &input_x, &input_y, &x[2], &y[2]);
        Rot_2D((angle + 1) * 16, &input_x, &input_y, &x[3], &y[3]);
        camera_x = ((TaiikuCamera0C *)overlay_scratch)->x;
        camera_y = ((TaiikuCamera0C *)overlay_scratch)->y;
        for (i = 0; i < 4; i++) {
            x[i] += camera_x;
            y[i] += camera_y;
        }
        for (i = 0; i < 4; i++) {
            scale = func_800A0140(0x2AA);
            y[i] = (y[i] * scale) >> 12;
        }
        middle0 = (x[0] + x[2]) >> 1;
        middle1 = (x[1] + x[3]) >> 1;
        for (i = 0; i < 2; i++) {
            first_y = y[i << 1];
            if (first_y > 0 || y[(i << 1) + 1] > 0) {
                if (middle0 < x[i << 1]) {
                    x[i << 1] = x[i << 1] + first_y + middle0;
                }
                if (x[i << 1] < middle0) {
                    x[i << 1] = x[i << 1] - first_y + middle0;
                }
                if (middle1 < x[(i << 1) + 1]) {
                    x[(i << 1) + 1] = y[(i << 1) + 1] + x[(i << 1) + 1] + middle1;
                }
                if (x[(i << 1) + 1] < middle1) {
                    x[(i << 1) + 1] = x[(i << 1) + 1] - y[(i << 1) + 1] + middle1;
                }
            }
        }
        for (i = 0; i < 4; i++) {
            x[i] = -x[i];
        }
        for (i = 0; i < 4; i++) {
            y[i] -= 24;
        }
        ball_x = (s16)D_80149968.screen_coordinates[0] >> 2;
        for (i = 0; i < 4; i++) {
            x[i] -= ball_x;
        }
        for (i = 0; i != 2; i++) {
            if ((x[i << 1] >= -160 && x[i << 1] <= 160 && y[i << 1] >= -23 && y[i << 1] < 121) ||
                (x[(i << 1) + 1] >= -160 && x[(i << 1) + 1] <= 160 &&
                 y[(i << 1) + 1] >= -23 && y[(i << 1) + 1] < 121)) {
                prim_gradient_line((s16)x[i << 1], (s16)y[i << 1], (s16)x[(i << 1) + 1], (s16)y[(i << 1) + 1],
                              0xFFFFFF, 0xFFFFFF, 250);
            }
        }
    }
}

void sports_candidate_rotated_floor_strip_draw(void) {
    s32 i;
    s32 middle0;
    s32 middle1;
    s32 x[4];
    s32 y[4];
    s32 scale;
    s32 input_x;
    s32 input_y;
    s32 camera_x;
    s32 camera_y;
    s32 *xp;
    s32 *yp;
    s32 ball_x;
    s32 angle;
    s32 first_y;

    angle = 0;
    do {
        /* Rot_2D is called without a prototype (implicit int, as in sports_candidate_motion_velocity_set's
         * caller above): the int result makes uopt treat $v0 as written by the call, which keeps
         * the x pointer of the loops below out of $v0 (T-25102). */
        input_x = -320;
        input_y = 0;
        Rot_2D(((TaiikuCamera0C *)overlay_scratch)->angle + angle, &input_x, &input_y, &x[0], &y[0]);
        Rot_2D(((TaiikuCamera0C *)overlay_scratch)->angle + angle + 16, &input_x, &input_y, &x[1], &y[1]);
        input_x = -240;
        input_y = 0;
        Rot_2D(((TaiikuCamera0C *)overlay_scratch)->angle + angle, &input_x, &input_y, &x[2], &y[2]);
        Rot_2D(((TaiikuCamera0C *)overlay_scratch)->angle + angle + 16, &input_x, &input_y, &x[3], &y[3]);
        camera_x = ((TaiikuCamera0C *)overlay_scratch)->x;
        camera_y = ((TaiikuCamera0C *)overlay_scratch)->y;
        for (i = 0; i < 4; i++) {
            x[i] += camera_x;
            y[i] += camera_y;
        }
        for (i = 0; i < 4; i++) {
            scale = func_800A0140(0x2AA);
            y[i] = (y[i] * scale) >> 12;
        }
        middle0 = (x[0] + x[2]) >> 1;
        middle1 = (x[1] + x[3]) >> 1;
        /* Rows are x[2 * i], x[2 * i + 1]. `i << 1` keeps the counter: uopt strength-reduces
         * `i * 2` into a second induction variable, the original recomputes the row offset (T-25102). */
        for (i = 0; i < 2; i++) {
            xp = x + (i << 1);
            yp = y + (i << 1);
            first_y = yp[0];
            if (first_y > 0 || yp[1] > 0) {
                if (middle0 < xp[0]) {
                    xp[0] = xp[0] + first_y + middle0;
                }
                if (xp[0] < middle0) {
                    xp[0] = xp[0] - first_y + middle0;
                }
                if (middle1 < xp[1]) {
                    xp[1] = yp[1] + xp[1] + middle1; /* operand order sets the addu order (T-25102) */
                }
                if (xp[1] < middle1) {
                    xp[1] = xp[1] - yp[1] + middle1;
                }
            }
        }
        for (i = 0; i < 4; i++) {
            x[i] = -x[i];
        }
        for (i = 0; i < 4; i++) {
            y[i] -= 24;
        }
        ball_x = (s16)D_80149968.screen_coordinates[0] >> 2;
        for (i = 0; i < 4; i++) {
            x[i] -= ball_x;
        }
        for (i = 0; i != 2; i++) {
            xp = x + (i << 1);
            yp = y + (i << 1);
            if ((xp[0] >= -160 && xp[0] <= 160 && yp[0] >= -23 && yp[0] < 121) ||
                (xp[1] >= -160 && xp[1] <= 160 && yp[1] >= -23 && yp[1] < 121)) {
                prim_gradient_line((s16)xp[0], (s16)yp[0], (s16)xp[1], (s16)yp[1], 0xFFFFFF, 0xFFFFFF, 250);
            }
        }
        angle += 16;
    } while (angle != 0x800);
}

void sports_candidate_floor_transition_draw(void) {
    s32 i;
    s32 middle0;
    s32 middle1;
    s32 x[4];
    s32 y[4];
    s32 scale;
    s32 input_x;
    s32 input_y;
    s32 camera_x;
    s32 camera_y;
    s32 ball_x;
    s32 angle;
    s32 first_y;

    /* Straight strips, each corner turned by the camera angle. */
    for (angle = 0; angle < 64; angle++) {
        x[1] = x[0] = 320;
        x[3] = x[2] = 240;
        y[0] = angle << 3;
        y[1] = (angle + 1) << 3;
        /* Copies, not chained stores: `y[2] = y[0] = angle << 3` reserves one more spill slot, and the counter
         * spill lands at 0x74 instead of 0x78 (T-25102). */
        y[2] = y[0];
        y[3] = y[1];
        for (i = 0; i < 4; i++) {
            input_x = x[i];
            input_y = y[i];
            /* Rot_2D is called without a prototype (implicit int), as in sports_candidate_rotated_floor_strip_draw. */
            Rot_2D(((TaiikuCamera0C *)overlay_scratch)->angle, &input_x, &input_y, &x[i], &y[i]);
        }
        camera_x = ((TaiikuCamera0C *)overlay_scratch)->x;
        camera_y = ((TaiikuCamera0C *)overlay_scratch)->y;
        for (i = 0; i < 4; i++) {
            x[i] += camera_x;
            y[i] += camera_y;
        }
        for (i = 0; i < 4; i++) {
            scale = func_800A0140(0x2AA);
            y[i] = (y[i] * scale) >> 12;
        }
        middle0 = (x[0] + x[2]) >> 1;
        middle1 = (x[1] + x[3]) >> 1;
        for (i = 0; i < 2; i++) {
            first_y = y[i << 1];
            if (first_y > 0 || y[(i << 1) + 1] > 0) {
                if (middle0 < x[i << 1]) {
                    x[i << 1] = x[i << 1] + first_y + middle0;
                }
                if (x[i << 1] < middle0) {
                    x[i << 1] = x[i << 1] - first_y + middle0;
                }
                if (middle1 < x[(i << 1) + 1]) {
                    x[(i << 1) + 1] = y[(i << 1) + 1] + x[(i << 1) + 1] + middle1;
                }
                if (x[(i << 1) + 1] < middle1) {
                    x[(i << 1) + 1] = x[(i << 1) + 1] - y[(i << 1) + 1] + middle1;
                }
            }
        }
        for (i = 0; i < 4; i++) {
            x[i] = -x[i];
        }
        for (i = 0; i < 4; i++) {
            y[i] -= 24;
        }
        ball_x = (s16)D_80149968.screen_coordinates[0] >> 2;
        for (i = 0; i < 4; i++) {
            x[i] -= ball_x;
        }
        for (i = 0; i != 2; i++) {
            if ((x[i << 1] >= -160 && x[i << 1] <= 160 && y[i << 1] >= -23 && y[i << 1] < 121) ||
                (x[(i << 1) + 1] >= -160 && x[(i << 1) + 1] <= 160 &&
                 y[(i << 1) + 1] >= -23 && y[(i << 1) + 1] < 121)) {
                prim_gradient_line((s16)x[i << 1], (s16)y[i << 1], (s16)x[(i << 1) + 1], (s16)y[(i << 1) + 1],
                              0xFFFFFF, 0xFFFFFF, 250);
            }
        }
    }
    ((TaiikuActor44 *)actor_pool)[102].unk_26 = x[3] + 8;
    /* Rotated strips. `angle * 16` with `angle++` lets uopt strength-reduce the counter into one spilled
     * value at 0x78, the slot of the first phase, ending with `bne v0,0x400`. The second corner adds 16 to
     * that sum; `(angle + 1) * 16` would be a second strength-reduced value (T-25102). */
    for (angle = 0; angle < 64; angle++) {
        input_x = 0;
        input_y = -320;
        Rot_2D(((TaiikuCamera0C *)overlay_scratch)->angle + angle * 16, &input_x, &input_y, &x[0], &y[0]);
        Rot_2D(((TaiikuCamera0C *)overlay_scratch)->angle + angle * 16 + 16, &input_x, &input_y, &x[1], &y[1]);
        input_x = 0;
        input_y = -240;
        Rot_2D(((TaiikuCamera0C *)overlay_scratch)->angle + angle * 16, &input_x, &input_y, &x[2], &y[2]);
        Rot_2D(((TaiikuCamera0C *)overlay_scratch)->angle + angle * 16 + 16, &input_x, &input_y, &x[3], &y[3]);
        camera_x = ((TaiikuCamera0C *)overlay_scratch)->x;
        camera_y = ((TaiikuCamera0C *)overlay_scratch)->y;
        for (i = 0; i < 4; i++) {
            x[i] += camera_x;
            y[i] += camera_y;
        }
        for (i = 0; i < 4; i++) {
            scale = func_800A0140(0x2AA);
            y[i] = (y[i] * scale) >> 12;
        }
        middle0 = (x[0] + x[2]) >> 1;
        middle1 = (x[1] + x[3]) >> 1;
        for (i = 0; i < 2; i++) {
            first_y = y[i << 1];
            if (first_y > 0 || y[(i << 1) + 1] > 0) {
                if (middle0 < x[i << 1]) {
                    x[i << 1] = x[i << 1] + first_y + middle0;
                }
                if (x[i << 1] < middle0) {
                    x[i << 1] = x[i << 1] - first_y + middle0;
                }
                if (middle1 < x[(i << 1) + 1]) {
                    x[(i << 1) + 1] = y[(i << 1) + 1] + x[(i << 1) + 1] + middle1;
                }
                if (x[(i << 1) + 1] < middle1) {
                    x[(i << 1) + 1] = x[(i << 1) + 1] - y[(i << 1) + 1] + middle1;
                }
            }
        }
        for (i = 0; i < 4; i++) {
            x[i] = -x[i];
        }
        for (i = 0; i < 4; i++) {
            y[i] -= 24;
        }
        ball_x = (s16)D_80149968.screen_coordinates[0] >> 2;
        for (i = 0; i < 4; i++) {
            x[i] -= ball_x;
        }
        for (i = 0; i != 2; i++) {
            if ((x[i << 1] >= -160 && x[i << 1] <= 160 && y[i << 1] >= -23 && y[i << 1] < 121) ||
                (x[(i << 1) + 1] >= -160 && x[(i << 1) + 1] <= 160 &&
                 y[(i << 1) + 1] >= -23 && y[(i << 1) + 1] < 121)) {
                prim_gradient_line((s16)x[i << 1], (s16)y[i << 1], (s16)x[(i << 1) + 1], (s16)y[(i << 1) + 1],
                              0xFFFFFF, 0xFFFFFF, 250);
            }
        }
    }
}

void sports_candidate_rival_display_update(void) {
    s32 runner_index;

    runner_index = 0;
    do {
        sports_candidate_runner_display_position_update(runner_index);
        runner_index += 1;
    } while (runner_index != 4);
    sports_candidate_runner_blink_animation_update();
}

/* Positions display records 104 + arg0 and 108 + arg0 from the runner's state (D_80122EC4 for arg0 0, else
 * sports_candidate_runner_state_select) and phase (sports_candidate_player_position.halves.high, else sports_candidate_runner_phase_select). */
void sports_candidate_runner_display_position_update(s32 runner_index) {
    s16 phase;
    s32 state;
    s32 t;
    s16 x;

    if (runner_index == 0) {
        state = D_80122EC4;
        phase = sports_candidate_player_position.halves.high;
        if (sports_candidate_input_and_boundary_state.boundary_side != 0) {
            ((Actor *)actor_pool)[104].draw_flags |= 0x40;
            ((Actor *)actor_pool)[108].draw_flags |= 0x40;
        } else {
            ((Actor *)actor_pool)[104].draw_flags &= ~0x40;
            ((Actor *)actor_pool)[108].draw_flags &= ~0x40;
        }
    } else {
        if (sports_candidate_activity_flags[runner_index - 1] == 1) {
            ((Actor *)actor_pool)[runner_index + 104].draw_flags |= 0x40;
            ((Actor *)actor_pool)[runner_index + 108].draw_flags |= 0x40;
        } else {
            ((Actor *)actor_pool)[runner_index + 104].draw_flags &= ~0x40;
            ((Actor *)actor_pool)[runner_index + 108].draw_flags &= ~0x40;
        }
        state = sports_candidate_runner_state_select(runner_index);
        phase = sports_candidate_runner_phase_select(runner_index);
    }
    switch (state) {
    case 0:
    case 6:
        ((Actor *)actor_pool)[runner_index + 104].x = (phase >> 3) + (phase >> 6) - 0x48;
        ((Actor *)actor_pool)[runner_index + 108].x = ((Actor *)actor_pool)[runner_index + 104].x + 0x10;
        break;
    case 1:
        t = phase - 0x200;
        if (t < 0x80) {
            ((Actor *)actor_pool)[runner_index + 104].x = (t >> 2) + 0x48;
            ((Actor *)actor_pool)[runner_index + 104].y = -0x48 - (t >> 4);
            ((Actor *)actor_pool)[runner_index + 104].y = ((Actor *)actor_pool)[runner_index + 104].y - runner_index + 3;
        } else {
            t = phase - 0x280;
            ((Actor *)actor_pool)[runner_index + 104].x = (t >> 3) + 0x68;
            ((Actor *)actor_pool)[runner_index + 104].y = -0x50 - (t >> 3);
            ((Actor *)actor_pool)[runner_index + 104].y = ((Actor *)actor_pool)[runner_index + 104].y - runner_index + 3;
        }
        x = ((Actor *)actor_pool)[runner_index + 104].x;
        if (x >= 0x68 && ((Actor *)actor_pool)[runner_index + 104].unk_18 < 0x8E) {
            if (runner_index == 0) {
                ((Actor *)actor_pool)[104].unk_18 = 0x90;
            } else {
                ((Actor *)actor_pool)[runner_index + 104].unk_18 = 0x8E;
            }
            ((Actor *)actor_pool)[runner_index + 104].draw_flags = 0x84;
        }
        if (x < 0x68) {
            ((Actor *)actor_pool)[runner_index + 108].x = x + 0x10;
            ((Actor *)actor_pool)[runner_index + 108].y = ((Actor *)actor_pool)[runner_index + 104].y;
        } else {
            ((Actor *)actor_pool)[runner_index + 108].x = x;
            ((Actor *)actor_pool)[runner_index + 108].y = ((Actor *)actor_pool)[runner_index + 104].y - 8;
        }
        break;
    case 2:
        if (phase < 0x380) {
            t = phase - 0x300;
            ((Actor *)actor_pool)[runner_index + 104].x = 0x78 - (t >> 4);
            ((Actor *)actor_pool)[runner_index + 104].y = -0x60 - (t >> 4);
            ((Actor *)actor_pool)[runner_index + 104].y = ((Actor *)actor_pool)[runner_index + 104].y - runner_index + 3;
        } else {
            t = phase - 0x380;
            ((Actor *)actor_pool)[runner_index + 104].x = 0x68 - (t >> 2);
            ((Actor *)actor_pool)[runner_index + 104].y = -0x68 - (t >> 4);
            ((Actor *)actor_pool)[runner_index + 104].y = ((Actor *)actor_pool)[runner_index + 104].y - runner_index + 3;
        }
        x = ((Actor *)actor_pool)[runner_index + 104].x;
        if (x < 0x69 && ((Actor *)actor_pool)[runner_index + 104].unk_18 >= 0x8E) {
            if (runner_index == 0) {
                ((Actor *)actor_pool)[104].unk_18 = 0x8A;
            } else {
                ((Actor *)actor_pool)[runner_index + 104].unk_18 = 0x86;
            }
            ((Actor *)actor_pool)[runner_index + 104].draw_flags = 0x84;
        }
        if (x < 0x68) {
            ((Actor *)actor_pool)[runner_index + 108].x = x - 0x10;
            ((Actor *)actor_pool)[runner_index + 108].y = ((Actor *)actor_pool)[runner_index + 104].y;
        } else {
            ((Actor *)actor_pool)[runner_index + 108].x = x;
            ((Actor *)actor_pool)[runner_index + 108].y = ((Actor *)actor_pool)[runner_index + 104].y - 8;
        }
        break;
    case 3:
        ((Actor *)actor_pool)[runner_index + 104].x = -(phase >> 3) - (phase >> 6) + 0x48;
        ((Actor *)actor_pool)[runner_index + 108].x = ((Actor *)actor_pool)[runner_index + 104].x - 0x10;
        ((Actor *)actor_pool)[runner_index + 108].y = ((Actor *)actor_pool)[runner_index + 104].y;
        break;
    case 4:
        t = phase - 0x200;
        if (t < 0x80) {
            ((Actor *)actor_pool)[runner_index + 104].x = -0x48 - (t >> 2);
            ((Actor *)actor_pool)[runner_index + 104].y = (t >> 4) - 0x70;
            ((Actor *)actor_pool)[runner_index + 104].y = ((Actor *)actor_pool)[runner_index + 104].y - runner_index + 3;
        } else {
            t = phase - 0x280;
            ((Actor *)actor_pool)[runner_index + 104].x = -0x68 - (t >> 3);
            ((Actor *)actor_pool)[runner_index + 104].y = (t >> 3) - 0x68;
            ((Actor *)actor_pool)[runner_index + 104].y = ((Actor *)actor_pool)[runner_index + 104].y - runner_index + 3;
        }
        x = ((Actor *)actor_pool)[runner_index + 104].x;
        if (x < -0x68 && ((Actor *)actor_pool)[runner_index + 104].draw_flags != 0) {
            ((Actor *)actor_pool)[runner_index + 104].draw_flags = 0;
        }
        if (x < -0x68) {
            ((Actor *)actor_pool)[runner_index + 108].x = x;
            ((Actor *)actor_pool)[runner_index + 108].y = ((Actor *)actor_pool)[runner_index + 104].y + 8;
        } else {
            ((Actor *)actor_pool)[runner_index + 108].x = x - 0x10;
            ((Actor *)actor_pool)[runner_index + 108].y = ((Actor *)actor_pool)[runner_index + 104].y;
        }
        break;
    case 5:
        t = phase - 0x300;
        if (t < 0x80) {
            ((Actor *)actor_pool)[runner_index + 104].x = (t >> 3) - 0x78;
            ((Actor *)actor_pool)[runner_index + 104].y = (t >> 4) - 0x58;
            ((Actor *)actor_pool)[runner_index + 104].y = ((Actor *)actor_pool)[runner_index + 104].y - runner_index + 3;
        } else {
            t = phase - 0x380;
            ((Actor *)actor_pool)[runner_index + 104].x = (t >> 2) - 0x68;
            ((Actor *)actor_pool)[runner_index + 104].y = (t >> 4) - 0x50;
            ((Actor *)actor_pool)[runner_index + 104].y = ((Actor *)actor_pool)[runner_index + 104].y - runner_index + 3;
        }
        x = ((Actor *)actor_pool)[runner_index + 104].x;
        if (x >= -0x67 && ((Actor *)actor_pool)[runner_index + 104].draw_flags == 0) {
            ((Actor *)actor_pool)[runner_index + 104].draw_flags = 0x85;
            if (runner_index == 0) {
                ((Actor *)actor_pool)[104].unk_18 = 0x8A;
            } else {
                ((Actor *)actor_pool)[runner_index + 104].unk_18 = 0x86;
            }
        }
        if (x < -0x68) {
            ((Actor *)actor_pool)[runner_index + 108].x = x;
            ((Actor *)actor_pool)[runner_index + 108].y = ((Actor *)actor_pool)[runner_index + 104].y + 8;
        } else {
            ((Actor *)actor_pool)[runner_index + 108].x = x + 0x10;
            ((Actor *)actor_pool)[runner_index + 108].y = ((Actor *)actor_pool)[runner_index + 104].y;
        }
        break;
    }
}

void sports_candidate_runner_blink_animation_update(void) {
    s32 i;

    if (D_801499DC >= 31) {
        D_801499DC = 0;
        for (i = 0; i < 4; i++) {
            if (i == 0) {
                if (sports_candidate_input_and_boundary_state.boundary_side == 0) {
                    if (((Actor *)actor_pool)[104].unk_18 >= 0x90) {
                        ((Actor *)actor_pool)[104].unk_18 ^= 1;
                    } else if (((Actor *)actor_pool)[104].unk_18 == 0x8D) {
                        ((Actor *)actor_pool)[104].unk_18 = 0x8A;
                    } else {
                        ((Actor *)actor_pool)[104].unk_18++;
                    }
                }
            } else if (sports_candidate_activity_flags[i - 1] == 0) {
                if (((Actor *)actor_pool)[104 + i].unk_18 >= 0x8E) {
                    ((Actor *)actor_pool)[104 + i].unk_18 ^= 1;
                } else if (((Actor *)actor_pool)[104 + i].unk_18 == 0x89) {
                    ((Actor *)actor_pool)[104 + i].unk_18 = 0x86;
                } else {
                    ((Actor *)actor_pool)[104 + i].unk_18++;
                }
            }
        }
    }
}

s32 sports_candidate_runner_state_select(s32 runner_index) {
    s16 runner_progress;

    runner_progress = sports_candidate_player_progress_record[runner_index].val;
    if (runner_progress < 0x400) {
        return 0;
    }
    if (runner_progress < 0x500) {
        return 1;
    }
    if (runner_progress < 0x600) {
        return 2;
    }
    if (runner_progress < 0xA00) {
        return 3;
    }
    if (runner_progress < 0xB00) {
        return 4;
    }
    if (runner_progress < 0xC00) {
        return 5;
    }
    return 6;
}

s16 sports_candidate_runner_phase_select(s32 runner_index) {
    s16 runner_progress;

    runner_progress = sports_candidate_player_progress_record[runner_index].val;
    if (runner_progress < 0x400) {
        return runner_progress;
    }
    if (runner_progress < 0x500) {
        return (s16) (runner_progress - 0x200);
    }
    if (runner_progress < 0x600) {
        return (s16) (runner_progress - 0x200);
    }
    if (runner_progress < 0xA00) {
        return (s16) (runner_progress - 0x600);
    }
    if (runner_progress < 0xB00) {
        return (s16) (runner_progress - 0x800);
    }
    if (runner_progress < 0xC00) {
        return (s16) (runner_progress - 0x800);
    }
    return (s16) (runner_progress - 0xC00);
}

void sports_candidate_scene_progress_update(void) {
    s32 candidate_index;
    s32 result_status_slot_index;
    TaiikuI3 candidate_metric_deltas;

    candidate_metric_deltas = sports_candidate_progress_deltas;
    for (candidate_index = 0; candidate_index < 3; candidate_index++) {
        if (sports_candidate_cpu_progress_records[candidate_index].halves.high < 0xE00) {
            if (sports_candidate_activity_flags[candidate_index] == 0) {
                sports_candidate_cpu_progress_records[candidate_index].word += candidate_metric_deltas.v[candidate_index];
            }
        } else if (sports_candidate_activity_frames[candidate_index] != 0xFFFF) {
            sports_candidate_activity_frames[candidate_index] = 0xFFFF;
            for (result_status_slot_index = 0; sports_candidate_result_order[result_status_slot_index] != 0xFF; result_status_slot_index++) {
            }
            sports_candidate_result_order[result_status_slot_index] = candidate_index + 1;
        }
        if ((sports_candidate_runner_state_select(candidate_index + 1) == 1 || sports_candidate_runner_state_select(candidate_index + 1) == 2 || sports_candidate_runner_state_select(candidate_index + 1) == 4 ||
             sports_candidate_runner_state_select(candidate_index + 1) == 5) && !(rand() & 0xFFF)) {
            sports_candidate_activity_flags[candidate_index] = 1;
        }
        if (sports_candidate_activity_flags[candidate_index] == 1) {
            sports_candidate_activity_frames[candidate_index]++;
            if (sports_candidate_activity_frames[candidate_index] > 240) {
                sports_candidate_activity_flags[candidate_index] = 0;
                sports_candidate_activity_frames[candidate_index] = 0;
            }
        }
    }
}

s8 sports_girl_scene_result_candidate_select(void) {
    s32 result_candidate_index;
    s16 result_candidate_metric_values[3];

    for (result_candidate_index = 0; result_candidate_index < 3; result_candidate_index++) {
        if (sports_candidate_cpu_progress_records[result_candidate_index].halves.high < 0xE00) {
            result_candidate_metric_values[result_candidate_index] = sports_candidate_cpu_progress_records[result_candidate_index].halves.high;
        } else {
            result_candidate_metric_values[result_candidate_index] = -1;
        }
    }
    if ((result_candidate_metric_values[0] >= result_candidate_metric_values[1]) && (result_candidate_metric_values[0] >= result_candidate_metric_values[2])) {
        sports_candidate_cpu_progress_records[0].halves.high = -1;
        return 1;
    }
    if (result_candidate_metric_values[1] >= result_candidate_metric_values[2]) {
        sports_candidate_cpu_progress_records[1].halves.high = -1;
        return 2;
    }
    sports_candidate_cpu_progress_records[2].halves.high = -1;
    return 3;
}

void sports_candidate_model_transform_update(void) {
    MATRIX m1;
    MATRIX m2;
    TaiikuSVec rot;
    VECTOR scale;

    m1 = *(MATRIX *)view_base_matrix;
    rot.vx = 0;
    rot.vz = 0;
    rot.vy = D_8014995A;
    RotMatrix(&rot, &m1);
    m2 = *(MATRIX *)view_base_matrix;
    rot.vy = 0;
    rot.vz = 0;
    rot.vx = D_80149958;
    RotMatrix(&rot, &m2);
    MulMatrix(&m1, &m2);
    scale.vx = D_80149954;
    scale.vy = D_80149954;
    scale.vz = D_80149954;
    ScaleMatrix(&m2, &scale);
    MulMatrix(&m1, &m2);
    m1.t[0] = D_80149968.screen_coordinates[0];
    m1.t[1] = D_80149968.screen_coordinates[1];
    m1.t[2] = 0x1B58;
    *(MATRIX *)D_801227A4 = m1;
    view_coordinates->flag = 0;
    D_80149954 -= 0x80;
    if ((D_80149968.screen_coordinates[1] >> 3) >= -0x3F) {
        D_80149968.screen_coordinates[1] -= 8;
    }
}

void sports_girl_scene_result_actor_setup(void) {
    s32 result_actor_or_slot_index;

    for (result_actor_or_slot_index = 0; result_actor_or_slot_index < 8; result_actor_or_slot_index++) {
        ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_03 = 0xA4;
        ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_02 = 0x40;
        ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_05 = 0x80;
        ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_01 = 3;
        ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_38 = 0x1000000;
        ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_07 = 0x80;
        ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_0C = (u8 *)D_80149FE4;
        ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_10 = (u8 *)D_8014A01C;
        ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_34 = (u8 *)D_80149DF4;
        ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_14 = D_8014A054;
        ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_1A = 0x1000;
        ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_1C = 0x1000;
        ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_06 = 2;
    }
    for (result_actor_or_slot_index = 0; result_actor_or_slot_index < 4; result_actor_or_slot_index++) {
        ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_26 = result_actor_or_slot_index * 0x40 - 0x78;
        ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_2A = -8;
        switch (sports_candidate_result_order[result_actor_or_slot_index]) {
        case 0:
            ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_18 = D_80149940 * 7;
            break;
        case 1:
            ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_18 = 0x4D;
            ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_43 = 0x1C;
            break;
        case 2:
            ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_18 = 0x4D;
            ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_43 = 0x1D;
            break;
        case 3:
            ((TaiikuActor44 *)actor_pool)[96 + result_actor_or_slot_index].unk_18 = 0x4D;
            break;
        default:
            game_mode_reset_to_title();
            break;
        }
    }
    for (result_actor_or_slot_index = 0; result_actor_or_slot_index < 4; result_actor_or_slot_index++) {
        ((TaiikuActor44 *)actor_pool)[100 + result_actor_or_slot_index].unk_26 = result_actor_or_slot_index * 0x40 - 0x78;
        ((TaiikuActor44 *)actor_pool)[100 + result_actor_or_slot_index].unk_2A = -8;
        ((TaiikuActor44 *)actor_pool)[100 + result_actor_or_slot_index].unk_18 = 0x58;
        switch (sports_candidate_result_order[result_actor_or_slot_index]) {
        case 0:
            break;
        case 1:
            ((TaiikuActor44 *)actor_pool)[100 + result_actor_or_slot_index].unk_43 = 0x1C;
            break;
        case 2:
            ((TaiikuActor44 *)actor_pool)[100 + result_actor_or_slot_index].unk_43 = 0x1D;
            break;
        case 3:
            ((TaiikuActor44 *)actor_pool)[100 + result_actor_or_slot_index].unk_43 = 0x1E;
            break;
        }
    }
}

void sports_girl_result_actor_motion_update(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        ((Actor *)actor_pool)[i + 100].unk_18 -= 4 - i;
    }
}

void sports_girl_result_actor_sequence_advance(void) {
    s32 i;
    TkS16x4 t;

    t = D_80149F8C;
    for (i = 0; i < 4; i++) {
        ((Actor *)actor_pool)[i + 96].unk_18 += t.v[i];
    }
}

void sports_girl_result_animation_toggle(void) {
    if ((u32)sports_candidate_transition_wait_frames >= 0xF) {
        *(s16 *)(actor_pool + 0x19DC) ^= 1;
        *(s16 *)(actor_pool + 0x1A20) ^= 1;
        sports_candidate_transition_wait_frames = 0;
    }
}

void sports_candidate_model_lighting_init(void) {
    func_8009AD30(0x3E8);
    D_80122740 = 0;
    D_80122744 = 0;
    D_80122748 = -0x3E8;
    D_8012274C = 0;
    D_80122750 = 0;
    D_80122754 = 0;
    D_80122758 = 0;
    D_8012275C = 0;
    func_80099540(&D_80122740);
    func_8009AD50(0);
    func_8009AD60(0x10000);
}

void sports_candidate_scene_lighting_init(void) {
    view_light_0 = 0x64;
    D_80122764 = 0x64;
    D_80122768 = 0x320;
    D_8012276C = 0xD0;
    D_8012276D = 0xD0;
    D_8012276E = 0xD0;
    func_8009AD70(0, &view_light_0);
    view_light_1 = 0x64;
    D_80122774 = 0x64;
    D_80122778 = 0x64;
    D_8012277C = 0xF0;
    D_8012277D = 0;
    D_8012277E = 0;
    func_8009AD70(1, &view_light_1);
    view_light_2 = 0x64;
    D_80122784 = 0x64;
    D_80122788 = 0x64;
    D_8012278C = 0;
    D_8012278D = 0;
    D_8012278E = 0xF0;
    func_8009AD70(2, &view_light_2);
    GsSetAmbient(0x800, 0x800, 0x800);
    D_80122790 = -0xF00;
    D_80122794 = 0x01400000;
    D_80122798 = 1;
    D_80122799 = 1;
    D_8012279A = 1;
    sports_scene_display_parameters_set(&D_80122790);
    func_8009B340(1);
}

void sports_candidate_model_slots_init(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        func_80099E30(0, &view_coordinates[i]);
        view_coordinates[i].translate_z = -4000;
        view_coordinates[i].flag = 0;
    }
}

void sports_candidate_model_draw_packets_init(s32 *arg0) {
    s32 *dop;
    s32 i;

    dop = arg0;
    dop++;
    func_8009B3C0((s32)dop);
    dop++;
    dop++;
    for (i = 0; i < 16; i++) {
        func_8009B430((s32)dop, (u8 *)&model_objects[i * 4], 0);
        model_objects[i * 4 + 1] = (u32)&view_coordinates[i];
        model_objects[i * 4] = 0x80000000;
    }
}

void sports_candidate_model_draw_packet_bind(s32 arg0, s32 arg1) {
    u8 *p;
    s32 sp30;

    sp30 = arg0 + 4;
    func_8009B3C0(sp30);
    p = (u8 *)model_objects + arg1 * 0x10;
    func_8009B430(sp30 + 8, p, 0, sp30);
    *(u8 **)((u8 *)model_objects + arg1 * 0x10 + 4) = (u8 *)&view_coordinates[arg1];
    *(s32 *)((u8 *)model_objects + arg1 * 0x10) = 0;
}

void sports_candidate_model_resource_addresses_init(void) {
    D_80149FA0 = 0x8019D000;
    D_80149FA4 = 0x8018F000;
    D_80149FA8 = 0x8019E218;
    D_80149FAC = 0x8019D008;
}
