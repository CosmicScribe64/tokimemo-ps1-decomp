#include "common.h"
#include "ovl/DATE.h"

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

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80150BCC);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80150C78);

void func_80150E9C(void) {
    func_80046318(0x35, 0x801B0000, 0x8144);
    func_80150010();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80150ED4);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80151064);

void func_8015109C(void) {
    func_800853FC();
    D_801206DF = D_800B593C;
    if ((u8) D_800B593C < 8U) {
        D_801206DB &= 0xFF7F;
    }
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_801510E8);

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

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80151360);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80151424);
