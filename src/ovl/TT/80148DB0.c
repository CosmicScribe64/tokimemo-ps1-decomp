#include "common.h"
#include "ovl/TT.h"

void func_80148DB0(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = D_80158A88;
    for (; i < 8; i++) {
        *p = 0;
        p += 0x58;
    }
}

void func_80148DE4(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = D_80158A88; i < 8; i++, p += 0x58) {
        p[0] = 0;
        *(s16 *)(p + 0x10) = 0xB;
        *(s16 *)(p + 0x3E) = 0xA;
        *(s16 *)(p + 0x40) = 0x3C8B;
        p[0x42] = 0xF0;
        p[0x43] = 0xD0;
    }
}

void func_80148E7C(arg0, arg1)
u8 *arg0;
s32 arg1;
{
    u8 *src;
    s32 *pp;
    u8 *p;
    s32 i;
    s32 d;
    s32 dx;
    s32 e;

    pp = D_80158A70;
    src = D_80158A74;
    if (arg1 < 2) {
        *(s32 *)(arg0 + 0x20) = *(s16 *)(src + 0x14) << 16;
        *(s32 *)(arg0 + 0x1C) = *pp - ((*(s16 *)(src + 0x16) - 0x60) << 16);
        *(u16 *)(arg0 + 0x44) = 0xFFFF;
        i = 0;
        p = D_80158A9C;
        for (; i != 0x20; i++, p += 0x78) {
            if (p[0] == 0) continue;
            if (*(u16 *)(p + 4) != 0x40) continue;
            if (p[0x64] != 0) continue;
            d = *(s16 *)(src + 0x16) - *(s16 *)(p + 0x16);
            if (d < 0x60 && d >= 0x11) {
                e = *(s16 *)(p + 0x14) - *(s16 *)(src + 0x14);
                if (e < 0) {
                    dx = -e;
                } else {
                    dx = e;
                }
                if (dx < 0x20) {
                    *(s32 *)(arg0 + 0x20) = *(s32 *)(p + 0x20);
                    *(s32 *)(arg0 + 0x1C) = *(s32 *)(p + 0x1C);
                    *(s16 *)(arg0 + 0x44) = i;
                    p[0x64] = 1;
                    break;
                }
            }
        }
    } else {
        src = D_80158A78 + (arg1 >> 1) * 0xA4 - 0xA4;
        *(s32 *)(arg0 + 0x20) = *(s16 *)(src + 0x14) << 16;
        *(s32 *)(arg0 + 0x1C) = *pp - ((*(s16 *)(src + 0x16) - 0x60) << 16);
        *(u16 *)(arg0 + 0x44) = 0xFFFF;
        p = D_80158A9C;
        for (i = 0; i != 0x20; i++, p += 0x78) {
            if (p[0] != 0 && p[0x64] == 0) {
                d = *(s16 *)(src + 0x16) - *(s16 *)(p + 0x16);
                if (d < 0x60 && d >= 0x11) {
                    e = *(s16 *)(p + 0x14) - *(s16 *)(src + 0x14);
                    if (e < 0) {
                        dx = -e;
                    } else {
                        dx = e;
                    }
                    if (dx < 0x20) {
                        *(s32 *)(arg0 + 0x20) = *(s32 *)(p + 0x20);
                        *(s32 *)(arg0 + 0x1C) = *(s32 *)(p + 0x1C);
                        *(s16 *)(arg0 + 0x44) = i;
                        p[0x64] = 1;
                        break;
                    }
                }
            }
        }
    }
    *(s32 *)(arg0 + 0x28) = 0;
    *(s32 *)(arg0 + 0x2C) = 0;
    *(s16 *)(arg0 + 0x46) = 0;
}

void func_80149098(u8 *arg0) {
    if ((u32)(u16)++*(u16 *)(arg0 + 0x46) >= 0x79) {
        arg0[0] = 0;
    }
}

void func_801490C0(u8 *arg0) {
    *(s32 *)(arg0 + 0x24) = *D_80158A70 - *(s32 *)(arg0 + 0x1C);
    *(s32 *)(arg0 + 0x20) += *(s32 *)(arg0 + 0x28);
    *(s32 *)(arg0 + 0x24) += *(s32 *)(arg0 + 0x2C);
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x22);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x26);
}

void func_80149108(void) {
    s32 index;
    u16 state;
    TtState58 *record;

    record = (TtState58 *)D_80158A88;
    index = 0;
    do {
        if (record->unk_00 != 0) {
            state = record->unk_04;
            switch (state) {
            case 0x20:
                func_80148E7C((u8 *)record, index);
                record->unk_04 = 0x40U;
                record->unk_0A = 0;
                record->unk_08 = 0;
                record->unk_06 = 0;
                record->unk_0F = 0;
                record->unk_0E = 0;
                record->unk_0D = 0;
                break;
            case 0x40:
                func_80149098((u8 *)record);
                break;
            case 0x80:
                record->unk_00 = 0;
                break;
            }
            /* FAKE: the dead bound test gives 8 its original saved register;
             * no extra instructions are emitted. T-10020. */
            if (index != 8) {}
            func_801490C0((u8 *)record);
        }
        index += 1;
        record++;
    } while (index != 8);
}

void func_801491F0(void) {
    DR_MODE *m;
    TtDrawBuffer80B8 *buffer;
    TtSpritePacket20 *q;
    s32 i;
    u8 *p;

    p = D_80158A88;
    buffer = (TtDrawBuffer80B8 *)(D_80158AB4 + ((TtDrawSelect0C *)D_80158A68)->index * 0x80B8);
    for (i = 0; i != 8; i++) {
        if (p[0] != 0) {
            q = (TtSpritePacket20 *)buffer->cursor;
            buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x14);
            q->tag.len = 4; q->code = 0x65; /* one line (setlen/setcode macro style) */
            q->x = *(s16 *)(p + 0x14) - 8;
            q->y = *(s16 *)(p + 0x16) - 8;
            q->h = 0x10;
            q->w = 0x10;
            q->u = p[0x42];
            q->v = p[0x43];
            q->clut = *(u16 *)(p + 0x40);
            q->tag.addr = buffer->ordering[*(u16 *)(p + 0x10)].addr;
            buffer->ordering[*(u16 *)(p + 0x10)].addr = (u32)q;
            m = buffer->cursor;
            buffer->cursor++;
            func_8009D294((s32)m, 0, 0, *(u16 *)(p + 0x3E), 0);
            ((TtOtTag *)m)->addr = buffer->ordering[*(u16 *)(p + 0x10)].addr;
            buffer->ordering[*(u16 *)(p + 0x10)].addr = (u32)m;
        }
        if ((s32)p != 8) {} /* FAKE: a second use of the bound gives it a saved register ahead of i. T-11800 */
        p += 0x58;
    }
}

void func_80149400(void) {
    s32 pad; /* FAKE: moves the spill slot of p to sp+0x28. T-11800 */
    u8 *p;

    p = D_80158A64;
    if ((*(u16 *)(D_80158A60 + 0x14) & D_80156C90[p[0x21]]) == D_80156C90[p[0x21]]) {
        p[0x20] = 0x10;
        if (++p[0x21] == 6) {
            func_80133AD0(0x607);
            p[0x21] = 0;
            p[0x24] = 1;
            p[0x25] = 0;
            D_80158AA8[0x78] = 1;
        }
    } else if (*(u16 *)(*(s32 *)&D_80158A60 + 0x14) != 0) { /* FAKE: the s32 view stops uopt reusing the first read. T-11800 */
        p[0x21] = 0;
    }
    if (--p[0x20] == 0) {
        p[0x21] = 0;
    }
}

void func_801494D8(void) {
    u8 *scene;
    u8 *state;
    u16 pad1; /* FAKE: pad1..pad3 reproduce the original frame (c at sp+0x44). T-11860 */
    u16 pad2;
    s32 pad3;
    u8 *c;
    u8 *input;
    u8 *q;
    u8 *p;
    u32 max;

    scene = D_80158A60;
    input = scene + 0x10;
    state = D_80158A64;
    c = D_80158A6C;
    q = D_80158AA4;
    p = D_80158AA8;
    max = 3;
    if (scene[0x53] == 0) {
        max = 2;
    }
    p[c[0xA] * 0x14 + 2] = 0;
    if (*(u16 *)(input + 4) & 0x1000) {
        func_80133AD0(0x613);
        *(s16 *)(state + 0x26) = 0;
        if (--c[0xA] == 0xFF) {
            c[0xA] = max;
        }
    } else if (*(u16 *)(input + 4) & 0x4000) {
        func_80133AD0(0x613);
        *(s16 *)(state + 0x26) = 0;
        if (++c[0xA] > max) {
            c[0xA] = 0;
        }
    }
    p[c[0xA] * 0x14 + 2] = 2;
    q[0] = 1;
    q[1] = 0;
    *(s16 *)(q + 0x44) = 0;
    *(s16 *)(q + 0x14) = -0x2C;
    *(s16 *)(q + 0x16) = c[0xA] * 8 + 0xA0;
    *(u16 *)(q + 0x10) = 1;
    q[0x3B] = 0x63;
    *(s16 *)(q + 0x3C) = 0x58; *(s16 *)(q + 0x3E) = 8; /* FAKE: one line keeps the original li order (as1 schedules by line). T-11860 */
    q[0x38] = 0x80;
    q[0x39] = 0x40;
    q[0x3A] = 0x40;
    if ((*(u16 *)(input + 4) & 0x800) || (*(u16 *)(input + 4) & 0x20)) {
        func_80044750(0xB1);
        func_80133AD0(0x612);
        state[0x22] = 0;
        *(u16 *)(state + 2) += 1;
        *(s16 *)(state + 4) = 0;
        *(s16 *)(c + 0x10) = 0;
        *(s16 *)(c + 0x12) = 0;
        func_80149B20();
    }
    if (scene[0x53] == 1 && state[0x24] == 0) {
        func_80149400();
    }
}

void func_80149704(void) {
    u8 *scene;
    u8 *q;
    u8 *p;
    s32 i;

    scene = D_80158A60;
    q = D_80158AA4;
    p = D_80158AA8;
    func_8009C5E0(1);
    for (i = 0; i < 4; i++, p += 0x14) {
        *(s16 *)(p + 0xA) = D_80156C9C[i];
        *(s16 *)(p + 0xC) = i * 8 + 0xA0;
        *(u16 *)(p + 0xE) = D_80156CA4[i];
        p[2] = 0;
        p[0] = 1;
        *(s16 *)(p + 6) = 0;
        *(s16 *)(p + 0x10) = 0;
    }
    if (scene[0x53] == 0) {
        p[-0x14] = 0;
    }
    for (i = 0; i != 2; i++, p += 0x14) {
        *(s16 *)(p + 0xA) = D_80156CAC[i];
        *(s16 *)(p + 0xC) = i * 8 + 0xD0;
        *(u16 *)(p + 0xE) = D_80156CB0[i];
        p[2] = 0;
        p[0] = 1;
        *(s16 *)(p + 6) = 0;
        *(s16 *)(p + 0x10) = 0;
    }
    *(s16 *)(p + 0xA) = -0x70;
    *(s16 *)(p + 0xC) = 0x88;
    *(s16 *)(p + 0xE) = 0;
    p[2] = 3;
    p[0] = 0;
    *(s16 *)(p + 6) = 0;
    *(s16 *)(p + 0x10) = 0;
    q += 0x64;
    q[0] = 0;
    q[1] = 0;
    *(s16 *)(q + 0x44) = 0;
    *(s16 *)(q + 0x14) = -0x60;
    *(s16 *)(q + 0x16) = 0x38;
    *(s16 *)(q + 0x3C) = 0x80;
    *(s16 *)(q + 0x3E) = 0x30;
    q[0x46] = 0;
    q[0x47] = 0;
    *(s16 *)(q + 0x40) = 0xB;
    *(s16 *)(q + 0x42) = 0x3C20;
    if (q) {} /* FAKE: splits the uopt block so 0x65 can take v0. T-11820, still needed T-11850 */
    q[0x3B] = 0x65;
    *(s16 *)(q + 0x10) = 0;
    q += 0x64;
    q[0] = q[1] = 0;
    *(s16 *)(q + 0x44) = 0;
    *(s16 *)(q + 0x14) = 0x20;
    *(s16 *)(q + 0x16) = 0x38;
    *(s16 *)(q + 0x3C) = 0x40;
    *(s16 *)(q + 0x3E) = 0x30;
    q[0x46] = 0;
    q[0x47] = 0x30;
    *(s16 *)(q + 0x40) = 0xB;
    *(s16 *)(q + 0x42) = 0x3C20;
    q[0x3B] = 0x65;
    *(s16 *)(q + 0x10) = 0;
}

void func_801498C8(void) {
    u8 *q;
    u8 *c;
    u8 *o;
    s32 i;

    c = D_80158A6C;
    q = D_80158AB0 + 0x10;
    *(s16 *)(q + 4) = -0x100;
    *(s16 *)(q + 6) = 0x44;
    *(s16 *)q = 0x1000;
    *(s16 *)(q + 2) = 0x1000;
    *(s16 *)(q + 0x14) = 0x100;
    *(s16 *)(q + 0x16) = 0x44;
    *(s16 *)(q + 0x10) = 0x1000;
    *(s16 *)(q + 0x12) = 0x1000;
    o = D_80158AA4 + 0xFA0;
    for (i = 0; i < 4; i++, o += 0x64) {
        o[0] = 1;
        o[1] = 0;
        *(s16 *)(o + 0x44) = 0;
        o[0x3B] = 0x80;
        o[0x4D] = 0;
        o[0x4E] = 1;
        o[0x4F] = 1;
        o[0x50] = 0;
        *(s16 *)(o + 0x40) = 0x19;
        *(s16 *)(o + 0x42) = 0x3C07;
        o[0x4C] = 0;
        *(s16 *)(o + 0x10) = 0;
    }
    o = D_80158AA4 + 0xFA0;
    o[0x46] = 0;
    o[0x47] = 0x80;
    *(s16 *)(o + 0x3C) = 0x50;
    *(s16 *)(o + 0x3E) = 0x20;
    *(s32 *)(o + 0x20) = (*(s16 *)(q + 0x14) - 0x6C) << 16;
    *(s32 *)(o + 0x24) = 0x500000;
    *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
    *(s16 *)(o + 0x16) = *(s16 *)(o + 0x26);
    *(s32 *)(o + 0x28) = 0;
    *(s32 *)(o + 0x2C) = 0;
    *(u8 **)(o + 0x48) = q + 0x10;
    o += 0x64;
    o[0x47] = 0xA0;
    o[0x46] = 0;
    *(s16 *)(o + 0x3C) = 0x80;
    *(s16 *)(o + 0x3E) = 0x20;
    *(s32 *)(o + 0x20) = (*(s16 *)(q + 0x14) - 0x18) << 16;
    *(s32 *)(o + 0x24) = 0x500000;
    *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
    *(s16 *)(o + 0x16) = *(s16 *)(o + 0x26);
    *(s32 *)(o + 0x28) = 0;
    *(s32 *)(o + 0x2C) = 0;
    *(u8 **)(o + 0x48) = q + 0x10;
    o += 0x64;
    o[0x47] = 0;
    o[0x46] = 0;
    *(s16 *)(o + 0x3C) = 0x80;
    *(s16 *)(o + 0x3E) = 0x40;
    *(s32 *)(o + 0x20) = (*(s16 *)(q + 4) - 0x60) << 16;
    *(s32 *)(o + 0x24) = 0x180000;
    *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
    *(s16 *)(o + 0x16) = *(s16 *)(o + 0x26);
    *(s32 *)(o + 0x28) = 0;
    *(s32 *)(o + 0x2C) = 0;
    *(u8 **)(o + 0x48) = q;
    o += 0x64;
    o[0x47] = 0x40;
    o[0x46] = 0;
    *(s16 *)(o + 0x3C) = 0x40;
    *(s16 *)(o + 0x3E) = 0x40;
    *(s32 *)(o + 0x20) = (*(s16 *)(q + 4) + 0x20) << 16;
    *(s32 *)(o + 0x24) = 0x180000;
    *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
    *(s16 *)(o + 0x16) = *(s16 *)(o + 0x26);
    *(s32 *)(o + 0x28) = 0;
    *(s32 *)(o + 0x2C) = 0;
    *(u8 **)(o + 0x48) = q;
    *(s16 *)(c + 0x12) = 0;
}

void func_80149B20(void) {
    u8 *q;
    u8 *o;

    q = D_80158AB0 + 0x10;
    *(s16 *)(q + 4) = 0;
    *(s16 *)q = *(s16 *)(q + 2) = 0x1000;
    *(s16 *)(q + 0x14) = 0;
    *(s16 *)(q + 0x10) = *(s16 *)(q + 0x12) = 0x1000;
    o = D_80158AA4 + 0xFA0;
    *(s32 *)(o + 0x20) = 0xFF940000;
    o[0x4F] = 0;
    *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
    o += 0x64;
    *(s32 *)(o + 0x20) = 0xFFE80000;
    o[0x4F] = 0;
    *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
    o += 0x64;
    *(s32 *)(o + 0x20) = 0xFFA00000;
    o[0x4F] = 0;
    *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
    o += 0x64;
    *(s32 *)(o + 0x20) = 0x200000;
    o[0x4F] = 0;
    *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
}

/* T-11800: matched by walking the four 0x64-byte records (D_80158AA4 + 0xFA0, then o += 0x64 three times); uopt folds the walk into the original D_80158AA4 + 0x10CC rebase. */
void func_80149BB0(void) {
    u8 *p;
    u8 *q;
    u8 *c;
    u8 *o;
    s32 ang;

    p = D_80158A64;
    q = D_80158AB0 + 0x10;
    c = D_80158A6C;
    switch (*(u16 *)(p + 4)) {
    case 0x40:
        ang = *(u16 *)(c + 0x12);
        *(s16 *)(q + 4) = -0x80 - func_800A0140(ang) / 32;
        *(s16 *)q = *(s16 *)(q + 2) = 0x1000 - func_800A0070(ang) / 2;
        *(s16 *)(q + 0x14) = func_800A0140(ang) / 32 + 0x80;
        *(s16 *)(q + 0x10) = *(s16 *)(q + 0x12) = func_800A0070(ang) / 2 + 0x1000;
        o = D_80158AA4 + 0xFA0;
        *(s32 *)(o + 0x20) = (*(s16 *)(q + 0x14) - 0x6C) << 16;
        *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
        o += 0x64;
        *(s32 *)(o + 0x20) = (*(s16 *)(q + 0x14) - 0x18) << 16;
        *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
        o += 0x64;
        *(s32 *)(o + 0x20) = (*(s16 *)(q + 4) - 0x60) << 16;
        *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
        o += 0x64;
        *(s32 *)(o + 0x20) = (*(s16 *)(q + 4) + 0x20) << 16;
        *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
        *(u16 *)(c + 0x12) += 0x18;
        if (*(u16 *)(c + 0x12) >= 0x800) {
            *(s16 *)(p + 4) = 0xA0;
        }
        break;
    case 0xA0:
        func_80149B20();
        break;
    }
}

void func_80149D64(void) {
    u8 *p;
    u8 *q;
    u32 i;

    p = D_80158A74;
    p[0] = 1;
    p[0xC] = 0;
    *(s16 *)(p + 4) = 0;
    p[0xD] = 0;
    p[0xE] = 0;
    p[0xF] = 0;
    *(s16 *)(p + 6) = 0;
    *(s16 *)(p + 8) = 0;
    *(s16 *)(p + 0xA) = 0;
    *(s16 *)(p + 0x10) = 2;
    *(s32 *)(p + 0x20) = 0x480000;
    *(s32 *)(p + 0x24) = 0x840000;
    *(s16 *)(p + 0x14) = 0x48;
    *(s16 *)(p + 0x16) = 0x9C;
    *(s32 *)(p + 0x28) = 0;
    *(s32 *)(p + 0x2C) = 0;
    *(u8 **)(p + 0x38) = D_80151A50;
    p[0x3C] = 0;
    p[0x3D] = 0xB;
    p[0x3E] = 4;
    p[0x40] = 0x60;
    p[0x48] = 0;
    func_801478D0(p + 0x38);
    *(s16 *)(p + 0x50) = 0;
    *(s16 *)(p + 0x6A) = 0;
    q = D_80158A7C;
    i = 0;
    do {
        i++;
        q += 0x60;
        q[-0x60] = 0;
        *(s32 *)(q - 0x28) = 0;
    } while (i != 2);
}

void func_80149E44(void) {
    u8 *p = D_80158A74;

    *(s32 *)(p + 0x24) = func_800A0070(*(u16 *)(D_80158A74 + 0x6A)) * 0x78 + 0x9C0000;
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    *(u16 *)(p + 0x6A) += 0x10;
}

void func_80149EA0(void) {
    u8 *p;
    u8 *c;
    u8 *s;
    u8 *f;

    p = D_80158A74;
    s = D_80158A64;
    c = D_80158A6C;
    f = D_80158AA8;
    /* abs(x - 0x9C0000) written as the usual ABS() expansion; locals for x or the difference give other registers. T-11810 */
    if ((*(s32 *)(p + 0x24) - 0x9C0000 < 0 ? -(*(s32 *)(p + 0x24) - 0x9C0000) : *(s32 *)(p + 0x24) - 0x9C0000) < 0x8001) {
        *(s32 *)(p + 0x24) = 0x9C0000;
    } else if (*(s32 *)(p + 0x24) < 0x9C0000) {
        *(s32 *)(p + 0x24) += 0x8000;
    } else {
        *(s32 *)(p + 0x24) -= 0x8000;
    }
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    if (*(u16 *)(c + 0x10) >= 0x1E) {
        *(u16 *)(c + 0x10) = 0;
        f[0] = 1;
        *(u16 *)(s + 2) += 1;
    }
    switch (*(u16 *)(c + 0x10)) {
    case 0:
        p[0x3E] = 5;
        break;
    case 1:
        func_80133AD0(0x601);
        break;
    case 6:
        p[0x3E] = 6;
        break;
    case 30:
        p[0x3E] = 7;
        break;
    }
    *(u16 *)(c + 0x10) += 1;
}

void func_8014A000(void) {
    s32 i;
    u8 *a64;
    u8 *a6C;
    u8 *a74;
    u8 *p;
    u8 *aA8;

    a64 = D_80158A64;
    a6C = D_80158A6C;
    a74 = D_80158A74;
    p = D_80158AA4 + 0xFA0;
    aA8 = D_80158AA8;
    if (*(u16 *) (a6C + 0x10) >= 0x2C) {
        *(u16 *) (a6C + 0x10) = 0;
        *(u16 *) (a64 + 2) += 1;
    }
    switch (*(u16 *) (a6C + 0x10)) {
    case 8:
        a74[0x3E] = 8;
        break;
    case 16:
        a74[0x3E] = 9;
        break;
    case 22:
        a74[0x3E] = 0xA;
        break;
    case 28:
        a74[0x3E] = 0xB;
        break;
    case 34:
        a74[0x3E] = 0xC;
        break;
    case 40:
        a74[0x3E] = 0xD;
        break;
    case 42:
        a74[0x3E] = 0xE;
        break;
    case 44:
        a74[0x3E] = 0xF;
        break;
    }
    for (i = 0; i < 4; i++) {
        *(s32 *) (p + 0x24) += 0xFFFC0000;
        *(s16 *) (p + 0x16) = *(s16 *) (p + 0x26);
        p += 0x64;
    }
    for (i = 0; i < 7; i++) {
        *(s16 *) (aA8 + 0xC) += 4;
        aA8 += 0x14;
    }
    D_80158AA8[0x78] = 0;
    *(s16 *) (D_80158AA4 + 0x16) += 4;
    *(u16 *) (a6C + 0x10) += 1;
}

void func_8014A198(void) {
    u8 *a;
    u8 *b;
    u8 *c;
    u8 *p;
    s16 t;

    a = D_80158AA4;
    b = D_80158A64;
    p = a + 0xC80;
    c = D_80158AB0;
    if (*(u16 *)(a + 0xCDA) >= 0x1000) {
        *(u16 *)(b + 2) += 1;
    }
    *(u32 *)(p + 0x20) = 0x18000000 / *(u16 *)(p + 0x5A);
    *(s32 *)(p + 0x24) = 0x6C0000 - 0x3600 / *(u16 *)(p + 0x5A);
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    *(s16 *)(c + 4) = *(s16 *)(p + 0x14);
    *(s16 *)(c + 6) = *(s16 *)(p + 0x16) + 0x18;
    t = 0x180000 / *(u16 *)(p + 0x5A);
    *(s16 *)(c + 2) = t;
    *(s16 *)(c + 0) = t;
    if (*(u16 *)(p + 0x52) & 1) {
        p[0x46] = 0x80;
        p[0x47] = 0;
    } else {
        p[0x46] = 0;
        p[0x47] = 0x80;
    }
    *(u16 *)(p + 0x52) += 1;
    *(u16 *)(p + 0x5A) += 0x40;
}

#ifdef NON_MATCHING
/* NON_MATCHING: T-11840/T-11850, 28 lines (registers only, case 7): the original loads the constants 0x180, 0x1A, 0x3D04
 * and 2 into t2..t5 before every other value of the block (0x100000 then gets t6), this draft at their stores. ugen
 * hands out temporaries first-in first-out by free order (scratch test), so four ugen temps loaded first would make
 * 0x1000 t6, not the original's t2; the original behaves as if t2..t5 were uopt colours of four hoisted constants
 * (needs each constant to be worth a register and v0..t1 busy). Tried: autotweak 300 s, an empty if at every line of
 * case 7, store orders, u16 stores (all 64 s16/u16 combinations), constant locals (s16/s32/u16/u32, chained or not),
 * self-chains, duplicate stores, empty compares, zero-store chains. */
void func_8014A2B4(void) {
    u8 *s;
    u8 *e;
    u8 *g;
    s32 pad1; /* FAKE: stack slot of the original frame. T-11840 */
    u8 *w;
    u8 *y;
    u8 *p;
    u8 *f;
    s32 pad3; /* FAKE: stack slot of the original frame. T-11840 */
    u32 k; /* one unsigned local for both selectors: $v1 in cases 2 and 9, compare constant 1 shared. T-11840 */

    s = D_80158A64;
    p = D_80158AA4 + 0xC80;
    g = D_80158A60;
    w = D_80158A6C;
    e = D_80158A74;
    y = D_80158AB0;
    switch (*(u16 *)(s + 2)) {
    case 0:
        if (*(u16 *)(D_80158A60 + 0xA4) != 0) {
            func_8014304C(0x80016000, 0xC812, 0x2420);
            func_8014304C(0x8002E800, 0xC7C1, 0x69E4);
            func_8014304C(0x8019A000, 0xC7C5, 0x27D60);
            func_8014304C(0x8001A000, 0xBFD1, 0xE20);
            func_8014304C(0x801C2000, 0xBF9C, 0x1A680);
        }
        *(u16 *)(s + 2) = 1;
        break;
    case 1:
        func_80044750(0x22);
        func_80044750(0x2F);
        func_80044750(0x205);
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
        s[0x11] = 0;
        s[0x24] = 0;
        *(u8 ***)(s + 0x18) = D_8015676C;
        s[0x1C] = 0x20;
        func_80149704();
        func_801498C8();
        func_80149D64();
        *(s16 *)(w + 0x10) = 0;
        w[0xA] = 0;
        *(u16 *)(s + 0x26) = 0;
        *(u16 *)(s + 2) += 1;
        *(s16 *)(s + 4) = 0x40;
        break;
    case 2:
        if (*(u16 *)(D_80158A60 + 0xA4) != 0) {
            k = func_800460DC();
            if ((s32)((k - func_801430C4() + 0x100) & 0xFF) >= 5) {
                func_80044750(0x12);
                *(u16 *)(s + 2) += 1;
            }
        } else {
            *(u16 *)(s + 2) = 4;
        }
        func_80149E44();
        func_80149BB0();
        break;
    case 3:
        if (*(u16 *)(D_80158A60 + 0xA4) != 0) {
            *(u16 *)(g + 0xA4) = 0;
            if (func_80044F94(1) != 0) {
                *(u16 *)(s + 2) += 1;
            }
        }
        func_80149E44();
        func_80149BB0();
        break;
    case 4:
        if (*(u16 *)(D_80158A64 + 0x26) >= 0x708) {
            s[0x24] = 0;
            s[0x22] = 1;
            g[0x52] = 0xFF;
            *(u8 **)(g + 0x4C) = D_80150A50;
            *(s16 *)s = 2;
            s[9] = 0;
            s[0xA] = 0;
            s[0xB] = 0;
            *(u16 *)(s + 2) = 0;
            *(s16 *)(s + 4) = 0;
            *(s16 *)(s + 6) = 0;
        }
        func_80149E44();
        func_80149BB0();
        func_801494D8();
        *(u16 *)(s + 0x26) += 1;
        break;
    case 5:
        f = D_80158AA8 + w[0xA] * 0x14;
        *f = *(u16 *)(g + 8) & 1;
        if (w[0xA] == 0) {
            func_80149EA0();
        } else {
            if (*(u16 *)(w + 0x10) >= 0x1E) {
                *(u16 *)(w + 0x10) = 0;
                *(u16 *)(s + 2) = 9;
            }
            *(u16 *)(w + 0x10) += 1;
        }
        break;
    case 6:
        func_8014A000();
        break;
    case 7:
        *e = 0;
        *(s32 *)(p + 0x20) = 0x100000;
        *(s32 *)(p + 0x24) = 0x6C0000;
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x52) = 0;
        *(s16 *)(p + 0x5A) = 0x180;
        p[0] = 1;
        p[1] = 0;
        *(s16 *)(p + 0x44) = 0;
        p[0x3B] = 0x80;
        p[0x4D] = 0;
        p[0x4E] = 1;
        p[0x50] = 0;
        p[0x46] = 0x80;
        p[0x47] = 0;
        p[0x4F] = 1;
        *(s16 *)(p + 0x3C) = 0x80;
        *(s16 *)(p + 0x3E) = 0x80;
        *(s16 *)(p + 0x40) = 0x1A;
        *(s16 *)(p + 0x42) = 0x3D04;
        p[0x4C] = 0;
        *(s16 *)(p + 0x10) = 2;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0;
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        *(s16 *)(y + 4) = *(s16 *)(p + 0x14);
        *(s16 *)(y + 6) = *(s16 *)(p + 0x16);
        *(s16 *)(y + 2) = 0x1000;
        *(s16 *)y = *(s16 *)(y + 2);
        *(u8 **)(p + 0x48) = y;
        func_80133AD0(0x614);
        *(u16 *)(s + 2) += 1;
        break;
    case 8:
        func_8014A198();
        break;
    case 9:
        k = D_80158A6C[0xA];
        switch (k) {
        case 0:
            *(s16 *)s = 2;
            break;
        case 1:
            *(s16 *)s = 7;
            break;
        case 2:
            *(s16 *)s = 3;
            break;
        case 3:
            *(s16 *)s = 8;
            break;
        }
        s[9] = 1;
        break;
    }
    if (s[9] != 0) {
        *(s16 *)(s + 6) = 0;
        *(s16 *)(s + 4) = 0;
        *(u16 *)(s + 2) = 0;
        s[0xB] = 0;
        s[0xA] = 0;
        s[9] = 0;
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_8014A2B4);
#endif

void func_8014A7E0(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = D_80158A80; i < 12; i++, p += 0x54) {
        *p = 0;
    }
}

void func_8014A814(void) {
    s32 i;
    u8 *p;
    int new_var; /* FAKE: the dead test of D_80158A80 reserves two registers (a0/a1) so the hoisted constants land in a2..t2 as in the original; found with decomp-permuter. T-10910 */

    for (i = 0, p = D_80158A80; i < 12; i++, p += 0x54) {
        p[0] = 0;
        new_var = D_80158A80 && D_80158A80;
        *(s16 *)(p + 0x10) = 3;
        *(s16 *)(p + 0x3E) = 0xA;
        *(s16 *)(p + 0x40) = 0x3C87;
        *(s16 *)(p + 4) = 0x40;
        *(s16 *)(p + 10) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        if (new_var && D_80158A80) { /* FAKE: see above. T-10910 */
        }
        p[0xE] = 0;
        p[0xD] = 0;
    }
}

void func_8014A8F8(u8 *arg0, u16 arg1) {
    *(u16 *)(arg0 + 2) = arg1;
    switch (arg1) {
    case 8:
    default:
        arg0[0x42] = 0x80;
        arg0[0x43] = 0xC0;
        break;
    case 9:
        arg0[0x42] = 0x90;
        arg0[0x43] = 0xC0;
        break;
    case 10:
        arg0[0x42] = 0x80;
        arg0[0x43] = 0xD0;
        break;
    }
}

void func_8014A958(u8 *arg0, s32 arg1) {
    u8 *p;
    u8 *q;
    u8 *r;

    p = D_80158A74;
    q = D_80158A78;
    if (arg1 < 6) {
        func_8014A8F8(arg0, p[0x55]);
    } else {
        func_8014A8F8(arg0, q[0x92]);
    }
    if (arg1 < 6) {
        *(s32 *)(arg0 + 0x20) = *(s16 *)(p + 0x14) << 16;
        *(s32 *)(arg0 + 0x24) = *(s16 *)(p + 0x16) << 16;
    } else if (arg1 < 8) {
        q = D_80158A78;
        *(s32 *)(arg0 + 0x20) = *(s16 *)(q + 0x50) << 16;
        *(s32 *)(arg0 + 0x24) = *(s16 *)(q + 0x70) << 16;
    } else if (arg1 < 10) {
        q = D_80158A78;
        r = q + 0xA4;
        *(s32 *)(arg0 + 0x20) = *(s16 *)(q + 0xF4) << 16;
        *(s32 *)(arg0 + 0x24) = *(s16 *)(r + 0x70) << 16;
    } else {
        q = D_80158A78;
        r = q + 0x148;
        *(s32 *)(arg0 + 0x20) = *(s16 *)(q + 0x198) << 16;
        *(s32 *)(arg0 + 0x24) = *(s16 *)(r + 0x70) << 16;
    }
    if (arg1 >= 2 && arg1 < 4) {
        *(s32 *)(arg0 + 0x28) = 0xFFFD0000;
    } else if (arg1 >= 4 && arg1 < 6) {
        *(s32 *)(arg0 + 0x28) = 0x30000;
    } else {
        *(s32 *)(arg0 + 0x28) = 0;
    }
    *(s32 *)(arg0 + 0x2C) = 0xFFFA0000;
}

void func_8014AAE8(u8 *arg0) {
    *(s32 *)(arg0 + 0x20) += *(s32 *)(arg0 + 0x28);
    *(s32 *)(arg0 + 0x24) += *(s32 *)(arg0 + 0x2C);
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x22);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x26);
    if (*(s16 *)(arg0 + 0x16) < 0) {
        *(s16 *)(arg0 + 4) = 0xA0;
        *(s16 *)(arg0 + 0xA) = 0;
        *(s16 *)(arg0 + 8) = 0;
        *(s16 *)(arg0 + 6) = 0;
        arg0[0xF] = 0;
        arg0[0xE] = 0;
        arg0[0xD] = 0;
    }
}

void func_8014AB48(void) {
    s32 i;
    u32 limit;
    u16 state;
    TtRecord54 *record;

    record = (TtRecord54 *)D_80158A80;
    i = 0;
    for (limit = 12; i != limit;) {
        if (record->unk_00 != 0) {
            state = record->unk_04;
            switch (state) {
            case 0x20:
                func_8014A958((u8 *)record, i);
                record->unk_04 = 0x40U;
                record->unk_0A = 0;
                record->unk_08 = 0;
                record->unk_06 = 0;
                record->unk_0F = 0;
                record->unk_0E = 0;
                record->unk_0D = 0;
                break;
            case 0x80:
                record->unk_42 = 0xD0;
                record->unk_43 = 0xD0;
                record->unk_44 = 1U;
                record->unk_04 = 0x81U;
                record->unk_0A = 0;
                record->unk_08 = 0;
                record->unk_06 = 0;
                record->unk_0F = 0;
                record->unk_0E = 0;
                record->unk_0D = 0;
                record->unk_2C = 0;
                record->unk_28 = 0;
                break;
            case 0x81:
                if (--record->unk_44 == 0) {
                    record->unk_04 = 0x82U;
                }
                break;
            case 0x82:
                record->unk_42 = 0xE0;
                record->unk_43 = 0xD0;
                record->unk_44 = 1U;
                record->unk_04 = 0x83U;
                break;
            case 0x83:
                if (--record->unk_44 == 0) {
                    record->unk_04 = 0xA0U;
                }
                break;
            case 0xA0:
                record->unk_00 = 0;
                break;
            }
            func_8014AAE8((u8 *)record);
        }
        i += 1;
        record++;
    }
}

void func_8014ACEC(void) {
    DR_MODE *m;
    TtDrawBuffer80B8 *buffer;
    TtSpritePacket20 *q;
    s32 i;
    u8 *p;

    p = D_80158A80;
    buffer = (TtDrawBuffer80B8 *)(D_80158AB4 + ((TtDrawSelect0C *)D_80158A68)->index * 0x80B8);
    for (i = 0; i != 12; i++) {
        if (p[0] != 0) {
            q = (TtSpritePacket20 *)buffer->cursor;
            buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x14);
            q->tag.len = 4; q->code = 0x65; /* one line (setlen/setcode macro style) */
            q->x = *(s16 *)(p + 0x22) - 8;
            q->y = *(s16 *)(p + 0x26) - 8;
            q->h = 0x10;
            q->w = 0x10;
            q->u = p[0x42];
            q->v = p[0x43];
            q->clut = *(u16 *)(p + 0x40);
            q->tag.addr = buffer->ordering[*(u16 *)(p + 0x10)].addr;
            buffer->ordering[*(u16 *)(p + 0x10)].addr = (u32)q;
            m = buffer->cursor;
            buffer->cursor++;
            func_8009D294((s32)m, 0, 0, *(u16 *)(p + 0x3E), 0);
            ((TtOtTag *)m)->addr = buffer->ordering[*(u16 *)(p + 0x10)].addr;
            buffer->ordering[*(u16 *)(p + 0x10)].addr = (u32)m;
        }
        if ((s32)p != 12) {} /* FAKE: a second use of the bound gives it a saved register ahead of i. T-11800 */
        p += 0x54;
    }
}
