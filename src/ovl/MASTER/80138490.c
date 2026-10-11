#include "ovl/MASTER.h"

void func_80138490(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    func_80083808();
    switch (D_800E6280.unk_110A) {
    case 0:
        D_800E6280.unk_71C += 1;
        D_800E6280.unk_71E |= 1;
        func_80044750(0xCF);
        func_80085ED0();
        if ((D_800E6280.unk_03F == (D_800E6280.unk_0F8 & 0xF)) && (D_800E6280.unk_040 == ((u32) (D_800E6280.unk_0F8 << 0x17) >> 0x1B))) {
            if (!(D_800E6280.unk_71E & 4)) {
                if (func_8008667C(0) < 2) {
                    D_800E6280.unk_71E |= 4;
                    func_80046318(3U, 0x801B0000, 0xAF43);
                    func_80042940(0x1B);
                } else {
                    goto block_7;
                }
            } else {
                goto block_8;
            }
        } else {
block_7:
block_8:
            func_80046318(3U, 0x801E8000, 0x49B7);
            func_8004284C();
        }
        break;
    case 1:
        func_8007EAAC();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_80044890(1, 0xC043, 0xC01D, 0xCA95, 0xCA4F, 0xCA3E);
            D_800E6280.unk_F5F = 0;
            func_800847B8(0U);
            func_801336E8();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_8013C4A0;
            D_800CA164 = (s32) D_8013C4A4;
            D_800CA168 = (s32) D_8013C4A8;
            func_8004284C();
        }
        break;
    case 3:
        func_80085A60();
        break;
    case 4:
        func_80083174();
        break;
    case 5:
        if ((*(u32 *)&D_80122CDC) != 0) {
            func_80085A60();
        } else {
            func_80042940(7);
        }
        break;
    case 6:
        D_800E6280.unk_1BC[0].unk_06 -= 1;
        D_800E6280.unk_1BC[0].unk_0A += 5;
        func_80042808();
        break;
    case 7:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_80062CD0(0x541D);
            D_800CA148 = 2;
            func_8004284C();
        }
        break;
    case 8:
        func_80085A60();
        break;
    case 9:
        D_800CA148 = 5;
        func_8004284C();
        break;
    case 10:
        func_80085A60();
        break;
    case 11:
        func_80082F70();
        break;
    case 12:
        func_80138E58();
        D_800CA148 = 6;
        func_8004284C();
        break;
    case 13:
        func_80085A60();
        break;
    case 14:
        if (func_800460CC() & 1) {
            func_8007C740();
            func_80083474();
        }
        break;
    case 15:
        D_800CA148 = 8;
        func_8004284C();
        break;
    case 16:
        func_80085A60();
        break;
    case 17:
        func_8007E390();
        break;
    case 18:
        temp_v0 = func_8008667C(0);
        switch (temp_v0) {
        case 0:
        case 1:
            D_800CA148 = 0xF;
            break;
        case 2:
            D_800CA148 = 0xF;
            break;
        case 3:
            D_800CA148 = 0x10;
            break;
        case 4:
            D_800CA148 = 0x11;
            break;
        }
        func_8004284C();
        break;
    case 19:
        func_80085A60();
        break;
    case 20:
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x17;
            break;
        case 2:
            D_800CA148 = 0x18;
            break;
        case 3:
            D_800CA148 = 0x19;
            break;
        case 4:
            D_800CA148 = 0x1A;
            break;
        }
        func_8004284C();
        break;
    case 21:
        func_80085A60();
        break;
    case 22:
        switch ((*(u32 *)&D_80122CDC)) {
        case 0:
            D_800CA148 = 0x1B;
            func_80083440(1U);
            D_800E6280.unk_1BC[0].unk_02 += 3;
            D_800E6280.unk_1BC[0].unk_06 += 2;
            D_800E6280.unk_1BC[0].unk_0A -= 0xF;
            break;
        case 1:
            D_800CA148 = 0x1D;
            func_80083440(2U);
            D_800E6280.unk_1BC[0].unk_02 += 2;
            D_800E6280.unk_1BC[0].unk_06 += 1;
            D_800E6280.unk_1BC[0].unk_0A -= 0xA;
            break;
        case 2:
            D_800CA148 = 0x1F;
            func_80083440(3U);
            D_800E6280.unk_1BC[0].unk_06 += 1;
            D_800E6280.unk_1BC[0].unk_0A += 0xA;
            break;
        }
        func_80063668();
        func_8004284C();
        break;
    case 23:
        func_80085A60();
        break;
    case 24:
        func_8007E5A0();
        break;
    case 25:
        func_80085A60();
        break;
    case 26:
        func_80085B3C(0x15, 6);
        func_80042808();
        break;
    case 27:
        D_800E6280.unk_71C += 1;
        func_8007EAAC();
        break;
    case 28:
        if (func_800460CC() & 1) {
            D_800E6280.unk_F5F = 0;
            func_800847B8(0U);
            func_801337B8();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = D_8013C4D8;
            D_800CA164 = D_8013C50C;
            D_800CA168 = D_8013C540;
            func_80062CD0(0x541D);
            func_8004284C();
        }
        break;
    case 29:
        if (func_800460CC() & 1) {
            func_8007C740();
            func_80083474();
            func_80044750(0xBF);
            func_80044890(1, 0xC043, 0xC01D, 0xCA95, 0xCA4F, 0xCA3E);
        }
        break;
    case 30:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_8004284C();
        }
        break;
    case 31:
        func_80085A60();
        break;
    case 32:
        func_8007E390();
        break;
    case 33:
        func_80085A60();
        break;
    case 34:
        func_80046318(3U, 0x801E8000, 0x49B7);
        func_8004284C();
        break;
    case 35:
        if (func_800460CC() & 1) {
            func_801336E8();
            D_800CA148 = 0xC;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_8013C4A0;
            D_800CA164 = (s32) D_8013C4A4;
            D_800CA168 = (s32) D_8013C4A8;
            func_8004284C();
        }
        break;
    case 36:
        func_80085A60();
        break;
    case 37:
        D_800CA148 = 0x21;
        func_8004284C();
        break;
    case 38:
        func_80085A60();
        break;
    case 39:
        func_80082F70();
        break;
    case 40:
        D_800CA148 = 0x17;
        func_8004284C();
        break;
    case 41:
        func_80085A60();
        break;
    case 42:
        switch ((*(u32 *)&D_80122CDC)) {
        case 0:
            D_800CA148 = 0x1B;
            func_80083440(1U);
            D_800E6280.unk_1BC[0].unk_02 += 3;
            D_800E6280.unk_1BC[0].unk_06 += 2;
            D_800E6280.unk_1BC[0].unk_0A -= 0xF;
            break;
        case 1:
            D_800CA148 = 0x1D;
            func_80083440(2U);
            D_800E6280.unk_1BC[0].unk_02 += 2;
            D_800E6280.unk_1BC[0].unk_06 += 1;
            D_800E6280.unk_1BC[0].unk_0A -= 0xA;
            break;
        case 2:
            D_800CA148 = 0x1F;
            func_80083440(3U);
            D_800E6280.unk_1BC[0].unk_06 += 1;
            D_800E6280.unk_1BC[0].unk_0A += 0xA;
            break;
        }
        func_80063668();
        func_8004284C();
        break;
    case 43:
        func_80085A60();
        break;
    case 44:
        func_8007E5A0();
        break;
    case 45:
        func_80085A60();
        break;
    case 46:
        func_80085B3C(0x15, 6);
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}

typedef struct {
    void *p[3];
} PtrTbl3; /* size 0xC */
extern PtrTbl3 D_8013C700;

void func_80138E58(void) {
    s32 idx; /* unused: its stack slot sits above tbl (T-3330) */
    PtrTbl3 tbl;

    tbl = D_8013C700;
    func_800AE0F0(D_800CA25C, tbl.p[D_80122CDC]);
}

INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80138490.rodata", D_8013BFB0);

INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80138490.rodata", D_8013BFBC);

INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80138490.rodata", D_8013BFC8);
