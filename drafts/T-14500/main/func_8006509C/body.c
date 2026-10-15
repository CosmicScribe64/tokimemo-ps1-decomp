void func_8006509C(void) {
    GameState *sp5C;
    s32 *var_v0_3;
    s32 *var_v1;
    s32 *var_v1_2;
    GameState *var_a1;
    s16 *var_v1_3;
    s16 temp_t0;
    s16 temp_t1;
    s16 temp_t2;
    s16 temp_t3;
    s16 temp_t4;
    s16 temp_t5;
    s16 temp_t8;
    s16 temp_t9;
    s32 var_a0;
    s32 var_at;
    u16 var_s1;
    u32 var_s1_2;
    u32 var_s1_3;
    u8 *var_s0;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 *var_v0_4;
    u8 *var_v0_5;

    var_a1 = &D_800E6280;
    if (D_800E71EC == 2) {
        D_800E71ED = 0xF;
    }
    var_s1 = 0;
    if (D_800E71ED != 0) {
        var_v0 = D_80125C90;
        do {
            *var_v0 = 1;
            var_s1 += 1;
            var_v0 += 1;
        } while (var_s1 < (u8) D_800E6280.unk_F6D);
        var_s1 = 0;
    }
    if (D_800E71ED != 0) {
        var_s0 = D_8011ECD0;
        do {
            sp5C = var_a1;
            func_80048F64(var_s1 + 0x50);
            *(s8 *)((unsigned char *)var_s0 + (5441)) = 3;
            *(s8 *)((unsigned char *)var_s0 + (5442)) = 1;
            *(s32 *)((unsigned char *)var_s0 + (5496)) = 0x40000000;
            *(s8 *)((unsigned char *)var_s0 + (5443)) = 0x84;
            *(u8 **)((unsigned char *)var_s0 + (5452)) = sprite_frame_table;
            *(u8 **)((unsigned char *)var_s0 + (5456)) = D_800C9A14;
            *(u8 **)((unsigned char *)var_s0 + (5492)) = D_800C9730;
            *(s8 *)((unsigned char *)var_s0 + (5444)) = 8;
            *(s8 *)((unsigned char *)var_s0 + (5445)) = 1;
            *(u16 *)((unsigned char *)var_s0 + (5462)) = var_s1;
            *(s8 *)((unsigned char *)var_s0 + (5446)) = 1;
            *(s16 *)((unsigned char *)var_s0 + (5460)) = (s16) D_800C9A54;
            func_80048F64(var_s1 + 0x40);
            *(s8 *)((unsigned char *)var_s0 + (4353)) = 4;
            *(s8 *)((unsigned char *)var_s0 + (4354)) = 0x40;
            *(s32 *)((unsigned char *)var_s0 + (4408)) = 0x40000000;
            *(s8 *)((unsigned char *)var_s0 + (4355)) = 0x84;
            *(u8 **)((unsigned char *)var_s0 + (4364)) = sprite_frame_table;
            *(u8 **)((unsigned char *)var_s0 + (4368)) = D_800C9A14;
            *(u8 **)((unsigned char *)var_s0 + (4404)) = D_800C9730;
            *(s8 *)((unsigned char *)var_s0 + (4356)) = 8;
            *(s8 *)((unsigned char *)var_s0 + (4357)) = 1;
            *(s16 *)((unsigned char *)var_s0 + (4376)) = 2;
            *(s16 *)((unsigned char *)var_s0 + (4372)) = (s16) D_800C9A54;
            var_s1 += 1;
            var_s0 += 0x44;
        } while (var_s1 < (u8) var_a1->unk_F6D);
    }
    D_801205C9 = 3;
    D_80120189 = 4;
    D_801205DE = 0xF;
    D_801205EE = 0;
    D_801205F2 = 0;
    D_8012019E = 0xF;
    D_801201AE = 0;
    D_801201B2 = 0;
    if (D_800E71EC != 0) {
        if (D_800E71EC != 1) {
            var_a0 = 0;
            if (D_800E71EC != 2) {

            } else {
                var_v0_2 = D_80125C10;
loop_22:
                var_at = var_a0 < 2;
                if ((*(s32 *)var_v0_2) == 0) {
                    var_at = var_a0 < 2;
                    if (*(s16 *)((unsigned char *)var_v0_2 + (2)) == 0) {
                        var_a0 += 1;
                        var_at = var_a0 < 2;
                    }
                }
                var_v0_2 += 4;
                if (var_at == 0) {
                    *D_80125C10 = D_800B5AE4;
                    D_80125C12 = D_800B5AE6;
                    D_80125C14 = D_800B5AE8;
                    D_80125C16 = D_800B5AEA;
                    D_80125C18 = D_800B5AEC;
                    var_v0_3 = &D_800B5AF0;
                    var_v1 = &D_80125C1C;
                    D_80125C1A = D_800B5AEE;
                    do {
                        temp_t0 = *(s16 *)((unsigned char *)var_v0_3 + (4));
                        temp_t1 = *(s16 *)((unsigned char *)var_v0_3 + (6));
                        temp_t2 = *(s16 *)((unsigned char *)var_v0_3 + (8));
                        temp_t3 = *(s16 *)((unsigned char *)var_v0_3 + (10));
                        temp_t4 = *(s16 *)((unsigned char *)var_v0_3 + (12));
                        temp_t5 = *(s16 *)((unsigned char *)var_v0_3 + (14));
                        temp_t8 = (*(s32 *)var_v0_3);
                        temp_t9 = *(s16 *)((unsigned char *)var_v0_3 + (2));
                        var_v0_3 += 0x10;
                        var_v1 += 0x10;
                        *(s16 *)((unsigned char *)var_v1 + (-12)) = temp_t0;
                        *(s16 *)((unsigned char *)var_v1 + (-10)) = temp_t1;
                        *(s16 *)((unsigned char *)var_v1 + (-8)) = temp_t2;
                        *(s16 *)((unsigned char *)var_v1 + (-6)) = temp_t3;
                        *(s16 *)((unsigned char *)var_v1 + (-4)) = temp_t4;
                        *(s16 *)((unsigned char *)var_v1 + (-2)) = temp_t5;
                        *(s16 *)((unsigned char *)var_v1 + (-16)) = temp_t8;
                        *(s16 *)((unsigned char *)var_v1 + (-14)) = temp_t9;
                    } while (var_v0_3 != &D_800B5B20);
                } else if (var_v0_2 != &D_80125C4C) {
                    goto loop_22;
                }
            }
        } else {
            var_s1_2 = 0;
            if (D_800E71ED != 0) {
                var_v1_2 = &D_800B5AA4;
                var_v0_4 = D_80125C10;
                do {
                    (*(s32 *)var_v0_4) = (s16) (*(s32 *)var_v1_2);
                    *(s16 *)((unsigned char *)var_v0_4 + (2)) = (s16) *(s16 *)((unsigned char *)var_v1_2 + (2));
                    var_s1_2 += 1;
                    var_v0_4 += 4;
                    var_v1_2 += 4;
                } while (var_s1_2 < (u8) var_a1->unk_F6D);
            }
        }
    } else {
        var_s1_3 = 0;
        if (D_800E71ED != 0) {
            var_v1_3 = &D_800B5AE4;
            var_v0_5 = D_80125C10;
            do {
                (*(s32 *)var_v0_5) = (s16) (*(s32 *)var_v1_3);
                *(s16 *)((unsigned char *)var_v0_5 + (2)) = (s16) *(s16 *)((unsigned char *)var_v1_3 + (2));
                var_s1_3 += 1;
                var_v0_5 += 4;
                var_v1_3 += 4;
            } while (var_s1_3 < (u8) var_a1->unk_F6D);
        }
    }
    func_800654D0((s32) D_800E71ED);
    LoadSquare(0x120U, 0x1F1U, 0x10U, 1U, &D_800C6200);
    LoadSquare(0x120U, 0x1F2U, 0x10U, 1U, D_800C6220);
    LoadSquare(0x120U, 0x1F3U, 0x10U, 1U, &D_800C6240);
}
