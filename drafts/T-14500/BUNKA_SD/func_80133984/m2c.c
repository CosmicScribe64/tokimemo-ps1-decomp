? func_80048514(?, s32, ?);                         /* extern */
extern void *D_8013C6E8;
extern s32 D_8013C6EC;
extern s32 D_8013C6F0;
extern s32 D_8013C6F4;

void func_80133984(void) {
    s32 var_s1;
    s32 var_s1_2;
    u8 *var_s0;

    var_s1 = 0;
    do {
        func_80043914(D_8013C6F4, (var_s1 + 0x10) & 0xFF, 1, 1, var_s1);
        var_s1 += 1;
    } while (var_s1 < 0x10);
    var_s0 = D_8011ECD0;
    var_s1_2 = 0;
    do {
        func_80048F64(var_s1_2 + 0x60);
        *(s8 *)((unsigned char *)var_s0 + (6529)) = 4;
        *(s32 *)((unsigned char *)var_s0 + (6584)) = 0x01000000;
        *(s8 *)((unsigned char *)var_s0 + (6531)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (6530)) = 0x40;
        *(s32 *)((unsigned char *)var_s0 + (6540)) = (s32) D_8013C6EC;
        *(s32 *)((unsigned char *)var_s0 + (6544)) = (s32) D_8013C6F0;
        var_s1_2 += 1;
        var_s0 += 0x44;
        *(s16 *)((unsigned char *)var_s0 + (6484)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (6466)) = 1;
        *(s8 *)((unsigned char *)var_s0 + (6467)) = 0;
        *(s16 *)((unsigned char *)var_s0 + (6498)) = -0xA0;
        *(s16 *)((unsigned char *)var_s0 + (6502)) = -0x78;
        *(s8 *)((unsigned char *)var_s0 + (6465)) = 0x80;
        *(s8 *)((unsigned char *)var_s0 + (6527)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (6464)) = 0;
        *(void **)((unsigned char *)var_s0 + (6512)) = (void *) D_8013C6E8;
    } while (var_s1_2 != 0x20);
    func_801341A0(0x1F, 0xBF, 1);
    func_801341A0(1, 0, 1);
    func_80048514(5, D_8013C6E8->unk0, 1);
    func_80048514(6, *(s32 *)((unsigned char *)D_8013C6E8 + (4)), 1);
    func_80048514(7, *(s32 *)((unsigned char *)D_8013C6E8 + (8)), 1);
    func_80048514(8, *(s32 *)((unsigned char *)D_8013C6E8 + (12)), 1);
    func_80048514(9, *(s32 *)((unsigned char *)D_8013C6E8 + (16)), 1);
    func_80048514(0xA, *(s32 *)((unsigned char *)D_8013C6E8 + (20)), 1);
    func_80048514(0xB, *(s32 *)((unsigned char *)D_8013C6E8 + (24)), 1);
    func_80048514(0xC, *(s32 *)((unsigned char *)D_8013C6E8 + (28)), 1);
    func_80048514(0xD, *(s32 *)((unsigned char *)D_8013C6E8 + (32)), 1);
    func_80048514(0xE, *(s32 *)((unsigned char *)D_8013C6E8 + (36)), 1);
    func_80048514(0xF, *(s32 *)((unsigned char *)D_8013C6E8 + (40)), 1);
    func_80048514(0x25, *(s32 *)((unsigned char *)D_8013C6E8 + (44)), 1);
    func_80048514(0x26, *(s32 *)((unsigned char *)D_8013C6E8 + (48)), 1);
    func_80048514(0x27, *(s32 *)((unsigned char *)D_8013C6E8 + (52)), 1);
    func_80048514(0x28, *(s32 *)((unsigned char *)D_8013C6E8 + (56)), 1);
    func_80048514(0x29, *(s32 *)((unsigned char *)D_8013C6E8 + (60)), 1);
    func_80048514(0x2A, *(s32 *)((unsigned char *)D_8013C6E8 + (64)), 1);
    func_80048514(0x2B, *(s32 *)((unsigned char *)D_8013C6E8 + (68)), 1);
    func_80048514(0x2C, *(s32 *)((unsigned char *)D_8013C6E8 + (72)), 1);
    func_80048514(0x2D, *(s32 *)((unsigned char *)D_8013C6E8 + (76)), 1);
    func_80048514(0x2E, *(s32 *)((unsigned char *)D_8013C6E8 + (80)), 1);
}
