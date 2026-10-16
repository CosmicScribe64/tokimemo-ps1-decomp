void func_80079C70(void) {
    s32 sp2C;
    s16 var_v0;
    s32 temp_a1;
    s32 temp_t3;
    s32 temp_v1;
    s32 var_t6;

    var_v0 = 0;
    temp_v1 = D_80125D10.unk_04 & 0xFF000000;
    var_t6 = 0x01000000 << 0;
loop_1:
    temp_a1 = var_t6 & 0x0F000000;
    if ((temp_v1 & temp_a1) && !(temp_v1 & 0xF0000000)) {
        temp_t3 = (0x10000000 << var_v0) & 0xF0000000;
        D_80125D10.unk_04 = (D_80125D10.unk_04 & ~temp_a1) | temp_t3;
        sp2C = temp_t3;
        func_80079D28(var_v0);
        D_80125D10.unk_04 &= ~sp2C;
        return;
    }
    var_v0 += 1;
    var_t6 = 0x01000000 << var_v0;
    if (var_v0 == 4) {
        return;
    }
    goto loop_1;
}
