#include "common.h"
#include "ovl/KANGEI.h"

typedef struct {
    void (*f[24])();
} FnTbl24; /* size 0x60 */
extern FnTbl24 D_80139F4C;

typedef struct {
    void (*f[40])();
} FnTbl40; /* size 0xA0 */
extern FnTbl40 D_80139E70;

void func_80135F10(void) {
    D_80139DB0 = 0x801B4400;
    D_80139DB4 = 0x801B6400;
    D_80139DB8 = 0x801BA400;
    D_80139DBC = 0x801BE400;
    D_80139DC0 = 0x801C2400;
    D_80139DC4 = 0x801C6400;
    D_80139DC8 = 0x801CA400;
}

void func_80135F84(void) {
    D_80139DCC = 0x801EAFC4;
    D_80139DD0 = 0x801EB144;
    D_80139DD4 = 0x801EAFF0;
    D_80139DD8 = 0x801EB14C;
    D_80139DDC = 0x801EB030;
    D_80139DE0 = 0x801EB174;
    D_80139DE4 = *(s16 *)0x801EB17C;
    D_80139DE8 = *(s16 *)0x801EB180;
    D_80139DEC = 0x801B4400;
}

void func_80136018(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl40 tbl;

    tbl = D_80139E70;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80136094(void) {
    s16 i;

    func_8006509C();
    for (i = 0; i < 0xF; i++) {
        icon_can_use_set(i, 0);
    }
    func_80065B0C(1);
    parameter_show_init();
    parameter_show_init();
    D_800B593C = 0;
    D_80139AC0 = 0;
    func_8004284C();
}

void func_80136108(void) {
    if (D_80139AE0 == 0) {
        D_80139AE4 = 2;
    } else {
        D_80139AE4 = 0;
    }
    func_80133EF8();
}

void func_8013614C(void) {
    if ((D_80139DF4[(u8) D_80139DF0] == 0) && (D_800E6280.unk_75D != 0xFF) && D_800E6280.unk_1BC[D_800E6280.unk_75D].unk_0C.f.b1) {
        D_800E6280.unk_1BC[D_800E6280.unk_75D].unk_02 -= 0xA;
    } else if (D_80139DF4[(u8) D_80139DF0] == 1) {
        D_800E6280.unk_11C.unk_02 += 0xA;
    }
    func_80084D3C();
    func_8004284C();
}

void func_801361F8(void) {
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x2E);
    func_80044750(0x203);
    func_8004284C();
}

void func_80136238(void) {
    func_80133EF8();
    if (D_800E6280.unk_1104.w++ == 0 && D_80139AE0 == 3) {
        if (D_80139DF4[(u8)D_80139DF0] == 0 && D_80139E0C == 0) {
            func_80044750(0x503);
        }
    }
}

void func_801362BC(void) {
    func_80044750(0x500);
    D_80139DF0 = D_80122CDC;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80135F10", func_801362F0);

void func_80136494(void) {
    load_palette(D_80139DB0, 0x11, 1, 2, 0);
    func_80084E90(D_80139DB4, D_80139DB8, D_80139DBC, D_80139DC0, D_80139DC4, D_80139DC8);
    func_800850D4(0, 0, 0, 0);
    func_8004284C();
}

void func_80136520(void) {
    if (D_800E6280.unk_1104.w++ == 0) {
        if (D_80139DF4[(u8)D_80139DF0] == 0) {
            func_80044750(0x204);
        } else {
            func_80044750(0x205);
        }
    }
    if (D_80139DF4[(u8)D_80139DF0] == 1) {
        func_8007E81C();
        return;
    }
    func_80085368();
}

void func_801365C4(void) {
    if (D_80139DF4[(u8)D_80139DF0] != 0) {
        func_8007E934();
        return;
    }
    if (D_80139E0C != 0) {
        func_800853FC();
        return;
    }
    func_80136630();
}

void func_80136630(void) {
    s16 i;

    if (D_800E6280.unk_1104.w == 0) {
        D_80139E04 = 0;
        D_80139E08 = 0;
        D_80139E00 = 0;
        D_800E6280.unk_1104.w = 1;
    } else if (D_800E6280.unk_1104.w == 1) {
        D_80139E00 += 0x10;
        D_80139E04 += 0x10;
        D_80139E08 += 0x10;
        if (D_80139E00 >= 0xF0) {
            D_800E6280.unk_1104.w = 2;
            D_80139E04 = 0xFF;
            D_80139E08 = 0xFF;
            D_80139E00 = 0xFF;
            for (i = 0x3A; i < 0x40; i++) {
                func_80084C98(i, 0);
            }
            D_800B593C = 0;
        }
    } else {
        D_80139E00 -= 8;
        D_80139E04 -= 8;
        D_80139E08 -= 8;
        if (D_80139E00 < 9) {
            D_80139E04 = 0;
            D_80139E08 = 0;
            D_80139E00 = 0;
            func_8004E9F4(1);
            func_80041584();
            func_8004284C();
        }
    }
    func_80136810(-0xA0, -0x78, 0x140, 0xA0, D_80139E00, D_80139E04, D_80139E08, 9, 1);
    dtd_on_tpage(0, 0, 9, 1, 0);
}

void func_80136810(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, s32 arg8) {
    s32 unused; /* FAKE: unused local, only its 4-byte frame slot above p is observed (T-6030) */
    u8 *p;

    p = func_800490F0(0x10, D_8011ECA0);
    func_8009F0F4(p);
    func_8009EF84(p, arg8);
    p[4] = arg4;
    p[5] = arg5;
    p[6] = arg6;
    *(s16 *) (p + 8) = D_800E6280.unk_014[D_8011ECA0] + arg0;
    *(s16 *) (p + 0xA) = D_800E6280.unk_018[D_8011ECA0] + arg1;
    *(s16 *) (p + 0xC) = arg2;
    *(s16 *) (p + 0xE) = arg3;
    func_8009EED0(&D_800E8CA0[(D_8011ECA0 << 0xA) + (arg7 * 4)], p);
}

void func_80136900(void) {
    D_80139DFC += 1;
    normal_date_two_select();
}

void func_80136930(void) {
    if (D_80122CDC != 0) {
        D_800E6280.unk_110A += 0x13;
    }
    func_8004284C();
}

/* FAKE: shared constant and byte selector preserve compare operand order. T-10850 */
void func_80136970(void) {
    u8 t;
    s32 r;
    u32 one;
    u8 coin;

    one = 1;
    func_800AE120(D_800E6280.unk_10F8);
    r = func_800AE0D0();
    D_80139E0C = r % 2;
    t = D_80139DF4[(u8)D_80139DF0];
    D_80139ADC = (t == 2) * 6 + (one == t) * 4 + 4;
    coin = D_80139E0C;
    if (coin == one && t == 0) {
        D_80139ADC += 2;
    }
    func_8004284C();
}

void func_80136A24(void) {
    D_80139ADC = 0xC;
    D_80139AE0 = (D_80139DFC >= 0x97U) + (D_80139DFC >= 0x12DU);
    func_8004284C();
}

void func_80136A70(void) {
    func_800AE120(D_800E6280.unk_10F8);
    D_800E6280.unk_F5F = 0xE;
    D_80139DFC = 0;
    D_80139ADC = 1;
    D_80139AE0 = 0;
    func_800AE120(D_800E6280.unk_10F8);
    D_80139DF4[0] = rand() % 2 + 1;
    D_80139DF4[1] = rand() % 2 + 1;
    D_80139DF4[2] = rand() % 2 + 1;
    if (rand() % 2) {
        D_80139DF4[0] = 0;
    }
    if (rand() % 2) {
        D_80139DF4[1] = 0;
    }
    if (rand() % 2) {
        D_80139DF4[2] = 0;
    }
    func_8004284C();
}

INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80135F10.rodata", D_801398E0);

void func_80136BB8(void) {
    bg_read_sub2(0x42BC);
    func_8004284C();
    func_800AE0F0(D_800CA19C, &D_801398E0);
}

void func_80136BF4(void) {
    if (D_80139DF4[(u8)D_80139DF0] == 2) {
        func_8007ED84(0x42B3);
    } else if (D_80139DF4[(u8)D_80139DF0] == 1) {
        func_8007ED84(0x477D);
    } else {
        switch (D_80139DF8) {
        case 0:
            func_8007ED84(0x4787);
            break;
        case 1:
            func_8007ED84(0x478E);
            break;
        case 2:
            func_8007ED84(0x4797);
            break;
        case 3:
            func_8007ED84(0x47A9);
            break;
        default:
            func_8007ED84(0x47B2);
            break;
        }
    }
    func_8004284C();
}

void func_80136CD8(void) {
    place_init();
    LoadSquare(0x320, 0x100, 4, 0xC, &D_80139E10);
    LoadSquare(0x324, 0x100, 4, 0xC, &D_80139E10);
    LoadSquare(0x328, 0x100, 4, 0xC, &D_80139E10);
}

void func_80136D58(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl24 tbl;

    tbl = D_80139F4C;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    if (D_800E6280.unk_110A >= 9U) {
        if (D_800E6280.unk_110A < 0x14U) {
            func_801370A0();
        }
    }
}

INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80135F10.rodata", D_801398E8);

INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80135F10.rodata", D_801398EC);

void func_80136DF4(void) {
    dialogue_render(-0x64, 0x30, 2, D_80139AD0->unk_34->unk_08, 0, &D_801398E8, &D_801398EC, 0);
    func_8004284C();
}

INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80135F10.rodata", D_801398F0);

INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80135F10.rodata", D_801398F4);

void func_80136E54(void) {
    dialogue_render(-0x56, 0x40, 1, D_80139AD0->unk_34->unk_0C, 0, &D_801398F0, &D_801398F4, 0);
    func_8004284C();
}

void func_80136EB4(void) {
    dialogue_render(-0x2C, 0x50, 0, ((s32 *)D_80139AD0->unk_34)[4], 0, "", "", 0);
    D_80139AE0 = 5;
    func_8004284C();
}

void func_80136F1C(void) {
    D_80139AE4 = 2;
    func_8004284C();
}

void func_80136F44(void) {
    D_80139AE4 = 1;
    func_8004284C();
}

void func_80136F6C(void) {
    D_80139AE4 = 0;
    func_8004284C();
}

void func_80136F90(void) {
    func_80044750(0xBF);
    func_80044890(1, 0xBF98, 0xBF79, 0xC982, 0xC935, 0xC91B);
    func_8004284C();
}

void func_80136FDC(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0x203);
        func_8004284C();
    }
    if (D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_80137064(void) {
    cd_read_sectors(0x6E, 0x801B4400, 0xBBA5);
    func_80135F84();
    func_8004284C();
}

/* Port of the MASTER clone func_80137BF4 (same opcodes, T-18400). */
void func_801370A0(void) {
    KangeiPoint2 *temp_s0_2;
    s16 *temp_s1;
    /* FAKE: wide X local plus a narrow third argument preserves the
     * original sign-extension sequence (as MASTER func_80137BF4, T-10630). */
    s32 temp_a0;
    s16 temp_a1;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 var_s2;
    s32 temp_hi;
    KangeiSprite44 *temp_s0;

    if ((((KangeiSprite44 *)D_80120650)->unk_16 == 0) && (((KangeiSprite44 *)D_80120650)->unk_18 == 7) && (((KangeiSprite44 *)D_80120650)->unk_08 == 2)) {
        ((KangeiSprite44 *)D_80120650)->unk_16 = 1;
        ((KangeiSprite44 *)D_80120650)->unk_18 = 0;
        ((KangeiSprite44 *)D_80120650)->unk_08 = 0;
        ((KangeiSprite44 *)D_80120650)->unk_02 = 1;
    }
    var_s2 = 0x61;
    if ((((KangeiSprite44 *)D_80120650)->unk_16 == 2) && (((KangeiSprite44 *)D_80120650)->unk_18 == 1) && (((KangeiSprite44 *)D_80120650)->unk_08 == 4)) {
        ((KangeiSprite44 *)D_80120650)->unk_16 = 3;
        ((KangeiSprite44 *)D_80120650)->unk_18 = 0;
        ((KangeiSprite44 *)D_80120650)->unk_08 = 0;
        ((KangeiSprite44 *)D_80120650)->unk_02 = 1;
    }
    do {
        temp_s0 = &((KangeiSprite44 *)D_8011ECD0)[var_s2];
        temp_v0 = temp_s0->unk_18;
        if ((temp_v0 < 2) || (temp_v0 >= 8)) {
            temp_s0->unk_28.w = temp_s0->unk_28.w - ((((KangeiSprite44 *)D_80120650)->unk_16 == 3) * 0x50000) - 0x50000;
        } else if ((temp_v0 == 4) || (temp_v0 == 7)) {
            temp_s0->unk_28.w = temp_s0->unk_28.w - ((((KangeiSprite44 *)D_80120650)->unk_16 == 3) * 0x30000) - 0x30000;
        } else {
            temp_s0->unk_28.w = temp_s0->unk_28.w - ((((KangeiSprite44 *)D_80120650)->unk_16 == 3) * 0x20000) - 0x20000;
        }
        if (temp_s0->unk_28.h[1] < -0x9F) {
            temp_hi = func_800AE0D0() % 320;
            temp_s0->unk_26 = (s16) (temp_hi - 0xA0);
            temp_s0->unk_28.h[1] = 0x3C;
            temp_s0->unk_18 = (s16) (func_800AE0D0() % 10);
        }
        var_s2 += 1;
    } while (var_s2 < 0x74);
    var_s2 = 0;
    if ((u8) ((KangeiSprite44 *)D_80120650)->unk_07 >= 0x78U) {
        do {
            temp_s0_2 = &D_80139F10[var_s2];
            temp_s1 = &D_80139F38[var_s2];
            temp_a0 = temp_s0_2->x;
            temp_v0_2 = *temp_s1;
            temp_s0_2->y -= temp_v0_2 * 4 * ((((KangeiSprite44 *)D_80120650)->unk_16 == 3) + 1);
            temp_a1 = temp_s0_2->y;
            func_80049FF0((s32) temp_a0, (s32) temp_a1, (s16) temp_a0, (s32) (s16) ((temp_v0_2 * 0x14) + temp_a1), 0xFFFFFF, 0xFFFFFF, 0xB);
            if (temp_s0_2->y < -0xAA) {
                temp_s0_2->y = 0x3C;
                temp_s0_2->x = (func_800AE0D0() % 320) - 0xA0;
                *temp_s1 = (func_800AE0D0() % 5) + 1;
            }
            var_s2 += 1;
        } while (var_s2 < 0xA);
    }
}

void func_80137440(void) {
    D_80120666 = 2;
    D_80120668 = 0;
    D_80120658 = 0;
    D_80120652 = 5;
    func_8004284C();
}

void func_80137484(void) {
    s16 i;
    KangeiSprite44 *sprite;

    ((KangeiSprite44 *)D_8011ECD0)[96].unk_03 |= 0x80;
    ((KangeiSprite44 *)D_8011ECD0)[96].unk_07 += 8;
    for (i = 0x61; i < 0x74; i++) {
        sprite = &((KangeiSprite44 *)D_8011ECD0)[i];
        sprite->unk_03 |= 0x80;
        sprite->unk_07 += 8;
    }
    ((KangeiSprite44 *)D_8011ECD0)[96].unk_02 = 5;
    if (((KangeiSprite44 *)D_8011ECD0)[96].unk_07 >= 0x80U) {
        ((KangeiSprite44 *)D_8011ECD0)[96].unk_07 = 0x80;
        ((KangeiSprite44 *)D_8011ECD0)[97].unk_07 = 0x80;
        func_8004284C();
    }
}

/* T-9740: integer definition without an explicit return matches the original. */
s32 func_80137544(void) {
    s16 i;
    s32 mask;
    KangeiSprite44 *sprite;
    ((KangeiSprite44 *)D_8011ECD0)[96].unk_07 -= 8;
    for (i = 0x61; i < 0x74; i++) {
        sprite = &((KangeiSprite44 *)D_8011ECD0)[i];
        sprite->unk_07 -= 8;
    }
    /* The signed mask is shared by the scalar and loop flag clears. */
    mask = ~0x80;
    if (((KangeiSprite44 *)D_8011ECD0)[96].unk_07 < 9U) {
        ((KangeiSprite44 *)D_8011ECD0)[96].unk_07 = 0;
        ((KangeiSprite44 *)D_8011ECD0)[96].unk_03 &= mask;
        for (i = 0x61; i < 0x74; i++) {
            sprite = &((KangeiSprite44 *)D_8011ECD0)[i];
            sprite->unk_07 = 0;
            sprite->unk_03 &= mask;
        }
        D_800B5BD4 = 0xF;
        func_80041584();
        func_8004500C(0, 0x200);
        func_8004284C();
    }
}

typedef struct {
    u32 pad0 : 25;
    u32 grade : 2;
    u32 pad1 : 5;
} KgSlot;

s32 func_80137638(void) {
    if (((KgSlot *) &D_800E6280.unk_66C[(u32) D_800E6280.unk_0F4.h >> 12])->grade < 3) {
        func_80042808();
        return 0;
    }
    D_800B5BD4 = 0xA;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80135F10", func_8013769C);
