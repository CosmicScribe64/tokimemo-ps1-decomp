/* runner_actor_display.c: src/ovl/TAIIKU/801452D0.c:77,86,150; runner-scene mode-stage callback dispatch, display-record setup, animation/movement and slot updates; wiki/spec/o-taiiku.md records the body-backed actor contracts. Original object 0x801452D0-0x801488F0 (config/objects/TAIIKU.txt). */
#define MAIN_API_OVERRIDE_scene_screen_reset /* called as an implicit int function (definition: void): the call's $v0 result keeps the loop pointer off $v0 in sports_controller_c_resource_setup, T-20600 */
#include "common.h"
#include "ovl/TAIIKU.h"

s32 scene_screen_reset();

/* Display record n of the runner sprites (actor_pool is an array of 0x44-byte records). */
#define DISP(n) (((Actor *)actor_pool)[n])
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A3B0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A3B4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A3B8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A3C4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A3D0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A3D4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A3D8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A3DC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A3E0);
SportsController sports_controller_c_player_controller = { 0 };
SportsController sports_controller_c_cpu_controllers[3] = { 0 };
SportsRunnerDisplaySlot sports_runner_display_slots[4] = { 0 };

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A4C4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A4D4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A4D8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A4E4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A4E8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A4F4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", sports_controller_c_texture_page_buffer_ids);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", sports_controller_c_texture_page_buffer_ids_table_b);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A52C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A53C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A54C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A56C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A570);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A5A0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A5C4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A5DC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A5F4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A5FC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A608);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A614);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A620);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A634);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A648);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A658);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_actor_display.data", D_8014A668);

typedef struct {
    void (*f[8])();
} FnTbl8; /* size 0x20 */
extern FnTbl8 D_8014A54C;

typedef struct {
    s32 v[3];
} Lim3; /* size 0xC */

typedef struct TkPl {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ u8 pad4[2];
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ u8 unk_0C;
    /* 0x0D */ u8 unk_0D;
    /* 0x0E */ u8 pad0E[6];
    /* 0x14 */ u16 unk_14;
    /* 0x16 */ u16 unk_16;
    /* 0x18 */ u8 pad18[4];
} TkPl; /* size 0x1C */
extern Lim3 D_8014A5FC;
extern Lim3 D_8014A608;
extern Lim3 D_8014A614;
extern SportsRunnerDisplaySlot D_8014A4A8; /* slot 3, the end of the first slot loop in sports_controller_c_motion_choices_update. T-16553 */

void sports_controller_c_mode_dispatch(void) {
    s32 mode_stage_index; /* declared before controller_c_stage_callbacks: its stack slot sits above controller_c_stage_callbacks (T-3330) */
    FnTbl8 controller_c_stage_callbacks;

    controller_c_stage_callbacks = D_8014A54C;
    mode_stage_index = game_state.mode_stage;
    controller_c_stage_callbacks.f[mode_stage_index]();
}

void sports_controller_c_resource_setup(void) {
    s32 resource_index;
    s32 palette_index;
    s32 initial_texture_page_buffer_id[1];
    TaiWords12 texture_page_buffer_ids;
    TaiWords9 secondary_texture_page_buffer_ids;

    initial_texture_page_buffer_id[0] = D_8014A56C;
    texture_page_buffer_ids = D_8014A570;
    secondary_texture_page_buffer_ids = D_8014A5A0;
    switch (game_state.mode_step) {
    case 0:
        scene_screen_reset(1, 1);
        for (resource_index = 0; resource_index < 11; resource_index++) {
            game_state.tpage_buf[resource_index + 5] = 8;
        }
        for (resource_index = 0; resource_index < 3; resource_index++) {
            game_state.tpage_buf[resource_index + 0x25] = 8;
        }
        D_8014A4F4 = initial_texture_page_buffer_id[0];
        for (resource_index = 0; resource_index < 12; resource_index++) {
            sports_controller_c_texture_page_buffer_ids[resource_index] = texture_page_buffer_ids.v[resource_index];
        }
        sports_controller_c_texture_page_buffer_ids_table_b[0] = secondary_texture_page_buffer_ids.v[0];
        for (resource_index = 1; resource_index < 9; resource_index++) {
            sports_controller_c_texture_page_buffer_ids_table_b[resource_index] = secondary_texture_page_buffer_ids.v[resource_index];
        }
        actor_clear_range(0);
        hud_cursor_show(0);
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
            cd_read_sectors(0x26, 0x8018F000, 0xB969);
            mode_step_next();
        }
        break;
    case 3:
        if ((Fade_Out_Main() != 0) && (cd_read_sync_status() & 1)) {
            sports_scene_actor_resource_addresses_init();
            sports_runner_resource_addresses_init();
            search_load_tpage_buf_lock(D_8014A4F4, 1);
            for (palette_index = 0; palette_index < 0x10; palette_index++) {
                load_palette(D_80148EF4, (palette_index + 0x10) & 0xFF, 1, 1, palette_index);
            }
            sports_controller_c_state_init();
            sports_controller_c_sprite_resource_setup();
            sports_controller_c_primary_actor_init();
            cd_read_sectors(0xB9, 0x8018F000, 0xB8B0);
            mode_stage_next();
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void sports_controller_c_resource_load(void) {
    s32 slot;
    s32 i;

    switch (game_state.mode_step) {
    case 0:
        if (cd_read_sync_status() & 1) {
            for (i = 0; i < 0xC; i++) {
                slot = search_load_tpage_buf_lock(sports_controller_c_texture_page_buffer_ids[i], 1);
                sports_controller_c_texture_page_buffer_ids[i] = game_state.tpage_buf[slot] = i + 0x80000100;
            }
            for (i = 5; i < 9; i++) {
                slot = search_load_tpage_buf_lock(sports_controller_c_texture_page_buffer_ids_table_b[i], 1);
                sports_controller_c_texture_page_buffer_ids_table_b[i] = game_state.tpage_buf[slot] = i + 0x80000200;
            }
            mode_step_next();
        }
        break;
    case 1:
        audio_command(0x201);
        Fade_In_Init();
        mode_step_next();
        break;
    case 2:
        if (Fade_In_Main() != 0) {
            mode_step_next();
        }
        if (game_state.pad_pressed & 0xF0) {
            Fade_Out_Init();
            mode_step_set(4);
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        break;
    case 3:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        switch (game_state.substep) {
        case 0:
            cd_audio_request_set(0x1F000330, 0x3F000356, 0);
            audio_command(0x300);
            game_state.substep += 1;
            break;
        case 1:
        case 3:
            if (cd_audio_state() == 4) {
                game_state.substep += 1;
            }
            break;
        case 2:
            if (cd_audio_state() == 2) {
                cd_audio_request_set(0x1F000357, 0x40000360, 0);
                audio_command(0x300);
                game_state.substep += 1;
            }
            break;
        case 4:
            if (cd_audio_state() == 2) {
                Vblnk_Timer_Init();
                game_state.substep += 1;
            }
            break;
        case 5:
            if (Vblnk_Timer() >= 0xB5) {
                game_state.substep = 0;
            }
            break;
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
            i = 0;
            do {
                search_load_tpage_buf_lock(D_8014A070[i], 1);
                i++;
            } while ((s32 *)&D_8014A088 != &D_8014A070[i]); /* bound: the next table symbol (T-16553) */
            mode_step_next();
        }
        break;
    case 6:
        actor_clear(0x60);
        sports_runner_scene_graphics_init();
        game_state.tpage_buf[21] = -1;
        sports_dance_actors_init();
        sports_controller_c_actor_display_init();
        audio_command(0x505);
        Fade_In_Init();
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void sports_controller_c_gameplay_mode_update(void) {
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
            mode_step_next();
        }
        sports_runner_scene_scroll_update();
        break;
    case 2:
        sports_controller_c_ticks_advance();
        sports_controller_c_player_frame_update();
        sports_controller_c_scroll_integrate();
        sports_dance_actors_position(D_8014A3DE, 4);
        sports_controller_c_cpu_frame_update();
        sports_controller_c_frame_update();
        sports_controller_c_slot_motion_update();
        if (sports_controller_c_finish_order_update() != 0) {
            audio_command(0x504);
            Fade_Out_Init();
            mode_step_next();
        }
        break;
    case 3:
        Scroll(D_8014A3DE, 0xF, 0, 0x70, 0x280);
        Scroll(D_8014A3DE % 128, 0xF, 0x70, 0x80, 0x80);
        if (Fade_Out_Main() != 0) {
            audio_command(0xC2);
            sports_controller_c_result_actors_init();
            sports_runner_scene_actor_init();
            mode_stage_next();
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void sports_controller_c_result_flow_update(void) {
    s32 result_slot_index;
    s32 girl_index; /* declared before result_status_pointer: the spill slot of result_status_pointer and the 0x48 frame depend on it (T-16553) */
    u8 *result_status_pointer;

    Scroll(0, 0xF, 0, 0xF0, 0x280);
    switch (game_state.mode_step) {
    case 0:
        audio_command(0x50B);
        Fade_In_Init();
        mode_step_next();
        break;
    case 1:
        if (Fade_In_Main() != 0) {
            if ((D_8014A3D8[0] == 1) || (D_8014A3D8[1] == 1)) {
                audio_command(0x205);
            } else {
                audio_command(0x206);
            }
            mode_step_next();
        }
        break;
    case 2:
        if (sports_controller_c_result_actor_approach() != 0) {
            mode_step_next();
        }
        break;
    case 3:
        if (sports_controller_c_result_sequence_shift() != 0) {
            mode_step_next();
        }
        break;
    case 4:
        sports_controller_c_result_actor_motion_start();
        result_status_pointer = D_8014A3D8;
        for (result_slot_index = 0; result_slot_index < 4; result_slot_index++) {
            if (*result_status_pointer == 1) {
                sports_scene_audio_sequence_request(0xB, result_slot_index & 0xFF);
            }
            result_status_pointer++;
        }
        mode_step_next();
        break;
    case 5:
        sports_controller_c_result_sequence_advance();
        break;
    case 6:
        Vblnk_Timer_Init();
        mode_step_next();
        break;
    case 7:
        if (Vblnk_Timer() >= 0xF1) {
            Fade_Out_Init();
            mode_step_next();
        }
        break;
    case 8:
        if (Fade_Out_Main() != 0) {
            if (D_8014A3D8[0] == 1) {
                for (girl_index = 0; girl_index < 13; girl_index++) {
                    game_state.girls[girl_index].tokimeki += 5;
                }
            } else if (D_8014A3D8[1] == 1) {
                for (girl_index = 0; girl_index < 13; girl_index++) {
                    game_state.girls[girl_index].tokimeki += 3;
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

void sports_controller_c_sprite_resource_setup(void) {
    s32 tpage_buffer_index;

    layer_tileset_init(0, D_80148EF8, D_80148EF0);
    srn_init(0, D_80148EFC, 0);
    srn_vram_set(0, 5, 0x580, 0xF0);
    for (tpage_buffer_index = 0; tpage_buffer_index < 3; tpage_buffer_index++) {
        game_state.tpage_buf[tpage_buffer_index + 5] = 8;
    }
    for (tpage_buffer_index = 0; tpage_buffer_index < 3; tpage_buffer_index++) {
        game_state.tpage_buf[tpage_buffer_index + 0x25] = 8;
    }
}

void sports_controller_c_primary_actor_init(void) {
    actor_clear(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120652 = 1;
    D_80120653 = 0xA4;
    D_80120657 = 0x80;
    D_8012065C = (u8 *) D_80149FD0;
    D_80120660 = (u8 *) D_8014A008;
    D_80120684 = (u8 *)&D_8014A4F4;
    D_80120664 = D_8014A040;
    D_80120654 = 0;
    D_80120655 = 0x80;
    D_80120666 = 0;
    D_80120676 = 0x20;
    D_8012067A = 0x10;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
}

/* FAKE: T-24133. `u32 raw` with `if (raw) {}` and `raw = 0` keeps the raw flags byte in $a0 and its `& 0xF` mask in $v1
 * without a J<-L widening CVT, while reading `unk_02` through `gs` loads it directly into $v0. */
void sports_controller_c_state_init(void) {
    GameState *game_state_ptr;
    s32 controller_slot_index;
    s32 effective_athletics_value;
    s32 player_flag_nibble;
    u32 player_flags_byte;

    sports_controller_c_player_controller.unk_00 = 0;
    sports_controller_c_player_controller.unk_08 = 0;
    sports_controller_c_player_controller.unk_0C = 0;
    sports_controller_c_player_controller.unk_04 = 0;
    sports_controller_c_player_controller.position.h[0] = 0;
    sports_controller_c_player_controller.unk_14 = 0;
    sports_controller_c_player_controller.unk_01 = 0;
    sports_controller_c_player_controller.unk_02 = 0;
    for (controller_slot_index = 0; controller_slot_index < 3; controller_slot_index++) {
        sports_controller_c_cpu_controllers[controller_slot_index].position.h[1] = (controller_slot_index + 1) * 8;
        sports_controller_c_cpu_controllers[controller_slot_index].unk_0C = 0;
        sports_controller_c_cpu_controllers[controller_slot_index].unk_04 = 0;
        sports_controller_c_cpu_controllers[controller_slot_index].unk_00 = 0;
        sports_controller_c_cpu_controllers[controller_slot_index].unk_14 = 0;
        sports_controller_c_cpu_controllers[controller_slot_index].position.h[0] = 0;
        sports_controller_c_cpu_controllers[controller_slot_index].unk_01 = 0;
    }
    for (controller_slot_index = 0; controller_slot_index < 4; controller_slot_index++) {
        D_8014A3D8[controller_slot_index] = 0;
        sports_runner_display_slots[controller_slot_index].unk_18 = 0;
        sports_runner_display_slots[controller_slot_index].unk_02 = 0x37;
        sports_runner_display_slots[controller_slot_index].unk_06 = 0x19;
        sports_runner_display_slots[controller_slot_index].unk_08 = 0;
        sports_runner_display_slots[controller_slot_index].unk_14 = 0;
        sports_runner_display_slots[controller_slot_index].cooldown = 0;
        sports_runner_display_slots[controller_slot_index].unk_0C = 0;
        sports_runner_display_slots[controller_slot_index].motion_timer = 0;
        sports_runner_display_slots[controller_slot_index].unk_0E = 0;
        sports_runner_display_slots[controller_slot_index].unk_0F = 0;
    }
    sports_controller_c_player_controller.position.h[1] = -0xA0;
    D_8014A3E0 = 0;
    D_8014A3DC = 0;
    D_8014A3C4 = 0;
    game_state_ptr = &game_state;
    player_flags_byte = game_state_ptr->player_flags.b[1];
    if (player_flags_byte) {}
    effective_athletics_value = game_state_ptr->param_athletics.value;
    player_flag_nibble = player_flags_byte & 0xF;
    player_flags_byte = 0;
    if ((player_flag_nibble & 1) || (player_flag_nibble & 2)) {
        effective_athletics_value = (s16) (effective_athletics_value >> 1);
    }
    if (effective_athletics_value >= 0xA0) {
        D_8014A3D0 = 0x280;
        D_8014A3D4 = 0x20000;
    } else if (effective_athletics_value >= 0x7D) {
        D_8014A3D0 = 0x220;
        D_8014A3D4 = 0x1E000;
    } else if (effective_athletics_value >= 0x64) {
        D_8014A3D0 = 0x1C0;
        D_8014A3D4 = 0x1C000;
    } else if (effective_athletics_value >= 0x4B) {
        D_8014A3D0 = 0x160;
        D_8014A3D4 = 0x1A000;
    } else {
        D_8014A3D0 = 0x100;
        D_8014A3D4 = 0x18000;
    }
    srand(game_state.timer_ticks);
}

void sports_controller_c_actor_display_init(void) {
    s32 actor_display_index;
    TaiShorts12x a;
    TaiShorts12x b;

    a = D_8014A5C4;
    b = D_8014A5DC;
    for (actor_display_index = 0; actor_display_index < 12; actor_display_index++) {
        actor_clear(actor_display_index + 0x60);
        DISP(96 + actor_display_index).quad_flags = 0x01000000;
        DISP(96 + actor_display_index).brightness = 0x80;
        DISP(96 + actor_display_index).frame_table = (u8 *)D_80149FD4;
        DISP(96 + actor_display_index).sequence_table = (u8 *)D_8014A00C;
        DISP(96 + actor_display_index).tpage_table = (u8 *)sports_controller_c_texture_page_buffer_ids;
        DISP(96 + actor_display_index).unk_14 = D_8014A044;
        DISP(96 + actor_display_index).unk_04 = 0;
        DISP(96 + actor_display_index).unk_05 = 0x80;
        DISP(96 + actor_display_index).scale_x = 0x1000;
        DISP(96 + actor_display_index).scale_y = 0x1000;
        DISP(96 + actor_display_index).blink_period = 2;
        DISP(96 + actor_display_index).y = a.v[actor_display_index];
        DISP(96 + actor_display_index).unk_18 = b.v[actor_display_index];
    }
    for (actor_display_index = 0; actor_display_index < 4; actor_display_index++) {
        DISP(96 + actor_display_index).unk_01 = actor_display_index * 2 + 3;
        DISP(96 + actor_display_index).x = actor_display_index * 8 - 0xA0;
        DISP(96 + actor_display_index).flags = 0x40;
        DISP(96 + actor_display_index).draw_flags = 0xA4;
    }
    for (actor_display_index = 0; actor_display_index < 4; actor_display_index++) {
        DISP(100 + actor_display_index).unk_01 = actor_display_index * 2 + 2;
        DISP(100 + actor_display_index).x = DISP(96 + actor_display_index).x + 0x37;
        DISP(100 + actor_display_index).y = DISP(96 + actor_display_index).y + 0x1C;
        DISP(100 + actor_display_index).flags = 0x40;
        DISP(100 + actor_display_index).draw_flags = 0xA4;
    }
    DISP(104).unk_01 = 9;
    DISP(105).unk_01 = 8;
    DISP(106).unk_01 = 9;
    DISP(107).unk_01 = 9;
    DISP(107).x = -0x30;
    DISP(107).flags = 0x40;
    DISP(107).draw_flags = 0xA4;
}

s32 sports_controller_c_ticks_advance(void) {
    SportsController *controller;
    s32 i;

    controller = &sports_controller_c_player_controller;
    controller->unk_0C++;
    controller->unk_08++;
    controller->unk_04++;
    controller->unk_02++;
    i = 0;
    /* the pointer bound keeps the original's sltu and lui order; `i < 3` gives bne (T-16553) */
    do {
        sports_controller_c_cpu_controllers[i].unk_0C++;
        sports_controller_c_cpu_controllers[i].unk_04++;
        i++;
    } while (&sports_controller_c_cpu_controllers[i] < (SportsController *)sports_runner_display_slots);
    sports_runner_display_slots[0].unk_18++;
    sports_runner_display_slots[0].unk_14++;
    sports_runner_display_slots[0].cooldown++;
    sports_runner_display_slots[0].motion_timer++;
    sports_runner_display_slots[0].unk_0F++;
    sports_runner_display_slots[1].unk_18++;
    sports_runner_display_slots[1].unk_14++;
    sports_runner_display_slots[1].cooldown++;
    sports_runner_display_slots[1].motion_timer++;
    sports_runner_display_slots[1].unk_0F++;
    sports_runner_display_slots[2].unk_18++;
    sports_runner_display_slots[2].unk_14++;
    sports_runner_display_slots[2].cooldown++;
    sports_runner_display_slots[2].motion_timer++;
    sports_runner_display_slots[2].unk_0F++;
    sports_runner_display_slots[3].unk_18++;
    sports_runner_display_slots[3].unk_14++;
    sports_runner_display_slots[3].cooldown++;
    sports_runner_display_slots[3].motion_timer++;
    sports_runner_display_slots[3].unk_0F++;
}

void sports_controller_c_player_frame_update(void) {
    sports_controller_c_player_velocity_input_update();
    sports_controller_c_player_velocity_integrate();
    sports_controller_c_player_position_clamp();
    sports_controller_c_player_sequence_select();
}

void sports_controller_c_player_velocity_input_update(void) {
    if (sports_runner_display_slots[0].unk_0C != 0) {
        sports_controller_c_player_controller.unk_10 = -0x300;
        return;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        D_8014A3C4 = 1;
        return;
    }
    if (sports_controller_c_player_controller.unk_08 >= 0x10) {
        if (D_8014A3C4 != 0) {
            if (sports_controller_c_player_controller.unk_14 < 0) {
                sports_controller_c_player_controller.unk_14 = 0;
            }
            sports_controller_c_player_controller.unk_10 = D_8014A3D0;
        } else {
            sports_controller_c_player_controller.unk_10 = -0x300;
        }
        sports_controller_c_player_controller.unk_08 = 0;
        D_8014A3C4 = 0;
    }
}

void sports_controller_c_player_velocity_integrate(void) {
    if ((u32)sports_controller_c_player_controller.unk_0C >= 0xB) {
        sports_controller_c_player_controller.unk_14 += sports_controller_c_player_controller.unk_10;
        if (sports_controller_c_player_controller.unk_14 < -0x10000) {
            sports_controller_c_player_controller.unk_14 = -0x10000;
        }
        if (D_8014A3D4 < sports_controller_c_player_controller.unk_14) {
            sports_controller_c_player_controller.unk_14 = D_8014A3D4;
        }
        sports_controller_c_player_controller.unk_0C = 0;
    }
}

void sports_controller_c_player_position_clamp(void) {
    if ((D_80120668 != 3) && ((sports_runner_display_slots[0].unk_0C == 0) || (D_8014A3E0 != 0))) {
        if ((sports_controller_c_player_controller.unk_10 < 0) || (D_8014A3DE == 0)) {
            sports_controller_c_player_controller.position.w -= D_8014A3E0;
        } else {
            sports_controller_c_player_controller.position.w += 0x8000;
        }
    }
    if (sports_controller_c_player_controller.position.h[1] >= -0x1F) {
        sports_controller_c_player_controller.position.h[1] = -0x20;
    }
    if (sports_controller_c_player_controller.position.h[1] < -0x9F) {
        sports_controller_c_player_controller.position.h[1] = -0xA0;
    }
    D_80120676 = sports_controller_c_player_controller.position.h[1];
}

void sports_controller_c_player_sequence_select(void) {
    s32 flags;

    if (sports_runner_display_slots[0].unk_0C == 0) {
        sports_controller_c_player_sequence_advance();
        flags = game_state.pad_held;
        if ((flags & 0xA) && !(flags & 5)) {
            if (sports_controller_c_player_controller.unk_02 >= 0x15 && D_8014A3E0 > 0) {
                audio_command(0x507);
                sports_controller_c_player_controller.unk_02 = 0;
            }
            D_80120668 = D_8014A4D4[sports_controller_c_player_controller.unk_00];
            return;
        }
        if ((flags & 5) && (flags & 0xA) == 0) {
            if (sports_controller_c_player_controller.unk_02 >= 0x15 && D_8014A3E0 > 0) {
                audio_command(0x508);
                sports_controller_c_player_controller.unk_02 = 0;
            }
            D_80120668 = D_8014A4E4[sports_controller_c_player_controller.unk_00];
            return;
        }
        D_80120668 = D_8014A4C4[sports_controller_c_player_controller.unk_00];
    }
}

void sports_controller_c_player_sequence_advance(void) {
    if (D_8014A3E0 == 0) {
        sports_controller_c_player_controller.unk_00 = 0;
        return;
    }
    if (((u32)sports_controller_c_player_controller.unk_04 >= 6 && sports_controller_c_player_controller.unk_10 > 0) || ((u32)sports_controller_c_player_controller.unk_04 >= 0xB && sports_controller_c_player_controller.unk_14 != 0)) {
        if (sports_controller_c_player_controller.unk_00 >= 7) {
            sports_controller_c_player_controller.unk_00 = 2;
            sports_controller_c_player_controller.unk_04 = 0;
        } else {
            sports_controller_c_player_controller.unk_00++;
            sports_controller_c_player_controller.unk_04 = 0;
        }
    }
}

void sports_controller_c_scroll_integrate(void) {
    D_8014A3E0 += sports_controller_c_player_controller.unk_10;
    if (D_8014A3D4 < D_8014A3E0) {
        D_8014A3E0 = D_8014A3D4;
    }
    if (D_8014A3E0 < 0 || (sports_controller_c_player_controller.unk_10 < 0 && sports_controller_c_player_controller.position.h[1] < -0x9F)) {
        D_8014A3E0 = 0;
    }
    D_8014A3DC += D_8014A3E0; /* the fixed-point high half (+2) is read as an s16 below */
    Scroll(((s16 *)&D_8014A3DC)[1] / 2 % 640, 0xF, 0, 0x70, 0x280);
    Scroll(((s16 *)&D_8014A3DC)[1] % 128, 0xF, 0x70, 0x80, 0x80);
}

s32 sports_controller_c_finish_order_update(void) {
    s32 finish_order_fill_index;
    s32 finish_order_index;
    TkS16x4 finish_marker_offsets;

    finish_marker_offsets = D_8014A5F4;
    if (DISP(104).flags == 0 && D_8014A3DE >= 0x781) {
        for (finish_order_index = 0; finish_order_index < 3; finish_order_index++) {
            DISP(104 + finish_order_index).flags = 0x40;
            DISP(104 + finish_order_index).draw_flags = 0xA4;
            if (sports_controller_c_cpu_controllers[finish_order_index].unk_01 == 1) {
                DISP(105).draw_flags = 0;
            }
        }
    }
    if (DISP(105).unk_18 == 0x4F) {
        DISP(105).draw_flags |= 0x40;
    }
    for (finish_order_index = 0; finish_order_index < 3; finish_order_index++) {
        if (sports_controller_c_cpu_controllers[finish_order_index].unk_01 == 1 && DISP(97 + finish_order_index).x >= DISP(105).x + 0x30) {
            DISP(105).draw_flags = 0;
            DISP(105).flags = 0;
        }
    }
    for (finish_order_index = 0; &finish_marker_offsets.v[finish_order_index] < &finish_marker_offsets.v[4]; finish_order_index++) {
        DISP(104 + finish_order_index).x = finish_marker_offsets.v[finish_order_index] - D_8014A3DE * 2 - 0xA0;
    }
    if (DISP(107).x < -0xDC && DISP(107).draw_flags != 0) {
        DISP(107).draw_flags = 0;
    }
    if (DISP(105).unk_18 == 0x4F) {
        DISP(105).x = DISP(104).x + 0x20;
    }
    if (DISP(96).x + 0x40 >= DISP(104).x) {
        if (D_8014A3D8[0] == 0) {
            DISP(105).draw_flags = 0xE4;
            DISP(105).unk_18 = 0x4F;
            DISP(105).x = DISP(106).x + 0x20;
        }
        finish_order_index = 0;
        while (D_8014A3D8[finish_order_index] != 0) {
            finish_order_index++;
        }
        D_8014A3D8[finish_order_index] = 1;
        for (finish_order_fill_index = finish_order_index + 1; finish_order_fill_index < 4; finish_order_fill_index++) {
            D_8014A3D8[finish_order_fill_index] = sports_controller_c_cpu_place_select();
        }
        return 1;
    }
    return 0;
}

/* FAKE: v[6] as in sports_runner_cpu_place_select. T-0016 */
s32 sports_controller_c_cpu_place_select(void) {
    s16 cpu_place_metric_values[6];
    s32 cpu_runner_index;

    for (cpu_runner_index = 0; cpu_runner_index < 3; cpu_runner_index++) {
        if (sports_controller_c_cpu_controllers[cpu_runner_index].unk_01 == 0) {
            cpu_place_metric_values[cpu_runner_index] = *(s16 *)(actor_pool + cpu_runner_index * 0x44 + 0x19EA);
        } else {
            cpu_place_metric_values[cpu_runner_index] = -0xF00;
        }
    }
    if (cpu_place_metric_values[0] >= cpu_place_metric_values[1] && cpu_place_metric_values[0] >= cpu_place_metric_values[2]) {
        sports_controller_c_cpu_controllers[0].unk_01 = 1;
        return 2;
    }
    if (cpu_place_metric_values[1] >= cpu_place_metric_values[2]) {
        sports_controller_c_cpu_controllers[1].unk_01 = 1;
        return 3;
    }
    sports_controller_c_cpu_controllers[2].unk_01 = 1;
    return 4;
}

void sports_controller_c_cpu_frame_update(void) {
    sports_controller_c_cpu_velocity_target_update();
    sports_controller_c_cpu_motion_update();
    sports_controller_c_cpu_position_integrate();
    sports_controller_c_slot_animation_update();
}

void sports_controller_c_cpu_velocity_target_update(void) {
    s32 cpu_runner_index;
    Lim3 cpu_velocity_limits;

    cpu_velocity_limits = D_8014A5FC;
    for (cpu_runner_index = 0; cpu_runner_index < 3; cpu_runner_index++) {
        if (sports_runner_display_slots[cpu_runner_index + 1].unk_0C != 0) {
            sports_controller_c_cpu_controllers[cpu_runner_index].unk_10 = -0x4000;
        } else {
            sports_controller_c_cpu_controllers[cpu_runner_index].unk_10 = cpu_velocity_limits.v[cpu_runner_index];
        }
    }
}

void sports_controller_c_cpu_motion_update(void) {
    s32 i;
    Lim3 cpu_motion_position_limits;

    cpu_motion_position_limits = D_8014A608;
    for (i = 0; i < 3; i++) {
        if (sports_controller_c_cpu_controllers[i].unk_01 == 1 && D_80120896 + 0x140 < *(s16 *)(actor_pool + i * 0x44 + 0x19EA)) {
            sports_controller_c_cpu_controllers[i].unk_14 = 0;
        } else if (sports_controller_c_cpu_controllers[i].unk_0C >= 0xB) {
            sports_controller_c_cpu_controllers[i].unk_14 += sports_controller_c_cpu_controllers[i].unk_10;
            if (sports_controller_c_cpu_controllers[i].unk_14 < 0) {
                sports_controller_c_cpu_controllers[i].unk_14 = 0;
            }
            if (cpu_motion_position_limits.v[i] < sports_controller_c_cpu_controllers[i].unk_14) {
                sports_controller_c_cpu_controllers[i].unk_14 = cpu_motion_position_limits.v[i];
            }
            sports_controller_c_cpu_controllers[i].unk_0C = 0;
        }
    }
}


void sports_controller_c_cpu_position_integrate(void) {
    s32 i;
    s32 cpu_order_list_index;
    SportsController *v;
    u8 *a;
    s32 cpu_position_origin_offset;
    s32 cpu_actor_relative_position;

    for (i = 0, v = sports_controller_c_cpu_controllers, a = actor_pool, cpu_position_origin_offset = D_8014A3DE; i < 3; i++, v++, a += 0x44) {
        v->position.w += v->unk_14;
        cpu_actor_relative_position = v->position.h[1] - cpu_position_origin_offset;
        *(s16 *)(a + 0x19EA) = cpu_actor_relative_position - 0xA0;
        if (D_80120896 + (i + 1) * 8 < *(s16 *)(a + 0x19EA) + 0x40 && v->unk_01 == 0) {
            D_801208CC = 0x4F;
            cpu_order_list_index = 0;
            if (D_8014A3D8[0] != 0) {
                do {
                    cpu_order_list_index += 1;
                } while (D_8014A3D8[cpu_order_list_index] != 0);
            }
            D_8014A3D8[cpu_order_list_index] = i + 2;
            v->unk_01 = 1;
            cpu_actor_relative_position = v->position.h[1] - cpu_position_origin_offset;
        }
        if (cpu_actor_relative_position >= 0x1E1 || cpu_actor_relative_position < -0xA0) {
            a[0x19C7] = 0;
        } else {
            a[0x19C7] = 0xA4;
        }
    }
}

void sports_controller_c_slot_animation_update(void) {
    s32 i;
    s32 cpu_slot_sequence_offset;
    Actor *rec;

    for (i = 0; i < 3; i++) {
        switch (sports_runner_display_slots[i + 1].unk_0C) {
        case 0:
            if (sports_controller_c_cpu_controllers[i].unk_04 >= 6) {
                rec = &((Actor *)actor_pool)[i];
                cpu_slot_sequence_offset = i * 0x13 + 0x13;
                if (sports_controller_c_cpu_controllers[i].unk_00 >= 7) {
                    sports_controller_c_cpu_controllers[i].unk_00 = 2;
                    sports_controller_c_cpu_controllers[i].unk_04 = 0;
                } else {
                    sports_controller_c_cpu_controllers[i].unk_00 += 1;
                    sports_controller_c_cpu_controllers[i].unk_04 = 0;
                }
                if (sports_runner_display_slots[i + 1].unk_08 > 0) {
                    rec[97].unk_18 = D_8014A4D4[sports_controller_c_cpu_controllers[i].unk_00] + cpu_slot_sequence_offset;
                } else if (sports_runner_display_slots[i + 1].unk_08 < 0) {
                    rec[97].unk_18 = D_8014A4E4[sports_controller_c_cpu_controllers[i].unk_00] + cpu_slot_sequence_offset;
                } else {
                    rec[97].unk_18 = D_8014A4C4[sports_controller_c_cpu_controllers[i].unk_00] + cpu_slot_sequence_offset;
                }
            }
            break;
        case 1:
            if (sports_runner_display_slots[i + 1].motion_timer < 0x32) {
                ((Actor *)actor_pool)[97 + i].unk_18 = i * 2 + 0x53;
            }
            break;
        case 2:
            if (sports_runner_display_slots[i + 1].motion_timer < 0x32) {
                ((Actor *)actor_pool)[97 + i].unk_18 = i * 2 + 0x54;
            }
            break;
        default:
            game_mode_reset_to_title();
            break;
        }
    }
}

void sports_controller_c_frame_update(void) {
    s32 var_s0;

    sports_controller_c_motion_choice_update();
    var_s0 = 0;
    do {
        sports_controller_c_slot_position_step(var_s0);
        sports_controller_c_slot_actor_position_update(var_s0);
        var_s0 += 1;
    } while (var_s0 != 4);
}

void sports_controller_c_motion_choice_update(void) {
    sports_controller_c_motion_choices_update();
    sports_controller_c_player_direction_update();
}

void sports_controller_c_motion_choices_update(void) {
    s32 k;
    s32 i; /* i, k and r are declared before cpu_motion_adjustment_thresholds: it sits 12 bytes below the frame top */
    s32 r;
    Lim3 cpu_motion_adjustment_thresholds;

    cpu_motion_adjustment_thresholds = D_8014A614;
    i = 0;
    do {
        if (sports_runner_display_slots[i + 1].unk_14 >= 0x65) {
            r = rand() & 0x1F;
            if (r < 0xA) {
                sports_runner_display_slots[i + 1].unk_08 = -0x64;
            } else if (r < 0x14) {
                sports_runner_display_slots[i + 1].unk_08 = 0x64;
            } else {
                sports_runner_display_slots[i + 1].unk_08 = 0;
            }
            sports_runner_display_slots[i + 1].unk_14 = 0;
        }
        i++;
    } while (&D_8014A4A8 != &sports_runner_display_slots[i]); /* bounds of the loops below: the next table symbol, as in the original (T-16553) */
    k = 0; do { /* one line: as1 schedules the move after the hoisted table addresses (T-16553) */
        if (sports_runner_display_slots[k + 1].unk_18 >= 0xB) {
            i = 0;
            do {
                if (DISP(97 + k).unk_18 == D_8014A4D8[i] + (k * 0x13 + 0x13)) {
                    if (DISP(97 + k).x + 0x4F < DISP(101 + k).x && (rand() & 0xFF) < cpu_motion_adjustment_thresholds.v[k]) {
                        sports_runner_display_slots[k + 1].unk_08 -= 0x22;
                    }
                    if (DISP(101 + k).x < DISP(97 + k).x + 0x40 && (rand() & 0xFF) < cpu_motion_adjustment_thresholds.v[k]) {
                        sports_runner_display_slots[k + 1].unk_08 += 0x22;
                    }
                }
                i++;
            } while (&D_8014A4D8[i] < D_8014A4E4);
            i = 0;
            do {
                if (DISP(97 + k).unk_18 == D_8014A4E8[i] + (k * 0x13 + 0x13)) {
                    if (DISP(97 + k).x + 0x2D < DISP(101 + k).x && (rand() & 0xFF) < cpu_motion_adjustment_thresholds.v[k]) {
                        sports_runner_display_slots[k + 1].unk_08 -= 0x22;
                    }
                    if (DISP(101 + k).x < DISP(97 + k).x + 0x28 && (rand() & 0xFF) < cpu_motion_adjustment_thresholds.v[k]) {
                        sports_runner_display_slots[k + 1].unk_08 += 0x22;
                    }
                }
                i++;
            } while (&D_8014A4E8[i] < (u16 *)&D_8014A4F4);
            i = 0;
            do {
                if (DISP(97 + k).unk_18 == D_8014A4C4[i] + (k * 0x13 + 0x13)) {
                    if (DISP(97 + k).x + 0x2D < DISP(101 + k).x && (rand() & 0xFF) < cpu_motion_adjustment_thresholds.v[k]) {
                        sports_runner_display_slots[k + 1].unk_08 -= 0x22;
                    }
                    if (DISP(101 + k).x < DISP(97 + k).x + 0x28 && (rand() & 0xFF) < cpu_motion_adjustment_thresholds.v[k]) {
                        sports_runner_display_slots[k + 1].unk_08 += 0x22;
                    }
                }
                i++;
            } while (D_8014A4D4 != &D_8014A4C4[i]);
            sports_runner_display_slots[k + 1].unk_18 = 0;
        }
        k++;
    } while (k != 3);
}

void sports_controller_c_player_direction_update(void) {
    s32 player_direction_random_value;
    s32 player_direction_random_bonus;
    u32 player_direction_random_percentile;

    if ((sports_controller_c_player_controller.unk_14 > 0) || (D_8014A3E0 != 0)) {
        if (sports_runner_display_slots[0].unk_14 >= 0x65 && sports_runner_display_slots[0].unk_08 == 0) {
            player_direction_random_value = rand();
            player_direction_random_bonus = (sports_controller_c_player_controller.unk_14 == D_8014A3D4) ? 0xF : 0;
            player_direction_random_percentile = (player_direction_random_value % 100) & 0xFFFF;
            if (player_direction_random_percentile < player_direction_random_bonus + 0x19) {
                sports_runner_display_slots[0].unk_08 = -0x64;
                sports_runner_display_slots[0].unk_14 = 0;
            } else if (player_direction_random_percentile < player_direction_random_bonus + 0x32) {
                sports_runner_display_slots[0].unk_08 = 0x64;
                sports_runner_display_slots[0].unk_14 = 0;
            }
        }
        if ((game_state.pad_held & 0xA) && !(game_state.pad_held & 5)) {
            sports_runner_display_slots[0].unk_08 -= 0xB;
            if (sports_runner_display_slots[0].unk_08 == 0) {
                sports_runner_display_slots[0].unk_14 = 0;
            }
        }
        if (game_state.pad_held & 5) {
            if ((game_state.pad_held & 0xA) == 0) {
                sports_runner_display_slots[0].unk_08 += 0xB;
                if (sports_runner_display_slots[0].unk_08 == 0) {
                    sports_runner_display_slots[0].unk_14 = 0;
                }
            }
        }
    }
}

void sports_controller_c_slot_position_step(s32 index) {
    SportsRunnerDisplaySlot *slot;

    slot = &sports_runner_display_slots[index];
    /* FAKE: this empty test preserves the slot pointer's register range. T-10020. */
    if (!slot) {
    }
    if (slot->unk_0C == 0 && (index != 0 || sports_controller_c_player_controller.unk_10 > 0 || D_8014A3E0 != 0) && slot->cooldown >= 6) {
        if (slot->unk_08 > 0) {
            slot->unk_02++;
            slot->cooldown = 0;
        }
        if (slot->unk_08 < 0) {
            slot->cooldown = 0;
            slot->unk_02--;
        }
    }
}

void sports_controller_c_slot_actor_position_update(s32 index) {
    s32 i;
    Fix32 slot_actor_fixed_x;
    Fix32 slot_actor_fixed_y;
    TaiikuHalf10 sequence_group_a_y_offsets;
    TaiikuHalf10 sequence_group_a_x_offsets;
    TaiikuHalf8 sequence_group_b_y_offsets;
    TaiikuHalf8 sequence_group_c_y_offsets;

    sequence_group_a_y_offsets = D_8014A620;
    sequence_group_a_x_offsets = D_8014A634;
    sequence_group_b_y_offsets = D_8014A648;
    sequence_group_c_y_offsets = D_8014A658;
    slot_actor_fixed_x.w = ((ActorFixed *)actor_pool)[index + 96].x.w;
    slot_actor_fixed_x.w += *(s32 *)&sports_runner_display_slots[index].unk_00;
    slot_actor_fixed_y.w = ((ActorFixed *)actor_pool)[index + 96].y.w;
    slot_actor_fixed_y.w += *(s32 *)&sports_runner_display_slots[index].unk_04;
    i = 0;
    do {
        if (DISP(index + 96).unk_18 == D_8014A4C4[i] + index * 0x13) {
            slot_actor_fixed_y.h[1] += sequence_group_a_y_offsets.h[i];
            slot_actor_fixed_x.h[1] += sequence_group_a_x_offsets.h[i];
        }
        i++;
    } while (&D_8014A4C4[i] < D_8014A4D4);
    i = 2;
    do {
        if (DISP(index + 96).unk_18 == D_8014A4D4[i] + index * 0x13) {
            slot_actor_fixed_x.h[1] += 0x16;
            slot_actor_fixed_y.h[1] -= 4;
            slot_actor_fixed_y.h[1] += sequence_group_b_y_offsets.h[i];
        }
        i++;
    } while (&D_8014A4D4[i] < &D_8014A4D4[8]);
    for (i = 2; i < 8; i++) {
        if (DISP(index + 96).unk_18 == D_8014A4E4[i] + index * 0x13) {
            slot_actor_fixed_x.h[1] -= 0xC;
            slot_actor_fixed_y.h[1] -= 4;
            slot_actor_fixed_y.h[1] += sequence_group_c_y_offsets.h[i];
        }
    }
    DISP(index + 100).x = slot_actor_fixed_x.h[1];
    DISP(index + 100).y = slot_actor_fixed_y.h[1];
    for (index = 0; index < 3; index++) {
        if (sports_controller_c_cpu_controllers[index].position.h[1] - D_8014A3DE >= 0x1E1 || sports_controller_c_cpu_controllers[index].position.h[1] - D_8014A3DE < -0xA0) {
            DISP(101 + index).draw_flags = 0;
        } else {
            DISP(101 + index).draw_flags = 0xA4;
        }
    }
}

void sports_controller_c_slot_motion_update(void) {
    s32 var_s0;

    var_s0 = 0;
    do {
        sports_controller_c_slot_animation_threshold_check(var_s0);
        sports_controller_c_slot_move_finish(var_s0);
        var_s0 += 1;
    } while (var_s0 != 4);
}

void sports_controller_c_slot_animation_threshold_check(s32 arg0) {
    s32 i; /* declared before tbl: the stack table sits 4 bytes below the locals area top (T-3330, T-16553) */
    TaiikuHalf8 tbl;

    tbl = D_8014A668;
    if (sports_runner_display_slots[arg0].unk_0C == 0) {
        i = 0;
        do {
            if (DISP(arg0 + 96).unk_18 == D_8014A4C4[i] + arg0 * 0x13) {
                if (tbl.h[i] + DISP(arg0 + 96).x + 0x46 < DISP(arg0 + 100).x) {
                    sports_controller_c_slot_motion_a_start(arg0);
                }
                if (DISP(arg0 + 100).x < tbl.h[i] + DISP(arg0 + 96).x + 0x23) {
                    sports_controller_c_slot_motion_b_start(arg0);
                }
            }
            i++;
        } while (&D_8014A4C4[i] < D_8014A4D4); /* pointer bounds: the next table symbol, as in the original (sltu) */
        i = 0;
        do {
            if (DISP(arg0 + 96).unk_18 == D_8014A4D8[i] + arg0 * 0x13) {
                if (DISP(arg0 + 96).x + 0x5E < DISP(arg0 + 100).x) {
                    sports_controller_c_slot_motion_a_start(arg0);
                }
                if (DISP(arg0 + 100).x < DISP(arg0 + 96).x + 0x3B) {
                    sports_controller_c_slot_motion_b_start(arg0);
                }
            }
            i++;
        } while (&D_8014A4D8[i] < D_8014A4E4);
        i = 0;
        do {
            if (DISP(arg0 + 96).unk_18 == D_8014A4E8[i] + arg0 * 0x13) {
                if (DISP(arg0 + 96).x + 0x3C < DISP(arg0 + 100).x) {
                    sports_controller_c_slot_motion_a_start(arg0);
                }
                if (DISP(arg0 + 100).x < DISP(arg0 + 96).x + 0x19) {
                    sports_controller_c_slot_motion_b_start(arg0);
                }
            }
            i++;
        } while ((u16 *)&D_8014A4F4 != &D_8014A4E8[i]);
    }
}

void sports_controller_c_slot_motion_a_start(s32 arg0) {
    u8 *temp_v0;
    SportsRunnerDisplaySlot *temp_v1;

    temp_v1 = &sports_runner_display_slots[arg0];
    temp_v0 = (arg0 * 0x44) + actor_pool;
    temp_v1->unk_0C = 1;
    temp_v1->motion_timer = 0;
    *(s16 *)(temp_v0 + 0x1998) = (s16) ((arg0 * 2) + 0x51);
    *(s16 *)(temp_v0 + 0x1AB6) = (s16) (*(s16 *)(temp_v0 + 0x19A6) + 0x74);
    *(s16 *)(temp_v0 + 0x1ABA) = (s16) (*(s16 *)(temp_v0 + 0x19AA) + 0x32);
    temp_v1->unk_02 = 0x6A;
    temp_v1->unk_06 = 0x32;
    if (arg0 == 0) {
        audio_command(0x503);
    }
}

void sports_controller_c_slot_motion_b_start(s32 arg0) {
    u8 *temp_v0;
    SportsRunnerDisplaySlot *temp_v1;

    temp_v1 = &sports_runner_display_slots[arg0];
    temp_v0 = (arg0 * 0x44) + actor_pool;
    temp_v1->unk_0C = 2;
    temp_v1->motion_timer = 0;
    *(s16 *)(temp_v0 + 0x1998) = (s16) ((arg0 * 2) + 0x52);
    *(s16 *)(temp_v0 + 0x1AB6) = (s16) (*(s16 *)(temp_v0 + 0x19A6) - 0x17);
    *(s16 *)(temp_v0 + 0x1ABA) = (s16) (*(s16 *)(temp_v0 + 0x19AA) + 0x32);
    temp_v1->unk_02 = -0x17;
    temp_v1->unk_06 = 0x32;
    if (arg0 == 0) {
        audio_command(0x503);
    }
}

void sports_controller_c_slot_move_finish(s32 index) {
    SportsController *controller;
    SportsRunnerDisplaySlot *slot;

    slot = &sports_runner_display_slots[index];
    if (slot->unk_0C != 0 && slot->motion_timer >= 0x65U) {
        slot->unk_0C = 0;
        if (index == 0) {
            sports_controller_c_player_controller.unk_00 = 0;
            sports_controller_c_player_controller.unk_08 = 0;
            sports_controller_c_player_controller.unk_0C = 0;
            sports_controller_c_player_controller.unk_04 = 0;
            sports_controller_c_player_controller.unk_14 = 0;
        } else {
            controller = &sports_controller_c_cpu_controllers[index - 1];
            controller->unk_00 = 0;
            controller->unk_08 = 0;
            controller->unk_0C = 0;
            controller->unk_04 = 0;
            controller->unk_14 = 0;
        }
        slot->unk_02 = 0x37;
        slot->unk_06 = 0x19;
        slot->unk_08 = 0;
        slot->unk_14 = 0;
        slot->cooldown = 0;
        slot->motion_timer = 0;
    }
}

s32 sports_controller_c_result_actors_init(void) {
    s32 i;

    actor_clear_upper();
    for (i = 0; i < 9; i++) {
        DISP(96 + i).quad_flags = 0x01000000;
        DISP(96 + i).brightness = 0x80;
        DISP(96 + i).flags = 0x40;
        DISP(96 + i).draw_flags = 0xA4;
        DISP(96 + i).frame_table = (u8 *)D_80149FD8;
        DISP(96 + i).sequence_table = (u8 *)D_8014A010;
        DISP(96 + i).tpage_table = (u8 *)sports_controller_c_texture_page_buffer_ids_table_b;
        DISP(96 + i).unk_14 = D_8014A048;
        DISP(96 + i).unk_04 = 0;
        DISP(96 + i).unk_05 = 0x80;
        DISP(96 + i).scale_x = 0x1000;
        DISP(96 + i).scale_y = 0x1000;
        DISP(96 + i).blink_period = 2;
    }
    for (i = 0; i < 4; i++) {
        DISP(96 + i).unk_01 = 3;
        DISP(96 + i).unk_18 = D_8014A3D8[i] * 5 + 0x1D;
        DISP(96 + i).y = -8;
        DISP(96 + i).x = i * 0x30 - 0x70;
    }
    for (i = 0; i < 4; i++) {
        DISP(100 + i).unk_01 = 3;
        DISP(100 + i).unk_18 = D_8014A3D8[i] * 5 + 0xD;
        DISP(100 + i).x = DISP(96 + i).x;
        DISP(100 + i).y = DISP(96 + i).y;
    }
    DISP(104).unk_01 = 3;
    DISP(104).sequence_slot = 9;
    DISP(104).x = 0xC0;
    DISP(104).y = -0x20;
    DISP(104).flags = 1;
}

s32 sports_controller_c_result_actor_approach(void) {
    s32 index;
    s32 chosen;
    u8 *p;

    p = D_8014A3D8;
    index = 0;
    do {
        if (*p == 1) {
            chosen = index;
        }
        index++;
        p++;
    } while (index < 4);
    if (chosen * 48 - 64 < ((ActorFixed *)actor_pool)[104].x.h[1]) {
        ((ActorFixed *)actor_pool)[104].x.w += 0xFFFE8000;
        return 0;
    }
    ((ActorFixed *)actor_pool)[104].flags = 64;
    ((ActorFixed *)actor_pool)[104].frame = 61;
    Vblnk_Timer_Init();
    return 1;
}

s32 sports_controller_c_result_sequence_shift(void) {
    s32 i;
    s32 n;

    if (Vblnk_Timer() >= 0x2E) {
        ((Actor *)actor_pool)[104].unk_18 = 0x3C;
        ((Actor *)actor_pool)[104].y = -8;
        for (i = 0; i < 4; i++) {
            if (D_8014A3D8[i] == 1) {
                n = i;
            }
        }
        for (i = 0; i < 4; i++) {
            if (n < i) {
                ((Actor *)actor_pool)[i + 96].x += 0x30;
                *(s16 *)&actor_pool[0x19A6 + 0x110 + i * 0x44] += 0x30;
            }
        }
        return 1;
    }
    return 0;
}

void sports_controller_c_result_actor_motion_start(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        ((Actor *)actor_pool)[i + 100].unk_18 -= 4 - i;
    }
    Vblnk_Timer_Init();
}

void sports_controller_c_result_sequence_advance(void) {
    s32 i;
    s32 n;

    for (i = 0; i < 4; i++) {
        if (D_8014A3D8[i] == 1) {
            n = i;
        }
    }
    if (Vblnk_Timer() >= 0x33) {
        for (i = 0; i < 4; i++) {
            ((Actor *)actor_pool)[i + 96].unk_18 += i + 1;
        }
        ((Actor *)actor_pool)[104].unk_18 = ((Actor *)actor_pool)[104].unk_18 + n + 2;
        mode_step_next();
    }
}
