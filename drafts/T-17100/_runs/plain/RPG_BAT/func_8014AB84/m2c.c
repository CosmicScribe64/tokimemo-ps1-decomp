void func_8014AB84(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_v1;
    void *temp_v0;

    func_8013F0F4(0x500, 1, 0);
    *(s32 *)((unsigned char *)(&(&D_8015EBAC)[arg0]) + (200)) = 1;
    temp_v1 = arg0 * 3;
    temp_v0 = &D_8015EBAC + (arg0 * 8);
    temp_a0 = temp_v1 + 4;
    sp30 = temp_a0;
    sp34 = temp_v1;
    *(s32 *)((unsigned char *)temp_v0 + (220)) = arg1;
    *(s32 *)((unsigned char *)temp_v0 + (224)) = arg2;
    func_8013E97C(temp_a0, arg1, arg2);
    temp_a0_2 = sp34 + 5;
    sp2C = temp_a0_2;
    func_8013E97C(temp_a0_2, arg1, arg2);
    temp_a0_3 = sp34 + 6;
    sp28 = temp_a0_3;
    func_8013E97C(temp_a0_3, arg1, arg2);
    func_8013E810(temp_a0, 6, 5, 1);
    func_8013E810(temp_a0_2, 8, 1, 1);
    func_8013E810(temp_a0_3, 0xD, 5, 1);
}
