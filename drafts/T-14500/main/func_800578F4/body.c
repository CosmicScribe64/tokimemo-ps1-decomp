void func_800578F4(s32 a) {
    u8 *sp38;
    s32 temp_v0;
    s32 temp_v0_2;
    u8 *temp_t2;

    if (a != -1) {
        D_800B5948 = a;
        temp_v0 = GetWorkBase(0x14, D_8011ECA0);
        func_8009F0A4(temp_v0);
        SetSemiTrans(temp_v0, 0);
        SetShadeTex(temp_v0, 0);
        *(u8 *)((unsigned char *)temp_v0 + (4)) = (u8) D_800B593C;
        *(u8 *)((unsigned char *)temp_v0 + (5)) = (u8) D_800B593C;
        *(u8 *)((unsigned char *)temp_v0 + (6)) = (u8) D_800B593C;
        if (a == 1) {
            *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0);
            *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
            *(s8 *)((unsigned char *)temp_v0 + (13)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (12)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (16)) = 0xC0;
            *(s16 *)((unsigned char *)temp_v0 + (18)) = 0xA0;
        } else if (a == 2) {
            *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0);
            *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
            *(s8 *)((unsigned char *)temp_v0 + (13)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (12)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (16)) = 0xC0;
            *(s16 *)((unsigned char *)temp_v0 + (18)) = 0xF0;
        } else {
            *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0);
            *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
            *(s8 *)((unsigned char *)temp_v0 + (13)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0 + (12)) = 0;
            *(s16 *)((unsigned char *)temp_v0 + (16)) = 0xC0;
            *(s16 *)((unsigned char *)temp_v0 + (18)) = 0x58;
        }
        AddPrim((D_8011ECA0 << 0xA) + 0x34 + D_800E8CA0, temp_v0);
        temp_t2 = &D_800B5938[a];
        sp38 = temp_t2;
        if (*temp_t2 != 0) {
            goto_tpage(0x140, 0x100, 0xD, 0, 2);
        } else {
            goto_tpage(0x2C0, 0, 0xD, 0, 2);
        }
        temp_v0_2 = GetWorkBase(0x14, D_8011ECA0);
        func_8009F0A4(temp_v0_2);
        SetSemiTrans(temp_v0_2, 0);
        SetShadeTex(temp_v0_2, 0);
        *(u8 *)((unsigned char *)temp_v0_2 + (4)) = (u8) D_800B593C;
        *(u8 *)((unsigned char *)temp_v0_2 + (5)) = (u8) D_800B593C;
        *(u8 *)((unsigned char *)temp_v0_2 + (6)) = (u8) D_800B593C;
        if (sp38 == &D_800B5939) {
            *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20);
            *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
            *(s8 *)((unsigned char *)temp_v0_2 + (12)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (18)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0_2 + (16)) = 0x80;
        } else if (sp38 == &D_800B593A) {
            *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20);
            *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
            *(s8 *)((unsigned char *)temp_v0_2 + (12)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (18)) = 0xF0;
            *(s16 *)((unsigned char *)temp_v0_2 + (16)) = 0x80;
        } else {
            *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20);
            *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
            *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 0xA0;
            *(s8 *)((unsigned char *)temp_v0_2 + (12)) = 0;
            *(s16 *)((unsigned char *)temp_v0_2 + (16)) = 0x80;
            *(s16 *)((unsigned char *)temp_v0_2 + (18)) = 0x58;
        }
        AddPrim((D_8011ECA0 << 0xA) + 0x34 + D_800E8CA0, temp_v0_2);
        if (*sp38 != 0) {
            goto_tpage(0x200, 0x100, 0xD, 0, 2);
            return;
        }
        goto_tpage(0x380, 0, 0xD, 0, 2);
    }
}
