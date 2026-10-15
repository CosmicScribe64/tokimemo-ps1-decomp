void func_8013F054(void) {
    s32 var_at;
    u8 temp_v1;
    u8 var_a3;
    u8 var_t0;
    u8 var_t1;
    u8 *temp_v0;
    u8 *temp_v0_2;
    u8 *var_v0;

    var_t0 = 0;
    var_a3 = 0;
    var_t1 = 0;
    if (D_800E73A4 == 0) {
        if ((D_801500D8 == 1) || (D_801500D8 == 3)) {
            temp_v0 = (D_80150064 * 8) + (u8 *)&D_800E6280;
            var_t1 = *(u8 *)((unsigned char *)temp_v0 + (1878));
            var_a3 = *(u8 *)((unsigned char *)temp_v0 + (1880));
            var_t0 = *(u8 *)((unsigned char *)temp_v0 + (1879));
        } else if (((D_801500D8 == 0) || (D_801500D8 == 2)) && (D_80150064 >= 0)) {
            var_v0 = (D_80150064 * 8) + (u8 *)&D_800E6280;
loop_8:
            temp_v1 = *(u8 *)((unsigned char *)var_v0 + (1878));
            var_v0 -= 8;
            if ((temp_v1 != 0xC) || (D_800E71DF != 0xC)) {
                if (temp_v1 == 0xB) {
                    var_at = (u32) var_v0 < (u32) &D_800E6280;
                    if (D_800E71DF == 0xB) {
                        goto block_12;
                    }
                    goto block_14;
                }
                var_at = (u32) var_v0 < (u32) &D_800E6280;
block_14:
                if (var_at != 0) {

                } else {
                    goto loop_8;
                }
            } else {
block_12:
                var_a3 = *(u8 *)((unsigned char *)var_v0 + (1888));
                var_t0 = *(u8 *)((unsigned char *)var_v0 + (1887));
                var_t1 = temp_v1;
            }
        }
    } else {
        temp_v0_2 = (D_80150080 * 8) + &D_800E7D10;
        var_t1 = *(u8 *)((unsigned char *)temp_v0_2 + (88));
        var_a3 = *(u8 *)((unsigned char *)temp_v0_2 + (90));
        var_t0 = *(u8 *)((unsigned char *)temp_v0_2 + (89));
    }
    if ((D_800E71DF == 0xC) && (var_t1 == 0xC)) {
        if (var_a3 == 4) {
            func_8013F320(1, 0xB);
        } else if (var_a3 == 6) {
            func_8013F320(2, 0xB);
        } else if (var_a3 == 7) {
            func_8013F320(3, 0xB);
        } else if ((var_a3 == 2) && (var_t0 == 0x4F)) {
            func_8013F320(5, 0xB);
        } else if ((var_a3 == 2) && (var_t0 == 0x50)) {
            func_8013F320(4, 0xB);
        } else if (var_a3 == 2) {
            if (var_t0 == 0x51) {
                func_8013F320(6, 0xB);
            }
        }
    } else if ((D_800E71DF == 0xB) && (var_t1 == 0xB)) {
        if ((var_a3 == 2) && (var_t0 == 0x4A)) {
            func_8013F320(0, 0xB);
        } else if (((var_a3 == 2) && (var_t0 == 0x4C)) || ((var_a3 == 0xB) && (var_t0 != 0x1E))) {
            func_8013F320(1, 0xB);
        } else if ((var_a3 != 2) || (var_t0 != 0x4E)) {
            if (var_a3 == 0xB) {
                if (var_t0 == 0x1E) {
                    goto block_50;
                }
                goto block_52;
            }
block_52:
            if (var_a3 == 0x12) {
                func_8013F320(3, 0xB);
            }
        } else {
block_50:
            func_8013F320(2, 0xB);
        }
    }
}
