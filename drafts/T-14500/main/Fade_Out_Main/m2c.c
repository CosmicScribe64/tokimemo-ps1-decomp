extern u8 D_800E62BA;

s32 Fade_Out_Main(void) {
    if (Vblnk_Timer() != 0) {
        if ((u8) D_800E62BA < 3U) {
            D_800E62BA = 0;
            return 1;
        }
        D_800E62BA -= 4;
        Vblnk_Timer_Init();
        goto block_4;
    }
block_4:
    return 0;
}
