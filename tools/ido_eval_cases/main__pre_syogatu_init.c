#include "common.h"
#include "game.h"
#include "main_only.h"

void func_80061044();
void pre_syogatu_init1();
void pre_syogatu_init2();
void pre_syogatu_init3();

void pre_syogatu_init(void) {
    D_800E7384 += 1;
    switch (D_800E738A) {                           /* irregular */
    case 0:
        func_80061044();
        return;
    case 1:
        pre_syogatu_init1();
        return;
    case 2:
        pre_syogatu_init2();
        return;
    case 3:
        pre_syogatu_init3();
        return;
    }
}
