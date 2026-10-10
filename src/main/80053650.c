#include "common.h"
#include "game.h"

void func_80053650(s32 arg0, s32 arg1, s32 arg2) {
    DecDCTReset(0);
    func_800869C8(0);
    func_800869A4(0);
    D_80125C08 = arg0;
    D_80125C0C = arg1;
    D_80125C04 = arg2;
}

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

u8 Sw_Test(void) {
    D_800E6280.unk_1115 = 0;
    if (TestEvent(D_8011ECAC) != 0) {
        D_800E6280.unk_1115 = 1;
    } else if (TestEvent(D_8011ECB0) != 0) {
        D_800E6280.unk_1115 = 2;
    } else if (TestEvent(D_8011ECB4) != 0) {
        D_800E6280.unk_1115 = 4;
    } else if (TestEvent(D_8011ECB8) != 0) {
        D_800E6280.unk_1115 = 8;
    }
    if (D_800E6280.unk_1115 != 0) {
        Sw_Clear();
    }
    return D_800E6280.unk_1115;
}

void Sw_Clear(void) {
    TestEvent(D_8011ECAC);
    TestEvent(D_8011ECB0);
    TestEvent(D_8011ECB4);
    TestEvent(D_8011ECB8);
}

u8 Hw_Test(void) {
    D_800E6280.unk_1115 = 0;
    do {
        if (TestEvent(D_8011ECBC) != 0) {
            D_800E6280.unk_1115 = 0x81;
        } else if (TestEvent(D_8011ECC0) != 0) {
            D_800E6280.unk_1115 = 0x82;
        } else if (TestEvent(D_8011ECC4) != 0) {
            D_800E6280.unk_1115 = 0x84;
        } else if (TestEvent(D_8011ECC8) != 0) {
            D_800E6280.unk_1115 = 0x88;
        }
    } while (D_800E6280.unk_1115 == 0);
    Hw_Clear();
    return D_800E6280.unk_1115;
}

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
    D_800E6280.unk_1115 = 0;
    D_800E6280.unk_111C = arg0;
}

void func_80053CC0(void) {
    D_800E6280.unk_1115 = 0;
    D_800E6280.unk_111C += 1;
}

void func_80053CE0(void) {
    D_800E6280.unk_035 = 1;
    D_800E6280.unk_111C = 0;
    D_800E6280.unk_1115 = 0;
    D_800E6280.unk_111D = 0;
    D_800E6280.unk_1120 = 0;
}

void func_80053D10(void) {
    D_800E6280.unk_035 = 0;
    D_800E6280.unk_1120 = 0;
}

void func_80053D24(u8 arg0) {
    switch (Sw_Test()) {
    case 1:
        func_80053CC0();
        return;
    case 8:
        func_80053CAC(arg0);
        return;
    case 4:
        D_800E6280.unk_111D += 1;
        func_80053CAC(0);
        if ((u32)D_800E6280.unk_111D >= 3) {
            D_800E6280.unk_035 = 0;
            D_800E6280.unk_1115 = 0xFF;
            return;
        }
    case 0:
        return;
    default:
        func_80053CAC(0);
        break;
    }
}

s32 func_80053DDC(void) {
    s32 status;

    status = _card_status(0);
    while (status == 0) {
        status = _card_status(0);
    }
    switch (status) {
    case 1:
        close(D_800B58E4);
        break;
    case 2:
        close(D_800B58E4);
        break;
    case 4:
        close(D_800B58E4);
        break;
    case 8:
        close(D_800B58E4);
        break;
    case 0x11:
        D_800E6280.unk_111D += 1;
        func_80053CAC(0);
        close(D_800B58E4);
        break;
    case 0x21:
        D_800E6280.unk_111D += 1;
        func_80053CAC(0);
        close(D_800B58E4);
        break;
    }
    return status;
}

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80053F04);

s32 func_80054108(void) {
    if (format((u8 *)"bu00:") == 1) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054140);

u8 func_80054284(void) {
    D_800E6280.unk_1120 += 1;
    if (D_800E6280.unk_035 != 0) {
        switch (D_800E6280.unk_111C) {
        case 0:
            Sw_Clear();
            D_800E6280.unk_1118 = 0;
            if (func_8005478C(0) == 1) {
                func_80053CC0();
            } else {
                D_800E6280.unk_1115 = 0xD0;
                D_800E6280.unk_035 = 0;
            }
            break;
        case 1:
            if (func_80053DDC() == 1) {
                func_80053CC0();
            }
            break;
        case 2:
            D_800E6280.unk_1115 = 3;
            D_800E6280.unk_035 = 0;
            break;
        case 16:
            D_800E6280.unk_035 = 0;
            D_800E6280.unk_1115 = 0xF0;
            break;
        }
    }
    return D_800E6280.unk_1115;
}

s32 func_80054388(void) {
    D_800E6280.unk_1120 += 1;
    if (D_800E6280.unk_035 != 0) {
        switch (D_800E6280.unk_111C) {
        case 0:
            Sw_Clear();
            D_800E6280.unk_1118 = 0;
            if (func_80054884(0) == 1) {
                func_80053CC0();
            } else {
                D_800E6280.unk_1115 = 0xD0;
                D_800E6280.unk_035 = 0;
            }
            break;
        case 1:
            if (func_80053DDC() == 1) {
                func_80053CC0();
            }
            break;
        case 2:
            D_800E6280.unk_1115 = 0x13;
            D_800E6280.unk_035 = 0;
            break;
        case 16:
            D_800E6280.unk_035 = 0;
            D_800E6280.unk_1115 = 0xF0;
            break;
        }
    }
    return (s32) D_800E6280.unk_1115;
}

u8 func_8005448C(void) {
    D_800E6280.unk_1120 += 1;
    if (D_800E6280.unk_035 != 0) {
        switch (D_800E6280.unk_111C) {
        case 0:
            Sw_Clear();
            if (func_8005478C(D_800E6280.unk_1118) == 1) {
                func_80053CC0();
            } else {
                D_800E6280.unk_1115 = 0xD0;
                D_800E6280.unk_035 = 0;
            }
            break;
        case 1:
            if (func_80053DDC() == 1) {
                func_80053CC0();
            }
            break;
        case 2:
            D_800E6280.unk_1115 = 4;
            D_800E6280.unk_035 = 0;
            break;
        case 16:
            D_800E6280.unk_035 = 0;
            D_800E6280.unk_1115 = 0xF0;
            break;
        }
    }
    return D_800E6280.unk_1115;
}

u8 func_80054590(void) {
    D_800E6280.unk_1120 += 1;
    if (D_800E6280.unk_035 != 0) {
        switch (D_800E6280.unk_111C) {
        case 0:
            Sw_Clear();
            if (func_80054884(D_800E6280.unk_1118) == 1) {
                func_80053CC0();
            } else {
                D_800E6280.unk_1115 = 0xD0;
                D_800E6280.unk_035 = 0;
            }
            break;
        case 1:
            if (func_80053DDC() == 1) {
                func_80053CC0();
            }
            break;
        case 2:
            D_800E6280.unk_1115 = 0x14;
            D_800E6280.unk_035 = 0;
            break;
        case 16:
            D_800E6280.unk_035 = 0;
            D_800E6280.unk_1115 = 0xF0;
            break;
        }
    }
    return D_800E6280.unk_1115;
}

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

s32 func_80054BE4(void) {
    s32 i;
    s32 n;
    u8 *p;
    s32 t;

    n = 0;
    p = D_800E7D11;
    i = 1;
    do {
        t = func_80054AF4(i);
        i++;
        p++;
        p[3] = t;
        n++;
    } while (i != 0xF);
    return n;
}

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
