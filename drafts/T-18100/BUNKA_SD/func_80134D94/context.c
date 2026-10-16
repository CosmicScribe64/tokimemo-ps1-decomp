#define MAIN_API_OVERRIDE_D_80120656 /* 0x82 must not sign-extend (main_api.h: s8) */
#include "common.h"
#include "ovl/BUNKA_SD.h"

extern u8 D_80120656;

s32 func_80134540(void) {
    if (D_800E6280.unk_110D == 0) {
        cd_read_sectors(0x85, 0x80180000, 0x4BD4);
        D_800E6280.unk_110D += 1;
    } else if ((D_800E6280.unk_110D == 1) && (cd_read_sync_status() & 1)) {
        func_8004284C();
    }
}

/* FAKE: the D_8012069A store shares a source line with the loop head; as1 schedules by line and this gives the original's order of the loop setup (T-7020, T-9380) */
void func_801345BC(void) {
    s32 i;

    if ((D_800E6280.unk_110A >= 3) && (D_800E6280.unk_110A < 7)) {
        if (D_800E6280.unk_F80 & 0x20) {
            func_801346D4();
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
    if ((D_800E6280.unk_110A >= 3) && (D_800E6280.unk_110A < 6) && (D_800E6280.unk_F80 & 0x40)) {
        func_80042940(5);
    }
    func_801346D4();
}

void func_801346D4(void) {
    func_80066C08(2);
    func_80064F48();
    func_80066334();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80134260();
        return;
    case 1:
        func_80134540();
        return;
    case 2:
        func_801347CC();
        return;
    case 3:
        func_80134418();
        return;
    case 4:
        func_80134800();
        return;
    case 5:
        func_80044750(0xC1);
        func_8004E9F4(1);
        func_8004284C();
        return;
    case 6:
        func_80134850();
        return;
    case 7:
        func_80134314();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_801347CC(void) {
    func_8004500C(1, 0x201);
    func_80134938();
    func_8004284C();
}

void func_80134800(void) {
    u32 t;

    func_80134B88();
    func_80134D94();
    t = D_800E6280.unk_1104.u + 1;
    D_800E6280.unk_1104.u = t;
    if (t >= 0x818) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80134540", func_80134850);

void func_80134938(void) {
    s32 i;

    for (i = 0; i < 0x10; i++) {
        load_palette(D_8013C71C, (i + 0x10) & 0xFF, 1, 1, i);
    }
    func_80048F64(0x60);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 5;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_0C = D_8013C714;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_10 = D_8013C718;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_34 = (u8 *)D_8013C710;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0xA;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0xA0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x78;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_43 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 0;
    for (i = 1; i < 5; i++) {
        func_80048F64(i + 0x60);
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_01 = 4;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_38 = 0x01000000;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_03 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_0C = D_8013C714;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_10 = D_8013C718;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_34 = (u8 *)D_8013C710;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_16 = i - 1;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_06 = 1;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_07 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_2A = -0x78;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_05 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_43 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_04 = 0;
    }
    ((DisplayActor44 *)D_8011ECD0)[99].unk_16 = D_80122EAC * 2 + 2;
    ((DisplayActor44 *)D_8011ECD0)[99].unk_03 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[100].unk_16 = D_80122EAC * 2 + 3;
    search_load_tpage_buf_lock(D_8013C710[11], 1);
    search_load_tpage_buf_lock(D_8013C710[12], 1);
    search_load_tpage_buf_lock(D_8013C710[13], 1);
    search_load_tpage_buf_lock(D_8013C710[14], 1);
}

void func_80134B88(void) {
    /* u32 selector: sltiu compares and the selector stays in $v0 (T-8020) */
    switch ((u32)D_800E6280.unk_1104.w) {                     /* irregular */
    case 0x4F2:
        func_80044750(0x509);
        break;
    case 0x542:
        func_80044750(0x509);
        break;
    case 0x592:
        func_80044750(0x509);
        break;
    case 0x606:
        func_80044750(0x500);
        break;
    case 0xF0:
        func_80044750(0x50A);
        break;
    case 0x150:
        func_80044750(0x50A);
        break;
    case 0x1B0:
        func_80044750(0x50A);
        break;
    case 0x210:
        func_80044750(0x50A);
        break;
    case 0x270:
        func_80044750(0x50A);
        break;
    case 0x2D0:
        func_80044750(0x50A);
        break;
    case 0x330:
        func_80044750(0x50A);
        break;
    case 0x390:
        func_80044750(0x50A);
        break;
    case 0x3F0:
        func_80044750(0x50A);
        break;
    case 0x450:
        func_80044750(0x50A);
        break;
    case 0x610:
        func_80044750(0x502);
        break;
    case 0x4B0:
        if (D_80122EAC == 3) {
            func_80044750(0x50A);
        }
        break;
    case 0x520:
        if (D_80122EAC == 3) {
            func_80044750(0x50A);
        }
        break;
    }
}

/* bulk_m2c declarations (T-11190) */
extern u8 D_800E738C;
/* end bulk_m2c declarations */
void func_80134D94(void) {
    u8 v;
    u32 q;
    s32 color;
    s16 pic;

    if (D_800E6280.unk_1104.u == 0) {
        D_8012071E = 5;
    }
    if (D_800E6280.unk_1104.u == 0xFE) {
        D_80120697 = 0x80;
        D_80120696 = 5;
        D_8012069B = 0x80;
    }
    if (!(D_80120696 & 1)) {
        D_80120697 = 0;
    }
    if (D_800E6280.unk_1104.u == 0x29C) {
        D_801206DB = 0x80;
        D_801206DA = 5;
        D_801206DF = 0x80;
    }
    if (D_800E6280.unk_1104.u >= 0xFF && D_800E6280.unk_1104.u < 0x5A3) {
        v = D_800E6280.unk_110D;
        D_8012071E = 0x40;
        q = v / 14;
        if (q >= 2) {
            v = 0;
            q = 0;
        }
        switch (D_80122EAC) {
        case 0:
            D_80120734 = q + 0x5D;
            break;
        case 1:
            D_80120734 = q + 0x72;
            break;
        case 2:
            D_80120734 = q + 0x87;
            break;
        case 3:
        default:
            if (D_800E6280.unk_1104.u < 0x4C2) {
                D_80120734 = q + 0x9D;
            } else {
                D_80120734 = q + 0x9F;
            }
            break;
        }
        D_800E6280.unk_110D = v + 1;
    }
    if (D_800E6280.unk_1104.u == 0x4C2 && D_80122EAC != 3) {
        D_8012071F = 0;
        D_80120763 = 0x80;
        D_80120762 = 5;
        D_80120767 = 0x80;
    }
    if (D_800E6280.unk_1104.u == 0x58F && D_80122EAC == 3) {
        D_8012071F = 0;
        D_80120763 = 0x80;
        D_80120762 = 5;
        D_80120767 = 0x80;
    }
    if (D_800E6280.unk_1104.u == 0x606 || D_800E6280.unk_1104.u == 0x609 || D_800E6280.unk_1104.u == 0x60C) {
        func_80049A40(-0xA0, -0x78, 0x140, 0xA0, 2, 0xFFFFFF, 0x81);
        dtd_on_tpage(0, 0, 2, 0, 0);
    }
    if (D_800E6280.unk_1104.u == 0x606) {
        D_801206F0 = 0x48;
        D_801206E0 = 0;
    }
    if (D_800E6280.unk_1104.u >= 0x60F && D_800E6280.unk_1104.u < 0x68E) {
        color = (0x68E - D_800E6280.unk_1104.u) * 0x10201;
        func_80049A40(-0x130, -0x78, 0x140, 0xA0, 2, color, 0x86);
        func_80049A40(0x10, -0x78, 0x140, 0xA0, 2, color, 0x85);
        dtd_on_tpage(0, 0, 2, 1, 0);
    }
    if (D_800E6280.unk_1104.u == 0x67E) {
        D_80120657 = 0x80;
        D_801206DF = 0x80;
        D_80120767 = 0x80;
    }
    if (D_800E6280.unk_1104.u >= 0x723 && D_80122EAC == 3) {
        v = *(u8 *)&D_800E6280.unk_110C;
        D_80120762 = 0x40;
        q = v / 14;
        pic = q + 0xA2;
        if (q >= 2) {
            v = 0;
            pic = 0xA2;
        }
        D_80120778 = pic;
        *(u8 *)&D_800E6280.unk_110C = v + 1;
    }
}
