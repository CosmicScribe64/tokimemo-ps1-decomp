#define MAIN_API_OVERRIDE_D_80120656 /* 0x82 must not sign-extend (main_api.h: s8) */
#include "common.h"
#include "ovl/BUNKA_SD.h"

extern u8 D_80120656;

s32 func_80135160(void) {
    if (D_800E6280.unk_110D == 0) {
        cd_read_sectors(0x61, 0x80180000, 0x4E4E);
        D_800E6280.unk_110D += 1;
    } else if ((D_800E6280.unk_110D == 1) && (cd_read_sync_status() & 1)) {
        func_8004284C();
    }
}

void func_801351DC(void) {
    s32 i;

    if ((D_800E6280.unk_110A >= 3) && (D_800E6280.unk_110A < 9)) {
        if (D_800E6280.unk_F80 & 0x20) {
            func_801352F4();
            D_80120656 = 0x82;
            D_8012069A = 0x82; for (i = 0; i < 2; i++) {
                D_8011ED58[(i + 1) * 0x110 + 0x18BA] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x18FE] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x1942] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x1876] = 0x82;
            }
        } else {
            D_80120656 = 1;
            D_8012069A = 1; for (i = 0; i < 2; i++) {
                D_8011ED58[(i + 1) * 0x110 + 0x18BA] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x18FE] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x1942] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x1876] = 1;
            }
        }
    }
    if ((D_800E6280.unk_110A >= 3) && (D_800E6280.unk_110A < 8) && (D_800E6280.unk_F80 & 0x40)) {
        func_80042940(7);
    }
    func_801352F4();
}

void func_801352F4(void) {
    func_80066C08(2);
    func_80064F48();
    func_80066334();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80134260();
        return;
    case 1:
        func_80135160();
        return;
    case 2:
        func_8013540C();
        return;
    case 3:
        func_80134418();
        return;
    case 4:
        func_80135440();
        return;
    case 5:
        func_801355AC();
        return;
    case 6:
        func_801354C8();
        return;
    case 7:
        func_80044750(0xC1);
        func_8004E9F4(1);
        func_8004284C();
        return;
    case 8:
        func_801344A8();
        return;
    case 9:
        func_80134314();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8013540C(void) {
    func_8004500C(1, 0x201);
    func_80135630();
    func_8004284C();
}

void func_80135440(void) {
    if (D_800E6280.unk_1104.u == 0) {
        D_80120652 = 5;
    }
    D_800E6280.unk_1104.u += 1;
    if (D_80122EAC == 3) {
        if (D_800E6280.unk_1104.u >= 0x1C2) {
            func_8004284C();
        }
    } else {
        if (!(D_80120652 & 1)) {
            func_8004284C();
        }
    }
}

s32 func_801354C8(void) {
    if (D_800E6280.unk_1104.u == 0) {
        *(u8 *)&D_800E6280.unk_110B = 0x80;
        D_80120653 = 0;
        D_801206DB = 0;
        D_8012069B = 0x80;
        D_80120697 = 0x80;
        D_80120696 = 5;
    }
    func_80066334();
    D_800E6280.unk_1104.u += 1;
    if (D_800E6280.unk_1104.u % 9U == 0) {
        if ((D_801206D7 += 1) >= 0x15U) {
            D_801206D7 = 0x11;
        }
    }
    if (!(D_80120696 & 1)) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80135160", func_801355AC);

void func_80135630(void) {
    s32 var_s0;

    var_s0 = 0;
    do {
        load_palette(D_8013C72C, (var_s0 + 0x10) & 0xFF, 1, 1, var_s0);
        var_s0 += 1;
    } while (var_s0 != 0x10);
    func_80048F64(0x60);
    D_80120651 = 4;
    D_80120688 = 0x01000000;
    D_80120653 = 0x80;
    D_8012065C = D_8013C724;
    D_80120660 = D_8013C728;
    D_80120684 = D_8013C720;
    D_80120666 = (s16) D_80122EAC;
    D_80120656 = 1;
    D_80120657 = 0;
    D_80120676 = -0xA0;
    D_8012067A = -0x78;
    D_80120655 = 0x80;
    D_80120693 = 0;
    D_80120654 = 0;
    func_80048F64(0x61);
    D_80120695 = 4;
    D_801206CC = 0x01000000;
    D_80120697 = 0;
    D_801206A0 = (s32) D_8013C724;
    D_801206A4 = (s32) D_8013C728;
    D_801206C8 = (s32) D_8013C720;
    if (D_80122EAC == 3) {
        D_801206AA = 5;
    } else {
        D_801206AA = 4;
    }
    D_8012069A = 1;
    D_8012069B = 0x80;
    D_801206BA = -0xA0;
    D_801206BE = -0x78;
    D_80120699 = 0;
    D_801206D7 = 0x11;
    D_80120698 = 0;
    func_80048F64(0x62);
    D_801206D9 = 4;
    D_80120710 = 0x01000000;
    D_801206DB = 0x80;
    D_801206DA = 0x40;
    D_801206E4 = (s32) D_8013C724;
    D_801206E8 = (s32) D_8013C728;
    D_8012070C = (s32) D_8013C720;
    D_801206F0 = 0x80;
    D_801206DE = 1;
    D_801206DF = 0;
    D_801206FE = -0xA0;
    D_80120702 = -0x78;
    D_801206DD = 0x80;
    D_8012071B = 0;
    D_801206DC = 0;
    load_tpage_buf_lock(5, (*(s32 *)D_8013C720), 1);
    load_tpage_buf_lock(6, *(s32 *)((unsigned char *)D_8013C720 + (4)), 1);
    load_tpage_buf_lock(7, *(s32 *)((unsigned char *)D_8013C720 + (8)), 1);
    load_tpage_buf_lock(8, *(s32 *)((unsigned char *)D_8013C720 + (12)), 1);
    load_tpage_buf_lock(9, *(s32 *)((unsigned char *)D_8013C720 + (16)), 1);
    load_tpage_buf_lock(0xA, *(s32 *)((unsigned char *)D_8013C720 + (20)), 1);
    load_tpage_buf_lock(0x25, *(s32 *)((unsigned char *)D_8013C720 + (24)), 1);
    load_tpage_buf_lock(0x26, *(s32 *)((unsigned char *)D_8013C720 + (28)), 1);
    load_tpage_buf_lock(0x27, *(s32 *)((unsigned char *)D_8013C720 + (32)), 1);
    load_tpage_buf_lock(0x28, *(s32 *)((unsigned char *)D_8013C720 + (36)), 1);
}
