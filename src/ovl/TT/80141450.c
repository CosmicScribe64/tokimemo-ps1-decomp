#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80141450", func_80141450);

/* The two stores of the 0x2C packet share one line (as1 keeps the li order). */
void func_80141A1C(TtOtTag *arg0, u8 *arg1, s32 arg2) {
    s32 pad; /* FAKE: unused home keeps the 72-byte frame and the spill slot at sp+0x3C. T-11000 */
    TtDrawBuffer80B8 *buffer;
    DR_MODE *m;
    buffer = (TtDrawBuffer80B8 *)(D_80158AB4 + ((TtDrawSelect0C *)D_80158A68)->index * 0x80B8);
    if (arg1[0x2C] == 0 && arg1[0x2B] == 0 && arg1[0x28] == 0) {
        ((TtOtTag *)arg1)->len = 3;
        arg1[7] = 0x74;
        if (arg1[0x29] != 0) {
            arg1[7] |= 2;
        } else {
            arg1[7] &= ~2;
        }
        if (arg1[0x2A] != 0) {
            arg1[7] |= 1;
        } else {
            arg1[7] &= ~1;
        }
        ((TtOtTag *)arg1)->addr = arg0->addr;
        arg0->addr = (u32)arg1;
        m = buffer->cursor;
        buffer->cursor++;
        func_8009D294((s32)m, 0, 0, *(u16 *)(arg1 + 0x16), 0);
        ((TtOtTag *)m)->addr = arg0->addr;
        arg0->addr = (u32)m;
        return;
    }
    ((TtOtTag *)arg1)->len = 9; arg1[7] = 0x2C;
    if (arg1[0x29] != 0) {
        arg1[7] |= 2;
    } else {
        arg1[7] &= ~2;
    }
    if (arg1[0x2A] != 0) {
        arg1[7] |= 1;
    } else {
        arg1[7] &= ~1;
    }
    *(s16 *)(arg1 + 0x10) = 8;
    *(s16 *)(arg1 + 0x12) = 8;
    func_80141450(arg1, arg2);
    ((TtOtTag *)arg1)->addr = arg0->addr;
    arg0->addr = (u32)arg1;
}
void func_80141C6C(TtOtTag *arg0, u8 *arg1, s32 arg2) {
    s32 pad; /* FAKE: unused home keeps the 72-byte frame and the spill slot at sp+0x3C. T-11500 */
    TtDrawBuffer80B8 *buffer;
    DR_MODE *m;
    buffer = (TtDrawBuffer80B8 *)(D_80158AB4 + ((TtDrawSelect0C *)D_80158A68)->index * 0x80B8);
    if (arg1[0x2C] == 0 && arg1[0x2B] == 0 && arg1[0x28] == 0) {
        ((TtOtTag *)arg1)->len = 3;
        arg1[7] = 0x7C;
        if (arg1[0x29] != 0) {
            arg1[7] |= 2;
        } else {
            arg1[7] &= ~2;
        }
        if (arg1[0x2A] != 0) {
            arg1[7] |= 1;
        } else {
            arg1[7] &= ~1;
        }
        ((TtOtTag *)arg1)->addr = arg0->addr;
        arg0->addr = (u32)arg1;
        m = buffer->cursor;
        buffer->cursor++;
        func_8009D294((s32)m, 0, 0, *(u16 *)(arg1 + 0x16), 0);
        ((TtOtTag *)m)->addr = arg0->addr;
        arg0->addr = (u32)m;
        return;
    }
    ((TtOtTag *)arg1)->len = 9; arg1[7] = 0x2C;
    if (arg1[0x29] != 0) {
        arg1[7] |= 2;
    } else {
        arg1[7] &= ~2;
    }
    if (arg1[0x2A] != 0) {
        arg1[7] |= 1;
    } else {
        arg1[7] &= ~1;
    }
    *(s16 *)(arg1 + 0x10) = 0x10;
    *(s16 *)(arg1 + 0x12) = 0x10;
    func_80141450(arg1, arg2);
    ((TtOtTag *)arg1)->addr = arg0->addr;
    arg0->addr = (u32)arg1;
}

void func_80141EBC(TtOtTag *arg0, u8 *arg1, s32 arg2) {
    s32 pad; /* FAKE: unused home keeps the 72-byte frame and the spill slot at sp+0x3C. T-11500 */
    TtDrawBuffer80B8 *buffer;
    DR_MODE *m;
    buffer = (TtDrawBuffer80B8 *)(D_80158AB4 + ((TtDrawSelect0C *)D_80158A68)->index * 0x80B8);
    if (arg1[0x2C] == 0 && arg1[0x2B] == 0 && arg1[0x28] == 0) {
        ((TtOtTag *)arg1)->len = 4;
        arg1[7] = 0x64;
        if (arg1[0x29] != 0) {
            arg1[7] |= 2;
        } else {
            arg1[7] &= ~2;
        }
        if (arg1[0x2A] != 0) {
            arg1[7] |= 1;
        } else {
            arg1[7] &= ~1;
        }
        ((TtOtTag *)arg1)->addr = arg0->addr;
        arg0->addr = (u32)arg1;
        m = buffer->cursor;
        buffer->cursor++;
        func_8009D294((s32)m, 0, 0, *(u16 *)(arg1 + 0x16), 0);
        ((TtOtTag *)m)->addr = arg0->addr;
        arg0->addr = (u32)m;
        return;
    }
    ((TtOtTag *)arg1)->len = 9; arg1[7] = 0x2C;
    if (arg1[0x29] != 0) {
        arg1[7] |= 2;
    } else {
        arg1[7] &= ~2;
    }
    if (arg1[0x2A] != 0) {
        arg1[7] |= 1;
    } else {
        arg1[7] &= ~1;
    }
    func_80141450(arg1, arg2);
    ((TtOtTag *)arg1)->addr = arg0->addr;
    arg0->addr = (u32)arg1;
}
