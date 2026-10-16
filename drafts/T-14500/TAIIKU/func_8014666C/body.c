s32 func_8014666C(void) {
    TaiikuTickController *controller;
    TaiikuController *work;
    TaiikuController *end;

    controller = (TaiikuTickController *)&D_8014A3E4;
    controller->unk_0C++;
    controller->unk_08++;
    controller->unk_04++;
    controller->unk_02++;
    end = (TaiikuController *)D_8014A454;
    work = (TaiikuController *)D_8014A400;
    do {
        work[0].unk_0C++;
        work[0].unk_04++;
        work++;
    } while (work < end);
    ((TaiikuTick1C *)D_8014A454)[0].unk_18++;
    ((TaiikuTick1C *)D_8014A454)[0].unk_14++;
    ((TaiikuTick1C *)D_8014A454)[0].unk_16++;
    ((TaiikuTick1C *)D_8014A454)[0].unk_0D++;
    ((TaiikuTick1C *)D_8014A454)[0].unk_0F++;
    ((TaiikuTick1C *)D_8014A454)[1].unk_18++;
    ((TaiikuTick1C *)D_8014A454)[1].unk_14++;
    ((TaiikuTick1C *)D_8014A454)[1].unk_16++;
    ((TaiikuTick1C *)D_8014A454)[1].unk_0D++;
    ((TaiikuTick1C *)D_8014A454)[1].unk_0F++;
    ((TaiikuTick1C *)D_8014A454)[2].unk_18++;
    ((TaiikuTick1C *)D_8014A454)[2].unk_14++;
    ((TaiikuTick1C *)D_8014A454)[2].unk_16++;
    ((TaiikuTick1C *)D_8014A454)[2].unk_0D++;
    ((TaiikuTick1C *)D_8014A454)[2].unk_0F++;
    ((TaiikuTick1C *)D_8014A454)[3].unk_18++;
    ((TaiikuTick1C *)D_8014A454)[3].unk_14++;
    ((TaiikuTick1C *)D_8014A454)[3].unk_16++;
    ((TaiikuTick1C *)D_8014A454)[3].unk_0D++;
    ((TaiikuTick1C *)D_8014A454)[3].unk_0F++;
}
