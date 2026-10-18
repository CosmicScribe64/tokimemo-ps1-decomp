void func_80143758(void) {
    s16 var_a2;
    s32 var_a0;
    u8 *var_a3;
    DisplayFixActor44 *var_v1;
    u8 *var_a1;

    var_a0 = D_80150144;
    var_a1 = (var_a0 * 2) + D_801503C8;
    var_a3 = D_80120650;
    var_v1 = (DisplayFixActor44 *)D_8011ECD0;
    var_a2 = 0x8C;
    do {
        var_v1[96].unk_18 = (s16) (*(u8 *)var_a1);
        var_v1[96].unk_01 = 0;
        var_v1[96].unk_02 = 0x40;
        var_v1[96].unk_38 = 0;
        var_v1[96].unk_03 = 0xA4;
        var_v1[96].unk_0C = (u8 *)0x801F86EC;
        var_v1[96].unk_10 = (u8 *)0x801F897C;
        var_v1[96].unk_34 = (u8 *)0x801F86A4;
        var_v1[96].unk_04 = 8;
        var_v1[96].unk_43 = 0xF;
        var_v1[96].unk_05 = 0x80;
        var_v1[96].unk_24.h[1] = (s16) (*(u8 *)((unsigned char *)var_a1 + (1)) - 0xA0);
        var_v1[96].unk_28.h[1] = var_a2;
        /* The original clears only the low halfword of this word view. */
        (*(s16 *)&var_v1[96].unk_2C) = 0;
        var_v1[96].unk_30 = -1;
        var_v1[96].unk_07 = 0x80;
        if (var_v1[96].unk_18 >= 0xF1) {
            *(s8 *)((unsigned char *)var_a3 + (3)) = 0x34;
        } else {
            *(s8 *)((unsigned char *)var_a3 + (3)) = 0xB4;
        }
        var_a3 += 0x44;
        var_v1++;
        var_a2 += 0x28;
        var_a0 += 1;
        var_a1 += 2;
    } while (var_a3 != D_80120650 + 7 * 0x44);
    D_80150144 = var_a0;
}
