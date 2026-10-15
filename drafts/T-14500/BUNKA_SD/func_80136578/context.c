#include "common.h"
#include "ovl/BUNKA_SD.h"

s32 func_80136270(void) {
    if (D_800E6280.unk_110D == 0) {
        cd_read_sectors(0x6F, 0x80180000, 0x4C59);
        D_800E6280.unk_110D += 1;
    } else if ((D_800E6280.unk_110D == 1) && (cd_read_sync_status() & 1)) {
        func_8004284C();
    }
}

void func_801362EC(void) {
    s32 i;

    if (D_800E6280.unk_110A >= 3 && D_800E6280.unk_110A < 7) {
        if (D_800E6280.unk_F80 & 0x20) {
            func_801363BC();
            for (i = 0; i < 3; i++) {
                D_8011ECD0[i * 0x44 + 0x1986] = 0x82;
            }
        } else {
            for (i = 0; i < 3; i++) {
                D_8011ECD0[i * 0x44 + 0x1986] = 1;
            }
        }
    }
    if (D_800E6280.unk_110A >= 3 && D_800E6280.unk_110A < 6 && (D_800E6280.unk_F80 & 0x40)) {
        func_80042940(5);
    }
    func_801363BC();
}

void func_801363BC(void) {
    func_80066C08(2);
    func_80064F48();
    func_80066334();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80134260();
        return;
    case 1:
        func_80136270();
        return;
    case 2:
        func_801364B4();
        return;
    case 3:
        func_80134418();
        return;
    case 4:
        func_801364F0();
        return;
    case 5:
        func_80044750(0xC1);
        func_8004E9F4(1);
        func_8004284C();
        return;
    case 6:
        func_801344A8();
        return;
    case 7:
        func_80134314();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_801364B4(void) {
    func_8004500C(1, 0x201);
    func_80136578();
    D_80120696 = 1;
    func_8004284C();
}

void func_801364F0(void) {
    func_80136904();
    func_801369AC();
    D_800E6280.unk_1104.u += 1;
    if (D_80122EAC == 3) {
        if (D_800E6280.unk_1104.u >= 0x25B) {
            func_8004284C();
        }
    } else if (!(D_80120652 & 1)) {
        func_8004284C();
    }
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_8013C74C;
extern u8 *D_8013C740;
extern u8 *D_8013C744;
extern u8 *D_8013C748;
/* end bulk_m2c declarations */
void func_80136578(void) {
    s32 var_s0;

    var_s0 = 0;
    do {
        load_palette(D_8013C74C, (var_s0 + 0x10) & 0xFF, 1, 1, var_s0);
        var_s0 += 1;
    } while (var_s0 != 0x10);
    func_80048F64(0x60);
    D_80120651 = 4;
    D_80120688 = 0x01000000;
    D_80120653 = 0x80;
    D_8012065C = D_8013C744;
    D_80120660 = D_8013C748;
    D_80120684 = D_8013C740;
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
    D_80120697 = 0x80;
    D_801206A0 = (s32) D_8013C744;
    D_801206A4 = (s32) D_8013C748;
    D_801206C8 = (s32) D_8013C740;
    D_801206AA = 4;
    D_8012069A = 1;
    D_8012069B = 0;
    D_801206BA = -0xA0;
    D_801206BE = -0x78;
    D_80120699 = 0x80;
    D_801206D7 = 0;
    D_80120698 = 0;
    func_80048F64(0x62);
    D_801206D9 = 4;
    D_80120710 = 0x01000000;
    D_801206DB = 0x80;
    D_801206DA = 0x40;
    D_801206E4 = (s32) D_8013C744;
    D_801206E8 = (s32) D_8013C748;
    D_8012070C = (s32) D_8013C740;
    D_801206F0 = 0x58;
    D_801206DE = 1;
    D_801206DF = 0;
    D_801206FE = -0xA0;
    D_80120702 = -0x78;
    D_801206DD = 0x80;
    D_8012071B = 0;
    D_801206DC = 0;
    load_tpage_buf_lock(5, (*(s32 *)D_8013C740), 1);
    load_tpage_buf_lock(6, *(s32 *)((unsigned char *)D_8013C740 + (4)), 1);
    load_tpage_buf_lock(7, *(s32 *)((unsigned char *)D_8013C740 + (8)), 1);
    load_tpage_buf_lock(8, *(s32 *)((unsigned char *)D_8013C740 + (12)), 1);
    load_tpage_buf_lock(9, *(s32 *)((unsigned char *)D_8013C740 + (16)), 1);
    load_tpage_buf_lock(0xA, *(s32 *)((unsigned char *)D_8013C740 + (20)), 1);
    load_tpage_buf_lock(0x25, *(s32 *)((unsigned char *)D_8013C740 + (24)), 1);
    load_tpage_buf_lock(0x26, *(s32 *)((unsigned char *)D_8013C740 + (28)), 1);
    load_tpage_buf_lock(0x27, *(s32 *)((unsigned char *)D_8013C740 + (32)), 1);
    load_tpage_buf_lock(0x28, *(s32 *)((unsigned char *)D_8013C740 + (36)), 1);
    load_tpage_buf_lock(0x29, *(s32 *)((unsigned char *)D_8013C740 + (40)), 1);
    load_tpage_buf_lock(0x2A, *(s32 *)((unsigned char *)D_8013C740 + (44)), 1);
}

void func_80136904(void) {
    if (D_80122EAC == 3) {
        switch (D_800E6280.unk_1104.u) {
        case 0x14C:
            func_80044750(0x501);
            return;
        case 0x1BF:
            func_80044750(0x502);
            return;
        }
    } else {
        switch (D_800E6280.unk_1104.u) {
        case 0x157:
            func_80044750(0x501);
            return;
        case 0x1CA:
            func_80044750(0x502);
            return;
        }
    }
}

void func_801369AC(void) {
    if (D_800E6280.unk_1104.w == 0) {
        D_80120652 = 5;
    }
}
