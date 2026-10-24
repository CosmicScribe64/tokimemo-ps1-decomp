/* paired_actor_scene_flow.c: src/ovl/KANGEI/801355F0.c:9-36; binds two parallel actor resource records, runs an80-entry step callback table and updates their scene presentation. Original object 0x801355F0-0x80135F10 (config/objects/KANGEI.txt). */
#include "common.h"
#include "ovl/KANGEI.h"

typedef struct KangeiControlPair {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
} KangeiControlPair; /* size 0x02; byte pair at D_80120652, T-10030 */

void func_801355F0(void) {
    D_80139D10[0].unk_00 = 0x801D4074;
    D_80139D10[0].unk_04 = 0x801D4078;
    D_80139D10[0].unk_08 = 0x801D4088;
    D_80139D10[0].unk_0C = *(s16 *)0x801D4094;
}

void func_80135634(void) {
    D_80139D10[1].unk_00 = 0x801D4074;
    D_80139D10[1].unk_04 = 0x801D4078;
    D_80139D10[1].unk_08 = 0x801D4088;
    D_80139D10[1].unk_0C = *(s16 *)0x801D4094;
}

/* The original leaves the K&R result in v0 without an explicit return. */
s32 func_80135678(void) {
    s32 tick;
    KangeiDispatchTable80 table;
    u8 girl;

    table = D_80139D30;
    table.functions[game_state.mode_step](0x80);
    girl = game_state.current_girl;
    /* FAKE: the condition and store share a source line to retain the original
     * timer/girl address-load scheduling. T-10030. */
    if ((D_8011F4CA & 0x10) && (girl != 5)) { D_8011F4CA = 3;
    }
    tick = ((u32)game_state.mode_stage_timer++ % 300U) == 0;
    if (tick != 0) {
        if (D_8011F50F & 0x80) {
            if ((girl == 10) || (girl == 3)) {
                ((KangeiControlPair *)&D_80120652)->unk_00 = 9;
                ((KangeiControlPair *)&D_80120652)->unk_01 |= 0x80;
            }
        }
    }
}

void func_801357A4(void) {
    game_state.current_girl = game_state.date_girl;
    mode_step_next();
}

void func_801357D0(void) {
    audio_command(0x23);
    audio_command(0x24);
    audio_command(0x2E);
    mode_step_next();
}

void func_80135808(void) {
    audio_command(0x24);
    audio_command(0x502);
    mode_step_next();
}

/* FAKE: the date checks read the GameState fields through the pointer g; read directly, IDO swaps $v0/$v1
 * between the date word and unk_040 (register-cheatsheet date-check row). T-18400 */
s32 func_80135838(void) {
    GameState *g;

    g = &game_state;
    if ((u8) D_80139ABC != 0) {
        mode_stage_next();
        return 0;
    }
    if (game_state.date_girl == 0xFF) {
        mode_stage_next();
        return 0;
    }
    srand(game_state.frame_count);
    if (((rand() % 256) >= 5) || (game_state.date_girl >= GIRL_MIHARU)) {
        mode_stage_next();
        return 0;
    }
    if (!game_state.girls[game_state.date_girl].flags.f.met) {
        mode_stage_next();
        return 0;
    }
    if ((((u32) (g->girls[0].birthday.w << 0x17) >> 0x1B) == g->day) && ((g->girls[0].birthday.w & 0xF) == g->month)) {
        mode_stage_next();
        return 0;
    }
    if ((((u32) (g->player_birthday << 0x17) >> 0x1B) == g->day) && ((g->player_birthday & 0xF) == g->month)) {
        mode_stage_next();
        return 0;
    }
    game_state.current_girl = game_state.date_girl;
    D_80139ABC = 1;
    *(s32 *)&game_state.param_condition /= 2;
    *(s32 *)&game_state.girls[game_state.date_girl].syoushin_fraction = 0;
    back_clear_switch(1);
    actor_clear_range(0);
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    message_window_init();
    hizuke_init();
    tpage_buf_clear();
    text_system_reset();
    k_reset(1);
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    talk_clear();
    parameter_disp_switch(0);
    icon_disp_switch(0);
    D_80139ADC = 0;
    D_80139AE0 = 0;
    addr_init_bustup();
    text_marker_hide();
    talk_names_set(game_state.current_girl);
    func_80132E74();
    func_801349D4();
    D_80139AC0 = 0;
    event_log_add(0xE, 0xD);
    return mode_step_next();
}

void func_80135AC0(void) {
    mode_step_next();
}

void func_80135AE0(void) {
    mode_stage_next();
    mode_stage_next();
    mode_stage_next();
    mode_stage_next();
}

INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/paired_actor_scene_flow.rodata", D_801398B0);

void func_80135B18(void) {
    bg_read_sub2(0x42A9);
    strcpy(D_800CA19C, &D_801398B0);
    mode_step_next();
}

void func_80135B54(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        portrait_read_start(0x5747);
        break;
    case GIRL_MIO:
        portrait_read_start(0x5ED6);
        break;
    case GIRL_YUINA:
        portrait_read_start(0x5396);
        break;
    case GIRL_AYAKO:
        portrait_read_start(0x5A44);
        break;
    case GIRL_SAKI:
        portrait_read_start(0x6881);
        break;
    case GIRL_YUKARI:
        portrait_read_start(0x6179);
        break;
    case GIRL_NOZOMI:
        portrait_read_start(0x63EF);
        break;
    default:
        portrait_read_start(0x6ACA);
        break;
    }
    mode_step_next();
}

void func_80135C18(void) {
    normal_date_girl_in();
    if ((game_state.current_girl == GIRL_YUMI) || (game_state.current_girl == GIRL_AYAKO)) {
        ((Actor *)actor_pool)[96].draw_flags |= 0x80;
        ((Actor *)actor_pool)[96].brightness = ((Actor *)actor_pool)[31].brightness;
    }
}

/* FAKE: a no-value s32 return preserves the original early RA reload.
 * The intended return type is unknown. T-9520. */
s32 func_80135C78(void) {
    normal_date_girl_out();
    if ((game_state.current_girl == GIRL_YUMI) || (game_state.current_girl == GIRL_AYAKO)) {
        D_80120657 = D_8011F513;
        if ((u8) D_8011F513 < 8U) {
            D_80120653 &= 0x7F;
        }
    }
}

void func_80135CE0(void) {
    if (game_state.current_girl == GIRL_YUMI) {
        actor_clear(0x60);
        D_80120651 = 0xB;
        D_80120652 = 0;
        D_80120688 = 0x41000000;
        D_80120653 = 4;
        D_8012065C = (u8 *) D_80139D10[0].unk_04;
        D_80120660 = (u8 *) D_80139D10[0].unk_08;
        D_80120684 = (u8 *) D_80139D10[0].unk_00;
        D_80120664 = D_80139D10[0].unk_0C;
        D_80120666 = 0;
        D_80120668 = 0;
        D_80120656 = 2;
        D_80120693 = 0x10;
        D_80120676 = 0;
        D_8012067A = -0x20;
        D_80120657 = 0;
    } else if (game_state.current_girl == GIRL_AYAKO) {
        actor_clear(0x60);
        D_80120651 = 0xB;
        D_80120652 = 0;
        D_80120688 = 0x41000000;
        D_80120653 = 4;
        D_8012065C = (u8 *) D_80139D10[1].unk_04;
        D_80120660 = (u8 *) D_80139D10[1].unk_08;
        D_80120684 = (u8 *) D_80139D10[1].unk_00;
        D_80120664 = D_80139D10[1].unk_0C;
        D_80120666 = 0;
        D_80120668 = 0;
        D_80120656 = 2;
        D_80120693 = 0x10;
        D_80120676 = 0;
        D_8012067A = -0x20;
        D_80120657 = 0;
    }
    mode_step_next();
}

void func_80135E90(void) {
    if (game_state.current_girl == GIRL_YUMI) {
        cd_read_sectors(9, 0x801D0000, 0xBCC8);
        func_801355F0();
    } else {
        cd_read_sectors(9, 0x801D0000, 0xBD95);
        func_80135634();
    }
    audio_bank_start(1, 0);
    mode_step_next();
}
