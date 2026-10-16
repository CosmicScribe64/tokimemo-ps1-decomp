void hizuke_init(void) {
    DisplayActor44 *var_s0;
    s32 var_s1;
    u8 temp_v0;

    var_s0 = D_8011ECD0;
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
        var_s0 += 0x44;
        *(s8 *)((unsigned char *)var_s0 + (1024)) = 8;
        *(s8 *)((unsigned char *)var_s0 + (1025)) = 1;
        *(s16 *)((unsigned char *)var_s0 + (1040)) = (s16) D_800C9A54;
    } while (var_s1 != 8);
    *(s16 *)((unsigned char *)D_8011ECD0 + (1180)) = (s16) (func_80046590((s16) D_800E6280.unk_03E, 2U) + 0x44);
    *(s16 *)((unsigned char *)D_8011ECD0 + (1194)) = 0x8C;
    *(s16 *)((unsigned char *)D_8011ECD0 + (1198)) = -0x41;
    *(s16 *)((unsigned char *)D_8011ECD0 + (1248)) = (s16) (func_80046590((s16) D_800E6280.unk_03E, 1U) + 0x44);
    *(s16 *)((unsigned char *)D_8011ECD0 + (1262)) = 0x94;
    *(s16 *)((unsigned char *)D_8011ECD0 + (1266)) = -0x41;
    if ((u8) D_800E6280.unk_03F >= 0xAU) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (1316)) = (s16) (func_80046590((s16) D_800E6280.unk_03F, 2U) + 0x44);
        *(s16 *)((unsigned char *)D_8011ECD0 + (1330)) = 0x8C;
        *(s16 *)((unsigned char *)D_8011ECD0 + (1334)) = -0x22;
        *(s16 *)((unsigned char *)D_8011ECD0 + (1384)) = (s16) (func_80046590((s16) D_800E6280.unk_03F, 1U) + 0x44);
        *(s16 *)((unsigned char *)D_8011ECD0 + (1398)) = 0x94;
        *(s16 *)((unsigned char *)D_8011ECD0 + (1402)) = -0x22;
    } else {
        *(s8 *)((unsigned char *)D_8011ECD0 + (1295)) = 0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (1316)) = (s16) (func_80046590((s16) D_800E6280.unk_03F, 2U) + 0x44);
        *(s16 *)((unsigned char *)D_8011ECD0 + (1330)) = 0xFF;
        *(s16 *)((unsigned char *)D_8011ECD0 + (1334)) = 0xFF;
        *(s16 *)((unsigned char *)D_8011ECD0 + (1384)) = (s16) (func_80046590((s16) D_800E6280.unk_03F, 1U) + 0x44);
        *(s16 *)((unsigned char *)D_8011ECD0 + (1398)) = 0x90;
        *(s16 *)((unsigned char *)D_8011ECD0 + (1402)) = -0x22;
    }
    if ((u8) D_800E6280.unk_040 >= 0xAU) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (1452)) = (s16) (func_80046590((s16) D_800E6280.unk_040, 2U) + 0x44);
        *(s16 *)((unsigned char *)D_8011ECD0 + (1466)) = 0x8C;
        *(s16 *)((unsigned char *)D_8011ECD0 + (1470)) = -1;
        *(s16 *)((unsigned char *)D_8011ECD0 + (1520)) = (s16) (func_80046590((s16) D_800E6280.unk_040, 1U) + 0x44);
        *(s16 *)((unsigned char *)D_8011ECD0 + (1534)) = 0x94;
        *(s16 *)((unsigned char *)D_8011ECD0 + (1538)) = -1;
    } else {
        *(s8 *)((unsigned char *)D_8011ECD0 + (1431)) = 0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (1452)) = (s16) (func_80046590((s16) D_800E6280.unk_040, 2U) + 0x44);
        *(s16 *)((unsigned char *)D_8011ECD0 + (1466)) = 0xFF;
        *(s16 *)((unsigned char *)D_8011ECD0 + (1470)) = 0xFF;
        *(s16 *)((unsigned char *)D_8011ECD0 + (1520)) = (s16) (func_80046590((s16) D_800E6280.unk_040, 1U) + 0x44);
        *(s16 *)((unsigned char *)D_8011ECD0 + (1534)) = 0x90;
        *(s16 *)((unsigned char *)D_8011ECD0 + (1538)) = -1;
    }
    *(s16 *)((unsigned char *)D_8011ECD0 + (1588)) = (s16) (D_800E6280.unk_041 + 0x52);
    *(s16 *)((unsigned char *)D_8011ECD0 + (1602)) = 0x92;
    *(s16 *)((unsigned char *)D_8011ECD0 + (1606)) = 0x1F;
    if (D_800E6280.unk_041 == 5) {
        *(s8 *)((unsigned char *)D_8011ECD0 + (1631)) = 0x43;
    } else if (D_800E6280.unk_041 == 6) {
        *(s8 *)((unsigned char *)D_8011ECD0 + (1631)) = 0x44;
    } else {
        *(s8 *)((unsigned char *)D_8011ECD0 + (1631)) = 0;
    }
    temp_v0 = D_800E6280.unk_044[0].unk_00[D_800E6280.unk0];
    if ((temp_v0 == 1) || (temp_v0 == 2)) {
        *(s8 *)((unsigned char *)D_8011ECD0 + (1631)) = 0x44;
    }
    *(s16 *)((unsigned char *)D_8011ECD0 + (1112)) = 0x3C;
    *(s16 *)((unsigned char *)D_8011ECD0 + (1126)) = 0x90;
    *(s16 *)((unsigned char *)D_8011ECD0 + (1130)) = -0x12;
}
