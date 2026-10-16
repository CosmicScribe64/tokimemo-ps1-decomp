extern s32 D_8014361C;
extern s32 D_80143620;
extern u8 D_800E71DF;
extern u8 D_800E738A;
extern u8 D_800E738D;
void func_80133B44(void) {
    typedef struct { u8 b[3]; } UnalignedBytes3; /* size 0x3 */
    extern UnalignedBytes3 D_801435F8;
    s32 sp44;
    UnalignedBytes3 sp40;

    sp40 = D_801435F8;
    if (D_800E71DF != 0) {
        if (D_800E738D == 0) {
            set_kanji_string((s16) (-0x68 - (func_800AE0E0(D_800CA16C) * 4)), 0x30, 0, (s32) D_800CA16C, 0);
            set_kanji_string(-0x56, 0x30, 0, (s32) "『 ", 0);
            dialogue_render(-0x48, 0x30, 0, (s32) "それじゃ、神社に行くか。", 0, &D_8014361C, &D_80143620, 0);
            D_80143B00 = 0;
            D_80143B04 = 0;
            **(u8 **)D_80143AF8 = 7;
            ***(u8 ***)D_80143AF4 = sp40.b[0];
            *(u8 *)((unsigned char *)(**(u8 ***)D_80143AF4) + (1)) = sp40.b[1];
            *(s8 *)((unsigned char *)(**(u8 ***)D_80143AF4) + (2)) = 0;
            D_800E738D = 1;
        }
        sp44 = (s32) D_800E738A;
        func_801323D4();
        if (sp44 != D_800E738A) {
            func_80042808();
        }
    } else {
        func_8004284C();
    }
}
