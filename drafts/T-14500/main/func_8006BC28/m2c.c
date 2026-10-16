extern s8 D_8011ECD1;
extern s8 D_8011ECD2;
extern s8 D_8011ECD4;
extern s8 D_8011ECD5;
extern u8 *D_8011ECDC;
extern u8 *D_8011ECE0;
extern s16 D_8011ECE4;
extern s16 D_8011ECE6;
extern u8 *D_8011ED04;
extern s32 D_8011ED08;

void func_8006BC28(s32 arg0) {
    s16 sp2E;
    s16 sp2C;

    sp2E = D_8011ECF6;
    sp2C = D_8011ECFA;
    func_80048F64(0);
    D_8011ECD1 = 1;
    D_8011ECD2 = 0x40;
    D_8011ED08 = 0;
    D_8011ECD3 = 0x84;
    D_8011ECDC = sprite_frame_table;
    D_8011ECE0 = D_800C9A14;
    D_8011ED04 = D_800C9730;
    D_8011ECE4 = D_800C9A54;
    D_8011ECD4 = 8;
    D_8011ECD5 = 1;
    D_8011ECE6 = 0;
    if ((arg0 == 0) || (arg0 != 1)) {
        if ((D_800B6708 == 0) || ((s32) D_800B6708 < 0x9B) || (D_8011ECE8 = (s16) D_800B6708, (((s32) D_800B6708 < 0xAE) == 0))) {
            goto block_10;
        }
    } else if ((D_800B6708 == 0) || ((s32) D_800B6708 < 0x9B) || (D_8011ECE8 = (s16) D_800B6708, (((s32) D_800B6708 < 0xAE) == 0))) {
block_10:
        D_8011ECE8 = 0;
    }
    D_8011ECF6 = sp2E;
    D_8011ECFA = sp2C;
}
