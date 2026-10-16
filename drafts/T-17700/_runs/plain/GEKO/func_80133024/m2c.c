extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;

void func_80133024(void) {
    s16 sp3A;
    s16 temp_v1;
    s16 var_s0;

    sp38[0].unk0 = (s32) D_80144D28.unk0;
    *(s32 *)((unsigned char *)(&sp38[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80144D28) + (4));
    *(s32 *)((unsigned char *)(&sp38[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80144D28) + (8));
    var_s0 = 1;
    *(s32 *)((unsigned char *)(&sp38[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80144D28) + (12));
    *(s32 *)((unsigned char *)(&sp38[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80144D28) + (16));
    *(s32 *)((unsigned char *)(&sp38[0]) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80144D28) + (20));
    if (D_800E62BE == 0x60) {
        sp3A = 0x1D;
    }
    D_80144C50 = 0;
    do {
        temp_v1 = *(s16 *)((unsigned char *)(&(&sp38[0])[D_800E62BF]) + (-2));
        if ((u32) temp_v1 >= (u32) (D_800E62C0 + var_s0)) {
            func_80132D04(0, (s32) (s16) ((D_800E62C0 + var_s0) - 1));
        } else {
            func_80132D04(1, (s32) (s16) (((D_800E62C0 + var_s0) - temp_v1) - 1));
        }
        var_s0 += 1;
    } while (var_s0 < 0x14);
    func_80132B24();
}
