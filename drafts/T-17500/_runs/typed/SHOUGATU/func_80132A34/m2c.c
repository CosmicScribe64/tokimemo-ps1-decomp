void func_80132A34(void) {
    s32 var_t4;

    switch (D_800E6280.unk_F5F) {
    case 0:
        D_80143AF4 = D_80143A48;
        D_80143AF8 = D_80143A84;
        D_80143AFC = D_80143AC0;
        return;
    case 1:
        D_80143AF4 = D_80143A58;
        D_80143AF8 = D_80143A94;
        D_80143AFC = D_80143AD0;
        return;
    case 2:
        D_80143AF4 = D_80143A4C;
        D_80143AF8 = D_80143A88;
        D_80143AFC = D_80143AC4;
        return;
    case 3:
        D_80143AF4 = D_80143A50;
        D_80143AF8 = D_80143A8C;
        D_80143AFC = D_80143AC8;
        return;
    case 4:
        D_80143AF4 = D_80143A6C;
        D_80143AF8 = D_80143AA8;
        D_80143AFC = D_80143AE4;
        return;
    case 5:
        D_80143AF4 = D_80143A5C;
        var_t4 = D_80143AD4;
        D_80143AF8 = D_80143A98;
block_14:
        D_80143AFC = var_t4;
    default:
        return;
    case 6:
        D_80143AF4 = D_80143A64;
        D_80143AF8 = D_80143AA0;
        D_80143AFC = D_80143ADC;
        if ((u32) (func_80051A68(6) & 0x7F) < 2U) {
            D_80143AF4 = D_80143A60;
            D_80143AF8 = D_80143A9C;
            D_80143AFC = D_80143AD8;
            return;
        }
        break;
    case 7:
        D_80143AF4 = D_80143A54;
        D_80143AF8 = D_80143A90;
        D_80143AFC = D_80143ACC;
        return;
    case 8:
        D_80143AF4 = D_80143A44;
        D_80143AF8 = D_80143A80;
        D_80143AFC = D_80143ABC;
        return;
    case 9:
        D_80143AF4 = D_80143A68;
        D_80143AF8 = D_80143AA4;
        D_80143AFC = D_80143AE0;
        return;
    case 10:
        D_80143AF4 = D_80143A74;
        var_t4 = D_80143AEC;
        D_80143AF8 = D_80143AB0;
        goto block_14;
    }
}
