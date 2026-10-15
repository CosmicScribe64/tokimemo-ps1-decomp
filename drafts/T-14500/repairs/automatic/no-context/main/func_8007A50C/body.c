void func_8007A50C(void) {
    s16 temp_a1;
    u16 temp_t9;

    if ((D_80125D10 & 0x4400) && (temp_t9 = (D_80125D44 + D_80125D48) & 0xFFFF, D_80125D44 = temp_t9, ((temp_t9 & 0xFF00) != 0))) {
        temp_a1 = D_80125D2C - D_80125D46;
        D_80125D44 = temp_t9 & 0xFF;
        if (temp_a1 <= 0) {
            D_80125D2C = temp_a1;
            func_8007A5BC(temp_a1);
            return;
        }
        D_80125D2C = temp_a1;
        func_8008BFB0(0, temp_a1, temp_a1);
    }
}
