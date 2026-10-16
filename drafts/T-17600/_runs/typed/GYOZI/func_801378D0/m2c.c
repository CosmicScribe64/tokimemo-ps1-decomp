void func_801378D0(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_8005E0E0(D_800F62CF);
    temp_v1 = *(&D_800F53A0 + (D_800F62CF * 0x38));
    if ((D_800F53E0 == ((u32) (temp_v1 << 0x17) >> 0x1B)) && (D_800F53DF == (temp_v1 & 0xF)) && (D_80145F60 == 0) && (D_800F62CF == 0)) {
        D_800F5AAE |= 1;
        D_800F647A += 4;
    } else if ((D_800F53E0 == ((u32) (D_800F5490 << 0x17) >> 0x1B)) && (D_800F53DF == (D_800F5490 & 0xF)) && (D_80145F60 == 0)) {
        if ((u32) (temp_v0 & 0x7F) < 2U) {
            D_800F5AAE |= 4;
            D_800F647A += 0x1B;
        } else {
            D_800F5AAE |= 8;
        }
    }
    func_8004DE1C();
}
