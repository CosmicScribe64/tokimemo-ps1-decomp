s32 Fade_In_Main(void) {
    if (Vblnk_Timer() != 0) {
        if ((u8) D_800E6280.unk_03A >= 0x7EU) {
            D_800E6280.unk_03A = 0x80;
            return 1;
        }
        D_800E6280.unk_03A += 4;
        Vblnk_Timer_Init();
        goto block_4;
    }
block_4:
    return 0;
}
