? func_8004F984(?, s16, s16);                       /* extern */

void func_801376A8(void) {
    OptActor44 *temp_a0;
    OptActor44 *var_v0;
    s32 temp_t3;
    s32 temp_t9;
    s32 var_v1;

    *(s8 *)((unsigned char *)D_8011ECD0 + (10815)) = 0x80;
    *(s16 *)((unsigned char *)D_8011ECD0 + (10838)) = 0x1000;
    *(s16 *)((unsigned char *)D_8011ECD0 + (10840)) = 0x1000;
    func_8004F984(1, D_8011ECD0->unk_26, D_8011ECD0->unk_2A);
    func_8004FC10(1);
    func_801320C0();
    temp_t9 = D_800E7D4F & 1;
    if (temp_t9 == 1) {
        *(s8 *)((unsigned char *)D_8011ECD0 + (7551)) = 0x80;
    }
    temp_t3 = D_800E7D50 & 1;
    if (temp_t3 == 1) {
        *(s8 *)((unsigned char *)D_8011ECD0 + (7619)) = 0x80;
    }
    var_v0 = D_8011ECD0 + 0x1980;
    temp_a0 = &var_v0[D_800E6280.unk0];
    var_v1 = 0;
    do {
        if (var_v0 == temp_a0) {
            *(s16 *)((unsigned char *)var_v0 + (1044)) = (s16) (var_v1 + 0x3B);
        } else {
            *(s16 *)((unsigned char *)var_v0 + (1044)) = (s16) (var_v1 + 0x3C);
        }
        var_v0 += 0x44;
        var_v1 += 2;
    } while (var_v0 != (D_8011ECD0 + 0x1A08));
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk0) {                  /* irregular */
        case 0:
            if (temp_t9 == 1) {
                D_800E7D4F &= 0xFFFD;
                func_80044750(0x501);
                func_80048EB8(0);
                func_8004E58C();
                main_state_set(0xD0);
            }
            break;
        case 1:
            if (temp_t3 == 1) {
                D_800E7D50 = (u8) D_800E7D50 & 0xFFFD;
                func_80044750(0x501);
                func_80048EB8(0);
                func_8004E58C();
                main_state_set(0x90);
            }
            break;
        }
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
    }
}
