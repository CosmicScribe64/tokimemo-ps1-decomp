/* runner_race_dispatch.c: src/ovl/TAIIKU/80133C80.c:84,94,169; competition mode-stage dispatch, racer setup and update/input handlers; wiki/spec/o-taiiku.md records the observed runner behavior without claiming a fully decoded win rule. Original object 0x80133C80-0x80138950 (config/objects/TAIIKU.txt). */
#include "common.h"
#include "ovl/TAIIKU.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801491C0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801491C4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801491C8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801491CC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801491D4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801491D8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801491DC);
/* Defined locally so the four result bytes share the original store base. T-24008. */
u8 D_801491E0[4] = { 0 };
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801491E4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801491E8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801491EC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149204);
SportsRunnersA sports_runner_a_runners = { 0 };
TaiHalves D_80149298 = { 0 };
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_8014929C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801492A0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801492A4);
/* Defined in the unit (not an island piece) so that stores to it share one lui at; T-16551, sports_runner_race_state_init. */
u8 D_801492A8[4] = { 0 };
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801492AC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801492B8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801492C4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801492C8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801492DC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801492F4);
/* Defined in the unit (not an island piece) so that stores to it share one lui at; T-16551, sports_runner_race_resource_setup. */
s32 D_801492F8[10] = { 0x801BB000, 0x801BF000, 0x801C3000, 0x801C7000, 0x801CB000, 0x801D3000, 0x801D7000, 0x801DB000, 0x801DF000, 0x801E3000 };
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149320);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_8014933C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149364);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149368);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_8014937C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149398);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801493C0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801493D4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801493E8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801493FC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149414);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149440);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_8014946C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149498);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801494C4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801494F0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149508);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149530);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149558);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149570);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_8014964C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149728);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149754);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_8014976C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149784);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801497DC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801497F4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801497FC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149808);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149814);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_8014981C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_8014982C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_8014983C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_8014984C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_8014985C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149864);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149870);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149884);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149898);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801498AC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801498BC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_801498E8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.data", D_80149914);



typedef struct {
    void (*f[10])();
} FnTbl10; /* size 0x28 */
extern FnTbl10 D_8014933C;

void sports_runner_race_mode_dispatch(void) {
    s32 mode_stage_index; /* declared before runner_race_callbacks: its stack slot sits above runner_race_callbacks (T-3330) */
    s32 pad; /* FAKE: unused local; the original frame has a second word above runner_race_callbacks (two-word case, T-3330), real source unknown. T-4080 */
    FnTbl10 runner_race_callbacks;

    runner_race_callbacks = D_8014933C;
    mode_stage_index = game_state.mode_stage;
    runner_race_callbacks.f[mode_stage_index]();
}

void sports_runner_race_resource_setup(void) {
    s32 resource_index;
    s32 j; /* FAKE: unused local; the original frame has a second word above the tables (the offsets of sp9C.. only match with it), real source unknown. T-16551 */
    s32 sp9C;
    TaiikuW5 sp88;
    TaiWords7 sp6C;
    TaiWords10 sp44;

    sp9C = D_80149364;
    sp88 = D_80149368;
    sp6C = D_8014937C;
    sp44 = D_80149398;
    switch (game_state.mode_step) {
    case 0:
        scene_screen_reset(1, 1);
        actor_clear_range(0);
        hud_cursor_show(0);
        for (resource_index = 0; resource_index < 11; resource_index++) {
            game_state.tpage_buf[resource_index + 5] = 8;
        }
        for (resource_index = 0; resource_index < 3; resource_index++) {
            game_state.tpage_buf[resource_index + 0x25] = 8;
        }
        game_state.tpage_buf[28] = 8;
        game_state.tpage_buf[63] = 8;
        D_801492C4 = sp9C;
        for (resource_index = 0; resource_index < 5; resource_index++) {
            D_801492C8[resource_index] = sp88.v[resource_index];
        }
        for (resource_index = 0; resource_index < 7; resource_index++) {
            D_801492DC[resource_index] = sp6C.v[resource_index];
        }
        /* The first two words are written out and the loop starts at 2: IDO unrolls the 8-word copy by four, and this split
         * gives the original's single shared lui at for the two stores (D_801492F8 is defined in this unit for the same
         * reason, see matching-notes T-16551). */
        D_801492F8[0] = sp44.v[0];
        D_801492F8[1] = sp44.v[1];
        for (resource_index = 2; resource_index < 10; resource_index++) {
            D_801492F8[resource_index] = sp44.v[resource_index];
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
            cd_read_sectors(0x1F, 0x8018F000, 0xB79F);
            mode_step_next();
        }
        break;
    case 3:
        if (Fade_Out_Main() != 0 && (cd_read_sync_status() & 1)) {
            sports_scene_actor_resource_addresses_init();
            sports_candidate_model_resource_addresses_init();
            search_load_tpage_buf_lock(D_801492C4, 1);
            for (resource_index = 0; resource_index < 16; resource_index++) {
                load_palette(D_80149FA4, (u8) (resource_index + 0x10), 1, 1, resource_index);
            }
            sports_runner_race_state_init();
            sports_runner_sprite_memory_init();
            sports_runner_display_context_init();
            cd_read_sectors(0xC1, 0x8018F000, 0xB6DE);
            mode_stage_next();
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void sports_runner_race_scene_setup(void) {
    s32 i;
    s32 idx;
    s32 j; /* FAKE: unused upper word reproduces the original table homes. T-24521. */
    TaiShorts12 a;
    TaiShorts12 b;
    TaiShorts12 c;

    a = D_801493C0;
    b = D_801493D4;
    c = D_801493E8;
    switch (game_state.mode_step) {
    case 0:
        if (cd_read_sync_status() & 1) {
            idx = search_load_tpage_buf_lock(D_801492F4, 1);
            /* FAKE: unsigned handle view keeps the shared constant in a ugen temporary, matching the original ring. T-24521. */
            ((u32 *)game_state.tpage_buf)[idx] = 0x80000206;
            D_801492F4 = (s32)0x80000206;
            for (i = 0; i < 5; i++) {
                idx = search_load_tpage_buf_lock(D_801492C8[i], 1);
                game_state.tpage_buf[idx] = i + 0x80000100;
                D_801492C8[i] = i + 0x80000100;
            }
            for (i = 0; i < 6; i++) {
                idx = search_load_tpage_buf_lock(D_801492DC[i], 1);
                game_state.tpage_buf[idx] = i + 0x80000200;
                D_801492DC[i] = i + 0x80000200;
            }
            for (i = 0; i != 10; i++) {
                idx = search_load_tpage_buf_lock(D_801492F8[i], 1);
                game_state.tpage_buf[idx] = i + 0x80000300;
                D_801492F8[i] = i + 0x80000300;
            }
            mode_step_next();
        }
        break;
    case 1:
        Fade_In_Init();
        audio_command(0x204);
        mode_step_next();
        break;
    case 2:
        if (Fade_In_Main() != 0) {
            mode_step_next();
        }
        if (D_80120652 == 0) {
            idx = rand();
            D_80120652 = 5;
            D_80120653 = 0xA4;
            idx %= 9;
            D_80120676 = a.value[idx];
            D_8012067A = b.value[idx];
            D_80120668 = 0;
            D_80120666 = c.value[idx];
        }
        if (game_state.pad_pressed & 0xF0) {
            Fade_Out_Init();
            mode_step_set(4);
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        break;
    case 3:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if (D_80120652 == 0) {
            idx = rand();
            D_80120652 = 5;
            D_80120653 = 0xA4;
            idx %= 9;
            D_80120676 = a.value[idx];
            D_8012067A = b.value[idx];
            D_80120668 = 0;
            D_80120666 = c.value[idx];
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
            actor_clear(0x60);
            game_state.tpage_buf[21] = -1;
            sports_candidate_scene_layer_resource_addresses_init();
            for (i = 0; i < 6; i++) {
                search_load_tpage_buf_lock(D_8014A070[i], 1);
            }
            mode_step_next();
        }
        break;
    case 6:
        sports_runner_scene_graphics_init();
        sports_dance_actors_init();
        sports_runner_actor_group_init();
        sports_runner_obstacle_actor_init();
        sports_runner_obstacle_sequences_assign();
        sports_runner_animation_and_rival_select();
        audio_command(0x505);
        Fade_In_Init();
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void sports_runner_race_phase_one(void) {
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
            audio_bank_wait_done = 0;
            audio_bank_load_start(0, 0xBF98, 0xBF79, 0xC107, 0xC0C2, 0xD6B7);
            mode_step_next();
        }
        sports_runner_scene_scroll_update();
        break;
    case 2:
        audio_bank_wait_latch();
        sports_runner_actor_timer_increment();
        sports_runner_player_control_update();
        sports_runner_scroll_update();
        sports_dance_actors_position((s32) D_80149298.halves.high, 3);
        sports_runner_finish_line_update();
        sports_runner_obstacle_scroll_update();
        sports_runner_cpu_update();
        sports_runner_finish_actor_display_update();
        sports_runner_finish_audio_and_lane_update();
        if (D_80149298.halves.high >= 0x140) {
            mode_stage_next();
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void sports_runner_race_phase_two(void) {
    s16 text_x;
    s16 panel_x;
    s32 index_home; /* FAKE: preserves the original table offsets after index inlining. T-10890. */
    TaiikuS11 sprites;
    TaiText11 first_text;
    TaiText11 second_text;
    TaiWords11 delta_x;
    TaiWords11 delta_y;

    sprites = D_801493FC;
    first_text = D_80149414;
    second_text = D_80149440;
    delta_x = D_8014946C;
    delta_y = D_80149498;
    switch (game_state.mode_step) {
    case 0:
        audio_bank_wait_latch();
        if ((audio_bank_wait_done == 1) && (D_801491D4 == 0)) {
            D_801491D4 = 1;
            audio_command(0x1605);
            audio_command(0x700);
        }
        sports_runner_actor_timer_increment();
        sports_runner_a_runners.player.unk_18 = ((s32) D_801491E8 >> 1) + 1;
        sports_runner_a_runners.player.speed = 1;
        sports_runner_player_animation_update();
        sports_runner_cpu_update();
        sports_runner_finish_actor_display_update();
        Scroll((s32) ((s16) D_80149298.halves.high >> 1) % 640, 0xF, 0, 0x70, 0x280);
        Scroll(((s16) D_80149298.halves.high >> 1) & 0x7F, 0xF, 0x70, 0x80, 0x80);
        sports_dance_actors_position((s32) D_80149298.halves.high, 3);
        sports_runner_finish_line_update();
        sports_runner_finish_audio_and_lane_update();
        sports_runner_obstacle_scroll_update();
        D_80149298.word += 0x18000;
        ((Actor *)actor_pool)[96].x--;
        if (((Actor *)actor_pool)[96].x < -0xAF) {
            ((Actor *)actor_pool)[96].x = -0xB0;
        }
        if (D_80149298.halves.high >= 0x280) {
            audio_command(0x606);
            ((Actor *)actor_pool)[96].unk_18 = 0x87;
            ((Actor *)actor_pool)[106].draw_flags = 0xA4;
            ((Actor *)actor_pool)[106].flags = 0x40;
            ((Actor *)actor_pool)[104].flags = 0x40;
            ((Actor *)actor_pool)[104].draw_flags = 0xA4;
            ((Actor *)actor_pool)[105].flags = 0x40;
            ((Actor *)actor_pool)[105].draw_flags = 0xA4;
            ((Actor *)actor_pool)[105].unk_18 = sprites.v[D_801491EC];
            set_kanji_string((s16) (((Actor *)actor_pool)[96].x + 0x40), 0x40, 0, first_text.value[D_801491EC], 0);
            set_kanji_string((s16) (((Actor *)actor_pool)[96].x + 0x48), 0x50, 0, second_text.value[D_801491EC], 0);
            k_disp_start(2);
            text_x = ((Actor *)actor_pool)[96].x + 0x30;
            ((Actor *)actor_pool)[105].x = text_x + 0x50;
            ((Actor *)actor_pool)[104].x = text_x;
            ((TextEntry *)get_k_work(0))->x = ((Actor *)actor_pool)[96].x + 0x40;
            ((TextEntry *)get_k_work(1))->x = ((Actor *)actor_pool)[96].x + 0x48;
            mode_step_next();
        }
        break;
    case 1:
        if ((audio_bank_wait_done != 0) && (D_801491D4 == 0)) {
            D_801491D4 = 1;
            audio_command(0x1605);
            audio_command(0x700);
        }
        sports_runner_actor_timer_increment();
        sports_runner_cpu_update();
        sports_runner_finish_actor_display_update();
        sports_runner_rival_choice_input_process();
        sports_runner_obstacle_scroll_update();
        Scroll((s32) ((s16) D_80149298.halves.high >> 1) % 640, 0xF, 0, 0x70, 0x280);
        Scroll(D_80149298.halves.high & 0x7F, 0xF, 0x70, 0x80, 0x80);
        sports_dance_actors_position((s32) D_80149298.halves.high, 3);
        sports_runner_finish_line_update();
        if (sports_runner_a_runners.player.unk_01 != 0) {
            mode_step_next();
        }
        break;
    case 2:
        sports_runner_actor_timer_increment();
        sports_runner_cpu_update();
        sports_runner_finish_actor_display_update();
        sports_runner_obstacle_scroll_update();
        Scroll((s32) ((s16) D_80149298.halves.high >> 1) % 640, 0xF, 0, 0x70, 0x280);
        Scroll(D_80149298.halves.high & 0x7F, 0xF, 0x70, 0x80, 0x80);
        sports_runner_finish_line_update();
        if (sports_runner_finish_actor_slide_update(0) != 0) {
            sports_runner_a_runners.player.unk_01 = 0U;
            sports_runner_a_runners.player.unk_00 = 1;
            if (D_80122E94 == 1) {
                D_801491E8 += delta_x.value[D_80149320[D_801491DC]];
                D_801491E4 += delta_y.value[D_80149320[D_801491DC]];
            } else {
                D_801491E8 += delta_x.value[D_801491EC];
                D_801491E4 += delta_y.value[D_801491EC];
            }
            mode_stage_next();
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void sports_runner_course_end_transition(void) {
    s32 var_s0;

    switch (game_state.mode_step) {
    case 0:
        sports_runner_actor_timer_increment();
        sports_runner_player_control_update();
        sports_runner_obstacle_scroll_update();
        sports_runner_cpu_update();
        sports_runner_finish_actor_display_update();
        sports_runner_scroll_update();
        sports_dance_actors_position((s32) D_80149298.halves.high, 3);
        sports_runner_finish_line_update();
        if (D_80149298.halves.high >= 0x401) {
            audio_command(0x1700);
            audio_command(0x605);
            mode_step_next();
        }
        break;
    case 1:
        sports_runner_actor_timer_increment();
        sports_runner_player_control_update();
        sports_runner_cpu_update();
        sports_runner_finish_actor_display_update();
        sports_dance_actors_position((s32) D_80149298.halves.high, 3);
        sports_runner_scroll_update();
        if (sports_runner_finish_line_update() != 0) {
            audio_command(0x604);
            Fade_Out_Init();
            mode_step_next();
        }
        break;
    case 2:
        Scroll((s32) ((s16) D_80149298.halves.high >> 1) % 640, 0xF, 0, 0x70, 0x280);
        Scroll(D_80149298.halves.high & 0x7F, 0xF, 0x70, 0x80, 0x80);
        sports_dance_actors_position((s32) D_80149298.halves.high, 3);
        if (Fade_Out_Main() != 0) {
            audio_command(0xC2);
            var_s0 = 0;
            do {
                actor_clear(var_s0);
                var_s0 += 1;
            } while (var_s0 < 0x17);
            var_s0 = 0;
            do {
                actor_clear(var_s0 + 0x4C);
                var_s0 += 1;
            } while (var_s0 != 0xA);
            sports_runner_finish_actor_group_init();
            mode_stage_next();
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void sports_runner_race_result_apply(void) {
    s32 j; /* FAKE: upper word preserves the table and counter spill homes. T-24521. */
    s32 i;
    TaiWords11 table;

    table = D_801494C4;
    Scroll(0, 0xF, 0, 0x70, 0x280);
    Scroll(0, 0xF, 0x70, 0x80, 0x80);
    switch (game_state.mode_step) {
    case 0:
        audio_command(0x60B);
        Fade_In_Init();
        mode_step_next();
        break;
    case 1:
        if (Fade_In_Main() != 0) {
            if ((D_801491E0[0] == 1) || (D_801491E0[1] == 1)) {
                audio_bank_start(1, 0x205);
            } else {
                audio_bank_start(1, 0x206);
            }
            sports_runner_a_runners.player.animation_frame = 0;
            sports_runner_a_runners.player.unk_0C = 0;
            mode_step_next();
        }
        break;
    case 2:
        sports_runner_a_runners.player.unk_0C += 1;
        sports_runner_finish_animation_update();
        for (i = 0; i < 4; i++) {
            ((Actor *)actor_pool)[96 + i].x -= 1;
        }
        for (i = 0; i < 4; i++) {
            ((Actor *)actor_pool)[107 + i].x -= 1;
        }
        if ((((Actor *)actor_pool)[107].x < -0x9F) || (((Actor *)actor_pool)[108].x < -0x9F) || (((Actor *)actor_pool)[109].x < -0x9F) || (((Actor *)actor_pool)[110].x < -0x9F)) {
            if (D_80122E94 == 1) {
                sports_candidate_scene_audio_request();
            } else {
                for (i = 0; i != 4; i++) {
                    if (D_801491E0[i] == 1) {
                        sports_scene_audio_sequence_request(table.value[D_801491EC], (u8)i);
                    }
                }
            }
            Vblnk_Timer_Init();
            mode_step_next();
        }
        break;
    case 3:
        sports_runner_finish_animation_update();
        if (D_80122E94 == 1) {
            if (D_80120ABA < -0x58) {
                D_80120ABA += 4;
            } else if (D_80120ABA == -0x58) {
                audio_command(0x50D);
                D_80120ABA = -0x54;
            }
        }
        if (Vblnk_Timer() >= 0x79) {
            mode_step_next();
        }
        break;
    case 4:
        if (cd_audio_state() == 2) {
            Vblnk_Timer_Init();
            mode_step_next();
        }
        break;
    case 5:
        if (Vblnk_Timer() >= 0x79) {
            Fade_Out_Init();
            mode_step_next();
        }
        break;
    case 6:
        if (Fade_Out_Main() != 0) {
            if ((u8) D_801491DC < 0xBU) {
                game_state.girls[D_801491DC].affection += 2;
            } else {
                game_state.girls[table.value[D_801491EC]].affection += 1;
            }
            if (D_801491E0[0] == 1) {
                game_state.girls[0].tokimeki += 6;
                for (i = 1; i < 13; i++) {
                    game_state.girls[i].tokimeki += 6;
                }
            } else if (D_801491E0[1] == 1) {
                game_state.girls[0].tokimeki += 3;
                for (i = 1; i < 13; i++) {
                    game_state.girls[i].tokimeki += 3;
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


/* FAKE: T-24133. `u32 raw` with `if (raw) {}` and `raw = 0` keeps the raw flags byte in $a0 and its `& 0xF` mask in $v1
 * without a J<-L widening CVT; sharing `GameState *t` for the `unk_02` read and packing `s16 v` above `sp38` keeps the
 * 0x58 frame and loads `unk_02` directly into $v0. */
void sports_runner_race_state_init(void) {
    s32 i;
    GameState *t;
    s16 v;
    TaiikuS11 sp38;
    u32 raw;

    sp38 = D_801494F0;
    for (i = 0; i < 11; i++) {
        D_80149320[i] = sp38.v[i];
    }
    /* FAKE: indexes the four runners (player, cpu[3]) from the player record, as in sports_runner_actor_timer_increment. T-16551 */
    for (i = 0; i < 4; i++) {
        D_801491E0[i] = 0;
        (&sports_runner_a_runners.player)[i].track_position.halves.high = i * 8;
        (&sports_runner_a_runners.player)[i].unk_00 = 0;
        (&sports_runner_a_runners.player)[i].unk_01 = 0;
        (&sports_runner_a_runners.player)[i].display_actor_index = 0;
        (&sports_runner_a_runners.player)[i].unk_06 = 0;
        (&sports_runner_a_runners.player)[i].animation_frame = 0;
        (&sports_runner_a_runners.player)[i].unk_04 = 0;
        (&sports_runner_a_runners.player)[i].unk_0C = 0;
        (&sports_runner_a_runners.player)[i].unk_0E = 0;
        (&sports_runner_a_runners.player)[i].speed = 0;
        (&sports_runner_a_runners.player)[i].unk_14 = 0;
        (&sports_runner_a_runners.player)[i].unk_18 = 0;
        (&sports_runner_a_runners.player)[i].unk_1C = 0;
        (&sports_runner_a_runners.player)[i].track_position.halves.low = 0;
        (&sports_runner_a_runners.player)[i].start_sequence_state = 0;
        (&sports_runner_a_runners.player)[i].unk_08 = 0;
        (&sports_runner_a_runners.player)[i].unk_0A = 0;
        (&sports_runner_a_runners.player)[i].unk_07 = 0;
    }
    sports_runner_a_runners.player.track_position.halves.high = -0xA0;
    D_80149204 = 0;
    D_801491D4 = 0;
    D_8014929C = 0;
    D_80149298.word = 0;
    D_801492A0 = 0;
    t = &game_state;
    raw = t->player_flags.b[1];
    if (raw) {}
    v = t->param_athletics.value;
    t = (GameState *)(raw & 0xF);
    raw = 0;
    if (((u32)t & 1) || ((u32)t & 2)) {
        v = (s16) (v >> 1);
    }
    if (v >= 0xA0) {
        D_801491E4 = 0x280;
        D_801491E8 = 0x20000;
    } else if (v >= 0x7D) {
        D_801491E4 = 0x220;
        D_801491E8 = 0x1E000;
    } else if (v >= 0x64) {
        D_801491E4 = 0x1C0;
        D_801491E8 = 0x1C000;
    } else if (v >= 0x4B) {
        D_801491E4 = 0x160;
        D_801491E8 = 0x1A000;
    } else {
        D_801491E4 = 0x100;
        D_801491E8 = 0x18000;
    }
    srand(game_state.timer_ticks);
    for (i = 0; i < 2; i++) {
        D_801492A4[i] = 0xFF;
    }
    D_801492A8[0] = 0;
    D_801492A8[1] = 0x3C;
    D_80122E94 = 0xFF;
    D_801491DC = 0xFF;
}

void sports_runner_sprite_memory_init(void) {
    s32 i;

    layer_tileset_init(0, D_80149FAC, D_80149FA0);
    srn_init(0, D_80149FA8, 0);
    srn_vram_set(0, 5, 0x580, 0xF0);
    for (i = 0; i < 3; i++) {
        game_state.tpage_buf[i + 5] = 8;
    }
    for (i = 0; i < 3; i++) {
        game_state.tpage_buf[i + 0x25] = 8;
    }
}

void sports_runner_display_context_init(void) {
    actor_clear(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120657 = 0x80;
    D_8012065C = (u8 *) D_80149FB0;
    D_80120660 = (u8 *) D_80149FE8;
    D_80120684 = (u8 *)&D_801492C4;
    D_80120664 = D_8014A020;
    D_80120654 = 0;
    D_80120655 = 0x80;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
}

void sports_runner_actor_group_init(void) {
    s32 i;
    TaiShorts20 sp84;
    TaiShorts20 sp5C;

    sp84 = D_80149508;
    sp5C = D_80149530;
    for (i = 0; i < 20; i++) {
        actor_clear(i + 0x60);
        ((Actor *)actor_pool)[96 + i].quad_flags = 0x01000000;
        ((Actor *)actor_pool)[96 + i].brightness = 0x80;
        ((Actor *)actor_pool)[96 + i].frame_table = (u8 *)D_80149FB8;
        ((Actor *)actor_pool)[96 + i].sequence_table = (u8 *)D_80149FF0;
        ((Actor *)actor_pool)[96 + i].tpage_table = (u8 *)D_801492DC;
        ((Actor *)actor_pool)[96 + i].unk_14 = D_8014A028;
        ((Actor *)actor_pool)[96 + i].y = sp84.v[i];
        ((Actor *)actor_pool)[96 + i].unk_18 = sp5C.v[i];
        ((Actor *)actor_pool)[96 + i].unk_04 = 0;
        ((Actor *)actor_pool)[96 + i].unk_05 = 0x80;
        ((Actor *)actor_pool)[96 + i].scale_x = 0x1000;
        ((Actor *)actor_pool)[96 + i].scale_y = 0x1000;
        ((Actor *)actor_pool)[96 + i].blink_period = 2;
    }
    for (i = 0; i < 4; i++) {
        ((Actor *)actor_pool)[96 + i].x = -0xA0 + i * 8;
        ((Actor *)actor_pool)[96 + i].unk_01 = i + 4;
        ((Actor *)actor_pool)[96 + i].flags = 0x40;
        ((Actor *)actor_pool)[96 + i].draw_flags = 0xA4;
    }
    for (i = 0; i < 4; i++) {
        ((Actor *)actor_pool)[100 + i].unk_01 = 8;
        ((Actor *)actor_pool)[100 + i].x = 0xE8;
        ((Actor *)actor_pool)[100 + i].flags = 0x40;
        ((Actor *)actor_pool)[100 + i].draw_flags = 0xA4;
    }
    /* FAKE: dead store; with `i` assigned again IDO keeps the 4-trip loop above rolled and replaces its test by the
     * record-walk compare (`sltu` against D_8011EDE0, walk in $s0). Real source unknown. T-19004 */
    i = 0;
    ((Actor *)actor_pool)[104].unk_01 = 3;
    ((Actor *)actor_pool)[105].unk_01 = 2;
    ((Actor *)actor_pool)[106].unk_01 = 1;
    ((Actor *)actor_pool)[107].unk_01 = 4;
    ((Actor *)actor_pool)[108].unk_01 = 5;
    ((Actor *)actor_pool)[109].unk_01 = 6;
    ((Actor *)actor_pool)[110].unk_01 = 7;
    ((Actor *)actor_pool)[111].unk_01 = 9;
    ((Actor *)actor_pool)[112].unk_01 = 8;
    ((Actor *)actor_pool)[113].unk_01 = 9;
    ((Actor *)actor_pool)[114].unk_01 = 9;
    ((Actor *)actor_pool)[114].x = -0x30;
    ((Actor *)actor_pool)[114].flags = 0x40;
    ((Actor *)actor_pool)[114].draw_flags = 0xA4;
    ((Actor *)actor_pool)[0].x = 0;
    ((Actor *)actor_pool)[0].y = 0;
}

void sports_runner_obstacle_actor_init(void) {
    s32 i;
    s32 count;
    s32 row;
    s32 available[11];
    s32 temp;
    TaiikuS11 characters;

    characters = D_80149558;
    for (i = 0; i < 11; i++) {
        available[i] = 128;
    }
    count = 0;
    for (i = 0; i < 11; i++) {
        if (((game_state.girls[i].flags.u << 30) >> 31) == 1) {
            available[count++] = i;
        }
    }
    D_801491EC = characters.v[available[rand() % count]];
    for (i = 0; i < 30; i++) {
        if (i < 10) {
            row = 0;
        } else if (i < 20) {
            row = 1;
        } else {
            row = 2;
        }
        actor_clear(i + 46);
        ((Actor *)actor_pool)[i + 46].quad_flags = 0x01000000;
        temp = row + 9;
        ((Actor *)actor_pool)[i + 46].unk_01 = temp;
        ((Actor *)actor_pool)[i + 46].unk_04 = 0;
        ((Actor *)actor_pool)[i + 46].unk_05 = 128;
        ((Actor *)actor_pool)[i + 46].blink_period = 2;
        ((Actor *)actor_pool)[i + 46].unk_08 = 0;
        ((Actor *)actor_pool)[i + 46].frame_table = (u8 *)D_80149FB4;
        ((Actor *)actor_pool)[i + 46].sequence_table = (u8 *)D_80149FEC;
        ((Actor *)actor_pool)[i + 46].tpage_table = (u8 *)D_801492C8;
        ((Actor *)actor_pool)[i + 46].unk_14 = (s16)D_8014A024;
        ((Actor *)actor_pool)[i + 46].scale_x = 0x1000;
        ((Actor *)actor_pool)[i + 46].scale_y = 0x1000;
        ((Actor *)actor_pool)[i + 46].brightness = 128;
        ((Actor *)actor_pool)[i + 46].y = -48 - row * 16;
        ((Actor *)actor_pool)[i + 46].sequence_slot = sports_runner_obstacle_sequence_select();
        while (((Actor *)actor_pool)[i + 46].sequence_slot == ((Actor *)actor_pool)[i + 45].sequence_slot) {
            ((Actor *)actor_pool)[i + 46].sequence_slot = sports_runner_obstacle_sequence_select();
        }
        ((Actor *)actor_pool)[i + 46].unk_18 = 0;
    }
}

s32 sports_runner_obstacle_sequence_select(void) {
    s32 row;
    s32 random;
    SportsRandomTable table;

    table = D_80149570;
    random = rand() % 11;
    row = random;
    if (random >= D_801491EC) {
        row = random + 1;
    }
    if (row >= 11) {
        row = rand() % 10;
    }
    random = rand();
    return table.value[row][random % 10];
}

void sports_runner_obstacle_sequences_assign(void) {
    s32 index;
    s32 row;
    s32 column;
    s32 skip;
    SportsRandomTable table;

    table = D_8014964C;
    index = 0;
    for (row = 0; row < 10; row++) {
        skip = rand() % 3;
        for (column = 0; column < 3; column++) {
            if (skip != column) {
                ((Actor *)actor_pool)[row * 3 + column + 46].sequence_slot = table.value[D_801491EC][index];
                index++;
                if (index >= 10) {
                    index = 0;
                }
            }
        }
    }
}

/* Display records of the 0x44-byte pool. T-25106. */
#define DISP(n) (((Actor *)actor_pool)[n])
/* FAKE: record of runner index n (SportsRunnerA.display_actor_index), counted from record 46 instead of from the
 * pool start: the u8 index stays unsigned and every product shares one 0x44 constant, as in the
 * original. The pool is one array; the real source is unknown. T-25106 */
#define RUNNER_DISP(n) ((&DISP(46))[n])
/* FAKE: runner n (0 the player) indexed from the player record; the object is player + cpu[3],
 * not one array. As in sports_runner_finish_display_selector and sports_runner_actor_timer_increment. T-25106 */
#define RUN(n) ((&sports_runner_a_runners.player)[n])
/* Picks a random display record for each runner: the player's from 10..29, the others' from
 * 0..29, never 19, and no two runners (or the extra record D_801491D8) with the same unk_16 value.
 * The comparisons read the records directly, with no local for the value: uopt's PRE then keeps
 * one value per test in the original's registers. FAKE: `pad` is an unused third word above the
 * copied tables, where the original frame keeps 12 bytes. T-25106 */
void sports_runner_animation_and_rival_select(void) {
    s32 i;
    s32 j;
    s32 pad;
    TaiWords11 choices;
    TaiikuS11 player_codes;
    TaiikuS11 rival_codes;
    TaiFlags58 flags;

    choices = D_80149728;
    player_codes = D_80149754;
    rival_codes = D_8014976C;
    flags = D_80149784;
    sports_runner_a_runners.player.display_actor_index = rand() % 20 + 10;
    while (sports_runner_a_runners.player.display_actor_index == 19) {
        sports_runner_a_runners.player.display_actor_index = rand() % 20 + 10;
    }
    RUNNER_DISP(sports_runner_a_runners.player.display_actor_index).sequence_slot = player_codes.v[D_801491EC];
    for (i = 0; i < 3; i++) {
        sports_runner_a_runners.cpu[i].display_actor_index = rand() % 30;
        while (sports_runner_a_runners.cpu[i].display_actor_index == 19) {
            sports_runner_a_runners.cpu[i].display_actor_index = rand() % 30;
        }
    }
    D_801491DC = select_girl_by_scores();
    if (girl_state_rank(D_801491DC) < 2 && D_801491DC != choices.value[D_801491EC]) {
        D_801491D8 = rand() % 10;
        RUNNER_DISP(D_801491D8).sequence_slot = rival_codes.v[D_801491DC];
        DISP(115).sequence_slot = RUNNER_DISP(D_801491D8).sequence_slot >> 3;
        DISP(115).unk_43 = flags.value[RUNNER_DISP(D_801491D8).sequence_slot >> 3][RUNNER_DISP(D_801491D8).sequence_slot & 7];
    } else {
        D_801491DC = 0xFF;
    }
    if (D_801491DC == choices.value[D_801491EC]) {
        D_801491DC = 0xFF;
    }
    for (i = 0; i < 3; i++) {
        for (j = 1; j <= i + 1; j++) {
            while (RUNNER_DISP(sports_runner_a_runners.cpu[i].display_actor_index).sequence_slot == RUNNER_DISP(RUN(i + 1 - j).display_actor_index).sequence_slot
                   || RUNNER_DISP(sports_runner_a_runners.cpu[i].display_actor_index).sequence_slot == RUNNER_DISP(D_801491D8).sequence_slot) {
                sports_runner_a_runners.cpu[i].display_actor_index = rand() % 30;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        DISP(107 + i).sequence_slot = RUNNER_DISP(RUN(i).display_actor_index).sequence_slot >> 3;
    }
    for (i = 0; i < 4; i++) {
        DISP(107 + i).unk_43 = flags.value[RUNNER_DISP(RUN(i).display_actor_index).sequence_slot >> 3][RUNNER_DISP(RUN(i).display_actor_index).sequence_slot & 7];
    }
}
#undef RUN
#undef RUNNER_DISP
#undef DISP

void sports_runner_obstacle_scroll_update(void) {
    s32 i;
    s32 j;
    s32 span;

    for (i = 0; i < 2; i++) {
        ++D_801492A8[i];
        if (D_801492A8[i] >= 121U) {
            D_801492A8[i] = 0;
        } else if (D_801492A8[i] == 10) {
            D_801492A4[i] = rand() % 20 + 10;
        } else if (D_801492A8[i] == 27) {
            D_801492A4[i] = 0xFF;
        } else if (D_801492A8[i] >= 19U && D_801492A4[i] != 0xFF) {
            ((Actor *)actor_pool)[46 + D_801492A4[i]].y++;
        } else if (D_801492A8[i] >= 11U && D_801492A4[i] != 0xFF) {
            ((Actor *)actor_pool)[46 + D_801492A4[i]].y--;
        }
    }
    for (i = 0, span = 0; i < 30; i++, span += 32) {
        if (span >= 320) {
            span -= 320;
        }
        ((Actor *)actor_pool)[46 + i].x = span + 480 - D_80149298.halves.high;
        if (i >= 10 && i < 20) {
            ((Actor *)actor_pool)[46 + i].x += 16;
        }
        if (i == 19) {
            ((Actor *)actor_pool)[46 + i].flags = 0;
            ((Actor *)actor_pool)[46 + i].draw_flags = 0;
        } else if (((Actor *)actor_pool)[46 + i].x >= -207 && ((Actor *)actor_pool)[46 + i].x < 321) {
            ((Actor *)actor_pool)[46 + i].flags = 1;
            ((Actor *)actor_pool)[46 + i].draw_flags = 0xA4;
        } else {
            ((Actor *)actor_pool)[46 + i].flags = 0;
            ((Actor *)actor_pool)[46 + i].draw_flags = 0;
        }
        /* FAKE: contiguous four-runner view and empty constant comparisons to order loop constants. */
        if ((u32)i == 1U) {}
        if ((u32)i == 0xA4U) {}
        if (i == 1) {}
        if (i == 4) {}
        if (i == 0xFF) {}
        j = 0;
        while (1) {
            if (D_801491D8 == i && ((Actor *)actor_pool)[107].flags != 0 && (&D_80122E94)[0] == 1) {
                ((Actor *)actor_pool)[46 + i].flags = 0;
                ((Actor *)actor_pool)[46 + i].draw_flags = 0;
            } else if (i == (&sports_runner_a_runners.player)[j].display_actor_index && ((Actor *)actor_pool)[107 + j].flags != 0) {
                if (j == 0) {
                    if ((&D_80122E94)[0] == 0xFF) {
                        ((Actor *)actor_pool)[46 + i].flags = 0;
                        ((Actor *)actor_pool)[46 + i].draw_flags = 0;
                    }
                } else {
                    ((Actor *)actor_pool)[46 + i].flags = 0;
                    ((Actor *)actor_pool)[46 + i].draw_flags = 0;
                }
            }
            if (i == (&sports_runner_a_runners.player)[j].display_actor_index && ((Actor *)actor_pool)[107 + j].flags == 0) {
                ((Actor *)actor_pool)[46 + i].flags = 1;
                ((Actor *)actor_pool)[46 + i].draw_flags = 0xA4;
            }
            j++;
            if (j == 4) {
                break;
            }
        }
    }
}

s32 sports_runner_actor_timer_increment() {
    s32 i;

    /* FAKE: indexes the four runners (player, cpu[3]) from the player record; the object is player + cpu[3], not one array.
     * IDO unrolls the loop, and the peeled first runner is what makes the scheduling match. T-16551 */
    for (i = 0; i < 4; i++) {
        (&sports_runner_a_runners.player)[i].unk_0C += 1;
        (&sports_runner_a_runners.player)[i].unk_0E += 1;
        (&sports_runner_a_runners.player)[i].unk_14 += 1;
        (&sports_runner_a_runners.player)[i].unk_1C += 1;
    }
    sports_runner_a_runners.player.unk_07 += 1;
}

void sports_runner_player_control_update(void) {
    sports_runner_player_speed_input_update();
    sports_runner_player_position_integrate();
    sports_runner_player_screen_position_update();
    sports_runner_player_animation_update();
}

void sports_runner_player_speed_input_update(void) {
    if (game_state.pad_pressed & PAD_CIRCLE) {
        D_80149204 = 1;
    }
    if (sports_runner_a_runners.player.unk_14 >= 0x10U) {
        if (D_80149204 != 0) {
            if (sports_runner_a_runners.player.unk_18 < 0) {
                sports_runner_a_runners.player.unk_18 = 0;
            }
            sports_runner_a_runners.player.speed = D_801491E4;
        } else {
            sports_runner_a_runners.player.speed = -0x300;
        }
        sports_runner_a_runners.player.unk_14 = 0;
        D_80149204 = 0;
    }
}

void sports_runner_player_position_integrate(void) {
    if ((u32)sports_runner_a_runners.player.unk_1C >= 0xB) {
        sports_runner_a_runners.player.unk_18 += sports_runner_a_runners.player.speed;
        if (sports_runner_a_runners.player.unk_18 < -0x10000) {
            sports_runner_a_runners.player.unk_18 = -0x10000;
        }
        if (D_801491E8 < sports_runner_a_runners.player.unk_18) {
            sports_runner_a_runners.player.unk_18 = D_801491E8;
        }
        sports_runner_a_runners.player.unk_1C = 0;
    }
}

void sports_runner_player_screen_position_update(void) {
    SportsRunnerA *state;

    state = &sports_runner_a_runners.player;
    if (sports_runner_a_runners.player.speed < 0 || D_80149298.halves.high == 0) {
        sports_runner_a_runners.player.track_position.word -= D_8014929C;
    } else {
        sports_runner_a_runners.player.track_position.word = sports_runner_a_runners.player.track_position.word + 0x8000;
    }
    if (state->track_position.halves.high >= -0x1F) {
        state->track_position.halves.high = -0x20;
    }
    if (state->track_position.halves.high < -0x9F) {
        state->track_position.halves.high = -0xA0;
    }
    D_80120676 = state->track_position.halves.high;
}

void sports_runner_player_animation_update(void) {
    u16 limit;
    TaiikuU11 limits;
    u8 temp;
    u16 count;

    limits = D_801497DC;
    if (D_80122E94 == 1) {
        limit = limits.v[D_801491DC];
    } else {
        limit = limits.v[((Actor *)actor_pool)[46 + sports_runner_a_runners.player.display_actor_index].sequence_slot >> 3];
    }
    if ((limit < sports_runner_a_runners.player.unk_0E) && (sports_runner_a_runners.player.unk_18 != 0)) {
        if (sports_runner_a_runners.player.unk_04 >= 5U) {
            sports_runner_a_runners.player.unk_04 = 0;
        } else {
            sports_runner_a_runners.player.unk_04++;
        }
        sports_runner_a_runners.player.unk_0E = 0;
    }
    count = sports_runner_a_runners.player.unk_0C;
    if (((count >= 6U) && (sports_runner_a_runners.player.speed > 0)) || ((count >= 8U) && (((s32) D_801491E8 >> 1) < sports_runner_a_runners.player.unk_18)) || ((count >= 11U) && (sports_runner_a_runners.player.unk_18 != 0))) {
        temp = sports_runner_a_runners.player.animation_frame;
        if (temp >= 5U) {
            sports_runner_a_runners.player.animation_frame = 0;
        } else {
            sports_runner_a_runners.player.animation_frame = sports_runner_a_runners.player.animation_frame + 1;
        }
        sports_runner_a_runners.player.unk_0C = 0;
    }
    ((Actor *)actor_pool)[96].unk_18 = *(s16 *)(D_801492AC + sports_runner_a_runners.player.animation_frame * 2);
    if ((D_8014929C == 0) && (sports_runner_a_runners.player.speed <= 0)) {
        sports_runner_a_runners.player.animation_frame = 5;
        ((Actor *)actor_pool)[96].unk_18 = 0x90;
    }
}

s32 sports_runner_scroll_update() {
    s32 temp;
    D_8014929C += sports_runner_a_runners.player.speed;
    temp = (*(u32 *)&D_801491E8);
    if (temp < D_8014929C) {
        D_8014929C = D_801491E8;
    }
    if (D_8014929C < 0 || (sports_runner_a_runners.player.speed < 0 && sports_runner_a_runners.player.track_position.halves.high < -0x9F)) {
        D_8014929C = 0;
    }
    D_80149298.word += D_8014929C;
    Scroll(((D_80149298.halves.high >> 1)) % 640, 0xF, 0, 0x70, 0x280);
    Scroll(D_80149298.halves.high & 0x7F, 0xF, 0x70, 0x80, 0x80);
}

void sports_runner_cpu_update(void) {
    sports_runner_cpu_phase_update();
    sports_runner_cpu_speed_config_copy();
    sports_runner_cpu_motion_state_update();
    sports_runner_position_and_finish_update();
    sports_runner_actor_animation_state_update();
}

/* Signed selector keeps the case constants distinct from byte stores. T-12020. */
void sports_runner_cpu_phase_update(void) {
    s32 i;
    TaiWait3 wait;

    wait = D_801497F4;
    for (i = 0; i < 3; i++) {
        if (sports_runner_a_runners.cpu[i].unk_01 != 0) {
            switch ((s32)sports_runner_a_runners.cpu[i].start_sequence_state) {
            case 0:
                sports_runner_a_runners.cpu[i].unk_08 = rand() % 120 + wait.value[i] + 60;
                sports_runner_a_runners.cpu[i].start_sequence_state++;
                break;
            case 1:
                if (++sports_runner_a_runners.cpu[i].unk_0A >= sports_runner_a_runners.cpu[i].unk_08) {
                    sports_runner_a_runners.cpu[i].start_sequence_state++;
                }
                break;
            case 2:
                if (sports_runner_finish_actor_slide_update(i + 1) != 0) {
                    sports_runner_a_runners.cpu[i].unk_01 = 0;
                    sports_runner_a_runners.cpu[i].unk_00 = 1;
                }
                break;
            default:
                game_mode_reset_to_title();
                break;
            }
        }
    }
}

typedef struct {
    s32 w[3];
} Tw3; /* size 0xC */
extern Tw3 D_801497FC;

void sports_runner_cpu_speed_config_copy(void) {
    s32 i;
    Tw3 t;

    t = D_801497FC;
    for (i = 0; i < 3; i++) {
        sports_runner_a_runners.cpu[i].speed = t.w[i];
    }
}

/* Slot records of 0x24 bytes at sports_runner_a_runners + 0x24 * i, reached as members of sports_runner_a_runners so IDO sees the object
 * (loop-invariant D_80120A72 is hoisted). FAKE: the unused-looking s16 y reserves the 4 stack bytes the original has
 * and `th.v[i] <= (y = ...)` keeps the load order. T-10720. */
void sports_runner_cpu_motion_state_update(void) {
    s32 i;
    s16 y;
    TaiikuI3 lim;
    TaiikuH3 th;

    lim = D_80149808;
    th = D_80149814;
    for (i = 0; i < 3; i++) {
        if (sports_runner_a_runners.cpu[i].unk_06 == 1 && D_80120A72 + i * 8 + 0x148 < *(s16 *)(actor_pool + i * 0x44 + 0x19EA)) {
            sports_runner_a_runners.cpu[i].unk_18 = 0;
        } else {
            if (th.v[i] <= (y = sports_runner_a_runners.cpu[i].track_position.halves.high) && sports_runner_a_runners.cpu[i].unk_00 == 0) {
                sports_runner_a_runners.cpu[i].unk_18 = 0;
                sports_runner_a_runners.cpu[i].unk_01 = 1;
            } else if (y >= 0x140 && sports_runner_a_runners.cpu[i].unk_00 == 0) {
                sports_runner_a_runners.cpu[i].unk_18 = (lim.v[i] >> 1) + 1;
            } else if (sports_runner_a_runners.cpu[i].unk_1C >= 0xB && sports_runner_a_runners.cpu[i].unk_01 == 0 && sports_runner_a_runners.cpu[i].unk_06 == 0) {
                sports_runner_a_runners.cpu[i].unk_18 += sports_runner_a_runners.cpu[i].speed;
                if (sports_runner_a_runners.cpu[i].unk_18 < 0) {
                    sports_runner_a_runners.cpu[i].unk_18 = 0;
                }
                if (lim.v[i] < sports_runner_a_runners.cpu[i].unk_18) {
                    sports_runner_a_runners.cpu[i].unk_18 = lim.v[i];
                }
                sports_runner_a_runners.cpu[i].unk_1C = 0;
            }
        }
    }
}

void sports_runner_position_and_finish_update(void) {
    s32 i;
    s32 k;

    for (i = 0; i < 3; i++) {
        sports_runner_a_runners.cpu[i].track_position.word += sports_runner_a_runners.cpu[i].unk_18;
        if (((Actor *)actor_pool)[111].x + (i + 1) * 8 < sports_runner_a_runners.cpu[i].track_position.halves.high - D_80149298.halves.high - 0x60 && sports_runner_a_runners.cpu[i].unk_06 == 0) {
            ((Actor *)actor_pool)[112].unk_18 = 0x8D;
            if (sports_runner_a_runners.cpu[i].unk_06 == 0) {
                k = 0;
                if (D_801491E0[0] != 0) {
                    do {
                        k += 1;
                    } while (D_801491E0[k] != 0);
                }
                D_801491E0[k] = i + 2;
            }
            sports_runner_a_runners.cpu[i].unk_06 = 1;
        }
        ((Actor *)actor_pool)[97 + i].x = sports_runner_a_runners.cpu[i].track_position.halves.high - D_80149298.halves.high - 0xA0;
        if (sports_runner_a_runners.cpu[i].track_position.halves.high - D_80149298.halves.high >= 0x1E1 || sports_runner_a_runners.cpu[i].track_position.halves.high - D_80149298.halves.high < -0xA0) {
            ((Actor *)actor_pool)[97 + i].draw_flags = 0;
        } else {
            ((Actor *)actor_pool)[97 + i].draw_flags = 0xA4;
        }
    }
}

/* FAKE: byte-offset walk over the cpu runners of sports_runner_a_runners; written as SportsRunnerA fields (p = &sports_runner_a_runners.cpu[i],
 * or the byte base plus 0x24 * i cast to SportsRunnerA *) the code differs (checked in the T-16550 review). T-16550 */
void sports_runner_actor_animation_state_update(void) {
    s32 i;
    u8 *p;
    u32 t;
    s32 k;
    for (i = 0; i < 3; i++) {
        p = ((u8 *)&sports_runner_a_runners) + i * 0x24;
        if (p[0x25] != 0) {
            ((Actor *)actor_pool)[i + 97].unk_18 = i + 0x88;
        } else {
            t = *(u16 *)(p + 0x30);
            if (t >= 6 && !(*(s32 *)(p + 0x3C) < 0xD000) || t >= 8 && !(*(s32 *)(p + 0x3C) < 0x6801) || t >= 0xB) {
                k = p[0x27];
                if (k >= 5) {
                    p[0x27] = 0;
                    *(u16 *)(p + 0x30) = 0;
                } else {
                    p[0x27] = k + 1;
                    *(u16 *)(p + 0x30) = 0;
                }
                ((Actor *)actor_pool)[i + 97].unk_18 = *(s16 *)&D_801492AC[p[0x27] * 2] + i * 6 + 6;
            }
        }
    }
}

/* The original leaves the K&R result in v0 without an explicit return. */
s32 sports_runner_finish_audio_and_lane_update(void) {
    s32 timer;
    s16 y;
    Actor *record;

    /* FAKE: the indirect halfword reads retain the timer in a0; direct scalar
     * reads introduce an extra v0-to-a0 copy. T-10030. */
    timer = *(s16 *)&D_80149298.halves.high;
    if (timer < 0x3C1) {
        if (((D_80120676 + 0x10) >= D_80120786) && (D_80120762 != 0)) {
            if (audio_bank_wait_done == 1) {
                audio_command(0x609);
            } else {
                audio_command(0x509);
            }
            D_80120762 = 0;
            timer = *(s16 *)&D_80149298.halves.high;
            D_80120763 = 0;
        }
        /* FAKE: joining initialization and loop opening preserves the two
         * address-setup instructions' order. T-10030. */
        record = (Actor *)actor_pool; do {
            if (record[97].x >= record[101].x) {
                record[101].flags = 0;
                record[101].draw_flags = 0;
            }
            record++;
        } while ((u32) record < (u32) (actor_pool + 0xCC));
        y = 0xE8 - (timer * 2);
        D_80120786 = y;
        D_801207CA = y;
        D_8012080E = y;
        D_80120852 = y;
    }
}

void sports_runner_rival_choice_input_process(void) {
    u8 sel;
    s32 i;
    s32 a;
    s32 b;
    s16 lo;
    s16 hi;
    s32 pos;
    s32 one; /* FAKE: names the constant 1 so that it is the first operand of the three compares (bne v0,t8). */

    one = 1;
    Yubi(0x6A);
    if (pad_code_entered != 0 && D_80122E94 == 0xFF) {
        if (one == audio_bank_wait_done) {
            audio_command(0x60E);
        } else {
            audio_command(0x50E);
        }
        D_80122E94 = 1;
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        if (sports_runner_a_runners.player.unk_07 >= 0x29U) {
            i = 0;
            a = -0xA0;
            b = -0x80;
            if (D_80120922 < 0 && D_80120922 >= -0x30) {
                sel = 0;
            } else if (D_80120922 < -0x30 && D_80120922 >= -0x40) {
                sel = 0xA;
            } else if (D_80120922 < -0x40 && D_80120922 >= -0x50) {
                sel = 0x14;
            } else {
                sel = 0x80;
            }
            do {
                lo = a;
                hi = b;
                if (sel == 10) {
                    lo += 0x10;
                    hi += 0x10;
                }
                a += 0x20;
                if (lo < (&D_8012091E)[0] + 0xB && (&D_8012091E)[0] + 0xB < hi) {
                    sel += i;
                    break;
                }
                i++;
                b += 0x20;
            } while (i != 10);
            if (one == D_80122E94 && sel == D_801491D8 && D_801491DC != 0xFF) {
                if (one == audio_bank_wait_done) {
                    audio_command(0x60C);
                } else {
                    audio_command(0x50C);
                }
                sports_runner_a_runners.player.unk_01 = 1;
            } else if (sel == sports_runner_a_runners.player.display_actor_index) {
                audio_command(0x601);
                sports_runner_a_runners.player.unk_01 = 1;
                D_80122E94 = 0xFF;
                D_801491DC = 0xFF;
            } else {
                /* FAKE: extends the hoisted CSE live range past the loop so i outranks it. */
                pos = (&D_8012091E)[0] + 0xB;
                pos = pos;
                audio_command(0x600);
            }
            sports_runner_a_runners.player.unk_07 = 0;
        }
    }
}

/* Display records of the 0x44-byte pool; DISPF reads +0x24/+0x28 as 16.16 words. T-25106.
 * The arg0 == 0 before D_8014929C and the one before D_80122E94 == 1 become bne a0,t8 /
 * bne a1,t9: &D_8014984C[arg0] and &D_8014981C[arg0] are in a0/a1 by then, and uopt rewrites the
 * index test into a compare of that address with the array base, built fresh at the compare
 * (the address comes from an earlier block and arg0 is not used after the test). The other
 * arg0 tests stay bnez. T-25502. */
#define DISP(n) (((Actor *)actor_pool)[n])
#define DISPF(n) (((ActorFixed *)actor_pool)[n])
/* FAKE: shared player/cpu[3] index, as in matched sports_runner_finish_display_selector; T-25106. */
#define RUN(n) ((&sports_runner_a_runners.player)[n])
/* FAKE: (s32) on the runner's record index. Without it the u8 index takes its own unsigned 0x44 and
 * the entry product is strength-reduced; the original shares one li t5,68 with the arg0 products. T-25106 */
s32 sports_runner_finish_actor_slide_update(s32 arg0) {
    s32 i;

    if (DISP(46 + (s32)RUN(arg0).display_actor_index).flags != 0 && (arg0 != 0 || D_80122E94 == 0xFF)) {
        DISP(46 + (s32)RUN(arg0).display_actor_index).flags = 0;
        DISP(46 + (s32)RUN(arg0).display_actor_index).draw_flags = 0;
        if (arg0 == 0) {
            for (i = 0; i < 3; i++) {
                DISP(104 + i).flags = 0;
                DISP(104 + i).draw_flags = 0;
            }
            k_reset(0);
        }
        DISP(107 + arg0).flags = 1;
        DISP(107 + arg0).draw_flags = 0xA4;
        DISP(107 + arg0).x = (((s16)RUN(arg0).display_actor_index % 10) << 5) - D_80149298.halves.high + 0x1E0;
        DISP(107 + arg0).y = -((s16)RUN(arg0).display_actor_index / 10) * 16 - 0x30;
        D_8014982C[arg0] = (DISPF(96 + arg0).x.w - DISPF(107 + arg0).x.w) >> 7;
        D_8014983C[arg0] = (DISPF(96 + arg0).y.w - DISPF(107 + arg0).y.w) >> 7;
        D_8014984C[arg0].w = DISPF(107 + arg0).x.w - DISPF(96 + arg0).x.w;
        if (D_8014982C[arg0] > 0) {
            DISP(107 + arg0).draw_flags |= 1;
        }
        D_8014981C[arg0] = 0;
        RUN(arg0).animation_frame = 0;
        RUN(arg0).unk_04 = 0;
        RUN(arg0).unk_0C = 0;
        RUN(arg0).unk_0E = 0;
        RUN(arg0).speed = 0;
        RUN(arg0).unk_14 = 0;
        RUN(arg0).unk_18 = 0;
        RUN(arg0).unk_1C = 0;
        if (arg0 == 0) {
            sports_runner_a_runners.player.track_position.halves.high = -0xA0;
            D_8014929C = 0;
        }
        return 0;
    }
    /* FAKE: record D_801491D8 indexed from record 46, so the u8 global stays unsigned and its product is
     * strength-reduced, as in sports_runner_animation_and_rival_select. T-25106 */
    if ((&DISP(46))[D_801491D8].flags != 0 && D_80122E94 == 1 && arg0 == 0) {
        (&DISP(46))[D_801491D8].flags = 0;
        (&DISP(46))[D_801491D8].draw_flags = 0;
        for (i = 0; i < 3; i++) {
            DISP(104 + i).flags = 0;
            DISP(104 + i).draw_flags = 0;
        }
        k_reset(0);
        DISP(107).sequence_slot = DISP(115).sequence_slot;
        DISP(107).unk_43 = DISP(115).unk_43;
        DISP(107).unk_18 = 0;
        DISP(107).unk_08 = 0;
        DISP(107).flags = 1;
        DISP(107).draw_flags = 0xA4;
        DISP(107).x = (D_801491D8 << 5) - D_80149298.halves.high + 0x1E0;
        DISP(107).y = -0x30;
        D_8014982C[0] = (DISPF(96).x.w - DISPF(107).x.w) >> 7;
        D_8014983C[0] = (DISPF(96).y.w - DISPF(107).y.w) >> 7;
        D_8014984C[0].w = DISPF(107).x.w - DISPF(96).x.w;
        if (D_8014982C[0] > 0) {
            DISP(107).draw_flags = 0xA5;
        }
        D_8014981C[0] = 0;
        sports_runner_a_runners.player.animation_frame = 0;
        sports_runner_a_runners.player.unk_04 = 0;
        sports_runner_a_runners.player.unk_0C = 0;
        sports_runner_a_runners.player.unk_0E = 0;
        sports_runner_a_runners.player.speed = 0;
        sports_runner_a_runners.player.unk_14 = 0;
        sports_runner_a_runners.player.unk_18 = 0;
        sports_runner_a_runners.player.unk_1C = 0;
        sports_runner_a_runners.player.track_position.halves.high = -0xA0;
        D_8014929C = 0;
        return 0;
    }
    D_8014984C[arg0].w += D_8014982C[arg0];
    DISP(107 + arg0).x = DISP(96 + arg0).x + D_8014984C[arg0].h[1];
    DISPF(107 + arg0).y.w += D_8014983C[arg0];
    if (++D_8014981C[arg0] >= 0x80) {
        DISP(96 + arg0).unk_18 = arg0 + 0x90;
        if (arg0 == 0 && D_80122E94 == 1) {
            DISP(107).unk_18 = D_80149320[D_801491DC] * 6 + 5;
        } else {
            DISP(107 + arg0).unk_18 = D_801491EC * 6 + 5;
        }
        DISP(107 + arg0).x = DISP(96 + arg0).x - 8;
        DISPF(107 + arg0).y.w = DISPF(96 + arg0).y.w;
        DISP(107 + arg0).flags = 0x40;
        DISP(107 + arg0).draw_flags = 0xA4;
        return 1;
    }
    return 0;
}
#undef RUN
#undef DISPF
#undef DISP

/* Display-record view shared with the matched game-C sibling. T-24008. */
#define DISP(n) (((Actor *)actor_pool)[n])
/* FAKE: display record of runner n (SportsRunnerA.display_actor_index), counted from record 46. Indexing from that
 * record lets a u8 global index stay unsigned, as the runner field does, so both share one 0x44
 * constant; DISP(46 + n) widens the global and gives it the counter's constant. T-25106. */
#define RUNNER_DISP(n) ((&DISP(46))[n])
/* FAKE: byte-offset address of record i for the second loop. Written as DISP(108 + i) it is the same
 * expression as the first loop's DISP(107 + i), and the second loop then ends on the runner
 * pointer instead of the record pointer. T-25106 */
#define DISP_AT(i) ((Actor *)(actor_pool + (i) * 0x44))
s32 sports_runner_finish_actor_display_update(void) {
    s32 i;
    s32 x;
    s32 pos;
    SportsRunnerA *runner;

    for (i = 0, runner = &sports_runner_a_runners.player; i < 4; i++, runner++) {
        if (runner->unk_00 != 0) {
            DISP(107 + i).x = DISP(96 + i).x - 8;
            if (i == 0) {
                /* FAKE: read through an address view; an indirect load is not a plain variable, so uopt
                 * keeps the constant first (bne t4,a1), as in the original. T-25106 */
                if (*(s32 *)&D_80122E94 == 1) {
                    DISP(107).unk_18 = D_801492B8[sports_runner_a_runners.player.unk_04] + ((RUNNER_DISP(D_801491D8).sequence_slot >> 3) * 6);
                } else {
                    DISP(107).unk_18 = D_801492B8[sports_runner_a_runners.player.unk_04] + ((RUNNER_DISP(runner->display_actor_index).sequence_slot >> 3) * 6);
                }
            } else {
                DISP(107 + i).unk_18 = D_801492B8[runner->animation_frame] + ((RUNNER_DISP(runner->display_actor_index).sequence_slot >> 3) * 6);
            }
            if (i + 0x90 == DISP(96 + i).unk_18) {
                /* FAKE: address view as above. T-25106 */
                if (*(s32 *)&D_80122E94 == 1) {
                    DISP(107 + i).unk_18 = D_80149320[D_801491DC] * 6 + 5;
                } else {
                    DISP(107 + i).unk_18 = ((RUNNER_DISP(runner->display_actor_index).sequence_slot >> 3) * 6) + 5;
                }
            }
        }
    }
    pos = D_80149298.halves.high;
    for (i = 0; i < 3; i++) {
        if (((sports_runner_a_runners.cpu[i].start_sequence_state == 2 && DISP_AT(i)[108].flags != 0) || sports_runner_a_runners.cpu[i].unk_00 == 1) && (x = sports_runner_a_runners.cpu[i].track_position.halves.high - pos) < 0x140 && x >= -0x13F) {
            DISP_AT(i)[108].draw_flags = 0xA4;
        } else {
            DISP_AT(i)[108].draw_flags = 0;
        }
    }
}
#undef DISP_AT
#undef RUNNER_DISP

/* Keep the explicit final else and assign result slot zero first: uopt spills
 * the result-loop pointer and writes the promoted first byte after its neighbors. T-24008. */
s32 sports_runner_finish_line_update(void) {
    s32 finish_order_index;
    s32 cpu_runner_index;
    TkS16x4 finish_line_offsets;

    finish_line_offsets = D_8014985C;
    if (DISP(111).flags == 0 && D_80149298.halves.high >= 0x781) {
        for (cpu_runner_index = 0; cpu_runner_index < 3; cpu_runner_index++) {
            DISP(111 + cpu_runner_index).flags = 0x40;
            DISP(111 + cpu_runner_index).draw_flags = 0xA4;
            if (sports_runner_a_runners.cpu[cpu_runner_index].unk_06 == 1) {
                DISP(112).draw_flags = 0;
            }
        }
    }
    if (DISP(112).unk_18 == 0x8D) {
        DISP(112).draw_flags |= 0x40;
    }
    for (cpu_runner_index = 0; cpu_runner_index < 3; cpu_runner_index++) {
        if (sports_runner_a_runners.cpu[cpu_runner_index].unk_06 == 1 && DISP(97 + cpu_runner_index).x >= DISP(112).x + 0x30) {
            DISP(112).draw_flags = 0;
            DISP(112).flags = 0;
        }
    }
    for (cpu_runner_index = 0; &finish_line_offsets.v[cpu_runner_index] < &finish_line_offsets.v[4]; cpu_runner_index++) {
        DISP(111 + cpu_runner_index).x = finish_line_offsets.v[cpu_runner_index] - D_80149298.halves.high * 2 - 0xA0;
    }
    if (DISP(114).x < -0xDC && DISP(114).draw_flags != 0) {
        DISP(114).draw_flags = 0;
    }
    if (DISP(112).unk_18 == 0x8D) {
        DISP(112).x = DISP(111).x + 0x20;
    }
    if (DISP(96).x + 0x40 >= DISP(111).x) {
        if (D_801491E0[0] == 0) {
            DISP(112).draw_flags = 0xE4;
            DISP(112).unk_18 = 0x8D;
            DISP(112).x = DISP(113).x + 0x20;
        }
        cpu_runner_index = 0;
        while (D_801491E0[cpu_runner_index] != 0) {
            cpu_runner_index++;
        }
        D_801491E0[cpu_runner_index] = 1;
        for (finish_order_index = cpu_runner_index + 1; finish_order_index < 4; finish_order_index++) {
            D_801491E0[finish_order_index] = sports_runner_cpu_place_select();
        }
        if (D_80122E94 == 1) {
            D_801491E0[0] = 3;
            D_801491E0[1] = 2;
            D_801491E0[2] = 4;
            D_801491E0[3] = 1;
        }
        return 1;
    } else {
        return 0;
    }
}

#undef DISP


/* FAKE: v[6] instead of three elements reproduces the original local offset
 * (0x1c); the real source is unknown. T-0016 */
s32 sports_runner_cpu_place_select(void) {
    s16 cpu_place_values[6];
    s32 cpu_runner_index;

    for (cpu_runner_index = 0; cpu_runner_index < 3; cpu_runner_index++) {
        if (sports_runner_a_runners.cpu[cpu_runner_index].unk_06 == 0) {
            cpu_place_values[cpu_runner_index] = *(s16 *)(actor_pool + cpu_runner_index * 0x44 + 0x19EA);
        } else {
            cpu_place_values[cpu_runner_index] = -0xF00;
        }
    }
    if (cpu_place_values[0] >= cpu_place_values[1] && cpu_place_values[0] >= cpu_place_values[2]) {
        sports_runner_a_runners.cpu[0].unk_06 = 1;
        return 2;
    }
    if (cpu_place_values[1] >= cpu_place_values[2]) {
        sports_runner_a_runners.cpu[1].unk_06 = 1;
        return 3;
    }
    sports_runner_a_runners.cpu[2].unk_06 = 1;
    return 4;
}

void sports_runner_finish_actor_group_init(void) {
    s32 i;
    s32 j; /* FAKE: unused local; the original frame is 8 bytes larger, two words above spA4 (a second counter?), real source unknown. T-16551 */
    TaiBytes10 spA4;
    TaiikuHalf10 sp90;
    TaiikuHalf10 sp7C;
    TaiikuHalf10 sp68;

    spA4 = D_80149864;
    sp90 = D_80149870;
    sp7C = D_80149884;
    sp68 = D_80149898;
    for (i = 0; i < 4; i++) {
        ((Actor *)actor_pool)[96 + i].quad_flags = 0x01000000;
        ((Actor *)actor_pool)[96 + i].unk_01 = i * 2 + 2;
        ((Actor *)actor_pool)[96 + i].unk_04 = 0;
        ((Actor *)actor_pool)[96 + i].unk_05 = 0x80;
        ((Actor *)actor_pool)[96 + i].flags = 0x40;
        ((Actor *)actor_pool)[96 + i].draw_flags = 0xA4;
        ((Actor *)actor_pool)[96 + i].blink_period = 2;
        ((Actor *)actor_pool)[96 + i].unk_08 = 0;
        ((Actor *)actor_pool)[96 + i].frame_table = (u8 *)D_80149FBC;
        ((Actor *)actor_pool)[96 + i].sequence_table = (u8 *)D_80149FF4;
        ((Actor *)actor_pool)[96 + i].tpage_table = (u8 *)D_801492F8;
        ((Actor *)actor_pool)[96 + i].unk_14 = D_8014A02C;
        ((Actor *)actor_pool)[96 + i].sequence_slot = 0;
        ((Actor *)actor_pool)[96 + i].unk_18 = sports_runner_finish_display_selector(i, 1);
        ((Actor *)actor_pool)[96 + i].scale_x = 0x1000;
        ((Actor *)actor_pool)[96 + i].scale_y = 0x1000;
        ((Actor *)actor_pool)[96 + i].brightness = 0x80;
        ((Actor *)actor_pool)[96 + i].x = sports_runner_finish_marker_x_select(i);
        ((Actor *)actor_pool)[96 + i].y = 0x28 - i * 0x18;
    }
    for (i = 0; i < 4; i++) {
        ((Actor *)actor_pool)[107 + i].quad_flags = 0x01000000;
        ((Actor *)actor_pool)[107 + i].unk_01 = i * 2 + 3;
        ((Actor *)actor_pool)[107 + i].unk_04 = 0;
        ((Actor *)actor_pool)[107 + i].unk_05 = 0x80;
        ((Actor *)actor_pool)[107 + i].flags = 0x40;
        ((Actor *)actor_pool)[107 + i].draw_flags = 0xA4;
        ((Actor *)actor_pool)[107 + i].blink_period = 2;
        ((Actor *)actor_pool)[107 + i].unk_08 = 0;
        ((Actor *)actor_pool)[107 + i].frame_table = (u8 *)D_80149FBC;
        ((Actor *)actor_pool)[107 + i].sequence_table = (u8 *)D_80149FF4;
        ((Actor *)actor_pool)[107 + i].tpage_table = (u8 *)D_801492F8;
        ((Actor *)actor_pool)[107 + i].unk_14 = D_8014A02C;
        ((Actor *)actor_pool)[107 + i].sequence_slot = 0;
        ((Actor *)actor_pool)[107 + i].unk_18 = sports_runner_finish_display_selector(i, 0);
        ((Actor *)actor_pool)[107 + i].scale_x = 0x1000;
        ((Actor *)actor_pool)[107 + i].scale_y = 0x1000;
        ((Actor *)actor_pool)[107 + i].brightness = 0x80;
        ((Actor *)actor_pool)[107 + i].x = sports_runner_finish_marker_x_select(i);
        ((Actor *)actor_pool)[107 + i].y = 0x28 - i * 0x18;
    }
    for (i = 0; i != 10; i++) {
        actor_clear(i + 111);
        ((Actor *)actor_pool)[111 + i].quad_flags = 0x01000000;
        ((Actor *)actor_pool)[111 + i].unk_01 = spA4.v[i];
        ((Actor *)actor_pool)[111 + i].unk_04 = 0;
        ((Actor *)actor_pool)[111 + i].unk_05 = 0x80;
        ((Actor *)actor_pool)[111 + i].flags = 1;
        ((Actor *)actor_pool)[111 + i].draw_flags = 0xA4;
        ((Actor *)actor_pool)[111 + i].blink_period = 2;
        ((Actor *)actor_pool)[111 + i].unk_08 = 0;
        ((Actor *)actor_pool)[111 + i].frame_table = (u8 *)D_80149FBC;
        ((Actor *)actor_pool)[111 + i].sequence_table = (u8 *)D_80149FF4;
        ((Actor *)actor_pool)[111 + i].tpage_table = (u8 *)D_801492F8;
        ((Actor *)actor_pool)[111 + i].unk_14 = D_8014A02C;
        ((Actor *)actor_pool)[111 + i].sequence_slot = sp90.h[i];
        ((Actor *)actor_pool)[111 + i].unk_18 = 0;
        ((Actor *)actor_pool)[111 + i].scale_x = 0x1000;
        ((Actor *)actor_pool)[111 + i].scale_y = 0x1000;
        ((Actor *)actor_pool)[111 + i].unk_43 = 0;
        ((Actor *)actor_pool)[111 + i].brightness = 0x80;
        ((Actor *)actor_pool)[111 + i].x = sp7C.h[i];
        ((Actor *)actor_pool)[111 + i].y = sp68.h[i];
    }
    actor_clear(0x70);
    ((Actor *)actor_pool)[112].quad_flags = 0x01000000;
    ((Actor *)actor_pool)[112].unk_01 = 1;
    ((Actor *)actor_pool)[112].unk_04 = 0;
    ((Actor *)actor_pool)[112].unk_05 = 0x80;
    ((Actor *)actor_pool)[112].flags = 0x40;
    ((Actor *)actor_pool)[112].draw_flags = 0xA4;
    ((Actor *)actor_pool)[112].blink_period = 2;
    ((Actor *)actor_pool)[112].unk_08 = 0;
    ((Actor *)actor_pool)[112].frame_table = (u8 *)D_80149FB8;
    ((Actor *)actor_pool)[112].sequence_table = (u8 *)D_80149FF0;
    ((Actor *)actor_pool)[112].tpage_table = (u8 *)D_801492DC;
    ((Actor *)actor_pool)[112].unk_14 = D_8014A028;
    ((Actor *)actor_pool)[112].sequence_slot = 0;
    ((Actor *)actor_pool)[112].unk_18 = 0xA0;
    ((Actor *)actor_pool)[112].scale_x = 0x2000;
    ((Actor *)actor_pool)[112].scale_y = 0x2000;
    ((Actor *)actor_pool)[112].unk_43 = 0;
    ((Actor *)actor_pool)[112].brightness = 0x80;
    ((Actor *)actor_pool)[112].x = -0x80;
    ((Actor *)actor_pool)[112].y = -0xF8;
}

s32 sports_runner_finish_display_selector(s32 arg0, s32 arg1) {
    s32 key;
    s32 slot;
    TaiWords4 small;
    TaiWords11 large;

    small = D_801498AC;
    large = D_801498BC;
    key = arg0 + 1;
    if (key == D_801491E0[0]) {
        slot = 0;
    }
    if (key == D_801491E0[1]) {
        slot = 1;
    }
    if (key == D_801491E0[2]) {
        slot = 2;
    }
    if (key == D_801491E0[3]) {
        slot = 3;
    }
    if (arg1 != 0) {
        slot = slot * 4 + arg0 * 16;
    } else {
        slot = small.value[slot];
        if (D_80122E94 == 1 && slot == 4) {
            slot += large.value[D_80149320[D_801491DC]];
        } else {
            /* FAKE: indexes the four runners (player, cpu[3]) of sports_runner_a_runners from the player record; the
             * object is player + cpu[3], not one array (see SportsRunnersA). T-16550 */
            slot += large.value[((Actor *)actor_pool)[(&sports_runner_a_runners.player)[arg0].display_actor_index + 46].sequence_slot >> 3];
        }
    }
    return slot;
}

void sports_runner_finish_animation_update(void) {
    s32 i;
    s16 t;

    if (sports_runner_a_runners.player.unk_0C >= 0xBU) {
        sports_runner_a_runners.player.animation_frame += 1;
        sports_runner_a_runners.player.unk_0C = 0;
    }
    if (sports_runner_a_runners.player.animation_frame >= 4U) {
        sports_runner_a_runners.player.animation_frame = 0;
    }
    for (i = 0; i < 4; i++) {
        t = sports_runner_finish_display_selector(i, 1);
        *(s16 *)(actor_pool + i * 0x44 + 0x1998) = sports_runner_a_runners.player.animation_frame + t;
    }
    for (i = 0; i != 4; i++) {
        t = sports_runner_finish_display_selector(i, 0);
        *(s16 *)(actor_pool + i * 0x44 + 0x1C84) = (sports_runner_a_runners.player.animation_frame & 1) + t;
    }
}

s16 sports_runner_finish_marker_x_select(s32 arg0) {
    s32 key;
    s32 slot;

    key = arg0 + 1;
    if (key == D_801491E0[0]) {
        slot = 0;
    }
    if (key == D_801491E0[1]) {
        slot = 1;
    }
    if (key == D_801491E0[2]) {
        slot = 2;
    }
    if (key == D_801491E0[3]) {
        slot = 3;
    }
    return (slot << 6) + 0xA0;
}

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148A90);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148A98);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148AA0);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148AA8);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148AB0);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148AB8);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148AC0);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148AC8);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148AD0);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148AD8);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148ADC);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148AE4);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148AEC);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148AF0);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148AF8);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148B00);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148B08);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148B10);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148B18);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148B1C);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148B24);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/runner_race_dispatch.rodata", D_80148B2C);
