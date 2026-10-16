? func_80048514(s32, s32, ?);                       /* extern */

void func_80141060(void) {
    DisplayActor44 *sp3C;
    DisplayActor44 *temp_v0;
    DisplayActor44 *var_v0;
    DisplayActor44 *var_v0_2;
    DisplayActor44 *var_v0_3;
    DisplayActor44 *var_v0_4;
    DisplayActor44 *var_v0_5;
    DisplayActor44 *var_v0_6;
    DisplayActor44 *var_v0_7;
    DisplayActor44 *var_v0_8;
    DisplayActor44 *var_v0_9;
    s32 var_a0;
    s32 var_at;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_3;

    var_v0 = D_8011ECD0;
    var_s1 = 0;
    do {
        sp3C = var_v0;
        func_80048F64(var_s1 + 0x40);
        if (var_s1 < 0x3F) {
            *(s8 *)((unsigned char *)var_v0 + (4353)) = 3;
        } else if (var_s1 < 0x40) {
            *(s8 *)((unsigned char *)var_v0 + (4353)) = 9;
        } else if (var_s1 < 0x48) {
            *(s8 *)((unsigned char *)var_v0 + (4353)) = 4;
        } else {
            *(s8 *)((unsigned char *)var_v0 + (4353)) = 5;
        }
        var_s1 += 1;
        *(s32 *)((unsigned char *)var_v0 + (4408)) = 0x41000000;
        var_v0 += 0x44;
        *(s8 *)((unsigned char *)var_v0 + (4287)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (4286)) = 1;
        *(s8 *)((unsigned char *)var_v0 + (4290)) = 1;
        *(s8 *)((unsigned char *)var_v0 + (4291)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (4322)) = -0xA0;
        *(s16 *)((unsigned char *)var_v0 + (4326)) = -0x78;
        *(s8 *)((unsigned char *)var_v0 + (4289)) = 0x80;
        *(s8 *)((unsigned char *)var_v0 + (4351)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (4288)) = 8;
        *(s32 *)((unsigned char *)var_v0 + (4336)) = (s32) D_8015EE8C;
        *(s32 *)((unsigned char *)var_v0 + (4300)) = (s32) D_8015EEBC;
        *(s32 *)((unsigned char *)var_v0 + (4296)) = (s32) D_8015EEA4;
    } while (var_s1 < 0x60);
    var_v0_2 = D_8011ECD0;
    var_s1_2 = 0;
    do {
        var_s1_2 += 1;
        var_v0_2 += 0x44;
        *(s32 *)((unsigned char *)var_v0_2 + (4296)) = (s32) D_8015EEA4;
        *(s32 *)((unsigned char *)var_v0_2 + (4300)) = (s32) D_8015EEBC;
        *(s32 *)((unsigned char *)var_v0_2 + (4336)) = (s32) D_8015EE8C;
    } while (var_s1_2 < 0x48);
    func_80048514(0x15, D_8015EE8C->unk0, 1);
    func_80048514(0x16, *(s32 *)((unsigned char *)D_8015EE8C + (4)), 1);
    func_80048514(0x17, *(s32 *)((unsigned char *)D_8015EE8C + (8)), 1);
    func_80048514(0x18, *(s32 *)((unsigned char *)D_8015EE8C + (12)), 1);
    func_80048514(0x19, *(s32 *)((unsigned char *)D_8015EE8C + (16)), 1);
    func_80048514(0x35, D_8015EE98->unk0, 1);
    func_80048514(0x39, *(s32 *)((unsigned char *)D_8015EE98 + (4)), 1);
    switch (D_8015EB98) {                           /* switch 1; irregular */
    case 0x10000:                                   /* switch 1 */
        func_80048514(0xB, *(s32 *)((unsigned char *)D_8015EE94 + (4)), 1);
        func_80048514(0xC, *(s32 *)((unsigned char *)D_8015EE94 + (8)), 1);
        /* fallthrough */
    case 0x20000:                                   /* switch 1 */
    case 0x40000:                                   /* switch 1 */
        func_80048514(0xD, D_8015EE94->unk0, 1);
        func_80048514(0x2B, D_8015EE90->unk0, 1);
        func_80048514(0x2C, *(s32 *)((unsigned char *)D_8015EE90 + (4)), 1);
        func_80048514(0x2D, *(s32 *)((unsigned char *)D_8015EE90 + (8)), 1);
        if (D_8015EB9C == 0xD) {
            func_80048514(5, D_8015EEA0->unk0, 1);
            func_80048514(6, *(s32 *)((unsigned char *)D_8015EEA0 + (4)), 1);
            func_80048514(7, *(s32 *)((unsigned char *)D_8015EEA0 + (8)), 1);
        }
        break;
    case 0x80000000:                                /* switch 1 */
        func_80048514(0xB, D_8015EEA0->unk0, 1);
        func_80048514(0xC, *(s32 *)((unsigned char *)D_8015EEA0 + (4)), 1);
        func_80048514(0xD, *(s32 *)((unsigned char *)D_8015EEA0 + (8)), 1);
        func_80048514(0xE, *(s32 *)((unsigned char *)D_8015EEA0 + (12)), 1);
        func_80048514(0xF, *(s32 *)((unsigned char *)D_8015EEA0 + (16)), 1);
        func_80048514(0x2B, *(s32 *)((unsigned char *)D_8015EEA0 + (20)), 1);
        func_80048514(0x2D, D_8015EE90->unk0, 1);
        func_80048514(0x2E, *(s32 *)((unsigned char *)D_8015EE90 + (4)), 1);
        func_80048514(0x2F, *(s32 *)((unsigned char *)D_8015EE90 + (8)), 1);
        break;
    case 0x100000:                                  /* switch 1 */
        func_80048514(0xB, *(s32 *)((unsigned char *)D_8015EE90 + (24)), 1);
        switch (D_8015EB9C) {                       /* switch 2; irregular */
        case 0:                                     /* switch 2 */
            var_s1_2 = 1;
        default:                                    /* switch 2 */
block_37:
            var_s0 = 0;
            if (var_s1_2 > 0) {
                var_v0_3 = NULL;
                do {
                    sp3C = var_v0_3;
                    func_80048514(var_s0 + 0x25, *(D_8015EEA0 + var_v0_3), 1);
                    var_s0 += 1;
                    var_v0_3 = (DisplayActor44 *) &var_v0_3->unk_04;
                } while (var_s0 != var_s1_2);
            }
            break;
        case 1:                                     /* switch 2 */
            var_s1_2 = 2;
            goto block_37;
        case 2:                                     /* switch 2 */
            var_s1_2 = 4;
            goto block_37;
        case 3:                                     /* switch 2 */
        case 13:                                    /* switch 2 */
block_36:
            var_s1_2 = 3;
            goto block_37;
        case 4:                                     /* switch 2 */
            var_s1_2 = 2;
            goto block_37;
        case 5:                                     /* switch 2 */
            var_s1_2 = 2;
            goto block_37;
        case 6:                                     /* switch 2 */
            goto block_36;
        case 7:                                     /* switch 2 */
            var_s1_2 = 2;
            goto block_37;
        case 8:                                     /* switch 2 */
            var_s1_2 = 2;
            goto block_37;
        case 9:                                     /* switch 2 */
            var_s1_2 = 1;
            goto block_37;
        case 10:                                    /* switch 2 */
            var_s1_2 = 2;
            goto block_37;
        }
        /* fallthrough */
    case 0x8000000:                                 /* switch 1 */
        if (D_8015EBA4 == 1) {
            var_s0_2 = 0;
            switch (D_8015EBA0) {                   /* switch 3 */
            case 0:                                 /* switch 3 */
                var_s1_2 = 4;
                break;
            case 1:                                 /* switch 3 */
                var_s1_2 = 4;
                break;
            case 2:                                 /* switch 3 */
                var_s1_2 = 4;
                break;
            case 3:                                 /* switch 3 */
                var_s1_2 = 5;
                break;
            case 4:                                 /* switch 3 */
                var_s1_2 = 5;
                break;
            case 5:                                 /* switch 3 */
                var_s1_2 = 4;
                break;
            case 6:                                 /* switch 3 */
                var_s1_2 = 3;
                break;
            case 7:                                 /* switch 3 */
                var_s1_2 = 3;
                break;
            case 8:                                 /* switch 3 */
                var_s1_2 = 5;
                break;
            case 9:                                 /* switch 3 */
                var_s1_2 = 5;
                break;
            case 10:                                /* switch 3 */
                var_s1_2 = 3;
                break;
            case 11:                                /* switch 3 */
                var_s1_2 = 6;
                break;
            }
            var_at = var_s1_2 > 0;
            do {
                if (var_at != 0) {
                    func_80048514(var_s0_2 + 5, *(D_8015EE9C + (var_s0_2 * 4)), 1);
                } else {
                    func_80041168(var_s0_2 + 5);
                }
                var_s0_2 += 1;
                var_at = var_s0_2 < var_s1_2;
            } while (var_s0_2 != 6);
        }
        func_80048514(0x2C, D_8015EE90->unk0, 1);
        func_80048514(0xC, *(s32 *)((unsigned char *)D_8015EE90 + (4)), 1);
        func_80048514(0xD, *(s32 *)((unsigned char *)D_8015EE90 + (8)), 1);
        func_80048514(0xE, *(s32 *)((unsigned char *)D_8015EE90 + (12)), 1);
        func_80048514(0xF, *(s32 *)((unsigned char *)D_8015EE90 + (16)), 1);
        func_80048514(0x2B, *(s32 *)((unsigned char *)D_8015EE90 + (20)), 1);
        break;
    }
    var_s1_3 = 0;
    var_a0 = 0;
    do {
        func_8013E8B8(var_a0, 0x27, 0);
        var_s1_3 += 1;
        var_a0 = var_s1_3;
    } while (var_s1_3 < 0x30);
    if (D_8015EB9C != 0xE) {
        var_v0_4 = D_8011ECD0 + 0x7B4;
        do {
            var_v0_4 += 0x44;
            *(s32 *)((unsigned char *)var_v0_4 + (6472)) = (s32) D_8015EEB8;
            *(s32 *)((unsigned char *)var_v0_4 + (6476)) = (s32) D_8015EED0;
            *(s32 *)((unsigned char *)var_v0_4 + (6512)) = (s32) D_8015EEA0;
            *(s8 *)((unsigned char *)var_v0_4 + (6527)) = 0x1C;
            *(s8 *)((unsigned char *)var_v0_4 + (6461)) = 5;
        } while (var_v0_4 != (D_8011ECD0 + 0x83C));
    }
    if (D_8015EBA4 == 1) {
        var_v0_5 = D_8011ECD0 + 0xAA0;
        do {
            var_v0_5 += 0x110;
            *(s32 *)((unsigned char *)var_v0_5 + (6336)) = (s32) D_8015EEB4;
            *(s32 *)((unsigned char *)var_v0_5 + (6340)) = (s32) D_8015EECC;
            *(s32 *)((unsigned char *)var_v0_5 + (6376)) = (s32) D_8015EE9C;
            *(s8 *)((unsigned char *)var_v0_5 + (6391)) = 0x1D;
            *(s32 *)((unsigned char *)var_v0_5 + (6404)) = (s32) D_8015EEB4;
            *(s32 *)((unsigned char *)var_v0_5 + (6408)) = (s32) D_8015EECC;
            *(s32 *)((unsigned char *)var_v0_5 + (6444)) = (s32) D_8015EE9C;
            *(s8 *)((unsigned char *)var_v0_5 + (6459)) = 0x1D;
            *(s32 *)((unsigned char *)var_v0_5 + (6472)) = (s32) D_8015EEB4;
            *(s32 *)((unsigned char *)var_v0_5 + (6476)) = (s32) D_8015EECC;
            *(s32 *)((unsigned char *)var_v0_5 + (6512)) = (s32) D_8015EE9C;
            *(s8 *)((unsigned char *)var_v0_5 + (6527)) = 0x1D;
            *(s32 *)((unsigned char *)var_v0_5 + (6268)) = (s32) D_8015EEB4;
            *(s32 *)((unsigned char *)var_v0_5 + (6272)) = (s32) D_8015EECC;
            *(s32 *)((unsigned char *)var_v0_5 + (6308)) = (s32) D_8015EE9C;
            *(s8 *)((unsigned char *)var_v0_5 + (6323)) = 0x1D;
        } while (var_v0_5 != (D_8011ECD0 + 0xCC0));
    }
    if (D_8015EB98 & 0xF0000) {
        var_v0_6 = D_8011ECD0 + 0xAA0;
        do {
            var_v0_6 += 0x110;
            *(s32 *)((unsigned char *)var_v0_6 + (6336)) = (s32) D_8015EEAC;
            *(s32 *)((unsigned char *)var_v0_6 + (6340)) = (s32) D_8015EEC4;
            *(s32 *)((unsigned char *)var_v0_6 + (6376)) = (s32) D_8015EE94;
            *(s8 *)((unsigned char *)var_v0_6 + (6391)) = 0x18;
            *(s32 *)((unsigned char *)var_v0_6 + (6404)) = (s32) D_8015EEAC;
            *(s32 *)((unsigned char *)var_v0_6 + (6408)) = (s32) D_8015EEC4;
            *(s32 *)((unsigned char *)var_v0_6 + (6444)) = (s32) D_8015EE94;
            *(s8 *)((unsigned char *)var_v0_6 + (6459)) = 0x18;
            *(s32 *)((unsigned char *)var_v0_6 + (6472)) = (s32) D_8015EEAC;
            *(s32 *)((unsigned char *)var_v0_6 + (6476)) = (s32) D_8015EEC4;
            *(s32 *)((unsigned char *)var_v0_6 + (6512)) = (s32) D_8015EE94;
            *(s8 *)((unsigned char *)var_v0_6 + (6527)) = 0x18;
            *(s32 *)((unsigned char *)var_v0_6 + (6268)) = (s32) D_8015EEAC;
            *(s32 *)((unsigned char *)var_v0_6 + (6272)) = (s32) D_8015EEC4;
            *(s32 *)((unsigned char *)var_v0_6 + (6308)) = (s32) D_8015EE94;
            *(s8 *)((unsigned char *)var_v0_6 + (6323)) = 0x18;
        } while (var_v0_6 != (D_8011ECD0 + 0xCC0));
    }
    if (D_8015EB98 & 0x80000000) {
        var_v0_7 = D_8011ECD0 + 0xAA0;
        do {
            var_v0_7 += 0x110;
            *(s32 *)((unsigned char *)var_v0_7 + (6336)) = (s32) D_8015EEA8;
            *(s32 *)((unsigned char *)var_v0_7 + (6340)) = (s32) D_8015EEC0;
            *(s32 *)((unsigned char *)var_v0_7 + (6376)) = (s32) D_8015EE90;
            *(s8 *)((unsigned char *)var_v0_7 + (6391)) = 0x12;
            *(s32 *)((unsigned char *)var_v0_7 + (6404)) = (s32) D_8015EEA8;
            *(s32 *)((unsigned char *)var_v0_7 + (6408)) = (s32) D_8015EEC0;
            *(s32 *)((unsigned char *)var_v0_7 + (6444)) = (s32) D_8015EE90;
            *(s8 *)((unsigned char *)var_v0_7 + (6459)) = 0x12;
            *(s32 *)((unsigned char *)var_v0_7 + (6472)) = (s32) D_8015EEA8;
            *(s32 *)((unsigned char *)var_v0_7 + (6476)) = (s32) D_8015EEC0;
            *(s32 *)((unsigned char *)var_v0_7 + (6512)) = (s32) D_8015EE90;
            *(s8 *)((unsigned char *)var_v0_7 + (6527)) = 0x12;
            *(s32 *)((unsigned char *)var_v0_7 + (6268)) = (s32) D_8015EEA8;
            *(s32 *)((unsigned char *)var_v0_7 + (6272)) = (s32) D_8015EEC0;
            *(s32 *)((unsigned char *)var_v0_7 + (6308)) = (s32) D_8015EE90;
            *(s8 *)((unsigned char *)var_v0_7 + (6323)) = 0x12;
        } while (var_v0_7 != (D_8011ECD0 + 0xCC0));
    }
    var_v0_8 = D_8011ECD0 + 0xCC0;
    do {
        var_v0_8 += 0x44;
        *(s32 *)((unsigned char *)var_v0_8 + (6472)) = (s32) D_8015EEB0;
        *(s32 *)((unsigned char *)var_v0_8 + (6476)) = (s32) D_8015EEC8;
        *(s32 *)((unsigned char *)var_v0_8 + (6512)) = (s32) D_8015EE98;
        *(s8 *)((unsigned char *)var_v0_8 + (6527)) = 0x1E;
    } while ((u32) var_v0_8 < (u32) D_8011ECD0);
    if (D_8015EBA4 == 1) {
        *(s32 *)((unsigned char *)D_8011ECD0 + (9940)) = (s32) D_8015EEB4;
        *(s32 *)((unsigned char *)D_8011ECD0 + (9944)) = (s32) D_8015EECC;
        *(s32 *)((unsigned char *)D_8011ECD0 + (9980)) = (s32) D_8015EE9C;
        *(s8 *)((unsigned char *)D_8011ECD0 + (9995)) = 0x1D;
        *(s32 *)((unsigned char *)D_8011ECD0 + (10008)) = (s32) D_8015EEB4;
        *(s32 *)((unsigned char *)D_8011ECD0 + (10012)) = (s32) D_8015EECC;
        *(s32 *)((unsigned char *)D_8011ECD0 + (10048)) = (s32) D_8015EE9C;
        *(s8 *)((unsigned char *)D_8011ECD0 + (10063)) = 0x1D;
        temp_v0 = &D_8011ECD0[0x34];
        *(s32 *)((unsigned char *)temp_v0 + (6608)) = (s32) D_8015EEB4;
        *(s32 *)((unsigned char *)temp_v0 + (6612)) = (s32) D_8015EECC;
        *(s32 *)((unsigned char *)temp_v0 + (6648)) = (s32) D_8015EE9C;
        *(s8 *)((unsigned char *)temp_v0 + (6663)) = 0x1D;
        *(s32 *)((unsigned char *)temp_v0 + (6676)) = (s32) D_8015EEB4;
        *(s32 *)((unsigned char *)temp_v0 + (6680)) = (s32) D_8015EECC;
        *(s32 *)((unsigned char *)temp_v0 + (6716)) = (s32) D_8015EE9C;
        *(s8 *)((unsigned char *)temp_v0 + (6731)) = 0x1D;
        *(s32 *)((unsigned char *)temp_v0 + (6744)) = (s32) D_8015EEB4;
        *(s32 *)((unsigned char *)temp_v0 + (6748)) = (s32) D_8015EECC;
        *(s32 *)((unsigned char *)temp_v0 + (6784)) = (s32) D_8015EE9C;
        *(s8 *)((unsigned char *)temp_v0 + (6799)) = 0x1D;
        *(s32 *)((unsigned char *)temp_v0 + (6540)) = (s32) D_8015EEB4;
        *(s32 *)((unsigned char *)temp_v0 + (6544)) = (s32) D_8015EECC;
        *(s32 *)((unsigned char *)temp_v0 + (6580)) = (s32) D_8015EE9C;
        *(s8 *)((unsigned char *)temp_v0 + (6595)) = 0x1D;
    }
    var_v0_9 = D_8011ECD0 + 0xEE0;
    do {
        *(s32 *)((unsigned char *)var_v0_9 + (6540)) = (s32) D_8015EEA8;
        *(s32 *)((unsigned char *)var_v0_9 + (6544)) = (s32) D_8015EEC0;
        *(s32 *)((unsigned char *)var_v0_9 + (6580)) = (s32) D_8015EE90;
        if (D_8015EB98 == 0x20000) {
            *(s8 *)((unsigned char *)var_v0_9 + (6595)) = 0x13;
        } else {
            *(s8 *)((unsigned char *)var_v0_9 + (6595)) = 0x12;
        }
        *(s32 *)((unsigned char *)var_v0_9 + (6608)) = (s32) D_8015EEA8;
        *(s32 *)((unsigned char *)var_v0_9 + (6612)) = (s32) D_8015EEC0;
        *(s32 *)((unsigned char *)var_v0_9 + (6648)) = (s32) D_8015EE90;
        if (D_8015EB98 == 0x20000) {
            *(s8 *)((unsigned char *)var_v0_9 + (6663)) = 0x13;
        } else {
            *(s8 *)((unsigned char *)var_v0_9 + (6663)) = 0x12;
        }
        *(s32 *)((unsigned char *)var_v0_9 + (6676)) = (s32) D_8015EEA8;
        *(s32 *)((unsigned char *)var_v0_9 + (6680)) = (s32) D_8015EEC0;
        *(s32 *)((unsigned char *)var_v0_9 + (6716)) = (s32) D_8015EE90;
        if (D_8015EB98 == 0x20000) {
            *(s8 *)((unsigned char *)var_v0_9 + (6731)) = 0x13;
        } else {
            *(s8 *)((unsigned char *)var_v0_9 + (6731)) = 0x12;
        }
        *(s32 *)((unsigned char *)var_v0_9 + (6744)) = (s32) D_8015EEA8;
        *(s32 *)((unsigned char *)var_v0_9 + (6748)) = (s32) D_8015EEC0;
        *(s32 *)((unsigned char *)var_v0_9 + (6784)) = (s32) D_8015EE90;
        if (D_8015EB98 == 0x20000) {
            *(s8 *)((unsigned char *)var_v0_9 + (6799)) = 0x13;
        } else {
            *(s8 *)((unsigned char *)var_v0_9 + (6799)) = 0x12;
        }
        var_v0_9 += 0x110;
    } while (var_v0_9 != (D_8011ECD0 + 0x1100));
}
