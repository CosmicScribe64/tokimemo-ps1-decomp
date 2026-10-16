void func_80154960(void) {
    u8 *sp30;
    s32 temp_v0;
    s32 temp_v0_10;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_8;
    void *temp_v0_7;
    void *temp_v0_9;

    D_801604C0 += 1;
    func_8015B59C();
    switch (D_801604C4) {
    case 0:
        temp_v0 = D_801604C0;
        if (temp_v0 < 0x834) {
            D_801604C0 = temp_v0;
            if ((temp_v0 % 7) == 0) {
                func_80154F74();
            }
        }
        temp_v0_2 = D_801604C0;
        if (temp_v0_2 < 0x4B0) {
            D_801604C0 = temp_v0_2;
            if ((temp_v0_2 % 50) == 0) {
                func_801563E4(0);
            }
        }
        temp_v0_3 = D_801604C0;
        if (temp_v0_3 >= 0x259) {
            D_801604C0 = temp_v0_3;
            if ((temp_v0_3 < 0x708) && (func_8015CCC8(4) == 0)) {
                func_801553AC();
            }
        }
        if (D_801604C0 >= 0x709) {
            func_8015A900();
        default: ;
        }
        break;
    case 1:
        D_80160470 = func_80156178();
        func_8015A900();
        break;
    case 2:
        temp_v0_4 = D_801604C0;
        if (temp_v0_4 < 0x258) {
            D_801604C0 = temp_v0_4;
            if ((temp_v0_4 % 7) == 0) {
                func_80154F74();
            }
        }
        if ((D_801604C0 < 0x12C) && (func_8015CCC8(4) == 0)) {
            func_801553AC();
        }
        temp_v0_5 = D_801604C0;
        if (temp_v0_5 >= 0x1E1) {
            D_801604C0 = temp_v0_5;
            if ((temp_v0_5 % 110) == 0) {
                func_801563E4(1);
            }
        }
        temp_v0_6 = D_801604C0;
        if (temp_v0_6 >= 0x12D) {
            D_801604C0 = temp_v0_6;
            if ((temp_v0_6 % 120) == 0) {
                func_8015595C();
            }
        }
        if (func_8015CCC8(6) == 0) {
            func_8015A900();
        }
        if (D_8015EDB4[D_80160470].unk3 == 2) {
            func_8015A900();
        }
        break;
    case 3:
        func_8015C698();
        func_8015A900();
        break;
    case 4:
        temp_v0_7 = GetWorkBase(0x18, D_8011ECA0);
        sp30 = temp_v0_7;
        func_8009F02C((s32) temp_v0_7);
        SetSemiTrans(sp30, 1);
        *(s8 *)((unsigned char *)sp30 + (4)) = (s8) D_801604C0;
        *(s8 *)((unsigned char *)sp30 + (5)) = (s8) D_801604C0;
        *(s8 *)((unsigned char *)sp30 + (6)) = (s8) D_801604C0;
        *(s16 *)((unsigned char *)sp30 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0x100);
        *(s16 *)((unsigned char *)sp30 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        *(s16 *)((unsigned char *)sp30 + (12)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x100);
        *(s16 *)((unsigned char *)sp30 + (14)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        *(s16 *)((unsigned char *)sp30 + (16)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0x100);
        *(s16 *)((unsigned char *)sp30 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + 0x78);
        *(s16 *)((unsigned char *)sp30 + (20)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x100);
        *(s16 *)((unsigned char *)sp30 + (22)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + 0x78);
        AddPrim((D_8011ECA0 << 0xA) + D_800E8CA0, sp30);
        dtd_on_tpage(0, 0, 0, 1, 0);
        temp_v0_8 = D_801604C0 + 2;
        D_801604C0 = temp_v0_8;
        if (temp_v0_8 >= 0x80) {
            func_8015A900();
        }
        break;
    case 5:
        temp_v0_9 = GetWorkBase(0x18, D_8011ECA0);
        sp30 = temp_v0_9;
        func_8009F02C((s32) temp_v0_9);
        SetSemiTrans(sp30, 1);
        *(s8 *)((unsigned char *)sp30 + (4)) = (s8) (0x80 - D_801604C0);
        *(s8 *)((unsigned char *)sp30 + (5)) = (s8) (0x80 - D_801604C0);
        *(s8 *)((unsigned char *)sp30 + (6)) = (s8) (0x80 - D_801604C0);
        *(s16 *)((unsigned char *)sp30 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0x100);
        *(s16 *)((unsigned char *)sp30 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        *(s16 *)((unsigned char *)sp30 + (12)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x100);
        *(s16 *)((unsigned char *)sp30 + (14)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        *(s16 *)((unsigned char *)sp30 + (16)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0x100);
        *(s16 *)((unsigned char *)sp30 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + 0x78);
        *(s16 *)((unsigned char *)sp30 + (20)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x100);
        *(s16 *)((unsigned char *)sp30 + (22)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + 0x78);
        AddPrim((D_8011ECA0 << 0xA) + D_800E8CA0, sp30);
        dtd_on_tpage(0, 0, 0, 1, 0);
        temp_v0_10 = D_801604C0 + 3;
        D_801604C0 = temp_v0_10;
        if (temp_v0_10 >= 0x80) {
            func_8015ABF0();
            func_8004284C();
        }
        break;
    }
}
