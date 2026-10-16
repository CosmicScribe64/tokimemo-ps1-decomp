? func_80048514(?, s32, ?);                         /* extern */
extern s16 D_801207BC;
extern void *D_8013C730;
extern s32 D_8013C734;
extern s32 D_8013C738;
extern s32 D_8013C73C;

void func_80135CD4(void) {
    s16 temp_t0;
    s32 var_s1;
    s32 var_s1_2;
    u8 *var_s0;

    var_s1 = 0;
    do {
        func_80043914(D_8013C73C, (var_s1 + 0x10) & 0xFF, 1, 1, var_s1);
        var_s1 += 1;
    } while (var_s1 < 0x10);
    var_s0 = D_8011ECD0;
    var_s1_2 = 0;
    do {
        func_80048F64(var_s1_2 + 0x60);
        *(s8 *)((unsigned char *)var_s0 + (6529)) = 4;
        *(s32 *)((unsigned char *)var_s0 + (6584)) = 0x01000000;
        *(s8 *)((unsigned char *)var_s0 + (6531)) = 0x80;
        *(s32 *)((unsigned char *)var_s0 + (6540)) = (s32) D_8013C734;
        *(s32 *)((unsigned char *)var_s0 + (6544)) = (s32) D_8013C738;
        temp_t0 = var_s1_2 + 3;
        var_s1_2 += 1;
        var_s0 += 0x44;
        *(s16 *)((unsigned char *)var_s0 + (6482)) = temp_t0;
        *(s8 *)((unsigned char *)var_s0 + (6466)) = 1;
        *(s8 *)((unsigned char *)var_s0 + (6467)) = 0;
        *(s16 *)((unsigned char *)var_s0 + (6498)) = -0xA0;
        *(s16 *)((unsigned char *)var_s0 + (6502)) = -0x78;
        *(s8 *)((unsigned char *)var_s0 + (6465)) = 0x80;
        *(s8 *)((unsigned char *)var_s0 + (6527)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (6464)) = 0;
        *(void **)((unsigned char *)var_s0 + (6512)) = (void *) D_8013C730;
    } while (var_s1_2 != 6);
    D_80120666 = (s16) D_80122EAC;
    D_8012071F = 0;
    D_80120763 = 0;
    D_801207A6 = 0x40;
    D_801207BC = 0x8B;
    func_80048514(5, D_8013C730->unk0, 1);
    func_80048514(6, *(s32 *)((unsigned char *)D_8013C730 + (4)), 1);
    func_80048514(7, *(s32 *)((unsigned char *)D_8013C730 + (8)), 1);
    func_80048514(8, *(s32 *)((unsigned char *)D_8013C730 + (12)), 1);
    func_80048514(9, *(s32 *)((unsigned char *)D_8013C730 + (16)), 1);
    func_80048514(0xA, *(s32 *)((unsigned char *)D_8013C730 + (20)), 1);
    func_80048514(0x25, *(s32 *)((unsigned char *)D_8013C730 + (24)), 1);
    func_80048514(0x26, *(s32 *)((unsigned char *)D_8013C730 + (28)), 1);
    func_80048514(0x27, *(s32 *)((unsigned char *)D_8013C730 + (32)), 1);
    func_80048514(0x28, *(s32 *)((unsigned char *)D_8013C730 + (36)), 1);
    func_80048514(0x29, *(s32 *)((unsigned char *)D_8013C730 + (40)), 1);
}
