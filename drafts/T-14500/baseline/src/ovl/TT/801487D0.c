#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801487D0", func_801487D0);

/* The number printers below and their callee are old-style (K&R) definitions: the s16
 * parameters are converted in place at entry, a u8 parameter that is assigned stays in its
 * home slot (lbu/sb 0x54(sp)), and the locals declared before the buffer reserve the slots
 * above it (T-9210). */
void func_80148974(arg0, arg1, arg2)
    s16 arg0;
    s16 arg1;
    u8 *arg2;
{
    while (*arg2 != 0) {
        func_801487D0(arg0, arg1, *arg2++);
        arg0 += 6;
    }
}

void func_80148A04(arg0, arg1, arg2, arg3)
    s16 arg0;
    s16 arg1;
    s32 arg2;
    u8 arg3;
{
    u8 *p;
    s32 i;
    s32 m;
    u8 buf[12];

    m = 1;
    if (arg3 == 0 || arg3 >= 0xB) {
        arg3 = 0xA;
    }
    for (i = 1; i < arg3; i++) {
        m *= 10;
    }
    if (arg2 < 0) {
        buf[0] = '-';
        p = buf + 1;
        arg2 = -arg2;
    } else {
        buf[0] = '+';
        p = buf + 1;
    }
    arg2 %= m * 10;
    for (i = 0; i < arg3; i++) {
        *p++ = arg2 / m + '0';
        arg2 %= m;
        m /= 10;
    }
    *p = 0;
    func_80148974(arg0, arg1, buf);
}

void func_80148B6C(arg0, arg1, arg2, arg3)
    s16 arg0;
    s16 arg1;
    u32 arg2;
    u8 arg3;
{
    u8 *p;
    s32 i;
    u32 d;
    u8 buf[12];

    if (arg3 == 0 || arg3 >= 9) {
        arg3 = 8;
    }
    p = buf;
    for (i = arg3 - 1; i >= 0; i--) {
        d = (arg2 >> (i * 4)) & 0xF;
        if (d < 10) {
            *p = d + 0x30;
        } else {
            *p = d + 0x37;
        }
        p++;
    }
    *p = 0;
    func_80148974(arg0, arg1, buf);
}

void func_80148C04(arg0, arg1, arg2, arg3)
    s16 arg0;
    s16 arg1;
    u32 arg2;
    u8 arg3;
{
    u8 *p;
    s32 i;
    s32 b;
    u8 buf[36];

    p = buf;
    if (--arg3 == 0 || arg3 >= 0x20) {
        arg3 = 0x1F;
    }
    for (i = arg3; i >= 0; i--) {
        if ((arg2 >> i) & 1) {
            b = 1;
        } else {
            b = 0;
        }
        *p = b + 0x30;
        p++;
    }
    *p = 0;
    func_80148974(arg0, arg1, buf);
}

void func_80148C94(arg0, arg1, arg2, arg3)
    s16 arg0;
    s16 arg1;
    u32 arg2;
    u8 arg3;
{
    u8 *p;
    s32 i;
    s32 m;
    u8 buf[12];

    m = 1;
    p = buf;
    if (arg3 == 0 || arg3 >= 0xB) {
        arg3 = 0xA;
    }
    for (i = 1; i < arg3; i++) {
        m *= 10;
    }
    arg2 %= m * 10;
    for (i = 0; i < arg3; i++) {
        *p++ = arg2 / m + '0';
        arg2 %= m;
        m /= 10;
    }
    *p = 0;
    func_80148974(arg0, arg1, buf);
}
