#include "common.h"
#include "ovl/SHOUGATU.h"

typedef struct {
    u32 pad : 1;
    u32 f : 1;
    u32 rest : 30;
} Bit1Flag;

void func_8013DD90(void) {
    D_80145FD0 = 0x801A6558;
    D_80145FD4 = 0x801A655C;
    D_80145FD8 = 0x801A6580;
    D_80145FE0 = 0x801A0000;
}

s32 func_8013DDD0(void) {
    s32 index;
    ShoCallbacks23 table;

    table = D_80145FE8;
    index = D_800E6280.unk_110A;
    table.f[index](0x80);
    ((DisplayActor44 *)D_80120650)->unk_07 = D_800B593C;
    if (((DisplayActor44 *)D_80120650)->unk_18 == 0 && ((DisplayActor44 *)D_80120650)->unk_08 == 0) {
        ((DisplayActor44 *)D_80120650)->unk_16++;
        if (((DisplayActor44 *)D_80120650)->unk_16 >= 3) {
            ((DisplayActor44 *)D_80120650)->unk_16 = 0;
        }
    }
}

void func_8013DEBC(void) {
    func_80044750(0x204);
    func_8004284C();
}

void func_8013DEE4(void) {
    D_80120653 |= 0x80;
    func_8004284C();
}

void func_8013DF14(void) {
    /* FAKE: pointer alias for increment/compare paired with direct global store in the if-body so uopt shares $v0 for lw/lbu/sb at +0x468/+0x469 without forwarding the byte into the clamp store. T-24088 */
    GameState *state;

    state = &D_800E6280;
    func_80085B3C(4, 0);
    state->unk_1BC[12].unk_0C.f.f9 += 1;
    if (state->unk_1BC[12].unk_0C.f.f9 >= 4) {
        D_800E6280.unk_1BC[12].unk_0C.f.f9 = 3;
    }
    func_800AE0F0(D_800CA188, "伊集院");
    func_800AE0F0(girl_name_buf, "伊集院");
    func_800AE0F0(D_800CA1DC, "パーティー会場");
    D_80145F2C = 9;
    func_8004284C();
    func_8013DD90();
    load_palette(D_80145FE0, 0x11, 1, 3, 0);
    func_80048F64(0x60);
    D_80120651 = 0xC;
    D_80120652 = 1;
    D_80120688 = 0x41000000;
    D_80120653 = 4;
    D_8012065C = (u8 *)D_80145FD4;
    D_80120660 = (u8 *)D_80145FD8;
    D_80120684 = (u8 *)D_80145FD0;
    D_80120664 = D_80145FDC;
    D_80120666 = 0;
    D_80120668 = 0;
    D_80120693 = 0x11;
    D_80120676 = -0xA0;
    D_8012067A = -0x78;
    D_80120657 = 0;
    D_80120655 = 0;
    D_80120654 = 8;
}

void func_8013E0C8(void) {
    bg_read_sub2(0x41D4);
    func_8004284C();
}

void func_8013E0F0(void) {
    D_80145F2C += D_800E6280.unk_1BC[12].unk_0C.f.f9 * 3 - 3;
    func_8004284C();
}

void func_8013E13C(void) {
    func_80062CD0(0x5855);
    func_8004284C();
}

void func_8013E164(void) {
    cd_read_sectors(0xD, 0x801A0000, 0xBD9F);
    func_8013DD90();
    func_8004284C();
}

void func_8013E19C(void) {
    s32 r;
    u8 a;
    u8 pad; /* FAKE: unused byte between a and b, real source unknown (frame layout) */
    u8 b;

    a = func_80051A68(7);
    r = func_80051B48(7);
    b = 0xFF;
    if (((Bit1Flag *)&D_800E6280.unk_1BC[7].unk_0C.w)->f && D_800E6280.unk_56C[52] == 0 && a == 0x80 && r == 0) {
        b = 7;
    }
    a = func_80051A68(0);
    r = func_80051B48(0);
    if (D_800E6280.unk_56C[6] == 0 && a == 0x80 && r == 0 && (b == 0xFF || (b == 7 && D_800E6280.unk_1BC[0].unk_0A < D_800E6280.unk_1BC[7].unk_0A))) {
        b = 0;
    }
    a = func_80051A68(9);
    r = func_80051B48(9);
    if (((Bit1Flag *)&D_800E6280.unk_1BC[9].unk_0C.w)->f && D_800E6280.unk_56C[66] == 0 && a == 0x80 && r == 0 &&
        (b == 0xFF || (b == 7 && D_800E6280.unk_1BC[9].unk_0A < D_800E6280.unk_1BC[7].unk_0A) || (b == 0 && D_800E6280.unk_1BC[9].unk_0A < D_800E6280.unk_1BC[0].unk_0A))) {
        b = 9;
    }
    D_80145FE4 = b;
    func_8004284C();
}
