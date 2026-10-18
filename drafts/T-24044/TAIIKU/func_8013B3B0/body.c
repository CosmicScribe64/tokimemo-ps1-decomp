/* FAKE: counter/flag u16 views follow the matched T-19008 family recipe. */
s32 func_8013B3B0(void) {
    s32 tmp; /* FAKE: observed packed halfword homes, matched sibling T-19008. */
    s16 var_v0;
    s32 temp_v1;

    if (((u16 *)&D_801499A0)[1] == 0x258) {
        func_80044750(0x802);
        D_801499B0[0].unk0 = 1;
        D_801499A0.unk_08 = 0;
        D_801499A0.unk_0C = 0;
        ((u16 *)D_801499B0)[1] = 0;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0x7F;
    } else if ((u16) ((u16 *)&D_801499A0)[1] >= 0x2D1U) {
        D_801499B0[0].unk0 = 0;
        ((u16 *)&D_801499A0)[1] = 0;
    } else if ((u16) ((u16 *)&D_801499A0)[1] >= 0x259U) {
        if (((DisplayActor44 *)D_8011ECD0)[97].unk_2A < 0x30) {
            ((DisplayActor44 *)D_8011ECD0)[97].unk_2A = (s16) (((DisplayActor44 *)D_8011ECD0)[97].unk_2A + 1);
        }
    } else {
        func_8013C5C8(0x10000);
        func_8013C694();
        if (!(((u16 *)&D_801499A0)[1] & 8)) {
            func_8013C4E4(0x8000);
        }
        if (!(((u16 *)&D_801499A0)[1] & 0xF)) {
            temp_v1 = (s16) D_80149968.unk_1C[0] >> 3;
            ((s16 *)&tmp)[0] = 0;
            var_v0 = temp_v1 + ((s32) (temp_v1 * 0x50) / 48);
            if ((((s32) D_80122EC4 % 3) != 0) && ((D_80149968.unk_1C[0] >= 0) || (D_80149968.unk_0C > 0))) {
                var_v0 = temp_v1 + 0x20;
            }
            D_801499A0.unk_04 = var_v0;
            D_801499A0.unk_06 = (D_80149968.unk_1C[1] >> 3) + 0x10;
            ((s16 *)&tmp)[1] = var_v0;
        }
    }
    if (((u16) D_801499B0[0].unk0 == 0) && (((u16 *)&D_801499A0)[1] != 0) && (((u16) ((u16 *)&D_801499A0)[1] % 45U) == 0)) {
        if (((u16 *)D_801499B0)[1] == 0) {
            ((u16 *)D_801499B0)[1] = 1;
            func_80044750(0x800);
        } else {
            ((u16 *)D_801499B0)[1] = 0;
            func_80044750(0x801);
        }
        ((u16 *)&D_801499A0)[1] += 1;
    }
}
