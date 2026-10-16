? cd_read_sectors(?, ?, ?);                         /* extern */
s32 cd_read_sync_status();                          /* extern */
? func_80042808();                                  /* extern */
? func_8004284C();                                  /* extern */
? func_80042940(?);                                 /* extern */
? func_80057418(?, s32);                            /* extern */
s32 func_8005751C(?);                               /* extern */
? func_801328A4(?, ?);                              /* extern */
? func_8013D264();                                  /* extern */
? func_8013D2C0();                                  /* extern */
? func_8013D32C();                                  /* extern */
? func_8013DEA0(?);                                 /* extern */
? func_8013E364();                                  /* extern */
? func_8013E89C(?);                                 /* extern */
? func_8013ECDC();                                  /* extern */
? func_8013EDF8();                                  /* extern */
? func_8013EE28();                                  /* extern */
? func_8013EEFC();                                  /* extern */
s32 func_8013F3F8();                                /* extern */
? func_8013F498();                                  /* extern */
? func_8013FA7C();                                  /* extern */
? func_8013FC08();                                  /* extern */
extern ? D_800E6280;
extern s32 D_801220D0;
extern s32 D_8014E0C0;
extern s32 D_80150064;
extern s32 D_80150068;
extern s32 D_80150070;
extern s32 D_80150080;
extern s32 D_801500D4;
extern s32 D_801500D8;

void func_8013BFFC(void) {
    switch (*(u8 *)((unsigned char *)(&D_800E6280) + (4365))) {
    case 0:
        func_8013FC08();
        D_801220D0 = 0x80000000;
        if (D_80150068 > 0) {
            D_80150070 = 0;
        }
        if (D_80150070 == -1) {
            D_801500D4 = 0x800;
            func_8013FA7C();
            func_80042808();
        } else {
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        }
        break;
    case 1:
        if (D_80150068 > 0) {
            func_8013E89C(1);
            if (D_80150070 >= 3) {
                if (D_80150068 == 1) {

                } else {
                    D_80150068 = 0;
                }
                D_80150070 = 0;
                *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (D_800E6280.unk0 + 1);
            } else if (D_80150070 == -1) {
                D_801500D4 = 0x800;
                func_8013FA7C();
                func_80042940(5);
            }
        } else {
            D_800E6280.unk0 = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        }
        break;
    case 2:
        if (func_8013F3F8() == 1) {
            cd_read_sectors(0xE, 0x80197000, 0xA9D0);
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        } else {
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (D_800E6280.unk0 + 1);
        }
        break;
    case 3:
        if (func_8013F3F8() == 0) {
            if (D_8014E0C0 == 1) {
                func_80057418(1, func_8005751C(1));
                func_8013DEA0(1);
            }
            func_8013ECDC();
            func_8013EDF8();
            func_8013E364();
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        } else if ((func_8013F3F8() == 1) && (cd_read_sync_status() & 1)) {
            if (D_8014E0C0 == 1) {
                func_80057418(1, func_8005751C(1));
                func_8013DEA0(1);
            }
            func_8013ECDC();
            if (func_8013F3F8() == 1) {
                func_8013F498();
            }
            func_8013EDF8();
            func_8013E364();
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        }
        break;
    case 4:
        func_8013D264();
        func_8013D2C0();
        func_8013EE28();
        func_801328A4(0x28, 0x3C);
        if (D_80150064 < 0xFF) {
            D_80150080 = D_80150064;
            D_80150064 += 1;
        }
        if ((D_801500D8 == 0) || (D_801500D8 == 2)) {
            func_8013EEFC();
        }
        func_8004284C();
        break;
    }
    func_8013D32C();
}
