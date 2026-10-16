void func_8013F9C0(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_v0;
    s32 var_v0_2;

    temp_v0 = GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0);
    *(s16 *)((unsigned char *)temp_v0 + (16)) = 0x20;
    *(s16 *)((unsigned char *)temp_v0 + (18)) = 0x20;
    SetSemiTrans(temp_v0, 1);
    *(s8 *)((unsigned char *)temp_v0 + (4)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0 + (5)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0 + (6)) = 0x80;
    *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0xC8);
    *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + ((u32) (D_8015EDB4->unk2 & 0x40) >> 2) + 0x28);
    *(s8 *)((unsigned char *)temp_v0 + (12)) = 0xE0;
    *(s8 *)((unsigned char *)temp_v0 + (13)) = 0;
    if ((u8) D_8015EDB4->unk84[1] < 0x20U) {
        var_v0 = func_8009ED74(0x10, 0x1EF);
    } else {
        var_v0 = func_8009ED74(0, 0x1EF);
    }
    *(s16 *)((unsigned char *)temp_v0 + (14)) = (s16) var_v0;
    AddPrim((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0);
    temp_v0_2 = GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0_2);
    *(s16 *)((unsigned char *)temp_v0_2 + (16)) = 0x20;
    *(s16 *)((unsigned char *)temp_v0_2 + (18)) = 0x20;
    SetSemiTrans(temp_v0_2, 1);
    *(s8 *)((unsigned char *)temp_v0_2 + (4)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0_2 + (5)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0_2 + (6)) = 0x80;
    *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) (*(&D_800E6294 + (D_8011ECA0 * 2)) + 0xC8);
    *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) (*(&D_800E6298 + (D_8011ECA0 * 2)) + 0x48);
    *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 0x20;
    *(s8 *)((unsigned char *)temp_v0_2 + (12)) = 0xE0;
    if ((u8) D_8015EDB4->unk84[1] < 0x20U) {
        var_v0_2 = func_8009ED74(0x10, 0x1EF);
    } else {
        var_v0_2 = func_8009ED74(0, 0x1EF);
    }
    *(s16 *)((unsigned char *)temp_v0_2 + (14)) = (s16) var_v0_2;
    AddPrim((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0_2);
    dtd_on_tpage(0x340, 0x100, 1, 0, 0);
}
