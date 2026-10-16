extern ? D_800E7D55;
extern ? D_800E7D58;
extern s32 D_800E7D5C;
extern s32 D_800E7D60;
extern s32 D_800E7D64;

void func_801345D4(void) {
    if (D_800E7D50 & 1) {
        D_8015EDF8 = D_800E7D5C;
        D_8015EDFC = D_800E7D60;
        D_8015EE00 = D_800E7D64;
        D_8015EDD8 = (u8) ((u32) (*(s32 *)((unsigned char *)(&D_800E7D10) + (64)) << 0x1D) >> 0x1F);
        D_8015EDDC = (u8) ((u32) (*(s32 *)((unsigned char *)(&D_800E7D10) + (64)) << 0x1B) >> 0x1E);
        D_8015EDE0 = (u8) ((u16) *(s32 *)((unsigned char *)(&D_800E7D10) + (64)) >> 0xC);
        D_8015EDE4 = *(u8 *)((unsigned char *)(&D_800E7D10) + (65)) & 0xF;
        func_800AE090(D_8015EE04, &D_800E7D50 + 2, 3);
        func_800AE090(D_8015EE08, &D_800E7D55, 3);
        func_800AE090(D_8015EE0C, &D_800E7D58, 3);
        return;
    }
    *(s32 *)((unsigned char *)(&D_800E7D10) + (64)) = (s8) ((u8) *(s32 *)((unsigned char *)(&D_800E7D10) + (64)) | 1);
    D_8015EDF8 = 0x2EA694;
    D_8015EDFC = 0x1F6454;
    D_8015EE00 = 0x102214;
    func_800AE0F0(D_8015EE04, "K.K");
    func_800AE0F0(D_8015EE08, "K.T");
    func_800AE0F0(D_8015EE0C, "N.S");
}
