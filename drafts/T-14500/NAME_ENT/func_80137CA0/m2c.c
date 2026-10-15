extern u8 D_800E62BA;
extern ? D_8014D014;

void func_80137CA0(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v0_3;
    void *temp_v0_2;
    void *temp_v0_4;

    temp_v0 = func_800490F0(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0);
    func_8009EF84(temp_v0, 0);
    *(u8 *)((unsigned char *)temp_v0 + (4)) = (u8) D_800E62BA;
    *(u8 *)((unsigned char *)temp_v0 + (5)) = (u8) D_800E62BA;
    *(u8 *)((unsigned char *)temp_v0 + (6)) = (u8) D_800E62BA;
    *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + arg1) - 8);
    *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg2);
    temp_v0_2 = ((arg0 / 10) * 2) + &D_8014D014;
    *(u8 *)((unsigned char *)temp_v0 + (12)) = (u8) temp_v0_2->unk0;
    *(s16 *)((unsigned char *)temp_v0 + (18)) = 0x10;
    *(s16 *)((unsigned char *)temp_v0 + (16)) = 8;
    *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) *(u8 *)((unsigned char *)temp_v0_2 + (1));
    *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74(0x10, 0x1F0);
    func_8009EED0((D_8011ECA0 << 0xA) + 8 + D_800E8CA0, temp_v0);
    temp_v0_3 = func_800490F0(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0_3);
    func_8009EF84(temp_v0_3, 0);
    *(u8 *)((unsigned char *)temp_v0_3 + (4)) = (u8) D_800E62BA;
    *(u8 *)((unsigned char *)temp_v0_3 + (5)) = (u8) D_800E62BA;
    *(u8 *)((unsigned char *)temp_v0_3 + (6)) = (u8) D_800E62BA;
    *(s16 *)((unsigned char *)temp_v0_3 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg1);
    *(s16 *)((unsigned char *)temp_v0_3 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg2);
    temp_v0_4 = ((arg0 % 10) * 2) + &D_8014D014;
    *(u8 *)((unsigned char *)temp_v0_3 + (12)) = (u8) temp_v0_4->unk0;
    *(s16 *)((unsigned char *)temp_v0_3 + (18)) = 0x10;
    *(s16 *)((unsigned char *)temp_v0_3 + (16)) = 8;
    *(u8 *)((unsigned char *)temp_v0_3 + (13)) = (u8) *(u8 *)((unsigned char *)temp_v0_4 + (1));
    *(s16 *)((unsigned char *)temp_v0_3 + (14)) = func_8009ED74(0x10, 0x1F0);
    func_8009EED0((D_8011ECA0 << 0xA) + 8 + D_800E8CA0, temp_v0_3);
}
