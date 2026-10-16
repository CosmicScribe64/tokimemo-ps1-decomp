#include "common.h"
#include "ovl/TACO.h"

void func_80132220(void) {
    D_8015EDB4->unk74 = 0;
    D_8015EDB4->unk76 = 0;
    D_8015EDB4->unk78 = 0x800;
    D_8015EDB4[8].unk74 = 0;
    D_8015EDB4[8].unk76 = 0;
    D_8015EDB4[8].unk78 = 0x800;
    D_8015EDB4->unk2 = 0;
    D_8015EDB4[8].unk2 = 0;
    D_8015EDB4->unk3 = 0;
    D_8015EDB4->unk68 = 0;
    D_8015EDB4[8].unk68 = 0;
}

void func_801322AC(void) {
    func_801438F0(0x8019C800, 0, D_8015EDB4);
    D_8015EDB4->unk0[1] = 0x80;
    D_8015EDB4->unk6C = 0x400;
    D_8015EDB4->unk6E = 0x400;
    D_8015EDB4->unk70 = 0x400;
    func_801438F0(0x8019C800, 1, &D_8015EDB4[8]);
    D_8015EDB4[8].unk0[1] = 0x80;
    D_8015EDB4[8].unk6C = 0x400;
    D_8015EDB4[8].unk6E = 0x400;
    D_8015EDB4[8].unk70 = 0x400;
    D_8015EDB4->unk7A = 0;
    D_8015EDB4[8].unk7A = 0;
    D_8015EDB4->unk72 = -1;
    D_8015EDB4[8].unk72 = -1;
    D_8015EDB4->unk82 = -1;
    D_8015EDB4[8].unk82 = -1;
    D_8015EDB4->unk7C = 4;
    D_8015EDB4->unk7E = 3;
    D_8015EDB4->unk80 = 0;
    D_8015EDB4->unk84[1] = 0xFF;
    D_8015EDB4[8].unk84[1] = 0x80;
    func_80132220();
    D_8015EDD0 = 0;
    D_8015EDD4 = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_8013240C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_801328B0);

/* NON_MATCHING: T-19025, 10 differing lines; the two hoisted lui 0x8016 ranges of D_8015EDE4 take $v0, the original $a0 (an interfering range holds $v0 in the original), which also moves the hoisted move a0,zero */
#ifdef NON_MATCHING
void func_80132FE8(s32 arg0, s32 arg1) {
    /* FAKE: pad1..pad4 are unused locals that reserve the original's four extra frame words (flags at sp+0x30, actor at sp+0x2C); real source unknown. T-19025 */
    s32 pad1;
    s32 pad2;
    s32 pad3;
    s32 flags;
    TcActor *actor;
    s32 pad4;
    s32 m80;
    s32 m40;
    s32 m20;

    actor = D_8015EDB4;
    if (actor->unk2 & 0x40) {
    } else {
        func_8013240C(arg0, arg1);
        func_801328B0();
        func_80140000();
        func_80140DF0();
        flags = 0;
        if (D_8015EDD8 != 0) {
            if ((D_800E6280.unk_F80 & 0x20) && (D_8015EDE0 % 3 == 0) && !(D_800E6280.unk_10F8 & 3)) {
                flags = 0x20;
            }
            if ((D_800E6280.unk_F80 & 0x40) && (D_8015EDE0 % 3 == 1) && !(D_800E6280.unk_10F8 & 3)) {
                flags |= 0x20;
            }
            if ((D_800E6280.unk_F80 & 0x80) && (D_8015EDE0 % 3 == 2) && !(D_800E6280.unk_10F8 & 3)) {
                flags |= 0x20;
            }
            m80 = D_800E6280.unk_F88 & 0x80;
            m40 = D_800E6280.unk_F88 & 0x40;
            m20 = D_800E6280.unk_F88 & 0x20;
        } else {
            m20 = D_800E6280.unk_F88 & 0x20;
            m40 = D_800E6280.unk_F88 & 0x40;
            if (m20 && (D_8015EDE0 % 3 == 0)) {
                flags = 0x20;
            }
            if (m40 && (D_8015EDE0 % 3 == 1)) {
                flags |= 0x20;
            }
            m80 = D_800E6280.unk_F88 & 0x80;
            if (m80 && (D_8015EDE0 % 3 == 2)) {
                flags |= 0x20;
            }
        }
        if (m40 && (D_8015EDE4 % 3 == 0)) {
            flags |= 0x40;
        }
        if (m80 && (D_8015EDE4 % 3 == 1)) {
            flags |= 0x40;
        }
        if (m20 && (D_8015EDE4 % 3 == 2)) {
            flags |= 0x40;
        }
        if (actor->unk72 == 0) {
            func_80141170(0, flags);
        } else if (actor->unk72 == 1) {
            func_80141290(0, flags);
        } else {
            func_80141050(0, flags);
        }
        if (actor->unk82 == 0) {
            func_801404FC(0, flags);
        } else if (actor->unk82 == 1) {
            func_80140364(0, flags);
        }
    }
    func_80141DE0(flags);
    func_8013B990();
    func_8013D348();
    func_8013D3F4();
    func_8013DDE4();
    func_8013C6D4();
    func_8013E4FC();
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_80132FE8);
#endif

void func_80133374(void) {
    if (D_8015EDCC < 3U) {
        D_8015EDEC -= 1;
    }
    if (D_8015EDEC == 0) {
        func_80042908(0x2D);
    }
    if (D_800E6280.unk_F88 & 0x800) {
        func_80042908(0x2D);
    }
    if (D_8015EDB4->unk84[1] == 0) {
        func_80042908(0x2D);
    }
}

void func_80133410(s32 arg0) {
    s32 i;

    if (arg0 == 0) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 16; i++) D_8015E270[i].unk13 &= ~0x20;
    } else {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 16; i++) D_8015E3B0[i].unk13 &= ~0x20;
    }
}

void func_801334BC(void) {
    s32 i;

    D_80160460 = 0;
    D_80160464 = 0; for (i = 0; i < 16; i++) { /* FAKE: store and loop head on one source line, as1 then schedules the store after the pointer setup */
        D_8015E270[i].unk13 = 0;
        D_8015E3B0[i].unk13 = 0;
        D_8015E4F0[i].unk13 = 0;
        D_8015E630[i].unk13 = 0;
        D_8015E770[i].unk13 = 0;
    }
}

void func_8013356C(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        D_8015EB30[i].unk13 = 0;
    }
}
