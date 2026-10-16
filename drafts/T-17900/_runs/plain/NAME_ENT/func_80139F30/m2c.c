extern u8 D_800E7397;
extern s32 D_800E7D18;
extern u8 D_800E7D24;
extern u8 D_800E7D25;
extern u8 D_800E7D26;
extern u8 D_800E7D27;
extern u8 D_800E7D28;
extern u8 D_800E7D29;
extern u8 D_800E7D2A;
extern u8 D_800E7D2B;
extern u8 D_800E7D2D;
extern u8 D_800E7D2E;
extern u8 D_800E7D2F;
extern u8 D_800E7D30;
extern s8 D_801210F1;
extern u8 D_801210F3;
extern s8 D_80121135;
extern u8 D_80121137;
extern s8 D_80121179;
extern u8 D_8012117B;
extern s8 D_801211BD;
extern u8 D_801211BF;
extern s8 D_80121201;
extern u8 D_80121203;
extern ? D_801237A8;
extern ? D_801237B4;

void func_80139F30(void) {
    s32 *sp34;                                      /* compiler-managed */
    s32 *var_v1;
    s32 *var_v1_3;
    s32 *var_v1_4;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_s0_5;
    s32 var_s0_6;
    s32 var_t3;
    s32 var_t7;
    s32 var_t8;
    u8 *var_v0;
    u8 *var_v0_3;
    u8 *var_v1_2;
    u8 temp_a0;
    u8 temp_a0_10;
    u8 temp_a0_11;
    u8 temp_a0_12;
    u8 temp_a0_13;
    u8 temp_a0_2;
    u8 temp_a0_3;
    u8 temp_a0_4;
    u8 temp_a0_5;
    u8 temp_a0_6;
    u8 temp_a0_7;
    u8 temp_a0_8;
    u8 temp_a0_9;
    u8 temp_t1;
    u8 var_t0;
    void *temp_a2;
    void *temp_v0;
    void *var_v0_2;
    void *var_v1_5;

    if (D_800E8BEE != 0) {
        if (D_800E7D24 != 0) {
            func_80139740((s32) D_800E7D24, -4, 0x1E);
        }
        if (D_800E7D25 != 0) {
            func_80139740((s32) D_800E7D25, -4, 0x2E);
        }
        if (D_800E7D26 != 0) {
            func_80139740((s32) D_800E7D26, -4, 0x3E);
        }
        if (D_800E7D27 != 0) {
            func_80139740((s32) D_800E7D27, -4, 0x4E);
        }
        if (D_800E7D28 != 0) {
            func_80139740((s32) D_800E7D28, 0x3C, 0x1E);
        }
        if (D_800E7D29 != 0) {
            func_80139740((s32) D_800E7D29, 0x3C, 0x2E);
        }
        if (D_800E7D2A != 0) {
            func_80139740((s32) D_800E7D2A, 0x3C, 0x3E);
        }
        if (D_800E7D2B != 0) {
            func_80139740((s32) D_800E7D2B, 0x3C, 0x4E);
        }
        if (D_800E7D30 != 0) {
            func_80139740((s32) D_800E7D30, 0x88, 0xE);
        }
        if (D_800E7D2C != 0) {
            func_80139740((s32) D_800E7D2C, 0x88, 0x1E);
        }
        if (D_800E7D2D != 0) {
            func_80139740((s32) D_800E7D2D, 0x88, 0x2E);
        }
        if (D_800E7D2E != 0) {
            func_80139740((s32) D_800E7D2E, 0x88, 0x3E);
        }
        if (D_800E7D2F != 0) {
            func_80139740((s32) D_800E7D2F, 0x88, 0x4E);
        }
        var_v1 = &D_800E7D10;
        var_s0 = 0;
        do {
            if (*(u8 *)((unsigned char *)var_v1 + (20)) != 0) {
                sp34 = var_v1;
                func_80139AD8(var_s0, -0x23, ((var_s0 % 4) * 0x10) + 0x1D);
            }
            var_s0 += 1;
            var_v1 += 1;
        } while (var_s0 < 4);
        var_v1_2 = D_800E7D14;
        var_s0_2 = 4;
        do {
            if (*(u8 *)((unsigned char *)var_v1_2 + (20)) != 0) {
                sp34 = var_v1_2;
                func_80139AD8(var_s0_2, 0x1E, ((var_s0_2 % 4) * 0x10) + 0x1D);
            }
            var_s0_2 += 1;
            var_v1_2 += 1;
        } while (var_s0_2 < 8);
        var_v1_3 = &D_800E7D18;
        var_s0_3 = 8;
        do {
            if (*(u8 *)((unsigned char *)var_v1_3 + (20)) != 0) {
                sp34 = var_v1_3;
                func_80139AD8(var_s0_3, 0x69, ((var_s0_3 % 4) * 0x10) + 0x1D);
            }
            var_s0_3 += 1;
            var_v1_3 += 1;
        } while (var_s0_3 < 0xC);
        if (D_800E7D30 != 0) {
            func_80139AD8(0xC, 0x69, 0xD);
        }
        var_v1_4 = &D_800E7D10;
        var_v0 = D_8011ECD0;
        do {
            temp_t1 = *(u8 *)((unsigned char *)var_v1_4 + (20));
            var_v1_4 += 1;
            if (temp_t1 == 0) {
                *(s8 *)((unsigned char *)var_v0 + (8705)) = 4;
                *(u8 *)((unsigned char *)var_v0 + (8707)) = (u8) (*(u8 *)((unsigned char *)var_v0 + (8707)) | 0x80);
            } else {
                *(u8 *)((unsigned char *)var_v0 + (8707)) = (u8) (*(u8 *)((unsigned char *)var_v0 + (8707)) & 0x7F);
            }
            var_v0 += 0x44;
        } while ((u32) var_v1_4 < (u32) &D_800E7D18);
        if (D_800E7D2C == 0) {
            D_80121137 |= 0x80;
            D_80121135 = 4;
        } else {
            D_80121137 &= 0x7F;
        }
        if (D_800E7D2D == 0) {
            D_8012117B |= 0x80;
            D_80121179 = 4;
        } else {
            D_8012117B &= 0x7F;
        }
        if (D_800E7D2E == 0) {
            D_801211BF |= 0x80;
            D_801211BD = 4;
        } else {
            D_801211BF &= 0x7F;
        }
        if (D_800E7D2F == 0) {
            D_80121203 |= 0x80;
            D_80121201 = 4;
        } else {
            D_80121203 &= 0x7F;
        }
        if (D_800E7D30 == 0) {
            D_801210F3 |= 0x80;
            D_801210F1 = 4;
            return;
        }
        D_801210F3 &= 0x7F;
        return;
    }
    var_v0_2 = (D_800E7397 * 0xF00) + D_80123120;
    temp_a0 = *(u8 *)((unsigned char *)var_v0_2 + (1672));
    if (temp_a0 != 0) {
        func_80139740((s32) temp_a0, -4, 0x1E);
        var_v0_2 = (D_800E7397 * 0xF00) + D_80123120;
    }
    temp_a0_2 = *(u8 *)((unsigned char *)var_v0_2 + (1673));
    if (temp_a0_2 != 0) {
        func_80139740((s32) temp_a0_2, -4, 0x2E);
        var_v0_2 = (D_800E7397 * 0xF00) + D_80123120;
    }
    temp_a0_3 = *(u8 *)((unsigned char *)var_v0_2 + (1674));
    if (temp_a0_3 != 0) {
        func_80139740((s32) temp_a0_3, -4, 0x3E);
        var_v0_2 = (D_800E7397 * 0xF00) + D_80123120;
    }
    temp_a0_4 = *(u8 *)((unsigned char *)var_v0_2 + (1675));
    if (temp_a0_4 != 0) {
        func_80139740((s32) temp_a0_4, -4, 0x4E);
        var_v0_2 = (D_800E7397 * 0xF00) + D_80123120;
    }
    temp_a0_5 = *(u8 *)((unsigned char *)var_v0_2 + (1676));
    if (temp_a0_5 != 0) {
        func_80139740((s32) temp_a0_5, 0x3C, 0x1E);
        var_v0_2 = (D_800E7397 * 0xF00) + D_80123120;
    }
    temp_a0_6 = *(u8 *)((unsigned char *)var_v0_2 + (1677));
    if (temp_a0_6 != 0) {
        func_80139740((s32) temp_a0_6, 0x3C, 0x2E);
        var_v0_2 = (D_800E7397 * 0xF00) + D_80123120;
    }
    temp_a0_7 = *(u8 *)((unsigned char *)var_v0_2 + (1678));
    if (temp_a0_7 != 0) {
        func_80139740((s32) temp_a0_7, 0x3C, 0x3E);
        var_v0_2 = (D_800E7397 * 0xF00) + D_80123120;
    }
    temp_a0_8 = *(u8 *)((unsigned char *)var_v0_2 + (1679));
    if (temp_a0_8 != 0) {
        func_80139740((s32) temp_a0_8, 0x3C, 0x4E);
        var_v0_2 = (D_800E7397 * 0xF00) + D_80123120;
    }
    temp_a0_9 = *(u8 *)((unsigned char *)var_v0_2 + (1684));
    if (temp_a0_9 != 0) {
        func_80139740((s32) temp_a0_9, 0x88, 0xE);
        var_v0_2 = (D_800E7397 * 0xF00) + D_80123120;
    }
    temp_a0_10 = *(u8 *)((unsigned char *)var_v0_2 + (1680));
    if (temp_a0_10 != 0) {
        func_80139740((s32) temp_a0_10, 0x88, 0x1E);
        var_v0_2 = (D_800E7397 * 0xF00) + D_80123120;
    }
    temp_a0_11 = *(u8 *)((unsigned char *)var_v0_2 + (1681));
    if (temp_a0_11 != 0) {
        func_80139740((s32) temp_a0_11, 0x88, 0x2E);
        var_v0_2 = (D_800E7397 * 0xF00) + D_80123120;
    }
    temp_a0_12 = *(u8 *)((unsigned char *)var_v0_2 + (1682));
    if (temp_a0_12 != 0) {
        func_80139740((s32) temp_a0_12, 0x88, 0x3E);
        var_v0_2 = (D_800E7397 * 0xF00) + D_80123120;
    }
    temp_a0_13 = *(u8 *)((unsigned char *)var_v0_2 + (1683));
    var_s0_4 = 0;
    if (temp_a0_13 != 0) {
        func_80139740((s32) temp_a0_13, 0x88, 0x4E);
        var_s0_4 = 0;
    }
    var_t8 = D_800E7397 * 0x10;
    do {
        if (*(&D_801237A8 + (((var_t8 - D_800E7397) << 8) + var_s0_4)) != 0) {
            func_80139AD8(var_s0_4, -0x23, ((var_s0_4 % 4) * 0x10) + 0x1D);
        }
        var_s0_4 += 1;
        var_t8 = D_800E7397 * 0x10;
    } while (var_s0_4 < 4);
    var_s0_5 = 4;
    var_t3 = D_800E7397 * 0x10;
    do {
        if (*(&D_801237A8 + (((var_t3 - D_800E7397) << 8) + var_s0_5)) != 0) {
            func_80139AD8(var_s0_5, 0x1E, ((var_s0_5 % 4) * 0x10) + 0x1D);
        }
        var_s0_5 += 1;
        var_t3 = D_800E7397 * 0x10;
    } while (var_s0_5 < 8);
    var_s0_6 = 8;
    var_t7 = D_800E7397 * 0x10;
    do {
        if (*(&D_801237A8 + (((var_t7 - D_800E7397) << 8) + var_s0_6)) != 0) {
            func_80139AD8(var_s0_6, 0x69, ((var_s0_6 % 4) * 0x10) + 0x1D);
        }
        var_s0_6 += 1;
        var_t7 = D_800E7397 * 0x10;
    } while (var_s0_6 < 0xC);
    var_t0 = *(&D_801237B4 + (D_800E7397 * 0xF00));
    if (var_t0 != 0) {
        func_80139AD8(0xC, 0x69, 0xD);
        var_t0 = *(&D_801237B4 + (D_800E7397 * 0xF00));
    }
    temp_a2 = (D_800E7397 * 0xF00) + D_80123120;
    var_v0_3 = D_8011ECD0;
    var_v1_5 = temp_a2;
    do {
        if (*(u8 *)((unsigned char *)var_v1_5 + (1672)) == 0) {
            *(s8 *)((unsigned char *)var_v0_3 + (8705)) = 4;
            *(u8 *)((unsigned char *)var_v0_3 + (8707)) = (u8) (*(u8 *)((unsigned char *)var_v0_3 + (8707)) | 0x80);
        } else {
            *(u8 *)((unsigned char *)var_v0_3 + (8707)) = (u8) (*(u8 *)((unsigned char *)var_v0_3 + (8707)) & 0x7F);
        }
        var_v0_3 += 0x44;
        var_v1_5 += 1;
    } while ((u32) var_v0_3 < (u32) D_8011EEF0);
    temp_v0 = temp_a2 + 8;
    if (*(u8 *)((unsigned char *)temp_v0 + (1672)) == 0) {
        D_80121137 |= 0x80;
        D_80121135 = 4;
    } else {
        D_80121137 &= 0x7F;
    }
    if (*(u8 *)((unsigned char *)temp_v0 + (1673)) == 0) {
        D_8012117B |= 0x80;
        D_80121179 = 4;
    } else {
        D_8012117B &= 0x7F;
    }
    if (*(u8 *)((unsigned char *)temp_v0 + (1674)) == 0) {
        D_801211BF |= 0x80;
        D_801211BD = 4;
    } else {
        D_801211BF &= 0x7F;
    }
    if (*(u8 *)((unsigned char *)temp_v0 + (1675)) == 0) {
        D_80121203 |= 0x80;
        D_80121201 = 4;
    } else {
        D_80121203 &= 0x7F;
    }
    if (var_t0 == 0) {
        D_801210F3 |= 0x80;
        D_801210F1 = 4;
        return;
    }
    D_801210F3 &= 0x7F;
}
