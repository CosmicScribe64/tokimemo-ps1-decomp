extern s32 D_800E6448;
extern u8 D_800E71DF;
s32 func_80133760(void) {
    s32 var_t8;

    if (D_80122CDC == 0xB) {
        func_8013397C();
        return 0;
    }
    if (D_800E71DF == D_80122CDC) {
        var_t8 = D_80122CDC * 8;
        if (D_80143B14 != 0) {
            func_8004284C();
            return 0;
        }
        goto block_6;
    }
    var_t8 = D_80122CDC * 8;
block_6:
    if (!(*(&D_800E6448 + ((var_t8 - D_80122CDC) * 8)) & 8)) {
        D_80143B00 = 0;
        D_80143AF4 = D_80143A78;
        D_80143AF8 = D_80143AB4;
        D_80143AFC = D_80143AF0;
        func_800847B8((u8) D_80122CDC);
        func_8004284C();
        return 0;
    }
    D_800E71DF = (u8) D_80122CDC;
    func_80132A34();
    func_800847B8(D_800E71DF);
    D_80143B00 = 9;
    func_8004284C();
    func_8004284C();
    return func_8004284C();
}
