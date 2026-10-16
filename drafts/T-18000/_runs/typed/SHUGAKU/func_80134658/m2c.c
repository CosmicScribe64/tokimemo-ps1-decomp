void func_80134658(void) {
    u8 temp_t1;
    u8 temp_t5;

    func_801365F4();
    D_8013BFFC = D_8013C57C;
    D_8013C000 = D_8013C6B8;
    D_8013C004 = D_8013C7F4;
    func_801344B0();
    func_80043914(D_8013C0D0, 0x11, 1, 2, 0);
    func_80084E90((s32) D_8013C0D4, (s32) D_8013C0D8, (s32) D_8013C0DC, (s32) D_8013C0E0, (s32) D_8013C0E4, (s32) D_8013C0E8);
    func_800850D4((s32) D_8013C0C4, (s32) D_8013C0C8, (s32) D_8013C0C0, (s32) D_8013C0CC);
    temp_t1 = D_800E6280.unk_F5F;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6532)) = 8;
    D_800CA2E4 = 0;
    D_800CA2E8 = 0;
    D_800E6280.unk_1BC[temp_t1].unk_02 += 1;
    temp_t5 = D_800E6280.unk_F5F;
    D_800E6280.unk_1BC[temp_t5].unk_06 += 2;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6600)) = 8;
    func_80084D3C(&D_800E6280, 0x38);
    func_800AE0F0(D_800CA19C, "ホテル・廊下");
    func_8004284C();
}
