extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern ? D_800E6A80;
extern u8 D_800E71DF;
extern s8 D_800E7D37;
extern s8 D_800E7D38;

void func_80141ABC(void) {
    GameState *var_s0;
    s8 var_s1;
    u8 temp_v0;
    void *temp_t3;

    func_800AE080(&D_800E7D68, 0x800);
    var_s0 = &D_800E6280;
    var_s1 = 0;
    do {
        temp_v0 = var_s0->unk_75E[0].unk_02;
        func_800AE0B0("%d %d %d %d %d \n", var_s0->unk_75E[0].unk_04, var_s0->unk_75E[0].unk_05, var_s0->unk_75E[0].unk_06, (s32) temp_v0, (s32) temp_v0);
        if ((D_800E62BE != 0) && (D_800E62BF != 0) && (D_800E62C0 != 0) && ((D_800E71DF & 0xF) == var_s0->unk_75E[0].unk_00)) {
            temp_t3 = &D_800E7D10 + (var_s1 * 8);
            *(? *)((unsigned char *)temp_t3 + (88)) = (unaligned s32) var_s0->unk_75E[0].unk_00;
            var_s1 += 1;
            *(? *)((unsigned char *)temp_t3 + (92)) = (unaligned s32) var_s0->unk_75E[0].unk_04;
        }
        var_s0 += 8;
    } while (var_s0 != &D_800E6A80);
    D_800E7D38 = var_s1;
    D_800E7D37 = 1;
    func_800AE0B0("ALBUM_MODE IN SAVE %d \n", 1);
}
