? func_800639D8();                                  /* extern */
? func_80065048(s16, ?);                            /* extern */
extern s8 D_800E62BA;
extern s16 D_800E643E;
extern s16 D_800E6442;
extern u8 D_800E699E;
extern u8 D_800E71DF;
extern s32 D_800E7378;

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
    D_800E699E |= 5;
    D_80122D38 = 1;
    D_800E62BA = 0x80;
    D_800B593C = 0x80;
    D_800B5940 = 0;
    func_800AE120(D_800E7378);
    D_800E71DF = 0;
    func_800AE0F0(D_800CA19C, "詩織の部屋");
    func_800AE0F0(D_800CA1DC, "自宅前");
    D_800CA150 = 0;
    D_800CA154 = 0;
    func_8007C740();
    func_800847B8(D_800E71DF);
    func_80137990();
    D_80146274 = D_80145EF4;
    D_80146278 = D_80146030;
    D_8014627C = D_8014616C;
    D_800E643E += 2;
    D_800E6442 += 2;
    func_80084D3C();
    func_8004284C();
}
