#include "common.h"
#include "ovl/TT.h"

/* Font glyph: sprite packet plus draw mode. The comma lines follow the libgpu set* macros (one source line each for as1). T-11860 */
void func_801487D0(s16 x, s16 y, u8 ch) {
    TtDrawBuffer80B8 *buffer;
    TtSpritePacket20 *q;
    s32 pad1; /* FAKE: unused, frame slot above m (0x38). T-11860 */
    DR_MODE *m;
    s32 pad2; /* FAKE: unused, frame slot under m; moves the spill of the ordering-table address to 0x30. T-11860 */

    buffer = (TtDrawBuffer80B8 *)(D_80158AB4 + ((TtDrawSelect0C *)D_80158A68)->index * 0x80B8);
    if (ch != 0x20) {
        ch -= 0x20;
        q = (TtSpritePacket20 *)buffer->cursor;
        buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x14);
        q->tag.len = 4, q->code = 0x65;
        q->w = 6, q->h = 8;
        q->r = 0x80, q->g = 0x80, q->b = 0x80;
        /* FAKE: the (u8) cast costs the ugen temporary the original has between the shift and the multiply. T-11860 */
        q->u = (ch & 0xF) * 6, q->v = (u8)((ch >> 4) * 8) + 0x80;
        q->clut = 0x7FFC;
        q->x = x, q->y = y;
        q->tag.addr = buffer->ordering[0].addr;
        buffer->ordering[0].addr = (u32)q;
        m = buffer->cursor;
        buffer->cursor++;
        func_8009D294((s32)m, 0, 0, 0x1F, 0);
        ((TtOtTag *)m)->addr = buffer->ordering[0].addr;
        buffer->ordering[0].addr = (u32)m;
    }
}

/* The number printers below are old-style (K&R) definitions: the s16
 * parameters are converted in place at entry, a u8 parameter that is assigned stays in its
 * home slot (lbu/sb 0x54(sp)), and the locals declared before the buffer reserve the slots
 * above it (T-9210). */
void func_80148974(arg0, arg1, arg2)
    s16 arg0;
    s16 arg1;
    u8 *arg2;
{
    while (*arg2 != 0) {
        func_801487D0(arg0, arg1, *arg2++);
        arg0 += 6;
    }
}

void func_80148A04(arg0, arg1, arg2, arg3)
    s16 arg0;
    s16 arg1;
    s32 arg2;
    u8 arg3;
{
    u8 *p;
    s32 i;
    s32 m;
    u8 buf[12];

    m = 1;
    if (arg3 == 0 || arg3 >= 0xB) {
        arg3 = 0xA;
    }
    for (i = 1; i < arg3; i++) {
        m *= 10;
    }
    if (arg2 < 0) {
        buf[0] = '-';
        p = buf + 1;
        arg2 = -arg2;
    } else {
        buf[0] = '+';
        p = buf + 1;
    }
    arg2 %= m * 10;
    for (i = 0; i < arg3; i++) {
        *p++ = arg2 / m + '0';
        arg2 %= m;
        m /= 10;
    }
    *p = 0;
    func_80148974(arg0, arg1, buf);
}

void func_80148B6C(arg0, arg1, arg2, arg3)
    s16 arg0;
    s16 arg1;
    u32 arg2;
    u8 arg3;
{
    u8 *p;
    s32 i;
    u32 d;
    u8 buf[12];

    if (arg3 == 0 || arg3 >= 9) {
        arg3 = 8;
    }
    p = buf;
    for (i = arg3 - 1; i >= 0; i--) {
        d = (arg2 >> (i * 4)) & 0xF;
        if (d < 10) {
            *p = d + 0x30;
        } else {
            *p = d + 0x37;
        }
        p++;
    }
    *p = 0;
    func_80148974(arg0, arg1, buf);
}

void func_80148C04(arg0, arg1, arg2, arg3)
    s16 arg0;
    s16 arg1;
    u32 arg2;
    u8 arg3;
{
    u8 *p;
    s32 i;
    s32 b;
    u8 buf[36];

    p = buf;
    if (--arg3 == 0 || arg3 >= 0x20) {
        arg3 = 0x1F;
    }
    for (i = arg3; i >= 0; i--) {
        if ((arg2 >> i) & 1) {
            b = 1;
        } else {
            b = 0;
        }
        *p = b + 0x30;
        p++;
    }
    *p = 0;
    func_80148974(arg0, arg1, buf);
}

void func_80148C94(arg0, arg1, arg2, arg3)
    s16 arg0;
    s16 arg1;
    u32 arg2;
    u8 arg3;
{
    u8 *p;
    s32 i;
    s32 m;
    u8 buf[12];

    m = 1;
    p = buf;
    if (arg3 == 0 || arg3 >= 0xB) {
        arg3 = 0xA;
    }
    for (i = 1; i < arg3; i++) {
        m *= 10;
    }
    arg2 %= m * 10;
    for (i = 0; i < arg3; i++) {
        *p++ = arg2 / m + '0';
        arg2 %= m;
        m /= 10;
    }
    *p = 0;
    func_80148974(arg0, arg1, buf);
}
