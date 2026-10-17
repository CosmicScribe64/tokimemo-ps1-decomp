s32 func_80043010(void) {
    s32 a;
    s32 b;

    if (D_800E6280.unk_F74 != 0xFF) {
        if (D_800E6280.unk_F74 != 0) {
            return (~D_800E8C32 << 24) + ((~D_800E8C33 & 0xFF) << 16) + ((~D_800E8BF2 & 0xFF) << 8) + (~D_800E8BF3 & 0xFF);
        }
        a = 0;
        b = ~D_800E8BF3 & 0xFF;
        if (b & 8) {
            a = 0x20;
        }
        if (b & 4) {
            a |= 0x40;
        }
        return a;
    }
}
