s32 func_80132834(void) {
    s32 i;

    switch (D_800E6280.unk_110A) {
    case 0:
        D_800E6280.unk_03A = 0;
        func_8004E44C(0, (u32 *) D_80139218, D_80139200);
        srn_init(0, (u32 *) D_80139214, 0);
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
            /* FAKE: the first two of six iterations written out: the original stores them through absolute
             * addresses with one constant each, then runs the loop from 2; one 6-trip loop unrolls the other
             * way in this IDO. T-18200 */
            D_800E6280.unk_1228[5] = -1;
            D_800E6280.unk_1228[37] = -1;
            D_800E6280.unk_1228[6] = -1;
            D_800E6280.unk_1228[38] = -1;
            for (i = 2; i < 6; i++) {
                D_800E6280.unk_1228[i + 5] = -1;
                D_800E6280.unk_1228[i + 37] = -1;
            }
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
