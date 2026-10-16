#include "common.h"
#include "ovl/TACO.h"

void *func_8015D0D0(s32 arg0, void *arg1) {
    s16 temp_t2;
    s16 temp_t3;
    s16 temp_t4;
    s16 temp_t5;
    s16 temp_t6;
    s16 temp_t7_2;
    s16 var_t1;
    s32 temp_t7;
    s32 temp_t9;
    s32 temp_t9_2;
    s32 var_t0;

    if (arg0 < 0) {
        temp_t7 = -arg0;
        if (temp_t7 < 0) {

        }
        temp_t9 = D_800DE500[temp_t7 & 0xFFF];
        var_t1 = -(s16) temp_t9;
        var_t0 = temp_t9 >> 0x10;
    } else {
        temp_t9_2 = D_800DE500[arg0 & 0xFFF];
        var_t1 = (s16) temp_t9_2;
        var_t0 = temp_t9_2 >> 0x10;
    }
    temp_t2 = *(s16 *)((unsigned char *)arg1 + (6));
    temp_t5 = *(s16 *)((unsigned char *)arg1 + (12));
    temp_t3 = *(s16 *)((unsigned char *)arg1 + (8));
    temp_t6 = *(s16 *)((unsigned char *)arg1 + (14));
    temp_t4 = *(s16 *)((unsigned char *)arg1 + (10));
    temp_t7_2 = *(s16 *)((unsigned char *)arg1 + (16));
    *(s16 *)((unsigned char *)arg1 + (6)) = (s16) ((s32) ((var_t0 * temp_t2) - (var_t1 * temp_t5)) >> 0xC);
    *(s16 *)((unsigned char *)arg1 + (8)) = (s16) ((s32) ((var_t0 * temp_t3) - (var_t1 * temp_t6)) >> 0xC);
    *(s16 *)((unsigned char *)arg1 + (10)) = (s16) ((s32) ((var_t0 * temp_t4) - (var_t1 * temp_t7_2)) >> 0xC);
    *(s16 *)((unsigned char *)arg1 + (12)) = (s16) ((s32) ((var_t1 * temp_t2) + (var_t0 * temp_t5)) >> 0xC);
    *(s16 *)((unsigned char *)arg1 + (14)) = (s16) ((s32) ((var_t1 * temp_t3) + (var_t0 * temp_t6)) >> 0xC);
    *(s16 *)((unsigned char *)arg1 + (16)) = (s16) ((s32) ((var_t1 * temp_t4) + (var_t0 * temp_t7_2)) >> 0xC);
    return arg1;
}
