u8 cd_read_sectors(?, ?, ?);                        /* extern */
u8 cd_read_sync_status(s32);                        /* extern */
u8 func_80042940(?);                                /* extern */
? func_8004A1A0(s32 *, ?, ?, ?, s32, s32, s32, s32, s32, s32); /* extern */
u8 func_800AE0A0(?, ?, ?);                          /* extern */
u8 func_8013FA7C(s32 *);                            /* extern */
extern ? D_800E6280;
extern s32 D_801220D0;
extern u8 D_801220E0;
extern u8 D_801220E1;
extern s8 D_801220E2;
extern s32 D_80150068;
extern s32 D_801500D4;

u8 func_8013CCE4(void) {
    s32 sp58;
    s32 *temp_v0;
    s32 temp_v1;
    u8 var_v0;

    var_v0 = *(u8 *)((unsigned char *)(&D_800E6280) + (4365));
    switch (*(u8 *)((unsigned char *)(&D_800E6280) + (4365))) { /* irregular */
    case 0:
        if (D_80150068 > 0) {
            var_v0 = cd_read_sectors(0x69, 0x801A0000, 0xB389);
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        } else {
            D_800E6280.unk0 = (s8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        }
        break;
    case 1:
        if (D_80150068 == 0) {
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        } else if (D_80150068 > 0) {
            var_v0 = cd_read_sync_status(D_80150068);
            if (var_v0 & 1) {
                var_v0 = func_800AE0A0(0x801E2000, 0x801A0600, 0x18000);
                if (D_80150068 == 1) {
                    D_80150068 = 2;
                }
                *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
            }
        }
        break;
    case 2:
        if (D_800E6280.unk0 & 0x20) {
            temp_v1 = 2 << 0xE;
            temp_v0 = &(&sp54[0])[2];
            sp58 = 0x801E6000;
            sp54[0] = 0x801E2000;
            *(s32 *)((unsigned char *)temp_v0 + (4)) = (s32) (temp_v1 + 0x801E6000);
            temp_v0->unk0 = temp_v1 + 0x801E2000;
            *(s32 *)((unsigned char *)temp_v0 + (12)) = (s32) (temp_v1 + 0x801EE000);
            *(s32 *)((unsigned char *)temp_v0 + (8)) = (s32) (temp_v1 + 0x801EA000);
            func_8004A1A0(&sp54[0], 0, 6, 3, 2, 0x1FF, -0xA0, -0x7D, 0xD, 5);
            D_801220D0 = 0x01000000;
            var_v0 = 0x80 & 0xFF;
            D_801220E1 = var_v0;
            D_801220E0 = var_v0;
            *(s8 *)((unsigned char *)(&D_800E6280) + (58)) = 0x80;
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
            D_801220E2 = 0x80;
        }
        break;
    case 3:
        if (D_801500D4 >= 0xC00) {
            var_v0 = func_80042940(0);
        } else {
            D_801500D4 += 8;
            var_v0 = func_8013FA7C(&D_80150068);
        }
        break;
    }
    return var_v0;
}
