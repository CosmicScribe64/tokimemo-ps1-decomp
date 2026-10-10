#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004E500);

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004E58C);

void func_8004E750(u8 arg0) {
    if (arg0 == 0x10) {
        D_800B3F6A = 0x10;
    } else {
        D_800B3F6A = 0xE;
    }
}

void func_8004E780(void) {
}

s32 set_kanji_string(s16 x, s16 y, u8 col, u8 *str, s32 arg4) {
    if (D_800B3F60 >= 0x34) {
        return -1;
    }
    if (str == 0) {
        ((Entry8 *)D_800B3DC0)[D_800B3F60].unk_07 = 0;
    } else if (str[0] == 0 || str[1] == 0) {
        ((Entry8 *)D_800B3DC0)[D_800B3F60].unk_07 = 0;
    } else {
        ((Entry8 *)D_800B3DC0)[D_800B3F60].unk_02 = y;
        ((Entry8 *)D_800B3DC0)[D_800B3F60].unk_04 = col;
        ((Entry8 *)D_800B3DC0)[D_800B3F60].unk_00 = x;
        ((Entry8 *)D_800B3DC0)[D_800B3F60].unk_05 = load_string(str, y, col);
        ((Entry8 *)D_800B3DC0)[D_800B3F60].unk_06 = arg4;
        ((Entry8 *)D_800B3DC0)[D_800B3F60].unk_07 = 1;
    }
    return ++D_800B3F60;
}

INCLUDE_ASM("asm/nonmatchings/main/8004E500", k_disp_start);

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
    return D_800B3DC0 + arg0 * 8;
}

s16 *get_ksys(void) {
    return &D_800B3F60;
}

void set_k_work(s32 arg0, Entry8 arg1) {
    Entry8 *p = (Entry8 *)D_800B3DC0 + arg0;

    p->unk_00 = arg1.unk_00;
    p->unk_02 = arg1.unk_02;
    p->unk_04 = arg1.unk_04;
    p->unk_05 = arg1.unk_05;
    p->unk_06 = arg1.unk_06;
    p->unk_07 = arg1.unk_07;
}

INCLUDE_ASM("asm/nonmatchings/main/8004E500", k_reset);

void k_sub_reset_point_set(void) {
    D_800B3F64 = D_800B3F60;
}

void k_sub_reset(void) {
    k_reset(D_800B3F64);
}

void k_sub_disp_start(s32 arg0) {
    k_disp_start(D_800B3F64 + arg0);
}

INCLUDE_ASM("asm/nonmatchings/main/8004E500", k_disp_goto_line_end);

void k_speed_set(u8 arg0) {
    D_800B3F66 = arg0;
    if ((u32)arg0 >= 0x81) {
        D_800B3F66 = 0x7F;
    }
}

u8 get_k_speed(void) {
    return D_800B3F66;
}

s32 k_disp_inc(void) {
    if (D_800E6280.unk_10F8 % (D_800B3F66 + 1) == 0) {
        if (D_800B3F68 < D_800B3F62) {
            D_800B3F68 += 1;
            return 0;
        }
        return 1;
    }
    if (D_800B3F68 == D_800B3F62) {
        return 1;
    }
    return 0;
}

/* The operand order sets the load order of the two globals (T-0017). */
s32 check_end_k(void) {
    if (D_800B3F68 == D_800B3F62) {
        return 1;
    }
    return 0;
}

u8 check_set_k(void) {
    return *(u8 *)&D_800B3F60;
}

INCLUDE_ASM("asm/nonmatchings/main/8004E500", get_now_k);

INCLUDE_ASM("asm/nonmatchings/main/8004E500", disp_string);

INCLUDE_ASM("asm/nonmatchings/main/8004E500", load_string);

INCLUDE_ASM("asm/nonmatchings/main/8004E500", vload_font);

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004F4F0);
