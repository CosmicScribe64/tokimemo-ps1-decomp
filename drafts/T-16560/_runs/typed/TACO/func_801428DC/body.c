void func_801428DC(void) {
    D_800E7D5C = D_8015EDF8;
    D_800E7D60 = D_8015EDFC;
    D_800E7D64 = D_8015EE00;
    D_800E7D50 = (u8) (D_800E7D50 | 1);
    func_800AE090(&D_800E7D50 + 2, D_8015EE04, 3);
    func_800AE090(&D_800E7D55, D_8015EE08, 3);
    func_800AE090(&D_800E7D58, D_8015EE0C, 3);
    *(u8 *)((unsigned char *)(&D_800E7D50) + (1)) = (u8) ((((u8) D_8015EDE4 % 3U) & 0xF) | (((((u8) D_8015EDE0 % 3U) * 0x10) | (*(u8 *)((unsigned char *)(&D_800E7D50) + (1)) & 0xFF0F)) & 0xFFF0));
    D_800E7D50 = (u8) (((((u8) D_8015EDDC % 3U) * 8) & 0x18) | ((((D_8015EDD8 * 4) & 4) | (D_800E7D50 & 0xFFFB)) & 0xFFE7));
    func_80041584();
    draw2d3d(0, 0);
    D_800E6280.unk_014[0] = 0xA0;
    D_800E6280.unk_014[1] = 0xA0;
    func_8009C674(0);
    func_8009C210(3);
    func_80097D90(0x140, 0xF0, 0, 0, 0);
    if (func_80045288() == 0) {
        func_800452C4();
    }
    func_8005907C();
    func_80059048();
    func_801347A8();
    func_80048EB8(0);
    func_80048DAC(0);
    func_8004284C();
}
