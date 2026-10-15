#include "common.h"
#include "ovl/SHUGAKU.h"

void func_80135430(void) {
    D_8013C2D0 = 0x801C25B8;
    D_8013C2D4 = 0x801C25C8;
    D_8013C2D8 = 0x801C2674;
    D_8013C2DC = *(s16 *)0x801C26A0;
    D_8013C2E0 = 0x801B0000;
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_80135480);

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

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7368;
extern s8 D_800E62BA;
extern s8 D_800E71DF;
extern s8 D_800E7322;
/* end bulk_m2c declarations */
void func_80135630(void) {
    s16 sp32;
    s16 sp30;
    s16 sp2E;
    s16 sp2C;

    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_8006BD6C(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    func_8008585C();
    k_reset(1);
    sp2C = 0x140;
    sp2E = 0x1A0;
    sp30 = 0x140;
    sp32 = 0x58;
    func_8009C7F8((RECT *) &sp2C, 8, 0, 0);
    sp2C = 0x2C0;
    sp2E = 0xA0;
    func_8009C7F8((RECT *) &sp2C, 8, 0, 0);
    D_800E7322 = 0;
    D_800E7368 = 1;
    D_800E71DF = 2;
    func_800847B8(2U);
    addr_init_bustup();
    D_800E62BA = 0x80;
    D_800B593C = 0;
    hizuke_init();
    message_window_init();
    func_80084E4C();
    D_8013C2E4 = 0;
    D_8013C2E8 = 0;
    D_8013C2EC = (u8) D_80122CE0;
    D_80122CE0 = 0xFF;
    func_801365F4();
    D_8013BFFC = D_8013C508;
    D_8013C000 = D_8013C644;
    D_8013C004 = D_8013C780;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_801357AC);

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
    D_80120650[3] |= 0x80;
    *(s16 *) &D_80120650[0x16] = 8;
    D_80120650[0x47] |= 0x80;
    *(s16 *) &D_80120650[0x5A] = 7;
    D_80120650[0x8A] = 0x40;
    *(s16 *) &D_80120650[0xA0] = 0x14;
    func_8004284C();
}

u32 func_80135B68(void) {
    D_80120650[3] &= ~0x80;
    D_80120650[0x47] &= ~0x80;
    D_80120650[0x8A] = 5;
    *(s16 *)&D_80120650[0x9E] = 4;
    *(s16 *)&D_80120650[0xA0] = 0;
    return ~0x80;
}

void func_80135BB8(void) {
    D_80120650[3] |= 0x80;
    *(s16 *) &D_80120650[0x16] = 3;
    D_80120650[0x47] |= 0x80;
    *(s16 *) &D_80120650[0x5A] = 1;
    D_80120650[0x8A] = 0x40;
    *(s16 *) &D_80120650[0xA0] = 0x16;
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

typedef struct {
    s32 w[17];
} Blk44; /* size 0x44 */

void func_80135CF8(void) {
    D_8013C2E4 = 1;
    *(Blk44 *) &D_80120650[0xCC] = *(Blk44 *) D_80120650;
    D_80120650[0xCD] = 0xB;
    D_80120650[0xCF] |= 0x80;
    *(s16 *) &D_80120650[0xE2] = 5;
    D_80120650[0xCE] = 9;
    *(s16 *) &D_80120650[0xF2] = -0x60;
    *(s16 *) &D_80120650[0xF6] = -0x48;
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

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_80135EA4);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_801360B0);
