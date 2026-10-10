#include "common.h"
#include "ovl/SHOUGATU.h"

/* A table of script-step functions copied from .data into a local and called by index. */
typedef struct FnTbl {
    void (*f[24])(void);
} FnTbl;

extern FnTbl D_80144E20;

void func_80137C28(void) {
    FnTbl tbl;

    tbl = D_80144E20;
    tbl.f[D_800E738A]();
}
