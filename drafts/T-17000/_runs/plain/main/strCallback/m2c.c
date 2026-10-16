extern s32 D_80125C5C;

void strCallback(void) {
    s16 sp32;
    s16 sp30;
    s32 temp_t6;
    s32 var_v0;
    void *temp_v0;

    temp_t6 = D_800B5900 & 1;
    var_v0 = temp_t6;
    if ((temp_t6 != 0) && (D_80125C5C != 0)) {
        func_80088210();
        var_v0 = D_800B5900 & 1;
        D_80125C5C = 0;
    }
    sp30.unk0 = (s32) (unaligned s32) *(s16 *)((unsigned char *)D_80125C58 + (40));
    *(s32 *)((unsigned char *)(&sp30) + (4)) = (s32) (unaligned s32) *(s16 *)((unsigned char *)D_80125C58 + (44));
    if (var_v0 != 0) {
        sp30 += (s32) ((D_800B58F8 * 3) + 2) / 3;
        sp32 += (s32) ((D_800B58FC * 3) + 2) / 3;
    } else {
        sp30 += D_800B58F8;
        sp32 += D_800B58FC;
    }
    func_8009C884((RECT *) &sp30, *(void **)((unsigned char *)D_80125C58 + (16)));
    func_8009C674(0);
    *(s16 *)((unsigned char *)D_80125C58 + (40)) = (s16) (*(s16 *)((unsigned char *)D_80125C58 + (40)) + *(s16 *)((unsigned char *)D_80125C58 + (44)));
    temp_v0 = D_80125C58 + (*(s32 *)((unsigned char *)D_80125C58 + (36)) * 8);
    if (*(s16 *)((unsigned char *)D_80125C58 + (40)) < (*(s16 *)((unsigned char *)temp_v0 + (24)) + *(s16 *)((unsigned char *)temp_v0 + (20)))) {
        func_80086944((s32) *(void **)((unsigned char *)D_80125C58 + (16)), (s32) (*(s16 *)((unsigned char *)D_80125C58 + (44)) * *(s16 *)((unsigned char *)D_80125C58 + (46))) / 2);
        return;
    }
    *(s32 *)((unsigned char *)D_80125C58 + (48)) = 1;
    if (*(s32 *)((unsigned char *)D_80125C58 + (36)) != 0) {
        *(s32 *)((unsigned char *)D_80125C58 + (36)) = 0;
    } else {
        *(s32 *)((unsigned char *)D_80125C58 + (36)) = 1;
    }
    *(s16 *)((unsigned char *)D_80125C58 + (40)) = (s16) *(s16 *)((unsigned char *)(D_80125C58 + (*(s32 *)((unsigned char *)D_80125C58 + (36)) * 8)) + (20));
    *(s16 *)((unsigned char *)D_80125C58 + (42)) = (s16) *(s16 *)((unsigned char *)(D_80125C58 + (*(s32 *)((unsigned char *)D_80125C58 + (36)) * 8)) + (22));
}
