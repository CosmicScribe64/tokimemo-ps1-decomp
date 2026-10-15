#include "common.h"
#include "ovl/BUNKA_SD.h"

s32 func_80133540(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80044890(1, 0xBEFB, 0xBEE2, 0xCDC0, 0xCD82, 0xCD70);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    } else if (D_800E6280.unk_110D == 1) {
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        if (func_80044F94(1) == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else if (D_800E6280.unk_110D == 3) {
        cd_read_sectors(0xB4, 0x80180000, 0x4D9A);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    } else if (D_800E6280.unk_110D == 4) {
        if (cd_read_sync_status() & 1) {
            D_8013C6CC = D_8013B810;
            D_8013C6D0 = D_8013B938;
            D_800E6280.unk_F5F = 0xE;
            func_8004284C();
        }
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80133540", func_801336AC);

void func_8013376C(void) {
    func_80066C08(2);
    func_80064F48();
    func_80066334();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80134260();
        func_80044750(0x7F);
        break;
    case 1:
        func_80133540();
        break;
    case 2:
        func_80044750(0x200);
        func_80133924();
        break;
    case 3:
        func_80134418();
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        func_80133D54();
        break;
    case 12:
        func_801344A8();
        break;
    case 13:
        func_80044750(0x201);
        func_80133CC4();
        break;
    case 14:
        func_80134418();
        break;
    case 15:
    case 16:
    case 17:
    case 18:
        func_80133D54();
        break;
    case 19:
        func_80044750(0x202);
        func_8004284C();
        break;
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
        func_80133D54();
        break;
    case 37:
        func_80044750(0xC1);
        func_80044750(0x500);
        func_8004E9F4(1);
        func_8004284C();
        break;
    case 38:
        if ((u32) D_800E6280.unk_1104.u >= 0x1FU) {
            func_8004284C();
        }
        break;
    case 39:
        func_801344A8();
        break;
    case 40:
        func_80134314();
        break;
    default:
        func_80046500();
        break;
    }
    D_800E6280.unk_1104.u += 1;
}

void func_80133924(void) {
    func_80133984();
    k_speed_set(5);
    D_8013C6C8 = 0;
    D_8013C6D4 = 1;
    D_8013C6D8 = 0;
    D_8013C6DC = 0;
    D_8013C6E0 = 0;
    D_8013C6E4 = 0;
    func_8004284C();
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_8013C6EC;
extern s32 D_8013C6F0;
extern s32 D_8013C6F4;
extern void *D_8013C6E8;
/* end bulk_m2c declarations */
void func_80133984(void) {
    s32 var_s1;
    s32 var_s1_2;
    u8 *var_s0;

    var_s1 = 0;
    do {
        load_palette(D_8013C6F4, (var_s1 + 0x10) & 0xFF, 1, 1, var_s1);
        var_s1 += 1;
    } while (var_s1 < 0x10);
    var_s0 = D_8011ECD0;
    var_s1_2 = 0;
    do {
        func_80048F64(var_s1_2 + 0x60);
        *(s8 *)((unsigned char *)var_s0 + (6529)) = 4;
        *(s32 *)((unsigned char *)var_s0 + (6584)) = 0x01000000;
        *(s8 *)((unsigned char *)var_s0 + (6531)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (6530)) = 0x40;
        *(s32 *)((unsigned char *)var_s0 + (6540)) = (s32) D_8013C6EC;
        *(s32 *)((unsigned char *)var_s0 + (6544)) = (s32) D_8013C6F0;
        var_s1_2 += 1;
        var_s0 += 0x44;
        *(s16 *)((unsigned char *)var_s0 + (6484)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (6466)) = 1;
        *(s8 *)((unsigned char *)var_s0 + (6467)) = 0;
        *(s16 *)((unsigned char *)var_s0 + (6498)) = -0xA0;
        *(s16 *)((unsigned char *)var_s0 + (6502)) = -0x78;
        *(s8 *)((unsigned char *)var_s0 + (6465)) = 0x80;
        *(s8 *)((unsigned char *)var_s0 + (6527)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (6464)) = 0;
        *(void **)((unsigned char *)var_s0 + (6512)) = (void *) D_8013C6E8;
    } while (var_s1_2 != 0x20);
    func_801341A0(0x1F, 0xBF, 1);
    func_801341A0(1, 0, 1);
    load_tpage_buf_lock(5, (*(s32 *)D_8013C6E8), 1);
    load_tpage_buf_lock(6, *(s32 *)((unsigned char *)D_8013C6E8 + (4)), 1);
    load_tpage_buf_lock(7, *(s32 *)((unsigned char *)D_8013C6E8 + (8)), 1);
    load_tpage_buf_lock(8, *(s32 *)((unsigned char *)D_8013C6E8 + (12)), 1);
    load_tpage_buf_lock(9, *(s32 *)((unsigned char *)D_8013C6E8 + (16)), 1);
    load_tpage_buf_lock(0xA, *(s32 *)((unsigned char *)D_8013C6E8 + (20)), 1);
    load_tpage_buf_lock(0xB, *(s32 *)((unsigned char *)D_8013C6E8 + (24)), 1);
    load_tpage_buf_lock(0xC, *(s32 *)((unsigned char *)D_8013C6E8 + (28)), 1);
    load_tpage_buf_lock(0xD, *(s32 *)((unsigned char *)D_8013C6E8 + (32)), 1);
    load_tpage_buf_lock(0xE, *(s32 *)((unsigned char *)D_8013C6E8 + (36)), 1);
    load_tpage_buf_lock(0xF, *(s32 *)((unsigned char *)D_8013C6E8 + (40)), 1);
    load_tpage_buf_lock(0x25, *(s32 *)((unsigned char *)D_8013C6E8 + (44)), 1);
    load_tpage_buf_lock(0x26, *(s32 *)((unsigned char *)D_8013C6E8 + (48)), 1);
    load_tpage_buf_lock(0x27, *(s32 *)((unsigned char *)D_8013C6E8 + (52)), 1);
    load_tpage_buf_lock(0x28, *(s32 *)((unsigned char *)D_8013C6E8 + (56)), 1);
    load_tpage_buf_lock(0x29, *(s32 *)((unsigned char *)D_8013C6E8 + (60)), 1);
    load_tpage_buf_lock(0x2A, *(s32 *)((unsigned char *)D_8013C6E8 + (64)), 1);
    load_tpage_buf_lock(0x2B, *(s32 *)((unsigned char *)D_8013C6E8 + (68)), 1);
    load_tpage_buf_lock(0x2C, *(s32 *)((unsigned char *)D_8013C6E8 + (72)), 1);
    load_tpage_buf_lock(0x2D, *(s32 *)((unsigned char *)D_8013C6E8 + (76)), 1);
    load_tpage_buf_lock(0x2E, *(s32 *)((unsigned char *)D_8013C6E8 + (80)), 1);
}

void func_80133CC4(void) {
    func_801341A0(0x1F, 0xBF, 0);
    func_801341A0(0x1E, 0xC0, 1);
    func_801341A0(0x1D, 0xC1, 1);
    func_801341A0(8, 7, 1);
    func_801341A0(2, 0x4D, 1);
    func_801341A0(1, 0x2E, 1);
    func_801341A0(0, 0x50, 1);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80133540", func_80133D54);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A090);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A0AC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A0C4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A0E4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A0F8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A118);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A13C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A164);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A188);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A1B0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A1CC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A1E4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A200);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A210);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A22C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A244);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A254);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A268);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A284);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A294);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A2AC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A2C4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A2D0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A2F0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A304);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A31C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A338);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A350);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A364);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A37C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A394);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A3B0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A3C8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A3E8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A400);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A414);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A430);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A450);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A46C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A480);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A498);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A4B8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A4CC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A4E4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A504);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A51C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A534);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A554);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A56C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A590);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A5B0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A5CC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A5FC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A610);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A62C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A644);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A670);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A684);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A6A0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A6BC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A6D4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A6EC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A704);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A720);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A734);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A754);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A770);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A788);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A798);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A7B0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A7D0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A7E8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A808);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A83C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A858);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A860);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A868);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A870);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A878);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A880);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A888);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A898);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A8A0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A8A8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A8B0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A8B8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A8C0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A8CC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A8D4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A8E0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A8F0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A8FC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A908);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A914);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A91C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A924);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A92C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A934);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A940);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A948);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A950);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A95C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata", D_8013A968);

void func_80133FA4(void) {
    s32 i;
    s32 off;
    u8 *p;

    if (D_800E6280.unk_1100 == 1) {
        D_800E6280.unk_1100 += 1;
        set_kanji_string(-0x56, 0x40, 0, "「", 0);
        for (i = D_8013C6C8; i < 0x64; i++) {
            off = i * 4;
            set_kanji_string(-0x48, (s16)(((i - D_8013C6C8) * 0x10) + 0x40), 0,
                             *(s32 *)(D_8013C6CC + off) + 1, 0);
            if (**(u8 **)(D_8013C6CC + off) == 0x67) {
                if (D_800E6280.unk_110A == 0x23) {
                    set_kanji_string(-0x80, 0x70, 0, "奉行", 0);
                    set_kanji_string(-0x56, 0x80, 0, "「おのれー・・・。", 0);
                }
                if (!(D_800E6280.unk_F80 & 0x20)) {
                    /* FAKE: two statements load the table before the index; unk_110C is read
                     * as u8 here (lbu), as in func_80132708. T-11430 */
                    p = D_8013C6D0;
                    p += *(u8 *)&D_800E6280.unk_110C * 4;
                    func_80046290(*(s32 *)p, *(s32 *)(p + 4), D_800E6280.unk_F5F);
                    func_80044750(0x300);
                }
                *(u8 *)&D_800E6280.unk_110C += 2;
                D_8013C6C8 = i + 1;
                i = 0x64;
            }
        }
    }
    if (func_800460EC() == 2 && check_end_k() != 0) {
        D_800E6280.unk_1100 = 0;
    }
    check_k_scroll();
    k_disp_inc();
}

void func_801341A0(s32 arg0, s32 arg1, s32 arg2) {
    u8 *p;

    if (arg2 == 1) {
        p = D_8011ECD0 + arg0 * 0x44;
        p[0x1983] = 0x80;
    } else {
        p = D_8011ECD0 + arg0 * 0x44;
        p[0x1983] = 0;
    }
    *(s16 *)(p + 0x1998) = arg1;
}

void func_801341F0(s32 arg0) {
    k_disp_start(set_kanji_string(-0x80, 0x30, 0, arg0, 0));
}

void func_80134228(u32 arg0) {
    if (arg0 < (u32)D_800E6280.unk_1104.u) {
        func_8004284C();
    }
}
