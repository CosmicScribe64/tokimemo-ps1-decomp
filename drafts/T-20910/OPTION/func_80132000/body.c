void func_80132000(void) {
    if ((D_800E6280.unk_110A != 0x21) && (D_800E6280.unk_110A != 0x22) && (D_800E6280.unk_110A != 0x23)) {
        if (D_800E6280.unk_03A != 0) {
            if ((u8) ((SpriteSlot24 *)&D_801220D0)->unk_10 >= 0x7FU) {
                func_80049A40(((DisplayActor44 *)D_8011ECD0)[96].unk_26 + 0x18, ((DisplayActor44 *)D_8011ECD0)[96].unk_2A + 0x10, 0x7C, 0xBC, 0xA, 0x10907, 0x83);
                dtd_on_tpage(0, 0, 0xA, 0, 0);
            }
        }
    }
}
