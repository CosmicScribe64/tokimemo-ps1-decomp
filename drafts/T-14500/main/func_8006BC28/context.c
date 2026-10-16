#define MAIN_API_OVERRIDE_func_80049FF0 /* Line draw calls narrow four coordinates at the call site. T-11060. */
#define MAIN_API_OVERRIDE_D_8011F414    /* Stored as s16 in func_80067870. T-11100. */
#define MAIN_API_OVERRIDE_D_8011ECD0    /* Indexed as DisplayRecord44[] in func_8006A2CC. T-11100. */
#define MAIN_API_OVERRIDE_strncat       /* func_800696DC calls strncat with 2 args for club name. T-11100. */
#define TOKIMEMO_DISPLAY_RECORD44_H     /* Exposes s16 unk_08, unk_1A, unk_1C for func_80068EC0. T-11100. */
#include "common.h"
#include "game.h"

extern s16 D_8011F414;
s32 func_80049FF0(s16 x0, s16 y0, s16 x1, s16 y1, s32 arg4, s32 arg5, s32 arg6);
u8 *strncat();

typedef struct DisplayRecord44 {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 unk_07;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 *unk_0C;
    /* 0x10 */ u8 *unk_10;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ u8 unk_1E[8];
    /* 0x26 */ s16 unk_26;
    /* 0x28 */ u8 unk_28[2];
    /* 0x2A */ s16 unk_2A;
    /* 0x2C */ u8 unk_2C[8];
    /* 0x34 */ u8 *unk_34;
    /* 0x38 */ s32 unk_38;
    /* 0x3C */ u8 unk_3C[8];
} DisplayRecord44; /* size 0x44 */

extern DisplayRecord44 D_8011ECD0[];

/* Flag word of the 4-byte entries at GameState +0x54C (GsWord unk_54C[8]): nibble in bits 25..28, flag in bit 29. */
typedef struct {
    u32 pad : 25;
    u32 nib : 4;
    u32 b29 : 1;
    u32 rest : 2;
} TelEntryFlags; /* size 4 */

void func_800674B0(void) {
    u8 v;
    s32 i;

    func_80044750(5);
    func_80044750(0x71);
    v = D_800E6280.unk_03D;
    if (D_800E6280.unk_03E == 0x60 && D_800E6280.unk_03F == 9 && D_800E6280.unk_040 == 1) {
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

INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_800676AC);

/* NON_MATCHING: T-11100, 142 words differ in registers (14 structural): the original loads D_800B5C08 into $a3 and
 * passes it with `move a0,a3`, and keeps D_80125C54 in $a2 and the D_80125C5x values in other temporaries (T-0018 style
 * global promotion); the draft gets $a0/$t* for them. */
#ifdef NON_MATCHING
void func_80067870(void) {
    s32 x1;
    s16 dx;
    s16 dy;
    s32 y1;

    if (D_800B5C08 == 0 || D_800B5C08 >= 0xE) {
        func_800676AC(0);
        return;
    }
    func_800676AC(D_800B5C08);
    if (D_800B5C08 < 0xE) {
        if ((u32)D_800E6280.unk_F88 >> 16) {
            D_80125C55 = (u32)(D_80125C54 + 1) / 2;
        }
        if (((s32)D_800E6280.unk_10F8 & 0x3F) == 0x3F && D_80125C55 != 0) {
            D_80125C55 -= 1;
        }
        dx = (u32)(rsin(((s32)D_800E6280.unk_10F8 & 0x7F) << 5) * D_80125C55) >> 12;
        D_8011F41C = ((dx * -2) / 3) << 12;
        dy = 0xF - SquareRoot0(D_80125C54 * D_80125C54 - dx * dx);
        x1 = D_80125C50 + dx;
        D_8011F422 = x1;
        y1 = D_80125C52 + D_80125C54;
        D_8011F426 = y1 - dy;
        D_8011F414 = D_800B5C08 - 1;
        D_8011F466 = D_80125C50;
        D_8011F46A = D_80125C52;
        if (D_800E6280.unk_F80 & 2) {
            D_8011F458 = 0xE;
            switch (D_800E6280.unk_F80 & 0x5000) {
            case 0x1000:
                if (D_80125C54 >= 3U) {
                    D_80125C54 -= 2;
                    D_80125C55 = (u32)(D_80125C54 + 1) / 2;
                    y1 = D_80125C52 + D_80125C54;
                }
                break;
            case 0x4000:
                if (D_80125C54 < 0xC4U) {
                    D_80125C54 += 2;
                    D_80125C55 = (u32)(D_80125C54 + 1) / 2;
                    y1 = D_80125C52 + D_80125C54;
                }
                break;
            }
        } else if (D_800E6280.unk_F80 & 8) {
            D_8011F458 = 0xE;
            switch (D_800E6280.unk_F80 & 0x5000) {
            case 0x1000:
                D_80125C52 -= 4;
                y1 = D_80125C52 + D_80125C54;
                break;
            case 0x4000:
                D_80125C52 += 4;
                y1 = D_80125C52 + D_80125C54;
                break;
            }
            switch (D_800E6280.unk_F80 & 0xA000) {
            case 0x8000:
                D_80125C50 -= 4;
                x1 -= 4;
                break;
            case 0x2000:
                D_80125C50 += 4;
                x1 += 4;
                break;
            }
        } else {
            D_8011F458 = 0xD;
        }
        func_80049FF0(D_80125C50, D_80125C52, x1, y1 - dy, 0xF8B0C8, 0xF8B0C8, 1);
        if (D_80125C50 >= 0x201) {
            D_80125C50 = -0x200;
        }
        if (D_80125C50 < -0x200) {
            D_80125C50 = 0x200;
        }
        if (D_80125C52 >= 0x201) {
            D_80125C52 = -0x200;
        }
        if (D_80125C52 < -0x200) {
            D_80125C52 = 0x200;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_80067870);
#endif

INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_80067C28);

/* FAKE: promoted halfword and byte alias retain the final andi before flag storage. T-9550 */
s32 func_80067DD4(void)
{
  u8 *byte;
  s16 promoted;
  u8 value;
  promoted = D_80120696;
  value = promoted;
  byte = &value;
  if (((*byte) & 1) == 0)
  {
    value |= 3;
    D_80120696 = *byte;
  }
}

/* FAKE: promoted halfword and byte alias retain the final andi before flag storage. T-9550 */
s32 func_80067DFC(void)
{
  u8 *byte;
  s16 promoted;
  u8 value;
  promoted = D_80120652;
  value = promoted;
  byte = &value;
  if (((*byte) & 1) == 0)
  {
    value |= 3;
    D_80120652 = *byte;
  }
}

void func_80067E34(void) {
    func_80048F64(0x60);
    D_80120651 = 4;
    D_80120652 = 0x40;
    D_80120688 = 0x40000000;
    D_80120653 = 0x84;
    D_8012065C = sprite_frame_table;
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

INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_80067F04);

s16 func_80068898(s32 arg0, s32 arg1) {
    s32 v = D_800E6280.unk_044[0].unk_20[arg0 * 0x40];

    if (v == 6) {
        v -= 7;
    }
    return arg0 * 0x88 + ((v + arg1 + 1) % 7) * 16 - 0x78;
}

s16 func_800688F0(s32 arg0, s32 arg1) {
    s32 v = D_800E6280.unk_044[0].unk_20[arg0 * 0x40];

    if (v == 6) {
        v -= 7;
    }
    return ((v + arg1 + 1) / 7) * 16 - 0x30;
}

/* FAKE: the loop stores go through the pointer `gs` so uopt reloads D_800B3748 on every trip as the original does (named
 * stores to D_800E6280 keep it in a register); `q` is the +0x800 base of the length/start words. T-11100 */
void func_80068938(u8 arg0, u8 arg1, s32 arg2) {
    s32 idx;
    s32 n;
    s32 i;
    s32 m;
    GameState *gs;
    s32 *q;

    idx = (arg0 - 95) * 12 + arg1 - 4;
    if (idx < 35) {
        LoadSquare(0x3A0, 0x100, 0x20, 0x80, D_800B3748[idx]);
        LoadSquare(0x3A0, 0x180, 0x20, 0x80, D_800B3748[idx + 1]);
        if (arg2 != 0) {
            memcpy(&D_800E6280.unk_044[0], &D_800E6280.unk_044[1], 0x40);
            gs = &D_800E6280;
            q = D_800B3748[idx + 1] + 0x800;
            n = q[0];
            m = q[1];
            D_800E6280.unk_044[1].unk_00[0] = n;
            D_800E6280.unk_044[1].unk_20[0] = m;
            for (i = 0; i < n; i++) {
                gs->unk_044[1].unk_00[i + 1] = D_800B3748[idx + 1][0x802 + i];
                gs->unk_044[1].unk_20[i + 1] = D_800B3748[idx + 1][0x802 + n + i];
            }
        }
    }
}

void func_80068BE4(void) {
    /* FAKE: unused stack locals match original 0x158 frame size and slot offsets. T-11060 */
    u16 top_pad1;
    u16 top_pad2;
    u16 top_pad3;
    u16 top_pad4;
    u16 saved;
    s32 sp10C[16];
    s32 spCC[16];
    s32 sp8C[16];
    s32 half;
    s32 i;
    s32 phase;
    s32 pad;

    phase = D_800E6280.unk_10F8;
    if ((s32) D_800E6280.unk_10F8 & 0x80) {
        half = (0x7F - ((s32) D_800E6280.unk_10F8 & 0x7F)) / 2;
    } else {
        half = ((s32) D_800E6280.unk_10F8 & 0x7F) / 2;
    }
    func_80049FF0(-0x80, -0xC, 0x7C, -0xC, half * 0x10101, half * 0x10101, 0x81);
    saved = D_800E6280.unk_042;
    D_800E6280.unk_042 = saved - 8;
    for (i = 0; i != 0x10; i++) {
        sp10C[i] = func_8006C334(1, 0, 0, -0x20);
        spCC[i] = func_8006C334(0, 1, 0, -0x20);
        sp8C[i] = func_8006C334(0, 0, 1, -0x20);
        D_800E6280.unk_042 += 1;
        if (i != 0) {
            func_80049FF0(i * 0x10 - 0x90, sp10C[i - 1] - 0xC, i * 0x10 - 0x80, sp10C[i] - 0xC, (((i + phase) & 0xF) * 4 + half) * 0x101, (((i + phase) & 0xF) * 4 + half) * 0x101, 0x81);
            func_80049FF0(i * 0x10 - 0x90, spCC[i - 1] - 0xC, i * 0x10 - 0x80, spCC[i] - 0xC, (((i + phase) & 0xF) * 4 + half) * 0x10100 + 0xF0F00, (((i + phase) & 0xF) * 4 + half) * 0x10100 + 0xF0F00, 0x81);
            func_80049FF0(i * 0x10 - 0x90, sp8C[i - 1] - 0xC, i * 0x10 - 0x80, sp8C[i] - 0xC, (((i + phase) & 0xF) * 4 + half) * 0x10001, (((i + phase) & 0xF) * 4 + half) * 0x10001, 0x81);
        }
    }
    D_800E6280.unk_042 = saved;
}

void func_80068EC0(void) {
    s32 i;
    s32 t;

    func_80068BE4();
    dtd_on_tpage(0, 0, 1, 2, 0);
    cal_base_show();
    t = ((DisplayRecord44 *)D_8011ECD0)[0x61].unk_08++;
    for (i = 0x61; i < 0xA0; i++) {
        switch ((t >> 6) & 3) {
        case 0:
            ((DisplayRecord44 *)D_8011ECD0)[i].unk_1A = (t & 0x3F) << 6;
            ((DisplayRecord44 *)D_8011ECD0)[i].unk_1C = (t & 0x3F) << 6;
            break;
        case 1:
            ((DisplayRecord44 *)D_8011ECD0)[i].unk_1A = 0x1000;
            ((DisplayRecord44 *)D_8011ECD0)[i].unk_1C = 0x1000;
            break;
        case 2:
            ((DisplayRecord44 *)D_8011ECD0)[i].unk_1A = (0x3F - (t & 0x3F)) << 6;
            ((DisplayRecord44 *)D_8011ECD0)[i].unk_1C = (0x3F - (t & 0x3F)) << 6;
            break;
        case 3:
            ((DisplayRecord44 *)D_8011ECD0)[i].unk_1A = 0;
            ((DisplayRecord44 *)D_8011ECD0)[i].unk_1C = 0;
            break;
        }
    }
}

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

void cal_sprite_disp_switch(s32 arg0) {
    if (arg0 == 1) {
        D_801217D0[0].unk_00 &= 0x7FFFFFFF;
        D_801217D0[1].unk_00 &= 0x7FFFFFFF;
        D_801217D0[2].unk_00 &= 0x7FFFFFFF;
        D_801217D0[3].unk_00 &= 0x7FFFFFFF;
        return;
    }
    D_801217D0[0].unk_00 |= 0x80000000;
    D_801217D0[1].unk_00 |= 0x80000000;
    D_801217D0[2].unk_00 |= 0x80000000;
    D_801217D0[3].unk_00 |= 0x80000000;
}

void cal_sprite_init(void) {
    D_801217D0[2].unk_00 = 0;
    D_801217D0[2].unk_04 = -0x88;
    D_801217D0[2].unk_06 = -0x58;
    D_801217D0[2].unk_08 = 0x7E;
    D_801217D0[2].unk_0A = 0x7E;
    D_801217D0[2].unk_0C = 0x1E;
    D_801217D0[2].unk_0E = 0x80;
    D_801217D0[2].unk_0F = 0;
    D_801217D0[2].unk_10 = 0xC0;
    D_801217D0[2].unk_12 = 0x1E0;
    D_801217D0[2].unk_14 = 0x80;
    D_801217D0[2].unk_15 = 0x80;
    D_801217D0[2].unk_16 = 0x80;
    D_801217D0[2].unk_18 = 0;
    D_801217D0[2].unk_1A = 0;
    D_801217D0[2].unk_1C = 0x1000;
    D_801217D0[2].unk_1E = 0x1000;
    D_801217D0[2].unk_20 = 0;
    D_800E6280.unk_10A5[2] = 0x84;
    D_801217D0[3].unk_00 = 0;
    D_801217D0[3].unk_04 = 0;
    D_801217D0[3].unk_06 = -0x58;
    D_801217D0[3].unk_08 = 0x7F;
    D_801217D0[3].unk_0A = 0x7F;
    D_801217D0[3].unk_0C = 0x1E;
    D_801217D0[3].unk_0E = 0x80;
    D_801217D0[3].unk_0F = 0x80;
    D_801217D0[3].unk_10 = 0xC0;
    D_801217D0[3].unk_12 = 0x1E0;
    D_801217D0[3].unk_14 = 0x80;
    D_801217D0[3].unk_15 = 0x80;
    D_801217D0[3].unk_16 = 0x80;
    D_801217D0[3].unk_18 = 0;
    D_801217D0[3].unk_1A = 0;
    D_801217D0[3].unk_1C = 0x1000;
    D_801217D0[3].unk_1E = 0x1000;
    D_801217D0[3].unk_20 = 0;
    D_800E6280.unk_10A5[3] = 0x84;
    D_801217D0[0].unk_00 = 0;
    D_801217D0[0].unk_04 = -0x58;
    D_801217D0[0].unk_06 = -0x58;
    D_801217D0[0].unk_08 = 0x18;
    D_801217D0[0].unk_0A = 0x10;
    D_801217D0[0].unk_0C = 0x1E;
    D_801217D0[0].unk_0E = 0xB0;
    D_801217D0[0].unk_0F = 0;
    D_801217D0[0].unk_10 = 0xB0;
    D_801217D0[0].unk_12 = 0x1E0;
    D_801217D0[0].unk_14 = 0x80;
    D_801217D0[0].unk_15 = 0x80;
    D_801217D0[0].unk_16 = 0x80;
    D_801217D0[0].unk_18 = 0;
    D_801217D0[0].unk_1A = 0;
    D_801217D0[0].unk_1C = 0x1000;
    D_801217D0[0].unk_1E = 0x1000;
    D_801217D0[0].unk_20 = 0;
    D_800E6280.unk_10A5[0] = 0x84;
    D_801217D0[1].unk_00 = 0;
    D_801217D0[1].unk_04 = 0x30;
    D_801217D0[1].unk_06 = -0x58;
    D_801217D0[1].unk_08 = 0x18;
    D_801217D0[1].unk_0A = 0x10;
    D_801217D0[1].unk_0C = 0x1E;
    D_801217D0[1].unk_0E = 0xB0;
    D_801217D0[1].unk_0F = 0x80;
    D_801217D0[1].unk_10 = 0xB0;
    D_801217D0[1].unk_12 = 0x1E0;
    D_801217D0[1].unk_14 = 0x80;
    D_801217D0[1].unk_15 = 0x80;
    D_801217D0[1].unk_16 = 0x80;
    D_801217D0[1].unk_18 = 0;
    D_801217D0[1].unk_1A = 0;
    D_801217D0[1].unk_1C = 0x1000;
    D_801217D0[1].unk_1E = 0x1000;
    D_801217D0[1].unk_20 = 0;
    D_800E6280.unk_10A5[1] = 0x84;
}

void data_save_load_class_init(void) {
    func_80048F64(0x60);
    D_80120651 = 3;
    D_80120652 = 0x40;
    D_80120688 = 0x40000000;
    D_80120653 = 0x84;
    D_8012065C = sprite_frame_table;
    D_80120660 = D_800C9A14;
    D_80120684 = D_800C9730;
    D_80120664 = D_800C9A54;
    D_80120654 = 8;
    D_80120655 = 1;
    D_80120668 = 0x43;
    D_80120676 = -0x10;
    D_8012067A = -0x13;
}

/* NON_MATCHING: T-11100, 44 structural differences (603 against 602 instructions, frame 240 against 248): the original
 * keeps the clear counter in $s0 and the loop index in another register, and addresses the NameMetricWork/
 * NameRecordF00 slots through a different set of hoisted constants (lui a3 chain). Registers and frame do not follow
 * from the draft's locals yet. */
#ifdef NON_MATCHING
void func_800696DC(s32 arg0, s32 arg1) {
    s32 i;
    s32 var_s0;
    s32 spEC;
    u8 spC8[32];
    u8 spA8[32];
    u8 sp88[32];
    u8 sp68[32];
    u8 sp48[32];

    if (arg1 != 0) {
        spEC = -0x8C;
    } else {
        spEC = -8;
    }
    k_sub_reset();
    var_s0 = 0;
    if ((u8) D_800E8BEE != 0) {
        var_s0 = 0;
        for (i = 0; i < 13; i++) {
            if (((NameMetricWork *)&D_800E7D10)->flags[i] != 0) {
                var_s0 += 1;
            }
        }
        strcpy(spC8, D_800B3288[D_800E7D36]);
        strncat((s32) spC8, (s32) "番アルバム", 2);
        set_kanji_string((s16) (spEC + 0xC), -0x50, 0xFU, spC8, 0);
        if (D_800E7D36 == 0) {
            set_kanji_string((s16) spEC, -0x40, 0xFU, "システム", 0);
        } else {
            if (((NameMetricWork *)&D_800E7D10)->slot_status[D_800E7D36] == 0) {
                set_kanji_string((s16) spEC, -0x40, 0xFU, "未使用", 0);
            } else if (((NameMetricWork *)&D_800E7D10)->slot_status[D_800E7D36] == 0xFF) {
                set_kanji_string((s16) spEC, -0x40, 0xFU, "データ異常", 0);
            } else {
                strcpy(spC8, D_800B3288[(u8) ((NameMetricWork *)&D_800E7D10)->year[D_800E7D36] / 10U]);
                strncat((s32) spC8, (s32) D_800B3288[(u8) ((NameMetricWork *)&D_800E7D10)->year[D_800E7D36] % 10U], 2);
                strncat((s32) spC8, (s32) "年", 2);
                strncat((s32) spC8, (s32) D_800B3288[((NameMetricWork *)&D_800E7D10)->month[D_800E7D36]], 4);
                strncat((s32) spC8, (s32) "月", 2);
                if (((NameMetricWork *)&D_800E7D10)->day[D_800E7D36] >= 0xAU) {
                    strncat((s32) spC8, (s32) D_800B3288[((NameMetricWork *)&D_800E7D10)->day[D_800E7D36] / 10U], 2);
                }
                strncat((s32) spC8, (s32) D_800B3288[((NameMetricWork *)&D_800E7D10)->day[D_800E7D36] % 10U], 2);
                strncat((s32) spC8, (s32) "日", 2);
                set_kanji_string((s16) spEC, -0x40, 0xFU, spC8, 0);
                strcpy(spA8, "名前…");
                strncat((s32) spA8, (s32) ((NameMetricWork *)&D_800E7D10)->family_name[D_800E7D36], 6);
                strncat((s32) spA8, (s32) ((NameMetricWork *)&D_800E7D10)->given_name[D_800E7D36], 6);
                strcpy(sp88, "あだ名…");
                strncat((s32) sp88, (s32) ((NameMetricWork *)&D_800E7D10)->nickname[D_800E7D36], 0xD);
                set_kanji_string((s16) spEC, -0x30, 0xFU, spA8, 0);
                set_kanji_string((s16) spEC, -0x20, 0xFU, sp88, 0);
                strcpy(sp68, "所属部‥");
                strncat((s32) sp68, club_name_table[((NameMetricWork *)&D_800E7D10)->club[D_800E7D36]]);
                set_kanji_string((s16) spEC, -0x10, 0xFU, sp68, 0);
                strcpy(sp48, "クリア人数‥");
                if (var_s0 >= 0x64) {
                    var_s0 = 0x63;
                }
                if (var_s0 >= 0xA) {
                    strncat((s32) sp48, (s32) D_800B3288[var_s0 / 10], 2);
                }
                strncat((s32) sp48, (s32) D_800B3288[var_s0 % 10], 2);
                set_kanji_string((s16) spEC, 0, 0xFU, sp48, 0);
            }
        }
    } else {
        for (i = 0; i < 13; i++) {
            if (((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].flags[i] != 0) {
                var_s0 += 1;
            }
        }
        if (((NameMetricWork *)&D_800E7D10)->slot_status[D_800E6280.unk_1118] == 0) {
            set_kanji_string((s16) spEC, -0x40, 0xFU, "未使用", 0);
        } else if (((NameMetricWork *)&D_800E7D10)->slot_status[D_800E6280.unk_1118] == 0xFF) {
            set_kanji_string((s16) spEC, -0x40, 0xFU, "データ異常", 0);
        } else {
            strcpy(spC8, D_800B3288[D_800E6280.unk_1118]);
            strncat((s32) spC8, (s32) "番アルバム", 2);
            set_kanji_string((s16) (spEC + 0xC), -0x50, 0xFU, spC8, 0);
            strcpy(spC8, D_800B3288[(u8) ((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].year / 10U]);
            strncat((s32) spC8, (s32) D_800B3288[(u8) ((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].year % 10U], 2);
            strncat((s32) spC8, (s32) "年", 2);
            strncat((s32) spC8, (s32) D_800B3288[((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].month], 4);
            strncat((s32) spC8, (s32) "月", 2);
            if (((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].day >= 0xAU) {
                strncat((s32) spC8, (s32) D_800B3288[((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].day / 10U], 2);
            }
            strncat((s32) spC8, (s32) D_800B3288[((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].day % 10U], 2);
            strncat((s32) spC8, (s32) "日", 2);
            set_kanji_string((s16) spEC, -0x40, 0xFU, spC8, 0);
            strcpy(spA8, "名前…");
            strncat((s32) spA8, (s32) ((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].family_name, 6);
            strncat((s32) spA8, (s32) ((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].given_name, 6);
            strcpy(sp88, "あだ名…");
            strncat((s32) sp88, (s32) ((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].nickname, 0xD);
            set_kanji_string((s16) spEC, -0x30, 0xFU, spA8, 0);
            set_kanji_string((s16) spEC, -0x20, 0xFU, sp88, 0);
            strcpy(sp68, "所属部‥");
            strncat((s32) sp68, club_name_table[((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].club >> 12]);
            set_kanji_string((s16) spEC, -0x10, 0xFU, sp68, 0);
            strcpy(sp48, "クリア人数‥");
            if (var_s0 >= 0x64) {
                var_s0 = 0x63;
            }
            if (var_s0 >= 0xA) {
                strncat((s32) sp48, (s32) D_800B3288[var_s0 / 10], 2);
            }
            strncat((s32) sp48, (s32) D_800B3288[var_s0 % 10], 2);
            set_kanji_string((s16) spEC, 0, 0xFU, sp48, 0);
        }
    }
    k_sub_disp_start(6);
}
#else
INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_800696DC);
#endif

INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_8006A044);

/* NON_MATCHING: T-11100, ~115 differing words: the store triples (unk_18/unk_26/unk_2A of the next DisplayRecord44) are
 * emitted in other orders in several branches (e.g. 26,18,2A or 2A,26,18 with the same constant registers), and the
 * unk_03E/unk_03F byte loads come in the other order at the start of the switch tail. Reordering the C statements
 * changes the constant registers instead; a record pointer local did not help. */
#ifdef NON_MATCHING
void func_8006A2CC(void) {
    s32 pad1;
    s32 var_v1;
    s32 var_v1_2;
    s32 pad2;
    s32 sp3C;

    var_v1 = D_800E6280.unk_03F;
    func_80048E78();
    func_80048F64(0x60);
    D_8011ECD0[96].unk_01 = 5;
    D_8011ECD0[96].unk_02 = 0x40;
    D_8011ECD0[96].unk_38 = 0x40000000;
    D_8011ECD0[96].unk_03 = 0x84;
    D_8011ECD0[96].unk_0C = sprite_frame_table;
    D_8011ECD0[96].unk_10 = D_800C9A14;
    D_8011ECD0[96].unk_34 = D_800C9730;
    D_8011ECD0[96].unk_14 = D_800C9A54;
    D_8011ECD0[96].unk_04 = 8;
    D_8011ECD0[96].unk_05 = 1;
    D_8011ECD0[96].unk_18 = 0x76;
    D_8011ECD0[96].unk_26 = 0;
    D_8011ECD0[96].unk_2A = -0x28;
    for (var_v1 = 0; var_v1 < 0x20; var_v1++) {
        func_80048F64(var_v1 + 0x61);
        D_8011ECD0[var_v1 + 0x61].unk_01 = 4;
        D_8011ECD0[var_v1 + 0x61].unk_02 = 0x40;
        D_8011ECD0[var_v1 + 0x61].unk_38 = 0;
        D_8011ECD0[var_v1 + 0x61].unk_03 = 0x84;
        D_8011ECD0[var_v1 + 0x61].unk_0C = sprite_frame_table;
        D_8011ECD0[var_v1 + 0x61].unk_10 = D_800C9A14;
        D_8011ECD0[var_v1 + 0x61].unk_34 = D_800C9730;
        D_8011ECD0[var_v1 + 0x61].unk_14 = D_800C9A54;
        D_8011ECD0[var_v1 + 0x61].unk_04 = 8;
        D_8011ECD0[var_v1 + 0x61].unk_05 = 1;
        D_8011ECD0[var_v1 + 0x61].unk_06 = 8;
        D_8011ECD0[var_v1 + 0x61].unk_18 = 0;
        D_8011ECD0[var_v1 + 0x61].unk_26 = 0x100;
        D_8011ECD0[var_v1 + 0x61].unk_2A = 0x100;
        D_8011ECD0[var_v1 + 0x61].unk_07 = 0x80;
    }
    var_v1_2 = 0x61;
    switch (D_800E6280.unk_03F) {
    case 2:
    case 3:
    case 4:
        if (D_800E6280.unk_03E == 0x5F) {
            D_8011ECD0[0x61].unk_18 = 0x79;
            D_8011ECD0[0x61].unk_26 = 0;
            sp3C = 0;
            D_8011ECD0[0x61].unk_2A = -0x28;
            var_v1_2++;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E0C, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E25, 0);
        }
        if (D_800E6280.unk_03E == 0x60) {
            sp3C = 4;
            if (D_800E6280.unk_03F != 2) {
                D_800E6280.unk_120[13] = 1;
            }
            D_8011ECD0[var_v1_2].unk_18 = 0x7A;
            D_8011ECD0[var_v1_2].unk_26 = 0;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
            D_8011ECD0[var_v1_2].unk_18 = 0x81;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x61) {
            sp3C = 8;
            if (D_800E6280.unk_03F != 2) {
                D_800E6280.unk_120[11] = 1;
            }
            D_8011ECD0[var_v1_2].unk_18 = 0x7A;
            D_8011ECD0[var_v1_2].unk_26 = 0;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
            D_8011ECD0[var_v1_2].unk_18 = 0x84;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x62) {
            sp3C = 0xC;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E70, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E89, 0);
        }
        break;
    case 5:
    case 6:
    case 7:
        D_8011ECD0[0x61].unk_18 = 0x7B;
        D_8011ECD0[0x61].unk_26 = 0;
        D_8011ECD0[0x61].unk_2A = -0x28;
        var_v1_2++;
        if (D_800E6280.unk_03E == 0x5F) {
            sp3C = 1;
            if (D_800E6280.unk_03F != 5) {
                D_800E6280.unk_120[1] = 1;
            }
            D_8011ECD0[var_v1_2].unk_18 = 0x7E;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x60) {
            sp3C = 5;
            if (D_800E6280.unk_03F != 5) {
                D_800E6280.unk_120[5] = 1;
            }
            D_8011ECD0[var_v1_2].unk_18 = 0x82;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x61) {
            sp3C = 9;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E3E, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E57, 0);
        }
        break;
    case 8:
    case 9:
    case 10:
        D_8011ECD0[0x61].unk_18 = 0x7C;
        D_8011ECD0[0x61].unk_26 = 0;
        D_8011ECD0[0x61].unk_2A = -0x28;
        var_v1_2++;
        if (D_800E6280.unk_03E == 0x5F) {
            sp3C = 2;
            if (D_800E6280.unk_03F != 8) {
                D_800E6280.unk_120[7] = 1;
            }
            D_8011ECD0[var_v1_2].unk_18 = 0x7F;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x60) {
            sp3C = 6;
            if (D_800E6280.unk_03F != 8) {
                D_800E6280.unk_120[10] = 1;
            }
            D_8011ECD0[var_v1_2].unk_18 = 0x83;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x61) {
            sp3C = 0xA;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E3E, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E57, 0);
        }
        break;
    case 11:
    case 12:
        D_8011ECD0[0x61].unk_18 = 0x7D;
        D_8011ECD0[0x61].unk_26 = 0;
        D_8011ECD0[0x61].unk_2A = -0x28;
        var_v1_2++;
        if (D_800E6280.unk_03E == 0x5F) {
            sp3C = 3;
            if (D_800E6280.unk_03F == 0xC) {
                D_800E6280.unk_120[3] = 1;
            }
            D_8011ECD0[var_v1_2].unk_18 = 0x80;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x60) {
            sp3C = 7;
            if (D_800E6280.unk_03F == 0xC) {
                D_800E6280.unk_120[6] = 1;
            }
            D_8011ECD0[var_v1_2].unk_18 = 0x85;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x61) {
            sp3C = 0xB;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E70, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E89, 0);
        }
        break;
    case 1:
        D_8011ECD0[0x61].unk_18 = 0x7D;
        D_8011ECD0[0x61].unk_26 = 0;
        D_8011ECD0[0x61].unk_2A = -0x28;
        var_v1_2++;
        if (D_800E6280.unk_03E == 0x60) {
            D_800E6280.unk_120[3] = 1;
            sp3C = 3;
            D_8011ECD0[var_v1_2].unk_18 = 0x80;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x61) {
            D_800E6280.unk_120[6] = 1;
            sp3C = 7;
            D_8011ECD0[var_v1_2].unk_18 = 0x85;
            D_8011ECD0[var_v1_2].unk_26 = 8;
            D_8011ECD0[var_v1_2].unk_2A = -0x28;
            var_v1_2++;
        }
        if (D_800E6280.unk_03E == 0x62) {
            sp3C = 0xB;
            set_kanji_string(-0x9E, -0x3C, 3U, D_800B5E70, 0);
            set_kanji_string(-0x9E, -0x2C, 3U, D_800B5E89, 0);
        }
        break;
    }
    if ((D_800E6280.unk_03E != 0x5F) || (D_800E6280.unk_03F >= 6U)) {
        D_8011ECD0[var_v1_2].unk_18 = 0x77;
        D_8011ECD0[var_v1_2].unk_26 = 0;
        D_8011ECD0[var_v1_2].unk_2A = -0x28;
        var_v1_2++;
    }
    if (D_800E6280.unk_03E == 0x5F) {
        D_8011ECD0[var_v1_2].unk_18 = (s16) (sp3C + 0x97);
        D_8011ECD0[var_v1_2].unk_26 = 0;
        D_8011ECD0[var_v1_2].unk_2A = -0x28;
        var_v1_2++;
    } else if ((D_800E6280.unk_03E == 0x60) && (D_800E6280.unk_03F < 2U)) {
        D_8011ECD0[var_v1_2].unk_18 = (s16) (sp3C + 0x97);
        D_8011ECD0[var_v1_2].unk_26 = 0;
        D_8011ECD0[var_v1_2].unk_2A = -0x28;
        var_v1_2++;
    } else {
        D_8011ECD0[var_v1_2].unk_01++;
        D_8011ECD0[var_v1_2].unk_18 = 0x78;
        D_8011ECD0[var_v1_2].unk_26 = 0;
        D_8011ECD0[var_v1_2].unk_2A = -0x28;
        var_v1_2++;
        D_8011ECD0[var_v1_2].unk_18 = (s16) (D_800B3750[sp3C][0] + 0x90);
        D_8011ECD0[var_v1_2].unk_26 = 0x110;
        D_8011ECD0[var_v1_2].unk_2A = 0x18;
        var_v1_2++;
        D_8011ECD0[var_v1_2].unk_18 = (s16) (D_800B3750[sp3C][1] + 0x90);
        D_8011ECD0[var_v1_2].unk_26 = 0x110;
        D_8011ECD0[var_v1_2].unk_2A = 0x28;
        var_v1_2++;
        D_8011ECD0[var_v1_2].unk_18 = (s16) (D_800B3750[sp3C][2] + 0x90);
        D_8011ECD0[var_v1_2].unk_26 = 0x110;
        D_8011ECD0[var_v1_2].unk_2A = 0x38;
        var_v1_2++;
        set_kanji_string(0x100, 0x10, 6U, D_800B32CC[(sp3C * 3 + 2) % 12], 0);
        set_kanji_string(0x100, 0x20, 6U, D_800B32CC[(sp3C * 3 + 3) % 12], 0);
        set_kanji_string(0x100, 0x30, 6U, D_800B32CC[(sp3C * 3 + 4) % 12], 0);
    }
    if (D_800B5DFC[sp3C] != 0xFF) {
        D_8011ECD0[var_v1_2].unk_18 = (s16) D_800B5DFC[sp3C];
        D_8011ECD0[var_v1_2].unk_26 = 0;
        D_8011ECD0[var_v1_2].unk_2A = -0x28;
        var_v1_2++;
    }
    set_kanji_string(0x20, -0x5E, 7U, D_800B5D1C[sp3C], 0);
    if (D_800B5D0C[sp3C] != 0xFF) {
        D_8011ECD0[var_v1_2].unk_18 = (s16) D_800B5D0C[sp3C];
        D_8011ECD0[var_v1_2].unk_26 = 0;
        D_8011ECD0[var_v1_2].unk_2A = -0x28;
        var_v1_2++;
    }
    set_kanji_string(0x20, -0x3E, 8U, D_800B5C2C[sp3C], 0);
    if (sp3C < 4) {
        D_8011ECD0[var_v1_2].unk_18 = (s16) ((sp3C & 3) + 0x97);
        D_8011ECD0[var_v1_2].unk_26 = 0;
        D_8011ECD0[var_v1_2].unk_2A = -0x28;
        var_v1_2++;
    }
    if (sp3C >= 4) {
        D_8011ECD0[var_v1_2].unk_18 = (s16) ((sp3C & 3) + 0x93);
        D_8011ECD0[var_v1_2].unk_26 = 0;
        D_8011ECD0[var_v1_2].unk_2A = -0x28;
        var_v1_2++;
    }
    set_kanji_string(-0x8F, -0xE, 6U, D_800B5EA4[sp3C][0], 0);
    set_kanji_string(-0x8F, 2, 6U, D_800B5EA4[sp3C][1], 0);
    set_kanji_string(-0x8F, 0x12, 6U, D_800B5EA4[sp3C][2], 0);
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    gnsx(&D_800E6280.unk_0D4[0]);
    sndisp(D_800B66A4[sp3C], 0, 0x1F);
}
#else
INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_8006A2CC);
#endif

s32 appointment_find_today(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 r;

    r = 0;
    if (arg1 == 0xD) {
        arg1 = 1;
    }
    if (arg1 == 0xC && arg2 == 0x18) {
        return -1;
    }
    if (arg1 == 0xD) {
        if (arg2 == 1) {
            return -1;
        }
    }
    if (arg1 == 1 && arg2 == 1) {
        return -1;
    }
    if (arg1 >= 3 && arg0 >= 0x62) {
        return -1;
    }
    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 0; i < 8; i++) if (arg1 == ((u32) D_800E6280.unk_54C[i].h[0] >> 12) && arg2 == (D_800E6280.unk_54C[i].h[1] & 0x1F)) r = i + 1;
    return r;
}

typedef struct EntryFlags54C {
    u32 pad : 25;
    u32 nib : 4;
    u32 b29 : 1;
    u32 rest : 2;
} EntryFlags54C;

s32 func_8006B014(s32 arg0) {
    s32 i;
    s32 n;

    n = 0;
    for (i = 0; i < 8; i++) {
        if (((EntryFlags54C *)&D_800E6280.unk_54C[i])->b29 && arg0 == ((EntryFlags54C *)&D_800E6280.unk_54C[i])->nib) {
            n++;
        }
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_8006B0C8);

void func_8006B5BC(GsWord arg0) {
    s32 i;

    i = 0;
    if (((TelEntryFlags *)&D_800E6280.unk_54C[i])->b29) {
        if (i) { /* FAKE: the empty test keeps i from folding to 0, the original has `sll t9,zero,2; addu v0,t9,base`; real source unknown. T-9340 */
        }
        do {
            i++;
        } while (((TelEntryFlags *)&D_800E6280.unk_54C[i])->b29 && i != 8);
    }
    D_800E6280.unk_54C[i] = arg0;
    func_8006B0C8();
}

void func_8006B640(void) {
}

/* NON_MATCHING: T-11040, 10 differing lines: the loop test is `li at,11; bne s0,at` where the original keeps `slti at,s0,11; bnez` (uopt counted-loop normalisation), and the constants for the cancel entry (li a0,40 / a1,8) come after li t9,64 / t0,16 where the original has them first. Frame, array slots and registers match (arrays x,y,w,h declared in that order; `first`,`i` and a pad above them). */
#ifdef NON_MATCHING
void club_list_menu(void) {
    s32 pad[2];
    s32 first;
    s32 i;
    s16 x[12];
    s16 y[12];
    s16 w[12];
    s16 h[12];

    func_80048F64(0x60);
    D_80120651 = 4;
    D_80120652 = 0x40;
    D_80120688 = 0x40000000;
    D_80120653 = 0x84;
    D_8012065C = sprite_frame_table;
    D_80120660 = D_800C9A14;
    D_80120684 = D_800C9730;
    D_80120664 = D_800C9A54;
    D_80120654 = 8;
    D_80120655 = 1;
    D_80120668 = 0x3F;
    D_80120676 = -0x10;
    D_8012067A = -0x13;
    set_kanji_string(-0x80, -0x52, 3, "部活一覧", 0);
    first = -1;
    for (i = 0; i < 0xB; i++) {
        x[i] = (i / 5) * 0x54 - 0x80;
        y[i] = (i % 5) * 0x10 - 0x38;
        w[i] = 0x40;
        h[i] = 0x10;
        if (!(D_800E6280.unk_66C[i].unk_03 & 1)) {
            set_kanji_string(x[i], y[i], 3, club_name_table[i], 0);
            if (first == -1) {
                first = i;
            }
        } else {
            w[i] = 0;
        }
    }
    w[11] = 0x40;
    h[11] = 0x10;
    set_kanji_string(x[11] = 0x28, y[11] = 8, 3, "取り消し", 0);
    menu_set(1, 0xC, x, y, w, h);
    D_800E6280.unk_1094 = first;
    k_sub_reset_point_set();
    sndi("入部する部活を選択してください", 0, 0xFF);
    k_sub_disp_start(1);
}
#else
INCLUDE_ASM("asm/nonmatchings/main/800674B0", club_list_menu);
#endif

void func_8006B900(void) {
    s32 i;

    _sprite_set_light_effect1(-0x120, -0xF8, D_800B66E0[0], 0x3C, 0xA0, 0x3C, 9, 0xA0A0A0, D_800B66F4[0]);
    for (i = 1; i < 9; i++) {
        _sprite_set_light_effect1(-0x120, -0xF8, D_800B66E0[i], 0x3C, 0, 0xB4, 9, 0xA0A0A0, D_800B66F4[i]);
    }
    dtd_on_tpage(0, 0, 9, 1, 0);
    _sprite_set_light_effect1(-0x140, -0x118, -0x142, -0x141, -0xA2, -0x79, 9, 0x808080, 0);
    dtd_on_tpage(0, 0, 9, 1, 0);
}

INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_8006BA40);

/* bulk_m2c declarations (T-11190) */
extern s16 D_8011ECE4;
extern s16 D_8011ECE6;
extern s32 D_8011ED08;
extern s8 D_8011ECD1;
extern s8 D_8011ECD2;
extern s8 D_8011ECD4;
extern s8 D_8011ECD5;
extern u8 *D_8011ECDC;
extern u8 *D_8011ECE0;
extern u8 *D_8011ED04;
/* end bulk_m2c declarations */
void func_8006BC28(s32 arg0) {
    s16 sp2E;
    s16 sp2C;

    sp2E = D_8011ECF6;
    sp2C = D_8011ECFA;
    func_80048F64(0);
    D_8011ECD1 = 1;
    D_8011ECD2 = 0x40;
    D_8011ED08 = 0;
    D_8011ECD3 = 0x84;
    D_8011ECDC = sprite_frame_table;
    D_8011ECE0 = D_800C9A14;
    D_8011ED04 = D_800C9730;
    D_8011ECE4 = D_800C9A54;
    D_8011ECD4 = 8;
    D_8011ECD5 = 1;
    D_8011ECE6 = 0;
    if ((arg0 == 0) || (arg0 != 1)) {
        if ((D_800B6708 == 0) || ((s32) D_800B6708 < 0x9B) || (D_8011ECE8 = (s16) D_800B6708, (((s32) D_800B6708 < 0xAE) == 0))) {
            goto block_10;
        }
    } else if ((D_800B6708 == 0) || ((s32) D_800B6708 < 0x9B) || (D_8011ECE8 = (s16) D_800B6708, (((s32) D_800B6708 < 0xAE) == 0))) {
block_10:
        D_8011ECE8 = 0;
    }
    D_8011ECF6 = sp2E;
    D_8011ECFA = sp2C;
}

void func_8006BD6C(s32 arg0) {
    if (arg0 == 1) {
        D_8011ECD3 |= 0x80;
    } else {
        D_8011ECD3 &= 0x7F;
    }
}

s32 func_8006BDA8(s32 arg0) {
    /* FAKE: unused stack word matches original 0x40 frame size and local offsets. T-11060 */
    s32 pad;
    s32 score;
    s32 failures;
    u16 day;
    u16 old_day;
    s32 factor;

    old_day = D_800E6280.unk_042;
    day = old_day;
    D_800E6280.unk_042 = old_day + arg0;
    score = 0;
    switch (D_800E6280.unk_F68.w & 15) {
    case 0:
        score = func_8006C334(0, 0, 1, 16) + D_800E6280.unk_100.unk_02 / 16;
        break;
    case 1:
        score = func_8006C334(0, 0, 1, 16) + D_800E6280.unk_104.unk_02 / 16;
        break;
    case 2:
        score = func_8006C334(0, 1, 0, 16) + D_800E6280.unk_108.unk_02 / 16;
        break;
    case 3:
        score = func_8006C334(1, 0, 0, 16) + D_800E6280.unk_10C.unk_02 / 16;
        break;
    case 4:
        switch ((u16)D_800E6280.unk_0F4.h >> 12) {
        case 0:
            score = func_8006C334(0, 0, 1, 16);
            break;
        case 1:
            score = func_8006C334(0, 1, 0, 16);
            break;
        case 2:
            score = func_8006C334(0, 0, 1, 16);
            break;
        case 3:
            score = func_8006C334(0, 0, 1, 16);
            break;
        case 4:
            score = func_8006C334(0, 1, 0, 16);
            break;
        case 5:
            score = func_8006C334(0, 1, 0, 16);
            break;
        case 6:
            score = func_8006C334(1, 0, 0, 16);
            break;
        case 7:
            score = func_8006C334(1, 0, 0, 16);
            break;
        case 8:
            score = func_8006C334(1, 0, 0, 16);
            break;
        case 9:
            score = func_8006C334(1, 0, 0, 16);
            break;
        case 10:
            score = func_8006C334(1, 0, 0, 16);
            break;
        }
        score += (u16)D_800E6280.unk_66C[(u16)D_800E6280.unk_0F4.h >> 12].unk_00 >> 4;
        break;
    case 5:
        score = func_8006C334(0, 1, 0, 16) + D_800E6280.unk_110.unk_02 / 16;
        break;
    case 6:
        score = func_8006C334(0, 1, 0, 16) + D_800E6280.unk_114.unk_02 / 16;
        break;
    case 7:
        func_8006C334(1, 1, 1, 16);
        score = 16;
        break;
    }
    score += 6;
    if (D_800E6280.unk_11C.unk_02 >= 97) {
        factor = 4;
    } else {
        factor = D_800E6280.unk_11C.unk_02 / 32;
    }
    score = rand() % (score + 4);
    score = (8 - factor) * score / 8;
    failures = 0;
    if (func_8006C334(1, 0, 0, 3) == 0) {
        failures++;
    }
    if (func_8006C334(0, 1, 0, 3) == 0) {
        failures++;
    }
    if (func_8006C334(0, 0, 1, 3) == 0) {
        failures++;
    }
    if (failures == 3) {
        score /= 4;
    }
    D_800E6280.unk_042 = day;
    if (score >= 4) {
        return 0;
    }
    return 1;
}

void func_8006C308(void) {
    func_8006C334(1, 1, 1, 2);
}

s32 func_8006C334(s32 a, s32 b, s32 c, s32 scale) {
    s32 magnitude;
    s32 offset;
    s32 day;
    s32 wave_a;
    s32 wave_b;
    s32 wave_c;
    s32 bad;
    s32 count;
    s32 total;

    wave_a = rsin((day = (offset = func_8006C700()) + (u16)D_800E6280.unk_042) * 0xB2);
    wave_b = rsin(day * 0x92);
    wave_c = rsin(day * 0x7C);
    bad = 0;
    count = 0;
    total = 0;
    if (a != 0) {
        count++;
        if ((magnitude = abs(wave_a)) < 0x199) {
            total -= 0x1000;
            bad++;
        } else if (wave_a > 0) {
            total += wave_a;
        } else {
            total -= wave_a / 2;
        }
    }
    if (b != 0) {
        count++;
        if ((magnitude = abs(wave_b)) < 0x199) {
            total -= 0x1000;
            bad++;
        } else if (wave_b > 0) {
            total += wave_b;
        } else {
            total -= wave_b / 2;
        }
    }
    if (c != 0) {
        count++;
        if ((magnitude = abs(wave_c)) < 0x199) {
            total -= 0x1000;
            bad++;
        } else if (wave_c > 0) {
            total += wave_c;
        } else {
            total -= wave_c / 2;
        }
    }
    if (count == 0) return 1;
    if (scale > 0) {
        if (total / count < 0) return 0;
        return (total * scale / 4096) / count;
    } else if (scale < 0) {
        if (a != 0) return wave_a * scale / 4096;
        if (b != 0) return wave_b * scale / 4096;
        if (c != 0) return wave_c * scale / 4096;
    } else {
        /* The original retains this count check in the zero-scale path. T-10880. */
        if (count == 0) return 1;
        if (count == 1) {
            if (bad != 0) return -1;
            return 1;
        }
        if (count == 2) {
            if (bad != 0) return -1;
            return 1;
        }
        if (count == 3) {
            if (bad >= 2) return -1;
            else return 1;
        }
        return 1;
    }
}

s32 func_8006C700(void) {
    s32 result;
    result = -(s32)D_800B670A[D_800E6280.unk_0F8 & 0xF] - (((u32)D_800E6280.unk_0F8 << 23) >> 27) + 0x1563;
    if ((D_800E6280.unk_0F8 & 0xF) == 4 && (((u32)D_800E6280.unk_0F8 << 23) >> 27) < 4U) {
        result = -(s32)D_800B670A[3] - (((u32)D_800E6280.unk_0F8 << 23) >> 27) + 0x1582;
    }
    return result;
}

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
    D_8011F0D8 = sprite_frame_table;
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
    D_8011ED64 = sprite_frame_table;
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

s16 func_8006C934(void) {
    if (D_8011ED82 < 0x4F) {
        if (!(D_800E6280.unk_10F8 & 3)) {
            D_8011ED82 += 1;
        }
    } else {
        D_8011ED82 = 0x48;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_8006C988);

u8 *func_8006CA9C(void) {
    if (D_800E6280.unk_F74 == 0) {
        if (D_800E6280.unk_F75 != 0) {
            return D_800B672C;
        }
        return D_800B6730;
    }
    return D_800B6724;
}

u8 *func_8006CAE0(void) {
    if (D_800E6280.unk_F74 == 0) {
        if (D_800E6280.unk_F75 != 0) {
            return D_800B6730;
        }
        return D_800B672C;
    }
    return D_800B6728;
}
