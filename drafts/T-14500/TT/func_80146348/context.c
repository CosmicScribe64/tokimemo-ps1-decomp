#include "common.h"
#include "ovl/TT.h"

s32 func_80145E60(void) {
    u8 *a;
    u8 *b;
    u16 v;
    u16 k;

    a = D_80158A60;
    b = D_80158A6C;
    v = *(u16 *)(b + 8);
    if (v < *(u16 *)(a + 0x30)) {
        *(u16 *)(a + 0x3C) = *(u16 *)(a + 0x36);
        *(u16 *)(a + 0x36) = *(u16 *)(a + 0x30);
        *(u16 *)(a + 0x30) = *(u16 *)(b + 8);
        func_800AE0A0(a + 0x3E, a + 0x38, 3);
        func_800AE0A0(a + 0x38, a + 0x32, 3);
        *(u16 *)(b + 0x18) = 0;
    } else if (v < *(u16 *)(a + 0x36)) {
        *(u16 *)(a + 0x3C) = *(u16 *)(a + 0x36);
        *(u16 *)(a + 0x36) = *(u16 *)(b + 8);
        func_800AE0A0(a + 0x3E, a + 0x38, 3);
        *(u16 *)(b + 0x18) = 1;
    } else if (v < *(u16 *)(a + 0x3C)) {
        *(u16 *)(a + 0x3C) = *(u16 *)(b + 8);
        *(u16 *)(b + 0x18) = 2;
    } else {
        return 0;
    }
    a[*(u16 *)(b + 0x18) * 6 + 0x32] = 0xA;
    a[*(u16 *)(b + 0x18) * 6 + 0x33] = 0x2F;
    a[*(u16 *)(b + 0x18) * 6 + 0x34] = 0x2F;
    a[*(u16 *)(b + 0x18) * 6 + 0x35] = 0xFF;
    k = *(u16 *)(b + 0x18);
    func_801486D8(a + 0x32 + k * 6, k + 0x2B);
    return 1;
}

#ifdef NON_MATCHING
/* NON_MATCHING: T-11820, 7 instructions (registers only): the D_80158A6C local c gets a1 and the record constant 8 (u16 stores) a0 where the original has a0/v1; regdiff: the 8 web (priority 2) is coloured before c (priority 1). The u16 view of the three record stores splits the constant 8 from the s16 one in s6. Tried: empty if (c) uses, other assignment and declaration orders. */
void func_80146000(void) {
    u8 *src;
    u8 *p;
    s32 i;
    s32 angle;
    u8 *c;
    u8 *a;
    u8 *b;

    c = D_80158A6C;
    src = D_80158AB0;
    a = D_80158AA8;
    *(s16 *)(a + 0x14A) = -0x40;
    *(s16 *)(a + 0x14C) = 0x48;
    *(s16 *)(a + 0x14E) = 0x11;
    a[0x142] = 1;
    a[0x140] = 1;
    *(u16 *)(a + 0x146) = 8;
    *(s16 *)(a + 0x150) = 0;
    a += 0x154;
    *(s16 *)(a + 0xA) = 0;
    *(s16 *)(a + 0xC) = 0x58;
    *(s16 *)(a + 0xE) = 0x26;
    a[2] = 1;
    a[0] = 1;
    *(u16 *)(a + 6) = 8;
    *(s16 *)(a + 0x10) = 0;
    b = D_80158AA4;
    b[0] = 1;
    b[1] = 0;
    *(u16 *)(b + 0x44) = 8;
    *(s16 *)(b + 0x14) = 0x34;
    *(s16 *)(b + 0x16) = *(u16 *)(c + 0x18) * 16 + 0x7E;
    *(s16 *)(b + 0x3C) = 8;
    *(s16 *)(b + 0x3E) = 8;
    b[0x46] = 0x30;
    b[0x47] = 0x10;
    *(s16 *)(b + 0x40) = 0xA;
    *(s16 *)(b + 0x42) = 0x3C0F;
    b[0x3B] = 0x65;
    *(s16 *)(b + 0x10) = 0;
    for (i = 0, angle = 0, p = D_80158AA4 + 0x1900; i < 3; i++, angle += 0x55, p += 0x64) {
        p[0] = 1;
        p[1] = 0;
        p[0x3B] = 0x80;
        *(u8 **)(p + 0x48) = src;
        p[0x4D] = 0;
        p[0x4E] = 1;
        p[0x38] = 0x40;
        p[0x39] = 0x40;
        p[0x3A] = 0x40;
        p[0x50] = 0;
        *(s16 *)(p + 0x14) = *(s16 *)(src + 4) - 0x24;
        *(s16 *)(p + 0x16) = *(s16 *)(src + 6) - 0x24;
        *(s16 *)(p + 0x14) = func_800A0140(angle) / 128;
        *(s16 *)(p + 0x16) = func_800A0070(angle) / 48 + 0x78;
        p[0x46] = (i & 0xF) * 8;
        p[0x47] = (i / 16) * 8;
        p[0x4F] = 0;
        *(s16 *)(p + 0x3C) = 8;
        *(s16 *)(p + 0x3E) = 8;
        *(s16 *)(p + 0x40) = 0xA;
        *(s16 *)(p + 0x42) = 0x3C00;
        p[0x4C] = 0;
        *(s16 *)(p + 0x10) = 0xD;
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80145E60", func_80146000);
#endif

void func_80146224(void) {
    u8 *p = D_80158A6C;
    s16 *q = (s16 *)D_80158AB0;
    s32 i;

    for (i = 0; i != 0x19; i++) {
        s16 v = func_800A0140(*(u16 *)(p + 0x1E));

        q += 8;
        q[-8] = v;
        q[-7] = 0x1000;
    }
    *(u16 *)(p + 0x1E) += 0x10;
}

void func_801462AC(void) {
    u8 *p;
    u8 *q;
    s32 i;

    p = D_80158A64;
    p[0x1D] += 4;
    p[0x1E] = p[0x1F] = p[0x1D];
    if (p[0x1D] >= 0x80U) {
        q = D_80158AA8;
        for (i = 0; i < 0x80; i++) {
            q += 0x14;
            *(s16 *)(q - 0xE) = 0;
        }
        q = D_80158AA4;
        for (i = 0; i < 0x80; i++) {
            *(s16 *)(q + 0x44) = 0;
            q += 0x64;
        }
        *(s16 *)(p + 2) = 0x40;
    }
    func_80146224();
}

void func_80146348(void) {
    u8 *sp44;
    u8 *sp40;
    u8 *sp3C;
    u8 *sp38;
    u8 *sp34;
    u8 *sp30;
    u8 sp2B;
    s32 var_t3;
    s32 var_t5;
    s32 var_t6;
    u16 temp_a2;
    u16 temp_t0;
    u16 temp_t1;
    u16 temp_t4;
    u16 temp_t6;
    u16 temp_t6_2;
    u16 temp_t9;
    u16 temp_t9_2;
    u16 temp_v0;
    u16 temp_v0_2;
    u16 temp_v0_3;
    u16 temp_v0_4;
    u16 temp_v0_5;
    u16 var_a0;
    u16 var_v0;
    u16 var_v0_2;
    u8 *temp_a0;
    u8 *var_v1;
    u8 var_v0_3;

    var_v1 = D_80158A6C;
    sp44 = D_80158A60;
    sp40 = D_80158A64;
    temp_a0 = D_80158A60 + 0x10;
    sp38 = (*(u16 *)((unsigned char *)var_v1 + (24)) * 0x3C) + D_80158AA8;
    sp30 = temp_a0;
    sp34 = D_80158AA4;
    temp_v0 = *(u16 *)((unsigned char *)D_80158A64 + (4));
    switch (temp_v0) {
    case 0x0:
        *(s16 *)((unsigned char *)var_v1 + (26)) = 0;
        *(s16 *)((unsigned char *)var_v1 + (28)) = 0xA;
        *(s16 *)((unsigned char *)sp40 + (4)) = 0x40;
        break;
    case 0x40:
        temp_v0_2 = *(u16 *)((unsigned char *)temp_a0 + (4));
        if (temp_v0_2 & 0x8000) {
            sp3C = var_v1;
            func_80133AD0(0x615);
            *(u16 *)((unsigned char *)var_v1 + (28)) = (u16) (*(u16 *)((unsigned char *)var_v1 + (28)) - 1);
            if ((s16) *(u16 *)((unsigned char *)var_v1 + (28)) < 0) {
                *(u16 *)((unsigned char *)var_v1 + (28)) = 0x2FU;
            }
            goto block_11;
        }
        if (temp_v0_2 & 0x2000) {
            sp3C = var_v1;
            func_80133AD0(0x615);
            temp_t4 = *(u16 *)((unsigned char *)var_v1 + (28)) + 1;
            *(u16 *)((unsigned char *)var_v1 + (28)) = temp_t4;
            if ((u32) (temp_t4 & 0xFFFF) >= 0x30U) {
                *(u16 *)((unsigned char *)var_v1 + (28)) = 0U;
            }
block_11:
            *(s16 *)((unsigned char *)var_v1 + (16)) = 0;
        }
        temp_v0_3 = (*(s32 *)sp30);
        if (temp_v0_3 & 0x8000) {
            temp_t1 = *(u16 *)((unsigned char *)var_v1 + (16)) + 1;
            *(u16 *)((unsigned char *)var_v1 + (16)) = temp_t1;
            if ((u32) (temp_t1 & 0xFFFF) >= 0x1FU) {
                sp3C = var_v1;
                func_80133AD0(0x615);
                *(u16 *)((unsigned char *)var_v1 + (28)) = (u16) (*(u16 *)((unsigned char *)var_v1 + (28)) - 1);
                *(s16 *)((unsigned char *)var_v1 + (16)) = 0x1A;
                if ((s16) *(u16 *)((unsigned char *)var_v1 + (28)) < 0) {
                    *(u16 *)((unsigned char *)var_v1 + (28)) = 0x2FU;
                }
            }
        } else if (temp_v0_3 & 0x2000) {
            temp_t0 = *(u16 *)((unsigned char *)var_v1 + (16)) + 1;
            *(u16 *)((unsigned char *)var_v1 + (16)) = temp_t0;
            if ((u32) (temp_t0 & 0xFFFF) >= 0x1FU) {
                sp3C = var_v1;
                func_80133AD0(0x615);
                *(s16 *)((unsigned char *)var_v1 + (16)) = 0x1A;
                temp_t6 = *(u16 *)((unsigned char *)var_v1 + (28)) + 1;
                *(u16 *)((unsigned char *)var_v1 + (28)) = temp_t6;
                if ((u32) (temp_t6 & 0xFFFF) >= 0x30U) {
                    *(u16 *)((unsigned char *)var_v1 + (28)) = 0U;
                }
            }
        }
        var_a0 = *(u16 *)((unsigned char *)sp30 + (4));
        if (var_a0 & 4) {
            var_v0 = *(u16 *)((unsigned char *)var_v1 + (26));
            if (var_v0 != 0) {
                sp3C = var_v1;
                func_80133AD0(0x617);
                temp_t9 = *(u16 *)((unsigned char *)var_v1 + (26)) - 1;
                var_v0 = temp_t9 & 0xFFFF;
                *(u16 *)((unsigned char *)var_v1 + (26)) = temp_t9;
            }
            *(s16 *)((unsigned char *)var_v1 + (28)) = (s16) *(u8 *)((unsigned char *)(sp44 + (*(u16 *)((unsigned char *)var_v1 + (24)) * 6) + var_v0) + (50));
            var_a0 = *(u16 *)((unsigned char *)sp30 + (4));
            goto block_30;
        }
        var_t5 = var_a0 & 0x20;
        if (var_a0 & 8) {
            var_v0_2 = *(u16 *)((unsigned char *)var_v1 + (26));
            if (var_v0_2 < 2U) {
                sp3C = var_v1;
                func_80133AD0(0x617);
                temp_t9_2 = *(u16 *)((unsigned char *)var_v1 + (26)) + 1;
                var_v0_2 = temp_t9_2 & 0xFFFF;
                *(u16 *)((unsigned char *)var_v1 + (26)) = temp_t9_2;
            }
            *(s16 *)((unsigned char *)var_v1 + (28)) = (s16) *(u8 *)((unsigned char *)(sp44 + (*(u16 *)((unsigned char *)var_v1 + (24)) * 6) + var_v0_2) + (50));
            var_a0 = *(u16 *)((unsigned char *)sp30 + (4));
block_30:
            var_t5 = var_a0 & 0x20;
        }
        var_t6 = var_a0 & 0x40;
        if (var_t5 != 0) {
            sp3C = var_v1;
            func_80133AD0(0x616);
            temp_v0_4 = *(u16 *)((unsigned char *)var_v1 + (26));
            if (temp_v0_4 < 2U) {
                *(u16 *)((unsigned char *)var_v1 + (26)) = (u16) (temp_v0_4 + 1);
                *(s16 *)((unsigned char *)var_v1 + (28)) = 0xA;
                var_a0 = *(u16 *)((unsigned char *)sp30 + (4));
            } else {
                *(s8 *)((unsigned char *)sp40 + (10)) = 1;
                var_a0 = *(u16 *)((unsigned char *)sp30 + (4));
            }
            var_t6 = var_a0 & 0x40;
        }
        var_t3 = var_a0 & 0x800;
        if (var_t6 != 0) {
            var_t3 = var_a0 & 0x800;
            if (*(u16 *)((unsigned char *)var_v1 + (26)) != 0) {
                sp3C = var_v1;
                func_80133AD0(0x617);
                *(s8 *)((unsigned char *)(sp44 + (*(u16 *)((unsigned char *)var_v1 + (24)) * 6) + *(u16 *)((unsigned char *)var_v1 + (26))) + (50)) = 0x3F;
                temp_t6_2 = *(u16 *)((unsigned char *)var_v1 + (26)) - 1;
                *(u16 *)((unsigned char *)var_v1 + (26)) = temp_t6_2;
                *(s16 *)((unsigned char *)var_v1 + (28)) = (s16) *(u8 *)((unsigned char *)(sp44 + (*(u16 *)((unsigned char *)var_v1 + (24)) * 6) + (temp_t6_2 & 0xFFFF)) + (50));
                var_t3 = *(u16 *)((unsigned char *)sp30 + (4)) & 0x800;
            }
        }
        if (var_t3 != 0) {
            sp3C = var_v1;
            func_80133AD0(0x616);
            *(s8 *)((unsigned char *)sp40 + (10)) = 1;
        }
        *(s8 *)((unsigned char *)(sp44 + (*(u16 *)((unsigned char *)var_v1 + (24)) * 6) + *(u16 *)((unsigned char *)var_v1 + (26))) + (50)) = (s8) *(u16 *)((unsigned char *)var_v1 + (28));
        break;
    }
    temp_v0_5 = *(u16 *)((unsigned char *)var_v1 + (24));
    switch (temp_v0_5) {
    case 0:
        sp2B = 2;
        break;
    case 1:
        sp2B = 6;
        break;
    case 2:
        sp2B = 4;
        break;
    }
    var_v0_3 = 0xF;
    if (*(u16 *)((unsigned char *)sp44 + (10)) & 3) {
        var_v0_3 = sp2B;
    }
    *(u8 *)((unsigned char *)sp38 + (42)) = var_v0_3;
    *(u8 *)((unsigned char *)sp38 + (22)) = var_v0_3;
    *(u8 *)((unsigned char *)sp38 + (2)) = var_v0_3;
    temp_a2 = *(u16 *)((unsigned char *)var_v1 + (24));
    sp3C = var_v1;
    func_801486D8((s32) (sp44 + (temp_a2 * 6) + 0x32), temp_a2 + 0x2B);
    *(s16 *)((unsigned char *)sp34 + (20)) = (s16) ((*(u16 *)((unsigned char *)var_v1 + (26)) * 8) + 0x34);
    *(s16 *)((unsigned char *)sp34 + (22)) = (s16) ((*(u16 *)((unsigned char *)var_v1 + (24)) * 0x10) + 0x7E);
    (*(s32 *)sp34) = ((u16) *(u16 *)((unsigned char *)sp44 + (10)) >> 3) & 1;
}

void func_8014687C(void) {
    u8 *p;
    u8 *q;
    s32 i;

    p = D_80158A64;
    if (p[0x1D] == 0x80U) {
        i = 0;
        q = D_80158AA8;
        for (; i < 0x80; i++) {
            q += 0x14;
            *(s16 *)(q - 0xE) = 8;
        }
        q = D_80158AA4;
        for (i = 0; i < 0x80; i++) {
            *(s16 *)(q + 0x44) = 8;
            q += 0x64;
        }
    }
    p[0x1D] -= 4;
    p[0x1E] = p[0x1F] = p[0x1D];
    if (p[0x1D] == 0) {
        p[9] = 1;
    }
    func_80146224();
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80145E60", func_80146924);
