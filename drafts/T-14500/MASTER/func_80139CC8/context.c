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

typedef struct {
    void (*f[7])();
} FnTbl7; /* size 0x1C */
extern FnTbl7 D_8013C798;

void func_80139150(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl7 tbl;

    tbl = D_8013C798;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80139000", func_801391E4);

typedef struct {
    s32 v[13];
} Tbl13; /* size 0x34 */
extern Tbl13 D_8013C7B4;

void func_80139810(void) {
    s32 pad1; /* FAKE: two unused locals declared before tbl reproduce the original frame (0x68); T-8020 */
    s32 pad2;
    Tbl13 tbl;

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


/* bulk_m2c declarations (T-11190) */
extern s16 D_800E643E;
extern s16 D_8013C794;
extern s32 D_800E62B8;
extern s32 D_800E6558;
extern s32 D_800E68EC;
extern s32 D_8013C78C;
extern s32 D_8013C78D;
extern u16 D_800E6374;
extern u8 D_800E738A;
extern u8 D_8013C780;
extern u8 D_8013C788;
/* end bulk_m2c declarations */
void func_80139CC8(void) {
    s32 sp3C;
    u32 sp34;
    s32 sp30;
    s32 *var_v0_3;
    s32 *var_v0_4;
    s32 *var_v0_5;
    s32 *var_v0_6;
    s16 temp_t1_2;
    s16 temp_t2;
    s16 temp_t3;
    s16 temp_t3_2;
    s16 temp_t4_2;
    s16 temp_t4_3;
    s16 temp_t5;
    s16 temp_t5_2;
    s16 temp_t6;
    s16 temp_t6_2;
    s16 temp_t7_3;
    s16 temp_t7_4;
    s16 temp_t8;
    s16 temp_t8_2;
    s16 temp_t9;
    s16 temp_t9_2;
    s16 temp_v0;
    s32 temp_v1;
    u32 temp_t1;
    u32 temp_t4;
    u32 temp_t7;
    u32 temp_t7_2;
    u32 var_t0;
    u32 var_t0_2;
    u32 var_t0_3;
    u8 *temp_a0_2;
    u8 *temp_v1_2;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 temp_a0;
    u8 temp_a1;
    u8 temp_a2;
    u8 var_a0;

    switch (D_800E738A) {
    case 0:
        temp_t7 = (u16) D_800E6374 >> 0xC;
        switch (temp_t7) {
        case 7:
            dec_bg_cd_read(0x4183, 1);
            break;
        case 8:
            dec_bg_cd_read(0x419E, 1);
            break;
        case 9:
            dec_bg_cd_read(0x4195, 1);
            break;
        case 10:
            dec_bg_cd_read(0x418D, 1);
            break;
        default:
            func_80046500();
            break;
        }
        k_reset(0);
        func_8006612C((s32) "決勝戦");
        D_80125CA0 = 0xC0;
        func_8004284C();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            temp_t1 = (u16) D_800E6374 >> 0xC;
            switch (temp_t1) {
            case 7:
                cd_read_sectors(0x1AU, 0x80197000, 0xAFE7);
                break;
            case 8:
                cd_read_sectors(0x1BU, 0x80197000, 0xB01C);
                break;
            case 9:
                cd_read_sectors(0x1BU, 0x80197000, 0xB001);
                break;
            case 10:
                cd_read_sectors(0x1BU, 0x80197000, 0xAFCC);
                break;
            default:
                func_80046500();
                break;
            }
            func_8004284C();
        }
        break;
    case 2:
        normal_date_bg_fadein();
        break;
    case 3:
        D_800CA148 = 0xC;
        func_8004284C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_t4 = (u16) D_800E6374 >> 0xC;
            switch (temp_t4) {
            case 7:
                func_80139054();
                break;
            case 8:
                func_801390FC();
                break;
            case 9:
                func_801390A8();
                break;
            case 10:
                func_80139000();
                break;
            default:
                func_80046500();
                break;
            }
            var_t0 = 0;
            do {
                sp34 = var_t0;
                load_palette(D_8013C770, (var_t0 + 0x10) & 0xFF, 1, 1, var_t0);
                var_t0 += 1;
            } while (var_t0 != 5);
            func_8004284C();
        }
        break;
    case 5:
    case 11:
    case 13:
        normal_date_speak();
        break;
    case 6:
        temp_v0 = *(&D_800E68EC + (((u16) D_800E6374 >> 0xC) * 4)) - 0x15E;
        if (temp_v0 < -0x1E) {
            D_8013C784 = 0;
        } else {
            var_a0 = 1;
            if (temp_v0 < -0x14) {
                goto block_58;
            }
            var_a0 = 1;
            if (temp_v0 < -0xA) {
                goto block_58;
            }
            if (temp_v0 < 0) {
                var_a0 = 2;
                goto block_58;
            }
            var_a0 = 5;
            if (temp_v0 >= 0x1E) {
                goto block_58;
            }
            var_a0 = 4;
            if (temp_v0 >= 0x14) {
                goto block_58;
            }
            var_a0 = 4;
            if (temp_v0 >= 0xA) {
                goto block_58;
            }
            var_a0 = 3;
            if (temp_v0 >= 0) {
block_58:
                D_8013C784 = var_a0;
            }
        }
        func_8004284C();
        break;
    case 7:
        var_v0 = &D_8013C788;
        do {
            var_v0 += 1;
            *(s8 *)((unsigned char *)var_v0 + (-1)) = 0;
        } while ((u32) var_v0 < (u32) &D_8013C78D);
        var_t0_2 = 0;
        if (D_8013C784 != 0) {
            var_v0_2 = &D_8013C788;
            do {
                var_t0_2 += 1;
                var_v0_2 += 1;
                *(s8 *)((unsigned char *)var_v0_2 + (-1)) = 1;
            } while (var_t0_2 < (u8) D_8013C784);
        }
        sp34 = 0;
        temp_a0 = D_8013C784;
        var_t0_3 = 0;
        temp_v1 = func_800AE0D0() & 0xF;
        sp30 = temp_v1;
        if (temp_a0 >= 3U) {
            temp_a2 = D_8013C78C;
            D_8013C78C = (u8) D_8013C788;
            *(u8 *)((unsigned char *)(&D_8013C78C) + (-4)) = temp_a2;
        }
        if (temp_v1 > 0) {
            do {
                sp34 = var_t0_3;
                sp3C = func_800AE0D0() & 3;
                temp_a0_2 = &(&D_8013C788)[func_800AE0D0() & 3];
                temp_v1_2 = &(&D_8013C788)[sp3C];
                temp_a1 = *temp_v1_2;
                var_t0_3 += 1;
                *temp_v1_2 = *temp_a0_2;
                *temp_a0_2 = temp_a1;
            } while (var_t0_3 != sp30);
        }
        if (((u16) D_800E6374 >> 0xC) == 9) {
            if (temp_a0 >= 3U) {
                D_8013C788 = 1;
            } else {
                D_8013C788 = 0;
            }
        }
        func_8004284C();
        break;
    case 8:
        func_80048F64(0x60);
        D_80120651 = 3;
        D_80120688 = 0x01000000;
        D_80120655 = 0x80;
        D_8012066A = 0x1000;
        D_8012066C = 0x1000;
        D_80120656 = 1;
        D_80120657 = 0x80;
        D_80120693 = (func_800AE0D0() % 5) + 0x10;
        D_80120676 = -0x60;
        D_8012067A = 0x20;
        D_8012065C = (u8 *) D_8013C764;
        D_80120660 = (u8 *) D_8013C768;
        D_80120684 = (u8 *) D_8013C760;
        D_80120664 = D_8013C76C;
        D_8013C780 = 0;
        func_8004284C();
        break;
    case 9:
        if (D_80120652 == 0) {
            if ((u8) D_8013C780 < 5U) {
                if (D_8013C780 != 0) {
                    if (((u16) D_800E6374 >> 0xC) == 9) {
                        goto block_81;
                    }
                    goto block_83;
                }
block_83:
                D_80120653 = 0x84;
                D_80120652 = 5;
                D_80120668 = 0;
                D_80120658 = 0;
                if ((&D_8013C788)[D_8013C780] == 1) {
                    D_80120666 = 0;
                } else {
                    D_80120666 = 1;
                }
                D_8013C780 += 1;
            } else {
block_81:
                func_8004284C();
            }
        }
        break;
    case 10:
        if ((u8) D_8013C784 >= 3U) {
            D_800CA148 = 0xD;
            func_80044750(0x204);
        } else {
            D_800CA148 = D_8013C794 + 0x10;
            func_80044750(0x205);
        }
        func_8004284C();
        break;
    case 12:
        temp_t7_2 = (u16) D_800E6374 >> 0xC;
        switch (temp_t7_2) {
        case 7:
            if ((u8) D_8013C784 >= 3U) {
                D_800CA148 = 0x17;
            } else {
                D_800CA148 = 0x66;
            }
            break;
        case 8:
            if ((u8) D_8013C784 >= 3U) {
                D_800CA148 = 0x18;
            } else {
                D_800CA148 = 0x67;
            }
            break;
        case 9:
            if ((u8) D_8013C784 >= 3U) {
                D_800CA148 = 0x19;
            } else {
                D_800CA148 = 0x68;
            }
            break;
        case 10:
            if ((u8) D_8013C784 >= 3U) {
                D_800CA148 = 0x1A;
            } else {
                D_800CA148 = 0x69;
            }
            break;
        default:
            func_80046500();
            break;
        }
        func_8004284C();
        break;
    case 14:
        if ((u8) D_80120657 < 4U) {
            D_80120657 = 0;
        } else {
            D_80120657 -= 4;
        }
        normal_date_bg_fadeout();
        break;
    case 15:
        func_80048F64(0x60);
        if (D_8013C790 == 0xFF) {
            if ((u8) D_8013C784 >= 3U) {
                D_800E643E += 0x19;
                var_v0_3 = &D_800E62B8;
                do {
                    temp_t4_2 = *(s16 *)((unsigned char *)var_v0_3 + (502));
                    temp_t6 = *(s16 *)((unsigned char *)var_v0_3 + (558));
                    temp_t8 = *(s16 *)((unsigned char *)var_v0_3 + (614));
                    temp_t2 = *(s16 *)((unsigned char *)var_v0_3 + (446));
                    var_v0_3 += 0xE0;
                    *(s16 *)((unsigned char *)var_v0_3 + (390)) = (s16) (temp_t8 + 0x19);
                    *(s16 *)((unsigned char *)var_v0_3 + (334)) = (s16) (temp_t6 + 0x19);
                    *(s16 *)((unsigned char *)var_v0_3 + (278)) = (s16) (temp_t4_2 + 0x19);
                    *(s16 *)((unsigned char *)var_v0_3 + (222)) = (s16) (temp_t2 + 0x19);
                } while (var_v0_3 != &D_800E6558);
            } else {
                D_800E643E += 0x12;
                var_v0_4 = &D_800E62B8;
                do {
                    temp_t5 = *(s16 *)((unsigned char *)var_v0_4 + (502));
                    temp_t7_3 = *(s16 *)((unsigned char *)var_v0_4 + (558));
                    temp_t9 = *(s16 *)((unsigned char *)var_v0_4 + (614));
                    temp_t3 = *(s16 *)((unsigned char *)var_v0_4 + (446));
                    var_v0_4 += 0xE0;
                    *(s16 *)((unsigned char *)var_v0_4 + (390)) = (s16) (temp_t9 + 0x12);
                    *(s16 *)((unsigned char *)var_v0_4 + (334)) = (s16) (temp_t7_3 + 0x12);
                    *(s16 *)((unsigned char *)var_v0_4 + (278)) = (s16) (temp_t5 + 0x12);
                    *(s16 *)((unsigned char *)var_v0_4 + (222)) = (s16) (temp_t3 + 0x12);
                } while (var_v0_4 != &D_800E6558);
            }
            func_80042908(6);
        } else {
            k_reset(0);
            if (((u16) D_800E6374 >> 0xC) == 7) {
                func_8006612C((s32) "国立競技場");
            } else {
                func_8006612C((s32) "選手控え室");
            }
            D_80125CA0 = 0xC0;
            D_800E6280.unk_1BC[D_8013C790].unk_06 += 2;
            if ((u8) D_8013C784 >= 3U) {
                D_800E643E += 0x19;
                var_v0_5 = &D_800E62B8;
                do {
                    temp_t5_2 = *(s16 *)((unsigned char *)var_v0_5 + (502));
                    temp_t7_4 = *(s16 *)((unsigned char *)var_v0_5 + (558));
                    temp_t9_2 = *(s16 *)((unsigned char *)var_v0_5 + (614));
                    temp_t3_2 = *(s16 *)((unsigned char *)var_v0_5 + (446));
                    var_v0_5 += 0xE0;
                    *(s16 *)((unsigned char *)var_v0_5 + (390)) = (s16) (temp_t9_2 + 0x19);
                    *(s16 *)((unsigned char *)var_v0_5 + (334)) = (s16) (temp_t7_4 + 0x19);
                    *(s16 *)((unsigned char *)var_v0_5 + (278)) = (s16) (temp_t5_2 + 0x19);
                    *(s16 *)((unsigned char *)var_v0_5 + (222)) = (s16) (temp_t3_2 + 0x19);
                } while (var_v0_5 != &D_800E6558);
                func_80042808();
            } else {
                D_800E643E += 0x12;
                var_v0_6 = &D_800E62B8;
                do {
                    temp_t6_2 = *(s16 *)((unsigned char *)var_v0_6 + (502));
                    temp_t8_2 = *(s16 *)((unsigned char *)var_v0_6 + (558));
                    temp_t1_2 = *(s16 *)((unsigned char *)var_v0_6 + (614));
                    temp_t4_3 = *(s16 *)((unsigned char *)var_v0_6 + (446));
                    var_v0_6 += 0xE0;
                    *(s16 *)((unsigned char *)var_v0_6 + (390)) = (s16) (temp_t1_2 + 0x12);
                    *(s16 *)((unsigned char *)var_v0_6 + (334)) = (s16) (temp_t8_2 + 0x12);
                    *(s16 *)((unsigned char *)var_v0_6 + (278)) = (s16) (temp_t6_2 + 0x12);
                    *(s16 *)((unsigned char *)var_v0_6 + (222)) = (s16) (temp_t4_3 + 0x12);
                } while (var_v0_6 != &D_800E6558);
                func_80042908(5);
            }
        }
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}

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


typedef struct {
    u32 pad0 : 25;
    u32 grade : 2;
    u32 pad1 : 3;
    u32 flag : 1;
    u32 pad2 : 1;
} MasterSlot; /* word of GameState.unk_66C[]: bit 30 is the flag */

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
                ((MasterSlot *)&D_800E6280.unk_66C[(u32)D_800E6280.unk_0F4.h >> 0xC])->flag = 1;
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
