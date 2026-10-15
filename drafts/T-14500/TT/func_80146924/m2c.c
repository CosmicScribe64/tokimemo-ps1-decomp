void func_80146924(void) {
    u8 *sp3C;
    u8 sp37;
    u16 temp_t5;
    u16 temp_t8;
    u16 temp_v0;
    u16 temp_v0_2;
    u8 *temp_s0;
    void *temp_v1;

    temp_s0 = D_80158A64;
    sp3C = D_80158A6C;
    temp_v0 = *(u16 *)((unsigned char *)temp_s0 + (2));
    switch (temp_v0) {                              /* switch 1; irregular */
    case 0x0:                                       /* switch 1 */
        func_80044750(0x22);
        func_80044750(0x2F);
        func_80044750(4);
        func_80044750(0x206);
        *(s8 *)((unsigned char *)temp_s0 + (28)) = 1;
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
        if (func_80145E60() != 0) {
            *(s8 *)((unsigned char *)temp_s0 + (29)) = 0;
            *(s8 *)((unsigned char *)temp_s0 + (30)) = 0;
            *(s8 *)((unsigned char *)temp_s0 + (31)) = 0;
            func_80146000();
            func_80132198(2, 6, 4);
            func_8013230C(0xC, 1);
            *(s16 *)((unsigned char *)sp3C + (30)) = 0;
        } else {
            *(u8 *)((unsigned char *)temp_s0 + (9)) = 1U;
        }
        *(u16 *)((unsigned char *)temp_s0 + (38)) = 0xE10U;
        *(s16 *)((unsigned char *)temp_s0 + (2)) = 0x20;
        break;
    case 0x20:                                      /* switch 1 */
        func_801462AC();
        break;
    case 0x40:                                      /* switch 1 */
        func_80146224();
        func_80146348();
        if ((*(u8 *)((unsigned char *)temp_s0 + (10)) != 0) || (temp_t5 = *(u16 *)((unsigned char *)temp_s0 + (38)) - 1, *(u16 *)((unsigned char *)temp_s0 + (38)) = temp_t5, ((temp_t5 & 0xFFFF) == 0))) {
            *D_80158AA4 = 0;
            *(u8 *)((unsigned char *)temp_s0 + (10)) = 0U;
            *(s16 *)((unsigned char *)temp_s0 + (4)) = 0;
            *(s16 *)((unsigned char *)temp_s0 + (2)) = 0x80;
            *(u16 *)((unsigned char *)temp_s0 + (40)) = 0x3CU;
            temp_v0_2 = *(u16 *)((unsigned char *)sp3C + (24));
            temp_v1 = (*(u16 *)((unsigned char *)sp3C + (24)) * 0x3C) + D_80158AA8;
            switch (temp_v0_2) {                    /* switch 2; irregular */
            case 0:                                 /* switch 2 */
                sp37 = 2;
                break;
            case 1:                                 /* switch 2 */
                sp37 = 6;
                break;
            case 2:                                 /* switch 2 */
                sp37 = 4;
                break;
            }
            *(u8 *)((unsigned char *)temp_v1 + (42)) = sp37;
            *(u8 *)((unsigned char *)temp_v1 + (22)) = sp37;
            *(u8 *)((unsigned char *)temp_v1 + (2)) = sp37;
            *(s16 *)((unsigned char *)temp_s0 + (2)) = 0x70;
        }
        break;
    case 0x70:                                      /* switch 1 */
        func_80146224();
        temp_t8 = *(u16 *)((unsigned char *)temp_s0 + (40)) - 1;
        *(u16 *)((unsigned char *)temp_s0 + (40)) = temp_t8;
        if (!(temp_t8 & 0xFFFF)) {
            *(s8 *)((unsigned char *)temp_s0 + (29)) = 0x80;
            *(s8 *)((unsigned char *)temp_s0 + (30)) = 0x80;
            *(s8 *)((unsigned char *)temp_s0 + (31)) = 0x80;
            *(s16 *)((unsigned char *)temp_s0 + (2)) = 0x80;
        }
        break;
    case 0x80:                                      /* switch 1 */
        func_8014687C();
        break;
    }
    if (*(u8 *)((unsigned char *)temp_s0 + (9)) != 0) {
        temp_s0->unk0 = 7;
        *(s16 *)((unsigned char *)temp_s0 + (6)) = 0;
        *(s16 *)((unsigned char *)temp_s0 + (4)) = 0;
        *(s16 *)((unsigned char *)temp_s0 + (2)) = 0;
        *(s8 *)((unsigned char *)temp_s0 + (11)) = 0;
        *(u8 *)((unsigned char *)temp_s0 + (10)) = 0U;
        *(u8 *)((unsigned char *)temp_s0 + (9)) = 0U;
    }
    func_80148714((s32) ((u16) *(u16 *)((unsigned char *)temp_s0 + (38)) / 60U), 0x26);
}
