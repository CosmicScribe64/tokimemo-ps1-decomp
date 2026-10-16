? func_800639D8();                                  /* extern */
? func_80065048(s16, ?);                            /* extern */

void func_8013AB4C(void) {
    s16 var_a0;
    s16 var_s0;

    func_8006509C();
    var_s0 = 0;
    var_a0 = 0;
    do {
        func_80065048(var_a0, 0);
        var_s0 += 1;
        var_a0 = var_s0;
    } while (var_s0 < 0xF);
    func_80065B0C(1);
    func_800639D8();
    func_800639D8();
    func_8008585C();
    func_80064E84();
    func_80084E4C();
    func_800649D4();
    func_8008585C();
    D_800E6280.unk_71E = D_800E6280.unk_71E | 5;
    D_80122D38 = 1;
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0x80;
    D_800B5940 = 0;
    func_800AE120((s32) D_800E6280.unk_10F8);
    D_800E6280.unk_F5F = 0;
    func_800AE0F0(D_800CA19C, "詩織の部屋");
    func_800AE0F0(D_800CA1DC, "自宅前");
    D_800CA150 = 0;
    D_800CA154 = 0;
    func_8007C740();
    func_800847B8(D_800E6280.unk_F5F);
    func_80137990();
    D_80146274 = D_80145EF4;
    D_80146278 = D_80146030;
    D_8014627C = D_8014616C;
    D_800E6280.unk_1BC[0].unk_02 = D_800E6280.unk_1BC[0].unk_02 + 2;
    D_800E6280.unk_1BC[0].unk_06 = D_800E6280.unk_1BC[0].unk_06 + 2;
    func_80084D3C();
    func_8004284C();
}
