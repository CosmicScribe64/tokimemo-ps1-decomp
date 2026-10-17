#include "common.h"
#include "ovl/SHUGAKU.h"

void func_80135430(void) {
    D_8013C2D0 = 0x801C25B8;
    D_8013C2D4 = 0x801C25C8;
    D_8013C2D8 = 0x801C2674;
    D_8013C2DC = *(s16 *)0x801C26A0;
    D_8013C2E0 = 0x801B0000;
}

/* s32 with no return (v0 live at the exit): the original keeps the dead `li v0,-129` of the last `&= ~0x80`. T-22014 */
s32 func_80135480(void) {
    u8 index;
    ShFnTbl74 table;

    table = D_8013C2F0;
    index = D_800E6280.unk_110A;
    table.f[index](0x80);
    index = D_800E6280.unk_110A;
    if (D_80085368 == table.f[index]) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_03 |= 0x80;
    }
    if ((func_800853FC == table.f[index] || func_80135A80 == table.f[index]) && D_800B593C < 8) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_03 &= ~0x80;
        ((DisplayActor44 *)D_8011ECD0)[99].unk_03 &= ~0x80;
    }
    if ((index >= 0x16 && index < 0x1C) || (index >= 0x20 && index < 0x25)) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_07 = ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
    }
    if (D_8013C2E4 != 0) {
        func_801360B0();
    }
}

void func_801355D4(void) {
    if (D_8013C2EC != 0) {
        func_80139018();
        return;
    }
    func_80135CC8();
}

void func_80135610(void) {
    func_80042940(0x1F);
}

void func_80135630(void) {
    s32 pad; /* FAKE: unused scalar puts the RECT at +0x2C, as in GEKO func_8013B3C0 (T-9770); original local unknown. T-18000 */
    RECT r;

    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_8006BD6C(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    func_8008585C();
    func_8004E9F4(1);
    r.x = 0x140;
    r.y = 0x1A0;
    r.w = 0x140;
    r.h = 0x58;
    func_8009C7F8(&r, 8, 0, 0);
    r.x = 0x2C0;
    r.y = 0xA0;
    func_8009C7F8(&r, 8, 0, 0);
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    /* FAKE: byte store then member read shares the constant in a0, as in GEKO func_8013B3C0 (T-9770). T-18000 */
    *(u8 *)&D_800E6280.unk_F5F = 2;
    func_800847B8(D_800E6280.unk_F5F);
    func_8007C740();
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    D_8013C2E4 = 0;
    D_8013C2E8 = 0;
    D_8013C2EC = D_80122CE0;
    D_80122CE0 = 0xFF;
    func_801365F4();
    D_8013BFFC = D_8013C508;
    D_8013C000 = D_8013C644;
    D_8013C004 = D_8013C780;
    func_8004284C();
}

#ifdef NON_MATCHING
/* NON_MATCHING: T-22014, 10 rows; ugen takes t8 for the end pointer of the block copy where the original takes t7 (one temporary fewer skipped; the known copy-state gap), every later t register follows. */
void func_801357AC(void) {
    s16 i;

    func_80135430();
    load_palette(D_8013C2E0, 0x11, 1, 2, 0);
    for (i = 0; i < 6; i++) {
        D_801217D0[58 + i].unk_08 = 0;
    }
    func_800850D4(D_8013C2D4, D_8013C2D8, D_8013C2D0, (s32) D_8013C2DC);
    ((DisplayActor44 *)D_8011ECD0)[97].unk_04 = 8;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 8;
    ((DisplayActor44 *)D_8011ECD0)[97].unk_01 = 0xB;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 0xB;
    ((DisplayActor44 *)D_8011ECD0)[98] = ((DisplayActor44 *)D_8011ECD0)[96];
    ((DisplayActor44 *)D_8011ECD0)[98].unk_02 = 0x40;
    ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0x14;
    make_color_bar16(D_800E6280.unk_163C, 0xFFFFFF, 0xFFFFFF, 0x10, 0);
    load_palette((s32) D_800E6280.unk_163C, 0x1F, 1, 3, 0);
    D_800E6280.unk_1228[0x25] = 1;
    func_80046754(0x801C4000, 0x25, 0x80);
    func_8004284C();
}
#else
INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_801357AC);
#endif

void func_80135938(void) {
    func_80044750(0x203);
    func_8004284C();
}

void func_80135960(void) {
    if (D_8013C2EC == 0) {
        func_80044750(0x202);
    }
    func_80042908(5);
}

void func_80135994(void) {
    D_80122CE0 = 7;
    func_80048EB8(0);
    func_8006BD6C(0);
    D_800E6280.unk_720 = D_800E6280.unk_1108;
    D_800E6280.unk_721 = D_800E6280.unk_1109;
    D_800E6280.unk_722 = 0x38;
    func_8004E9F4(0);
    main_state_set(0x81);
}

void func_80135A04(void) {
    cd_read_sectors(0x25, 0x801B0000, 0x8240);
    func_80135430();
    func_8004284C();
}

void func_80135A3C(void) {
    func_800853FC();
    D_80120657 = D_800B593C;
    D_8012069B = D_800B593C;
    D_801206DF = D_800B593C;
    D_80120723 = D_800B593C;
}

s32 func_80135A80(void) {
    if (D_8013C2E8 < 0xA0 || (u8)D_800B5BD4 != 9) {
        return 0;
    }
    func_800853FC();
    D_80120657 = D_800B593C;
    D_8012069B = D_800B593C;
    D_801206DF = D_800B593C;
    D_80120723 = D_800B593C;
}

/* One symbol for the whole 0x44-byte-entry table: as1 then keeps the later lbu below the earlier stores (T-2040, T-4020). */
void func_80135AF4(void) {
    ((DisplayActor44 *)D_80120650)->unk_03 |= 0x80;
    ((DisplayActor44 *)D_80120650)->unk_16 = 8;
    ((DisplayActor44 *)D_80120650)[1].unk_03 |= 0x80;
    ((DisplayActor44 *)D_80120650)[1].unk_16 = 7;
    ((DisplayActor44 *)D_80120650)[2].unk_02 = 0x40;
    ((DisplayActor44 *)D_80120650)[2].unk_18 = 0x14;
    func_8004284C();
}

u32 func_80135B68(void) {
    ((DisplayActor44 *)D_80120650)->unk_03 &= ~0x80;
    ((DisplayActor44 *)D_80120650)[1].unk_03 &= ~0x80;
    ((DisplayActor44 *)D_80120650)[2].unk_02 = 5;
    ((DisplayActor44 *)D_80120650)[2].unk_16 = 4;
    ((DisplayActor44 *)D_80120650)[2].unk_18 = 0;
    return ~0x80;
}

void func_80135BB8(void) {
    ((DisplayActor44 *)D_80120650)->unk_03 |= 0x80;
    ((DisplayActor44 *)D_80120650)->unk_16 = 3;
    ((DisplayActor44 *)D_80120650)[1].unk_03 |= 0x80;
    ((DisplayActor44 *)D_80120650)[1].unk_16 = 1;
    ((DisplayActor44 *)D_80120650)[2].unk_02 = 0x40;
    ((DisplayActor44 *)D_80120650)[2].unk_18 = 0x16;
}

void func_80135C14(void) {
    D_80120666 = 2;
    D_801206AA = 0;
    D_801206DA = 0x40;
    D_801206F0 = 0x14;
    func_8004284C();
}

void func_80135C5C(void) {
    D_8013C2E4 = 0;
    D_800B5BD4 = 0xF;
    func_8004284C();
}

void func_80135C8C(void) {
    func_80044750(0x203);
    bg_read_sub2(0x4761);
    func_80085B3C(2, 0x13);
    func_8004284C();
}

void func_80135CC8(void) {
    func_80044750(0x203);
    bg_read_sub2(0x4761);
    func_8004284C();
}

void func_80135CF8(void) {
    D_8013C2E4 = 1;
    *(DisplayActor44 *) &D_80120650[0xCC] = *(DisplayActor44 *) D_80120650;
    ((DisplayActor44 *)D_80120650)[3].unk_01 = 0xB;
    ((DisplayActor44 *)D_80120650)[3].unk_03 |= 0x80;
    ((DisplayActor44 *)D_80120650)[3].unk_16 = 5;
    ((DisplayActor44 *)D_80120650)[3].unk_02 = 9;
    ((DisplayActor44 *)D_80120650)[3].unk_26 = -0x60;
    ((DisplayActor44 *)D_80120650)[3].unk_2A = -0x48;
    func_8004284C();
}

void func_80135DB8(void) {
    if (D_8013C2EC == 0) {
        D_800E6280.unk_1BC[(s32) D_800E6280.unk_F5F].unk_02 += 1;
        D_800E6280.unk_1BC[(s32) D_800E6280.unk_F5F].unk_06 += 2;
        func_80084D3C();
    } else {
        D_800E6280.unk_1BC[(s32) D_800E6280.unk_F5F].unk_06 += 1;
        func_80084D3C();
        D_800CA2E4 += 2;
        D_800E6280.unk_110A += 0xB;
    }
    func_8004284C();
}

#ifdef NON_MATCHING
/* NON_MATCHING: T-22014, 10 rows; ugen takes t3 for the end pointer of the block copy where the original takes t2 (one more
 * temporary skipped; the known copy-state gap, same as func_801357AC), every later t register follows. */
void func_80135EA4(void) {
    s16 i;

    D_800E6280.unk_F5F = 2;
    D_8013C2E4 = 0;
    D_8013C2E8 = 0;
    D_800CA2F4 = 1;
    func_801365F4();
    D_800CA2E4 = 0;
    D_800CA2E8 = 0;
    D_8013BFFC = D_8013C508;
    D_8013C000 = D_8013C644;
    D_8013C004 = D_8013C780;
    func_80135430();
    load_palette(D_8013C2E0, 0x11, 1, 2, 0);
    D_80122D04 = 0;
    D_80122D0C = 0;
    D_80122D10 = 0;
    for (i = 0; i < 6; i++) {
        D_801217D0[58 + i].unk_08 = 0;
    }
    func_800850D4(D_8013C2D4, D_8013C2D8, D_8013C2D0, (s32) D_8013C2DC);
    ((DisplayActor44 *)D_8011ECD0)[97].unk_04 = 8;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 8;
    ((DisplayActor44 *)D_8011ECD0)[97].unk_01 = 0xB;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 0xB;
    ((DisplayActor44 *)D_8011ECD0)[98] = ((DisplayActor44 *)D_8011ECD0)[96];
    ((DisplayActor44 *)D_8011ECD0)[98].unk_02 = 0x40;
    ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0x14;
    make_color_bar16(D_800E6280.unk_163C, 0xFFFFFF, 0xFFFFFF, 0x10, 0);
    load_palette((s32) D_800E6280.unk_163C, 0x1F, 1, 3, 0);
    D_800E6280.unk_1228[0x25] = 1;
    func_80046754(0x801C4000, 0x25, 0x80);
    func_8004284C();
}
#else
INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_80135EA4);
#endif

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_801360B0);
