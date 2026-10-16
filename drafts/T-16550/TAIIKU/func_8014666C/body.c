s32 func_8014666C(void) {
    TaiikuController *controller;
    TaiikuController *work;
    TaiikuController *end;

    controller = &D_8014A3E4;
    controller->unk_0C++;
    controller->unk_08++;
    controller->unk_04++;
    controller->unk_02++;
    end = &D_8014A400[3];
    work = D_8014A400;
    do {
        work[0].unk_0C++;
        work[0].unk_04++;
        work++;
    } while (work < end);
    D_8014A454[0].unk_18++;
    D_8014A454[0].unk_14++;
    D_8014A454[0].unk_16++;
    D_8014A454[0].unk_0D++;
    D_8014A454[0].unk_0F++;
    D_8014A454[1].unk_18++;
    D_8014A454[1].unk_14++;
    D_8014A454[1].unk_16++;
    D_8014A454[1].unk_0D++;
    D_8014A454[1].unk_0F++;
    D_8014A454[2].unk_18++;
    D_8014A454[2].unk_14++;
    D_8014A454[2].unk_16++;
    D_8014A454[2].unk_0D++;
    D_8014A454[2].unk_0F++;
    D_8014A454[3].unk_18++;
    D_8014A454[3].unk_14++;
    D_8014A454[3].unk_16++;
    D_8014A454[3].unk_0D++;
    D_8014A454[3].unk_0F++;
}
