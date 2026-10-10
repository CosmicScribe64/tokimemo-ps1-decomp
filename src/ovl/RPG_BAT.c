#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80132000);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801321D0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80132348);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801328C4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80132A94);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80132B34);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80132CE4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80132DE8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80132EE0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80132FE0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80133108);

void func_801340F0(void) {
    switch (D_8015EDF0) {
    case 1:
        func_8013415C();
        return;
    case 2:
        func_80134570();
        return;
    case 3:
        func_80134984();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013415C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80134570);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80134984);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80134A70);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80134D30);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80134E78);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80135358);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801359FC);

void func_80135AE0(void) {
    switch (D_8015EDF0) {
    case 1:
        func_80135B4C();
        return;
    case 2:
        func_80135D90();
        return;
    case 3:
        func_80135E80();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80135B4C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80135D90);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80135E80);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80136070);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801361AC);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80136374);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80136530);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013673C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801368B0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80136A9C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80136BD8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80136CCC);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80136D50);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80136E2C);

void func_80137148(s32 arg0, s32 arg1) {
    if (arg1 == 7) {
        if (!(D_8015EC74 & 1)) {
            func_8013E97C(arg0, func_8013E9A8(0x10) + 0x108, 0x40);
        } else {
            func_8013E97C(arg0, func_8013E9A8(0x10) + 0x108, 0x58);
        }
        D_8015EC74 += 1;
    } else {
        func_8013E97C(arg0, func_8013E9A8(0x10) + 0x108, 0x40);
    }
    func_8013E810(arg0, arg1, 5, 1);
}

void func_80137204(void) {
    s32 var_s1;
    u8 *var_s0;

    var_s0 = D_8011ECD0;
    var_s1 = 0;
    do {
        *(s16 *)(var_s0 + 0x1ABA) = (s16) (*(s16 *)(var_s0 + 0x1ABA) + 1);
        if (!(*(u8 *)(var_s0 + 0x1A92) & 1)) {
            func_8013E7C0(var_s1 + 4, 0xFF, 1, 0);
        }
        var_s1 += 1;
        var_s0 += 0x44;
    } while (var_s1 != 0xC);
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80137280);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801373A8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801374D0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801375F8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801376D4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013794C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80137AC8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80137C44);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80137E74);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80137F50);

void func_80138120(void) {
    switch (D_8015EDD8) {
    case 0:
        func_80137280();
        return;
    case 1:
        func_8014EF8C();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80138170);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80138324);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801387F0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80138B9C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80138F6C);

void func_80139460(void) {
    switch (D_8015EDD8) {
    case 0:
        D_8015ED98 = 0x1000;
        func_8013E7C0(0x30, 7, 1, 1);
        func_8004500C(1, 0x203);
        func_8014EF3C();
        return;
    case 1:
        func_8014F080(0x78);
        return;
    case 2:
        func_801373A8();
        return;
    case 3:
        D_8015EC14 = 1;
        func_80042808();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80139510);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80139E88);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013A034);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013A2F4);

void func_8013A464(s8 a0, s8 a1) {
    D_80121353 = a0;
    D_80121317 = a1;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013A480);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013A650);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013A98C);

void func_8013ACA4(void) {
    s32 temp_v0;

    func_8013F0F4(0x507, 0, 2);
    func_8013E7C0(0x2E, 0xA, 1, 1);
    func_8013E97C(0x2E, -D_8015EDB4 * 8, 0);
    temp_v0 = D_8015EDB4 + 1;
    D_8015EDB4 = temp_v0;
    if (temp_v0 >= 0x27) {
        D_80121685 = 5;
        func_8013E7C0(0x2E, 0xA, 1, 0);
        func_8014EBA8();
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013AD50);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013AF1C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013B2F0);

void func_8013B534(void) {
    switch (D_8015EDB0) {
    case 0:
        func_8013E7C0(0x30, 6, 5, 1);
        func_8014EBD0();
        break;
    case 1:
        if (func_8013E90C(0x30) != 0) {
            func_8014EBA8();
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013B5B0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013B910);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013BBF8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013BEA8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013C1CC);

void func_8013C2A0(void) {
    if (D_8015EE1C == 1) {
        switch (D_8015EB98) {
        case 0x100000:
            if (D_8015EDC4 & 0x100000) {
                func_80139510();
                return;
            }
            func_801596E8();
            return;
        case 0x8000000:
            func_8013F82C();
            break;
        }
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013C32C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013C5D0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013C890);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013CC90);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013D030);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013D0D4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013D1D0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013D290);

void func_8013D350(void) {
    if (D_8015EDC4 & 0x100000) {
        func_8013EA60(0, 0, D_8015EBB0, 1);
        func_8013EA60(1, 1, D_8015EBB4, 1);
        func_8013EA60(2, 2, D_8015EBB8, 1);
        return;
    }
    func_8013EA60(0, D_8015EBAC, D_8015EBB0, 1);
}

void func_8013D3E4(void) {
    func_8013EA60(0, D_8015EBAC, D_8015EBB0, 1);
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013D418);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013D59C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013D780);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013D834);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013DC60);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013E300);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013E4D0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013E54C);

void func_8013E7C0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 *temp_v0;

    temp_v0 = D_8011ECD0 + arg0 * 0x44;
    *(s16 *)(temp_v0 + 0x1988) = 0;
    *(u8 *)(temp_v0 + 0x1982) = arg2;
    if (arg3 == 1) {
        *(u8 *)(temp_v0 + 0x1983) = 0x80;
    } else {
        *(u8 *)(temp_v0 + 0x1983) = 0;
    }
    *(s16 *)(temp_v0 + 0x1998) = 0;
    if (arg1 != 0xFF) {
        *(s16 *)(temp_v0 + 0x1996) = arg1;
    }
}

void func_8013E810(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 *temp_v0;

    temp_v0 = D_8011ECD0 + arg0 * 0x44;
    *(s16 *)(temp_v0 + 0x1988) = 0;
    *(u8 *)(temp_v0 + 0x1982) = arg2;
    if (arg3 == 1) {
        *(u8 *)(temp_v0 + 0x1983) = 0x84;
    } else {
        *(u8 *)(temp_v0 + 0x1983) = 4;
    }
    *(s16 *)(temp_v0 + 0x1998) = 0;
    if (arg1 != 0xFF) {
        *(s16 *)(temp_v0 + 0x1996) = arg1;
    }
}

void func_8013E864(s32 arg0, s32 arg1, s32 arg2) {
    u8 *temp_v0;

    temp_v0 = D_8011ECD0 + arg0 * 0x44;
    *(s16 *)(temp_v0 + 0x8) = 0;
    *(u8 *)(temp_v0 + 0x2) = 1;
    if (arg2 == 1) {
        *(u8 *)(temp_v0 + 0x3) = 0x80;
    } else {
        *(u8 *)(temp_v0 + 0x3) = 0;
    }
    *(s16 *)(temp_v0 + 0x18) = 0;
    if (arg1 != 0xFF) {
        *(s16 *)(temp_v0 + 0x16) = arg1;
    }
}

void func_8013E8B8(s32 arg0, s32 arg1, s32 arg2) {
    u8 *temp_v0;

    temp_v0 = D_8011ECD0 + arg0 * 0x44;
    *(s16 *)(temp_v0 + 0x1108) = 0;
    *(u8 *)(temp_v0 + 0x1102) = 1;
    if (arg2 == 1) {
        *(u8 *)(temp_v0 + 0x1103) = 0x80;
    } else {
        *(u8 *)(temp_v0 + 0x1103) = 0;
    }
    *(s16 *)(temp_v0 + 0x1118) = 0;
    if (arg1 != 0xFF) {
        *(s16 *)(temp_v0 + 0x1116) = arg1;
    }
}

s32 func_8013E90C(s32 i) {
    return (*(&D_80120652 + i * 0x44) & 1) == 0;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013E934);

void func_8013E97C(s32 i, s32 x, s32 y) {
    u8 *p = D_8011ECD0 + i * 0x44;
    *(s16 *)(p + 0x19A6) = x - 0xA0;
    *(s16 *)(p + 0x19AA) = y - 0x78;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013E9A8);

s32 func_8013EA00(s32 arg0, s32 arg1) {
    s32 a = func_8013E9A8(arg0);
    s32 b = func_8013E9A8(arg1);
    return a + b;
}

void func_8013EA30(s32 arg0) {
    s32 i;

    for (i = 0; i < 96; i++) {
        D_8011ECD0[0x1107 + i * 0x44] = arg0;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013EA60);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013EEAC);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013EFD0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013F0F4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013F15C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013F1C4);

void func_8013F220(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        D_8015ED64[i][0] = 0;
        D_8015ED64[i][1] = 0;
        D_8015ED64[i][2] = 0;
        D_8015ED64[i][3] = 0;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013F250);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013F350);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013F3F0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013F4F0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013F5C8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013F694);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013F760);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013F82C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013FD8C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8013FF28);

void func_801401D0(s32 i) {
    u8 *p = D_8011ECD0 + i * 0x44;
    *(s16 *)(p + 0x1AB6) += 1;
    *(s16 *)(p + 0x1ABA) -= 1;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80140204);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80140A58);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80140CC8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80140E70);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80141060);

void func_80141A64(void) {
    D_80120DC1 = 1;
    func_8013E7C0(0x1F, 0, 1, 1);
    func_8013E7C0(0x26, 1, 1, 1);
    func_8013E7C0(0x23, 4, 1, 0);
    D_80120AB6 = 0x6B;
    D_80120ABA = 0x3B;
    D_80120BC6 = 0x6B;
    D_80120BCA = 0x4B;
    D_80120CD6 = 0x6B;
    D_80120CDA = 0x5B;
    D_80120AFA = 0x74;
    D_80120AFE = 0x3B;
    D_80120C0A = 0x74;
    D_80120C0E = 0x4B;
    D_80120D1A = 0x74;
    D_80120D1E = 0x5B;
    D_80120B3E = 0x7D;
    D_80120B42 = 0x3B;
    D_80120C4E = 0x7D;
    D_80120C52 = 0x4B;
    D_80120D5E = 0x7D;
    D_80120D62 = 0x5B;
    D_80120B82 = 0x86;
    D_80120B86 = 0x3B;
    D_80120C92 = 0x86;
    D_80120C96 = 0x4B;
    D_80120DA2 = 0x86;
    D_80120DA6 = 0x5B;
    D_80120F3A = 0x58;
    D_80120F3E = 0x50;
    D_80120F2A = 8;
    D_80120EF6 = 0x58;
    D_80120EFA = 0x60;
    D_80120EE6 = 8;
}

void func_80141C30(void) {
    if (func_80156870() != 0) {
        func_80141C70();
        return;
    }
    func_80141E38();
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80141C70);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80141E38);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80142000);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801420EC);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801421E0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80142544);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801426B0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801427E4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80142A30);

void func_80142CB8(void) {
    switch (D_8015EDB0) {
    case 0:
        func_8013E97C(0x2A, 0, 0);
        func_8013E7C0(0x2A, 0xF, 5, 1);
        func_8014EBD0();
        return;
    case 1:
        if (!(D_8012117A & 1)) {
            func_8013F0F4(0x505, 0xF, 5);
            func_8013E7C0(0x2A, 0x10, 5, 1);
            D_8015EDB4 += 1;
            func_8014EBD0();
            return;
        }
        return;
    case 2:
        if (D_8015EDB4 < 0x32) {
            func_8013F0F4(0x505, 0xF, 5);
            if (!(D_8012117A & 1)) {
                func_8013E7C0(0x2A, 0x10, 5, 1);
                D_8015EDB4 += 1;
                return;
            }
        } else {
            func_8013E7C0(0x2A, 0xFF, 1, 0);
            func_8014EBA8();
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80142DF4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80142F70);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80143830);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801439E0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80143C28);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80143CE8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80143DA8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80143ECC);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80143F68);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80144010);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80144480);

s32 func_801445E8(s32 arg0) {
    if (arg0 == 0) {
        return 0x78;
    }
    if (D_8015EDC4 & 0x80000000) {
        return func_8013EA00(0x1E, 0x1E) + 0x1E;
    }
    return func_8013EA00(0x28, 0x1E) + 0x3C;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80144640);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801446F8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801447B8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801447E0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80144B7C);

s32 func_80144C54(void) {
    if (++D_8015EDD4 < 3) {
        return 1;
    }
    return 2;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80144C84);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80145180);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801452C0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801453B4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801454F0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80145CC0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80146578);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801468B0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014698C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80146AB4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80146C30);

void func_801477E0(void) {
    s32 temp_t4;
    s32 var_a0;

    switch (D_8015EB9C) {
    case 2:
        D_8015EBB0 = func_8013EA00(0x4E2, 0x2EE) + 1;
        return;
    case 5:
        D_8015EBB0 = func_8013EA00(0x7D0, 0x3E8) + 1;
        return;
    case 3:
        if (D_8015EDC4 & 0x600000) {
            D_8015EBB0 = func_8013EA00(0x3E8, 0x1F4) + 1;
        } else {
            D_8015EBB0 = func_8013EA00(0x258, 0x12C) + 1;
            D_8015EBB4 = func_8013EA00(0x258, 0x12C) + 1;
            D_8015EBB8 = func_8013EA00(0x258, 0x12C) + 1;
        }
        temp_t4 = (s32) D_8015ED94 / 4;
        var_a0 = temp_t4;
        if (temp_t4 >= 0x65) {
            var_a0 = 0x64;
        }
        D_8015EBBC = func_8013EA00(var_a0, var_a0);
        return;
    case 4:
        D_8015EBB0 = func_8013EA00(0x4B, 0x7D) + 1;
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80147930);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80147F10);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801480F0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801481C8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801484E0);

void func_80148654(void) {
    switch (D_8015EDB0) {
    case 0:
        func_8013E7C0(0x34, 4, 5, 1);
        func_8013F0F4(0x506, 1, 0);
        func_8014EBD0();
        break;
    case 1:
        if (!(D_80121422 & 1)) {
            func_8013E7C0(0x34, 4, 5, 1);
            D_8015EDB8 += 1;
        }
        if (D_8015EDB8 >= 0xE) {
            func_8014EBA8();
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80148714);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80148B08);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80148F70);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801495DC);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801496F0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801498C0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80149AA8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80149C5C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014A16C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014A3DC);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014A660);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014AB84);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014AC74);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014ADD0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014B294);

void func_8014B738(s32 arg0, s32 arg1, s32 arg2) {
    k_sub_reset();
    D_8015E740 = arg0;
    D_8015E744 = arg2;
    D_8015E748 = arg1;
    D_8015EE44 = 0;
    D_8015EE4C = 0;
    D_8015EE48 = 1;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014B79C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014BAA4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014BB80);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014BFE4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014C360);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014C4FC);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014C804);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014CB0C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014CE14);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014D11C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014D380);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014DF60);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014E1A4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014E28C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014E300);

void func_8014E5B4(s32 arg0) {
    switch (arg0) {
    case 0:
        D_8015EC28 = 0x78;
        D_8015EC2C = 0x68;
        return;
    case 1:
        D_8015EC28 = 0x40;
        D_8015EC2C = 0x58;
        return;
    case 2:
        D_8015EC28 = 0x38;
        D_8015EC2C = 0x88;
        return;
    case 3:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014E64C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014E780);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014E8F4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014E994);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014EB58);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014EBA8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014EBD0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014EBEC);

void func_8014ED44(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_8015EDB8 + 1;
    D_8015EDB8 = temp_v0;
    if (temp_v0 >= arg0) {
        func_8014EBA8();
    }
}

void func_8014ED80(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_8015EDB8 + 1;
    D_8015EDB8 = temp_v0;
    if (temp_v0 >= arg0) {
        func_8014EBD0();
    }
}

void func_8014EDBC(s32 arg0) {
    if (!(*(&D_80120652 + (arg0 * 0x44)) & 1)) {
        func_8014EBA8();
    }
}

void func_8014EDFC(s32 arg0) {
    if (!(*(&D_80120652 + (arg0 * 0x44)) & 1)) {
        func_8014EBD0();
    }
}

void func_8014EE3C(s32 arg0) {
    if (D_8015EE48 == 0) {
        func_8014EBA8();
    }
    if (arg0 != 0) {
        if ((D_8015EE44 - 3) >= arg0) {
            func_8014EBA8();
        }
    }
}

void func_8014EE98(s32 arg0) {
    if (D_8015EE48 == 0) {
        func_8014EBD0();
    }
    if (arg0 != 0) {
        if ((D_8015EE44 - 3) >= arg0) {
            func_8014EBD0();
        }
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014EEF4);

void func_8014EF3C(void) {
    D_8015EDD8++;
    D_8015EDE0 = 0;
    D_8015EDDC = 0;
    D_8015EDE4 = 0;
}

void func_8014EF6C(void) {
    D_8015EDDC++;
    D_8015EDE4 = 0;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014EF8C);

void func_8014F080(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_8015EDE4 + 1;
    D_8015EDE4 = temp_v0;
    if (temp_v0 >= arg0) {
        func_8014EF3C();
    }
}

void func_8014F0BC(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_8015EDE4 + 1;
    D_8015EDE4 = temp_v0;
    if (temp_v0 >= arg0) {
        func_8014EF6C();
    }
}

void func_8014F0F8(s32 arg0) {
    if (!(*(&D_80120652 + (arg0 * 0x44)) & 1)) {
        func_8014EF3C();
    }
}

void func_8014F138(s32 arg0) {
    if (!(*(&D_80120652 + (arg0 * 0x44)) & 1)) {
        func_8014EF6C();
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014F178);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014F1D4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014F230);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014F258);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014F278);

void func_8014F350(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_8015EE10 + 1;
    D_8015EE10 = temp_v0;
    if (temp_v0 >= arg0) {
        func_8014F230();
    }
}

void func_8014F38C(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_8015EE10 + 1;
    D_8015EE10 = temp_v0;
    if (temp_v0 >= arg0) {
        func_8014F258();
    }
}

void func_8014F3C8(s32 arg0) {
    if (!(*(&D_80120652 + (arg0 * 0x44)) & 1)) {
        func_8014F230();
    }
}

void func_8014F408(s32 arg0) {
    if (!(*(&D_80120652 + (arg0 * 0x44)) & 1)) {
        func_8014F258();
    }
}

void func_8014F448(s32 arg0) {
    if (D_8015EE48 == 0) {
        func_8014F230();
    }
    if (arg0 != 0) {
        if ((D_8015EE44 - 3) >= arg0) {
            func_8014F230();
        }
    }
}

void func_8014F4A4(s32 arg0) {
    if (D_8015EE48 == 0) {
        func_8014F258();
    }
    if (arg0 != 0) {
        if ((D_8015EE44 - 3) >= arg0) {
            func_8014F258();
        }
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014F500);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014F524);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014F540);

void func_8014F5DC(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_8015EE3C + 1;
    D_8015EE3C = temp_v0;
    if (temp_v0 >= arg0) {
        func_8014F500();
    }
}

void func_8014F618(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_8015EE3C + 1;
    D_8015EE3C = temp_v0;
    if (temp_v0 >= arg0) {
        func_8014F524();
    }
}

void func_8014F654(s32 arg0) {
    if (!(*(&D_80120652 + (arg0 * 0x44)) & 1)) {
        func_8014F500();
    }
}

void func_8014F694(s32 arg0) {
    if (!(*(&D_80120652 + (arg0 * 0x44)) & 1)) {
        func_8014F524();
    }
}

void func_8014F6D4(s32 arg0) {
    if (D_8015EE48 == 0) {
        func_8014F500();
    }
    if (arg0 != 0) {
        if ((D_8015EE44 - 3) >= arg0) {
            func_8014F500();
        }
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014F730);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014F790);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014F88C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8014FDBC);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80150120);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801504E0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801505B8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80150AA4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80150B90);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80150CD8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80150DE0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80150F84);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80151108);

void func_80151380(s32 arg0) {
    s32 temp_v0;

    D_8012146B = arg0;
    D_801214AF = arg0;
    D_801214F3 = arg0;
    if (arg0 < 0x80) {
        temp_v0 = 0x80 - arg0;
        D_80121603 = temp_v0;
        D_80121647 = temp_v0;
        D_8012168B = temp_v0;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801513C8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801515F0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801516E4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80151984);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80151C10);

void func_80151F38(void) {
    s32 temp_v0;

    temp_v0 = func_8013E9A8(3);
    switch (temp_v0) {
    case 0:
        func_8013F15C(0x50E, 1, 0);
        return;
    case 1:
        func_8013F15C(0x50F, 1, 0);
        return;
    case 2:
        func_8013F15C(0x510, 1, 0);
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80151FC0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80152080);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80152384);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801525B0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801528DC);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801529B4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80152F30);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801535E4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80153784);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80153A60);

void func_80153B08(void) {
    s32 temp_s1;
    s32 temp_s3;
    s32 var_s0;

    var_s0 = 0;
    do {
        temp_s1 = var_s0 + 4;
        temp_s3 = (s32) (func_800A0070(((s32) ((var_s0 % (s32) D_8015EC44) << 0xC) / (s32) D_8015EC44) + ((s32) (D_8015EC38 << 0xC) / (s32) D_8015EC40)) * D_8015EC3C) >> 0xD;
        func_80153F3C(temp_s3, (s32) (func_800A0070(((s32) ((temp_s1 % (s32) D_8015EC44) << 0xC) / (s32) D_8015EC44) + ((s32) (D_8015EC38 << 0xC) / (s32) D_8015EC40)) * D_8015EC3C) >> 0xD, var_s0, temp_s1, 0, 5);
        var_s0 += 5;
    } while (var_s0 != 0xAA);
}

void func_80153D20(void) {
    s32 temp_s1;
    s32 temp_s3;
    s32 var_s0;

    var_s0 = 0;
    do {
        temp_s1 = var_s0 + 4;
        temp_s3 = (s32) (func_800A0070(((s32) ((var_s0 % (s32) D_8015ED34) << 0xC) / (s32) D_8015ED34) + ((s32) (D_8015ED28 << 0xC) / (s32) D_8015ED30)) * D_8015ED2C) >> 0xD;
        func_80153F3C(temp_s3, (s32) (func_800A0070(((s32) ((temp_s1 % (s32) D_8015ED34) << 0xC) / (s32) D_8015ED34) + ((s32) (D_8015ED28 << 0xC) / (s32) D_8015ED30)) * D_8015ED2C) >> 0xD, var_s0, temp_s1, 2, 5);
        var_s0 += 5;
    } while (var_s0 != 0xAA);
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80153F3C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80154220);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80154430);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8015461C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80154760);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80154DD8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8015528C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80155868);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80155A70);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80155CC0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80155E80);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80156064);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801563A0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801564F0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8015675C);

s32 func_80156870(void) {
    if ((u32)(get_g_zyotai_s(6) & 0x7F) < 2) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801568A4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80156CC0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80156D78);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80157158);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80157640);

void func_8015785C(void) {
    func_8013E97C(0x2F, D_8015EC38, D_8015EC3C);
    func_8013E97C(0x2E, D_8015EC38, D_8015EC3C);
    func_8013E97C(0x2A, D_8015EC38 - 0x67, D_8015EC3C - 6);
    func_8013E97C(0x2B, D_8015EC38 - 0x43, D_8015EC3C + 6);
    func_8013E97C(0x2C, D_8015EC38 + 0x43, D_8015EC3C + 6);
    func_8013E97C(0x2D, D_8015EC38 + 0x67, D_8015EC3C - 6);
    if (!(D_8012117A & 1)) {
        func_8013E810(0x2A, 0xFF, 1, 0);
        D_8015EC40 = 0;
    }
    if (!(D_801211BE & 1)) {
        func_8013E810(0x2B, 0xFF, 1, 0);
        D_8015EC44 = 0;
    }
    if (!(D_80121202 & 1)) {
        func_8013E810(0x2C, 0xFF, 1, 0);
        D_8015EC48 = 0;
    }
    if (!(D_80121246 & 1)) {
        func_8013E810(0x2D, 0xFF, 1, 0);
        D_8015EC4C = 0;
    }
    if ((D_8015EC40 == 0) && (func_8013E9A8(0x64) == 0)) {
        func_8013E810(0x2A, 7, 5, 1);
        D_8015EC40 = 1;
    }
    if ((D_8015EC44 == 0) && (func_8013E9A8(0x64) == 0)) {
        func_8013E810(0x2B, 7, 5, 1);
        D_8015EC44 = 1;
    }
    if ((D_8015EC48 == 0) && (func_8013E9A8(0x64) == 0)) {
        func_8013E810(0x2C, 7, 5, 1);
        D_8015EC48 = 1;
    }
    if (D_8015EC4C == 0) {
        if (func_8013E9A8(0x64) == 0) {
            func_8013E810(0x2D, 7, 5, 1);
            D_8015EC4C = 1;
        }
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80157AE0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80157BA4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80157CCC);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80157E20);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80157F48);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801580C4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80158240);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8015831C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801583F8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801584D4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801585B0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8015868C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801587B4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80158930);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80158A0C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80158B70);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80158C4C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8015906C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80159214);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80159620);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_801596E8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80159B40);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80159D50);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_80159E28);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8015A11C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8015A1F0);

void func_8015A6D4(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        func_8013E810(i + 0x28, 7, 1, 1);
        func_8013E97C(i + 0x28, -0x19, 0x109);
    }
    func_8014EBA8();
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8015A744);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT", func_8015A8B4);
