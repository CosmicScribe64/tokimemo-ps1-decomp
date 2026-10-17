#include "common.h"
#include "ovl/BUNKASAI.h"

void func_801511E0(void) {
    ((BunkaDispatch8 *)&D_80122EB8)->unk_04 = 0;
    D_800E6280.unk_1BC[0].unk_0C.f.b2 = 1;
    switch (((BunkaDispatch8 *)&D_80122EB8)->unk_00) {
    case 0:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(3);
            return;
        }
        func_80042908(2);
        return;
    case 1:
        func_80042908(4);
        return;
    case 2:
        func_80042908(5);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(6);
            return;
        case 0x60:
            func_80042908(8);
            return;
        case 0x61:
            func_80042908(9);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0xA);
            return;
        case 0x60:
            func_80042908(0xB);
            return;
        case 0x61:
            func_80042908(0xC);
            return;
        }
        break;
    case 5:
        func_80042908(0xD);
        return;
    default:
        func_80046500();
        return;
    }
}

/* NON_MATCHING: T-10040, T-22012, 2 rows: in case 5 the two address `lui`s before the year compare come out in the other order (built: D_800CA148 then D_800E62BE; original: D_800E62BE then D_800CA148); everything else matches. Compound/plain add, year statement or in-condition, operand order, one-line forms, a tmp local, a pointer reassignment, viewsweep and autotweak did not move it. */
#ifdef NON_MATCHING
void func_80151364(void) {
    u8 year;
    GameState *game = &D_800E6280;
    u32 temp_v0;

    switch (game->unk_110A) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x544A);
            func_8004284C();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            addr_init_bustup();
            game->unk_F5F = 0;
            func_80083474();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 4:
        normal_date_bg_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 2;
                break;
            case 2:
                D_800CA148 = 4;
                break;
            case 3:
                D_800CA148 = 6;
                break;
            case 4:
                D_800CA148 = 8;
                break;
            }
            if ((year = game->unk_03E) == 0x60) {
                D_800CA148 += 0xC;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_girl_in();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        normal_date_bggirl_fadeout();
        D_800CA148 = 0;
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        func_8007E884();
        break;
    case 12:
        if (func_8008667C(0) == 4) {
            if ((u16) game->unk_66C[0].unk_00 >= (u32) (((year = game->unk_03E) * 0x64) - 0x24B8)) {
                D_800CA148 = 0xC;
            } else {
                D_800CA148 = 0xD;
            }
        } else {
            if ((u16) game->unk_66C[0].unk_00 >= (u32) (((year = game->unk_03E) * 0x64) - 0x24B8)) {
                D_800CA148 = 0xA;
            } else {
                D_800CA148 = 0xB;
            }
        }

        /* FAKE: pointer reassignment splits the year/base live ranges. T-10040. */
        if ((year = (game = &D_800E6280)->unk_03E) == 0x60) {
            D_800CA148 = D_800CA148 + 0xC;
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
        normal_date_bg_fadeout();
        break;
    case 16:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}
#else
INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/801511E0", func_80151364);
#endif

void func_801516B0(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x544A);
            func_8004284C();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            addr_init_bustup();
            D_800E6280.unk_F5F = 0;
            func_80083474();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 4:
        func_8007E884();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x1A;
                break;
            case 2:
                D_800CA148 = 0x1C;
                break;
            case 3:
                D_800CA148 = 0x1E;
                break;
            case 4:
                D_800CA148 = 0x20;
                break;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_bggirl_fadeout();
        break;
    case 8:
        D_80122EAC = 2;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 9:
        func_800866A0(0x544A);
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            D_800E6280.unk_F5F = 0;
            func_80083474();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 11:
        func_8007E884();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x1B;
                break;
            case 2:
                D_800CA148 = 0x1D;
                break;
            case 3:
                D_800CA148 = 0x1F;
                break;
            case 4:
                D_800CA148 = 0x21;
                break;
            }
            func_80153FA4();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_girl_out();
        break;
    case 15:
        normal_date_bggirl_fadeout();
        break;
    case 16:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80151A10(void) {
    u8 year_copy;
    u32 temp_v0;
    u32 temp_v0_2;

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
            func_80062CD0(0x5477);
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            addr_init_bustup();
            D_800E6280.unk_F5F = 0;
            func_80083474();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 4:
        func_8007E884();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            if (D_800E6280.unk_03E == 0x5F) {
                temp_v0 = func_8008667C(0);
                switch (temp_v0) {
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
                case 4:
                    D_800CA148 = 0x25;
                    break;
                }
            } else {
                temp_v0_2 = func_8008667C(0);
                switch (temp_v0_2) {
                case 0:
                case 1:
                    D_800CA148 = 0x26;

                    break;
                case 2:
                case 3:
                    D_800CA148 = 0x27;
                    break;
                case 4:
                    D_800CA148 = 0x28;
                    break;
                }
                D_800CA148 = (D_800CA148 + (D_800E6280.unk_03E * 3)) - 0x120;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if ((D_800E6280.unk_03E == 0x60) && (func_8008667C(0) == 4)) {
            D_800CA148 = 0xB1;
            normal_date_girl_out();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 8:
        if ((D_800E6280.unk_03E == 0x60) && (func_8008667C(0) == 4)) {
            normal_date_speak();
        } else {
            func_80042940(0xA);
        }
        break;
    case 9:
        normal_date_bg_fadeout();
        break;
    case 10:
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 11:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x5477);
        }
        break;
    case 13:
        dec_bg_show_set(1, func_8005751C(1));
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 0;
        func_80083474();
        cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        break;
    case 14:
        func_8007E884();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            if (func_8008667C(0) == 4) {
                if ((u16) D_800E6280.unk_66C[1].unk_00 >= (u32) (((year_copy = D_800E6280.unk_03E) * 0x64) - 0x24B8)) {
                    D_800CA148 = 0x2E;
                } else {
                    D_800CA148 = 0x2F;
                }
            } else {
                if ((u16) D_800E6280.unk_66C[1].unk_00 >= (u32) (((year_copy = D_800E6280.unk_03E) * 0x64) - 0x24B8)) {
                    D_800CA148 = 0x2C;
                } else {
                    D_800CA148 = 0x2D;
                }
            }

            if ((year_copy = D_800E6280.unk_03E) == 0x61) {
                D_800CA148 = D_800CA148 + 4;
            }
            func_80153FA4();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        normal_date_bg_fadeout();
        break;
    case 19:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80151F10(void) {
    u8 year;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80062CD0(0x54D1);
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            addr_init_bustup();
            D_800E6280.unk_F5F = 0;
            func_80083474();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 4:
        func_8007E884();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x34;

                break;
            case 2:
                D_800CA148 = 0x35;
                break;
            case 3:
                D_800CA148 = 0x36;
                break;
            case 4:
                D_800CA148 = 0x37;
                break;
            }

            if ((year = D_800E6280.unk_03E) == 0x60) {
                D_800CA148 += 0xC;
            }

            if ((year = D_800E6280.unk_03E) == 0x61) {
                D_800CA148 = D_800CA148 + 0x14;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_bggirl_fadeout();
        break;
    case 8:
        D_80122EAC = 2;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 9:
        func_800866A0(0x54D1);
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            D_800E6280.unk_F5F = 0;
            func_80083474();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 11:
        func_8007E884();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            if ((year = D_800E6280.unk_03E) == 0x5F) {
                temp_v0_2 = func_8008667C(0);
                switch (temp_v0_2) {
                case 0:
                    D_800CA148 = 0x39;

                    break;
                case 1:
                    D_800CA148 = 0x38;
                    break;
                case 2:
                    D_800CA148 = 0x3A;
                    break;
                case 3:
                    D_800CA148 = 0x3C;
                    break;
                case 4:
                    D_800CA148 = 0x3E;
                    break;
                }
                if (((u16) D_800E6280.unk_66C[2].unk_00 < 0x64U) && (func_8008667C(0) >= 2)) {
                    D_800CA148 += 1;
                }
            } else {
                temp_v0_3 = func_8008667C(0);
                switch (temp_v0_3) {
                case 0:
                case 1:
                    D_800CA148 = 0x44;

                    break;
                case 2:
                    D_800CA148 = 0x45;
                    break;
                case 3:
                    D_800CA148 = 0x46;
                    break;
                case 4:
                    D_800CA148 = 0x47;
                    break;
                }
                if ((year = D_800E6280.unk_03E) == 0x61) {
                    D_800CA148 += 8;
                }
            }
            func_80153FA4();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_girl_out();
        break;
    case 15:
        normal_date_bg_fadeout();
        break;
    case 16:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801523A0(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x544A);
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_801431D4();
            addr_init_bustup();
            D_800E6280.unk_F5F = 0;
            func_80083474();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 4:
        func_8007E884();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x50;
                break;
            case 2:
                D_800CA148 = 0x56;
                break;
            case 3:
                D_800CA148 = 0x5C;
                break;
            case 4:
                D_800CA148 = 0x62;
                break;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if (func_8008667C(0) == 4) {
            normal_date_girl_out();
        } else {
            normal_date_speak();
        }
        break;
    case 8:
        if (func_8008667C(0) == 4) {
            normal_date_speak();
        } else {
            normal_date_girl_out();
        }
        break;
    case 9:
        func_80044750(0x202);
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042808();
        break;
    case 10:
        cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        func_8004284C();
        break;
    case 11:
        if (func_8008667C(0) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            func_80153FA4();
            temp_v0_2 = func_8008667C(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x52;
                break;
            case 2:
                D_800CA148 = 0x58;
                break;
            case 3:
                D_800CA148 = 0x5E;
                break;
            case 4:
                D_800CA148 = 0x64;
                func_80042940(0xE);
                break;
            }
        }
        break;
    case 13:
        D_800CA148 += 1;
        func_8004284C();
        break;
    case 14:
        D_800CA148 += D_80122EA8;
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (func_8008667C(0) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        D_800CA148 = 1;
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

void func_801527A8(void) {
    /* FAKE: unused frame words preserve the observed 0x40 frame
     * and the real length spill at sp+0x36; original locals unknown. T-10040. */
    s32 frame_pad[2];
    s16 sp36;
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        func_800866A0(0x544A);
        break;
    case 4:
        addr_init_bustup();
        D_800E6280.unk_F5F = 0;
        func_80083474();
        func_801431D4();
        cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        break;
    case 5:
        func_8007E884();
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x67;
                break;
            case 2:
                D_800CA148 = 0x6C;
                break;
            case 3:
                D_800CA148 = 0x71;
                break;
            case 4:
                D_800CA148 = 0x76;
                break;
            }
            func_80153FA4();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (func_8008667C(0) == 4) {
            normal_date_girl_out();
        } else {
            func_80042940(0xB);
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        func_80042940(0x10);
        break;
    case 11:
        normal_date_two_select();
        break;
    case 12:
        if (D_80122CDC != 0) {
            normal_date_speak();
        } else {
            func_80042940(0x10);
        }
        break;
    case 13:
        normal_date_two_select();
        break;
    case 14:
        if (D_80122CDC != 0) {
            normal_date_speak();
        } else {
            func_80042940(0x10);
        }
        break;
    case 15:
        func_80042940(0x1B);
        break;
    case 16:
        if (func_8008667C(0) == 4) {
            func_8004284C();
        } else {
            switch (D_800E6280.unk_110D) {
            case 0:
                sp36 = func_800AE0E0(D_800E6280.unk_0D4);
                k_reset(1);
                set_kanji_string((s16) (-0x68 - (sp36 * 4)), 0x32, 0, (s32) D_800E6280.unk_0D4, 0);
                set_kanji_string(-0x56, 0x32, 0, (s32) "『それじゃ、やってみせるかな。", 0);
                k_disp_start(2);
                D_800E6280.unk_110D += 1;
                D_80160AD0 = 0;
                break;
            case 1:
                D_80160AD0 += 1;
                if ((D_80160AD0 >= 0x10) && (check_end_k() != 0)) {
                    D_8011ED5B ^= 0x80;
                    D_80160AD0 = 0;
                }
                if ((D_800E6280.unk_F88 & 0xF0) && (check_end_k() != 0)) {
                    k_reset(1);
                    D_8011ED5B &= 0x7F;
                    D_800E6280.unk_110D += 1;
                }
                break;
            case 2:
                normal_date_girl_out();
                break;
            }
        }
        break;
    case 17:
        ((Flags7D4F *)&D_800E7D4F)->b0 = 1;
        ((Flags7D4F *)&D_800E7D4F)->b1 = 1;
        ((Flags7D4F *)&D_800E7D4F)->unk_2_4 = 0;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        func_80042908(0xA4);
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
        message_window_init();
        hizuke_init();
        func_80084E4C();
        func_800847B8(0U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 20:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            func_8004284C();
        }
        break;
    case 21:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 22:
        func_800866A0(0x544A);
        break;
    case 23:
        addr_init_bustup();
        D_800E6280.unk_F5F = 0;
        func_80083474();
        cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        break;
    case 24:
        if (func_8008667C(0) == 4) {
            normal_date_bg_fadein();
        } else {
            func_8007E884();
        }
        break;
    case 25:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x6A;
                break;
            case 2:
                D_800CA148 = 0x6F;
                break;
            case 3:
                D_800CA148 = 0x74;
                break;
            case 4:
                D_800CA148 = 0xB2;
                break;
            }
            if (((u8) D_800E7D4E < 2U) && (func_8008667C(0) < 4)) {
                D_800CA148 += 1;
            }
            func_80153FA4();
        }
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        if (func_8008667C(0) == 4) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 28:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80152F08(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x544A);
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_801431D4();
            addr_init_bustup();
            D_800E6280.unk_F5F = 0;
            func_80083474();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 4:
        func_8007E884();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
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
            case 4:
                D_800CA148 = 0x7B;
                break;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_bggirl_fadeout();
        break;
    case 8:
        D_80122EAC = 2;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 9:
        func_800866A0(0x544A);
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            D_800E6280.unk_F5F = 0;
            func_80083474();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 11:
        func_8007E884();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x7C;
                break;
            case 2:
                D_800CA148 = 0x7D;
                break;
            case 3:
                D_800CA148 = 0x7E;
                break;
            case 4:
                D_800CA148 = 0x7F;
                break;
            }
            func_80153FA4();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_girl_out();
        break;
    case 15:
        normal_date_bg_fadeout();
        break;
    case 16:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80153260(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x54A4);
        }
        break;
    case 3:
        dec_bg_show_set(1, func_8005751C(1));
        func_801431D4();
        addr_init_bustup();
        D_800E6280.unk_F5F = 0;
        func_80083474();
        cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        break;
    case 4:
        func_8007E884();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x80;
                break;
            case 2:
                D_800CA148 = 0x81;
                break;
            case 3:
                D_800CA148 = 0x82;
                break;
            case 4:
                D_800CA148 = 0x83;
                break;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        D_800CA148 = 0;
        normal_date_bggirl_fadeout();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        func_8007E884();
        break;
    case 10:
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x84;
            break;
        case 2:
            D_800CA148 = 0x86;
            break;
        case 3:
            D_800CA148 = 0x86;
            break;
        case 4:
            D_800CA148 = 0x88;
            break;
        }
        if ((u16) D_800E6280.unk_66C[4].unk_00 < 0x64U) {
            D_800CA148 += 1;
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
        normal_date_bg_fadeout();
        break;
    case 14:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80153530(void) {
    u32 temp_v0;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x54A4);
        }
        break;
    case 3:
        dec_bg_show_set(1, func_8005751C(1));
        func_801431D4();
        addr_init_bustup();
        D_800E6280.unk_F5F = 0;
        func_80083474();
        cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        break;
    case 4:
        func_8007E884();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x8A;
                break;
            case 2:
                D_800CA148 = 0x8C;
                break;
            case 3:
                D_800CA148 = 0x8D;
                break;
            case 4:
                D_800CA148 = 0x8E;
                break;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_girl_out();
        break;
    case 8:
        if ((func_8008667C(0) == 2) || (func_8008667C(0) == 3)) {
            func_80042940(0xA);
        } else {
            func_8004284C();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if ((func_8008667C(0) == 2) || (func_8008667C(0) == 3)) {
            normal_date_girl_out();
        } else {
            func_8004284C();
        }
        break;
    case 11:
        normal_date_bg_fadeout();
        break;
    case 12:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801537BC(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80062CD0(0x54A4);
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            addr_init_bustup();
            D_800E6280.unk_F5F = 0;
            func_80083474();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 4:
        func_8007E884();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
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
            case 4:
                D_800CA148 = 0x93;
                break;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_bggirl_fadeout();
        break;
    case 8:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
        case 2:
        case 3:
            D_800CA148 = 0x94;
            break;
        case 4:
            D_800CA148 = 0x97;
            break;
        }
        if ((u16) D_800E6280.unk_66C[4].unk_00 < 0x12CU) {
            D_800CA148 += 1;
        }
        func_8004284C();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_girl_out();
        break;
    case 14:
        normal_date_bg_fadeout();
        break;
    case 15:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80153A98(void) {
    u8 year;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

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
            func_800866A0(0x544A);
        }
        break;
    case 3:
        dec_bg_show_set(1, func_8005751C(1));
        func_8014338C();
        addr_init_bustup();
        D_800E6280.unk_F5F = 0;
        func_80083474();
        cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        break;
    case 4:
        func_8007E884();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            if (D_800E6280.unk_03E == 0x5F) {
                temp_v0 = func_8008667C(0);
                switch (temp_v0) {
                case 0:
                case 1:
                    D_800CA148 = 0x99;
                    break;
                case 2:
                    D_800CA148 = 0x9A;
                    break;
                case 3:
                    D_800CA148 = 0x9B;
                    break;
                case 4:
                    D_800CA148 = 0x9C;
                    break;
                }
            } else {
                temp_v0_2 = func_8008667C(0);
                switch (temp_v0_2) {
                case 0:
                case 1:
                    D_800CA148 = 0x9E;
                    break;
                case 2:
                    D_800CA148 = 0x9F;
                    break;
                case 3:
                    D_800CA148 = 0x9F;
                    break;
                case 4:
                    D_800CA148 = 0xA0;
                    break;
                }
                if (D_800E6280.unk_03E == 0x61) {
                    D_800CA148 += 4;
                }
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if (func_8008667C(0) == 4) {
            normal_date_girl_out();
        } else {
            func_80042940(9);
        }
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        if (func_8008667C(0) == 4) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 10:
        D_80122EAC = 3;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 11:
        func_800866A0(0x544A);
        break;
    case 12:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 0;
        func_80083474();
        cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        break;
    case 13:
        func_8007E884();
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = func_8008667C(0);
            switch (temp_v0_3) {
            case 0:
            case 1:
            case 2:
            case 3:
                D_800CA148 = 0xA6;
                break;
            case 4:
                D_800CA148 = 0xA8;
                break;
            }
            if ((year = D_800E6280.unk_03E) == 0x61) {
                D_800CA148 += 6;
            }
            if ((u16) D_800E6280.unk_66C[5].unk_00 < (u32) (((year = D_800E6280.unk_03E) * 0x64) - 0x24B8)) {
                if (func_8008667C(0) == 4) {
                    D_800CA148 += 2;
                } else {
                    D_800CA148 += 1;
                }
            }
            func_80153FA4();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (func_8008667C(0) == 4) {
            normal_date_girl_out();
        } else {
            func_80042940(0x12);
        }
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if (func_8008667C(0) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 19:
        normal_date_bg_fadeout();
        break;
    case 20:
        D_800CA148 = 1;
        func_8004284C();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80153FA4(void) {
    func_800847B8(0);
    func_80132034();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
