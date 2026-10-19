#include "common.h"
#include "ovl/BUNKASAI.h"

void func_80142700(void) {
    s32 i;
    GameState *g = &D_800E6280;
    BunkasaiGirlBytes sp3C;

    sp3C = D_80160520;
    func_80083808();
    switch (D_80122EC0) {
    case 0:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_8008585C();
        for (i = 0; i < 11; i++) {
            D_800CA2B0[i] = g->unk_1BC[i].unk_0C.f.b1;
        }
        dec_bg_reset();
        message_window_init();
        k_reset(0);
        func_8006612C((s32) "文化祭");
        func_80065F34(0);
        func_8006764C(0);
        hizuke_init();
        icon_disp_switch(0);
        parameter_disp_switch(0);
        D_80122EBC = 0xFF;
        D_80122EA4 = 0xFF;
        D_80122E98 = 0;
        /* FAKE: shared D_80122E98 base retains the state load after the clear; block layout unknown (T-24030). */
        (&D_80122E98)[10] += 1;
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
        func_80142B98();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_80084E4C();
            func_800847B8(0U);
            func_801323A8();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_801604F0;
            D_800CA164 = (s32) D_801604F4;
            D_800CA168 = (s32) D_801604F8;
            D_80122EC0 += 1;
        }
        break;
    case 6:
        normal_date_speak();
        func_80142B98();
        break;
    case 7:
        func_80150930();
        break;
    case 8:
        if ((g->unk_03F == (g->unk_1BC[0].unk_10.w & 0xF)) && (g->unk_040 == ((u32) (g->unk_1BC[0].unk_10.w << 0x17) >> 0x1B)) && !(g->unk_71E & 1)) {
            D_80122EC0 = 9;
            g->unk_1109 = 0;
            g->unk_110A = 0;
        } else if (g->unk_1109 == 0) {
            func_80044750(0xBF);
            func_800674B0();
            func_80042808();
        } else if (func_80044E8C() == 1) {
            func_80142BE8();
            func_801433C0();
            for (i = 0; i < 64; i++) {
                ((s32 *)D_80122DD0)[i] = 0;
            }
            func_8008585C();
            dec_bg_reset();
            set_dec_bri(0);
            func_80072B5C(1);
        }
        break;
    case 9:
        func_80147214();
        break;
    default:
        func_80046500();
        break;
    }
    if (D_80122E98 == 0) {
        Default_Disp();
    }
}

void func_80142B98(void) {
    if (D_800E6280.unk_110A != 0) {
        D_800E6280.unk_110A = 0;
        D_80122EC0 += 1;
    }
}

void func_80142BC8(void) {
    D_800E6280.unk_1109 = 0;
    D_800E6280.unk_110A = 0;
    D_80122EC0 = 8;
}

/* FAKE: s32 return type reserves $v0 in uopt so bonus gets $a3; pad1/pad2 place amount at sp+0x11C; (&g->unk_0FC) indexes across the 4-entry Sub_0FC block. T-24094 */
s32 func_80142BE8(void) {
    s32 i;
    GameState *g = &D_800E6280;
    u8 pad1;
    u8 pad2;
    u8 val;
    u8 amount;
    BunkasaiColumns sel;
    BunkasaiGirlColumns first;
    BunkasaiGirlColumns second;
    BunkasaiGirlColumns third;
    u32 bonus;

    sel = D_8016052C;
    first = D_80160534;
    second = D_80160584;
    third = D_801605D4;
    bonus = ((u32) (g->unk_66C[g->unk_0F4.h >> 12].unk_00 >> 3) / (u32) (g->unk_03E - 94) +
             (&g->unk_0FC)[sel.v[g->unk_0F4.h >> 12]].unk_02 / 20) & 0xFF;
    if (bonus >= 11) {
        bonus = 10;
    }
    switch ((g->unk_03E - 95) & 0xFF) {
    case 0:
        for (i = 0; i < 13; i++) {
            val = first.v[i][g->unk_0F4.h >> 12] >> 1;
            g->unk_1BC[i].unk_02 += bonus;
            g->unk_1BC[i].unk_02 += val;
        }
        amount = first.v[D_80122EBC][g->unk_0F4.h >> 12];
        break;
    case 1:
        for (i = 0; i < 13; i++) {
            val = second.v[i][g->unk_0F4.h >> 12] >> 1;
            g->unk_1BC[i].unk_02 += bonus;
            g->unk_1BC[i].unk_02 += val;
        }
        amount = second.v[D_80122EBC][g->unk_0F4.h >> 12];
        break;
    case 2:
        for (i = 0; i < 13; i++) {
            val = third.v[i][g->unk_0F4.h >> 12] >> 1;
            g->unk_1BC[i].unk_02 += bonus;
            g->unk_1BC[i].unk_02 += val;
        }
        amount = third.v[D_80122EBC][g->unk_0F4.h >> 12];
        break;
    }
    if (amount != 0) {
        amount--;
    }
    g->unk_1BC[D_80122EBC].unk_06 += 2;
    g->unk_1BC[D_80122EBC].unk_02 += amount;
}

void func_801431D4(void) {
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

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/80142700.rodata", D_8015D774);

void func_8014338C(void) {
    k_reset(0);
    func_8006612C(&D_8015D774);
    k_disp_start(1);
}

void func_801433C0(void) {
    /* FAKE: 4-byte stack pad places sp2B at sp+0x2B instead of sp+0x2F. T-24094 */
    s32 pad;
    u8 sp2B;
    GameState *game = &D_800E6280;

    switch (game->unk_0F4.h >> 12) {
    case 0:
        if ((((u8) game->unk_1BC[0].unk_0C.b[2] >> 4) == 0) ||
            ((((u8) game->unk_1BC[1].unk_0C.b[2] >> 4) == 0) && (game->unk_1BC[1].unk_0C.f.b1 == 1))) {
            sp2B = 2;
        } else {
            sp2B = 1;
        }
        break;
    case 1:
        if ((((u8) game->unk_1BC[0].unk_0C.b[2] >> 4) == 1) ||
            ((((u8) game->unk_1BC[1].unk_0C.b[2] >> 4) == 1) && (game->unk_1BC[1].unk_0C.f.b1 == 1))) {
            sp2B = 6;
        } else {
            sp2B = 5;
        }
        break;
    case 2:
        if ((((u8) game->unk_1BC[0].unk_0C.b[2] >> 4) == 2) ||
            ((((u8) game->unk_1BC[2].unk_0C.b[2] >> 4) == 2) && (game->unk_1BC[2].unk_0C.f.b1 == 1))) {
            sp2B = 10;
        } else {
            sp2B = 9;
        }
        break;
    case 3:
        if ((((u8) game->unk_1BC[0].unk_0C.b[2] >> 4) == 3) ||
            ((((u8) game->unk_1BC[2].unk_0C.b[2] >> 4) == 3) && (game->unk_1BC[2].unk_0C.f.b1 == 1))) {
            sp2B = 14;
        } else {
            sp2B = 13;
        }
        break;
    case 4:
        if ((((u8) game->unk_1BC[0].unk_0C.b[2] >> 4) == 4) ||
            ((((u8) game->unk_1BC[3].unk_0C.b[2] >> 4) == 4) && (game->unk_1BC[3].unk_0C.f.b1 == 1))) {
            sp2B = 18;
        } else {
            if ((D_80122EBC == 10) && (func_8008667C(10) == 4)) {
                sp2B = 17;
                return;
            }
            sp2B = 17;
        }
        break;
    case 5:
        if ((((u8) game->unk_1BC[0].unk_0C.b[2] >> 4) == 5) ||
            ((((u8) game->unk_1BC[3].unk_0C.b[2] >> 4) == 5) && (game->unk_1BC[3].unk_0C.f.b1 == 1))) {
            sp2B = 22;
        } else {
            sp2B = 21;
        }
        break;
    }
    game->unk_75E[game->unk_F5E].unk_00 = D_80122EBC;
    game->unk_75E[game->unk_F5E].unk_02 = 20;
    game->unk_75E[game->unk_F5E].unk_01 = sp2B;
    game->unk_75E[game->unk_F5E].unk_03 = func_80051B48(D_80122EBC);
    game->unk_75E[game->unk_F5E].unk_04 = game->unk_03E;
    game->unk_75E[game->unk_F5E].unk_05 = game->unk_03F;
    game->unk_75E[game->unk_F5E].unk_06 = game->unk_040;
    game->unk_75E[game->unk_F5E].unk_07 = game->unk_041;
    if (++game->unk_F5E >= 0xFF) {
        game->unk_F5E = 0xFE;
    }
}
