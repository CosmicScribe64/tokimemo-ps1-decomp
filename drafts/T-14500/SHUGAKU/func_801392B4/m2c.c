extern u8 D_800E71DF;
extern u8 D_800E738A;
extern s16 D_8013C970;

s32 func_801392B4(void) {
    s32 sp28;
    s16 temp_t2;

    if (D_80122CDC == 0) {
        if (D_800E71DF == 8) {
            D_800CA2DC = 6;
            sp28 = (s32) D_800E738A;
            func_801386C4();
            if (sp28 == D_800E738A) {
                return 0;
            }
            D_800E738A -= 1;
            goto block_5;
        }
block_5:
        temp_t2 = D_800CA2CC + 0x1A;
        D_8013C970 = temp_t2;
        D_800CA2DC = temp_t2;
        func_8004284C();
        func_8004284C();
        func_8004284C();
        goto block_7;
    }
    D_800E6280.unk_1BC[D_800E71DF].unk_02 -= 1;
    D_800E6280.unk_1BC[D_800E71DF].unk_06 -= 2;
    func_80084D3C(0x38);
block_7:
    return func_8004284C();
}
