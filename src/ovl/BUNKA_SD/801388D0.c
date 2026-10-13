#include "common.h"
#include "ovl/BUNKA_SD.h"

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_801388D0);

s32 func_801389DC(void) {
    if (D_800E6280.unk_110D == 0) {
        cd_read_sectors(0x56, 0x80180000, 0x4F1D);
        D_800E6280.unk_110D += 1;
    } else if (D_800E6280.unk_110D == 1 && (cd_read_sync_status() & 1)) {
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80046290(0x3F, 0x0500039C, 0xF);
            break;
        case 0x60:
            func_80046290(0x0500005B, 0x05000450, 0xF);
            break;
        default:
        case 0x61:
            func_80046290(0x02000062, 0x080003F6, 0xF);
            break;
        }
        func_80044750(0x300);
        func_8004284C();
    }
}

void func_80138ADC(void) {
    if ((u32)D_800E6280.unk_110A >= 3 && (u32)D_800E6280.unk_110A < 6 && (D_800E6280.unk_F80 & 0x40)) {
        func_80042940(5);
        func_80044750(0xC4);
    }
    func_80138B3C();
}

void func_80138B3C(void) {
    func_80066C08(2);
    func_80064F48();
    func_80066334();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80134260();
        func_80044750(0x7F);
        return;
    case 1:
        func_801389DC();
        return;
    case 2:
        func_80138C34();
        return;
    case 3:
        func_80134418();
        return;
    case 4:
        func_80138CAC();
        return;
    case 5:
        func_8004E9F4(1);
        func_8004284C();
        return;
    case 6:
        func_8013987C();
        return;
    case 7:
        func_801388D0();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80138C34(void) {
    if (func_800460EC() & 4) {
        if (D_800E6280.unk_1100++ >= 1U) {
            func_80138D48();
            func_8004284C();
            func_80045414(9, 0, 0);
        }
    } else {
        D_800E6280.unk_1100 = 0;
    }
}

void func_80138CAC(void) {
    GameState *g;

    func_80139100();
    func_80139290();
    g = &D_800E6280;
    g->unk_1104.u += 1;
    if ((g->unk_1104.u + 0x90000000U) == 0xFA && g->unk_03E != 0x60) {
        func_8004284C();
    }
    if ((g->unk_1104.u + 0x90000000U) == 0xD0 && g->unk_03E == 0x60) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_80138D48);

s32 func_80139100(void) {
    if (D_800E6280.unk_1104.u == 0xBE) {
        D_800E6280.unk_1100 = func_80046274() - 0x1000;
    }
    if (D_800E6280.unk_1104.u == 0xBC) {
        func_80044750(0x300);
    }
    func_8004B19C(func_80046274(), 0, 0);
    func_8004B19C(D_800E6280.unk_1100, 0, 0x10);
    func_8004B19C(func_80046274() - D_800E6280.unk_1100, 0, 0x20);
    if (D_800E6280.unk_1104.u >= 0xC0U) {
        if (D_800E6280.unk_1104.u < 0x70000000U) {
            switch (D_800E6280.unk_03E) {
            case 0x5F:
                if ((u32)(func_80046274() - D_800E6280.unk_1100) >= 0x2609U) {
                    D_800E6280.unk_1104.u = 0x70000000;
                }
                break;
            case 0x60:
                if ((u32)(func_80046274() - D_800E6280.unk_1100) >= 0x2AF1U) {
                    D_800E6280.unk_1104.u = 0x70000000;
                }
                break;
            case 0x61:
            default:
                if ((u32)(func_80046274() - D_800E6280.unk_1100) >= 0x27E9U) {
                    D_800E6280.unk_1104.u = 0x70000000;
                }
                break;
            }
        }
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_80139290);

/* NON_MATCHING: T-10710, 6 lines: v is kept as t6 copy (move v1,t6) where the original masks into t6 and later reloads v1=0 (move v1,zero) for the clear loop stores */
#ifdef NON_MATCHING
void func_8013987C(void) {
    u8 v;
    u8 *p;
    s32 i;

    v = D_800E6280.unk_110B;
    p = D_80120650;
    do {
        p[7] = v;
        p += 0x44;
    } while (p < D_80121750);
    if (v == 0) {
        v = 2;
    }
    v -= 1;
    D_800E6280.unk_110B = v;
    if (v == 0) {
        for (i = 0; i < 0x40; i++) {
            D_80120650[i * 0x44 + 7] = v;
        }
        *(u8 *)&D_800E6280.unk_110B = v;
        func_8004284C();
    }
}
#else
INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_8013987C);
#endif
