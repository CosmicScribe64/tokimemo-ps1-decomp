#include "common.h"
#include "ovl/TT.h"

void func_80141450(void *arg0, void *arg1) {
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 sp48;
    s32 sp3C;
    s16 sp36;
    s16 sp34;
    s16 sp32;
    s16 sp30;
    s16 temp_a0;
    s16 temp_a0_2;
    s16 temp_a0_3;
    s16 temp_a0_4;
    s16 temp_a0_5;
    s16 temp_a0_6;
    s16 temp_a1;
    s16 temp_a1_2;
    s16 temp_a3;
    s16 temp_a3_2;
    s16 temp_a3_3;
    s16 temp_t0;
    s16 temp_t1;
    s16 temp_t2;
    s16 temp_v0_10;
    s16 temp_v0_7;
    s16 temp_v0_8;
    s16 temp_v0_9;
    s16 temp_v1_10;
    s16 temp_v1_11;
    s16 temp_v1_5;
    s16 temp_v1_6;
    s16 temp_v1_7;
    s16 temp_v1_8;
    s16 temp_v1_9;
    s16 var_t0;
    s16 var_t1;
    s32 temp_t3;
    s32 temp_t4;
    s32 temp_v0_11;
    s32 var_a0;
    s32 var_a1;
    s32 var_ra;
    s32 var_ra_2;
    s32 var_t5;
    u8 temp_v0;
    u8 temp_v0_2;
    u8 temp_v0_3;
    u8 temp_v0_4;
    u8 temp_v0_5;
    u8 temp_v0_6;
    u8 temp_v1;
    u8 temp_v1_2;
    u8 temp_v1_3;
    u8 temp_v1_4;

    var_t1 = *(s16 *)((unsigned char *)arg0 + (18));
    var_t0 = *(s16 *)((unsigned char *)arg0 + (16));
    if ((*(s32 *)arg1) >= 0x2000) {
        var_a0 = -1;
    } else {
        var_a0 = 0;
    }
    var_a1 = 0;
    if (*(s16 *)((unsigned char *)arg1 + (2)) >= 0x2000) {
        var_a1 = -1;
    }
    temp_v0 = *(u8 *)((unsigned char *)arg0 + (40));
    switch (temp_v0) {
    default:
        var_ra = sp48;
        var_t5 = sp4C;
        break;
    case 0:
        temp_v0_2 = *(u8 *)((unsigned char *)arg0 + (12));
        sp54 = (s32) temp_v0_2;
        temp_v1 = *(u8 *)((unsigned char *)arg0 + (13));
        var_t5 = temp_v0_2 + var_t0 + var_a0;
        var_ra = temp_v1 + var_t1 + var_a1;
        sp50 = (s32) temp_v1;
        break;
    case 1:
        temp_v0_3 = *(u8 *)((unsigned char *)arg0 + (12));
        sp54 = (s32) temp_v0_3;
        temp_v1_2 = *(u8 *)((unsigned char *)arg0 + (13));
        var_ra_2 = temp_v1_2 - var_a1;
        sp50 = (temp_v1_2 + var_t1) - 1;
        var_t5 = temp_v0_3 + var_t0 + var_a0;
block_14:
        var_ra = var_ra_2 - 1;
        break;
    case 2:
        temp_v0_4 = *(u8 *)((unsigned char *)arg0 + (12));
        sp54 = (temp_v0_4 + var_t0) - 1;
        temp_v1_3 = *(u8 *)((unsigned char *)arg0 + (13));
        var_t5 = (temp_v0_4 - var_a0) - 1;
        var_ra = temp_v1_3 + var_t1 + var_a1;
        sp50 = (s32) temp_v1_3;
        break;
    case 3:
        temp_v0_5 = *(u8 *)((unsigned char *)arg0 + (12));
        sp54 = (temp_v0_5 + var_t0) - 1;
        temp_v1_4 = *(u8 *)((unsigned char *)arg0 + (13));
        var_ra_2 = temp_v1_4 - var_a1;
        sp50 = (temp_v1_4 + var_t1) - 1;
        var_t5 = (temp_v0_5 - var_a0) - 1;
        goto block_14;
    }
    if (var_t5 < 0) {
        var_t5 = 0;
        goto block_19;
    }
    if (var_t5 >= 0x100) {
        var_t5 = 0xFF;
block_19:
        var_t0 -= 1;
    }
    if (var_ra < 0) {
        var_ra = 0;
        goto block_24;
    }
    if (var_ra >= 0x100) {
        var_ra = 0xFF;
block_24:
        var_t1 -= 1;
    }
    temp_v0_6 = *(u8 *)((unsigned char *)arg0 + (43));
    switch (temp_v0_6) {
    case 0:
        temp_v1_5 = *(s16 *)((unsigned char *)arg0 + (8));
        temp_a0 = *(s16 *)((unsigned char *)arg0 + (10));
        temp_a1 = temp_v1_5 + var_t0;
        temp_a3 = temp_a0 + var_t1;
        *(s16 *)((unsigned char *)arg0 + (16)) = temp_a1;
        *(s16 *)((unsigned char *)arg0 + (26)) = temp_a3;
        *(s16 *)((unsigned char *)arg0 + (32)) = temp_a1;
        *(s16 *)((unsigned char *)arg0 + (34)) = temp_a3;
        *(s16 *)((unsigned char *)arg0 + (24)) = temp_v1_5;
        *(s16 *)((unsigned char *)arg0 + (18)) = temp_a0;
        break;
    case 1:
        temp_v1_6 = *(s16 *)((unsigned char *)arg0 + (8));
        temp_a0_2 = *(s16 *)((unsigned char *)arg0 + (10));
        temp_a1_2 = temp_v1_6 + var_t0;
        temp_a3_2 = temp_a0_2 + var_t1;
        *(s16 *)((unsigned char *)arg0 + (16)) = temp_a1_2;
        *(s16 *)((unsigned char *)arg0 + (26)) = temp_a3_2;
        *(s16 *)((unsigned char *)arg0 + (32)) = temp_a1_2;
        *(s16 *)((unsigned char *)arg0 + (34)) = temp_a3_2;
        *(s16 *)((unsigned char *)arg0 + (24)) = temp_v1_6;
        *(s16 *)((unsigned char *)arg0 + (18)) = temp_a0_2;
        temp_v0_7 = *(s16 *)((unsigned char *)arg1 + (4));
        *(s16 *)((unsigned char *)arg0 + (8)) = (s16) (temp_v0_7 + ((s32) ((temp_v1_6 - temp_v0_7) * (*(s32 *)arg1)) >> 0xC));
        temp_v0_8 = *(s16 *)((unsigned char *)arg1 + (4));
        *(s16 *)((unsigned char *)arg0 + (16)) = (s16) (temp_v0_8 + ((s32) ((*(s16 *)((unsigned char *)arg0 + (16)) - temp_v0_8) * (*(s32 *)arg1)) >> 0xC));
        temp_v0_9 = *(s16 *)((unsigned char *)arg1 + (4));
        *(s16 *)((unsigned char *)arg0 + (24)) = (s16) (temp_v0_9 + ((s32) ((*(s16 *)((unsigned char *)arg0 + (24)) - temp_v0_9) * (*(s32 *)arg1)) >> 0xC));
        temp_v0_10 = *(s16 *)((unsigned char *)arg1 + (4));
        *(s16 *)((unsigned char *)arg0 + (32)) = (s16) (temp_v0_10 + ((s32) ((*(s16 *)((unsigned char *)arg0 + (32)) - temp_v0_10) * (*(s32 *)arg1)) >> 0xC));
        temp_v1_7 = *(s16 *)((unsigned char *)arg1 + (6));
        *(s16 *)((unsigned char *)arg0 + (10)) = (s16) (temp_v1_7 + ((s32) ((temp_a0_2 - temp_v1_7) * *(s16 *)((unsigned char *)arg1 + (2))) >> 0xC));
        temp_v1_8 = *(s16 *)((unsigned char *)arg1 + (6));
        *(s16 *)((unsigned char *)arg0 + (18)) = (s16) (temp_v1_8 + ((s32) ((*(s16 *)((unsigned char *)arg0 + (18)) - temp_v1_8) * *(s16 *)((unsigned char *)arg1 + (2))) >> 0xC));
        temp_v1_9 = *(s16 *)((unsigned char *)arg1 + (6));
        *(s16 *)((unsigned char *)arg0 + (26)) = (s16) (temp_v1_9 + ((s32) ((*(s16 *)((unsigned char *)arg0 + (26)) - temp_v1_9) * *(s16 *)((unsigned char *)arg1 + (2))) >> 0xC));
        temp_v1_10 = *(s16 *)((unsigned char *)arg1 + (6));
        *(s16 *)((unsigned char *)arg0 + (34)) = (s16) (temp_v1_10 + ((s32) ((*(s16 *)((unsigned char *)arg0 + (34)) - temp_v1_10) * *(s16 *)((unsigned char *)arg1 + (2))) >> 0xC));
        break;
    }
    if (*(u8 *)((unsigned char *)arg0 + (44)) != 0) {
        sp48 = var_ra;
        sp4C = var_t5;
        sp3C = func_800A0140(*(s32 *)((unsigned char *)arg1 + (12)));
        temp_v0_11 = func_800A0070(*(s32 *)((unsigned char *)arg1 + (12)));
        temp_v1_11 = *(s16 *)((unsigned char *)arg1 + (8));
        temp_t3 = *(s16 *)((unsigned char *)arg0 + (8)) - temp_v1_11;
        temp_a0_3 = *(s16 *)((unsigned char *)arg1 + (10));
        temp_t4 = *(s16 *)((unsigned char *)arg0 + (10)) - temp_a0_3;
        sp36 = temp_v1_11 + ((s32) ((temp_t3 * sp3C) - (temp_t4 * temp_v0_11)) >> 0xC);
        temp_a3_3 = *(s16 *)((unsigned char *)arg0 + (16));
        temp_t0 = *(s16 *)((unsigned char *)arg0 + (18));
        sp34 = temp_v1_11 + ((s32) (((temp_a3_3 - temp_v1_11) * sp3C) - ((temp_t0 - temp_a0_3) * temp_v0_11)) >> 0xC);
        temp_t1 = *(s16 *)((unsigned char *)arg0 + (24));
        temp_t2 = *(s16 *)((unsigned char *)arg0 + (26));
        sp32 = temp_v1_11 + ((s32) (((temp_t1 - temp_v1_11) * sp3C) - ((temp_t2 - temp_a0_3) * temp_v0_11)) >> 0xC);
        sp30 = temp_v1_11 + ((s32) (((*(s16 *)((unsigned char *)arg0 + (32)) - temp_v1_11) * sp3C) - ((*(s16 *)((unsigned char *)arg0 + (34)) - temp_a0_3) * temp_v0_11)) >> 0xC);
        *(s16 *)((unsigned char *)arg0 + (10)) = (s16) (temp_a0_3 + ((s32) ((temp_t3 * temp_v0_11) + (temp_t4 * sp3C)) >> 0xC));
        temp_a0_4 = *(s16 *)((unsigned char *)arg1 + (10));
        *(s16 *)((unsigned char *)arg0 + (18)) = (s16) (temp_a0_4 + ((s32) (((temp_a3_3 - *(s16 *)((unsigned char *)arg1 + (8))) * temp_v0_11) + ((temp_t0 - temp_a0_4) * sp3C)) >> 0xC));
        temp_a0_5 = *(s16 *)((unsigned char *)arg1 + (10));
        *(s16 *)((unsigned char *)arg0 + (26)) = (s16) (temp_a0_5 + ((s32) (((temp_t1 - *(s16 *)((unsigned char *)arg1 + (8))) * temp_v0_11) + ((temp_t2 - temp_a0_5) * sp3C)) >> 0xC));
        temp_a0_6 = *(s16 *)((unsigned char *)arg1 + (10));
        *(s16 *)((unsigned char *)arg0 + (34)) = (s16) (temp_a0_6 + ((s32) (((*(s16 *)((unsigned char *)arg0 + (32)) - *(s16 *)((unsigned char *)arg1 + (8))) * temp_v0_11) + ((*(s16 *)((unsigned char *)arg0 + (34)) - temp_a0_6) * sp3C)) >> 0xC));
        *(s16 *)((unsigned char *)arg0 + (8)) = sp36;
        *(s16 *)((unsigned char *)arg0 + (16)) = sp34;
        *(s16 *)((unsigned char *)arg0 + (24)) = sp32;
        *(s16 *)((unsigned char *)arg0 + (32)) = sp30;
        if (sp54 < var_t5) {
            var_t5 -= 1;
        } else {
            var_t5 += 1;
        }
        if (sp50 < var_ra) {
            var_ra -= 1;
        } else {
            var_ra += 1;
        }
    }
    *(u8 *)((unsigned char *)arg0 + (12)) = (u8) sp54;
    *(s8 *)((unsigned char *)arg0 + (20)) = (s8) var_t5;
    *(u8 *)((unsigned char *)arg0 + (13)) = (u8) sp50;
    *(s8 *)((unsigned char *)arg0 + (21)) = (s8) sp50;
    *(s8 *)((unsigned char *)arg0 + (37)) = (s8) var_ra;
    *(s8 *)((unsigned char *)arg0 + (36)) = (s8) var_t5;
    *(s8 *)((unsigned char *)arg0 + (29)) = (s8) var_ra;
    *(s8 *)((unsigned char *)arg0 + (28)) = (s8) sp54;
}

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
