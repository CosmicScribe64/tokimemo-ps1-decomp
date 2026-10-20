/* T-24301 draft, 57 rows: GameState pointer g (kept in $a1 for the F6D loop tests), u8 views for the s8
 * F6C/F6D reads, the 15-pair copy as an index loop (IDO unrolls it 3 + 3x4). Left: the copy loops' dest/src
 * pointers swap $v0/$v1, case 2 keeps the base in $a0 where the original has the count, the i = 0 placement. */
void func_8006509C(void) {
    GameState *g = &D_800E6280;
    u32 i;
    s32 n;
    s16 *p;

    if (*(u8 *)&D_800E6280.unk_F6C == 2) {
        D_800E6280.unk_F6D = 0xF;
    }
    i = 0;
    if (*(u8 *)&D_800E6280.unk_F6D != 0) {
        do {
            D_80125C90[i] = 1;
            i++;
        } while (i < *(u8 *)&g->unk_F6D);
        i = 0;
    }
    if (*(u8 *)&D_800E6280.unk_F6D != 0) {
        i = 0;
        do {
            func_80048F64(i + 0x50);
            ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_01 = 3;
            ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_02 = 1;
            ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_38 = 0x40000000;
            ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_03 = 0x84;
            ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_0C = sprite_frame_table;
            ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_10 = D_800C9A14;
            ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_34 = D_800C9730;
            ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_04 = 8;
            ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_05 = 1;
            ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_16 = i;
            ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_06 = 1;
            ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_14 = D_800C9A54;
            func_80048F64(i + 0x40);
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_01 = 4;
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_02 = 0x40;
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_38 = 0x40000000;
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_03 = 0x84;
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_0C = sprite_frame_table;
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_10 = D_800C9A14;
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_34 = D_800C9730;
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_14 = D_800C9A54;
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_04 = 8;
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_05 = 1;
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_18 = 2;
            i++;
        } while (i < *(u8 *)&g->unk_F6D);
    }
    ((DisplayActor44 *)D_8011ECD0)[94].unk_01 = 3;
    ((DisplayActor44 *)D_8011ECD0)[78].unk_01 = 4;
    ((DisplayActor44 *)D_8011ECD0)[94].unk_16 = 0xF;
    ((DisplayActor44 *)D_8011ECD0)[94].unk_26 = 0;
    ((DisplayActor44 *)D_8011ECD0)[94].unk_2A = 0;
    ((DisplayActor44 *)D_8011ECD0)[78].unk_16 = 0xF;
    ((DisplayActor44 *)D_8011ECD0)[78].unk_26 = 0;
    ((DisplayActor44 *)D_8011ECD0)[78].unk_2A = 0;
    switch ((u8)D_800E6280.unk_F6C) {
    case 0:
        i = 0;
        if (*(u8 *)&D_800E6280.unk_F6D != 0) {
            do {
                ((s16 *)D_80125C10)[i * 2] = D_800B5AE4[i][0];
                ((s16 *)D_80125C10)[i * 2 + 1] = D_800B5AE4[i][1];
                i++;
            } while (i < *(u8 *)&g->unk_F6D);
        }
        break;
    case 1:
        i = 0;
        if (*(u8 *)&D_800E6280.unk_F6D != 0) {
            do {
                ((s16 *)D_80125C10)[i * 2] = D_800B5AA4[i][0];
                ((s16 *)D_80125C10)[i * 2 + 1] = D_800B5AA4[i][1];
                i++;
            } while (i < *(u8 *)&g->unk_F6D);
        }
        break;
    case 2:
        n = 0;
        for (p = (s16 *)D_80125C10; p != (s16 *)(D_80125C10 + 0x3C); p += 2) {
            if (p[0] == 0 && p[1] == 0) {
                n++;
            }
            if (n >= 2) {
                for (i = 0; i < 15; i++) {
                    ((s16 *)D_80125C10)[i * 2] = D_800B5AE4[i][0];
                    ((s16 *)D_80125C10)[i * 2 + 1] = D_800B5AE4[i][1];
                }
                break;
            }
        }
        break;
    }
    func_800654D0(*(u8 *)&D_800E6280.unk_F6D);
    LoadSquare(0x120, 0x1F1, 0x10, 1, &D_800C6200);
    LoadSquare(0x120, 0x1F2, 0x10, 1, &D_800C6220);
    LoadSquare(0x120, 0x1F3, 0x10, 1, &D_800C6220[0x20]);
}
