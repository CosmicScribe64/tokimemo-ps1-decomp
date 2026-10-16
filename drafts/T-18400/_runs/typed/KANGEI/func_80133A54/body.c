void func_80133A54(void) {
    s16 temp_v1;
    s32 temp_t8;
    s32 var_v0;
    u32 temp_t2;
    u32 temp_t6;
    u8 temp_a3;
    u8 temp_t9;

    if (!(((DisplayActor44 *)D_8011ECD0)[96].unk_02 & 1)) {
        temp_v1 = ((DisplayActor44 *)D_8011ECD0)[96].unk_08;
        var_v0 = temp_v1 < 5;
        if (temp_v1 == 0) {
            temp_t8 = ((DisplayActor44 *)D_8011ECD0)[96].unk_16 & 1;
            switch (temp_t8) {
            case 0:
                func_80044750(0x601);
                break;
            case 1:
                func_80044750(0x602);
                break;
            }
            temp_t9 = D_800E6280.unk_71C;
            temp_t2 = (u16) D_800E6280.unk_0F4.h >> 0xC;
            temp_a3 = D_800E6280.unk_69C[temp_t9].unk_01;
            D_800E6280.unk_66C[temp_t2].unk_00 += 2;
            temp_t6 = (u16) D_800E6280.unk_0F4.h >> 0xC;
            if (temp_t6 < 2U) {
                *(s32 *)&D_800E6280.unk_100 = *(s32 *)&D_800E6280.unk_100 + 0x18000;
            } else if (temp_t6 < 4U) {
                *(s32 *)&D_800E6280.unk_104 = *(s32 *)&D_800E6280.unk_104 + 0x18000;
            } else if (temp_t6 < 6U) {
                *(s32 *)&D_800E6280.unk_108 = *(s32 *)&D_800E6280.unk_108 + 0x18000;
            } else {
                *(s32 *)&D_800E6280.unk_10C = *(s32 *)&D_800E6280.unk_10C + 0x18000;
                *(s32 *)&D_800E6280.unk_118 = *(s32 *)&D_800E6280.unk_118 + 0x18000;
            }
            parameter_up_down(temp_a3, 0, D_800E6280.unk_69C[temp_t9].unk_02, temp_a3);
            parameter_show_init();
            var_v0 = ((DisplayActor44 *)D_8011ECD0)[96].unk_08 < 5;
        }
        ((DisplayActor44 *)D_8011ECD0)[96].unk_08 = (s16) (temp_v1 + 1);
        if ((var_v0 ^ 1) != 0) {
            func_80042808();
        }
    }
}
