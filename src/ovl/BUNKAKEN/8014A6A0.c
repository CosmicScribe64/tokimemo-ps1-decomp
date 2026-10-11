#include "common.h"
#include "ovl/BUNKAKEN.h"

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014A6A0", func_8014A6A0);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014A6A0", func_8014A9A8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014A6A0", func_8014AF48);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014A6A0", func_8014B378);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014A6A0", func_8014B7B8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014A6A0", func_8014BD80);

void func_8014C374(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            func_80062CD0(0x544A);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        if (func_800460CC() & 1) {
            func_8007C740();
            g->unk_F5F = 0;
            func_80083474();
            func_80046318(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 5:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            func_8007E390();
        }
        break;
    case 6:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 1;
                break;
            case 2:
                D_800CA148 = 2;
                break;
            case 3:
                D_800CA148 = 3;
                break;
            }
            if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
                D_800CA148 = 0x69;
            }
            func_8014E3DC();
        }
        break;
    case 7:
        func_80085A60();
        break;
    case 8:
        if (g->unk_1BC[0].unk_0C.f.b2) {
            func_80042940(0xA);
        } else {
            g->unk_1BC[0].unk_0C.f.b2 = 1;
            D_800CA148 = 4;
            func_8004284C();
        }
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x5A;
            break;
        case 2:
            D_800CA148 = 0x5F;
            break;
        case 3:
            D_800CA148 = 0x64;
            break;
        }
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_80042940(0xC);
        } else {
            func_8004284C();
        }
        break;
    case 11:
        func_80085A60();
        break;
    case 12:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            func_8007E5A0();
        }
        break;
    case 13:
        ((BunkakenModeFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkakenModeFlags *)&D_800E7D4F)->mode = 0;
        g->unk_720 = g->unk_1108;
        g->unk_721 = g->unk_1109;
        g->unk_722 = g->unk_110A + 1;
        func_80042908(0x8D);
        break;
    case 14:
        D_80122E98 = 1;
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        func_80042908(0x8C);
        break;
    case 15:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        func_80064E84();
        func_800649D4();
        func_80084E4C();
        func_800847B8(8U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 16:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_800573AC();
            func_80057390(0);
            func_8005742C(0x4105, 1);
            func_8004284C();
        }
        break;
    case 17:
        if (func_800460CC() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 18:
        func_800866A0(0x544A);
        break;
    case 19:
        func_8007C740();
        g->unk_F5F = 0;
        func_80083474();
        func_80046318(0x17U, 0x801E8000, 0x49EB);
        break;
    case 20:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8007E81C();
        } else {
            func_8007E884();
        }
        break;
    case 21:
        if (func_800460CC() & 1) {
            func_8014E3DC();
            temp_v0_3 = func_8008667C(0);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x5B;
                break;
            case 2:
                D_800CA148 = 0x60;
                break;
            case 3:
                D_800CA148 = 0x65;
                break;
            }
            if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
                D_800CA148 = 0x6A;
            }
        }
        break;
    case 22:
        func_80085A60();
        break;
    case 23:
        func_8004284C();
        temp_v0_4 = func_8008667C(0);
        switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 0x5C;
            break;
        case 2:
            D_800CA148 = 0x61;
            break;
        case 3:
            D_800CA148 = 0x66;
            break;
        }
        if ((u8) D_800E7D4E < 2U) {
            D_800CA148 += 1;
        }
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_80042940(0x1B);
        }
        break;
    case 24:
        func_80085A60();
        break;
    case 25:
        temp_v0_5 = func_8008667C(0);
        switch (temp_v0_5) {
        case 0:
        case 1:
            D_800CA148 = 0x5E;
            break;
        case 2:
            D_800CA148 = 0x63;
            break;
        case 3:
            D_800CA148 = 0x68;
            break;
        }
        func_8004284C();
        break;
    case 26:
        func_80085A60();
        break;
    case 27:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8007E934();
        } else {
            func_8007E99C();
        }
        break;
    case 28:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 29:
        func_80085A60();
        break;
    case 30:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014A6A0", func_8014CB4C);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014A6A0", func_8014D0FC);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014A6A0", func_8014D65C);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014A6A0", func_8014DB34);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014A6A0", func_8014DFB8);

void func_8014E3DC(void) {
    func_800847B8(0);
    func_80132034();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
