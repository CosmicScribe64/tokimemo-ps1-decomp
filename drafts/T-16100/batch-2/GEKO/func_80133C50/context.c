#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[19])();
} FnTbl19; /* size 0x4C */
extern FnTbl19 D_80144D40;

void func_801338F0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl19 tbl;

    tbl = D_80144D40;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8013396C(void) {
    if (D_80144C50 == 0x64) {
        func_801322E8();
        return;
    }
    func_8004284C();
}

void func_801339AC(void) {
    u16 raw;

    if (D_80144C50 == 0x64) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    } else if (D_800E6280.unk_F5F == 4) {
        if (D_80144C3C == 0x26) {
            raw = D_800E6280.unk_0F4.h;
            if (((raw >> 0xC) == 6) && ((D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == 6)) {
                D_80144C3C = 0x28;
            } else if (((raw >> 0xC) == 7) && ((D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == 7)) {
                D_80144C3C = 0x29;
            } else {
                D_80144C3C = 0x2A;
            }
        } else {
            func_8004284C();
        }
    } else {
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801338F0", func_80133AA0);

/* bulk_m2c declarations (T-11190) */
extern s32 D_800CA280;
extern s32 D_80144D8C;
extern u16 D_800E6374;
extern u8 D_800E71DF;
/* end bulk_m2c declarations */
void func_80133C50(void) {
    typedef struct { s32 w[60]; } M2cBlockF0; /* m2c memcpy of 240 bytes */
    s32 sp30[60];
    s32 temp_v0;
    s32 var_at;
    u32 temp_t1;
    u32 temp_t2;
    u32 temp_t8;
    u32 temp_t9;

    *(M2cBlockF0 *)(&sp30) = *(M2cBlockF0 *)(&D_80144D8C);
    func_800AE0F0(&D_800CA280, (u8 *)sp30 + (((u16) D_800E6374 >> 0xC) * 0x14));
    D_800E6280.unk_1BC[D_800E71DF].unk_0C.b[0] |= 1;
    if (D_80144C50 == 0xA) {
        D_800E6280.unk_1BC[D_800E71DF].unk_0C.b[0] |= 0x40;
    }
    D_80144C3C = 0x16;
    temp_v0 = get_g_zyotai_s(D_800E71DF, &D_800E6280, 0x38);
    if (D_80144C50 == 2) {
        D_80144C50 = 0;
        if (D_800E71DF == 0) {
            temp_t9 = (u16) D_800E6374 >> 0xC;
            temp_t1 = (u8) D_800E6280.unk_1BC[D_800E71DF].unk_0C.b[2] >> 4;
            if ((temp_t9 == temp_t1) && (temp_t9 < 6U)) {
                func_80132A4C(2);
            } else if ((temp_t9 != temp_t1) && (temp_t9 < 6U)) {
                var_at = temp_t9 < 6U;
                if (temp_t1 < 6U) {
                    func_80132A4C(3);
                } else {
                    goto block_12;
                }
            } else {
                var_at = temp_t9 < 6U;
block_12:
                if ((var_at != 0) && (temp_t1 >= 6U)) {
                    func_80132A4C(4);
                } else {
                    func_80132A4C(5);
                }
            }
        } else if ((D_800E71DF == 1) || (D_800E71DF == 2) || (D_800E71DF == 3)) {
            temp_t2 = (u16) D_800E6374 >> 0xC;
            temp_t8 = (u8) D_800E6280.unk_1BC[D_800E71DF].unk_0C.b[2] >> 4;
            if (temp_t2 == temp_t8) {
                func_80132A4C(2);
            } else if ((temp_t2 != temp_t8) && (temp_t2 < 6U)) {
                func_80132A4C(3);
            } else {
                func_80132A4C(5);
            }
        } else if ((D_800E71DF == 4) || (D_800E71DF == 5) || (D_800E71DF == 0xA)) {
            if ((u32) ((u16) D_800E6374 >> 0xC) < 6U) {
                func_80132A4C(3);
            } else if ((u32) (temp_v0 & 0x7F) < 2U) {
                func_80132A4C(4);
            } else {
                func_80132A4C(5);
            }
        } else if ((D_800E71DF == 6) || (D_800E71DF == 7) || (D_800E71DF == 8) || (D_800E71DF == 9)) {
            if ((u32) ((u16) D_800E6374 >> 0xC) < 6U) {
                func_80132A4C(2);
            } else {
                func_80132A4C(3);
            }
        }
    }
    func_8004284C();
}

void func_80133F94(void) {
    u32 cur;

    D_80144C3C = (D_80144C3C + D_80144C50) - 1;
    cur = D_80144C50;
    if (cur == 0x64) {
        D_80144C3C = 0x28;
    }
    func_8004284C();
}

void func_80133FE8(void) {
    D_80144C3C = 0x27;
    func_8004284C();
}

void func_80134010(void) {
    bg_read_sub2(0x42D8);
    func_8004284C();
}
