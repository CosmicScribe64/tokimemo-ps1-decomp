#include "common.h"
#include "ovl/KANGEI.h"

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_80134120);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_801341E8);

void func_8013425C(void) {
    func_80044890(1, 0xBF98, 0xBF79, (&D_800B3688)[D_800E71DF], (&D_800B36C8)[D_800E71DF], (&D_800B3708)[D_800E71DF]);
    if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_8004284C();
    }
    func_8004284C();
}

void func_801342EC(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_8004284C();
    }
    if ((u32) D_800E7384++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E738A - 1) & 0xFF);
    }
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_80134374);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_8013454C);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_80134724);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_801349D4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_80134BC4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_80134DB0);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_80134EA8);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_80134F00);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_80135004);

void func_801350E0(void) {
    RECT rect;

    func_80083474();
    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C884(&rect, (void *)0x80180000);
}

void func_8013512C(void) {
    D_800E643C[D_800E71DF].unk_06 = 0x32;
    func_80044750(0x24);
    func_80135438();
}

void func_80135178(void) {
    if (D_800E71DF == 6) {
        don_wait();
        return;
    }
    func_8004284C();
}

void func_801351B8(void) {
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x2E);
    if ((u8) D_800E71DF < 4U) {
        func_80044750(0x202);
    } else {
        func_80044750(0x201);
    }
    func_8004284C();
}

void func_80135220(void) {
    func_80044750(0x24);
    func_80044750(0x501);
    func_8004284C();
}

void func_80135250(void) {
    D_800E643C[D_800E71DF].unk_06 = 0x46;
    D_80139ADC = 3;
    func_801349D4();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_801352A4);

INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80134120.rodata", D_8013980C);

void func_80135314(void) {
    bg_read_sub2(0x42C6);
    func_80085B3C(0xE, 0xB);
    func_800AE0F0(D_800CA1DC, &D_8013980C);
    func_8004284C();
}

void func_8013535C(void) {
    bg_read_sub2(0x429F);
    func_80085B3C(0xE, 0xC);
    func_8004284C();
}

void func_80135390(void) {
    if ((D_80139AE0 == 2) && (D_800E71DF == 6)) {
        D_80139AE4 = 2;
    } else {
        D_80139AE4 = 0;
    }
    func_80133EF8();
    if ((D_80139AE0 == 3) && (D_800E7384++ == 0)) {
        if (D_800E71DF == 6) {
            func_80044750(0x503);
            func_80047550();
        }
    }
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_80135438);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_801354AC);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_80135570);
