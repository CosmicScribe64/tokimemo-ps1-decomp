? func_800639D8();                                  /* extern */
? func_800747DC(u8, ?, u8, u8);                     /* extern */
extern u16 D_800E6374;
extern s32 D_800E6380;
extern s32 D_800E6384;
extern s32 D_800E6388;
extern s32 D_800E638C;
extern s32 D_800E6398;
extern u8 D_800E699C;

void func_80133A54(void) {
    s16 temp_v1;
    s32 temp_t8;
    s32 var_v0;
    u32 temp_t2;
    u32 temp_t6;
    u8 temp_a3;

    if (!(D_80120652 & 1)) {
        temp_v1 = D_80120658;
        var_v0 = temp_v1 < 5;
        if (temp_v1 == 0) {
            temp_t8 = D_80120666 & 1;
            switch (temp_t8) {                      /* irregular */
            case 0:
                func_80044750(0x601);
                break;
            case 1:
                func_80044750(0x602);
                break;
            }
            temp_t2 = (u16) D_800E6374 >> 0xC;
            temp_a3 = D_800E6280.unk_69C[D_800E699C].unk_01;
            D_800E6280.unk_66C[temp_t2].unk_00 += 2;
            temp_t6 = (u16) D_800E6374 >> 0xC;
            if (temp_t6 < 2U) {
                D_800E6380 += 0x18000;
            } else if (temp_t6 < 4U) {
                D_800E6384 += 0x18000;
            } else if (temp_t6 < 6U) {
                D_800E6388 += 0x18000;
            } else {
                D_800E638C += 0x18000;
                D_800E6398 += 0x18000;
            }
            func_800747DC(temp_a3, 0, D_800E6280.unk_69C[D_800E699C].unk_02, temp_a3);
            func_800639D8();
            var_v0 = D_80120658 < 5;
        }
        D_80120658 = temp_v1 + 1;
        if ((var_v0 ^ 1) != 0) {
            func_80042808();
        }
    }
}
