#include "common.h"
#include "ovl/BUNKASAI.h"

void func_80154018(void) {
    ((BunkSelectWords *)&D_80122EB8)->unk_04 = 1;
    switch (((BunkSelectWords *)&D_80122EB8)->unk_00) {
    case 0:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(0x1E);
            return;
        }
        func_80042908(0x1D);
        return;
    case 1:
        func_80042908(0x1F);
        return;
    case 2:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x20);
            return;
        case 0x60:
            func_80042908(0x21);
            return;
        case 0x61:
            func_80042908(0x22);
            return;
        }
        break;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x23);
            return;
        case 0x60:
            func_80042908(0x25);
            return;
        case 0x61:
            func_80042908(0x22);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x26);
            return;
        case 0x60:
            func_80042908(0x27);
            return;
        case 0x61:
            func_80042908(0x28);
            return;
        }
        break;
    case 5:
        func_80042908(0x29);
        return;
    default:
        func_80046500();
        return;
    }
}

void func_801541D8(void) {
    GameState *state;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_7;
    u32 temp_v0;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x5D41);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 1;
            func_80083474();
            if (D_800E6280.unk_03E == 0x5F) {
                D_800CA148 = 0x24;
            } else {
                D_800CA148 = 0x25;
            }
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        func_80046318(0x14U, 0x801E8000, 0x4A83);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(1);
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
            case 4:
                if (D_800E6280.unk_03E == 0x5F) {
                    D_800CA148 = 0x27;
                } else {
                    D_800CA148 = 0x28;
                }
                break;
            }
            func_80158DD8();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (func_8008667C(1) == 4) {
            func_80042940(0x14);
        } else if (((CharFlags *)&D_800E6280.unk_1BC[1].unk_0C)->b2) {
            D_800CA148 = 0xB;
            func_8004284C();
        } else {
            D_800CA148 = 6;
            func_80042940(0xD);
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = func_8008667C(1);
        switch (temp_v0_2) {
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
        func_8004284C();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        temp_v0_3 = func_8008667C(1);
        switch (temp_v0_3) {
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
        if (D_800E6280.unk_03E == 0x60) {
            D_800CA148 += 3;
        }
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (!(D_800E6280.unk_1BC[1].unk_0A & 3)) {
            temp_v0_4 = func_8008667C(1);
            switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 0x24;
            break;
        case 2:
            D_800CA148 = 0x25;
            break;
        case 3:
            D_800CA148 = 0x26;
            break;
        }
        } else {
            temp_v0_5 = func_8008667C(1);
            switch (temp_v0_5) {
        case 0:
        case 1:
            D_800CA148 = 0x18;
            break;
        case 2:
            D_800CA148 = 0x19;
            break;
        case 3:
            D_800CA148 = 0x1A;
            break;
        }
        }
        func_8004284C();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        state = &D_800E6280;
        if (!(state->unk_1BC[1].unk_0A & 3)) {
            temp_v0_6 = func_8008667C(1);
            switch (temp_v0_6) {
            case 0:
            case 1:
                D_800CA148 = 0xD;
                break;
            case 2:
                D_800CA148 = 0xC;
                break;
            case 3:
                D_800CA148 = 0xC;
                break;
            }
        } else {
            temp_v0_7 = func_8008667C(1);
            switch (temp_v0_7) {
        case 0:
        case 1:
            D_800CA148 = 0x1B;
            break;
        case 2:
            D_800CA148 = 0x1C;
            break;
        case 3:
            D_800CA148 = 0x1D;
            break;
        }
            if (!(state->unk_1BC[1].unk_0C.f.b2)) {
                state->unk_1BC[1].unk_0C.f.b2 = 1;
                if (state->unk_03E == 0x5F) {
                    D_800CA148 += 3;
                } else {
                    D_800CA148 += 6;
                }
            }
        }
        func_8004284C();
        break;
    case 19:
        if (!(D_800E6280.unk_1BC[1].unk_0A & 3)) {
            normal_date_girl_out();
        } else {
            normal_date_speak();
        }
        break;
    case 20:
        if (!(D_800E6280.unk_1BC[1].unk_0A & 3) && (func_8008667C(1) < 4)) {
            normal_date_speak();
        } else {
            normal_date_girl_out();
        }
        break;
    case 21:
        normal_date_bg_fadeout();
        break;
    case 22:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        if (func_8008667C(1) != 4) {
            if (D_800E6280.unk_03E == 0x5F) {
                ((CharFlags *)&D_800E6280.unk_1BC[1].unk_0C)->b12 = 1;
            } else {
                ((CharFlags *)&D_800E6280.unk_1BC[1].unk_0C)->b13 = 1;
            }
        }
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801548C8(void) {
    u8 year;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;
    u32 temp_v0_4;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_8014338C();
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        D_800CA148 = 0x26;
        func_8004284C();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EAC = 0;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042878(0x80);
        break;
    case 8:
        func_800866A0(0x5D41);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 1;
        func_80083474();
        func_80046318(0x14U, 0x801E8000, 0x4A83);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(1);
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
            case 4:
                D_800CA148 = 4;
                break;
            }
            func_80158DD8();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = func_8008667C(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
        case 2:
        case 3:
            D_800CA148 = 7;
            break;
        case 4:
            D_800CA148 = 8;
            break;
        }
        func_8004284C();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        temp_v0_3 = func_8008667C(1);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 0x29;
            break;
        case 2:
            D_800CA148 = 0x2A;
            break;
        case 3:
            D_800CA148 = 0x2B;
            break;
        case 4:
            D_800CA148 = 0x2C;
            break;
        }
        if (!(D_800E6280.unk_1BC[1].unk_0A & 3)) {
            D_800CA148 += 8;
        } else if (!(((CharFlags *)&D_800E6280.unk_1BC[1].unk_0C)->b2)) {
            D_800CA148 += 4;
        }
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (!(D_800E6280.unk_1BC[1].unk_0A & 3)) {
            temp_v0_4 = func_8008667C(1);
            switch (temp_v0_4) {
            case 0:
            case 1:
                D_800CA148 = 0xD;
                break;
            case 2:
            case 3:
                D_800CA148 = 0xC;
                break;
            case 4:
                D_800CA148 = 0xE;
                break;
            }
        } else {
            D_800E6280.unk_1BC[1].unk_0C.f.b2 = 1;
        }
        func_8004284C();
        break;
    case 18:
        normal_date_girl_out();
        break;
    case 19:
        if (!(D_800E6280.unk_1BC[1].unk_0A & 3)) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 20:
        normal_date_bg_fadeout();
        break;
    case 21:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80154D64(void) {
    GameState *state;
    s32 temp_v0_4;
    s32 temp_v0_5;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;
    u32 temp_v0_6;

    state = &D_800E6280;
    switch (state->unk_110A) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_8014338C();
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        if (state->unk_03E == 0x5F) {
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
        D_80122EB0 = (s32) state->unk_1109;
        D_80122EB4 = state->unk_110A + 1;
        func_80042878(0x80);
        break;
    case 7:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 8:
        if (func_800460CC() & 1) {
            func_800866A0(0x5D41);
        }
        break;
    case 9:
        dec_bg_show_set(1, func_8005751C(1));
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        state->unk_F5F = 1;
        func_80083474();
        func_80046318(0x14U, 0x801E8000, 0x4A83);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(1);
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
            case 4:
                D_800CA148 = 4;
                break;
            }
            func_80158DD8();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = func_8008667C(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
        case 2:
        case 3:
            D_800CA148 = 7;

            break;
        case 4:
            D_800CA148 = 8;
            break;
        }
        func_8004284C();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        func_8004284C();
        temp_v0_3 = func_8008667C(1);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = (state->unk_03E * 3) - 0xE8;
            break;
        case 2:
            D_800CA148 = (state->unk_03E * 3) - 0xE7;
            break;
        case 3:
            D_800CA148 = (state->unk_03E * 3) - 0xE6;
            break;
        case 4:
            func_80042940(0x1A);
            break;
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        temp_v0_4 = func_8008667C(1);
        switch (temp_v0_4) {
        default:

            break;
        case 0:
        case 1:
            D_800CA148 = 0x3E;
            break;
        case 2:
            D_800CA148 = 0x3F;
            break;
        case 3:
            D_800CA148 = 0x40;
            break;
        }
        if (!(state->unk_1BC[1].unk_0A & 3)) {
            D_800CA148 = D_800CA148 + 6;
        } else {

            if (state->unk_03E == 0x61) {
                D_800CA148 = D_800CA148 + 3;
            }
        }
        func_8004284C();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if (!(state->unk_1BC[1].unk_0A & 3)) {
            func_80042940(0x1A);
        } else {
            temp_v0_5 = func_8008667C(1);
            switch (temp_v0_5) {
            default:

                break;
            case 0:
            case 1:
                D_800CA148 = 0x47;
                break;
            case 2:
                D_800CA148 = 0x48;
                break;
            case 3:
                D_800CA148 = 0x49;
                break;
            }
            if (!(((CharFlags *)&state->unk_1BC[1].unk_0C)->b2)) {
                D_800CA148 += 3;
            }

            if (state->unk_03E == 0x61) {
                D_800CA148 = D_800CA148 + 6;
            }
            state->unk_1BC[1].unk_0C.f.b2 = 1;
            func_8004284C();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        normal_date_girl_out();
        break;
    case 22:
        normal_date_bg_fadeout();
        break;
    case 23:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        func_80142BC8();
        break;
    case 26:
        normal_date_girl_out();
        break;
    case 27:
        temp_v0_6 = func_8008667C(1);
        switch (temp_v0_6) {
        case 0:
        case 1:
            D_800CA148 = 0xD;

            break;
        case 2:
        case 3:
            D_800CA148 = 0xC;
            break;
        case 4:
            D_800CA148 = 0xE;
            break;
        }
        func_8004284C();
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        func_80042940(0x16);
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80155388(void) {
    GameState *state;
    s32 temp_v0_2;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_7;
    u32 temp_v0;
    u32 temp_v0_3;
    u32 temp_v0_4;

    state = &D_800E6280;
    switch (state->unk_110A) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x5D41);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            state->unk_F5F = 1;
            func_80083474();
            D_800CA148 = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        func_80046318(0x14U, 0x801E8000, 0x4A83);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(1);
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
            case 4:
                D_800CA148 = 5;
                break;
            }
            func_80158DD8();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if ((state->unk_1BC[1].unk_0C.f.b2) && (func_8008667C(1) != 4)) {
            if (((u8) state->unk_1BC[1].unk_0C.b[2] >> 4) == 0) {
                D_800CA148 = 0xA;
            } else {
                D_800CA148 = 0xB;
            }

            func_8004284C();
        } else {
            func_80042940(0xE);
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = func_8008667C(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x53;
            break;
        case 2:
            D_800CA148 = 0x54;
            break;
        case 3:
            D_800CA148 = 0x55;
            break;
        }
        func_8004284C();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        temp_v0_3 = func_8008667C(1);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 0x56;
            break;
        case 2:
            D_800CA148 = 0x57;
            break;
        case 3:
            D_800CA148 = 0x58;
            break;
        case 4:
            D_800CA148 = 0x59;
            break;
        }
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_bggirl_fadeout();
        break;
    case 17:
        D_80122EAC = 0;
        D_80122EB0 = (s32) state->unk_1109;
        D_80122EB4 = state->unk_110A + 1;
        func_80042878(0x80);
        break;
    case 18:
        func_800866A0(0x5D41);
        break;
    case 19:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        state->unk_F5F = 1;
        func_80083474();
        func_80046318(0x14U, 0x801E8000, 0x4A83);
        break;
    case 20:
        normal_date_bg_fadein();
        break;
    case 21:
        if (func_800460CC() & 1) {
            temp_v0_4 = func_8008667C(1);
            switch (temp_v0_4) {
            case 0:
            case 1:
                D_800CA148 = 0x5A;
                break;
            case 2:
                D_800CA148 = 0x5C;
                break;
            case 3:
                D_800CA148 = 0x5E;
                break;
            case 4:
                D_800CA148 = 0x60;
                break;
            }
            func_80158DD8();
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        if (func_8008667C(1) == 4) {
            func_80042940(0x1E);
        } else {
            normal_date_girl_in();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        temp_v0_5 = func_8008667C(1);
        switch (temp_v0_5) {
        case 0:
        case 1:
            D_800CA148 = 0x61;
            break;
        case 2:
            D_800CA148 = 0x62;
            break;
        case 3:
            D_800CA148 = 0x63;
            break;
        }
        func_8004284C();
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        if (!(state->unk_1BC[1].unk_0A & 3)) {
            temp_v0_6 = func_8008667C(1);
            switch (temp_v0_6) {
            case 0:
            case 1:
                D_800CA148 = 0x70;
                break;
            case 2:
                D_800CA148 = 0x71;
                break;
            case 3:
                D_800CA148 = 0x72;
                break;
            }
        } else {
            temp_v0_7 = func_8008667C(1);
            switch (temp_v0_7) {
            case 0:
            case 1:
                D_800CA148 = 0x64;
                break;
            case 2:
                D_800CA148 = 0x65;
                break;
            case 3:
                D_800CA148 = 0x66;
                break;
            }
            if (!state->unk_1BC[1].unk_0C.f.b2) {
                D_800CA148 += 3;
            }

            /* FAKE: dead result test supplies a live range without code,
             * keeping the message in a0 and the flag load temporary. T-10040. */
            if (temp_v0_7) {}
            if ((((u8) state->unk_1BC[1].unk_0C.b[2]) >> 4) == 1) {
                D_800CA148 = D_800CA148 + 6;
            }
            state->unk_1BC[1].unk_0C.f.b2 = 1;
        }
        func_8004284C();
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        normal_date_girl_out();
        if (((u8) state->unk_1BC[1].unk_0C.b[2] >> 4) == 1) {
            D_800CA148 = 0xC;
        } else if (func_8008667C(1) < 2) {
            D_800CA148 = 0xD;
        } else {
            D_800CA148 = 0xC;
        }
        break;
    case 30:
        if (func_8008667C(1) < 4) {
            normal_date_speak();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 31:
        if (func_8008667C(1) < 4) {
            normal_date_bg_fadeout();
        } else {
            func_8004284C();
        }
        D_800CA148 = 0;
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80155B1C(void) {
    GameState *state;
    s32 temp_v0_2;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_7;
    u32 temp_v0;
    u32 temp_v0_3;
    u32 temp_v0_4;

    state = &D_800E6280;
    switch (state->unk_110A) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x5D41);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            state->unk_F5F = 1;
            func_80083474();
            D_800CA148 = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        func_80046318(0x14U, 0x801E8000, 0x4A83);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(1);
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
            case 4:
                D_800CA148 = 5;
                break;
            }
            func_80158DD8();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if ((state->unk_1BC[1].unk_0C.f.b2) && (func_8008667C(1) != 4)) {
            if (((u8) state->unk_1BC[1].unk_0C.b[2] >> 4) == 0) {
                D_800CA148 = 0xA;
            } else {
                D_800CA148 = 0xB;
            }

            func_8004284C();
        } else {
            func_80042940(0xE);
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = func_8008667C(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x53;
            break;
        case 2:
            D_800CA148 = 0x54;
            break;
        case 3:
            D_800CA148 = 0x55;
            break;
        }
        func_8004284C();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        temp_v0_3 = func_8008667C(1);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 0x56;

            break;
        case 2:
            D_800CA148 = 0x57;
            break;
        case 3:
            D_800CA148 = 0x58;
            break;
        case 4:
            D_800CA148 = 0x59;
            break;
        }
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_bggirl_fadeout();
        break;
    case 17:
        D_80122EAC = 0;
        D_80122EB0 = (s32) state->unk_1109;
        D_80122EB4 = state->unk_110A + 1;
        func_80042878(0x80);
        break;
    case 18:
        func_800866A0(0x5D41);
        break;
    case 19:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        state->unk_F5F = 1;
        func_80083474();
        func_80046318(0x14U, 0x801E8000, 0x4A83);
        break;
    case 20:
        if (func_8008667C(1) == 4) {
            normal_date_bg_fadein();
        } else {
            func_8007E884();
        }
        break;
    case 21:
        if (func_800460CC() & 1) {
            temp_v0_4 = func_8008667C(1);
            switch (temp_v0_4) {
            case 0:
            case 1:
                D_800CA148 = 0x73;

                break;
            case 2:
                D_800CA148 = 0x74;
                break;
            case 3:
                D_800CA148 = 0x75;
                break;
            case 4:
                D_800CA148 = 0x76;
                break;
            }
            func_80158DD8();
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        if (func_8008667C(1) == 4) {
            func_80042940(0x1E);
        } else {
            func_8004284C();
        }
        break;
    case 24:
        temp_v0_5 = func_8008667C(1);
        switch (temp_v0_5) {
        case 0:
        case 1:
            D_800CA148 = 0x77;
            break;
        case 2:
            D_800CA148 = 0x78;
            break;
        case 3:
            D_800CA148 = 0x79;
            break;
        }
        if (!(state->unk_1BC[1].unk_0A & 3)) {
            D_800CA148 += 0xF;
        }
        func_8004284C();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        if (!(state->unk_1BC[1].unk_0A & 3)) {
            temp_v0_6 = func_8008667C(1);
            switch (temp_v0_6) {
            case 0:
            case 1:
                D_800CA148 = 0xD;
                break;
            case 2:
            case 3:
                D_800CA148 = 0xC;
                break;
            }
            func_80042940(0x1C);
        } else {
            temp_v0_7 = func_8008667C(1);
            switch (temp_v0_7) {
            case 0:
            case 1:
                D_800CA148 = 0x7A;
                break;
            case 2:
                D_800CA148 = 0x7B;
                break;
            case 3:
                D_800CA148 = 0x7C;
                break;
            }
            if (!(state->unk_1BC[1].unk_0C.f.b2)) {
                D_800CA148 += 3;
            }

            if (((u8) state->unk_1BC[1].unk_0C.b[2] >> 4) == 1) {
                D_800CA148 = D_800CA148 + 6;
            }
            state->unk_1BC[1].unk_0C.f.b2 = 1;
            func_8004284C();
        }
        break;
    case 27:
        normal_date_speak();
        break;
    case 28:
        normal_date_girl_out();
        break;
    case 29:
        if (!(state->unk_1BC[1].unk_0A & 3)) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 30:
        normal_date_bg_fadeout();
        break;
    case 31:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80156284(void) {
    GameState *state;
    s32 temp_v0_2;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_7;
    u32 temp_v0;
    u32 temp_v0_3;
    u32 temp_v0_4;

    state = &D_800E6280;
    switch (state->unk_110A) {
    case 0:
        if (((u16) state->unk_0F4.h >> 0xC) == 2) {
            dec_bg_cd_read(0x40FC, 1);
        } else {
            dec_bg_cd_read(0x4105, 1);
        }
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x5D41);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            state->unk_F5F = 1;
            func_80083474();
            D_800CA148 = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        func_80046318(0x14U, 0x801E8000, 0x4A83);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(1);
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
            case 4:
                D_800CA148 = 5;
                break;
            }
            func_80158DD8();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if ((state->unk_1BC[1].unk_0C.f.b2) && (func_8008667C(1) < 4)) {
            if (((u8) state->unk_1BC[1].unk_0C.b[2] >> 4) == 0) {
                D_800CA148 = 0xA;
            } else {
                D_800CA148 = 0xB;
            }

            func_8004284C();
        } else {
            func_80042940(0xE);
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = func_8008667C(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x53;
            break;
        case 2:
            D_800CA148 = 0x54;
            break;
        case 3:
            D_800CA148 = 0x55;
            break;
        }
        func_8004284C();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        temp_v0_3 = func_8008667C(1);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 0x56;

            break;
        case 2:
            D_800CA148 = 0x57;
            break;
        case 3:
            D_800CA148 = 0x58;
            break;
        case 4:
            D_800CA148 = 0x59;
            break;
        }
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_bggirl_fadeout();
        break;
    case 17:
        if (((u16) state->unk_0F4.h >> 0xC) == 2) {
            D_80122EAC = 0;
        } else {
            D_80122EAC = 0;
        }
        D_80122EB0 = (s32) state->unk_1109;
        D_80122EB4 = state->unk_110A + 1;
        func_80042878(0x80);
        break;
    case 18:
        func_800866A0(0x5D41);
        break;
    case 19:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        state->unk_F5F = 1;
        func_80083474();
        func_80046318(0x14U, 0x801E8000, 0x4A83);
        break;
    case 20:
        normal_date_bg_fadein();
        break;
    case 21:
        if (func_800460CC() & 1) {
            temp_v0_4 = func_8008667C(1);
            switch (temp_v0_4) {
            case 0:
            case 1:
                D_800CA148 = 0x89;

                break;
            case 2:
                D_800CA148 = 0x8B;
                break;
            case 3:
                D_800CA148 = 0x8D;
                break;
            case 4:
                D_800CA148 = 0x8F;
                break;
            }
            func_80158DD8();
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        if (func_8008667C(1) == 4) {
            func_80042940(0x1E);
        } else {
            normal_date_girl_in();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        if (!(state->unk_1BC[1].unk_0A & 3)) {
            temp_v0_5 = func_8008667C(1);
            switch (temp_v0_5) {
            case 0:
            case 1:
                D_800CA148 = 0x9C;
                break;
            case 2:
                D_800CA148 = 0x9D;
                break;
            case 3:
                D_800CA148 = 0x9E;
                break;
            }
        } else {
            temp_v0_6 = func_8008667C(1);
            switch (temp_v0_6) {
            case 0:
            case 1:
                D_800CA148 = 0x90;
                break;
            case 2:
                D_800CA148 = 0x91;
                break;
            case 3:
                D_800CA148 = 0x92;
                break;
            }
            if (!(state->unk_1BC[1].unk_0C.f.b2)) {
                D_800CA148 += 3;
            }

            if (((u8) state->unk_1BC[1].unk_0C.b[2] >> 4) == 1) {
                D_800CA148 = D_800CA148 + 6;
            }
            state->unk_1BC[1].unk_0C.f.b2 = 1;
        }
        func_8004284C();
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        normal_date_girl_out();
        break;
    case 28:
        temp_v0_7 = func_8008667C(1);
        switch (temp_v0_7) {
            case 0:
            case 1:
                D_800CA148 = 0xD;
                break;
            case 2:
            case 3:
                D_800CA148 = 0xC;
                break;
            }
        func_8004284C();
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        normal_date_bg_fadeout();
        break;
    case 31:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801569BC(void) {
    GameState *state;
    s32 temp_v0_6;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;
    u32 temp_v0_4;
    u32 temp_v0_5;

state = &D_800E6280;
    switch (state->unk_110A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x5D41);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            D_800CA148 = 0x2A;
            addr_init_bustup();
            state->unk_F5F = 1;
            func_80083474();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        func_80046318(0x12U, 0x801E8000, 0x4A97);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(1);
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
            case 4:
                D_800CA148 = 5;
                break;
            }
            func_80158E3C();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = func_8008667C(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0xE;

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
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_girl_out();
        break;
    case 13:
        if (func_8008667C(1) == 4) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 14:
        func_80044750(0x202);
        D_80122EB0 = (s32) state->unk_1109;
        D_80122EB4 = state->unk_110A + 1;
        func_80042808();
        break;
    case 15:
        func_80046318(0x12U, 0x801E8000, 0x4A97);
        func_8004284C();
        break;
    case 16:
        if (func_8008667C(1) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 17:
        if (func_800460CC() & 1) {
            temp_v0_3 = func_8008667C(1);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x13;

                break;
            case 2:
                D_800CA148 = 0x16;
                break;
            case 3:
                D_800CA148 = 0x19;
                break;
            case 4:
                D_800CA148 = 0x1C;
                break;
            }
            D_800CA148 += D_80122EA8;
            func_80158E3C();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        func_8004284C();
        if (!(state->unk_1BC[1].unk_0A & 3)) {
            temp_v0_4 = func_8008667C(1);
            switch (temp_v0_4) {
            case 0:
            case 1:
                D_800CA148 = 0x2E;
                break;
            case 2:
                D_800CA148 = 0x2F;
                break;
            case 3:
                D_800CA148 = 0x30;
                break;
            case 4:
                func_80042940(0x1A);
                break;
            }
        } else {
            temp_v0_5 = func_8008667C(1);
            switch (temp_v0_5) {
            case 0:
            case 1:
                D_800CA148 = 0x1F;
                break;
            case 2:
                D_800CA148 = 0x20;
                break;
            case 3:
                D_800CA148 = 0x21;
                break;
            case 4:
                func_80042940(0x1A);
                break;
            }
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (!(state->unk_1BC[1].unk_0A & 3)) {
            func_80042940(0x17);
        } else {
            temp_v0_6 = func_8008667C(1);
            switch (temp_v0_6) {
            case 0:
            case 1:
                D_800CA148 = 0x22;
                break;
            case 2:
                D_800CA148 = 0x23;
                break;
            case 3:
                D_800CA148 = 0x24;
                break;
            }
            if (!(state->unk_1BC[1].unk_0C.f.b2)) {
                D_800CA148 += 3;
            }

            if (((u8) state->unk_1BC[1].unk_0C.b[2] >> 4) == 1) {
                D_800CA148 = D_800CA148 + 6;
            }
            func_8004284C();
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        normal_date_girl_out();
        break;
    case 24:
        if (!(state->unk_1BC[1].unk_0A & 3)) {
            if (func_8008667C(1) < 2) {
                D_800CA148 = 0xD;
            } else {
                D_800CA148 = 0xC;
            }

            func_8004284C();
        } else {
            state->unk_1BC[1].unk_0C.f.b2 = 1;
            func_80042940(0x1A);
        }
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        normal_date_bg_fadeout();
        break;
    case 27:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8015706C(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;
    u32 temp_v0_4;
    u32 temp_v0_5;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        func_800866A0(0x5D41);
        break;
    case 4:
        addr_init_bustup();
        D_800E6280.unk_F5F = 1;
        func_80083474();
        func_801431D4();
        D_800CA148 = 0x2A;
        break;
    case 5:
        normal_date_bg_fadein();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        func_80046318(0x12U, 0x801E8000, 0x4A97);
        func_8004284C();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(1);
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
            case 4:
                D_800CA148 = 5;
                break;
            }
            func_80158E3C();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        temp_v0_2 = func_8008667C(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x31;
            break;
        case 2:
            D_800CA148 = 0x39;
            break;
        case 3:
            D_800CA148 = 0x41;
            break;
        case 4:
            D_800CA148 = 0x49;
            break;
        }
        func_8004284C();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (func_8008667C(1) == 4) {
            normal_date_girl_out();
        } else {
            func_80042940(0xF);
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (func_8008667C(1) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 16:
        ((BunkasaiModeFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkasaiModeFlags *)&D_800E7D4F)->mode = 0;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        func_80042908(0xA4);
        break;
    case 17:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0xA3);
        break;
    case 18:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        func_80084E4C();
        func_800847B8(1U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 19:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            func_8004284C();
        }
        break;
    case 20:
        if (func_800460CC() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 21:
        func_800866A0(0x5D41);
        break;
    case 22:
        addr_init_bustup();
        D_800E6280.unk_F5F = 1;
        func_80083474();
        func_80046318(0x12U, 0x801E8000, 0x4A97);
        break;
    case 23:
        if (func_8008667C(1) == 4) {
            normal_date_bg_fadein();
        } else {
            func_8007E884();
        }
        break;
    case 24:
        if (func_800460CC() & 1) {
            temp_v0_3 = func_8008667C(1);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x32;
                break;
            case 2:
                D_800CA148 = 0x3A;
                break;
            case 3:
                D_800CA148 = 0x42;
                break;
            case 4:
                D_800CA148 = 0x4B;
                break;
            }
            func_80158E3C();
        }
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        func_8004284C();
        if (!(D_800E6280.unk_1BC[1].unk_0A & 3)) {
            temp_v0_4 = func_8008667C(1);
            switch (temp_v0_4) {
            case 0:
            case 1:
                D_800CA148 = 0x38;
                break;
            case 2:
                D_800CA148 = 0x40;
                break;
            case 3:
                D_800CA148 = 0x48;
                break;
            case 4:
                func_80042940(0x1F);
                break;
            }
        } else {
            temp_v0_5 = func_8008667C(1);
            switch (temp_v0_5) {
            case 0:
            case 1:
                D_800CA148 = 0x33;
                break;
            case 2:
                D_800CA148 = 0x3B;
                break;
            case 3:
                D_800CA148 = 0x43;
                break;
            case 4:
                func_80042940(0x1F);
                break;
            }
        }
        break;
    case 27:
        normal_date_speak();
        break;
    case 28:
        if (!(D_800E6280.unk_1BC[1].unk_0A & 3)) {
            normal_date_girl_out();
        } else {
            if (!(((CharFlags *)&D_800E6280.unk_1BC[1].unk_0C)->b2)) {
                D_800CA148 += 1;
            }
            if (((u8) D_800E6280.unk_1BC[1].unk_0C.b[2] >> 4) == 1) {
                D_800CA148 += 2;
            }
            func_8004284C();
        }
        break;
    case 29:
        if (!(D_800E6280.unk_1BC[1].unk_0A & 3)) {
            if (func_8008667C(1) < 2) {
                D_800CA148 = 0xD;
            } else {
                D_800CA148 = 0xC;
            }
            func_8004284C();
        } else {
            normal_date_speak();
        }
        break;
    case 30:
        if (!(D_800E6280.unk_1BC[1].unk_0A & 3)) {
            normal_date_speak();
        } else {
            normal_date_girl_out();
        }
        break;
    case 31:
        normal_date_bg_fadeout();
        break;
    case 32:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 33:
        normal_date_speak();
        break;
    case 34:
        ((BunkasaiModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80157860(void) {
    GameState *game = &D_800E6280;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game->unk_110A) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x5D41);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            game->unk_F5F = 1;
            func_80083474();
            D_800CA148 = 0x2B;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        func_80046318(0x12U, 0x801E8000, 0x4A97);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(1);
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
            case 4:
                D_800CA148 = 5;
                break;
            }
            func_80158E3C();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = func_8008667C(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x4D;

            break;
        case 2:
            D_800CA148 = 0x50;
            break;
        case 3:
            D_800CA148 = 0x53;
            break;
        case 4:
            D_800CA148 = 0x56;
            break;
        }
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (func_8008667C(1) == 4) {
            func_80042940(0x12);
        } else {
            if ((u16) game->unk_66C[4].unk_00 < 0x64U) {
                D_800CA148 += 1;
            }
            func_8004284C();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if (!(game->unk_1BC[1].unk_0A & 3)) {
            temp_v0_3 = func_8008667C(1);
            switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 0x67;
            break;
        case 2:
            D_800CA148 = 0x68;
            break;
        case 3:
            D_800CA148 = 0x69;
            break;
        }
        } else {
            temp_v0_4 = func_8008667C(1);
            switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 0x58;
            break;
        case 2:
            D_800CA148 = 0x59;
            break;
        case 3:
            D_800CA148 = 0x5A;
            break;
        }
        }
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (!(game->unk_1BC[1].unk_0A & 3)) {
            if (func_8008667C(1) < 2) {
                D_800CA148 = 0xD;
            } else {
                D_800CA148 = 0xC;
            }

            func_80042940(0x12);
        } else {
            temp_v0_5 = func_8008667C(1);
            switch (temp_v0_5) {
            case 0:
            case 1:
                D_800CA148 = 0x5B;
                break;
            case 2:
                D_800CA148 = 0x5C;
                break;
            case 3:
                D_800CA148 = 0x5D;
                break;
            }
            if (!(game->unk_1BC[1].unk_0C.f.b2)) {
                D_800CA148 += 3;
            }

            /* FAKE: dead result test keeps the byte load temporary. T-10040. */
            if (temp_v0_5) {}
            if (((u8) game->unk_1BC[1].unk_0C.b[2] >> 4) == 1) {
                D_800CA148 = D_800CA148 + 6;
            }
            game->unk_1BC[1].unk_0C.f.b2 = 1;
            func_8004284C();
        }
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        normal_date_girl_out();
        break;
    case 19:
        if ((func_8008667C(1) == 4) || !(game->unk_1BC[1].unk_0A & 3)) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 20:
        normal_date_bg_fadeout();
        break;
    case 21:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80157DD4(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x5D41);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 1;
            func_80083474();
            D_800CA148 = 0x2C;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        func_80046318(0x12U, 0x801E8000, 0x4A97);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(1);
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
            case 4:
                D_800CA148 = 5;
                break;
            }
            func_80158E3C();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        D_800CA148 = 0x6A;
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = func_8008667C(1);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x6B;
            break;
        case 2:
            D_800CA148 = 0x6C;
            break;
        case 3:
            D_800CA148 = 0x6D;
            break;
        case 4:
            D_800CA148 = 0x6E;
            break;
        }
        func_8004284C();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_girl_out();
        break;
    case 15:
        D_800CA148 = 0x6F;
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
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

void func_801580EC(void) {
    GameState *state;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    u32 temp_v0;

state = &D_800E6280;
    switch (state->unk_110A) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x5D41);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            state->unk_F5F = 1;
            func_80083474();
            D_800CA148 = 0x2D;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        func_80046318(0x12U, 0x801E8000, 0x4A97);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(1);
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
            case 4:
                D_800CA148 = 5;
                break;
            }
            func_80158E3C();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (func_8008667C(1) == 4) {
            D_800CA148 = 0x70;
        } else if (!state->unk_1BC[1].unk_0C.f.b2) {
            D_800CA148 = 6;
        } else if ((state->unk_1BC[1].unk_0C.b[2] >> 4) == 0) {
            D_800CA148 = 0xA;
        } else {
            D_800CA148 = 0xB;
        }
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (func_8008667C(1) == 4) {
            func_80042940(0x14);
        } else if (((u32) (state->unk_1BC[1].unk_0C.w << 0x1D) >> 0x1F) == 1) {
            temp_v0_2 = func_8008667C(1);
            switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x72;
            break;
        case 2:
            D_800CA148 = 0x73;
            break;
        case 3:
            D_800CA148 = 0x74;
            break;
        }
            func_8004284C();
        } else {
            func_80042940(0xE);
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        temp_v0_3 = func_8008667C(1);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 0x75;
            break;
        case 2:
            D_800CA148 = 0x76;
            break;
        case 3:
            D_800CA148 = 0x77;
            break;
        }
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (!(state->unk_1BC[1].unk_0A & 3)) {
            temp_v0_4 = func_8008667C(1);
            switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 0x8A;
            break;
        case 2:
            D_800CA148 = 0x8B;
            break;
        case 3:
            D_800CA148 = 0x8C;
            break;
        }
        } else {
            temp_v0_5 = func_8008667C(1);
            switch (temp_v0_5) {
        case 0:
        case 1:
            D_800CA148 = 0x78;
            break;
        case 2:
            D_800CA148 = 0x79;
            break;
        case 3:
            D_800CA148 = 0x7A;
            break;
        }
        }
        func_8004284C();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if (!(state->unk_1BC[1].unk_0A & 3)) {
            func_80042940(0x14);
        } else {
            temp_v0_6 = func_8008667C(1);
            switch (temp_v0_6) {
            case 0:
            case 1:
                D_800CA148 = 0x7E;
                break;
            case 2:
                D_800CA148 = 0x7F;
                break;
            case 3:
                D_800CA148 = 0x80;
                break;
            }
            if (!(state->unk_1BC[1].unk_0C.f.b2)) {
                D_800CA148 += 3;
            }

            if (((u8) state->unk_1BC[1].unk_0C.b[2] >> 4) == 1) {
                D_800CA148 = D_800CA148 + 6;
            }
            state->unk_1BC[1].unk_0C.f.b2 = 1;
            func_8004284C();
        }
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        normal_date_girl_out();
        break;
    case 21:
        if (func_8008667C(1) == 4) {
            func_8004284C();
        } else if (!(state->unk_1BC[1].unk_0A & 3)) {
            if (func_8008667C(1) < 2) {
                D_800CA148 = 0xD;
            } else {
                D_800CA148 = 0xC;
            }

            func_8004284C();
        } else {
            func_80042940(0x17);
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        normal_date_bg_fadeout();
        break;
    case 24:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80154018", func_80158764);

void func_80158DD8(void) {
    func_800847B8(1);
    func_80132068();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

void func_80158E3C(void) {
    func_800847B8(1);
    func_8013209C();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
