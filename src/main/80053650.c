#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80053650);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_800536AC);

void card_ev_set(void) {
    Sw_Start();
    Hw_Start();
    InitCARD(1);
    StartCARD();
    _bu_init();
    _card_auto(0);
}

void Sw_Start(void) {
    EnterCriticalSection();
    D_8011ECAC = OpenEvent(0xF4000001, 4, 0x2000, 0);
    D_8011ECB0 = OpenEvent(0xF4000001, 0x8000, 0x2000, 0);
    D_8011ECB4 = OpenEvent(0xF4000001, 0x100, 0x2000, 0);
    D_8011ECB8 = OpenEvent(0xF4000001, 0x2000, 0x2000, 0);
    EnableEvent(D_8011ECAC);
    EnableEvent(D_8011ECB0);
    EnableEvent(D_8011ECB4);
    EnableEvent(D_8011ECB8);
    ExitCriticalSection();
}

void Hw_Start(void) {
    EnterCriticalSection();
    D_8011ECBC = OpenEvent(0xF0000011, 4, 0x2000, 0);
    D_8011ECC0 = OpenEvent(0xF0000011, 0x8000, 0x2000, 0);
    D_8011ECC4 = OpenEvent(0xF0000011, 0x100, 0x2000, 0);
    D_8011ECC8 = OpenEvent(0xF0000011, 0x2000, 0x2000, 0);
    EnableEvent(D_8011ECBC);
    EnableEvent(D_8011ECC0);
    EnableEvent(D_8011ECC4);
    EnableEvent(D_8011ECC8);
    ExitCriticalSection();
}

INCLUDE_ASM("asm/nonmatchings/main/80053650", Sw_Test);

void Sw_Clear(void) {
    TestEvent(D_8011ECAC);
    TestEvent(D_8011ECB0);
    TestEvent(D_8011ECB4);
    TestEvent(D_8011ECB8);
}

INCLUDE_ASM("asm/nonmatchings/main/80053650", Hw_Test);

void Hw_Clear(void) {
    TestEvent(D_8011ECBC);
    TestEvent(D_8011ECC0);
    TestEvent(D_8011ECC4);
    TestEvent(D_8011ECC8);
}

void Hw_Stop(void) {
    EnterCriticalSection();
    CloseEvent(D_8011ECBC);
    CloseEvent(D_8011ECC0);
    CloseEvent(D_8011ECC4);
    CloseEvent(D_8011ECC8);
    ExitCriticalSection();
}

void func_80053CAC(u8 arg0) {
    D_800E7395 = 0;
    D_800E739C = arg0;
}

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80053CC0);

void func_80053CE0(void) {
    D_800E62B5 = 1;
    D_800E739C = 0;
    D_800E7395 = 0;
    D_800E739D = 0;
    D_800E73A0 = 0;
}

void func_80053D10(void) {
    D_800E62B5 = 0;
    D_800E73A0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80053D24);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80053DDC);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80053F04);

s32 func_80054108(void) {
    if (format(D_800AFBF0) == 1) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054140);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054284);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054388);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_8005448C);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054590);

s32 func_80054694(s32 arg0) {
    FileReq req;
    s32 fd;

    func_80056070(req.name, arg0);
    close(D_800B58E4);
    fd = open(req.name, 0x10200);
    D_800B58E4 = fd;
    if (fd == -1) {
        return -1;
    }
    close(D_800B58E4);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054704);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_8005478C);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054884);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054AF4);

void func_80054BA8(void) {
    if (func_80054AF4(0) != 0) {
        D_800E8BEE = 1;
        return;
    }
    D_800E8BEE = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054BE4);

void func_80054C4C(void) {
    bzero(D_80123120, 0x2000);
}

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054C74);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054E6C);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054FC0);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_800552DC);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_8005557C);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80055A38);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80055AFC);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80056070);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80056284);
