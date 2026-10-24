#include "common.h"
#include "game.h"

void func_8004F6B0(void) {
    s32 pad;

    game_state.pad_pressed = 0x80000;
    D_80125BF0 = 0;
    D_80125BF2 = 0;
    pad = game_state.pad_held;
    while (1) {
        if (pad & 0x10000000) {
            D_80125BF2 -= 0x10;
        }
        if (pad & 0x40000000) {
            D_80125BF2 += 0x10;
        }
        if (pad & 0x20000000) {
            D_80125BF0 += 0x10;
        }
        if (pad & 0x80000000) {
            D_80125BF0 -= 0x10;
        }
        if (D_80125BF0 < 0) {
            D_80125BF0 = 0;
        }
        if (D_80125BF2 < 0) {
            D_80125BF2 = 0;
        }
        func_80042A00();
        func_80098380();
        func_80098490(D_80125BF0, D_80125BF2, D_80125BF0, D_80125BF2);
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
