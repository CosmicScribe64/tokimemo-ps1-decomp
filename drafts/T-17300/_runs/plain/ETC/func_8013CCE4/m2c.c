extern s8 D_800E62BA;
extern s32 D_800E7208;
extern u8 D_800E738D;

void func_8013CCE4(void) {
    s32 sp58;
    s32 *temp_v0;
    s32 temp_v1;
    u8 temp_v0_2;

    switch (D_800E738D) {                           /* irregular */
    case 0:
        if (D_80150068 > 0) {
            cd_read_sectors(0x69U, 0x801A0000, 0xB389);
            D_800E738D += 1;
        } else {
            D_800E738D += 1;
        }
        break;
    case 1:
        if (D_80150068 == 0) {
            D_800E738D += 1;
        } else if (D_80150068 > 0) {
            if (cd_read_sync_status() & 1) {
                func_800AE0A0((void *)0x801E2000, 0x801A0600, 0x18000);
                if (D_80150068 == 1) {
                    D_80150068 = 2;
                }
                D_800E738D += 1;
            }
        }
        break;
    case 2:
        if (D_800E7208 & 0x20) {
            temp_v1 = 2 << 0xE;
            temp_v0 = &(&sp54[0])[2];
            sp58 = 0x801E6000;
            sp54[0] = 0x801E2000;
            *(s32 *)((unsigned char *)temp_v0 + (4)) = (s32) (temp_v1 + 0x801E6000);
            temp_v0->unk0 = temp_v1 + 0x801E2000;
            *(s32 *)((unsigned char *)temp_v0 + (12)) = (s32) (temp_v1 + 0x801EE000);
            *(s32 *)((unsigned char *)temp_v0 + (8)) = (s32) (temp_v1 + 0x801EA000);
            func_8004A1A0((s32) &sp54[0], 0, 6, 3, 2, 0x1FF, -0xA0, -0x7D, 0xD, 5);
            D_801220D0 = 0x01000000;
            temp_v0_2 = 0x80 & 0xFF;
            D_801220E1 = temp_v0_2;
            D_801220E0 = temp_v0_2;
            D_800E62BA = 0x80;
            D_800E738D += 1;
            D_801220E2 = 0x80;
        }
        break;
    case 3:
        if (D_801500D4 >= 0xC00) {
            func_80042940(0);
        } else {
            D_801500D4 += 8;
            func_8013FA7C();
        }
        break;
    }
}
