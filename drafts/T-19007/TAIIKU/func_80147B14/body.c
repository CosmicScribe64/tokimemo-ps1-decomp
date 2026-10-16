void func_80147B14(s32 index) {
    TaiikuScene44 *rec;
    TaiikuPosition px;
    TaiikuPosition py;
    TaiikuHalf10 t60;
    TaiikuHalf10 t4C;
    TaiikuHalf8 t3C;
    TaiikuHalf8 t2C;
    s16 id;
    s32 off;
    s32 i;
    s32 k;
    TaiikuSlot1C *slot;

    t60 = D_8014A620;
    t4C = D_8014A634;
    t3C = D_8014A648;
    t2C = D_8014A658;
    rec = (TaiikuScene44 *)((index * 0x44) + D_8011ECD0);
    slot = (TaiikuSlot1C *)((index * 0x1C) + (u8 *)D_8014A454);
    py.w = rec[96].position2.w;
    off = index * 0x13;
    px.w = rec[96].position.w + ((TaiikuPosition *)slot)[0].w;
    id = rec[96].unk_18;
    py.w += ((TaiikuPosition *)slot)[1].w;
    i = 0;
    do {
        if (id == D_8014A4C4[i] + off) {
            py.h[1] += t60.h[i];
            px.h[1] += t4C.h[i];
        }
        i++;
    } while (&D_8014A4C4[i] < D_8014A4D4);
    i = 2;
    do {
        if (id == D_8014A4D4[i] + off) {
            px.h[1] += 0x16;
            py.h[1] -= 4;
            py.h[1] += t3C.h[i];
        }
        i++;
    } while (&D_8014A4D4[i] < D_8014A4E4);
    for (k = 2; k < 8; k++) {
        if (id == D_8014A4E4[k] + off) {
            px.h[1] -= 0xC;
            py.h[1] -= 4;
            py.h[1] += t2C.h[k];
        }
    }
    rec[100].position.h[1] = px.h[1];
    rec[100].position2.h[1] = py.h[1];
    i = 0;
    do {
        if (D_8014A400[i].position.h[1] - D_8014A3DE >= 0x1E1 || D_8014A400[i].position.h[1] - D_8014A3DE < -0xA0) {
            DISP(101 + i).unk_03 = 0;
        } else {
            DISP(101 + i).unk_03 = 0xA4;
        }
        i++;
    } while (&D_8014A400[i] != &D_8014A400[3]);
}
