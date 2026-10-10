#include "common.h"
#include "game.h"
#include "main_only.h"

void func_80046318();
extern s8 D_800B5BC8;
extern u8 D_800E7312;

void func_8005A560(void) {
    if (D_800E738D == 0) {
        if (func_80044E8C() == 1) {
            func_8004500C(0, 0);
            D_800E738D += 1;
        }
    } else if (D_800E738D == 1) {
        func_80046318(0x95, 0x801A0000, 0x9B94);
        D_800E738D += 1;
    } else if (D_800E738D == 2) {
        if (((u8 (*)())func_800460CC)(0x95) & 1) {
            func_80068938(D_800E62BE, D_800E62BF, 0);
            D_800E7312 &= 3;
            D_800B5BC8 = 0xFE;
            if (D_800B5C08 < 0xE) {
                func_800676AC(D_800B5C08);
                func_8006764C(1);
            }
            func_8004284C();
        }
    }
}
