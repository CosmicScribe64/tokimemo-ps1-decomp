#include "common.h"
#include "ovl/ETC.h"

/* NON_MATCHING: T-24012, RGB rolling buffer; frame/RECT exact, row base and arithmetic allocation differ. */
void func_801328A4(s32 arg0, s32 arg1) {
    RECT rect;
    Rgb555Split rows[822];
    s32 row;
    s32 i;
    s32 next_row;
    Rgb555Split *p;
    Rgb555Split *q;
    Rgb555Split *next;
    Rgb555Split *current;
    Rgb555Split *previous;
    Rgb555Split *a;
    u16 *input;
    u16 *output;
    u32 r;
    u32 g;
    u32 b;
    s16 pixel;

    rect.x = 0x157;
    rect.y = 0x113;
    rect.w = 0x112;
    rect.h = 0xA0;
    func_8009C8E0(&rect, (void *)0x80198000);
    func_8009C674(0);
    input = (u16 *)0x80198000;
    output = (u16 *)0x80198000;
    row = 0;
    do {
        p = rows;
        if (row == 0) {
            q = rows + 548;
            do {
                p[548].color = *input;
                func_80132880(q);
                p->color = *input;
                func_80132880(p);
                q++;
                p++;
                input++;
            } while (q != rows + 822);
        }
        next_row = row + 1;
        i = 0;
        next = (rows + (next_row % 3) * 274);
        current = (rows + (row % 3) * 274);
        do {
            p = (Rgb555Split *)((u8 *)next + i);
            if (row == 159) {
                p->color = ((Rgb555Split *)((u8 *)current + i))->color;
                func_80132880(p);
            } else {
                p->color = *input;
                func_80132880(p);
                input++;
            }
            i += 16;
        } while (i < 0x1120);
        previous = (rows + ((row + 2) % 3) * 274);
        output++;
        i = 16;
        do {
            p = (Rgb555Split *)((u8 *)current + i);
            q = (Rgb555Split *)((u8 *)previous + i);
            a = (Rgb555Split *)((u8 *)next + i);
            g = (u32)(p[1].g + q->g + a->g + p[-1].g) >> 2;
            r = (u32)(p[1].r + q->r + a->r + p[-1].r) >> 2;
            b = (u32)(p[1].b + q->b + a->b + p[-1].b) >> 2;
            pixel = (((u32)(p->r * arg0) / 100U + r * arg1 / 100U) & 0x1F) |
                      (((u32)(p->g * arg0) / 100U + g * arg1 / 100U) & 0x3E0) |
                      (((u32)(p->b * arg0) / 100U + b * arg1 / 100U) & 0x7C00);
            *output = pixel;
            *output = (pixel & 0xFFFF) | 0x8000;
            i += 16;
            output++;
        } while (i != 0x1110);
        row = next_row;
        output++;
    } while (row != 160);
    func_8009C884(&rect, (void *)0x80198000);
    func_8009C674(0);
}
