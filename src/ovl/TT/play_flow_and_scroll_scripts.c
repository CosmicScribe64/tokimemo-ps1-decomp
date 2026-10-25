/* play_flow_and_scroll_scripts.c: src/ovl/TT/80136830.c:39 selects scroll/enemy scripts and boss callback by level selector; :75 chooses replay stream; :142 advances scroll accumulator and initializes/updates entity pools; :327 calls selected boss; :13 updates speed gauge; entry dispatch src/ovl/TT/frame_loop_and_pause.c:48. Original object 0x80136830-0x801374C0 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/TT.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/TT/data/TT/play_flow_and_scroll_scripts.data", tt_stage1_air_script);
INCLUDE_RODATA("asm/ovl/TT/data/TT/play_flow_and_scroll_scripts.data", tt_stage1_ground_script);
INCLUDE_RODATA("asm/ovl/TT/data/TT/play_flow_and_scroll_scripts.data", tt_demo_input_stage2);
/* Previous and current page (*tt_run / 0x8000) of tt_speed_gauge_update. File-static in the
 * original: uopt keeps both in registers between the stores and the compare. T-11860 */
static s32 tt_speed_gauge_previous_step = 0;
static s32 tt_speed_gauge_current_step = 0;

void tt_speed_gauge_update(void) {
    u8 *run;
    u8 *p;
    s32 i;

    run = tt_run;
    tt_speed_gauge_previous_step = tt_speed_gauge_current_step;
    tt_speed_gauge_current_step = *(s32 *)run / 0x8000;
    if (tt_speed_gauge_previous_step != tt_speed_gauge_current_step) {
        tt_se_request(TT_SE_SPEED_GAUGE);
    }
    if (*(s32 *)run == 0) {
        for (i = 0, p = tt_sprites + 0xC8; i < 6; i++, p += 0x64) {
            *(s16 *)(p + 0x42) = 0x3C0A;
        }
    } else {
        for (i = 0, p = tt_sprites + 0xC8; i < 6; i++, p += 0x64) {
            if (*(s32 *)run / 0x8000 == i + 1) {
                *(s16 *)(p + 0x42) = 0x3C09;
            } else {
                *(s16 *)(p + 0x42) = 0x3C0A;
            }
        }
    }
}

void tt_stage_script_init(void) {
    s32 *stage_script;
    u8 *q;
    u8 k;

    q = tt_run;
    stage_script = tt_stage_script;
    stage_script[0] = 0;
    k = q[0xB];
    *(s32 *)q = 0x8000;
    *(s32 *)(q + 4) = 0x8000;
    switch (k) {
    default:
    case 0:
        stage_script[1] = (s32)tt_stage1_air_script;
        stage_script[2] = (s32)tt_stage1_ground_script;
        stage_script[3] = (s32)tt_boss1_update;
        return;
    case 1:
        stage_script[1] = (s32)tt_stage2_air_script;
        stage_script[2] = (s32)tt_stage2_ground_script;
        stage_script[3] = (s32)tt_boss2_update;
        return;
    case 2:
        stage_script[1] = (s32)tt_stage3_air_script;
        stage_script[2] = (s32)tt_stage3_ground_script;
        stage_script[3] = (s32)tt_boss3_update;
        return;
    case 3:
        stage_script[1] = (s32)tt_stage4_air_script;
        stage_script[2] = (s32)tt_stage4_ground_script;
        stage_script[3] = (s32)tt_boss4_update;
        break;
    }
}

void tt_stage_setup(void) {
    u8 *sys;
    u8 *core;
    u8 *run;

    sys = tt_sys;
    core = tt_core;
    run = tt_run;
    func_8009C5E0(1);
    if (core[0x22] != 0) {
        if (sys[0x52] == 0xFF) {
            u8 *p = tt_texts + 0x9D8;

            *(s16 *)(tt_texts + 0x9E2) = -0x24;
            *(s16 *)(p + 0xC) = 0x74;
            *(s16 *)(p + 0xE) = 9;
            p[2] = 2;
            p[0] = 1;
            *(s16 *)(p + 6) = 0;
            *(s16 *)(p + 0x10) = 0;
        }
        switch (sys[0x52]) {
        case 0xFF:
        default:
            *(u8 **)(sys + 0x4C) = tt_demo_input_attract;
            *(s16 *)(run + 8) = 0;
            run[0xB] = 0;
            run[0xC] = 0;
            run[0xD] = 0;
            break;
        case 1:
            *(u8 **)(sys + 0x4C) = tt_demo_input_stage1;
            *(s16 *)(run + 8) = 0;
            run[0xB] = 0;
            run[0xC] = 2;
            run[0xD] = 0;
            break;
        case 2:
            *(u8 **)(sys + 0x4C) = tt_demo_input_stage2;
            *(s16 *)(run + 8) = 0xC3E;
            run[0xB] = 1;
            run[0xC] = 0;
            run[0xD] = 0;
            break;
        case 3:
            *(u8 **)(sys + 0x4C) = tt_demo_input_stage3;
            *(s16 *)(run + 8) = 0x131F;
            run[0xB] = 2;
            run[0xC] = 0;
            run[0xD] = 0;
            break;
        case 4:
            *(u8 **)(sys + 0x4C) = tt_demo_input_stage4;
            *(s16 *)(run + 8) = 0x235D;
            run[0xB] = 3;
            run[0xC] = 2;
            run[0xD] = 0;
            break;
        }
    } else {
        *(s16 *)(run + 8) = 0;
        run[0xB] = 0;
        run[0xC] = 2;
        run[0xD] = 0;
    }
}

void tt_state_stage(void) {
    u8 *sys;
    u8 *b;
    u8 *run;
    s32 *stage_script;
    u8 *q;
    u8 *sprite;
    u8 *player;
    s32 i;
    u8 *boss;
    s32 pad[2]; /* FAKE: the original frame has two more home slots (sp+0x38, sp+0x34) above t. T-11840 */
    s32 t;

    sys = tt_sys;
    b = tt_core;
    run = tt_run;
    stage_script = tt_stage_script;
    q = tt_texts;
    player = tt_player;
    *stage_script += *(s32 *)run;
    if (run) {} /* FAKE: an extra use of run raises its priority over b, so run takes v1 and b a2 as in the original; no code. T-11840 */
    switch (*(u16 *)(b + 2)) {
    case 0:
        if (b[0x22] != 0) {
            switch (sys[0x52]) {
            case 1:
            case 2:
            case 3:
            case 4:
                t = cd_done_count();
                if (((t - tt_cd_read_counter_before() + 0x100) & 0xFF) < 5) {
                    return;
                }
                tt_sound_banks_load();
                audio_command(0x21);
                audio_command(0x22);
                audio_command(0x2F);
                audio_command(4);
                audio_command(0x201);
                break;
            default:
                audio_command(0xB1);
                break;
            }
        } else {
            audio_command(0x21);
            audio_command(0x22);
            audio_command(0x2F);
            audio_command(4);
            audio_command(0x201);
        }
        bzero(run, 0x20);
        bzero((u8 *)stage_script, 0x20);
        *(s16 *)(sys + 0x50) = 0;
        tt_sprites_clear();
        tt_texts_clear();
        tt_player_clear();
        tt_shots_clear();
        tt_bombs_clear();
        tt_bomb_targets_clear();
        tt_options_clear();
        tt_air_enemies_clear();
        tt_ground_targets_clear();
        tt_enemy_shots_clear();
        tt_bells_clear();
        tt_clouds_clear();
        tt_boss_sprites_clear();
        if (b[0x22] != 0) {
            b[0x11] = 0;
        } else {
            b[0x11] = 1;
        }
        tt_options_reset();
        tt_shots_reset();
        tt_bomb_targets_reset();
        tt_player_init();
        tt_air_enemies_reset();
        tt_ground_targets_reset();
        b[0x12] = 1;
        *(u8 **)(b + 0x18) = tt_stage_tile_map;
        b[0x1C] = 2;
        tt_stage_setup();
        tt_stage_script_init();
        *(s16 *)(q + 0xA) = -0x70;
        *(s16 *)(q + 0xC) = 0x10;
        *(s16 *)(q + 0xE) = 0xD;
        q[2] = 1;
        q[0] = 1;
        *(s16 *)(q + 6) = 0;
        *(s16 *)(q + 0x10) = 0;
        q += 0x14;
        *(s16 *)(q + 0xA) = -0x48;
        *(s16 *)(q + 0xC) = 0x10;
        *(s16 *)(q + 0xE) = 0x27;
        q[2] = 1;
        q[0] = 1;
        *(s16 *)(q + 6) = 0;
        *(s16 *)(q + 0x10) = 0;
        q += 0x14;
        tt_time_text_set(*(u16 *)(run + 8), 0x27);
        sprite = tt_sprites;
        for (i = 0; i < 2; i++, sprite += 0x64) {
            sprite[0] = 1;
            sprite[1] = 0;
            *(s16 *)(sprite + 0x44) = 0;
            *(s16 *)(sprite + 0x16) = 0x18;
            *(s16 *)(sprite + 0x3C) = 0x10;
            *(s16 *)(sprite + 0x3E) = 0x10;
            sprite[0x46] = 0;
            sprite[0x47] = 0x70;
            *(s16 *)(sprite + 0x40) = 0xA;
            *(s16 *)(sprite + 0x42) = 0x3C0B;
            sprite[0x3B] = 0x65;
            *(s16 *)(sprite + 0x10) = 0;
            *(s16 *)(sprite + 0x14) = i * 0x10 - 0x70;
        }
        tt_lives_icons_update();
        for (i = 0; i < 6; i++, sprite += 0x64) {
            sprite[0] = 1;
            sprite[1] = 0;
            *(s16 *)(sprite + 0x44) = 0;
            *(s16 *)(sprite + 0x14) = -0x78;
            *(s16 *)(sprite + 0x16) = 0xD8 - i * 8;
            *(s16 *)(sprite + 0x3E) = 8;
            *(s16 *)(sprite + 0x3C) = 0x10;
            sprite[0x46] = (i / 2) * 0x10 + 0x50;
            sprite[0x47] = (i & 1) * 8 + 0x30;
            *(s16 *)(sprite + 0x40) = 0xA;
            *(s16 *)(sprite + 0x42) = 0x3C09;
            sprite[0x3B] = 0x65;
            *(s16 *)(sprite + 0x10) = 0;
        }
        player[0x59] = 0;
        player[0x58] = 0;
        *(s16 *)(b + 2) = 0x40;
        break;
    case 1:
        tt_sound_bank_stage_reload();
        audio_command(0x21);
        audio_command(0x22);
        audio_command(0x2F);
        audio_command(4);
        audio_command(0x201);
        tt_shots_clear();
        tt_bombs_clear();
        tt_bomb_targets_clear();
        tt_air_enemies_clear();
        tt_ground_targets_clear();
        tt_enemy_shots_clear();
        tt_bells_clear();
        tt_clouds_clear();
        tt_boss_sprites_clear();
        b[0x11] = 1;
        player[0x57] = 0;
        /* Chained stores as in the original: the 2-chain costs one ugen temporary (t-register numbering below), the longer chains leave uopt ranges for the reloaded value in v1/a0, so player takes a1. T-11840 */
        player[0x58] = player[0x59] = 0;
        player[0x5A] = player[0x5B] = player[0x5C] = player[0x5D] = player[0x5E] = player[0x5F] = 0;
        *(s16 *)(player + 4) = 0x20;
        player[0xD] = player[0xE] = player[0xF] = 0;
        *(s16 *)(player + 6) = 0;
        *(s16 *)(player + 8) = 0;
        *(s16 *)(player + 0xA) = 0;
        tt_shots_reset();
        b[0x12] = 1;
        b[0x1C] = 2;
        tt_stage_script_init();
        *(s16 *)(b + 2) = 0x40;
        break;
    case 0x40:
        tt_time_text_set(*(u16 *)(run + 8), 0x27);
        if (*(u16 *)((u8 *)stage_script + 2) > 0x400) {
            boss = tt_boss;
            *(s16 *)(boss + 4) = 0;
            boss[0xC] = 0;
            *(s16 *)(boss + 0xA) = 0;
            *(s16 *)(boss + 8) = 0;
            *(s16 *)(boss + 6) = 0;
            boss[0xF] = 0;
            boss[0xE] = 0;
            boss[0xD] = 0;
            *(u16 *)(b + 2) = 0x50;
        }
        break;
    case 0x50:
        tt_time_text_set(*(u16 *)(run + 8), 0x27);
        ((void (*)(void))stage_script[3])();
        break;
    case 0xA0:
        tt_time_text_set(*(u16 *)(run + 8), 0x27);
        *(s16 *)(player + 4) = 0xA0;
        break;
    }
    if (b[0x22] != 0 && sys[0x52] == 0xFF) {
        q = tt_texts + 0x9D8;
        if ((*(u16 *)(sys + 8) >> 4) & 1) {
            *(u16 *)(q + 6) = 1;
        } else {
            *(s16 *)(q + 6) = 0;
        }
        if ((*(u16 *)(sys + 0x14) & 0x800) || (*(u16 *)(sys + 0x14) & 0x20)) {
            *(u16 *)b = 1;
            *(s16 *)(b + 6) = 0;
            *(s16 *)(b + 4) = 0;
            *(s16 *)(b + 2) = 0;
            b[0xB] = 0;
            b[0xA] = 0;
            b[9] = 0;
        }
    }
    tt_options_update();
    tt_player_update();
    tt_shots_update();
    tt_bombs_update();
    tt_bomb_targets_update();
    tt_bells_update();
    tt_clouds_update();
    tt_air_enemies_update();
    tt_ground_targets_update();
    tt_enemy_shots_update();
    tt_scroll_speed_update();
    tt_speed_gauge_update();
    tt_collisions_update();
}
