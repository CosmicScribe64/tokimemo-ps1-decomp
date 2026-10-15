extern ? D_80085368;
extern u8 D_800E738A;
extern u8 D_80120657;
extern u8 D_801206DB;
extern u8 D_801206DF;
extern ? D_8013C228;
extern ? func_800853FC;

void func_80134B94(void) {
    s32 temp_v0;

    M2C_MEMCPY_ALIGNED(&sp30[0], &D_8013C228, 0xA4);
    (&sp30[0])[D_800E738A](0x80);
    temp_v0 = *(s32 *)((unsigned char *)(sp + (D_800E738A * 4)) + (48));
    if (&D_80085368 == temp_v0) {
        D_801206DB |= 0x80;
        D_801206DF = D_80120657;
    }
    if (&func_800853FC == temp_v0) {
        D_801206DF = D_80120657;
        if ((u8) D_80120657 < 9U) {
            D_801206DB &= 0xFF7F;
        }
    }
}
