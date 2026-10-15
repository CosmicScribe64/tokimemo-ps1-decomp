void func_8013D988(void) {
    s32 *sp52C;
    void *sp524;
    s32 sp514;
    ? sp2BC;
    ? sp298;
    ? sp78;
    s32 sp70;
    s32 sp60;
    s32 sp54;
    ? *var_t1;
    ? *var_t2;
    s16 *var_s0;
    s16 *var_s1;
    s16 temp_t4;
    s16 temp_t7_2;
    s16 temp_v1;
    s32 temp_a0_4;
    s32 temp_lo;
    s32 temp_t6;
    s32 temp_t6_2;
    s32 temp_t7;
    s32 temp_v1_4;
    s32 var_s2;
    s32 var_s3;
    s32 var_t3;
    s8 temp_a0_3;
    s8 temp_t0;
    s8 temp_t6_3;
    s8 temp_v1_3;
    s8 var_a1;
    u8 *temp_s4;
    u8 temp_a0_2;
    u8 temp_a2;
    void *temp_a0;
    void *temp_a1;
    void *temp_a3;
    void *temp_v0;
    void *temp_v1_2;

    sp52C = D_80158A70;
    temp_s4 = D_80158A64;
    sp524 = (*(s32 *)((unsigned char *)D_80158A68 + (8)) * 0x80B8) + D_80158AB4;
    sp60 = 0;
    sp514 = 0;
    sp70 = 0;
    do {
        var_s2 = 0;
        var_s3 = 0;
        var_s1 = sp70 + &sp78;
        var_s0 = sp70 + &sp2BC;
loop_2:
        *var_s0 = (s16) ((s32) ((var_s2 - 8) * (0x8800 - *(s16 *)((unsigned char *)temp_s4 + (52)))) / 1920);
        *var_s1 = (s16) ((s32) ((sp514 - 8) * (0x8800 - *(s16 *)((unsigned char *)temp_s4 + (52)))) / 1920);
        *var_s0 += func_800A0140((*(u16 *)((unsigned char *)temp_s4 + (50)) << 5) + sp60) / (s16) *(s16 *)((unsigned char *)temp_s4 + (52));
        temp_t7_2 = *var_s1;
        temp_lo = func_800A0070((*(u16 *)((unsigned char *)temp_s4 + (50)) << 5) + var_s3) / (s16) *(s16 *)((unsigned char *)temp_s4 + (52));
        var_s2 += 1;
        var_s0 += 0x22;
        var_s1 += 0x22;
        var_s3 += 0x200;
        *(s16 *)((unsigned char *)var_s1 + (-34)) = (s16) (temp_t7_2 + (s16) temp_lo);
        if (var_s2 != 0x11) {
            goto loop_2;
        }
        temp_t7 = sp514 + 1;
        sp60 += 0x200;
        sp70 += 2;
        sp514 = temp_t7;
    } while (temp_t7 < 0x11);
    temp_v1 = 0x300 - *(u16 *)((unsigned char *)sp52C + (2));
    temp_t4 = temp_v1 & 0xF;
    sp54 = temp_v1 & ~0xF;
    sp514 = 0;
    do {
        var_t2 = &sp2BC;
        var_t1 = &sp78;
        temp_t6_2 = (sp514 / 16) * 2;
        var_t3 = sp514;
loop_6:
        temp_v0 = *(void **)((unsigned char *)sp524 + (4));
        temp_a1 = var_t1 + temp_t6_2;
        *(void **)((unsigned char *)sp524 + (4)) = (void *) (temp_v0 + 0x28);
        *(s8 *)((unsigned char *)temp_v0 + (7)) = 0x2D;
        *(s8 *)((unsigned char *)temp_v0 + (3)) = 9;
        temp_a0 = var_t2 + temp_t6_2;
        *(s16 *)((unsigned char *)temp_v0 + (34)) = (s16) ((*(s16 *)((unsigned char *)temp_a1 + (36)) - temp_t4) + 0x80);
        *(s16 *)((unsigned char *)temp_v0 + (26)) = (s16) ((*(s16 *)((unsigned char *)temp_a1 + (2)) - temp_t4) + 0x80);
        *(s16 *)((unsigned char *)temp_v0 + (18)) = (s16) ((*(s16 *)((unsigned char *)temp_a1 + (34)) - temp_t4) + 0x80);
        *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) ((temp_a1->unk0 - temp_t4) + 0x80);
        *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) temp_a0->unk0;
        *(s16 *)((unsigned char *)temp_v0 + (16)) = (s16) *(s16 *)((unsigned char *)temp_a0 + (34));
        *(s16 *)((unsigned char *)temp_v0 + (24)) = (s16) *(s16 *)((unsigned char *)temp_a0 + (2));
        *(s16 *)((unsigned char *)temp_v0 + (32)) = (s16) *(s16 *)((unsigned char *)temp_a0 + (36));
        temp_v1_2 = *(void **)((unsigned char *)temp_s4 + (24));
        temp_a3 = (*(temp_v1_2->unk0 + ((var_t3 + sp54) & 0x3FF)) * 3) + *(s32 *)((unsigned char *)temp_v1_2 + (4));
        temp_a0_2 = *(u8 *)((unsigned char *)temp_a3 + (1));
        temp_t0 = ((temp_a0_2 & 7) * 0x10) & 0xFF;
        temp_t6_3 = ((temp_a0_2 >> 3) * 0x10) & 0xFF;
        var_a1 = temp_t6_3;
        if (temp_a3->unk0 == 1) {
            var_a1 = (temp_t6_3 + 0x80) & 0xFF;
        }
        temp_v1_3 = temp_t0 + 0xF;
        temp_a0_3 = var_a1 + 0xF;
        *(s8 *)((unsigned char *)temp_v0 + (29)) = temp_a0_3;
        *(s8 *)((unsigned char *)temp_v0 + (37)) = temp_a0_3;
        *(s8 *)((unsigned char *)temp_v0 + (20)) = temp_v1_3;
        *(s8 *)((unsigned char *)temp_v0 + (36)) = temp_v1_3;
        *(s8 *)((unsigned char *)temp_v0 + (12)) = temp_t0;
        *(s8 *)((unsigned char *)temp_v0 + (13)) = var_a1;
        *(s8 *)((unsigned char *)temp_v0 + (21)) = var_a1;
        *(s8 *)((unsigned char *)temp_v0 + (28)) = temp_t0;
        *(s16 *)((unsigned char *)temp_v0 + (22)) = 0xE;
        temp_a2 = *(u8 *)((unsigned char *)temp_a3 + (2));
        *(s16 *)((unsigned char *)temp_v0 + (14)) = (s16) ((temp_a2 & 0xF) + (((temp_a2 >> 4) + 0xF0) << 6));
        temp_a0_4 = temp_v0->unk0;
        temp_v0->unk0 = (s32) (((u32) (((*(s32 *)((unsigned char *)sp524 + (32828)) & 0xFFFFFF) ^ temp_a0_4) << 8) >> 8) ^ temp_a0_4);
        temp_v1_4 = *(s32 *)((unsigned char *)sp524 + (32828));
        var_t1 += 0x22;
        var_t2 += 0x22;
        var_t3 += 1;
        *(s32 *)((unsigned char *)sp524 + (32828)) = (s32) (((u32) ((((s32) temp_v0 & 0xFFFFFF) ^ temp_v1_4) << 8) >> 8) ^ temp_v1_4);
        if (var_t1 != &sp298) {
            goto loop_6;
        }
        temp_t6 = sp514 + 0x10;
        sp514 = temp_t6;
    } while (temp_t6 != 0x100);
}
