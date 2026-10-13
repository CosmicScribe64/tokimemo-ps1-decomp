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

#ifdef NON_MATCHING
/* NON_MATCHING: T-10720, shape matches; the hoisted loop constants 0x40/0x20 sit in t0/t1 where the original has t1/t0 and one li/sh pair in the hit branch is ordered differently, about 10 lines */
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
            if (p[0] != 0 && *(u16 *)(p + 4) == 0x40 && p[0x64] == 0) {
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
                        *(s16 *)(arg0 + 0x44) = i;
                        *(s32 *)(arg0 + 0x1C) = *(s32 *)(p + 0x1C);
                        p[0x64] = 1;
                        break;
                    }
                }
            }
        }
    } else {
        src = D_80158A78 + (arg1 >> 1) * 0xA4 - 0xA4;
        *(s32 *)(arg0 + 0x20) = *(s16 *)(src + 0x14) << 16;
        *(s32 *)(arg0 + 0x1C) = *pp - ((*(s16 *)(src + 0x16) - 0x60) << 16);
        *(u16 *)(arg0 + 0x44) = 0xFFFF;
        i = 0;
        p = D_80158A9C;
        for (; i != 0x20; i++, p += 0x78) {
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
                        *(s16 *)(arg0 + 0x44) = i;
                        *(s32 *)(arg0 + 0x1C) = *(s32 *)(p + 0x1C);
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
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_80148E7C);
#endif

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_801491F0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_80149400);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_801494D8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_80149704);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_801498C8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_80149B20);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_80149BB0);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_80149EA0);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_8014A2B4);

void func_8014A7E0(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = D_80158A80; i < 12; i++, p += 0x54) {
        *p = 0;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_8014A814);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148DB0", func_8014ACEC);
