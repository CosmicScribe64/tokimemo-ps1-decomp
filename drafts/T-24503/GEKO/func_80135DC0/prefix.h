/* FAKE: T-24503 implicit-int strcpy ABI view preserves the original
 * return-register allocation. The shared API's void return remains unchanged. */
#define MAIN_API_OVERRIDE_func_800AE0F0 /* original implicit-int call ABI */
#include "common.h"
extern s32 func_800AE0F0();
