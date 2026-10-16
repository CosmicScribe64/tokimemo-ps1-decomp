#define MAIN_API_OVERRIDE_get_g_zyotai_h /* T-18500: func_80145A64 passes two arguments (main_api.h: one). */
#include "common.h"
#include "ovl/BUNKAKEN.h"

s32 get_g_zyotai_h();

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80144F70", func_80144F70);

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
