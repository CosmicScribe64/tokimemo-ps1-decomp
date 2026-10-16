extern u8 D_800E71DF;
extern u8 D_800E738A;

s32 func_801398E8(void) {
    u8 sp2B;
    u8 temp_a0;

    if (D_800E71DF == D_80122CDC) {
        sp2B = D_800E738A;
        func_801386C4();
        if (sp2B != D_800E738A) {
            D_800CA2DC -= 2;
            D_800E738A -= 3;
        }
        return 0;
    }
    temp_a0 = D_80122CDC & 0xFF;
    if (D_80122CDC == 0xB) {
        D_800CA2DC = 0;
        D_800CA2D0 = D_8013C450;
        D_800CA2D4 = D_8013C484;
        D_800CA2D8 = D_8013C4B8;
        D_800CA2EC = 1;
        D_800E71DF = 0xD;
        func_800847B8(0xDU);
        D_800E738A = 0x33;
        return 0;
    }
    D_800E71DF = temp_a0;
    func_800847B8(temp_a0);
    func_801399F4();
    return func_8004284C();
}
