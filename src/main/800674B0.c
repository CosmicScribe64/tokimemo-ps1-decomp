#define MAIN_API_OVERRIDE_func_80049FF0 /* Line draw calls narrow four coordinates at the call site. T-11060. */
#include "common.h"
#include "game.h"

s32 func_80049FF0(s16 x0, s16 y0, s16 x1, s16 y1, s32 arg4, s32 arg5, s32 arg6);

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

INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_80067870);

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

INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_80068938);

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

INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_80068EC0);

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

INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_800696DC);

INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_8006A044);

INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_8006A2CC);

s32 func_8006AEC4(s32 arg0, s32 arg1, s32 arg2) {
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
void func_8006B648(void) {
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
    D_8012065C = D_800C975C;
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
            set_kanji_string(x[i], y[i], 3, D_800B3908[i], 0);
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
INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_8006B648);
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

INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_8006BC28);

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
