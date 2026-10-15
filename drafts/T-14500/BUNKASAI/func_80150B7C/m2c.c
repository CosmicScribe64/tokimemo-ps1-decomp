extern u8 D_800E62BE;
extern u16 D_800E6374;
extern u8 D_800E644A;
extern s32 D_800E6480;
extern s32 D_800E64B8;
extern s32 D_800E64F0;
extern u8 D_800E738A;

void func_80150B7C(void) {
    u32 temp_t2;

    switch (D_800E738A) {                           /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        D_800CA148 = ((((u16) D_800E6374 >> 0xC) * 3) + D_800E62BE) - 0x4D;
        func_8004284C();
        return;
    case 1:                                         /* switch 1 */
        func_80085A60(2);
        return;
    case 2:                                         /* switch 1 */
        D_80122EB8 = (s32) ((u16) D_800E6374 >> 0xC);
        temp_t2 = (u16) D_800E6374 >> 0xC;
        switch (temp_t2) {                          /* switch 2 */
        case 0:                                     /* switch 2 */
            if (((u8) D_800E644A >> 4) == 0) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[1].unk_0C.b[2] >> 4) == 0) && (((u32) (D_800E6280.unk_1BC[1].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x2A);
                return;
            }
            func_80042908(0xE);
            return;
        case 1:                                     /* switch 2 */
            if (((u8) D_800E644A >> 4) == 1) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[1].unk_0C.b[2] >> 4) == 1) && (((u32) (D_800E6480 << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x2A);
                return;
            }
            func_80042908(0xE);
            return;
        case 2:                                     /* switch 2 */
            if (((u8) D_800E644A >> 4) == 2) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[2].unk_0C.b[2] >> 4) == 2) && (((u32) (D_800E64B8 << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x3B);
                return;
            }
            func_80042908(0xE);
            return;
        case 3:                                     /* switch 2 */
            if (((u8) D_800E644A >> 4) == 3) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[2].unk_0C.b[2] >> 4) == 3) && (((u32) (D_800E6280.unk_1BC[2].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x3B);
                return;
            }
            func_80042908(0xE);
            return;
        case 4:                                     /* switch 2 */
            if (((u8) D_800E644A >> 4) == 4) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[3].unk_0C.b[2] >> 4) == 4) && (((u32) (D_800E64F0 << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x50);
                return;
            }
            func_80042908(0xE);
            return;
        case 5:                                     /* switch 2 */
            if (((u8) D_800E644A >> 4) == 5) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[3].unk_0C.b[2] >> 4) == 5) && (((u32) (D_800E6280.unk_1BC[3].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x50);
                return;
            }
            func_80042908(0xE);
            return;
        default:                                    /* switch 2 */
            func_80046500();
            return;
        }
        break;
    default:                                        /* switch 1 */
        func_80046500();
        return;
    }
}
