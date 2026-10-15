void func_80143A44(void) {
    u8 *sp3C;
    u8 *sp38;
    u8 *sp30;
    u8 *sp2C;
    u8 *sp28;
    s32 temp_a0;
    s32 temp_t6;
    s32 temp_t9;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a0_4;
    s32 var_a0_5;
    s32 var_a0_6;
    s32 var_a3;
    s32 var_at;
    s32 var_t4;
    u16 temp_v0_6;
    u8 *temp_v0;
    u8 *var_t0;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 *var_v0_3;
    u8 *var_v0_4;
    u8 *var_v0_5;
    u8 *var_v0_7;
    u8 *var_v1;
    u8 *var_v1_2;
    u8 temp_t5;
    u8 temp_v0_2;
    u8 temp_v0_3;
    u8 temp_v0_4;
    u8 temp_v0_5;
    u8 var_v0_6;

    var_t0 = D_80158A74;
    temp_v0 = D_80158A60 + 0x20;
    if (*(u16 *)((unsigned char *)D_80158A60 + (32)) & 0x8000) {
        temp_v0_2 = *(u8 *)((unsigned char *)var_t0 + (88));
        *(s32 *)((unsigned char *)var_t0 + (40)) = -1;
        *(u8 *)((unsigned char *)var_t0 + (89)) = 0U;
        if (temp_v0_2 < 8U) {
            *(u8 *)((unsigned char *)var_t0 + (88)) = (u8) (temp_v0_2 + 1);
        }
    } else if (*(u16 *)((unsigned char *)D_80158A60 + (32)) & 0x2000) {
        temp_v0_3 = *(u8 *)((unsigned char *)var_t0 + (89));
        *(s32 *)((unsigned char *)var_t0 + (40)) = 1;
        *(u8 *)((unsigned char *)var_t0 + (88)) = 0U;
        if (temp_v0_3 < 8U) {
            *(u8 *)((unsigned char *)var_t0 + (89)) = (u8) (temp_v0_3 + 1);
        }
    } else {
        temp_v0_4 = *(u8 *)((unsigned char *)var_t0 + (88));
        *(s32 *)((unsigned char *)var_t0 + (40)) = 0;
        if (temp_v0_4 != 0) {
            *(u8 *)((unsigned char *)var_t0 + (88)) = (u8) (temp_v0_4 - 1);
        }
        temp_v0_5 = *(u8 *)((unsigned char *)var_t0 + (89));
        if (temp_v0_5 != 0) {
            *(u8 *)((unsigned char *)var_t0 + (89)) = (u8) (temp_v0_5 - 1);
        }
    }
    temp_v0_6 = *(u16 *)((unsigned char *)D_80158A60 + (32));
    if (temp_v0_6 & 0x1000) {
        *(s32 *)((unsigned char *)var_t0 + (44)) = -1;
    } else if (temp_v0_6 & 0x4000) {
        *(s32 *)((unsigned char *)var_t0 + (44)) = 1;
    } else {
        *(s32 *)((unsigned char *)var_t0 + (44)) = 0;
    }
    temp_v1 = *(s32 *)((unsigned char *)var_t0 + (40));
    if ((temp_v1 != 0) && (temp_a0 = *(s32 *)((unsigned char *)var_t0 + (44)), (temp_a0 != 0))) {
        temp_t6 = *(u8 *)((unsigned char *)var_t0 + (84)) * 0x6000;
        *(s32 *)((unsigned char *)var_t0 + (40)) = (s32) (temp_v1 * temp_t6);
        *(s32 *)((unsigned char *)var_t0 + (44)) = (s32) (temp_a0 * temp_t6);
    } else {
        temp_t9 = *(u8 *)((unsigned char *)var_t0 + (84)) << 0xF;
        *(s32 *)((unsigned char *)var_t0 + (40)) = (s32) (temp_v1 * temp_t9);
        *(s32 *)((unsigned char *)var_t0 + (44)) = (s32) (*(s32 *)((unsigned char *)var_t0 + (44)) * temp_t9);
    }
    if (*(u16 *)((unsigned char *)temp_v0 + (4)) & 1) {
        var_v0 = D_80158A80;
        var_a0 = 0;
loop_21:
        var_a0 += 1;
        if (*(u8 *)((unsigned char *)var_t0 + (85)) == 0xA) {
            var_at = var_a0 < 2;
            if ((var_v0->unk0 == 0) && (var_at = var_a0 < 2, (*(u8 *)((unsigned char *)var_v0 + (168)) == 0)) && (var_at = var_a0 < 2, (*(u8 *)((unsigned char *)var_v0 + (336)) == 0))) {
                sp30 = var_v0;
                sp3C = temp_v0;
                sp38 = var_t0;
                func_80133AD0(0x608, 0xA, (s32) temp_v0);
                var_v0->unk0 = 1;
                *(s16 *)((unsigned char *)var_v0 + (4)) = 0x20;
                *(s8 *)((unsigned char *)var_v0 + (13)) = 0;
                *(s8 *)((unsigned char *)var_v0 + (14)) = 0;
                *(s8 *)((unsigned char *)var_v0 + (15)) = 0;
                *(s16 *)((unsigned char *)var_v0 + (6)) = 0;
                *(s16 *)((unsigned char *)var_v0 + (8)) = 0;
                *(s16 *)((unsigned char *)var_v0 + (10)) = 0;
                *(u8 *)((unsigned char *)var_v0 + (168)) = 1U;
                *(s16 *)((unsigned char *)var_v0 + (172)) = 0x20;
                *(u8 *)((unsigned char *)var_v0 + (336)) = 1U;
                *(s16 *)((unsigned char *)var_v0 + (340)) = 0x20;
            } else {
                goto block_29;
            }
        } else {
            var_at = var_a0 < 2;
            if (var_v0->unk0 == 0) {
                sp30 = var_v0;
                sp3C = temp_v0;
                sp38 = var_t0;
                func_80133AD0(0x608, 0xA, (s32) temp_v0);
                var_v0->unk0 = 1;
                *(s16 *)((unsigned char *)var_v0 + (4)) = 0x20;
                *(s8 *)((unsigned char *)var_v0 + (13)) = 0;
                *(s8 *)((unsigned char *)var_v0 + (14)) = 0;
                *(s8 *)((unsigned char *)var_v0 + (15)) = 0;
                *(s16 *)((unsigned char *)var_v0 + (6)) = 0;
                *(s16 *)((unsigned char *)var_v0 + (8)) = 0;
                *(s16 *)((unsigned char *)var_v0 + (10)) = 0;
            } else {
block_29:
                var_v0 += 0x54;
                if (var_at == 0) {

                } else {
                    goto loop_21;
                }
            }
        }
        if (*(u8 *)((unsigned char *)var_t0 + (83)) != 0) {
            var_a0_2 = 6;
            var_v0_2 = D_80158A80 + 0x1F8;
loop_33:
            var_a0_2 += 1;
            if (var_v0_2->unk0 == 0) {
                var_v0_2->unk0 = 1;
                *(s16 *)((unsigned char *)var_v0_2 + (4)) = 0x20;
                *(s16 *)((unsigned char *)var_v0_2 + (10)) = 0;
                *(s16 *)((unsigned char *)var_v0_2 + (8)) = 0;
                *(s16 *)((unsigned char *)var_v0_2 + (6)) = 0;
                *(s8 *)((unsigned char *)var_v0_2 + (15)) = 0;
                *(s8 *)((unsigned char *)var_v0_2 + (14)) = 0;
                *(s8 *)((unsigned char *)var_v0_2 + (13)) = 0;
            } else {
                var_v0_2 += 0x54;
                if (var_a0_2 < 8) {
                    goto loop_33;
                }
            }
            var_a0_3 = 8;
            var_v0_3 = D_80158A80 + 0x2A0;
loop_37:
            var_a0_3 += 1;
            if (var_v0_3->unk0 == 0) {
                var_v0_3->unk0 = 1;
                *(s16 *)((unsigned char *)var_v0_3 + (4)) = 0x20;
                *(s16 *)((unsigned char *)var_v0_3 + (10)) = 0;
                *(s16 *)((unsigned char *)var_v0_3 + (8)) = 0;
                *(s16 *)((unsigned char *)var_v0_3 + (6)) = 0;
                *(s8 *)((unsigned char *)var_v0_3 + (15)) = 0;
                *(s8 *)((unsigned char *)var_v0_3 + (14)) = 0;
                *(s8 *)((unsigned char *)var_v0_3 + (13)) = 0;
            } else {
                var_v0_3 += 0x54;
                if (var_a0_3 < 0xA) {
                    goto loop_37;
                }
            }
            var_a0_4 = 0xA;
            var_v0_4 = D_80158A80 + 0x348;
loop_41:
            var_a0_4 += 1;
            if (var_v0_4->unk0 == 0) {
                var_v0_4->unk0 = 1;
                *(s16 *)((unsigned char *)var_v0_4 + (4)) = 0x20;
                *(s16 *)((unsigned char *)var_v0_4 + (10)) = 0;
                *(s16 *)((unsigned char *)var_v0_4 + (8)) = 0;
                *(s16 *)((unsigned char *)var_v0_4 + (6)) = 0;
                *(s8 *)((unsigned char *)var_v0_4 + (15)) = 0;
                *(s8 *)((unsigned char *)var_v0_4 + (14)) = 0;
                *(s8 *)((unsigned char *)var_v0_4 + (13)) = 0;
            } else {
                var_v0_4 += 0x54;
                if (var_a0_4 != 0xC) {
                    goto loop_41;
                }
            }
        }
    }
    if (*(u16 *)((unsigned char *)temp_v0 + (4)) & 2) {
        var_v1 = D_80158A84;
        var_v0_5 = D_80158A88;
        var_t4 = 0;
        var_a0_5 = 0;
loop_46:
        var_a0_5 += 1;
        if ((var_v0_5->unk0 == 0) && (var_v1->unk0 == 0) && ((*(u8 *)((unsigned char *)var_t0 + (92)) == 0) || (*(u8 *)((unsigned char *)var_t0 + (93)) == 0))) {
            sp2C = var_v0_5;
            sp28 = var_v1;
            sp38 = var_t0;
            func_80133AD0(0x609, M2C_ERROR(/* Read from unset register $a1 */), 2);
            var_v0_5->unk0 = 1;
            *(s16 *)((unsigned char *)var_v0_5 + (4)) = 0x20;
            *(s8 *)((unsigned char *)var_v0_5 + (13)) = 0;
            *(s8 *)((unsigned char *)var_v0_5 + (14)) = 0;
            *(s8 *)((unsigned char *)var_v0_5 + (15)) = 0;
            *(s16 *)((unsigned char *)var_v0_5 + (6)) = 0;
            *(s16 *)((unsigned char *)var_v0_5 + (8)) = 0;
            *(s16 *)((unsigned char *)var_v0_5 + (10)) = 0;
            var_v1->unk0 = 1;
            *(s8 *)((unsigned char *)var_v1 + (1)) = 1;
            *(s16 *)((unsigned char *)var_v1 + (62)) = 1;
            *(s16 *)((unsigned char *)var_v1 + (4)) = 0x20;
            *(s8 *)((unsigned char *)var_v1 + (13)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (14)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (15)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (6)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (8)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (10)) = 0;
            *(u8 *)((unsigned char *)var_v1 + (61)) = (u8) *(u8 *)((unsigned char *)var_t0 + (96));
            var_v0_6 = *(u8 *)((unsigned char *)var_t0 + (83));
            var_t4 = 1 & 0xFF;
        } else {
            var_v0_5 += 0x58;
            var_v1 += 0x58;
            if (var_a0_5 == 2) {
                var_v0_6 = *(u8 *)((unsigned char *)var_t0 + (83));
            } else {
                goto loop_46;
            }
        }
        var_a3 = 0xB0;
        if (var_v0_6 != 0) {
            do {
                var_a0_6 = 0;
                var_v1_2 = &D_80158A84[var_a3];
                var_v0_7 = &D_80158A88[var_a3];
loop_56:
                var_a0_6 += 1;
                if ((var_v0_7->unk0 == 0) && (var_v1_2->unk0 == 0) && ((*(u8 *)((unsigned char *)var_t0 + (92)) == 0) || (*(u8 *)((unsigned char *)var_t0 + (93)) == 0))) {
                    var_v0_7->unk0 = 1;
                    *(s16 *)((unsigned char *)var_v0_7 + (4)) = 0x20;
                    *(s16 *)((unsigned char *)var_v0_7 + (10)) = 0;
                    *(s16 *)((unsigned char *)var_v0_7 + (8)) = 0;
                    *(s16 *)((unsigned char *)var_v0_7 + (6)) = 0;
                    *(s8 *)((unsigned char *)var_v0_7 + (15)) = 0;
                    *(s8 *)((unsigned char *)var_v0_7 + (14)) = 0;
                    *(s8 *)((unsigned char *)var_v0_7 + (13)) = 0;
                    var_v1_2->unk0 = 1;
                    *(s8 *)((unsigned char *)var_v1_2 + (1)) = 1;
                    *(s16 *)((unsigned char *)var_v1_2 + (62)) = 1;
                    *(s16 *)((unsigned char *)var_v1_2 + (4)) = 0x20;
                    *(s8 *)((unsigned char *)var_v1_2 + (13)) = 0;
                    *(s8 *)((unsigned char *)var_v1_2 + (14)) = 0;
                    *(s8 *)((unsigned char *)var_v1_2 + (15)) = 0;
                    *(s16 *)((unsigned char *)var_v1_2 + (6)) = 0;
                    *(s16 *)((unsigned char *)var_v1_2 + (8)) = 0;
                    *(s16 *)((unsigned char *)var_v1_2 + (10)) = 0;
                    var_t4 = 1;
                    *(u8 *)((unsigned char *)var_v1_2 + (61)) = (u8) *(u8 *)((unsigned char *)var_t0 + (96));
                } else {
                    var_v0_7 += 0x58;
                    var_v1_2 += 0x58;
                    if (var_a0_6 != 2) {
                        goto loop_56;
                    }
                }
                var_a3 += 0xB0;
            } while (var_a3 != 0x2C0);
        }
        if (var_t4 != 0) {
            if (*(u8 *)((unsigned char *)var_t0 + (96)) != 0) {
                *(u8 *)((unsigned char *)var_t0 + (93)) = 1U;
            } else {
                *(u8 *)((unsigned char *)var_t0 + (92)) = 1U;
            }
            temp_t5 = *(u8 *)((unsigned char *)var_t0 + (96)) + 1;
            *(u8 *)((unsigned char *)var_t0 + (96)) = temp_t5;
            *(u8 *)((unsigned char *)var_t0 + (96)) = (u8) (temp_t5 & 1);
        }
    }
}
