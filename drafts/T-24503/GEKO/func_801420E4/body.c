/* T-24503 unmatched natural top-six ranking source; cost357, no linked proof. */
void func_801420E4(void) {
    GekoShorts6 scores;
    GekoShorts6 girls;
    s16 girl;
    s32 value;

    scores = D_8014760C;
    girls = D_80147618;
    D_800E6280.unk_75D = 1;
    girl = 9;
    do {
        if (D_800E6280.unk_1BC[girl].unk_0C.f.b1) {
            value = D_800E6280.unk_1BC[girl].unk_02 + D_800E6280.unk_1BC[girl].unk_06;
            if (value >= scores.v[0]) {
                scores.v[5] = scores.v[4];
                girls.v[5] = girls.v[4];
                scores.v[4] = scores.v[3];
                girls.v[4] = girls.v[3];
                scores.v[3] = scores.v[2];
                girls.v[3] = girls.v[2];
                scores.v[2] = scores.v[1];
                girls.v[2] = girls.v[1];
                scores.v[1] = scores.v[0];
                girls.v[1] = girls.v[0];
                scores.v[0] = value;
                girls.v[0] = girl;
            }
            else if (value >= scores.v[1]) {
                scores.v[5] = scores.v[4];
                girls.v[5] = girls.v[4];
                scores.v[4] = scores.v[3];
                girls.v[4] = girls.v[3];
                scores.v[3] = scores.v[2];
                girls.v[3] = girls.v[2];
                scores.v[2] = scores.v[1];
                girls.v[2] = girls.v[1];
                scores.v[1] = value;
                girls.v[1] = girl;
            }
            else if (value >= scores.v[2]) {
                scores.v[5] = scores.v[4];
                girls.v[5] = girls.v[4];
                scores.v[4] = scores.v[3];
                girls.v[4] = girls.v[3];
                scores.v[3] = scores.v[2];
                girls.v[3] = girls.v[2];
                scores.v[2] = value;
                girls.v[2] = girl;
            }
            else if (value >= scores.v[3]) {
                scores.v[5] = scores.v[4];
                girls.v[5] = girls.v[4];
                scores.v[4] = scores.v[3];
                girls.v[4] = girls.v[3];
                scores.v[3] = value;
                girls.v[3] = girl;
            }
            else if (value >= scores.v[4]) {
                scores.v[5] = scores.v[4];
                girls.v[5] = girls.v[4];
                scores.v[4] = value;
                girls.v[4] = girl;
            }
            else if (value >= scores.v[5]) {
                scores.v[5] = value;
                girls.v[5] = girl;
            }
        }
        girl--;
    } while (girl > 0);
    D_800E6280.unk_75D = (u8)girls.v[5];
    D_800E6280.unk_F5F = 0xD;
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    func_8006612C((s32) &D_80144B50);
    *(s8 *)((unsigned char *)D_8011ECD0 + (207)) = 0;
    *(s8 *)((unsigned char *)D_8011ECD0 + (275)) = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    func_80137990();
    hizuke_init();
    message_window_init();
    func_80084E4C();
    D_800B593C = 0;
    func_800847B8(D_800E6280.unk_F5F);
    func_8008585C();
    func_80137990();
    D_80146274 = D_80145FF8;
    D_80146278 = D_80146134;
    D_8014627C = D_80146270;
    func_800AE0F0(D_800CA19C, "教室");
    D_8014755C = 0;
    D_800CA368 = 0xFF;
    func_8004284C();
    D_800E6280.unk_56C[0x52] = D_800E6280.unk_56C[0x52] + 1;
}
