void func_80133870(void) {
    s16 temp_a0;

    D_800E6280.unk_1104.u += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80133980();
        break;
    case 1:
        func_80133AF4();
        break;
    case 2:
        func_80133C08();
        break;
    case 3:
        func_80133E98();
        break;
    case 4:
        func_80134500();
        break;
    }
    if (D_800E6280.unk_1104.u & 0x10) {
        func_801432F0((s32) "NOW LOADING DATA ", -0x60, 0, 1);
    }
    if ((D_800E6280.unk_1104.u != 0) && ((u32) D_800E6280.unk_1104.u < 0x400U)) {
        temp_a0 = D_8015EDEC;
        D_8015EDEC -= 1;
        func_801430D0((s32) temp_a0, 0, 0x32, 6);
    }
}
