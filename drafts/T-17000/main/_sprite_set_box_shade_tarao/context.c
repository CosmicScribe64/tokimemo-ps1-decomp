#define MAIN_API_OVERRIDE_func_80049FF0 /* Transition calls narrow four coordinates at the call site. T-10870. */
#define MAIN_API_OVERRIDE_SsVabTransCompleted /* SD_GetCDLevel reads the transfer status; canonical legacy view is void. T-10870. */
#define MAIN_API_OVERRIDE_dialogue_render /* integer result read by k_disp_start; callee narrows K&R arguments, T-10730 */
#define MAIN_API_OVERRIDE_func_80084C98 /* promoted s32 index view avoids call-site narrowing; existing callee performs the s16 conversion, T-9540 */
#define MAIN_API_OVERRIDE_D_80122CD0 /* word-indexed clearing view, canonical scalar retained elsewhere, T-9540 */
#define MAIN_API_OVERRIDE_select_girl_init /* K&R byte parameter is narrowed in the callee. T-10900. */
#include "common.h"
#include "game.h"
#include "psx_types.h"
s32 func_80049FF0(s16 x0, s16 y0, s16 x1, s16 y1, s32 arg4, s32 arg5, s32 arg6);
s32 SsVabTransCompleted();
s32 dialogue_render();
void select_girl_init();
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800C9F60);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800C9FF8);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA048);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA108);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA120);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA124);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA128);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA130);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA134);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA138);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA13C);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA140);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA144);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA148);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA14C);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA160);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA164);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA168);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA16C);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA174);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA17C);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA188);
INCLUDE_RODATA("asm/data/main/8007C030.data", girl_name_buf);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA19C);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA1DC);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA23C);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA248);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA250);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA25C);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA280);
u8 D_800CA2A4[4] = { 0, 1, 2, 0 };
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA2A8);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA2AC);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA2B0);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA2BC);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA2C0);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA2C4);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA2C8);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA2F0);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA300);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA318);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA330);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA348);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA360);
INCLUDE_RODATA("asm/data/main/8007C030.data", D_800CA36C);
/* FAKE: typed view of the existing same-address alias keeps the byte argument load in a1 and the index conversion inside the matched callee. T-9540 */
void func_80084C98(s32 idx, u8 val);
/* FAKE: minimum array view inferred from clearing words 5 through 30; full object extent remains unknown. T-9540 */
extern s32 D_80122CD0[];
/* Declared here, not in main_api.h: EVENT src/ovl/EVENT/80103B60.c declares the same address as
 * a function table (FnTbl55); moving it needs a MAIN_API_OVERRIDE there. T-11590 */
extern s32 D_80122D48;

void SD_GetCDLevel(CdLevelWork *arg0) {
    s32 shift;
    u32 left;
    u32 right;

    arg0->unk_00 = 0U;
    arg0->unk_02 = 0U;
    if (((u32) D_80125D14 >> 0x18) == 0) {
        if (!(D_80125D10.unk_00 & 0x4000) && (func_80094B80(0) == 1)) {
            if (SsVabTransCompleted(0) == 1) {
                func_80094C60(0);
                func_80094C90((s32) D_80126080, 0x200);
                func_80094B80(1);
                func_80094C60(0x400);
                func_80094C90((s32) &D_80126080[0x200], 0x200);
                func_80094B80(1);
                switch (arg0->unk_08 & 0xF) {
                case 1:
                    SD_DetectCDPeak(&arg0->unk_00, &arg0->unk_02, 0, (s32) arg0->unk_06);
                    break;
                case 2:
                    SD_CalcCDAve(&arg0->unk_00, &arg0->unk_02, 0, (s32) arg0->unk_06);
                    break;
                }
                left = arg0->unk_00;
                right = arg0->unk_02;
                if ((arg0->unk_08 & 0xF0) == 0x20) {
                    left = ((u32) (left * left) >> 0xF);
                    right = ((u32) (right * right) >> 0xF);
                }
                switch (arg0->unk_08 & 0xF00) {
                case 0x100:
                    if (left < 0x3333U) {
                        left *= 4;
                    } else {
                        left = (left >> 2) + 0xC000;
                    }
                    if (right < 0x3333U) {
                        right *= 4;
                    } else {
                        right = (right >> 2) + 0xC000;
                    }
                    break;
                case 0x400:
                    if (left < 0x9999U) {
                        left = ((u32) (left * 4) / 6U);
                    } else {
                        left = ((u32) (left * 6) >> 2) + 0xFFFF7FFF;
                    }
                    if (right < 0x9999U) {
                        right = ((u32) (right * 4) / 6U);
                    } else {
                        right = ((u32) (right * 6) >> 2) + 0xFFFF7FFF;
                    }
                    break;
                case 0x800:
                    if (left < 0xB333U) {
                        left = ((u32) (left * 2) / 7U);
                    } else {
                        left = ((u32) (left * 8) / 3U) + 0xFFFE5555;
                    }
                    if (right < 0xB333U) {
                        right = ((u32) (right * 2) / 7U);
                    } else {
                        right = ((u32) (right * 8) / 3U) + 0xFFFE5555;
                    }
                    break;
                }
                shift = 0x10 - arg0->unk_04;
                /* FAKE: explicit output masks reproduce the ugen temporaries; casts alone do not. T-10870. */
                arg0->unk_00 = (left >> shift) & 0xFFFF;
                arg0->unk_02 = (right >> shift) & 0xFFFF;
            }
        }
    }
}

void SD_DetectCDPeak(u16 *l, u16 *r, u16 pos, u16 n) {
    u16 i;
    u16 step;
    s16 a;
    s16 b;

    i = 0;
    if (n != 0) {
        step = 0x100 / n;
        do {
            a = D_80126080[pos];
            b = D_80126080[pos + 0x200];
            if (a & 0x8000) {
                a = ~(a - 1);
            }
            if (b & 0x8000) {
                b = ~(b - 1);
            }
            if (*l < a) {
                *l = a;
            }
            if (*r < b) {
                *r = b;
            }
            pos += step;
            i++;
        } while (i < n);
    }
    *l *= 2;
    *r *= 2;
}

void SD_CalcCDAve(s16 *left, s16 *right, u16 start, u16 count) {
    s32 left_sum;
    s32 right_sum;
    u16 index;
    s16 left_sample;
    s16 right_sample;

    left_sum = 0;
    right_sum = 0;
    for (index = 0; index < count; index++) {
        left_sample = D_80126080[start];
        right_sample = D_80126080[start + 0x200];
        if (left_sample & 0x8000) {
            left_sample = ~(left_sample - 1);
        }
        left_sum += left_sample;
        if (right_sample & 0x8000) {
            right_sample = ~(right_sample - 1);
        }
        right_sum += right_sample;
        start += (u16)(256U / count);
    }
    /* FAKE: explicit masks retain the ANDIs before the halfword stores. T-10020. */
    *left = ((u32)(left_sum * 2) / count) & 0xFFFF;
    *right = ((u32)(right_sum * 2) / count) & 0xFFFF;
}

s16 getCDlevel(void) {
    if (D_80125D10.unk_00 & 0x400) {
        return (u32)(D_80125E60 + D_80125E62) >> 1;
    }
    return 0;
}

void addr_init_weekly_sd(s32 arg0) {
    u16 *p;

    D_80125CA4 = 0x801E0000;
    p = D_800C9F60 + arg0 % 25 * 3;
    D_80125CB0 = p[0] + 0x801E0000;
    D_80125CA8 = p[1] + 0x801E0000;
    D_80125CAC = p[2] + 0x801E0000;
}

void addr_init_holiday_sd(s32 arg0) {
    u16 *p;

    D_80125CA4 = 0x801E0000;
    p = D_800C9FF8 + arg0 % 13 * 3;
    D_80125CB0 = p[0] + 0x801E0000;
    D_80125CA8 = p[1] + 0x801E0000;
    D_80125CAC = p[2] + 0x801E0000;
}

void addr_init_club_sd(s32 arg0) {
    u16 *p;

    D_80125CA4 = 0x801E0000;
    p = D_800CA048 + arg0 % 32 * 3;
    D_80125CB0 = p[0] + 0x801E0000;
    D_80125CA8 = p[1] + 0x801E0000;
    D_80125CAC = p[2] + 0x801E0000;
}

void addr_init_else_sd(s32 arg0) {
    u16 *p;

    D_80125CA4 = 0x801E0000;
    p = D_800CA108 + arg0 % 3 * 3;
    D_80125CB0 = p[0] + 0x801E0000;
    D_80125CA8 = p[1] + 0x801E0000;
    D_80125CAC = p[2] + 0x801E0000;
}

void addr_init_bustup(void) {
    D_800CA120 = 0x80180084;
    D_800CA124 = 0x80180098;
    D_800CA128 = 0x8018009C;
    D_800CA130 = 0x801800AC;
}

void func_8007C784(void) {
    s32 pad; /* FAKE: unused slot above the byte locals, the original frame has it (real source unknown). T-8070 */
    u8 sp2B;
    u8 sp2A;
    u8 sp29;

    if (cd_read_sync_status() & 1) {
        func_8004284C();
        return;
    }
    if ((u32)D_800E6280.unk_1104.w++ >= 0x401) {
        func_800452C4();
        sp29 = D_800B5939;
        sp2A = *(u8 *)&D_800B5948;
        sp2B = D_800B593C;
        dec_bg_reset();
        *(u8 *)&D_800B5948 = sp2A;
        D_800B593C = sp2B;
        D_800B5939 = sp29;
        D_800E6280.unk_110A -= 1;
    }
}

void func_8007C844(void) {
    D_80122CF8 = 1;
    D_800B593C = 0x80;
    func_8004284C();
}

void func_8007C878(void) {
    D_80122CF8 = 0;
    D_800B593C = 0;
    func_8004284C();
}

void func_8007C8A4(void) {
    if (func_8007C8D8() == 1) {
        func_8004284C();
    }
}

s32 func_8007C8D8(void) {
    /* T-10900: inline half-width and right edge retain the original frame. */
    s16 top_y;
    s16 bottom_y;
    s16 left_x;
    s16 outer;
    GameState *g;
    s32 width;

    g = &D_800E6280;
    srand(g->unk_10F8);
    if (g->unk_1104.u == 0) {
        D_80122D3C = g->unk_03F % 7U;
    }
    if (D_80122D3C != 6) {
        D_80122CF8 = 2;
    }
    D_800B593C += 8;
    if ((u8)D_800B593C >= 0x80U) {
        D_800B593C = 0x80;
    }
    switch (D_80122D3C) {
    case 1:
        for (outer = g->unk_1104.u * 5; outer < 0x50; outer++) {
            func_80049FF0(-0xA0, -0x29 - outer, 0xA0, -0x29 - outer, 0, 0, 0xD);
            func_80049FF0(-0xA0, outer - 0x29, 0xA0, outer - 0x29, 0, 0, 0xD);
        }
        break;
    case 2:
        for (outer = g->unk_1104.u; outer < 0x10; outer++) {
            for (left_x = 0; left_x < 10; left_x++) {
                top_y = left_x * 16 + outer - 0x78;
                func_80049FF0(-0xA0, top_y, 0xA0, top_y, 0, 0, 0xD);
            }
        }
        break;
    case 3:
        for (outer = g->unk_1104.u << 5; outer < 0x1E0; outer++) {
            if (outer < 0xA0) {
                func_80049FF0(outer - 0xa0, -0x78, -0xA0, outer - 0x78, 0, 0, 0xD);
            } else if (outer < 0x140) {
                func_80049FF0(outer - 0xa0, -0x78, outer - 0x140, 0x27, 0, 0, 0xD);
            } else {
                func_80049FF0(0xA0, outer - 0x1b8, outer - 0x140, 0x27, 0, 0, 0xD);
            }
        }
        break;
    case 4:
        for (outer = g->unk_1104.u << 6; outer < 0x1E0; outer += 4) {
            if (outer < 0xA0) {
                func_80049FF0(outer - 0xa0, -0x78, -0xA0, outer - 0x78, 0, 0, 0xD);
            } else if (outer < 0x140) {
                func_80049FF0(outer - 0xa0, -0x78, outer - 0x140, 0x27, 0, 0, 0xD);
            } else {
                func_80049FF0(0xA0, outer - 0x1b8, outer - 0x140, 0x27, 0, 0, 0xD);
            }
        }
        for (outer = g->unk_1104.u << 6; outer < 0x1E0; outer += 4) {
            if (outer < 0xA0) {
                func_80049FF0(outer - 0x9f, -0x78, -0xA0, outer - 0x77, 0, 0, 0xD);
            } else if (outer < 0x140) {
                func_80049FF0(outer - 0x9f, -0x78, outer - 0x13f, 0x27, 0, 0, 0xD);
            } else {
                func_80049FF0(0xA0, outer - 0x1b7, outer - 0x13f, 0x27, 0, 0, 0xD);
            }
        }
        if ((g->unk_1104.u << 6) < 0x1E0U) left_x = 0;
        else left_x = (g->unk_1104.u << 6) - 0x1E0;
        for (outer = left_x; outer < 0x1E0; outer += 4) {
            if (outer < 0xA0) {
                func_80049FF0(outer - 0x9e, -0x78, -0xA0, outer - 0x76, 0, 0, 0xD);
            } else if (outer < 0x140) {
                func_80049FF0(outer - 0x9e, -0x78, outer - 0x13e, 0x27, 0, 0, 0xD);
            } else {
                func_80049FF0(0xA0, outer - 0x1b6, outer - 0x13e, 0x27, 0, 0, 0xD);
            }
        }
        for (outer = left_x; outer < 0x1E0; outer += 4) {
            if (outer < 0xA0) {
                func_80049FF0(outer - 0x9d, -0x78, -0xA0, outer - 0x75, 0, 0, 0xD);
            } else if (outer < 0x140) {
                func_80049FF0(outer - 0x9d, -0x78, outer - 0x13d, 0x27, 0, 0, 0xD);
            } else {
                func_80049FF0(0xA0, outer - 0x1b5, outer - 0x13d, 0x27, 0, 0, 0xD);
            }
        }
        break;
    case 5:
        for (outer = g->unk_1104.u * 10; outer < 0xA0; outer++) {
            width = 0xA0 - outer;
            left_x = width - 0xA0;
            top_y = (width / 2) - 0x78;
            func_80049FF0(left_x, top_y, (0x9F - width), top_y, 0, 0, 0xD);
            bottom_y = 0x27 - (width / 2);
            func_80049FF0(left_x, bottom_y, (0x9F - width), bottom_y, 0, 0, 0xD);
            func_80049FF0(left_x, top_y, left_x, bottom_y, 0, 0, 0xD);
            func_80049FF0(0xA0-width, top_y, (0x9F - width), bottom_y, 0, 0, 0xD);
        }
        break;
    case 6:
        if (g->unk_1104.u < 0x10U) set_tarao_bg(g->unk_1104.u >> 1);
        else set_tarao_bg(8);
        break;
    default:
        for (outer = 0; outer < 0xF; outer++) {
            if ((outer << 5) < 0xA0) {
                for (left_x = g->unk_1104.u * 2; left_x < 0x20; left_x++) {
                    func_80049FF0((outer << 5)+left_x-0xA0, -0x78, -0xA0, (outer << 5)+left_x-0x78, 0, 0, 0xD);
                }
            } else if ((outer << 5) < 0x140) {
                for (left_x = g->unk_1104.u * 2; left_x < 0x20; left_x++) {
                    func_80049FF0((outer << 5)+left_x-0xA0, -0x78, (outer << 5)+left_x-0x140, 0x28, 0, 0, 0xD);
                }
            } else {
                for (left_x = g->unk_1104.u * 2; left_x < 0x20; left_x++) {
                    func_80049FF0(0xA0, (outer << 5)+left_x-0x1B8, (outer << 5)+left_x-0x140, 0x28, 0, 0, 0xD);
                }
            }
        }
        break;
    }
    g->unk_1104.u++;
    if (g->unk_1104.u >= 16U) {
        D_80122CF8 = 2;
        return 1;
    }
    return 0;
}

void set_tarao_bg(s32 arg0) {
    s16 row;
    s16 col;
    s32 x;
    s32 y;
    s32 row_y;
    s32 offset;

    for (row = 0; row < 20; row++) {
        row_y = row * 8;
        for (col = 0; col < 16; col += 2) {
            x = col * 10;
            offset = (col % 2) * 8;
            y = row_y + offset;
            set_tarao_sprt(x, y, 10, arg0, x, y, 12, 128, 128, 128);
        }
        for (col = 1; col < 16; col += 2) {
            x = col * 10;
            offset = (col % 2) * 8;
            y = row_y + offset - arg0;
            set_tarao_sprt(x, y, 10, arg0, x, y, 12, 128, 128, 128);
        }
    }
    if (D_800B5939 != 0) {
        goto_tpage(0x140, 0x100, 12, 0, 2);
    } else {
        goto_tpage(0x2C0, 0, 12, 0, 2);
    }
    for (row = 0; row < 20; row++) {
        row_y = row * 8;
        for (col = 16; col < 32; col += 2) {
            x = col * 10;
            offset = (col % 2) * 8;
            y = row_y + offset;
            set_tarao_sprt(x, y, 10, arg0, x - 128, y, 12, 128, 128, 128);
        }
        for (col = 17; col < 32; col += 2) {
            x = col * 10;
            offset = (col % 2) * 8;
            y = row_y + offset - arg0;
            set_tarao_sprt(x, y, 10, arg0, x - 128, y, 12, 128, 128, 128);
        }
    }
    if (D_800B5939 != 0) {
        goto_tpage(0x1E0, 0x100, 12, 0, 2);
    } else {
        goto_tpage(0x360, 0, 12, 0, 2);
    }
}

/* K&R parameters preserve promoted callers; the helper narrows them. T-10870. */
void set_tarao_sprt(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
s16 arg0;
s16 arg1;
s16 arg2;
s16 arg3;
u8 arg4;
u8 arg5;
s16 arg6;
u8 arg7;
u8 arg8;
u8 arg9;
{
    u8 *p;

    p = (u8 *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(p);
    SetSemiTrans(p, 0);
    SetShadeTex(p, 0);
    p[4] = arg7;
    p[5] = arg8;
    p[6] = arg9;
    *(s16 *)(p + 8) = arg0;
    *(s16 *)(p + 0xA) = arg1;
    *(s16 *)(p + 0x10) = arg2;
    *(s16 *)(p + 0x12) = arg3;
    p[0xC] = arg4;
    p[0xD] = arg5;
    if (D_8011ECA0 == 0) {
        *(s16 *)(p + 0xA) += 0xF0;
    }
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg6 * 4, p);
}

void set_tarao_rect(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, u8 arg5, u8 arg6, u8 arg7) {
    u8 *p;

    p = (u8 *)GetWorkBase(0x10, D_8011ECA0);
    func_8009F0F4(p);
    SetSemiTrans(p, 0);
    SetShadeTex(p, 0);
    p[4] = arg5;
    p[5] = arg6;
    p[6] = arg7;
    *(s16 *)(p + 8) = arg0;
    *(s16 *)(p + 0xA) = arg1;
    *(s16 *)(p + 0xC) = arg2;
    *(s16 *)(p + 0xE) = arg3;
    if (D_8011ECA0 == 0) {
        *(s16 *)(p + 0xA) += 0xF0;
    }
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg4 * 4, (s32)p);
}

void normal_date_bg_out(void) {
    if (func_8007D8AC() == 1) {
        func_8004284C();
    }
}

/* T-10900: inline right edge retains the original spill at sp+0x54.
 * FAKE: the redundant word view in case 1 preserves its original raw-state
 * register. A direct member read changes four instructions. */
s32 func_8007D8AC(void) {
    s16 line_y;
    s16 bottom_y;
    s16 inner;
    s16 outer;
    s32 limit;
    s32 coordinate;

    D_800B593C -= 8;
    if ((u8)D_800B593C >= 0x81U) {
        D_800B593C = 0;
    }
    if (D_800E6280.unk_1104.u == 0) {
        D_80122D3C = (u8) D_800E6280.unk_03F % 7U;
    }
    if (D_80122D3C == 6) {
        D_80122CF8 = 0;
    }
    switch (D_80122D3C) {
    case 1:
        limit = (s16)(((GsWord *)&D_800E6280.unk_1104)->u * 5);
        if (limit > 0x50) limit = 0x50;
        for (outer=0; outer<limit; outer++) {
            func_80049FF0(-0xA0, outer-0x78, 0xA0, outer-0x78, 0, 0, 6);
            func_80049FF0(-0xA0, 0x27-outer, 0xA0, 0x27-outer, 0, 0, 6);
        }
        break;
    case 2:
        limit = (s16) D_800E6280.unk_1104.h[0];
        if (limit >= 0x11) {
            limit = 0x10;
        }
        outer = 0;
        if (limit > 0) {
            do {
                inner = 0;
                do {
                line_y = (inner * 0x10 - outer) - 0x69;
                func_80049FF0(-0xA0, line_y, 0xA0, line_y, 0, 0, 6);
                inner += 1;
                } while (inner < 0xA);
                outer += 1;
            } while (outer < limit);
        }
        break;
    case 3:
        limit = (s16)(D_800E6280.unk_1104.u << 5);
        if (limit >= 0x1E1) {
            limit = 0x1E0;
        }
        outer = 0;
        if (limit > 0) {
            do {

                if (outer >= 0x141) {
                    func_80049FF0(((0x1DF - outer) - 0xA0), -0x78, -0xA0, ((0x1DF - outer) - 0x78), 0, 0, 0xD);
                } else if (outer >= 0xA1) {

                    func_80049FF0(((0x1DF - outer) - 0xA0), -0x78, ((0x1DF - outer) - 0x140), 0x27, 0, 0, 0xD);
                } else {

                    func_80049FF0(0xA0, ((0x1DF - outer) - 0x1B8), ((0x1DF - outer) - 0x140), 0x27, 0, 0, 0xD);
                }
                outer += 1;
            } while (outer < limit);
        }
        break;
    case 4:
        limit = (s16)(D_800E6280.unk_1104.u << 6);
        if (limit >= 0x1E1) {
            limit = 0x1E0;
        }
        outer = 0;
        if (limit > 0) {
            do {

                if (outer >= 0x141) {
                    func_80049FF0(((0x1DF - outer) - 0xA0), -0x78, -0xA0, ((0x1DF - outer) - 0x78), 0, 0, 0xD);
                } else if (outer >= 0xA1) {

                    func_80049FF0(((0x1DF - outer) - 0xA0), -0x78, ((0x1DF - outer) - 0x140), 0x27, 0, 0, 0xD);
                } else {

                    func_80049FF0(0xA0, ((0x1DF - outer) - 0x1B8), ((0x1DF - outer) - 0x140), 0x27, 0, 0, 0xD);
                }
                outer += 4;
            } while (outer < limit);
            outer = 0;
        }
        if (limit > 0) {
            do {

                if (outer >= 0x141) {
                    func_80049FF0(((0x1DF - outer) - 0x9F), -0x78, -0xA0, ((0x1DF - outer) - 0x77), 0, 0, 0xD);
                } else if (outer >= 0xA1) {

                    func_80049FF0(((0x1DF - outer) - 0x9F), -0x78, ((0x1DF - outer) - 0x13F), 0x27, 0, 0, 0xD);
                } else {

                    func_80049FF0(0xA0, ((0x1DF - outer) - 0x1B7), ((0x1DF - outer) - 0x13F), 0x27, 0, 0, 0xD);
                }
                outer += 4;
            } while (outer < limit);
            outer = 0;
        }
        limit = (s16)((D_800E6280.unk_1104.u << 6) - 0x1E0);
        if (limit >= 0x1E1) {
            limit = 0x1E0;
        }
        if (limit > 0) {
            do {

                if (outer >= 0x141) {
                    func_80049FF0(((0x1DF - outer) - 0x9E), -0x78, -0xA0, ((0x1DF - outer) - 0x76), 0, 0, 0xD);
                } else if (outer >= 0xA1) {

                    func_80049FF0(((0x1DF - outer) - 0x9E), -0x78, ((0x1DF - outer) - 0x13E), 0x27, 0, 0, 0xD);
                } else {

                    func_80049FF0(0xA0, ((0x1DF - outer) - 0x1B6), ((0x1DF - outer) - 0x13E), 0x27, 0, 0, 0xD);
                }
                outer += 4;
            } while (outer < limit);
            outer = 0;
        }
        if (limit > 0) {
            do {

                if (outer >= 0x141) {
                    func_80049FF0(((0x1DF - outer) - 0x9D), -0x78, -0xA0, ((0x1DF - outer) - 0x75), 0, 0, 0xD);
                } else if (outer >= 0xA1) {

                    func_80049FF0(((0x1DF - outer) - 0x9D), -0x78, ((0x1DF - outer) - 0x13D), 0x27, 0, 0, 0xD);
                } else {

                    func_80049FF0(0xA0, ((0x1DF - outer) - 0x1B5), ((0x1DF - outer) - 0x13D), 0x27, 0, 0, 0xD);
                }
                outer += 4;
            } while (outer < limit);
        }
        break;
    case 5:
        limit = (s16)(D_800E6280.unk_1104.u * 0xA);
        if (limit >= 0xA1) {
            limit = 0xA0;
        }
        outer = 0;
        if (limit > 0) {
            do {
                inner = outer - 0xA0;
                coordinate = outer / 2;
                line_y = coordinate - 0x78;
                func_80049FF0(inner, line_y, (0x9F - outer), line_y, 0, 0, 0xD);
                bottom_y = 0x27 - coordinate;
                func_80049FF0(inner, bottom_y, (0x9F - outer), bottom_y, 0, 0, 0xD);
                func_80049FF0(inner, line_y, inner, bottom_y, 0, 0, 0xD);
                func_80049FF0((0xA0 - outer), line_y, (0x9F - outer), bottom_y, 0, 0, 0xD);
                outer += 1;
            } while (outer < limit);
        }
        break;
    case 6:
        if ((u32) D_800E6280.unk_1104.u < 0x10U) {
            set_tarao_bg(8 - ((u32) D_800E6280.unk_1104.u >> 1));
        } else {
            set_tarao_bg(0);
        }
        break;
    default:
        limit = (s16)(D_800E6280.unk_1104.u * 2);
        if (limit >= 0x21) {
            limit = 0x20;
        }
        for (outer = 0; outer < 15; outer++) {
            inner = 0;
            if ((outer << 5) < 0xA0) {
                coordinate = (outer << 5) + 0x1F;
                if (limit > 0) {
                    do {

                        func_80049FF0(((coordinate - inner) - 0xA0), -0x78, -0xA0, ((coordinate - inner) - 0x78), 0, 0, 0xD);
                        inner += 1;
                    } while (inner < limit);
                }
            } else if ((outer << 5) < 0x140) {
                coordinate = (outer << 5) + 0x1F;
                if (limit > 0) {
                    do {

                        func_80049FF0(((coordinate - inner) - 0xA0), -0x78, ((coordinate - inner) - 0x140), 0x28, 0, 0, 0xD);
                        inner += 1;
                    } while (inner < limit);
                }
            } else {
                coordinate = (outer << 5) + 0x1F;
                if (limit > 0) {
                    do {

                        func_80049FF0(0xA0, ((coordinate - inner) - 0x1B8), ((coordinate - inner) - 0x140), 0x28, 0, 0, 0xD);
                        inner += 1;
                    } while (inner < limit);
                }
            }
        }
        break;
    }
    D_800E6280.unk_1104.u++;
    if (D_800E6280.unk_1104.u >= 0x12U) {
        D_80122CF8 = 0;
        return 1;
    }
    return 0;
}

void normal_date_girl_in(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        normal_date_girl_in_init();
        break;
    case 1:
        normal_date_girl_in_main();
        break;
    default:
        func_80046500();
        break;
    }
    func_80057D28(1);
}

s32 normal_date_girl_in_init(void) {
    D_8011ECD0[0x843] = 0;
    D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = 0;
    D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = 0;
    D_800B5940 = 0;
    D_80122CF8 = 0;
    D_80122CF4 = 1;
    D_800E6280.unk_1104.w += 1;
}

s32 normal_date_girl_in_main(void) {
    if (D_800B5940 < 0x80) {
        D_800B5940 += 8;
    } else {
        func_8004284C();
        D_80122CF8 = 1;
        D_801217D0[0].unk_00 &= 0x7FFFFFFF;
        D_801217D0[1].unk_00 &= 0x7FFFFFFF;
        return 0;
    }
    D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = D_800B5940;
    D_8011ECD0[0x843] = D_800B5940;
}

s32 normal_date_girl_suddenin(void) {
    D_80122CF4 = 1;
    D_8011ECD0[0x843] = 0x80;
    D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = 0x80;
    D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = 0x80;
    func_8004284C();
}

void normal_date_girl_out(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        normal_date_girl_out_init();
        break;
    case 1:
        normal_date_girl_out_main();
        break;
    default:
        func_80046500();
        break;
    }
    func_80057D28(1);
}

void normal_date_girl_out_init(void) {
    D_800B5940 = 0x80;
    D_80122CF8 = 0;
    D_800E6280.unk_1104.w = D_800E6280.unk_1104.w + 1;
}

s32 normal_date_girl_out_main(void) {
    if (D_800B5940 >= 8) {
        D_800B5940 -= 8;
    } else {
        D_8011ECD0[0x83F] &= ~0x80;
        D_8011ECD0[0x7FB] &= ~0x80;
        func_8004284C();
        D_801217D0[0].unk_00 |= 0x80000000;
        D_801217D0[1].unk_00 |= 0x80000000;
        D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = 0x80;
        D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = 0x80;
        D_80122CF4 = 0;
        D_80122CF8 = 1;
        return 0;
    }
    D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = D_800B5940;
    D_8011ECD0[0x843] = D_800B5940;
}

s32 normal_date_girl_suddenout(void) {
    D_8011ECD0[0x83F] &= ~0x80;
    D_8011ECD0[0x7FB] &= ~0x80;
    D_801217D0[0].unk_00 |= 0x80000000;
    D_801217D0[1].unk_00 |= 0x80000000;
    D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = 0x80;
    D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = 0x80;
    D_80122CF4 = 0;
    D_80122CF8 = 1;
    func_8004284C();
}

s32 normal_date_bg_fadein(void) {
    D_80122CF8 = 1;
    D_800B593C += 4;
    if (D_800B593C >= 0x80) {
        D_800B593C = 0x80;
    }
    if (D_800B593C >= 0x7D) {
        D_800B593C = 0x80;
        func_8004284C();
    }
}

/* s32 without a return value: the last chain value is then kept in v0 (andi v0,a0,0xff). T-11590 */
s32 normal_date_bggirl_fadein(void) {
    u8 x;

    D_8011ECD0[0x83F] |= 0x80;
    normal_date_bg_fadein();
    x = D_800B593C;
    D_8011ECD0[0x843] = x;
    D_801217D0[0].unk_00 &= 0x7FFFFFFF;
    D_801217D0[1].unk_00 &= 0x7FFFFFFF;
    D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = x;
    D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = x;
    D_80122CF4 = 1;
}

void normal_date_bg_fadeout(void) {
    D_80122CF8 = 1;
    D_800B593C -= 4;
    if (D_800B593C >= 0x81) {
        D_800B593C = 0;
    }
    if (D_800B593C < 4) {
        D_80122CF8 = 0;
        D_800B593C = 0;
        func_8004284C();
    }
}

/* s32 without a return value, as normal_date_bggirl_fadein. T-11590 */
s32 normal_date_bggirl_fadeout(void) {
    u8 x;

    if (D_800B593C < 8) {
        D_80122CF4 = 0;
        D_80122CF8 = 0;
        D_8011ECD0[0x83F] &= ~0x80;
        D_8011ECD0[0x7FB] &= ~0x80;
        D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = 0x80;
        D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = 0x80;
        D_801217D0[0].unk_00 |= 0x80000000;
        D_801217D0[1].unk_00 |= 0x80000000;
    }
    normal_date_bg_fadeout();
    x = D_800B593C;
    D_8011ECD0[0x843] = x;
    D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = x;
    D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = x;
}

void normal_date_move_place(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        normal_date_move_place_init();
        return;
    case 1:
        normal_date_move_place_main();
        return;
    default:
        func_80046500();
        return;
    }
}

void normal_date_move_place_init(void) {
    k_reset(1);
    D_800B5940 = 0x80;
    D_80122CF8 = 0;
    D_800E6280.unk_1104.w = D_800E6280.unk_1104.w + 1;
    func_80058398(1);
}

s32 normal_date_move_place_main(void) {
    if (D_800B5940 >= 8) {
        D_800B5940 -= 8;
    } else {
        func_80058398(1);
        dec_bg_show_switch(1);
        func_8004284C();
        D_801217D0[0].unk_00 |= 0x80000000;
        D_801217D0[1].unk_00 |= 0x80000000;
        D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = 0x80;
        D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = 0x80;
        D_80122CF8 = 1;
        return 0;
    }
    D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = D_800B5940;
    func_80058398(1);
}

void place_init(void) {
    k_reset(0);
    func_8006612C(D_800CA19C);
    func_8004284C();
}

void place_init2(void) {
    k_reset(0);
    func_8006612C(D_800CA1DC);
    func_8004284C();
}

void change_dec_bg(void) {
    dec_bg_show_switch(1);
    func_8004284C();
}

s32 dec_init(void) {
    if ((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000 || (((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000) {
        D_800B5920[2] = 0;
        D_800B5920[3] = 0;
        func_8004284C();
        D_800E6280.unk_110A -= 3;
        return 0;
    }
    func_8005751C(1);
    func_8004284C();
}

void bg_read_sub2(s32 arg0) {
    s32 t;

    D_800B3D60 = 0;
    t = dec_bg_cd_read(arg0, 1);
    if (t == D_800B5939) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    } else if (t == 1 - D_800B5939) {
        func_8004284C();
        func_8004284C();
    }
}

/* FAKE: the chained next-index assignment keeps a0 across the call, and the
 * unsigned comparison keeps the target branch operand order. T-9720. */
void check_k_scroll(void) {
    s32 sp38;
    Entry8 *sp48;
    s16 temp_v1;
    s32 temp_a0;
    s32 temp_v0;
    s32 var_a0;
    s32 var_s0;
    u32 temp_s2;
    Entry8 *temp_v0_2;
    Entry8 *temp_v0_3;

    get_ksys();
    temp_v0 = get_now_k();
    temp_s2 = temp_v0 & 0xFF;
    sp48 = (Entry8 *) get_k_work(temp_v0);
    temp_a0 = (var_a0 = temp_s2 + 1);
    sp38 = temp_a0;
    get_k_work(temp_a0);
    if (sp48->unk_02 >= 0x60) {
        var_a0 = sp38;
        if (D_800CA2BC != 0) {
            var_s0 = 1;
            if (temp_s2 >= 2U) {
                do {
                    temp_v0_2 = (Entry8 *) get_k_work(var_s0);
                    temp_v1 = temp_v0_2->unk_02;
                    if ((u32) temp_v1 == 0x30U) {
                        temp_v0_2->unk_07 = 0;
                    } else {
                        temp_v0_2->unk_02 = (s16) (temp_v1 - 0x10);
                    }
                    var_s0 = (s32) ((s16) (var_s0 + 1));
                }
                while (((u32) var_s0) < temp_s2);
            }
            D_800CA2BC = 0;
            sp48->unk_02 = 0x50;
            sp48->unk_07 = 1;
            var_a0 = sp38;
        }
    }
    temp_v0_3 = (Entry8 *) get_k_work(var_a0);
    if (temp_v0_3->unk_02 >= 0x60) {
        D_800CA2BC = 1;
        temp_v0_3->unk_07 = 0;
    }
}

void wait_sub_sub(s16 arg0) {
    s32 t = D_800E6280.unk_1104.w + 1;
    D_800E6280.unk_1104.w = t;
    if ((u32)t >= (u32)arg0) {
        func_8004284C();
    }
}

/* NON_MATCHING: T-10870, 16 differing lines: two four-word scheduling rotations (li a0,-40) remain at the incremented-Y calls; T-11590 retried same-line, inline +=, casts and split arguments. */
#ifdef NON_MATCHING
s32 dialogue_speak_part(arg0, arg1, arg2, arg3, arg4, arg5, arg6)
u8 *arg0;
u8 arg1;
u8 arg2;
DialogueVoice8 ***arg3;
u16 arg4;
u16 arg5;
u8 arg6;
{
    s16 temp_s1;
    s16 var_v1;
    DialogueWork *dialogue;
    s16 sp286;
    s16 var_a1_2;
    u8 *sp27C;
    u8 *p;
    u8 sp7C[0x200];
    u8 sp74[8];
    u8 sp68[12];
    DialogueBytes16 sp58;
    /* FAKE: observed 0x290-byte frame extent; T-10730. */
    s32 pad[1];

    dialogue = (DialogueWork *)D_80122CD0;
    sp58 = D_800CA36C;
    /* FAKE: line grouping preserves as1 scheduling; T-10730. */
    if (arg1 != 0xFF) { strcpy(sp7C, arg0); }
    dialogue->unk_20 = 0;
    D_800CA2A8 = 0;
    temp_s1 = (dialogue->unk_58 * 0x10) + 0x30;
    /* FAKE: byte view shares the 0xFF constant in v1; T-10730. */
    switch (*(u8 *)&arg1) {
    case 0x7:
        if (((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) && (*(u8 *)&D_800E6280.unk_03C != 0)) {
            if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                func_80046290(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[D_800E6280.unk_F5F]);
                func_80044750(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {
                        return 0;
                    }
                }
            } else {
                D_800CA2AC = 0;
                func_8007B640();
                func_80044750(0x74);
                return 0x64;
            }
            dialogue->unk_20 = 1;
            D_800CA2AC = 0;
        }
        if (sp7C[0] == 0x3E) {
            strncpy(sp74, &sp7C[1], 4);
            sp74[4] = 0;
            sprintf(sp68, "%s ", sp74);
            set_kanji_string(-0x78, temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C[5], 0, "", "", 0);
        } else if (sp7C[0] == 0x2D) {
            strncpy(sp74, &sp7C[1], 6);
            sp74[6] = 0;
            sprintf(sp68, "%s ", sp74);
            set_kanji_string(-0x80, temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C[7], 0, "", "", 0);
        } else if (sp7C[0] == 0x2B) {
            strncpy(sp74, &sp7C[1], 2);
            sp74[2] = 0;
            sprintf(sp68, "%s ", sp74);
            set_kanji_string(-0x70, temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C[3], 0, "", "", 0);
        } else if (sp7C[0] == 0x3C) {
            strncpy(sp74, &sp7C[1], 4);
            sp74[4] = 0;
            sprintf(sp68, "%s ", sp74);
            set_kanji_string(-0x78, temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "（", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C[5], 0, "）", "", 0);
        } else if (strcmp(sp7C, "そして…") == 0) {
            temp_s1 += 0x10;
            dialogue_render(-0x28, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        } else if ((strcmp(sp7C, "ガチャ") == 0) || (strcmp(sp7C, "カチャ") == 0) || (strcmp(sp7C, "ガラッ") == 0) || (strcmp(sp7C, "ごそごそ") == 0)) {
            dialogue_render(-0x80, (s32) temp_s1, 2, (s32) sp7C, 0, "", "", 0);
        } else if ((strcmp(sp7C, "ジロジロ") == 0) || (strcmp(sp7C, "ドン") == 0)) {
            dialogue_render(-0x80, (s32) temp_s1, 1, (s32) sp7C, 0, "", "", 0);
        } else if (strcmp(sp7C, "ピンポーン") == 0) {
            temp_s1 += 0x10;
            dialogue_render(-0x28, (s32) temp_s1, 2, (s32) sp7C, 0, "", "", 0);
        } else {
            dialogue_render(-0x80, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        }
        dialogue->unk_14 = 1;
        break;
    case 0x10:
        dialogue_render(-0x80, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        dialogue->unk_30 = 1;
        break;
    case 0x11:
        dialogue_render(-0x48, (s32) D_800CA2C0, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        dialogue->unk_30 = 1;
        break;
    case 0x9:
        if (sp7C[0] != 0) {
            if (strcmp(sp7C, "ドン") == 0) {
                dialogue_render(-0x80, (s32) temp_s1, 1, (s32) sp7C, 0, "", "", 0);
            }
            dialogue_render(-0x80, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        }
        break;
    case 0x0:
        sp286 = strlen(sp7C);
        if ((sp7C[sp286 - 1] == 0x6E) && (sp7C[sp286 - 2] == 0x6E)) {
            if (dialogue->unk_0C == 0) {
                sp7C[sp286 - 2] = 0;
                sp286 = strlen(D_800CA16C);
                sprintf(sp68, "%s ", D_800CA16C);
                set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
                set_kanji_string(-0x56, temp_s1, arg6, "『 ", 0);
                dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_14 = 1;
            }
        } else {
            sp286 = strlen(D_800CA16C);
            sprintf(sp68, "%s ", D_800CA16C);
            set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "『 ", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0xE:
        sp286 = strlen(D_800CA16C);
        sprintf(sp68, "%s ", D_800CA16C);
        set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
        set_kanji_string(-0x56, temp_s1, arg6, "（ ", 0);
        dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "）", "", 0);
        break;
    case 0x1:
        sp286 = strlen(sp7C);
        if (((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) && ((*(u8 *)&D_800E6280.unk_03C != 0) && ((sp7C[sp286 - 1] != 0x6E) || (sp7C[sp286 - 2] != 0x6E) || (dialogue->unk_0C == 0)))) {
                if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                    func_80046290(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[D_800E6280.unk_F5F]);
                    func_80044750(0x300);
                }
                if (D_800CA2AC < 0x400) {
                    if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                        D_800CA2A8 = 1;
                        func_80081128();
                        if (D_800CA2A8 == 1) {
                            return 0;
                        }
                    }
                } else {
                    D_800CA2AC = 0;
                    func_8007B640();
                    func_80044750(0x74);
                    return 0x64;
                }
                dialogue->unk_20 = 1;
                D_800CA2AC = 0;
        }
        p = &sp7C[sp286];
        if ((p[-1] == 0x6E) && (p[-2] == 0x6E)) {
            if (dialogue->unk_0C == 0) {
                p[-2] = 0U;
                sp286 = strlen(D_800CA188);
                sprintf(sp68, "%s ", D_800CA188);
                set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
                set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
                dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_14 = 1;
            }
        } else {
            sp286 = strlen(D_800CA188);
            sprintf(sp68, "%s ", D_800CA188);
            set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0xF:
        if (((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) && (*(u8 *)&D_800E6280.unk_03C != 0)) {
            if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                func_80046290(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[D_800E6280.unk_F5F]);
                func_80044750(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {
                        return 0;
                    }
                }
            } else {
                D_800CA2AC = 0;
                func_8007B640();
                func_80044750(0x74);
                return 0x64;
            }
            dialogue->unk_20 = 1;
            D_800CA2AC = 0;
        }
        sp286 = strlen(D_800CA188);
        sprintf(sp68, "%s ", D_800CA188);
        set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
        set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
        dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        dialogue->unk_48 = 1;
        break;
    case 0xC:
        D_800CA2C4 = 0;
        sp286 = strlen(sp7C);
        if (((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) && (*(u8 *)&D_800E6280.unk_03C != 0)) {
            if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                func_80046290(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[D_800E6280.unk_F5F]);
                func_80044750(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {
                        return 0;
                    }
                }
            } else {
                D_800CA2AC = 0;
                func_8007B640();
                func_80044750(0x74);
                return 0x64;
            }
            dialogue->unk_20 = 1;
            D_800CA2AC = 0;
        }
        p = &sp7C[sp286];
        if ((p[-1] == 0x6E) && (p[-2] == 0x6E)) {
            if (dialogue->unk_0C == 0) {
                p[-2] = 0U;
                sp286 = strlen(D_800CA188);
                sprintf(sp68, "%s ", D_800CA188);
                set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
                set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
                dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_30 = 1;
            }
        } else {
            sp286 = strlen(D_800CA188);
            sprintf(sp68, "%s ", D_800CA188);
            set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_30 = 1;
        }
        break;
    case 0xD:
        sp286 = strlen(sp7C);
        if (((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) && (*(u8 *)&D_800E6280.unk_03C != 0) && (D_800CA2C4 == 0)) {
            if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                func_80046290(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[D_800E6280.unk_F5F]);
                func_80044750(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {
                        return 0;
                    }
                }
            } else {
                D_800CA2AC = 0;
                func_8007B640();
                func_80044750(0x74);
                return 0x64;
            }
            dialogue->unk_20 = 1;
            D_800CA2AC = 0;
        }
        p = &sp7C[sp286];
        if ((p[-1] == 0x6E) && (p[-2] == 0x6E)) {
            if (dialogue->unk_0C == 0) {
                p[-2] = 0U;
                dialogue_render(-0x48, (s32) D_800CA2C0, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_14 = 1;
            }
        } else {
            set_kanji_string(-0x56, temp_s1, arg6, "　 ", 0);
            dialogue_render(-0x48, (s32) D_800CA2C0, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0x12:
        strlen(sp7C);
        if ((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) { /* FAKE: same-line branches preserve scheduling, T-10730. */ if (*(u8 *)&D_800E6280.unk_03C != 0) {

                if (D_800CA2C4 == 0) {
                    if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                            func_80046290(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[D_800E6280.unk_F5F]);
                        func_80044750(0x300);
                    }
                    if (D_800CA2AC < 0x400) {
                        if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                            D_800CA2A8 = 1;
                            func_80081128();
                            if (D_800CA2A8 == 1) {
                                return 0;
                            }
                        }
                    } else {
                        D_800CA2AC = 0;
                        func_8007B640();
                        func_80044750(0x74);
                        return 0x64;
                    }
                    dialogue->unk_20 = 1;
                    D_800CA2AC = 0;
                }
            }
        }
        set_kanji_string(-0x56, temp_s1, arg6, "　 ", 0);
        dialogue_render(-0x48, (s32) D_800CA2C0, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        dialogue->unk_30 = 1;
        break;
    case 0xB:
        sp286 = strlen(sp7C);
        if ((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) { /* FAKE: same-line branches preserve scheduling, T-10730. */ if (*(u8 *)&D_800E6280.unk_03C != 0) {
                if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                    func_80046290(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, 0);
                    func_80044750(0x300);
                }
                if (D_800CA2AC < 0x400) {
                    if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                        D_800CA2A8 = 1;
                        func_80081128();
                        if (D_800CA2A8 == 1) {
                            return 0;
                        }
                    }
                } else {
                    D_800CA2AC = 0;
                    func_8007B640();
                    func_80044750(0x74);
                    return 0x64;
                }
                dialogue->unk_20 = 1;
                D_800CA2AC = 0;
            }
        }
        p = &sp7C[sp286];
        if ((p[-1] == 0x6E) && (p[-2] == 0x6E)) {
            if (dialogue->unk_0C == 0) {
                p[-2] = 0U;
                set_kanji_string((s16) (-0x68 - ((s16) strlen("藤崎") * 4)), temp_s1, arg6, "藤崎", 0);
                set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
                dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_14 = 1;
            }
        } else {
            set_kanji_string((s16) (-0x68 - ((s16) strlen("藤崎") * 4)), temp_s1, arg6, "藤崎", 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0xA:
        sp286 = strlen(sp7C);
        if ((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) { /* FAKE: same-line branches preserve scheduling, T-10730. */ if (*(u8 *)&D_800E6280.unk_03C != 0) {
                if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                    func_80046290(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, 0xD);
                    func_80044750(0x300);
                }
                if (D_800CA2AC < 0x400) {
                    if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                        D_800CA2A8 = 1;
                        func_80081128();
                        if (D_800CA2A8 == 1) {
                            return 0;
                        }
                    }
                } else {
                    D_800CA2AC = 0;
                    func_8007B640();
                    func_80044750(0x74);
                    return 0x64;
                }
                dialogue->unk_20 = 1;
                D_800CA2AC = 0;
            }
        }
        p = &sp7C[sp286];
        if ((p[-1] == 0x6E) && (p[-2] == 0x6E)) {
            if (dialogue->unk_0C == 0) {
                p[-2] = 0U;
                set_kanji_string((s16) (-0x68 - ((s16) strlen("好雄") * 4)), temp_s1, arg6, "好雄", 0);
                set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
                dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_14 = 1;
            }
        } else {
            set_kanji_string((s16) (-0x68 - ((s16) strlen("好雄") * 4)), temp_s1, arg6, "好雄", 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0x4:
    case 0x6:
        sp286 = strlen(D_800CA16C);
        sprintf(sp68, "%s ", D_800CA16C);
        set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
        set_kanji_string(-0x56, temp_s1, arg6, "（ ", 0);
        dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "）", "", 0);
        dialogue->unk_14 = 1;
        break;
    case 0x3:
        if (arg2 != 0xFF) {
            junban_init();
            make_three_select(sp7C);
        } else {
            D_800CA2A4[0] = 0;
            D_800CA2A4[1] = 1;
            D_800CA2A4[2] = 2;
        }
        k_disp_start(dialogue_render(-0x72, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "』", "『", 1));
        break;
    case 0x8:
        D_800CA2A4[0] = 0;
        D_800CA2A4[1] = 1;
        k_disp_start(dialogue_render(-0x72, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "』", "『", 1));
        break;
    case 0x13:
        D_800CA2A4[0] = 0;
        D_800CA2A4[1] = 1;
        k_disp_start(dialogue_render(-0x72, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "）", "（", 1));
        break;
    case 0x2:
    case 0x5:
        if ((((s32 *)arg3[arg4][arg5])[dialogue->unk_0C * 2] != 0) && (*(u8 *)&D_800E6280.unk_03C != 0)) {
            if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                func_80046290(((s32 *)arg3[arg4][arg5])[dialogue->unk_0C * 2], ((s32 *)arg3[arg4][arg5])[dialogue->unk_0C * 2 + 1], sp58.value[D_800E6280.unk_F5F]);
                func_80044750(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {
                        return 0;
                    }
                }
            } else {
                D_800CA2AC = 0;
                func_8007B640();
                func_80044750(0x74);
                return 0x64;
            }
            dialogue->unk_20 = 1;
            D_800CA2AC = 0;
        }
        sp286 = 0;
        var_a1_2 = 0;
        for (var_v1 = 0; ; var_v1 += 2) {
            if (sp7C[var_v1] == 0x20 || sp7C[var_v1] == 0x6D ||
                sp7C[var_v1] == 0x4D || sp7C[var_v1] == 0x41 ||
                sp7C[var_v1] == 0x57 || sp7C[var_v1] == 0x4F ||
                sp7C[var_v1] == 0x46 || sp7C[var_v1] == 0x59 ||
                sp7C[var_v1] == 0x70 || sp7C[var_v1] == 0x47 ||
                sp7C[var_v1] == 0x50 || sp7C[var_v1] == 0x53) {
                var_v1--;
            } else if (sp7C[var_v1] == 0x71 || sp7C[var_v1] == 0) {
                sp7C[var_v1] = 0;
                p = &sp7C[sp286];
                var_a1_2++;
                if (arg2 < var_a1_2) {
                    break;
                }
                sp286 = var_v1 + 1;
                var_v1--;
            }
        }
        if (*p != 0x6E) {
            sp27C = p;
            sp286 = strlen(D_800CA188);
            sprintf(sp68, "%s ", D_800CA188);
            set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp27C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0xFF:
        func_8004284C();
        break;
    default:
        func_80046500();
        break;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/main/8007C030", dialogue_speak_part);
#endif

void func_80081128(void) {
    if (D_800CA2AC > 0x200) {
        if (D_800E6280.unk_F88 & 0x600060) {
            func_80044750(0xB4);
            D_800CA2A8 = 0;
            D_800CA2AC = 0;
            D_800CA2C4 = 1;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", dialogue_render);

s32 func_80082764(arg0, arg1, arg2)
u8 arg0;
u8 arg1;
u8 arg2;
{
    DialogueTables *tables;
    DialogueWork *dialogue;
    s16 group;
    s32 result;
    u8 command;

    tables = (DialogueTables *)&D_800CA134;
    dialogue = (DialogueWork *)D_80122CD0;
    if (dialogue->unk_30 != 0) {
        if ((check_end_k() != 0) && ((dialogue->unk_20 == 0) || ((dialogue->unk_20 == 1) && (func_800460EC() != 4)) || ((*(u8 *)&D_800E6280.unk_03C) == 0))) {
            dialogue->unk_30 = 0;
            D_800CA2AC = 0;
        } else if ((D_800E6280.unk_F88 & 0x400040) && ((*(u8 *)&D_800E6280.unk_03C) == 2)) {
            func_80044750(0xB4);
            dialogue->unk_30 = 2;
            D_800CA2AC = 0;
            D_800CA2C4 = 1;
        }
        if ((check_end_k() != 0) && (dialogue->unk_30 == 2) && (func_800460EC() != 4)) {
            dialogue->unk_30 = 0;
            D_800CA2AC = 0;
        }
    } else if (dialogue->unk_48 != 0) {
        if ((check_end_k() != 0) && ((dialogue->unk_20 == 0) || ((dialogue->unk_20 == 1) && (func_800460EC() != 4)) || ((*(u8 *)&D_800E6280.unk_03C) == 0))) {
            k_reset(1);
            dialogue->unk_48 = 0;
            D_800CA2AC = 0;
        }
    } else if (dialogue->unk_14 != 0) {
        if ((D_800E6280.unk_F88 & 0x400040) && ((*(u8 *)&D_800E6280.unk_03C) == 2)) {
            func_80044750(0xB4);
            D_800CA2C4 = 1;
            dialogue->unk_14 = 2;
            D_800CA2AC = 0;
        }
        if ((check_end_k() != 0) && (dialogue->unk_14 == 2) && (func_800460EC() != 4)) {
            k_reset(1);
            dialogue->unk_14 = 0;
            D_8011ED5B &= 0xFF7F;
            D_800CA2AC = 0;
            dialogue->unk_20 = 0;
            return 1;
        }
        if ((D_800E6280.unk_F88 & 0x200020) && (check_end_k() != 0) && ((dialogue->unk_20 == 0) || (dialogue->unk_20 == 0xA) || ((dialogue->unk_20 == 1) && (func_800460EC() != 4)) || ((*(u8 *)&D_800E6280.unk_03C) == 0))) {
            k_reset(1);
            dialogue->unk_14 = 0;
            D_8011ED5B &= 0xFF7F;
            D_800CA2AC = 0;
            dialogue->unk_20 = 0;
            return 1;
        }
        if ((check_end_k() != 0) && ((dialogue->unk_20 == 0) || (dialogue->unk_20 == 0xA) || ((dialogue->unk_20 == 1) && (func_800460EC() != 4)) || ((*(u8 *)&D_800E6280.unk_03C) == 0))) {
            D_8011ED5B |= 0x80;
            func_8006C934();
        } else {
            D_8011ED5B &= 0x7F;
        }
    } else {
        D_800CA2C8 = 0;
        D_800E6280.unk_1104.w = 0;
        group = *tables->group;
        result = dialogue_speak_part(tables->text[group][*tables->line], (s32) tables->command[group][*tables->line], (s32) arg0, tables->voice, (s32) group, (s32) (*tables->line), (s32) arg2);
        if ((arg1 != 0) && (D_800CA2A8 == 0) && ((result & 0xFF) != 0x64)) {
            /* FAKE: expression statement seeds uopt CSE on *tables->line to preserve load and addu operand order. T-11060 */
            *tables->line;
            if (tables->command[*tables->group][*tables->line] == 0xFF) {
                *tables->group = (s16) (*tables->group + 1);
                *tables->line = 0;
            } else {
                *tables->line = (s16) (*tables->line + 1);
            }
        }
    }
    if ((dialogue->unk_30 != 0) || (dialogue->unk_14 != 0) || (dialogue->unk_48 != 0)) {
        if ((check_end_k() == 0) && ((*(u8 *)&D_800E6280.unk_03C) == 0)) {
            command = tables->command[*tables->group][*tables->line - 1];
            if (((command == 1) || (command == 2) || (command == 5) || (command == 0xA) || (command == 0xB) || (command == 0xC) || (command == 0xD)) && (dialogue->unk_2C == 0)) {
                if ((dialogue->unk_24 != 0) && (dialogue->unk_4C == 0) && (dialogue->unk_50 == 0)) {
                    bustup_speech();
                } else if ((dialogue->unk_34 != 0) && (dialogue->unk_40 == 0) && (dialogue->unk_4C == 0)) {
                    func_80067DFC();
                }
            }
        }
    }
    return 0;
}

s32 make_three_select(str)
    u8 *str;
{
    s16 i;
    s16 n;
    s16 start;
    /* FAKE: unused 0x44 bytes above the buffers reproduce the original's 0x170 frame with buf at
     * sp+0x64 (IDO fills locals from the frame top in declaration order); the real locals are
     * unknown. T-16220 */
    u8 pad[0x44];
    u8 buf[3][0x40];

    n = 0;
    start = 0;
    i = 0;
    for (;;) {
        if (str[i] == 'P' || str[i] == 'G') {
            i--;
        } else if (str[i] == 'q' || str[i] == ' ') {
            str[i] = 0;
            strncpy(buf[n++], &str[start], 0x40);
            start = i + 1;
            i--;
        } else if (str[i] == 0) {
            strncpy(buf[n], &str[start], 0x40);
            break;
        }
        i += 2;
    }
    sprintf(str, "%sq%sq%s", buf[D_800CA2A4[0]], buf[D_800CA2A4[1]], buf[D_800CA2A4[2]]);
}

s32 junban_init(void) {
    D_800CA2A4[0] = D_800E6280.unk_10F8 % 3;
    if (D_800CA2A4[0] == 0) {
        D_800CA2A4[1] = D_800CA2A4[0] + (D_800E6280.unk_10F8 & 1) + 1;
        D_800CA2A4[2] = D_800CA2A4[0] + ((D_800E6280.unk_10F8 + 1) & 1) + 1;
    } else if (D_800CA2A4[0] == 1) {
        D_800CA2A4[1] = (D_800CA2A4[0] - 1) + ((D_800E6280.unk_10F8 & 1) * 2);
        D_800CA2A4[2] = (D_800CA2A4[0] - 1) + (((D_800E6280.unk_10F8 + 1) & 1) * 2);
    } else {
        D_800CA2A4[1] = (D_800CA2A4[0] - 1) - (D_800E6280.unk_10F8 & 1);
        D_800CA2A4[2] = (D_800CA2A4[0] - 1) - ((D_800E6280.unk_10F8 + 1) & 1);
    }
}

void normal_date_three_select(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        normal_date_three_select_init();
        return;
    case 1:
        normal_date_three_select_main();
        return;
    default:
        func_80046500();
        return;
    }
}

/* s32 without a return value: the last chain value stays in v0 (the 0x10 height). T-11590 */
s32 normal_date_three_select_init(void) {
    s32 pad; /* FAKE: unused word above the arrays, the original frame is 8 bytes bigger (as in normal_date_two_select_init, T-9020). T-11590 */
    s16 x[3];
    s16 y[3];
    s16 w[3];
    s16 h[3];

    x[0] = x[1] = x[2] = -0x80;
    y[0] = 0x30;
    y[1] = 0x40;
    y[2] = 0x50;
    w[0] = w[1] = w[2] = 0xFC;
    h[0] = h[1] = h[2] = 0x10;
    menu_set(1, 3, x, y, w, h);
    D_800E6280.unk_1094 = 0;
    D_80122D30 = 1;
    D_800E6280.unk_1104.w++;
}

s32 normal_date_three_select_main(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    menu_bar_show(1);
    if (D_800E6280.unk_F88 & 0x200020) {
        if (check_end_k() != 0) {
            if (D_800E6280.unk_F90[1].unk_06 != 0) {
                D_80122CDC = D_800CA2A4[0];
            } else if (D_800E6280.unk_F90[3].unk_06 != 0) {
                D_80122CDC = D_800CA2A4[1];
            } else if (D_800E6280.unk_F90[5].unk_06 != 0) {
                D_80122CDC = D_800CA2A4[2];
            } else {
                return 0;
            }
            D_8011ECD3 &= 0xFF7F;
            D_80122D30 = 0;
            k_reset(1);
            func_8004284C();
        }
    }
}

void normal_date_two_select(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        normal_date_two_select_init();
        return;
    case 1:
        normal_date_two_select_main();
        return;
    default:
        func_80046500();
        return;
    }
}

void normal_date_two_select_init(void) {
    s32 pad; /* FAKE: unused word above the arrays, the original frame is 8 bytes bigger (T-9020) */
    s16 x[2];
    s16 y[2];
    s16 w[2];
    s16 h[2];

    x[0] = x[1] = -0x80;
    y[0] = 0x30;
    y[1] = 0x40;
    w[0] = w[1] = 0xFC;
    h[0] = h[1] = 0x10;
    menu_set(1, 2, x, y, w, h);
    D_800E6280.unk_1094 = 0;
    D_80122D30 = 1;
    D_800E6280.unk_1104.w++;
}

s32 normal_date_two_select_main(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    menu_bar_show(1);
    if (D_800E6280.unk_F88 & 0x200020) {
        if (check_end_k() != 0) {
            if (D_800E6280.unk_F90[1].unk_06 != 0) {
                D_80122CDC = 0;
            } else if (D_800E6280.unk_F90[3].unk_06 != 0) {
                D_80122CDC = 1;
            } else {
                return 0;
            }
            D_8011ECD3 &= 0xFF7F;
            D_80122D30 = 0;
            k_reset(1);
            func_8004284C();
        }
    }
}

void func_80083338(void) {
    func_80083440(get_g_zyotai_h(D_800E6280.unk_F5F) & 0x7F);
    func_8004284C();
}

void func_80083378(void) {
    func_80083440(0);
    func_8004284C();
}

void func_800833A0(void) {
    func_80083440(1);
    func_8004284C();
}

void func_800833C8(void) {
    func_80083440(2);
    func_8004284C();
}

void func_800833F0(void) {
    func_80083440(3);
    func_8004284C();
}

void func_80083418(void) {
    func_80083440(4);
    func_8004284C();
}

void func_80083440(u8 arg0) {
    func_80085CD4(arg0);
    func_800634FC(arg0);
}

s32 func_80083474(void) {
    u8 var_v1;

    if ((((s32 *)D_800CA120)[0] != 0x801820AC || ((s32 *)D_800CA120)[1] != 0x801860AC ||
         ((s32 *)D_800CA120)[2] != 0x8018A0AC || ((s32 *)D_800CA120)[3] != 0x8018E0AC ||
         ((s32 *)D_800CA120)[4] != 0x801920AC || ((s32 *)D_800CA128)[0] != 0x8018005C ||
         ((s32 *)D_800CA128)[1] != 0x80180070 || *(s32 *)D_800CA124 != 0x80180000) &&
        D_800E6280.unk_1108 >= 0x50 && D_800E6280.unk_1108 < 0x70) {
        D_800E6280.unk_110A -= 3;
        func_8004284C();
        return 0;
    }
    if (D_800E6280.unk_1108 >= 0x50 && D_800E6280.unk_1108 < 0x70 && file_checksum_ok(0x2D, 0x80180000) == 0) {
        func_8004284C();
        D_800E6280.unk_110A -= 3;
        return 0;
    }
    var_v1 = get_g_zyotai_h((s32) D_800E6280.unk_F5F) & 0x7F;
    if ((u8) D_800E6280.unk_F5F >= 0xCU) {
        var_v1 = 0;
    }
    func_80062DBC(D_800CA130, D_800CA120, D_800CA124, D_800CA128, var_v1);
    func_8004284C();
}

void func_80083628(void) {
    u8 var_v1;

    var_v1 = get_g_zyotai_h((s32) D_800E6280.unk_F5F) & 0x7F;
    if ((u8) D_800E6280.unk_F5F >= 0xCU) {
        var_v1 = 0;
    }
    func_80062DBC(D_800CA130, D_800CA120, D_800CA124, D_800CA128, var_v1);
    func_8004284C();
}

s32 func_800836A0(void) {
    /* The two early failure paths return zero; the success path falls through. */
    if ((((s32 *)D_800CA120)[0] != 0x801820AC) || (((s32 *)D_800CA120)[1] != 0x801860AC) || (((s32 *)D_800CA120)[2] != 0x8018A0AC) || (((s32 *)D_800CA120)[3] != 0x8018E0AC) || (((s32 *)D_800CA120)[4] != 0x801920AC)) {
        D_800E6280.unk_110A -= 4;
        return 0;
    }
    if (file_checksum_ok(0x2D, 0x80180000) == 0) {
        func_8004284C();
        D_800E6280.unk_110A -= 5;
        return 0;
    }
    /* FAKE: narrowing before the mask preserves temporary allocation. T-10020. */
    func_80062DBC(D_800CA130, D_800CA120, D_800CA124, D_800CA128, (u8)get_g_zyotai_h((s32) D_800E6280.unk_F5F) & 0x7F);
    /* One record-table view keeps the byte clear before the flag updates. */
    D_8011ECD0[31 * 0x44 + 7] = 0;
    *(s32 *)&D_8011ECD0[31 * 0x44 + 0x38] |= 0x81000000;
    *(s32 *)&D_8011ECD0[30 * 0x44 + 0x38] |= 0x81000000;
    D_801217D0->unk_00 |= 0x81000000;
    D_801217D0[1].unk_00 |= 0x81000000;
    func_8004284C();
}

/* s32 without a return value: the ~0x80 mask is then loaded into v0 (li v0,-129) as in the
 * original, although the byte clears use andi 0xFF7F. T-11590 */
s32 func_80083808(void) {
    if (D_80122CF4 != 0) {
        D_8011ECD0[0x83F] |= 0x80;
        D_8011ECD0[0x7FB] |= 0x80;
        D_801217D0[0].unk_00 &= 0x7FFFFFFF;
        D_801217D0[1].unk_00 &= 0x7FFFFFFF;
        func_80063668();
        if (D_80122D20 != 0) {
            D_801217D0[0].unk_0F = 0;
        }
        if (D_800E6280.unk_10F8 % 240 == 0) {
            bustup_wink();
        }
    } else if (D_80122D04 != 0) {
        D_8011ECD0[0x83F] &= ~0x80;
        D_8011ECD0[0x7FB] &= ~0x80;
        D_801217D0[0].unk_00 |= 0x80000000;
        D_801217D0[1].unk_00 |= 0x80000000;
        if (D_80122D10 == 1) {
            if (D_800E6280.unk_10F8 % 240 == 0) {
                func_80067DFC();
            }
        } else if (D_80122D10 == 0) {
            func_80067C28();
        }
        if (D_800E6280.unk_10F8 % 240 == 0 && D_80122D0C == 0) {
            func_80067DD4();
        }
    } else {
        D_8011ECD0[0x83F] &= ~0x80;
        D_8011ECD0[0x7FB] &= ~0x80;
        D_801217D0[0].unk_00 |= 0x80000000;
        D_801217D0[1].unk_00 |= 0x80000000;
    }
}

void func_80083A10(void) {
    if (D_80122CF8 != 0) {
        func_800578F4(1);
    }
    if (D_80122D2C == 0 && D_80122D04 == 0 && (D_800E6280.unk_F80 & 0xF000F000)) {
        D_8011ECD0[3] |= 0x80;
    }
    if (D_80122D04 != 0 || D_80122D2C != 0) {
        D_8011ECD0[3] &= 0x7F;
    }
    if (D_80122D30 == 1) {
        D_8011ECD0[3] |= 0x80;
    }
    if (D_8011ECD0[3] & 0x80) {
        func_8006BA40();
    }
}

void hizuke_hide(void) {
    hizuke_disp_switch(0);
    func_8004284C();
}

void hizuke_appear(void) {
    hizuke_disp_switch(1);
    func_8004284C();
}

void read_bustup(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x55B2);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x55DF);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x560C);
        } else {
            func_80062CD0(0x5639);
        }
        break;
    case 1:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x5D9B);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x5DC8);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x5DF5);
        } else {
            func_80062CD0(0x5E22);
        }
        break;
    case 2:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x5288);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x52B5);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x52E2);
        } else {
            func_80062CD0(0x530F);
        }
        break;
    case 3:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x5909);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x5936);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x5963);
        } else {
            func_80062CD0(0x5990);
        }
        break;
    case 4:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x6719);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x6746);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x6773);
        } else {
            func_80062CD0(0x67A0);
        }
        break;
    case 5:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x6011);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x603E);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x606B);
        } else {
            func_80062CD0(0x6098);
        }
        break;
    case 6:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x6287);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x62B4);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x62E1);
        } else {
            func_80062CD0(0x630E);
        }
        break;
    case 7:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x5B52);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x5B7F);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x5BAC);
        } else {
            func_80062CD0(0x5BD9);
        }
        break;
    case 8:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x503F);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x506C);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x5099);
        } else {
            func_80062CD0(0x50C6);
        }
        break;
    case 9:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x64FD);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x652A);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x6557);
        } else {
            func_80062CD0(0x6584);
        }
        break;
    case 10:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x698F);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x69BC);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x69E9);
        } else {
            func_80062CD0(0x6A16);
        }
        break;
    case 13:
        func_80062CD0(0x6C8C);
        break;
    }
    func_8004284C();
}

void read_bustup_uniform(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x541D);
        } else {
            func_80062CD0(0x544A);
        }
        break;
    case 1:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x5D14);
        } else {
            func_80062CD0(0x5D41);
        }
        break;
    case 2:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x5201);
        } else {
            func_80062CD0(0x522E);
        }
        break;
    case 3:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x5882);
        } else {
            func_80062CD0(0x58AF);
        }
        break;
    case 4:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x6692);
        } else {
            func_80062CD0(0x66BF);
        }
        break;
    case 5:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x5F8A);
        } else {
            func_80062CD0(0x5FB7);
        }
        break;
    case 6:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x6200);
        } else {
            func_80062CD0(0x622D);
        }
        break;
    case 7:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x5AF8);
        } else {
            func_80062CD0(0x5B25);
        }
        break;
    case 8:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x4FE5);
        } else {
            func_80062CD0(0x5012);
        }
        break;
    case 9:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x64A3);
        } else {
            func_80062CD0(0x64D0);
        }
        break;
    case 10:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x6908);
        } else {
            func_80062CD0(0x6935);
        }
        break;
    case 12:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x57FB);
        } else {
            func_80062CD0(0x5828);
        }
        break;
    case 11:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x6B51);
        } else {
            func_80062CD0(0x6B7E);
        }
        break;
    case 13:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x6C32);
        } else {
            func_80062CD0(0x6C5F);
        }
        break;
    case 14:
        func_80062CD0(0x6B24);
        break;
    }
    func_8004284C();
}

void read_bustup_swimsuit(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        func_80062CD0(0x5666);
        break;
    case 1:
        func_80062CD0(0x5E4F);
        break;
    case 2:
        func_80062CD0(0x533C);
        break;
    case 3:
        func_80062CD0(0x59BD);
        break;
    case 4:
        func_80062CD0(0x67CD);
        break;
    case 5:
        func_80062CD0(0x60C5);
        break;
    case 6:
        func_80062CD0(0x633B);
        break;
    case 7:
        func_80062CD0(0x5C06);
        break;
    case 8:
        if (D_800E6280.unk_03E == 0x5F) {
            func_80062CD0(0x50F3);
        } else if (D_800E6280.unk_03E == 0x60) {
            func_80062CD0(0x5120);
        } else {
            func_80062CD0(0x514D);
        }
        break;
    case 9:
        func_80062CD0(0x65B1);
        break;
    case 10:
        func_80062CD0(0x6A43);
        break;
    }
    func_8004284C();
}

void bustup_return(void) {
    read_bustup();
}

void return_step(void) {
    main_state_set(D_800E6280.unk_720);
    func_80042908(D_800E6280.unk_721);
    func_80042940(D_800E6280.unk_722);
}

void k_disp_inc2(void) {
    s32 pad; /* FAKE: unused slot above sp2B, the original frame has it (real source unknown). T-8070 */
    u8 sp2B;

    if (D_80122CFC-- < 1) {
        D_80122CFC = 0;
        sp2B = get_k_speed();
        if (D_800E6280.unk_F80 & 0x600060) {
            k_speed_set(0);
        }
        k_disp_inc();
        k_speed_set(sp2B);
    }
}

void yosi_trans(void) {
    func_8004500C(0, 0x200);
    func_8004284C();
}

void change_yoshio(void) {
    D_800E6280.unk_F5F = 0xD;
    func_8004284C();
}

void change_girl(void) {
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_8004284C();
}

void func_800847B8(u8 arg0) {
    s32 t;

    strcpy(D_800CA16C, D_800E6280.unk_0D4);
    strcpy(D_800CA174, D_800E6280.unk_0DC);
    if (arg0 < 0xE) {
        t = arg0;
        get_p_name(D_800CA17C, t);
        strcpy(D_800CA188, girl_short_name_table[arg0]);
        get_g_name(girl_name_buf, t);
    }
}

void select_girl(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        select_girl_init(0);
        return;
    case 1:
        select_girl_main(0);
        return;
    default:
        func_80046500();
        return;
    }
}

void select_girl2(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        select_girl_init(1);
        return;
    case 1:
        select_girl_main(1);
        return;
    default:
        func_80046500();
        return;
    }
}

void select_girl_init(arg0)
u8 arg0;
{
    /* T-10900: promoted scalars precede the eight-byte name buffer. */
    s16 i;
    s32 length;
    s32 result;
    u8 label[8];
    for (i = 0; i < 11; i++) {
        D_800CA300[i] = ((i / 3) << 6) - 0x80;
        D_800CA318[i] = ((i % 3) * 16) + 0x30;
        if (D_800CA2B0[i] != 0) {
            strcpy(label, girl_short_name_table[i]);
            result = set_kanji_string(D_800CA300[i], D_800CA318[i], 0, label, 0);
            length = strlen(label);
            D_800CA330[i] = (length / 2) * 14;
        } else {
            D_800CA330[i] = 0;
        }
        D_800CA348[i] = 16;
    }
    D_800CA300[11] = 0x40;
    D_800CA318[11] = 0x50;
    D_800CA330[11] = 0x2A;
    D_800CA348[11] = 0x10;
    if ((u8)arg0 != 0) D_800CA330[11] = 0;
    if ((u8)arg0 == 0) result = set_kanji_string(0x40, 0x50, 0, "やめる", 0);
    k_disp_start(result);
    menu_set(1, (0xC - (u8)arg0) & 0xFF, D_800CA300, D_800CA318, D_800CA330, D_800CA348);
    for (i = 0; i < 11; i++) {
        if (D_800CA330[i] != 0) {
            D_800E6280.unk_1094 = i;
            break;
        }
    }
    D_80122D30 = 1;
    D_800E6280.unk_1104.w++;
}

s32 select_girl_main(s32 arg0) {
    s16 i;
    u32 n;

    menu_check(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    menu_bar_show(1);
    if (D_800E6280.unk_F88 & 0x200020) {
        n = 0xC - (u8)arg0;
        for (i = 0; i < n; i++) {
            if (((u8 *)&D_800E6280)[i * 16 + 0xF9E] != 0 && D_800CA330[i] != 0) {
                D_80122CDC = i;
                D_8011ECD3 &= 0xFF7F;
                D_80122D30 = 0;
                k_reset(1);
                func_8004284C();
                return 0;
            }
        }
    }
}

void sprite_brightness(s16 idx, u8 val) {
    u8 *p = (u8 *)D_801217D0 + idx * 36;
    p[0x16] = val;
    p[0x15] = val;
    p[0x14] = val;
}

void day_plus(void) {
    s32 sp28; /* FAKE: unused local, the original frame has one more word below sp2B (T-3330 slot rule); source unknown. T-6040 */
    u8 sp2B;

    D_800CA2F0 = 0;
    sp2B = D_8011F113;
    func_80072338();
    hizuke_init();
    if (sp2B & 0x80) {
        hizuke_disp_switch(1);
    } else {
        hizuke_disp_switch(0);
    }
    hizuke_show();
    func_8004284C();
}

void check_para_limit(void) {
    s16 i;

    for (i = 0; i < 11; i++) {
        if (D_800E6280.unk_1BC[i].unk_02 < 0) {
            *(s32 *) &D_800E6280.unk_1BC[i].unk_00 = 0; /* word store over the s16 pair */
        }
        if (D_800E6280.unk_1BC[i].unk_02 >= 0x80) {
            D_800E6280.unk_1BC[i].unk_02 = 0x80;
        }
        if (D_800E6280.unk_1BC[i].unk_06 < 0) {
            *(s32 *) &D_800E6280.unk_1BC[i].unk_04 = 0;
        }
        if (D_800E6280.unk_1BC[i].unk_06 >= 0x80) {
            D_800E6280.unk_1BC[i].unk_06 = 0x80;
        }
        if (D_800E6280.unk_1BC[i].unk_0A < 0) {
            *(s32 *) &D_800E6280.unk_1BC[i].unk_08 = 0;
        }
        if (D_800E6280.unk_1BC[i].unk_0A >= 0xFF) {
            D_800E6280.unk_1BC[i].unk_0A = 0xFF;
        }
    }
    for (i = 0; i < 9; i++) {
        if ((&D_800E6280.unk_0FC)[i].unk_02 < 0) {
            ((s32 *) &D_800E6280.unk_0FC)[i] = 0;
        }
        if ((&D_800E6280.unk_0FC)[i].unk_02 >= 0x3E7) {
            ((s32 *) &D_800E6280.unk_0FC)[i] = 0x3E70000;
        }
    }
}

void func_80084E4C(void) {
    func_8006C848(0);
}

void func_80084E6C(void) {
    D_80122D20 = 0;
    func_8004284C();
}

/* s32 without a return value: the last chain value stays in v0 (andi v0,a0,0xffff). T-11590 */
s32 func_80084E90(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s16 i;

    D_80122D04 = 0;
    D_80122D0C = 0;
    D_80122D10 = 0;
    LoadSquare(0x140, 0x80, 0x40, 0x80, (void *)arg0);
    LoadSquare(0x180, 0x80, 0x40, 0x80, (void *)arg1);
    LoadSquare(0x1C0, 0x80, 0x40, 0x80, (void *)arg2);
    LoadSquare(0x200, 0x80, 0x40, 0x80, (void *)arg3);
    LoadSquare(0x240, 0x80, 0x40, 0x80, (void *)arg4);
    LoadSquare(0x280, 0x80, 0x40, 0x80, (void *)arg5);
    D_800E6280.unk_1228[37] = 1;
    D_800E6280.unk_1228[38] = 1;
    D_800E6280.unk_1228[39] = 1;
    D_800E6280.unk_1228[40] = 1;
    D_800E6280.unk_1228[41] = 1;
    D_800E6280.unk_1228[42] = 1;
    for (i = 0x3A; i < 0x40; i++) {
        D_801217D0[i].unk_0C = i - 0x35;
        D_801217D0[i].unk_00 = 0xC1000000;
        /* FAKE: u16 view of the width (GsSPRITE w is unsigned; Rec24 keeps s16 for ETC
         * func_80142A4C): the 0x80 constant is then shared with unk_0A and unk_0F. T-11590 */
        *(u16 *)&D_801217D0[i].unk_08 = 0x80;
        D_801217D0[i].unk_0A = 0x80;
        D_801217D0[i].unk_0E = 0;
        D_801217D0[i].unk_0F = 0x80;
        D_801217D0[i].unk_10 = 0;
        D_801217D0[i].unk_12 = 0x1F1;
        D_801217D0[i].unk_14 = 0;
        D_801217D0[i].unk_15 = 0;
        D_801217D0[i].unk_16 = 0;
        D_801217D0[i].unk_18 = 0;
        D_801217D0[i].unk_1A = 0;
        D_801217D0[i].unk_1C = 0x1000;
        D_801217D0[i].unk_1E = 0x1000;
        D_801217D0[i].unk_20 = 0;
        D_801217D0[i].unk_04 = ((i - 0x3A) % 3) * 128 - 0xA0;
        D_801217D0[i].unk_06 = ((i - 0x3A) / 3) * 128 - 0x78;
        D_800E6280.unk_10A5[i] = 0xB;
    }
    D_801217D0[60].unk_08 = D_801217D0[63].unk_08 = 0x40;
    D_801217D0[61].unk_0A = D_801217D0[62].unk_0A = D_801217D0[63].unk_0A = 0x20;
}

s32 func_800850D4(arg0, arg1, arg2, arg3)
    s32 arg0;
    s32 arg1;
    s32 arg2;
    s16 arg3;
{
    s16 i;
    DisplayActor44 *p;

    if (arg0 == 0) {
        func_80048F64(0x60);
        func_80048F64(0x61);
        return 0;
    }
    for (i = 0; i < 2; i++) {
        func_80048F64(i + 0x60);
        p = &((DisplayActor44 *)D_8011ECD0)[i + 96];
        p->unk_01 = 0xA;
        p->unk_02 = 0;
        p->unk_38 = 0x41000000;
        p->unk_03 = 4;
        p->unk_0C = (u8 *)arg0;
        p->unk_10 = (u8 *)arg1;
        p->unk_34 = (u8 *)arg2;
        p->unk_16 = 0;
        p->unk_18 = 0;
        p->unk_06 = 2;
        p->unk_43 = 0x11;
        p->unk_26 = -0xA0;
        p->unk_2A = -0x78;
        p->unk_07 = 0;
        p->unk_05 = 0;
        p->unk_04 = 0;
        p->unk_14 = arg3;
    }
    ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 4;
    if (arg3 >= 2) {
        D_800CA360 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 2;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0;
    }
}

void func_80085234(arg0)
    u8 arg0;
{
    s16 i;
    u8 t;

    if (arg0) {
        if (D_8011ECD0[0x1983] != 0) {
            t = D_800B5A60;
            D_800B5A60 = D_800CA360;
            D_800CA360 = t;
            D_8011ECD0[0x1983] |= 0x80;
        }
        if (D_8011ECD0[0x19C7] != 0) {
            D_8011ECD0[0x19C7] |= 0x80;
        }
        for (i = 0x3A; i < 0x40; i++) {
            D_801217D0[i].unk_00 &= 0x7FFFFFFF;
        }
    } else {
        D_800B5A60 = D_800CA360;
        if (D_8011ECD0[0x1983] != 0) {
            D_8011ECD0[0x1983] &= ~0x80;
        }
        if (D_8011ECD0[0x19C7] != 0) {
            D_8011ECD0[0x19C7] &= ~0x80;
        }
        for (i = 0x3A; i < 0x40; i++) {
            D_801217D0[i].unk_00 |= 0x80000000;
        }
    }
}

void func_80085368(void) {
    s16 var_s0;

    normal_date_bg_fadein();
    if (D_800B593C == 4) {
        func_80085234(1);
    }
    D_80122D04 = 1;
    func_80065F34(0);
    D_80120657 = D_800B593C;
    var_s0 = 0x3A;
    D_8012069B = D_800B593C;
    do {
        func_80084C98(var_s0, D_800B593C);
        var_s0 += 1;
    } while (var_s0 < 0x40);
}

void func_800853FC(void) {
    s16 var_s0;
    u8 temp_s0;
    u8 state;

    temp_s0 = D_800E6280.unk_110A;
    normal_date_bg_fadeout();
    state = D_800E6280.unk_110A;
    if (state != temp_s0) {
        func_80085234(0);
        D_80122D04 = 0;
        func_80065F34(1);
    }
    D_80120657 = D_800B593C;
    var_s0 = 0x3A;
    D_8012069B = D_800B593C;
    do {
        func_80084C98(var_s0, D_800B593C);
        var_s0 += 1;
    } while (var_s0 < 0x40);
}

void event_face0(void) {
    D_80120666 = 2;
    D_801206AA = 0;
    func_8004284C();
}

void event_face1(void) {
    D_80120666 = 3;
    D_801206AA = 1;
    func_8004284C();
}

void event_face2(void) {
    D_80120666 = 6;
    D_801206AA = 4;
    func_8004284C();
}

void event_face3(void) {
    D_80120666 = 7;
    D_801206AA = 5;
    func_8004284C();
}

void event_face_kuchi0(void) {
    D_80120666 = 2;
    func_8004284C();
}

void event_face_kuchi1(void) {
    D_80120666 = 3;
    func_8004284C();
}

void event_kuchi_ani_no(void) {
    D_80122D10 = 0;
    D_80120652 = 4;
    func_8004284C();
}

void event_kuchi_ani_yes(void) {
    D_80122D10 = 1;
    D_80120652 = 0;
    func_8004284C();
}

void ev_me_on(void) {
    D_80122D0C = 0;
    D_80120696 = 5;
    func_8004284C();
}

void ev_me_on_continue(void) {
    D_80122D0C = 1;
    D_80120696 = 5;
    func_8004284C();
}

void _sprite_set_box_shade_tarao(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u32 arg5, u32 arg6) {
    s32 sp28;
    s32 temp_v0;
    u32 temp_t6;
    u32 temp_t7;
    u32 temp_t8;
    u32 temp_t9;

    temp_v0 = GetWorkBase(0x24, D_8011ECA0);
    sp28 = temp_v0;
    func_8009F054(temp_v0);
    SetSemiTrans(sp28, 0);
    temp_t8 = arg5 >> 0x10;
    temp_t6 = arg6 >> 0x10;
    temp_t7 = arg6 >> 8;
    temp_t9 = arg5 >> 8;
    *(s8 *)((unsigned char *)sp28 + (5)) = (s8) temp_t7;
    *(s8 *)((unsigned char *)sp28 + (4)) = (s8) temp_t6;
    *(s8 *)((unsigned char *)sp28 + (6)) = (s8) arg6;
    *(s8 *)((unsigned char *)sp28 + (13)) = (s8) temp_t7;
    *(s8 *)((unsigned char *)sp28 + (12)) = (s8) temp_t6;
    *(s8 *)((unsigned char *)sp28 + (14)) = (s8) arg6;
    *(s8 *)((unsigned char *)sp28 + (21)) = (s8) temp_t9;
    *(s8 *)((unsigned char *)sp28 + (20)) = (s8) temp_t8;
    *(s8 *)((unsigned char *)sp28 + (22)) = (s8) arg5;
    *(s8 *)((unsigned char *)sp28 + (28)) = (s8) temp_t8;
    *(s8 *)((unsigned char *)sp28 + (30)) = (s8) arg5;
    *(s8 *)((unsigned char *)sp28 + (29)) = (s8) temp_t9;
    *(s16 *)((unsigned char *)sp28 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0);
    *(s16 *)((unsigned char *)sp28 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1);
    *(s16 *)((unsigned char *)sp28 + (16)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0 + arg2);
    *(s16 *)((unsigned char *)sp28 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1);
    *(s16 *)((unsigned char *)sp28 + (24)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0);
    *(s16 *)((unsigned char *)sp28 + (26)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1 + arg3);
    *(s16 *)((unsigned char *)sp28 + (32)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0 + arg2);
    *(s16 *)((unsigned char *)sp28 + (34)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1 + arg3);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (arg4 * 4)], sp28);
}

void func_8008585C(void) {
    s16 i;
    for (i = 5; i < 31;) {
        D_80122CD0[i] = 0;
        i++;
    }
    D_800CA2AC = 0;
    func_8006BC28(0);
    D_8011ECD3 &= 0xFF7F;
    k_reset(0);
    func_8006612C((s32) "　");
    func_80065F34(0);
    func_8006764C(0);
}

void don_init2(void) {
    func_80047550();
    func_8004284C();
}

void don_wait(void) {
    if ((u32)D_800E6280.unk_1104.w++ >= 0x81U) {
        k_reset(1);
        func_8004284C();
    }
}

void normal_date_speak_1line(void) {
    D_800CA134 = (u8 *)&D_800CA148;
    D_800CA138 = (u8 *)&D_800CA14C;
    D_800CA13C = D_800CA160;
    D_800CA140 = D_800CA164;
    D_800CA144 = D_800CA168;
    if (func_80082764(D_80122CDC, 1, 0) == 1) {
        func_8004284C();
    }
}

void normal_date_speak_012(void) {
    D_800CA134 = &D_800CA148;
    D_800CA138 = &D_800CA14C;
    D_800CA13C = D_800CA160;
    D_800CA140 = D_800CA164;
    D_800CA144 = D_800CA168;
    func_80082764(0xFF, 1, 0);
}

void normal_date_speak(void) {
    D_800CA134 = &D_800CA148;
    D_800CA138 = &D_800CA14C;
    D_800CA13C = D_800CA160;
    D_800CA140 = D_800CA164;
    D_800CA144 = D_800CA168;
    func_80082764(D_80122CDC, 1, 0);
}

void vram_bustup_clear(void) {
    s16 i;

    func_80048F64(0x1F);
    func_80048F64(0x1E);
    for (i = 5; i < 0xA; i++) {
        D_800E6280.unk_1228[i] = -1;
    }
    func_8004284C();
}

s32 func_80085B3C(arg0, arg1)
    u8 arg0;
    u8 arg1;
{
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_00 = D_800E6280.unk_F5F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_02 = arg0;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = arg1;
    if (D_80122D48 == 0 || D_80122D48 >= 7) {
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = get_g_zyotai_h(D_800E6280.unk_F5F);
    } else {
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = D_80122D48 - 1;
    }
    D_80122D48 = 0;
    if (D_800E6280.unk_F5F == 0xC) {
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 0;
    }
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_04 = D_800E6280.unk_03E;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_05 = D_800E6280.unk_03F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_06 = D_800E6280.unk_040;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = D_800E6280.unk_041;
    if (++D_800E6280.unk_F5E >= 0xFF) {
        D_800E6280.unk_F5E = 0xFE;
    }
}

void func_80085CD4(u8 a) {
    switch (a) {
    case 0:
        D_801217D0[0].unk_0C = 7;
        D_801217D0[0].unk_0E = 0x30;
        D_801217D0[1].unk_0C = 7;
        D_801217D0[1].unk_0E = 0;
        break;
    case 1:
        D_801217D0[0].unk_0C = 7;
        D_801217D0[0].unk_0E = 0x70;
        D_801217D0[1].unk_0C = 7;
        D_801217D0[1].unk_0E = 0x40;
        break;
    case 2:
        D_801217D0[0].unk_0C = 8;
        D_801217D0[0].unk_0E = 0x30;
        D_801217D0[1].unk_0C = 8;
        D_801217D0[1].unk_0E = 0;
        break;
    case 3:
        D_801217D0[0].unk_0C = 8;
        D_801217D0[0].unk_0E = 0x70;
        D_801217D0[1].unk_0C = 8;
        D_801217D0[1].unk_0E = 0x40;
        break;
    case 4:
        D_801217D0[0].unk_0C = 9;
        D_801217D0[0].unk_0E = 0x30;
        D_801217D0[1].unk_0C = 9;
        D_801217D0[1].unk_0E = 0;
        break;
    case 5:
        D_801217D0[0].unk_0C = 9;
        D_801217D0[0].unk_0E = 0x70;
        D_801217D0[1].unk_0C = 9;
        D_801217D0[1].unk_0E = 0x40;
        break;
    }
}
