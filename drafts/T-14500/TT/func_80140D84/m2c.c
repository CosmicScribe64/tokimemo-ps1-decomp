void func_80140D84(void) {
    u8 *sp34;
    u8 *sp2C;
    u8 *sp28;
    u16 temp_v0;
    u16 temp_v0_2;
    u16 temp_v0_3;
    u8 *temp_a1;
    u8 *temp_v1;

    temp_a1 = D_80158A8C;
    temp_v0 = *(u16 *)((unsigned char *)temp_a1 + (4));
    temp_v1 = D_80158A74;
    switch (temp_v0) {                              /* switch 1; irregular */
    case 0x0:                                       /* switch 1 */
        sp2C = temp_a1;
        sp34 = D_80158A64;
        func_80142570(M2C_ERROR(/* Read from unset register $a0 */));
        if (*(u16 *)((unsigned char *)sp34 + (4)) == 0) {
            sp2C = temp_a1;
            func_80044750(0xB1);
            func_800451E0(0x8019A000);
            func_80044750(0x12);
            func_80044750(0x23);
            func_80044750(0x2E);
            func_80044750(0x21);
            func_80044750(4);
            func_80044750(0x200);
        }
        *(s16 *)((unsigned char *)sp34 + (50)) = 0;
        *(s16 *)((unsigned char *)sp34 + (52)) = 0x1000;
        sp2C = temp_a1;
        func_8013F3B4();
        *(s16 *)((unsigned char *)temp_a1 + (4)) = 0x20;
        break;
    case 0x20:                                      /* switch 1 */
        if (*D_80158A6C == 0) {
            *(s8 *)((unsigned char *)D_80158A64 + (28)) = 0x10;
        }
        func_8013F588();
        func_8013F878();
        break;
    case 0x30:                                      /* switch 1 */
        if (*D_80158A6C == 0) {
            *(s8 *)((unsigned char *)D_80158A64 + (28)) = 0x10;
        }
        func_8013F5F8();
        func_8013F878();
        break;
    case 0x40:                                      /* switch 1 */
        if (*D_80158A6C == 0) {
            *(s8 *)((unsigned char *)D_80158A64 + (28)) = 0x10;
        }
        sp2C = temp_a1;
        func_8013F7B0();
        func_8013F878();
        func_80140CBC();
        if (*(s16 *)((unsigned char *)temp_a1 + (780)) < 0) {
            temp_v0_2 = *(u16 *)((unsigned char *)temp_a1 + (782));
            switch (temp_v0_2) {                    /* switch 2; irregular */
            case 0:                                 /* switch 2 */
                if (*(u16 *)((unsigned char *)temp_a1 + (6)) == 2) {
                    if ((u16) *(u16 *)((unsigned char *)temp_a1 + (8)) < 0x80U) {
                        *(u16 *)((unsigned char *)temp_a1 + (8)) = 0x80U;
                    }
                } else {
                    *(u16 *)((unsigned char *)temp_a1 + (782)) = 0x10U;
                    *(u16 *)((unsigned char *)temp_a1 + (870)) = 0U;
                    *(u16 *)((unsigned char *)temp_a1 + (8)) = 0U;
                }
                break;
            case 1:                                 /* switch 2 */
                *(s8 *)((unsigned char *)temp_a1 + (807)) = 0;
                *(s8 *)((unsigned char *)temp_a1 + (836)) = 1;
                sp2C = temp_a1;
                func_80142774();
                *(s16 *)((unsigned char *)temp_a1 + (10)) = 0;
                *(u16 *)((unsigned char *)temp_a1 + (8)) = 0U;
                *(u16 *)((unsigned char *)temp_a1 + (6)) = 0U;
                *(s8 *)((unsigned char *)temp_a1 + (15)) = 0;
                *(s8 *)((unsigned char *)temp_a1 + (14)) = 0;
                *(s8 *)((unsigned char *)temp_a1 + (13)) = 0;
                *(s16 *)((unsigned char *)temp_a1 + (4)) = 0x50;
                break;
            case 16:                                /* switch 2 */
                temp_v0_3 = *(u16 *)((unsigned char *)temp_a1 + (870));
                if (temp_v0_3 >= 0x3CU) {
                    *(s16 *)((unsigned char *)temp_a1 + (780)) = 0x100;
                    *(u16 *)((unsigned char *)temp_a1 + (782)) = 1U;
                } else {
                    *(u16 *)((unsigned char *)temp_a1 + (870)) = (u16) (temp_v0_3 + 1);
                }
                break;
            }
        }
        break;
    case 0x50:                                      /* switch 1 */
        func_8013F9E0();
        func_8013F878();
        break;
    case 0x80:                                      /* switch 1 */
        sp28 = temp_v1;
        sp34 = D_80158A64;
        func_80044750(0xC1);
        *(s8 *)((unsigned char *)temp_v1 + (82)) = 1;
        *(s8 *)((unsigned char *)sp34 + (28)) = 0x11;
        func_8013FBD0();
        break;
    case 0xA0:                                      /* switch 1 */
        sp2C = temp_a1;
        sp34 = D_80158A64;
        func_80044750(0xB1);
        temp_a1->unk0 = 0;
        *(s8 *)((unsigned char *)sp34 + (18)) = 0;
        *(s16 *)((unsigned char *)D_80158A70 + (30)) = 1;
        *(s16 *)((unsigned char *)sp34 + (2)) = 0xA0;
        break;
    }
    func_8013F040();
}
