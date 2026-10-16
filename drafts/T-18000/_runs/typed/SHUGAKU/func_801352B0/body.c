void func_801352B0(void) {
    s32 sp30;
    s32 sp2C;
    s32 temp_v1;

    if ((u8) D_800E6280.unk_03C == 2) {
        D_800E6280.unk_03C = 1;
    }
    temp_v1 = D_800E6280.unk_1104.w;
    sp30 = (s32) D_800E6280.unk_110A;
    if (!(D_800E6280.unk_F88 & 0x600060) || (((DisplayActor44 *)D_8011ECD0)[98].unk_26 < -0xBF)) {
        sp2C = temp_v1;
        func_801343B0();
    }
    D_800E6280.unk_1104.w = temp_v1 + 1;
    if (((DisplayActor44 *)D_8011ECD0)[98].unk_26 >= -0xBF) {
        if (!(D_800E6280.unk_1104.w & 1) && (check_end_k() != 0) && (((u8) D_800E6280.unk_03C == 0) || (func_80046094() == 9))) {
            ((DisplayActor44 *)D_8011ECD0)[98].unk_26 = (s16) (((DisplayActor44 *)D_8011ECD0)[98].unk_26 - 1);
        }
        ((DisplayActor44 *)D_8011ECD0)[2].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[2].unk_03 & 0x7F);
    }
    if (sp30 != D_800E6280.unk_1104.w) {
        if (((DisplayActor44 *)D_8011ECD0)[98].unk_26 >= -0xBF) {
            D_800E6280.unk_110A = (u8) sp30;
            D_800CA2E4 = 9;
            D_800CA2E8 = 0;
        } else {
            D_800CA2E4 = 9;
            D_800CA2E8 = 1;
        }
    }
}
