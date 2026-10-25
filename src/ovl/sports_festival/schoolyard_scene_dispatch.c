/* schoolyard_scene_dispatch.c: ; src/ovl/TAIIKU/80142240.c:5-10 dispatches the schoolyard-scene stage table. Original object 0x80142240-0x80145210 (config/objects/TAIIKU.txt). */
#define MAIN_API_OVERRIDE_scene_screen_reset /* called as an implicit int function (definition: void): the call's $v0 result keeps the loop pointers off $v0 in sports_schoolyard_scene_resource_setup, T-22004 */
#include "common.h"
#include "ovl/sports_festival.h"
#define DISP(n) (((Actor *)actor_pool)[n])
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", sports_schoolyard_result_selector);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", sports_schoolyard_speed_update_frames);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", sports_schoolyard_transition_wait_frames);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", sports_schoolyard_result_status);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", sports_schoolyard_animation_interval);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", sports_schoolyard_input_speed);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", sports_schoolyard_motion_limit);
/* Defined in the unit (not island pieces) so that the three stores in sports_schoolyard_controller_state_init share one lui at; T-24100. */
s32 D_8014A130[3] = { 0 };
SportsRunnerB sports_controller_c_runners[4] = { 0 };
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A1CC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", sports_schoolyard_scroll_velocity);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A1D4);
/* Defined in the unit (not island pieces) so that the first two stores share one lui at; T-16551, sports_runner_race_resource_setup, T-22004, sports_schoolyard_scene_resource_setup. */
s16 D_8014A1E8[10] = { 0 };
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A1FC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A208);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A228);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A254);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A27C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A290);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A2A4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A2C4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A2F0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A304);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A318);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A320);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", sports_schoolyard_cpu_speeds);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", sports_schoolyard_cpu_motion_limits);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", sports_schoolyard_cpu_position_adjustments);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", sports_schoolyard_cpu_animation_intervals);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", sports_schoolyard_cpu_action_thresholds);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A360);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A370);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A374);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A378);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A37C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A380);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A384);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/sports_festival/schoolyard_scene_dispatch.data", D_8014A388);

typedef struct {
    void (*f[10])();
} FnTbl10; /* size 0x28 */
extern FnTbl10 D_8014A254;

void sports_schoolyard_mode_dispatch(void) {
    s32 mode_stage_index; /* declared before schoolyard_scene_callbacks: its stack slot sits above schoolyard_scene_callbacks (T-3330) */
    FnTbl10 schoolyard_scene_callbacks;

    schoolyard_scene_callbacks = D_8014A254;
    mode_stage_index = game_state.mode_stage;
    schoolyard_scene_callbacks.f[mode_stage_index]();
}

void sports_schoolyard_scene_eligibility_gate(void) {
    if (game_state.mode_step == 0) {
        if ((girl_state_rank(1) < 2) && (((u32) (game_state.girls[1].flags.w << 0x1E) >> 0x1F) == 1) && (game_state.scene_flags[13] == 0)) {
            game_state.scene_flags[13] = 1;
            sports_schoolyard_result_selector = 1;
            audio_bank_load_start(0, 0xBF98, 0xBF79, 0xD712, 0xD6C3, 0xD6B7);
            mode_step_next();
        } else {
            sports_schoolyard_result_selector = 0;
            mode_stage_set(3);
        }
        Default_Disp();
        return;
    }
    if (audio_bank_load_poll() == 1) {
        mode_stage_next();
    }
    Default_Disp();
}

void sports_schoolyard_setup_result_route(void) {
    s32 setup_result;

    setup_result = sports_mio_ground_scene_step();
    switch (setup_result) {
    case 1:
        mode_stage_next();
        return;
    case 2:
        sports_schoolyard_result_selector = 0;
        mode_stage_next();
        /* fallthrough */
    case 0:
        return;
    }
}

/* FAKE: `pad` reserves the unused local word in the 192-byte frame while sharing `i` across case 0 and case 3
 * gives `i` a higher uopt priority than the case 3 tpage induction pointer, assigning `$s0/$s1` in the target order.
 * D_8014A1E8 is peeled by two stores because IDO unrolls a plain 0..10 loop by four. T-22004, T-24100 */
void sports_schoolyard_scene_resource_setup(void) {
    s32 resource_index;
    s32 pad;
    TaiikuHalf10 sp_a;
    TaiikuHalf10 sp_b;
    TaiWords8 sp_c;
    TaiWords11 sp_d;

    sp_a = D_8014A27C;
    sp_b = D_8014A290;
    sp_c = D_8014A2A4;
    sp_d = D_8014A2C4;
    switch (game_state.mode_step) {
    case 0:
        if (sports_schoolyard_result_selector == 1) {
            audio_command(0xBF);
        }
        audio_bank_load_start(0, 0xBF98, 0xBF79, 0xC20F, 0xC1EF, 0xD6B7);
        scene_screen_reset(1, 1);
        for (resource_index = 0; resource_index < 11; resource_index++) {
            game_state.tpage_buf[resource_index + 5] = 8;
        }
        for (resource_index = 0; resource_index < 3; resource_index++) {
            game_state.tpage_buf[resource_index + 0x25] = 8;
        }
        for (resource_index = 0; resource_index < 8; resource_index++) {
            D_8014A208[resource_index] = sp_c.v[resource_index];
        }
        for (resource_index = 0; resource_index < 11; resource_index++) {
            D_8014A228[resource_index] = sp_d.value[resource_index];
        }
        for (resource_index = 0; resource_index < 10; resource_index++) {
            *(s16 *)&D_8014A1D4[resource_index * 2] = sp_a.h[resource_index];
        }
        D_8014A1E8[0] = sp_b.h[0];
        D_8014A1E8[1] = sp_b.h[1];
        for (resource_index = 2; resource_index < 10; resource_index++) {
            D_8014A1E8[resource_index] = sp_b.h[resource_index];
        }
        actor_clear_range(0);
        hud_cursor_show(0);
        mode_step_next();
        break;
    case 1:
        if (audio_bank_load_poll() == 1) {
            cd_read_sectors(0x1E, 0x80180000, 0xBAD0);
            mode_step_next();
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x4F, 0x8018F000, 0xB861);
            Fade_Out_Init();
            mode_step_next();
        }
        break;
    case 3:
        if ((Fade_Out_Main() != 0) && (cd_read_sync_status() & 1)) {
            sports_scene_actor_resource_addresses_init();
            sports_schoolyard_resource_addresses_init();
            for (resource_index = 0; resource_index < 3; resource_index++) {
                search_load_tpage_buf_lock(((s32 *)&D_8014A1FC)[resource_index], 1);
            }
            for (resource_index = 0; resource_index < 0x10; resource_index++) {
                load_palette(D_8014A378, (resource_index + 0x10) & 0xFF, 1, 1, resource_index);
            }
            sports_schoolyard_controller_state_init();
            sports_schoolyard_sprite_resource_setup();
            sports_schoolyard_primary_actor_setup();
            cd_read_sectors(0xA1, 0x8018F000, 0xB7C0);
            mode_stage_next();
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

/* FAKE: `pad` reserves the top local word: the original keeps the two loop spills of the
 * tpage loops at 0x38 and 0x30 (sp). The u8 views of the flag sports_schoolyard_result_selector keep its lbu loads and
 * stop uopt from promoting it (as in sports_schoolyard_input_state_update; sports_schoolyard_result_selector is `u8` since T-19010). T-16554 */
void sports_schoolyard_scene_audio_sequence(void) {
    u8 pad[4];
    s32 buffer_index;

    switch (game_state.mode_step) {
    case 0:
        if (cd_read_sync_status() & 1) {
            for (buffer_index = 0; buffer_index < 8; buffer_index++) {
                game_state.tpage_buf[search_load_tpage_buf_lock(D_8014A208[buffer_index], 1)] = buffer_index + 0x80000100;
                D_8014A208[buffer_index] = buffer_index + 0x80000100;
            }
            for (buffer_index = 0; buffer_index != 0xB; buffer_index++) {
                game_state.tpage_buf[search_load_tpage_buf_lock(D_8014A228[buffer_index], 1)] = buffer_index + 0x80000200;
                D_8014A228[buffer_index] = buffer_index + 0x80000200;
            }
            mode_step_next();
        }
        break;
    case 1:
        Fade_In_Init();
        audio_bank_start(1, 3);
        mode_step_next();
        break;
    case 2:
        if (Fade_In_Main() != 0) {
            mode_step_next();
        }
        if (game_state.pad_pressed & 0xF0) {
            Fade_Out_Init();
            cd_read_sectors(0x87U, 0x8018F000, 0xB98F);
            mode_step_set(5);
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        break;
    case 3:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if (((Actor *)actor_pool)[96].y >= 0x61) {
            /* the 16.16 y position at 0x28 (its high half is unk_2A), through the fixed-point view. T-16554, T-20900 */
            ((ActorFixed *)actor_pool)[96].y.w -= 0x18000;
        } else {
            if (*(u8 *)&sports_schoolyard_result_selector == 0) {
                ((Actor *)actor_pool)[96].sequence_slot = 1;
            } else {
                ((Actor *)actor_pool)[96].sequence_slot = 3;
            }
            switch (game_state.substep) {
            case 0:
                if (*(u8 *)&sports_schoolyard_result_selector != 0) {
                    cd_audio_request_set(0x1E000050, 0x06000060, 1);
                } else {
                    cd_audio_request_set(0x1F000394, 0x440003A4, 0);
                }
                audio_command(0x300);
                game_state.substep += 1;
                break;
            case 1:
            case 3:
            case 5:
                if (cd_audio_state() == 4) {
                    game_state.substep += 1;
                }
                break;
            case 2:
                if (cd_audio_state() == 2) {
                    if (*(u8 *)&sports_schoolyard_result_selector != 0) {
                        cd_audio_request_set(0x1F00009F, 0x140000A9, 1);
                    } else {
                        cd_audio_request_set(0x1F0003A5, 0x450003B1, 0);
                    }
                    audio_command(0x300);
                    game_state.substep += 1;
                }
                break;
            case 4:
                if (cd_audio_state() == 2) {
                    if (*(u8 *)&sports_schoolyard_result_selector != 0) {
                        cd_audio_request_set(0x1F0000AA, 0x150000B7, 1);
                    } else {
                        cd_audio_request_set(0x0A0003C3, 0x430003D0, 0);
                    }
                    audio_command(0x300);
                    game_state.substep += 1;
                }
                break;
            case 6:
                if (cd_audio_state() == 2) {
                    Vblnk_Timer_Init();
                    game_state.substep += 1;
                }
                break;
            case 7:
                if (Vblnk_Timer() >= 0xB5) {
                    game_state.substep = 0;
                }
                break;
            }
        }
        if (game_state.pad_pressed & 0xF0) {
            Fade_Out_Init();
            audio_command(0x74);
            mode_step_next();
        }
        break;
    case 4:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if (cd_audio_state() == 2) {
            cd_read_sectors(0x87U, 0x8018F000, 0xB98F);
            mode_step_next();
        }
        break;
    case 5:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if ((Fade_Out_Main() != 0) && (cd_read_sync_status() & 1)) {
            sports_candidate_scene_layer_resource_addresses_init();
            for (buffer_index = 0; buffer_index != 6; buffer_index++) {
                search_load_tpage_buf_lock(D_8014A070[buffer_index], 1);
            }
            mode_step_next();
        }
        break;
    case 6:
        actor_clear(0x60);
        for (buffer_index = 0; buffer_index < 3; buffer_index++) {
            game_state.tpage_buf[0x15 + buffer_index] = -1;
        }
        sports_runner_scene_graphics_init();
        sports_dance_actors_init();
        sports_schoolyard_actor_display_init();
        audio_command(0x505);
        Fade_In_Init();
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void sports_schoolyard_gameplay_mode_update(void) {
    switch (game_state.mode_step) {
    case 0:
        if (Fade_In_Main() != 0) {
            Vblnk_Timer_Init();
            mode_step_next();
        }
        sports_runner_scene_scroll_update();
        break;
    case 1:
        if (Vblnk_Timer() >= 0x5B) {
            audio_command(0x502);
            if ((u8) sports_schoolyard_result_selector != 0) {
                D_80120778 = 0x39;
            } else {
                D_80120778 = 0x2D;
                audio_command(0x800);
            }
            D_80120763 = 0xA4;
            D_80120762 = 0x40;
            mode_step_next();
        }
        sports_runner_scene_scroll_update();
        break;
    case 2:
        sports_schoolyard_frame_counters_advance();
        sports_dance_actors_position((s32) (s16) ((s16) D_8014A1CE / 2), 4);
        sports_schoolyard_player_frame_update();
        sports_schoolyard_cpu_animation_and_motion_update();
        sports_schoolyard_scroll_update();
        if (sports_schoolyard_finish_order_update() != 0) {
            audio_command(0x504);
            Fade_Out_Init();
            mode_step_next();
        }
        break;
    case 3:
        Scroll((s32) ((s16) D_8014A1CE / 4) % 640, 0xF, 0, 0x70, 0x280);
        Scroll((s32) ((s16) D_8014A1CE / 2) % 128, 0xF, 0x70, 0x80, 0x80);
        if (Fade_Out_Main() != 0) {
            sports_schoolyard_actor_group_init();
            sports_runner_scene_actor_init();
            mode_stage_next();
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void sports_schoolyard_result_flow_update(void) {
    s32 result_slot_index;
    s32 girl_index;

    Scroll(0, 0xF, 0, 0xF0, 0x280);
    switch (game_state.mode_step) {
    case 0:
        Fade_In_Init();
        mode_step_next();
        break;
    case 1:
        if (Fade_In_Main() != 0) {
            if (sports_schoolyard_result_status[0] == 1 || sports_schoolyard_result_status[1] == 1) {
                audio_command(0x205);
            } else {
                audio_command(0x206);
            }
            Vblnk_Timer_Init();
            mode_step_next();
        }
        break;
    case 2:
        if (Vblnk_Timer() >= 0x3D) {
            sports_schoolyard_runner_animation_step();
            Vblnk_Timer_Init();
            mode_step_next();
        }
        break;
    case 3:
        if (Vblnk_Timer() >= 0x3D) {
            sports_schoolyard_runner_animation_table_step();
            if (sports_schoolyard_result_selector != 0) {
                for (result_slot_index = 0; result_slot_index < 4; result_slot_index++) {
                    if (sports_schoolyard_result_status[result_slot_index] == 1) {
                        sports_scene_audio_sequence_request(1, (u8) result_slot_index);
                    }
                }
            } else {
                for (result_slot_index = 0; result_slot_index < 4; result_slot_index++) {
                    if (sports_schoolyard_result_status[result_slot_index] == 1) {
                        sports_scene_audio_sequence_request(0, (u8) result_slot_index);
                    }
                }
            }
            Vblnk_Timer_Init();
            sports_schoolyard_transition_wait_frames = 0;
            mode_step_next();
        }
        break;
    case 4:
        sports_schoolyard_transition_wait_frames += 1;
        sports_schoolyard_actor_visibility_toggle();
        if (Vblnk_Timer() >= 0xF1) {
            Fade_Out_Init();
            mode_step_next();
        }
        break;
    case 5:
        if (Fade_Out_Main() != 0) {
            if (sports_schoolyard_result_status[0] == 1) {
                for (girl_index = 0; girl_index < 13; girl_index++) {
                    game_state.girls[girl_index].tokimeki += 4;
                }
                if (sports_schoolyard_result_selector != 0) {
                    game_state.girls[1].tokimeki += 2;
                    game_state.girls[1].affection += 0xA;
                } else {
                    game_state.girls[0].tokimeki += 2;
                    game_state.girls[0].affection += 0xA;
                }
            } else if (sports_schoolyard_result_status[1] == 1) {
                for (girl_index = 0; girl_index < 13; girl_index++) {
                    game_state.girls[girl_index].tokimeki += 2;
                }
                if (sports_schoolyard_result_selector != 0) {
                    game_state.girls[1].tokimeki += 1;
                    game_state.girls[1].affection += 5;
                } else {
                    game_state.girls[0].tokimeki += 1;
                    game_state.girls[0].affection += 5;
                }
            }
            mode_stage_next();
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void sports_schoolyard_sprite_resource_setup(void) {
    s32 i;

    if ((u8) sports_schoolyard_result_selector == 0) {
        layer_tileset_init(0, D_8014A384, D_8014A370);
        srn_init(0, D_8014A37C, 0);
    } else {
        layer_tileset_init(0, D_8014A388, D_8014A374);
        srn_init(0, D_8014A380, 0);
    }
    srn_vram_set(0, 5, 0x580, 0xF0);
    for (i = 0; i < 3; i++) {
        game_state.tpage_buf[i + 5] = 8;
    }
    for (i = 0; i < 3; i++) {
        game_state.tpage_buf[i + 0x25] = 8;
    }
}

void sports_schoolyard_primary_actor_setup(void) {
    actor_clear(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120652 = 1;
    D_80120653 = 0xA4;
    D_80120657 = 0x80;
    D_8012065C = (u8 *) D_80149FC0;
    D_80120660 = (u8 *) D_80149FF8;
    D_80120684 = &D_8014A1FC;
    D_80120664 = D_8014A030;
    D_80120654 = 0;
    D_80120655 = 0x80;
    if ((u8) sports_schoolyard_result_selector == 0) {
        D_80120666 = 0;
    } else {
        D_80120666 = 2;
    }
    D_80120676 = 0x50;
    D_8012067A = 0xB0;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
}

/* FAKE: T-24133. `u32 raw` with `if (raw) {}` and `raw = 0` keeps the raw flags byte in $a0 and its `& 0xF` mask in $v1
 * without a J<-L widening CVT, while reading `unk_02` through `gs` loads it directly into $v0. */
void sports_schoolyard_controller_state_init(void) {
    GameState *game_state_ptr;
    s32 effective_athletics_value;
    s32 player_flag_nibble;
    u32 player_flags_byte;

    game_state_ptr = &game_state;
    sports_schoolyard_result_status[0] = 0;
    sports_controller_c_runners[0].finish_recorded = 0;
    sports_controller_c_runners[0].animation_frame = 0;
    sports_controller_c_runners[0].animation_frames = 0;
    sports_controller_c_runners[0].speed = 0;
    sports_controller_c_runners[0].motion_increment = 0;
    sports_controller_c_runners[0].motion_update_frames = 0;
    sports_controller_c_runners[0].track_position.halves.low = 0;
    sports_controller_c_runners[0].action_ready = 1;
    sports_controller_c_runners[0].unk_20 = 0;
    sports_controller_c_runners[0].action_recovery_frames = 0;
    sports_controller_c_runners[0].animation_start_frames = 0;
    sports_controller_c_runners[0].animation_transition_pending = 0;
    sports_schoolyard_result_status[1] = 0;
    sports_controller_c_runners[1].finish_recorded = 0;
    sports_controller_c_runners[1].animation_frame = 0;
    sports_controller_c_runners[1].animation_frames = 0;
    sports_controller_c_runners[1].speed = 0;
    sports_controller_c_runners[1].motion_increment = 0;
    sports_controller_c_runners[1].track_position.halves.high = 8; sports_controller_c_runners[1].action_ready = 1;
    sports_controller_c_runners[1].motion_update_frames = 0;
    sports_controller_c_runners[1].track_position.halves.low = 0;
    sports_controller_c_runners[1].unk_20 = 0;
    sports_controller_c_runners[1].action_recovery_frames = 0;
    sports_controller_c_runners[1].animation_start_frames = 0;
    sports_controller_c_runners[1].animation_transition_pending = 0;
    sports_schoolyard_result_status[2] = 0;
    sports_controller_c_runners[2].finish_recorded = 0;
    sports_controller_c_runners[2].animation_frame = 0;
    sports_controller_c_runners[2].animation_frames = 0;
    sports_controller_c_runners[2].speed = 0;
    sports_controller_c_runners[2].track_position.halves.high = 0x10;
    sports_controller_c_runners[2].motion_increment = 0;
    sports_controller_c_runners[2].motion_update_frames = 0;
    sports_controller_c_runners[2].track_position.halves.low = 0;
    sports_controller_c_runners[2].action_ready = 1;
    sports_controller_c_runners[2].unk_20 = 0;
    sports_controller_c_runners[2].action_recovery_frames = 0;
    sports_controller_c_runners[2].animation_start_frames = 0;
    sports_controller_c_runners[2].animation_transition_pending = 0;
    sports_schoolyard_result_status[3] = 0;
    sports_controller_c_runners[3].finish_recorded = 0;
    sports_controller_c_runners[3].animation_frame = 0;
    sports_controller_c_runners[3].animation_frames = 0;
    sports_controller_c_runners[3].track_position.halves.high = 0x18;
    sports_controller_c_runners[3].speed = 0;
    sports_controller_c_runners[3].motion_increment = 0;
    sports_controller_c_runners[3].motion_update_frames = 0;
    sports_controller_c_runners[3].track_position.halves.low = 0;
    sports_controller_c_runners[3].action_ready = 1;
    sports_controller_c_runners[3].unk_20 = 0;
    sports_controller_c_runners[3].action_recovery_frames = 0;
    sports_controller_c_runners[3].animation_start_frames = 0;
    sports_controller_c_runners[3].animation_transition_pending = 0;
    sports_controller_c_runners[0].track_position.halves.high = -0xA0;
    sports_schoolyard_scroll_velocity = 0;
    D_8014A1CC.w = 0;
    sports_schoolyard_speed_update_frames = 0;
    D_8014A130[0] = 0x7E0000;
    D_8014A130[1] = 0x7F0000;
    D_8014A130[2] = 0x7D0000;
    player_flags_byte = game_state_ptr->player_flags.b[1];
    if (player_flags_byte) {}
    effective_athletics_value = game_state_ptr->param_athletics.value;
    player_flag_nibble = player_flags_byte & 0xF;
    player_flags_byte = 0;
    if ((player_flag_nibble & 1) || (player_flag_nibble & 2)) {
        effective_athletics_value = (s16) (effective_athletics_value >> 1);
    }
    if (effective_athletics_value >= 0xB4) {
        sports_schoolyard_animation_interval = 6;
        effective_athletics_value = 0x20000;
    } else if (effective_athletics_value >= 0x64) {
        sports_schoolyard_animation_interval = 7;
        effective_athletics_value = 0x1E000;
    } else if (effective_athletics_value >= 0x41) {
        sports_schoolyard_animation_interval = 8;
        effective_athletics_value = 0x1C000;
    } else if (effective_athletics_value >= 0x1E) {
        sports_schoolyard_animation_interval = 9;
        effective_athletics_value = 0x1A000;
    } else {
        sports_schoolyard_animation_interval = 0xA;
        effective_athletics_value = 0x18000;
    }
    if ((u8) sports_schoolyard_result_selector != 0) {
        effective_athletics_value -= 0x2000;
    }
    sports_schoolyard_input_speed = effective_athletics_value >> 3;
    sports_schoolyard_motion_limit = effective_athletics_value;
    srand(game_state_ptr->timer_ticks);
}

/* FAKE: the dead `i = 0;` at the end keeps `i` live after the loop, so IDO tests the loop with
 * `slti` like the original and not as `i != 9`. T-19010 */
void sports_schoolyard_actor_display_init(void) {
    s32 actor_display_index;
    TaiShorts12 actor_y_initial_values;
    TaiShorts12 actor_sequence_initial_values;

    actor_y_initial_values = D_8014A2F0;
    actor_sequence_initial_values = D_8014A304;
    for (actor_display_index = 0; actor_display_index < 9; actor_display_index++) {
        actor_clear(actor_display_index + 0x60);
        DISP(96 + actor_display_index).quad_flags = 0x01000000;
        DISP(96 + actor_display_index).brightness = 0x80;
        DISP(96 + actor_display_index).frame_table = (u8 *)D_80149FC4;
        DISP(96 + actor_display_index).sequence_table = (u8 *)D_80149FFC;
        DISP(96 + actor_display_index).tpage_table = (u8 *)D_8014A208;
        DISP(96 + actor_display_index).unk_14 = D_8014A034;
        DISP(96 + actor_display_index).unk_04 = 0;
        DISP(96 + actor_display_index).unk_05 = 0x80;
        DISP(96 + actor_display_index).scale_x = 0x1000;
        DISP(96 + actor_display_index).scale_y = 0x1000;
        DISP(96 + actor_display_index).blink_period = 2;
        DISP(96 + actor_display_index).y = actor_y_initial_values.value[actor_display_index];
        DISP(96 + actor_display_index).unk_18 = actor_sequence_initial_values.value[actor_display_index];
    }
    if (sports_schoolyard_result_selector != 0) {
        DISP(96).unk_18 = 0xE;
    }
    DISP(96).unk_01 = 2;
    DISP(96).x = -0xA0;
    DISP(96).flags = 0x40;
    DISP(96).draw_flags = 0xA4;
    DISP(97).unk_01 = 3;
    DISP(97).x = -0x98;
    DISP(97).flags = 0x40;
    DISP(97).draw_flags = 0xA4;
    DISP(98).unk_01 = 4;
    DISP(98).x = -0x90;
    DISP(98).flags = 0x40;
    DISP(98).draw_flags = 0xA4;
    DISP(99).unk_01 = 5;
    DISP(99).x = -0x88;
    DISP(99).flags = 0x40;
    DISP(99).draw_flags = 0xA4;
    DISP(100).unk_01 = 1;
    DISP(100).x = DISP(96).x;
    DISP(100).y = DISP(96).y;
    DISP(101).unk_01 = 7;
    DISP(102).unk_01 = 5;
    DISP(103).unk_01 = 6;
    DISP(104).unk_01 = 7;
    DISP(104).x = -0x30;
    DISP(104).flags = 0x40;
    DISP(104).draw_flags = 0xA4;
    actor_display_index = 0;
}

void sports_schoolyard_frame_counters_advance(void) {
    s32 controller_index;

    for (controller_index = 0; controller_index < 4; controller_index++) {
        sports_controller_c_runners[controller_index].animation_frames += 1;
        sports_controller_c_runners[controller_index].motion_update_frames += 1;
        sports_controller_c_runners[controller_index].action_recovery_frames += 1;
        sports_controller_c_runners[controller_index].animation_start_frames += 1;
    }
    sports_schoolyard_speed_update_frames += 1;
}

void sports_schoolyard_player_frame_update(void) {
    if ((D_80120778 != 0x2D) && (D_80120778 != 0x39)) {
        sports_schoolyard_player_direction_set();
        sports_schoolyard_player_velocity_integrate();
        sports_schoolyard_player_position_clamp();
        sports_schoolyard_player_sequence_update();
        sports_schoolyard_input_state_update();
        return;
    }
    if ((u32)sports_controller_c_runners[0].animation_start_frames >= 0x32) {
        D_80120778 = 0x2F;
    }
}

void sports_schoolyard_player_direction_set(void) {
    if (sports_controller_c_runners[0].action_ready != 0) {
        sports_controller_c_runners[0].speed = sports_schoolyard_input_speed;
        return;
    }
    sports_controller_c_runners[0].speed = -sports_schoolyard_input_speed;
}

void sports_schoolyard_player_velocity_integrate(void) {
    if ((u32)sports_controller_c_runners[0].motion_update_frames >= 0xB) {
        sports_controller_c_runners[0].motion_increment += sports_controller_c_runners[0].speed;
        if (sports_controller_c_runners[0].motion_increment < -0x4000) {
            sports_controller_c_runners[0].motion_increment = -0x4000;
        }
        if (sports_schoolyard_motion_limit < sports_controller_c_runners[0].motion_increment) {
            sports_controller_c_runners[0].motion_increment = sports_schoolyard_motion_limit;
        }
        sports_controller_c_runners[0].motion_update_frames = 0;
    }
}

/* sports_controller_c_runners has to be an array here (not a struct) so that the base stays in a register; see SportsRunnerB. T-16550 */
void sports_schoolyard_player_position_clamp(void) {
    if (sports_controller_c_runners[0].speed < 0 || D_8014A1CE == 0) {
        if (sports_schoolyard_scroll_velocity != 0) {
            sports_controller_c_runners[0].track_position.word -= sports_schoolyard_scroll_velocity;
        }
    } else {
        sports_controller_c_runners[0].track_position.word += (s32)sports_schoolyard_motion_limit >> 2;
    }
    if (sports_controller_c_runners[0].track_position.halves.high >= -0x1F) {
        sports_controller_c_runners[0].track_position.halves.high = -0x20;
    }
    if (sports_controller_c_runners[0].track_position.halves.high < -0x9F) {
        sports_controller_c_runners[0].track_position.halves.high = -0xA0;
    }
    D_80120676 = sports_controller_c_runners[0].track_position.halves.high;
}

void sports_schoolyard_player_sequence_update(void) {
    if (((u32) sports_schoolyard_animation_interval < (u32) sports_controller_c_runners[0].animation_frames) && (sports_controller_c_runners[0].action_ready != 0)) {
        if ((s32) sports_controller_c_runners[0].animation_frame >= 9) {
            sports_controller_c_runners[0].animation_frame = 2;
            sports_controller_c_runners[0].animation_frames = 0;
        } else {
            sports_controller_c_runners[0].animation_frame = sports_controller_c_runners[0].animation_frame + 1;
            sports_controller_c_runners[0].animation_frames = 0;
        }
        if (sports_controller_c_runners[0].animation_frame == 6) {
            ((Actor *)actor_pool)[100].unk_18 = 0x2E;
            if ((u8) sports_schoolyard_result_selector != 0) {
                audio_command(0x804);
            } else {
                audio_command(0x801);
            }
            sports_controller_c_runners[0].animation_transition_pending = 1;
        }
        if (sports_controller_c_runners[0].animation_frame == 2) {
            ((Actor *)actor_pool)[100].unk_18 = 0x2F;
            if ((u8) sports_schoolyard_result_selector != 0) {
                audio_command(0x805);
            } else {
                audio_command(0x802);
            }
        }
    }
    if (sports_controller_c_runners[0].action_ready != 0) {
        if ((u8) sports_schoolyard_result_selector != 0) {
            D_80120668 = D_8014A1E8[sports_controller_c_runners[0].animation_frame];
        } else {
            D_80120668 = *(s16 *)&D_8014A1D4[sports_controller_c_runners[0].animation_frame * 2];
        }
    }
    ((Actor *)actor_pool)[100].x = ((Actor *)actor_pool)[96].x;
    ((Actor *)actor_pool)[100].y = ((Actor *)actor_pool)[96].y;
}

/* FAKE: the u8 view of the flag sports_schoolyard_result_selector keeps the lbu loads and the register shape of the
 * original; a plain read (promoted) changes the allocation. T-16554
 *
 * `bits` (GameState unk_F88) is read per region into a local: reading the field
 * directly lets the hoisted `& 5` test outrank it and take $v0. */
void sports_schoolyard_input_state_update(void) {
    s32 pressed_button_bits;

    if ((sports_controller_c_runners[0].animation_frame >= 2U) && (sports_controller_c_runners[0].animation_frame < 6U)) {
        pressed_button_bits = game_state.pad_pressed;
        if (pressed_button_bits & 5) {
            sports_controller_c_runners[0].unk_20 = 1;
        }
        if ((pressed_button_bits & 0xA) && (sports_controller_c_runners[0].action_ready != 0)) {
            sports_controller_c_runners[0].action_ready = 0;
            sports_controller_c_runners[0].action_recovery_frames = 0;
            sports_controller_c_runners[0].animation_transition_pending = 0;
            sports_controller_c_runners[0].motion_increment = 0;
            if (*(u8 *)&sports_schoolyard_result_selector != 0) {
                if (sports_controller_c_runners[0].unk_20 != 0) {
                    ((Actor *)actor_pool)[96].unk_18 = 0x11;
                } else {
                    audio_command(0x1805);
                    audio_command(0x1804);
                    audio_command(0x806);
                    ((Actor *)actor_pool)[96].unk_18 = 0x10;
                }
            } else if (sports_controller_c_runners[0].unk_20 != 0) {
                ((Actor *)actor_pool)[96].unk_18 = 8;
            } else {
                audio_command(0x1802);
                audio_command(0x1801);
                audio_command(0x803);
                ((Actor *)actor_pool)[96].unk_18 = 7;
            }
            sports_controller_c_runners[0].unk_20 = 0;
            ((Actor *)actor_pool)[100].draw_flags = 0;
            audio_command(0x50A);
        }
    }
    if ((sports_controller_c_runners[0].animation_frame >= 6U) && (sports_controller_c_runners[0].animation_frame < 0xAU)) {
        pressed_button_bits = game_state.pad_pressed;
        if (pressed_button_bits & 0xA) {
            sports_controller_c_runners[0].unk_20 = 1;
        }
        if ((pressed_button_bits & 5) && (sports_controller_c_runners[0].action_ready != 0)) {
            sports_controller_c_runners[0].action_ready = 0;
            sports_controller_c_runners[0].action_recovery_frames = 0;
            sports_controller_c_runners[0].animation_transition_pending = 0;
            sports_controller_c_runners[0].motion_increment = 0;
            if (*(u8 *)&sports_schoolyard_result_selector != 0) {
                if (sports_controller_c_runners[0].unk_20 != 0) {
                    ((Actor *)actor_pool)[96].unk_18 = 0x11;
                } else {
                    audio_command(0x1805);
                    audio_command(0x1804);
                    audio_command(0x806);
                    ((Actor *)actor_pool)[96].unk_18 = 0x10;
                }
            } else if (sports_controller_c_runners[0].unk_20 != 0) {
                ((Actor *)actor_pool)[96].unk_18 = 8;
            } else {
                audio_command(0x1802);
                audio_command(0x1801);
                audio_command(0x803);
                ((Actor *)actor_pool)[96].unk_18 = 7;
            }
            sports_controller_c_runners[0].unk_20 = 0;
            ((Actor *)actor_pool)[100].draw_flags = 0;
            audio_command(0x50A);
        }
    }
    if ((sports_controller_c_runners[0].animation_frames == 0) && ((sports_controller_c_runners[0].animation_frame == 5) || (sports_controller_c_runners[0].animation_frame == 9)) && (sports_controller_c_runners[0].animation_transition_pending == 1)) {
        if (sports_controller_c_runners[0].unk_20 != 0) {
            sports_controller_c_runners[0].unk_20 = 0;
        } else if (sports_controller_c_runners[0].action_ready != 0) {
            sports_controller_c_runners[0].unk_20 = 0;
            sports_controller_c_runners[0].action_ready = 0;
            sports_controller_c_runners[0].action_recovery_frames = 0;
            sports_controller_c_runners[0].animation_transition_pending = 0;
            sports_controller_c_runners[0].motion_increment = 0;
            if (*(u8 *)&sports_schoolyard_result_selector != 0) {
                audio_command(0x1805);
                audio_command(0x1804);
                audio_command(0x806);
                ((Actor *)actor_pool)[96].unk_18 = 0xF;
            } else {
                audio_command(0x1802);
                audio_command(0x1801);
                audio_command(0x803);
                ((Actor *)actor_pool)[96].unk_18 = 6;
            }
            ((Actor *)actor_pool)[100].draw_flags = 0;
            audio_command(0x50A);
        }
    }
    if (sports_controller_c_runners[0].action_ready == 0) {
        if (sports_controller_c_runners[0].action_recovery_frames >= 0x1EU) {
            sports_controller_c_runners[0].unk_20 = 0;
            sports_controller_c_runners[0].action_ready = 1;
            sports_controller_c_runners[0].animation_start_frames = 0;
            sports_controller_c_runners[0].animation_frame = 0;
            if (*(u8 *)&sports_schoolyard_result_selector != 0) {
                ((Actor *)actor_pool)[96].unk_18 = 0xE;
            } else {
                ((Actor *)actor_pool)[96].unk_18 = 5;
            }
            ((Actor *)actor_pool)[100].draw_flags = 0xA4;
            if (*(u8 *)&sports_schoolyard_result_selector == 0) {
                ((Actor *)actor_pool)[100].unk_18 = 0x2D;
                audio_command(0x800);
            } else {
                ((Actor *)actor_pool)[100].unk_18 = 0x39;
            }
        }
    }
}

void sports_schoolyard_scroll_update(void) {

    if ((u8) sports_schoolyard_speed_update_frames >= 0xBU) {
        sports_schoolyard_scroll_velocity += (sports_controller_c_runners[0].speed);
        sports_schoolyard_speed_update_frames = 0;
    }
    if (sports_schoolyard_motion_limit < sports_schoolyard_scroll_velocity) {
        sports_schoolyard_scroll_velocity = sports_schoolyard_motion_limit;
    }
    if (sports_schoolyard_scroll_velocity < 0) {
        sports_schoolyard_scroll_velocity = 0;
    }
    if ((D_80120778 == 0x2D) || (D_80120778 == 0x39)) {
        sports_schoolyard_scroll_velocity = 0;
    }
    D_8014A1CC.w += sports_schoolyard_scroll_velocity;
    Scroll((s32) ((s16) D_8014A1CC.h[1] / 4) % 640, 0xF, 0, 0x70, 0x280);
    Scroll((s32) ((s16) D_8014A1CC.h[1] / 2) % 128, 0xF, 0x70, 0x80, 0x80);
    debug_print_hex(sports_schoolyard_scroll_velocity, 0, -0x3C);
}

s32 sports_schoolyard_finish_order_update(void) {
    s32 i;
    s32 j;
    TkS16x4 t;

    t = D_8014A318;
    for (i = 0; i < 3; i++) {
        DISP(101 + i).flags = 0x40;
        DISP(101 + i).draw_flags = 0xA4;
    }
    for (i = 0; i < 3; i++) {
        if (sports_controller_c_runners[i + 1].finish_recorded == 1 && DISP(101).x + 0x30 < DISP(97 + i).x + 0x40) {
            DISP(102).draw_flags = 0;
            DISP(102).flags = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        DISP(101 + i).x = t.v[i] - D_8014A1CE - 0xA0;
    }
    if (DISP(102).unk_18 == 0x32) {
        DISP(102).draw_flags |= 0x40;
        DISP(102).x = DISP(101).x + 0x20;
    }
    if (DISP(104).x < -0xDC && DISP(104).draw_flags != 0) {
        DISP(104).draw_flags = 0;
    }
    if (DISP(96).x + 0x40 >= DISP(101).x) {
        if (sports_schoolyard_result_status[0] == 0) {
            DISP(102).draw_flags = 0xE4;
            DISP(102).unk_18 = 0x32;
            DISP(102).x = DISP(101).x + 0x20;
        }
        i = 0;
        if (sports_schoolyard_result_status[0] != 0) {
            do {
                i++;
            } while (sports_schoolyard_result_status[i] != 0);
        }
        sports_schoolyard_result_status[i] = 1;
        for (j = i + 1; j < 4; j++) {
            sports_schoolyard_result_status[j] = sports_schoolyard_cpu_place_select();
        }
        return 1;
    }
    return 0;
}

/* FAKE: v[6] as in sports_runner_cpu_place_select. T-0016 */
s32 sports_schoolyard_cpu_place_select(void) {
    s16 v[6];
    s32 i;

    for (i = 0; i < 3; i++) {
        if (sports_controller_c_runners[i + 1].finish_recorded == 0) {
            v[i] = *(s16 *)(actor_pool + i * 0x44 + 0x19EA);
        } else {
            v[i] = -0xF00;
        }
    }
    if (v[0] >= v[1] && v[0] >= v[2]) {
        sports_controller_c_runners[1].finish_recorded = 1;
        return 2;
    }
    if (v[1] >= v[2]) {
        sports_controller_c_runners[2].finish_recorded = 1;
        return 3;
    }
    sports_controller_c_runners[3].finish_recorded = 1;
    return 4;
}

typedef struct {
    s16 v[3];
} Tri3S; /* size 0x6 */
extern Tri3S D_8014A320;

void sports_schoolyard_cpu_animation_and_motion_update(void) {
    s32 i; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    Tri3S tbl;

    tbl = D_8014A320;
    for (i = 0; i < 3; i++) {
        if (sports_controller_c_runners[i + 1].animation_start_frames >= 0x33U) {
            if (i + 0x36 == ((Actor *)actor_pool)[i + 97].unk_18) {
                ((Actor *)actor_pool)[i + 97].unk_18 = tbl.v[i];
            }
        }
    }
    sports_schoolyard_cpu_speed_limit_update();
    sports_schoolyard_cpu_acceleration_update();
    sports_schoolyard_runner_position_and_finish_order_update();
    sports_schoolyard_cpu_animation_state_advance();
    sports_schoolyard_cpu_random_animation_transition();
}

typedef struct {
    s32 v[3];
} Tri3; /* size 0xC */
extern Tri3 sports_schoolyard_cpu_speeds;

void sports_schoolyard_cpu_speed_limit_update(void) {
    s32 i; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    Tri3 tbl;

    tbl = sports_schoolyard_cpu_speeds;
    for (i = 0; i < 3; i++) {
        if (i + 0x36 != *(s16 *) (actor_pool + i * 0x44 + 0x19DC)) {
            if (sports_controller_c_runners[i + 1].action_ready != 0) {
                sports_controller_c_runners[i + 1].speed = tbl.v[i];
            } else {
                sports_controller_c_runners[i + 1].speed = -0x500;
            }
        }
    }
}

extern Tri3 sports_schoolyard_cpu_motion_limits;

/* FAKE: byte-offset walk over the SportsRunnerB records; with r = &sports_controller_c_runners[i + 1] or the byte base plus 0x24 * i
 * cast to SportsRunnerB *, the fields give different code (checked in the T-16550 review). T-16550 */
void sports_schoolyard_cpu_acceleration_update(void) {
    s32 i; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    Tri3 tbl;
    u8 *q;
    u8 *r;
    s32 v;

    tbl = sports_schoolyard_cpu_motion_limits;
    for (i = 0, q = actor_pool; i < 3; i++, q += 0x44) {
        if (i + 0x36 != *(s16 *)(q + 0x19DC)) {
            r = (u8 *)sports_controller_c_runners + i * 0x24;
            if (*(u32 *)(r + 0x34) >= 0xBU) {
                v = *(s32 *)(r + 0x30) += *(s32 *)(r + 0x2C);
                if (v < 0) {
                    *(s32 *)(r + 0x30) = 0;
                    v = 0;
                }
                if (tbl.v[i] < v) {
                    *(s32 *)(r + 0x30) = tbl.v[i];
                }
                *(s32 *)(r + 0x34) = 0;
            }
        }
    }
}

extern Tri3 sports_schoolyard_cpu_position_adjustments;

/* FAKE: byte view of the SportsRunnerB records. Written as sports_controller_c_runners[i + 1] fields the loop gets a
 * second induction register and a frame 8 bytes larger (T-16550). The factored flag-store stride
 * preserves the height reload. T-10010. */
void sports_schoolyard_runner_position_and_finish_order_update(void) {
    s32 i;
    s32 d;
    u8 *q; /* FAKE: unused word keeps tbl at stack +0x48. T-10010. */
    Tri3 tbl;
    s32 y;
    s32 n;

    tbl = sports_schoolyard_cpu_position_adjustments;
    d = D_8014A1CE;
    for (i = 0; i < 3; i++) {
        *(s32 *)&((u8 *)sports_controller_c_runners)[i * 0x24 + 0x38] += *(s32 *)&((u8 *)sports_controller_c_runners)[i * 0x24 + 0x30];
        y = *(s16 *)&((u8 *)sports_controller_c_runners)[i * 0x24 + 0x3A] - d;
        ((Actor *)actor_pool)[i + 97].x = y - 0xA0;
        if (i + 0x36 != ((Actor *)actor_pool)[i + 97].unk_18 && D_8014A130[i] > 0) {
            *(s32 *)&((u8 *)sports_controller_c_runners)[i * 0x24 + 0x38] += tbl.v[i];
            D_8014A130[i] -= tbl.v[i];
            y = *(s16 *)&((u8 *)sports_controller_c_runners)[i * 0x24 + 0x3A] - d;
        }
        if (((Actor *)actor_pool)[101].x + i * 8 + 8 < y - 0x60 && ((u8 *)sports_controller_c_runners)[i * 0x24 + 0x25] == 0) {
            ((Actor *)actor_pool)[102].unk_18 = 0x32;
            ((Actor *)actor_pool)[102].x = ((Actor *)actor_pool)[101].x + 0x20;
            n = 0;
            while (sports_schoolyard_result_status[n] != 0) {
                n++;
            }
            sports_schoolyard_result_status[n] = i + 2;
            ((u8 *)sports_controller_c_runners)[(i * 3) * 12 + 0x25] = 1;
            y = *(s16 *)&((u8 *)sports_controller_c_runners)[i * 0x24 + 0x3A] - d;
        }
        if (y >= 0x1E1 || y < -0xA0) {
            ((Actor *)actor_pool)[i + 97].draw_flags = 0;
        } else {
            ((Actor *)actor_pool)[i + 97].draw_flags = 0xA4;
        }
    }
}

extern Tri3 sports_schoolyard_cpu_animation_intervals;

void sports_schoolyard_cpu_animation_state_advance(void) {
    s32 i;
    Tri3 tbl;

    tbl = sports_schoolyard_cpu_animation_intervals;
    for (i = 0; i < 3; i++) {
        if (i + 0x36 != ((Actor *)actor_pool)[i + 97].unk_18) {
            if ((u32)tbl.v[i] < sports_controller_c_runners[i + 1].animation_frames && sports_controller_c_runners[i + 1].action_ready == 1) {
                if ((s32)sports_controller_c_runners[i + 1].animation_frame >= 9) {
                    sports_controller_c_runners[i + 1].animation_frame = 2;
                    sports_controller_c_runners[i + 1].animation_frames = 0;
                } else {
                    sports_controller_c_runners[i + 1].animation_frame += 1;
                    sports_controller_c_runners[i + 1].animation_frames = 0;
                }
                ((Actor *)actor_pool)[i + 97].unk_18 = *(s16 *)&D_8014A1D4[sports_controller_c_runners[i + 1].animation_frame * 2] + i * 9 + 0x12;
            }
            if (sports_controller_c_runners[i + 1].animation_frame == 6) {
                sports_controller_c_runners[i + 1].animation_transition_pending = 1;
            }
        }
    }
}

extern Tri3S sports_schoolyard_cpu_action_thresholds;

/* FAKE: byte-offset walk over the SportsRunnerB records, same as sports_schoolyard_cpu_acceleration_update (the field form was checked in the
 * T-16550 review and differs). T-16550 */
void sports_schoolyard_cpu_random_animation_transition(void) {
    s32 i; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    Tri3S tbl;
    u8 *q;
    u8 *r;

    tbl = sports_schoolyard_cpu_action_thresholds;
    for (i = 0, q = actor_pool; i < 3; i++, q += 0x44) {
        if (i + 0x36 != *(s16 *)(q + 0x19DC)) {
            r = (u8 *)sports_controller_c_runners + i * 0x24;
            if (*(s32 *)(r + 0x28) == 0 && (r[0x24] == 2 || r[0x24] == 6) && *(s32 *)(r + 0x40) == 1 &&
                tbl.v[i] < (rand() & 0xFF) && r[0x26] == 1) {
                *(s32 *)(r + 0x40) = 0;
                *(s16 *)(r + 0x3E) = 0;
                *(s32 *)(r + 0x30) = 0;
                *(s16 *)(q + 0x19DC) = rand() % 3 + i * 9 + 0x18;
            }
        }
    }
    for (i = 0, q = actor_pool; i != 3; i++, q += 0x44) {
        if (i + 0x36 != *(s16 *)(q + 0x19DC)) {
            r = (u8 *)sports_controller_c_runners + i * 0x24;
            if (*(s32 *)(r + 0x40) == 0 && *(u16 *)(r + 0x3E) >= 0x1EU) {
                *(s32 *)(r + 0x40) = 1;
                *(s16 *)(r + 0x3C) = 0;
                r[0x24] = 0;
                r[0x26] = 0;
                *(s16 *)(q + 0x19DC) = i + 0x36;
            }
        }
    }
}

/* FAKE: the dead `i = sports_schoolyard_result_selector;` before the first call puts the flag in the entry set of
 * tools/cvt_pass.py, which then strips the widening of the flag test in the second loop (promoted
 * `lbu a1`, flag compared first); the dead `i = 0;` at the end keeps the second loop rolled with the
 * walk compare, as in sports_runner_actor_group_init. sports_schoolyard_result_selector is `u8` (was `s8`; the matched uses are unchanged). T-19010 */
void sports_schoolyard_actor_group_init(void) {
    s32 i;

    i = sports_schoolyard_result_selector;
    actor_clear_upper();
    for (i = 0; i < 8; i++) {
        DISP(96 + i).draw_flags = 0xA4;
        DISP(96 + i).flags = 0x40;
        DISP(96 + i).unk_05 = 0x80;
        DISP(96 + i).unk_01 = 3;
        DISP(96 + i).quad_flags = 0x01000000;
        DISP(96 + i).brightness = 0x80;
        DISP(96 + i).frame_table = (u8 *)D_80149FC8;
        DISP(96 + i).sequence_table = (u8 *)D_8014A000;
        DISP(96 + i).tpage_table = (u8 *)D_8014A228;
        DISP(96 + i).unk_14 = D_8014A038;
        DISP(96 + i).scale_x = 0x1000;
        DISP(96 + i).scale_y = 0x1000;
        DISP(96 + i).blink_period = 2;
    }
    for (i = 0; i < 4; i++) {
        DISP(96 + i).x = i * 0x40 - 0x78;
        DISP(96 + i).unk_18 = sports_schoolyard_result_status[i] * 8 + 0x1A;
        DISP(96 + i).y = -8;
        if (DISP(96 + i).unk_18 == 0x22 && sports_schoolyard_result_selector == 1) {
            DISP(96 + i).unk_18 = 0x42;
        }
    }
    DISP(100).x = -0x78;
    DISP(100).y = -8;
    DISP(100).unk_18 = sports_schoolyard_result_status[0] * 5 + 0xD;
    DISP(101).x = -0x38;
    DISP(101).y = -8;
    DISP(101).unk_18 = sports_schoolyard_result_status[1] * 5 + 0xD;
    DISP(102).x = 8;
    DISP(102).y = -8;
    DISP(102).unk_18 = sports_schoolyard_result_status[2] * 5 + 0xD;
    DISP(103).x = 0x48;
    DISP(103).y = -8;
    DISP(103).unk_18 = sports_schoolyard_result_status[3] * 5 + 0xD;
    i = 0;
}

void sports_schoolyard_runner_animation_step(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        ((Actor *)actor_pool)[i + 96].unk_18 += 1;
    }
    for (i = 0; i < 4; i++) {
        ((Actor *)actor_pool)[i + 100].unk_18 -= 4 - i;
    }
}

void sports_schoolyard_runner_animation_table_step(void) {
    s32 i;
    TkS16x4 t;

    t = D_8014A360;
    for (i = 0; i < 4; i++) {
        ((Actor *)actor_pool)[i + 96].unk_18 += t.v[i];
    }
}

void sports_schoolyard_actor_visibility_toggle(void) {
    if ((u32)sports_schoolyard_transition_wait_frames >= 0xF) {
        *(s16 *)(actor_pool + 0x19DC) ^= 1;
        *(s16 *)(actor_pool + 0x1A20) ^= 1;
        sports_schoolyard_transition_wait_frames = 0;
    }
}

void sports_schoolyard_resource_addresses_init(void) {
    D_8014A370 = 0x801AD000;
    D_8014A374 = 0x801AE018;
    D_8014A378 = 0x8018F000;
    D_8014A37C = 0x801AF1B0;
    D_8014A380 = 0x801B29EC;
    D_8014A384 = 0x801AD008;
    D_8014A388 = 0x801AE020;
}
