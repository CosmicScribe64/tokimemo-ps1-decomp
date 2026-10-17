#define MAIN_API_OVERRIDE_D_8012070C /* func_8014472C: pointer view of the record field (main_api.h: s32), T-22007 */
#include "common.h"
#include "ovl/NAME_ENT.h"

extern s32 *D_8012070C;

#ifdef NON_MATCHING
/* NON_MATCHING: T-22007, 0 rows in the code (funcdiff --resolve: only the 21 string-address pairs differ). The song-line strings are
 * not copied: they stay extern rodata labels, so the unit rodata still needs INCLUDE_RODATA pieces for them (rodata_pieces.py
 * writes only unowned symbols). */
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
void func_80142A80(void) {
    s32 pad; /* FAKE: unused; its stack home gives the original 72-byte frame. T-22007 */
    u8 sp43;
    s32 sp3C;
    u8 sp34[8];

    D_800E6280.unk_03A = 0x80;
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
#else
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80142A80);
#endif

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

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80143580);

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

void func_8014472C(void) {
    s32 level;
    s32 avg;

    if (func_800460EC() & 4) {
        level = func_8007C51C();
    } else {
        level = 0;
        if (((u32)D_800E6280.unk_F80 >> 16) != 0) {
            level = D_800E6280.unk_1100 & 0x3F;
        }
    }
    D_80125C80[D_800E6280.unk_10F8 & 3] = level;
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
