void func_8007ABE0(void) {
    s16 temp_a1;
    s16 temp_t2;
    s16 var_a0;

    if ((D_80125D10.unk_04 & 2) && (func_80079E00((s32) D_80125D10.unk_3C) != 0)) {
        func_8007AF0C();
    }
    if ((D_80125D10.unk_00 & 0x100) && (D_80125D10.unk_04 & 1) && (temp_t2 = ((u16) D_80125D10.unk_2E + (u16) D_80125D10.unk_32) & 0xFFFF, D_80125D10.unk_2E = temp_t2, ((temp_t2 & 0xFF00) != 0))) {
        temp_a1 = D_80125D10.unk_20 - (u16) D_80125D10.unk_30;
        D_80125D10.unk_2E = temp_t2 & 0xFF & 0xFFFF;
        if (temp_a1 < 0) {
            D_80125D10.unk_20 = temp_a1;
            func_8007AD6C();
            return;
        }
        if ((D_80125D10.unk_00 & 0x100000) && !(D_80125D10.unk_00 & 0x400)) {
            if (D_80125D10.unk_22 == 0) {
                D_80125D10.unk_20 = temp_a1;
                var_a0 = 0;
            } else {
                D_80125D10.unk_20 = temp_a1;
                var_a0 = (s16) ((s32) (temp_a1 * 0x7F) / (s16) D_80125D10.unk_22);
            }
            func_8008B750((s32) var_a0, (s32) var_a0);
            return;
        }
        D_80125D10.unk_20 = temp_a1;
        func_8008C720((s32) D_80125D10.unk_28, (s32) temp_a1, (s32) temp_a1);
    }
}
