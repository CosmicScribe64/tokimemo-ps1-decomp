#include "common.h"
#include "ovl/ENDING.h"

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133540", func_80133540);

void func_801335F4(void) {
    if (D_8013C3E0 == 0xC) {
        func_80083440(4);
    } else {
        func_80083440(0);
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133540", func_8013363C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133540", func_801336A8);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133540", func_80133738);

void func_801337A4(void) {
    RECT rect;
    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C8E0(&rect, (void *)0x80180000);
    func_8004284C();
}

void func_801337F0(void) {
    if (D_800CA14C == 4) {
        func_80083440(4);
        load_palette(D_800CA130, 0x10, 1, 2, 1);
    }
    normal_date_speak();
    if (D_800CA14C == 7) {
        D_80122D20 = 1;
        return;
    }
    D_80122D20 = 0;
}

void func_80133870(void) {
    if (D_800E69DD == 0xE) {
        func_80062CD0(0x6B24);
    } else {
        func_80062CD0(0x5828);
    }
    func_8004284C();
}

void func_801338B8(void) {
    load_palette(D_800CA130, 0x10, 1, 2, 1);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133540", func_801338F8);

INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/80133540.rodata", D_8013BFB8);

void func_801339C0(void) {
    D_800E69DD = 0xC;
    func_800AE0F0(D_800CA188, D_8013BFB8);
    func_8004284C();
}

INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/80133540.rodata", D_8013BFC0);

void func_80133A00(void) {
    D_800E69DD = 0xE;
    func_800AE0F0(D_800CA188, D_8013BFC0);
    func_8004284C();
}

void func_80133A40(void) {
    D_800E71DF = D_8013C3E0;
    func_8004284C();
}

void func_80133A6C(void) {
    if (D_800E71DF >= 0xDU) {
        func_80042908(7);
        D_800E7D34 |= 8;
        return;
    }
    func_8004284C();
    D_800E7D34 |= 4;
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133540", func_80133AD0);

void func_80133B80(void) {
    D_800CA148 = 0;
    D_800CA14C = 0;
    func_80132000();
    D_800CA160 = D_8013C2B4;
    D_800CA164 = D_8013C2F8;
    D_800CA168 = D_8013C33C;
    func_8004284C();
}

void func_80133BE0(void) {
    bg_read_sub2(0x4055);
    func_8004284C();
}
