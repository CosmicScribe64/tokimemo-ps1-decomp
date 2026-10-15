/* NON_MATCHING: T-11820, 6 lines (registers only): the record-2 constants 0x20/0x40/0x30 get ugen temps t9/t1/t2 where the original has t1/t2/t3 (t9 busy there). The empty if before the 0x65 store (FAKE) splits the block so the 0x65 constant can take v0. */
void func_80149704(void) {
    u8 *scene;
    u8 *q;
    u8 *p;
    s32 i;

    scene = D_80158A60;
    q = D_80158AA4;
    p = D_80158AA8;
    func_8009C5E0(1);
    for (i = 0; i < 4; i++, p += 0x14) {
        *(s16 *)(p + 0xA) = D_80156C9C[i];
        *(s16 *)(p + 0xC) = i * 8 + 0xA0;
        *(u16 *)(p + 0xE) = D_80156CA4[i];
        p[2] = 0;
        p[0] = 1;
        *(s16 *)(p + 6) = 0;
        *(s16 *)(p + 0x10) = 0;
    }
    if (scene[0x53] == 0) {
        p[-0x14] = 0;
    }
    for (i = 0; i != 2; i++, p += 0x14) {
        *(s16 *)(p + 0xA) = D_80156CAC[i];
        *(s16 *)(p + 0xC) = i * 8 + 0xD0;
        *(u16 *)(p + 0xE) = D_80156CB0[i];
        p[2] = 0;
        p[0] = 1;
        *(s16 *)(p + 6) = 0;
        *(s16 *)(p + 0x10) = 0;
    }
    *(s16 *)(p + 0xA) = -0x70;
    *(s16 *)(p + 0xC) = 0x88;
    *(s16 *)(p + 0xE) = 0;
    p[2] = 3;
    p[0] = 0;
    *(s16 *)(p + 6) = 0;
    *(s16 *)(p + 0x10) = 0;
    q += 0x64;
    q[0] = 0;
    q[1] = 0;
    *(s16 *)(q + 0x44) = 0;
    *(s16 *)(q + 0x14) = -0x60;
    *(s16 *)(q + 0x16) = 0x38;
    *(s16 *)(q + 0x3C) = 0x80;
    *(s16 *)(q + 0x3E) = 0x30;
    q[0x46] = 0;
    q[0x47] = 0;
    *(s16 *)(q + 0x40) = 0xB;
    *(s16 *)(q + 0x42) = 0x3C20;
    if (q) {} /* FAKE: splits the uopt block so 0x65 can take v0. T-11820 */
    q[0x3B] = 0x65;
    *(s16 *)(q + 0x10) = 0;
    q += 0x64;
    q[0] = 0;
    q[1] = 0;
    *(s16 *)(q + 0x44) = 0;
    *(s16 *)(q + 0x14) = 0x20;
    *(s16 *)(q + 0x16) = 0x38;
    *(s16 *)(q + 0x3C) = 0x40;
    *(s16 *)(q + 0x3E) = 0x30;
    q[0x46] = 0;
    q[0x47] = 0x30;
    *(s16 *)(q + 0x40) = 0xB;
    *(s16 *)(q + 0x42) = 0x3C20;
    q[0x3B] = 0x65;
    *(s16 *)(q + 0x10) = 0;
}
