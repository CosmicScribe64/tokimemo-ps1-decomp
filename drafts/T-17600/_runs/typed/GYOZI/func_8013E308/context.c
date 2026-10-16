#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_80148114;

void func_8013E1D0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl25 tbl;

    tbl = D_80148114;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void func_8013E24C(void) {
    func_800504CC(1, 0xB290, 0xB271, 0xBB4E, 0xBB06, 0xBAF5);
    func_8004DE1C();
}

void func_8013E290(void) {
    if (func_80050AB8() == 1) {
        func_80086AB0(0x202);
        func_8004DE1C();
    }
}

void func_8013E2CC(void) {
    func_80051DD8(9, 0x80197000, 0xA46B);
    func_8013DE30();
    func_8004DE1C();
}

void func_8013E308(void) {
    s16 var_v0;
    s32 temp_lo;

    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    func_800BCE10(&D_800D92A0, "廊下");
    D_800F6412 = 0;
    D_800F6458 = 1;
    func_8009068C();
    D_800F53DA = 0x80;
    D_801317EB = 0;
    D_800C51C4 = 0;
    func_80089200();
    func_800744A0();
    func_80074950();
    func_8008FC10();
    D_800F62CF = 0xC;
    func_8008F618(0xC);
    func_8013DE30();
    D_801480FC = D_8014806C;
    D_80148100 = D_801480A0;
    D_80148104 = D_801480D4;
    D_80148108 = 0;
    D_8014810C = 0;
    D_80148110 = 0;
    var_v0 = 0;
    do {
        temp_lo = var_v0 * 0x38;
        var_v0 += 1;
        if (*(u8 *)((unsigned char *)(&D_800F53A0.unk_000[temp_lo]) + (441)) & 1) {
            D_80148110 = 1;
        }
    } while (var_v0 < 0xB);
    func_8004DE1C();
}

void func_8013E47C(void) {
    func_80090960(7, 0);
    func_8008A0D4(0x3FED);
    func_8004DE1C();
}

void func_8013E4B0(void) {
    D_80148108 += ((u8) D_800F53DE >= 0x61U) * 4;
    func_8004DE1C();
}

void func_8013E4F4(void) {
    if (D_80148110 == 0) {
        D_80148108 += 1;
    }
    func_8004DE1C();
}

void func_8013E534(void) {
    if (D_80148110 != 0) {
        func_8004DE1C();
        return;
    }
    func_8013E148();
}

void func_8013E570(void) {
    if (D_80148110 != 0) {
        func_8004DDD8();
        return;
    }
    func_8004DE1C();
}
