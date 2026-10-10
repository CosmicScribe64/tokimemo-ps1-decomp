#include "common.h"
#include "ovl/TAIIKU.h"

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_80138950);

typedef struct {
    void (*f[9])();
} FnTbl9; /* size 0x24 */
extern FnTbl9 D_80149E08;

void func_80138A34(void) {
    s32 idx;
    s32 pad; /* FAKE: second word above tbl (T-3330 two-word case); real source unknown. T-4030 */
    FnTbl9 tbl;

    tbl = D_80149E08;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_80138AA8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_80138DF8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_80139330);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_80139550);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_80139610);

void func_80139A20(void) {
    if ((func_8008667C(6) < 2) && (((u32) (D_800E6280.unk_1BC[6].unk_0C.w << 0x1E) >> 0x1F) == 1) && (D_80149940 == 6) && (D_800E6280.unk_56C[45] == 0)) {
        D_800E6280.unk_56C[45] = 1;
        func_80042808();
        return;
    }
    D_800E6280.unk_1109 = 0;
    D_800E6280.unk_110A = 0;
    D_80122ECC = 1;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_80139AB0);

void func_80139DDC(void) {
    func_8004E44C(0, D_8014A0BC, D_8014A0B0);
    srn_init(0, D_8014A0B8, 0);
    srn_vram_set(0, 5, 0x580, 0xF0);
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_80139E34);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_80139FE4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013A314);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013A63C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013A6FC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013ACBC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013AE14);

void func_8013AEFC(void) {
    if (D_80120676 > 0xA0) {
        D_80120676 = 0xA0;
    }
    if (D_80120676 < -0xA0) {
        D_80120676 = -0xA0;
    }
    if (D_8012067A > 0x30) {
        D_8012067A = 0x30;
    }
    if (D_8012067A < -0x18) {
        D_8012067A = -0x18;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013AF74);

void func_8013B0AC(void) {
    if (D_80149990 == 0) {
        func_8013B104();
    } else {
        func_8013C978();
    }
    if ((u16)D_801499B0[0].unk0 == 0) {
        func_8013C780();
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013B104);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013B200);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013B3B0);

void func_8013B5C0(void) {
    if (D_801499A2 == 1) {
        func_80044750(0x800);
        return;
    }
    if ((u32)(u16)D_801499A2 >= 0x2D1) {
        D_801499A2 = 0;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013B608);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013B78C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013B94C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013BB3C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013BCEC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013BEF8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013C0E4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013C33C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013C4E4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013C5C8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013C694);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013C780);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013C978);

void func_8013CCFC(void) {
    func_8013CD34();
    func_8013CF88();
    func_8013D114();
    func_8013D2B4();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013CD34);

s32 func_8013CEF8(void) {
    s32 dx;
    s32 temp_v0;

    dx = func_800AE0C0(((s16) D_80149984 >> 3) - D_80120676);
    temp_v0 = func_800AE0C0(((s16) D_80149986 >> 3) - D_8012067A);
    if (D_80149944 != 1) {
        return 0;
    }
    if ((0x10 - (dx >> 2)) >= (temp_v0 - 0x10)) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013CF88);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013D114);

void func_8013D254(void) {
    func_80044750(0x504);
    D_80149988 = 0;
    D_8014999C = 0;
    D_801499B0[0].unk0 = 0;
    D_80149974 = 0;
    D_8014994C = 0;
    D_80149950 = 0;
    D_801207A7 |= 0x80;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013D2B4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013D788);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013D9B4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013DDB8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013E1FC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013E894);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013EC60);

void func_8013F35C(void) {
    s32 var_s0;

    var_s0 = 0;
    do {
        func_8013F3A4(var_s0);
        var_s0 += 1;
    } while (var_s0 != 4);
    func_8013F948();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013F3A4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013F948);

s32 func_8013FB14(s32 arg0) {
    s16 temp_v0;

    temp_v0 = D_801499B0[arg0].val;
    if (temp_v0 < 0x400) {
        return 0;
    }
    if (temp_v0 < 0x500) {
        return 1;
    }
    if (temp_v0 < 0x600) {
        return 2;
    }
    if (temp_v0 < 0xA00) {
        return 3;
    }
    if (temp_v0 < 0xB00) {
        return 4;
    }
    if (temp_v0 < 0xC00) {
        return 5;
    }
    return 6;
}

s16 func_8013FBA8(s32 arg0) {
    s16 temp_v1;

    temp_v1 = D_801499B0[arg0].val;
    if (temp_v1 < 0x400) {
        return temp_v1;
    }
    if (temp_v1 < 0x500) {
        return (s16) (temp_v1 - 0x200);
    }
    if (temp_v1 < 0x600) {
        return (s16) (temp_v1 - 0x200);
    }
    if (temp_v1 < 0xA00) {
        return (s16) (temp_v1 - 0x600);
    }
    if (temp_v1 < 0xB00) {
        return (s16) (temp_v1 - 0x800);
    }
    if (temp_v1 < 0xC00) {
        return (s16) (temp_v1 - 0x800);
    }
    return (s16) (temp_v1 - 0xC00);
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013FC5C);

s8 func_8013FE44(void) {
    s32 i;
    s16 v[3];

    for (i = 0; i < 3; i++) {
        if (D_801499B4[i].val < 0xE00) {
            v[i] = D_801499B4[i].val;
        } else {
            v[i] = -1;
        }
    }
    if ((v[0] >= v[1]) && (v[0] >= v[2])) {
        D_801499B4[0].val = -1;
        return 1;
    }
    if (v[1] >= v[2]) {
        D_801499B4[1].val = -1;
        return 2;
    }
    D_801499B4[2].val = -1;
    return 3;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013FEF0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_801400D8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_80140338);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8014038C);

void func_80140414(void) {
    if ((u32)D_801499D8 >= 0xF) {
        *(s16 *)(D_8011ECD0 + 0x19DC) ^= 1;
        *(s16 *)(D_8011ECD0 + 0x1A20) ^= 1;
        D_801499D8 = 0;
    }
}

void func_8014045C(void) {
    func_8009AD30(0x3E8);
    D_80122740 = 0;
    D_80122744 = 0;
    D_80122748 = -0x3E8;
    D_8012274C = 0;
    D_80122750 = 0;
    D_80122754 = 0;
    D_80122758 = 0;
    D_8012275C = 0;
    func_80099540(&D_80122740);
    func_8009AD50(0);
    func_8009AD60(0x10000);
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_801404DC);

void func_8014064C(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        func_80099E30(0, &D_801227A0[i]);
        D_801227A0[i].unk20 = -4000;
        D_801227A0[i].unk0 = 0;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_801406A8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_80140738);

void func_801407C0(void) {
    D_80149FA0 = 0x8019D000;
    D_80149FA4 = 0x8018F000;
    D_80149FA8 = 0x8019E218;
    D_80149FAC = 0x8019D008;
}
