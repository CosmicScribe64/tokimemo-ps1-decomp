#include "common.h"
#include "ovl/TACO.h"

s32 func_80137590(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013760C();
        break;
    case 1:
        func_8013788C();
        break;
    }
    if (D_800E6280.unk_10F8 & 0x10) {
        func_801432F0("DEMO PLAY", -0x40, 0, 2);
    }
}

/* s32 without a return (v0 live at the exit) and the u8 view of the selector reproduce the original switch; the empty
   default adds the basic blocks that keep the 0x801AC000 constant out of a third s register. T-16561 */
s32 func_8013760C(void) {
    s32 i;
    s32 end;

    switch (*(u8 *)&D_800E6280.unk_110D) {
    case 0:
        func_8013AFDC();
        D_8015F4F0 = 0x80;
        D_8015EDCC = 0;
        D_8015EDF0 = 0;
        D_8015EDB0 = 0x80;
        D_8015EDEC = 0x4650;
        func_80134884();
        D_8015EDBC = 0x10;
        D_800E6280.unk_036 = 0;
        D_800E6280.unk_037 = 0;
        D_800E6280.unk_038 = 0;
        func_800591D8(1);
        k_reset(0);
        back_clear_switch(1);
        func_80143A74();
        func_8014394C();
        func_80059048();
        func_801347A8();
        func_80143AF4();
        i = 0;
        end = 0x29F8;
        do {
            /* FAKE: the (u8 *)i + (u32)ptr form puts the index first in the addu, as the original; plain ptr + i puts the pointer first. T-16561 */
            func_80099E30(0, (u8 *)i + (u32)D_8015EDB4 + 0x14);
            func_801438F0(0x801AC000, 0, (u8 *)i + (u32)D_8015EDB4);
            i += 0x88;
        } while (i != end);
        func_801322AC();
        func_80132220();
        func_801334BC();
        D_8011ED17 &= 0x7F;
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (D_8015EDBC == 0) {
            func_8015ABF0();
            D_8015EDB0 = 0x80;
            D_8015EDB4[0].unk84[1] = 0xFF;
            D_8015EDB4[8].unk84[1] = 0xFF;
            switch (D_800E6280.unk_10F8 & 3) {
            case 0:
                D_8015EDB4[0].unk72 = 0;
                break;
            case 1:
                D_8015EDB4[0].unk72 = 1;
                break;
            }
            load_csr_tp(0x80162000, 0xE, 1, 0, 0, 0x100, 0x100);
            load_csr_tp(0x80172000, 0xC, 1, 0, 0, 0x100, 0x100);
            load_csr_tp(0x80182000, 0xA, 1, 0, 0, 0x80, 0x80);
            func_8004284C();
        }
        break;
    default:
        break;
    }
}

void func_8013788C(void) {
    u32 t;

    func_80154960();
    if (D_800E6280.unk_F88 != 0) {
        func_80143AF4();
        func_801347A8();
        func_80059048();
        func_8015ABF0();
        func_800591D8(0);
        func_80042908(2);
    } else {
        D_800E6280.unk_F88 |= 0x20;
        t = D_800E6280.unk_1100 & 0x1FF;
        D_800E6280.unk_F80 |= 0x20;
        if (t >= 0x79 && t < 0x8C) {
            D_800E6280.unk_F80 |= 0x2000;
        }
        if (t >= 0xAB && t < 0xBE) {
            D_800E6280.unk_F80 |= 0x4000;
        }
        if (t >= 0xDD && t < 0xF0) {
            D_800E6280.unk_F80 |= 0x8000;
        }
        if (t >= 0x10F && t < 0x122) {
            D_800E6280.unk_F80 |= 0x1000;
        }
        if (t >= 0x191 && t < 0x1B8) {
            D_800E6280.unk_F80 |= 0x3000;
        }
        if (t >= 0x1CD && t < 0x1E0) {
            D_800E6280.unk_F80 |= 0xC000;
        }
        if (t >= 0x191 && t < 0x1B8) {
            D_800E6280.unk_F80 |= 0x6000;
        }
        if (t >= 0x1CD && t < 0x1E0) {
            D_800E6280.unk_F80 |= 0x9000;
        }
        func_8013A790(1, 1);
        func_8013F250();
        func_80132FE8();
    }
    if ((u32) D_800E6280.unk_1100 >= 0x709U) {
        func_80143AF4();
        func_801347A8();
        func_80059048();
        func_8015ABF0();
        func_800591D8(0);
        func_80042908(2);
    }
}
