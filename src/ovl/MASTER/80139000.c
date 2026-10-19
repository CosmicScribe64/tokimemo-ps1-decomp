#include "ovl/MASTER.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80139000.data", D_8013C760);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80139000.data", D_8013C764);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80139000.data", D_8013C768);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80139000.data", D_8013C76C);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80139000.data", D_8013C770);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80139000.data", D_8013C780);
u8 D_8013C784 = 0;
u8 D_8013C788[5] = { 0 };
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80139000.data", D_8013C790);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80139000.data", D_8013C794);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80139000.data", D_8013C798);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80139000.data", D_8013C7B4);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80139000.data", D_8013C7E8);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80139000.data", D_8013C804);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/80139000.data", D_8013C820);

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

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80139000", func_801391E4);

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


/* FAKE: scalar byte views on D_8013C780 and D_8013C784 preserve the target live ranges,
 * and value is reused as the second shuffle index to preserve the frame layout (T-10630, T-24102). */
void func_80139CC8(void) {
    u8 swap;
    u8 other;
    s32 value;
    s32 first;
    u8 *a;
    s32 i;
    s32 swap_count;

    switch (D_800E6280.unk_110A) {
    case 0:
        switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
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
            switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
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
            switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
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
            for (i = 0; i < 5; i++) {
                load_palette(D_8013C770, (i + 0x10) & 0xFF, 1, 1, i);
            }
            func_8004284C();
        }
        break;
    case 5:
    case 11:
    case 13:
        normal_date_speak();
        break;
    case 6:
        value = (s16)(D_800E6280.unk_66C[(u16)D_800E6280.unk_0F4.h >> 0xC].unk_00 - 350);
        if (value < -0x1E) {
            (*(u8 *)&D_8013C784) = 0;
        } else if (value < -0x14) {
            (*(u8 *)&D_8013C784) = 1;
        } else if (value < -0xA) {
            (*(u8 *)&D_8013C784) = 1;
        } else if (value < 0) {
            (*(u8 *)&D_8013C784) = 2;
        } else if (value >= 0x1E) {
            (*(u8 *)&D_8013C784) = 5;
        } else if (value >= 0x14) {
            (*(u8 *)&D_8013C784) = 4;
        } else if (value >= 0xA) {
            (*(u8 *)&D_8013C784) = 4;
        } else if (value >= 0) {
            (*(u8 *)&D_8013C784) = 3;
        }
        func_8004284C();
        break;
    case 7:
        for (a = D_8013C788; a < D_8013C788 + 5; a++) {
            *a = 0;
        }
        for (i = 0; i < (u8)(*(u8 *)&D_8013C784); i++) {
            D_8013C788[i] = 1;
        }
        i = 0;
        swap_count = func_800AE0D0() & 0xF;
        if ((u8)(*(u8 *)&D_8013C784) >= 3U) {
            swap = D_8013C788[0];
            other = D_8013C788[4];
            D_8013C788[0] = other;
            D_8013C788[4] = swap;
        }
        if (swap_count > 0) {
            do {
                first = func_800AE0D0() & 3;
                value = func_800AE0D0() & 3;
                swap = D_8013C788[first];
                i++;
                other = D_8013C788[value];
                D_8013C788[first] = other;
                D_8013C788[value] = swap;
            } while (i != swap_count);
        }
        if (((u16)D_800E6280.unk_0F4.h >> 0xC) == 9) {
            if ((u8)(*(u8 *)&D_8013C784) >= 3U) {
                D_8013C788[0] = 1;
            } else {
                D_8013C788[0] = 0;
            }
        }
        func_8004284C();
        break;
    case 8:
        func_80048F64(0x60);
        ((DisplayFixActor44 *)D_80120650)->unk_01 = 3;
        ((DisplayFixActor44 *)D_80120650)->unk_38 = 0x01000000;
        ((DisplayFixActor44 *)D_80120650)->unk_05 = 0x80;
        ((DisplayFixActor44 *)D_80120650)->unk_1A = 0x1000;
        ((DisplayFixActor44 *)D_80120650)->unk_1C = 0x1000;
        ((DisplayFixActor44 *)D_80120650)->unk_06 = 1;
        ((DisplayFixActor44 *)D_80120650)->unk_07 = 0x80;
        ((DisplayFixActor44 *)D_80120650)->unk_43 = (func_800AE0D0() % 5) + 0x10;
        ((DisplayFixActor44 *)D_80120650)->unk_24.h[1] = -0x60;
        ((DisplayFixActor44 *)D_80120650)->unk_28.h[1] = 0x20;
        ((DisplayFixActor44 *)D_80120650)->unk_0C = (u8 *) D_8013C764;
        ((DisplayFixActor44 *)D_80120650)->unk_10 = (u8 *) D_8013C768;
        ((DisplayFixActor44 *)D_80120650)->unk_34 = (u8 *) D_8013C760;
        ((DisplayFixActor44 *)D_80120650)->unk_14 = D_8013C76C;
        D_8013C780 = 0;
        func_8004284C();
        break;
    case 9:
        if (((DisplayFixActor44 *)D_80120650)->unk_02 == 0) {
            if (((*(u8 *)&D_8013C780) >= 5U) || ((*(u8 *)&D_8013C780) && (((u16) D_800E6280.unk_0F4.h >> 0xC) == 9))) {
                func_8004284C();
            } else {
                ((DisplayFixActor44 *)D_80120650)->unk_03 = 0x84;
                ((DisplayFixActor44 *)D_80120650)->unk_02 = 5;
                ((DisplayFixActor44 *)D_80120650)->unk_18 = 0;
                ((DisplayFixActor44 *)D_80120650)->unk_08 = 0;
                if (D_8013C788[(*(u8 *)&D_8013C780)] == 1) {
                    ((DisplayFixActor44 *)D_80120650)->unk_16 = 0;
                } else {
                    ((DisplayFixActor44 *)D_80120650)->unk_16 = 1;
                }
                (*(u8 *)&D_8013C780) += 1;
            }
        }
        break;
    case 10:
        if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
            D_800CA148 = 0xD;
            func_80044750(0x204);
        } else {
            D_800CA148 = D_8013C794 + 0x10;
            func_80044750(0x205);
        }
        func_8004284C();
        break;
    case 12:
        switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
        case 7:
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
                D_800CA148 = 0x17;
            } else {
                D_800CA148 = 0x66;
            }
            break;
        case 8:
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
                D_800CA148 = 0x18;
            } else {
                D_800CA148 = 0x67;
            }
            break;
        case 9:
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
                D_800CA148 = 0x19;
            } else {
                D_800CA148 = 0x68;
            }
            break;
        case 10:
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
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
        if (((DisplayFixActor44 *)D_80120650)->unk_07 < 4U) {
            ((DisplayFixActor44 *)D_80120650)->unk_07 = 0;
        } else {
            ((DisplayFixActor44 *)D_80120650)->unk_07 -= 4;
        }
        normal_date_bg_fadeout();
        break;
    case 15:
        func_80048F64(0x60);
        if (D_8013C790 == 0xFF) {
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
                D_800E6280.unk_1BC[0].unk_02 += 0x19;
                for (i = 1; i < 13; i++) {
                    D_800E6280.unk_1BC[i].unk_02 += 0x19;
                }
            } else {
                D_800E6280.unk_1BC[0].unk_02 += 0x12;
                for (i = 1; i < 13; i++) {
                    D_800E6280.unk_1BC[i].unk_02 += 0x12;
                }
            }
            func_80042908(6);
        } else {
            k_reset(0);
            if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 7) {
                func_8006612C((s32) "国立競技場");
            } else {
                func_8006612C((s32) "選手控え室");
            }
            D_80125CA0 = 0xC0;
            D_800E6280.unk_1BC[D_8013C790].unk_06 += 2;
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
                D_800E6280.unk_1BC[0].unk_02 += 0x19;
                for (i = 1; i < 13; i++) {
                    D_800E6280.unk_1BC[i].unk_02 += 0x19;
                }
                func_80042808();
            } else {
                D_800E6280.unk_1BC[0].unk_02 += 0x12;
                for (i = 1; i < 13; i++) {
                    D_800E6280.unk_1BC[i].unk_02 += 0x12;
                }
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
                ((DisplayFixActor44 *)D_80120650)->unk_03 |= 0x80;
                ((DisplayFixActor44 *)D_80120650)->unk_16 = 9;
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
            ((DisplayFixActor44 *)D_80120650)->unk_07 += 4;
            if (((DisplayFixActor44 *)D_80120650)->unk_07 >= 0x7DU) {
                ((DisplayFixActor44 *)D_80120650)->unk_07 = 0x80;
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
                ((DisplayFixActor44 *)D_80120650)->unk_16 = 5;
                ((DisplayFixActor44 *)D_80120650)->unk_03 |= 0x80;
                ((DisplayFixActor44 *)D_80120650)->unk_07 = 0x80;
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
                ((DisplayFixActor44 *)D_80120650)->unk_03 |= 0x80;
                value = 9;
                ((DisplayFixActor44 *)D_80120650)->unk_16 = value;
            }
            if (index == 0xA) {
                ((DisplayFixActor44 *)D_80120650)->unk_03 |= 0x80;
                ((DisplayFixActor44 *)D_80120650)->unk_16 = 0xB;
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
            if ((u8) (*(u8 *)&((DisplayFixActor44 *)D_80120650)->unk_07) >= 0x81U) {
                (*(u8 *)&((DisplayFixActor44 *)D_80120650)->unk_07) = 0x80;
            } else {
                (*(u8 *)&((DisplayFixActor44 *)D_80120650)->unk_07) += 4;
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
                ((DisplayFixActor44 *)D_80120650)->unk_16 = 1;
                ((DisplayFixActor44 *)D_80120650)->unk_03 |= 0x80;
                (*(u8 *)&((DisplayFixActor44 *)D_80120650)->unk_07) = 0x80;
                func_80042940(3);
            } else {
                D_800CA148 = 0x24;
                func_8004284C();
            }
            break;
        case 1:
            if (D_800CA148 == 0x31) {
                ((DisplayFixActor44 *)D_80120650)->unk_16 = 3;
                ((DisplayFixActor44 *)D_80120650)->unk_03 |= 0x80;
                (*(u8 *)&((DisplayFixActor44 *)D_80120650)->unk_07) = 0x80;
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
                ((DisplayFixActor44 *)D_80120650)->unk_16 = 5;
                ((DisplayFixActor44 *)D_80120650)->unk_03 |= 0x80;
                (*(u8 *)&((DisplayFixActor44 *)D_80120650)->unk_07) = 0x80;
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
                ((DisplayFixActor44 *)D_80120650)->unk_03 |= 0x80;
                ((DisplayFixActor44 *)D_80120650)->unk_16 = 7;
                (*(u8 *)&((DisplayFixActor44 *)D_80120650)->unk_07) = 0x80;
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
                ((ScheduleClubFlags *)&D_800E6280.unk_66C[(u32)D_800E6280.unk_0F4.h >> 0xC])->b30 = 1;
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

void func_8013B448(void) {
    GameState *g = &D_800E6280;

    g->unk_75E[g->unk_F5E].unk_00 = g->unk_F5F;
    g->unk_75E[g->unk_F5E].unk_02 = 0x15;
    g->unk_75E[g->unk_F5E].unk_04 = g->unk_03E;
    g->unk_75E[g->unk_F5E].unk_05 = g->unk_03F;
    g->unk_75E[g->unk_F5E].unk_06 = g->unk_040;
    g->unk_75E[g->unk_F5E].unk_07 = g->unk_041;
    switch ((u16) g->unk_0F4.h >> 0xC) {
    case 7:
        g->unk_75E[g->unk_F5E].unk_01 = 7;
        break;
    case 8:
        g->unk_75E[g->unk_F5E].unk_01 = 8;
        break;
    case 9:
        g->unk_75E[g->unk_F5E].unk_01 = 9;
        break;
    case 10:
        g->unk_75E[g->unk_F5E].unk_01 = 0xA;
        break;
    }
    switch (g->unk_F5F) {
    case 0:
        g->unk_75E[g->unk_F5E].unk_03 = 1;
        break;
    case 1:
        g->unk_75E[g->unk_F5E].unk_03 = 1;
        break;
    case 2:
        g->unk_75E[g->unk_F5E].unk_03 = 1;
        break;
    case 3:
        g->unk_75E[g->unk_F5E].unk_03 = 1;
        break;
    case 4:
        if (((g->unk_1BC[4].unk_0C.b[2] >> 4) == 7) && (((u16) g->unk_0F4.h >> 0xC) == 7)) {
            g->unk_75E[g->unk_F5E].unk_03 = 5;
            g->unk_75E[g->unk_F5E].unk_07 = 0xFF;
        } else {
            g->unk_75E[g->unk_F5E].unk_03 = 1;
        }
        break;
    case 5:
        g->unk_75E[g->unk_F5E].unk_03 = 1;
        break;
    case 6:
        if (((u16) g->unk_0F4.h >> 0xC) == 9) {
            g->unk_75E[g->unk_F5E].unk_03 = 1;
        } else {
            g->unk_75E[g->unk_F5E].unk_03 = 0;
        }
        break;
    case 7:
        g->unk_75E[g->unk_F5E].unk_03 = 0;
        break;
    case 8:
        g->unk_75E[g->unk_F5E].unk_03 = 1;
        break;
    case 9:
        g->unk_75E[g->unk_F5E].unk_03 = 5;
        g->unk_75E[g->unk_F5E].unk_07 = 0xFF;
        break;
    case 10:
        g->unk_75E[g->unk_F5E].unk_03 = 1;
        break;
    case 11:
        g->unk_75E[g->unk_F5E].unk_03 = 1;
        break;
    case 12:
        g->unk_75E[g->unk_F5E].unk_03 = 1;
        break;
    }
    g->unk_F5E++;
    if (g->unk_F5E >= 0xFFU) {
        g->unk_F5E = 0xFE;
    }
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80139000", func_8013B804);
