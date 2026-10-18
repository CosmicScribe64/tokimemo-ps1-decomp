#include "common.h"
#include "game.h"
#include "psx_types.h"

s32 func_80049FF0(x0, y0, x1, y1, c0, c1, flags)
s16 x0;
s16 y0;
s16 x1;
s16 y1;
u32 c0;
u32 c1;
u8 flags;
{
    GameState *g;
    LINE_G2 *line;

    g = &D_800E6280;
    line = (LINE_G2 *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F11C(line);
    if (flags & 0x80) {
        SetSemiTrans(line, 1);
    } else {
        SetSemiTrans(line, 0);
    }
    /* FAKE: explicit colour masks and comma grouping preserve renderer scheduling (T-20600, T-24041). */
    line->r0 = (c0 >> 16) & 0xFF, line->g0 = (c0 >> 8) & 0xFF, line->b0 = c0 & 0xFF;
    line->r1 = (c1 >> 16) & 0xFF, line->g1 = (c1 >> 8) & 0xFF, line->b1 = c1 & 0xFF;
    line->x0 = g->unk_014[D_8011ECA0] + x0;
    line->y0 = g->unk_018[D_8011ECA0] + y0;
    line->x1 = g->unk_014[D_8011ECA0] + x1;
    line->y1 = g->unk_018[D_8011ECA0] + y1;
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 10) + ((flags & 0x7F) * 4)], line);
}

void func_8004A14C(s32 arg0, RECT rect, s32 arg3) {
    func_80049A40(rect.x, rect.y, rect.w, rect.h, 1, arg3, 1);
}

INCLUDE_ASM("asm/nonmatchings/main/80049FF0", func_8004A1A0);

INCLUDE_ASM("asm/nonmatchings/main/80049FF0", func_8004A414);

INCLUDE_ASM("asm/nonmatchings/main/80049FF0", func_8004A814);

void func_8004AC18(s32 arg0) {
    s32 level;
    s32 color;

    level = arg0;
    if (level < 0x100) {
        color = level * 0x10101;
    } else {
        color = 0xFFFFFF;
    }
    func_80049A40(-0xA0, -0x78, 0xA0, 0xF0, 1, color, 0x81);
    func_80049A40(0, -0x78, 0xA0, 0xF0, 1, color, 0x81);
    dtd_on_tpage(0, 0, 1, 1, 1);
}

void func_8004ACC8(s32 arg0) {
    func_80049A40(-0x100, -0x78, 0x100, 0xF0, 1, arg0, 0x81);
    func_80049A40(0, -0x78, 0x100, 0xF0, 1, arg0, 0x81);
    dtd_on_tpage(0, 0, 1, 1, 1);
}
