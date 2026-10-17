#include "common.h"
#include "ovl/EN_NICHI.h"

typedef struct EnWork1C20 {
    /* 0x0000 */ u8 pad0000[0x1A2E];
    /* 0x1A2E */ s16 unk_1A2E;
    /* 0x1A30 */ u8 pad1A30[2];
    /* 0x1A32 */ s16 unk_1A32;
    /* 0x1A34 */ u8 pad1A34[0x1C8];
    /* 0x1BFC */ s16 unk_1BFC;
    /* 0x1BFE */ u8 pad1BFE[0x1E];
    /* 0x1C1C */ s32 unk_1C1C;
} EnWork1C20; /* size 0x1C20 */
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139C30);
u8 D_80139C34[8] = { 0 };
u8 D_80139C3C[8] = { 0 };
u8 D_80139C44[8] = { 0 };
s16 D_80139C4C[6] = { 0 };
u16 D_80139C58 = 0;
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139C5C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139C68);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139C94);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139CC0);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139CEC);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139D04);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139D1C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139DA0);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139E20);

typedef struct {
    void (*f[3])();
} FnTbl3; /* size 0xC */

extern FnTbl3 D_80139C5C;

void func_80136B10(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    FnTbl3 tbl;

    tbl = D_80139C5C;
    tbl.f[D_800E6280.unk_1109]();
}

void func_80136B74(void) {
    s32 i;

    switch (D_800E6280.unk_110A) {
    case 0:
        func_80085E30(1, 0);
        func_8006BD6C(0);
        hizuke_disp_switch(0);
        message_disp_switch(0);
        func_800AE120(D_800E6280.unk_10F4);
        func_80044750(0xCF);
        func_8004284C();
        return;
    case 1:
        cd_read_sectors(0x6C, 0x801B0000, 0x7D0B);
        func_8004284C();
        return;
    case 2:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_03A = 0;
            func_80132040();
            for (i = 0; i != 2; i++) {
                load_palette(D_801391F4, (i + 0x10) & 0xFF, 1, 1, i);
            }
            func_8004E44C(0, (u32 *) D_801391FC, D_801391E0);
            srn_init(0, (u32 *) D_801391F8, 0);
            srn_vram_set(0, 5, 0x580, 0xF0);
            Fade_In_Init();
            func_8004284C();
        }
        return;
    case 3:
        func_80044750(0x203);
        func_8004284C();
        return;
    case 4:
        Fade_In_Main();
        if (D_800E6280.unk_F88 & 0x60) {
            func_80044750(0x74);
            Fade_Out_Init();
            func_8004284C();
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        return;
    case 5:
        if (Fade_Out_Main() != 0) {
            for (i = 0; i < 6; i++) {
                D_800E6280.unk_1228[i + 5] = -1;
                D_800E6280.unk_1228[i + 37] = -1;
            }
            func_80042808();
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        return;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80136B10", func_80136DEC);

/* bulk_m2c declarations (T-11190) */
extern s16 D_80139C4E;
extern s16 D_80139C50;
extern s16 D_80139C52;
extern s16 D_80139C54;
extern s32 D_800E7208;
extern s32 D_8011EE24;
extern s32 D_8011F2A8;
extern u8 D_800E69A0;
extern u8 D_800E69A1;
extern u8 D_800E69A2;
extern u8 D_800E71DF;
extern u8 D_800E738A;
extern u8 D_800E738D;
extern u8 D_80139C30;
extern u8 D_80139C35;
extern u8 D_80139C36;
extern u8 D_80139C37;
extern u8 D_80139C38;
void func_80137C58(void);
void func_80138CCC(void);
/* end bulk_m2c declarations */
void func_8013735C(void) {
    s16 *temp_a0;
    s16 temp_a1;
    s16 temp_a1_2;
    s16 temp_a1_3;
    s16 temp_v0;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_v0_3;
    s32 var_at;
    s32 var_v1;
    u8 *var_v0_2;
    u8 temp_t4;
    u8 *temp_v0_2;
    u8 *temp_v1;
    u8 *temp_v1_2;
    u8 *var_v0;

    switch (D_800E738A) {
    case 0:
        func_8006BA40();
        D_80120852 = D_8011ECF6;
        D_80120856 = D_8011ECFA;
        if ((D_800E7208 & 0x20) && (D_80139C30 != 0)) {
            temp_t4 = (D_80139C30 - 1) & 0xFF;
            D_80139C34[temp_t4] = 1;
            D_80139C3C[temp_t4] = ((u16) D_80139C58 >> 4) + 1;
            temp_v1 = D_8011ECD0 + (temp_t4 * 0x44);
            *(s16 *)((unsigned char *)temp_v1 + (6702)) = (s16) D_80120852;
            *(s8 *)((unsigned char *)temp_v1 + (6665)) = 4;
            *(s16 *)((unsigned char *)temp_v1 + (6706)) = (s16) D_80120856;
            D_80139C30 = temp_t4;
            func_80044750(0x50C);
        }
        func_80137C58();
        func_80138CCC();
        if ((D_80139C30 == 0) && (*D_80139C34 == 0) && (D_80139C35 == 0) && (D_80139C36 == 0) && (D_80139C37 == 0) && (D_80139C38 == 0)) {
            func_8004284C();
        }
        break;
    case 1:
        D_80120BE7 |= 0x80;
        var_v1 = 0;
        if (*D_80139C4C != 0xFF) {
            D_801206F0 = *D_80139C4C;
            var_v1 = 1;
        }
        D_801206D9 = 1;
        temp_a0 = &D_80139C4C[1];
        temp_v0 = (*(s32 *)temp_a0);
        if (temp_v0 != 0xFF) {
            temp_lo = var_v1 * 0x44;
            var_v1 += 1;
            *(s16 *)((unsigned char *)(&D_8011ECD0[temp_lo]) + (6688)) = temp_v0;
        }
        temp_a1 = *(s16 *)((unsigned char *)temp_a0 + (2));
        temp_v0_2 = D_8011ECD0 + (1 * 0x44);
        *(s8 *)((unsigned char *)temp_v0_2 + (6665)) = 1;
        if (temp_a1 != 0xFF) {
            temp_lo_2 = var_v1 * 0x44;
            var_v1 += 1;
            *(s16 *)((unsigned char *)(&D_8011ECD0[temp_lo_2]) + (6688)) = temp_a1;
        }
        temp_a1_2 = *(s16 *)((unsigned char *)temp_a0 + (4));
        *(s8 *)((unsigned char *)temp_v0_2 + (6733)) = 1;
        if (temp_a1_2 != 0xFF) {
            temp_lo_3 = var_v1 * 0x44;
            var_v1 += 1;
            *(s16 *)((unsigned char *)(&D_8011ECD0[temp_lo_3]) + (6688)) = temp_a1_2;
        }
        temp_a1_3 = *(s16 *)((unsigned char *)temp_a0 + (6));
        *(s8 *)((unsigned char *)temp_v0_2 + (6801)) = 1;
        if (temp_a1_3 != 0xFF) {
            temp_lo_4 = var_v1 * 0x44;
            var_v1 += 1;
            *(s16 *)((unsigned char *)(&D_8011ECD0[temp_lo_4]) + (6688)) = temp_a1_3;
        }
        *(s8 *)((unsigned char *)temp_v0_2 + (6869)) = 1;
        if (var_v1 < 5) {
            var_v0 = (var_v1 * 0x44) + D_8011ECD0;
            do {
                var_v0 += 0x44;
                *(s16 *)((unsigned char *)var_v0 + (6620)) = 0xFF;
            } while ((u32) var_v0 < (u32) &D_8011EE24);
        }
        Vblnk_Timer_Init(temp_a0, temp_a1_3, 1, 0x44);
        D_80139C30 = 0;
        func_8004284C();
        break;
    case 2:
        if (Vblnk_Timer() >= 0x2E) {
            if ((*(s16 *)((unsigned char *)(D_8011ECD0 + (D_80139C30 * 0x44)) + (6688)) == 0xFF) || ((u8) D_80139C30 >= 5U)) {
                if ((*D_80139C4C == 0x1F) && (D_80139C4E == 0x1F) && (D_80139C50 == 0x1F) && (D_80139C52 == 0x1F) && (D_80139C54 == 0x1F)) {
                    func_80042940(5);
                } else if ((*D_80139C4C == 0x20) && (D_80139C4E == 0x20) && (D_80139C50 == 0x20) && (D_80139C52 == 0x20) && (D_80139C54 == 0x20)) {
                    func_80042940(6);
                } else {
                    Vblnk_Timer_Init((s16 *) *D_80139C4C, D_80139C4E, D_80139C50, D_80139C52);
                    func_8004284C();
                }
                func_80044750(0x50B);
            } else {
                func_80044750(0x50A);
                temp_v1_2 = D_8011ECD0 + (D_80139C30 * 0x44);
                *(s8 *)((unsigned char *)temp_v1_2 + (6667)) = 0x84;
                *(s16 *)((unsigned char *)temp_v1_2 + (6702)) = (s16) ((D_80139C30 << 6) - 0x80);
                *(s16 *)((unsigned char *)temp_v1_2 + (6706)) = 0x38;
                D_80139C30 += 1;
                Vblnk_Timer_Init();
            }
        }
        break;
    case 3:
        if (Vblnk_Timer() >= 0x79) {
            if ((u8) D_80120657 < 4U) {
                func_80048EB8(0);
                func_8004284C();
            } else {
                D_80120657 -= 4;
            }
            var_v0_2 = D_8011ED58;
            D_8012069B = D_80120657;
            do {
                *(u8 *)((unsigned char *)var_v0_2 + (6535)) = (u8) D_80120657;
                *(u8 *)((unsigned char *)var_v0_2 + (6603)) = (u8) D_80120657;
                *(u8 *)((unsigned char *)var_v0_2 + (6671)) = (u8) D_80120657;
                var_v0_2 += 0x110;
                *(u8 *)((unsigned char *)var_v0_2 + (6467)) = (u8) D_80120657;
            } while (var_v0_2 != &D_8011F2A8);
        }
        break;
    case 4:
        if (((*D_80139C4C < 0x10) && (D_80139C4E < 0x10) && (D_80139C50 < 0x10) && (D_80139C52 < 0x10) && (D_80139C54 < 0x10)) || (*D_80139C4C != 0xFF) || (D_80139C4E != 0xFF)) {

        }
        if ((*D_80139C4C == 0x10) && (D_80139C4E == 0x10) && (D_80139C50 == 0x10) && (D_80139C52 == 0x10) && (D_80139C54 == 0x10) && (D_800E71DF == 2)) {
            D_80122E9C = 3;
        } else {
            temp_v0_3 = D_800E71DF + 0x14;
            var_at = *D_80139C4C < 0x21;
            if ((temp_v0_3 == *D_80139C4C) && (var_at = *D_80139C4C < 0x21, (temp_v0_3 == D_80139C4E)) && (var_at = *D_80139C4C < 0x21, (temp_v0_3 == D_80139C50)) && (var_at = *D_80139C4C < 0x21, (temp_v0_3 == D_80139C52))) {
                if ((temp_v0_3 == D_80139C54) && (D_800E71DF != 2)) {
                    D_80122E9C = 3;
                } else {
                    var_at = *D_80139C4C < 0x21;
                    goto block_73;
                }
            } else {
block_73:
                if ((var_at != 0) && (D_80139C4E < 0x21) && (D_80139C50 < 0x21) && (D_80139C52 < 0x21) && (D_80139C54 < 0x21)) {
                    D_80122E9C = 1;
                } else {
                    D_80122E9C = 4;
                }
            }
        }
        main_state_set((s32) D_800E69A0);
        func_80042908((s32) D_800E69A1);
        func_80042940((s32) D_800E69A2);
        break;
    case 5:
        switch (D_800E738D) {
        case 0:
            func_80046290(0x1A0000B1, 0x170000B7, 0xE);
            func_80044750(0x300);
            D_800E738D += 1;
            break;
        case 1:
            if (func_800460EC() == 4) {
                D_800E738D += 1;
            }
            break;
        case 2:
            if (func_800460EC() == 2) {
                func_80044750(0x74);
                D_800E738D += 1;
            }
            break;
        case 3:
            Vblnk_Timer_Init();
            D_800E738D += 1;
            break;
        case 4:
            if (Vblnk_Timer() >= 0x79) {
                Vblnk_Timer_Init();
                func_80042940(3);
            }
            break;
        default:
            func_80046500();
            break;
        }
        break;
    case 6:
        switch (D_800E738D) {
        case 0:
            func_80046290(0x03000000, 0x01000007, 0xE);
            func_80044750(0x300);
            D_800E738D += 1;
            break;
        case 1:
            if (func_800460EC() == 4) {
                D_800E738D += 1;
            }
            break;
        case 2:
            if (func_800460EC() == 2) {
                func_80044750(0x74);
                D_800E738D += 1;
            }
            break;
        case 3:
            Vblnk_Timer_Init();
            D_800E738D += 1;
            break;
        case 4:
            if (Vblnk_Timer() >= 0x79) {
                Vblnk_Timer_Init();
                func_80042940(3);
            }
            break;
        default:
            func_80046500();
            break;
        }
        break;
    default:
        func_80046500();
        break;
    }
}

/* FAKE: the u8 locals `one` and `two` put the hoisted constants first in both bne compares (T-9410 idiom), T-11300 */
void func_80137C58(void) {
    s32 i;
    u8 *p;
    u8 *r;
    u8 v;
    u8 one;
    u8 two;

    one = 1;
    two = 2;
    D_80139C58 = D_80139C58 + 4;
    if (D_80139C58 >= 0x71) {
        D_80139C58 = 0;
    }
    func_80137E2C();
    for (i = 0, p = D_80139C34; i < 5; i++, p++) {
        v = *p;
        if (one == v) {
            r = D_8011ECD0 + i * 0x44;
            *(s16 *)(r + 0x1A22) -= D_80139C3C[i] * 8;
            *(s16 *)(r + 0x1A24) -= D_80139C3C[i] * 8;
            if (*(s16 *)(r + 0x1A22) < 0x401) {
                if (func_80137FEC(i) != 0) {
                    *(s16 *)(r + 0x1A20) = 0x27;
                    if ((D_80139C4C[i] < 0x10) || (D_80139C4C[i] >= 0x14)) {
                        func_80044750(0x508);
                    } else {
                        func_80044750(0x509);
                    }
                } else {
                    func_80044750(0x507);
                    *(s16 *)(r + 0x1A20) = 0x26;
                }
                *(s16 *)(r + 0x1A22) = 0x1000;
                *(s16 *)(r + 0x1A24) = 0x1000;
                *p = 2;
                v = 2;
            }
        }
        if (two == v) {
            D_80139C44[i] += 1;
            if (D_80139C44[i] >= 0x3D) {
                ((DisplayActor44 *)D_8011ECD0)[i + 98].unk_03 = 0;
                *p = 0;
            }
        }
    }
}

/* FAKE: the empty do-while after the load of D_80139C58 puts the load and its uses in the original's registers (a2, v0); found with the permuter, T-9380 */
void func_80137E2C(void) {
    u8 *p;
    u16 w;
    s32 a;

    p = func_800490F0(0x24, D_8011ECA0);
    func_8009F054(p);
    func_8009EF84(p, 1);
    func_8009EFB0(p, 0);
    w = D_80139C58;
    do {
    } while (0);
    a = w * 2 + 0x1F;
    p[6] = 0xFF;
    p[4] = 0x1F;
    p[5] = 0x1F;
    p[0xC] = a;
    p[0xE] = 0xFF;
    p[0xD] = 0x1F;
    p[0x16] = 0xFF;
    p[0x14] = 0x1F;
    p[0x15] = 0x1F;
    p[0x1C] = a;
    p[0x1E] = 0xFF;
    p[0x1D] = 0x1F;
    *(s16 *)(p + 8) = D_800E6280.unk_014[D_8011ECA0] + 0x20;
    *(s16 *)(p + 0xA) = D_800E6280.unk_018[D_8011ECA0] + 0x50;
    *(s16 *)(p + 0x10) = D_800E6280.unk_014[D_8011ECA0] + w + 0x20;
    *(s16 *)(p + 0x12) = D_800E6280.unk_018[D_8011ECA0] + 0x50;
    *(s16 *)(p + 0x18) = D_800E6280.unk_014[D_8011ECA0] + 0x20;
    *(s16 *)(p + 0x1A) = D_800E6280.unk_018[D_8011ECA0] + 0x60;
    *(s16 *)(p + 0x20) = D_800E6280.unk_014[D_8011ECA0] + w + 0x20;
    *(s16 *)(p + 0x22) = D_800E6280.unk_018[D_8011ECA0] + 0x60;
    func_8009EED0((D_8011ECA0 << 10) + 0xC + D_800E8CA0, p, w);
}

/* FAKE: local reuse and the callback comma expression retain the table slots
 * and post-call decrement scheduling. T-9720. */
s32 func_80137FEC(s32 arg0) {
    s32 i;
    s32 value;
    s32 outside;
    EnHalf12 limits;
    EnFn33 calls;
    EnWork1C20 *record;
    s16 limit;

    limits = D_80139D04;
    calls = D_80139D1C;
    outside = 0;
    record = (EnWork1C20 *)((u8 *)D_8011ECD0 + arg0 * 0x44);
    i = 0;
    value = -0x78;
    do {
        if (record->unk_1A32 < value && ((limit = limits.value[i], record->unk_1A2E < -limit) || limit < D_801206FE)) {
            outside = 1;
        }
        i++;
        value += 8;
    } while (i < 12);
    i = 11;
    if (outside != 0) {
        D_80139C4C[arg0] = 0xFF;
        return 0;
    }
    record = (EnWork1C20 *)((u8 *)D_8011ECD0 + 11 * 0x44);
loop:
    if ((outside = calls.f[record->unk_1BFC](arg0, i), i--, outside != 0)) {
        value = record->unk_1C1C;
        if (!(value & 0x40000000)) {
            record->unk_1C1C = value | 0x40000000;
            D_80139C4C[arg0] = record->unk_1BFC;
            return 1;
        }
    }
    record = (EnWork1C20 *)((u8 *)record - 0x44);
    if (i < 0) {
        D_80139C4C[arg0] = 0xFF;
        return 0;
    }
    goto loop;
}

s32 func_801381C8(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xh;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xh = x + 0x10;
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (xh >= px && px >= x - 0x10) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x8 >= py && py >= y - 0x8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xh >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y - 0x8 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_801382B4(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (x >= px && px >= x - 0x10) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x8 >= py && py >= y - 0x8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_801383A0(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x8 >= py && py >= y - 0x8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138434(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xs;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xs = x + 8;
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= xs) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xs >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 8 >= py && py >= y - 8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138524(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xs;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xs = x + 8;
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= xs) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xs >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138614(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xs;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xs = x + 8;
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x - 0x10) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 8 >= py && py >= y - 8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xs >= px && px >= x - 8) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138708(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xs;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xs = x - 8;
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= xs) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xs >= px && px >= x - 0x10) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 8 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_801387FC(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138890(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xa;
    s32 xb;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xa = x + 8;
    xb = x + 0x10;
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (xa >= px && px >= x - 8) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xb >= px && px >= xa) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 8 >= py && py >= y - 8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138980(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x - 0x10) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 8 >= py && py >= y - 8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138A18(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x - 0x8) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x8 >= py && py >= y - 0x8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138AB0(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xh;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xh = x + 0x10;
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (xh >= px && px >= x - 0x8) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xh >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y - 0x8 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138B9C(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x20 >= px && px >= x - 0x20) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x20 >= py && py >= y - 0x20) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138C34(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x20 >= px && px >= x - 0x20) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 16 >= py && py >= y - 16) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

/* Moves the three rows of four display records (105.., 109.., 113..) in 16.16 steps; a record that
 * leaves the screen re-enters behind its neighbour with a frame from tbl, whose entries 32..47 are
 * the values of girls picked at random from list (girls with flag bit 1, and 11, 12 when set).
 * The neighbour is read before the new frame is stored, as in the original. T-20920 */
void func_80138CCC(void) {
    s32 i;
    s32 r1;
    s32 r2;
    s32 r3;
    EnHalf64 tbl;
    EnHalf13 girls;
    u8 n;
    u8 list[15];

    tbl = D_80139DA0;
    girls = D_80139E20;
    n = 0;
    for (i = 0; i < 11; i++) {
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1 == 1) {
            list[n] = i;
            n++;
        }
    }
    if (D_800E7D10.flags[11] != 0 || D_800E6280.unk_1BC[11].unk_0C.f.f9 != 0) {
        list[n] = 11;
        n++;
    }
    if (D_800E7D10.flags[12] != 0) {
        list[n] = 12;
        n++;
    }
    for (i = 32; i < 48; i++) {
        tbl.value[i] = girls.value[list[func_800AE0D0() % n]];
    }
    for (i = 0; i < 4; i++) {
        ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_24.h[1]++;
        if (((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_24.h[1] >= 0xC1) {
            r1 = func_800AE0D0() & 0x3F;
            ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_24.h[1] = ((DisplayFixActor44 *)D_8011ECD0)[105 + (i + 1) % 4].unk_24.h[1] - 0x60;
            ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_18 = tbl.value[r1];
            if ((r1 & 1) && r1 < 0x20) {
                ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_28.h[1] = -0x38;
            } else {
                ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_28.h[1] = -0x48;
            }
            ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_38 = 0x01000000;
        }
    }
    for (i = 0; i < 4; i++) {
        ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_24.w += 0xFFFF2000;
        if (((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_24.h[1] < -0xC0) {
            r2 = func_800AE0D0() & 0x3F;
            ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_24.h[1] = ((DisplayFixActor44 *)D_8011ECD0)[109 + (i + 3) % 4].unk_24.h[1] + 0x60;
            ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_18 = tbl.value[r2];
            if ((r2 & 1) && r2 < 0x20) {
                ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_28.h[1] = -0x10;
            } else {
                ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_28.h[1] = -0x20;
            }
            ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_38 = 0x01000000;
        }
    }
    for (i = 0; i < 4; i++) {
        ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_24.w += 0xC000;
        if (((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_24.h[1] >= 0xC1) {
            r3 = func_800AE0D0() & 0x3F;
            ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_24.h[1] = ((DisplayFixActor44 *)D_8011ECD0)[113 + (i + 1) % 4].unk_24.h[1] - 0x60;
            ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_18 = tbl.value[r3];
            if ((r3 & 1) && r3 < 0x20) {
                ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_28.h[1] = 0x18;
            } else {
                ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_28.h[1] = 8;
            }
            ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_38 = 0x01000000;
        }
    }
}
