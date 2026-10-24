#define MAIN_API_OVERRIDE_func_8004AC18 /* original called it undeclared (implicit int): its $v0 result keeps v0 busy after the calls in func_80143580, T-25604 */
#define MAIN_API_OVERRIDE_D_8012070C /* func_8014472C: pointer view of the record field (main_api.h: s32), T-22007 */
#include "common.h"
#include "ovl/NAME_ENT.h"

extern s32 *D_8012070C;
extern s32 func_8004AC18();

/* The 21 song-line strings are placed from their rodata pieces (rodata_pieces.py), not copied into the source. T-22007 */
extern u8 D_8014C4F0[];
extern u8 D_8014C4FC[];
extern u8 D_8014C510[];
extern u8 D_8014C534[];
extern u8 D_8014C554[];
extern u8 D_8014C578[];
extern u8 D_8014C5A4[];
extern u8 D_8014C5B8[];
extern u8 D_8014C5D8[];
extern u8 D_8014C5F0[];
extern u8 D_8014C60C[];
extern u8 D_8014C62C[];
extern u8 D_8014C63C[];
extern u8 D_8014C65C[];
extern u8 D_8014C668[];
extern u8 D_8014C684[];
extern u8 D_8014C6A0[];
extern u8 D_8014C6B0[];
extern u8 D_8014C6D0[];
extern u8 D_8014C6E0[];
extern u8 D_8014C6F4[];
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C4F0);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C4FC);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C510);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C534);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C554);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C578);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C5A4);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C5B8);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C5D8);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C5F0);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C60C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C62C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C63C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C65C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C668);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C684);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C6A0);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C6B0);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C6D0);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C6E0);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80142A80.rodata", D_8014C6F4);

void func_80142A80(void) {
    s32 pad; /* FAKE: unused; its stack home gives the original 72-byte frame. T-22007 */
    u8 sp43;
    s32 sp3C;
    u8 sp34[8];

    D_800E6280.screen_brightness = 0x80;
    sp43 = func_8004ADD4();
    func_8004ADAC(0);
    func_80048DD0(0);
    func_8006C760(1);
    func_8006BD6C(0);
    back_clear_switch(1);
    func_8004E58C();
    sp3C = get_k_speed();
    k_speed_set(3U);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C4F0, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C4FC, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C510, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C534, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C554, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C578, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C5A4, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C5B8, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C5D8, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C5F0, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C60C, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C62C, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C63C, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C65C, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C668, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C684, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C6A0, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C6B0, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C6D0, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C6E0, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) D_8014C6F4, 0);
    k_disp_start(0);
    set_movie_offset(0x18, 0x10);
    func_80056BA8(0, 0x33B, 0x820);
    func_80088070();
    sp34[0] = 0x80;
    func_80045414(0xE, 0, sp34);
    func_80045414(9, 0, 0);
    func_80045414(0xA, 0, 0);
    func_80053650(0x80167000, 0x80162000, 0x80162000);
    func_8009C210(1);
    func_80041168(0);
    func_8009C674(0);
    func_80048DD0(0);
    main_state_set(0x12);
    func_8004ADAC((s32) sp43);
    k_speed_set((u8) sp3C);
    func_8006C760(0);
    func_8006BC28(0);
    func_8006BD6C(0);
    D_800B3CF8 = 0;
    D_800B3CFC = 0;
    D_800B3D00 = 0;
}

void func_80142E24(void) {
    s32 row;
    s32 addresses[8];
    u8 step;

    /* Four address columns per 0x10000-byte bank. T-20010. */
    for (row = 0; row < 2; row++) {
        addresses[row * 4 + 0] = 0x80180000 + row * 0x10000;
        addresses[row * 4 + 1] = 0x80184000 + row * 0x10000;
        addresses[row * 4 + 2] = 0x80188000 + row * 0x10000;
        addresses[row * 4 + 3] = 0x8018C000 + row * 0x10000;
    }
    step = D_800E6280.substep;
    switch (step) {
    case 0:
        D_800E6280.screen_brightness = 0x80;
        draw2d3d(1, 0);
        func_80048DAC(1);
        func_8006BD6C(0);
        func_80048DD0(0);
        func_80048E78();
        func_80041584();
        func_8004E58C();
        back_clear_switch(1);
        D_800E6280.clear_color_g = 0;
        D_800E6280.clear_color_r = 0;
        D_800E6280.clear_color_b = 0;
        func_80044750(0x7F);
        func_80044890(1, 0xC592, 0xC580, 0xCFA7, 0xCF8B, 0xCF83);
        D_800E6280.substep = D_800E6280.substep + 1;
        return;
    case 1:
        if (func_80044E8C() == 1) {
            func_8004500C(1, 0);
            cd_read_sectors(0x21U, 0x80180000, 0x9FD5);
            D_800E6280.substep = D_800E6280.substep + 1;
            return;
        }
    default:
        return;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) addresses, 0, 4, 2, 2, 0x1FF, -0xA0, -0x78, 8, 9);
            load_palette(0x80190000, 0x1F, 1, 3, 0);
            ((BgLayer *)&D_801220D0)[0].unk_00 = 0x80000000;
            D_800E6280.substep = D_800E6280.substep + 1;
            return;
        }
        break;
    case 3:
        cd_read_sectors(0x21U, 0x80180000, 0xA048);
        D_800E6280.substep = D_800E6280.substep + 1;
        return;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) addresses, 0, 4, 2, 2, 0x1FE, -0xA0, -0x78, 8, 0xB);
            load_palette(0x80190000, 0x1E, 1, 3, 0);
            ((BgLayer *)&D_801220D0)[0].unk_00 = 0x80000000;
            D_800E6280.substep = D_800E6280.substep + 1;
            return;
        }
        break;
    case 5:
        cd_read_sectors(0x21U, 0x80180000, 0x9FF6);
        D_800E6280.substep = D_800E6280.substep + 1;
        return;
    case 6:
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) addresses, 0, 4, 2, 2, 0x1FD, -0xA0, -0x78, 8, 0xD);
            load_palette(0x80190000, 0x1D, 1, 3, 0);
            ((BgLayer *)&D_801220D0)[0].unk_00 = 0x80000000;
            D_800E6280.substep = D_800E6280.substep + 1;
            return;
        }
        break;
    case 7:
        cd_read_sectors(0x41U, 0x80180000, 0x9F22);
        D_800E6280.substep = D_800E6280.substep + 1;
        return;
    case 8:
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) addresses, 1, 8, 4, 2, 0x1EE, -0x140, -0x78, 8, 0x15);
            load_palette(0x801A0000, 0xE, 1, 3, 0);
            ((BgLayer *)&D_801220D0)[1].unk_00 = 0x51000000;
            ((BgLayer *)&D_801220D0)[1].unk_11 = 0;
            ((BgLayer *)&D_801220D0)[1].unk_10 = 0;
            D_800E6280.substep = D_800E6280.substep + 1;
            ((BgLayer *)&D_801220D0)[1].unk_12 = 0;
            return;
        }
        break;
    case 9:
        cd_read_sectors(0x31U, 0x801B0000, 0x9FA4);
        D_800E6280.substep = D_800E6280.substep + 1;
        return;
    case 10:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x41U, 0x80180000, 0x9EE1);
            D_800E6280.substep = D_800E6280.substep + 1;
            return;
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) addresses, 0, 8, 4, 2, 0x1EF, -0xA0, -0x78, 8, 5);
            load_palette(0x801A0000, 0xF, 1, 3, 0);
            ((BgLayer *)&D_801220D0)[0].unk_11 = 0;
            ((BgLayer *)&D_801220D0)[0].unk_10 = 0;
            ((BgLayer *)&D_801220D0)[0].unk_00 = 0x51000000;
            D_800E6280.substep = D_800E6280.substep + 1;
            ((BgLayer *)&D_801220D0)[0].unk_12 = 0;
            return;
        }
        break;
    case 12:
        cd_read_sectors(0x41U, 0x80180000, 0x9F63);
        D_800E6280.substep = D_800E6280.substep + 1;
        return;
    case 13:
        if (cd_read_sync_status() & 1) {
            func_80046290(0x06000BC8, 0x11000CBA, 0xF);
            func_80044750(0x300);
            func_8004E58C();
            set_kanji_string(-0x90, 0x14, 0, "始まりがあれば終わりがあるように、", 0);
            set_kanji_string(-0x90, 0x24, 0, "出会いがあればまた、別れもあるのです。", 0);
            set_kanji_string(-0x90, 0x34, 0, "永遠に続く二人の関係、", 0);
            set_kanji_string(-0x90, 0x44, 0, "それはどんなに幸せなことでしょう。", 0);
            k_disp_start(4);
            k_disp_switch(0, 0);
            k_disp_switch(1, 0);
            k_disp_switch(2, 0);
            k_disp_switch(3, 0);
            func_8004284C();
        }
        break;
    }
}

void func_80143498(void) {
    if ((func_800460EC() & 2) && D_8014D170 == 0) {
        func_80046290(0x06000CBB, 0x12000E0C, 0xF);
        func_80044750(0x300);
        func_8004E58C();
        set_kanji_string(-0x90, 0x14, 0, "ここ、きらめき高校には、", 0);
        set_kanji_string(-0x90, 0x24, 0, "一つの伝説があります。", 0);
        set_kanji_string(-0x90, 0x34, 0, "校庭のはずれにある一本の古木。", 0);
        k_disp_start(3);
        k_disp_switch(0, 0);
        k_disp_switch(1, 0);
        k_disp_switch(2, 0);
        D_8014D170 = 1;
    }
}

/* Same-file functions defined below. */
void func_80144654(void);
void func_8014472C(void);
void func_80144908(void);

/* Introduction scene state machine. T-25604: case 9 hides the three records with one chain
 * (D_801217D0[2] is read back, so uopt keeps it in a2 and the selector gets t0), func_8004AC18 is
 * called undeclared (its $v0 result keeps v0 busy after each call), and every tile fill counts with
 * i. */
void func_80143580(void) {
    s32 pad0; /* FAKE: unused; with pad1 its stack home gives the original 0x90 frame. T-25604 */
    s32 tiles[8];
    s32 pad1; /* FAKE: unused, see pad0. T-25604 */
    s32 i;
    u8 step;

    /* FAKE: T-25319, T-25604: a reference to unk_F5F before the first branch satisfies the entry
     * rule of tools/cvt_pass.py, so the CVTs of the case 16/17 compares go and F5F is one promoted
     * range (v0) as in the target; uopt removes the self-store. */
    D_800E6280.current_girl = D_800E6280.current_girl;
    for (i = 0; i < 8; i++) {
        tiles[i] = 0x80180000 + i * 0x4000;
    }
    step = D_800E6280.substep;
    switch (step) {
    case 0:
        if ((((BgLayer *)&D_801220D0)[0].unk_10 = ((BgLayer *)&D_801220D0)[0].unk_11 = ((BgLayer *)&D_801220D0)[0].unk_12++) >= 0x81U) {
            ((BgLayer *)&D_801220D0)[0].unk_00 = 0x01000000;
            D_800E6280.substep++;
            D_8014D170 = 0;
        }
        break;
    case 1:
        if ((D_800E6280.frame_count & 3) == 3) {
            if (((BgLayer *)&D_801220D0)[0].unk_04-- < -0xF0) {
                D_800E6280.substep++;
                ((BgLayer *)&D_801220D0)[0].unk_00 = 0x51000000;
            }
        }
        break;
    case 2:
        if ((((BgLayer *)&D_801220D0)[0].unk_10 = ((BgLayer *)&D_801220D0)[0].unk_11 = ((BgLayer *)&D_801220D0)[0].unk_12--) != 0) {
            if ((((BgLayer *)&D_801220D0)[1].unk_10 = ((BgLayer *)&D_801220D0)[1].unk_11 = ((BgLayer *)&D_801220D0)[1].unk_12++) >= 0x81U) {
                ((BgLayer *)&D_801220D0)[0].unk_00 = 0x80000000;
                ((BgLayer *)&D_801220D0)[1].unk_00 = 0x01000000;
                D_800E6280.substep++;
            }
        }
        /* fallthrough */
    case 3:
        if ((D_800E6280.frame_count & 3) == 3) {
            ((BgLayer *)&D_801220D0)[0].unk_04 -= 1;
            if (((BgLayer *)&D_801220D0)[1].unk_04++ >= -0xEF) {
                func_8004A1A0((s32) tiles, 0, 8, 4, 2, 0x1EF, -0xA0, -0x78, 8, 5);
                load_palette((s32 *)0x801A0000U, 0xF, 1, 3, 0);
                ((BgLayer *)&D_801220D0)[0].unk_11 = 0;
                ((BgLayer *)&D_801220D0)[0].unk_00 = 0x51000000;
                ((BgLayer *)&D_801220D0)[0].unk_12 = 0;
                ((BgLayer *)&D_801220D0)[0].unk_10 = 0;
                D_800E6280.substep++;
            }
        }
        break;
    case 4:
        func_80143498();
        if ((((BgLayer *)&D_801220D0)[1].unk_10 = ((BgLayer *)&D_801220D0)[1].unk_11 = ((BgLayer *)&D_801220D0)[1].unk_12--) != 0) {
            if ((((BgLayer *)&D_801220D0)[0].unk_10 = ((BgLayer *)&D_801220D0)[0].unk_11 = ((BgLayer *)&D_801220D0)[0].unk_12++) >= 0x81U) {
                ((BgLayer *)&D_801220D0)[1].unk_00 = 0x80000000;
                ((BgLayer *)&D_801220D0)[0].unk_00 = 0x01000000;
                D_800E6280.substep++;
            }
        }
        /* fallthrough */
    case 5:
        if ((D_800E6280.frame_count & 3) == 3) {
            if (((BgLayer *)&D_801220D0)[1].unk_04 < -0xA0) {
                ((BgLayer *)&D_801220D0)[1].unk_04 += 1;
            }
            if (((BgLayer *)&D_801220D0)[0].unk_04-- < -0x140) {
                ((BgLayer *)&D_801220D0)[0].unk_00 = 0x51000000;
                D_800E6280.substep++;
            }
            /* FAKE: T-25319, T-25604: ^ 0 drops the a0 argument preference of the promoted field;
             * gives the target's move a0,v0 and F8 in a0. */
            func_8004B19C(((BgLayer *)&D_801220D0)[0].unk_04 ^ 0, 0, 0);
            if (((BgLayer *)&D_801220D0)[0].unk_04 == -0x118) {
                func_8004E58C();
                set_kanji_string(-0x90, 0x14, 0, "そのたもとで卒業の日に、", 0);
                set_kanji_string(-0x90, 0x24, 0, "女の子からの告白で生まれた", 0);
                set_kanji_string(-0x90, 0x34, 0, "恋人達は、永遠に幸せな関係に", 0);
                set_kanji_string(-0x90, 0x44, 0, "なれるという伝説が…", 0);
                k_disp_start(4);
                k_disp_switch(0, 0);
                k_disp_switch(1, 0);
                k_disp_switch(2, 0);
                k_disp_switch(3, 0);
            }
        }
        break;
    case 6:
        if (((D_800E6280.frame_count & 3) == 3) && (((BgLayer *)&D_801220D0)[0].unk_04 >= -0x13F)) {
            ((BgLayer *)&D_801220D0)[0].unk_04 -= 1;
        }
        if ((((BgLayer *)&D_801220D0)[0].unk_10 = ((BgLayer *)&D_801220D0)[0].unk_11 = ((BgLayer *)&D_801220D0)[0].unk_12--) == 0) {
            ((BgLayer *)&D_801220D0)[0].unk_00 = 0x80000000;
            /* FAKE: T-25131, T-25319, T-25604 (from T-20008): the target has this nested fill loop
             * (the outer loop runs once); its original source form is unknown. */
            for (i = 0; i < 8; i++) {
                for (i = 0; i < 8; i++) {
                    tiles[i] = 0x801B0000 + i * 0x4000;
                }
            }
            func_8004A1A0((s32) tiles, 1, 6, 3, 2, 0x1EE, -0xA0, -0x78, 8, 0x15);
            load_palette((s32 *)0x801C8000U, 0xE, 1, 3, 0);
            ((BgLayer *)&D_801220D0)[1].unk_10 = ((BgLayer *)&D_801220D0)[1].unk_11 = ((BgLayer *)&D_801220D0)[1].unk_12 = 0;
            for (i = 0; i < 3; i++) {
                *(u32 *)&D_801217D0[i].flags = 0x01000000;
                D_801217D0[i].x = i * 0x50 - 0xD0;
                D_801217D0[i].y = -0x78;
                D_801217D0[i].w = 0xFF;
                D_801217D0[i].h = 0xF0;
                D_801217D0[i].unk_0C = i * 2 + 9;
                D_801217D0[i].unk_12 = 0x1FF - i;
                D_801217D0[i].unk_1C = 0x1000;
                D_801217D0[i].unk_1E = 0x1000;
                D_800E6280.quad_layer[i] = 7;
            }
            D_800E6280.quad_layer[1] = 8;
            D_801217D0[1].y = -0x6E;
            D_800E6280.substep++;
        }
        break;
    case 7:
        if ((D_801217D0[0].tint_r = D_801217D0[0].tint_g = D_801217D0[0].tint_b = D_801217D0[1].tint_r = D_801217D0[1].tint_g = D_801217D0[1].tint_b = D_801217D0[2].tint_r = D_801217D0[2].tint_g = D_801217D0[2].tint_b++) >= 0x81U) {
            D_800E6280.substep++;
        } else {
            ((BgLayer *)&D_801220D0)[1].unk_10 = ((BgLayer *)&D_801220D0)[1].unk_11 = ((BgLayer *)&D_801220D0)[1].unk_12++;
        }
        break;
    case 8:
        if ((D_800E6280.frame_count & 3) == 3) {
            if (D_801217D0[0].x-- < -0x160) {
                func_80044750(0xD1);
                func_8004E58C();
                cd_read_sectors(0x31U, 0x80180000U, 0xA017);
                ((BgLayer *)&D_801220D0)[0].unk_10 = 0;
                D_800E6280.substep++;
            } else {
                D_801217D0[2].x += 1;
            }
        }
        break;
    case 9:
        if ((u8) ((BgLayer *)&D_801220D0)[0].unk_10 < 0xFEU) {
            func_8004AC18(((BgLayer *)&D_801220D0)[0].unk_10++);
        } else {
            func_8004AC18(0xFF);
            *(u32 *)&D_801217D0->flags = *(u32 *)&D_801217D0[1].flags = *(u32 *)&D_801217D0[2].flags = 0x80000000;
            ((BgLayer *)&D_801220D0)[1].unk_00 = 0x80000000;
            ((BgLayer *)&D_801220D0)[0].unk_00 = 0x01000000;
            ((BgLayer *)&D_801220D0)[0].unk_11 = ((BgLayer *)&D_801220D0)[0].unk_12 = ((BgLayer *)&D_801220D0)[0].unk_10 = 0x80;
            ((BgLayer *)&D_801220D0)[1].unk_10 = 0xFF;
            D_800E6280.substep++;
        }
        break;
    case 10:
        func_8004AC18(0xFF);
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) tiles, 0, 6, 3, 2, 0x1EF, -0xA0, -0x78, 8, 0xD);
            load_palette((s32 *)0x801C8000U, 0xF, 1, 3, 0);
            ((BgLayer *)&D_801220D0)[0].unk_00 = 0x01000000;
            cd_read_sectors(0xAEU, 0x80180000U, 0x9DF6);
            D_800E6280.substep++;
        }
        break;
    case 11:
        func_8004AC18(0xFF);
        if (cd_read_sync_status() & 1) {
            tpage_buf_clear();
            i = 0;
            do {
                load_palette((s32 *)0x80180000U, (i + 0x10) & 0xFF, 1, 2, i);
                i += 1;
            } while (i < 3);
            col2sepia(0x80180000U, &D_800E6280.scratch_words[0], 8, 4, 0);
            load_palette(&D_800E6280.scratch_words[0], 0x10, 1, 2, 0);
            i = 0x60;
            func_80048F64(i);
            ((Actor *)D_8011ECD0)[i].unk_01 = 5;
            ((Actor *)D_8011ECD0)[i].flags = 4;
            ((Actor *)D_8011ECD0)[i].quad_flags = 0x41000000;
            ((Actor *)D_8011ECD0)[i].draw_flags = 0x84;
            ((Actor *)D_8011ECD0)[i].frame_table = (u8 *)0x801A2624;
            ((Actor *)D_8011ECD0)[i].sequence_table = (u8 *)0x801A263C;
            ((Actor *)D_8011ECD0)[i].tpage_table = (u8 *)0x801A2610;
            ((Actor *)D_8011ECD0)[i].sequence_slot = 0;
            ((Actor *)D_8011ECD0)[i].unk_04 = 8;
            ((Actor *)D_8011ECD0)[i].unk_05 = 0x80;
            ((Actor *)D_8011ECD0)[i].x = -0xA0;
            ((Actor *)D_8011ECD0)[i].y = -0x78;
            D_800E6280.step_timer.u = 0;
            D_800E6280.substep++;
        }
        break;
    case 12:
        if (D_800E6280.step_timer.u == 0xC8) {
            D_80120652 |= 1;
        }
        if (((BgLayer *)&D_801220D0)[1].unk_10 != 0) {
            func_8004AC18(((BgLayer *)&D_801220D0)[1].unk_10--);
        } else if ((u32) D_800E6280.step_timer.u >= 0x12DU) {
            D_800E6280.substep++;
        } else if (!(D_80120652 & 1)) {
            D_800E6280.substep++;
        }
        break;
    case 13:
        /* FAKE: T-25131, T-25319, T-25604 (from T-20008): nested fill loop as in case 6. */
        for (i = 0; i < 6; i++) {
            for (i = 0; i < 6; i++) {
                tiles[i] = 0x80186000 + i * 0x4000;
            }
        }
        func_8004A1A0((s32) tiles, 1, 6, 3, 2, 0x1EF, -0xA0, -0x78, 8, 0x15);
        ((BgLayer *)&D_801220D0)[0].unk_00 = 0x80000000;
        ((BgLayer *)&D_801220D0)[1].unk_00 = 0x01000000;
        load_palette((s32 *)0x8019E000U, 0xF, 1, 2, 0);
        col2sepia(0x8019E000U, &D_800E6280.scratch_words[0], 8, 4, 0);
        for (i = 0; i != 128; i++) {
            if (i < 32) {
                ((s32 *)D_800E6280.scratch_buffer)[i] = D_800E6280.scratch_words[i];
            } else {
                ((s32 *)D_800E6280.scratch_buffer)[i] = ((s32 *)0x8019E000)[i];
            }
        }
        load_palette((s32 *)D_800E6280.scratch_buffer, 0xF, 1, 2, 0);
        col2sepia(0x80180200U, &D_800E6280.scratch_words[0], 8, 4, 0);
        load_palette(&D_800E6280.scratch_words[0], 0x11, 1, 2, 0);
        func_80048E78();
        func_8004E58C();
        func_80046290(0x39D, 0x060003A9, 0xF);
        func_80044750(0x300);
        func_8004E58C();
        set_kanji_string(-0x90, 0x44, 0, "あなたが…、", 0);
        k_disp_start(1);
        k_disp_switch(0, 0);
        D_800E6280.step_timer.u = 0;
        D_800E6280.substep++;
        break;
    case 14:
        if (func_800460EC() & 4) {
            D_800E6280.step_timer.u = 0;
            D_800E6280.substep++;
        }
        break;
    case 15:
        func_8014472C();
        func_80144908();
        if (D_800E6280.pad_pressed & 0xFFFF) {
            D_800E6280.substep++;
        } else if ((u32) D_800E6280.step_timer.u >= 0x101U) {
            D_800E6280.substep++;
        } else if ((func_800460EC() & 2) && ((u32) D_800E6280.step_timer.u >= 0x79U)) {
            D_800E6280.substep++;
        }
        break;
    case 16:
        /* FAKE: T-25131, T-25319, T-25604 (from T-20008): nested fill loop as in case 6. */
        for (i = 0; i < 6; i++) {
            for (i = 0; i < 6; i++) {
                tiles[i] = 0x801BA7F0 + i * 0x4000;
            }
        }
        func_8004A1A0((s32) tiles, 0, 6, 3, 2, 0x1EE, -0xA0, -0x78, 8, 0x15);
        ((BgLayer *)&D_801220D0)[1].unk_00 = 0x80000000;
        ((BgLayer *)&D_801220D0)[0].unk_00 = 0x01000000;
        load_palette((s32 *)0x801D27F0U, 0xE, 1, 2, 0);
        col2sepia(0x801D27F0U, (s32 *)D_800E6280.scratch_buffer, 8, 4, 0);
        load_palette((s32 *)D_800E6280.scratch_buffer, 0xE, 1, 2, 0);
        col2sepia(0x80180400U, &D_800E6280.scratch_words[0], 8, 4, 0);
        load_palette(&D_800E6280.scratch_words[0], 0x12, 1, 2, 0);
        func_80048E78();
        i = 0x60;
        func_80048F64(i);
        ((Actor *)D_8011ECD0)[i].unk_01 = 5;
        ((Actor *)D_8011ECD0)[i].flags = 0;
        ((Actor *)D_8011ECD0)[i].quad_flags = 0x41000000;
        ((Actor *)D_8011ECD0)[i].draw_flags = 0xA4;
        ((Actor *)D_8011ECD0)[i].frame_table = (u8 *)0x801D6B60;
        ((Actor *)D_8011ECD0)[i].sequence_table = (u8 *)0x801D6B90;
        ((Actor *)D_8011ECD0)[i].tpage_table = (u8 *)0x801D6B54;
        ((Actor *)D_8011ECD0)[i].sequence_slot = 0;
        ((Actor *)D_8011ECD0)[i].unk_04 = 8;
        ((Actor *)D_8011ECD0)[i].unk_05 = 0x80;
        ((Actor *)D_8011ECD0)[i].x = -0xA0;
        ((Actor *)D_8011ECD0)[i].y = -0x78;
        D_800E6280.step_timer.u = 0;
        D_800E6280.substep++;
        if (D_800E6280.current_girl >= 0x11U) {
            D_800E6280.current_girl = (D_800E6280.current_girl & 0xF) + 0x10;
        }
        if ((D_800E6280.current_girl == 0x10) || (((u32) D_800E6280.pad_held >> 0x10) != 0)) {
            D_800E6280.current_girl = 0x10;
            func_80046290(0x3AA, 0x070003B5, 0xF);
            func_8004E58C();
            set_kanji_string(-0x90, 0x44, 0, "す・き・で・す", 0);
            k_disp_start(1);
            k_disp_switch(0, 0);
            func_80044750(0x300);
        }
        break;
    case 17:
        if (D_800E6280.current_girl == 0x10) {
            if ((func_800460EC() & 4) && ((u32) D_800E6280.step_timer.u >= 0x21U)) {
                D_80120652 = 3;
                D_80120668 = 1;
                D_800E6280.step_timer.u = 0;
                D_800E6280.substep++;
            }
        } else {
            D_80120652 = 3;
            D_80120668 = 1;
            D_800E6280.substep++;
            D_800E6280.step_timer.u = 0;
        }
        break;
    case 18:
        if ((u32) D_800E6280.step_timer.u >= 0x41U) {
            D_800E6280.step_timer.u = 0;
            D_800E6280.substep++;
        }
        break;
    case 19:
        func_8004AC18((s32) D_800E6280.step_timer.u);
        if ((u32) D_800E6280.step_timer.u >= 0xFFU) {
            func_8004284C();
        }
        break;
    }
    func_80144654();
}

void func_80144654(void) {
    if (D_800E6280.pad_held & 0x10) {
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

void func_8014472C(void) {
    s32 level;
    s32 avg;

    if (func_800460EC() & 4) {
        level = getCDlevel();
    } else {
        level = 0;
        if (((u32)D_800E6280.pad_held >> 16) != 0) {
            level = D_800E6280.stage_timer & 0x3F;
        }
    }
    D_80125C80[D_800E6280.frame_count & 3] = level;
    /* FAKE: the original computes a four-sample average that nothing reads; the empty compare keeps it from being dropped. T-22007 */
    avg = (D_80125C80[0] + D_80125C80[1] + D_80125C80[2] + D_80125C80[3]) / 4;
    if (avg == 0x7FFF) {
    }
    if (func_800460EC() & 4) {
        if (!((u8)D_801206DA & 1) || !(D_801206DB & 0x80)) {
            func_80048F64(0x62);
            D_801206D9 = 3;
            D_801206DA = 0x11;
            D_80120710 = 0x41000000;
            D_801206DB = 0xAC;
            D_801206E4 = 0x801B27B0;
            D_801206E8 = 0x801B27D8;
            D_8012070C = (s32 *)0x801B27AC;
            D_801206EE = 1;
            D_801206DC = 8;
            D_801206DD = 0x80;
            D_801206FE = -0xA0;
            D_80120702 = -0x78;
            load_tpage_buf_lock(5, *D_8012070C, 1);
        }
    } else {
        func_80048F64(0x62);
        D_801206DB = 0;
        D_801206DA = 0;
        D_801206F0 = 0;
        D_801206E0 = 0;
    }
}

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
    D_800E6280.step_timer.w += 1;
    switch (D_800E6280.step) {
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
    if (D_800E6280.pad_pressed & 0x860) {
        func_80042940(2);
    }
}
