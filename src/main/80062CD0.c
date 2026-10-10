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

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80066A2C);

s32 func_80066A68(void) {
    return 1;
}

s32 func_80066A70(void) {
    return 2;
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80066A78);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80066A84);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80066AC0);

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

void func_800674B0(void) {
    u8 v;
    s32 i;

    func_80044750(5);
    func_80044750(0x71);
    v = D_800E62BD;
    if (D_800E62BE == 0x60 && D_800E62BF == 9 && D_800E62C0 == 1) {
        v = 1;
    }
    switch (v) {
    case 0:
        i = func_80066A2C() & 3;
        func_80044890(0, 0xBF98, 0xBF79, D_800B5BE8[i], D_800B5BF8[i], D_800B5BD8[i]);
        break;
    case 1:
        func_80044890(0, 0xBF98, 0xBF79, 0xD7D1, 0xD7AF, 0xD7A4);
        break;
    case 2:
        func_80044890(0, 0xBF98, 0xBF79, 0xD73F, 0xD71E, 0xD717);
        break;
    case 3:
        func_80044890(0, 0xBF98, 0xBF79, 0xD780, 0xD752, 0xD743);
        break;
    }
}

void func_80067610(void) {
    if (func_80044E8C() == 1) {
        func_8004500C(0, 0);
        func_80042808();
    }
}

void func_8006764C(s32 arg0) {
    /* D_8011F443 is element 1 (stride 0x44) of the table at D_8011F3FF; the array form keeps IDO from hoisting the second load */
    if (arg0) {
        D_8011F3FF[0] |= 0x80;
        D_8011F3FF[0x44] |= 0x80;
    } else {
        D_8011F3FF[0] &= 0x7F;
        D_8011F3FF[0x44] &= 0x7F;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_800676AC);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80067870);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80067C28);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80067DD4);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80067DFC);

void func_80067E34(void) {
    func_80048F64(0x60);
    D_80120651 = 4;
    D_80120652 = 0x40;
    D_80120688 = 0x40000000;
    D_80120653 = 0x84;
    D_8012065C = D_800C975C;
    D_80120660 = D_800C9A14;
    D_80120684 = D_800C9730;
    D_80120664 = D_800C9A54;
    D_80120654 = 8;
    D_80120655 = 1;
    D_80120668 = 0x43;
    D_80120676 = -0x10;
    D_8012067A = -0x13;
    func_80067F04();
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80067F04);

s16 func_80068898(s32 arg0, s32 arg1) {
    s32 v = D_800E62E4[arg0 * 0x40];

    if (v == 6) {
        v -= 7;
    }
    return arg0 * 0x88 + ((v + arg1 + 1) % 7) * 16 - 0x78;
}

s16 func_800688F0(s32 arg0, s32 arg1) {
    s32 v = D_800E62E4[arg0 * 0x40];

    if (v == 6) {
        v -= 7;
    }
    return ((v + arg1 + 1) / 7) * 16 - 0x30;
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80068938);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80068BE4);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80068EC0);

void cal_base_show(void) {
    func_80049A40(-0xA0, -0x57, 0x11B, 0x7D, 5, 0xE0E0E0, 2);
    dtd_on(5);
}

void appraisal_base_show(void) {
    func_80049A40(-0xA0, -0x68, 0x11B, 0x8E, 5, 0xE0E0E0, 2);
    dtd_on(5);
}

void magazine_base_show(void) {
    func_80049A40(-0xA0, -0x78, 0x140, 0x9E, 5, 0xE0E0E0, 2);
    dtd_on(5);
}

/* FAKE: bit 31 of the first four 36-byte entries of the table at D_801217D0 is reached by indexing the first symbol; separate scalar names let IDO hoist the loads above the stores. Real source unknown. T-2090 */
void cal_sprite_disp_switch(s32 arg0) {
    if (arg0 == 1) {
        D_801217D0 &= 0x7FFFFFFF;
        (&D_801217D0)[9] &= 0x7FFFFFFF;
        (&D_801217D0)[18] &= 0x7FFFFFFF;
        (&D_801217D0)[27] &= 0x7FFFFFFF;
        return;
    }
    D_801217D0 |= 0x80000000;
    (&D_801217D0)[9] |= 0x80000000;
    (&D_801217D0)[18] |= 0x80000000;
    (&D_801217D0)[27] |= 0x80000000;
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", cal_sprite_init);

void data_save_load_class_init(void) {
    func_80048F64(0x60);
    D_80120651 = 3;
    D_80120652 = 0x40;
    D_80120688 = 0x40000000;
    D_80120653 = 0x84;
    D_8012065C = D_800C975C;
    D_80120660 = D_800C9A14;
    D_80120684 = D_800C9730;
    D_80120664 = D_800C9A54;
    D_80120654 = 8;
    D_80120655 = 1;
    D_80120668 = 0x43;
    D_80120676 = -0x10;
    D_8012067A = -0x13;
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_800696DC);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006A044);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006A2CC);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006AEC4);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006B014);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006B0C8);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006B5BC);

void func_8006B640(void) {
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006B648);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006B900);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006BA40);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006BC28);

void func_8006BD6C(s32 arg0) {
    if (arg0 == 1) {
        D_8011ECD3 |= 0x80;
    } else {
        D_8011ECD3 &= 0x7F;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006BDA8);

void func_8006C308(void) {
    func_8006C334(1, 1, 1, 2);
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006C334);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006C700);

void func_8006C760(s32 arg0) {
    func_80048F64(0xF);
    D_8011F0CD = 0;
    D_8011F0CE = 0x40;
    D_8011F104 = 0;
    if (arg0) {
        D_8011F0CF = 0x8C;
    } else {
        D_8011F0CF = 0;
    }
    D_8011F0D8 = D_800C975C;
    D_8011F0DC = D_800C9A14;
    D_8011F100 = D_800C9730;
    D_8011F0E0 = D_800C9A54;
    D_8011F0D0 = 8;
    D_8011F0D1 = 1;
    D_8011F10F = 0;
    D_8011F0E2 = 0;
    D_8011F0E4 = 0xAD;
    D_8011F0F2 = -0x86;
    D_8011F0F6 = -0x5C;
}

void func_8006C848(s32 arg0) {
    func_80048F64(2);
    D_8011ED59 = 2;
    D_8011ED5A = 0x40;
    D_8011ED90 = 0x40000000;
    D_8011ED64 = D_800C975C;
    D_8011ED68 = D_800C9A14;
    D_8011ED8C = D_800C9730;
    D_8011ED70 = 1;
    D_8011ED5C = 8;
    D_8011ED5D = 1;
    D_8011ED7E = 0x74;
    D_8011ED82 = 0x48;
    if (arg0 == 1) {
        D_8011ED5B |= 0x80;
    } else {
        D_8011ED5B &= 0x7F;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006C934);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006C988);

u8 *func_8006CA9C(void) {
    if (D_800E71F4 == 0) {
        if (D_800E71F5 != 0) {
            return D_800B672C;
        }
        return D_800B6730;
    }
    return D_800B6724;
}

u8 *func_8006CAE0(void) {
    if (D_800E71F4 == 0) {
        if (D_800E71F5 != 0) {
            return D_800B6730;
        }
        return D_800B672C;
    }
    return D_800B6728;
}
