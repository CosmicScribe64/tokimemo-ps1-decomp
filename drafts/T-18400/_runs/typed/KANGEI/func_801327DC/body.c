void func_801327DC(void) {
    s16 var_v1;
    s16 var_v1_2;
    s32 temp_v0_2;
    u32 temp_t6;
    u32 temp_v0;
    u8 temp_t7;

    func_80085B3C(0x10, (s32) ((u16) D_800E6280.unk_0F4.h >> 0xC));
    temp_t6 = (u16) D_800E6280.unk_0F4.h >> 0xC;
    switch (temp_t6) {
    case 0:
        func_800AE0F0(D_800CA19C, "文芸部");
        bg_read_sub2(0x40BC);
        break;
    case 1:
        func_800AE0F0(D_800CA19C, "演劇部");
        bg_read_sub2(0x40B2);
        break;
    case 2:
        func_800AE0F0(D_800CA19C, "科学部");
        bg_read_sub2(0x40C6);
        break;
    case 3:
        func_800AE0F0(D_800CA19C, "電脳部");
        bg_read_sub2(0x40D0);
        break;
    case 4:
        func_800AE0F0(D_800CA19C, "美術部");
        bg_read_sub2(0x40D9);
        break;
    case 5:
        func_800AE0F0(D_800CA19C, "吹奏楽部");
        bg_read_sub2(0x40E2);
        break;
    case 6:
        func_800AE0F0(D_800CA19C, "野球部");
        bg_read_sub2(0x4097);
        break;
    case 7:
        func_800AE0F0(D_800CA19C, "サッカー部");
        bg_read_sub2(0x4097);
        break;
    case 8:
        func_800AE0F0(D_800CA19C, "テニス部");
        bg_read_sub2(0x4097);
        break;
    case 9:
        func_800AE0F0(D_800CA19C, "水泳部");
        bg_read_sub2(0x40A0);
        break;
    default:
        func_800AE0F0(D_800CA19C, "バスケ部");
        bg_read_sub2(0x40A9);
        break;
    }
    temp_t7 = D_800E6280.unk_F5F;
    D_800CA14C = 0;
    switch (temp_t7) {
    case 0:
        temp_v0 = (u16) D_800E6280.unk_0F4.h >> 0xC;
        if (temp_v0 < 6U) {
            var_v1 = 0;
        } else if (temp_v0 == 6) {
            var_v1 = 2;
        } else {
            var_v1 = 6;
            if (temp_v0 == 7) {
                var_v1 = 4;
            }
        }
        D_800CA148 = var_v1;
        if (!(D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0] & 4)) {
            D_800CA148 = var_v1 + 1;
        }
        break;
    case 1:
        D_800CA148 = 0;
        if (!(D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0] & 4)) {
            D_800CA148 = 1;
        }
        break;
    case 2:
        temp_v0_2 = D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0];
        D_800CA148 = 0;
        if (!(temp_v0_2 & 4)) {
            D_800CA148 = 1;
        }
        if (temp_v0_2 & 0x80000) {
            D_800CA148 = 2;
        }
        break;
    case 3:
        if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 4) {
            var_v1_2 = 0;
        } else {
            var_v1_2 = 1;
        }
        D_800CA148 = var_v1_2;
        if (!(D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0] & 4)) {
            D_800CA148 = var_v1_2 + 2;
        }
        break;
    case 4:
        if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 6) {
block_44:
            D_800CA148 = 0;
        } else {
            D_800CA148 = 1;
        }
        break;
    case 5:
        D_800CA148 = 0;
        if (!(D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0] & 4)) {
            D_800CA148 = 1;
        }
        break;
    case 6:
        D_800CA148 = 0;
        if ((u32) (get_g_zyotai_s(6) & 0x7F) < 2U) {
            D_800CA148 += 1;
        }
        break;
    default:
        goto block_44;
    }
    func_8004284C();
}
