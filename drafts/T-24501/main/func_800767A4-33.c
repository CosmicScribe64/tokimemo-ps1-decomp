/* T-24501 diagnostic only: 33 IDs plus one unused scalar; expected cost30.
 * Buffers/list/frame exact, picks18C/190/194 instead of188/18C/190. */
void func_800767A4(void) {
    s32 third;
    s32 second;
    s32 first;
    s32 choices[33];
    s32 gap; /* FAKE: diagnostic unused scalar; this alternative is not a final source shape. */
    s32 temp_hi; /* FAKE: unused original draft scalar retained for the diagnosed stack layout. */
    s32 var_v1_2;
    u8 text1[64];
    u8 text2[64];
    u8 text3[64];
    s32 i;
    for (i = 0; i < 64; i++) {
        text1[i] = text2[i] = text3[i] = 0;
    }
    if (func_800764AC(choices) >= 3) {
        var_v1_2 = rand() % 3;
        if (var_v1_2 < 0) {
            var_v1_2 = 0;
        }
        first = choices[var_v1_2 % 3];
        second = choices[(s32) (var_v1_2 + 1) % 3];
        third = choices[(s32) (var_v1_2 + 2) % 3];
        D_800E6280.unk_724 = 2;
        x_taku_menu_set(1, 3);
        strcpy(text1, "『");
        strcat(text1, D_800B342C[first]);
        strcat(text1, &D_800B69BC);
        strcpy(text2, "『");
        strcat(text2, D_800B342C[second]);
        strcat(text2, &D_800B69BC);
        strcpy(text3, "『");
        strcat(text3, D_800B342C[third]);
        strcat(text3, &D_800B69BC);
        x_taku_string_set((s32) text1, (s32) text2, (s32) text3, 3);
    } else {
        var_v1_2 = rand() & 1;
        if (var_v1_2 < 0) {
            var_v1_2 = 0;
        }
        first = choices[var_v1_2 % 2];
        second = choices[(s32) (var_v1_2 + 1) % 2];
        third = 0xFF;
        D_800E6280.unk_724 = 1;
        x_taku_menu_set(1, 2);
        strcpy(text1, "『");
        strcat(text1, D_800B342C[first]);
        strcat(text1, &D_800B69BC);
        strcpy(text2, "『");
        strcat(text2, D_800B342C[second]);
        strcat(text2, &D_800B69BC);
        x_taku_string_set((s32) text1, (s32) text2, 0, 2);
    }
    D_800E6280.unk_725 = (u8) first;
    D_800E6280.unk_726 = (u8) second;
    D_800E6280.unk_727 = (u8) third;
}