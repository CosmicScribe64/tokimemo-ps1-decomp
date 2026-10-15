#include "common.h"
#include "ovl/BUNKAKEN.h"

typedef struct {
    s32 w[6];
} Tbl6_80144D38;
extern Tbl6_80144D38 D_80155F70;

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80143DD0", func_80143DD0);

void func_80144A70(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_800866A0(0x6C5F);
        break;
    case 1:
        addr_init_bustup();
        D_800E6280.unk_F5F = 0xD;
        func_80083474();
        cd_read_sectors(5U, 0x801E8000, 0x49BA);
        break;
    case 2:
        normal_date_girl_in();
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_800847B8(0xDU);
            func_801324E0();
            D_800CA148 = 6;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_80155E90;
            D_800CA164 = (s32) D_80155E94;
            D_800CA168 = (s32) D_80155E98;
            func_8004284C();
        }
        break;
    case 4:
        normal_date_speak();
        break;
    case 5:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            break;
        case 0x60:
            D_800CA148 += 1;
            break;
        case 0x61:
            D_800CA148 += 2;
            break;
        default:
            func_80046500();
            break;
        }
        func_8004284C();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_girl_out();
        break;
    case 8:
        D_800CA148 = 0xA;
        func_8004284C();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        func_80042908(3);
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80144C5C(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80144D38();
        func_8004284C();
        return;
    case 1:
        func_80144E9C();
        return;
    case 2:
        ((BunkaDispatch8 *)&D_80122EB8)->unk_00 = D_80122CDC;
        if (((BunkaDispatch8 *)&D_80122EB8)->unk_04 == 0xFF) {
            D_800CA148 = D_80122CDC + 0xC;
            func_8004284C();
            return;
        }
        func_80042808();
        return;
    case 3:
        normal_date_speak();
        return;
    case 4:
        func_80042808();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80144D38(void) {
    s32 i;
    s32 pad[2]; /* FAKE: unused local above the arrays reproduces the original frame 0xC0; real source unknown. T-9350 */
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];
    Tbl6_80144D38 tbl;

    tbl = D_80155F70;
    func_8004E9F4(1);
    for (i = 0; i < 6; i++) {
        a[i] = (i & 1) * 0x78 - 0x80;
        b[i] = (i >> 1) * 0x10 + 0x32;
        func_8004E788(a[i], b[i], 0, tbl.w[i], 0);
        c[i] = 0x78;
        d[i] = 0x10;
    }
    D_80122D30 = 1;
    func_8004E884(7);
    func_8004F870(0, 6, a, b, c, d);
    func_8006BC28(0);
}

void func_80144E9C(void) {
    s32 i;

    func_8004F984(0, D_8011ECF6, D_8011ECFA);
    func_8004FC10(0);
    func_80050B54(0);
    func_8006BD6C(0);
    func_8006BA40();
    i = 0;
    if (D_800E6280.unk_F88 & 0x20) {
        for (i = 0; i < 6; i++) {
            if (D_800E6280.unk_F90[i * 2].unk_06 != 0) {
                D_80122CDC = i;
                D_8011ECD3 &= ~0x80;
                func_8004E9F4(1);
                D_80122D30 = 0;
                func_8004284C();
            }
        }
    }
}

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154920);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154930);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154940);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154950);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154960);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154970);
