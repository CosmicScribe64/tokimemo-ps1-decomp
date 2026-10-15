void func_80142AF0(void *arg0, s32 arg1) {
    s16 temp_t1;
    s8 var_a2;
    u8 *temp_a0;
    u8 temp_t0;
    u8 temp_t9;
    u8 *temp_a2;
    u8 *temp_t0_2;

    temp_a2 = D_80158A8C + (arg1 * 0x14) + 0x38;
    if ((*D_80158A8C != 0) && (temp_a0 = &D_80158A8C[arg1], (*(u8 *)((unsigned char *)temp_a0 + (784)) != 0))) {
        temp_t0 = *(u8 *)((unsigned char *)temp_a2 + (8));
        if (temp_t0 & 0x80) {
            *(u8 *)((unsigned char *)temp_a2 + (8)) = (u8) (*(u8 *)((unsigned char *)temp_a2 + (8)) & 0xFF7F);
            *(s8 *)((unsigned char *)temp_a2 + (6)) = (s8) (*(s32 *)arg0);
            *(u8 *)((unsigned char *)temp_a2 + (9)) = (u8) *(s16 *)((unsigned char *)arg0 + (2));
            *(s8 *)((unsigned char *)(&D_80158A8C[arg1]) + (808)) = 0;
            return;
        }
        if (temp_t0 & 0x20) {
            *(u8 *)((unsigned char *)temp_a2 + (8)) = (u8) (*(u8 *)((unsigned char *)temp_a2 + (8)) & 0xFFDF);
            *(s8 *)((unsigned char *)(&D_80158A8C[arg1]) + (808)) = 0;
            return;
        }
        if (*(u8 *)((unsigned char *)temp_a2 + (9)) == 0) {
            var_a2 = *(u8 *)((unsigned char *)temp_a0 + (808)) + 2;
            temp_t0_2 = ((u8 *)arg0) + (var_a2 * 2);
            temp_t1 = (*(s32 *)temp_t0_2);
            if (temp_t1 == -1) {
                temp_t9 = *(u8 *)((unsigned char *)temp_a2 + (8)) | 1;
                *(u8 *)((unsigned char *)temp_a2 + (8)) = temp_t9;
                *(u8 *)((unsigned char *)temp_a2 + (8)) = (u8) (temp_t9 | 0x40);
                *(s8 *)((unsigned char *)temp_a2 + (6)) = (s8) *(s16 *)((unsigned char *)temp_t0_2 + (-4));
            } else if (temp_t1 == -2) {
                var_a2 = 0;
                *(s8 *)((unsigned char *)temp_a2 + (6)) = (s8) (*(s32 *)arg0);
                *(u8 *)((unsigned char *)temp_a2 + (9)) = (u8) *(s16 *)((unsigned char *)arg0 + (2));
            } else if (temp_t1 == -3) {
                *(s8 *)((unsigned char *)(&D_80158A8C[arg1]) + (784)) = 0;
                *(u8 *)((unsigned char *)temp_a2 + (8)) = (u8) (*(u8 *)((unsigned char *)temp_a2 + (8)) | 1);
            } else {
                *(s8 *)((unsigned char *)temp_a2 + (6)) = (s8) temp_t1;
                *(u8 *)((unsigned char *)temp_a2 + (9)) = (u8) *(s16 *)((unsigned char *)temp_t0_2 + (2));
            }
            *(s8 *)((unsigned char *)(&D_80158A8C[arg1]) + (808)) = var_a2;
            return;
        }
        *(u8 *)((unsigned char *)temp_a2 + (9)) = (u8) (*(u8 *)((unsigned char *)temp_a2 + (9)) - 1);
    }
}
