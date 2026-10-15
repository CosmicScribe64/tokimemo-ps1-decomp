? func_8007E5A0();                                  /* extern */
? func_8007E99C();                                  /* extern */
? func_80085ED0();                                  /* extern */
extern u16 D_800E6374;
extern u8 D_800E644A;
extern u8 D_800E652A;
extern u8 D_800E667A;
extern u8 D_800E71DF;
extern u8 D_800E738A;
extern ? D_8013435C;

void func_80132A28(void) {
    ? sp30;
    u8 sp2F;
    u32 temp_t7;
    u8 var_a1;

    sp30.unk0 = (s32) D_8013435C.unk0;
    *(s32 *)((unsigned char *)(&sp30) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8013435C) + (8));
    *(s32 *)((unsigned char *)(&sp30) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8013435C) + (4));
    *(s32 *)((unsigned char *)(&sp30) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8013435C) + (12));
    *(s32 *)((unsigned char *)(&sp30) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8013435C) + (16));
    *(u16 *)((unsigned char *)(&sp30) + (24)) = (u16) *(u16 *)((unsigned char *)(&D_8013435C) + (24));
    *(s32 *)((unsigned char *)(&sp30) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_8013435C) + (20));
    func_80083808();
    switch (D_800E738A) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        if (func_80044E8C() == 1) {
            cd_read_sectors(7U, 0x801E8000, 0xB037);
            func_8004284C();
        }
        break;
    case 2:                                         /* switch 1 */
    case 4:                                         /* switch 1 */
    case 10:                                        /* switch 1 */
        func_80085A60();
        break;
    case 1:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_8004500C(1, 0);
            func_800847B8(D_801342D0);
            func_80132000();
            D_800CA160 = D_801342B0;
            D_800CA164 = D_801342B4;
            D_800CA168 = D_801342B8;
            D_800CA148 = *(s16 *)((unsigned char *)(sp + (D_801342D0 * 2)) + (48));
            D_800CA14C = 0;
            func_8007C740();
            D_800E71DF = D_801342D0;
            func_80083474();
            if (D_801342D0 == 0xC) {
                func_80083440(0U);
            }
            if (D_801342D0 == 0xB) {
                func_80044750(0x500);
                func_80047550();
            }
        }
        break;
    case 3:                                         /* switch 1 */
        func_8007E390();
        break;
    case 5:                                         /* switch 1 */
        if (D_801342D0 == 0xB) {
            func_8007E5A0();
            D_800CA148 = 0x5E;
        } else {
            D_800CA148 = 0x10;
            func_8004284C();
        }
        break;
    case 6:                                         /* switch 1 */
        func_80044750(0xC1);
        func_80085ED0();
        func_8004284C();
        break;
    case 7:                                         /* switch 1 */
        if (D_801342D0 == 0xB) {
            func_80085A60();
        } else {
            func_8004284C();
        }
        break;
    case 8:                                         /* switch 1 */
        if (D_801342D0 == 0xB) {
            func_8007E934();
        } else {
            func_8007E99C();
        }
        break;
    case 9:                                         /* switch 1 */
        func_8004500C(0, 6);
        if (D_801342D0 == 0xB) {
            func_8004284C();
            func_8004284C();
        } else {
            func_8004284C();
        }
        break;
    case 11:                                        /* switch 1 */
        temp_t7 = (u16) D_800E6374 >> 0xC;
        switch (temp_t7) {                          /* switch 2 */
        case 6:                                     /* switch 2 */
            if (((((u8) D_800E644A >> 4) == 6) && (D_801342D0 == 0)) || ((var_a1 = 0, (((u8) D_800E652A >> 4) == 6)) && (D_801342D0 == 4))) {
                var_a1 = 5;
            }
block_53:
            sp2F = var_a1;
            break;
        case 7:                                     /* switch 2 */
            if (((((u8) D_800E644A >> 4) == 7) && (D_801342D0 == 0)) || ((var_a1 = 1, (((u8) D_800E652A >> 4) == 7)) && (D_801342D0 == 4))) {
                var_a1 = 6;
            }
            goto block_53;
        case 8:                                     /* switch 2 */
            if (((((u8) D_800E644A >> 4) == 8) && (D_801342D0 == 0)) || (var_a1 = 2, (D_801342D0 == 5))) {
                var_a1 = 7;
            }
            goto block_53;
        case 9:                                     /* switch 2 */
            if (((((u8) D_800E644A >> 4) == 9) && (D_801342D0 == 0)) || (var_a1 = 3, (D_801342D0 == 6))) {
                var_a1 = 8;
            }
            goto block_53;
        case 10:                                    /* switch 2 */
            if (((((u8) D_800E644A >> 4) == 0xA) && (D_801342D0 == 0)) || ((var_a1 = 4, (((u8) D_800E667A >> 4) == 0xA)) && (D_801342D0 == 0xA))) {
                var_a1 = 9;
            }
            goto block_53;
        }
        func_80085B3C(0x12, (s32) sp2F);
        func_80065F34(0);
        func_80042808();
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
    func_80086424();
}
