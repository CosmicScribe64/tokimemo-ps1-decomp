void func_801352B0(void) {
    s32 count;
    s32 page;

    if (*(u8 *)&D_800E6280.unk_03C == 2) {
        *(u8 *)&D_800E6280.unk_03C = 1;
    }
    count = D_800E6280.unk_1104.w;
    page = D_800E6280.unk_110A;
    if (!(D_800E6280.unk_F88 & 0x600060) || (D_801206FE < -0xBF)) {
        func_801343B0();
    }
    D_800E6280.unk_1104.w = count + 1;
    if (D_801206FE >= -0xBF) {
        if (!(D_800E6280.unk_1104.w & 1) && (func_8004ECB4() != 0)
            && ((*(u8 *)&D_800E6280.unk_03C == 0) || (func_80046094() == 9))) {
            D_801206FE--;
        }
        D_8011ED5B &= 0x7F;
    }
    if (page != D_800E6280.unk_1104.w) {
        if (D_801206FE >= -0xBF) {
            D_800E6280.unk_110A = page;
            D_800CA2E4 = 9;
            D_800CA2E8 = 0;
        } else {
            D_800CA2E4 = 9;
            D_800CA2E8 = 1;
        }
    }
}
