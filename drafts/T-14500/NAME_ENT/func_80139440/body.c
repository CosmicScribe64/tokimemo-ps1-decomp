void func_80139440(void) {
    s32 *var_v0;
    s32 *var_v0_2;
    s32 *var_v0_3;
    s32 *var_v0_4;
    u32 temp_t9;
    u32 var_v1;
    u8 temp_t5;
    u8 temp_t6;

    if (func_800AEEB0(D_8014CC84, "こなみまん") == 0) {
        var_v0 = &D_800E6284;
        D_800E637E = 0x23D;
        do {
            var_v0 += 0x10;
            *(s16 *)((unsigned char *)var_v0 + (242)) = 0x23D;
            *(s16 *)((unsigned char *)var_v0 + (246)) = 0x23D;
            *(s16 *)((unsigned char *)var_v0 + (250)) = 0x23D;
            *(s16 *)((unsigned char *)var_v0 + (238)) = 0x23D;
        } while (var_v0 != &D_800E62A4);
    }
    if ((func_800AEEB0(D_8014CC74, "みんな") == 0) && (func_800AEEB0(D_8014CC7C, "仲良し") == 0)) {
        D_800E643E = 0x20;
        D_800E6442 = 0x40;
        var_v0_2 = &D_800E62B8;
        do {
            temp_t9 = (u8) D_800E644A >> 4;
            var_v1 = temp_t9;
            if (((u8) *(u8 *)((unsigned char *)var_v0_2 + (458)) >> 4) != temp_t9) {
                *(s16 *)((unsigned char *)var_v0_2 + (446)) = 0x7F;
                temp_t6 = *(u8 *)((unsigned char *)var_v0_2 + (456)) | 2;
                *(u8 *)((unsigned char *)var_v0_2 + (456)) = temp_t6;
                *(u8 *)((unsigned char *)var_v0_2 + (456)) = (u8) (temp_t6 | 8);
                *(s16 *)((unsigned char *)var_v0_2 + (450)) = 0x7F;
                var_v1 = (u8) D_800E644A >> 4;
            }
            if (((u8) *(u8 *)((unsigned char *)var_v0_2 + (514)) >> 4) != var_v1) {
                *(s16 *)((unsigned char *)var_v0_2 + (502)) = 0x7F;
                temp_t5 = *(u8 *)((unsigned char *)var_v0_2 + (512)) | 2;
                *(u8 *)((unsigned char *)var_v0_2 + (512)) = temp_t5;
                *(u8 *)((unsigned char *)var_v0_2 + (512)) = (u8) (temp_t5 | 8);
                *(s16 *)((unsigned char *)var_v0_2 + (506)) = 0x7F;
            }
            var_v0_2 += 0x70;
        } while (var_v0_2 != &D_800E6478);
    }
    if ((func_800AEEB0(D_8014CC74, "上上") == 0) && (func_800AEEB0(D_8014CC7C, "下下") == 0) && (func_800AEEB0(D_8014CC84, "左右左右×") == 0)) {
        var_v0_3 = &D_800E6284;
        D_800E637E = 0;
        do {
            var_v0_3 += 0x10;
            *(s16 *)((unsigned char *)var_v0_3 + (242)) = 0;
            *(s16 *)((unsigned char *)var_v0_3 + (246)) = 0;
            *(s16 *)((unsigned char *)var_v0_3 + (250)) = 0;
            *(s16 *)((unsigned char *)var_v0_3 + (238)) = 0;
        } while (var_v0_3 != &D_800E62A4);
    }
    if ((func_800AEEB0(D_8014CC74, "↑↑") == 0) && (func_800AEEB0(D_8014CC7C, "↓↓") == 0) && (func_800AEEB0(D_8014CC84, "←→←→×") == 0)) {
        var_v0_4 = &D_800E6284;
        D_800E637E = 0x3E7;
        do {
            var_v0_4 += 0x10;
            *(s16 *)((unsigned char *)var_v0_4 + (242)) = 0x3E7;
            *(s16 *)((unsigned char *)var_v0_4 + (246)) = 0x3E7;
            *(s16 *)((unsigned char *)var_v0_4 + (250)) = 0x3E7;
            *(s16 *)((unsigned char *)var_v0_4 + (238)) = 0x3E7;
        } while (var_v0_4 != &D_800E62A4);
    }
}
