#include "common.h"
#include "ovl/ETC.h"

/* NON_MATCHING: T-24012, background-read selection; table-address CSE and branch layout differ. */
void func_80132FB0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 **temp_v0;
    s32 temp_a0;

    D_8014E0C0 = 0;
    if (arg0 == 0x14) {
        switch (arg1) {
        case 1:
        case 2:
        case 3:
            if (D_80150054 == 0x61) {
                dec_bg_cd_read(0x40F4, 1);
            } else {
                dec_bg_cd_read(D_8014EA18[arg0][arg1], 1);
            }
            D_8014E0C0 = 1;
            break;
        case 4:
            if (D_80150054 == 0x61) {
                dec_bg_cd_read(0x467C, 1);
            } else {
                dec_bg_cd_read(D_8014EA18[arg0][arg1], 1);
            }
            D_8014E0C0 = 1;
            break;
        case 17:
        case 18:
        case 19:
        case 20:
            if (D_80150054 == 0x60) {
                dec_bg_cd_read(0x4118, 1);
            } else {
                dec_bg_cd_read(D_8014EA18[arg0][arg1], 1);
            }
            D_8014E0C0 = 1;
            break;
        default:
            dec_bg_cd_read(D_8014EA18[arg0][arg1], 1);
            D_8014E0C0 = 1;
            break;
        }
    } else {
        temp_v0 = &D_8014EA18[arg0];
        temp_a0 = (*temp_v0)[arg1];
        if (temp_a0 != -1) {
            if (temp_a0 == 0x42EB) {
                if ((arg3 == 2) || (arg3 == 3)) {
                    dec_bg_cd_read(0x42F4, 1);
                    D_8014E0C0 = 1;
                } else {
                    dec_bg_cd_read(temp_a0, 1);
                    D_8014E0C0 = 1;
                }
            } else if (temp_a0 == 0x42FD) {
                if ((arg3 == 2) || (arg3 == 3)) {
                    dec_bg_cd_read(0x4306, 1);
                    D_8014E0C0 = 1;
                } else {
                    dec_bg_cd_read(temp_a0, 1);
                    D_8014E0C0 = 1;
                }
            } else if (temp_a0 == 0x430F) {
                if ((arg3 == 2) || (arg3 == 3)) {
                    dec_bg_cd_read(0x4318, 1);
                    D_8014E0C0 = 1;
                } else {
                    dec_bg_cd_read(temp_a0, 1);
                    D_8014E0C0 = 1;
                }
            } else if (((temp_v0 != D_8014EA18) && (temp_v0 != &D_8014EA18[1]) && (temp_v0 != &D_8014EA18[11]) && (temp_v0 != &D_8014EA18[10]) && (temp_v0 != &D_8014EA18[13])) || (arg1 != 0)) {
                dec_bg_cd_read(temp_a0, 1);
                D_8014E0C0 = 1;
            }
        }
    }
}
