#include "common.h"
#include "ovl/BUNKA_SD.h"

void func_80134260(void) {
    talk_clear();
    actor_clear_upper();
    k_reset(1);
    tpage_buf_clear();
    game_state.clear_color_r = 0;
    game_state.clear_color_b = 0;
    game_state.clear_color_g = 0;
    if (D_80122EB8 == 1) {
        back_clear_switch(0);
    } else {
        back_clear_switch(1);
    }
    draw2d3d(1, 0);
    actor_draw_enable(1);
    hizuke_disp_switch(0);
    sprite_tables_clear();
    D_8013C700 = get_k_speed();
    mode_step_next();
}

s32 func_80134314(void) {
    if (game_state.substep == 0) {
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        game_state.substep += 1;
    } else if (game_state.substep == 1) {
        if (audio_bank_load_poll() == 1) {
            game_state.substep += 1;
        }
    } else {
        tpage_buf_clear();
        actor_clear_upper();
        if (D_80122EB8 == 1) {
            dec_bg_reset();
            back_clear_switch(1);
        }
        hizuke_disp_switch(1);
        k_speed_set(*(u8 *)&D_8013C700);
        mode_stage_set(0x10);
        audio_command(0x200);
    }
}

/* FAKE: u8 views keep one promoted counter range as in func_8013987C; original source unknown. T-24035 */
void func_80134418(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        ((Actor *)actor_pool_96)[i].brightness = *(u8 *)&game_state.game_mode_scratch;
    }
    *(u8 *)&game_state.game_mode_scratch += 2;
    if (*(u8 *)&game_state.game_mode_scratch > 0x80) {
        /* FAKE: single-expression * 0 + 0x80 folds the cap constant and its narrowing in one uopt pass while keeping unk_110B in the single promoted $v1 range. T-24082 */
        *(u8 *)&game_state.game_mode_scratch = *(u8 *)&game_state.game_mode_scratch * 0 + 0x80;
        for (i = 0; i < 0x40; i++) {
            ((Actor *)actor_pool_96)[i].brightness = *(u8 *)&game_state.game_mode_scratch;
        }
        mode_step_next();
    }
}

/* FAKE: T-24035. The u8 views keep the counter in one promoted range; *= 0 preserves its zero register for the clear loop instead of folding the stores to sb zero, as in matched sibling func_8013987C. Original source unknown. */
void func_801344A8(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        ((Actor *)actor_pool_96)[i].brightness = *(u8 *)&game_state.game_mode_scratch;
    }
    if (*(u8 *)&game_state.game_mode_scratch == 0) {
        *(u8 *)&game_state.game_mode_scratch = 2;
    }
    *(u8 *)&game_state.game_mode_scratch -= 2;
    if (*(u8 *)&game_state.game_mode_scratch == 0) {
        *(u8 *)&game_state.game_mode_scratch *= 0;
        for (i = 0; i < 0x40; i++) {
            ((Actor *)actor_pool_96)[i].brightness = *(u8 *)&game_state.game_mode_scratch;
        }
        mode_step_next();
    }
}
