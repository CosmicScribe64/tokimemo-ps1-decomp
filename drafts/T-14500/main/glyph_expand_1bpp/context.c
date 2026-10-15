#include "common.h"
#include "game.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/data/main/8004E500.data", text_list);
INCLUDE_RODATA("asm/data/main/8004E500.data", D_800B3F58);
KWork text_list_count = { 0, 0, 0, 0, 0, 0, 0 };
INCLUDE_RODATA("asm/data/main/8004E500.data", D_800B3F6C);
INCLUDE_RODATA("asm/data/main/8004E500.data", D_800B3FAC);
INCLUDE_RODATA("asm/data/main/8004E500.data", D_800B3FCC);
INCLUDE_RODATA("asm/data/main/8004E500.data", D_800B3FEC);
INCLUDE_RODATA("asm/data/main/8004E500.data", D_800B400C);
INCLUDE_RODATA("asm/data/main/8004E500.data", font_range_table);
INCLUDE_RODATA("asm/data/main/8004E500.data", D_800B4030);
INCLUDE_RODATA("asm/data/main/8004E500.data", D_800B40E4);

void func_8004E500(s32 arg0) {
    s32 i;
    RECT r;

    for (i = 0; i < arg0; i++) {
        r.x = 0x320 + i * 0x40;
        r.y = 0x100;
        r.w = 0x20;
        r.h = 0x100;
        func_8009C7F8(&r, 0, 0, 0);
    }
    func_8009C674(0);
}

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004E58C);

void text_set_advance(u8 arg0) {
    if (arg0 == 0x10) {
        text_list_count.unk_6A = 0x10;
    } else {
        text_list_count.unk_6A = 0xE;
    }
}

void func_8004E780(void) {
}

s32 set_kanji_string(s16 x, s16 y, u8 col, u8 *str, s32 arg4) {
    if (text_list_count.count >= 0x34) {
        return -1;
    }
    if (str == 0) {
        ((Entry8 *)text_list)[text_list_count.count].unk_07 = 0;
    } else if (str[0] == 0 || str[1] == 0) {
        ((Entry8 *)text_list)[text_list_count.count].unk_07 = 0;
    } else {
        ((Entry8 *)text_list)[text_list_count.count].unk_02 = y;
        ((Entry8 *)text_list)[text_list_count.count].unk_04 = col;
        ((Entry8 *)text_list)[text_list_count.count].unk_00 = x;
        ((Entry8 *)text_list)[text_list_count.count].unk_05 = load_string(str, y, col);
        ((Entry8 *)text_list)[text_list_count.count].unk_06 = arg4;
        ((Entry8 *)text_list)[text_list_count.count].unk_07 = 1;
    }
    return ++text_list_count.count;
}

void k_disp_start(s32 arg0) {
    s32 i;

    text_list_count.pos = 0;
    for (i = 0; i < arg0; i++) {
        text_list_count.pos += text_list[i * 8 + 5];
    }
}

void k_disp_switch(s32 arg0, s32 arg1) {
    if (arg1 > 0) {
        D_800B3DC7[arg0 * 8] = 1;
    } else {
        D_800B3DC7[arg0 * 8] = 0;
    }
}

u8 *get_k_work(s32 arg0) {
    if (arg0 >= 0x35) {
        return D_800B3F58;
    }
    return text_list + arg0 * 8;
}

s16 *get_ksys(void) {
    return (u8 *)&text_list_count;
}

void set_k_work(s32 arg0, Entry8 arg1) {
    Entry8 *p = (Entry8 *)text_list + arg0;

    p->unk_00 = arg1.unk_00;
    p->unk_02 = arg1.unk_02;
    p->unk_04 = arg1.unk_04;
    p->unk_05 = arg1.unk_05;
    p->unk_06 = arg1.unk_06;
    p->unk_07 = arg1.unk_07;
}

void k_reset(s32 arg0) {
    s16 sum;
    s32 i;

    sum = 0;
    for (i = 0; i < arg0; i++) {
        sum += text_list[i * 8 + 5];
    }
    text_list_count.count = arg0;
    text_list_count.total = sum;
    text_list_count.pos = sum;
}

void k_sub_reset_point_set(void) {
    text_list_count.mark = text_list_count.count;
}

void k_sub_reset(void) {
    k_reset(text_list_count.mark);
}

void k_sub_disp_start(s32 arg0) {
    k_disp_start(text_list_count.mark + arg0);
}

s16 k_disp_goto_line_end(void) {
    s32 i;
    s32 sum;

    sum = 0;
    for (i = 0; i < 0x34; i++) {
        sum += text_list[i * 8 + 5];
        if (sum >= text_list_count.pos) {
            text_list_count.pos = sum;
            return sum;
        }
    }
    return -1;
}

void k_speed_set(u8 arg0) {
    text_list_count.speed = arg0;
    if ((u32)arg0 >= 0x81) {
        text_list_count.speed = 0x7F;
    }
}

u8 get_k_speed(void) {
    return text_list_count.speed;
}

s32 k_disp_inc(void) {
    if (D_800E6280.unk_10F8 % (text_list_count.speed + 1) == 0) {
        if (text_list_count.pos < text_list_count.total) {
            text_list_count.pos += 1;
            return 0;
        }
        return 1;
    }
    if (text_list_count.pos == text_list_count.total) {
        return 1;
    }
    return 0;
}

/* The operand order sets the load order of the two globals (T-0017). */
s32 check_end_k(void) {
    if (text_list_count.pos == text_list_count.total) {
        return 1;
    }
    return 0;
}

u8 check_set_k(void) {
    return *(u8 *)&text_list_count;
}

u8 get_now_k(void) {
    s32 i;
    s32 sum;

    sum = 0;
    for (i = 0; i < text_list_count.pos; i++) {
        sum += text_list[i * 8 + 5];
        if (sum >= text_list_count.pos) {
            return i;
        }
    }
    return 0xFF;
}

INCLUDE_ASM("asm/nonmatchings/main/8004E500", disp_string);

INCLUDE_ASM("asm/nonmatchings/main/8004E500", load_string);

void vload_font(arg0)
s32 arg0;
{
    glyph_expand_1bpp(arg0, D_801255F0 + (text_list_count.total & 0xF) * 0x18);
    if (text_list_count.total < 0x80) {
        text_list_count.total = text_list_count.total;
        LoadSquare((text_list_count.total & 7) * 4 + 0x320, (text_list_count.total & 0x78) * 2 + 0x100, 4, 0xC,
                   D_801255F0 + (text_list_count.total & 0xF) * 0x18);
    } else if (text_list_count.total < 0x100) {
        text_list_count.total = text_list_count.total;
        LoadSquare((text_list_count.total & 7) * 4 + 0x360, (text_list_count.total & 0x78) * 2 + 0x100, 4, 0xC,
                   D_801255F0 + (text_list_count.total & 0xF) * 0x18);
    } else {
        text_list_count.total = text_list_count.total;
        LoadSquare((text_list_count.total & 7) * 4 + 0x3A0, (text_list_count.total & 0x78) * 2 + 0x100, 4, 0xC,
                   D_801255F0 + (text_list_count.total & 0xF) * 0x18);
    }
    func_8009C674(0);
    text_list_count.total += 1;
}

void glyph_expand_1bpp(s32 arg0, void *arg1) {
    s32 spB0[1];
    u8 sp20;
    s32 temp_t0;
    s32 temp_t2;
    s32 temp_t3;
    s32 temp_t3_2;
    s32 temp_t7;
    s32 var_a2_2;
    s32 var_a2_3;
    s32 var_a2_4;
    s32 var_v0;
    u8 *var_a2;
    u8 *var_v1;
    u8 *var_v1_2;
    u8 temp_t4;
    u8 temp_t4_2;
    u8 temp_t7_2;
    u8 *temp_a1;
    u8 *temp_a1_2;
    u8 *var_a1;

    var_a1 = arg1;
    var_a2 = (arg0 * 0x12) + font_glyphs;
    var_v1 = &sp20;
    do {
        var_v1 += 8;
        *(s8 *)((unsigned char *)var_v1 + (-8)) = (s8) ((u32) (*var_a2 & 0x80) >> 7);
        *(s8 *)((unsigned char *)var_v1 + (-7)) = (s8) ((u32) (*var_a2 & 0x40) >> 6);
        temp_t4 = *var_a2;
        var_a2 += 1;
        *(s8 *)((unsigned char *)var_v1 + (-6)) = (s8) ((u32) (temp_t4 & 0x20) >> 5);
        *(s8 *)((unsigned char *)var_v1 + (-5)) = (s8) ((u32) (*(u8 *)((unsigned char *)var_a2 + (-1)) & 0x10) >> 4);
        *(s8 *)((unsigned char *)var_v1 + (-4)) = (s8) ((u32) (*(u8 *)((unsigned char *)var_a2 + (-1)) & 8) >> 3);
        *(s8 *)((unsigned char *)var_v1 + (-3)) = (s8) ((u32) (*(u8 *)((unsigned char *)var_a2 + (-1)) & 4) >> 2);
        *(s8 *)((unsigned char *)var_v1 + (-2)) = (s8) ((u32) (*(u8 *)((unsigned char *)var_a2 + (-1)) & 2) >> 1);
        *(s8 *)((unsigned char *)var_v1 + (-1)) = (s8) (*(u8 *)((unsigned char *)var_a2 + (-1)) & 1);
    } while ((u32) var_v1 < (u32) &spB0);
    var_v0 = 0;
    var_v1_2 = &sp20;
    do {
        (*(s32 *)var_a1) = 0;
        var_a2_2 = 0;
loop_4:
        temp_t3 = *var_v1_2 << var_a2_2;
        var_a2_2 += 4;
        (*(s32 *)var_a1) = (s32) ((*(s32 *)var_a1) + temp_t3);
        var_v1_2 += 1;
        if (var_a2_2 < 0x20) {
            goto loop_4;
        }
        temp_a1 = var_a1 + 4;
        *(s32 *)((unsigned char *)var_a1 + (4)) = 0;
        var_a2_3 = 0;
loop_6:
        temp_t7 = *var_v1_2 << var_a2_3;
        var_a2_3 += 4;
        *(s32 *)((unsigned char *)var_a1 + (4)) = (s32) (*(s32 *)((unsigned char *)var_a1 + (4)) + temp_t7);
        var_v1_2 += 1;
        if (var_a2_3 < 0x10) {
            goto loop_6;
        }
        temp_a1_2 = temp_a1 + 4;
        *(s32 *)((unsigned char *)temp_a1 + (4)) = 0;
        var_a2_4 = 0;
loop_8:
        temp_t2 = *var_v1_2 << var_a2_4;
        var_a2_4 += 4;
        *(s32 *)((unsigned char *)temp_a1 + (4)) = (s32) (*(s32 *)((unsigned char *)temp_a1 + (4)) + temp_t2);
        var_v1_2 += 1;
        if (var_a2_4 < 0x20) {
            goto loop_8;
        }
        *(s32 *)((unsigned char *)temp_a1_2 + (4)) = 0;
        temp_t4_2 = (*(s32 *)var_v1_2);
        var_v0 += 1;
        *(s32 *)((unsigned char *)temp_a1_2 + (4)) = (s32) temp_t4_2;
        temp_t7_2 = *(u8 *)((unsigned char *)var_v1_2 + (1));
        var_v1_2 += 4;
        temp_t0 = temp_t4_2 + (temp_t7_2 * 0x10);
        *(s32 *)((unsigned char *)temp_a1_2 + (4)) = temp_t0;
        var_a1 = temp_a1_2 + 8;
        temp_t3_2 = temp_t0 + (*(u8 *)((unsigned char *)var_v1_2 + (-2)) << 8);
        *(s32 *)((unsigned char *)var_a1 + (-4)) = temp_t3_2;
        *(s32 *)((unsigned char *)var_a1 + (-4)) = (s32) (temp_t3_2 + (*(u8 *)((unsigned char *)var_v1_2 + (-1)) << 0xC));
    } while (var_v0 != 6);
}
