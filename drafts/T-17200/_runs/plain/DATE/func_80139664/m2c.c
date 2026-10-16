extern u8 D_800E682A;
extern u8 D_800E71DF;
extern u8 D_800E738A;
extern u8 D_800E738D;

u8 func_80139664(void) {
    s32 sp28;
    u8 temp_t8;
    u8 var_v0;

    if (D_80122CDC == 1) {
        D_800CA148 = 9;
        if (D_800E71DF == 7) {
            D_800CA148 = 0xA;
        } else if (D_800E71DF == 9) {
            D_800CA148 = 8;
        } else if ((D_800E71DF == 4) || (D_800E71DF == 8) || (D_800E71DF == 5) || (D_800E71DF == 0xA)) {
            D_800CA148 = 7;
        }
        D_800E738A += 0x12;
        return D_800E71DF;
    }
    if ((D_800E71DF == 4) || (D_800E71DF == 8) || (D_800E71DF == 5) || (D_800E71DF == 0xA)) {
        D_8015B67C = 1;
        D_800E738A += 6;
        return D_800E71DF;
    }
    if ((D_800E71DF == 9) && (D_800E682A == 0)) {
        D_8015B67C = 1;
        D_800CA148 += 1;
        D_800E738A += 6;
        return D_800E71DF;
    }
    if ((D_800E71DF == 9) && (D_800E682A != 0)) {
        if (D_800E738D == 0) {
            func_80083440(3);
            D_800E738D = 1;
        }
        sp28 = (s32) D_800E738A;
        func_80085A60();
        var_v0 = D_800E738A;
        if (sp28 != var_v0) {
            D_800CA148 = 8;
            temp_t8 = (var_v0 + 0x11) & 0xFF;
            D_800E738A = temp_t8;
            return temp_t8;
        }
        /* Duplicate return node #26. Try simplifying control flow for better match */
        return var_v0;
    }
    var_v0 = func_8004284C();
    return var_v0;
}
