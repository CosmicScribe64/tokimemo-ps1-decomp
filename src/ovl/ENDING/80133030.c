#include "common.h"
#include "ovl/ENDING.h"

typedef struct {
    void (*f[16])();
} FnTbl16; /* size 0x40 */
extern FnTbl16 D_8013C370;

void func_80133030(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl16 tbl;

    tbl = D_8013C370;
    func_80083808();
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    func_8007EDF8();
    func_800846C0();
    func_80066C08(2);
    func_80064F48();
    func_80066334();
    func_80064DEC();
    func_80083A10();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133030", func_801330E4);

void func_80133400(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}

void func_80133428(void) {
    func_80046318(0x11, 0x801F0000, 0xAE07);
    func_80132000();
    func_8004284C();
}

void func_80133460(void) {
    if (((u8) D_800E6280.unk_F5F >= 0xDU) && ((u32) ((u32) (D_800E6280.unk_1BC[14].unk_0C.w << 0x14) >> 0x1D) < 3U)) {
        func_80042908(7);
        D_800E7D34 |= 8;
        return;
    }
    func_8004284C();
    D_800E7D34 |= 4;
}

void func_801334E4(void) {
    if ((((u32)(D_800E6280.unk_1BC[1].unk_0C.w << 0x1E) >> 0x1F)) && (get_h_tokimeki(1) >= 0x46U)) {
        normal_date_speak();
        return;
    }
    func_8004284C();
}
