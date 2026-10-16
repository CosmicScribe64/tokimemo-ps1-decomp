extern ? D_800E7D24;
extern u8 D_800E7D35;
extern u8 D_800E7D37;
extern u8 D_800E7D38;
extern ? D_800E8568;
extern ? D_800E8578;
extern ? D_800E8588;
extern ? D_800E8598;
extern ? D_800E8618;
extern ? D_800E8698;
extern ? D_800E8798;
extern ? D_800E89D8;
extern ? D_800E8AD8;
extern ? D_800E8BC8;
extern u8 D_800E8BCC;
extern u8 D_800E8BCD;
extern u8 D_800E8BCE;
extern ? D_800E8BD8;

void func_800552DC(s32 arg0) {
    void *temp_s0;

    if (func_80055A38(0) == -1) {
        return;
    }
    temp_s0 = (D_800E6280 * 0xF00) + 0x200 + D_80123120;
    memcpy((s32) &D_800E7D24, (s32) (temp_s0 + 0x14), 0x10);
    D_800E7D34 |= *(u8 *)((unsigned char *)temp_s0 + (36));
    D_800E7D35 = *(u8 *)((unsigned char *)temp_s0 + (37));
    D_800E7D36 = *(u8 *)((unsigned char *)temp_s0 + (38));
    if (!(D_800E7D4F & 1)) {
        D_800E7D3C.unk0 = (s32) *(s32 *)((unsigned char *)temp_s0 + (40));
        *(s32 *)((unsigned char *)(&D_800E7D3C) + (4)) = (s32) *(s32 *)((unsigned char *)temp_s0 + (44));
        *(s32 *)((unsigned char *)(&D_800E7D3C) + (8)) = (s32) *(s32 *)((unsigned char *)temp_s0 + (48));
        *(s32 *)((unsigned char *)(&D_800E7D3C) + (12)) = (s32) *(s32 *)((unsigned char *)temp_s0 + (52));
        *(s32 *)((unsigned char *)(&D_800E7D3C) + (16)) = (s32) *(s32 *)((unsigned char *)temp_s0 + (56));
    }
    if (!(D_800E7D50.unk0 & 1)) {
        D_800E7D50.unk0 = *(s32 *)((unsigned char *)temp_s0 + (60));
        *(s32 *)((unsigned char *)(&D_800E7D50) + (4)) = (s32) *(s32 *)((unsigned char *)temp_s0 + (64));
        *(s32 *)((unsigned char *)(&D_800E7D50) + (8)) = (s32) *(s32 *)((unsigned char *)temp_s0 + (68));
        *(s32 *)((unsigned char *)(&D_800E7D50) + (12)) = (s32) *(s32 *)((unsigned char *)temp_s0 + (72));
        *(s32 *)((unsigned char *)(&D_800E7D50) + (16)) = (s32) *(s32 *)((unsigned char *)temp_s0 + (76));
        *(s32 *)((unsigned char *)(&D_800E7D50) + (20)) = (s32) *(s32 *)((unsigned char *)temp_s0 + (80));
    }
    D_800E7D37 = *(u8 *)((unsigned char *)temp_s0 + (84));
    D_800E7D38 = *(u8 *)((unsigned char *)temp_s0 + (85));
    memcpy((s32) &D_800E7D68, (s32) (temp_s0 + 0x56), 0x800);
    memcpy((s32) &D_800E8568, (s32) (temp_s0 + 0x856), 0x10);
    memcpy((s32) &D_800E8578, (s32) (temp_s0 + 0x866), 0x10);
    memcpy((s32) &D_800E8588, (s32) (temp_s0 + 0x876), 0x10);
    memcpy((s32) &D_800E8598, (s32) (temp_s0 + 0x886), 0x80);
    memcpy((s32) &D_800E8618, (s32) (temp_s0 + 0x906), 0x80);
    memcpy((s32) &D_800E8698, (s32) (temp_s0 + 0x986), 0x100);
    memcpy((s32) &D_800E8BD8, (s32) (temp_s0 + 0xA86), 0x10);
    memcpy((s32) &D_800E8798, (s32) (temp_s0 + 0xA98), 0x240);
    memcpy((s32) &D_800E89D8, (s32) (temp_s0 + 0xCD8), 0x100);
    memcpy((s32) &D_800E8AD8, (s32) (temp_s0 + 0xDD8), 0x100);
    memcpy((s32) D_80125C10, (s32) &D_800E8AD8, 0x40);
    memcpy((s32) &D_80125C50, (s32) &D_800E8BC8, 4);
    D_800B5C08 = (s16) D_800E8BCC;
    D_80125C54 = D_800E8BCD;
    D_80125C55 = D_800E8BCE;
}
