? func_80048514(?, s32, ?);                         /* extern */
? func_800485BC(s32, ?);                            /* extern */
extern s8 D_80139C30;

void func_80136DEC(void) {
    s16 spC4;
    s16 sp98;
    s16 sp6C;
    u8 sp54;
    DisplayActor44 *sp4C;
    s16 *sp44;
    s16 *sp40;
    s16 *sp3C;
    DisplayActor44 *var_v0;
    DisplayActor44 *var_v0_2;
    DisplayActor44 *var_v0_3;
    s16 *var_a1;
    s16 *var_a2;
    s16 *var_v1;
    s16 temp_t2;
    s16 temp_t5;
    s16 temp_t9;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s1;
    u8 *var_s0_3;
    u8 temp_t0;
    u8 temp_t1;
    u8 temp_t2_2;
    u8 temp_t4;
    u8 temp_t8;
    u8 temp_v1;

    temp_v1 = D_800E6280.unk_110A;
    M2C_MEMCPY_ALIGNED(&spC4, &D_80139C68, 0x24);
    *(s32 *)((unsigned char *)(&spC4) + (36)) = (s32) D_80139C68.value[0x12].unk0;
    *(s32 *)((unsigned char *)(&spC4 + 0x24) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80139C68.value[0x12]) + (4));
    M2C_MEMCPY_ALIGNED(&sp98, &D_80139C94, 0x24);
    *(s32 *)((unsigned char *)(&sp98) + (36)) = (s32) D_80139C94.value[0x12].unk0;
    *(s32 *)((unsigned char *)(&sp98 + 0x24) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80139C94.value[0x12]) + (4));
    M2C_MEMCPY_ALIGNED(&sp6C, &D_80139CC0, 0x24);
    *(s32 *)((unsigned char *)(&sp6C) + (36)) = (s32) D_80139CC0.value[0x12].unk0;
    *(s32 *)((unsigned char *)(&sp6C + 0x24) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80139CC0.value[0x12]) + (4));
    sp54.unk0 = (s32) D_80139CEC.unk0;
    *(s32 *)((unsigned char *)(&sp54) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80139CEC) + (8));
    *(s32 *)((unsigned char *)(&sp54) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80139CEC) + (4));
    *(s32 *)((unsigned char *)(&sp54) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80139CEC) + (16));
    *(u16 *)((unsigned char *)(&sp54) + (20)) = (u16) *(u16 *)((unsigned char *)(&D_80139CEC) + (20));
    *(s32 *)((unsigned char *)(&sp54) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80139CEC) + (12));
    switch (temp_v1) {                              /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        func_80048514(5, *(s32 *)((unsigned char *)D_801391E4 + (28)), 1);
        var_s0 = 0;
        do {
            func_800485BC(*(D_801391E4 + var_s0), 1);
            var_s0 += 4;
        } while (var_s0 < 0x1C);
        var_s0_2 = 0x20;
        do {
            func_800485BC(*(D_801391E4 + var_s0_2), 1);
            var_s0_2 += 4;
        } while (var_s0_2 != 0x28);
        func_8004284C();
        return;
    case 1:                                         /* switch 1 */
        var_v0 = D_8011ECD0;
        var_s1 = 0;
        var_s0_3 = &sp54;
        var_v1 = &spC4;
        var_a1 = &sp98;
        var_a2 = &sp6C;
        do {
            sp4C = var_v0;
            sp44 = var_v1;
            sp40 = var_a1;
            sp3C = var_a2;
            func_80048F64(var_s1 + 0x60);
            *(s32 *)((unsigned char *)var_v0 + (6584)) = 0x01000000;
            *(s8 *)((unsigned char *)var_v0 + (6535)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (6530)) = 0x40;
            *(s8 *)((unsigned char *)var_v0 + (6531)) = 4;
            *(u8 *)((unsigned char *)var_v0 + (6529)) = (u8) *var_s0_3;
            *(s32 *)((unsigned char *)var_v0 + (6540)) = (s32) D_801391E8;
            *(s32 *)((unsigned char *)var_v0 + (6544)) = (s32) D_801391EC;
            *(s32 *)((unsigned char *)var_v0 + (6580)) = (s32) D_801391E4;
            temp_t9 = *var_v1;
            temp_t2 = *var_a1;
            temp_t5 = *var_a2;
            var_s1 += 1;
            var_s0_3 += 1;
            var_v0 += 0x44;
            *(s8 *)((unsigned char *)var_v0 + (6464)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (6465)) = 0x80;
            *(s16 *)((unsigned char *)var_v0 + (6486)) = 0x1000;
            *(s16 *)((unsigned char *)var_v0 + (6488)) = 0x1000;
            *(s8 *)((unsigned char *)var_v0 + (6466)) = 0;
            *(s16 *)((unsigned char *)var_v0 + (6480)) = (s16) D_801391F0;
            var_v1 += 2;
            var_a1 += 2;
            var_a2 += 2;
            *(s16 *)((unsigned char *)var_v0 + (6484)) = temp_t9;
            *(s16 *)((unsigned char *)var_v0 + (6498)) = temp_t2;
            *(s16 *)((unsigned char *)var_v0 + (6502)) = temp_t5;
        } while (var_s1 < 0x16);
        *(u8 *)((unsigned char *)D_8011ECD0 + (6531)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6531)) | 0x80);
        var_v0_2 = D_8011ECD0 + 0x44;
        do {
            temp_t0 = *(u8 *)((unsigned char *)var_v0_2 + (6599));
            temp_t8 = *(u8 *)((unsigned char *)var_v0_2 + (6667));
            temp_t2_2 = *(u8 *)((unsigned char *)var_v0_2 + (6735));
            temp_t1 = *(u8 *)((unsigned char *)var_v0_2 + (6531));
            var_v0_2 += 0x110;
            *(s8 *)((unsigned char *)var_v0_2 + (6463)) = (s8) (temp_t2_2 | 0x80);
            *(s8 *)((unsigned char *)var_v0_2 + (6395)) = (s8) (temp_t8 | 0x80);
            *(s8 *)((unsigned char *)var_v0_2 + (6327)) = (s8) (temp_t0 | 0x80);
            *(s8 *)((unsigned char *)var_v0_2 + (6259)) = (s8) (temp_t1 | 0x80);
        } while (var_v0_2 != (D_8011ECD0 + 0x594));
        func_8004284C();
        return;
    case 2:                                         /* switch 1 */
        if ((u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535)) >= 0x80U) {
            D_800E6280.unk_03A = 0x80;
            *(u8 *)((unsigned char *)D_8011ECD0 + (6535)) = 0x80U;
            func_8004284C();
        } else {
            *(u8 *)((unsigned char *)D_8011ECD0 + (6535)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6535)) + 4);
        }
        var_v0_3 = D_8011ECD0 + 0x88;
        *(u8 *)((unsigned char *)D_8011ECD0 + (6603)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535));
        do {
            *(u8 *)((unsigned char *)var_v0_3 + (6535)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535));
            *(u8 *)((unsigned char *)var_v0_3 + (6603)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535));
            *(u8 *)((unsigned char *)var_v0_3 + (6671)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535));
            var_v0_3 += 0x110;
            *(u8 *)((unsigned char *)var_v0_3 + (6467)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (6535));
        } while (var_v0_3 != (D_8011ECD0 + 0x5D8));
        return;
    case 3:                                         /* switch 1 */
        temp_t4 = D_800E6280.unk_110D;
        switch (temp_t4) {                          /* switch 2 */
        case 0:                                     /* switch 2 */
            func_80046290(0x07000002, 0x03000008, 0xE);
            func_80044750(0x300);
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            return;
        case 1:                                     /* switch 2 */
        case 3:                                     /* switch 2 */
            if (func_800460EC() == 4) {
                D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
                return;
            }
        default:                                    /* switch 2 */
            return;
        case 2:                                     /* switch 2 */
            if (func_800460EC() == 2) {
                func_80046290(0x08000001, 0x02000004, 0xE);
                func_80044750(0x300);
                D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
                return;
            }
            break;
        case 4:                                     /* switch 2 */
            if (func_800460EC() == 2) {
                D_80139C58 = 0;
                D_80139C30 = 5;
                *D_80139C34 = 0;
                *D_80139C3C = 0;
                *D_80139C44 = 0;
                *(s8 *)((unsigned char *)(&D_80139C44[1]) + (1)) = 0;
                *(s8 *)((unsigned char *)(&D_80139C44[1]) + (2)) = 0;
                *(s8 *)((unsigned char *)(&D_80139C44[1]) + (3)) = 0;
                *(s8 *)((unsigned char *)(&D_80139C3C[1]) + (1)) = 0;
                *(s8 *)((unsigned char *)(&D_80139C3C[1]) + (2)) = 0;
                *(s8 *)((unsigned char *)(&D_80139C3C[1]) + (3)) = 0;
                *(s8 *)((unsigned char *)(&D_80139C34[1]) + (1)) = 0;
                *(s8 *)((unsigned char *)(&D_80139C34[1]) + (2)) = 0;
                *(s8 *)((unsigned char *)(&D_80139C34[1]) + (3)) = 0;
                D_80139C34[1].unk0 = 0;
                D_80139C3C[1].unk0 = 0;
                D_80139C44[1].unk0 = 0;
                D_8011ECD0->unk_26 = 0;
                D_8011ECD0->unk_2A = 0;
                func_80042808();
                return;
            }
            break;
        }
        break;
    default:                                        /* switch 1 */
        func_80046500();
        break;
    }
}
