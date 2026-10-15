void func_8013CCD0(void) {
    s32 sp78[16];
    s32 sp98[1];
    GameState *var_fp;
    GameState *var_s7;
    s16 temp_s6;
    s16 temp_v0_2;
    s32 temp_t0;
    s32 temp_t7;
    s32 var_s1;
    s32 var_s2;
    s32 var_s2_2;
    s32 var_s3;
    s8 temp_t6;
    u8 *var_s0;
    u8 temp_v0;
    u8 *temp_s0;
    u8 *temp_s0_2;
    u8 *temp_s0_3;

    var_s0 = D_8011ECD0;
    var_s2 = 0;
    do {
        func_80048F64(var_s2 + 0x60);
        *(s8 *)((unsigned char *)var_s0 + (6529)) = 4;
        *(s8 *)((unsigned char *)var_s0 + (6530)) = 0x40;
        *(s32 *)((unsigned char *)var_s0 + (6584)) = 0x40000000;
        *(s8 *)((unsigned char *)var_s0 + (6531)) = 0x84;
        *(u8 **)((unsigned char *)var_s0 + (6540)) = sprite_frame_table;
        *(u8 **)((unsigned char *)var_s0 + (6544)) = D_800C9A14;
        *(u8 **)((unsigned char *)var_s0 + (6580)) = D_800C9730;
        var_s2 += 1;
        var_s0 += 0x44;
        *(s8 *)((unsigned char *)var_s0 + (6464)) = 8;
        *(s8 *)((unsigned char *)var_s0 + (6465)) = 1;
        *(s16 *)((unsigned char *)var_s0 + (6498)) = 0x200;
        *(s16 *)((unsigned char *)var_s0 + (6480)) = (s16) D_800C9A54;
    } while (var_s2 < 0x22);
    D_80120668 = 0x42;
    D_80120676 = -0x10;
    D_8012067A = -0x1B;
    D_801230D4 = set_kanji_string(-0x80, -0x62, 4, (s32) "女の子からの評価", 0);
    var_s3 = 0x61;
    func_80053418((s32) &sp98, (s32) &sp78[0]);
    var_s7 = &D_800E6280;
    var_fp = &D_800E6280;
    var_s2_2 = 0;
    do {
        var_s7->unk_1BC[0xF].unk_14[0] = 0;
        if (((u32) (var_fp->unk_1BC[0].unk_0C.w << 0x1E) >> 0x1F) != 0) {
            temp_s0 = (var_s3 * 0x44) + D_8011ECD0;
            temp_t7 = (var_s2_2 / 6) * 0x82;
            temp_t0 = (var_s2_2 % 6) * 0x10;
            set_kanji_string((s16) (temp_t7 - 0x82), (s16) (temp_t0 - 0x48), 3, D_800B3538[var_s2_2], 0);
            temp_s6 = temp_t0 - 0x44;
            *(s16 *)((unsigned char *)temp_s0 + (42)) = temp_s6;
            *(s16 *)((unsigned char *)temp_s0 + (38)) = (s16) (temp_t7 - 0x32);
            *(s16 *)((unsigned char *)temp_s0 + (24)) = 0x61;
            temp_s0_2 = temp_s0 + 0x44;
            if ((&sp78[0])[var_s2_2] >= 0x1F4) {
                var_s1 = 0;
            } else {
                temp_v0 = *(u8 *)((unsigned char *)(sp + var_s2_2) + (152));
                if (temp_v0 < 6U) {
                    var_s1 = 6;
                } else if (temp_v0 < 0xBU) {
                    var_s1 = 5;
                } else if (temp_v0 < 0x15U) {
                    var_s1 = 4;
                } else if (temp_v0 < 0x29U) {
                    var_s1 = 3;
                } else {
                    var_s1 = 1;
                    if (temp_v0 < 0x3DU) {
                        var_s1 = 2;
                    }
                }
            }
            temp_v0_2 = var_fp->unk_1BC[0].unk_0A;
            *(s16 *)((unsigned char *)temp_s0_2 + (38)) = (s16) (temp_t7 - 0x22);
            *(s16 *)((unsigned char *)temp_s0_2 + (24)) = (s16) (var_s1 + 0x59);
            *(s16 *)((unsigned char *)temp_s0_2 + (42)) = temp_s6;
            var_s3 = var_s3 + 1 + 1;
            temp_s0_3 = temp_s0_2 + 0x44;
            if (temp_v0_2 >= 0x3C) {
                var_s3 += 1;
                temp_t6 = ((func_800AE0C0(temp_v0_2 - 0x3C) / 10) * -2) + 8;
                *(s8 *)((unsigned char *)temp_s0_3 + (6)) = temp_t6;
                if (!(temp_t6 & 0xFF)) {
                    *(s8 *)((unsigned char *)temp_s0_3 + (6)) = 1;
                }
                *(s16 *)((unsigned char *)temp_s0_3 + (24)) = 0x60;
                *(s16 *)((unsigned char *)temp_s0_3 + (38)) = (s16) (temp_t7 - 0x12);
                *(s16 *)((unsigned char *)temp_s0_3 + (42)) = temp_s6;
            }
            var_s7->unk_1BC[0xF].unk_14[0] = var_s1 + 1;
        }
        var_s2_2 += 1;
        var_s7 += 1;
        var_fp += 0x38;
    } while (var_s2_2 != 0xB);
    D_801230D4 = set_kanji_string(-0x80, -0x62, 4, (s32) "女の子からの評価", 0);
}
