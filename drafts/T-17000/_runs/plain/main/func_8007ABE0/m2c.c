extern s16 D_80125D30;
extern s16 D_80125D32;
extern u16 D_80125D3E;
extern u16 D_80125D40;
extern u16 D_80125D42;

void func_8007ABE0(void) {
    s16 temp_a1;
    s16 var_a0;
    u16 temp_t2;

    if ((D_80125D14 & 2) && (func_80079E00((s32) (s16) D_80125D4C) != 0)) {
        func_8007AF0C();
    }
    if ((D_80125D10.unk_00 & 0x100) && (D_80125D14 & 1) && (temp_t2 = (D_80125D3E + D_80125D42) & 0xFFFF, D_80125D3E = temp_t2, ((temp_t2 & 0xFF00) != 0))) {
        temp_a1 = D_80125D30 - D_80125D40;
        D_80125D3E = temp_t2 & 0xFF & 0xFFFF;
        if (temp_a1 < 0) {
            D_80125D30 = temp_a1;
            func_8007AD6C();
            return;
        }
        if ((D_80125D10.unk_00 & 0x100000) && !(D_80125D10.unk_00 & 0x400)) {
            if (D_80125D32 == 0) {
                D_80125D30 = temp_a1;
                var_a0 = 0;
            } else {
                D_80125D30 = temp_a1;
                var_a0 = (s16) ((s32) (temp_a1 * 0x7F) / (s16) D_80125D32);
            }
            func_8008B750((s32) var_a0, (s32) var_a0);
            return;
        }
        D_80125D30 = temp_a1;
        func_8008C720((s32) D_80125D38, (s32) temp_a1, (s32) temp_a1);
    }
}
