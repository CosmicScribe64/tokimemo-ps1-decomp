extern ? D_8015676C;

void func_8014A2B4(void) {
    u8 *sp44;
    s32 sp30;
    u16 temp_t7;
    u16 temp_v0_2;
    u16 var_t1;
    u8 *temp_s0;
    u8 *temp_v0;
    u8 temp_v1;

    temp_s0 = D_80158A64;
    temp_t7 = *(u16 *)((unsigned char *)temp_s0 + (2));
    temp_v0 = D_80158AA4 + 0xC80;
    switch (temp_t7) {                              /* switch 1 */
    case 0:                                         /* switch 1 */
        if (*(u16 *)((unsigned char *)D_80158A60 + (164)) != 0) {
            func_8014304C(0x80016000, 0xC812, 0x2420);
            func_8014304C(0x8002E800, 0xC7C1, 0x69E4);
            func_8014304C(0x8019A000, 0xC7C5, 0x27D60);
            func_8014304C(0x8001A000, 0xBFD1, 0xE20);
            func_8014304C(0x801C2000, 0xBF9C, 0x1A680);
        }
        *(u16 *)((unsigned char *)temp_s0 + (2)) = 1U;
        break;
    case 1:                                         /* switch 1 */
        sp44 = D_80158A6C;
        func_80044750(0x22);
        func_80044750(0x2F);
        func_80044750(0x205);
        func_8014CBD0();
        func_80148210();
        func_80143570();
        func_8014A7E0();
        func_80146BE0();
        func_80148DB0();
        func_80147CE0();
        func_8014AF3C();
        func_80132F4C();
        func_80133B70();
        func_8013BD4C();
        func_801473BC();
        func_801426F0();
        *(s8 *)((unsigned char *)temp_s0 + (17)) = 0;
        *(s8 *)((unsigned char *)temp_s0 + (36)) = 0;
        *(? **)((unsigned char *)temp_s0 + (24)) = &D_8015676C;
        *(s8 *)((unsigned char *)temp_s0 + (28)) = 0x20;
        func_80149704();
        func_801498C8();
        func_80149D64();
        *(s16 *)((unsigned char *)sp44 + (16)) = 0;
        *(s8 *)((unsigned char *)sp44 + (10)) = 0;
        *(u16 *)((unsigned char *)temp_s0 + (38)) = 0U;
        *(u16 *)((unsigned char *)temp_s0 + (2)) = (u16) (*(u16 *)((unsigned char *)temp_s0 + (2)) + 1);
        *(s16 *)((unsigned char *)temp_s0 + (4)) = 0x40;
        break;
    case 2:                                         /* switch 1 */
        if (*(u16 *)((unsigned char *)D_80158A60 + (164)) != 0) {
            sp30 = func_800460DC();
            if ((((sp30 - func_801430C4()) + 0x100) & 0xFF) >= 5) {
                func_80044750(0x12);
                *(u16 *)((unsigned char *)temp_s0 + (2)) = (u16) (*(u16 *)((unsigned char *)temp_s0 + (2)) + 1);
            }
        } else {
            *(u16 *)((unsigned char *)temp_s0 + (2)) = 4U;
        }
        func_80149E44();
        func_80149BB0();
        break;
    case 3:                                         /* switch 1 */
        if (*(u16 *)((unsigned char *)D_80158A60 + (164)) != 0) {
            *(u16 *)((unsigned char *)D_80158A60 + (164)) = 0U;
            if (func_80044F94(1) != 0) {
                *(u16 *)((unsigned char *)temp_s0 + (2)) = (u16) (*(u16 *)((unsigned char *)temp_s0 + (2)) + 1);
            }
        }
        func_80149E44();
        func_80149BB0();
        break;
    case 4:                                         /* switch 1 */
        if ((u16) *(u16 *)((unsigned char *)D_80158A64 + (38)) >= 0x708U) {
            *(s8 *)((unsigned char *)temp_s0 + (36)) = 0;
            *(s8 *)((unsigned char *)temp_s0 + (34)) = 1;
            *(s8 *)((unsigned char *)D_80158A60 + (82)) = 0xFF;
            *(u8 **)((unsigned char *)D_80158A60 + (76)) = D_80150A50;
            temp_s0->unk0 = 2;
            *(u8 *)((unsigned char *)temp_s0 + (9)) = 0U;
            *(s8 *)((unsigned char *)temp_s0 + (10)) = 0;
            *(s8 *)((unsigned char *)temp_s0 + (11)) = 0;
            *(u16 *)((unsigned char *)temp_s0 + (2)) = 0U;
            *(s16 *)((unsigned char *)temp_s0 + (4)) = 0;
            *(s16 *)((unsigned char *)temp_s0 + (6)) = 0;
        }
        func_80149E44();
        func_80149BB0();
        func_801494D8();
        *(u16 *)((unsigned char *)temp_s0 + (38)) = (u16) (*(u16 *)((unsigned char *)temp_s0 + (38)) + 1);
        break;
    case 5:                                         /* switch 1 */
        *((*(u8 *)((unsigned char *)D_80158A6C + (10)) * 0x14) + D_80158AA8) = *(u16 *)((unsigned char *)D_80158A60 + (8)) & 1;
        if (*(u8 *)((unsigned char *)D_80158A6C + (10)) == 0) {
            func_80149EA0();
        } else {
            temp_v0_2 = *(u16 *)((unsigned char *)D_80158A6C + (16));
            var_t1 = temp_v0_2 + 1;
            if (temp_v0_2 >= 0x1EU) {
                *(u16 *)((unsigned char *)D_80158A6C + (16)) = 0U;
                *(u16 *)((unsigned char *)temp_s0 + (2)) = 9U;
                var_t1 = *(u16 *)((unsigned char *)D_80158A6C + (16)) + 1;
            }
            *(u16 *)((unsigned char *)D_80158A6C + (16)) = var_t1;
        }
        break;
    case 6:                                         /* switch 1 */
        func_8014A000();
        break;
    case 7:                                         /* switch 1 */
        *D_80158A74 = 0;
        *(s32 *)((unsigned char *)temp_v0 + (32)) = 0x100000;
        *(s32 *)((unsigned char *)temp_v0 + (36)) = 0x6C0000;
        *(s16 *)((unsigned char *)temp_v0 + (20)) = (s16) *(s16 *)((unsigned char *)temp_v0 + (34));
        *(s16 *)((unsigned char *)temp_v0 + (82)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (90)) = 0x180;
        *(s8 *)((unsigned char *)D_80158AA4 + (3200)) = 1;
        *(s8 *)((unsigned char *)temp_v0 + (1)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (68)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (59)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0 + (77)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (78)) = 1;
        *(s8 *)((unsigned char *)temp_v0 + (80)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (70)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0 + (71)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (79)) = 1;
        *(s16 *)((unsigned char *)temp_v0 + (60)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0 + (62)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0 + (64)) = 0x1A;
        *(s16 *)((unsigned char *)temp_v0 + (66)) = 0x3D04;
        *(s8 *)((unsigned char *)temp_v0 + (76)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (16)) = 2;
        *(s32 *)((unsigned char *)temp_v0 + (40)) = 0;
        *(s32 *)((unsigned char *)temp_v0 + (44)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (22)) = (s16) *(s16 *)((unsigned char *)temp_v0 + (38));
        *(s16 *)((unsigned char *)D_80158AB0 + (4)) = (s16) *(s16 *)((unsigned char *)temp_v0 + (20));
        *(s16 *)((unsigned char *)D_80158AB0 + (2)) = 0x1000;
        *(s16 *)((unsigned char *)D_80158AB0 + (6)) = (s16) *(s16 *)((unsigned char *)temp_v0 + (22));
        D_80158AB0->unk0 = (s16) *(s16 *)((unsigned char *)D_80158AB0 + (2));
        *(u8 **)((unsigned char *)temp_v0 + (72)) = (u8 *) D_80158AB0;
        func_80133AD0(0x614);
        *(u16 *)((unsigned char *)temp_s0 + (2)) = (u16) (*(u16 *)((unsigned char *)temp_s0 + (2)) + 1);
        break;
    case 8:                                         /* switch 1 */
        func_8014A198();
        break;
    case 9:                                         /* switch 1 */
        temp_v1 = *(u8 *)((unsigned char *)D_80158A6C + (10));
        switch (temp_v1) {                          /* switch 2; irregular */
        case 0:                                     /* switch 2 */
            temp_s0->unk0 = 2;
            break;
        case 1:                                     /* switch 2 */
            temp_s0->unk0 = 7;
            break;
        case 2:                                     /* switch 2 */
            temp_s0->unk0 = 3;
            break;
        case 3:                                     /* switch 2 */
            temp_s0->unk0 = 8;
            break;
        }
        *(u8 *)((unsigned char *)temp_s0 + (9)) = 1U;
        break;
    }
    if (*(u8 *)((unsigned char *)temp_s0 + (9)) != 0) {
        *(s16 *)((unsigned char *)temp_s0 + (6)) = 0;
        *(s16 *)((unsigned char *)temp_s0 + (4)) = 0;
        *(u16 *)((unsigned char *)temp_s0 + (2)) = 0U;
        *(s8 *)((unsigned char *)temp_s0 + (11)) = 0;
        *(s8 *)((unsigned char *)temp_s0 + (10)) = 0;
        *(u8 *)((unsigned char *)temp_s0 + (9)) = 0U;
    }
}
