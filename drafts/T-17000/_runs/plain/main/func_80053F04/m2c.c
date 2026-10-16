extern u8 D_800E62B5;
extern u8 D_800E7395;
extern u8 D_800E739C;
extern s32 D_800E73A0;

s32 func_80053F04(void) {
    D_800E73A0 += 1;
    if (D_800E62B5 != 0) {
        switch ((u8) D_800E739C) {                  /* switch 1; irregular */
        case 0x0:                                   /* switch 1 */
            D_800E7395 = 0;
            Sw_Clear();
            if (_card_info(0) == 1) {
                func_80053CC0();
            }
            break;
        case 0x1:                                   /* switch 1 */
            func_80053D24(0x20);
            break;
        case 0x2:                                   /* switch 1 */
            if (_card_load(0) == 1) {
                func_80053CC0();
            }
            break;
        case 0x3:                                   /* switch 1 */
            func_80053D24(0x40);
            break;
        case 0x4:                                   /* switch 1 */
            D_800E62B5 = 0;
            D_800E7395 = 1;
            break;
        case 0x20:                                  /* switch 1 */
            func_800AD950(0);
            Hw_Clear();
            if (_card_clear(0) == 0) {
                func_80053CAC(0);
            } else {
                Hw_Test();
                switch (D_800E7395) {               /* switch 2; irregular */
                default:                            /* switch 2 */
                case 0x0:                           /* switch 2 */
                case 0x81:                          /* switch 2 */
                    func_80053CAC(0);
                    break;
                case 0x88:                          /* switch 2 */
                    func_80053CAC(0x30);
                    break;
                }
            }
            break;
        case 0x30:                                  /* switch 1 */
            if (_card_load(0) == 1) {
                func_80053CC0();
            }
            break;
        case 0x31:                                  /* switch 1 */
            func_80053D24(0x40);
            break;
        case 0x32:                                  /* switch 1 */
            D_800E62B5 = 0;
            D_800E7395 = 1;
            break;
        case 0x40:                                  /* switch 1 */
            D_800E62B5 = 0;
            D_800E7395 = 0xF1;
            break;
        }
    }
    return (s32) D_800E7395;
}
