u8 func_80153784(void) {
    s16 sp52;
    s16 sp50;
    s16 sp4E;
    s16 sp4C;
    s16 sp4A;
    s16 sp48;
    s16 sp46;
    s16 sp44;
    s16 sp42;
    s16 sp40;
    s16 sp3E;
    s16 sp3C;
    s16 sp3A;
    s16 sp38;
    s16 sp36;
    s16 sp34;
    s32 *var_v0_2;
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    u8 var_v0;

    switch (D_800E738A) {
    case 0:
        sp4E = 0x100;
        sp4C = 0x240;
        sp52 = 0x28;
        sp50 = 0x40;
        func_8009C93C(&sp4C, 0x180, 0x180);
        sp46 = 0x128;
        sp44 = 0x240;
        sp4A = 0x28;
        sp48 = 0x40;
        func_8009C93C(&sp44, 0x1C0, 0x180);
        sp3E = 0x100;
        sp3C = 0x220;
        sp42 = 0x28;
        sp40 = 0x40;
        func_8009C93C(&sp3C, 0x200, 0x180);
        sp36 = 0x100;
        sp34 = 0x21F;
        sp3A = 0x80;
        sp38 = 1;
        func_8009C93C(&sp34, 0x220, 0x100);
        D_8015EC40 = 0x20;
        D_8015EC44 = 0x40;
        D_8015EC48 = 1;
        D_8015ED2C = -0x80;
        D_8015ED30 = 0x20;
        D_8015ED34 = 0x40;
        D_8015ED38 = 1;
        D_8015EC38 = 0;
        D_8015EC3C = 0x80;
        D_8015ED28 = 0;
        return func_8004284C();
    case 1:
        temp_v0 = D_8015EC38 + D_8015EC48;
        D_8015EC38 = temp_v0;
        temp_a0 = D_8015ED28 + D_8015ED38;
        if (temp_v0 >= D_8015EC40) {
            D_8015EC38 = 0;
        }
        D_8015ED28 = temp_a0;
        if (temp_a0 >= D_8015ED30) {
            D_8015ED28 = 0;
        }
        func_80153B08(temp_a0);
        func_80153D20();
        var_v0 = func_80153A60();
        D_8015ED2C += 1;
        temp_v1 = D_8015EC3C - 1;
        D_8015EC3C = temp_v1;
        if (temp_v1 == 0) {
            return func_8004284C();
        }
        return var_v0;
    case 2:
        sp3E = 0x180;
        sp3C = 0x200;
        sp42 = 0x28;
        sp40 = 1;
        func_8009C93C(&sp3C, 0x220, 0x100);
        func_8013E7C0(0x1F, 0, 1, 1);
        return func_8004284C();
    case 3:
        D_8015EC74 = 0;
        var_v0_2 = &D_8015EC7C;
        D_8015EC78 = 0;
        do {
            var_v0_2 += 0x10;
            *(s32 *)((unsigned char *)var_v0_2 + (-16)) = 0;
            *(s32 *)((unsigned char *)var_v0_2 + (-12)) = 0;
            *(s32 *)((unsigned char *)var_v0_2 + (-8)) = 0;
            *(s32 *)((unsigned char *)var_v0_2 + (-4)) = 0;
        } while (var_v0_2 != &D_8015ED3C);
        var_v0 = func_80042808(&D_8015ED28);
        /* Duplicate return node #16. Try simplifying control flow for better match */
        return var_v0;
    default:
        return D_800E738A;
    }
}
