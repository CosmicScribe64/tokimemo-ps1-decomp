void func_80079C70(void) {
    s32 sp2C;
    s32 var_v0;
    s32 temp_a1;
    s32 temp_v1;

    var_v0 = 0;
    temp_v1 = D_80125D10.unk_04 & 0xFF000000;
loop_1:
    temp_a1 = (0x01000000 << var_v0) & 0x0F000000;
    if ((temp_v1 & temp_a1) && !(temp_v1 & 0xF0000000)) {
        temp_v1 = (0x10000000 << var_v0) & 0xF0000000;
        D_80125D10.unk_04 = (D_80125D10.unk_04 & ~temp_a1) | temp_v1;
        sp2C = temp_v1;
        func_80079D28(var_v0);
        D_80125D10.unk_04 &= ~sp2C;
        return;
    }
    var_v0 += 1;
    if (var_v0 == 4) {
        return;
    }
    goto loop_1;
}
