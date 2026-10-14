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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801478D0", func_80147A18);

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

/* NON_MATCHING: T-10610, the original keeps the zero of the byte and halfword store pairs in registers (move v1,zero; move a1,zero) and the loop counter/pointer in v1/a1; ~14 lines */
#ifdef NON_MATCHING
void func_80147FD0(u8 *arg0) {
    s32 i;
    u8 *p;
    u8 *src;

    src = D_80158A74;
    arg0[0] = 1;
    *(s16 *)(arg0 + 4) = 0x40;
    arg0[0xE] = arg0[0xD] = 0;
    *(u16 *)(arg0 + 8) = *(u16 *)(arg0 + 6) = 0;
    arg0[0xF] = 0;
    *(s16 *)(arg0 + 0xA) = 0;
    p = arg0 + 4;
    *(s16 *)(arg0 + 0x14) = *(s16 *)(src + 0x14);
    i = 2;
    *(s16 *)(arg0 + 0x16) = *(s16 *)(src + 0x16);
    *(s16 *)(arg0 + 0x50) = *(s16 *)(src + 0x14);
    *(s16 *)(arg0 + 0x70) = *(s16 *)(src + 0x16);
    *(s16 *)(arg0 + 0x52) = *(s16 *)(src + 0x14);
    *(s16 *)(arg0 + 0x72) = *(s16 *)(src + 0x16);
    do {
        i += 4;
        *(s16 *)(p + 0x50) = *(s16 *)(src + 0x14);
        p += 8;
        *(s16 *)(p + 0x68) = *(s16 *)(src + 0x16);
        *(s16 *)(p + 0x4A) = *(s16 *)(src + 0x14);
        *(s16 *)(p + 0x6A) = *(s16 *)(src + 0x16);
        *(s16 *)(p + 0x4C) = *(s16 *)(src + 0x14);
        *(s16 *)(p + 0x6C) = *(s16 *)(src + 0x16);
        *(s16 *)(p + 0x4E) = *(s16 *)(src + 0x14);
        *(s16 *)(p + 0x6E) = *(s16 *)(src + 0x16);
    } while (i != 0xA);
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801478D0", func_80147FD0);
#endif

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
