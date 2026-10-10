#include "common.h"
#include "game.h"

void func_800789E0(void) {
    D_800B6D30 = 0;
    D_800B6D34 = 0;
    D_800B6D38 = 0;
    D_800B6D3C = 0;
    D_800B6D40 = 0;
}

void func_80078A0C(void) {
    u8 sel = D_800B6D34; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        func_80078C48();
        break;
    case 1:
        func_80078FE0();
        break;
    case 2:
        func_80079014();
        break;
    case 3:
        func_80079070();
        break;
    }
    func_80078A94();
}

void func_80078A94(void) {
    s32 pad; /* FAKE: unused local above the spill slot of n, the original has the spill at sp+0x28 (T-3330) */
    s32 n;

    n = 0;
    if (D_8011F113 & 0x80) {
        hizuke_show();
        n = 1;
    }
    if (D_8011ED17 & 0x80) {
        message_window_show();
        n += 1;
    }
    if (D_8011F553 & 0x80) {
        parameter_show();
        n += 1;
    }
    if (*D_8011F3FF & 0x80) {
        func_80067870();
        n += 1;
    }
    if (D_8011ECD3 & 0x80) {
        func_8006BA40();
        n += 1;
    }
    if (D_800E6280.unk_110E == 0x45) {
        cal_base_show();
    }
    if (n != 0) {
        func_80066C08(0);
        switch (D_800B6D30) {
        case 0:
            if (D_80122D44 & 1) {
                func_8006B900();
            }
            if (D_80122D44 & 2) {
                func_80066334();
            }
            func_800578F4(get_last_gamen_mode());
            break;
        case 1:
            func_80057710();
            func_800578F4(1);
            break;
        default:
            back_clear_switch(1);
            break;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_80078C48);

void func_80078FE0(void) {
    if (func_800460CC() & 1) {
        D_800B6D34 = 2;
    }
}

void func_80079014(void) {
    if (func_8004636C(D_800B6D38, D_800B6D3C) != 0) {
        D_800B6D34 = 3;
        func_80042458();
    } else {
        D_800B6D34 = 0;
        D_800B6D40 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_80079070);

s32 func_80079524(void) {
    if (D_800E6280.unk_1108 == 0xD0 || D_800E6280.unk_1108 == 0xD1) {
        return 1;
    }
    return 0;
}

s32 func_80079554(void) {
    if (D_800E6280.unk_1108 == 0x90) {
        return 1;
    }
    return 0;
}

s32 func_80079578(void) {
    if (D_800E6280.unk_1108 == 0x21) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_8007959C);

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_80079800);
