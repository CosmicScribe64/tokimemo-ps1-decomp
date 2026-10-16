void func_80139E34(void) {
    s16 sp54[11];
    s16 temp_t1;
    s16 var_s1;
    s32 var_s2;
    u8 *var_s0;

    *(s32 *)sp54 = (s32) D_80149EE8;
    *(s32 *)((unsigned char *)(&sp54[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80149EE8) + (4));
    var_s2 = 0;
    *(s32 *)((unsigned char *)(&sp54[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80149EE8) + (8));
    var_s0 = D_8011ECD0;
    *(s32 *)((unsigned char *)(&sp54[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80149EE8) + (12));
    var_s1 = 0;
    *(s32 *)((unsigned char *)(&sp54[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80149EE8) + (16));
    *(u16 *)((unsigned char *)(&sp54[0]) + (20)) = (u16) *(u16 *)((unsigned char *)(&D_80149EE8) + (20));
    do {
        func_80048F64(var_s1 + 0x60);
        *(s32 *)((unsigned char *)var_s0 + (6584)) = 0x01000000;
        *(s8 *)((unsigned char *)var_s0 + (6535)) = 0x80;
        *(s32 *)((unsigned char *)var_s0 + (6540)) = (s32) D_80149FE0;
        *(s32 **)((unsigned char *)var_s0 + (6580)) = &D_80149DA4;
        *(s32 *)((unsigned char *)var_s0 + (6544)) = (s32) D_8014A018;
        *(s8 *)((unsigned char *)var_s0 + (6533)) = 0x80;
        *(s16 *)((unsigned char *)var_s0 + (6554)) = 0x1000;
        *(s16 *)((unsigned char *)var_s0 + (6556)) = 0x1000;
        *(s8 *)((unsigned char *)var_s0 + (6534)) = 2;
        *(s8 *)((unsigned char *)var_s0 + (6530)) = 1;
        *(s16 *)((unsigned char *)var_s0 + (6548)) = (s16) D_8014A050;
        if (var_s1 == 0xB) {
            *(s16 *)((unsigned char *)var_s0 + (6566)) = 0;
            *(s16 *)((unsigned char *)var_s0 + (6570)) = 0;
            *(s8 *)((unsigned char *)var_s0 + (6529)) = 2;
            *(s8 *)((unsigned char *)var_s0 + (6531)) = 0xA4;
        } else {
            if (((u32) (*(&D_800E6448 + (var_s1 * 0x38)) << 0x1E) >> 0x1F) == 1) {
                temp_t1 = (&sp54[0])[var_s2];
                *(s8 *)((unsigned char *)var_s0 + (6531)) = 0xA4;
                var_s2 += 1;
                *(s16 *)((unsigned char *)var_s0 + (6566)) = temp_t1;
            } else {
                *(s16 *)((unsigned char *)var_s0 + (6566)) = 0xC8;
                *(s8 *)((unsigned char *)var_s0 + (6531)) = 0;
            }
            *(s16 *)((unsigned char *)var_s0 + (6570)) = 0x28;
            *(s8 *)((unsigned char *)var_s0 + (6529)) = 3;
        }
        *(s16 *)((unsigned char *)var_s0 + (6550)) = var_s1;
        var_s1 += 1;
        var_s0 += 0x44;
    } while (var_s1 != 0xC);
    D_8011ECF6 = 0;
    D_8011ECFA = 0;
}
