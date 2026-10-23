#include "common.h"
#include "game.h"

s32 func_80046500(void) {
    func_800789E0();
    func_800412E0(0);
    main_state_set(0x12);
    func_80041584();
    func_80048DD0(0);
    back_clear_switch(1);
    dec_bg_reset();
    func_8004482C();
    tpage_buf_clear();
    func_80044750(0x7F);
    func_8007BDE8();
    func_8007B844();
    func_800452C4();
    func_80041878();
    func_80042058();
    return -1;
}

s32 func_80046590(s16 arg0, u8 arg1) {
    s32 p;
    u32 i;

    p = 1;
    if ((u32)arg1 >= 6) {
        return 0;
    }
    i = 0;
    if (arg1 != 0) {
        do {
            i += 1;
            p *= 10;
        } while (i < (u32)arg1);
    }
    return (arg0 / (p / 10)) % 10;
}

s32 func_80046684(s32 arg0) {
    s32 result;
    s32 mul;
    s32 i;

    result = 0;
    mul = 1;
    for (i = 0; i < 8; i++) {
        result += (mul * (arg0 & (0xF << (i * 4)))) >> (i * 4);
        mul *= 10;
    }
    return result;
}

void func_80046754(void *arg0, s32 arg1, s32 arg2) {
    s32 x;
    s32 y;
    s32 shade;
    u8 color;
    u16 load_x;
    u16 load_y;
    u32 *column;
    u32 *out;

    /* Clear the 128 rows of sixteen packed 4-bit pixel words. */
    x = 0;
    column = arg0;
    for (; x < 16; x++) {
        y = 0;
        out = column;
        for (; y < 128; y++) {
            *out = 0;
            out += 16;
        }
        column++;
    }
    /* High mode selects radial shading; the low nibble selects a flat fill. */
    if ((arg2 / 16) & 0xFF) {
        for (x = 0; x < 128; x++) {
            for (y = 0; y < 128; y++) {
                for (shade = 15; shade != 0; shade--) {
                    if (x * x + y * y < ((15 - shade) * (15 - shade) << 6)) {
                        *((u32 *)arg0 + x / 8 + y * 16) |= shade << ((x % 8) * 4);
                        break;
                    }
                }
            }
        }
    } else {
        color = arg2 & 15;
        for (x = 0; x < 128; x++) {
            for (y = 0; y < 128; y++) {
                if (color) {
                    if (x * x + y * y < 0x3F01) {
                        *((u32 *)arg0 + x / 8 + y * 16) |= color << ((x % 8) * 4);
                    }
                } else if (x * x + y * y >= 0x3F02) {
                    *((u32 *)arg0 + x / 8 + y * 16) |= 1 << ((x % 8) * 4);
                }
            }
        }
    }
    load_x = (arg1 & 15) << 6;
    load_y = ((arg1 & 16) * 16) + ((arg1 & 32) * 4);
    LoadSquare(load_x, load_y, 32U, 128U, arg0);
}

INCLUDE_ASM("asm/nonmatchings/main/80046500", func_80046AC8);
