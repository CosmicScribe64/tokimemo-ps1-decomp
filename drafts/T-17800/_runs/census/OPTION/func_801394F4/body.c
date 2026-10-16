void func_801394F4(void) {
    s32 *var_s0_2;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 temp_v0_3;
    s16 var_a0;
    s16 var_a1;
    s16 var_a2;
    s16 var_v1;
    s32 temp_s1;
    s32 temp_s1_2;
    s32 temp_s1_3;
    s32 temp_v1_2;
    s32 var_at;
    s32 var_at_2;
    s32 var_s2;
    s32 var_s3;
    u8 *var_s0;
    u8 *var_s0_3;
    u8 temp_v1;

    var_s3 = 0;
    var_s0 = ((OptActor44 *)D_8011ECD0);
    var_s2 = -0x64;
    do {
        temp_s1 = func_800AE0C0((D_8011ECF6 - *(s16 *)((unsigned char *)var_s0 + (6566))) & 0x3FF);
        func_8004AE54(func_800AE0C0((D_8011ECFA - *(s16 *)((unsigned char *)var_s0 + (6570))) & 0x1FF) + temp_s1, -0x8C, var_s2, 6);
        temp_s1_2 = func_800AE0C0((D_8011ECF6 - *(s16 *)((unsigned char *)var_s0 + (6566))) & 0x3FF);
        if (((func_800AE0C0((D_8011ECFA - *(s16 *)((unsigned char *)var_s0 + (6570))) & 0x1FF) + temp_s1_2) < 0x20) && (D_800E6280.unk_F88 & 0x20)) {
            temp_v1 = *(u8 *)((unsigned char *)var_s0 + (6531));
            if (temp_v1 & 0x10) {
                *(u8 *)((unsigned char *)var_s0 + (6531)) = (u8) (temp_v1 & 0xFFEF);
                if (*(s16 *)((unsigned char *)var_s0 + (6566)) < -0x200) {
                    *(s16 *)((unsigned char *)var_s0 + (6566)) = 0x200;
                }
                if (*(s16 *)((unsigned char *)var_s0 + (6566)) >= 0x201) {
                    *(s16 *)((unsigned char *)var_s0 + (6566)) = -0x200;
                }
                temp_v0 = *(s16 *)((unsigned char *)var_s0 + (6570));
                var_at = temp_v0 < 0x201;
                if (temp_v0 < -0x200) {
                    *(s16 *)((unsigned char *)var_s0 + (6570)) = 0x200;
                    var_at = *(s16 *)((unsigned char *)var_s0 + (6570)) < 0x201;
                }
                if (var_at == 0) {
                    *(s16 *)((unsigned char *)var_s0 + (6570)) = -0x200;
                }
            } else {
                *(u8 *)((unsigned char *)var_s0 + (6531)) = (u8) (temp_v1 | 0x10);
            }
        }
        if (!(*(u8 *)((unsigned char *)var_s0 + (6531)) & 0x10)) {
            temp_v0_2 = *(s16 *)((unsigned char *)var_s0 + (6536));
            *(s16 *)((unsigned char *)var_s0 + (6536)) = (s16) (temp_v0_2 + 1);
            *(s32 *)((unsigned char *)var_s0 + (6568)) = (s32) (*(s32 *)((unsigned char *)var_s0 + (6568)) + (func_800A0070(temp_v0_2 << 6) * 4));
            var_s3 += 1;
        }
        var_s2 += 8;
        var_s0 += 0x44;
    } while (var_s2 < 4);
    func_8004AE54(var_s3, 0, -0x64, 6);
    if (var_s3 == 0xD) {
        D_80120676 = -0x96;
        var_s0_2 = &D_8011ED14;
        D_8012067A = -0x64;
        var_a0 = -0x76;
        var_a1 = -0x56;
        var_a2 = -0x36;
        var_v1 = -0x16;
        do {
            *(s16 *)((unsigned char *)var_s0_2 + (6770)) = var_v1;
            var_v1 += 0x80;
            *(s16 *)((unsigned char *)var_s0_2 + (6634)) = var_a1;
            *(s16 *)((unsigned char *)var_s0_2 + (6702)) = var_a2;
            *(s16 *)((unsigned char *)var_s0_2 + (6566)) = var_a0;
            var_a0 += 0x80;
            var_a2 += 0x80;
            var_a1 += 0x80;
            *(s16 *)((unsigned char *)var_s0_2 + (6638)) = -0x64;
            *(s16 *)((unsigned char *)var_s0_2 + (6706)) = -0x64;
            *(s16 *)((unsigned char *)var_s0_2 + (6774)) = -0x64;
            var_s0_2 += 68;
            *(s16 *)((unsigned char *)var_s0_2 + (6298)) = -0x64;
        } while (var_v1 != 0x16A);
        return;
    }
    if (var_s3 >= 5) {
        var_s0_3 = ((OptActor44 *)D_8011ECD0);
        do {
            if (!(*(u8 *)((unsigned char *)var_s0_3 + (6531)) & 0x10)) {
                temp_s1_3 = *(s16 *)((unsigned char *)var_s0_3 + (6566)) - (func_800A0070((s32) *(s16 *)((unsigned char *)var_s0_3 + (6536))) / 32);
                temp_v1_2 = *(s16 *)((unsigned char *)var_s0_3 + (6570)) - (func_800A0140((s32) *(s16 *)((unsigned char *)var_s0_3 + (6536))) / 40);
                if ((temp_s1_3 < -0x10) || (var_at_2 = temp_v1_2 < -0x10, ((temp_s1_3 < 0x11) == 0))) {
                    if (temp_s1_3 > 0) {
                        *(s16 *)((unsigned char *)var_s0_3 + (6566)) = (s16) (*(s16 *)((unsigned char *)var_s0_3 + (6566)) - 1);
                    } else {
                        *(s16 *)((unsigned char *)var_s0_3 + (6566)) = (s16) (*(s16 *)((unsigned char *)var_s0_3 + (6566)) + 1);
                    }
                    var_at_2 = temp_v1_2 < -0x10;
                }
                if ((var_at_2 != 0) || (temp_v1_2 >= 0x11)) {
                    temp_v0_3 = *(s16 *)((unsigned char *)var_s0_3 + (6570));
                    if (temp_v1_2 > 0) {
                        *(s16 *)((unsigned char *)var_s0_3 + (6570)) = (s16) (temp_v0_3 - 1);
                    } else {
                        *(s16 *)((unsigned char *)var_s0_3 + (6570)) = (s16) (temp_v0_3 + 1);
                    }
                }
            }
            var_s0_3 += 0x44;
        } while (var_s0_3 != D_8011F044);
    }
}
