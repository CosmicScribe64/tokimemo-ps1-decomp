#include "common.h"
#include "ovl/BUNKA_SD.h"

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801359B0", func_801359B0);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801359B0", func_80135A2C);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801359B0", func_80135B44);

void func_80135C3C(void) {
    func_8004500C(1, 0x201);
    func_80135CD4();
    func_8004284C();
}

void func_80135C70(void) {
    u32 t;

    func_80135F38();
    func_801361B8();
    t = D_800E6280.unk_1104.u + 1;
    D_800E6280.unk_1104.u = t;
    if (t >= 0x21) {
        if (!(D_80120652 & 1)) {
            func_8004284C();
        }
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801359B0", func_80135CD4);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801359B0", func_80135F38);

void func_801361B8(void) {
    if (D_800E6280.unk_1104.u == 0) {
        D_80120652 = 5;
        D_80120696 = 5;
        D_801206DA = 5;
    }
    if (D_800E6280.unk_1104.u == 0x3A6) {
        D_8012071F = 0x80;
        D_8012071E = 5;
        D_80120723 = 0x80;
        D_80120763 = 0x80;
        D_80120762 = 5;
        D_80120767 = 0x80;
    }
    if (!(D_8012071E & 1)) {
        D_8012071F = 0;
        D_80120763 = 0;
    }
}
