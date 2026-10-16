? func_800AE110(? *, ? *, ?);                       /* extern */
extern s32 D_800B3564;
extern s32 D_800B356C;
extern ? D_800E6344;
extern u8 D_800E6354;
extern ? D_800E635C;
extern ? D_800E6448;
extern s32 D_800E66B0;
extern u32 D_800E7384;
extern u8 D_800E738D;
extern s32 D_80150684;

void func_8014790C(void) {
    ? sp7C;
    s16 *var_s2;
    s16 *var_s3;
    s16 temp_a1;
    s16 temp_v1;
    s32 *var_s4;
    s32 temp_v0;
    s32 var_s1;
    u8 *var_s0;
    void *var_t1;

    switch (D_800E738D) {                           /* irregular */
    case 0:
        func_8004E58C();
        func_8006612C((s32) "結果発\x95\\ ");
        func_80050DFC(&D_800E6354);
        func_80050E8C("今日は、試験結果の 発\x95\\の日だ。）", 0, 0x1F);
        func_8004EA98();
        func_8004EAD4(0);
        func_8004500C(1, 0x202);
        func_800649D4();
        func_80048E78();
        D_800E738D += 1;
        cd_read_sectors(0xDU, 0x801E0000, 0x9CA8);
        break;
    case 1:
        if ((u32) D_800E7384 >= 0x101U) {
            D_800E738D += 1;
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8004E58C();
            func_80064FA4(0);
            func_80063930(0);
            func_8006492C(0);
            func_80048F64(0x60);
            D_80120651 = 4;
            D_80120652 = 0x40;
            D_80120688 = 0;
            D_80120653 = 0xA4;
            D_8012065C = (u8 *)0x801E64A0;
            D_80120660 = (u8 *)0x801E6520;
            D_80120684 = (u8 *)0x801E6494;
            D_80120668 = 0;
            D_80120655 = 1;
            D_80120654 = 8;
            D_80120676 = 0;
            D_8012067A = -0x28;
            D_80120657 = 0x80;
            func_80148E98();
            func_80148EE8();
            func_801480D0();
            var_s4 = &D_80150684;
            var_s3 = D_8015064C;
            var_s2 = D_80150630;
            var_s0 = D_8011ECD0;
            var_s1 = 0;
            do {
                temp_v1 = *var_s2;
                temp_a1 = *var_s3;
                temp_v0 = *var_s4;
                *(s8 *)((unsigned char *)var_s0 + (6597)) = 3;
                *(s8 *)((unsigned char *)var_s0 + (6598)) = 0x40;
                *(s32 *)((unsigned char *)var_s0 + (6652)) = 0;
                *(s8 *)((unsigned char *)var_s0 + (6599)) = 0x24;
                *(s32 *)((unsigned char *)var_s0 + (6608)) = 0x801E64A0;
                *(s32 *)((unsigned char *)var_s0 + (6612)) = 0x801E6520;
                *(s32 *)((unsigned char *)var_s0 + (6648)) = 0x801E6494;
                *(s16 *)((unsigned char *)var_s0 + (6620)) = 0x1F;
                *(s8 *)((unsigned char *)var_s0 + (6601)) = 1;
                *(s8 *)((unsigned char *)var_s0 + (6600)) = 8;
                *(s8 *)((unsigned char *)var_s0 + (6603)) = 0x80;
                *(s16 *)((unsigned char *)var_s0 + (6634)) = temp_v1;
                *(s16 *)((unsigned char *)var_s0 + (6638)) = temp_a1;
                if (temp_v0 == 0xD) {
                    func_800AE090(&sp7C, &D_800E6354, 8);
                    func_800AE110(&sp7C, &D_800E635C, 8);
                    func_8004E788((s16) (*var_s2 + 0x18), *var_s3, 3, (s32) &sp7C, 0);
                    func_800AE0B0("%s\n", &D_800E6344);
                } else if (temp_v0 == 0xA) {
                    func_8004E788((s16) (temp_v1 + 0x18), temp_a1, 3, D_800B356C, 0);
                } else if (temp_v0 == 0xB) {
                    if (((u32) (D_800E66B0 << 0x14) >> 0x1D) != 0) {
                        func_8004E788((s16) (temp_v1 + 0x18), temp_a1, 3, D_800B3564, 0);
                    } else {
                        var_t1 = D_8011ECD0 + ((var_s1 + 0x61) * 0x44);
                        goto block_20;
                    }
                } else if (*(&D_800E6448 + (temp_v0 * 0x38)) & 2) {
                    func_8004E788((s16) (temp_v1 + 0x18), temp_a1, 3, D_800B3538[temp_v0], 0);
                } else {
                    var_t1 = D_8011ECD0 + ((var_s1 + 0x61) * 0x44);
block_20:
                    *(s8 *)((unsigned char *)var_t1 + (3)) = 0xA4;
                }
                var_s1 += 1;
                var_s0 += 0x44;
                var_s2 += 2;
                var_s3 += 2;
                var_s4 += 4;
            } while (var_s1 != 0xE);
            func_8004EA98();
            func_8004EAD4(0);
            func_80148F34();
            func_8004284C();
        }
        break;
    }
}
