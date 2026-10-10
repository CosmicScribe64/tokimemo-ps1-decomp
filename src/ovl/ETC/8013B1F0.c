#include "common.h"
#include "ovl/ETC.h"

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013B1F0", func_8013B1F0);

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
