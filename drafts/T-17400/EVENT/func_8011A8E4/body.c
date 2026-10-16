void func_8011A8E4(void) {
    U16x21 sp90;
    U16x21 sp64;
    U16x21 sp38;
    s32 var_a1;
    s32 var_a2;
    s32 var_v0;

    sp90 = D_80125454;
    sp64 = D_80125480;
    sp38 = D_801254AC;
    var_a1 = 0xAF42;
    var_a2 = 0xAF23;
    if (D_801243B0 == 2) {
        var_a1 = 0xB384;
        var_a2 = 0xB381;
        goto block_27;
    }
    if (D_801243B0 == 0x11) {
        var_a1 = 0xB3DA;
        var_a2 = 0xB3CB;
        goto block_27;
    }
    if ((D_801243B0 == 1) || (D_801243B0 == 3)) {
        var_a1 = 0xB163;
        var_a2 = 0xB14D;
        goto block_27;
    }
    if ((D_801243B0 == 0xA) || (D_801243B0 == 9)) {
        var_a1 = 0xB172;
        var_a2 = 0xB165;
        goto block_27;
    }
    if (D_801243B0 == 0xC) {
        var_a1 = 0xB444;
        var_a2 = 0xB42F;
        goto block_27;
    }
    if ((D_801243B0 == 0x11) || (D_801243B0 == 0x10)) {
        var_a1 = 0xB3DA;
        var_a2 = 0xB3CB;
        goto block_27;
    }
    if ((D_801243B0 == 0x14) || (D_801243B0 == 0x13)) {
        var_a1 = 0xB41D;
        var_a2 = 0xB410;
        goto block_27;
    }
    if (D_801243B0 == 8) {
        var_a1 = 0xB054;
        var_a2 = 0xB04A;
        goto block_27;
    }
    var_v0 = D_801243B0 * 2;
    if (D_801243B0 == 7) {
        var_a1 = 0xB304;
        var_a2 = 0xB2EB;
block_27:
        var_v0 = D_801243B0 * 2;
    }
    func_800143DC(1, var_a1, var_a2, *(sp64.v + var_v0), (s32) *(sp38.v + var_v0), (s32) *(sp90.v + var_v0));
    func_80011DFC();
}
