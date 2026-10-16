extern s32 D_800E7200;
extern s32 D_800E7208;

s16 func_8013240C(void) {
    s32 sp30;
    s32 sp2C;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 temp_v0_3;
    s16 temp_v0_4;
    s32 var_a0;
    s32 var_t2;
    s32 var_t5;
    s32 var_t8;
    u32 temp_hi;
    u32 var_v0;
    u8 temp_t6;
    u8 temp_t7;

    var_a0 = 0x20;
    if (D_800E7200 & 0xA000) {
        var_t8 = D_800E7200 & 0x8000;
        if (D_800E7200 & 0x5000) {
            var_a0 = 0x2D;
            goto block_3;
        }
    } else {
block_3:
        var_t8 = D_800E7200 & 0x8000;
    }
    if (var_t8 != 0) {
        if (D_8015EDB4->unk74 >= 0x801) {
            temp_v0 = D_8015EDB4->unk74;
            D_8015EDB4->unk74 = temp_v0 + ((s32) (var_a0 * (0x1000 - temp_v0)) / 2048);
        } else {
            D_8015EDB4->unk74 += var_a0;
        }
    }
    var_t5 = D_800E7200 & 0x1000;
    if (D_800E7200 & 0x2000) {
        temp_v0_2 = D_8015EDB4->unk74;
        if (temp_v0_2 < -0x800) {
            D_8015EDB4->unk74 = temp_v0_2 + ((s32) (var_a0 * (-0x1000 - temp_v0_2)) / 2048);
        } else {
            D_8015EDB4->unk74 = temp_v0_2 - var_a0;
        }
        var_t5 = D_800E7200 & 0x1000;
    }
    var_t2 = D_800E7200 & 0x4000;
    if (var_t5 != 0) {
        temp_v0_3 = D_8015EDB4->unk76;
        if (temp_v0_3 < -0x700) {
            D_8015EDB4->unk76 = temp_v0_3 + ((s32) (var_a0 * (-0x800 - temp_v0_3)) / 256);
        } else {
            D_8015EDB4->unk76 = temp_v0_3 - var_a0;
        }
        var_t2 = D_800E7200 & 0x4000;
    }
    if (var_t2 != 0) {
        temp_v0_4 = D_8015EDB4->unk76;
        if (temp_v0_4 >= 0x81) {
            D_8015EDB4->unk76 = temp_v0_4 + ((s32) (var_a0 * (0x180 - temp_v0_4)) / 256);
        } else {
            D_8015EDB4->unk76 = temp_v0_4 + var_a0;
        }
    }
    if (D_8015EDB4->unk74 < -0x1000) {
        D_8015EDB4->unk74 = -0x1000;
    }
    if (D_8015EDB4->unk74 >= 0x1001) {
        D_8015EDB4->unk74 = 0x1000;
    }
    if (D_8015EDB4->unk76 < -0x800) {
        D_8015EDB4->unk76 = -0x800;
    }
    if (D_8015EDB4->unk76 >= 0x181) {
        D_8015EDB4->unk76 = 0x180;
    }
    temp_hi = (u8) D_8015EDDC % 3U;
    var_v0 = temp_hi;
    switch (temp_hi) {                              /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        sp30 = 5;
        sp2C = 0xA;
        break;
    case 1:                                         /* switch 1 */
        sp30 = 4;
        sp2C = 8;
        break;
    case 2:                                         /* switch 1 */
        sp30 = 1;
        sp2C = 2;
        break;
    }
    if (D_8015EDB4->unk3 == 0) {
        if (D_800E7208 & sp30) {
            *(u8 *)((unsigned char *)D_8015EDB4 + (1090)) = (u8) (*(u8 *)((unsigned char *)D_8015EDB4 + (1090)) + 1);
            D_8015EDB4->unk2 = 0x81;
            D_8015EDB4->unk3 = 0x7F;
        }
        if (D_800E7208 & sp2C) {
            *(u8 *)((unsigned char *)D_8015EDB4 + (1090)) = (u8) (*(u8 *)((unsigned char *)D_8015EDB4 + (1090)) + 1);
            D_8015EDB4->unk2 = 0x82;
            D_8015EDB4->unk3 = 0x7F;
        }
    }
    if (D_8015EDB4->unk2 & 1) {
        var_v0 = (u32) D_8015EDB4->unk3;
        D_8015EDB4->unk68 += var_v0;
        D_8015EDB4->unk74 += 8;
        if (var_v0 < 0xFU) {
            if ((D_8015EDB4->unk68 & 0xFFF) >= 0xFE1) {
                D_8015EDB4->unk3 = 0;
                D_8015EDB4->unk68 = 0;
                D_8015EDB4->unk2 = 0;
            }
        } else {
            temp_t6 = var_v0 - 1;
            D_8015EDB4->unk3 = temp_t6;
            if ((u32) (temp_t6 & 0xFF) < 0x40U) {
                D_8015EDB4->unk2 &= 0x7F;
            }
        }
    }
    if (D_8015EDB4->unk2 & 2) {
        var_v0 = (u32) D_8015EDB4->unk3;
        D_8015EDB4->unk68 -= var_v0;
        D_8015EDB4->unk74 -= 8;
        if (var_v0 < 0xFU) {
            if ((D_8015EDB4->unk68 & 0xFFF) < 0x1F) {
                D_8015EDB4->unk3 = 0;
                D_8015EDB4->unk68 = 0;
                D_8015EDB4->unk2 = 0;
            }
        } else {
            temp_t7 = var_v0 - 1;
            D_8015EDB4->unk3 = temp_t7;
            if ((u32) (temp_t7 & 0xFF) < 0x40U) {
                D_8015EDB4->unk2 &= 0x7F;
            }
        }
    }
    if (D_8015EDB4->unk2 & 4) {
        var_v0 = (u32) D_8015EDB4->unk7A;
        D_8015EDB4->unk7A = var_v0 - 1;
        switch (var_v0) {                           /* switch 2 */
        case 2:                                     /* switch 2 */
        case 10:                                    /* switch 2 */
        case 14:                                    /* switch 2 */
        case 15:                                    /* switch 2 */
            var_v0 = func_801430B0();
            break;
        case 0:                                     /* switch 2 */
            D_8015EDB4->unk2 &= 0xFB;
            break;
        }
    }
    return (s16) var_v0;
}
