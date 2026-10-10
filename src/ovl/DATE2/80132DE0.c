#include "common.h"
#include "ovl/DATE2.h"

typedef struct {
    void (*f[58])();
} FnTbl58; /* size 0xE8 */
extern FnTbl58 D_8013A4C8;

void func_80132DE0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl58 tbl;

    tbl = D_8013A4C8;
    idx = D_800E738A;
    tbl.f[idx](0x80);
}

void func_80132E5C(void) {
    func_800AE0A0((void *)(0x80180000 + D_800E7384 * 0x1400), 0x801C0000 + D_800E7384 * 0x1400, 0x1400);
    D_800E7384 += 1;
    if (D_800E7384 == 0x12) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80132EC8);

void func_80132F48(void) {
    s32 pad; /* FAKE: unused local that moves rect to sp+0x2C like the original (T-3330 frame rule). T-4050 */
    RECT rect;

    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C8E0(&rect, (void *)0x80180000);
    D_800E71DF = D_800E69DD;
    func_800847B8(D_800E69DD);
    func_80133258();
    func_8004284C();
}

void func_80132FB0(void) {
    func_800AE0A0((void *)(0x801C0000 + D_800E7384 * 0x1400), 0x80180000 + D_800E7384 * 0x1400, 0x1400);
    D_800E7384 += 1;
    if (D_800E7384 == 0x12) {
        func_80083474();
    }
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_8013301C);

void func_80133088(void) {
    RECT rect;

    func_80083474();
    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C884(&rect, (void *)0x80180000);
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_801330D4);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80133258);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80133494);

void func_8013357C(void) {
    func_80062CD0(0x6C8C);
    func_8004284C();
}

void func_801335A4(void) {
    D_800E71DF = 0;
    get_p_name(&D_800CA17C, 0);
    func_80062CD0(0x55DF);
    func_8004284C();
}

void func_801335E4(void) {
    D_800E71DF = D_800E69DD;
    func_800847B8(D_800E69DD);
    func_80133258();
    func_80083B24();
}

void func_80133620(void) {
    D_800E71DF = D_800E69DD;
    func_80133258();
    func_800847B8(D_800E71DF);
    switch (D_800E71DF) {
    case 1:
        func_80062CD0(0x5DC8);
        break;
    case 2:
        func_80062CD0(0x52B5);
        break;
    case 3:
        func_80062CD0(0x5936);
        break;
    case 4:
        func_80062CD0(0x6746);
        break;
    case 5:
        func_80062CD0(0x603E);
        break;
    case 6:
        func_80062CD0(0x62B4);
        break;
    case 7:
        func_80062CD0(0x5B7F);
        break;
    case 8:
        func_80062CD0(0x506C);
        break;
    case 9:
        func_80062CD0(0x652A);
        break;
    }
    func_8004284C();
}

void func_80133720(void) {
    if (D_80122CDC != 0) {
        func_80133620();
        D_8013A4C4 |= 1 << (s8)D_8013A4C0;
    } else {
        func_801335A4();
    }
    func_80133258();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80133784);

void func_80133884(void) {
    if (D_8013A4C0 == 2) {
        func_80042808();
        return;
    }
    func_8004284C();
}

void func_801338C4(void) {
    D_8013A4C0 += 1;
    func_80042940(0x1B);
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_801338F8);

void func_801339F8(void) {
    D_8013A5B0 = 0x801C6000;
    D_8013A5B4 = 0x801AE000;
    D_8013A5B8 = 0x801B2000;
    D_8013A5BC = 0x801B6000;
    D_8013A5C0 = 0x801BA000;
    D_8013A5C4 = 0x801BE000;
    D_8013A5C8 = 0x801C2000;
}

void func_80133A6C(void) {
    switch (D_8013A4C0) {                           /* irregular */
    case 0:
        func_801348F0();
        return;
    case 1:
        if ((u8) D_800E62BE >= 0x61U) {
            func_80133AF0();
            return;
        }
        func_801350A4();
        return;
    default:
        func_80135E50();
        return;
    }
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80133AF0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_801348F0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_801350A4);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80135DA4);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80135E50);

void func_801365DC(s32 arg0) {
    s32 v;

    D_800B3D60 = 0;
    v = func_8005742C(arg0, 1);
    if (v == D_800B5939) {
        func_800AE0B0("same\n");
        D_800E7384 += 3;
        return;
    }
    if (v == 1 - D_800B5939) {
        func_800AE0B0("another\n");
        D_800E7384 += 2;
    }
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80136660);

s32 func_801366E8(void) {
    s32 v;

    v = func_80051A68(0);
    if ((D_800E6378 & 0xF) == D_800E62BF && ((u32)(D_800E6378 << 0x17) >> 0x1B) == D_800E62C0) {
        if (((u8)v & 0x7F) < 2U) {
            D_800E699E |= 4;
            /* FAKE: reading D_800E738A (= D_800E699E + 0x9EC) through the other symbol keeps as1 from hoisting the load above the store; real source unknown. T-4050 */
            D_800E69A2 = (&D_800E699E)[0x9EC] + 1;
            func_80042908(4);
            return 0;
        }
        D_800E699E |= 8;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80136794);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80136860);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80136A90);

void func_80136B7C(void) {
    bg_read_sub2(0x432B);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80136BA4);

void func_80136CEC(void) {
    D_8013A428 = 0x17;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80136D14);
