#include "common.h"
#include "ovl/TT.h"

u8 *func_80147380(u8 *arg0, u8 *arg1) {
    while (arg0 < arg1) {
        if (*arg0 == 0) {
            return arg0;
        }
        arg0 += 0x68;
    }
    return 0;
}

void func_801473BC(void) {
    u8 *p;

    p = D_80158A94;
    do { /* FAKE: one-pass loop keeps p in $v0 (permuter); real source unknown. T-9320 */
        p[0] = 0;
    } while (0);
    p[0x68] = 0;
    p[0x138] = 0;
    p[0x1A0] = 0;
    p += 0x208;
    p[0] = 0;
    p[-0x138] = 0;
}

void func_801473E8(void) {
    u8 *p;

    p = D_80158A94;
    if (1) { /* FAKE: block, one-pass loop and empty test keep p in $v0 (permuter); real source unknown. T-9320 */
        *(s16 *)(p + 0x10) = 5;
        *(s16 *)(p + 0x78) = 5;
        *(s16 *)(p + 0x148) = 5;
        *(s16 *)(p + 0x1B0) = 5;
        *(s16 *)(p + 0x218) = 5;
        do {
            p[0] = 0;
        } while (0);
        p[0x68] = 0;
        p[0x138] = 0;
    }
    p[0x1A0] = 0;
    if (p) {
    }
    p += 0x208;
    p[0] = 0;
    p[-0x138] = 0;
    *(s16 *)(p - 0x128) = 5;
}

void func_8014742C(u8 *arg0, u16 arg1) {
    arg0[0] = 1;
    *(u16 *)(arg0 + 2) = arg1;
}

void func_8014743C(s16 arg0, u16 arg1, u8 arg2) {
    u8 *p;

    p = func_80147380(D_80158A94, D_80158A94 + 0x270);
    if (p != 0) {
        *(s32 *)(p + 0x20) = arg0 << 16;
        *(s32 *)(p + 0x24) = 0xFFF00000;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = (arg2 << 14) + 0x10000;
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        *(s16 *)(p + 0x52) = 0x1000;
        func_8014742C(p, arg1);
        if (arg1 == 0x30) {
            func_8013BF9C(p);
        }
        *(s16 *)(p + 4) = 0x40;
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        p[0xE] = 0;
        p[0xD] = 0;
    }
}

void func_8014750C(u8 *arg0) {
    *(s32 *)(arg0 + 0x20) += *(s32 *)(arg0 + 0x28);
    *(s32 *)(arg0 + 0x24) += *(s32 *)(arg0 + 0x2C);
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x22);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x26);
    if (*(s16 *)(arg0 + 0x16) >= 0x101) {
        arg0[0] = 0;
        *(s16 *)(arg0 + 4) = 0;
        arg0[0xC] = 0;
        *(s16 *)(arg0 + 0xA) = 0;
        *(s16 *)(arg0 + 8) = 0;
        *(s16 *)(arg0 + 6) = 0;
        arg0[0xF] = 0;
        arg0[0xE] = 0;
        arg0[0xD] = 0;
        return;
    }
    if (*(s16 *)(arg0 + 0x52) < 0xA01) {
        arg0[0x54] = 1;
    } else if (*(s16 *)(arg0 + 0x52) >= 0x1000) {
        *(s8 *)(arg0 + 0x54) = -1;
    }
    *(s16 *)(arg0 + 0x52) += *(s8 *)(arg0 + 0x54) * 0x30;
}

void func_801475C8(void) {
    TtSlot68 *p;
    u32 observed;
    u32 i;
    s32 count;
    u16 kind;

    i = 0;
    p = (TtSlot68 *)D_80158A94;
    count = 6;
    kind = 0x40;
    do {
        if (p->active != 0 && p->kind == kind) {
            func_8014750C((u8 *)p);
            /* FAKE: empty branch preserves the saved constant allocation (T-9780). */
            goto matched_call; matched_call: ;
        }
        i++;
        p++;
    } while (count != (observed = i)); /* FAKE: copy keeps the counter's compare order (T-9780). */
}

void func_80147644(void) {
    TtDrawBuffer80B8 *buffer;
    TtRectPacket16 *q;
    u8 *big;
    u8 *e;
    u8 *p;
    s32 i;

    e = D_80158AB0;
    p = D_80158A94;
    buffer = (TtDrawBuffer80B8 *)(D_80158AB4 + ((TtDrawSelect0C *)D_80158A68)->index * 0x80B8);
    for (i = 0; i != 6; i++, p += 0x68) {
        if (p[0] == 0) {
            continue;
        }
        if (*(s16 *)(p + 0x52) < 0xB00) {
            q = (TtRectPacket16 *)buffer->cursor;
            buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x10);
            q->tag.len = 3; q->code = 0x75;
            q->x = *(s16 *)(p + 0x14) - 4;
            q->y = *(s16 *)(p + 0x16) - 3;
            q->v = 0x50;
            q->u = 0x58;
            q->clut = 0x3C8B;
            q->tag.addr = buffer->ordering[5].addr;
            buffer->ordering[5].addr = (u32)q;
        } else if (*(s16 *)(p + 0x52) >= 0xF01) {
            q = (TtRectPacket16 *)buffer->cursor;
            buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x10);
            q->tag.len = 3; q->code = 0x75;
            q->x = *(s16 *)(p + 0x14) - 3;
            q->y = *(s16 *)(p + 0x16) - 3;
            q->v = 0x50;
            q->u = 0x50;
            q->clut = 0x3C8B;
            q->tag.addr = buffer->ordering[5].addr;
            buffer->ordering[5].addr = (u32)q;
        }
        big = (u8 *)buffer->cursor;
        buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x30);
        *(s16 *)(e + 4) = *(s16 *)(p + 0x14);
        *(s16 *)(e + 6) = *(s16 *)(p + 0x16);
        *(s16 *)e = *(s16 *)(e + 2) = *(s16 *)(p + 0x52);
        *(s16 *)(big + 8) = *(s16 *)(e + 4) - 0x1F;
        *(s16 *)(big + 10) = *(s16 *)(e + 6) - 0xF;
        *(s16 *)(big + 16) = 0x40;
        *(s16 *)(big + 18) = 0x20;
        /* FAKE: the two stores on one source line put the reload of the constant 1 ($v0) before the 0x60 temp, as in the original (line-based scheduling). T-11510 */
        big[13] = 0x60; big[12] = 0;
        big[40] = 0;
        big[41] = 0;
        big[42] = 1;
        *(s16 *)(big + 22) = 0xB;
        *(u16 *)(big + 14) = 0x3C8B;
        big[44] = 0;
        big[43] = 1;
        func_80141EBC((TtOtTag *)&buffer->ordering[5], big, e);
    }
}
