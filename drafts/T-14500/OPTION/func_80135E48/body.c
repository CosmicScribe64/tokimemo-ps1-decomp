s32 func_80135E48(void) {
    s32 sp70[16];
    s16 sp64;
    s16 sp58;
    s16 sp4C;
    s16 *temp_a0;
    s16 *temp_a1;
    s16 *temp_t0;
    s16 *temp_t1;
    s16 temp_a3;
    s16 temp_t8;
    s32 temp_v1;
    u32 temp_hi;
    u8 *temp_v0;

    if (D_800E738D == 0) {
        D_80120910 = 4;
        D_8012091E = -0xA0;
        D_80120922 = -0x78;
        D_801208FB = 0;
        D_80120914 = 0x7FFF;
        D_800E738D = 1;
        *D_80120912 = 0x7FFF;
        return;
    }
    temp_a3 = 1 * 0x18;
    temp_t8 = ((s16) *D_80120912 / 17) * 0x10;
    D_80120914 = ((s16) D_80120914 / 17) * 0x10;
    *D_80120912 = temp_t8;
    if (temp_t8 < 0x1001) {
        D_80120914 = 0x1000;
        *D_80120912 = 0x1000;
        D_80120954 = 0xC;
        D_80120962 = 0x10;
        D_80120966 = -0x18;
        sp58 = 0x30;
        D_8012093F |= 0x80;
        sp4C = 0x10;
        sp64 = -0x18;
        temp_v1 = 1 * 2;
        temp_a0 = &(&sp70[0])[1];
        sp70[0] = 0x10;
        temp_v0 = (1 * 0x44) + D_80120650;
        temp_a1 = &(&sp64)[1];
        (*(s32 *)temp_a1) = temp_a3 - 0x18;
        (*(s32 *)temp_a0) = 0x10;
        *(s16 *)((unsigned char *)temp_v0 + (772)) = (s16) (temp_v1 + 0xC);
        *(s16 *)((unsigned char *)temp_a0 + (2)) = 0x10;
        *(s16 *)((unsigned char *)temp_v0 + (790)) = (s16) (*(s32 *)temp_a1);
        *(u8 *)((unsigned char *)temp_v0 + (751)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (751)) | 0x80);
        *(s16 *)((unsigned char *)temp_v0 + (786)) = (s16) (*(s32 *)temp_a0);
        *(s16 *)((unsigned char *)temp_a1 + (2)) = temp_a3;
        *(s16 *)((unsigned char *)temp_v0 + (840)) = (s16) (temp_v1 + 0xE);
        *(s16 *)((unsigned char *)temp_v0 + (854)) = (s16) *(s16 *)((unsigned char *)temp_a0 + (2));
        *(s16 *)((unsigned char *)temp_a0 + (4)) = 0x10;
        *(s16 *)((unsigned char *)temp_a1 + (4)) = (s16) (temp_a3 + 0x18);
        *(s16 *)((unsigned char *)temp_v0 + (858)) = (s16) *(s16 *)((unsigned char *)temp_a1 + (2));
        *(s16 *)((unsigned char *)temp_v0 + (908)) = (s16) (temp_v1 + 0x10);
        *(u8 *)((unsigned char *)temp_v0 + (819)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (819)) | 0x80);
        *(s16 *)((unsigned char *)temp_v0 + (922)) = (s16) *(s16 *)((unsigned char *)temp_a0 + (4));
        *(s16 *)((unsigned char *)temp_a1 + (6)) = (s16) (temp_a3 + 0x30);
        temp_t1 = &(&sp4C)[1];
        *(u8 *)((unsigned char *)temp_v0 + (887)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (887)) | 0x80);
        *(s16 *)((unsigned char *)temp_a0 + (6)) = 0x10;
        temp_t0 = &(&sp58)[1];
        *(s16 *)((unsigned char *)temp_v0 + (926)) = (s16) *(s16 *)((unsigned char *)temp_a1 + (4));
        *(s16 *)((unsigned char *)temp_v0 + (976)) = (s16) (temp_v1 + 0x12);
        *(s16 *)((unsigned char *)temp_t1 + (2)) = 0x10;
        *(s16 *)((unsigned char *)temp_t1 + (4)) = 0x10;
        *(s16 *)((unsigned char *)temp_t1 + (6)) = 0x10;
        (*(s32 *)temp_t1) = 0x10;
        *(u8 *)((unsigned char *)temp_v0 + (955)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (955)) | 0x80);
        *(s16 *)((unsigned char *)temp_t0 + (2)) = 0x30;
        *(s16 *)((unsigned char *)temp_t0 + (4)) = 0x30;
        *(s16 *)((unsigned char *)temp_t0 + (6)) = 0x30;
        (*(s32 *)temp_t0) = 0x30;
        *(s16 *)((unsigned char *)temp_v0 + (990)) = (s16) *(s16 *)((unsigned char *)temp_a0 + (6));
        *(s16 *)((unsigned char *)temp_v0 + (994)) = (s16) *(s16 *)((unsigned char *)temp_a1 + (6));
        menu_set(1, 5, &sp70[0], &sp64, &sp58, &sp4C);
        temp_hi = (u8) D_800E71EF % 10U;
        switch (temp_hi) {
        default:
            D_80120954 += 1;
            D_800E7314 = 0;
            break;
        case 2:
            D_80120998 += 1;
            D_800E7314 = 1;
            break;
        case 3:
            D_801209DC += 1;
            D_800E7314 = 1;
            break;
        case 4:
            D_80120A20 += 1;
            D_800E7314 = 1;
            break;
        case 6:
            D_80120A64 += 1;
            D_800E7314 = 1;
            break;
        }
        func_8004284C();
    }
}
