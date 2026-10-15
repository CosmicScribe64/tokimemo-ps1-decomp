extern s16 D_80149986;
extern u16 D_801499A2;
extern ? D_801499A6;
extern u16 D_801499B2;

void func_8013C0E4(void) {
    s32 sp2C;
    s32 var_t7;
    u16 var_a1;

    func_8013C5C8(0x11800);
    func_8013C694();
    var_t7 = D_801499A2 & 0xF;
    if (!(D_801499A2 & 8)) {
        func_8013C4E4(0x8000);
        var_t7 = D_801499A2 & 0xF;
    }
    if (var_t7 == 0) {
        sp2C.unk0 = 0;
        *(s16 *)((unsigned char *)(&sp2C) + (2)) = (s16) ((s16) *D_80149984 >> 3);
        sp2C += D_80149974.unk_00 * 0x50;
        D_801499A6.unk0 = (s16) (((s16) D_80149986 >> 3) + 0x10);
        *(s16 *)((unsigned char *)(&D_801499A6) + (-2)) = (s16) *(s16 *)((unsigned char *)(&sp2C) + (2));
    }
    if ((D_80120676 < D_801206BA) && (func_800AE0C0(D_8012067A - D_801206BE) < 0xC) && ((D_801206BA - 0x20) < D_80120676)) {
        *(s32 *)((unsigned char *)D_8011ECD0 + (6632)) = (s32) (*(s32 *)((unsigned char *)D_8011ECD0 + (6632)) + 0x11800);
    } else if ((*(s16 *)((unsigned char *)D_8011ECD0 + (6634)) < D_80120676) && (func_800AE0C0(D_8012067A - D_801206BE) < 0xC) && (D_80120676 < (*(s16 *)((unsigned char *)D_8011ECD0 + (6634)) + 0x20))) {
        *(s32 *)((unsigned char *)D_8011ECD0 + (6632)) = (s32) (*(s32 *)((unsigned char *)D_8011ECD0 + (6632)) + 0xFFFEE800);
    }
    var_a1 = D_801499A2;
    if (var_a1 >= 0x81U) {
        var_a1 = 0x20;
    }
    D_801499A2 = var_a1;
    if ((var_a1 != 0) && (D_801499A2 = var_a1, ((var_a1 & 0x1F) == 0)) && (D_801499A2 = var_a1, ((var_a1 < 0x21U) == 0))) {
        if (D_801499B2 == 0) {
            D_801499B2 = 1;
            D_801499A2 = var_a1 + 1;
            func_80044750(0x800);
            return;
        }
        if (D_801499B2 == 1) {
            D_801499B2 = 0;
            D_801499A2 = var_a1 + 1;
            func_80044750(0x801);
            return;
        }
        D_801499A2 = var_a1;
        D_801499B2 = 0;
    }
}
