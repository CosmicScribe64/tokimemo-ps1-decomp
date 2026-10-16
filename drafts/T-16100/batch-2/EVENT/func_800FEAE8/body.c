void func_800FEAE8(void) {
    s32 spD4;
    typedef struct { s32 w[40]; } M2cBlockA0; /* m2c memcpy of 160 bytes */
    s32 spD0;
    s32 sp2C[41];
    s16 var_v0;
    s32 var_a0;
    s32 var_a2;
    s32 var_t6;
    u8 *temp_v1;

    spD0 = D_80121A2C;
    spD4 = (s32) *(s32 *)((unsigned char *)(&D_80121A2C) + (4));
    *(M2cBlockA0 *)(&sp2C) = *(M2cBlockA0 *)(&D_80121A34);
    *(s32 *)((unsigned char *)((u8 *)sp2C + 0x9C) + (4)) = (s32) *(s32 *)((unsigned char *)((u8 *)&D_80121A34 + 0x9C) + (4));
    if (D_800B1AF0 == 0) {
        func_80015C34(spD0, spD4, 0xF);
        func_800433D0(0x71);
        func_800433D0(0x300);
        D_800B1AF0 += 1;
    } else if (func_80015A0C(D_800B1AF0) == 9) {
        func_80011DFC();
        func_800433D0(0x202);
    } else {
        var_a2 = 0;
        if (D_800B176C & 0x400040) {
            func_80011DFC();
            func_800433D0(0xB4);
            func_800433D0(0x202);
        } else {
            var_a0 = 0;
            var_v0 = 0;
            var_t6 = 0 * 4;
            do {
                temp_v1 = var_t6 + (u8 *)sp2C;
                if (((u32) D_800B1AF0 >= (u32) (*(s32 *)temp_v1)) && ((u32) *(s16 *)((unsigned char *)temp_v1 + (2)) >= (u32) D_800B1AF0)) {
                    var_a2 = 1;
                    if (var_v0 == 6) {
                        D_800EAFB8 = 2;
                        goto block_31;
                    }
                    if (var_v0 == 0xC) {
                        D_800EAFB8 = 2;
                        goto block_31;
                    }
                    if (var_v0 == 0x16) {
                        D_800EAFB8 = 0;
                        goto block_31;
                    }
                    if (var_v0 == 0x17) {
                        D_800EAFB8 = 1;
                        goto block_31;
                    }
                    if (var_v0 == 0x18) {
                        D_800EAFB8 = 2;
                        goto block_31;
                    }
                    if (var_v0 == 0x19) {
                        D_800EAFB8 = 2;
                        goto block_31;
                    }
                    if (var_v0 == 0x1A) {
                        D_800EAFB8 = 0;
                        goto block_31;
                    }
                    if (var_v0 == 0x1D) {
                        D_800EAFB8 = 0;
                        goto block_31;
                    }
                    if (var_v0 == 0x21) {
                        D_800EAFB8 = 0;
                        goto block_31;
                    }
                    if (var_v0 == 0x24) {
                        D_800EAFB8 = 0;
                        goto block_31;
                    }
                    if (var_v0 == 0x28) {
                        D_800EAFB8 = 0;
block_31:
                        var_a0 = 1;
                    }
                }
                var_v0 += 1;
                var_t6 = var_v0 * 4;
            } while (var_v0 < 0x29);
            if (var_a2 == 0) {
                D_800EAFB8 = 3;
            } else if (var_a0 == 0) {
                func_800FEDC8();
            }
        }
    }
    if (func_80015A64() & 4) {
        D_800B1AF0 += 1;
    }
}
