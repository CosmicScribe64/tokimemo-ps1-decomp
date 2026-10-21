/* T-25002 draft, ENDING func_801387B0 (src/ovl/ENDING/80137340.c); 80/76 differing rows, original 748 B, built 16 B shorter.
 * Left: the original keeps (byte & 1) in v1 and recomputes xor a1,v1; sltiu for each of the four "0xC - (flag == 1)"
 * compares (no CSE), with the byte itself in a0; every form tried CSEs the boolean from the second compare on
 * (s32/u8/u32 flag, byte + bit locals, 1 == flag, !(x ^ 1), ternary, direct field reads). "* 1" on D_8011F4DE/D_8011F4D0
 * gives the original bne v1,t0 order. autotweak 300 s: view D_8013C360 u32->s32 at the end (exact 156 -> 123). */
void func_801387B0(void) {
    u8 flag;

    if (D_8013C35C == 1) {
        if (D_8013C360 == 0) {
            ((DisplayActor44 *)D_8011ECD0)[31].unk_16 = 7;
            func_801396A4(0, 0xFF);
            D_80122D0C = 0;
            D_8013CAA0 = 0;
        }
    } else if (D_8013C35C == 8) {
        if (D_8013C360 == 0xC8) {
            func_801396A4(1, 5);
            D_80122D0C = 1;
        } else if (D_8011F4DE * 1 == 1 && D_8011F4E0 == 2 && D_8011F4D0 * 1 == 1) {
            func_801396A4(2, 0);
            D_80122D0C = 0;
        }
    } else {
        flag = D_800E6280.unk_53C[6];
        if (D_8013C35C == 0xC - ((flag & 1) == 1)) {
            if (D_8013CAA0 == 0 && D_8013C364 == 0xA) {
                D_8013CAA0 = 1;
                D_8013C364 = 0;
            }
            if (D_8013C364 == 0x1E && D_8011F4DE == 2 && D_8013CAA0 == 1) {
                func_801396A4(3, 0);
                ((DisplayActor44 *)D_8011ECD0)[31].unk_16 = 8;
            }
        } else if (D_8013C35C == 0xE - ((flag & 1) == 1)) {
            if (D_8013C360 == 0) {
                func_801396A4(0, 0);
            }
        } else if (D_8013C35C == 0x13 - ((flag & 1) == 1)) {
            if (D_8013C360 == 0) {
                ((DisplayActor44 *)D_8011ECD0)[31].unk_16 = 7;
            }
        } else if (D_8013C35C == 0x16 - ((flag & 1) == 1)) {
            if (D_8013C360 == 0) {
                ((DisplayActor44 *)D_8011ECD0)[31].unk_16 = 8;
            }
        } else if (D_8013C35C == 0x18) {
            if (D_8013C360 == 1) {
                func_801396A4(4, 5);
                D_80122D0C = 1;
            }
        } else if (D_8013C35C == 0x1A) {
            if (D_8013C360 == 0) {
                ((DisplayActor44 *)D_8011ECD0)[31].unk_16 = 7;
            }
        } else if (D_8013C35C == 0x1C && D_8013C360 != 0) {
            if (D_8013C360 == 1) {
                func_801396A4(5, 5);
            } else if (D_8011F4DE == 5 && D_8011F4E0 == 4 && D_8011F4D0 == 0xD) {
                func_801396A4(6, 1);
            }
        }
    }
    D_8013C360 += 1;
}
