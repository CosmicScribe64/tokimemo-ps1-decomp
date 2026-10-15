s32 func_800485BC(s32, ?, s32 *);                   /* extern */
? func_8004D088(?, ?, ?, ?);                        /* extern */
? func_8004E350(?, s32, ?);                         /* extern */
? func_80085ED0();                                  /* extern */
? func_800862D0();                                  /* extern */
s32 func_800862F0();                                /* extern */
? func_80086350();                                  /* extern */
s32 func_80086370();                                /* extern */
? func_800863D4(?);                                 /* extern */
extern s32 D_800E7208;
extern u8 D_800E738A;
extern s32 D_800E7558;
extern s16 D_80120962;
extern s32 D_80149DAC;
extern s32 D_80149DF4;
extern ? D_80149E90;
extern ? D_80149EBC;
extern ? D_8014A088;

void func_80138DF8(void) {
    s16 spA2;
    s16 spA0;
    s16 sp9E;
    s16 sp9C;
    ? sp44;
    s32 *sp40;
    s16 temp_v1_3;
    s16 temp_v1_4;
    s32 *var_a2;
    s32 *var_a2_2;
    s32 *var_v1;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    u8 *var_s1;

    M2C_MEMCPY_ALIGNED(&sp70[0], &D_80149E90, 0x28);
    *(s32 *)((unsigned char *)(&sp70[0] + 0x24) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80149E90 + 0x24) + (4));
    M2C_MEMCPY_ALIGNED(&sp44, &D_80149EBC, 0x24);
    *(s32 *)((unsigned char *)(&sp44) + (36)) = (s32) *(s32 *)((unsigned char *)(&D_80149EBC) + (36));
    *(s32 *)((unsigned char *)(&sp44 + 0x24) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80149EBC + 0x24) + (4));
    switch (D_800E738A) {
    case 0:
        var_s0 = 0;
        if (cd_read_sync_status() & 1) {
            var_a2 = &D_80149DAC;
            do {
                sp40 = var_a2;
                temp_v1 = var_s0 + 0x80000100;
                var_s0 += 1;
                D_800E6280.unk_1228[func_800485BC(*var_a2, 1, var_a2)] = temp_v1;
                var_a2 = &var_a2[1];
                *(s32 *)((unsigned char *)var_a2 + (-4)) = temp_v1;
            } while (var_s0 < 0x12);
            sp9C = 0x300;
            sp9E = 0x80;
            spA0 = 0x40;
            spA2 = 0x80;
            func_8009C884((RECT *) &sp9C, (void *)0x801BB000);
            D_800E7558 = 8;
            var_a2_2 = &D_80149DF4;
            var_s0_2 = 0;
            do {
                sp40 = var_a2_2;
                temp_v1_2 = var_s0_2 + 0x80000200;
                var_s0_2 += 1;
                D_800E6280.unk_1228[func_800485BC(*var_a2_2, 1, var_a2_2)] = temp_v1_2;
                var_a2_2 = &var_a2_2[1];
                *(s32 *)((unsigned char *)var_a2_2 + (-4)) = temp_v1_2;
            } while (var_s0_2 != 5);
            cd_read_sectors(3U, 0x801F8000, 0xBE2A);
            func_80086350();
            func_80044750(0x200);
            func_8004284C();
        }
        break;
    case 1:
        if (func_80086370() != 0) {
            if (cd_read_sync_status() & 1) {
                func_800438DC(1, 1);
                func_8014045C();
                func_801404DC();
                func_8014064C();
                func_801406A8(0x801F8000);
                func_80140738(0x801F8000, 0);
                func_8004284C();
            }
        }
        func_80085F0C(0, 0xF, 0, 0xF0, 0x280);
        break;
    case 2:
        func_80085F0C(0, 0xF, 0, 0xF0, 0x280);
        func_80139FE4();
        func_800863D4(0x6B);
        var_s0_3 = 0;
        if (D_800E7208 & 0xF0) {
            var_s1 = D_8011ECD0;
            do {
                temp_v1_3 = *(s16 *)((unsigned char *)var_s1 + (6566));
                temp_v0 = D_80120962 + 8;
                if ((temp_v0 >= temp_v1_3) && ((temp_v1_3 + 0x20) >= temp_v0)) {
                    temp_v1_4 = *(s16 *)((unsigned char *)var_s1 + (6570));
                    temp_v0_2 = D_80120966 + 0x10;
                    if ((temp_v0_2 >= temp_v1_4) && ((temp_v1_4 + 0x40) >= temp_v0_2) && (*(u8 *)((unsigned char *)var_s1 + (6531)) != 0)) {
                        D_80149940 = var_s0_3;
                        func_800862D0();
                        func_80044750(0x74);
                        func_8004284C();
                    }
                }
                var_s0_3 += 1;
                var_s1 += 0x44;
            } while (var_s0_3 != 0xB);
        }
        break;
    case 3:
        func_80085F0C(0, 0xF, 0, 0xF0, 0x280);
        if (func_800460EC() == 2) {
            cd_read_sectors(0x87U, 0x8018F000, 0xB98F);
            func_8004284C();
        }
        break;
    case 4:
        func_80085F0C(0, 0xF, 0, 0xF0, 0x280);
        if (func_800862F0() != 0) {
            func_8004284C();
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_80133C30();
            func_8004E44C(0, (u32 *) D_801491C4, D_801491C0);
            func_8004E350(0, D_801491C8, 0);
            func_8004D088(0, 5, 0x140, 0xF0);
            func_801331D0();
            var_v1 = D_8014A070;
            do {
                sp40 = var_v1;
                func_800485BC(*var_v1, 1);
                var_v1 = &var_v1[1];
            } while (var_v1 != &D_8014A088);
            func_8013A314();
            D_80122EA0 = 0;
            func_80044890(0, 0xBF98, 0xBF79, (&sp70[0])[D_80149940], *(s32 *)((unsigned char *)(sp + (D_80149940 * 4)) + (68)), 0xD6B7);
            func_80086350();
            func_8004284C();
        }
        break;
    case 6:
        func_800867CC();
        if (func_80086370() != 0) {
            func_80085ED0();
            func_80042808();
        }
        func_8013DDB8();
        func_8013D788();
        func_8013D2B4();
        break;
    default:
        func_80046500();
        break;
    }
}
