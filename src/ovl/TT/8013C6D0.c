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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013C8DC);

void func_8013CA0C(void) {
    u8 *p;
    s32 pad[3]; /* FAKE: unused locals between p and r, real source unknown (frame layout) */
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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013CC24);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013CDC0);

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

/* NON_MATCHING: T-10710, 99 of 99 instructions: locals for the four pointer globals (hoisted loads) match; remaining diff is register/constant sharing: the draft hoists 0x30000/0x8000 into a0/a1 while the original builds them in t7/v0/a0 per use, and the first-branch constant stays separate */
#ifdef NON_MATCHING
void func_8013D150(void) {
    u8 *p;
    s32 *q;
    s32 *r;
    u8 *t;
    u8 mode;
    u16 w;
    s16 h;
    s32 target;
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
    target = 0;
    if (*(u16 *)((u8 *)r + 2) >= 0x401) {
        q[1] = 0;
    } else {
        h = *(s16 *)(t + 0x16);
        if (h < 0x50) {
            q[1] = 0x30000U;
        } else {
            q[1] = 0x28000 - (((h - 0x50) / 32) << 15);
        }
        target = q[1];
        if (target < 0x8000) {
            target = q[1] = 0x8000;
        }
        if (target > 0x30000) {
            q[1] = target = 0x30000;
        }
    }
    cur = q[0];
    if (target < cur) {
        q[0] = cur - 0x400;
    } else {
        q[0] = cur + 0x200;
    }
    target = q[1];
    diff = q[0] - target;
    cur = diff;
    if (diff < 0) {
        cur = -diff;
    }
    if (cur < 0x1000) {
        q[0] = target;
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013D150);
#endif

/* NON_MATCHING: T-10510, registers only (v in a1: a0 left unused by the original) */
#ifdef NON_MATCHING
void func_8013D2E0(void) {
    u8 *c;
    TtDrawBuffer80B8 *p;
    TILE *v;

    c = D_80158A64;
    p = ((TtDrawSelect0C *)D_80158A68)->index + (TtDrawBuffer80B8 *)D_80158AB4;
    v = (TILE *)p->cursor;
    p->cursor = (DR_MODE *)(v + 1);
    ((TtOtTag *)v)->len = 3;
    v->code = 0x61;
    v->r0 = c[0x13];
    v->g0 = c[0x14];
    v->b0 = c[0x15];
    v->x0 = -0x80;
    v->y0 = 8;
    v->w = 0x100;
    v->h = 0xE8;
    ((TtOtTag *)v)->addr = *(u32 *)&p->ordering[15];
    (&p->ordering[15])->addr = (u32)v;
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013D2E0);
#endif

/* NON_MATCHING: T-10850, 20 differing listing lines; regdiff evidence in the ticket scratch. */
#ifdef NON_MATCHING
/* FAKE: same-line stores preserve packet instruction scheduling. T-10850 */
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
    ((TtOtTag *)v)->len = 3; v->code = 0x61;
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
    v->w = 0x100; v->h = 0xE8;
    ((TtOtTag *)v)->addr = *(u32 *)&p->ordering[15];
    (&p->ordering[15])->addr = (u32)v;
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013D3C0);
#endif

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013D508);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013D73C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013D988);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013DE10);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013E1E8);
