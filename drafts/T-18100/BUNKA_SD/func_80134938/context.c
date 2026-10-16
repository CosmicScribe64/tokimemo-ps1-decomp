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
    DisplayActor44 *var_s0;
    s16 temp_t9;
    s32 temp_t0;
    s32 var_s1;
    s32 var_s1_2;

    var_s1 = 0;
    do {
        load_palette(D_8013C71C, (var_s1 + 0x10) & 0xFF, 1, 1, var_s1);
        var_s1 += 1;
    } while (var_s1 < 0x10);
    func_80048F64(0x60);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 5;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0x80;
    *(u8 **)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6540)) = (u8 *) D_8013C714;
    *(u8 **)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6544)) = (u8 *) D_8013C718;
    *(s32 **)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6580)) = (s32 *) D_8013C710;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0xA;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0xA0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x78;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_43 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 0;
    var_s0 = &((DisplayActor44 *)D_8011ECD0)[1];
    var_s1_2 = 1;
    do {
        func_80048F64(var_s1_2 + 0x60);
        var_s0[96].unk_01 = 4;
        var_s0[96].unk_38 = 0x01000000;
        var_s0[96].unk_03 = 0;
        *(u8 **)((unsigned char *)var_s0 + (6540)) = (u8 *) D_8013C714;
        *(u8 **)((unsigned char *)var_s0 + (6544)) = (u8 *) D_8013C718;
        temp_t9 = var_s1_2 - 1;
        var_s1_2 += 1;
        var_s0 += 1;
        var_s0[95].unk_16 = temp_t9;
        var_s0[95].unk_06 = 1;
        var_s0[95].unk_07 = 0;
        var_s0[95].unk_26 = -0xA0;
        var_s0[95].unk_2A = -0x78;
        var_s0[95].unk_05 = 0x80;
        var_s0[95].unk_43 = 0;
        var_s0[95].unk_04 = 0;
        *(s32 **)((unsigned char *)var_s0 + (6512)) = (s32 *) D_8013C710;
    } while (var_s1_2 != 5);
    temp_t0 = D_80122EAC * 2;
    ((DisplayActor44 *)D_8011ECD0)[99].unk_16 = (s16) (temp_t0 + 2);
    ((DisplayActor44 *)D_8011ECD0)[99].unk_03 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[100].unk_16 = (s16) (temp_t0 + 3);
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

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80134540", func_80134D94);
