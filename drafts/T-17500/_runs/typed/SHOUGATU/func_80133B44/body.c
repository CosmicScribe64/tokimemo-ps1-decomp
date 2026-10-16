void func_80133B44(void) {
    s32 sp44;
    u8 sp40;

    *(? *)((unsigned char *)(&sp40) + (-1)) = M2C_LWL(*(? *)((unsigned char *)(&D_801435F8) + (2)));
    if (D_800E6280.unk_F5F != 0) {
        if (D_800E6280.unk_110D == 0) {
            set_kanji_string((s16) (-0x68 - (func_800AE0E0(D_800CA16C) * 4)), 0x30, 0, (s32) D_800CA16C, 0);
            set_kanji_string(-0x56, 0x30, 0, (s32) "『 ", 0);
            dialogue_render(-0x48, 0x30, 0, (s32) "それじゃ、神社に行くか。", 0, &D_8014361C, &D_80143620, 0);
            D_80143B00 = 0;
            D_80143B04 = 0;
            **D_80143AF8 = 7;
            ***D_80143AF4 = sp40;
            *(u8 *)((unsigned char *)(**D_80143AF4) + (1)) = sp41;
            *(s8 *)((unsigned char *)(**D_80143AF4) + (2)) = 0;
            D_800E6280.unk_110D = 1;
        }
        sp44 = (s32) D_800E6280.unk_110A;
        func_801323D4();
        if (sp44 != D_800E6280.unk_110A) {
            func_80042808();
        }
    } else {
        func_8004284C();
    }
}
