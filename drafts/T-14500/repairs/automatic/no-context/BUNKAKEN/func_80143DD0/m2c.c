? cd_read_sectors(u8, ?, s32, s32);                 /* extern */
s32 cd_read_sync_status();                          /* extern */
? func_8004284C(s32);                               /* extern */
? func_80042908(?, ?);                              /* extern */
? func_80042940(?, s32);                            /* extern */
? func_80046500();                                  /* extern */
s32 func_80051A68(s32, s32);                        /* extern */
? func_8007C740();                                  /* extern */
? func_8007E390();                                  /* extern */
? func_8007E5A0();                                  /* extern */
? func_80083174();                                  /* extern */
? func_80083474();                                  /* extern */
? func_800847B8(s32, ?);                            /* extern */
? func_8008484C();                                  /* extern */
? func_80085A60();                                  /* extern */
u32 func_8008667C(s32, s32 *);                      /* extern */
? func_800866A0(s32, s32);                          /* extern */
? func_801324E0();                                  /* extern */
? func_8013BFA0();                                  /* extern */
extern s16 D_800CA148;
extern s16 D_800CA14C;
extern s32 D_800CA160;
extern s32 D_800CA164;
extern s32 D_800CA168;
extern ? D_800E6280;
extern u8 D_800E62BE;
extern u16 D_800E6374;
extern s32 D_800E6448;
extern u8 D_800E644A;
extern u8 D_800E6482;
extern u8 D_800E64BA;
extern u8 D_800E64F2;
extern u8 D_800E652A;
extern s8 D_800E71DF;
extern u8 D_800E738A;
extern s32 D_80122CDC;
extern s32 D_80122EA4;
extern s32 D_80122EBC;
extern s32 D_80155E90;
extern s32 D_80155E94;
extern s32 D_80155E98;
extern ? D_80155EA0;
extern ? D_80155ED0;
extern ? D_80155F00;
extern ? D_80155F0C;

void func_80143DD0(? arg3) {
    ? var_a3;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a3;
    s32 temp_a3_2;
    s32 var_v0;
    s32 var_v0_2;
    u32 temp_t4;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;
    u32 temp_v0_4;
    u32 temp_v0_5;
    u32 temp_v0_7;
    void *temp_v0_6;

    var_a3 = arg3;
    switch (D_800E738A) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        if (D_80122EBC != 0xFF) {
            cd_read_sectors(5U, 0x801E8000, 0x49BA, D_80122EBC);
            func_8004284C();
        } else {
            D_800CA148 = 1;
            D_800CA14C = 0;
            func_80042940(2, D_80122EBC);
        }
        break;
    case 1:                                         /* switch 1 */
        if (cd_read_sync_status() & 1) {
            func_801324E0();
            D_800CA148 = 1;
            D_800CA14C = 0;
            D_800CA160 = D_80155E90;
            D_800CA164 = D_80155E94;
            D_800CA168 = D_80155E98;
            func_8004284C();
        }
        break;
    case 2:                                         /* switch 1 */
        func_80085A60();
        break;
    case 3:                                         /* switch 1 */
        func_80083174();
        break;
    case 4:                                         /* switch 1 */
        if (D_80122CDC == 1) {
            func_80042940(6);
        } else {
            func_80042940(7);
        }
        break;
    case 5:                                         /* switch 1 */
        func_80085A60();
        break;
    case 6:                                         /* switch 1 */
        if (D_80122EBC != 0xFF) {
            D_80122EBC = 0xFF;
        }
        func_80042908(2, 0xFF);
        break;
    case 7:                                         /* switch 1 */
        func_80085A60();
        break;
    case 8:                                         /* switch 1 */
        func_8008484C();
        break;
    case 9:                                         /* switch 1 */
        if (D_80122CDC == D_80122EA4) {
            D_800CA148 = 4;
            D_800CA14C = 0;
            func_8004284C(D_80122CDC);
        } else if (D_80122CDC == 0xB) {
            D_800CA148 = 5;
            D_800CA14C = 0;
            func_80042940(5);
        } else {
            if ((D_80122CDC == 0) && (var_a3 = 0xB, (((u32) ((u8) D_800E644A >> 4) < 6U) == 0))) {
                D_80122EBC = 0xB;
            } else {
                D_80122EBC = D_80122CDC;
            }
            func_800847B8(D_80122CDC, var_a3);
            func_80042940(0xC);
        }
        break;
    case 10:                                        /* switch 1 */
        func_80085A60();
        break;
    case 11:                                        /* switch 1 */
        D_800CA148 = 2;
        func_80042940(7);
        break;
    case 12:                                        /* switch 1 */
        func_80085A60();
        break;
    case 13:                                        /* switch 1 */
        func_800866A0(*(&D_80155EA0 + (D_80122EBC * 4)), D_80122EBC);
        break;
    case 14:                                        /* switch 1 */
        func_8007C740();
        if (D_80122EBC == 0xB) {
            D_800E71DF = 0;
        } else {
            D_800E71DF = (s8) D_80122EBC;
        }
        func_80083474();
        cd_read_sectors(*(&D_80155F00 + D_80122EBC), 0x801E8000, *(&D_80155F0C + (D_80122EBC * 4)), D_80122EBC);
        break;
    case 15:                                        /* switch 1 */
        func_8007E390();
        break;
    case 16:                                        /* switch 1 */
        if (cd_read_sync_status() & 1) {
            *(&D_80155ED0 + (D_80122EBC * 4))();
            func_8013BFA0();
            if (D_80122EBC == 0) {
                func_80042940(0x14);
            } else if (D_80122EBC < 4) {
                func_80042940(0x1F);
            } else {
                func_80042940(0x2A);
            }
        }
        break;
    case 17:                                        /* switch 1 */
        func_8007E5A0();
        break;
    case 18:                                        /* switch 1 */
        func_80085A60();
        break;
    case 19:                                        /* switch 1 */
        D_80122EBC = 0xFF;
        func_80042908(2, 0xFF);
        break;
    case 20:                                        /* switch 1 */
        temp_v0 = func_8008667C(0);
        switch (temp_v0) {                          /* switch 2 */
        case 0:                                     /* switch 2 */
        case 1:                                     /* switch 2 */
            D_800CA148 = 0x10;
            break;
        case 2:                                     /* switch 2 */
            D_800CA148 = 0x1B;
            break;
        case 3:                                     /* switch 2 */
            D_800CA148 = 0x24;
            break;
        case 4:                                     /* switch 2 */
            D_800CA148 = 0x2D;
            break;
        }
        D_800CA14C = 0;
        func_8004284C();
        break;
    case 21:                                        /* switch 1 */
        func_80085A60();
        break;
    case 22:                                        /* switch 1 */
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {                        /* switch 3 */
        case 0:                                     /* switch 3 */
        case 1:                                     /* switch 3 */
            D_800CA148 = 0x11;
            break;
        case 2:                                     /* switch 3 */
            D_800CA148 = 0x1C;
            break;
        case 3:                                     /* switch 3 */
            D_800CA148 = 0x25;
            break;
        case 4:                                     /* switch 3 */
            D_800CA148 = 0x2E;
            break;
        }
        D_800CA148 += (u8) D_800E644A >> 4;
        func_8004284C();
        break;
    case 23:                                        /* switch 1 */
        func_80085A60();
        break;
    case 24:                                        /* switch 1 */
        if (func_8008667C(0) == 4) {
            D_800CA148 = 0x34;
            func_80042940(0x11);
        } else {
            if (D_800E6448 & 4) {
                D_800CA148 = 0x17;
            } else {
                D_800CA148 = 0x18;
            }
            func_8004284C();
        }
        D_800E6448 = (s8) ((u8) D_800E6448 | 4);
        break;
    case 25:                                        /* switch 1 */
        func_80085A60();
        break;
    case 26:                                        /* switch 1 */
        temp_v0_3 = func_8008667C(0);
        switch (temp_v0_3) {                        /* switch 4; irregular */
        case 0:                                     /* switch 4 */
        case 1:                                     /* switch 4 */
            D_800CA148 = 0x19;
            func_8004284C();
            break;
        case 2:                                     /* switch 4 */
            D_800CA148 = 0x22;
            func_8004284C();
            break;
        case 3:                                     /* switch 4 */
            D_800CA148 = 0x2B;
            func_8004284C();
            break;
        }
        /* fallthrough */
    case 27:                                        /* switch 1 */
        func_80085A60();
        break;
    case 28:                                        /* switch 1 */
        func_8007E5A0();
        break;
    case 29:                                        /* switch 1 */
        func_80085A60();
        break;
    case 30:                                        /* switch 1 */
        D_80122EBC = 0xFF;
        func_80042908(2, 0xFF);
        break;
    case 31:                                        /* switch 1 */
        temp_v0_4 = func_8008667C(D_80122EBC, &D_80122EBC);
        switch (temp_v0_4) {                        /* switch 5 */
        case 0:                                     /* switch 5 */
        case 1:                                     /* switch 5 */
            D_800CA148 = 8;
            break;
        case 2:                                     /* switch 5 */
            D_800CA148 = 0xF;
            break;
        case 3:                                     /* switch 5 */
            D_800CA148 = 0x15;
            break;
        case 4:                                     /* switch 5 */
            D_800CA148 = 0x1B;
            break;
        }
        D_800CA14C = 0;
        func_8004284C();
        break;
    case 32:                                        /* switch 1 */
        func_80085A60();
        break;
    case 33:                                        /* switch 1 */
        temp_v0_5 = func_8008667C(D_80122EBC, &D_80122EBC);
        switch (temp_v0_5) {                        /* switch 6 */
        case 0:                                     /* switch 6 */
        case 1:                                     /* switch 6 */
            D_800CA148 = 9;
            break;
        case 2:                                     /* switch 6 */
            D_800CA148 = 0x10;
            break;
        case 3:                                     /* switch 6 */
            D_800CA148 = 0x16;
            break;
        case 4:                                     /* switch 6 */
            D_800CA148 = 0x1C;
            break;
        }
        if (((D_80122EBC == 1) && (((u8) D_800E6482 >> 4) == 1)) || ((D_80122EBC == 2) && (((u8) D_800E64BA >> 4) == 3)) || ((D_80122EBC == 3) && (((u8) D_800E64F2 >> 4) == 5))) {
            D_800CA148 += 1;
        }
        func_8004284C();
        break;
    case 34:                                        /* switch 1 */
        func_80085A60();
        break;
    case 35:                                        /* switch 1 */
        if (func_8008667C(D_80122EBC, &D_80122EBC) == 4) {
            D_800CA148 = 0xE;
            func_80042940(0x11);
        } else {
            if ((D_80122EBC == 1) && (func_8008667C(1) < 2)) {
                D_800CA148 = 0x12;
            } else if (((u32) (*(&D_800E6448 + (D_80122EBC * 0x38)) << 0x1D) >> 0x1F) == 1) {
                D_800CA148 = 0xB;
            } else {
                D_800CA148 = 0x14;
            }
            func_8004284C();
        }
        temp_v0_6 = (D_80122EBC * 0x38) + &D_800E6280;
        *(u8 *)((unsigned char *)temp_v0_6 + (456)) = (u8) (*(u8 *)((unsigned char *)temp_v0_6 + (456)) | 4);
        break;
    case 36:                                        /* switch 1 */
        func_80085A60();
        break;
    case 37:                                        /* switch 1 */
        temp_v0_7 = func_8008667C(D_80122EBC, &D_80122EBC);
        switch (temp_v0_7) {                        /* switch 7; irregular */
        case 0:                                     /* switch 7 */
        case 1:                                     /* switch 7 */
            D_800CA148 = 0xC;
            if (((D_80122EBC == 1) && (((u8) D_800E6482 >> 4) == 1)) || ((D_80122EBC == 2) && (((u8) D_800E64BA >> 4) == 3)) || ((D_80122EBC == 3) && (((u8) D_800E64F2 >> 4) == 5))) {
                D_800CA148 = 0xD;
            }
            func_8004284C();
            break;
        case 2:                                     /* switch 7 */
            D_800CA148 = 0x13;
            func_8004284C();
            break;
        case 3:                                     /* switch 7 */
            D_800CA148 = 0x19;
            func_8004284C();
            break;
        }
        break;
    case 38:                                        /* switch 1 */
        func_80085A60();
        break;
    case 39:                                        /* switch 1 */
        func_8007E5A0();
        D_800CA148 = 0xE;
        break;
    case 40:                                        /* switch 1 */
        func_80085A60();
        break;
    case 41:                                        /* switch 1 */
        D_80122EBC = 0xFF;
        func_80042908(2, 0xFF);
        break;
    case 42:                                        /* switch 1 */
        temp_a3 = D_80122EBC;
        temp_a0 = temp_a3;
        if (temp_a3 == 0xB) {
            var_v0 = func_80051A68(0, temp_a3);
        } else {
            D_80122EBC = temp_a3;
            var_v0 = func_80051A68(temp_a0, temp_a3);
        }
        temp_t4 = var_v0 & 0x7F;
        switch (temp_t4) {                          /* switch 8 */
        case 0:                                     /* switch 8 */
        case 1:                                     /* switch 8 */
            D_800CA148 = 3;
            break;
        case 2:                                     /* switch 8 */
            D_800CA148 = 4;
            break;
        case 3:                                     /* switch 8 */
            D_800CA148 = 5;
            break;
        case 4:                                     /* switch 8 */
            if ((D_80122EBC == 4) && (((u16) D_800E6374 >> 0xC) == ((u8) D_800E652A >> 4))) {
                D_800CA148 = 8;
            } else {
                D_800CA148 = 6;
            }
            break;
        }
        D_800CA14C = 0;
        func_8004284C();
        break;
    case 43:                                        /* switch 1 */
        func_80085A60();
        break;
    case 44:                                        /* switch 1 */
        temp_a3_2 = D_80122EBC;
        temp_a0_2 = temp_a3_2;
        if (temp_a3_2 == 0xB) {
            var_v0_2 = func_80051A68(0, temp_a3_2);
        } else {
            D_80122EBC = temp_a3_2;
            var_v0_2 = func_80051A68(temp_a0_2, temp_a3_2);
        }
        if (var_v0_2 == 4) {
            func_80042940(0x11);
        } else if ((D_80122EBC == 8) && (func_8008667C(8) >= 2)) {
            func_8004284C();
        } else {
            func_80042908(3);
        }
        break;
    case 45:                                        /* switch 1 */
        switch (D_800E62BE) {                       /* switch 9; irregular */
        case 0x5F:                                  /* switch 9 */
            D_800CA148 = 9;
            break;
        case 0x60:                                  /* switch 9 */
            D_800CA148 = 0xA;
            break;
        case 0x61:                                  /* switch 9 */
            D_800CA148 = 0xB;
            break;
        }
        if (func_8008667C(8) == 3) {
            D_800CA148 += 3;
        }
        func_8004284C();
        break;
    case 46:                                        /* switch 1 */
        func_80085A60();
        break;
    case 47:                                        /* switch 1 */
        D_800CA148 = 0xF;
        func_8004284C();
        break;
    case 48:                                        /* switch 1 */
        func_80085A60();
        break;
    case 49:                                        /* switch 1 */
        func_80042908(3);
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
