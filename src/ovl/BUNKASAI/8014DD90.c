#include "common.h"
#include "ovl/BUNKASAI.h"

void func_8014DD90(void) {
    BunkaDispatch8 *dispatch = (BunkaDispatch8 *)&D_80122EB8;
    dispatch->unk_04 = 6;
    switch (dispatch->unk_00) {
    case 0:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(0x6F);
            return;
        }
        func_80042908(0x6E);
        return;
    case 1:
        func_80042908(0x70);
        return;
    case 2:
        func_80042908(0x71);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x72);
            return;
        case 0x60:
            func_80042908(0x74);
            return;
        case 0x61:
            func_80042908(0x71);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x75);
            return;
        case 0x60:
            func_80042908(0x76);
            return;
        case 0x61:
            func_80042908(0x77);
            return;
        }
        break;
    case 5:
        func_80042908(0x78);
        return;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8014DD90", func_8014DF04);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8014DD90", func_8014E2AC);

void func_8014E5EC(void) {
    s32 temp_v0_2;
    u32 temp_v0;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        if (D_800E6280.unk_03E == 0x5F) {
            D_800CA148 = 0x27;
        } else {
            D_800CA148 = 0x28;
        }
        break;
    case 4:
        normal_date_speak();
        break;
    case 5:
        normal_date_bg_fadeout();
        break;
    case 6:
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 7:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x622D);
        }
        break;
    case 9:
        dec_bg_show_set(1, func_8005751C(1));
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 6;
        func_80083474();
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = (D_800E6280.unk_03E * 3) - 0x10B;
                break;
            case 2:
                D_800CA148 = (D_800E6280.unk_03E * 3) - 0x10A;
                break;
            case 3:
                D_800CA148 = (D_800E6280.unk_03E * 3) - 0x109;
                break;
            case 4:
                D_800CA148 = 0x1B;
                break;
            }
            func_801508C4();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (func_8008667C(6) == 4) {
            normal_date_girl_out();
        } else {
            func_8004284C();
        }
        break;
    case 14:
        temp_v0_2 = func_8008667C(6);
        switch (temp_v0_2) {
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
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (func_8008667C(6) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8014DD90", func_8014E9D0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8014DD90", func_8014EF24);

void func_8014F36C(void) {
    s32 temp_v0_2;
    s32 temp_v0_3;
    u32 temp_v0;
    u32 temp_v0_4;
    u32 temp_v0_5;

    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        func_800866A0(0x622D);
        break;
    case 4:
        func_8007C740();
        D_800E6280.unk_F5F = 6;
        func_80083474();
        func_801431D4();
        D_800CA148 = 0x2A;
        break;
    case 5:
        func_8007E81C();
        break;
    case 6:
        func_80085A60();
        break;
    case 7:
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        func_8004284C();
        break;
    case 8:
        func_8007E390();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x40;
                break;
            case 2:
                D_800CA148 = 0x41;
                break;
            case 3:
                D_800CA148 = 0x42;
                break;
            case 4:
                D_800CA148 = 0x43;
                break;
            }
            func_801508C4();
        }
        break;
    case 10:
        func_80085A60();
        break;
    case 11:
        if (func_8008667C(6) == 4) {
            func_8007E5A0();
        } else {
            func_80042940(0xE);
        }
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        func_80042940(0x1B);
        break;
    case 14:
        temp_v0_2 = func_8008667C(6);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x45;
            break;
        case 2:
            D_800CA148 = 0x46;
            break;
        case 3:
            D_800CA148 = 0x47;
            break;
        }
        func_8004284C();
        break;
    case 15:
        func_80085A60();
        break;
    case 16:
        func_8007E99C();
        break;
    case 17:
        ((BunkasaiModeFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkasaiModeFlags *)&D_800E7D4F)->mode = 2;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        main_state_set(0xD0);
        break;
    case 18:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0xA3);
        break;
    case 19:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        func_80064E84();
        func_800649D4();
        func_80084E4C();
        func_800847B8(6U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 20:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_800573AC();
            func_80057390(0);
            func_8005742C(0x4105, 1);
            func_8004284C();
        }
        break;
    case 21:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 22:
        func_800866A0(0x622D);
        break;
    case 23:
        func_8007C740();
        D_800E6280.unk_F5F = 6;
        func_80083474();
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        break;
    case 24:
        func_8007E884();
        break;
    case 25:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = func_8008667C(6);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x4B;
                break;
            case 2:
                D_800CA148 = 0x4C;
                break;
            case 3:
                D_800CA148 = 0x4D;
                break;
            }
            func_801508C4();
        }
        break;
    case 26:
        func_80085A60();
        break;
    case 27:
        if (func_8008667C(6) == 4) {
            func_8004284C();
        } else {
            func_8007E5A0();
        }
        break;
    case 28:
        ((BunkasaiModeFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkasaiModeFlags *)&D_800E7D4F)->mode = 0;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        func_80042908(0xA4);
        break;
    case 29:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0xA3);
        break;
    case 30:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        func_80064E84();
        func_800649D4();
        func_80084E4C();
        func_800847B8(6U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 31:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_800573AC();
            func_80057390(0);
            func_8005742C(0x4105, 1);
            func_8004284C();
        }
        break;
    case 32:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 33:
        func_800866A0(0x622D);
        break;
    case 34:
        func_8007C740();
        D_800E6280.unk_F5F = 6;
        func_80083474();
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        break;
    case 35:
        if (func_8008667C(6) == 4) {
            func_8007E81C();
        } else {
            func_8007E884();
        }
        break;
    case 36:
        if (cd_read_sync_status() & 1) {
            temp_v0_4 = func_8008667C(6);
            switch (temp_v0_4) {
            case 0:
            case 1:
                D_800CA148 = 0x4E;
                break;
            case 2:
                D_800CA148 = 0x50;
                break;
            case 3:
                D_800CA148 = 0x52;
                break;
            case 4:
                D_800CA148 = 0x54;
                break;
            }
            func_801508C4();
            if ((u8) D_800E7D4E < 2U) {
                D_800CA148 += 1;
            }
        }
        break;
    case 37:
        func_80085A60();
        break;
    case 38:
        func_8004284C();
        temp_v0_5 = func_8008667C(6);
        switch (temp_v0_5) {
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
        case 4:
            func_80042940(0x29);
            break;
        }
        break;
    case 39:
        func_80085A60();
        break;
    case 40:
        func_8007E5A0();
        break;
    case 41:
        func_8007E934();
        break;
    case 42:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 43:
        func_80085A60();
        break;
    case 44:
        ((BunkasaiModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8014DD90", func_8014FC70);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8014DD90", func_8014FFE4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8014DD90", func_80150234);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8014DD90", func_8015052C);

void func_801508C4(void) {
    func_800847B8(6);
    func_801322A4();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
