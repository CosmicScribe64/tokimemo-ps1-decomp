extern s16 D_800E639A;
extern s32 D_8015EBD8;
void func_801447E0(void) {
    s32 sp38;
    s32 sp2C;
    s16 var_a2;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_t6;
    s32 temp_t7;
    s32 var_v1;
    s32 var_v1_2;

    if (D_800E639A >= 0x97) {
        var_a2 = 0x96;
    } else {
        var_a2 = D_800E639A;
    }
    if (D_8015EB98 & 0x80000000) {
        temp_t7 = D_8015EDC4 & 0xFFFF;
        if ((temp_t7 == 1) || (temp_t7 != 2)) {
            sp38 = (s32) var_a2;
            var_v1 = func_8013EA00((s32) D_8015EDA0 / 10, (s32) (D_8015EDA0 * 5) / 100) + 0x64;
        } else {
            temp_lo = (s32) D_8015EDA0 / 10;
            sp38 = (s32) var_a2;
            var_v1 = func_8013EA00(temp_lo, temp_lo) + 0x96;
        }
        temp_lo_2 = (s32) (D_8015EBD8 * ((s32) (var_v1 * (0xE1 - var_a2)) / 150)) / 1000;
        D_8015EBB0 = temp_lo_2;
        if (D_8015ED98 & 0x100) {
            D_8015EBD8 += 0x64;
            D_8015EBB0 = temp_lo_2 / 2;
        }
    }
    temp_a1 = D_8015EB98;
    temp_a0 = D_8015ED98;
    if (temp_a1 & 0x100000) {
        temp_t6 = D_8015EDC4 & 0xFFFF;
        sp2C = 0xE1 - var_a2;
        if ((temp_t6 == 2) || (temp_t6 != 4)) {
            var_v1_2 = func_8013E9A8(((s32) (D_8015EDA0 * 0x19) / 100) - 0x1F4) + 0x1F4;
            if (D_8015ED98 & 0x100) {
                var_v1_2 = var_v1_2 / 3;
            }
        } else {
            var_v1_2 = func_8013E9A8(((s32) (D_8015EDA0 * 0x32) / 100) - 0x3E8) + 0x3E8;
            if (D_8015ED98 & 0x100) {
                var_v1_2 = var_v1_2 / 10;
            }
        }
        D_8015EBB0 = (s32) (D_8015EBD8 * ((s32) (var_v1_2 * sp2C) / 150)) / 1000;
    }
    if (temp_a1 & 0xF0000) {
        if (temp_a0 & 0x100) {
            D_8015EBB0 = (s32) D_8015ED94 / 100;
        } else {
            D_8015EBB0 = (s32) D_8015ED94 / 10;
        }
    }
    if (temp_a1 & 0x08000000) {
        if (temp_a0 & 0x1000) {
            D_8015EBB0 = (s32) D_8015ED94 / 2;
            return;
        }
        if (temp_a0 & 0x100) {
            D_8015EBB0 = (s32) D_8015ED94 / 50;
        } else {
            D_8015EBB0 = (s32) D_8015ED94 / 5;
        }
        if (D_8015ED94 < 0x14) {
            D_8015EBB0 = 0;
        }
    }
}
