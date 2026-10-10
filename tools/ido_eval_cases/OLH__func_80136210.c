#include "common.h"
#include "ovl/OLH.h"

void func_8004E788();
void func_8004EAAC();
void func_80052000();
extern u8 *D_8013CEB0;
extern u8 D_80138E10[];

void func_80136210(void) {
    switch (D_800E738D) {                           /* irregular */
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013CEB0, 0);
        func_8004E788(-0x88, -0x30, 0xF, D_80138E10, 0);
        D_800E738D += 1;
        return;
    case 1:
        func_80052000();
        return;
    case 2:
        func_80042940(0);
        return;
    }
}
