#include "common.h"
#include "ovl/BUNKASAI.h"

void func_80146FA0(void) {
    D_80160640 = 0x801B00DC;
    D_80160644 = 0x801B02AC;
    D_80160648 = 0x801B03C8;
    D_8016064C = 0x801B04C4;
    D_80160650 = 0x801B0668;
    D_80160654 = 0x801B0808;
    D_80160658 = 0x801B09DC;
    D_8016065C = 0x801B0BE8;
    D_80160660 = 0x801B0D8C;
    D_80160664 = 0x801B0F5C;
    D_80160668 = 0x801B1164;
    D_8016066C = 0x801B12D8;
    D_80160670 = 0x801B1454;
    D_80160674 = 0x801B00F4;
    D_80160678 = 0x801B02C8;
    D_8016067C = 0x801B03D0;
    D_80160680 = 0x801B04DC;
    D_80160684 = 0x801B0680;
    D_80160688 = 0x801B0820;
    D_8016068C = 0x801B09F8;
    D_80160690 = 0x801B0C00;
    D_80160694 = 0x801B0DA4;
    D_80160698 = 0x801B0F78;
    D_8016069C = 0x801B1180;
    D_801606A0 = 0x801B12E0;
    D_801606A4 = 0x801B1470;
    D_801606A8 = 0x801B01A8;
    D_801606AC = 0x801B0398;
    D_801606B0 = 0x801B03F0;
    D_801606B4 = 0x801B0590;
    D_801606B8 = 0x801B0734;
    D_801606BC = 0x801B08D4;
    D_801606C0 = 0x801B0AE4;
    D_801606C4 = 0x801B0CB4;
    D_801606C8 = 0x801B0E58;
    D_801606CC = 0x801B1064;
    D_801606D0 = 0x801B126C;
    D_801606D4 = 0x801B1338;
    D_801606D8 = 0x801B155C;
}

void func_80147214(void) {
    u32 temp_t1;
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0x0:
        func_8004E9F4(0);
        func_8006612C((s32) "　");
        func_80065F34(0);
        func_80057390(0);
        D_800E6280.unk_71E |= 1;
        func_80044750(0xCF);
        func_80085ED0();
        cd_read_sectors(3U, 0x801E8000, 0x49B7);
        func_8004284C();
        break;
    case 0x1:
        if (func_80085EEC() >= 0x15) {
            if (cd_read_sync_status() & 1) {
                func_80044890(1, 0xC478, 0xC44B, 0xCA95, 0xCA4F, 0xCA3E);
                D_800E6280.unk_F5F = 0;
                func_800847B8(0U);
                func_80132444();
                D_800CA148 = 0;
                D_800CA14C = 0;
                D_800CA160 = (s32) D_801604F0;
                D_800CA164 = (s32) D_801604F4;
                D_800CA168 = (s32) D_801604F8;
                func_8004284C();
            }
        }
        break;
    case 0x2:
        if (((D_800E6280.unk_0F8 & 0xF) == D_800E6280.unk_03F) && (((u32) (D_800E6280.unk_0F8 << 0x17) >> 0x1B) == D_800E6280.unk_040) && (func_8008667C(0) < 2) && (D_80122EBC == 0)) {
            func_80042940(0x24);
        } else {
            func_800573AC();
            func_80057390(0);
            func_8004284C();
        }
        break;
    case 0x3:
        func_80085A60();
        break;
    case 0x4:
        func_80083174();
        break;
    case 0x5:
        if (D_80122CDC != 0) {
            func_80085A60();
        } else {
            func_80042940(7);
        }
        break;
    case 0x6:
        D_800E6280.unk_1BC[0].unk_06 -= 1;
        D_800E6280.unk_1BC[0].unk_0A += 5;
        func_80142BC8();
        break;
    case 0x7:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            switch ((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) {
            case 0:
                if (D_800E6280.unk_03E == 0x61) {
                    func_80085B3C(0x14, 0x1C);
                    func_8005742C(0x40F4, 1);
                } else {
                    func_80085B3C(0x14, 0x1B);
                    func_8005742C(0x40EA, 1);
                }
                break;
            case 1:
                func_80085B3C(0x14, 0x1D);
                func_8005742C(0x40F4, 1);
                break;
            case 5:
                func_80085B3C(0x14, 0x22);
                func_8005742C(0x40F4, 1);
                break;
            case 2:
                func_80085B3C(0x14, 0x1E);
                func_8005742C(0x40FC, 1);
                break;
            case 3:
                func_80085B3C(0x14, 0x1F);
                func_8005742C(0x4105, 1);
                break;
            case 4:
                if (D_800E6280.unk_03E == 0x60) {
                    func_80085B3C(0x14, 0x21);
                    func_8005742C(0x4118, 1);
                } else {
                    func_80085B3C(0x14, 0x20);
                    func_8005742C(0x410F, 1);
                }
                break;
            default:
                func_8005742C(0x403B, 1);
                break;
            }
            D_800CA148 = 2;
            func_8004284C();
        }
        break;
    case 0x8:
        func_80085A60();
        break;
    case 0x9:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            temp_t1 = (u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4;
            switch (temp_t1) {
            case 1:
                func_80062CD0(0x5477);
                break;
            case 2:
                func_80062CD0(0x54D1);
                break;
            case 4:
                func_80062CD0(0x54A4);
                break;
            default:
                func_80062CD0(0x544A);
                break;
            }
            func_8004284C();
        }
        break;
    case 0xA:
        D_800CA148 = D_800E6280.unk_03E - 0x5C;
        func_8004284C();
        break;
    case 0xB:
        func_80085A60();
        break;
    case 0xC:
        func_80082F70();
        break;
    case 0xD:
        func_80147F8C();
        D_800CA148 = 6;
        func_8004284C();
        break;
    case 0xE:
        func_80085A60();
        break;
    case 0xF:
        if (cd_read_sync_status() & 1) {
            func_8007C740();
            func_80083474();
        }
        break;
    case 0x10:
        if (((u16) D_800E6280.unk_0F4.h >> 0xC) == ((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4)) {
            D_800CA148 = 0xA;
        } else {
            D_800CA148 = 0xB;
        }
        func_8004284C();
        break;
    case 0x11:
        func_80085A60();
        break;
    case 0x12:
        func_8007E81C();
        break;
    case 0x13:
        if (((u16) D_800E6280.unk_0F4.h >> 0xC) == ((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4)) {
            D_800CA148 = 0xE;
        } else {
            D_800CA148 = 0xD;
        }
        func_8004284C();
        break;
    case 0x14:
        func_80085A60();
        break;
    case 0x15:
        func_8007E390();
        break;
    case 0x16:
        temp_v0 = func_8008667C(0);
        switch (temp_v0) {
        case 0:
        case 1:
            D_800CA148 = 0x12;
            break;
        case 2:
            D_800CA148 = 0x13;
            break;
        case 3:
            D_800CA148 = 0x14;
            break;
        case 4:
            D_800CA148 = 0x15;
            break;
        }
        func_8004284C();
        break;
    case 0x17:
        func_80085A60();
        break;
    case 0x18:
        if (((u16) D_800E6280.unk_0F4.h >> 0xC) == ((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4)) {
            D_800CA148 = 0x16;
            func_8004284C();
        } else {
            func_80042940(0x1A);
        }
        break;
    case 0x19:
        func_80085A60();
        break;
    case 0x1A:
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
    case 0x1B:
        func_80085A60();
        break;
    case 0x1C:
        switch (*(u32 *)&D_80122CDC) {
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
    case 0x1D:
        func_80085A60();
        break;
    case 0x1E:
        func_8007E5A0();
        break;
    case 0x1F:
        func_80085A60();
        break;
    case 0x20:
        func_8007E934();
        break;
    case 0x21:
        D_800CA148 = 0x24;
        func_8004284C();
        break;
    case 0x22:
        func_80085A60();
        break;
    case 0x23:
        func_80142BC8();
        break;
    case 0x24:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            D_800E6280.unk_71E |= 4;
            func_8004284C();
        }
        break;
    case 0x25:
        func_8007E81C();
        break;
    case 0x26:
        if (((u16) D_800E6280.unk_0F4.h >> 0xC) != ((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4)) {
            D_800CA148 = 0x22;
            func_80042940(0x3E);
        } else {
            D_800CA148 = 0x2A;
            func_8004284C();
        }
        break;
    case 0x27:
        func_8007E390();
        break;
    case 0x28:
        func_80085A60();
        break;
    case 0x29:
        cd_read_sectors(3U, 0x801B0000, 0xAF43);
        func_8004284C();
        break;
    case 0x2A:
        if (cd_read_sync_status() & 1) {
            func_80146FA0();
            D_800CA148 = 1;
            D_800CA14C = 0;
            D_800CA160 = D_80160644;
            D_800CA164 = D_80160678;
            D_800CA168 = D_801606AC;
            func_8004284C();
        }
        break;
    case 0x2B:
        func_80085A60();
        break;
    case 0x2C:
        cd_read_sectors(3U, 0x801E8000, 0x49B7);
        func_8004284C();
        break;
    case 0x2D:
        if (cd_read_sync_status() & 1) {
            func_80132444();
            D_800CA148 = 0x26;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_801604F0;
            D_800CA164 = (s32) D_801604F4;
            D_800CA168 = (s32) D_801604F8;
            func_8004284C();
        }
        break;
    case 0x2E:
        func_80085A60();
        break;
    case 0x2F:
        D_800CA148 = 0x29;
        func_8004284C();
        break;
    case 0x30:
        func_80085A60();
        break;
    case 0x31:
        D_800CA148 = D_800E6280.unk_03E - 0x5C;
        func_8004284C();
        break;
    case 0x32:
        func_80085A60();
        break;
    case 0x33:
        func_80082F70();
        break;
    case 0x34:
        D_800CA148 = 0x23;
        func_8004284C();
        break;
    case 0x35:
        func_80085A60();
        break;
    case 0x36:
        switch (*(u32 *)&D_80122CDC) {
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
    case 0x37:
        func_80085A60();
        break;
    case 0x38:
        func_8007E5A0();
        break;
    case 0x39:
        func_80085A60();
        break;
    case 0x3A:
        func_8007E934();
        break;
    case 0x3B:
        D_800CA148 = 0x24;
        func_8004284C();
        break;
    case 0x3C:
        func_80085A60();
        break;
    case 0x3D:
        func_80142BC8();
        break;
    case 0x3E:
        func_8007E390();
        break;
    case 0x3F:
        func_80085A60();
        break;
    case 0x40:
        D_800CA148 = 0x26;
        func_80042940(0x2E);
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80147F8C(void) {
    u8 *name;
    BunkasaiNameTable tbl;

    tbl = D_801606DC;
    name = tbl.s[D_800E6280.unk_03E - 95][D_80122CDC];
    func_800AE0F0(D_800CA25C, name);
}
