#include "common.h"
#include "ovl/TT.h"

void func_80146BE0(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = D_80158A84;
    for (; i < 8; i++) {
        *p = 0;
        p += 0x58;
    }
}

void func_80146C14(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = D_80158A84;
    for (; i < 8; i++) {
        *(s16 *)(p + 0x10) = 3;
        *(s16 *)(p + 0x40) = 0xA;
        *(s16 *)(p + 0x42) = 0x3C81;
        p += 0x58;
    }
}

void func_80146C74(u8 *arg0, s32 arg1) {
    u8 *p = D_80158A74;

    if (arg1 < 2) {
        if (arg0[0x3D] != 0) {
            *(s16 *)(arg0 + 0x14) = *(s16 *)(p + 0x14) + 6;
        } else {
            *(s16 *)(arg0 + 0x14) = *(s16 *)(p + 0x14) - 6;
        }
        *(s32 *)(arg0 + 0x20) = *(s16 *)(arg0 + 0x14) << 16;
        *(s16 *)(arg0 + 0x16) = *(s16 *)(p + 0x16) - 0x10;
        *(s32 *)(arg0 + 0x24) = *(s16 *)(arg0 + 0x16) << 16;
        *(s32 *)(arg0 + 0x2C) = 0xFFFC0000;
        arg0[0x44] = 0xA0;
        arg0[0x45] = 0xD0;
        *(s16 *)(arg0 + 0x10) = 3;
        *(s16 *)(arg0 + 0x40) = 0xA;
        *(s16 *)(arg0 + 0x42) = 0x3C81;
        *(s16 *)(arg0 + 0x46) = 0;
        return;
    }
    p = D_80158A78 + (arg1 >> 1) * 0xA4 - 0xA4;
    if (arg0[0x3D] != 0) {
        *(s16 *)(arg0 + 0x14) = *(s16 *)(p + 0x14) + 6;
    } else {
        *(s16 *)(arg0 + 0x14) = *(s16 *)(p + 0x14) - 6;
    }
    *(s32 *)(arg0 + 0x20) = *(s16 *)(arg0 + 0x14) << 16;
    *(s16 *)(arg0 + 0x16) = *(s16 *)(p + 0x16) - 0x10;
    *(s32 *)(arg0 + 0x24) = *(s16 *)(arg0 + 0x16) << 16;
    *(s32 *)(arg0 + 0x2C) = 0xFFFC0000;
    arg0[0x44] = 0xA0;
    arg0[0x45] = 0xD0;
    *(s16 *)(arg0 + 0x10) = 3;
    *(s16 *)(arg0 + 0x40) = 0xA;
    *(s16 *)(arg0 + 0x42) = 0x3C81;
    *(s16 *)(arg0 + 0x46) = 0;
}

void func_80146DC8(u8 *arg0) {
    u32 v = (u16)++*(u16 *)(arg0 + 0x46);

    if (v >= 0x15) {
        arg0[0x44] = 0xC0;
    } else if (v >= 0xB) {
        arg0[0x44] = 0xB0;
    } else {
        arg0[0x44] = 0xA0;
    }
}

void func_80146E10(void *arg0, s32 arg1) {
    s16 temp_a2;
    s16 temp_v1;
    s16 temp_v1_2;
    s32 temp_a3;
    s32 temp_v1_4;
    s32 temp_v1_5;
    s32 temp_v1_6;
    s32 temp_v1_7;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a1_3;
    s32 var_a1_4;
    s32 var_a1_5;
    s32 var_a1_6;
    s32 var_a2;
    u16 temp_v1_3;
    u8 *temp_a0_2;
    u8 *var_a0;
    u8 *temp_a0;
    u8 *temp_v0;

    *(s32 *)((unsigned char *)arg0 + (32)) = (s32) (*(s32 *)((unsigned char *)arg0 + (32)) + *(s32 *)((unsigned char *)arg0 + (40)));
    temp_v0 = (arg1 * 0x58) + D_80158A88;
    *(s32 *)((unsigned char *)arg0 + (36)) = (s32) (*(s32 *)((unsigned char *)arg0 + (36)) + *(s32 *)((unsigned char *)arg0 + (44)));
    *(s16 *)((unsigned char *)arg0 + (20)) = (s16) *(s16 *)((unsigned char *)arg0 + (34));
    *(s16 *)((unsigned char *)arg0 + (22)) = (s16) *(s16 *)((unsigned char *)arg0 + (38));
    temp_a2 = *(s16 *)((unsigned char *)temp_v0 + (20));
    temp_v1 = *(s16 *)((unsigned char *)arg0 + (20));
    temp_a3 = temp_v1 - temp_a2;
    var_a1 = temp_a3;
    if (temp_a3 < 0) {
        var_a1 = -temp_a3;
    }
    if (var_a1 < 2) {
        *(s32 *)((unsigned char *)arg0 + (40)) = 0;
    } else {
        *(s32 *)((unsigned char *)arg0 + (40)) = (s32) ((temp_a2 - temp_v1) << 0xC);
        var_a1_2 = 0xFFFE0000;
        if (temp_v1 < *(s16 *)((unsigned char *)temp_v0 + (20))) {
            var_a1_2 = 0x20000;
        }
        *(s32 *)((unsigned char *)arg0 + (40)) = (s32) (*(s32 *)((unsigned char *)arg0 + (40)) + var_a1_2);
    }
    temp_v1_2 = *(s16 *)((unsigned char *)arg0 + (22));
    if ((temp_v1_2 < *(s16 *)((unsigned char *)temp_v0 + (22))) || (temp_v1_2 < 0)) {
        (*(s32 *)arg0) = 0;
        temp_v1_3 = *(u16 *)((unsigned char *)temp_v0 + (68));
        (*(s32 *)temp_v0) = 0;
        if (temp_v1_3 != 0xFFFF) {
            temp_a0 = (temp_v1_3 * 0x78) + D_80158A9C;
            if (*(u16 *)((unsigned char *)temp_a0 + (4)) == 0x40) {
                *(u16 *)((unsigned char *)temp_a0 + (4)) = 0x70U;
                *(s16 *)((unsigned char *)temp_a0 + (10)) = 0;
                *(s16 *)((unsigned char *)temp_a0 + (8)) = 0;
                *(s16 *)((unsigned char *)temp_a0 + (6)) = 0;
                *(s8 *)((unsigned char *)temp_a0 + (15)) = 0;
                *(s8 *)((unsigned char *)temp_a0 + (14)) = 0;
                *(s8 *)((unsigned char *)temp_a0 + (13)) = 0;
            }
        } else {
            var_a0 = D_80158A9C;
            var_a2 = 0;
            do {
                var_a2 += 2;
                if (*(u16 *)((unsigned char *)var_a0 + (4)) == 0x40) {
                    temp_v1_4 = *(s16 *)((unsigned char *)temp_v0 + (20)) - *(s16 *)((unsigned char *)var_a0 + (20));
                    var_a1_3 = temp_v1_4;
                    if (temp_v1_4 < 0) {
                        var_a1_3 = -temp_v1_4;
                    }
                    if (var_a1_3 < 8) {
                        temp_v1_5 = *(s16 *)((unsigned char *)temp_v0 + (22)) - *(s16 *)((unsigned char *)var_a0 + (22));
                        var_a1_4 = temp_v1_5;
                        if (temp_v1_5 < 0) {
                            var_a1_4 = -temp_v1_5;
                        }
                        if (var_a1_4 < 8) {
                            *(u16 *)((unsigned char *)var_a0 + (4)) = 0x70U;
                            *(s16 *)((unsigned char *)var_a0 + (10)) = 0;
                            *(s16 *)((unsigned char *)var_a0 + (8)) = 0;
                            *(s16 *)((unsigned char *)var_a0 + (6)) = 0;
                            *(s8 *)((unsigned char *)var_a0 + (15)) = 0;
                            *(s8 *)((unsigned char *)var_a0 + (14)) = 0;
                            *(s8 *)((unsigned char *)var_a0 + (13)) = 0;
                        }
                    }
                }
                temp_a0_2 = var_a0 + 0x78;
                if (*(u16 *)((unsigned char *)var_a0 + (124)) == 0x40) {
                    temp_v1_6 = *(s16 *)((unsigned char *)temp_v0 + (20)) - *(s16 *)((unsigned char *)temp_a0_2 + (20));
                    var_a1_5 = temp_v1_6;
                    if (temp_v1_6 < 0) {
                        var_a1_5 = -temp_v1_6;
                    }
                    if (var_a1_5 < 8) {
                        temp_v1_7 = *(s16 *)((unsigned char *)temp_v0 + (22)) - *(s16 *)((unsigned char *)temp_a0_2 + (22));
                        var_a1_6 = temp_v1_7;
                        if (temp_v1_7 < 0) {
                            var_a1_6 = -temp_v1_7;
                        }
                        if (var_a1_6 < 8) {
                            *(s16 *)((unsigned char *)temp_a0_2 + (4)) = 0x70;
                            *(s16 *)((unsigned char *)temp_a0_2 + (10)) = 0;
                            *(s16 *)((unsigned char *)temp_a0_2 + (8)) = 0;
                            *(s16 *)((unsigned char *)temp_a0_2 + (6)) = 0;
                            *(s8 *)((unsigned char *)temp_a0_2 + (15)) = 0;
                            *(s8 *)((unsigned char *)temp_a0_2 + (14)) = 0;
                            *(s8 *)((unsigned char *)temp_a0_2 + (13)) = 0;
                        }
                    }
                }
                var_a0 = temp_a0_2 + 0x78;
            } while (var_a2 != 0x20);
        }
    }
}

void func_80147074(void) {
    u32 limit;
    s32 i;
    u16 mode;
    TtState58 *p;

    limit = 8;
    p = (TtState58 *)D_80158A84;
    i = 0;
    do {
        if (p->unk_00 != 0 && p->unk_01 == 0) {
            mode = p->unk_04;
            switch (mode) {
            case 0x20:
                func_80146C74((u8 *)p, i);
                p->unk_04 = 0x40U;
                p->unk_0A = 0;
                p->unk_08 = 0;
                p->unk_06 = 0;
                p->unk_0F = 0;
                p->unk_0E = 0;
                p->unk_0D = 0;
                break;
            case 0x40:
                func_80146DC8((u8 *)p);
                break;
            }
            func_80146E10((u8 *)p, i);
        }
        i++;
        p++;
    /* FAKE: narrowing the constant limit keeps 8 in s3 after 0x40. T-9520. */
    } while (i != (limit & 0xFF));
}

void func_80147150(void) {
    TtDrawBuffer80B8 *buffer;
    TtSpritePacket20 *q;
    DR_MODE *m;
    u8 *p;
    s32 i;
    s32 n;

    p = D_80158A84;
    buffer = (TtDrawBuffer80B8 *)(D_80158AB4 + ((TtDrawSelect0C *)D_80158A68)->index * 0x80B8);
    for (i = 0, n = 8; i != n; i++, p += 0x58) {
        if (p[0] != 0 && !(*(u16 *)(p + 0x3E) & 1)) {
            q = (TtSpritePacket20 *)buffer->cursor;
            buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x14);
            q->tag.len = 4; q->code = 0x65;
            q->x = *(s16 *)(p + 0x22) - 8;
            q->y = *(s16 *)(p + 0x26) - 8;
            q->h = 0x10;
            q->w = 0x10;
            q->u = p[0x44];
            q->v = p[0x45];
            q->clut = *(u16 *)(p + 0x42);
            q->tag.addr = buffer->ordering[*(u16 *)(p + 0x10)].addr;
            buffer->ordering[*(u16 *)(p + 0x10)].addr = (u32)q;
            m = buffer->cursor;
            buffer->cursor++;
            func_8009D294((s32)m, 0, 0, *(u16 *)(p + 0x40), 0);
            ((TtOtTag *)m)->addr = buffer->ordering[*(u16 *)(p + 0x10)].addr;
            buffer->ordering[*(u16 *)(p + 0x10)].addr = (u32)m;
        }
    }
}
