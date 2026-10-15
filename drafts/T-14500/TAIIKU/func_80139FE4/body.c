typedef struct {
    u32 low : 1;
    u32 f : 1;
    u32 rest : 30;
} TaiikuBit1;

void func_80139FE4(void) {
    s32 n;
    s32 i;
    s32 cnt;
    s16 pos;
    s32 sp1C[11];

    n = 0;
    for (i = 0; i < 0xB; i++) {
        if (((TaiikuBit1 *)&D_800E6280.unk_1BC[i].unk_0C)->f == 1) {
            sp1C[n] = i;
            n++;
        }
    }
    cnt = 0;
    if (((TaiikuBit1 *)&D_800E6280.unk_1BC[0].unk_0C)->f == 1) {
        cnt = 1;
        if (D_800E6280.unk_F80 & 4) {
            pos = D_80120676 - 4;
        } else {
            pos = D_80120676 - 1;
        }
        ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = pos;
        if (pos < -0xC0) {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = ((DisplayActor44 *)D_8011ECD0)[96 + sp1C[(n - 1) % n]].unk_26 + 0x78;
            if (((DisplayActor44 *)D_8011ECD0)[96].unk_26 < 0xA0) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = 0xA0;
            }
        }
    }
    for (i = 1; i != 0xB; i += 2) {
        if (((TaiikuBit1 *)&D_800E6280.unk_1BC[i].unk_0C)->f == 1) {
            if (D_800E6280.unk_F80 & 4) {
                ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 = ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 - 4;
            } else {
                ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 = ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 - 1;
            }
            if (((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 < -0xC0) {
                ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 = ((DisplayActor44 *)D_8011ECD0)[96 + sp1C[(cnt + n - 1) % n]].unk_26 + 0x78;
                if (((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 < 0xA0) {
                    ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 = 0xA0;
                }
            }
            cnt++;
        }
        if (((TaiikuBit1 *)&D_800E6280.unk_1BC[i + 1].unk_0C)->f == 1) {
            if (D_800E6280.unk_F80 & 4) {
                ((DisplayActor44 *)D_8011ECD0)[97 + i].unk_26 = ((DisplayActor44 *)D_8011ECD0)[97 + i].unk_26 - 4;
            } else {
                ((DisplayActor44 *)D_8011ECD0)[97 + i].unk_26 = ((DisplayActor44 *)D_8011ECD0)[97 + i].unk_26 - 1;
            }
            if (((DisplayActor44 *)D_8011ECD0)[97 + i].unk_26 < -0xC0) {
                ((DisplayActor44 *)D_8011ECD0)[97 + i].unk_26 = ((DisplayActor44 *)D_8011ECD0)[96 + sp1C[(cnt + n - 1) % n]].unk_26 + 0x78;
                if (((DisplayActor44 *)D_8011ECD0)[97 + i].unk_26 < 0xA0) {
                    ((DisplayActor44 *)D_8011ECD0)[97 + i].unk_26 = 0xA0;
                }
            }
            cnt++;
        }
    }
}
