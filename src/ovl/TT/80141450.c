#include "common.h"
#include "ovl/TT.h"

/* Sets the UVs of a sprite POLY_FT4 (flip mode at +0x28, clamped to 0..255) and optionally scales (+0x2B) and
 * rotates (+0x2C) its corners with the parameters in q. T-11840 */
void func_80141450(u8 *arg0, TtSprXform *q) {
    s32 w;
    s32 h;
    s32 cu;
    s32 cv;
    s32 u;
    s32 v;
    s32 dx;
    s32 dy;
    s32 s;
    s32 c;
    s16 nx0;
    s16 nx1;
    s16 nx2;
    s16 nx3;
    POLY_FT4 *p;

    p = (POLY_FT4 *)arg0;
    /* One line: as1 then schedules the y1 load first, as in the original. T-11840 */
    w = p->x1; h = p->y1;
    if (q->sx >= 0x2000) {
        dx = -1;
    } else {
        dx = 0;
    }
    if (q->sy >= 0x2000) {
        dy = -1;
    } else {
        dy = 0;
    }
    switch (arg0[0x28]) {
    case 0:
        cu = p->u0;
        cv = p->v0;
        u = p->u0 + w + dx;
        v = p->v0 + h + dy;
        break;
    case 1:
        cu = p->u0;
        cv = p->v0 + h - 1;
        u = p->u0 + w + dx;
        v = p->v0 - dy - 1;
        break;
    case 2:
        cu = p->u0 + w - 1;
        cv = p->v0;
        u = p->u0 - dx - 1;
        v = p->v0 + h + dy;
        break;
    case 3:
        cu = p->u0 + w - 1;
        cv = p->v0 + h - 1;
        u = p->u0 - dx - 1;
        v = p->v0 - dy - 1;
        break;
    }
    if (u < 0) {
        u = 0;
        w--;
    } else if (u >= 0x100) {
        u = 0xFF;
        w--;
    }
    if (v < 0) {
        v = 0;
        h--;
    } else if (v >= 0x100) {
        v = 0xFF;
        h--;
    }
    /* FAKE: the corner stores and the scale go through the parameter, the rest through p; the two bases keep separate
     * uopt expression bits, which orders the x2/y2 ranges of the rotation like the original. T-11840 */
    switch (arg0[0x2B]) {
    case 0:
        ((POLY_FT4 *)arg0)->x1 = ((POLY_FT4 *)arg0)->x0 + w;
        ((POLY_FT4 *)arg0)->y2 = ((POLY_FT4 *)arg0)->y0 + h;
        ((POLY_FT4 *)arg0)->x3 = ((POLY_FT4 *)arg0)->x0 + w;
        ((POLY_FT4 *)arg0)->y3 = ((POLY_FT4 *)arg0)->y0 + h;
        ((POLY_FT4 *)arg0)->x2 = ((POLY_FT4 *)arg0)->x0;
        ((POLY_FT4 *)arg0)->y1 = ((POLY_FT4 *)arg0)->y0;
        break;
    case 1:
        ((POLY_FT4 *)arg0)->x1 = ((POLY_FT4 *)arg0)->x0 + w;
        ((POLY_FT4 *)arg0)->y2 = ((POLY_FT4 *)arg0)->y0 + h;
        ((POLY_FT4 *)arg0)->x3 = ((POLY_FT4 *)arg0)->x0 + w;
        ((POLY_FT4 *)arg0)->y3 = ((POLY_FT4 *)arg0)->y0 + h;
        ((POLY_FT4 *)arg0)->x2 = ((POLY_FT4 *)arg0)->x0;
        ((POLY_FT4 *)arg0)->y1 = ((POLY_FT4 *)arg0)->y0;
        ((POLY_FT4 *)arg0)->x0 = q->cx + (((((POLY_FT4 *)arg0)->x0 - q->cx) * q->sx) >> 12);
        ((POLY_FT4 *)arg0)->x1 = q->cx + (((((POLY_FT4 *)arg0)->x1 - q->cx) * q->sx) >> 12);
        ((POLY_FT4 *)arg0)->x2 = q->cx + (((((POLY_FT4 *)arg0)->x2 - q->cx) * q->sx) >> 12);
        ((POLY_FT4 *)arg0)->x3 = q->cx + (((((POLY_FT4 *)arg0)->x3 - q->cx) * q->sx) >> 12);
        ((POLY_FT4 *)arg0)->y0 = q->cy + (((((POLY_FT4 *)arg0)->y0 - q->cy) * q->sy) >> 12);
        ((POLY_FT4 *)arg0)->y1 = q->cy + (((((POLY_FT4 *)arg0)->y1 - q->cy) * q->sy) >> 12);
        ((POLY_FT4 *)arg0)->y2 = q->cy + (((((POLY_FT4 *)arg0)->y2 - q->cy) * q->sy) >> 12);
        ((POLY_FT4 *)arg0)->y3 = q->cy + (((((POLY_FT4 *)arg0)->y3 - q->cy) * q->sy) >> 12);
        break;
    }
    if (arg0[0x2C] != 0) {
        s = func_800A0140(q->angle);
        c = func_800A0070(q->angle);
        nx0 = q->rx + (((p->x0 - q->rx) * s - (p->y0 - q->ry) * c) >> 12);
        nx1 = q->rx + (((p->x1 - q->rx) * s - (p->y1 - q->ry) * c) >> 12);
        nx2 = q->rx + (((p->x2 - q->rx) * s - (p->y2 - q->ry) * c) >> 12);
        nx3 = q->rx + (((p->x3 - q->rx) * s - (p->y3 - q->ry) * c) >> 12);
        p->y0 = q->ry + (((p->x0 - q->rx) * c + (p->y0 - q->ry) * s) >> 12);
        p->y1 = q->ry + (((p->x1 - q->rx) * c + (p->y1 - q->ry) * s) >> 12);
        p->y2 = q->ry + (((p->x2 - q->rx) * c + (p->y2 - q->ry) * s) >> 12);
        p->y3 = q->ry + (((p->x3 - q->rx) * c + (p->y3 - q->ry) * s) >> 12);
        p->x0 = nx0;
        p->x1 = nx1;
        p->x2 = nx2;
        p->x3 = nx3;
        if (cu < u) {
            u--;
        } else {
            u++;
        }
        if (cv < v) {
            v--;
        } else {
            v++;
        }
    }
    p->u0 = cu; p->v0 = cv; p->u1 = u; p->v1 = cv; p->u2 = cu; p->v2 = v; p->u3 = u; p->v3 = v;
}

/* The two stores of the 0x2C packet share one line (as1 keeps the li order). */
void func_80141A1C(TtOtTag *arg0, u8 *arg1, s32 arg2) {
    s32 pad; /* FAKE: unused home keeps the 72-byte frame and the spill slot at sp+0x3C. T-11000 */
    TtDrawBuffer80B8 *buffer;
    DR_MODE *m;
    buffer = (TtDrawBuffer80B8 *)(D_80158AB4 + ((TtDrawSelect0C *)D_80158A68)->index * 0x80B8);
    if (arg1[0x2C] == 0 && arg1[0x2B] == 0 && arg1[0x28] == 0) {
        ((TtOtTag *)arg1)->len = 3;
        arg1[7] = 0x74;
        if (arg1[0x29] != 0) {
            arg1[7] |= 2;
        } else {
            arg1[7] &= ~2;
        }
        if (arg1[0x2A] != 0) {
            arg1[7] |= 1;
        } else {
            arg1[7] &= ~1;
        }
        ((TtOtTag *)arg1)->addr = arg0->addr;
        arg0->addr = (u32)arg1;
        m = buffer->cursor;
        buffer->cursor++;
        func_8009D294((s32)m, 0, 0, *(u16 *)(arg1 + 0x16), 0);
        ((TtOtTag *)m)->addr = arg0->addr;
        arg0->addr = (u32)m;
        return;
    }
    ((TtOtTag *)arg1)->len = 9; arg1[7] = 0x2C;
    if (arg1[0x29] != 0) {
        arg1[7] |= 2;
    } else {
        arg1[7] &= ~2;
    }
    if (arg1[0x2A] != 0) {
        arg1[7] |= 1;
    } else {
        arg1[7] &= ~1;
    }
    *(s16 *)(arg1 + 0x10) = 8;
    *(s16 *)(arg1 + 0x12) = 8;
    func_80141450(arg1, arg2);
    ((TtOtTag *)arg1)->addr = arg0->addr;
    arg0->addr = (u32)arg1;
}
void func_80141C6C(TtOtTag *arg0, u8 *arg1, s32 arg2) {
    s32 pad; /* FAKE: unused home keeps the 72-byte frame and the spill slot at sp+0x3C. T-11500 */
    TtDrawBuffer80B8 *buffer;
    DR_MODE *m;
    buffer = (TtDrawBuffer80B8 *)(D_80158AB4 + ((TtDrawSelect0C *)D_80158A68)->index * 0x80B8);
    if (arg1[0x2C] == 0 && arg1[0x2B] == 0 && arg1[0x28] == 0) {
        ((TtOtTag *)arg1)->len = 3;
        arg1[7] = 0x7C;
        if (arg1[0x29] != 0) {
            arg1[7] |= 2;
        } else {
            arg1[7] &= ~2;
        }
        if (arg1[0x2A] != 0) {
            arg1[7] |= 1;
        } else {
            arg1[7] &= ~1;
        }
        ((TtOtTag *)arg1)->addr = arg0->addr;
        arg0->addr = (u32)arg1;
        m = buffer->cursor;
        buffer->cursor++;
        func_8009D294((s32)m, 0, 0, *(u16 *)(arg1 + 0x16), 0);
        ((TtOtTag *)m)->addr = arg0->addr;
        arg0->addr = (u32)m;
        return;
    }
    ((TtOtTag *)arg1)->len = 9; arg1[7] = 0x2C;
    if (arg1[0x29] != 0) {
        arg1[7] |= 2;
    } else {
        arg1[7] &= ~2;
    }
    if (arg1[0x2A] != 0) {
        arg1[7] |= 1;
    } else {
        arg1[7] &= ~1;
    }
    *(s16 *)(arg1 + 0x10) = 0x10;
    *(s16 *)(arg1 + 0x12) = 0x10;
    func_80141450(arg1, arg2);
    ((TtOtTag *)arg1)->addr = arg0->addr;
    arg0->addr = (u32)arg1;
}

void func_80141EBC(TtOtTag *arg0, u8 *arg1, s32 arg2) {
    s32 pad; /* FAKE: unused home keeps the 72-byte frame and the spill slot at sp+0x3C. T-11500 */
    TtDrawBuffer80B8 *buffer;
    DR_MODE *m;
    buffer = (TtDrawBuffer80B8 *)(D_80158AB4 + ((TtDrawSelect0C *)D_80158A68)->index * 0x80B8);
    if (arg1[0x2C] == 0 && arg1[0x2B] == 0 && arg1[0x28] == 0) {
        ((TtOtTag *)arg1)->len = 4;
        arg1[7] = 0x64;
        if (arg1[0x29] != 0) {
            arg1[7] |= 2;
        } else {
            arg1[7] &= ~2;
        }
        if (arg1[0x2A] != 0) {
            arg1[7] |= 1;
        } else {
            arg1[7] &= ~1;
        }
        ((TtOtTag *)arg1)->addr = arg0->addr;
        arg0->addr = (u32)arg1;
        m = buffer->cursor;
        buffer->cursor++;
        func_8009D294((s32)m, 0, 0, *(u16 *)(arg1 + 0x16), 0);
        ((TtOtTag *)m)->addr = arg0->addr;
        arg0->addr = (u32)m;
        return;
    }
    ((TtOtTag *)arg1)->len = 9; arg1[7] = 0x2C;
    if (arg1[0x29] != 0) {
        arg1[7] |= 2;
    } else {
        arg1[7] &= ~2;
    }
    if (arg1[0x2A] != 0) {
        arg1[7] |= 1;
    } else {
        arg1[7] &= ~1;
    }
    func_80141450(arg1, arg2);
    ((TtOtTag *)arg1)->addr = arg0->addr;
    arg0->addr = (u32)arg1;
}
