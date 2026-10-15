#include "common.h"
#include "ovl/TACO.h"

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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80142820", func_801428DC);

/* bulk_m2c declarations (T-11190) */
extern u32 D_800E73A0;
extern u8 D_800E738D;
extern u8 D_800E7395;
/* end bulk_m2c declarations */
void func_80142AF0(void) {
    switch ((u8) D_800E738D) {
    case 0x0:
        if (func_80053F04() != 0) {
            if (D_800E7395 == 1) {
                D_800E738D += 1;
            } else if (D_800E7395 == 0xF1) {
                D_800E738D += 1;
            } else if (D_800E7395 == 0xFF) {
                D_800E738D += 1;
            }
        } else if ((u32) D_800E73A0 >= 0x201U) {
            D_800E738D += 1;
        }
        break;
    case 0x1:
        func_80053D10();
        if (D_800E7395 == 1) {
            if (func_80054AF4(0) == 1) {
                D_800E738D += 1;
                func_80053CE0();
                func_8005C4CC(0);
            } else {
                D_800E738D = 0xFF;
            }
        } else {
            D_800E738D = 0xFF;
        }
        break;
    case 0x2:
        if (func_80054284() == 3) {
            func_80053D10();
            D_800E738D += 1;
        } else if (((u32) D_800E73A0 >= 0x201U) || (D_800E7395 == 0xF0)) {
            func_80053D10();
            D_800E738D = 0xFF;
        }
        break;
    case 0x3:
        if (func_800552DC(0) == -1) {
            func_8005C4CC(1);
            D_800E738D = 0xFF;
        } else {
            func_8005C4CC(1);
            D_800E738D += 1;
        }
        break;
    case 0x4:
        func_80053CE0();
        D_800E7D36 = 0;
        D_800E738D += 1;
        break;
    case 0x5:
        if (func_80054388() == 0x13) {
            func_80044750(0x50F);
            func_80053D10();
            D_800E738D += 1;
        } else if (((u32) D_800E73A0 >= 0x201U) || (D_800E7395 == 0xF0)) {
            func_80044750(0x502);
            func_80053D10();
            D_800E738D += 1;
        }
        break;
    default:
        func_8004284C();
        break;
    }
}

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
