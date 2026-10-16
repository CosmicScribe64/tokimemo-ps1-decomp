#include "ovl/MASTER.h"

void func_80139000(void) {
    D_8013C760 = 0x801A42D4;
    D_8013C764 = 0x801A42E0;
    D_8013C768 = 0x801A4330;
    D_8013C76C = *(s16 *)0x801A4338;
    D_8013C770 = 0x80197000;
}

void func_80139054(void) {
    D_8013C760 = 0x801A3E3C;
    D_8013C764 = 0x801A3E48;
    D_8013C768 = 0x801A3E90;
    D_8013C76C = *(s16 *)0x801A3E98;
    D_8013C770 = 0x80197000;
}

void func_801390A8(void) {
    D_8013C760 = 0x801A42E4;
    D_8013C764 = 0x801A42F0;
    D_8013C768 = 0x801A4368;
    D_8013C76C = *(s16 *)0x801A4370;
    D_8013C770 = 0x80197000;
}

void func_801390FC(void) {
    D_8013C760 = 0x801A432C;
    D_8013C764 = 0x801A4338;
    D_8013C768 = 0x801A4394;
    D_8013C76C = *(s16 *)0x801A43A4;
    D_8013C770 = 0x80197000;
}

void func_80139150(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl7 tbl;

    tbl = D_8013C798;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

/* bulk_m2c declarations (T-11190) */
extern s16 D_800E643E;
extern s16 D_800E6442;
extern s16 D_800E666E;
extern s16 D_800E6672;
extern s32 D_800E7374;
extern u16 D_800E6374;
extern u8 D_800E644A;
extern u8 D_800E66B3;
extern u8 D_800E66EB;
extern u8 D_800E738A;
/* end bulk_m2c declarations */
void func_801391E4(void) {
    s32 sp3C;
    GameState *var_s1;
    s32 temp_a0;
    s32 temp_hi;
    s32 var_s0;
    u32 temp_t1;
    u32 temp_t9;
    u8 var_t2;

    func_80083808();
    switch (D_800E738A) {
    case 0:
        func_80044750(0xBF);
        func_80044890(1, 0xC043, 0xC01D, 0xC982, 0xC935, 0xC91B);
        func_80085E30(1, 0);
        dec_bg_reset();
        set_dec_bri(0);
        message_window_init();
        k_reset(0);
        func_8006612C((s32) "　");
        func_80065F34(0);
        hizuke_init();
        parameter_disp_switch(0);
        icon_disp_switch(0);
        func_8008585C();
        func_800AE120(D_800E7374);
        func_8004284C();
        break;
    case 1:
        if (func_80044E8C() == 1) {
            k_reset(0);
            temp_t9 = (u16) D_800E6374 >> 0xC;
            switch (temp_t9) {
            case 7:
                func_8006612C((s32) "国立競技場");
                func_80044750(0x207);
                dec_bg_cd_read(0x41B1, 1);
                break;
            case 8:
                func_8006612C((s32) "インターハイ");
                func_80044750(0x209);
                dec_bg_cd_read(0x41BA, 1);
                break;
            case 9:
                func_8006612C((s32) "インターハイ");
                func_80044750(0x208);
                dec_bg_cd_read(0x41BA, 1);
                break;
            case 10:
                func_8006612C((s32) "インターハイ");
                func_80044750(0x201);
                dec_bg_cd_read(0x41BA, 1);
                break;
            default:
                func_80046500();
                break;
            }
            func_80065F34(0);
            func_8004284C();
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            temp_a0 = func_8008647C();
            var_t2 = 0xC;
            if ((u8) D_800E66EB >= 0x41U) {
                goto block_42;
            }
            sp3C = temp_a0;
            if (func_8008667C(temp_a0) < 2) {
                D_8013C790 = (u8) temp_a0;
                var_s1 = &D_800E6280;
                var_s0 = 0;
                do {
                    if ((((u16) D_800E6374 >> 0xC) == ((u8) var_s1->unk_1BC[0].unk_0C.b[2] >> 4)) && (((u32) (var_s1->unk_1BC[0].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                        if (func_8008667C(var_s0) < 2) {
                            D_8013C790 = (u8) var_s0;
                        }
                        if ((((u8) D_800E644A >> 4) == 0xA) && (((u16) D_800E6374 >> 0xC) == 0xA)) {
                            if ((func_8008667C(0) < 2) && (func_8008667C(0xA) >= 2)) {
                                D_8013C790 = 0;
                            } else if ((func_8008667C(0) >= 2) && (func_8008667C(0xA) < 2)) {
                                D_8013C790 = 0xA;
                            } else if ((func_8008667C(0) < 2) && (func_8008667C(0xA) < 2)) {
                                if ((D_800E643E + D_800E6442) >= (D_800E666E + D_800E6672)) {
                                    D_8013C790 = 0;
                                } else {
                                    D_8013C790 = 0xA;
                                }
                            }
                        }
                    }
                    var_s0 += 1;
                    var_s1 += 0x38;
                } while (var_s0 != 0xB);
            } else {
                var_t2 = 0xFF;
                if ((u8) D_800E66B3 >= 2U) {
                    D_8013C790 = 0xB;
                } else {
block_42:
                    D_8013C790 = var_t2;
                }
            }
            if (D_8013C790 != 0xFF) {
                func_80044890(0, 0xBF98, 0xBF79, D_800B3688[D_8013C790], D_800B36C8[D_8013C790], D_800B3708[D_8013C790]);
            }
            func_8004284C();
        }
        break;
    case 3:
        if ((func_80044E8C() == 1) || (D_8013C790 == 0xFF)) {
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(0xBU, 0x801E8000, 0xA9C5);
            func_80065F34(1);
            func_8004284C();
        }
        break;
    case 4:
        normal_date_bg_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_800847B8(0U);
            func_80084E4C();
            func_80138FC0();
            D_800CA160 = (s32) D_8013C750;
            D_800CA164 = (s32) D_8013C754;
            D_800CA168 = (s32) D_8013C758;
            temp_t1 = (u16) D_800E6374 >> 0xC;
            switch (temp_t1) {
            case 7:
                D_800CA148 = 1;
                break;
            case 8:
                D_800CA148 = 2;
                break;
            case 9:
                D_800CA148 = 3;
                break;
            case 10:
                D_800CA148 = 4;
                break;
            default:
                func_80046500();
                break;
            }
            D_800CA14C = 0;
            func_8004284C();
        }
        break;
    case 6:
    case 8:
        normal_date_speak();
        break;
    case 7:
        temp_hi = func_800AE0D0() % 6;
        D_800CA148 = temp_hi + 6;
        D_8013C794 = (s16) temp_hi;
        func_8004284C();
        break;
    case 9:
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}

void func_80139810(void) {
    s32 pad1; /* FAKE: two unused locals declared before tbl reproduce the original frame (0x68); T-8020 */
    s32 pad2;
    MasterWords13 tbl;

    tbl = D_8013C7B4;
    switch (D_800E6280.unk_110A) {
    case 0:
        if (D_8013C790 != 0xFF) {
            func_80062CD0(tbl.v[D_8013C790]);
            func_80042808();
        } else {
            D_8013C790 = 0xFF;
            func_8004284C();
        }
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        func_80042908(3);
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}

/* FAKE: the scalar byte view preserves the selector live range (T-13001).
 * The thirteen-halfword copy is demonstrated by the original accesses. */
void func_80139910(void) {
    u32 temp_t9;
    MasterHalfwords13 table;

    table = D_8013C7E8;
    func_80083808();
    switch (D_800E6280.unk_110A) {
    case 0:
        if (cd_read_sync_status() & 1) {
            func_800847B8((*(u8 *)&D_8013C790));
            D_800CA148 = table.v[(*(u8 *)&D_8013C790)];
            D_800CA14C = 0;
            func_8007C740();
            D_800E6280.unk_F5F = (*(u8 *)&D_8013C790);
            func_80083474();
            if ((*(u8 *)&D_8013C790) == 0xC) {
                func_80083440(0U);
            }
            if (((u16) D_800E6280.unk_0F4.h >> 0xC) != 7) {
                func_8005742C(0x41C3, 1);
            }
        }
        break;
    case 1:
        if ((cd_read_sync_status() & 1) || (((u16) D_800E6280.unk_0F4.h >> 0xC) == 7)) {
            if (((u16) D_800E6280.unk_0F4.h >> 0xC) != 7) {
                func_8004E9F4(0);
                func_8006612C("選手控え室");
                D_80125CA0 = 0xC0;
            }
            func_8005751C(1);
            func_8004284C();
        }
        break;
    case 2:
        if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 7) {
            func_8004284C();
        } else {
            func_8007EAAC();
        }
        if (((*(u8 *)&D_8013C790) == 0xB) && (D_800E6280.unk_110A == 3)) {
            func_80044750(0x500);
            func_80047550();
        }
        break;
    case 3:
        func_80085A60();
        if (D_800E6280.unk_110A == 4) {
            func_8004500C(0, 0);
        }
        break;
    case 5:
    case 7:
        func_80085A60();
        break;
    case 4:
        func_8007E390();
        break;
    case 6:
        if ((*(u8 *)&D_8013C790) == 0xB) {
            if (((u32) (D_800E6280.unk_1BC[11].unk_0C.w << 0x14) >> 0x1D) == 0) {
                D_800CA148 = 0x1E;
            } else {
                D_800CA148 = 0x1F;
            }
            func_8007E5A0();
        } else if ((*(u8 *)&D_8013C790) == 0xC) {
            D_800CA148 = 0x20;
            func_8004284C();
        } else {
            D_800CA148 = 0x1D;
            func_8004284C();
        }
        break;
    case 8:
        if ((*(u8 *)&D_8013C790) == 0xB) {
            func_8007E934();
        } else {
            func_8007E99C();
        }
        break;
    case 9:
        temp_t9 = (u16) D_800E6280.unk_0F4.h >> 0xC;
        switch (temp_t9) {
        case 7:
            func_8004500C(1, 7);
            break;
        case 8:
            func_8004500C(1, 9);
            break;
        case 9:
            func_8004500C(1, 8);
            break;
        case 10:
            func_8004500C(1, 1);
            break;
        }
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}


INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80139000", func_80139CC8);

/* FAKE: scalar byte views preserve the selector live ranges; the cached
 * girls pointer reproduces the copied-table stack offset (T-13001).
 * The sprite aggregate keeps flag loads after neighboring field stores. */
void func_8013A784(void) {
    Rec38 *girls;
    u8 index;
    MasterHalfwords13 table;

    table = D_8013C804;
    func_80083808();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80083474();
        switch ((*(u8 *)&D_8013C790)) {
        case 0:
            func_80083440(0U);
        default:
            break;
        case 6:
            if (((u16) D_800E6280.unk_0F4.h >> 0xC) != 9) {
                func_80083440(1U);
            }
            break;
        case 7:
            func_80083440(1U);
            break;
        case 9:
            func_80083440(5U);
            D_801217D0[0].unk_0C = 7;
            D_801217D0[0].unk_0E = 0x70;
            break;
        case 12:
            func_80083440(0U);
            break;
        }
        cd_read_sectors(0xEU, 0x80197000, 0xA9D0);
        func_800573F8(1);
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            girls = D_800E6280.unk_1BC;
            func_8013B354();
            index = (*(u8 *)&D_8013C790);
            if ((index == 4) && (((u16) D_800E6280.unk_0F4.h >> 0xC) != ((u8) girls[4].unk_0C.b[2] >> 4))) {
                D_800CA148 = 0x40;
            } else if ((index == 6) && (((u16) D_800E6280.unk_0F4.h >> 0xC) != 9)) {
                D_800CA148 = 0x4B;
            } else {
                D_800CA148 = table.v[index];
            }
            if (index == 9) {
                ((MasterSprite44 *)D_80120650)->unk_03 |= 0x80;
                ((MasterSprite44 *)D_80120650)->unk_16 = 9;
            }
            func_8004284C();
        }
        break;
    case 2:
        if ((*(u8 *)&D_8013C790) == 0xB) {
            func_8007E81C();
        } else {
            func_8007E884();
        }
        if ((*(u8 *)&D_8013C790) == 9) {
            ((MasterSprite44 *)D_80120650)->unk_07 += 4;
            if (((MasterSprite44 *)D_80120650)->unk_07 >= 0x7DU) {
                ((MasterSprite44 *)D_80120650)->unk_07 = 0x80;
            }
        }
        if (((*(u8 *)&D_8013C790) != 0xB) && (D_800E6280.unk_110A == 3)) {
            func_8004500C(0, 0);
        }
        break;
    case 3:
        func_80085A60();
        break;
    case 4:
        switch ((*(u8 *)&D_8013C790)) {
        case 0:
            if (D_800CA148 == 0x2A) {
                func_80083440(1U);
                func_80063668();
                func_80042940(3);
            } else {
                D_800CA148 = 0x21;
                func_8004284C();
            }
            break;
        case 1:
            D_800CA148 = 0x21;
            func_8004284C();
            break;
        case 2:
            D_800CA148 = 0x21;
            func_8004284C();
            break;
        case 3:
            D_800CA148 = 0x21;
            func_8004284C();
            break;
        case 4:
            if (D_800CA148 == 0x3F) {
                ((MasterSprite44 *)D_80120650)->unk_16 = 5;
                ((MasterSprite44 *)D_80120650)->unk_03 |= 0x80;
                ((MasterSprite44 *)D_80120650)->unk_07 = 0x80;
                func_80083440(5U);
                D_801217D0[0].unk_0C = 7;
                D_801217D0[0].unk_0E = 0x70;
                func_80063668();
                func_80042940(3);
            } else {
                D_800CA148 = 0x21;
                func_8004284C();
            }
            break;
        case 5:
            D_800CA148 = 0x21;
            func_8004284C();
            break;
        case 6:
            if (D_800CA148 == 0x4C) {
                func_80083440(0U);
                func_80063668();
                func_80042940(3);
            } else {
                D_800CA148 = 0x21;
                func_8004284C();
            }
            break;
        case 7:
            if (D_800CA148 == 0x52) {
                func_80083440(0U);
                func_80063668();
                func_80042940(3);
            } else {
                D_800CA148 = 0x21;
                func_8004284C();
            }
            break;
        case 8:
            D_800CA148 = 0x21;
            func_8004284C();
            break;
        case 9:
            D_800CA148 = 0x21;
            func_8004284C();
            break;
        case 10:
            D_800CA148 = 0x21;
            func_8004284C();
            break;
        case 11:
            func_80042940(6);
            break;
        case 12:
            D_800CA148 = 0x23;
            func_8004284C();
            break;
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_8013B448();
        func_80042908(6);
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}


/* FAKE: scalar byte views preserve live ranges; the constant value local
 * reproduces the copied-table stack offset (T-13001).
 * The sprite aggregate keeps flag loads after neighboring field stores. */
void func_8013ACF0(void) {
    s32 value;
    u8 index;
    MasterHalfwords13 table;

    table = D_8013C820;
    func_80083808();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80083474();
        func_80083440(5U);
        if ((*(u8 *)&D_8013C790) == 0xC) {
            func_80083440(0U);
        }
        func_800573F8(1);
        cd_read_sectors(0xEU, 0x80197000, 0xA9D0);
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_8013B354();
            index = (*(u8 *)&D_8013C790);
            D_800CA148 = table.v[index];
            D_800CA14C = 0;
            if (index == 9) {
                ((MasterSprite44 *)D_80120650)->unk_03 |= 0x80;
                value = 9;
                ((MasterSprite44 *)D_80120650)->unk_16 = value;
            }
            if (index == 0xA) {
                ((MasterSprite44 *)D_80120650)->unk_03 |= 0x80;
                ((MasterSprite44 *)D_80120650)->unk_16 = 0xB;
            }
            func_8004284C();
        }
        break;
    case 2:
        if ((*(u8 *)&D_8013C790) == 0xB) {
            func_8007E81C();
        } else {
            func_8007E884();
        }
        if (((*(u8 *)&D_8013C790) == 0xA) || ((*(u8 *)&D_8013C790) == 9)) {
            if ((u8) (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) >= 0x81U) {
                (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) = 0x80;
            } else {
                (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) += 4;
            }
        }
        if (((*(u8 *)&D_8013C790) != 0xB) && (D_800E6280.unk_110A == 3)) {
            func_8004500C(0, 0);
        }
        break;
    case 3:
    case 5:
        func_80085A60();
        break;
    case 4:
        switch ((*(u8 *)&D_8013C790)) {
        case 0:
            if (D_800CA148 == 0x2C) {
                ((MasterSprite44 *)D_80120650)->unk_16 = 1;
                ((MasterSprite44 *)D_80120650)->unk_03 |= 0x80;
                (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) = 0x80;
                func_80042940(3);
            } else {
                D_800CA148 = 0x24;
                func_8004284C();
            }
            break;
        case 1:
            if (D_800CA148 == 0x31) {
                ((MasterSprite44 *)D_80120650)->unk_16 = 3;
                ((MasterSprite44 *)D_80120650)->unk_03 |= 0x80;
                (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) = 0x80;
                func_80042940(3);
            } else {
                D_800CA148 = 0x24;
                func_8004284C();
            }
            break;
        case 2:
            D_800CA148 = 0x24;
            func_8004284C();
            break;
        case 3:
            D_800CA148 = 0x24;
            func_8004284C();
            break;
        case 4:
            if (D_800CA148 == 0x42) {
                ((MasterSprite44 *)D_80120650)->unk_16 = 5;
                ((MasterSprite44 *)D_80120650)->unk_03 |= 0x80;
                (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) = 0x80;
                func_80042940(3);
            } else {
                D_800CA148 = 0x24;
                func_8004284C();
            }
            break;
        case 5:
            if (D_800CA148 == 0x47) {
                func_80083440(2U);
                func_80063668();
                func_80042940(3);
            } else {
                D_800CA148 = 0x24;
                func_8004284C();
            }
            break;
        case 6:
            if (D_800CA148 == 0x4E) {
                ((MasterSprite44 *)D_80120650)->unk_03 |= 0x80;
                ((MasterSprite44 *)D_80120650)->unk_16 = 7;
                (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) = 0x80;
                func_80042940(3);
            } else {
                D_800CA148 = 0x24;
                func_8004284C();
            }
            break;
        case 7:
            D_800CA148 = 0x24;
            func_8004284C();
            break;
        case 8:
            D_800CA148 = 0x24;
            func_8004284C();
            break;
        case 9:
            D_800CA148 = 0x24;
            func_8004284C();
            break;
        case 10:
            D_800CA148 = 0x24;
            func_8004284C();
            break;
        case 11:
            func_80042940(6);
            break;
        case 12:
            D_800CA148 = 0x26;
            func_8004284C();
            break;
        }
        break;
    case 6:
        func_8013B804();
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}


s32 func_8013B20C(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        if (D_80120657 < 4U) {
            D_80120657 = 0;
        } else {
            D_80120657 -= 4;
        }
        if (D_8013C790 == 0xB) {
            func_8007E934();
        } else {
            func_8007E99C();
        }
        break;
    case 1:
        func_80044750(0xBF);
        func_800674B0();
        func_8004284C();
        break;
    case 2:
        if (func_80044E8C() == 1) {
            D_800E6280.unk_71E |= 0x10;
            func_8008585C();
            if (D_8013C784 >= 3U) {
                ((MasterClubBits *)&D_800E6280.unk_66C[(u32)D_800E6280.unk_0F4.h >> 0xC])->b30 = 1;
            }
            func_80057390(0);
            func_80072B5C(1);
        }
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}

void func_8013B354(void) {
    func_80138F70();
    func_80048F64(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120657 = 0;
    D_80120655 = 0x80;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
    D_80120693 = 0x10;
    D_80120676 = 0;
    D_8012067A = -0x20;
    D_8012065C = (u8 *)D_8013C744;
    D_80120660 = (u8 *)D_8013C748;
    D_80120684 = (u8 *)D_8013C740;
    D_80120664 = D_8013C74C;
    D_80120652 = 1;
    D_80120653 = 4;
    D_80120668 = 0;
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80139000", func_8013B448);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80139000", func_8013B804);
