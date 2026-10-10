#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 D_80146060;

void func_80135900(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_80146060;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void func_80135988(void) {
    s16 v;

    func_800BCE10(&D_800D92A0, "初詣");
    if (D_80145F60 != 0) {
        v = 0;
    } else {
        v = 0xF;
    }
    if (D_80145EC8 == 0) {
        v += 2;
    }
    /* FAKE: `^ 0` keeps IDO from dropping the second, identical store below; real source unknown. T-8010 */
    D_80145EB4 = v ^ 0;
    if (D_80145EC8 == 0) {
        D_80145EB4 = v;
        if (D_800F62CF == 5) {
            v += 2;
            D_80145EB4 = v;
        }
    }
    func_8004DE1C();
}

void func_80135A20(void) {
    if (D_80145F60 != 0) {
        func_801343A4();
        return;
    }
    func_8004DE1C();
}

void func_80135A5C(void) {
    if (D_8012E66C != 0) {
        func_8008E2E0();
        return;
    }
    func_8008E280();
}

void func_80135A98(void) {
    func_80090960(5, 0);
    func_8008A0D4(0x418C);
    func_8004DE1C();
}

void func_80135ACC(void) {
    if ((D_800F62CF != 5) || (D_80145F60 != 0) || (D_80145EC8 == 0)) {
        func_8004DDD8();
        return;
    }
    func_8004DE1C();
}

void func_80135B30(void) {
    if (D_80145F60 != 0) {
        D_800F647A += 4;
    }
    func_8004DE1C();
}

void func_80135B70(void) {
    if (D_80145EC8 == 0) {
        func_8004DE1C();
        return;
    }
    func_801343A4();
}

void func_80135BAC(void) {
    if ((D_800F62CF == 5) && (D_80145EC8 == 0)) {
        func_801343A4();
        return;
    }
    func_8004DE1C();
}

void func_80135C00(void) {
    if (D_80145EC8 != 0) {
        func_801343A4();
        return;
    }
    func_8004DE1C();
}

void func_80135C3C(void) {
    D_800F53A0.girl[5].unk_0A -= 1;
    D_800F53A0.girl[5].unk_06 -= 1;
    D_800F53A0.girl[5].unk_0E += 0xA;
    func_8008FB00();
    func_8004DE1C();
}
