#define MAIN_API_OVERRIDE_D_80120656 /* 0x82 must not sign-extend (main_api.h: s8) */
#include "common.h"
#include "ovl/BUNKA_SD.h"

extern u8 D_80120656;

s32 func_80134540(void) {
    if (D_800E6280.unk_110D == 0) {
        cd_read_sectors(0x85, 0x80180000, 0x4BD4);
        D_800E6280.unk_110D += 1;
    } else if ((D_800E6280.unk_110D == 1) && (cd_read_sync_status() & 1)) {
        func_8004284C();
    }
}

/* FAKE: the D_8012069A store shares a source line with the loop head; as1 schedules by line and this gives the original's order of the loop setup (T-7020, T-9380) */
void func_801345BC(void) {
    s32 i;

    if ((D_800E6280.unk_110A >= 3) && (D_800E6280.unk_110A < 7)) {
        if (D_800E6280.unk_F80 & 0x20) {
            func_801346D4();
            D_80120656 = 0x82;
            D_8012069A = 0x82; for (i = 0; i < 2; i++) {
                D_8011ED58[(i + 1) * 0x110 + 0x18BA] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x18FE] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x1942] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x1876] = 0x82;
            }
        } else {
            D_80120656 = 1;
            D_8012069A = 1; for (i = 0; i < 2; i++) {
                D_8011ED58[(i + 1) * 0x110 + 0x18BA] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x18FE] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x1942] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x1876] = 1;
            }
        }
    }
    if ((D_800E6280.unk_110A >= 3) && (D_800E6280.unk_110A < 6) && (D_800E6280.unk_F80 & 0x40)) {
        func_80042940(5);
    }
    func_801346D4();
}

void func_801346D4(void) {
    func_80066C08(2);
    func_80064F48();
    func_80066334();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80134260();
        return;
    case 1:
        func_80134540();
        return;
    case 2:
        func_801347CC();
        return;
    case 3:
        func_80134418();
        return;
    case 4:
        func_80134800();
        return;
    case 5:
        func_80044750(0xC1);
        func_8004E9F4(1);
        func_8004284C();
        return;
    case 6:
        func_80134850();
        return;
    case 7:
        func_80134314();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_801347CC(void) {
    func_8004500C(1, 0x201);
    func_80134938();
    func_8004284C();
}

void func_80134800(void) {
    u32 t;

    func_80134B88();
    func_80134D94();
    t = D_800E6280.unk_1104.u + 1;
    D_800E6280.unk_1104.u = t;
    if (t >= 0x818) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80134540", func_80134850);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80134540", func_80134938);

void func_80134B88(void) {
    /* u32 selector: sltiu compares and the selector stays in $v0 (T-8020) */
    switch ((u32)D_800E6280.unk_1104.w) {                     /* irregular */
    case 0x4F2:
        func_80044750(0x509);
        break;
    case 0x542:
        func_80044750(0x509);
        break;
    case 0x592:
        func_80044750(0x509);
        break;
    case 0x606:
        func_80044750(0x500);
        break;
    case 0xF0:
        func_80044750(0x50A);
        break;
    case 0x150:
        func_80044750(0x50A);
        break;
    case 0x1B0:
        func_80044750(0x50A);
        break;
    case 0x210:
        func_80044750(0x50A);
        break;
    case 0x270:
        func_80044750(0x50A);
        break;
    case 0x2D0:
        func_80044750(0x50A);
        break;
    case 0x330:
        func_80044750(0x50A);
        break;
    case 0x390:
        func_80044750(0x50A);
        break;
    case 0x3F0:
        func_80044750(0x50A);
        break;
    case 0x450:
        func_80044750(0x50A);
        break;
    case 0x610:
        func_80044750(0x502);
        break;
    case 0x4B0:
        if (D_80122EAC == 3) {
            func_80044750(0x50A);
        }
        break;
    case 0x520:
        if (D_80122EAC == 3) {
            func_80044750(0x50A);
        }
        break;
    }
}

/* bulk_m2c declarations (T-11190) */
extern u32 D_800E7384;
extern u8 D_800E738C;
extern u8 D_800E738D;
/* end bulk_m2c declarations */
void func_80134D94(void) {
    s32 sp3C;
    s16 var_t7;
    s32 temp_v0;
    u32 temp_lo;
    u32 temp_t0;
    u32 var_a0;
    u8 var_a0_2;
    u8 var_a1;

    temp_t0 = D_800E7384;
    if (temp_t0 == 0) {
        D_8012071E = 5;
    }
    if (temp_t0 == 0xFE) {
        D_80120697 = 0x80;
        D_80120696 = 5;
        D_8012069B = 0x80;
    }
    if (!(D_80120696 & 1)) {
        D_80120697 = 0;
    }
    if (temp_t0 == 0x29C) {
        D_801206DB = 0x80;
        D_801206DA = 5;
        D_801206DF = 0x80;
    }
    if ((temp_t0 >= 0xFFU) && (temp_t0 < 0x5A3U)) {
        var_a1 = D_800E738D;
        D_8012071E = 0x40;
        var_a0 = var_a1 / 14U;
        if (var_a0 >= 2U) {
            var_a1 = 0;
            var_a0 = 0;
        }
        switch (D_80122EAC) {
        case 0:
            D_80120734 = var_a0 + 0x5D;
            break;
        case 1:
            D_80120734 = var_a0 + 0x72;
            break;
        case 2:
            D_80120734 = var_a0 + 0x87;
            break;
        default:
        case 3:
            if (temp_t0 < 0x4C2U) {
                D_80120734 = var_a0 + 0x9D;
            } else {
                D_80120734 = var_a0 + 0x9F;
            }
            break;
        }
        D_800E738D = var_a1 + 1;
    }
    if ((temp_t0 == 0x4C2) && (D_80122EAC != 3)) {
        D_8012071F = 0;
        D_80120763 = 0x80;
        D_80120762 = 5;
        D_80120767 = 0x80;
    }
    if ((temp_t0 == 0x58F) && (D_80122EAC == 3)) {
        D_8012071F = 0;
        D_80120763 = 0x80;
        D_80120762 = 5;
        D_80120767 = 0x80;
    }
    if ((temp_t0 == 0x606) || (temp_t0 == 0x609) || (temp_t0 == 0x60C)) {
        func_80049A40(-0xA0, -0x78, 0x140, 0xA0, 2, 0xFFFFFF, 0x81);
        dtd_on_tpage(0, 0, 2, 0, 0);
    }
    if (temp_t0 == 0x606) {
        D_801206F0 = 0x48;
        D_801206E0 = 0;
    }
    if ((temp_t0 >= 0x60FU) && (temp_t0 < 0x68EU)) {
        temp_v0 = (0x68E - temp_t0) * 0x10201;
        sp3C = temp_v0;
        func_80049A40(-0x130, -0x78, 0x140, 0xA0, 2, temp_v0, 0x86);
        func_80049A40(0x10, -0x78, 0x140, 0xA0, 2, temp_v0, 0x85);
        dtd_on_tpage(0, 0, 2, 1, 0);
    }
    if (temp_t0 == 0x67E) {
        D_80120657 = 0x80;
        D_801206DF = 0x80;
        D_80120767 = 0x80;
    }
    if ((temp_t0 >= 0x723U) && (D_80122EAC == 3)) {
        var_a0_2 = D_800E738C;
        D_80120762 = 0x40;
        temp_lo = var_a0_2 / 14U;
        var_t7 = temp_lo + 0xA2;
        if (temp_lo >= 2U) {
            var_a0_2 = 0;
            var_t7 = 0xA2;
        }
        D_80120778 = var_t7;
        D_800E738C = var_a0_2 + 1;
    }
}
