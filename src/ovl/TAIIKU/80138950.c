#define MAIN_API_OVERRIDE_cd_read_sectors /* arg1 is a RAM address: matched as u32 (main_api.h: s32), T-16552 */
#define MAIN_API_OVERRIDE_func_80085E30 /* called as an implicit int function (definition: void), T-16552 */
#include "common.h"
#include "ovl/TAIIKU.h"
#include "psx_types.h"
void cd_read_sectors(u8 arg0, u32 arg1, s32 arg2);
s32 func_80085E30();
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149940);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149944);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149948);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_8014994C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149950);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149954);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149958);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_8014995C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149960);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149964);
TaiikuBall D_80149968 = { 0, 0, 0, 0, 0, 0, 0, 0, { 0, 0 } };
TaiikuSel D_80149988 = { 0, 0, 0 };
Taiiku3s32 D_80149990 = { 0, 0, 0 };
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_8014999C);
TaiikuA0 D_801499A0 = { 0, 0, 0, 0, 0, 0 };
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_801499B0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_801499B4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_801499C0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_801499C4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_801499CC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_801499D4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_801499D8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_801499DC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_801499E0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_801499E4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149A84);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149B24);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149BC4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149C64);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149D04);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149DA4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149DAC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149DF4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149E08);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149E2C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149E34);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149E7C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149E90);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149EBC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149EE8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149F00);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149F20);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149F40);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149F50);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149F68);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149F80);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80138950.data", D_80149F8C);

void func_80138950(void) {
    TaiikuTbl11 a;
    TaiikuTbl11 b;

    a = D_801498E8;
    b = D_80149914;
    func_80046290(a.v[D_801491DC], b.v[D_801491DC], D_801491DC);
    func_80044750(0x300);
}

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

/* func_80085E30 is called as an int function here (see the override at the top): its unused $v0 result keeps
 * the loop pointer of case 0 out of $v0, and the constant 8 takes it, as in the original (with void the pointer,
 * priority 41, is coloured first and takes $v0). The call result goes through idx: A[f()] = v with the call in
 * the index reserves a second spill slot (see func_80138DF8). T-16552. */
void func_80138AA8(void) {
    s32 p; /* FAKE: T-16552, case 2 buffer address; its only job is the third scalar frame slot, real local unknown */
    s32 i;
    s32 idx;
    TaiikuW2 tpage;
    TaiikuW18 tbl;
    TaiikuW5 rgb;

    tpage = D_80149E2C;
    tbl = D_80149E34;
    rgb = D_80149E7C;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80085E30(1, 1);
        for (i = 0; i < 3; i++) {
            D_800E6280.unk_1228[i + 5] = 8;
        }
        for (i = 0; i < 3; i++) {
            D_800E6280.unk_1228[i + 0x25] = 8;
        }
        D_800E6280.unk_1228[28] = 8;
        func_80048EB8(0);
        func_8006BD6C(0);
        for (i = 0; i < 2; i++) {
            D_80149DA4[i] = tpage.v[i];
        }
        for (i = 0; i < 18; i++) {
            D_80149DAC[i] = tbl.v[i];
        }
        D_80149DF4[0] = rgb.v[0];
        for (i = 1; i < 5; i++) {
            D_80149DF4[i] = rgb.v[i];
        }
        func_8004284C();
        break;
    case 1:
        cd_read_sectors(0x1E, 0x80180000, 0xBAD0);
        Fade_Out_Init();
        func_8004284C();
        break;
    case 2:
        Fade_Out_Main();
        if (cd_read_sync_status() & 1) {
            p = 0x8018F000;
            cd_read_sectors(0x27, p, 0xBAEE);
            func_8004284C();
        }
        break;
    case 3:
        if (Fade_Out_Main() != 0 && (cd_read_sync_status() & 1)) {
            func_80140810();
            func_80141BB0();
            for (i = 0; i < 2; i++) {
                idx = search_load_tpage_buf_lock(D_80149DA4[i], 1);
                D_800E6280.unk_1228[idx] = i + 0x80000300;
                D_80149DA4[i] = i + 0x80000300;
            }
            for (i = 0; i < 16; i++) {
                load_palette(D_8014A0B4, (u8) (i + 0x10), 1, 1, i);
            }
            cd_read_sectors(0xB9, 0x8018F000, 0xBA17);
            func_80139AB0();
            func_80139DDC();
            func_80139E34();
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}

/* The A[f()] = v stores go through idx: with the call inside the index IDO reserves a second, unused spill slot
 * and the tables sit 4 bytes high (T-16552). cd_read_sectors takes its address as u32 here (see the override at
 * the top): with s32 IDO builds 0x801F8000 of case 0 through a hoisted 0x801F0000 in $s0. p is the buffer address
 * of case 1. */
void func_80138DF8(void) {
    s32 p;
    s32 i;
    s32 idx;
    RECT rect;
    TaiWords11 tblA;
    TaiWords11 tblB;

    tblA = D_80149E90;
    tblB = D_80149EBC;
    switch (D_800E6280.unk_110A) {
    case 0:
        /* FAKE: T-16552. i = 0 between the call and the test puts `move s0,zero` in the beqz delay slot; the goto
         * enters the loop without an entry test (with one, uopt leaves the pointer start as `sll t9,zero,2`) and
         * keeps the increments on the for line, where as1 schedules them before the stores. Real source unknown. */
        idx = cd_read_sync_status();
        i = 0;
        if (idx & 1) {
            goto first;
            for (; i < 18; i++) {
            first:
                idx = search_load_tpage_buf_lock(D_80149DAC[i], 1);
                D_800E6280.unk_1228[idx] = i + 0x80000100;
                D_80149DAC[i] = i + 0x80000100;
            }
            rect.x = 0x300;
            rect.y = 0x80;
            rect.w = 0x40;
            rect.h = 0x80;
            func_8009C884(&rect, (void *)0x801BB000);
            D_800E6280.unk_1228[44] = 8;
            for (i = 0; i < 5; i++) {
                idx = search_load_tpage_buf_lock(D_80149DF4[i], 1);
                D_800E6280.unk_1228[idx] = i + 0x80000200;
                D_80149DF4[i] = i + 0x80000200;
            }
            cd_read_sectors(3, 0x801F8000, 0xBE2A);
            func_80086350();
            func_80044750(0x200);
            func_8004284C();
        }
        break;
    case 1:
        if (func_80086370() != 0) {
            if (cd_read_sync_status() & 1) {
                func_800438DC(1, 1);
                func_8014045C();
                func_801404DC();
                func_8014064C();
                p = 0x801F8000;
                func_801406A8(p);
                func_80140738(p, 0);
                func_8004284C();
            }
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        break;
    case 2:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        func_80139FE4();
        func_800863D4(0x6B);
        if (D_800E6280.unk_F88 & 0xF0) {
            for (i = 0; i < 11; i++) {
                if (((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 <= ((DisplayActor44 *)D_8011ECD0)[107].unk_26 + 8 &&
                    ((DisplayActor44 *)D_8011ECD0)[107].unk_26 + 8 <= ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 + 0x20 &&
                    ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_2A <= ((DisplayActor44 *)D_8011ECD0)[107].unk_2A + 0x10 &&
                    ((DisplayActor44 *)D_8011ECD0)[107].unk_2A + 0x10 <= ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_2A + 0x40 &&
                    ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_03 != 0) {
                    D_80149940 = i;
                    Fade_Out_Init();
                    func_80044750(0x74);
                    func_8004284C();
                }
            }
        }
        break;
    case 3:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if (func_800460EC() == 2) {
            cd_read_sectors(0x87, 0x8018F000, 0xB98F);
            func_8004284C();
        }
        break;
    case 4:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if (Fade_Out_Main() != 0) {
            func_8004284C();
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_80133C30();
            func_8004E44C(0, (u32 *)D_801491C4, D_801491C0);
            srn_init(0, D_801491C8, 0);
            srn_vram_set(0, 5, 0x140, 0xF0);
            func_801331D0();
            for (i = 0; i < 6; i++) {
                search_load_tpage_buf_lock(D_8014A070[i], 1);
            }
            func_8013A314();
            D_80122EA0 = 0;
            func_80044890(0, 0xBF98, 0xBF79, tblA.value[D_80149940], tblB.value[D_80149940], 0xD6B7);
            func_80086350();
            func_8004284C();
        }
        break;
    case 6:
        func_800867CC();
        if (func_80086370() != 0) {
            func_80085ED0();
            func_80042808();
        }
        func_8013DDB8();
        func_8013D788();
        func_8013D2B4();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80139330(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_800867CC();
        if (D_80122EA0 != 0) {
            func_80044750(0x2E);
            D_80149990.unk_00 = 0;
            func_8004284C();
        }
        func_8013DDB8();
        func_8013D788();
        func_8013D2B4();
        return;
    case 1:
        func_80044750(0x24);
        func_8013DDB8();
        func_8013D788();
        func_8013D2B4();
        func_8004284C();
        return;
    case 2:
        func_80044750(0x23);
        func_8013DDB8();
        func_8013D788();
        func_8013D2B4();
        func_8004284C();
        return;
    case 3:
        func_80044750(0x502);
        func_8013DDB8();
        func_8013D788();
        func_8013D2B4();
        func_80085ED0();
        func_8004284C();
        return;
    case 4:
        func_8013A63C();
        func_8013CCFC();
        func_8013A6FC();
        func_8013B0AC();
        if (D_8012067A >= D_801206BE) {
            D_80120651 = 2;
            D_80120695 = 3;
        } else {
            D_80120651 = 3;
            D_80120695 = 2;
        }
        ((DisplayActor44 *)D_8011ECD0)[98].unk_26 = ((DisplayActor44 *)D_8011ECD0)[96].unk_26;
        ((DisplayActor44 *)D_8011ECD0)[99].unk_26 = ((DisplayActor44 *)D_8011ECD0)[97].unk_26;
        D_80120702 = D_8012067A;
        D_80120746 = D_801206BE;
        D_80120852 = (s16) ((s16) D_80149968.unk_1C[0] >> 3);
        D_80120856 = ((s16) D_80149968.unk_1C[1] >> 3) + 0x20;
        func_8013DDB8();
        func_8013D788();
        func_8013F35C();
        func_8013FC5C();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80139550(void) {
    s32 i;

    func_80044750(0x504);
    i = 0;
    while (D_801499D4[i] != 0xFF) {
        i++;
    }
    D_801499D4[i] = 0;
    for (i++; i < 4; i++) {
        D_801499D4[i] = func_8013FE44();
    }
    func_8013DDB8();
    func_8013D788();
    D_80149954 = 0x1200;
    D_8012082F = 0;
    func_80042808();
}

void func_80139610(void) {
    s32 pad; /* FAKE: T-11640, unused local; its slot puts the spills of the case 3 loop where the original has them */
    s32 i;

    switch (D_800E6280.unk_110A) {
    case 0:
        if (D_80149954 < 0x80) {
            func_80048E78();
            D_80149954 = 0;
            Fade_Out_Init();
            D_800E6280.unk_03A = 0;
            func_8004284C();
        }
        func_8013FEF0();
        func_8013DDB8();
        func_8013D788();
        break;
    case 1:
        func_801400D8();
        func_80086350();
        func_80133258();
        func_8004284C();
        break;
    case 2:
        if (func_80086370() != 0) {
            if (D_801499D4[0] == 0 || D_801499D4[1] == 0) {
                func_80044750(0x205);
            } else {
                func_80044750(0x206);
            }
            func_80085ED0();
            func_8004284C();
        }
        Scroll(0, 0xF, 0, 0xF0, 0xC0);
        break;
    case 3:
        if (func_80085EEC() >= 0x5B) {
            for (i = 0; i < 4; i++) {
                if (D_801499D4[i] == 0) {
                    func_8013384C(D_80149940, (u8) i);
                }
            }
            func_80140338();
            func_8014038C();
            D_801499D8 = 0;
            func_80085ED0();
            func_8004284C();
        }
        Scroll(0, 0xF, 0, 0xF0, 0xC0);
        break;
    case 4:
        D_801499D8++;
        func_80140414();
        if (func_80085EEC() >= 0xB5) {
            func_80085ED0();
            func_8004284C();
        }
        Scroll(0, 0xF, 0, 0xF0, 0xC0);
        break;
    case 5:
        if (func_80085EEC() >= 0xF1) {
            Fade_Out_Init();
            func_8004284C();
        }
        Scroll(0, 0xF, 0, 0xF0, 0xC0);
        break;
    case 6:
        if (Fade_Out_Main() != 0) {
            if (D_801499D4[0] == 0) {
                for (i = 0; i < 13; i++) {
                    D_800E6280.unk_1BC[i].unk_02 += 4;
                }
                D_800E6280.unk_1BC[D_80149940].unk_02 += 2;
                D_800E6280.unk_1BC[D_80149940].unk_06 += 10;
            } else if (D_801499D4[1] == 0) {
                for (i = 0; i < 13; i++) {
                    D_800E6280.unk_1BC[i].unk_02 += 2;
                }
                D_800E6280.unk_1BC[D_80149940].unk_02 += 1;
                D_800E6280.unk_1BC[D_80149940].unk_06 += 5;
            }
            func_80042808();
        }
        Scroll(0, 0xF, 0, 0xF0, 0xC0);
        break;
    default:
        func_80046500();
        break;
    }
}

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

typedef struct {
    u32 low : 1;
    u32 f : 1;
    u32 rest : 30;
} TaiikuBit1;

void func_80139E34(void) {
    s32 i;
    s32 j;
    DisplayActor44 *p;
    TaiikuS11 tbl;

    tbl = D_80149EE8;
    j = 0;
    p = (DisplayActor44 *)D_8011ECD0;
    for (i = 0; i < 12; i++) {
        func_80048F64(i + 0x60);
        p[96].unk_38 = 0x01000000;
        p[96].unk_07 = 0x80;
        p[96].unk_0C = (u8 *)D_80149FE0;
        p[96].unk_10 = (u8 *)D_8014A018;
        p[96].unk_34 = (u8 *)D_80149DA4;
        p[96].unk_14 = D_8014A050;
        p[96].unk_05 = 0x80;
        p[96].unk_1A = 0x1000;
        p[96].unk_1C = 0x1000;
        p[96].unk_06 = 2;
        p[96].unk_02 = 1;
        if (i == 11) {
            p[96].unk_26 = 0;
            p[96].unk_2A = 0;
            p[96].unk_01 = 2;
            p[96].unk_03 = 0xA4;
        } else {
            if (((TaiikuBit1 *)&D_800E6280.unk_1BC[i].unk_0C)->f == 1) {
                p[96].unk_26 = tbl.v[j];
                p[96].unk_03 = 0xA4;
                j++;
            } else {
                p[96].unk_26 = 0xC8;
                p[96].unk_03 = 0;
            }
            p[96].unk_2A = 0x28;
            p[96].unk_01 = 3;
        }
        p[96].unk_16 = i;
        p++;
    }
    ((DisplayActor44 *)D_8011ECD0)[0].unk_26 = 0;
    ((DisplayActor44 *)D_8011ECD0)[0].unk_2A = 0;
}

/* FAKE: T-16552. This leaf function is coloured in bit order and the bits follow the first mention in the
 * ucode: the loop pointer (D_800E6280 + i * 0x38) is numbered in the first loop and so takes a lower register than
 * the record address and the unk_F80 copy, while the original has cnt $a0, record $a1, flags $a2, pointer $a3.
 * The two dead assignments at the top of the first loop (m = a field of record 96 + i, cnt = unk_F80) are
 * overwritten before any use and generate no code; they number the record address and unk_F80 first. Mentions
 * in code that cfe drops (if (0), unused locals, an address without a load, two adjacent assignments to one
 * local) do not renumber. m also reproduces the fourth scalar slot (sp1C at sp+0x1C), real local unknown. */
void func_80139FE4(void) {
    s32 n;
    s32 i;
    s32 cnt;
    s32 m;
    s32 sp1C[11];

    n = 0;
    for (i = 0, cnt = 0; i < 0xB; i++) {
        m = ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26;
        cnt = D_800E6280.unk_F80;
        if (((TaiikuBit1 *)&D_800E6280.unk_1BC[i].unk_0C)->f == 1) {
            sp1C[n] = i;
            n++;
        }
    }
    cnt = 0;
    m = n;
    for (i = 0; i < 0xB; i++) {
        if (((TaiikuBit1 *)&D_800E6280.unk_1BC[i].unk_0C)->f == 1) {
            if (D_800E6280.unk_F80 & 4) {
                ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 = ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 - 4;
            } else {
                ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 = ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 - 1;
            }
            if (((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 < -0xC0) {
                ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 = ((DisplayActor44 *)D_8011ECD0)[96 + sp1C[(cnt + n - 1) % m]].unk_26 + 0x78;
                if (((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 < 0xA0) {
                    ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 = 0xA0;
                }
            }
            cnt++;
        }
    }
}

void func_8013A314(void) {
    s32 i;
    TaiikuH16 x;
    TaiikuH16 y;
    TaiikuB16 frame;

    x = D_80149F00;
    y = D_80149F20;
    frame = D_80149F40;
    func_80048E78();
    for (i = 0; i < 16; i++) {
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_38 = 0x1000000;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_07 = 0x80;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_05 = 0x80;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_0C = (u8 *)D_80149FDC;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_10 = (u8 *)D_8014A014;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_34 = (u8 *)D_80149DAC;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_14 = D_8014A04C;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_1A = 0x1000;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_1C = 0x1000;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_02 = 0x40;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_03 = 0x84;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_01 = frame.v[i] + 0xF0;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_26 = x.v[i];
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_2A = y.v[i];
    }
    ((TaiikuActor44 *)D_8011ECD0)[96].unk_18 = 0;
    ((TaiikuActor44 *)D_8011ECD0)[96].unk_01 = 3;
    ((TaiikuActor44 *)D_8011ECD0)[97].unk_18 = D_80149940 * 6 + 6;
    ((TaiikuActor44 *)D_8011ECD0)[97].unk_01 = 3;
    ((TaiikuActor44 *)D_8011ECD0)[98].unk_18 = 0x82;
    ((TaiikuActor44 *)D_8011ECD0)[98].unk_07 = 0x68;
    ((TaiikuActor44 *)D_8011ECD0)[98].unk_38 |= 0x40000000;
    ((TaiikuActor44 *)D_8011ECD0)[99].unk_18 = 0x83;
    ((TaiikuActor44 *)D_8011ECD0)[99].unk_07 = 0x68;
    ((TaiikuActor44 *)D_8011ECD0)[99].unk_38 |= 0x40000000;
    ((TaiikuActor44 *)D_8011ECD0)[100].unk_18 = 0x94;
    ((TaiikuActor44 *)D_8011ECD0)[101].unk_01 = 1;
    ((TaiikuActor44 *)D_8011ECD0)[101].unk_03 = 4;
    ((TaiikuActor44 *)D_8011ECD0)[101].unk_18 = 0x84;
    ((TaiikuActor44 *)D_8011ECD0)[102].unk_18 = 0x85;
    ((TaiikuActor44 *)D_8011ECD0)[102].unk_03 = 4;
    ((TaiikuActor44 *)D_8011ECD0)[103].unk_18 = 0x81;
    ((TaiikuActor44 *)D_8011ECD0)[103].unk_07 = 0x68;
    ((TaiikuActor44 *)D_8011ECD0)[103].unk_38 |= 0x40000000;
    ((TaiikuActor44 *)D_8011ECD0)[104].unk_18 = 0x8C;
    ((TaiikuActor44 *)D_8011ECD0)[104].unk_03 |= 1;
    for (i = 0; i < 3; i++) {
        ((TaiikuActor44 *)D_8011ECD0)[105 + i].unk_18 = 0x86;
        ((TaiikuActor44 *)D_8011ECD0)[105 + i].unk_03 |= 1;
        ((TaiikuActor44 *)D_8011ECD0)[105 + i].unk_43 = i + 0x1C;
    }
    ((TaiikuActor44 *)D_8011ECD0)[108].unk_18 = 0x92;
    for (i = 0; i < 3; i++) {
        ((TaiikuActor44 *)D_8011ECD0)[109 + i].unk_18 = 0x93;
        ((TaiikuActor44 *)D_8011ECD0)[109 + i].unk_43 = i + 0x1C;
    }
}

void func_8013A63C(void) {
    if (Vblnk_Timer() >= 2) {
        D_801499A0.unk_02 += 1;
        Vblnk_Timer_Init();
    }
    D_801499DC += 1;
    D_80149968.unk_08 += 1;
    D_80149968.unk_0A += 1;
    D_801499E0 += 1;
    D_80149988.unk_04 += 1;
    D_80149988.unk_06 += 1;
    D_801499A0.unk_00 += 1;
}

void func_8013A6FC(void) {
    s16 next_x;
    s32 limit_y1;
    s32 limit_y2;
    s32 limit_y3;
    s32 limit_y4;

    switch (D_80149944) {
    case 0:
        func_800863D4(0x60);
        break;
    case 1:
        func_80133928(0x60);
        func_8013AEFC();
        limit_y1 = ((s16) D_80149968.unk_1C[1] >> 3) + 0x10;
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_2A < limit_y1) {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = limit_y1;
        }
        break;
    case 2:
        func_80133928(0x60);
        func_8013AEFC();
        if ((((s16) D_80149968.unk_1C[0] >> 3) + 0x50) >= ((DisplayActor44 *)D_8011ECD0)[96].unk_26) {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = ((s16) D_80149968.unk_1C[0] >> 3) + 0x51;
        }
        break;
    case 3:
        func_80133928(0x60);
        func_8013AEFC();
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_26 >= (((s16) D_80149968.unk_1C[0] >> 3) - 0x50)) {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = ((s16) D_80149968.unk_1C[0] >> 3) - 0x51;
        }
        break;
    case 5:
        func_80133928(0x60);
        func_8013AEFC();
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_26 >= (((s16) D_80149968.unk_1C[0] >> 3) - 0x50)) {
            limit_y2 = ((s16) D_80149968.unk_1C[1] >> 3) + 0x10;
            if (((DisplayActor44 *)D_8011ECD0)[96].unk_2A < limit_y2) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = ((s16) D_80149968.unk_1C[0] >> 3) - 0x51;
                ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = limit_y2;
            }
        }
        break;
    case 6:
        func_80133928(0x60);
        func_8013AEFC();
        if ((((s16) D_80149968.unk_1C[0] >> 3) + 0x50) >= ((DisplayActor44 *)D_8011ECD0)[96].unk_26) {
            limit_y3 = ((s16) D_80149968.unk_1C[1] >> 3) + 0x10;
            if (((DisplayActor44 *)D_8011ECD0)[96].unk_2A < limit_y3) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = ((s16) D_80149968.unk_1C[0] >> 3) + 0x51;
                ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = limit_y3;
            }
        }
        break;
    case 7:
        switch (D_8014994C) {
        case 0:
            limit_y4 = ((s16) D_80149968.unk_1C[1] >> 3) + 0x10;
            if (((DisplayActor44 *)D_8011ECD0)[96].unk_2A < limit_y4) {
                if ((((DisplayActor44 *)D_8011ECD0)[96].unk_26 < (((s16) D_80149968.unk_1C[0] >> 3) + 0x50)) && ((((s16) D_80149968.unk_1C[0] >> 3) - 0x50) < ((DisplayActor44 *)D_8011ECD0)[96].unk_26)) {
                    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = limit_y4;
                }
            }
            next_x = ((DisplayActor44 *)D_8011ECD0)[101].unk_26 - 8;
            ((DisplayActor44 *)D_8011ECD0)[101].unk_26 = next_x;
            if ((next_x < -0x1C0) && (D_80149960 == 0)) {
                D_80149988.unk_00 = 0;
                ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0xA4;
                D_8014994C += 1;
            }
            break;
        case 1:
            D_8014994C += 1;
            break;
        case 2:
            D_8014994C += 1;
            break;
        case 3:
            if (D_80149990.unk_00 == 1) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = 0x90;
            } else {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0x90;
                ((DisplayActor44 *)D_8011ECD0)[96].unk_03 |= 1;
            }
            ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = 0x10;
            D_8014994C += 1;
            break;
        case 4:
            if (D_80149950 == 4) {
                if (D_80149990.unk_00 == 1) {
                    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = ((s16) D_80149968.unk_1C[0] >> 3) + 0x60;
                    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = ((s16) D_80149968.unk_1C[1] >> 3) - 0x10;
                    if (D_80149968.unk_14 <= 0) {
                        D_80149988.unk_00 = 0;
                        D_80149968.unk_0C = 0;
                        D_8014994C += 1;
                    }
                } else {
                    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = ((s16) D_80149968.unk_1C[0] >> 3) - 0x60;
                    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = ((s16) D_80149968.unk_1C[1] >> 3) - 0x10;
                    if (D_80149968.unk_14 >= 0) {
                        D_80149988.unk_00 = 0;
                        ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0xA4;
                        D_80149968.unk_0C = 0;
                        ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0x50;
                        D_8014994C += 1;
                    }
                }
            }
            break;
        case 5:
            if (((DisplayActor44 *)D_8011ECD0)[96].unk_2A < 0x30) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_2A += 2;
            } else {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = 0x30;
                if (D_80149990.unk_00 == 2) {
                    ((DisplayActor44 *)D_8011ECD0)[96].unk_03 |= 1;
                }
                D_80149988.unk_00 = 0;
                D_8014994C += 1;
            }
            break;
        case 6:
            if (D_80149990.unk_00 == 1) {
                if (((DisplayActor44 *)D_8011ECD0)[96].unk_26 >= 0x21) {
                    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 -= 2;
                } else {
                    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = 0x20;
                    D_8014994C += 1;
                }
            } else if (((DisplayActor44 *)D_8011ECD0)[96].unk_26 < 0x20) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_26 += 2;
            } else {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = 0x20;
                D_8014994C += 1;
            }
            break;
        case 7:
            if (D_80149950 >= 7) {
                func_8013ACBC();
            }
            break;
        default:
            func_80046500();
            break;
        }
        break;
    default:
        func_80046500();
        break;
    }
    D_80149944 = func_8013AF74();
    if (D_800E6280.unk_F88 & 0x20) {
        D_80149948 = 1;
    }
    if ((D_80149948 != 0) || (D_800E6280.unk_F80 & 0xF000) || (D_80149990.unk_00 != 0)) {
        func_8013AE14();
    }
}

void func_8013ACBC(void) {
    D_80149990.unk_00 = 0;
    D_80149988.unk_00 = 0;
    D_80149990.unk_04 = 0;
    D_80149990.unk_08 = 0;
    D_8014999C = 0;
    D_801499A0.unk_02 = 0;
    D_801499A0.unk_08 = 0;
    D_801499A0.unk_0C = 0;
    D_801499B0[0].val = 0;
    D_80149968.unk_14 = 0;
    ((s16 *)&D_80149968.unk_18)[0] = 0;
    ((s16 *)&D_80149968.unk_18)[1] = 0x80;
    D_80149968.unk_00 = 0;
    D_80149968.unk_04 = 0;
    D_80149968.unk_0C = 0;
    D_80149968.unk_10 = 0;
    D_8014995A = 0;
    D_80149944 = 1;
    D_80120674 = 0;
    D_80120676 = 0x20;
    D_80120678 = 0;
    D_8012067A = 0x30;
    D_801206B8 = 0;
    if (((D_80122EC4 % 3) == 0) || (D_80149940 == 2)) {
        D_801206BA = -0x20;
    } else {
        D_801206BA = 0;
    }
    D_801206BC = 0;
    D_801206BE = 0x30;
    D_80120668 = 0;
    D_801206AC = (D_80149940 * 6) + 6;
    D_80120653 = 0xA4;
    D_80120697 = 0xA4;
    D_801207CA = 0xA0;
}

void func_8013AE14(void) {
    if (D_80149988.unk_04 >= 0xB) {
        if (D_80149988.unk_00 >= 3 && D_80149990.unk_00 != 0 && (D_8014994C == 2 || D_8014994C == 4 || D_8014994C == 6)) {
            D_80149988.unk_00 = 0;
        } else if (D_80149988.unk_00 >= 5) {
            D_80149988.unk_00 = 0;
        } else {
            D_80149988.unk_00 = D_80149988.unk_00 + 1;
        }
        D_80149948 = 0;
        D_80149988.unk_04 = 0;
    }
    D_80120668 = D_80149988.unk_00;
    if (D_80149990.unk_00 != 0) {
        if (D_8014994C == 2 || D_8014994C == 4 || D_8014994C == 6) {
            D_80120668 += 0x48;
        }
    }
}

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

s32 func_8013AF74(void) {
    s32 v;

    if (D_80149990.unk_00 != 0) {
        return 7;
    }
    if (D_8012067A < 0x31 && D_8012067A >= ((s16)D_80149968.unk_1C[1] >> 3) + 0x10 && ((s16)D_80149968.unk_1C[0] >> 3) + 0x50 >= D_80120676 && D_80120676 >= ((s16)D_80149968.unk_1C[0] >> 3) - 0x50) {
        return 1;
    }
    v = ((s16)D_80149968.unk_1C[1] >> 3) + 0x10;
    if (D_8012067A < v && ((s16)D_80149968.unk_1C[0] >> 3) + 0x50 < D_80120676) {
        return 2;
    }
    if (D_8012067A < v && D_80120676 < ((s16)D_80149968.unk_1C[0] >> 3) - 0x50) {
        return 3;
    }
    if (D_8012067A >= v && D_80120676 < ((s16)D_80149968.unk_1C[0] >> 3) - 0x50) {
        return 5;
    }
    if (D_8012067A >= v && ((s16)D_80149968.unk_1C[0] >> 3) + 0x50 < D_80120676) {
        return 6;
    }
    return 0;
}

void func_8013B0AC(void) {
    if (D_80149990.unk_00 == 0) {
        func_8013B104();
    } else {
        func_8013C978();
    }
    if ((u16)D_801499B0[0].unk0 == 0) {
        func_8013C780();
    }
}

void func_8013B104(void) {
    switch (D_80149940) {
    case 0:
        func_8013B200();
        break;
    case 1:
        func_8013B3B0();
        break;
    case 2:
        func_8013B5C0();
        break;
    case 3:
        func_8013B608();
        break;
    case 4:
        func_8013B78C();
        break;
    case 5:
        func_8013B94C();
        break;
    case 6:
        func_8013BB3C();
        break;
    case 7:
        func_8013BCEC();
        break;
    case 8:
        func_8013BEF8();
        break;
    case 9:
        func_8013C0E4();
        break;
    case 10:
        func_8013C33C();
        break;
    default:
        func_80046500();
        break;
    }
}

/* FAKE: T-19008. The counter D_801499A0.unk_02 and the flag D_801499B0[0].val are read through u16 pointer
 * views, as in func_8013BEF8 (the plain field gets a CVT copy of the promoted counter; with the volatile
 * D_801499B2 the lui of the flag load takes the delay slot the original gives to lw ra). tmp is a stack
 * word whose halves are written and read through s16 views (the 16.16 position of the sibling functions);
 * the x >> 3 term is written out at each use, a local x gets a range of its own. Real source unknown. */
void func_8013B200(void) {
    s32 tmp;

    func_8013C5C8(0x18000);
    func_8013C694();
    if (!(((u16 *)&D_801499A0)[1] & 4)) {
        func_8013C4E4(0x10000);
    }
    if (((u16 *)&D_801499A0)[1] % 6 == 0) {
        ((s16 *)&tmp)[0] = 0;
        ((s16 *)&tmp)[1] = D_80149968.unk_1C[0] >> 3;
        tmp += D_80149968.unk_0C * 40;
        ((s16 *)&tmp)[1] += (D_80149968.unk_1C[0] >> 3) * 40 / 48;
        if (D_80122EC4 % 3 != 0 && (D_80149968.unk_1C[0] >= 0 || D_80149968.unk_0C > 0)) {
            ((s16 *)&tmp)[1] = (D_80149968.unk_1C[0] >> 3) + 0x20;
        }
        D_801499A0.unk_04 = ((s16 *)&tmp)[1];
        D_801499A0.unk_06 = (D_80149968.unk_1C[1] >> 3) + 0x10;
    }
    if (((u16 *)&D_801499A0)[1] >= 0x23A) {
        ((u16 *)D_801499B0)[1] = 0;
        func_80044750(0x802);
        ((u16 *)&D_801499A0)[1] = 0;
    }
    if (((u16 *)&D_801499A0)[1] != 0 && ((u16 *)&D_801499A0)[1] % 30 == 0) {
        if (((u16 *)D_801499B0)[1] == 0) {
            ((u16 *)D_801499B0)[1] = 1;
            func_80044750(0x800);
        } else {
            ((u16 *)D_801499B0)[1] = 0;
            func_80044750(0x801);
        }
        ((u16 *)&D_801499A0)[1]++;
    }
}

/* FAKE: T-24124. The counter and flag use volatile/u16 views (as in func_8013B608 and func_8013C33C), and
 * tmp/v reproduce the original's stack word and ugen temporary before the 80 / 48 term. */
void func_8013B3B0(void) {
    s32 tmp;
    s16 v;

    if (D_801499A2 == 0x258) {
        func_80044750(0x802);
        ((u16 *)D_801499B0)[0] = 1;
        D_801499A0.unk_08 = 0;
        D_801499A0.unk_0C = 0;
        ((u16 *)D_801499B0)[1] = 0;
        D_801206AC = 0x7F;
    } else if (D_801499A2 >= 0x2D1) {
        ((u16 *)D_801499B0)[0] = 0;
        ((u16 *)&D_801499A0)[1] = 0;
    } else if (((u16 *)&D_801499A0)[1] >= 0x259) {
        if (D_801206BE < 0x30) {
            D_801206BE++;
        }
    } else {
        func_8013C5C8(0x10000);
        func_8013C694();
        if (!(((u16 *)&D_801499A0)[1] & 8)) {
            func_8013C4E4(0x8000);
        }
        if (!(((u16 *)&D_801499A0)[1] & 0xF)) {
            ((s16 *)&tmp)[0] = 0;
            v = D_80149968.unk_1C[0] >> 3;
            v += (D_80149968.unk_1C[0] >> 3) * 80 / 48;
            if (D_80122EC4 % 3 != 0 && (D_80149968.unk_1C[0] >= 0 || D_80149968.unk_0C > 0)) {
                v = (D_80149968.unk_1C[0] >> 3) + 0x20;
            }
            D_801499A0.unk_04 = v;
            D_801499A0.unk_06 = (D_80149968.unk_1C[1] >> 3) + 0x10;
            ((s16 *)&tmp)[1] = v;
        }
    }
    if (((u16 *)D_801499B0)[0] == 0 && ((u16 *)&D_801499A0)[1] != 0 && ((u16 *)&D_801499A0)[1] % 45 == 0) {
        if (((u16 *)D_801499B0)[1] == 0) {
            ((u16 *)D_801499B0)[1] = 1;
            func_80044750(0x800);
        } else {
            ((u16 *)D_801499B0)[1] = 0;
            func_80044750(0x801);
        }
        ((u16 *)&D_801499A0)[1]++;
    }
}

void func_8013B5C0(void) {
    if (D_801499A0.unk_02 == 1) {
        func_80044750(0x800);
        return;
    }
    if ((u32)(u16)D_801499A0.unk_02 >= 0x2D1) {
        D_801499A0.unk_02 = 0;
    }
}

/* FAKE: T-19008. The counter and the flag are read through u16 pointer views and the halves of tmp are
 * written through s16 views, as in func_8013BEF8. v takes x >> 3 first and adds the x * 80 / 48 term in a
 * second statement: the in-place add gives the ugen temporary the original has before that chain (as1
 * drops the redundant sign extension of the first assignment). */
void func_8013B608(void) {
    s32 tmp;
    s16 v;

    if (((u16 *)&D_801499A0)[1] >= 0x258) {
        ((u16 *)D_801499B0)[1] = 0;
        func_80044750(0x802);
        ((u16 *)&D_801499A0)[1] = 0;
    }
    func_8013C5C8(0x10000);
    func_8013C694();
    if (!(((u16 *)&D_801499A0)[1] & 8)) {
        func_8013C4E4(0x9000);
    }
    if (!(((u16 *)&D_801499A0)[1] & 0xF)) {
        ((s16 *)&tmp)[0] = 0;
        v = D_80149968.unk_1C[0] >> 3;
        v += (D_80149968.unk_1C[0] >> 3) * 80 / 48;
        if (D_80122EC4 % 3 != 0 && (D_80149968.unk_1C[0] >= 0 || D_80149968.unk_0C > 0)) {
            v = (D_80149968.unk_1C[0] >> 3) + 0x20;
        }
        D_801499A0.unk_04 = v;
        D_801499A0.unk_06 = (D_80149968.unk_1C[1] >> 3) + 0x10;
        ((s16 *)&tmp)[1] = v;
    }
    if (((u16 *)&D_801499A0)[1] != 0 && ((u16 *)&D_801499A0)[1] % 45 == 0) {
        if (((u16 *)D_801499B0)[1] == 0) {
            ((u16 *)D_801499B0)[1] = 1;
            func_80044750(0x800);
        } else {
            ((u16 *)D_801499B0)[1] = 0;
            func_80044750(0x801);
        }
        ((u16 *)&D_801499A0)[1]++;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013B78C);

/* FAKE: T-24124. Same counter/flag pointer views and split-half stack word tmp as func_8013BB3C and
 * func_8013C33C. */
void func_8013B94C(void) {
    s32 tmp;

    if (D_801499A2 == 1) {
        if (((D_80149968.unk_1C[0] >> 3) - D_801206BA >= 0x21 && D_80149974 > 0) ||
            ((D_80149968.unk_1C[0] >> 3) - D_801206BA < -0x20 && D_80149968.unk_0C < 0)) {
            func_80044750(0x802);
            D_801499B2 = 2;
        }
    }
    if (((u16 *)&D_801499A0)[1] >= 0x88) {
        ((u16 *)&D_801499A0)[1] = 0;
    }
    func_8013C5C8(0x12000);
    func_8013C694();
    if (!(((u16 *)&D_801499A0)[1] & 0x1F)) {
        func_8013C4E4(0xC000);
    }
    if (!(((u16 *)&D_801499A0)[1] & 0xF)) {
        ((s16 *)&tmp)[0] = 0;
        ((s16 *)&tmp)[1] = D_80149968.unk_1C[0] >> 3;
        ((s16 *)&tmp)[1] += (D_80149968.unk_1C[0] >> 3) * 79 / 48;
        D_801499A0.unk_04 = ((s16 *)&tmp)[1];
        D_801499A0.unk_06 = (D_80149968.unk_1C[1] >> 3) + 0x10;
    }
    if (D_801499B2 == 2 && *(u16 *)&D_801499A2 >= 0x79) {
        D_801499B2 = 0;
    }
    if (((u16 *)&D_801499A0)[1] != 0 && ((u16 *)&D_801499A0)[1] % 90 == 0) {
        if (((u16 *)D_801499B0)[1] == 0) {
            D_801499B2 = 1;
            func_80044750(0x800);
            ((u16 *)&D_801499A0)[1]++;
        } else if (*(u16 *)&D_801499B2 == 1) {
            D_801499B2 = 0;
            func_80044750(0x801);
            ((u16 *)&D_801499A0)[1]++;
        }
    }
}

/* FAKE: T-11640, same views as func_8013C33C (counter and flag reloaded per block, tmp split into s16 halves). */
void func_8013BB3C(void) {
    s32 tmp;

    if (D_80149968.unk_10 == 0 && D_801499B2 != 2) {
        func_80044750(0x802);
        D_801499B2 = 2;
        ((u16 *)&D_801499A0)[1] = 0;
    }
    if (((u16 *)&D_801499A0)[1] > 600) {
        ((u16 *)&D_801499A0)[1] = 0x1F;
    }
    func_8013C5C8(0x1C000);
    func_8013C694();
    if (!(((u16 *)&D_801499A0)[1] & 4)) {
        func_8013C4E4(0x12000);
    }
    if (!(((u16 *)&D_801499A0)[1] & 0xF)) {
        ((s16 *)&tmp)[0] = 0;
        ((s16 *)&tmp)[1] = D_80149968.unk_1C[0] >> 3;
        tmp += D_80149968.unk_0C * 80;
        D_801499A0.unk_04 = ((s16 *)&tmp)[1];
        D_801499A0.unk_06 = (D_80149968.unk_1C[1] >> 3) + 0x10;
    }
    if (((u16 *)&D_801499A0)[1] != 0 && ((u16 *)&D_801499A0)[1] % 30 == 0 && ((u16 *)&D_801499A0)[1] >= 60) {
        if (D_801499B2 == 0) {
            D_801499B2 = 1;
            func_80044750(0x800);
            ((u16 *)&D_801499A0)[1]++;
        } else if (*(u16 *)&D_801499B2 == 1) {
            D_801499B2 = 0;
            func_80044750(0x801);
            ((u16 *)&D_801499A0)[1]++;
        } else {
            D_801499B2 = 0;
        }
    }
}

/* NON_MATCHING: T-19008, 2 instructions missing (12 differing lines with the shifted branch offsets): the
 * original writes the counter back (lui at; sh v0,D_801499A2) before the call to func_8013C5C8, although no
 * path to it changes the counter. A self-assignment of the counter there brings the store back, but its u16
 * normalisation takes one more ugen temporary before the cnt & 8 test and moves the counter reload of the
 * cnt & 8 block to the join; no form without the temporary was found.
 * FAKE (T-19008): the counter and the flag are read through u16 pointer views as in func_8013BEF8. v takes
 * x >> 3 first and adds the x * 79 / 48 term in a second statement (the ugen temporary the original has
 * before the chain). */
#ifdef NON_MATCHING
void func_8013BCEC(void) {
    s16 v;

    if (((u16 *)&D_801499A0)[1] == 0x12C) {
        ((u16 *)D_801499B0)[0] = 1;
        if (((u16 *)D_801499B0)[1] == 0) {
            ((u16 *)D_801499B0)[1] = 1;
            func_80044750(0x800);
        } else {
            ((u16 *)D_801499B0)[1] = 0;
            func_80044750(0x801);
        }
        D_801499A0.unk_08 = 0;
        D_801499A0.unk_0C = 0;
        ((u16 *)&D_801499A0)[1]++;
        D_801206AC = 0x7C;
        func_8013C694();
        return;
    }
    if (((u16 *)&D_801499A0)[1] == 0x12F) {
        D_801206AC = 0x7D;
        func_8013C694();
        return;
    }
    if (((u16 *)&D_801499A0)[1] == 0x131) {
        D_801206AC = 0x7E;
        func_8013C694();
        return;
    }
    if (((u16 *)&D_801499A0)[1] < 0x1A5 && ((u16 *)&D_801499A0)[1] >= 0x12D) {
        func_8013C694();
        return;
    }
    if (((u16 *)&D_801499A0)[1] >= 0x1A5) {
        func_8013C694();
        ((u16 *)D_801499B0)[0] = 0;
        ((u16 *)&D_801499A0)[1] = 0;
        return;
    }
    func_8013C5C8(0x12000);
    func_8013C694();
    if (!(((u16 *)&D_801499A0)[1] & 8)) {
        func_8013C4E4(0xE000);
    }
    if (!(((u16 *)&D_801499A0)[1] & 0xF)) {
        v = D_80149968.unk_1C[0] >> 3;
        v += (D_80149968.unk_1C[0] >> 3) * 79 / 48;
        if (D_80122EC4 % 3 != 0 && (D_80149968.unk_1C[0] >= 0 || D_80149968.unk_0C > 0)) {
            v = (D_80149968.unk_1C[0] >> 3) + 0x20;
        }
        D_801499A0.unk_04 = v;
        D_801499A0.unk_06 = (D_80149968.unk_1C[1] >> 3) + 0x10;
    }
}
#else
INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013BCEC);
#endif

/* FAKE: T-11640. The counter D_801499A0.unk_02 and the flag D_801499B0[0].val are read through u16 pointer
 * views: with the plain field IDO keeps a CVT copy of the promoted counter in a second register, and the
 * volatile D_801499B2 of func_8013C33C stops the lui of the flag load from moving into the branch delay slot.
 * tmp is a stack word whose halves are written and read through s16 views (as in func_8013C33C).
 * Real source unknown (same family as func_8013BB3C/func_8013C33C). */
void func_8013BEF8(void) {
    s32 tmp;

    func_8013C5C8(0x15000);
    func_8013C694();
    if (!(((u16 *)&D_801499A0)[1] & 4)) {
        func_8013C4E4(0xF000);
    }
    if (!(((u16 *)&D_801499A0)[1] & 0xC)) {
        ((s16 *)&tmp)[0] = 0;
        ((s16 *)&tmp)[1] = D_80149968.unk_1C[0] >> 3;
        tmp += D_80149968.unk_0C * 80;
        D_801499A0.unk_04 = ((s16 *)&tmp)[1];
        D_801499A0.unk_06 = (D_80149968.unk_1C[1] >> 3) + 0x10;
    }
    if (((u16 *)D_801499B0)[1] != 0 && ((u16 *)&D_801499A0)[1] > 64) {
        ((u16 *)D_801499B0)[1] = 0;
    }
    if (D_80149990.unk_00 == 0 && !(((u16 *)&D_801499A0)[1] & 0x1F) && ((u16 *)D_801499B0)[1] == 0) {
        if (D_80149968.unk_0C > 0) {
            if (D_801206BA < (D_80149968.unk_1C[0] >> 3) - 0x18) {
                ((u16 *)D_801499B0)[1] = 1;
                ((u16 *)&D_801499A0)[1] = 0;
                func_80044750(0x802);
            } else {
                func_80044750(0x801);
            }
        } else if (D_80149968.unk_0C < 0) {
            if ((D_80149968.unk_1C[0] >> 3) + 0x18 < D_801206BA) {
                ((u16 *)D_801499B0)[1] = 1;
                ((u16 *)&D_801499A0)[1] = 0;
                func_80044750(0x802);
            } else {
                func_80044750(0x800);
            }
        }
        ((u16 *)&D_801499A0)[1]++;
    }
    if (((u16 *)&D_801499A0)[1] > 128) {
        ((u16 *)&D_801499A0)[1] = 64;
    }
}

/* FAKE: T-11640, the u16 pointer views of the counter and the flag and the s16 views of tmp as in func_8013BEF8. */
void func_8013C0E4(void) {
    s32 tmp;

    func_8013C5C8(0x11800);
    func_8013C694();
    if (!(((u16 *)&D_801499A0)[1] & 8)) {
        func_8013C4E4(0x8000);
    }
    if (!(((u16 *)&D_801499A0)[1] & 0xF)) {
        ((s16 *)&tmp)[0] = 0;
        ((s16 *)&tmp)[1] = D_80149968.unk_1C[0] >> 3;
        tmp += D_80149968.unk_0C * 80;
        D_801499A0.unk_04 = ((s16 *)&tmp)[1];
        D_801499A0.unk_06 = (D_80149968.unk_1C[1] >> 3) + 0x10;
    }
    if (D_80120676 < ((DisplayFixActor44 *)D_8011ECD0)[97].unk_24.h[1] &&
        func_800AE0C0(D_8012067A - D_801206BE) < 12 &&
        ((DisplayFixActor44 *)D_8011ECD0)[97].unk_24.h[1] - 0x20 < D_80120676) {
        ((DisplayFixActor44 *)D_8011ECD0)[97].unk_24.w += 0x11800;
    } else if (((DisplayFixActor44 *)D_8011ECD0)[97].unk_24.h[1] < D_80120676 &&
               func_800AE0C0(D_8012067A - D_801206BE) < 12 &&
               D_80120676 < ((DisplayFixActor44 *)D_8011ECD0)[97].unk_24.h[1] + 0x20) {
        ((DisplayFixActor44 *)D_8011ECD0)[97].unk_24.w -= 0x11800;
    }
    if (((u16 *)&D_801499A0)[1] > 128) {
        ((u16 *)&D_801499A0)[1] = 0x20;
    }
    if (((u16 *)&D_801499A0)[1] != 0 && !(((u16 *)&D_801499A0)[1] & 0x1F) && ((u16 *)&D_801499A0)[1] > 32) {
        if (((u16 *)D_801499B0)[1] == 0) {
            ((u16 *)D_801499B0)[1] = 1;
            ((u16 *)&D_801499A0)[1]++;
            func_80044750(0x800);
        } else if (((u16 *)D_801499B0)[1] == 1) {
            ((u16 *)D_801499B0)[1] = 0;
            ((u16 *)&D_801499A0)[1]++;
            func_80044750(0x801);
        } else {
            ((u16 *)D_801499B0)[1] = 0;
        }
    }
}

/* FAKE: T-11640. The original reloads the counter D_801499A0.unk_02 and the flag D_801499B2 in
 * every block where IDO keeps them in a register: the counter is read through a u16 view of
 * D_801499A0, the flag is declared volatile and its second test reads it through a plain u16
 * view (all-volatile reads change the delay-slot fill). tmp is a stack word whose halves are written and
 * read through s16 views (the 16.16 position of the four sibling functions). Real source unknown. */
void func_8013C33C(void) {
    s32 tmp;

    if (D_80149968.unk_10 == 0 && D_801499B2 != 2) {
        D_801499B2 = 2;
        ((u16 *)&D_801499A0)[1] = 0;
        func_80044750(0x802);
    }
    if (((u16 *)&D_801499A0)[1] > 600) {
        ((u16 *)&D_801499A0)[1] = 0;
    }
    func_8013C5C8(0x18800);
    func_8013C694();
    if (!(((u16 *)&D_801499A0)[1] & 4)) {
        func_8013C4E4(0x10000);
    }
    if (!(((u16 *)&D_801499A0)[1] & 0xC)) {
        ((s16 *)&tmp)[0] = 0;
        ((s16 *)&tmp)[1] = D_80149968.unk_1C[0] >> 3;
        tmp += D_80149968.unk_0C * 80;
        D_801499A0.unk_04 = ((s16 *)&tmp)[1];
        D_801499A0.unk_06 = (D_80149968.unk_1C[1] >> 3) + 0x10;
    }
    if (((u16 *)&D_801499A0)[1] != 0 && ((u16 *)&D_801499A0)[1] % 30 == 0 && ((u16 *)&D_801499A0)[1] >= 60) {
        if (D_801499B2 == 0) {
            D_801499B2 = 1;
            func_80044750(0x800);
            ((u16 *)&D_801499A0)[1]++;
        } else if (*(u16 *)&D_801499B2 == 1) {
            D_801499B2 = 0;
            func_80044750(0x801);
            ((u16 *)&D_801499A0)[1]++;
        } else {
            D_801499B2 = 0;
        }
    }
}

void func_8013C4E4(s32 arg0) {
    s32 var_a0;
    /* FAKE: two unused locals reproduce the original's frame (the real locals are unknown). T-10520 */
    s32 pad;
    s32 pad2;
    s32 sp40;
    s32 sp3C;
    s32 sp38;
    s32 sp34;

    if (func_800AE0C0((D_80149968.unk_1C[0] >> 3) - D_801206BA) < 0x50) {
        if (D_80149968.unk_10 < 0) {
            D_80149968.unk_10 = 0;
        }
        var_a0 = -((((D_80149968.unk_1C[0] >> 3) - D_801206BA) << 0xA) >> 6);
        if ((D_80122EC4 % 3) != 0) {
            var_a0 = var_a0 - 0x100;
        }
        sp40 = 0;
        sp3C = arg0;
        func_80086204(var_a0, &sp40, &sp3C, &sp38, &sp34);
        D_801499A0.unk_08 = sp38;
        D_801499A0.unk_0C = sp34;
        return;
    }
    D_801499A0.unk_08 = 0;
    D_801499A0.unk_0C = 0;
}

void func_8013C5C8(s32 arg0)
{
  /* FAKE: empty conditions retain the shared record base in V1 instead
   * of folding the word accesses to separate addresses. T-9510. */
  TkCoordsView *records = (TkCoordsView *) D_8011ECD0;
  if ((D_801499A0.unk_04 + 2) < D_801206BA)
  {
    records->pos[0].word -= arg0;
  }
  else
  {
    if ((records->pos[0].halves.hi + 2) < D_801499A0.unk_04)
    {
      records->pos[0].word += arg0;
    }
    if (!records->pos[0].halves.hi)
    {
    }
    if (!records->pos[1].halves.hi)
    {
    }
  }
  if ((D_801499A0.unk_06 + 2) < D_801206BE)
  {
    records->pos[1].word -= arg0;
    return;
  }
  if ((records->pos[1].halves.hi + 2) < D_801499A0.unk_06)
  {
    records->pos[1].word += arg0;
  }
}

void func_8013C694(void) {
    if (D_801206BE < (D_80149968.unk_1C[1] >> 3) + 0x10) {
        if (D_801206BA < (D_80149968.unk_1C[0] >> 3) + 0x50 && (D_80149968.unk_1C[0] >> 3) - 0x50 < D_801206BA) {
            D_801206BE = (D_80149968.unk_1C[1] >> 3) + 0x10;
        }
    }
    if ((D_80149968.unk_1C[0] >> 3) - 0x50 < D_801206BA && D_801206BA < (D_80149968.unk_1C[0] >> 3) && D_801206BE < (D_80149968.unk_1C[1] >> 3) + 0x10) {
        D_801206BA = (D_80149968.unk_1C[0] >> 3) - 0x50;
    }
    if (D_801206BA < (D_80149968.unk_1C[0] >> 3) + 0x50) {
        if ((D_80149968.unk_1C[0] >> 3) < D_801206BA && D_801206BE < (D_80149968.unk_1C[1] >> 3) + 0x10) {
            D_801206BA = (D_80149968.unk_1C[0] >> 3) + 0x50;
        }
    }
}

void func_8013C780(void) {
    TaiikuU11 a;
    TaiikuS11 b;

    a = D_80149F50;
    b = D_80149F68;
    if (a.v[D_80149940] < D_801499A0.unk_00 && (D_80149940 != 2 || (D_80149940 == 2 && D_80149960 > 0) || D_80149990.unk_00 != 0)) {
        if (D_8014999C >= 3 && D_80149990.unk_00 != 0 && (D_80149950 == 2 || D_80149950 == 4 || D_80149950 == 6)) {
            D_8014999C = 0;
        } else if (D_8014999C >= 5) {
            D_8014999C = 0;
        } else {
            D_8014999C++;
        }
        D_801499A0.unk_00 = 0;
    }
    D_801206AC = D_80149940 * 6 + D_8014999C + 6;
    if (D_80149990.unk_00 != 0 && (D_80149950 == 2 || D_80149950 == 4 || D_80149950 == 6)) {
        if (D_8014999C >= 3) {
            D_8014999C = 0;
        }
        D_801206AC = b.v[D_80149940] + D_8014999C;
        if (D_80149940 == 2 && D_80120697 == 0xA5) {
            D_801206AC += 4;
        }
    }
}

void func_8013C978(void) {
    s32 target;

    switch (D_80149950) {
    case 0:
        if (D_801206BE < (D_80149968.unk_1C[1] >> 3) + 0x10 &&
            D_801206BA < (D_80149968.unk_1C[0] >> 3) + 0x50 && (D_80149968.unk_1C[0] >> 3) - 0x50 < D_801206BA) {
            D_801206BE = (D_80149968.unk_1C[1] >> 3) + 0x10;
        }
        if (D_801207CA < -0x1C0 && D_80149960 == 0) {
            D_8014999C = 0;
            D_80149950++;
        }
        break;
    case 1:
        D_80149950++;
        break;
    case 2:
        D_80149950++;
        break;
    case 3:
        if (D_80149990.unk_00 == 1) {
            D_801206BA = 0x90;
        } else {
            D_801206BA = -0x90;
            D_80120697 |= 1;
        }
        D_801206BE = 0x20;
        D_80149950++;
        break;
    case 4:
        if (D_80149990.unk_00 == 1 && D_8014994C >= 4) {
            D_801206BA = (D_80149968.unk_1C[0] >> 3) + 0x60;
            D_801206BE = D_80149968.unk_1C[1] >> 3;
            if (D_80149968.unk_14 <= 0) {
                D_8014999C = 0;
                D_80149968.unk_0C = 0;
                D_80149950++;
            }
        }
        if (D_80149990.unk_00 == 2 && D_8014994C >= 4) {
            D_801206BA = (D_80149968.unk_1C[0] >> 3) - 0x60;
            D_801206BE = D_80149968.unk_1C[1] >> 3;
            if (D_80149968.unk_14 >= 0) {
                D_8014999C = 0;
                D_80120697 = 0xA4;
                D_80149968.unk_0C = 0;
                D_801206BA = -0x50;
                D_80149950++;
            }
        }
        break;
    case 5:
        if (D_801206BE < 0x30) {
            D_801206BE += 2;
        } else {
            D_801206BE = 0x30;
            if (D_80149990.unk_00 == 2) {
                D_80120697 |= 1;
            }
            D_8014999C = 0;
            D_80149950++;
        }
        break;
    case 6:
        if (D_80122EC4 % 3 == 0 || D_80149940 == 2) {
            target = -0x20;
        } else {
            target = 0;
        }
        if (D_80149990.unk_00 == 1) {
            if (target < D_801206BA) {
                D_801206BA -= 2;
            } else {
                D_801206BA = target;
                D_80149950++;
            }
        } else if (D_801206BA < target) {
            D_801206BA += 2;
        } else {
            D_801206BA = target;
            D_80149950++;
        }
        break;
    case 7:
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013CCFC(void) {
    func_8013CD34();
    func_8013CF88();
    func_8013D114();
    func_8013D2B4();
}

void func_8013CD34(void) {
    s32 unused; /* FAKE: T-11640, reserves the original's frame slot above a0 (no use in the code) */
    s32 a0;
    s32 sp44;
    s32 sp40;
    s32 sp3C;
    s32 sp38;

    if (D_80149990.unk_00 != 0) {
        D_80149968.unk_00 = 0;
        D_80149968.unk_04 = 0;
    } else if (func_8013CEF8() != 0 && (D_800E6280.unk_F88 & 0x20)) {
        if (D_80149968.unk_10 < 0) {
            D_80149968.unk_10 = 0;
        }
        a0 = -(((((s16) D_80149968.unk_1C[0] >> 3) - D_80120676) << 10) >> 6);
        if (D_80122EC4 % 3 != 0) {
            a0 = -(((((s16) D_80149968.unk_1C[0] >> 3) - D_80120676) << 10) >> 6) - 0x200;
        }
        sp44 = 0;
        sp40 = 0x10000;
        Rot_2D(a0, &sp44, &sp40, &sp3C, &sp38);
        D_80149990.unk_04 = sp3C;
        D_80149990.unk_08 = sp38;
        D_80149968.unk_08 = 0;
    } else if (D_80149968.unk_08 > 10) {
        D_80149968.unk_08 = 0;
        if (D_801499A0.unk_0C <= 0 && (u16) D_801499B0[0].unk0 == 0) {
            D_80149990.unk_08 -= 0x4000;
            if (D_80149968.unk_0C > 0) {
                D_80149990.unk_04 -= 0x4000;
            } else if (D_80149968.unk_0C < 0) {
                D_80149990.unk_04 += 0x4000;
            }
        } else {
            D_80149990.unk_04 = 0;
            D_80149990.unk_08 = 0;
        }
    }
    if (D_80149990.unk_00 == 0) {
        D_80149968.unk_00 = D_80149990.unk_04 + D_801499A0.unk_08;
        D_80149968.unk_04 = D_80149990.unk_08 + D_801499A0.unk_0C;
    }
}

s32 func_8013CEF8(void) {
    s32 dx;
    s32 temp_v0;

    dx = func_800AE0C0(((s16) D_80149968.unk_1C[0] >> 3) - D_80120676);
    temp_v0 = func_800AE0C0(((s16) D_80149968.unk_1C[1] >> 3) - D_8012067A);
    if (D_80149944 != 1) {
        return 0;
    }
    if ((0x10 - (dx >> 2)) >= (temp_v0 - 0x10)) {
        return 1;
    }
    return 0;
}

void func_8013CF88(void) {
    if (D_80149990.unk_00 != 0 && D_8014994C == 4 && D_80149950 == 4) {
        D_80149968.unk_10 = 0;
        if (D_80149990.unk_00 == 1) {
            D_80149968.unk_0C = -0x10000;
            return;
        }
        D_80149968.unk_0C = 0x10000;
        return;
    }
    if (D_80149968.unk_0A > 10) {
        D_80149968.unk_0A = 0;
        D_80149968.unk_10 += D_80149968.unk_04;
        if (D_80149968.unk_10 < 0) {
            D_80149968.unk_10 = 0;
        }
        if (D_80149968.unk_10 > 0x10000) {
            D_80149968.unk_10 = 0x10000;
        }
        if (D_80149968.unk_0C > 0) {
            D_80149968.unk_0C += D_80149968.unk_00;
            if (D_80149968.unk_0C <= 0) {
                D_80149990.unk_04 = 0;
                D_801499A0.unk_08 = 0;
                D_80149968.unk_00 = 0;
                D_80149968.unk_0C = 0;
            }
        } else if (D_80149968.unk_0C < 0) {
            D_80149968.unk_0C += D_80149968.unk_00;
            if (D_80149968.unk_0C >= 0) {
                D_80149990.unk_04 = 0;
                D_801499A0.unk_08 = 0;
                D_80149968.unk_00 = 0;
                D_80149968.unk_0C = 0;
            }
        } else {
            D_80149968.unk_0C += D_80149968.unk_00;
        }
        if (D_80149968.unk_0C < -0x10000) {
            D_80149968.unk_0C = -0x10000;
        }
        if (D_80149968.unk_0C > 0x10000) {
            D_80149968.unk_0C = 0x10000;
        }
        if (D_80149968.unk_10 < -0x10000) {
            D_80149968.unk_10 = -0x10000;
        }
        if (D_80149968.unk_10 > 0x10000) {
            D_80149968.unk_10 = 0x10000;
        }
    }
}

/* FAKE: T-11640, the high halves of the 16.16 words (unk_14, unk_18) and of tmp are read and written through s16
 * views, which is what reproduces the original's code here. Real source unknown. */
void func_8013D114(void) {
    s32 tmp;

    if (D_80149968.unk_04 <= 0) {
        D_80149968.unk_18 += 0x18000;
    } else {
        D_80149968.unk_18 -= 0x10000;
    }
    D_80149968.unk_14 += D_80149968.unk_0C;
    if (((s16 *)&D_80149968.unk_14)[1] > 0x60) {
        D_80149990.unk_00 = 1;
        ((s16 *)&D_80149968.unk_14)[1] = 0x60;
        D_801499B0[0].unk0 = 0;
        func_8013D254();
    } else if (((s16 *)&D_80149968.unk_14)[1] < -0x60) {
        D_80149990.unk_00 = 2;
        ((s16 *)&D_80149968.unk_14)[1] = -0x60;
        D_801499B0[0].unk0 = 0;
        func_8013D254();
    }
    if (((s16 *)&D_80149968.unk_18)[1] < 0) {
        ((s16 *)&D_80149968.unk_18)[1] = 0;
    }
    if (((s16 *)&D_80149968.unk_18)[1] >= 0x80) {
        ((s16 *)&D_80149968.unk_18)[1] = 0x80;
    }
    tmp = D_80149968.unk_14 * 4;
    D_80149968.unk_1C[0] = ((s16 *)&tmp)[1];
    tmp = D_80149968.unk_18 * 2;
    D_80149968.unk_1C[1] = ((s16 *)&tmp)[1];
}

void func_8013D254(void) {
    func_80044750(0x504);
    D_80149988.unk_00 = 0;
    D_8014999C = 0;
    D_801499B0[0].unk0 = 0;
    D_80149968.unk_0C = 0;
    D_8014994C = 0;
    D_80149950 = 0;
    D_801207A7 |= 0x80;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013D2B4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80138950", func_8013D788);

/* FAKE: T-11640, the (u8) narrowing of v before "+ 8" changes no stored value (the fields are u8) but
 * reproduces the original's ugen temporaries; perhaps a u8 texture-coordinate macro in the real source. */
void func_8013D9B4(void) {
    s16 dx;
    s16 dy;
    s32 i;
    s32 row;
    POLY_FT4 *p;

    dx = D_800E6280.unk_014[D_8011ECA0];
    dy = D_800E6280.unk_018[D_8011ECA0];
    for (i = 0; i < 80; i++) {
        p = func_800490F0(0x28, D_8011ECA0);
        func_8009F040(p);
        func_8009EF84(p, 0);
        func_8009EFB0(p, 0);
        row = i >> 2;
        switch (row) {
        case 0:
            p->r0 = 0x68;
            p->g0 = 0x68;
            p->b0 = 0x68;
            p->u0 = 0;
            p->v0 = (D_80149966 >> 3) + row * 8;
            p->u1 = 0x40;
            p->v1 = (D_80149966 >> 3) + row * 8;
            p->u2 = 0;
            p->v2 = (u8) ((D_80149966 >> 3) + row * 8) + 8;
            p->u3 = 0x40;
            p->v3 = (u8) ((D_80149966 >> 3) + row * 8) + 8;
            break;
        case 1:
            p->r0 = 0x70;
            p->g0 = 0x70;
            p->b0 = 0x70;
            p->u0 = 0;
            p->v0 = (D_80149966 >> 2) + row * 8;
            p->u1 = 0x40;
            p->v1 = (D_80149966 >> 2) + row * 8;
            p->u2 = 0;
            p->v2 = (u8) ((D_80149966 >> 2) + row * 8) + 8;
            p->u3 = 0x40;
            p->v3 = (u8) ((D_80149966 >> 2) + row * 8) + 8;
            break;
        case 2:
            p->r0 = 0x78;
            p->g0 = 0x78;
            p->b0 = 0x78;
            p->u0 = 0;
            p->v0 = (D_80149966 >> 1) + row * 8;
            p->u1 = 0x40;
            p->v1 = (D_80149966 >> 1) + row * 8;
            p->u2 = 0;
            p->v2 = (u8) ((D_80149966 >> 1) + row * 8) + 8;
            p->u3 = 0x40;
            p->v3 = (u8) ((D_80149966 >> 1) + row * 8) + 8;
            break;
        default:
            p->r0 = 0x80;
            p->g0 = 0x80;
            p->b0 = 0x80;
            p->u0 = 0;
            p->v0 = D_80149966 + row * 8;
            p->u1 = 0x40;
            p->v1 = D_80149966 + row * 8;
            p->u2 = 0;
            p->v2 = (u8) (D_80149966 + row * 8) + 8;
            p->u3 = 0x40;
            p->v3 = (u8) (D_80149966 + row * 8) + 8;
            break;
        }
        p->x0 = D_80149B24[i] + dx;
        p->y0 = D_801499E4[i] + dy;
        p->x1 = D_80149BC4[i] + dx;
        p->y1 = D_801499E4[i] + dy;
        p->x2 = D_80149C64[i] + dx;
        p->y2 = D_80149A84[i] + dy;
        p->x3 = D_80149D04[i] + dx;
        p->y3 = D_80149A84[i] + dy;
        p->tpage = func_8009ECB0(1, 0, 0x300, 0);
        p->clut = func_8009ED74(0, 0x1F0);
        func_8009EED0(&D_800E8CA0[D_8011ECA0 * 0x400 + 0x3EC], p);
    }
}

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

void func_8013F948(void) {
    s32 i;

    if (D_801499DC >= 31) {
        D_801499DC = 0;
        for (i = 0; i < 4; i++) {
            if (i == 0) {
                if (D_80149990.unk_00 == 0) {
                    if (((DisplayActor44 *)D_8011ECD0)[104].unk_18 >= 0x90) {
                        ((DisplayActor44 *)D_8011ECD0)[104].unk_18 ^= 1;
                    } else if (((DisplayActor44 *)D_8011ECD0)[104].unk_18 == 0x8D) {
                        ((DisplayActor44 *)D_8011ECD0)[104].unk_18 = 0x8A;
                    } else {
                        ((DisplayActor44 *)D_8011ECD0)[104].unk_18++;
                    }
                }
            } else if (D_801499CC[i - 1] == 0) {
                if (((DisplayActor44 *)D_8011ECD0)[104 + i].unk_18 >= 0x8E) {
                    ((DisplayActor44 *)D_8011ECD0)[104 + i].unk_18 ^= 1;
                } else if (((DisplayActor44 *)D_8011ECD0)[104 + i].unk_18 == 0x89) {
                    ((DisplayActor44 *)D_8011ECD0)[104 + i].unk_18 = 0x86;
                } else {
                    ((DisplayActor44 *)D_8011ECD0)[104 + i].unk_18++;
                }
            }
        }
    }
}

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

void func_8013FC5C(void) {
    s32 i;
    s32 j;
    TaiikuI3 speed;

    speed = D_80149F80;
    for (i = 0; i < 3; i++) {
        if (D_801499B4[i].halves.high < 0xE00) {
            if (D_801499CC[i] == 0) {
                D_801499B4[i].word += speed.v[i];
            }
        } else if (D_801499C4[i] != 0xFFFF) {
            D_801499C4[i] = 0xFFFF;
            for (j = 0; D_801499D4[j] != 0xFF; j++) {
            }
            D_801499D4[j] = i + 1;
        }
        if ((func_8013FB14(i + 1) == 1 || func_8013FB14(i + 1) == 2 || func_8013FB14(i + 1) == 4 ||
             func_8013FB14(i + 1) == 5) && !(func_800AE0D0() & 0xFFF)) {
            D_801499CC[i] = 1;
        }
        if (D_801499CC[i] == 1) {
            D_801499C4[i]++;
            if (D_801499C4[i] > 240) {
                D_801499CC[i] = 0;
                D_801499C4[i] = 0;
            }
        }
    }
}

s8 func_8013FE44(void) {
    s32 i;
    s16 v[3];

    for (i = 0; i < 3; i++) {
        if (D_801499B4[i].halves.high < 0xE00) {
            v[i] = D_801499B4[i].halves.high;
        } else {
            v[i] = -1;
        }
    }
    if ((v[0] >= v[1]) && (v[0] >= v[2])) {
        D_801499B4[0].halves.high = -1;
        return 1;
    }
    if (v[1] >= v[2]) {
        D_801499B4[1].halves.high = -1;
        return 2;
    }
    D_801499B4[2].halves.high = -1;
    return 3;
}

void func_8013FEF0(void) {
    MATRIX m1;
    MATRIX m2;
    TaiikuSVec rot;
    VECTOR scale;

    m1 = *(MATRIX *)D_80125C60;
    rot.vx = 0;
    rot.vz = 0;
    rot.vy = D_8014995A;
    func_800A09D0(&rot, &m1);
    m2 = *(MATRIX *)D_80125C60;
    rot.vy = 0;
    rot.vz = 0;
    rot.vx = D_80149958;
    func_800A09D0(&rot, &m2);
    func_800A0C64(&m1, &m2);
    scale.vx = D_80149954;
    scale.vy = D_80149954;
    scale.vz = D_80149954;
    func_800A0F4C(&m2, &scale);
    func_800A0C64(&m1, &m2);
    m1.t[0] = D_80149968.unk_1C[0];
    m1.t[1] = D_80149968.unk_1C[1];
    m1.t[2] = 0x1B58;
    *(MATRIX *)D_801227A4 = m1;
    D_801227A0->unk0 = 0;
    D_80149954 -= 0x80;
    if ((D_80149968.unk_1C[1] >> 3) >= -0x3F) {
        D_80149968.unk_1C[1] -= 8;
    }
}

void func_801400D8(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_03 = 0xA4;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_02 = 0x40;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_05 = 0x80;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_01 = 3;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_38 = 0x1000000;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_07 = 0x80;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_0C = (u8 *)D_80149FE4;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_10 = (u8 *)D_8014A01C;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_34 = (u8 *)D_80149DF4;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_14 = D_8014A054;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_1A = 0x1000;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_1C = 0x1000;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_06 = 2;
    }
    for (i = 0; i < 4; i++) {
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_26 = i * 0x40 - 0x78;
        ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_2A = -8;
        switch (D_801499D4[i]) {
        case 0:
            ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_18 = D_80149940 * 7;
            break;
        case 1:
            ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_18 = 0x4D;
            ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_43 = 0x1C;
            break;
        case 2:
            ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_18 = 0x4D;
            ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_43 = 0x1D;
            break;
        case 3:
            ((TaiikuActor44 *)D_8011ECD0)[96 + i].unk_18 = 0x4D;
            break;
        default:
            func_80046500();
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        ((TaiikuActor44 *)D_8011ECD0)[100 + i].unk_26 = i * 0x40 - 0x78;
        ((TaiikuActor44 *)D_8011ECD0)[100 + i].unk_2A = -8;
        ((TaiikuActor44 *)D_8011ECD0)[100 + i].unk_18 = 0x58;
        switch (D_801499D4[i]) {
        case 0:
            break;
        case 1:
            ((TaiikuActor44 *)D_8011ECD0)[100 + i].unk_43 = 0x1C;
            break;
        case 2:
            ((TaiikuActor44 *)D_8011ECD0)[100 + i].unk_43 = 0x1D;
            break;
        case 3:
            ((TaiikuActor44 *)D_8011ECD0)[100 + i].unk_43 = 0x1E;
            break;
        }
    }
}

void func_80140338(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        ((DisplayActor44 *)D_8011ECD0)[i + 100].unk_18 -= 4 - i;
    }
}

void func_8014038C(void) {
    s32 i;
    TkS16x4 t;

    t = D_80149F8C;
    for (i = 0; i < 4; i++) {
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_18 += t.v[i];
    }
}

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

void func_801404DC(void) {
    D_80122760 = 0x64;
    D_80122764 = 0x64;
    D_80122768 = 0x320;
    D_8012276C = 0xD0;
    D_8012276D = 0xD0;
    D_8012276E = 0xD0;
    func_8009AD70(0, &D_80122760);
    D_80122770 = 0x64;
    D_80122774 = 0x64;
    D_80122778 = 0x64;
    D_8012277C = 0xF0;
    D_8012277D = 0;
    D_8012277E = 0;
    func_8009AD70(1, &D_80122770);
    D_80122780 = 0x64;
    D_80122784 = 0x64;
    D_80122788 = 0x64;
    D_8012278C = 0;
    D_8012278D = 0;
    D_8012278E = 0xF0;
    func_8009AD70(2, &D_80122780);
    func_8009B310(0x800, 0x800, 0x800);
    D_80122790 = -0xF00;
    D_80122794 = 0x01400000;
    D_80122798 = 1;
    D_80122799 = 1;
    D_8012279A = 1;
    func_801488F0(&D_80122790);
    func_8009B340(1);
}

void func_8014064C(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        func_80099E30(0, &D_801227A0[i]);
        D_801227A0[i].unk20 = -4000;
        D_801227A0[i].unk0 = 0;
    }
}

void func_801406A8(s32 *arg0) {
    s32 *dop;
    s32 i;

    dop = arg0;
    dop++;
    func_8009B3C0((s32)dop);
    dop++;
    dop++;
    for (i = 0; i < 16; i++) {
        func_8009B430((s32)dop, (u8 *)&D_80122640[i * 4], 0);
        D_80122640[i * 4 + 1] = (u32)&D_801227A0[i];
        D_80122640[i * 4] = 0x80000000;
    }
}

void func_80140738(s32 arg0, s32 arg1) {
    u8 *p;
    s32 sp30;

    sp30 = arg0 + 4;
    func_8009B3C0(sp30);
    p = (u8 *)D_80122640 + arg1 * 0x10;
    func_8009B430(sp30 + 8, p, 0, sp30);
    *(u8 **)((u8 *)D_80122640 + arg1 * 0x10 + 4) = (u8 *)&D_801227A0[arg1];
    *(s32 *)((u8 *)D_80122640 + arg1 * 0x10) = 0;
}

void func_801407C0(void) {
    D_80149FA0 = 0x8019D000;
    D_80149FA4 = 0x8018F000;
    D_80149FA8 = 0x8019E218;
    D_80149FAC = 0x8019D008;
}
