#include "common.h"
#include "ovl/NAME_ENT.h"

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80142A80);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80142E24);

void func_80143498(void) {
    if ((func_800460EC() & 2) && D_8014D170 == 0) {
        func_80046290(0x06000CBB, 0x12000E0C, 0xF);
        func_80044750(0x300);
        func_8004E58C();
        func_8004E788(-0x90, 0x14, 0, "ここ、きらめき高校には、", 0);
        func_8004E788(-0x90, 0x24, 0, "一つの伝説があります。", 0);
        func_8004E788(-0x90, 0x34, 0, "校庭のはずれにある一本の古木。", 0);
        func_8004E884(3);
        func_8004E93C(0, 0);
        func_8004E93C(1, 0);
        func_8004E93C(2, 0);
        D_8014D170 = 1;
    }
}

/* bulk_m2c declarations (T-11190) */
extern s16 D_8012181C;
extern s16 D_801220F8;
extern s32 D_800E6300;
extern s32 D_800E6480;
extern s32 D_800E7208;
extern s32 D_800E7378;
extern s32 D_800E78BC;
extern s32 D_800E7ABC;
extern s32 D_801217F4;
extern s32 D_80184000;
extern s32 D_8019E000;
extern s32 D_801B4000;
extern s8 D_800E7326;
extern u32 D_800E7200;
extern u32 D_800E7384;
extern u8 D_800E71DF;
extern u8 D_800E738D;
extern u8 D_8012182C;
extern u8 D_8012182D;
extern u8 D_8012182E;
void func_80144908(void);
void func_80144654(void);
/* end bulk_m2c declarations */
void func_80143580(void) {
    s32 sp6C;
    s32 sp64;
    s32 *var_v1;
    s32 *var_v1_2;
    GameState *var_a2;
    GameState *var_v0_4;
    Rec24 *var_v1_3;
    s16 temp_t6;
    s16 temp_v0;
    s16 var_a0;
    s16 var_a1;
    s32 *var_v0;
    s32 *var_v0_2;
    s32 *var_v0_3;
    s32 *var_v0_5;
    s32 var_a1_2;
    s32 var_t0;
    s32 var_t0_2;
    s32 var_v1_4;
    s32 var_v1_5;
    s32 var_v1_6;
    u8 temp_a0;
    u8 temp_a0_2;
    u8 temp_a1;
    u8 temp_a1_2;
    u8 temp_a1_3;
    u8 temp_a1_4;
    u8 temp_t0;
    u8 temp_t2;
    u8 temp_t2_2;
    u8 temp_t2_3;
    u8 temp_v0_2;
    u8 var_v0_6;

    var_v0 = &sp6C;
    var_v1 = &D_80184000;
    do {
        *var_v0 = (s32) var_v1;
        var_v0 += 4;
    } while ((s32) var_v1 < 0x801A0000);
    temp_t0 = D_800E738D;
    switch (temp_t0) {
    case 0:
        temp_a1 = D_801220E2;
        D_801220E1 = temp_a1;
        D_801220E2 = temp_a1 + 1;
        D_801220E0 = temp_a1;
        if (temp_a1 >= 0x81U) {
            D_801220D0 = 0x01000000;
            D_8014D170 = 0;
            D_800E738D = temp_t0 + 1;
        }
        break;
    case 1:
        if ((D_800E7378 & 3) == 3) {
            D_801220D4 -= 1;
            if (D_801220D4 < -0xF0) {
                D_801220D0 = 0x51000000;
                D_800E738D = temp_t0 + 1;
            }
        }
        break;
    case 2:
        temp_a1_2 = D_801220E2;
        D_801220E1 = temp_a1_2;
        D_801220E2 = temp_a1_2 - 1;
        D_801220E0 = temp_a1_2;
        if (temp_a1_2 != 0) {
            temp_t2 = D_80122106;
            D_80122105 = temp_t2;
            D_80122106 = temp_t2 + 1;
            D_80122104 = temp_t2;
            if (temp_t2 >= 0x81U) {
                D_801220D0 = 0x80000000;
                D_801220F4 = 0x01000000;
                D_800E738D = temp_t0 + 1;
            }
        }
        /* fallthrough */
    case 3:
        if ((D_800E7378 & 3) == 3) {
            D_801220D4 -= 1;
            if (D_801220F8++ >= -0xEF) {
                func_8004A1A0((s32) &sp6C, 0, 8, 4, 2, 0x1EF, -0xA0, -0x78, 8, 5);
                load_palette((s32 *)0x801A0000, 0xF, 1, 3, 0);
                D_801220E1 = 0;
                D_801220D0 = 0x51000000;
                D_801220E2 = 0;
                D_801220E0 = 0;
                D_800E738D += 1;
            }
        }
        break;
    case 4:
        func_80143498();
        temp_t2_2 = D_80122106;
        D_80122105 = temp_t2_2;
        D_80122106 = temp_t2_2 - 1;
        D_80122104 = temp_t2_2;
        if (temp_t2_2 != 0) {
            temp_a1_3 = D_801220E2;
            D_801220E1 = temp_a1_3;
            D_801220E2 = temp_a1_3 + 1;
            D_801220E0 = temp_a1_3;
            if (temp_a1_3 >= 0x81U) {
                D_801220F4 = 0x80000000;
                D_801220D0 = 0x01000000;
                D_800E738D += 1;
            }
        }
        /* fallthrough */
    case 5:
        D_800E738D = temp_t0;
        if ((D_800E7378 & 3) == 3) {
            if (D_801220F8 < -0xA0) {
                D_801220F8 += 1;
            }
            temp_v0 = D_801220D4 - 1;
            D_800E738D = temp_t0;
            if (D_801220D4 < -0x140) {
                D_801220D0 = 0x51000000;
                D_800E738D = temp_t0 + 1;
            }
            D_801220D4 = temp_v0;
            func_8004B19C((s32) temp_v0, 0, 0);
            if (D_801220D4 == -0x118) {
                func_8004E58C();
                set_kanji_string(-0x90, 0x14, 0, (s32) "そのたもとで卒業の日に、", 0);
                set_kanji_string(-0x90, 0x24, 0, (s32) "女の子からの告白で生まれた", 0);
                set_kanji_string(-0x90, 0x34, 0, (s32) "恋人達は、永遠に幸せな関係に", 0);
                set_kanji_string(-0x90, 0x44, 0, (s32) "なれるという伝説が…", 0);
                k_disp_start(4);
                k_disp_switch(0, 0);
                k_disp_switch(1, 0);
                k_disp_switch(2, 0);
                k_disp_switch(3, 0);
            }
        }
        break;
    case 6:
        if (((D_800E7378 & 3) == 3) && (D_801220D4 >= -0x13F)) {
            D_801220D4 -= 1;
        }
        temp_a1_4 = D_801220E2;
        D_801220E1 = temp_a1_4;
        D_801220E2 = temp_a1_4 - 1;
        D_801220E0 = temp_a1_4;
        if (temp_a1_4 == 0) {
            D_801220D0 = 0x80000000;
            var_v0_2 = &sp6C;
            do {
                var_v1_2 = &D_801B4000;
loop_32:
                *var_v0_2 = (s32) var_v1_2;
                var_v0_2 += 4;
                if ((s32) var_v1_2 < 0x801D0000) {
                    goto loop_32;
                }
                var_v0_2 = &sp6C;
            } while ((s32) (var_v1_2 + 0x4000) < 0x801D0000);
            func_8004A1A0((s32) &sp6C, 1, 6, 3, 2, 0x1EE, -0xA0, -0x78, 8, 0x15);
            load_palette((s32 *)0x801C8000, 0xE, 1, 3, 0);
            D_80122105 = 0;
            D_80122106 = 0;
            D_80122104 = 0;
            var_a2 = &D_800E6280;
            var_v1_3 = D_801217D0;
            var_t0 = 0;
            var_a0 = -0xD0;
            var_a1 = 9;
            do {
                temp_t6 = 0x1FF - var_t0;
                var_t0 += 1;
                var_v1_3->unk_04 = var_a0;
                var_v1_3->unk_0C = var_a1;
                var_a1 += 2;
                var_a0 += 0x50;
                var_v1_3 += 0x24;
                var_a2 += 1;
                *(s32 *)((unsigned char *)var_v1_3 + (-36)) = 0x01000000;
                *(s16 *)((unsigned char *)var_v1_3 + (-30)) = -0x78;
                *(s16 *)((unsigned char *)var_v1_3 + (-28)) = 0xFF;
                *(s16 *)((unsigned char *)var_v1_3 + (-26)) = 0xF0;
                *(s16 *)((unsigned char *)var_v1_3 + (-18)) = temp_t6;
                *(s16 *)((unsigned char *)var_v1_3 + (-8)) = 0x1000;
                *(s16 *)((unsigned char *)var_v1_3 + (-6)) = 0x1000;
                var_a2->unk_10A4 = 7;
            } while (var_t0 != 3);
            D_800E7326 = 8;
            D_801217FA = -0x6E;
            D_800E738D += 1;
        }
        break;
    case 7:
        temp_v0_2 = D_8012182E;
        D_8012182D = temp_v0_2;
        D_8012182C = temp_v0_2;
        D_8012180A = temp_v0_2;
        D_80121809 = temp_v0_2;
        D_80121808 = temp_v0_2;
        D_801217E6 = temp_v0_2;
        D_801217E5 = temp_v0_2;
        D_801217E4 = temp_v0_2;
        D_8012182E = temp_v0_2 + 1;
        if (temp_v0_2 >= 0x81U) {
            D_800E738D = temp_t0 + 1;
        } else {
            temp_t2_3 = D_80122106;
            D_80122105 = temp_t2_3;
            D_80122106 = temp_t2_3 + 1;
            D_80122104 = temp_t2_3;
        }
        break;
    case 8:
        if ((D_800E7378 & 3) == 3) {
            D_801217D4 -= 1;
            if (D_801217D4 < -0x160) {
                func_80044750(0xD1);
                func_8004E58C();
                cd_read_sectors(0x31U, 0x80180000, 0xA017);
                D_801220E0 = 0;
                D_800E738D += 1;
            } else {
                D_8012181C += 1;
            }
        }
        break;
    case 9:
        temp_a0 = D_801220E0;
        if ((u8) D_801220E0 < 0xFEU) {
            D_801220E0 = (D_801220E0 + 1) & 0xFF;
            func_8004AC18((s32) temp_a0);
        } else {
            func_8004AC18(0xFF);
            D_801217F4 = 0x80000000;
            D_801217D0->unk_00 = 0x80000000;
            D_801220F4 = 0x80000000;
            D_801220D0 = 0x01000000;
            D_801220E1 = 0x80;
            D_801220E2 = 0x80;
            D_801220E0 = 0x80;
            D_80121818 = 0x80000000;
            D_800E738D += 1;
            D_80122104 = 0xFF;
        }
        break;
    case 10:
        func_8004AC18(0xFF);
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) &sp6C, 0, 6, 3, 2, 0x1EF, -0xA0, -0x78, 8, 0xD);
            load_palette((s32 *)0x801C8000, 0xF, 1, 3, 0);
            D_801220D0 = 0x01000000;
            cd_read_sectors(0xAEU, 0x80180000, 0x9DF6);
            D_800E738D += 1;
        }
        break;
    case 11:
        func_8004AC18(0xFF);
        if (cd_read_sync_status() & 1) {
            tpage_buf_clear();
            var_t0_2 = 0;
            var_a1_2 = 0x10;
            do {
                sp64 = var_t0_2;
                load_palette((s32 *)0x80180000, var_a1_2 & 0xFF, 1, 2, var_t0_2);
                var_t0_2 += 1;
                var_a1_2 = var_t0_2 + 0x10;
            } while (var_t0_2 < 3);
            col2sepia(0x80180000, &D_800E78BC, 8, 4, 0);
            load_palette(&D_800E78BC, 0x10, 1, 2, 0);
            func_80048F64(0x60);
            D_80120651 = 5;
            D_80120652 = 4;
            D_80120688 = 0x41000000;
            D_80120653 = 0x84;
            D_8012065C = (u8 *)0x801A2624;
            D_80120660 = (u8 *)0x801A263C;
            D_80120684 = (u8 *)0x801A2610;
            D_80120666 = 0;
            D_80120654 = 8;
            D_80120655 = 0x80;
            D_80120676 = -0xA0;
            D_8012067A = -0x78;
            D_800E7384 = 0;
            D_800E738D += 1;
        }
        break;
    case 12:
        if (D_800E7384 == 0xC8) {
            D_80120652 |= 1;
        }
        temp_a0_2 = D_80122104;
        if (D_80122104 != 0) {
            D_80122104 = (D_80122104 - 1) & 0xFF;
            func_8004AC18((s32) temp_a0_2);
        } else if ((u32) D_800E7384 >= 0x12DU) {
            D_800E738D = temp_t0 + 1;
        } else if (!(D_80120652 & 1)) {
            D_800E738D = temp_t0 + 1;
        }
        break;
    case 13:
        do {
            var_v1_4 = 0x80186000;
            var_v0_3 = &sp6C;
loop_63:
            *var_v0_3 = var_v1_4;
            var_v1_4 += 0x4000;
            var_v0_3 += 4;
            if (var_v1_4 < 0x8019E000) {
                goto loop_63;
            }
        } while ((var_v1_4 + 0x4000) < 0x8019E000);
        func_8004A1A0((s32) &sp6C, 1, 6, 3, 2, 0x1EF, -0xA0, -0x78, 8, 0x15);
        D_801220D0 = 0x80000000;
        D_801220F4 = 0x01000000;
        load_palette((s32 *)0x8019E000, 0xF, 1, 2, 0);
        col2sepia(0x8019E000, &D_800E78BC, 8, 4, 0);
        var_v0_4 = &D_800E6280;
        var_v1_5 = 0;
        do {
            if ((u32) var_v0_4 < (u32) &D_800E6300) {
                *(s32 *)((unsigned char *)var_v0_4 + (6204)) = (s32) var_v0_4->unk_163C[0];
            } else {
                *(s32 *)((unsigned char *)var_v0_4 + (6204)) = (s32) *(&D_8019E000 + var_v1_5);
            }
            var_v0_4 += 4;
            var_v1_5 += 4;
        } while (var_v0_4 != &D_800E6480);
        load_palette(&D_800E7ABC, 0xF, 1, 2, 0);
        col2sepia(0x80180200, &D_800E78BC, 8, 4, 0);
        load_palette(&D_800E78BC, 0x11, 1, 2, 0);
        func_80048E78();
        func_8004E58C();
        func_80046290(0x39D, 0x060003A9, 0xF);
        func_80044750(0x300);
        func_8004E58C();
        set_kanji_string(-0x90, 0x44, 0, (s32) "あなたが…、", 0);
        k_disp_start(1);
        k_disp_switch(0, 0);
        D_800E7384 = 0;
        D_800E738D += 1;
        break;
    case 14:
        if (func_800460EC() & 4) {
            D_800E7384 = 0;
            D_800E738D += 1;
        }
        break;
    case 15:
        func_8014472C();
        func_80144908();
        if (D_800E7208 & 0xFFFF) {
            D_800E738D += 1;
        } else if ((u32) D_800E7384 >= 0x101U) {
            D_800E738D += 1;
        } else if ((func_800460EC() & 2) && ((u32) D_800E7384 >= 0x79U)) {
            D_800E738D += 1;
        }
        break;
    case 16:
        do {
            var_v1_6 = 0x801BA7F0;
            var_v0_5 = &sp6C;
loop_82:
            *var_v0_5 = var_v1_6;
            var_v1_6 += 0x4000;
            var_v0_5 += 4;
            if (var_v1_6 < 0x801D27F0) {
                goto loop_82;
            }
        } while ((var_v1_6 + 0x4000) < 0x801D27F0);
        func_8004A1A0((s32) &sp6C, 0, 6, 3, 2, 0x1EE, -0xA0, -0x78, 8, 0x15);
        D_801220F4 = 0x80000000;
        D_801220D0 = 0x01000000;
        load_palette((s32 *)0x801D27F0, 0xE, 1, 2, 0);
        col2sepia(0x801D27F0, &D_800E7ABC, 8, 4, 0);
        load_palette(&D_800E7ABC, 0xE, 1, 2, 0);
        col2sepia(0x80180400, &D_800E78BC, 8, 4, 0);
        load_palette(&D_800E78BC, 0x12, 1, 2, 0);
        func_80048E78();
        func_80048F64(0x60);
        D_80120651 = 5;
        D_80120652 = 0;
        D_80120688 = 0x41000000;
        D_80120653 = 0xA4;
        D_8012065C = (u8 *)0x801D6B60;
        D_80120660 = (u8 *)0x801D6B90;
        D_80120684 = (u8 *)0x801D6B54;
        D_80120666 = 0;
        D_80120654 = 8;
        D_80120655 = 0x80;
        D_80120676 = -0xA0;
        D_8012067A = -0x78;
        var_v0_6 = D_800E71DF;
        D_800E7384 = 0;
        D_800E738D += 1;
        if (var_v0_6 >= 0x11U) {
            var_v0_6 = ((var_v0_6 & 0xF) + 0x10) & 0xFF;
        }
        if ((var_v0_6 == 0x10) || (D_800E71DF = var_v0_6, (((u32) D_800E7200 >> 0x10) != 0))) {
            D_800E71DF = 0x10;
            func_80046290(0x3AA, 0x070003B5, 0xF);
            func_8004E58C();
            set_kanji_string(-0x90, 0x44, 0, (s32) "す・き・で・す", 0);
            k_disp_start(1);
            k_disp_switch(0, 0);
            func_80044750(0x300);
        }
        break;
    case 17:
        if (D_800E71DF == 0x10) {
            if ((func_800460EC() & 4) && ((u32) D_800E7384 >= 0x21U)) {
                D_80120652 = 3;
                D_80120668 = 1;
                D_800E7384 = 0;
                D_800E738D += 1;
            }
        } else {
            D_80120652 = 3;
            D_80120668 = 1;
            D_800E7384 = 0;
            D_800E738D = temp_t0 + 1;
        }
        break;
    case 18:
        if ((u32) D_800E7384 >= 0x41U) {
            D_800E7384 = 0;
            D_800E738D = temp_t0 + 1;
        }
        break;
    case 19:
        func_8004AC18((s32) D_800E7384);
        if ((u32) D_800E7384 >= 0xFFU) {
            func_8004284C();
        }
        break;
    }
    func_80144654();
}

void func_80144654(void) {
    if (D_800E6280.unk_F80 & 0x10) {
        k_disp_switch(0, 1);
        k_disp_switch(1, 1);
        k_disp_switch(2, 1);
        k_disp_switch(3, 1);
        func_80049A40(-0x9C, 8, 0x120, 0x58, 2, 0x808080, 0x83);
        dtd_on_tpage(0, 0, 2, 0, 0);
        return;
    }
    k_disp_switch(0, 0);
    k_disp_switch(1, 0);
    k_disp_switch(2, 0);
    k_disp_switch(3, 0);
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_8014472C);

void func_80144908(void) {
    if (!(D_80120696 & 1) && ((rand() & 0x7F) == 0x7F)) {
        D_80120695 = 4;
        D_80120696 = 3;
        D_801206CC = 0x41000000;
        D_80120697 = 0xA4;
        D_801206A0 = 0x801B27B0;
        D_801206A4 = 0x801B27D8;
        D_801206C8 = 0x801B27AC;
        D_801206AA = 0;
        D_80120698 = 8;
        D_80120699 = 0x80;
        D_801206BA = -0xA0;
        D_801206BE = -0x78;
    }
}

void func_801449DC(void) {
    func_8004AC18(0xFF);
    func_800452C4();
    func_80041168(0);
    func_80041584();
    func_80048DD0(0);
    func_80044750(0x71);
    func_80044750(0x7F);
    func_80042808();
}

void func_80144A34(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80142E24();
        break;
    case 1:
        func_80143580();
        break;
    case 2:
        func_801449DC();
        break;
    }
    if (D_800E6280.unk_F88 & 0x860) {
        func_80042940(2);
    }
}
