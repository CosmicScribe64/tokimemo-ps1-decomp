#define MAIN_API_OVERRIDE_func_80049A40 /* T-10890: matched full-width coordinate calls; narrow canonical prototype adds conversions. */
#include "common.h"
#include "game.h"
#include "psx_types.h"

void func_80049A40();

void func_80062CD0(s32 arg0) {
    cd_read_sectors(0x2D, 0x80180000, arg0);
    D_8011F50F = 0;
    D_8011F4CB = 0;
    D_800B5A60 = 0;
}

void func_80062D0C(s32 arg0) {
    if (arg0 == 0) {
        D_8011ECD0[0x83F] &= 0x7F;
        D_8011ECD0[0x7FB] &= 0x7F;
        D_801217D0[0].unk_00 |= 0x80000000;
        D_801217D0[1].unk_00 |= 0x80000000;
        D_801217D0[2].unk_00 |= 0x80000000;
        D_801217D0[3].unk_00 |= 0x80000000;
        D_801217D0[4].unk_00 |= 0x80000000;
        D_801217D0[5].unk_00 |= 0x80000000;
    }
}

void func_80062DBC(s32 arg0, s32 *arg1, u8 **arg2, u8 *arg3, s32 arg4) {
    s16 x0;
    s16 y0;
    s16 w0;
    s16 h0;
    s16 x1;
    s16 y1;
    s16 origin_x;
    s16 origin_y;
    s16 w1;
    s16 h1;
    DisplayImage78 *temp_v0;

    load_palette(arg0, 0x10, 1, 2, 0);
    load_tpage_buf_lock(5, arg1[0], 1);
    load_tpage_buf_lock(6, arg1[1], 1);
    load_tpage_buf_lock(7, arg1[2], 1);
    load_tpage_buf_lock(8, arg1[3], 1);
    load_tpage_buf_lock(9, arg1[4], 1);
    func_80048F64(0x1F);
    ((DisplayActor44 *)D_8011ECD0)[31].unk_01 = 0xC;
    ((DisplayActor44 *)D_8011ECD0)[31].unk_02 = 0;
    ((DisplayActor44 *)D_8011ECD0)[31].unk_38 = 0xC1000000;
    ((DisplayActor44 *)D_8011ECD0)[31].unk_03 = 0x8C;
    ((DisplayActor44 *)D_8011ECD0)[31].unk_0C = (u8 *)arg2;
    ((DisplayActor44 *)D_8011ECD0)[31].unk_10 = arg3;
    ((DisplayActor44 *)D_8011ECD0)[31].unk_34 = (u8 *)arg1;
    ((DisplayActor44 *)D_8011ECD0)[31].unk_16 = 0;
    ((DisplayActor44 *)D_8011ECD0)[31].unk_06 = 2;
    ((DisplayActor44 *)D_8011ECD0)[31].unk_43 = 0x10;
    ((DisplayActor44 *)D_8011ECD0)[31].unk_2A = -0x20;
    ((DisplayActor44 *)D_8011ECD0)[31].unk_07 = 0;
    func_80048F64(0x1E);
    ((DisplayActor44 *)D_8011ECD0)[30].unk_01 = 0xC;
    ((DisplayActor44 *)D_8011ECD0)[30].unk_02 = 0;
    ((DisplayActor44 *)D_8011ECD0)[30].unk_38 = 0xC1000000;
    ((DisplayActor44 *)D_8011ECD0)[30].unk_03 = 0x8C;
    ((DisplayActor44 *)D_8011ECD0)[30].unk_0C = (u8 *)arg2;
    ((DisplayActor44 *)D_8011ECD0)[30].unk_10 = arg3;
    ((DisplayActor44 *)D_8011ECD0)[30].unk_34 = (u8 *)arg1;
    ((DisplayActor44 *)D_8011ECD0)[30].unk_16 = 1;
    ((DisplayActor44 *)D_8011ECD0)[30].unk_06 = 2;
    ((DisplayActor44 *)D_8011ECD0)[30].unk_43 = 0x10;
    ((DisplayActor44 *)D_8011ECD0)[30].unk_2A = -0x20;
    ((DisplayActor44 *)D_8011ECD0)[30].unk_07 = 0;
    temp_v0 = (DisplayImage78 *)*arg2;
    if (temp_v0->count >= 6) {
        origin_x = temp_v0->origin_x;
        origin_y = temp_v0->origin_y;
        x0 = temp_v0->part[5].unk_00;
        y0 = temp_v0->part[5].unk_02;
        x1 = temp_v0->part[4].unk_00;
        y1 = temp_v0->part[4].unk_02;
        D_801217D0->unk_00 = 0x81000000;
        D_801217D0[0].unk_08 = 0x10;
        D_801217D0[0].unk_0A = 0x10;
        D_801217D0[0].unk_0E = 0x30;
        D_801217D0[0].unk_0F = 0;
        D_801217D0[0].unk_10 = 0;
        D_801217D0[0].unk_12 = 0x1F0;
        D_801217D0[0].unk_14 = 0x80;
        D_801217D0[0].unk_15 = 0x80;
        D_801217D0[0].unk_16 = 0x80;
        D_801217D0[0].unk_18 = 0;
        D_801217D0[0].unk_1A = 0;
        D_801217D0[0].unk_1C = 0x1000;
        D_801217D0[0].unk_1E = 0x1000;
        D_801217D0[0].unk_20 = 0;
        D_801217D0[0].unk_04 = (((DisplayActor44 *)D_8011ECD0)[31].unk_26 + x0) - origin_x;
        D_801217D0[0].unk_06 = (((DisplayActor44 *)D_8011ECD0)[31].unk_2A + y0) - origin_y;
        D_800E6280.unk_10A5[0] = 0xB;
        D_801217D0[1].unk_00 = 0x81000000;
        D_801217D0[1].unk_08 = 0x30;
        D_801217D0[1].unk_0A = 0x20;
        D_801217D0[1].unk_0E = 0;
        D_801217D0[1].unk_0F = 0;
        D_801217D0[1].unk_10 = 0;
        D_801217D0[1].unk_12 = 0x1F0;
        D_801217D0[1].unk_14 = 0x80;
        D_801217D0[1].unk_15 = 0x80;
        D_801217D0[1].unk_16 = 0x80;
        D_801217D0[1].unk_18 = 0;
        D_801217D0[1].unk_1A = 0;
        D_801217D0[1].unk_1C = 0x1000;
        D_801217D0[1].unk_1E = 0x1000;
        D_801217D0[1].unk_20 = 0;
        D_801217D0[1].unk_04 = (((DisplayActor44 *)D_8011ECD0)[31].unk_26 + x1) - origin_x;
        D_801217D0[1].unk_06 = (((DisplayActor44 *)D_8011ECD0)[31].unk_2A + y1) - origin_y;
        D_800E6280.unk_10A5[1] = 0xB;
    }
    if (temp_v0->count >= 4) {
        x0 = temp_v0->part[2].unk_00;
        y0 = temp_v0->part[2].unk_02;
        w0 = temp_v0->part[2].unk_0A;
        h0 = temp_v0->part[2].unk_0C;
        x1 = temp_v0->part[3].unk_00;
        y1 = temp_v0->part[3].unk_02;
        w1 = temp_v0->part[3].unk_0A;
        h1 = temp_v0->part[3].unk_0C;
        D_801217D0[2].unk_00 = -0x3F000000;
        D_801217D0[2].unk_08 = 0x10;
        D_801217D0[2].unk_0A = w0;
        D_801217D0[2].unk_0E = (s8) h0;
        D_801217D0[2].unk_0F = 0;
        D_801217D0[2].unk_10 = 0;
        D_801217D0[2].unk_12 = 0x1F0;
        D_801217D0[2].unk_14 = 0x80;
        D_801217D0[2].unk_15 = 0x80;
        D_801217D0[2].unk_16 = 0x80;
        D_801217D0[2].unk_18 = 0;
        D_801217D0[2].unk_1A = 0;
        D_801217D0[2].unk_1C = 0x1000;
        D_801217D0[2].unk_1E = 0x1000;
        D_801217D0[2].unk_20 = 0;
        D_801217D0[2].unk_04 = (((DisplayActor44 *)D_8011ECD0)[31].unk_26 + x0) - origin_x;
        D_801217D0[2].unk_06 = (((DisplayActor44 *)D_8011ECD0)[31].unk_2A + y0) - origin_y;
        D_800E6280.unk_10A5[2] = 0x8B;
        D_801217D0[3].unk_00 = -0x3F000000;
        D_801217D0[3].unk_08 = w1;
        D_801217D0[3].unk_0A = h1;
        D_801217D0[3].unk_0E = 0;
        D_801217D0[3].unk_0F = 0;
        D_801217D0[3].unk_10 = 0;
        D_801217D0[3].unk_12 = 0x1F0;
        D_801217D0[3].unk_14 = 0x80;
        D_801217D0[3].unk_15 = 0x80;
        D_801217D0[3].unk_16 = 0x80;
        D_801217D0[3].unk_18 = 0;
        D_801217D0[3].unk_1A = 0;
        D_801217D0[3].unk_1C = 0x1000;
        D_801217D0[3].unk_1E = 0x1000;
        D_801217D0[3].unk_20 = 0;
        D_801217D0[3].unk_04 = (((DisplayActor44 *)D_8011ECD0)[31].unk_26 + x1) - origin_x;
        D_801217D0[3].unk_06 = (((DisplayActor44 *)D_8011ECD0)[31].unk_2A + y1) - origin_y;
        D_800E6280.unk_10A5[3] = 0x8B;
    }
    if (temp_v0->count >= 7) {
        x0 = temp_v0->part[6].unk_00;
        y0 = temp_v0->part[6].unk_02;
        w0 = temp_v0->part[6].unk_0A;
        h0 = temp_v0->part[6].unk_0C;
        D_801217D0[4].unk_00 = -0x3F000000;
        D_801217D0[4].unk_08 = 0x10;
        D_801217D0[4].unk_0A = w0;
        D_801217D0[4].unk_0E = (s8) h0;
        D_801217D0[4].unk_0F = 0;
        D_801217D0[4].unk_10 = 0;
        D_801217D0[4].unk_12 = 0x1F0;
        D_801217D0[4].unk_14 = 0x80;
        D_801217D0[4].unk_15 = 0x80;
        D_801217D0[4].unk_16 = 0x80;
        D_801217D0[4].unk_18 = 0;
        D_801217D0[4].unk_1A = 0;
        D_801217D0[4].unk_1C = 0x1000;
        D_801217D0[4].unk_1E = 0x1000;
        D_801217D0[4].unk_20 = 0;
        D_801217D0[4].unk_04 = (((DisplayActor44 *)D_8011ECD0)[31].unk_26 + x0) - origin_x;
        D_801217D0[4].unk_06 = (((DisplayActor44 *)D_8011ECD0)[31].unk_2A + y0) - origin_y;
        D_800E6280.unk_10A5[4] = 0x8B;
    }
    if (temp_v0->count == 8) {
        x1 = temp_v0->part[7].unk_00;
        y1 = temp_v0->part[7].unk_02;
        w1 = temp_v0->part[7].unk_0A;
        h1 = temp_v0->part[7].unk_0C;
        D_801217D0[5].unk_00 = -0x3F000000;
        D_801217D0[5].unk_08 = w1;
        D_801217D0[5].unk_0A = h1;
        D_801217D0[5].unk_0E = 0;
        D_801217D0[5].unk_0F = 0;
        D_801217D0[5].unk_10 = 0;
        D_801217D0[5].unk_12 = 0x1F0;
        D_801217D0[5].unk_14 = 0x80;
        D_801217D0[5].unk_15 = 0x80;
        D_801217D0[5].unk_16 = 0x80;
        D_801217D0[5].unk_18 = 0;
        D_801217D0[5].unk_1A = 0;
        D_801217D0[5].unk_1C = 0x1000;
        D_801217D0[5].unk_1E = 0x1000;
        D_801217D0[5].unk_20 = 0;
        D_801217D0[5].unk_04 = (((DisplayActor44 *)D_8011ECD0)[31].unk_26 + x1) - origin_x;
        D_801217D0[5].unk_06 = (((DisplayActor44 *)D_8011ECD0)[31].unk_2A + y1) - origin_y;
        D_800E6280.unk_10A5[5] = 0x8B;
    }
    func_80063520(arg4);
    func_800634FC(arg4);
}

void func_800634FC(void) {
    if ((u32)D_800B5A64 >= 0xA5) {
        D_800B5A64 = 0;
    }
}

void func_80063520(u32 arg0) {
    switch (arg0) {
    case 0:
        D_801217D0[1].unk_0C = 7;
        D_801217D0[0].unk_0C = 7;
        return;
    case 1:
        D_801217D0[1].unk_0C = 7;
        D_801217D0[1].unk_0E += 0x40;
        D_801217D0[0].unk_0C = 7;
        D_801217D0[0].unk_0E += 0x40;
        return;
    case 2:
        D_801217D0[1].unk_0C = 8;
        D_801217D0[0].unk_0C = 8;
        return;
    case 3:
        D_801217D0[1].unk_0C = 8;
        D_801217D0[1].unk_0E += 0x40;
        D_801217D0[0].unk_0C = 8;
        D_801217D0[0].unk_0E += 0x40;
        return;
    case 4:
        D_801217D0[1].unk_0C = 9;
        D_801217D0[0].unk_0C = 9;
        return;
    case 5:
        D_801217D0[1].unk_0C = 9;
        D_801217D0[1].unk_0E += 0x40;
        D_801217D0[0].unk_0C = 9;
        D_801217D0[0].unk_0E += 0x40;
        return;
    }
}

/* FAKE: the u8 view of the s8 field unk_03C gives the original lbu (as in func_80067C28), and the three locals declared
 * before `saved` put three unused slots above its spill slot (frame size 0x38, as TACO func_801438F0). T-17011 */
void func_80063668(void) {
    s32 max;
    s32 level;
    s32 n;
    s32 saved;
    s32 i;
    u8 k;
    s32 face;

    ((DisplayActor44 *)D_8011ECD0)[31].unk_07 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[31].unk_38 &= 0x7FFFFFFF;
    D_801217D0[0].unk_00 &= 0x7FFFFFFF;
    D_801217D0[1].unk_00 &= 0x7FFFFFFF;
    saved = ((DisplayActor44 *)D_8011ECD0)[30].unk_18;
    if (*(u8 *)&D_800E6280.unk_03C != 0) {
        if (func_800460EC() & 4) {
            level = getCDlevel() + 1;
        } else {
            level = 0;
        }
        D_80125C80[D_800E6280.unk_10F8 & 3] = level;
        /* max is never read; the loop is in the original code. */
        max = 0;
        for (i = 0; i < 4; i++) {
            if (max < D_80125C80[i]) {
                max = D_80125C80[i];
            }
        }
        n = 0;
        k = D_800E6280.unk_F5F;
        for (i = 0; i < 3; i++) {
            if (D_800B5A68[k][i] < level) {
                n = i + 1;
            }
        }
        if (n >= 4) {
            n = 3;
        }
        if (D_801217D0[0].unk_0F == 0 || !(D_800E6280.unk_10F8 & 1)) {
            if (level == 0) {
                D_801217D0[0].unk_0F = 0;
            } else if (n == 0) {
                D_801217D0[0].unk_0F = 0;
            } else if (n == 1) {
                D_801217D0[0].unk_0F = 0x10;
            } else if (n == 2) {
                D_801217D0[0].unk_0F = 0x20;
            } else if (n >= 3) {
                D_801217D0[0].unk_0F = 0x30;
            }
        }
        if (D_800E6280.unk_F80 & 0x80) {
            D_800B5A98 = 0;
        }
        if (D_800B5A98 < level) {
            D_800B5A98 = level;
        }
    } else {
        face = ((DisplayActor44 *)D_8011ECD0)[31].unk_18;
        D_801217D0[0].unk_0F = face * 0x10;
    }
    D_801217D0[1].unk_0F = saved << 5;
}

void bustup_speech(void) {
    /* GameState.unk_03C is s8; this function reads it as u8 (lbu) */
    if (*(u8 *)&D_800E6280.unk_03C == 0 && !(D_8011F50E & 1)) {
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

/* nine +0xFC records, reached by byte offset: the word view (value) and the s16 at +2 (cap) */
void parameter_change(s32 arg0, s32 arg1) {
    u8 *p = (u8 *)&D_800E6280 + arg0 * 4;

    *(s32 *)(p + 0xFC) += arg1;
    if (*(s32 *)(p + 0xFC) < 0) {
        *(s32 *)(p + 0xFC) = 0;
    }
    if (*(s16 *)(p + 0xFE) >= 0x3E8) {
        *(s16 *)(p + 0xFE) = 0x3E7;
    }
}

void parameter_disp_switch(s32 arg0) {
    s32 i;

    if (arg0 == 1) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 28; i++) D_8011ECD0[0x883 + i * 0x44] |= 0x80;
    } else {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 28; i++) D_8011ECD0[0x883 + i * 0x44] &= 0x7F;
    }
}

void parameter_show_init(void) {
    s32 i;
    s32 value;
    u8 *snapshot;
    s32 digit;

    /* FAKE: digit and x stores share a source line for as1 scheduling. T-10890 */
    LoadSquare(0x120U, 0x1E0U, 0x10U, 5U, &D_800C6220);
    for (i = 0; i != 28; i++) {
        func_80048F64(i + 32);
        ((DisplayActor44 *)D_8011ECD0)[i + 32].unk_01 = 11;
        ((DisplayActor44 *)D_8011ECD0)[i + 32].unk_02 = 0x40;
        ((DisplayActor44 *)D_8011ECD0)[i + 32].unk_38 = 0x40000000;
        ((DisplayActor44 *)D_8011ECD0)[i + 32].unk_03 = 0x84;
        ((DisplayActor44 *)D_8011ECD0)[i + 32].unk_0C = sprite_frame_table;
        ((DisplayActor44 *)D_8011ECD0)[i + 32].unk_10 = D_800C9A14;
        ((DisplayActor44 *)D_8011ECD0)[i + 32].unk_34 = D_800C9730;
        ((DisplayActor44 *)D_8011ECD0)[i + 32].unk_14 = D_800C9A54;
        ((DisplayActor44 *)D_8011ECD0)[i + 32].unk_04 = 8;
        ((DisplayActor44 *)D_8011ECD0)[i + 32].unk_05 = 1;
    }
    value = D_800E6280.unk_0FC.unk_02;
    digit = func_80046590((s16)value, 3); ((DisplayActor44 *)D_8011ECD0)[32].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[32].unk_26 = -0x49;
    ((DisplayActor44 *)D_8011ECD0)[32].unk_2A = -0x58;
    value = D_800E6280.unk_0FC.unk_02;
    digit = func_80046590((s16)value, 2); ((DisplayActor44 *)D_8011ECD0)[33].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[33].unk_26 = -0x41;
    ((DisplayActor44 *)D_8011ECD0)[33].unk_2A = -0x58;
    value = D_800E6280.unk_0FC.unk_02;
    digit = func_80046590((s16)value, 1); ((DisplayActor44 *)D_8011ECD0)[34].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[34].unk_26 = -0x39;
    ((DisplayActor44 *)D_8011ECD0)[34].unk_2A = -0x58;
    if ((*(s16 *)&D_800E6280.unk_120[0x5A]) < D_800E6280.unk_0FC.unk_02) {
        ((DisplayActor44 *)D_8011ECD0)[32].unk_43 = ((DisplayActor44 *)D_8011ECD0)[33].unk_43 = ((DisplayActor44 *)D_8011ECD0)[34].unk_43 = 0x43;
        ((DisplayActor44 *)D_8011ECD0)[32].unk_2E = ((DisplayActor44 *)D_8011ECD0)[33].unk_2E = ((DisplayActor44 *)D_8011ECD0)[34].unk_2E = 1;
    } else if (D_800E6280.unk_0FC.unk_02 < (*(s16 *)&D_800E6280.unk_120[0x5A])) {
        ((DisplayActor44 *)D_8011ECD0)[32].unk_43 = ((DisplayActor44 *)D_8011ECD0)[33].unk_43 = ((DisplayActor44 *)D_8011ECD0)[34].unk_43 = 0x44;
        ((DisplayActor44 *)D_8011ECD0)[32].unk_2E = ((DisplayActor44 *)D_8011ECD0)[33].unk_2E = ((DisplayActor44 *)D_8011ECD0)[34].unk_2E = 1;
    }
    value = D_800E6280.unk_100.unk_02;
    digit = func_80046590((s16)value, 3); ((DisplayActor44 *)D_8011ECD0)[35].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[35].unk_26 = 0xF;
    ((DisplayActor44 *)D_8011ECD0)[35].unk_2A = -0x58;
    value = D_800E6280.unk_100.unk_02;
    digit = func_80046590((s16)value, 2); ((DisplayActor44 *)D_8011ECD0)[36].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[36].unk_26 = 0x17;
    ((DisplayActor44 *)D_8011ECD0)[36].unk_2A = -0x58;
    value = D_800E6280.unk_100.unk_02;
    digit = func_80046590((s16)value, 1); ((DisplayActor44 *)D_8011ECD0)[37].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[37].unk_26 = 0x1F;
    ((DisplayActor44 *)D_8011ECD0)[37].unk_2A = -0x58;
    if ((*(s16 *)&D_800E6280.unk_120[0x5E]) < D_800E6280.unk_100.unk_02) {
        ((DisplayActor44 *)D_8011ECD0)[35].unk_43 = ((DisplayActor44 *)D_8011ECD0)[36].unk_43 = ((DisplayActor44 *)D_8011ECD0)[37].unk_43 = 0x43;
        ((DisplayActor44 *)D_8011ECD0)[35].unk_2E = ((DisplayActor44 *)D_8011ECD0)[36].unk_2E = ((DisplayActor44 *)D_8011ECD0)[37].unk_2E = 1;
    } else if (D_800E6280.unk_100.unk_02 < (*(s16 *)&D_800E6280.unk_120[0x5E])) {
        ((DisplayActor44 *)D_8011ECD0)[35].unk_43 = ((DisplayActor44 *)D_8011ECD0)[36].unk_43 = ((DisplayActor44 *)D_8011ECD0)[37].unk_43 = 0x44;
        ((DisplayActor44 *)D_8011ECD0)[35].unk_2E = ((DisplayActor44 *)D_8011ECD0)[36].unk_2E = ((DisplayActor44 *)D_8011ECD0)[37].unk_2E = 1;
    }
    value = D_800E6280.unk_104.unk_02;
    digit = func_80046590((s16)value, 3); ((DisplayActor44 *)D_8011ECD0)[38].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[38].unk_26 = 0x67;
    ((DisplayActor44 *)D_8011ECD0)[38].unk_2A = -0x58;
    value = D_800E6280.unk_104.unk_02;
    digit = func_80046590((s16)value, 2); ((DisplayActor44 *)D_8011ECD0)[39].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[39].unk_26 = 0x6F;
    ((DisplayActor44 *)D_8011ECD0)[39].unk_2A = -0x58;
    value = D_800E6280.unk_104.unk_02;
    digit = func_80046590((s16)value, 1); ((DisplayActor44 *)D_8011ECD0)[40].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[40].unk_26 = 0x77;
    ((DisplayActor44 *)D_8011ECD0)[40].unk_2A = -0x58;
    if ((*(s16 *)&D_800E6280.unk_120[0x62]) < D_800E6280.unk_104.unk_02) {
        ((DisplayActor44 *)D_8011ECD0)[38].unk_43 = ((DisplayActor44 *)D_8011ECD0)[39].unk_43 = ((DisplayActor44 *)D_8011ECD0)[40].unk_43 = 0x43;
        ((DisplayActor44 *)D_8011ECD0)[38].unk_2E = ((DisplayActor44 *)D_8011ECD0)[39].unk_2E = ((DisplayActor44 *)D_8011ECD0)[40].unk_2E = 1;
    } else if (D_800E6280.unk_104.unk_02 < (*(s16 *)&D_800E6280.unk_120[0x62])) {
        ((DisplayActor44 *)D_8011ECD0)[38].unk_43 = ((DisplayActor44 *)D_8011ECD0)[39].unk_43 = ((DisplayActor44 *)D_8011ECD0)[40].unk_43 = 0x44;
        ((DisplayActor44 *)D_8011ECD0)[38].unk_2E = ((DisplayActor44 *)D_8011ECD0)[39].unk_2E = ((DisplayActor44 *)D_8011ECD0)[40].unk_2E = 1;
    }
    value = D_800E6280.unk_108.unk_02;
    digit = func_80046590((s16)value, 3); ((DisplayActor44 *)D_8011ECD0)[41].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[41].unk_26 = -0x49;
    ((DisplayActor44 *)D_8011ECD0)[41].unk_2A = -0x4A;
    value = D_800E6280.unk_108.unk_02;
    digit = func_80046590((s16)value, 2); ((DisplayActor44 *)D_8011ECD0)[42].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[42].unk_26 = -0x41;
    ((DisplayActor44 *)D_8011ECD0)[42].unk_2A = -0x4A;
    value = D_800E6280.unk_108.unk_02;
    digit = func_80046590((s16)value, 1); ((DisplayActor44 *)D_8011ECD0)[43].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[43].unk_26 = -0x39;
    ((DisplayActor44 *)D_8011ECD0)[43].unk_2A = -0x4A;
    if ((*(s16 *)&D_800E6280.unk_120[0x66]) < D_800E6280.unk_108.unk_02) {
        ((DisplayActor44 *)D_8011ECD0)[41].unk_43 = ((DisplayActor44 *)D_8011ECD0)[42].unk_43 = ((DisplayActor44 *)D_8011ECD0)[43].unk_43 = 0x43;
        ((DisplayActor44 *)D_8011ECD0)[41].unk_2E = ((DisplayActor44 *)D_8011ECD0)[42].unk_2E = ((DisplayActor44 *)D_8011ECD0)[43].unk_2E = 1;
    } else if (D_800E6280.unk_108.unk_02 < (*(s16 *)&D_800E6280.unk_120[0x66])) {
        ((DisplayActor44 *)D_8011ECD0)[41].unk_43 = ((DisplayActor44 *)D_8011ECD0)[42].unk_43 = ((DisplayActor44 *)D_8011ECD0)[43].unk_43 = 0x44;
        ((DisplayActor44 *)D_8011ECD0)[41].unk_2E = ((DisplayActor44 *)D_8011ECD0)[42].unk_2E = ((DisplayActor44 *)D_8011ECD0)[43].unk_2E = 1;
    }
    value = D_800E6280.unk_10C.unk_02;
    digit = func_80046590((s16)value, 3); ((DisplayActor44 *)D_8011ECD0)[44].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[44].unk_26 = 0xF;
    ((DisplayActor44 *)D_8011ECD0)[44].unk_2A = -0x4A;
    value = D_800E6280.unk_10C.unk_02;
    digit = func_80046590((s16)value, 2); ((DisplayActor44 *)D_8011ECD0)[45].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[45].unk_26 = 0x17;
    ((DisplayActor44 *)D_8011ECD0)[45].unk_2A = -0x4A;
    value = D_800E6280.unk_10C.unk_02;
    digit = func_80046590((s16)value, 1); ((DisplayActor44 *)D_8011ECD0)[46].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[46].unk_26 = 0x1F;
    ((DisplayActor44 *)D_8011ECD0)[46].unk_2A = -0x4A;
    if ((*(s16 *)&D_800E6280.unk_120[0x6A]) < D_800E6280.unk_10C.unk_02) {
        ((DisplayActor44 *)D_8011ECD0)[44].unk_43 = ((DisplayActor44 *)D_8011ECD0)[45].unk_43 = ((DisplayActor44 *)D_8011ECD0)[46].unk_43 = 0x43;
        ((DisplayActor44 *)D_8011ECD0)[44].unk_2E = ((DisplayActor44 *)D_8011ECD0)[45].unk_2E = ((DisplayActor44 *)D_8011ECD0)[46].unk_2E = 1;
    } else if (D_800E6280.unk_10C.unk_02 < (*(s16 *)&D_800E6280.unk_120[0x6A])) {
        ((DisplayActor44 *)D_8011ECD0)[44].unk_43 = ((DisplayActor44 *)D_8011ECD0)[45].unk_43 = ((DisplayActor44 *)D_8011ECD0)[46].unk_43 = 0x44;
        ((DisplayActor44 *)D_8011ECD0)[44].unk_2E = ((DisplayActor44 *)D_8011ECD0)[45].unk_2E = ((DisplayActor44 *)D_8011ECD0)[46].unk_2E = 1;
    }
    value = D_800E6280.unk_110.unk_02;
    digit = func_80046590((s16)value, 3); ((DisplayActor44 *)D_8011ECD0)[47].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[47].unk_26 = 0x67;
    ((DisplayActor44 *)D_8011ECD0)[47].unk_2A = -0x4A;
    value = D_800E6280.unk_110.unk_02;
    digit = func_80046590((s16)value, 2); ((DisplayActor44 *)D_8011ECD0)[48].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[48].unk_26 = 0x6F;
    ((DisplayActor44 *)D_8011ECD0)[48].unk_2A = -0x4A;
    value = D_800E6280.unk_110.unk_02;
    digit = func_80046590((s16)value, 1); ((DisplayActor44 *)D_8011ECD0)[49].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[49].unk_26 = 0x77;
    ((DisplayActor44 *)D_8011ECD0)[49].unk_2A = -0x4A;
    if ((*(s16 *)&D_800E6280.unk_120[0x6E]) < D_800E6280.unk_110.unk_02) {
        ((DisplayActor44 *)D_8011ECD0)[47].unk_43 = ((DisplayActor44 *)D_8011ECD0)[48].unk_43 = ((DisplayActor44 *)D_8011ECD0)[49].unk_43 = 0x43;
        ((DisplayActor44 *)D_8011ECD0)[47].unk_2E = ((DisplayActor44 *)D_8011ECD0)[48].unk_2E = ((DisplayActor44 *)D_8011ECD0)[49].unk_2E = 1;
    } else if (D_800E6280.unk_110.unk_02 < (*(s16 *)&D_800E6280.unk_120[0x6E])) {
        ((DisplayActor44 *)D_8011ECD0)[47].unk_43 = ((DisplayActor44 *)D_8011ECD0)[48].unk_43 = ((DisplayActor44 *)D_8011ECD0)[49].unk_43 = 0x44;
        ((DisplayActor44 *)D_8011ECD0)[47].unk_2E = ((DisplayActor44 *)D_8011ECD0)[48].unk_2E = ((DisplayActor44 *)D_8011ECD0)[49].unk_2E = 1;
    }
    value = D_800E6280.unk_114.unk_02;
    digit = func_80046590((s16)value, 3); ((DisplayActor44 *)D_8011ECD0)[50].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[50].unk_26 = -0x49;
    ((DisplayActor44 *)D_8011ECD0)[50].unk_2A = -0x3C;
    value = D_800E6280.unk_114.unk_02;
    digit = func_80046590((s16)value, 2); ((DisplayActor44 *)D_8011ECD0)[51].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[51].unk_26 = -0x41;
    ((DisplayActor44 *)D_8011ECD0)[51].unk_2A = -0x3C;
    value = D_800E6280.unk_114.unk_02;
    digit = func_80046590((s16)value, 1); ((DisplayActor44 *)D_8011ECD0)[52].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[52].unk_26 = -0x39;
    ((DisplayActor44 *)D_8011ECD0)[52].unk_2A = -0x3C;
    if ((*(s16 *)&D_800E6280.unk_120[0x72]) < D_800E6280.unk_114.unk_02) {
        ((DisplayActor44 *)D_8011ECD0)[50].unk_43 = ((DisplayActor44 *)D_8011ECD0)[51].unk_43 = ((DisplayActor44 *)D_8011ECD0)[52].unk_43 = 0x43;
        ((DisplayActor44 *)D_8011ECD0)[50].unk_2E = ((DisplayActor44 *)D_8011ECD0)[51].unk_2E = ((DisplayActor44 *)D_8011ECD0)[52].unk_2E = 1;
    } else if (D_800E6280.unk_114.unk_02 < (*(s16 *)&D_800E6280.unk_120[0x72])) {
        ((DisplayActor44 *)D_8011ECD0)[50].unk_43 = ((DisplayActor44 *)D_8011ECD0)[51].unk_43 = ((DisplayActor44 *)D_8011ECD0)[52].unk_43 = 0x44;
        ((DisplayActor44 *)D_8011ECD0)[50].unk_2E = ((DisplayActor44 *)D_8011ECD0)[51].unk_2E = ((DisplayActor44 *)D_8011ECD0)[52].unk_2E = 1;
    }
    value = D_800E6280.unk_118.unk_02;
    digit = func_80046590((s16)value, 3); ((DisplayActor44 *)D_8011ECD0)[53].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[53].unk_26 = 0xF;
    ((DisplayActor44 *)D_8011ECD0)[53].unk_2A = -0x3C;
    value = D_800E6280.unk_118.unk_02;
    digit = func_80046590((s16)value, 2); ((DisplayActor44 *)D_8011ECD0)[54].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[54].unk_26 = 0x17;
    ((DisplayActor44 *)D_8011ECD0)[54].unk_2A = -0x3C;
    value = D_800E6280.unk_118.unk_02;
    digit = func_80046590((s16)value, 1); ((DisplayActor44 *)D_8011ECD0)[55].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[55].unk_26 = 0x1F;
    ((DisplayActor44 *)D_8011ECD0)[55].unk_2A = -0x3C;
    if ((*(s16 *)&D_800E6280.unk_120[0x76]) < D_800E6280.unk_118.unk_02) {
        ((DisplayActor44 *)D_8011ECD0)[53].unk_43 = ((DisplayActor44 *)D_8011ECD0)[54].unk_43 = ((DisplayActor44 *)D_8011ECD0)[55].unk_43 = 0x43;
        ((DisplayActor44 *)D_8011ECD0)[53].unk_2E = ((DisplayActor44 *)D_8011ECD0)[54].unk_2E = ((DisplayActor44 *)D_8011ECD0)[55].unk_2E = 1;
    } else if (D_800E6280.unk_118.unk_02 < (*(s16 *)&D_800E6280.unk_120[0x76])) {
        ((DisplayActor44 *)D_8011ECD0)[53].unk_43 = ((DisplayActor44 *)D_8011ECD0)[54].unk_43 = ((DisplayActor44 *)D_8011ECD0)[55].unk_43 = 0x44;
        ((DisplayActor44 *)D_8011ECD0)[53].unk_2E = ((DisplayActor44 *)D_8011ECD0)[54].unk_2E = ((DisplayActor44 *)D_8011ECD0)[55].unk_2E = 1;
    }
    value = D_800E6280.unk_11C.unk_02;
    digit = func_80046590((s16)value, 3); ((DisplayActor44 *)D_8011ECD0)[56].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[56].unk_26 = 0x67;
    ((DisplayActor44 *)D_8011ECD0)[56].unk_2A = -0x3C;
    value = D_800E6280.unk_11C.unk_02;
    digit = func_80046590((s16)value, 2); ((DisplayActor44 *)D_8011ECD0)[57].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[57].unk_26 = 0x6F;
    ((DisplayActor44 *)D_8011ECD0)[57].unk_2A = -0x3C;
    value = D_800E6280.unk_11C.unk_02;
    digit = func_80046590((s16)value, 1); ((DisplayActor44 *)D_8011ECD0)[58].unk_18 = digit + 0x44; ((DisplayActor44 *)D_8011ECD0)[58].unk_26 = 0x77;
    ((DisplayActor44 *)D_8011ECD0)[58].unk_2A = -0x3C;
    if ((*(s16 *)&D_800E6280.unk_120[0x7A]) < D_800E6280.unk_11C.unk_02) {
        ((DisplayActor44 *)D_8011ECD0)[56].unk_43 = ((DisplayActor44 *)D_8011ECD0)[57].unk_43 = ((DisplayActor44 *)D_8011ECD0)[58].unk_43 = 0x43;
        ((DisplayActor44 *)D_8011ECD0)[56].unk_2E = ((DisplayActor44 *)D_8011ECD0)[57].unk_2E = ((DisplayActor44 *)D_8011ECD0)[58].unk_2E = 1;
    } else if (D_800E6280.unk_11C.unk_02 < (*(s16 *)&D_800E6280.unk_120[0x7A])) {
        ((DisplayActor44 *)D_8011ECD0)[56].unk_43 = ((DisplayActor44 *)D_8011ECD0)[57].unk_43 = ((DisplayActor44 *)D_8011ECD0)[58].unk_43 = 0x44;
        ((DisplayActor44 *)D_8011ECD0)[56].unk_2E = ((DisplayActor44 *)D_8011ECD0)[57].unk_2E = ((DisplayActor44 *)D_8011ECD0)[58].unk_2E = 1;
    }
    ((DisplayActor44 *)D_8011ECD0)[59].unk_18 = 0x3D; ((DisplayActor44 *)D_8011ECD0)[59].unk_26 = 0;
    ((DisplayActor44 *)D_8011ECD0)[59].unk_2A = -0x48;
    snapshot = &D_800E6280.unk_120[0x20];
    memcpy(snapshot, D_800E6280.unk_0C4, 0x7C);
}


void parameter_show(void) {
    s32 i;
    s32 d;
    s32 step;
    DisplayActor44 *tab;

    if ((u8)D_800E6280.unk_F6E != 0) {
        step = 4;
    } else {
        step = 8;
    }
    tab = (DisplayActor44 *)D_8011ECD0;
    if (tab[32].unk_03 & 0x80) {
        func_80049A40(-0x84, -0x61, 0x108, 0x2A, 0xC, 0x30E00, 0);
        dtd_on(0xC);
        for (i = 32; i < 56; i++) {
            if (tab[i].unk_2E == 1) {
                d = 0x1000 / step;
                tab[i].unk_08 += d;
                tab[i].unk_1A -= d;
                if (d >= tab[i].unk_1A) {
                    tab[i].unk_2E = 2;
                }
            } else if (tab[i].unk_2E == 2) {
                d = 0x1000 / step;
                tab[i].unk_08 -= d;
                tab[i].unk_1A += d;
                if (tab[i].unk_1A >= 0x1000) {
                    tab[i].unk_1A = 0x1000;
                    tab[i].unk_2E = 0;
                    tab[i].unk_08 = 0;
                }
            }
        }
        for (i = 56; i < 59; i++) {
            if (tab[i].unk_2E == 1) {
                d = 0x1000 / step;
                *(s16 *)tab[i].pad_1C -= d;
                if (d >= *(s16 *)tab[i].pad_1C) {
                    tab[i].unk_2E = 2;
                }
            } else if (tab[i].unk_2E == 2) {
                *(s16 *)tab[i].pad_1C += 0x1000 / step;
                if (*(s16 *)tab[i].pad_1C >= 0x1000) {
                    *(s16 *)tab[i].pad_1C = 0x1000;
                    tab[i].unk_2E = 0;
                }
            }
        }

    }
}

void hizuke_disp_switch(s32 arg0) {
    s32 i;

    if (arg0 == 1) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 8; i++) D_8011ECD0[0x443 + i * 0x44] |= 0x80;
    } else {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 8; i++) D_8011ECD0[0x443 + i * 0x44] &= 0x7F;
    }
}

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
    D_8011ED20 = sprite_frame_table;
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

void icon_disp_switch(s32 arg0) {
    u32 i;
    u8 *p;
    GameState *g;

    g = &D_800E6280; /* keeps the base address in a register before the branch (T-9360) */
    if (arg0 != 0) {
        i = 0;
        if ((u8)D_800E6280.unk_F6D != 0) {
            p = D_8011ECD0;
            do {
                p[0x1543] |= 0x80;
                p[0x1103] |= 0x80;
                i++;
                p += 0x44;
            } while (i < (u8)g->unk_F6D);
        }
    } else {
        i = 0;
        if ((u8)D_800E6280.unk_F6D != 0) {
            p = D_8011ECD0;
            do {
                p[0x1543] &= 0x7F;
                p[0x1103] &= 0x7F;
                i++;
                p += 0x44;
            } while (i < (u8)g->unk_F6D);
        }
    }
}

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

/* NON_MATCHING: T-17011, 20 differing lines; the second loop schedules the ucode's last stores and the D_800B5B24
 * write-back differently (v0/v1 reloads after the call, store inside the loop instead of before it) */
#ifdef NON_MATCHING
void func_8006612C(u8 *arg0) {
    DisplayActor44 *tab;
    u8 *p;
    s32 i;

    tab = (DisplayActor44 *)D_8011ECD0;
    D_80125CA0 = -0x40;
    for (i = 0; i < D_800B5B24 + 2; i++) {
        func_80048F64(i + 3);
    }
    set_kanji_string(-0xCE, -0x98, 0U, arg0, 0);
    D_800B5B24 = strlen(arg0) / 2;
    for (i = 0; i < D_800B5B24 + 2; i++) {
        func_80048F64(i + 3);
        tab[i + 3].unk_01 = 2;
        tab[i + 3].unk_02 = 0x40;
        tab[i + 3].unk_38 = 0x40000000;
        tab[i + 3].unk_03 = 0xA4;
        tab[i + 3].unk_0C = sprite_frame_table;
        tab[i + 3].unk_10 = D_800C9A14;
        tab[i + 3].unk_34 = D_800C9730;
        tab[i + 3].unk_26 = (D_800B5B24 - i) * 0xE + D_80125CA0 - 0x87;
        tab[i + 3].unk_2A = D_80125CA0 - 0x51;
        tab[i + 3].unk_14 = D_800C9A54;
        tab[i + 3].unk_04 = 8;
        tab[i + 3].unk_05 = 1;
        tab[i + 3].unk_18 = 0x72;
    }
    p = D_8011ECD0 + D_800B5B24 * 0x44;
    ((DisplayActor44 *)(p + 0x110))->unk_18 = 0x71;
    ((DisplayActor44 *)(p + 0x110))->unk_26 += 2;
    tab[3].unk_18 = 0x73;
    tab[3].unk_26 -= 2;
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006612C);
#endif

void func_80066334(void) {
    s32 x;
    s32 y;
    /* FAKE: paired work-field writes share a line for as1 scheduling. T-10890 */
    Entry8 *work;
    s32 i;

    if (((DisplayActor44 *)D_8011ECD0)[3].unk_03 & 0x80) {
        work = (Entry8 *)get_k_work(0);
        if (D_80125CA0 < 0) {
            D_80125CA0++;
            for (i = 0; i < D_800B5B24 + 2; i++) {
                ((DisplayActor44 *)D_8011ECD0)[i + 3].unk_26 = (D_800B5B24 - i) * 14 + D_80125CA0 - 135;
                ((DisplayActor44 *)D_8011ECD0)[i + 3].unk_2A = D_80125CA0 - 81;
            }
            ((DisplayActor44 *)D_8011ECD0)[3].unk_26 -= 2;
            ((DisplayActor44 *)D_8011ECD0)[D_800B5B24 + 4].unk_26 += 2;
            x = D_80125CA0 - 144;
            y = D_80125CA0 - 90;
            func_80049A40(x, y, D_800B5B24 * 14 + 8, 20, 2, 0x30E00, 0);
            work->unk_00++; work->unk_02++;
            return;
        }
        if (D_80125CA0 < 120) {
            D_80125CA0++;
            x = -144;
            y = -90;
            func_80049A40(x, y, D_800B5B24 * 14 + 8, 20, 2, 0x30E00, 0);
            work->unk_00 = -142; work->unk_02 = -88;
            return;
        }
        if (D_80125CA0 < 184) {
            D_80125CA0++;
            for (i = 0; i < D_800B5B24 + 2; i++) {
                ((DisplayActor44 *)D_8011ECD0)[i + 3].unk_26 = (D_800B5B24 - i) * 14 + (120 - D_80125CA0) - 135;
                ((DisplayActor44 *)D_8011ECD0)[i + 3].unk_2A = (120 - D_80125CA0) - 81;
            }
            ((DisplayActor44 *)D_8011ECD0)[3].unk_26 -= 2;
            ((DisplayActor44 *)D_8011ECD0)[D_800B5B24 + 4].unk_26 += 2;
            x = 120 - D_80125CA0 - 144;
            y = 120 - D_80125CA0 - 90;
            func_80049A40(x, y, D_800B5B24 * 14 + 8, 20, 2, 0x30E00, 0);
            work->unk_00--; work->unk_02--;
            return;
        }
        if (D_800E6280.unk_F80 & 0x10) {
            for (i = 0; i < D_800B5B24 + 2; i++) {
                ((DisplayActor44 *)D_8011ECD0)[i + 3].unk_26 = (D_800B5B24 - i) * 14 - 135;
                ((DisplayActor44 *)D_8011ECD0)[i + 3].unk_2A = -81;
            }
            ((DisplayActor44 *)D_8011ECD0)[3].unk_26 -= 2;
            ((DisplayActor44 *)D_8011ECD0)[D_800B5B24 + 4].unk_26 += 2;
            x = -144;
            y = -90;
            func_80049A40(x, y, D_800B5B24 * 14 + 8, 20, 2, 0x30E00, 0);
            work->unk_00 = -142; work->unk_02 = -88;
            return;
        }
        for (i = 0; i < D_800B5B24 + 2; i++) {
            ((DisplayActor44 *)D_8011ECD0)[i + 3].unk_26 = (D_800B5B24 - i) * 14 - 199;
            ((DisplayActor44 *)D_8011ECD0)[i + 3].unk_2A = -145;
        }
        ((DisplayActor44 *)D_8011ECD0)[3].unk_26 -= 2;
        ((DisplayActor44 *)D_8011ECD0)[D_800B5B24 + 4].unk_26 += 2;
        x = -208;
        y = -154;
        func_80049A40(x, y, D_800B5B24 * 14 + 8, 20, 2, 0x30E00, 0);
        work->unk_00 = -206; work->unk_02 = -152;
    }
}


s32 func_80066A2C(void) {
    switch (D_800E6280.unk_03F) {
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
    switch (D_800E6280.unk_03F) {
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

s32 func_80066ACC(void) {
    s32 ret;

    ret = func_80066A84();
    if (D_800E6280.unk_03F == 9) {
        if (D_800E6280.unk_040 >= 0x18U) {
            ret = -1;
        }
        /* FAKE: empty test of ret makes ret the first register-allocated local ($v1), the global gets $a0. T-4100 */
        if (!ret) {
        }
    }
    if (D_800E6280.unk_03F == 5) {
        if (D_800E6280.unk_040 >= 0x19U) {
            ret = -1;
        }
    }
    return ret;
}

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

void func_80066C08(s32 a) {
    s32 x;
    s32 y;
    s32 old_index;
    s32 next_index;
    u32 blend;
    u8 fade;
    s32 row;
    s32 col;
    SPRT *spr;
    s32 current;
    DisplayColorPair4 *old;
    DisplayColorPair4 *next;
    DisplayColorPair4 *out;
    s32 old_weight;
    s32 next_weight;

    u8 enabled[10][8];
    current = func_80066A2C();
    switch (D_800E6280.unk_0F4.b[1] & 15) {
    case 1:
        if ((D_800E6280.unk_10F8 & 3) == 3) D_800B5BD0--;
        if ((D_800E6280.unk_10F8 & 1) == 1) D_800B5BCC++;
        break;
    case 2:
        if ((D_800E6280.unk_10F8 & 7) == 7) D_800B5BD0--;
        if ((D_800E6280.unk_10F8 & 3) == 3) D_800B5BCC++;
        break;
    case 3:
        if ((D_800E6280.unk_10F8 & 0x15) == 0x15) D_800B5BD0--;
        if ((D_800E6280.unk_10F8 & 7) == 7) D_800B5BCC++;
        break;
    case 4:
        D_800B5BD0 += 16 - (rand() & 31);
        D_800B5BCC += 16 - (rand() & 31);
        break;
    case 5:
        D_800B5BD0 += rsin(D_800E6280.unk_10F8) / 32;
        D_800B5BCC += func_800A0140(D_800E6280.unk_10F8) / 32;
        break;
    case 0:
    default:
        if ((D_800E6280.unk_10F8 & 1) == 1) D_800B5BD0--;
        D_800B5BCC++;
        break;
    }
    for (row = 0; row < 8; row++) {
        for (col = 0; col < 10; col++) {
                enabled[col][row] = 1;
            }
    }
    switch (a) {
    case 0:
        break;
    case 1:
        for (row = 1; row < 6; row++) {
            for (col = 1; col < 9; col++) {
                enabled[col][row] = 0;
            }
        }
        break;
    case 2:
        for (row = 0; row < 6; row++) {
            for (col = 0; col < 10; col++) {
                enabled[col][row] = 0;
            }
        }
        enabled[0][6] = 1;
        enabled[9][6] = 1;
        enabled[0][5] = 1;
        enabled[9][5] = 1;
        break;
    }
    for (row = 0; row < 8; row++) {
        for (col = 0; col < 10; col++) {
            if (enabled[col][row] != 0) {
                spr = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
                func_8009F0A4(spr);
                SetSemiTrans(spr, 0);
                x = col * 32;
                y = (s32)((u32)row * 32 + 240);
                spr->x0 = x;
                spr->y0 = y - D_8011ECA0 * 240;
                spr->u0 = (D_800B5BCC & 31) + 128;
                spr->v0 = (D_800B5BD0 & 31) + 128;
                spr->h = 32;
                spr->w = 32;
                spr->r0 = D_800E6280.unk_03A;
                spr->g0 = D_800E6280.unk_03A;
                spr->b0 = D_800E6280.unk_03A;
                spr->clut = func_8009ED74(0xA0, 0x1E1);
                AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + (u8)D_800B5BD4 * 4, (s32)spr);
            }
        }
    }
    goto_tpage(0x3C0, 0x100, (u8)D_800B5BD4, 0, 0);
    if ((D_800E6280.unk_10F8 & 3) == 3 && (u8)D_800B5BC8 >= 0xEF) {
        fade = ++D_800B5BC8;
        if (fade == 255) {
            current++;
            LoadSquare(0xA0, 0x1E1, 16, 1, D_800B5B28[(current + 3) & 3]);
            return;
        }
        if (fade >= 240) {
            next_weight = fade - 239;
            old_weight = 255 - fade;
            old_index = (current + 3) & 3;
            next_index = (current + 4) & 3;
            old = (DisplayColorPair4 *)D_800B5B28[old_index];
            next = (DisplayColorPair4 *)D_800B5B28[next_index];
            out = (DisplayColorPair4 *)D_800B5BA8;
            for (col = 0; col < 8; col++) {
                blend = old_weight * old->r0 + next_weight * next->r0;
                out->r0 = blend >> 4;
                blend = old_weight * old->g0 + next_weight * next->g0;
                out->g0 = blend >> 4;
                blend = old_weight * old->b0 + next_weight * next->b0;
                out->b0 = blend >> 4;
                blend = old_weight * old->r1 + next_weight * next->r1;
                out->r1 = blend >> 4;
                blend = old_weight * old->g1 + next_weight * next->g1;
                out->g1 = blend >> 4;
                blend = old_weight * old->b1 + next_weight * next->b1;
                out->b1 = blend >> 4;
                old++;
                next++;
                out++;
            }
            LoadSquare(0xA0, 0x1E1, 16, 1, D_800B5BA8);
        }
    }
}

void func_800673B8(void) {
    s32 i;
    s32 j;

    LoadSquare(0x3E0, 0x180, 0x20, 0x80, D_800B374C);
    for (i = 0; i < 32; i++) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (j = 0; j < 4; j++) D_800E6280.unk_163C[i * 4 + j] = 0;
    }
    LoadSquare(0x3E0, 0x1C0, 4, 0x10, D_800E6280.unk_163C);
}

void func_80067438(void) {
    s32 v;

    func_80044750(5);
    func_80044750(0x71);
    v = func_80066A2C() & 3;
    func_80044890(0, 0xBF98, 0xBF79, D_800B5BE8[v], D_800B5BF8[v], D_800B5BD8[v]);
}
