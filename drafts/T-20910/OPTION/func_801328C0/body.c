void func_801328C0(void) {
    s32 i;
    s32 j;
    u8 t;

    ((SpriteSlot24 *)&D_801220D0)->unk_00 = 0x01000000;
    ((SpriteSlot24 *)&D_801220D0)->unk_04 = -0xA0;
    ((SpriteSlot24 *)&D_801220D0)->unk_06 = -0x78;
    ((SpriteSlot24 *)&D_801220D0)->unk_08 = 0x200;
    ((SpriteSlot24 *)&D_801220D0)->unk_0A = 0x140;
    ((SpriteSlot24 *)&D_801220D0)->unk_0C = 0;
    ((SpriteSlot24 *)&D_801220D0)->unk_0E = 0;
    t = D_800E6280.unk_03A;
    ((SpriteSlot24 *)&D_801220D0)->unk_10 = t;
    ((SpriteSlot24 *)&D_801220D0)->unk_11 = t;
    ((SpriteSlot24 *)&D_801220D0)->unk_12 = t;
    ((SpriteSlot24 *)&D_801220D0)->unk_14 = (u32)&D_80122120[0];
    ((SpriteSlot24 *)&D_801220D0)->unk_18 = 0;
    ((SpriteSlot24 *)&D_801220D0)->unk_1A = 0;
    ((SpriteSlot24 *)&D_801220D0)->unk_1C = 0x1000;
    ((SpriteSlot24 *)&D_801220D0)->unk_1E = 0x1000;
    ((SpriteSlot24 *)&D_801220D0)->unk_20 = 0;
    D_80122120[0].cellw = 0x80;
    D_80122120[0].cellh = 0x40;
    D_80122120[0].ncellw = 4;
    D_80122120[0].ncellh = 5;
    D_80122120[0].base = D_80122140[0];
    D_80122120[0].index = D_80122540[0].v;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            D_80122540[0].v[j + i * 4] = 0;
            D_80122140[0][j + i * 4].cba = func_8009ED74(0, 0x1FA);
            D_80122140[0][j + i * 4].u = 0;
            D_80122140[0][j + i * 4].v = 0;
            D_80122140[0][j + i * 4].tpage = 5;
            D_80122140[0][j + i * 4].flag = 0;
        }
    }
    D_800E6280.unk_10EC[0] = 0xD;
}
