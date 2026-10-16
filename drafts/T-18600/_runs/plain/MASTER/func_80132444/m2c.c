extern u16 D_800E6374;
extern u8 D_800E69A0;
extern s8 D_800E69A1;
extern s8 D_800E69A2;
extern u8 D_800E7388;
extern u8 D_800E7389;
extern u8 D_800E738A;

void func_80132444(void) {
    u32 temp_t8;

    switch (D_800E738A) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        temp_t8 = (u16) D_800E6374 >> 0xC;
        switch (temp_t8) {                          /* switch 2 */
        case 0:                                     /* switch 2 */
            cd_read_sectors(3U, 0x801E8000, 0xA9DE);
            func_80132104();
            break;
        case 1:                                     /* switch 2 */
            cd_read_sectors(3U, 0x801E8000, 0xA9B4);
            func_80132000();
            break;
        case 2:                                     /* switch 2 */
            cd_read_sectors(3U, 0x801E8000, 0xA9BF);
            func_8013209C();
            break;
        case 3:                                     /* switch 2 */
            cd_read_sectors(3U, 0x801E8000, 0xA9B7);
            func_80132034();
            break;
        case 4:                                     /* switch 2 */
            cd_read_sectors(3U, 0x801E8000, 0xA9BC);
            func_80132068();
            break;
        case 5:                                     /* switch 2 */
            cd_read_sectors(3U, 0x801E8000, 0xA9C2);
            func_801320D0();
            break;
        default:                                    /* switch 2 */
            func_80046500();
            break;
        }
        D_800CA160 = (s32) D_8013C2D0;
        D_800CA164 = (s32) D_8013C2D4;
        D_800CA168 = (s32) D_8013C2D8;
        func_8004284C();
        break;
    case 1:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0;
            D_800CA14C = 0;
            func_8004284C();
        }
        break;
    case 2:                                         /* switch 1 */
        func_80085A60();
        break;
    case 3:                                         /* switch 1 */
        func_8007E934();
        break;
    case 4:                                         /* switch 1 */
        if (((u16) D_800E6374 >> 0xC) == 3) {
            func_80044750(0xBF);
            D_800E69A0 = D_800E7388;
            D_800E69A1 = D_800E7389 + 1;
            D_800E69A2 = 0;
            D_800E7D50 |= 3;
            main_state_set(0x90);
        } else {
            func_80042808();
        }
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
    func_80086424();
}
