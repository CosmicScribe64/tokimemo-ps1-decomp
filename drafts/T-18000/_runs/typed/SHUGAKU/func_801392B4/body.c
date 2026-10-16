s32 func_801392B4(void) {
    s32 sp28;
    s16 temp_t2;
    u8 temp_t3;
    u8 temp_t7;
    u8 temp_v0;

    if (D_80122CDC == 0) {
        if (D_800E6280.unk_F5F == 8) {
            D_800CA2DC = 6;
            sp28 = (s32) D_800E6280.unk_110A;
            func_801386C4();
            temp_v0 = D_800E6280.unk_110A;
            if (sp28 == temp_v0) {
                return 0;
            }
            D_800E6280.unk_110A = temp_v0 - 1;
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
    temp_t3 = D_800E6280.unk_F5F;
    D_800E6280.unk_1BC[temp_t3].unk_02 -= 1;
    temp_t7 = D_800E6280.unk_F5F;
    D_800E6280.unk_1BC[temp_t7].unk_06 -= 2;
    check_para_limit(0x38);
block_7:
    return func_8004284C();
}
