void func_80135A04(void) {
    s32 sp58;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 sp48;
    s32 sp44;
    s32 *temp_v0;
    s32 temp_t6;
    u32 temp_hi;
    u32 temp_hi_2;
    u32 temp_hi_3;

    sp48 = 3;
    sp44 = 3;
    temp_v0 = &(&sp44)[2];
    (*(s32 *)temp_v0) = 3;
    *(s32 *)((unsigned char *)temp_v0 + (4)) = 3;
    *(s32 *)((unsigned char *)temp_v0 + (8)) = 3;
    *(s32 *)((unsigned char *)temp_v0 + (12)) = 3;
    (&sp44)[D_8015F3F0] = 5;
    func_80049A40(-0x88, -0x1C, 0x10E, 0x78, 4, 0x3F3F3F, 0x81);
    func_801357B0((s32) "SETTING", 0x1000, 0x1000, sp58, 3, -0x30, -0x40);
    func_801357B0((s32) "AUTO SHOT", 0x1000, 0x1000, sp44, 3, -0x78, -0x10);
    func_801357B0((s32) "ROLL BUTTON", 0x1000, 0x1000, sp48, 3, -0x78, 8);
    func_801357B0((s32) "SHOT BUTTON", 0x1000, 0x1000, sp4C, 3, -0x78, 0x20);
    func_801357B0((s32) "BOMB BUTTON", 0x1000, 0x1000, sp50, 3, -0x78, 0x38);
    func_801357B0((s32) "EXIT", 0x1000, 0x1000, sp54, 3, -0x78, 0x50);
    if ((D_8015F3F4 != 1) || (D_8015F3F0 != 0)) {
        temp_t6 = D_8015EDD8 & 1;
        switch (temp_t6) {
        case 0:
            func_801357B0((s32) "OFF", 0x1000, 0x1000, sp44, 3, 0x2A, -0x10);
            break;
        case 1:
            func_801357B0((s32) "ON", 0x1000, 0x1000, sp44, 3, 0x2A, -0x10);
            break;
        }
    }
    if ((D_8015F3F4 != 1) || (D_8015F3F0 != 1)) {
        temp_hi = (u8) D_8015EDDC % 3U;
        switch (temp_hi) {
        case 0:
            func_801357B0((s32) "R12-L12", 0x1000, 0x1000, sp48, 3, 0x2A, 8);
            break;
        case 1:
            func_801357B0((s32) "R1-L1", 0x1000, 0x1000, sp48, 3, 0x2A, 8);
            break;
        case 2:
            func_801357B0((s32) "R2-L2", 0x1000, 0x1000, sp48, 3, 0x2A, 8);
            break;
        }
    }
    if ((D_8015F3F4 != 1) || (D_8015F3F0 != 2)) {
        temp_hi_2 = (u8) D_8015EDE0 % 3U;
        switch (temp_hi_2) {
        case 0:
            func_801357B0((s32) "@", 0x1000, 0x1000, sp4C, 3, 0x2A, 0x20);
            break;
        case 1:
            func_801357B0((s32) "*", 0x1000, 0x1000, sp4C, 3, 0x2A, 0x20);
            break;
        case 2:
            func_801357B0((s32) "#", 0x1000, 0x1000, sp4C, 3, 0x2A, 0x20);
            break;
        }
    }
    if ((D_8015F3F4 != 1) || (D_8015F3F0 != 3)) {
        temp_hi_3 = (u8) D_8015EDE4 % 3U;
        switch (temp_hi_3) {
        case 0:
            func_801357B0((s32) "*", 0x1000, 0x1000, sp50, 3, 0x2A, 0x38);
            break;
        case 1:
            func_801357B0((s32) "#", 0x1000, 0x1000, sp50, 3, 0x2A, 0x38);
            break;
        case 2:
            func_801357B0((s32) "@", 0x1000, 0x1000, sp50, 3, 0x2A, 0x38);
            break;
        }
    }
}
