#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80042540", func_80042540);

void func_80042798(void) {
    D_800E6280.unk_1108 += 1;
    D_800E6280.unk_1109 = 0;
    D_800E6280.unk_110A = 0;
    D_800E6280.unk_10FC = 0;
    D_800E6280.unk_1100 = 0;
    D_800E6280.unk_1104.w = 0;
    D_800E6280.unk_110B = 0;
    D_800E6280.unk_110C = 0;
    D_800E6280.unk_110D = 0;
    func_80042960();
}

/* FAKE: z is read through its address, so uopt sets v0 = 0 before the last
 * store and as1 puts that store in the jr delay slot, as the original does; no
 * `return 0` form does (the normal_date_girl_in_init trick, T-9540). s32 with no
 * return. T-11610 */
s32 func_80042808(void) {
    s32 *p;
    s32 z;

    D_800E6280.unk_1109 += 1;
    D_800E6280.unk_110A = 0;
    D_800E6280.unk_1100 = 0;
    D_800E6280.unk_1104.w = 0;
    D_800E6280.unk_110C = 0;
    z = 0;
    p = &z;
    D_800E6280.unk_110D = *p;
}

/* FAKE: z is read through its address, so uopt sets v0 = 0 before the last
 * store and as1 puts that store in the jr delay slot, as the original does; no
 * `return 0` form does (the normal_date_girl_in_init trick, T-9540). s32 with no
 * return. T-11610 */
s32 func_8004284C(void) {
    s32 *p;
    s32 z;

    D_800E6280.unk_110A += 1;
    D_800E6280.unk_1104.w = 0;
    z = 0;
    p = &z;
    D_800E6280.unk_110D = *p;
}

void main_state_set(s32 arg0) {
    u32 t;

    D_800E6280.unk_1108 = arg0;
    D_800E6280.unk_1109 = 0;
    D_800E6280.unk_110A = 0;
    D_800E6280.unk_10FC = 0;
    D_800E6280.unk_1100 = 0;
    D_800E6280.unk_1104.w = 0;
    D_800E6280.unk_110B = 0;
    D_800E6280.unk_110C = 0;
    D_800E6280.unk_110D = 0;
    func_80042960();
    t = D_800E6280.unk_110E;
    D_800CA2AC = 0;
    if (t != 0x5E) {
        if (t != 0x50) {
            func_80065F34(0);
        }
    }
}

/* FAKE: z is read through its address, so uopt sets v0 = 0 before the last
 * store and as1 puts that store in the jr delay slot, as the original does; no
 * `return 0` form does (the normal_date_girl_in_init trick, T-9540). s32 with no
 * return. T-11610 */
s32 func_80042908(s32 arg0) {
    s32 *p;
    s32 z;

    D_800E6280.unk_1109 = arg0;
    D_800E6280.unk_110A = 0;
    D_800E6280.unk_1100 = 0;
    D_800E6280.unk_1104.w = 0;
    D_800E6280.unk_110C = 0;
    z = 0;
    p = &z;
    D_800E6280.unk_110D = *p;
}

/* FAKE: z is read through its address, so uopt sets v0 = 0 before the last
 * store and as1 puts that store in the jr delay slot, as the original does; no
 * `return 0` form does (the normal_date_girl_in_init trick, T-9540). s32 with no
 * return. T-11610 */
s32 func_80042940(s32 arg0) {
    s32 *p;
    s32 z;

    D_800E6280.unk_110A = arg0;
    D_800E6280.unk_1104.w = 0;
    z = 0;
    p = &z;
    D_800E6280.unk_110D = *p;
}

s32 func_80042960(void) {
    switch (*(u8 *)&D_800E6280.unk_1108) {
    case 0x11:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x35:
    case 0x36:
    case 0x37:
    case 0x3A:
    case 0x43:
    case 0x45:
    case 0x50:
    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57:
    case 0x58:
    case 0x59:
    case 0x5A:
    case 0x5B:
    case 0x5C:
    case 0x5D:
    case 0x5E:
    case 0x5F:
    case 0x60:
    case 0x61:
    case 0x62:
    case 0x63:
    case 0x70:
    case 0x71:
    case 0x72:
    case 0x73:
    case 0x74:
    case 0x75:
    case 0x76:
    case 0x80:
    case 0x81:
    case 0x82:
    case 0x90:
    case 0x91:
    case 0xC2:
    case 0xC3:
    case 0xC4:
        D_800E6280.unk_110E = D_800E6280.unk_1108;
        D_800E6280.unk_1108 = 2;
        return;
    }
}
