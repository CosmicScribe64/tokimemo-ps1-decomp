extern ? D_800E6294;
extern ? D_800E6298;
extern s32 D_800E7378;

void func_8013FBF0(void) {
    u8 sp42;
    u8 sp41;
    s32 temp_v0;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v1;
    s32 var_s0;
    u8 temp_v0_2;
    u8 var_t0;
    u8 var_t1;

    temp_v0 = func_800490F0(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0);
    *(s16 *)((unsigned char *)temp_v0 + (18)) = 0x40;
    *(s16 *)((unsigned char *)temp_v0 + (16)) = 0x20;
    func_8009EF84(temp_v0, 1);
    *(s8 *)((unsigned char *)temp_v0 + (4)) = 0xFF;
    *(s8 *)((unsigned char *)temp_v0 + (5)) = 0xFF;
    *(s8 *)((unsigned char *)temp_v0 + (6)) = 0xFF;
    *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xC8);
    *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + 0x28);
    *(s8 *)((unsigned char *)temp_v0 + (12)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0 + (13)) = 0x40;
    *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74(0x10, 0x1EF);
    func_8009EED0((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0);
    func_800494BC(0x340, 0x100, 1, 0, 0);
    temp_v0_2 = D_8015EDB4->unk84[1];
    var_s0 = 0;
    if (temp_v0_2 >= 0xC1U) {
        var_t0 = 0;
        var_t1 = 1;
    } else {
        var_s0 = 0;
        if (temp_v0_2 >= 0x81U) {
            var_t0 = 1;
        } else {
            var_s0 = 1;
            if (temp_v0_2 >= 0x41U) {
                var_s0 = 1;
                var_t0 = 1;
            } else {
                var_t0 = 0;
            }
        }
        var_t1 = 0;
    }
    sp42 = var_t0;
    sp41 = var_t1;
    temp_v1 = ((s32) (0x1000 - func_800A0070(D_800E7378 * 0xF0)) / 128) + 0x40;
    func_80049A40(-0xBF, 0x68, 0xE, (s16) ((u32) -(s32) D_8015EDB4->unk84[1] >> 2), 2, (((var_s0 * temp_v1) & 0xFF) << 0x10) + (((var_t0 * temp_v1) & 0xFF) << 8) + ((var_t1 * temp_v1) & 0xFF), 0x81);
    func_800494BC(0, 0, 2, 1, 0);
    temp_v0_3 = func_800490F0(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0_3);
    *(s16 *)((unsigned char *)temp_v0_3 + (16)) = 0x20;
    *(s16 *)((unsigned char *)temp_v0_3 + (18)) = 0x20;
    func_8009EF84(temp_v0_3, 1);
    *(s8 *)((unsigned char *)temp_v0_3 + (4)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0_3 + (5)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0_3 + (6)) = 0x80;
    *(s16 *)((unsigned char *)temp_v0_3 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xC8);
    *(s16 *)((unsigned char *)temp_v0_3 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + ((u32) (0xFF - D_8015EDB4->unk84[1]) >> 2) + 0x18);
    *(s8 *)((unsigned char *)temp_v0_3 + (12)) = 0xA0;
    *(s8 *)((unsigned char *)temp_v0_3 + (13)) = 0x60;
    *(s16 *)((unsigned char *)temp_v0_3 + (14)) = func_8009ED74(0x20, 0x1EF);
    func_8009EED0((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0_3);
    func_800494BC(0x340, 0x100, 1, 0, 0);
    temp_v0_4 = func_800490F0(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0_4);
    *(s16 *)((unsigned char *)temp_v0_4 + (18)) = 0x18;
    *(s16 *)((unsigned char *)temp_v0_4 + (16)) = 0x28;
    func_8009EF84(temp_v0_4, 1);
    *(s8 *)((unsigned char *)temp_v0_4 + (4)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0_4 + (5)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0_4 + (6)) = 0x80;
    *(s16 *)((unsigned char *)temp_v0_4 + (8)) = (s16) (*(&D_800E6294 + (D_8011ECA0 * 2)) - 0xCE);
    *(s16 *)((unsigned char *)temp_v0_4 + (10)) = (s16) (*(&D_800E6298 + (D_8011ECA0 * 2)) + 0x68);
    *(s8 *)((unsigned char *)temp_v0_4 + (12)) = 0xA0;
    *(s8 *)((unsigned char *)temp_v0_4 + (13)) = 0x40;
    *(s16 *)((unsigned char *)temp_v0_4 + (14)) = func_8009ED74(0x20, 0x1EF);
    func_8009EED0((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0_4);
    func_800494BC(0x340, 0x100, 1, 0, 0);
}
