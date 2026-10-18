#include "common.h"
#include "ovl/ETC.h"

void func_8013B1F0(u32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    D_8014F990 = 0;
    switch (arg0) {
    case 0:
        switch (arg1) {
        case 29:
        case 30:
        case 40:
            func_8013B98C(arg2);
            return;
        case 32:
            func_8013BA3C(arg2);
            return;
        case 34:
            func_8013BA80(arg2);
            return;
        default:
            func_8013BDEC(arg2, arg3);
            return;
        }
        break;
    case 1:
        switch (arg1) {
        case 32:
            func_8013BA3C(arg2);
            return;
        case 34:
            func_8013BA80(arg2);
            return;
        default:
            func_8013BDEC(arg2, arg3);
            return;
        }
        break;
    case 2:
        switch (arg1) {
        case 0x14:
            func_8013BDEC(arg2, arg3);
            return;
        case 0x1:
        case 0x22:
        case 0x35:
        case 0x3D:
        case 0x44:
        case 0x4A:
        case 0x4F:
        case 0x51:
            func_8013BD40(arg2, arg3);
            return;
        case 0x4C:
            func_8013BDEC(arg2, arg3);
            return;
        case 0x4E:
            func_8013B98C(arg2);
            return;
        default:
            return;
        }
        break;
    case 3:
        func_8013BD40(arg2, arg3);
        return;
    case 4:
        if ((arg1 == 1) && (arg2 == 9)) {
            D_8014F990 = 1;
            func_80062CD0(0x6638);
            return;
        }
        func_8013BCFC(arg2);
        return;
    case 5:
        if (arg1 < 3) {
            func_8013BC74(arg2);
            return;
        }
        func_8013BCB8(arg2);
        return;
    case 6:
        func_8013BD40(arg2, arg3);
        return;
    case 7:
        func_8013BD40(arg2, arg3);
        return;
    case 8:
        switch (arg1) {
        case 0x16:
        case 0x17:
        case 0x18:
            func_8013BB08(2, 2);
            return;
        case 0x1F:
            func_8013BB08(3, 4);
            return;
        case 0x29:
        case 0x2A:
            func_8013BB08(4, 7);
            return;
        case 0x32:
        case 0x33:
            func_8013BB08(5, 8);
            return;
        case 0x3C:
            D_8014F990 = 1;
            func_80062CD0(0x6449);
            return;
        case 0x3D:
            func_8013BB08(6, 9);
            return;
        case 0x5A:
            D_8014F990 = 1;
            func_80062CD0(0x6638);
            return;
        case 0xE:
        case 0x46:
            /* empty cases: they pull the jump table down to 0xE; without them IDO emits a compare chain (T-22026) */
            break;
        default:
            func_8013BD40(arg2, arg3);
            return;
        }
        break;
    case 9:
        func_8013BD40(arg2, arg3);
        return;
    case 10:
        switch (arg1) {
        case 0x20:
            func_8013BA3C(arg2);
            return;
        case 0x22:
            func_8013BA80(arg2);
            return;
        case 0x25:
        case 0x29:
        case 0x2A:
        case 0x2B:
        case 0x2C:
            func_8013BD40(arg2, arg3);
            return;
        case 0x2E:
            func_8013BC74(arg2);
            return;
        case 0x32:
            func_8013BCB8(arg2);
            return;
        case 0x30:
        case 0x31:
            func_8013BC30(arg2);
            return;
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x37:
        case 0x38:
        case 0x39:
        case 0x3A:
        case 0x3B:
        case 0x3C:
        case 0x3D:
            func_8013BB08(arg2, arg1 - 0x33);
            return;
        default:
            func_8013BDEC(arg2, arg3);
            return;
        }
        break;
    case 11:
        if ((arg1 == 0x1E) || (arg1 == 0x1D) || (arg1 == 0x28)) {
            D_8014F990 = 1;
            func_80062CD0(0x6BD8);
            return;
        }
        func_8013BDEC(arg2, arg3);
        return;
    case 12:
        func_8013BDEC(arg2, arg3);
        return;
    case 13:
        switch (arg1) {
        case 0x20:
            func_8013BA3C(arg2);
            return;
        case 0x22:
            func_8013BA80(arg2);
            return;
        case 0x26:
        case 0x27:
        case 0x28:
        case 0x29:
        case 0x2B:
            func_8013BD40(arg2, arg3);
            return;
        case 0x2A:
            if ((arg2 == 7) || (arg2 == 8) || (arg2 == 9)) {
                func_8013BDEC(arg2, arg3);
                return;
            }
            func_8013BD40(arg2, arg3);
            return;
        case 0x2C:
            func_8013BC74(arg2);
            return;
        case 0x2D:
            func_8013BCB8(arg2);
            return;
        case 0x2E:
        case 0x2F:
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x37:
        case 0x38:
            func_8013BB08(arg2, arg1 - 0x2E);
            return;
        default:
            func_8013BDEC(arg2, arg3);
            return;
        }
        break;
    case 14:
        if (arg1 == 0xB) {
            func_8013BB64(arg2);
            return;
        }
        if (arg1 == 0xC) {
            func_8013BBA8(arg2);
            return;
        }
        if (arg1 == 0xD) {
            func_8013BBEC(arg2);
            return;
        }
        func_8013BB08(arg2, arg1);
        return;
    case 15:
        func_8013BD40(arg2, arg3);
        return;
    case 16:
        func_8013BB08(arg2, arg1);
        return;
    case 18:
        switch (arg1) {
        case 5:
            func_8013BB08(arg2, 6);
            return;
        case 6:
            func_8013BB08(arg2, 7);
            return;
        case 7:
            func_8013BB08(arg2, 8);
            return;
        case 8:
            func_8013BB08(arg2, 9);
            return;
        case 9:
            func_8013BB08(arg2, 0xA);
            return;
        default:
            func_8013BD40(arg2, arg3);
            return;
        }
        break;
    case 19:
        func_8013BAC4(arg2);
        return;
    case 20:
        switch (arg1) {
        case 6:
        case 7:
        case 29:
            func_8013BB08(arg2, 1);
            return;
        case 10:
        case 11:
        case 30:
            func_8013BB08(arg2, 2);
            return;
        case 18:
        case 19:
        case 32:
        case 33:
            func_8013BB08(arg2, 4);
            return;
        default:
            func_8013BD40(arg2, arg3);
            return;
        }
        break;
    case 21:
        if ((arg2 == 0xC) || (arg2 == 0xB)) {
            func_8013BD40(arg2, arg3);
            return;
        }
        switch (arg1) {
        case 0:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            func_8013BD40(arg2, arg3);
            return;
        case 12:
            D_8014F990 = 1;
            func_80062CD0(0x5D6E);
            return;
        default:
            func_8013BDEC(arg2, arg3);
            break;
        }
        break;
    }
}

void func_8013B98C(s32 arg0) {
    s32 *p;
    s32 v;

    p = &D_8014FAFC[arg0];
    v = *p;
    if (v != -1) {
        D_8014F990 = 1;
        if (p == &D_8014FB1C) {
            switch (D_80150054) {
            case 0x5F:
                func_80062CD0(0x50F3);
                break;
            case 0x60:
                func_80062CD0(0x5120);
                break;
            default:
            case 0x61:
                func_80062CD0(0x514D);
                break;
            }
        } else {
            func_80062CD0(v);
        }
    }
}

void func_8013BA3C(s32 arg0) {
    s32 t = D_8014FB38[arg0];

    if (t != -1) {
        D_8014F990 = 1;
        func_80062CD0(t);
    }
}

void func_8013BA80(s32 arg0) {
    s32 t = D_8014FB74[arg0];

    if (t != -1) {
        D_8014F990 = 1;
        func_80062CD0(t);
    }
}

void func_8013BAC4(s32 arg0) {
    s32 t = D_8014FCA0[arg0];

    if (t != -1) {
        D_8014F990 = 1;
        func_80062CD0(t);
    }
}

void func_8013BB08(s32 arg0, s32 arg1) {
    s32 t = D_8014FD90[arg0][arg1];

    if (t != -1) {
        D_8014F990 = 1;
        func_80062CD0(t);
    }
}

void func_8013BB64(s32 arg0) {
    s32 t = D_8014FCDC[arg0];

    if (t != -1) {
        D_8014F990 = 1;
        func_80062CD0(t);
    }
}

void func_8013BBA8(s32 arg0) {
    s32 t = D_8014FD18[arg0];

    if (t != -1) {
        D_8014F990 = 1;
        func_80062CD0(t);
    }
}

void func_8013BBEC(s32 arg0) {
    s32 t = D_8014FD54[arg0];

    if (t != -1) {
        D_8014F990 = 1;
        func_80062CD0(t);
    }
}

void func_8013BC30(s32 arg0) {
    s32 t = D_8014FC64[arg0];

    if (t != -1) {
        D_8014F990 = 1;
        func_80062CD0(t);
    }
}

void func_8013BC74(s32 arg0) {
    s32 t = D_8014FBEC[arg0];

    if (t != -1) {
        D_8014F990 = 1;
        func_80062CD0(t);
    }
}

void func_8013BCB8(s32 arg0) {
    s32 t = D_8014FC28[arg0];

    if (t != -1) {
        D_8014F990 = 1;
        func_80062CD0(t);
    }
}

void func_8013BCFC(s32 arg0) {
    s32 t = D_8014FBB0[arg0];

    if (t != -1) {
        D_8014F990 = 1;
        func_80062CD0(t);
    }
}

void func_8013BD40(s32 arg0, s32 arg1) {
    s32 v;

    switch (D_80150058) {
    case 6:
    case 7:
    case 8:
    case 9:
        v = D_8014FA84[arg0];
        if (v != -1) {
            D_8014F990 = 1;
            func_80062CD0(v, arg0);
        }
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 10:
    case 11:
    case 12:
        v = D_8014FAC0[arg0];
        if (v != -1) {
            D_8014F990 = 1;
            func_80062CD0(v, arg0);
        }
        break;
    }
}

void func_8013BDEC(s32 arg0, s32 arg1) {
    s32 t;

    switch (arg1) {
    case 0:
        t = D_8014F994[arg0];
        if (t != -1) {
            D_8014F990 = 1;
            func_80062CD0(t);
        }
        break;
    case 1:
        t = D_8014F9D0[arg0];
        if (t != -1) {
            D_8014F990 = 1;
            func_80062CD0(t);
        }
        break;
    case 2:
        t = D_8014FA0C[arg0];
        if (t != -1) {
            D_8014F990 = 1;
            func_80062CD0(t);
        }
        break;
    case 3:
        t = D_8014FA48[arg0];
        if (t != -1) {
            D_8014F990 = 1;
            func_80062CD0(t);
        }
        break;
    }
}
