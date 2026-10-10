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

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133C10", func_80134270);

void func_801349A8(void) {
    func_80046318(0x78, 0x80180000, 0x7B58);
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

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133C10", func_80134B18);

void func_80134BE8(void) {
    if (D_80120668 == 2) {
        if (D_80120658 == 0x2F) {
            func_8004284C();
        }
    }
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133C10", func_80134C2C);
