/* scene_library.c: src/main/8007C030.c; T-27059: original object [0x8007C030,0x80085E30); shared date/scene step primitives, portrait and face handling, dialogue and choices, heroine selection, SD sprite setup, day advance and parameter-limit helpers. Includes leading CD-level sampling, peak and average helpers as well as SD/portrait/face, normal-date transitions, girl selection, name-code and talk-script utilities. Original object 0x8007C030-0x80085E30 (config/objects/main.txt). */
#define MAIN_API_OVERRIDE_strncpy /* Implicit integer strncpy call view preserves v1 length calculation; shared void view differs by 12 rows. T-20007. */
#define MAIN_API_OVERRIDE_prim_gradient_line /* Transition calls narrow four coordinates at the call site. T-10870. */
#define MAIN_API_OVERRIDE_SsVabTransCompleted /* SD_GetCDLevel reads the transfer status; canonical legacy view is void. T-10870. */
#define MAIN_API_OVERRIDE_func_80084C98 /* promoted s32 index view avoids call-site narrowing; existing callee performs the s16 conversion, T-9540 */
#define MAIN_API_OVERRIDE_talk_work /* word-indexed clearing view, canonical scalar retained elsewhere, T-9540 */
#define MAIN_API_OVERRIDE_select_girl_init /* K&R byte parameter is narrowed in the callee. T-10900. */
#define MAIN_API_OVERRIDE_get_now_k /* implicit declaration, as matched (main_api.h: u8(void)) */
#include "common.h"
#include "game.h"
#include "psx_types.h"
s32 prim_gradient_line(s16 x0, s16 y0, s16 x1, s16 y1, s32 arg4, s32 arg5, s32 arg6);
s32 SsVabTransCompleted();
/* FAKE: preserve the original implicit integer view of the SDK strncpy alias. T-20007. */
s32 strncpy();
void select_girl_init();
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800C9F60);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800C9FF8);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA048);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA108);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA120);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA124);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA128);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA130);
INCLUDE_RODATA("asm/data/main/scene_library.data", talk_tables);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA138);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA13C);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA140);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA144);
INCLUDE_RODATA("asm/data/main/scene_library.data", talk_group);
INCLUDE_RODATA("asm/data/main/scene_library.data", talk_line);
INCLUDE_RODATA("asm/data/main/scene_library.data", talk_text_table);
INCLUDE_RODATA("asm/data/main/scene_library.data", talk_command_table);
INCLUDE_RODATA("asm/data/main/scene_library.data", talk_voice_table);
INCLUDE_RODATA("asm/data/main/scene_library.data", name_code_family);
INCLUDE_RODATA("asm/data/main/scene_library.data", name_code_given);
INCLUDE_RODATA("asm/data/main/scene_library.data", name_code_player_form);
INCLUDE_RODATA("asm/data/main/scene_library.data", name_code_girl_short);
INCLUDE_RODATA("asm/data/main/scene_library.data", girl_name_buf);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA19C);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA1DC);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA23C);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA248);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA250);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA25C);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA280);
u8 D_800CA2A4[4] = { 0, 1, 2, 0 };
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA2A8);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA2AC);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA2B0);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA2BC);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA2C0);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA2C4);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA2C8);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA2F0);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA300);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA318);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA330);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA348);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA360);
INCLUDE_RODATA("asm/data/main/scene_library.data", D_800CA36C);
/* FAKE: typed view of the existing same-address alias keeps the byte argument load in a1 and the index conversion inside the matched callee. T-9540 */
void func_80084C98(s32 idx, u8 val);
/* FAKE: minimum array view inferred from clearing words 5 through 30; full object extent remains unknown. T-9540 */
extern s32 talk_work[];
/* Declared here, not in main_api.h: EVENT src/ovl/EVENT/nozomi_scenes.c declares the same address as
 * a function table (FnTbl55); moving it needs a MAIN_API_OVERRIDE there. T-11590 */
extern s32 D_80122D48;

void SD_GetCDLevel(CdLevelWork *arg0) {
    s32 shift;
    u32 left;
    u32 right;

    arg0->unk_00 = 0U;
    arg0->unk_02 = 0U;
    if (((u32) D_80125D14 >> 0x18) == 0) {
        if (!(audio_state.unk_00 & 0x4000) && (func_80094B80(0) == 1)) {
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
    if (audio_state.unk_00 & 0x400) {
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

/* Set the portrait sprite addresses for the current girl */
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
        mode_step_next();
        return;
    }
    if ((u32)game_state.mode_step_timer.w++ >= 0x401) {
        cd_command_reset();
        sp29 = D_800B5939;
        sp2A = *(u8 *)&dec_bg_last_screen_mode;
        sp2B = dec_bg_brightness;
        dec_bg_reset();
        *(u8 *)&dec_bg_last_screen_mode = sp2A;
        dec_bg_brightness = sp2B;
        D_800B5939 = sp29;
        game_state.mode_step -= 1;
    }
}

void func_8007C844(void) {
    D_80122CF8 = 1;
    dec_bg_brightness = 0x80;
    mode_step_next();
}

void func_8007C878(void) {
    D_80122CF8 = 0;
    dec_bg_brightness = 0;
    mode_step_next();
}

void func_8007C8A4(void) {
    if (func_8007C8D8() == 1) {
        mode_step_next();
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

    g = &game_state;
    srand(g->frame_count);
    if (g->mode_step_timer.u == 0) {
        D_80122D3C = g->month % 7U;
    }
    if (D_80122D3C != 6) {
        D_80122CF8 = 2;
    }
    dec_bg_brightness += 8;
    if ((u8)dec_bg_brightness >= 0x80U) {
        dec_bg_brightness = 0x80;
    }
    switch (D_80122D3C) {
    case 1:
        for (outer = g->mode_step_timer.u * 5; outer < 0x50; outer++) {
            prim_gradient_line(-0xA0, -0x29 - outer, 0xA0, -0x29 - outer, 0, 0, 0xD);
            prim_gradient_line(-0xA0, outer - 0x29, 0xA0, outer - 0x29, 0, 0, 0xD);
        }
        break;
    case 2:
        for (outer = g->mode_step_timer.u; outer < 0x10; outer++) {
            for (left_x = 0; left_x < 10; left_x++) {
                top_y = left_x * 16 + outer - 0x78;
                prim_gradient_line(-0xA0, top_y, 0xA0, top_y, 0, 0, 0xD);
            }
        }
        break;
    case 3:
        for (outer = g->mode_step_timer.u << 5; outer < 0x1E0; outer++) {
            if (outer < 0xA0) {
                prim_gradient_line(outer - 0xa0, -0x78, -0xA0, outer - 0x78, 0, 0, 0xD);
            } else if (outer < 0x140) {
                prim_gradient_line(outer - 0xa0, -0x78, outer - 0x140, 0x27, 0, 0, 0xD);
            } else {
                prim_gradient_line(0xA0, outer - 0x1b8, outer - 0x140, 0x27, 0, 0, 0xD);
            }
        }
        break;
    case 4:
        for (outer = g->mode_step_timer.u << 6; outer < 0x1E0; outer += 4) {
            if (outer < 0xA0) {
                prim_gradient_line(outer - 0xa0, -0x78, -0xA0, outer - 0x78, 0, 0, 0xD);
            } else if (outer < 0x140) {
                prim_gradient_line(outer - 0xa0, -0x78, outer - 0x140, 0x27, 0, 0, 0xD);
            } else {
                prim_gradient_line(0xA0, outer - 0x1b8, outer - 0x140, 0x27, 0, 0, 0xD);
            }
        }
        for (outer = g->mode_step_timer.u << 6; outer < 0x1E0; outer += 4) {
            if (outer < 0xA0) {
                prim_gradient_line(outer - 0x9f, -0x78, -0xA0, outer - 0x77, 0, 0, 0xD);
            } else if (outer < 0x140) {
                prim_gradient_line(outer - 0x9f, -0x78, outer - 0x13f, 0x27, 0, 0, 0xD);
            } else {
                prim_gradient_line(0xA0, outer - 0x1b7, outer - 0x13f, 0x27, 0, 0, 0xD);
            }
        }
        if ((g->mode_step_timer.u << 6) < 0x1E0U) left_x = 0;
        else left_x = (g->mode_step_timer.u << 6) - 0x1E0;
        for (outer = left_x; outer < 0x1E0; outer += 4) {
            if (outer < 0xA0) {
                prim_gradient_line(outer - 0x9e, -0x78, -0xA0, outer - 0x76, 0, 0, 0xD);
            } else if (outer < 0x140) {
                prim_gradient_line(outer - 0x9e, -0x78, outer - 0x13e, 0x27, 0, 0, 0xD);
            } else {
                prim_gradient_line(0xA0, outer - 0x1b6, outer - 0x13e, 0x27, 0, 0, 0xD);
            }
        }
        for (outer = left_x; outer < 0x1E0; outer += 4) {
            if (outer < 0xA0) {
                prim_gradient_line(outer - 0x9d, -0x78, -0xA0, outer - 0x75, 0, 0, 0xD);
            } else if (outer < 0x140) {
                prim_gradient_line(outer - 0x9d, -0x78, outer - 0x13d, 0x27, 0, 0, 0xD);
            } else {
                prim_gradient_line(0xA0, outer - 0x1b5, outer - 0x13d, 0x27, 0, 0, 0xD);
            }
        }
        break;
    case 5:
        for (outer = g->mode_step_timer.u * 10; outer < 0xA0; outer++) {
            width = 0xA0 - outer;
            left_x = width - 0xA0;
            top_y = (width / 2) - 0x78;
            prim_gradient_line(left_x, top_y, (0x9F - width), top_y, 0, 0, 0xD);
            bottom_y = 0x27 - (width / 2);
            prim_gradient_line(left_x, bottom_y, (0x9F - width), bottom_y, 0, 0, 0xD);
            prim_gradient_line(left_x, top_y, left_x, bottom_y, 0, 0, 0xD);
            prim_gradient_line(0xA0-width, top_y, (0x9F - width), bottom_y, 0, 0, 0xD);
        }
        break;
    case 6:
        if (g->mode_step_timer.u < 0x10U) set_tarao_bg(g->mode_step_timer.u >> 1);
        else set_tarao_bg(8);
        break;
    default:
        for (outer = 0; outer < 0xF; outer++) {
            if ((outer << 5) < 0xA0) {
                for (left_x = g->mode_step_timer.u * 2; left_x < 0x20; left_x++) {
                    prim_gradient_line((outer << 5)+left_x-0xA0, -0x78, -0xA0, (outer << 5)+left_x-0x78, 0, 0, 0xD);
                }
            } else if ((outer << 5) < 0x140) {
                for (left_x = g->mode_step_timer.u * 2; left_x < 0x20; left_x++) {
                    prim_gradient_line((outer << 5)+left_x-0xA0, -0x78, (outer << 5)+left_x-0x140, 0x28, 0, 0, 0xD);
                }
            } else {
                for (left_x = g->mode_step_timer.u * 2; left_x < 0x20; left_x++) {
                    prim_gradient_line(0xA0, (outer << 5)+left_x-0x1B8, (outer << 5)+left_x-0x140, 0x28, 0, 0, 0xD);
                }
            }
        }
        break;
    }
    g->mode_step_timer.u++;
    if (g->mode_step_timer.u >= 16U) {
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

    p = (u8 *)GetWorkBase(0x14, draw_buffer);
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
    if (draw_buffer == 0) {
        *(s16 *)(p + 0xA) += 0xF0;
    }
    AddPrim(D_800E8CA0 + (draw_buffer << 10) + arg6 * 4, p);
}

void set_tarao_rect(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, u8 arg5, u8 arg6, u8 arg7) {
    u8 *p;

    p = (u8 *)GetWorkBase(0x10, draw_buffer);
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
    if (draw_buffer == 0) {
        *(s16 *)(p + 0xA) += 0xF0;
    }
    AddPrim(D_800E8CA0 + (draw_buffer << 10) + arg4 * 4, (s32)p);
}

void normal_date_bg_out(void) {
    if (func_8007D8AC() == 1) {
        mode_step_next();
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

    dec_bg_brightness -= 8;
    if ((u8)dec_bg_brightness >= 0x81U) {
        dec_bg_brightness = 0;
    }
    if (game_state.mode_step_timer.u == 0) {
        D_80122D3C = (u8) game_state.month % 7U;
    }
    if (D_80122D3C == 6) {
        D_80122CF8 = 0;
    }
    switch (D_80122D3C) {
    case 1:
        limit = (s16)(((WordView *)&game_state.mode_step_timer)->u * 5);
        if (limit > 0x50) limit = 0x50;
        for (outer=0; outer<limit; outer++) {
            prim_gradient_line(-0xA0, outer-0x78, 0xA0, outer-0x78, 0, 0, 6);
            prim_gradient_line(-0xA0, 0x27-outer, 0xA0, 0x27-outer, 0, 0, 6);
        }
        break;
    case 2:
        limit = (s16) game_state.mode_step_timer.h[0];
        if (limit >= 0x11) {
            limit = 0x10;
        }
        outer = 0;
        if (limit > 0) {
            do {
                inner = 0;
                do {
                line_y = (inner * 0x10 - outer) - 0x69;
                prim_gradient_line(-0xA0, line_y, 0xA0, line_y, 0, 0, 6);
                inner += 1;
                } while (inner < 0xA);
                outer += 1;
            } while (outer < limit);
        }
        break;
    case 3:
        limit = (s16)(game_state.mode_step_timer.u << 5);
        if (limit >= 0x1E1) {
            limit = 0x1E0;
        }
        outer = 0;
        if (limit > 0) {
            do {

                if (outer >= 0x141) {
                    prim_gradient_line(((0x1DF - outer) - 0xA0), -0x78, -0xA0, ((0x1DF - outer) - 0x78), 0, 0, 0xD);
                } else if (outer >= 0xA1) {

                    prim_gradient_line(((0x1DF - outer) - 0xA0), -0x78, ((0x1DF - outer) - 0x140), 0x27, 0, 0, 0xD);
                } else {

                    prim_gradient_line(0xA0, ((0x1DF - outer) - 0x1B8), ((0x1DF - outer) - 0x140), 0x27, 0, 0, 0xD);
                }
                outer += 1;
            } while (outer < limit);
        }
        break;
    case 4:
        limit = (s16)(game_state.mode_step_timer.u << 6);
        if (limit >= 0x1E1) {
            limit = 0x1E0;
        }
        outer = 0;
        if (limit > 0) {
            do {

                if (outer >= 0x141) {
                    prim_gradient_line(((0x1DF - outer) - 0xA0), -0x78, -0xA0, ((0x1DF - outer) - 0x78), 0, 0, 0xD);
                } else if (outer >= 0xA1) {

                    prim_gradient_line(((0x1DF - outer) - 0xA0), -0x78, ((0x1DF - outer) - 0x140), 0x27, 0, 0, 0xD);
                } else {

                    prim_gradient_line(0xA0, ((0x1DF - outer) - 0x1B8), ((0x1DF - outer) - 0x140), 0x27, 0, 0, 0xD);
                }
                outer += 4;
            } while (outer < limit);
            outer = 0;
        }
        if (limit > 0) {
            do {

                if (outer >= 0x141) {
                    prim_gradient_line(((0x1DF - outer) - 0x9F), -0x78, -0xA0, ((0x1DF - outer) - 0x77), 0, 0, 0xD);
                } else if (outer >= 0xA1) {

                    prim_gradient_line(((0x1DF - outer) - 0x9F), -0x78, ((0x1DF - outer) - 0x13F), 0x27, 0, 0, 0xD);
                } else {

                    prim_gradient_line(0xA0, ((0x1DF - outer) - 0x1B7), ((0x1DF - outer) - 0x13F), 0x27, 0, 0, 0xD);
                }
                outer += 4;
            } while (outer < limit);
            outer = 0;
        }
        limit = (s16)((game_state.mode_step_timer.u << 6) - 0x1E0);
        if (limit >= 0x1E1) {
            limit = 0x1E0;
        }
        if (limit > 0) {
            do {

                if (outer >= 0x141) {
                    prim_gradient_line(((0x1DF - outer) - 0x9E), -0x78, -0xA0, ((0x1DF - outer) - 0x76), 0, 0, 0xD);
                } else if (outer >= 0xA1) {

                    prim_gradient_line(((0x1DF - outer) - 0x9E), -0x78, ((0x1DF - outer) - 0x13E), 0x27, 0, 0, 0xD);
                } else {

                    prim_gradient_line(0xA0, ((0x1DF - outer) - 0x1B6), ((0x1DF - outer) - 0x13E), 0x27, 0, 0, 0xD);
                }
                outer += 4;
            } while (outer < limit);
            outer = 0;
        }
        if (limit > 0) {
            do {

                if (outer >= 0x141) {
                    prim_gradient_line(((0x1DF - outer) - 0x9D), -0x78, -0xA0, ((0x1DF - outer) - 0x75), 0, 0, 0xD);
                } else if (outer >= 0xA1) {

                    prim_gradient_line(((0x1DF - outer) - 0x9D), -0x78, ((0x1DF - outer) - 0x13D), 0x27, 0, 0, 0xD);
                } else {

                    prim_gradient_line(0xA0, ((0x1DF - outer) - 0x1B5), ((0x1DF - outer) - 0x13D), 0x27, 0, 0, 0xD);
                }
                outer += 4;
            } while (outer < limit);
        }
        break;
    case 5:
        limit = (s16)(game_state.mode_step_timer.u * 0xA);
        if (limit >= 0xA1) {
            limit = 0xA0;
        }
        outer = 0;
        if (limit > 0) {
            do {
                inner = outer - 0xA0;
                coordinate = outer / 2;
                line_y = coordinate - 0x78;
                prim_gradient_line(inner, line_y, (0x9F - outer), line_y, 0, 0, 0xD);
                bottom_y = 0x27 - coordinate;
                prim_gradient_line(inner, bottom_y, (0x9F - outer), bottom_y, 0, 0, 0xD);
                prim_gradient_line(inner, line_y, inner, bottom_y, 0, 0, 0xD);
                prim_gradient_line((0xA0 - outer), line_y, (0x9F - outer), bottom_y, 0, 0, 0xD);
                outer += 1;
            } while (outer < limit);
        }
        break;
    case 6:
        if ((u32) game_state.mode_step_timer.u < 0x10U) {
            set_tarao_bg(8 - ((u32) game_state.mode_step_timer.u >> 1));
        } else {
            set_tarao_bg(0);
        }
        break;
    default:
        limit = (s16)(game_state.mode_step_timer.u * 2);
        if (limit >= 0x21) {
            limit = 0x20;
        }
        for (outer = 0; outer < 15; outer++) {
            inner = 0;
            if ((outer << 5) < 0xA0) {
                coordinate = (outer << 5) + 0x1F;
                if (limit > 0) {
                    do {

                        prim_gradient_line(((coordinate - inner) - 0xA0), -0x78, -0xA0, ((coordinate - inner) - 0x78), 0, 0, 0xD);
                        inner += 1;
                    } while (inner < limit);
                }
            } else if ((outer << 5) < 0x140) {
                coordinate = (outer << 5) + 0x1F;
                if (limit > 0) {
                    do {

                        prim_gradient_line(((coordinate - inner) - 0xA0), -0x78, ((coordinate - inner) - 0x140), 0x28, 0, 0, 0xD);
                        inner += 1;
                    } while (inner < limit);
                }
            } else {
                coordinate = (outer << 5) + 0x1F;
                if (limit > 0) {
                    do {

                        prim_gradient_line(0xA0, ((coordinate - inner) - 0x1B8), ((coordinate - inner) - 0x140), 0x28, 0, 0, 0xD);
                        inner += 1;
                    } while (inner < limit);
                }
            }
        }
        break;
    }
    game_state.mode_step_timer.u++;
    if (game_state.mode_step_timer.u >= 0x12U) {
        D_80122CF8 = 0;
        return 1;
    }
    return 0;
}

/* Slide the girl's portrait in */
void normal_date_girl_in(void) {
    switch (game_state.mode_step_timer.u) {
    case 0:
        normal_date_girl_in_init();
        break;
    case 1:
        normal_date_girl_in_main();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    func_80057D28(1);
}

s32 normal_date_girl_in_init(void) {
    actor_pool[0x843] = 0;
    sprite_quads[0].tint_r = sprite_quads[0].tint_g = sprite_quads[0].tint_b = 0;
    sprite_quads[1].tint_r = sprite_quads[1].tint_g = sprite_quads[1].tint_b = 0;
    D_800B5940 = 0;
    D_80122CF8 = 0;
    D_80122CF4 = 1;
    game_state.mode_step_timer.w += 1;
}

s32 normal_date_girl_in_main(void) {
    if (D_800B5940 < 0x80) {
        D_800B5940 += 8;
    } else {
        mode_step_next();
        D_80122CF8 = 1;
        sprite_quads[0].flags &= 0x7FFFFFFF;
        sprite_quads[1].flags &= 0x7FFFFFFF;
        return 0;
    }
    sprite_quads[0].tint_r = sprite_quads[0].tint_g = sprite_quads[0].tint_b = sprite_quads[1].tint_r = sprite_quads[1].tint_g = sprite_quads[1].tint_b = D_800B5940;
    actor_pool[0x843] = D_800B5940;
}

s32 normal_date_girl_suddenin(void) {
    D_80122CF4 = 1;
    actor_pool[0x843] = 0x80;
    sprite_quads[0].tint_r = sprite_quads[0].tint_g = sprite_quads[0].tint_b = 0x80;
    sprite_quads[1].tint_r = sprite_quads[1].tint_g = sprite_quads[1].tint_b = 0x80;
    mode_step_next();
}

/* Slide the girl's portrait out */
void normal_date_girl_out(void) {
    switch (game_state.mode_step_timer.u) {
    case 0:
        normal_date_girl_out_init();
        break;
    case 1:
        normal_date_girl_out_main();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    func_80057D28(1);
}

void normal_date_girl_out_init(void) {
    D_800B5940 = 0x80;
    D_80122CF8 = 0;
    game_state.mode_step_timer.w = game_state.mode_step_timer.w + 1;
}

s32 normal_date_girl_out_main(void) {
    if (D_800B5940 >= 8) {
        D_800B5940 -= 8;
    } else {
        actor_pool[0x83F] &= ~0x80;
        actor_pool[0x7FB] &= ~0x80;
        mode_step_next();
        sprite_quads[0].flags |= 0x80000000;
        sprite_quads[1].flags |= 0x80000000;
        sprite_quads[0].tint_r = sprite_quads[0].tint_g = sprite_quads[0].tint_b = 0x80;
        sprite_quads[1].tint_r = sprite_quads[1].tint_g = sprite_quads[1].tint_b = 0x80;
        D_80122CF4 = 0;
        D_80122CF8 = 1;
        return 0;
    }
    sprite_quads[0].tint_r = sprite_quads[0].tint_g = sprite_quads[0].tint_b = sprite_quads[1].tint_r = sprite_quads[1].tint_g = sprite_quads[1].tint_b = D_800B5940;
    actor_pool[0x843] = D_800B5940;
}

s32 normal_date_girl_suddenout(void) {
    actor_pool[0x83F] &= ~0x80;
    actor_pool[0x7FB] &= ~0x80;
    sprite_quads[0].flags |= 0x80000000;
    sprite_quads[1].flags |= 0x80000000;
    sprite_quads[0].tint_r = sprite_quads[0].tint_g = sprite_quads[0].tint_b = 0x80;
    sprite_quads[1].tint_r = sprite_quads[1].tint_g = sprite_quads[1].tint_b = 0x80;
    D_80122CF4 = 0;
    D_80122CF8 = 1;
    mode_step_next();
}

/* Fade the scene background in */
s32 normal_date_bg_fadein(void) {
    D_80122CF8 = 1;
    dec_bg_brightness += 4;
    if (dec_bg_brightness >= 0x80) {
        dec_bg_brightness = 0x80;
    }
    if (dec_bg_brightness >= 0x7D) {
        dec_bg_brightness = 0x80;
        mode_step_next();
    }
}

/* s32 without a return value: the last chain value is then kept in v0 (andi v0,a0,0xff). T-11590 */
/* Fade background and portrait in together */
s32 normal_date_bggirl_fadein(void) {
    u8 x;

    actor_pool[0x83F] |= 0x80;
    normal_date_bg_fadein();
    x = dec_bg_brightness;
    actor_pool[0x843] = x;
    sprite_quads[0].flags &= 0x7FFFFFFF;
    sprite_quads[1].flags &= 0x7FFFFFFF;
    sprite_quads[0].tint_r = sprite_quads[0].tint_g = sprite_quads[0].tint_b = x;
    sprite_quads[1].tint_r = sprite_quads[1].tint_g = sprite_quads[1].tint_b = x;
    D_80122CF4 = 1;
}

/* Fade the scene background out */
void normal_date_bg_fadeout(void) {
    D_80122CF8 = 1;
    dec_bg_brightness -= 4;
    if (dec_bg_brightness >= 0x81) {
        dec_bg_brightness = 0;
    }
    if (dec_bg_brightness < 4) {
        D_80122CF8 = 0;
        dec_bg_brightness = 0;
        mode_step_next();
    }
}

/* s32 without a return value, as normal_date_bggirl_fadein. T-11590 */
/* Fade background and portrait out together */
s32 normal_date_bggirl_fadeout(void) {
    u8 x;

    if (dec_bg_brightness < 8) {
        D_80122CF4 = 0;
        D_80122CF8 = 0;
        actor_pool[0x83F] &= ~0x80;
        actor_pool[0x7FB] &= ~0x80;
        sprite_quads[0].tint_r = sprite_quads[0].tint_g = sprite_quads[0].tint_b = 0x80;
        sprite_quads[1].tint_r = sprite_quads[1].tint_g = sprite_quads[1].tint_b = 0x80;
        sprite_quads[0].flags |= 0x80000000;
        sprite_quads[1].flags |= 0x80000000;
    }
    normal_date_bg_fadeout();
    x = dec_bg_brightness;
    actor_pool[0x843] = x;
    sprite_quads[0].tint_r = sprite_quads[0].tint_g = sprite_quads[0].tint_b = x;
    sprite_quads[1].tint_r = sprite_quads[1].tint_g = sprite_quads[1].tint_b = x;
}

void normal_date_move_place(void) {
    switch (game_state.mode_step_timer.u) {
    case 0:
        normal_date_move_place_init();
        return;
    case 1:
        normal_date_move_place_main();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void normal_date_move_place_init(void) {
    k_reset(1);
    D_800B5940 = 0x80;
    D_80122CF8 = 0;
    game_state.mode_step_timer.w = game_state.mode_step_timer.w + 1;
    func_80058398(1);
}

s32 normal_date_move_place_main(void) {
    if (D_800B5940 >= 8) {
        D_800B5940 -= 8;
    } else {
        func_80058398(1);
        dec_bg_show_switch(1);
        mode_step_next();
        sprite_quads[0].flags |= 0x80000000;
        sprite_quads[1].flags |= 0x80000000;
        sprite_quads[0].tint_r = sprite_quads[0].tint_g = sprite_quads[0].tint_b = 0x80;
        sprite_quads[1].tint_r = sprite_quads[1].tint_g = sprite_quads[1].tint_b = 0x80;
        D_80122CF8 = 1;
        return 0;
    }
    sprite_quads[0].tint_r = sprite_quads[0].tint_g = sprite_quads[0].tint_b = sprite_quads[1].tint_r = sprite_quads[1].tint_g = sprite_quads[1].tint_b = D_800B5940;
    func_80058398(1);
}

void place_init(void) {
    k_reset(0);
    message_window_set_title(D_800CA19C);
    mode_step_next();
}

void place_init2(void) {
    k_reset(0);
    message_window_set_title(D_800CA1DC);
    mode_step_next();
}

void change_dec_bg(void) {
    dec_bg_show_switch(1);
    mode_step_next();
}

s32 dec_init(void) {
    if ((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000 || (((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000) {
        dec_bg_lba_cache[2] = 0;
        dec_bg_lba_cache[3] = 0;
        mode_step_next();
        game_state.mode_step -= 3;
        return 0;
    }
    dec_bg_decode_to_vram(1);
    mode_step_next();
}

/* Background read helper: starts the read of the scene background */
void bg_read_sub2(s32 arg0) {
    s32 t;

    D_800B3D60 = 0;
    t = dec_bg_cd_read(arg0, 1);
    if (t == D_800B5939) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
    } else if (t == 1 - D_800B5939) {
        mode_step_next();
        mode_step_next();
    }
}

/* FAKE: the chained next-index assignment keeps a0 across the call, and the
 * unsigned comparison keeps the target branch operand order. T-9720. */
void check_k_scroll(void) {
    s32 sp38;
    TextEntry *sp48;
    s16 temp_v1;
    s32 temp_a0;
    s32 temp_v0;
    s32 var_a0;
    s32 var_s0;
    u32 temp_s2;
    TextEntry *temp_v0_2;
    TextEntry *temp_v0_3;

    get_ksys();
    temp_v0 = get_now_k();
    temp_s2 = temp_v0 & 0xFF;
    sp48 = (TextEntry *) get_k_work(temp_v0);
    temp_a0 = (var_a0 = temp_s2 + 1);
    sp38 = temp_a0;
    get_k_work(temp_a0);
    if (sp48->y >= 0x60) {
        var_a0 = sp38;
        if (D_800CA2BC != 0) {
            var_s0 = 1;
            if (temp_s2 >= 2U) {
                do {
                    temp_v0_2 = (TextEntry *) get_k_work(var_s0);
                    temp_v1 = temp_v0_2->y;
                    if ((u32) temp_v1 == 0x30U) {
                        temp_v0_2->active = 0;
                    } else {
                        temp_v0_2->y = (s16) (temp_v1 - 0x10);
                    }
                    var_s0 = (s32) ((s16) (var_s0 + 1));
                }
                while (((u32) var_s0) < temp_s2);
            }
            D_800CA2BC = 0;
            sp48->y = 0x50;
            sp48->active = 1;
            var_a0 = sp38;
        }
    }
    temp_v0_3 = (TextEntry *) get_k_work(var_a0);
    if (temp_v0_3->y >= 0x60) {
        D_800CA2BC = 1;
        temp_v0_3->active = 0;
    }
}

void wait_sub_sub(s16 arg0) {
    s32 t = game_state.mode_step_timer.w + 1;
    game_state.mode_step_timer.w = t;
    if ((u32)t >= (u32)arg0) {
        mode_step_next();
    }
}

/* Matched: T-24021. The two incremented-Y calls need an early argument definition. */
s32 talk_speak_part(arg0, arg1, arg2, arg3, arg4, arg5, arg6)
u8 *arg0;
u8 arg1;
u8 arg2;
TalkVoiceRef ***arg3;
u16 arg4;
u16 arg5;
u8 arg6;
{
    s16 temp_s1;
    s16 var_v1;
    TalkWork *dialogue;
    s16 sp286;
    s16 var_a1_2;
    u8 *sp27C;
    u8 *p;
    u8 sp7C[0x200];
    u8 sp74[8];
    u8 sp68[12];
    TalkSelectors sp58;
    /* FAKE: reuse the former padding word for the call argument's live range;
     * the observed frame remains 0x290 bytes. T-24021. */
    s32 render_x;

    dialogue = (TalkWork *)talk_work;
    sp58 = D_800CA36C;
    /* FAKE: line grouping preserves as1 scheduling; T-10730. */
    if (arg1 != 0xFF) { strcpy(sp7C, arg0); }
    dialogue->unk_20 = 0;
    D_800CA2A8 = 0;
    temp_s1 = (dialogue->unk_58 * 0x10) + 0x30;
    /* FAKE: byte view shares the 0xFF constant in v1; T-10730. */
    switch (*(u8 *)&arg1) {
    case 0x7:
        if (((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) && (*(u8 *)&game_state.voice_mode != 0)) {
            if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                cd_audio_request_set(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[game_state.current_girl]);
                audio_command(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(cd_audio_state() & 4) && dialogue->unk_14 == 0) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {
                        return 0;
                    }
                }
            } else {
                D_800CA2AC = 0;
                func_8007B640();
                audio_command(0x74);
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
            talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C[5], 0, "", "", 0);
        } else if (sp7C[0] == 0x2D) {
            strncpy(sp74, &sp7C[1], 6);
            sp74[6] = 0;
            sprintf(sp68, "%s ", sp74);
            set_kanji_string(-0x80, temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C[7], 0, "", "", 0);
        } else if (sp7C[0] == 0x2B) {
            strncpy(sp74, &sp7C[1], 2);
            sp74[2] = 0;
            sprintf(sp68, "%s ", sp74);
            set_kanji_string(-0x70, temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C[3], 0, "", "", 0);
        } else if (sp7C[0] == 0x3C) {
            strncpy(sp74, &sp7C[1], 4);
            sp74[4] = 0;
            sprintf(sp68, "%s ", sp74);
            set_kanji_string(-0x78, temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "（", 0);
            talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C[5], 0, "）", "", 0);
        } else if ((render_x = strcmp(sp7C, "そして…")) == 0) {
            /* FAKE: the zero mask removes the strcmp-result dependency while
             * retaining an early definition of a0, ahead of Y narrowing and
             * outgoing string stores. A literal -40 schedules too late. T-24021. */
            render_x = (render_x & 0) - 40;
            temp_s1 += 0x10;
            talk_render(render_x, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        } else if ((strcmp(sp7C, "ガチャ") == 0) || (strcmp(sp7C, "カチャ") == 0) || (strcmp(sp7C, "ガラッ") == 0) || (strcmp(sp7C, "ごそごそ") == 0)) {
            talk_render(-0x80, (s32) temp_s1, 2, (s32) sp7C, 0, "", "", 0);
        } else if ((strcmp(sp7C, "ジロジロ") == 0) || (strcmp(sp7C, "ドン") == 0)) {
            talk_render(-0x80, (s32) temp_s1, 1, (s32) sp7C, 0, "", "", 0);
        } else if ((render_x = strcmp(sp7C, "ピンポーン")) == 0) {
            /* FAKE: preserve the same early argument definition as above. T-24021. */
            render_x = (render_x & 0) - 40;
            temp_s1 += 0x10;
            talk_render(render_x, (s32) temp_s1, 2, (s32) sp7C, 0, "", "", 0);
        } else {
            talk_render(-0x80, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        }
        dialogue->unk_14 = 1;
        break;
    case 0x10:
        talk_render(-0x80, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        dialogue->unk_30 = 1;
        break;
    case 0x11:
        talk_render(-0x48, (s32) D_800CA2C0, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        dialogue->unk_30 = 1;
        break;
    case 0x9:
        if (sp7C[0] != 0) {
            if (strcmp(sp7C, "ドン") == 0) {
                talk_render(-0x80, (s32) temp_s1, 1, (s32) sp7C, 0, "", "", 0);
            }
            talk_render(-0x80, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        }
        break;
    case 0x0:
        sp286 = strlen(sp7C);
        if ((sp7C[sp286 - 1] == 0x6E) && (sp7C[sp286 - 2] == 0x6E)) {
            if (dialogue->unk_0C == 0) {
                sp7C[sp286 - 2] = 0;
                sp286 = strlen(name_code_family);
                sprintf(sp68, "%s ", name_code_family);
                set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
                set_kanji_string(-0x56, temp_s1, arg6, "『 ", 0);
                talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_14 = 1;
            }
        } else {
            sp286 = strlen(name_code_family);
            sprintf(sp68, "%s ", name_code_family);
            set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "『 ", 0);
            talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0xE:
        sp286 = strlen(name_code_family);
        sprintf(sp68, "%s ", name_code_family);
        set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
        set_kanji_string(-0x56, temp_s1, arg6, "（ ", 0);
        talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "）", "", 0);
        break;
    case 0x1:
        sp286 = strlen(sp7C);
        if (((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) && ((*(u8 *)&game_state.voice_mode != 0) && ((sp7C[sp286 - 1] != 0x6E) || (sp7C[sp286 - 2] != 0x6E) || (dialogue->unk_0C == 0)))) {
                if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                    cd_audio_request_set(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[game_state.current_girl]);
                    audio_command(0x300);
                }
                if (D_800CA2AC < 0x400) {
                    if (!(cd_audio_state() & 4) && dialogue->unk_14 == 0) {
                        D_800CA2A8 = 1;
                        func_80081128();
                        if (D_800CA2A8 == 1) {
                            return 0;
                        }
                    }
                } else {
                    D_800CA2AC = 0;
                    func_8007B640();
                    audio_command(0x74);
                    return 0x64;
                }
                dialogue->unk_20 = 1;
                D_800CA2AC = 0;
        }
        p = &sp7C[sp286];
        if ((p[-1] == 0x6E) && (p[-2] == 0x6E)) {
            if (dialogue->unk_0C == 0) {
                p[-2] = 0U;
                sp286 = strlen(name_code_girl_short);
                sprintf(sp68, "%s ", name_code_girl_short);
                set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
                set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
                talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_14 = 1;
            }
        } else {
            sp286 = strlen(name_code_girl_short);
            sprintf(sp68, "%s ", name_code_girl_short);
            set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
            talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0xF:
        if (((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) && (*(u8 *)&game_state.voice_mode != 0)) {
            if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                cd_audio_request_set(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[game_state.current_girl]);
                audio_command(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(cd_audio_state() & 4) && dialogue->unk_14 == 0) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {
                        return 0;
                    }
                }
            } else {
                D_800CA2AC = 0;
                func_8007B640();
                audio_command(0x74);
                return 0x64;
            }
            dialogue->unk_20 = 1;
            D_800CA2AC = 0;
        }
        sp286 = strlen(name_code_girl_short);
        sprintf(sp68, "%s ", name_code_girl_short);
        set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
        set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
        talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        dialogue->unk_48 = 1;
        break;
    case 0xC:
        D_800CA2C4 = 0;
        sp286 = strlen(sp7C);
        if (((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) && (*(u8 *)&game_state.voice_mode != 0)) {
            if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                cd_audio_request_set(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[game_state.current_girl]);
                audio_command(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(cd_audio_state() & 4) && dialogue->unk_14 == 0) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {
                        return 0;
                    }
                }
            } else {
                D_800CA2AC = 0;
                func_8007B640();
                audio_command(0x74);
                return 0x64;
            }
            dialogue->unk_20 = 1;
            D_800CA2AC = 0;
        }
        p = &sp7C[sp286];
        if ((p[-1] == 0x6E) && (p[-2] == 0x6E)) {
            if (dialogue->unk_0C == 0) {
                p[-2] = 0U;
                sp286 = strlen(name_code_girl_short);
                sprintf(sp68, "%s ", name_code_girl_short);
                set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
                set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
                talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_30 = 1;
            }
        } else {
            sp286 = strlen(name_code_girl_short);
            sprintf(sp68, "%s ", name_code_girl_short);
            set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
            talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_30 = 1;
        }
        break;
    case 0xD:
        sp286 = strlen(sp7C);
        if (((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) && (*(u8 *)&game_state.voice_mode != 0) && (D_800CA2C4 == 0)) {
            if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                cd_audio_request_set(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[game_state.current_girl]);
                audio_command(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(cd_audio_state() & 4) && dialogue->unk_14 == 0) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {
                        return 0;
                    }
                }
            } else {
                D_800CA2AC = 0;
                func_8007B640();
                audio_command(0x74);
                return 0x64;
            }
            dialogue->unk_20 = 1;
            D_800CA2AC = 0;
        }
        p = &sp7C[sp286];
        if ((p[-1] == 0x6E) && (p[-2] == 0x6E)) {
            if (dialogue->unk_0C == 0) {
                p[-2] = 0U;
                talk_render(-0x48, (s32) D_800CA2C0, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_14 = 1;
            }
        } else {
            set_kanji_string(-0x56, temp_s1, arg6, "　 ", 0);
            talk_render(-0x48, (s32) D_800CA2C0, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0x12:
        strlen(sp7C);
        if ((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) { /* FAKE: same-line branches preserve scheduling, T-10730. */ if (*(u8 *)&game_state.voice_mode != 0) {

                if (D_800CA2C4 == 0) {
                    if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                            cd_audio_request_set(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[game_state.current_girl]);
                        audio_command(0x300);
                    }
                    if (D_800CA2AC < 0x400) {
                        if (!(cd_audio_state() & 4) && dialogue->unk_14 == 0) {
                            D_800CA2A8 = 1;
                            func_80081128();
                            if (D_800CA2A8 == 1) {
                                return 0;
                            }
                        }
                    } else {
                        D_800CA2AC = 0;
                        func_8007B640();
                        audio_command(0x74);
                        return 0x64;
                    }
                    dialogue->unk_20 = 1;
                    D_800CA2AC = 0;
                }
            }
        }
        set_kanji_string(-0x56, temp_s1, arg6, "　 ", 0);
        talk_render(-0x48, (s32) D_800CA2C0, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        dialogue->unk_30 = 1;
        break;
    case 0xB:
        sp286 = strlen(sp7C);
        if ((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) { /* FAKE: same-line branches preserve scheduling, T-10730. */ if (*(u8 *)&game_state.voice_mode != 0) {
                if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                    cd_audio_request_set(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, 0);
                    audio_command(0x300);
                }
                if (D_800CA2AC < 0x400) {
                    if (!(cd_audio_state() & 4) && dialogue->unk_14 == 0) {
                        D_800CA2A8 = 1;
                        func_80081128();
                        if (D_800CA2A8 == 1) {
                            return 0;
                        }
                    }
                } else {
                    D_800CA2AC = 0;
                    func_8007B640();
                    audio_command(0x74);
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
                talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_14 = 1;
            }
        } else {
            set_kanji_string((s16) (-0x68 - ((s16) strlen("藤崎") * 4)), temp_s1, arg6, "藤崎", 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0xA:
        sp286 = strlen(sp7C);
        if ((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) { /* FAKE: same-line branches preserve scheduling, T-10730. */ if (*(u8 *)&game_state.voice_mode != 0) {
                if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                    cd_audio_request_set(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, 0xD);
                    audio_command(0x300);
                }
                if (D_800CA2AC < 0x400) {
                    if (!(cd_audio_state() & 4) && dialogue->unk_14 == 0) {
                        D_800CA2A8 = 1;
                        func_80081128();
                        if (D_800CA2A8 == 1) {
                            return 0;
                        }
                    }
                } else {
                    D_800CA2AC = 0;
                    func_8007B640();
                    audio_command(0x74);
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
                talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_14 = 1;
            }
        } else {
            set_kanji_string((s16) (-0x68 - ((s16) strlen("好雄") * 4)), temp_s1, arg6, "好雄", 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0x4:
    case 0x6:
        sp286 = strlen(name_code_family);
        sprintf(sp68, "%s ", name_code_family);
        set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
        set_kanji_string(-0x56, temp_s1, arg6, "（ ", 0);
        talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "）", "", 0);
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
        k_disp_start(talk_render(-0x72, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "』", "『", 1));
        break;
    case 0x8:
        D_800CA2A4[0] = 0;
        D_800CA2A4[1] = 1;
        k_disp_start(talk_render(-0x72, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "』", "『", 1));
        break;
    case 0x13:
        D_800CA2A4[0] = 0;
        D_800CA2A4[1] = 1;
        k_disp_start(talk_render(-0x72, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "）", "（", 1));
        break;
    case 0x2:
    case 0x5:
        if ((((s32 *)arg3[arg4][arg5])[dialogue->unk_0C * 2] != 0) && (*(u8 *)&game_state.voice_mode != 0)) {
            if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                cd_audio_request_set(((s32 *)arg3[arg4][arg5])[dialogue->unk_0C * 2], ((s32 *)arg3[arg4][arg5])[dialogue->unk_0C * 2 + 1], sp58.value[game_state.current_girl]);
                audio_command(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(cd_audio_state() & 4) && dialogue->unk_14 == 0) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {
                        return 0;
                    }
                }
            } else {
                D_800CA2AC = 0;
                func_8007B640();
                audio_command(0x74);
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
            sp286 = strlen(name_code_girl_short);
            sprintf(sp68, "%s ", name_code_girl_short);
            set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            talk_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp27C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0xFF:
        mode_step_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    return 0;
}

void func_80081128(void) {
    if (D_800CA2AC > 0x200) {
        if (game_state.pad_pressed & 0x600060) {
            audio_command(0xB4);
            D_800CA2A8 = 0;
            D_800CA2AC = 0;
            D_800CA2C4 = 1;
        }
    }
}

/* Split a script string into lines and queue them with name code expansion */
s32 talk_render(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
    s16 arg0;
    s16 arg1;
    u8 arg2;
    u8 *arg3;
    s32 arg4;
    u8 *arg5;
    u8 *arg6;
    u8 arg7;
{
    s16 i;
    s16 length;
    s16 scan;
    s16 copy;
    s32 final_y;
    u8 token;
    s16 in_index;
    s16 row;
    u8 line[64];
    u8 name[24];
    u8 text[512];
    s16 out_index;
    u8 open[3];
    u8 punct0[3] = "…";
    u8 punct1[3] = "‥";
    u8 punct2[3] = "「";
    u8 punct3[3] = "。";
    u8 punct4[3] = "、";

    if (*arg3 == 0) {
        return 0;
    }
    strcpy(open, "（");
    strcpy(text, arg3);
    D_80122D1C = 1;
    i = strlen(text);
    for (scan = 0; scan < i; scan += 2) {
        if (text[scan] == 'm' || text[scan] == 'M' || text[scan] == 'A' || text[scan] == 'W' || text[scan] == 'O' || text[scan] == 'F' || text[scan] == 'Y' || text[scan] == 'S' || text[scan] == 'P' || text[scan] == 'p' || text[scan] == 'G' || text[scan] == ' ' || text[scan] == 'q') {
            scan--;
        } else if ((text[scan] != punct0[0] || text[scan + 1] != punct0[1]) && (text[scan] != punct1[0] || text[scan + 1] != punct1[1]) && (text[scan] != punct2[0] || text[scan + 1] != punct2[1]) && (text[scan] != punct3[0] || text[scan + 1] != punct3[1]) && (text[scan] != punct4[0] || text[scan + 1] != punct4[1]) ) {
            D_80122D1C = 0;
        }
    }
    i = 0;
    row = 0;
    scan = 0;
parse_next:
    token = text[scan];
    if (token == 'm' || token == 'M' || token == 'A' || token == 'W' || token == 'O' || token == 'F' || token == 'Y' || token == 'S' || token == 'P' || token == 'p' || token == 'G') {
        scan--;
    advance_text:
        scan += 2;
        goto parse_next;
    }
    if (token == ' ' || token == 'q') {
        text[scan] = 0;
        if (arg7 != 0) {
            set_kanji_string(arg0 - 0xE, arg1 + row, arg2, arg6, arg4);
        }

        strncpy(line, (text + i), 0x40);
        length = scan - i;
        /* FAKE: grouped narrow index updates below preserve as1 scheduling. T-20007. */
        for (out_index = 0, in_index = 0; out_index < length; out_index += 2, in_index += 2) {
            if (line[out_index] == 'P') {
                copy = 0;
                if (strlen(name_code_player_form) > 0) {
                    do {
                        line[out_index + copy] = name_code_player_form[copy];
                        copy++;
                    } while (copy < strlen(name_code_player_form));
                }
                strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
                length += strlen(name_code_player_form) - 1;
                out_index += strlen(name_code_player_form); out_index -= 2; in_index--;
            }
            else if (line[out_index] == 'S') {
                copy = 0;
                if (strlen(D_800CA23C) > 0) {
                    do {
                        line[out_index + copy] = D_800CA23C[copy];
                        copy++;
                    } while (copy < strlen(D_800CA23C));
                }
                strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
                length += strlen(D_800CA23C) - 1;
                out_index += strlen(D_800CA23C); out_index -= 2; in_index--;
            }
            else if (line[out_index] == 'G') {
                copy = 0;
                if (strlen(girl_name_buf) > 0) {
                    do {
                        line[out_index + copy] = girl_name_buf[copy];
                        copy++;
                    } while (copy < strlen(girl_name_buf));
                }
                strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
                length += strlen(girl_name_buf) - 1;
                out_index += strlen(girl_name_buf); out_index -= 2; in_index--;
            }
            else if (line[out_index] == 'p') {
                copy = 0;
                if (strlen(name_code_family) > 0) {
                    do {
                        line[out_index + copy] = name_code_family[copy];
                        copy++;
                    } while (copy < strlen(name_code_family));
                }
                strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
                length += strlen(name_code_family) - 1;
                out_index += strlen(name_code_family); out_index -= 2; in_index--;
            }
            else if (line[out_index] == 'Y') {
                strcpy(name, "好雄");
                copy = 0;
                if (strlen(name) > 0) {
                    do {
                        line[out_index + copy] = name[copy];
                        copy++;
                    } while (copy < strlen(name));
                }
                strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
                length += strlen(name) - 1;
                out_index += strlen(name); out_index -= 2; in_index--;
            }
            else if (line[out_index] == 'F') {
                sprintf(name, "%s%s", name_code_family, name_code_given);
                copy = 0;
                if (strlen(name) > 0) {
                    do {
                        line[out_index + copy] = name[copy];
                        copy++;
                    } while (copy < strlen(name));
                }
                strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
                length += strlen(name) - 1;
                out_index += strlen(name); out_index -= 2; in_index--;
            }
            else if (line[out_index] == 'm') {
                strcpy(name, name_code_given);
                copy = 0;
                if (strlen(name) > 0) {
                    do {
                        line[out_index + copy] = name[copy];
                        copy++;
                    } while (copy < strlen(name));
                }
                strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
                length += strlen(name) - 1;
                out_index += strlen(name); out_index -= 2; in_index--;
            }
            else if (line[out_index] == 'O') {
                copy = 0;
                if (strlen(D_800CA248) > 0) {
                    do {
                        line[out_index + copy] = D_800CA248[copy];
                        copy++;
                    } while (copy < strlen(D_800CA248));
                }
                strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
                length += strlen(D_800CA248) - 1;
                out_index += strlen(D_800CA248); out_index -= 2; in_index--;
            }
            else if (line[out_index] == 'W') {
                copy = 0;
                if (strlen(D_800CA250) > 0) {
                    do {
                        line[out_index + copy] = D_800CA250[copy];
                        copy++;
                    } while (copy < strlen(D_800CA250));
                }
                strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
                length += strlen(D_800CA250) - 1;
                out_index += strlen(D_800CA250); out_index -= 2; in_index--;
            }
            else if (line[out_index] == 'A') {
                copy = 0;
                if (strlen(D_800CA25C) > 0) {
                    do {
                        line[out_index + copy] = D_800CA25C[copy];
                        copy++;
                    } while (copy < strlen(D_800CA25C));
                }
                strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
                length += strlen(D_800CA25C) - 1;
                out_index += strlen(D_800CA25C); out_index -= 2; in_index--;
            }
            else if (line[out_index] == 'M') {
                copy = 0;
                if (strlen(D_800CA280) > 0) {
                    do {
                        line[out_index + copy] = D_800CA280[copy];
                        copy++;
                    } while (copy < strlen(D_800CA280));
                }
                strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
                length += strlen(D_800CA280) - 1;
                out_index += strlen(D_800CA280); out_index -= 2; in_index--;
            }
        }
        if (arg7 != 0) {
            strcat(line, arg5);
        }
        if (line[0] == open[0] && line[1] == open[1]) {
            set_kanji_string(arg0 - 0xE, (s16)(arg1 + row), arg2, line, arg4);
        } else {
            set_kanji_string(arg0, (s16)(arg1 + row), arg2, line, arg4);
        }
        row += 0x10;
        i = scan + 1;
        scan--;
        goto advance_text;
    }
    if (token != 0) {
        goto advance_text;
    }
    if (arg7 != 0) {
        set_kanji_string(arg0 - 0xE, arg1 + row, arg2, arg6, arg4);
    }

    final_y = arg1 + row;
    strncpy(line, (text + i), 0x40);
    length = scan - i;
    /* FAKE: grouped narrow index updates below preserve as1 scheduling. T-20007. */
    for (out_index = 0, in_index = 0; out_index < length; out_index += 2, in_index += 2) {
        if (line[out_index] == 'P') {
            copy = 0;
            if (strlen(name_code_player_form) > 0) {
                do {
                    line[out_index + copy] = name_code_player_form[copy];
                    copy++;
                } while (copy < strlen(name_code_player_form));
            }
            strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
            length += strlen(name_code_player_form) - 1;
            out_index += strlen(name_code_player_form); out_index -= 2; in_index--;
        }
        else if (line[out_index] == 'S') {
            copy = 0;
            if (strlen(D_800CA23C) > 0) {
                do {
                    line[out_index + copy] = D_800CA23C[copy];
                    copy++;
                } while (copy < strlen(D_800CA23C));
            }
            strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
            length += strlen(D_800CA23C) - 1;
            out_index += strlen(D_800CA23C); out_index -= 2; in_index--;
        }
        else if (line[out_index] == 'G') {
            copy = 0;
            if (strlen(girl_name_buf) > 0) {
                do {
                    line[out_index + copy] = girl_name_buf[copy];
                    copy++;
                } while (copy < strlen(girl_name_buf));
            }
            strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
            length += strlen(girl_name_buf) - 1;
            out_index += strlen(girl_name_buf); out_index -= 2; in_index--;
        }
        else if (line[out_index] == 'p') {
            copy = 0;
            if (strlen(name_code_family) > 0) {
                do {
                    line[out_index + copy] = name_code_family[copy];
                    copy++;
                } while (copy < strlen(name_code_family));
            }
            strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
            length += strlen(name_code_family) - 1;
            out_index += strlen(name_code_family); out_index -= 2; in_index--;
        }
        else if (line[out_index] == 'Y') {
            strcpy(name, "好雄");
            copy = 0;
            if (strlen(name) > 0) {
                do {
                    line[out_index + copy] = name[copy];
                    copy++;
                } while (copy < strlen(name));
            }
            strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
            length += strlen(name) - 1;
            out_index += strlen(name); out_index -= 2; in_index--;
        }
        else if (line[out_index] == 'F') {
            sprintf(name, "%s%s", name_code_family, name_code_given);
            copy = 0;
            if (strlen(name) > 0) {
                do {
                    line[out_index + copy] = name[copy];
                    copy++;
                } while (copy < strlen(name));
            }
            strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
            length += strlen(name) - 1;
            out_index += strlen(name); out_index -= 2; in_index--;
        }
        else if (line[out_index] == 'm') {
            strcpy(name, name_code_given);
            copy = 0;
            if (strlen(name) > 0) {
                do {
                    line[out_index + copy] = name[copy];
                    copy++;
                } while (copy < strlen(name));
            }
            strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
            length += strlen(name) - 1;
            out_index += strlen(name); out_index -= 2; in_index--;
        }
        else if (line[out_index] == 'O') {
            copy = 0;
            if (strlen(D_800CA248) > 0) {
                do {
                    line[out_index + copy] = D_800CA248[copy];
                    copy++;
                } while (copy < strlen(D_800CA248));
            }
            strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
            length += strlen(D_800CA248) - 1;
            out_index += strlen(D_800CA248); out_index -= 2; in_index--;
        }
        else if (line[out_index] == 'W') {
            copy = 0;
            if (strlen(D_800CA250) > 0) {
                do {
                    line[out_index + copy] = D_800CA250[copy];
                    copy++;
                } while (copy < strlen(D_800CA250));
            }
            strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
            length += strlen(D_800CA250) - 1;
            out_index += strlen(D_800CA250); out_index -= 2; in_index--;
        }
        else if (line[out_index] == 'A') {
            copy = 0;
            if (strlen(D_800CA25C) > 0) {
                do {
                    line[out_index + copy] = D_800CA25C[copy];
                    copy++;
                } while (copy < strlen(D_800CA25C));
            }
            strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
            length += strlen(D_800CA25C) - 1;
            out_index += strlen(D_800CA25C); out_index -= 2; in_index--;
        }
        else if (line[out_index] == 'M') {
            copy = 0;
            if (strlen(D_800CA280) > 0) {
                do {
                    line[out_index + copy] = D_800CA280[copy];
                    copy++;
                } while (copy < strlen(D_800CA280));
            }
            strncpy(&line[out_index + copy], (text + i) + in_index + 1, -out_index - copy + 0x40);
            length += strlen(D_800CA280) - 1;
            out_index += strlen(D_800CA280); out_index -= 2; in_index--;
        }
    }
    if (*arg5 != 0) {
        strcat(line, arg5);
    }
    D_800CA2C0 = final_y + 0x10;
    if (line[0] == open[0] && line[1] == open[1]) {
        final_y = set_kanji_string(arg0 - 0xE, (s16)(arg1 + row), arg2, line, arg4);
    } else {
        final_y = set_kanji_string(arg0, (s16)(arg1 + row), arg2, line, arg4);
    }
    return final_y;
}

s32 talk_run(arg0, arg1, arg2)
u8 arg0;
u8 arg1;
u8 arg2;
{
    TalkTables *tables;
    TalkWork *dialogue;
    s16 group;
    s32 result;
    u8 command;

    tables = (TalkTables *)&talk_tables;
    dialogue = (TalkWork *)talk_work;
    if (dialogue->unk_30 != 0) {
        if ((check_end_k() != 0) && ((dialogue->unk_20 == 0) || ((dialogue->unk_20 == 1) && (cd_audio_state() != 4)) || ((*(u8 *)&game_state.voice_mode) == 0))) {
            dialogue->unk_30 = 0;
            D_800CA2AC = 0;
        } else if ((game_state.pad_pressed & 0x400040) && ((*(u8 *)&game_state.voice_mode) == 2)) {
            audio_command(0xB4);
            dialogue->unk_30 = 2;
            D_800CA2AC = 0;
            D_800CA2C4 = 1;
        }
        if ((check_end_k() != 0) && (dialogue->unk_30 == 2) && (cd_audio_state() != 4)) {
            dialogue->unk_30 = 0;
            D_800CA2AC = 0;
        }
    } else if (dialogue->unk_48 != 0) {
        if ((check_end_k() != 0) && ((dialogue->unk_20 == 0) || ((dialogue->unk_20 == 1) && (cd_audio_state() != 4)) || ((*(u8 *)&game_state.voice_mode) == 0))) {
            k_reset(1);
            dialogue->unk_48 = 0;
            D_800CA2AC = 0;
        }
    } else if (dialogue->unk_14 != 0) {
        if ((game_state.pad_pressed & 0x400040) && ((*(u8 *)&game_state.voice_mode) == 2)) {
            audio_command(0xB4);
            D_800CA2C4 = 1;
            dialogue->unk_14 = 2;
            D_800CA2AC = 0;
        }
        if ((check_end_k() != 0) && (dialogue->unk_14 == 2) && (cd_audio_state() != 4)) {
            k_reset(1);
            dialogue->unk_14 = 0;
            D_8011ED5B &= 0xFF7F;
            D_800CA2AC = 0;
            dialogue->unk_20 = 0;
            return 1;
        }
        if ((game_state.pad_pressed & 0x200020) && (check_end_k() != 0) && ((dialogue->unk_20 == 0) || (dialogue->unk_20 == 0xA) || ((dialogue->unk_20 == 1) && (cd_audio_state() != 4)) || ((*(u8 *)&game_state.voice_mode) == 0))) {
            k_reset(1);
            dialogue->unk_14 = 0;
            D_8011ED5B &= 0xFF7F;
            D_800CA2AC = 0;
            dialogue->unk_20 = 0;
            return 1;
        }
        if ((check_end_k() != 0) && ((dialogue->unk_20 == 0) || (dialogue->unk_20 == 0xA) || ((dialogue->unk_20 == 1) && (cd_audio_state() != 4)) || ((*(u8 *)&game_state.voice_mode) == 0))) {
            D_8011ED5B |= 0x80;
            hud_text_marker_bob();
        } else {
            D_8011ED5B &= 0x7F;
        }
    } else {
        D_800CA2C8 = 0;
        game_state.mode_step_timer.w = 0;
        group = *tables->group;
        result = talk_speak_part(tables->text[group][*tables->line], (s32) tables->command[group][*tables->line], (s32) arg0, tables->voice, (s32) group, (s32) (*tables->line), (s32) arg2);
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
        if ((check_end_k() == 0) && ((*(u8 *)&game_state.voice_mode) == 0)) {
            command = tables->command[*tables->group][*tables->line - 1];
            if (((command == 1) || (command == 2) || (command == 5) || (command == 0xA) || (command == 0xB) || (command == 0xC) || (command == 0xD)) && (dialogue->unk_2C == 0)) {
                if ((dialogue->unk_24 != 0) && (dialogue->unk_4C == 0) && (dialogue->unk_50 == 0)) {
                    bustup_speech();
                } else if ((dialogue->unk_34 != 0) && (dialogue->unk_40 == 0) && (dialogue->unk_4C == 0)) {
                    portrait_event_face_mouth_animation_start();
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
    D_800CA2A4[0] = game_state.frame_count % 3;
    if (D_800CA2A4[0] == 0) {
        D_800CA2A4[1] = D_800CA2A4[0] + (game_state.frame_count & 1) + 1;
        D_800CA2A4[2] = D_800CA2A4[0] + ((game_state.frame_count + 1) & 1) + 1;
    } else if (D_800CA2A4[0] == 1) {
        D_800CA2A4[1] = (D_800CA2A4[0] - 1) + ((game_state.frame_count & 1) * 2);
        D_800CA2A4[2] = (D_800CA2A4[0] - 1) + (((game_state.frame_count + 1) & 1) * 2);
    } else {
        D_800CA2A4[1] = (D_800CA2A4[0] - 1) - (game_state.frame_count & 1);
        D_800CA2A4[2] = (D_800CA2A4[0] - 1) - ((game_state.frame_count + 1) & 1);
    }
}

void normal_date_three_select(void) {
    switch (game_state.mode_step_timer.u) {
    case 0:
        normal_date_three_select_init();
        return;
    case 1:
        normal_date_three_select_main();
        return;
    default:
        game_mode_reset_to_title();
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
    game_state.menu_cursor_1 = 0;
    D_80122D30 = 1;
    game_state.mode_step_timer.w++;
}

s32 normal_date_three_select_main(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_cursor_update(1);
    menu_bar_show(1);
    if (game_state.pad_pressed & 0x200020) {
        if (check_end_k() != 0) {
            if (game_state.menu_items[1].hover != 0) {
                D_80122CDC = D_800CA2A4[0];
            } else if (game_state.menu_items[3].hover != 0) {
                D_80122CDC = D_800CA2A4[1];
            } else if (game_state.menu_items[5].hover != 0) {
                D_80122CDC = D_800CA2A4[2];
            } else {
                return 0;
            }
            D_8011ECD3 &= 0xFF7F;
            D_80122D30 = 0;
            k_reset(1);
            mode_step_next();
        }
    }
}

void normal_date_two_select(void) {
    switch (game_state.mode_step_timer.u) {
    case 0:
        normal_date_two_select_init();
        return;
    case 1:
        normal_date_two_select_main();
        return;
    default:
        game_mode_reset_to_title();
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
    game_state.menu_cursor_1 = 0;
    D_80122D30 = 1;
    game_state.mode_step_timer.w++;
}

s32 normal_date_two_select_main(void) {
    menu_check(1, cursor_x, cursor_y);
    menu_cursor_update(1);
    menu_bar_show(1);
    if (game_state.pad_pressed & 0x200020) {
        if (check_end_k() != 0) {
            if (game_state.menu_items[1].hover != 0) {
                D_80122CDC = 0;
            } else if (game_state.menu_items[3].hover != 0) {
                D_80122CDC = 1;
            } else {
                return 0;
            }
            D_8011ECD3 &= 0xFF7F;
            D_80122D30 = 0;
            k_reset(1);
            mode_step_next();
        }
    }
}

void func_80083338(void) {
    portrait_layout_set(get_g_zyotai_h(game_state.current_girl) & 0x7F);
    mode_step_next();
}

void func_80083378(void) {
    portrait_layout_set(0);
    mode_step_next();
}

void func_800833A0(void) {
    portrait_layout_set(1);
    mode_step_next();
}

void func_800833C8(void) {
    portrait_layout_set(2);
    mode_step_next();
}

void func_800833F0(void) {
    portrait_layout_set(3);
    mode_step_next();
}

void func_80083418(void) {
    portrait_layout_set(4);
    mode_step_next();
}

void portrait_layout_set(u8 arg0) {
    portrait_quad_offsets(arg0);
    portrait_value_clear_if_high(arg0);
}

s32 portrait_show_checked(void) {
    u8 var_v1;

    if ((((s32 *)D_800CA120)[0] != 0x801820AC || ((s32 *)D_800CA120)[1] != 0x801860AC ||
         ((s32 *)D_800CA120)[2] != 0x8018A0AC || ((s32 *)D_800CA120)[3] != 0x8018E0AC ||
         ((s32 *)D_800CA120)[4] != 0x801920AC || ((s32 *)D_800CA128)[0] != 0x8018005C ||
         ((s32 *)D_800CA128)[1] != 0x80180070 || *(s32 *)D_800CA124 != 0x80180000) &&
        game_state.game_mode >= 0x50 && game_state.game_mode < 0x70) {
        game_state.mode_step -= 3;
        mode_step_next();
        return 0;
    }
    if (game_state.game_mode >= 0x50 && game_state.game_mode < 0x70 && file_checksum_ok(0x2D, 0x80180000) == 0) {
        mode_step_next();
        game_state.mode_step -= 3;
        return 0;
    }
    var_v1 = get_g_zyotai_h((s32) game_state.current_girl) & 0x7F;
    if ((u8) game_state.current_girl >= GIRL_REI) {
        var_v1 = 0;
    }
    portrait_build(D_800CA130, D_800CA120, D_800CA124, D_800CA128, var_v1);
    mode_step_next();
}

void func_80083628(void) {
    u8 var_v1;

    var_v1 = get_g_zyotai_h((s32) game_state.current_girl) & 0x7F;
    if ((u8) game_state.current_girl >= GIRL_REI) {
        var_v1 = 0;
    }
    portrait_build(D_800CA130, D_800CA120, D_800CA124, D_800CA128, var_v1);
    mode_step_next();
}

s32 func_800836A0(void) {
    /* The two early failure paths return zero; the success path falls through. */
    if ((((s32 *)D_800CA120)[0] != 0x801820AC) || (((s32 *)D_800CA120)[1] != 0x801860AC) || (((s32 *)D_800CA120)[2] != 0x8018A0AC) || (((s32 *)D_800CA120)[3] != 0x8018E0AC) || (((s32 *)D_800CA120)[4] != 0x801920AC)) {
        game_state.mode_step -= 4;
        return 0;
    }
    if (file_checksum_ok(0x2D, 0x80180000) == 0) {
        mode_step_next();
        game_state.mode_step -= 5;
        return 0;
    }
    /* FAKE: narrowing before the mask preserves temporary allocation. T-10020. */
    portrait_build(D_800CA130, D_800CA120, D_800CA124, D_800CA128, (u8)get_g_zyotai_h((s32) game_state.current_girl) & 0x7F);
    /* One record-table view keeps the byte clear before the flag updates. */
    actor_pool[31 * 0x44 + 7] = 0;
    *(s32 *)&actor_pool[31 * 0x44 + 0x38] |= 0x81000000;
    *(s32 *)&actor_pool[30 * 0x44 + 0x38] |= 0x81000000;
    sprite_quads->flags |= 0x81000000;
    sprite_quads[1].flags |= 0x81000000;
    mode_step_next();
}

/* s32 without a return value: the ~0x80 mask is then loaded into v0 (li v0,-129) as in the
 * original, although the byte clears use andi 0xFF7F. T-11590 */
s32 portrait_wink_update(void) {
    if (D_80122CF4 != 0) {
        actor_pool[0x83F] |= 0x80;
        actor_pool[0x7FB] |= 0x80;
        sprite_quads[0].flags &= 0x7FFFFFFF;
        sprite_quads[1].flags &= 0x7FFFFFFF;
        portrait_face_update();
        if (D_80122D20 != 0) {
            sprite_quads[0].unk_0F = 0;
        }
        if (game_state.frame_count % 240 == 0) {
            bustup_wink();
        }
    } else if (D_80122D04 != 0) {
        actor_pool[0x83F] &= ~0x80;
        actor_pool[0x7FB] &= ~0x80;
        sprite_quads[0].flags |= 0x80000000;
        sprite_quads[1].flags |= 0x80000000;
        if (D_80122D10 == 1) {
            if (game_state.frame_count % 240 == 0) {
                portrait_event_face_mouth_animation_start();
            }
        } else if (D_80122D10 == 0) {
            portrait_event_face_mouth_lipsync();
        }
        if (game_state.frame_count % 240 == 0 && D_80122D0C == 0) {
            portrait_event_face_eyes_animation_start();
        }
    } else {
        actor_pool[0x83F] &= ~0x80;
        actor_pool[0x7FB] &= ~0x80;
        sprite_quads[0].flags |= 0x80000000;
        sprite_quads[1].flags |= 0x80000000;
    }
}

void func_80083A10(void) {
    if (D_80122CF8 != 0) {
        dec_bg_draw(1);
    }
    if (D_80122D2C == 0 && D_80122D04 == 0 && (game_state.pad_held & 0xF000F000)) {
        actor_pool[3] |= 0x80;
    }
    if (D_80122D04 != 0 || D_80122D2C != 0) {
        actor_pool[3] &= 0x7F;
    }
    if (D_80122D30 == 1) {
        actor_pool[3] |= 0x80;
    }
    if (actor_pool[3] & 0x80) {
        hud_cursor_update();
    }
}

void hizuke_hide(void) {
    hizuke_disp_switch(0);
    mode_step_next();
}

void hizuke_appear(void) {
    hizuke_disp_switch(1);
    mode_step_next();
}

void read_bustup(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        if (game_state.month >= 3U && game_state.month < 6U) {
            portrait_read_start(0x55B2);
        } else if (game_state.month >= 6U && game_state.month < 9U) {
            portrait_read_start(0x55DF);
        } else if (game_state.month >= 9U && game_state.month < 12U) {
            portrait_read_start(0x560C);
        } else {
            portrait_read_start(0x5639);
        }
        break;
    case GIRL_MIO:
        if (game_state.month >= 3U && game_state.month < 6U) {
            portrait_read_start(0x5D9B);
        } else if (game_state.month >= 6U && game_state.month < 9U) {
            portrait_read_start(0x5DC8);
        } else if (game_state.month >= 9U && game_state.month < 12U) {
            portrait_read_start(0x5DF5);
        } else {
            portrait_read_start(0x5E22);
        }
        break;
    case GIRL_YUINA:
        if (game_state.month >= 3U && game_state.month < 6U) {
            portrait_read_start(0x5288);
        } else if (game_state.month >= 6U && game_state.month < 9U) {
            portrait_read_start(0x52B5);
        } else if (game_state.month >= 9U && game_state.month < 12U) {
            portrait_read_start(0x52E2);
        } else {
            portrait_read_start(0x530F);
        }
        break;
    case GIRL_AYAKO:
        if (game_state.month >= 3U && game_state.month < 6U) {
            portrait_read_start(0x5909);
        } else if (game_state.month >= 6U && game_state.month < 9U) {
            portrait_read_start(0x5936);
        } else if (game_state.month >= 9U && game_state.month < 12U) {
            portrait_read_start(0x5963);
        } else {
            portrait_read_start(0x5990);
        }
        break;
    case GIRL_SAKI:
        if (game_state.month >= 3U && game_state.month < 6U) {
            portrait_read_start(0x6719);
        } else if (game_state.month >= 6U && game_state.month < 9U) {
            portrait_read_start(0x6746);
        } else if (game_state.month >= 9U && game_state.month < 12U) {
            portrait_read_start(0x6773);
        } else {
            portrait_read_start(0x67A0);
        }
        break;
    case GIRL_YUKARI:
        if (game_state.month >= 3U && game_state.month < 6U) {
            portrait_read_start(0x6011);
        } else if (game_state.month >= 6U && game_state.month < 9U) {
            portrait_read_start(0x603E);
        } else if (game_state.month >= 9U && game_state.month < 12U) {
            portrait_read_start(0x606B);
        } else {
            portrait_read_start(0x6098);
        }
        break;
    case GIRL_NOZOMI:
        if (game_state.month >= 3U && game_state.month < 6U) {
            portrait_read_start(0x6287);
        } else if (game_state.month >= 6U && game_state.month < 9U) {
            portrait_read_start(0x62B4);
        } else if (game_state.month >= 9U && game_state.month < 12U) {
            portrait_read_start(0x62E1);
        } else {
            portrait_read_start(0x630E);
        }
        break;
    case GIRL_MIRA:
        if (game_state.month >= 3U && game_state.month < 6U) {
            portrait_read_start(0x5B52);
        } else if (game_state.month >= 6U && game_state.month < 9U) {
            portrait_read_start(0x5B7F);
        } else if (game_state.month >= 9U && game_state.month < 12U) {
            portrait_read_start(0x5BAC);
        } else {
            portrait_read_start(0x5BD9);
        }
        break;
    case GIRL_YUKO:
        if (game_state.month >= 3U && game_state.month < 6U) {
            portrait_read_start(0x503F);
        } else if (game_state.month >= 6U && game_state.month < 9U) {
            portrait_read_start(0x506C);
        } else if (game_state.month >= 9U && game_state.month < 12U) {
            portrait_read_start(0x5099);
        } else {
            portrait_read_start(0x50C6);
        }
        break;
    case GIRL_MEGUMI:
        if (game_state.month >= 3U && game_state.month < 6U) {
            portrait_read_start(0x64FD);
        } else if (game_state.month >= 6U && game_state.month < 9U) {
            portrait_read_start(0x652A);
        } else if (game_state.month >= 9U && game_state.month < 12U) {
            portrait_read_start(0x6557);
        } else {
            portrait_read_start(0x6584);
        }
        break;
    case GIRL_YUMI:
        if (game_state.month >= 3U && game_state.month < 6U) {
            portrait_read_start(0x698F);
        } else if (game_state.month >= 6U && game_state.month < 9U) {
            portrait_read_start(0x69BC);
        } else if (game_state.month >= 9U && game_state.month < 12U) {
            portrait_read_start(0x69E9);
        } else {
            portrait_read_start(0x6A16);
        }
        break;
    case GIRL_YOSHIO:
        portrait_read_start(0x6C8C);
        break;
    }
    mode_step_next();
}

void read_bustup_uniform(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        if (game_state.month >= 6U && game_state.month < 10U) {
            portrait_read_start(0x541D);
        } else {
            portrait_read_start(0x544A);
        }
        break;
    case GIRL_MIO:
        if (game_state.month >= 6U && game_state.month < 10U) {
            portrait_read_start(0x5D14);
        } else {
            portrait_read_start(0x5D41);
        }
        break;
    case GIRL_YUINA:
        if (game_state.month >= 6U && game_state.month < 10U) {
            portrait_read_start(0x5201);
        } else {
            portrait_read_start(0x522E);
        }
        break;
    case GIRL_AYAKO:
        if (game_state.month >= 6U && game_state.month < 10U) {
            portrait_read_start(0x5882);
        } else {
            portrait_read_start(0x58AF);
        }
        break;
    case GIRL_SAKI:
        if (game_state.month >= 6U && game_state.month < 10U) {
            portrait_read_start(0x6692);
        } else {
            portrait_read_start(0x66BF);
        }
        break;
    case GIRL_YUKARI:
        if (game_state.month >= 6U && game_state.month < 10U) {
            portrait_read_start(0x5F8A);
        } else {
            portrait_read_start(0x5FB7);
        }
        break;
    case GIRL_NOZOMI:
        if (game_state.month >= 6U && game_state.month < 10U) {
            portrait_read_start(0x6200);
        } else {
            portrait_read_start(0x622D);
        }
        break;
    case GIRL_MIRA:
        if (game_state.month >= 6U && game_state.month < 10U) {
            portrait_read_start(0x5AF8);
        } else {
            portrait_read_start(0x5B25);
        }
        break;
    case GIRL_YUKO:
        if (game_state.month >= 6U && game_state.month < 10U) {
            portrait_read_start(0x4FE5);
        } else {
            portrait_read_start(0x5012);
        }
        break;
    case GIRL_MEGUMI:
        if (game_state.month >= 6U && game_state.month < 10U) {
            portrait_read_start(0x64A3);
        } else {
            portrait_read_start(0x64D0);
        }
        break;
    case GIRL_YUMI:
        if (game_state.month >= 6U && game_state.month < 10U) {
            portrait_read_start(0x6908);
        } else {
            portrait_read_start(0x6935);
        }
        break;
    case GIRL_REI:
        if (game_state.month >= 6U && game_state.month < 10U) {
            portrait_read_start(0x57FB);
        } else {
            portrait_read_start(0x5828);
        }
        break;
    case GIRL_MIHARU:
        if (game_state.month >= 6U && game_state.month < 10U) {
            portrait_read_start(0x6B51);
        } else {
            portrait_read_start(0x6B7E);
        }
        break;
    case GIRL_YOSHIO:
        if (game_state.month >= 6U && game_state.month < 10U) {
            portrait_read_start(0x6C32);
        } else {
            portrait_read_start(0x6C5F);
        }
        break;
    case GIRL_SOTOI:
        portrait_read_start(0x6B24);
        break;
    }
    mode_step_next();
}

void read_bustup_swimsuit(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        portrait_read_start(0x5666);
        break;
    case GIRL_MIO:
        portrait_read_start(0x5E4F);
        break;
    case GIRL_YUINA:
        portrait_read_start(0x533C);
        break;
    case GIRL_AYAKO:
        portrait_read_start(0x59BD);
        break;
    case GIRL_SAKI:
        portrait_read_start(0x67CD);
        break;
    case GIRL_YUKARI:
        portrait_read_start(0x60C5);
        break;
    case GIRL_NOZOMI:
        portrait_read_start(0x633B);
        break;
    case GIRL_MIRA:
        portrait_read_start(0x5C06);
        break;
    case GIRL_YUKO:
        if (game_state.year == 0x5F) {
            portrait_read_start(0x50F3);
        } else if (game_state.year == 0x60) {
            portrait_read_start(0x5120);
        } else {
            portrait_read_start(0x514D);
        }
        break;
    case GIRL_MEGUMI:
        portrait_read_start(0x65B1);
        break;
    case GIRL_YUMI:
        portrait_read_start(0x6A43);
        break;
    }
    mode_step_next();
}

void bustup_return(void) {
    read_bustup();
}

/* Restore the main mode, stage and step saved in saved_mode, saved_stage and saved_step by calling game_mode_set,
 * mode_stage_set and mode_step_set */
void return_step(void) {
    game_mode_set(game_state.saved_game_mode);
    mode_stage_set(game_state.saved_mode_stage);
    mode_step_set(game_state.saved_mode_step);
}

void k_disp_inc2(void) {
    s32 pad; /* FAKE: unused slot above sp2B, the original frame has it (real source unknown). T-8070 */
    u8 sp2B;

    if (D_80122CFC-- < 1) {
        D_80122CFC = 0;
        sp2B = get_k_speed();
        if (game_state.pad_held & 0x600060) {
            k_speed_set(TEXT_SPEED_FAST);
        }
        k_disp_inc();
        k_speed_set(sp2B);
    }
}

void yosi_trans(void) {
    audio_bank_start(0, 0x200);
    mode_step_next();
}

void change_yoshio(void) {
    game_state.current_girl = GIRL_YOSHIO;
    mode_step_next();
}

void change_girl(void) {
    game_state.current_girl = game_state.date_girl;
    mode_step_next();
}

void talk_names_set(u8 arg0) {
    s32 t;

    strcpy(name_code_family, game_state.player_family_name);
    strcpy(name_code_given, game_state.player_given_name);
    if (arg0 < 0xE) {
        t = arg0;
        get_p_name(name_code_player_form, t);
        strcpy(name_code_girl_short, girl_short_name_table[arg0]);
        get_g_name(girl_name_buf, t);
    }
}

void select_girl(void) {
    switch (game_state.mode_step_timer.u) {
    case 0:
        select_girl_init(0);
        return;
    case 1:
        select_girl_main(0);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void select_girl2(void) {
    switch (game_state.mode_step_timer.u) {
    case 0:
        select_girl_init(1);
        return;
    case 1:
        select_girl_main(1);
        return;
    default:
        game_mode_reset_to_title();
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
            game_state.menu_cursor_1 = i;
            break;
        }
    }
    D_80122D30 = 1;
    game_state.mode_step_timer.w++;
}

s32 select_girl_main(s32 arg0) {
    s16 i;
    u32 n;

    menu_check(1, cursor_x, cursor_y);
    menu_cursor_update(1);
    menu_bar_show(1);
    if (game_state.pad_pressed & 0x200020) {
        n = 0xC - (u8)arg0;
        for (i = 0; i < n; i++) {
            if (((u8 *)&game_state)[i * 16 + 0xF9E] != 0 && D_800CA330[i] != 0) {
                D_80122CDC = i;
                D_8011ECD3 &= 0xFF7F;
                D_80122D30 = 0;
                k_reset(1);
                mode_step_next();
                return 0;
            }
        }
    }
}

void sprite_brightness(s16 idx, u8 val) {
    u8 *p = (u8 *)sprite_quads + idx * 36;
    p[0x16] = val;
    p[0x15] = val;
    p[0x14] = val;
}

void day_plus(void) {
    s32 sp28; /* FAKE: unused local, the original frame has one more word below sp2B (T-3330 slot rule); source unknown. T-6040 */
    u8 sp2B;

    D_800CA2F0 = 0;
    sp2B = D_8011F113;
    calendar_advance_day();
    hizuke_init();
    if (sp2B & 0x80) {
        hizuke_disp_switch(1);
    } else {
        hizuke_disp_switch(0);
    }
    hizuke_show();
    mode_step_next();
}

/* Clamp girl tokimeki/affection (0..0x80), syoushin (0..0xFF) and the nine parameters (0..999); the queue reader
 * then clamps all three girl counters to 0x80 (girl_counters_clamp) */
void check_para_limit(void) {
    s16 i;

    for (i = 0; i < 11; i++) {
        if (game_state.girls[i].tokimeki < 0) {
            *(s32 *) &game_state.girls[i].tokimeki_fraction = 0; /* word store over the s16 pair */
        }
        if (game_state.girls[i].tokimeki >= 0x80) {
            game_state.girls[i].tokimeki = 0x80;
        }
        if (game_state.girls[i].affection < 0) {
            *(s32 *) &game_state.girls[i].affection_fraction = 0;
        }
        if (game_state.girls[i].affection >= 0x80) {
            game_state.girls[i].affection = 0x80;
        }
        if (game_state.girls[i].syoushin < 0) {
            *(s32 *) &game_state.girls[i].syoushin_fraction = 0;
        }
        if (game_state.girls[i].syoushin >= 0xFF) {
            game_state.girls[i].syoushin = 0xFF;
        }
    }
    for (i = 0; i < 9; i++) {
        if ((&game_state.param_condition)[i].value < 0) {
            ((s32 *) &game_state.param_condition)[i] = 0;
        }
        if ((&game_state.param_condition)[i].value >= 0x3E7) {
            ((s32 *) &game_state.param_condition)[i] = 0x3E70000;
        }
    }
}

void text_marker_hide(void) {
    hud_text_marker_set(0);
}

void func_80084E6C(void) {
    D_80122D20 = 0;
    mode_step_next();
}

/* s32 without a return value: the last chain value stays in v0 (andi v0,a0,0xffff). T-11590 */
s32 scene_still_show(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
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
    game_state.tpage_buf[37] = 1;
    game_state.tpage_buf[38] = 1;
    game_state.tpage_buf[39] = 1;
    game_state.tpage_buf[40] = 1;
    game_state.tpage_buf[41] = 1;
    game_state.tpage_buf[42] = 1;
    for (i = 0x3A; i < 0x40; i++) {
        sprite_quads[i].unk_0C = i - 0x35;
        sprite_quads[i].flags = 0xC1000000;
        /* FAKE: u16 view of the width (GsSPRITE w is unsigned; SpriteQuad keeps s16 for ETC
         * func_80142A4C): the 0x80 constant is then shared with unk_0A and unk_0F. T-11590 */
        *(u16 *)&sprite_quads[i].w = 0x80;
        sprite_quads[i].h = 0x80;
        sprite_quads[i].unk_0E = 0;
        sprite_quads[i].unk_0F = 0x80;
        sprite_quads[i].unk_10 = 0;
        sprite_quads[i].unk_12 = 0x1F1;
        sprite_quads[i].tint_r = 0;
        sprite_quads[i].tint_g = 0;
        sprite_quads[i].tint_b = 0;
        sprite_quads[i].anchor_x = 0;
        sprite_quads[i].anchor_y = 0;
        sprite_quads[i].unk_1C = 0x1000;
        sprite_quads[i].unk_1E = 0x1000;
        sprite_quads[i].unk_20 = 0;
        sprite_quads[i].x = ((i - 0x3A) % 3) * 128 - 0xA0;
        sprite_quads[i].y = ((i - 0x3A) / 3) * 128 - 0x78;
        game_state.quad_layer[i] = 0xB;
    }
    sprite_quads[60].w = sprite_quads[63].w = 0x40;
    sprite_quads[61].h = sprite_quads[62].h = sprite_quads[63].h = 0x20;
}

s32 scene_anim_show(arg0, arg1, arg2, arg3)
    s32 arg0;
    s32 arg1;
    s32 arg2;
    s16 arg3;
{
    s16 i;
    Actor *p;

    if (arg0 == 0) {
        actor_clear(0x60);
        actor_clear(0x61);
        return 0;
    }
    for (i = 0; i < 2; i++) {
        actor_clear(i + 0x60);
        p = &((Actor *)actor_pool)[i + 96];
        p->unk_01 = 0xA;
        p->flags = 0;
        p->quad_flags = 0x41000000;
        p->draw_flags = 4;
        p->frame_table = (u8 *)arg0;
        p->sequence_table = (u8 *)arg1;
        p->tpage_table = (u8 *)arg2;
        p->sequence_slot = 0;
        p->unk_18 = 0;
        p->blink_period = 2;
        p->unk_43 = 0x11;
        p->x = -0xA0;
        p->y = -0x78;
        p->brightness = 0;
        p->unk_05 = 0;
        p->unk_04 = 0;
        p->unk_14 = arg3;
    }
    ((Actor *)actor_pool)[96].flags = 4;
    if (arg3 >= 2) {
        D_800CA360 = 0;
        ((Actor *)actor_pool)[96].sequence_slot = 2;
    } else {
        ((Actor *)actor_pool)[96].draw_flags = 0;
    }
}

void func_80085234(arg0)
    u8 arg0;
{
    s16 i;
    u8 t;

    if (arg0) {
        if (actor_pool[0x1983] != 0) {
            t = D_800B5A60;
            D_800B5A60 = D_800CA360;
            D_800CA360 = t;
            actor_pool[0x1983] |= 0x80;
        }
        if (actor_pool[0x19C7] != 0) {
            actor_pool[0x19C7] |= 0x80;
        }
        for (i = 0x3A; i < 0x40; i++) {
            sprite_quads[i].flags &= 0x7FFFFFFF;
        }
    } else {
        D_800B5A60 = D_800CA360;
        if (actor_pool[0x1983] != 0) {
            actor_pool[0x1983] &= ~0x80;
        }
        if (actor_pool[0x19C7] != 0) {
            actor_pool[0x19C7] &= ~0x80;
        }
        for (i = 0x3A; i < 0x40; i++) {
            sprite_quads[i].flags |= 0x80000000;
        }
    }
}

void func_80085368(void) {
    s16 var_s0;

    normal_date_bg_fadein();
    if (dec_bg_brightness == 4) {
        func_80085234(1);
    }
    D_80122D04 = 1;
    message_window_title_show(0);
    D_80120657 = dec_bg_brightness;
    var_s0 = 0x3A;
    D_8012069B = dec_bg_brightness;
    do {
        func_80084C98(var_s0, dec_bg_brightness);
        var_s0 += 1;
    } while (var_s0 < 0x40);
}

void func_800853FC(void) {
    s16 var_s0;
    u8 temp_s0;
    u8 state;

    temp_s0 = game_state.mode_step;
    normal_date_bg_fadeout();
    state = game_state.mode_step;
    if (state != temp_s0) {
        func_80085234(0);
        D_80122D04 = 0;
        message_window_title_show(1);
    }
    D_80120657 = dec_bg_brightness;
    var_s0 = 0x3A;
    D_8012069B = dec_bg_brightness;
    do {
        func_80084C98(var_s0, dec_bg_brightness);
        var_s0 += 1;
    } while (var_s0 < 0x40);
}

void event_face0(void) {
    D_80120666 = 2;
    D_801206AA = 0;
    mode_step_next();
}

void event_face1(void) {
    D_80120666 = 3;
    D_801206AA = 1;
    mode_step_next();
}

void event_face2(void) {
    D_80120666 = 6;
    D_801206AA = 4;
    mode_step_next();
}

void event_face3(void) {
    D_80120666 = 7;
    D_801206AA = 5;
    mode_step_next();
}

void event_face_kuchi0(void) {
    D_80120666 = 2;
    mode_step_next();
}

void event_face_kuchi1(void) {
    D_80120666 = 3;
    mode_step_next();
}

void event_kuchi_ani_no(void) {
    D_80122D10 = 0;
    D_80120652 = 4;
    mode_step_next();
}

void event_kuchi_ani_yes(void) {
    D_80122D10 = 1;
    D_80120652 = 0;
    mode_step_next();
}

void ev_me_on(void) {
    D_80122D0C = 0;
    D_80120696 = 5;
    mode_step_next();
}

void ev_me_on_continue(void) {
    D_80122D0C = 1;
    D_80120696 = 5;
    mode_step_next();
}

/* FAKE: the explicit "& 0xFF" masks leave the colour shifts in ugen temporaries (t6..t9), and each
 * colour trio shares one source line because as1 schedules stores by source line. The local v4 holds
 * the red byte of arg5 and is read once (the r3 store repeats the expression): it numbers that
 * uopt range first, so it takes $a0 and the blue byte of arg6 takes $a1 (p moves to $a2), and it
 * makes arg5 the first stack argument loaded. The real source is unknown. T-17001 */
void _sprite_set_box_shade_tarao(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u32 arg5, u32 arg6) {
    GameState *gs;
    POLY_G4 *p;
    u32 v4;

    p = (POLY_G4 *)GetWorkBase(0x24, draw_buffer);
    func_8009F054(p);
    SetSemiTrans(p, 0);
    gs = &game_state;
    v4 = (arg5 >> 16) & 0xFF;
    p->r0 = (arg6 >> 16) & 0xFF; p->g0 = (arg6 >> 8) & 0xFF; p->b0 = arg6 & 0xFF;
    p->r1 = (arg6 >> 16) & 0xFF; p->g1 = (arg6 >> 8) & 0xFF; p->b1 = arg6 & 0xFF;
    p->r2 = v4; p->g2 = (arg5 >> 8) & 0xFF; p->b2 = arg5 & 0xFF;
    p->r3 = (arg5 >> 16) & 0xFF; p->g3 = (arg5 >> 8) & 0xFF; p->b3 = arg5 & 0xFF;
    p->x0 = gs->screen_center_x[draw_buffer] + arg0;
    p->y0 = gs->screen_center_y[draw_buffer] + arg1;
    p->x1 = gs->screen_center_x[draw_buffer] + arg0 + arg2;
    p->y1 = gs->screen_center_y[draw_buffer] + arg1;
    p->x2 = gs->screen_center_x[draw_buffer] + arg0;
    p->y2 = gs->screen_center_y[draw_buffer] + arg1 + arg3;
    p->x3 = gs->screen_center_x[draw_buffer] + arg0 + arg2;
    p->y3 = gs->screen_center_y[draw_buffer] + arg1 + arg3;
    AddPrim(D_800E8CA0 + (draw_buffer << 10) + arg4 * 4, (s32)p);
}

void talk_clear(void) {
    s16 i;
    for (i = 5; i < 31;) {
        talk_work[i] = 0;
        i++;
    }
    D_800CA2AC = 0;
    hud_cursor_actor_init(0);
    D_8011ECD3 &= 0xFF7F;
    k_reset(0);
    message_window_set_title((s32) "　");
    message_window_title_show(0);
    hud_mascot_display_switch(0);
}

void don_init2(void) {
    func_80047550();
    mode_step_next();
}

void don_wait(void) {
    if ((u32)game_state.mode_step_timer.w++ >= 0x81U) {
        k_reset(1);
        mode_step_next();
    }
}

void normal_date_speak_1line(void) {
    talk_tables = (u8 *)&talk_group;
    D_800CA138 = (u8 *)&talk_line;
    D_800CA13C = talk_text_table;
    D_800CA140 = talk_command_table;
    D_800CA144 = talk_voice_table;
    if (talk_run(D_80122CDC, 1, 0) == 1) {
        mode_step_next();
    }
}

void normal_date_speak_012(void) {
    talk_tables = &talk_group;
    D_800CA138 = &talk_line;
    D_800CA13C = talk_text_table;
    D_800CA140 = talk_command_table;
    D_800CA144 = talk_voice_table;
    talk_run(0xFF, 1, 0);
}

/* Wire the dialogue table pointers and run the talk runner for the current scene girl */
void normal_date_speak(void) {
    talk_tables = &talk_group;
    D_800CA138 = &talk_line;
    D_800CA13C = talk_text_table;
    D_800CA140 = talk_command_table;
    D_800CA144 = talk_voice_table;
    talk_run(D_80122CDC, 1, 0);
}

void vram_bustup_clear(void) {
    s16 i;

    actor_clear(0x1F);
    actor_clear(0x1E);
    for (i = 5; i < 0xA; i++) {
        game_state.tpage_buf[i] = -1;
    }
    mode_step_next();
}

s32 event_log_add(arg0, arg1)
    u8 arg0;
    u8 arg1;
{
    game_state.event_log[game_state.event_log_count].girl = game_state.current_girl;
    game_state.event_log[game_state.event_log_count].scene = arg0;
    game_state.event_log[game_state.event_log_count].flag = arg1;
    if (D_80122D48 == 0 || D_80122D48 >= 7) {
        game_state.event_log[game_state.event_log_count].rank = get_g_zyotai_h(game_state.current_girl);
    } else {
        game_state.event_log[game_state.event_log_count].rank = D_80122D48 - 1;
    }
    D_80122D48 = 0;
    if (game_state.current_girl == GIRL_REI) {
        game_state.event_log[game_state.event_log_count].rank = 0;
    }
    game_state.event_log[game_state.event_log_count].year = game_state.year;
    game_state.event_log[game_state.event_log_count].month = game_state.month;
    game_state.event_log[game_state.event_log_count].day = game_state.day;
    game_state.event_log[game_state.event_log_count].weekday = game_state.weekday;
    if (++game_state.event_log_count >= 0xFF) {
        game_state.event_log_count = 0xFE;
    }
}

void portrait_quad_offsets(u8 a) {
    switch (a) {
    case 0:
        sprite_quads[0].unk_0C = 7;
        sprite_quads[0].unk_0E = 0x30;
        sprite_quads[1].unk_0C = 7;
        sprite_quads[1].unk_0E = 0;
        break;
    case 1:
        sprite_quads[0].unk_0C = 7;
        sprite_quads[0].unk_0E = 0x70;
        sprite_quads[1].unk_0C = 7;
        sprite_quads[1].unk_0E = 0x40;
        break;
    case 2:
        sprite_quads[0].unk_0C = 8;
        sprite_quads[0].unk_0E = 0x30;
        sprite_quads[1].unk_0C = 8;
        sprite_quads[1].unk_0E = 0;
        break;
    case 3:
        sprite_quads[0].unk_0C = 8;
        sprite_quads[0].unk_0E = 0x70;
        sprite_quads[1].unk_0C = 8;
        sprite_quads[1].unk_0E = 0x40;
        break;
    case 4:
        sprite_quads[0].unk_0C = 9;
        sprite_quads[0].unk_0E = 0x30;
        sprite_quads[1].unk_0C = 9;
        sprite_quads[1].unk_0E = 0;
        break;
    case 5:
        sprite_quads[0].unk_0C = 9;
        sprite_quads[0].unk_0E = 0x70;
        sprite_quads[1].unk_0C = 9;
        sprite_quads[1].unk_0E = 0x40;
        break;
    }
}
