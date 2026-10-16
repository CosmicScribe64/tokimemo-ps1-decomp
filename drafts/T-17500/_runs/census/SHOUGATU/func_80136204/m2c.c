extern s32 D_800E6378;
extern s32 D_800E644C;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E699E;
extern u8 D_800E71DF;
extern u8 D_800E738A;
void func_80136204(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = get_g_zyotai_s(D_800E71DF);
    temp_v1 = *(&D_800E644C + (D_800E71DF * 0x38));
    if ((D_800E62C0 == ((u32) (temp_v1 << 0x17) >> 0x1B)) && (D_800E62BF == (temp_v1 & 0xF)) && (D_80143B20 == 0) && (D_800E71DF == 0)) {
        D_800E699E |= 1;
        D_800E738A += 4;
    } else if ((D_800E62C0 == ((u32) (D_800E6378 << 0x17) >> 0x1B)) && (D_800E62BF == (D_800E6378 & 0xF)) && (D_80143B20 == 0)) {
        if ((u32) (temp_v0 & 0x7F) < 2U) {
            D_800E699E |= 4;
            D_800E738A += 0x1B;
        } else {
            D_800E699E |= 8;
        }
    }
    func_8004284C();
}
