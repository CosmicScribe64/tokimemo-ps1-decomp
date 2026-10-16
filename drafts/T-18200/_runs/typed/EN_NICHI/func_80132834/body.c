s32 func_80132834(void) {
    u8 temp_v1;
    u8 *temp_v0;

    temp_v1 = D_800E6280.unk_110A;
    switch (temp_v1) {
    case 0:
        D_800E6280.unk_03A = 0;
        func_8004E44C(0, (u32 *) D_80139218, D_80139200);
        srn_init(0, D_80139214, 0);
        srn_vram_set(0, 5, 0x580, 0xF0);
        Fade_In_Init();
        func_8004284C();
        break;
    case 1:
        if (func_800460EC() == 2) {
            func_8004284C();
        }
        break;
    case 2:
        Fade_In_Main();
        if (D_800E6280.unk_F88 & 0x60) {
            func_80044750(0x74);
            Fade_Out_Init();
            func_8004284C();
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        break;
    case 3:
        if (Fade_Out_Main() != 0) {
            D_800E6280.unk_1228[5] = -1;
            D_800E6280.unk_1228[0x25] = -1;
            D_800E6280.unk_1228[6] = -1;
            D_800E6280.unk_1228[0x26] = -1;
            temp_v0 = (2 * 4) + &D_800E6280;
            *(s32 *)((unsigned char *)temp_v0 + (4672)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4800)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4676)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4804)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4680)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4808)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4668)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4796)) = -1;
            func_80044750(0x201);
            func_80042808();
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        break;
    default:
        func_80046500();
        break;
    }
}
