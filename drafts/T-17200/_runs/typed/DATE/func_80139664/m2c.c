u8 func_80139664(void) {
    s32 sp28;
    u8 temp_t8;
    u8 var_v0;

    if (D_80122CDC == 1) {
        D_800CA148 = 9;
        if (D_800E6280.unk_F5F == 7) {
            D_800CA148 = 0xA;
        } else if (D_800E6280.unk_F5F == 9) {
            D_800CA148 = 8;
        } else if ((D_800E6280.unk_F5F == 4) || (D_800E6280.unk_F5F == 8) || (D_800E6280.unk_F5F == 5) || (D_800E6280.unk_F5F == 0xA)) {
            D_800CA148 = 7;
        }
        D_800E6280.unk_110A = D_800E6280.unk0 + 0x12;
        return D_800E6280.unk_F5F;
    }
    if ((D_800E6280.unk_F5F == 4) || (D_800E6280.unk_F5F == 8) || (D_800E6280.unk_F5F == 5) || (D_800E6280.unk_F5F == 0xA)) {
        D_8015B67C = 1;
        D_800E6280.unk_110A += 6;
        return D_800E6280.unk_F5F;
    }
    if ((D_800E6280.unk_F5F == 9) && (D_800E6280.unk_56C[0x3E] == 0)) {
        D_8015B67C = 1;
        D_800CA148 += 1;
        D_800E6280.unk_110A += 6;
        return D_800E6280.unk_F5F;
    }
    if ((D_800E6280.unk_F5F == 9) && (D_800E6280.unk_56C[0x3E] != 0)) {
        if (D_800E6280.unk_110D == 0) {
            func_80083440(3);
            D_800E6280.unk_110D = 1;
        }
        sp28 = (s32) D_800E6280.unk_110A;
        func_80085A60();
        var_v0 = D_800E6280.unk_110A;
        if (sp28 != var_v0) {
            D_800CA148 = 8;
            temp_t8 = (var_v0 + 0x11) & 0xFF;
            D_800E6280.unk_110A = temp_t8;
            return temp_t8;
        }
        /* Duplicate return node #26. Try simplifying control flow for better match */
        return var_v0;
    }
    var_v0 = func_8004284C();
    return var_v0;
}
