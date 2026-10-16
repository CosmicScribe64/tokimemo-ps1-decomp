#include "common.h"
#include "ovl/ENDING.h"
#include "game.h"

void func_80133C10(void) {
    D_8013C4C0 = 0x801B6550;
    D_8013C4C4 = 0x801B6C6C;
    D_8013C4C8 = 0x801B7364;
    D_8013C4CC = 0x801B7AB0;
    D_8013C4D0 = 0x801B8200;
    D_8013C4D4 = 0x801B8834;
    D_8013C4D8 = 0x801B8EC4;
    D_8013C4DC = 0x801B9520;
    D_8013C4E0 = 0x801B9B98;
    D_8013C4E4 = 0x801BA088;
    D_8013C4E8 = 0x801BA790;
    D_8013C4EC = 0x801BACB4;
    D_8013C4F0 = 0x801BB3B0;
    D_8013C4F4 = 0x801BB760;
    D_8013C4F8 = 0x801B6554;
    D_8013C4FC = 0x801B6C70;
    D_8013C500 = 0x801B7368;
    D_8013C504 = 0x801B7AB4;
    D_8013C508 = 0x801B8204;
    D_8013C50C = 0x801B8838;
    D_8013C510 = 0x801B8EC8;
    D_8013C514 = 0x801B9524;
    D_8013C518 = 0x801B9B9C;
    D_8013C51C = 0x801BA08C;
    D_8013C520 = 0x801BA794;
    D_8013C524 = 0x801BACB8;
    D_8013C528 = 0x801BB3B4;
    D_8013C52C = 0x801BB764;
    D_8013C530 = 0x801B65AC;
    D_8013C534 = 0x801B6D14;
    D_8013C538 = 0x801B7408;
    D_8013C53C = 0x801B7B5C;
    D_8013C540 = 0x801B82AC;
    D_8013C544 = 0x801B88C4;
    D_8013C548 = 0x801B8F60;
    D_8013C54C = 0x801B95B4;
    D_8013C550 = 0x801B9C30;
    D_8013C554 = 0x801BA128;
    D_8013C558 = 0x801BA834;
    D_8013C55C = 0x801BAD58;
    D_8013C560 = 0x801BB454;
    D_8013C564 = 0x801BB7CC;
    D_8013C5A0 = 0x80180000;
    D_8013C5A4 = 0x80184000;
    D_8013C5A8 = 0x80188000;
    D_8013C5AC = 0x8018C000;
    D_8013C5B0 = 0x80190000;
    D_8013C5B4 = 0x80194000;
    D_8013C5B8 = 0x80198000;
}

typedef struct {
    void (*f[13])();
} FnTbl13; /* size 0x34 */
extern FnTbl13 D_8013C5BC;

void func_80133F1C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl13 tbl;

    tbl = D_8013C5BC;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

s32 func_80133F98(void) {
    D_800E6280.unk_1104.w += 1;
    if ((u32)D_800E6280.unk_1104.w < 2U) {
        func_80132000();
        func_800438DC(1, 0);
        func_80048DAC(1);
        func_800438F0(1);
        func_80048390();
        func_80041584();
        func_80048EB8(0);
        func_8006BD6C(0);
        func_8004E58C();
        func_8008585C();
        D_800E6280.unk_03A = 0x80;
        D_800B593C = 0;
        D_800B5940 = 0;
        return 0;
    }
    if (D_800E6280.unk_1104.w == 5) {
        func_8009C5E0(0);
    } else if ((u32)D_800E6280.unk_1104.w < 0xBU) {
        return 0;
    }
    func_80134140();
    func_8004284C();
}

s32 func_8013408C(void) {
    D_800E6280.unk_1104.w += 1;
    if ((u32)D_800E6280.unk_1104.w < 2U) {
        func_800438DC(1, 0);
        func_80048DAC(1);
        func_800438F0(1);
        func_80048390();
        func_80041584();
        func_80048EB8(0);
        func_8006BD6C(0);
        func_8004E58C();
        func_8004E9F4(1);
        return 0;
    }
    if ((u32)D_800E6280.unk_1104.w < 0xBU) {
        return 0;
    }
    func_801341D8();
    func_8004284C();
}

void func_80134140(void) {
    func_80097D90(0x100, 0xF0, 0, 0, 0);
    func_80098380();
    func_80098490(0, 0, 0, 0xF0);
    D_800E8C70 = 8;
    D_800E8C74 = D_800E8CA0;
    D_800E8C84 = 8;
    D_800E8C88 = D_800E90A0;
    func_80098530();
    InitGeom();
    D_800E6280.unk_000 = 0x100;
}

void func_801341D8(void) {
    func_80097D90(0x140, 0xF0, 0, 0, 0);
    func_80098380();
    func_80098490(0, 0, 0, 0xF0);
    D_800E8C70 = 8;
    D_800E8C74 = D_800E8CA0;
    D_800E8C84 = 8;
    D_800E8C88 = D_800E90A0;
    func_80098530();
    InitGeom();
    D_800E6280.unk_000 = 0x140;
}

void func_80134270(void) {
    GameState *g = &D_800E6280;

    Rec24 *temp_v0;
    s16 var_v1;

    func_8009C5E0(1);
    func_80133C10();
    func_80043914(D_8013C5A0, 0x10, 1, 2, 0);
    func_80043914(D_8013C5A0, 0x11, 1, 2, 1);
    func_8004435C(0x140U, 0x80U, 0x40U, 0x80U, D_8013C5A4);
    func_8004435C(0x180U, 0x80U, 0x40U, 0x80U, D_8013C5A8);
    func_8004435C(0x1C0U, 0x80U, 0x40U, 0x80U, D_8013C5AC);
    func_8004435C(0x200U, 0x80U, 0x40U, 0x80U, D_8013C5B0);
    func_8004435C(0x240U, 0x80U, 0x40U, 0x80U, D_8013C5B4);
    func_8004435C(0x280U, 0x80U, 0x40U, 0x80U, D_8013C5B8);
    g->unk_1228[37] = 1;
    g->unk_1228[38] = 1;
    g->unk_1228[39] = 1;
    g->unk_1228[40] = 1;
    g->unk_1228[41] = 1;
    g->unk_1228[42] = 1;
    var_v1 = 0;
    do {
        temp_v0 = &D_801217D0[var_v1];
        temp_v0[4].unk_0C = (s16) (var_v1 + 5);
        temp_v0[4].unk_00 = 0x41000000;
        temp_v0[4].unk_08 = 0x80;
        temp_v0[4].unk_0A = 0x80;
        temp_v0[4].unk_0E = 0;
        temp_v0[4].unk_0F = 0x80;
        temp_v0[4].unk_10 = 0;
        temp_v0[4].unk_12 = 0x1F0;
        temp_v0[4].unk_14 = 0;
        temp_v0[4].unk_15 = 0;
        temp_v0[4].unk_16 = 0;
        temp_v0[4].unk_18 = 0;
        temp_v0[4].unk_1A = 0;
        temp_v0[4].unk_1C = 0x1000;
        temp_v0[4].unk_1E = 0x1000;
        temp_v0[4].unk_20 = 0;
        temp_v0[4].unk_04 = (s16) (((var_v1 % 3) << 7) - 0x80);
        temp_v0[4].unk_06 = (s16) (((var_v1 / 3) << 7) - 0x78);
        g->unk_10A5[var_v1 + 4] = 0xB;
        var_v1++;
    } while (var_v1 < 6);
    func_80048F64(0x60);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 0xC;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0x84;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_0C = (u8 *) D_8013C4F8;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_10 = (u8 *) D_8013C530;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_34 = (u8 *) D_8013C4C0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_14 = D_8013C568[0][0];
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_06 = 1;
    D_80120693 = 0x11;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0x20;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = 0x28;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 0;
    ((DisplayActor44 *)D_8011ECD0)[97] = ((DisplayActor44 *)D_8011ECD0)[96];
    ((DisplayActor44 *)D_8011ECD0)[97].unk_03 = 4;
    ((DisplayActor44 *)D_8011ECD0)[97].unk_26 = 0x10;
    switch (g->unk_F5F) {
    case 0:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_0C = D_8013C4FC;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_10 = D_8013C534;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_34 = D_8013C4C4;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = D_8013C568[1][0];
        break;
    case 1:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_0C = D_8013C500;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_10 = D_8013C538;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_34 = D_8013C4C8;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = D_8013C568[2][0];
        break;
    case 2:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_0C = D_8013C504;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_10 = D_8013C53C;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_34 = D_8013C4CC;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = D_8013C568[3][0];
        break;
    case 3:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_0C = D_8013C508;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_10 = D_8013C540;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_34 = D_8013C4D0;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = D_8013C568[4][0];
        break;
    case 4:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_0C = D_8013C50C;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_10 = D_8013C544;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_34 = D_8013C4D4;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = D_8013C568[5][0];
        break;
    case 5:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_0C = D_8013C510;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_10 = D_8013C548;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_34 = D_8013C4D8;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = D_8013C568[6][0];
        break;
    case 6:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_0C = D_8013C514;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_10 = D_8013C54C;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_34 = D_8013C4DC;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = D_8013C568[7][0];
        break;
    case 7:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_0C = D_8013C518;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_10 = D_8013C550;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_34 = D_8013C4E0;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = D_8013C568[8][0];
        break;
    case 8:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_0C = D_8013C51C;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_10 = D_8013C554;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_34 = D_8013C4E4;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = D_8013C568[9][0];
        break;
    case 9:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_0C = D_8013C520;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_10 = D_8013C558;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_34 = D_8013C4E8;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = D_8013C568[10][0];
        break;
    case 10:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_0C = D_8013C524;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_10 = D_8013C55C;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_34 = D_8013C4EC;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = D_8013C568[11][0];
        break;
    case 12:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_0C = D_8013C528;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_10 = D_8013C560;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_34 = D_8013C4F0;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = D_8013C568[12][0];
        break;
    default:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_0C = D_8013C52C;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_10 = D_8013C564;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_34 = D_8013C4F4;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = D_8013C568[13][0];
        break;
    }
    g->unk_037 = 0;
    g->unk_036 = 0;
    g->unk_038 = 0;
    func_8004284C();
}


void func_801349A8(void) {
    cd_read_sectors(0x78, 0x80180000, 0x7B58);
    func_80133C10();
    func_8004284C();
}

void func_801349E0(void) {
    s16 t;
    s16 i;

    t = D_80120676 - 1;
    for (i = 0; i < 6; i++) {
        *(s16 *)((u8 *)D_801217D0 + i * 0x24 + 0x94) -= 1;
    }
    D_80120676 = t;
    if (t < -0x9E) {
        D_8011ECD0[0x1982] = 1;
        *(s16 *)&D_8011ECD0[0x1996] = 1;
        *(s16 *)&D_8011ECD0[0x1998] = 0;
        *(s16 *)&D_8011ECD0[0x1988] = 0;
        D_8011ECD0[0x19C7] |= 0x80;
        D_8011ECD0[0x19C6] = 5;
        func_8004284C();
    }
}

void func_80134AA8(void) {
    if ((D_80120696 == 0) && (D_80120668 == 1) && (D_80120658 == 0x14)) {
        D_80120666 = 2;
        D_80120668 = 0;
        D_80120658 = 0;
        D_80120652 = 5;
        func_8004284C();
    }
}

#ifdef NON_MATCHING
/* NON_MATCHING: T-10520, only the register of the promoted D_80120657 differs ($v0 here, $a1 in the original: one range across the calls), 9 lines */
void func_80134B18(void) {
    s16 i;

    D_800E6280.unk_036 += 8;
    if (D_800E6280.unk_036 == 0 || D_800E6280.unk_036 == 7) {
        D_800E6280.unk_036 = 0xFF;
    }
    D_800E6280.unk_037 = D_800E6280.unk_038 = D_800E6280.unk_036;
    D_80120657 += 4;
    D_8012069B += 4;
    for (i = 4; i < 0xA; i++) {
        func_80084C98(i, D_80120657);
    }
    if (D_80120657 >= 0x80) {
        D_80120652 = 5;
        func_8004284C();
    }
}
#else
INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133C10", func_80134B18);
#endif

void func_80134BE8(void) {
    if (D_80120668 == 2) {
        if (D_80120658 == 0x2F) {
            func_8004284C();
        }
    }
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133C10", func_80134C2C);
