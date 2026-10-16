void func_80079070(void) {
    u8 sp32;
    u8 sp31;
    u8 sp30;
    u8 sp2F;
    s32 *var_t4;

    switch ((u8) D_800E6280.unk_110E) {
    case 0x3A:
        D_80123110 = (u32) &D_80132000;
        break;
    case 0x82:
        D_80123110 = (u32) &D_80132000;
        break;
    case 0x76:
        D_80123110 = (u32) &D_80132140;
        break;
    case 0x75:
        D_80123110 = (u32) &D_80139150;
        break;
    case 0x74:
        D_80123110 = (u32) &D_80133A2C;
        break;
    case 0x80:
        D_80123110 = (u32) &D_80137890;
        break;
    case 0x81:
        D_80123110 = (u32) &D_8013B5B0;
        break;
    case 0x72:
        var_t4 = &D_80141280;
block_57:
        D_80123110 = (u32) var_t4;
        break;
    case 0x60:
        D_80123110 = (u32) &D_80132000;
        break;
    case 0x5F:
        D_80123110 = (u32) &D_80132FF8;
        break;
    case 0x73:
        D_80123110 = (u32) &D_80132040;
        break;
    case 0x70:
        D_80123110 = (u32) &D_80142700;
        break;
    case 0x71:
        D_80123110 = (u32) &D_80144F70;
        break;
    case 0x11:
        D_80123110 = (u32) &D_80144AD0;
        break;
    case 0x14:
        D_80123110 = (u32) &D_8013209C;
        break;
    case 0x15:
        D_80123110 = (u32) &D_8014AF28;
        break;
    case 0x13:
        D_80123110 = (u32) &D_8014231C;
        break;
    case 0x43:
        var_t4 = &D_801397D0;
        goto block_57;
    case 0x45:
        D_80123110 = (u32) &D_80132000;
        break;
    case 0x63:
        D_80123110 = (u32) &D_80132334;
        break;
    case 0xC4:
        D_80123110 = (u32) &D_80145A6C;
        break;
    case 0x37:
        D_80123110 = (u32) &D_801492C0;
        break;
    case 0x36:
        D_80123110 = (u32) &D_8014ABD0;
        break;
    case 0xC2:
        D_80123110 = (u32) &D_80142974;
        break;
    case 0x91:
        D_80123110 = (u32) &D_801407F0;
        break;
    case 0xC3:
        D_80123110 = (u32) &D_8014636C;
        break;
    case 0x35:
        D_80123110 = (u32) &D_801465F8;
        break;
    case 0x5E:
        var_t4 = &D_8014C08C;
        goto block_57;
    case 0x50:
        D_80123110 = (u32) &D_801448D4;
        break;
    case 0x5C:
        D_80123110 = (u32) &D_80137F64;
        break;
    case 0x58:
        D_80123110 = (u32) &D_80132214;
        break;
    case 0x62:
        D_80123110 = (u32) &D_80132000;
        break;
    case 0x61:
        D_80123110 = (u32) &D_80133120;
        break;
    case 0x59:
        D_80123110 = (u32) &D_80132244;
        break;
    case 0x56:
        D_80123110 = (u32) &D_80134040;
        break;
    case 0x57:
        D_80123110 = (u32) &D_80134C90;
        break;
    case 0x5D:
        D_80123110 = (u32) &D_8013882C;
        break;
    case 0x5A:
        var_t4 = &D_801426D0;
        goto block_57;
    case 0x51:
        D_80123110 = (u32) &D_80137490;
        break;
    case 0x54:
        D_80123110 = (u32) &D_80132274;
        break;
    case 0x52:
        D_80123110 = (u32) &D_8013D180;
        break;
    case 0x53:
        D_80123110 = (u32) &D_801322D4;
        break;
    case 0x55:
        D_80123110 = (u32) &D_8013B484;
        break;
    case 0x5B:
        D_80123110 = (u32) &D_80137144;
        break;
    case 0x20:
        D_80123110 = (u32) &D_801335E8;
        break;
    case 0x21:
        D_80123110 = (u32) &D_801389A0;
        break;
    case 0x22:
        D_80123110 = (u32) &D_8013C780;
        break;
    default:
        var_t4 = &D_80132000;
        goto block_57;
    }
    ovl_load_state = 0;
    D_800E6280.unk_1108 = D_800E6280.unk_110E;
    sp32 = D_800E6280.unk_110A;
    sp31 = D_800E6280.unk_110D;
    sp2F = (u8) D_800E6280.unk_110C;
    sp30 = (u8) D_800E6280.unk_110B;
    func_80042908((s32) D_800E6280.unk_1109);
    func_80042940((s32) sp32);
    D_800E6280.unk_110D = sp31;
    D_800E6280.unk_110C = (s8) sp2F;
    D_800E6280.unk_110B = (s8) sp30;
}
