void func_80137A78(void) {
    u8 *sp2C;
    u8 *sp28;
    s16 var_v0_2;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
    u16 temp_v0;
    u8 *temp_a2;
    u8 *var_a1;
    u8 *var_v0;
    u8 *var_v1;
    u8 *var_v1_2;
    u8 *var_v1_3;

    temp_a2 = D_80158AAC;
    var_a1 = D_80158A8C;
    if ((*(s32 *)D_80158A8C) != 0) {
        var_a0 = 0;
        var_v0 = D_80158A8C;
        do {
            var_a0 += 1;
            var_v0 += 0x14;
            *(s8 *)((unsigned char *)var_v0 + (46)) = 0;
        } while (var_a0 < 0x18);
        if (*(u8 *)((unsigned char *)var_a1 + (834)) == 0) {
            sp2C = var_a1;
            sp28 = temp_a2;
            var_a0_2 = 0;
            if (func_801427A8() != 0) {
                sp2C = var_a1;
                sp28 = temp_a2;
                func_80133AD0(0x508);
                var_a0_3 = 0;
                var_v0_2 = 0x3CDF;
                var_v1 = D_80158AA4 + 0xFA0;
                do {
                    var_a0_3 += 1;
                    var_v1 -= 0x64;
                    *(s16 *)((unsigned char *)var_v1 + (166)) = 0x3CDF;
                } while (var_a0_3 < 7);
                *(s16 *)((unsigned char *)D_80158AA4 + (4866)) = 0x3CDF;
                *(s16 *)((unsigned char *)D_80158AA4 + (4766)) = 0x3CDF;
                *(s16 *)((unsigned char *)D_80158AA4 + (4666)) = 0x3CDF;
                *(s16 *)((unsigned char *)D_80158AA4 + (4466)) = 0x3CDF;
                *(s16 *)((unsigned char *)D_80158AA4 + (4366)) = 0x3CDF;
                var_v1_2 = D_80158AA4 + 0x1068;
                *(s16 *)((unsigned char *)var_v1_2 + (66)) = 0x3CDF;
            } else {
                var_v0_2 = 0x3CD9;
                var_v1_3 = D_80158AA4 + 0xFA0;
                do {
                    var_a0_2 += 1;
                    var_v1_3 -= 0x64;
                    *(s16 *)((unsigned char *)var_v1_3 + (166)) = 0x3CD9;
                } while (var_a0_2 < 7);
                *(s16 *)((unsigned char *)D_80158AA4 + (4866)) = 0x3CD9;
                *(s16 *)((unsigned char *)D_80158AA4 + (4766)) = 0x3CD9;
                *(s16 *)((unsigned char *)D_80158AA4 + (4666)) = 0x3CD9;
                *(s16 *)((unsigned char *)D_80158AA4 + (4466)) = 0x3CD9;
                *(s16 *)((unsigned char *)D_80158AA4 + (4366)) = 0x3CD9;
                var_v1_2 = D_80158AA4 + 0x1068;
                *(s16 *)((unsigned char *)var_v1_2 + (66)) = 0x3CD9;
            }
            *(s16 *)((unsigned char *)var_v1_2 + (366)) = var_v0_2;
        }
        if (*(u8 *)((unsigned char *)var_a1 + (835)) == 0) {
            sp2C = var_a1;
            sp28 = temp_a2 + 0xEFC;
            func_801374C0((s32) temp_a2);
            func_801374C0((s32) sp28);
        }
        temp_v0 = *(u16 *)((unsigned char *)var_a1 + (782));
        if (temp_v0 == 1) {
            func_80137654();
            func_80137878();
        } else if (temp_v0 == 2) {
            func_80137878();
        }
    }
}
