#include "common.h"
#include "ovl/TAIIKU.h"

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80141280", func_80141280);

void func_80141964(void) {
    s32 pad; /* FAKE: unused word above the arrays, the original frame has it (T-9020) */
    s16 x[3];
    s16 y[3];
    s16 w[3];
    s16 h[3];

    x[0] = x[1] = x[2] = -0x80;
    y[0] = 0x30;
    y[1] = 0x40;
    y[2] = 0x50;
    w[0] = w[1] = w[2] = 0xFC;
    h[0] = h[1] = h[2] = 0x10;
    func_8004F870(0, 3, x, y, w, h);
    func_8006BC28(0);
    func_8006BD6C(1);
}

s32 func_801419F8(void) {
    func_8004F984(0, D_8011ECF6, D_8011ECFA);
    func_8004FC10(0);
    func_80050B54(0);
    func_8006BD6C(1);
    func_8006BA40();
    if (D_800E6280.unk_F88 & 0x20) {
        if (func_8004ECB4() != 0) {
            if (D_800E6280.unk_F90[0].unk_06 != 0) {
                D_80122CDC = 0;
            } else if (D_800E6280.unk_F90[2].unk_06 != 0) {
                D_80122CDC = 1;
            } else if (D_800E6280.unk_F90[4].unk_06 != 0) {
                D_80122CDC = 2;
            } else {
                return 0;
            }
            D_8011ECD0[3] &= 0xFF7F;
            func_8004E9F4(1);
            func_8004284C();
        }
    }
}

void func_80141AE8(void) {
    D_80122EC8 = 9;
    D_800CA148 = 2;
    D_800CA14C = 0;
    k_reset(1);
}

void func_80141B28(void) {
    D_80122EC8 = 0xB;
    D_800CA148 = 1;
    D_800CA14C = 0;
    k_reset(1);
}
