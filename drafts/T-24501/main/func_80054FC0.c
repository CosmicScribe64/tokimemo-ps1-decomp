void func_80054FC0(s32 arg0) {
    u8 *image;
    u8 *p;
    s32 sum;
    u32 offset;

    D_800E7D10.slot_status[D_800E7D10.selected_slot] = 1;
    memcpy(((SaveImageView *)D_80123320)->slot_status, D_800E7D10.slot_status, 16);
    memcpy(((SaveImageView *)D_80123320)->flags, D_800E7D10.flags, 16);
    ((SaveImageView *)D_80123320)->unk_24[0] = D_800E7D10.unk_024[0];
    ((SaveImageView *)D_80123320)->unk_24[1] = D_800E7D10.unk_024[1];
    if (D_800E7D10.selected_slot != 0) {
        ((SaveImageView *)D_80123320)->selected_slot = D_800E7D10.selected_slot % 15U;
    }
    ((SaveImageView *)D_80123320)->config0 = ((SaveWorkView *)&D_800E7D10)->config0;
    ((SaveImageView *)D_80123320)->config1 = ((SaveWorkView *)&D_800E7D10)->config1;
    ((SaveImageView *)D_80123320)->unk_54[0] = D_800E7D10.unk_027[0];
    ((SaveImageView *)D_80123320)->unk_54[1] = D_800E7D10.unk_027[1];
    memcpy(((SaveImageView *)D_80123320)->records, D_800E7D10.unk_058, 0x800);
    memcpy(((SaveImageView *)D_80123320)->year, D_800E7D10.year, 16);
    memcpy(((SaveImageView *)D_80123320)->month, D_800E7D10.month, 16);
    memcpy(((SaveImageView *)D_80123320)->day, D_800E7D10.day, 16);
    memcpy(((SaveImageView *)D_80123320)->family_name, D_800E7D10.family_name, 0x80);
    memcpy(((SaveImageView *)D_80123320)->given_name, D_800E7D10.given_name, 0x80);
    memcpy(((SaveImageView *)D_80123320)->nickname, D_800E7D10.nickname, 0x100);
    memcpy(((SaveImageView *)D_80123320)->club, D_800E7D10.club, 16);
    memcpy(((SaveImageView *)D_80123320)->metrics, D_800E7D10.metrics, 0x240);
    memcpy(((SaveImageView *)D_80123320)->unk_CD8, D_800E7D10.unk_CC8, 0x100);
    if (D_800E6280.unk_1124 == 0) {
        if (D_800E7D10.selected_slot != 0) {
            memcpy(&D_800E7D10.unk_CC8[0x100], D_80125C10, 0x40);
            memcpy(&D_800E7D10.unk_CC8[0x1F0], &D_80125C50, 4);
            D_800E7D10.unk_CC8[0x1F4] = D_800B5C08;
            D_800E7D10.unk_CC8[0x1F5] = D_80125C54;
            D_800E7D10.unk_CC8[0x1F6] = D_80125C55;
            memcpy(((SaveImageView *)D_80123320)->unk_DD8, &D_800E7D10.unk_CC8[0x100], 0x100);
        }
    }
    image = arg0 * 0xF00 + D_80123120;
    p = image + 0x204;
    sum = 0;
    offset = 4;
    for (; offset < 0xF00; offset++) {
        sum += *p++;
    }
    ((SaveImageView *)(image + 0x200))->checksum = sum;
}
