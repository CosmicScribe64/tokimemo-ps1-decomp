void func_80134D94(void) {
    s32 sp3C;
    s16 var_t7;
    s32 temp_t0;
    s32 temp_v0;
    u32 temp_lo;
    u32 var_a0;
    u8 var_a0_2;
    u8 var_a1;

    temp_t0 = D_800E6280.unk_1104.w;
    if (temp_t0 == 0) {
        ((DisplayActor44 *)D_8011ECD0)[99].unk_02 = 5;
    }
    if (temp_t0 == 0xFE) {
        ((DisplayActor44 *)D_8011ECD0)[97].unk_03 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_02 = 5;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_07 = 0x80;
    }
    if (!(((DisplayActor44 *)D_8011ECD0)->unk_00 & 1)) {
        ((DisplayActor44 *)D_8011ECD0)[97].unk_03 = 0;
    }
    if (temp_t0 == 0x29C) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_03 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[98].unk_02 = 5;
        ((DisplayActor44 *)D_8011ECD0)[98].unk_07 = 0x80;
    }
    if (((u32) temp_t0 >= 0xFFU) && ((u32) temp_t0 < 0x5A3U)) {
        var_a1 = D_800E6280.unk_110D;
        ((DisplayActor44 *)D_8011ECD0)[99].unk_02 = 0x40;
        var_a0 = var_a1 / 14U;
        if (var_a0 >= 2U) {
            var_a1 = 0;
            var_a0 = 0;
        }
        switch (D_80122EAC) {
        case 0:
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = (s16) (var_a0 + 0x5D);
            break;
        case 1:
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = (s16) (var_a0 + 0x72);
            break;
        case 2:
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = (s16) (var_a0 + 0x87);
            break;
        default:
        case 3:
            if ((u32) temp_t0 < 0x4C2U) {
                ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = (s16) (var_a0 + 0x9D);
            } else {
                ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = (s16) (var_a0 + 0x9F);
            }
            break;
        }
        D_800E6280.unk_110D = var_a1 + 1;
    }
    if ((temp_t0 == 0x4C2) && (D_80122EAC != 3)) {
        ((DisplayActor44 *)D_8011ECD0)[99].unk_03 = 0;
        ((DisplayActor44 *)D_8011ECD0)[100].unk_03 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[100].unk_02 = 5;
        ((DisplayActor44 *)D_8011ECD0)[100].unk_07 = 0x80;
    }
    if ((temp_t0 == 0x58F) && (D_80122EAC == 3)) {
        ((DisplayActor44 *)D_8011ECD0)[99].unk_03 = 0;
        ((DisplayActor44 *)D_8011ECD0)[100].unk_03 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[100].unk_02 = 5;
        ((DisplayActor44 *)D_8011ECD0)[100].unk_07 = 0x80;
    }
    if ((temp_t0 == 0x606) || (temp_t0 == 0x609) || (temp_t0 == 0x60C)) {
        func_80049A40(-0xA0, -0x78, 0x140, 0xA0, 2, 0xFFFFFF, 0x81);
        dtd_on_tpage(0, 0, 2, 0, 0);
    }
    if (temp_t0 == 0x606) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0x48;
        ((DisplayActor44 *)D_8011ECD0)[98].unk_08 = 0;
    }
    if (((u32) temp_t0 >= 0x60FU) && ((u32) temp_t0 < 0x68EU)) {
        temp_v0 = (0x68E - temp_t0) * 0x10201;
        sp3C = temp_v0;
        func_80049A40(-0x130, -0x78, 0x140, 0xA0, 2, temp_v0, 0x86);
        func_80049A40(0x10, -0x78, 0x140, 0xA0, 2, temp_v0, 0x85);
        dtd_on_tpage(0, 0, 2, 1, 0);
    }
    if (temp_t0 == 0x67E) {
        ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[98].unk_07 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[100].unk_07 = 0x80;
    }
    if (((u32) temp_t0 >= 0x723U) && (D_80122EAC == 3)) {
        var_a0_2 = (u8) D_800E6280.unk_110C;
        ((DisplayActor44 *)D_8011ECD0)[100].unk_02 = 0x40;
        temp_lo = var_a0_2 / 14U;
        var_t7 = temp_lo + 0xA2;
        if (temp_lo >= 2U) {
            var_a0_2 = 0;
            var_t7 = 0xA2;
        }
        ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = var_t7;
        D_800E6280.unk_110C = var_a0_2 + 1;
    }
}
