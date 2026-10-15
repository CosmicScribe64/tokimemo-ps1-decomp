void func_801487D0(s16 arg0, s16 arg1, u8 arg2) {
    void *sp38;
    void *sp30;
    s32 temp_t2;
    s32 temp_t2_2;
    s32 temp_t3;
    s32 temp_t4;
    s32 temp_v0;
    u32 temp_t9;
    void *temp_a0;
    void *temp_t0;
    void *temp_t1;

    temp_t4 = arg2 & 0xFF;
    temp_t0 = (*(s32 *)((unsigned char *)D_80158A68 + (8)) * 0x80B8) + D_80158AB4;
    if (temp_t4 != 0x20) {
        temp_t1 = *(void **)((unsigned char *)temp_t0 + (4));
        *(void **)((unsigned char *)temp_t0 + (4)) = (void *) (temp_t1 + 0x14);
        temp_t9 = (temp_t4 - 0x20) & 0xFF;
        *(s8 *)((unsigned char *)temp_t1 + (3)) = 4;
        *(s16 *)((unsigned char *)temp_t1 + (16)) = 6;
        *(s16 *)((unsigned char *)temp_t1 + (18)) = 8;
        *(s8 *)((unsigned char *)temp_t1 + (4)) = 0x80;
        *(s8 *)((unsigned char *)temp_t1 + (5)) = 0x80;
        *(s8 *)((unsigned char *)temp_t1 + (6)) = 0x80;
        *(s8 *)((unsigned char *)temp_t1 + (7)) = 0x65;
        *(s8 *)((unsigned char *)temp_t1 + (12)) = (s8) ((temp_t9 & 0xF) * 6);
        *(s8 *)((unsigned char *)temp_t1 + (13)) = (s8) (((temp_t9 >> 4) * 8) + 0x80);
        *(s16 *)((unsigned char *)temp_t1 + (14)) = 0x7FFC;
        *(s16 *)((unsigned char *)temp_t1 + (10)) = arg1;
        *(s16 *)((unsigned char *)temp_t1 + (8)) = arg0;
        temp_t3 = temp_t1->unk0;
        temp_t1->unk0 = (s32) (((u32) (((*(s32 *)((unsigned char *)temp_t0 + (32776)) & 0xFFFFFF) ^ temp_t3) << 8) >> 8) ^ temp_t3);
        temp_t2 = *(s32 *)((unsigned char *)temp_t0 + (32776));
        *(s32 *)((unsigned char *)temp_t0 + (32776)) = (s32) (((u32) ((((s32) temp_t1 & 0xFFFFFF) ^ temp_t2) << 8) >> 8) ^ temp_t2);
        temp_a0 = *(void **)((unsigned char *)temp_t0 + (4));
        *(void **)((unsigned char *)temp_t0 + (4)) = (void *) (temp_a0 + 0xC);
        sp30 = temp_t0 + 0x8000;
        sp38 = temp_a0;
        func_8009D294((s32) temp_a0, 0, 0, 0x1F, 0);
        temp_v0 = temp_a0->unk0;
        temp_a0->unk0 = (s32) (((u32) (((*(s32 *)((unsigned char *)temp_t0 + (32776)) & 0xFFFFFF) ^ temp_v0) << 8) >> 8) ^ temp_v0);
        temp_t2_2 = *(s32 *)((unsigned char *)temp_t0 + (32776));
        *(s32 *)((unsigned char *)temp_t0 + (32776)) = (s32) (((u32) ((((s32) temp_a0 & 0xFFFFFF) ^ temp_t2_2) << 8) >> 8) ^ temp_t2_2);
    }
}
