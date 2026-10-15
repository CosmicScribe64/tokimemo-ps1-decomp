#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[33])();
} FnTbl33; /* size 0x84 */
extern FnTbl33 D_80148640;

void func_801400D0(void) {
    s32 idx; /* unused: its stack slot sits above tbl (T-3330) */
    FnTbl33 tbl;

    tbl = D_80148640;
    tbl.f[D_800F647A](0x80);
    func_800BCDF0("e%d %d\n", D_800F594E, D_800F594F);
}

void func_80140160(void) {
    func_800504CC(1, 0xB755, 0xB747, 0xC956, 0xC90A, 0xC8EF);
    func_8004DE1C();
}

void func_801401A4(void) {
    if (func_80050AB8() == 1) {
        func_80086AB0(0x202);
        func_8004DE1C();
    }
}

void func_801401E0(void) {
    func_80051DD8(0xE, 0x80197000, 0xA474);
    func_8013EB10();
    func_8004DE1C();
}

void func_8014021C(void) {
    D_8012E68C = 0x1E;
    func_8004DE1C();
}

void func_80140244(void) {
    s32 lv = D_800F53DE; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    if (((u32) D_800F54A6 >= (u32) ((lv * 0x19) - 0x8B1)) && (D_800F594F != 0)) {
        D_800F594F += 1;
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        return;
    }
    if ((u32) D_800F54AE >= (u32) (((lv * 0x1E) - ((lv == 0x61) * 0xA)) - 0xADC)) {
        func_8004DE1C();
        return;
    }
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
}

void func_80140330(void) {
    if ((u32)D_800F54A6 >= (u32)(D_800F53DE * 0x19 - 0x8B1)) {
        D_80148620 = (D_800F594F >= 1) * 3 + 4;
        D_800F594F += 1;
        func_8004DE1C();
        return;
    }
    D_80148620 += 1;
    D_800F54B2 += 0xA;
    func_8008FB00();
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
}

void func_8014041C(void) {
    if (D_800F594F == 1) {
        func_8013FD20();
        return;
    }
    func_8004DE1C();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801400D0", func_80140464);

void func_801406A4(void) {
    func_8008A0D4(0x4169);
    func_8004DE1C();
}

void func_801406CC(void) {
    func_80072734(0x6940);
    func_8004DE1C();
}

void func_801406F4(void) {
    func_8004DDD8();
}

void func_80140714(void) {
    func_8004DEAC(4);
}
