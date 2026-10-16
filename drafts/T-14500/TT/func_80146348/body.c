void func_80146348(void) {
    u8 *sp44;
    u8 *sp40;
    u8 *sp3C;
    u8 *sp38;
    u8 *sp34;
    u8 *sp30;
    u8 sp2B;
    s32 var_t3;
    s32 var_t5;
    s32 var_t6;
    u16 temp_a2;
    u16 temp_t0;
    u16 temp_t1;
    u16 temp_t4;
    u16 temp_t6;
    u16 temp_t6_2;
    u16 temp_t9;
    u16 temp_t9_2;
    u16 temp_v0;
    u16 temp_v0_2;
    u16 temp_v0_3;
    u16 temp_v0_4;
    u16 temp_v0_5;
    u16 var_a0;
    u16 var_v0;
    u16 var_v0_2;
    u8 *temp_a0;
    u8 *var_v1;
    u8 var_v0_3;

    var_v1 = D_80158A6C;
    sp44 = D_80158A60;
    sp40 = D_80158A64;
    temp_a0 = D_80158A60 + 0x10;
    sp38 = (*(u16 *)((unsigned char *)var_v1 + (24)) * 0x3C) + D_80158AA8;
    sp30 = temp_a0;
    sp34 = D_80158AA4;
    temp_v0 = *(u16 *)((unsigned char *)D_80158A64 + (4));
    switch (temp_v0) {
    case 0x0:
        *(s16 *)((unsigned char *)var_v1 + (26)) = 0;
        *(s16 *)((unsigned char *)var_v1 + (28)) = 0xA;
        *(s16 *)((unsigned char *)sp40 + (4)) = 0x40;
        break;
    case 0x40:
        temp_v0_2 = *(u16 *)((unsigned char *)temp_a0 + (4));
        if (temp_v0_2 & 0x8000) {
            sp3C = var_v1;
            func_80133AD0(0x615);
            *(u16 *)((unsigned char *)var_v1 + (28)) = (u16) (*(u16 *)((unsigned char *)var_v1 + (28)) - 1);
            if ((s16) *(u16 *)((unsigned char *)var_v1 + (28)) < 0) {
                *(u16 *)((unsigned char *)var_v1 + (28)) = 0x2FU;
            }
            goto block_11;
        }
        if (temp_v0_2 & 0x2000) {
            sp3C = var_v1;
            func_80133AD0(0x615);
            temp_t4 = *(u16 *)((unsigned char *)var_v1 + (28)) + 1;
            *(u16 *)((unsigned char *)var_v1 + (28)) = temp_t4;
            if ((u32) (temp_t4 & 0xFFFF) >= 0x30U) {
                *(u16 *)((unsigned char *)var_v1 + (28)) = 0U;
            }
block_11:
            *(s16 *)((unsigned char *)var_v1 + (16)) = 0;
        }
        temp_v0_3 = (*(s32 *)sp30);
        if (temp_v0_3 & 0x8000) {
            temp_t1 = *(u16 *)((unsigned char *)var_v1 + (16)) + 1;
            *(u16 *)((unsigned char *)var_v1 + (16)) = temp_t1;
            if ((u32) (temp_t1 & 0xFFFF) >= 0x1FU) {
                sp3C = var_v1;
                func_80133AD0(0x615);
                *(u16 *)((unsigned char *)var_v1 + (28)) = (u16) (*(u16 *)((unsigned char *)var_v1 + (28)) - 1);
                *(s16 *)((unsigned char *)var_v1 + (16)) = 0x1A;
                if ((s16) *(u16 *)((unsigned char *)var_v1 + (28)) < 0) {
                    *(u16 *)((unsigned char *)var_v1 + (28)) = 0x2FU;
                }
            }
        } else if (temp_v0_3 & 0x2000) {
            temp_t0 = *(u16 *)((unsigned char *)var_v1 + (16)) + 1;
            *(u16 *)((unsigned char *)var_v1 + (16)) = temp_t0;
            if ((u32) (temp_t0 & 0xFFFF) >= 0x1FU) {
                sp3C = var_v1;
                func_80133AD0(0x615);
                *(s16 *)((unsigned char *)var_v1 + (16)) = 0x1A;
                temp_t6 = *(u16 *)((unsigned char *)var_v1 + (28)) + 1;
                *(u16 *)((unsigned char *)var_v1 + (28)) = temp_t6;
                if ((u32) (temp_t6 & 0xFFFF) >= 0x30U) {
                    *(u16 *)((unsigned char *)var_v1 + (28)) = 0U;
                }
            }
        }
        var_a0 = *(u16 *)((unsigned char *)sp30 + (4));
        if (var_a0 & 4) {
            var_v0 = *(u16 *)((unsigned char *)var_v1 + (26));
            if (var_v0 != 0) {
                sp3C = var_v1;
                func_80133AD0(0x617);
                temp_t9 = *(u16 *)((unsigned char *)var_v1 + (26)) - 1;
                var_v0 = temp_t9 & 0xFFFF;
                *(u16 *)((unsigned char *)var_v1 + (26)) = temp_t9;
            }
            *(s16 *)((unsigned char *)var_v1 + (28)) = (s16) *(u8 *)((unsigned char *)(sp44 + (*(u16 *)((unsigned char *)var_v1 + (24)) * 6) + var_v0) + (50));
            var_a0 = *(u16 *)((unsigned char *)sp30 + (4));
            goto block_30;
        }
        var_t5 = var_a0 & 0x20;
        if (var_a0 & 8) {
            var_v0_2 = *(u16 *)((unsigned char *)var_v1 + (26));
            if (var_v0_2 < 2U) {
                sp3C = var_v1;
                func_80133AD0(0x617);
                temp_t9_2 = *(u16 *)((unsigned char *)var_v1 + (26)) + 1;
                var_v0_2 = temp_t9_2 & 0xFFFF;
                *(u16 *)((unsigned char *)var_v1 + (26)) = temp_t9_2;
            }
            *(s16 *)((unsigned char *)var_v1 + (28)) = (s16) *(u8 *)((unsigned char *)(sp44 + (*(u16 *)((unsigned char *)var_v1 + (24)) * 6) + var_v0_2) + (50));
            var_a0 = *(u16 *)((unsigned char *)sp30 + (4));
block_30:
            var_t5 = var_a0 & 0x20;
        }
        var_t6 = var_a0 & 0x40;
        if (var_t5 != 0) {
            sp3C = var_v1;
            func_80133AD0(0x616);
            temp_v0_4 = *(u16 *)((unsigned char *)var_v1 + (26));
            if (temp_v0_4 < 2U) {
                *(u16 *)((unsigned char *)var_v1 + (26)) = (u16) (temp_v0_4 + 1);
                *(s16 *)((unsigned char *)var_v1 + (28)) = 0xA;
                var_a0 = *(u16 *)((unsigned char *)sp30 + (4));
            } else {
                *(s8 *)((unsigned char *)sp40 + (10)) = 1;
                var_a0 = *(u16 *)((unsigned char *)sp30 + (4));
            }
            var_t6 = var_a0 & 0x40;
        }
        var_t3 = var_a0 & 0x800;
        if (var_t6 != 0) {
            var_t3 = var_a0 & 0x800;
            if (*(u16 *)((unsigned char *)var_v1 + (26)) != 0) {
                sp3C = var_v1;
                func_80133AD0(0x617);
                *(s8 *)((unsigned char *)(sp44 + (*(u16 *)((unsigned char *)var_v1 + (24)) * 6) + *(u16 *)((unsigned char *)var_v1 + (26))) + (50)) = 0x3F;
                temp_t6_2 = *(u16 *)((unsigned char *)var_v1 + (26)) - 1;
                *(u16 *)((unsigned char *)var_v1 + (26)) = temp_t6_2;
                *(s16 *)((unsigned char *)var_v1 + (28)) = (s16) *(u8 *)((unsigned char *)(sp44 + (*(u16 *)((unsigned char *)var_v1 + (24)) * 6) + (temp_t6_2 & 0xFFFF)) + (50));
                var_t3 = *(u16 *)((unsigned char *)sp30 + (4)) & 0x800;
            }
        }
        if (var_t3 != 0) {
            sp3C = var_v1;
            func_80133AD0(0x616);
            *(s8 *)((unsigned char *)sp40 + (10)) = 1;
        }
        *(s8 *)((unsigned char *)(sp44 + (*(u16 *)((unsigned char *)var_v1 + (24)) * 6) + *(u16 *)((unsigned char *)var_v1 + (26))) + (50)) = (s8) *(u16 *)((unsigned char *)var_v1 + (28));
        break;
    }
    temp_v0_5 = *(u16 *)((unsigned char *)var_v1 + (24));
    switch (temp_v0_5) {
    case 0:
        sp2B = 2;
        break;
    case 1:
        sp2B = 6;
        break;
    case 2:
        sp2B = 4;
        break;
    }
    var_v0_3 = 0xF;
    if (*(u16 *)((unsigned char *)sp44 + (10)) & 3) {
        var_v0_3 = sp2B;
    }
    *(u8 *)((unsigned char *)sp38 + (42)) = var_v0_3;
    *(u8 *)((unsigned char *)sp38 + (22)) = var_v0_3;
    *(u8 *)((unsigned char *)sp38 + (2)) = var_v0_3;
    temp_a2 = *(u16 *)((unsigned char *)var_v1 + (24));
    sp3C = var_v1;
    func_801486D8((s32) (sp44 + (temp_a2 * 6) + 0x32), temp_a2 + 0x2B);
    *(s16 *)((unsigned char *)sp34 + (20)) = (s16) ((*(u16 *)((unsigned char *)var_v1 + (26)) * 8) + 0x34);
    *(s16 *)((unsigned char *)sp34 + (22)) = (s16) ((*(u16 *)((unsigned char *)var_v1 + (24)) * 0x10) + 0x7E);
    (*(s32 *)sp34) = ((u16) *(u16 *)((unsigned char *)sp44 + (10)) >> 3) & 1;
}
