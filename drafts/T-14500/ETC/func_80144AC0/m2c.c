extern s8 D_800E7314;
extern u8 D_800E7398;

void func_80144AC0(void) {
    s16 spCC;
    s16 spAC;
    s16 sp8C;
    s16 sp6C;
    s16 *var_s1;
    s16 *var_s2;
    s16 *var_s3;
    s16 *var_s4;
    s32 var_s0;
    s32 var_s5;
    u8 *var_v0;
    u8 var_s0_2;

    func_8004E58C();
    func_80066104((s32) "データセーブ");
    var_s0 = 0;
    var_s1 = &spCC;
    var_s2 = &spAC;
    var_s3 = &sp8C;
    var_s4 = &sp6C;
    var_s5 = 0;
    do {
        *var_s3 = 0x20;
        *var_s4 = 0x10;
        *var_s1 = ((var_s0 / 5) << 5) - 0x7C;
        *var_s2 = ((var_s0 % 5) * 0x10) - 0x40;
        func_8004E788(*var_s1, *var_s2, 0xF, *(D_800B32CC + var_s5), 0);
        var_s0 += 1;
        var_s1 += 2;
        var_s2 += 2;
        var_s3 += 2;
        var_s4 += 2;
        var_s5 += 4;
    } while (var_s0 < 0xE);
    func_8004F870(1, 0xE, &spCC, &spAC, &sp8C, &sp6C);
    D_800E7314 = 0;
    func_8004EA98();
    var_v0 = D_800E7D11;
    var_s0_2 = 1;
loop_3:
    if (*(u8 *)((unsigned char *)var_v0 + (4)) != 1) {
        var_s0_2 += 1;
        var_v0 += 1;
        if (var_s0_2 != 0xF) {
            goto loop_3;
        }
    }
    if ((u8) D_800E8BEE != 0) {
        D_800E7314 = D_800E7D36 - 1;
        D_800E7398 = D_800E7D36;
        func_80042940(2);
        func_800696DC(0, 0);
        func_8005C7C0();
    } else if (var_s0_2 == 0xF) {
        func_8005C7C0();
        func_80042940(2);
    } else {
        D_800E7398 = var_s0_2;
        func_80042940(0xC0);
    }
    func_8005C7FC();
}
