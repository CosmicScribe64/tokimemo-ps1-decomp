/* model_palette_and_color.c: src/ovl/TACO/80154230.c:7: model/object CLUT replacement, flicker, texture-page and flat-color edits; body 19: count = arg0[0]; 20: limit = arg0[2];. Original object 0x80154230-0x80154960 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

/* The header is walked in two steps (arg0++, then arg0 += 8): as1 turns the first add into the
   entry copy `move s0,a0` and moves its offset into the loads and the second add. One add of 9
   is folded into `addiu s0,a0,36` with the loads on a0. T-16562 */
void shooter_set_model_clut(arg0, arg1)
u32 *arg0;
u16 arg1;
{
    u32 command0;
    u32 command1;
    u32 command;
    u32 word;
    s32 count;
    u32 limit;

    arg0++;
    count = arg0[0];
    limit = arg0[2];
    command0 = 0x24000507;
    command1 = 0x34000609;
    arg0 += 8;
loop_1:
    if (shooter_is_model_address((u32)arg0) != 0) {
        command = arg0[0];
        if (command0 == command || command1 == command) {
            word = arg0[2];
            arg0 += 2;
            *arg0 = (word & 0xFFFF) + (word & 0x01E00000) + ((arg1 & 0x1F) << 0x10);
        }
        arg0++;
        if (count == 0 || limit >= (u32)arg0) {
            if (count == 0 && (u32)(limit * 4 + (u32)arg0 + 0xC) < (u32)arg0) {
            } else {
                goto loop_1;
            }
        }
    }
}

s32 shooter_is_model_address(u32 arg0) {
    if (arg0 < 0x8018A000U) {
        return 0;
    }
    if (arg0 >= 0x801B8001U) {
        return 0;
    }
    return 1;
}

void shooter_set_object_clut(s32 arg0, u16 arg1) {
    u32 **bounds;
    u32 command;
    u32 command0;
    u32 command1;
    u32 command2;
    s32 word;
    u32 *p;

    /* Unsigned command constants preserve the original comparison order. T-10030. */
    command0 = 0x24000507;
    command1 = 0x34000609;
    command2 = 0x24000014;
    bounds = (u32 **)(shooter_model_polygon_ranges + arg0);
    p = bounds[0];
loop_1:
    if (shooter_is_model_address((u32)p) != 0) {
        command = p[0];
        if (command0 == command || command1 == command || command2 == command) {
            word = p[2];
            p += 2;
            *p = (s32) ((word & 0xFFFF) + (word & 0x01E00000) + ((arg1 & 0x1F) << 0x10));
        }
        p++;
        if ((u32) p < (u32) bounds[1]) {
            goto loop_1;
        }
    }
}

/* Header walked in two steps for the entry copy, as in shooter_set_model_clut. command0/command1 are
   folded into the compares but keep their two stack words (the original's 88-byte frame). T-16562 */
void shooter_flicker_model_clut(arg0, arg1, arg2, arg3)
u32 *arg0;
u16 arg1;
s32 arg2;
s32 arg3;
{
    u32 command0;
    u32 command1;
    s32 interval;
    u32 v;
    s32 count;
    u32 limit;
    s32 i;
    u32 word;
    u32 command;

    command0 = 0x24000507;
    command1 = 0x34000609;
    v = shooter_actors[20].mode;
    arg0++;
    count = arg0[0];
    limit = arg0[2];
    i = 0;
    arg0 += 8;
    if (v < 0x10) {
        interval = rand() % 150 + 100;
    } else if (v >= 0x10 && v < 0x20) {
        interval = rand() % 100 + 50;
    } else if (v >= 0x20 && v < 0x30) {
        interval = rand() % 50 + 30;
    } else if (v >= 0x30 && v < 0x40) {
        interval = rand() % 30 + 20;
    } else if (v >= 0x40 && v < 0x64) {
        interval = rand() % 10 + 5;
    }
loop_1:
    if (shooter_is_model_address((u32)arg0) != 0) {
        command = arg0[0];
        if (command == command0 || command == command1) {
            if (shooter_actors[20].mode < 0x50) {
                if (i >= interval) {
                    word = arg0[2];
                    arg0[1] = (arg0[1] & 0xFFFF) + ((arg3 << 0x16) + (((arg2 >> 4) & 0x3F) << 0x10));
                    arg0 += 2;
                    *arg0 = (word & 0xFFFF) + (word & 0x01E00000) + ((arg1 & 0x1F) << 0x10);
                    i = 0;
                }
                i++;
            } else {
                interval = rand() % 70;
                if (interval == 0) {
                    word = arg0[2];
                    arg0[1] = (arg0[1] & 0xFFFF) + ((arg3 << 0x16) + (((arg2 >> 4) & 0x3F) << 0x10));
                    arg0 += 2;
                    *arg0 = (word & 0xFFFF) + (word & 0x01E00000) + ((arg1 & 0x1F) << 0x10);
                }
            }
        }
        arg0++;
        if (count == 0 || limit >= (u32)arg0) {
            if (count == 0 && (u32)(limit * 4 + (u32)arg0 + 0xC) < (u32)arg0) {
            } else {
                goto loop_1;
            }
        }
    }
}

/* Header walked in two steps for the entry copy, as in shooter_set_model_clut. T-16562 */
void shooter_set_model_clut_and_page(arg0, arg1, arg2, arg3)
u32 *arg0;
u16 arg1;
s32 arg2;
s32 arg3;
{
    u32 command0;
    u32 command1;
    u32 command;
    u32 word;
    s32 count;
    u32 limit;

    arg0++;
    count = arg0[0];
    limit = arg0[2];
    command0 = 0x24000507;
    command1 = 0x34000609;
    arg0 += 8;
loop_1:
    if (shooter_is_model_address((u32)arg0) != 0) {
        command = arg0[0];
        if (command0 == command || command1 == command) {
            word = arg0[2];
            arg0[1] = (arg0[1] & 0xFFFF) + ((arg3 << 0x16) + (((arg2 >> 4) & 0x3F) << 0x10));
            arg0 += 2;
            *arg0 = (word & 0xFFFF) + (word & 0x01E00000) + ((arg1 & 0x1F) << 0x10);
        }
        arg0++;
        if (count == 0 || limit >= (u32)arg0) {
            if (count == 0 && (u32)(limit * 4 + (u32)arg0 + 0xC) < (u32)arg0) {
            } else {
                goto loop_1;
            }
        }
    }
}

/* The record word is read as *p each time (no local copy) and r/g/b are first the raw shifted/masked word, then the clamped sum: the shifts stay uopt values in $v1/$a0/$a1 as in the original. T-16562 */
void shooter_set_object_flat_color(s32 arg0, s32 arg1) {
    u32 **bounds;
    u32 *p;
    u32 command0;
    u32 command1;
    u32 command2;
    s32 r;
    s32 g;
    s32 b;

    command0 = 0x21010001;
    command1 = 0x21010002;
    command2 = 0x21010304;
    bounds = (u32 **)(shooter_model_polygon_ranges + arg0);
    p = bounds[0];
loop_1:
    if (shooter_is_model_address((u32)p) != 0) {
        if (command0 == p[0] || command1 == p[0] || command2 == p[0]) {
            p += 1;
            r = *p >> 16;
            g = *p >> 8;
            b = *p & 0xFF;
            r = (r & 0xFF) + arg1;
            g = (g & 0xFF) + arg1;
            b = b + arg1;
            if (r >= 0x100) {
                r = 0xFF;
            }
            if (g >= 0x100) {
                g = 0xFF;
            }
            if (b >= 0x100) {
                b = 0xFF;
            }
            *p = (*p & 0xFF000000) + ((r & 0xFF) << 16) + ((g & 0xFF) << 8) + (b & 0xFF);
        }
        p++;
        if ((u32)p < (u32)bounds[1]) {
            goto loop_1;
        }
    }
}
