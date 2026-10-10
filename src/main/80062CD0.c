#include "common.h"
#include "game.h"
#include "main_only.h"

void func_80062CD0(s32 arg0) {
    func_80046318(0x2D, 0x80180000, arg0);
    D_8011F50F = 0;
    D_8011F4CB = 0;
    D_800B5A60 = 0;
}

/* FAKE: indexed views of D_8011F50F and D_801217D0 stop IDO from hoisting the later loads above the stores (see cal_sprite_disp_switch). Real source unknown. T-2090 */
void func_80062D0C(s32 arg0) {
    if (arg0 == 0) {
        D_8011F50F &= 0x7F;
        (&D_8011F50F)[-0x44] &= 0x7F;
        D_801217D0 |= 0x80000000;
        (&D_801217D0)[9] |= 0x80000000;
        (&D_801217D0)[18] |= 0x80000000;
        (&D_801217D0)[27] |= 0x80000000;
        (&D_801217D0)[36] |= 0x80000000;
        (&D_801217D0)[45] |= 0x80000000;
    }
}
INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80062DBC);

void func_800634FC(void) {
    if ((u32)D_800B5A64 >= 0xA5) {
        D_800B5A64 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80063520);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80063668);

void bustup_speech(void) {
    /* D_800E62BC is declared s8 in game.h; this function reads it as u8 (lbu) */
    if (*(u8 *)&D_800E62BC == 0 && !(D_8011F50E & 1)) {
        /* FAKE: the no-op "& 0xFF" shifts IDO's temp numbering (ori in $t9, as in the original); T-1010 */
        D_8011F50E = (D_8011F50E & 0xFF) | 0x11;
    }
}

void bustup_wink(void) {
    if (!(D_8011F4CA & 1)) {
        /* FAKE: the no-op "& 0xFF" shifts IDO's temp numbering (ori in $t8, as in the original); T-1010 */
        D_8011F4CA = (D_8011F4CA & 0xFF) | 0x11;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", parameter_change);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", parameter_disp_switch);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", parameter_show_init);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", parameter_show);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", hizuke_disp_switch);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", hizuke_init);

void hizuke_show(void) {
    if (D_8011F113 & 0x80) {
        func_80049A40(0x86, -0x4B, 0x14, 0x72, 5, 0x30E00, 0);
        dtd_on(5);
    }
}

void message_disp_switch(s32 arg0) {
    if (arg0 == 1) {
        D_8011ED17 |= 0x80;
    } else {
        D_8011ED17 &= 0x7F;
    }
}

void message_window_init(void) {
    func_80048F64(1);
    D_8011ED15 = 7;
    D_8011ED16 = 0x40;
    D_8011ED4C = 0x40000000;
    D_8011ED17 = 0x84;
    D_8011ED20 = D_800C975C;
    D_8011ED24 = D_800C9A14;
    D_8011ED48 = D_800C9730;
    D_8011ED28 = D_800C9A54;
    D_8011ED18 = 8;
    D_8011ED19 = 1;
    D_8011ED2C = 0x3E;
    D_8011ED3A = 0;
    D_8011ED3E = 0x4B;
}

void message_window_show(void) {
    if (D_8011ED17 & 0x80) {
        func_80049A40(-0x85, 0x2E, 0x10A, 0x34, 8, 0x30E00, 0);
        dtd_on(8);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", icon_disp_switch);

void icon_can_use_set(s32 idx, s32 on) {
    if (on) {
        D_80125C90[idx] = 1;
    } else {
        D_80125C90[idx] = 0;
    }
}

s32 get_icon_can_use(s32 arg0) {
    if (D_80125C90[arg0] == 1) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006509C);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_800654D0);

void func_80065900(u8 arg0) {
    /* D_8012059A[1] is D_8012059C; the array form keeps IDO from hoisting the load above the first store */
    if (arg0) {
        D_8012059A[0] = 0xE;
        if (D_8012059A[1] >= 4) {
            D_8012059A[1] = 0;
        }
    } else {
        D_8012059A[0] = 0xD;
        if (D_8012059A[1] >= 6) {
            D_8012059A[1] = 0;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80065964);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80065B0C);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80065F34);

void func_80066104(void) {
    func_8006612C();
    D_80125CA0 = 0xB8;
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006612C);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80066334);

s32 func_80066A2C(void) {
    switch (D_800E62BF) {
    case 3:
    case 4:
    case 5:
    default:
        return 0;
    case 6:
    case 7:
    case 8:
        return 1;
    case 9:
    case 10:
    case 11:
        return 2;
    case 1:
    case 2:
    case 12:
        return 3;
    }
}

s32 func_80066A84(void) {
    switch (D_800E62BF) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 10:
    case 11:
    case 12:
    default:
        return 0;
    case 6:
    case 7:
    case 8:
    case 9:
        return 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80066ACC);

s32 func_80066B40(s32 arg0, s32 arg1) {
    if (arg0 == 8) {
        return 1;
    }
    if (arg0 == 7 && arg1 >= 0x18) {
        return 1;
    }
    if (arg0 == 0xC && arg1 >= 0x19) {
        return 2;
    }
    if (arg0 == 0xD && arg1 < 8) {
        return 2;
    }
    if (arg0 == 1 && arg1 < 8) {
        return 2;
    }
    if (arg0 == 3 && arg1 >= 0x18) {
        return 3;
    }
    if (arg0 == 4 && arg1 < 4) {
        return 3;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80066C08);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_800673B8);

void func_80067438(void) {
    s32 v;

    func_80044750(5);
    func_80044750(0x71);
    v = func_80066A2C() & 3;
    func_80044890(0, 0xBF98, 0xBF79, D_800B5BE8[v], D_800B5BF8[v], D_800B5BD8[v]);
}
