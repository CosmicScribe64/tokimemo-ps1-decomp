/* ending_audio_visual_update.c: ; src/ovl/ENDING/80137340.c:5-22 dispatches ending scene callbacks and updates audio-level display. Original object 0x80137340-0x801396E0 (config/objects/ENDING.txt). */
#include "common.h"
#include "ovl/ENDING.h"
#include "game.h"

s32 ending_audio_scene_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    EdFnTbl6 tbl;

    tbl = ending_audio_scene_step_callbacks;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
    if (game_state.mode_step >= 3 || game_state.mode_stage >= 6) {
        return 0;
    }
    ending_scroll_dialogue_text_entries();
    k_disp_inc();
    if (tbl.f[game_state.mode_step] == ending_scene_dialogue_table_bind) {
        ending_audio_level_update();
    }
    _sprite_set_box_shade_tarao(-0xA0, -0x78, 0x140, 0xF0, 0xE, 0xC0C0FF, 0x4080FF);
    dtd_on(0xE);
    ending_audience_actor_blink_update();
    ending_audio_scene_character_dispatch();
    if (game_state.frame_count % 240 == 0 && D_80122D0C == 0) {
        ending_audio_scene_flag_normalize();
    }
}

void ending_audio_scene_options_exit_check(void) {
    if (game_state.options_dirty != 0) {
        game_mode_set(GAME_MODE_ETC_C2);
        return;
    }
    mode_stage_next();
}

void ending_audio_scene_shade_wait(void) {
    _sprite_set_box_shade_tarao(-0xA0, -0x78, 0x140, 0xF0, 0xE, 0xC0C0FF, 0x4080FF);
    dtd_on(0xE);
    if (game_state.mode_step_timer.w++ >= 0x81U) {
        mode_step_next();
    }
}

s32 ending_audio_scene_flag_normalize(void)
{
  u8 *byte;
  s16 promoted;
  u8 value;
  promoted = D_8011F4CA;
  value = promoted;
  byte = &value;
  if (((*byte) & 1) == 0)
  {
    value |= 3;
    D_8011F4CA = *byte;
  }
}

void ending_audio_scene_fade_out_update(void) {
    s16 i;
    s32 r;
    s32 g;
    s32 c1;
    s32 c2;

    k_reset(0);
    actor_pool[0x843] -= 1;
    if (actor_pool[0x843] < 8U) {
        actor_pool[0x843] = 0;
        mode_step_next();
    }
    actor_pool[0x7FF] = actor_pool[0x843];
    for (i = 4; i < 0x13; i++) {
        func_80084C98(i, actor_pool[0x843]);
    }
    for (i = 0; i < 0xE; i++) {
        (actor_pool + i * 0x44)[0x1987] = actor_pool[0x843];
    }
    /* FAKE: T-24131; dead CSE seed gives the 0xFF channel $v0 ahead of r ($v1) and c1 ($t0), and the dead store in the 7th call argument makes cfe evaluate (c2 << 16) | (g << 8) before the 0xFF CSE operand. */
    r = (((u32)(actor_pool[0x843] * 0xFF) >> 7) & 0xFF) + (((u32)(actor_pool[0x843] * 0xFF) >> 7) & 0xFF);
    c1 = r;
    r = ((u32)(actor_pool[0x843] * 0xC0) >> 7) & 0xFF;
    c1 = (r << 16) | (r << 8);
    c1 |= ((u32)(actor_pool[0x843] * 0xFF) >> 7) & 0xFF;
    c2 = ((u32)(actor_pool[0x843] * 0x40) >> 7) & 0xFF;
    g = ((u32)(actor_pool[0x843] * 0x80) >> 7) & 0xFF;
    _sprite_set_box_shade_tarao(-0xA0, -0x78, 0x140, 0xF0, 0xE, c1, (((u32)(actor_pool[0x843] * 0xFF) >> 7) & 0xFF) | (r = (c2 << 16) | (g << 8)));
    dtd_on(0xE);
}

void ending_audio_scene_state_reset(void) {
    D_80122CE4 = 0;
    ending_audio_scene_text_timer = 0;
    ending_audio_scene_tick_timer = 0;
    mode_step_next();
}

/* Falls through without a return statement on the last path (K&R), as the original does. T-16930 */
s32 ending_audio_level_update(void) {
    GameState *g = &game_state;
    s16 *cnt;
    s16 i;
    s32 best;
    s32 imax;
    s32 lvl;
    u32 tick;
    s32 b;
    u8 v;
    TextEntry *e;

    cnt = (s16 *)get_ksys();
    if (ending_scene_step_index <= 0) {
        return 0;
    }
    b = ((u8 **)ending_scene_dialogue_tables.unk_04)[ending_scene_dialogue_page_index][ending_scene_step_index - 1];
    if ((b == 0) || (b == 6)) {
        for (i = 1; i <= *cnt; i++) {
            e = (TextEntry *)get_k_work(i);
            e->active = 1;
        }
        D_8011F524 = 0;
        return 0;
    }
    if (cd_audio_state() & 4) {
        best = getCDlevel();
    } else {
        best = 0;
    }
    tick = g->frame_count;
    D_80125C80[tick & 3] = best;
    best = D_80125C80[0];
    imax = 0;
    if (best < D_80125C80[1]) {
        best = D_80125C80[1];
        imax = 1;
    }
    if (best < D_80125C80[2]) {
        best = D_80125C80[2];
        imax = 2;
    }
    if (best < D_80125C80[3]) {
        best = D_80125C80[3];
        imax = 3;
    }
    i = 0;
    lvl = 0;
    best = 0;
    for (; i < 4; i++) {
        if ((i != imax) && (best < D_80125C80[i])) {
            best = D_80125C80[i];
        }
    }
    for (i = 0; i < 3; i++) {
        v = D_800B5A68[g->current_girl][i];
        if (i == 0) {
            v = 8;
        }
        if (v < best) {
            lvl = i + 1;
        }
    }
    D_800B5A60 = 0;
    if ((g->current_girl == GIRL_SHIORI) && (D_8011F522 == 8)) {
        D_800B5A60 = 1;
    }
    if (lvl >= 4) {
        lvl = 3;
    }
    if (lvl != 0) {
        if (D_800B5A60 != 0) {
            lvl = 3 - lvl;
        }
    }
    if ((D_8011F524 != 0) && (tick & 1)) {
    } else if (best == 0) {
        D_8011F524 = 0;
    } else if (lvl == 0) {
        D_8011F524 = 0;
    } else if (lvl == 1) {
        D_8011F524 = 1;
    } else if (lvl == 2) {
        D_8011F524 = 2;
    } else if (lvl >= 3) {
        D_8011F524 = 3;
    }
    if (g->pad_held != 0) {
        for (i = 1; i <= *cnt; i++) {
            e = (TextEntry *)get_k_work(i);
            if (e->y >= 0x48) {
                e->active = 1;
            }
        }
        return 0;
    }
    for (i = 1; i <= *cnt; i++) {
        e = (TextEntry *)get_k_work(i);
        e->active = 0;
    }
}

void ending_audio_scene_character_dispatch(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        ending_audio_scene_update_shiori();
        return;
    case GIRL_MIO:
        ending_audio_scene_update_mio();
        return;
    case GIRL_YUINA:
        ending_audio_scene_update_yuina();
        return;
    case GIRL_AYAKO:
        ending_audio_scene_update_ayako();
        return;
    case GIRL_SAKI:
        ending_audio_scene_update_saki();
        return;
    case GIRL_YUKARI:
        ending_audio_scene_update_yukari();
        return;
    case GIRL_NOZOMI:
        ending_audio_scene_update_nozomi();
        return;
    case GIRL_MIRA:
        ending_audio_scene_update_mira();
        return;
    case GIRL_YUKO:
        ending_audio_scene_update_yuko();
        return;
    case GIRL_MEGUMI:
        ending_audio_scene_update_megumi();
        return;
    case GIRL_YUMI:
        ending_audio_scene_update_yumi();
        return;
    case GIRL_REI:
        ending_audio_scene_update_rei();
        return;
    default:
        ending_audio_scene_update_default();
        return;
    }
}

/* FAKE: T-24104 (cf. T-22028, T-9410); s32 return without a return statement matches the implicit-int call in ending_audio_scene_character_dispatch and keeps $v0 live at exit so as1 does not hoist `li $v0, 1` across `beqz $a0`; dead entry loads of ending_audio_scene_text_timer and ending_audio_scene_branch_state satisfy cvt_pass's entry condition for variable-first bne order. */
s32 ending_audio_scene_update_shiori(void) {
    u32 remainder;
    u32 temp;
    u8 dummy;

    dummy = ending_audio_scene_text_timer;
    dummy = ending_audio_scene_branch_state;
    if (ending_scene_step_index == 1) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 7;
            ending_audio_scene_state_apply(0, 0xFF);
            D_80122D0C = 0;
            ending_audio_scene_branch_state = 0;
        }
    } else if (ending_scene_step_index == 0xE) {
        if (ending_audio_scene_branch_state == 0 && ending_audio_scene_text_timer == 0x32) {
            ending_audio_scene_branch_state = 1;
            ending_audio_scene_text_timer = 0;
        }
        if (ending_audio_scene_text_timer == 1 && D_8011F4DE == 0 && ending_audio_scene_branch_state == 1) {
            ending_audio_scene_state_apply(1, 5);
            D_80122D0C = 1;
        }
    } else if (ending_scene_step_index == 0x11 && ending_audio_scene_tick_timer != 0) {
        if (ending_audio_scene_tick_timer == 1) {
            ((Actor *)actor_pool)[31].sequence_slot = 8;
            ending_audio_scene_state_apply(2, 1);
            D_80122D0C = 1;
        } else if (ending_audio_scene_tick_timer == 0x28) {
            ending_audio_scene_state_apply(3, 5);
        } else if (D_8011F4DE == 3 && D_8011F4E0 == 5) {
            ending_audio_scene_state_apply(2, 1);
        } else if (ending_audio_scene_tick_timer == 0x78) {
            ending_audio_scene_state_apply(4, 5);
        } else if (D_8011F4DE == 4 && D_8011F4E0 == 7) {
            ending_audio_scene_state_apply(5, 1);
            D_80122D0C = 0;
        }
    }
    if (ending_scene_step_index >= 0x12) {
        /* FAKE: T-22028; the dead test on a word copy selects v1, the unsigned copy keeps divu (T-9410). */
        temp = game_state.frame_count;
        if (temp) {
        }
        remainder = temp % 240;
        if (remainder == 0) {
            D_8011F4DE = 6;
            D_8011F4E0 = 0;
            D_8011F4D0 = 0;
        }
        if (remainder >= 0xCU) {
            D_8011F4E0 = 0;
            D_8011F4CA = 1;
            D_8011F4D0 = 0;
            D_8011F4DE = 5;
        }
    }
    if (game_state.mode_step == 2) {
        D_8011F4DE = 5;
        D_8011F4CA = 1;
    }
    ending_audio_scene_tick_timer += 1;
}

void ending_audio_scene_update_mio(void) {
    s32 off;

    if (ending_scene_step_index == 1) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 4;
            ending_audio_scene_state_apply(0, 0xFF);
            D_80122D0C = 0;
        }
    } else {
        /* FAKE: dead test on the flag word selects a0 (T-24104, cf. T-9410). */
        if (game_state.girls[1].flags.u) {
        }
        off = game_state.girls[1].flags.f.b14 >= 1;
        off = off * 2;
        if (ending_scene_step_index == off + 0x13) {
            if (ending_audio_scene_tick_timer == 1) {
                ((Actor *)actor_pool)[31].sequence_slot = 5;
                ending_audio_scene_state_apply(1, 1);
                D_80122D0C = 1;
            }
        } else if (ending_scene_step_index == off + 0x16) {
            if (ending_audio_scene_tick_timer == 1) {
                ending_audio_scene_state_apply(2, 0);
                D_80122D0C = 0;
            }
        } else if (ending_scene_step_index == off + 0x17 && ending_audio_scene_tick_timer == 1) {
            ending_audio_scene_state_apply(3, 5);
            D_80122D0C = 1;
        }
    }
    ending_audio_scene_tick_timer += 1;
}

/* D_8011F4D0/F4DE/F4E0/F522 are fields +0x18/+0x26/+0x28 of the 0x44-byte records 30 and 31 of the
 * actor_pool table; read through the table they compare in the original's operand order (T-9020). */
void ending_audio_scene_update_yuina(void) {
    if (ending_scene_step_index == 1) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 5;
            ending_audio_scene_state_apply(0, 0xFF);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 0x17) {
        if (ending_audio_scene_tick_timer == 0) {
        } else if (ending_audio_scene_tick_timer == 1) {
            ((Actor *)actor_pool)[31].sequence_slot = 6;
            ending_audio_scene_state_apply(1, 5);
            D_80122D0C = 1;
        } else if (((Actor *)actor_pool)[30].sequence_slot == 1 && ((Actor *)actor_pool)[30].unk_18 == 3) {
            ending_audio_scene_state_apply(2, 1);
        }
    } else if (ending_scene_step_index == 0x18) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(3, 0);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 0x1B && ending_audio_scene_tick_timer == 0) {
        ending_audio_scene_state_apply(4, 5);
        D_80122D0C = 1;
    }
    ending_audio_scene_tick_timer += 1;
}

void ending_audio_scene_update_ayako(void) {
    if (ending_scene_step_index == 1) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 4;
            ending_audio_scene_state_apply(0, 0xFF);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 0xD) {
        if (ending_audio_scene_tick_timer == 1) {
            ((Actor *)actor_pool)[31].sequence_slot = 5;
            ending_audio_scene_state_apply(1, 1);
            D_80122D0C = 1;
        }
    } else if (ending_scene_step_index == 0xE) {
        if (ending_audio_scene_tick_timer == 1) {
            ((Actor *)actor_pool)[31].sequence_slot = 4;
            ending_audio_scene_state_apply(0, 0);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 0x12) {
        if (ending_audio_scene_tick_timer == 1) {
            ((Actor *)actor_pool)[31].sequence_slot = 5;
            ending_audio_scene_state_apply(1, 1);
            D_80122D0C = 1;
        }
    } else if (ending_scene_step_index == 0x15) {
        if (ending_audio_scene_tick_timer == 1) {
            ((Actor *)actor_pool)[31].sequence_slot = 4;
            ending_audio_scene_state_apply(0, 0);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 0x17) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 6;
            ending_audio_scene_state_apply(2, 1);
            D_80122D0C = 1;
        }
    } else if (ending_scene_step_index == 0x19 && ending_audio_scene_tick_timer == 1) {
        ending_audio_scene_state_apply(3, 0);
        D_80122D0C = 0;
    }
    ending_audio_scene_tick_timer += 1;
}

void ending_audio_scene_update_saki(void) {
    u32 remainder;

    if (ending_scene_step_index == 1) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 6;
            ending_audio_scene_state_apply(0, 0xFFU);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 8) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(1, 0xFFU);
        }
    } else if (ending_scene_step_index == 9) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(0, 0xFFU);
        }
    } else if (ending_scene_step_index == 0xA) {
        if (ending_audio_scene_tick_timer == 0xF0) {
            ((Actor *)actor_pool)[31].sequence_slot = 7;
            ending_audio_scene_state_apply(1, 0xFFU);
        }
    } else if (ending_scene_step_index == 0xC) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(0, 0xFFU);
        }
    } else if (ending_scene_step_index == 0xF) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 6;
            ending_audio_scene_state_apply(1, 0xFFU);
        }
    } else if (ending_scene_step_index == 0x11) {
        if (ending_audio_scene_tick_timer == 1) {
            ((Actor *)actor_pool)[31].sequence_slot = 8;
            ending_audio_scene_state_apply(2, 1U);
            D_80122D0C = 1;
        }
        if (ending_audio_scene_tick_timer == 0x8C) {
            ending_audio_scene_state_apply(4, 5U);
        }
    } else if (ending_scene_step_index == 0x12) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(2, 1U);
            D_80122D0C = 0;
        }
    } else if ((game_state.mode_step == 2) && (game_state.mode_step_timer.w == 0)) {
        ending_audio_scene_state_apply(5, 5U);
        D_80122D0C = 1;
    }
    if ((ending_scene_step_index >= 0x12) && (D_8011F4DE < 4)) {
        /* FAKE: this dead test puts the dividend in v1 and removes one
         * temporary from the remainder chunk (T-9410). */
        if (game_state.frame_count) {
        }
        remainder = game_state.frame_count % 240;
        if (remainder == 0) {
            D_8011F4DE = 3;
            D_8011F4E0 = 0;
            D_8011F4D0 = 0;
        }
        if (remainder >= 8U) {
            D_8011F4DE = 2;
            D_8011F4E0 = 0;
            D_8011F4D0 = 0;
            D_8011F4CA = 1;
        }
    }
    ending_audio_scene_tick_timer += 1;
}

/* The two table compares read actor_pool through the table view (T-9020), which gives the original's operand order
 * without the former FAKE local. T-16930 */
void ending_audio_scene_update_yukari(void) {
    u32 remainder;
    if (ending_scene_step_index == 1) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 7;
            ending_audio_scene_state_apply(0, 0xFF);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 0xB) {
        if (ending_audio_scene_tick_timer == 0xFA) {
            ending_audio_scene_state_apply(1, 0xFF);
        }
    } else if (ending_scene_step_index == 0xD) {
        if (ending_audio_scene_tick_timer == 1) {
            ((Actor *)actor_pool)[31].sequence_slot = 8;
            ending_audio_scene_state_apply(2, 5);
            D_80122D0C = 1;
        } else if (ending_audio_scene_tick_timer == 0x64) {
            ending_audio_scene_state_apply(3, 5);
        } else if (D_8011F4DE == 3 && ((Actor *)actor_pool)[30].unk_18 == 1 && ((Actor *)actor_pool)[30].unk_08 == 1) {
            ending_audio_scene_state_apply(4, 1);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 0x11) {
        if (ending_audio_scene_tick_timer == 0x172) {
            ending_audio_scene_state_apply(6, 5);
            D_80122D0C = 1;
        }
    }
    if (ending_scene_step_index >= 0xD) {
        /* FAKE: dead test selects v1 and removes a remainder temporary (T-9410). */
        if (game_state.frame_count) {
        }
        remainder = game_state.frame_count % 240;
        if (remainder == 0) {
            if (D_8011F4DE == 4) {
                D_8011F4DE = 5;
                D_8011F4E0 = 0;
                D_8011F4D0 = 0;
            }
        }
        if ((remainder >= 0xCU) && (D_8011F4DE == 5)) {
            D_8011F4DE = 4;
            D_8011F4E0 = 0;
            D_8011F4CA = 1;
            D_8011F4D0 = 0;
        }
    }
    ending_audio_scene_tick_timer += 1;
    if (ending_scene_step_index == 9 && ending_audio_scene_tick_timer != 0) {
        k_speed_set(TEXT_SPEED_EVERY_FIFTEEN_FRAMES);
    }
}

/* FAKE: T-25148; the four N - ((flag & 1) == 1) tests are spelled differently (plain, ^ 0, * 1, << 0) so that uopt
 * shares the AND in v1 but recomputes xor/sltiu per test, as the original does; with one spelling uopt keeps the
 * boolean itself. `* 1` on D_8011F4DE/D_8011F4D0 gives the original's constant-first bne (T-25002). */
void ending_audio_scene_update_nozomi(void) {
    u8 flag;
    u8 c;

    flag = game_state.girl_cleared[6];
    if (ending_scene_step_index == 1) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 7;
            ending_audio_scene_state_apply(0, 0xFF);
            D_80122D0C = 0;
            ending_audio_scene_branch_state = 0;
        }
    } else if (ending_scene_step_index == 8) {
        if (ending_audio_scene_tick_timer == 0xC8) {
            ending_audio_scene_state_apply(1, 5);
            D_80122D0C = 1;
        } else if (D_8011F4DE * 1 == 1 && D_8011F4E0 == 2 && D_8011F4D0 * 1 == 1) {
            ending_audio_scene_state_apply(2, 0);
            D_80122D0C = 0;
        }
    } else {
        if (ending_scene_step_index == 0xC - ((flag & 1) == 1)) {
            if (ending_audio_scene_branch_state == 0 && ending_audio_scene_text_timer == 0xA) {
                ending_audio_scene_branch_state = 1;
                ending_audio_scene_text_timer = 0;
            }
            /* FAKE: T-25148; ending_audio_scene_branch_state read through ending_audio_scene_step_callbacks - 4 into a local. uopt then keeps the load first
             * (`bne t0,a1`); with the plain symbol tools/cvt_pass.py's constant-first fix-up swaps it, because
             * ending_audio_scene_update_shiori's entry loads put ending_audio_scene_branch_state in the unit's set of rewritten globals. */
            c = ((u8 *)&ending_audio_scene_step_callbacks)[-4];
            if (ending_audio_scene_text_timer == 0x1E && D_8011F4DE == 2 && c == 1) {
                ending_audio_scene_state_apply(3, 0);
                ((Actor *)actor_pool)[31].sequence_slot = 8;
            }
        } else if (ending_scene_step_index == 0xE - (((flag & 1) ^ 0) == 1)) {
            if (ending_audio_scene_tick_timer == 0) {
                ending_audio_scene_state_apply(0, 0);
            }
        } else if (ending_scene_step_index == 0x13 - ((flag & 1) * 1 == 1)) {
            if (ending_audio_scene_tick_timer == 0) {
                ((Actor *)actor_pool)[31].sequence_slot = 7;
            }
        } else if (ending_scene_step_index == 0x16 - (((flag & 1) << 0) == 1)) {
            if (ending_audio_scene_tick_timer == 0) {
                ((Actor *)actor_pool)[31].sequence_slot = 8;
            }
        } else if (ending_scene_step_index == 0x18) {
            if (ending_audio_scene_tick_timer == 1) {
                ending_audio_scene_state_apply(4, 5);
                D_80122D0C = 1;
            }
        } else if (ending_scene_step_index == 0x1A) {
            if (ending_audio_scene_tick_timer == 0) {
                ((Actor *)actor_pool)[31].sequence_slot = 7;
            }
        } else if (ending_scene_step_index == 0x1C) {
            if (ending_audio_scene_tick_timer == 0) {
            } else if (ending_audio_scene_tick_timer == 1) {
                ending_audio_scene_state_apply(5, 5);
            } else if (D_8011F4DE == 5 && D_8011F4E0 == 4 && D_8011F4D0 == 0xD) {
                ending_audio_scene_state_apply(6, 1);
            }
        }
    }
    ending_audio_scene_tick_timer += 1;
}

void ending_audio_scene_update_mira(void) {
    if (ending_scene_step_index == 1) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 7;
            ending_audio_scene_state_apply(0, 0xFF);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 6) {
        if (ending_audio_scene_tick_timer == 1) {
            ending_audio_scene_state_apply(1, 5);
            D_80122D0C = 1;
        }
    } else if (ending_scene_step_index == 8) {
        if (ending_audio_scene_tick_timer == 1) {
            ending_audio_scene_state_apply(2, 5);
        }
    } else if (ending_scene_step_index == 9) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(0, 0);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 0x18) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(3, 5);
            D_80122D0C = 1;
        } else if (((Actor *)actor_pool)[30].sequence_slot == 3 && ((Actor *)actor_pool)[30].unk_18 == 2 && ((Actor *)actor_pool)[30].unk_08 == 1) {
            ending_audio_scene_state_apply(4, 0);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 0x1C) {
        ((Actor *)actor_pool)[31].sequence_slot = 8;
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(5, 0xFF);
        }
    } else if (ending_scene_step_index == 0x1E && ending_audio_scene_tick_timer == 0x16D) {
        ending_audio_scene_state_apply(6, 5);
        D_80122D0C = 1;
    }
    ending_audio_scene_tick_timer += 1;
}

void ending_audio_scene_update_yuko(void) {
    if (ending_scene_step_index == 1) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 5;
            ending_audio_scene_state_apply(0, 0xFF);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 7) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 6;
        }
    } else if (ending_scene_step_index == 9) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 5;
            ending_audio_scene_state_apply(1, 5);
            D_80122D0C = 1;
        }
    } else if (ending_scene_step_index == 0xB) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(2, 0);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 0xF) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 7;
            ending_audio_scene_state_apply(3, 5);
            D_80122D0C = 1;
        } else if (ending_audio_scene_tick_timer == 0x3C) {
            ending_audio_scene_state_apply(4, 0);
            D_80122D0C = 0;
        }
    }
    ending_audio_scene_tick_timer += 1;
}

void ending_audio_scene_update_megumi(void) {
    if (ending_scene_step_index == 1) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 8;
            ending_audio_scene_state_apply(0, 0xFF);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 6) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(1, 5);
            D_80122D0C = 1;
        }
    } else if (ending_scene_step_index == 8) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(0, 0);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 0xB) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(2, 5);
            D_80122D0C = 1;
        } else if (((Actor *)actor_pool)[30].sequence_slot == 2 && ((Actor *)actor_pool)[30].unk_18 == 2 && ((Actor *)actor_pool)[30].unk_08 == 1) {
            ending_audio_scene_state_apply(3, 0);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 0xD) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(4, 5);
            D_80122D0C = 1;
        }
    } else if (ending_scene_step_index == 0xF) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(5, 5);
        }
        if (ending_audio_scene_tick_timer == 0xD2) {
            ((Actor *)actor_pool)[31].sequence_slot = 9;
            ending_audio_scene_state_apply(6, 0);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 0x14 && ending_audio_scene_tick_timer == 0xC8) {
        ending_audio_scene_state_apply(4, 5);
        D_80122D0C = 1;
    }
    ending_audio_scene_tick_timer += 1;
}

void ending_audio_scene_update_yumi(void) {
    u32 remainder;

    if (ending_scene_step_index == 1) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 6;
            ending_audio_scene_state_apply(0, 0xFFU);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 9) {
        if (ending_audio_scene_tick_timer == 0xDC) {
            ending_audio_scene_state_apply(1, 1U);
            D_80122D0C = 1;
        }
    } else if (ending_scene_step_index == 0xC) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 7;
            ending_audio_scene_state_apply(2, 1U);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 0x10) {
        if (ending_audio_scene_tick_timer == 1) {
            ending_audio_scene_state_apply(4, 5U);
            D_80122D0C = 1;
        } else if (ending_audio_scene_tick_timer == 0x5A) {
            ending_audio_scene_state_apply(2, 1U);
            D_80122D0C = 0;
        }
    } else if ((ending_scene_step_index == 0x14) && (ending_audio_scene_tick_timer == 0xC8)) {
        ending_audio_scene_state_apply(5, 5U);
        D_80122D0C = 1;
    }
    if (ending_scene_step_index >= 0xC) {
        /* FAKE: dead test selects v1 and removes a remainder temporary (T-9410). */
        if (game_state.frame_count) {
        }
        remainder = game_state.frame_count % 240;
        if (remainder == 0) {
            if (D_8011F4DE == 2) {
                D_8011F4DE = 3;
                D_8011F4E0 = 0;
                D_8011F4D0 = 0;
            }
        }
        if ((remainder >= 0xBU) && (D_8011F4DE == 3)) {
            D_8011F4DE = 2;
            D_8011F4E0 = 0;
            D_8011F4D0 = 0;
            D_8011F4CA = 1;
        }
    }
    ending_audio_scene_tick_timer += 1;
}

void ending_audio_scene_update_rei(void) {
    u32 remainder;
    if (ending_scene_step_index == 1) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 4;
            ending_audio_scene_state_apply(0, 0xFF);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 19) {
        if (ending_audio_scene_tick_timer == 170) {
            ending_audio_scene_state_apply(1, 1);
        }
    } else if (ending_scene_step_index == 23) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(0, 0);
        }
    } else if (ending_scene_step_index == 32) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(2, 1);
        }
    } else if (ending_scene_step_index == 34) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 5;
        }
    } else if (ending_scene_step_index == 37) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 4;
            ending_audio_scene_state_apply(3, 0);
        }
    } else if (ending_scene_step_index == 42) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(0, 0);
        }
    } else if (ending_scene_step_index == 44) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 5;
            ending_audio_scene_state_apply(2, 1);
        }
    }
    if ((ending_scene_step_index >= 0x13 && ending_scene_step_index < 0x17) || (ending_scene_step_index >= 0x22 && ending_scene_step_index < 0x25) || ending_scene_step_index >= 0x2C) {
        /* FAKE: dead test selects v1 and removes a remainder temporary (T-9410). */
        if (game_state.frame_count) {
        }
        remainder = game_state.frame_count % 240;
        if (remainder == 0) {
            if (D_8011F4DE == 2) {
                D_8011F4DE = 3;
                D_8011F4E0 = 0;
                D_8011F4D0 = 0;
            }
        }
        if ((remainder >= 0xAU) && (D_8011F4DE == 3)) {
            D_8011F4DE = 2;
            D_8011F4E0 = 0;
            D_8011F4D0 = 0;
            D_8011F4CA = 1;
        }
    }
    if (game_state.mode_step == 2) {
        D_8011F4DE = 2;
        D_8011F4CA = 1;
    }
    ending_audio_scene_tick_timer += 1;
}

void ending_audio_scene_update_default(void) {
    if (ending_scene_step_index == 1) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 5;
            ending_audio_scene_state_apply(0, 0xFF);
            D_80122D0C = 0;
            ending_audio_scene_branch_state = 0;
        }
    } else if (ending_scene_step_index == 7) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(1, 0);
            D_80122D0C = 1;
        }
    } else if (ending_scene_step_index == 8) {
        if (ending_audio_scene_tick_timer == 0xC8) {
            ending_audio_scene_state_apply(0, 5);
            D_80122D0C = 0;
        }
    } else if (ending_scene_step_index == 0xB) {
        if (ending_audio_scene_branch_state == 0 && ending_audio_scene_text_timer == 0xA) {
            ending_audio_scene_branch_state = 1;
            ending_audio_scene_text_timer = 0;
        }
        if (ending_audio_scene_text_timer == 0x1E && D_8011F4DE == 0 && ending_audio_scene_branch_state == 1) {
            ending_audio_scene_state_apply(2, 0);
            ((Actor *)actor_pool)[31].sequence_slot = 6;
            D_80122D0C = 1;
        }
    } else if (ending_scene_step_index == 0xE) {
        if (ending_audio_scene_tick_timer == 0) {
            ((Actor *)actor_pool)[31].sequence_slot = 7;
            ending_audio_scene_state_apply(3, 1);
        } else if (ending_audio_scene_tick_timer == 0x8C) {
            ending_audio_scene_state_apply(4, 5);
        }
    } else if (ending_scene_step_index == 0xF) {
        if (ending_audio_scene_tick_timer == 0) {
            ending_audio_scene_state_apply(3, 1);
        }
    } else if (ending_scene_step_index == 0x10 && ending_audio_scene_tick_timer == 0) {
        ending_audio_scene_state_apply(4, 5);
    }
    ending_audio_scene_tick_timer += 1;
}

void ending_audio_scene_state_apply(s16 arg0, u8 arg1) {
    D_8011F4DE = arg0;
    D_8011F4E0 = 0;
    D_8011F4D0 = 0;
    if (arg1 != 0xFF) {
        D_8011F4CA = arg1;
    }
}
