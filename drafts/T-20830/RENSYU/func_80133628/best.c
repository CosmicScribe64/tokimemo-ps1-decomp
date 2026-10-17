void func_80133628(void) {
    GameState *g = &D_800E6280;
    u32 girl;
    s16 diff;
    RensyuTbl11 tbl;

    tbl = D_80134390;
    switch (g->unk_110A) {
    case 0:
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_02 == 0) {
            if ((D_801342C0 >= 5U) || ((D_801342C0 != 0) && (((u16) g->unk_0F4.h >> 0xC) == 9))) {
                func_8004284C();
            } else {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0x84;
                ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 5;
                ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0;
                ((DisplayActor44 *)D_8011ECD0)[96].unk_08 = 0;
                if (D_801342C8[D_801342C0] == 1) {
                    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0;
                } else {
                    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 1;
                }
                D_801342C0 += 1;
            }
        }
        break;
    case 1:
        if (D_801342C4 >= 3U) {
            func_80044750(0x204);
            D_800CA148 = 0x11;
        } else {
            func_80044750(0x205);
            D_800CA148 = ((RensyuClubCount *)&g->unk_66C[(u16) g->unk_0F4.h >> 0xC])->count + 0x12;
        }
        k_reset(1);
        func_8004284C();
        break;
    case 2:
        normal_date_speak();
        break;
    case 3:
        girl = (u16) D_800E6280.unk_0F4.h >> 0xC;
        diff = D_800E6280.unk_66C[girl].unk_00 - tbl.v[((RensyuClubCount *)&D_800E6280.unk_66C[girl])->count];
        if (diff < -0x1E) {
            D_800CA148 = 0x1D;
        } else if (diff < -0x14) {
            D_800CA148 = 0x1E;
        } else if (diff < -0xA) {
            D_800CA148 = 0x1F;
        } else if (diff < 0) {
            D_800CA148 = 0x20;
        } else if (diff >= 0x1E) {
            D_800CA148 = 0x24;
        } else if (diff >= 0x14) {
            D_800CA148 = 0x23;
        } else if (diff >= 0xA) {
            D_800CA148 = 0x22;
        } else if (diff >= 0) {
            D_800CA148 = 0x21;
        }
        func_8004284C();
        break;
    case 4:
        normal_date_speak();
        break;
    case 5:
        ((DisplayActor44 *)D_8011ECD0)[96].unk_07 -= 4;
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_07 < 3U) {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0;
        }
        normal_date_bg_fadeout();
        break;
    case 6:
        func_80048F64(0x60);
        if ((D_801342D0 == 0xB) || (D_801342D0 == 0xFF)) {
            func_80042908(7);
        } else {
            func_80042808();
        }
        break;
    }
    Default_Disp();
}
