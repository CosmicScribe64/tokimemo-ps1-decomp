/* vram_viewer.c: src/main/8004F6B0.c; T-27059: object [0x8004F6B0,0x8004F870); one mode-0xFE developer viewer scrolls display origin by 16-pixel steps from pad 2 and restores the original video area on exit (reuse reviewed m-menutext filename). Original object 0x8004F6B0-0x8004F870 (config/objects/main.txt). */
#include "common.h"
#include "game.h"

void menu_vram_view_mode_handler(void) {
    s32 pad;

    game_state.pad_pressed = 0x80000;
    menu_vram_view_x = 0;
    menu_vram_view_y = 0;
    pad = game_state.pad_held;
    while (1) {
        if (pad & 0x10000000) {
            menu_vram_view_y -= 0x10;
        }
        if (pad & 0x40000000) {
            menu_vram_view_y += 0x10;
        }
        if (pad & 0x20000000) {
            menu_vram_view_x += 0x10;
        }
        if (pad & 0x80000000) {
            menu_vram_view_x -= 0x10;
        }
        if (menu_vram_view_x < 0) {
            menu_vram_view_x = 0;
        }
        if (menu_vram_view_y < 0) {
            menu_vram_view_y = 0;
        }
        func_80042A00();
        func_80098380();
        func_80098490(menu_vram_view_x, menu_vram_view_y, menu_vram_view_x, menu_vram_view_y);
        func_800422C8();
        func_8009C674(0);
        func_800AD950(0);
        func_8009AC20();
        pad = game_state.pad_held;
        if (pad & 0x08000000) {
            func_80097D90(game_state.video_width, game_state.video_height, game_state.video_interlace, 0, 0);
            func_80098380();
            func_80098490(0, 0, 0, game_state.video_height);
            func_80098530();
            return;
        }
    }
}
