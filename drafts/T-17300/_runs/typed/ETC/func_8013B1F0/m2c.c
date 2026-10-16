? func_80062CD0(?);                                 /* extern */
? func_8013B98C(s32);                               /* extern */
? func_8013BA3C(s32);                               /* extern */
? func_8013BA80(s32);                               /* extern */
? func_8013BAC4(s32);                               /* extern */
? func_8013BB08(s32, s32);                          /* extern */
? func_8013BB64(s32);                               /* extern */
? func_8013BBA8(s32);                               /* extern */
? func_8013BBEC(s32, s32);                          /* extern */
? func_8013BC30(s32);                               /* extern */
? func_8013BC74(s32);                               /* extern */
? func_8013BCB8(s32);                               /* extern */
? func_8013BCFC(s32);                               /* extern */
? func_8013BD40(s32, ?);                            /* extern */
? func_8013BDEC(s32, ?);                            /* extern */
extern s32 D_8014F990;

void func_8013B1F0(u32 arg0, s32 arg1, s32 arg2, ? arg3) {
    D_8014F990 = 0;
    switch (arg0) {                                 /* switch 1 */
    case 0:                                         /* switch 1 */
        switch (arg1) {                             /* switch 2 */
        case 29:                                    /* switch 2 */
        case 30:                                    /* switch 2 */
        case 40:                                    /* switch 2 */
            func_8013B98C(arg2);
            return;
        case 32:                                    /* switch 2 */
            func_8013BA3C(arg2);
            return;
        case 34:                                    /* switch 2 */
            func_8013BA80(arg2);
            return;
        default:                                    /* switch 2 */
            func_8013BDEC(arg2, arg3);
            return;
        }
        break;
    case 1:                                         /* switch 1 */
        switch (arg1) {                             /* switch 3; irregular */
        case 32:                                    /* switch 3 */
            func_8013BA3C(arg2);
            return;
        case 34:                                    /* switch 3 */
            func_8013BA80(arg2);
            return;
        default:                                    /* switch 3 */
            func_8013BDEC(arg2, arg3);
            return;
        }
        break;
    case 2:                                         /* switch 1 */
        switch (arg1) {                             /* switch 4 */
        case 0x14:                                  /* switch 4 */
            func_8013BDEC(arg2, arg3);
            return;
        case 0x1:                                   /* switch 4 */
        case 0x22:                                  /* switch 4 */
        case 0x35:                                  /* switch 4 */
        case 0x3D:                                  /* switch 4 */
        case 0x44:                                  /* switch 4 */
        case 0x4A:                                  /* switch 4 */
        case 0x4F:                                  /* switch 4 */
        case 0x51:                                  /* switch 4 */
            func_8013BD40(arg2, arg3);
            return;
        case 0x4C:                                  /* switch 4 */
            func_8013BDEC(arg2, arg3);
            return;
        case 0x4E:                                  /* switch 4 */
            func_8013B98C(arg2);
            return;
        default:                                    /* switch 1 */
        default:                                    /* switch 4 */
            return;
        }
        break;
    case 3:                                         /* switch 1 */
        func_8013BD40(arg2, arg3);
        return;
    case 4:                                         /* switch 1 */
        if ((arg1 == 1) && (arg2 == 9)) {
            D_8014F990 = 1;
            func_80062CD0(0x6638);
            return;
        }
        func_8013BCFC(arg2);
        return;
    case 5:                                         /* switch 1 */
        if (arg1 < 3) {
            func_8013BC74(arg2);
            return;
        }
        func_8013BCB8(arg2);
        return;
    case 6:                                         /* switch 1 */
        func_8013BD40(arg2, arg3);
        return;
    case 7:                                         /* switch 1 */
        func_8013BD40(arg2, arg3);
        return;
    case 8:                                         /* switch 1 */
        switch (arg1) {                             /* switch 5 */
        case 0x16:                                  /* switch 5 */
        case 0x17:                                  /* switch 5 */
        case 0x18:                                  /* switch 5 */
            func_8013BB08(2, 2);
            return;
        case 0x1F:                                  /* switch 5 */
            func_8013BB08(3, 4);
            return;
        case 0x29:                                  /* switch 5 */
        case 0x2A:                                  /* switch 5 */
            func_8013BB08(4, 7);
            return;
        case 0x32:                                  /* switch 5 */
        case 0x33:                                  /* switch 5 */
            func_8013BB08(5, 8);
            return;
        case 0x3C:                                  /* switch 5 */
            D_8014F990 = 1;
            func_80062CD0(0x6449);
            return;
        case 0x3D:                                  /* switch 5 */
            func_8013BB08(6, 9);
            return;
        case 0x5A:                                  /* switch 5 */
            D_8014F990 = 1;
            func_80062CD0(0x6638);
            return;
        default:                                    /* switch 5 */
            func_8013BD40(arg2, arg3);
            return;
        }
        break;
    case 9:                                         /* switch 1 */
        func_8013BD40(arg2, arg3);
        return;
    case 10:                                        /* switch 1 */
        switch (arg1) {                             /* switch 6 */
        case 0x20:                                  /* switch 6 */
            func_8013BA3C(arg2);
            return;
        case 0x22:                                  /* switch 6 */
            func_8013BA80(arg2);
            return;
        case 0x25:                                  /* switch 6 */
        case 0x29:                                  /* switch 6 */
        case 0x2A:                                  /* switch 6 */
        case 0x2B:                                  /* switch 6 */
        case 0x2C:                                  /* switch 6 */
            func_8013BD40(arg2, arg3);
            return;
        case 0x2E:                                  /* switch 6 */
            func_8013BC74(arg2);
            return;
        case 0x32:                                  /* switch 6 */
            func_8013BCB8(arg2);
            return;
        case 0x30:                                  /* switch 6 */
        case 0x31:                                  /* switch 6 */
            func_8013BC30(arg2);
            return;
        case 0x33:                                  /* switch 6 */
        case 0x34:                                  /* switch 6 */
        case 0x35:                                  /* switch 6 */
        case 0x36:                                  /* switch 6 */
        case 0x37:                                  /* switch 6 */
        case 0x38:                                  /* switch 6 */
        case 0x39:                                  /* switch 6 */
        case 0x3A:                                  /* switch 6 */
        case 0x3B:                                  /* switch 6 */
        case 0x3C:                                  /* switch 6 */
        case 0x3D:                                  /* switch 6 */
            func_8013BB08(arg2, arg1 - 0x33);
            return;
        default:                                    /* switch 6 */
            func_8013BDEC(arg2, arg3);
            return;
        }
        break;
    case 11:                                        /* switch 1 */
        if ((arg1 == 0x1E) || (arg1 == 0x1D) || (arg1 == 0x28)) {
            D_8014F990 = 1;
            func_80062CD0(0x6BD8);
            return;
        }
        func_8013BDEC(arg2, arg3);
        return;
    case 12:                                        /* switch 1 */
        func_8013BDEC(arg2, arg3);
        return;
    case 13:                                        /* switch 1 */
        switch (arg1) {                             /* switch 7 */
        case 0x20:                                  /* switch 7 */
            func_8013BA3C(arg2);
            return;
        case 0x22:                                  /* switch 7 */
            func_8013BA80(arg2);
            return;
        case 0x26:                                  /* switch 7 */
        case 0x27:                                  /* switch 7 */
        case 0x28:                                  /* switch 7 */
        case 0x29:                                  /* switch 7 */
        case 0x2B:                                  /* switch 7 */
            func_8013BD40(arg2, arg3);
            return;
        case 0x2A:                                  /* switch 7 */
            if ((arg2 == 7) || (arg2 == 8) || (arg2 == 9)) {
                func_8013BDEC(arg2, arg3);
                return;
            }
            func_8013BD40(arg2, arg3);
            return;
        case 0x2C:                                  /* switch 7 */
            func_8013BC74(arg2);
            return;
        case 0x2D:                                  /* switch 7 */
            func_8013BCB8(arg2);
            return;
        case 0x2E:                                  /* switch 7 */
        case 0x2F:                                  /* switch 7 */
        case 0x30:                                  /* switch 7 */
        case 0x31:                                  /* switch 7 */
        case 0x32:                                  /* switch 7 */
        case 0x33:                                  /* switch 7 */
        case 0x34:                                  /* switch 7 */
        case 0x35:                                  /* switch 7 */
        case 0x36:                                  /* switch 7 */
        case 0x37:                                  /* switch 7 */
        case 0x38:                                  /* switch 7 */
            func_8013BB08(arg2, arg1 - 0x2E);
            return;
        default:                                    /* switch 7 */
            func_8013BDEC(arg2, arg3);
            return;
        }
        break;
    case 14:                                        /* switch 1 */
        if (arg1 == 0xB) {
            func_8013BB64(arg2);
            return;
        }
        if (arg1 == 0xC) {
            func_8013BBA8(arg2);
            return;
        }
        if (arg1 == 0xD) {
            func_8013BBEC(arg2, arg1);
            return;
        }
        func_8013BB08(arg2, arg1);
        return;
    case 15:                                        /* switch 1 */
        func_8013BD40(arg2, arg3);
        return;
    case 16:                                        /* switch 1 */
        func_8013BB08(arg2, arg1);
        return;
    case 18:                                        /* switch 1 */
        switch (arg1) {                             /* switch 8 */
        case 5:                                     /* switch 8 */
            func_8013BB08(arg2, 6);
            return;
        case 6:                                     /* switch 8 */
            func_8013BB08(arg2, 7);
            return;
        case 7:                                     /* switch 8 */
            func_8013BB08(arg2, 8);
            return;
        case 8:                                     /* switch 8 */
            func_8013BB08(arg2, 9);
            return;
        case 9:                                     /* switch 8 */
            func_8013BB08(arg2, 0xA);
            return;
        default:                                    /* switch 8 */
            func_8013BD40(arg2, arg3);
            return;
        }
        break;
    case 19:                                        /* switch 1 */
        func_8013BAC4(arg2);
        return;
    case 20:                                        /* switch 1 */
        switch (arg1) {                             /* switch 9 */
        case 6:                                     /* switch 9 */
        case 7:                                     /* switch 9 */
        case 29:                                    /* switch 9 */
            func_8013BB08(arg2, 1);
            return;
        case 10:                                    /* switch 9 */
        case 11:                                    /* switch 9 */
        case 30:                                    /* switch 9 */
            func_8013BB08(arg2, 2);
            return;
        case 18:                                    /* switch 9 */
        case 19:                                    /* switch 9 */
        case 32:                                    /* switch 9 */
        case 33:                                    /* switch 9 */
            func_8013BB08(arg2, 4);
            return;
        default:                                    /* switch 9 */
            func_8013BD40(arg2, arg3);
            return;
        }
        break;
    case 21:                                        /* switch 1 */
        if ((arg2 == 0xC) || (arg2 == 0xB)) {
            func_8013BD40(arg2, arg3);
            return;
        }
        switch (arg1) {                             /* switch 10 */
        case 0:                                     /* switch 10 */
        case 5:                                     /* switch 10 */
        case 6:                                     /* switch 10 */
        case 7:                                     /* switch 10 */
        case 8:                                     /* switch 10 */
        case 9:                                     /* switch 10 */
        case 10:                                    /* switch 10 */
        case 11:                                    /* switch 10 */
            func_8013BD40(arg2, arg3);
            return;
        case 12:                                    /* switch 10 */
            D_8014F990 = 1;
            func_80062CD0(0x5D6E);
            return;
        default:                                    /* switch 10 */
            func_8013BDEC(arg2, arg3);
            break;
        }
        break;
    }
}
