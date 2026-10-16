/* T-17100 hand edit of the redraft, 16 funcdiff lines: m2c read the D_8015EC68 store as record i + 1
 * (it is the last unk_26, kept in $a0 and stored after the loop). Left: the original hoists `i + 0x28`
 * into the load delay slot above the first branch. */
void func_8015A8B4(void) {
    s32 i;
    s32 v;
    s32 last;
    s32 id;

    for (i = 0; i < 8; i++) {
        id = i + 0x28;
        if ((((RpgObjectRecord44 *)D_8011ECD0)[i + 136].unk_26 < -0xB4) ||
            (((RpgObjectRecord44 *)D_8011ECD0)[i + 136].unk_2A >= 0x8D)) {
            func_8013E810(id, 7, 1, 0);
        }
        v = ((0x70 - ((RpgObjectRecord44 *)D_8011ECD0)[i + 136].unk_26) << 0xC) / 240;
        ((RpgObjectRecord44 *)D_8011ECD0)[i + 136].unk_1A = v;
        ((RpgObjectRecord44 *)D_8011ECD0)[i + 136].unk_1C = v;
        last = ((RpgObjectRecord44 *)D_8011ECD0)[i + 136].unk_26;
    }
    D_8015EC58.unk_10 = last;
}
