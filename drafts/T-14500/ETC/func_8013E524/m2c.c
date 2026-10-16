? func_80049450(?);                                 /* extern */
? func_8009EFB0(s32, ?);                            /* extern */

void func_8013E524(s32 a, s32 b) {
    s16 temp_v0_2;
    s32 temp_v0;
    void *temp_s0;

    temp_v0 = func_800490F0(0x18, D_8011ECA0);
    func_8009F02C(temp_v0);
    func_8009EF84(temp_v0, 1);
    func_8009EFB0(temp_v0, 0);
    *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) a;
    *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + 0x2B);
    temp_v0_2 = a + b;
    *(s16 *)((unsigned char *)temp_v0 + (12)) = temp_v0_2;
    *(s16 *)((unsigned char *)temp_v0 + (16)) = (s16) a;
    *(s16 *)((unsigned char *)temp_v0 + (14)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + 0x2B);
    *(s16 *)((unsigned char *)temp_v0 + (20)) = temp_v0_2;
    *(s16 *)((unsigned char *)temp_v0 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + 0x37);
    *(s8 *)((unsigned char *)temp_v0 + (6)) = 0;
    *(s16 *)((unsigned char *)temp_v0 + (22)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + 0x37);
    *(s8 *)((unsigned char *)temp_v0 + (5)) = 0;
    *(s8 *)((unsigned char *)temp_v0 + (4)) = 0;
    func_8009EED0((D_8011ECA0 << 0xA) + 0xC + D_800E8CA0, temp_v0);
    temp_s0 = func_800490F0(0xC, D_8011ECA0);
    func_8009D294((s32) temp_s0, 0, 0, func_8009ECB0(1, 0, 0, 0), 0);
    func_8009EED0((D_8011ECA0 << 0xA) + 0xC + D_800E8CA0, (s32) temp_s0);
    func_80049450(3);
}
