#include "common.h"
#include "ovl/SHOUGATU.h"

/* Bit 2 of the girl record flag byte. T-9720. */
typedef struct ShougatuFlag4 {
    /* 0x00 */ u8 pad : 2;
    /* 0x00 */ u8 set : 1;
    /* 0x00 */ u8 rest : 5;
} ShougatuFlag4; /* size 0x01 */

void func_8013A190(void) {
    if (D_80144E14 == 0) {
        func_8013A1CC();
        return;
    }
    func_8013A27C();
}

void func_8013A1CC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl26 tbl;

    tbl = D_801455E0;
    idx = game_state.step;
    tbl.f[idx]();
}

void func_8013A254(void) {
    snd_command(0x502);
    main_step_next();
}

void func_8013A27C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl19 tbl;

    tbl = D_80145648;
    idx = game_state.step;
    tbl.f[idx]();
}

void func_8013A2F8(void) {
    draw2d3d(1, 0);
    actors_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    dialogue_clear();
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    ((ShougatuFlag4 *) (&game_state.girls[5].flags.b[0]))->set = 1;
    /* FAKE: the dead triple test preserves the original argument/store scheduling.
     * IDO removes all of its reads and tests. T-9720. */
    if ((!D_80122EBC) && (!D_80122EBC) && (!D_80122EBC)) {}
    dialogue_names_set(*((u8 *) (&game_state.current_girl)) = 5);
    func_80136F90();
    D_80144DFC = D_80144DA4;
    D_80144E00 = D_80144DC8;
    D_80144E04 = D_80144DEC;
    main_step_next();
}

void func_8013A3FC(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "グランド");
    main_step_next();
}

void func_8013A440(void) {
    D_80144E08 = 5;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "グランド");
    main_step_next();
}

void func_8013A488(void) {
    bg_read_sub2(0x4097);
    main_step_next();
}

void func_8013A4B0(void) {
    if (game_state.current_girl == GIRL_YUKARI) {
        bustup_read_start(0x5FE4);
    } else {
        bustup_read_start(0x552B);
    }
    main_step_next();
}
