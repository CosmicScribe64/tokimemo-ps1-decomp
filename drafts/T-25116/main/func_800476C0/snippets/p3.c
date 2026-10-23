#include "p0.h"
void f(s32 n, s32 slot) {
    s32 i, idx;
    for (i = 0; i < n; i++) {
        idx = ((s16 **)((R *)RB)[i].p10)[slot][1];
        if (idx) g(idx);
        if (((R *)RB)[i].b3 != 0) {
            ((R *)RB)[i].h8++;
        }
        if (((R *)RB)[i].h8 >= ((s16 **)((R *)RB)[i].p10)[slot][2]) {
            ((R *)RB)[i].h18++;
        }
    }
}
