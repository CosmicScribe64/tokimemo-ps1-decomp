s32 func_8013B94C(void) {
    s16 sp2E;
    s16 sp2C;
    s16 temp_t3;
    s32 temp_t9;
    s32 temp_v0;

    if ((D_801499A0.unk_02 == 1) && (((temp_v0 = ((s16) D_80149968.unk_1C[0] >> 3) - *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6634)), ((temp_v0 < 0x21) == 0)) && (D_80149968.unk_0C > 0)) || ((temp_v0 < -0x20) && (D_80149968.unk_0C < 0)))) {
        func_80044750(0x802);
        D_801499B2 = 2;
    }
    if ((u16) D_801499A0.unk_02 >= 0x88U) {
        D_801499A0.unk_02 = 0;
    }
    func_8013C5C8(0x12000);
    func_8013C694();
    if (!(D_801499A0.unk_02 & 0x1F)) {
        func_8013C4E4(0xC000);
    }
    if (!(D_801499A0.unk_02 & 0xF)) {
        temp_t9 = (s16) D_80149968.unk_1C[0] >> 3;
        sp2C = 0;
        D_801499A0.unk_06 = ((s16) D_80149968.unk_1C[1] >> 3) + 0x10;
        temp_t3 = temp_t9 + ((s32) (temp_t9 * 0x4F) / 48);
        D_801499A0.unk_04 = temp_t3;
        sp2E = temp_t3;
    }
    if ((D_801499B2 == 2) && ((u16) D_801499A0.unk_00 >= 0x79U)) {
        D_801499B2 = 0;
    }
    if ((D_801499A0.unk_00 != 0) && (((u16) D_801499A0.unk_00 % 90U) == 0)) {
        if (D_801499B2 == 0) {
            D_801499B2 = 1;
            func_80044750(0x800);
            D_801499A0.unk_02 += 1;
        } else if (D_801499B2 == 1) {
            D_801499B2 = 0;
            func_80044750(0x801);
            D_801499A0.unk_02 += 1;
        }
    }
}
