void func_80135F38(void) {
    switch ((u32)D_800E6280.unk_1104.w) {
    case 0x92:
        func_80044750(0x503);
        break;
    case 0x175:
        func_80044750(0x503);
        break;
    case 0x205:
        func_80044750(0x503);
        break;
    case 0x293:
        func_80044750(0x503);
        break;
    case 0x45F:
        func_80044750(0x503);
        break;
    case 0x4C3:
        func_80044750(0x503);
        break;
    case 0x529:
        func_80044750(0x503);
        break;
    case 0x269:
        func_80044750(0x507);
        break;
    case 0x3C6:
        func_80044750(0x50B);
        break;
    case 0x3C7:
        func_80044750(0x506);
        break;
    case 0x3E6:
        func_80044750(0x506);
        break;
    case 0x406:
        func_80044750(0x506);
        break;
    case 0x426:
        func_80044750(0x506);
        break;
    case 0x2DA:
        func_80044750(0x504);
        break;
    case 0x31A:
        func_80044750(0x504);
        break;
    case 0x35A:
        func_80044750(0x504);
        break;
    }
    /* FAKE: (&D_80122EAC)[0] is a non-plain operand, so IDO keeps the constant first in the
     * compares (beq a0,v1); the original probably reads a field of a main-exe aggregate around
     * D_80122EA8 that is not declared yet. T-18100 */
    if ((&D_80122EAC)[0] != 3 && D_800E6280.unk_1104.w == 0x3A7) {
        func_80044750(0x505);
    } else if ((&D_80122EAC)[0] == 3 && D_800E6280.unk_1104.w == 0x39D) {
        func_80044750(0x505);
    }
}
