#define MAIN_API_OVERRIDE_get_g_zyotai_h /* T-18500: func_80145A64 passes two arguments (main_api.h: one). */
#include "common.h"
#include "ovl/BUNKAKEN.h"

s32 get_g_zyotai_h();

void func_80144F70(void) {
    BunkakenBytes11 sp3C;
    s32 i;
    s32 temp_a0;
    s32 temp_a0_2;

    sp3C = D_80155F90;
    func_80083808();
    switch (D_80122EC0) {
    case 0:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        for (i = 0; i < 11; i++) {
            D_800CA2B0[i] = D_800E6280.unk_1BC[i].unk_0C.f.b1;
        }
        func_8008585C();
        dec_bg_reset();
        message_window_init();
        k_reset(0);
        func_8006612C((s32) "文化祭");
        func_80065F34(0);
        func_8006764C(0);
        hizuke_init();
        icon_disp_switch(0);
        parameter_disp_switch(0);
        D_80122EA4 = 0xFF;
        D_80122E98 = 0;
        D_80122EC0 += 1;
        D_80122EBC = 0xFF;
        break;
    case 1:
        set_dec_bri(0);
        D_80122EC0 += 1;
        break;
    case 2:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            dec_bg_cd_read(0x4029, 1);
            D_80122EC0 += 1;
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            func_80065F34(1);
            D_80122EC0 += 1;
        }
        break;
    case 4:
        normal_date_bg_fadein();
        func_80145500();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_80084E4C();
            func_800847B8(0U);
            func_801324E0();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_80155E90;
            D_800CA164 = (s32) D_80155E94;
            D_800CA168 = (s32) D_80155E98;
            D_80122EC0 += 1;
        }
        break;
    case 6:
        normal_date_speak();
        func_80145500();
        break;
    case 7:
        func_80145E40();
        break;
    case 8:
        if ((D_800E6280.unk_03F == (D_800E6280.unk_1BC[0].unk_10.w & 0xF)) && (D_800E6280.unk_040 == ((u32) (D_800E6280.unk_1BC[0].unk_10.w << 0x17) >> 0x1B)) && !(D_800E6280.unk_71E & 1) && ((D_80122EBC == 0) || (D_80122EBC == 0xB))) {
            D_80122EC0 = 9;
            D_800E6280.unk_1109 = 0;
            D_800E6280.unk_110A = 0;
        } else {
            temp_a0 = D_80122EBC;
            if ((D_800E6280.unk_03F == (D_800E6280.unk_0F8 & 0xF)) && (D_800E6280.unk_040 == ((u32) (D_800E6280.unk_0F8 << 0x17) >> 0x1B)) && !(D_800E6280.unk_71E & 4) && (temp_a0 >= 4) && (temp_a0 < 0xB) && ((D_80122EBC = temp_a0, ((func_8008667C(temp_a0) < 2) != 0)) || ((D_80122EBC == 0xB) && (func_8008667C(0) < 2)))) {
                D_80122EC0 = 0xA;
                D_800E6280.unk_1109 = 0;
                D_800E6280.unk_110A = 0;
            } else if (D_800E6280.unk_1109 == 0) {
                func_80044750(0xBF);
                func_800674B0();
                func_80042808();
            } else if (func_80044E8C() == 1) {
                func_80145550();
                temp_a0_2 = D_80122EBC;
                if (temp_a0_2 != 0xFF) {
                    D_80122EBC = temp_a0_2;
                    if (func_8008667C(temp_a0_2) < 4) {
                        func_80145A64();
                    }
                }
                for (i = 0; i < 0x40; i++) {
                    ((s32 *)D_80122DD0)[i] = 0;
                }
                func_8008585C();
                dec_bg_reset();
                set_dec_bri(0);
                func_80072B5C(1);
            }
        }
        break;
    case 9:
        func_80149144();
        break;
    case 10:
        func_8014A0E0();
        break;
    default:
        func_80046500();
        break;
    }
    if (D_80122E98 == 0) {
        Default_Disp();
    }
}

void func_80145500(void) {
    if (D_800E6280.unk_110A != 0) {
        D_800E6280.unk_110A = 0;
        D_80122EC0 += 1;
    }
}

void func_80145530(void) {
    D_800E6280.unk_1109 = 0;
    D_800E6280.unk_110A = 0;
    D_80122EC0 = 8;
}

void func_80145550(void) {
    GameState *g = &D_800E6280;
    u8 pad; /* FAKE: T-18500, an unused byte above v puts v at sp+0xDA, the home its uninitialized default read uses */
    u8 v;
    BunkakenBytes9x6 a;
    BunkakenBytes11x6 b;
    BunkakenBytes11x6 c;
    s32 sel;

    a = D_80155F9C;
    b = D_80155FD4;
    c = D_80156018;
    sel = (g->unk_03E - 0x5F) & 0xFF;
    if (D_80122EBC == 0xB) {
        g->unk_1BC[0].unk_06 += 2;
        switch (sel) {
        case 0:
            v = a.b[0][D_80122EB8];
            break;
        case 1:
            v = b.b[0][D_80122EB8];
            break;
        case 2:
            v = c.b[0][D_80122EB8];
            break;
        }
        if (v != 0) {
            v--;
        }
        g->unk_1BC[0].unk_02 += v;
    } else if (D_80122EBC < 0xB) {
        g->unk_1BC[D_80122EBC].unk_06 += 2;
        switch (sel) {
        case 0:
            v = a.b[D_80122EBC][D_80122EB8];
            break;
        case 1:
            v = b.b[D_80122EBC][D_80122EB8];
            break;
        case 2:
            v = c.b[D_80122EBC][D_80122EB8];
            break;
        }
        if (v != 0) {
            v--;
        }
        g->unk_1BC[D_80122EBC].unk_02 += v;
    }
    if (D_80122EA4 != 0xFF) {
        g->unk_1BC[D_80122EA4].unk_06 -= 0xA;
        g->unk_1BC[D_80122EA4].unk_0A += 0xA;
    }
}

void func_80145844(void) {
    k_reset(0);
    switch (D_80122EB8) {
    case 0:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_8006612C((s32) "詩集展示");
            break;
        case 0x60:
            func_8006612C((s32) "文集展示");
            break;
        case 0x61:
            func_8006612C((s32) "弁論大会");
            break;
        }
        break;
    case 1:
        func_8006612C((s32) "演劇部公演");
        break;
    case 2:
        func_8006612C((s32) "実験ショー");
        break;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_8006612C((s32) "パソコン占い");
            break;
        case 0x60:
            func_8006612C((s32) "ゲーム");
            break;
        case 0x61:
            func_8006612C((s32) "実験ショー");
            break;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_8006612C((s32) "似顔絵");
            break;
        case 0x60:
            func_8006612C((s32) "公開デッサン");
            break;
        case 0x61:
            func_8006612C((s32) "油絵展示");
            break;
        }
        break;
    case 5:
        func_8006612C((s32) "演奏会");
        break;
    }
    k_disp_start(1);
}

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80144F70.rodata", D_80154B84);

void func_801459FC(void) {
    k_reset(0);
    func_8006612C(&D_80154B84);
    k_disp_start(1);
}

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80144F70.rodata", D_80154B8C);

void func_80145A30(void) {
    k_reset(0);
    func_8006612C(&D_80154B8C);
    func_80065F34(0);
}

void func_80145A64(void) {
    GameState *g = &D_800E6280;
    u8 v;

    if ((func_8008667C(D_80122EBC) != 4) && (D_80122EA4 != D_80122EBC)) {
        switch (D_80122EB8) {
        case 0:
            if ((((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) == 0) && (D_80122EBC == 0)) {
                v = 0x3;
            } else if ((((u8) g->unk_1BC[1].unk_0C.b[2] >> 4) == 0) && (D_80122EBC == 1)) {
                v = 0x3;
            } else {
                v = 0x4;
            }
            break;
        case 1:
            if ((((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) == 1) && (D_80122EBC == 0)) {
                v = 0x7;
            } else if ((((u8) g->unk_1BC[1].unk_0C.b[2] >> 4) == 1) && (D_80122EBC == 1)) {
                v = 0x7;
            } else {
                v = 0x8;
            }
            break;
        case 2:
            if ((((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) == 2) && (D_80122EBC == 0)) {
                v = 0xB;
            } else if ((((u8) g->unk_1BC[2].unk_0C.b[2] >> 4) == 2) && (D_80122EBC == 2)) {
                v = 0xB;
            } else {
                v = 0xC;
            }
            break;
        case 3:
            if ((((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) == 3) && (D_80122EBC == 0)) {
                v = 0xF;
            } else if ((((u8) g->unk_1BC[2].unk_0C.b[2] >> 4) == 3) && (D_80122EBC == 2)) {
                v = 0xF;
            } else {
                v = 0x10;
            }
            break;
        case 4:
            if ((((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) == 4) && (D_80122EBC == 0)) {
                v = 0x13;
            } else if ((((u8) g->unk_1BC[3].unk_0C.b[2] >> 4) == 4) && (D_80122EBC == 3)) {
                v = 0x13;
            } else {
                v = 0x14;
            }
            break;
        case 5:
            if ((((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) == 5) && (D_80122EBC == 0)) {
                v = 0x17;
            } else if ((((u8) g->unk_1BC[3].unk_0C.b[2] >> 4) == 5) && (D_80122EBC == 3)) {
                v = 0x17;
            } else {
                v = 0x18;
            }
            break;
        }
        if (D_80122EBC == 0xB) {
            g->unk_75E[g->unk_F5E].unk_00 = 0;
            g->unk_75E[g->unk_F5E].unk_03 = get_g_zyotai_h(0, v);
        } else {
            g->unk_75E[g->unk_F5E].unk_00 = D_80122EBC;
            g->unk_75E[g->unk_F5E].unk_03 = get_g_zyotai_h(D_80122EBC, v);
        }
        g->unk_75E[g->unk_F5E].unk_02 = 0x14;
        g->unk_75E[g->unk_F5E].unk_01 = v;
        g->unk_75E[g->unk_F5E].unk_04 = g->unk_03E;
        g->unk_75E[g->unk_F5E].unk_05 = g->unk_03F;
        g->unk_75E[g->unk_F5E].unk_06 = g->unk_040;
        g->unk_75E[g->unk_F5E].unk_07 = g->unk_041;
        g->unk_F5E++;
        if (g->unk_F5E >= 0xFF) {
            g->unk_F5E = 0xFE;
        }
    }
}
