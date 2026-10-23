void func_80135F3C(void) {
    s32 i;
    s32 j;
    s32 k;
    s16 code;
    u8 chosen;
    TaiRunnerA *r;
    TaiWords11 choices;
    TaiikuS11 player_codes;
    TaiikuS11 rival_codes;
    TaiFlags58 flags;

    choices = D_80149728;
    player_codes = D_80149754;
    rival_codes = D_8014976C;
    flags = D_80149784;
    chosen = (func_800AE0D0() % 20) + 10;
    if (chosen == 19) {
        do {
            chosen = (func_800AE0D0() % 20) + 10;
            D_80149208.player.unk_02 = chosen;
        } while (chosen == 19);
    }
    ((DisplayActor44 *)D_8011ECD0)[46 + chosen].unk_16 = player_codes.v[D_801491EC];
    D_80149208.player.unk_02 = chosen;
    r = &D_80149208.cpu[0];
    do {
        r->unk_02 = func_800AE0D0() % 30;
        if (r->unk_02 == 19) {
            do {
                r->unk_02 = func_800AE0D0() % 30;
            } while (r->unk_02 == 19);
        }
        r++;
    } while (r < &D_80149208.cpu[3]);
    D_801491DC = func_8008647C();
    if (func_8008667C(D_801491DC) < 2 && D_801491DC != choices.value[D_801491EC]) {
        chosen = D_801491DC;
        D_801491D8 = func_800AE0D0() % 10;
        ((DisplayActor44 *)D_8011ECD0)[46 + D_801491D8].unk_16 = rival_codes.v[chosen];
        ((DisplayActor44 *)D_8011ECD0)[115].unk_16 = (s16)((DisplayActor44 *)D_8011ECD0)[46 + D_801491D8].unk_16 >> 3;
        code = ((DisplayActor44 *)D_8011ECD0)[46 + D_801491D8].unk_16;
        ((DisplayActor44 *)D_8011ECD0)[115].unk_43 = flags.value[code >> 3][code & 7];
    } else {
        chosen = 0xFF;
    }
    D_801491DC = chosen;
    if (chosen == choices.value[D_801491EC]) {
        D_801491DC = 0xFF;
    }
    for (i = 0; i < 3; i++) {
        code = ((DisplayActor44 *)D_8011ECD0)[46 + D_80149208.cpu[i].unk_02].unk_16;
        for (j = i; j >= 0; j--) {
            if (code == ((DisplayActor44 *)D_8011ECD0)[46 + (&D_80149208.player)[j].unk_02].unk_16 || code == ((DisplayActor44 *)D_8011ECD0)[46 + D_801491D8].unk_16) {
                do {
                    D_80149208.cpu[i].unk_02 = func_800AE0D0() % 30;
                    code = ((DisplayActor44 *)D_8011ECD0)[46 + D_80149208.cpu[i].unk_02].unk_16;
                } while (code == ((DisplayActor44 *)D_8011ECD0)[46 + (&D_80149208.player)[j].unk_02].unk_16 || code == ((DisplayActor44 *)D_8011ECD0)[46 + D_801491D8].unk_16);
            }
        }
    }
    for (i = 0; i < 4; i++) {
        ((DisplayActor44 *)D_8011ECD0)[107 + i].unk_16 = (s16)((DisplayActor44 *)D_8011ECD0)[46 + (&D_80149208.player)[i].unk_02].unk_16 >> 3;
    }
    code = ((DisplayActor44 *)D_8011ECD0)[46 + D_80149208.player.unk_02].unk_16;
    ((DisplayActor44 *)D_8011ECD0)[107].unk_43 = flags.value[code >> 3][code & 7];
    code = ((DisplayActor44 *)D_8011ECD0)[46 + D_80149208.cpu[0].unk_02].unk_16;
    ((DisplayActor44 *)D_8011ECD0)[108].unk_43 = flags.value[code >> 3][code & 7];
    code = ((DisplayActor44 *)D_8011ECD0)[46 + D_80149208.cpu[1].unk_02].unk_16;
    ((DisplayActor44 *)D_8011ECD0)[109].unk_43 = flags.value[code >> 3][code & 7];
    code = ((DisplayActor44 *)D_8011ECD0)[46 + D_80149208.cpu[2].unk_02].unk_16;
    ((DisplayActor44 *)D_8011ECD0)[110].unk_43 = flags.value[code >> 3][code & 7];
}
