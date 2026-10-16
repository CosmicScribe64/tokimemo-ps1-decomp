s32 func_80145384(void) {
    GzCallbacksBC sp28;
    s32 var_v0;

    sp28 = D_8014901C;
    var_v0 = sp28.callbacks[D_800F647A](0x3C);
    if ((u8) D_800F647A < 0x10U) {
        ((DisplayActor44 *)D_80129F40)[96].unk_07 = (u8) D_801317EB;
        if ((((DisplayActor44 *)D_80129F40)[96].unk_18 == 0) && (((DisplayActor44 *)D_80129F40)[96].unk_08 == 0)) {
            var_v0 = (s32) (s16) (((DisplayActor44 *)D_80129F40)[96].unk_16 + 1);
            ((DisplayActor44 *)D_80129F40)[96].unk_16 = (s16) var_v0;
            if (var_v0 >= 3) {
                ((DisplayActor44 *)D_80129F40)[96].unk_16 = 0;
                var_v0 = 0;
            }
        }
    }
    if (D_8012E694 != 0) {
        ((DisplayActor44 *)D_80129F40)[98].unk_03 = (u8) (((DisplayActor44 *)D_80129F40)[98].unk_03 | 0x80);
        ((DisplayActor44 *)D_80129F40)[98].unk_07 = (u8) ((DisplayActor44 *)D_80129F40)[96].unk_07;
        if (((u32) D_800F6468 % 300U) == 0) {
            var_v0 = func_80145520();
            ((DisplayActor44 *)D_80129F40)[98].unk_02 = 3;
        }
    } else {
        ((DisplayActor44 *)D_80129F40)[98].unk_03 = (u8) (((DisplayActor44 *)D_80129F40)->unk_00 & 0xFF7F);
    }
    return var_v0;
}
