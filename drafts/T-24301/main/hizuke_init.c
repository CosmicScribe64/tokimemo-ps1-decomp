/* T-24301 draft, ~225 rows: the loop is close (the original stores unk_04/05/14 after the pointer
 * increment); after it the original keeps unk_03F/unk_040 promoted in $s0 across the calls, reloading
 * after each one, and narrows them to s16 for func_80046590. */
void hizuke_init(void) {
    s32 i;
    u8 event;

    for (i = 0; i < 8; i++) {
        func_80048F64(i + 16);
        ((DisplayActor44 *)D_8011ECD0)[i + 16].unk_01 = 4;
        ((DisplayActor44 *)D_8011ECD0)[i + 16].unk_02 = 0x40;
        ((DisplayActor44 *)D_8011ECD0)[i + 16].unk_38 = 0x40000000;
        ((DisplayActor44 *)D_8011ECD0)[i + 16].unk_03 = 0x84;
        ((DisplayActor44 *)D_8011ECD0)[i + 16].unk_0C = sprite_frame_table;
        ((DisplayActor44 *)D_8011ECD0)[i + 16].unk_10 = D_800C9A14;
        ((DisplayActor44 *)D_8011ECD0)[i + 16].unk_34 = D_800C9730;
        ((DisplayActor44 *)D_8011ECD0)[i + 16].unk_04 = 8;
        ((DisplayActor44 *)D_8011ECD0)[i + 16].unk_05 = 1;
        ((DisplayActor44 *)D_8011ECD0)[i + 16].unk_14 = D_800C9A54;
    }
    ((DisplayActor44 *)D_8011ECD0)[17].unk_18 = func_80046590(D_800E6280.unk_03E, 2) + 0x44;
    ((DisplayActor44 *)D_8011ECD0)[17].unk_26 = 0x8C;
    ((DisplayActor44 *)D_8011ECD0)[17].unk_2A = -0x41;
    ((DisplayActor44 *)D_8011ECD0)[18].unk_18 = func_80046590(D_800E6280.unk_03E, 1) + 0x44;
    ((DisplayActor44 *)D_8011ECD0)[18].unk_26 = 0x94;
    ((DisplayActor44 *)D_8011ECD0)[18].unk_2A = -0x41;
    if (D_800E6280.unk_03F >= 10) {
        ((DisplayActor44 *)D_8011ECD0)[19].unk_18 = func_80046590(D_800E6280.unk_03F, 2) + 0x44;
        ((DisplayActor44 *)D_8011ECD0)[19].unk_26 = 0x8C;
        ((DisplayActor44 *)D_8011ECD0)[19].unk_2A = -0x22;
        ((DisplayActor44 *)D_8011ECD0)[20].unk_18 = func_80046590(D_800E6280.unk_03F, 1) + 0x44;
        ((DisplayActor44 *)D_8011ECD0)[20].unk_26 = 0x94;
        ((DisplayActor44 *)D_8011ECD0)[20].unk_2A = -0x22;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[19].unk_03 = 0;
        ((DisplayActor44 *)D_8011ECD0)[19].unk_18 = func_80046590(D_800E6280.unk_03F, 2) + 0x44;
        ((DisplayActor44 *)D_8011ECD0)[19].unk_26 = 0xFF;
        ((DisplayActor44 *)D_8011ECD0)[19].unk_2A = 0xFF;
        ((DisplayActor44 *)D_8011ECD0)[20].unk_18 = func_80046590(D_800E6280.unk_03F, 1) + 0x44;
        ((DisplayActor44 *)D_8011ECD0)[20].unk_26 = 0x90;
        ((DisplayActor44 *)D_8011ECD0)[20].unk_2A = -0x22;
    }
    if (D_800E6280.unk_040 >= 10) {
        ((DisplayActor44 *)D_8011ECD0)[21].unk_18 = func_80046590(D_800E6280.unk_040, 2) + 0x44;
        ((DisplayActor44 *)D_8011ECD0)[21].unk_26 = 0x8C;
        ((DisplayActor44 *)D_8011ECD0)[21].unk_2A = -1;
        ((DisplayActor44 *)D_8011ECD0)[22].unk_18 = func_80046590(D_800E6280.unk_040, 1) + 0x44;
        ((DisplayActor44 *)D_8011ECD0)[22].unk_26 = 0x94;
        ((DisplayActor44 *)D_8011ECD0)[22].unk_2A = -1;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[21].unk_03 = 0;
        ((DisplayActor44 *)D_8011ECD0)[21].unk_18 = func_80046590(D_800E6280.unk_040, 2) + 0x44;
        ((DisplayActor44 *)D_8011ECD0)[21].unk_26 = 0xFF;
        ((DisplayActor44 *)D_8011ECD0)[21].unk_2A = 0xFF;
        ((DisplayActor44 *)D_8011ECD0)[22].unk_18 = func_80046590(D_800E6280.unk_040, 1) + 0x44;
        ((DisplayActor44 *)D_8011ECD0)[22].unk_26 = 0x90;
        ((DisplayActor44 *)D_8011ECD0)[22].unk_2A = -1;
    }
    ((DisplayActor44 *)D_8011ECD0)[23].unk_18 = D_800E6280.unk_041 + 0x52;
    ((DisplayActor44 *)D_8011ECD0)[23].unk_26 = 0x92;
    ((DisplayActor44 *)D_8011ECD0)[23].unk_2A = 0x1F;
    if (D_800E6280.unk_041 == 5) {
        ((DisplayActor44 *)D_8011ECD0)[23].unk_43 = 0x43;
    } else if (D_800E6280.unk_041 == 6) {
        ((DisplayActor44 *)D_8011ECD0)[23].unk_43 = 0x44;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[23].unk_43 = 0;
    }
    event = D_800E6280.unk_044[0].unk_00[D_800E6280.unk_040];
    if (event == 1 || event == 2) {
        ((DisplayActor44 *)D_8011ECD0)[23].unk_43 = 0x44;
    }
    ((DisplayActor44 *)D_8011ECD0)[16].unk_18 = 0x3C;
    ((DisplayActor44 *)D_8011ECD0)[16].unk_26 = 0x90;
    ((DisplayActor44 *)D_8011ECD0)[16].unk_2A = -0x12;
}
