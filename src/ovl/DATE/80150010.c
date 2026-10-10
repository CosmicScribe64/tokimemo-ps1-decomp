#include "common.h"
#include "ovl/DATE.h"

typedef struct {
    void (*f[40])();
} FnTbl40; /* size 0xA0 */

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80150010);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80150080);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80150130);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80150200);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_801502B0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80150344);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80150460);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_801505B4);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80150890);

void func_80150A84(void) {
    bg_read_sub2(0x4587);
    func_8004284C();
}

void func_80150AAC(void) {
    bg_read_sub2(0x457D);
    func_8004284C();
}

void func_80150AD4(void) {
    bg_read_sub2(0x4925);
    func_8004284C();
}

void func_80150AFC(void) {
    bg_read_sub2(0x4631);
    func_8004284C();
}

void func_80150B24(void) {
    bg_read_sub2(0x4845);
    func_8004284C();
}

void func_80150B4C(void) {
    bg_read_sub2(0x44D6);
    func_8004284C();
}

void func_80150B74(void) {
    bg_read_sub2(0x453B);
    func_8004284C();
}

void func_80150B9C(void) {
    func_80044750(0x203);
    bg_read_sub2(0x4520);
    func_8004284C();
}

extern FnTbl40 D_8015EAE8;

void func_80150BCC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl40 tbl;

    tbl = D_8015EAE8;
    idx = D_800E738A;
    tbl.f[idx]();
    if (D_80122D04 != 0) {
        func_80150C78();
        D_800B5BD4 = 9;
        return;
    }
    D_800B5BD4 = 0xF;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80150C78);

void func_80150E9C(void) {
    func_80046318(0x35, 0x801B0000, 0x8144);
    func_80150010();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80150ED4);

void func_80151064(void) {
    func_80046318(0x3D, 0x801B0000, 0x8179);
    func_80150080();
    func_8004284C();
}

void func_8015109C(void) {
    func_800853FC();
    D_801206DF = D_800B593C;
    if ((u8) D_800B593C < 8U) {
        D_801206DB &= 0xFF7F;
    }
}

void func_801510E8(void) {
    /* one base symbol for the byte table: separate symbols let as1 hoist the lbu (wave 2 ETC note) */
    ((u8 *)&D_801206DA)[5] = 0x80;
    ((u8 *)&D_801206DA)[1] |= 0x80;
    *(s16 *)((u8 *)&D_801206DA + 6) = 0;
    *(s16 *)((u8 *)&D_801206DA + 0x14) = 4;
    *(s16 *)((u8 *)&D_801206DA + 0x16) = 0;
    D_801206DA = 1;
    func_8004284C();
}

void func_8015114C(void) {
    func_80044750(0x500);
}

void func_8015116C(void) {
    func_80044750(0x503);
}

void func_8015118C(void) {
    func_80044750(0x504);
    func_8004284C();
}

extern FnTbl40 D_8015EC24;

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_801511B4);

void func_80151300(void) {
    func_80044750(0x502);
    func_8004284C();
}

void func_80151328(void) {
    func_80046318(0x45, 0x801B0000, 0x81B6);
    func_80150130();
    func_8004284C();
}

extern FnTbl40 D_8015ECC4;

void func_80151360(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl40 tbl;

    tbl = D_8015ECC4;
    idx = D_800E738A;
    tbl.f[idx](0x80);
    if (D_80122D04 != 0) {
        D_801206DB |= 0x80;
        D_801206DF = D_800B593C;
        return;
    }
    D_801206DB &= 0xFF7F;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80151424);
