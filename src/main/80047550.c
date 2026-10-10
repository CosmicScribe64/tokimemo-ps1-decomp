#include "common.h"
#include "game.h"

void func_80047550(void) {
    D_801255DC = 4;
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80047560);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800476C0);

void tpage_buf_clear_all(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        if ((i & 0xF) < 5) {
            D_800E6280.unk_1228[i] = 0;
        } else {
            D_800E6280.unk_1228[i] = -1;
        }
    }
}

void tpage_buf_clear(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        if ((i & 0xF) < 5) {
            D_800E6280.unk_1228[i] = 0;
        } else {
            D_800E6280.unk_1228[i] = -1;
        }
    }
    _sys_default_tpage_set();
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", _sys_default_tpage_set);

INCLUDE_ASM("asm/nonmatchings/main/80047550", load_tpage_buf_lock);

INCLUDE_ASM("asm/nonmatchings/main/80047550", search_load_tpage_buf_lock);

s32 search_tpage_multi(s32 arg0, u8 arg1) {
    s32 j;
    s32 i;

    for (i = 0xF; i >= 5; i--) {
        for (j = 3; j >= 0; j--) {
            if (D_800E6280.unk_1128[i + j * 16] == 0x7FFE && D_800E6280.unk_1228[i + j * 16] == arg0) {
                return (j * 16 + i) & 0xFFFF;
            }
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (arg0 == D_800E6280.unk_1128[i]) {
            return i & 0xFFFF;
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (D_800E6280.unk_1128[i] == 0x7FFF) {
            D_800E6280.unk_1128[i] = arg0;
            if (i < 0x20) {
                load_csr_tp(arg0, i & 0xFF, arg1, 0, 0, 0x80, 0x80);
            } else {
                load_csr_tp(arg0, i & 0xFF, arg1, 0, 0x80, 0x80, 0x80);
            }
            return i & 0xFFFF;
        }
        if (i == 0x3F) {
            return 0x3F;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", search_tpage);

INCLUDE_ASM("asm/nonmatchings/main/80047550", search_tpage8);

void func_80048CF8(void) {
    s32 i;

    if (D_800E6280.unk_F80 & 2) {
        printf("PRINT_TPAGE_BUF\n");
        for (i = 0; i < 0x40; i++) {
            printf("[%d]%08x", i, D_800E6280.unk_1228[i]);
            if ((i & 3) == 3) {
                printf("\r");
            }
        }
    }
}

void func_80048DAC(s32 arg0) {
    if (arg0 != 0) {
        D_800E6280.unk_1112 = 1;
    } else {
        D_800E6280.unk_1112 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048DD0);

void func_80048E78(void) {
    s32 i;

    for (i = 0x60; i < 0xA0; i++) {
        func_80048F64(i);
    }
}

void func_80048EB8(s32 arg0) {
    s32 n;
    s32 i;
    s32 a;
    s32 b;
    s32 c;

    a = D_8011ECF6;
    b = D_8011ECFA;
    c = D_8011ECE8;
    if (arg0 == 0) {
        n = 0xA0;
    } else {
        n = (arg0 >= 0xA1) ? 0xA0 : arg0;
    }
    for (i = 1; i < n; i++) {
        func_80048F64(i);
    }
    D_8011ECF6 = a;
    D_8011ECFA = b;
    D_8011ECE8 = c;
}

void func_80048F64(s32 arg0) {
    u8 *p;

    if (arg0 < 0xA0) {
        p = D_8011ECD0 + arg0 * 0x44;
        p[0] = 0;
        *(s32 *)(p + 0x38) = 0;
        p[1] = 0;
        p[4] = 0;
        p[5] = 0;
        p[2] = 0;
        p[3] = 0;
        p[6] = 1;
        *(s16 *)(p + 8) = 0;
        *(s32 *)(p + 0xC) = (s32)D_800C975C;
        *(s32 *)(p + 0x10) = (s32)D_800C9A14;
        *(s32 *)(p + 0x34) = (s32)D_800C9730;
        *(s16 *)(p + 0x14) = 0;
        *(s16 *)(p + 0x16) = 0;
        *(s16 *)(p + 0x18) = 0;
        *(s16 *)(p + 0x1A) = 0x1000;
        *(s16 *)(p + 0x1C) = 0x1000;
        p[0x43] = 0;
        p[7] = 0x80;
        *(s32 *)(p + 0x20) = 0;
        *(s32 *)(p + 0x24) = 0;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0;
        *(s32 *)(p + 0x30) = 0;
        p[0x40] = 0;
        p[0x41] = 0;
        p[0x42] = 0;
    }
}

u8 func_8004901C(void) {
    return D_800E6280.unk_03A;
}

void func_8004902C(void) {
    D_800E6280.unk_03A = 0x80;
    D_800E6280.unk_03B = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80049044);

void func_800490B4(u8 arg0) {
    D_800E6280.unk_03B = arg0;
}
