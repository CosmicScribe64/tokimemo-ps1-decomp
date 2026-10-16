/* bulk_m2c declarations (T-11190) */
extern s8 D_800E69A1;
extern s8 D_800E69A2;
extern u16 D_800E6374;
extern u8 D_800E69A0;
extern u8 D_800E7388;
extern u8 D_800E7389;
extern u8 D_800E738A;
/* end bulk_m2c declarations */
void func_80132444(void) {
    u32 temp_t8;

    switch (D_800E738A) {
    case 0:
        temp_t8 = (u16) D_800E6374 >> 0xC;
        switch (temp_t8) {
        case 0:
            cd_read_sectors(3U, 0x801E8000, 0xA9DE);
            func_80132104();
            break;
        case 1:
            cd_read_sectors(3U, 0x801E8000, 0xA9B4);
            func_80132000();
            break;
        case 2:
            cd_read_sectors(3U, 0x801E8000, 0xA9BF);
            func_8013209C();
            break;
        case 3:
            cd_read_sectors(3U, 0x801E8000, 0xA9B7);
            func_80132034();
            break;
        case 4:
            cd_read_sectors(3U, 0x801E8000, 0xA9BC);
            func_80132068();
            break;
        case 5:
            cd_read_sectors(3U, 0x801E8000, 0xA9C2);
            func_801320D0();
            break;
        default:
            func_80046500();
            break;
        }
        D_800CA160 = (s32) D_8013C2D0;
        D_800CA164 = (s32) D_8013C2D4;
        D_800CA168 = (s32) D_8013C2D8;
        func_8004284C();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0;
            D_800CA14C = 0;
            func_8004284C();
        }
        break;
    case 2:
        normal_date_speak();
        break;
    case 3:
        normal_date_bg_fadeout();
        break;
    case 4:
        if (((u16) D_800E6374 >> 0xC) == 3) {
            func_80044750(0xBF);
            D_800E6280.unk_720 = D_800E6280.unk_1108;
            D_800E6280.unk_721 = D_800E6280.unk_1109 + 1;
            D_800E6280.unk_722 = 0;
            *(u8 *)&D_800E7D50 |= 3;
            main_state_set(0x90);
        } else {
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}
