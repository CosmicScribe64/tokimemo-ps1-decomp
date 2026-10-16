? func_8004284C();                                  /* extern */
? func_80042908(?);                                 /* extern */
? func_80042940(?);                                 /* extern */
? func_8004EC24();                                  /* extern */
? func_8004F984(?, s16, s16);                       /* extern */
? func_8004FC10(?);                                 /* extern */
? func_80050B54(?);                                 /* extern */
? func_80053CE0(s32);                               /* extern */
s32 func_80054694(u8);                              /* extern */
? func_8005C7C0();                                  /* extern */
? func_8005C7FC();                                  /* extern */
? func_800696DC(?, ?);                              /* extern */
extern ? D_800E6280;
extern ? D_800E7D14;
extern u8 D_800E7D36;
extern u8 D_800E8BEE;
extern ? D_8011ECD0;

void func_80144DF0(void) {
    s32 sp28;
    s32 temp_v1;
    u8 temp_v0;
    u8 temp_v0_2;

    func_8004F984(1, *(s16 *)((unsigned char *)(&D_8011ECD0) + (38)), *(s16 *)((unsigned char *)(&D_8011ECD0) + (42)));
    func_8004FC10(1);
    func_80050B54(1);
    temp_v1 = *(s8 *)((unsigned char *)(&D_800E6280) + (4244)) + 1;
    sp28 = temp_v1;
    func_8004EC24();
    if (*(s32 *)((unsigned char *)(&D_800E6280) + (3976)) & 0x20) {
        if (*(s8 *)((unsigned char *)(&D_800E6280) + (4244)) != -1) {
            if (*(&D_800E7D14 + temp_v1) == 0) {
                temp_v0 = temp_v1 & 0xFF;
                *(u8 *)((unsigned char *)(&D_800E6280) + (4376)) = temp_v0;
                if (func_80054694(temp_v0) == 1) {
                    func_80053CE0();
                    func_8004284C();
                } else {
                    func_80042940(0xF0);
                }
            } else if (temp_v1 == D_800E6280.unk0) {
                func_80053CE0(*(s32 *)((unsigned char *)(&D_800E6280) + (3976)));
                func_8004284C();
            } else {
                temp_v0_2 = temp_v1 & 0xFF;
                if (D_800E8BEE != 0) {
                    D_800E7D36 = temp_v0_2;
                    *(u8 *)((unsigned char *)(&D_800E6280) + (4376)) = temp_v0_2;
                    func_800696DC(0, 0);
                    func_8005C7C0();
                    *(s8 *)((unsigned char *)(&D_800E6280) + (4244)) = (s8) (D_800E7D36 - 1);
                } else {
                    D_800E6280.unk0 = temp_v0_2;
                    func_80042940(0xC0);
                }
            }
        }
    } else if (*(s32 *)((unsigned char *)(&D_800E6280) + (3976)) & 0x40) {
        func_80042908(2);
    }
    func_8004EC24();
    func_8005C7FC();
}
