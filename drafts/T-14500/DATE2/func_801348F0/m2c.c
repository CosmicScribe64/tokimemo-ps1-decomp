? func_800573F8(?);                                 /* extern */
extern s16 D_800B5922;
extern s16 D_800B5924;
extern s16 D_800B5926;
extern s16 D_800B592A;
extern s8 D_800B593A;
extern u32 D_800E7384;
extern u8 D_800E738A;
extern u8 D_8012200C;
extern u8 D_8012200D;
extern u8 D_8012200E;
extern u16 D_8013A5CC;

void func_801348F0(void) {
    u32 sp30;
    Rec24 *temp_v0;
    s16 temp_t8;
    s16 var_a0;
    s16 var_a0_10;
    s16 var_a0_11;
    s16 var_a0_2;
    s16 var_a0_3;
    s16 var_a0_4;
    s16 var_a0_5;
    s16 var_a0_6;
    s16 var_a0_7;
    s16 var_a0_8;
    s16 var_a0_9;
    s32 *temp_v0_10;
    s32 *temp_v0_11;
    s32 *temp_v0_2;
    s32 *temp_v0_3;
    s32 temp_lo;
    s32 temp_lo_10;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_lo_5;
    s32 temp_lo_6;
    s32 temp_lo_7;
    s32 temp_lo_8;
    s32 temp_lo_9;
    s32 temp_t9;
    u16 temp_t6_2;
    u32 var_a3;
    u8 temp_a1;
    u8 temp_a1_2;
    u8 temp_a1_3;
    void *temp_t6;
    void *temp_v0_4;
    void *temp_v0_5;
    void *temp_v0_6;
    void *temp_v0_7;
    void *temp_v0_8;
    void *temp_v0_9;

    var_a3 = D_800E7384;
    D_800B3D60 = 0;
    switch (var_a3) {
    case 0:
        D_80122D2C = 1;
        func_80044750(0x201);
        D_800B5926 = 0;
        D_800B5922 = 0;
        D_800B5924 = 0;
        *D_800B5920 = 0;
        D_800B5939 = 0;
        D_800B5938 = 0;
        D_800B5948 = -1;
        D_800B592A = 0;
        D_800B593A = 0;
        func_8006492C(0);
        D_8013A5CC = 0;
        var_a0 = 0;
        do {
            temp_t8 = (var_a0 * 2) + 0x15;
            temp_v0 = &D_801217D0[var_a0];
            *(s16 *)((unsigned char *)temp_v0 + (368)) = (s16) ((var_a0 << 6) + 0x80);
            temp_t9 = var_a0 << 7;
            temp_t6 = &D_800E6280 + var_a0;
            var_a0 += 1;
            *(s32 *)((unsigned char *)temp_v0 + (360)) = 0x82000000;
            *(s16 *)((unsigned char *)temp_v0 + (370)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0 + (374)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (375)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (376)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (372)) = temp_t8;
            *(s16 *)((unsigned char *)temp_v0 + (378)) = 0x1E0;
            *(s8 *)((unsigned char *)temp_v0 + (380)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (381)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (382)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (384)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (386)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (388)) = 0x1000;
            *(s16 *)((unsigned char *)temp_v0 + (390)) = 0x1000;
            *(s32 *)((unsigned char *)temp_v0 + (392)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (364)) = (s16) (temp_t9 - 0xA0);
            *(s16 *)((unsigned char *)temp_v0 + (366)) = -0x96;
            *(s8 *)((unsigned char *)temp_t6 + (4271)) = 0xC;
        } while (var_a0 < 2);
        func_801365DC(0x43C2);
        D_800E7384 += 1;
        return;
    case 1:
        if (cd_read_sync_status() & 1) {
            if (((D_80125C04->unk0 & 0xFFFF0000) != 0x38000000) || ((*(s32 *)((unsigned char *)D_80125C04 + (4)) & 0xFF010000) != 0x10000)) {
                func_800AE0B0("DEC_BS header Error \n");
                D_800B5924 = 0;
                D_800B5926 = 0;
                D_800E7384 -= 1;
                return;
            }
            func_800536AC(0x140, 0x100, 0x140, 0xA0);
            D_800E7384 += 1;
        } else {
            return;
        }
        break;
    case 2:
        cd_read_sectors(0x31U, 0x801AE000, 0xBCD1);
        func_801339F8();
        D_800E7384 += 1;
        return;
    case 3:
    case 11:
    case 14:
    case 16:
    case 18:
        if (cd_read_sync_status() & 1) {
            D_800E7384 += 1;
        }
        return;
    case 4:
        func_80043914(D_8013A5B0, 0x11, 1, 0, 0);
        func_80084E90(D_8013A5B4, D_8013A5B8, D_8013A5BC, D_8013A5C0, D_8013A5C4, D_8013A5C8);
        D_800E7384 += 1;
        return;
    case 5:
        var_a0_2 = 0xA;
        do {
            temp_lo = var_a0_2 * 0x24;
            var_a0_2 += 1;
            temp_v0_2 = D_801217D0 + temp_lo;
            *temp_v0_2 &= 0x7FFFFFFF;
        } while (var_a0_2 < 0xC);
        var_a0_3 = 0x3A;
        do {
            temp_lo_2 = var_a0_3 * 0x24;
            var_a0_3 += 1;
            temp_v0_3 = D_801217D0 + temp_lo_2;
            *temp_v0_3 &= 0x7FFFFFFF;
        } while (var_a0_3 < 0x40);
        D_800E7384 = var_a3 + 1;
        return;
    case 6:
        temp_a1 = D_8012200C + 4;
        D_8012200C = temp_a1;
        D_8012200E = temp_a1;
        D_8012200D = temp_a1;
        var_a0_4 = 0xA;
        do {
            temp_lo_3 = var_a0_4 * 0x24;
            var_a0_4 += 1;
            temp_v0_4 = D_801217D0 + temp_lo_3;
            *(u8 *)((unsigned char *)temp_v0_4 + (22)) = (u8) D_8012200C;
            *(u8 *)((unsigned char *)temp_v0_4 + (21)) = (u8) D_8012200C;
            *(u8 *)((unsigned char *)temp_v0_4 + (20)) = (u8) D_8012200C;
        } while (var_a0_4 < 0xC);
        var_a0_5 = 0x3B;
        do {
            temp_lo_4 = var_a0_5 * 0x24;
            var_a0_5 += 1;
            temp_v0_5 = D_801217D0 + temp_lo_4;
            *(u8 *)((unsigned char *)temp_v0_5 + (22)) = (u8) D_8012200C;
            *(u8 *)((unsigned char *)temp_v0_5 + (21)) = (u8) D_8012200C;
            *(u8 *)((unsigned char *)temp_v0_5 + (20)) = (u8) D_8012200C;
        } while (var_a0_5 < 0x40);
        if (D_8012200C == 0x40) {
            D_800E7384 = var_a3 + 1;
        }
        return;
    case 7:
        temp_a1_2 = D_8012200C + 4;
        D_8012200C = temp_a1_2;
        D_8012200E = temp_a1_2;
        D_8012200D = temp_a1_2;
        var_a0_6 = 0xA;
        do {
            temp_lo_5 = var_a0_6 * 0x24;
            var_a0_6 += 1;
            temp_v0_6 = D_801217D0 + temp_lo_5;
            *(u8 *)((unsigned char *)temp_v0_6 + (22)) = (u8) D_8012200C;
            *(u8 *)((unsigned char *)temp_v0_6 + (21)) = (u8) D_8012200C;
            *(u8 *)((unsigned char *)temp_v0_6 + (20)) = (u8) D_8012200C;
        } while (var_a0_6 < 0xC);
        var_a0_7 = 0x3B;
        do {
            temp_lo_6 = var_a0_7 * 0x24;
            var_a0_7 += 1;
            temp_v0_7 = D_801217D0 + temp_lo_6;
            *(u8 *)((unsigned char *)temp_v0_7 + (22)) = (u8) D_8012200C;
            *(u8 *)((unsigned char *)temp_v0_7 + (21)) = (u8) D_8012200C;
            *(u8 *)((unsigned char *)temp_v0_7 + (20)) = (u8) D_8012200C;
        } while (var_a0_7 < 0x40);
        if (D_8012200C == 0x80) {
            var_a3 += 1;
        }
    case 8:
block_50:
        temp_t6_2 = (D_8013A5CC + 1) & 0xFFFF;
        D_8013A5CC = temp_t6_2;
        D_800E7384 = var_a3;
        if (temp_t6_2 == 0x96) {
            D_800E7384 = var_a3 + 1;
        }
        return;
    case 9:
        temp_a1_3 = D_8012200C - 4;
        D_8012200C = temp_a1_3;
        D_8012200E = temp_a1_3;
        D_8012200D = temp_a1_3;
        var_a0_8 = 0xA;
        do {
            temp_lo_7 = var_a0_8 * 0x24;
            var_a0_8 += 1;
            temp_v0_8 = D_801217D0 + temp_lo_7;
            *(u8 *)((unsigned char *)temp_v0_8 + (22)) = (u8) D_8012200C;
            *(u8 *)((unsigned char *)temp_v0_8 + (21)) = (u8) D_8012200C;
            *(u8 *)((unsigned char *)temp_v0_8 + (20)) = (u8) D_8012200C;
        } while (var_a0_8 < 0xC);
        var_a0_9 = 0x3B;
        do {
            temp_lo_8 = var_a0_9 * 0x24;
            var_a0_9 += 1;
            temp_v0_9 = D_801217D0 + temp_lo_8;
            *(u8 *)((unsigned char *)temp_v0_9 + (22)) = (u8) D_8012200C;
            *(u8 *)((unsigned char *)temp_v0_9 + (21)) = (u8) D_8012200C;
            *(u8 *)((unsigned char *)temp_v0_9 + (20)) = (u8) D_8012200C;
        } while (var_a0_9 < 0x40);
        var_a0_10 = 0xA;
        if (D_8012200C == 0) {
            do {
                temp_lo_9 = var_a0_10 * 0x24;
                var_a0_10 += 1;
                temp_v0_10 = D_801217D0 + temp_lo_9;
                *temp_v0_10 |= 0x80000000;
            } while (var_a0_10 < 0xC);
            var_a0_11 = 0x3A;
            do {
                temp_lo_10 = var_a0_11 * 0x24;
                var_a0_11 += 1;
                temp_v0_11 = D_801217D0 + temp_lo_10;
                *temp_v0_11 |= 0x80000000;
            } while (var_a0_11 < 0x40);
            var_a3 += 1;
        }
        goto block_50;
    case 10:
        func_800462C8(0xAU, 0x80162000, 0x4350);
        D_800E7384 += 1;
        return;
    case 12:
        if (((D_80125C04->unk0 & 0xFFFF0000) != 0x38000000) || ((*(s32 *)((unsigned char *)D_80125C04 + (4)) & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n", var_a3);
            D_800B5924 = 0;
            D_800B5926 = 0;
            D_800E7384 -= 2;
            return;
        }
        func_8005751C(1);
        func_800573F8(1);
        D_800E7384 += 1;
        return;
    case 13:
        sp30 = var_a3;
        D_800E7384 = var_a3;
        func_80132BB0();
        D_800E738A -= 1;
        D_800E7384 = var_a3 + 1;
        return;
    default:
        func_8004284C();
        D_80122D2C = 0;
        func_80044750(0x200);
        func_8006492C(1);
        return;
    }
}
