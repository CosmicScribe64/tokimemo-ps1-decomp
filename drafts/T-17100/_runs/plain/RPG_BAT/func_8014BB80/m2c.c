? func_8004F984(?, s16, s16);                       /* extern */
extern s32 D_800E7208;
extern u8 D_800E7312;
extern s8 D_800E7313;
extern s16 D_8012103C;
extern s32 D_8015EBA8;

void func_8014BB80(void) {
    ? sp46;
    ? sp40;
    ? sp3A;
    ? sp34;
    s32 var_v1;

    var_v1 = D_8015EE50.unk_00;
    sp34.unk0 = (s32) D_8015E720.unk0;
    *(s32 *)((unsigned char *)(&sp34) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015E720) + (4));
    *(s32 *)((unsigned char *)(&sp34) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015E720) + (8));
    *(s32 *)((unsigned char *)(&sp34) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015E720) + (12));
    *(s32 *)((unsigned char *)(&sp34) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8015E720) + (16));
    *(s32 *)((unsigned char *)(&sp34) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_8015E720) + (20));
    if ((var_v1 != 3) && !(D_800E7312 & 1)) {
        func_8006BA40();
        var_v1 = D_8015EE50.unk_00;
    }
    if (!(D_8015ED98 & 0x10000) && (var_v1 != 4) && (var_v1 != 5)) {
        var_v1 = 6;
    }
    D_8015EE50.unk_00 = var_v1;
    switch (var_v1) {                               /* switch 1 */
    case 0:                                         /* switch 1 */
        func_8013E7C0(0x25, 0x31, 5, 1);
        if (D_8015EBA8 == 1) {
            func_8004F870(0, 3, &sp34, &sp3A, &sp40, &sp46);
        } else {
            func_8004F870(0, 2, &sp34, &sp3A, &sp40, &sp46);
        }
        func_8014EB58(1);
    default:                                        /* switch 1 */
        break;
    case 1:                                         /* switch 1 */
        if (func_8013E90C(0x25) != 0) {
            func_8014EB58(1);
        }
        break;
    case 2:                                         /* switch 1 */
        func_8013E7C0(0x25, 2, 1, 1);
        if (D_8015EBA8 == 1) {
            func_8013E7C0(0x23, 0xFF, 1, 1);
        } else {
            func_8013E7C0(0x23, 0xFF, 1, 0);
        }
        func_8014EB58(1);
        break;
    case 3:                                         /* switch 1 */
        func_8004F984(0, D_8011ECF6, D_8011ECFA);
        func_8004FC10(0);
        func_8013E7C0(0x22, 0xFF, 1, 1);
        switch (D_800E7313) {                       /* switch 2; irregular */
        case 0:                                     /* switch 2 */
            func_8013E7C0(0x22, 5, 1, 1);
            break;
        case 1:                                     /* switch 2 */
            func_8013E7C0(0x22, 6, 1, 1);
            break;
        case 2:                                     /* switch 2 */
            func_8013E7C0(0x22, 7, 1, 1);
            break;
        case -1:                                    /* switch 2 */
            func_8013E7C0(0x22, 0xFF, 1, 0);
            break;
        }
        if ((D_800E7313 != -1) && (D_800E7208 & 0x20)) {
            func_8013E7C0(0x22, 0xFF, 1, 0);
            func_8013E7C0(0x23, 0xFF, 1, 0);
            switch (D_800E7313) {                   /* switch 3; irregular */
            case 0:                                 /* switch 3 */
                D_8015ED98 ^= 0x01000000;
                break;
            case 1:                                 /* switch 3 */
                D_8015ED98 ^= 0x02000000;
                break;
            case 2:                                 /* switch 3 */
                D_8015ED98 ^= 0x04000000;
                break;
            }
            func_8014EB58(1);
        }
        break;
    case 4:                                         /* switch 1 */
        func_8013E7C0(0x25, 0x31, 0x21, 1);
        D_8012103C = 6;
        func_8013E7C0(0x22, 0xFF, 1, 0);
        func_8013E7C0(0x23, 0xFF, 1, 0);
        func_8014EB58(1);
        break;
    case 5:                                         /* switch 1 */
        if (func_8013E90C(0x25) != 0) {
            func_8013E7C0(0x25, 0xFF, 1, 0);
            func_8014EB58(1);
        }
        break;
    case 6:                                         /* switch 1 */
        D_8015ED98 &= 0xFFFEFFFF;
        func_8013E7C0(0x22, 0xFF, 1, 0);
        func_8013E7C0(0x23, 0xFF, 1, 0);
        func_8013E7C0(0x25, 0xFF, 1, 0);
        D_8015EE50.unk_00 = 0;
        break;
    }
}
