void hizuke_init(void) {
    DisplayActor44 *var_s0;
    s32 var_s1;
    u8 temp_v0;

    var_s0 = ((DisplayActor44 *)D_8011ECD0);
    var_s1 = 0;
    do {
        func_80048F64(var_s1 + 0x10);
        *(s8 *)((unsigned char *)var_s0 + (1089)) = 4;
        *(s8 *)((unsigned char *)var_s0 + (1090)) = 0x40;
        *(s32 *)((unsigned char *)var_s0 + (1144)) = 0x40000000;
        *(s8 *)((unsigned char *)var_s0 + (1091)) = 0x84;
        *(u8 **)((unsigned char *)var_s0 + (1100)) = sprite_frame_table;
        *(u8 **)((unsigned char *)var_s0 + (1104)) = D_800C9A14;
        *(u8 **)((unsigned char *)var_s0 + (1140)) = D_800C9730;
        var_s1 += 1;
        var_s0 += 1;
        *(s8 *)((unsigned char *)var_s0 + (1024)) = 8;
        *(s8 *)((unsigned char *)var_s0 + (1025)) = 1;
        *(s16 *)((unsigned char *)var_s0 + (1040)) = (s16) D_800C9A54;
    } while (var_s1 != 8);
    ((DisplayActor44 *)D_8011ECD0)[17].unk_18 = (s16) (func_80046590((s16) D_800E6280.unk_03E, 2U) + 0x44);
    ((DisplayActor44 *)D_8011ECD0)[17].unk_26 = 0x8C;
    ((DisplayActor44 *)D_8011ECD0)[17].unk_2A = -0x41;
    ((DisplayActor44 *)D_8011ECD0)[18].unk_18 = (s16) (func_80046590((s16) D_800E6280.unk_03E, 1U) + 0x44);
    ((DisplayActor44 *)D_8011ECD0)[18].unk_26 = 0x94;
    ((DisplayActor44 *)D_8011ECD0)[18].unk_2A = -0x41;
    if ((u8) D_800E6280.unk_03F >= 0xAU) {
        ((DisplayActor44 *)D_8011ECD0)[19].unk_18 = (s16) (func_80046590((s16) D_800E6280.unk_03F, 2U) + 0x44);
        ((DisplayActor44 *)D_8011ECD0)[19].unk_26 = 0x8C;
        ((DisplayActor44 *)D_8011ECD0)[19].unk_2A = -0x22;
        ((DisplayActor44 *)D_8011ECD0)[20].unk_18 = (s16) (func_80046590((s16) D_800E6280.unk_03F, 1U) + 0x44);
        ((DisplayActor44 *)D_8011ECD0)[20].unk_26 = 0x94;
        ((DisplayActor44 *)D_8011ECD0)[20].unk_2A = -0x22;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[19].unk_03 = 0;
        ((DisplayActor44 *)D_8011ECD0)[19].unk_18 = (s16) (func_80046590((s16) D_800E6280.unk_03F, 2U) + 0x44);
        ((DisplayActor44 *)D_8011ECD0)[19].unk_26 = 0xFF;
        ((DisplayActor44 *)D_8011ECD0)[19].unk_2A = 0xFF;
        ((DisplayActor44 *)D_8011ECD0)[20].unk_18 = (s16) (func_80046590((s16) D_800E6280.unk_03F, 1U) + 0x44);
        ((DisplayActor44 *)D_8011ECD0)[20].unk_26 = 0x90;
        ((DisplayActor44 *)D_8011ECD0)[20].unk_2A = -0x22;
    }
    if ((u8) D_800E6280.unk_040 >= 0xAU) {
        ((DisplayActor44 *)D_8011ECD0)[21].unk_18 = (s16) (func_80046590((s16) D_800E6280.unk_040, 2U) + 0x44);
        ((DisplayActor44 *)D_8011ECD0)[21].unk_26 = 0x8C;
        ((DisplayActor44 *)D_8011ECD0)[21].unk_2A = -1;
        ((DisplayActor44 *)D_8011ECD0)[22].unk_18 = (s16) (func_80046590((s16) D_800E6280.unk_040, 1U) + 0x44);
        ((DisplayActor44 *)D_8011ECD0)[22].unk_26 = 0x94;
        ((DisplayActor44 *)D_8011ECD0)[22].unk_2A = -1;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[21].unk_03 = 0;
        ((DisplayActor44 *)D_8011ECD0)[21].unk_18 = (s16) (func_80046590((s16) D_800E6280.unk_040, 2U) + 0x44);
        ((DisplayActor44 *)D_8011ECD0)[21].unk_26 = 0xFF;
        ((DisplayActor44 *)D_8011ECD0)[21].unk_2A = 0xFF;
        ((DisplayActor44 *)D_8011ECD0)[22].unk_18 = (s16) (func_80046590((s16) D_800E6280.unk_040, 1U) + 0x44);
        ((DisplayActor44 *)D_8011ECD0)[22].unk_26 = 0x90;
        ((DisplayActor44 *)D_8011ECD0)[22].unk_2A = -1;
    }
    ((DisplayActor44 *)D_8011ECD0)[23].unk_18 = (s16) (D_800E6280.unk_041 + 0x52);
    ((DisplayActor44 *)D_8011ECD0)[23].unk_26 = 0x92;
    ((DisplayActor44 *)D_8011ECD0)[23].unk_2A = 0x1F;
    if (D_800E6280.unk_041 == 5) {
        ((DisplayActor44 *)D_8011ECD0)[23].unk_43 = 0x43;
    } else if (D_800E6280.unk_041 == 6) {
        ((DisplayActor44 *)D_8011ECD0)[23].unk_43 = 0x44;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[23].unk_43 = 0;
    }
    temp_v0 = D_800E6280.unk_044[0].unk_00[D_800E6280.unk0];
    if ((temp_v0 == 1) || (temp_v0 == 2)) {
        ((DisplayActor44 *)D_8011ECD0)[23].unk_43 = 0x44;
    }
    ((DisplayActor44 *)D_8011ECD0)[16].unk_18 = 0x3C;
    ((DisplayActor44 *)D_8011ECD0)[16].unk_26 = 0x90;
    ((DisplayActor44 *)D_8011ECD0)[16].unk_2A = -0x12;
}
