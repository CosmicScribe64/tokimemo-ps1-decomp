#include "common.h"
#include "ovl/TAIIKU.h"
extern u8 D_80149208[]; /* 3 slots of 0x24 bytes; byte at +0x2A is read */
extern u8 D_80149232;
extern u8 D_80149256;
extern u8 D_8014927A;

typedef struct {
    void (*f[10])();
} FnTbl10; /* size 0x28 */
extern FnTbl10 D_8014933C;

void func_80133C80(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    s32 pad; /* FAKE: unused local; the original frame has a second word above tbl (two-word case, T-3330), real source unknown. T-4080 */
    FnTbl10 tbl;

    tbl = D_8014933C;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80133CFC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80134060);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_801345A4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_801346FC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80134D94);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80134F98);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80135480);

void func_801356E0(void) {
    s32 i;

    func_8004E44C(0, D_80149FAC, D_80149FA0);
    srn_init(0, D_80149FA8, 0);
    srn_vram_set(0, 5, 0x580, 0xF0);
    for (i = 0; i < 3; i++) {
        D_800E6280.unk_1228[i + 5] = 8;
    }
    for (i = 0; i < 3; i++) {
        D_800E6280.unk_1228[i + 0x25] = 8;
    }
}

void func_80135778(void) {
    func_80048F64(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120657 = 0x80;
    D_8012065C = (u8 *) D_80149FB0;
    D_80120660 = (u8 *) D_80149FE8;
    D_80120684 = &D_801492C4;
    D_80120664 = D_8014A020;
    D_80120654 = 0;
    D_80120655 = 0x80;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80135830);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80135AD0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80135D24);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80135DF8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80135F3C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_801365C8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_801368B8);

void func_801369F0(void) {
    func_80136A28();
    func_80136AB0();
    func_80136B18();
    func_80136BB4();
}

void func_80136A28(void) {
    if (D_800E6280.unk_F88 & 0x20) {
        D_80149204 = 1;
    }
    if (D_8014921C >= 0x10U) {
        if (D_80149204 != 0) {
            if (D_80149220 < 0) {
                D_80149220 = 0;
            }
            D_80149218 = D_801491E4;
        } else {
            D_80149218 = -0x300;
        }
        D_8014921C = 0;
        D_80149204 = 0;
    }
}

void func_80136AB0(void) {
    if ((u32)D_80149224 >= 0xB) {
        D_80149220 += D_80149218;
        if (D_80149220 < -0x10000) {
            D_80149220 = -0x10000;
        }
        if (D_801491E8 < D_80149220) {
            D_80149220 = D_801491E8;
        }
        D_80149224 = 0;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80136B18);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80136BB4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80136DA4);

void func_80136E80(void) {
    func_80136EC0();
    func_8013703C();
    func_80137090();
    func_80137228();
    func_8013732C();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80136EC0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_8013703C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80137090);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80137228);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_8013732C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80137440);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80137544);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80137784);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80137C44);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80137EA8);

/* FAKE: v[6] instead of three elements reproduces the original local offset
 * (0x1c); the real source is unknown. T-0016 */
s32 func_8013815C(void) {
    s16 v[6];
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_80149208[i * 0x24 + 0x2A] == 0) {
            v[i] = *(s16 *)(D_8011ECD0 + i * 0x44 + 0x19EA);
        } else {
            v[i] = -0xF00;
        }
    }
    if (v[0] >= v[1] && v[0] >= v[2]) {
        D_80149232 = 1;
        return 2;
    }
    if (v[1] >= v[2]) {
        D_80149256 = 1;
        return 3;
    }
    D_8014927A = 1;
    return 4;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80138224);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80138680);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80138800);

s16 func_801388E4(s32 arg0) {
    s32 key;
    s32 slot;

    key = arg0 + 1;
    if (key == D_801491E0[0]) {
        slot = 0;
    }
    if (key == D_801491E0[1]) {
        slot = 1;
    }
    if (key == D_801491E0[2]) {
        slot = 2;
    }
    if (key == D_801491E0[3]) {
        slot = 3;
    }
    return (slot << 6) + 0xA0;
}

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148A90);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148A98);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AA0);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AA8);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AB0);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AB8);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AC0);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AC8);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AD0);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AD8);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148ADC);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AE4);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AEC);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AF0);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AF8);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148B00);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148B08);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148B10);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148B18);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148B1C);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148B24);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148B2C);
