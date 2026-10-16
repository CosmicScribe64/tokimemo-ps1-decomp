#define MAIN_API_OVERRIDE_func_800AE090 /* matched as int: the call's $v0 result range (main_api.h: void), T-19009 */
#include "common.h"
#include "ovl/TACO.h"

s32 func_800AE090();

void func_80142820(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_801428DC();
        break;
    case 1:
        func_80142FE8();
        break;
    case 2:
        func_80142AF0();
        break;
    case 3:
        func_80142EC4();
        break;
    case 4:
        func_80142EE4();
        break;
    case 5:
        func_80142F0C();
        break;
    case 6:
        func_80142F34();
        break;
    case 7:
        func_80142DE0();
        break;
    }
}

/* Settings block at D_800E7D50 (+0x40 of D_800E7D10, see TcSave in 80133870.c): byte 0 holds the
 * "saved" flag, the sound mode and the selector, byte 1 two more selectors. */
typedef struct TcSetBits {
    u8 flag : 1;
    u8 pad0 : 1;
    u8 mode : 1;
    u8 sel : 2;
    u8 pad1 : 3;
    u8 lo : 4;
    u8 hi : 4;
} TcSetBits;

#ifdef NON_MATCHING
void func_801428DC(void) {
    ((TcSetBits *)&D_800E7D50)->flag = 1;
    D_800E7D5C = D_8015EDF8;
    D_800E7D60 = D_8015EDFC;
    D_800E7D64 = D_8015EE00;
    func_800AE090((u8 *)&D_800E7D50 + 2, D_8015EE04, 3);
    func_800AE090(D_800E7D55, D_8015EE08, 3);
    func_800AE090(D_800E7D58, D_8015EE0C, 3);
    ((TcSetBits *)&D_800E7D50)->mode = D_8015EDD8 % 2;
    ((TcSetBits *)&D_800E7D50)->sel = D_8015EDDC % 3;
    ((TcSetBits *)&D_800E7D50)->hi = D_8015EDE0 % 3;
    ((TcSetBits *)&D_800E7D50)->lo = D_8015EDE4 % 3;
    func_80041584();
    draw2d3d(0, 0);
    D_800E6280.unk_014[0] = 0xA0;
    D_800E6280.unk_014[1] = 0xA0;
    func_8009C674(0);
    func_8009C210(3);
    func_80097D90(0x140, 0xF0, 0, 0, 0);
    if (func_80045288() == 0) {
        func_800452C4();
    }
    func_8005907C();
    func_80059048();
    func_801347A8();
    func_80048EB8(0);
    func_80048DAC(0);
    func_8004284C();
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80142820", func_801428DC);
#endif

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80142820", func_80142AF0);

typedef struct {
    u32 pad0 : 1;
    u32 flag : 1;
    u32 pad1 : 30;
} TcBits;

void func_80142DE0(void) {
    func_8004E58C();
    func_800573AC();
    func_800438DC(1, 0);
    func_80048DAC(1);
    *(s32 *) &D_800E6280.unk_1095[3] = 0x9F;
    if (((TcBits *) &D_800E7D50)->flag == 1) {
        func_80048EB8(0);
        func_80048390();
        main_state_set(D_800E6280.unk_720);
        func_80042908(D_800E6280.unk_721);
        func_80042940(D_800E6280.unk_722);
        func_8006BC28(0);
        func_8006C848(0);
        func_80064E84();
        func_800649D4();
        func_80064E48(1);
        func_8006492C(1);
        return;
    }
    main_state_set(0x12);
}

void func_80142EC4(void) {
    func_80059E00();
}

void func_80142EE4(void) {
    func_80059BE8();
    func_8004284C();
}

void func_80142F0C(void) {
    func_8004ADE4();
    func_8004284C();
}

u8 func_80142F34(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        cd_read_sectors(0x95, 0x801A0000, 0x9B94);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_80068938(D_800E6280.unk_03E, D_800E6280.unk_03F, 0);
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        func_8004284C();
        break;
    }
}

u8 func_80142FE8(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004482C();
        func_800674B0();
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        if (D_800B3220 == 0) {
            func_80053CE0();
        } else {
            func_8004284C();
        }
        func_8004284C();
        break;
    }
}
