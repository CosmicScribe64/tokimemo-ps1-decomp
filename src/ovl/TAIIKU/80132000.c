#include "common.h"
#include "ovl/TAIIKU.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
/* Defined in the unit (not an island piece) so that the stores to the runner list share one lui at; T-19031, func_80132000. */
u8 D_80148EC0[4] = { 0 };
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80132000.data", D_80148EC4);

void func_80132000(void) {
    s32 i;
    s32 j;
    s32 n;
    u8 count;
    u8 a;
    u8 b;
    u8 t;
    u8 sp74[11];
    s32 k;
    s32 l;
    s32 m;
    TaiWords11 sp3C;

    sp3C = D_80148EC4;
    func_80083808();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80044890(0, 0xBF98, 0xBF79, 0xD712, 0xD6C3, 0xD6B7);
        func_80085E30(1, 0);
        dec_bg_reset();
        set_dec_bri(0);
        func_8008585C();
        func_80048E78();
        k_reset(0);
        func_8006612C((s32) "フォークダンス");
        k_disp_start(1);
        func_80065F34(0);
        parameter_disp_switch(0);
        icon_disp_switch(0);
        message_window_init();
        func_800AE120(D_800E6280.unk_10F4);
        D_80122E90 = 0;
        func_8004284C();
        break;
    case 1:
        if (func_80044E8C() == 1) {
            count = 0;
            if (j) {} /* FAKE: empty test splits the live range of j. T-19031 */
            for (j = 0; j < 11; j++) {
                if (D_800E6280.unk_1BC[j].unk_0C.f.b1 == 1) {
                    sp74[count] = j;
                    count++;
                }
            }
            if (count < 3) {
                D_80122ECC = 2;
            } else {
                if (D_80122E90 == 0) {
                    dec_bg_cd_read(0x4133, 1);
                }
                D_800E6280.unk_03A = 0x80;
                a = sp74[func_800AE0D0() % count];
                b = sp74[func_800AE0D0() % count];
                if (D_80122E90 != 0) {
                    a = sp74[1];
                    b = sp74[2];
                } else {
                    while (a == 0) {
                        a = sp74[func_800AE0D0() % count];
                    }
                    while ((a == b) || (b == 0)) {
                        b = sp74[func_800AE0D0() % count];
                    }
                    D_80122E90 += 1;
                }
                D_80148EC0[0] = 0;
                D_80148EC0[1] = a;
                D_80148EC0[2] = b;
                n = func_800AE0D0() & 0x1F;
                for (j = 0; j < n; j++) {
                    k = func_800AE0D0() % 3;
                    l = func_800AE0D0() % 3;
                    t = D_80148EC0[k];
                    m = D_80148EC0[l];
                    D_80148EC0[k] = m;
                    D_80148EC0[l] = t;
                }
                func_8004284C();
            }
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80132970();
            func_8004500C(0, 0x200);
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(sp3C.value[D_80148EC0[0]]);
            func_80065F34(1);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = D_80148EC0[0];
            func_80083474();
        }
        break;
    case 5:
        cd_read_sectors(2, 0x801E8000, 0xB6DC);
        func_8004284C();
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            func_80084E4C();
            func_800847B8(D_80148EC0[0]);
            func_80133BF0();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = D_801491B0;
            D_800CA164 = D_801491B4;
            D_800CA168 = D_801491B8;
            func_8004284C();
        }
        break;
    case 7:
    case 8:
    case 9:
    case 12:
    case 15:
    case 17:
    case 22:
    case 25:
    case 27:
    case 32:
    case 35:
    case 40:
        normal_date_speak();
        break;
    case 10:
        switch (func_8008667C(D_80148EC0[0])) {
        case 0:
        case 1:
            D_800CA148 = D_80148EC0[0] * 3 + 5;
            break;
        case 2:
        case 3:
            D_800CA148 = D_80148EC0[0] * 3 + 6;
            break;
        case 4:
            D_800CA148 = D_80148EC0[0] * 3 + 7;
            break;
        }
        func_8004284C();
        break;
    case 11:
    case 20:
    case 30:
        normal_date_girl_in();
        break;
    case 13:
    case 23:
    case 33:
    case 38:
        normal_date_girl_out();
        break;
    case 14:
    case 24:
        D_800CA148 = 1;
        D_800CA14C = 0;
        func_8004284C();
        break;
    case 16:
        func_800847B8(D_80148EC0[1]);
        func_8004284C();
        break;
    case 18:
        func_80062CD0(sp3C.value[D_80148EC0[1]]);
        func_8004284C();
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = D_80148EC0[1];
            func_80083474();
        }
        break;
    case 21:
        switch (func_8008667C(D_80148EC0[1])) {
        case 0:
        case 1:
            D_800CA148 = D_80148EC0[1] * 3 + 5;
            break;
        case 2:
        case 3:
            D_800CA148 = D_80148EC0[1] * 3 + 6;
            break;
        case 4:
            D_800CA148 = D_80148EC0[1] * 3 + 7;
            break;
        }
        func_8004284C();
        break;
    case 26:
        func_800847B8(D_80148EC0[2]);
        func_8004284C();
        break;
    case 28:
        func_80062CD0(sp3C.value[D_80148EC0[2]]);
        func_8004284C();
        break;
    case 29:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = D_80148EC0[2];
            func_80083474();
        }
        break;
    case 31:
        if ((D_800E6280.unk_1BC[D_80148EC0[2]].unk_0A >= 0x4C) || (func_8008667C(D_80148EC0[2]) == 4)) {
            func_80044750(0xBF);
            func_80083440(4);
            func_80042940(0x26);
        } else {
            switch (func_8008667C(D_80148EC0[2])) {
            case 0:
            case 1:
                D_800CA148 = D_80148EC0[2] * 3 + 5;
                break;
            case 2:
            case 3:
                D_800CA148 = D_80148EC0[2] * 3 + 6;
                break;
            case 4:
                D_800CA148 = D_80148EC0[2] * 3 + 7;
                break;
            }
            func_8004284C();
        }
        break;
    case 34:
        D_800CA148 = 3;
        D_800CA14C = 0;
        func_8004284C();
        break;
    case 36:
    case 41:
        normal_date_bg_fadeout();
        break;
    case 37:
    case 42:
        for (i = 0; i < 3; i++) {
            D_800E6280.unk_1BC[D_80148EC0[i]].unk_06 += 1;
        }
        D_80122ECC = 2;
        break;
    case 39:
        D_800CA148 = 4;
        D_800CA14C = 0;
        func_8004284C();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}

/* The GameState pointer makes the record stores indirect, so the runner list
 * byte is reloaded for each use as in the original (T-16554). */
void func_80132970(void) {
    GameState *gs;
    s32 i;

    gs = &D_800E6280;
    for (i = 0; i < 3; i++) {
        gs->unk_75E[gs->unk_F5E].unk_00 = D_80148EC0[i];
        gs->unk_75E[gs->unk_F5E].unk_02 = 0x13;
        gs->unk_75E[gs->unk_F5E].unk_01 = 0;
        gs->unk_75E[gs->unk_F5E].unk_03 = get_g_zyotai_h(D_80148EC0[i]);
        if ((i == 2) && (gs->unk_1BC[D_80148EC0[i]].unk_0A >= 0x4C)) {
            gs->unk_75E[gs->unk_F5E].unk_03 = 4;
        }
        gs->unk_75E[gs->unk_F5E].unk_04 = gs->unk_03E;
        gs->unk_75E[gs->unk_F5E].unk_05 = gs->unk_03F;
        gs->unk_75E[gs->unk_F5E].unk_06 = gs->unk_040;
        gs->unk_75E[gs->unk_F5E].unk_07 = gs->unk_041;
        gs->unk_F5E++;
        if (gs->unk_F5E >= 0xFF) {
            gs->unk_F5E = 0xFE;
        }
    }
}

