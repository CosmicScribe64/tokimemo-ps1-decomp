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
extern s8 D_800E8BCC;
extern u8 D_800E8BCD;
extern u8 D_800E8BCE;
extern ? D_800E8BD8;
extern ? D_80123334;
extern u8 D_80123344;
extern u8 D_80123345;
extern s8 D_80123346;
extern ? D_80123348;
extern ? D_8012335C;
extern u8 D_80123374;
extern u8 D_80123375;
extern ? D_80123376;
extern ? D_80123B76;
extern ? D_80123B86;
extern ? D_80123B96;
extern ? D_80123BA6;
extern ? D_80123C26;
extern ? D_80123CA6;
extern ? D_80123DA6;
extern ? D_80123DB8;
extern ? D_80123FF8;
extern ? D_801240F8;

void func_80054FC0(s32 arg0) {
    s32 temp_v1;
    s32 var_a0;
    s32 var_v1;
    u8 temp_t3;
    void *temp_a2;
    void *var_v0;

    D_800E7D14[D_800E7D36] = 1;
    memcpy((s32) D_80123324, (s32) D_800E7D14, 0x10);
    memcpy((s32) &D_80123334, (s32) &D_800E7D24, 0x10);
    D_80123344 = D_800E7D34;
    D_80123345 = D_800E7D35;
    if (D_800E7D36 != 0) {
        D_80123346 = (s8) ((u8) D_800E7D36 % 15U);
    }
    D_80123348.unk0 = (s32) D_800E7D3C.unk0;
    *(s32 *)((unsigned char *)(&D_80123348) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_800E7D3C) + (8));
    *(s32 *)((unsigned char *)(&D_80123348) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_800E7D3C) + (4));
    *(s32 *)((unsigned char *)(&D_80123348) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_800E7D3C) + (12));
    *(s32 *)((unsigned char *)(&D_80123348) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_800E7D3C) + (16));
    D_8012335C.unk0 = (s32) D_800E7D50.unk0;
    *(s32 *)((unsigned char *)(&D_8012335C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_800E7D50) + (4));
    *(s32 *)((unsigned char *)(&D_8012335C) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_800E7D50) + (8));
    *(s32 *)((unsigned char *)(&D_8012335C) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_800E7D50) + (12));
    *(s32 *)((unsigned char *)(&D_8012335C) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_800E7D50) + (16));
    *(s32 *)((unsigned char *)(&D_8012335C) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_800E7D50) + (20));
    D_80123374 = D_800E7D37;
    D_80123375 = D_800E7D38;
    memcpy((s32) &D_80123376, (s32) &D_800E7D68, 0x800);
    memcpy((s32) &D_80123B76, (s32) &D_800E8568, 0x10);
    memcpy((s32) &D_80123B86, (s32) &D_800E8578, 0x10);
    memcpy((s32) &D_80123B96, (s32) &D_800E8588, 0x10);
    memcpy((s32) &D_80123BA6, (s32) &D_800E8598, 0x80);
    memcpy((s32) &D_80123C26, (s32) &D_800E8618, 0x80);
    memcpy((s32) &D_80123CA6, (s32) &D_800E8698, 0x100);
    memcpy((s32) &D_80123DA6, (s32) &D_800E8BD8, 0x10);
    memcpy((s32) &D_80123DB8, (s32) &D_800E8798, 0x240);
    memcpy((s32) &D_80123FF8, (s32) &D_800E89D8, 0x100);
    if (D_800E6280.unk_1124 == 0) {
        if (D_800E7D36 != 0) {
            memcpy((s32) &D_800E8AD8, (s32) D_80125C10, 0x40);
            memcpy((s32) &D_800E8BC8, (s32) &D_80125C50, 4);
            D_800E8BCC = (s8) D_800B5C08;
            D_800E8BCD = D_80125C54;
            D_800E8BCE = D_80125C55;
            memcpy((s32) &D_801240F8, (s32) &D_800E8AD8, 0x100);
        }
    }
    temp_a2 = (arg0 * 0xF00) + D_80123120;
    var_v0 = temp_a2 + 0x204;
    var_v1 = 0;
    var_a0 = 4;
    do {
        temp_t3 = *(u8 *)((unsigned char *)var_v0 + (3));
        var_a0 += 4;
        temp_v1 = var_v1 + var_v0->unk0 + *(u8 *)((unsigned char *)var_v0 + (1)) + *(u8 *)((unsigned char *)var_v0 + (2));
        var_v0 += 4;
        var_v1 = temp_v1 + temp_t3;
    } while (var_a0 != 0xF00);
    *(s32 *)((unsigned char *)temp_a2 + (512)) = var_v1;
}
