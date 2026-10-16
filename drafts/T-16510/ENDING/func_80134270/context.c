#include "common.h"
#include "ovl/ENDING.h"

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

/* bulk_m2c declarations (T-11190) */
extern s16 D_8013C568;
extern s16 D_8013C56C;
extern s16 D_8013C570;
extern s16 D_8013C574;
extern s16 D_8013C578;
extern s16 D_8013C57C;
extern s16 D_8013C580;
extern s16 D_8013C584;
extern s16 D_8013C588;
extern s16 D_8013C58C;
extern s16 D_8013C590;
extern s16 D_8013C594;
extern s16 D_8013C598;
extern s16 D_8013C59C;
extern s32 D_800E753C;
extern s32 D_800E7540;
extern s32 D_800E7544;
extern s32 D_800E7548;
extern s32 D_800E754C;
extern s32 D_800E7550;
extern s8 D_800E62B6;
extern s8 D_800E62B7;
extern s8 D_800E62B8;
extern u8 D_800E71DF;
/* end bulk_m2c declarations */
void func_80134270(void) {
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    Rec24 *temp_v0;
    s16 temp_v1;
    s16 var_v1;
    s32 temp_at;
    u8 *temp_t7;

    func_8009C5E0(1);
    func_80133C10();
    load_palette(D_8013C5A0, 0x10, 1, 2, 0);
    load_palette(D_8013C5A0, 0x11, 1, 2, 1);
    LoadSquare(0x140U, 0x80U, 0x40U, 0x80U, D_8013C5A4);
    LoadSquare(0x180U, 0x80U, 0x40U, 0x80U, D_8013C5A8);
    LoadSquare(0x1C0U, 0x80U, 0x40U, 0x80U, D_8013C5AC);
    LoadSquare(0x200U, 0x80U, 0x40U, 0x80U, D_8013C5B0);
    LoadSquare(0x240U, 0x80U, 0x40U, 0x80U, D_8013C5B4);
    LoadSquare(0x280U, 0x80U, 0x40U, 0x80U, D_8013C5B8);
    D_800E753C = 1;
    D_800E7540 = 1;
    D_800E7544 = 1;
    D_800E7548 = 1;
    D_800E754C = 1;
    D_800E7550 = 1;
    var_v1 = 0;
    do {
        temp_v0 = &D_801217D0[var_v1];
        *(s16 *)((unsigned char *)temp_v0 + (156)) = (s16) (var_v1 + 5);
        *(s32 *)((unsigned char *)temp_v0 + (144)) = 0x41000000;
        *(s16 *)((unsigned char *)temp_v0 + (152)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0 + (154)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0 + (158)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (159)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0 + (160)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (162)) = 0x1F0;
        *(s8 *)((unsigned char *)temp_v0 + (164)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (165)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (166)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (168)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (170)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (172)) = 0x1000;
        *(s16 *)((unsigned char *)temp_v0 + (174)) = 0x1000;
        *(s32 *)((unsigned char *)temp_v0 + (176)) = 0;
        temp_t7 = &D_800E6280 + var_v1;
        temp_v1 = var_v1 + 1;
        *(s16 *)((unsigned char *)temp_v0 + (148)) = (s16) (((var_v1 % 3) << 7) - 0x80);
        temp_at = temp_v1 < 6;
        *(s16 *)((unsigned char *)temp_v0 + (150)) = (s16) (((var_v1 / 3) << 7) - 0x78);
        var_v1 = temp_v1;
        *(s8 *)((unsigned char *)temp_t7 + (4265)) = 0xB;
    } while (temp_at != 0);
    func_80048F64(0x60);
    D_80120651 = 0xC;
    D_80120652 = 0;
    D_80120688 = 0;
    D_80120653 = 0x84;
    D_8012065C = (u8 *) D_8013C4F8;
    D_80120660 = (u8 *) D_8013C530;
    D_80120684 = (u8 *) D_8013C4C0;
    D_80120664 = D_8013C568;
    D_80120666 = 0;
    D_80120668 = 0;
    D_80120656 = 1;
    D_80120693 = 0x11;
    D_80120676 = -0x20;
    D_8012067A = 0x28;
    D_80120657 = 0;
    D_80120655 = 1;
    D_80120654 = 0;
    *(M2cBlock44 *)(D_80120694) = *(M2cBlock44 *)(D_80120650);
    D_80120697 = 4;
    D_801206BA = 0x10;
    switch (D_800E71DF) {
    case 0:
        D_801206A0 = D_8013C4FC;
        D_801206A4 = D_8013C534;
        D_801206C8 = D_8013C4C4;
        D_801206A8 = D_8013C56C;
        break;
    case 1:
        D_801206A0 = D_8013C500;
        D_801206A4 = D_8013C538;
        D_801206C8 = D_8013C4C8;
        D_801206A8 = D_8013C570;
        break;
    case 2:
        D_801206A0 = D_8013C504;
        D_801206A4 = D_8013C53C;
        D_801206C8 = D_8013C4CC;
        D_801206A8 = (s16) D_8013C574;
        break;
    case 3:
        D_801206A0 = D_8013C508;
        D_801206A4 = D_8013C540;
        D_801206C8 = D_8013C4D0;
        D_801206A8 = D_8013C578;
        break;
    case 4:
        D_801206A0 = D_8013C50C;
        D_801206A4 = D_8013C544;
        D_801206C8 = D_8013C4D4;
        D_801206A8 = D_8013C57C;
        break;
    case 5:
        D_801206A0 = D_8013C510;
        D_801206A4 = D_8013C548;
        D_801206C8 = D_8013C4D8;
        D_801206A8 = D_8013C580;
        break;
    case 6:
        D_801206A0 = D_8013C514;
        D_801206A4 = D_8013C54C;
        D_801206C8 = D_8013C4DC;
        D_801206A8 = D_8013C584;
        break;
    case 7:
        D_801206A0 = D_8013C518;
        D_801206A4 = D_8013C550;
        D_801206C8 = D_8013C4E0;
        D_801206A8 = (s16) D_8013C588;
        break;
    case 8:
        D_801206A0 = D_8013C51C;
        D_801206A4 = D_8013C554;
        D_801206C8 = D_8013C4E4;
        D_801206A8 = D_8013C58C;
        break;
    case 9:
        D_801206A0 = D_8013C520;
        D_801206A4 = D_8013C558;
        D_801206C8 = D_8013C4E8;
        D_801206A8 = D_8013C590;
        break;
    case 10:
        D_801206A0 = D_8013C524;
        D_801206A4 = D_8013C55C;
        D_801206C8 = D_8013C4EC;
        D_801206A8 = D_8013C594;
        break;
    case 12:
        D_801206A0 = D_8013C528;
        D_801206A4 = D_8013C560;
        D_801206C8 = D_8013C4F0;
        D_801206A8 = D_8013C598;
        break;
    default:
        D_801206A0 = D_8013C52C;
        D_801206A4 = D_8013C564;
        D_801206C8 = D_8013C4F4;
        D_801206A8 = (s16) D_8013C59C;
        break;
    }
    D_800E62B7 = 0;
    D_800E62B6 = 0;
    D_800E62B8 = 0;
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
