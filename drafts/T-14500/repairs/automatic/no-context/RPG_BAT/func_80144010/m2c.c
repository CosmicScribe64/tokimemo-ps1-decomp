s32 func_801447B8(?, s16, ?, ?);                    /* extern */
extern ? D_800E6280;
extern s16 D_800E637E;
extern s32 D_8015EB98;
extern u32 D_8015EB9C;
extern s32 D_8015EBA0;
extern s32 D_8015EBA4;
extern s32 D_8015EBA8;
extern s32 D_8015EBC0;
extern s32 D_8015EBC4;
extern s32 D_8015EBC8;
extern s32 D_8015EBD8;
extern s32 D_8015EC30;
extern s32 D_8015EC34;
extern s32 D_8015ED94;
extern s32 D_8015ED98;
extern s32 D_8015ED9C;
extern s32 D_8015EDA0;
extern s32 D_8015EDC0;
extern s32 D_8015EDC4;
extern s32 D_8015EDCC;
extern s32 D_8015EDEC;
extern s32 D_8015EDF8;
extern s32 D_8015EE18;

void func_80144010(void) {
    ? *var_v1;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a3;
    s32 var_v0;
    s32 var_v0_2;
    u16 temp_t9;
    u32 var_a0_2;

    var_a0 = 0;
    var_a3 = 0xFF;
    var_v1 = &D_800E6280;
    var_v0 = 0;
    do {
        if (((u32) (*(s32 *)((unsigned char *)var_v1 + (1644)) << 5) >> 0x1E) == 3) {
            var_a3 = var_v0;
        }
        temp_t9 = (u16) *(s32 *)((unsigned char *)var_v1 + (1644));
        var_v0 += 1;
        var_v1 += 4;
        var_a0 += temp_t9;
    } while (var_v0 != 0xC);
    if (var_a3 == 0xB) {
        var_a3 = 0xFF;
        if (var_a0 == 0) {
            var_a3 = 0xB;
        }
    }
    D_8015EBA0 = var_a3;
    if (var_a3 == 0xFF) {
        D_8015EBA4 = 0;
        D_8015EBA8 = 0;
        D_8015EC30 = 0;
    } else {
        D_8015EBA4 = 1;
        D_8015EBA8 = 1;
        D_8015EC30 = 0x64;
    }
    temp_v1 = D_8015EB98;
    D_8015EC34 = 0;
    if (temp_v1 & 0x800F0000) {
        D_8015EBA4 = 0;
        D_8015EBA8 = 0;
    }
    temp_v0 = temp_v1 & 0xF0000;
    if ((temp_v0 == 0) && (D_8015EB9C == 0xD)) {
        D_8015EB9C = 9;
    }
    var_a0_2 = D_8015EB9C;
    if ((temp_v0 == 0) && (var_a0_2 == 0xE)) {
        var_a0_2 = 9;
    }
    if ((temp_v0 != 0) && (var_a0_2 != 0xD)) {
        var_a0_2 = 0xE;
    }
    if (temp_v1 & 0x80000000) {
        var_a0_2 = 2;
    }
    if (temp_v1 & 0x08000000) {
        var_a0_2 = 0xE;
    }
    if (temp_v1 != 0x08000000) {
        D_8015ED9C = 0xFFFF;
    } else {
        D_8015ED9C = 0;
        D_8015ED98 = 0x1000;
    }
    if (D_800E637E == 0) {
        var_v0_2 = 0xA;
    } else {
        var_v0_2 = D_800E637E * 0xA;
    }
    D_8015EDA0 = var_v0_2;
    D_8015ED94 = var_v0_2;
    D_8015EB9C = var_a0_2;
    switch (var_a0_2) {                             /* switch 1 */
    case 0:                                         /* switch 1 */
        D_8015EDEC = 0x100;
        break;
    case 1:                                         /* switch 1 */
        D_8015EDEC = 0x63;
        break;
    case 2:                                         /* switch 1 */
        D_8015EDEC = 0x138;
        break;
    case 3:                                         /* switch 1 */
        D_8015EDEC = 0x130;
        break;
    case 4:                                         /* switch 1 */
        D_8015EDEC = 0xBD;
        break;
    case 5:                                         /* switch 1 */
        D_8015EDEC = 0x12A;
        break;
    case 6:                                         /* switch 1 */
        D_8015EDEC = 0x756;
        break;
    case 7:                                         /* switch 1 */
        D_8015EDEC = 0x115;
        break;
    case 8:                                         /* switch 1 */
        D_8015EDEC = 0x16D;
        break;
    case 9:                                         /* switch 1 */
        D_8015EDEC = 0xAD;
        break;
    case 10:                                        /* switch 1 */
        D_8015EDEC = 0x109;
        break;
    case 13:                                        /* switch 1 */
        D_8015EDEC = 0x23D;
        break;
    case 14:                                        /* switch 1 */
        D_8015EDEC = 0x7FFFFFFF;
        break;
    }
    D_8015EDF8 = D_8015EDEC;
    D_8015EDC4 = temp_v1;
    if (temp_v1 == 0x08000000) {
        D_8015EDC4 = 0x08000002;
    }
    switch (temp_v1) {                              /* switch 2; irregular */
    case 0x10000:                                   /* switch 2 */
    case 0x20000:                                   /* switch 2 */
    case 0x40000:                                   /* switch 2 */
        D_8015EDC0 = 1;
        D_8015EDCC = 1;
        break;
    case 0x8000000:                                 /* switch 2 */
        D_8015EDC0 = 0x1388;
        D_8015EDCC = 0x1388;
        break;
    case 0x80000000:                                /* switch 2 */
        D_8015EDC0 = 0x7FFFFFFF;
        D_8015EDCC = 0x7FFFFFFF;
        D_8015EBD8 = 0x3E8;
        break;
    case 0x100000:                                  /* switch 2 */
        D_8015EDC0 = 0x10000;
        D_8015EDCC = 0x10000;
        if (func_801447B8(0x7FFFFFFF, D_800E637E, 0x08000000, 0x80000000) != 0) {
            D_8015EBD8 = 0x3E8;
            D_8015EBC0 = 0x1388;
            D_8015EBC4 = 0x1388;
            D_8015EBC8 = 0x1388;
        } else {
            D_8015EBD8 = 0x5DC;
            D_8015EBC0 = 0x270F;
            D_8015EBC4 = 0x270F;
            D_8015EBC8 = 0x270F;
        }
        break;
    }
    switch (temp_v1) {                              /* switch 3; irregular */
    default:                                        /* switch 3 */
        D_8015EE18 = 0x7FFFFFFF;
        return;
    case 0x10000:                                   /* switch 3 */
        D_8015EE18 = 0x10000075;
        return;
    case 0x20000:                                   /* switch 3 */
        D_8015EE18 = 0x10000232;
        return;
    case 0x40000:                                   /* switch 3 */
        D_8015EE18 = 0x1000012A;
        return;
    }
}
