extern u8 D_800B3D98;
extern u8 D_800B3D99;

void func_8004B358(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp5C;
    s32 temp_s3;
    s32 temp_t2;
    s32 temp_v0;
    s32 var_s1;
    s32 var_s4;
    u8 temp_t8;
    u8 var_s2;
    u8 var_v0;

    if ((u8) D_800B3D80 < 4U) {

    } else if (D_800B3220 == 0) {

    } else {
        temp_t8 = *arg0;
        var_s4 = arg0;
        var_s1 = arg1;
        if (temp_t8 != 0) {
            sp5C = (arg3 & 7) + 0x1F8;
            var_v0 = temp_t8 & 0xFF;
            var_s2 = var_v0;
            do {
                temp_t2 = var_s2 & 0x1F;
                temp_s3 = var_v0 & 0x20;
                temp_v0 = GetWorkBase(0x14, D_8011ECA0);
                func_8009F0A4(temp_v0);
                SetSemiTrans(temp_v0, 0);
                *(s8 *)((unsigned char *)temp_v0 + (4)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0 + (5)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0 + (6)) = 0x80;
                *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + var_s1);
                *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg2);
                if (temp_s3 != 0) {
                    var_s1 += 6;
                } else {
                    var_s1 += 8;
                }
                if (temp_s3 != 0) {
                    *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) ((D_800B3D98 + (temp_t2 * 8)) - 8);
                    *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) D_800B3D99;
                } else {
                    *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) ((D_800B3D98 + (temp_t2 * 8)) - 8);
                    *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) (D_800B3D99 - 0x10);
                }
                if (temp_s3 != 0) {
                    *(s16 *)((unsigned char *)temp_v0 + (18)) = 0xC;
                    *(s16 *)((unsigned char *)temp_v0 + (16)) = 6;
                } else {
                    *(s16 *)((unsigned char *)temp_v0 + (16)) = 8;
                    *(s16 *)((unsigned char *)temp_v0 + (18)) = 8;
                }
                *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74(((arg3 & 8) * 2) + 0x100, sp5C);
                AddPrim((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0);
                var_v0 = *(u8 *)((unsigned char *)var_s4 + (1));
                var_s4 += 1;
                var_s2 = var_v0;
            } while (var_v0 != 0);
        }
        dtd_on_tpage(0x2C0, 0x100, 1, 0, 0);
    }
}
