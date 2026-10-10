#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014E780", func_8014E780);

void func_8014E8F4(void) {
    func_80048390();
    func_800573AC();
    func_8004E58C();
    func_80048EB8(0);
    func_8006BC28(0);
    func_8006BD6C(0);
    func_800438F0(1);
    D_80122CE0 = D_8015EC14;
    D_800E6280.unk_F5F = (s8) D_8015EC18;
    func_80042878((s32) D_800E6280.unk_720);
    func_80042908((s32) D_800E6280.unk_721);
    func_80042940((s32) D_800E6280.unk_722);
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014E780", func_8014E994);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014E780", func_8014EB58);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014E780", func_8014EBA8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014E780", func_8014EBD0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014E780", func_8014EBEC);

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

void func_8014EEF4(void) {
    s32 i;

    for (i = 0x28; i < 0x30; i++) {
        func_8013E97C(i, 0, 0);
    }
}

void func_8014EF3C(void) {
    D_8015EDD8++;
    D_8015EDE0 = 0;
    D_8015EDDC = 0;
    D_8015EDE4 = 0;
}

/* no prototype: func_8014F1D4 calls it with an argument */
void func_8014EF6C() {
    D_8015EDDC++;
    D_8015EDE4 = 0;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014E780", func_8014EF8C);

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

void func_8014F178(s32 arg0) {
    if (D_8015EE48 == 0) {
        func_8014EF3C();
    }
    if (arg0 != 0) {
        if ((D_8015EE44 - 3) >= arg0) {
            func_8014EF3C();
        }
    }
}

void func_8014F1D4(s32 arg0) {
    if (D_8015EE48 == 0) {
        func_8014EF6C();
    }
    if (arg0 != 0) {
        if ((D_8015EE44 - 3) >= arg0) {
            func_8014EF6C();
        }
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014E780", func_8014F230);

void func_8014F258(void) {
    D_8015EE08++;
    D_8015EE10 = 0;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014E780", func_8014F278);

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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014E780", func_8014F500);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014E780", func_8014F524);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014E780", func_8014F540);

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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014E780", func_8014F730);
