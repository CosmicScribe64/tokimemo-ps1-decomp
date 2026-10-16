s32 func_8013B3B0(void) {
    u16 sp2E;
    s16 sp2C;
    s16 var_v0;
    s32 temp_v1;

    if (D_801499A0.unk_02 == 0x258) {
        func_80044750(0x802);
        D_801499B0->unk0 = 1;
        D_801499A0.unk_08 = 0;
        D_801499A0.unk_0C = 0;
        D_801499B2 = 0;
        *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6620)) = 0x7F;
    } else if ((u16) D_801499A0.unk_02 >= 0x2D1U) {
        D_801499B0->unk0 = 0;
        D_801499A0.unk_02 = 0;
    } else if ((u16) D_801499A0.unk_00 >= 0x259U) {
        if (*(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6638)) < 0x30) {
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6638)) = (s16) (*(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6638)) + 1);
        }
    } else {
        func_8013C5C8(0x10000);
        func_8013C694();
        if (!(D_801499A0.unk_02 & 8)) {
            func_8013C4E4(0x8000);
        }
        if (!(D_801499A0.unk_02 & 0xF)) {
            temp_v1 = (s16) D_80149968.unk_1C[0] >> 3;
            sp2C = 0;
            var_v0 = temp_v1 + ((s32) (temp_v1 * 0x50) / 48);
            if ((((s32) D_80122EC4 % 3) != 0) && ((D_80149968.unk_1C[0] >= 0) || (D_80149968.unk_0C > 0))) {
                var_v0 = temp_v1 + 0x20;
            }
            D_801499A0.unk_00 = (u16) var_v0;
            D_801499A0.unk_00 = ((s16) D_80149968.unk0 >> 3) + 0x10;
            sp2E = (u16) var_v0;
        }
    }
    if (((u16) D_801499B0->unk0 == 0) && (D_801499A0.unk_02 != 0) && (((u16) D_801499A0.unk_02 % 45U) == 0)) {
        if (D_801499B2 == 0) {
            D_801499B2 = 1;
            func_80044750(0x800);
        } else {
            D_801499B2 = 0;
            func_80044750(0x801);
        }
        D_801499A0.unk_02 += 1;
    }
}
