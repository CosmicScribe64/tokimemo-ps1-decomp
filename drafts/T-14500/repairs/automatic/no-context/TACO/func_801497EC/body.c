void func_801497EC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp78[8];
    s16 sp74;
    s16 sp72;
    s16 sp70;
    s16 sp68;
    s32 sp5C;
    s32 sp58;
    s32 sp54;
    s32 sp4C;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a0_4;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    u8 var_a0;
    u8 *temp_s2;
    u8 *temp_v0;
    u8 *temp_v0_2;
    u8 *temp_v0_3;

    temp_v1 = arg1 * 2;
    sp5C = arg1 + 0x10;
    temp_v0 = arg2 + (temp_v1 * 2);
    sp58 = (s32) (*(s32 *)temp_v0);
    sp54 = (s32) *(s16 *)((unsigned char *)temp_v0 + (2));
    *(u8 *)((unsigned char *)(D_8015EDB4 + (arg1 * 0x88)) + (2308)) = (u8) *(u8 *)(arg3 + arg1);
    (*(s32 *)&sp78) = (s32) (*(s32 *)&D_80125C60);
    *(s32 *)((unsigned char *)(&sp78) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (4));
    *(s32 *)((unsigned char *)(&sp78) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (8));
    *(s32 *)((unsigned char *)(&sp78) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (12));
    *(s32 *)((unsigned char *)(&sp78) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (16));
    *(s32 *)((unsigned char *)(&sp78) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (20));
    *(s32 *)((unsigned char *)(&sp78) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (24));
    *(s32 *)((unsigned char *)(&sp78) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (28));
    sp4C = temp_v1;
    func_8015D0D0((s32) (D_80128880 << 0xC) / 360, &sp78);
    func_8015CF30((s32) (D_80128884 << 0xC) / 360, &sp78);
    func_8015D270((s32) (D_80128882 << 0xC) / 360, &sp78);
    temp_s2 = (arg0 * 0x50) + &D_80127480;
    sp70 = (s16) *(s32 *)((unsigned char *)temp_s2 + (24));
    sp72 = (s16) *(s32 *)((unsigned char *)temp_s2 + (28));
    sp74 = (s16) *(s32 *)((unsigned char *)temp_s2 + (32));
    func_800A0ECC(&sp78, &sp70, &sp68);
    temp_v1_2 = sp5C * 0x88;
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_v1_2) + (116)) = (s16) (D_80127498 + sp68 + sp58);
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_v1_2) + (118)) = (s16) (D_8012749C + sp6A + sp54);
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_v1_2) + (120)) = (s16) (D_801274A0 + sp6C);
    temp_v0_2 = temp_v1_2 + D_8015EDB4;
    var_a0 = *(u8 *)((unsigned char *)temp_v0_2 + (133));
    if ((var_a0 < 0x15U) && (*(s16 *)((unsigned char *)temp_v0_2 + (130)) < *(s16 *)((unsigned char *)temp_v0_2 + (122)))) {
        *(u8 *)((unsigned char *)temp_v0_2 + (133)) = (u8) *(u8 *)(sp4C + arg4);
        temp_v0_3 = temp_v1_2 + D_8015EDB4;
        *(s16 *)((unsigned char *)temp_v0_3 + (130)) = (s16) (*(s16 *)((unsigned char *)temp_v0_3 + (130)) + 1);
        *(s8 *)((unsigned char *)D_8015EDB4 + (2446)) = 1;
        var_a0 = *(u8 *)((unsigned char *)(D_8015EDB4 + temp_v1_2) + (133));
    }
    if (var_a0 == 0) {
        func_8015435C(arg0, 9);
        var_s0 = 4;
        if (temp_s2 == &D_801274D0) {
            var_a0_2 = 4;
            do {
                func_8015435C(var_a0_2, 9);
                var_s0 += 3;
                var_a0_2 = var_s0;
            } while (var_s0 != 0x16);
        }
        var_s0_2 = 5;
        if (temp_s2 == &D_80127520) {
            var_a0_3 = 5;
            do {
                func_8015435C(var_a0_3, 9);
                var_s0_2 += 3;
                var_a0_3 = var_s0_2;
            } while (var_s0_2 != 0x17);
        }
        var_s0_3 = 6;
        if (temp_s2 == &D_80127570) {
            var_a0_4 = 6;
            do {
                func_8015435C(var_a0_4, 9);
                var_s0_3 += 3;
                var_a0_4 = var_s0_3;
            } while (var_s0_3 != 0x18);
        }
    }
}
