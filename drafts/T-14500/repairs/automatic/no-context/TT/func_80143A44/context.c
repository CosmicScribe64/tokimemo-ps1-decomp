#include "common.h"
#include "ovl/TT.h"

void func_80142D40(u8 *arg0, s16 arg1, s16 arg2) {
    RECT r;
    s32 i;

    /* FAKE: w and h on one source line; IDO then stores h before w (line-based scheduling). T-6070 */
    r.w = 0x100; r.h = 0x10;
    r.x = arg1;
    r.y = arg2;
    for (i = 0; i < 0x2000; i += 0x20) {
        *(s16 *)(arg0 + i) = 0;
    }
    func_8009C884(&r, arg0);
}

void func_80142DA8(void *arg0, s16 arg1, s16 arg2) {
    RECT r;

    /* FAKE: w and h on one source line; IDO then stores h before w (line-based scheduling). T-6070 */
    r.w = 0x20; r.h = 0x80;
    r.x = arg1;
    r.y = arg2;
    func_8009C884(&r, arg0);
}

void func_80142DEC(s32 arg0, s32 arg1, s32 arg2) {
    u8 pos[4];
    s32 sec;
    s32 min;
    s32 frame;

    frame = arg1 % 75;
    sec = arg1 / 75;
    pos[2] = (frame / 10) * 16 + frame % 10;
    min = sec / 60;
    sec = sec % 60;
    pos[0] = (min / 10) * 16 + min % 10;
    pos[1] = (sec / 10) * 16 + sec % 10;
    while (func_800879D0(2, pos, 0) == 0) {
    }
    while (func_80087954(1, 0) != 2) {
    }
    func_80087E6C((arg2 + 0x7FF) / 0x800, arg0, 0x80);
    while (func_80087E4C(1, 0) > 0) {
    }
}

void func_8014304C(s32 arg0, s32 arg1, s32 arg2) {
    D_80156784 = D_80156780;
    D_80156780 = func_800460DC();
    func_80045414(9, 0, 0);
    func_800462C8((arg2 + 0x7FF) / 2048, arg0, arg1);
}

u8 func_801430C4(void) {
    return D_80156784;
}

void func_801430D4(void) {
    RECT r;
    s32 pad[2]; /* FAKE: unused home below r puts the RECT at sp+0x30 in the 56-byte frame. T-11520 */

    func_80142DEC(0x80162000, 0xBECA, 0x54900);
    func_80142D40((u8 *)0x80162000, 0, 0xF0);
    func_80142D40((u8 *)0x80164000, 0x100, 0xF0);
    func_80142DA8((void *)0x80166000, 0x280, 0);
    func_80142DA8((void *)0x80168000, 0x280, 0x80);
    func_80142DA8((void *)0x8016A000, 0x2A0, 0);
    func_80142DA8((void *)0x8016C000, 0x2A0, 0x80);
    func_80142DA8((void *)0x8016E000, 0x2C0, 0);
    func_80142DA8((void *)0x80170000, 0x2C0, 0x80);
    func_80142DA8((void *)0x80172000, 0x2E0, 0);
    func_80142DA8((void *)0x80174000, 0x380, 0);
    func_80142DA8((void *)0x80176000, 0x380, 0x80);
    func_80142DA8((void *)0x80178000, 0, 0x100);
    func_80142DA8((void *)0x8017A000, 0, 0x180);
    func_80142DA8((void *)0x8017C000, 0x20, 0x100);
    func_80142DA8((void *)0x8017E000, 0x20, 0x180);
    func_80142DA8((void *)0x80180000, 0x40, 0x100);
    func_80142DA8((void *)0x80182000, 0x40, 0x180);
    func_80142DA8((void *)0x80184000, 0x60, 0x100);
    func_80142DA8((void *)0x80186000, 0x60, 0x180);
    func_80142DA8((void *)0x80188000, 0x80, 0x100);
    func_80142DA8((void *)0x8018A000, 0x80, 0x180);
    func_80142DA8((void *)0x8018C000, 0xA0, 0x100);
    func_80142DA8((void *)0x8018E000, 0xA0, 0x180);
    func_80142DA8((void *)0x80190000, 0xC0, 0x100);
    func_80142DA8((void *)0x80192000, 0xC0, 0x180);
    func_80142DA8((void *)0x80194000, 0xE0, 0x100);
    func_80142DA8((void *)0x80196000, 0xE0, 0x180);
    func_80142DA8((void *)0x80198000, 0x100, 0x100);
    func_80142DA8((void *)0x8019A000, 0x100, 0x180);
    func_80142DA8((void *)0x8019C000, 0x120, 0x100);
    func_80142DA8((void *)0x8019E000, 0x120, 0x180);
    func_80142DA8((void *)0x801A0000, 0x140, 0x100);
    func_80142DA8((void *)0x801A2000, 0x140, 0x180);
    func_80142DA8((void *)0x801A4000, 0x200, 0x100);
    func_80142DA8((void *)0x801A6000, 0x200, 0x180);
    func_80142DA8((void *)0x801A8000, 0x260, 0x100);
    func_80142DA8((void *)0x801AA000, 0x260, 0x180);
    func_80142DA8((void *)0x801AC000, 0x280, 0x100);
    func_80142DA8((void *)0x801AE000, 0x280, 0x180);
    func_80142DA8((void *)0x801B0000, 0x2A0, 0x100);
    func_80142DA8((void *)0x801B2000, 0x240, 0x100);
    func_80142DA8((void *)0x801B4000, 0x240, 0x180);
    /* The four RECT fields on one line: IDO then stores h, w, y, x (line-based scheduling). T-11520 */
    r.x = 0x3C0; r.y = 0x180; r.w = 0x18; r.h = 0x30;
    func_8009C884(&r, (void *)0x801B6000);
    r.x = 0x3C0; r.y = 0x1FF; r.w = 0x10; r.h = 1;
    func_8009C884(&r, D_80156788);
    func_8009C674(0);
    func_80142DEC(0x80010000, 0xC08B, 0xE20);
    func_80142DEC(0x80162000, 0xC06B, 0xFED0);
    func_80142DEC(0x80012000, 0xD6D4, 0x2420);
    func_80142DEC(0x8001C000, 0xD678, 0x69E4);
    func_80142DEC(0x80172000, 0xD685, 0x27D60);
    func_80142DEC(0x80016000, 0xC8A8, 0x2420);
    func_80142DEC(0x8002E800, 0xC857, 0x69E4);
    func_80142DEC(0x8019A000, 0xC85B, 0x27D60);
    func_80142DEC(0x8001A000, 0xC067, 0xE20);
    func_80142DEC(0x801C2000, 0xC032, 0x1A680);
}

void func_80143570(void) {
    *D_80158A74 = 0;
}

void func_80143580(void) {
    s32 i;
    u8 *p;
    u8 *q;
    s8 v;

    q = D_80158A74;
    p = D_80158A78;
    v = 8;
    if (D_80158A64[0x22] != 0) {
        switch (D_80158A60[0x52]) {
        default:
            v = 8;
            q[0x53] = 0;
            q[0x54] = 4;
            q[0x55] = v;
            p[0x92] = v;
            break;
        case 1:
            v = 8;
            q[0x53] = 0;
            q[0x54] = 4;
            q[0x55] = v;
            p[0x92] = v;
            break;
        case 2:
            v = 9;
            q[0x53] = 0;
            q[0x54] = 4;
            q[0x55] = v;
            p[0x92] = v;
            break;
        case 3:
            v = 9;
            q[0x53] = 0;
            q[0x54] = 4;
            q[0x55] = v;
            p[0x92] = v;
            break;
        case 4:
            q[0x53] = 1;
            q[0x54] = 5;
            q[0x55] = 0xA;
            p[0x92] = 9;
            for (i = 0; i != 3; i++) {
                func_80147FD0(p);
                p += 0xA4;
            }
            break;
        }
    } else {
        q[0x53] = 0;
        q[0x54] = 4;
        q[0x55] = 8;
        p[0x92] = v;
    }
}

#ifdef NON_MATCHING
/* NON_MATCHING: T-10320, T-11620, constants 2 and D_80151A50 sit in $a1/$a2, the original keeps them in $t0/$t1 (26 diff lines; a counter loop gives the 5/15 order but not the registers, T-11620) */
void func_801436BC(void) {
    u8 *p;
    u8 *q;
    s32 n;

    p = D_80158A74;
    *p = 0;
    *(s16 *)(p + 2) = 0;
    p[12] = 0;
    *(s16 *)(p + 4) = 0;
    p[13] = 0;
    p[14] = 0;
    p[15] = 0;
    *(s16 *)(p + 6) = 0;
    *(s16 *)(p + 8) = 0;
    *(s16 *)(p + 10) = 0;
    *(s16 *)(p + 16) = 2;
    *(u8 **)(p + 56) = D_80151A50;
    p[60] = 0;
    p[61] = 0;
    p[62] = 0;
    p[64] = 0x60;
    p[72] = 0;
    func_80143580();
    p[87] = 0;
    p[88] = 0;
    p[89] = 0;
    p[90] = 0;
    p[91] = 0;
    p[92] = 0;
    p[93] = 0;
    p[94] = 0;
    p[95] = 0;
    p[96] = 0;
    *(s16 *)(p + 4) = 0x20;
    *(s16 *)(p + 10) = 0;
    *(s16 *)(p + 8) = 0;
    *(s16 *)(p + 6) = 0;
    p[15] = 0;
    p[14] = 0;
    p[13] = 0;
    q = D_80158A7C;
    n = 5;
    do {
        q[62] = n;
        n += 5;
        q += 0x60;
        q[-96] = 0;
        *(s16 *)(q - 94) = 0;
        *(s16 *)(q - 92) = 0;
        q[-84] = 0;
        *(s16 *)(q - 86) = 0;
        *(s16 *)(q - 88) = 0;
        *(s16 *)(q - 90) = 0;
        q[-81] = 0;
        q[-82] = 0;
        q[-83] = 0;
        *(s16 *)(q - 80) = 2;
        *(u8 **)(q - 40) = D_80151A50;
        q[-36] = 0;
        q[-35] = 0;
        q[-32] = 0x60;
    } while (n != 0xF);
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_801436BC);
#endif

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_801437F0);

void func_80143970(void) {
}

void func_80143978(void) {
    u8 *p = D_80158A74;
    u8 *q = D_80158A6C;

    switch (*(u16 *)(p + 6)) {
    case 0:
        p[0x52] = 1;
        *(u16 *)(p + 0x50) |= 2;
        p[0x56] = 1;
        p[0x5A] = 0;
        p[0x5B] = 0;
        *(u16 *)(p + 6) = 0x40;
        break;
    case 0x40:
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0xFFFC0000;
        if (*(s32 *)(p + 0x24) < (s32)0xFFF00000) {
            *(s32 *)(p + 0x2C) = 0;
            *(u16 *)(p + 6) = 0x50;
            *(u16 *)(q + 0x10) = 0;
        }
        break;
    case 0x50:
        if ((u32)*(u16 *)(D_80158A6C + 0x10) >= 0x5B) {
            *(u16 *)(p + 6) = 0;
            p[0xD] = 1;
        }
        *(u16 *)(q + 0x10) += 1;
        break;
    }
}

/* bulk_m2c declarations (T-11190) */
s32 func_80133AD0();
/* end bulk_m2c declarations */
void func_80143A44(void) {
    void *sp3C;
    void *sp38;
    void *sp30;
    void *sp2C;
    void *sp28;
    s32 temp_a0;
    s32 temp_t6;
    s32 temp_t9;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a0_4;
    s32 var_a0_5;
    s32 var_a0_6;
    s32 var_a3;
    s32 var_at;
    s32 var_t4;
    u16 temp_v0_6;
    u8 temp_t5;
    u8 temp_v0_2;
    u8 temp_v0_3;
    u8 temp_v0_4;
    u8 temp_v0_5;
    u8 var_v0_6;
    u8 *temp_v0;
    u8 *var_t0;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 *var_v0_3;
    u8 *var_v0_4;
    u8 *var_v0_5;
    u8 *var_v0_7;
    u8 *var_v1;
    u8 *var_v1_2;

    var_t0 = D_80158A74;
    temp_v0 = D_80158A60 + 0x20;
    if (*(u16 *)((unsigned char *)D_80158A60 + (32)) & 0x8000) {
        temp_v0_2 = *(u8 *)((unsigned char *)var_t0 + (88));
        *(s32 *)((unsigned char *)var_t0 + (40)) = -1;
        *(u8 *)((unsigned char *)var_t0 + (89)) = 0U;
        if (temp_v0_2 < 8U) {
            *(u8 *)((unsigned char *)var_t0 + (88)) = (u8) (temp_v0_2 + 1);
        }
    } else if (*(u16 *)((unsigned char *)D_80158A60 + (32)) & 0x2000) {
        temp_v0_3 = *(u8 *)((unsigned char *)var_t0 + (89));
        *(s32 *)((unsigned char *)var_t0 + (40)) = 1;
        *(u8 *)((unsigned char *)var_t0 + (88)) = 0U;
        if (temp_v0_3 < 8U) {
            *(u8 *)((unsigned char *)var_t0 + (89)) = (u8) (temp_v0_3 + 1);
        }
    } else {
        temp_v0_4 = *(u8 *)((unsigned char *)var_t0 + (88));
        *(s32 *)((unsigned char *)var_t0 + (40)) = 0;
        if (temp_v0_4 != 0) {
            *(u8 *)((unsigned char *)var_t0 + (88)) = (u8) (temp_v0_4 - 1);
        }
        temp_v0_5 = *(u8 *)((unsigned char *)var_t0 + (89));
        if (temp_v0_5 != 0) {
            *(u8 *)((unsigned char *)var_t0 + (89)) = (u8) (temp_v0_5 - 1);
        }
    }
    temp_v0_6 = *(u16 *)((unsigned char *)D_80158A60 + (32));
    if (temp_v0_6 & 0x1000) {
        *(s32 *)((unsigned char *)var_t0 + (44)) = -1;
    } else if (temp_v0_6 & 0x4000) {
        *(s32 *)((unsigned char *)var_t0 + (44)) = 1;
    } else {
        *(s32 *)((unsigned char *)var_t0 + (44)) = 0;
    }
    temp_v1 = *(s32 *)((unsigned char *)var_t0 + (40));
    if ((temp_v1 != 0) && (temp_a0 = *(s32 *)((unsigned char *)var_t0 + (44)), (temp_a0 != 0))) {
        temp_t6 = *(u8 *)((unsigned char *)var_t0 + (84)) * 0x6000;
        *(s32 *)((unsigned char *)var_t0 + (40)) = (s32) (temp_v1 * temp_t6);
        *(s32 *)((unsigned char *)var_t0 + (44)) = (s32) (temp_a0 * temp_t6);
    } else {
        temp_t9 = *(u8 *)((unsigned char *)var_t0 + (84)) << 0xF;
        *(s32 *)((unsigned char *)var_t0 + (40)) = (s32) (temp_v1 * temp_t9);
        *(s32 *)((unsigned char *)var_t0 + (44)) = (s32) (*(s32 *)((unsigned char *)var_t0 + (44)) * temp_t9);
    }
    if (*(u16 *)((unsigned char *)temp_v0 + (4)) & 1) {
        var_v0 = D_80158A80;
        var_a0 = 0;
loop_21:
        var_a0 += 1;
        if (*(u8 *)((unsigned char *)var_t0 + (85)) == 0xA) {
            var_at = var_a0 < 2;
            if (((*(s32 *)var_v0) == 0) && (var_at = var_a0 < 2, (*(u8 *)((unsigned char *)var_v0 + (168)) == 0)) && (var_at = var_a0 < 2, (*(u8 *)((unsigned char *)var_v0 + (336)) == 0))) {
                sp30 = var_v0;
                sp3C = temp_v0;
                sp38 = var_t0;
                func_80133AD0(0x608, 0xA, temp_v0);
                (*(s32 *)var_v0) = 1U;
                *(s16 *)((unsigned char *)var_v0 + (4)) = 0x20;
                *(s8 *)((unsigned char *)var_v0 + (13)) = 0;
                *(s8 *)((unsigned char *)var_v0 + (14)) = 0;
                *(s8 *)((unsigned char *)var_v0 + (15)) = 0;
                *(s16 *)((unsigned char *)var_v0 + (6)) = 0;
                *(s16 *)((unsigned char *)var_v0 + (8)) = 0;
                *(s16 *)((unsigned char *)var_v0 + (10)) = 0;
                *(u8 *)((unsigned char *)var_v0 + (168)) = 1U;
                *(s16 *)((unsigned char *)var_v0 + (172)) = 0x20;
                *(u8 *)((unsigned char *)var_v0 + (336)) = 1U;
                *(s16 *)((unsigned char *)var_v0 + (340)) = 0x20;
            } else {
                goto block_29;
            }
        } else {
            var_at = var_a0 < 2;
            if ((*(s32 *)var_v0) == 0) {
                sp30 = var_v0;
                sp3C = temp_v0;
                sp38 = var_t0;
                func_80133AD0(0x608, 0xA, temp_v0);
                (*(s32 *)var_v0) = 1U;
                *(s16 *)((unsigned char *)var_v0 + (4)) = 0x20;
                *(s8 *)((unsigned char *)var_v0 + (13)) = 0;
                *(s8 *)((unsigned char *)var_v0 + (14)) = 0;
                *(s8 *)((unsigned char *)var_v0 + (15)) = 0;
                *(s16 *)((unsigned char *)var_v0 + (6)) = 0;
                *(s16 *)((unsigned char *)var_v0 + (8)) = 0;
                *(s16 *)((unsigned char *)var_v0 + (10)) = 0;
            } else {
block_29:
                var_v0 += 0x54;
                if (var_at == 0) {

                } else {
                    goto loop_21;
                }
            }
        }
        if (*(u8 *)((unsigned char *)var_t0 + (83)) != 0) {
            var_a0_2 = 6;
            var_v0_2 = D_80158A80 + 0x1F8;
loop_33:
            var_a0_2 += 1;
            if ((*(s32 *)var_v0_2) == 0) {
                (*(s32 *)var_v0_2) = 1U;
                *(s16 *)((unsigned char *)var_v0_2 + (4)) = 0x20;
                *(s16 *)((unsigned char *)var_v0_2 + (10)) = 0;
                *(s16 *)((unsigned char *)var_v0_2 + (8)) = 0;
                *(s16 *)((unsigned char *)var_v0_2 + (6)) = 0;
                *(s8 *)((unsigned char *)var_v0_2 + (15)) = 0;
                *(s8 *)((unsigned char *)var_v0_2 + (14)) = 0;
                *(s8 *)((unsigned char *)var_v0_2 + (13)) = 0;
            } else {
                var_v0_2 += 0x54;
                if (var_a0_2 < 8) {
                    goto loop_33;
                }
            }
            var_a0_3 = 8;
            var_v0_3 = D_80158A80 + 0x2A0;
loop_37:
            var_a0_3 += 1;
            if ((*(s32 *)var_v0_3) == 0) {
                (*(s32 *)var_v0_3) = 1U;
                *(s16 *)((unsigned char *)var_v0_3 + (4)) = 0x20;
                *(s16 *)((unsigned char *)var_v0_3 + (10)) = 0;
                *(s16 *)((unsigned char *)var_v0_3 + (8)) = 0;
                *(s16 *)((unsigned char *)var_v0_3 + (6)) = 0;
                *(s8 *)((unsigned char *)var_v0_3 + (15)) = 0;
                *(s8 *)((unsigned char *)var_v0_3 + (14)) = 0;
                *(s8 *)((unsigned char *)var_v0_3 + (13)) = 0;
            } else {
                var_v0_3 += 0x54;
                if (var_a0_3 < 0xA) {
                    goto loop_37;
                }
            }
            var_a0_4 = 0xA;
            var_v0_4 = D_80158A80 + 0x348;
loop_41:
            var_a0_4 += 1;
            if ((*(s32 *)var_v0_4) == 0) {
                (*(s32 *)var_v0_4) = 1U;
                *(s16 *)((unsigned char *)var_v0_4 + (4)) = 0x20;
                *(s16 *)((unsigned char *)var_v0_4 + (10)) = 0;
                *(s16 *)((unsigned char *)var_v0_4 + (8)) = 0;
                *(s16 *)((unsigned char *)var_v0_4 + (6)) = 0;
                *(s8 *)((unsigned char *)var_v0_4 + (15)) = 0;
                *(s8 *)((unsigned char *)var_v0_4 + (14)) = 0;
                *(s8 *)((unsigned char *)var_v0_4 + (13)) = 0;
            } else {
                var_v0_4 += 0x54;
                if (var_a0_4 != 0xC) {
                    goto loop_41;
                }
            }
        }
    }
    if (*(u16 *)((unsigned char *)temp_v0 + (4)) & 2) {
        var_v1 = D_80158A84;
        var_v0_5 = D_80158A88;
        var_t4 = 0;
        var_a0_5 = 0;
loop_46:
        var_a0_5 += 1;
        if (((*(s32 *)var_v0_5) == 0) && ((*(s32 *)var_v1) == 0) && ((*(u8 *)((unsigned char *)var_t0 + (92)) == 0) || (*(u8 *)((unsigned char *)var_t0 + (93)) == 0))) {
            sp2C = var_v0_5;
            sp28 = var_v1;
            sp38 = var_t0;
            func_80133AD0(0x609);
            (*(s32 *)var_v0_5) = 1U;
            *(s16 *)((unsigned char *)var_v0_5 + (4)) = 0x20;
            *(s8 *)((unsigned char *)var_v0_5 + (13)) = 0;
            *(s8 *)((unsigned char *)var_v0_5 + (14)) = 0;
            *(s8 *)((unsigned char *)var_v0_5 + (15)) = 0;
            *(s16 *)((unsigned char *)var_v0_5 + (6)) = 0;
            *(s16 *)((unsigned char *)var_v0_5 + (8)) = 0;
            *(s16 *)((unsigned char *)var_v0_5 + (10)) = 0;
            (*(s32 *)var_v1) = 1U;
            *(s8 *)((unsigned char *)var_v1 + (1)) = 1;
            *(s16 *)((unsigned char *)var_v1 + (62)) = 1;
            *(s16 *)((unsigned char *)var_v1 + (4)) = 0x20;
            *(s8 *)((unsigned char *)var_v1 + (13)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (14)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (15)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (6)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (8)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (10)) = 0;
            *(u8 *)((unsigned char *)var_v1 + (61)) = (u8) *(u8 *)((unsigned char *)var_t0 + (96));
            var_v0_6 = *(u8 *)((unsigned char *)var_t0 + (83));
            var_t4 = 1 & 0xFF;
        } else {
            var_v0_5 += 0x58;
            var_v1 += 0x58;
            if (var_a0_5 == 2) {
                var_v0_6 = *(u8 *)((unsigned char *)var_t0 + (83));
            } else {
                goto loop_46;
            }
        }
        var_a3 = 0xB0;
        if (var_v0_6 != 0) {
            do {
                var_a0_6 = 0;
                var_v1_2 = var_a3 + D_80158A84;
                var_v0_7 = var_a3 + D_80158A88;
loop_56:
                var_a0_6 += 1;
                if (((*(s32 *)var_v0_7) == 0) && ((*(s32 *)var_v1_2) == 0) && ((*(u8 *)((unsigned char *)var_t0 + (92)) == 0) || (*(u8 *)((unsigned char *)var_t0 + (93)) == 0))) {
                    (*(s32 *)var_v0_7) = 1U;
                    *(s16 *)((unsigned char *)var_v0_7 + (4)) = 0x20;
                    *(s16 *)((unsigned char *)var_v0_7 + (10)) = 0;
                    *(s16 *)((unsigned char *)var_v0_7 + (8)) = 0;
                    *(s16 *)((unsigned char *)var_v0_7 + (6)) = 0;
                    *(s8 *)((unsigned char *)var_v0_7 + (15)) = 0;
                    *(s8 *)((unsigned char *)var_v0_7 + (14)) = 0;
                    *(s8 *)((unsigned char *)var_v0_7 + (13)) = 0;
                    (*(s32 *)var_v1_2) = 1U;
                    *(s8 *)((unsigned char *)var_v1_2 + (1)) = 1;
                    *(s16 *)((unsigned char *)var_v1_2 + (62)) = 1;
                    *(s16 *)((unsigned char *)var_v1_2 + (4)) = 0x20;
                    *(s8 *)((unsigned char *)var_v1_2 + (13)) = 0;
                    *(s8 *)((unsigned char *)var_v1_2 + (14)) = 0;
                    *(s8 *)((unsigned char *)var_v1_2 + (15)) = 0;
                    *(s16 *)((unsigned char *)var_v1_2 + (6)) = 0;
                    *(s16 *)((unsigned char *)var_v1_2 + (8)) = 0;
                    *(s16 *)((unsigned char *)var_v1_2 + (10)) = 0;
                    var_t4 = 1;
                    *(u8 *)((unsigned char *)var_v1_2 + (61)) = (u8) *(u8 *)((unsigned char *)var_t0 + (96));
                } else {
                    var_v0_7 += 0x58;
                    var_v1_2 += 0x58;
                    if (var_a0_6 != 2) {
                        goto loop_56;
                    }
                }
                var_a3 += 0xB0;
            } while (var_a3 != 0x2C0);
        }
        if (var_t4 != 0) {
            if (*(u8 *)((unsigned char *)var_t0 + (96)) != 0) {
                *(u8 *)((unsigned char *)var_t0 + (93)) = 1U;
            } else {
                *(u8 *)((unsigned char *)var_t0 + (92)) = 1U;
            }
            temp_t5 = *(u8 *)((unsigned char *)var_t0 + (96)) + 1;
            *(u8 *)((unsigned char *)var_t0 + (96)) = temp_t5;
            *(u8 *)((unsigned char *)var_t0 + (96)) = (u8) (temp_t5 & 1);
        }
    }
}

void func_80143FE8(void) {
    u8 *p = D_80158A74;
    s32 v;

    *(s32 *)(p + 0x20) += *(s32 *)(p + 0x28);
    *(s32 *)(p + 0x24) += *(s32 *)(p + 0x2C);
    if (p[0x56] == 0) {
        if (*(s32 *)(p + 0x20) < (s32)0xFF880000) {
            *(s32 *)(p + 0x20) = -0x780000;
        } else if (*(s32 *)(p + 0x20) >= 0x780001) {
            *(s32 *)(p + 0x20) = 0x780000;
        }
        v = *(s32 *)(p + 0x24);
        if (v < 0x380000) {
            *(s32 *)(p + 0x24) = 0x380000;
        } else if (v >= 0xE00001) {
            *(s32 *)(p + 0x24) = 0xE00000;
        }
    }
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
}

typedef struct {
    s32 w[9];
} TtBlk36; /* a 0x24-byte struct copy: IDO copies it in a 3-trip loop of 3 words */

void func_80144094(void) {
    s32 pad; /* FAKE: unused local declared first puts sp38/sp34 at the original slots; real source unknown. T-9340 */
    u8 *sp38;
    s32 *sp34;
    u8 *p;
    u8 *q;

    sp38 = D_80158A6C;
    sp34 = D_80158A70;
    p = D_80158A74;
    q = D_80158A7C;
    func_80133AD0(0x610);
    func_80133AD0(0x607);
    if (*(u16 *)((u8 *)sp34 + 2) < 0x401) {
        *(s32 *)(sp38 + 4) = 0x8000;
        *(s32 *)sp38 = 0x8000;
    }
    p[0x52] = 1;
    p[0x56] = 1;
    p[0x53] = 0;
    *(s32 *)(p + 0x28) = 0;
    *(s32 *)(p + 0x2C) = 0x10000;
    *(u8 **)(p + 0x38) = D_80151A50;
    p[0x3C] = 0;
    p[0x3D] = 1;
    p[0x3E] = 0;
    p[0x40] = 0x80;
    func_801478D0(p + 0x38);
    *(TtBlk36 *)(q + 0x14) = *(TtBlk36 *)(p + 0x14);
    *(s32 *)(q + 0x28) = 0;
    *(s32 *)(q + 0x2C) = 0;
    *(u8 **)(q + 0x38) = D_80151A50;
    q[0x3C] = 0;
    q[0x3D] = 2;
    q[0x3E] = 0;
    q[0x40] = 0x80;
    func_801478D0(q + 0x38);
    *(s32 *)(q + 0x98) = 0;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_801441BC);

/* FAKE: the first test reads D_80158A74 through an array view of the pointer block at D_80158A60, so uopt does not
 * merge it with p; the original loads p straight into s2 and reloads the global for the test (T-0018 promo). T-11620 */
void func_80144854(void) {
    u8 *p;
    u8 *q;
    u16 k;
    u8 n;

    p = D_80158A74;
    q = D_80158A7C;
    if (p[0] != 0) {
        if (!(*(u16 *)(((u8 **)&D_80158A60)[5] + 0x50) & 2) || !(*(u16 *)(D_80158A60 + 8) & 1)) {
            k = *(u16 *)(D_80158A74 + 4);
            if (k >= 0x60 && k < 0x64) {
                if (!(D_80158A74[0x40] & 1)) {
                    func_80147A18(p + 0x14, p + 0x38, *(u16 *)(p + 0x10));
                }
                if (!(q[0x40] & 1)) {
                    func_80147A18(q + 0x14, q + 0x38, *(u16 *)(p + 0x10));
                }
            } else {
                n = D_80158A74[0x57];
                if (n == 0) {
                    func_80147A18(p + 0x14, q + 0x38, *(u16 *)(q + 0x10));
                    q += 0x60;
                    func_80147A18(p + 0x14, q + 0x38, *(u16 *)(q + 0x10));
                    func_80147A18(p + 0x14, p + 0x38, *(u16 *)(p + 0x10));
                } else if (n < 3) {
                    func_80147A18(p + 0x14, q + 0x98, *(u16 *)(q + 0x10));
                    func_80147A18(p + 0x14, p + 0x38, *(u16 *)(p + 0x10));
                    func_80147A18(p + 0x14, q + 0x38, *(u16 *)(q + 0x10));
                } else {
                    func_80147A18(p + 0x14, q + 0x38, *(u16 *)(q + 0x10));
                    func_80147A18(p + 0x14, p + 0x38, *(u16 *)(p + 0x10));
                    func_80147A18(p + 0x14, q + 0x98, *(u16 *)(q + 0x10));
                }
            }
        }
    }
}

/* NON_MATCHING: T-11500, T-11620, 3 instructions (6 diff lines): the two stores of the constant 9 (sb 0x55 and sb 0x92 via D_80158A78) use $v0 here, the original holds 9 in $a0. T-11510: the 9 is a ugen temp (used twice), which takes the lowest free of v0/v1/a0, so the original has v0/v1 busy in that block for an unknown reason; the return type makes no difference (void == s32). */
#ifdef NON_MATCHING
s32 func_80144A04(u8 *arg0) {
    u8 *p;
    u8 *w;
    s32 i;
    u8 *q;

    p = D_80158A74;
    switch (*(u16 *)(arg0 + 4)) {
    case 0x40:
    case 0x60:
        switch (arg0[0x58]) {
        case 0:
            func_80133AD0(0x60A);
            arg0[0] = 0;
            break;
        case 1:
            func_80133AD0(0x605);
            func_8013BE08(arg0, 0x28);
            p[0x55] = 9;
            D_80158A78[0x92] = 9;
            *(s32 *)(arg0 + 0x34) = 0;
            *(s32 *)(arg0 + 0x2C) = 0;
            *(s32 *)(arg0 + 0x28) = 0;
            *(s16 *)(arg0 + 0x62) = 0;
            *(u16 *)(arg0 + 4) = 0x90;
            arg0[0xD] = 0;
            arg0[0xE] = 0;
            arg0[0xF] = 0;
            *(s16 *)(arg0 + 6) = 0;
            *(s16 *)(arg0 + 8) = 0;
            *(s16 *)(arg0 + 0xA) = 0;
            break;
        case 2:
            func_80133AD0(0x602);
            func_8013BE08(arg0, 0x29);
            p[0x54] += 1;
            *(s32 *)(arg0 + 0x34) = 0;
            *(s32 *)(arg0 + 0x2C) = 0;
            *(s32 *)(arg0 + 0x28) = 0;
            *(s16 *)(arg0 + 0x62) = 0;
            *(u16 *)(arg0 + 4) = 0x90;
            arg0[0xD] = 0;
            arg0[0xE] = 0;
            arg0[0xF] = 0;
            *(s16 *)(arg0 + 6) = 0;
            *(s16 *)(arg0 + 8) = 0;
            *(s16 *)(arg0 + 0xA) = 0;
            break;
        case 3:
            func_80133AD0(0x604);
            if (p[0x53] == 0) {
                func_8013BE08(arg0, 0x2A);
                p[0x53] = 1;
                q = D_80158A78;
                for (i = 0; i != 3; i++, q += 0xA4) {
                    func_80147FD0(q);
                }
            }
            *(s32 *)(arg0 + 0x34) = 0;
            *(s32 *)(arg0 + 0x2C) = 0;
            *(s32 *)(arg0 + 0x28) = 0;
            *(s16 *)(arg0 + 0x62) = 0;
            *(u16 *)(arg0 + 4) = 0x90;
            *(s16 *)(arg0 + 0xA) = 0;
            *(s16 *)(arg0 + 8) = 0;
            *(s16 *)(arg0 + 6) = 0;
            arg0[0xF] = 0;
            arg0[0xE] = 0;
            arg0[0xD] = 0;
            break;
        case 6:
            func_80133AD0(0x603);
            func_8013BE08(arg0, 0x2B);
            p[0x54] -= 1;
            if (p[0x54] == 0) {
                p[0x54] = 1;
            }
            *(s32 *)(arg0 + 0x34) = 0;
            *(s32 *)(arg0 + 0x2C) = 0;
            *(s32 *)(arg0 + 0x28) = 0;
            *(s16 *)(arg0 + 0x62) = 0;
            *(u16 *)(arg0 + 4) = 0x90;
            *(s16 *)(arg0 + 0xA) = 0;
            *(s16 *)(arg0 + 8) = 0;
            *(s16 *)(arg0 + 6) = 0;
            arg0[0xF] = 0;
            arg0[0xE] = 0;
            arg0[0xD] = 0;
            break;
        }
        break;
    case 0x20:
        break;
    case 0x70:
        if (D_80158A74[0x52] == 0) {
            w = D_80158A6C;
            if (*(u16 *)((u8 *)D_80158A70 + 2) < 0x401) {
                *(s32 *)(w + 4) = 0x8000;
                *(s32 *)w = 0x8000;
            }
            func_80144094();
            *(u16 *)(p + 4) = 0x60;
            *(s16 *)(p + 0xA) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[0xF] = 0;
            p[0xE] = 0;
            p[0xD] = 0;
            arg0[0] = 0;
        }
        break;
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_80144A04);
#endif

void func_80144CA8(void) {
    u8 *q;
    u8 *p;
    u32 i;

    q = D_80158A6C;
    for (i = 0, p = D_80158AA4 + 0x64; i < (u32)(2 - q[0xC]); i++) {
        *p = 0;
        p -= 0x64;
    }
}

void func_80144CF0(void) {
    u8 *a;
    u8 *p;
    s32 i;

    a = D_80158A74;
    p = D_80158A78;
    *(u16 *)(D_80158A6C + 8) = 0x8CA0;
    a[0x53] = 1;
    a[0x55] = 9;
    p[0x92] = 9;
    for (i = 0; i != 3; i++) {
        func_80147FD0(p);
        p += 0xA4;
    }
}
