#include "common.h"
#include "ovl/TT.h"

void func_8013C6D0(void) {
    u8 *q = D_80158A6C;
    u8 *p = D_80158AA4;

    p[0] = 1;
    p[1] = 0;
    *(s16 *)(p + 0x44) = 0;
    *(s16 *)(p + 0x14) = -0x60;
    *(s16 *)(p + 0x16) = q[0xA] * 16 + 0x60;
    *(s16 *)(p + 0x3C) = 8;
    *(s16 *)(p + 0x3E) = 8;
    p[0x46] = 0;
    p[0x47] = 0x18;
    *(s16 *)(p + 0x40) = 0xA;
    *(s16 *)(p + 0x42) = 0x3C0F;
    p[0x3B] = 0x65;
    *(s16 *)(p + 0x10) = 0;
}

void func_8013C740(void) {
    *(s16 *)(D_80158AA4 + 0x16) = D_80158A6C[0xA] * 16 + 0x60;
}

void func_8013C764(u16 arg0, u16 arg1) {
    u8 *p = D_80158A60;
    u8 v = 0;

    if (arg0 != 0) {
        if (arg0 & 0x20) {
            v = 5;
        } else if (arg0 & 0x40) {
            v = 6;
        } else if (arg0 & 0x10) {
            v = 4;
        } else if (arg0 & 0x80) {
            v = 7;
        } else if (arg0 & 4) {
            v = 2;
        } else if (arg0 & 8) {
            v = 3;
        } else if (!(arg0 & 1) && (arg0 & 2)) {
            v = 1;
        }
        if (arg1 == 0) {
            p[0x44] = v;
        } else {
            p[0x45] = v;
        }
    }
}

u16 func_8013C818(u8 arg0, s8 *arg1) {
    u16 ret;

    switch (arg0) {
    case 5:
        ret = 0x1E;
        *arg1 = 0;
        break;
    case 6:
        ret = 0x1F;
        *arg1 = 0;
        break;
    case 4:
        ret = 0x20;
        *arg1 = 0;
        break;
    case 7:
        ret = 0x21;
        *arg1 = 0;
        break;
    case 2:
        ret = 0x22;
        *arg1 = 0xF;
        break;
    case 3:
        ret = 0x23;
        *arg1 = 0xF;
        break;
    case 0:
        ret = 0x24;
        *arg1 = 0xF;
        break;
    case 1:
        ret = 0x25;
        *arg1 = 0xF;
        break;
    }
    return ret;
}

void func_8013C8DC(void) {
    u8 *q;
    TtPoolDraw14 *p;
    s32 i;

    p = (TtPoolDraw14 *)D_80158AA8;
    q = D_80158A60;
    D_80158A6C[0xA] = 0;
    func_8013C6D0();
    p->unk_00 = 1;
    p->x = -0x48;
    p->y = 0x30;
    p->text = 0x12;
    p->unk_02 = 0xF;
    p->flags = 0;
    p->ot = 0;
    p++;
    for (i = 0; i < 3; i++, p++) {
        p->unk_00 = 1;
        p->x = -0x50;
        p->y = i * 16 + 0x60;
        p->text = D_80155D50[i];
        p->unk_02 = 0xF;
        p->flags = 0;
        p->ot = 0;
    }
    p = (TtPoolDraw14 *)D_80158AA8 + 16;
    p->unk_00 = 1;
    p->x = 0x30;
    p->y = 0x60;
    p->text = func_8013C818(q[0x44], (s8 *)&p->unk_02);
    p->flags = 0;
    p->ot = 0;
    p[1].unk_00 = 1;
    p[1].x = 0x30;
    p[1].y = 0x70;
    p++;
    p->text = func_8013C818(q[0x45], (s8 *)&p->unk_02);
    p->flags = 0;
    p->ot = 0;
}

void func_8013CA0C(void) {
    u8 *p;
    s32 pad[3]; /* FAKE: unused locals between p and r, real source unknown (frame layout). T-9130 */
    u8 *r;
    u8 *q;
    u8 *a;
    u8 *b;

    p = D_80158A60;
    a = D_80158A64;
    b = D_80158A6C;
    q = p + 0x10;
    if (*(u16 *)(p + 0x14) & 0x1000) {
        b[0xA] -= 1;
        if ((s8)b[0xA] < 0) {
            b[0xA] = 2;
        }
    } else if (*(u16 *)(q + 4) & 0x4000) {
        b[0xA] += 1;
        if (b[0xA] >= 3U) {
            b[0xA] = 0;
        }
    }
    if ((*(u16 *)(q + 4) & 0x820) && b[0xA] == 2) {
        *(u16 *)(a + 4) = 0;
        *(u16 *)(a + 2) = 0;
    }
    if (b[0xA] == 0 || b[0xA] == 1) {
        if (*(u16 *)(q + 4) & 0xFF) {
            func_8013C764(*(u16 *)(q + 4), b[0xA]);
        }
    }
    r = D_80158AA8 + 0x140;
    *(s16 *)(r + 0xE) = func_8013C818(p[0x44], D_80158AA8 + 0x142);
    r += 0x14;
    *(s16 *)(r + 0xE) = func_8013C818(p[0x45], r + 2);
    func_8013C740();
}

void func_8013CB58(void) {
    u8 *p = D_80158A64;

    switch (*(u16 *)(p + 4)) {
    case 0x0:
        func_8014CBD0();
        func_80148210();
        func_80143570();
        func_8014A7E0();
        func_80146BE0();
        func_80148DB0();
        func_80147CE0();
        func_8014AF3C();
        func_80132F4C();
        func_80133B70();
        func_8013BD4C();
        func_801473BC();
        func_801426F0();
        func_8013C8DC();
        *(u16 *)(p + 4) = 0x40;
        break;
    case 0x40:
        func_8013CA0C();
        break;
    }
}

void func_8013CC24(void) {
    u8 *q;
    s32 i;
    TtPoolDraw14 *p;

    q = D_80158A60;
    p = (TtPoolDraw14 *)D_80158AA8;
    D_80158A6C[0xA] = 0;
    func_8013C6D0();
    p->unk_00 = 1;
    p->x = -0x18;
    p->y = 0x30;
    p->text = 4;
    p->unk_02 = 0xF;
    p->flags = 0;
    p->ot = 0;
    p++;
    for (i = 0; i < 4; i++, p++) {
        p->unk_00 = 1;
        p->x = -0x50;
        p->y = i * 16 + 0x60;
        p->text = D_80155D50[i + 4];
        p->unk_02 = 0xF;
        p->flags = 0;
        p->ot = 0;
    }
    p = (TtPoolDraw14 *)D_80158AA8 + 16;
    p->unk_00 = 1;
    p->x = 0x20;
    p->y = 0x70;
    p->unk_02 = 0xF;
    p->flags = 0;
    p->ot = 0;
    if (q[0x43] != 0) {
        p->text = 0x18;
    } else {
        p->text = 0x19;
    }
    p[1].unk_00 = 1;
    p[1].x = 0x20;
    p[1].y = 0x80;
    p[1].unk_02 = 0xF;
    p[1].flags = 0;
    p[1].ot = 0;
    p++;
    if (q[0x49] != 0) {
        p->text = 0x16;
    } else {
        p->text = 0x15;
    }
}

void func_8013CDC0(void) {
    s32 pad; /* FAKE: unused home; the spill slots of t, s, g sit at sp+0x48/0x44/0x40 only with it. T-11500 */
    u8 *t;
    u8 *s;
    u8 *g;
    u8 *a;

    a = D_80158A60;
    t = D_80158A64;
    s = D_80158A6C;
    g = a + 0x10;
    if (*(u16 *)(a + 0x14) & 0x1000) {
        s[10] -= 1;
        if ((s8)s[10] < 0) {
            s[10] = 3;
        }
    } else if (*(u16 *)(g + 4) & 0x4000) {
        s[10] += 1;
        if (s[10] >= 4) {
            s[10] = 0;
        }
    }
    if (*(u16 *)(g + 4) & 0x8000) {
        switch (s[10]) {
        case 1:
            a[0x43] = 0;
            *(s16 *)(D_80158AA8 + 0x14E) = 0x19;
            break;
        case 2:
            a[0x49] = 0;
            *(s16 *)(D_80158AA8 + 0x162) = 0x15;
            {
                TtFrame80B8 *p1;

                p1 = (TtFrame80B8 *)D_80158AB4;
                func_8009F638(&p1[0].disp_rect, 0x140, 0, 0x140, 0xF0);
                func_8009F638(&p1[1].disp_rect, 0, 0, 0x140, 0xF0);
            }
            break;
        }
    } else if (*(u16 *)(g + 4) & 0x2000) {
        switch (s[10]) {
        case 1:
            a[0x43] = 1;
            *(s16 *)(D_80158AA8 + 0x14E) = 0x18;
            break;
        case 2:
            a[0x49] = 1;
            *(s16 *)(D_80158AA8 + 0x162) = 0x16;
            {
                TtFrame80B8 *p2;

                p2 = (TtFrame80B8 *)D_80158AB4;
                func_8009F638(&p2[0].disp_rect, 0x160, 0, 0x100, 0xF0);
                func_8009F638(&p2[1].disp_rect, 0x20, 0, 0x100, 0xF0);
            }
            break;
        }
    }
    if (*(u16 *)(g + 4) & 0x820) {
        switch (s[10]) {
        case 0:
            *(s16 *)(t + 2) = 0x50;
            break;
        case 3:
            *(s16 *)(t + 6) = 0;
            *(s16 *)(t + 4) = 0;
            *(s16 *)(t + 2) = 0;
            t[11] = 0;
            t[10] = 0;
            t[9] = 0;
            *(u16 *)t = 1;
            break;
        }
    }
    func_8013C740();
}

void func_8013D060(void) {
    u8 *p = D_80158A64;

    switch (*(u16 *)(p + 2)) {
    case 0x0:
        func_8014CBD0();
        func_80148210();
        func_80143570();
        func_8014A7E0();
        func_80146BE0();
        func_80148DB0();
        func_80147CE0();
        func_8014AF3C();
        func_80132F4C();
        func_80133B70();
        func_8013BD4C();
        func_801473BC();
        func_801426F0();
        p[0x11] = 0;
        p[0x1C] = 0;
        p[0x13] = 0;
        p[0x14] = 0;
        p[0x15] = 0;
        func_8013CC24();
        *(u16 *)(p + 2) = 0x40;
        break;
    case 0x40:
        func_8013CDC0();
        break;
    case 0x50:
        func_8013CB58();
        break;
    }
}

void func_8013D150(void) {
    u8 *p;
    s32 *q;
    s32 *r;
    u8 *t;
    u8 mode;
    u16 w;
    s16 h;
    s32 cur;
    s32 diff;

    p = D_80158A64;
    q = (s32 *)D_80158A6C;
    r = D_80158A70;
    t = D_80158A74;
    mode = p[0x1C];
    if (mode == 0x10) {
        *(u16 *)(p + 0x32) = *(u16 *)(p + 0x32) + 1;
        w = *(u16 *)(p + 0x34);
        if (w < 0x101) {
            *(u16 *)(p + 0x34) = 0x100;
        } else {
            *(u16 *)(p + 0x34) = w - 0x20;
        }
    } else if (mode == 0x11) {
        *(u16 *)(p + 0x32) = *(u16 *)(p + 0x32) + 1;
        w = *(u16 *)(p + 0x34);
        if (w >= 0x1000) {
            *(u16 *)(p + 0x34) = 0x1000;
            p[0x1C] = 2;
        } else {
            *(u16 *)(p + 0x34) = w + 0x40;
        }
    }
    if (*(u16 *)((u8 *)r + 2) >= 0x401) {
        q[1] = 0;
    } else {
        h = *(s16 *)(t + 0x16);
        if (h < 0x50) {
            q[1] = 0x30000;
        } else {
            q[1] = 0x28000 - (((h - 0x50) / 32) << 15);
        }
        if (q[1] < 0x8000) {
            q[1] = 0x8000;
        }
        if (q[1] > 0x30000) {
            q[1] = 0x30000;
        }
    }
    if (q[1] < q[0]) {
        q[0] -= 0x400;
    } else {
        q[0] += 0x200;
    }
    diff = q[0] - q[1];
    cur = (diff < 0) ? -diff : diff;
    if (cur < 0x1000) {
        q[0] = q[1];
    }
}

/* NON_MATCHING: T-11630, 20 differing lines, registers only: the original leaves a0 unused (v in a1, base in a2, tag in a3) */
#ifdef NON_MATCHING
void func_8013D2E0(void) {
    u8 *c;
    TtDrawBuffer80B8 *p;
    TILE *v;

    c = D_80158A64;
    p = ((TtDrawSelect0C *)D_80158A68)->index + (TtDrawBuffer80B8 *)D_80158AB4;
    v = (TILE *)p->cursor;
    p->cursor = (DR_MODE *)(v + 1);
    ((TtOtTag *)v)->len = 3, v->code = 0x61;
    v->r0 = c[0x13], v->g0 = c[0x14], v->b0 = c[0x15];
    v->x0 = -0x80, v->y0 = 8;
    v->w = 0x100, v->h = 0xE8;
    ((TtOtTag *)v)->addr = *(u32 *)&p->ordering[15];
    (&p->ordering[15])->addr = (u32)v;
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013D2E0);
#endif

void func_8013D3C0(void) {
    u8 *c;
    TtDrawBuffer80B8 *p;
    TILE *v;
    u32 r;
    u32 g;
    u32 b;

    c = D_80158A64;
    p = ((TtDrawSelect0C *)D_80158A68)->index + (TtDrawBuffer80B8 *)D_80158AB4;
    v = (TILE *)p->cursor;
    p->cursor = (DR_MODE *)(v + 1);
    /* comma expressions as in the libgpu setTile/setWH macros (one source line for as1) */
    ((TtOtTag *)v)->len = 3, v->code = 0x61;
    /* FAKE: dead store; it numbers g before r, so uopt colours g first (original: r in a2, g in a1). T-11630 */
    g = 0;
    r = (c[0x13] * c[0x1D]) >> 7;
    if ((s32)r >= 0x100) {
        r = 0xFF;
    }
    g = (c[0x14] * c[0x1E]) >> 7;
    if ((s32)g >= 0x100) {
        g = 0xFF;
    }
    b = (c[0x15] * c[0x1F]) >> 7;
    if ((s32)b >= 0x100) {
        b = 0xFF;
    }
    v->r0 = r;
    v->g0 = g;
    v->b0 = b;
    v->x0 = -0x80;
    v->y0 = 8;
    v->w = 0x100, v->h = 0xE8;
    ((TtOtTag *)v)->addr = *(u32 *)&p->ordering[15];
    (&p->ordering[15])->addr = (u32)v;
}

/* Tile map of D_80158A64 + 0x18, used only by the draw functions below: index (u16 per cell), 3-byte records, flag bytes. */
typedef struct TtTileMap {
    /* 0x00 */ u8 *index;
    /* 0x04 */ u8 *record;
    /* 0x08 */ u8 *flags;
} TtTileMap;

void func_8013D508(void) {
    TtDrawBuffer80B8 *buffer;
    TtRectPacket16 *q;
    DR_MODE *m;
    u8 *c;
    u8 *r;
    s32 row;
    s32 col;
    s16 scroll;
    TtTileMap *tm;
    TtCmdStream *st;

    buffer = (TtDrawBuffer80B8 *)(D_80158AB4 + ((TtDrawSelect0C *)D_80158A68)->index * 0x80B8);
    st = (TtCmdStream *)D_80158A70;
    scroll = 0x300 - st->time;
    c = D_80158A64;
    for (row = 0; row != 0x100; row += 0x10) {
        for (col = 0; col != 0x10; col++) {
            q = (TtRectPacket16 *)buffer->cursor;
            buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x10);
            q->code = 0x7D;
            q->tag.len = 3;
            q->x = (col << 4) - 0x80;
            q->y = row - (scroll & 0xF);
            tm = *(TtTileMap **)(c + 0x18);
            r = tm->record + tm->index[(row + col + (scroll & ~0xF)) & 0x3FF] * 3;
            q->u = (r[1] & 7) << 4;
            q->v = (r[0] << 7) + ((r[1] >> 3) << 4);
            q->clut = (((r[2] >> 4) + 0xF0) << 6) + (r[2] & 0xF);
            q->tag.addr = buffer->ordering[13].addr;
            buffer->ordering[13].addr = (u32)q;
        }
    }
    m = buffer->cursor;
    buffer->cursor++;
    func_8009D294((s32)m, 0, 0, 0xE, 0);
    ((TtOtTag *)m)->addr = buffer->ordering[13].addr;
    buffer->ordering[13].addr = (u32)m;
}

void func_8013D73C(void) {
    TtDrawBuffer80B8 *buffer;
    TtRectPacket16 *q;
    DR_MODE *m;
    u8 *c;
    u8 *r;
    s32 row;
    s32 col;
    s16 scroll;
    TtTileMap *tm;
    TtCmdStream *st;

    buffer = (TtDrawBuffer80B8 *)(D_80158AB4 + ((TtDrawSelect0C *)D_80158A68)->index * 0x80B8);
    st = (TtCmdStream *)D_80158A70;
    scroll = 0x300 - st->time;
    c = D_80158A64;
    for (row = 0; row != 0x100; row += 0x10) {
        for (col = 0; col != 0x10; col++) {
            q = (TtRectPacket16 *)buffer->cursor;
            buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x10);
            q->code = 0x7C;
            q->tag.len = 3;
            q->r = c[0x1D];
            q->g = c[0x1E];
            q->b = c[0x1F];
            q->y = row - (scroll & 0xF);
            q->x = (col << 4) - 0x80;
            tm = *(TtTileMap **)(c + 0x18);
            r = tm->record + tm->index[(row + col + (scroll & ~0xF)) & 0x3FF] * 3;
            q->u = (r[1] & 7) << 4;
            q->v = (r[0] << 7) + ((r[1] >> 3) << 4);
            q->clut = (((r[2] >> 4) + 0xF0) << 6) + (r[2] & 0xF);
            q->tag.addr = buffer->ordering[13].addr;
            buffer->ordering[13].addr = (u32)q;
        }
    }
    m = buffer->cursor;
    buffer->cursor++;
    func_8009D294((s32)m, 0, 0, 0xE, 0);
    ((TtOtTag *)m)->addr = buffer->ordering[13].addr;
    buffer->ordering[13].addr = (u32)m;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013D988);

void func_8013DE10(void) {
    s32 pad1; /* FAKE: unused, with pad2 reproduces the 0x78 frame. T-11510 */
    s32 pad2; /* FAKE: see pad1. T-11510 */
    TtDrawBuffer80B8 *buffer;
    TtRectPacket16 *q;
    POLY_G4 *g;
    DR_MODE *m;
    u8 *c;
    u8 *r;
    s32 row;
    s32 col;

    c = D_80158A64;
    buffer = (TtDrawBuffer80B8 *)(D_80158AB4 + ((TtDrawSelect0C *)D_80158A68)->index * 0x80B8);
    for (row = 0; row != 0x80; row += 0x10) {
        for (col = 0; col != 0x10; col++) {
            q = (TtRectPacket16 *)buffer->cursor;
            buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x10);
            q->code = 0x7D;
            q->tag.len = 3;
            q->x = (col << 4) - 0x80;
            q->y = row + 0x70;
            r = ((TtTileMap *)*(u8 **)(c + 0x18))->record + ((TtTileMap *)*(u8 **)(c + 0x18))->index[(row + col) & 0x3FF] * 3;
            /* Operand order of both sums and of the flags address sets the ugen temps (t6/t7); found with
               tools/order_search.py. T-11540 */
            q->u = ((r[1] & 7) << 4) + (*(r[0] + ((TtTileMap *)*(u8 **)(c + 0x18))->flags) & 0x80);
            q->v = ((r[1] >> 3) << 4) + ((*(r[0] + ((TtTileMap *)*(u8 **)(c + 0x18))->flags) & 0x40) * 2);
            q->clut = (((r[2] >> 4) + 0xF0) << 6) + (r[2] & 0xF);
            q->tag.addr = buffer->ordering[13].addr;
            buffer->ordering[13].addr = (u32)q;
        }
    }
    m = buffer->cursor;
    buffer->cursor++;
    func_8009D294((s32)m, 0, 0, 0x18, 0);
    ((TtOtTag *)m)->addr = buffer->ordering[13].addr;
    buffer->ordering[13].addr = (u32)m;
    m = buffer->cursor;
    buffer->cursor++;
    func_8009D294((s32)m, 0, 0, 0x18, 0);
    ((TtOtTag *)m)->addr = buffer->ordering[15].addr;
    buffer->ordering[15].addr = (u32)m;
    g = (POLY_G4 *)buffer->cursor;
    buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x24);
    ((TtOtTag *)g)->len = 8; g->code = 0x39;
    g->x0 = -0x80; g->y0 = 0; g->x1 = 0x80; g->y1 = 0; g->x2 = -0x80; g->y2 = 0xA8; g->x3 = 0x80; g->y3 = 0xA8;
    g->r0 = 0; g->g0 = 0x68; g->b0 = 0xF0;
    g->r1 = 0; g->g1 = 0x68; g->b1 = 0xF0;
    g->r2 = 0xB0; g->g2 = 0xF0; g->b2 = 0xD8;
    g->r3 = 0xB0; g->g3 = 0xF0; g->b3 = 0xD8;
    ((TtOtTag *)g)->addr = buffer->ordering[15].addr;
    buffer->ordering[15].addr = (u32)g;
    m = buffer->cursor;
    buffer->cursor++;
    func_8009D294((s32)m, 0, 1, 0x18, 0);
    ((TtOtTag *)m)->addr = buffer->ordering[15].addr;
    buffer->ordering[15].addr = (u32)m;
}

void func_8013E1E8(void) {
    switch (D_80158A64[0x1C]) {
    case 0:
        func_8013D2E0();
        return;
    case 1:
        func_8013D3C0();
        return;
    case 2:
        func_8013D508();
        return;
    case 3:
        func_8013D73C();
        return;
    case 0x10:
    case 0x11:
        func_8013D988();
        return;
    case 0x20:
    case 0x21:
        func_8013DE10();
        /* fallthrough */
    default:
        return;
    }
}
