? func_8004DE48(?);                                 /* extern */
extern s32 D_800D9264;

void func_80140464(void) {
    s32 temp_v1;
    u8 temp_t6;
    u8 temp_t7;

    temp_v1 = D_800F53A0.girl[0].unk_14;
    temp_t7 = (D_800F5AAE | 0x20) & 0xFF;
    if (((temp_v1 & 0xF) == 0xC) && (((u32) (temp_v1 << 0x17) >> 0x1B) == 0x18) && (temp_t7 != 0)) {
        D_800F5AAE = temp_t7;
        D_800F5AAE |= 1;
        *(D_800F5A2D + (D_800F5AAC * 4)) = 0;
        func_8004DE48(0x57);
        return;
    }
    temp_t6 = (temp_t7 | 0x40) & 0xFF;
    D_800F5AAE = temp_t6;
    if ((D_800F5490 & 0xF) == 0xC) {
        D_800F5AAE = temp_t6;
        if (((u32) (D_800F5490 << 0x17) >> 0x1B) == 0x18) {
            D_800F5AAE = temp_t6;
            if (temp_t6 != 0) {
                D_800F5AAE = temp_t6 | 4;
                func_8004DE48(0x56);
                return;
            }
        }
    }
    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    func_8009068C();
    func_800BCE10(&D_800D92A0, "伊集院邸");
    D_800F6412 = 0;
    D_800F6458 = 1;
    D_800F53DA = 0x80;
    D_801317EB = 0;
    D_800C51C4 = 0;
    D_80148620 = 0;
    D_80148624 = 0;
    func_80089200();
    func_800744A0();
    func_80074950();
    func_8008FC10();
    func_800BCE10(&D_800D9264, "プレイ");
    if (D_800F594E == 0) {
        func_800BCE10((s32 *) &D_800D9280, "黒服");
    } else {
        func_800BCE10((s32 *) &D_800D9280, "外井");
    }
    D_800F62CF = 0xC;
    func_8013EB10();
    D_80148614 = D_801481FC;
    D_80148618 = D_80148230;
    D_8014861C = D_80148264;
    func_8004DE1C();
}
