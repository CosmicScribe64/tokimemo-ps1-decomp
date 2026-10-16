? func_800639D8();                                  /* extern */
? func_800747DC(u8, ?, u8, u8);                     /* extern */

void func_80133A54(void) {
    s16 temp_v1;
    s32 temp_t8;
    s32 var_v0;
    u32 temp_t2;
    u32 temp_t6;
    u8 temp_a3;
    u8 temp_t9;

    if (!(*(u8 *)((unsigned char *)D_8011ECD0 + (6530)) & 1)) {
        temp_v1 = *(s16 *)((unsigned char *)D_8011ECD0 + (6536));
        var_v0 = temp_v1 < 5;
        if (temp_v1 == 0) {
            temp_t8 = *(s16 *)((unsigned char *)D_8011ECD0 + (6550)) & 1;
            switch (temp_t8) {                      /* irregular */
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
                D_800E6280.unk_100 = (s32) D_800E6280.unk_100 + 0x18000;
            } else if (temp_t6 < 4U) {
                D_800E6280.unk_104 = (s32) D_800E6280.unk_104 + 0x18000;
            } else if (temp_t6 < 6U) {
                D_800E6280.unk_108 = (s32) D_800E6280.unk_108 + 0x18000;
            } else {
                D_800E6280.unk_10C = (s32) D_800E6280.unk_10C + 0x18000;
                D_800E6280.unk_118 = (s32) D_800E6280.unk_118 + 0x18000;
            }
            func_800747DC(temp_a3, 0, D_800E6280.unk_69C[temp_t9].unk_02, temp_a3);
            func_800639D8();
            var_v0 = *(s16 *)((unsigned char *)D_8011ECD0 + (6536)) < 5;
        }
        *(s16 *)((unsigned char *)D_8011ECD0 + (6536)) = (s16) (temp_v1 + 1);
        if ((var_v0 ^ 1) != 0) {
            func_80042808();
        }
    }
}
