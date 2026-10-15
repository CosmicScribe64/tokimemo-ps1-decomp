void func_8014C84C(void) {
    s16 temp_t1;
    s32 var_s5;
    u8 *temp_s1;
    u8 *var_s0;
    u8 *temp_a1;
    u8 *temp_s2;
    u8 *temp_v0;
    u8 *temp_v0_2;

    temp_s1 = D_80158AB0;
    var_s0 = D_80158A98;
    var_s5 = 0;
    temp_s2 = (*(s32 *)((unsigned char *)D_80158A68 + (8)) * 0x80B8) + D_80158AB4;
    do {
        if ((*(s32 *)var_s0) != 0) {
            if (*(u16 *)((unsigned char *)var_s0 + (4)) == 0x71) {
                func_80147A18((s32) (var_s0 + 0x14), (s32) (var_s0 + 0x38), (s32) *(u16 *)((unsigned char *)var_s0 + (16)));
            } else if (*(u16 *)((unsigned char *)var_s0 + (2)) == 0x47) {
                temp_v0 = *(void **)((unsigned char *)temp_s2 + (4));
                *(void **)((unsigned char *)temp_s2 + (4)) = (void *) (temp_v0 + 0x30);
                *(s16 *)((unsigned char *)temp_s1 + (4)) = (s16) *(s16 *)((unsigned char *)var_s0 + (20));
                *(s16 *)((unsigned char *)temp_s1 + (6)) = (s16) *(s16 *)((unsigned char *)var_s0 + (22));
                *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (*(s16 *)((unsigned char *)var_s0 + (20)) - 8);
                *(s8 *)((unsigned char *)temp_v0 + (41)) = 0;
                *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (*(s16 *)((unsigned char *)var_s0 + (22)) - 8);
                *(s8 *)((unsigned char *)temp_v0 + (42)) = 1;
                *(s16 *)((unsigned char *)temp_v0 + (16)) = 0x10;
                *(s16 *)((unsigned char *)temp_v0 + (18)) = 0x10;
                *(u8 *)((unsigned char *)temp_v0 + (40)) = (u8) *(u8 *)((unsigned char *)var_s0 + (60));
                *(u8 *)((unsigned char *)temp_v0 + (12)) = (u8) *(u8 *)((unsigned char *)var_s0 + (92));
                *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) *(u8 *)((unsigned char *)var_s0 + (93));
                *(u16 *)((unsigned char *)temp_v0 + (22)) = (u16) *(u16 *)((unsigned char *)var_s0 + (88));
                *(s8 *)((unsigned char *)temp_v0 + (44)) = 0;
                *(u16 *)((unsigned char *)temp_v0 + (14)) = (u16) *(u16 *)((unsigned char *)var_s0 + (90));
                if (*(u16 *)((unsigned char *)var_s0 + (6)) == 2) {
                    *(s8 *)((unsigned char *)temp_v0 + (43)) = 0;
                    func_80141EBC((s32) (temp_s2 + (*(u16 *)((unsigned char *)var_s0 + (16)) * 4) + 0x8008), (s32) temp_v0, (s32) temp_s1);
                } else {
                    *(s8 *)((unsigned char *)temp_v0 + (43)) = 1;
                    temp_t1 = ((u32) (*(u16 *)((unsigned char *)var_s0 + (110)) - 0x400) >> 1) + 0xC00;
                    (*(s32 *)temp_s1) = temp_t1;
                    *(s16 *)((unsigned char *)temp_s1 + (2)) = temp_t1;
                    if ((*(s32 *)temp_s1) >= 0x1001) {
                        *(s8 *)((unsigned char *)temp_v0 + (43)) = 0;
                    }
                    func_80141EBC((s32) (temp_s2 + 0x8024), (s32) temp_v0, (s32) temp_s1);
                }
            } else {
                temp_v0_2 = *(void **)((unsigned char *)temp_s2 + (4));
                *(void **)((unsigned char *)temp_s2 + (4)) = (void *) (temp_v0_2 + 0x30);
                *(s16 *)((unsigned char *)temp_s1 + (4)) = (s16) *(s16 *)((unsigned char *)var_s0 + (20));
                *(s16 *)((unsigned char *)temp_s1 + (6)) = (s16) *(s16 *)((unsigned char *)var_s0 + (22));
                if (*(u16 *)((unsigned char *)var_s0 + (2)) == 0x42) {
                    *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) (*(s16 *)((unsigned char *)var_s0 + (20)) - 8);
                    *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) (*(s16 *)((unsigned char *)var_s0 + (22)) - 0x10);
                } else {
                    *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) (*(s16 *)((unsigned char *)var_s0 + (20)) - 8);
                    *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) (*(s16 *)((unsigned char *)var_s0 + (22)) - 8);
                }
                *(u8 *)((unsigned char *)temp_v0_2 + (12)) = (u8) *(u8 *)((unsigned char *)var_s0 + (92));
                *(u8 *)((unsigned char *)temp_v0_2 + (13)) = (u8) *(u8 *)((unsigned char *)var_s0 + (93));
                *(s8 *)((unsigned char *)temp_v0_2 + (41)) = 0;
                *(s8 *)((unsigned char *)temp_v0_2 + (42)) = 1;
                *(u8 *)((unsigned char *)temp_v0_2 + (40)) = (u8) *(u8 *)((unsigned char *)var_s0 + (60));
                *(u16 *)((unsigned char *)temp_v0_2 + (22)) = (u16) *(u16 *)((unsigned char *)var_s0 + (88));
                *(s8 *)((unsigned char *)temp_v0_2 + (44)) = 0;
                *(s8 *)((unsigned char *)temp_v0_2 + (43)) = 0;
                *(u16 *)((unsigned char *)temp_v0_2 + (14)) = (u16) *(u16 *)((unsigned char *)var_s0 + (90));
                func_80141C6C((s32) (temp_s2 + (*(u16 *)((unsigned char *)var_s0 + (16)) * 4) + 0x8008), (s32) temp_v0_2, (s32) temp_s1);
                if (*(u16 *)((unsigned char *)var_s0 + (2)) == 0x42) {
                    temp_a1 = *(void **)((unsigned char *)temp_s2 + (4));
                    *(void **)((unsigned char *)temp_s2 + (4)) = (void *) (temp_a1 + 0x30);
                    *(s16 *)((unsigned char *)temp_s1 + (4)) = (s16) *(s16 *)((unsigned char *)var_s0 + (20));
                    *(s16 *)((unsigned char *)temp_s1 + (6)) = (s16) *(s16 *)((unsigned char *)var_s0 + (22));
                    *(s16 *)((unsigned char *)temp_a1 + (8)) = (s16) (*(s16 *)((unsigned char *)var_s0 + (20)) - 8);
                    *(u8 *)((unsigned char *)temp_a1 + (13)) = 0x10U;
                    *(u8 *)((unsigned char *)temp_a1 + (12)) = 0xD0U;
                    *(s16 *)((unsigned char *)temp_a1 + (10)) = (s16) *(s16 *)((unsigned char *)var_s0 + (22));
                    *(s8 *)((unsigned char *)temp_a1 + (41)) = 0;
                    *(s8 *)((unsigned char *)temp_a1 + (42)) = 1;
                    *(u8 *)((unsigned char *)temp_a1 + (40)) = (u8) *(u8 *)((unsigned char *)var_s0 + (60));
                    *(u16 *)((unsigned char *)temp_a1 + (22)) = (u16) *(u16 *)((unsigned char *)var_s0 + (88));
                    *(s8 *)((unsigned char *)temp_a1 + (44)) = 0;
                    *(s8 *)((unsigned char *)temp_a1 + (43)) = 0;
                    *(u16 *)((unsigned char *)temp_a1 + (14)) = (u16) *(u16 *)((unsigned char *)var_s0 + (90));
                    func_80141C6C((s32) (temp_s2 + (*(u16 *)((unsigned char *)var_s0 + (16)) * 4) + 0x8008), (s32) temp_a1, (s32) temp_s1);
                }
            }
        }
        var_s5 += 1;
        var_s0 += 0x78;
    } while (var_s5 != 0x40);
}
