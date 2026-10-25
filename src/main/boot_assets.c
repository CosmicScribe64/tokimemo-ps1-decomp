/* boot_assets.c: src/main/80059B40.c; Boot-stage state machine reads common assets from CD, relocates texture pointers, loads tpage cache, initializes palettes/UI actors, and proceeds toward title setup; retains all six original definitions. Original object 0x80059B40-0x8005A0B0 (config/objects/main.txt). */
#define MAIN_API_OVERRIDE_actor_default_texture_blocks /* matched as the s32 table it is (11 words up to sprite_frame_table); main_api.h: u8[]; T-17008 */
#define MAIN_API_OVERRIDE_D_800C5FCC /* matched as the two-word table up to D_800C5FD4; main_api.h: s32; T-17008 */
#include "common.h"
#include "game.h"

extern s32 actor_default_texture_blocks[11];
extern s32 D_800C5FCC[2];

void boot_mode_step(void) {
    switch (game_state.mode_step) {
    case 0:
        boot_data_load_step();
        return;
    case 1:
        boot_hud_textures_step();
        return;
    case 2:
        debug_font_load();
        mode_step_next();
        return;
    default:
        mode_stage_next();
        return;
    }
}

void boot_hud_textures_step(void) {
    boot_hud_textures_load();
    mode_step_next();
}

void boot_hud_textures_load(void) {
    s32 i;
    s32 k;

    /* FAKE: the subtract loops need the 0x80000 in a register (`subu rd,rs,s0`, shared with the add loops); a literal `-= 0x80000` becomes `addu` of 0xFFF80000. Real source unknown. T-17008 */
    k = 0x80000;
    if (!(actor_default_texture_blocks[0] & 0x80000)) {
        for (i = 0; i < 11; i++) {
            actor_default_texture_blocks[i] += 0x80000;
        }
        for (i = 0; i < 2; i++) {
            D_800C5FCC[i] += 0x80000;
        }
    }
    load_tpage_buf_lock(0x1F, actor_default_texture_blocks[7], 2);
    load_tpage_buf_lock(0x1E, actor_default_texture_blocks[8], 2);
    load_tpage_buf_lock(0x1D, actor_default_texture_blocks[9], 2);
    load_tpage_buf_lock(0x1C, actor_default_texture_blocks[10], 2);
    load_tpage_buf_lock(0x1B, D_800C5FCC[0], 2);
    load_tpage_buf_lock(0x3B, D_800C5FCC[1], 2);
    load_tpage_buf_lock(0x3C, actor_default_texture_blocks[0], 2);
    load_tpage_buf_lock(0x3D, actor_default_texture_blocks[1], 2);
    load_tpage_buf_lock(0x3E, actor_default_texture_blocks[2], 2);
    load_tpage_buf_lock(0x3F, actor_default_texture_blocks[3], 2);
    for (i = 0; i < 11; i++) {
        actor_default_texture_blocks[i] -= k;
    }
    for (i = 0; i < 2; i++) {
        D_800C5FCC[i] -= k;
    }
    for (i = 0x1B; i < 0x20; i++) {
        game_state.tpage_buf[i] += 0xFFF80000;
    }
    /* do-while with a pre-increment test: IDO peels the first of these five trips and unrolls the rest as the original does; a `for` here also unrolls the loop above. T-17008 */
    i = 0x3B;
    do {
        game_state.tpage_buf[i] += 0xFFF80000;
    } while (++i < 0x40);
}

/* FAKE: u8 views give the original's global access (LOD): `andi`/`or v0` copy of the increment; the s32 return keeps that copy live (T-5010) */
s32 boot_data_load_step(void) {
    if (*(u8 *)&game_state.substep == 0) {
        printf("LDVI");
        back_clear_switch(1);
        vram_clear_blocks(0xFF);
        cd_read_sectors(0x35, 0x80180000, 0x9D85);
        *(u8 *)&game_state.substep += 1;
    } else if (*(u8 *)&game_state.substep == 1) {
        if (cd_read_sync_status() & 1) {
            boot_hud_init();
            cd_read_sectors_nowait(8, 0x801F0000, 0x9C97);
            *(u8 *)&game_state.substep += 1;
        }
    } else if (*(u8 *)&game_state.substep == 2 && (cd_read_sync_status() & 1)) {
        mode_step_next();
    }
}

void boot_hud_init(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 16; i++) {
        for (j = 0; j < 32; j++) {
            if (D_800C62C0[j] & 0x7FFF0000) {
                D_800C62C0[j] |= 0x80000000;
            }
            if (D_800C62C0[j] & 0x7FFF) {
                D_800C62C0[j] |= 0x8000;
            }
        }
        for (j = 0; j < 8; j++) {
            if (D_800C6200[j] & 0x7FFF0000) {
                D_800C6200[j] |= 0x80000000;
            }
            if (D_800C6200[j] & 0x7FFF) {
                D_800C6200[j] |= 0x8000;
            }
        }
        load_palette(&D_800C6020, i & 0xFF, 1, 1, i & 0xFF);
    }
    hizuke_init();
    hizuke_disp_switch(0);
    parameter_show_init();
    parameter_show_init();
    parameter_disp_switch(0);
    hud_icon_bar_init();
    icon_disp_switch(0);
    message_window_init();
    message_disp_switch(0);
    hud_vram_init();
    hud_mascot_display_switch(0);
    message_window_title_show(0);
}

void boot_mode_handler(void) {
    if (game_state.mode_stage == 0) {
        boot_mode_step();
        return;
    }
    game_mode_set(GAME_MODE_TITLE);
}
