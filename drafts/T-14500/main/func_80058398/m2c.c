void func_80058398(s32 arg0) {
    u8 *sp3C;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    u8 *temp_t4;

    temp_v0 = GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0);
    SetSemiTrans(temp_v0, 1);
    SetShadeTex(temp_v0, 0);
    *(s8 *)((unsigned char *)temp_v0 + (4)) = (s8) (0x80 - D_800B5940);
    *(s8 *)((unsigned char *)temp_v0 + (5)) = (s8) (0x80 - D_800B5940);
    *(s8 *)((unsigned char *)temp_v0 + (6)) = (s8) (0x80 - D_800B5940);
    if (arg0 == 1) {
        *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0);
        *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        *(s8 *)((unsigned char *)temp_v0 + (13)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (12)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (16)) = 0xC0;
        *(s16 *)((unsigned char *)temp_v0 + (18)) = 0xA0;
    } else {
        *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0);
        *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
        *(s8 *)((unsigned char *)temp_v0 + (13)) = 0xA0;
        *(s8 *)((unsigned char *)temp_v0 + (12)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (16)) = 0xC0;
        *(s16 *)((unsigned char *)temp_v0 + (18)) = 0x58;
    }
    AddPrim((D_8011ECA0 << 0xA) + 0x28 + D_800E8CA0, temp_v0);
    temp_t4 = &D_800B5938[arg0];
    sp3C = temp_t4;
    if (*temp_t4 == 0) {
        goto_tpage(0x140, 0x100, 0xA, 1, 2);
    } else {
        goto_tpage(0x2C0, 0, 0xA, 1, 2);
    }
    temp_v0_2 = GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0_2);
    SetSemiTrans(temp_v0_2, 1);
    SetShadeTex(temp_v0_2, 0);
    *(s8 *)((unsigned char *)temp_v0_2 + (4)) = (s8) (0x80 - D_800B5940);
    *(s8 *)((unsigned char *)temp_v0_2 + (5)) = (s8) (0x80 - D_800B5940);
    *(s8 *)((unsigned char *)temp_v0_2 + (6)) = (s8) (0x80 - D_800B5940);
    if (sp3C == &D_800B5939) {
        *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20);
        *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 0;
        *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        *(s8 *)((unsigned char *)temp_v0_2 + (12)) = 0;
        *(s16 *)((unsigned char *)temp_v0_2 + (18)) = 0xA0;
        *(s16 *)((unsigned char *)temp_v0_2 + (16)) = 0x80;
    } else {
        *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20);
        *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
        *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 0xA0;
        *(s8 *)((unsigned char *)temp_v0_2 + (12)) = 0;
        *(s16 *)((unsigned char *)temp_v0_2 + (16)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0_2 + (18)) = 0x58;
    }
    AddPrim((D_8011ECA0 << 0xA) + 0x28 + D_800E8CA0, temp_v0_2);
    if (*sp3C == 0) {
        goto_tpage(0x200, 0x100, 0xA, 1, 2);
    } else {
        goto_tpage(0x380, 0, 0xA, 1, 2);
    }
    temp_v0_3 = GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0_3);
    SetSemiTrans(temp_v0_3, 0);
    SetShadeTex(temp_v0_3, 0);
    *(u8 *)((unsigned char *)temp_v0_3 + (4)) = (u8) D_800B5940;
    *(u8 *)((unsigned char *)temp_v0_3 + (5)) = (u8) D_800B5940;
    *(u8 *)((unsigned char *)temp_v0_3 + (6)) = (u8) D_800B5940;
    if (sp3C == &D_800B5939) {
        *(s16 *)((unsigned char *)temp_v0_3 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0);
        *(s8 *)((unsigned char *)temp_v0_3 + (13)) = 0;
        *(s16 *)((unsigned char *)temp_v0_3 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        *(s8 *)((unsigned char *)temp_v0_3 + (12)) = 0;
        *(s16 *)((unsigned char *)temp_v0_3 + (18)) = 0xA0;
        *(s16 *)((unsigned char *)temp_v0_3 + (16)) = 0xC0;
    } else {
        *(s16 *)((unsigned char *)temp_v0_3 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0);
        *(s16 *)((unsigned char *)temp_v0_3 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
        *(s8 *)((unsigned char *)temp_v0_3 + (13)) = 0xA0;
        *(s8 *)((unsigned char *)temp_v0_3 + (12)) = 0;
        *(s16 *)((unsigned char *)temp_v0_3 + (16)) = 0xC0;
        *(s16 *)((unsigned char *)temp_v0_3 + (18)) = 0x58;
    }
    AddPrim((D_8011ECA0 << 0xA) + 0x30 + D_800E8CA0, temp_v0_3);
    if (*sp3C != 0) {
        goto_tpage(0x140, 0x100, 0xC, 0, 2);
    } else {
        goto_tpage(0x2C0, 0, 0xC, 0, 2);
    }
    temp_v0_4 = GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0_4);
    SetSemiTrans(temp_v0_4, 0);
    SetShadeTex(temp_v0_4, 0);
    *(u8 *)((unsigned char *)temp_v0_4 + (4)) = (u8) D_800B5940;
    *(u8 *)((unsigned char *)temp_v0_4 + (5)) = (u8) D_800B5940;
    *(u8 *)((unsigned char *)temp_v0_4 + (6)) = (u8) D_800B5940;
    if (sp3C == &D_800B5939) {
        *(s16 *)((unsigned char *)temp_v0_4 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20);
        *(s8 *)((unsigned char *)temp_v0_4 + (13)) = 0;
        *(s16 *)((unsigned char *)temp_v0_4 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        *(s8 *)((unsigned char *)temp_v0_4 + (12)) = 0;
        *(s16 *)((unsigned char *)temp_v0_4 + (18)) = 0xA0;
        *(s16 *)((unsigned char *)temp_v0_4 + (16)) = 0x80;
    } else {
        *(s16 *)((unsigned char *)temp_v0_4 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20);
        *(s16 *)((unsigned char *)temp_v0_4 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
        *(s8 *)((unsigned char *)temp_v0_4 + (13)) = 0xA0;
        *(s8 *)((unsigned char *)temp_v0_4 + (12)) = 0;
        *(s16 *)((unsigned char *)temp_v0_4 + (16)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0_4 + (18)) = 0x58;
    }
    AddPrim((D_8011ECA0 << 0xA) + 0x30 + D_800E8CA0, temp_v0_4);
    if (*sp3C != 0) {
        goto_tpage(0x200, 0x100, 0xC, 0, 2);
    } else {
        goto_tpage(0x380, 0, 0xC, 0, 2);
    }
    temp_v0_5 = GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0_5);
    SetSemiTrans(temp_v0_5, 0);
    SetShadeTex(temp_v0_5, 0);
    *(u8 *)((unsigned char *)temp_v0_5 + (4)) = (u8) D_800B5940;
    *(u8 *)((unsigned char *)temp_v0_5 + (5)) = (u8) D_800B5940;
    *(u8 *)((unsigned char *)temp_v0_5 + (6)) = (u8) D_800B5940;
    if (sp3C == &D_800B5939) {
        *(s16 *)((unsigned char *)temp_v0_5 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0);
        *(s8 *)((unsigned char *)temp_v0_5 + (13)) = 0;
        *(s16 *)((unsigned char *)temp_v0_5 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        *(s8 *)((unsigned char *)temp_v0_5 + (12)) = 0;
        *(s16 *)((unsigned char *)temp_v0_5 + (18)) = 0xA0;
        *(s16 *)((unsigned char *)temp_v0_5 + (16)) = 0xC0;
    } else {
        *(s16 *)((unsigned char *)temp_v0_5 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0);
        *(s16 *)((unsigned char *)temp_v0_5 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
        *(s8 *)((unsigned char *)temp_v0_5 + (13)) = 0xA0;
        *(s8 *)((unsigned char *)temp_v0_5 + (12)) = 0;
        *(s16 *)((unsigned char *)temp_v0_5 + (16)) = 0xC0;
        *(s16 *)((unsigned char *)temp_v0_5 + (18)) = 0x58;
    }
    AddPrim((D_8011ECA0 << 0xA) + 0x34 + D_800E8CA0, temp_v0_5);
    if (*sp3C == 0) {
        goto_tpage(0x140, 0x100, 0xD, 0, 2);
    } else {
        goto_tpage(0x2C0, 0, 0xD, 0, 2);
    }
    temp_v0_6 = GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0_6);
    SetSemiTrans(temp_v0_6, 0);
    SetShadeTex(temp_v0_6, 0);
    *(u8 *)((unsigned char *)temp_v0_6 + (4)) = (u8) D_800B5940;
    *(u8 *)((unsigned char *)temp_v0_6 + (5)) = (u8) D_800B5940;
    *(u8 *)((unsigned char *)temp_v0_6 + (6)) = (u8) D_800B5940;
    if (sp3C == &D_800B5939) {
        *(s16 *)((unsigned char *)temp_v0_6 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20);
        *(s8 *)((unsigned char *)temp_v0_6 + (13)) = 0;
        *(s16 *)((unsigned char *)temp_v0_6 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        *(s8 *)((unsigned char *)temp_v0_6 + (12)) = 0;
        *(s16 *)((unsigned char *)temp_v0_6 + (18)) = 0xA0;
        *(s16 *)((unsigned char *)temp_v0_6 + (16)) = 0x80;
    } else {
        *(s16 *)((unsigned char *)temp_v0_6 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20);
        *(s16 *)((unsigned char *)temp_v0_6 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
        *(s8 *)((unsigned char *)temp_v0_6 + (13)) = 0xA0;
        *(s8 *)((unsigned char *)temp_v0_6 + (12)) = 0;
        *(s16 *)((unsigned char *)temp_v0_6 + (16)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0_6 + (18)) = 0x58;
    }
    AddPrim((D_8011ECA0 << 0xA) + 0x34 + D_800E8CA0, temp_v0_6);
    if (*sp3C == 0) {
        goto_tpage(0x200, 0x100, 0xD, 0, 2);
        return;
    }
    goto_tpage(0x380, 0, 0xD, 0, 2);
}
