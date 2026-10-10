#include "common.h"
#include "game.h"
#include "main_only.h"

void func_800625F4();

void func_80062634(void) {
    D_800E7384 += 1;
    switch (D_800E738A) {                           /* irregular */
    case 0:
        func_80062210();
        return;
    case 1:
        func_80062524();
        return;
    case 2:
        func_800625F4();
        return;
    }
}
