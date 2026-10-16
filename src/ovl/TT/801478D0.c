#include "common.h"
#include "ovl/TT.h"

void func_801478D0(TtAnimState *arg0) {
    TtAnimFrame *frame;
    TtAnimFrame ***temp_v0_2;
    u8 temp_a1;
    u8 temp_a2;
    u8 temp_t1;
    u8 temp_t9;
    u8 temp_v0;

    temp_v0 = arg0->unk_08;
    if (temp_v0 & 0x80) {
        arg0->unk_08 = (u8) (temp_v0 & 0xFF7F);
        arg0->unk_06 = 0U;
        frame = **((TtAnimFrame ***)D_80151A50 + arg0->unk_05);
        arg0->unk_09 = frame->unk2;
        return;
    }
    if (temp_v0 & 0x20) {
        arg0->unk_08 = (u8) (temp_v0 & 0xFFDF);
        frame = **((TtAnimFrame ***)D_80151A50 + arg0->unk_05) + arg0->unk_06;
        arg0->unk_09 = frame->unk2;
        return;
    }
    if (!(temp_v0 & 0x40) && (--arg0->unk_09 == 0)) {
        temp_v0_2 = arg0->unk_05 + (TtAnimFrame ***)D_80151A50;
        temp_a1 = arg0->unk_06;
        frame = **temp_v0_2 + temp_a1;
        temp_a2 = frame->unk3;
        temp_t1 = temp_a1 + 1;
        if (temp_a2 == 0) {
            arg0->unk_06 = temp_t1;
            frame = **temp_v0_2 + (temp_t1 & 0xFF);
            arg0->unk_09 = frame->unk2;
            return;
        }
        temp_t9 = temp_a1 - temp_a2;
        if (temp_a2 == 0xFF) {
            arg0->unk_08 = (u8) (arg0->unk_08 | 1);
            return;
        }
        arg0->unk_06 = temp_t9;
        frame = **temp_v0_2 + (temp_t9 & 0xFF);
        arg0->unk_09 = frame->unk2;
    }
}

/* NON_MATCHING: T-11850, 58 funcdiff lines (29 instructions), registers only: the ugen temporaries t6 and t7 are swapped
 * from the arg1[6] load on (the original frees the K&R conversion temporary of arg2 after the D_80158A68 index load, IDO
 * at entry); every uopt register, the schedule and the frame match. Draft: K&R u16 arg2, flags and tile code share one
 * local (v1), idx = tile >> 6 in its own local (a2). */
#ifdef NON_MATCHING
void func_80147A18(arg0, arg1, arg2)
    u8 *arg0;
    u8 *arg1;
    u16 arg2;
{
    s32 pad[2]; /* FAKE: unused, sizes the 0x78 frame with the call spill at sp+0x50. T-11850 */
    TtDrawBuffer80B8 *buf;
    u8 *q;
    TtSprSet **sets;
    TtSprSet *set;
    TtSprFrame *fr;
    TtSprSeq *seq;
    TtSprPart *part;
    u8 *pal;
    u16 clut;
    s32 n;
    u8 *v;
    u8 *c;
    s32 code;
    s32 idx;
    s32 x;
    s32 y;
    u8 b;

    buf = (TtDrawBuffer80B8 *)(D_80158AB4 + ((TtDrawSelect0C *)D_80158A68)->index * 0x80B8);
    q = D_80158AB0;
    sets = *(TtSprSet ***)arg1;
    if (sets != 0) {
        set = sets[arg1[5]];
        fr = &set->frames[arg1[6]];
        seq = &set->seqs[fr->seq];
        part = set->parts + seq->start + seq->count - 1;
        pal = set->attr;
        clut = set->clut;
        *(s16 *)(q + 4) = *(s16 *)(arg0 + 0);
        *(s16 *)(q + 6) = *(s16 *)(arg0 + 2);
        if (arg1[0x10] != 0) {
            *(s16 *)(q + 0) = *(s16 *)(arg1 + 0xC);
            *(s16 *)(q + 2) = *(s16 *)(arg1 + 0xE);
        }
        for (n = seq->count; n > 0; n--, part--) {
            v = (u8 *)buf->cursor;
            buf->cursor = (DR_MODE *)(v + 0x30);
            v[0x28] = part->flags ^ arg1[4];
            code = arg1[4];
            if (code & 1) {
                y = *(s16 *)(arg0 + 2) - part->y - 0x10;
            } else {
                y = part->y + *(s16 *)(arg0 + 2);
            }
            if (code & 2) {
                x = *(s16 *)(arg0 + 0) - part->x - 0x10;
            } else {
                x = part->x + *(s16 *)(arg0 + 0);
            }
            *(s16 *)(v + 8) = x;
            *(s16 *)(v + 0xA) = y;
            idx = part->tile >> 6;
            c = pal + idx;
            code = part->tile & 0x3F;
            v[0xC] = (*c & 0x80) + (code & 7) * 16;
            v[0xD] = (*c & 0x40) * 2 + (code >> 3) * 16;
            v[0x29] = 0;
            v[0x2A] = 1;
            *(s16 *)(v + 0x16) = *c & 0x1F;
            if (arg1[0xA] != 0) {
                b = arg1[0xB];
                *(s16 *)(v + 0xE) = clut + (b & 0xF) + (b & 0xF0) * 4;
            } else {
                b = part->clut;
                *(s16 *)(v + 0xE) = clut + (b & 0xF) + (b & 0xF0) * 4;
            }
            v[0x2C] = 0;
            v[0x2B] = arg1[0x10];
            func_80141C6C(&buf->ordering[arg2], v, q);
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801478D0", func_80147A18);
#endif

void func_80147CE0(void) {
    s32 i = 0;
    u8 *p = D_80158A78;

    do {
        i += 1;
        p += 0xA4;
        p[-0xA4] = 0;
    } while (i != 3);
}

void func_80147D08(void) {
    s32 i;
    u8 *p;
    u8 *q;
    int new_var; /* FAKE: the dead test of D_80158A78 reserves a0/a1 so the hoisted constants start at a2, as in the original; same trick as func_8014A814. T-10910 */

    for (i = 0, p = D_80158A78; i < 3; i++, p += 0xA4) {
        p[0] = 0;
        new_var = D_80158A78 && D_80158A78;
        *(s16 *)(p + 2) = 4;
        *(s16 *)(p + 0x10) = 2;
        *(u8 **)(p + 0x38) = D_80151A50;
        p[0x3C] = 0;
        p[0x3D] = 0;
        p[0x3E] = 0x37;
        p[0x40] = 0x60;
        p[0x48] = 0;
        *(s16 *)(p + 4) = 0x40;
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        if (new_var && D_80158A78) { /* FAKE: see above. T-10910 */
        }
        p[0xE] = 0;
        p[0xD] = 0;
    }
    for (i = 0, q = D_80158A7C + 0xC0; i < 6; i++, q += 0x60) {
        q[0] = 0;
        *(s16 *)(q + 2) = 0;
        *(s16 *)(q + 4) = 0;
        q[0xC] = 0;
        *(s16 *)(q + 0xA) = 0;
        *(s16 *)(q + 8) = 0;
        *(s16 *)(q + 6) = 0;
        q[0xF] = 0;
        q[0xE] = 0;
        q[0xD] = 0;
        *(s16 *)(q + 0x10) = 2;
        *(u8 **)(q + 0x38) = D_80151A50;
        q[0x3C] = 0;
        q[0x3D] = 0;
        q[0x3E] = 0;
        q[0x40] = 0x60;
    }
}

void func_80147F14(u8 *arg0) {
    s32 i;
    u8 *q;

    q = D_80158A74;
    if (arg0 == D_80158A78) {
        *(s16 *)(arg0 + 0x62) = *(s16 *)(q + 0x14);
        *(s16 *)(arg0 + 0x82) = *(s16 *)(q + 0x16);
    } else {
        *(s16 *)(arg0 + 0x62) = *(s16 *)(arg0 - 0x54);
        *(s16 *)(arg0 + 0x82) = *(s16 *)(arg0 - 0x34);
    }
    for (i = 1; i < 10; i++) {
        *(s16 *)(arg0 + 0x4E + i * 2) = *(s16 *)(arg0 + 0x50 + i * 2);
        *(s16 *)(arg0 + 0x6E + i * 2) = *(s16 *)(arg0 + 0x70 + i * 2);
    }
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x50);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x70);
}

/* Three-store chains (u16 6, 8, A; bytes D, E, F) keep their zero in v1/a1 like the original; two-store chains or s16
 * give $zero stores. The loop is unrolled by IDO (T-11850). */
void func_80147FD0(u8 *arg0) {
    s32 i;
    u8 *src;

    src = D_80158A74;
    arg0[0] = 1;
    *(s16 *)(arg0 + 4) = 0x40;
    /* FAKE: both chains on one line give the original store order (as1 schedules by source line). T-11850 */
    *(u16 *)(arg0 + 6) = *(u16 *)(arg0 + 8) = *(u16 *)(arg0 + 0xA) = 0; arg0[0xD] = arg0[0xE] = arg0[0xF] = 0;
    *(s16 *)(arg0 + 0x14) = *(s16 *)(src + 0x14);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(src + 0x16);
    for (i = 0; i < 10; i++) {
        ((s16 *)(arg0 + 0x50))[i] = *(s16 *)(src + 0x14);
        ((s16 *)(arg0 + 0x70))[i] = *(s16 *)(src + 0x16);
    }
}

void func_801480BC(void) {
    s32 i;
    u8 *p;
    u8 *a;
    u8 *b;

    p = D_80158A74;
    if (p[0x53] != 0) {
        a = D_80158A78;
        i = 0;
        b = D_80158A7C + 0xC0;
        for (; i < 3; i++) {
            func_80147F14(a);
            b[0x3E] = p[0x5A] + 0x38;
            a += 0xA4;
            b += 0xC0;
            b[-0x22] = p[0x5B] + 0x3D;
        }
    }
}

void func_80148154(void) {
    s32 i;
    u8 *pos;
    u8 *p;
    u8 *q;

    if (D_80158A74[0x53] != 0) {
        for (i = 0, p = D_80158A78, q = D_80158A7C + 0xC0; i != 3; i++, p += 0xA4, q += 0x60) {
            pos = p + 0x14;
            if (p[0] != 0) {
                func_80147A18(pos, p + 0x38, *(u16 *)(p + 0x10));
                func_80147A18(pos, q + 0x38, *(u16 *)(q + 0x10));
                q += 0x60;
                func_80147A18(pos, q + 0x38, *(u16 *)(q + 0x10));
            }
        }
    }
}
