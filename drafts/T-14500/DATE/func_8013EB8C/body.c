void func_8013EB8C(void) {
    typedef struct { s32 w[21]; } M2cBlock54; /* m2c memcpy of 84 bytes */
    typedef struct { s32 w[10]; } M2cBlock28; /* m2c memcpy of 40 bytes */
    s32 sp110[21];
    s32 spBC[21];
    s32 sp68[21];
    s32 sp64;
    s32 sp60;
    s32 sp34[10];
    s32 var_a1;
    s32 var_a2;
    s32 var_v0;

    *(M2cBlock54 *)(&sp110) = *(M2cBlock54 *)(&D_8015B888);
    *(M2cBlock54 *)(&spBC) = *(M2cBlock54 *)(&D_8015B8DC);
    *(M2cBlock54 *)(&sp68) = *(M2cBlock54 *)(&D_8015B930);
    *(M2cBlock28 *)(&sp34) = *(M2cBlock28 *)(&D_8015B984);
    *(u16 *)((unsigned char *)(&sp34 + 0x24) + (4)) = (u16) *(u16 *)((unsigned char *)(&D_8015B984 + 0x24) + (4));
    sp60 = 0xBF79;
    sp64 = 0xBF98;
    func_80044750(0xCF);
    D_800B3D60 = 0;
    var_a1 = 0xBF98;
    var_a2 = 0xBF79;
    if (D_800CA2F8 == 2) {
        var_a1 = 0xC517;
        var_a2 = 0xC514;
        goto block_33;
    }
    if (D_800CA2F8 == 0x11) {
        var_a1 = 0xC57E;
        var_a2 = 0xC56F;
        goto block_33;
    }
    if ((D_800CA2F8 == 1) || (D_800CA2F8 == 3)) {
        var_a1 = 0xC1C3;
        var_a2 = 0xC1AD;
        goto block_33;
    }
    if ((D_800CA2F8 == 0xA) || (D_800CA2F8 == 9)) {
        var_a1 = 0xC1D2;
        var_a2 = 0xC1C5;
        goto block_33;
    }
    if (D_800CA2F8 == 0xC) {
        var_a1 = 0xC657;
        var_a2 = 0xC623;
        goto block_33;
    }
    if ((D_800CA2F8 == 0x11) || (D_800CA2F8 == 0x10)) {
        var_a1 = 0xC57E;
        var_a2 = 0xC56F;
        goto block_33;
    }
    if ((D_800CA2F8 == 0x14) || (D_800CA2F8 == 0x13)) {
        var_a1 = 0xC611;
        var_a2 = 0xC604;
        goto block_33;
    }
    if (D_800CA2F8 == 8) {
        var_a1 = 0xC0C0;
        var_a2 = 0xC0B6;
        goto block_33;
    }
    if (D_800CA2F8 == 7) {
        var_a1 = 0xC493;
        var_a2 = 0xC47A;
        goto block_33;
    }
    if (D_800CA2F8 == 0x12) {
        var_a1 = 0xC4C5;
        var_a2 = 0xC495;
        goto block_33;
    }
    var_v0 = D_800CA2F8 * 4;
    if (D_800CA2F8 == 0xE) {
        var_a1 = 0xC5C9;
        var_a2 = 0xC5B9;
block_33:
        var_v0 = D_800CA2F8 * 4;
    }
    func_80044890(1, var_a1, var_a2, *(s32 *)((unsigned char *)(sp + var_v0) + (188)), *(s32 *)((unsigned char *)(sp + var_v0) + (104)), *(s32 *)((unsigned char *)(sp + var_v0) + (272)));
    if (func_80044E8C() == 1) {
        func_80044750((s32) *(u16 *)((unsigned char *)(sp + (D_800CA2F8 * 2)) + (52)));
        func_8004284C();
    }
    func_8004284C();
}
