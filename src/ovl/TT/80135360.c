#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_80135360);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_801356C8);

void func_80135848(s32 arg0) {
    D_80158A8C[0x3E] = ((arg0 + 0x40) & 0xFFF) / 128;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_80135870);

void func_80135A54(void) {
    u8 *p = D_80158A8C;

    *(s32 *)(p + 0x20) = 0;
    *(s32 *)(p + 0x24) += 0x10000;
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    if (*(s16 *)(p + 0x16) >= 0x50) {
        *(s32 *)(p + 0x24) = 0x500000;
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        *(s16 *)(p + 4) = 0x30;
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        p[0xE] = 0;
        p[0xD] = 0;
    }
}

void func_80135AC4(void) {
    u8 *p = D_80158A8C;

    switch (*(u16 *)(p + 8)) {
    case 0:
        p[0x54] = 0x80;
        *(u16 *)(p + 8) = 0x40;
        break;
    case 0x40:
        func_80142AF0(D_80150978, 1);
        p[0x54] &= 1;
        if (p[0x54] != 0) {
            *(u8 **)(p + 0x218) = D_80155B54;
            p[0x343] = 1;
            *(u16 *)(p + 8) = 0;
            *(u16 *)(p + 4) = 0x40;
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_80135B5C);

void func_80135E9C(void) {
    u8 *q;

    q = D_80158A8C;
    *(s32 *)(q + 0x20) = func_800A0140(*(u16 *)(D_80158A8C + 0x3AE)) * 0x600;
    *(s32 *)(q + 0x24) = (func_800A0070(*(u16 *)(q + 0x3AE) * 2) << 9) + 0x500000;
    *(s16 *)(q + 0x14) = *(s16 *)(q + 0x22);
    *(s16 *)(q + 0x16) = *(s16 *)(q + 0x26);
    if (*(u16 *)(q + 6) == 2) {
        if (*(u16 *)(q + 0x3AE) == 0x400 || *(u16 *)(q + 0x3AE) == 0x1400) {
            func_80135B5C(1);
            return;
        }
        if (*(u16 *)(q + 0x3AE) == 0xC00 || *(u16 *)(q + 0x3AE) == 0x1C00) {
            func_80135B5C(0);
            return;
        }
        *(u16 *)(q + 0x3AE) += 8;
        if (*(u16 *)(q + 0x3AE) >= 0x2001U) {
            *(u16 *)(q + 0x3AE) -= 0x2000;
        }
    } else {
        *(u16 *)(q + 0x3AE) += 8;
        if (*(u16 *)(q + 0x3AE) >= 0x2001U) {
            *(u16 *)(q + 0x3AE) -= 0x2000;
        }
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_80135FA8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_80136244);

void func_801364EC(void) {
    u8 *q;

    q = D_80158A8C;
    switch (*(u16 *)(q + 6)) {
    case 0:
    case 1:
        if (*(u16 *)(D_80158A8C + 0x364) >= 0x60U) {
            func_80133D14(*(s16 *)(q + 0x14), *(s16 *)(q + 0x16), 0x18, 0xA0);
        }
        break;
    case 2:
        if (*(u16 *)(D_80158A8C + 0x364) >= 0x60U) {
            func_80133FB0(*(s16 *)(q + 0x14), *(s16 *)(q + 0x16), 0x100, 0x18, 0xA0);
        }
        break;
    }
    if (*(u16 *)(q + 0x364) >= 0x60U) {
        *(u16 *)(q + 0x364) = 0;
        return;
    }
    *(u16 *)(q + 0x364) += 1;
}

void func_801365C4(s32 arg0) {
    u8 *c;
    s16 v;
    u8 *t;
    u8 *s;

    s = D_80158A8C;
    c = D_80158A64;
    t = D_80158A74;
    switch (*(u16 *)(s + 4)) {
    case 0x0:
        func_80142570(arg0, t);
        if (*(u16 *)(c + 4) == 0) {
            func_80044750(0xB1);
            func_800451E0(0x8019A000);
            func_80044750(0x12);
            func_80044750(0x23);
            func_80044750(0x2E);
            func_80044750(0x21);
            func_80044750(4);
            func_80044750(0x200);
        }
        func_80135870();
        *(s16 *)(s + 4) = 0x20;
        break;
    case 0x20:
        func_80135A54();
        func_80136244();
        break;
    case 0x30:
        func_80135AC4();
        func_80136244();
        break;
    case 0x40:
        v = *(s16 *)(D_80158A8C + 0x30C);
        if (v < 0x20) {
            *(s16 *)(s + 6) = 2;
        } else if (v < 0x40) {
            *(s16 *)(s + 6) = 1;
        } else {
            *(s16 *)(s + 6) = 0;
        }
        func_80135E9C();
        func_80136244();
        if (*(u16 *)(s + 8) == 0) {
            func_801364EC();
        }
        if (*(s16 *)(s + 0x30C) < 0) {
            *(s16 *)(s + 4) = 0x80;
            *(s16 *)(s + 0xA) = 0;
            *(u16 *)(s + 8) = 0;
            *(s16 *)(s + 6) = 0;
            s[0xF] = 0;
            s[0xE] = 0;
            s[0xD] = 0;
        }
        break;
    case 0x80:
        func_80044750(0xC1);
        t[0x52] = 1;
        func_80135FA8();
        break;
    case 0xA0:
        s[0] = 0;
        c[0x12] = 0;
        *(s16 *)((u8 *)D_80158A70 + 0x1E) = 1;
        *(u16 *)(c + 2) = 0xA0;
        break;
    }
    func_80135360();
}
