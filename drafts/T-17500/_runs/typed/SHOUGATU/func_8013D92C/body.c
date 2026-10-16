void func_8013D92C(void) {
    u32 temp_t4;

    if (D_800E6280.unk_10C.unk_02 >= (s16) ((((u32) (D_800E6280.unk_1BC[0xC].unk_0C.w << 0x14) >> 0x1D) * 0x19) + 0x96)) {
        temp_t4 = (u32) (D_800E6280.unk_1BC[0xE].unk_0C.w << 0x14) >> 0x1D;
        D_80145F2C = ((temp_t4 != 0) * 3) + 4;
        D_800E6280.unk_1BC[0xE].unk_0C.b[1] = (((temp_t4 + 1) * 2) & 0xE) | (D_800E6280.unk_1BC[0xE].unk_0C.b[1] & 0xFFF1);
        func_8004284C();
        return;
    }
    D_80145F2C += 1;
    D_800E6280.unk_118.unk_02 += 0xA;
    D_800E6280.unk_11C.unk_02 += 0xA;
    check_para_limit();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
}
