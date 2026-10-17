#include "common.h"
#include "ovl/KANGEI.h"

typedef struct KangeiControlPair {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
} KangeiControlPair; /* size 0x02; byte pair at D_80120652, T-10030 */

void func_801355F0(void) {
    D_80139D10[0].unk_00 = 0x801D4074;
    D_80139D10[0].unk_04 = 0x801D4078;
    D_80139D10[0].unk_08 = 0x801D4088;
    D_80139D10[0].unk_0C = *(s16 *)0x801D4094;
}

void func_80135634(void) {
    D_80139D10[1].unk_00 = 0x801D4074;
    D_80139D10[1].unk_04 = 0x801D4078;
    D_80139D10[1].unk_08 = 0x801D4088;
    D_80139D10[1].unk_0C = *(s16 *)0x801D4094;
}

/* The original leaves the K&R result in v0 without an explicit return. */
s32 func_80135678(void) {
    s32 tick;
    KangeiDispatchTable80 table;
    u8 girl;

    table = D_80139D30;
    table.functions[D_800E6280.unk_110A](0x80);
    girl = D_800E6280.unk_F5F;
    /* FAKE: the condition and store share a source line to retain the original
     * timer/girl address-load scheduling. T-10030. */
    if ((D_8011F4CA & 0x10) && (girl != 5)) { D_8011F4CA = 3;
    }
    tick = ((u32)D_800E6280.unk_1100++ % 300U) == 0;
    if (tick != 0) {
        if (D_8011F50F & 0x80) {
            if ((girl == 10) || (girl == 3)) {
                ((KangeiControlPair *)&D_80120652)->unk_00 = 9;
                ((KangeiControlPair *)&D_80120652)->unk_01 |= 0x80;
            }
        }
    }
}

void func_801357A4(void) {
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_8004284C();
}

void func_801357D0(void) {
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x2E);
    func_8004284C();
}

void func_80135808(void) {
    func_80044750(0x24);
    func_80044750(0x502);
    func_8004284C();
}

/* FAKE: the date checks read the GameState fields through the pointer g; read directly, IDO swaps $v0/$v1
 * between the date word and unk_040 (register-cheatsheet date-check row). T-18400 */
s32 func_80135838(void) {
    GameState *g;

    g = &D_800E6280;
    if ((u8) D_80139ABC != 0) {
        func_80042808();
        return 0;
    }
    if (D_800E6280.unk_75D == 0xFF) {
        func_80042808();
        return 0;
    }
    func_800AE120(D_800E6280.unk_10F8);
    if (((func_800AE0D0() % 256) >= 5) || (D_800E6280.unk_75D >= 0xB)) {
        func_80042808();
        return 0;
    }
    if (!D_800E6280.unk_1BC[D_800E6280.unk_75D].unk_0C.f.b1) {
        func_80042808();
        return 0;
    }
    if ((((u32) (g->unk_1BC[0].unk_10.w << 0x17) >> 0x1B) == g->unk_040) && ((g->unk_1BC[0].unk_10.w & 0xF) == g->unk_03F)) {
        func_80042808();
        return 0;
    }
    if ((((u32) (g->unk_0F8 << 0x17) >> 0x1B) == g->unk_040) && ((g->unk_0F8 & 0xF) == g->unk_03F)) {
        func_80042808();
        return 0;
    }
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    D_80139ABC = 1;
    *(s32 *)&D_800E6280.unk_0FC /= 2;
    *(s32 *)&D_800E6280.unk_1BC[D_800E6280.unk_75D].unk_08 = 0;
    back_clear_switch(1);
    func_80048EB8(0);
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    message_window_init();
    hizuke_init();
    tpage_buf_clear();
    func_8004E58C();
    k_reset(1);
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    func_8008585C();
    parameter_disp_switch(0);
    icon_disp_switch(0);
    D_80139ADC = 0;
    D_80139AE0 = 0;
    addr_init_bustup();
    func_80084E4C();
    func_800847B8(D_800E6280.unk_F5F);
    func_80132E74();
    func_801349D4();
    D_80139AC0 = 0;
    func_80085B3C(0xE, 0xD);
    return func_8004284C();
}

void func_80135AC0(void) {
    func_8004284C();
}

void func_80135AE0(void) {
    func_80042808();
    func_80042808();
    func_80042808();
    func_80042808();
}

INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/801355F0.rodata", D_801398B0);

void func_80135B18(void) {
    bg_read_sub2(0x42A9);
    func_800AE0F0(D_800CA19C, &D_801398B0);
    func_8004284C();
}

void func_80135B54(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        func_80062CD0(0x5747);
        break;
    case 1:
        func_80062CD0(0x5ED6);
        break;
    case 2:
        func_80062CD0(0x5396);
        break;
    case 3:
        func_80062CD0(0x5A44);
        break;
    case 4:
        func_80062CD0(0x6881);
        break;
    case 5:
        func_80062CD0(0x6179);
        break;
    case 6:
        func_80062CD0(0x63EF);
        break;
    default:
        func_80062CD0(0x6ACA);
        break;
    }
    func_8004284C();
}

void func_80135C18(void) {
    func_8007E390();
    if ((D_800E6280.unk_F5F == 0xA) || (D_800E6280.unk_F5F == 3)) {
        ((DisplayActor44 *)D_8011ECD0)[96].unk_03 |= 0x80;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = ((DisplayActor44 *)D_8011ECD0)[31].unk_07;
    }
}

/* FAKE: a no-value s32 return preserves the original early RA reload.
 * The intended return type is unknown. T-9520. */
s32 func_80135C78(void) {
    normal_date_girl_out();
    if ((D_800E6280.unk_F5F == 0xA) || (D_800E6280.unk_F5F == 3)) {
        D_80120657 = D_8011F513;
        if ((u8) D_8011F513 < 8U) {
            D_80120653 &= 0x7F;
        }
    }
}

void func_80135CE0(void) {
    if (D_800E6280.unk_F5F == 0xA) {
        func_80048F64(0x60);
        D_80120651 = 0xB;
        D_80120652 = 0;
        D_80120688 = 0x41000000;
        D_80120653 = 4;
        D_8012065C = (u8 *) D_80139D10[0].unk_04;
        D_80120660 = (u8 *) D_80139D10[0].unk_08;
        D_80120684 = (u8 *) D_80139D10[0].unk_00;
        D_80120664 = D_80139D10[0].unk_0C;
        D_80120666 = 0;
        D_80120668 = 0;
        D_80120656 = 2;
        D_80120693 = 0x10;
        D_80120676 = 0;
        D_8012067A = -0x20;
        D_80120657 = 0;
    } else if (D_800E6280.unk_F5F == 3) {
        func_80048F64(0x60);
        D_80120651 = 0xB;
        D_80120652 = 0;
        D_80120688 = 0x41000000;
        D_80120653 = 4;
        D_8012065C = (u8 *) D_80139D10[1].unk_04;
        D_80120660 = (u8 *) D_80139D10[1].unk_08;
        D_80120684 = (u8 *) D_80139D10[1].unk_00;
        D_80120664 = D_80139D10[1].unk_0C;
        D_80120666 = 0;
        D_80120668 = 0;
        D_80120656 = 2;
        D_80120693 = 0x10;
        D_80120676 = 0;
        D_8012067A = -0x20;
        D_80120657 = 0;
    }
    func_8004284C();
}

void func_80135E90(void) {
    if (D_800E6280.unk_F5F == 0xA) {
        cd_read_sectors(9, 0x801D0000, 0xBCC8);
        func_801355F0();
    } else {
        cd_read_sectors(9, 0x801D0000, 0xBD95);
        func_80135634();
    }
    func_8004500C(1, 0);
    func_8004284C();
}
