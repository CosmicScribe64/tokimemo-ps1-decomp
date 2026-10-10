#include "common.h"
#include "ovl/TEL.h"

extern u8 D_800E738A;
u8 func_8013AA30();
u8 func_8013B54C();
u8 func_8013BF50();

u8 func_8013A40C(void) {
    switch (D_800E738A) {                           /* irregular */
    case 0:
        return func_8013AA30();
    case 1:
        return func_8013B54C();
    case 2:
        return func_8013BF50();
    default:
        return D_800E738A;
    }
}
