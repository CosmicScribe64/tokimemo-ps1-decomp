extern s8 D_800E62B6;
extern s8 D_800E62B7;
extern s8 D_800E62B8;

void func_8013B2A0(void) {
    s32 sp38;
    ? sp30;

    if (D_8015EDBC == 0) {
        func_8013B404();
        func_801347A8();
        func_80059048();
        sp30.unk0 = (s32) (unaligned s32) *(? *)((unsigned char *)D_8015EDB4 + (124));
        *(s32 *)((unsigned char *)(&sp30) + (4)) = (s32) (unaligned s32) *(? *)((unsigned char *)D_8015EDB4 + (128));
        sp38 = (s32) D_8015EDB4->unk84[1];
        func_80143AF4();
        *(? *)((unsigned char *)D_8015EDB4 + (124)) = (unaligned s32) sp30.unk0;
        *(? *)((unsigned char *)D_8015EDB4 + (128)) = (unaligned s32) *(s32 *)((unsigned char *)(&sp30) + (4));
        D_8015EDB4->unk84[1] = (u8) sp38;
        D_800E62B8 = 0x10;
        D_800E62B7 = 0x10;
        D_800E62B6 = 0x10;
        D_8015EDB0 = 0;
        D_8015EDBC = 0x44;
        func_800450F4(0, 0x202);
        func_8004284C();
    }
}
