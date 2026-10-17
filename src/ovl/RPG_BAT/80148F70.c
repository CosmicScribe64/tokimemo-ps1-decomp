#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80148F70(void) {
    s32 flag;
    u16 stat;
    s32 base;
    s32 half;

    stat = D_800E6280.unk_66C[D_8015EBA0].unk_00;
    if (stat < 50) {
        base = 50;
    } else if (stat > 550) {
        base = 550;
    } else {
        base = stat;
    }
    base = base * 10 - 500;
    if (D_8015EB98 & 0xF0000) {
        func_801495DC();
    }
    if (D_8015EB98 & 0x80000000) {
        func_801495DC();
    }
    flag = D_8015EDC4 & 0x100000;
    if (flag || (D_8015EDC4 & 0x600000) || (D_8015EDC4 & 0x08000000)) {
        if (D_8015ED98 & 2) {
            switch (D_8015EBA0) {
            case 2:
                if (flag) {
                    D_8015EBB0 = func_8013EA00(2500, 2500) + 2500;
                    D_8015EBB4 = func_8013EA00(2500, 2500) + 2500;
                    D_8015EBB8 = func_8013EA00(2500, 2500) + 2500;
                    if (D_8015EBAC == 0) {
                        D_8015EBB0 = 9999;
                    }
                    if (D_8015EBAC == 1) {
                        D_8015EBB4 = 9999;
                    }
                    if (D_8015EBAC == 2) {
                        D_8015EBB8 = 9999;
                    }
                } else {
                    D_8015EBB0 = 9999;
                }
                break;
            case 5:
                if (flag) {
                    D_8015EBB0 = func_8013EA00(2500, 1000) + 2000;
                    D_8015EBB4 = func_8013EA00(2500, 1000) + 2000;
                    D_8015EBB8 = func_8013EA00(2500, 1000) + 2000;
                    if (D_8015EBAC == 0) {
                        D_8015EBB0 += 2000;
                    }
                    if (D_8015EBAC == 1) {
                        D_8015EBB4 += 2000;
                    }
                    if (D_8015EBAC == 2) {
                        D_8015EBB8 += 2000;
                    }
                } else {
                    D_8015EBB0 = func_8013EA00(2500, 1000) + 4000;
                }
                break;
            case 6:
                if (flag) {
                    D_8015EBB0 = func_8013E9A8(50) * 100 + 2000;
                    D_8015EBB4 = func_8013E9A8(50) * 100 + 1000;
                    D_8015EBB8 = func_8013E9A8(50) * 100 + 1000;
                } else {
                    D_8015EBB0 = func_8013E9A8(50) * 100 + 3000;
                }
                break;
            case 1:
                if (flag) {
                    D_8015EBB0 = func_8013EA00(6000, base);
                    D_8015EBB4 = func_8013EA00(6000, base);
                    D_8015EBB8 = func_8013EA00(6000, base);
                } else {
                    D_8015EBB0 = func_8013EA00(6000, base);
                }
                break;
            case 4:
                if (flag) {
                    if (func_8013E9A8(100) < 20) {
                        D_8015EBB0 = 1000;
                    } else {
                        D_8015EBB0 = func_8013EA00(base / 2, 3500) + 2500;
                    }
                } else {
                    D_8015EBB0 = func_8013EA00(base / 2, 3500) + 2500;
                }
                break;
            case 0:
                D_8015EBB0 = func_8013E9A8(base / 1000 + 1) * 1000 + 5000;
                break;
            case 9:
                half = base / 2;
                D_8015EBB0 = func_8013EA00(half, half) + 6000;
                break;
            case 3:
                D_8015EBB0 = func_8013EA00(1500, base) + 3000;
                break;
            case 7:
                D_8015EBB0 = func_8013EA00(base, base + 2000) + 1000;
                break;
            case 10:
                D_8015EBB0 = func_8013EA00(base, base + 1000) + 2000;
                break;
            case 8:
                half = (base + 2000) / 2;
                D_8015EBB0 = func_8013EA00(half, half) + 4000;
                break;
            case 11:
                switch (D_8015EC34) {
                case 0:
                case 1:
                    D_8015EC34 = 0;
                    D_8015EBB0 = func_8013EA00(500, 500);
                    break;
                case 2:
                case 3:
                    D_8015EC34 = 1;
                    D_8015EBB0 = func_8013EA00(1000, 1000);
                    break;
                case 4:
                case 5:
                    D_8015EC34 = 2;
                    D_8015EBB0 = func_8013EA00(1500, 1500);
                    break;
                case 6:
                case 7:
                    D_8015EC34 = 3;
                    D_8015EBB0 = func_8013EA00(2000, 2000);
                    break;
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                    D_8015EC34 = 4;
                    D_8015EBB0 = func_8013EA00(2500, 2500);
                    break;
                default:
                    D_8015EC34 = 5;
                    if (flag) {
                        D_8015EBB0 = 9999;
                    } else {
                        D_8015EDC0 -= 30000;
                        D_8015EBB0 = 9999;
                    }
                    break;
                }
                break;
            }
        } else {
            func_801495DC();
        }
    }
    if (D_8015EBB0 >= 10000) {
        D_8015EBB0 = 9999;
    }
    if (D_8015EBB4 >= 10000) {
        D_8015EBB4 = 9999;
    }
    if (D_8015EBB8 >= 10000) {
        D_8015EBB8 = 9999;
    }
}

void func_801495DC(void) {
    s32 rate;
    s32 lv;
    s32 base;
    s32 r;

    rate = D_800E6280.unk_10C.unk_02;
    lv = (rate > 300) ? 300 : rate;
    base = func_8013EA00(100, 100) + lv * 9 / 2;
    r = func_8013E9A8(0x1000);
    if (r == 0) {
        D_8015EBB0 = 9999;
    } else if (r < 0x40) {
        D_8015EBB0 = 0;
    } else if (r < 0x10E) {
        D_8015EBB0 = func_8013EA00(500, 1000) + base;
    } else {
        D_8015EBB0 = base;
    }
    if (D_8015EBB0 >= 10000) {
        D_8015EBB0 = 9999;
    }
    if ((D_8015EDC4 & 0x600000) && D_8015EBA4 == 0 && D_8015EBB0 != 0) {
        D_8015EDC0 -= 1500;
    }
}
