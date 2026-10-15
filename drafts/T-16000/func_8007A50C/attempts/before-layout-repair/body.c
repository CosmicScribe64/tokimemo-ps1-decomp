void func_8007A50C(void) {
    s16 temp_a1;
    u16 temp_t9;

    if ((D_80125D10.unk_00 & 0x4400) && (temp_t9 = (D_80125D10.unk_34 + D_80125D10.unk_38) & 0xFFFF, D_80125D10.unk_34 = temp_t9, ((temp_t9 & 0xFF00) != 0))) {
        temp_a1 = D_80125D10.unk_1C - D_80125D10.unk_36;
        D_80125D10.unk_34 = temp_t9 & 0xFF;
        if (temp_a1 <= 0) {
            D_80125D10.unk_1C = temp_a1;
            func_8007A5BC();
            return;
        }
        D_80125D10.unk_1C = temp_a1;
        func_8008BFB0(0, (s32) temp_a1, (s32) temp_a1);
    }
}
