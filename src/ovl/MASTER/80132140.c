#include "ovl/MASTER.h"

void func_80132140(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl4 tbl;

    tbl = D_8013C2E4;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

void func_801321B0(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80044750(0xBF);
        func_80044890(1, 0xC043, 0xC01D, 0xC982, 0xC935, 0xC91B);
        func_80085E30(1, 0);
        func_800573AC();
        func_80057390(0);
        func_8008585C();
        func_80064E84();
        func_800649D4();
        func_80063930(0);
        func_80064FA4(0);
        func_8006764C(0);
        func_8004284C();
        break;
    case 1:
        if (func_80044E8C() == 1) {
            func_8004E9F4(0);
            switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
            case 0:
                func_8006612C((s32) "植木賞授賞式");
                func_8005742C(0x4644, 1);
                break;
            case 1:
                func_8006612C((s32) "演劇グランプリ");
                func_8005742C(0x464D, 1);
                break;
            case 2:
                func_8006612C((s32) "ノーベル賞受賞");
                func_8005742C(0x4656, 1);
                break;
            case 3:
                func_8006612C((s32) "ゲーム大賞");
                func_8005742C(0x465F, 1);
                break;
            case 4:
                func_8006612C((s32) "目展大賞");
                func_8005742C(0x4669, 1);
                break;
            case 5:
                func_8006612C((s32) "コンクール優勝");
                func_8005742C(0x4673, 1);
                break;
            default:
                func_80046500();
                break;
            }
            func_80065F34(0);
            func_80044750(0x202);
            func_8004284C();
        }
        break;
    case 2:
        func_8004284C();
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_800847B8(0U);
            func_80084E4C();
            func_80065F34(1);
            func_8004284C();
        }
        break;
    case 4:
        func_8007E81C();
        break;
    case 5:
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}

/* NON_MATCHING: T-22015, 22 rows, all in case 4 (the D_800E7388/89 copies and the D_800E7D50 |= 3): the original colours the loads and the sum $t1/$t2/$t3 (uopt ranges) and the D_800E7D50 address $v1, the draft keeps ugen temporaries $t5..$t9; locals, double uses, separate globals, byte views through D_800E7D10, statement orders, autotweak (90 edits), viewsweep (22 compiles) and a 280 s permuter run gave no gain */
#ifdef NON_MATCHING
void func_80132444(void) {
    u32 temp_t8;
    u8 temp_t6;

    temp_t6 = D_800E6280.unk_110A;
    switch (temp_t6) {
    case 0:
        temp_t8 = (u16) D_800E6280.unk_0F4.h >> 0xC;
        switch (temp_t8) {
        case 0:
            cd_read_sectors(3U, 0x801E8000, 0xA9DE);
            func_80132104();
            break;
        case 1:
            cd_read_sectors(3U, 0x801E8000, 0xA9B4);
            func_80132000();
            break;
        case 2:
            cd_read_sectors(3U, 0x801E8000, 0xA9BF);
            func_8013209C();
            break;
        case 3:
            cd_read_sectors(3U, 0x801E8000, 0xA9B7);
            func_80132034();
            break;
        case 4:
            cd_read_sectors(3U, 0x801E8000, 0xA9BC);
            func_80132068();
            break;
        case 5:
            cd_read_sectors(3U, 0x801E8000, 0xA9C2);
            func_801320D0();
            break;
        default:
            func_80046500();
            break;
        }
        D_800CA160 = (s32) D_8013C2D0;
        D_800CA164 = (s32) D_8013C2D4;
        D_800CA168 = (s32) D_8013C2D8;
        func_8004284C();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0;
            D_800CA14C = 0;
            func_8004284C();
        }
        break;
    case 2:
        normal_date_speak();
        break;
    case 3:
        normal_date_bg_fadeout();
        break;
    case 4:
        if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 3) {
            func_80044750(0xBF);
            D_800E6280.unk_720 = D_800E6280.unk_1108;
            D_800E6280.unk_721 = D_800E6280.unk_1109 + 1;
            D_800E6280.unk_722 = 0;
            /* byte view: the original does lbu/ori/sb on D_800E7D50 (main_api.h: s32), T-22015 */
            *(u8 *)&D_800E7D50 |= 3;
            main_state_set(0x90);
        } else {
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}
#else
INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80132140", func_80132444);
#endif

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80132140", func_801326B4);

void func_80133374(void) {
    s32 i;

    D_800E6280.unk_71E |= 0x10;
    D_800E6280.unk_1BC[0].unk_02 += 0x14;
    for (i = 1; i < 0xD; i++) {
        D_800E6280.unk_1BC[i].unk_02 += 0x14;
    }
    if (D_8013C2E0 != 0xFF) {
        D_800E6280.unk_1BC[D_8013C2E0].unk_06 += 2;
    }
    ((ScheduleClubFlags *)&D_800E6280.unk_66C[(u32)D_800E6280.unk_0F4.h >> 0xC])->b30 = 1;
    func_8008585C();
    func_800573AC();
    func_80057390(0);
    func_8013348C();
    func_80072B5C(1);
    func_80086424();
}

void func_8013348C(void) {
    u32 sel; /* declared before kind: its slot sits above kind's stack home (sp+0x2B), T-18600 */
    u8 kind;

    sel = (u16) D_800E6280.unk_0F4.h >> 0xC;
    switch (sel) {
    case 0:
        kind = 0;
        break;
    case 1:
        if ((((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) == 1) && (D_8013C2E0 == 0)) {
            kind = 0xB;
        } else if (((u8) D_800E6280.unk_1BC[1].unk_0C.b[2] >> 4) == 1) {
            kind = 1;
            if (D_8013C2E0 == 1) {
                kind = 0xC;
            }
        } else {
            kind = 1;
        }
        break;
    case 2:
        kind = 2;
        break;
    case 3:
        kind = 3;
        break;
    case 4:
        kind = 4;
        break;
    case 5:
        kind = 5;
        break;
    }
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_00 = D_8013C2E0;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_02 = 0x15;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = kind;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = get_g_zyotai_h(D_8013C2E0);
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_04 = D_800E6280.unk_03E;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_05 = D_800E6280.unk_03F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_06 = D_800E6280.unk_040;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = D_800E6280.unk_041;
    D_800E6280.unk_F5E++;
    if (D_800E6280.unk_F5E >= 0xFF) {
        D_800E6280.unk_F5E = 0xFE;
    }
}
