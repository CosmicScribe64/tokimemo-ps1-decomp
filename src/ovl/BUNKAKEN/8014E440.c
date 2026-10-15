#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_8014E440(void) {
    switch (D_80122EB8) {
    case 0:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x63);
            return;
        case 0x60:
            func_80042908(0x64);
            return;
        case 0x61:
            func_80042908(0x65);
            return;
        default:
            func_80046500();
            return;
        }
        break;
    case 1:
        func_80042908(0x66);
        return;
    case 2:
        func_80042908(0x67);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x68);
            return;
        case 0x60:
            func_80042908(0x69);
            return;
        case 0x61:
            func_80042908(0x6A);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x6B);
            return;
        case 0x60:
            func_80042908(0x6C);
            return;
        case 0x61:
            func_80042908(0x6D);
            return;
        }
        break;
    case 5:
        func_80042908(0x6E);
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8014E5E0(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x40EA, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E99C();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_80150CC8();
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                D_800CA148 = 3;
                break;
            case 2:
                D_800CA148 = 5;
                func_80042940(0xE);
                break;
            case 3:
                D_800CA148 = 8;
                func_80042940(0x12);
                break;
            }
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        D_800CA148 = 1;
        func_8007E99C();
        break;
    case 7:
        func_80085A60();
        break;
    case 8:
        D_800CA148 = 4;
        func_8007E884();
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        func_8007E99C();
        break;
    case 11:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        func_80145530();
        break;
    case 14:
        func_80085A60();
        break;
    case 15:
        D_800CA148 = 2;
        func_8007E99C();
        break;
    case 16:
        func_80085A60();
        break;
    case 17:
        D_800CA148 = 6;
        func_8007E884();
        break;
    case 18:
        func_80085A60();
        break;
    case 19:
        func_8007E5A0();
        break;
    case 20:
        func_80085A60();
        break;
    case 21:
        func_8007E934();
        break;
    case 22:
        func_80042940(0xB);
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014E890(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x40EA, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E99C();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_80150CC8();
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                D_800CA148 = 0xA;
                break;
            case 2:
                D_800CA148 = 0xC;
                func_80042940(9);
                break;
            case 3:
                D_800CA148 = 0xD;
                func_80042940(0xE);
                break;
            }
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        D_800CA148 = 1;
        func_8007E99C();
        break;
    case 7:
        func_80085A60();
        break;
    case 8:
        D_800CA148 = 0xB;
        func_8007E884();
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        func_8007E99C();
        break;
    case 11:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        func_80145530();
        break;
    case 14:
        func_80085A60();
        break;
    case 15:
        func_8007E5A0();
        break;
    case 16:
        func_80085A60();
        break;
    case 17:
        func_8007E934();
        break;
    case 18:
        func_80042940(0xB);
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014EAF0(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x467C, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E99C();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E884();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                D_800CA148 = 0xF;
                break;
            case 2:
                D_800CA148 = 0x10;
                break;
            case 3:
                D_800CA148 = 0x11;
                break;
            }
            func_80150CC8();
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_8007E99C();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x5B25);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        func_8007C740();
        g->unk_F5F = 7;
        func_80083474();
        cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(7);
            switch (temp_v0_2) {                    /* switch 3; irregular */
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
            }
            func_80150CC8();
        }
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        if (func_8008667C(7) < 2) {
            func_80042940(0x10);
        } else {
            func_8007E5A0();
        }
        break;
    case 14:
        D_800CA148 = 0x15;
        func_8004284C();
        break;
    case 15:
        func_80085A60();
        break;
    case 16:
        if (func_8008667C(7) < 2) {
            func_8007E99C();
        } else {
            func_8007E934();
        }
        break;
    case 17:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 18:
        func_80085A60();
        break;
    case 19:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_8014EE54);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_8014F214);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_8014F624);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_8014F9D8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_8014FEF4);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_80150244);

void func_8015049C(void) {
    s32 temp_v0;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(7);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x56;
                break;
            case 2:
                D_800CA148 = 0x58;
                break;
            case 3:
                D_800CA148 = 0x5A;
                break;
            }
            func_80150CC8();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_out();
        break;
    case 7:
        D_800E6280.unk_F5F = 0xE;
        func_8004284C();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        normal_date_bg_fadeout();
        break;
    case 10:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_80150680);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_80150958);

void func_80150CC8(void) {
    func_800847B8(7);
    func_80132374();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
