#include "common.h"
#include "ovl/TAIIKU.h"

extern u8 D_80149208[]; /* 3 slots of 0x24 bytes; byte at +0x2A is read */
extern u8 D_8011ECD0[]; /* 3 records of 0x44 bytes; s16 at +0x19EA is read */
extern u8 D_80149232;
extern u8 D_80149256;
extern u8 D_8014927A;
extern u8 D_8014A13C[]; /* func_801446A0: same shape as D_80149208 */
extern u8 D_8014A161;
extern u8 D_8014A185;
extern u8 D_8014A1A9;
extern u8 D_8014A400[]; /* func_80146FA0: stride 0x1C */
extern u8 D_8014A401;
extern u8 D_8014A41D;
extern u8 D_8014A439;

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80132000);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80132970);

void func_80132B40(void) {
    D_80148EF0 = 0x801A19CC;
    D_80148EF4 = 0x8018F000;
    D_80148EF8 = 0x8019D000;
    D_80148EFC = 0x8019E190;
}

void func_80132B90(void) {
    D_80148F00 = 0x801F016C;
    D_80148F04 = 0x801F01A4;
    D_80148F08 = 0x801F0284;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80132BD0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80132E20);

void func_801330D4(void) {
    func_801330FC();
    func_801331D0();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801330FC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801331D0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80133258);

void func_80133438(void) {
    func_80132E20(0, 1);
    func_80085F0C(0, 0xF, 0, 0x70, 0x280);
    func_80085F0C(0, 0xF, 0x70, 0x80, 0x80);
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80133494);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80133644);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013384C);

void func_80133928(s32 arg0) {
    u8 *p;

    func_801339C4();
    p = D_8011ECD0 + arg0 * 0x44;
    if (*(s16 *)(p + 0x26) >= 0x90) {
        *(s16 *)(p + 0x26) = 0x90;
    }
    if (*(s16 *)(p + 0x26) < -0x9F) {
        *(s16 *)(p + 0x26) = -0xA0;
    }
    if (*(s16 *)(p + 0x2A) >= 0x68) {
        *(s16 *)(p + 0x2A) = 0x68;
    }
    if (*(s16 *)(p + 0x2A) < -0x67) {
        *(s16 *)(p + 0x2A) = -0x68;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801339C4);

void func_80133BF0(void) {
    D_801491B0 = 0x801E8564;
    D_801491B4 = 0x801E8694;
    D_801491B8 = 0x801E8BE0;
}

void func_80133C30(void) {
    D_801491C0 = 0x801C3000;
    D_801491C4 = 0x801C300C;
    D_801491C8 = 0x801C479C;
    D_801491CC = 0x801B3000;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80133C80);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80133CFC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80134060);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801345A4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801346FC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80134D94);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80134F98);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80135480);

void func_801356E0(void) {
    s32 i;

    func_8004E44C(0, D_80149FAC, D_80149FA0);
    func_8004E350(0, D_80149FA8, 0);
    func_8004D088(0, 5, 0x580, 0xF0);
    for (i = 0; i < 3; i++) {
        *(s32 *)(D_800E6280 + 0x123C + i * 4) = 8;
    }
    for (i = 0; i < 3; i++) {
        *(s32 *)(D_800E6280 + 0x12BC + i * 4) = 8;
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

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80135830);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80135AD0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80135D24);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80135DF8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80135F3C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801365C8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801368B8);

void func_801369F0(void) {
    func_80136A28();
    func_80136AB0();
    func_80136B18();
    func_80136BB4();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80136A28);

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

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80136B18);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80136BB4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80136DA4);

void func_80136E80(void) {
    func_80136EC0();
    func_8013703C();
    func_80137090();
    func_80137228();
    func_8013732C();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80136EC0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013703C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80137090);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80137228);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013732C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80137440);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80137544);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80137784);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80137C44);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80137EA8);

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

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80138224);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80138680);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80138800);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801388E4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80138950);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80138A34);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80138AA8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80138DF8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80139330);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80139550);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80139610);

void func_80139A20(void) {
    if ((func_8008667C(6) < 2) && (((u32) (D_800E6598 << 0x1E) >> 0x1F) == 1) && (D_80149940 == 6) && (D_800E6819 == 0)) {
        D_800E6819 = 1;
        func_80042808();
        return;
    }
    D_800E7389 = 0;
    D_800E738A = 0;
    D_80122ECC = 1;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80139AB0);

void func_80139DDC(void) {
    func_8004E44C(0, D_8014A0BC, D_8014A0B0);
    func_8004E350(0, D_8014A0B8, 0);
    func_8004D088(0, 5, 0x580, 0xF0);
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80139E34);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80139FE4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013A314);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013A63C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013A6FC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013ACBC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013AE14);

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

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013AF74);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013B0AC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013B104);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013B200);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013B3B0);

void func_8013B5C0(void) {
    if (D_801499A2 == 1) {
        func_80044750(0x800);
        return;
    }
    if ((u32)(u16)D_801499A2 >= 0x2D1) {
        D_801499A2 = 0;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013B608);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013B78C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013B94C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013BB3C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013BCEC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013BEF8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013C0E4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013C33C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013C4E4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013C5C8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013C694);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013C780);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013C978);

void func_8013CCFC(void) {
    func_8013CD34();
    func_8013CF88();
    func_8013D114();
    func_8013D2B4();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013CD34);

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

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013CF88);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013D114);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013D254);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013D2B4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013D788);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013D9B4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013DDB8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013E1FC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013E894);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013EC60);

void func_8013F35C(void) {
    s32 var_s0;

    var_s0 = 0;
    do {
        func_8013F3A4(var_s0);
        var_s0 += 1;
    } while (var_s0 != 4);
    func_8013F948();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013F3A4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013F948);

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

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013FC5C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013FE44);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8013FEF0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801400D8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80140338);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8014038C);

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

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801404DC);

void func_8014064C(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        func_80099E30(0, &D_801227A0[i]);
        D_801227A0[i].unk20 = -4000;
        D_801227A0[i].unk0 = 0;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801406A8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80140738);

void func_801407C0(void) {
    D_80149FA0 = 0x8019D000;
    D_80149FA4 = 0x8018F000;
    D_80149FA8 = 0x8019E218;
    D_80149FAC = 0x8019D008;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80140810);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80140A90);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80141280);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80141964);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801419F8);

void func_80141AE8(void) {
    D_80122EC8 = 9;
    D_800CA148 = 2;
    D_800CA14C = 0;
    func_8004E9F4(1);
}

void func_80141B28(void) {
    D_80122EC8 = 0xB;
    D_800CA148 = 1;
    D_800CA14C = 0;
    func_8004E9F4(1);
}

void func_80141B70(void) {
    D_8014A0A0 = 0x801E83F8;
    D_8014A0A4 = 0x801E8460;
    D_8014A0A8 = 0x801E874C;
}

void func_80141BB0(void) {
    D_8014A0B0 = 0x801A1000;
    D_8014A0B4 = 0x8018F000;
    D_8014A0B8 = 0x801A2198;
    D_8014A0BC = 0x801A1008;
}

void func_80141C00(void) {
    if ((func_8008667C(6) < 2) && (((u32) (D_800E6598 << 0x1E) >> 0x1F) == 1) && (D_800E6819 == 0)) {
        D_800E6819 = 1;
        func_80042808();
    } else {
        D_800E7389 = 0;
        D_800E738A = 0;
        D_80122ECC = 1;
    }
    func_80086424();
}

void func_80141C88(void) {
    s32 temp_v0;

    temp_v0 = func_80141CDC();
    if (temp_v0 != 0) {
        if (temp_v0 == 1) {
            D_800E7389 = 0;
            D_800E738A = 0;
            D_80122ECC = 1;
            return;
        }
        func_80046500();
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80141CDC);

void func_80142140(void) {
    D_8014A0D0 = 0x801E8294;
    D_8014A0D4 = 0x801E82C8;
    D_8014A0D8 = 0x801E8468;
}

void func_80142180(void) {
    D_8014A0E0 = 0x801B93F0;
    D_8014A0E4 = 0x801B9360;
    D_8014A0E8 = 0x801B93D8;
    D_8014A0EC = *(s16 *)0x801B93F8;
    D_8014A0F0 = 0x80197000;
    D_8014A0F4 = 0x80199000;
    D_8014A0F8 = 0x8019D000;
    D_8014A0FC = 0x801A1000;
    D_8014A100 = 0x801A5000;
    D_8014A104 = 0x801A9000;
    D_8014A108 = 0x801AD000;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80142240);

void func_801422BC(void) {
    if (D_800E738A == 0) {
        if ((func_8008667C(1) < 2) && (((u32) (D_800E6480 << 0x1E) >> 0x1F) == 1) && (D_800E67F9 == 0)) {
            D_800E67F9 = 1;
            D_8014A110 = 1;
            func_80044890(0, 0xBF98, 0xBF79, 0xD712, 0xD6C3, 0xD6B7);
            func_8004284C();
        } else {
            D_8014A110 = 0;
            func_80042908(3);
        }
        func_80086424();
        return;
    }
    if (func_80044E8C() == 1) {
        func_80042808();
    }
    func_80086424();
}

void func_801423A4(void) {
    s32 temp_v0;

    temp_v0 = func_80140A90();
    switch (temp_v0) {
    case 1:
        func_80042808();
        return;
    case 2:
        D_8014A110 = 0;
        func_80042808();
        /* fallthrough */
    case 0:
        return;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80142400);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8014280C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80142D90);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80142FB8);

void func_80143398(void) {
    s32 i;

    if ((u8) D_8014A110 == 0) {
        func_8004E44C(0, D_8014A384, D_8014A370);
        func_8004E350(0, D_8014A37C, 0);
    } else {
        func_8004E44C(0, D_8014A388, D_8014A374);
        func_8004E350(0, D_8014A380, 0);
    }
    func_8004D088(0, 5, 0x580, 0xF0);
    for (i = 0; i < 3; i++) {
        *(s32 *)(D_800E6280 + 0x123C + i * 4) = 8;
    }
    for (i = 0; i < 3; i++) {
        *(s32 *)(D_800E6280 + 0x12BC + i * 4) = 8;
    }
}

void func_80143470(void) {
    func_80048F64(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120652 = 1;
    D_80120653 = 0xA4;
    D_80120657 = 0x80;
    D_8012065C = (u8 *) D_80149FC0;
    D_80120660 = (u8 *) D_80149FF8;
    D_80120684 = &D_8014A1FC;
    D_80120664 = D_8014A030;
    D_80120654 = 0;
    D_80120655 = 0x80;
    if ((u8) D_8014A110 == 0) {
        D_80120666 = 0;
    } else {
        D_80120666 = 2;
    }
    D_80120676 = 0x50;
    D_8012067A = 0xB0;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8014357C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80143814);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80143AC8);

void func_80143C04(void) {
    if ((D_80120778 != 0x2D) && (D_80120778 != 0x39)) {
        func_80143C84();
        func_80143CC0();
        func_80143D24();
        func_80143DD0();
        func_80143F50();
        return;
    }
    if ((u32)D_8014A154 >= 0x32) {
        D_80120778 = 0x2F;
    }
}

void func_80143C84(void) {
    if (D_8014A158 != 0) {
        D_8014A144 = D_8014A128;
        return;
    }
    D_8014A144 = -D_8014A128;
}

void func_80143CC0(void) {
    if ((u32)D_8014A14C >= 0xB) {
        D_8014A148 += D_8014A144;
        if (D_8014A148 < -0x4000) {
            D_8014A148 = -0x4000;
        }
        if (D_8014A12C < D_8014A148) {
            D_8014A148 = D_8014A12C;
        }
        D_8014A14C = 0;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80143D24);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80143DD0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80143F50);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8014430C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80144464);

/* FAKE: v[6] as in func_8013815C. T-0016 */
s32 func_801446A0(void) {
    s16 v[6];
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_8014A13C[i * 0x24 + 0x25] == 0) {
            v[i] = *(s16 *)(D_8011ECD0 + i * 0x44 + 0x19EA);
        } else {
            v[i] = -0xF00;
        }
    }
    if (v[0] >= v[1] && v[0] >= v[2]) {
        D_8014A161 = 1;
        return 2;
    }
    if (v[1] >= v[2]) {
        D_8014A185 = 1;
        return 3;
    }
    D_8014A1A9 = 1;
    return 4;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80144768);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80144834);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801448D4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801449A0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80144B40);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80144C70);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80144E54);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80145044);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801450C0);

void func_80145148(void) {
    if ((u32)D_8014A11C >= 0xF) {
        *(s16 *)(D_8011ECD0 + 0x19DC) ^= 1;
        *(s16 *)(D_8011ECD0 + 0x1A20) ^= 1;
        D_8014A11C = 0;
    }
}

void func_80145190(void) {
    D_8014A370 = 0x801AD000;
    D_8014A374 = 0x801AE018;
    D_8014A378 = 0x8018F000;
    D_8014A37C = 0x801AF1B0;
    D_8014A380 = 0x801B29EC;
    D_8014A384 = 0x801AD008;
    D_8014A388 = 0x801AE020;
}

void func_80145210(void) {
    D_8014A390 = 0x801B523C;
    D_8014A394 = 0x801B518C;
    D_8014A398 = 0x801B51DC;
    D_8014A39C = *(s16 *)0x801B5240;
    D_8014A3A0 = 0x80197000;
    D_8014A3A4 = 0x80199000;
    D_8014A3A8 = 0x8019D000;
    D_8014A3AC = 0x801A1000;
    D_8014A3B0 = 0x801A5000;
    D_8014A3B4 = 0x801A9000;
    D_8014A3B8 = 0x801AD000;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801452D0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80145370);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80145640);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80145A78);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80145C28);

void func_80145EF4(void) {
    s32 i;

    func_8004E44C(0, D_80148EF8, D_80148EF0);
    func_8004E350(0, D_80148EFC, 0);
    func_8004D088(0, 5, 0x580, 0xF0);
    for (i = 0; i < 3; i++) {
        *(s32 *)(D_800E6280 + 0x123C + i * 4) = 8;
    }
    for (i = 0; i < 3; i++) {
        *(s32 *)(D_800E6280 + 0x12BC + i * 4) = 8;
    }
}

void func_80145F8C(void) {
    func_80048F64(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120652 = 1;
    D_80120653 = 0xA4;
    D_80120657 = 0x80;
    D_8012065C = (u8 *) D_80149FD0;
    D_80120660 = (u8 *) D_8014A008;
    D_80120684 = &D_8014A4F4;
    D_80120664 = D_8014A040;
    D_80120654 = 0;
    D_80120655 = 0x80;
    D_80120666 = 0;
    D_80120676 = 0x20;
    D_8012067A = 0x10;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8014607C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80146338);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_8014666C);

void func_8014684C(void) {
    func_80146884();
    func_8014692C();
    func_80146994();
    func_80146A60();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80146884);

void func_8014692C(void) {
    if ((u32)D_8014A3F0 >= 0xB) {
        D_8014A3F8 += D_8014A3F4;
        if (D_8014A3F8 < -0x10000) {
            D_8014A3F8 = -0x10000;
        }
        if (D_8014A3D4 < D_8014A3F8) {
            D_8014A3F8 = D_8014A3D4;
        }
        D_8014A3F0 = 0;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80146994);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80146A60);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80146B8C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80146C20);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80146D18);

/* FAKE: v[6] as in func_8013815C. T-0016 */
s32 func_80146FA0(void) {
    s16 v[6];
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_8014A400[i * 0x1C + 1] == 0) {
            v[i] = *(s16 *)(D_8011ECD0 + i * 0x44 + 0x19EA);
        } else {
            v[i] = -0xF00;
        }
    }
    if (v[0] >= v[1] && v[0] >= v[2]) {
        D_8014A401 = 1;
        return 2;
    }
    if (v[1] >= v[2]) {
        D_8014A41D = 1;
        return 3;
    }
    D_8014A439 = 1;
    return 4;
}

void func_80147068(void) {
    func_801470A0();
    func_80147120();
    func_80147210();
    func_80147318();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801470A0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80147120);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80147210);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80147318);

void func_80147520(void) {
    s32 var_s0;

    func_8014756C();
    var_s0 = 0;
    do {
        func_80147A6C(var_s0);
        func_80147B14(var_s0);
        var_s0 += 1;
    } while (var_s0 != 4);
}

void func_8014756C(void) {
    func_80147594();
    func_80147928();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80147594);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80147928);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80147A6C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80147B14);

void func_80147F10(void) {
    s32 var_s0;

    var_s0 = 0;
    do {
        func_80147F58(var_s0);
        func_801482BC(var_s0);
        var_s0 += 1;
    } while (var_s0 != 4);
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80147F58);

void func_80148194(s32 arg0) {
    u8 *temp_v0;
    u8 *temp_v1;

    temp_v1 = (arg0 * 0x1C) + D_8014A454;
    temp_v0 = (arg0 * 0x44) + D_8011ECD0;
    *(u8 *)(temp_v1 + 0xC) = 1;
    *(u8 *)(temp_v1 + 0xD) = 0;
    *(s16 *)(temp_v0 + 0x1998) = (s16) ((arg0 * 2) + 0x51);
    *(s16 *)(temp_v0 + 0x1AB6) = (s16) (*(s16 *)(temp_v0 + 0x19A6) + 0x74);
    *(s16 *)(temp_v0 + 0x1ABA) = (s16) (*(s16 *)(temp_v0 + 0x19AA) + 0x32);
    *(s16 *)(temp_v1 + 0x2) = 0x6A;
    *(s16 *)(temp_v1 + 0x6) = 0x32;
    if (arg0 == 0) {
        func_80044750(0x503);
    }
}

void func_80148228(s32 arg0) {
    u8 *temp_v0;
    u8 *temp_v1;

    temp_v1 = (arg0 * 0x1C) + D_8014A454;
    temp_v0 = (arg0 * 0x44) + D_8011ECD0;
    *(u8 *)(temp_v1 + 0xC) = 2;
    *(u8 *)(temp_v1 + 0xD) = 0;
    *(s16 *)(temp_v0 + 0x1998) = (s16) ((arg0 * 2) + 0x52);
    *(s16 *)(temp_v0 + 0x1AB6) = (s16) (*(s16 *)(temp_v0 + 0x19A6) - 0x17);
    *(s16 *)(temp_v0 + 0x1ABA) = (s16) (*(s16 *)(temp_v0 + 0x19AA) + 0x32);
    *(s16 *)(temp_v1 + 0x2) = -0x17;
    *(s16 *)(temp_v1 + 0x6) = 0x32;
    if (arg0 == 0) {
        func_80044750(0x503);
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801482BC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80148370);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801485C4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80148678);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801487B4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_80148820);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU", func_801488F0);
