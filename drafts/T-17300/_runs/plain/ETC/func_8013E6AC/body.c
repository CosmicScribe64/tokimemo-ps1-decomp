void func_8013E6AC(void) {
    u8 temp_t6;
    u8 temp_t9;
    u8 *temp_v0;

    if (D_800E73A4 == 0) {
        D_8015004C = (s32) D_800E6280.unk_75E[D_80150064].unk_00;
        D_80150044 = (s32) D_800E6280.unk_75E[D_80150064].unk_01;
        D_80150040 = (s32) D_800E6280.unk_75E[D_80150064].unk_02;
        D_80150048 = (s32) ((u8) D_800E6280.unk_75E[D_80150064].unk_03 % 6U);
        D_80150054 = (s32) D_800E6280.unk_75E[D_80150064].unk_04;
        D_80150058 = (s32) D_800E6280.unk_75E[D_80150064].unk_05;
        D_8015005C = (s32) D_800E6280.unk_75E[D_80150064].unk_06;
        D_80150060 = (s32) D_800E6280.unk_75E[D_80150064].unk_07;
        temp_t9 = D_800E6280.unk_75E[D_80150064].unk_05;
        switch (temp_t9) {
        case 3:
        case 4:
        case 5:
            D_80150050 = 0;
            return;
        case 6:
        case 7:
        case 8:
            D_80150050 = 1;
            return;
        case 9:
        case 10:
        case 11:
            D_80150050 = 2;
            return;
        case 1:
        case 2:
        case 12:
            D_80150050 = 3;
            return;
        }
    } else {
        temp_v0 = (D_80150064 * 8) + &D_800E7D10;
        D_8015004C = (s32) *(u8 *)((unsigned char *)temp_v0 + (88));
        D_80150044 = (s32) *(u8 *)((unsigned char *)temp_v0 + (89));
        D_80150040 = (s32) *(u8 *)((unsigned char *)temp_v0 + (90));
        D_80150048 = (s32) ((u8) *(u8 *)((unsigned char *)temp_v0 + (91)) % 6U);
        D_80150054 = (s32) *(u8 *)((unsigned char *)temp_v0 + (92));
        D_80150058 = (s32) *(u8 *)((unsigned char *)temp_v0 + (93));
        D_8015005C = (s32) *(u8 *)((unsigned char *)temp_v0 + (94));
        D_80150060 = (s32) *(u8 *)((unsigned char *)temp_v0 + (95));
        temp_t6 = *(u8 *)((unsigned char *)temp_v0 + (93));
        switch (temp_t6) {
        case 3:
        case 4:
        case 5:
            D_80150050 = 0;
            return;
        case 6:
        case 7:
        case 8:
            D_80150050 = 1;
            return;
        case 9:
        case 10:
        case 11:
            D_80150050 = 2;
            return;
        case 1:
        case 2:
        case 12:
            D_80150050 = 3;
            /* fallthrough */
        default:
            return;
        }
    }
}
