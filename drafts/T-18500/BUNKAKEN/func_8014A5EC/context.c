#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_80148ED0(void) {
    D_801562A0[0][0] = 0x801B00DC;
    D_801562A0[0][1] = 0x801B02AC;
    D_801562A0[0][2] = 0x801B03C8;
    D_801562A0[0][3] = 0x801B04C4;
    D_801562A0[0][4] = 0x801B0668;
    D_801562A0[0][5] = 0x801B0808;
    D_801562A0[0][6] = 0x801B09DC;
    D_801562A0[0][7] = 0x801B0BE8;
    D_801562A0[0][8] = 0x801B0D8C;
    D_801562A0[0][9] = 0x801B0F5C;
    D_801562A0[0][10] = 0x801B1164;
    D_801562A0[0][11] = 0x801B12D8;
    D_801562A0[0][12] = 0x801B1454;
    D_801562A0[1][0] = 0x801B00F4;
    D_801562A0[1][1] = 0x801B02C8;
    D_801562A0[1][2] = 0x801B03D0;
    D_801562A0[1][3] = 0x801B04DC;
    D_801562A0[1][4] = 0x801B0680;
    D_801562A0[1][5] = 0x801B0820;
    D_801562A0[1][6] = 0x801B09F8;
    D_801562A0[1][7] = 0x801B0C00;
    D_801562A0[1][8] = 0x801B0DA4;
    D_801562A0[1][9] = 0x801B0F78;
    D_801562A0[1][10] = 0x801B1180;
    D_801562A0[1][11] = 0x801B12E0;
    D_801562A0[1][12] = 0x801B1470;
    D_801562A0[2][0] = 0x801B01A8;
    D_801562A0[2][1] = 0x801B0398;
    D_801562A0[2][2] = 0x801B03F0;
    D_801562A0[2][3] = 0x801B0590;
    D_801562A0[2][4] = 0x801B0734;
    D_801562A0[2][5] = 0x801B08D4;
    D_801562A0[2][6] = 0x801B0AE4;
    D_801562A0[2][7] = 0x801B0CB4;
    D_801562A0[2][8] = 0x801B0E58;
    D_801562A0[2][9] = 0x801B1064;
    D_801562A0[2][10] = 0x801B126C;
    D_801562A0[2][11] = 0x801B1338;
    D_801562A0[2][12] = 0x801B155C;
}

void func_80149144(void) {
    u32 temp_t7;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

    switch (D_800E6280.unk_110A) {
    case 0x0:
        D_800E6280.unk_71E |= 1;
        func_8004E9F4(0);
        func_8006612C((s32) "　");
        func_80065F34(0);
        func_800573AC();
        func_80057390(0);
        func_80044750(0xCF);
        cd_read_sectors(3U, 0x801E8000, 0x49B7);
        func_80085ED0();
        func_8004284C();
        break;
    case 0x1:
        if (func_80085EEC() >= 0x15) {
            if (cd_read_sync_status() & 1) {
                func_80044890(1, 0xC478, 0xC44B, 0xCA95, 0xCA4F, 0xCA3E);
                D_800E6280.unk_F5F = 0;
                func_800847B8(0U);
                func_8013257C();
                D_800CA148 = 0;
                D_800CA14C = 0;
                D_800CA160 = (s32) D_80155E90;
                D_800CA164 = (s32) D_80155E94;
                D_800CA168 = (s32) D_80155E98;
                func_8004284C();
            }
        }
        break;
    case 0x2:
        if ((u32) ((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) < 6U) {
            func_80042940(0x2E);
        } else if (((D_800E6280.unk_0F8 & 0xF) == D_800E6280.unk_03F) && (((u32) (D_800E6280.unk_0F8 << 0x17) >> 0x1B) == D_800E6280.unk_040) && (func_8008667C(0) < 2)) {
            func_80042940(0x16);
        } else {
            func_8004284C();
        }
        break;
    case 0x3:
        if (func_80044E8C() == 1) {
            D_800CA148 = 0x25;
            if (func_8008667C(0) < 2) {
                func_8005742C(0x42D8, 1);
            } else {
                func_8005742C(0x403B, 1);
            }
            func_8004284C();
        }
        break;
    case 0x4:
        func_80085A60();
        break;
    case 0x5:
        if (cd_read_sync_status() & 1) {
            func_80044750(0x200);
            func_8004E9F4(0);
            if (func_8008667C(0) < 2) {
                func_8006612C((s32) "近所の公園");
                func_80085B3C(0x14, 0x19);
            } else {
                func_8006612C((s32) "廊下");
                func_80085B3C(0x14, 0x1A);
            }
            D_80125CA0 = 0xC0;
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 0x6:
        func_8007E884();
        break;
    case 0x7:
        D_800CA148 = 0x28;
        func_8004284C();
        break;
    case 0x8:
        func_80085A60();
        break;
    case 0x9:
        D_800CA148 = D_800E6280.unk_03E - 0x5C;
        func_8004284C();
        break;
    case 0xA:
        func_80085A60();
        break;
    case 0xB:
        func_80082F70();
        break;
    case 0xC:
        temp_v0 = func_8008667C(0);
        switch (temp_v0) {
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
    case 0xD:
        func_80085A60();
        break;
    case 0xE:
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
    case 0xF:
        func_80085A60();
        break;
    case 0x10:
        if (func_8008667C(0) < 2) {
            D_800CA148 = 0x27;
        }
        func_8004284C();
        break;
    case 0x11:
        func_80085A60();
        break;
    case 0x12:
        func_8007E99C();
        break;
    case 0x13:
        func_8004284C();
    case 0x14:
        func_8004284C();
        break;
    case 0x15:
        func_80145530();
        break;
    case 0x16:
        if (func_80044E8C() == 1) {
            D_800E6280.unk_71E |= 4;
            D_800CA148 = 0x25;
            func_8005742C(0x42D8, 1);
            func_8004284C();
        }
        break;
    case 0x17:
        func_80085A60();
        break;
    case 0x18:
        if (cd_read_sync_status() & 1) {
            func_80085B3C(0x14, 0x19);
            func_80044750(0x200);
            func_8004E9F4(0);
            func_8006612C((s32) "近所の公園");
            D_80125CA0 = 0xC0;
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(3U, 0x801B0000, 0xAF43);
            func_8004284C();
        }
        break;
    case 0x19:
        func_8007E884();
        break;
    case 0x1A:
        if (cd_read_sync_status() & 1) {
            func_80148ED0();
            D_800CA148 = 1;
            D_800CA14C = 0;
            D_800CA160 = D_801562A0[0][1];
            D_800CA164 = D_801562A0[1][1];
            D_800CA168 = D_801562A0[2][1];
            func_8004284C();
        }
        break;
    case 0x1B:
        func_80085A60();
        break;
    case 0x1C:
        cd_read_sectors(3U, 0x801E8000, 0x49B7);
        func_8004284C();
        break;
    case 0x1D:
        if (cd_read_sync_status() & 1) {
            func_8013257C();
            D_800CA148 = 0x26;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_80155E90;
            D_800CA164 = (s32) D_80155E94;
            D_800CA168 = (s32) D_80155E98;
            func_8004284C();
        }
        break;
    case 0x1E:
        func_80085A60();
        break;
    case 0x1F:
        D_800CA148 = 0x29;
        func_8004284C();
        break;
    case 0x20:
        func_80085A60();
        break;
    case 0x21:
        D_800CA148 = D_800E6280.unk_03E - 0x5C;
        func_8004284C();
        break;
    case 0x22:
        func_80085A60();
        break;
    case 0x23:
        func_80082F70();
        break;
    case 0x24:
        D_800CA148 = 0x23;
        func_8004284C();
        break;
    case 0x25:
        func_80085A60();
        break;
    case 0x26:
        func_8004284C();
        break;
    case 0x27:
        func_8004284C();
        break;
    case 0x28:
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
    case 0x29:
        func_80085A60();
        break;
    case 0x2A:
        if ((func_8008667C(0) < 2) && (D_80122CDC == 0)) {
            D_800CA148 = 0x27;
        }
        func_8004284C();
        break;
    case 0x2B:
        func_80085A60();
        break;
    case 0x2C:
        func_8007E99C();
        break;
    case 0x2D:
        func_80145530();
        break;
    case 0x2E:
        if (func_80044E8C() == 1) {
            D_800CA148 = 0;
            switch ((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) {
            case 0:
                if (D_800E6280.unk_03E == 0x61) {
                    func_8005742C(0x40F4, 1);
                } else {
                    func_8005742C(0x40EA, 1);
                }
                break;
            case 1:
            case 5:
                func_8005742C(0x40F4, 1);
                break;
            case 2:
                func_8005742C(0x40FC, 1);
                break;
            case 3:
                func_8005742C(0x4105, 1);
                break;
            case 4:
                if (D_800E6280.unk_03E == 0x60) {
                    func_8005742C(0x4118, 1);
                } else {
                    func_8005742C(0x410F, 1);
                }
                break;
            }
            func_8004284C();
        }
        break;
    case 0x2F:
        func_80085A60();
        break;
    case 0x30:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            temp_t7 = (u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4;
            switch (temp_t7) {
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
    case 0x31:
        func_80083174();
        break;
    case 0x32:
        if (D_80122CDC == 0) {
            func_80042940(0x34);
        } else {
            func_80085A60();
        }
        break;
    case 0x33:
        D_800E6280.unk_1BC[0].unk_06 -= 1;
        D_800E6280.unk_1BC[0].unk_0A += 5;
        func_80145530();
        break;
    case 0x34:
        func_80044750(0x200);
        D_800CA148 = D_800E6280.unk_03E - 0x5C;
        func_8004284C();
        break;
    case 0x35:
        func_80085A60();
        break;
    case 0x36:
        func_80082F70();
        break;
    case 0x37:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 6;
            func_8014A5EC();
            func_8007C740();
            D_800E6280.unk_F5F = 0;
            func_800847B8(0U);
            func_80083474();
        }
        break;
    case 0x38:
        func_80085A60();
        break;
    case 0x39:
        func_8007E81C();
        break;
    case 0x3A:
        D_800CA148 = 0xD;
        func_8004284C();
        break;
    case 0x3B:
        func_80085A60();
        break;
    case 0x3C:
        func_8007E390();
        break;
    case 0x3D:
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {
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
    case 0x3E:
        func_80085A60();
        break;
    case 0x3F:
        temp_v0_3 = func_8008667C(0);
        switch (temp_v0_3) {
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
    case 0x40:
        func_80085A60();
        break;
    case 0x41:
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
    case 0x42:
        func_80085A60();
        break;
    case 0x43:
        func_8007E5A0();
        break;
    case 0x44:
        func_80085A60();
        break;
    case 0x45:
        func_8007E934();
        break;
    case 0x46:
        D_800CA148 = 0x24;
        func_8004284C();
        break;
    case 0x47:
        func_80085A60();
        break;
    case 0x48:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80148ED0", func_8014A0E0);

s32 func_8014A5EC(void) {
    GameState *g = &D_800E6280;
    typedef struct ByteTable297 { /* 0x00 */ u8 b[297]; } ByteTable297; /* size 0x129 */
    BunkakenNames3x3 sp28;

    sp28 = D_8015633C;
    func_800AE0F0(D_800CA25C, (sp28.b + (g->unk_03E * 0x63) + (D_80122CDC * 0x21)) - 0x24BD);
}
