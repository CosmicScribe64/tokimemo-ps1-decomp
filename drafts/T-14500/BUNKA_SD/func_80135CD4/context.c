#define MAIN_API_OVERRIDE_D_80120656 /* 0x82 must not sign-extend (main_api.h: s8) */
#include "common.h"
#include "ovl/BUNKA_SD.h"

extern u8 D_80120656;

s32 func_801359B0(void) {
    if (D_800E6280.unk_110D == 0) {
        cd_read_sectors(0x6D, 0x80180000, 0x4F73);
        D_800E6280.unk_110D += 1;
    } else if ((D_800E6280.unk_110D == 1) && (cd_read_sync_status() & 1)) {
        func_8004284C();
    }
}

void func_80135A2C(void) {
    s32 i;

    if ((D_800E6280.unk_110A >= 3) && (D_800E6280.unk_110A < 7)) {
        if (D_800E6280.unk_F80 & 0x20) {
            func_80135B44();
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
    func_80135B44();
}

void func_80135B44(void) {
    func_80066C08(2);
    func_80064F48();
    func_80066334();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80134260();
        return;
    case 1:
        func_801359B0();
        return;
    case 2:
        func_80135C3C();
        return;
    case 3:
        func_80134418();
        return;
    case 4:
        func_80135C70();
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

void func_80135C3C(void) {
    func_8004500C(1, 0x201);
    func_80135CD4();
    func_8004284C();
}

void func_80135C70(void) {
    u32 t;

    func_80135F38();
    func_801361B8();
    t = D_800E6280.unk_1104.u + 1;
    D_800E6280.unk_1104.u = t;
    if (t >= 0x21) {
        if (!(D_80120652 & 1)) {
            func_8004284C();
        }
    }
}

/* bulk_m2c declarations (T-11190) */
extern s16 D_801207BC;
extern s32 D_8013C734;
extern s32 D_8013C738;
extern s32 D_8013C73C;
extern void *D_8013C730;
/* end bulk_m2c declarations */
void func_80135CD4(void) {
    s16 temp_t0;
    s32 var_s1;
    s32 var_s1_2;
    u8 *var_s0;

    var_s1 = 0;
    do {
        load_palette(D_8013C73C, (var_s1 + 0x10) & 0xFF, 1, 1, var_s1);
        var_s1 += 1;
    } while (var_s1 < 0x10);
    var_s0 = D_8011ECD0;
    var_s1_2 = 0;
    do {
        func_80048F64(var_s1_2 + 0x60);
        *(s8 *)((unsigned char *)var_s0 + (6529)) = 4;
        *(s32 *)((unsigned char *)var_s0 + (6584)) = 0x01000000;
        *(s8 *)((unsigned char *)var_s0 + (6531)) = 0x80;
        *(s32 *)((unsigned char *)var_s0 + (6540)) = (s32) D_8013C734;
        *(s32 *)((unsigned char *)var_s0 + (6544)) = (s32) D_8013C738;
        temp_t0 = var_s1_2 + 3;
        var_s1_2 += 1;
        var_s0 += 0x44;
        *(s16 *)((unsigned char *)var_s0 + (6482)) = temp_t0;
        *(s8 *)((unsigned char *)var_s0 + (6466)) = 1;
        *(s8 *)((unsigned char *)var_s0 + (6467)) = 0;
        *(s16 *)((unsigned char *)var_s0 + (6498)) = -0xA0;
        *(s16 *)((unsigned char *)var_s0 + (6502)) = -0x78;
        *(s8 *)((unsigned char *)var_s0 + (6465)) = 0x80;
        *(s8 *)((unsigned char *)var_s0 + (6527)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (6464)) = 0;
        *(void **)((unsigned char *)var_s0 + (6512)) = (void *) D_8013C730;
    } while (var_s1_2 != 6);
    D_80120666 = (s16) D_80122EAC;
    D_8012071F = 0;
    D_80120763 = 0;
    D_801207A6 = 0x40;
    D_801207BC = 0x8B;
    load_tpage_buf_lock(5, (*(s32 *)D_8013C730), 1);
    load_tpage_buf_lock(6, *(s32 *)((unsigned char *)D_8013C730 + (4)), 1);
    load_tpage_buf_lock(7, *(s32 *)((unsigned char *)D_8013C730 + (8)), 1);
    load_tpage_buf_lock(8, *(s32 *)((unsigned char *)D_8013C730 + (12)), 1);
    load_tpage_buf_lock(9, *(s32 *)((unsigned char *)D_8013C730 + (16)), 1);
    load_tpage_buf_lock(0xA, *(s32 *)((unsigned char *)D_8013C730 + (20)), 1);
    load_tpage_buf_lock(0x25, *(s32 *)((unsigned char *)D_8013C730 + (24)), 1);
    load_tpage_buf_lock(0x26, *(s32 *)((unsigned char *)D_8013C730 + (28)), 1);
    load_tpage_buf_lock(0x27, *(s32 *)((unsigned char *)D_8013C730 + (32)), 1);
    load_tpage_buf_lock(0x28, *(s32 *)((unsigned char *)D_8013C730 + (36)), 1);
    load_tpage_buf_lock(0x29, *(s32 *)((unsigned char *)D_8013C730 + (40)), 1);
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801359B0", func_80135F38);

void func_801361B8(void) {
    if (D_800E6280.unk_1104.u == 0) {
        D_80120652 = 5;
        D_80120696 = 5;
        D_801206DA = 5;
    }
    if (D_800E6280.unk_1104.u == 0x3A6) {
        D_8012071F = 0x80;
        D_8012071E = 5;
        D_80120723 = 0x80;
        D_80120763 = 0x80;
        D_80120762 = 5;
        D_80120767 = 0x80;
    }
    if (!(D_8012071E & 1)) {
        D_8012071F = 0;
        D_80120763 = 0;
    }
}
