extern s32 D_800B58F0;
extern s32 D_800B58F4;
extern u8 D_8018003C;
extern ? strCallback;

void func_80056BA8(s32 arg0, s32 arg1, s32 arg2) {
    s8 spFD;
    s16 spF0;
    ? spEC;
    ? sp90;
    u8 sp5C;
    s32 sp54;
    s32 sp48;
    s16 var_s0_2;
    s32 temp_s1;
    s32 var_a0;
    s32 var_s0;
    s32 var_s2;
    s32 var_v0;
    s32 var_v0_2;
    void *temp_s0;
    void *temp_s0_2;

    D_800B58F4 = arg1;
    DecDCTReset(0);
    func_800869C8(0);
    func_800869A4(0);
    func_8009C210(1);
    if (D_8011ECA0 == 0) {
        func_8009AC20();
        func_800422C8();
    }
    D_800B58F0 = 0;
    D_80125C58 = &D_8018003C;
    D_8018003C.unk0 = 0x140;
    *(s32 *)((unsigned char *)(&D_8018003C) + (4)) = 0xF0;
    var_s2 = 0;
    var_s0 = 0;
    if (arg2 == 0) {
        if (func_80088E34((s32) &sp5C, arg0) == 0) {
            var_v0 = 0 < 0x10000;
loop_5:
            var_s0 += 1;
            if (var_v0 == 0) {
                return;
            }
            var_v0 = var_s0 < 0x10000;
            if (func_80088E34((s32) &sp5C, arg0) != 0) {
                goto block_8;
            }
            goto loop_5;
        }
block_8:
        D_80125C58->unk0 = sp5C;
        *(u8 *)((unsigned char *)D_80125C58 + (1)) = sp5D;
        *(u8 *)((unsigned char *)D_80125C58 + (2)) = sp5E;
        goto block_10;
    }
    func_80087EE8(arg2, (s32) &D_8018003C);
block_10:
    strSetDefDecEnv((s32) (D_80125C58 + 4), 0, 0, 0, /* extra? */ 0xF0);
    strInit((s32) D_80125C58, (s32) &strCallback);
    strNextVlc((s32) (D_80125C58 + 4));
    func_8008BEE0(0, 0, 1);
    func_8008BFB0(0, 0x7FFF, 0x7FFF);
    SetDefDispEnv((s32) &spEC, (s32) *(s16 *)((unsigned char *)D_80125C58 + (28)), (s32) *(s16 *)((unsigned char *)D_80125C58 + (30)), (s32) *(s16 *)((unsigned char *)D_80125C58 + (32)), /* extra? */ (s32) *(s16 *)((unsigned char *)D_80125C58 + (34)));
    func_8009F57C((s32) &sp90, (s32) *(s16 *)((unsigned char *)D_80125C58 + (28)), (s32) *(s16 *)((unsigned char *)D_80125C58 + (30)), (s32) *(s16 *)((unsigned char *)D_80125C58 + (32)), (s32) *(s16 *)((unsigned char *)D_80125C58 + (34)));
    if (D_800B5900 & 1) {
        spFD = 1;
        spF0 = (s16) ((s32) (spF0 * 2) / 3);
    }
    func_8009CCF4((s32) &spEC);
    func_8009CC04((s32) &sp90);
    sp48 = arg1 - 3;
    sp54 = D_800E6280.unk_10F4;
loop_13:
    func_800868BC(*(s32 *)((unsigned char *)(D_80125C58 + (*(s32 *)((unsigned char *)D_80125C58 + (12)) * 4)) + (4)), (D_800B5900 & 1) + 2);
    func_80086944(*(s32 *)((unsigned char *)D_80125C58 + (16)), (s32) (*(s16 *)((unsigned char *)D_80125C58 + (44)) * *(s16 *)((unsigned char *)D_80125C58 + (46))) / 2);
    strNextVlc((s32) (D_80125C58 + 4));
    if (!(D_800B5900 & 1)) {
        func_800422C8();
        func_8005715C((s32) ((u32) (D_800E6280.unk_10F4 - sp54) / 6U));
        func_800476C0();
        disp_string();
        func_8009AC20();
        func_8009AD00((s32) ((D_8011ECA0 * 0x14) + &D_800E8C70));
    }
    strSync((s32) (D_80125C58 + 4), 0);
    func_800AD950(0);
    var_v0_2 = 1;
    if (*(s32 *)((unsigned char *)D_80125C58 + (36)) != 0) {
        var_v0_2 = 0;
    }
    temp_s1 = var_v0_2 * 8;
    temp_s0 = D_80125C58 + temp_s1;
    SetDefDispEnv((s32) &spEC, (s32) *(s16 *)((unsigned char *)temp_s0 + (20)), (s32) *(s16 *)((unsigned char *)temp_s0 + (22)), (s32) *(s16 *)((unsigned char *)temp_s0 + (24)), /* extra? */ (s32) *(s16 *)((unsigned char *)temp_s0 + (26)));
    temp_s0_2 = D_80125C58 + temp_s1;
    func_8009F57C((s32) &sp90, (s32) *(s16 *)((unsigned char *)temp_s0_2 + (20)), (s32) *(s16 *)((unsigned char *)temp_s0_2 + (22)), (s32) *(s16 *)((unsigned char *)temp_s0_2 + (24)), (s32) *(s16 *)((unsigned char *)temp_s0_2 + (26)));
    if (D_800B5900 & 1) {
        spFD = 1;
        spF0 = (s16) ((s32) (spF0 * 2) / 3);
    }
    func_8009CCF4((s32) &spEC);
    func_8009CC04((s32) &sp90);
    func_8009C5E0(1);
    if ((D_800B58F0 < sp48) && (var_s2 < (arg1 - 0xF))) {
        D_800E6280.unk_10F8 += 1;
        var_s2 += 1;
        if (*(s32 *)((unsigned char *)D_80125C58 + (52)) != 1) {
            func_80042A00();
            if (!(D_800E6280.unk_F88 & 0x860)) {
                goto loop_13;
            }
        }
    }
    spFD = 0;
    func_8009CCF4((s32) &spEC);
    func_8009C5E0(0);
    func_8004111C();
    var_s0_2 = 0x7F;
    var_a0 = 0x7F << 0x10;
    do {
        func_8008B750(var_a0 >> 0x10, (s32) var_s0_2);
        func_8008BFB0(0, (s32) var_s0_2, (s32) var_s0_2);
        var_s0_2 -= 0xA;
        var_a0 = var_s0_2 << 0x10;
    } while (var_s0_2 > 0);
    func_8009C5E0(1);
    func_8009C674(0);
}
